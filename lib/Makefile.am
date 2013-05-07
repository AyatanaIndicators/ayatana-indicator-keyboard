AM_CFLAGS = -DGNOME_DESKTOP_USE_UNSTABLE_API
AM_LDFLAGS = -lm
AM_VALAFLAGS = --metadatadir $(top_srcdir)/deps \
               --vapidir $(top_srcdir)/deps

libexec_PROGRAMS = indicator-keyboard-service

indicator_keyboard_service_SOURCES = main.vala
indicator_keyboard_service_VALAFLAGS = $(AM_VALAFLAGS)         \
                                       --pkg posix             \
                                       --pkg pangocairo        \
                                       --pkg gtk+-3.0          \
                                       --pkg gnome-desktop-3.0 \
                                       --pkg Xkl-1.0           \
                                       --pkg libgnomekbd       \
                                       --pkg ibus-1.0
indicator_keyboard_service_CFLAGS = $(GTK_CFLAGS)           \
                                    $(PANGOCAIRO_CFLAGS)    \
                                    $(GNOME_DESKTOP_CFLAGS) \
                                    $(LIBXKLAVIER_CFLAGS)   \
                                    $(LIBGNOMEKBD_CFLAGS)   \
                                    $(IBUS_CFLAGS)          \
                                    $(AM_CFLAGS)
indicator_keyboard_service_LDFLAGS = $(GTK_LIBS)           \
                                     $(PANGOCAIRO_LIBS)    \
                                     $(GNOME_DESKTOP_LIBS) \
                                     $(LIBXKLAVIER_LIBS)   \
                                     $(LIBGNOMEKBD_LIBS)   \
                                     $(IBUS_LIBS)          \
                                     $(AM_LDFLAGS)
