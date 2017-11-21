cmake_minimum_required(VERSION 3.1.0)

#
# cpp-sort is a header-only library. Therefore, no build whatsoever
# is needed to use the project. However, it includes a testsuite that
# can optionally be built with CMake. This is what this script does.
#

set(CMAKE_CXX_STANDARD 14)

# Use the gold linker if possible
if (UNIX AND NOT APPLE)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=gold")
endif()

# Optionally enable sanitizers
if (UNIX AND SANITIZE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=${SANITIZE}")
endif()

# Include cpp-sort headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Include Catch in the project and build the testsuite
add_subdirectory(external/catch)
add_subdirectory(testsuite)
