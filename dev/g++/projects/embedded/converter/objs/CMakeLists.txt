cmake_minimum_required (VERSION 3.7)

# Project name
project (converter)

# Project version
set(converter VERSION_MAJOR 1)
set(converter VERSION_MINOR 1)

# Compile C files as C++ files
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

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

# Library source files dependencies
add_library(converter SHARED ${converter_SOURCES})

# Testing application source files dependencies
add_executable(test_converter ${converter_test_SOURCES})

# Add doxygen support
find_package(Doxygen)

# Add googletest support
find_package(GTest REQUIRED)

# Add libpthread support
find_package(Threads REQUIRED)
target_compile_options(test_converter PUBLIC "-pthread")

# Copy output file into bin directory
target_link_libraries(test_converter LINK_PUBLIC converter gtest gtest_main ${CMAKE_THREAD_LIBS_INIT})

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

# Add target to build documents
if(${DOXYGEN_FOUND})
  add_custom_target(doc
    COMMAND ${DOXYGEN_EXECUTABLE} ../docs/o2.cfg
    WORKING_DIRECTORY ../docs/o2.cfg
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM
    )
endif(${DOXYGEN_FOUND})

# run commands
add_custom_target(run
  COMMAND ../bin/test_converter
  )

if(${LCOV_FOUND})
  add_custom_target(run_c
    COMMAND ${LCOV_BIN} --directory . --zerocounters
    COMMAND ../bin/test_converter --gtest_output=xml:gtestresults.xml
    COMMAND ${LCOV_BIN} --directory . --output-file test_converter.coverage.info --capture
    ${LCOV_BIN} --list test_converter.coverage.coverage.info
  )
elseif(${GCOV_FOUND})
  add_custom_target(run_c
    COMMAND ${GCOV_BIN} ../bin/test_converter
  )
endif(${LCOV_FOUND})

add_custom_target(run_v
  COMMAND ${VALGRIND} ${VALGRIND_OPTIONS} ../bin/test_converter
  )

add_custom_target(run_d
  COMMAND gdb ../bin/test_converter
  )
