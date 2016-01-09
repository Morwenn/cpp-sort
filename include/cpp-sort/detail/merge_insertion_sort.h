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
    class quad_iterator
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

            quad_iterator() = default;

            quad_iterator(Iterator it, std::size_t size):
                _it(it),
                _size(size)
            {}

            ////////////////////////////////////////////////////////////
            // Assignment operator

            auto operator=(const quad_iterator& other)
                -> quad_iterator&
                = default;

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
                -> quad_iterator&
            {
                std::advance(_it, _size);
                return *this;
            }

            auto operator++(int)
                -> quad_iterator
            {
                auto tmp = *this;
                operator++();
                return tmp;
            }

            auto operator--()
                -> quad_iterator&
            {
                _it -= _size;
                return *this;
            }

            auto operator--(int)
                -> quad_iterator
            {
                auto tmp = *this;
                operator--();
                return tmp;
            }

            auto operator+=(std::size_t increment)
                -> quad_iterator&
            {
                std::advance(_it, _size * increment);
                return *this;
            }

            auto operator-=(std::size_t increment)
                -> quad_iterator&
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
    auto iter_swap(quad_iterator<Iterator1> lhs, quad_iterator<Iterator2> rhs)
        -> void
    {
        std::swap_ranges(lhs.base(), lhs.base() + lhs.size(), rhs.base());
    }

    ////////////////////////////////////////////////////////////
    // Comparison operators

    template<typename Iterator1, typename Iterator2>
    auto operator==(const quad_iterator<Iterator1>& lhs,
                    const quad_iterator<Iterator2>& rhs)
        -> bool
    {
        return lhs.base() == rhs.base();
    }

    template<typename Iterator1, typename Iterator2>
    auto operator!=(const quad_iterator<Iterator1>& lhs,
                    const quad_iterator<Iterator2>& rhs)
        -> bool
    {
        return lhs.base() != rhs.base();
    }

    ////////////////////////////////////////////////////////////
    // Relational operators

    template<typename Iterator1, typename Iterator2>
    auto operator<(const quad_iterator<Iterator1>& lhs,
                   const quad_iterator<Iterator2>& rhs)
        -> bool
    {
        return lhs.base() < rhs.base();
    }

    template<typename Iterator1, typename Iterator2>
    auto operator<=(const quad_iterator<Iterator1>& lhs,
                    const quad_iterator<Iterator2>& rhs)
        -> bool
    {
        return lhs.base() <= rhs.base();
    }

    template<typename Iterator1, typename Iterator2>
    auto operator>(const quad_iterator<Iterator1>& lhs,
                   const quad_iterator<Iterator2>& rhs)
        -> bool
    {
        return lhs.base() > rhs.base();
    }

    template<typename Iterator1, typename Iterator2>
    auto operator>=(const quad_iterator<Iterator1>& lhs,
                    const quad_iterator<Iterator2>& rhs)
        -> bool
    {
        return lhs.base <= rhs.base();
    }

    ////////////////////////////////////////////////////////////
    // Arithmetic operators

    template<typename Iterator>
    auto operator+(quad_iterator<Iterator> it, std::size_t size)
        -> quad_iterator<Iterator>
    {
        return it += size;
    }

    template<typename Iterator>
    auto operator+(std::size_t size, quad_iterator<Iterator> it)
        -> quad_iterator<Iterator>
    {
        return it += size;
    }

    template<typename Iterator>
    auto operator-(quad_iterator<Iterator> it, std::size_t size)
        -> quad_iterator<Iterator>
    {
        return it -= size;
    }

    template<typename Iterator>
    auto operator-(const quad_iterator<Iterator>& lhs, const quad_iterator<Iterator>& rhs)
        -> std::size_t
    {
        return (lhs.base() - rhs.base()) / lhs.size();
    }

    ////////////////////////////////////////////////////////////
    // Construction function

    template<typename Iterator>
    auto make_quad_iterator(Iterator it, std::size_t size)
        -> quad_iterator<Iterator>
    {
        return { it, size };
    }

    template<typename Iterator>
    auto make_quad_iterator(quad_iterator<Iterator> it, std::size_t size)
        -> quad_iterator<Iterator>
    {
        return { it.base(), size * it.size() };
    }

    ////////////////////////////////////////////////////////////
    // Nodes used by the sorting algorithm

    template<typename Iterator>
    struct node
    {
        Iterator it;
        typename std::list<node>::iterator prev;
        typename std::list<node>::iterator next;
        bool sorted;
    };

    template<typename T>
    auto operator<(const node<T>& lhs, const node<T>& rhs)
        -> bool
    {
        return *(lhs.it) < *(rhs.it);
    }

    ////////////////////////////////////////////////////////////
    // Actual sorting algorithms

    template<typename InputIterator, typename OutputIterator>
    auto to_cache(InputIterator first, InputIterator last, OutputIterator out,
                  std::false_type)
        -> void
    {
        for (auto it = first ; it != last ; ++it)
        {
            *out++ = std::move(**it);
        }
    }

    template<typename InputIterator, typename OutputIterator>
    auto to_cache(InputIterator first, InputIterator last, OutputIterator out,
                  std::true_type)
        -> void
    {
        for (auto it = first ; it != last ; ++it)
        {
            auto begin = it->base();
            auto end = it->base() + it->size();
            std::move(begin, end, out);
        }
    }

    template<typename InputIterator, typename OutputIterator>
    auto from_cache(InputIterator first, InputIterator last, OutputIterator out,
                    std::false_type)
        -> void
    {
        std::move(first, last, out);
    }

    template<typename InputIterator, typename OutputIterator>
    auto from_cache(InputIterator first, InputIterator last, OutputIterator out,
                    std::true_type)
        -> void
    {
        std::move(first, last, out.base());
    }

    template<typename T>
    struct is_quad_it:
        std::false_type
    {};

    template<typename Iterator>
    struct is_quad_it<quad_iterator<Iterator>>:
        std::true_type
    {};

    template<
        typename RandomAccessIterator,
        typename Compare,
        typename Projection
    >
    auto merge_insertion_sort(RandomAccessIterator first, RandomAccessIterator last,
                              Compare compare, Projection projection)
    {
        // Cache all the Jacobsthal numbers that fit in 64 bits
        static constexpr std::uint_least64_t jacobsthal[] = {
            1u, 1u, 3u, 5u, 11u, 21u, 43u, 85u, 171u, 341u, 683u, 1365u,
            2731u, 5461u, 10923u, 21845u, 43691u, 87381u, 174763u, 349525u,
            699051u, 1398101u, 2796203u, 5592405u, 11184811u, 22369621u,
            44739243u, 89478485u, 178956971u, 357913941u, 715827883u,
            1431655765u, 2863311531u, 5726623061u, 11453246123u, 22906492245u,
            45812984491u, 91625968981u, 183251937963u, 366503875925u, 733007751851u,
            1466015503701u, 2932031007403u, 5864062014805u, 11728124029611u, 23456248059221u,
            46912496118443u, 93824992236885u, 187649984473771u, 375299968947541u,
            750599937895083u, 1501199875790165u, 3002399751580330u, 6004799503160661u,
            12009599006321322u, 24019198012642644u, 48038396025285288u, 96076792050570576u,
            192153584101141152u, 384307168202282304u, 768614336404564608u, 1537228672809129216u,
            3074457345618258432u, 6148914691236516864u, 12297829382473033728u
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
            make_quad_iterator(first, 2),
            make_quad_iterator(end, 2),
            compare, projection
        );

        ////////////////////////////////////////////////////////////
        // Separate main chain and pend elements

        // Small node struct for pend elements
        struct node
        {
            RandomAccessIterator it;
            typename std::list<RandomAccessIterator>::iterator next;
            std::size_t value;
        };

        std::list<RandomAccessIterator> chain;
        std::list<node> pend;

        std::size_t count = 0;
        for (auto it = first ; it != end ; it += 2)
        {
            auto tmp = chain.insert(chain.end(), std::next(it));
            pend.push_back({it, tmp, ++count});
        }

        // Add the last element to pend if it exists
        if (has_stray)
        {
            pend.push_back({end, chain.end(), ++count});
        }

        // Move first element of pend in the main chain
        chain.push_front(pend.front().it);
        pend.pop_front();

        ////////////////////////////////////////////////////////////
        // Binary insertion into the main chain

        for (int k = 2 ; ; ++k)
        {
            // Find next index
            auto index = jacobsthal[k];
            auto it = pend.begin();
            while (it != pend.end() && it->value != index) ++it;

            if (it == pend.end()) break;

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

        // Move elements to new vector in chain order, move them back
        std::vector<typename std::iterator_traits<RandomAccessIterator>::value_type> cache;
        cache.reserve(size);

        using is_quad = is_quad_it<RandomAccessIterator>;
        to_cache(chain.begin(), chain.end(), std::back_inserter(cache), is_quad{});
        from_cache(cache.begin(), cache.end(), first, is_quad{});
    }
}}

#endif // CPPSORT_DETAIL_MERGE_INSERTION_SORT_H_
