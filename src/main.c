#include <locale.h>
#include <glib.h>
#include <glib/gi18n.h>
#include <glib-unix.h>
#include "service.h"

static void onNameLost(gpointer instance G_GNUC_UNUSED, gpointer pLoop)
{
    g_message("exiting: service couldn't acquire or lost ownership of busname");
    g_main_loop_quit((GMainLoop*)pLoop);
}

static gboolean onQuit(gpointer pData)
{
    GMainLoop *pLoop = (GMainLoop*)pData;
    g_main_loop_quit(pLoop);

    return G_SOURCE_REMOVE;
}

int main(int argc G_GNUC_UNUSED, char ** argv G_GNUC_UNUSED)
{
    setlocale(LC_ALL, "");
    bindtextdomain(GETTEXT_PACKAGE, LOCALEDIR);
    textdomain(GETTEXT_PACKAGE);

    IndicatorKeyboardService *pService = indicator_keyboard_service_new(NULL);
    GMainLoop *pLoop = g_main_loop_new(NULL, FALSE);

    g_signal_connect(pService, "name-lost", G_CALLBACK(onNameLost), pLoop);
    g_unix_signal_add(SIGINT, onQuit, pLoop);

    indicator_keyboard_service_AddKeyboardSource(pService);

    g_main_loop_run(pLoop);
    g_main_loop_unref(pLoop);
    g_clear_object(&pService);

    return 0;
}
