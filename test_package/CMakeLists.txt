cmake_minimum_required(VERSION 2.8.12)

project(CppSortTest CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(example cpp-sort-integrity.cpp)
set_property(TARGET example PROPERTY CXX_STANDARD 14)
