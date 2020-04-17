cmake_minimum_required (VERSION 3.7)

# Project name
project (converter)

# Project version
set(converter VERSION_MAJOR 1)
set(converter VERSION_MINOR 1)

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
file(GLOB_RECURSE converter_SOURCES "../src/*.cc")
# Set testing application source files
file(GLOB_RECURSE converter_test_SOURCES "../test/*.cc")

# Setup header files path
include_directories(
  "../include"
  "../../helper/include"
  "/usr/local/include"
  "/usr/local/include/gtest")

# Add compile flags
add_definitions(-g -ggdb -O0 -Wall -MMD -MP -std=c++11 -fmessage-length=0 -D_DEBUG -fPIC)

# Library source files dependencies
add_library(converter SHARED ${converter_SOURCES})

# Testing application source files dependencies
add_executable(test_converter ${converter_test_SOURCES})

# Add googletest support
find_package(GTest REQUIRED)

# Add libpthread support
find_package(Threads REQUIRED)
target_compile_options(test_converter PUBLIC "-pthread")

# Copy output file into bin directory
target_link_libraries(test_converter LINK_PUBLIC converter gtest gtest_main ${CMAKE_THREAD_LIBS_INIT})

#ADD_CUSTOM_TARGET(distclean
#  DEPENDS clean
#  COMMAND rm -fr $ENV{HOME}/.cmake/packages/converter
#  COMMAND rm -f $ENV{HOME_INC}/converter.hh
#  COMMAND rm -f $ENV{HOME_LIB}/libconverter.so
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
configure_file(converterConfig.cmake.in converterConfig.cmake)
export(PACKAGE converter)

# Installation
set_target_properties(converter PROPERTIES PUBLIC_HEADER "../include/converter.hh")
install(
  TARGETS converter EXPORT converter
  LIBRARY DESTINATION $ENV{HOME_LIB}
  ARCHIVE DESTINATION $ENV{HOME_LIB}
  PUBLIC_HEADER DESTINATION $ENV{HOME_INC}
  )

# Deinstallation

