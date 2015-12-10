/*
 * Grail sorting
 *
 * (c) 2013 by Andrey Astrelin
 * Modified in 2015 by Morwenn for inclusion into cpp-sort
 *
 *
 * Stable sorting that works in O(N*log(N)) worst time
 * and uses O(1) extra memory
 *
 * Define SORT_TYPE and SORT_CMP
 * and then call GrailSort() function
 *
 * For sorting with fixed external buffer (512 items)
 * use GrailSortWithBuffer()
 *
 * For sorting with dynamic external buffer (O(sqrt(N))
 * items)
 * use GrailSortWithDynBuffer()
 *
 * Also classic in-place merge sort is implemented
 * under the name of RecStableSort()
 *
 */
#ifndef CPPSORT_DETAIL_GRAIL_SORT_H_
#define CPPSORT_DETAIL_GRAIL_SORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <iterator>
#include <cpp-sort/utility/as_function.h>
#include "insertion_sort.h"

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

    template<typename RandomAccessIterator>
    auto grail_rotate(RandomAccessIterator arr, int l1, int l2)
        -> void
    {
        while(l1 && l2){
            if(l1<=l2){
                std::swap_ranges(arr,arr+l1,arr+l1);
                arr+=l1; l2-=l1;
            } else{
                std::swap_ranges(arr+(l1-l2),arr+l1,arr+l1);
                l1-=l2;
            }
        }
    }

    template<typename RandomAccessIterator, typename T,
             typename Compare, typename Projection>
    auto grail_BinSearchLeft(RandomAccessIterator arr, int len, const T& key,
                             Compare compare, Projection projection)
        -> int
    {
        auto&& proj = utility::as_function(projection);

        int a=-1,b=len,c;
        while(a<b-1){
            c=a+((b-a)>>1);
            if(compare(proj(arr[c]), key)>=0) b=c;
            else a=c;
        }
        return b;
    }

    template<typename RandomAccessIterator, typename T,
             typename Compare, typename Projection>
    auto grail_BinSearchRight(RandomAccessIterator arr, int len, const T& key,
                              Compare compare, Projection projection)
        -> int
    {
        auto&& proj = utility::as_function(projection);

        int a=-1,b=len,c;
        while(a<b-1){
            c=a+((b-a)>>1);
            if(compare(proj(arr[c]), key)>0) b=c;
            else a=c;
        }
        return b;
    }

    // cost: 2*len+nk^2/2
    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto grail_FindKeys(RandomAccessIterator arr, int len, int nkeys,
                        Compare compare, Projection projection)
        -> int
    {
        auto&& proj = utility::as_function(projection);

        int h = 1, h0 = 0;  // first key is always here
        int u = 1, r;
        while (u < len && h < nkeys) {
            r = grail_BinSearchLeft(arr+h0, h, proj(arr[u]), compare, projection);
            if (r == h || compare(proj(arr[u]), proj(arr[h0+r])) != 0) {
                grail_rotate(arr + h0, h, u - (h0+h));
                h0 = u - h;
                grail_rotate(arr + (h0+r), h - r, 1);
                ++h;
            }
            ++u;
        }
        grail_rotate(arr, h0, h);
        return h;
    }

    // cost: min(L1,L2)^2+max(L1,L2)
    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto grail_MergeWithoutBuffer(RandomAccessIterator arr, int len1, int len2,
                                  Compare compare, Projection projection)
        -> void
    {
        auto&& proj = utility::as_function(projection);

        int h;
        if(len1<len2){
            while(len1){
                h=grail_BinSearchLeft(arr+len1,len2,proj(arr[0]),compare,projection);
                if(h!=0){
                    grail_rotate(arr,len1,h);
                    arr+=h;
                    len2-=h;
                }
                if(len2==0) break;
                do {
                    ++arr;
                    --len1;
                } while (len1 && compare(proj(arr[0]), proj(arr[len1])) <= 0);
            }
        } else{
            while(len2){
                h=grail_BinSearchRight(arr,len1,proj(arr[len1+len2-1]),compare,projection);
                if(h!=len1){
                    grail_rotate(arr+h,len1-h,len2);
                    len1=h;
                }
                if(len1==0) break;
                do {
                    --len2;
                } while (len2 && compare(proj(arr[len1-1]), proj(arr[len1+len2-1])) <= 0);
            }
        }
    }

    // arr[M..-1] - buffer, arr[0,L1-1]++arr[L1,L1+L2-1] -> arr[M,M+L1+L2-1]
    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto grail_MergeLeft(RandomAccessIterator arr, int L1, int L2, int M,
                         Compare compare, Projection projection)
        -> void
    {
        auto&& proj = utility::as_function(projection);

        int p0 = 0, p1 = L1;
        L2 += L1;
        while (p1 < L2) {
            if (p0 == L1 || compare(proj(arr[p0]), proj(arr[p1])) > 0) {
                std::iter_swap(arr+(M++), arr+(p1++));
            } else {
                std::iter_swap(arr+(M++), arr+(p0++));
            }
        }
        if (M != p0) {
            std::swap_ranges(arr+M, arr+M+L1-p0, arr+p0);
        }
    }

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto grail_MergeRight(RandomAccessIterator arr, int L1, int L2, int M,
                          Compare compare, Projection projection)
        -> void
    {
        auto&& proj = utility::as_function(projection);

        int p0 = L1 + L2 + M - 1;
        int p1 = L1 - 1;
        int p2 = L1 + L2 - 1;

        while (p1 >= 0) {
            if (p2 < L1 || compare(proj(arr[p1]), proj(arr[p2])) > 0) {
                std::iter_swap(arr+(p0--), arr+(p1--));
            } else {
                std::iter_swap(arr+(p0--), arr+(p2--));
            }
        }
        if (p2 != p0) {
            while (p2 >= L1) {
                std::iter_swap(arr+(p0--), arr+(p2--));
            }
        }
    }

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto grail_SmartMergeWithBuffer(RandomAccessIterator arr, int *alen1, int *atype,
                                    int len2, int lkeys, Compare compare, Projection projection)
        -> void
    {
        auto&& proj = utility::as_function(projection);

        int p0=-lkeys,p1=0,p2=*alen1,q1=p2,q2=p2+len2;
        int ftype=1-*atype;  // 1 if inverted
        while(p1<q1 && p2<q2){
            if (compare(proj(arr[p1]), proj(arr[p2])) - ftype < 0) {
                std::iter_swap(arr+(p0++), arr+(p1++));
            } else {
                std::iter_swap(arr+(p0++), arr+(p2++));
            }
        }
        if(p1<q1){
            *alen1=q1-p1;
            while(p1<q1) std::iter_swap(arr+(--q1),arr+(--q2));
        } else{
            *alen1=q2-p2;
            *atype=ftype;
        }
    }

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto grail_SmartMergeWithoutBuffer(RandomAccessIterator arr, int *alen1, int *atype,
                                       int _len2, Compare compare, Projection projection)
        -> void
    {
        auto&& proj = utility::as_function(projection);
        int len1,len2,ftype,h;

        if(!_len2) return;
        len1=*alen1;
        len2=_len2;
        ftype=1-*atype;
        if (len1 && compare(proj(arr[len1-1]), proj(arr[len1])) - ftype >= 0) {
            while(len1){
                h=ftype ? grail_BinSearchLeft(arr+len1,len2,proj(arr[0]),compare,projection)
                        : grail_BinSearchRight(arr+len1,len2,proj(arr[0]),compare,projection);
                if(h!=0){
                    grail_rotate(arr,len1,h);
                    arr+=h;
                    len2-=h;
                }
                if(len2==0){
                    *alen1=len1;
                    return;
                }
                do {
                    ++arr;
                    --len1;
                } while (len1 && compare(proj(arr[0]), proj(arr[len1])) - ftype < 0);
            }
        }
        *alen1=len2; *atype=ftype;
    }

    /***** Sort With Extra Buffer *****/

    // arr[M..-1] - free, arr[0,L1-1]++arr[L1,L1+L2-1] -> arr[M,M+L1+L2-1]
    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto grail_MergeLeftWithXBuf(RandomAccessIterator arr, int L1, int L2, int M,
                                 Compare compare, Projection projection)
        -> void
    {
        auto&& proj = utility::as_function(projection);

        int p0 = 0, p1 = L1;
        L2 += L1;
        while (p1 < L2) {
            if (p0 == L1 || compare(proj(arr[p0]), proj(arr[p1])) > 0) {
                arr[M++] = arr[p1++];
            } else {
                arr[M++] = arr[p0++];
            }
        }
        if (M != p0) {
            while (p0 < L1) {
                arr[M++] = arr[p0++];
            }
        }
    }

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto grail_SmartMergeWithXBuf(RandomAccessIterator arr, int *alen1, int *atype,
                                  int len2, int lkeys, Compare compare, Projection projection)
        -> void
    {
        auto&& proj = utility::as_function(projection);

        int p0=-lkeys,p1=0,p2=*alen1,q1=p2,q2=p2+len2;
        int ftype=1-*atype;  // 1 if inverted
        while(p1<q1 && p2<q2){
            if (compare(proj(arr[p1]), proj(arr[p2])) - ftype < 0) {
                arr[p0++] = arr[p1++];
            } else {
                arr[p0++] = arr[p2++];
            }
        }
        if(p1<q1){
            *alen1=q1-p1;
            while(p1<q1) arr[--q2]=arr[--q1];
        } else{
            *alen1=q2-p2;
            *atype=ftype;
        }
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
        int l,prest,lrest,frest,pidx,cidx,fnext;

        if(nblock==0){
            l=nblock2*lblock;
            grail_MergeLeftWithXBuf(arr,l,llast,-lblock,compare,projection);
            return;
        }

        lrest = lblock;
        frest = compare(proj(*keys), proj(*midkey)) < 0 ? 0 : 1;
        pidx = lblock;
        for(cidx=1;cidx<nblock;cidx++,pidx+=lblock){
            prest = pidx - lrest;
            fnext = compare(proj(keys[cidx]), proj(*midkey)) < 0 ? 0 : 1;
            if(fnext==frest){
                std::copy(arr+prest, arr+prest+lrest, arr+prest-lblock);
                prest=pidx;
                lrest=lblock;
            } else{
                grail_SmartMergeWithXBuf(arr+prest,&lrest,&frest,lblock,lblock,compare,projection);
            }
        }
        prest = pidx - lrest;
        if (llast) {
            if (frest) {
                std::copy(arr+prest, arr+prest+lrest, arr+prest-lblock);
                prest = pidx;
                lrest = lblock * nblock2;
                frest = 0;
            } else {
                lrest += lblock * nblock2;
            }
            grail_MergeLeftWithXBuf(arr+prest, lrest, llast, -lblock, compare,projection);
        } else {
            std::copy(arr+prest, arr+prest+lrest, arr+prest-lblock);
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

        int m,u,h,p0,p1,rest,restk,p,kbuf;
        kbuf=K<LExtBuf ? K : LExtBuf;
        while(kbuf&(kbuf-1)) kbuf&=kbuf-1;  // max power or 2 - just in case

        if (kbuf) {
            std::copy(arr - kbuf, arr, extbuf);
            for(m=1;m<L;m+=2){
                u=0;
                if (compare(proj(arr[m-1]), proj(arr[m])) > 0) {
                    u = 1;
                }
                arr[m-3] = arr[m-1+u];
                arr[m-2] = arr[m-u];
            }
            if(L%2) arr[L-3]=arr[L-1];
            arr-=2;
            for(h=2;h<kbuf;h*=2){
                p0=0;
                p1=L-2*h;
                while(p0<=p1){
                    grail_MergeLeftWithXBuf(arr+p0,h,h,-h,compare,projection);
                    p0+=2*h;
                }
                rest=L-p0;
                if(rest>h){
                    grail_MergeLeftWithXBuf(arr+p0,h,rest-h,-h,compare,projection);
                } else {
                    for(;p0<L;p0++)	arr[p0-h]=arr[p0];
                }
                arr-=h;
            }
            std::copy(extbuf, extbuf + kbuf, arr + L);
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
                grail_MergeLeft(arr+p0,h,h,-h,compare,projection);
                p0+=2*h;
            }
            rest=L-p0;
            if(rest>h){
                grail_MergeLeft(arr+p0,h,rest-h,-h,compare,projection);
            } else grail_rotate(arr+p0-h,h,rest);
            arr-=h;
        }
        restk=L%(2*K);
        p=L-restk;
        if(restk<=K) grail_rotate(arr+p,restk,K);
        else grail_MergeRight(arr+p,K,restk-K,K,compare,projection);
        while(p>0){
            p-=2*K;
            grail_MergeRight(arr+p,K,K,K,compare,projection);
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
        int l,prest,lrest,frest,pidx,cidx,fnext;

        if(nblock==0){
            l=nblock2*lblock;
            if(havebuf) grail_MergeLeft(arr,l,llast,-lblock,compare,projection);
            else grail_MergeWithoutBuffer(arr,l,llast,compare,projection);
            return;
        }

        lrest=lblock;
        frest=compare(proj(*keys),proj(*midkey)) < 0 ? 0 : 1;
        pidx=lblock;
        for(cidx=1;cidx<nblock;cidx++,pidx+=lblock){
            prest=pidx-lrest;
            fnext=compare(proj(keys[cidx]),proj(*midkey)) < 0 ? 0 : 1;
            if(fnext==frest) {
                if (havebuf) {
                    std::swap_ranges(arr+prest-lblock,arr+prest-lblock+lrest,arr+prest);
                }
                prest=pidx;
                lrest=lblock;
            } else{
                if(havebuf) {
                    grail_SmartMergeWithBuffer(arr+prest,&lrest,&frest,lblock,lblock,compare,projection);
                } else {
                    grail_SmartMergeWithoutBuffer(arr+prest,&lrest,&frest,lblock,compare,projection);
                }

            }
        }
        prest=pidx-lrest;
        if (llast) {
            if (frest) {
                if (havebuf) {
                    std::swap_ranges(arr+prest-lblock,arr+prest-lblock+lrest,arr+prest);
                }
                prest=pidx;
                lrest=lblock*nblock2;
                frest=0;
            } else{
                lrest+=lblock*nblock2;
            }
            if (havebuf) {
                grail_MergeLeft(arr+prest,lrest,llast,-lblock,compare,projection);
            }
            else grail_MergeWithoutBuffer(arr+prest,lrest,llast,compare,projection);
        } else {
            if (havebuf) {
                std::swap_ranges(arr+prest,arr+prest+lrest,arr+(prest-lblock));
            }
        }
    }

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto grail_LazyStableSort(RandomAccessIterator arr, int L,
                              Compare compare, Projection projection)
        -> void
    {
        auto&& proj = utility::as_function(projection);

        int m,h,p0,p1,rest;
        for (m=1;m<L;m+=2) {
            if (compare(proj(arr[m-1]), proj(arr[m])) > 0) {
                std::iter_swap(arr+(m-1),arr+m);
            }
        }
        for (h=2;h<L;h*=2) {
            p0=0;
            p1=L-2*h;
            while (p0<=p1) {
                grail_MergeWithoutBuffer(arr+p0,h,h,compare,projection);
                p0+=2*h;
            }
            rest=L-p0;
            if (rest > h) {
                grail_MergeWithoutBuffer(arr+p0,h,rest-h,compare,projection);
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
            std::copy(arr - lblock, arr, xbuf);
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
            for (p=len;--p>=0;) arr[p]=arr[p-lblock];
            std::copy(xbuf, xbuf + lblock, arr - lblock);
        } else if(havebuf) while(--len>=0) std::iter_swap(arr+len,arr+len-lblock);
    }

    template<typename RandomAccessIterator, typename BufferIterator,
             typename Compare, typename Projection>
    auto grail_commonSort(RandomAccessIterator arr, int Len,
                          BufferIterator extbuf, int LExtBuf,
                          Compare compare, Projection projection)
        -> void
    {
        int lblock,nkeys,findkeys,ptr,cbuf,lb,nk;
        bool havebuf,chavebuf;
        long long s;

        if (Len < 16) {
            insertion_sort(arr, arr + Len, compare.base(), projection);
            return;
        }

        lblock=1;
        while(lblock*lblock<Len) lblock*=2;
        nkeys=(Len-1)/lblock+1;
        findkeys=grail_FindKeys(arr,Len,nkeys+lblock,compare,projection);
        havebuf=true;
        if(findkeys<nkeys+lblock){
            if(findkeys<4){
                grail_LazyStableSort(arr,Len,compare,projection);
                return;
            }
            nkeys=lblock;
            while(nkeys>findkeys) nkeys/=2;
            havebuf=false;
            lblock=0;
        }
        ptr=lblock+nkeys;
        cbuf=havebuf ? lblock : nkeys;
        if (havebuf) {
            grail_BuildBlocks(arr+ptr,Len-ptr,cbuf,extbuf,LExtBuf,compare,projection);
        } else {
            using T = typename std::iterator_traits<BufferIterator>::value_type;
            grail_BuildBlocks(arr+ptr,Len-ptr,cbuf,static_cast<T*>(nullptr),0,compare,projection);
        }

        // 2*cbuf are built
        while(Len-ptr>(cbuf*=2)){
            lb=lblock;
            chavebuf=havebuf;
            if(!havebuf){
                if(nkeys>4 && nkeys/8*nkeys>=cbuf){
                    lb=nkeys/2;
                    chavebuf=true;
                } else{
                    nk=1;
                    s=(long long)cbuf*findkeys/2;
                    while(nk<nkeys && s!=0){
                        nk*=2; s/=8;
                    }
                    lb=(2*cbuf)/nk;
                }
            } else{
    #if 0
                if(LExtBuf!=0){
                    while(lb>LExtBuf && lb*lb>2*cbuf) lb/=2;  // set size of block close to sqrt(new_block_length)
                }
    #endif
            }
            grail_CombineBlocks(arr, arr+ptr, Len-ptr, cbuf, lb,
                                chavebuf, chavebuf && lb<=LExtBuf ? extbuf : nullptr,
                                compare, projection);
        }
        insertion_sort(arr, arr + ptr, compare.base(), projection);
        grail_MergeWithoutBuffer(arr,ptr,Len-ptr,compare,projection);
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

        three_way_compare<Compare> cmp(compare);
        grail_commonSort(first, size, buffer.data(), buffer.size(),
                         cmp, projection);
    }
}}

#endif // CPPSORT_DETAIL_GRAIL_SORT_H_
