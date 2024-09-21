/*
 * Copyright (c) 2022-2024 Morwenn
 * SPDX-License-Identifier: MIT
 */

//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef CPPSORT_MSTD_DETAIL_SUBRANGE_H_
#define CPPSORT_MSTD_DETAIL_SUBRANGE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <concepts>
#include <cstddef>
#include <type_traits>
#include <utility>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/mstd/type_traits.h>
#include "../../detail/bitops.h"
#include "begin.h"
#include "common.h"
#include "end.h"
#include "range.h"
#include "range_traits.h"
#include "size.h"

namespace cppsort::mstd
{
    ////////////////////////////////////////////////////////////
    // Helpers

    namespace detail
    {
        template<typename From, typename To>
        concept uses_nonqualification_pointer_conversion =
            std::is_pointer_v<From> &&
            std::is_pointer_v<To> &&
            not std::convertible_to<
                std::remove_pointer_t<From>(*)[],
                std::remove_pointer_t<To>(*)[]
            >;

        template<typename From, typename To>
        concept convertible_to_non_slicing =
            std::convertible_to<From, To> &&
            not uses_nonqualification_pointer_conversion<
                std::decay_t<From>,
                std::decay_t<To>
            >;
    }

    ////////////////////////////////////////////////////////////
    // subrange
    //
    // This is currently a stripped-down version of the standard
    // library class, notably not inheriting from view_interface
    // nor implementing the structured bindings protocol since
    // those features are not needed for the current internal
    // use. As a result the class is not publicly documented nor
    // included in mstd/ranges.h.

    template<
        input_or_output_iterator Iterator,
        sentinel_for<Iterator> Sentinel = Iterator,
        std::ranges::subrange_kind Kind = sized_sentinel_for<Sentinel, Iterator>
            ? std::ranges::subrange_kind::sized
            : std::ranges::subrange_kind::unsized
        >
        requires (
            Kind == std::ranges::subrange_kind::sized ||
            not sized_sentinel_for<Sentinel, Iterator>
        )
    class subrange
    {
        public:

            // Note: this is an internal implementation detail that
            //       is public only for internal usage.
            static constexpr bool store_size = (
                Kind == std::ranges::subrange_kind::sized &&
                not sized_sentinel_for<Sentinel, Iterator>
            );

        private:

            // just to improve compiler diagnostics
            static constexpr bool must_provide_size_at_construction = not store_size;

            struct empty_struct
            {
                constexpr empty_struct(auto) noexcept {}
            };
            using Size = mstd::conditional_t<
                store_size,
                mstd::make_unsigned_t<iter_difference_t<Iterator>>,
                empty_struct
            >;
            [[no_unique_address]] Iterator begin_ = Iterator();
            [[no_unique_address]] Sentinel end_ = Sentinel();
            [[no_unique_address]] Size size_ = 0;

        public:

            subrange() requires std::default_initializable<Iterator> = default;

            constexpr subrange(
                detail::convertible_to_non_slicing<Iterator> auto iter,
                Sentinel sent
            )
                requires must_provide_size_at_construction:
                begin_(std::move(iter)),
                end_(std::move(sent))
            {}

            constexpr subrange(
                detail::convertible_to_non_slicing<Iterator> auto iter,
                Sentinel sent,
                make_unsigned_t<iter_difference_t<Iterator>> n
            )
                requires (Kind == std::ranges::subrange_kind::sized):
                begin_(std::move(iter)),
                end_(std::move(sent)),
                size_(n)
            {
                if constexpr (sized_sentinel_for<Sentinel, Iterator>) {
                    CPPSORT_ASSERT((end_ - begin_) == static_cast<iter_difference_t<Iterator>>(n));
                }
            }

            template<detail::different_from<subrange> Range>
                requires borrowed_range<Range> &&
                detail::convertible_to_non_slicing<iterator_t<Range>, Iterator> &&
                std::convertible_to<sentinel_t<Range>, Sentinel>
            constexpr subrange(Range&& range) requires (not store_size):
                subrange(mstd::begin(range), mstd::end(range))
            {}

            template<detail::different_from<subrange> Range>
                requires borrowed_range<Range> &&
                detail::convertible_to_non_slicing<iterator_t<Range>, Iterator> &&
                std::convertible_to<sentinel_t<Range>, Sentinel>
            constexpr subrange(Range&& range) requires store_size && sized_range<Range>:
                subrange(range, mstd::size(range))
            {}

            template<borrowed_range Range>
                requires detail::convertible_to_non_slicing<iterator_t<Range>, Iterator> &&
                std::convertible_to<sentinel_t<Range>, Sentinel>
            constexpr subrange(Range&& range, make_unsigned_t<iter_difference_t<Iterator>> n)
                requires (Kind == std::ranges::subrange_kind::sized):
                subrange(mstd::begin(range), mstd::end(range), n)
            {}

            constexpr auto begin() const
                -> Iterator
                requires std::copyable<Iterator>
            {
                return begin_;
            }

            [[nodiscard]]
            constexpr auto begin()
                -> Iterator
                requires (not std::copyable<Iterator>)
            {
                return std::move(begin_);
            }

            constexpr auto end() const
                -> Sentinel
            {
                return end_;
            }

            [[nodiscard]]
            constexpr auto empty() const
                -> bool
            {
                return begin_ == end_;
            }

            constexpr auto size() const
                -> make_unsigned_t<iter_difference_t<Iterator>>
                requires (Kind == std::ranges::subrange_kind::sized)
            {
                if constexpr (store_size) {
                    return size_;
                } else {
                    return cppsort::detail::as_unsigned(end_ - begin_);
                }
            }

            [[nodiscard]]
            constexpr auto next(iter_difference_t<Iterator> n = 1) const&
                -> subrange
                requires forward_iterator<Iterator>
            {
                auto tmp = *this;
                tmp.advance(n);
                return tmp;
            }

            [[nodiscard]]
            constexpr auto next(iter_difference_t<Iterator> n = 1) &&
                -> subrange
            {
                this->advance(n);
                return std::move(*this);
            }

            [[nodiscard]]
            constexpr auto prev(iter_difference_t<Iterator> n = 1) const
                -> subrange
                requires bidirectional_iterator<Iterator>
            {
                auto tmp = *this;
                tmp.advance(-n);
                return tmp;
            }

            constexpr auto advance(iter_difference_t<Iterator> n)
                -> subrange&
            {
                if constexpr (bidirectional_iterator<Iterator>) {
                    if (n < 0) {
                        mstd::advance(begin_, n);
                        if constexpr (store_size) {
                            size_ += cppsort::detail::as_unsigned(-n);
                        }
                        return *this;
                    }
                }

                auto d = n - mstd::advance(begin_, n, end_);
                if constexpr (store_size) {
                    size_ -= cppsort::detail::as_unsigned(d);
                }
                return *this;
            }
    };

    template<input_or_output_iterator Iterator, sentinel_for<Iterator> Sentinel>
    subrange(Iterator, Sentinel)
        -> subrange<Iterator, Sentinel>;

    template<input_or_output_iterator Iterator, sentinel_for<Iterator> Sentinel>
    subrange(Iterator, Sentinel, make_unsigned_t<iter_difference_t<Iterator>>)
        -> subrange<Iterator, Sentinel, std::ranges::subrange_kind::sized>;

    template<borrowed_range Range>
    subrange(Range&&)
        -> subrange<
            iterator_t<Range>, sentinel_t<Range>,
            (sized_range<Range> || sized_sentinel_for<sentinel_t<Range>, iterator_t<Range>>)
                ? std::ranges::subrange_kind::sized
                : std::ranges::subrange_kind::unsized
        >;

    template<borrowed_range Range>
    subrange(Range&&, make_unsigned_t<range_difference_t<Range>>)
        -> subrange<
            iterator_t<Range>, sentinel_t<Range>,
            std::ranges::subrange_kind::sized
        >;

    ////////////////////////////////////////////////////////////
    // borrowed_subrange_t

    template<range R>
    using borrowed_subrange_t = conditional_t<
        borrowed_range<R>,
        subrange<iterator_t<R>>,
        std::ranges::dangling
    >;
}

namespace std::ranges
{
    template<typename Iterator, typename Sentinel, subrange_kind Kind>
    inline constexpr bool enable_borrowed_range<
        cppsort::mstd::subrange<Iterator, Sentinel, Kind>
    > = true;
}

#endif // CPPSORT_MSTD_DETAIL_SUBRANGE_H_
