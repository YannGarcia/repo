cmake_minimum_required (VERSION 3.7)

# Project name
project (ipc)

# Project version
set(ipc VERSION_MAJOR 1)
set(ipc VERSION_MINOR 1)

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
file(GLOB_RECURSE ipc_SOURCES "../src/*.cc")
# Set testing application source files
file(GLOB_RECURSE ipc_test_SOURCES "../test/*.cc")

# Setup header files path
include_directories(
  "../include"
  "/usr/local/include"
  "/usr/local/include/gtest")

# Add compile flags
add_definitions(-g -ggdb -O0 -Wall -MMD -MP -std=c++11 -fmessage-length=0 -D_DEBUG -fPIC)

# Add converter support
find_package(logger REQUIRED)
include_directories(${LOGGER_INCLUDE_DIR})
link_directories(${LOGGER_LIB_DIR})

# Add googletest support
find_package(GTest REQUIRED)

# Add libpthread support
find_package(Threads REQUIRED)

# Library source files dependencies
add_library(ipc SHARED ${ipc_SOURCES})

# Testing application source files dependencies
add_executable(test_ipc ${ipc_test_SOURCES})
target_compile_options(test_ipc PUBLIC "-pthread")

# Set public shared libraries path
#link_directories($ENV{HOME_LIB})

# Copy output file into bin directory
target_link_libraries(test_ipc LINK_PUBLIC ipc ${CONVERTER_LIB_NAME} gtest gtest_main ${CMAKE_THREAD_LIBS_INIT})

# Custom targets
#ADD_CUSTOM_TARGET(distclean
#  DEPENDS clean
#  COMMAND rm -fr $ENV{HOME}/.cmake/packages/ipc
#  COMMAND rm -f $ENV{HOME_INC}/include/abstract_ipc.hh $ENV{HOME_INC}/include/ipc_manager.hh $ENV{HOME_INC}/include/ipc_rights.hh $ENV{HOME_INC}/include/ipc_type.hh $ENV{HOME_INC}/include/message_queue.hh $ENV{HOME_INC}/include/shared_memory.hh
#  COMMAND rm -f $ENV{HOME_LIB}/libipc.so
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
configure_file(ipcConfig.cmake.in ipcConfig.cmake)
export(PACKAGE ipc)

# Installation
set_target_properties(ipc PROPERTIES PUBLIC_HEADER "../include/abstract_ipc.hh;../include/ipc_manager.hh;../include/ipc_rights.hh;../include/ipc_type.hh;../include/message_queue.hh;../include/shared_memory.hh")
install(
  TARGETS ipc EXPORT ipc
  LIBRARY DESTINATION $ENV{HOME_LIB}
  ARCHIVE DESTINATION $ENV{HOME_LIB}
  PUBLIC_HEADER DESTINATION $ENV{HOME_INC}
  )

# Deinstallation
