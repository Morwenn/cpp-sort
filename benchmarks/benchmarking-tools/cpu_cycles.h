/*
 * Copyright (c) 2023 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <cstdint>
#include <utility>
#include <cpp-sort/utility/adapter_storage.h>
#include <cpp-sort/utility/metrics_tools.h>
#include <cpp-sort/detail/checkers.h>
#include "rdtsc.h"

////////////////////////////////////////////////////////////
// Tag

struct cpu_cycles_tag {};

////////////////////////////////////////////////////////////
// Metric

template<typename Sorter>
struct cpu_cycles:
    cppsort::utility::adapter_storage<Sorter>,
    cppsort::detail::check_iterator_category<Sorter>,
    cppsort::detail::check_is_always_stable<Sorter>
{
    using tag_t = cpu_cycles_tag;
    using metric_t = cppsort::utility::metric<unsigned long long, tag_t>;

    cpu_cycles() = default;

    constexpr explicit cpu_cycles(Sorter sorter):
        cppsort::utility::adapter_storage<Sorter>(std::move(sorter))
    {}

    template<typename... Args>
    auto operator()(Args&&... args) const
        -> decltype(
            this->get()(std::forward<Args>(args)...),
            metric_t(std::declval<unsigned long long>())
        )
    {
        auto start = ::rdtsc();
        this->get()(std::forward<Args>(args)...);
        auto stop = ::rdtsc();
        return metric_t(stop - start);
    }
};

////////////////////////////////////////////////////////////
// is_stable specialization

namespace cppsort
{
    template<typename Sorter, typename... Args>
    struct is_stable<cpu_cycles<Sorter>(Args...)>:
        is_stable<Sorter(Args...)>
    {};
}
