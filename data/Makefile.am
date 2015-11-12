
SUBDIRS = upstart

###########################
# Icon Generator
###########################

noinst_PROGRAMS = ayatana-indicator-keyboard-icon-generator

AM_CFLAGS = -w -DGNOME_DESKTOP_USE_UNSTABLE_API
AM_LDFLAGS = -lm
AM_VALAFLAGS = --enable-experimental-non-null   \
               --metadatadir $(top_srcdir)/deps \
               --vapidir $(top_srcdir)/deps

ayatana_indicator_keyboard_icon_generator_SOURCES = main.vala                     \
                                                    $(top_srcdir)/lib/common.vala
ayatana_indicator_keyboard_icon_generator_VALAFLAGS = $(AM_VALAFLAGS)         \
                                                      --pkg gee-1.0           \
                                                      --pkg fontconfig-2.0    \
                                                      --pkg freetype2-2.0     \
                                                      --pkg PangoFT2-1.0      \
                                                      --pkg gtk+-3.0          \
                                                      --pkg GDesktopEnums-3.0 \
                                                      --pkg GnomeDesktop-3.0  \
                                              --pkg Xkl-1.0
ayatana_indicator_keyboard_icon_generator_CFLAGS = $(AM_CFLAGS)            \
                                                   $(GEE_CFLAGS)           \
                                                   $(PANGOFT2_CFLAGS)      \
                                                   $(GTK_CFLAGS)           \
                                                   $(GNOME_DESKTOP_CFLAGS) \
                                                   $(LIBXKLAVIER_CFLAGS)   \
                                                   $(COVERAGE_CFLAGS)
ayatana_indicator_keyboard_icon_generator_LDFLAGS = $(AM_LDFLAGS)         \
                                                    $(GEE_LIBS)           \
                                                    $(PANGOFT2_LIBS)      \
                                                    $(GTK_LIBS)           \
                                                    $(GNOME_DESKTOP_LIBS) \
                                                    $(LIBXKLAVIER_LIBS)   \
                                                    $(COVERAGE_LDFLAGS)

###########################
# Upstart Config
###########################

upstart_jobsdir = $(datadir)/upstart/sessions/
upstart_jobs_DATA = ayatana-indicator-keyboard.conf

%.conf: %.conf.in
	$(AM_V_GEN) sed -e "s|\@pkglibexecdir\@|$(pkglibexecdir)|" $< > $@

###########################
# XDG Autostart File
###########################

xdg_autostartdir = /etc/xdg/autostart
xdg_autostart_DATA = ayatana-indicator-keyboard.desktop

%.desktop: %.desktop.in
	$(AM_V_GEN) sed -e "s|\@pkglibexecdir\@|$(pkglibexecdir)|" $< > $@

###########################
# Indicator Config
###########################

dist_indicator_DATA = org.ayatana.indicator.keyboard
indicatordir = $(INDICATOR_DIR)

org.ayatana.indicator.keyboard:
	$(AM_V_GEN) (echo '[Indicator Service]';                                              \
	             echo 'Name=indicator-keyboard';                                          \
	             echo 'ObjectPath=/org/ayatana/indicator/keyboard';                     \
	             echo 'Position=80';                                                      \
	             echo '';                                                                 \
	             echo '[desktop]';                                                        \
	             echo 'ObjectPath=/org/ayatana/indicator/keyboard/desktop';             \
	             echo '';                                                                 \
	             echo '[desktop_greeter]';                                                \
	             echo 'ObjectPath=/org/ayatana/indicator/keyboard/desktop_greeter';     \
	             echo '';                                                                 \
	             echo '[desktop_lockscreen]';                                             \
	             echo 'ObjectPath=/org/ayatana/indicator/keyboard/desktop_lockscreen';  \
	             echo '';                                                                 \
	             echo '[ubiquity]';                                                       \
	             echo 'ObjectPath=/org/ayatana/indicator/keyboard/desktop') > $@.tmp && \
	            mv $@.tmp $@

###########################
# GSettings Schemas
###########################

gsettings_SCHEMAS = org.ayatana.indicator.keyboard.gschema.xml

@GSETTINGS_RULES@

check_DATA = gschemas.compiled

gschemas.compiled: $(gsettings_SCHEMAS)
	$(GLIB_COMPILE_SCHEMAS) --strict .

EXTRA_DIST = $(dist_service_DATA)   \
	     $(dist_indicator_DATA) \
	     $(gsettings_SCHEMAS) \
	     indicator-keyboard.conf.in \
	     indicator-keyboard.desktop.in

