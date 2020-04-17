cmake_minimum_required (VERSION 3.7)

# Project name
project (logger)

# Project version
set(logger VERSION_MAJOR 1)
set(logger VERSION_MINOR 1)

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
file(GLOB_RECURSE logger_SOURCES "../src/*.cc")
# Set testing application source files
file(GLOB_RECURSE logger_test_SOURCES "../test/*.cc")

# Setup header files path
include_directories(
  "../include"
  "/usr/local/include"
  "/usr/local/include/gtest"
  )

# Add doxygen support
find_package(Doxygen)

# Add googletest support
find_package(GTest REQUIRED)

# Add libpthread support
find_package(Threads REQUIRED)

# Library source files dependencies
add_library(logger SHARED ${logger_SOURCES})

# Testing application source files dependencies
add_executable(test_logger ${logger_test_SOURCES})
target_compile_options(test_logger PUBLIC "-pthread")

# Copy output file into bin directory
target_link_libraries(test_logger LINK_PUBLIC logger ${CONVERTER_LIB_NAME} gtest gtest_main ${CMAKE_THREAD_LIBS_INIT})

# Packaging
set(CMAKE_EXPORT_PACKAGE_REGISTRY ON)
configure_file(loggerConfig.cmake.in loggerConfig.cmake)
export(PACKAGE logger)

# Installation
set_target_properties(logger PROPERTIES PUBLIC_HEADER "../include/logger.hh;../include/logger_factory.hh;../include/logger_levels.hh;../include/logger_time_formats.hh")
install(
  TARGETS logger EXPORT logger
  LIBRARY DESTINATION $ENV{HOME_LIB}
  ARCHIVE DESTINATION $ENV{HOME_LIB}
  PUBLIC_HEADER DESTINATION $ENV{HOME_INC}
  )

# Deinstallation

# Add target to build documents
if(${DOXYGEN_FOUND})
  add_custom_target(doc_${PROJECT_NAME}
    COMMAND ${DOXYGEN_EXECUTABLE} ../docs/o2.cfg
    WORKING_DIRECTORY ../docs
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM
    )
endif(${DOXYGEN_FOUND})

# run commands
add_custom_target(run_${PROJECT_NAME}
  COMMAND ../bin/test_logger
  )

if(${LCOV_FOUND})
  add_custom_target(run_${PROJECT_NAME}_c
    COMMAND ${LCOV_BIN} --directory . --zerocounters
    COMMAND ../bin/test_logger --gtest_output=xml:gtestresults.xml
    COMMAND ${LCOV_BIN} --directory . --output-file test_logger.coverage.info --capture
    ${LCOV_BIN} --list test_logger.coverage.coverage.info
  )
elseif(${GCOV_FOUND})
  add_custom_target(run_${PROJECT_NAME}_c
    COMMAND ${GCOV_BIN} ../bin/test_logger
  )
endif(${LCOV_FOUND})

add_custom_target(run_${PROJECT_NAME}_v
  COMMAND ${VALGRIND} ${VALGRIND_OPTIONS} ../bin/test_logger
  )

add_custom_target(run_${PROJECT_NAME}_d
  COMMAND gdb ../bin/test_logger
  )
