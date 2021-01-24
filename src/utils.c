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

#include "utils.h"

#include <string.h>

#ifdef HAS_URLDISPATCHER
# include <url-dispatcher.h>
#endif

// TODO: make case insensitive
gboolean
is_xdg_current_desktop (const gchar* desktop)
{
  const gchar *xdg_current_desktop;
  gchar **desktop_names;
  int i;

  xdg_current_desktop = g_getenv ("XDG_CURRENT_DESKTOP");
  if (xdg_current_desktop != NULL) {
    desktop_names = g_strsplit (xdg_current_desktop, ":", 0);
    for (i = 0; desktop_names[i]; ++i) {
      if (!g_strcmp0 (desktop_names[i], desktop)) {
        g_strfreev (desktop_names);
        return TRUE;
      }
    }
    g_strfreev (desktop_names);
  }
  return FALSE;
}

gboolean
is_lomiri ()
{
  // For legacy reasons keep the MIR_SOCKET hack
  return (g_getenv ("MIR_SOCKET") != NULL ||
          is_xdg_current_desktop(DESKTOP_LOMIRI));
}

gboolean
is_gnome ()
{
  return is_xdg_current_desktop(DESKTOP_GNOME);
}

gboolean
is_unity ()
{
  return is_xdg_current_desktop(DESKTOP_UNITY);
}

gboolean
is_mate ()
{
  return is_xdg_current_desktop(DESKTOP_MATE);
}

gboolean
is_xfce ()
{
  return is_xdg_current_desktop(DESKTOP_XFCE);
}

gboolean
is_pantheon ()
{
  return is_xdg_current_desktop(DESKTOP_PANTHEON);
}

// Bit of a hacky way? should use xdg open
char *
find_browser ()
{
  static char * browser_path = NULL;
  char* tmp_browser_path;
  gchar **browser_names;

  int i;

  if (browser_path == NULL)
  {
    browser_names = g_strsplit ("x-www-browser,google-chrome,firefox,chromium", ",", 0);

    for (i = 0; browser_names[i]; ++i) {
      tmp_browser_path = g_find_program_in_path (browser_names[i]);

      if (tmp_browser_path) {
        browser_path = g_strdup (tmp_browser_path);
        g_free (tmp_browser_path);
        g_strfreev (browser_names);
        break;
      }
    }
  }

  return browser_path;
}

gboolean
execute_command (const gchar * cmd)
{
  GError * err = NULL;

  g_debug ("Issuing command '%s'", cmd);

  if (!g_spawn_command_line_async (cmd, &err))
  {
    g_warning ("Unable to start %s: %s", cmd, err->message);
    g_error_free (err);
    return FALSE;
  }

  return TRUE;
}

gboolean
open_url (const gchar * url)
{
  char * browser = NULL;

  if (is_lomiri())
  {
#ifdef HAS_URLDISPATCHER
    url_dispatch_send("settings:///system/battery", NULL, NULL);
    return TRUE;
#else
    g_warning("Built without url-dispatcher, is not able to open url");
#endif
  }

  if (browser == NULL)
    browser = find_browser();

  if (browser != NULL)
    return execute_command(g_strdup_printf("%s '%s'", browser, url));
  else
    return FALSE;

}

gboolean
have_program (const gchar * program)
{
  gchar *path;
  gboolean have;

  path = g_find_program_in_path(program);
  have = path != NULL;
  g_free(path);

  return have;
}
