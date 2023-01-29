/*
 * Copyright (c) 2022-2023 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_BINARY_TREE_H_
#define CPPSORT_DETAIL_BINARY_TREE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <type_traits>
#include <utility>

namespace cppsort::detail
{
    struct binary_tree_node_base
    {
        constexpr binary_tree_node_base(binary_tree_node_base* parent,
                                        binary_tree_node_base* left_child,
                                        binary_tree_node_base* right_child) noexcept:
            parent(parent),
            left_child(left_child),
            right_child(right_child)
        {}

        // Make tree nodes immovable
        binary_tree_node_base(const binary_tree_node_base&) = delete;
        binary_tree_node_base(binary_tree_node_base&&) = delete;
        binary_tree_node_base& operator=(const binary_tree_node_base&) = delete;
        binary_tree_node_base& operator=(binary_tree_node_base&&) = delete;

        // Parent node
        binary_tree_node_base* parent;
        // Children nodes
        binary_tree_node_base* left_child;
        binary_tree_node_base* right_child;
    };

    template<typename T>
    struct binary_tree_node:
        binary_tree_node_base
    {
        constexpr binary_tree_node(T&& value, binary_tree_node_base* parent,
                                   binary_tree_node_base* left_child = nullptr,
                                   binary_tree_node_base* right_child = nullptr)
            noexcept(std::is_nothrow_move_constructible<T>::value):
            binary_tree_node_base(parent, left_child, right_child),
            value(std::move(value))
        {}

        // Stored value
        T value;
    };
}

#endif // CPPSORT_DETAIL_BINARY_TREE_H_
