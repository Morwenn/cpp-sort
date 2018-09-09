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
#ifndef CPPSORT_DETAIL_SCOPE_EXIT_H_
#define CPPSORT_DETAIL_SCOPE_EXIT_H_

#ifdef __cpp_lib_uncaught_exceptions

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <exception>
#include <type_traits>

namespace cppsort
{
namespace detail
{
    template<typename EF>
    struct scope_success
    {
        private:

            EF exit_function;
            bool execute_on_destruction = true;
            int uncaught_on_creation = std::uncaught_exceptions();

        public:

            template<typename EFP>
            explicit scope_success(EFP&& func)
                noexcept(std::is_nothrow_constructible<EF, EFP>::value ||
                         std::is_nothrow_constructible<EF, EFP&>::value):
                exit_function(std::forward<EFP>(func))
            {}

            scope_success(scope_success&& rhs)
                noexcept(std::is_nothrow_move_constructible<EF>::value ||
                         std::is_nothrow_copy_constructible<EF>::value):
                exit_function(std::forward<EF>(rhs.exit_function))
            {}

            ~scope_success()
                noexcept(noexcept(exit_function()))
            {
                if (execute_on_destruction && std::uncaught_exceptions() <= uncaught_on_creation) {
                    exit_function();
                }
            }

            auto release() noexcept
                -> void
            {
                execute_on_destruction = false;
            }

            scope_success(const scope_success&) = delete;
            scope_success& operator=(const scope_success&) = delete;
            scope_success& operator=(scope_success&&) = delete;
    };

    template<typename EF>
    scope_success(EF)
        -> scope_success<EF>;
}}

#endif // __cpp_lib_uncaught_exceptions

#endif // CPPSORT_DETAIL_SCOPE_EXIT_H_
