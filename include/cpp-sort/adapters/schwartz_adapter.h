/*
 * Copyright (c) 2016-2021 Morwenn
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
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/adapter_storage.h>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/branchless_traits.h>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/size.h>
#include "../detail/associate_iterator.h"
#include "../detail/checkers.h"
#include "../detail/config.h"
#include "../detail/immovable_vector.h"
#include "../detail/iterator_traits.h"
#include "../detail/type_traits.h"

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
        auto sort_with_schwartz(ForwardIterator first, difference_type_t<ForwardIterator> size,
                                Compare compare, Projection projection, Sorter&& sorter)
            -> decltype(auto)
        {
            static_assert(not std::is_same<Sorter, std_sorter>::value,
                          "std_sorter doesn't work with schwartz_adapter");
            static_assert(not std::is_same<Sorter, stable_adapter<std_sorter>>::value,
                          "stable_adapter<std_sorter> doesn't work with schwartz_adapter");

            auto&& proj = utility::as_function(projection);
            using proj_t = projected_t<ForwardIterator, Projection>;
            using value_t = association<ForwardIterator, proj_t>;
            using difference_type = difference_type_t<ForwardIterator>;

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
                typename ForwardIterable,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
                typename = detail::enable_if_t<
                    is_projection_v<Projection, ForwardIterable, Compare>
                >
            >
            auto operator()(ForwardIterable&& iterable, Compare compare, Projection projection) const
                -> decltype(auto)
            {
                return sort_with_schwartz(std::begin(iterable), utility::size(iterable),
                                          std::move(compare), std::move(projection),
                                          this->get());
            }

            template<
                typename ForwardIterator,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
                typename = detail::enable_if_t<
                    is_projection_iterator_v<Projection, ForwardIterator, Compare>
                >
            >
            auto operator()(ForwardIterator first, ForwardIterator last,
                            Compare compare, Projection projection) const
                -> decltype(auto)
            {
                return sort_with_schwartz(first, std::distance(first, last),
                                          std::move(compare), std::move(projection),
                                          this->get());
            }

            template<typename ForwardIterable, typename Compare=std::less<>>
            auto operator()(ForwardIterable&& iterable, Compare compare={}) const
                -> detail::enable_if_t<
                    not is_projection_v<Compare, ForwardIterable>,
                    decltype(this->get()(std::forward<ForwardIterable>(iterable), std::move(compare)))
                >
            {
                // No projection to handle, forward everything to the adapted sorter
                return this->get()(std::forward<ForwardIterable>(iterable), std::move(compare));
            }

            template<typename ForwardIterator, typename Compare=std::less<>>
            auto operator()(ForwardIterator first, ForwardIterator last,
                            Compare compare={}) const
                -> detail::enable_if_t<
                    not is_projection_iterator_v<Compare, ForwardIterator>,
                    decltype(this->get()(std::move(first), std::move(last), std::move(compare)))
                >
            {
                // No projection to handle, forward everything to the adapted sorter
                return this->get()(std::move(first), std::move(last), std::move(compare));
            }

            template<typename ForwardIterable, typename Compare>
            auto operator()(ForwardIterable&& iterable, Compare compare, utility::identity projection) const
                -> decltype(this->get()(std::forward<ForwardIterable>(iterable), std::move(compare), projection))
            {
                // utility::identity does nothing, bypass schartz_adapter entirely
                return this->get()(std::forward<ForwardIterable>(iterable), std::move(compare), projection);
            }

            template<typename ForwardIterator, typename Compare>
            auto operator()(ForwardIterator first, ForwardIterator last,
                            Compare compare, utility::identity projection) const
                -> decltype(this->get()(std::move(first), std::move(last), std::move(compare), projection))
            {
                // utility::identity does nothing, bypass schartz_adapter entirely
                return this->get()(std::move(first), std::move(last), std::move(compare), projection);
            }

#if CPPSORT_STD_IDENTITY_AVAILABLE
            template<typename ForwardIterable, typename Compare>
            auto operator()(ForwardIterable&& iterable, Compare compare, std::identity projection) const
                -> decltype(this->get()(std::forward<ForwardIterable>(iterable), std::move(compare), projection))
            {
                // std::identity does nothing, bypass schartz_adapter entirely
                return this->get()(std::forward<ForwardIterable>(iterable), std::move(compare), projection);
            }

            template<typename ForwardIterator, typename Compare>
            auto operator()(ForwardIterator first, ForwardIterator last,
                            Compare compare, std::identity projection) const
                -> decltype(this->get()(std::move(first), std::move(last), std::move(compare), projection))
            {
                // std::identity does nothing, bypass schartz_adapter entirely
                return this->get()(std::move(first), std::move(last), std::move(compare), projection);
            }
#endif
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
