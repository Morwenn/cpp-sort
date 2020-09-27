/*
 * Copyright (c) 2015 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_FRONT_INSERT_H_
#define CPPSORT_DETAIL_FRONT_INSERT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <utility>

namespace cppsort
{
namespace detail
{
    //
    // Family of fixed-size functions which will try to insert
    // *first in the rest of the collection, assuming that the
    // following elements are sorted with compare
    //
    // When there are enough elements, binary search is used to
    // locate the position where to insert *first
    //

    template<std::size_t N>
    struct front_inserter_n;

    template<
        std::size_t N,
        typename RandomAccessIterator,
        typename Compare,
        typename Projection
    >
    auto front_insert(RandomAccessIterator first, Compare compare, Projection projection)
        -> decltype(auto)
    {
        return front_inserter_n<N>{}(std::move(first), std::move(compare), std::move(projection));
    }
}}

// Specializations of low_moves_sorter_n for some values of N
#include "front_inserter/inserter0.h"
#include "front_inserter/inserter1.h"
#include "front_inserter/inserter2.h"
#include "front_inserter/inserter3.h"
#include "front_inserter/inserter4.h"
#include "front_inserter/inserter5.h"
#include "front_inserter/inserter6.h"
#include "front_inserter/inserter7.h"
#include "front_inserter/inserter8.h"
#include "front_inserter/inserter9.h"
#include "front_inserter/inserter10.h"
#include "front_inserter/inserter11.h"

#endif // CPPSORT_DETAIL_FRONT_INSERT_H_
