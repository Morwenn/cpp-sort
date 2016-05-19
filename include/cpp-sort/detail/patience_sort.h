/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 JuSch92
 * Modified in 2016 by Morwenn for inclusion into cpp-sort
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
#ifndef CPPSORT_DETAIL_PATIENCE_SORT_H_
#define CPPSORT_DETAIL_PATIENCE_SORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <iterator>
#include <list>
#include <memory>
#include <vector>
#include "iterator_traits.h"

namespace cppsort
{
namespace detail
{
    constexpr std::size_t kValuesPerBlock = 800;

    template<typename T>
    struct RunBlock {
        RunBlock* next;
        RunBlock* prev;
        int next_free_pos_;
        T values[kValuesPerBlock];
        bool is_front;
        RunBlock():
            next(nullptr),
            prev(nullptr),
            next_free_pos_(0),
            is_front(false)
        {}
    };

    template<typename T>
    class RunPool
    {
        class iterator
        {
        public:
            using self_type = iterator;
            using value_type = T;
            using reference = value_type&;
            using pointer = value_type*;
            using difference_type = std::size_t;
            using iterator_category = std::bidirectional_iterator_tag;

            iterator(RunBlock<value_type>* block, std::size_t index) : block_(block), index_(index) {}

            self_type& operator++() {
                index_++;
                if(index_ >= kValuesPerBlock) {
                    index_ -= kValuesPerBlock;
                    block_ = block_->next;
                    return *this;
                }
                return *this;
            }
            self_type& operator++(int) {
                self_type& temp = *this;
                index_++;
                if(index_ >= kValuesPerBlock) {
                    index_ -= kValuesPerBlock;
                    block_ = block_->next;
                    return temp;
                }
                return temp;
            }
            self_type& operator--() {
                index_--;
                if(index_ < 0) {
                    index_ += kValuesPerBlock;
                    block_ = block_->prev;
                }
                return *this;
            }
            self_type& operator--(int i) {
                self_type& temp = *this;
                index_--;
                if(index_ < 0) {
                    index_ += kValuesPerBlock;
                    block_ = block_->prev;
                }
                return temp;
            }
            reference operator*() {
                return block_->values[index_];
            }
            pointer operator->() {
                return &block_->values[index_];
            }
            bool operator==(const self_type& rhs) {
                return &block_->values[index_] == &rhs.block_[rhs.index_];
            }
            bool operator!=(const self_type& rhs) {
                return &block_->values[index_] != &rhs.block_->values[rhs.index_];
            }

            RunBlock<T>* block_;
            std::size_t index_;
        };

    public:
        RunPool():
            begin_back_(Alloc()),
            end_back_(begin_back_),
            size_(0),
            begin_front_(nullptr),
            end_front_(nullptr),
            end_block_(begin_back_)
        {}

        RunPool(const RunPool&) = delete;
        RunPool& operator=(const RunPool&) = delete;
        RunPool(RunPool&&) = default;
        RunPool& operator=(RunPool&&) = default;

        void Add(T &value) {
            if(end_back_->next_free_pos_ < kValuesPerBlock) {
                end_back_->values[end_back_->next_free_pos_] = value;
            } else {
                RunBlock<T>* temp = Alloc();
                temp->values[0] = value;
                temp->prev = end_back_;
                end_back_->next = temp;
                end_back_ = temp;
                end_block_ = temp;
                size_ += kValuesPerBlock;
            }
            end_back_->next_free_pos_++;
        }

        void AddFront(T &value) {
            if(begin_front_ == nullptr) {
                begin_front_ = Alloc();
                begin_front_->next_free_pos_ = kValuesPerBlock - 1;
                begin_front_->is_front = true;
                begin_front_->next = begin_back_;

                end_front_ = begin_front_;
                begin_back_->prev = end_front_;
            }

            if(begin_front_->next_free_pos_ < 0) {
                RunBlock<T>* temp = Alloc();
                temp->is_front = true;
                temp->next = begin_front_;
                temp->values[kValuesPerBlock - 1] = value;
                temp->next_free_pos_ = kValuesPerBlock - 2;

                begin_front_->prev = temp;
                begin_front_ = temp;
                size_ += kValuesPerBlock;

            } else {
                begin_front_->values[begin_front_->next_free_pos_] = value;
                begin_front_->next_free_pos_--;
            }
        }

        std::size_t size() const {
            int size_total = size_;
            size_total += end_back_->next_free_pos_;

            if(begin_front_ != nullptr) {
                size_total +=  kValuesPerBlock - begin_front_->next_free_pos_ - 1;
            }
            return size_total;
        }

        T& operator[](std::size_t t) {
            RunBlock<T>* temp = begin_back_;
            while(t >= kValuesPerBlock) {
                temp = temp->next;
                t -= kValuesPerBlock;
            }
            return temp->values[t];
        }

        iterator begin() {
            if(begin_front_ == nullptr) {
                return iterator(begin_back_, 0);
            } else {
                return iterator(begin_front_, begin_front_->next_free_pos_ + 1);
            }

        }

        iterator end() {        // points to the last element plus one
            if(end_block_->next_free_pos_>= kValuesPerBlock) {
                return iterator(end_block_, kValuesPerBlock);
            }
            return iterator(end_block_, end_block_->next_free_pos_);

        }

        iterator last() {       // points to the last element
            if(end_block_->next_free_pos_ > 0) {
                return iterator(end_block_, end_block_->next_free_pos_ - 1);
            } else {
                return iterator(end_block_->prev, kValuesPerBlock - 1);
            }
        }

        T& back() {
            int next_free = end_block_->next_free_pos_;
            if(next_free <= 0) {
                return end_block_->prev->values[kValuesPerBlock - 1];
            } else {
                return end_block_->values[next_free - 1];
            }
        }

        static void SetMemSize(std::size_t s) {
            mem_blocks_ = s;
        }

        static void Init() {
            memory_ = new RunBlock<T>[mem_blocks_];
            next_free_ = memory_;
        }

        static void Release() {
            delete[] memory_;
            memory_ = nullptr;
        }

        static RunBlock<T>* Alloc() {
            RunBlock<T>* ret = next_free_;
            next_free_++;
            return ret;
        }

    private:
        RunBlock<T>* begin_back_;
        RunBlock<T>* end_back_;
        RunBlock<T>* begin_front_;
        RunBlock<T>* end_front_;
        RunBlock<T>* end_block_;
        std::size_t size_;

        static RunBlock<T>* memory_;
        static RunBlock<T>* next_free_;
        static std::size_t mem_blocks_;
    };

    template<typename T>
    RunBlock<T>* RunPool<T>::memory_ = nullptr;

    template<typename T>
    RunBlock<T>* RunPool<T>::next_free_ = nullptr;

    template<typename T>
    std::size_t RunPool<T>::mem_blocks_;

    const float kMaxSortedness =    0.35f;
    const float kRunPoolSize =      1.5f;
    const float kBlockPoolFactor =  15.0f;

    struct RunInfo {
        std::size_t array_index, elem_index, run_size;
    public:
        RunInfo() { }

        RunInfo(std::size_t arr_index, std::size_t el_index, std::size_t run_count) {
            array_index = arr_index;
            elem_index = el_index;
            run_size = run_count;
        }
    };

    template<typename RandomAccessIterator>
    class PatienceSorting {
    public:

        using value_type = value_type_t<RandomAccessIterator>;
        using value_vector = std::vector<value_type>;
        using run_info_list = std::list<RunInfo>;

        void Sort(RandomAccessIterator begin, RandomAccessIterator end) {
            std::vector<RunPool<value_type>*> runs;
            GenerateRuns(begin, end, runs);
            Merge(begin, runs);
            // release pool memory of runs and runblocks
            RunPool<value_type>::Release();
            Release();
        }

    private:
        std::vector<value_type> lasts_;
        std::vector<value_type> heads_;
        long num_elements_ = 0;
        std::size_t num_runs_ = 0;
        float sortedness_ = 0.0f;
        int values_not_in_order_ = 0;

        static RunPool<value_type>* memory_;
        static RunPool<value_type>* next_free_;
        static std::size_t run_blocks_;

        void GenerateRuns(RandomAccessIterator begin, RandomAccessIterator end, std::vector<RunPool<value_type>*>& runs) {
            runs.clear();
            num_elements_ = std::distance(begin, end);
            num_runs_ = static_cast<std::size_t>(std::sqrt(num_elements_));

            // calculate the degree of disorder
            values_not_in_order_ = GetOrderLevel(begin, end);
            sortedness_ = values_not_in_order_ / static_cast<float>(num_elements_);

            // fallback to std::sort if input data is too random
            if(sortedness_ > kMaxSortedness) {
                std::sort(begin, end);
                return;
            }

            // Pool for runs
            SetMemSize(num_runs_ * kRunPoolSize);
            Init();

            // Pool for runblocks
            RunPool<value_type>::SetMemSize(GetMemPoolSize(num_elements_, num_runs_));
            RunPool<value_type>::Init();

            runs.reserve(num_runs_);
            lasts_.reserve(num_elements_);
            heads_.reserve(num_elements_);

            for (auto it = begin; it != end; ++it) {

                // binary search the right run to insert the current element
                value_type value = *it;
                auto key = std::lower_bound(lasts_.begin(), lasts_.end(), value, [](value_type& v1, value_type v2) { return v1 > v2;});

                if (key != lasts_.end()) {      // if suitable run is found, append
                    std::size_t i = std::distance(lasts_.begin(), key);
                    runs[i]->Add(value);
                    lasts_[i] = value;

                    // if we add to the first run, try to add as many elements as possible to avoid expensive binary search
                    if (i == 0) {
                        auto next_value = std::next(it);
                        while (next_value != end
                               && *next_value > lasts_[i]) {
                            ++it;
                            runs[0]->Add(*it);
                            lasts_[0] = *it;
                        }
                    }
                }
                else {      // no suitable run found, so we try to add the element to the begin of a run

                    // binary search the beginnings to find a suitable run
                    key = std::lower_bound(heads_.begin(), heads_.end(), value);
                    if (key == heads_.end()) {      // no suitable run found, create a new run and add it to sorted runs vector

                        RunPool<value_type>* run = Alloc();
                        new(run) RunPool<value_type>();      // necessary, otherwise the objects are not initialized -> memory error
                        runs.push_back(run);

                        runs.back()->Add(value);
                        lasts_.push_back(value);
                        heads_.push_back(value);
                    } else {
                        // suitable run found, so append to its beginning.
                        std::size_t i = std::distance(heads_.begin(), key);
                        runs[i]->AddFront(value);
                        heads_[i] = value;
                    }
                }
            }
        }

        void Merge(RandomAccessIterator begin, std::vector<RunPool<value_type>*>& runs) {
            // if no runs exist, input is probably empty, so exit here
            if(runs.size() == 0) {
                return;
            }

            // if only 1 run exists, this means the input data is in ascending order or in reversed, but
            // by adding to the front of a run it is automatically reversed
            if (runs.size() < 2) {
                // copy content to target array
                auto end = runs[0]->last();
                auto first_copy = begin;
                for(auto it = runs[0]->begin(); it != end; ++it) {
                    *first_copy = *it;
                    first_copy++;
                }
                *first_copy = runs[0]->back();
                return;
            }

            std::sort(runs.begin(), runs.end(), [](const RunPool<value_type>* a, const RunPool<value_type>* b) { return
                    a->size() <
                    b->size(); });
            value_vector elems1;
            value_vector elems2(num_elements_);
            elems1.reserve(num_elements_);
            run_info_list run_infos;
            std::size_t next_empty_arr_loc = 0;
            std::array<value_vector *, 2> arrs;
            arrs[0] = &elems1;
            arrs[1] = &elems2;

            // copy sorted runs consecutively to the first ping-pong array and save the information
            // in which array it is, at which index it starts and its size to an additional list element
            for (std::size_t i = 0; i < runs.size(); i++) {

                std::size_t temp_index = next_empty_arr_loc;
                auto it = runs[i]->begin();
                auto end = runs[i]->last();
                for(; it != end; ++it) {
                    elems1.push_back(*it);
                    temp_index++;
                }
                elems1.push_back(*it);

                RunInfo run_info;
                run_info.array_index = 0;
                run_info.elem_index = next_empty_arr_loc;
                run_info.run_size = runs[i]->size();
                run_infos.push_back(run_info);
                next_empty_arr_loc += runs[i]->size();
            }

            auto cur_run = run_infos.begin();
            const auto beginRun = cur_run;

            // Ping-pong merge until only 2 runs are left
            while (run_infos.size() > 2) {
                auto next_run = std::next(cur_run, 1);
                if (cur_run == run_infos.end() || std::next(cur_run, 1) == run_infos.end() ||
                    (cur_run->run_size + next_run->run_size) >
                    (beginRun->run_size + (std::next(beginRun, 1)->run_size))) {
                    cur_run = run_infos.begin();
                }
                next_run = std::next(cur_run, 1);
                if (cur_run->array_index == 0) {          // blindly merge curRun and curRuns next into Elems2
                    BlindMerge(arrs, elems2, cur_run, 1);
                } else {
                    BlindMerge(arrs, elems1, cur_run, 0);
                }
                cur_run->run_size += next_run->run_size;
                run_infos.erase(next_run);
                cur_run++;
            }

            // merge the last 2 runs directly to the output
            cur_run = run_infos.begin();
            BlindMerge(arrs, begin, cur_run);
        }

        // Merge 2 sorted runs into a ping-pong array
        void BlindMerge(std::array<value_vector *, 2> &arrs, value_vector &write,
                        typename std::list<RunInfo>::iterator cur_run, std::size_t arr_index) {
            auto next_run = std::next(cur_run, 1);
            std::size_t one = cur_run->elem_index,
                    two = next_run->elem_index,
                    k = cur_run->elem_index;
            value_vector &curr_arr = *arrs[cur_run->array_index];
            value_vector &next_arr = *arrs[next_run->array_index];

            while ((one - cur_run->elem_index) < cur_run->run_size &&
                   (two - next_run->elem_index) < next_run->run_size) {

                if (curr_arr[one] < next_arr[two]) {
                    write[k] = curr_arr[one];
                    one++;
                } else {
                    write[k] = next_arr[two];
                    two++;
                }
                k++;
            }
            // if both runs don't have the same size, copy the remaining elements over
            if ((one - cur_run->elem_index) < cur_run->run_size) {
                for (std::size_t i = (one - cur_run->elem_index); i < cur_run->run_size; i++) {
                    write[k] = curr_arr[i + cur_run->elem_index];
                    k++;
                }
            } else {
                for (std::size_t i = (two - next_run->elem_index); i < next_run->run_size; i++) {
                    write[k] = next_arr[i + next_run->elem_index];
                    k++;
                }
            }
            cur_run->array_index = arr_index;
        }

        // Merge the last 2 runs directly to the output
        void BlindMerge(std::array<value_vector *, 2> &arrs, RandomAccessIterator begin,
                        typename std::list<RunInfo>::iterator cur_run) {
            auto next_run = std::next(cur_run, 1);
            std::size_t one = cur_run->elem_index,
                    two = next_run->elem_index;
            value_vector &curr_arr = *arrs[cur_run->array_index];
            value_vector &next_arr = *arrs[next_run->array_index];

            while ((one - cur_run->elem_index) < cur_run->run_size &&
                   (two - next_run->elem_index) < next_run->run_size) {

                if (curr_arr[one] < next_arr[two]) {
                    *begin = curr_arr[one];
                    one++;
                } else {
                    *begin = next_arr[two];
                    two++;
                }
                begin++;
            }
            if ((one - cur_run->elem_index) < cur_run->run_size) {
                for (std::size_t i = (one - cur_run->elem_index); i < cur_run->run_size; i++) {
                    *begin = curr_arr[i + cur_run->elem_index];
                    begin++;
                }
            } else {
                for (std::size_t i = (two - next_run->elem_index); i < next_run->run_size; i++) {
                    *begin = next_arr[i + next_run->elem_index];
                    begin++;
                }
            }
        }

        static void SetMemSize(std::size_t s) {
            run_blocks_ = s;
        }

        static void Init() {
            memory_ = new RunPool<value_type>[run_blocks_];
            next_free_ = memory_;
        }

        static void Release() {
            delete[] memory_;
            memory_ = nullptr;
        }

        // Fetch a new memory block from the pool
        static RunPool<value_type>* Alloc() {
            RunPool<value_type>* ret = next_free_;
            next_free_++;
            return ret;
        }

        // Counts how many elements in the input sequence are not in ascending order
        int GetOrderLevel(RandomAccessIterator first, RandomAccessIterator last) {
            int k = 0;
            last -= 1;
            for(RandomAccessIterator i = first; i != last; ++i) {
                k += (*i > *(i+1));
            }
            return k;
        }

        std::size_t GetMemPoolSize(std::size_t num_elements, std::size_t num_runs) {

            std::size_t x = num_elements;
            while(x >= 100) {
                x /= 100;
            }
            std::size_t y = num_elements / x;

            return num_runs * std::sqrt(kBlockPoolFactor * (num_elements / y));
        }
    };

    template<typename RandomAccessIterator>
    RunPool<value_type_t<RandomAccessIterator>>* PatienceSorting<RandomAccessIterator>::memory_ = nullptr;

    template<typename RandomAccessIterator>
    RunPool<value_type_t<RandomAccessIterator>>* PatienceSorting<RandomAccessIterator>::next_free_ = nullptr;

    template<typename RandomAccessIterator>
    std::size_t PatienceSorting<RandomAccessIterator>::run_blocks_;

    template<typename RandomAccessIterator>
    auto patience_sort(RandomAccessIterator begin, RandomAccessIterator end)
        -> void
    {
        PatienceSorting<RandomAccessIterator> ps;
        ps.Sort(begin, end);
    }
}}

#endif // CPPSORT_DETAIL_PATIENCE_SORT_H_
