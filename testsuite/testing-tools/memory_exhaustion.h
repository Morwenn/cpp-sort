/*
 * Copyright (c) 2019 Morwenn
 * SPDX-License-Identifier: MIT
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
