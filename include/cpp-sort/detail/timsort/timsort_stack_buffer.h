/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Timothy Van Slyke
 * Modified in 2018 by Morwenn for inclusion into cpp-sort
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
#ifndef CPPSORT_DETAIL_TIMSORT_TIMSORT_STACK_BUFFER_H_
#define CPPSORT_DETAIL_TIMSORT_TIMSORT_STACK_BUFFER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <limits>
#include <type_traits>
#include <cpp-sort/utility/iter_move.h>
#include "compiler.h"
#include "memcpy_algos.h"
#include "minrun.h"
#include "../iterator_traits.h"
#include "../memory.h"
#include "../move.h"

namespace cppsort
{
namespace detail
{
    constexpr auto div_by_log2phi(std::size_t num) noexcept
        -> std::size_t
    {
        constexpr const double log2phi = 0.69424191363061737991557720306445844471454620361328125;
        return static_cast<std::size_t>(num / log2phi) + 1;
    }

    template<typename IntType>
    constexpr auto timsort_max_stack_size() noexcept
        -> std::size_t
    {
        return div_by_log2phi(std::numeric_limits<IntType>::digits) + 1;
    }

    template<typename IntType, typename ValueType>
    struct timsort_stack_buffer
    {
        using buffer_pointer_t = ValueType*;
        template<typename Iterator>
        using buffer_iter_t = std::conditional_t<
            (not can_forward_memcpy_v<Iterator>) and can_reverse_memcpy_v<Iterator>,
            std::reverse_iterator<buffer_pointer_t>,
            buffer_pointer_t
        >;
        using self_t = timsort_stack_buffer<IntType, ValueType>;
        static constexpr bool trivial_destructor = std::is_trivially_destructible<ValueType>::value;
        static constexpr bool nothrow_destructor = std::is_nothrow_destructible<ValueType>::value;
        static constexpr bool nothrow_move = std::is_nothrow_move_constructible<ValueType>::value;
        static constexpr bool trivial = std::is_trivial<ValueType>::value;

        timsort_stack_buffer() noexcept:
            buffer{},
            top{buffer + (buffer_size - 1)},
            num_in_merge_buffer{0}
        {
            push(0);
        }

        ~timsort_stack_buffer() noexcept(nothrow_destructor)
        {
            destroy_merge_buffer();
        }
        timsort_stack_buffer(const self_t&) = delete;
        timsort_stack_buffer(self_t&&) = delete;
        timsort_stack_buffer& operator=(self_t&&) = delete;
        timsort_stack_buffer& operator=(const self_t&) = delete;

        /* Returns the number of indices stored in the run stack. */
        auto offset_count() const noexcept
            -> std::size_t
        {
            return (buffer + (buffer_size - 1)) - top;
        }

        /*
         * Returns the number of runs in the run stack.
         * This is always one less than offset_count() because the first
         * item in the run stack is always SizeType(0).
         */
        auto run_count() const noexcept
            -> std::size_t
        {
            return offset_count() - 1;
        }

        /*
         * Returns the number of bytes used by the run stack.
         */
        auto bytes_consumed_by_run_stack() const noexcept
            -> std::size_t
        {
            return offset_count() * sizeof(IntType);
        }

        /*
         * Returns the number of bytes that would be used by the run stack
         * if an additional run were to be added.
         *
         * Used to determine whether some of the 'ValueType' objects in the
         * stack need to be destroyed before pushing an additional run onto
         * the run stack.
         */
        auto bytes_consumed_by_one_more_run() const noexcept
            -> std::size_t
        {
            return (offset_count() + 1) * sizeof(IntType);
        }

        /*
         * Returns the number of bytes in the stack buffer that are not
         * currently used to keep track of pending runs.
         *
         * Used to determine whether the free space in the stack buffer can be
         * used as a temporary buffer for the merge algorithm.
         */
        auto bytes_available_for_merge_buffer() const noexcept
            -> std::size_t
        {
            return buffer_size * sizeof(IntType) - bytes_consumed_by_run_stack();
        }

        /*
         * Returns the number of 'ValueType' objects that can fit in the
         * stack buffer without bleeding in to the space used to keep track
         * of pending runs.
         *
         * Used to determine whether the free space in the stack buffer can be
         * used as a temporary buffer for the merge algorithm.
         */
        auto count_available_for_merge_buffer() const noexcept
            -> std::size_t
        {
            return bytes_available_for_merge_buffer() / sizeof(ValueType);
        }

        /*
         * Returns the number of 'ValueType' objects that currently live in
         * the stack buffer and have yet to be destroyed.
         *
         * Used to determine whether some of the 'ValueType' objects in the
         * stack need to be destroyed before pushing an additional run onto
         * the run stack.
         */
        auto bytes_consumed_by_merge_buffer() const noexcept
            -> std::size_t
        {
            return num_in_merge_buffer * sizeof(ValueType);
        }
        /* Return the total number of bytes in the stack buffer. */
        static constexpr auto total_bytes_in_buffer() noexcept
            -> std::size_t
        {
            return buffer_size * sizeof(IntType);
        }
        /*
         * Returns a bool indicating whether some of the 'ValueType' objects
         * in the stack need to be destroyed before pushing an additional run
         * onto the run stack.
         */
        auto need_to_trim_merge_buffer() const noexcept
            -> bool
        {
            return (num_in_merge_buffer > 0) &&
                (total_bytes_in_buffer() - bytes_consumed_by_merge_buffer()) < bytes_consumed_by_one_more_run();
        }

        /*
         * Destroy some of the 'ValueType' objects in the stack buffer to
         * make room for an additional run to be pushed onto the stack.
         */
        auto destroy_enough_to_fit_one_more_run() noexcept
            -> void
        {
            if (sizeof(ValueType) >= sizeof(IntType)) {
                --num_in_merge_buffer;
                destroy_at(merge_buffer_begin() + num_in_merge_buffer);
            } else {
                std::size_t overlap =
                    (total_bytes_in_buffer() - bytes_consumed_by_one_more_run())
                    - bytes_consumed_by_merge_buffer();
                overlap = overlap / sizeof(ValueType) + ((overlap % sizeof(ValueType)) > 0);
                destroy(merge_buffer_end() - overlap, merge_buffer_end());
                num_in_merge_buffer -= overlap;
            }
        }

        /*
         * Destroy all of the 'ValueType' objects in the stack buffer.
         */
        auto destroy_merge_buffer(std::false_type)
            -> void
        {
            destroy(merge_buffer_begin(), merge_buffer_end());
            num_in_merge_buffer = 0;
        }

        auto destroy_merge_buffer(std::true_type) noexcept
            -> void
        {}

        auto destroy_merge_buffer() noexcept(nothrow_destructor)
            -> void
        {
            destroy_merge_buffer(std::integral_constant<bool, trivial_destructor>{});
        }

        /*
        Move elements from [begin, end) onto the unused memory in the run stack.
        It is assumed that the caller has ensured that the stack buffer has sufficient
        unused space to hold the provided range without overwriting any of the run counts.

        This member function tries suuuuper hard to safely use a memcpy().  std::copy() and
        friends cannot
        Using compile-time type information, this copy
        */

        template<typename Iterator>
        auto move_to_merge_buffer(Iterator begin, Iterator end)
            noexcept(trivial or nothrow_move)
            -> std::enable_if_t<
                can_forward_memcpy_v<Iterator>,
                buffer_iter_t<Iterator>
            >
        {
            static_assert(
                std::is_same<std::decay_t<ValueType>, std::decay_t<value_type_t<Iterator>>>::value,
                "if you see this message, timsort() is broken"
            );
            // both contiguous iterators to trivially copyable type.  do a memcpy
            std::memcpy(buffer, get_memcpy_iterator(begin), (end - begin) * sizeof(ValueType));
            return merge_buffer_begin();
        }

        template<typename Iterator>
        auto move_to_merge_buffer(Iterator begin, Iterator end)
            noexcept(trivial or nothrow_move)
            -> std::enable_if_t<
                can_reverse_memcpy_v<Iterator>,
                buffer_iter_t<Iterator>
            >
        {
            static_assert(
                std::is_same<std::decay_t<ValueType>, std::decay_t<value_type_t<Iterator>>>::value,
                "if you see this message, timsort() is broken"
            );
            // Iterator is a reverse contiguous iterators to trivially copyable type.
            // Do a memcpy and return a reverse iterator
            std::memcpy(buffer, get_memcpy_iterator(end - 1), (end - begin) * sizeof(ValueType));
            return std::make_reverse_iterator(merge_buffer_begin() + (end - begin));
        }

        template<typename Iterator>
        auto move_to_merge_buffer(Iterator begin, Iterator end)
            noexcept(trivial or nothrow_move)
            -> std::enable_if_t<
                not(can_forward_memcpy_v<Iterator> || can_reverse_memcpy_v<Iterator>),
                buffer_iter_t<Iterator>
            >
        {
            static_assert(
                std::is_same<std::decay_t<ValueType>, std::decay_t<utility::rvalue_reference_t<Iterator>>>::value,
                "if you see this message, timsort() is broken"
            );
            // not trivially copyable, provide at least basic exception guarantee
            fill_merge_buffer(begin, end);
            return merge_buffer_begin();
        }

        /*
         * Move the range [begin, end) on to the stack buffer.
         * If an exception is thrown while moving one of the objects to the
         * stack buffer, some of the objects in [begin, end) will be left in
         * a valid, but unspecified state.  No resources are leaked.
         */
        template<typename Iterator>
        auto fill_merge_buffer_inner(Iterator begin, Iterator end, std::true_type)
            noexcept(nothrow_move)
            -> void
        {
            auto dest = merge_buffer_begin();
            // if the destructor is trivial we don't need to increment 'num_in_merge_buffer'
            for(; begin < end ; (void)++dest, (void)++begin) {
                ::new(static_cast<ValueType*>(std::addressof(*dest))) ValueType(std::move(*begin));
            }
        }

        template<typename Iterator>
        auto fill_merge_buffer_inner(Iterator begin, Iterator end, std::false_type)
            noexcept(nothrow_move)
            -> void
        {
            auto dest = merge_buffer_begin();
            if (num_in_merge_buffer < std::size_t(end - begin)) {
                dest = detail::move(begin, begin + num_in_merge_buffer, dest);
                begin += num_in_merge_buffer;
            } else {
                dest = detail::move(begin, end, dest);
                begin = end;
                return;
            }

            // if the destructor is trivial we don't need to increment 'num_in_merge_buffer'
            for (; begin < end ; (void)++dest, (void)++begin) {
                using utility::iter_move;
                ::new(static_cast<ValueType*>(std::addressof(*dest))) ValueType(iter_move(begin));
                ++num_in_merge_buffer;
            }
        }

        template<typename Iterator>
        auto fill_merge_buffer(Iterator begin, Iterator end) noexcept(nothrow_move)
            -> void
        {
            fill_merge_buffer_inner(begin, end, std::integral_constant<bool, trivial_destructor>{});
        }

        template<typename Iterator>
        auto can_acquire_merge_buffer(Iterator begin, Iterator end) const noexcept
            -> bool
        {
            if (sizeof(ValueType) > (buffer_size * sizeof(IntType))) {
                return false;
            } else {
                return (end - begin) <= std::ptrdiff_t(count_available_for_merge_buffer());
            }
        }
        /*
         * @brief 	       Push a run onto the run stack.
         * @param run_end_pos  The index of the run being pushed onto the stack.
         */
        auto push(IntType run_end_pos, std::true_type) noexcept
            -> void
        {
            // TODO: assume(i > *(top + 1))
            *top = run_end_pos;
            --top;
        }

        auto push(IntType run_end_pos, std::false_type) noexcept(nothrow_destructor)
            -> void
        {
            // TODO: assume(i > *(top + 1))
            if (need_to_trim_merge_buffer()) {
                destroy_enough_to_fit_one_more_run();
            }
            *top = run_end_pos;
            --top;
        }

        auto push(IntType run_end_pos) noexcept(nothrow_destructor)
            -> void
        {
            // TODO: assume(i > *(top + 1))
            push(run_end_pos, std::integral_constant<bool, trivial_destructor>{});
        }

        /*
         * @brief 	  Erase the index at the top of the stack.
         * @param runlen  The index of the run being pushed onto the stack.
         */
        auto pop() noexcept
            -> void
        {
            ++top;
        }

        template<std::size_t I>
        auto get_offset() const noexcept
            -> const IntType&
        {
            static_assert(I < 5, "");
            return top[I + 1];
        }

        template<std::size_t I>
        auto get_offset() noexcept
            -> IntType&
        {
            static_assert(I < 5, "");
            return top[I + 1];
        }

        auto operator[](std::ptrdiff_t i) const noexcept
            -> const IntType&
        {
            return top[i + 1];
        }

        /*
         * Return a bool indicating whether a merge needs to occur between
         * two of the three runs at the top of the stack.
         */
        auto merge_ABC_case_1() const noexcept
            -> bool
        {
            return get_offset<2>() - get_offset<3>() <= get_offset<0>() - get_offset<2>();
        }

        /*
         * Return a bool indicating whether a merge needs to occur between
         * two of the three runs at the top of the stack.
         */
        auto merge_ABC_case_2() const noexcept
            -> bool
        {
            return get_offset<3>() - get_offset<4>() <= get_offset<1>() - get_offset<3>();
        }

        /*
         * Return a bool indicating whether a merge needs to occur between
         * two of the three runs at the top of the stack.
         */
        auto merge_ABC() const noexcept
            -> bool
        {
            return merge_ABC_case_1() || merge_ABC_case_2();
        }

        /*
         * Given that a merge needs to occur between two of the three runs
         * at the top of the stack, returns true if the merge should occur
         * between the third-from-the-top and second-from-the-top runs
         * on the stack.  If false is returned, the two topmost runs should
         * be merged instead.
         */
        auto merge_AB() const noexcept
            -> bool
        {
            return get_offset<2>() - get_offset<3>() < get_offset<0>() - get_offset<1>();
        }

        /*
         * Return a bool indicating whether a merge needs to occur between
         * the two runs at the top of the stack.
         */
        auto merge_BC() const noexcept
            -> bool
        {
            return get_offset<1>() - get_offset<2>() <= get_offset<0>() - get_offset<1>();
        }

        /*
         * Modifies the run stack to reflect the fact that the run ending at
         * index 'I' was merged with the run beginning at index 'I'.
         */
        template<std::size_t I>
        auto remove_run() noexcept
            -> std::enable_if_t<
                I == 0
            >
        {
            pop();
        }

        template<std::size_t I>
        auto remove_run() noexcept
            -> std::enable_if_t<
                I != 0
            >
        {
            static_assert(I < 4, "");
            get_offset<I>() = get_offset<I - 1>();
            remove_run<I - 1>();
        }

        /*
         * Obtain an iterator to the beginning of the merge buffer.
         */
        auto merge_buffer_begin() const noexcept
            -> const ValueType*
        {
            return reinterpret_cast<const ValueType*>(buffer);
        }

        /*
         * Obtain an iterator to the beginning of the merge buffer.
         */
        auto merge_buffer_begin() noexcept
            -> ValueType*
        {
            return reinterpret_cast<ValueType*>(buffer);
        }

        /*
         * Obtain an iterator to the end of the merge buffer.
         */
        auto merge_buffer_end() const noexcept
            -> const ValueType*
        {
            return reinterpret_cast<const ValueType*>(buffer) + num_in_merge_buffer;
        }

        /*
         * Obtain an iterator to the end of the merge buffer.
         */
        auto merge_buffer_end() noexcept
            -> ValueType*
        {
            return reinterpret_cast<ValueType*>(buffer) + num_in_merge_buffer;
        }

        static constexpr auto extra_stack_alloc() noexcept
            -> std::size_t
        {
            constexpr std::size_t minrun_bytes = max_minrun<ValueType>() * sizeof(ValueType);
            constexpr std::size_t max_bytes = 128 * sizeof(void*);
            // a merge only happens after at least two runs are on the
            // stack so subtract off 2 from the stack size to simulate
            constexpr std::size_t stack_max_bytes = (timsort_max_stack_size<IntType>() - 2) * sizeof(IntType);

            if (minrun_bytes < max_bytes)
            {
                std::size_t nbytes = minrun_bytes;
                // see how many '(2**k) * minrun's we can fit on the stack
                while (nbytes < max_bytes) {
                    nbytes += nbytes;
                }
                return (nbytes / sizeof(IntType)) / 2;
            }
            else if (max_bytes + stack_max_bytes >= minrun_bytes and not (stack_max_bytes >= minrun_bytes))
            {
                // if adding 'max_bytes' bytes to the run stack allows fitting
                // a run of length 'minrun' on the stack, then go ahead and allocate
                return max_bytes / sizeof(IntType);
            }
            else
            {
                // otherwise don't bother allocating extra, we'll probably be on the
                // heap the whole time anyway
                return 0;
            }
        }

        /* Size of the buffer measured in 'IntType' objects. */
        static constexpr std::size_t buffer_size = timsort_max_stack_size<IntType>();// + extra_stack_alloc();
        /* Alignment of the buffer. */
        static constexpr std::size_t required_alignment = alignof(std::aligned_union_t<sizeof(IntType), IntType, ValueType>);
        alignas(required_alignment) IntType buffer[buffer_size];
        IntType* top;
        /*
         * Number of 'ValueType' objects in the merge buffer.
         * If 'ValueType' is trivially destructible, this is always zero.
         */
        std::conditional_t<trivial_destructor, const std::size_t, std::size_t> num_in_merge_buffer = 0;
    };
}}

#endif // CPPSORT_DETAIL_TIMSORT_TIMSORT_STACK_BUFFER_H_
