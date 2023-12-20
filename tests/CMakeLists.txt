# Copyright (c) 2015-2023 Morwenn
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.24.0)

include(cpp-sort-utils)
include(FetchContent)

########################################
# Test suite options

option(CPPSORT_USE_VALGRIND "Whether to run the tests with Valgrind" OFF)
option(CPPSORT_ENABLE_COVERAGE "Whether to produce code coverage" OFF)
set(CPPSORT_SANITIZE "" CACHE STRING "Comma-separated list of options to pass to -fsanitize")
option(CPPSORT_STATIC_TESTS "Whether to turn some tests into static assertions" OFF)

########################################
# Find or download Catch2

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2
    GIT_TAG v3.5.0
    SYSTEM
    FIND_PACKAGE_ARGS 3.5.0
)
FetchContent_MakeAvailable(Catch2)
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)

########################################
# Configure coverage

if (CPPSORT_ENABLE_COVERAGE)
    set(ENABLE_COVERAGE ON CACHE BOOL "Enable coverage build." FORCE)
    find_package(codecov)
    list(APPEND LCOV_REMOVE_PATTERNS "'/usr/*'")
endif()

########################################
# Configure runtime tests

macro(configure_tests target)
    # Make testing tools easily available to tests
    # regardless of the directory of the test
    target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

    target_link_libraries(${target} PRIVATE
        Catch2::Catch2WithMain
        cpp-sort::cpp-sort
    )

    target_compile_definitions(${target} PRIVATE
        # Somewhat speed up Catch2 compile times
        CATCH_CONFIG_FAST_COMPILE
        # Enable assertions for more thorough tests
        _GLIBCXX_ASSERTIONS
        _LIBCPP_ENABLE_ASSERTIONS=1
        CPPSORT_ENABLE_ASSERTIONS
        # We test deprecated code but we don't want it to warn
        CPPSORT_DISABLE_DEPRECATION_WARNINGS
        # Conditionally turn some tests into static assertions
        $<$<NOT:$<BOOL:${CPPSORT_STATIC_TESTS}>>:CATCH_CONFIG_RUNTIME_STATIC_REQUIRE>
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
        add_coverage(${target})
    endif()
endmacro()

########################################
# Main tests

add_executable(main-tests
    # Tooling
    testing-tools/random.cpp

    # General utilities tests
    is_stable.cpp
    rebind_iterator_category.cpp
    sorter_facade.cpp
    sorter_facade_constexpr.cpp
    sorter_facade_defaults.cpp
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:sorter_facade_fptr.cpp>
    sorter_facade_range.cpp

    # Adapters tests
    adapters/container_aware_adapter.cpp
    adapters/container_aware_adapter_forward_list.cpp
    adapters/container_aware_adapter_list.cpp
    adapters/counting_adapter.cpp
    adapters/every_adapter_constexpr.cpp
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:adapters/every_adapter_fptr.cpp>
    adapters/every_adapter_internal_compare.cpp
    adapters/every_adapter_non_const_compare.cpp
    adapters/every_adapter_sentinel.cpp
    adapters/every_adapter_stateful_sorter.cpp
    adapters/every_adapter_tricky_difference_type.cpp
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
    adapters/split_adapter_every_sorter.cpp
    adapters/stable_adapter_every_sorter.cpp
    adapters/verge_adapter_every_sorter.cpp

    # Comparators tests
    comparators/case_insensitive_less.cpp
    comparators/flip_not.cpp
    comparators/natural_less.cpp
    comparators/projection_compare.cpp
    comparators/total_less.cpp
    comparators/transparent_comparators.cpp

    # Distributions tests
    distributions/all_equal.cpp
    distributions/alternating.cpp
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
    probes/block.cpp
    probes/dis.cpp
    probes/enc.cpp
    probes/exc.cpp
    probes/ham.cpp
    probes/inv.cpp
    probes/max.cpp
    probes/mono.cpp
    probes/osc.cpp
    probes/rem.cpp
    probes/runs.cpp
    probes/sus.cpp
    probes/relations.cpp
    probes/every_probe_common.cpp
    probes/every_probe_constexpr.cpp
    probes/every_probe_move_compare_projection.cpp
    probes/every_probe_sentinel.cpp

    # Sorters tests
    sorters/counting_sorter.cpp
    sorters/default_sorter.cpp
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:sorters/default_sorter_fptr.cpp>
    sorters/default_sorter_projection.cpp
    sorters/every_instantiated_sorter.cpp
    sorters/every_sorter_constexpr.cpp
    sorters/every_sorter_counted_iterator.cpp
    sorters/every_sorter_internal_compare.cpp
    sorters/every_sorter_long_string.cpp
    sorters/every_sorter_move_compare_projection.cpp
    sorters/every_sorter_move_only.cpp
    sorters/every_sorter_no_post_iterator.cpp
    sorters/every_sorter_non_const_compare.cpp
    sorters/every_sorter_rvalue_projection.cpp
    sorters/every_sorter_sentinel.cpp
    sorters/every_sorter_small_collections.cpp
    sorters/every_sorter_span.cpp
    sorters/every_sorter_throwing_moves.cpp
    sorters/every_sorter_tricky_difference_type.cpp
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
    utility/apply_permutation.cpp
    utility/as_comparison.cpp
    utility/as_projection.cpp
    utility/as_projection_range.cpp
    utility/branchless_traits.cpp
    utility/buffer.cpp
    utility/chainable_projections.cpp
    utility/iter_swap.cpp
    utility/sorted_indices.cpp
    utility/sorted_iterators.cpp
    utility/sorting_networks.cpp
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
        testing-tools/new_delete.cpp
        testing-tools/random.cpp
        probes/every_probe_heap_memory_exhaustion.cpp
        sorters/every_sorter_heap_memory_exhaustion.cpp
    )
    configure_tests(heap-memory-exhaustion-tests)
endif()

########################################
# Configure Valgrind

if (CPPSORT_USE_VALGRIND)
    find_program(MEMORYCHECK_COMMAND valgrind REQUIRED)
    set(MEMORYCHECK_COMMAND_OPTIONS "--leak-check=full --track-origins=yes --error-exitcode=1 --show-reachable=no")
endif()

########################################
# Discover tests

include(CTest)

string(RANDOM LENGTH 6 ALPHABET 123456789 RNG_SEED)
catch_discover_tests(main-tests EXTRA_ARGS --rng-seed ${RNG_SEED})
if (NOT "${CPPSORT_SANITIZE}" MATCHES "address|memory")
    catch_discover_tests(heap-memory-exhaustion-tests EXTRA_ARGS --rng-seed ${RNG_SEED})
endif()
