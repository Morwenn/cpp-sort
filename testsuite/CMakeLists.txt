# Copyright (c) 2015-2021 Morwenn
# SPDX-License-Identifier: MIT

include(cpp-sort-utils)
include(DownloadProject)

########################################
# Test suite options

option(USE_VALGRIND "Whether to run the tests with Valgrind (deprecated, use CPPSORT_USE_VALGRIND)" OFF)
option(ENABLE_COVERAGE "Whether to produce code coverage (deprecated, use CPPSORT_ENABLE_COVERAGE)" OFF)
set(SANITIZE "" CACHE STRING "Comma-separated list of options to pass to -fsanitize (deprecated, use CPPSORT_SANITIZE)")
option(CPPSORT_USE_VALGRIND "Whether to run the tests with Valgrind" ${USE_VALGRIND})
option(CPPSORT_ENABLE_COVERAGE "Whether to produce code coverage" ${ENABLE_COVERAGE})
set(CPPSORT_SANITIZE ${SANITIZE} CACHE STRING "Comma-separated list of options to pass to -fsanitize")

# Apparently ENABLE_COVERAGE is needed either way
if (CPPSORT_ENABLE_COVERAGE)
    set(ENABLE_COVERAGE ON CACHE BOOL "Enable coverage build." FORCE)
endif()

########################################
# Find or download Catch2

message(STATUS "Looking for Catch2 2.6.0+")
find_package(Catch2 2.6.0 QUIET)
if (TARGET Catch2::Catch2)
    get_target_property(Catch2_INCLUDE_DIRECTORY Catch2::Catch2 INTERFACE_INCLUDE_DIRECTORIES)
    message(STATUS "Catch2 found: ${Catch2_INCLUDE_DIRECTORY}")
else()
    message(STATUS "Catch2 not found")
    download_project(PROJ Catch2
                     GIT_REPOSITORY https://github.com/catchorg/Catch2
                     GIT_TAG v2.13.4
                     UPDATE_DISCONNECTED 1
    )
    add_subdirectory(${Catch2_SOURCE_DIR} ${Catch2_BINARY_DIR})
    list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/contrib)
endif()
include(Catch)

########################################
# Compilation tests

# These tests ensure that every public-facing header
# can be included individually without causing any
# compile-time error

foreach(HEADER_FILE
    adapters/container_aware_adapter.h
    adapters/counting_adapter.h
    adapters/hybrid_adapter.h
    adapters/indirect_adapter.h
    adapters/out_of_place_adapter.h
    adapters/schwartz_adapter.h
    adapters/self_sort_adapter.h
    adapters/small_array_adapter.h
    adapters/stable_adapter.h
    adapters/verge_adapter.h
    adapters.h
    comparators/case_insensitive_less.h
    comparators/natural_less.h
    comparators/partial_greater.h
    comparators/partial_less.h
    comparators/projection_compare.h
    comparators/total_greater.h
    comparators/total_less.h
    comparators/weak_greater.h
    comparators/weak_less.h
    fixed/low_comparisons_sorter.h
    fixed/low_moves_sorter.h
    fixed/sorting_network_sorter.h
    fixed_sorters.h
    fwd.h
    probes/dis.h
    probes/enc.h
    probes/exc.h
    probes/ham.h
    probes/inv.h
    probes/max.h
    probes/mono.h
    probes/osc.h
    probes/par.h
    probes/rem.h
    probes/runs.h
    probes/sus.h
    probes.h
    refined.h
    sort.h
    sorters/block_sorter.h
    sorters/counting_sorter.h
    sorters/default_sorter.h
    sorters/drop_merge_sorter.h
    sorters/grail_sorter.h
    sorters/heap_sorter.h
    sorters/insertion_sorter.h
    sorters/merge_insertion_sorter.h
    sorters/merge_sorter.h
    sorters/pdq_sorter.h
    sorters/poplar_sorter.h
    sorters/quick_merge_sorter.h
    sorters/quick_sorter.h
    sorters/selection_sorter.h
    sorters/ska_sorter.h
    sorters/smooth_sorter.h
    sorters/spin_sorter.h
    sorters/split_sorter.h
    sorters/spread_sorter/float_spread_sorter.h
    sorters/spread_sorter/integer_spread_sorter.h
    sorters/spread_sorter/string_spread_sorter.h
    sorters/spread_sorter.h
    sorters/std_sorter.h
    sorters/tim_sorter.h
    sorters/verge_sorter.h
    sorters.h
    sorter_facade.h
    sorter_traits.h
    stable_sort.h
    utility/adapter_storage.h
    utility/as_function.h
    utility/branchless_traits.h
    utility/buffer.h
    utility/functional.h
    utility/iter_move.h
    utility/make_integer_range.h
    utility/size.h
    utility/static_const.h
    version.h
)
    # Create standalone test file
    string(REPLACE "/" "-" TEST_FILE "${HEADER_FILE}")
    get_filename_component(TEST_FILE ${TEST_FILE} NAME_WLE)
    set(TEST_FILE "test-${TEST_FILE}")
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/header-test.cpp.in "${TEST_FILE}.cpp" @ONLY)

    # Compile standalone test file
    add_executable(${TEST_FILE} "${TEST_FILE}.cpp")
    target_link_libraries(${TEST_FILE} PRIVATE cpp-sort::cpp-sort)
    target_compile_definitions(${TEST_FILE} PRIVATE CPPSORT_DISABLE_DEPRECATION_WARNINGS)
endforeach()

########################################
# Configure runtime tests

macro(configure_tests target)
    # Make testing tools easily available to tests
    # regardless of the directory of the test
    target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

    target_link_libraries(${target} PRIVATE
        Catch2::Catch2
        cpp-sort::cpp-sort
    )

    target_compile_definitions(${target} PRIVATE
        # Somewhat speed up Catch2 compile times
        CATCH_CONFIG_FAST_COMPILE
        CATCH_CONFIG_DISABLE_MATCHERS
        # Enable assertions for more thorough tests
        CPPSORT_ENABLE_ASSERTIONS
        # We test deprecated code but we don't want it to warn
        CPPSORT_DISABLE_DEPRECATION_WARNINGS
    )

    # More warnings and settings
    cppsort_add_warnings(${target})
    target_compile_options(${target} PRIVATE
        $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:Clang>>:-O0>
        $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:GNU>>:-Og>
    )

    # Use lld or the gold linker if possible
    if (UNIX AND NOT APPLE)
        if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
            set_property(TARGET ${target} APPEND_STRING PROPERTY LINK_FLAGS " -fuse-ld=lld")
        else()
            set_property(TARGET ${target} APPEND_STRING PROPERTY LINK_FLAGS " -fuse-ld=gold")
        endif()
    endif()

    # Optionally enable sanitizers
    if (UNIX AND CPPSORT_SANITIZE)
        target_compile_options(${target} PRIVATE
            -fsanitize=${CPPSORT_SANITIZE}
            -fno-sanitize-recover=all
        )
        set_property(TARGET ${target}
            APPEND_STRING PROPERTY LINK_FLAGS
                " -fsanitize=${CPPSORT_SANITIZE}"
        )
    endif()

    if (CPPSORT_ENABLE_COVERAGE)
        find_package(codecov)
        add_coverage(${target})

        # Set flags specific to coverage builds
        target_compile_options(${target} PRIVATE --coverage)
        set_property(TARGET ${target} APPEND_STRING PROPERTY LINK_FLAGS " --coverage")
    endif()
endmacro()

########################################
# Main tests

add_executable(main-tests
    # General tests
    main.cpp
    every_instantiated_sorter.cpp
    every_sorter.cpp
    every_sorter_internal_compare.cpp
    every_sorter_long_string.cpp
    every_sorter_move_compare_projection.cpp
    every_sorter_move_only.cpp
    every_sorter_no_post_iterator.cpp
    every_sorter_non_const_compare.cpp
    every_sorter_span.cpp
    is_stable.cpp
    rebind_iterator_category.cpp
    sort_array.cpp
    sorter_facade.cpp
    sorter_facade_defaults.cpp
    sorter_facade_iterable.cpp
    stable_sort_array.cpp

    # Adapters tests
    adapters/container_aware_adapter.cpp
    adapters/container_aware_adapter_forward_list.cpp
    adapters/container_aware_adapter_list.cpp
    adapters/counting_adapter.cpp
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:adapters/every_adapter_fptr.cpp>
    adapters/every_adapter_internal_compare.cpp
    adapters/every_adapter_non_const_compare.cpp
    adapters/every_adapter_stateful_sorter.cpp
    adapters/hybrid_adapter_is_stable.cpp
    adapters/hybrid_adapter_many_sorters.cpp
    adapters/hybrid_adapter_nested.cpp
    adapters/hybrid_adapter_partial_compare.cpp
    adapters/hybrid_adapter_sfinae.cpp
    adapters/indirect_adapter.cpp
    adapters/indirect_adapter_every_sorter.cpp
    adapters/mixed_adapters.cpp
    adapters/return_forwarding.cpp
    adapters/schwartz_adapter_every_sorter.cpp
    adapters/schwartz_adapter_every_sorter_reversed.cpp
    adapters/schwartz_adapter_fixed_sorters.cpp
    adapters/self_sort_adapter.cpp
    adapters/self_sort_adapter_no_compare.cpp
    adapters/small_array_adapter.cpp
    adapters/small_array_adapter_is_stable.cpp
    adapters/stable_adapter_every_sorter.cpp
    adapters/verge_adapter_every_sorter.cpp

    # Comparators tests
    comparators/case_insensitive_less.cpp
    comparators/natural_less.cpp
    comparators/total_less.cpp
    comparators/transparent_comparators.cpp

    # Distributions tests
    distributions/all_equal.cpp
    distributions/alternating.cpp
    distributions/alternating_16_values.cpp
    distributions/ascending.cpp
    distributions/ascending_sawtooth.cpp
    distributions/descending.cpp
    distributions/descending_sawtooth.cpp
    distributions/median_of_3_killer.cpp
    distributions/pipe_organ.cpp
    distributions/push_front.cpp
    distributions/push_middle.cpp
    distributions/shuffled.cpp
    distributions/shuffled_16_values.cpp

    # Probes tests
    probes/dis.cpp
    probes/enc.cpp
    probes/exc.cpp
    probes/ham.cpp
    probes/inv.cpp
    probes/max.cpp
    probes/mono.cpp
    probes/osc.cpp
    probes/par.cpp
    probes/rem.cpp
    probes/runs.cpp
    probes/sus.cpp
    probes/relations.cpp
    probes/every_probe_common.cpp
    probes/every_probe_move_compare_projection.cpp

    # Sorters tests
    sorters/counting_sorter.cpp
    sorters/default_sorter.cpp
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:sorters/default_sorter_fptr.cpp>
    sorters/default_sorter_projection.cpp
    sorters/merge_insertion_sorter_projection.cpp
    sorters/merge_sorter.cpp
    sorters/merge_sorter_projection.cpp
    sorters/poplar_sorter.cpp
    sorters/ska_sorter.cpp
    sorters/ska_sorter_projection.cpp
    sorters/spin_sorter.cpp
    sorters/spread_sorter.cpp
    sorters/spread_sorter_defaults.cpp
    sorters/spread_sorter_projection.cpp
    sorters/std_sorter.cpp

    # Utilities tests
    utility/adapter_storage.cpp
    utility/as_projection.cpp
    utility/as_projection_iterable.cpp
    utility/branchless_traits.cpp
    utility/buffer.cpp
    utility/chainable_projections.cpp
    utility/iter_swap.cpp
)
configure_tests(main-tests)

########################################
# Heap memory exhaustion tests

if (NOT "${CPPSORT_SANITIZE}" MATCHES "address|memory")
    add_executable(heap-memory-exhaustion-tests
        # These tests are in a separate executable because we replace
        # the global [de]allocation functions in order to test the
        # algorithms that have a fallback when heap exhaustion occurs,
        # which isn't something we want for the main tests
        main.cpp
        testing-tools/new_delete.cpp
        heap_memory_exhaustion.cpp
        probes/every_probe_heap_memory_exhaustion.cpp
    )
    configure_tests(heap-memory-exhaustion-tests)
endif()

########################################
# Configure coverage

if (CPPSORT_ENABLE_COVERAGE)
    list(APPEND LCOV_REMOVE_PATTERNS "'/usr/*'")
    coverage_evaluate()
endif()

########################################
# Configure Valgrind

if (CPPSORT_USE_VALGRIND)
    find_program(MEMORYCHECK_COMMAND valgrind REQUIRED)
    set(MEMORYCHECK_COMMAND_OPTIONS "--leak-check=full --track-origins=yes --error-exitcode=1 --show-reachable=no")
    if (APPLE)
        set(MEMORYCHECK_SUPPRESSIONS_FILE ${CMAKE_CURRENT_SOURCE_DIR}/valgrind-osx.supp)
    endif()
endif()

########################################
# Discover tests

include(CTest)

string(RANDOM LENGTH 5 ALPHABET 0123456789 RNG_SEED)
catch_discover_tests(main-tests EXTRA_ARGS --rng-seed ${RNG_SEED})
if (NOT "${CPPSORT_SANITIZE}" MATCHES "address|memory")
    catch_discover_tests(heap-memory-exhaustion-tests EXTRA_ARGS --rng-seed ${RNG_SEED})
endif()
