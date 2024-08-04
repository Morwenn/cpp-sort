/*
 * Copyright (c) 2016-2024 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_CONFIG_H_
#define CPPSORT_DETAIL_CONFIG_H_

////////////////////////////////////////////////////////////
// General: assertions

// Ensure that enabling audits always enables assertions
#if defined(CPPSORT_ENABLE_AUDITS)
#   define CPPSORT_ENABLE_ASSERTIONS
#endif

#if defined(CPPSORT_ENABLE_ASSERTIONS) || defined(CPPSORT_ENABLE_AUDITS)
#   include <cassert>
#   if defined(CPPSORT_USE_LIBASSERT)
#       include <assert.hpp>
#   endif
#endif

////////////////////////////////////////////////////////////
// CPPSORT_ASSERT

// We want a slightly finer-grain control over assertions
// than just relying on NDEBUG, so assertions have to be
// explicitly enabled in cpp-sort

#if !defined(NDEBUG) && defined(CPPSORT_ENABLE_ASSERTIONS)
#   if !defined(CPPSORT_ASSERT)
#       if defined(CPPSORT_USE_LIBASSERT)
#           define CPPSORT_ASSERT(...) ASSERT(__VA_ARGS__)
#       else
#           define CPPSORT_ARG2(_0, _1, _2, ...) _2
#           define CPPSORT_NARG2(...) CPPSORT_ARG2(__VA_ARGS__, 2, 1, 0)
#           define CPPSORT_ONE_OR_TWO_ARGS_1(condition) assert(condition)
#           define CPPSORT_ONE_OR_TWO_ARGS_2(condition, message) assert(condition && message)
#           define CPPSORT_ONE_OR_TWO_ARGS_N(N, ...) CPPSORT_ONE_OR_TWO_ARGS_##N(__VA_ARGS__)
#           define CPPSORT_ONE_OR_TWO_ARGS(N, ...) CPPSORT_ONE_OR_TWO_ARGS_N(N, __VA_ARGS__)
#           define CPPSORT_ASSERT(...) CPPSORT_ONE_OR_TWO_ARGS(CPPSORT_NARG2(__VA_ARGS__), __VA_ARGS__)
#       endif
#   endif
#else
#   define CPPSORT_ASSERT(...) ((void)0)
#endif

////////////////////////////////////////////////////////////
// CPPSORT_AUDIT

// Some debug checks might be way too expensive for most
// scenarios, but still of great help when debugging tough
// problems, hence this audit feature

#if !defined(NDEBUG) && defined(CPPSORT_ENABLE_AUDITS)
#   ifndef CPPSORT_AUDIT
#       define CPPSORT_AUDIT(...) CPPSORT_ASSERT(__VA_ARGS__)
#   endif
#else
#   define CPPSORT_AUDIT(...) ((void)0)
#endif

////////////////////////////////////////////////////////////
// CPPSORT_ASSUME

// Assumptions may help the compiler to remove unnecessary code;
// some parts of the library may be significantly slower if this
// assumption mechanism isn't supported. In audit mode, we want
// to turn them into assertions to make sure the assumptions are
// actually correct.

#if defined(CPPSORT_ENABLE_AUDITS)
#   define CPPSORT_ASSUME(...) CPPSORT_ASSERT(__VA_ARGS__)
#else
#   define CPPSORT_ASSUME(...) [[assume(__VA_ARGS__)]]
#endif

////////////////////////////////////////////////////////////
// CPPSORT_UNREACHABLE

// Mostly useful to silence compiler warnings in the default
// clause of a switch when we know the default can never be
// reached

#if defined(CPPSORT_ENABLE_AUDITS)
#   define CPPSORT_UNREACHABLE CPPSORT_ASSERT(false, "unreachable");
#else
#   include <utility>
#   define CPPSORT_UNREACHABLE ::std::unreachable()
#endif

////////////////////////////////////////////////////////////
// CPPSORT_DEPRECATED

// [[deprecated]] is available since C++14, hence why this
// macro is not in attributes.h: we hide it behind a macro
// to make sure that it can be silenced

#ifndef CPPSORT_DEPRECATED
#   ifndef CPPSORT_DISABLE_DEPRECATION_WARNINGS
#       define CPPSORT_DEPRECATED(message) [[deprecated(message)]]
#   else
#       define CPPSORT_DEPRECATED(message)
#   endif
#endif

#endif // CPPSORT_DETAIL_CONFIG_H_

////////////////////////////////////////////////////////////
// CPPSORT_INT128_REQUIRES_LIBRARY_SUPPORT

// The compiler front-end sometimes supports __int128_t and
// __uint128_t but without giving it proper standard library
// support. In such a case, cpp-sort components need to
// provide explicit support for those types. It affects the
// following configurations:
// - Clang front-end with Microsoft STL
// - libc++ with GCC in strict conformance mode

#ifndef CPPSORT_INT128_REQUIRES_LIBRARY_SUPPORT
#   if defined(__SIZEOF_INT128__) && !defined(_LIBCPP_VERSION)
#       define CPPSORT_INT128_REQUIRES_LIBRARY_SUPPORT 1
#   else
#       define CPPSORT_INT128_REQUIRES_LIBRARY_SUPPORT 0
#   endif
#endif
