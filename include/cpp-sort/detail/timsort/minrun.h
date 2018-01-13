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
#ifndef CPPSORT_DETAIL_TIMSORT_MINRUN_H_
#define CPPSORT_DETAIL_TIMSORT_MINRUN_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>

namespace cppsort
{
namespace detail
{
    /*
     * Modified variant of the compute_minrun() function used in CPython's
     * list_sort().
     *
     * The CPython version of this function chooses a value in [32, 65) for
     * minrun. Unlike in CPython, C++ objects aren't guaranteed to be the
     * size of a pointer. A heuristic is used here under the assumption
     * that std::move(some_arbitrary_cpp_object) is basically a bit-blit.
     * If the type is larger that 4 pointers then minrun maxes out at 32
     * instead of 64. Similarly, if the type is larger than 8 pointers,
     * it maxes out at 16. This is a major win for large objects (think
     * tuple-of-strings).
     *
     * Four pointers is used as the cut-off because libstdc++'s std::string
     * implementation was slightly, but measurably worse in the benchmarks
     * when the max minrun was 32 instead of 64 (and their std::string is
     * 4 pointers large).
     */
    template<typename T>
    constexpr auto max_minrun() noexcept
        -> std::size_t
    {
        if (sizeof(T) > sizeof(void*) * 8) {
            return 16;
        } else if (sizeof(T) > sizeof(void*) * 4) {
            return 32;
        } else {
            return 64;
        }
    }

    template<typename T>
    constexpr auto compute_minrun(std::size_t n) noexcept
        -> std::size_t
    {
        constexpr std::size_t minrun_max = max_minrun<T>();
        std::size_t r = 0;
        while (n >= minrun_max) {
            r |= (n & 1);
            n >>= 1;
        }
        return n + r;
    }
}}

#endif // CPPSORT_DETAIL_TIMSORT_MINRUN_H_
