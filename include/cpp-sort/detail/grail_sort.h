/*
 * Grail sorting
 *
 * (c) 2013 by Andrey Astrelin
 * Modified in 2015-2019 by Morwenn for inclusion into cpp-sort
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
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/iter_move.h>
#include "insertion_sort.h"
#include "iterator_traits.h"
#include "lower_bound.h"
#include "merge_move.h"
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
            auto r = lower_bound(h0, h0 + dist, proj(*u), compare.base(), projection) - h0;
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

        if (std::distance(first, middle) < std::distance(middle, last)) {
            while (first != middle) {
                // Binary search left
                auto it = lower_bound(middle, last, proj(*first), compare.base(), projection);
                if (it != middle) {
                    detail::rotate(first, middle, it);
                    auto delta = std::distance(middle, it);
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
                    auto delta = std::distance(it, middle);
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
            detail::swap_ranges(M, M + std::distance(left_it, middle), left_it);
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

        while (p1 >= first) {
            if (p2 < middle || compare(proj(*p1), proj(*p2)) > 0) {
                iter_swap(p0, p1);
                --p1;
            } else {
                iter_swap(p0, p2);
                --p2;
            }
            --p0;
        }

        if (p2 != p0 && p2 >= middle) {
            do {
                iter_swap(p0, p2);
                --p0;
                --p2;
            } while (p2 != middle);
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

        auto p0 = first - block_len,
             p1 = first,
             p2 = middle,
             q1 = p2,
             q2 = last;
        int frag_type  = 1 - left_over_frag;  // 1 if inverted

        while (p1 != q1 && p2 != q2) {
            if (compare(proj(*p1), proj(*p2)) - frag_type  < 0) {
                iter_swap(p0, p1);
                ++p1;
            } else {
                iter_swap(p0, p2);
                ++p2;
            }
            ++p0;
        }

        difference_type_t<RandomAccessIterator> len;
        if (p1 < q1) {
            len = std::distance(p1, q1);
            do {
                iter_swap(--q1, --q2);
            } while (p1 < q1);
        } else {
            len = std::distance(p2, q2);
            left_over_frag = frag_type;
        }
        return { len, left_over_frag };
    }

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto smart_merge_without_buffer(RandomAccessIterator first, RandomAccessIterator middle,
                                    RandomAccessIterator last, int left_over_frag,
                                    Compare compare, Projection projection)
        -> std::pair<int, int>
    {
        auto&& proj = utility::as_function(projection);

        if (middle == last) {
            return { std::distance(first, middle), left_over_frag };
        }

        int frag_type = 1 - left_over_frag;
        if (first != middle && compare(proj(*std::prev(middle)), proj(*middle)) - frag_type >= 0) {
            while (first != middle) {
                auto h = frag_type ? (lower_bound(middle, last, proj(*first), compare.base(), projection) - middle)
                                   : (upper_bound(middle, last, proj(*first), compare.base(), projection) - middle);
                if (h != 0) {
                    detail::rotate(first, middle, middle + h);
                    first += h;
                    middle += h;
                }
                if (middle == last) {
                    return { std::distance(first, middle), left_over_frag };
                }
                do {
                    ++first;
                } while (first != middle && compare(proj(*first), proj(*middle)) - frag_type < 0);
            }
        }
        return { std::distance(middle, last), frag_type };
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

        auto p0 = first, p1 = middle;
        while (p1 != last) {
            if (p0 == middle || compare(proj(*p0), proj(*p1)) > 0) {
                *out = iter_move(p1);
                ++out; ++p1;
            } else {
                *out = iter_move(p0);
                ++out; ++p0;
            }
        }
        if (out != p0) {
            detail::move(p0, middle, out);
        }
    }

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto smart_merge_with_extra_buffer(RandomAccessIterator first, RandomAccessIterator middle,
                                       RandomAccessIterator last, int atype, int lkeys,
                                       Compare compare, Projection projection)
        -> std::pair<int, int>
    {
        using utility::iter_move;
        auto&& proj = utility::as_function(projection);

        RandomAccessIterator p0 = first - lkeys,
                             p1 = first,
                             p2 = middle,
                             q1 = p2,
                             q2 = last;

        int ftype = 1 - atype;  // 1 if inverted
        while (p1 < q1 && p2 < q2) {
            if (compare(proj(*p1), proj(*p2)) - ftype < 0) {
                *p0 = iter_move(p1);
                ++p1;
            } else {
                *p0 = iter_move(p2);
                ++p2;
            }
            ++p0;
        }

        int len;
        if (p1 < q1) {
            len = std::distance(p1, q1);
            while (p1 < q1) {
                *--q2 = iter_move(--q1);
            }
        } else {
            len = std::distance(p2, q2);
            atype = ftype;
        }
        return { len, atype };
    }

    // arr - starting array. arr[-lblock..-1] - buffer (if havebuf).
    // lblock - length of regular blocks. First nblocks are stable sorted by 1st elements and key-coded
    // keys - arrays of keys, in same order as blocks. key<midkey means stream A
    // nblock2 are regular blocks from stream A. llast is length of last (irregular) block from stream B, that should go before nblock2 blocks.
    // llast=0 requires nblock2=0 (no irregular blocks). llast>0, nblock2=0 is possible.
    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto merge_buffers_left_with_extra_buffer(RandomAccessIterator keys, RandomAccessIterator midkey,
                                              RandomAccessIterator arr, int nblock, int lblock, int nblock2,
                                              int llast, Compare compare, Projection projection)
        -> void
    {
        auto&& proj = utility::as_function(projection);

        if (nblock == 0) {
            auto l = arr + nblock2 * lblock;
            merge_left_with_extra_buffer(arr, l, l+llast, arr-lblock, compare, projection);
            return;
        }

        int lrest = lblock;
        int frest = compare(proj(*keys), proj(*midkey)) < 0 ? 0 : 1;
        auto pidx = arr + lblock;
        for (int cidx = 1 ; cidx < nblock ; (void) ++cidx, pidx += lblock) {
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
    auto merge_buffers_left(RandomAccessIterator keys, RandomAccessIterator midkey,
                            RandomAccessIterator arr, int nblock, int lblock, bool havebuf,
                            int nblock2, int llast, Compare compare, Projection projection)
        -> void
    {
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

        int lrest = lblock;
        int frest = compare(proj(*keys), midkey_proj) < 0 ? 0 : 1;
        auto pidx = arr + lblock;
        for (int cidx = 1 ; cidx < nblock ; (void) ++cidx, pidx += lblock) {
            auto prest = pidx - lrest;
            int fnext = compare(proj(keys[cidx]), midkey_proj) < 0 ? 0 : 1;
            if(fnext == frest) {
                if (havebuf) {
                    detail::swap_ranges(prest-lblock, prest-(lblock-lrest), prest);
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
                    detail::swap_ranges(prest-lblock, prest-(lblock-lrest), prest);
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
                detail::swap_ranges(prest, prest+lrest, prest-lblock);
            }
        }
    }

    // build blocks of length K
    // input: [-K,-1] elements are buffer
    // output: first K elements are buffer, blocks 2*K and last subblock sorted
    template<typename RandomAccessIterator, typename BufferIterator,
             typename Compare, typename Projection>
    auto build_blocks(RandomAccessIterator first, RandomAccessIterator last,
                      int K, BufferIterator extbuf, int LExtBuf,
                      Compare compare, Projection projection)
        -> void
    {
        using utility::iter_move;
        using utility::iter_swap;
        auto&& proj = utility::as_function(projection);
        auto size = std::distance(first, last);

        int kbuf = std::min(K, LExtBuf);
        while (kbuf & (kbuf - 1)) {
            kbuf &= kbuf - 1;  // max power or 2 - just in case
        }

        int h;
        if (kbuf) {
            detail::move(first - kbuf, first, extbuf);
            for (int m = 1 ; m < size ; m += 2) {
                int u = 0;
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
                int rest = std::distance(p0, last);
                if (rest > h) {
                    merge_left_with_extra_buffer(p0, p0+h, last, p0-h, compare, projection);
                } else {
                    for (; p0 < last ; ++p0) {
                        p0[-h] = iter_move(p0);
                    }
                }
                first -= h;
                last -= h;
            }
            detail::move(extbuf, extbuf + kbuf, last);
        } else {
            for (int m = 1 ; m < size ; m += 2) {
                int u = 0;
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
            auto p1 = last - 2 * h;
            while (p0 <= p1) {
                merge_left(p0, p0+h, p0+(h+h), p0-h, compare, projection);
                p0 += 2 * h;
            }
            int rest = std::distance(p0, last);
            if (rest > h) {
                merge_left(p0, p0+h, last, p0-h, compare, projection);
            } else {
                detail::rotate(p0-h, p0, last);
            }
            first -= h;
            last -= h;
        }
        int restk = size % (2 * K);
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
    auto combine_blocks(RandomAccessIterator keys, RandomAccessIterator arr, int len, int LL,
                        int lblock, bool havebuf, BufferIterator xbuf, bool usexbuf,
                        Compare compare, Projection projection)
        -> void
    {
        using utility::iter_move;
        using utility::iter_swap;
        auto&& proj = utility::as_function(projection);

        int M = len / (2 * LL);
        int lrest = len % (2 * LL);
        if (lrest <= LL) {
            len -= lrest;
            lrest = 0;
        }
        if (usexbuf) {
            detail::move(arr - lblock, arr, xbuf);
        }
        for (int b = 0 ; b <= M ; ++b) {
            if (b == M && lrest == 0) break;
            auto arr1 = arr + b * 2 * LL;
            int NBlk = (b == M ? lrest : 2 * LL) / lblock;
            insertion_sort(keys, keys + (NBlk + (b == M ? 1 : 0)),
                           compare.base(), projection);
            int midkey = LL / lblock;
            for (int u = 1 ; u < NBlk ; ++u) {
                int p = u - 1;
                for (int v = u ; v < NBlk ; ++v) {
                    int kc = compare(proj(arr1[p*lblock]), proj(arr1[v*lblock]));
                    if (kc > 0 || (kc == 0 && compare(proj(keys[p]), proj(keys[v])) > 0)) {
                        p = v;
                    }
                }
                if (p != u - 1) {
                    detail::swap_ranges(arr1+(u-1)*lblock, arr1+u*lblock, arr1+p*lblock);
                    iter_swap(keys+(u-1), keys+p);
                    if (midkey == u - 1 || midkey == p) {
                        midkey ^= (u - 1) ^ p;
                    }
                }
            }
            int nbl2 = 0;
            int llast = 0;
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
            for (int p = len ; --p >= 0;) {
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
        using utility::iter_swap;
        auto&& proj = utility::as_function(projection);

        for (auto it = std::next(first) ; it < last ; it += 2) {
            if (compare(proj(*std::prev(it)), proj(*it)) > 0) {
                iter_swap(std::prev(it), it);
            }
        }

        auto size = std::distance(first, last);
        for (int h = 2 ; h < size ; h *= 2) {
            auto p0 = first;
            auto p1 = last - 2 * h;
            while (p0 <= p1) {
                merge_without_buffer(p0, p0 + h, p0 + (h + h), compare, projection);
                p0 += 2 * h;
            }
            int rest = std::distance(p0, last);
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
        auto size = std::distance(first, last);
        if (size < 16) {
            insertion_sort(first, last, compare.base(), std::move(projection));
            return;
        }

        int lblock = 1;
        while (lblock * lblock < size) {
            lblock *= 2;
        }
        int nkeys = (size - 1) / lblock + 1;
        int findkeys = find_keys(first, last, nkeys + lblock, compare, projection);
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
        int cbuf = havebuf ? lblock : nkeys;
        if (havebuf) {
            build_blocks(ptr, last, cbuf, extbuf, LExtBuf, compare, projection);
        } else {
            using T = value_type_t<BufferIterator>;
            build_blocks(ptr, last, cbuf, static_cast<T*>(nullptr), 0, compare, projection);
        }

        // 2*cbuf are built
        while (std::distance(ptr, last) > (cbuf *= 2)) {
            int lb = lblock;
            bool chavebuf = havebuf;
            if (not havebuf) {
                if (nkeys > 4 && nkeys / 8 * nkeys >= cbuf) {
                    lb = nkeys / 2;
                    chavebuf = true;
                } else {
                    int nk = 1;
                    long long s = (long long) cbuf * findkeys / 2;
                    while (nk < nkeys && s != 0) {
                        nk *= 2;
                        s /= 8;
                    }
                    lb = (2 * cbuf) / nk;
                }
            }

            combine_blocks(first, ptr, std::distance(ptr, last), cbuf, lb,
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
        using rvalue_reference = remove_cvref_t<rvalue_reference_t<RandomAccessIterator>>;

        // Allocate temporary buffer
        auto size = std::distance(first, last);
        typename BufferProvider::template buffer<rvalue_reference> buffer(size);

        using compare_t = std::remove_reference_t<decltype(utility::as_function(compare))>;
        common_sort(std::move(first), std::move(last), buffer.begin(), buffer.size(),
                    three_way_compare<compare_t>(utility::as_function(compare)),
                    std::move(projection));
    }
}}}

#endif // CPPSORT_DETAIL_GRAIL_SORT_H_
