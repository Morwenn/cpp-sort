/*
 * Copyright (c) 2017-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_MEMCPY_CAST_H_
#define CPPSORT_DETAIL_MEMCPY_CAST_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstring>
#include <memory>
#include <type_traits>

namespace cppsort
{
namespace detail
{
    template<typename To, typename From>
    auto memcpy_cast(const From& value)
        -> To
    {
        static_assert(std::is_trivially_copyable_v<From>, "");
        static_assert(std::is_trivially_copyable_v<To>, "");
        static_assert(sizeof(From) == sizeof(To), "");

        To result;
        std::memcpy(std::addressof(result),
                    std::addressof(value),
                    sizeof(From));
        return result;
    }
}}

#endif // CPPSORT_DETAIL_MEMCPY_CAST_H_
