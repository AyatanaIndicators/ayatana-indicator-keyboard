/*
 * Copyright 2021 Robert Tari <robert@tari.in>
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3, as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranties of
 * MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <glib/gi18n.h>
#include <gio/gio.h>
#include <ayatana/common/utils.h>
#include <dlfcn.h>
#include "service.h"

#define BUS_NAME "org.ayatana.indicator.keyboard"
#define BUS_PATH "/org/ayatana/indicator/keyboard"

#define ICON_DEFAULT "input-keyboard"

static guint m_nSignal = 0;
static void *m_pLibHandle = NULL;
static Keyboard* (*m_fnKeyboardNew)();
static void (*m_fnKeyboardAddSource)(Keyboard *pKeyboard);
static guint (*m_fnKeyboardGetNumLayouts)(Keyboard *pKeyboard);
static void (*m_fnKeyboardGetLayout)(Keyboard *pKeyboard, gint nLayout, gchar **pLanguage, gchar **pDescription);
static void (*m_fnKeyboardSetLayout)(Keyboard *pKeyboard, gint nLayout);

enum
{
    SECTION_HEADER = (1 << 0),
    SECTION_LAYOUTS = (1 << 1),
    SECTION_SETTINGS = (1 << 2)
};

enum
{
    PROFILE_PHONE,
    PROFILE_DESKTOP,
    PROFILE_GREETER,
    N_PROFILES
};

static const char * const m_lMenuNames[N_PROFILES] =
{
    "phone",
    "desktop",
    "greeter"
};

struct ProfileMenuInfo
{
    GMenu *pMenu;
    GMenu *pSubmenu;
    guint nExportId;
};

struct _IndicatorKeyboardServicePrivate
{
    GCancellable *pCancellable;
    guint nOwnId;
    guint nActionsId;
    GDBusConnection *pConnection;
    gboolean bMenusBuilt;
    struct ProfileMenuInfo lMenus[N_PROFILES];
    GSimpleActionGroup *pActionGroup;
    GSimpleAction *pHeaderAction;
    GSimpleAction *pSettingsAction;
    GSimpleAction *pLayoutAction;
    GMenu *pLayoutSection;
    Keyboard *pKeyboard;
};

typedef IndicatorKeyboardServicePrivate priv_t;

G_DEFINE_TYPE_WITH_PRIVATE(IndicatorKeyboardService, indicator_keyboard_service, G_TYPE_OBJECT)

static GVariant* createHeaderState(IndicatorKeyboardService *self)
{
    GVariantBuilder cBuilder;
    g_variant_builder_init(&cBuilder, G_VARIANT_TYPE("a{sv}"));
    g_variant_builder_add(&cBuilder, "{sv}", "title", g_variant_new_string(_("Keyboard")));
    g_variant_builder_add(&cBuilder, "{sv}", "visible", g_variant_new_boolean(TRUE));

    GIcon *pIcon = NULL;
    if (ayatana_common_utils_is_lomiri()) {
        pIcon = g_themed_icon_new_with_default_fallbacks(ICON_DEFAULT);
    }
    else
    {
        gchar *sLanguage;
        m_fnKeyboardGetLayout(self->pPrivate->pKeyboard, -1, &sLanguage, NULL);

        gchar *sIcon = g_strconcat("ayatana-indicator-keyboard-", sLanguage, NULL);
        g_free(sLanguage);

        pIcon = g_themed_icon_new_with_default_fallbacks(sIcon);
        g_free(sIcon);
    }

    g_variant_builder_add(&cBuilder, "{sv}", "accessible-desc", g_variant_new_string(_("Current keyboard layout")));

    if (pIcon)
    {
        GVariant *pSerialized = g_icon_serialize(pIcon);

        if (pSerialized != NULL)
        {
            g_variant_builder_add(&cBuilder, "{sv}", "icon", pSerialized);
            g_variant_unref(pSerialized);
        }

        g_object_unref(pIcon);
    }

    return g_variant_builder_end(&cBuilder);
}

static GMenuModel* createDesktopLayoutSection(IndicatorKeyboardService *self, int nProfile)
{
    self->pPrivate->pLayoutSection = g_menu_new();

    guint nLayouts = m_fnKeyboardGetNumLayouts(self->pPrivate->pKeyboard);

    for (guint nLayout = 0; nLayout < nLayouts; nLayout++)
    {
        gchar *sLanguage;
        gchar *sDescription;
        m_fnKeyboardGetLayout(self->pPrivate->pKeyboard, nLayout, &sLanguage, &sDescription);
        GMenuItem *pItem = g_menu_item_new(sDescription, NULL);
        g_free(sDescription);
        g_menu_item_set_action_and_target_value(pItem, "indicator.layout", g_variant_new_byte(nLayout));
        g_menu_item_set_attribute_value(pItem, "x-ayatana-layout", g_variant_new_byte(nLayout));
        gchar *sIcon = g_strconcat("ayatana-indicator-keyboard-", sLanguage, NULL);
        g_free(sLanguage);
        GIcon *pIcon = g_themed_icon_new_with_default_fallbacks(sIcon);
        g_free(sIcon);
        GVariant *pSerialized = g_icon_serialize(pIcon);

        if (pSerialized != NULL)
        {
            g_menu_item_set_attribute_value(pItem, G_MENU_ATTRIBUTE_ICON, pSerialized);
            g_variant_unref(pSerialized);
        }

        g_object_unref(pIcon);

        g_menu_append_item(self->pPrivate->pLayoutSection, pItem);
        g_object_unref(pItem);
    }

    return G_MENU_MODEL(self->pPrivate->pLayoutSection);
}

static GMenuModel* createDesktopSettingsSection(IndicatorKeyboardService *self)
{
    GMenu * pMenu = g_menu_new();
    g_menu_append(pMenu, _("Keyboard Settings…"), "indicator.settings");

    return G_MENU_MODEL(pMenu);
}

static void rebuildSection(GMenu *pMenu, int nPos, GMenuModel *pModel)
{
    g_menu_remove(pMenu, nPos);
    g_menu_insert_section(pMenu, nPos, NULL, pModel);
    g_object_unref(pModel);
}

static void rebuildNow(IndicatorKeyboardService *self, guint nSections)
{
    struct ProfileMenuInfo *pInfoDesktop = &self->pPrivate->lMenus[PROFILE_DESKTOP];
    struct ProfileMenuInfo *pInfoGreeter = &self->pPrivate->lMenus[PROFILE_GREETER];

    if (nSections & SECTION_HEADER)
    {
        g_simple_action_set_state(self->pPrivate->pHeaderAction, createHeaderState(self));
    }

    if (!self->pPrivate->bMenusBuilt)
    {
        return;
    }

    if (nSections & SECTION_LAYOUTS)
    {
        rebuildSection(pInfoDesktop->pSubmenu, 0, createDesktopLayoutSection(self, PROFILE_DESKTOP));
        rebuildSection(pInfoGreeter->pSubmenu, 0, createDesktopLayoutSection(self, PROFILE_GREETER));
    }

    if (nSections & SECTION_SETTINGS)
    {
        rebuildSection(pInfoDesktop->pSubmenu, 1, createDesktopSettingsSection(self));
    }
}

static void createMenu(IndicatorKeyboardService *self, int nProfile)
{
    GMenu *pMenu;
    GMenu *pSubmenu;
    GMenuItem *pItem;
    GMenuModel *lSections[16];
    guint nSection = 0;

    g_assert(0 <= nProfile && nProfile < N_PROFILES);
    g_assert(self->pPrivate->lMenus[nProfile].pMenu == NULL);

    // Build the sections
    if (nProfile == PROFILE_PHONE)
    {
        lSections[nSection++] = createDesktopLayoutSection(self, nProfile);
        lSections[nSection++] = createDesktopSettingsSection(self);
    }
    else if (nProfile == PROFILE_DESKTOP)
    {
        lSections[nSection++] = createDesktopLayoutSection(self, nProfile);
        lSections[nSection++] = createDesktopSettingsSection(self);
    }
    else if (nProfile == PROFILE_GREETER)
    {
        lSections[nSection++] = createDesktopLayoutSection(self, nProfile);
    }

    // Add sections to the submenu
    pSubmenu = g_menu_new();

    for (guint i = 0; i < nSection; ++i)
    {
        g_menu_append_section(pSubmenu, NULL, lSections[i]);
        g_object_unref(lSections[i]);
    }

    // Add submenu to the header
    pItem = g_menu_item_new(NULL, "indicator._header");
    g_menu_item_set_attribute(pItem, "x-ayatana-type", "s", "org.ayatana.indicator.root");
    g_menu_item_set_submenu(pItem, G_MENU_MODEL(pSubmenu));
    g_object_unref(pSubmenu);

    // Add header to the menu
    pMenu = g_menu_new();
    g_menu_append_item(pMenu, pItem);
    g_object_unref(pItem);

    self->pPrivate->lMenus[nProfile].pMenu = pMenu;
    self->pPrivate->lMenus[nProfile].pSubmenu = pSubmenu;
}

static void onLayoutChanged(Keyboard *pKeyboard, gpointer pData)
{
    IndicatorKeyboardService *self = INDICATOR_KEYBOARD_SERVICE(pData);
    rebuildNow(self, SECTION_HEADER);
}

static void onConfigChanged(Keyboard *pKeyboard, gpointer pData)
{
    IndicatorKeyboardService *self = INDICATOR_KEYBOARD_SERVICE(pData);
    rebuildNow(self, SECTION_LAYOUTS);
}

static void onLayoutSelected(GSimpleAction *pAction, GVariant *pVariant, gpointer pData)
{
    IndicatorKeyboardService *self = INDICATOR_KEYBOARD_SERVICE(pData);
    const guint8 nLayout = g_variant_get_byte(pVariant);
    m_fnKeyboardSetLayout(self->pPrivate->pKeyboard, nLayout);
}

static void onSettings(GSimpleAction *pAction, GVariant *pVariant, gpointer pUserData)
{
    if (ayatana_common_utils_is_mate())
    {
        ayatana_common_utils_execute_command("mate-keyboard-properties");
    }
    else if (ayatana_common_utils_is_lomiri())
    {
        ayatana_common_utils_open_url("settings:///system/hw-keyboard-layouts");
    }
}

static void initActions(IndicatorKeyboardService *self)
{
    GSimpleAction *pAction;
    self->pPrivate->pActionGroup = g_simple_action_group_new();

    pAction = g_simple_action_new_stateful("_header", NULL, createHeaderState(self));
    g_action_map_add_action(G_ACTION_MAP(self->pPrivate->pActionGroup), G_ACTION(pAction));
    self->pPrivate->pHeaderAction = pAction;

    pAction = g_simple_action_new("layout", G_VARIANT_TYPE_BYTE);
    g_action_map_add_action(G_ACTION_MAP(self->pPrivate->pActionGroup), G_ACTION(pAction));
    self->pPrivate->pLayoutAction = pAction;
    g_signal_connect(pAction, "activate", G_CALLBACK(onLayoutSelected), self);

    pAction = g_simple_action_new("settings", NULL);
    g_action_map_add_action(G_ACTION_MAP(self->pPrivate->pActionGroup), G_ACTION(pAction));
    self->pPrivate->pSettingsAction = pAction;
    g_signal_connect(pAction, "activate", G_CALLBACK(onSettings), self);
}

static void onBusAcquired(GDBusConnection *pConnection, const gchar *sName, gpointer pData)
{
    IndicatorKeyboardService *self = INDICATOR_KEYBOARD_SERVICE(pData);

    g_debug("bus acquired: %s", sName);

    self->pPrivate->pConnection = (GDBusConnection*)g_object_ref(G_OBJECT (pConnection));
    guint nId;
    GError *pError = NULL;

    // Export the actions
    if ((nId = g_dbus_connection_export_action_group(pConnection, BUS_PATH, G_ACTION_GROUP(self->pPrivate->pActionGroup), &pError)))
    {
        self->pPrivate->nActionsId = nId;
    }
    else
    {
        g_warning("cannot export action group: %s", pError->message);
        g_clear_error(&pError);
    }

    GString *pPath = g_string_new(NULL);

    // Export the menus
    for (int nProfile = 0; nProfile < N_PROFILES; ++nProfile)
    {
        struct ProfileMenuInfo *pInfo = &self->pPrivate->lMenus[nProfile];

        g_string_printf(pPath, "%s/%s", BUS_PATH, m_lMenuNames[nProfile]);

        if ((nId = g_dbus_connection_export_menu_model(pConnection, pPath->str, G_MENU_MODEL(pInfo->pMenu), &pError)))
        {
            pInfo->nExportId = nId;
        }
        else
        {
            g_warning("cannot export %s menu: %s", pPath->str, pError->message);
            g_clear_error (&pError);
        }
    }

    g_string_free(pPath, TRUE);
}

static void unexport(IndicatorKeyboardService *self)
{
    // Unexport the menus
    for (int nProfile = 0; nProfile < N_PROFILES; ++nProfile)
    {
        guint *nId = &self->pPrivate->lMenus[nProfile].nExportId;

        if (*nId)
        {
            g_dbus_connection_unexport_menu_model(self->pPrivate->pConnection, *nId);
            *nId = 0;
        }
    }

    // Unexport the actions
    if (self->pPrivate->nActionsId)
    {
        g_dbus_connection_unexport_action_group(self->pPrivate->pConnection, self->pPrivate->nActionsId);
        self->pPrivate->nActionsId = 0;
    }
}

static void onNameLost(GDBusConnection *pConnection, const gchar *sName, gpointer pData)
{
    IndicatorKeyboardService *self = INDICATOR_KEYBOARD_SERVICE(pData);

    g_debug("%s %s name lost %s", G_STRLOC, G_STRFUNC, sName);

    unexport(self);
}

static void onDispose(GObject *pObject)
{
    IndicatorKeyboardService *self = INDICATOR_KEYBOARD_SERVICE(pObject);

    if (self->pPrivate->pKeyboard != NULL)
    {
        g_object_unref(G_OBJECT(self->pPrivate->pKeyboard));
        self->pPrivate->pKeyboard = NULL;
    }

    if (self->pPrivate->nOwnId)
    {
        g_bus_unown_name(self->pPrivate->nOwnId);
        self->pPrivate->nOwnId = 0;
    }

    unexport(self);

    if (self->pPrivate->pCancellable != NULL)
    {
        g_cancellable_cancel(self->pPrivate->pCancellable);
        g_clear_object(&self->pPrivate->pCancellable);
    }

    g_clear_object (&self->pPrivate->pSettingsAction);
    g_clear_object (&self->pPrivate->pLayoutAction);
    g_clear_object (&self->pPrivate->pHeaderAction);
    g_clear_object (&self->pPrivate->pActionGroup);
    g_clear_object (&self->pPrivate->pConnection);

    if (m_pLibHandle)
    {
        dlclose(m_pLibHandle);
        m_pLibHandle = NULL;
    }

    G_OBJECT_CLASS(indicator_keyboard_service_parent_class)->dispose(pObject);
}

static void indicator_keyboard_service_init(IndicatorKeyboardService *self)
{
    gchar *sLib = "libayatana-keyboard-x11.so.0";

    if (ayatana_common_utils_is_lomiri())
    {
        sLib = "libayatana-keyboard-lomiri.so.0";
    }

    m_pLibHandle = dlopen(sLib, RTLD_NOW);

    if (!m_pLibHandle)
    {
        g_error("%s", dlerror());
    }

    m_fnKeyboardNew = dlsym(m_pLibHandle, "keyboard_new");

    gchar *sError = dlerror();

    if (sError != NULL)
    {
        g_error("%s", sError);
    }

    m_fnKeyboardAddSource = dlsym(m_pLibHandle, "keyboard_AddSource");
    m_fnKeyboardGetNumLayouts = dlsym(m_pLibHandle, "keyboard_GetNumLayouts");
    m_fnKeyboardGetLayout = dlsym(m_pLibHandle, "keyboard_GetLayout");
    m_fnKeyboardSetLayout = dlsym(m_pLibHandle, "keyboard_SetLayout");

    self->pPrivate = indicator_keyboard_service_get_instance_private(self);
    self->pPrivate->pCancellable = g_cancellable_new();
    self->pPrivate->pKeyboard = m_fnKeyboardNew();
    g_signal_connect(self->pPrivate->pKeyboard, KEYBOARD_LAYOUT_CHANGED, G_CALLBACK(onLayoutChanged), self);
    g_signal_connect(self->pPrivate->pKeyboard, KEYBOARD_CONFIG_CHANGED, G_CALLBACK(onConfigChanged), self);
    initActions(self);

    for (int nProfile = 0; nProfile < N_PROFILES; ++nProfile)
    {
        createMenu(self, nProfile);
    }

    self->pPrivate->bMenusBuilt = TRUE;
    self->pPrivate->nOwnId = g_bus_own_name(G_BUS_TYPE_SESSION, BUS_NAME, G_BUS_NAME_OWNER_FLAGS_ALLOW_REPLACEMENT, onBusAcquired, NULL, onNameLost, self, NULL);
}

static void indicator_keyboard_service_class_init(IndicatorKeyboardServiceClass *klass)
{
    GObjectClass *pClass = G_OBJECT_CLASS(klass);
    pClass->dispose = onDispose;
    m_nSignal = g_signal_new("name-lost", G_TYPE_FROM_CLASS(klass), G_SIGNAL_RUN_LAST, G_STRUCT_OFFSET(IndicatorKeyboardServiceClass, pNameLost), NULL, NULL, g_cclosure_marshal_VOID__VOID, G_TYPE_NONE, 0);
}

IndicatorKeyboardService *indicator_keyboard_service_new()
{
    GObject *pObject = g_object_new(INDICATOR_TYPE_KEYBOARD_SERVICE, NULL);

    return INDICATOR_KEYBOARD_SERVICE(pObject);
}

void indicator_keyboard_service_AddKeyboardSource(IndicatorKeyboardService *self)
{
    m_fnKeyboardAddSource(self->pPrivate->pKeyboard);
}
