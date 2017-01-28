# Include Catch for the tests
include_directories(${CATCH_INCLUDE_DIR})

# Define test files

set(
    ADAPTERS_TESTS

    adapters/container_aware_adapter.cpp
    adapters/container_aware_adapter_list.cpp
    adapters/container_aware_adapter_forward_list.cpp
    adapters/counting_adapter.cpp
    adapters/hybrid_adapter_is_stable.cpp
    adapters/hybrid_adapter_partial_compare.cpp
    adapters/hybrid_adapter_sfinae.cpp
    adapters/indirect_adapter.cpp
    adapters/indirect_adapter_every_sorter.cpp
    adapters/mixed_adapters.cpp
    adapters/schwartz_adapter_every_sorter.cpp
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
    utility/buffer.cpp
    utility/iter_swap.cpp
)

# Make one executable for the whole testsuite
add_executable(
    cpp-sort-testsuite

    main.cpp
    every_instantiated_sorter.cpp
    every_sorter.cpp
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

add_test(testsuite cpp-sort-testsuite)

# Enable unit-testing
enable_testing(true)
