/*
 * Copyright (c) 2015-2024 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <cstddef>
#include <ctime>
#include <random>
#include <string>
#include <utility>
#include <vector>
#include <cpp-sort/detail/bitops.h>
#include <cpp-sort/detail/type_traits.h>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/functional.h>

// Pseudo-random number generator, used by some distributions
thread_local std::mt19937_64 distributions_prng(std::time(nullptr));

////////////////////////////////////////////////////////////
// Distributions for benchmarks
//
// Distributions are function objects used to fill the
// collections to be sorted before benchmarking the sorting
// algorithms. These distributions allow to check how a
// sorting algorithm behaves with common patterns found in
// real-life data sets, or with patterns known to adversely
// affect some classes of sorting algorithms.

namespace dist
{
    ////////////////////////////////////////////////////////////
    // Distributions CRTP base class

    template<typename Derived>
    struct base_distribution
    {
        template<typename OutputIterator>
        using fptr_t = void(*)(OutputIterator, long long int);

        template<typename OutputIterator, typename Projection>
        using fptr_proj_t = void(*)(OutputIterator, long long int, Projection);

        template<typename OutputIterator>
        operator fptr_t<OutputIterator>() const
        {
            return [](OutputIterator out, long long int size)
            {
                return Derived{}(out, size);
            };
        }

        template<typename OutputIterator, typename Projection>
        operator fptr_proj_t<OutputIterator, Projection>() const
        {
            return [](OutputIterator out, long long int size, Projection projection)
            {
                return Derived{}(out, size, std::move(projection));
            };
        }
    };

    ////////////////////////////////////////////////////////////
    // Distributions: testing patterns

    struct shuffled:
        base_distribution<shuffled>
    {
        template<typename OutputIterator, typename Projection=cppsort::utility::identity>
        auto operator()(OutputIterator out, long long int size, Projection projection={}) const
            -> void
        {
            std::vector<int> vec;
            vec.reserve(size);
            for (long long int i = 0; i < size; ++i) {
                vec.emplace_back(i);
            }
            std::shuffle(vec.begin(), vec.end(), distributions_prng);

            auto&& proj = cppsort::utility::as_function(projection);
            std::transform(vec.begin(), vec.end(), out, proj);
        }

        static constexpr const char* name = "shuffled";
    };

    struct shuffled_16_values:
        base_distribution<shuffled_16_values>
    {
        template<typename OutputIterator, typename Projection=cppsort::utility::identity>
        auto operator()(OutputIterator out, long long int size, Projection projection={}) const
            -> void
        {
            std::vector<int> vec;
            vec.reserve(size);
            for (long long int i = 0; i < size; ++i) {
                vec.emplace_back(i % 16);
            }
            std::shuffle(vec.begin(), vec.end(), distributions_prng);

            auto&& proj = cppsort::utility::as_function(projection);
            std::transform(vec.begin(), vec.end(), out, proj);
        }

        static constexpr const char* name = "shuffled_16_values";
    };

    struct all_equal:
        base_distribution<all_equal>
    {
        template<typename OutputIterator, typename Projection=cppsort::utility::identity>
        auto operator()(OutputIterator out, long long int size, Projection projection={}) const
            -> void
        {
            auto&& proj = cppsort::utility::as_function(projection);
            for (long long int i = 0; i < size; ++i) {
                *out++ = proj(0);
            }
        }

        static constexpr const char* name = "all_equal";
    };

    struct ascending:
        base_distribution<ascending>
    {
        template<typename OutputIterator, typename Projection=cppsort::utility::identity>
        auto operator()(OutputIterator out, long long int size, Projection projection={}) const
            -> void
        {
            auto&& proj = cppsort::utility::as_function(projection);
            for (long long int i = 0; i < size; ++i) {
                *out++ = proj(i);
            }
        }

        static constexpr const char* name = "ascending";
    };

    struct descending:
        base_distribution<descending>
    {
        template<typename OutputIterator, typename Projection=cppsort::utility::identity>
        auto operator()(OutputIterator out, long long int size, Projection projection={}) const
            -> void
        {
            auto&& proj = cppsort::utility::as_function(projection);
            while (size--) {
                *out++ = proj(size);
            }
        }

        static constexpr const char* name = "descending";
    };

    struct pipe_organ:
        base_distribution<pipe_organ>
    {
        template<typename OutputIterator, typename Projection=cppsort::utility::identity>
        auto operator()(OutputIterator out, long long int size, Projection projection={}) const
            -> void
        {
            auto&& proj = cppsort::utility::as_function(projection);
            for (long long int i = 0; i < size / 2; ++i) {
                *out++ = proj(i);
            }
            for (long long int i = size / 2; i < size; ++i) {
                *out++ = proj(size - i);
            }
        }

        static constexpr const char* name = "pipe_organ";
    };

    struct push_front:
        base_distribution<push_front>
    {
        template<typename OutputIterator, typename Projection=cppsort::utility::identity>
        auto operator()(OutputIterator out, long long int size, Projection projection={}) const
            -> void
        {
            auto&& proj = cppsort::utility::as_function(projection);
            if (size > 0) {
                for (long long int i = 0; i < size - 1; ++i) {
                    *out++ = proj(i);
                }
                *out = proj(0);
            }
        }

        static constexpr const char* name = "push_front";
    };

    struct push_middle:
        base_distribution<push_middle>
    {
        template<typename OutputIterator, typename Projection=cppsort::utility::identity>
        auto operator()(OutputIterator out, long long int size, Projection projection={}) const
            -> void
        {
            auto&& proj = cppsort::utility::as_function(projection);
            if (size > 0) {
                for (long long int i = 0; i < size; ++i) {
                    if (i != size / 2) {
                        *out++ = proj(i);
                    }
                }
                *out = proj(size / 2);
            }
        }

        static constexpr const char* name = "push_middle";
    };

    struct ascending_sawtooth:
        base_distribution<ascending_sawtooth>
    {
        template<typename OutputIterator, typename Projection=cppsort::utility::identity>
        auto operator()(OutputIterator out, long long int size, Projection projection={}) const
            -> void
        {
            auto&& proj = cppsort::utility::as_function(projection);
            long long int limit = size / cppsort::detail::log2(size) + 50;
            for (long long int i = 0; i < size; ++i) {
                *out++ = proj(i % limit);
            }
        }

        static constexpr const char* name = "ascending_sawtooth";
    };

    struct ascending_sawtooth_bad:
        base_distribution<ascending_sawtooth_bad>
    {
        template<typename OutputIterator, typename Projection=cppsort::utility::identity>
        auto operator()(OutputIterator out, long long int size, Projection projection={}) const
            -> void
        {
            auto&& proj = cppsort::utility::as_function(projection);
            long long int limit = size / cppsort::detail::log2(size) - 50;
            for (long long int i = 0; i < size; ++i) {
                *out++ = proj(i % limit);
            }
        }

        static constexpr const char* name = "ascending_sawtooth_bad";
    };

    struct descending_sawtooth:
        base_distribution<descending_sawtooth>
    {
        template<typename OutputIterator, typename Projection=cppsort::utility::identity>
        auto operator()(OutputIterator out, long long int size, Projection projection={}) const
            -> void
        {
            auto&& proj = cppsort::utility::as_function(projection);
            long long int limit = size / cppsort::detail::log2(size) + 50;
            while (size--) {
                *out++ = proj(size % limit);
            }
        }

        static constexpr const char* name = "descending_sawtooth";
    };

    struct descending_sawtooth_bad:
        base_distribution<descending_sawtooth_bad>
    {
        template<typename OutputIterator, typename Projection=cppsort::utility::identity>
        auto operator()(OutputIterator out, long long int size, Projection projection={}) const
            -> void
        {
            auto&& proj = cppsort::utility::as_function(projection);
            long long int limit = size / cppsort::detail::log2(size) - 50;
            while (size--) {
                *out++ = proj(size % limit);
            }
        }

        static constexpr const char* name = "descending_sawtooth_bad";
    };

    struct alternating:
        base_distribution<alternating>
    {
        template<typename OutputIterator, typename Projection=cppsort::utility::identity>
        auto operator()(OutputIterator out, long long int size, Projection projection={}) const
            -> void
        {
            auto&& proj = cppsort::utility::as_function(projection);
            for (long long int i = 0; i < size; ++i) {
                *out++ = proj((i % 2) ? i : -i);
            }
        }

        static constexpr const char* name = "alternating";
    };

    struct reversed_alternating:
        base_distribution<reversed_alternating>
    {
        template<typename OutputIterator, typename Projection=cppsort::utility::identity>
        auto operator()(OutputIterator out, long long int size, Projection projection={}) const
            -> void
        {
            // Especially interesting for a special case of melsort

            auto&& proj = cppsort::utility::as_function(projection);
            for (long long int i = size; i > 0; --i) {
                *out++ = proj((i % 2) ? i : -i);
            }
        }

        static constexpr const char* name = "reversed_alternating";
    };

    struct descending_plateau:
        base_distribution<descending_plateau>
    {
        template<typename OutputIterator, typename Projection=cppsort::utility::identity>
        auto operator()(OutputIterator out, long long int size, Projection projection={}) const
            -> void
        {
            auto&& proj = cppsort::utility::as_function(projection);

            long long int i = size;
            while (i > 2 * size / 3) {
                *out++ = proj(i);
                --i;
            }
            while (i > size / 3) {
                *out++ = proj(size / 2);
                --i;
            }
            while (i > 0) {
                *out++ = proj(i);
                --i;
            }
        }

        static constexpr const char* name = "descending_plateau";
    };

    struct vergesort_killer:
        base_distribution<vergesort_killer>
    {
        template<typename OutputIterator, typename Projection=cppsort::utility::identity>
        auto operator()(OutputIterator out, long long int size, Projection projection={}) const
            -> void
        {
            // WARNING: not for small collections, mostly because I'm lazy...

            const auto size_run = size / cppsort::detail::log2(size);
            auto desc = descending{};
            auto killer = pipe_organ{};

            auto size_output_left = size;
            while (true) {
                killer(out, size_run - 50, projection);
                size_output_left -= size_run - 50;
                if (size_output_left < size_run + 50) break;
                desc(out, size_run + 50, projection);
                size_output_left -= size_run + 50;
                if (size_output_left < size_run - 50) break;
            };

            // Just in case
            if (size_output_left) {
                shuffled{}(out, size_output_left, projection);
            }
        }

        static constexpr const char* name = "vergesort_killer";
    };

    ////////////////////////////////////////////////////////////
    // Distributions: testing measures of presortedness

    struct inv:
        base_distribution<inv>
    {
        // Percentage of chances that an "out-of-place" value
        // is produced for each position, the goal is to test
        // Inv-adaptive algorithms
        double factor;

        constexpr explicit inv(double factor) noexcept:
            factor(factor)
        {}

        template<typename OutputIterator, typename Projection=cppsort::utility::identity>
        auto operator()(OutputIterator out, long long int size, Projection projection={}) const
            -> void
        {
            auto&& proj = cppsort::utility::as_function(projection);

            // Generate a percentage of error
            std::uniform_real_distribution<double> percent_dis(0.0, 1.0);
            // Generate a random value
            std::uniform_int_distribution<long long int> value_dis(0, size - 1);

            if (factor <= 0.5) {
                for (long long int i = 0; i < size; ++i) {
                    if (percent_dis(distributions_prng) < factor * 2) {
                        *out++ = proj(value_dis(distributions_prng));
                    } else {
                        *out++ = proj(i);
                    }
                }
            } else {
                for (long long int i = size; i > 0; --i) {
                    if (percent_dis(distributions_prng) > (factor - 0.5) * 2) {
                        *out++ = proj(value_dis(distributions_prng));
                    } else {
                        *out++ = proj(i);
                    }
                }
            }
        }

        static constexpr const char* name = "inv";
    };

    struct runs:
        base_distribution<runs>
    {
        double factor;

        constexpr explicit runs(double factor) noexcept:
            factor(factor)
        {}

        template<typename OutputIterator, typename Projection=cppsort::utility::identity>
        auto operator()(OutputIterator out, long long int size, Projection projection={}) const
            -> void
        {
            auto&& proj = cppsort::utility::as_function(projection);

            if (size == 0) return;
            if (size == 1) {
                *out++ = proj(1);
                return;
            }

            // Runs(X) actually computes the number of step-downs in a collection:
            // the number of elements smaller than the previous one
            auto step_downs = static_cast<long long int>(factor * (size - 1));
            // Average number of elements between two step-downs
            auto delta = step_downs == 0 ? 0.0 :
                static_cast<double>(size - 1) / static_cast<double>(step_downs);

            // Generate increasing values, except when encountering a "step-down"
            // boundary, in which case we decrease by one: this way of generating
            // runs gives evenly sized runs no matter the factor
            long long int value = 0;
            long long int next_step = 1;
            for (long long int idx = 0; idx < size;) {
                if (++idx == static_cast<long long int>(next_step * delta)) {
                    --value;
                    ++next_step;
                } else {
                    ++value;
                }
                *out++ = proj(value);
            }
        }

        static constexpr const char* name = "runs";
    };

    ////////////////////////////////////////////////////////////
    // Miscellaneous related tools

    struct as_long_string:
        cppsort::utility::projection_base
    {
        auto operator()(long long int value) const
            -> std::string
        {
            auto str = std::to_string(value);
            return std::string(50 - str.size(), '0') + std::move(str);
        }
    };
}
