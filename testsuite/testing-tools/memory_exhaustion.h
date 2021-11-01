/*
 * Copyright (c) 2019-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_TESTSUITE_MEMORY_EXHAUSTION_H_
#define CPPSORT_TESTSUITE_MEMORY_EXHAUSTION_H_

// Class to make memory exhaustion fail in the current scope
struct scoped_memory_exhaustion
{
    scoped_memory_exhaustion() noexcept;
    ~scoped_memory_exhaustion();
};

#endif // CPPSORT_TESTSUITE_MEMORY_EXHAUSTION_H_
