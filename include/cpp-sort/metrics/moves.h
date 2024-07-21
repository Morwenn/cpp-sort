/*
 * Copyright (c) 2023-2024 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_METRICS_MOVES_H_
#define CPPSORT_METRICS_MOVES_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <utility>
#include <cpp-sort/fwd.h>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/mstd/ranges.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/adapter_storage.h>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/metrics_tools.h>
#include "../detail/checkers.h"
#include "../detail/fake_category_iterator.h"
#include "../detail/immovable_vector.h"
#include "../detail/iterator_traits.h"

namespace cppsort
{
namespace metrics
{
    namespace detail
    {
        template<typename T, typename CountType>
        struct move_counting_wrapper
        {
            // Wrapped value, readily accessible
            T value;
            // Moves counter
            CountType* count;

            move_counting_wrapper(T&& value, CountType& counter):
                value(std::move(value)),
                count(&counter)
            {
                // Do not increment the counter when constructing from value
            }

            move_counting_wrapper(move_counting_wrapper&& other):
                value(std::move(other.value)),
                count(other.count)
            {
                ++(*count);
            }

            auto operator=(move_counting_wrapper&& other)
                -> move_counting_wrapper&
            {
                value = std::move(other.value);
                count = other.count;
                ++(*count);
                return *this;
            }

            // Not copyable
            move_counting_wrapper(const move_counting_wrapper&) = delete;
            move_counting_wrapper& operator=(const move_counting_wrapper&) = delete;
        };

        template<typename CountType, typename ForwardIterator, typename Size,
                 typename Compare, typename Projection, typename Sorter>
        auto count_moves(ForwardIterator first, ForwardIterator last, Size size,
                         Compare compare, Projection projection, Sorter&& sorter)
            -> CountType
        {
            using wrapper_t = move_counting_wrapper<
                cppsort::detail::rvalue_type_t<ForwardIterator>,
                CountType
            >;
            // Use a special iterator that wraps wrapper_t* and pretends
            // it is of a different iterator category to better force the
            // sorter to use strategies matching the category of the
            // passed iterator type
            using iterator_t = cppsort::detail::fake_category_iterator<
                wrapper_t*,
                cppsort::detail::iterator_category_t<ForwardIterator>
            >;

            // Use an immovable_vector to ensure that no move is performed
            // hile filling the vector
            CountType count(0);
            cppsort::detail::immovable_vector<wrapper_t> vec(size);
            for (auto it = first; it != last; ++it) {
                vec.emplace_back(mstd::iter_move(it), count);
            }

            std::forward<Sorter>(sorter)(
                iterator_t(vec.begin()), iterator_t(vec.end()), std::move(compare),
                utility::as_projection(&wrapper_t::value) | std::move(projection)
            );

            // Move back the wrapped value only, not the wrapper itself,
            // this ensures that those moves are not counted
            auto vec_it = vec.begin();
            for (auto it = first; it != last; ++it, ++vec_it) {
                *it = std::move(vec_it->value);
            }

            return count;
        }
    }

    ////////////////////////////////////////////////////////////
    // Tag

    struct moves_tag {};

    ////////////////////////////////////////////////////////////
    // Metric

    namespace detail
    {
        template<typename Sorter, typename CountType>
        struct moves_impl:
            utility::adapter_storage<Sorter>,
            cppsort::detail::check_iterator_category<Sorter>,
            cppsort::detail::check_is_always_stable<Sorter>
        {
            using tag_t = moves_tag;
            using metric_t = utility::metric<CountType, tag_t>;

            moves_impl() = default;

            constexpr explicit moves_impl(Sorter&& sorter):
                utility::adapter_storage<Sorter>(std::move(sorter))
            {}

            template<
                typename ForwardIterator,
                typename Compare = std::less<>,
                typename Projection = std::identity
            >
                requires is_projection_iterator_v<Projection, ForwardIterator, Compare>
            auto operator()(ForwardIterator first, ForwardIterator last,
                            Compare compare={}, Projection projection={}) const
                -> metric_t
            {
                auto count = detail::count_moves<CountType>(
                    first, last,
                    mstd::distance(first, last),
                    std::move(compare), std::move(projection),
                    this->get()
                );
                return metric_t(count);
            }

            template<
                typename ForwardIterable,
                typename Compare = std::less<>,
                typename Projection = std::identity
            >
                requires is_projection_v<Projection, ForwardIterable, Compare>
            auto operator()(ForwardIterable&& iterable,
                            Compare compare={}, Projection projection={}) const
            {
                auto count = detail::count_moves<CountType>(
                    mstd::begin(iterable), mstd::end(iterable),
                    mstd::distance(iterable),
                    std::move(compare), std::move(projection),
                    this->get()
                );
                return metric_t(count);
            }
        };
    }

    template<typename Sorter, typename CountType>
    struct moves:
        sorter_facade<detail::moves_impl<Sorter, CountType>>
    {
        moves() = default;

        constexpr explicit moves(Sorter sorter):
            sorter_facade<detail::moves_impl<Sorter, CountType>>(std::move(sorter))
        {}
    };
}}

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // is_stable specialization

    template<typename Sorter, typename CountType, typename... Args>
    struct is_stable<metrics::moves<Sorter, CountType>(Args...)>:
        is_stable<Sorter(Args...)>
    {};
}

#endif // CPPSORT_METRICS_MOVES_H_
