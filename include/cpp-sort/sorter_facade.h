/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2016 Morwenn
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
#include <utility>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/begin_end.h>
#include <cpp-sort/utility/functional.h>
#include "detail/projection_compare.h"

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

            template<typename Iterable>
            using fptr_rvalue_t = std::result_of_t<this_class(Iterable&&)>(*)(Iterable&&);

            template<typename Iterable, typename Func>
            using fptr_func_t = std::result_of_t<this_class(Iterable&, Func)>(*)(Iterable&, Func);

            template<typename Iterable, typename Func>
            using fptr_rvalue_func_t = std::result_of_t<this_class(Iterable&&, Func)>(*)(Iterable&&, Func);

            template<typename Iterable, typename Func1, typename Func2>
            using fptr_func2_t
                = std::result_of_t<this_class(Iterable&, Func1, Func2)>(*)(Iterable&, Func1, Func2);

            template<typename Iterable, typename Func1, typename Func2>
            using fptr_rvalue_func2_t
                = std::result_of_t<this_class(Iterable&&, Func1, Func2)>(*)(Iterable&&, Func1, Func2);

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

            template<typename Iterable>
            operator fptr_rvalue_t<Iterable>() const
            {
                return [](Iterable&& iterable) {
                    return this_class{}(std::move(iterable));
                };
            }

            template<typename Iterable, typename Func>
            operator fptr_func_t<Iterable, Func>() const
            {
                return [](Iterable& iterable, Func func) {
                    return this_class{}(iterable, func);
                };
            }

            template<typename Iterable, typename Func>
            operator fptr_rvalue_func_t<Iterable, Func>() const
            {
                return [](Iterable&& iterable, Func func) {
                    return this_class{}(std::move(iterable), func);
                };
            }

            template<typename Iterable, typename Func1, typename Func2>
            operator fptr_func2_t<Iterable, Func1, Func2>() const
            {
                return [](Iterable& iterable, Func1 func1, Func2 func2) {
                    return this_class{}(iterable, func1, func2);
                };
            }

            template<typename Iterable, typename Func1, typename Func2>
            operator fptr_rvalue_func2_t<Iterable, Func1, Func2>() const
            {
                return [](Iterable&& iterable, Func1 func1, Func2 func2) {
                    return this_class{}(std::move(iterable), func1, func2);
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
            auto operator()(Iterable&& iterable) const
                -> std::enable_if_t<
                    detail::has_sort<Sorter, Iterable>::value,
                    decltype(Sorter::operator()(std::forward<Iterable>(iterable)))
                >
            {
                return Sorter::operator()(std::forward<Iterable>(iterable));
            }

            template<typename Iterable>
            auto operator()(Iterable&& iterable) const
                -> std::enable_if_t<
                    not detail::has_sort<Sorter, Iterable>::value,
                    decltype(Sorter::operator()(utility::begin(iterable), utility::end(iterable)))
                >
            {
                return Sorter::operator()(utility::begin(iterable), utility::end(iterable));
            }

            ////////////////////////////////////////////////////////////
            // Comparison overloads

            template<typename Iterator, typename Compare>
            auto operator()(Iterator first, Iterator last, Compare compare) const
                -> std::enable_if_t<
                    detail::has_comparison_sort_iterator<Sorter, Iterator, Compare>::value,
                    decltype(Sorter::operator()(first, last, compare))
                >
            {
                return Sorter::operator()(first, last, compare);
            }

            template<typename Iterable, typename Compare>
            auto operator()(Iterable&& iterable, Compare compare) const
                -> std::enable_if_t<
                    detail::has_comparison_sort<Sorter, Iterable, Compare>::value,
                    decltype(Sorter::operator()(std::forward<Iterable>(iterable), compare))
                >
            {
                return Sorter::operator()(std::forward<Iterable>(iterable), compare);
            }

            template<typename Iterable, typename Compare>
            auto operator()(Iterable&& iterable, Compare compare) const
                -> std::enable_if_t<
                    not detail::has_comparison_sort<Sorter, Iterable, Compare>::value &&
                    detail::has_comparison_sort_iterator<Sorter, decltype(utility::begin(iterable)), Compare>::value,
                    decltype(Sorter::operator()(utility::begin(iterable), utility::end(iterable), compare))
                >
            {
                return Sorter::operator()(utility::begin(iterable), utility::end(iterable), compare);
            }

            ////////////////////////////////////////////////////////////
            // Projection overloads

            template<typename Iterator, typename Projection>
            auto operator()(Iterator first, Iterator last, Projection projection) const
                -> std::enable_if_t<
                    not detail::has_comparison_sort_iterator<Sorter, Iterator, Projection>::value &&
                    detail::has_projection_sort_iterator<Sorter, Iterator, Projection>::value,
                    decltype(Sorter::operator()(first, last, projection))
                >
            {
                return Sorter::operator()(first, last, projection);
            }

            template<typename Iterator, typename Projection>
            auto operator()(Iterator first, Iterator last, Projection projection) const
                -> std::enable_if_t<
                    not detail::has_projection_sort_iterator<Sorter, Iterator, Projection>::value &&
                        detail::has_comparison_projection_sort_iterator<Sorter, Iterator, std::less<>, Projection>::value,
                    decltype(Sorter::operator()(first, last, std::less<>{}, projection))
                >
            {
                return Sorter::operator()(first, last, std::less<>{}, projection);
            }

            template<typename Iterable, typename Projection>
            auto operator()(Iterable&& iterable, Projection projection) const
                -> std::enable_if_t<
                    not detail::has_comparison_sort<Sorter, Iterable, Projection>::value &&
                    detail::has_projection_sort<Sorter, Iterable, Projection>::value,
                    decltype(Sorter::operator()(std::forward<Iterable>(iterable), projection))
                >
            {
                return Sorter::operator()(std::forward<Iterable>(iterable), projection);
            }

            template<typename Iterable, typename Projection>
            auto operator()(Iterable&& iterable, Projection projection) const
                -> std::enable_if_t<
                    not detail::has_projection_sort<Sorter, Iterable, Projection>::value &&
                        detail::has_comparison_projection_sort<Sorter, Iterable, std::less<>, Projection>::value,
                    decltype(Sorter::operator()(std::forward<Iterable>(iterable), std::less<>{}, projection))
                >
            {
                return Sorter::operator()(std::forward<Iterable>(iterable), std::less<>{}, projection);
            }

            template<typename Iterable, typename Projection>
            auto operator()(Iterable&& iterable, Projection projection) const
                -> std::enable_if_t<
                    not detail::has_comparison_sort<Sorter, Iterable, Projection>::value &&
                    not detail::has_comparison_sort_iterator<Sorter, decltype(utility::begin(iterable)), Projection>::value &&
                    not detail::has_projection_sort<Sorter, Iterable, Projection>::value &&
                    not detail::has_comparison_projection_sort<Sorter, Iterable, std::less<>, Projection>::value &&
                        detail::has_projection_sort_iterator<Sorter, decltype(utility::begin(iterable)), Projection>::value,
                    decltype(Sorter::operator()(utility::begin(iterable), utility::end(iterable), projection))
                >
            {
                return Sorter::operator()(utility::begin(iterable), utility::end(iterable), projection);
            }

            template<typename Iterable, typename Projection>
            auto operator()(Iterable&& iterable, Projection projection) const
                -> std::enable_if_t<
                    not detail::has_projection_sort<Sorter, Iterable, Projection>::value &&
                    not detail::has_comparison_projection_sort<Sorter, Iterable, std::less<>, Projection>::value &&
                    not detail::has_projection_sort_iterator<Sorter, decltype(utility::begin(iterable)), Projection>::value &&
                        detail::has_comparison_projection_sort_iterator<
                        Sorter,
                        decltype(utility::begin(iterable)),
                        std::less<>,
                        Projection
                    >::value,
                    decltype(Sorter::operator()(utility::begin(iterable), utility::end(iterable), std::less<>{}, projection))
                >
            {
                return Sorter::operator()(utility::begin(iterable), utility::end(iterable), std::less<>{}, projection);
            }

            ////////////////////////////////////////////////////////////
            // std::less<> overloads

            template<typename Iterator>
            auto operator()(Iterator first, Iterator last, std::less<>) const
                -> std::enable_if_t<
                    not detail::has_comparison_sort_iterator<Sorter, Iterator, std::less<>>::value,
                    decltype(Sorter::operator()(first, last))
                >
            {
                return Sorter::operator()(first, last);
            }

            template<typename Iterable>
            auto operator()(Iterable&& iterable, std::less<>) const
                -> std::enable_if_t<
                    not detail::has_comparison_sort_iterator<
                        Sorter,
                        decltype(utility::begin(iterable)),
                        std::less<>
                    >::value,
                    decltype(operator()(std::forward<Iterable>(iterable)))
                >
            {
                return operator()(std::forward<Iterable>(iterable));
            }

            ////////////////////////////////////////////////////////////
            // utility::identity overloads

            template<typename Iterator>
            auto operator()(Iterator first, Iterator last, utility::identity) const
                -> std::enable_if_t<
                    not detail::has_projection_sort_iterator<Sorter, Iterator, utility::identity>::value &&
                    not detail::has_comparison_projection_sort_iterator<
                        Sorter,
                        Iterator,
                        std::less<>,
                        utility::identity
                    >::value,
                    decltype(Sorter::operator()(first, last))
                >
            {
                return Sorter::operator()(first, last);
            }

            template<typename Iterable>
            auto operator()(Iterable&& iterable, utility::identity) const
                -> std::enable_if_t<
                    not detail::has_projection_sort_iterator<
                        Sorter,
                        decltype(utility::begin(iterable)),
                    utility::identity
                    >::value &&
                    not detail::has_comparison_projection_sort_iterator<
                        Sorter,
                        decltype(utility::begin(iterable)),
                        std::less<>,
                        utility::identity
                    >::value,
                    decltype(operator()(std::forward<Iterable>(iterable)))
                >
            {
                return operator()(std::forward<Iterable>(iterable));
            }

            ////////////////////////////////////////////////////////////
            // Fused comparison-projection overloads

            template<typename Iterator, typename Compare, typename Projection>
            auto operator()(Iterator first, Iterator last,
                            Compare compare, Projection projection) const
                -> std::enable_if_t<
                    detail::has_comparison_projection_sort_iterator<
                        Sorter,
                        Iterator,
                        Compare,
                        Projection
                    >::value,
                    decltype(Sorter::operator()(first, last, compare, projection))
                >
            {
                return Sorter::operator()(first, last, compare, projection);
            }

            template<typename Iterable, typename Compare, typename Projection>
            auto operator()(Iterable&& iterable, Compare compare, Projection projection) const
                -> std::enable_if_t<
                    detail::has_comparison_projection_sort<Sorter, Iterable, Compare, Projection>::value,
                    decltype(Sorter::operator()(std::forward<Iterable>(iterable), compare, projection))
                >
            {
                return Sorter::operator()(std::forward<Iterable>(iterable), compare, projection);
            }

            template<typename Iterable, typename Compare, typename Projection>
            auto operator()(Iterable&& iterable, Compare compare, Projection projection) const
                -> std::enable_if_t<
                    not detail::has_comparison_projection_sort<Sorter, Iterable, Compare, Projection>::value &&
                        detail::has_comparison_projection_sort_iterator<
                        Sorter,
                        decltype(utility::begin(iterable)),
                        Compare,
                        Projection
                    >::value,
                    decltype(Sorter::operator()(utility::begin(iterable), utility::end(iterable), compare, projection))
                >
            {
                return Sorter::operator()(utility::begin(iterable), utility::end(iterable), compare, projection);
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
                    >::value,
                    decltype(Sorter::operator()(first, last))
                >
            {
                return Sorter::operator()(first, last);
            }

            template<typename Iterable>
            auto operator()(Iterable&& iterable, std::less<>, utility::identity) const
                -> std::enable_if_t<
                    not detail::has_comparison_projection_sort_iterator<
                        Sorter,
                        decltype(utility::begin(iterable)),
                        std::less<>,
                        utility::identity
                    >::value,
                    decltype(operator()(std::forward<Iterable>(iterable)))
                >
            {
                return operator()(std::forward<Iterable>(iterable));
            }

            template<typename Iterator, typename Projection>
            auto operator()(Iterator first, Iterator last, std::less<>, Projection projection) const
                -> std::enable_if_t<
                    detail::has_comparison_projection_sort_iterator<
                        Sorter,
                        Iterator,
                        std::less<>,
                        Projection
                    >::value,
                    decltype(Sorter::operator()(first, last, std::less<>{}, projection))
                >
            {
                return Sorter::operator()(first, last, std::less<>{}, projection);
            }

            template<typename Iterator, typename Projection>
            auto operator()(Iterator first, Iterator last, std::less<>, Projection projection) const
                -> std::enable_if_t<
                    not detail::has_comparison_projection_sort_iterator<
                        Sorter,
                        Iterator,
                        std::less<>,
                        Projection
                    >::value &&
                    detail::has_projection_sort_iterator<
                        Sorter,
                        Iterator,
                        Projection
                    >::value,
                    decltype(Sorter::operator()(first, last, projection))
                >
            {
                return Sorter::operator()(first, last, projection);
            }

            template<typename Iterable, typename Projection>
            auto operator()(Iterable&& iterable, std::less<>, Projection projection) const
                -> std::enable_if_t<
                    detail::has_comparison_projection_sort<
                        Sorter,
                        Iterable,
                        std::less<>,
                        Projection
                    >::value,
                    decltype(Sorter::operator()(std::forward<Iterable>(iterable), std::less<>{}, projection))
                >
            {
                return Sorter::operator()(std::forward<Iterable>(iterable), std::less<>{}, projection);
            }

            template<typename Iterable, typename Projection>
            auto operator()(Iterable&& iterable, std::less<>, Projection projection) const
                -> std::enable_if_t<
                    not detail::has_comparison_projection_sort<
                        Sorter,
                        Iterable,
                        std::less<>,
                        Projection
                    >::value &&
                    detail::has_comparison_projection_sort_iterator<
                        Sorter,
                        decltype(utility::begin(iterable)),
                        std::less<>,
                        Projection
                    >::value,
                    decltype(Sorter::operator()(utility::begin(iterable), utility::end(iterable), std::less<>{}, projection))
                >
            {
                return Sorter::operator()(utility::begin(iterable), utility::end(iterable), std::less<>{}, projection);
            }

            template<typename Iterable, typename Projection>
            auto operator()(Iterable&& iterable, std::less<>, Projection projection) const
                -> std::enable_if_t<
                    not detail::has_comparison_projection_sort<
                        Sorter,
                        Iterable,
                        std::less<>,
                        Projection
                    >::value &&
                    not detail::has_comparison_projection_sort_iterator<
                        Sorter,
                        decltype(utility::begin(iterable)),
                        std::less<>,
                        Projection
                    >::value &&
                    detail::has_projection_sort<
                        Sorter,
                        Iterable,
                        Projection
                    >::value,
                    decltype(Sorter::operator()(std::forward<Iterable>(iterable), projection))
                >
            {
                return Sorter::operator()(std::forward<Iterable>(iterable), projection);
            }

            template<typename Iterable, typename Projection>
            auto operator()(Iterable&& iterable, std::less<>, Projection projection) const
                -> std::enable_if_t<
                    not detail::has_comparison_projection_sort<
                        Sorter,
                        Iterable,
                        std::less<>,
                        Projection
                    >::value &&
                    not detail::has_comparison_projection_sort_iterator<
                        Sorter,
                        decltype(utility::begin(iterable)),
                        std::less<>,
                        Projection
                    >::value &&
                    not detail::has_projection_sort<
                        Sorter,
                        Iterable,
                        Projection
                    >::value &&
                    detail::has_projection_sort_iterator<
                        Sorter,
                        decltype(utility::begin(iterable)),
                        Projection
                    >::value,
                    decltype(Sorter::operator()(utility::begin(iterable), utility::end(iterable), projection))
                >
            {
                return Sorter::operator()(utility::begin(iterable), utility::end(iterable), projection);
            }

            ////////////////////////////////////////////////////////////
            // Embed projection in comparison

            template<typename Iterator, typename Projection>
            auto operator()(Iterator first, Iterator last, Projection projection) const
                -> std::enable_if_t<
                    not detail::has_projection_sort_iterator<Sorter, Iterator, Projection>::value &&
                    not detail::has_comparison_projection_sort_iterator<
                        Sorter,
                        Iterator,
                        std::less<>,
                        Projection
                    >::value &&
                    detail::has_comparison_sort_iterator<
                        Sorter,
                        Iterator,
                        detail::projection_compare<std::less<>, Projection>
                    >::value,
                    decltype(Sorter::operator()(first, last, detail::make_projection_compare(std::less<>{}, projection)))
                >
            {
                return Sorter::operator()(first, last, detail::make_projection_compare(std::less<>{}, projection));
            }

            template<typename Iterator, typename Compare, typename Projection>
            auto operator()(Iterator first, Iterator last,
                            Compare compare, Projection projection) const
                -> std::enable_if_t<
                    not detail::has_comparison_projection_sort_iterator<
                        Sorter,
                        Iterator,
                        Compare,
                        Projection
                    >::value &&
                    detail::has_comparison_sort_iterator<
                        Sorter,
                        Iterator,
                        detail::projection_compare<Compare, Projection>
                    >::value,
                    decltype(Sorter::operator()(first, last, detail::make_projection_compare(compare, projection)))
                >
            {
                return Sorter::operator()(first, last, detail::make_projection_compare(compare, projection));
            }

            template<typename Iterable, typename Projection>
            auto operator()(Iterable&& iterable, Projection projection) const
                -> std::enable_if_t<
                    not detail::has_projection_sort_iterator<
                        Sorter,
                        decltype(utility::begin(iterable)),
                        Projection
                    >::value &&
                    not detail::has_comparison_projection_sort_iterator<
                        Sorter,
                        decltype(utility::begin(iterable)),
                        std::less<>,
                        Projection
                    >::value &&
                    detail::has_comparison_sort<
                        Sorter,
                        Iterable,
                        detail::projection_compare<std::less<>, Projection>
                    >::value,
                    decltype(Sorter::operator()(std::forward<Iterable>(iterable), detail::make_projection_compare(std::less<>{}, projection)))
                >
            {
                return Sorter::operator()(std::forward<Iterable>(iterable), detail::make_projection_compare(std::less<>{}, projection));
            }

            template<typename Iterable, typename Projection>
            auto operator()(Iterable&& iterable, Projection projection) const
                -> std::enable_if_t<
                    not detail::has_projection_sort_iterator<
                        Sorter,
                        decltype(utility::begin(iterable)),
                        Projection
                    >::value &&
                    not detail::has_comparison_projection_sort_iterator<
                        Sorter,
                        decltype(utility::begin(iterable)),
                        std::less<>,
                        Projection
                    >::value &&
                    not detail::has_comparison_sort<
                        Sorter,
                        Iterable,
                        detail::projection_compare<std::less<>, Projection>
                    >::value &&
                    detail::has_comparison_sort_iterator<
                        Sorter,
                        decltype(utility::begin(iterable)),
                        detail::projection_compare<std::less<>, Projection>
                    >::value,
                    decltype(Sorter::operator()(utility::begin(iterable), utility::end(iterable), detail::make_projection_compare(std::less<>{}, projection)))
                >
            {
                return Sorter::operator()(utility::begin(iterable), utility::end(iterable), detail::make_projection_compare(std::less<>{}, projection));
            }

            template<typename Iterable, typename Compare, typename Projection>
            auto operator()(Iterable&& iterable, Compare compare, Projection projection) const
                -> std::enable_if_t<
                    not detail::has_comparison_projection_sort_iterator<
                        Sorter,
                        decltype(utility::begin(iterable)),
                        Compare,
                        Projection
                    >::value &&
                    detail::has_comparison_sort<
                        Sorter,
                        Iterable,
                        detail::projection_compare<Compare, Projection>
                    >::value,
                    decltype(Sorter::operator()(std::forward<Iterable>(iterable), detail::make_projection_compare(compare, projection)))
                >
            {
                return Sorter::operator()(std::forward<Iterable>(iterable), detail::make_projection_compare(compare, projection));
            }

            template<typename Iterable, typename Compare, typename Projection>
            auto operator()(Iterable&& iterable, Compare compare, Projection projection) const
                -> std::enable_if_t<
                    not detail::has_comparison_projection_sort_iterator<
                        Sorter,
                        decltype(utility::begin(iterable)),
                        Compare,
                        Projection
                    >::value &&
                    not detail::has_comparison_sort<
                        Sorter,
                        Iterable,
                        detail::projection_compare<Compare, Projection>
                    >::value &&
                    detail::has_comparison_sort_iterator<
                        Sorter,
                        decltype(utility::begin(iterable)),
                        detail::projection_compare<Compare, Projection>
                    >::value,
                    decltype(Sorter::operator()(utility::begin(iterable), utility::end(iterable), detail::make_projection_compare(compare, projection)))
                >
            {
                return Sorter::operator()(utility::begin(iterable), utility::end(iterable), detail::make_projection_compare(compare, projection));
            }
    };
}

#endif // CPPSORT_SORTER_FACADE_H_
