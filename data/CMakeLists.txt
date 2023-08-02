
# Systemd Unit File

pkg_check_modules(SYSTEMD systemd)

if (${SYSTEMD_FOUND})

    pkg_get_variable(SYSTEMD_USER_DIR systemd systemduserunitdir)
    message (STATUS "${SYSTEMD_USER_DIR} is the systemd user unit file install dir")
    set (SYSTEMD_USER_NAME "${CMAKE_PROJECT_NAME}.service")
    set (SYSTEMD_USER_FILE "${CMAKE_CURRENT_BINARY_DIR}/${SYSTEMD_USER_NAME}")
    set (SYSTEMD_USER_FILE_IN "${CMAKE_CURRENT_SOURCE_DIR}/${SYSTEMD_USER_NAME}.in")
    configure_file ("${SYSTEMD_USER_FILE_IN}" "${SYSTEMD_USER_FILE}")
    install (FILES "${SYSTEMD_USER_FILE}" DESTINATION "${SYSTEMD_USER_DIR}")

endif()

# XDG Autostart File

set (XDG_AUTOSTART_DIR "/etc/xdg/autostart")
message (STATUS "${XDG_AUTOSTART_DIR} is the DBus Service File install dir")
set (XDG_AUTOSTART_NAME "${CMAKE_PROJECT_NAME}.desktop")
set (XDG_AUTOSTART_FILE "${CMAKE_CURRENT_BINARY_DIR}/${XDG_AUTOSTART_NAME}")
set (XDG_AUTOSTART_FILE_IN "${CMAKE_CURRENT_SOURCE_DIR}/${XDG_AUTOSTART_NAME}.in")
configure_file ("${XDG_AUTOSTART_FILE_IN}" "${XDG_AUTOSTART_FILE}")
install (FILES "${XDG_AUTOSTART_FILE}" DESTINATION "${XDG_AUTOSTART_DIR}")

# Ayatana Indicators File

set (AYATANA_INDICATOR_DIR "${CMAKE_INSTALL_FULL_DATADIR}/ayatana/indicators")
message (STATUS "${AYATANA_INDICATOR_DIR} is the Ayatana Indicators install dir")
set (AYATANA_INDICATOR_NAME "org.ayatana.indicator.keyboard")
set (AYATANA_INDICATOR_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${AYATANA_INDICATOR_NAME}")
install (FILES "${AYATANA_INDICATOR_FILE}" DESTINATION "${AYATANA_INDICATOR_DIR}")

# Icons

install (DIRECTORY "icons/" DESTINATION "${CMAKE_INSTALL_FULL_DATADIR}/icons")

# org.ayatana.indicator.keyboard.AccountsService.xml

install(FILES org.ayatana.indicator.keyboard.AccountsService.xml DESTINATION "${CMAKE_INSTALL_FULL_DATADIR}/dbus-1/interfaces")
install(DIRECTORY DESTINATION "${CMAKE_INSTALL_FULL_DATADIR}/accountsservice/interfaces/")
install(CODE "message(STATUS \"Symlinking: \$ENV{DESTDIR}${CMAKE_INSTALL_FULL_DATADIR}/accountsservice/interfaces/org.ayatana.indicator.keyboard.AccountsService.xml\")")
install(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ../../dbus-1/interfaces/org.ayatana.indicator.keyboard.AccountsService.xml \$ENV{DESTDIR}${CMAKE_INSTALL_FULL_DATADIR}/accountsservice/interfaces/org.ayatana.indicator.keyboard.AccountsService.xml)")

# org.ayatana.indicator.keyboard.AccountsService.policy

install(FILES org.ayatana.indicator.keyboard.AccountsService.policy DESTINATION "${CMAKE_INSTALL_FULL_DATADIR}/polkit-1/actions")

# 50-org.ayatana.indicator.keyboard.AccountsService.pkla

install(FILES 50-org.ayatana.indicator.keyboard.AccountsService.pkla DESTINATION "${CMAKE_INSTALL_FULL_LOCALSTATEDIR}/lib/polkit-1/localauthority/10-vendor.d")

# org.ayatana.indicator.keyboard.gschema.xml

find_package (GSettings REQUIRED)
add_schema ("org.ayatana.indicator.keyboard.gschema.xml")
