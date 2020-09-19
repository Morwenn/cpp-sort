/*
 * Copyright (c) 2018-2020 Morwenn
 * SPDX-License-Identifier: MIT
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
#include <cpp-sort/utility/adapter_storage.h>
#include <cpp-sort/utility/iter_move.h>
#include <cpp-sort/utility/size.h>
#include "../detail/checkers.h"
#include "../detail/iterator_traits.h"
#include "../detail/memory.h"
#include "../detail/scope_exit.h"
#include "../detail/type_traits.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Adapter

    namespace detail
    {
        template<typename Sorter, typename ForwardIterator, typename Size, typename... Args>
        auto sort_out_of_place(ForwardIterator first, Size size, const Sorter& sorter, Args&&... args)
            -> decltype(auto)
        {
            using utility::iter_move;
            using rvalue_reference = remove_cvref_t<rvalue_reference_t<ForwardIterator>>;

            // Copy the collection into contiguous memory buffer
            std::unique_ptr<rvalue_reference, operator_deleter> buffer(
                static_cast<rvalue_reference*>(::operator new(size * sizeof(rvalue_reference))),
                operator_deleter(size * sizeof(rvalue_reference))
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
            auto exit_function = make_scope_success([&] {
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
        utility::adapter_storage<Sorter>,
        detail::check_is_always_stable<Sorter>,
        detail::sorter_facade_fptr<
            out_of_place_adapter<Sorter>,
            std::is_empty<Sorter>::value
        >
    {
        ////////////////////////////////////////////////////////////
        // Construction

        out_of_place_adapter() = default;

        constexpr explicit out_of_place_adapter(Sorter sorter):
            utility::adapter_storage<Sorter>(std::move(sorter))
        {}

        ////////////////////////////////////////////////////////////
        // Wrap and call the sorter

        template<typename Iterator, typename... Args>
        auto operator()(Iterator first, Iterator last, Args&&... args) const
            -> decltype(auto)
        {
            auto size = std::distance(first, last);
            return detail::sort_out_of_place(first, size, this->get(), std::forward<Args>(args)...);
        }

        template<typename Iterable, typename... Args>
        auto operator()(Iterable&& iterable, Args&&... args) const
            -> decltype(auto)
        {
            // Might be an optimization for forward/bidirectional iterables
            auto size = utility::size(iterable);
            return detail::sort_out_of_place(std::begin(iterable), size, this->get(), std::forward<Args>(args)...);
        }

        ////////////////////////////////////////////////////////////
        // Sorter traits

        using iterator_category = std::forward_iterator_tag;
    };

    ////////////////////////////////////////////////////////////
    // is_stable specialization

    template<typename Sorter, typename... Args>
    struct is_stable<out_of_place_adapter<Sorter>(Args...)>:
        is_stable<Sorter(Args...)>
    {};
}

#endif // CPPSORT_ADAPTERS_OUT_OF_PLACE_ADAPTER_H_
