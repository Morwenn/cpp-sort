# Writing a sorter

In this tutorial, you will learn the quirks involved in writing a good sorter to
wrap a sorting algorithm. Writing a basic sorter is easy, but getting everything
right to the details might be a bit tricky from time to time. This guide should
help you to make the good choices (at least I hope so).

## Concepts

To simplify things, we will introduce two concepts: `Sorter` and `ComparisonSorter`.
Note that this part is a bit old and some things need to be defined again, but it is
a good starting point to to introduce the ideas used by the library.

A type satisfies the `Sorter` concept if it can be called on a `ForwardIterable&`
object, where a type satisfies the `ForwardIterable` concept if and only if calls
to `std::begin` and `std::end` on this object return instances of a type satisfying
the [`ForwardIterator`](http://en.cppreference.com/w/cpp/concept/ForwardIterator)
concept (since the iterable is sorted in-place and thus altered, sorting an
`InputIterable` type is not possible). To satisfy the `Sorter` concept, a type also
has to provide an overload of `operator()` which takes a pair of `ForwardIterator`.
A `Sorter` instance should additionally be convertible to `Ret(*)(ForwardIterable&)`
and `Ret(*)(ForwardIterator, ForwardIterator)` where `Ret` is the return type of
the operation.

A type satisfies the `ComparisonSorter` concept if it satisfies the `Sorter`
concept and can additionally be called with another parameter satisfying the
[`Compare`](http://en.cppreference.com/w/cpp/concept/Compare) concept. While
most sorters satisfy this concept, some of them might implement non-comparison
based sorting algorithms such as radix sort, and thus only satisfy the `Sorter`
concept. A `ComparisonSorter` instance should additionally be convertible to
`Ret(*)(ForwardIterable&, Compare)` and `Ret(*)(ForwardIterator, ForwardIterator, Compare)`
where `Ret` is the return type of the operation.

## Writing a generic sorter

Let's say you have the following `selection_sort` algorithm and you want to write
a sorter to wrap it:

```cpp
template<
    typename ForwardIterator,
    typename Compare = std::less<>
>
void selection_sort(ForwardIterator first, ForwardIterator last,
                    Compare compare={})
{
    for (auto it = first ; it != last ; ++it)
    {
        auto selection = std::min_element(it, last, compare);
        std::iter_swap(selection, it); 
    }
}
```

Wrapping this algorithm into a `selection_sorter` object to benefit from the full
power of **cpp-sort** is actually rather trivial. Here is the corresponding sorter:

```cpp
struct selection_sorter:
    cppsort::sorter_base<selection_sorter>
{
    using cppsort::sorter_base<selection_sorter>::operator();

    template<
        typename ForwardIterator,
        typename Compare = std::less<>
    >
    auto operator()(ForwardIterator first, ForwardIterator last,
                    Compare compare={}) const
        -> void
    {
        selection_sort(first, last, compare);
    }
};
```

As you can see, you simply have to provide an `operator()` overload that forwards its
values to the sorting function. If your sorter supports custom comparison functions,
make sure that it can also be called *without* a comparison function. The reason is
that a `ComparisonSorter` should also be usable wherever a `Sorter` is usable.

In the previous example, [`sorter_base`](sorter-base.md) is a CRTP base class that
is used to provide the function pointer conversion operators to the sorter. It also
generates the range overloads of `operator()`. The `using` declaration is needed if
you don't want the `sorter_base` overloads of `operator()` to be hidden by the one
you defined.

Note that you can still provide `operator()` overloads equivalent to those provided
by `sorter_base`. For example, if the sorting algorithm you are wrapping allows an
additional `size` parameter as a hint (computing the size of a `ForwardIterable` may
be needed but expensive), you can add the following `operator()` overload to your
sorter, and it should properly hide the corresponding overload in `sorter_base`:

```cpp
template<
    typename ForwardIterable,
    typename Compare = std::less<>
>
auto operator()(ForwardIterable iterable, Compare compare={}) const
    -> void
{
    selection_sort(iterable, compare, utility::size(iterable));
}
```

## Sorter traits

While the `selection_sorter` we wrote should work with most of the library, a little
bit of work has still to be done so that it can benefit from `hybrid_adapter`, which
is subjectively the most interesting component in this library. `hybrid_adapter`'s
`operator()` needs to know the iterator category of the sorters it aggregates so that
it can dispatch a call to the most suitable sorter. You can provide this information
by specializing [`sorter_traits`](sorter-traits.md) for your sorter:

```cpp
namespace cppsort
{
    template<>
    struct sorter_traits<selection_sorter>
    {
        using iterator_category = std::forward_iterator_tag;
        static constexpr bool is_stable = false;
    };
}
```

Note that `sorter_traits` also contains information about the stability of the sorting
algorithm. This information is currently unused in the library but still provided for
every sorter and sorter adapter.

## Writing a type-specific sorter

While most sorting algorithms are comparison sorts designed to work with any type for
which a total order exists (given the appropriate comparison function, which tends to
be `std::less<>` by default), it is also possible to write sorters for algorithms that
do not implement a comparison sort. In this chapter, we will see how to make a sorter
to wrap a [counting sort](https://en.wikipedia.org/wiki/Counting_sort).

A couting sort is not a comparison sort since it does not really care that much about
which element is greater than another, but relies on integer an array arithmetics to
perform the sort. That said, it being a non-comparison sort means that it cannot be
given an arbitrary comparison function to perform the sort. The naive implementation
for such a sorter remains trivial:

```cpp
struct counting_sorter:
    cppsort::sorter_base<counting_sorter>
{
    using cppsort::sorter_base<counting_sorter>::operator();

    template<typename ForwardIterator>
    auto operator()(ForwardIterator first, ForwardIterator last) const
        -> void
    {
        counting_sort(first, last);
    }
};
```

The appropriate operations will still be generated by `sorter_base` and you shouldn't
experiment a problem until you try to feed a comparison function to this sorter. That
begin said, non-comparison sorts tend to work only with collections of some specific
types since they generally use the properties of these types to sort the collection.
For example, our `counting_sorter` only works with built-in integer types, but this
information currently does not appear anywhere in the interface.

While it could be possible to trigger a hard error with a `static_assert`, using SFINAE
to trigger a soft error instead is actuallymore interesting:

```cpp
struct counting_sorter:
    cppsort::sorter_base<counting_sorter>
{
    using cppsort::sorter_base<counting_sorter>::operator();

    template<typename ForwardIterator>
    auto operator()(ForwardIterator first, ForwardIterator last) const
        -> std::enable_if_t<
            std::is_integral<
                typename std::iterator_traits<ForwardIterator>::value_type
            >::value
        >
    {
        counting_sort(first, last);
    }
};
```

Using such a soft error mechanism instead of a hard one will make it possible to use
your sorter together with `hybrid_adapter` (provided you specialized `sorter_traits`
accordingly) so that you can make it fallback to a generic sorting algorithm when a
collection not handled by your sorter is given to the aggregate:

```cpp
// This sorter will use couting_sorter if a collection
// of integers is given to it, and will fallback to
// inplace_merge_sorter otherwise
using generic_sorter = cppsort::hybrid_adapter<
    counting_sorter,
    cppsort::inplace_merge_sorter
>;
```

## Rules and guarantees

Enforcing everything from the library side is rather difficult and **cpp-sort** remains
rather permissive. That said, if you don't want to be surprised when implementing your
sorters, you should at least follow a few rules...

* For any sorter called without a comparison function, `std::is_sorted` called without
a comparison function on the resulting collection shall return `true`.
* For any sorter called with a specific comparison function, `std::is_sorted` called
with the same comparison function on the resulting collection shall return `true`.
* Calling a sorter with `std::less<>`, with `std::less<T>` (where `T` is the type of the
elements of the collection to sort) or without a comparison function should be strictly
equivalent: calling `std::is_sorted` without a comparison function on the resulting
collection should return `true`.
* Every sorter callable with at least one comparison function should also be callable
without a comparison function.
* Every sorter which can be called with a collection and a comparison function shall
also be callable with two corresponding iterators and the same comparison functions.

Some parts of the library should still work even if these rules are not enforced, but
enforcing should protect you against surprising behaviour.

## Improving type-specific sorters

The rules above allow to safely break from the black & white world of comparison and
non-comparison sorters, and to write sorters that accept only *some* specific comparison
functions. To support that model, [`sorter_base`](sorter-base.md) adds overloads to
`operator()` which take an `std::less<>` or an `std::less<T>` instance to non-comparison
sorters so that *any* non-comparison sorter can also be called with `std::less<>`, so
you don't have to do that by yourself either.

Now, assume that our `counting_sort` algorithm also has an evil twin, to which we will
give the name of `reverse_counting_sort`. From the following rules, it is pretty clear
that makinga  `reverse_counting_sorter` would be a bad idea since sorters called without
a comparison function are supposed to sort a collection in ascending order. However,
after having reverse-sorted a collection of integers, the following assertion should
hold:

```cpp
assert( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );
```

That means that we can actually safely add `reverse_counting_sort` to `counting_sorter`
by making an overload that takes an instance of `std::greater<>` and still satisfy all
the rules described in the previous section. Here is the overload:

```cpp
template<typename ForwardIterator>
auto operator()(ForwardIterator first, ForwardIterator last, std::greater<>) const
    -> std::enable_if_t<
        std::is_integral<
            typename std::iterator_traits<ForwardIterator>::value_type
        >::value
    >
{
    reverse_counting_sort(first, last);
}
```

Note that for the sorter to be really complete, it would need to take an equivalent
overload for `std::greater<T>`, but `sorter_base` does not automate the generation
of such overloads for you (yet?), so you are bound to write that boilerplate by
yourself. Now, the `hybrid_adapter` from a little while ago just got a bit more
powerful:

```cpp
// This sorter will use couting_sorter if a random-access collection of
// integers is given to it with no comparison function, with std::less
// or with std::greater and will fallback to inplace_merge_sorter in
// any other case
using generic_sorter = cppsort::hybrid_adapter<
    counting_sorter,
    cppsort::inplace_merge_sorter
>;
```
