/*
 * Copyright (c) 2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_MSTD_RANGES_H_
#define CPPSORT_MSTD_RANGES_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <memory>
#include <ranges>
#include <type_traits>
#include <utility>
#include <cpp-sort/mstd/iterator.h>
#include "../detail/bitops.h"

// Copy of _LIBCPP_AUTO_CAST, preserved as a macro to
// simplify future copy-paste-steal maintenance
#define CPPSORT_AUTO_CAST(expr) static_cast<std::decay_t<decltype((expr))>>(expr)

namespace cppsort::mstd
{
    namespace detail
    {
        template<typename T>
        concept can_borrow =
            std::is_lvalue_reference_v<T> ||
            std::ranges::enable_borrowed_range<std::remove_cvref_t<T>>;
    }

    ////////////////////////////////////////////////////////////
    // begin

    namespace detail_begin
    {
        template<typename T>
        concept member_begin =
            detail::can_borrow<T> &&
            detail::class_or_union<std::remove_cvref_t<T>> &&
            requires(T&& rng) {
                { CPPSORT_AUTO_CAST(rng.begin()) } -> input_or_output_iterator;
            };

        void begin(auto&) = delete;
        void begin(const auto&) = delete;

        template<class T>
        concept unqualified_begin =
            not member_begin<T> &&
            detail::can_borrow<T> &&
            detail::class_or_union_or_enum<std::remove_cvref_t<T>> &&
            requires(T&& rng) {
                { CPPSORT_AUTO_CAST(begin(rng)) } -> input_or_output_iterator;
            };

        struct begin_fn
        {
            template<typename T>
            [[nodiscard]]
            constexpr auto operator()(T (&arr)[]) const noexcept
                requires (sizeof(T) >= 0)  // Disallow incomplete element types.
            {
                return arr + 0;
            }

            template<typename T, std::size_t N>
            [[nodiscard]]
            constexpr auto operator()(T (&arr)[N]) const noexcept
                requires (sizeof(T) >= 0)  // Disallow incomplete element types.
            {
                return arr + 0;
            }

            template<member_begin T>
            [[nodiscard]]
            constexpr auto operator()(T&& rng) const
                noexcept(noexcept(CPPSORT_AUTO_CAST(rng.begin())))
            {
                return CPPSORT_AUTO_CAST(rng.begin());
            }

            template<unqualified_begin T>
            [[nodiscard]]
            constexpr auto operator()(T&& rng) const
                noexcept(noexcept(CPPSORT_AUTO_CAST(begin(rng))))
            {
                return CPPSORT_AUTO_CAST(begin(rng));
            }

            void operator()(auto&&) const = delete;
        };
    }

    inline namespace cpo
    {
        inline constexpr auto begin = detail_begin::begin_fn{};
    }

    ////////////////////////////////////////////////////////////
    // iterator_t

    template<typename T>
    using iterator_t = decltype(mstd::begin(std::declval<T&>()));

    ////////////////////////////////////////////////////////////
    // end

    namespace detail_end
    {
        template<typename T>
        concept member_end =
            detail::can_borrow<T> &&
            detail::class_or_union<std::remove_cvref_t<T>> &&
            requires(T&& rng) {
                typename iterator_t<T>;
                { CPPSORT_AUTO_CAST(rng.end()) } -> sentinel_for<iterator_t<T>>;
            };

        void end(auto&) = delete;
        void end(const auto&) = delete;

        template<typename T>
        concept unqualified_end =
            not member_end<T> &&
            detail::can_borrow<T> &&
            detail::class_or_union_or_enum<std::remove_cvref_t<T>> &&
            requires(T&& rng) {
                typename iterator_t<T>;
                { CPPSORT_AUTO_CAST(end(rng)) } -> sentinel_for<iterator_t<T>>;
            };

        struct end_fn
        {
            template<typename T, std::size_t N>
            [[nodiscard]]
            constexpr auto operator()(T (&arr)[N]) const noexcept
                requires (sizeof(T) >= 0)  // Disallow incomplete element types.
            {
                return arr + N;
            }

            template<member_end T>
            [[nodiscard]]
            constexpr auto operator()(T&& rng) const
                noexcept(noexcept(CPPSORT_AUTO_CAST(rng.end())))
            {
                return CPPSORT_AUTO_CAST(rng.end());
            }

            template<unqualified_end T>
            [[nodiscard]]
            constexpr auto operator()(T&& rng) const
                noexcept(noexcept(CPPSORT_AUTO_CAST(end(rng))))
            {
                return CPPSORT_AUTO_CAST(end(rng));
            }

            void operator()(auto&&) const = delete;
        };
    }

    inline namespace cpo
    {
        inline constexpr auto end = detail_end::end_fn{};
    }

    ////////////////////////////////////////////////////////////
    // data

    namespace detail_data
    {
        template<typename T>
        concept ptr_to_object =
            std::is_pointer_v<T> &&
            std::is_object_v<std::remove_pointer_t<T>>;

        template<typename T>
        concept member_data =
            detail::can_borrow<T> &&
            detail::class_or_union<std::remove_cvref_t<T>> &&
            requires(T&& t) {
                { CPPSORT_AUTO_CAST(t.data()) } -> ptr_to_object;
            };

        template<typename T>
        concept mstd_begin_invocable =
            not member_data<T> &&
            detail::can_borrow<T> &&
            requires(T&& t) {
                { mstd::begin(t) } -> contiguous_iterator;
            };

        struct data_fn
        {
            template<member_data T>
            constexpr auto operator()(T&& t) const
                noexcept(noexcept(t.data()))
            {
                return t.data();
            }

            template<mstd_begin_invocable T>
            constexpr auto operator()(T&& t) const
                noexcept(noexcept(std::to_address(mstd::begin(t))))
            {
                return std::to_address(mstd::begin(t));
            }
        };
    }

    inline namespace cpo
    {
        inline constexpr auto data = detail_data::data_fn{};
    }

    ////////////////////////////////////////////////////////////
    // range

    template<typename T>
    concept range = requires(T& value) {
        mstd::begin(value);
        mstd::end(value);
    };

    ////////////////////////////////////////////////////////////
    // input_range

    template<typename R>
    concept input_range =
        range<R> &&
        input_iterator<iterator_t<R>>;

    ////////////////////////////////////////////////////////////
    // forward_range

    template<typename R>
    concept forward_range =
        input_range<R> &&
        forward_iterator<iterator_t<R>>;

    ////////////////////////////////////////////////////////////
    // bidirectional_range

    template<typename R>
    concept bidirectional_range =
        forward_range<R> &&
        bidirectional_iterator<iterator_t<R>>;

    ////////////////////////////////////////////////////////////
    // random_access_range

    template<typename R>
    concept random_access_range =
        bidirectional_range<R> &&
        random_access_iterator<iterator_t<R>>;

    ////////////////////////////////////////////////////////////
    // contiguous_range

    template<typename R>
    concept contiguous_range =
        random_access_range<R> &&
        contiguous_iterator<iterator_t<R>> &&
        requires(R& rng) {
            { mstd::data(rng) }
                -> std::same_as<std::add_pointer_t<
                    decltype(*std::declval<iterator_t<R>&>())
                >>;
        };

    ////////////////////////////////////////////////////////////
    // sentinel_t

    template<range R>
    using sentinel_t = decltype(mstd::end(std::declval<R&>()));

    ////////////////////////////////////////////////////////////
    // range_difference_t

    template<range R>
    using range_difference_t = iter_difference_t<iterator_t<R>>;

    ////////////////////////////////////////////////////////////
    // range_reference_t

    template<range R>
    using range_reference_t = std::iter_reference_t<iterator_t<R>>;

    ////////////////////////////////////////////////////////////
    // size

    namespace detail_size
    {
        void size(auto&) = delete;
        void size(const auto&) = delete;

        template<typename T>
        concept size_enabled = not std::ranges::disable_sized_range<std::remove_cvref_t<T>>;

        template<typename T>
        concept member_size =
            size_enabled<T> &&
            detail::class_or_union<T> &&
            requires(T&& value) {
                { CPPSORT_AUTO_CAST(value.size()) } -> detail::integer_like;
            };

        template<typename T>
        concept unqualified_size =
            size_enabled<T> &&
            not member_size<T> &&
            detail::class_or_union_or_enum<std::remove_cvref_t<T>> &&
            requires(T&& value) {
                { CPPSORT_AUTO_CAST(size(value)) } -> detail::integer_like;
            };

        template<typename T>
        concept difference =
            not member_size<T> &&
            not unqualified_size<T> &&
            detail::class_or_union_or_enum<std::remove_cvref_t<T>> &&
            requires(T&& value) {
                { mstd::begin(value) } -> forward_iterator;
                { mstd::end(value) } -> sized_sentinel_for<decltype(mstd::begin(std::declval<T>()))>;
            };

        struct size_fn
        {
            // `[range.prim.size]`: the array case (for rvalues).
            template<typename T, std::size_t Size>
            [[nodiscard]]
            constexpr auto operator()(T (&&)[Size]) const noexcept
                -> std::size_t
            {
                return Size;
            }

            // `[range.prim.size]`: the array case (for lvalues).
            template<typename T, std::size_t Size>
            [[nodiscard]]
            constexpr auto operator()(T (&)[Size]) const noexcept
                -> std::size_t
            {
                return Size;
            }

            // `[range.prim.size]`: `auto(t.size())` is a valid expression.
            template<member_size T>
            [[nodiscard]]
            constexpr auto operator()(T&& value) const
                noexcept(noexcept(CPPSORT_AUTO_CAST(value.size())))
                 -> detail::integer_like auto
            {
                return CPPSORT_AUTO_CAST(value.size());
            }

            // `[range.prim.size]`: `auto(size(t))` is a valid expression.
            template<unqualified_size T>
            [[nodiscard]]
            constexpr auto operator()(T&& value) const
                noexcept(noexcept(CPPSORT_AUTO_CAST(size(value))))
                 ->  detail::integer_like auto
            {
                return CPPSORT_AUTO_CAST(size(value));
            }

            // [range.prim.size]: the `to-unsigned-like` case.
            template<difference T>
            [[nodiscard]]
            constexpr auto operator()(T&& value) const
                noexcept(noexcept(cppsort::detail::as_unsigned(mstd::end(value) - mstd::begin(value))))
                -> decltype(cppsort::detail::as_unsigned(mstd::end(value) - mstd::begin(value)))
            {
                return cppsort::detail::as_unsigned(mstd::end(value) - mstd::begin(value));
            }
        };
    }

    inline namespace cpo
    {
        inline constexpr auto size = detail_size::size_fn{};
    }

    ////////////////////////////////////////////////////////////
    // sized_range

    template<typename T>
    concept sized_range =
        range<T> &&
        requires(T& t) {
            mstd::size(t);
        };

    ////////////////////////////////////////////////////////////
    // distance

    namespace detail_distance
    {
        struct distance_fn
        {
            template<typename Iterator, sentinel_for<Iterator> Sentinel>
                requires (not sized_sentinel_for<Sentinel, Iterator>)
            [[nodiscard]]
            constexpr auto operator()(Iterator first, Sentinel last) const
                -> iter_difference_t<Iterator>
            {
                iter_difference_t<Iterator> n = 0;
                while (first != last) {
                    ++first;
                    ++n;
                }
                return n;
            }

            template<
                typename Iterator,
                sized_sentinel_for<std::decay_t<Iterator>> Sentinel
            >
            [[nodiscard]]
            constexpr auto operator()(Iterator&& first, Sentinel last) const
                -> iter_difference_t<std::decay_t<Iterator>>
            {
                if constexpr (sized_sentinel_for<Sentinel, std::remove_cvref_t<Iterator>>) {
                    return last - first;
                } else {
                    return last - static_cast<const std::decay_t<Iterator>&>(first);
                }
            }

            template<range Range>
            [[nodiscard]]
            constexpr auto operator()(Range&& range) const
                -> range_difference_t<Range>
            {
                if constexpr (sized_range<Range>) {
                    return static_cast<range_difference_t<Range>>(mstd::size(range));
                } else {
                    return operator()(mstd::begin(range), mstd::end(range));
                }
            }
        };
    }

    inline namespace cpo
    {
        inline constexpr auto distance = detail_distance::distance_fn{};
    }
}

#undef CPPSORT_AUTO_CAST

#endif // CPPSORT_MSTD_RANGES_H_
