# Sorters

**cpp-sort** uses function objects called *sorters* instead of regular function
templates in order to implement sorting algorithms. It defines two concepts
correspoding to two kinds of sorters: `Sorter` and `ComparisonSorter`.

A type satisfies the `Sorter` concept if it can be called on a `ForwardIterable&`
object, where a type satisfies the `ForwardIterable` concept if and only if calls
to `std::begin` and `std::end` on this object return instances of a type satisfying
the [`ForwardIterator`](http://en.cppreference.com/w/cpp/concept/ForwardIterator)
concept (since the iterable is sorted in-place and thus altered, sorting an
`InputIterable` type is not possible). A `Sorter` instance should additionally be
convertible to `Ret(*)(ForwardIterable&)` where `Ret` is the return type of the
operation.

A type satisfies the `ComparisonSorter` concept if it satisfies the `Sorter`
concept and can additionally be called with a second parameter satisfying the
[`Compare`](http://en.cppreference.com/w/cpp/concept/Compare) concept. While
most sorters satisfy this concept, some of them might implement non-comparison
based sorting algorithms such as radix sort, and thus only satisfy the `Sorter`
concept. A `ComparisonSorter` instance should additionally be convertible to
`Ret(*)(ForwardIterable&, Compare)` where `Ret` is the return type of the
operation.

Implementing a sorter is easy once you have a sorting algorithm: simply add an
`operator()` overload that forwards its values to a sorting function.

```cpp
#include <cpp-sort/sorter_base.h>

/**
 * ComparisonSorter implementing a spam_sort algorithm, where spam_sort
 * is a sorting algorithm working with bidirectional iterators.
 */
struct spam_sorter:
    cppsort::sorter_base<spam_sorter>
{
    template<typename BidirectionalIterable, typename Compare>
    auto operator()(BidirectionalIterable& iterable, Compare cmp) const
        -> void
    {
        spam_sort(std::begin(iterable), std::end(iterable), cmp);
    }
};
```

In the previous example, `sorter_base` is a CRTP base class that is used to provide
the function pointer conversion operators to the sorter.

While these function objects offer little more than regular sorting functions by
themselves, you can use them together with [*sorter adapaters*](sorter-adapters.md)
to craft more elaborate sorters effortlessly. Every sorter is available in its own
file. However, you can also include all the available sorters at once with the
following line:

```cpp
#include <cpp-sort/sorters.h>
```

## Available comparison sorters

The following comparison sorters are available in the library:

### `default_sorter`

```cpp
#include <cpp-sort/sorters/default_sorter.h>
```

Sorter striving to use a sorting algorithm as optimized as possible. The current
implementation defines it as follows:

```cpp
using default_sorter = self_sort_adapter<
    small_array_adapter<
        hybrid_adapter<
            inplace_merge_sorter,
            insertion_sorter,
            pdq_sorter
        >,
        std::make_index_sequence<10u>
    >
>;
```

### `heap_sorter`

```cpp
#include <cpp-sort/sorters/heap_sorter.h>
```

Implements a [heapsort](https://en.wikipedia.org/wiki/Heapsort).

    Best        Average     Worst       Memory      Stable      Iterators
    n log n     n log n     n log n     1           No          Random access

### `inplace_merge_sorter`

```cpp
#include <cpp-sort/sorters/inplace_merge_sorter.h>
```

Implements an in-place merge sort.

    Best        Average     Worst       Memory      Stable      Iterators
    ?           ?           n log² n    1           Yes         Forward

### `insertion_sorter`

```cpp
#include <cpp-sort/sorters/insertion_sorter.h>
```

Implements an [insertion sort](https://en.wikipedia.org/wiki/Insertion_sort).

    Best        Average     Worst       Memory      Stable      Iterators
    n           n²          n²          1           Yes         Bidirectional

### `merge_sorter`

```cpp
#include <cpp-sort/sorters/merge_sorter.h>
```

Implements a [merge sort](https://en.wikipedia.org/wiki/Merge_sort).

    Best        Average     Worst       Memory      Stable      Iterators
    n log n     n log n     n log n     n           Yes         Random access

### `pdq_sorter`

```cpp
#include <cpp-sort/sorters/pdq_sorter.h>
```

Implements a [pattern-defeating quicksort](https://github.com/orlp/pdqsort).


    Best        Average     Worst       Memory      Stable      Iterators
    n           n log n     n log n     log n       No          Random access

### `quick_sorter`

```cpp
#include <cpp-sort/sorters/quick_sorter.h>
```

Implements a [quicksort](https://en.wikipedia.org/wiki/Quicksort).


    Best        Average     Worst       Memory      Stable      Iterators
    n long n    n log n     n²          n           No          Bidirectional

### `std_sorter`

```cpp
#include <cpp-sort/sorters/std_sorter.h>
```

Uses the standard library [`std::sort`](http://en.cppreference.com/w/cpp/algorithm/sort)
to sort a collection.

    Best        Average     Worst       Memory      Stable      Iterators
    ?           n log n     n log n     ?           No          Random access

### `tim_sorter`

```cpp
#include <cpp-sort/sorters/tim_sorter.h>
```

Implements a [timsort](https://en.wikipedia.org/wiki/Timsort) algorithm.

    Best        Average     Worst       Memory      Stable      Iterators
    n           n log n     n log n     n           Yes         Random access

### `verge_sorter`

```cpp
#include <cpp-sort/sorters/verge_sorter.h>
```

Implements a [vergesort](https://github.com/Morwenn/vergesort).


    Best        Average     Worst       Memory      Stable      Iterators
    n           n log n     n log n     n           No          Random access
