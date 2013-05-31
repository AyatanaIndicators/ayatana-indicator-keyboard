AC_PREREQ([2.69])

AC_INIT([indicator-keyboard], [0.0.0])
AC_REVISION([0.0.0])

AC_CONFIG_SRCDIR([configure.ac])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([1.11 foreign -Wall -Woverride -Wno-portability])
AM_SILENT_RULES([yes])

LT_INIT

AC_PROG_CC
AM_PROG_VALAC
AC_PROG_LIBTOOL
AC_PROG_INSTALL
PKG_PROG_PKG_CONFIG([0.26])

AC_ARG_ENABLE([local-install], AS_HELP_STRING([--enable-local-install], [Enable local install]))
AS_IF([test "x$enable_local_install" = "xyes"], [dbus_service_dir=$prefix/share/dbus-1/services], [dbus_service_dir=`pkg-config --variable=session_bus_services_dir dbus-1`])

AC_ARG_WITH([dbus-service-dir], [AS_HELP_STRING([--with-dbus-service-dir=DIR], [D-Bus service directory [default=$dbus_service_dir]])], [], [with_dbus_service_dir=$dbus_service_dir])
AC_ARG_WITH([indicator-dir], [AS_HELP_STRING([--with-indicator-dir=DIR], [Indicator directory [default=$prefix/share/unity/indicators]])], [], [with_indicator_dir=$prefix/share/unity/indicators])

AC_SUBST([DBUS_SERVICE_DIR], [$with_dbus_service_dir])
AC_SUBST([INDICATOR_DIR], [$with_indicator_dir])

GETTEXT_PACKAGE=indicator-keyboard
AC_SUBST([GETTEXT_PACKAGE])
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE], ["$GETTEXT_PACKAGE"], [The prefix for our gettext translation domains.])
AM_GLIB_GNU_GETTEXT
IT_PROG_INTLTOOL

GLIB_GSETTINGS

PKG_CHECK_MODULES([GTK], [gtk+-3.0])
AC_SUBST([GTK_CFLAGS])
AC_SUBST([GTK_LIBS])

PKG_CHECK_MODULES([PANGOCAIRO], [pangocairo])
AC_SUBST([PANGOCAIRO_CFLAGS])
AC_SUBST([PANGOCAIRO_LIBS])

PKG_CHECK_MODULES([GNOME_DESKTOP], [gnome-desktop-3.0])
AC_SUBST([GNOME_DESKTOP_CFLAGS])
AC_SUBST([GNOME_DESKTOP_LIBS])

PKG_CHECK_MODULES([LIBXKLAVIER], [libxklavier])
AC_SUBST([LIBXKLAVIER_CFLAGS])
AC_SUBST([LIBXKLAVIER_LIBS])

PKG_CHECK_MODULES([LIBGNOMEKBD], [libgnomekbdui])
AC_SUBST([LIBGNOMEKBD_CFLAGS])
AC_SUBST([LIBGNOMEKBD_LIBS])

PKG_CHECK_MODULES([IBUS], [ibus-1.0])
AC_SUBST([IBUS_CFLAGS])
AC_SUBST([IBUS_LIBS])

AC_CONFIG_FILES([Makefile
                 data/Makefile
                 lib/Makefile
                 po/Makefile.in])

AC_OUTPUT
