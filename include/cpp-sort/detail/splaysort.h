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
    struct splay_tree_node_base
    {
        explicit splay_tree_node_base(splay_tree_node_base* parent) noexcept:
            parent(parent)
        {}

        explicit splay_tree_node_base(splay_tree_node_base* parent,
                                      splay_tree_node_base* left_child,
                                      splay_tree_node_base* right_child) noexcept:
            parent(parent),
            left_child(left_child),
            right_child(right_child)
        {}

        // Make tree nodes immovable
        splay_tree_node_base(const splay_tree_node_base&) = delete;
        splay_tree_node_base(splay_tree_node_base&&) = delete;
        splay_tree_node_base& operator=(const splay_tree_node_base&) = delete;
        splay_tree_node_base& operator=(splay_tree_node_base&&) = delete;

        // Parent node
        splay_tree_node_base* parent = nullptr;
        // Children nodes
        splay_tree_node_base* left_child = nullptr;
        splay_tree_node_base* right_child = nullptr;
    };

    template<typename T>
    struct splay_tree_node:
        splay_tree_node_base
    {
        constexpr splay_tree_node(T&& value, splay_tree_node_base* parent)
            noexcept(std::is_nothrow_move_constructible<T>::value):
            splay_tree_node_base(parent),
            value(std::move(value))
        {}

        constexpr splay_tree_node(T&& value, splay_tree_node_base* parent,
                                  splay_tree_node_base* left_child,
                                  splay_tree_node_base* right_child)
            noexcept(std::is_nothrow_move_constructible<T>::value):
            splay_tree_node_base(parent, left_child, right_child),
            value(std::move(value))
        {}

        // Stored value
        T value;
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
                sentinel_node_(nullptr),
                buffer_(size),
                root_(buffer_.begin()) // Original root is first element
            {
                using utility::iter_move;

                // Create the first node
                buffer_.emplace_back(iter_move(first), &sentinel_node_, &sentinel_node_, &sentinel_node_);
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
                splay_tree_node_base* prev = root();
                splay_tree_node_base* curr = nullptr;
                if (prev->left_child != &sentinel_node_) {
                    curr = prev->left_child;
                } else {
                    *out = std::move(static_cast<node_type*>(prev)->value);
                    ++out;
                    curr = prev->right_child;
                    curr->parent = &sentinel_node_;
                }

                bool from_parent = true;
                while (true) {
                    if (from_parent && curr->left_child != &sentinel_node_) {
                        prev = std::exchange(curr, curr->left_child);
                        continue;
                    }

                    *out = std::move(static_cast<node_type*>(curr)->value);
                    ++out;

                    if (curr->right_child != &sentinel_node_) {
                        from_parent = true;
                        prev = std::exchange(curr, curr->right_child);
                        curr->parent = prev->parent;
                        prev->right_child = &sentinel_node_;
                    } else if (curr->parent != &sentinel_node_) {
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

                splay_tree_node_base* node = root();
                CPPSORT_ASSERT(node != &sentinel_node_);
                while (true) {
                    if (comp(value_proj, proj(static_cast<node_type*>(node)->value))) {
                        if (node->left_child == &sentinel_node_) {
                            node->left_child = buffer_.emplace_back(iter_move(it), node, &sentinel_node_, &sentinel_node_);
                            splay(node->left_child);
                            break;
                        }
                        node = node->left_child;
                    } else {
                        if (node->right_child == &sentinel_node_) {
                            node->right_child = buffer_.emplace_back(iter_move(it), node, &sentinel_node_, &sentinel_node_);
                            splay(node->right_child);
                            break;
                        }
                        node = node->right_child;
                    }
                }
            }

            auto rotate_left(splay_tree_node_base* node)
                -> void
            {
                auto parent = node->parent;
                auto grand_parent = parent->parent;
                parent->right_child = std::exchange(node->left_child, parent);
                // Unconditional backlink thanks to the sentinel node
                parent->right_child->parent = parent;
                node->parent = std::exchange(parent->parent, node);

                if (grand_parent->left_child == parent) {
                    grand_parent->left_child = node;
                } else {
                    // This branch should always be taken when grand_parent
                    // is the sentinel node
                    grand_parent->right_child = node;
                }
            }

            auto rotate_right(splay_tree_node_base* node)
                -> void
            {
                auto parent = node->parent;
                auto grand_parent = parent->parent;
                parent->left_child = std::exchange(node->right_child, parent);
                // Unconditional backlink thanks to the sentinel node
                parent->left_child->parent = parent;
                node->parent = std::exchange(parent->parent, node);

                if (grand_parent->left_child == parent) {
                    grand_parent->left_child = node;
                } else {
                    // This branch should always be taken when grand_parent
                    // is the sentinel node
                    grand_parent->right_child = node;
                }
            }

            auto splay(splay_tree_node_base* node)
                -> void
            {
                auto parent = node->parent;
                CPPSORT_ASSERT(parent != &sentinel_node_);
                do {
                    auto grand_parent = parent->parent;
                    if (grand_parent == &sentinel_node_) {
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
                } while (parent != &sentinel_node_);
                root_ = static_cast<node_type*>(node);
            }

            ////////////////////////////////////////////////////////////
            // Data members

            // Sentinel node: it doesn't matter where it point to, this
            // node only exists to reduce branching in some operations
            splay_tree_node_base sentinel_node_;

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
