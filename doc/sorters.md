Sorters
=======

A *sorter* is a function object class whose `operator()` is overloaded to
take a `RandomAccessIterable&` and a `Compare` functor. It is easy to write
one yourself:

```cpp
/**
 * Sorter implementing a spam_sort algorithm.
 */
struct spam_sorter
{
    template<typename RandomAccessIterable, typename Compare>
    auto operator()(RandomAccessIterable& iterable, Compare cmp) const
        -> void
    {
        spam_sort(std::begin(iterable), std::end(iterable), cmp);
    }
};
```

`RandomAccessIterable` has to be a type for which `std::begin` and `std::end`
return [random-access iterators](http://en.cppreference.com/w/cpp/concept/RandomAccessIterator).
`Compare` should be a type satisfying the [`Compare`](http://en.cppreference.com/w/cpp/concept/Compare)
concept.

While these objects offer little more than regular sorting functions by themselves,
you can use them with [*sorter adapaters*](sorter-adapters.md) to craft more
elaborate sorters effortlessly. Every sorter is available from in its own file.
However, you can also include all the availble sorters at once with the following
include:

```cpp
#include <cpp-sort/sorters.h>
```

The following sorters are available:

`pdq_sorter`
------------

```cpp
#include <cpp-sort/sorters/pdq_sorter.h>
```

Implements a [pattern-defeating quicksort](https://github.com/orlp/pdqsort).

`tim_sorter`
------------

```cpp
#include <cpp-sort/sorters/tim_sorter.h>
```

Implements a [timsort](https://en.wikipedia.org/wiki/Timsort) algorithm.

`std_sorter`
------------

```cpp
#include <cpp-sort/sorters/std_sorter.h>
```

Uses the standard library [`std::sort`](http://en.cppreference.com/w/cpp/algorithm/sort)
to sort a collection.

`default_sorter`
----------------

```cpp
#include <cpp-sort/sorters/default_sorter.h>
```

Sorter striving to use a sorting algorithm as optimized as possible. The current
implementation defines it as follows:

```cpp
using default_sorter = small_array_sorter<
    pdq_sorter,
    std::make_index_sequence<10u>
>;
```
