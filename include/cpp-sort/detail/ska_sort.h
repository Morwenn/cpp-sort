//          Copyright Malte Skarupke 2016.
//  Modified in 2017 by Morwenn for inclusion into cpp-sort.
// Distributed under the Boost Software License, Version 1.0.
//    (See http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef CPPSORT_DETAIL_SKA_SORT_H_
#define CPPSORT_DETAIL_SKA_SORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <tuple>
#include <type_traits>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/iter_move.h>

namespace cppsort
{
namespace detail
{
    template<typename count_type, typename RandomAccessIterator, typename OutIt, typename Projection>
    auto counting_sort_impl(RandomAccessIterator begin, RandomAccessIterator end,
                            OutIt out_begin, Projection projection)
        -> void
    {
        auto&& proj = utility::as_function(projection);
        using utility::iter_move;

        count_type counts[256] = {};
        for (auto it = begin ; it != end ; ++it) {
            ++counts[proj(*it)];
        }
        count_type total = 0;
        for (count_type& count: counts) {
            count_type old_count = count;
            count = total;
            total += old_count;
        }
        for (; begin != end ; ++begin) {
            std::uint8_t key = proj(*begin);
            out_begin[counts[key]++] = iter_move(begin);
        }
    }

    template<typename RandomAccessIterator, typename OutIt, typename Projection>
    auto counting_sort_impl(RandomAccessIterator begin, RandomAccessIterator end,
                            OutIt out_begin, Projection projection)
        -> void
    {
        counting_sort_impl<std::uint64_t>(std::move(begin), std::move(end),
                                          out_begin, std::move(projection));
    }

    inline auto to_unsigned_or_bool(bool b)
        -> bool
    {
        return b;
    }

    inline auto to_unsigned_or_bool(unsigned char c)
        -> unsigned char
    {
        return c;
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
        return static_cast<unsigned short>(i) + static_cast<unsigned short>(1 << (sizeof(short) * 8 - 1));
    }

    inline auto to_unsigned_or_bool(unsigned short i)
        -> unsigned short
    {
        return i;
    }

    inline auto to_unsigned_or_bool(int i)
        -> unsigned int
    {
        return static_cast<unsigned int>(i) + static_cast<unsigned int>(1 << (sizeof(int) * 8 - 1));
    }

    inline auto to_unsigned_or_bool(unsigned int i)
        -> unsigned int
    {
        return i;
    }

    inline auto to_unsigned_or_bool(long l)
        -> unsigned long
    {
        return static_cast<unsigned long>(l) + static_cast<unsigned long>(1l << (sizeof(long) * 8 - 1));
    }

    inline auto to_unsigned_or_bool(unsigned long l)
        -> unsigned long
    {
        return l;
    }

    inline auto to_unsigned_or_bool(long long l)
        -> unsigned long long
    {
        return static_cast<unsigned long long>(l) + static_cast<unsigned long long>(1ll << (sizeof(long long) * 8 - 1));
    }

    inline auto to_unsigned_or_bool(unsigned long long l)
        -> unsigned long long
    {
        return l;
    }

    inline auto to_unsigned_or_bool(float f)
        -> std::uint32_t
    {
        union
        {
            float f;
            std::uint32_t u;
        } as_union = { f };
        std::uint32_t sign_bit = -std::int32_t(as_union.u >> 31);
        return as_union.u ^ (sign_bit | 0x80000000);
    }

    inline auto to_unsigned_or_bool(double f)
        -> std::uint64_t
    {
        union
        {
            double d;
            std::uint64_t u;
        } as_union = { f };
        std::uint64_t sign_bit = -std::int64_t(as_union.u >> 63);
        return as_union.u ^ (sign_bit | 0x8000000000000000);
    }

    template<typename T>
    auto to_unsigned_or_bool(T* ptr)
        -> std::size_t
    {
        return reinterpret_cast<std::size_t>(ptr);
    }

    template<size_t>
    struct SizedRadixSorter;

    template<>
    struct SizedRadixSorter<1>
    {
        template<typename RandomAccessIterator, typename OutIt, typename Projection>
        static auto sort(RandomAccessIterator begin, RandomAccessIterator end,
                         OutIt buffer_begin, Projection projection)
            -> bool
        {
            auto&& proj = utility::as_function(projection);
            counting_sort_impl(begin, end, buffer_begin, [&](auto&& o) {
                return to_unsigned_or_bool(proj(o));
            });
            return true;
        }

        static constexpr std::size_t pass_count = 2;
    };

    template<>
    struct SizedRadixSorter<2>
    {
        template<typename RandomAccessIterator, typename OutIt, typename Projection>
        static auto sort(RandomAccessIterator begin, RandomAccessIterator end,
                         OutIt buffer_begin, Projection projection)
            -> bool
        {
            std::ptrdiff_t num_elements = std::distance(begin, end);
            if (num_elements <= (1ll << 32)) {
                return sort_inline<std::uint32_t>(begin, end, buffer_begin, buffer_begin + num_elements, projection);
            } else {
                return sort_inline<std::uint64_t>(begin, end, buffer_begin, buffer_begin + num_elements, projection);
            }
        }

        template<typename count_type, typename RandomAccessIterator, typename OutIt, typename Projection>
        static auto sort_inline(RandomAccessIterator begin, RandomAccessIterator end,
                                OutIt out_begin, OutIt out_end, Projection projection)
            -> bool
        {
            auto&& proj = utility::as_function(projection);
            using utility::iter_move;

            count_type counts0[256] = {};
            count_type counts1[256] = {};

            for (auto it = begin ; it != end ; ++it) {
                std::uint16_t key = to_unsigned_or_bool(proj(*it));
                ++counts0[key & 0xff];
                ++counts1[(key >> 8) & 0xff];
            }
            count_type total0 = 0;
            count_type total1 = 0;
            for (int i = 0 ; i < 256 ; ++i) {
                count_type old_count0 = counts0[i];
                count_type old_count1 = counts1[i];
                counts0[i] = total0;
                counts1[i] = total1;
                total0 += old_count0;
                total1 += old_count1;
            }
            for (auto it = begin ; it != end ; ++it) {
                std::uint8_t key = to_unsigned_or_bool(proj(*it));
                out_begin[counts0[key]++] = iter_move(it);
            }
            for (OutIt it = out_begin ; it != out_end ; ++it) {
                std::uint8_t key = to_unsigned_or_bool(proj(*it)) >> 8;
                begin[counts1[key]++] = iter_move(it);
            }
            return false;
        }

        static constexpr std::size_t pass_count = 3;
    };

    template<>
    struct SizedRadixSorter<4>
    {

        template<typename RandomAccessIterator, typename OutIt, typename Projection>
        static auto sort(RandomAccessIterator begin, RandomAccessIterator end,
                         OutIt buffer_begin, Projection projection)
            -> bool
        {
            std::ptrdiff_t num_elements = std::distance(begin, end);
            if (num_elements <= (1ll << 32)) {
                return sort_inline<std::uint32_t>(begin, end, buffer_begin, buffer_begin + num_elements, projection);
            } else {
                return sort_inline<std::uint64_t>(begin, end, buffer_begin, buffer_begin + num_elements, projection);
            }
        }

        template<typename count_type, typename RandomAccessIterator, typename OutIt, typename Projection>
        static auto sort_inline(RandomAccessIterator begin, RandomAccessIterator end,
                                OutIt out_begin, OutIt out_end, Projection projection)
            -> bool
        {
            auto&& proj = utility::as_function(projection);
            using utility::iter_move;

            count_type counts0[256] = {};
            count_type counts1[256] = {};
            count_type counts2[256] = {};
            count_type counts3[256] = {};

            for (auto it = begin ; it != end ; ++it) {
                std::uint32_t key = to_unsigned_or_bool(proj(*it));
                ++counts0[key & 0xff];
                ++counts1[(key >> 8) & 0xff];
                ++counts2[(key >> 16) & 0xff];
                ++counts3[(key >> 24) & 0xff];
            }
            count_type total0 = 0;
            count_type total1 = 0;
            count_type total2 = 0;
            count_type total3 = 0;
            for (int i = 0 ; i < 256 ; ++i) {
                count_type old_count0 = counts0[i];
                count_type old_count1 = counts1[i];
                count_type old_count2 = counts2[i];
                count_type old_count3 = counts3[i];
                counts0[i] = total0;
                counts1[i] = total1;
                counts2[i] = total2;
                counts3[i] = total3;
                total0 += old_count0;
                total1 += old_count1;
                total2 += old_count2;
                total3 += old_count3;
            }
            for (auto it = begin ; it != end ; ++it) {
                std::uint8_t key = to_unsigned_or_bool(proj(*it));
                out_begin[counts0[key]++] = iter_move(it);
            }
            for (OutIt it = out_begin ; it != out_end ; ++it) {
                std::uint8_t key = to_unsigned_or_bool(proj(*it)) >> 8;
                begin[counts1[key]++] = iter_move(it);
            }
            for (auto it = begin ; it != end ; ++it) {
                std::uint8_t key = to_unsigned_or_bool(proj(*it)) >> 16;
                out_begin[counts2[key]++] = iter_move(it);
            }
            for (OutIt it = out_begin ; it != out_end ; ++it) {
                std::uint8_t key = to_unsigned_or_bool(proj(*it)) >> 24;
                begin[counts3[key]++] = iter_move(it);
            }
            return false;
        }

        static constexpr std::size_t pass_count = 5;
    };

    template<>
    struct SizedRadixSorter<8>
    {
        template<typename RandomAccessIterator, typename OutIt, typename Projection>
        static auto sort(RandomAccessIterator begin, RandomAccessIterator end,
                         OutIt buffer_begin, Projection projection)
            -> bool
        {
            std::ptrdiff_t num_elements = std::distance(begin, end);
            if (num_elements <= (1ll << 32)) {
                return sort_inline<std::uint32_t>(begin, end, buffer_begin, buffer_begin + num_elements, projection);
            } else {
                return sort_inline<std::uint64_t>(begin, end, buffer_begin, buffer_begin + num_elements, projection);
            }
        }

        template<typename count_type, typename RandomAccessIterator, typename OutIt, typename Projection>
        static auto sort_inline(RandomAccessIterator begin, RandomAccessIterator end,
                                OutIt out_begin, OutIt out_end, Projection projection)
            -> bool
        {
            auto&& proj = utility::as_function(projection);
            using utility::iter_move;

            count_type counts0[256] = {};
            count_type counts1[256] = {};
            count_type counts2[256] = {};
            count_type counts3[256] = {};
            count_type counts4[256] = {};
            count_type counts5[256] = {};
            count_type counts6[256] = {};
            count_type counts7[256] = {};

            for (auto it = begin ; it != end ; ++it) {
                std::uint64_t key = to_unsigned_or_bool(proj(*it));
                ++counts0[key & 0xff];
                ++counts1[(key >> 8) & 0xff];
                ++counts2[(key >> 16) & 0xff];
                ++counts3[(key >> 24) & 0xff];
                ++counts4[(key >> 32) & 0xff];
                ++counts5[(key >> 40) & 0xff];
                ++counts6[(key >> 48) & 0xff];
                ++counts7[(key >> 56) & 0xff];
            }
            count_type total0 = 0;
            count_type total1 = 0;
            count_type total2 = 0;
            count_type total3 = 0;
            count_type total4 = 0;
            count_type total5 = 0;
            count_type total6 = 0;
            count_type total7 = 0;
            for (int i = 0 ; i < 256 ; ++i) {
                count_type old_count0 = counts0[i];
                count_type old_count1 = counts1[i];
                count_type old_count2 = counts2[i];
                count_type old_count3 = counts3[i];
                count_type old_count4 = counts4[i];
                count_type old_count5 = counts5[i];
                count_type old_count6 = counts6[i];
                count_type old_count7 = counts7[i];
                counts0[i] = total0;
                counts1[i] = total1;
                counts2[i] = total2;
                counts3[i] = total3;
                counts4[i] = total4;
                counts5[i] = total5;
                counts6[i] = total6;
                counts7[i] = total7;
                total0 += old_count0;
                total1 += old_count1;
                total2 += old_count2;
                total3 += old_count3;
                total4 += old_count4;
                total5 += old_count5;
                total6 += old_count6;
                total7 += old_count7;
            }
            for (auto it = begin ; it != end ; ++it) {
                std::uint8_t key = to_unsigned_or_bool(proj(*it));
                out_begin[counts0[key]++] = iter_move(it);
            }
            for (OutIt it = out_begin ; it != out_end ; ++it) {
                std::uint8_t key = to_unsigned_or_bool(proj(*it)) >> 8;
                begin[counts1[key]++] = iter_move(it);
            }
            for (auto it = begin ; it != end ; ++it) {
                std::uint8_t key = to_unsigned_or_bool(proj(*it)) >> 16;
                out_begin[counts2[key]++] = iter_move(it);
            }
            for (OutIt it = out_begin ; it != out_end ; ++it) {
                std::uint8_t key = to_unsigned_or_bool(proj(*it)) >> 24;
                begin[counts3[key]++] = iter_move(it);
            }
            for (auto it = begin ; it != end ; ++it) {
                std::uint8_t key = to_unsigned_or_bool(proj(*it)) >> 32;
                out_begin[counts4[key]++] = iter_move(it);
            }
            for (OutIt it = out_begin ; it != out_end ; ++it) {
                std::uint8_t key = to_unsigned_or_bool(proj(*it)) >> 40;
                begin[counts5[key]++] = iter_move(it);
            }
            for (auto it = begin ; it != end ; ++it) {
                std::uint8_t key = to_unsigned_or_bool(proj(*it)) >> 48;
                out_begin[counts6[key]++] = iter_move(it);
            }
            for (OutIt it = out_begin ; it != out_end ; ++it) {
                std::uint8_t key = to_unsigned_or_bool(proj(*it)) >> 56;
                begin[counts7[key]++] = iter_move(it);
            }
            return false;
        }

        static constexpr std::size_t pass_count = 9;
    };

    template<typename>
    struct RadixSorter;

    template<>
    struct RadixSorter<bool>
    {
        template<typename RandomAccessIterator, typename OutIt, typename Projection>
        static auto sort(RandomAccessIterator begin, RandomAccessIterator end,
                         OutIt buffer_begin, Projection projection)
            -> bool
        {
            auto&& proj = utility::as_function(projection);
            using utility::iter_move;

            std::size_t false_count = 0;
            for (auto it = begin ; it != end ; ++it) {
                if (not prj(*it)) {
                    ++false_count;
                }
            }
            std::size_t true_position = false_count;
            false_count = 0;
            for (; begin != end ; ++begin) {
                if (proj(*begin)) {
                    buffer_begin[true_position++] = iter_move(begin);
                } else {
                    buffer_begin[false_count++] = iter_move(begin);
                }
            }
            return true;
        }

        static constexpr std::size_t pass_count = 2;
    };

    template<>
    struct RadixSorter<signed char>:
        SizedRadixSorter<sizeof(signed char)>
    {};

    template<>
    struct RadixSorter<unsigned char>:
        SizedRadixSorter<sizeof(unsigned char)>
    {};

    template<>
    struct RadixSorter<signed short>:
        SizedRadixSorter<sizeof(signed short)>
    {};

    template<>
    struct RadixSorter<unsigned short>:
        SizedRadixSorter<sizeof(unsigned short)>
    {};

    template<>
    struct RadixSorter<signed int>:
        SizedRadixSorter<sizeof(signed int)>
    {};

    template<>
    struct RadixSorter<unsigned int>:
        SizedRadixSorter<sizeof(unsigned int)>
    {};

    template<>
    struct RadixSorter<signed long>:
        SizedRadixSorter<sizeof(signed long)>
    {};

    template<>
    struct RadixSorter<unsigned long>:
        SizedRadixSorter<sizeof(unsigned long)>
    {};

    template<>
    struct RadixSorter<signed long long>:
        SizedRadixSorter<sizeof(signed long long)>
    {};

    template<>
    struct RadixSorter<unsigned long long>:
        SizedRadixSorter<sizeof(unsigned long long)>
    {};

    template<>
    struct RadixSorter<float>:
        SizedRadixSorter<sizeof(float)>
    {};

    template<>
    struct RadixSorter<double>:
        SizedRadixSorter<sizeof(double)>
    {};

    template<>
    struct RadixSorter<char>:
        SizedRadixSorter<sizeof(char)>
    {};

    template<>
    struct RadixSorter<wchar_t>:
        SizedRadixSorter<sizeof(wchar_t)>
    {};

    template<>
    struct RadixSorter<char16_t>:
        SizedRadixSorter<sizeof(char16_t)>
    {};

    template<>
    struct RadixSorter<char32_t>:
        SizedRadixSorter<sizeof(char32_t)>
    {};

    template<typename K, typename V>
    struct RadixSorter<std::pair<K, V>>
    {
        template<typename RandomAccessIterator, typename OutIt, typename Projection>
        static auto sort(RandomAccessIterator begin, RandomAccessIterator end,
                         OutIt buffer_begin, Projection projection)
            -> bool
        {
            auto&& proj = utility::as_function(projection);

            bool first_result = RadixSorter<V>::sort(begin, end, buffer_begin, [&](auto&& o) {
                return proj(o).second;
            });
            auto extract_first = [&](auto&& o) {
                return proj(o).first;
            };

            if (first_result) {
                return not RadixSorter<K>::sort(buffer_begin, buffer_begin + (end - begin), begin, extract_first);
            } else {
                return RadixSorter<K>::sort(begin, end, buffer_begin, extract_first);
            }
        }

        static constexpr size_t pass_count = RadixSorter<K>::pass_count + RadixSorter<V>::pass_count;
    };

    template<typename K, typename V>
    struct RadixSorter<const std::pair<K, V>&>
    {
        template<typename RandomAccessIterator, typename OutIt, typename Projection>
        static auto sort(RandomAccessIterator begin, RandomAccessIterator end,
                         OutIt buffer_begin, Projection projection)
            -> bool
        {
            auto&& proj = utility::as_function(projection);

            bool first_result = RadixSorter<V>::sort(begin, end, buffer_begin, [&](auto&& o) -> const V& {
                return proj(o).second;
            });
            auto extract_first = [&](auto&& o) -> const K& {
                return proj(o).first;
            };

            if (first_result) {
                return not RadixSorter<K>::sort(buffer_begin, buffer_begin + (end - begin), begin, extract_first);
            } else {
                return RadixSorter<K>::sort(begin, end, buffer_begin, extract_first);
            }
        }

        static constexpr std::size_t pass_count = RadixSorter<K>::pass_count + RadixSorter<V>::pass_count;
    };

    template<std::size_t I, std::size_t S, typename Tuple>
    struct TupleRadixSorter
    {
        using NextSorter = TupleRadixSorter<I + 1, S, Tuple>;
        using ThisSorter = RadixSorter<typename std::tuple_element<I, Tuple>::type>;

        template<typename RandomAccessIterator, typename OutIt, typename Projection>
        static auto sort(RandomAccessIterator begin, RandomAccessIterator end,
                         OutIt out_begin, OutIt out_end, Projection projection)
            -> bool
        {
            auto&& proj = utility::as_function(projection);

            bool which = NextSorter::sort(begin, end, out_begin, out_end, projection);
            auto extract_i = [&](auto&& o) {
                return std::get<I>(proj(o));
            };
            if (which) {
                return not ThisSorter::sort(out_begin, out_end, begin, extract_i);
            } else {
                return ThisSorter::sort(begin, end, out_begin, extract_i);
            }
        }

        static constexpr std::size_t pass_count = ThisSorter::pass_count + NextSorter::pass_count;
    };

    template<std::size_t I, std::size_t S, typename Tuple>
    struct TupleRadixSorter<I, S, const Tuple&>
    {
        using NextSorter = TupleRadixSorter<I + 1, S, const Tuple&>;
        using ThisSorter = RadixSorter<typename std::tuple_element<I, Tuple>::type>;

        template<typename RandomAccessIterator, typename OutIt, typename Projection>
        static auto sort(RandomAccessIterator begin, RandomAccessIterator end,
                         OutIt out_begin, OutIt out_end, Projection projection)
            -> bool
        {
            auto&& proj = utility::as_function(projection);

            bool which = NextSorter::sort(begin, end, out_begin, out_end, projection);
            auto extract_i = [&](auto&& o) -> decltype(auto) {
                return std::get<I>(proj(o));
            };
            if (which) {
                return not ThisSorter::sort(out_begin, out_end, begin, extract_i);
            } else {
                return ThisSorter::sort(begin, end, out_begin, extract_i);
            }
        }

        static constexpr size_t pass_count = ThisSorter::pass_count + NextSorter::pass_count;
    };

    template<size_t I, typename Tuple>
    struct TupleRadixSorter<I, I, Tuple>
    {
        template<typename RandomAccessIterator, typename OutIt, typename Projection>
        static auto sort(RandomAccessIterator, RandomAccessIterator,
                         OutIt, OutIt, Projection)
            -> bool
        {
            return false;
        }

        static constexpr std::size_t pass_count = 0;
    };

    template<size_t I, typename Tuple>
    struct TupleRadixSorter<I, I, const Tuple&>
    {
        template<typename RandomAccessIterator, typename OutIt, typename Projection>
        static auto sort(RandomAccessIterator, RandomAccessIterator,
                         OutIt, OutIt, Projection)
            -> bool
        {
            return false;
        }

        static constexpr std::size_t pass_count = 0;
    };

    template<typename... Args>
    struct RadixSorter<std::tuple<Args...>>
    {
        using SorterImpl = TupleRadixSorter<0, sizeof...(Args), std::tuple<Args...>>;

        template<typename RandomAccessIterator, typename OutIt, typename Projection>
        static auto sort(RandomAccessIterator begin, RandomAccessIterator end,
                         OutIt buffer_begin, Projection projection)
            -> bool
        {
            return SorterImpl::sort(std::move(begin), std::move(end),
                                    buffer_begin, buffer_begin + (end - begin),
                                    std::move(projection));
        }

        static constexpr std::size_t pass_count = SorterImpl::pass_count;
    };

    template<typename... Args>
    struct RadixSorter<const std::tuple<Args...>&>
    {
        using SorterImpl = TupleRadixSorter<0, sizeof...(Args), const std::tuple<Args...>&>;

        template<typename RandomAccessIterator, typename OutIt, typename Projection>
        static auto sort(RandomAccessIterator begin, RandomAccessIterator end,
                         OutIt buffer_begin, Projection projection)
            -> bool
        {
            return SorterImpl::sort(std::move(begin), std::move(end),
                                    buffer_begin, buffer_begin + (end - begin),
                                    std::move(projection));
        }

        static constexpr std::size_t pass_count = SorterImpl::pass_count;
    };

    template<typename T, std::size_t S>
    struct RadixSorter<std::array<T, S>>
    {
        template<typename RandomAccessIterator, typename OutIt, typename Projection>
        static auto sort(RandomAccessIterator begin, RandomAccessIterator end,
                         OutIt buffer_begin, Projection projection)
            -> bool
        {
            auto&& proj = utility::as_function(projection);

            auto buffer_end = buffer_begin + (end - begin);
            bool which = false;
            for (std::size_t i = S ; i > 0 ; --i) {
                auto extract_i = [&, i=i-1](auto&& o) {
                    return proj(o)[i];
                };
                if (which) {
                    which = not RadixSorter<T>::sort(buffer_begin, buffer_end, begin, extract_i);
                } else {
                    which = RadixSorter<T>::sort(begin, end, buffer_begin, extract_i);
                }
            }
            return which;
        }

        static constexpr size_t pass_count = RadixSorter<T>::pass_count * S;
    };

    template<typename T>
    struct RadixSorter<const T>:
        RadixSorter<T>
    {};

    template<typename T>
    struct RadixSorter<T&>:
        RadixSorter<const T&>
    {};

    template<typename T>
    struct RadixSorter<T&&>:
        RadixSorter<T>
    {};

    template<typename T>
    struct RadixSorter<const T&>:
        RadixSorter<T>
    {};

    template<typename T>
    struct RadixSorter<const T&&>:
        RadixSorter<T>
    {};

    // these structs serve two purposes
    // 1. they serve as illustration for how to implement the to_radix_sort_key function
    // 2. they help produce better error messages. with these overloads you get the
    //    error message "no matching function for call to to_radix_sort(your_type)"
    //    without these examples, you'd get the error message "to_radix_sort_key was
    //    not declared in this scope" which is a much less useful error message
    struct ExampleStructA { int i; };
    struct ExampleStructB { float f; };
    inline int to_radix_sort_key(ExampleStructA a) { return a.i; }
    inline float to_radix_sort_key(ExampleStructB b) { return b.f; }

    template<typename T, typename Enable=void>
    struct FallbackRadixSorter:
        RadixSorter<decltype(to_radix_sort_key(std::declval<T>()))>
    {
        using base = RadixSorter<decltype(to_radix_sort_key(std::declval<T>()))>;

        template<typename RandomAccessIterator, typename OutIt, typename Projection>
        static auto sort(RandomAccessIterator begin, RandomAccessIterator end,
                         OutIt buffer_begin, Projection projection)
            -> bool
        {
            auto&& proj = utility::as_function(projection);
            return base::sort(begin, end, buffer_begin, [&](auto&& a) -> decltype(auto) {
                return to_radix_sort_key(proj(a));
            });
        }
    };

    template<typename T>
    struct FallbackRadixSorter<T, std::enable_if_t<not std::is_same<void, decltype(to_unsigned_or_bool(std::declval<T>()))>::value>>:
        RadixSorter<decltype(to_unsigned_or_bool(std::declval<T>()))>
    {};

    template<typename T>
    struct RadixSorter:
        FallbackRadixSorter<T>
    {};

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
            case 2:
                to_call(begin);
                ++begin;
            case 1:
                to_call(begin);
        }
    }

    template<typename RandomAccessIterator, typename Function>
    auto custom_std_partition(RandomAccessIterator begin, RandomAccessIterator end,
                              Function function)
        -> RandomAccessIterator
    {
        auto&& func = utility::as_function(function);

        for (;; ++begin) {
            if (begin == end) {
                return end;
            }
            if (not func(*begin)) {
                break;
            }
        }
        RandomAccessIterator it = begin;
        for (++it ; it != end ; ++it) {
            if (not func(*it)) {
                continue;
            }

            using utility::iter_swap;
            iter_swap(begin, it);
            ++begin;
        }
        return begin;
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
    struct FallbackSubKey<T, std::enable_if_t<not std::is_same<void, decltype(to_unsigned_or_bool(std::declval<T>()))>::value>>:
        SubKey<decltype(to_unsigned_or_bool(std::declval<T>()))>
    {};

    template<typename T>
    struct SubKey:
        FallbackSubKey<T>
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

    template<>
    struct SubKey<unsigned char>:
        SizedSubKey<sizeof(unsigned char)>
    {};

    template<>
    struct SubKey<unsigned short>:
        SizedSubKey<sizeof(unsigned short)>
    {};

    template<>
    struct SubKey<unsigned int>:
        SizedSubKey<sizeof(unsigned int)>
    {};

    template<>
    struct SubKey<unsigned long>:
        SizedSubKey<sizeof(unsigned long)>
    {};

    template<>
    struct SubKey<unsigned long long>:
        SizedSubKey<sizeof(unsigned long long)>
    {};

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

        using next = std::conditional_t<
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

        using next = std::conditional_t<
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
        SubKey<std::decay_t<decltype(std::declval<T>()[0])>>
    {
        using base = SubKey<std::decay_t<decltype(std::declval<T>()[0])>>;
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
    struct FallbackSubKey<T, std::enable_if_t<not std::is_same<void, decltype(std::declval<T>()[0])>::value>>:
        ListSubKey<T>
    {};

    template<typename RandomAccessIterator, typename Projection>
    auto StdSortFallback(RandomAccessIterator begin, RandomAccessIterator end, Projection projection)
        -> void
    {
        auto&& proj = utility::as_function(projection);
        std::sort(std::move(begin), std::move(end), [&](auto&& l, auto&& r) {
            return proj(l) < proj(r);
        });
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
                    if (not StdSortIfLessThanThreshold<StdSortThreshold>(partition_begin, partition_end,
                                                                         num_elements, projection)) {
                        UnsignedInplaceSorter<StdSortThreshold, AmericanFlagSortThreshold,
                                              CurrentSubKey, NumBytes, Offset + 1>::sort(
                            partition_begin, partition_end, num_elements, projection, next_sort, sort_data);
                    }
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
                last_remaining = custom_std_partition(remaining_partitions, last_remaining,
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
                    if (not StdSortIfLessThanThreshold<StdSortThreshold>(partition_begin, partition_end, num_elements, projection)) {
                        UnsignedInplaceSorter<StdSortThreshold, AmericanFlagSortThreshold, CurrentSubKey, NumBytes, Offset + 1>::sort(partition_begin, partition_end, num_elements,
                                                                                                                                      projection, next_sort, sort_data);
                    }
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
            auto end_of_shorter_ones = std::partition(begin, end, [&](auto&& elem) {
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

            auto middle = std::partition(begin, end, [&](auto&& a) {
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
                                 std::enable_if_t<not std::is_same<void, decltype(std::declval<SubKeyType>()[0])>::value>>:
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
        using SubKey = SubKey<decltype(utility::as_function(projection)(*begin))>;
        SortStarter<StdSortThreshold, AmericanFlagSortThreshold, SubKey>::sort(begin, end, end - begin,
                                                                               std::move(projection));
    }

    template<typename RandomAccessIterator, typename Projection>
    auto ska_sort(RandomAccessIterator begin, RandomAccessIterator end,
                  Projection projection)
        -> void
    {
        detail::inplace_radix_sort<128, 128>(std::move(begin), std::move(end),
                                             std::move(projection));
    }
}}

#endif // CPPSORT_DETAIL_SKA_SORT_H_
