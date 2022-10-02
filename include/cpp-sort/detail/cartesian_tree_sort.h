/*
 * Copyright (c) 2021-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_CARTESIAN_TREE_SORT_H_
#define CPPSORT_DETAIL_CARTESIAN_TREE_SORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <type_traits>
#include <utility>
#include <vector>
#include <cpp-sort/comparators/flip.h>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/iter_move.h>
#include "../detail/heapsort.h"
#include "../detail/immovable_vector.h"
#include "../detail/iterator_traits.h"
#include "../detail/type_traits.h"

namespace cppsort
{
namespace detail
{
    template<typename T>
    struct cartesian_tree_node
    {
        constexpr cartesian_tree_node(T&& value, cartesian_tree_node* parent, cartesian_tree_node* left_child)
            noexcept(std::is_nothrow_move_constructible<T>::value):
            value(std::move(value)),
            parent(parent),
            left_child(left_child)
        {}

        // Make tree nodes immovable
        cartesian_tree_node(const cartesian_tree_node&) = delete;
        cartesian_tree_node(cartesian_tree_node&&) = delete;
        cartesian_tree_node& operator=(const cartesian_tree_node&) = delete;
        cartesian_tree_node& operator=(cartesian_tree_node&&) = delete;

        // Stored value
        T value;

        // Parent node
        cartesian_tree_node* parent = nullptr;
        // Children nodes
        cartesian_tree_node* left_child = nullptr;
        cartesian_tree_node* right_child = nullptr;
    };

    template<typename T>
    struct cartesian_tree
    {
        public:

            ////////////////////////////////////////////////////////////
            // Public types

            using difference_type = std::ptrdiff_t;
            using node_type = cartesian_tree_node<T>;

            ////////////////////////////////////////////////////////////
            // Constructor

            template<typename ForwardIterator, typename Compare, typename Projection>
            explicit cartesian_tree(ForwardIterator first, ForwardIterator last,
                                    difference_type_t<ForwardIterator> size,
                                    Compare compare, Projection projection):
                buffer_(size),
                root_(buffer_.begin()) // Original root is first element
            {
                using utility::iter_move;
                auto&& proj = utility::as_function(projection);

                // Pointer to the last node that was inserted
                node_type* prev_node = buffer_.begin();
                // Create the first node
                buffer_.emplace_back(iter_move(first), nullptr, nullptr);

                // Advance to the next element
                ++first;

                for (; first != last ; ++first) {
                    auto&& proj_value = proj(*first);
                    node_type* new_parent = _find_insertion_parent(prev_node, proj_value, compare, projection);
                    if (new_parent == nullptr) {
                        root_ = buffer_.emplace_back(iter_move(first), nullptr, root_);
                    } else {
                        new_parent->right_child = buffer_.emplace_back(iter_move(first), new_parent,
                                                                       new_parent->right_child);
                    }
                    prev_node = &buffer_.back();
                }
            }

            ////////////////////////////////////////////////////////////
            // Accessors

            auto root() const
                -> node_type*
            {
                return root_;
            }

        private:

            ////////////////////////////////////////////////////////////
            // Helper function

            template<typename U, typename Compare, typename Projection>
            auto _find_insertion_parent(node_type* prev_node, U&& value,
                                        Compare compare, Projection projection) const
                -> node_type*
            {
                auto&& comp = utility::as_function(compare);
                auto&& proj = utility::as_function(projection);

                while (comp(value, proj(prev_node->value))) {
                    if (prev_node == root_) {
                        return nullptr;
                    }
                    prev_node = prev_node->parent;
                }
                return prev_node;
            }

            ////////////////////////////////////////////////////////////
            // Data members

            // Backing storage
            immovable_vector<node_type> buffer_;
            // Root of the Cartesian tree
            node_type* root_;
    };

    template<typename ForwardIterator, typename Compare, typename Projection>
    auto cartesian_tree_sort(ForwardIterator first, ForwardIterator last,
                             difference_type_t<ForwardIterator> size,
                             Compare compare, Projection projection)
        -> void
    {
        using tree_type = cartesian_tree<rvalue_type_t<ForwardIterator>>;
        using node_type = typename tree_type::node_type;

        if (size < 2) {
            return;
        }

        tree_type tree(first, last, size, compare, projection);
        std::vector<node_type*> pq; // Priority queue
        pq.push_back(tree.root());

        auto&& comp = cppsort::flip(compare);
        auto proj_value = [proj=utility::as_function(projection)](auto* node) -> decltype(auto) {
            return proj(node->value);
        };

        while (not pq.empty()) {
            // Retrieve biggest element
            node_type* node = pq.front();

            // Add the element back to the original collection
            *first = std::move(node->value);
            ++first;

            // Add the node's children to the priority queue
            if (node->left_child != nullptr) {
                pq.front() = node->left_child;
                detail::sift_down(pq.begin(), pq.end(), comp, proj_value, pq.size(), pq.begin());
                if (node->right_child != nullptr) {
                    pq.push_back(node->right_child);
                    detail::push_heap(pq.begin(), pq.end(), comp, proj_value, pq.size());
                }
            } else if (node->right_child != nullptr) {
                pq.front() = node->right_child;
                detail::sift_down(pq.begin(), pq.end(), comp, proj_value, pq.size(), pq.begin());
            } else {
                detail::pop_heap(pq.begin(), pq.end(), comp, proj_value, pq.size());
                pq.pop_back();
            }
        }
    }
}}

#endif // CPPSORT_DETAIL_CARTESIAN_TREE_SORT_H_
