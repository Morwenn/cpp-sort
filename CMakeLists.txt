# Copyright (c) 2015-2021 Morwenn
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.8.0)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

project(cpp-sort VERSION 1.12.1 LANGUAGES CXX)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# Project options
option(BUILD_TESTING "Build the cpp-sort test suite (deprecated, use CPPSORT_BUILD_TESTING)" ON)
option(BUILD_EXAMPLES "Build the cpp-sort examples (deprecated, use CPPSORT_BUILD_EXAMPLES)" OFF)
option(CPPSORT_BUILD_TESTING "Build the cpp-sort test suite" ${BUILD_TESTING})
option(CPPSORT_BUILD_EXAMPLES "Build the cpp-sort examples" ${BUILD_EXAMPLES})

# Create cpp-sort library and configure it
add_library(cpp-sort INTERFACE)
target_include_directories(cpp-sort INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  
    $<INSTALL_INTERFACE:include>
)

target_compile_features(cpp-sort INTERFACE cxx_std_14)

# MSVC won't work without a stricter standard compliance
if (MSVC)
    target_compile_options(cpp-sort INTERFACE /permissive-)
endif()

add_library(cpp-sort::cpp-sort ALIAS cpp-sort)

# Install targets and files
install(
    TARGETS cpp-sort
    EXPORT cpp-sort-targets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
    EXPORT cpp-sort-targets
    NAMESPACE cpp-sort::
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/cpp-sort"
)

install(
    DIRECTORY "include/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/cpp-sort-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/cmake/cpp-sort-config.cmake
    INSTALL_DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/cmake/cpp-sort
)

# Bypass automatic architeture check introduced by CMake,
# use the ARCH_INDEPENDENT option for this in the future
set(CPPSORT_SIZEOF_VOID_P ${CMAKE_SIZEOF_VOID_P})
unset(CMAKE_SIZEOF_VOID_P)
write_basic_package_version_file(
    ${CMAKE_BINARY_DIR}/cmake/cpp-sort-config-version.cmake
    COMPATIBILITY
        SameMajorVersion
)
set(CMAKE_SIZEOF_VOID_P ${CPPSORT_SIZEOF_VOID_P})

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/cmake/cpp-sort-config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/cmake/cpp-sort-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cpp-sort
)

# Export target so that it can be used in subdirectories
export(
    EXPORT cpp-sort-targets
    FILE ${CMAKE_CURRENT_BINARY_DIR}/cmake/cpp-sort-targets.cmake
    NAMESPACE cpp-sort::
)

# Build tests and/or examples if this is the main project
if (PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    if (CPPSORT_BUILD_TESTING)
        enable_testing()
        add_subdirectory(tests)
    endif()

    if (CPPSORT_BUILD_EXAMPLES)
        add_subdirectory(examples)
    endif()
endif()
