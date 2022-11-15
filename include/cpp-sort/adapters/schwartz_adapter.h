/*
 * Copyright (c) 2016-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_ADAPTERS_SCHWARTZ_ADAPTER_H_
#define CPPSORT_ADAPTERS_SCHWARTZ_ADAPTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>
#include <cpp-sort/fwd.h>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/mstd/ranges.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/adapter_storage.h>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/branchless_traits.h>
#include "../detail/associate_iterator.h"
#include "../detail/checkers.h"
#include "../detail/config.h"
#include "../detail/immovable_vector.h"
#include "../detail/iterator_traits.h"

namespace cppsort
{
    namespace detail
    {
        struct data_getter
        {
            template<typename T>
            constexpr auto operator()(T&& value) const noexcept
                -> decltype(auto)
            {
                // Braces matter here
                return (std::forward<T>(value).data);
            }
        };
    }

    namespace utility
    {
        template<typename T>
        struct is_probably_branchless_projection<cppsort::detail::data_getter, T>:
            std::true_type
        {};
    }

    namespace detail
    {
        ////////////////////////////////////////////////////////////
        // Algorithm proper

        template<
            typename ForwardIterator,
            typename Compare,
            typename Projection,
            typename Sorter
        >
        auto sort_with_schwartz(ForwardIterator first, mstd::iter_difference_t<ForwardIterator> size,
                                Compare compare, Projection projection, Sorter&& sorter)
            -> decltype(auto)
        {
            static_assert(not std::is_same_v<Sorter, std_sorter>,
                          "std_sorter doesn't work with schwartz_adapter");
            static_assert(not std::is_same_v<Sorter, stable_adapter<std_sorter>>,
                          "stable_adapter<std_sorter> doesn't work with schwartz_adapter");

            auto&& proj = utility::as_function(projection);
            using proj_t = projected_t<ForwardIterator, Projection>;
            using value_t = association<ForwardIterator, proj_t>;
            using difference_type = mstd::iter_difference_t<ForwardIterator>;

            // Associate iterator to projected element
            immovable_vector<value_t> projected(size);
            for (difference_type count = 0; count != size; ++count) {
                projected.emplace_back(first, proj(*first));
                ++first;
            }

            // Indirectly sort the original sequence
            return std::forward<Sorter>(sorter)(
                make_associate_iterator(projected.begin()),
                make_associate_iterator(projected.end()),
                std::move(compare),
                data_getter{}
            );
        }

        ////////////////////////////////////////////////////////////
        // Adapter

        template<typename Sorter>
        struct schwartz_adapter_impl:
            utility::adapter_storage<Sorter>,
            check_iterator_category<Sorter>,
            check_is_always_stable<Sorter>
        {
            schwartz_adapter_impl() = default;

            constexpr explicit schwartz_adapter_impl(Sorter&& sorter):
                utility::adapter_storage<Sorter>(std::move(sorter))
            {}

            template<
                mstd::forward_range Range,
                typename Compare = std::less<>,
                typename Projection = std::identity
            >
                requires is_projection_v<Projection, Range, Compare>
            auto operator()(Range&& range, Compare compare, Projection projection) const
                -> decltype(auto)
            {
                return sort_with_schwartz(mstd::begin(range), mstd::distance(range),
                                          std::move(compare), std::move(projection),
                                          this->get());
            }

            template<
                mstd::forward_iterator Iterator,
                mstd::sentinel_for<Iterator> Sentinel,
                typename Compare = std::less<>,
                typename Projection = std::identity
            >
                requires is_projection_iterator_v<Projection, Iterator, Compare>
            auto operator()(Iterator first, Sentinel last,
                            Compare compare, Projection projection) const
                -> decltype(auto)
            {
                return sort_with_schwartz(first, mstd::distance(first, last),
                                          std::move(compare), std::move(projection),
                                          this->get());
            }

            template<
                mstd::forward_range Range,
                typename Compare = std::less<>
            >
                requires (not is_projection_v<Compare, Range>)
            auto operator()(Range&& range, Compare compare={}) const
                -> decltype(this->get()(std::forward<Range>(range), std::move(compare)))
            {
                // No projection to handle, forward everything to the adapted sorter
                return this->get()(std::forward<Range>(range), std::move(compare));
            }

            template<
                mstd::forward_iterator Iterator,
                mstd::sentinel_for<Iterator> Sentinel,
                typename Compare = std::less<>
            >
                requires (not is_projection_iterator_v<Compare, Iterator>)
            auto operator()(Iterator first, Sentinel last, Compare compare={}) const
                -> decltype(this->get()(std::move(first), std::move(last), std::move(compare)))
            {
                // No projection to handle, forward everything to the adapted sorter
                return this->get()(std::move(first), std::move(last), std::move(compare));
            }

            template<mstd::forward_range Range, typename Compare>
            auto operator()(Range&& range, Compare compare, std::identity projection) const
                -> decltype(this->get()(std::forward<Range>(range), std::move(compare), projection))
            {
                // std::identity does nothing, bypass schartz_adapter entirely
                return this->get()(std::forward<Range>(range), std::move(compare), projection);
            }

            template<
                mstd::forward_iterator Iterator,
                mstd::sentinel_for<Iterator> Sentinel,
                typename Compare
            >
            auto operator()(Iterator first, Sentinel last, Compare compare, std::identity projection) const
                -> decltype(this->get()(std::move(first), std::move(last), std::move(compare), projection))
            {
                // std::identity does nothing, bypass schartz_adapter entirely
                return this->get()(std::move(first), std::move(last), std::move(compare), projection);
            }
        };
    }

    template<typename Sorter>
    struct schwartz_adapter:
        sorter_facade<detail::schwartz_adapter_impl<Sorter>>
    {
        schwartz_adapter() = default;

        constexpr explicit schwartz_adapter(Sorter sorter):
            sorter_facade<detail::schwartz_adapter_impl<Sorter>>(std::move(sorter))
        {}
    };

    // The following specialization exists for two reasons:
    // - There does not seem to be a reason to pay twice the memory and indirection
    //   cost when two schwarz_adapters happen to be nested, nobody should want to
    //   pay such a cost for no apparent benefit (please open an issue if you do).
    // - Most of the time the "normal" behaviour simply does not work: during the
    //   first pass, the result of projections are copied. When doubly wrapping a
    //   sorter with schwartz_adapter, the projected type is some 'association'
    //   type which is not copyable, leading to various hard-to-debug issues. The
    //   following speciaization helps to mitigate that specific issue.

    template<typename Sorter>
    struct schwartz_adapter<schwartz_adapter<Sorter>>:
        schwartz_adapter<Sorter>
    {
        schwartz_adapter() = default;

        constexpr explicit schwartz_adapter(const schwartz_adapter<Sorter>& sorter):
            schwartz_adapter<Sorter>(sorter)
        {}

        constexpr explicit schwartz_adapter(schwartz_adapter<Sorter>&& sorter):
            schwartz_adapter<Sorter>(std::move(sorter))
        {}
    };

    ////////////////////////////////////////////////////////////
    // is_stable specialization

    template<typename Sorter, typename... Args>
    struct is_stable<schwartz_adapter<Sorter>(Args...)>:
        is_stable<Sorter(Args...)>
    {};
}

#ifdef CPPSORT_ADAPTERS_SMALL_ARRAY_ADAPTER_H_
#include "../detail/schwartz_small_array.h"
#endif

#endif // CPPSORT_ADAPTERS_SCHWARTZ_ADAPTER_H_
