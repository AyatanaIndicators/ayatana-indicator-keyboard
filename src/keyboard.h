/*
 * Copyright 2021-2023 Robert Tari <robert@tari.in>
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

#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include <glib.h>

G_BEGIN_DECLS

#define KEYBOARD_LAYOUT_CHANGED "layout-changed"
#define KEYBOARD_CONFIG_CHANGED "config-changed"

#define G_KEYBOARD(o) (G_TYPE_CHECK_INSTANCE_CAST((o), G_TYPE_KEYBOARD, Keyboard))
#define G_TYPE_KEYBOARD (keyboard_get_type())
#define G_IS_KEYBOARD(o) (G_TYPE_CHECK_INSTANCE_TYPE((o), G_TYPE_KEYBOARD))

typedef struct _Keyboard Keyboard;
typedef struct _KeyboardClass KeyboardClass;
typedef struct _KeyboardPrivate KeyboardPrivate;

struct _Keyboard
{
    GObject parent;
    KeyboardPrivate *pPrivate;
};

struct _KeyboardClass
{
    GObjectClass parent_class;
};

GType keyboard_get_type(void);
Keyboard* keyboard_new();
void keyboard_AddSource(Keyboard *pKeyboard);
guint keyboard_GetNumLayouts(Keyboard *pKeyboard);
guint keyboard_GetLayoutIndex (Keyboard *pKeyboard);
void keyboard_GetLayout(Keyboard *pKeyboard, gint nLayout, gchar **pLanguage, gchar **pDescription);
void keyboard_SetLayout(Keyboard *pKeyboard, gint nLayout);

G_END_DECLS

#endif
