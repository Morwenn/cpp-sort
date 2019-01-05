cmake_minimum_required(VERSION 3.8.0)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

project(cpp-sort VERSION 1.3.0 LANGUAGES CXX)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# Project options
option(BUILD_TESTING "Build the cpp-sort test suite" ON)
option(ENABLE_COVERAGE "Whether to make suitable build for code coverage" OFF)
option(USE_VALGRIND "Whether to run the tests with Valgrind" OFF)

# Create cpp-sort library and configure it
add_library(cpp-sort INTERFACE)
target_include_directories(cpp-sort INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  
    $<INSTALL_INTERFACE:include>
)

target_compile_features(cpp-sort INTERFACE cxx_std_14)

add_library(cpp-sort::cpp-sort ALIAS cpp-sort)

# Install targets and files
install(
	TARGETS
		cpp-sort
	EXPORT
		cpp-sort-targets
	DESTINATION
		${CMAKE_INSTALL_LIBDIR}
)

install(
	EXPORT
		cpp-sort-targets
	NAMESPACE
		cpp-sort::
	DESTINATION
		"${CMAKE_INSTALL_LIBDIR}/cmake/cpp-sort"
)

install(
	DIRECTORY
		"include/"
	DESTINATION
		"${CMAKE_INSTALL_INCLUDEDIR}"
)

configure_package_config_file(
    ${CMAKE_SOURCE_DIR}/cmake/cpp-sort-config.cmake.in
    ${CMAKE_BINARY_DIR}/cmake/cpp-sort-config.cmake
    INSTALL_DESTINATION
		${CMAKE_INSTALL_LIBDIR}/cmake/cpp-sort
)

write_basic_package_version_file(
    ${CMAKE_BINARY_DIR}/cmake/cpp-sort-config-version.cmake
    COMPATIBILITY
		SameMajorVersion
)

install(
    FILES
        ${CMAKE_BINARY_DIR}/cmake/cpp-sort-config.cmake
        ${CMAKE_BINARY_DIR}/cmake/cpp-sort-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cpp-sort
)

# Export target so that it can be used in subdirectories
export(EXPORT cpp-sort-targets
    FILE
		${CMAKE_BINARY_DIR}/cmake/cpp-sort-targets.cmake
    NAMESPACE
		cpp-sort::
)

# Build tests if this is the main project
if (BUILD_TESTING AND (PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR))
    enable_testing()
    add_subdirectory(testsuite)
endif()
