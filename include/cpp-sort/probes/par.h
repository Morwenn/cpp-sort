/*
 * Copyright (c) 2016-2025 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_PROBES_PAR_H_
#define CPPSORT_PROBES_PAR_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cpp-sort/probes/dis.h>
#include <cpp-sort/sorter_facade.h>
#include "../detail/config.h"

namespace cppsort
{
namespace probe
{
    namespace detail
    {
        struct par_impl:
            dis_impl
        {};
    }

    CPPSORT_DEPRECATED("probe::par is deprecated and will be removed in version 2.0.0, use probe::dis instead")
    inline constexpr sorter_facade<detail::par_impl> par{};
}}

#endif // CPPSORT_PROBES_PAR_H_
