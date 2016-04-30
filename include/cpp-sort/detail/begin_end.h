/// \file
// Range v3 library
//
//  Copyright Eric Niebler 2013-2014
//  Modified in 2016 by Morwenn for inclusion into cpp-sort
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3
//

#ifndef CPPSORT_DETAIL_BEGIN_END_H_
#define CPPSORT_DETAIL_BEGIN_END_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <type_traits>
#include <cpp-sort/utility/static_const.h>

namespace cppsort
{
namespace detail
{
    namespace adl_begin_end_detail
    {
        struct begin_fn;
        struct end_fn;
        struct cbegin_fn;
        struct cend_fn;
        struct rbegin_fn;
        struct rend_fn;
        struct crbegin_fn;
        struct crend_fn;
    }

    using adl_begin_end_detail::begin_fn;
    using adl_begin_end_detail::end_fn;
    using adl_begin_end_detail::cbegin_fn;
    using adl_begin_end_detail::cend_fn;
    using adl_begin_end_detail::rbegin_fn;
    using adl_begin_end_detail::rend_fn;
    using adl_begin_end_detail::crbegin_fn;
    using adl_begin_end_detail::crend_fn;

    namespace adl_begin_end_detail
    {
        using std::begin;
        using std::end;

        // A reference-wrapped Range is an Range
        template<typename T>
        auto begin(std::reference_wrapper<T> ref)
            -> decltype(begin(ref.get()))
        {
            return begin(ref.get());
        }

        template<typename T>
        auto end(std::reference_wrapper<T> ref)
            -> decltype(end(ref.get()))
        {
            return end(ref.get());
        }

        template<typename T>
        auto rbegin(std::reference_wrapper<T> ref)
            -> decltype(rbegin(ref.get()))
        {
            return rbegin(ref.get());
        }

        template<typename T>
        auto rend(std::reference_wrapper<T> ref)
            -> decltype(rend(ref.get()))
        {
            return rend(ref.get());
        }

        struct begin_fn
        {
        private:
            template<typename Rng>
            static constexpr auto impl(Rng&& rng, long)
                noexcept(noexcept(begin(static_cast<Rng&&>(rng))))
                -> decltype(begin(static_cast<Rng&&>(rng)))
            {
                return begin(static_cast<Rng&&>(rng));
            }

            template<
                typename Rng,
                typename = std::enable_if_t<std::is_lvalue_reference<Rng>::value>
            >
            static constexpr auto impl(Rng&& rng, int)
                noexcept(noexcept(rng.begin()))
                -> decltype(rng.begin())
            {
                return rng.begin();
            }

        public:
            template<typename Rng>
            constexpr auto operator()(Rng&& rng) const
                noexcept(noexcept(begin_fn::impl(static_cast<Rng&&>(rng), 0)))
                -> std::decay_t<decltype(begin_fn::impl(static_cast<Rng&&>(rng), 0))>
            {
                return begin_fn::impl(static_cast<Rng&&>(rng), 0);
            }
        };

        struct end_fn
        {
        private:
            template<typename Rng>
            static constexpr auto impl(Rng&& rng, long)
                noexcept(noexcept(end(static_cast<Rng&&>(rng))))
                -> decltype(end(static_cast<Rng&&>(rng)))
            {
                return end(static_cast<Rng&&>(rng));
            }

            template<
                typename Rng,
                typename = std::enable_if_t<std::is_lvalue_reference<Rng>::value>
            >
            static constexpr auto impl(Rng&& rng, int)
                noexcept(noexcept(rng.end()))
                -> decltype(rng.end())
            {
                return rng.end();
            }

        public:
            template<typename Rng>
            constexpr auto operator()(Rng&& rng) const
                noexcept(noexcept(end_fn::impl(static_cast<Rng&&>(rng), 0)))
                -> std::decay_t<decltype(end_fn::impl(static_cast<Rng&&>(rng), 0))>
            {
                return end_fn::impl(static_cast<Rng&&>(rng), 0);
            }
        };

        struct rbegin_fn
        {
        private:
            template<typename Rng>
            static constexpr auto impl(Rng&& rng, long)
                noexcept(noexcept(rbegin(static_cast<Rng&&>(rng))))
                -> decltype(rbegin(static_cast<Rng&&>(rng)))
            {
                return rbegin(static_cast<Rng&&>(rng));
            }

            template<
                typename Rng,
                typename = std::enable_if_t<std::is_lvalue_reference<Rng>::value>
            >
            static constexpr auto impl(Rng&& rng, int)
                noexcept(noexcept(rng.rbegin()))
                -> decltype(rng.rbegin())
            {
                return rng.rbegin();
            }

        public:
            template<typename Rng>
            constexpr auto operator()(Rng&& rng) const
                noexcept(noexcept(rbegin_fn::impl(static_cast<Rng&&>(rng), 0)))
                -> std::decay_t<decltype(rbegin_fn::impl(static_cast<Rng&&>(rng), 0))>
            {
                return rbegin_fn::impl(static_cast<Rng&&>(rng), 0);
            }

            template<typename T, std::size_t N>
            constexpr auto operator()(T (&t)[N]) const noexcept
                -> std::reverse_iterator<T*>
            {
                return std::reverse_iterator<T*>(t + N);
            }

            template<typename T>
            constexpr auto operator()(std::initializer_list<T> il) const noexcept
                -> std::reverse_iterator<T const*>
            {
                return std::reverse_iterator<T const*>(il.end());
            }
        };

        struct rend_fn
        {
        private:
            template<typename Rng>
            static constexpr auto impl(Rng&& rng, long)
                noexcept(noexcept(rend(static_cast<Rng&&>(rng))))
                -> decltype(rend(static_cast<Rng&&>(rng)))
            {
                return rend(static_cast<Rng&&>(rng));
            }

            template<
                typename Rng,
                typename = std::enable_if_t<std::is_lvalue_reference<Rng>::value>
            >
            static constexpr auto impl(Rng&& rng, int)
                noexcept(noexcept(rng.rend()))
                -> decltype(rng.rend())
            {
                return rng.rend();
            }
        public:
            template<typename Rng>
            constexpr auto operator()(Rng&& rng) const
                noexcept(noexcept(rend_fn::impl(static_cast<Rng&&>(rng), 0)))
                -> std::decay_t<decltype(rend_fn::impl(static_cast<Rng&&>(rng), 0))>
            {
                return rend_fn::impl(static_cast<Rng&&>(rng), 0);
            }

            template<typename T, std::size_t N>
            constexpr auto operator()(T (&t)[N]) const noexcept
                -> std::reverse_iterator<T*>
            {
                return std::reverse_iterator<T*>(t);
            }
            template<typename T>
            constexpr auto operator()(std::initializer_list<T> il) const noexcept
                -> std::reverse_iterator<T const*>
            {
                return std::reverse_iterator<T const*>(il.begin());
            }
        };

        struct cbegin_fn
        {
            template<typename Rng>
            constexpr auto operator()(const Rng& rng) const
                noexcept(noexcept(utility::static_const<begin_fn>::value(rng)))
                -> decltype(utility::static_const<begin_fn>::value(rng))
            {
                return utility::static_const<begin_fn>::value(rng);
            }
        };

        struct cend_fn
        {
            template<typename Rng>
            constexpr auto operator()(const Rng& rng) const
                noexcept(noexcept(utility::static_const<end_fn>::value(rng)))
                -> decltype(utility::static_const<end_fn>::value(rng))
            {
                return utility::static_const<end_fn>::value(rng);
            }
        };

        struct crbegin_fn
        {
            template<typename Rng>
            constexpr auto operator()(const Rng& rng) const
                noexcept(noexcept(utility::static_const<rbegin_fn>::value(rng)))
                -> decltype(utility::static_const<rbegin_fn>::value(rng))
            {
                return utility::static_const<rbegin_fn>::value(rng);
            }
        };

        struct crend_fn
        {
            template<typename Rng>
            constexpr auto operator()(const Rng& rng) const
                noexcept(noexcept(utility::static_const<rend_fn>::value(rng)))
                -> decltype(utility::static_const<rend_fn>::value(rng))
            {
                return utility::static_const<rend_fn>::value(rng);
            }
        };
    }

    /// \ingroup group-core
    /// \return The result of an unqualified call to the `begin` free function
    namespace
    {
        constexpr auto&& begin = utility::static_const<begin_fn>::value;
    }

    /// \ingroup group-core
    /// \return The result of an unqualified call to the `end` free function
    namespace
    {
        constexpr auto&& end = utility::static_const<end_fn>::value;
    }

    /// \ingroup group-core
    /// \return The result of an unqualified call to the `begin` free function
    /// with a const-qualified argument.
    namespace
    {
        constexpr auto&& cbegin = utility::static_const<cbegin_fn>::value;
    }

    /// \ingroup group-core
    /// \return The result of an unqualified call to the `end` free function
    /// with a const-qualified argument.
    namespace
    {
        constexpr auto&& cend = utility::static_const<cend_fn>::value;
    }

    /// \ingroup group-core
    /// \return The result of an unqualified call to the `rbegin` free function
    namespace
    {
        constexpr auto&& rbegin = utility::static_const<rbegin_fn>::value;
    }

    /// \ingroup group-core
    /// \return The result of an unqualified call to the `rend` free function
    namespace
    {
        constexpr auto&& rend = utility::static_const<rend_fn>::value;
    }

    /// \ingroup group-core
    /// \return The result of an unqualified call to the `rbegin` free function
    /// with a const-qualified argument.
    namespace
    {
        constexpr auto&& crbegin = utility::static_const<crbegin_fn>::value;
    }

    /// \ingroup group-core
    /// \return The result of an unqualified call to the `rend` free function
    /// with a const-qualified argument.
    namespace
    {
        constexpr auto&& crend = utility::static_const<crend_fn>::value;
    }
}}

#endif // CPPSORT_DETAIL_BEGIN_END_H_
