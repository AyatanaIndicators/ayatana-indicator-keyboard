cmake_minimum_required (VERSION 3.13)
project (ayatana-indicator-keyboard VERSION "24.7.2" LANGUAGES C)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    SET (CMAKE_INSTALL_PREFIX "/usr" CACHE PATH "..." FORCE)
endif (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

# Options

option (ENABLE_WERROR "Treat all build warnings as errors" OFF)
option (ENABLE_UBUNTU_ACCOUNTSSERVICE "Build with Ubuntu's fork of accountsservice" OFF)

set (CMAKE_BUILD_TYPE "Release")

if (ENABLE_WERROR)
    add_definitions ("-Werror")
endif ()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_definitions ("-Weverything")
else ()
    add_definitions ("-Wall")
endif ()

# Prerequisites

include (GNUInstallDirs)
find_package (PkgConfig REQUIRED)
include (FindPkgConfig)
pkg_check_modules (SERVICE_DEPS REQUIRED glib-2.0 gio-2.0 libayatana-common accountsservice xkbcommon xkbregistry
pkg_check_modules (X11_DEPS REQUIRED x11 libxklavier)

# Build

add_subdirectory (src)
add_subdirectory (data)
add_subdirectory (po)

# Info

message (STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
message (STATUS "Build with Ubuntu's fork of accountsservice: ${ENABLE_UBUNTU_ACCOUNTSSERVICE}")
message (STATUS "Build with -Werror: ${ENABLE_WERROR}")

# Needed by the translation helper shell scripts
set (GETTEXT_PACKAGE "ayatana-indicator-keyboard")
