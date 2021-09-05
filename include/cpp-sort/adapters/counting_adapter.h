/*
 * Copyright (c) 2015-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_ADAPTERS_COUNTING_ADAPTER_H_
#define CPPSORT_ADAPTERS_COUNTING_ADAPTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <type_traits>
#include <utility>
#include <cpp-sort/fwd.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/adapter_storage.h>
#include "../detail/checkers.h"
#include "../detail/comparison_counter.h"
#include "../detail/type_traits.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Adapter

    namespace detail
    {
        template<typename Sorter, typename CountType>
        struct counting_adapter_impl:
            utility::adapter_storage<Sorter>,
            check_iterator_category<Sorter>,
            check_is_always_stable<Sorter>
        {
            counting_adapter_impl() = default;

            constexpr explicit counting_adapter_impl(Sorter&& sorter):
                utility::adapter_storage<Sorter>(std::move(sorter))
            {}

            template<
                typename Iterable,
                typename Compare = std::less<>,
                typename = detail::enable_if_t<
                    not is_projection_v<Compare, Iterable>
                >
            >
            auto operator()(Iterable&& iterable, Compare compare={}) const
                -> CountType
            {
                CountType count(0);
                comparison_counter<Compare, CountType> cmp(std::move(compare), count);
                this->get()(std::forward<Iterable>(iterable), std::move(cmp));
                return count;
            }

            template<
                typename Iterator,
                typename Compare = std::less<>,
                typename = detail::enable_if_t<
                    not is_projection_iterator_v<Compare, Iterator>
                >
            >
            auto operator()(Iterator first, Iterator last, Compare compare={}) const
                -> CountType
            {
                CountType count(0);
                comparison_counter<Compare, CountType> cmp(std::move(compare), count);
                this->get()(std::move(first), std::move(last), std::move(cmp));
                return count;
            }

            template<
                typename Iterable,
                typename Compare,
                typename Projection,
                typename = detail::enable_if_t<
                    is_projection_v<Projection, Iterable, Compare>
                >
            >
            auto operator()(Iterable&& iterable, Compare compare, Projection projection) const
                -> CountType
            {
                CountType count(0);
                comparison_counter<Compare, CountType> cmp(std::move(compare), count);
                this->get()(std::forward<Iterable>(iterable), std::move(cmp), std::move(projection));
                return count;
            }

            template<
                typename Iterator,
                typename Compare,
                typename Projection,
                typename = detail::enable_if_t<
                    is_projection_iterator_v<Projection, Iterator, Compare>
                >
            >
            auto operator()(Iterator first, Iterator last,
                            Compare compare, Projection projection) const
                -> CountType
            {
                CountType count(0);
                comparison_counter<Compare, CountType> cmp(std::move(compare), count);
                this->get()(std::move(first), std::move(last), std::move(cmp), std::move(projection));
                return count;
            }
        };
    }

    template<typename Sorter, typename CountType>
    struct counting_adapter:
        sorter_facade<detail::counting_adapter_impl<
            Sorter,
            CountType
        >>
    {
        counting_adapter() = default;

        constexpr explicit counting_adapter(Sorter sorter):
            sorter_facade<detail::counting_adapter_impl<Sorter, CountType>>(std::move(sorter))
        {}
    };

    ////////////////////////////////////////////////////////////
    // is_stable specialization

    template<typename Sorter, typename CountType, typename... Args>
    struct is_stable<counting_adapter<Sorter, CountType>(Args...)>:
        is_stable<Sorter(Args...)>
    {};
}

#endif // CPPSORT_ADAPTERS_COUNTING_ADAPTER_H_
