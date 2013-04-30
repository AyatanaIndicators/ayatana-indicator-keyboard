servicedir = $(DBUS_SERVICE_DIR)
dist_service_DATA = indicator-keyboard.service

indicator-keyboard.service:
	$(AM_V_GEN) (echo '[D-BUS Service]';                                           \
	             echo 'Name=com.canonical.indicator.keyboard';                     \
	             echo 'Exec=${libexecdir}/indicator-keyboard-service') > $@.tmp && \
	            mv $@.tmp $@

indicatordir = $(INDICATOR_DIR)
dist_indicator_DATA = indicator-keyboard.indicator

indicator-keyboard.indicator:
	$(AM_V_GEN) (echo '[Indicator Service]';                                      \
	             echo 'Name=indicator-keyboard';                                  \
	             echo 'BusName=com.canonical.indicator.keyboard';                 \
	             echo 'ObjectPath=/com/canonical/indicator/keyboard') > $@.tmp && \
	            mv $@.tmp $@

gsettings_SCHEMAS = com.canonical.indicator.keyboard.gschema.xml

@GSETTINGS_RULES@

EXTRA_DIST = $(dist_service_DATA)   \
	     $(dist_indicator_DATA) \
	     $(gsettings_SCHEMAS)
