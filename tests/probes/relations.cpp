/*
 * Copyright (c) 2016-2026 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <cmath>
#include <iterator>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <rapidcheck.h>
#include <rapidcheck/catch.h>
#include <cpp-sort/probes.h>
#include <testing-tools/distributions.h>

TEST_CASE( "relations between measures of disorder", "[probe]" )
{
    // The adaptive sorting literature lists a number of relations between different
    // measures of disorder; the following tests check that these relations are
    // respected in the library

    using namespace cppsort::probe;
    using difference_type = std::vector<int>::difference_type;

    // Measures of Presortedness and Optimal Sorting Algorithms
    // by Heikki Mannila

    rc::prop("Exc(X) ≤ Inv(X)", [](const std::vector<int>& sequence) {
        return exc(sequence) <= inv(sequence);
    });

    // Splitsort - an adaptive sorting algorithm
    // by Christos Levcopoulos and Ola Petersson

    rc::prop("Rem(X) ≤ Inv(X)", [](const std::vector<int>& sequence) {
        return rem(sequence) <= inv(sequence);
    });

    // A framework for adaptive sorting
    // by Ola Petersson and Alistair Moffat

    rc::prop("Runs(X) ≤ Rem(X) + 1", [](const std::vector<int>& sequence) {
        return runs(sequence) <= rem(sequence) + 1;
    });

    rc::prop("Exc(X) + 1 ≤ Ham(X)", [](const std::vector<int>& sequence) {
        auto exc = cppsort::probe::exc(sequence);
        auto ham = cppsort::probe::ham(sequence);
        return (exc == 0 && ham == 0) || (exc + 1 <= ham);
    });
    rc::prop("Ham(X) ≤ 2 Exc(X)", [](const std::vector<int>& sequence) {
        return ham(sequence) <= 2 * exc(sequence);
    });

    rc::prop("Max(X) ≤ Dis(X)", [](const std::vector<int>& sequence) {
        return max(sequence) <= dis(sequence);
    });
    rc::prop("Dis(X) < 2 Max(X)", [](const std::vector<int>& sequence) {
        auto dis = cppsort::probe::dis(sequence);
        auto max = cppsort::probe::max(sequence);
        return (dis == 0 && max == 0) || (dis < 2 * max);
    });

    // A New Measure of Presortedness
    // by Vladimir Estivill-Castro and Derick Wood

    rc::prop("Dis(X) ≤ Inv(X)", [](const std::vector<int>& sequence) {
        return dis(sequence) <= inv(sequence);
    });

    rc::prop("Rem(X) ≤ |X| * (1 - 1 / (Dis(X) + 1))", [](const std::vector<int>& sequence) {
        auto size = static_cast<difference_type>(sequence.size());
        return rem(sequence) <= size * (1 - 1 / (dis(sequence) + 1));
    });

    rc::prop("Inv(X) ≤ |X| * Dis(X)/2", [](const std::vector<int>& sequence) {
        auto size = static_cast<difference_type>(sequence.size());
        return inv(sequence) <= size * dis(sequence) / 2;
    });

    // Practical Adaptive Sorting
    // by Vladimir Estivill-Castro and Derick Wood

    rc::prop("Rem(X) ≤ 2 Exc(X)", [](const std::vector<int>& sequence) {
        return rem(sequence) <= 2 * exc(sequence);
    });

    // Sorting, Measures of disorder, and Worst-case performance
    // by Vladimir Estivill-Castro and Derick Wood

    rc::prop("Rem(X) ≤ Ham(X)", [](const std::vector<int>& sequence) {
        return rem(sequence) <= ham(sequence);
    });

    // Encroaching lists as a measure of presortedness
    // by Steven S. Skiena

    rc::prop("Enc(X) ≤ Runs(X)", [](const std::vector<int>& sequence) {
        return enc(sequence) <= runs(sequence);
    });

    rc::prop("2 sqrt(Enc(X)) + 1 ≤ Inv(X)", [](const std::vector<int>& sequence) {
        auto enc = cppsort::probe::enc(sequence);
        auto inv = cppsort::probe::inv(sequence);
        return enc <= std::sqrt(2 * inv);
    });

    rc::prop("Enc(X) ≤ min(Rem(X) + 1, |X| - Rem(X))", [](const std::vector<int>& sequence) {
        auto size = static_cast<difference_type>(sequence.size());
        auto rem = cppsort::probe::rem(sequence);
        return enc(sequence) <= std::min<long long>(rem + 1, size - rem);
    });

    rc::prop("Enc(X) ≤ 2 Exc(X)", [](const std::vector<int>& sequence) {
        return cppsort::probe::enc(sequence) <= 2 * cppsort::probe::exc(sequence);
    });

    rc::prop("Conjecture: Enc(X) ≤ Exc(X)", [](const std::vector<int>& sequence) {
        return enc(sequence) <= exc(sequence);
    });

    // Sorting Shuffled Monotone Sequences
    // by Christos Levcopoulos and Ola Petersson

    rc::prop("SUS(X) ≤ Runs(X)", [](const std::vector<int>& sequence) {
        return sus(sequence) <= runs(sequence);
    });

    rc::prop("SUS(X) ≤ Max(X)", [](const std::vector<int>& sequence) {
        return sus(sequence) <= max(sequence);
    });

    rc::prop("Enc(X) ≤ SUS(X) + 1", [](const std::vector<int>& sequence) {
        return enc(sequence) <= sus(sequence) + 1;
    });

    // Heapsort - Adapted for Presorted Files
    // by Christos Levcopoulos and Ola Petersson

    rc::prop("Osc(X) ≤ 4 Inv(X)", [](const std::vector<int>& sequence) {
        return osc(sequence) <= 4 * inv(sequence);
    });

    rc::prop("Osc(X) ≤ 2 * |X| * Runs(X) + |X|", [](const std::vector<int>& sequence) {
        auto size = static_cast<difference_type>(sequence.size());
        return osc(sequence) <= 2 * size * runs(sequence) + size;
    });

    rc::prop("Osc(X) ≤ |X| * Dis(X)", [](const std::vector<int>& sequence) {
        auto size = static_cast<difference_type>(sequence.size());
        return osc(sequence) <= size * dis(sequence);
    });

    // Sublinear Merging and Natural Mergesort
    // by Svante Carlsson, Christos Levcopoulos and Ola Petersson

    rc::prop("Block(X) ≤ 3 Rem(X)", [](const std::vector<int>& sequence) {
        return block(sequence) <= 3 * rem(sequence);
    });

    // Computing and ranking measures of presortedness
    // by Jingsen Chen

    rc::prop("Enc(X) ≤ Dis(X) + 1", [](const std::vector<int>& sequence) {
        return enc(sequence) <= dis(sequence) + 1;
    });

    // Spearman's Footrule as a Measure of Disarray
    // by Persi Diaconis and Ronald Lewis Graham

    rc::prop("Inv(X) + Exc(X) ≤ Spear(X)", [](const std::vector<int>& sequence) {
        return inv(sequence) + exc(sequence) <= spear(sequence);
    });

    rc::prop("Spear(X) ≤ 2 Inv(X)", [](const std::vector<int>& sequence) {
        return spear(sequence) <= 2 * inv(sequence);
    });

    // Original research about Mono

    rc::prop("Mono(X) ≤ Runs(X)", [](const std::vector<int>& sequence) {
        return mono(sequence) <= runs(sequence);
    });

    rc::prop("Enc(X) ≤ Mono(X) + 1", [](const std::vector<int>& sequence) {
        return enc(sequence) <= mono(sequence) + 1;
    });

    // Original research about Amp

    // https://morwenn.github.io/presortedness/2025/11/09/TSB007-relationship-between-amp-and-runs.html
    rc::prop("Amp(X) ≤ 2 Runs(X)", [](const std::vector<int>& sequence) {
        return amp(sequence) <= 2 * runs(sequence);
    });

    // Conjectures
    rc::prop("Mono(X) ≤ Amp(X)", [](const std::vector<int>& sequence) {
        return mono(sequence) <= amp(sequence);
    });

    // Conjectures
    rc::prop("Mono(X) ≤ Amp(X)", [](const std::vector<int>& sequence) {
        return mono(sequence) <= amp(sequence);
    });

    // Conjectures
    rc::prop("Mono(X) ≤ Reve(X)", [](const std::vector<int>& sequence) {
        return mono(sequence) <= reve(sequence);
    });
    rc::prop("Reve(X) ≤ 2 Mono(X)", [](const std::vector<int>& sequence) {
        return reve(sequence) <= 2 * mono(sequence);
    });
    rc::prop("Reve(X) ≤ Amp(X)", [](const std::vector<int>& sequence) {
        return reve(sequence) <= amp(sequence);
    });

    rc::prop("Inv(X) % 2 = Exc(X) % 2", []() {
        auto sequence = *rc::gen::unique<std::vector<int>>(rc::gen::arbitrary<int>());
        return inv(sequence) % 2 == exc(sequence) % 2;
    });
}
