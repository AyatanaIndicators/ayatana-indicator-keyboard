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

#include <act/act.h>
#include <xkbcommon/xkbregistry.h>
#include <glib-object.h>
#include <libudev.h>
#include "languages.h"
#include "keyboard.h"
#include "system-layouts.h"

gchar *LOMIRI_TO_ISO[][2] =
{
    {"ar", "ara"},
    {"bn", "bd"},
    {"bn-probhat", "bd+probhat"},
    {"bs", "ba"},
    {"cs", "cz"},
    {"da", "dk"},
    {"el", "gr"},
    {"en", "us"},
    {"endv", "us+dvorak"},
    {"eo", "epo"},
    {"fa", "ir"},
    {"fr-ch", "ch+fr"},
    {"gd", "gb+gla"},
    {"he", "il"},
    {"ja", "jp"},
    {"ko", "kr"},
    {"nb", "no"},
    {"sl", "si"},
    {"sr", "rs"},
    {"sv", "se"},
    {"uk", "ua"},
    {NULL, NULL}
};

enum
{
    LAYOUT_CHANGED,
    CONFIG_CHANGED,
    LAST_SIGNAL
};

static guint m_lSignals[LAST_SIGNAL];

struct _KeyboardPrivate
{
    GHashTable *lLayouts;
    guint nLayout;
    GSList *lLayoutRecHardware;
    GSList *lLayoutRecSoftware;
    GSList *lUsers;
    GSettings *pSettings;
    struct udev *pUdev;
    struct udev_monitor *pMonitor;
    GIOChannel *pChannel;
    gboolean bHardwareKeyboard;
    gboolean bSoftwareKeyboard;
    GSettings *pLomiriSettings;
    GSettings *pMaliitSettings;
};

typedef KeyboardPrivate priv_t;

G_DEFINE_TYPE_WITH_PRIVATE(Keyboard, keyboard, G_TYPE_OBJECT)

typedef struct _Layout
{
    gchar *sId;
    gchar *sLanguage;
    gchar *sDescription;

} Layout;

static int emitDelayedSignal(Keyboard *pKeyboard)
{
    g_signal_emit(pKeyboard, m_lSignals[CONFIG_CHANGED], 0);

    return G_SOURCE_REMOVE;
}

static gboolean isGreeter()
{
    const char *sUser = g_get_user_name();

    return g_str_equal(sUser, "lightdm");
}

static void getAccountsService(Keyboard *pKeyboard)
{
    gboolean bChanged = FALSE;

    for (GSList *lUser = pKeyboard->pPrivate->lUsers; lUser; lUser = lUser->next)
    {
        ActUser *pUser = lUser->data;
        gboolean bIsUserLoaded = act_user_is_loaded(pUser);

        if (bIsUserLoaded)
        {
            gint nUid = act_user_get_uid(pUser);
            gchar *sPath = g_strdup_printf("/org/freedesktop/Accounts/User%i", nUid);
            GDBusConnection *pConnection = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, NULL);
            GDBusProxy *pProxy = g_dbus_proxy_new_sync(pConnection, G_DBUS_PROXY_FLAGS_NONE, NULL, "org.freedesktop.Accounts", sPath, "org.freedesktop.DBus.Properties", NULL, NULL);
            GVariant *pValue = g_dbus_proxy_call_sync(pProxy, "Get", g_variant_new("(ss)", "org.ayatana.indicator.keyboard.AccountsService", "Layouts"), G_DBUS_CALL_FLAGS_NONE, -1, NULL, NULL);
            GVariant *pChild0 = g_variant_get_child_value(pValue, 0);
            GVariant *pChild1 = g_variant_get_child_value(pChild0, 0);

            GVariantIter *pIter;

            g_variant_get(pChild1, "as", &pIter);

            gchar *sLayout;

            while (g_variant_iter_loop(pIter, "s", &sLayout))
            {
                if (g_slist_find_custom(pKeyboard->pPrivate->lLayoutRecHardware, sLayout, (GCompareFunc)g_strcmp0) == NULL)
                {
                    pKeyboard->pPrivate->lLayoutRecHardware = g_slist_append(pKeyboard->pPrivate->lLayoutRecHardware, g_strdup(sLayout));
                    bChanged = TRUE;
                }
            }

            g_variant_iter_free(pIter);
            g_variant_unref(pChild1);
            g_variant_unref(pChild0);
            g_variant_unref(pValue);
            g_object_unref(pConnection);
            g_free(sPath);
        }
    }

    if (bChanged == TRUE)
    {
        g_timeout_add(500, (GSourceFunc)emitDelayedSignal, pKeyboard);
    }
}

static void onUserLoaded(Keyboard *pKeyboard, ActUser *pUser)
{
    gboolean bIsUserLoaded = act_user_is_loaded(pUser);

    if (bIsUserLoaded)
    {
        getAccountsService(pKeyboard);
        g_signal_handlers_disconnect_by_func(G_OBJECT(pUser), G_CALLBACK(onUserLoaded), pKeyboard);
    }
}

static void onManagerLoaded(Keyboard *pKeyboard)
{
    ActUserManager *pManager = act_user_manager_get_default();
    gboolean bIsLoaded;
    g_object_get(pManager, "is-loaded", &bIsLoaded, NULL);

    if (bIsLoaded)
    {
        pKeyboard->pPrivate->lUsers = act_user_manager_list_users(pManager);

        for (GSList *lUser = pKeyboard->pPrivate->lUsers; lUser; lUser = lUser->next)
        {
            ActUser *pUser = lUser->data;
            gboolean bIsUserLoaded = act_user_is_loaded(pUser);

            if (bIsUserLoaded)
            {
                getAccountsService(pKeyboard);
            }
            else
            {
                g_signal_connect_swapped(pUser, "notify::is-loaded", G_CALLBACK(onUserLoaded), pKeyboard);
            }
        }
    }
}

static void freeLayout(gpointer pData)
{
    Layout *pLayout = pData;

    g_return_if_fail(pLayout != NULL);

    g_free(pLayout->sId);
    g_free(pLayout->sLanguage);
    g_free(pLayout->sDescription);
    g_slice_free(Layout, pLayout);
}

void keyboard_AddSource(Keyboard *pKeyboard)
{
    return;
}

guint keyboard_GetNumLayouts(Keyboard *pKeyboard, gboolean bSoftware)
{
    guint nLayouts = 0;

    if (bSoftware)
    {
        nLayouts = g_slist_length (pKeyboard->pPrivate->lLayoutRecSoftware);
    }
    else
    {
        nLayouts = g_slist_length (pKeyboard->pPrivate->lLayoutRecHardware);
    }

    return nLayouts;
}

guint keyboard_GetLayoutIndex (Keyboard *pKeyboard)
{
    return pKeyboard->pPrivate->nLayout;
}

void keyboard_GetLayout(Keyboard *pKeyboard, gboolean bSoftware, gint nLayout, gchar **pLanguage, gchar **pDescription, gchar **pId)
{
    if (nLayout == -1)
    {
        nLayout = pKeyboard->pPrivate->nLayout;
    }

    GSList *lLayoutRec = NULL;

    if (bSoftware)
    {
        lLayoutRec = pKeyboard->pPrivate->lLayoutRecSoftware;
    }
    else
    {
        lLayoutRec = pKeyboard->pPrivate->lLayoutRecHardware;
    }

    gchar *sLayout = g_slist_nth_data (lLayoutRec, nLayout);
    const Layout *pLayout;
    g_hash_table_lookup_extended(pKeyboard->pPrivate->lLayouts, sLayout, NULL, (gpointer*)&pLayout);

    if (pLanguage != NULL)
    {
        *pLanguage = g_strdup(pLayout->sLanguage);
    }

    if (pDescription != NULL)
    {
        *pDescription = g_strdup(pLayout->sDescription);
    }

    if (pId != NULL)
    {
        *pId = g_strdup (sLayout);
    }
}

void keyboard_SetLayout(Keyboard *pKeyboard, gint nLayout, gboolean bSoftware)
{
    if (isGreeter() == FALSE)
    {
        if (!bSoftware)
        {
            gchar *sId = g_slist_nth_data (pKeyboard->pPrivate->lLayoutRecHardware, nLayout);

            GVariantBuilder cSettingsBuilder;
            g_variant_builder_init (&cSettingsBuilder, G_VARIANT_TYPE ("a(ss)"));
            g_variant_builder_add (&cSettingsBuilder, "(ss)", "xkb", sId);

            GVariantBuilder cAccountsBuilder;
            g_variant_builder_init (&cAccountsBuilder, G_VARIANT_TYPE ("aa{ss}"));
            GVariantBuilder cDictBuilder;
            g_variant_builder_init (&cDictBuilder, G_VARIANT_TYPE ("a{ss}"));
            g_variant_builder_add (&cDictBuilder, "{ss}", "xkb", sId);
            GVariant *pDict = g_variant_builder_end (&cDictBuilder);
            g_variant_builder_add_value (&cAccountsBuilder, pDict);

            guint nSources = g_slist_length (pKeyboard->pPrivate->lLayoutRecHardware);

            for (guint nSource = 0; nSource < nSources; nSource++)
            {
                if (nSource != nLayout)
                {
                    gchar *sId = g_slist_nth_data (pKeyboard->pPrivate->lLayoutRecHardware, nSource);

                    g_variant_builder_add (&cSettingsBuilder, "(ss)", "xkb", sId);

                    GVariantBuilder cDictBuilder;
                    g_variant_builder_init (&cDictBuilder, G_VARIANT_TYPE ("a{ss}"));
                    g_variant_builder_add (&cDictBuilder, "{ss}", "xkb", sId);
                    GVariant *pDict = g_variant_builder_end (&cDictBuilder);
                    g_variant_builder_add_value (&cAccountsBuilder, pDict);
                }
            }

            GVariant *pSettingsSources = g_variant_builder_end (&cSettingsBuilder);
            g_settings_set_value (pKeyboard->pPrivate->pSettings, "sources", pSettingsSources);

            GDBusConnection *pConnection = g_bus_get_sync (G_BUS_TYPE_SYSTEM, NULL, NULL);
            gint nUid = geteuid ();
            gchar *sPath = g_strdup_printf ("/org/freedesktop/Accounts/User%i", nUid);
            GDBusProxy *pProxy = g_dbus_proxy_new_sync (pConnection, G_DBUS_PROXY_FLAGS_NONE, NULL, "org.freedesktop.Accounts", sPath, "org.freedesktop.DBus.Properties", NULL, NULL);
            g_free (sPath);
            GVariant *pAccountsSources = g_variant_builder_end (&cAccountsBuilder);

            #ifdef ENABLE_UBUNTU_ACCOUNTSSERVICE
                gchar *sInterface = "org.freedesktop.Accounts.User";
            #else
                gchar *sInterface = "com.lomiri.shell.AccountsService";
            #endif

            GVariant *pRet = g_dbus_proxy_call_sync (pProxy, "Set", g_variant_new ("(ssv)", sInterface, "InputSources", pAccountsSources), G_DBUS_CALL_FLAGS_NONE, -1, NULL, NULL);
            g_variant_unref (pRet);
            g_object_unref (pConnection);
        }
        else
        {
            gchar *sId = g_slist_nth_data (pKeyboard->pPrivate->lLayoutRecSoftware, nLayout);
            guint nId = 0;
            gchar *sLayout = NULL;

            while (LOMIRI_TO_ISO[nId][0] != NULL)
            {
                gboolean bEqual = g_str_equal (LOMIRI_TO_ISO[nId][1], sId);

                if (bEqual)
                {
                    sLayout = LOMIRI_TO_ISO[nId][0];

                    break;
                }

                nId++;
            }

            if (!sLayout)
            {
                sLayout = sId;
            }

            g_settings_set_string (pKeyboard->pPrivate->pMaliitSettings, "active-language", sLayout);
        }
    }
    else
    {
        // TODO
        gchar *sCommand;
        gchar *sId = g_slist_nth_data(pKeyboard->pPrivate->lLayoutRecHardware, nLayout);
        gchar **lParamas = g_strsplit(sId, "+", -1);
        guint nParams = g_strv_length(lParamas);

        if (nParams == 1)
        {
            sCommand = g_strdup_printf("setxkbmap -layout %s", lParamas[0]);
        }
        else
        {
            sCommand = g_strdup_printf("setxkbmap -layout %s -variant %s", lParamas[0], lParamas[1]);
        }

        g_strfreev(lParamas);

        gchar *sOutput = NULL;
        GError *pError = NULL;
        gboolean bResult = g_spawn_command_line_sync(sCommand, &sOutput, NULL, NULL, &pError);
        g_free(sCommand);

        if (bResult == FALSE)
        {
            g_message("COMMAND 01 %s: %s", sOutput, pError->message);

            return;
        }

        pKeyboard->pPrivate->nLayout = nLayout;
        g_signal_emit(pKeyboard, m_lSignals[LAYOUT_CHANGED], 0);
    }
}

static void onDispose(GObject *pObject)
{
    Keyboard *self = G_KEYBOARD(pObject);
    g_signal_handlers_disconnect_by_data (self->pPrivate->pSettings, self);
    g_clear_object (&self->pPrivate->pSettings);
    g_signal_handlers_disconnect_by_data (self->pPrivate->pLomiriSettings, self);
    g_clear_object (&self->pPrivate->pLomiriSettings);
    g_signal_handlers_disconnect_by_data (self->pPrivate->pMaliitSettings, self);
    g_clear_object (&self->pPrivate->pMaliitSettings);

    if (self->pPrivate->lLayouts)
    {
        g_hash_table_destroy(self->pPrivate->lLayouts);
    }

    if (self->pPrivate->lLayoutRecHardware)
    {
        g_slist_free_full(self->pPrivate->lLayoutRecHardware, g_free);
    }

    if (self->pPrivate->lLayoutRecSoftware)
    {
        g_slist_free_full (self->pPrivate->lLayoutRecSoftware, g_free);
    }

    if (self->pPrivate->lUsers)
    {
        g_slist_free(self->pPrivate->lUsers);
    }

    if (self->pPrivate->pChannel)
    {
        g_io_channel_unref (self->pPrivate->pChannel);
    }

    if (self->pPrivate->pMonitor)
    {
        udev_monitor_unref (self->pPrivate->pMonitor);
    }

    if (self->pPrivate->pUdev)
    {
        udev_unref (self->pPrivate->pUdev);
    }

    G_OBJECT_CLASS (keyboard_parent_class)->dispose(pObject);
}

static void keyboard_class_init(KeyboardClass *klass)
{
    GObjectClass *pClass = G_OBJECT_CLASS(klass);
    pClass->dispose = onDispose;
    m_lSignals[LAYOUT_CHANGED] = g_signal_new(KEYBOARD_LAYOUT_CHANGED, G_TYPE_FROM_CLASS(klass), G_SIGNAL_RUN_LAST, 0, NULL, NULL, NULL, G_TYPE_NONE, 0);
    m_lSignals[CONFIG_CHANGED] = g_signal_new(KEYBOARD_CONFIG_CHANGED, G_TYPE_FROM_CLASS(klass), G_SIGNAL_RUN_LAST, 0, NULL, NULL, NULL, G_TYPE_NONE, 0);
}

Keyboard* keyboard_new()
{
    GObject *pObject = g_object_new(G_TYPE_KEYBOARD, NULL);

    return G_KEYBOARD(pObject);
}

static void onSoftwareKeyboardEnabled (GSettings *pSettings, const gchar *sKey, gpointer pData)
{
    Keyboard *self = G_KEYBOARD (pData);
    self->pPrivate->bSoftwareKeyboard = g_settings_get_boolean (pSettings, "always-show-osk");
    g_signal_emit (self, m_lSignals[CONFIG_CHANGED], 0);
    g_signal_emit (self, m_lSignals[LAYOUT_CHANGED], 0);
}

static void onHardwareLayoutsChanged (GSettings *pSettings, const gchar *sKey, gpointer pData)
{
    Keyboard *pKeyboard = G_KEYBOARD (pData);
    gboolean bSignal = FALSE;

    if (pKeyboard->pPrivate->lLayoutRecHardware)
    {
        g_slist_free_full (g_steal_pointer (&pKeyboard->pPrivate->lLayoutRecHardware), g_free);
        bSignal = TRUE;
    }

    GVariant *pList = g_settings_get_value (pSettings, "sources");
    gsize nSources = g_variant_n_children (pList);

    if (nSources)
    {
        GVariantIter cIter;
        g_variant_iter_init (&cIter, pList);
        gchar *sLayout = NULL;

        while (g_variant_iter_loop (&cIter, "(ss)", NULL, &sLayout))
        {
            pKeyboard->pPrivate->lLayoutRecHardware = g_slist_append (pKeyboard->pPrivate->lLayoutRecHardware, g_strdup (sLayout));
        }
    }
    else
    {
        pKeyboard->pPrivate->lLayoutRecHardware = g_slist_append (pKeyboard->pPrivate->lLayoutRecHardware, g_strdup ("us"));
    }

    g_variant_unref (pList);

    if (bSignal)
    {
        g_signal_emit (pKeyboard, m_lSignals[CONFIG_CHANGED], 0);
        g_signal_emit (pKeyboard, m_lSignals[LAYOUT_CHANGED], 0);
    }
}

static void onSoftwareLayoutChanged (GSettings *pSettings, const gchar *sKey, gpointer pData)
{
    Keyboard *pKeyboard = G_KEYBOARD (pData);
    g_signal_emit (pKeyboard, m_lSignals[LAYOUT_CHANGED], 0);
}

static void onSoftwareLayoutsChanged (GSettings *pSettings, const gchar *sKey, gpointer pData)
{
    Keyboard *pKeyboard = G_KEYBOARD (pData);
    gboolean bSignal = FALSE;

    if (pKeyboard->pPrivate->lLayoutRecSoftware)
    {
        g_slist_free_full (g_steal_pointer (&pKeyboard->pPrivate->lLayoutRecSoftware), g_free);
        bSignal = TRUE;
    }

    GStrv lLayouts = g_settings_get_strv (pSettings, "enabled-languages");
    guint nLayouts = g_strv_length (lLayouts);

    if (lLayouts)
    {
        for (guint nLayout = 0; nLayout < nLayouts; nLayout++)
        {
            guint nId = 0;
            gchar *sLayout = NULL;

            while (LOMIRI_TO_ISO[nId][0] != NULL)
            {
                gboolean bEqual = g_str_equal (LOMIRI_TO_ISO[nId][0], lLayouts[nLayout]);

                if (bEqual)
                {
                    sLayout = g_strdup (LOMIRI_TO_ISO[nId][1]);

                    break;
                }

                nId++;
            }

            if (!sLayout)
            {
                sLayout = g_strdup (lLayouts[nLayout]);
            }

            pKeyboard->pPrivate->lLayoutRecSoftware = g_slist_append (pKeyboard->pPrivate->lLayoutRecSoftware, sLayout);
        }

        g_strfreev (lLayouts);
    }

    if (bSignal)
    {
        g_signal_emit (pKeyboard, m_lSignals[CONFIG_CHANGED], 0);
        g_signal_emit (pKeyboard, m_lSignals[LAYOUT_CHANGED], 0);
    }
}

static bool isKeyboard (struct udev_device *pDevice)
{
    const gchar *sValue = udev_device_get_property_value (pDevice, "ID_INPUT_KEYBOARD");
    gint nCompared = g_strcmp0 (sValue, "1");

    return nCompared == 0;
}

static gboolean hasKeyboard (struct udev *pUdev)
{
    struct udev_enumerate *pEnumerate = udev_enumerate_new (pUdev);
    udev_enumerate_add_match_subsystem (pEnumerate, "input");
    udev_enumerate_scan_devices (pEnumerate);
    struct udev_list_entry *lEntries = udev_enumerate_get_list_entry (pEnumerate);
    struct udev_list_entry *pEntry;
    gboolean bFound = FALSE;

    udev_list_entry_foreach (pEntry, lEntries)
    {
        const gchar *sPath = udev_list_entry_get_name (pEntry);
        struct udev_device *pDevice = udev_device_new_from_syspath (pUdev, sPath);
        gboolean bKeyboard = isKeyboard (pDevice);

        if (bKeyboard)
        {
            bFound = TRUE;
            udev_device_unref (pDevice);

            break;
        }

        udev_device_unref (pDevice);
    }

    udev_enumerate_unref (pEnumerate);

    return bFound;
}

static gboolean onUdev (GIOChannel *pChannel, GIOCondition nCondition, gpointer pData)
{
    Keyboard *self = G_KEYBOARD (pData);
    struct udev_device *pDevice = udev_monitor_receive_device (self->pPrivate->pMonitor);

    if (pDevice)
    {
        gboolean bKeyboard = isKeyboard (pDevice);

        if (bKeyboard)
        {
            struct udev *pUdev = udev_device_get_udev (pDevice);
            self->pPrivate->bHardwareKeyboard = hasKeyboard (pUdev);
            g_signal_emit (self, m_lSignals[CONFIG_CHANGED], 0);
            g_signal_emit (self, m_lSignals[LAYOUT_CHANGED], 0);
        }

        udev_device_unref (pDevice);
    }

    return TRUE;
}

gboolean keyboard_hasHardwareKeyboard (Keyboard *self)
{
    return self->pPrivate->bHardwareKeyboard;
}

gboolean keyboard_hasSoftwareKeyboard (Keyboard *self)
{
    return self->pPrivate->bSoftwareKeyboard;
}

static void keyboard_init(Keyboard *self)
{
    self->pPrivate = keyboard_get_instance_private(self);
    self->pPrivate->lLayoutRecHardware = NULL;
    self->pPrivate->lLayoutRecSoftware = NULL;
    self->pPrivate->lLayouts = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, freeLayout);

    // Read all available layouts
    struct rxkb_context *pContext = rxkb_context_new(RXKB_CONTEXT_LOAD_EXOTIC_RULES);
    g_assert(pContext);

    if (!rxkb_context_include_path_append_default(pContext))
    {
        g_warning("Failed to include default paths");
    }

    if (!rxkb_context_parse(pContext, "evdev"))
    {
        g_warning("Failed to parse XKB descriptions");
    }

    struct rxkb_layout *pRxkbLayout = rxkb_layout_first(pContext);

    while (pRxkbLayout != NULL)
    {
        const gchar *sLayout = rxkb_layout_get_name(pRxkbLayout);
        const gchar *sVariant = rxkb_layout_get_variant(pRxkbLayout);
        const gchar *sDescription = rxkb_layout_get_description(pRxkbLayout);
        const gchar *sTranslated = g_dgettext("xkeyboard-config", sDescription);
        sDescription = sTranslated;
        Layout *pLayout = g_slice_new0(Layout);

        if (sVariant != NULL && strlen(sVariant) > 0)
        {
            pLayout->sId = g_strjoin("+", sLayout, sVariant, NULL);
            pLayout->sLanguage = g_strdup(lookupLanguage(pLayout->sId));
            pLayout->sDescription = g_strdup(sDescription);
        }
        else
        {
            pLayout->sId = g_strdup(sLayout);
            pLayout->sLanguage = g_strdup(lookupLanguage(pLayout->sId));
            pLayout->sDescription = g_strdup(sDescription);
        }

        g_hash_table_replace(self->pPrivate->lLayouts, pLayout->sId, pLayout);

        pRxkbLayout = rxkb_layout_next(pRxkbLayout);
    }

    rxkb_context_unref(pContext);

    // Lomiri-specific layouts
    const gchar *LAYOUTS[][3] =
    {
        {"emoji", "emoji", "Emoji"},
        {"Bn", "bn-avro", "Bangla (Avro)"},
        {"Zn", "chewing", "Chinese (Chewing)"},
        {"Zn", "pinyin", "Chinese (Pinyin)"}
    };

    for (guint nLayout = 0; nLayout < 3; nLayout++)
    {
        Layout *pLayout = g_slice_new0 (Layout);
        pLayout->sId = g_strdup (LAYOUTS[nLayout][1]);
        pLayout->sLanguage = g_strdup (LAYOUTS[nLayout][0]);
        pLayout->sDescription = g_strdup (LAYOUTS[nLayout][2]);
        g_hash_table_replace (self->pPrivate->lLayouts, pLayout->sId, pLayout);
    }
    //~Lomiri-specific layouts

    if (isGreeter() == FALSE)
    {
        self->pPrivate->nLayout = 0;
        GSettingsSchemaSource *pSource = g_settings_schema_source_get_default ();
        GSettingsSchema *pSchema = NULL;

        if (pSource)
        {
            pSchema = g_settings_schema_source_lookup (pSource, "org.gnome.desktop.input-sources", FALSE);

            if (pSchema)
            {
                g_settings_schema_unref (pSchema);
                self->pPrivate->pSettings = g_settings_new ("org.gnome.desktop.input-sources");
                g_signal_connect (self->pPrivate->pSettings, "changed::sources", G_CALLBACK (onHardwareLayoutsChanged), self);
                onHardwareLayoutsChanged (self->pPrivate->pSettings, "sources", self);
            }
            else
            {
                g_error ("Panic: no org.gnome.desktop.input-sources schema found");
            }
        }
    }
    else
    {
        // Get system layouts
        getSystemLayouts ("/etc/default/keyboard", &self->pPrivate->lLayoutRecHardware, NULL, FALSE);

        if (!self->pPrivate->lLayoutRecHardware)
        {
            getSystemLayouts ("/etc/X11/xorg.conf.d/00-keyboard.conf", &self->pPrivate->lLayoutRecHardware, NULL, FALSE);
        }

        if (!self->pPrivate->lLayoutRecHardware)
        {
            gchar *sId = g_strdup ("us");
            self->pPrivate->lLayoutRecHardware = g_slist_append (self->pPrivate->lLayoutRecHardware, sId);
        }

        self->pPrivate->nLayout = 0;
        //~ Get system layouts

        ActUserManager *pManager = act_user_manager_get_default();
        gboolean bIsLoaded;
        g_object_get(pManager, "is-loaded", &bIsLoaded, NULL);

        if (bIsLoaded)
        {
            self->pPrivate->lUsers = act_user_manager_list_users(pManager);

            for (GSList *lUser = self->pPrivate->lUsers; lUser; lUser = lUser->next)
            {
                ActUser *pUser = lUser->data;
                gboolean bIsUserLoaded = act_user_is_loaded(pUser);

                if (bIsUserLoaded)
                {
                    getAccountsService(self);
                }
                else
                {
                    g_signal_connect_swapped(pUser, "notify::is-loaded", G_CALLBACK(onUserLoaded), self);
                }
            }
        }
        else
        {
            g_signal_connect_object(pManager, "notify::is-loaded", G_CALLBACK(onManagerLoaded), self, G_CONNECT_SWAPPED);
        }
    }

    // Watch for a hardware keyboard
    self->pPrivate->pUdev = udev_new ();
    self->pPrivate->pMonitor = udev_monitor_new_from_netlink (self->pPrivate->pUdev, "udev");
    udev_monitor_filter_add_match_subsystem_devtype (self->pPrivate->pMonitor, "input", NULL);
    udev_monitor_enable_receiving (self->pPrivate->pMonitor);
    gint nFd = udev_monitor_get_fd (self->pPrivate->pMonitor);
    self->pPrivate->bHardwareKeyboard = hasKeyboard (self->pPrivate->pUdev);
    self->pPrivate->pChannel = g_io_channel_unix_new (nFd);
    g_io_add_watch (self->pPrivate->pChannel, G_IO_IN, onUdev, self);
    //~Watch for a hardware keyboard

    // Watch software keyboard
    GSettingsSchemaSource *pSource = g_settings_schema_source_get_default ();
    GSettingsSchema *pSchema = NULL;

    if (pSource)
    {
        pSchema = g_settings_schema_source_lookup (pSource, "com.lomiri.Shell", FALSE);

        if (pSchema)
        {
            g_settings_schema_unref (pSchema);
            self->pPrivate->pLomiriSettings = g_settings_new ("com.lomiri.Shell");
            g_signal_connect (self->pPrivate->pLomiriSettings, "changed::always-show-osk", G_CALLBACK (onSoftwareKeyboardEnabled), self);
            onSoftwareKeyboardEnabled (self->pPrivate->pLomiriSettings, "always-show-osk", self);
        }
        else
        {
            g_error ("Panic: no com.lomiri.Shell schema found");
        }

        pSchema = g_settings_schema_source_lookup (pSource, "com.lomiri.keyboard.maliit", FALSE);

        if (pSchema)
        {
            g_settings_schema_unref (pSchema);
            self->pPrivate->pMaliitSettings = g_settings_new ("com.lomiri.keyboard.maliit");
            g_signal_connect (self->pPrivate->pMaliitSettings, "changed::enabled-languages", G_CALLBACK (onSoftwareLayoutsChanged), self);
            onSoftwareLayoutsChanged (self->pPrivate->pMaliitSettings, "enabled-languages", self);
            g_signal_connect (self->pPrivate->pMaliitSettings, "changed::active-language", G_CALLBACK (onSoftwareLayoutChanged), self);
            onSoftwareLayoutChanged (self->pPrivate->pMaliitSettings, "active-language", self);
        }
        else
        {
            g_error ("Panic: no com.lomiri.keyboard.maliit schema found");
        }
    }
    //~Watch software keyboard
}
