`cppsort::sort`
===============

`cppsort::sort` has two overloads. One which takes a [sorter](doc/sorters.md) to
sort the given iterable, and another one which uses the library's `cppsort::default_sorter`
instead:

```cpp 
template<
    typename RandomAccessIterable,
    typename Sorter,
    typename Compare = std::less<>
>
auto sort(RandomAccessIterable& iterable, const Sorter& sorter, Compare compare={})
    -> void;
```

This overload takes a `RandomAccessIterable` collection and sorts it in-place in
ascending order. The function uses `std::less<>` to compare the elements, unless
specified otherwise, and uses the given sorter to sort them.

```cpp
template<
    typename RandomAccessIterable,
    typename Compare = std::less<>
>
auto sort(RandomAccessIterable& iterable, Compare compare={})
    -> void;
```

This overload takes a `RandomAccessIterable` collection and sorts it in-place in
ascending order. The function uses `std::less<>` to compare the elements unless
specified otherwise, and uses `cppsort::default_sorter` to sort them.

Note that this overload does not appear in the overload set if `Compare` statifies
the `Sorter` concept. That way, it makes sure that there are no ambiguous overloads
when two parameters are given to `cppsort::sort`.
