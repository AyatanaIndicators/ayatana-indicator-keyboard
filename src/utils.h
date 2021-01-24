/*
 * Copyright 2021 Marius Gripsgard <marius@ubports.com>
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


#pragma once

#include <glib.h>

#define DESKTOP_LOMIRI  "Lomiri"
#define DESKTOP_UNITY  "Unity"
#define DESKTOP_MATE   "MATE"
#define DESKTOP_GNOME  "GNOME"
#define DESKTOP_XFCE   "XFCE"
#define DESKTOP_PANTHEON  "PANTHEON"

gboolean is_lomiri();
gboolean is_unity();
gboolean is_gnome();
gboolean is_mate();
gboolean is_xfce();
gboolean is_pantheon();

gboolean execute_command(const gchar * cmd);
gboolean open_url(const gchar * url);
gboolean have_program(const gchar * program);
