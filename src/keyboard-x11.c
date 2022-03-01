/*
 * Copyright 2021-2022 Robert Tari <robert@tari.in>
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

#include <X11/XKBlib.h>
#include <xkbcommon/xkbregistry.h>
#include <libxklavier/xklavier.h>
#include "languages.h"
#include "keyboard.h"

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

guint keyboard_GetNumLayouts(Keyboard *pKeyboard)
{
    return g_strv_length(pKeyboard->pPrivate->pConfigRec->layouts);
}

void keyboard_GetLayout(Keyboard *pKeyboard, gint nLayout, gchar **pLanguage, gchar **pDescription)
{
    if (nLayout == -1)
    {
        nLayout = pKeyboard->pPrivate->nLayout;
    }

    gchar *sLayout = pKeyboard->pPrivate->pConfigRec->layouts[nLayout];
    gchar *sVariant = pKeyboard->pPrivate->pConfigRec->variants[nLayout];
    gchar *sId;

    if (sVariant && strlen(sVariant))
    {
        sId = g_strconcat(sLayout, "+", sVariant, NULL);
    }
    else
    {
        sId = g_strdup(sLayout);
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
    xkl_engine_lock_group(pKeyboard->pPrivate->pEngine, nLayout);
    pKeyboard->pPrivate->nLayout = nLayout;
    g_signal_emit(pKeyboard, m_lSignals[LAYOUT_CHANGED], 0);
}

static void onDispose(GObject *pObject)
{
    Keyboard *self = G_KEYBOARD(pObject);

    if (self->pPrivate->lLayouts)
    {
        g_hash_table_destroy(self->pPrivate->lLayouts);
    }

    if (self->pPrivate->pConfigRec)
    {
        g_object_unref(self->pPrivate->pConfigRec);
        self->pPrivate->pConfigRec = NULL;
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

static void keyboard_init(Keyboard *self)
{
    self->pPrivate = keyboard_get_instance_private(self);
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

    self->pPrivate->pDisplay = XOpenDisplay(NULL);

    g_assert(self->pPrivate->pDisplay);

    self->pPrivate->pEngine = xkl_engine_get_instance(self->pPrivate->pDisplay);

    g_assert(self->pPrivate->pEngine);

    xkl_engine_start_listen(self->pPrivate->pEngine, XKLL_TRACK_KEYBOARD_STATE);
    self->pPrivate->pConfigRec = xkl_config_rec_new();
    xkl_config_rec_get_from_server(self->pPrivate->pConfigRec, self->pPrivate->pEngine);
    XklState *pState = xkl_engine_get_current_state(self->pPrivate->pEngine);
    self->pPrivate->nLayout = pState->group;
}
