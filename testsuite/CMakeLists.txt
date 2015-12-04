# Include Catch for the tests
include_directories(${CATCH_INCLUDE_DIR})

# Define test files

set(
    ADAPTER_TESTS

    adapters/counting_adapter.cpp
    adapters/hybrid_adapter_partial_compare.cpp
    adapters/hybrid_adapter_sfinae.cpp
    adapters/self_sort_adapter_no_compare.cpp
    adapters/small_array_adapter.cpp
)

set(
    SORTERS_TESTS

    sorters/default_sorter.cpp
    sorters/default_sorter_fptr.cpp
    sorters/default_sorter_projection.cpp
    sorters/merge_sorter.cpp
    sorters/merge_sorter_projection.cpp
    sorters/spread_sorter.cpp
    sorters/spread_sorter_defaults.cpp
)

# Make one executable for the whole testsuite
add_executable(
    cpp-sort-testsuite

    main.cpp
    rebind_iterator_category.cpp
    sorter_facade.cpp
    sorter_facade_defaults.cpp
    sorter_facade_iterable.cpp
    ${ADAPTER_TESTS}
    ${SORTERS_TESTS}
)

add_test(testsuite cpp-sort-testsuite)

# Enable unit-testing
enable_testing(true)
