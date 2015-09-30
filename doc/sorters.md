# Sorters

**cpp-sort** uses function objects called *sorters* instead of regular function
templates in order to implement sorting algorithms. The library provides two
categories of sorters: generic sorters that will sort a collection with any given
comparison function, and type-specific sorters which will be optimized to sort
collections of a given type, and generally don't allow to use custom comparison
functions due to the way they work.

While these function objects offer little more than regular sorting functions by
themselves, you can use them together with [*sorter adapters*](sorter-adapters.md)
to craft more elaborate sorters effortlessly. Every sorter is available in its own
file. However, you can also include all the available sorters at once with the
following line:

```cpp
#include <cpp-sort/sorters.h>
```

If you want to read more about sorters and/or write your own one, then you should
have a look at [the dedicated page](writing-sorter.md).

## Generic sorters

The following sorters are available and will work with any type for which `std::less`
works and should accept any well-formed comparison function:

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
            merge_sorter,
            rebind_iterator_category<
                quick_sorter,
                std::bidirectional_iterator_tag
            >,
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

### `insertion_sorter`

```cpp
#include <cpp-sort/sorters/insertion_sorter.h>
```

Implements an [insertion sort](https://en.wikipedia.org/wiki/Insertion_sort).

    Best        Average     Worst       Memory      Stable      Iterators
    n           n²          n²          1           Yes         Forward

### `merge_sorter`

```cpp
#include <cpp-sort/sorters/merge_sorter.h>
```

Implements a [merge sort](https://en.wikipedia.org/wiki/Merge_sort).

    Best        Average     Worst       Memory      Stable      Iterators
    n log n     n log n     n log n     n           Yes         Bidirectional
    n log n     n log n     n²          n           Yes         Forward

**Note:** the worst-case complexity of n² for forward iterators may only be
reached when no additional memory can be allocated. That means that the sort
won't ever have the worst-case memory *and* the worst-case complexity at the
same time.

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
    n log n     n log n     n²          n           No          Forward

### `selection_sorter`

```cpp
#include <cpp-sort/sorters/selection_sorter.h>
```

Implements a [selection sort](https://en.wikipedia.org/wiki/Selection_sort).


    Best        Average     Worst       Memory      Stable      Iterators
    n²          n²          n²          1           No          Forward

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

Implements a [timsort](https://en.wikipedia.org/wiki/Timsort).

    Best        Average     Worst       Memory      Stable      Iterators
    n           n log n     n log n     n           Yes         Random access

### `verge_sorter`

```cpp
#include <cpp-sort/sorters/verge_sorter.h>
```

Implements a [vergesort](https://github.com/Morwenn/vergesort).

    Best        Average     Worst       Memory      Stable      Iterators
    n           n log n     n log n     n           No          Random access
    n           n log n     n²          n           No          Bidirectional

**Note:** while the worst-case complexity is n², I don't even know how to reach
such a case: the fallback quicksort tends to have its worst-case complexity for
patterns and the vergesort layer defeats such patterns. Therefore, it *may* be
impossible for this sort to have a quadratic behavior. If you ever find a way to
trigger a quadratic behavior, don't hesitate to report it.

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
* `string_spread_sorter` works with `std::string` and `std::wstring` (if `wchar_t` is 2 bytes).

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
