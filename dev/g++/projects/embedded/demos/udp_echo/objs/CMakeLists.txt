cmake_minimum_required (VERSION 3.5.1)
# Use G++/GCC
set($ENV{CC} gcc)
set($ENV{CXX} g++)

# Project name
project (udp_echo CXX)

# Compile C files as C++ files
#set_source_files_properties(filename.c PROPERTIES LANGUAGE CXX)

# Project version
set(udp_echo VERSION_MAJOR 1)
set(udp_echo VERSION_MINOR 1)

# Setup source files
file(GLOB_RECURSE udp_echo_SOURCES "../src/*.cpp")

# Setup header files path
include_directories($ENV{HOME_INC})
include_directories("../include")

# Add compile flags
add_definitions(-g -ggdb -O0 -Wall -MMD -MP -Wno-uninitialized -Wno-unused-parameter -std=c++11 -fmessage-length=0 -D_DEBUG -fPIC)

# Declare shared libraries to be used
link_directories($ENV{HOME_LIB})

# Binary source files dependencies
add_executable(udp_echo ${udp_echo_SOURCES})
target_link_libraries(udp_echo logger helper comm)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin")
