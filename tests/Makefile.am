TESTS = indicator-keyboard-test

check_PROGRAMS = indicator-keyboard-test

AM_CFLAGS = -w
AM_LDFLAGS = -lm
AM_VALAFLAGS = --enable-experimental-non-null   \
               --metadatadir $(top_srcdir)/deps \
               --vapidir $(top_srcdir)/deps

indicator_keyboard_test_SOURCES = main.vala   \
                                  config.vala
indicator_keyboard_test_VALAFLAGS = $(AM_VALAFLAGS) \
                                    --pkg posix     \
                                    --pkg gio-2.0
indicator_keyboard_test_CFLAGS = $(AM_CFLAGS)  \
                                 $(GIO_CFLAGS)
indicator_keyboard_test_LDFLAGS = $(AM_LDFLAGS) \
                                  $(GIO_LIBS)
