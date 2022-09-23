/*
 * Copyright (c) 2020-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */

// Copyright (c) 2020, Matthew Bentley (mattreecebentley@gmail.com) www.plflib.org

// zLib license (https://www.zlib.net/zlib_license.html):
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
// 	claim that you wrote the original software. If you use this software
// 	in a product, an acknowledgement in the product documentation would be
// 	appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
// 	misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef CPPSORT_DETAIL_INDIESORT_H_
#define CPPSORT_DETAIL_INDIESORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <utility>
#include <cpp-sort/utility/iter_move.h>
#include "../detail/immovable_vector.h"
#include "iterator_traits.h"
#include "scope_exit.h"

namespace cppsort
{
namespace detail
{
    template<typename Iterator>
    struct it_and_index
    {
        Iterator original_location;
        difference_type_t<Iterator> original_index;

        it_and_index() = default;

        it_and_index(Iterator item, difference_type_t<Iterator> index) noexcept:
            original_location(item),
            original_index(index)
        {}
    };

    template<typename ForwardIterator, typename Sorter, typename Compare, typename Projection>
    auto indiesort(Sorter&& sorter, ForwardIterator first, ForwardIterator last,
                   difference_type_t<ForwardIterator> size,
                   Compare compare, Projection projection)
#ifdef __cpp_lib_uncaught_exceptions
        -> decltype(std::forward<Sorter>(sorter)(
            (it_and_index<ForwardIterator>*)0, (it_and_index<ForwardIterator>*)0,
            std::move(compare),
            &it_and_index<ForwardIterator>::original_location | indirect(projection)
        ))
#else
        -> std::enable_if_t<
                has_comparison_projection_sort_iterator<
                    Sorter,
                    it_and_index<ForwardIterator>*,
                    Compare,
                    decltype(&it_and_index<ForwardIterator>::original_location | indirect(projection))
                >::value
            >
#endif
    {
        using utility::iter_move;

        using item_index_tuple = it_and_index<ForwardIterator>;
        immovable_vector<item_index_tuple> storage(size);

        // Construct pointers to all elements in the sequence
        difference_type_t<ForwardIterator> index = 0;
        for (auto current_element = first; current_element != last; ++current_element) {
            storage.emplace_back(current_element, index);
            ++index;
        }

#ifndef __cpp_lib_uncaught_exceptions
        // Sort the iterators on pointed values
        std::forward<Sorter>(sorter)(
            storage.begin(), storage.end(), std::move(compare),
            &item_index_tuple::original_location | indirect(projection)
        );
#else
        // Work around the sorters that return void
        auto exit_function = make_scope_success([&] {
#endif

            // Sort the actual elements via the tuple array:
            index = 0;
            for (auto current_tuple = storage.begin(); current_tuple != storage.end(); ++current_tuple, ++index) {
                if (current_tuple->original_index != index) {
                    auto end_value = iter_move(current_tuple->original_location);

                    auto destination_index = index;
                    auto source_index = current_tuple->original_index;

                    do {
                        *(storage[destination_index].original_location) = iter_move(storage[source_index].original_location);

                        destination_index = source_index;
                        source_index = storage[destination_index].original_index;
                        storage[destination_index].original_index = destination_index;
                    } while (source_index != index);
                    *(storage[destination_index].original_location) = std::move(end_value);
                }
            }

#ifdef __cpp_lib_uncaught_exceptions
        });

        if (size < 2) {
            exit_function.deactivate();
        }

        // Sort the iterators on pointed values
        return std::forward<Sorter>(sorter)(
            storage.begin(), storage.end(), std::move(compare),
            &item_index_tuple::original_location | indirect(projection)
        );
#endif
    }
}}

#endif // CPPSORT_DETAIL_INDIESORT_H_
