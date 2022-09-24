/*
 * Copyright (c) 2017-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */

//          Copyright Malte Skarupke 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef CPPSORT_DETAIL_SKA_SORT_H_
#define CPPSORT_DETAIL_SKA_SORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <climits>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <limits>
#include <tuple>
#include <type_traits>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/iter_move.h>
#include "attributes.h"
#include "iterator_traits.h" // projected_t
#include "memcpy_cast.h"
#include "partition.h"
#include "pdqsort.h"
#include "type_traits.h"

namespace cppsort
{
namespace detail
{
    ////////////////////////////////////////////////////////////
    // ska_sort algorithm

    template<typename Unsigned>
    auto to_unsigned_or_bool(Unsigned value)
        -> detail::enable_if_t<
            detail::is_unsigned<Unsigned>::value, // also covers bool
            Unsigned
        >
    {
        return value;
    }

    inline auto to_unsigned_or_bool(signed char c)
        -> unsigned char
    {
        return static_cast<unsigned char>(c) + 128;
    }

    inline auto to_unsigned_or_bool(char c)
        -> unsigned char
    {
        return static_cast<unsigned char>(c);
    }

    inline auto to_unsigned_or_bool(char16_t c)
        -> std::uint16_t
    {
        return static_cast<std::uint16_t>(c);
    }

    inline auto to_unsigned_or_bool(char32_t c)
        -> std::uint32_t
    {
        return static_cast<std::uint32_t>(c);
    }

    inline auto to_unsigned_or_bool(wchar_t c)
        -> std::uint32_t
    {
        return static_cast<std::uint32_t>(c);
    }

    inline auto to_unsigned_or_bool(short i)
        -> unsigned short
    {
        return static_cast<unsigned short>(i)
             + static_cast<unsigned short>(1 << std::numeric_limits<short>::digits);
    }

    inline auto to_unsigned_or_bool(int i)
        -> unsigned int
    {
        return static_cast<unsigned int>(i)
             + static_cast<unsigned int>(1 << std::numeric_limits<int>::digits);
    }

    inline auto to_unsigned_or_bool(long l)
        -> unsigned long
    {
        return static_cast<unsigned long>(l)
             + static_cast<unsigned long>(1l << std::numeric_limits<long>::digits);
    }

    inline auto to_unsigned_or_bool(long long l)
        -> unsigned long long
    {
        return static_cast<unsigned long long>(l)
             + static_cast<unsigned long long>(1ll << std::numeric_limits<long long>::digits);
    }

#ifdef __SIZEOF_INT128__
    inline auto to_unsigned_or_bool(__int128_t l)
        -> __uint128_t
    {
        return static_cast<__uint128_t>(l)
             + static_cast<__uint128_t>(__int128_t(1) << (CHAR_BIT * sizeof(__int128_t) - 1));
    }
#endif

    inline auto to_unsigned_or_bool(float f)
        -> std::uint32_t
    {
        auto u = memcpy_cast<std::uint32_t>(f);
        std::uint32_t sign_bit = -std::int32_t(u >> 31);
        return u ^ (sign_bit | 0x80000000);
    }

    inline auto to_unsigned_or_bool(double f)
        -> std::uint64_t
    {
        auto u = memcpy_cast<std::uint64_t>(f);
        std::uint64_t sign_bit = -std::int64_t(u >> 63);
        return u ^ (sign_bit | 0x8000000000000000);
    }

#ifdef UINTPTR_MAX
    template<typename T>
    auto to_unsigned_or_bool(T* ptr)
        -> std::uintptr_t
    {
        return reinterpret_cast<std::uintptr_t>(ptr);
    }
#else
    template<typename T>
    auto to_unsigned_or_bool(T* ptr)
        -> std::size_t
    {
        return reinterpret_cast<std::size_t>(ptr);
    }
#endif

    template<typename RandomAccessIterator, typename Function>
    auto unroll_loop_four_times(RandomAccessIterator begin, std::size_t iteration_count,
                                Function&& to_call_func)
        -> void
    {
        auto&& to_call = utility::as_function(to_call_func);

        std::size_t loop_count = iteration_count / 4;
        std::size_t remainder_count = iteration_count - loop_count * 4;
        for (; loop_count > 0 ; --loop_count) {
            to_call(begin);
            ++begin;
            to_call(begin);
            ++begin;
            to_call(begin);
            ++begin;
            to_call(begin);
            ++begin;
        }

        switch (remainder_count) {
            case 3:
                to_call(begin);
                ++begin;
                CPPSORT_ATTRIBUTE_FALLTHROUGH;
            case 2:
                to_call(begin);
                ++begin;
                CPPSORT_ATTRIBUTE_FALLTHROUGH;
            case 1:
                to_call(begin);
        }
    }

    struct PartitionInfo
    {
        PartitionInfo():
            count(0)
        {}

        union
        {
            std::size_t count;
            std::size_t offset;
        };
        std::size_t next_offset;
    };

    template<std::size_t>
    struct UnsignedForSize;

    template<>
    struct UnsignedForSize<1>
    {
        using type = std::uint8_t;
    };

    template<>
    struct UnsignedForSize<2>
    {
        using type = std::uint16_t;
    };

    template<>
    struct UnsignedForSize<4>
    {
        using type = std::uint32_t;
    };

    template<>
    struct UnsignedForSize<8>
    {
        using type = std::uint64_t;
    };

#ifdef __SIZEOF_INT128__
    template<>
    struct UnsignedForSize<16>
    {
        using type = __uint128_t;
    };
#endif

    template<typename T>
    struct SubKey;

    template<std::size_t Size>
    struct SizedSubKey
    {
        template<typename T>
        static auto sub_key(T&& value, void*)
            -> decltype(auto)
        {
            return to_unsigned_or_bool(value);
        }

        using next = SubKey<void>;
        using sub_key_type = typename UnsignedForSize<Size>::type;
    };

    template<typename T>
    struct SubKey<const T>:
        SubKey<T>
    {};

    template<typename T>
    struct SubKey<T&>:
        SubKey<T>
    {};

    template<typename T>
    struct SubKey<T&&>:
        SubKey<T>
    {};

    template<typename T>
    struct SubKey<const T&>:
        SubKey<T>
    {};

    template<typename T>
    struct SubKey<const T&&>:
        SubKey<T>
    {};

    template<typename T, typename Enable=void>
    struct FallbackSubKey:
        SubKey<decltype(to_radix_sort_key(std::declval<T>()))>
    {
        using base = SubKey<decltype(to_radix_sort_key(std::declval<T>()))>;

        template<typename U>
        static auto sub_key(U&& value, void* data)
            -> decltype(auto)
        {
            return base::sub_key(to_radix_sort_key(value), data);
        }
    };

    template<typename T>
    struct FallbackSubKey<T, detail::enable_if_t<not std::is_same<void, decltype(to_unsigned_or_bool(std::declval<T>()))>::value>>:
        SubKey<decltype(to_unsigned_or_bool(std::declval<T>()))>
    {};

    template<typename T>
    struct SubKey:
        conditional_t<
            is_unsigned<T>::value,
            SizedSubKey<sizeof(T)>,
            FallbackSubKey<T>
        >
    {};

    template<>
    struct SubKey<bool>
    {
        template<typename T>
        static auto sub_key(T&& value, void*)
            -> bool
        {
            return value;
        }

        using next = SubKey<void>;
        using sub_key_type = bool;
    };

    template<>
    struct SubKey<void>;

    template<typename T>
    struct SubKey<T*>:
        SizedSubKey<sizeof(T*)>
    {};

    template<typename F, typename S, typename Current>
    struct PairSecondSubKey:
        Current
    {
        static auto sub_key(const std::pair<F, S>& value, void* sort_data)
            -> decltype(auto)
        {
            return Current::sub_key(value.second, sort_data);
        }

        using next = conditional_t<
            std::is_same<SubKey<void>, typename Current::next>::value,
            SubKey<void>,
            PairSecondSubKey<F, S, typename Current::next>
        >;
    };

    template<typename F, typename S, typename Current>
    struct PairFirstSubKey:
        Current
    {
        static auto sub_key(const std::pair<F, S>& value, void* sort_data)
            -> decltype(auto)
        {
            return Current::sub_key(value.first, sort_data);
        }

        using next = conditional_t<
            std::is_same<SubKey<void>, typename Current::next>::value,
            PairSecondSubKey<F, S, SubKey<S>>,
            PairFirstSubKey<F, S, typename Current::next>
        >;
    };

    template<typename F, typename S>
    struct SubKey<std::pair<F, S>>:
        PairFirstSubKey<F, S, SubKey<F>>
    {};

    template<std::size_t Index, typename First, typename... More>
    struct TypeAt:
        TypeAt<Index - 1, More..., void>
    {};

    template<typename First, typename... More>
    struct TypeAt<0, First, More...>
    {
        using type = First;
    };

    template<std::size_t Index, typename Current, typename First, typename... More>
    struct TupleSubKey;

    template<std::size_t Index, typename Next, typename First, typename... More>
    struct NextTupleSubKey
    {
        using type = TupleSubKey<Index, Next, First, More...>;
    };

    template<std::size_t Index, typename First, typename Second, typename... More>
    struct NextTupleSubKey<Index, SubKey<void>, First, Second, More...>
    {
        using type = TupleSubKey<Index + 1, SubKey<Second>, Second, More...>;
    };

    template<std::size_t Index, typename First>
    struct NextTupleSubKey<Index, SubKey<void>, First>
    {
        using type = SubKey<void>;
    };

    template<std::size_t Index, typename Current, typename First, typename... More>
    struct TupleSubKey:
        Current
    {
        template<typename Tuple>
        static auto sub_key(const Tuple& value, void* sort_data)
            -> decltype(auto)
        {
            return Current::sub_key(std::get<Index>(value), sort_data);
        }

        using next = typename NextTupleSubKey<Index, typename Current::next, First, More...>::type;
    };

    template<std::size_t Index, typename Current, typename First>
    struct TupleSubKey<Index, Current, First>:
        Current
    {
        template<typename Tuple>
        static auto sub_key(const Tuple& value, void* sort_data)
            -> decltype(auto)
        {
            return Current::sub_key(std::get<Index>(value), sort_data);
        }

        using next = typename NextTupleSubKey<Index, typename Current::next, First>::type;
    };

    template<typename First, typename... More>
    struct SubKey<std::tuple<First, More...>>:
        TupleSubKey<0, SubKey<First>, First, More...>
    {};

    struct BaseListSortData
    {
        std::size_t current_index;
        std::size_t recursion_limit;
        void* next_sort_data;
    };

    template<typename RandomAccessIterator, typename Projection>
    struct ListSortData:
        BaseListSortData
    {
        void (*next_sort)(RandomAccessIterator, RandomAccessIterator,
                          std::ptrdiff_t, Projection, void*);
    };

    template<typename CurrentSubKey, typename T>
    struct ListElementSubKey:
        SubKey<remove_cvref_t<decltype(std::declval<T>()[0])>>
    {
        using base = SubKey<remove_cvref_t<decltype(std::declval<T>()[0])>>;
        using next = ListElementSubKey;

        template<typename U>
        static auto sub_key(U&& value, void* sort_data)
            -> decltype(auto)
        {
            BaseListSortData* list_sort_data = static_cast<BaseListSortData*>(sort_data);
            const T& list = CurrentSubKey::sub_key(value, list_sort_data->next_sort_data);
            return base::sub_key(list[list_sort_data->current_index], list_sort_data->next_sort_data);
        }
    };

    template<typename T>
    struct ListSubKey
    {
        using next = SubKey<void>;
        using sub_key_type = T;

        static auto sub_key(const T& value, void*)
            -> const T&
        {
            return value;
        }
    };

    template<typename T>
    struct FallbackSubKey<T, detail::enable_if_t<not std::is_same<void, decltype(std::declval<T>()[0])>::value>>:
        ListSubKey<T>
    {};

    template<typename RandomAccessIterator, typename Projection>
    auto StdSortFallback(RandomAccessIterator begin, RandomAccessIterator end, Projection projection)
        -> void
    {
        pdqsort(std::move(begin), std::move(end), std::less<>{}, std::move(projection));
    }

    template<std::ptrdiff_t StdSortThreshold, typename RandomAccessIterator, typename Projection>
    auto StdSortIfLessThanThreshold(RandomAccessIterator begin, RandomAccessIterator end,
                                    std::ptrdiff_t num_elements, Projection projection)
        -> bool
    {
        if (num_elements <= 1) {
            return true;
        }
        if (num_elements >= StdSortThreshold) {
            return false;
        }
        StdSortFallback(std::move(begin), std::move(end), std::move(projection));
        return true;
    }

    template<std::ptrdiff_t StdSortThreshold, std::ptrdiff_t AmericanFlagSortThreshold,
             typename CurrentSubKey, typename SubKeyType=typename CurrentSubKey::sub_key_type>
    struct InplaceSorter;

    template<std::ptrdiff_t StdSortThreshold, std::ptrdiff_t AmericanFlagSortThreshold,
             typename CurrentSubKey, std::size_t NumBytes, std::size_t Offset=0>
    struct UnsignedInplaceSorter
    {
        static constexpr std::size_t ShiftAmount = (((NumBytes - 1) - Offset) * 8);
        template<typename T>
        static auto current_byte(T&& elem, void* sort_data)
            -> std::uint8_t
        {
            return CurrentSubKey::sub_key(elem, sort_data) >> ShiftAmount;
        }

        template<typename RandomAccessIterator, typename Projection>
        static auto sort(RandomAccessIterator begin, RandomAccessIterator end, std::ptrdiff_t num_elements, Projection projection,
                         void (*next_sort)(RandomAccessIterator, RandomAccessIterator, std::ptrdiff_t, Projection, void*),
                         void* sort_data)
            -> void
        {
            if (num_elements < AmericanFlagSortThreshold) {
                american_flag_sort(std::move(begin), std::move(end), std::move(projection),
                                   next_sort, sort_data);
            } else {
                ska_byte_sort(std::move(begin), std::move(end), std::move(projection),
                              next_sort, sort_data);
            }
        }

        template<typename RandomAccessIterator, typename Projection>
        static auto sort_partition(RandomAccessIterator partition_begin, RandomAccessIterator partition_end,
                                   std::ptrdiff_t num_elements, Projection projection,
                                   void (*next_sort)(RandomAccessIterator, RandomAccessIterator, std::ptrdiff_t, Projection, void*),
                                   void* sort_data)
            -> void
        {
            if (not StdSortIfLessThanThreshold<StdSortThreshold>(partition_begin, partition_end, num_elements, projection)) {
                UnsignedInplaceSorter<StdSortThreshold, AmericanFlagSortThreshold,
                                      CurrentSubKey, NumBytes, Offset + 1>::sort(
                    partition_begin, partition_end, num_elements, projection, next_sort, sort_data);
            }
        }

        template<typename RandomAccessIterator, typename Projection>
        static auto american_flag_sort(RandomAccessIterator begin, RandomAccessIterator end, Projection projection,
                                       void (*next_sort)(RandomAccessIterator, RandomAccessIterator, std::ptrdiff_t, Projection, void*),
                                       void* sort_data)
            -> void
        {
            auto&& proj = utility::as_function(projection);

            PartitionInfo partitions[256];
            for (auto it = begin ; it != end ; ++it) {
                ++partitions[current_byte(proj(*it), sort_data)].count;
            }
            std::size_t total = 0;
            std::uint8_t remaining_partitions[256];
            int num_partitions = 0;
            for (int i = 0 ; i < 256 ; ++i) {
                std::size_t count = partitions[i].count;
                if (!count) continue;
                partitions[i].offset = total;
                total += count;
                partitions[i].next_offset = total;
                remaining_partitions[num_partitions] = i;
                ++num_partitions;
            }
            if (num_partitions > 1) {
                std::uint8_t* current_block_ptr = remaining_partitions;
                PartitionInfo* current_block = partitions + *current_block_ptr;
                std::uint8_t* last_block = remaining_partitions + num_partitions - 1;
                auto it = begin;
                auto block_end = begin + current_block->next_offset;
                auto last_element = end - 1;
                for (;;) {
                    PartitionInfo* block = partitions + current_byte(proj(*it), sort_data);
                    if (block == current_block) {
                        ++it;
                        if (it == last_element) {
                            break;
                        } else if (it == block_end) {
                            for (;;) {
                                ++current_block_ptr;
                                if (current_block_ptr == last_block) {
                                    goto recurse;
                                }
                                current_block = partitions + *current_block_ptr;
                                if (current_block->offset != current_block->next_offset) break;
                            }

                            it = begin + current_block->offset;
                            block_end = begin + current_block->next_offset;
                        }
                    } else {
                        std::size_t offset = block->offset++;
                        using utility::iter_swap;
                        iter_swap(it, begin + offset);
                    }
                }
            }
            recurse:
            if (Offset + 1 != NumBytes || next_sort) {
                std::size_t start_offset = 0;
                auto partition_begin = begin;
                for (std::uint8_t *it = remaining_partitions,
                                  *end = remaining_partitions + num_partitions ;
                     it != end ; ++it) {
                    std::size_t end_offset = partitions[*it].next_offset;
                    auto partition_end = begin + end_offset;
                    std::ptrdiff_t num_elements = end_offset - start_offset;
                    sort_partition(partition_begin, partition_end, num_elements, projection, next_sort, sort_data);
                    start_offset = end_offset;
                    partition_begin = partition_end;
                }
            }
        }

        template<typename RandomAccessIterator, typename Projection>
        static auto ska_byte_sort(RandomAccessIterator begin, RandomAccessIterator end, Projection projection,
                                  void (*next_sort)(RandomAccessIterator, RandomAccessIterator, std::ptrdiff_t, Projection, void*),
                                  void* sort_data)
            -> void
        {
            auto&& proj = utility::as_function(projection);

            PartitionInfo partitions[256];
            for (auto it = begin ; it != end ; ++it) {
                ++partitions[current_byte(proj(*it), sort_data)].count;
            }
            std::uint8_t remaining_partitions[256];
            std::size_t total = 0;
            int num_partitions = 0;
            for (int i = 0 ; i < 256 ; ++i) {
                std::size_t count = partitions[i].count;
                if (count) {
                    partitions[i].offset = total;
                    total += count;
                    remaining_partitions[num_partitions] = i;
                    ++num_partitions;
                }
                partitions[i].next_offset = total;
            }
            for (std::uint8_t *last_remaining = remaining_partitions + num_partitions,
                              *end_partition = remaining_partitions + 1 ;
                 last_remaining > end_partition ;) {
                last_remaining = detail::partition(remaining_partitions, last_remaining,
                                                    [&](std::uint8_t partition) {
                    std::size_t& begin_offset = partitions[partition].offset;
                    std::size_t& end_offset = partitions[partition].next_offset;
                    if (begin_offset == end_offset) {
                        return false;
                    }

                    unroll_loop_four_times(begin + begin_offset, end_offset - begin_offset,
                                           [partitions=partitions, begin, &proj, sort_data](RandomAccessIterator it) {
                        std::uint8_t this_partition = current_byte(proj(*it), sort_data);
                        std::size_t offset = partitions[this_partition].offset++;
                        using utility::iter_swap;
                        iter_swap(it, begin + offset);
                    });
                    return begin_offset != end_offset;
                });
            }
            if (Offset + 1 != NumBytes || next_sort) {
                for (std::uint8_t* it = remaining_partitions + num_partitions ; it != remaining_partitions ; --it) {
                    std::uint8_t partition = it[-1];
                    std::size_t start_offset = (partition == 0 ? 0 : partitions[partition - 1].next_offset);
                    std::size_t end_offset = partitions[partition].next_offset;
                    auto partition_begin = begin + start_offset;
                    auto partition_end = begin + end_offset;
                    std::ptrdiff_t num_elements = end_offset - start_offset;
                    sort_partition(partition_begin, partition_end, num_elements, projection, next_sort, sort_data);
                }
            }
        }
    };

    template<std::ptrdiff_t StdSortThreshold, std::ptrdiff_t AmericanFlagSortThreshold,
             typename CurrentSubKey, size_t NumBytes>
    struct UnsignedInplaceSorter<StdSortThreshold, AmericanFlagSortThreshold, CurrentSubKey, NumBytes, NumBytes>
    {
        template<typename RandomAccessIterator, typename Projection>
        static auto sort(RandomAccessIterator begin, RandomAccessIterator end,
                         std::ptrdiff_t num_elements, Projection projection,
                         void (*next_sort)(RandomAccessIterator, RandomAccessIterator, std::ptrdiff_t, Projection, void*),
                         void* next_sort_data)
            -> void
        {
            next_sort(std::move(begin), std::move(end), num_elements,
                      std::move(projection), next_sort_data);
        }
    };

    template<typename RandomAccessIterator, typename Projection, typename ElementKey>
    auto CommonPrefix(RandomAccessIterator begin, RandomAccessIterator end,
                      std::size_t start_index, Projection projection, ElementKey&& element_key)
        -> std::size_t
    {
        auto proj = utility::as_function(projection);

        const auto& largest_match_list = proj(*begin);
        std::size_t largest_match = largest_match_list.size();
        if (largest_match == start_index) {
            return start_index;
        }

        for (++begin ; begin != end ; ++begin) {
            const auto& current_list = proj(*begin);
            std::size_t current_size = current_list.size();
            if (current_size < largest_match) {
                largest_match = current_size;
                if (largest_match == start_index) {
                    return start_index;
                }
            }
            if (element_key(largest_match_list[start_index]) != element_key(current_list[start_index])) {
                return start_index;
            }
            for (std::size_t i = start_index + 1 ; i < largest_match ; ++i) {
                if (element_key(largest_match_list[i]) != element_key(current_list[i])) {
                    largest_match = i;
                    break;
                }
            }
        }
        return largest_match;
    }

    template<std::ptrdiff_t StdSortThreshold, std::ptrdiff_t AmericanFlagSortThreshold,
             typename CurrentSubKey, typename ListType>
    struct ListInplaceSorter
    {
        using ElementSubKey = ListElementSubKey<CurrentSubKey, ListType>;
        template<typename RandomAccessIterator, typename Projection>
        static auto sort(RandomAccessIterator begin, RandomAccessIterator end, Projection projection,
                         ListSortData<RandomAccessIterator, Projection>* sort_data)
            -> void
        {
            auto&& proj = utility::as_function(projection);

            std::size_t current_index = sort_data->current_index;
            void* next_sort_data = sort_data->next_sort_data;
            auto current_key = [&](auto&& elem) -> decltype(auto) {
                return CurrentSubKey::sub_key(proj(elem), next_sort_data);
            };
            auto element_key = [&](auto&& elem) -> decltype(auto) {
                return ElementSubKey::base::sub_key(elem, sort_data);
            };
            sort_data->current_index = current_index = CommonPrefix(begin, end, current_index, current_key, element_key);
            auto end_of_shorter_ones = detail::partition(begin, end, [&](auto&& elem) {
                return current_key(elem).size() <= current_index;
            });
            std::ptrdiff_t num_shorter_ones = end_of_shorter_ones - begin;
            if (sort_data->next_sort && not StdSortIfLessThanThreshold<StdSortThreshold>(begin, end_of_shorter_ones,
                                                                                      num_shorter_ones, projection)) {
                sort_data->next_sort(begin, end_of_shorter_ones, num_shorter_ones, projection, next_sort_data);
            }
            std::ptrdiff_t num_elements = end - end_of_shorter_ones;
            if (not StdSortIfLessThanThreshold<StdSortThreshold>(end_of_shorter_ones, end, num_elements, projection)) {
                using SortType = void (*)(RandomAccessIterator, RandomAccessIterator, std::ptrdiff_t, Projection, void*);
                SortType sort_next_element = static_cast<SortType>(&sort_from_recursion);
                InplaceSorter<StdSortThreshold, AmericanFlagSortThreshold, ElementSubKey>::sort(end_of_shorter_ones, end, num_elements,
                                                                                                projection, sort_next_element, sort_data);
            }
        }

        template<typename RandomAccessIterator, typename Projection>
        static auto sort_from_recursion(RandomAccessIterator begin, RandomAccessIterator end,
                                        std::ptrdiff_t, Projection projection, void* next_sort_data)
            -> void
        {
            ListSortData<RandomAccessIterator, Projection> offset
                = *static_cast<ListSortData<RandomAccessIterator, Projection>*>(next_sort_data);
            ++offset.current_index;
            --offset.recursion_limit;
            if (offset.recursion_limit == 0) {
                StdSortFallback(begin, end, projection);
            } else {
                sort(begin, end, projection, &offset);
            }
        }

        template<typename RandomAccessIterator, typename Projection>
        static auto sort(RandomAccessIterator begin, RandomAccessIterator end,
                         std::ptrdiff_t, Projection projection,
                         void (*next_sort)(RandomAccessIterator, RandomAccessIterator, std::ptrdiff_t, Projection, void*),
                         void *next_sort_data)
            -> void
        {
            ListSortData<RandomAccessIterator, Projection> offset;
            offset.current_index = 0;
            offset.recursion_limit = 16;
            offset.next_sort = next_sort;
            offset.next_sort_data = next_sort_data;
            sort(begin, end, projection, &offset);
        }
    };

    template<std::ptrdiff_t StdSortThreshold, std::ptrdiff_t AmericanFlagSortThreshold, typename CurrentSubKey>
    struct InplaceSorter<StdSortThreshold, AmericanFlagSortThreshold, CurrentSubKey, bool>
    {
        template<typename RandomAccessIterator, typename Projection>
        static auto sort(RandomAccessIterator begin, RandomAccessIterator end,
                         std::ptrdiff_t, Projection projection,
                         void (*next_sort)(RandomAccessIterator, RandomAccessIterator, std::ptrdiff_t, Projection, void*),
                         void* sort_data)
            -> void
        {
            auto&& proj = utility::as_function(projection);

            auto middle = detail::partition(begin, end, [&](auto&& a) {
                return not CurrentSubKey::sub_key(proj(a), sort_data);
            });
            if (next_sort) {
                next_sort(begin, middle, middle - begin, projection, sort_data);
                next_sort(middle, end, end - middle, projection, sort_data);
            }
        }
    };

    template<std::ptrdiff_t StdSortThreshold, std::ptrdiff_t AmericanFlagSortThreshold, typename CurrentSubKey>
    struct InplaceSorter<StdSortThreshold, AmericanFlagSortThreshold, CurrentSubKey, std::uint8_t>:
        UnsignedInplaceSorter<StdSortThreshold, AmericanFlagSortThreshold, CurrentSubKey, 1>
    {};

    template<std::ptrdiff_t StdSortThreshold, std::ptrdiff_t AmericanFlagSortThreshold, typename CurrentSubKey>
    struct InplaceSorter<StdSortThreshold, AmericanFlagSortThreshold, CurrentSubKey, std::uint16_t>:
        UnsignedInplaceSorter<StdSortThreshold, AmericanFlagSortThreshold, CurrentSubKey, 2>
    {};

    template<std::ptrdiff_t StdSortThreshold, std::ptrdiff_t AmericanFlagSortThreshold, typename CurrentSubKey>
    struct InplaceSorter<StdSortThreshold, AmericanFlagSortThreshold, CurrentSubKey, std::uint32_t>:
        UnsignedInplaceSorter<StdSortThreshold, AmericanFlagSortThreshold, CurrentSubKey, 4>
    {};

    template<std::ptrdiff_t StdSortThreshold, std::ptrdiff_t AmericanFlagSortThreshold, typename CurrentSubKey>
    struct InplaceSorter<StdSortThreshold, AmericanFlagSortThreshold, CurrentSubKey, std::uint64_t>:
        UnsignedInplaceSorter<StdSortThreshold, AmericanFlagSortThreshold, CurrentSubKey, 8>
    {};

#ifdef __SIZEOF_INT128__
    template<std::ptrdiff_t StdSortThreshold, std::ptrdiff_t AmericanFlagSortThreshold, typename CurrentSubKey>
    struct InplaceSorter<StdSortThreshold, AmericanFlagSortThreshold, CurrentSubKey, __uint128_t>:
        UnsignedInplaceSorter<StdSortThreshold, AmericanFlagSortThreshold, CurrentSubKey, 16>
    {};
#endif

    template<std::ptrdiff_t StdSortThreshold, std::ptrdiff_t AmericanFlagSortThreshold,
             typename CurrentSubKey, typename SubKeyType, typename Enable=void>
    struct FallbackInplaceSorter;

    template<std::ptrdiff_t StdSortThreshold, std::ptrdiff_t AmericanFlagSortThreshold,
             typename CurrentSubKey, typename SubKeyType>
    struct InplaceSorter:
        FallbackInplaceSorter<StdSortThreshold, AmericanFlagSortThreshold, CurrentSubKey, SubKeyType>
    {};

    template<std::ptrdiff_t StdSortThreshold, std::ptrdiff_t AmericanFlagSortThreshold,
             typename CurrentSubKey, typename SubKeyType>
    struct FallbackInplaceSorter<StdSortThreshold, AmericanFlagSortThreshold, CurrentSubKey, SubKeyType,
                                 detail::enable_if_t<not std::is_same<void, decltype(std::declval<SubKeyType>()[0])>::value>>:
        ListInplaceSorter<StdSortThreshold, AmericanFlagSortThreshold, CurrentSubKey, SubKeyType>
    {};

    template<std::ptrdiff_t StdSortThreshold, std::ptrdiff_t AmericanFlagSortThreshold, typename CurrentSubKey>
    struct SortStarter;

    template<std::ptrdiff_t StdSortThreshold, std::ptrdiff_t AmericanFlagSortThreshold>
    struct SortStarter<StdSortThreshold, AmericanFlagSortThreshold, SubKey<void>>
    {
        template<typename RandomAccessIterator, typename Projection>
        static auto sort(RandomAccessIterator, RandomAccessIterator, std::ptrdiff_t, Projection, void*)
            -> void
        {}
    };

    template<std::ptrdiff_t StdSortThreshold, std::ptrdiff_t AmericanFlagSortThreshold, typename CurrentSubKey>
    struct SortStarter
    {
        template<typename RandomAccessIterator, typename Projection>
        static auto sort(RandomAccessIterator begin, RandomAccessIterator end,
                         std::ptrdiff_t num_elements, Projection projection,
                         void* next_sort_data=nullptr)
            -> void
        {
            if (StdSortIfLessThanThreshold<StdSortThreshold>(begin, end, num_elements, projection)) {
                return;
            }

            using SortType = void (*)(RandomAccessIterator, RandomAccessIterator, std::ptrdiff_t, Projection, void*);
            SortType next_sort = static_cast<SortType>(&SortStarter<StdSortThreshold,
                                                                    AmericanFlagSortThreshold,
                                                                    typename CurrentSubKey::next>::sort);
            if (next_sort == static_cast<SortType>(&SortStarter<StdSortThreshold, AmericanFlagSortThreshold, SubKey<void>>::sort)) {
                next_sort = nullptr;
            }
            InplaceSorter<StdSortThreshold, AmericanFlagSortThreshold, CurrentSubKey>::sort(
                std::move(begin), std::move(end), num_elements,
                std::move(projection), next_sort, next_sort_data);
        }
    };

    template<
        std::ptrdiff_t StdSortThreshold,
        std::ptrdiff_t AmericanFlagSortThreshold,
        typename RandomAccessIterator,
        typename Projection
    >
    auto inplace_radix_sort(RandomAccessIterator begin, RandomAccessIterator end,
                            Projection projection)
        -> void
    {
        using SubKey = SubKey<projected_t<RandomAccessIterator, Projection>>;
        SortStarter<StdSortThreshold, AmericanFlagSortThreshold, SubKey>::sort(begin, end, end - begin,
                                                                               std::move(projection));
    }

    template<typename RandomAccessIterator, typename Projection>
    auto ska_sort(RandomAccessIterator begin, RandomAccessIterator end,
                  Projection projection)
        -> void
    {
        detail::inplace_radix_sort<128, 1024>(std::move(begin), std::move(end),
                                              std::move(projection));
    }

    ////////////////////////////////////////////////////////////
    // Whether a type is sortable with ska_sort

    template<typename T>
    struct is_ska_sortable;

    template<typename T>
    using has_indexing_operator_t
        = remove_cvref_t<decltype(std::declval<T&>()[0])>;

    template<template<typename...> class Op, typename... Args>
    using is_index_ska_sortable = is_ska_sortable<detected_t<Op, Args...>>;

    // A bit hackish, but I'm bad at workarounds...
    template<>
    struct is_ska_sortable<nonesuch>:
        std::false_type
    {};

    template<typename T>
    struct is_ska_sortable:
        disjunction<
            is_integral<T>,
            is_index_ska_sortable<has_indexing_operator_t, T>
        >
    {};

    template<typename T>
    struct is_ska_sortable<T*>:
        std::true_type
    {};

    template<>
    struct is_ska_sortable<float>:
        std::integral_constant<bool,
            sizeof(float) == sizeof(std::uint32_t) &&
            std::numeric_limits<float>::is_iec559
        >
    {};

    template<>
    struct is_ska_sortable<double>:
        std::integral_constant<bool,
            sizeof(double) == sizeof(std::uint64_t) &&
            std::numeric_limits<double>::is_iec559
        >
    {};

    template<typename T, typename U>
    struct is_ska_sortable<std::pair<T, U>>:
        conjunction<
            is_ska_sortable<T>,
            is_ska_sortable<U>
        >
    {};

    template<typename... Args>
    struct is_ska_sortable<std::tuple<Args...>>:
        conjunction<
            is_ska_sortable<Args>...
        >
    {};

    template<typename T>
    constexpr bool is_ska_sortable_v = is_ska_sortable<T>::value;
}}

#endif // CPPSORT_DETAIL_SKA_SORT_H_
