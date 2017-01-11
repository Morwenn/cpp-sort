/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2017 Morwenn
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
#ifndef CPPSORT_DETAIL_ASSUME_H_
#define CPPSORT_DETAIL_ASSUME_H_

// Assumptions may help the compiler to remove unnecessary code
// Some parts of the library may be significantly slower if this
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

#endif // CPPSORT_DETAIL_ASSUME_H_
