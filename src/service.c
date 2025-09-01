/*
 * Copyright 2021-2025 Robert Tari <robert@tari.in>
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

#define HWKBD FALSE
#define OSK   TRUE

static guint m_nSignal = 0;
static void *m_pLibHandle = NULL;
static Keyboard* (*m_fnKeyboardNew)();
static void (*m_fnKeyboardAddSource)(Keyboard *pKeyboard);
static guint (*m_fnKeyboardGetNumLayouts)(Keyboard *pKeyboard, gboolean bOSK);
static guint (*m_fnKeyboardGetLayoutIndex)(Keyboard *pKeyboard);
static void (*m_fnKeyboardGetLayout)(Keyboard *pKeyboard, gboolean bOSK, gint nLayout, gchar **pLanguage, gchar **pDescription, gchar **pId);
static void (*m_fnKeyboardSetLayout)(Keyboard *pKeyboard, gint nLayout, gboolean bOSK);
static gboolean (*m_fnKeyboardHasHardwareKeyboard)(Keyboard *pKeyboard);
static gboolean (*m_fnKeyboardHasSoftwareKeyboard)(Keyboard *pKeyboard);

enum
{
    SECTION_HEADER = (1 << 0),
    SECTION_LAYOUTS = (1 << 1),
    SECTION_DISPLAY = (1 << 2),
    SECTION_SETTINGS = (1 << 3)
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
    GSimpleAction *pHeaderAction;
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
    GSimpleAction *pSettingsAction;
    GSimpleAction *pDisplayAction;
    GSimpleAction *pLayoutAction;
    GSimpleAction *pOSKLayoutAction;
    GMenu *pLayoutSection;
    Keyboard *pKeyboard;
    GSettings *pSettings;
    GSettings *pLomiriSettings;
    gboolean bLomiri;
};

typedef IndicatorKeyboardServicePrivate priv_t;

G_DEFINE_TYPE_WITH_PRIVATE(IndicatorKeyboardService, indicator_keyboard_service, G_TYPE_OBJECT)

static GVariant* createHeaderState(IndicatorKeyboardService *self, int nProfile)
{
    GVariantBuilder cBuilder;
    g_variant_builder_init(&cBuilder, G_VARIANT_TYPE("a{sv}"));
    g_variant_builder_add(&cBuilder, "{sv}", "title", g_variant_new_string(_("Keyboard")));
    g_variant_builder_add(&cBuilder, "{sv}", "tooltip", g_variant_new_string(_("Keyboard layout switcher and settings")));
    g_variant_builder_add(&cBuilder, "{sv}", "visible", g_variant_new_boolean(TRUE));

    gchar *sKey = NULL;

    if (nProfile == PROFILE_DESKTOP)
    {
        sKey = "language-icon-desktop";
    }
    else if (nProfile == PROFILE_PHONE)
    {
        sKey = "language-icon-phone";
    }
    else if (nProfile == PROFILE_GREETER)
    {
        sKey = "language-icon-greeter";
    }

    gboolean bLayout = g_settings_get_boolean (self->pPrivate->pSettings, sKey);
    GIcon *pIcon = NULL;

    if (bLayout == FALSE)
    {
        pIcon = g_themed_icon_new_with_default_fallbacks(ICON_DEFAULT);
    }
    else
    {
        gboolean bHardwareKeyboard = m_fnKeyboardHasHardwareKeyboard (self->pPrivate->pKeyboard);
        gchar *sLanguage;
        m_fnKeyboardGetLayout(self->pPrivate->pKeyboard, self->pPrivate->bLomiri && !bHardwareKeyboard, -1, &sLanguage, NULL, NULL);

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

static GMenuModel* createLayoutSection(IndicatorKeyboardService *self, gboolean bOSK)
{
    self->pPrivate->pLayoutSection = g_menu_new();
    gboolean bCreate = TRUE;

    if (self->pPrivate->bLomiri)
    {
        if (!bOSK)
        {
            gboolean bHardwareKeyboard = m_fnKeyboardHasHardwareKeyboard (self->pPrivate->pKeyboard);

            if (bHardwareKeyboard)
            {
                g_menu_append (self->pPrivate->pLayoutSection, _("External Keyboard"), NULL);
            }
            else
            {
                bCreate = FALSE;
            }
        }
        else if (bOSK)
        {
            gboolean bSoftwareKeyboard = m_fnKeyboardHasSoftwareKeyboard (self->pPrivate->pKeyboard);
            g_menu_append (self->pPrivate->pLayoutSection, _("On-Screen Keyboard"), NULL);

            if (!bSoftwareKeyboard)
            {
                bCreate = FALSE;
            }
        }
    }
    else if (bOSK)
    {
        bCreate = FALSE;
    }
    if (!bCreate)
    {
        return G_MENU_MODEL(self->pPrivate->pLayoutSection);
    }

    guint nLayouts = m_fnKeyboardGetNumLayouts(self->pPrivate->pKeyboard, bOSK);

    for (guint nLayout = 0; nLayout < nLayouts; nLayout++)
    {
        gchar *sLanguage;
        gchar *sDescription;
        m_fnKeyboardGetLayout(self->pPrivate->pKeyboard, bOSK, nLayout, &sLanguage, &sDescription, NULL);
        GMenuItem *pItem = g_menu_item_new(sDescription, NULL);
        g_free(sDescription);
        gchar *sAction = NULL;

        if (bOSK)
        {
            sAction = "indicator.osklayout";
        }
        else
        {
            sAction = "indicator.layout";
        }

        g_menu_item_set_action_and_target_value(pItem, sAction, g_variant_new_byte(nLayout));
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

static GMenuModel* createSettingsSection(IndicatorKeyboardService *self, gboolean bOSK)
{
    GMenu * pMenu = g_menu_new();
    gboolean bUbuntuTouch = ayatana_common_utils_is_ubuntutouch ();

    if (self->pPrivate->bLomiri && bOSK && !bUbuntuTouch)
    {
        GMenuItem *pItem = g_menu_item_new (_("Always show OSK"), "indicator.osk(true)");
        g_menu_item_set_attribute (pItem, "x-ayatana-type", "s", "org.ayatana.indicator.switch");
        g_menu_append_item (pMenu, pItem);
        g_object_unref (pItem);
    }

    gchar *sAction = NULL;

    if (self->pPrivate->bLomiri)
    {
        if (!bOSK)
        {
            gboolean bHardwareKeyboard = m_fnKeyboardHasHardwareKeyboard (self->pPrivate->pKeyboard);

            if (bHardwareKeyboard)
            {
                sAction = "indicator.settings";
            }
        }
        else if (bOSK)
        {
            sAction = "indicator.osksettings";
        }
    }
    else if (!bOSK)
    {
        sAction = "indicator.settings";
    }

    if (sAction)
    {
        g_menu_append(pMenu, _("Keyboard Settings…"), sAction);
    }

    return G_MENU_MODEL(pMenu);
}

static GMenuModel* createDisplaySection (IndicatorKeyboardService *self)
{
    GMenu * pMenu = g_menu_new ();
    gboolean bDisplay = TRUE;

    if (self->pPrivate->bLomiri)
    {
        gboolean bHardwareKeyboard = m_fnKeyboardHasHardwareKeyboard (self->pPrivate->pKeyboard);

        if (!bHardwareKeyboard)
        {
            bDisplay = FALSE;
        }
    }

    if (bDisplay)
    {
        g_menu_append (pMenu, _("Show Current Layout"), "indicator.display");
    }

    return G_MENU_MODEL (pMenu);
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
    struct ProfileMenuInfo *pInfoPhone = &self->pPrivate->lMenus[PROFILE_PHONE];
    struct ProfileMenuInfo *pInfoGreeter = &self->pPrivate->lMenus[PROFILE_GREETER];

    if (nSections & SECTION_HEADER)
    {
        g_simple_action_set_state(pInfoDesktop->pHeaderAction, createHeaderState(self, PROFILE_DESKTOP));
        g_simple_action_set_state(pInfoPhone->pHeaderAction, createHeaderState(self, PROFILE_PHONE));
        g_simple_action_set_state(pInfoGreeter->pHeaderAction, createHeaderState(self, PROFILE_GREETER));
    }

    if (!self->pPrivate->bMenusBuilt)
    {
        return;
    }

    if (nSections & SECTION_LAYOUTS)
    {
        rebuildSection(pInfoDesktop->pSubmenu, 0, createLayoutSection(self, HWKBD));
        rebuildSection(pInfoDesktop->pSubmenu, 3, createLayoutSection(self, OSK));
        rebuildSection(pInfoPhone->pSubmenu, 0, createLayoutSection(self, HWKBD));
        rebuildSection(pInfoPhone->pSubmenu, 2, createLayoutSection(self, OSK));
        rebuildSection(pInfoGreeter->pSubmenu, 0, createLayoutSection(self, HWKBD));
        rebuildSection(pInfoGreeter->pSubmenu, 1, createLayoutSection(self, OSK));
    }

    if (nSections & SECTION_DISPLAY)
    {
        rebuildSection (pInfoDesktop->pSubmenu, 1, createDisplaySection (self));
    }

    if (nSections & SECTION_SETTINGS)
    {
        rebuildSection(pInfoDesktop->pSubmenu, 2, createSettingsSection(self, HWKBD));
        rebuildSection(pInfoDesktop->pSubmenu, 4, createSettingsSection(self, OSK));
        rebuildSection(pInfoPhone->pSubmenu, 1, createSettingsSection(self, HWKBD));
        rebuildSection(pInfoPhone->pSubmenu, 3, createSettingsSection(self, OSK));
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
        lSections[nSection++] = createLayoutSection(self, HWKBD);
        lSections[nSection++] = createSettingsSection(self, HWKBD);
        lSections[nSection++] = createLayoutSection(self, OSK);
        lSections[nSection++] = createSettingsSection(self, OSK);
    }
    else if (nProfile == PROFILE_DESKTOP)
    {
        lSections[nSection++] = createLayoutSection(self, HWKBD);
        lSections[nSection++] = createDisplaySection(self);
        lSections[nSection++] = createSettingsSection(self, HWKBD);
        lSections[nSection++] = createLayoutSection(self, OSK);
        lSections[nSection++] = createSettingsSection(self, OSK);
    }
    else if (nProfile == PROFILE_GREETER)
    {
        lSections[nSection++] = createLayoutSection(self, HWKBD);
        lSections[nSection++] = createLayoutSection(self, OSK);
    }

    // Add sections to the submenu
    pSubmenu = g_menu_new();

    for (guint i = 0; i < nSection; ++i)
    {
        g_menu_append_section(pSubmenu, NULL, lSections[i]);
        g_object_unref(lSections[i]);
    }

    // Add submenu to the header
    gchar *sName = g_strdup_printf ("indicator._header-%s", m_lMenuNames[nProfile]);
    pItem = g_menu_item_new(NULL, sName);
    g_free (sName);
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
    m_fnKeyboardSetLayout(self->pPrivate->pKeyboard, nLayout, HWKBD);
}

static void onOSKLayoutSelected (GSimpleAction *pAction, GVariant *pVariant, gpointer pData)
{
    IndicatorKeyboardService *self = INDICATOR_KEYBOARD_SERVICE (pData);
    const guint8 nLayout = g_variant_get_byte (pVariant);
    m_fnKeyboardSetLayout (self->pPrivate->pKeyboard, nLayout, OSK);
}

static void onSettings(GSimpleAction *pAction, GVariant *pVariant, gpointer pData)
{
    IndicatorKeyboardService *self = INDICATOR_KEYBOARD_SERVICE (pData);

    if (ayatana_common_utils_is_mate())
    {
        ayatana_common_utils_execute_command("mate-keyboard-properties");
    }
    else if (self->pPrivate->bLomiri)
    {
        ayatana_common_utils_open_url("settings:///system/hw-keyboard-layouts");
    }
}

static void onOSKSettings(GSimpleAction *pAction, GVariant *pVariant, gpointer pData)
{
    ayatana_common_utils_open_url ("settings:///system/sw-keyboard-layouts");
}

static void onDisplay (GSimpleAction *pAction, GVariant *pVariant, gpointer pData)
{
    IndicatorKeyboardService *self = INDICATOR_KEYBOARD_SERVICE (pData);
    guint nLayout = m_fnKeyboardGetLayoutIndex (self->pPrivate->pKeyboard);
    gchar *sProgram = NULL;
    gchar *sArgs = NULL;
    gboolean bMate = ayatana_common_utils_is_mate ();
    gboolean bLomiri = ayatana_common_utils_is_lomiri ();

    if (bMate)
    {
        sProgram = "matekbd-keyboard-display";
        sArgs = g_strdup_printf ("-g %i", nLayout + 1);
    }
    else if (bLomiri)
    {

        sProgram = "tecla";
        m_fnKeyboardGetLayout (self->pPrivate->pKeyboard, HWKBD, -1, NULL, NULL, &sArgs);
    }
    else
    {
        sProgram = "gkbd-keyboard-display";
        sArgs = g_strdup_printf ("-g %i", nLayout + 1);
    }

    ayatana_common_utils_execute_command_warn (sProgram, sArgs);
    g_free (sArgs);
}

static gboolean valueFromVariant (GValue *pValue, GVariant *pVariant, gpointer pUserData)
{
    g_value_set_variant (pValue, pVariant);

    return TRUE;
}

static GVariant* valueToVariant (const GValue *pValue, const GVariantType *pType, gpointer pUserData)
{
    GVariant *pVariant = g_value_dup_variant (pValue);

    return pVariant;
}

static void initActions(IndicatorKeyboardService *self)
{
    GSimpleAction *pAction;
    self->pPrivate->pActionGroup = g_simple_action_group_new();

    for (int nProfile = 0; nProfile < N_PROFILES; ++nProfile)
    {
        gchar *sName = g_strdup_printf ("_header-%s", m_lMenuNames[nProfile]);
        pAction = g_simple_action_new_stateful(sName, NULL, createHeaderState(self, nProfile));
        g_free (sName);
        g_action_map_add_action(G_ACTION_MAP(self->pPrivate->pActionGroup), G_ACTION(pAction));
        self->pPrivate->lMenus[nProfile].pHeaderAction = pAction;
    }

    pAction = g_simple_action_new("layout", G_VARIANT_TYPE_BYTE);
    g_action_map_add_action(G_ACTION_MAP(self->pPrivate->pActionGroup), G_ACTION(pAction));
    self->pPrivate->pLayoutAction = pAction;
    g_signal_connect(pAction, "activate", G_CALLBACK(onLayoutSelected), self);

    if (self->pPrivate->bLomiri)
    {
        pAction = g_simple_action_new("osklayout", G_VARIANT_TYPE_BYTE);
        g_action_map_add_action(G_ACTION_MAP(self->pPrivate->pActionGroup), G_ACTION(pAction));
        self->pPrivate->pOSKLayoutAction = pAction;
        g_signal_connect(pAction, "activate", G_CALLBACK(onOSKLayoutSelected), self);
    }

    if (self->pPrivate->bLomiri && (!ayatana_common_utils_is_ubuntutouch()))
    {
        gboolean bOsk = g_settings_get_boolean (self->pPrivate->pLomiriSettings, "always-show-osk");
        GVariant *pOsk = g_variant_new_boolean (bOsk);
        pAction = g_simple_action_new_stateful ("osk", G_VARIANT_TYPE_BOOLEAN, pOsk);
        g_settings_bind_with_mapping (self->pPrivate->pLomiriSettings, "always-show-osk", pAction, "state", G_SETTINGS_BIND_DEFAULT, valueFromVariant, valueToVariant, NULL, NULL);
        g_action_map_add_action (G_ACTION_MAP (self->pPrivate->pActionGroup), G_ACTION (pAction));
        g_object_unref (G_OBJECT (pAction));
    }

    pAction = g_simple_action_new("settings", NULL);
    g_action_map_add_action(G_ACTION_MAP(self->pPrivate->pActionGroup), G_ACTION(pAction));
    self->pPrivate->pSettingsAction = pAction;
    g_signal_connect(pAction, "activate", G_CALLBACK(onSettings), self);

    if (self->pPrivate->bLomiri)
    {
        pAction = g_simple_action_new ("osksettings", NULL);
        g_action_map_add_action(G_ACTION_MAP (self->pPrivate->pActionGroup), G_ACTION (pAction));
        self->pPrivate->pSettingsAction = pAction;
        g_signal_connect (pAction, "activate", G_CALLBACK (onOSKSettings), self);
    }

    pAction = g_simple_action_new ("display", NULL);
    g_action_map_add_action (G_ACTION_MAP (self->pPrivate->pActionGroup), G_ACTION (pAction));
    self->pPrivate->pDisplayAction = pAction;
    g_signal_connect (pAction, "activate", G_CALLBACK (onDisplay), self);
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

    if (self->pPrivate->pSettings != NULL)
    {
        g_signal_handlers_disconnect_by_data (self->pPrivate->pSettings, self);
        g_clear_object (&self->pPrivate->pSettings);
    }

    g_clear_object (&self->pPrivate->pLomiriSettings);

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
    g_clear_object (&self->pPrivate->pDisplayAction);
    g_clear_object (&self->pPrivate->pLayoutAction);
    g_clear_object (&self->pPrivate->pOSKLayoutAction);

    for (int nProfile = 0; nProfile < N_PROFILES; ++nProfile)
    {
        g_clear_object (&self->pPrivate->lMenus[nProfile].pHeaderAction);
    }

    g_clear_object (&self->pPrivate->pActionGroup);
    g_clear_object (&self->pPrivate->pConnection);

    if (m_pLibHandle)
    {
        dlclose(m_pLibHandle);
        m_pLibHandle = NULL;
    }

    G_OBJECT_CLASS(indicator_keyboard_service_parent_class)->dispose(pObject);
}

static void onSettingsChanged(GSettings *pSettings, gchar *sKey, gpointer pData)
{
    IndicatorKeyboardService *self = INDICATOR_KEYBOARD_SERVICE(pData);
    rebuildNow(self, SECTION_HEADER);
}

static void indicator_keyboard_service_init(IndicatorKeyboardService *self)
{
    gchar *sLib = "libayatana-keyboard-x11.so.0";
    gboolean bLomiri = ayatana_common_utils_is_lomiri ();

    if (bLomiri)
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
    m_fnKeyboardGetLayoutIndex = dlsym(m_pLibHandle, "keyboard_GetLayoutIndex");
    m_fnKeyboardGetLayout = dlsym(m_pLibHandle, "keyboard_GetLayout");
    m_fnKeyboardSetLayout = dlsym(m_pLibHandle, "keyboard_SetLayout");
    m_fnKeyboardHasHardwareKeyboard = dlsym(m_pLibHandle, "keyboard_hasHardwareKeyboard");
    m_fnKeyboardHasSoftwareKeyboard = dlsym(m_pLibHandle, "keyboard_hasSoftwareKeyboard");
    self->pPrivate = indicator_keyboard_service_get_instance_private(self);
    self->pPrivate->bLomiri = bLomiri;
    self->pPrivate->pCancellable = g_cancellable_new();
    self->pPrivate->pSettings = g_settings_new ("org.ayatana.indicator.keyboard");
    g_signal_connect(self->pPrivate->pSettings, "changed", G_CALLBACK(onSettingsChanged), self);

    if (self->pPrivate->bLomiri)
    {
        self->pPrivate->pLomiriSettings = g_settings_new ("com.lomiri.Shell");
    }

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
