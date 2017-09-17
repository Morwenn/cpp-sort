/*
 * ips4o/ips4o.hpp
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
#ifndef CPPSORT_DETAIL_IPS4O_IPS4O_H_
#define CPPSORT_DETAIL_IPS4O_IPS4O_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <utility>
#include "ips4o_fwd.h"
#include "base_case.h"
#include "config.h"
#include "memory.h"
#include "sequential.h"
#include "../insertion_sort.h"

namespace cppsort
{
namespace detail
{
namespace ips4o
{
    /*
     * Helper function for creating a reusable sequential sorter.
     */
    template<typename RandomAccessIterator, typename Cfg=Config<>, typename Compare>
    auto make_sorter(Compare compare)
        -> SequentialSorter<ExtendedConfig<RandomAccessIterator, Compare, Cfg>>
    {
        return SequentialSorter<ExtendedConfig<RandomAccessIterator, Compare, Cfg>>{std::move(compare)};
    }

    /*
     * Configurable interface.
     */
    template<typename Cfg, typename RandomAccessIterator,
             typename Compare, typename Projection>
    auto sort(RandomAccessIterator begin, RandomAccessIterator end,
              Compare compare, Projection projection)
        -> void
    {
        if (std::distance(begin, end) <= Cfg::kBaseCaseMultiplier * Cfg::kBaseCaseSize) {
            detail::baseCaseSort(std::move(begin), std::move(end), std::move(compare));
        } else {
            make_sorter<RandomAccessIterator, Cfg>(std::move(compare))(std::move(begin),
                                                                       std::move(end));
        }
    }

    /*
     * Standard interface.
     */
    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto sort(RandomAccessIterator begin, RandomAccessIterator end,
              Compare compare, Projection projection)
        -> void
    {
        ips4o::sort<Config<>>(std::move(begin), std::move(end),
                              std::move(compare), std::move(projection));
    }
}}}

#endif // CPPSORT_DETAIL_IPS4O_IPS4O_H_
