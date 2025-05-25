cmake_minimum_required (VERSION 3.13)
project (ayatana-indicator-keyboard VERSION "24.7.2" LANGUAGES C)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    SET (CMAKE_INSTALL_PREFIX "/usr" CACHE PATH "..." FORCE)
endif (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

# Options

option (ENABLE_WERROR "Treat all build warnings as errors" OFF)
option (ENABLE_UBUNTU_ACCOUNTSSERVICE "Build with Ubuntu's fork of accountsservice" OFF)
option (ENABLE_LOMIRI_FEATURES "Build with Lomiri-specific libraries, schemas and media" OFF)

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
set (SERVICE_DEPS glib-2.0 gio-2.0 libayatana-common accountsservice xkbcommon xkbregistry)

if (NOT ENABLE_LOMIRI_FEATURES)
    list (APPEND SERVICE_DEPS x11 libxklavier)
endif ()

pkg_check_modules (SERVICE_DEPS REQUIRED ${SERVICE_DEPS})

# Build

add_subdirectory (src)
add_subdirectory (data)
add_subdirectory (po)

# Info

message (STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
message (STATUS "Build with Ubuntu's fork of accountsservice: ${ENABLE_UBUNTU_ACCOUNTSSERVICE}")
message (STATUS "Build with -Werror: ${ENABLE_WERROR}")
message (STATUS "Build with Lomiri features: ${ENABLE_LOMIRI_FEATURES}")

# Needed by the translation helper shell scripts
set (GETTEXT_PACKAGE "ayatana-indicator-keyboard")
