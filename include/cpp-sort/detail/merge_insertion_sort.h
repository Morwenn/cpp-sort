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
#ifndef CPPSORT_DETAIL_MERGE_INSERTION_SORT_H_
#define CPPSORT_DETAIL_MERGE_INSERTION_SORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <list>
#include <iterator>
#include <type_traits>
#include <vector>
#include <cpp-sort/utility/as_function.h>

namespace cppsort
{
namespace detail
{
    ////////////////////////////////////////////////////////////
    // Iterator used to sort groups

    template<typename Iterator>
    class group_iterator
    {
        private:

            Iterator _it;
            std::size_t _size;

        public:

            ////////////////////////////////////////////////////////////
            // Public types

            using iterator_category = typename std::iterator_traits<Iterator>::iterator_category;
            using iterator_type     = Iterator;
            using value_type        = typename std::iterator_traits<Iterator>::value_type;
            using difference_type   = typename std::iterator_traits<Iterator>::difference_type;
            using pointer           = typename std::iterator_traits<Iterator>::pointer;
            using reference         = typename std::iterator_traits<Iterator>::reference;

            ////////////////////////////////////////////////////////////
            // Constructors

            group_iterator() = default;

            group_iterator(Iterator it, std::size_t size):
                _it(it),
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
                -> std::size_t
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

            auto operator+=(std::size_t increment)
                -> group_iterator&
            {
                std::advance(_it, _size * increment);
                return *this;
            }

            auto operator-=(std::size_t increment)
                -> group_iterator&
            {
                _it -= _size * increment;
                return *this;
            }

            ////////////////////////////////////////////////////////////
            // Elements access operators

            auto operator[](std::size_t pos)
                -> decltype(_it[pos * _size + _size - 1])
            {
                return _it[pos * _size + _size - 1];
            }

            auto operator[](std::size_t pos) const
                -> decltype(_it[pos * _size + _size - 1])
            {
                return _it[pos * _size + _size - 1];
            }
    };

    template<typename Iterator1, typename Iterator2>
    auto iter_swap(group_iterator<Iterator1> lhs, group_iterator<Iterator2> rhs)
        -> void
    {
        std::swap_ranges(lhs.base(), lhs.base() + lhs.size(), rhs.base());
    }

    ////////////////////////////////////////////////////////////
    // Comparison operators

    template<typename Iterator1, typename Iterator2>
    auto operator==(const group_iterator<Iterator1>& lhs,
                    const group_iterator<Iterator2>& rhs)
        -> bool
    {
        return lhs.base() == rhs.base();
    }

    template<typename Iterator1, typename Iterator2>
    auto operator!=(const group_iterator<Iterator1>& lhs,
                    const group_iterator<Iterator2>& rhs)
        -> bool
    {
        return lhs.base() != rhs.base();
    }

    ////////////////////////////////////////////////////////////
    // Relational operators

    template<typename Iterator1, typename Iterator2>
    auto operator<(const group_iterator<Iterator1>& lhs,
                   const group_iterator<Iterator2>& rhs)
        -> bool
    {
        return lhs.base() < rhs.base();
    }

    template<typename Iterator1, typename Iterator2>
    auto operator<=(const group_iterator<Iterator1>& lhs,
                    const group_iterator<Iterator2>& rhs)
        -> bool
    {
        return lhs.base() <= rhs.base();
    }

    template<typename Iterator1, typename Iterator2>
    auto operator>(const group_iterator<Iterator1>& lhs,
                   const group_iterator<Iterator2>& rhs)
        -> bool
    {
        return lhs.base() > rhs.base();
    }

    template<typename Iterator1, typename Iterator2>
    auto operator>=(const group_iterator<Iterator1>& lhs,
                    const group_iterator<Iterator2>& rhs)
        -> bool
    {
        return lhs.base >= rhs.base();
    }

    ////////////////////////////////////////////////////////////
    // Arithmetic operators

    template<typename Iterator>
    auto operator+(group_iterator<Iterator> it, std::size_t size)
        -> group_iterator<Iterator>
    {
        return it += size;
    }

    template<typename Iterator>
    auto operator+(std::size_t size, group_iterator<Iterator> it)
        -> group_iterator<Iterator>
    {
        return it += size;
    }

    template<typename Iterator>
    auto operator-(group_iterator<Iterator> it, std::size_t size)
        -> group_iterator<Iterator>
    {
        return it -= size;
    }

    template<typename Iterator>
    auto operator-(const group_iterator<Iterator>& lhs, const group_iterator<Iterator>& rhs)
        -> typename group_iterator<Iterator>::difference_type
    {
        return (lhs.base() - rhs.base()) / lhs.size();
    }

    ////////////////////////////////////////////////////////////
    // Construction function

    template<typename Iterator>
    auto make_group_iterator(Iterator it, std::size_t size)
        -> group_iterator<Iterator>
    {
        return { it, size };
    }

    template<typename Iterator>
    auto make_group_iterator(group_iterator<Iterator> it, std::size_t size)
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
    auto merge_insertion_sort_impl(RandomAccessIterator first, RandomAccessIterator last,
                                   Compare compare, Projection projection)
    {
        // Cache all the differences between a Jacobsthal number and its
        // predecessor minus 1 that fit in 64 bits, starting with the
        // difference between the Jacobsthal numbers 4 and 3 (the previous
        // ones are unneeded)
        static constexpr std::uint_least64_t jacobsthal_diff[] = {
            1u, 1u, 5u, 9u, 21u, 41u, 85u, 169u, 341u, 681u, 1365u,
            2729u, 5461u, 10921u, 21845u, 43689u, 87381u, 174761u, 349525u, 699049u,
            1398101u, 2796201u, 5592405u, 11184809u, 22369621u, 44739241u, 89478485u,
            178956969u, 357913941u, 715827881u, 1431655765u, 2863311529u, 5726623061u,
            11453246121u, 22906492245u, 45812984489u, 91625968981u, 183251937961u,
            366503875925u, 733007751849u, 1466015503701u, 2932031007401u, 5864062014805u,
            11728124029609u, 23456248059221u, 46912496118441u, 93824992236885u, 187649984473769u,
            375299968947541u, 750599937895081u, 1501199875790164u, 3002399751580330u,
            6004799503160660u, 12009599006321321u, 24019198012642643u, 48038396025285287u,
            96076792050570575u, 192153584101141151u, 384307168202282303u, 768614336404564607u,
            1537228672809129215u, 3074457345618258431u, 6148914691236516863u
        };

        using std::iter_swap;

        auto size = std::distance(first, last);
        if (size < 2) return;

        auto&& proj = utility::as_function(projection);

        // Whether there is a stray element not in a pair
        // at the end of the chain
        bool has_stray = (size % 2 != 0);

        ////////////////////////////////////////////////////////////
        // Group elements by pairs

        auto end = has_stray ? std::prev(last) : last;
        for (auto it = first ; it != end ; it += 2)
        {
            if (compare(proj(it[1]), proj(it[0])))
            {
                iter_swap(it, it + 1);
            }
        }

        ////////////////////////////////////////////////////////////
        // Recursively sort the pairs by max

        merge_insertion_sort(
            make_group_iterator(first, 2),
            make_group_iterator(end, 2),
            compare, projection
        );

        ////////////////////////////////////////////////////////////
        // Separate main chain and pend elements

        // Small node struct for pend elements
        struct node
        {
            RandomAccessIterator it;
            typename std::list<RandomAccessIterator>::iterator next;
        };

        // The first pend element is always part of the main chain,
        // so we can safely initialize the list with the first two
        // elements of the sequence
        std::list<RandomAccessIterator> chain = { first, std::next(first) };
        std::list<node> pend;

        for (auto it = first + 2 ; it != end ; it += 2)
        {
            auto tmp = chain.insert(chain.end(), std::next(it));
            pend.push_back({it, tmp});
        }

        // Add the last element to pend if it exists, when it
        // exists, it always has to be inserted in the full chain,
        // so giving it chain.end() as end insertion point is ok
        if (has_stray)
        {
            pend.push_back({end, chain.end()});
        }

        ////////////////////////////////////////////////////////////
        // Binary insertion into the main chain

        for (int k = 0 ; ; ++k)
        {
            // Find next index
            auto dist = jacobsthal_diff[k];
            if (dist >= pend.size()) break;
            auto it = pend.begin();
            std::advance(it, dist);

            while (true)
            {
                auto insertion_point = std::upper_bound(
                    chain.begin(), it->next, it->it,
                    [=](auto lhs, auto rhs) {
                        return compare(proj(*lhs), proj(*rhs));
                    }
                );
                chain.insert(insertion_point, it->it);

                it = pend.erase(it);
                if (it == pend.begin()) break;
                --it;
            }
        }

        // If there are elements left, insert them too
        while (not pend.empty())
        {
            auto it = std::prev(pend.end());
            auto insertion_point = std::upper_bound(
                chain.begin(), it->next, it->it,
                [=](auto lhs, auto rhs) {
                    return compare(proj(*lhs), proj(*rhs));
                }
            );
            chain.insert(insertion_point, it->it);
            pend.pop_back();
        }

        ////////////////////////////////////////////////////////////
        // Move values in order to a cache then back to origin

        std::vector<typename std::iterator_traits<RandomAccessIterator>::value_type> cache;
        cache.reserve(size);

        for (auto&& it: chain)
        {
            auto begin = it.base();
            auto end = begin + it.size();
            std::move(begin, end, std::back_inserter(cache));
        }
        std::move(cache.begin(), cache.end(), first.base());
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
            make_group_iterator(first, 1),
            make_group_iterator(last, 1),
            compare, projection
        );
    }
}}

#endif // CPPSORT_DETAIL_MERGE_INSERTION_SORT_H_
