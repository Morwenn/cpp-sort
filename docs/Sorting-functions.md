***WARNING:** sorting functions are deprecated in version 1.8.0 and removed in version 2.0.0, see [issue #167][issue-167] for the rationale.*

**cpp-sort** has two general sorting functions designed to match the ones in the standard library: `cppsort::sort` and `cppsort::stable_sort`. Both of them have several overloads: some take a [sorter][sorters] to sort the given iterable while others call the library's default sorter instead. They are designed so that it is possible to pass either a collection to sort or a pair of iterators, and to specify a comparison function and/or a projection function. They can also be passed a sorter to perform the sort.

## `cppsort::sort`

### Overloads calling [`default_sorter`][default-sorter]

```cpp
template<typename Iterable>
auto sort(Iterable&& iterable)
    -> void;

template<typename Iterable, typename Compare>
auto sort(Iterable&& iterable, Compare compare)
    -> void;

template<typename Iterable, typename Projection>
auto sort(Iterable&& iterable, Projection projection)
    -> void;

template<typename Iterable, typename Compare, typename Projection>
auto sort(Iterable&& iterable, Compare compare, Projection projection)
    -> void;

template<typename Iterator>
auto sort(Iterator first, Iterator last)
    -> void;

template<typename Iterator, typename Compare>
auto sort(Iterator first, Iterator last, Compare compare)
    -> void;

template<typename Iterator, typename Projection>
auto sort(Iterator first, Iterator last, Projection projection)
    -> void;

template<typename Iterator, typename Compare, typename Projection>
auto sort(Iterator first, Iterator last, Compare compare, Projection projection)
    -> void;
```

These overloads take either an `Iterable` or a pair of `Iterator`, and sort the corresponding collection in-place in ascending order using [`cppsort::default_sorter`][default-sorter] to perform the sort. If provided, the comparison function `compare` and the projection function `projection` are used instead of the default [`std::less<>`][std-less-void] and [`utility::identity`][utility-identity].

### Overload calling a user-provided sorter

```cpp 
template<typename Sorter, typename Iterable>
auto sort(const Sorter& sorter, Iterable&& iterable)
    -> decltype(auto);

template<typename Sorter, typename Iterable, typename Compare>
auto sort(const Sorter& sorter, Iterable&& iterable, Compare compare)
    -> decltype(auto);

template<typename Sorter, typename Iterable, typename Projection>
auto sort(const Sorter& sorter, Iterable&& iterable, Projection projection)
    -> decltype(auto);

template<typename Sorter, typename Iterable,
         typename Compare, typename Projection>
auto sort(const Sorter& sorter, Iterable&& iterable,
          Compare compare, Projection projection)
    -> decltype(auto);

template<typename Sorter, typename Iterator>
auto sort(const Sorter& sorter, Iterator first, Iterator last)
    -> decltype(auto);

template<typename Sorter, typename Iterator, typename Compare>
auto sort(const Sorter& sorter, Iterator first, Iterator last, Compare compare)
    -> decltype(auto);

template<typename Sorter, typename Iterator, typename Projection>
auto sort(const Sorter& sorter, Iterator first, Iterator last, Projection projection)
    -> decltype(auto);

template<typename Sorter, typename Iterator,
         typename Compare, typename Projection>
auto sort(const Sorter& sorter, Iterator first, Iterator last,
          Compare compare, Projection projection)
    -> decltype(auto);
```

These overloads take either an `Iterable` or a pair of `Iterator`, and sort the corresponding collection in-place in ascending order using the given sorter. If provided, the comparison function `compare` and the projection function `projection` are used instead of the default [`std::less<>`][std-less-void] and [`utility::identity`][utility-identity]. The returned value corresponds to the value returned by the sorter's `operator()` when it is given the other parameters.

Note that there is some heavy SFINAE wizardry happening to ensure that none of the `sort` overloads are ambiguous. This magic has been stripped from the documentation for clarity but may contribute to highly unreadable error messages. However, there is still some ambiguity left: the overload resolution might fail if `sort` is given an object that satisfies both the `Compare` and `Projection` concepts. This issue can be worked around with [`as_comparison` and `as_projection`][as-comparison].

## `cppsort::stable_sort`

The overload set for `cppsort::stable_sort` matches that of `cppsort::sort`, so their exact behavior won't be repeated here. The main difference is that they will use [`stable_adapter`][stable-adapter] to wrap the given sorter instead of using the raw sorter. The sorter instance is actually discarded and is only used for overload resolution, so mutable sorters won't work at all. The overloads that do not take a sorter use `stable_adapter<default_sorter>` instead.

### Overloads calling `stable_adapter<default_sorter>`

```cpp
template<typename Iterable>
auto stable_sort(Iterable&& iterable)
    -> void;

template<typename Iterable, typename Compare>
auto stable_sort(Iterable&& iterable, Compare compare)
    -> void;

template<typename Iterable, typename Projection>
auto stable_sort(Iterable&& iterable, Projection projection)
    -> void;

template<typename Iterable, typename Compare, typename Projection>
auto stable_sort(Iterable&& iterable, Compare compare, Projection projection)
    -> void;

template<typename Iterator>
auto stable_sort(Iterator first, Iterator last)
    -> void;

template<typename Iterator, typename Compare>
auto stable_sort(Iterator first, Iterator last, Compare compare)
    -> void;

template<typename Iterator, typename Projection>
auto stable_sort(Iterator first, Iterator last, Projection projection)
    -> void;

template<typename Iterator, typename Compare, typename Projection>
auto stable_sort(Iterator first, Iterator last, Compare compare, Projection projection)
    -> void;
```

### Overload calling a user-provided sorter

```cpp 
template<typename Sorter, typename Iterable>
auto stable_sort(const Sorter& sorter, Iterable&& iterable)
    -> decltype(auto);

template<typename Sorter, typename Iterable, typename Compare>
auto stable_sort(const Sorter& sorter, Iterable&& iterable, Compare compare)
    -> decltype(auto);

template<typename Sorter, typename Iterable, typename Projection>
auto stable_sort(const Sorter& sorter, Iterable&& iterable, Projection projection)
    -> decltype(auto);

template<typename Sorter, typename Iterable,
         typename Compare, typename Projection>
auto stable_sort(const Sorter& sorter, Iterable&& iterable,
                 Compare compare, Projection projection)
    -> decltype(auto);

template<typename Sorter, typename Iterator>
auto stable_sort(const Sorter& sorter, Iterator first, Iterator last)
    -> decltype(auto);

template<typename Sorter, typename Iterator, typename Compare>
auto stable_sort(const Sorter& sorter, Iterator first, Iterator last, Compare compare)
    -> decltype(auto);

template<typename Sorter, typename Iterator, typename Projection>
auto stable_sort(const Sorter& sorter, Iterator first, Iterator last, Projection projection)
    -> decltype(auto);

template<typename Sorter, typename Iterator,
         typename Compare, typename Projection>
auto stable_sort(const Sorter& sorter, Iterator first, Iterator last,
                 Compare compare, Projection projection)
    -> decltype(auto);
```


  [as-comparison]: Miscellaneous-utilities.md#as_comparison-and-as_projection
  [default-sorter]: Sorters.md#default_sorter
  [issue-167]: https://github.com/Morwenn/cpp-sort/issues/167
  [sorters]: Sorters.md
  [std-less-void]: https://en.cppreference.com/w/cpp/utility/functional/less_void
  [stable-adapter]: Sorter-adapters.md#stable_adapter-make_stable-and-stable_t
  [utility-identity]: Miscellaneous-utilities.md#Miscellaneous-function-objects
