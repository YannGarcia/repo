cmake_minimum_required (VERSION 3.7)

# Project name
project (udp_echo)

# Project version
set(udp_echo VERSION_MAJOR 1)
set(udp_echo VERSION_MINOR 1)

# Compile C files as C++ files
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Copy output file into bin directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin")

# Setup source files
file(GLOB_RECURSE udp_echo_SOURCES "../src/*.cc")

# Setup header files path
include_directories($ENV{HOME_INC})
include_directories("../include")

# Add compile flags
add_definitions(-g -ggdb -O0 -Wall -MMD -MP -std=c++11 -fmessage-length=0 -D_DEBUG -fPIC)

# Binary source files dependencies
add_executable(udp_echo ${udp_echo_SOURCES})

# Add libpthread support
find_package(Threads REQUIRED)
target_compile_options(udp_echo PUBLIC "-pthread")

# Declare shared libraries path and,
link_directories($ENV{HOME_LIB})

# Add user librairies
find_library(LIB_HELPER libhelper.so $ENV{HOME_LIB} REQUIRED)
find_library(LIB_COMM libcomm.so $ENV{HOME_LIB} REQUIRED)
find_library(LIB_LOGGER liblogger.so $ENV{HOME_LIB} REQUIRED)

# Add libraries dependencies
target_link_libraries(udp_echo LINK_PUBLIC ${LIB_COMM} ${LIB_LOGGER} ${LIB_HELPER} ${CMAKE_THREAD_LIBS_INIT})
