/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018-2021 Morwenn
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
#ifndef CPPSORT_DETAIL_MELSORT_H_
#define CPPSORT_DETAIL_MELSORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <vector>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/iter_move.h>
#include "fixed_size_list.h"
#include "iterator_traits.h"
#include "merge_move.h"
#include "move.h"
#include "type_traits.h"

namespace cppsort
{
namespace detail
{
    template<typename ForwardIterator, typename Compare, typename Projection>
    auto melsort(ForwardIterator first, ForwardIterator last,
                 difference_type_t<ForwardIterator> size,
                 Compare compare, Projection projection)
        -> void
    {
        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        // Encroaching lists
        using rvalue_reference = remove_cvref_t<rvalue_reference_t<ForwardIterator>>;
        fixed_size_list_node_pool<rvalue_reference> node_pool(size);
        std::vector<fixed_size_list<rvalue_reference>> lists;

        ////////////////////////////////////////////////////////////
        // Create encroaching lists

        for (auto it = first ; it != last ; ++it) {
            auto&& value = proj(*it);

            // Binary search for an encroaching list where
            // value >= list.second or value <= list.first

            // Whether the found value is smaller than the head
            // of the found encroaching list or greater than its
            // tail
            bool value_is_smaller = false;

            auto size = lists.size();
            auto enc_it = lists.begin();
            while (size > 0) {
                auto tmp_it = enc_it;
                std::advance(tmp_it, size / 2);
                if (not comp(value, proj(tmp_it->back()))) {
                    size /= 2;
                    value_is_smaller = false;
                } else if (not comp(proj(tmp_it->front()), value)) {
                    size /= 2;
                    value_is_smaller = true;
                } else {
                    enc_it = ++tmp_it;
                    size -= size / 2 + 1;
                }
            }

            using utility::iter_move;
            if (enc_it != lists.end()) {
                // Add the new element to the head or tail or the most
                // suitable encroaching list if any has been found
                if (value_is_smaller) {
                    enc_it->push_front(iter_move(it));
                } else {
                    enc_it->push_back(iter_move(it));
                }
            } else {
                // Create a new encroaching list if the element
                // didn't fit in any of the existing ones
                lists.emplace_back(node_pool);
                lists.back().push_back(iter_move(it));
            }
        }

        ////////////////////////////////////////////////////////////
        // Merge encroaching lists

        while (lists.size() > 2) {
            if (lists.size() % 2 != 0) {
                auto last_it = std::prev(lists.end());
                auto last_1_it = std::prev(last_it);
                last_1_it->merge(*last_it, compare, projection);
                lists.pop_back();
            }

            auto first_it = lists.begin();
            auto half_it = first_it + lists.size() / 2;
            while (half_it != lists.end()) {
                first_it->merge(*half_it, compare, projection);
                ++first_it;
                ++half_it;
            }

            lists.erase(lists.begin() + lists.size() / 2, lists.end());
        }

        // Merge lists back into the original collection

        if (lists.size() == 2) {
            detail::merge_move(
                lists.front().begin(), lists.front().end(),
                lists.back().begin(), lists.back().end(),
                first, compare, projection, projection
            );
        } else if (lists.size() == 1) {
            detail::move(lists.front().begin(), lists.front().end(), first);
        }
    }
}}


#endif // CPPSORT_DETAIL_MELSORT_H_
