# Sorter adapters

Sorter adapters are the main reason for using sorter function objects instead
of regular functions. A *sorter adapter* is a class template that takes another
`Sorter` template parameter and alters its behavior. The resulting class can be
used as a regular sorter, and be adapted in turn. It is possible to include all
the available adapters at once with the following directive:

```cpp
#include <cpp-sort/adapters.h>
```

In this documentation, we will call *adapted sorters* the sorters passed to the
adapters and *resulting sorter* the sorter class that results from the adaption
of a sorter by an adapter.

## Available sorter adapters

The following sorter adapters are available in the library:

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
which defaults to `std::size_t` if not specified.

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
the sorters to aggregate. Therefore, if you write a sorter, you will need to
specialize `cppsort::sorter_traits` if you want it to be usable with this adapter.

The stability of the *resulting sorter* is `true` if and only if the stability
of every *adapter sorter* is `true`. The iterator category of the *resulting
sorter* is the most permissive iterator category among the the *adapted sorters*.

### `low_comparisons_adapter`

```cpp
#include <cpp-sort/adapters/low_comparisons_adapter.h>
```

This adapter implements small array sorts whose goal is to keep the number of
comparisons at its lowest, which may be useful when comparisons are expensive
but moves are cheap (large strings with common prefixes for example). To know
which sorting algorithm has the lowest overall number of comparisons for an
array of a given size, the following method is used: count the total number of
comparisons needed to sort every permutation of the array, we will call this
the *comparison weight* of the array.

The specific sorting algorithms will only be called if the *resulting sorter*
is given a fixed-size C array or an `std::array`. Otherwise it will call the
fallback sorter if one has been given or cause a compile-time error.

The following table illustrates the *comparison weight* of the algorithms used
by the different specializations of `low_comparisons_adapter`. If you ever find
an algorithm that beats one of those, do not hesitate to conribute:

Size | Comparison weight
---- | -----------------
0 | 0
1 | 0
2 | 2
3 | 16
4 | 118
5 | 840
6 | 7440
7 | 66720
8 | 696000
9 | 7565760
10 | 92718720
11 | 1190033280
12 | 16854704640
13 | 249831406080

This adapter uses a variety of specialized sorting algorithms depending on the
size of the array to sort. Many of them correspond to unrolled versions of more
general sorts. The following table describes the algorithms used:

Size | Algorithm
---- | ---------
0 | Nothing
1 | Nothing
2 | Compare and swap
3 | Insertion sort
4 | Insertion sort
5 | Algorithm 5*
6 | Insertion sort
7 | Double insertion sort
8 | Double insertion sort
9 | Double insertion sort
10 | Double insertion sort
11 | Double insertion sort
12 | Double insertion sort
13 | Double insertion sort

I don't know whether the algorithm 5 has a name or not. It is actually an algorithm
that I found [on StackOverflow](http://stackoverflow.com/a/1935491/1364752) before
translating it from LISP to C++. From the answer, it seems that it can be found in
[The Art of Computer Programming](https://en.wikipedia.org/wiki/The_Art_of_Computer_Programming),
volume 3 by Donald Knuth.

The algorithms 7 to 13 use a supposedly a novel sorting algorithm that I named the
*double insertion sort* (I would be suprised if it hadn't been discovered before,
but I couldn't find any information about it anywhere). Actually, the algorithm is
rather simple: it sorts everything but the first and last elements of the array,
switches the first and last elements if they are not ordered, then insert the first
element into the sorted sequence from the front and insert the last element from
the back. Even in the worst case, it shouldn't take more than *n* comparisons to
insert both values, where *n* is the size of the collection.

The simple `low_comparisons_adapter` takes a `Sorter` template parameter so that
it can fall back to that sorter when no specific algorithm exists for the given
array size:

```cpp
template<typename Sorter>
struct low_comparisons_adapter<Sorter>;
```

There is another version that takes an additional `std::index_sequence` template
parameter:

```cpp
template<
    typename Sorter,
    std::size_t... Indices
>
struct low_comparisons_adapter<Sorter, std::index_sequence<Indices...>>;
```

That version calls the specific algorithms of `low_comparisons_adapter` only
if the size of the given array is comprised in the integer sequence. It allows
to pick specific algorithms if they are deemed better than the *adapter sorter*
for the given size.

### `low_moves_adapter`

```cpp
#include <cpp-sort/adapters/low_moves_adapter.h>
```

This adapter implements small array sorts whose goal is to keep the number of
moves performed by the sorting algorithm at its lowest, which may be useful when
comparisons are cheaps but moves are expensive (large objects that only use one
field for the comparison for example). To know which sorting algorithm has the
lowest overall number of move operations performed for an array of a given size,
the following method is used: count the total number of moves needed to sort all
the permutations of the array, we will call this the *move weight* of the array.

The specific sorting algorithms will only be called if the *resulting sorter*
is given a fixed-size C array or an `std::array`. Otherwise it will call the
fallback sorter if one has been given or cause a compile-time error.

The following table illustrates the *move weight* of the algorithms used by the
different specializations of `low_moves_adapter`. If you ever find an algorithm
that beats one of those, do not hesitate to conribute:

Size | Move weight
---- | -----------
0 | 0
1 | 0
2 | 2
3 | 17
4 | 176
5 | 1384
6 | 14396

The algorithms 0 to 4 use an unrolled insertion sort. The algorithms 5 and 6 use a
double insertion sort. This family of algorithms is still a work in progress and
more specializations will come in the future.

The simple `low_moves_adapter` takes a `Sorter` template parameter so that
it can fall back to that sorter when no specific algorithm exists for the given
array size:

```cpp
template<typename Sorter>
struct low_moves_adapter<Sorter>;
```

There is another version that takes an additional `std::index_sequence` template
parameter:

```cpp
template<
    typename Sorter,
    std::size_t... Indices
>
struct low_moves_adapter<Sorter, std::index_sequence<Indices...>>;
```

That version calls the specific algorithms of `low_moves_adapter` only if the size
of the given array is comprised in the integer sequence. It allows to pick specific
algorithms if they are deemed better than the *adapter sorter* for the given size.

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

This sorter adapter comes into two flavors:

```cpp
template<typename Sorter>
struct small_array_adapter<Sorter>;
```

This specialization takes a sorter and uses it to sort the collections that it
receives. However, if the collection is an `std::array` or a fixed-size C array,
it replaces the `Sorter` sort by special algorithms designed to efficiently sort
small arrays of fixed size. While it kind of overlaps `low_comparisons_adapter`
and `low_moves_adapter`, it is different in that it does not use an objective
criteria: it only tries to be faster than anything for « usual » types (most of
the time for types ranging from `char` to `long long int`) and this might depend
on the optimization level of the compiler and on the architecture.

The specific sorting algorithms used by `small_array_adapter` mostly correspond
to [sorting networks](https://en.wikipedia.org/wiki/Sorting_network) of a given
size. There are specialized algorithms for the sizes 0 to 32; the algorithms 5 to
13 (inclusive) correspond to an unrolled double insertion sort while every other
algorithm actually uses sorting networks. The following table documents the number
of comparisons and the number of swaps made by every algorithm:

Size | comparisons | swaps
---- | ----------- | -----
0 | 0 | 0
1 | 0 | 0
2 | 1 | ≤ 1
3 | 3 | ≤ 3
4 | 5 | ≤ 5
5 | 6-9 | ?
6 | 8-12 | ?
7 | 9-17 | ?
8 | 11-21 | ?
9 | 12-27 | ?
10 | 14-32 | ?
11 | 15-39 | ?
12 | 17-45 | ?
13 | 18-53 | ?
14 | 51 | ≤ 51
15 | 56 | ≤ 56
16 | 60 | ≤ 60
17 | 74 | ≤ 74
18 | 82 | ≤ 82
19 | 91 | ≤ 91
20 | 97 | ≤ 97
21 | 107 | ≤ 107
22 | 114 | ≤ 114
23 | 122 | ≤ 122
24 | 127 | ≤ 127
25 | 138 | ≤ 138
26 | 146 | ≤ 146
27 | 155 | ≤ 155
28 | 161 | ≤ 161
29 | 171 | ≤ 171
30 | 178 | ≤ 178
31 | 186 | ≤ 186
32 | 191 | ≤ 191

The code for most of these algorithms has been generated using the `SWAP` macros
generated by http://pages.ripco.net/~jgamble/nw.html with the "Best" algorithm (as
described on the site) for inputs inferior or equal to 16, and Batcher's Merge-Exchange
algorithm otherwise.

There is another specialization of `small_array_adapter` which takes a `Sorter` and
an `std::index_sequence`:

```cpp
template<
    typename Sorter,
    std::size_t... Indices
>
struct small_array_adapter<Sorter, std::index_sequence<Indices...>>;
```

This version of the sorter adapter only uses the specialized sorting algorithms
for fixed-size arrays if the size of the array to sort is contained in `Indices`,
and falls back to `Sorter` in every other case. The main advantage is that it allows
to use the class [`std::make_index_sequence`](http://en.cppreference.com/w/cpp/utility/integer_sequence)
to generate the indices and pick the specialized algorithms for the smallest values
of N, which tend to be the most optimized ones.
