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
#include "../detail/config.h"

namespace cppsort
{
    struct verge_sorter;

    namespace deprecated
    {
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
    }

    ////////////////////////////////////////////////////////////
    // Stable sorter

    // Declare the specialization first to avoid deprecation
    // warning at definition time
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

        constexpr explicit stable_adapter(const verge_sorter&):
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

    template<>
    struct stable_adapter<deprecated::verge_sorter>:
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

        constexpr explicit stable_adapter(deprecated::verge_sorter):
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
    // Sorters

    struct CPPSORT_DEPRECATED("verge_sorter is deprecated and will be removed in version 2.0.0, use verge_adapter instead")
    verge_sorter:
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

    ////////////////////////////////////////////////////////////
    // Sort function

    namespace
    {
        CPPSORT_DEPRECATED("verge_sort is deprecated and will be removed in version 2.0.0, use verge_adapter instead")
        constexpr auto&& verge_sort
            = utility::static_const<deprecated::verge_sorter>::value;
    }
}

#endif // CPPSORT_SORTERS_VERGE_SORTER_H_
