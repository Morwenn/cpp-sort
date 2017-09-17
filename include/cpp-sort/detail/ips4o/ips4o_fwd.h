/*
 * ips4o/ips4o_fwd.hpp
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
#ifndef CPPSORT_DETAIL_IPS4O_FWD_H_
#define CPPSORT_DETAIL_IPS4O_FWD_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <utility>

namespace cppsort
{
namespace detail
{
namespace ips4o
{
    namespace detail
    {
        template<typename It, typename Compare>
        auto baseCaseSort(It begin, It end, Compare&& compare)
            -> void;

        template<typename It, typename RandomGen>
        auto selectSample(It begin, const It end,
                          typename std::iterator_traits<It>::difference_type num_samples,
                          RandomGen&& gen)
            -> void;

        template<typename Cfg>
        class Sorter
        {
            public:

                using iterator = typename Cfg::iterator;
                using diff_t = typename Cfg::difference_type;
                using value_type = typename Cfg::value_type;

                class BufferStorage;
                class Block;
                class Buffers;
                class BucketPointers;
                class Classifier;
                struct LocalData;
                struct SharedData;
                explicit Sorter(LocalData& local):
                    local_(local)
                {}

                auto sequential(const iterator begin, const iterator end)
                    -> void;

            private:

                LocalData& local_;
                SharedData* shared_;
                Classifier* classifier_;

                diff_t* bucket_start_;
                BucketPointers* bucket_pointers_;
                Block* overflow_;

                iterator begin_;
                iterator end_;
                int num_buckets_;
                int my_id_;
                int num_threads_;

                static auto computeLogBuckets(const diff_t n)
                    -> int;

                auto buildClassifier(const iterator begin, const iterator end,
                                     Classifier& classifier)
                    -> std::pair<int, bool>;

                template<bool kEqualBuckets> /* __attribute__((flatten)) */
                auto classifyLocally(const iterator my_begin, const iterator my_end)
                    -> diff_t;

                auto parallelClassification(const bool use_equal_buckets)
                    -> void;

                auto sequentialClassification(const bool use_equal_buckets)
                    -> void;

                auto moveEmptyBlocks(const diff_t my_begin, const diff_t my_end,
                                     const diff_t my_first_empty_block)
                    -> void;

                auto computeOverflowBucket()
                    -> int;

                template<bool kEqualBuckets, bool kIsParallel>
                auto classifyAndReadBlock(const int read_bucket)
                    -> int;

                template <bool kEqualBuckets, bool kIsParallel>
                auto swapBlock(const diff_t max_off, const int dest_bucket,
                               const bool current_swap)
                    -> int;

                template<bool kEqualBuckets, bool kIsParallel>
                auto permuteBlocks()
                    -> void;

                auto saveMargins(int last_bucket)
                    -> std::pair<int, diff_t>;

                auto writeMargins(const int first_bucket, const int last_bucket,
                                  const int overflow_bucket, const int swap_bucket,
                                  const diff_t in_swap_buffer)
                    -> void;

                template<bool kIsParallel>
                auto partition(const iterator begin, const iterator end,
                               diff_t* const bucket_start, SharedData* const shared,
                               const int my_id, const int num_threads)
                    -> std::pair<int, bool>;

                auto processSmallTasks(const iterator begin, SharedData& shared)
                    -> void;
        };
    }

    template<typename Cfg>
    class SequentialSorter;

    template<typename Cfg>
    class ParallelSorter;

    template<typename It, class Comp>
    auto sort(It begin, It end, Comp comp)
        -> void;

    template<typename It>
    auto sort(It begin, It end)
        -> void;
}}}

#endif // CPPSORT_DETAIL_IPS4O_FWD_H_
