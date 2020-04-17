cmake_minimum_required (VERSION 3.7)

# Project name
project (helper)

# Project version
set(helper VERSION_MAJOR 1)
set(helper VERSION_MINOR 1)

# Compile C files as C++ files
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Build type. Default Release
#set(DEFAULT_BUILD_TYPE "Release")
set(DEFAULT_BUILD_TYPE "Debug")

# Install directories
include(GNUInstallDirs)

# Copy output file into lib directory
set(EXECUTABLE_OUTPUT_PATH "../bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin")
# Copy output file into bin directory
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "../lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "../lib")

# Setup library source files
file(GLOB_RECURSE helper_SOURCES "../src/*.cc")
# Set testing application source files
file(GLOB_RECURSE helper_test_SOURCES "../test/*.cc")

# Setup header files path
include_directories(
  "../include"
  "/usr/local/include"
  "/usr/local/include/gtest")

# Add compile flags
add_definitions(-g -ggdb -O0 -Wall -MMD -MP -std=c++11 -fmessage-length=0 -D_DEBUG -fPIC)

# Add converter support
find_package(converter REQUIRED)
include_directories(${CONVERTER_INCLUDE_DIR})
link_directories(${CONVERTER_LIB_DIR})

# Add googletest support
find_package(GTest REQUIRED)

# Add libpthread support
find_package(Threads REQUIRED)

# Library source files dependencies
add_library(helper SHARED ${helper_SOURCES})

# Testing application source files dependencies
add_executable(test_helper ${helper_test_SOURCES})
target_compile_options(test_helper PUBLIC "-pthread")

# Set public shared libraries path
#link_directories($ENV{HOME_LIB})

# Copy output file into bin directory
target_link_libraries(test_helper LINK_PUBLIC helper ${CONVERTER_LIB_NAME} gtest gtest_main ${CMAKE_THREAD_LIBS_INIT})

#ADD_CUSTOM_TARGET(distclean
#  DEPENDS clean
#  COMMAND rm -fr $ENV{HOME}/.cmake/packages/helper
#  COMMAND rm -f $ENV{HOME_INC}/date_time.hh $ENV{HOME_INC}/get_opt.hh $ENV{HOME_INC}/helper.hh $ENV{HOME_INC}/helper.t.h $ENV{HOME_INC}/ibstream.hh $ENV{HOME_INC}/ibstream.t.h $ENV{HOME_INC}/keyboard.hh $ENV{HOME_INC}/obstream.hh $ENV{HOME_INC}/obstream.t.h $ENV{HOME_INC}/runnable.hh
#  rm -f $ENV{HOME_LIB}/libhelper.so
#  # cmake files production
#  COMMAND find . -name "CMakeFiles" -type d -exec rm -fr {} "\;"
#  COMMAND find . -name "CMakeCache.txt" -type f -exec rm -f {} "\;"
#  COMMAND find . -name "*.cmake" -type f -exec rm -f {} "\;"
#  COMMAND find . -name "install_manifest.txt" -type f -exec rm -f {} "\;"
#  COMMAND find . -name "Makefile" -type f -exec rm -f {} "\;"
#  COMMAND find . -name "*~" -type f -exec rm -f {} "\;"
#  )

# Packaging
set(CMAKE_EXPORT_PACKAGE_REGISTRY ON)
configure_file(helperConfig.cmake.in helperConfig.cmake)
export(PACKAGE helper)

# Installation
set_target_properties(helper PROPERTIES PUBLIC_HEADER "../include/date_time.hh;../include/get_opt.hh;../include/helper.hh;../include/helper.t.h;../include/ibstream.hh;../include/ibstream.t.h;../include/keyboard.hh;../include/obstream.hh;../include/obstream.t.h;../include/runnable.hh")
install(
  TARGETS helper EXPORT helper
  LIBRARY DESTINATION $ENV{HOME_LIB}
  ARCHIVE DESTINATION $ENV{HOME_LIB}
  PUBLIC_HEADER DESTINATION $ENV{HOME_INC}
  )

# Deinstallation
