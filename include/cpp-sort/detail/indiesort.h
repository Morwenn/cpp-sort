/*
 * Copyright (c) 2020 Morwenn
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
#include <memory>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/iter_move.h>
#include "iterator_traits.h"
#include "memory.h"
#include "scope_exit.h"

namespace cppsort
{
namespace detail
{
    template<typename PointerType, typename SizeType>
    struct pointer_index_tuple
    {
        PointerType original_location;
        SizeType original_index;

        pointer_index_tuple() = default;

        pointer_index_tuple(PointerType item, SizeType index) noexcept:
            original_location(item),
            original_index(index)
        {}
    };

    template<typename ForwardIterator, typename Sorter, typename Compare, typename Projection>
    auto indiesort(Sorter&& sorter, ForwardIterator first, ForwardIterator last,
                   difference_type_t<ForwardIterator> size,
                   Compare compare, Projection projection)
        -> decltype(auto)
    {
        using difference_type = difference_type_t<ForwardIterator>;
        using utility::iter_move;
        auto&& proj = utility::as_function(projection);

        using item_index_tuple = pointer_index_tuple<ForwardIterator, difference_type>;
        std::unique_ptr<item_index_tuple, operator_deleter> storage(
            static_cast<item_index_tuple*>(::operator new(size * sizeof(item_index_tuple))),
            operator_deleter(size * sizeof(item_index_tuple))
        );
        destruct_n<item_index_tuple> d(0);
        std::unique_ptr<item_index_tuple, destruct_n<item_index_tuple>&> h2(storage.get(), d);

        auto sort_array = storage.get();
        item_index_tuple* tuple_pointer = storage.get();

        // Construct pointers to all elements in the sequence:
        difference_type index = 0;
        for (auto current_element = first; current_element != last; ++current_element) {
            ::new(tuple_pointer) item_index_tuple(current_element, index);
            ++tuple_pointer;
            ++index;
            ++d;
        }

#ifndef __cpp_lib_uncaught_exceptions
        // Sort the iterators on pointed values
        std::forward<Sorter>(sorter)(
            sort_array, sort_array + size, std::move(compare),
            [&proj](auto& elem) -> decltype(auto) {
                return proj(*(elem.original_location));
            }
        );
#else
        // Work around the sorters that return void
        auto exit_function = make_scope_success([&] {
#endif

            // Sort the actual elements via the tuple array:
            index = 0;
            for (auto current_tuple = sort_array; current_tuple != tuple_pointer; ++current_tuple, ++index) {
                if (current_tuple->original_index != index) {
                    auto end_value = iter_move(current_tuple->original_location);

                    difference_type destination_index = index;
                    difference_type source_index = current_tuple->original_index;

                    do {
                        *(sort_array[destination_index].original_location) = iter_move(sort_array[source_index].original_location);

                        destination_index = source_index;
                        source_index = sort_array[destination_index].original_index;
                        sort_array[destination_index].original_index = destination_index;
                    } while (source_index != index);
                    *(sort_array[destination_index].original_location) = std::move(end_value);
                }
            }

#ifdef __cpp_lib_uncaught_exceptions
        });

        if (size < 2) {
            exit_function.release();
        }

        // Sort the iterators on pointed values
        return std::forward<Sorter>(sorter)(
            sort_array, sort_array + size, std::move(compare),
            [&proj](auto& elem) -> decltype(auto) {
                return proj(*(elem.original_location));
            }
        );
#endif
    }
}}

#endif // CPPSORT_DETAIL_INDIESORT_H_
