AM_VALAFLAGS = --vapidir $(top_srcdir)/deps

libexec_PROGRAMS = indicator-keyboard-service

indicator_keyboard_service_SOURCES = main.vala
indicator_keyboard_service_VALAFLAGS = $(AM_VALAFLAGS)   \
                                       --pkg gtk+-3.0    \
                                       --pkg ibus-1.0    \
                                       --pkg libgnomekbd \
                                       --pkg Xkl-1.0
indicator_keyboard_service_CFLAGS = $(GIO_CFLAGS)         \
                                    $(IBUS_CFLAGS)        \
                                    $(LIBGNOMEKBD_CFLAGS) \
                                    $(AM_CFLAGS)
indicator_keyboard_service_LDFLAGS = $(GIO_LIBS)         \
                                     $(IBUS_LIBS)        \
                                     $(LIBGNOMEKBD_LIBS) \
                                     $(AM_LDFLAGS)
