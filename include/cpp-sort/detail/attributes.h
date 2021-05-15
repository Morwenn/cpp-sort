/*
 * Copyright (c) 2018-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_ATTRIBUTES_H_
#define CPPSORT_DETAIL_ATTRIBUTES_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "config.h"

//
// Some attributes aren't available in C++14 but might be
// useful to provide in later revisions of the standard, but
// even though they should be ignored compilers tend to
// generate warnings, even in C++14 mode where the warnings
// don't officially exist, so we need to fall back to older
// platform-specific equivalents when possible
//

// CPPSORT_ATTRIBUTE_FALLTHROUGH

#if __has_cpp_attribute(fallthrough) && !(defined(__clang__) && __cplusplus < 201703L)
#   define CPPSORT_ATTRIBUTE_FALLTHROUGH [[fallthrough]]
#elif defined(__clang__)
#   define CPPSORT_ATTRIBUTE_FALLTHROUGH [[clang::fallthrough]]
#elif defined(__GNUC__)
#   if __GNUC__ >= 7
#       define CPPSORT_ATTRIBUTE_FALLTHROUGH [[gnu::fallthrough]]
#   else
#       define CPPSORT_ATTRIBUTE_FALLTHROUGH (void)0
#   endif
#else
#   define CPPSORT_ATTRIBUTE_FALLTHROUGH (void)0
#endif

// CPPSORT_ATTRIBUTE_NODISCARD

#if __has_cpp_attribute(nodiscard) && !(defined(__clang__) && __cplusplus < 201703L)
#   define CPPSORT_ATTRIBUTE_NODISCARD [[nodiscard]]
#elif defined(__clang__)
#   define CPPSORT_ATTRIBUTE_NODISCARD [[clang::warn_unused_result]]
#elif defined(__GNUC__)
#   define CPPSORT_ATTRIBUTE_NODISCARD [[gnu::warn_unused_result]]
#else
#   define CPPSORT_ATTRIBUTE_NODISCARD
#endif

#endif // CPPSORT_DETAIL_ATTRIBUTES_H_
