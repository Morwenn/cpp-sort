/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Morwenn
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef CPPSORT_ADAPTERS_CONTAINER_AWARE_ADAPTER_H_
#define CPPSORT_ADAPTERS_CONTAINER_AWARE_ADAPTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <type_traits>
#include <cpp-sort/sort.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/is_callable.h>
#include <cpp-sort/utility/logical_traits.h>
#include "../detail/projection_compare.h"

namespace cppsort
{
    namespace detail
    {
        // Hide the generic cppsort::sort
        struct nope_type {};
        template<typename... Args>
        auto sort(Args&&...)
            -> nope_type;

        struct adl_despair
        {
            template<typename Sorter, typename Iterable>
            auto operator()(Sorter sorter, Iterable& iterable) const
                -> decltype(sort(sorter, iterable))
            {
                return sort(sorter, iterable);
            }

            template<typename Sorter, typename Iterable, typename Function>
            auto operator()(Sorter sorter, Iterable& iterable, Function function) const
                -> decltype(sort(sorter, iterable, function))
            {
                return sort(sorter, iterable, function);
            }

            template<typename Sorter, typename Iterable, typename Compare, typename Projection>
            auto operator()(Sorter sorter, Iterable& iterable, Compare compare, Projection projection) const
                -> decltype(sort(sorter, iterable, compare, projection))
            {
                return sort(sorter, iterable, compare, projection);
            }
        };

        template<typename Sorter, typename Iterable>
        struct can_sort:
            utility::conjunction<
                utility::is_callable<adl_despair(Sorter, Iterable&)>,
                utility::negation<utility::is_callable<adl_despair(Sorter, Iterable&), nope_type>>
            >
        {};

        template<typename Sorter, typename Iterable, typename Compare>
        struct can_comparison_sort:
            utility::conjunction<
                utility::is_callable<adl_despair(Sorter, Iterable&, Compare)>,
                utility::negation<utility::is_callable<adl_despair(Sorter, Iterable&, Compare), nope_type>>,
                is_projection<utility::identity, Iterable, Compare>
            >
        {};

        template<typename Sorter, typename Iterable, typename Projection>
        struct can_projection_sort:
            utility::conjunction<
                utility::is_callable<adl_despair(Sorter, Iterable&, Projection)>,
                utility::negation<utility::is_callable<adl_despair(Sorter, Iterable&, Projection), nope_type>>,
                is_projection<Projection, Iterable>
            >
        {};

        template<typename Sorter, typename Iterable, typename Compare, typename Projection>
        struct can_comparison_projection_sort:
            utility::conjunction<
                utility::is_callable<adl_despair(Sorter, Iterable&, Compare, Projection)>,
                utility::negation<utility::is_callable<adl_despair(Sorter, Iterable&, Compare, Projection), nope_type>>,
                is_projection<Projection, Iterable, Compare>
            >
        {};

        template<typename Sorter>
        struct container_aware_adapter_base
        {
            template<typename Iterable>
            auto operator()(Iterable& iterable) const
                -> std::enable_if_t<
                    detail::can_sort<Sorter, Iterable>::value,
                    decltype(detail::adl_despair{}(Sorter{}, iterable))
                >
            {
                return detail::adl_despair{}(Sorter{}, iterable);
            }

            template<typename Iterable>
            auto operator()(Iterable& iterable) const
                -> std::enable_if_t<
                    not detail::can_sort<Sorter, Iterable>::value,
                    decltype(cppsort::sort(Sorter{}, iterable))
                >
            {
                return cppsort::sort(Sorter{}, iterable);
            }

            template<typename Iterable, typename Compare>
            auto operator()(Iterable& iterable, Compare compare) const
                -> std::enable_if_t<
                    detail::can_comparison_sort<Sorter, Iterable, Compare>::value,
                    decltype(detail::adl_despair{}(Sorter{}, iterable, compare))
                >
            {
                return detail::adl_despair{}(Sorter{}, iterable, compare);
            }

            template<typename Iterable, typename Compare>
            auto operator()(Iterable& iterable, Compare compare) const
                -> std::enable_if_t<
                    not is_projection<Compare, Iterable>::value &&
                    not detail::can_comparison_sort<Sorter, Iterable, Compare>::value,
                    decltype(cppsort::sort(Sorter{}, iterable, compare))
                >
            {
                return cppsort::sort(Sorter{}, iterable, compare);
            }

            template<typename Iterable, typename Projection>
            auto operator()(Iterable& iterable, Projection projection) const
                -> std::enable_if_t<
                    detail::can_projection_sort<Sorter, Iterable, Projection>::value,
                    decltype(detail::adl_despair{}(Sorter{}, iterable, projection))
                >
            {
                return detail::adl_despair{}(Sorter{}, iterable, projection);
            }

            template<typename Iterable, typename Projection>
            auto operator()(Iterable& iterable, Projection projection) const
                -> std::enable_if_t<
                    not detail::can_projection_sort<Sorter, Iterable, Projection>::value &&
                        detail::can_comparison_projection_sort<Sorter, Iterable, std::less<>, Projection>::value,
                    decltype(detail::adl_despair{}(Sorter{}, iterable, std::less<>{}, projection))
                >
            {
                return detail::adl_despair{}(Sorter{}, iterable, std::less<>{}, projection);
            }

            template<typename Iterable, typename Projection>
            auto operator()(Iterable& iterable, Projection projection) const
                -> std::enable_if_t<
                    not detail::can_projection_sort<Sorter, Iterable, Projection>::value &&
                    not detail::can_comparison_projection_sort<Sorter, Iterable, std::less<>, Projection>::value &&
                    detail::can_comparison_sort<
                        Sorter,
                        Iterable,
                        detail::projection_compare<std::less<>, Projection>
                    >::value,
                    decltype(detail::adl_despair{}(Sorter{}, iterable, detail::make_projection_compare(std::less<>{}, projection)))
                >
            {
                return detail::adl_despair{}(Sorter{}, iterable, detail::make_projection_compare(std::less<>{}, projection));
            }

            template<typename Iterable, typename Projection>
            auto operator()(Iterable& iterable, Projection projection) const
                -> std::enable_if_t<
                    is_projection<Projection, Iterable>::value &&
                    not detail::can_projection_sort<Sorter, Iterable, Projection>::value &&
                    not detail::can_comparison_projection_sort<Sorter, Iterable, std::less<>, Projection>::value &&
                    not detail::can_comparison_sort<
                        Sorter,
                        Iterable,
                        detail::projection_compare<std::less<>, Projection>
                    >::value,
                    decltype(cppsort::sort(Sorter{}, iterable, projection))
                >
            {
                return cppsort::sort(Sorter{}, iterable, projection);
            }

            template<typename Iterable, typename Compare, typename Projection>
            auto operator()(Iterable& iterable, Compare compare, Projection projection) const
                -> std::enable_if_t<
                    detail::can_comparison_projection_sort<Sorter, Iterable, Compare, Projection>::value,
                    decltype(detail::adl_despair{}(Sorter{}, iterable, compare, projection))
                >
            {
                return detail::adl_despair{}(Sorter{}, iterable, compare, projection);
            }

            template<typename Iterable, typename Compare, typename Projection>
            auto operator()(Iterable& iterable, Compare compare, Projection projection) const
                -> std::enable_if_t<
                    not detail::can_comparison_projection_sort<Sorter, Iterable, Compare, Projection>::value &&
                    detail::can_comparison_sort<
                        Sorter,
                        Iterable,
                        detail::projection_compare<Compare, Projection>
                    >::value,
                    decltype(detail::adl_despair{}(Sorter{}, iterable, detail::make_projection_compare(compare, projection)))
                >
            {
                return detail::adl_despair{}(Sorter{}, iterable, detail::make_projection_compare(compare, projection));
            }

            template<typename Iterable, typename Compare, typename Projection>
            auto operator()(Iterable& iterable, Compare compare, Projection projection) const
                -> std::enable_if_t<
                    not detail::can_comparison_projection_sort<Sorter, Iterable, Compare, Projection>::value &&
                    not detail::can_comparison_sort<
                        Sorter,
                        Iterable,
                        detail::projection_compare<Compare, Projection>
                    >::value,
                    decltype(cppsort::sort(Sorter{}, iterable, compare, projection))
                >
            {
                return cppsort::sort(Sorter{}, iterable, compare, projection);
            }
        };
    }

    template<typename Sorter>
    struct container_aware_adapter:
        detail::container_aware_adapter_base<Sorter>
    {};
}

#ifdef CPPSORT_SORTERS_INSERTION_SORTER_H_
#include "../detail/container_aware/insertion_sort.h"
#endif

#ifdef CPPSORT_SORTERS_SELECTION_SORTER_H_
#include "../detail/container_aware/selection_sort.h"
#endif

#endif // CPPSORT_ADAPTERS_CONTAINER_AWARE_ADAPTER_H_
