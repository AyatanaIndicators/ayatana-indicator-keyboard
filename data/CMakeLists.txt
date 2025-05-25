
# ayatana-indicator-keyboard.service

pkg_check_modules (SYSTEMD systemd)

if (${SYSTEMD_FOUND})
    pkg_get_variable (SYSTEMD_USER_DIR systemd systemduserunitdir)
    configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_PROJECT_NAME}.service.in" "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.service")
    install (FILES "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.service" DESTINATION "${SYSTEMD_USER_DIR}")
endif ()

# ayatana-indicator-keyboard.desktop

configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_PROJECT_NAME}.desktop.in" "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.desktop")
install (FILES "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.desktop" DESTINATION "${CMAKE_INSTALL_FULL_SYSCONFDIR}/xdg/autostart")

# org.ayatana.indicator.keyboard

install (FILES "${CMAKE_CURRENT_SOURCE_DIR}/org.ayatana.indicator.keyboard" DESTINATION "${CMAKE_INSTALL_FULL_DATADIR}/ayatana/indicators")

# icons

install (DIRECTORY "icons/" DESTINATION "${CMAKE_INSTALL_FULL_DATADIR}/icons")

# org.ayatana.indicator.keyboard.AccountsService.xml

install (FILES org.ayatana.indicator.keyboard.AccountsService.xml DESTINATION "${CMAKE_INSTALL_FULL_DATADIR}/dbus-1/interfaces")
install (DIRECTORY DESTINATION "${CMAKE_INSTALL_FULL_DATADIR}/accountsservice/interfaces/")
install (CODE "message(STATUS \"Symlinking: \$ENV{DESTDIR}${CMAKE_INSTALL_FULL_DATADIR}/accountsservice/interfaces/org.ayatana.indicator.keyboard.AccountsService.xml\")")
install (CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ../../dbus-1/interfaces/org.ayatana.indicator.keyboard.AccountsService.xml \$ENV{DESTDIR}${CMAKE_INSTALL_FULL_DATADIR}/accountsservice/interfaces/org.ayatana.indicator.keyboard.AccountsService.xml)")

# org.ayatana.indicator.keyboard.AccountsService.policy

install(FILES org.ayatana.indicator.keyboard.AccountsService.policy DESTINATION "${CMAKE_INSTALL_FULL_DATADIR}/polkit-1/actions")

# 50-org.ayatana.indicator.keyboard.AccountsService.rules

install (FILES 50-org.ayatana.indicator.keyboard.AccountsService.rules DESTINATION "${CMAKE_INSTALL_FULL_DATADIR}/polkit-1/rules.d")

# org.ayatana.indicator.keyboard.gschema.xml

find_package (GSettings REQUIRED)
add_schema ("org.ayatana.indicator.keyboard.gschema.xml")
