// Range v3 library
//
//  Copyright Eric Niebler 2013-2014
//  Modified in 2015 by Morwenn for inclusion into cpp-sort
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3
//
#ifndef CPPSORT_UTILITY_AS_FUNCTION_H_
#define CPPSORT_UTILITY_AS_FUNCTION_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <type_traits>
#include <utility>
#include "../detail/static_const.h"

namespace cppsort
{
namespace utility
{
    namespace detail
    {
        struct as_function_fn
        {
        private:
            template<typename R, typename...Args>
            struct ptr_fn_
            {
            private:
                R (*pfn_)(Args...);
            public:
                ptr_fn_() = default;
                constexpr explicit ptr_fn_(R (*pfn)(Args...))
                  : pfn_(pfn)
                {}
                R operator()(Args...args) const
                {
                    return (*pfn_)(std::forward<Args>(args)...);
                }
            };
        public:
            template<typename R, typename ...Args>
            constexpr ptr_fn_<R, Args...> operator()(R (*p)(Args...)) const
            {
                return ptr_fn_<R, Args...>(p);
            }
            template<typename R, typename T>
            auto operator()(R T::* p) const -> decltype(std::mem_fn(p))
            {
                return std::mem_fn(p);
            }
            template<typename T, typename U = std::decay_t<T>>
            constexpr auto operator()(T && t) const ->
                std::enable_if_t<
                    !std::is_pointer<U>::value && !std::is_member_pointer<U>::value,
                    T
                >
            {
                return std::forward<T>(t);
            }
        };
    }

    namespace
    {
        constexpr auto&& as_function = cppsort::detail::static_const<
            detail::as_function_fn
        >::value;
    }
}}

#endif // CPPSORT_UTILITY_AS_FUNCTION_H_
