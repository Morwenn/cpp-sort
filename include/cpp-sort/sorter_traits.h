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
#ifndef CPPSORT_SORTER_TRAITS_H_
#define CPPSORT_SORTER_TRAITS_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <type_traits>
#include <cpp-sort/utility/detection.h>

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Sorter type categories

    namespace detail
    {
        // Group two iterators as a range, used to discriminate
        // sorters from comparison functions
        template<typename Iterator>
        class range
        {
            public:

                range(Iterator begin, Iterator end):
                    _begin(begin),
                    _end(end)
                {}

                Iterator begin() { return _begin; }
                Iterator end() { return _end; }
                Iterator begin() const { return _begin; }
                Iterator end() const { return _end; }

            private:

                Iterator _begin, _end;
        };

        template<typename Sorter, typename Iterable>
        using has_sort_t = std::result_of_t<Sorter(Iterable&)>;

        template<typename Sorter, typename Iterable, typename Compare>
        using has_comparison_sort_t = utility::void_t<
            std::result_of_t<Sorter(Iterable&, Compare)>,
            std::result_of_t<Compare(
                decltype(*std::begin(std::declval<Iterable&>())),
                decltype(*std::end(std::declval<Iterable&>()))
            )>
        >;

        template<typename Sorter, typename Iterator>
        using has_sort_iterator_t = std::result_of_t<Sorter(Iterator, Iterator)>;

        template<typename Sorter, typename Iterator, typename Compare>
        using has_comparison_sort_iterator_t = utility::void_t<
            std::result_of_t<Sorter(Iterator, Iterator, Compare)>,
            std::result_of_t<Compare(
                decltype(*std::declval<Iterator&>()),
                decltype(*std::declval<Iterator&>())
            )>
        >;

        template<typename Sorter, typename Iterable>
        constexpr bool has_sort
            = utility::is_detected_v<has_sort_t, Sorter, Iterable>;

        template<typename Sorter, typename Iterable, typename Compare>
        constexpr bool has_comparison_sort
            = utility::is_detected_v<has_comparison_sort_t, Sorter, Iterable, Compare>;

        template<typename Sorter, typename Iterator>
        constexpr bool has_sort_iterator
            = utility::is_detected_v<has_sort_iterator_t, Sorter, Iterator>;

        template<typename Sorter, typename Iterator, typename Compare>
        constexpr bool has_comparison_sort_iterator
            = utility::is_detected_v<has_comparison_sort_iterator_t, Sorter, Iterator, Compare>;
    }

    template<typename Sorter, typename Iterable>
    constexpr bool is_sorter
        = detail::has_sort<Sorter, Iterable>;

    template<typename Sorter, typename Iterable, typename Compare>
    constexpr bool is_comparison_sorter
        = is_sorter<Sorter, Iterable> &&
          detail::has_comparison_sort<Sorter, Iterable, Compare>;

    template<typename Sorter, typename Iterator>
    constexpr bool is_sorter_iterator
        = is_sorter<Sorter, detail::range<Iterator>> &&
          detail::has_sort_iterator<Sorter, Iterator>;

    template<typename Sorter, typename Iterator, typename Compare>
    constexpr bool is_comparison_sorter_iterator
        = is_sorter_iterator<Sorter, Iterator> &&
          is_comparison_sorter<Sorter, detail::range<Iterator>, Compare> &&
          detail::has_comparison_sort_iterator<Sorter, Iterator, Compare>;

    ////////////////////////////////////////////////////////////
    // Sorter traits

    template<typename Sorter>
    struct sorter_traits
    {
        // Defined but empty for SFINAE friendliness
    };

    template<typename Sorter>
    using iterator_category = typename sorter_traits<Sorter>::iterator_category;

    template<typename Sorter>
    constexpr bool is_stable = sorter_traits<Sorter>::is_stable;

    ////////////////////////////////////////////////////////////
    // Sorter traits modifiers

    template<typename Sorter, typename Category>
    struct rebind_iterator_category:
        Sorter
    {
        static_assert(
            std::is_base_of<
                iterator_category<Sorter>,
                Category
            >::value,
            "the new iterator category should be more specific"
        );
    };

    template<typename Sorter, typename Category>
    struct sorter_traits<rebind_iterator_category<Sorter, Category>>
    {
        using iterator_category = Category;
        static constexpr bool is_stable = cppsort::is_stable<Sorter>;
    };
}

#endif // CPPSORT_SORTER_TRAITS_H_
