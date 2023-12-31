/*
 * Copyright (c) 2023 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <chrono>
#include <utility>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/utility/adapter_storage.h>
#include <cpp-sort/utility/metrics_tools.h>

TEST_CASE("simple utility::metric tests", "[utility][metrics]")
{
    using namespace cppsort::utility;
    using namespace std::chrono_literals;

    struct time_tag {};

    constexpr metric<std::chrono::seconds, time_tag> m1(5s);
    constexpr metric<std::chrono::milliseconds, time_tag> m2(5000ms);
    constexpr metric<std::chrono::milliseconds, time_tag> m3(4000ms);
    STATIC_CHECK( m1 == m2 );
    STATIC_CHECK( m3 != m2 );
    STATIC_CHECK( m3 < m1 );
}

TEST_CASE("simple utility::metrics tests", "[utility][metrics]")
{
    using std::get;
    using namespace cppsort::utility;

    struct foo_tag {};
    struct bar_tag {};
    using metric1 = metric<int, foo_tag>;
    using metric2 = metric<float, bar_tag>;

    metrics<metric1, metric2> mm(metric1(42), metric2(140.0));
    auto& m1 = get<foo_tag>(mm);
    CHECK( m1 == 42 );
    auto m2 = get<bar_tag>(mm);
    CHECK( m2 == 140.0 );

    get<foo_tag>(mm) = 35;
    CHECK( m1 == 35 );
}
