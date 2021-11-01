/*
 * Copyright (c) 2019-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <cstddef>
#include <cstdlib>
#include <new>
#include <testing-tools/memory_exhaustion.h>

////////////////////////////////////////////////////////////
// Variable to control whether memory exhaustion should fail

static thread_local bool heap_memory_exhaustion_should_fail = false;

////////////////////////////////////////////////////////////
// scoped_memory_exhaustion (scope guard)

scoped_memory_exhaustion::scoped_memory_exhaustion() noexcept
{
    heap_memory_exhaustion_should_fail = true;
}

scoped_memory_exhaustion::~scoped_memory_exhaustion()
{
    heap_memory_exhaustion_should_fail = false;
}

////////////////////////////////////////////////////////////
// Replace the global new and delete functions for the
// purpose of testing that some algorithms still work when
// heap memory allocations fail

auto operator new(std::size_t size)
    -> void*
{
    if (heap_memory_exhaustion_should_fail) {
        throw std::bad_alloc();
    }

    if (size == 0) {
        size = 1;
    }

    void* ptr = std::malloc(size);
    while (not ptr) {
        std::new_handler handler = std::get_new_handler();
        if (not handler) {
            throw std::bad_alloc();
        }
        handler();
        ptr = std::malloc(size);
    }
    return ptr;
}

auto operator new(std::size_t size, const std::nothrow_t&) noexcept
    -> void*
{
    if (heap_memory_exhaustion_should_fail) {
        return nullptr;
    }

    void* ptr = nullptr;
    try {
        ptr = ::operator new(size);
    }
    catch (...) {}
    return ptr;
}

auto operator new[](std::size_t size)
    -> void*
{
    return ::operator new(size);
}

auto operator new[](std::size_t size, const std::nothrow_t&) noexcept
    -> void*
{
    if (heap_memory_exhaustion_should_fail) {
        return nullptr;
    }

    void* ptr = nullptr;
    try {
        ptr = ::operator new[](size);
    }
    catch (...) {}
    return ptr;
}

auto operator delete(void* ptr) noexcept
    -> void
{
    std::free(ptr);
}

auto operator delete(void* ptr, const std::nothrow_t&) noexcept
    -> void
{
    ::operator delete(ptr);
}

auto operator delete(void* ptr, std::size_t) noexcept
    -> void
{
    ::operator delete(ptr);
}

auto operator delete[](void* ptr) noexcept
    -> void
{
    ::operator delete(ptr);
}

auto operator delete[](void* ptr, const std::nothrow_t&) noexcept
    -> void
{
    ::operator delete[](ptr);
}

auto operator delete[](void* ptr, std::size_t) noexcept
    -> void
{
    ::operator delete[](ptr);
}
