To make a decent full-fledged sorter, implementers have to implement a variety of `operator()` overloads with a rather high redundancy factor. To make the task simpler, **cpp-sort** provides a wrapper class which generates most of the boilerplate for the required operations in the simplest cases. To benefit from it, one needs to create a *sorter implementation* and to wrap it into `sorter_facade`:

```cpp
struct frob_sorter_impl
{
    // Regular sorter code
};

struct frob_sorter:
    cppsort::sorter_facade<frob_sorter_impl>
{};
```

Moreover, `sorter_facade` inherits from its template parameter, therefore it has all the properties of the *sorter implementation* it wraps, including the nested type aliases and additional functions. The only things that may be overridden are described below, but all of them eventually end up calling functions from the *sorter implementation* anyway.

### Construction

`sorter_facade` is default-constructible if the *sorter implementation* is default-constructible.

`sorter_facade` also has a constructor tailored to forward its parameters to the *sorter implementation*:

```cpp
template<typename... Args>
constexpr sorter_facade(Args&&... args):
    Sorter(std::forward<Args>(args)...)
{}
```

*Changed in version 1.5.0:* `sorter_facade` can now be constructed with any number of parameters and forwards them to the *sorter implementation*.

### Conversion to function pointers

As long as the *sorter implementation* it wraps is an empty and default-constructible type, `sorter_facade` provides the following member functions so that a sorter can be turned into a function pointer:

```cpp
template<typename Iterable, typename... Args>
constexpr operator Ret(*)(Iterable&, Args...)() const;

template<typename Iterable, typename... Args>
constexpr operator Ret(*)(Iterable&&, Args...)() const;

template<typename Iterator, typename... Args>
constexpr operator Ret(*)(Iterator, Iterator, Args...)() const;
```

Note that the function pointer conversion syntax above is made up, but it allows to clearly highlight what it does while hiding the ugly `typedef`s needed for the syntax to be valid. In these signatures, `Ret` is an [`std::result_of_t`](http://en.cppreference.com/w/cpp/types/result_of) of the parameters (well, it is what you would expect it to be). The actual implementation is more verbose and redundant, but it allows to transform a sorter into a function pointer corresponding to any valid overload of `operator()`.

Since C++17, these function pointer conversion operators are also `constexpr`.

*Changed in version 1.5.0:* these conversion operators exists if and only if the wrapped *sorter implementation* is empty and default-constructible.

### `operator()` for pairs of iterators

`sorter_facade` provides the following overloads of `operator()` to handle pairs of iterators:

```cpp
template<typename Iterator>
auto operator()(Iterator first, Iterator last) const
    -> /* implementation-defined */;

template<typename Iterator, typename Compare>
auto operator()(Iterator first, Iterator last, Compare compare) const
    -> /* implementation-defined */;

template<typename Iterator, typename Projection>
auto operator()(Iterator first, Iterator last, Projection projection) const
    -> /* implementation-defined */;

template<typename Iterator, typename Compare, typename Projection>
auto operator()(Iterator first, Iterator last,
                Compare compare, Projection projection) const
    -> /* implementation-defined */;
```

These overloads will generally forward the parameters to the corresponding `operator()` in the wrapped *sorter implementation*. It does some additional magic to forward `compare` and `projection` to the most suitable `operator()` overload in the *sorter implementation* and to complete the call with instances of [`std::less<>`](http://en.cppreference.com/w/cpp/utility/functional/less_void) and/or [`utility::identity`](https://github.com/Morwenn/cpp-sort/wiki/Miscellaneous-utilities#miscellaneous-function-objects) when additional parameters are needed. Basically, it ensures that everything can be done if `Sorter` has a single `operator()` taking a pair of iterators, a comparison function and a projection function.

Provided you have a sorting function with a standard iterator interface, creating the corresponding sorter becomes trivial thanks to `sorter_facade`. For instance, here is a simple sorter wrapping a [`selection_sort`](https://en.wikipedia.org/wiki/Selection_sort):

```cpp
struct selection_sorter_impl
{
    template<
        typename RandomAccessIterator,
        typename Compare = std::less<>,
        typename Projection = cppsort::utility::identity
    >
    auto operator()(RandomAccessIterator first, RandomAccessIterator last,
                    Compare compare={}, Projection projection={}) const
        -> void
    {
        selection_sort(first, last, compare, projection);
    }
};

struct selection_sorter:
    cppsort::sorter_facade<selection_sorter_impl>
{};
```

### `operator()` for ranges

`sorter_facade` provides the following overloads of `operator()` to handle ranges:

```cpp
template<typename Iterable>
auto operator()(Iterable&& iterable) const
    -> /* implementation-defined */;

template<typename Iterable, typename Compare>
auto operator()(Iterable&& iterable, Compare compare) const
    -> /* implementation-defined */;

template<typename Iterable, typename Projection>
auto operator()(Iterable&& iterable, Projection projection) const
    -> /* implementation-defined */;

template<typename Iterable, typename Compare, typename Projection>
auto operator()(Iterable&& iterable, Compare compare, Projection projection) const
    -> /* implementation-defined */;
```

These overloads will generally forward the parameters to the corresponding `operator()` overloads in the wrapped *sorter implementation* if they exist, or try to call an equivalent `operator()` taking a pair of iterators in the wrapped sorter by using `utility::begin` and `utility::end` on the iterable to sort. It also does some additional magic to forward `compare` and `projection` to the most suitable `operator()` overload in `sorter` and to complete the call with instances of [`std::less<>`](http://en.cppreference.com/w/cpp/utility/functional/less_void) and/or [`utility::identity`](https://github.com/Morwenn/cpp-sort/wiki/Miscellaneous-utilities#miscellaneous-function-objects) when additional parameters are needed. Basically, it ensures that everything can be done if `Sorter` has a single `operator()` taking a pair of iterators, a comparison function and a projection function.

It will always call the most suitable iterable `operator()` overload in the wrapped *sorter implementation* if there is one, and dispatch the call to an overload taking a pair of iterators when it cannot do otherwise.

### Projection support for comparison-only sorters

Some *sorter implementations* are able to handle custom comparison functions but don't have any dedicated support for projections. If such an implementation is wrapped by `sorter_facade` and is given a projection function, `sorter_facade` will bake the projection into the comparison function and give the result to the *sorter implementation* as a comparison function. Basically it means that a *sorter implementation* with a single `operator()` taking a pair of iterators and a comparison function can take any iterable, pair of iterators, comparison and/or projection function once it wrapped into `sorter_facade`.

The reverse operation (baking a comparison function into a projection function) is not doable and simply does not make sense most of the time, so `sorter_facade` does not provide it for projection-only *sorter implementations*.

### Universal support for `std::less<>` and `utility::identity`

**cpp-sort** considers that every collection sorted without a specific comparison nor projection function shoud work *as if* it was sorted with `std::less<>` and `utility::identity`. However, some sorters do not provide overloads for `operator()` taking comparison and/or projection functions. `sorter_facade` provides the following overloads so that every sorter can be passed `std::less<>` and/or `utility::identity` even if does not handle other comparisons or projections:

```cpp
template<typename Iterable>
auto operator()(Iterable&& iterable, std::less<>) const
    -> /* implementation-defined */;

template<typename Iterable>
auto operator()(Iterable&& iterable, utility::identity) const
    -> /* implementation-defined */;

template<typename Iterable>
auto operator()(Iterable&& iterable, std::less<>, utility::identity) const
    -> /* implementation-defined */;

template<typename Iterable, typename Projection>
auto operator()(Iterable&& iterable, std::less<>, Projection projection) const
    -> /* implementation-defined */;

template<typename Iterator>
auto operator()(Iterator first, Iterator last, std::less<>) const
    -> /* implementation-defined */;

template<typename Iterator>
auto operator()(Iterator first, Iterator last, utility::identity) const
    -> /* implementation-defined */;

template<typename Iterator>
auto operator()(Iterator first, Iterator last,
                std::less<>, utility::identity) const
    -> /* implementation-defined */;

template<typename Iterator, typename Projection>
auto operator()(Iterator first, Iterator last,
                std::less<>, Projection projection) const
    -> /* implementation-defined */;
```

While it does not appear in this documentation, `sorter_facade` actually relies on an extensive amount of SFINAE tricks to ensure that only the `operator()` overloads that are needed and viable are generated. For example, the magic `std::less<>` overloads won't be generated if the wrapped *sorter implementation* already accepts a comparison function.
