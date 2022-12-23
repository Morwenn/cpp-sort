/*
 * Copyright (c) 2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_SPLAYSORT_H_
#define CPPSORT_DETAIL_SPLAYSORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <type_traits>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/iter_move.h>
#include "config.h"
#include "immovable_vector.h"
#include "iterator_traits.h"

namespace cppsort
{
namespace detail
{
    template<typename T>
    struct splay_tree_node
    {
        constexpr splay_tree_node(T&& value, splay_tree_node* parent)
            noexcept(std::is_nothrow_move_constructible<T>::value):
            value(std::move(value)),
            parent(parent)
        {}

        // Make tree nodes immovable
        splay_tree_node(const splay_tree_node&) = delete;
        splay_tree_node(splay_tree_node&&) = delete;
        splay_tree_node& operator=(const splay_tree_node&) = delete;
        splay_tree_node& operator=(splay_tree_node&&) = delete;

        // Stored value
        T value;

        // Parent node
        splay_tree_node* parent = nullptr;
        // Children nodes
        splay_tree_node* left_child = nullptr;
        splay_tree_node* right_child = nullptr;
    };

    template<typename T>
    struct splay_tree
    {
        public:

            ////////////////////////////////////////////////////////////
            // Public types

            using difference_type = std::ptrdiff_t;
            using node_type = splay_tree_node<T>;

            ////////////////////////////////////////////////////////////
            // Constructor

            template<typename ForwardIterator, typename Compare, typename Projection>
            explicit splay_tree(ForwardIterator first, ForwardIterator last,
                                difference_type_t<ForwardIterator> size,
                                Compare compare, Projection projection):
                buffer_(size),
                root_(buffer_.begin()) // Original root is first element
            {
                using utility::iter_move;

                // Create the first node
                buffer_.emplace_back(iter_move(first), nullptr);
                // Advance to the next element
                ++first;

                for (; first != last; ++first) {
                    insert(first, compare, projection);
                }
            }

            ////////////////////////////////////////////////////////////
            // Accessors

            auto root() const
                -> node_type*
            {
                return root_;
            }

            ////////////////////////////////////////////////////////////
            // Destroying move

            template<typename OutputIterator>
            auto move_to(OutputIterator out)
                -> void
            {
                node_type* prev = root();
                node_type* curr = nullptr;
                if (prev->left_child) {
                    curr = prev->left_child;
                } else {
                    *out = std::move(prev->value);
                    ++out;
                    curr = prev->right_child;
                    curr->parent = nullptr;
                }

                bool from_parent = true;
                while (true) {
                    if (from_parent && curr->left_child) {
                        prev = std::exchange(curr, curr->left_child);
                        continue;
                    }

                    *out = std::move(curr->value);
                    ++out;

                    if (curr->right_child) {
                        from_parent = true;
                        prev = std::exchange(curr, curr->right_child);
                        curr->parent = prev->parent;
                        prev->right_child = nullptr;
                    } else if (curr->parent) {
                        from_parent = false;
                        prev = std::exchange(curr, curr->parent);
                    } else {
                        return;
                    }
                }
            }

        private:

            ////////////////////////////////////////////////////////////
            // Helper functions

            template<typename ForwardIterator, typename Compare, typename Projection>
            auto insert(ForwardIterator it, Compare compare, Projection projection)
                -> void
            {
                using utility::iter_move;
                auto&& comp = utility::as_function(compare);
                auto&& proj = utility::as_function(projection);

                auto&& value_proj = proj(*it);

                auto node = root();
                CPPSORT_ASSERT(node != nullptr);
                while (true) {
                    if (comp(value_proj, proj(node->value))) {
                        if (not node->left_child) {
                            node->left_child = buffer_.emplace_back(iter_move(it), node);
                            splay(node->left_child);
                            break;
                        }
                        node = node->left_child;
                    } else {
                        if (not node->right_child) {
                            node->right_child = buffer_.emplace_back(iter_move(it), node);
                            splay(node->right_child);
                            break;
                        }
                        node = node->right_child;
                    }
                }
            }

            auto rotate_left(node_type* node)
                -> void
            {
                auto parent = node->parent;
                auto grand_parent = parent->parent;
                parent->right_child = std::exchange(node->left_child, parent);
                if (parent->right_child) {
                    parent->right_child->parent = parent;
                }
                node->parent = std::exchange(parent->parent, node);

                if (grand_parent) {
                    if (grand_parent->left_child == parent) {
                        grand_parent->left_child = node;
                    } else {
                        grand_parent->right_child = node;
                    }
                }
            }

            auto rotate_right(node_type* node)
                -> void
            {
                auto parent = node->parent;
                auto grand_parent = parent->parent;
                parent->left_child = std::exchange(node->right_child, parent);
                if (parent->left_child) {
                    parent->left_child->parent = parent;
                }
                node->parent = std::exchange(parent->parent, node);

                if (grand_parent) {
                    if (grand_parent->left_child == parent) {
                        grand_parent->left_child = node;
                    } else {
                        grand_parent->right_child = node;
                    }
                }
            }

            auto splay(node_type* node)
                -> void
            {
                auto parent = node->parent;
                CPPSORT_ASSERT(parent != nullptr);
                do {
                    auto grand_parent = parent->parent;
                    if (not grand_parent) {
                        if (node == parent->left_child) {
                            // zig
                            rotate_right(node);
                        } else {
                            // zag
                            rotate_left(node);
                        }
                    } else {
                        if (parent == grand_parent->left_child) {
                            if (node == parent->left_child) {
                                // zig-zig
                                rotate_right(parent);
                                rotate_right(node);
                            } else {
                                // zig-zag
                                rotate_left(node);
                                rotate_right(node);
                            }
                        } else {
                            if (node == parent->left_child) {
                                // zag-zig
                                rotate_right(node);
                                rotate_left(node);
                            } else {
                                // zag-zag
                                rotate_left(parent);
                                rotate_left(node);
                            }
                        }
                    }
                    parent = node->parent;
                } while (parent != nullptr);
                root_ = node;
            }

            ////////////////////////////////////////////////////////////
            // Data members

            // Backing storage
            immovable_vector<node_type> buffer_;
            // Root of the splay tree
            node_type* root_;
    };

    template<typename ForwardIterator, typename Compare, typename Projection>
    auto splaysort(ForwardIterator first, ForwardIterator last,
                   difference_type_t<ForwardIterator> size,
                   Compare compare, Projection projection)
        -> void
    {
        if (size < 2) {
            return;
        }

        using rvalue_type = rvalue_type_t<ForwardIterator>;
        splay_tree<rvalue_type> tree(
            first, last, size,
            std::move(compare), std::move(projection)
        );

        tree.move_to(first);
    }
}}

#endif // CPPSORT_DETAIL_SPLAYSORT_H_
