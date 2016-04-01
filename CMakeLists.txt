cmake_minimum_required(VERSION 3.1.0)

#
# cpp-sort is a header-only library. Therefore, no build whatsoever
# is needed to use the project. However, it includes a testsuite that
# can optionally be built with CMake. This is what this script does.
#

set(CMAKE_CXX_STANDARD 14)

# Include cpp-sort headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Include Catch in the project and build the testsuite
add_subdirectory(external/catch)
add_subdirectory(testsuite)
