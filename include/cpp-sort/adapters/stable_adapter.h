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
#include <memory>
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
#include "../detail/iterator_traits.h"
#include "../detail/memory.h"
#include "../detail/sized_iterator.h"
#include "../detail/type_traits.h"

namespace cppsort
{
    namespace detail
    {
        ////////////////////////////////////////////////////////////
        // Stable comparison function

        template<
            typename Compare,
            typename Projection = utility::identity
        >
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

                stable_compare(Compare compare, Projection projection={}):
                    data(utility::as_function(compare), utility::as_function(projection))
                {}

                auto compare() const
                    -> compare_t
                {
                    return std::get<0>(data);
                }

                auto projection() const
                    -> projection_t
                {
                    return std::get<1>(data);
                }

                template<typename T, typename U>
                auto operator()(T&& lhs, U&& rhs)
                    -> bool
                {
                    if (std::get<0>(data)(std::get<1>(data)(std::forward<T>(lhs).get()),
                                          std::get<1>(data)(std::forward<U>(rhs).get())))
                    {
                        return true;
                    }
                    if (std::get<0>(data)(std::get<1>(data)(std::forward<U>(rhs).get()),
                                          std::get<1>(data)(std::forward<T>(lhs).get())))
                    {
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

            std::unique_ptr<value_t, operator_deleter> iterators(
                static_cast<value_t*>(::operator new(size * sizeof(value_t))),
                operator_deleter(size * sizeof(value_t))
            );
            destruct_n<value_t> d(0);
            std::unique_ptr<value_t, destruct_n<value_t>&> h2(iterators.get(), d);

            // Associate iterators to their position
            auto ptr = iterators.get();
            for (difference_type count = 0 ; count != size ; ++count) {
                ::new(ptr) value_t(first, count);
                ++d;
                ++first;
                ++ptr;
            }

            ////////////////////////////////////////////////////////////
            // Sort but takes the index into account to ensure stability

            return std::forward<Sorter>(sorter)(
                make_associate_iterator(iterators.get()),
                make_associate_iterator(iterators.get() + size),
                make_stable_compare(std::move(compare), std::move(projection))
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
            return make_stable_and_sort(first.base(), size,
                                        std::move(compare), std::move(projection),
                                        std::move(sorter));
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
                typename = std::enable_if_t<
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
                typename = std::enable_if_t<
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

        constexpr explicit stable_adapter(Sorter sorter):
            utility::adapter_storage<Sorter>(std::move(sorter))
        {}

        template<
            typename... Args,
            typename = std::enable_if_t<is_stable_v<Sorter(Args...)>>
        >
        auto operator()(Args&&... args) const
            -> decltype(this->get()(std::forward<Args>(args)...))
        {
            return this->get()(std::forward<Args>(args)...);
        }

        template<
            typename... Args,
            typename = std::enable_if_t<not is_stable_v<Sorter(Args...)>>,
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
        using type = stable_adapter<Sorter>;
    };

    // Accidental nesting can happen, unwrap
    template<typename Sorter>
    struct stable_adapter<stable_adapter<Sorter>>:
        stable_adapter<Sorter>
    {
        using type = stable_adapter<Sorter>;
    };

    ////////////////////////////////////////////////////////////
    // stable_t

    namespace detail
    {
        template<typename Sorter, typename=void>
        struct stable_t_impl_false
        {
            // The sorter is not always stable and does not have
            // a type member named 'type'
            using type = stable_adapter<Sorter>;
        };

        template<typename Sorter>
        struct stable_t_impl_false<Sorter, detail::void_t<typename stable_adapter<Sorter>::type>>
        {
            // The sorter is not always stable but has a type member
            // called 'type', use that one
            using type = typename stable_adapter<Sorter>::type;
        };

        template<typename Sorter, bool>
        struct stable_t_impl
        {
            // The sorter is always stable, alias it directly
            using type = Sorter;
        };

        template<typename Sorter>
        struct stable_t_impl<Sorter, false>
        {
            using type = typename stable_t_impl_false<Sorter>::type;
        };
    }

    template<typename Sorter>
    using stable_t = typename detail::stable_t_impl<
        Sorter,
        cppsort::is_always_stable_v<Sorter>
    >::type;
}

#ifdef CPPSORT_ADAPTERS_HYBRID_ADAPTER_DONE_
#   include "../detail/stable_adapter_hybrid_adapter.h"
#endif

#ifdef CPPSORT_ADAPTERS_SELF_SORT_ADAPTER_DONE_
#   include "../detail/stable_adapter_self_sort_adapter.h"
#endif

#define CPPSORT_ADAPTERS_STABLE_ADAPTER_DONE_

#endif // CPPSORT_ADAPTERS_STABLE_ADAPTER_H_
