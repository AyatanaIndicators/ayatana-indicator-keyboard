/*
 * Copyright 2021-2024 Robert Tari <robert@tari.in>
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
#include <X11/XKBlib.h>
#include <xkbcommon/xkbregistry.h>
#include <libxklavier/xklavier.h>
#include "languages.h"
#include "keyboard.h"
#include "system-layouts.h"

#define GREETER_BUS_NAME "org.ayatana.greeter"
#define GREETER_BUS_PATH "/org/ayatana/greeter"

enum
{
    LAYOUT_CHANGED,
    CONFIG_CHANGED,
    LAST_SIGNAL
};

static guint m_lSignals[LAST_SIGNAL];

struct _KeyboardPrivate
{
    GPollFD cPollFD;
    GSourceFuncs cSourceFuncs;
    XklEngine *pEngine;
    GHashTable *lLayouts;
    Display *pDisplay;
    guint nLayout;
    gint nXkbEventType;
    XklConfigRec *pConfigRec;
    GSList *lLayoutRec;
    GSList *lUsers;
    GDBusConnection *pConnection;
    guint nSubscription;
    gchar *sUser;
    GStrv lSystemLayouts;
};

typedef KeyboardPrivate priv_t;

G_DEFINE_TYPE_WITH_PRIVATE(Keyboard, keyboard, G_TYPE_OBJECT)

typedef struct _Layout
{
    gchar *sId;
    gchar *sLanguage;
    gchar *sDescription;

} Layout;

typedef struct _Source
{
    GSource cSource;
    Keyboard *pKeyboard;

} Source;

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

static void setAccountsService(Keyboard *pKeyboard)
{
    gint nUid = geteuid();
    gchar *sPath = g_strdup_printf("/org/freedesktop/Accounts/User%i", nUid);
    GDBusConnection *pConnection = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, NULL);
    GDBusProxy *pProxy = g_dbus_proxy_new_sync(pConnection, G_DBUS_PROXY_FLAGS_NONE, NULL, "org.freedesktop.Accounts", sPath, "org.freedesktop.DBus.Properties", NULL, NULL);
    GVariantBuilder *pBuilder = g_variant_builder_new(G_VARIANT_TYPE("as"));
    guint nLayouts = g_strv_length(pKeyboard->pPrivate->pConfigRec->layouts);

    for (guint nLayout = 0; nLayout < nLayouts; nLayout++)
    {
        gchar *sId = NULL;
        gchar *sLayout = pKeyboard->pPrivate->pConfigRec->layouts[nLayout];
        gchar *sVariant = pKeyboard->pPrivate->pConfigRec->variants[nLayout];

        if (sVariant && strlen(sVariant))
        {
            sId = g_strconcat(sLayout, "+", sVariant, NULL);
        }
        else
        {
            sId = g_strdup(sLayout);
        }

        g_variant_builder_add(pBuilder, "s", sId);
        g_free(sId);
    }

    GVariant *pValue = g_variant_new("as", pBuilder);
    GVariant *pRet = g_dbus_proxy_call_sync(pProxy, "Set", g_variant_new("(ssv)", "org.ayatana.indicator.keyboard.AccountsService", "Layouts", pValue), G_DBUS_CALL_FLAGS_NONE, -1, NULL, NULL);

    g_variant_builder_unref(pBuilder);
    g_variant_unref(pRet);
    g_object_unref(pConnection);
    g_free(sPath);
}

static void getAccountsService(Keyboard *pKeyboard, ActUser *pUser)
{
    if (!pKeyboard->pPrivate->sUser)
    {
        GError *pError = NULL;
        GVariant *pUser = g_dbus_connection_call_sync (pKeyboard->pPrivate->pConnection, GREETER_BUS_NAME, GREETER_BUS_PATH, GREETER_BUS_NAME, "GetUser", NULL, G_VARIANT_TYPE ("(s)"), G_DBUS_CALL_FLAGS_NONE, -1, NULL, &pError);

        if (pError)
        {
            g_warning ("Failed calling GetUser, the greeter may not be ready yet: %s", pError->message);
            g_error_free (pError);

            return;
        }

        g_variant_get (pUser, "(s)", &pKeyboard->pPrivate->sUser);
    }

    gboolean bPrefix = g_str_has_prefix (pKeyboard->pPrivate->sUser, "*");

    if (bPrefix)
    {
        if (pKeyboard->pPrivate->lLayoutRec)
        {
            g_slist_free_full (g_steal_pointer (&pKeyboard->pPrivate->lLayoutRec), g_free);
        }

        guint nLayouts = g_strv_length (pKeyboard->pPrivate->lSystemLayouts);

        for (guint nLayout = 0; nLayout < nLayouts; nLayout++)
        {
            pKeyboard->pPrivate->lLayoutRec = g_slist_append (pKeyboard->pPrivate->lLayoutRec, g_strdup (pKeyboard->pPrivate->lSystemLayouts[nLayout]));
        }

        g_timeout_add(500, (GSourceFunc)emitDelayedSignal, pKeyboard);
    }
    else
    {
        const gchar *sUser = act_user_get_user_name (pUser);
        gboolean bSame = g_str_equal(pKeyboard->pPrivate->sUser, sUser);

        if (bSame)
        {
            if (pKeyboard->pPrivate->lLayoutRec)
            {
                g_slist_free_full (g_steal_pointer (&pKeyboard->pPrivate->lLayoutRec), g_free);
            }

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
                pKeyboard->pPrivate->lLayoutRec = g_slist_append(pKeyboard->pPrivate->lLayoutRec, g_strdup(sLayout));
            }

            guint nLayouts = g_slist_length(pKeyboard->pPrivate->lLayoutRec);

            if (!nLayouts)
            {
                guint nLayouts = g_strv_length (pKeyboard->pPrivate->lSystemLayouts);

                for (guint nLayout = 0; nLayout < nLayouts; nLayout++)
                {
                    pKeyboard->pPrivate->lLayoutRec = g_slist_append (pKeyboard->pPrivate->lLayoutRec, g_strdup (pKeyboard->pPrivate->lSystemLayouts[nLayout]));
                }
            }

            g_variant_iter_free(pIter);
            g_variant_unref(pChild1);
            g_variant_unref(pChild0);
            g_variant_unref(pValue);
            g_object_unref(pConnection);
            g_free(sPath);

            g_timeout_add(500, (GSourceFunc)emitDelayedSignal, pKeyboard);
        }
    }
}

static void onUserLoaded(Keyboard *pKeyboard, ActUser *pUser)
{
    gboolean bIsUserLoaded = act_user_is_loaded(pUser);

    if (bIsUserLoaded)
    {
        getAccountsService(pKeyboard, pUser);
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
        if (!pKeyboard->pPrivate->lUsers)
        {
            pKeyboard->pPrivate->lUsers = act_user_manager_list_users(pManager);
        }

        for (GSList *lUser = pKeyboard->pPrivate->lUsers; lUser; lUser = lUser->next)
        {
            ActUser *pUser = lUser->data;
            gboolean bIsUserLoaded = act_user_is_loaded(pUser);

            if (bIsUserLoaded)
            {
                getAccountsService(pKeyboard, pUser);
            }
            else
            {
                g_signal_connect_swapped(pUser, "notify::is-loaded", G_CALLBACK(onUserLoaded), pKeyboard);
            }
        }
    }
}

static gboolean onCheckEvent(Display *pDisplay, XEvent *pEvent, XPointer pData)
{
    gint *pXkbEventType = (gint*)pData;

    if (pEvent->type == *pXkbEventType)
    {
        XkbEvent *pXkbEvent = (XkbEvent*)pEvent;

        if (pXkbEvent->any.xkb_type == XkbStateNotify || pXkbEvent->any.xkb_type == XkbNamesNotify)
        {
            return TRUE;
        }
    }

    return FALSE;
}

static gboolean onCheckSource(GSource *pSource)
{
    Keyboard *pKeyboard = ((Source*)pSource)->pKeyboard;
    XEvent cEvent;
    gboolean bEvent = XCheckIfEvent(pKeyboard->pPrivate->pDisplay, &cEvent, onCheckEvent, (XPointer)&pKeyboard->pPrivate->nXkbEventType);

    if (bEvent)
    {
        XklConfigRec *pConfigRec = xkl_config_rec_new();
        xkl_config_rec_get_from_server(pConfigRec, pKeyboard->pPrivate->pEngine);
        gboolean bConfigChanged = FALSE;
        gboolean bLayoutChanged = FALSE;

        if (!xkl_config_rec_equals(pKeyboard->pPrivate->pConfigRec, pConfigRec))
        {
            gboolean bBadSignal = FALSE;

            if (g_strv_length(pKeyboard->pPrivate->pConfigRec->layouts) > g_strv_length(pConfigRec->layouts))
            {
                // Pairing a Bluetooth device does this sometimes
                if (pConfigRec->variants[0] == NULL)
                {
                    bBadSignal = TRUE;
                }
                else
                {
                    xkl_engine_lock_group(pKeyboard->pPrivate->pEngine, 0);
                    pKeyboard->pPrivate->nLayout = 0;
                    bLayoutChanged = TRUE;
                }
            }

            if (!bBadSignal)
            {
                xkl_config_rec_get_from_server(pKeyboard->pPrivate->pConfigRec, pKeyboard->pPrivate->pEngine);
                bConfigChanged = TRUE;
            }
        }

        g_object_unref(pConfigRec);
        pConfigRec = NULL;

        if (((XkbEvent*)&cEvent)->any.xkb_type == XkbStateNotify && !bConfigChanged && ((XkbEvent*)&cEvent)->state.group != pKeyboard->pPrivate->nLayout && ((XkbEvent*)&cEvent)->state.group < g_strv_length(pKeyboard->pPrivate->pConfigRec->layouts))
        {
            pKeyboard->pPrivate->nLayout = ((XkbEvent*)&cEvent)->state.group;
            bLayoutChanged = TRUE;
        }

        if (bLayoutChanged)
        {
            g_signal_emit(pKeyboard, m_lSignals[LAYOUT_CHANGED], 0);
        }

        if (bConfigChanged)
        {
            setAccountsService(pKeyboard);
            g_signal_emit(pKeyboard, m_lSignals[CONFIG_CHANGED], 0);
        }
    }

    return FALSE;
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
    if (isGreeter() == FALSE)
    {
        XkbQueryExtension(pKeyboard->pPrivate->pDisplay, 0, &pKeyboard->pPrivate->nXkbEventType, 0, 0, 0);
        XkbSelectEventDetails(pKeyboard->pPrivate->pDisplay, XkbUseCoreKbd, XkbStateNotify, XkbAllStateComponentsMask, XkbGroupStateMask);

        pKeyboard->pPrivate->cPollFD.fd = ConnectionNumber(pKeyboard->pPrivate->pDisplay);
        pKeyboard->pPrivate->cPollFD.events = G_IO_IN | G_IO_HUP | G_IO_ERR;
        pKeyboard->pPrivate->cPollFD.revents = 0;
        pKeyboard->pPrivate->cSourceFuncs.prepare = NULL;
        pKeyboard->pPrivate->cSourceFuncs.check = onCheckSource;
        pKeyboard->pPrivate->cSourceFuncs.dispatch = NULL;
        pKeyboard->pPrivate->cSourceFuncs.finalize = NULL;

        GSource *pSource = g_source_new(&pKeyboard->pPrivate->cSourceFuncs, sizeof(Source));
        ((Source*)pSource)->pKeyboard = pKeyboard;
        g_source_add_poll(pSource, &pKeyboard->pPrivate->cPollFD);
        g_source_attach(pSource, NULL);
    }
}

guint keyboard_GetNumLayouts(Keyboard *pKeyboard)
{
    guint nLayouts = 0;

    if (isGreeter())
    {
        nLayouts = g_slist_length(pKeyboard->pPrivate->lLayoutRec);
    }
    else
    {
        nLayouts = g_strv_length(pKeyboard->pPrivate->pConfigRec->layouts);
    }

    return nLayouts;
}

guint keyboard_GetLayoutIndex (Keyboard *pKeyboard)
{
    return pKeyboard->pPrivate->nLayout;
}

void keyboard_GetLayout(Keyboard *pKeyboard, gint nLayout, gchar **pLanguage, gchar **pDescription)
{
    if (nLayout == -1)
    {
        nLayout = pKeyboard->pPrivate->nLayout;
    }

    gchar *sId = NULL;

    if (isGreeter() == TRUE)
    {
        gchar *sLayout = g_slist_nth_data(pKeyboard->pPrivate->lLayoutRec, nLayout);
        sId = g_strdup(sLayout);
    }
    else
    {
        gchar *sLayout = pKeyboard->pPrivate->pConfigRec->layouts[nLayout];
        gchar *sVariant = pKeyboard->pPrivate->pConfigRec->variants[nLayout];

        if (sVariant && strlen(sVariant))
        {
            sId = g_strconcat(sLayout, "+", sVariant, NULL);
        }
        else
        {
            sId = g_strdup(sLayout);
        }
    }

    const Layout *pLayout;
    g_hash_table_lookup_extended(pKeyboard->pPrivate->lLayouts, sId, NULL, (gpointer*)&pLayout);

    if (pLanguage != NULL)
    {
        *pLanguage = g_strdup(pLayout->sLanguage);
    }

    if (pDescription != NULL)
    {
        *pDescription = g_strdup(pLayout->sDescription);
    }

    g_free(sId);
}

void keyboard_SetLayout(Keyboard *pKeyboard, gint nLayout)
{
    if (isGreeter() == FALSE)
    {
        xkl_engine_lock_group(pKeyboard->pPrivate->pEngine, nLayout);
    }
    else
    {
        gchar *sId = g_slist_nth_data(pKeyboard->pPrivate->lLayoutRec, nLayout);
        gchar **lParams = g_strsplit(sId, "+", -1);
        guint nParams = g_strv_length(lParams);
        gchar *sVariant = "";

        if (nParams == 2)
        {
            sVariant = lParams[1];
        }

        GError *pError = NULL;
        GVariant *pLayout = g_variant_new ("(ss)", lParams[0], sVariant);
        g_dbus_connection_call_sync (pKeyboard->pPrivate->pConnection, GREETER_BUS_NAME, GREETER_BUS_PATH, GREETER_BUS_NAME, "SetLayout", pLayout, NULL, G_DBUS_CALL_FLAGS_NONE, -1, NULL, &pError);

        if (pError)
        {
            g_error ("Panic: Failed calling SetLayout: %s", pError->message);
            g_error_free (pError);

            return;
        }

        g_strfreev (lParams);
    }

    pKeyboard->pPrivate->nLayout = nLayout;
    g_signal_emit(pKeyboard, m_lSignals[LAYOUT_CHANGED], 0);
}

static void onDispose(GObject *pObject)
{
    Keyboard *self = G_KEYBOARD (pObject);

    if (self->pPrivate->nSubscription)
    {
        g_dbus_connection_signal_unsubscribe (self->pPrivate->pConnection, self->pPrivate->nSubscription);
    }

    if (self->pPrivate->lUsers)
    {
        g_slist_free(self->pPrivate->lUsers);
    }

    if (self->pPrivate->pConnection)
    {
        g_object_unref (self->pPrivate->pConnection);
    }

    if (self->pPrivate->lLayouts)
    {
        g_hash_table_destroy(self->pPrivate->lLayouts);
    }

    if (self->pPrivate->pConfigRec)
    {
        g_object_unref(self->pPrivate->pConfigRec);
    }

    if (self->pPrivate->lLayoutRec)
    {
        g_slist_free_full(self->pPrivate->lLayoutRec, g_free);
    }

    if (self->pPrivate->lSystemLayouts)
    {
        g_strfreev (self->pPrivate->lSystemLayouts);
    }

    G_OBJECT_CLASS(keyboard_parent_class)->dispose(pObject);
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

static void onUserChanged (GDBusConnection *pConnection, const gchar *sSender, const gchar *sPath, const gchar *sInterface, const gchar *sSignal, GVariant *pParameters, gpointer pUserData)
{
    Keyboard *self = G_KEYBOARD (pUserData);
    g_variant_get (pParameters, "(s)", &self->pPrivate->sUser);
    ActUserManager *pManager = act_user_manager_get_default ();
    gboolean bIsLoaded;
    g_object_get (pManager, "is-loaded", &bIsLoaded, NULL);

    if (bIsLoaded)
    {
        if (!self->pPrivate->lUsers)
        {
            self->pPrivate->lUsers = act_user_manager_list_users(pManager);
        }

        for (GSList *lUser = self->pPrivate->lUsers; lUser; lUser = lUser->next)
        {
            ActUser *pUser = lUser->data;
            gboolean bIsUserLoaded = act_user_is_loaded (pUser);

            if (bIsUserLoaded)
            {
                getAccountsService (self, pUser);
            }
            else
            {
                g_signal_connect_swapped (pUser, "notify::is-loaded", G_CALLBACK (onUserLoaded), self);
            }
        }
    }
    else
    {
        g_signal_connect_object (pManager, "notify::is-loaded", G_CALLBACK (onManagerLoaded), self, G_CONNECT_SWAPPED);
    }
}

static void keyboard_init(Keyboard *self)
{
    self->pPrivate = keyboard_get_instance_private(self);
    self->pPrivate->lLayoutRec = NULL;
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

        gboolean bContains = g_hash_table_contains (self->pPrivate->lLayouts, pLayout->sId);

        if (!bContains)
        {
            g_debug ("    {\"%s\", \"%s\"}, //%s", pLayout->sLanguage, pLayout->sId, pLayout->sDescription);
        }

        g_hash_table_replace(self->pPrivate->lLayouts, pLayout->sId, pLayout);
        pRxkbLayout = rxkb_layout_next(pRxkbLayout);
    }

    rxkb_context_unref(pContext);

    GError *pError = NULL;
    self->pPrivate->sUser = NULL;
    self->pPrivate->pConnection = g_bus_get_sync (G_BUS_TYPE_SESSION, NULL, &pError);

    if (pError)
    {
        g_error ("Panic: Failed connecting to the session bus: %s", pError->message);
        g_error_free (pError);

        return;
    }

    if (isGreeter() == FALSE)
    {
        self->pPrivate->pDisplay = XOpenDisplay(NULL);

        g_assert(self->pPrivate->pDisplay);

        self->pPrivate->pEngine = xkl_engine_get_instance(self->pPrivate->pDisplay);

        g_assert(self->pPrivate->pEngine);

        xkl_engine_start_listen(self->pPrivate->pEngine, XKLL_TRACK_KEYBOARD_STATE);
        self->pPrivate->pConfigRec = xkl_config_rec_new();
        xkl_config_rec_get_from_server(self->pPrivate->pConfigRec, self->pPrivate->pEngine);
        XklState *pState = xkl_engine_get_current_state(self->pPrivate->pEngine);

        self->pPrivate->nLayout = pState->group;

        setAccountsService(self);
    }
    else
    {
        self->pPrivate->lUsers = NULL;
        self->pPrivate->nSubscription = g_dbus_connection_signal_subscribe (self->pPrivate->pConnection, NULL, GREETER_BUS_NAME, "UserChanged", GREETER_BUS_PATH, NULL, G_DBUS_SIGNAL_FLAGS_NONE, onUserChanged, self, NULL);

        // Get system layouts
        getSystemLayouts ("/etc/default/keyboard", &self->pPrivate->lLayoutRec, &self->pPrivate->lSystemLayouts, TRUE);

        if (!self->pPrivate->lLayoutRec)
        {
            getSystemLayouts ("/etc/X11/xorg.conf.d/00-keyboard.conf", &self->pPrivate->lLayoutRec, &self->pPrivate->lSystemLayouts, TRUE);
        }

        if (!self->pPrivate->lLayoutRec)
        {
            GStrvBuilder *pBuilder = g_strv_builder_new ();
            gchar *sId = g_strdup ("us");
            self->pPrivate->lLayoutRec = g_slist_append (self->pPrivate->lLayoutRec, sId);
            g_strv_builder_add (pBuilder, sId);
            self->pPrivate->lSystemLayouts = g_strv_builder_end (pBuilder);
            g_strv_builder_unref (pBuilder);
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
                    getAccountsService(self, pUser);
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
}
