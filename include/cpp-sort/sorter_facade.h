/*
 * Copyright (c) 2015-2024 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_SORTER_FACADE_H_
#define CPPSORT_SORTER_FACADE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <type_traits>
#include <utility>
#include <cpp-sort/comparators/projection_compare.h>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/mstd/ranges.h>
#include <cpp-sort/mstd/type_traits.h>
#include <cpp-sort/refined.h>
#include <cpp-sort/sorter_traits.h>
#include "detail/type_traits.h"

namespace cppsort
{
    namespace detail
    {
        template<typename Sorter>
        struct invoker
        {
            // These functions are used to create function pointers
            // from stateless sorters: the first overload allows to
            // create function pointers with a result similar to that
            // of the sorter, the second one allows to cast away the
            // result to void

            template<typename Ret, typename... Args>
                requires (not std::is_void_v<Ret>)
            static constexpr auto invoke(Args... args)
                -> Ret
            {
                return Sorter{}(std::forward<Args>(args)...);
            }

            template<typename Ret, typename... Args>
                requires std::is_void_v<Ret>
            static constexpr auto invoke(Args... args)
                -> void
            {
                Sorter{}(std::forward<Args>(args)...);
            }
        };

        // Helper class to allow to convert a sorter_facade into a variety
        // of function pointers, but only if the wrapped sorter is empty:
        // conversion to function pointer does not make sense when state
        // is involved

        template<typename Sorter, bool IsEmpty>
        class sorter_facade_fptr;

        template<typename Sorter>
        class sorter_facade_fptr<Sorter, true>
        {
            protected:

                // A type alias is required for the function pointer
                // conversion operator syntax to be valid

                template<typename Ret, typename... Args>
                using fptr_t = Ret(*)(Args...);

            public:

                ////////////////////////////////////////////////////////////
                // Conversion to function pointers

                template<typename Ret, mstd::forward_range Range>
                constexpr operator fptr_t<Ret, Range&>()
                {
                    return invoker<Sorter>::template invoke<Ret, Range&>;
                }

                template<typename Ret, mstd::forward_range Range>
                constexpr operator fptr_t<Ret, Range&&>()
                {
                    return invoker<Sorter>::template invoke<Ret, Range&&>;
                }

                template<typename Ret, mstd::forward_range Range, typename Func>
                constexpr operator fptr_t<Ret, Range&, Func>()
                {
                    return invoker<Sorter>::template invoke<Ret, Range&, Func>;
                }

                template<typename Ret, mstd::forward_range Range, typename Func>
                constexpr operator fptr_t<Ret, Range&&, Func>()
                {
                    return invoker<Sorter>::template invoke<Ret, Range&&, Func>;
                }

                template<typename Ret, mstd::forward_range Range, typename Func1, typename Func2>
                constexpr operator fptr_t<Ret, Range&, Func1, Func2>()
                {
                    return invoker<Sorter>::template invoke<Ret, Range&, Func1, Func2>;
                }

                template<typename Ret, mstd::forward_range Range, typename Func1, typename Func2>
                constexpr operator fptr_t<Ret, Range&&, Func1, Func2>()
                {
                    return invoker<Sorter>::template invoke<Ret, Range&&, Func1, Func2>;
                }

                template<
                    typename Ret,
                    mstd::forward_iterator Iterator,
                    mstd::sentinel_for<Iterator> Sentinel
                >
                constexpr operator fptr_t<Ret, Iterator, Sentinel>()
                {
                    return invoker<Sorter>::template invoke<Ret, Iterator, Sentinel>;
                }

                template<
                    typename Ret,
                    mstd::forward_iterator Iterator,
                    mstd::sentinel_for<Iterator> Sentinel,
                    typename Func
                >
                constexpr operator fptr_t<Ret, Iterator, Sentinel, Func>()
                {
                    return invoker<Sorter>::template invoke<Ret, Iterator, Sentinel, Func>;
                }

                template<
                    typename Ret,
                    mstd::forward_iterator Iterator,
                    mstd::sentinel_for<Iterator> Sentinel,
                    typename Func1, typename Func2
                >
                constexpr operator fptr_t<Ret, Iterator, Sentinel, Func1, Func2>()
                {
                    return invoker<Sorter>::template invoke<Ret, Iterator, Sentinel, Func1, Func2>;
                }
        };

        template<typename Sorter>
        class sorter_facade_fptr<Sorter, false> {};
    }

    // This class takes an incomplete sorter, analyses it and creates
    // all the methods needed to complete it: additional overloads to
    // operator() and conversions to function pointers

    template<typename Sorter>
    struct sorter_facade:
        Sorter,
        detail::sorter_facade_fptr<
            sorter_facade<Sorter>,
            std::is_empty_v<Sorter>
        >
    {
        ////////////////////////////////////////////////////////////
        // Constructors

        sorter_facade() = default;

        template<typename... Args>
        constexpr sorter_facade(Args&&... args):
            Sorter(std::forward<Args>(args)...)
        {}

        ////////////////////////////////////////////////////////////
        // Non-comparison overloads

        template<
            typename Self,
            mstd::forward_iterator Iterator,
            mstd::sentinel_for<Iterator> Sentinel
        >
        constexpr auto operator()(this Self&& self, Iterator first, Sentinel last)
            -> decltype(std::forward<Self>(self).Sorter::operator()(
                std::move(first), std::move(last)))
        {
            return std::forward<Self>(self).Sorter::operator()(
                std::move(first), std::move(last));
        }

        template<typename Self, mstd::forward_range Range>
            requires detail::has_sort<
                detail::copy_cvref_t<Self, Sorter>,
                Range
            >::value
        constexpr auto operator()(this Self&& self, Range&& range)
            -> decltype(std::forward<Self>(self).Sorter::operator()(std::forward<Range>(range)))
        {
            return std::forward<Self>(self).Sorter::operator()(std::forward<Range>(range));
        }

        template<typename Self, mstd::forward_range Range>
            requires (not detail::has_sort<
                detail::copy_cvref_t<Self, Sorter>,
                Range
            >::value)
        constexpr auto operator()(this Self&& self, Range&& range)
            -> decltype(std::forward<Self>(self).Sorter::operator()(
                mstd::begin(range), mstd::end(range)))
        {
            return std::forward<Self>(self).Sorter::operator()(
                mstd::begin(range), mstd::end(range));
        }

        ////////////////////////////////////////////////////////////
        // Comparison overloads

        template<
            typename Self,
            mstd::forward_iterator Iterator,
            mstd::sentinel_for<Iterator> Sentinel,
            typename Compare
        >
            requires detail::has_comparison_sort_iterator<
                detail::copy_cvref_t<Self, Sorter>,
                Iterator,
                refined_t<std::iter_reference_t<Iterator>, Compare>
            >::value
        constexpr auto operator()(this Self&& self, Iterator first, Sentinel last, Compare compare)
            -> decltype(std::forward<Self>(self).Sorter::operator()(
                std::move(first), std::move(last),
                refined<std::iter_reference_t<Iterator>>(std::move(compare))))
        {
            return std::forward<Self>(self).Sorter::operator()(
                std::move(first), std::move(last),
                refined<std::iter_reference_t<Iterator>>(std::move(compare)));
        }

        template<typename Self, mstd::forward_range Range, typename Compare>
            requires detail::has_comparison_sort<
                detail::copy_cvref_t<Self, Sorter>,
                Range,
                refined_t<mstd::range_reference_t<Range>, Compare>
            >::value
        constexpr auto operator()(this Self&& self, Range&& range, Compare compare)
            -> decltype(std::forward<Self>(self).Sorter::operator()(
                std::forward<Range>(range),
                refined<mstd::range_reference_t<Range>>(std::move(compare))))
        {
            return std::forward<Self>(self).Sorter::operator()(
                std::forward<Range>(range),
                refined<mstd::range_reference_t<Range>>(std::move(compare)));
        }

        template<typename Self, mstd::forward_range Range, typename Compare>
            requires (
                not detail::has_comparison_sort<
                    detail::copy_cvref_t<Self, Sorter>,
                    Range,
                    refined_t<mstd::range_reference_t<Range>, Compare>
                >::value &&
                detail::has_comparison_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                    mstd::iterator_t<Range>,
                    refined_t<mstd::range_reference_t<Range>, Compare>
                >::value
            )
        constexpr auto operator()(this Self&& self, Range&& range, Compare compare)
            -> decltype(std::forward<Self>(self).Sorter::operator()(
                mstd::begin(range), mstd::end(range),
                refined<mstd::range_reference_t<Range>>(std::move(compare))))
        {
            return std::forward<Self>(self).Sorter::operator()(
                mstd::begin(range), mstd::end(range),
                refined<mstd::range_reference_t<Range>>(std::move(compare)));
        }

        ////////////////////////////////////////////////////////////
        // Projection overloads

        template<
            typename Self,
            mstd::forward_iterator Iterator,
            mstd::sentinel_for<Iterator> Sentinel,
            typename Projection
        >
            requires (
                not detail::has_comparison_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                    Iterator,
                    refined_t<std::iter_reference_t<Iterator>, Projection>
                >::value &&
                detail::has_projection_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                    Iterator,
                    refined_t<std::iter_reference_t<Iterator>, Projection>
                >::value
            )
        constexpr auto operator()(this Self&& self, Iterator first, Sentinel last,
                                  Projection projection)
            -> decltype(std::forward<Self>(self).Sorter::operator()(
                std::move(first), std::move(last),
                refined<std::iter_reference_t<Iterator>>(std::move(projection))))
        {
            return std::forward<Self>(self).Sorter::operator()(
                std::move(first), std::move(last),
                refined<std::iter_reference_t<Iterator>>(std::move(projection)));
        }

        template<
            typename Self,
            mstd::forward_iterator Iterator,
            mstd::sentinel_for<Iterator> Sentinel,
            typename Projection
        >
            requires (
                not detail::has_projection_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                    Iterator,
                    refined_t<std::iter_reference_t<Iterator>, Projection>
                >::value &&
                detail::has_comparison_projection_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                    Iterator,
                    std::less<>,
                    refined_t<std::iter_reference_t<Iterator>, Projection>
                >::value
            )
        constexpr auto operator()(this Self&& self, Iterator first, Sentinel last,
                                  Projection projection)
            -> decltype(std::forward<Self>(self).Sorter::operator()(
                std::move(first), std::move(last), std::less{},
                refined<std::iter_reference_t<Iterator>>(std::move(projection))))
        {
            return std::forward<Self>(self).Sorter::operator()(
                std::move(first), std::move(last), std::less{},
                refined<std::iter_reference_t<Iterator>>(std::move(projection)));
        }

        template<typename Self, mstd::forward_range Range, typename Projection>
            requires (
                not detail::has_comparison_sort<
                    detail::copy_cvref_t<Self, Sorter>,
                    Range,
                    refined_t<mstd::range_reference_t<Range>, Projection>
                >::value &&
                detail::has_projection_sort<
                    detail::copy_cvref_t<Self, Sorter>,
                    Range,
                    refined_t<mstd::range_reference_t<Range>, Projection>
                >::value
            )
        constexpr auto operator()(this Self&& self, Range&& range, Projection projection)
            -> decltype(std::forward<Self>(self).Sorter::operator()(
                std::forward<Range>(range),
                refined<mstd::range_reference_t<Range>>(std::move(projection))))
        {
            return std::forward<Self>(self).Sorter::operator()(
                std::forward<Range>(range),
                refined<mstd::range_reference_t<Range>>(std::move(projection)));
        }

        template<typename Self, mstd::forward_range Range, typename Projection>
            requires (
                not detail::has_comparison_sort<
                    detail::copy_cvref_t<Self, Sorter>,
                    Range,
                    refined_t<mstd::range_reference_t<Range>, Projection>
                >::value &&
                not detail::has_projection_sort<
                    detail::copy_cvref_t<Self, Sorter>,
                    Range,
                    refined_t<mstd::range_reference_t<Range>, Projection>
                >::value &&
                detail::has_comparison_projection_sort<
                    detail::copy_cvref_t<Self, Sorter>,
                    Range,
                    std::less<>,
                    refined_t<mstd::range_reference_t<Range>, Projection>
                >::value
            )
        constexpr auto operator()(this Self&& self, Range&& range, Projection projection)
            -> decltype(std::forward<Self>(self).Sorter::operator()(
                std::forward<Range>(range), std::less{},
                refined<mstd::range_reference_t<Range>>(std::move(projection))))
        {
            return std::forward<Self>(self).Sorter::operator()(
                std::forward<Range>(range), std::less{},
                refined<mstd::range_reference_t<Range>>(std::move(projection)));
        }

        template<typename Self, mstd::forward_range Range, typename Projection>
            requires (
                not detail::has_comparison_sort<
                    detail::copy_cvref_t<Self, Sorter>,
                    Range,
                    refined_t<mstd::range_reference_t<Range>, Projection>
                >::value &&
                not detail::has_comparison_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                    mstd::iterator_t<Range>,
                    refined_t<mstd::range_reference_t<Range>, Projection>
                >::value &&
                not detail::has_projection_sort<
                    detail::copy_cvref_t<Self, Sorter>,
                    Range,
                    refined_t<mstd::range_reference_t<Range>, Projection>
                >::value &&
                not detail::has_comparison_projection_sort<
                    detail::copy_cvref_t<Self, Sorter>,
                    Range,
                    std::less<>,
                    refined_t<mstd::range_reference_t<Range>, Projection>
                >::value &&
                detail::has_projection_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                    mstd::iterator_t<Range>,
                    refined_t<mstd::range_reference_t<Range>, Projection>
                >::value
            )
        constexpr auto operator()(this Self&& self, Range&& range, Projection projection)
            -> decltype(std::forward<Self>(self).Sorter::operator()(
                mstd::begin(range), mstd::end(range),
                refined<mstd::range_reference_t<Range>>(std::move(projection))))
        {
            return std::forward<Self>(self).Sorter::operator()(
                mstd::begin(range), mstd::end(range),
                refined<mstd::range_reference_t<Range>>(std::move(projection)));
        }

        template<typename Self, mstd::forward_range Range, typename Projection>
            requires (
                not detail::has_projection_sort<
                    detail::copy_cvref_t<Self, Sorter>,
                    Range,
                    refined_t<mstd::range_reference_t<Range>, Projection>
                >::value &&
                not detail::has_comparison_projection_sort<
                    detail::copy_cvref_t<Self, Sorter>,
                    Range,
                    std::less<>,
                    refined_t<mstd::range_reference_t<Range>, Projection>
                >::value &&
                not detail::has_projection_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                    mstd::iterator_t<Range>,
                    refined_t<mstd::range_reference_t<Range>, Projection>
                >::value &&
                detail::has_comparison_projection_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                    mstd::iterator_t<Range>,
                    std::less<>,
                    refined_t<mstd::range_reference_t<Range>, Projection>
                >::value
            )
        constexpr auto operator()(this Self&& self, Range&& range, Projection projection)
            -> decltype(std::forward<Self>(self).Sorter::operator()(
                mstd::begin(range), mstd::end(range), std::less{},
                refined<mstd::range_reference_t<Range>>(std::move(projection))))
        {
            return std::forward<Self>(self).Sorter::operator()(
                mstd::begin(range), mstd::end(range), std::less{},
                refined<mstd::range_reference_t<Range>>(std::move(projection)));
        }

        ////////////////////////////////////////////////////////////
        // std::less<> overloads

        template<
            typename Self,
            mstd::forward_iterator Iterator,
            mstd::sentinel_for<Iterator> Sentinel
        >
            requires (
                not detail::has_comparison_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                    Iterator,
                    std::less<>
                >::value
            )
        constexpr auto operator()(this Self&& self, Iterator first, Sentinel last, std::less<>)
            -> decltype(std::forward<Self>(self).Sorter::operator()(
                std::move(first), std::move(last)))
        {
            return std::forward<Self>(self).Sorter::operator()(
                std::move(first), std::move(last));
        }

        template<typename Self, mstd::forward_range Range>
            requires (
                not detail::has_comparison_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                    mstd::iterator_t<Range>,
                    std::less<>
                >::value
            )
        constexpr auto operator()(this Self&& self, Range&& range, std::less<>)
            -> decltype(std::forward<Self>(self).operator()(std::forward<Range>(range)))
        {
            return std::forward<Self>(self).operator()(std::forward<Range>(range));
        }

        template<
            typename Self,
            mstd::forward_iterator Iterator,
            mstd::sentinel_for<Iterator> Sentinel
        >
            requires (
                not detail::has_comparison_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                    Iterator,
                    std::ranges::less
                >::value
            )
        constexpr auto operator()(this Self&& self, Iterator first, Sentinel last, std::ranges::less)
            -> decltype(std::forward<Self>(self).Sorter::operator()(
                std::move(first), std::move(last)))
        {
            return std::forward<Self>(self).Sorter::operator()(
                std::move(first), std::move(last));
        }

        template<typename Self, mstd::forward_range Range>
            requires (
                not detail::has_comparison_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                    mstd::iterator_t<Range>,
                    std::ranges::less
                >::value
            )
        constexpr auto operator()(this Self&& self, Range&& range, std::ranges::less)
            -> decltype(std::forward<Self>(self)(std::forward<Range>(range)))
        {
            return std::forward<Self>(self)(std::forward<Range>(range));
        }

        ////////////////////////////////////////////////////////////
        // std::identity overloads

        template<
            typename Self,
            mstd::forward_iterator Iterator,
            mstd::sentinel_for<Iterator> Sentinel
        >
            requires (
                not detail::has_projection_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                    Iterator,
                std::identity>::value &&
                not detail::has_comparison_projection_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                    Iterator,
                    std::less<>,
                    std::identity
                >::value
            )
        constexpr auto operator()(this Self&& self, Iterator first, Sentinel last, std::identity)
            -> decltype(std::forward<Self>(self).Sorter::operator()(
                std::move(first), std::move(last)))
        {
            return std::forward<Self>(self).Sorter::operator()(
                std::move(first), std::move(last));
        }

        template<typename Self, mstd::forward_range Range>
            requires (
                not detail::has_projection_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                    mstd::iterator_t<Range>,
                std::identity
                >::value &&
                not detail::has_comparison_projection_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                    mstd::iterator_t<Range>,
                    std::less<>,
                    std::identity
                >::value
            )
        constexpr auto operator()(this Self&& self, Range&& range, std::identity)
            -> decltype(std::forward<Self>(self)(std::forward<Range>(range)))
        {
            return std::forward<Self>(self)(std::forward<Range>(range));
        }

        ////////////////////////////////////////////////////////////
        // Fused comparison-projection overloads

        template<
            typename Self,
            mstd::forward_iterator Iterator,
            mstd::sentinel_for<Iterator> Sentinel,
            typename Compare,
            typename Projection
        >
            requires detail::has_comparison_projection_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                Iterator,
                refined_t<std::iter_reference_t<Iterator>, Compare>,
                refined_t<std::iter_reference_t<Iterator>, Projection>
            >::value
        constexpr auto operator()(this Self&& self, Iterator first, Sentinel last, Compare compare, Projection projection)
            -> decltype(std::forward<Self>(self).Sorter::operator()(
                first, last,
                refined<std::iter_reference_t<Iterator>>(std::move(compare)),
                refined<std::iter_reference_t<Iterator>>(std::move(projection))))
        {
            return std::forward<Self>(self).Sorter::operator()(
                first, last,
                refined<std::iter_reference_t<Iterator>>(std::move(compare)),
                refined<std::iter_reference_t<Iterator>>(std::move(projection)));
        }

        template<
            typename Self,
            mstd::forward_range Range,
            typename Compare,
            typename Projection
        >
            requires detail::has_comparison_projection_sort<
                detail::copy_cvref_t<Self, Sorter>,
                Range,
                refined_t<mstd::range_reference_t<Range>, Compare>,
                refined_t<mstd::range_reference_t<Range>, Projection>
            >::value
        constexpr auto operator()(this Self&& self, Range&& range, Compare compare, Projection projection)
            -> decltype(std::forward<Self>(self).Sorter::operator()(
                std::forward<Range>(range),
                refined<mstd::range_reference_t<Range>>(std::move(compare)),
                refined<mstd::range_reference_t<Range>>(std::move(projection))))
        {
            return std::forward<Self>(self).Sorter::operator()(
                std::forward<Range>(range),
                refined<mstd::range_reference_t<Range>>(std::move(compare)),
                refined<mstd::range_reference_t<Range>>(std::move(projection)));
        }

        template<
            typename Self,
            mstd::forward_range Range,
            typename Compare,
            typename Projection
        >
            requires (
                not detail::has_comparison_projection_sort<
                    detail::copy_cvref_t<Self, Sorter>,
                    Range,
                    refined_t<mstd::range_reference_t<Range>, Compare>,
                    refined_t<mstd::range_reference_t<Range>, Projection>
                >::value &&
                detail::has_comparison_projection_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                    mstd::iterator_t<Range>,
                    refined_t<mstd::range_reference_t<Range>, Compare>,
                    refined_t<mstd::range_reference_t<Range>, Projection>
                >::value
            )
        constexpr auto operator()(this Self&& self, Range&& range, Compare compare, Projection projection)
            -> decltype(std::forward<Self>(self).Sorter::operator()(
                mstd::begin(range), mstd::end(range),
                refined<mstd::range_reference_t<Range>>(compare),
                refined<mstd::range_reference_t<Range>>(projection)))
        {
            return std::forward<Self>(self).Sorter::operator()(
                mstd::begin(range), mstd::end(range),
                refined<mstd::range_reference_t<Range>>(std::move(compare)),
                refined<mstd::range_reference_t<Range>>(std::move(projection)));
        }

        ////////////////////////////////////////////////////////////
        // std::less<> and std::identity overloads

        template<
            typename Self,
            mstd::forward_iterator Iterator,
            mstd::sentinel_for<Iterator> Sentinel
        >
            requires (
                not detail::has_comparison_projection_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                    Iterator,
                    std::less<>,
                    std::identity
                >::value
            )
        constexpr auto operator()(this Self&& self, Iterator first, Sentinel last,
                                  std::less<>, std::identity)
            -> decltype(std::forward<Self>(self).Sorter::operator()(
                std::move(first), std::move(last)))
        {
            return std::forward<Self>(self).Sorter::operator()(
                std::move(first), std::move(last));
        }

        template<typename Self, mstd::forward_range Range>
            requires (
                not detail::has_comparison_projection_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                    mstd::iterator_t<Range>,
                    std::less<>,
                    std::identity
                >::value
            )
        constexpr auto operator()(this Self&& self, Range&& range, std::less<>, std::identity)
            -> decltype(std::forward<Self>(self)(std::forward<Range>(range)))
        {
            return std::forward<Self>(self)(std::forward<Range>(range));
        }

        template<
            typename Self,
            mstd::forward_iterator Iterator,
            mstd::sentinel_for<Iterator> Sentinel,
            typename Projection
        >
            requires detail::has_comparison_projection_sort_iterator<
                detail::copy_cvref_t<Self, Sorter>,
                Iterator,
                std::less<>,
                refined_t<std::iter_reference_t<Iterator>, Projection>
            >::value
        constexpr auto operator()(this Self&& self, Iterator first, Sentinel last,
                                  std::less<> compare, Projection projection)
            -> decltype(std::forward<Self>(self).Sorter::operator()(
                std::move(first), std::move(last), compare,
                refined<std::iter_reference_t<Iterator>>(std::move(projection))))
        {
            return std::forward<Self>(self).Sorter::operator()(
                std::move(first), std::move(last), compare,
                refined<std::iter_reference_t<Iterator>>(std::move(projection)));
        }

        template<
            typename Self,
            mstd::forward_iterator Iterator,
            mstd::sentinel_for<Iterator> Sentinel,
            typename Projection
        >
            requires (
                not detail::has_comparison_projection_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                    Iterator,
                    std::less<>,
                    refined_t<std::iter_reference_t<Iterator>, Projection>
                >::value &&
                detail::has_projection_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                    Iterator,
                    refined_t<std::iter_reference_t<Iterator>, Projection>
                >::value
            )
        constexpr auto operator()(this Self&& self, Iterator first, Sentinel last,
                                  std::less<>, Projection projection)
            -> decltype(std::forward<Self>(self).Sorter::operator()(
                std::move(first), std::move(last),
                refined<std::iter_reference_t<Iterator>>(std::move(projection))))
        {
            return std::forward<Self>(self).Sorter::operator()(
                std::move(first), std::move(last),
                refined<std::iter_reference_t<Iterator>>(std::move(projection)));
        }

        template<typename Self, mstd::forward_range Range, typename Projection>
            requires detail::has_comparison_projection_sort<
                detail::copy_cvref_t<Self, Sorter>,
                Range,
                std::less<>,
                refined_t<mstd::range_reference_t<Range>, Projection>
            >::value
        constexpr auto operator()(this Self&& self, Range&& range,
                                  std::less<> compare, Projection projection)
            -> decltype(std::forward<Self>(self).Sorter::operator()(
                std::forward<Range>(range), compare,
                refined<mstd::range_reference_t<Range>>(std::move(projection))))
        {
            return std::forward<Self>(self).Sorter::operator()(
                std::forward<Range>(range), compare,
                refined<mstd::range_reference_t<Range>>(std::move(projection)));
        }

        template<typename Self, mstd::forward_range Range, typename Projection>
            requires (
                not detail::has_comparison_projection_sort<
                    detail::copy_cvref_t<Self, Sorter>,
                    Range,
                    std::less<>,
                    refined_t<mstd::range_reference_t<Range>, Projection>
                >::value &&
                detail::has_comparison_projection_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                    mstd::iterator_t<Range>,
                    std::less<>,
                    refined_t<mstd::range_reference_t<Range>, Projection>
                >::value
            )
        constexpr auto operator()(this Self&& self, Range&& range,
                                  std::less<> compare, Projection projection)
            -> decltype(std::forward<Self>(self).Sorter::operator()(
                mstd::begin(range), mstd::end(range), compare,
                refined<mstd::range_reference_t<Range>>(std::move(projection))))
        {
            return std::forward<Self>(self).Sorter::operator()(
                mstd::begin(range), mstd::end(range), compare,
                refined<mstd::range_reference_t<Range>>(std::move(projection)));
        }

        template<typename Self, mstd::forward_range Range, typename Projection>
            requires (
                not detail::has_comparison_projection_sort<
                    detail::copy_cvref_t<Self, Sorter>,
                    Range,
                    std::less<>,
                    refined_t<mstd::range_reference_t<Range>, Projection>
                >::value &&
                not detail::has_comparison_projection_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                    mstd::iterator_t<Range>,
                    std::less<>,
                    refined_t<mstd::range_reference_t<Range>, Projection>
                >::value &&
                detail::has_projection_sort<
                    detail::copy_cvref_t<Self, Sorter>,
                    Range,
                    refined_t<mstd::range_reference_t<Range>, Projection>
                >::value
            )
        constexpr auto operator()(this Self&& self, Range&& range,
                                  std::less<>, Projection projection)
            -> decltype(std::forward<Self>(self).Sorter::operator()(
                std::forward<Range>(range),
                refined<mstd::range_reference_t<Range>>(std::move(projection))))
        {
            return std::forward<Self>(self).Sorter::operator()(
                std::forward<Range>(range),
                refined<mstd::range_reference_t<Range>>(std::move(projection)));
        }

        template<typename Self, mstd::forward_range Range, typename Projection>
            requires (
                not detail::has_comparison_projection_sort<
                    detail::copy_cvref_t<Self, Sorter>,
                    Range,
                    std::less<>,
                    refined_t<mstd::range_reference_t<Range>, Projection>
                >::value &&
                not detail::has_comparison_projection_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                    mstd::iterator_t<Range>,
                    std::less<>,
                    refined_t<mstd::range_reference_t<Range>, Projection>
                >::value &&
                not detail::has_projection_sort<
                    detail::copy_cvref_t<Self, Sorter>,
                    Range,
                    refined_t<mstd::range_reference_t<Range>, Projection>
                >::value &&
                detail::has_projection_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                    mstd::iterator_t<Range>,
                    refined_t<mstd::range_reference_t<Range>, Projection>
                >::value
            )
        constexpr auto operator()(this Self&& self, Range&& range,
                                  std::less<>, Projection projection)
            -> decltype(std::forward<Self>(self).Sorter::operator()(
                mstd::begin(range), mstd::end(range),
                refined<mstd::range_reference_t<Range>>(std::move(projection))))
        {
            return std::forward<Self>(self).Sorter::operator()(
                mstd::begin(range), mstd::end(range),
                refined<mstd::range_reference_t<Range>>(std::move(projection)));
        }

        template<
            typename Self,
            mstd::forward_iterator Iterator,
            mstd::sentinel_for<Iterator> Sentinel
        >
            requires (
                not detail::has_comparison_projection_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                    Iterator,
                    std::ranges::less,
                    std::identity
                >::value
            )
        constexpr auto operator()(this Self&& self, Iterator first, Sentinel last,
                                  std::ranges::less, std::identity)
            -> decltype(std::forward<Self>(self).Sorter::operator()(
                std::move(first), std::move(last)))
        {
            return std::forward<Self>(self).Sorter::operator()(
                std::move(first), std::move(last));
        }

        template<typename Self, mstd::forward_range Range>
            requires (
                not detail::has_comparison_projection_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                    mstd::iterator_t<Range>,
                    std::ranges::less,
                    std::identity
                >::value
            )
        constexpr auto operator()(this Self&& self, Range&& range, std::ranges::less, std::identity)
            -> decltype(std::forward<Self>(self)(std::forward<Range>(range)))
        {
            return std::forward<Self>(self)(std::forward<Range>(range));
        }

        template<
            typename Self,
            mstd::forward_iterator Iterator,
            mstd::sentinel_for<Iterator> Sentinel,
            typename Projection
        >
            requires detail::has_comparison_projection_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                Iterator,
                std::ranges::less,
                refined_t<std::iter_reference_t<Iterator>, Projection>
            >::value
        constexpr auto operator()(this Self&& self, Iterator first, Sentinel last,
                                  std::ranges::less compare, Projection projection)
            -> decltype(std::forward<Self>(self).Sorter::operator()(
                std::move(first), std::move(last), compare,
                refined<std::iter_reference_t<Iterator>>(std::move(projection))))
        {
            return std::forward<Self>(self).Sorter::operator()(
                std::move(first), std::move(last), compare,
                refined<std::iter_reference_t<Iterator>>(std::move(projection)));
        }

        template<
            typename Self,
            mstd::forward_iterator Iterator,
            mstd::sentinel_for<Iterator> Sentinel,
            typename Projection
        >
            requires (
                not detail::has_comparison_projection_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                    Iterator,
                    std::ranges::less,
                    refined_t<std::iter_reference_t<Iterator>, Projection>
                >::value &&
                detail::has_projection_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                    Iterator,
                    refined_t<std::iter_reference_t<Iterator>, Projection>
                >::value
            )
        constexpr auto operator()(this Self&& self, Iterator first, Sentinel last,
                                  std::ranges::less, Projection projection)
            -> decltype(std::forward<Self>(self).Sorter::operator()(
                std::move(first), std::move(last),
                refined<std::iter_reference_t<Iterator>>(std::move(projection))))
        {
            return std::forward<Self>(self).Sorter::operator()(
                std::move(first), std::move(last),
                refined<std::iter_reference_t<Iterator>>(std::move(projection)));
        }

        template<typename Self, mstd::forward_range Range, typename Projection>
            requires detail::has_comparison_projection_sort<
                detail::copy_cvref_t<Self, Sorter>,
                Range,
                std::ranges::less,
                refined_t<mstd::range_reference_t<Range>, Projection>
            >::value
        constexpr auto operator()(this Self&& self, Range&& range,
                                  std::ranges::less compare, Projection projection)
            -> decltype(std::forward<Self>(self).Sorter::operator()(
                std::forward<Range>(range), compare,
                refined<mstd::range_reference_t<Range>>(std::move(projection))))
        {
            return std::forward<Self>(self).Sorter::operator()(
                std::forward<Range>(range), compare,
                refined<mstd::range_reference_t<Range>>(std::move(projection)));
        }

        template<typename Self, mstd::forward_range Range, typename Projection>
            requires (
                not detail::has_comparison_projection_sort<
                    detail::copy_cvref_t<Self, Sorter>,
                    Range,
                    std::ranges::less,
                    refined_t<mstd::range_reference_t<Range>, Projection>
                >::value &&
                detail::has_comparison_projection_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                    mstd::iterator_t<Range>,
                    std::ranges::less,
                    refined_t<mstd::range_reference_t<Range>, Projection>
                >::value
            )
        constexpr auto operator()(this Self&& self, Range&& range,
                                  std::ranges::less compare, Projection projection)
            -> decltype(std::forward<Self>(self).Sorter::operator()(
                mstd::begin(range), mstd::end(range), compare,
                refined<mstd::range_reference_t<Range>>(std::move(projection))))
        {
            return std::forward<Self>(self).Sorter::operator()(
                mstd::begin(range), mstd::end(range), compare,
                refined<mstd::range_reference_t<Range>>(std::move(projection)));
        }

        template<typename Self, mstd::forward_range Range, typename Projection>
            requires (
                not detail::has_comparison_projection_sort<
                    detail::copy_cvref_t<Self, Sorter>,
                    Range,
                    std::ranges::less,
                    refined_t<mstd::range_reference_t<Range>, Projection>
                >::value &&
                not detail::has_comparison_projection_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                    mstd::iterator_t<Range>,
                    std::ranges::less,
                    refined_t<mstd::range_reference_t<Range>, Projection>
                >::value &&
                detail::has_projection_sort<
                    detail::copy_cvref_t<Self, Sorter>,
                    Range,
                    refined_t<mstd::range_reference_t<Range>, Projection>
                >::value
            )
        constexpr auto operator()(this Self&& self, Range&& range,
                                  std::ranges::less, Projection projection)
            -> decltype(std::forward<Self>(self).Sorter::operator()(
                std::forward<Range>(range),
                refined<mstd::range_reference_t<Range>>(std::move(projection))))
        {
            return std::forward<Self>(self).Sorter::operator()(
                std::forward<Range>(range),
                refined<mstd::range_reference_t<Range>>(std::move(projection)));
        }

        template<typename Self, mstd::forward_range Range, typename Projection>
            requires (
                not detail::has_comparison_projection_sort<
                    detail::copy_cvref_t<Self, Sorter>,
                    Range,
                    std::ranges::less,
                    refined_t<mstd::range_reference_t<Range>, Projection>
                >::value &&
                not detail::has_comparison_projection_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                    mstd::iterator_t<Range>,
                    std::ranges::less,
                    refined_t<mstd::range_reference_t<Range>, Projection>
                >::value &&
                not detail::has_projection_sort<
                    detail::copy_cvref_t<Self, Sorter>,
                    Range,
                    refined_t<mstd::range_reference_t<Range>, Projection>
                >::value &&
                detail::has_projection_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                    mstd::iterator_t<Range>,
                    refined_t<mstd::range_reference_t<Range>, Projection>
                >::value
            )
        constexpr auto operator()(this Self&& self, Range&& range,
                                  std::ranges::less, Projection projection)
            -> decltype(std::forward<Self>(self).Sorter::operator()(
                mstd::begin(range), mstd::end(range),
                refined<mstd::range_reference_t<Range>>(std::move(projection))))
        {
            return std::forward<Self>(self).Sorter::operator()(
                mstd::begin(range), mstd::end(range),
                refined<mstd::range_reference_t<Range>>(std::move(projection)));
        }

        ////////////////////////////////////////////////////////////
        // Embed projection in comparison

        template<
            typename Self,
            mstd::forward_iterator Iterator,
            mstd::sentinel_for<Iterator> Sentinel,
            typename Projection
        >
            requires (
                not detail::has_projection_sort_iterator<Sorter, Iterator, Projection>::value &&
                not detail::has_comparison_projection_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                    Iterator,
                    std::less<>,
                    refined_t<std::iter_reference_t<Iterator>, Projection>
                >::value &&
                detail::has_comparison_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                    Iterator,
                    projection_compare_t<std::less<>, refined_t<std::iter_reference_t<Iterator>, Projection>>
                >::value
            )
        constexpr auto operator()(this Self&& self, Iterator first, Sentinel last, Projection projection)
            -> decltype(std::forward<Self>(self).Sorter::operator()(
                first, last, projection_compare(std::less{},
                    refined<std::iter_reference_t<Iterator>>(std::move(projection)))))
        {
            return std::forward<Self>(self).Sorter::operator()(
                first, last, projection_compare(std::less{},
                    refined<std::iter_reference_t<Iterator>>(std::move(projection))));
        }

        template<
            typename Self,
            mstd::forward_iterator Iterator,
            mstd::sentinel_for<Iterator> Sentinel,
            typename Compare,
            typename Projection
        >
            requires (
                not detail::has_comparison_projection_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                    Iterator,
                    refined_t<std::iter_reference_t<Iterator>, Compare>,
                    refined_t<std::iter_reference_t<Iterator>, Projection>
                >::value &&
                detail::has_comparison_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                    Iterator,
                    projection_compare_t<
                        refined_t<std::iter_reference_t<Iterator>, Compare>,
                        refined_t<std::iter_reference_t<Iterator>, Projection>
                    >
                >::value
            )
        constexpr auto operator()(this Self&& self, Iterator first, Sentinel last,
                                  Compare compare, Projection projection)
            -> decltype(std::forward<Self>(self).Sorter::operator()(first, last, projection_compare(
                    refined<std::iter_reference_t<Iterator>>(std::move(compare)),
                    refined<std::iter_reference_t<Iterator>>(std::move(projection)))))
        {
            return std::forward<Self>(self).Sorter::operator()(first, last, projection_compare(
                refined<std::iter_reference_t<Iterator>>(std::move(compare)),
                refined<std::iter_reference_t<Iterator>>(std::move(projection)))
            );
        }

        template<typename Self, mstd::forward_range Range, typename Projection>
            requires (
                not detail::has_projection_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                    mstd::iterator_t<Range>,
                    refined_t<mstd::range_reference_t<Range>, Projection>
                >::value &&
                not detail::has_comparison_projection_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                    mstd::iterator_t<Range>,
                    std::less<>,
                    refined_t<mstd::range_reference_t<Range>, Projection>
                >::value &&
                detail::has_comparison_sort<
                    detail::copy_cvref_t<Self, Sorter>,
                    Range,
                    projection_compare_t<
                        std::less<>,
                        refined_t<mstd::range_reference_t<Range>, Projection>
                    >
                >::value
            )
        constexpr auto operator()(this Self&& self, Range&& range, Projection projection)
            -> decltype(std::forward<Self>(self).Sorter::operator()(
                std::forward<Range>(range), projection_compare(
                    std::less{}, refined<mstd::range_reference_t<Range>>(std::move(projection)))))
        {
            return std::forward<Self>(self).Sorter::operator()(
                std::forward<Range>(range), projection_compare(
                    std::less{}, refined<mstd::range_reference_t<Range>>(std::move(projection))));
        }

        template<typename Self, mstd::forward_range Range, typename Projection>
            requires (
                not detail::has_projection_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                    mstd::iterator_t<Range>,
                    refined_t<mstd::range_reference_t<Range>, Projection>
                >::value &&
                not detail::has_comparison_projection_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                    mstd::iterator_t<Range>,
                    std::less<>,
                    refined_t<mstd::range_reference_t<Range>, Projection>
                >::value &&
                not detail::has_comparison_sort<
                    detail::copy_cvref_t<Self, Sorter>,
                    Range,
                    projection_compare_t<
                        std::less<>,
                        refined_t<mstd::range_reference_t<Range>, Projection>
                    >
                >::value &&
                detail::has_comparison_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                    mstd::iterator_t<Range>,
                    projection_compare_t<
                        std::less<>,
                        refined_t<mstd::range_reference_t<Range>, Projection>
                    >
                >::value
            )
        constexpr auto operator()(this Self&& self, Range&& range, Projection projection)
            -> decltype(std::forward<Self>(self).Sorter::operator()(
                mstd::begin(range), mstd::end(range),
                projection_compare(std::less{},
                    refined<mstd::range_reference_t<Range>>(std::move(projection)))))
        {
            return std::forward<Self>(self).Sorter::operator()(
                mstd::begin(range), mstd::end(range),
                projection_compare(std::less{},
                    refined<mstd::range_reference_t<Range>>(std::move(projection))));
        }

        template<
            typename Self,
            mstd::forward_range Range,
            typename Compare,
            typename Projection
        >
            requires (
                not detail::has_comparison_projection_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                    mstd::iterator_t<Range>,
                    refined_t<mstd::range_reference_t<Range>, Compare>,
                    refined_t<mstd::range_reference_t<Range>, Projection>
                >::value &&
                detail::has_comparison_sort<
                    detail::copy_cvref_t<Self, Sorter>,
                    Range,
                    projection_compare_t<
                        refined_t<mstd::range_reference_t<Range>, Compare>,
                        refined_t<mstd::range_reference_t<Range>, Projection>
                    >
                >::value
            )
        constexpr auto operator()(this Self&& self, Range&& range,
                                  Compare compare, Projection projection)
            -> decltype(std::forward<Self>(self).Sorter::operator()(
                std::forward<Range>(range), projection_compare(
                    refined<mstd::range_reference_t<Range>>(std::move(compare)),
                    refined<mstd::range_reference_t<Range>>(std::move(projection)))))
        {
            return std::forward<Self>(self).Sorter::operator()(
                std::forward<Range>(range), projection_compare(
                    refined<mstd::range_reference_t<Range>>(std::move(compare)),
                    refined<mstd::range_reference_t<Range>>(std::move(projection))));
        }

        template<
            typename Self,
            mstd::forward_range Range,
            typename Compare,
            typename Projection
        >
            requires (
                not detail::has_comparison_projection_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                    mstd::iterator_t<Range>,
                    refined_t<mstd::range_reference_t<Range>, Compare>,
                    refined_t<mstd::range_reference_t<Range>, Projection>
                >::value &&
                not detail::has_comparison_sort<
                    detail::copy_cvref_t<Self, Sorter>,
                    Range,
                    projection_compare_t<
                        refined_t<mstd::range_reference_t<Range>, Compare>,
                        refined_t<mstd::range_reference_t<Range>, Projection>
                    >
                >::value &&
                detail::has_comparison_sort_iterator<
                    detail::copy_cvref_t<Self, Sorter>,
                    mstd::iterator_t<Range>,
                    projection_compare_t<
                        refined_t<mstd::range_reference_t<Range>, Compare>,
                        refined_t<mstd::range_reference_t<Range>, Projection>
                    >
                >::value
            )
        constexpr auto operator()(this Self&& self, Range&& range,
                                  Compare compare, Projection projection)
            -> decltype(std::forward<Self>(self).Sorter::operator()(
                mstd::begin(range), mstd::end(range), projection_compare(
                    refined<mstd::range_reference_t<Range>>(std::move(compare)),
                    refined<mstd::range_reference_t<Range>>(std::move(projection)))))
        {
            return std::forward<Self>(self).Sorter::operator()(
                mstd::begin(range), mstd::end(range), projection_compare(
                    refined<mstd::range_reference_t<Range>>(std::move(compare)),
                    refined<mstd::range_reference_t<Range>>(std::move(projection))));
        }
    };
}

#endif // CPPSORT_SORTER_FACADE_H_
