/*
 * Copyright (c) 2015-2020 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_INDIRECT_COMPARE_H_
#define CPPSORT_DETAIL_INDIRECT_COMPARE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <tuple>
#include <utility>
#include <cpp-sort/utility/as_function.h>

namespace cppsort
{
namespace detail
{
    template<typename Compare, typename Projection>
    class indirect_compare
    {
        private:

            // Pack compare and projection for EBCO
            std::tuple<Compare, Projection> data;

        public:

            indirect_compare(Compare compare, Projection projection):
                data(std::move(compare), std::move(projection))
            {}

            template<typename Iterator>
            auto operator()(Iterator lhs, Iterator rhs)
                -> bool
            {
                auto&& comp = utility::as_function(std::get<0>(data));
                auto&& proj = utility::as_function(std::get<1>(data));
                return comp(proj(*lhs), proj(*rhs));
            }
    };

    template<typename Compare, typename Projection=utility::identity>
    auto make_indirect_compare(Compare compare, Projection projection={})
        -> indirect_compare<Compare, Projection>
    {
        return { compare, projection };
    }
}}

#endif // CPPSORT_DETAIL_INDIRECT_COMPARE_H_
