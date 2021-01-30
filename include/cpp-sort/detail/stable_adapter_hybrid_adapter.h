/*
 * Copyright (c) 2018-2020 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_STABLE_ADAPTER_HYBRID_ADAPTER_H_
#define CPPSORT_DETAIL_STABLE_ADAPTER_HYBRID_ADAPTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <type_traits>
#include <utility>

namespace cppsort
{
    template<typename... Sorters>
    struct stable_adapter<hybrid_adapter<Sorters...>>:
        hybrid_adapter<stable_t<Sorters>...>
    {
        private:

            template<std::size_t... Indices>
            constexpr explicit stable_adapter(std::index_sequence<Indices...>, hybrid_adapter<Sorters...>&& sorters):
                hybrid_adapter<stable_t<Sorters>...>(
                    (stable_t<Sorters>(std::move(sorters).template get<Indices, Sorters>()))...
                )
            {}

        public:

            ////////////////////////////////////////////////////////////
            // Construction

            stable_adapter() = default;

            constexpr explicit stable_adapter(hybrid_adapter<Sorters...> sorters):
                stable_adapter(std::make_index_sequence<sizeof...(Sorters)>{}, std::move(sorters))
            {}

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using is_always_stable = std::true_type;
            using type = hybrid_adapter<stable_t<Sorters>...>;
    };
}

#endif // CPPSORT_DETAIL_STABLE_ADAPTER_HYBRID_ADAPTER_H_
