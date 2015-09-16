# Include Catch for the tests
include_directories(${CATCH_INCLUDE_DIR})

# Define test files

set(
    ADAPTER_TESTS

    adapters/hybrid_adapter_sfinae.cpp
    adapters/self_sort_adapter_no_compare.cpp
    adapters/small_array_adapter3.cpp
    adapters/small_array_adapter4.cpp
    adapters/small_array_adapter5.cpp
)

set(
    SORTERS_TESTS

    sorters/default_sorter.cpp
)

# Make one executable for the whole testsuite
add_executable(
    cpp-sort-testsuite

    main.cpp
    ${ADAPTER_TESTS}
    ${SORTERS_TESTS}
)

add_test(testsuite cpp-sort-testsuite)

# Enable unit-testing
enable_testing(true)
