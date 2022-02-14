cmake_minimum_required(VERSION 3.13)

# libayatana-keyboard-backend-x11.so

add_library("ayatana-keyboard-x11" SHARED keyboard-x11.c)
target_link_libraries("ayatana-keyboard-x11" ${SERVICE_DEPS_LIBRARIES} ${X11_DEPS_LIBRARIES})
set_target_properties("ayatana-keyboard-x11" PROPERTIES VERSION 0.0.0 SOVERSION 0)
install(TARGETS "ayatana-keyboard-x11" DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR})

# libayatana-keyboard-backend-lomiri.so

add_library("ayatana-keyboard-lomiri" SHARED keyboard-lomiri.c)
target_link_libraries("ayatana-keyboard-lomiri" ${SERVICE_DEPS_LIBRARIES})
set_target_properties("ayatana-keyboard-lomiri" PROPERTIES VERSION 0.0.0 SOVERSION 0)
install(TARGETS "ayatana-keyboard-lomiri" DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR})

# libayatanaindicatorkeyboardservice.a

add_definitions(-DG_LOG_DOMAIN="ayatana-indicator-keyboard")
add_library("ayatanaindicatorkeyboardservice" STATIC service.c)
include_directories(${CMAKE_SOURCE_DIR})
link_directories(${SERVICE_DEPS_LIBRARY_DIRS})

# ayatana-indicator-keyboard-service

add_executable("ayatana-indicator-keyboard-service" main.c)
target_link_libraries("ayatana-indicator-keyboard-service" "ayatanaindicatorkeyboardservice" "${SERVICE_DEPS_LIBRARIES} -ldl")
install(TARGETS "ayatana-indicator-keyboard-service" RUNTIME DESTINATION "${CMAKE_INSTALL_FULL_LIBEXECDIR}/${CMAKE_PROJECT_NAME}")
