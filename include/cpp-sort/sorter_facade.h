/*
 * Copyright (c) 2015-2021 Morwenn
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
#include <cpp-sort/refined.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/functional.h>
#include "detail/config.h"
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

            template<
                typename Ret,
                typename... Args
            >
            static constexpr auto invoke(Args... args)
                -> detail::enable_if_t<
                    not std::is_void<Ret>::value,
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
                -> detail::enable_if_t<
                    std::is_void<Ret>::value,
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

                template<typename Ret, typename Iterable>
                constexpr operator fptr_t<Ret, Iterable&>() const
                {
                    return invoker<Sorter>::template invoke<Ret, Iterable&>;
                }

                template<typename Ret, typename Iterable>
                constexpr operator fptr_t<Ret, Iterable&&>() const
                {
                    return invoker<Sorter>::template invoke<Ret, Iterable&&>;
                }

                template<typename Ret, typename Iterable, typename Func>
                constexpr operator fptr_t<Ret, Iterable&, Func>() const
                {
                    return invoker<Sorter>::template invoke<Ret, Iterable&, Func>;
                }

                template<typename Ret, typename Iterable, typename Func>
                constexpr operator fptr_t<Ret, Iterable&&, Func>() const
                {
                    return invoker<Sorter>::template invoke<Ret, Iterable&&, Func>;
                }

                template<typename Ret, typename Iterable, typename Func1, typename Func2>
                constexpr operator fptr_t<Ret, Iterable&, Func1, Func2>() const
                {
                    return invoker<Sorter>::template invoke<Ret, Iterable&, Func1, Func2>;
                }

                template<typename Ret, typename Iterable, typename Func1, typename Func2>
                constexpr operator fptr_t<Ret, Iterable&&, Func1, Func2>() const
                {
                    return invoker<Sorter>::template invoke<Ret, Iterable&&, Func1, Func2>;
                }

                template<typename Ret, typename Iterator>
                constexpr operator fptr_t<Ret, Iterator, Iterator>() const
                {
                    return invoker<Sorter>::template invoke<Ret, Iterator, Iterator>;
                }

                template<typename Ret, typename Iterator, typename Func>
                constexpr operator fptr_t<Ret, Iterator, Iterator, Func>() const
                {
                    return invoker<Sorter>::template invoke<Ret, Iterator, Iterator, Func>;
                }

                template<typename Ret, typename Iterator, typename Func1, typename Func2>
                constexpr operator fptr_t<Ret, Iterator, Iterator, Func1, Func2>() const
                {
                    return invoker<Sorter>::template invoke<Ret, Iterator, Iterator, Func1, Func2>;
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
            std::is_empty<Sorter>::value
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

        template<typename Iterator>
        constexpr auto operator()(Iterator first, Iterator last) const
            -> decltype(Sorter::operator()(std::move(first), std::move(last)))
        {
            return Sorter::operator()(std::move(first), std::move(last));
        }

        template<typename Iterable>
        constexpr auto operator()(Iterable&& iterable) const
            -> detail::enable_if_t<
                detail::has_sort<Sorter, Iterable>::value,
                decltype(Sorter::operator()(std::forward<Iterable>(iterable)))
            >
        {
            return Sorter::operator()(std::forward<Iterable>(iterable));
        }

        template<typename Iterable>
        constexpr auto operator()(Iterable&& iterable) const
            -> detail::enable_if_t<
                not detail::has_sort<Sorter, Iterable>::value,
                decltype(Sorter::operator()(std::begin(iterable), std::end(iterable)))
            >
        {
            return Sorter::operator()(std::begin(iterable), std::end(iterable));
        }

        ////////////////////////////////////////////////////////////
        // Comparison overloads

        template<typename Iterator, typename Compare>
        constexpr auto operator()(Iterator first, Iterator last, Compare compare) const
            -> detail::enable_if_t<
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

        template<typename Iterable, typename Compare>
        constexpr auto operator()(Iterable&& iterable, Compare compare) const
            -> detail::enable_if_t<
                detail::has_comparison_sort<
                    Sorter,
                    Iterable,
                    refined_t<decltype(*std::begin(iterable)), Compare>
                >::value,
                decltype(Sorter::operator()(std::forward<Iterable>(iterable),
                                            refined<decltype(*std::begin(iterable))>(std::move(compare))))
            >
        {
            return Sorter::operator()(std::forward<Iterable>(iterable),
                                      refined<decltype(*std::begin(iterable))>(std::move(compare)));
        }

        template<typename Iterable, typename Compare>
        constexpr auto operator()(Iterable&& iterable, Compare compare) const
            -> detail::enable_if_t<
                not detail::has_comparison_sort<
                    Sorter,
                    Iterable,
                    refined_t<decltype(*std::begin(iterable)), Compare>
                >::value &&
                detail::has_comparison_sort_iterator<
                    Sorter,
                    decltype(std::begin(iterable)),
                    refined_t<decltype(*std::begin(iterable)), Compare>
                >::value,
                decltype(Sorter::operator()(std::begin(iterable), std::end(iterable),
                                            refined<decltype(*std::begin(iterable))>(std::move(compare))))
            >
        {
            return Sorter::operator()(std::begin(iterable), std::end(iterable),
                                      refined<decltype(*std::begin(iterable))>(std::move(compare)));
        }

        ////////////////////////////////////////////////////////////
        // Projection overloads

        template<typename Iterator, typename Projection>
        constexpr auto operator()(Iterator first, Iterator last, Projection projection) const
            -> detail::enable_if_t<
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

        template<typename Iterator, typename Projection>
        constexpr auto operator()(Iterator first, Iterator last, Projection projection) const
            -> detail::enable_if_t<
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
                decltype(Sorter::operator()(std::move(first), std::move(last), std::less<>{},
                                            refined<decltype(*first)>(std::move(projection))))
            >
        {
            return Sorter::operator()(std::move(first), std::move(last), std::less<>{},
                                      refined<decltype(*first)>(std::move(projection)));
        }

        template<typename Iterable, typename Projection>
        constexpr auto operator()(Iterable&& iterable, Projection projection) const
            -> detail::enable_if_t<
                not detail::has_comparison_sort<
                    Sorter,
                    Iterable,
                    refined_t<decltype(*std::begin(iterable)), Projection>
                >::value &&
                detail::has_projection_sort<
                    Sorter,
                    Iterable,
                    refined_t<decltype(*std::begin(iterable)), Projection>
                >::value,
                decltype(Sorter::operator()(std::forward<Iterable>(iterable),
                                            refined<decltype(*std::begin(iterable))>(std::move(projection))))
            >
        {
            return Sorter::operator()(std::forward<Iterable>(iterable),
                                      refined<decltype(*std::begin(iterable))>(std::move(projection)));
        }

        template<typename Iterable, typename Projection>
        constexpr auto operator()(Iterable&& iterable, Projection projection) const
            -> detail::enable_if_t<
                not detail::has_comparison_sort<
                    Sorter,
                    Iterable,
                    refined_t<decltype(*std::begin(iterable)), Projection>
                >::value &&
                not detail::has_projection_sort<
                    Sorter,
                    Iterable,
                    refined_t<decltype(*std::begin(iterable)), Projection>
                >::value &&
                detail::has_comparison_projection_sort<
                    Sorter,
                    Iterable,
                    std::less<>,
                    refined_t<decltype(*std::begin(iterable)), Projection>
                >::value,
                decltype(Sorter::operator()(std::forward<Iterable>(iterable), std::less<>{},
                                            refined<decltype(*std::begin(iterable))>(std::move(projection))))
            >
        {
            return Sorter::operator()(std::forward<Iterable>(iterable), std::less<>{},
                                      refined<decltype(*std::begin(iterable))>(std::move(projection)));
        }

        template<typename Iterable, typename Projection>
        constexpr auto operator()(Iterable&& iterable, Projection projection) const
            -> detail::enable_if_t<
                not detail::has_comparison_sort<
                    Sorter,
                    Iterable,
                    refined_t<decltype(*std::begin(iterable)), Projection>
                >::value &&
                not detail::has_comparison_sort_iterator<
                    Sorter,
                    decltype(std::begin(iterable)),
                    refined_t<decltype(*std::begin(iterable)), Projection>
                >::value &&
                not detail::has_projection_sort<
                    Sorter,
                    Iterable,
                    refined_t<decltype(*std::begin(iterable)), Projection>
                >::value &&
                not detail::has_comparison_projection_sort<
                    Sorter,
                    Iterable,
                    std::less<>,
                    refined_t<decltype(*std::begin(iterable)), Projection>
                >::value &&
                detail::has_projection_sort_iterator<
                    Sorter,
                    decltype(std::begin(iterable)),
                    refined_t<decltype(*std::begin(iterable)), Projection>
                >::value,
                decltype(Sorter::operator()(std::begin(iterable), std::end(iterable),
                                            refined<decltype(*std::begin(iterable))>(std::move(projection))))
            >
        {
            return Sorter::operator()(std::begin(iterable), std::end(iterable),
                                      refined<decltype(*std::begin(iterable))>(std::move(projection)));
        }

        template<typename Iterable, typename Projection>
        constexpr auto operator()(Iterable&& iterable, Projection projection) const
            -> detail::enable_if_t<
                not detail::has_projection_sort<
                    Sorter,
                    Iterable,
                    refined_t<decltype(*std::begin(iterable)), Projection>
                >::value &&
                not detail::has_comparison_projection_sort<
                    Sorter,
                    Iterable,
                    std::less<>,
                    refined_t<decltype(*std::begin(iterable)), Projection>
                >::value &&
                not detail::has_projection_sort_iterator<
                    Sorter,
                    decltype(std::begin(iterable)),
                    refined_t<decltype(*std::begin(iterable)), Projection>
                >::value &&
                detail::has_comparison_projection_sort_iterator<
                    Sorter,
                    decltype(std::begin(iterable)),
                    std::less<>,
                    refined_t<decltype(*std::begin(iterable)), Projection>
                >::value,
                decltype(Sorter::operator()(std::begin(iterable), std::end(iterable), std::less<>{},
                                            refined<decltype(*std::begin(iterable))>(std::move(projection))))
            >
        {
            return Sorter::operator()(std::begin(iterable), std::end(iterable), std::less<>{},
                                      refined<decltype(*std::begin(iterable))>(std::move(projection)));
        }

        ////////////////////////////////////////////////////////////
        // std::less<> overloads

        template<typename Iterator>
        constexpr auto operator()(Iterator first, Iterator last, std::less<>) const
            -> detail::enable_if_t<
                not detail::has_comparison_sort_iterator<Sorter, Iterator, std::less<>>::value,
                decltype(Sorter::operator()(std::move(first), std::move(last)))
            >
        {
            return Sorter::operator()(std::move(first), std::move(last));
        }

        template<typename Iterable>
        constexpr auto operator()(Iterable&& iterable, std::less<>) const
            -> detail::enable_if_t<
                not detail::has_comparison_sort_iterator<
                    Sorter,
                    decltype(std::begin(iterable)),
                    std::less<>
                >::value,
                decltype(operator()(std::forward<Iterable>(iterable)))
            >
        {
            return operator()(std::forward<Iterable>(iterable));
        }

#ifdef __cpp_lib_ranges
        template<typename Iterator>
        constexpr auto operator()(Iterator first, Iterator last, std::ranges::less) const
            -> detail::enable_if_t<
                not detail::has_comparison_sort_iterator<Sorter, Iterator, std::ranges::less>::value,
                decltype(Sorter::operator()(std::move(first), std::move(last)))
            >
        {
            return Sorter::operator()(std::move(first), std::move(last));
        }

        template<typename Iterable>
        constexpr auto operator()(Iterable&& iterable, std::ranges::less) const
            -> detail::enable_if_t<
                not detail::has_comparison_sort_iterator<
                    Sorter,
                    decltype(std::begin(iterable)),
                    std::ranges::less
                >::value,
                decltype(operator()(std::forward<Iterable>(iterable)))
            >
        {
            return operator()(std::forward<Iterable>(iterable));
        }
#endif

        ////////////////////////////////////////////////////////////
        // utility::identity overloads

        template<typename Iterator>
        constexpr auto operator()(Iterator first, Iterator last, utility::identity) const
            -> detail::enable_if_t<
                not detail::has_projection_sort_iterator<Sorter, Iterator, utility::identity>::value &&
                not detail::has_comparison_projection_sort_iterator<
                    Sorter,
                    Iterator,
                    std::less<>,
                    utility::identity
                >::value,
                decltype(Sorter::operator()(std::move(first), std::move(last)))
            >
        {
            return Sorter::operator()(std::move(first), std::move(last));
        }

        template<typename Iterable>
        constexpr auto operator()(Iterable&& iterable, utility::identity) const
            -> detail::enable_if_t<
                not detail::has_projection_sort_iterator<
                    Sorter,
                    decltype(std::begin(iterable)),
                utility::identity
                >::value &&
                not detail::has_comparison_projection_sort_iterator<
                    Sorter,
                    decltype(std::begin(iterable)),
                    std::less<>,
                    utility::identity
                >::value,
                decltype(operator()(std::forward<Iterable>(iterable)))
            >
        {
            return operator()(std::forward<Iterable>(iterable));
        }

#if CPPSORT_STD_IDENTITY_AVAILABLE
        template<typename Iterator>
        constexpr auto operator()(Iterator first, Iterator last, std::identity) const
            -> detail::enable_if_t<
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

        template<typename Iterable>
        constexpr auto operator()(Iterable&& iterable, std::identity) const
            -> detail::enable_if_t<
                not detail::has_projection_sort_iterator<
                    Sorter,
                    decltype(std::begin(iterable)),
                std::identity
                >::value &&
                not detail::has_comparison_projection_sort_iterator<
                    Sorter,
                    decltype(std::begin(iterable)),
                    std::less<>,
                    std::identity
                >::value,
                decltype(operator()(std::forward<Iterable>(iterable)))
            >
        {
            return operator()(std::forward<Iterable>(iterable));
        }
#endif

        ////////////////////////////////////////////////////////////
        // Fused comparison-projection overloads

        template<typename Iterator, typename Compare, typename Projection>
        constexpr auto operator()(Iterator first, Iterator last, Compare compare, Projection projection) const
            -> detail::enable_if_t<
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

        template<typename Iterable, typename Compare, typename Projection>
        constexpr auto operator()(Iterable&& iterable, Compare compare, Projection projection) const
            -> detail::enable_if_t<
                detail::has_comparison_projection_sort<
                    Sorter,
                    Iterable,
                    refined_t<decltype(*std::begin(iterable)), Compare>,
                    refined_t<decltype(*std::begin(iterable)), Projection>
                >::value,
                decltype(Sorter::operator()(std::forward<Iterable>(iterable),
                                            refined<decltype(*std::begin(iterable))>(std::move(compare)),
                                            refined<decltype(*std::begin(iterable))>(std::move(projection))))
            >
        {
            return Sorter::operator()(std::forward<Iterable>(iterable),
                                      refined<decltype(*std::begin(iterable))>(std::move(compare)),
                                      refined<decltype(*std::begin(iterable))>(std::move(projection)));
        }

        template<typename Iterable, typename Compare, typename Projection>
        constexpr auto operator()(Iterable&& iterable, Compare compare, Projection projection) const
            -> detail::enable_if_t<
                not detail::has_comparison_projection_sort<
                    Sorter,
                    Iterable,
                    refined_t<decltype(*std::begin(iterable)), Compare>,
                    refined_t<decltype(*std::begin(iterable)), Projection>
                >::value &&
                detail::has_comparison_projection_sort_iterator<
                    Sorter,
                    decltype(std::begin(iterable)),
                    refined_t<decltype(*std::begin(iterable)), Compare>,
                    refined_t<decltype(*std::begin(iterable)), Projection>
                >::value,
                decltype(Sorter::operator()(std::begin(iterable), std::end(iterable),
                                            refined<decltype(*std::begin(iterable))>(compare),
                                            refined<decltype(*std::begin(iterable))>(projection)))
            >
        {
            return Sorter::operator()(std::begin(iterable), std::end(iterable),
                                      refined<decltype(*std::begin(iterable))>(std::move(compare)),
                                      refined<decltype(*std::begin(iterable))>(std::move(projection)));
        }

        ////////////////////////////////////////////////////////////
        // std::less<> and utility::identity overloads

        template<typename Iterator>
        constexpr auto operator()(Iterator first, Iterator last, std::less<>, utility::identity) const
            -> detail::enable_if_t<
                not detail::has_comparison_projection_sort_iterator<
                    Sorter,
                    Iterator,
                    std::less<>,
                    utility::identity
                >::value,
                decltype(Sorter::operator()(std::move(first), std::move(last)))
            >
        {
            return Sorter::operator()(std::move(first), std::move(last));
        }

        template<typename Iterable>
        constexpr auto operator()(Iterable&& iterable, std::less<>, utility::identity) const
            -> detail::enable_if_t<
                not detail::has_comparison_projection_sort_iterator<
                    Sorter,
                    decltype(std::begin(iterable)),
                    std::less<>,
                    utility::identity
                >::value,
                decltype(operator()(std::forward<Iterable>(iterable)))
            >
        {
            return operator()(std::forward<Iterable>(iterable));
        }

#if CPPSORT_STD_IDENTITY_AVAILABLE
        template<typename Iterator>
        constexpr auto operator()(Iterator first, Iterator last, std::less<>, std::identity) const
            -> detail::enable_if_t<
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

        template<typename Iterable>
        constexpr auto operator()(Iterable&& iterable, std::less<>, std::identity) const
            -> detail::enable_if_t<
                not detail::has_comparison_projection_sort_iterator<
                    Sorter,
                    decltype(std::begin(iterable)),
                    std::less<>,
                    std::identity
                >::value,
                decltype(operator()(std::forward<Iterable>(iterable)))
            >
        {
            return operator()(std::forward<Iterable>(iterable));
        }
#endif

        template<typename Iterator, typename Projection>
        constexpr auto operator()(Iterator first, Iterator last, std::less<> compare, Projection projection) const
            -> detail::enable_if_t<
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

        template<typename Iterator, typename Projection>
        constexpr auto operator()(Iterator first, Iterator last, std::less<>, Projection projection) const
            -> detail::enable_if_t<
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

        template<typename Iterable, typename Projection>
        constexpr auto operator()(Iterable&& iterable, std::less<> compare, Projection projection) const
            -> detail::enable_if_t<
                detail::has_comparison_projection_sort<
                    Sorter,
                    Iterable,
                    std::less<>,
                    refined_t<decltype(*std::begin(iterable)), Projection>
                >::value,
                decltype(Sorter::operator()(std::forward<Iterable>(iterable), compare,
                                            refined<decltype(*std::begin(iterable))>(std::move(projection))))
            >
        {
            return Sorter::operator()(std::forward<Iterable>(iterable), compare,
                                      refined<decltype(*std::begin(iterable))>(std::move(projection)));
        }

        template<typename Iterable, typename Projection>
        constexpr auto operator()(Iterable&& iterable, std::less<> compare, Projection projection) const
            -> detail::enable_if_t<
                not detail::has_comparison_projection_sort<
                    Sorter,
                    Iterable,
                    std::less<>,
                    refined_t<decltype(*std::begin(iterable)), Projection>
                >::value &&
                detail::has_comparison_projection_sort_iterator<
                    Sorter,
                    decltype(std::begin(iterable)),
                    std::less<>,
                    refined_t<decltype(*std::begin(iterable)), Projection>
                >::value,
                decltype(Sorter::operator()(std::begin(iterable), std::end(iterable), compare,
                                            refined<decltype(*std::begin(iterable))>(std::move(projection))))
            >
        {
            return Sorter::operator()(std::begin(iterable), std::end(iterable), compare,
                                      refined<decltype(*std::begin(iterable))>(std::move(projection)));
        }

        template<typename Iterable, typename Projection>
        constexpr auto operator()(Iterable&& iterable, std::less<>, Projection projection) const
            -> detail::enable_if_t<
                not detail::has_comparison_projection_sort<
                    Sorter,
                    Iterable,
                    std::less<>,
                    refined_t<decltype(*std::begin(iterable)), Projection>
                >::value &&
                not detail::has_comparison_projection_sort_iterator<
                    Sorter,
                    decltype(std::begin(iterable)),
                    std::less<>,
                    refined_t<decltype(*std::begin(iterable)), Projection>
                >::value &&
                detail::has_projection_sort<
                    Sorter,
                    Iterable,
                    refined_t<decltype(*std::begin(iterable)), Projection>
                >::value,
                decltype(Sorter::operator()(std::forward<Iterable>(iterable),
                                            refined<decltype(*std::begin(iterable))>(std::move(projection))))
            >
        {
            return Sorter::operator()(std::forward<Iterable>(iterable),
                                      refined<decltype(*std::begin(iterable))>(std::move(projection)));
        }

        template<typename Iterable, typename Projection>
        constexpr auto operator()(Iterable&& iterable, std::less<>, Projection projection) const
            -> detail::enable_if_t<
                not detail::has_comparison_projection_sort<
                    Sorter,
                    Iterable,
                    std::less<>,
                    refined_t<decltype(*std::begin(iterable)), Projection>
                >::value &&
                not detail::has_comparison_projection_sort_iterator<
                    Sorter,
                    decltype(std::begin(iterable)),
                    std::less<>,
                    refined_t<decltype(*std::begin(iterable)), Projection>
                >::value &&
                not detail::has_projection_sort<
                    Sorter,
                    Iterable,
                    refined_t<decltype(*std::begin(iterable)), Projection>
                >::value &&
                detail::has_projection_sort_iterator<
                    Sorter,
                    decltype(std::begin(iterable)),
                    refined_t<decltype(*std::begin(iterable)), Projection>
                >::value,
                decltype(Sorter::operator()(std::begin(iterable), std::end(iterable),
                                            refined<decltype(*std::begin(iterable))>(std::move(projection))))
            >
        {
            return Sorter::operator()(std::begin(iterable), std::end(iterable),
                                      refined<decltype(*std::begin(iterable))>(std::move(projection)));
        }

#ifdef __cpp_lib_ranges
        template<typename Iterator>
        constexpr auto operator()(Iterator first, Iterator last, std::ranges::less, std::identity) const
            -> detail::enable_if_t<
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

        template<typename Iterable>
        constexpr auto operator()(Iterable&& iterable, std::ranges::less, std::identity) const
            -> detail::enable_if_t<
                not detail::has_comparison_projection_sort_iterator<
                    Sorter,
                    decltype(std::begin(iterable)),
                    std::ranges::less,
                    std::identity
                >::value,
                decltype(operator()(std::forward<Iterable>(iterable)))
            >
        {
            return operator()(std::forward<Iterable>(iterable));
        }

        template<typename Iterator, typename Projection>
        constexpr auto operator()(Iterator first, Iterator last, std::ranges::less compare, Projection projection) const
            -> detail::enable_if_t<
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

        template<typename Iterator, typename Projection>
        constexpr auto operator()(Iterator first, Iterator last, std::ranges::less, Projection projection) const
            -> detail::enable_if_t<
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

        template<typename Iterable, typename Projection>
        constexpr auto operator()(Iterable&& iterable, std::ranges::less compare, Projection projection) const
            -> detail::enable_if_t<
                detail::has_comparison_projection_sort<
                    Sorter,
                    Iterable,
                    std::ranges::less,
                    refined_t<decltype(*std::begin(iterable)), Projection>
                >::value,
                decltype(Sorter::operator()(std::forward<Iterable>(iterable), compare,
                                            refined<decltype(*std::begin(iterable))>(std::move(projection))))
            >
        {
            return Sorter::operator()(std::forward<Iterable>(iterable), compare,
                                      refined<decltype(*std::begin(iterable))>(std::move(projection)));
        }

        template<typename Iterable, typename Projection>
        constexpr auto operator()(Iterable&& iterable, std::ranges::less compare, Projection projection) const
            -> detail::enable_if_t<
                not detail::has_comparison_projection_sort<
                    Sorter,
                    Iterable,
                    std::ranges::less,
                    refined_t<decltype(*std::begin(iterable)), Projection>
                >::value &&
                detail::has_comparison_projection_sort_iterator<
                    Sorter,
                    decltype(std::begin(iterable)),
                    std::ranges::less,
                    refined_t<decltype(*std::begin(iterable)), Projection>
                >::value,
                decltype(Sorter::operator()(std::begin(iterable), std::end(iterable), compare,
                                            refined<decltype(*std::begin(iterable))>(std::move(projection))))
            >
        {
            return Sorter::operator()(std::begin(iterable), std::end(iterable), compare,
                                      refined<decltype(*std::begin(iterable))>(std::move(projection)));
        }

        template<typename Iterable, typename Projection>
        constexpr auto operator()(Iterable&& iterable, std::ranges::less, Projection projection) const
            -> detail::enable_if_t<
                not detail::has_comparison_projection_sort<
                    Sorter,
                    Iterable,
                    std::ranges::less,
                    refined_t<decltype(*std::begin(iterable)), Projection>
                >::value &&
                not detail::has_comparison_projection_sort_iterator<
                    Sorter,
                    decltype(std::begin(iterable)),
                    std::ranges::less,
                    refined_t<decltype(*std::begin(iterable)), Projection>
                >::value &&
                detail::has_projection_sort<
                    Sorter,
                    Iterable,
                    refined_t<decltype(*std::begin(iterable)), Projection>
                >::value,
                decltype(Sorter::operator()(std::forward<Iterable>(iterable),
                                            refined<decltype(*std::begin(iterable))>(std::move(projection))))
            >
        {
            return Sorter::operator()(std::forward<Iterable>(iterable),
                                      refined<decltype(*std::begin(iterable))>(std::move(projection)));
        }

        template<typename Iterable, typename Projection>
        constexpr auto operator()(Iterable&& iterable, std::ranges::less, Projection projection) const
            -> detail::enable_if_t<
                not detail::has_comparison_projection_sort<
                    Sorter,
                    Iterable,
                    std::ranges::less,
                    refined_t<decltype(*std::begin(iterable)), Projection>
                >::value &&
                not detail::has_comparison_projection_sort_iterator<
                    Sorter,
                    decltype(std::begin(iterable)),
                    std::ranges::less,
                    refined_t<decltype(*std::begin(iterable)), Projection>
                >::value &&
                not detail::has_projection_sort<
                    Sorter,
                    Iterable,
                    refined_t<decltype(*std::begin(iterable)), Projection>
                >::value &&
                detail::has_projection_sort_iterator<
                    Sorter,
                    decltype(std::begin(iterable)),
                    refined_t<decltype(*std::begin(iterable)), Projection>
                >::value,
                decltype(Sorter::operator()(std::begin(iterable), std::end(iterable),
                                            refined<decltype(*std::begin(iterable))>(std::move(projection))))
            >
        {
            return Sorter::operator()(std::begin(iterable), std::end(iterable),
                                      refined<decltype(*std::begin(iterable))>(std::move(projection)));
        }
#endif

        ////////////////////////////////////////////////////////////
        // Embed projection in comparison

        template<typename Iterator, typename Projection>
        constexpr auto operator()(Iterator first, Iterator last, Projection projection) const
            -> detail::enable_if_t<
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
                    projection_compare<std::less<>, refined_t<decltype(*first), Projection>>
                >::value,
                decltype(Sorter::operator()(first, last,
                                            make_projection_compare(std::less<>{},
                                                                    refined<decltype(*first)>(std::move(projection)))))
            >
        {
            return Sorter::operator()(first, last,
                                      make_projection_compare(std::less<>{},
                                                              refined<decltype(*first)>(std::move(projection))));
        }

        template<typename Iterator, typename Compare, typename Projection>
        constexpr auto operator()(Iterator first, Iterator last, Compare compare, Projection projection) const
            -> detail::enable_if_t<
                not detail::has_comparison_projection_sort_iterator<
                    Sorter,
                    Iterator,
                    refined_t<decltype(*first), Compare>,
                    refined_t<decltype(*first), Projection>
                >::value &&
                detail::has_comparison_sort_iterator<
                    Sorter,
                    Iterator,
                    projection_compare<
                        refined_t<decltype(*first), Compare>,
                        refined_t<decltype(*first), Projection>
                    >
                >::value,
                decltype(Sorter::operator()(first, last, make_projection_compare(
                    refined<decltype(*first)>(std::move(compare)),
                    refined<decltype(*first)>(std::move(projection)))))
            >
        {
            return Sorter::operator()(first, last, make_projection_compare(
                refined<decltype(*first)>(std::move(compare)),
                refined<decltype(*first)>(std::move(projection))));
        }

        template<typename Iterable, typename Projection>
        constexpr auto operator()(Iterable&& iterable, Projection projection) const
            -> detail::enable_if_t<
                not detail::has_projection_sort_iterator<
                    Sorter,
                    decltype(std::begin(iterable)),
                    refined_t<decltype(*std::begin(iterable)), Projection>
                >::value &&
                not detail::has_comparison_projection_sort_iterator<
                    Sorter,
                    decltype(std::begin(iterable)),
                    std::less<>,
                    refined_t<decltype(*std::begin(iterable)), Projection>
                >::value &&
                detail::has_comparison_sort<
                    Sorter,
                    Iterable,
                    projection_compare<
                        std::less<>,
                        refined_t<decltype(*std::begin(iterable)), Projection>
                    >
                >::value,
                decltype(Sorter::operator()(std::forward<Iterable>(iterable), make_projection_compare(
                    std::less<>{}, refined<decltype(*std::begin(iterable))>(std::move(projection)))))
            >
        {
            return Sorter::operator()(std::forward<Iterable>(iterable), make_projection_compare(
                std::less<>{}, refined<decltype(*std::begin(iterable))>(std::move(projection))));
        }

        template<typename Iterable, typename Projection>
        constexpr auto operator()(Iterable&& iterable, Projection projection) const
            -> detail::enable_if_t<
                not detail::has_projection_sort_iterator<
                    Sorter,
                    decltype(std::begin(iterable)),
                    refined_t<decltype(*std::begin(iterable)), Projection>
                >::value &&
                not detail::has_comparison_projection_sort_iterator<
                    Sorter,
                    decltype(std::begin(iterable)),
                    std::less<>,
                    refined_t<decltype(*std::begin(iterable)), Projection>
                >::value &&
                not detail::has_comparison_sort<
                    Sorter,
                    Iterable,
                    projection_compare<
                        std::less<>,
                        refined_t<decltype(*std::begin(iterable)), Projection>
                    >
                >::value &&
                detail::has_comparison_sort_iterator<
                    Sorter,
                    decltype(std::begin(iterable)),
                    projection_compare<
                        std::less<>,
                        refined_t<decltype(*std::begin(iterable)), Projection>
                    >
                >::value,
                decltype(Sorter::operator()(std::begin(iterable), std::end(iterable),
                                            make_projection_compare(std::less<>{},
                                                                    refined<decltype(*std::begin(iterable))>(std::move(projection)))))
            >
        {
            return Sorter::operator()(std::begin(iterable), std::end(iterable),
                                      make_projection_compare(std::less<>{},
                                                              refined<decltype(*std::begin(iterable))>(std::move(projection))));
        }

        template<typename Iterable, typename Compare, typename Projection>
        constexpr auto operator()(Iterable&& iterable, Compare compare, Projection projection) const
            -> detail::enable_if_t<
                not detail::has_comparison_projection_sort_iterator<
                    Sorter,
                    decltype(std::begin(iterable)),
                    refined_t<decltype(*std::begin(iterable)), Compare>,
                    refined_t<decltype(*std::begin(iterable)), Projection>
                >::value &&
                detail::has_comparison_sort<
                    Sorter,
                    Iterable,
                    projection_compare<
                        refined_t<decltype(*std::begin(iterable)), Compare>,
                        refined_t<decltype(*std::begin(iterable)), Projection>
                    >
                >::value,
                decltype(Sorter::operator()(std::forward<Iterable>(iterable), make_projection_compare(
                    refined<decltype(*std::begin(iterable))>(std::move(compare)),
                    refined<decltype(*std::begin(iterable))>(std::move(projection)))))
            >
        {
            return Sorter::operator()(std::forward<Iterable>(iterable), make_projection_compare(
                refined<decltype(*std::begin(iterable))>(std::move(compare)),
                refined<decltype(*std::begin(iterable))>(std::move(projection))));
        }

        template<typename Iterable, typename Compare, typename Projection>
        constexpr auto operator()(Iterable&& iterable, Compare compare, Projection projection) const
            -> detail::enable_if_t<
                not detail::has_comparison_projection_sort_iterator<
                    Sorter,
                    decltype(std::begin(iterable)),
                    refined_t<decltype(*std::begin(iterable)), Compare>,
                    refined_t<decltype(*std::begin(iterable)), Projection>
                >::value &&
                not detail::has_comparison_sort<
                    Sorter,
                    Iterable,
                    projection_compare<
                        refined_t<decltype(*std::begin(iterable)), Compare>,
                        refined_t<decltype(*std::begin(iterable)), Projection>
                    >
                >::value &&
                detail::has_comparison_sort_iterator<
                    Sorter,
                    decltype(std::begin(iterable)),
                    projection_compare<
                        refined_t<decltype(*std::begin(iterable)), Compare>,
                        refined_t<decltype(*std::begin(iterable)), Projection>
                    >
                >::value,
                decltype(Sorter::operator()(std::begin(iterable), std::end(iterable), make_projection_compare(
                    refined<decltype(*std::begin(iterable))>(std::move(compare)),
                    refined<decltype(*std::begin(iterable))>(std::move(projection)))))
            >
        {
            return Sorter::operator()(std::begin(iterable), std::end(iterable), make_projection_compare(
                refined<decltype(*std::begin(iterable))>(std::move(compare)),
                refined<decltype(*std::begin(iterable))>(std::move(projection))));
        }
    };
}

#endif // CPPSORT_SORTER_FACADE_H_
