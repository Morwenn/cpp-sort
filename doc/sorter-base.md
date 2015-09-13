# Sorter base

The [`Sorter`](sorters.md) and `ComparisonSorter` require sorter implementers
to implement a variety of methods (up to eight) with a redundancy factor that
can be rather high. Therefore, **cpp-sort** provides a CRTP base class which
makes creating sorters easier by generating most of the required operations
in the simplest cases.

`sorter_base` provides the following functions so that a sorter can be turned
into a function pointer:

```cpp
template<typename Iterable>
operator Ret(*)(Iterable&)() const;

template<typename Iterable, typename Compare>
operator Ret(*)(Iterable&, Compare)() const;

template<typename Iterator>
operator Ret(*)(Iterator, Iterator)() const;

template<typename Iterator, typename Compare>
operator Ret(*)(Iterator, Iterator, Compare)() const;
```

Note that the syntax is made up, but it allows to clearly highlight what it
does while hiding the ugly `typedef`s needed for the syntax to be valid. In
these signatures, `Ret` is an `std::result_of_t` of the parameters (well, it
is what you would expect it to be).

The following overloads of `operator()` are implemented in `sorter_base`:

```cpp
template<typename Iterable>
auto operator()(Iterable& iterable) const
    -> decltype(auto);

template<typename Iterable, typename Compare>
auto operator()(Iterable& iterable, Compare compare) const
    -> decltype(auto);
```

This overload simply call the `operator()` overloads taking two iterators
by using `std::begin` and `std::end` on `iterable`. When implementing your
own sorters, these overload will be hidden by default but you can import
them in the class with a `using` directive.

Provided you have a sorting function with a standard iterator interface,
creating the corresponding sorter becomes trivial thanks to `sorter_base`.
For instance, here is a simple sorter wrapping `std::sorter`:

```cpp
struct std_sorter:
    cppsort::sorter_base<std_sorter>
{
    using cppsort::sorter_base<std_sorter>::operator();

    template<
        typename RandomAccessIterator,
        typename Compare = std::less<>
    >
    auto operator()(RandomAccessIterator first,
                    RandomAccessIterator last,
                    Compare compare={}) const
        -> void
    {
        std::sort(first, last, compare);
    }
};
```
