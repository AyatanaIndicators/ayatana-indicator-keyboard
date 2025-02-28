cmake_minimum_required (VERSION 3.13)
project (ayatana-indicator-keyboard VERSION "24.7.1" LANGUAGES C CXX)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    SET(CMAKE_INSTALL_PREFIX "/usr" CACHE PATH "..." FORCE)
endif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# Options
option(ENABLE_WERROR "Treat all build warnings as errors" OFF)
option(ENABLE_UBUNTU_ACCOUNTSSERVICE "Build with Ubuntu's fork of accountsservice" OFF)

set(CMAKE_BUILD_TYPE "Release")

if(ENABLE_WERROR)
    add_definitions("-Werror")
endif()

if(ENABLE_UBUNTU_ACCOUNTSSERVICE)
    add_compile_definitions(ENABLE_UBUNTU_ACCOUNTSSERVICE)
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_definitions("-Weverything")
else()
    add_definitions("-Wall")
endif()

# GNU standard installation directories
include (GNUInstallDirs)

# Gettext
set(GETTEXT_PACKAGE "ayatana-indicator-keyboard")
add_definitions (-DGETTEXT_PACKAGE="${GETTEXT_PACKAGE}" -DLOCALEDIR="${CMAKE_INSTALL_FULL_LOCALEDIR}")

# Check for prerequisites
find_package (PkgConfig REQUIRED)
include (CheckIncludeFile)
include (FindPkgConfig)
pkg_check_modules(SERVICE_DEPS REQUIRED glib-2.0>=2.36 gio-2.0>=2.36 libayatana-common>=0.9.11 accountsservice xkbcommon>=1.0.3 xkbregistry>=1.0.3)
pkg_check_modules(X11_DEPS REQUIRED x11>=1.6.5 libxklavier>=5.3)
include_directories (SYSTEM ${SERVICE_DEPS_INCLUDE_DIRS})

# custom targets
set (ARCHIVE_NAME ${CMAKE_PROJECT_NAME}-${PROJECT_VERSION})
add_custom_target (dist COMMAND bzr export --root=${ARCHIVE_NAME} ${CMAKE_BINARY_DIR}/${ARCHIVE_NAME}.tar.gz WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
add_custom_target (cppcheck COMMAND cppcheck --enable=all -q --error-exitcode=2 --inline-suppr ${CMAKE_SOURCE_DIR}/src)

# Actual building
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories (${CMAKE_CURRENT_BINARY_DIR}/include)
add_subdirectory(src)
add_subdirectory(data)
add_subdirectory(po)

# Display config info
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Build with Ubuntu's fork of accountsservice: ${ENABLE_UBUNTU_ACCOUNTSSERVICE}")
message(STATUS "Build with -Werror: ${ENABLE_WERROR}")
