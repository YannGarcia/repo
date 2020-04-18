cmake_minimum_required (VERSION 3.7)

# Project name
project (embedded)

# Project version
set(embedded VERSION_MAJOR 1)
set(embedded VERSION_MINOR 1)

# Compile C files as C++ files
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add doxygen support
find_package(Doxygen)

# Add lcov support
find_package(Lcov)

# Add gcov support
find_package(Gcov)

# Add doxygen support
find_package(Doxygen)

# Add googletest support
find_package(GTest REQUIRED)

# Add libpthread support
find_package(Threads REQUIRED)

# Build type. Default Release
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release) # Possible value Debug, Valgrind and Coverage
endif(NOT CMAKE_BUILD_TYPE)

# Build type. Default Release
if(CMAKE_BUILD_TYPE MATCHES Release)
  message("Set Release mode")
  set(CMAKE_CXX_FLAGS "-O3 -Wall -MMD -MP -std=c++11 -fmessage-length=0 -D_DEBUG -fPIC")
elseif(CMAKE_BUILD_TYPE MATCHES Debug)
  message("-- Set Debug mode")
  set(CMAKE_CXX_FLAGS "-g -ggdb -O0 -Wall -MMD -MP -std=c++11 -fmessage-length=0 -D_DEBUG -fPIC")
elseif(CMAKE_BUILD_TYPE MATCHES Coverage)
  set(CMAKE_CXX_FLAGS "-g -ggdb -O0 -Wall -MMD -MP -std=c++11 -fmessage-length=0 -D_DEBUG -fPIC --coverage")
  set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} --coverage")
  if(${LCOV_FOUND})
    message("Set Coverage mode with LCOV")
  elseif(${GCOV_FOUND})
    message("-- Set Coverage mode with GCOV")
  endif(${LCOV_FOUND})
elseif(CMAKE_BUILD_TYPE MATCHES Valgrind)
  message("Set Coverage mode")
  set(VALGRIND "valgrind")
  set(VALGRIND_OPTIONS "-v --leak-check=yes --leak-check=full --show-leak-kinds=all --track-origins=yes")
  set(CMAKE_CXX_FLAGS "-g -ggdb -O0 -Wall -MMD -MP -std=c++11 -fmessage-length=0 -D_DEBUG -fPIC --coverage")
else(1)
  message("Set Debug mode")
  set(CMAKE_CXX_FLAGS "-g -ggdb -O0 -Wall -MMD -MP -std=c++11 -fmessage-length=0 -D_DEBUG -fPIC")
endif(CMAKE_BUILD_TYPE MATCHES Release)

# Add sub directories
add_subdirectory(logger/objs)
add_subdirectory(converter/objs)
add_subdirectory(helper/objs)
add_subdirectory(ipc/objs)
add_subdirectory(comm/objs)
add_subdirectory(security/objs)

# Custom targets
add_custom_target(distclean
  # cmake files production
  COMMAND make clean &&
    find . -name 'CMakeCache.txt' -type f -exec rm -f {} "\;" &&
    find . -name '*.cmake' -type f -exec rm -f {} "\;" &&
    find . -name 'install_manifest.txt' -type f -exec rm -f {} "\;" &&
    find . -name 'Makefile' -type f -exec rm -f {} "\;" &&
    find . -name 'CMakeDoxyfile.in' -type f -exec rm -f {} "\;" &&
    find . -name '*~' -type f -exec rm -f {} "\;" &&
    find . -name 'CMakeFiles' -type d -exec rm -fr {} "\;" &&
    find . -name 'html' -type d -exec rm -fr {} "\;" &&
    find . -name 'latex' -type d -exec rm -fr {} "\;" &&
    find . -name 'man' -type d -exec rm -fr {} "\;"
  )

configure_file (
  "deinstall.cmake.in"
  "deinstall.cmake"
  IMMEDIATE @ONLY
  )

add_custom_target(deinstall
  COMMAND ${CMAKE_COMMAND} -P "deinstall.cmake"
  OUTPUT_VARIABLE rm_out
  RESULT_VARIABLE rm_retval
  )
