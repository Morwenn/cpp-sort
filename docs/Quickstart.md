The core idea behind **cpp-sort** is rather simple: providing a collection of sorting algorithms, along with a few tools allowing to make the most of them. This page describes the most important parts of the library and how to effectively use them.

Before jumping in, you might want to install the library: you can use [CMake][cmake] and/or [Conan][conan] for the task, as described in the [tooling page][tooling] of this documentation.

## Sorting algorithms

Arguably the most basic task one might want to perform is to sort a collection of elements with a sorting algorithm of their choice. This can be achieved as follows:

```cpp
#include <vector>
#include <cpp-sort/sorters/pdq_sorter.h>

int main()
{
    std::vector<int> collection = { /* ... */ };
    cppsort::pdq_sort(collection);
}
```

The library provides dozens of sorting algorithms described in [this page][sorters]. Their space and time complexities are all documented there, as well as their stability and the kind of iterators they work on. The [`pdq_sort`][pdq-sorter] algorithm used above only handles collections that provide random-access iterators, but other algorithms such as [`drop_merge_sort`][drop-merge-sorter] or [`merge_sort`][merge-sorter] work with bidirectional iterators and forward iterators.

The algorithms are designed after [`std::sort`][std-sort] with extensions borrowed from later standards, and as such accept the following arguments:
* A collection, which is represented by either a range or a pair of iterators.
* A comparison function (optional).
* A projection function (optional).

The following example demonstrates how a sorter can be used to sort a collection in a variety of ways:

```cpp
#include <functional>
#include <list>
#include <string>
#include <tuple>
#include <cpp-sort/comparators/flip.h>
#include <cpp-sort/sorters/quick_sorter.h>

struct Person {
    std::string name;
    int age;

    friend bool operator<(Person const& lhs, Person const& rhs) {
        return std::tie(lhs.name, lhs.age) < std::tie(rhs.name, rhs.age);
    }
}

int main()
{
    Person[100] persons = { /* ... */ };
    using cppsort::quick_sort;

    // Sort using operator<(Person, Person)
    quick_sort(persons);
    // Sort in reverse order usinig operator<(Person, Person)
    quick_sort(persons, cppsort::flip(std::less<>{}));
    // Sort the first half of the collection from youngest to oldest
    quick_sort(std::begin(persons), persons+50, &Person::age);
    // Sort the second half of the collection from older to youngest
    quick_sort(persons+50, std::end(persons), std::greater<>{}, &Person::age);

    std::list<Person> li(std::begin(persons), std::end(persons));
    // quick_sort also works on bidirectional (and forward) iterators
    quick_sort(li.begin(), li.end());
    // Though passing the container directly allows to use the size
    // information directly, allowing to perform less work
    quick_sort(li);
}
```

Most of the provided sorting algorithms are comparison sorts, but a few others such as [`ska_sort`][ska-sorter] do not support comparisons at all. Almost all sorters support projections. All the variations of the collection/comparison/projection triplet supported by the sorters are known as the *unified sorting interface*.

## Sorters

Every sorting algorithm in the library is actually an instance of a *sorter* object. For example `cppsort::quick_sort` is an instance of `cppsort::quick_sorter`. Using function objects for algorithms has a few advantages: it makes it easier to pass complete overload sets around, and also allows to separate sorter-specific arguments from the *unified sorting interface*. This design is philosophically similar to that of the [standard library searchers][std-searchers] and ensures that all sorters, once constructed, can be called in a similar fashion, in turn allowing to write functions that generically manipulate sorter objects.

For example, a function to count the number of comparisons performed by a sorter while sorting a collection can be written as follows:

```cpp
template<typename Sorter, typename Iterator>
int sort_count_comparisons(Sorter& sorter, Iterator first, Iterator last)
{
    int count = 0;
    auto cmp = [&count](auto const& lhs, auto const& rhs) {
        ++count;
        return lhs < rhs;
    };
    sorter(first, last, cmp);
    return count;
}
```

All sorters described in the list linked in the previous section are actually stateless and default-constructible, but nothing prevents a custom sorter from taking parameters during construction and holding a state. Stateless sorters, however, do have an advantage over stateful ones: they can be converted to any function pointer corresponding to one of its `operator()` overload:

```cpp
void(*sort)(std::forward_list<int>&, std::negate<>) = cppsort::mel_sort;
```

Sorters are generally implemented using the library's [`sorter_facade`][sorter-facade], which generates most of the boilerplate needed for a sorter to implement the *unified sorting interface*. Its documentation is useful to understand all the combinations of parameters accepted by the library's sorters, but it isn't a component that you should use explicitly unless you are building your own *sorters* or *sorter adapters*.

## Sorters adapters

Building on the ideas of *sorters*, [*sorter adapters*][sorter-adapters] are function objects provided by the library that are constructed with a *sorter* object and are themselves a *sorter*. This pattern can be used to solve a variety of issues, for example:

* Sort a list by moving its elements to a contiguous memory buffer and sorting them there before moving them back, allowing to use any sorter that accepts random-access iterators:
    ```cpp
    #include <list>
    #include <cpp-sort/adapters/out_of_place_adapter.h>
    #include <cpp-sort/sorters/smooth_sorter.h>

    int main()
    {
        auto sorter = cppsort::out_of_place_adapter<cppsort::smooth_sorter>{};
        std::list<int> collection = { /* ... */ };
        sorter(collection);
    }
    ```

* Reduce the cost of expensive projections with a [Schwartzian transform][schwartzian-transform]:
    ```cpp
    #include <vector>
    #include <cpp-sort/adapters/schwartz_adapter.h>
    #include <cpp-sort/sorters/poplar_sorter.h>

    int main()
    {
        auto sorter = cppsort::schwartz_adapter<cppsort::poplar_sorter>{};
        std::vector<int> collection = { /* ... */ };
        sorter(collection, expensive_projection);
    }
    ```

* Minimze the number of element moves by sorting a collection of iterators, and moving all the elemrnts directly to their sorted position:
    ```cpp
    #include <vector>
    #include <cpp-sort/adapters/indirect_adapter.h>
    #include <cpp-sort/sorters/pdq_sorter.h>

    int main()
    {
        auto sorter = cppsort::indirect_adapter<cppsort::pdq_sorter>{};
        std::vector<huge_class> collection = { /* ... */ };
        sorter(collection);
    }
    ```

* Make any sorter [*Rem*-adaptive][probe-rem] with an additional O(n) pass and a merge:
    ```cpp
    #include <vector>
    #include <cpp-sort/adapters/split_adapter.h>
    #include <cpp-sort/sorters/quick_merge_sorter.h>

    int main()
    {
        auto sorter = cppsort::split_adapter<cppsort::quick_merge_sorter>{};
        std::vector<int> collection = { /* ... */ };
        sorter(collection);
    }
    ```

Almost any sorter can be passed to any adapter, with a few exceptions:
* The iterator categories have to match.
* Some adapters require the *adapted sorter* to handle comparisons.
* Some adapters requires *adapted sorter* to handle projections.
* Fixed-size sorter adapters only adapt fixed-size sorters.

The specific restrictions are all documented in the adapters descriptions.

## Two-step sorting

Sometimes the information is not represented as simple collection of class instances, but as [parallel arrays][parallel-arrays] (also known as structure of arrays). To sort those, **cpp-sort** provides components for two-step sorting of random-access collections:
1. Extract the sorted indices of a collection with [`utility::sorted_indices`][utility-sorter-indices] (similar to [`numpy.argsort`][numpy-argsort]).
2. Use the sorted indices to permute the arrays with [`utility::apply_permutation`][utility-apply-permutation].

```cpp
// Names & ages of persons, where the identity of the person is the
// same index in both collections
std::vector<std::string> names = { /* ... */ };
std::vector<int> ages = { /* ... */ };

auto get_sorted_indices_for = cppsort::utility::sorted_indices<cppsort::poplar_sorter>{};
auto indices = get_sorted_indices_for(names); // Get sorted indices to sort by name
// Bring persons in sorted order
cppsort::utility::apply_permutation(names, auto(indices));
cppsort::utility::apply_permutation(ages, indices);
```

This method allows to call a O(n log n) sorting agorithm once, and to call the O(n) `apply_permutation` once per array to bring into sorted order. Do note however that `apply_permutation` also alters the indices collection, which has to be copied for each array to bring into sorted order.

## Measures of presortedness

**cpp-sort** also provides a collection of [measures of presortedness][mops]: also known as *measures of disorder*, they are algorithms that evaluate the amount of disorder present in a collection - the exact algorithms used are described in the corresponding documentation page, and mostly come from the literature around adaptive sorting. These measures are provided as function objects implementing the *unified sorting interface* and returning an integer type. The exact value returned depends on the measure used, but all of them return 0 for a sorted collection.

```cpp
#include <iostream>
#include <vector>
#include <cpp-sort/probes.h>

int main()
{
    std::vector<long> collection = { /* ... */ };
    std::cout << "Number of inversions: " << cppsort::probe::inv(collection) << '\n'
              << "Number of runs: " << cppsort::probe::runs(collection) << '\n'
              << "Number of encroaching lists: " << cppsort::probe::enc(collection) << std::endl;
}
```

For a measure of presortedness *M*, some algorithms are said to be *M*-adaptive when they naturally take advantage of the particular kind of presortedness described by *M* to perform proportionally less work. Analyzing the presortedness patterns found in real data can therefore be useful to pick an adaptive sorting algorithm tailored for the job.

## Going further

The previous sections describe some of the main tools provided by **cpp-sort** but that list is far from being exhaustive: the library contains more sorting-adjacent utilities, as well as tools that are meant to help you build your own sorters or sorter adapters. This documentation tries to cover most of the useful concepts and to describe the public-facing library components, so don't hesitate to open an issue if you feel that something is missing or unclear.


  [cmake]: https://cmake.org/
  [conan]: https://conan.io/
  [drop-merge-sorter]: Sorters.md#drop_merge_sorter
  [merge-sorter]: Sorters.md#merge_sorter
  [mops]: Measures-of-presortedness.md
  [numpy-argsort]: https://numpy.org/doc/stable/reference/generated/numpy.argsort.html
  [parallel-arrays]: https://en.wikipedia.org/wiki/Parallel_array
  [pdq-sorter]: Sorters.md#pdq_sorter
  [probe-rem]: Measures-of-presortedness.md#rem
  [schwartzian-transform]: https://en.wikipedia.org/wiki/Schwartzian_transform
  [ska-sorter]: Sorters.md#ska_sorter
  [sorter-adapters]: Sorter-adapters.md
  [sorter-facade]: Sorter-facade.md
  [sorter-traits]: Sorter-traits.md
  [sorters]: Sorters.md
  [std-searchers]: https://en.cppreference.com/w/cpp/utility/functional#Searchers
  [std-sort]: https://en.cppreference.com/w/cpp/algorithm/sort
  [tooling]: Tooling.md
  [utility-apply-permutation]: https://github.com/Morwenn/cpp-sort/wiki/Miscellaneous-utilities#apply_permutation
  [utility-sorted-indices]: https://github.com/Morwenn/cpp-sort/wiki/Miscellaneous-utilities#sorted_indices
