/*
 * Copyright (c) 2018-2022 Morwenn
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

namespace cppsort::detail
{
    constexpr auto uncaught_exceptions() noexcept
        -> int
    {
        if (std::is_constant_evaluated()) {
            // Throwing an exception at compile time terminates the
            // compilation process, so there can never be an exception
            // in flight in a constant-evaluated expressions
            return 0;
        } else {
            return std::uncaught_exceptions();
        }
    }

    template<typename EF>
    struct scope_exit
    {
        private:

            EF exit_function;
            bool execute_on_destruction = true;

        public:

            template<typename EFP>
            constexpr explicit scope_exit(EFP&& func)
                noexcept(std::is_nothrow_constructible_v<EF, EFP> ||
                         std::is_nothrow_constructible_v<EF, EFP&>):
                exit_function(std::forward<EFP>(func))
            {}

            constexpr scope_exit(scope_exit&& rhs)
                noexcept(std::is_nothrow_move_constructible_v<EF> ||
                         std::is_nothrow_copy_constructible_v<EF>):
                exit_function(std::forward<EF>(rhs.exit_function))
            {}

            constexpr ~scope_exit()
                noexcept(noexcept(exit_function()))
            {
                if (execute_on_destruction) {
                    exit_function();
                }
            }

            constexpr auto activate() noexcept
                -> void
            {
                execute_on_destruction = true;
            }

            constexpr auto deactivate() noexcept
                -> void
            {
                execute_on_destruction = false;
            }

            scope_exit(const scope_exit&) = delete;
            scope_exit& operator=(const scope_exit&) = delete;
            scope_exit& operator=(scope_exit&&) = delete;
    };

    template<typename EF>
    constexpr auto make_scope_exit(EF&& function)
        -> scope_exit<EF>
    {
        return scope_exit<EF>(std::forward<EF>(function));
    }

    template<typename EF>
    struct scope_success
    {
        private:

            EF exit_function;
            bool execute_on_destruction = true;
            int uncaught_on_creation = uncaught_exceptions();

        public:

            template<typename EFP>
            constexpr explicit scope_success(EFP&& func)
                noexcept(std::is_nothrow_constructible_v<EF, EFP> ||
                         std::is_nothrow_constructible_v<EF, EFP&>):
                exit_function(std::forward<EFP>(func))
            {}

            constexpr scope_success(scope_success&& rhs)
                noexcept(std::is_nothrow_move_constructible_v<EF> ||
                         std::is_nothrow_copy_constructible_v<EF>):
                exit_function(std::forward<EF>(rhs.exit_function))
            {}

            constexpr ~scope_success()
                noexcept(noexcept(exit_function()))
            {
                if (execute_on_destruction && uncaught_exceptions() <= uncaught_on_creation) {
                    exit_function();
                }
            }

            constexpr auto activate() noexcept
                -> void
            {
                execute_on_destruction = true;
            }

            constexpr auto deactivate() noexcept
                -> void
            {
                execute_on_destruction = false;
            }

            scope_success(const scope_success&) = delete;
            scope_success& operator=(const scope_success&) = delete;
            scope_success& operator=(scope_success&&) = delete;
    };

    template<typename EF>
    constexpr auto make_scope_success(EF&& function)
        -> scope_success<EF>
    {
        return scope_success<EF>(std::forward<EF>(function));
    }
}

#endif // CPPSORT_DETAIL_SCOPE_EXIT_H_
