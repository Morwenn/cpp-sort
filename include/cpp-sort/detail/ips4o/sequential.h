/*
 * ips4o/sequential.hpp
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
#ifndef CPPSORT_DETAIL_IPS4O_SEQUENTIAL_H_
#define CPPSORT_DETAIL_IPS4O_SEQUENTIAL_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <utility>
#include "ips4o_fwd.h"
#include "base_case.h"
#include "memory.h"
#include "partitioning.h"

namespace cppsort
{
namespace detail
{
namespace ips4o
{
    namespace detail
    {
        /*
         * Recursive entry point for sequential algorithm.
         */
        template<typename Cfg>
        auto Sorter<Cfg>::sequential(const iterator begin, const iterator end)
            -> void
        {
            // Check for base case
            const auto n = std::distance(begin, end);
            if (n <= 2 * Cfg::kBaseCaseSize) {
                detail::baseCaseSort(begin, end, local_.classifier.getComparator());
                return;
            }
            diff_t bucket_start[Cfg::kMaxBuckets + 1];

            // Do the partitioning
            const auto res = partition<false>(begin, end, bucket_start, nullptr, 0, 1);
            const int num_buckets = std::get<0>(res);
            const bool equal_buckets = std::get<1>(res);

            // Final base case is executed in cleanup step, so we're done here
            if (n <= Cfg::kSingleLevelThreshold) {
                return;
            }

            // Recurse
            for (int i = 0; i < num_buckets; i += 1 + equal_buckets) {
                const auto start = bucket_start[i];
                const auto stop = bucket_start[i + 1];
                if (stop - start > 2 * Cfg::kBaseCaseSize)
                    sequential(begin + start, begin + stop);
            }
            if (equal_buckets) {
                const auto start = bucket_start[num_buckets - 1];
                const auto stop = bucket_start[num_buckets];
                if (stop - start > 2 * Cfg::kBaseCaseSize)
                    sequential(begin + start, begin + stop);
            }
        }
    }

    /*
     * Reusable sequential sorter.
     */
    template<typename Cfg>
    class SequentialSorter
    {
        using Sorter = detail::Sorter<Cfg>;
        using iterator = typename Cfg::iterator;

        public:

            explicit SequentialSorter(typename Cfg::less comp):
                buffer_storage_(1),
                local_ptr_(Cfg::kDataAlignment, std::move(comp), buffer_storage_.get())
            {}

            explicit SequentialSorter(typename Cfg::less comp, char* buffer_storage):
                local_ptr_(Cfg::kDataAlignment, std::move(comp), buffer_storage)
            {}

            auto operator()(iterator begin, iterator end)
                -> void
            {
                Sorter(local_ptr_.get()).sequential(std::move(begin), std::move(end));
            }

        private:

            typename Sorter::BufferStorage buffer_storage_;
            detail::AlignedPtr<typename Sorter::LocalData> local_ptr_;
    };
}}}

#endif // CPPSORT_DETAIL_IPS4O_SEQUENTIAL_H_
