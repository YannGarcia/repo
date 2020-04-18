cmake_minimum_required (VERSION 3.7)

# Project name
project (helper)

# Project version
set(helper VERSION_MAJOR 1)
set(helper VERSION_MINOR 1)

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
file(GLOB_RECURSE helper_SOURCES "../src/*.cc")
# Set testing application source files
file(GLOB_RECURSE helper_test_SOURCES "../test/*.cc")

# Setup header files path
include_directories(
  "../include"
  "../../converter/include"
  "/usr/local/include"
  "/usr/local/include/gtest")

# Add converter support
find_package(converter REQUIRED)
include_directories(${CONVERTER_INCLUDE_DIR})
link_directories(${CONVERTER_LIB_DIR})

# Add doxygen support
find_package(Doxygen)

# Add googletest support
find_package(GTest REQUIRED)

# Add libpthread support
find_package(Threads REQUIRED)

# Library source files dependencies
add_library(helper SHARED ${helper_SOURCES})

# Testing application source files dependencies
add_executable(test_helper ${helper_test_SOURCES})
target_compile_options(test_helper PUBLIC "-pthread")

# Copy output file into bin directory
target_link_libraries(test_helper LINK_PUBLIC helper ${CONVERTER_LIB_NAME} gtest gtest_main ${CMAKE_THREAD_LIBS_INIT})

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
  COMMAND ../bin/test_helper
  )

if(${LCOV_FOUND})
  add_custom_target(run_${PROJECT_NAME}_c
    COMMAND ${LCOV_BIN} --directory . --zerocounters
    COMMAND ../bin/test_helper --gtest_output=xml:gtestresults.xml
    COMMAND ${LCOV_BIN} --directory . --output-file test_helper.coverage.info --capture
    ${LCOV_BIN} --list test_helper.coverage.coverage.info
  )
elseif(${GCOV_FOUND})
  add_custom_target(run_${PROJECT_NAME}_c
    COMMAND ${GCOV_BIN} ../bin/test_helper
  )
endif(${LCOV_FOUND})

add_custom_target(run_${PROJECT_NAME}_v
  COMMAND ${VALGRIND} ${VALGRIND_OPTIONS} ../bin/test_helper
  )

add_custom_target(run_${PROJECT_NAME}_d
  COMMAND gdb ../bin/test_helper
  )
