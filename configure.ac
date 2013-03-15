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

AC_ARG_WITH([dbus-service-dir], [AS_HELP_STRING([--with-dbus-service-dir=DIR], [D-Bus service directory [default=`pkg-config --variable=session_bus_services_dir dbus-1`]])], [], [with_dbus_service_dir=`pkg-config --variable=session_bus_services_dir dbus-1`])
AC_ARG_WITH([indicator-dir], [AS_HELP_STRING([--with-indicator-dir=DIR], [Indicator directory [default=$sysconfdir/indicators]])], [], [with_indicator_dir=$sysconfdir/indicators])

AC_SUBST([DBUS_SERVICE_DIR], [$with_dbus_service_dir])
AC_SUBST([INDICATOR_DIR], [$with_indicator_dir])

PKG_CHECK_MODULES([GIO], [gio-2.0])
AC_SUBST(GIO_CFLAGS)
AC_SUBST(GIO_LIBS)

AC_CONFIG_FILES([Makefile
                 data/Makefile
                 data/indicator-keyboard.service
                 lib/Makefile])

AC_OUTPUT
