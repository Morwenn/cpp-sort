# Sorter facade

The [`Sorter`](sorters.md) and `ComparisonSorter` require sorter implementers
to implement a variety of methods (up to eight) with a redundancy factor that
can be rather high. Therefore, **cpp-sort** provides a CRTP base class which
makes creating sorters easier by generating most of the required operations
in the simplest cases.

### Conversions to function pointers

`sorter_facade` provides the following member functions so that a sorter can
be turned into a function pointer:

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

### `operator()` for ranges

`sorter_facade` provides overloads `operator()` to handle ranges:

```cpp
template<typename Iterable>
auto operator()(Iterable& iterable) const
    -> decltype(Sorter{}(std::begin(iterable), std::end(iterable)));

template<typename Iterable, typename Compare>
auto operator()(Iterable& iterable, Compare compare) const
    -> decltype(Sorter{}(std::begin(iterable), std::end(iterable)));
```

This overload simply call the `operator()` overloads taking two iterators
by using `std::begin` and `std::end` on `iterable`. When implementing your
own sorters, these overload will be hidden by default but you can import
them in the class with a `using` directive.

Provided you have a sorting function with a standard iterator interface,
creating the corresponding sorter becomes trivial thanks to `sorter_facade`.
For instance, here is a simple sorter wrapping `std::sort`:

```cpp
struct std_sorter:
    cppsort::sorter_facade<std_sorter>
{
    using cppsort::sorter_facade<std_sorter>::operator();

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

### Universal support for `std::less`

**cpp-sort** considers that every collection sorted without a specific
comparison function shoud work *as if* it was sorted with `std::less<>`.
However, some sorters do not implement comparison sorts and therefore do
not provide overloads of `operator()` taking custom comparison functions.

`sorter_facade` provides the following overloads so that every sorter,
even non-comparison sorters, can be passed `std::less<>` or `std::less<T>`
where `T` is the type of the values to sort:

```cpp
template<typename Iterable>
auto operator()(Iterable& iterable, std::less<>) const
    -> decltype(Sorter{}(iterable));

template<typename Iterator>
auto operator()(Iterator first, Iterator last, std::less<>) const
    -> decltype(Sorter{}(first, last));

template<typename Iterable>
auto operator()(Iterable& iterable,
                std::less<typename std::iterator_traits<decltype(std::begin(iterable))>::value_type>) const
    -> decltype(Sorter{}(iterable));

template<typename Iterator>
auto operator()(Iterator first, Iterator last,
                std::less<typename std::iterator_traits<Iterator>::value_type>) const
    -> decltype(Sorter{}(first, last));
```

While they do not appear in the signatures above, there are some SFINAE
tricks to ensure that these overloads are generated only if the adapted
sorter is not already a comparison sorter.
