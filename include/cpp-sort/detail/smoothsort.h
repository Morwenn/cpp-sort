/*
 * Copyright (c) 2015-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */

/*
 * File: Smoothsort.hh
 * Author: Keith Schwarz (htiek@cs.stanford.edu)
 *
 * An implementation of Dijkstra's Smoothsort algorithm, a modification of
 * heapsort that runs in O(n lg n) in the worst case, but O(n) if the data
 * are already sorted.  For more information about how this algorithm works
 * and some of the details necessary for its proper operation, please see
 *
 *              http://www.keithschwarz.com/smoothsort/
 *
 * This implementation is designed to work on a 64-bit machine.
 */
#ifndef CPPSORT_DETAIL_SMOOTHSORT_H_
#define CPPSORT_DETAIL_SMOOTHSORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <bitset>
#include <cstddef>
#include <cstdint>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/iter_move.h>
#include "config.h"

namespace cppsort
{
namespace detail
{
    namespace smoothsort_detail {
      /* A constant containing the number of Leonardo numbers that can fit into
       * 64 bits.
       */
      CPPSORT_INLINE_VARIABLE constexpr std::size_t kNumLeonardoNumbers = 92;

      /* A list of all the Leonardo numbers below 2^64, precomputed for
       * efficiency.
       */
      CPPSORT_INLINE_VARIABLE constexpr std::uint_fast64_t kLeonardoNumbers[kNumLeonardoNumbers] = {
        1u, 1u, 3u, 5u, 9u, 15u, 25u, 41u, 67u, 109u, 177u, 287u, 465u, 753u,
        1219u, 1973u, 3193u, 5167u, 8361u, 13529u, 21891u, 35421u, 57313u,
        92735u, 150049u, 242785u, 392835u, 635621u, 1028457u, 1664079u,
        2692537u, 4356617u, 7049155u, 11405773u, 18454929u, 29860703u,
        48315633u, 78176337u, 126491971u, 204668309u, 331160281u, 535828591u,
        866988873u, 1402817465u, 2269806339u, 3672623805u, 5942430145u,
        9615053951u, 15557484097u, 25172538049u, 40730022147u, 65902560197u,
        106632582345u, 172535142543u, 279167724889u, 451702867433u,
        730870592323u, 1182573459757u, 1913444052081u, 3096017511839u,
        5009461563921u, 8105479075761u, 13114940639683u, 21220419715445u,
        34335360355129u, 55555780070575u, 89891140425705u, 145446920496281u,
        235338060921987u, 380784981418269u, 616123042340257u, 996908023758527u,
        1613031066098785u, 2609939089857313u, 4222970155956099u, 6832909245813413u,
        11055879401769513u, 17888788647582927u, 28944668049352441u,
        46833456696935369u, 75778124746287811u, 122611581443223181u,
        198389706189510993u, 321001287632734175u, 519390993822245169u,
        840392281454979345u, 1359783275277224515u, 2200175556732203861u,
        3559958832009428377u, 5760134388741632239u, 9320093220751060617u,
        15080227609492692857u
      };

      /* A structure containing a bitvector encoding of the trees in a Leonardo
       * heap.  The representation is as a bitvector shifted down so that its
       * first digit is a one, along with the amount that it was shifted.
       */
      struct HeapShape {
        /* A bitvector capable of holding all the Leonardo numbers. */
        std::bitset<kNumLeonardoNumbers> trees;

        /* The shift amount, which is also the size of the smallest tree. */
        std::size_t smallestTreeSize;
      };

      /*
       * Function: RandomIterator SecondChild(RandomIterator root)
       * ---------------------------------------------------------------------
       * Given an iterator to the root of Leonardo heap, returns an iterator
       * to the root of that tree's second child.  It's assumed that the heap
       * is well-formed and that size > 1.
       */
      template<typename RandomIterator>
      auto SecondChild(RandomIterator root)
          -> RandomIterator
      {
        /* The second child root is always one step before the root. */
        return root - 1;
      }

      /*
       * Function: RandomIterator FirstChild(RandomIterator root, std::size_t size)
       * ---------------------------------------------------------------------
       * Given an iterator to the root of Leonardo heap, returns an iterator
       * to the root of that tree's first child.  It's assumed that the heap
       * is well-formed and that size > 1.
       */
      template<typename RandomIterator>
      auto FirstChild(RandomIterator root, std::size_t size)
          -> RandomIterator
      {
        /* Go to the second child, then step backwards L(size - 2) steps to
         * skip over it.
         */
        return SecondChild(root) - kLeonardoNumbers[size - 2];
      }

      /*
       * Function: RandomIterator LargerChild(RandomIterator root, std::size_t size,
       *                                      Compare compare, Projection projection);
       * --------------------------------------------------------------------
       * Given an iterator to the root of a max-heap Leonardo tree, returns
       * an iterator to its larger child.  It's assumed that the heap is
       * well-formatted and that the heap has order > 1.
       */
      template<typename RandomIterator, typename Compare, typename Projection>
      auto LargerChild(RandomIterator root, std::size_t size,
                       Compare compare, Projection projection)
          -> RandomIterator
      {
        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        /* Get pointers to the first and second child. */
        RandomIterator first  = FirstChild(root, size);
        RandomIterator second = SecondChild(root);

        /* Determine which is greater. */
        return comp(proj(*first), proj(*second)) ? second : first;
      }

      /*
       * Function: RebalanceSingleHeap(RandomIterator root, std::size_t size,
       *                               Compare compare, Projection proj);
       * --------------------------------------------------------------------
       * Given an iterator to the root of a single Leonardo tree that needs
       * rebalancing, rebalances that tree using the standard "bubble-down"
       * approach.
       */
      template<typename RandomIterator, typename Compare, typename Projection>
      auto RebalanceSingleHeap(RandomIterator root, std::size_t size,
                               Compare compare, Projection projection)
          -> void
      {
        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        /* Loop until the current node has no children, which happens when the order
         * of the tree is 0 or 1.
         */
        while (size > 1) {
          /* Get pointers to the first and second child. */
          RandomIterator first  = FirstChild(root, size);
          RandomIterator second = SecondChild(root);

          /* Determine which child is larger and remember the order of its tree. */
          RandomIterator largerChild;
          std::size_t childSize;
          if (comp(proj(*first), proj(*second))) {
            largerChild = second; // Second child is larger...
            childSize = size - 2; // ... and has order k - 2.
          } else {
            largerChild = first;  // First child is larger...
            childSize = size - 1; // ... and has order k - 1.
          }

          /* If the root is bigger than this child, we're done. */
          if (not comp(proj(*root), proj(*largerChild)))
            return;

          /* Otherwise, swap down and update our order. */
          using utility::iter_swap;
          iter_swap(root, largerChild);
          root = largerChild;
          size = childSize;
        }
      }

      /*
       * Function: LeonardoHeapRectify(RandomIterator begin, RandomIterator end,
       *                               HeapShape shape, Compare compare,
       *                               Projection projection);
       * ---------------------------------------------------------------------
       * Given an implicit Leonardo heap spanning [begin, end) that has just
       * had an element inserted into it at the very end, along with the
       * size list for that heap, rectifies the heap structure by shuffling
       * the new root down to the proper position and rebalancing the target
       * heap.
       */
      template<typename RandomIterator, typename Compare, typename Projection>
      auto LeonardoHeapRectify(RandomIterator begin, RandomIterator end,
                               HeapShape shape, Compare compare,
                               Projection projection)
          -> void
      {
        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        /* Back up the end iterator one step to get to the root of the rightmost
         * heap.
         */
        RandomIterator itr = end - 1;

        /* Keep track of the size of the last heap size that we visited.  We need
         * this so that once we've positioned the new node atop the correct heap
         * we remember how large it is.
         */
        std::size_t lastHeapSize;

        /* Starting at the last heap and working backward, check whether we need
         * to swap the root of the current heap with the previous root.
         */
        while (true) {
          /* Cache the size of the heap we're currently on top of. */
          lastHeapSize = shape.smallestTreeSize;

          /* If this is the very first heap in the tree, we're done. */
          if (std::size_t(itr - begin) == kLeonardoNumbers[lastHeapSize] - 1)
            break;

          /* We want to swap the previous root with this one if it's strictly
           * greater than both the root of this tree and both its children.
           * In order to avoid weird edge cases when the current heap has
           * size zero or size one, we'll compute what value will be compared
           * against.
           */
          RandomIterator toCompare = itr;

          /* If we aren't an order-0 or order-1 tree, we have two children, and
           * need to check which of the three values is largest.
           */
          if (shape.smallestTreeSize > 1) {
            /* Get the largest child and see if we need to change what we're
             * comparing against.
             */
            RandomIterator largeChild = LargerChild(itr, shape.smallestTreeSize,
                                                    compare, projection);

            /* Update what element is being compared against. */
            if (comp(proj(*toCompare), proj(*largeChild)))
              toCompare = largeChild;
          }

          /* Get a pointer to the root of the second heap by backing up the size
           * of this heap.
           */
          RandomIterator priorHeap = itr - kLeonardoNumbers[lastHeapSize];

          /* If we ran out of trees or the new tree root is less than the element
           * we're comparing, we now have the new node at the top of the correct
           * heap.
           */
          if (not comp(proj(*toCompare), proj(*priorHeap)))
            break;

          /* Otherwise, do the swap and adjust our location. */
          using utility::iter_swap;
          iter_swap(itr, priorHeap);
          itr = priorHeap;

          /* Scan down until we find the heap before this one.  We do this by
           * continously shifting down the tree bitvector and bumping up the size
           * of the smallest tree until we hit a new tree.
           */
          do {
            shape.trees >>= 1;
            ++shape.smallestTreeSize;
          } while (!shape.trees[0]);
        }

        /* Finally, rebalance the current heap. */
        RebalanceSingleHeap(itr, lastHeapSize, std::move(compare), std::move(projection));
      }

      /*
       * Function: LeonardoHeapAdd(RandomIterator begin, RandomIterator end,
       *                           RandomIterator heapEnd, HeapShape& shape,
       *                           Compare compare, Projection projection);
       * ----------------------------------------------------------------------
       * Given an implicit Leonardo heap spanning [begin, end) in a range spanned
       * by [begin, heapEnd], along with the shape, a Compare and a projection,
       * increases the size of that heap by one by inserting the element at *end.
       */
      template<typename RandomIterator, typename Compare, typename Projection>
      auto LeonardoHeapAdd(RandomIterator begin, RandomIterator end,
                           RandomIterator heapEnd, HeapShape& shape,
                           Compare compare, Projection projection)
          -> void
      {
        /* There are three cases to consider, which are analogous to the cases
         * in the proof that it is possible to partition the input into heaps
         * of decreasing size:
         *
         * Case 0: If there are no elements in the heap, add a tree of order 1.
         * Case 1: If the last two heaps have sizes that differ by one, we
         *         add the new element by merging the last two heaps.
         * Case 2: Otherwise, if the last heap has Leonardo number 1, add
         *         a singleton heap of Leonardo number 0.
         * Case 3: Otherwise, add a singleton heap of Leonardo number 1.
         */

        /* Case 0 represented by the first bit being a zero; it should always be
         * one during normal operation.
         */
        if (!shape.trees[0]) {
          shape.trees[0] = true;
          shape.smallestTreeSize = 1;
        }
        /* Case 1 would be represented by the last two bits of the bitvector both
         * being set.
         */
        else if (shape.trees[1] && shape.trees[0]) {
          /* First, remove those two trees by shifting them off the bitvector. */
          shape.trees >>= 2;

          /* Set the last bit of the bitvector; we just added a tree of this
           * size.
           */
          shape.trees[0] = true;

          /* Finally, increase the size of the smallest tree by two, since the new
           * Leonardo tree has order one greater than both of them.
           */
          shape.smallestTreeSize += 2;
        }
        /* Case two is represented by the size of the smallest tree being 1. */
        else if (shape.smallestTreeSize == 1) {
          /* Shift the bits up one spot so that we have room for the zero bit. */
          shape.trees <<= 1;
          shape.smallestTreeSize = 0;

          /* Set the bit. */
          shape.trees[0] = true;
        }
        /* Case three is everything else. */
        else {
          /* We currently have a forest encoded with a format that looks like
           * (W, n) for bitstring W and exponent n.  We want to convert this to
           * (W00...01, 1) by shifting up n - 1 spaces, then setting the last bit.
           */
          shape.trees <<= shape.smallestTreeSize - 1;
          shape.trees[0] = true;

          /* Set the smallest tree size to one, since that is the new smallest
           * tree size.
           */
          shape.smallestTreeSize = 1;
        }

        /* At this point, we've set up a new tree.  We need to see if this tree is
         * at the final size it's going to take.  If so, we'll do a full rectify
         * on it.  Otherwise, all we need to do is maintain the heap property.
         */
        bool isLast = false;
        switch (shape.smallestTreeSize) {
          /* If this last heap has order 0, then it's in its final position only
           * if it's the very last element of the array.
           */
        case 0:
          if (end + 1 == heapEnd)
            isLast = true;
          break;

          /* If this last heap has order 1, then it's in its final position if
           * it's the last element, or it's the penultimate element and it's not
           * about to be merged.  For simplicity
           */
        case 1:
          if (end + 1 == heapEnd || (end + 2 == heapEnd && !shape.trees[1]))
            isLast = true;
          break;

          /* Otherwise, this heap is in its final position if there isn't enough
           * room for the next Leonardo number and one extra element.
           */
        default:
          if (std::size_t(heapEnd - (end + 1)) < kLeonardoNumbers[shape.smallestTreeSize - 1] + 1)
            isLast = true;
          break;
        }

        /* If this isn't a final heap, then just rebalance the current heap. */
        if (!isLast)
          RebalanceSingleHeap(end, shape.smallestTreeSize,
                              std::move(compare), std::move(projection));
        /* Otherwise do a full rectify to put this node in its place. */
        else
          LeonardoHeapRectify(begin, end + 1, shape,
                              std::move(compare), std::move(projection));
      }

      /*
       * Function: LeonardoHeapRemove(RandomIterator begin, RandomIterator end,
       *                              HeapShape& shape, Compare compare,
       *                              Projection projection);
       * ----------------------------------------------------------------------
       * Given an implicit Leonardo heap spanning [begin, end), along with the
       * size list, a Compare and a projection, dequeues the element at the
       * position end - 1 and rebalances the heap. Since the largest element of
       * the heap is already at end, this essentially keeps the max element in
       * its place and does a rebalance if necessary.
       */
      template<typename RandomIterator, typename Compare, typename Projection>
      auto LeonardoHeapRemove(RandomIterator begin, RandomIterator end,
                              HeapShape& shape, Compare compare, Projection projection)
          -> void
      {
        /* There are two cases to consider:
         *
         * Case 1: The last heap is of order zero or one.  In this case,
         *         removing it doesn't expose any new trees and we can just
         *         drop it from the list of trees.
         * Case 2: The last heap is of order two or greater.  In this case,
         *         we exposed two new heaps, which may require rebalancing.
         */

        /* Case 1. */
        if (shape.smallestTreeSize <= 1) {
          /* Keep scanning up the list looking for the next tree. */
          do {
            shape.trees >>= 1;
            ++shape.smallestTreeSize;
          } while (shape.trees.any() && !shape.trees[0]);
          return;
        }

        /* Break open the last heap to expose two subheaps of order
         * k - 2 and k - 1.  This works by mapping the encoding (W1, n) to the
         * encoding (W011, n - 2).
         */
        const std::size_t heapOrder = shape.smallestTreeSize;
        shape.trees[0] = false;
        shape.trees <<= 2;
        shape.trees[1] = shape.trees[0] = true;
        shape.smallestTreeSize -= 2;

        /* We now do the insertion-sort/rebalance operation on the larger exposed heap to
         * put it in its proper place, then on the smaller of the two.  But first, we need
         * to find where they are.  This can be done by just looking up the first and second
         * children of the former root, which was at end - 1.
         */
        RandomIterator leftHeap  = FirstChild(end - 1, heapOrder);
        RandomIterator rightHeap = SecondChild(end - 1);

        /* Rebalance the left heap.  For this step we'll pretend that there is
         * one fewer heap than there actually is, since we're ignoring the
         * rightmost heap.
         */
        HeapShape allButLast = shape;
        ++allButLast.smallestTreeSize;
        allButLast.trees >>= 1;

        /* We add one to the position of the left heap because the function
         * assumes an exclusive range, while leftHeap is actually an iterator
         * directly to where the root is.
         */
        LeonardoHeapRectify(begin, leftHeap + 1,  allButLast, compare, projection);
        LeonardoHeapRectify(begin, rightHeap + 1, shape, compare, projection);
      }
    }

    /* Actual smoothsort implementation. */
    template<typename RandomIterator, typename Compare, typename Projection>
    auto smoothsort(RandomIterator begin, RandomIterator end,
                    Compare compare, Projection projection)
        -> void
    {
      /* Edge case: Check that the range isn't empty or a singleton. */
      if ((end - begin) < 2) return;

      /* Construct a shape object describing the empty heap. */
      smoothsort_detail::HeapShape shape;
      shape.smallestTreeSize = 0;

      /* Convert the input into an implicit Leonardo heap. */
      for (RandomIterator itr = begin; itr != end; ++itr)
        smoothsort_detail::LeonardoHeapAdd(begin, itr, end, shape, compare, projection);

      /* Continuously dequeue from the implicit Leonardo heap until we've
       * consumed all the elements.
       */
      for (RandomIterator itr = end; itr != begin; --itr)
        smoothsort_detail::LeonardoHeapRemove(begin, itr, shape, compare, projection);
    }
}}

#endif // CPPSORT_DETAIL_SMOOTHSORT_H_
