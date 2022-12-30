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
#include "binary_tree.h"
#include "config.h"
#include "immovable_vector.h"
#include "iterator_traits.h"

namespace cppsort
{
namespace detail
{
    struct splay_tree_base
    {
        public:

            constexpr splay_tree_base() noexcept:
                sentinel_node_(nullptr, nullptr, nullptr)
            {}

            auto rotate_left(binary_tree_node_base* node) noexcept
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

            auto rotate_right(binary_tree_node_base* node) noexcept
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

            auto splay(binary_tree_node_base* node) noexcept
                -> void
            {
                // Note: we don't store a root here, so this splay
                //       operation does not reassign the root

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
            }

        protected:

            // Sentinel node: it doesn't matter where it point to, this
            // node only exists to reduce branching in some operations
            binary_tree_node_base sentinel_node_;
    };

    template<typename T>
    struct splay_tree:
        splay_tree_base
    {
        public:

            ////////////////////////////////////////////////////////////
            // Public types

            using difference_type = std::ptrdiff_t;
            using node_type = binary_tree_node<T>;

            ////////////////////////////////////////////////////////////
            // Constructor

            template<typename ForwardIterator, typename Compare, typename Projection>
            explicit splay_tree(ForwardIterator first, ForwardIterator last,
                                difference_type_t<ForwardIterator> size,
                                Compare compare, Projection projection):
                splay_tree_base(),
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

            auto root() const noexcept
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
                binary_tree_node_base* curr = root();
                while (curr->left_child != &sentinel_node_) {
                    curr = curr->left_child;
                }
                *out = std::move(static_cast<node_type*>(curr)->value);
                ++out;

                while (true) {
                    if (curr->right_child != &sentinel_node_) {
                        auto prev = std::exchange(curr, curr->right_child);
                        curr->parent = prev->parent;
                        prev->right_child = &sentinel_node_;
                        while (curr->left_child != &sentinel_node_) {
                            curr = curr->left_child;
                        }
                    } else if (curr->parent != &sentinel_node_) {
                        curr = curr->parent;
                    } else {
                        return;
                    }

                    *out = std::move(static_cast<node_type*>(curr)->value);
                    ++out;
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

                binary_tree_node_base* node = root();
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

            auto splay(binary_tree_node_base* node) noexcept
                -> void
            {
                splay_tree_base::splay(node);
                root_ = static_cast<node_type*>(node);
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
