# Sorters

**cpp-sort** uses function objects called *sorters* instead of regular function
templates in order to implement sorting algorithms. It defines two concepts
correspoding to two kinds of sorters: `Sorter` and `ComparisonSorter`.

A type satisfies the `Sorter` concept if it can be called on a `ForwardIterable&`
object, where a type satisfies the `ForwardIterable` concept if and only if calls
to `std::begin` and `std::end` on this object return instances of a type satisfying
the [`ForwardIterator`](http://en.cppreference.com/w/cpp/concept/ForwardIterator)
concept (since the iterable is sorted in-place and thus altered, sorting an
`InputIterable` type is not possible). To satisfy the `Sorter` concept, a type also
has to provide an overload of `operator()` which takes a pair of `ForwardIterator`.
A `Sorter` instance should additionally be convertible to `Ret(*)(ForwardIterable&)`
and `Ret(*)(ForwardIterator, ForwardIterator)` where `Ret` is the return type of
the operation.

A type satisfies the `ComparisonSorter` concept if it satisfies the `Sorter`
concept and can additionally be called with another parameter satisfying the
[`Compare`](http://en.cppreference.com/w/cpp/concept/Compare) concept. While
most sorters satisfy this concept, some of them might implement non-comparison
based sorting algorithms such as radix sort, and thus only satisfy the `Sorter`
concept. A `ComparisonSorter` instance should additionally be convertible to
`Ret(*)(ForwardIterable&, Compare)` and `Ret(*)(ForwardIterator, ForwardIterator, Compare)`
where `Ret` is the return type of the operation.

Implementing a sorter is easy once you have a sorting algorithm: simply add an
`operator()` overload that forwards its values to a sorting function. If your
sorter supports custom comparison functions, make sure that it can also be
called *without* such a function. A `ComparisonSorter` should be usable wherever
a `Sorter` is usable.

```cpp
#include <functional>
#include <iterator>
#include <cpp-sort/sorter_base.h>
#include "spam_sort.h"

/**
 * ComparisonSorter implementing a spam_sort algorithm, where spam_sort
 * is a sorting algorithm working with bidirectional iterators.
 */
struct spam_sorter:
    cppsort::sorter_base<spam_sorter>
{
    using cppsort::sorter_base<spam_sorter>::operator();

    template<
        typename BidirectionalIterator,
        typename Compare = std::less<>
    >
    auto operator()(BidirectionalIterator first,
                    BidirectionalIterator last,
                    Compare cmp={}) const
        -> void
    {
        spam_sort(std::begin(iterable), std::end(iterable), cmp);
    }
};
```

In the previous example, [`sorter_base`](sorter-base.md) is a CRTP base class that
is used to provide the function pointer conversion operators to the sorter. It also
generates the range overloads of `operator()`. If you want your sorter to integrate
seamlessly into the library, you should additionally specialize the template class
[`cppsort::sorter_traits`](sorter-traits.md) for it:

```cpp
namespace cppsort
{
    template<>
    struct sorter_traits<spam_sorter>
    {
        using iterator_category = std::bidirectional_iterator_tag;
        static constexpr bool is_stable = true;
    };
}
```

While these function objects offer little more than regular sorting functions by
themselves, you can use them together with [*sorter adapaters*](sorter-adapters.md)
to craft more elaborate sorters effortlessly. Every sorter is available in its own
file. However, you can also include all the available sorters at once with the
following line:

```cpp
#include <cpp-sort/sorters.h>
```

## Generic sorters

The following sorters are available and will work with any type for which `std::less`
works:

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

## Type-specific sorters

The following sorters are available but will only work for some specific types:

### `spread_sorter`

```cpp
#include <cpp-sort/sorters/spread_sorter.h>
```

`spread_sorter` impements a [spreadsort](https://en.wikipedia.org/wiki/Spreadsort)
and comes into three main flavours:

* `integer_spread_sorter` works with any type satisfying `std::is_integral`.
* `float_spread_sorter` works with any type satisfying `std::numeric_limits::is_iec559`.
* `string_spread_sorter` works with `std::string` and `std::wstring`.

    Best        Average     Worst       Memory      Stable      Iterators
    ?           n*(k/d)     n*(k/s+d)   n*(k/d)     No          Random access

These three sorters are aggregated into one main sorter the following way:

```cpp
using spread_sorter = hybrid_adapter<
    integer_spread_sorter,
    float_spread_sorter,
    string_spread_sorter
>;
```
