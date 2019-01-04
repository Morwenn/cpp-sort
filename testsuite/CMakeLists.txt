include(DownloadProject)

# Download and configure Catch2 for the tests
download_project(PROJ Catch2
                 GIT_REPOSITORY https://github.com/catchorg/Catch2
                 GIT_TAG master
                 UPDATE_DISCONNECTED 1
)
add_subdirectory(${Catch2_SOURCE_DIR} ${Catch2_BINARY_DIR})
list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/contrib)
include(Catch)

# Define test files

set(
    ADAPTERS_TESTS

    adapters/container_aware_adapter.cpp
    adapters/container_aware_adapter_forward_list.cpp
    adapters/container_aware_adapter_list.cpp
    adapters/counting_adapter.cpp
    adapters/every_adapter_fptr.cpp
    adapters/every_adapter_internal_compare.cpp
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
)

set(
    COMPARATORS_TESTS

    comparators/case_insensitive_less.cpp
    comparators/natural_less.cpp
    comparators/total_less.cpp
)

set(
    DISTRIBUTIONS_TESTS

    distributions/all_equal.cpp
    distributions/alternating.cpp
    distributions/alternating_16_values.cpp
    distributions/ascending.cpp
    distributions/ascending_sawtooth.cpp
    distributions/descending.cpp
    distributions/descending_sawtooth.cpp
    distributions/pipe_organ.cpp
    distributions/push_front.cpp
    distributions/push_middle.cpp
    distributions/shuffled.cpp
    distributions/shuffled_16_values.cpp
)

set(
    PROBES_TESTS

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
    probes/relations.cpp
)

set(
    SORTERS_TESTS

    sorters/counting_sorter.cpp
    sorters/default_sorter.cpp
    sorters/default_sorter_fptr.cpp
    sorters/default_sorter_projection.cpp
    sorters/merge_insertion_sorter_projection.cpp
    sorters/merge_sorter.cpp
    sorters/merge_sorter_projection.cpp
    sorters/poplar_sorter.cpp
    sorters/ska_sorter.cpp
    sorters/ska_sorter_projection.cpp
    sorters/spread_sorter.cpp
    sorters/spread_sorter_defaults.cpp
    sorters/spread_sorter_projection.cpp
    sorters/std_sorter.cpp
)

set(
    UTILITY_TESTS

    utility/as_projection.cpp
    utility/as_projection_iterable.cpp
    utility/branchless_traits.cpp
    utility/buffer.cpp
    utility/iter_swap.cpp
)

# Make one executable for the whole testsuite
add_executable(
    cpp-sort-testsuite

    main.cpp
    every_instantiated_sorter.cpp
    every_sorter.cpp
    every_sorter_internal_compare.cpp
    every_sorter_move_only.cpp
    every_sorter_no_post_iterator.cpp
    every_sorter_span.cpp
    is_stable.cpp
    rebind_iterator_category.cpp
    sorter_facade.cpp
    sorter_facade_defaults.cpp
    sorter_facade_iterable.cpp
    ${ADAPTERS_TESTS}
    ${COMPARATORS_TESTS}
    ${DISTRIBUTIONS_TESTS}
    ${PROBES_TESTS}
    ${SORTERS_TESTS}
    ${UTILITY_TESTS}
)

target_link_libraries(cpp-sort-testsuite
    PRIVATE
        Catch2::Catch2
        cpp-sort::cpp-sort
)

# Somewhat speed up Catch2 compile times
target_compile_definitions(cpp-sort-testsuite PRIVATE CATCH_CONFIG_FAST_COMPILE)
target_compile_definitions(cpp-sort-testsuite PRIVATE CATCH_CONFIG_DISABLE_MATCHERS)

# Use lld or the gold linker if possible
if (UNIX AND NOT APPLE)
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set_property(TARGET cpp-sort-testsuite APPEND_STRING PROPERTY LINK_FLAGS " -fuse-ld=lld")
    else()
        set_property(TARGET cpp-sort-testsuite APPEND_STRING PROPERTY LINK_FLAGS " -fuse-ld=gold")
    endif()
endif()

# Optionally enable sanitizers
if (UNIX AND SANITIZE)
	target_compile_options(cpp-sort-testsuite PRIVATE -fsanitize=${SANITIZE})
	set_property(TARGET cpp-sort-testsuite APPEND_STRING PROPERTY LINK_FLAGS " -fsanitize=${SANITIZE}")
endif()

if (ENABLE_COVERAGE)
    find_package(codecov)
    add_coverage(cpp-sort-testsuite)

	# Set flags specific to coverage builds
	target_compile_options(cpp-sort-testsuite PRIVATE -g -O0 -Wall -Wextra -fprofile-arcs -ftest-coverage)
    set_property(TARGET cpp-sort-testsuite APPEND_STRING PROPERTY LINK_FLAGS " -fprofile-arcs -ftest-coverage")

    set(ENABLE_COVERAGE ON CACHE BOOL "Enable coverage build." FORCE)
    list(APPEND LCOV_REMOVE_PATTERNS "'/usr/*'")
    coverage_evaluate()
endif()

if (USE_VALGRIND)
    find_program(MEMORYCHECK_COMMAND valgrind)
    set(MEMORYCHECK_COMMAND_OPTIONS "--leak-check=full --track-origins=yes --error-exitcode=1 --show-reachable=no")
endif()

include(CTest)

string(RANDOM LENGTH 5 ALPHABET 0123456789 RNG_SEED)
catch_discover_tests(cpp-sort-testsuite EXTRA_ARGS --rng-seed ${RNG_SEED})

# Optimize build times
set_property(TARGET cpp-sort-testsuite PROPERTY CXX_STANDARD 14)  # Required because of a cotire issue
cotire(cpp-sort-testsuite)
