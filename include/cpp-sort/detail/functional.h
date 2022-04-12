/*
 * Copyright (c) 2021-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_FUNCTIONAL_H_
#define CPPSORT_DETAIL_FUNCTIONAL_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <type_traits>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/functional.h>

namespace cppsort
{
namespace detail
{
    ////////////////////////////////////////////////////////////
    // indirect

    template<typename Projection>
    class indirect_t:
        utility::projection_base
    {
        private:

            Projection projection;

        public:

            indirect_t() = delete;

            explicit indirect_t(Projection projection):
                projection(std::move(projection))
            {}

            template<typename T>
            auto operator()(T&& indirect_value)
                -> decltype(utility::as_function(projection)(*indirect_value))
            {
                auto&& proj = utility::as_function(projection);
                return proj(*indirect_value);
            }

            template<typename T>
            auto operator()(T&& indirect_value) const
                -> decltype(utility::as_function(projection)(*indirect_value))
            {
                auto&& proj = utility::as_function(projection);
                return proj(*indirect_value);
            }
    };

    template<typename Projection>
    auto indirect(Projection&& proj)
        -> indirect_t<std::decay_t<Projection>>
    {
        return indirect_t<std::decay_t<Projection>>(std::forward<Projection>(proj));
    }
}}

#endif // CPPSORT_DETAIL_FUNCTIONAL_H_
