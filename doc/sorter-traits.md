# Sorter traits

```cpp
#include <cpp-sort/sorter_traits.h>
```

### `is_sorter` and friends

These variable templates `is_sorter<typename Sorter, typename Iterable>` and
`is_comparison_sorter<typename Sorter, typename Iterable, typename Compare>`
respectively equal `true` if the type `Sorter` satisfies the [`sorter`](sorters.md)
concept or the `ComparisonSorter` concept and equals `false` otherwise.

In our case, a `Sorter` is a type that can be called with an `Iterable&`
while a  `ComparisonSorter` is a that can be called with an `Iterable&` and
a `Compare`. These traits mainly exist for SFINAE purpose and concept
checking.

```cpp
template<typename Sorter, typename Iterable>
constexpr bool is_sorter = /* implementation-defined */;

template<typename Sorter, typename Iterable, typename Compare>
constexpr bool is_comparison_sorter = /* implementation-defined */;
```

There are also variants of these variable templates which take a potential
sorter type and an iterator type. These exist to check whether the sorter
can be called with a pair of iterators.

```cpp
template<typename Sorter, typename Iterator>
constexpr bool is_sorter_iterator = /* implementation-defined */;

template<typename Sorter, typename Iterator, typename Compare>
constexpr bool is_comparison_sorter_iterator = /* implementation-defined */;
```

### `sorter_traits`

The class template `sorter_traits<Sorter>` contains information about
sorters and sorter adapters such as the kind of iterators accepted by
a sorter and whether it implements or not a stable sorting algorithm.

```cpp
template<typename Sorter>
struct sorter_traits {};
```

This class can be specialized for every sorter object and contains the
following fields:

* `using iterator_category = /* depends on the sorter */;`
* `static constexpr bool is_stable = /* depends on the context */;`

The non-specialized version of `sorter_traits` is defined but empty for
SFINAE friendliness.

### `iterator_category`

```cpp
template<typename Sorter>
using iterator_category = typename sorter_traits<Sorter>::iterator_category;
```

Some tools need to know which category of iterators a sorting algorithm
can work with. Therefore, a well-defined sorter provides one of the standard
library [iterator tags](http://en.cppreference.com/w/cpp/iterator/iterator_tags)
in order to document that.

When a sorter is adapted so that it may be used with several categories of
iterators, the resulting sorter's iterator category will correspond to the
most permissive. For example, if an `hybrid_sorter` merges sorting algorithms
with `std::forward_iterator_tag` and `std::random_access_iterator_tag`, then
the resulting sorter's category will be `std::forward_iterator_tag` since it
is guaranteed to work with any iterable type which has *at least* forward
iterators.

### `is_stable`

```cpp
template<typename Sorter>
constexpr bool is_stable = sorter_traits<Sorter>::is_stable;
```

This variable template contains a boolean value which tells whether a sorting
algorithm is [stable](https://en.wikipedia.org/wiki/Sorting_algorithm#Stability)
or not. This information may be useful in some contexts.

When a sorter adapter is used, the stability of the resulting sorter is
`true` if and only if its stability can be guaranteed and `false` otherwise,
even when the adapted sorter *may* be stable (for example, `self_sorter` is
always `false` since you can't guarantee its stability).
