###########################
# Upstart XDG Override
###########################

xdg_autostartdir = $(datadir)/upstart/xdg/autostart
xdg_autostart_DATA = indicator-keyboard.desktop

%.desktop: %.desktop.in
	$(AM_V_GEN) sed -e "s|\@pkglibexecdir\@|$(pkglibexecdir)|" $< > $@

EXTRA_DIST = indicator-keyboard.desktop.in

