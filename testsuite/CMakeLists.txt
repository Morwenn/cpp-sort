# Include Catch for the tests
include_directories(${CATCH_INCLUDE_DIR})

# Define test files
set(
    SORT_TESTS

    sort3.cpp
)

# Make one executable for the whole testsuite
add_executable(
    cpp-sort-testsuite

    main.cpp
    ${SORT_TESTS}
)

add_test(testsuite cpp-sort-testsuite)

# Enable unit-testing
enable_testing(true)
