To write a full-fledged sorter, implementers have to implement a variety of `operator()` overloads with a rather high redundancy factor. To make the task simpler, **cpp-sort** provides a wrapper class which generates most of the boilerplate for the required operations in the simplest cases. To benefit from it, one needs to create a *sorter implementation* and to wrap it into `sorter_facade`:

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

All the `operator()` of `sorter_facade` use [`mstd::forward_iterator`][mstd-iterator] or [`mstd::forward_range`][mstd-ranges] to constrain their parameters. It is the responsibility of the *sorter implementation* author to stregthen those constraints in the implementation overloads.

### Construction

`sorter_facade` is default-constructible if the *sorter implementation* is default-constructible.

`sorter_facade` also has a constructor tailored to forward its parameters to the *sorter implementation*:

```cpp
template<typename... Args>
constexpr sorter_facade(Args&&... args):
    Sorter(std::forward<Args>(args)...)
{}
```

### Conversion to function pointers

As long as the *sorter implementation* it wraps is an empty and default-constructible type, `sorter_facade` provides the following member functions so that a sorter can be turned into a function pointer:

```cpp
template<mstd::forward_range Range, typename... Args>
constexpr operator Ret(*)(Range&, Args...)() const;

template<mstd::forward_range Range, typename... Args>
constexpr operator Ret(*)(Range&&, Args...)() const;

template<
    mstd::forward_iterator Iterator,
    mstd::sentinel_for<Iterator> Sentinel,
    typename... Args
>
constexpr operator Ret(*)(Iterator, Sentinel, Args...)() const;
```

The return type `Ret` can either match that of the sorter, or be `void`, in which case the return value is discarded.

Note that the function pointer conversion syntax above is made up, but it allows to clearly highlight what it does while hiding the `typedef`s needed for the syntax to be valid. In these signatures, `Ret` is the [`std::result_of_t`][std-result-of] of the sorter called with the parameters. The actual implementation is more verbose and redundant, but it allows to transform a sorter into a function pointer corresponding to any valid overload of `operator()`.

***WARNING:** conversion to function pointers does not work with MSVC ([issue #185][issue-185]).*

### `operator()` for pairs of iterators

`sorter_facade` provides the following overloads of `operator()` to handle pairs of iterators:

```cpp
template<
    mstd::forward_iterator Iterator,
    mstd::sentinel_for<Iterator> Sentinel
>
constexpr auto operator()(Iterator first, Sentinel last) const
    -> /* implementation-defined */;

template<
    mstd::forward_iterator Iterator,
    mstd::sentinel_for<Iterator> Sentinel,
    typename Compare
>
constexpr auto operator()(Iterator first, Sentinel last, Compare compare) const
    -> /* implementation-defined */;

template<
    mstd::forward_iterator Iterator,
    mstd::sentinel_for<Iterator> Sentinel,
    typename Projection
>
constexpr auto operator()(Iterator first, Sentinel last, Projection projection) const
    -> /* implementation-defined */;

template<
    mstd::forward_iterator Iterator,
    mstd::sentinel_for<Iterator> Sentinel,
    typename Compare,
    typename Projection
>
constexpr auto operator()(Iterator first, Sentinel last,
                          Compare compare, Projection projection) const
    -> /* implementation-defined */;
```

These overloads will generally forward the parameters to the corresponding `operator()` in the wrapped *sorter implementation*. It does some additional magic to forward `compare` and `projection` to the most suitable `operator()` overload in the *sorter implementation* and to complete the call with instances of [`std::less<>`][std-less-void] and/or [`std::identity`][std-identity] when additional parameters are needed. Basically, it ensures that everything can be done if `Sorter` has a single `operator()` taking an iterator/sentinel pair, a comparison function and a projection function.

Provided you have a sorting function with a standard iterator interface, creating the corresponding sorter becomes trivial thanks to `sorter_facade`. For instance, here is a simple sorter wrapping a [`selection_sort`][selection-sort]:

```cpp
struct selection_sorter_impl
{
    template<
        mstd::random_access_iterator Iterator,
        mstd::sentinel_for<Iterator> Sentinel,
        typename Compare = std::less<>,
        typename Projection = std::identity
    >
    auto operator()(Iterator first, Sentinel last,
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
template<mstd::forward_range Range>
constexpr auto operator()(Range&& range) const
    -> /* implementation-defined */;

template<mstd::forward_range Range, typename Compare>
constexpr auto operator()(Range&& range, Compare compare) const
    -> /* implementation-defined */;

template<mstd::forward_range Range, typename Projection>
constexpr auto operator()(Range&& range, Projection projection) const
    -> /* implementation-defined */;

template<mstd::forward_range Range, typename Compare, typename Projection>
constexpr auto operator()(Range&& range, Compare compare, Projection projection) const
    -> /* implementation-defined */;
```

These overloads will generally forward the parameters to the corresponding `operator()` overloads in the wrapped *sorter implementation* if they exist, or try to call an equivalent `operator()` taking an iterator/sentinel pair in the wrapped sorter by using [`mstd::begin` and `mstd::end`][mstd-ranges] on the range to sort. It also does some additional magic to forward `compare` and `projection` to the most suitable `operator()` overload in `sorter` and to complete the call with instances of [`std::less<>`][std-less-void] and/or [`std::identity`][std-identity] when additional parameters are needed. Basically, it ensures that everything can be done if `Sorter` has a single `operator()` taking an iterator/sentinel pair, a comparison function and a projection function.

It will always call the most suitable range `operator()` overload in the wrapped *sorter implementation* if there is one, and dispatch the call to an overload taking an iterator/sentinel pair when it cannot do otherwise.

### Projection support for comparison-only sorters

Some *sorter implementations* are able to handle custom comparison functions but don't have any dedicated support for projections. If such an implementation is wrapped by `sorter_facade` and is given a projection function, `sorter_facade` will bake the projection into the comparison function and give the result to the *sorter implementation* as a comparison function. Basically it means that a *sorter implementation* with a single `operator()` taking an iterator/sentinel pair and a comparison function can take any range, iterator/sentinel pair, comparison and/or projection function once it is wrapped into `sorter_facade`.

The reverse operation (baking a comparison function into a projection function) is not doable and simply does not make sense most of the time, so `sorter_facade` does not provide such a feature for *sorter implementations* that only provide support for projections.

### Universal support for `std::less<>`/`std::ranges::less` and `std::identity`

**cpp-sort** considers that every collection sorted without a specific comparison nor projection function shoud work *as if* it was sorted with [`std::less<>`][std-less-void] and [`std::identity`][std-identity]. However, some sorters do not provide overloads for `operator()` taking comparison and/or projection functions. `sorter_facade` provides the following overloads so that every sorter can be passed `std::less<>` and/or `std::identity` even if does not handle other comparisons or projections:

```cpp
template<mstd::forward_range Range>
auto operator()(Range&& range, std::less<>) const
    -> /* implementation-defined */;

template<mstd::forward_range Range>
auto operator()(Range&& range, std::identity) const
    -> /* implementation-defined */;

template<mstd::forward_range Range>
auto operator()(Range&& range, std::less<>, std::identity) const
    -> /* implementation-defined */;

template<mstd::forward_range Range, typename Projection>
auto operator()(Range&& range, std::less<>, Projection projection) const
    -> /* implementation-defined */;

template<
    mstd::forward_iterator Iterator,
    mstd::sentinel_for<Iterator> Sentinel
>
auto operator()(Iterator first, Sentinel last, std::less<>) const
    -> /* implementation-defined */;

template<
    mstd::forward_iterator Iterator,
    mstd::sentinel_for<Iterator> Sentinel
>
auto operator()(Iterator first, Sentinel last, std::identity) const
    -> /* implementation-defined */;

template<
    mstd::forward_iterator Iterator,
    mstd::sentinel_for<Iterator> Sentinel
>
auto operator()(Iterator first, Sentinel last, std::less<>, std::identity) const
    -> /* implementation-defined */;

template<
    mstd::forward_iterator Iterator,
    mstd::sentinel_for<Iterator> Sentinel,
    typename Projection
>
auto operator()(Iterator first, Sentinel last, std::less<>, Projection projection) const
    -> /* implementation-defined */;
```

Special overloads using [`std::ranges::less`][std-ranges-less] as a vocabulary are also available, with a behaviour similar to that of the `std::less<>` ones above.

While it does not appear in this documentation, `sorter_facade` actually relies on an extensive amount of SFINAE tricks to ensure that only the `operator()` overloads that are needed and viable are generated. For example, the magic `std::less<>` overloads won't be generated if the wrapped *sorter implementation* already accepts a comparison function.


  [issue-185]: https://github.com/Morwenn/cpp-sort/issues/185
  [mstd-iterator]: Modified-standard-library.md#cpp-sortmstditeratorh
  [mstd-ranges]: Modified-standard-library.md#cpp-sortmstdrangesh
  [selection-sort]: https://en.wikipedia.org/wiki/Selection_sort
  [std-identity]: https://en.cppreference.com/w/cpp/utility/functional/identity
  [std-less-void]: https://en.cppreference.com/w/cpp/utility/functional/less_void
  [std-ranges-less]: https://en.cppreference.com/w/cpp/utility/functional/ranges/less
  [std-result-of]: https://en.cppreference.com/w/cpp/types/result_of
