# Fixed-size sorters

Fixed-size sorters, sometimes called *fixed sorters* for simplicity are a special
kind of sorters designed to sort a fixed number of values. Their `operator()` also
takes a pair of iterators and an optional comparison function. Most of the time,
the end iterator is unused, but future versions of the library may start to use it
to optionaly perform bounds-checking.

Fixed-sized sorters are not actual sorters per se but class templates that takes an
`std::size_t` template parameter. Every specialization of a class template for some
size gives a "valid" sorter. Many of them have specializations for some sizes only
and will produce a compile-time error otherwise. **cpp-sort** provides some tools
to enhance the usability of fixed-sized sorters. You can read more about such tools
in the page about [*sorter adapters*](sorter-adapters.md).

It is possible to include every fixed-sized sorter at once with the following line:

```cpp
#include <cpp-sort/fixed_sorters.h>
```

The following fixed-size sorters are available and should work with any type for
which `std::less` works:

### `low_comparisons_sorter`

```cpp
#include <cpp-sort/fixed/low_comparisons_sorter.h>
```

This fixed-size sorter implements specific sorting agolrithms whose goal is to keep
the number of comparisons performed at its lowest, which may be desirable when the
comparisons are expensive but the moves are cheap (for example strings with large
common prefixes). To know which sorting algorithm has the lowest overall number of
comparisons for an array of a given size, the following method is used: count the
total number of comparisons needed to sort every permutation of the array, we call
this the *comparison weight* of the array.

The following table illustrates the *comparison weight* of the algorithms used by
the different specializations of `low_comparisons_sorter`. If you find an algorithm
that beats one of those, do not hesitate to contribute:

Size | Comparison weight
---- | -----------------
0 | 0
1 | 0
2 | 2
3 | 16
4 | 112
5 | 840
6 | 6960
7 | 63840
8 | 631680
9 | 6854400
10 | 80881920
11 | 1031224320
12 | 15292247040
13 | 225057208320

This sorter uses a variety of specialized sorting algorithms depending on the size
of the array to sort. Many of them correspond to unrolled versions of more general
sorts. The following table describes the algorithms used:

Size | Algorithm
---- | ---------
0 | Nothing
1 | Nothing
2 | Compare and exchange
3 | Insertion sort
4 | Insertion sort
5 | Merge insertion sort
6 | Insertion sort
7 | Insertion sort
8 | Insertion sort
9 | Insertion sort
10 | Insertion sort
11 | Insertion sort
12 | [Double insertion sort](research.md)
13 | Double insertion sort

However, many of the dedicated sorting algorithms are probably still suboptimal and
could use a merge insertion sort instead.

```cpp
template<std::size_t N>
struct low_comparisons_sorter;
```

### `low_moves_sorter`

```cpp
#include <cpp-sort/fixed/low_moves_sorter.h>
```

This fixed-size sorter implements specific sorting agolrithms whose goal is to keep
the number of moves performed at its lowest, which may be useful when comparisons
are cheaps but moves are expensive (large objects that only use one field for the
comparison for example). To know which sorting algorithm performs the lowest number
of move operations overall for an array of a given size, the following method is
used: count the total number of moves needed to sort all the permutations of the
array, we will call this the *move weight* of the array.

The following table illustrates the *move weight* of the algorithms used by the
different specializations of `low_moves_sorter`. If you ever find an algorithm that
beats one of those, do not hesitate to contribute:

Size | Move weight
---- | -----------
0 | 0
1 | 0
2 | 2
3 | 17
4 | 122
5 | 898
6 | 7188
7 | 63276
8 | 612048
9 | 6476112
10 | 74558880
11 | 929011680
12 | 12465394560
13 | 179294186880

The algorithms 0 to 3 use an unrolled insertion sort. The algorithm 4 uses a simple
selection sort. The following algorithms use a recursive bidirectional selection
sort, sometimes known as cocktail selection sort or minmax sort. While it does not
perform less moves than a selection sort, it performs a bit less comparisons. This
sorter has no upper bound, it can sort an array of size 155 if needed, but then it
might generate too much code, so try to keep the size low if possible.

```cpp
template<std::size_t N>
struct low_moves_sorter;
```

### `sorting_network_sorter`

```cpp
#include <cpp-sort/fixed/sorting_network_sorter.h>
```

This sorter provides [sorting network](https://en.wikipedia.org/wiki/Sorting_network)
algorithms to sort collections of size 0 to 32. While using a generic algorithm for
the task such as a Batcher's odd-even mergesort may be too slow to be usable, the
unrolled resulting sorting networks may be fast enough and even tend to be faster
than everything else when it comes to sorting small arrays of integers without even
requiring additional memory.

*Note: don't be fooled by the name, none of the algorithms in this sorter perform
any operation in parallel. Everything is sequential. The algorithms are but long
sequences of compare-and-exhange units.*

```cpp
template<std::size_t N>
struct sorting_network_sorter;
```
