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
#include <cstddef>
#include <cstdlib>
#include <new>
#include <testing-tools/memory_exhaustion.h>

//
// Replace the global new and delete for the purpose of testing
// algorithms that have fallbacks when they can't allocate
// extra memory
//

// This variable controls whether memory exhaustion should fail
thread_local bool heap_memory_exhaustion_should_fail = false;

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
