cmake_minimum_required (VERSION 3.7)

# Project name
project (embedded)

# Project version
set(embedded VERSION_MAJOR 1)
set(embedded VERSION_MINOR 1)

# Compile C files as C++ files
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

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
    find . -name '*~' -type f -exec rm -f {} "\;" &&
    find . -name 'CMakeFiles' -type d -exec rm -fr {} "\;"
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
