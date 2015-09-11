# `cppsort::sort`

`cppsort::sort` has several overloads. Some take a [sorter](sorters.md) to sort
the given iterable while others call the library's default sorter instead:

```cpp
template<typename Iterable>
auto sort(Iterable& iterable)
    -> void;

template<typename Iterable, typename Compare>
auto sort(Iterable& iterable, Compare compare)
    -> void;
```

These overloads take an `Iterable` collection and sort it in-place in ascending order
using `cppsort::default_sorter` to perform the sort. The comparator `compare` is used
if provided.

```cpp 
template<
    typename Iterable,
    typename Sorter
>
auto sort(Iterable& iterable, const Sorter& sorter)
    -> decltype(auto);

template<
    typename Iterable,
    typename ComparisonSorter,
    typename Compare
>
auto sort(Iterable& iterable, const ComparisonSorter& sorter, Compare compare)
    -> decltype(auto);
```

These overloads take an `Iterable` collection and sort it in-place in ascending order
using the given sorter. The comparator `compare` is used if provided and if the sorter
satisfies the `ComparisonSorter` concept. The value returned corresponds to the value
returned when applying the sorter to the iterable.

Note that there is some SFINAE magic happening to ensure that none of the overloads
are ambiguous. This magic has been stripped from the documentation for clarity.
