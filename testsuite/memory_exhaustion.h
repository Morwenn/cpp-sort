/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Morwenn
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
#ifndef CPPSORT_TESTSUITE_MEMORY_EXHAUSTION_H_
#define CPPSORT_TESTSUITE_MEMORY_EXHAUSTION_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////

// This variable is defined in new_delete.h
extern thread_local bool heap_memory_exhaustion_should_fail;

// Class to make memory exhaustion fail in the current scope
struct scoped_memory_exhaustion
{
    scoped_memory_exhaustion() noexcept
    {
        heap_memory_exhaustion_should_fail = true;
    }

    ~scoped_memory_exhaustion()
    {
        heap_memory_exhaustion_should_fail = false;
    }
};

#endif // CPPSORT_TESTSUITE_MEMORY_EXHAUSTION_H_
