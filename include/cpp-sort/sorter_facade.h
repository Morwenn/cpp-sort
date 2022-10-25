/*
 * Copyright (c) 2015-2022 Morwenn
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

            template<
                typename Ret,
                typename... Args
            >
            static constexpr auto invoke(Args... args)
                -> mstd::enable_if_t<
                    not std::is_void_v<Ret>,
                    Ret
                >
            {
                return Sorter{}(std::forward<Args>(args)...);
            }

            template<
                typename Ret,
                typename... Args
            >
            static constexpr auto invoke(Args... args)
                -> mstd::enable_if_t<
                    std::is_void_v<Ret>,
                    void
                >
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
                constexpr operator fptr_t<Ret, Range&>() const
                {
                    return invoker<Sorter>::template invoke<Ret, Range&>;
                }

                template<typename Ret, mstd::forward_range Range>
                constexpr operator fptr_t<Ret, Range&&>() const
                {
                    return invoker<Sorter>::template invoke<Ret, Range&&>;
                }

                template<typename Ret, mstd::forward_range Range, typename Func>
                constexpr operator fptr_t<Ret, Range&, Func>() const
                {
                    return invoker<Sorter>::template invoke<Ret, Range&, Func>;
                }

                template<typename Ret, mstd::forward_range Range, typename Func>
                constexpr operator fptr_t<Ret, Range&&, Func>() const
                {
                    return invoker<Sorter>::template invoke<Ret, Range&&, Func>;
                }

                template<typename Ret, mstd::forward_range Range, typename Func1, typename Func2>
                constexpr operator fptr_t<Ret, Range&, Func1, Func2>() const
                {
                    return invoker<Sorter>::template invoke<Ret, Range&, Func1, Func2>;
                }

                template<typename Ret, mstd::forward_range Range, typename Func1, typename Func2>
                constexpr operator fptr_t<Ret, Range&&, Func1, Func2>() const
                {
                    return invoker<Sorter>::template invoke<Ret, Range&&, Func1, Func2>;
                }

                template<
                    typename Ret,
                    mstd::forward_iterator Iterator,
                    mstd::sentinel_for<Iterator> Sentinel
                >
                constexpr operator fptr_t<Ret, Iterator, Sentinel>() const
                {
                    return invoker<Sorter>::template invoke<Ret, Iterator, Sentinel>;
                }

                template<
                    typename Ret,
                    mstd::forward_iterator Iterator,
                    mstd::sentinel_for<Iterator> Sentinel,
                    typename Func
                >
                constexpr operator fptr_t<Ret, Iterator, Sentinel, Func>() const
                {
                    return invoker<Sorter>::template invoke<Ret, Iterator, Sentinel, Func>;
                }

                template<
                    typename Ret,
                    mstd::forward_iterator Iterator,
                    mstd::sentinel_for<Iterator> Sentinel,
                    typename Func1, typename Func2
                >
                constexpr operator fptr_t<Ret, Iterator, Sentinel, Func1, Func2>() const
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
            mstd::forward_iterator Iterator,
            mstd::sentinel_for<Iterator> Sentinel
        >
        constexpr auto operator()(Iterator first, Sentinel last) const
            -> decltype(Sorter::operator()(std::move(first), std::move(last)))
        {
            return Sorter::operator()(std::move(first), std::move(last));
        }

        template<mstd::forward_range Range>
        constexpr auto operator()(Range&& range) const
            -> mstd::enable_if_t<
                detail::has_sort<Sorter, Range>::value,
                decltype(Sorter::operator()(std::forward<Range>(range)))
            >
        {
            return Sorter::operator()(std::forward<Range>(range));
        }

        template<mstd::forward_range Range>
        constexpr auto operator()(Range&& range) const
            -> mstd::enable_if_t<
                not detail::has_sort<Sorter, Range>::value,
                decltype(Sorter::operator()(mstd::begin(range), mstd::end(range)))
            >
        {
            return Sorter::operator()(mstd::begin(range), mstd::end(range));
        }

        ////////////////////////////////////////////////////////////
        // Comparison overloads

        template<
            mstd::forward_iterator Iterator,
            mstd::sentinel_for<Iterator> Sentinel,
            typename Compare
        >
        constexpr auto operator()(Iterator first, Sentinel last, Compare compare) const
            -> mstd::enable_if_t<
                detail::has_comparison_sort_iterator<
                    Sorter,
                    Iterator,
                    refined_t<decltype(*first), Compare>
                >::value,
                decltype(Sorter::operator()(std::move(first), std::move(last),
                                            refined<decltype(*first)>(std::move(compare))))
            >
        {
            return Sorter::operator()(std::move(first), std::move(last),
                                      refined<decltype(*first)>(std::move(compare)));
        }

        template<mstd::forward_range Range, typename Compare>
        constexpr auto operator()(Range&& range, Compare compare) const
            -> mstd::enable_if_t<
                detail::has_comparison_sort<
                    Sorter,
                    Range,
                    refined_t<decltype(*mstd::begin(range)), Compare>
                >::value,
                decltype(Sorter::operator()(std::forward<Range>(range),
                                            refined<decltype(*mstd::begin(range))>(std::move(compare))))
            >
        {
            return Sorter::operator()(std::forward<Range>(range),
                                      refined<decltype(*mstd::begin(range))>(std::move(compare)));
        }

        template<mstd::forward_range Range, typename Compare>
        constexpr auto operator()(Range&& range, Compare compare) const
            -> mstd::enable_if_t<
                not detail::has_comparison_sort<
                    Sorter,
                    Range,
                    refined_t<decltype(*mstd::begin(range)), Compare>
                >::value &&
                detail::has_comparison_sort_iterator<
                    Sorter,
                    mstd::iterator_t<Range>,
                    refined_t<decltype(*mstd::begin(range)), Compare>
                >::value,
                decltype(Sorter::operator()(mstd::begin(range), mstd::end(range),
                                            refined<decltype(*mstd::begin(range))>(std::move(compare))))
            >
        {
            return Sorter::operator()(mstd::begin(range), mstd::end(range),
                                      refined<decltype(*mstd::begin(range))>(std::move(compare)));
        }

        ////////////////////////////////////////////////////////////
        // Projection overloads

        template<
            mstd::forward_iterator Iterator,
            mstd::sentinel_for<Iterator> Sentinel,
            typename Projection
        >
        constexpr auto operator()(Iterator first, Sentinel last, Projection projection) const
            -> mstd::enable_if_t<
                not detail::has_comparison_sort_iterator<
                    Sorter,
                    Iterator,
                    refined_t<decltype(*first), Projection>
                >::value &&
                detail::has_projection_sort_iterator<
                    Sorter,
                    Iterator,
                    refined_t<decltype(*first), Projection>
                >::value,
                decltype(Sorter::operator()(std::move(first), std::move(last),
                                            refined<decltype(*first)>(std::move(projection))))
            >
        {
            return Sorter::operator()(std::move(first), std::move(last),
                                      refined<decltype(*first)>(std::move(projection)));
        }

        template<
            mstd::forward_iterator Iterator,
            mstd::sentinel_for<Iterator> Sentinel,
            typename Projection
        >
        constexpr auto operator()(Iterator first, Sentinel last, Projection projection) const
            -> mstd::enable_if_t<
                not detail::has_projection_sort_iterator<
                    Sorter,
                    Iterator,
                    refined_t<decltype(*first), Projection>
                >::value &&
                detail::has_comparison_projection_sort_iterator<
                    Sorter,
                    Iterator,
                    std::less<>,
                    refined_t<decltype(*first), Projection>
                >::value,
                decltype(Sorter::operator()(std::move(first), std::move(last), std::less{},
                                            refined<decltype(*first)>(std::move(projection))))
            >
        {
            return Sorter::operator()(std::move(first), std::move(last), std::less{},
                                      refined<decltype(*first)>(std::move(projection)));
        }

        template<mstd::forward_range Range, typename Projection>
        constexpr auto operator()(Range&& range, Projection projection) const
            -> mstd::enable_if_t<
                not detail::has_comparison_sort<
                    Sorter,
                    Range,
                    refined_t<decltype(*mstd::begin(range)), Projection>
                >::value &&
                detail::has_projection_sort<
                    Sorter,
                    Range,
                    refined_t<decltype(*mstd::begin(range)), Projection>
                >::value,
                decltype(Sorter::operator()(std::forward<Range>(range),
                                            refined<decltype(*mstd::begin(range))>(std::move(projection))))
            >
        {
            return Sorter::operator()(std::forward<Range>(range),
                                      refined<decltype(*mstd::begin(range))>(std::move(projection)));
        }

        template<mstd::forward_range Range, typename Projection>
        constexpr auto operator()(Range&& range, Projection projection) const
            -> mstd::enable_if_t<
                not detail::has_comparison_sort<
                    Sorter,
                    Range,
                    refined_t<decltype(*mstd::begin(range)), Projection>
                >::value &&
                not detail::has_projection_sort<
                    Sorter,
                    Range,
                    refined_t<decltype(*mstd::begin(range)), Projection>
                >::value &&
                detail::has_comparison_projection_sort<
                    Sorter,
                    Range,
                    std::less<>,
                    refined_t<decltype(*mstd::begin(range)), Projection>
                >::value,
                decltype(Sorter::operator()(std::forward<Range>(range), std::less{},
                                            refined<decltype(*mstd::begin(range))>(std::move(projection))))
            >
        {
            return Sorter::operator()(std::forward<Range>(range), std::less{},
                                      refined<decltype(*mstd::begin(range))>(std::move(projection)));
        }

        template<mstd::forward_range Range, typename Projection>
        constexpr auto operator()(Range&& range, Projection projection) const
            -> mstd::enable_if_t<
                not detail::has_comparison_sort<
                    Sorter,
                    Range,
                    refined_t<decltype(*mstd::begin(range)), Projection>
                >::value &&
                not detail::has_comparison_sort_iterator<
                    Sorter,
                    mstd::iterator_t<Range>,
                    refined_t<decltype(*mstd::begin(range)), Projection>
                >::value &&
                not detail::has_projection_sort<
                    Sorter,
                    Range,
                    refined_t<decltype(*mstd::begin(range)), Projection>
                >::value &&
                not detail::has_comparison_projection_sort<
                    Sorter,
                    Range,
                    std::less<>,
                    refined_t<decltype(*mstd::begin(range)), Projection>
                >::value &&
                detail::has_projection_sort_iterator<
                    Sorter,
                    mstd::iterator_t<Range>,
                    refined_t<decltype(*mstd::begin(range)), Projection>
                >::value,
                decltype(Sorter::operator()(mstd::begin(range), mstd::end(range),
                                            refined<decltype(*mstd::begin(range))>(std::move(projection))))
            >
        {
            return Sorter::operator()(mstd::begin(range), mstd::end(range),
                                      refined<decltype(*mstd::begin(range))>(std::move(projection)));
        }

        template<mstd::forward_range Range, typename Projection>
        constexpr auto operator()(Range&& range, Projection projection) const
            -> mstd::enable_if_t<
                not detail::has_projection_sort<
                    Sorter,
                    Range,
                    refined_t<decltype(*mstd::begin(range)), Projection>
                >::value &&
                not detail::has_comparison_projection_sort<
                    Sorter,
                    Range,
                    std::less<>,
                    refined_t<decltype(*mstd::begin(range)), Projection>
                >::value &&
                not detail::has_projection_sort_iterator<
                    Sorter,
                    mstd::iterator_t<Range>,
                    refined_t<decltype(*mstd::begin(range)), Projection>
                >::value &&
                detail::has_comparison_projection_sort_iterator<
                    Sorter,
                    mstd::iterator_t<Range>,
                    std::less<>,
                    refined_t<decltype(*mstd::begin(range)), Projection>
                >::value,
                decltype(Sorter::operator()(mstd::begin(range), mstd::end(range), std::less{},
                                            refined<decltype(*mstd::begin(range))>(std::move(projection))))
            >
        {
            return Sorter::operator()(mstd::begin(range), mstd::end(range), std::less{},
                                      refined<decltype(*mstd::begin(range))>(std::move(projection)));
        }

        ////////////////////////////////////////////////////////////
        // std::less<> overloads

        template<
            mstd::forward_iterator Iterator,
            mstd::sentinel_for<Iterator> Sentinel
        >
        constexpr auto operator()(Iterator first, Sentinel last, std::less<>) const
            -> mstd::enable_if_t<
                not detail::has_comparison_sort_iterator<Sorter, Iterator, std::less<>>::value,
                decltype(Sorter::operator()(std::move(first), std::move(last)))
            >
        {
            return Sorter::operator()(std::move(first), std::move(last));
        }

        template<mstd::forward_range Range>
        constexpr auto operator()(Range&& range, std::less<>) const
            -> mstd::enable_if_t<
                not detail::has_comparison_sort_iterator<
                    Sorter,
                    mstd::iterator_t<Range>,
                    std::less<>
                >::value,
                decltype(operator()(std::forward<Range>(range)))
            >
        {
            return operator()(std::forward<Range>(range));
        }

        template<
            mstd::forward_iterator Iterator,
            mstd::sentinel_for<Iterator> Sentinel
        >
        constexpr auto operator()(Iterator first, Sentinel last, std::ranges::less) const
            -> mstd::enable_if_t<
                not detail::has_comparison_sort_iterator<Sorter, Iterator, std::ranges::less>::value,
                decltype(Sorter::operator()(std::move(first), std::move(last)))
            >
        {
            return Sorter::operator()(std::move(first), std::move(last));
        }

        template<mstd::forward_range Range>
        constexpr auto operator()(Range&& range, std::ranges::less) const
            -> mstd::enable_if_t<
                not detail::has_comparison_sort_iterator<
                    Sorter,
                    mstd::iterator_t<Range>,
                    std::ranges::less
                >::value,
                decltype(operator()(std::forward<Range>(range)))
            >
        {
            return operator()(std::forward<Range>(range));
        }

        ////////////////////////////////////////////////////////////
        // std::identity overloads

        template<
            mstd::forward_iterator Iterator,
            mstd::sentinel_for<Iterator> Sentinel
        >
        constexpr auto operator()(Iterator first, Sentinel last, std::identity) const
            -> mstd::enable_if_t<
                not detail::has_projection_sort_iterator<Sorter, Iterator, std::identity>::value &&
                not detail::has_comparison_projection_sort_iterator<
                    Sorter,
                    Iterator,
                    std::less<>,
                    std::identity
                >::value,
                decltype(Sorter::operator()(std::move(first), std::move(last)))
            >
        {
            return Sorter::operator()(std::move(first), std::move(last));
        }

        template<mstd::forward_range Range>
        constexpr auto operator()(Range&& range, std::identity) const
            -> mstd::enable_if_t<
                not detail::has_projection_sort_iterator<
                    Sorter,
                    mstd::iterator_t<Range>,
                std::identity
                >::value &&
                not detail::has_comparison_projection_sort_iterator<
                    Sorter,
                    mstd::iterator_t<Range>,
                    std::less<>,
                    std::identity
                >::value,
                decltype(operator()(std::forward<Range>(range)))
            >
        {
            return operator()(std::forward<Range>(range));
        }

        ////////////////////////////////////////////////////////////
        // Fused comparison-projection overloads

        template<
            mstd::forward_iterator Iterator,
            mstd::sentinel_for<Iterator> Sentinel,
            typename Compare,
            typename Projection
        >
        constexpr auto operator()(Iterator first, Sentinel last, Compare compare, Projection projection) const
            -> mstd::enable_if_t<
                detail::has_comparison_projection_sort_iterator<
                    Sorter,
                    Iterator,
                    refined_t<decltype(*first), Compare>,
                    refined_t<decltype(*first), Projection>
                >::value,
                decltype(Sorter::operator()(first, last,
                                            refined<decltype(*first)>(std::move(compare)),
                                            refined<decltype(*first)>(std::move(projection))))
            >
        {
            return Sorter::operator()(first, last,
                                      refined<decltype(*first)>(std::move(compare)),
                                      refined<decltype(*first)>(std::move(projection)));
        }

        template<mstd::forward_range Range, typename Compare, typename Projection>
        constexpr auto operator()(Range&& range, Compare compare, Projection projection) const
            -> mstd::enable_if_t<
                detail::has_comparison_projection_sort<
                    Sorter,
                    Range,
                    refined_t<decltype(*mstd::begin(range)), Compare>,
                    refined_t<decltype(*mstd::begin(range)), Projection>
                >::value,
                decltype(Sorter::operator()(std::forward<Range>(range),
                                            refined<decltype(*mstd::begin(range))>(std::move(compare)),
                                            refined<decltype(*mstd::begin(range))>(std::move(projection))))
            >
        {
            return Sorter::operator()(std::forward<Range>(range),
                                      refined<decltype(*mstd::begin(range))>(std::move(compare)),
                                      refined<decltype(*mstd::begin(range))>(std::move(projection)));
        }

        template<mstd::forward_range Range, typename Compare, typename Projection>
        constexpr auto operator()(Range&& range, Compare compare, Projection projection) const
            -> mstd::enable_if_t<
                not detail::has_comparison_projection_sort<
                    Sorter,
                    Range,
                    refined_t<decltype(*mstd::begin(range)), Compare>,
                    refined_t<decltype(*mstd::begin(range)), Projection>
                >::value &&
                detail::has_comparison_projection_sort_iterator<
                    Sorter,
                    mstd::iterator_t<Range>,
                    refined_t<decltype(*mstd::begin(range)), Compare>,
                    refined_t<decltype(*mstd::begin(range)), Projection>
                >::value,
                decltype(Sorter::operator()(mstd::begin(range), mstd::end(range),
                                            refined<decltype(*mstd::begin(range))>(compare),
                                            refined<decltype(*mstd::begin(range))>(projection)))
            >
        {
            return Sorter::operator()(mstd::begin(range), mstd::end(range),
                                      refined<decltype(*mstd::begin(range))>(std::move(compare)),
                                      refined<decltype(*mstd::begin(range))>(std::move(projection)));
        }

        ////////////////////////////////////////////////////////////
        // std::less<> and std::identity overloads

        template<
            mstd::forward_iterator Iterator,
            mstd::sentinel_for<Iterator> Sentinel
        >
        constexpr auto operator()(Iterator first, Sentinel last, std::less<>, std::identity) const
            -> mstd::enable_if_t<
                not detail::has_comparison_projection_sort_iterator<
                    Sorter,
                    Iterator,
                    std::less<>,
                    std::identity
                >::value,
                decltype(Sorter::operator()(std::move(first), std::move(last)))
            >
        {
            return Sorter::operator()(std::move(first), std::move(last));
        }

        template<mstd::forward_range Range>
        constexpr auto operator()(Range&& range, std::less<>, std::identity) const
            -> mstd::enable_if_t<
                not detail::has_comparison_projection_sort_iterator<
                    Sorter,
                    mstd::iterator_t<Range>,
                    std::less<>,
                    std::identity
                >::value,
                decltype(operator()(std::forward<Range>(range)))
            >
        {
            return operator()(std::forward<Range>(range));
        }

        template<
            mstd::forward_iterator Iterator,
            mstd::sentinel_for<Iterator> Sentinel,
            typename Projection
        >
        constexpr auto operator()(Iterator first, Sentinel last, std::less<> compare, Projection projection) const
            -> mstd::enable_if_t<
                detail::has_comparison_projection_sort_iterator<
                    Sorter,
                    Iterator,
                    std::less<>,
                    refined_t<decltype(*first), Projection>
                >::value,
                decltype(Sorter::operator()(std::move(first), std::move(last), compare,
                                            refined<decltype(*first)>(std::move(projection))))
            >
        {
            return Sorter::operator()(std::move(first), std::move(last), compare,
                                      refined<decltype(*first)>(std::move(projection)));
        }

        template<
            mstd::forward_iterator Iterator,
            mstd::sentinel_for<Iterator> Sentinel,
            typename Projection
        >
        constexpr auto operator()(Iterator first, Sentinel last, std::less<>, Projection projection) const
            -> mstd::enable_if_t<
                not detail::has_comparison_projection_sort_iterator<
                    Sorter,
                    Iterator,
                    std::less<>,
                    refined_t<decltype(*first), Projection>
                >::value &&
                detail::has_projection_sort_iterator<
                    Sorter,
                    Iterator,
                    refined_t<decltype(*first), Projection>
                >::value,
                decltype(Sorter::operator()(std::move(first), std::move(last),
                                            refined<decltype(*first)>(std::move(projection))))
            >
        {
            return Sorter::operator()(std::move(first), std::move(last),
                                      refined<decltype(*first)>(std::move(projection)));
        }

        template<mstd::forward_range Range, typename Projection>
        constexpr auto operator()(Range&& range, std::less<> compare, Projection projection) const
            -> mstd::enable_if_t<
                detail::has_comparison_projection_sort<
                    Sorter,
                    Range,
                    std::less<>,
                    refined_t<decltype(*mstd::begin(range)), Projection>
                >::value,
                decltype(Sorter::operator()(std::forward<Range>(range), compare,
                                            refined<decltype(*mstd::begin(range))>(std::move(projection))))
            >
        {
            return Sorter::operator()(std::forward<Range>(range), compare,
                                      refined<decltype(*mstd::begin(range))>(std::move(projection)));
        }

        template<mstd::forward_range Range, typename Projection>
        constexpr auto operator()(Range&& range, std::less<> compare, Projection projection) const
            -> mstd::enable_if_t<
                not detail::has_comparison_projection_sort<
                    Sorter,
                    Range,
                    std::less<>,
                    refined_t<decltype(*mstd::begin(range)), Projection>
                >::value &&
                detail::has_comparison_projection_sort_iterator<
                    Sorter,
                    mstd::iterator_t<Range>,
                    std::less<>,
                    refined_t<decltype(*mstd::begin(range)), Projection>
                >::value,
                decltype(Sorter::operator()(mstd::begin(range), mstd::end(range), compare,
                                            refined<decltype(*mstd::begin(range))>(std::move(projection))))
            >
        {
            return Sorter::operator()(mstd::begin(range), mstd::end(range), compare,
                                      refined<decltype(*mstd::begin(range))>(std::move(projection)));
        }

        template<mstd::forward_range Range, typename Projection>
        constexpr auto operator()(Range&& range, std::less<>, Projection projection) const
            -> mstd::enable_if_t<
                not detail::has_comparison_projection_sort<
                    Sorter,
                    Range,
                    std::less<>,
                    refined_t<decltype(*mstd::begin(range)), Projection>
                >::value &&
                not detail::has_comparison_projection_sort_iterator<
                    Sorter,
                    mstd::iterator_t<Range>,
                    std::less<>,
                    refined_t<decltype(*mstd::begin(range)), Projection>
                >::value &&
                detail::has_projection_sort<
                    Sorter,
                    Range,
                    refined_t<decltype(*mstd::begin(range)), Projection>
                >::value,
                decltype(Sorter::operator()(std::forward<Range>(range),
                                            refined<decltype(*mstd::begin(range))>(std::move(projection))))
            >
        {
            return Sorter::operator()(std::forward<Range>(range),
                                      refined<decltype(*mstd::begin(range))>(std::move(projection)));
        }

        template<mstd::forward_range Range, typename Projection>
        constexpr auto operator()(Range&& range, std::less<>, Projection projection) const
            -> mstd::enable_if_t<
                not detail::has_comparison_projection_sort<
                    Sorter,
                    Range,
                    std::less<>,
                    refined_t<decltype(*mstd::begin(range)), Projection>
                >::value &&
                not detail::has_comparison_projection_sort_iterator<
                    Sorter,
                    mstd::iterator_t<Range>,
                    std::less<>,
                    refined_t<decltype(*mstd::begin(range)), Projection>
                >::value &&
                not detail::has_projection_sort<
                    Sorter,
                    Range,
                    refined_t<decltype(*mstd::begin(range)), Projection>
                >::value &&
                detail::has_projection_sort_iterator<
                    Sorter,
                    mstd::iterator_t<Range>,
                    refined_t<decltype(*mstd::begin(range)), Projection>
                >::value,
                decltype(Sorter::operator()(mstd::begin(range), mstd::end(range),
                                            refined<decltype(*mstd::begin(range))>(std::move(projection))))
            >
        {
            return Sorter::operator()(mstd::begin(range), mstd::end(range),
                                      refined<decltype(*mstd::begin(range))>(std::move(projection)));
        }

        template<
            mstd::forward_iterator Iterator,
            mstd::sentinel_for<Iterator> Sentinel
        >
        constexpr auto operator()(Iterator first, Sentinel last, std::ranges::less, std::identity) const
            -> mstd::enable_if_t<
                not detail::has_comparison_projection_sort_iterator<
                    Sorter,
                    Iterator,
                    std::ranges::less,
                    std::identity
                >::value,
                decltype(Sorter::operator()(std::move(first), std::move(last)))
            >
        {
            return Sorter::operator()(std::move(first), std::move(last));
        }

        template<mstd::forward_range Range>
        constexpr auto operator()(Range&& range, std::ranges::less, std::identity) const
            -> mstd::enable_if_t<
                not detail::has_comparison_projection_sort_iterator<
                    Sorter,
                    mstd::iterator_t<Range>,
                    std::ranges::less,
                    std::identity
                >::value,
                decltype(operator()(std::forward<Range>(range)))
            >
        {
            return operator()(std::forward<Range>(range));
        }

        template<
            mstd::forward_iterator Iterator,
            mstd::sentinel_for<Iterator> Sentinel,
            typename Projection
        >
        constexpr auto operator()(Iterator first, Sentinel last, std::ranges::less compare, Projection projection) const
            -> mstd::enable_if_t<
                detail::has_comparison_projection_sort_iterator<
                    Sorter,
                    Iterator,
                    std::ranges::less,
                    refined_t<decltype(*first), Projection>
                >::value,
                decltype(Sorter::operator()(std::move(first), std::move(last), compare,
                                            refined<decltype(*first)>(std::move(projection))))
            >
        {
            return Sorter::operator()(std::move(first), std::move(last), compare,
                                      refined<decltype(*first)>(std::move(projection)));
        }

        template<
            mstd::forward_iterator Iterator,
            mstd::sentinel_for<Iterator> Sentinel,
            typename Projection
        >
        constexpr auto operator()(Iterator first, Sentinel last, std::ranges::less, Projection projection) const
            -> mstd::enable_if_t<
                not detail::has_comparison_projection_sort_iterator<
                    Sorter,
                    Iterator,
                    std::ranges::less,
                    refined_t<decltype(*first), Projection>
                >::value &&
                detail::has_projection_sort_iterator<
                    Sorter,
                    Iterator,
                    refined_t<decltype(*first), Projection>
                >::value,
                decltype(Sorter::operator()(std::move(first), std::move(last),
                                            refined<decltype(*first)>(std::move(projection))))
            >
        {
            return Sorter::operator()(std::move(first), std::move(last),
                                      refined<decltype(*first)>(std::move(projection)));
        }

        template<mstd::forward_range Range, typename Projection>
        constexpr auto operator()(Range&& range, std::ranges::less compare, Projection projection) const
            -> mstd::enable_if_t<
                detail::has_comparison_projection_sort<
                    Sorter,
                    Range,
                    std::ranges::less,
                    refined_t<decltype(*mstd::begin(range)), Projection>
                >::value,
                decltype(Sorter::operator()(std::forward<Range>(range), compare,
                                            refined<decltype(*mstd::begin(range))>(std::move(projection))))
            >
        {
            return Sorter::operator()(std::forward<Range>(range), compare,
                                      refined<decltype(*mstd::begin(range))>(std::move(projection)));
        }

        template<mstd::forward_range Range, typename Projection>
        constexpr auto operator()(Range&& range, std::ranges::less compare, Projection projection) const
            -> mstd::enable_if_t<
                not detail::has_comparison_projection_sort<
                    Sorter,
                    Range,
                    std::ranges::less,
                    refined_t<decltype(*mstd::begin(range)), Projection>
                >::value &&
                detail::has_comparison_projection_sort_iterator<
                    Sorter,
                    mstd::iterator_t<Range>,
                    std::ranges::less,
                    refined_t<decltype(*mstd::begin(range)), Projection>
                >::value,
                decltype(Sorter::operator()(mstd::begin(range), mstd::end(range), compare,
                                            refined<decltype(*mstd::begin(range))>(std::move(projection))))
            >
        {
            return Sorter::operator()(mstd::begin(range), mstd::end(range), compare,
                                      refined<decltype(*mstd::begin(range))>(std::move(projection)));
        }

        template<mstd::forward_range Range, typename Projection>
        constexpr auto operator()(Range&& range, std::ranges::less, Projection projection) const
            -> mstd::enable_if_t<
                not detail::has_comparison_projection_sort<
                    Sorter,
                    Range,
                    std::ranges::less,
                    refined_t<decltype(*mstd::begin(range)), Projection>
                >::value &&
                not detail::has_comparison_projection_sort_iterator<
                    Sorter,
                    mstd::iterator_t<Range>,
                    std::ranges::less,
                    refined_t<decltype(*mstd::begin(range)), Projection>
                >::value &&
                detail::has_projection_sort<
                    Sorter,
                    Range,
                    refined_t<decltype(*mstd::begin(range)), Projection>
                >::value,
                decltype(Sorter::operator()(std::forward<Range>(range),
                                            refined<decltype(*mstd::begin(range))>(std::move(projection))))
            >
        {
            return Sorter::operator()(std::forward<Range>(range),
                                      refined<decltype(*mstd::begin(range))>(std::move(projection)));
        }

        template<mstd::forward_range Range, typename Projection>
        constexpr auto operator()(Range&& range, std::ranges::less, Projection projection) const
            -> mstd::enable_if_t<
                not detail::has_comparison_projection_sort<
                    Sorter,
                    Range,
                    std::ranges::less,
                    refined_t<decltype(*mstd::begin(range)), Projection>
                >::value &&
                not detail::has_comparison_projection_sort_iterator<
                    Sorter,
                    mstd::iterator_t<Range>,
                    std::ranges::less,
                    refined_t<decltype(*mstd::begin(range)), Projection>
                >::value &&
                not detail::has_projection_sort<
                    Sorter,
                    Range,
                    refined_t<decltype(*mstd::begin(range)), Projection>
                >::value &&
                detail::has_projection_sort_iterator<
                    Sorter,
                    mstd::iterator_t<Range>,
                    refined_t<decltype(*mstd::begin(range)), Projection>
                >::value,
                decltype(Sorter::operator()(mstd::begin(range), mstd::end(range),
                                            refined<decltype(*mstd::begin(range))>(std::move(projection))))
            >
        {
            return Sorter::operator()(mstd::begin(range), mstd::end(range),
                                      refined<decltype(*mstd::begin(range))>(std::move(projection)));
        }

        ////////////////////////////////////////////////////////////
        // Embed projection in comparison

        template<
            mstd::forward_iterator Iterator,
            mstd::sentinel_for<Iterator> Sentinel,
            typename Projection
        >
        constexpr auto operator()(Iterator first, Sentinel last, Projection projection) const
            -> mstd::enable_if_t<
                not detail::has_projection_sort_iterator<Sorter, Iterator, Projection>::value &&
                not detail::has_comparison_projection_sort_iterator<
                    Sorter,
                    Iterator,
                    std::less<>,
                    refined_t<decltype(*first), Projection>
                >::value &&
                detail::has_comparison_sort_iterator<
                    Sorter,
                    Iterator,
                    projection_compare_t<std::less<>, refined_t<decltype(*first), Projection>>
                >::value,
                decltype(Sorter::operator()(first, last,
                                            projection_compare(std::less{},
                                                               refined<decltype(*first)>(std::move(projection)))))
            >
        {
            return Sorter::operator()(first, last,
                                      projection_compare(std::less{},
                                                         refined<decltype(*first)>(std::move(projection))));
        }

        template<
            mstd::forward_iterator Iterator,
            mstd::sentinel_for<Iterator> Sentinel,
            typename Compare,
            typename Projection
        >
        constexpr auto operator()(Iterator first, Sentinel last, Compare compare, Projection projection) const
            -> mstd::enable_if_t<
                not detail::has_comparison_projection_sort_iterator<
                    Sorter,
                    Iterator,
                    refined_t<decltype(*first), Compare>,
                    refined_t<decltype(*first), Projection>
                >::value &&
                detail::has_comparison_sort_iterator<
                    Sorter,
                    Iterator,
                    projection_compare_t<
                        refined_t<decltype(*first), Compare>,
                        refined_t<decltype(*first), Projection>
                    >
                >::value,
                decltype(Sorter::operator()(first, last, projection_compare(
                    refined<decltype(*first)>(std::move(compare)),
                    refined<decltype(*first)>(std::move(projection)))))
            >
        {
            return Sorter::operator()(first, last, projection_compare(
                refined<decltype(*first)>(std::move(compare)),
                refined<decltype(*first)>(std::move(projection))));
        }

        template<mstd::forward_range Range, typename Projection>
        constexpr auto operator()(Range&& range, Projection projection) const
            -> mstd::enable_if_t<
                not detail::has_projection_sort_iterator<
                    Sorter,
                    mstd::iterator_t<Range>,
                    refined_t<decltype(*mstd::begin(range)), Projection>
                >::value &&
                not detail::has_comparison_projection_sort_iterator<
                    Sorter,
                    mstd::iterator_t<Range>,
                    std::less<>,
                    refined_t<decltype(*mstd::begin(range)), Projection>
                >::value &&
                detail::has_comparison_sort<
                    Sorter,
                    Range,
                    projection_compare_t<
                        std::less<>,
                        refined_t<decltype(*mstd::begin(range)), Projection>
                    >
                >::value,
                decltype(Sorter::operator()(std::forward<Range>(range), projection_compare(
                    std::less{}, refined<decltype(*mstd::begin(range))>(std::move(projection)))))
            >
        {
            return Sorter::operator()(std::forward<Range>(range), projection_compare(
                std::less{}, refined<decltype(*mstd::begin(range))>(std::move(projection))));
        }

        template<mstd::forward_range Range, typename Projection>
        constexpr auto operator()(Range&& range, Projection projection) const
            -> mstd::enable_if_t<
                not detail::has_projection_sort_iterator<
                    Sorter,
                    mstd::iterator_t<Range>,
                    refined_t<decltype(*mstd::begin(range)), Projection>
                >::value &&
                not detail::has_comparison_projection_sort_iterator<
                    Sorter,
                    mstd::iterator_t<Range>,
                    std::less<>,
                    refined_t<decltype(*mstd::begin(range)), Projection>
                >::value &&
                not detail::has_comparison_sort<
                    Sorter,
                    Range,
                    projection_compare_t<
                        std::less<>,
                        refined_t<decltype(*mstd::begin(range)), Projection>
                    >
                >::value &&
                detail::has_comparison_sort_iterator<
                    Sorter,
                    mstd::iterator_t<Range>,
                    projection_compare_t<
                        std::less<>,
                        refined_t<decltype(*mstd::begin(range)), Projection>
                    >
                >::value,
                decltype(Sorter::operator()(mstd::begin(range), mstd::end(range),
                                            projection_compare(std::less{},
                                                               refined<decltype(*mstd::begin(range))>(std::move(projection)))))
            >
        {
            return Sorter::operator()(mstd::begin(range), mstd::end(range),
                                      projection_compare(std::less{},
                                                         refined<decltype(*mstd::begin(range))>(std::move(projection))));
        }

        template<mstd::forward_range Range, typename Compare, typename Projection>
        constexpr auto operator()(Range&& range, Compare compare, Projection projection) const
            -> mstd::enable_if_t<
                not detail::has_comparison_projection_sort_iterator<
                    Sorter,
                    mstd::iterator_t<Range>,
                    refined_t<decltype(*mstd::begin(range)), Compare>,
                    refined_t<decltype(*mstd::begin(range)), Projection>
                >::value &&
                detail::has_comparison_sort<
                    Sorter,
                    Range,
                    projection_compare_t<
                        refined_t<decltype(*mstd::begin(range)), Compare>,
                        refined_t<decltype(*mstd::begin(range)), Projection>
                    >
                >::value,
                decltype(Sorter::operator()(std::forward<Range>(range), projection_compare(
                    refined<decltype(*mstd::begin(range))>(std::move(compare)),
                    refined<decltype(*mstd::begin(range))>(std::move(projection)))))
            >
        {
            return Sorter::operator()(std::forward<Range>(range), projection_compare(
                refined<decltype(*mstd::begin(range))>(std::move(compare)),
                refined<decltype(*mstd::begin(range))>(std::move(projection))));
        }

        template<mstd::forward_range Range, typename Compare, typename Projection>
        constexpr auto operator()(Range&& range, Compare compare, Projection projection) const
            -> mstd::enable_if_t<
                not detail::has_comparison_projection_sort_iterator<
                    Sorter,
                    mstd::iterator_t<Range>,
                    refined_t<decltype(*mstd::begin(range)), Compare>,
                    refined_t<decltype(*mstd::begin(range)), Projection>
                >::value &&
                not detail::has_comparison_sort<
                    Sorter,
                    Range,
                    projection_compare_t<
                        refined_t<decltype(*mstd::begin(range)), Compare>,
                        refined_t<decltype(*mstd::begin(range)), Projection>
                    >
                >::value &&
                detail::has_comparison_sort_iterator<
                    Sorter,
                    mstd::iterator_t<Range>,
                    projection_compare_t<
                        refined_t<decltype(*mstd::begin(range)), Compare>,
                        refined_t<decltype(*mstd::begin(range)), Projection>
                    >
                >::value,
                decltype(Sorter::operator()(mstd::begin(range), mstd::end(range), projection_compare(
                    refined<decltype(*mstd::begin(range))>(std::move(compare)),
                    refined<decltype(*mstd::begin(range))>(std::move(projection)))))
            >
        {
            return Sorter::operator()(mstd::begin(range), mstd::end(range), projection_compare(
                refined<decltype(*mstd::begin(range))>(std::move(compare)),
                refined<decltype(*mstd::begin(range))>(std::move(projection))));
        }
    };
}

#endif // CPPSORT_SORTER_FACADE_H_
