/*
 * ips4o/base_case.hpp
 *
 * In-place Parallel Super Scalar Samplesort (IPS⁴o)
 *
 ******************************************************************************
 * BSD 2-Clause License
 *
 * Copyright © 2017, Michael Axtmann <michael.axtmann@kit.edu>
 * Copyright © 2017, Daniel Ferizovic <daniel.ferizovic@student.kit.edu>
 * Copyright © 2017, Sascha Witt <sascha.witt@kit.edu>
 * All rights reserved.
 *
 * Modified in 2017 by Morwenn for inclusion into cpp-sort.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef CPPSORT_DETAIL_IPS4O_BASE_CASE_H_
#define CPPSORT_DETAIL_IPS4O_BASE_CASE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <cstddef>
#include <utility>
#include "ips4o_fwd.h"
#include "../assume.h"

namespace cppsort
{
namespace detail
{
namespace ips4o
{
namespace detail
{
    template<typename Iterator, typename Compare>
    auto insertionSort(Iterator begin, Iterator end, Compare compare)
        -> void
    {
        CPPSORT_ASSUME(begin < end);

        for (auto it = begin + 1 ; it < end ; ++it) {
            auto val = std::move(*it);
            if (compare(val, *begin)) {
                std::move_backward(begin, it, it + 1);
                *begin = std::move(val);
            } else {
                auto cur = it;
                for (auto next = it - 1; compare(val, *next); --next) {
                    *cur = std::move(*next);
                    cur = next;
                }
                *cur = std::move(val);
            }
        }
    }

    /*
     * Wrapper for base case sorter, for easier swapping.
     */
    template<typename Iterator, typename Compare>
    auto baseCaseSort(Iterator begin, Iterator end, Compare&& compare)
        -> void
    {
        if (begin == end) return;
        detail::insertionSort(std::move(begin), std::move(end), std::forward<Compare>(compare));
    }
}}}}

#endif // CPPSORT_DETAIL_IPS4O_BASE_CASE_H_
