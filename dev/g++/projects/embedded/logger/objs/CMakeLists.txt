cmake_minimum_required (VERSION 3.7)

# Project name
project (logger)

# Project version
set(logger VERSION_MAJOR 1)
set(logger VERSION_MINOR 1)

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
file(GLOB_RECURSE logger_SOURCES "../src/*.cc")
# Set testing application source files
file(GLOB_RECURSE logger_test_SOURCES "../test/*.cc")

# Setup header files path
include_directories(
  "../include"
  "/usr/local/include"
  "/usr/local/include/gtest")

# Add compile flags
add_definitions(-g -ggdb -O0 -Wall -MMD -MP -std=c++11 -fmessage-length=0 -D_DEBUG -fPIC)

# Add googletest support
find_package(GTest REQUIRED)

# Add libpthread support
find_package(Threads REQUIRED)

# Library source files dependencies
add_library(logger SHARED ${logger_SOURCES})

# Testing application source files dependencies
add_executable(test_logger ${logger_test_SOURCES})
target_compile_options(test_logger PUBLIC "-pthread")

# Set public shared libraries path
#link_directories($ENV{HOME_LIB})

# Copy output file into bin directory
target_link_libraries(test_logger LINK_PUBLIC logger ${CONVERTER_LIB_NAME} gtest gtest_main ${CMAKE_THREAD_LIBS_INIT})

#ADD_CUSTOM_TARGET(distclean
#  DEPENDS clean
#  COMMAND rm -fr $ENV{HOME}/.cmake/packages/logger
#  COMMAND rm -f $ENV{HOME_INC}/include/logger.hh $ENV{HOME_INC}/include/logger_factory.hh $ENV{HOME_INC}/include/logger_levels.hh $ENV{HOME_INC}/include/logger_time_formats.hh
#  COMMAND rm -f $ENV{HOME_LIB}/liblogger.so
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
configure_file(loggerConfig.cmake.in loggerConfig.cmake)
export(PACKAGE logger)

# Installing
set_target_properties(logger PROPERTIES PUBLIC_HEADER "../include/logger.hh;../include/logger_factory.hh;../include/logger_levels.hh;../include/logger_time_formats.hh")
install(
  TARGETS logger EXPORT logger
  LIBRARY DESTINATION $ENV{HOME_LIB}
  ARCHIVE DESTINATION $ENV{HOME_LIB}
  PUBLIC_HEADER DESTINATION $ENV{HOME_INC}
  )
