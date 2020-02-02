/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2019 Morwenn
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
#ifndef CPPSORT_DETAIL_MERGE_INSERTION_SORT_H_
#define CPPSORT_DETAIL_MERGE_INSERTION_SORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <cstdint>
#include <iterator>
#include <list>
#include <new>
#include <type_traits>
#include <utility>
#include <vector>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/iter_move.h>
#include "config.h"
#include "iterator_traits.h"
#include "memory.h"
#include "move.h"
#include "swap_if.h"
#include "swap_ranges.h"
#include "type_traits.h"

#if __has_include(<ext/bitmap_allocator.h>)
#   include <ext/bitmap_allocator.h>
#endif

namespace cppsort
{
namespace detail
{
    ////////////////////////////////////////////////////////////
    // Iterator used to sort groups

    template<typename Iterator>
    class group_iterator
    {
        public:

            ////////////////////////////////////////////////////////////
            // Public types

            using iterator_category = iterator_category_t<Iterator>;
            using iterator_type     = Iterator;
            using value_type        = value_type_t<Iterator>;
            using difference_type   = difference_type_t<Iterator>;
            using pointer           = pointer_t<Iterator>;
            using reference         = reference_t<Iterator>;

            ////////////////////////////////////////////////////////////
            // Constructors

            group_iterator() = default;

            group_iterator(Iterator it, difference_type size):
                _it(std::move(it)),
                _size(size)
            {}

            ////////////////////////////////////////////////////////////
            // Members access

            auto base() const
                -> iterator_type
            {
                return _it;
            }

            auto size() const
                -> difference_type
            {
                return _size;
            }

            ////////////////////////////////////////////////////////////
            // Element access

            auto operator*() const
                -> reference
            {
                return _it[_size - 1];
            }

            auto operator->() const
                -> pointer
            {
                return &(operator*());
            }

            ////////////////////////////////////////////////////////////
            // Increment/decrement operators

            auto operator++()
                -> group_iterator&
            {
                std::advance(_it, _size);
                return *this;
            }

            auto operator++(int)
                -> group_iterator
            {
                auto tmp = *this;
                operator++();
                return tmp;
            }

            auto operator--()
                -> group_iterator&
            {
                _it -= _size;
                return *this;
            }

            auto operator--(int)
                -> group_iterator
            {
                auto tmp = *this;
                operator--();
                return tmp;
            }

            auto operator+=(difference_type increment)
                -> group_iterator&
            {
                std::advance(_it, _size * increment);
                return *this;
            }

            auto operator-=(difference_type increment)
                -> group_iterator&
            {
                _it -= _size * increment;
                return *this;
            }

            ////////////////////////////////////////////////////////////
            // Elements access operators

            auto operator[](difference_type pos)
                -> decltype(base()[pos * size() + size() - 1])
            {
                return base()[pos * size() + size() - 1];
            }

            auto operator[](difference_type pos) const
                -> decltype(base()[pos * size() + size() - 1])
            {
                return base()[pos * size() + size() - 1];
            }

            ////////////////////////////////////////////////////////////
            // Comparison operators

            friend auto operator==(const group_iterator& lhs, const group_iterator& rhs)
                -> bool
            {
                return lhs.base() == rhs.base();
            }

            friend auto operator!=(const group_iterator& lhs, const group_iterator& rhs)
                -> bool
            {
                return lhs.base() != rhs.base();
            }

            ////////////////////////////////////////////////////////////
            // Relational operators

            friend auto operator<(const group_iterator& lhs, const group_iterator& rhs)
                -> bool
            {
                return lhs.base() < rhs.base();
            }

            friend auto operator<=(const group_iterator& lhs, const group_iterator& rhs)
                -> bool
            {
                return lhs.base() <= rhs.base();
            }

            friend auto operator>(const group_iterator& lhs, const group_iterator& rhs)
                -> bool
            {
                return lhs.base() > rhs.base();
            }

            friend auto operator>=(const group_iterator& lhs, const group_iterator& rhs)
                -> bool
            {
                return lhs.base() >= rhs.base();
            }

            ////////////////////////////////////////////////////////////
            // Arithmetic operators

            friend auto operator+(group_iterator it, difference_type size)
                -> group_iterator
            {
                return it += size;
            }

            friend auto operator+(difference_type size, group_iterator it)
                -> group_iterator
            {
                return it += size;
            }

            friend auto operator-(group_iterator it, difference_type size)
                -> group_iterator
            {
                return it -= size;
            }

            friend auto operator-(const group_iterator& lhs, const group_iterator& rhs)
                -> difference_type
            {
                return (lhs.base() - rhs.base()) / lhs.size();
            }

        private:

            Iterator _it;
            difference_type _size;
    };

    template<typename Iterator>
    auto iter_swap(group_iterator<Iterator> lhs, group_iterator<Iterator> rhs)
        -> void
    {
        detail::swap_ranges(lhs.base(), lhs.base() + lhs.size(), rhs.base());
    }

    ////////////////////////////////////////////////////////////
    // Construction function

    template<typename Iterator>
    auto make_group_iterator(Iterator it, difference_type_t<group_iterator<Iterator>> size)
        -> group_iterator<Iterator>
    {
        return { it, size };
    }

    template<typename Iterator>
    auto make_group_iterator(group_iterator<Iterator> it, difference_type_t<group_iterator<Iterator>> size)
        -> group_iterator<Iterator>
    {
        return { it.base(), size * it.size() };
    }

    ////////////////////////////////////////////////////////////
    // Merge-insertion sort

    template<
        typename RandomAccessIterator,
        typename Compare,
        typename Projection
    >
    auto merge_insertion_sort_impl(group_iterator<RandomAccessIterator> first,
                                   group_iterator<RandomAccessIterator> last,
                                   Compare compare, Projection projection)
        -> void
    {
        // Cache all the differences between a Jacobsthal number and its
        // predecessor that fit in 64 bits, starting with the difference
        // between the Jacobsthal numbers 4 and 3 (the previous ones are
        // unneeded)
        constexpr std::uint_fast64_t jacobsthal_diff[] = {
            2u, 2u, 6u, 10u, 22u, 42u, 86u, 170u, 342u, 682u, 1366u,
            2730u, 5462u, 10922u, 21846u, 43690u, 87382u, 174762u, 349526u, 699050u,
            1398102u, 2796202u, 5592406u, 11184810u, 22369622u, 44739242u, 89478486u,
            178956970u, 357913942u, 715827882u, 1431655766u, 2863311530u, 5726623062u,
            11453246122u, 22906492246u, 45812984490u, 91625968982u, 183251937962u,
            366503875926u, 733007751850u, 1466015503702u, 2932031007402u, 5864062014806u,
            11728124029610u, 23456248059222u, 46912496118442u, 93824992236886u, 187649984473770u,
            375299968947542u, 750599937895082u, 1501199875790165u, 3002399751580331u,
            6004799503160661u, 12009599006321322u, 24019198012642644u, 48038396025285288u,
            96076792050570576u, 192153584101141152u, 384307168202282304u, 768614336404564608u,
            1537228672809129216u, 3074457345618258432u, 6148914691236516864u
        };

        auto size = std::distance(first, last);
        if (size < 2) return;

        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        // Whether there is a stray element not in a pair
        // at the end of the chain
        bool has_stray = (size % 2 != 0);

        ////////////////////////////////////////////////////////////
        // Group elements by pairs

        auto end = has_stray ? std::prev(last) : last;
        for (auto it = first ; it != end ; it += 2)
        {
            iter_swap_if(it, it + 1, compare, projection);
        }

        ////////////////////////////////////////////////////////////
        // Recursively sort the pairs by max

        merge_insertion_sort_impl(
            make_group_iterator(first, 2),
            make_group_iterator(end, 2),
            compare, projection
        );

        ////////////////////////////////////////////////////////////
        // Separate main chain and pend elements

#if __has_include(<ext/bitmap_allocator.h>)
        using list_t = std::list<
            group_iterator<RandomAccessIterator>,
            __gnu_cxx::bitmap_allocator<group_iterator<RandomAccessIterator>>
        >;
#else
        using list_t = std::list<group_iterator<RandomAccessIterator>>;
#endif

        // The first pend element is always part of the main chain,
        // so we can safely initialize the list with the first two
        // elements of the sequence
        list_t chain = { first, std::next(first) };

        // Upper bounds for the insertion of pend elements
        std::vector<typename list_t::iterator> pend;
        pend.reserve((size + 1) / 2 - 1);

        for (auto it = first + 2 ; it != end ; it += 2)
        {
            auto tmp = chain.insert(std::end(chain), std::next(it));
            pend.push_back(tmp);
        }

        // Add the last element to pend if it exists; when it
        // exists, it always has to be inserted in the full chain,
        // so giving it chain.end() as end insertion point is ok
        if (has_stray)
        {
            pend.push_back(std::end(chain));
        }

        ////////////////////////////////////////////////////////////
        // Binary insertion into the main chain

        auto current_it = first + 2;
        auto current_pend = std::begin(pend);

        for (int k = 0 ; ; ++k)
        {
            // Should be safe: in this code, std::distance should always return
            // a positive number, so there is of risk comparing funny values
            using size_type = std::common_type_t<
                std::uint_fast64_t,
                typename list_t::difference_type
            >;

            // Find next index
            auto dist = jacobsthal_diff[k];
            if (dist > static_cast<size_type>(std::distance(current_pend, std::end(pend)))) break;

            auto it = std::next(current_it, dist * 2);
            auto pe = std::next(current_pend, dist);

            do
            {
                --pe;
                it -= 2;

                auto insertion_point = std::upper_bound(
                    std::begin(chain), *pe, proj(*it),
                    [=](const auto& lhs, const auto& rhs) mutable {
                        return comp(lhs, proj(*rhs));
                    }
                );
                chain.insert(insertion_point, it);
            } while (pe != current_pend);

            std::advance(current_it, dist * 2);
            std::advance(current_pend, dist);
        }

        // If there are pend elements left, insert them into
        // the main chain, the order of insertion does not
        // matter so forward traversal is ok
        while (current_pend != std::end(pend))
        {
            auto insertion_point = std::upper_bound(
                std::begin(chain), *current_pend, proj(*current_it),
                [=](const auto& lhs, const auto& rhs) mutable {
                    return comp(lhs, proj(*rhs));
                }
            );
            chain.insert(insertion_point, current_it);
            current_it += 2;
            ++current_pend;
        }

        ////////////////////////////////////////////////////////////
        // Move values in order to a cache then back to origin

        // Number of sub-iterators
        auto full_size = size * first.size();

        using rvalue_reference = remove_cvref_t<rvalue_reference_t<RandomAccessIterator>>;
        std::unique_ptr<rvalue_reference, operator_deleter> cache(
            static_cast<rvalue_reference*>(::operator new(full_size * sizeof(rvalue_reference))),
            operator_deleter(full_size * sizeof(rvalue_reference))
        );
        destruct_n<rvalue_reference> d(0);
        std::unique_ptr<rvalue_reference, destruct_n<rvalue_reference>&> h2(cache.get(), d);

        rvalue_reference* buff_it = cache.get();
        for (auto&& it: chain) {
            auto begin = it.base();
            auto end = begin + it.size();
            buff_it = uninitialized_move(begin, end, buff_it, d);
        }
        detail::move(cache.get(), cache.get() + full_size, first.base());
    }

    template<
        typename RandomAccessIterator,
        typename Compare,
        typename Projection
    >
    auto merge_insertion_sort(RandomAccessIterator first, RandomAccessIterator last,
                              Compare compare, Projection projection)
        -> void
    {
        merge_insertion_sort_impl(
            make_group_iterator(std::move(first), 1),
            make_group_iterator(std::move(last), 1),
            std::move(compare), std::move(projection)
        );
    }
}}

#endif // CPPSORT_DETAIL_MERGE_INSERTION_SORT_H_
