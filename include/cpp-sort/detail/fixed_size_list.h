/*
 * Copyright (c) 2020-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_FIXED_SIZE_LIST_H_
#define CPPSORT_DETAIL_FIXED_SIZE_LIST_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <iterator>
#include <memory>
#include <utility>
#include "config.h"
#include "memory.h"

namespace cppsort
{
namespace detail
{
    ////////////////////////////////////////////////////////////
    // List node
    //
    // A list node is an immovable structure which contains a
    // value and pointers to the previous and next nodes if any.
    // The value is declared in an anonymous union to ensure
    // that it can be left unconstructed as needed: the list
    // structure itself handles whether a node value should be
    // constructed or destructed.

    template<typename T>
    struct fixed_size_list_node
    {
        constexpr explicit fixed_size_list_node(fixed_size_list_node* next) noexcept:
            next(next)
        {}

        constexpr fixed_size_list_node(fixed_size_list_node* prev, fixed_size_list_node* next) noexcept:
            prev(prev),
            next(next)
        {}

        // Make list nodes immovable
        fixed_size_list_node(const fixed_size_list_node&) = delete;
        fixed_size_list_node(fixed_size_list_node&&) = delete;
        fixed_size_list_node& operator=(const fixed_size_list_node&) = delete;
        fixed_size_list_node& operator=(fixed_size_list_node&&) = delete;

        // The list takes care of managing the lifetime of value
        ~fixed_size_list_node() {}

        // Inhibit construction of the node with a value
        union { T value; };

        // Pointers to the previous and next nodes
        fixed_size_list_node* prev;
        fixed_size_list_node* next;
    };

    ////////////////////////////////////////////////////////////
    // List iterator
    //
    // Iterator wrapping a fixed_size_list_node: it uses the
    // node's next & prev pointers to advance through the list

    template<typename T>
    struct fixed_size_list_iterator
    {
        public:

            ////////////////////////////////////////////////////////////
            // Public types

            using iterator_category = std::bidirectional_iterator_tag;
            using value_type        = T;
            using difference_type   = std::ptrdiff_t;
            using pointer           = T*;
            using reference         = T&;

            ////////////////////////////////////////////////////////////
            // Constructors

            constexpr explicit fixed_size_list_iterator(fixed_size_list_node<T>* ptr) noexcept:
                ptr_(ptr)
            {}

            ////////////////////////////////////////////////////////////
            // Members access

            constexpr auto base() const
                -> fixed_size_list_node<T>*
            {
                return ptr_;
            }

            ////////////////////////////////////////////////////////////
            // Element access

            auto operator*() const
                -> reference
            {
                return ptr_->value;
            }

            auto operator->() const
                -> pointer
            {
                return &(operator*());
            }

            ////////////////////////////////////////////////////////////
            // Increment/decrement operators

            auto operator++()
                -> fixed_size_list_iterator&
            {
                ptr_ = ptr_->next;
                return *this;
            }

            auto operator++(int)
                -> fixed_size_list_iterator
            {
                auto tmp = *this;
                operator++();
                return tmp;
            }

            auto operator--()
                -> fixed_size_list_iterator&
            {
                ptr_ = ptr_->prev;
                return *this;
            }

            auto operator--(int)
                -> fixed_size_list_iterator
            {
                auto tmp = *this;
                operator--();
                return tmp;
            }

            ////////////////////////////////////////////////////////////
            // Comparison operators

            constexpr
            friend auto operator==(const fixed_size_list_iterator& lhs, const fixed_size_list_iterator& rhs)
                -> bool
            {
                return lhs.base() == rhs.base();
            }

            constexpr
            friend auto operator!=(const fixed_size_list_iterator& lhs, const fixed_size_list_iterator& rhs)
                -> bool
            {
                return lhs.base() != rhs.base();
            }

        private:

            fixed_size_list_node<T>* ptr_;
    };

    ////////////////////////////////////////////////////////////
    // Fixed size list
    //
    // This is an immovable double linked list data structure that
    // can be constructed with a max capacity (N thereafter): it
    // allocates a contiguous block of memory for N nodes and a
    // sentinel node and never performs any other allocation through
    // the course of its lifetime. It is meant to be used from
    // algorithms which know how many elements will be needed in a
    // list and allow to reuse the list nodes: the contiguous node
    // storage and the node reuse are expected to make the code
    // using it faster than the equivalent code using std::list.
    //
    // The list itself is a doubly linked list of immovables nodes
    // that starts at begin() and ends at the sentinel node end().
    // It can mostly be used like the standard library std::list.
    // The sentinel nodes is always the last node of the backing
    // storage.
    //
    // Internally the free nodes are tracked in a singly linked list
    // which reuses the nodes internal pointers: that singly linked
    // list of free nodes starts at first_free_ and ends at the
    // sentinel node. It is worth noting that the sentinel node's
    // prev pointer points at the previous element in the doubly
    // linked list, not at the previous node in the free nodes list.
    //
    // This list implementation does not check for overflow: when
    // trying to insert too many nodes there is a precondition that
    // there are still free nodes left in the list.
    //
    // This implementation does not provide all the functions to
    // match std::list, but only the ones actually used by the
    // library's algorithms, growing as needed

    template<typename T>
    struct fixed_size_list
    {
        public:

            ////////////////////////////////////////////////////////////
            // Public types

            using value_type = T;
            using size_type = std::size_t;
            using reference = value_type&;
            using const_reference = const value_type&;
            using pointer = value_type*;
            using const_pointer = const value_type*;
            using difference_type = std::ptrdiff_t;
            using iterator = fixed_size_list_iterator<T>;
            // TODO: const_iterator
            using node_type = fixed_size_list_node<T>;

            ////////////////////////////////////////////////////////////
            // Constructors

            // Make fixed_size_list immovable
            fixed_size_list(const fixed_size_list&) = delete;
            fixed_size_list(fixed_size_list&&) = delete;
            fixed_size_list& operator=(const fixed_size_list&) = delete;
            fixed_size_list& operator=(fixed_size_list&&) = delete;

            explicit fixed_size_list(std::ptrdiff_t capacity):
                // Allocate enough space to store N nodes plus a
                // sentinel node (where N = capacity)
                buffer_(static_cast<node_type*>(::operator new((capacity + 1) * sizeof(node_type))),
                       operator_deleter((capacity + 1) * sizeof(node_type))),
                sentinel_node_(buffer_.get() + capacity),
                first_free_(buffer_.get())
            {
                // fixed_size_list_node constructors are noexcept, so we don't
                // need to handle the cleanup of list nodes that would have been
                // required if exceptions could have been thrown

                for (auto ptr = buffer_.get() ; ptr != sentinel_node_ ; ++ptr) {
                    // Each node is initialized with a "next" field pointing to the next
                    // node in memory: this allows to create a list of free nodes layed
                    // out contiguously in memory in one pass
                    ::new (ptr) node_type(ptr + 1);
                }
                // Initialize sentinel node
                ::new (sentinel_node_) node_type(sentinel_node_, sentinel_node_);
            }

            ////////////////////////////////////////////////////////////
            // Destructor

            ~fixed_size_list()
            {
                // Destroy the constructed values
                if (sentinel_node_->next != sentinel_node_) {
                    node_type* ptr = sentinel_node_->next;
                    do {
                        auto next_ptr = ptr->next;
                        ptr->value.~T();
                        ptr = next_ptr;
                    } while (ptr != sentinel_node_);
                }

                // Destroy the nodes
                for (node_type* ptr = buffer_.get() ; ptr != sentinel_node_ ; ++ptr) {
                    ptr->~node_type();
                }
                sentinel_node_->~node_type();
            }

            ////////////////////////////////////////////////////////////
            // Iterator access

            auto begin() const
                -> iterator
            {
                return iterator(sentinel_node_->next);
            }

            auto end() const
                -> iterator
            {
                return iterator(sentinel_node_);
            }

            ////////////////////////////////////////////////////////////
            // Modifiers

            auto insert(iterator pos, const T& value)
                -> iterator
            {
                node_type* new_node = next_free_node_();
                ::new (&new_node->value) T(value);
                link_node_before_(new_node, pos.base());
                return iterator(new_node);
            }

            auto insert(iterator pos, T&& value)
                -> iterator
            {
                node_type* new_node = next_free_node_();
                ::new (&new_node->value) T(std::move(value));
                link_node_before_(new_node, pos.base());
                return iterator(new_node);
            }

            auto push_back(const T& value)
                -> void
            {
                insert(end(), value);
            }

            auto push_back(T&& value)
                -> void
            {
                insert(end(), std::move(value));
            }

        private:

            ////////////////////////////////////////////////////////////
            // Helper functions

            auto next_free_node_()
                -> node_type*
            {
                // Retrieve next free node
                CPPSORT_ASSERT(first_free_ != sentinel_node_);
                node_type* new_node = first_free_;
                first_free_ = first_free_->next;
                return new_node;
            }

            auto link_node_before_(node_type* node, node_type* pos)
                -> void
            {
                // Relink pointers to a new node
                node->prev = pos->prev;
                node->next = pos;
                node->prev->next = node;
                node->next->prev = node;
            }

            ////////////////////////////////////////////////////////////
            // Data members

            // Backing storage
            std::unique_ptr<node_type, operator_deleter> buffer_;

            // Immovable sentinel node: its prev field points to the last
            // element of the list, its next field to the first one
            node_type* const sentinel_node_;

            // First free node of the list
            node_type* first_free_;
    };
}}

#endif // CPPSORT_DETAIL_FIXED_SIZE_LIST_H_
