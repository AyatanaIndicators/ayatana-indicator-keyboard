/*
 * Copyright 2024 Robert Tari <robert@tari.in>
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

#ifndef __SYSTEM_LAYOUTS__
#define __SYSTEM_LAYOUTS__

G_BEGIN_DECLS

static void getSystemLayouts (gchar *sLocation, GSList **lLayoutRec, GStrv *lSystemLayouts, gboolean bSystemLayouts)
{
    gboolean bExists = g_file_test (sLocation, G_FILE_TEST_EXISTS);

    if (!bExists)
    {
        return;
    }

    gchar *sFile = NULL;
    GError *pError = NULL;
    g_file_get_contents (sLocation, &sFile, NULL, &pError);

    if (pError)
    {
        g_error_free (pError);

        return;
    }

    GRegex *pRegex = NULL;
    gboolean bDefaultLocation = g_str_equal (sLocation, "/etc/default/keyboard");

    if (bDefaultLocation)
    {
        #if GLIB_CHECK_VERSION(2, 73, 0)
            pRegex = g_regex_new (" *XKBLAYOUT *= *\"*([,a-zA-Z0-9]*)\"*", G_REGEX_DEFAULT, G_REGEX_MATCH_DEFAULT, &pError);
        #else
            pRegex = g_regex_new (" *XKBLAYOUT *= *\"*([,a-zA-Z0-9]*)\"*", (GRegexCompileFlags) 0, (GRegexMatchFlags) 0, &pError);
        #endif
    }
    else
    {
        #if GLIB_CHECK_VERSION(2, 73, 0)
            pRegex = g_regex_new (" *Option +\"*XkbLayout\"* +\"*([,a-zA-Z0-9]*)\"*", G_REGEX_DEFAULT, G_REGEX_MATCH_DEFAULT, &pError);
        #else
            pRegex = g_regex_new (" *Option +\"*XkbLayout\"* +\"*([,a-zA-Z0-9]*)\"*", (GRegexCompileFlags) 0, (GRegexMatchFlags) 0, &pError);
        #endif
    }

    if (pError)
    {
        g_warning ("Panic: Failed to compile regex: %s", pError->message);
        g_error_free (pError);
        g_free(sFile);

        return;
    }

    GMatchInfo *pMatchInfo = NULL;

    #if GLIB_CHECK_VERSION(2, 73, 0)
        gboolean bMatch = g_regex_match (pRegex, sFile, G_REGEX_MATCH_DEFAULT, &pMatchInfo);
    #else
        gboolean bMatch = g_regex_match (pRegex, sFile, (GRegexMatchFlags) 0, &pMatchInfo);
    #endif

    if (!bMatch)
    {
        g_regex_unref (pRegex);
        g_free(sFile);

        return;
    }

    gchar *sLayouts = g_match_info_fetch (pMatchInfo, 1);
    GStrv lLayouts = g_strsplit (sLayouts, ",", -1);
    g_free (sLayouts);
    g_match_info_free (pMatchInfo);
    g_regex_unref (pRegex);

    if (bDefaultLocation)
    {
        #if GLIB_CHECK_VERSION(2, 73, 0)
            pRegex = g_regex_new (" *XKBVARIANT *= *\"*([,a-zA-Z0-9]*)\"*", G_REGEX_DEFAULT, G_REGEX_MATCH_DEFAULT, &pError);
        #else
            pRegex = g_regex_new (" *XKBVARIANT *= *\"*([,a-zA-Z0-9]*)\"*", (GRegexCompileFlags) 0, (GRegexMatchFlags) 0, &pError);
        #endif
    }
    else
    {
        #if GLIB_CHECK_VERSION(2, 73, 0)
            pRegex = g_regex_new (" *Option +\"*XkbVariant\"* +\"*([,a-zA-Z0-9]*)\"*", G_REGEX_DEFAULT, G_REGEX_MATCH_DEFAULT, &pError);
        #else
            pRegex = g_regex_new (" *Option +\"*XkbVariant\"* +\"*([,a-zA-Z0-9]*)\"*", (GRegexCompileFlags) 0, (GRegexMatchFlags) 0, &pError);
        #endif
    }

    if (pError)
    {
        g_warning ("Panic: Failed to compile regex: %s", pError->message);
        g_error_free (pError);
        g_free(sFile);

        return;
    }

    #if GLIB_CHECK_VERSION(2, 73, 0)
        bMatch = g_regex_match (pRegex, sFile, G_REGEX_MATCH_DEFAULT, &pMatchInfo);
    #else
        bMatch = g_regex_match (pRegex, sFile, (GRegexMatchFlags) 0, &pMatchInfo);
    #endif

    if (!bMatch)
    {
        g_regex_unref (pRegex);
        g_free(sFile);

        return;
    }

    gchar *sVariants = g_match_info_fetch (pMatchInfo, 1);
    GStrv lVariants = g_strsplit (sVariants, ",", -1);
    g_free (sVariants);
    g_match_info_free (pMatchInfo);
    g_regex_unref (pRegex);
    g_free(sFile);
    guint nLayouts = g_strv_length (lLayouts);
    GStrvBuilder *pBuilder = NULL;

    if (bSystemLayouts)
    {
        pBuilder = g_strv_builder_new ();
    }

    for (guint nLayout = 0; nLayout < nLayouts; nLayout++)
    {
        gchar *sId = NULL;
        guint nVariantLength = 0;

        if (lVariants && lVariants[nLayout])
        {
            nVariantLength = strlen (lVariants[nLayout]);
        }

        if (nVariantLength)
        {
            sId = g_strconcat (lLayouts[nLayout], "+", lVariants[nLayout], NULL);
        }
        else
        {
            sId = g_strdup (lLayouts[nLayout]);
        }

        *lLayoutRec = g_slist_append (*lLayoutRec, sId);

        if (bSystemLayouts)
        {
            g_strv_builder_add (pBuilder, sId);
        }
    }

    if (bSystemLayouts)
    {
        *lSystemLayouts = g_strv_builder_end (pBuilder);
        g_strv_builder_unref (pBuilder);
    }

    g_strfreev (lLayouts);
    g_strfreev (lVariants);
}

G_END_DECLS

#endif
