/*
 * Copyright (c) 2018-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_SCOPE_EXIT_H_
#define CPPSORT_DETAIL_SCOPE_EXIT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <exception>
#include <type_traits>
#include <utility>

namespace cppsort
{
namespace detail
{
    template<typename EF>
    struct scope_exit
    {
        private:

            EF exit_function;
            bool execute_on_destruction = true;

        public:

            template<typename EFP>
            explicit scope_exit(EFP&& func)
                noexcept(std::is_nothrow_constructible<EF, EFP>::value ||
                         std::is_nothrow_constructible<EF, EFP&>::value):
                exit_function(std::forward<EFP>(func))
            {}

            scope_exit(scope_exit&& rhs)
                noexcept(std::is_nothrow_move_constructible<EF>::value ||
                         std::is_nothrow_copy_constructible<EF>::value):
                exit_function(std::forward<EF>(rhs.exit_function))
            {}

            ~scope_exit()
                noexcept(noexcept(exit_function()))
            {
                if (execute_on_destruction) {
                    exit_function();
                }
            }

            auto activate() noexcept
                -> void
            {
                execute_on_destruction = true;
            }

            auto deactivate() noexcept
                -> void
            {
                execute_on_destruction = false;
            }

            scope_exit(const scope_exit&) = delete;
            scope_exit& operator=(const scope_exit&) = delete;
            scope_exit& operator=(scope_exit&&) = delete;
    };

    template<typename EF>
    auto make_scope_exit(EF&& function)
        -> scope_exit<EF>
    {
        return scope_exit<EF>(std::forward<EF>(function));
    }

#ifdef __cpp_lib_uncaught_exceptions
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

            auto activate() noexcept
                -> void
            {
                execute_on_destruction = true;
            }

            auto deactivate() noexcept
                -> void
            {
                execute_on_destruction = false;
            }

            scope_success(const scope_success&) = delete;
            scope_success& operator=(const scope_success&) = delete;
            scope_success& operator=(scope_success&&) = delete;
    };

    template<typename EF>
    auto make_scope_success(EF&& function)
        -> scope_success<EF>
    {
        return scope_success<EF>(std::forward<EF>(function));
    }
#endif // __cpp_lib_uncaught_exceptions
}}

#endif // CPPSORT_DETAIL_SCOPE_EXIT_H_
