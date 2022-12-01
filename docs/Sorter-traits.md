```cpp
#include <cpp-sort/sorter_traits.h>
```

### `is_projection` and `is_projection_iterator`

The goal of these type traits is to check whether a projection function can be applied on the `reference_type` of a range or of an iterator. An additional template parameter `Compare` may be specified, in which case the traits will also check whether the given binary comparison function can be called with two projected values.

```cpp
template<
    typename Projection,
    typename Range,
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

These traits are modeled after the standard library's type traits (that is, they inherit from either `std::true_type` or `std::false_type`), and as such provide the exact same member types, functions and static variables. Also, just like the standard library type traits, they come along with associated variable templates to mitigate the boilerplate:

```cpp
template<typename Projection, typename Range, typename Compare=std::less<>>
inline constexpr bool is_projection_v
    = is_projection<Projection, Range, Compare>::value;

template<typename Projection, typename Iterator, typename Compare=std::less<>>
inline constexpr bool is_projection_iterator_v
    = is_projection_iterator<Projection, Iterator, Compare>::value;
```

### `is_sorter` and friends

The `is_sorter` family of type traits is meant to check whether a given sorter satisfies some properties: being able to be called with of variety of parameters and checking whether the parameters play nice together. For example, `is_comparison_sorter` inherits from `std::true_type` if `Sorter` is a type that can be called with a collection `Range&` and a comparison function `Compare`, and inherits from `std::false_type` otherwise. These traits mainly exist for SFINAE purpose and concept checking.

```cpp
template<typename Sorter, typename Range>
struct is_sorter;

template<typename Sorter, typename Range, typename Compare>
struct is_comparison_sorter;

template<typename Sorter, typename Range, typename Projection>
struct is_projection_sorter;

template<typename Sorter, typename Range, typename Compare, typename Projection>
struct is_comparison_projection_sorter;
```

The following variants of the previous traits check whether a sorter can be called with an iterator/sentinel pair.

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

These traits are modeled after the standard library's type traits (that is, they inherit from either `std::true_type` or `std::false_type`), and as such provide the exact same member types, functions and static variables. Also, just like the standard library type traits, they come along with associated variable templates to mitigate the boilerplate:

```cpp
template<typename Sorter, typename Range>
inline constexpr bool is_sorter_v
    = is_sorter<Sorter, Range>::value;

template<typename Sorter, typename Range, typename Compare>
inline constexpr bool is_comparison_sorter_v
    = is_comparison_sorter<Sorter, Range, Compare>::value;

template<typename Sorter, typename Range, typename Projection>
inline constexpr bool is_projection_sorter_v
    = is_projection_sorter<Sorter, Range, Projection>::value;

template<typename Sorter, typename Range, typename Compare, typename Projection>
inline constexpr bool is_comparison_projection_sorter_v
    = is_comparison_projection_sorter<Sorter, Range, Compare, Projection>::value;

template<typename Sorter, typename Iterator>
inline constexpr bool is_sorter_iterator_v
    = is_sorter_iterator<Sorter, Iterator>::value;

template<typename Sorter, typename Iterator, typename Compare>
inline constexpr bool is_comparison_sorter_iterator_v
    = is_comparison_sorter_iterator<Sorter, Iterator, Compare>::value;

template<typename Sorter, typename Iterator, typename Projection>
inline constexpr bool is_projection_sorter_iterator_v
    = is_projection_sorter_iterator<Sorter, Iterator, Projection>::value;

template<typename Sorter, typename Iterator, typename Compare, typename Projection>
inline constexpr bool is_comparison_projection_sorter_iterator_v
    = is_comparison_projection_sorter_iterator<Sorter, Iterator, Compare, Projection>::value;
```

### `sorter_traits`

The class template `sorter_traits<Sorter>` contains information about *[sorters][sorters]* and *[sorter adapters][sorter-adapters]* such as the kind of iterators accepted by a sorter and whether it is guaranteed to always sort stably.

```cpp
template<typename Sorter>
struct sorter_traits;
```

This class template peeks into `Sorter` to extract the following types:
* `using iterator_category = typename Sorter::iterator_category;`
* `using is_always_stable = typename Sorter::is_always_stable;`

Its behaviour is however a bit different from that of the trait classes in the standard library: if one of the types above doesn't exist in the passed sorter, it won't exist in the corresponding `sorter_traits` specialization either. That means that the traits are not tightly coupled: for example if a sorter doesn't define `is_always_stable` but defines `iterator_category`, it can still be used in [`hybrid_adapter`][hybrid-adapter]; instantiating the corresponding `sorter_traits` won't cause a compile-time error because of the missing `is_always_stable`.

`sorter_traits` can be specialized for user-defined *sorters* or *sorter adapters*, and those specialization are sometimes the only place where the information exists. As such, `sorter_traits` is the preferred way to query information about a sorter - it's all the more important as **cpp-sort** itself sometimes fully relies on a `sorter_traits` specialization to provide some information. The in-class traits should be seen as convenient short-hands, but `sorter_traits` as the main source of truth - it is considered an error if both exist and differ.

### `iterator_category`

```cpp
template<typename Sorter, typename MergeWithCategory=void>
using iterator_category = /* see below */;
```

This trait is expect to alias a standard [iterator tag][iterator-tags], working as follows:
* When `MergeWithCategory` is `void`, it aliases `typename sorter_traits<Sorter>::iterator_category`.
* When `typename sorter_traits<Sorter>::iterator_category` is not valid, it aliases `MergeWithCategory`.
* When both are valid and non-`void`, it aliases the stronger category of the two (for example when given `std::random_access_iterator_tag` and `std::bidirectional_iterator_tag`, then it aliases `std::random_access_iterator_tag`).

If `MergeWithCategory` is a type than `void` or an iterator type, then the program is ill-formed and triggers a hard compile time error.

This trait is meant to be used to define the `iterator_category` that the *resulting sorter* of a [sorter adapter][sorter-adapters] works on. The second parameter is useful when the sorter adapter runs the *adapted sorter* directly on [parts of] the passed collection, in which case the *resulting sorter* only works on the stronger iterator category of the two.

### `is_always_stable`

```cpp
template<typename Sorter>
using is_always_stable = typename sorter_traits<Sorter>::is_always_stable;

template<typename Sorter>
constexpr bool is_always_stable_v
    = is_always_stable<Sorter>::value;
```

This type trait is always either [`std::true_type` or `std::false_type`][std-integral-constant] and tells whether a sorter is always [stable][stability] or not. This information may be useful in some contexts, and is most notably by [`stable_t`][stable-adapter] to avoid unnecessarily nesting templates when possible.

When a sorter adapter is used, the *resulting sorter* is considered always stable if and only if its stability can be guaranteed, and considered unstable otherwise, even when the *adapted sorter* may be stable (for example, [`self_sort_adapter`][self-sort-adapter]`::is_always_stable` is aliased to `std::false_type` since it is impossible to guarantee the stability of every collection's `sort` method).

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

This trait is a more flexible version of [`is_always_stable`][is-always-stable]: it tells whether a given sorter uses a stable sorting algorithm when called with a specific set of parameters. It can be used as follows:

```cpp
using sorter = self_sort_adapter<heap_sorter>;
static_assert(is_stable<sorter(std::list<int>&)>, "");
```

[`self_sort_adapter`][self-sort-adapter] is a [*sorter adapter*][sorter-adapters] that checks whether a container can sort itself and, if so, uses the container's sorting method instead of the *adapted sorter*. As a matter of fact, [`std::list::sort`][std-list-sort] implements a stable sorting algorithm and **cpp-sort** specializes `is_stable` to take that information into account, so `is_stable_v<sorter(std::list<int>&)>` is `true` despite `is_always_stable_v<sorter>` being `false`. However, `is_stable_v<sorter(std::vector<int>&)>` and `is_stable_v<sorter(std::list<int>::iterator, std::list<int>::iterator)>` remain `false`.

The default version of `is_stable` uses `sorter_traits<Sorter>::is_always_stable` to infer the stability of a sorter, but most sorter adapters have dedicated specializations. These specializations notably allow [`stable_adapter`][stable-adapter] to sometimes avoid using `make_stable` and to instead use the *adapted sorter* directly when it knows that calling it with specific parameters already yields a stable sort.

### `rebind_iterator_category`

```cpp
template<typename Sorter, typename Category>
struct rebind_iterator_category;
```

This class allows to get a sorter similar to the one passed but with a stricter [iterator category][iterator-tags]. For example, it can generate a sorter whose iterator category is `std::bidirectional_iterator_tag` from another sorter whose iterator category is `std::forward_iterator_tag`. It is mostly useful to make several sorters with the same iterator category work for different iterator categories in [`hybrid_adapter`][hybrid-adapter]. Let's say we have two sorters, `foo_sorter` and `bar_sorter`; both have the iterator category `std::forward_iterator_tag`, but `foo_sorter` is the best to sort forward iterators while `bar_sorter` benefits from some optimizations for bidirectional and random-access iterators. It is possible to use the best of both with the following sorter:

```cpp
using sorter = cppsort::hybrid_adapter<
    foo_sorter,
    cppsort::rebind_iterator_category<
        bar_sorter,
        std::bidirectional_iterator_tag
    >
>;
```

The sorter above will pick `foo_sorter` for forward iterators, but `bar_sorter` for bidirectional and random-access iterators. A compile-time error occurs when one tries to rebind a sorter to a less strict iterator category than its own.

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

* `domain`: a specialization of [`std::index_sequence`][std-integer-sequence] containing all the sizes for which a specialization of the fixed-size sorter exists. If this trait isn't specified, it is assumed that the fixed-size sorter can be specialized for any value of `N`.
* `iterator_category`: the category of iterators every specialization of the fixed-size sorter is guaranteed to work with. Individual specializations may work with less strict iterator categories.
* `is_always_stable`: an alias for [`std::true_type`][std-integral-constant] if every specialization of the fixed-size sorter is guaranteed to always be stable, and `std::false_type` otherwise.


  [heap-sorter]: Sorters.md#heap_sorter
  [hybrid-adapter]: Sorter-adapters.md#hybrid_adapter
  [is-always-stable]: Sorter-traits.md#is_always_stable
  [iterator-tags]: https://en.cppreference.com/w/cpp/iterator/iterator_tags
  [out-of-place-adapter]: Sorter-adapters.md#out_of_place_adapter
  [self-sort-adapter]: Sorter-adapters.md#self_sort_adapter
  [sorter-adapters]: Sorter-adapters.md
  [sorters]: Sorters.md
  [stability]: https://en.wikipedia.org/wiki/Sorting_algorithm#Stability
  [stable-adapter]: Sorter-adapters.md#stable_adapter-make_stable-and-stable_t
  [std-integer-sequence]: https://en.cppreference.com/w/cpp/utility/integer_sequence
  [std-integral-constant]: https://en.cppreference.com/w/cpp/types/integral_constant
  [std-list-sort]: https://en.cppreference.com/w/cpp/container/list/sort
