libexec_PROGRAMS = indicator-keyboard-service

indicator_keyboard_service_SOURCES = main.vala
indicator_keyboard_service_VALAFLAGS = --pkg gio-2.0 --pkg ibus-1.0 $(AM_VALAFLAGS)
indicator_keyboard_service_CFLAGS = $(GIO_CFLAGS) $(IBUS_CFLAGS) $(AM_CFLAGS)
indicator_keyboard_service_LDFLAGS = $(GIO_LIBS) $(IBUS_LIBS) $(AM_LDFLAGS)
