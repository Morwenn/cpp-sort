/*
 * Copyright (c) 2016-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_ADAPTERS_STABLE_ADAPTER_H_
#define CPPSORT_ADAPTERS_STABLE_ADAPTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <tuple>
#include <type_traits>
#include <utility>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/adapter_storage.h>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/size.h>
#include "../detail/associate_iterator.h"
#include "../detail/checkers.h"
#include "../detail/immovable_vector.h"
#include "../detail/iterator_traits.h"
#include "../detail/raw_checkers.h"
#include "../detail/sized_iterator.h"
#include "../detail/type_traits.h"

namespace cppsort
{
    namespace detail
    {
        ////////////////////////////////////////////////////////////
        // Stable comparison function

        template<typename Compare, typename Projection>
        class stable_compare
        {
            private:

                using projection_t = detail::remove_cvref_t<
                    decltype(utility::as_function(std::declval<Projection>()))
                >;
                using compare_t = detail::remove_cvref_t<
                    decltype(utility::as_function(std::declval<Compare>()))
                >;
                std::tuple<compare_t, projection_t> data;

            public:

                stable_compare(Compare compare, Projection projection):
                    data(utility::as_function(compare), utility::as_function(projection))
                {}

                template<typename T, typename U>
                auto operator()(T&& lhs, U&& rhs)
                    -> bool
                {
                    if (std::get<0>(data)(std::get<1>(data)(lhs.get()),
                                          std::get<1>(data)(rhs.get()))) {
                        return true;
                    }
                    if (std::get<0>(data)(std::get<1>(data)(rhs.get()),
                                          std::get<1>(data)(lhs.get()))) {
                        return false;
                    }
                    return lhs.data < rhs.data;
                }
        };

        template<typename Compare, typename Projection=utility::identity>
        auto make_stable_compare(Compare compare, Projection projection={})
            -> stable_compare<Compare, Projection>
        {
            return { compare, projection };
        }

        ////////////////////////////////////////////////////////////
        // make_stable_and_sort

        template<
            typename ForwardIterator,
            typename Compare,
            typename Projection,
            typename Sorter
        >
        auto make_stable_and_sort(ForwardIterator first, difference_type_t<ForwardIterator> size,
                                  Compare&& compare, Projection&& projection, Sorter&& sorter)
            -> decltype(auto)
        {
            using difference_type = difference_type_t<ForwardIterator>;
            using value_t = association<ForwardIterator, difference_type>;

            ////////////////////////////////////////////////////////////
            // Bind index to iterator

            // Associate iterators to their position
            immovable_vector<value_t> iterators(size);
            for (difference_type count = 0; count != size; ++count) {
                iterators.emplace_back(first, count);
                ++first;
            }

            ////////////////////////////////////////////////////////////
            // Sort but takes the index into account to ensure stability

            return std::forward<Sorter>(sorter)(
                make_associate_iterator(iterators.begin()),
                make_associate_iterator(iterators.end()),
                make_stable_compare(
                    std::forward<Compare>(compare),
                    std::forward<Projection>(projection)
                )
            );
        }

        template<
            typename ForwardIterator,
            typename Compare,
            typename Projection,
            typename Sorter
        >
        auto make_stable_and_sort(sized_iterator<ForwardIterator> first, difference_type_t<ForwardIterator> size,
                                  Compare&& compare, Projection&& projection, Sorter&& sorter)
            -> decltype(auto)
        {
            // Hack to get the stable bidirectional version of vergesort
            // to work correctly without duplicating tons of code
            return make_stable_and_sort(
                first.base(), size,
                std::forward<Compare>(compare),
                std::forward<Projection>(projection),
                std::forward<Sorter>(sorter)
            );
        }

        ////////////////////////////////////////////////////////////
        // make_stable_impl

        template<typename Sorter>
        struct make_stable_impl:
            utility::adapter_storage<Sorter>,
            check_iterator_category<Sorter>
        {
            make_stable_impl() = default;

            constexpr explicit make_stable_impl(Sorter&& sorter):
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
            auto operator()(ForwardIterable&& iterable,
                            Compare compare={}, Projection projection={}) const
                -> decltype(auto)
            {
                return make_stable_and_sort(std::begin(iterable), utility::size(iterable),
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
                            Compare compare={}, Projection projection={}) const
                -> decltype(auto)
            {
                using std::distance; // Hack for sized_iterator
                return make_stable_and_sort(first, distance(first, last),
                                            std::move(compare), std::move(projection),
                                            this->get());
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using is_always_stable = std::true_type;
        };
    }

    ////////////////////////////////////////////////////////////
    // make_stable

    // Expose the underlying mechanism
    template<typename Sorter>
    struct make_stable:
        sorter_facade<detail::make_stable_impl<Sorter>>
    {
        make_stable() = default;

        constexpr explicit make_stable(Sorter sorter):
            sorter_facade<detail::make_stable_impl<Sorter>>(std::move(sorter))
        {}
    };

    ////////////////////////////////////////////////////////////
    // stable_adapter

    template<typename Sorter>
    struct stable_adapter:
        utility::adapter_storage<Sorter>,
        detail::check_iterator_category<Sorter>,
        detail::sorter_facade_fptr<
            stable_adapter<Sorter>,
            std::is_empty<Sorter>::value
        >
    {
        stable_adapter() = default;

        constexpr explicit stable_adapter(const Sorter& sorter):
            utility::adapter_storage<Sorter>(sorter)
        {}

        constexpr explicit stable_adapter(Sorter&& sorter):
            utility::adapter_storage<Sorter>(std::move(sorter))
        {}

        template<
            typename... Args,
            typename = detail::enable_if_t<is_stable_v<Sorter(Args...)>>
        >
        auto operator()(Args&&... args) const
            -> decltype(this->get()(std::forward<Args>(args)...))
        {
            return this->get()(std::forward<Args>(args)...);
        }

        template<
            typename... Args,
            typename = detail::enable_if_t<not is_stable_v<Sorter(Args...)>>,
            typename = void
        >
        auto operator()(Args&&... args) const
            -> decltype(make_stable<Sorter>(this->get())(std::forward<Args>(args)...))
        {
            return make_stable<Sorter>(this->get())(std::forward<Args>(args)...);
        }

        ////////////////////////////////////////////////////////////
        // Sorter traits

        using is_always_stable = std::true_type;
        using type = detail::conditional_t<
            cppsort::is_always_stable_v<Sorter>,
            Sorter,
            stable_adapter<Sorter>
        >;
    };

    template<typename Sorter>
    struct sorter_traits<stable_adapter<Sorter>>:
        detail::raw_check_iterator_category<stable_adapter<Sorter>>
    {
        // Ensure that all user-defined specializations of stable_adapter
        // are considered always by default by sorter_traits
        using is_always_stable = std::true_type;
    };

    ////////////////////////////////////////////////////////////
    // stable_t

    namespace detail
    {
        template<typename Sorter, typename=void>
        struct stable_t_impl_type_or
        {
            using type = Sorter;
        };

        template<typename Sorter>
        struct stable_t_impl_type_or<Sorter, detail::void_t<typename Sorter::type>>
        {
            using type = typename Sorter::type;
        };

        template<typename Sorter, bool IsStableAdapter>
        struct stable_t_impl_true
        {
            // The sorter is a stable_adapter specialization, use its ::type
            // member if it exists, otherwise use the specialization directly
            using type = typename stable_t_impl_type_or<Sorter>::type;
        };

        template<typename Sorter>
        struct stable_t_impl_true<Sorter, false>
        {
            // The sorter is always stable but not a specialization of
            // stable_adapter, alias it directly
            using type = Sorter;
        };

        template<typename Sorter, bool IsStable>
        struct stable_t_impl
        {
            // The sorter is always stable, check whether it is already a
            // stable_adapter specialization
            using type = typename stable_t_impl_true<
                Sorter,
                is_specialization_of_v<Sorter, stable_adapter>
            >::type;
        };

        template<typename Sorter>
        struct stable_t_impl<Sorter, false>
        {
            // The sorter is not always stable, wrap it in stable_adapter or
            // in something close enough
            using type = typename stable_t_impl_type_or<
                stable_adapter<Sorter>
            >::type;
        };
    }

    template<typename Sorter>
    using stable_t = typename detail::stable_t_impl<
        Sorter,
        cppsort::is_always_stable_v<Sorter>
    >::type;

    ////////////////////////////////////////////////////////////
    // stable_adapter<stable_adapter<T>>

    // Accidental nesting can happen, in which case we try to
    // unwrap as many levels as possible
    template<typename Sorter>
    struct stable_adapter<stable_adapter<Sorter>>:
        stable_adapter<Sorter>
    {
        stable_adapter() = default;

        constexpr explicit stable_adapter(const stable_adapter<Sorter>& sorter):
            stable_adapter<Sorter>(sorter)
        {}

        constexpr explicit stable_adapter(stable_adapter<Sorter>&& sorter):
            stable_adapter<Sorter>(std::move(sorter))
        {}

        using type = typename detail::stable_t_impl_type_or<stable_adapter<Sorter>>::type;
    };
}

#ifdef CPPSORT_ADAPTERS_HYBRID_ADAPTER_DONE_
#   include "../detail/stable_adapter_hybrid_adapter.h"
#endif

#ifdef CPPSORT_ADAPTERS_SELF_SORT_ADAPTER_DONE_
#   include "../detail/stable_adapter_self_sort_adapter.h"
#endif

#define CPPSORT_ADAPTERS_STABLE_ADAPTER_DONE_

#endif // CPPSORT_ADAPTERS_STABLE_ADAPTER_H_
