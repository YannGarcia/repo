cmake_minimum_required (VERSION 3.7)

# Project name
project (security)

# Project version
set(security VERSION_MAJOR 1)
set(security VERSION_MINOR 1)

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
file(GLOB_RECURSE security_SOURCES "../src/*.cc")
# Set testing application source files
file(GLOB_RECURSE security_test_SOURCES "../test/*.cc")

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

# Add logger support
find_package(logger REQUIRED)
include_directories(${LOGGER_INCLUDE_DIR})
link_directories(${LOGGER_LIB_DIR})

# Add googletest support
find_package(GTest REQUIRED)

# Add libpthread support
find_package(Threads REQUIRED)

# Library source files dependencies
add_library(security SHARED ${security_SOURCES})

# Testing application source files dependencies
add_executable(testlib ${security_test_SOURCES})
target_compile_options(testlib PUBLIC "-pthread")

# Set public shared libraries path
#link_directories($ENV{HOME_LIB})

# Copy output file into bin directory
target_link_libraries(testlib LINK_PUBLIC security ${CONVERTER_LIB_NAME} gtest gtest_main ${CMAKE_THREAD_LIBS_INIT})

# Custom targets
ADD_CUSTOM_TARGET(distclean
  rm -fr $ENV{HOME}/.cmake/packages/security
  rm -f $ENV{HOME_INC}/ecdsa_signature.hh $ENV{HOME_INC}/key.hh key.t.h $ENV{HOME_INC}/keys_pair.hh $ENV{HOME_INC}/rnd.hh $ENV{HOME_INC}/sha.hh $ENV{HOME_INC}/sha_algorithms.hh $ENV{HOME_INC}/signature_algorithms.hh
  rm -f $ENV{HOME_LIB}/libsecurity.so
  rm -fr CMakeCache.txt CMakeFiles cmake_install.cmake securityConfig.cmake install_manifest.txt Makefile *~
  )

# Packaging
set(CMAKE_EXPORT_PACKAGE_REGISTRY ON)
configure_file(securityConfig.cmake.in securityConfig.cmake)
export(PACKAGE security)

# Installing
set_target_properties(security PROPERTIES PUBLIC_HEADER "../include/ecdsa_signature.hh;../include/key.hh;key.t.h;../include/keys_pair.hh;../include/rnd.hh;../include/sha.hh;../include/sha_algorithms.hh;../include/signature_algorithms.hh")
install(
  TARGETS security EXPORT security
  LIBRARY DESTINATION $ENV{HOME_LIB}
  ARCHIVE DESTINATION $ENV{HOME_LIB}
  PUBLIC_HEADER DESTINATION $ENV{HOME_INC}
  )
