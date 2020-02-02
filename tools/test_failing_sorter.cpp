#include <algorithm>
#include <iostream>
#include <iterator>
#include <random>
#include <string>
#include <utility>
#include <vector>
#include <cpp-sort/probes.h>
#include <cpp-sort/sorters.h>
#include <cpp-sort/utility/buffer.h>
#include <cpp-sort/utility/functional.h>
#include "../testsuite/distributions.h"

struct shuffled_string:
    dist::distribution<shuffled_string>
{
    template<typename OutputIterator, typename T=long long int>
    auto operator()(OutputIterator out, long long int size, T start=T(0)) const
        -> void
    {
        // Pseudo-random number generator
        thread_local std::mt19937 engine(15321);

        std::vector<std::string> vec;
        vec.reserve(size);

        T end = start + size;
        for (auto i = start ; i < end ; ++i) {
            auto s = std::to_string(i);
            vec.push_back(std::string(100 - s.size(), '0') + std::move(s));
        }
        std::shuffle(std::begin(vec), std::end(vec), engine);
        std::move(std::begin(vec), std::end(vec), out);
    }
};

template<typename Sorter>
void test(const char* name)
{
    const int size = 491;

    std::vector<std::string> collection;
    collection.reserve(size);
    auto distribution = shuffled_string{};
    distribution(std::back_inserter(collection), size, -125);

    auto copy = collection;
    std::sort(std::begin(copy), std::end(copy));

    std::cout << std::boolalpha << name << '\n';
    auto sorter = Sorter{};
    sorter(collection);
    std::cout << "is the collection sorted? ";
    std::cout << std::is_sorted(std::begin(collection), std::end(collection)) << '\n';
    std::cout << "is it the same as the one sorted with std::sort? ";
    std::cout << (collection == copy) << '\n';
    std::cout << "were some elements altered? ";
    auto copy2 = collection;
    std::sort(std::begin(collection), std::end(collection));
    std::cout << (collection != copy) << '\n';

    std::cout << '\n'
        << "dis: " << cppsort::probe::dis(copy2) << '\n'
        << "enc: " << cppsort::probe::enc(copy2) << '\n'
        << "exc: " << cppsort::probe::exc(copy2) << '\n'
        << "ham: " << cppsort::probe::ham(copy2) << '\n'
        << "inv: " << cppsort::probe::inv(copy2) << '\n'
        << "max: " << cppsort::probe::max(copy2) << '\n'
        << "mono: " << cppsort::probe::mono(copy2) << '\n'
        << "osc: " << cppsort::probe::osc(copy2) << '\n'
        << "par: " << cppsort::probe::par(copy2) << '\n'
        << "rem: " << cppsort::probe::rem(copy2) << '\n'
        << "runs: " << cppsort::probe::runs(copy2) << '\n'
        << '\n';

    if (size < 40) {
        std::cout << "unsorted collection:\n";
        for (const auto& elem: copy2) {
            std::cout << elem << ' ';
        }
        std::cout << "\n\n";
    }
}

int main()
{
    test<cppsort::poplar_sorter2>("poplar_sort");
}
