# libayatanaindicatorkeyboardservice.a

set (SOURCES service.c)

if (ENABLE_LOMIRI_FEATURES)
    list (APPEND SOURCES keyboard-lomiri.c)
else ()
    list (APPEND SOURCES keyboard-x11.c)
endif ()

add_library ("ayatanaindicatorkeyboardservice" STATIC ${SOURCES})
target_compile_definitions ("ayatanaindicatorkeyboardservice" PUBLIC G_LOG_DOMAIN="${CMAKE_PROJECT_NAME}" LOCALEDIR="${CMAKE_INSTALL_FULL_LOCALEDIR}" GETTEXT_PACKAGE="${CMAKE_PROJECT_NAME}")
target_include_directories ("ayatanaindicatorkeyboardservice" PUBLIC ${SERVICE_DEPS_INCLUDE_DIRS})

if (ENABLE_UBUNTU_ACCOUNTSSERVICE)
    target_compile_definitions ("ayatanaindicatorkeyboardservice" PUBLIC ENABLE_UBUNTU_ACCOUNTSSERVICE)
endif ()

# ayatana-indicator-keyboard-service

add_executable ("ayatana-indicator-keyboard-service" main.c)
target_link_libraries ("ayatana-indicator-keyboard-service" "ayatanaindicatorkeyboardservice" ${SERVICE_DEPS_LIBRARIES})
install (TARGETS "ayatana-indicator-keyboard-service" RUNTIME DESTINATION "${CMAKE_INSTALL_FULL_LIBEXECDIR}/${CMAKE_PROJECT_NAME}")
