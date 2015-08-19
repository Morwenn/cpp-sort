Sorters
=======

A *sorter* is a function object class whose `operator()` is overloaded to take
an `Iterable&` collection and a `Compare` functor. It is easy to write a new
sorter by yourself:

```cpp
/**
 * Sorter implementing a spam_sort algorithm.
 */
struct spam_sorter
{
    template<typename Iterable, typename Compare>
    auto operator()(Iterable& iterable, Compare cmp) const
        -> void
    {
        spam_sort(std::begin(iterable), std::end(iterable), cmp);
    }
};
```

`Iterable` has to be a type on which `std::begin` and `std::end` can be used.
`Compare` should be a type satisfying the [`Compare`](http://en.cppreference.com/w/cpp/concept/Compare)
concept.

While these funnction objects offer little more than regular sorting functions by
themselves, you can use them together with [*sorter adapaters*](sorter-adapters.md)
to craft more elaborate sorters effortlessly. Every sorter is available in its own
file. However, you can also include all the availble sorters at once with the
following line:

```cpp
#include <cpp-sort/sorters.h>
```

Note that the library does not make a difference between sorters and sorter adapaters,
so the adapters will also be included by the lie above. The following non-adapter
sorters are available:

`default_sorter`
----------------

```cpp
#include <cpp-sort/sorters/default_sorter.h>
```

Sorter striving to use a sorting algorithm as optimized as possible. The current
implementation defines it as follows:

```cpp
using default_sorter = self_sorter<
    small_array_sorter<
        pdq_sorter,
        std::make_index_sequence<10u>
    >
>;
```

`insertion_sorter`
------------------

```cpp
#include <cpp-sort/sorters/insertion_sorter.h>
```

Implements an [insertion sort](https://en.wikipedia.org/wiki/Insertion_sort).

    Best        Average     Worst       Memory      Stable
    n           n²          n²          1           Yes

`merge_sorter`
--------------

```cpp
#include <cpp-sort/sorters/merge_sorter.h>
```

Implements a [merge sort](https://en.wikipedia.org/wiki/Merge_sort).

    Best        Average     Worst       Memory      Stable
    n log n     n log n     n log n     n           Yes

`pdq_sorter`
------------

```cpp
#include <cpp-sort/sorters/pdq_sorter.h>
```

Implements a [pattern-defeating quicksort](https://github.com/orlp/pdqsort).


    Best        Average     Worst       Memory      Stable
    n           n log n     n log n     log n       No

`std_sorter`
------------

```cpp
#include <cpp-sort/sorters/std_sorter.h>
```

Uses the standard library [`std::sort`](http://en.cppreference.com/w/cpp/algorithm/sort)
to sort a collection.

    Best        Average     Worst       Memory      Stable
    ?           n log n     n log n     ?           No

`tim_sorter`
------------

```cpp
#include <cpp-sort/sorters/tim_sorter.h>
```

Implements a [timsort](https://en.wikipedia.org/wiki/Timsort) algorithm.

    Best        Average     Worst       Memory      Stable
    n           n log n     n log n     n           Yes
