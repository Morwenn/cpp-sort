/*
 * C++ implementation of timsort
 *
 * ported from Python's and OpenJDK's:
 * - http://svn.python.org/projects/python/trunk/Objects/listobject.c
 * - http://cr.openjdk.java.net/~martin/webrevs/openjdk7/timsort/raw_files/new/src/share/classes/java/util/TimSort.java
 *
 * Copyright (c) 2011 Fuji, Goro (gfx) <gfuji@cpan.org>.
 * Modified in 2015-2019 by Morwenn for inclusion into cpp-sort.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */
#ifndef CPPSORT_DETAIL_TIMSORT_H_
#define CPPSORT_DETAIL_TIMSORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <cassert>
#include <iterator>
#include <memory>
#include <new>
#include <type_traits>
#include <utility>
#include <vector>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/iter_move.h>
#include "iterator_traits.h"
#include "lower_bound.h"
#include "memory.h"
#include "move.h"
#include "reverse.h"
#include "type_traits.h"
#include "upper_bound.h"

namespace cppsort
{
namespace detail
{
    template<typename Iterator>
    struct run
    {
        using difference_type = difference_type_t<Iterator>;

        Iterator base;
        difference_type len;

        run(Iterator base, difference_type len):
            base(std::move(base)),
            len(std::move(len))
        {}
    };

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    class TimSort
    {
        using iterator = RandomAccessIterator;
        using value_type = value_type_t<iterator>;
        using rvalue_reference = remove_cvref_t<rvalue_reference_t<iterator>>;
        using difference_type = difference_type_t<iterator>;

        static constexpr int min_merge = 32;
        static constexpr int min_gallop = 7;

        int minGallop_ = min_gallop;

        // Buffer used for merges
        std::unique_ptr<rvalue_reference, operator_deleter> buffer;
        std::ptrdiff_t buffer_size = 0;

        // Silence GCC -Winline warning
        ~TimSort() noexcept {}

        std::vector<run<iterator>> pending_;

        static auto sort(iterator const lo, iterator const hi, Compare compare, Projection projection)
            -> void
        {
            assert( lo <= hi );

            difference_type nRemaining = std::distance(lo, hi);
            if (nRemaining < 2) {
                return; // nothing to do
            }

            if (nRemaining < min_merge) {
                difference_type const initRunLen = countRunAndMakeAscending(lo, hi, compare, projection);
                binarySort(lo, hi, lo + initRunLen, std::move(compare), std::move(projection));
                return;
            }

            TimSort ts{};
            difference_type const minRun = minRunLength(nRemaining);
            iterator cur          = lo;
            do {
                difference_type runLen = countRunAndMakeAscending(cur, hi, compare, projection);

                if (runLen < minRun) {
                    difference_type const force  = std::min(nRemaining, minRun);
                    binarySort(cur, cur + force, cur + runLen, compare, projection);
                    runLen = force;
                }

                ts.pushRun(cur, runLen);
                ts.mergeCollapse(compare, projection);

                cur        += runLen;
                nRemaining -= runLen;
            } while (nRemaining != 0);

            assert( cur == hi );
            ts.mergeForceCollapse(compare, projection);
            assert( ts.pending_.size() == 1 );
        } // sort()

        static auto binarySort(iterator const lo, iterator const hi, iterator start,
                               Compare compare, Projection projection)
            -> void
        {
            assert( lo <= start );
            assert( start <= hi );

            using utility::iter_move;
            auto&& proj = utility::as_function(projection);

            if (start == lo) {
                ++start;
            }
            for ( ; start < hi; ++start ) {
                assert(lo <= start);
                auto pivot = iter_move(start);

                iterator const pos = upper_bound(lo, start, proj(pivot), compare, projection);
                for (iterator p = start; p > pos; --p) {
                    *p = iter_move(std::prev(p));
                }
                *pos = std::move(pivot);
            }
        }

        static auto countRunAndMakeAscending(iterator const lo, iterator const hi,
                                             Compare compare, Projection projection)
            -> difference_type
        {
            assert( lo < hi );
            auto&& comp = utility::as_function(compare);
            auto&& proj = utility::as_function(projection);

            iterator runHi = std::next(lo);
            if ( runHi == hi ) {
                return 1;
            }

            if (comp(proj(*runHi), proj(*lo))) { // descending
                do {
                    ++runHi;
                } while (runHi < hi && comp(proj(*runHi), proj(*std::prev(runHi))));
                detail::reverse(lo, runHi);
            } else { // ascending
                do {
                    ++runHi;
                } while (runHi < hi && not comp(proj(*runHi), proj(*std::prev(runHi))));
            }

            return runHi - lo;
        }

        static auto minRunLength(difference_type n)
            -> difference_type
        {
            assert( n >= 0 );

            difference_type r = 0;
            while (n >= min_merge) {
                r |= (n & 1);
                n >>= 1;
            }
            return n + r;
        }

        auto pushRun(iterator const runBase, difference_type const runLen)
            -> void
        {
            pending_.emplace_back(runBase, runLen);
        }

        auto mergeCollapse(Compare compare, Projection projection)
            -> void
        {
            while ( pending_.size() > 1 ) {
                difference_type n = pending_.size() - 2;

                if ((n > 0 && pending_[n - 1].len <= pending_[n].len + pending_[n + 1].len)
                    || (n > 1 && pending_[n - 2].len <= pending_[n - 1].len + pending_[n].len)){
                    if (pending_[n - 1].len < pending_[n + 1].len) {
                        --n;
                    }
                    mergeAt(n, compare, projection);
                }
                else if (pending_[n].len <= pending_[n + 1].len) {
                    mergeAt(n, compare, projection);
                }
                else {
                    break;
                }
            }
        }

        auto mergeForceCollapse(Compare compare, Projection projection)
            -> void
        {
            while ( pending_.size() > 1 ) {
                difference_type n = pending_.size() - 2;

                if (n > 0 && pending_[n - 1].len < pending_[n + 1].len) {
                    --n;
                }
                mergeAt(n, compare, projection);
            }
        }

        auto mergeAt(difference_type const i, Compare compare, Projection projection)
            -> void
        {
            difference_type const stackSize = pending_.size();
            assert( stackSize >= 2 );
            assert( i >= 0 );
            assert( i == stackSize - 2 || i == stackSize - 3 );

            iterator base1 = pending_[i].base;
            difference_type len1  = pending_[i].len;
            iterator base2 = pending_[i + 1].base;
            difference_type len2  = pending_[i + 1].len;

            assert( len1 > 0 );
            assert( len2 > 0 );
            assert( base1 + len1 == base2 );

            pending_[i].len = len1 + len2;

            if (i == stackSize - 3) {
                pending_[i + 1] = pending_[i + 2];
            }

            pending_.pop_back();

            difference_type const k = gallopRight(*base2, base1, len1, 0, compare, projection);
            assert( k >= 0 );

            base1 += k;
            len1  -= k;

            if (len1 == 0) {
                return;
            }

            len2 = gallopLeft(base1[len1 - 1], base2, len2, len2 - 1, compare, projection);
            assert( len2 >= 0 );
            if (len2 == 0) {
                return;
            }

            if (len1 <= len2) {
                mergeLo(base1, len1, base2, len2, compare, projection);
            }
            else {
                mergeHi(base1, len1, base2, len2, compare, projection);
            }
        }

        template<typename T, typename Iter>
        auto gallopLeft(const T& key, Iter const base, difference_type const len, difference_type const hint,
                        Compare compare, Projection projection)
            -> difference_type
        {
            assert( len > 0 );
            assert( hint >= 0 );
            assert( hint < len );

            auto&& comp = utility::as_function(compare);
            auto&& proj = utility::as_function(projection);
            auto&& key_proj = proj(key);

            difference_type lastOfs = 0;
            difference_type ofs = 1;

            if (comp(proj(base[hint]), key_proj)) {
                difference_type const maxOfs = len - hint;
                while (ofs < maxOfs && comp(proj(base[hint + ofs]), key_proj)) {
                    lastOfs = ofs;
                    ofs     = (ofs << 1) + 1;

                    if (ofs <= 0) { // int overflow
                        ofs = maxOfs;
                    }
                }
                if (ofs > maxOfs) {
                    ofs = maxOfs;
                }

                lastOfs += hint;
                ofs     += hint;
            }
            else {
                difference_type const maxOfs = hint + 1;
                while (ofs < maxOfs && not comp(proj(base[hint - ofs]), key_proj)) {
                    lastOfs = ofs;
                    ofs     = (ofs << 1) + 1;

                    if (ofs <= 0) {
                        ofs = maxOfs;
                    }
                }
                if (ofs > maxOfs) {
                    ofs = maxOfs;
                }

                difference_type const tmp = lastOfs;
                lastOfs          = hint - ofs;
                ofs              = hint - tmp;
            }
            assert( -1 <= lastOfs );
            assert( lastOfs < ofs );
            assert( ofs <= len );

            return lower_bound(base+(lastOfs+1), base+ofs, key_proj, compare, projection) - base;
        }

        template<typename T, typename Iter>
        auto gallopRight(const T& key, Iter const base, difference_type const len, difference_type const hint,
                         Compare compare, Projection projection)
            -> difference_type
        {
            assert( len > 0 );
            assert( hint >= 0 );
            assert( hint < len );

            auto&& comp = utility::as_function(compare);
            auto&& proj = utility::as_function(projection);
            auto&& key_proj = proj(key);

            difference_type ofs = 1;
            difference_type lastOfs = 0;

            if (comp(key_proj, proj(base[hint]))) {
                difference_type const maxOfs = hint + 1;
                while (ofs < maxOfs && comp(key_proj, proj(base[hint - ofs]))) {
                    lastOfs = ofs;
                    ofs     = (ofs << 1) + 1;

                    if (ofs <= 0) {
                        ofs = maxOfs;
                    }
                }
                if (ofs > maxOfs) {
                    ofs = maxOfs;
                }

                difference_type const tmp = lastOfs;
                lastOfs          = hint - ofs;
                ofs              = hint - tmp;
            }
            else {
                difference_type const maxOfs = len - hint;
                while (ofs < maxOfs && not comp(key_proj, proj(base[hint + ofs]))) {
                    lastOfs = ofs;
                    ofs     = (ofs << 1) + 1;

                    if (ofs <= 0) { // int overflow
                        ofs = maxOfs;
                    }
                }
                if (ofs > maxOfs) {
                    ofs = maxOfs;
                }

                lastOfs += hint;
                ofs     += hint;
            }
            assert( -1 <= lastOfs );
            assert( lastOfs < ofs );
            assert( ofs <= len );

            return upper_bound(base+(lastOfs+1), base+ofs, key_proj, compare, projection) - base;
        }

        auto resize_buffer(std::ptrdiff_t new_size)
            -> void
        {
            // Resize the merge buffer if the old one isn't big enough
            if (buffer_size < new_size) {
                // Release memory first, then allocate again to prevent
                // easily avoidable out-of-memory errors and make sized
                // deallocation work properly
                buffer.reset(nullptr);
                buffer.get_deleter() = operator_deleter(new_size * sizeof(rvalue_reference));
                buffer.reset(static_cast<rvalue_reference*>(
                    ::operator new(new_size * sizeof(rvalue_reference))
                ));
                buffer_size = new_size;
            }
        }

        auto mergeLo(iterator const base1, difference_type len1, iterator const base2, difference_type len2,
                     Compare compare, Projection projection)
            -> void
        {
            assert( len1 > 0 );
            assert( len2 > 0 );
            assert( base1 + len1 == base2 );

            using utility::iter_move;
            auto&& comp = utility::as_function(compare);
            auto&& proj = utility::as_function(projection);

            if (len1 == 1) {
                detail::rotate_left(base1, base2 + len2);
                return;
            }
            if (len2 == 1) {
                detail::rotate_right(base1, base2 + len2);
                return;
            }

            resize_buffer(len1);
            destruct_n<rvalue_reference> d(0);
            std::unique_ptr<rvalue_reference, destruct_n<rvalue_reference>&> h2(buffer.get(), d);

            rvalue_reference* ptr = buffer.get();
            for (auto it = base1 ; it != base1 + len1 ; ++d, (void) ++it, ++ptr) {
                ::new(ptr) rvalue_reference(iter_move(it));
            }

            auto cursor1 = buffer.get();
            auto cursor2 = base2;
            auto dest = base1;

            *dest = iter_move(cursor2);
            ++dest;
            ++cursor2;
            --len2;

            int minGallop(minGallop_);

            // outer:
            while (true) {
                int count1 = 0;
                int count2 = 0;

                bool break_outer = false;
                do {
                    assert( len1 > 1 );
                    assert( len2 > 0 );

                    if (comp(proj(*cursor2), proj(*cursor1))) {
                        *dest = iter_move(cursor2);
                        ++dest;
                        ++cursor2;
                        ++count2;
                        count1 = 0;
                        if (--len2 == 0) {
                            break_outer = true;
                            break;
                        }
                    }
                    else {
                        *dest = iter_move(cursor1);
                        ++dest;
                        ++cursor1;
                        ++count1;
                        count2 = 0;
                        if (--len1 == 1) {
                            break_outer = true;
                            break;
                        }
                    }
                } while ( (count1 | count2) < minGallop );
                if (break_outer) {
                    break;
                }

                do {
                    assert( len1 > 1 );
                    assert( len2 > 0 );

                    count1 = gallopRight(*cursor2, cursor1, len1, 0, compare, projection);
                    if (count1 != 0) {
                        detail::move_backward(cursor1, cursor1 + count1, dest + count1);
                        dest    += count1;
                        cursor1 += count1;
                        len1    -= count1;

                        if (len1 <= 1) {
                            break_outer = true;
                            break;
                        }
                    }
                    *dest = iter_move(cursor2);
                    ++dest;
                    ++cursor2;
                    if (--len2 == 0) {
                        break_outer = true;
                        break;
                    }

                    count2 = gallopLeft(*cursor1, cursor2, len2, 0, compare, projection);
                    if (count2 != 0) {
                        detail::move(cursor2, cursor2 + count2, dest);
                        dest    += count2;
                        cursor2 += count2;
                        len2    -= count2;
                        if (len2 == 0) {
                            break_outer = true;
                            break;
                        }
                    }
                    *dest = iter_move(cursor1);
                    ++dest;
                    ++cursor1;
                    if (--len1 == 1) {
                        break_outer = true;
                        break;
                    }

                    --minGallop;
                } while ( (count1 >= min_gallop) | (count2 >= min_gallop) );
                if (break_outer) {
                    break;
                }

                if (minGallop < 0) {
                    minGallop = 0;
                }
                minGallop += 2;
            } // end of "outer" loop

            minGallop_ = std::min(minGallop, 1);

            if (len1 == 1) {
                assert( len2 > 0 );
                detail::move(cursor2, cursor2 + len2, dest);
                dest[len2] = iter_move(cursor1);
            }
            else {
                assert( len1 != 0 && "comparison function violates its general contract" );
                assert( len2 == 0 );
                assert( len1 > 1 );
                detail::move(cursor1, cursor1 + len1, dest);
            }
        }

        auto mergeHi(iterator const base1, difference_type len1, iterator const base2, difference_type len2,
                     Compare compare, Projection projection)
            -> void
        {
            assert( len1 > 0 );
            assert( len2 > 0 );
            assert( base1 + len1 == base2 );

            using utility::iter_move;
            auto&& comp = utility::as_function(compare);
            auto&& proj = utility::as_function(projection);

            if (len1 == 1) {
                detail::rotate_left(base1, base2 + len2);
                return;
            }
            if (len2 == 1) {
                detail::rotate_right(base1, base2 + len2);
                return;
            }

            resize_buffer(len2);
            destruct_n<rvalue_reference> d(0);
            std::unique_ptr<rvalue_reference, destruct_n<rvalue_reference>&> h2(buffer.get(), d);

            rvalue_reference* ptr = buffer.get();
            for (auto it = base2 ; it != base2 + len2 ; ++d, (void) ++it, ++ptr) {
                ::new(ptr) rvalue_reference(iter_move(it));
            }

            auto cursor1 = base1 + (len1 - 1);
            auto cursor2 = buffer.get() + (len2 - 1);
            auto dest = base2 + (len2 - 1);

            *dest = iter_move(cursor1);
            --dest;
            --cursor1;
            --len1;

            int minGallop( minGallop_ );

            // outer:
            while (true) {
                int count1 = 0;
                int count2 = 0;

                bool break_outer = false;
                do {
                    assert( len1 > 0 );
                    assert( len2 > 1 );

                    if (comp(proj(*cursor2), proj(*cursor1))) {
                        *dest = iter_move(cursor1);
                        --dest;
                        --cursor1;
                        ++count1;
                        count2 = 0;
                        if (--len1 == 0) {
                            break_outer = true;
                            break;
                        }
                    }
                    else {
                        *dest = iter_move(cursor2);
                        --dest;
                        --cursor2;
                        ++count2;
                        count1 = 0;
                        if (--len2 == 1) {
                            break_outer = true;
                            break;
                        }
                    }
                } while ( (count1 | count2) < minGallop );
                if (break_outer) {
                    break;
                }

                do {
                    assert( len1 > 0 );
                    assert( len2 > 1 );

                    count1 = len1 - gallopRight(*cursor2, base1, len1, len1 - 1, compare, projection);
                    if (count1 != 0) {
                        dest    -= count1;
                        cursor1 -= count1;
                        len1    -= count1;
                        detail::move_backward(cursor1 + 1, cursor1 + (1 + count1), dest + (1 + count1));

                        if (len1 == 0) {
                            break_outer = true;
                            break;
                        }
                    }
                    *dest = iter_move(cursor2);
                    --dest;
                    --cursor2;
                    if (--len2 == 1) {
                        break_outer = true;
                        break;
                    }

                    count2 = len2 - gallopLeft(*cursor1, buffer.get(), len2, len2 - 1, compare, projection);
                    if (count2 != 0) {
                        dest    -= count2;
                        cursor2 -= count2;
                        len2    -= count2;
                        detail::move(cursor2 + 1, cursor2 + (1 + count2), dest + 1);
                        if (len2 <= 1) {
                            break_outer = true;
                            break;
                        }
                    }
                    *dest = iter_move(cursor1);
                    --dest;
                    --cursor1;
                    if (--len1 == 0) {
                        break_outer = true;
                        break;
                    }

                    minGallop--;
                } while ( (count1 >= min_gallop) | (count2 >= min_gallop) );
                if (break_outer) {
                    break;
                }

                if (minGallop < 0) {
                    minGallop = 0;
                }
                minGallop += 2;
            } // end of "outer" loop

            minGallop_ = std::min(minGallop, 1);

            if (len2 == 1) {
                assert( len1 > 0 );
                dest    -= len1;
                detail::move_backward(cursor1 + (1 - len1), cursor1 + 1, dest + (1 + len1));
                *dest = iter_move(cursor2);
            }
            else {
                assert( len2 != 0 && "comparison function violates its general contract");
                assert( len1 == 0 );
                assert( len2 > 1 );
                detail::move(buffer.get(), buffer.get() + len2, dest - (len2 - 1));
            }
        }

        // the only interface is the friend timsort() function
        template<typename IterT, typename LessT, typename Proj>
        friend void timsort(IterT, IterT, LessT, Proj);
    };

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto timsort(RandomAccessIterator const first, RandomAccessIterator const last,
                 Compare compare, Projection projection)
        -> void
    {
        TimSort<RandomAccessIterator, Compare, Projection>::sort(
            std::move(first), std::move(last),
            std::move(compare), std::move(projection));
    }
}}

#endif // CPPSORT_DETAIL_TIMSORT_H_
