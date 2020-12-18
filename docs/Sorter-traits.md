```cpp
#include <cpp-sort/sorter_traits.h>
```

### `is_projection` and `is_projection_iterator`

The goal is these type traits is to check whether a projection function can be applied on the `value_type` of an iterable or an iterator. An additional template parameter `Compare` may be specified, in which case the traits will also check whether the given binary comparison function can be called with two projected values.

```cpp
template<
    typename Projection,
    typename Iterable,
    typename Compare = std::less<>
>
struct is_projection;

template<
    typename Projection,
    typename Iterator,
    typename Compare = std::less<>
>
struct is_projection_iterator;
```

These traits are modeled after the standard library's type traits (that is, they inherit from either `std::true_type` or `std::false_type`), so you can expect them to provide the exact same member types, functions and static variables. Also, just like the standard library type traits, they come along with associated variable templates to reduce the boilerplate:

```cpp
template<typename Projection, typename Iterable, typename Compare=std::less<>>
constexpr bool is_projection_v
    = is_projection<Projection, Iterable, Compare>::value;

template<typename Projection, typename Iterator, typename Compare=std::less<>>
constexpr bool is_projection_iterator_v
    = is_projection_iterator<Projection, Iterator, Compare>::value;
```

### `is_sorter` and friends

The `is_sorter` family of type traits is meant to check whether a given sorter satisfies some properties: being able to be called with of variety of parameters and checking whether the parameters play nice together. For example, `is_comparison_sorter` inherits from `std::true_type` if `Sorter` is a type that can be called with a collection `Iterable&` and a comparison function `Compare`, and inherits from `std::false_type` otherwise. These traits mainly exist for SFINAE purpose and concept checking.

```cpp
template<typename Sorter, typename Iterable>
struct is_sorter;

template<typename Sorter, typename Iterable, typename Compare>
struct is_comparison_sorter;

template<typename Sorter, typename Iterable, typename Projection>
struct is_projection_sorter;

template<typename Sorter, typename Iterable, typename Compare, typename Projection>
struct is_comparison_projection_sorter;
```

There are also variants of these traits which take a potential sorter type and an iterator type (instead of an iterable type). They exist to check whether the sorter can be called with a pair of iterators.

```cpp
template<typename Sorter, typename Iterator>
struct is_sorter_iterator;

template<typename Sorter, typename Iterator, typename Compare>
struct is_comparison_sorter_iterator;

template<typename Sorter, typename Iterator, typename Projection>
struct is_projection_sorter_iterator;

template<typename Sorter, typename Iterator, typename Compare, typename Projection>
struct is_comparison_projection_sorter_iterator;
```

These traits are modeled after the standard library's type traits (that is, they inherit from either `std::true_type` or `std::false_type`), so you can expect them to provide the exact same member types, functions and static variables. Also, just like the standard library type traits, they come along with associated variable templates to reduce the boilerplate:

```cpp
template<typename Sorter, typename Iterable>
constexpr bool is_sorter_v
    = is_sorter<Sorter, Iterable>::value;

template<typename Sorter, typename Iterable, typename Compare>
constexpr bool is_comparison_sorter_v
    = is_comparison_sorter<Sorter, Iterable, Compare>::value;

template<typename Sorter, typename Iterable, typename Projection>
constexpr bool is_projection_sorter_v
    = is_projection_sorter<Sorter, Iterable, Projection>::value;

template<typename Sorter, typename Iterable, typename Compare, typename Projection>
constexpr bool is_comparison_projection_sorter_v
    = is_comparison_projection_sorter<Sorter, Iterable, Compare, Projection>::value;

template<typename Sorter, typename Iterator>
constexpr bool is_sorter_iterator_v
    = is_sorter_iterator<Sorter, Iterator>::value;

template<typename Sorter, typename Iterator, typename Compare>
constexpr bool is_comparison_sorter_iterator_v
    = is_comparison_sorter_iterator<Sorter, Iterator, Compare>::value;

template<typename Sorter, typename Iterator, typename Projection>
constexpr bool is_projection_sorter_iterator_v
    = is_projection_sorter_iterator<Sorter, Iterator, Projection>::value;

template<typename Sorter, typename Iterator, typename Compare, typename Projection>
constexpr bool is_comparison_projection_sorter_iterator_v
    = is_comparison_projection_sorter_iterator<Sorter, Iterator, Compare, Projection>::value;
```

### `sorter_traits`

The class template `sorter_traits<Sorter>` contains information about *sorters* and *[[sorter adapters|Sorter adapters]]* such as the kind of iterators accepted by a sorter and whether it implements or not a stable sorting algorithm.

```cpp
template<typename Sorter>
struct sorter_traits;
```

This class template peeks into `Sorter` to extract the following types:

* `using iterator_category = typename Sorter::iterator_category;`
* `using is_always_stable = typename Sorter::is_always_stable;`

This class is a bit different than the trait classes in the standard library: if one of the types above doesn't exist in the passed sorter, it won't exist in `sorter_traits` either. That means that the traits are not tightly coupled: if a sorter doesn't define `is_always_stable` but defines `iterator_category`, it can still be used in [`hybrid_adapter`](https://github.com/Morwenn/cpp-sort/wiki/Sorter-adapters#hybrid_adapter); instantiating the corresponding `sorter_traits` won't cause a compile-time error because of the missing `is_always_stable`.

### `iterator_category`

```cpp
template<typename Sorter>
using iterator_category = typename sorter_traits<Sorter>::iterator_category;
```

Some tools need to know which category of iterators a sorting algorithm can work with. Therefore, a well-defined sorter shall provide one of the standard library [iterator tags](https://en.cppreference.com/w/cpp/iterator/iterator_tags) in order to document that.

When a sorter is adapted so that it may be used with several categories of iterators, the resulting sorter's iterator category will correspond to the most permissive among the original sorters. For example, if an [`hybrid_adapter`](https://github.com/Morwenn/cpp-sort/wiki/Sorter-adapters#hybrid_adapter) merges sorting algorithms with `std::forward_iterator_tag` and `std::random_access_iterator_tag`, the resulting sorter's category will be `std::forward_iterator_tag` since it is guaranteed to work with any iterable type which has *at least* forward iterators.

### `is_always_stable`

```cpp
template<typename Sorter>
using is_always_stable = typename sorter_traits<Sorter>::is_always_stable;

template<typename Sorter>
constexpr bool is_always_stable_v
    = is_always_stable<Sorter>::value;
```

This type trait is always either [`std::true_type`](https://en.cppreference.com/w/cpp/types/integral_constant) or `std::false_type` and tells whether a sorting algorithm is always [stable](https://en.wikipedia.org/wiki/Sorting_algorithm#Stability) or not. This information may be useful in some contexts, most notably to make sure that [`stable_adapter`](https://github.com/Morwenn/cpp-sort/wiki/Sorter-adapters#stable_adapter) can use a stable sorter directly instead of artificially making it stable.

When a sorter adapter is used, the *resulting sorter* is stable if and only if its stability can be guaranteed and unstable otherwise, even when the *adapted sorter* may be stable (for example, [`self_sort_adapter`](https://github.com/Morwenn/cpp-sort/wiki/Sorter-adapters#self_sort_adapter)'s `is_always_stable` is aliased to `std::false_type` since it is impossible to guarantee the stability of every `sort` method).

### `is_stable`

```cpp
template<typename>
struct is_stable;

template<typename Sorter, typename Args> 
struct is_stable<Sorter(Args...)>:
    sorter_traits<Sorter>::is_always_stable
{};

template<typename Arg>
constexpr bool is_stable_v = is_stable<Arg>::value;
```

This trait is a more flexible version of `is_always_stable`: it tells whether a given sorter will use a stable sorting algorithm when called with a specific set of parameters. I can be used as follows:

```cpp
using sorter = self_sort_adapter<verge_sorter>;
static_assert(is_stable<sorter(std::list<int>&)>, "");
```

[`self_sort_adapter`](https://github.com/Morwenn/cpp-sort/wiki/Sorter-adapters#self_sort_adapter) is a sorter adapter that checks whether a container can sort itself and, if so, uses the container's sorting method instead of the adapted sorter. As a matter of fact, `std::list::sort` implements a stable sorting algorithm (and there is a tweak in **cpp-sort** to take that information into account), so `is_stable<sorter(std::list<int>&)>` will inherit from `std::true_type`. However, `is_stable<sorter(std::vector<int>&)>` or `is_stable<sorter(std::list<int>::iterator, std::list<int>::iterator)>` will inherit from `std::false_type` instead.

The default version of `is_stable` will use `sorter_traits<Sorter>::is_always_stable` to infer the stability of a sorter, but most sorter adapters have dedicated specializations. These specializations allow [`stable_adapter`](https://github.com/Morwenn/cpp-sort/wiki/Sorter-adapters#stable_adapter) to sometimes avoid `make_stable` and to instead use the *adapted sorter* directly when it knows that calling it with specific parameters already yields a stable sort.

### `rebind_iterator_category`

```cpp
template<typename Sorter, typename Category>
struct rebind_iterator_category;
```

This class allows to get a sorter similar to the one passed but with a stricter iterator category. For example, it can generate a sorter whose iterator category is `std::bidirectional_iterator_tag` from another sorter whose iterator category is `std::forward_iterator_tag`. It is mostly useful to make several sorters with the same iterator category work for different iterator categories in an [`hybrid_adapter`](https://github.com/Morwenn/cpp-sort/wiki/Sorter-adapters#hybrid_adapter). Let's say we have two sorters, `foo_sorter` and `bar_sorter`; both have the iterator category `std::forward_iterator_tag`, but `foo_sorter` is the best to sort forward iterators while `bar_sorter` benefits from some optimizations for bidirectional and random-access iterators. It is possible to use the best of both with the following sorter:

```cpp
using sorter = cppsort::hybrid_adapter<
    foo_sorter,
    cppsort::rebind_iterator_category<
        bar_sorter,
        std::bidirectional_iterator_tag
    >
>;
```

The sorter above will pick `foo_sorter` for forward iterators, but will pick `bar_sorter` for bidirectional and random-access iterators. A compile-time error will occur if one tries to rebind to an iterator category that is not derived from the sorter's original iterator category.

### `fixed_sorter_traits`


The class template `fixed_sorter_traits<FixedSizeSorter>` contains information about *fixed-size sorters* such as their stability or the sizes for which a valid specialization of the fixed-size sorter exists.

```cpp
template<
    template<std::size_t N> class FixedSizeSorter,
    typename Indices
>
struct fixed_sorter_traits;
```

This class template can be specialized for any fixed-size sorter and exposes the following properties:

* `domain`: a specialization of [`std::index_sequence`](https://en.cppreference.com/w/cpp/utility/integer_sequence) containing all the sizes for which a specialization of the fixed-size sorter exists. If this trait isn't specified, it is assumed that the fixed-size sorter can be specialized for any value of `N`.
* `iterator_category`: the category of iterators every specialization of the fixed-size sorter is guaranteed to work with. Individual specializations may work with less strict iterator categories.
* `is_always_stable`: an alias for [`std::true_type`](https://en.cppreference.com/w/cpp/types/integral_constant) if every specialization of the fixed-size sorter is guaranteed to always be stable, and `std::false_type` otherwise.