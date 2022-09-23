/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */

/*
 * Grail sorting
 *
 * (c) 2013 by Andrey Astrelin
 *
 * Stable sorting that works in O(N*log(N)) time
 * and uses O(1) extra memory
 *
 */
#ifndef CPPSORT_DETAIL_GRAIL_SORT_H_
#define CPPSORT_DETAIL_GRAIL_SORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <iterator>
#include <tuple>
#include <type_traits>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/iter_move.h>
#include "insertion_sort.h"
#include "iterator_traits.h"
#include "lower_bound.h"
#include "move.h"
#include "rotate.h"
#include "swap_ranges.h"
#include "three_way_compare.h"
#include "type_traits.h"
#include "upper_bound.h"

namespace cppsort
{
namespace detail
{
namespace grail
{
    // cost: 2 * len + nk^2 / 2
    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto find_keys(RandomAccessIterator first, RandomAccessIterator last,
                   difference_type_t<RandomAccessIterator> key_count,
                   Compare compare, Projection projection)
        -> difference_type_t<RandomAccessIterator>
    {
        auto&& proj = utility::as_function(projection);

        difference_type_t<RandomAccessIterator> dist = 1;
        auto h0 = first;
        for (auto u = std::next(first) ; u != last ; ++u) {
            if (dist == key_count) break;
            auto r = lower_bound_n(h0, dist, proj(*u), compare.base(), projection) - h0;
            if (r == dist || compare(proj(*u), proj(h0[r])) != 0) {
                h0 = detail::rotate(h0, h0 + dist, u);
                detail::rotate(h0 + r, u, std::next(u));
                ++dist;
            }
        }
        detail::rotate(first, h0, h0 + dist);
        return dist;
    }

    // cost: min(L1, L2)^2 + max(L1, L2)
    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto merge_without_buffer(RandomAccessIterator first, RandomAccessIterator middle,
                              RandomAccessIterator last,
                              Compare compare, Projection projection)
        -> void
    {
        auto&& proj = utility::as_function(projection);

        if (middle - first < last - middle) {
            while (first != middle) {
                // Binary search left
                auto it = lower_bound(middle, last, proj(*first), compare.base(), projection);
                if (it != middle) {
                    detail::rotate(first, middle, it);
                    auto delta = it - middle;
                    first += delta;
                    middle += delta;
                }
                if (middle == last) break;
                do {
                    ++first;
                } while (first != middle && compare(proj(*first), proj(*middle)) <= 0);
            }
        } else {
            while (middle != last) {
                // Binary search right
                auto it = upper_bound(first, middle, proj(*std::prev(last)), compare.base(), projection);
                if (it != middle) {
                    detail::rotate(it, middle, last);
                    auto delta = middle - it;
                    middle -= delta;
                    last -= delta;
                }
                if (first == middle) break;
                do {
                    --last;
                } while (middle != last && compare(proj(*std::prev(middle)), proj(*std::prev(last))) <= 0);
            }
        }
    }

    // arr[M..-1] - buffer, arr[0,L1-1]++arr[L1,L1+L2-1] -> arr[M,M+L1+L2-1]
    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto merge_left(RandomAccessIterator first, RandomAccessIterator middle,
                    RandomAccessIterator last, RandomAccessIterator M,
                    Compare compare, Projection projection)
        -> void
    {
        using utility::iter_swap;
        auto&& proj = utility::as_function(projection);

        auto left_it = first;
        auto right_it = middle;

        while (right_it != last) {
            if (left_it == middle || compare(proj(*left_it), proj(*right_it)) > 0) {
                iter_swap(M, right_it);
                ++right_it;
            } else {
                iter_swap(M, left_it);
                ++left_it;
            }
            ++M;
        }
        if (M != left_it) {
            detail::swap_ranges_overlap(M, M + (middle - left_it), left_it);
        }
    }

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto merge_right(RandomAccessIterator first, RandomAccessIterator middle,
                     RandomAccessIterator last, RandomAccessIterator M,
                     Compare compare, Projection projection)
        -> void
    {
        using utility::iter_swap;
        auto&& proj = utility::as_function(projection);

        auto p0 = std::prev(M),
             p1 = std::prev(middle),
             p2 = std::prev(last);

        while (p1 > first) {
            if (p2 < middle || compare(proj(*p1), proj(*p2)) > 0) {
                iter_swap(p0, p1);
                --p1;
            } else {
                iter_swap(p0, p2);
                --p2;
            }
            --p0;
        }
        // Same as the previous loop, just avoids decrementing p1,
        // which can make it end up before the origin, causing
        // issues with some kinds of iterators
        if (p1 == first) {
            while (not (p2 < middle || compare(proj(*p1), proj(*p2)) > 0)) {
                iter_swap(p0, p2);
                --p2;
                --p0;
            }
            iter_swap(p0, p1);
            --p0;
        }

        if (p2 != p0) {
            while (p2 > middle) {
                iter_swap(p0, p2);
                --p0;
                --p2;
            }
            if (p2 == middle){
                iter_swap(p0, p2);
            }
        }
    }

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto smart_merge_with_buffer(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last,
                                 difference_type_t<RandomAccessIterator> block_len, int left_over_frag,
                                 Compare compare, Projection projection)
        -> std::pair<difference_type_t<RandomAccessIterator>, int>
    {
        using utility::iter_swap;
        auto&& proj = utility::as_function(projection);

        auto out_it = first - block_len,
             left_it = first,
             right_it = middle;
        int frag_type  = 1 - left_over_frag;  // 1 if inverted

        while (left_it != middle && right_it != last) {
            if (compare(proj(*left_it), proj(*right_it)) - frag_type  < 0) {
                iter_swap(out_it, left_it);
                ++left_it;
            } else {
                iter_swap(out_it, right_it);
                ++right_it;
            }
            ++out_it;
        }

        //auto p1 = left_it, q1 = middle, p2 = right_it, q2 = last;
        difference_type_t<RandomAccessIterator> len;
        if (left_it < middle) {
            len = middle - left_it;
            do {
                iter_swap(--middle, --last);
            } while (left_it != middle);
        } else {
            len = last - right_it;
            left_over_frag = frag_type;
        }
        return { len, left_over_frag };
    }

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto smart_merge_without_buffer(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last,
                                    int left_over_frag, Compare compare, Projection projection)
        -> std::pair<difference_type_t<RandomAccessIterator>, int>
    {
        auto&& proj = utility::as_function(projection);

        if (middle == last) {
            return { (middle - first), left_over_frag };
        }

        int frag_type = 1 - left_over_frag;
        if (first != middle && compare(proj(*std::prev(middle)), proj(*middle)) - frag_type >= 0) {
            while (first != middle) {
                auto len = frag_type ? (lower_bound(middle, last, proj(*first), compare.base(), projection) - middle)
                                     : (upper_bound(middle, last, proj(*first), compare.base(), projection) - middle);
                if (len != 0) {
                    detail::rotate(first, middle, middle + len);
                    first += len;
                    middle += len;
                }
                if (middle == last) {
                    return { (middle - first), left_over_frag };
                }
                do {
                    ++first;
                } while (first != middle && compare(proj(*first), proj(*middle)) - frag_type < 0);
            }
        }
        return { (last - middle), frag_type };
    }

    // Sort With Extra Buffer

    template<typename ForwardIterator, typename Compare, typename Projection>
    auto merge_left_with_extra_buffer(ForwardIterator first, ForwardIterator middle,
                                      ForwardIterator last, ForwardIterator out,
                                      Compare compare, Projection projection)
        -> void
    {
        using utility::iter_move;
        auto&& proj = utility::as_function(projection);

        auto it = middle;
        while (it != last) {
            if (first == middle || compare(proj(*first), proj(*it)) > 0) {
                *out = iter_move(it);
                ++out;
                ++it;
            } else {
                *out = iter_move(first);
                ++out;
                ++first;
            }
        }
        if (out != first) {
            detail::move(first, middle, out);
        }
    }

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto smart_merge_with_extra_buffer(RandomAccessIterator first, RandomAccessIterator middle,
                                       RandomAccessIterator last,
                                       int left_over_frag, difference_type_t<RandomAccessIterator> block_len,
                                       Compare compare, Projection projection)
        -> std::pair<difference_type_t<RandomAccessIterator>, int>
    {
        using utility::iter_move;
        auto&& proj = utility::as_function(projection);

        auto out = first - block_len;
        auto it = middle;

        int frag_type = 1 - left_over_frag;  // 1 if inverted
        while (first < middle && it < last) {
            if (compare(proj(*first), proj(*it)) - frag_type < 0) {
                *out = iter_move(first);
                ++first;
            } else {
                *out = iter_move(it);
                ++it;
            }
            ++out;
        }

        if (first < middle) {
            auto left_over_len = middle - first;
            do {
                *--last = iter_move(--middle);
            } while (first != middle);
            return { left_over_len, left_over_frag };
        }
        return { (last - it), frag_type };
    }

    // arr - starting array. arr[-lblock..-1] - buffer (if havebuf).
    // lblock - length of regular blocks. First nblocks are stable sorted by 1st elements and key-coded
    // keys - arrays of keys, in same order as blocks. key<midkey means stream A
    // nblock2 are regular blocks from stream A. llast is length of last (irregular) block from stream B, that should go before nblock2 blocks.
    // llast=0 requires nblock2=0 (no irregular blocks). llast>0, nblock2=0 is possible.
    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto merge_buffers_left_with_extra_buffer(RandomAccessIterator keys, RandomAccessIterator midkey, RandomAccessIterator arr,
                                              difference_type_t<RandomAccessIterator> nblock,
                                              difference_type_t<RandomAccessIterator> lblock,
                                              difference_type_t<RandomAccessIterator> nblock2,
                                              difference_type_t<RandomAccessIterator> llast,
                                              Compare compare, Projection projection)
        -> void
    {
        using difference_type = difference_type_t<RandomAccessIterator>;
        auto&& proj = utility::as_function(projection);

        if (nblock == 0) {
            auto l = arr + nblock2 * lblock;
            merge_left_with_extra_buffer(arr, l, l+llast, arr-lblock, compare, projection);
            return;
        }

        auto lrest = lblock;
        int frest = compare(proj(*keys), proj(*midkey)) < 0 ? 0 : 1;
        auto pidx = arr + lblock;
        for (difference_type cidx = 1 ; cidx < nblock ; (void) ++cidx, pidx += lblock) {
            auto prest = pidx - lrest;
            int fnext = compare(proj(keys[cidx]), proj(*midkey)) < 0 ? 0 : 1;
            if (fnext == frest) {
                detail::move(prest, prest + lrest, prest - lblock);
                prest = pidx;
                lrest = lblock;
            } else {
                std::tie(lrest, frest) = smart_merge_with_extra_buffer(
                    prest, prest + lrest, prest + (lrest + lblock),
                    frest, lblock, compare, projection);
            }
        }
        auto prest = pidx - lrest;
        if (llast) {
            if (frest) {
                detail::move(prest, prest + lrest, prest - lblock);
                prest = pidx;
                lrest = lblock * nblock2;
            } else {
                lrest += lblock * nblock2;
            }
            merge_left_with_extra_buffer(prest, prest + lrest, prest + (lrest + llast), prest - lblock,
                                         std::move(compare), std::move(projection));
        } else {
            detail::move(prest, prest + lrest, prest - lblock);
        }
    }

    /***** End Sort With Extra Buffer *****/

    // arr - starting array. arr[-lblock..-1] - buffer (if havebuf).
    // lblock - length of regular blocks. First nblocks are stable sorted by 1st elements and key-coded
    // keys - arrays of keys, in same order as blocks. key<midkey means stream A
    // nblock2 are regular blocks from stream A. llast is length of last (irregular) block from stream B,
    // that should go before nblock2 blocks.
    // llast=0 requires nblock2=0 (no irregular blocks). llast>0, nblock2=0 is possible.
    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto merge_buffers_left(RandomAccessIterator keys, RandomAccessIterator midkey, RandomAccessIterator arr,
                            difference_type_t<RandomAccessIterator> nblock,
                            difference_type_t<RandomAccessIterator> lblock, bool havebuf,
                            difference_type_t<RandomAccessIterator> nblock2,
                            difference_type_t<RandomAccessIterator> llast,
                            Compare compare, Projection projection)
        -> void
    {
        using difference_type = difference_type_t<RandomAccessIterator>;
        auto&& proj = utility::as_function(projection);
        auto&& midkey_proj = proj(*midkey);

        if (nblock == 0) {
            auto l = arr + nblock2 * lblock;
            if (havebuf) {
                merge_left(arr, l, l+llast, arr-lblock, std::move(compare), std::move(projection));
            } else {
                merge_without_buffer(arr, l, l + llast, std::move(compare), std::move(projection));
            }
            return;
        }

        auto lrest = lblock;
        int frest = compare(proj(*keys), midkey_proj) < 0 ? 0 : 1;
        auto pidx = arr + lblock;
        for (difference_type cidx = 1 ; cidx < nblock ; (void) ++cidx, pidx += lblock) {
            auto prest = pidx - lrest;
            int fnext = compare(proj(keys[cidx]), midkey_proj) < 0 ? 0 : 1;
            if(fnext == frest) {
                if (havebuf) {
                    detail::swap_ranges_overlap(prest-lblock, prest-(lblock-lrest), prest);
                }
                prest = pidx;
                lrest = lblock;
            } else {
                if (havebuf) {
                    std::tie(lrest, frest) = smart_merge_with_buffer(
                        prest, prest + lrest, prest + (lrest + lblock),
                        lblock, frest, compare, projection);
                } else {
                    std::tie(lrest, frest) = smart_merge_without_buffer(
                        prest, prest + lrest, prest + (lrest + lblock),
                        frest, compare, projection);
                }

            }
        }

        auto prest = pidx - lrest;
        if (llast) {
            if (frest) {
                if (havebuf) {
                    detail::swap_ranges_overlap(prest-lblock, prest-(lblock-lrest), prest);
                }
                prest = pidx;
                lrest = lblock * nblock2;
            } else {
                lrest += lblock * nblock2;
            }
            if (havebuf) {
                merge_left(prest, prest+lrest, prest+(lrest+llast), prest-lblock,
                           std::move(compare), std::move(projection));
            } else {
                merge_without_buffer(prest, prest + lrest, prest + (lrest + llast),
                                     std::move(compare), std::move(projection));
            }
        } else {
            if (havebuf) {
                detail::swap_ranges_overlap(prest, prest+lrest, prest-lblock);
            }
        }
    }

    // build blocks of length K
    // input: [-K,-1] elements are buffer
    // output: first K elements are buffer, blocks 2*K and last subblock sorted
    template<typename RandomAccessIterator, typename BufferIterator,
             typename Compare, typename Projection>
    auto build_blocks(RandomAccessIterator first, RandomAccessIterator last,
                      difference_type_t<RandomAccessIterator> K,
                      BufferIterator extbuf, difference_type_t<RandomAccessIterator> LExtBuf,
                      Compare compare, Projection projection)
        -> void
    {
        using difference_type = difference_type_t<RandomAccessIterator>;
        using utility::iter_move;
        using utility::iter_swap;
        auto&& proj = utility::as_function(projection);
        auto size = last - first;

        auto kbuf = std::min(K, LExtBuf);
        while (kbuf & (kbuf - 1)) {
            kbuf &= kbuf - 1;  // max power or 2 - just in case
        }

        difference_type h;
        if (kbuf) {
            detail::move(first - kbuf, first, extbuf);
            for (difference_type m = 1 ; m < size ; m += 2) {
                difference_type u = 0;
                if (compare(proj(first[m-1]), proj(first[m])) > 0) {
                    u = 1;
                }
                first[m-3] = iter_move(first + (m - 1 + u));
                first[m-2] = iter_move(first + (m - u));
            }
            if (size % 2) {
                first[size-3] = iter_move(first + (size - 1));
            }
            first -= 2;
            last -= 2;
            for (h = 2 ; h < kbuf ; h *= 2) {
                auto p0 = first;
                auto p1 = last - 2 * h;
                while (p0 <= p1) {
                    merge_left_with_extra_buffer(p0, p0+h, p0+(h+h), p0-h, compare, projection);
                    p0 += 2 * h;
                }
                auto rest = last - p0;
                if (rest > h) {
                    merge_left_with_extra_buffer(p0, p0+h, last, p0-h, compare, projection);
                } else {
                    detail::move(p0, last, p0 - h);
                }
                first -= h;
                last -= h;
            }
            detail::move(extbuf, extbuf + kbuf, last);
        } else {
            for (difference_type m = 1 ; m < size ; m += 2) {
                difference_type u = 0;
                if (compare(proj(first[m-1]), proj(first[m])) > 0) {
                    u = 1;
                }
                iter_swap(first + (m-3), first + (m-1+u));
                iter_swap(first + (m-2), first + (m-u));
            }
            if (size % 2) {
                iter_swap(last - 1, last - 3);
            }
            first -= 2;
            last -= 2;
            h = 2;
        }
        for (; h < K ; h *= 2) {
            auto p0 = first;
            if (2 * h <= (last - p0)) {
                auto p1 = last - 2 * h;
                while (p0 <= p1) {
                    merge_left(p0, p0+h, p0+(h+h), p0-h, compare, projection);
                    p0 += 2 * h;
                }
            }
            auto rest = last - p0;
            if (rest > h) {
                merge_left(p0, p0+h, last, p0-h, compare, projection);
            } else {
                detail::rotate(p0-h, p0, last);
            }
            first -= h;
            last -= h;
        }
        auto restk = size % (2 * K);
        auto p = last - restk;
        if (restk <= K) {
            detail::rotate(p, last, last+K);
        } else {
            merge_right(p, p+K, last, last+K, compare, projection);
        }
        while (p > first) {
            p -= 2 * K;
            merge_right(p, p+K, p+(K+K), p+(K+K+K), compare, projection);
        }
    }

    // keys are on the left of arr. Blocks of length LL combined. We'll combine them in pairs
    // LL and nkeys are powers of 2. (2*LL/lblock) keys are guarantied
    template<typename RandomAccessIterator, typename BufferIterator,
             typename Compare, typename Projection>
    auto combine_blocks(RandomAccessIterator keys, RandomAccessIterator arr,
                        difference_type_t<RandomAccessIterator> len,
                        difference_type_t<RandomAccessIterator> LL,
                        difference_type_t<RandomAccessIterator> lblock,
                        bool havebuf, BufferIterator xbuf, bool usexbuf,
                        Compare compare, Projection projection)
        -> void
    {
        using difference_type = difference_type_t<RandomAccessIterator>;
        using utility::iter_move;
        using utility::iter_swap;
        auto&& proj = utility::as_function(projection);

        auto M = len / (2 * LL);
        auto lrest = len % (2 * LL);
        if (lrest <= LL) {
            len -= lrest;
            lrest = 0;
        }
        if (usexbuf) {
            detail::move(arr - lblock, arr, xbuf);
        }
        for (difference_type b = 0 ; b <= M ; ++b) {
            if (b == M && lrest == 0) break;
            auto arr1 = arr + b * 2 * LL;
            auto NBlk = (b == M ? lrest : 2 * LL) / lblock;
            insertion_sort(keys, keys + (NBlk + (b == M ? 1 : 0)),
                           compare.base(), projection);
            auto midkey = LL / lblock;
            for (difference_type u = 1 ; u < NBlk ; ++u) {
                auto p = u - 1;
                for (auto v = u ; v < NBlk ; ++v) {
                    int kc = compare(proj(arr1[p*lblock]), proj(arr1[v*lblock]));
                    if (kc > 0 || (kc == 0 && compare(proj(keys[p]), proj(keys[v])) > 0)) {
                        p = v;
                    }
                }
                if (p != u - 1) {
                    detail::swap_ranges_overlap(arr1+(u-1)*lblock, arr1+u*lblock, arr1+p*lblock);
                    iter_swap(keys+(u-1), keys+p);
                    if (midkey == u - 1 || midkey == p) {
                        midkey ^= (u - 1) ^ p;
                    }
                }
            }
            difference_type nbl2 = 0;
            difference_type llast = 0;
            if (b == M) {
                llast = lrest % lblock;
            }
            if (llast != 0) {
                while (nbl2 < NBlk &&
                       compare(proj(arr1[NBlk*lblock]), proj(arr1[(NBlk-nbl2-1)*lblock])) < 0) {
                    ++nbl2;
                }
            }
            if (usexbuf) {
                merge_buffers_left_with_extra_buffer(keys, keys + midkey, arr1, NBlk - nbl2, lblock, nbl2, llast,
                                                     compare, projection);
            } else {
                merge_buffers_left(keys, keys + midkey, arr1, NBlk - nbl2, lblock, havebuf, nbl2, llast,
                                   compare, projection);
            }
        }
        if (usexbuf) {
            for (auto p = len ; --p >= 0;) {
                arr[p] = iter_move(arr + (p - lblock));
            }
            detail::move(xbuf, xbuf + lblock, arr - lblock);
        } else if (havebuf) {
            while (--len >= 0) {
                iter_swap(arr + len, arr + (len - lblock));
            }
        }
    }

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto lazy_stable_sort(RandomAccessIterator first, RandomAccessIterator last,
                          Compare compare, Projection projection)
        -> void
    {
        using difference_type = difference_type_t<RandomAccessIterator>;
        using utility::iter_swap;
        auto&& proj = utility::as_function(projection);

        auto size = last - first;
        auto end_loop = size % 2 == 0 ? last : std::prev(last);
        for (auto it = first ; it != end_loop ; it += 2) {
            if (compare(proj(*it), proj(*std::next(it))) > 0) {
                iter_swap(it, std::next(it));
            }
        }

        for (difference_type h = 2 ; h < size ; h *= 2) {
            auto p0 = first;
            if (2 * h <= size) {
                auto p1 = last - 2 * h;
                while (p0 <= p1) {
                    merge_without_buffer(p0, p0 + h, p0 + (h + h), compare, projection);
                    p0 += 2 * h;
                }
            }
            int rest = last - p0;
            if (rest > h) {
                merge_without_buffer(p0, p0 + h, last, compare, projection);
            }
        }
    }

    template<typename RandomAccessIterator, typename BufferIterator,
             typename Compare, typename Projection>
    auto common_sort(RandomAccessIterator first, RandomAccessIterator last,
                     BufferIterator extbuf, int LExtBuf,
                     Compare compare, Projection projection)
        -> void
    {
        using difference_type = difference_type_t<RandomAccessIterator>;

        auto size = last - first;
        if (size < 16) {
            insertion_sort(first, last, compare.base(), std::move(projection));
            return;
        }

        difference_type lblock = 4;
        while (lblock * lblock < size) {
            lblock *= 2;
        }
        auto nkeys = (size - 1) / lblock + 1;
        auto findkeys = find_keys(first, last, nkeys + lblock, compare, projection);
        bool havebuf = true;
        if (findkeys < nkeys + lblock) {
            if (findkeys < 4) {
                lazy_stable_sort(first, last, std::move(compare), std::move(projection));
                return;
            }
            nkeys = lblock;
            while (nkeys > findkeys) {
                nkeys /= 2;
            }
            havebuf = false;
            lblock = 0;
        }
        auto ptr = first + (lblock + nkeys);
        auto cbuf = havebuf ? lblock : nkeys;
        if (havebuf) {
            build_blocks(ptr, last, cbuf, extbuf, LExtBuf, compare, projection);
        } else {
            using T = value_type_t<BufferIterator>;
            build_blocks(ptr, last, cbuf, static_cast<T*>(nullptr), 0, compare, projection);
        }

        // 2*cbuf are built
        while ((last - ptr) > (cbuf *= 2)) {
            auto lb = lblock;
            bool chavebuf = havebuf;
            if (not havebuf) {
                if (nkeys > 4 && nkeys / 8 * nkeys >= cbuf) {
                    lb = nkeys / 2;
                    chavebuf = true;
                } else {
                    difference_type nk = 1;
                    long long s = (long long) cbuf * findkeys / 2;
                    while (nk < nkeys && s != 0) {
                        nk *= 2;
                        s /= 8;
                    }
                    lb = (2 * cbuf) / nk;
                }
            }

            combine_blocks(first, ptr, last - ptr, cbuf, lb,
                           chavebuf, extbuf, chavebuf && lb <= LExtBuf,
                           compare, projection);
        }
        insertion_sort(first, ptr, compare.base(), projection);
        merge_without_buffer(first, ptr, last, std::move(compare), std::move(projection));
    }

    template<typename BufferProvider, typename RandomAccessIterator,
             typename Compare, typename Projection>
    auto grail_sort(RandomAccessIterator first, RandomAccessIterator last,
                    Compare compare, Projection projection)
        -> void
    {
        // Allocate temporary buffer
        auto size = last - first;
        typename BufferProvider::template buffer<rvalue_type_t<RandomAccessIterator>> buffer(size);

        using compare_t = std::remove_reference_t<decltype(utility::as_function(compare))>;
        common_sort(std::move(first), std::move(last), buffer.begin(), buffer.size(),
                    three_way_compare<compare_t>(utility::as_function(compare)),
                    std::move(projection));
    }
}}}

#endif // CPPSORT_DETAIL_GRAIL_SORT_H_
