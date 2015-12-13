/*
 * Grail sorting
 *
 * (c) 2013 by Andrey Astrelin
 * Modified in 2015 by Morwenn for inclusion into cpp-sort
 *
 * Stable sorting that works in O(N*log(N)) worst time
 * and uses O(1) extra memory
 *
 */
#ifndef CPPSORT_DETAIL_GRAIL_SORT_H_
#define CPPSORT_DETAIL_GRAIL_SORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <tuple>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include "insertion_sort.h"
#include "lower_bound.h"
#include "merge_move.h"
#include "upper_bound.h"

namespace cppsort
{
namespace detail
{
    template<typename Compare>
    struct three_way_compare
    {
        public:

            constexpr three_way_compare(Compare compare):
                compare(compare)
            {}

            template<typename T, typename U>
            constexpr auto operator()(const T& lhs, const U& rhs) const
                -> int
            {
                if (compare(lhs, rhs))
                {
                    return -1;
                }
                if (compare(rhs, lhs))
                {
                    return 1;
                }
                return 0;
            }

            constexpr auto base() const noexcept
                -> Compare
            {
                return compare;
            }

        private:

            Compare compare;
    };

    // cost: 2*len+nk^2/2
    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto grail_FindKeys(RandomAccessIterator first, RandomAccessIterator last,
                        int nkeys, Compare compare, Projection projection)
        -> int
    {
        auto&& proj = utility::as_function(projection);

        int r, h = 1;
        RandomAccessIterator h0 = first;
        RandomAccessIterator u = std::next(first);
        while (u != last && h < nkeys) {
            r = lower_bound(h0, h0 + h, proj(*u), compare.base(), projection) - h0;
            if (r == h || compare(proj(*u), proj(h0[r])) != 0) {
                std::rotate(h0, h0 + h, u);
                h0 = u - h;
                std::rotate(h0 + r, u, std::next(u));
                ++h;
            }
            ++u;
        }
        std::rotate(first, h0, h0 + h);
        return h;
    }

    // cost: min(L1,L2)^2+max(L1,L2)
    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto grail_MergeWithoutBuffer(RandomAccessIterator first, RandomAccessIterator middle,
                                  RandomAccessIterator last,
                                  Compare compare, Projection projection)
        -> void
    {
        auto&& proj = utility::as_function(projection);

        if (std::distance(first, middle) < std::distance(middle, last)) {
            while (first != middle) {
                auto h = lower_bound(middle, last, proj(*first), compare.base(), projection);
                if (h != middle) {
                    std::rotate(first, middle, h);
                    std::size_t delta = std::distance(middle, h);
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
                auto h = upper_bound(first, middle, proj(*std::prev(last)), compare.base(), projection);
                if (h != middle) {
                    std::rotate(h, middle, last);
                    std::size_t delta = std::distance(h, middle);
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
    auto grail_MergeLeft(RandomAccessIterator first, RandomAccessIterator middle,
                         RandomAccessIterator last, RandomAccessIterator M,
                         Compare compare, Projection projection)
        -> void
    {
        auto&& proj = utility::as_function(projection);

        RandomAccessIterator p0 = first;
        RandomAccessIterator p1 = middle;

        while (p1 < last) {
            if (p0 == middle || compare(proj(*p0), proj(*p1)) > 0) {
                std::iter_swap(M++, p1++);
            } else {
                std::iter_swap(M++, p0++);
            }
        }
        if (M != p0) {
            std::swap_ranges(M, M + std::distance(p0, middle), p0);
        }
    }

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto grail_MergeRight(RandomAccessIterator first, RandomAccessIterator middle,
                          RandomAccessIterator last, RandomAccessIterator M,
                          Compare compare, Projection projection)
        -> void
    {
        auto&& proj = utility::as_function(projection);

        RandomAccessIterator p0 = std::prev(M);
        RandomAccessIterator p1 = std::prev(middle);
        RandomAccessIterator p2 = std::prev(last);

        while (p1 >= first) {
            if (p2 < middle || compare(proj(*p1), proj(*p2)) > 0) {
                std::iter_swap(p0--, p1--);
            } else {
                std::iter_swap(p0--, p2--);
            }
        }
        if (p2 != p0) {
            while (p2 >= middle) {
                std::iter_swap(p0--, p2--);
            }
        }
    }

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto grail_SmartMergeWithBuffer(RandomAccessIterator first, RandomAccessIterator middle,
                                    RandomAccessIterator last, int lkeys,
                                    int atype, Compare compare, Projection projection)
        -> std::pair<int, int>
    {
        auto&& proj = utility::as_function(projection);

        RandomAccessIterator p0 = first - lkeys,
                             p1 = first,
                             p2 = middle,
                             q1 = p2,
                             q2 = last;

        int ftype = 1 - atype;  // 1 if inverted
        while (p1 < q1 && p2 < q2) {
            if (compare(proj(*p1), proj(*p2)) - ftype < 0) {
                std::iter_swap(p0++, p1++);
            } else {
                std::iter_swap(p0++, p2++);
            }
        }

        int len;
        if (p1 < q1) {
            len = std::distance(p1, q1);
            while (p1 < q1) {
                std::iter_swap(--q1, --q2);
            }
        } else {
            len = std::distance(p2, q2);
            atype = ftype;
        }
        return { len, atype };
    }

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto grail_SmartMergeWithoutBuffer(RandomAccessIterator first, RandomAccessIterator middle,
                                       RandomAccessIterator last, int atype,
                                       Compare compare, Projection projection)
        -> std::pair<int, int>
    {
        auto&& proj = utility::as_function(projection);

        if (middle == last) {
            return { std::distance(first, middle), atype };
        }

        int ftype = 1 - atype;
        if (first != middle && compare(proj(*std::prev(middle)), proj(*middle)) - ftype >= 0) {
            while (first != middle) {
                int h = ftype ? (lower_bound(middle, last, proj(*first), compare.base(), projection) - middle)
                              : (upper_bound(middle, last, proj(*first), compare.base(), projection) - middle);
                if (h != 0) {
                    std::rotate(first, middle, middle + h);
                    first += h;
                    middle += h;
                }
                if (middle == last) {
                    return { std::distance(first, middle), atype };
                }
                do {
                    ++first;
                } while (first != middle && compare(proj(*first), proj(*middle)) - ftype < 0);
            }
        }
        return { std::distance(middle, last), ftype };
    }

    // Sort With Extra Buffer

    template<typename ForwardIterator, typename Compare, typename Projection>
    auto grail_MergeLeftWithXBuf(ForwardIterator first, ForwardIterator middle,
                                 ForwardIterator last, ForwardIterator out,
                                 Compare compare, Projection projection)
        -> void
    {
        merge_move(first, middle, middle, last, out,
                   compare.base(), projection, projection);
    }

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto grail_SmartMergeWithXBuf(RandomAccessIterator arr, int len1, int len2,
                                  int atype, int lkeys, Compare compare, Projection projection)
        -> std::pair<int, int>
    {
        auto&& proj = utility::as_function(projection);

        int p0 = -lkeys,
            p1 = 0,
            p2 = len1,
            q1 = p2,
            q2 = p2 + len2;

        int ftype = 1 - atype;  // 1 if inverted
        while (p1 < q1 && p2 < q2){
            if (compare(proj(arr[p1]), proj(arr[p2])) - ftype < 0) {
                arr[p0++] = std::move(arr[p1++]);
            } else {
                arr[p0++] = std::move(arr[p2++]);
            }
        }
        if (p1 < q1) {
            len1 = q1 - p1;
            while (p1 < q1) {
                arr[--q2] = std::move(arr[--q1]);
            }
        } else {
            len1 = q2 - p2;
            atype = ftype;
        }
        return { len1, atype };
    }

    // arr - starting array. arr[-lblock..-1] - buffer (if havebuf).
    // lblock - length of regular blocks. First nblocks are stable sorted by 1st elements and key-coded
    // keys - arrays of keys, in same order as blocks. key<midkey means stream A
    // nblock2 are regular blocks from stream A. llast is length of last (irregular) block from stream B, that should go before nblock2 blocks.
    // llast=0 requires nblock2=0 (no irregular blocks). llast>0, nblock2=0 is possible.
    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto grail_MergeBuffersLeftWithXBuf(RandomAccessIterator keys, RandomAccessIterator midkey,
                                        RandomAccessIterator arr, int nblock, int lblock, int nblock2,
                                        int llast, Compare compare, Projection projection)
        -> void
    {
        auto&& proj = utility::as_function(projection);
        int lrest, frest, cidx, fnext;
        RandomAccessIterator prest;

        if (nblock == 0) {
            RandomAccessIterator l = arr + nblock2 * lblock;
            grail_MergeLeftWithXBuf(arr, l, l+llast, arr-lblock, compare, projection);
            return;
        }

        lrest = lblock;
        frest = compare(proj(*keys), proj(*midkey)) < 0 ? 0 : 1;
        RandomAccessIterator pidx = arr + lblock;
        for (cidx = 1 ; cidx < nblock ; ++cidx, pidx += lblock) {
            prest = pidx - lrest;
            fnext = compare(proj(keys[cidx]), proj(*midkey)) < 0 ? 0 : 1;
            if (fnext == frest) {
                std::move(prest, prest+lrest, prest-lblock);
                prest = pidx;
                lrest = lblock;
            } else {
                std::tie(lrest, frest) = grail_SmartMergeWithXBuf(prest, lrest, lblock,
                                                                  frest, lblock,
                                                                  compare, projection);
            }
        }
        prest = pidx - lrest;
        if (llast) {
            if (frest) {
                std::move(prest, prest+lrest, prest-lblock);
                prest = pidx;
                lrest = lblock * nblock2;
            } else {
                lrest += lblock * nblock2;
            }
            grail_MergeLeftWithXBuf(prest, prest+lrest, prest+lrest+llast,
                                    prest-lblock, compare, projection);
        } else {
            std::move(prest, prest+lrest, prest-lblock);
        }
    }

    /***** End Sort With Extra Buffer *****/

    // build blocks of length K
    // input: [-K,-1] elements are buffer
    // output: first K elements are buffer, blocks 2*K and last subblock sorted
    template<typename RandomAccessIterator, typename BufferIterator,
             typename Compare, typename Projection>
    auto grail_BuildBlocks(RandomAccessIterator arr, int L, int K,
                           BufferIterator extbuf, int LExtBuf,
                           Compare compare, Projection projection)
        -> void
    {
        auto&& proj = utility::as_function(projection);

        int m, u, h, p0, p1, rest, restk, p, kbuf;
        kbuf = std::min(K, LExtBuf);
        while (kbuf & (kbuf - 1)) {
            kbuf &= kbuf - 1;  // max power or 2 - just in case
        }

        if (kbuf) {
            std::move(arr - kbuf, arr, extbuf);
            for(m=1;m<L;m+=2){
                u=0;
                if (compare(proj(arr[m-1]), proj(arr[m])) > 0) {
                    u = 1;
                }
                arr[m-3] = std::move(arr[m-1+u]);
                arr[m-2] = std::move(arr[m-u]);
            }
            if (L % 2) {
                arr[L-3] = std::move(arr[L-1]);
            }
            arr-=2;
            for(h=2;h<kbuf;h*=2){
                p0=0;
                p1=L-2*h;
                while(p0<=p1){
                    grail_MergeLeftWithXBuf(arr+p0, arr+p0+h, arr+p0+h+h, arr+p0-h, compare, projection);
                    p0+=2*h;
                }
                rest = L - p0;
                if (rest > h) {
                    grail_MergeLeftWithXBuf(arr+p0, arr+p0+h, arr+p0+rest, arr+p0-h, compare, projection);
                } else {
                    for (; p0 < L ; ++p0) {
                        arr[p0-h] = std::move(arr[p0]);
                    }
                }
                arr-=h;
            }
            std::move(extbuf, extbuf + kbuf, arr + L);
        } else{
            for(m=1;m<L;m+=2){
                u=0;
                if (compare(proj(arr[m-1]), proj(arr[m])) > 0) {
                    u = 1;
                }
                std::iter_swap(arr+(m-3),arr+(m-1+u));
                std::iter_swap(arr+(m-2),arr+(m-u));
            }
            if(L%2) std::iter_swap(arr+(L-1),arr+(L-3));
            arr-=2;
            h=2;
        }
        for(;h<K;h*=2){
            p0=0;
            p1=L-2*h;
            while(p0<=p1){
                grail_MergeLeft(arr+p0, arr+p0+h, arr+p0+h+h, arr+p0-h, compare, projection);
                p0+=2*h;
            }
            rest=L-p0;
            if (rest > h) {
                grail_MergeLeft(arr+p0, arr+p0+h, arr+p0+rest, arr+p0-h, compare, projection);
            } else {
                std::rotate(arr+p0-h, arr+p0, arr+p0+rest);
            }
            arr-=h;
        }
        restk = L % (2 * K);
        p = L - restk;
        if (restk <= K) {
            std::rotate(arr+p, arr+L, arr+L+K);
        } else {
            grail_MergeRight(arr+p, arr+p+K, arr+p+restk, arr+p+restk+K, compare, projection);
        }
        while (p > 0) {
            p -= 2 * K;
            grail_MergeRight(arr+p, arr+p+K, arr+p+K+K, arr+p+K+K+K, compare, projection);
        }
    }

    // arr - starting array. arr[-lblock..-1] - buffer (if havebuf).
    // lblock - length of regular blocks. First nblocks are stable sorted by 1st elements and key-coded
    // keys - arrays of keys, in same order as blocks. key<midkey means stream A
    // nblock2 are regular blocks from stream A. llast is length of last (irregular) block from stream B, that should go before nblock2 blocks.
    // llast=0 requires nblock2=0 (no irregular blocks). llast>0, nblock2=0 is possible.
    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto grail_MergeBuffersLeft(RandomAccessIterator keys, RandomAccessIterator midkey,
                                RandomAccessIterator arr, int nblock, int lblock, bool havebuf,
                                int nblock2, int llast, Compare compare, Projection projection)
        -> void
    {
        auto&& proj = utility::as_function(projection);
        int l, lrest, frest, cidx, fnext;
        RandomAccessIterator prest;

        if (nblock == 0) {
            l = nblock2 * lblock;
            if (havebuf) {
                grail_MergeLeft(arr, arr+l, arr+l+llast, arr-lblock, compare, projection);
            } else {
                grail_MergeWithoutBuffer(arr, arr+l, arr+l+llast, compare, projection);
            }
            return;
        }

        lrest = lblock;
        frest = compare(proj(*keys), proj(*midkey)) < 0 ? 0 : 1;
        RandomAccessIterator pidx = arr + lblock;
        for (cidx = 1 ; cidx < nblock ; ++cidx, pidx += lblock) {
            prest = pidx - lrest;
            fnext=compare(proj(keys[cidx]),proj(*midkey)) < 0 ? 0 : 1;
            if(fnext == frest) {
                if (havebuf) {
                    std::swap_ranges(prest-lblock, prest-lblock+lrest, prest);
                }
                prest = pidx;
                lrest = lblock;
            } else {
                if (havebuf) {
                    std::tie(lrest, frest) = grail_SmartMergeWithBuffer(prest, prest+lrest, prest+lrest+lblock,
                                                                        lblock, frest,
                                                                        compare, projection);
                } else {
                    std::tie(lrest, frest) = grail_SmartMergeWithoutBuffer(prest, prest+lrest, prest+lrest+lblock,
                                                                           frest, compare, projection);
                }

            }
        }
        prest = pidx - lrest;
        if (llast) {
            if (frest) {
                if (havebuf) {
                    std::swap_ranges(prest-lblock, prest-lblock+lrest, prest);
                }
                prest = pidx;
                lrest = lblock * nblock2;
            } else {
                lrest += lblock * nblock2;
            }
            if (havebuf) {
                grail_MergeLeft(prest, prest+lrest, prest+lrest+llast,
                                prest-lblock, compare, projection);
            } else {
                grail_MergeWithoutBuffer(prest, prest+lrest, prest+lrest+llast, compare, projection);
            }
        } else {
            if (havebuf) {
                std::swap_ranges(prest, prest+lrest, prest-lblock);
            }
        }
    }

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto grail_LazyStableSort(RandomAccessIterator first, RandomAccessIterator last,
                              Compare compare, Projection projection)
        -> void
    {
        auto size = std::distance(first, last);
        auto&& proj = utility::as_function(projection);

        for (RandomAccessIterator it = std::next(first) ; it < last ; it += 2) {
            if (compare(proj(*std::prev(it)), proj(*it)) > 0) {
                std::iter_swap(std::prev(it), it);
            }
        }

        for (int h = 2 ; h < size ; h *= 2) {
            RandomAccessIterator p0 = first;
            RandomAccessIterator p1 = last - 2 * h;
            while (p0 <= p1) {
                grail_MergeWithoutBuffer(p0, p0+h, p0+h+h, compare, projection);
                p0 += 2 * h;
            }
            int rest = last - p0;
            if (rest > h) {
                grail_MergeWithoutBuffer(p0, p0+h, p0+rest, compare, projection);
            }
        }
    }

    // keys are on the left of arr. Blocks of length LL combined. We'll combine them in pairs
    // LL and nkeys are powers of 2. (2*LL/lblock) keys are guarantied
    template<typename RandomAccessIterator, typename BufferIterator,
             typename Compare, typename Projection>
    auto grail_CombineBlocks(RandomAccessIterator keys, RandomAccessIterator arr, int len, int LL,
                             int lblock, bool havebuf, BufferIterator xbuf,
                             Compare compare, Projection projection)
        -> void
    {
        auto&& proj = utility::as_function(projection);

        int M,b,NBlk,midkey,lrest,u,p,v,kc,nbl2,llast;
        RandomAccessIterator arr1;

        M=len/(2*LL);
        lrest=len%(2*LL);
        if(lrest<=LL){
            len-=lrest;
            lrest=0;
        }
        if (xbuf) {
            std::move(arr - lblock, arr, xbuf);
        }
        for(b=0;b<=M;b++){
            if(b==M && lrest==0) break;
            arr1=arr+b*2*LL;
            NBlk=(b==M ? lrest : 2*LL)/lblock;
            insertion_sort(keys, keys + NBlk + (b == M ? 1 : 0),
                           compare.base(), projection);
            midkey=LL/lblock;
            for(u=1;u<NBlk;u++){
                p=u-1;
                for(v=u;v<NBlk;v++){
                    kc = compare(proj(arr1[p*lblock]), proj(arr1[v*lblock]));
                    if (kc > 0 || (kc == 0 && compare(proj(keys[p]), proj(keys[v])) > 0)) {
                        p = v;
                    }
                }
                if(p!=u-1){
                    std::swap_ranges(arr1+(u-1)*lblock,arr1+u*lblock,arr1+p*lblock);
                    std::iter_swap(keys+(u-1),keys+p);
                    if(midkey==u-1 || midkey==p) midkey^=(u-1)^p;
                }
            }
            nbl2=llast=0;
            if(b==M) llast=lrest%lblock;
            if(llast!=0){
                while (nbl2 < NBlk &&
                       compare(proj(arr1[NBlk*lblock]), proj(arr1[(NBlk-nbl2-1)*lblock])) < 0) {
                    ++nbl2;
                }
            }
            if (xbuf) {
                grail_MergeBuffersLeftWithXBuf(keys,keys+midkey,arr1,NBlk-nbl2,lblock,nbl2,llast,compare,projection);
            } else {
                grail_MergeBuffersLeft(keys,keys+midkey,arr1,NBlk-nbl2,lblock,havebuf,nbl2,llast,compare,projection);
            }
        }
        if (xbuf) {
            for (p = len ; --p >= 0;) {
                arr[p] = std::move(arr[p-lblock]);
            }
            std::move(xbuf, xbuf + lblock, arr - lblock);
        } else if(havebuf) while(--len>=0) std::iter_swap(arr+len,arr+len-lblock);
    }

    template<typename RandomAccessIterator, typename BufferIterator,
             typename Compare, typename Projection>
    auto grail_commonSort(RandomAccessIterator first, RandomAccessIterator last,
                          BufferIterator extbuf, int LExtBuf,
                          Compare compare, Projection projection)
        -> void
    {
        auto size = std::distance(first, last);

        int lblock,nkeys,findkeys,ptr,cbuf,lb,nk;
        bool havebuf,chavebuf;
        long long s;

        if (size < 16) {
            insertion_sort(first, last, compare.base(), projection);
            return;
        }

        lblock = 1;
        while (lblock * lblock < size) {
            lblock *= 2;
        }
        nkeys = (size - 1) / lblock + 1;
        findkeys = grail_FindKeys(first, last, nkeys + lblock, compare, projection);
        havebuf = true;
        if (findkeys < nkeys + lblock) {
            if (findkeys < 4) {
                grail_LazyStableSort(first, last, compare, projection);
                return;
            }
            nkeys = lblock;
            while (nkeys > findkeys) {
                nkeys /= 2;
            }
            havebuf = false;
            lblock = 0;
        }
        ptr = lblock + nkeys;
        cbuf = havebuf ? lblock : nkeys;
        if (havebuf) {
            grail_BuildBlocks(first + ptr, size - ptr, cbuf, extbuf, LExtBuf, compare, projection);
        } else {
            using T = typename std::iterator_traits<BufferIterator>::value_type;
            grail_BuildBlocks(first + ptr, size - ptr, cbuf, static_cast<T*>(nullptr), 0, compare, projection);
        }

        // 2*cbuf are built
        while (size - ptr > (cbuf *= 2)) {
            lb = lblock;
            chavebuf = havebuf;
            if (!havebuf) {
                if (nkeys > 4 && nkeys / 8 * nkeys >= cbuf) {
                    lb = nkeys / 2;
                    chavebuf = true;
                } else {
                    nk = 1;
                    s = (long long) cbuf * findkeys / 2;
                    while (nk < nkeys && s != 0) {
                        nk *= 2;
                        s /= 8;
                    }
                    lb = (2 * cbuf) / nk;
                }
            } else {
    #if 0
                if(LExtBuf!=0){
                    while(lb>LExtBuf && lb*lb>2*cbuf) lb/=2;  // set size of block close to sqrt(new_block_length)
                }
    #endif
            }
            grail_CombineBlocks(first, first + ptr, size - ptr, cbuf, lb,
                                chavebuf, chavebuf && lb <= LExtBuf ? extbuf : nullptr,
                                compare, projection);
        }
        insertion_sort(first, first + ptr, compare.base(), projection);
        grail_MergeWithoutBuffer(first, first+ptr, first+size, compare, projection);
    }

    template<typename BufferProvider, typename RandomAccessIterator,
             typename Compare, typename Projection>
    auto grail_sort(RandomAccessIterator first, RandomAccessIterator last,
                    Compare compare, Projection projection)
        -> void
    {
        using T = typename std::iterator_traits<RandomAccessIterator>::value_type;

        // Allocate temporary buffer
        std::size_t size = std::distance(first, last);
        typename BufferProvider::template buffer<T> buffer(size);

        grail_commonSort(first, last, buffer.data(), buffer.size(),
                         three_way_compare<Compare>(compare), projection);
    }
}}

#endif // CPPSORT_DETAIL_GRAIL_SORT_H_
