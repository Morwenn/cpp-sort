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
#ifndef CPPSORT_ADAPTERS_OUT_OF_PLACE_ADAPTER_H_
#define CPPSORT_ADAPTERS_OUT_OF_PLACE_ADAPTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <iterator>
#include <memory>
#include <new>
#include <utility>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/iter_move.h>
#include "../detail/checkers.h"
#include "../detail/scope_exit.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Adapter

    namespace detail
    {
        template<typename Sorter, typename ForwardIterator, typename Size, typename... Args>
        auto sort_out_of_place(ForwardIterator first, Size size, Sorter sorter, Args&&... args)
            -> decltype(auto)
        {
            using utility::iter_move;
            using rvalue_reference = remove_cvref_t<rvalue_reference_t<ForwardIterator>>;

            // Copy the collection into contiguous memory buffer
            std::unique_ptr<rvalue_reference, operator_deleter> buffer(
                static_cast<rvalue_reference*>(::operator new(size * sizeof(rvalue_reference)))
            );
            destruct_n<rvalue_reference> d(0);
            std::unique_ptr<rvalue_reference, destruct_n<rvalue_reference>&> h2(buffer.get(), d);

            auto it = first;
            auto ptr = buffer.get();
            for (Size i = 0 ; i < size ; ++i) {
                ::new(ptr) rvalue_reference(iter_move(it));
                ++it;
                ++ptr;
                ++d;
            }

#ifdef __cpp_lib_uncaught_exceptions
            // Work around the sorters that return void
            auto exit_function = scope_success([&] {
                // Copy the sorted elements back in the original collection
                std::move(buffer.get(), buffer.get() + size, first);
            });

            // Sort the elements in the memory buffer
            return sorter(buffer.get(), buffer.get() + size, std::forward<Args>(args)...);
#else
            // Sort the elements in the memory buffer
            sorter(buffer.get(), buffer.get() + size, std::forward<Args>(args)...);
            // Copy the sorted elements back in the original collection
            std::move(buffer.get(), buffer.get() + size, first);
#endif
        }
    }

    template<typename Sorter>
    struct out_of_place_adapter:
        detail::check_iterator_category<Sorter>,
        detail::check_is_always_stable<Sorter>,
        sorter_facade_fptr<out_of_place_adapter<Sorter>>
    {
        ////////////////////////////////////////////////////////////
        // Construction

        out_of_place_adapter() = default;

        // Automatic deduction guide
        constexpr explicit out_of_place_adapter(Sorter) noexcept {}

        ////////////////////////////////////////////////////////////
        // Wrap and call the sorter

        template<typename Iterator, typename... Args>
        auto operator()(Iterator first, Iterator last, Args&&... args) const
            -> decltype(auto)
        {
            auto size = std::distance(first, last);
            return detail::sort_out_of_place(first, size, Sorter{}, std::forward<Args>(args)...);
        }

        template<typename Iterable, typename... Args>
        auto operator()(Iterable&& iterable, Args&&... args) const
            -> decltype(auto)
        {
            // Might be an optimization for forward/bidirectional iterables
            auto size = utility::size(iterable);
            return detail::sort_out_of_place(std::begin(iterable), size, Sorter{}, std::forward<Args>(args)...);
        }
    };

    ////////////////////////////////////////////////////////////
    // is_stable specialization

    template<typename Sorter, typename... Args>
    struct is_stable<out_of_place_adapter<Sorter>(Args...)>:
        is_stable<Sorter(Args...)>
    {};
}

#endif // CPPSORT_ADAPTERS_OUT_OF_PLACE_ADAPTER_H_
