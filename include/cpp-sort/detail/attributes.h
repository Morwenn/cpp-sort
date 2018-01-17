/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Morwenn
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

#if __has_cpp_attribute(fallthrough)
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

#endif // CPPSORT_DETAIL_ATTRIBUTES_H_
