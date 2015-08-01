cmake_minimum_required(VERSION 2.8.8)

#
# cpp-sort is a header-only library. Therefore, no build whatsoever
# is needed to use the project. However, it includes a testsuite that
# can optionally be built with CMake. This is what this script does.
#

# cpp-sort should work fine with C++14
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

# Include cpp-sort headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Include Catch in the project and build the testsuite
add_subdirectory(external/catch)
add_subdirectory(testsuite)
