/*
 * Copyright (c) 2018-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_MELSORT_H_
#define CPPSORT_DETAIL_MELSORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cmath>
#include <iterator>
#include <vector>
#include <cpp-sort/comparators/flip.h>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/iter_move.h>
#include "fixed_size_list.h"
#include "iterator_traits.h"
#include "lower_bound.h"
#include "merge_move.h"
#include "move.h"
#include "type_traits.h"

namespace cppsort
{
namespace detail
{
    template<typename ForwardIterator, typename NodeType, typename Compare, typename Projection>
    auto merge_encroaching_lists(std::vector<fixed_size_list<NodeType>>& lists,
                                 ForwardIterator first, bool extract_edges,
                                 Compare compare, Projection projection)
    {
        // This part of the algorithm is an optimization which is not described in
        // the original paper, but somewhat hinted at nonetheless: the heads of the
        // encroaching lists form a sorted collection, and the tails form another
        // collection sorted in descending order. When those collections are big
        // enough, it is more efficient to split them in their own list and merge
        // them back with the other lists later.
        //
        // Since the biggest of the minimums is not bigger than the smallest of the
        // maximums, we can just add the minimums to the new list, followed directly
        // by the maximum in reverse order, without having to perform a merge at all.
        //
        // WARNING: when making the edges list, only the next pointer of its nodes is
        //          correctly set, as well as the prev pointer of the sentinel node.
        //          All other prev pointers in the list aren't guaranteed to be valid.
        //          This is cursed but saves some operations.

        fixed_size_list<NodeType> edges(lists.front().node_pool());

        if (extract_edges) {
            list_node_base* insert_node = edges.end().base();
            // Add minimums of encroaching lists
            for (auto& list : lists) {
                insert_node->next = list.extract_front();
                insert_node = insert_node->next;
            }
            // Only the last node can have a single element
            // and end up empty after the previous step
            if (lists.back().is_empty()) {
                lists.pop_back();
            }
            // Add maximums of encroaching lists
            for (auto it = lists.rbegin() ; it != lists.rend() ; ++it) {
                auto& list = *it;
                insert_node->next = list.extract_back();
                insert_node = insert_node->next;
            }
            insert_node->next = edges.end().base();
            // Proper link back from the sentinel node
            insert_node->next->prev = insert_node;

            // Remove the empty lists to help with the merges
            lists.erase(std::remove_if(lists.begin(), lists.end(), [](auto& list) {
                return list.is_empty();
            }), lists.end());
        }

        // Merge lists pairwise, picking a list from each half
        // of the collection instead of adjacent ones
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

        // If the edges were split into a single sorted list,
        // add it back to the collection of encroaching lists
        if (extract_edges) {
            if (lists.empty()) {
                lists.push_back(std::move(edges));
            } else {
                lists.back().merge_unsafe(edges, compare, projection);
            }
        }

        // Merge remaining list(s) back into the original collection
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

    template<typename ForwardIterator, typename Compare, typename Projection>
    auto melsort(ForwardIterator first, ForwardIterator last,
                 difference_type_t<ForwardIterator> size,
                 Compare compare, Projection projection)
        -> void
    {
        using utility::iter_move;
        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        if (first == last || std::next(first) == last) {
            return;
        }

        // Encroaching lists
        using node_type = list_node<rvalue_type_t<ForwardIterator>>;
        fixed_size_list_node_pool<node_type> node_pool(size);
        std::vector<fixed_size_list<node_type>> lists;
        // Ensure that there is always one list and that the last list
        // always has at least one element, this simplifies the rest
        // of the computations
        lists.emplace_back(node_pool);
        lists.back().push_back(iter_move(first));

        ////////////////////////////////////////////////////////////
        // Create encroaching lists

        for (auto it = std::next(first) ; it != last ; ++it) {
            auto&& value = proj(*it);

            // The heads of the lists form an ascending collection while the heads
            // form a descending collection, plus the we have the guarantee that the
            // last lists contains the biggest of the heads and the smallest of the
            // tails. Since the biggest of the heads is smaller than the smallest of
            // the tails, we only need to compare the element to insert to the head
            // and tail of the last list to know whether we need to add it to the
            // heads, to the tails, or to its own new list.
            //
            // The search is done with lower_bound to ensure that the element will
            // be added to the oldest suitable list, which is a necessary condition
            // to keep the guarantees provided by the encroaching lists collection.

            auto& last_list = lists.back();
            if (not comp(value, proj(last_list.back()))) {
                // Element belongs to the tails (bigger elements)
                auto insertion_point = detail::lower_bound(
                    lists.begin(), std::prev(lists.end()), value, cppsort::flip(compare),
                    [&proj](auto& list) -> decltype(auto) { return proj(list.back()); }
                );
                insertion_point->push_back(iter_move(it));
            } else if (not comp(proj(last_list.front()), value)) {
                // Element belongs to the heads (smaller elements)
                auto insertion_point = detail::lower_bound(
                    lists.begin(), std::prev(lists.end()), value, compare,
                    [&proj](auto& list) -> decltype(auto) { return proj(list.front()); }
                );
                insertion_point->push_front(iter_move(it));
            } else {
                // Element does not belong to the existing encroaching lists,
                // create a new list for it
                lists.emplace_back(node_pool);
                lists.back().push_back(iter_move(it));
            }
        }

        ////////////////////////////////////////////////////////////
        // Merge encroaching lists

        // This heuristic is selected so that the edge extraction
        // optimization only occurs when the number of encroaching
        // lists is greater than the average size of the encroaching
        // lists to merge
        bool bad_distribution = lists.size() > std::sqrt(size);
        merge_encroaching_lists(lists, first, bad_distribution,
                                std::move(compare), std::move(projection));
    }
}}

#endif // CPPSORT_DETAIL_MELSORT_H_
