cmake_minimum_required (VERSION 3.7)

# Project name
project (comm)

# Project version
set(comm VERSION_MAJOR 1)
set(comm VERSION_MINOR 1)

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
file(GLOB_RECURSE comm_SOURCES "../src/*.cc")
# Set testing application source files
file(GLOB_RECURSE comm_test_SOURCES "../test/*.cc")

# Setup header files path
include_directories(
  "../include"
  "../../converter/include"
  "../../helper/include"
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
add_library(comm SHARED ${comm_SOURCES})

# Testing application source files dependencies
add_executable(test_comm ${comm_test_SOURCES})
target_compile_options(test_comm PUBLIC "-pthread")

# Copy output file into bin directory
target_link_libraries(test_comm LINK_PUBLIC comm ${CONVERTER_LIB_NAME} gtest gtest_main ${CMAKE_THREAD_LIBS_INIT})

# Packaging
set(CMAKE_EXPORT_PACKAGE_REGISTRY ON)
configure_file(commConfig.cmake.in commConfig.cmake)
export(PACKAGE comm)

# Installation
set_target_properties(comm PROPERTIES PUBLIC_HEADER "../include/abstract_channel.hh;../include/channel_type.hh;../include/ipv4_socket.hh;../include/ipv6_socket.hh;../include/ipvx_socket.hh;../include/socket.hh;../include/tcp_channel.hh;../include/channel_manager.hh;../include/ipv4_address.hh;../include/ipv6_address.hh;../include/ipvx_address.hh;../include/raw_channel.hh;../include/socket_address.hh;../include/udp_channel.hh")
install(
  TARGETS comm EXPORT comm
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
  COMMAND ../bin/test_comm
  )

if(${LCOV_FOUND})
  add_custom_target(run_${PROJECT_NAME}_c
    COMMAND ${LCOV_BIN} --directory . --zerocounters
    COMMAND ../bin/test_comm --gtest_output=xml:gtestresults.xml
    COMMAND ${LCOV_BIN} --directory . --output-file test_comm.coverage.info --capture
    ${LCOV_BIN} --list test_comm.coverage.coverage.info
  )
elseif(${GCOV_FOUND})
  add_custom_target(run_${PROJECT_NAME}_c
    COMMAND ${GCOV_BIN} ../bin/test_comm
  )
endif(${LCOV_FOUND})

add_custom_target(run_${PROJECT_NAME}_v
  COMMAND ${VALGRIND} ${VALGRIND_OPTIONS} ../bin/test_comm
  )

add_custom_target(run_${PROJECT_NAME}_d
  COMMAND gdb ../bin/test_comm
  )
