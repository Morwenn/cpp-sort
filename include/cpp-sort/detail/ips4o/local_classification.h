/*
 * ips4o/local_classification.hpp
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
#ifndef CPPSORT_DETAIL_IPS4O_LOCAL_CLASSIFICATION_H_
#define CPPSORT_DETAIL_IPS4O_LOCAL_CLASSIFICATION_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "ips4o_fwd.h"
#include "classifier.h"
#include "empty_block_movement.h"
#include "memory.h"

namespace cppsort
{
namespace detail
{
namespace ips4o
{
namespace detail
{
    /*
     * Local classification phase.
     */
    template<typename Cfg>
    template<bool kEqualBuckets>
    auto Sorter<Cfg>::classifyLocally(const iterator my_begin, const iterator my_end)
        -> typename Cfg::difference_type
    {
        auto write = my_begin;
        auto& buffers = local_.buffers;

        // Do the classification
        classifier_->template classify<kEqualBuckets>(my_begin, my_end,
            [&](typename Cfg::bucket_type bucket, iterator it) {
                // Only flush buffers on overflow
                if (buffers.isFull(bucket)) {
                    buffers.writeTo(bucket, write);
                    write += Cfg::kBlockSize;
                    local_.bucket_size[bucket] += Cfg::kBlockSize;
                }
                buffers.push(bucket, std::move(*it));
            });

        // Update bucket sizes to account for partially filled buckets
        for (int i = 0, end = num_buckets_ ; i < end ; ++i) {
            local_.bucket_size[i] += local_.buffers.size(i);
        }
        return write - begin_;
    }

    /*
     * Local classification in the sequential case.
     */
    template<typename Cfg>
    auto Sorter<Cfg>::sequentialClassification(const bool use_equal_buckets)
        -> void
    {
        const auto my_first_empty_block = use_equal_buckets
            ? classifyLocally<true>(begin_, end_)
            : classifyLocally<false>(begin_, end_);

        // Find bucket boundaries
        diff_t sum = 0;
        bucket_start_[0] = 0;
        for (int i = 0, end = num_buckets_ ; i < end ; ++i) {
            sum += local_.bucket_size[i];
            bucket_start_[i + 1] = sum;
        }
        CPPSORT_ASSUME(bucket_start_[num_buckets_] == end_ - begin_);

        // Set write/read pointers for all buckets
        for (int bucket = 0, end = num_buckets_ ; bucket < end ; ++bucket) {
            const auto start = Cfg::alignToNextBlock(bucket_start_[bucket]);
            const auto stop = Cfg::alignToNextBlock(bucket_start_[bucket + 1]);
            bucket_pointers_[bucket].set(
                    start,
                    (start >= my_first_empty_block
                             ? start
                             : (stop <= my_first_empty_block ? stop : my_first_empty_block))
                            - Cfg::kBlockSize);
        }
    }
}}}}

#endif // CPPSORT_DETAIL_IPS4O_LOCAL_CLASSIFICATION_H_
