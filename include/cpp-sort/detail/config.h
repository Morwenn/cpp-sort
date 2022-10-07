/*
 * Copyright (c) 2016-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_CONFIG_H_
#define CPPSORT_DETAIL_CONFIG_H_

////////////////////////////////////////////////////////////
// Make <version> available when possible

// config.h is what should be included to get configuration
// information, which includes standard library feature-test
// macros when available

#if __has_include(<version>)
#   include <version>
#endif

////////////////////////////////////////////////////////////
// Check for __has_* macros

#ifndef __has_include
#   define __has_include(x) 0
#endif

#ifndef __has_cpp_attribute
#   define __has_cpp_attribute(x) 0
#endif

////////////////////////////////////////////////////////////
// Check for C++17 features

// Make sure that there is a single variable before C++17,
// default to static in C++14 to avoid ODR issues

#if defined(__cpp_inline_variables)
#   define CPPSORT_INLINE_VARIABLE inline
#else
#   define CPPSORT_INLINE_VARIABLE static
#endif

////////////////////////////////////////////////////////////
// Check for C++20 features

// There is no feature-test macro for std::identity that can
// be used reliably, so we have to fall back to checking
// compiler and standard versions

#if defined(__cpp_lib_ranges)
#   define CPPSORT_STD_IDENTITY_AVAILABLE 1
#elif defined(__GNUC__)
#   if __GNUC__ > 9 && __cplusplus > 201703L
#       define CPPSORT_STD_IDENTITY_AVAILABLE 1
#   else
#       define CPPSORT_STD_IDENTITY_AVAILABLE 0
#   endif
#else
#   define CPPSORT_STD_IDENTITY_AVAILABLE 0
#endif

////////////////////////////////////////////////////////////
// CPPSORT_ASSUME

// Assumptions may help the compiler to remove unnecessary code;
// some parts of the library may be significantly slower if this
// assumption mechanism isn't supported

#if defined(__GNUC__)
#   define CPPSORT_ASSUME(expression) do { if (!(expression)) __builtin_unreachable(); } while(0)
#elif defined(__clang__)
#   define CPPSORT_ASSUME(expression) __builtin_assume(expression)
#elif defined(_MSC_VER)
#   define CPPSORT_ASSUME(expression) __assume(expression)
#else
#   define CPPSORT_ASSUME(cond)
#endif

////////////////////////////////////////////////////////////
// CPPSORT_UNREACHABLE

// Mostly useful to silence compiler warnings in the default
// clause of a switch when we know the default can never be
// reached

#if defined(__GNUC__) || defined(__clang__)
#   define CPPSORT_UNREACHABLE __builtin_unreachable()
#elif defined(_MSC_VER)
#   define CPPSORT_UNREACHABLE __assume(false)
#else
#   define CPPSORT_UNREACHABLE
#endif

////////////////////////////////////////////////////////////
// CPPSORT_ASSERT

// We want a slightly finer-grain control over assertions
// than just relying on NDEBUG, so assertions have to be
// explicitly enabled in cpp-sort

#ifndef CPPSORT_ASSERT
#   ifdef CPPSORT_ENABLE_ASSERTIONS
#       include <cassert>
#       define CPPSORT_ASSERT(...) assert((__VA_ARGS__))
#   else
#       define CPPSORT_ASSERT(...)
#   endif
#endif

////////////////////////////////////////////////////////////
// CPPSORT_AUDIT

// Some debug checks might be way too expensive for most
// scenarios, but still of great help when debugging tough
// problems, hence this audit feature

#ifndef CPPSORT_AUDIT
#   ifdef CPPSORT_ENABLE_AUDITS
#       include <cassert>
#       define CPPSORT_AUDIT(...) assert((__VA_ARGS__))
#   else
#       define CPPSORT_AUDIT(...)
#   endif
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
