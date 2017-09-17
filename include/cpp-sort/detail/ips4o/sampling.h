/*
 * ips4o/sampling.hpp
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
#ifndef CPPSORT_DETAIL_IPS4O_SAMPLING_H_
#define CPPSORT_DETAIL_IPS4O_SAMPLING_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <iterator>
#include <random>
#include <utility>
#include "ips4o_fwd.h"
#include "classifier.h"
#include "config.h"
#include "memory.h"
#include "../assume.h"
#include "../bitops.h"

namespace cppsort
{
namespace detail
{
namespace ips4o
{
namespace detail
{
    /*
     * Selects a random sample in-place.
     */
    template<typename It, typename RandomGen>
    auto selectSample(It begin, const It end,
                      typename std::iterator_traits<It>::difference_type num_samples,
                      RandomGen&& gen)
        -> void
    {
        using std::swap;

        auto n = std::distance(begin, end);
        while (num_samples--) {
            const auto i = std::uniform_int_distribution<typename std::iterator_traits<It>::difference_type>(0, --n)(gen);
            swap(*begin, begin[i]);
            ++begin;
        }
    }

    /*
     * Builds the classifer.
     */
    template<typename Cfg>
    auto Sorter<Cfg>::buildClassifier(const iterator begin, const iterator end,
                                      Classifier& classifier)
        -> std::pair<int, bool>
    {
        const auto n = std::distance(begin, end);
        int log_buckets = Cfg::logBuckets(n);
        int num_buckets = 1 << log_buckets;
        const auto step = std::max<diff_t>(1, Cfg::oversamplingFactor(n));
        const auto num_samples = step * num_buckets - 1;

        // Select the sample
        detail::selectSample(begin, end, num_samples, local_.random_generator);

        // Sort the sample
        sequential(begin, begin + num_samples);
        auto splitter = begin + step - 1;
        auto sorted_splitters = classifier.getSortedSplitters();
        const auto compare = classifier.getComparator();

        // Choose the splitters
        CPPSORT_ASSUME(sorted_splitters != nullptr);
        new (sorted_splitters) typename Cfg::value_type(*splitter);
        for (int i = 2 ; i < num_buckets ; ++i) {
            splitter += step;
            // Skip duplicates
            if (compare(*sorted_splitters, *splitter)) {
                CPPSORT_ASSUME(sorted_splitters + 1 != nullptr);
                new (++sorted_splitters) typename Cfg::value_type(*splitter);
            }
        }

        // Check for duplicate splitters
        const auto diff_splitters = sorted_splitters - classifier.getSortedSplitters() + 1;
        const bool use_equal_buckets = Cfg::kAllowEqualBuckets
                && num_buckets - 1 - diff_splitters >= Cfg::kEqualBucketsThreshold;

        // Fill the array to the next power of two
        log_buckets = cppsort::detail::log2(diff_splitters) + 1;
        num_buckets = 1 << log_buckets;
        for (int i = diff_splitters + 1 ; i < num_buckets ; ++i) {
            CPPSORT_ASSUME(sorted_splitters + 1 != nullptr);
            new (++sorted_splitters) typename Cfg::value_type(*splitter);
        }

        // Build the tree
        classifier.build(log_buckets);
        this->classifier_ = &classifier;

        const int used_buckets = num_buckets * (1 + use_equal_buckets);
        return {used_buckets, use_equal_buckets};
    }
}}}}

#endif // CPPSORT_DETAIL_IPS4O_SAMPLING_H_
