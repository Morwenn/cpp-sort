/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Timothy Van Slyke
 * Modified in 2018 by Morwenn for inclusion into cpp-sort
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef CPPSORT_DETAIL_TIMSORT_COMPILER_H_
#define CPPSORT_DETAIL_TIMSORT_COMPILER_H_

# ifndef TIMSORT_NO_USE_COMPILER_INTRINSICS

#  if defined _MSC_VER
#   include "intrin.h"
#  endif

#  if defined  _MSC_VER
#   define COMPILER_UNREACHABLE_ __assume(0);
#  elif defined __GNUC__ || defined __clang__
#   define COMPILER_UNREACHABLE_ __builtin_unreachable();
#  else
#   define COMPILER_UNREACHABLE_
#  endif

#  if defined __GNUC__
#   define GCC_ASSUME__(x) do{ if(not (x)) {__builtin_unreachable();} } while(false);
#  endif

#  if defined  _MSC_VER || defined __INTEL_COMPILER
#   define COMPILER_ASSUME_(x) __assume(x);
#  elif defined __clang__
#   if __has_builtin(__builtin_assume)
#    define COMPILER_ASSUME_(x) __builtin_assume(x)
#   else
#    define COMPILER_ASSUME_(x) GCC_ASSUME__(x)
#   endif
#  elif defined __GNUC__
#   define COMPILER_ASSUME_(x) GCC_ASSUME__(x)
#  else
#   define COMPILER_ASSUME_(x)
#  endif

#  if defined __GNUC__
#   define COMPILER_LIKELY_(x) __builtin_expect(x, true)
#   define COMPILER_UNLIKELY_(x) __builtin_expect(x, false)
#  else
#   define COMPILER_LIKELY_(x)   (x)
#   define COMPILER_UNLIKELY_(x) (x)
#  endif

# else
#  define COMPILER_LIKELY_(x)   (x)
#  define COMPILER_UNLIKELY_(x) (x)
#  define COMPILER_ASSUME_(x)   (x)
#  define COMPILER_UNREACHABLE_

# endif /* TIMSORT_NO_USE_COMPILER_INTRINSICS */

#endif // CPPSORT_DETAIL_TIMSORT_COMPILER_H_
