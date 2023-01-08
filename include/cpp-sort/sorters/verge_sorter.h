/*
 * Copyright (c) 2015-2023 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_SORTERS_VERGE_SORTER_H_
#define CPPSORT_SORTERS_VERGE_SORTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cpp-sort/adapters/hybrid_adapter.h>
#include <cpp-sort/adapters/stable_adapter.h>
#include <cpp-sort/adapters/verge_adapter.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorters/pdq_sorter.h>
#include <cpp-sort/sorters/quick_merge_sorter.h>
#include <cpp-sort/utility/static_const.h>

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Sorters

    struct verge_sorter:
        sorter_facade<
            detail::verge_adapter_impl<
                hybrid_adapter<
                    pdq_sorter,
                    quick_merge_sorter
                >,
                false
            >
        >
    {
        verge_sorter() = default;
    };

    template<>
    struct stable_adapter<verge_sorter>:
        sorter_facade<
            detail::verge_adapter_impl<
                hybrid_adapter<
                    pdq_sorter,
                    quick_merge_sorter
                >,
                true
            >
        >
    {
        stable_adapter() = default;

        constexpr explicit stable_adapter(verge_sorter):
            sorter_facade<
                detail::verge_adapter_impl<
                    hybrid_adapter<
                        pdq_sorter,
                        quick_merge_sorter
                    >,
                    true
                >
            >()
        {}
    };

    ////////////////////////////////////////////////////////////
    // Sort function

    namespace
    {
        constexpr auto&& verge_sort
            = utility::static_const<verge_sorter>::value;
    }
}

#endif // CPPSORT_SORTERS_VERGE_SORTER_H_
