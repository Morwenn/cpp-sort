# Sorter adapters

Sorter adapters are the main reason for using sorter function objects instead
of regular functions. A *sorter adapter* is a class template that takes another
`Sorter` template parameter and alters its behavior. The resulting class can be
used as a regular sorter, and be adapted in turn. Note that some of the adapters
are actually [fixed-size sorter](fixed-sorters.md) adapters instead of regular
sorter adapters. It is possible to include all the available adapters at once with
the following directive:

```cpp
#include <cpp-sort/adapters.h>
```

In this documentation, we will call *adapted sorters* the sorters passed to the
adapters and *resulting sorter* the sorter class that results from the adaption
of a sorter by an adapter.

## Available sorter adapters

The following sorter adapters and fixed-size sorter adapters are available in the
library:

### `counting_adapter`

```cpp
#include <cpp-sort/adapters/counting_adapter.h>
```

Unlike regular sorters, `counting_adapter::operator()` does not return `void` but
the number of comparisons that have been needed to sort the iterable. It will
adapt the comparison functor so that it can count the number of comparisons
made by any other sorter with a reasonable implementation. The actual number of
comparisons needed to sort an interable can be used as a heuristic in hybrid sorts
and may constitute an interesting information nevertheless.

The actual counter type can be configured with the template parameter `CountType`,
which defaults to `std::size_t` if not specified. While this doesn't matter most of
the times, this parameter may be changed to `std::atomic<std::size_t>` to count the
number of comparisons performed by a parallel sorting algorithm (or to a reducer,
which would probably be a better idea).

```cpp
template<
    typename ComparisonSorter,
    typename CountType = std::size_t
>
struct counting_adapter;
```

The iterator category and the stability of the *resulting sorter* are those of the
*adapted sorter*. Note that this adapter only works with sorters that satisfy the
`ComparisonSorter` concept since it needs to adapt a comparison function.

### `hybrid_adapter`

```cpp
#include <cpp-sort/adapters/hybrid_adapter.h>
```

The goal of this sorter adapter is to aggregate several sorters into one unique
sorter. The new sorter will call the appropriate sorting algorithm based on the
iterator category of the iterable to sort. If several of the aggregated sorters
have the same iterator category, the first to appear in the template parameter
list will be the chosen one, unless some SFINAE condition prevents it from being
used. As long as the iterator categories are different, the order of the sorters
in the parameter pack does not matter.

For example, the following sorter will call a pattern-defeating quicksort to sort
a random-access iterable, an insertion sort to sort a bidirectional iterable and
a bubble sort to sort a forward iterable:

```cpp
using general_purpose_sorter = hybrid_adapter<
    bubble_sorter,
    insertion_sorter,
    pdq_sorter
>;
```

This adapter uses `cppsort::iterator_category` to check the iterator category of 
the sorters to aggregate. Therefore, if you write a sorter and want it to be usable
with `hybrid_adapter`, you will need to specialize `cppsort::sorter_traits` for
your sorter. If you write specific sorters that only work with some specific types,
you might want to SFINAE out the overloads of `operator()` when they are not valid
instead of triggering a hard error. Doing so will allow to use them with fallback
sorters in `hybrid_adapter` to handle the cases where the type to sort cannot be
handled by your sorter.

The stability of the *resulting sorter* is `true` if and only if the stability
of every *adapter sorter* is `true`. The iterator category of the *resulting
sorter* is the most permissive iterator category among the the *adapted sorters*.

### `self_sort_adapter`

```cpp
#include <cpp-sort/adapters/self_sort_adapter.h>
```

This adapter takes a sorter and, if the object to be sorted has a `sort` method,
it is used to sort the object. Otherwise, the *adapted sorter* is used instead to
sort the iterable.

This sorter adapter allows to support out-of-the-box sorting for `std::list` and
`std::forward_list` as well as other user-defined classes that implement a `sort`
method.

```cpp
template<typename Sorter>
struct self_sort_adapter;
```

Since it is impossible to guarantee the stability of the `sort` method of a
given iterable, the stability of the *resulting sorter* is always `false`.
The iterator category of the *resulting sorter* is that of the *adapted sorter*.

### `small_array_adapter`

```cpp
#include <cpp-sort/adapters/small_array_adapter.h>
```

This adapter is not a regular sorter adapter, but a *fixed-size* sorter adapter.
It wraps a fixed-size sorter and calls it whenever it is passed a fixed-size C
array or an `std::array`.

```cpp
template<
    template<std::size_t> class FixedSorter,
    typename Indices = void
>
struct small_array_adapter;
```

The `Indices` parameter may be either `void` or a specialization of the standard
class template `std::index_sequence`. If it is `void`, `small_array_adapter` will
try to call the underlying fixed-size sorter for C arrays or `std::array` instances
of any size. If an `std::index_sequence` specialization is given instead, then the
adapter will try to call the underlying fixed-size sorter only if the size of the
array to be sorted is appear in the index sequence. The `operator()` overloads are
SFINAEd out if these conditions are not fulfilled.

These soft errors allow `small_array_adapter` to play nice with `hybrid_adapter`.
For example, if one wants to call `low_moves_sorter` when a sorter is given an
array of size 0 to 8 and a `pdq_sorter` otherwise, they could easily create an
appropriate sorter the following way:

```cpp
using sorter = cppsort::hybrid_adapter<
    cppsort::small_array_adapter<
        cppsort::low_moves_sorter,
        std::make_index_sequence<9>
    >,
    cppsort::pdq_sorter
>;
```
