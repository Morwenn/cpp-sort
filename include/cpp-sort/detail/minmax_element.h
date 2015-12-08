// Range v3 library
//
//  Copyright Eric Niebler 2014
//  Modified in 2015 by Morwenn for inclusion into cpp-sort
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3
//
// Implementation based on the code in libc++
//   http://http://libcxx.llvm.org/
#ifndef CPPSORT_DETAIL_MINMAX_ELEMENT_H_
#define CPPSORT_DETAIL_MINMAX_ELEMENT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/functional.h>

namespace cppsort
{
namespace detail
{
    template<
        typename ForwardIterator,
        typename Compare = std::less<>,
        typename Projection = utility::identity
    >
    auto minmax_element(ForwardIterator begin, ForwardIterator end,
                        Compare compare={}, Projection projection={})
        -> std::pair<ForwardIterator, ForwardIterator>
    {
        auto && pred = utility::as_function(compare);
        auto && proj = utility::as_function(projection);
        std::pair<ForwardIterator, ForwardIterator> result{begin, begin};
        if(begin == end || ++begin == end)
            return result;
        if(pred(proj(*begin), proj(*result.first)))
            result.first = begin;
        else
            result.second = begin;
        while(++begin != end)
        {
            ForwardIterator tmp = begin;
            if(++begin == end)
            {
                if(pred(proj(*tmp), proj(*result.first)))
                    result.first = tmp;
                else if(!pred(proj(*tmp), proj(*result.second)))
                    result.second = tmp;
                break;
            }
            else
            {
                if(pred(proj(*begin), proj(*tmp)))
                {
                    if(pred(proj(*begin), proj(*result.first)))
                        result.first = begin;
                    if(!pred(proj(*tmp), proj(*result.second)))
                        result.second = tmp;
                }
                else
                {
                    if(pred(proj(*tmp), proj(*result.first)))
                        result.first = tmp;
                    if(!pred(proj(*begin), proj(*result.second)))
                        result.second = begin;
                }
            }
        }
        return result;
    }
}}

#endif // CPPSORT_DETAIL_MINMAX_ELEMENT_H_
