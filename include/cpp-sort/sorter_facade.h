/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Morwenn
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
#ifndef CPPSORT_SORTER_FACADE_H_
#define CPPSORT_SORTER_FACADE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <type_traits>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/detection.h>
#include <cpp-sort/utility/identity.h>
#include "detail/as_function.h"

namespace cppsort
{
    // This class takes an incomplete sorter, analyses it and creates
    // all the methods needed to complete it: additional overloads to
    // operator() and conversions to function pointers

    template<typename Sorter>
    class sorter_facade:
        public Sorter
    {
        protected:

            // Function pointer types, a type alias is required
            // for the function pointer conversion operator syntax
            // to be valid

            using this_class = sorter_facade<Sorter>;

            template<typename Iterable>
            using fptr_t = std::result_of_t<this_class(Iterable&)>(*)(Iterable&);

            template<typename Iterable, typename Func>
            using fptr_func_t = std::result_of_t<this_class(Iterable&, Func)>(*)(Iterable&, Func);

            template<typename Iterable, typename Func1, typename Func2>
            using fptr_func2_t
                = std::result_of_t<this_class(Iterable&, Func1, Func2)>(*)(Iterable&, Func1, Func2);

            template<typename Iterator>
            using fptr_it_t = std::result_of_t<this_class(Iterator, Iterator)>(*)(Iterator, Iterator);

            template<typename Iterator, typename Func>
            using fptr_func_it_t
                = std::result_of_t<this_class(Iterator, Iterator, Func)>(*)(Iterator, Iterator, Func);

            template<typename Iterator, typename Func1, typename Func2>
            using fptr_func2_it_t
                = std::result_of_t<this_class(Iterator, Iterator, Func1, Func2)>(*)(Iterator, Iterator, Func1, Func2);

        public:

            ////////////////////////////////////////////////////////////
            // Conversion to function pointers

            template<typename Iterable>
            operator fptr_t<Iterable>() const
            {
                return [](Iterable& iterable) {
                    return this_class{}(iterable);
                };
            }

            template<typename Iterable, typename Func>
            operator fptr_func_t<Iterable, Func>() const
            {
                return [](Iterable& iterable, Func func) {
                    return this_class{}(iterable, func);
                };
            }

            template<typename Iterable, typename Func1, typename Func2>
            operator fptr_func2_t<Iterable, Func1, Func2>() const
            {
                return [](Iterable& iterable, Func1 func1, Func2 func2) {
                    return this_class{}(iterable, func1, func2);
                };
            }

            template<typename Iterator>
            operator fptr_it_t<Iterator>() const
            {
                return [](Iterator first, Iterator last) {
                    return this_class{}(first, last);
                };
            }

            template<typename Iterator, typename Func>
            operator fptr_func_it_t<Iterator, Func>() const
            {
                return [](Iterator first, Iterator last, Func func) {
                    return this_class{}(first, last, func);
                };
            }

            template<typename Iterator, typename Func1, typename Func2>
            operator fptr_func2_it_t<Iterator, Func1, Func2>() const
            {
                return [](Iterator first, Iterator last, Func1 func1, Func2 func2) {
                    return this_class{}(first, last, func1, func2);
                };
            }

            ////////////////////////////////////////////////////////////
            // Non-comparison overloads

            template<typename Iterator>
            auto operator()(Iterator first, Iterator last) const
                -> decltype(Sorter::operator()(first, last))
            {
                return Sorter::operator()(first, last);
            }

            template<typename Iterable>
            auto operator()(Iterable& iterable) const
                -> std::enable_if_t<
                    detail::has_sort<Sorter, Iterable>,
                    decltype(Sorter::operator()(iterable))
                >
            {
                return Sorter::operator()(iterable);
            }

            template<typename Iterable>
            auto operator()(Iterable& iterable) const
                -> std::enable_if_t<
                    not detail::has_sort<Sorter, Iterable>,
                    decltype(Sorter::operator()(std::begin(iterable), std::end(iterable)))
                >
            {
                return Sorter::operator()(std::begin(iterable), std::end(iterable));
            }

            ////////////////////////////////////////////////////////////
            // Comparison overloads

            template<typename Iterator, typename Compare>
            auto operator()(Iterator first, Iterator last, Compare compare) const
                -> std::enable_if_t<
                    detail::has_comparison_sort_iterator<Sorter, Iterator, Compare>,
                    decltype(Sorter::operator()(first, last, compare))
                >
            {
                return Sorter::operator()(first, last, compare);
            }

            template<typename Iterable, typename Compare>
            auto operator()(Iterable& iterable, Compare compare) const
                -> std::enable_if_t<
                    detail::has_comparison_sort<Sorter, Iterable, Compare>,
                    decltype(Sorter::operator()(iterable, compare))
                >
            {
                return Sorter::operator()(iterable, compare);
            }

            template<typename Iterable, typename Compare>
            auto operator()(Iterable& iterable, Compare compare) const
                -> std::enable_if_t<
                    not detail::has_comparison_sort<Sorter, Iterable, Compare>
                    &&  detail::has_comparison_sort_iterator<Sorter, decltype(std::begin(iterable)), Compare>,
                    decltype(Sorter::operator()(std::begin(iterable), std::end(iterable), compare))
                >
            {
                return Sorter::operator()(std::begin(iterable), std::end(iterable), compare);
            }

            ////////////////////////////////////////////////////////////
            // std::less<> overloads

            template<typename Iterator>
            auto operator()(Iterator first, Iterator last, std::less<>) const
                -> std::enable_if_t<
                    not detail::has_comparison_sort_iterator<Sorter, Iterator, std::less<>>,
                    decltype(Sorter::operator()(first, last))
                >
            {
                return Sorter::operator()(first, last);
            }

            template<typename Iterable>
            auto operator()(Iterable& iterable, std::less<>) const
                -> std::enable_if_t<
                    not detail::has_comparison_sort_iterator<
                        Sorter,
                        decltype(std::begin(iterable)),
                        std::less<>
                    >,
                    decltype(operator()(iterable))
                >
            {
                return operator()(iterable);
            }

            ////////////////////////////////////////////////////////////
            // Projection overloads

            template<typename Iterator, typename Projection>
            auto operator()(Iterator first, Iterator last, Projection projection) const
                -> std::enable_if_t<
                    detail::has_projection_sort_iterator<Sorter, Iterator, Projection>,
                    decltype(Sorter::operator()(first, last, projection))
                >
            {
                return Sorter::operator()(first, last, projection);
            }

            template<typename Iterator, typename Projection>
            auto operator()(Iterator first, Iterator last, Projection projection) const
                -> std::enable_if_t<
                    not detail::has_projection_sort_iterator<Sorter, Iterator, Projection>
                    &&  detail::has_comparison_projection_sort_iterator<Sorter, Iterator, std::less<>, Projection>,
                    decltype(Sorter::operator()(first, last, std::less<>{}, projection))
                >
            {
                return Sorter::operator()(first, last, std::less<>{}, projection);
            }

            template<typename Iterable, typename Projection>
            auto operator()(Iterable& iterable, Projection projection) const
                -> std::enable_if_t<
                    detail::has_projection_sort<Sorter, Iterable, Projection>,
                    decltype(Sorter::operator()(iterable, projection))
                >
            {
                return Sorter::operator()(iterable, projection);
            }

            template<typename Iterable, typename Projection>
            auto operator()(Iterable& iterable, Projection projection) const
                -> std::enable_if_t<
                    not detail::has_projection_sort<Sorter, Iterable, Projection> &&
                        detail::has_comparison_projection_sort<Sorter, Iterable, std::less<>, Projection>,
                    decltype(Sorter::operator()(iterable, std::less<>{}, projection))
                >
            {
                return Sorter::operator()(iterable, std::less<>{}, projection);
            }

            template<typename Iterable, typename Projection>
            auto operator()(Iterable& iterable, Projection projection) const
                -> std::enable_if_t<
                    not detail::has_projection_sort<Sorter, Iterable, Projection> &&
                    not detail::has_comparison_projection_sort<Sorter, Iterable, std::less<>, Projection> &&
                        detail::has_projection_sort_iterator<Sorter, decltype(std::begin(iterable)), Projection>,
                    decltype(Sorter::operator()(std::begin(iterable), std::end(iterable), projection))
                >
            {
                return Sorter::operator()(std::begin(iterable), std::end(iterable), projection);
            }

            template<typename Iterable, typename Projection>
            auto operator()(Iterable& iterable, Projection projection) const
                -> std::enable_if_t<
                    not detail::has_projection_sort<Sorter, Iterable, Projection> &&
                    not detail::has_comparison_projection_sort<Sorter, Iterable, std::less<>, Projection> &&
                    not detail::has_projection_sort_iterator<Sorter, decltype(std::begin(iterable)), Projection> &&
                        detail::has_comparison_projection_sort_iterator<
                        Sorter,
                        decltype(std::begin(iterable)),
                        std::less<>,
                        Projection
                    >,
                    decltype(Sorter::operator()(std::begin(iterable), std::end(iterable), std::less<>{}, projection))
                >
            {
                return Sorter::operator()(std::begin(iterable), std::end(iterable), std::less<>{}, projection);
            }

            ////////////////////////////////////////////////////////////
            // utility::identity overloads

            template<typename Iterator>
            auto operator()(Iterator first, Iterator last, utility::identity) const
                -> std::enable_if_t<
                    not detail::has_projection_sort_iterator<Sorter, Iterator, utility::identity> &&
                    not detail::has_comparison_projection_sort_iterator<
                        Sorter,
                        Iterator,
                        std::less<>,
                        utility::identity
                    >,
                    decltype(Sorter::operator()(first, last))
                >
            {
                return Sorter::operator()(first, last);
            }

            template<typename Iterable>
            auto operator()(Iterable& iterable, utility::identity) const
                -> std::enable_if_t<
                    not detail::has_projection_sort_iterator<
                        Sorter,
                        decltype(std::begin(iterable)),
                    utility::identity
                    > &&
                    not detail::has_comparison_projection_sort_iterator<
                        Sorter,
                        decltype(std::begin(iterable)),
                        std::less<>,
                        utility::identity
                    >,
                    decltype(operator()(iterable))
                >
            {
                return operator()(iterable);
            }

            ////////////////////////////////////////////////////////////
            // Fused comparison-projection overloads

            template<typename Iterator, typename Compare, typename Projection>
            auto operator()(Iterator first, Iterator last,
                            Compare compare, Projection projection) const
                -> decltype(Sorter::operator()(first, last, compare, projection))
            {
                return Sorter::operator()(first, last, compare, projection);
            }

            template<typename Iterable, typename Compare, typename Projection>
            auto operator()(Iterable& iterable, Compare compare, Projection projection) const
                -> std::enable_if_t<
                    detail::has_comparison_projection_sort<Sorter, Iterable, Compare, Projection>,
                    decltype(Sorter::operator()(iterable, compare, projection))
                >
            {
                return Sorter::operator()(iterable, compare, projection);
            }

            ////////////////////////////////////////////////////////////
            // std::less<> and utility::identity overloads

            template<typename Iterator>
            auto operator()(Iterator first, Iterator last, std::less<>, utility::identity) const
                -> std::enable_if_t<
                    not detail::has_comparison_projection_sort_iterator<
                        Sorter,
                        Iterator,
                        std::less<>,
                        utility::identity
                    >,
                    decltype(Sorter::operator()(first, last))
                >
            {
                return Sorter::operator()(first, last);
            }

            template<typename Iterable>
            auto operator()(Iterable& iterable, std::less<>, utility::identity) const
                -> std::enable_if_t<
                    not detail::has_comparison_projection_sort_iterator<
                        Sorter,
                        decltype(std::begin(iterable)),
                        std::less<>,
                        utility::identity
                    >,
                    decltype(operator()(iterable))
                >
            {
                return operator()(iterable);
            }
    };
}

#endif // CPPSORT_SORTER_FACADE_H_
