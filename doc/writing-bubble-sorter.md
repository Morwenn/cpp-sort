# Writing a `bubble_sorter`

If you have read the general tutorial about [writing sorters](writing-sorter.md),
you might be interested in a full concrete example. In this tutorial, we will see
step by step how to implement a simple [bubble sort](https://en.wikipedia.org/wiki/Bubble_sort)
and how to write a `bubble_sorter` to wrap it.

## The bubble sort algorithm

The bubble sort is a simple algorithm that goes through an iterable, comparing
adjacent elements and switching them if they are not in order. There are some
very specific cases where it might be the ideal algorithm, but it is useless most
of the time. Anyway, here a basic implementation, using taking a pair of iterators
like many standard library algorithms:

```cpp
template<typename RandomAccessIterator>
auto bubble_sort(RandomAccessIterator first, RandomAccessIterator last)
    -> void
{
    while (first != last--)
    {
        for (auto it = first ; it != last ; ++it)
        {
            if (*(it + 1) < *it)
            {
                std::iter_swap(it, it + 1);
            }
        }
    }
}
```

This version works with random-access iterators only, but we can see that making
it work with bidirectional iterators too is only a matter of changing the `it + 1`
into an `std::next(it)`:

```cpp
template<typename BidirectionalIterator>
auto bubble_sort(BidirectionalIterator first, BidirectionalIterator last)
    -> void
{
    while (first != last--)
    {
        for (auto it = first ; it != last ; ++it)
        {
            auto next = std::next(it);
            if (*next < *it)
            {
                std::iter_swap(it, next);
            }
        }
    }
}
```

Some versions of `bubble_sort` track whether swaps were actually performed during
the traversal of the array and return early if no swap was made (it means that the
array was already sorted). For the sake of simplicity, we won't add this check to
our algorithm.

## A simple `bubble_sorter`

Now that we have a working `bubble_sort` algorithm, we will wrap it into a sorter
so that it can use the full potential of the **cpp-sort** tools when needed. Here
is a very basic `bubble_sorter` implementation:

```cpp
struct bubble_sorter
{
    template<typename BidirectionalIterator>
    auto operator()(BidirectionalIterator first, BidirectionalIterator last) const
        -> void
    {
        bubble_sort(first, last);
    }
};
```

Unfortunately, **cpp-sort** requires sorters to be convertible to function pointers
and to implement range-based algorithms too. Implement by hand this whole set of
features is boring and error-prone. Fortunately, **cpp-sort** provides an utility
class named [`sorter_facade`](sorter-facade.md) to generate all these feature from an
iterator-based `operator()`:

```cpp
struct bubble_sorter:
    cppsort::sorter_facade<bubble_sorter>
{
    using cppsort::sorter_facade<bubble_sorter>::operator();

    template<typename BidirectionalIterator>
    auto operator()(BidirectionalIterator first, BidirectionalIterator last) const
        -> void
    {
        bubble_sort(first, last);
    }
};
```

Now our `bubble_sorter` satisfies the library's requirements and implements all the
additional features without too much additional work. However, we might also want
it to work in a `cppsort::hybrid_adapter`, the main building block which allows to
merge different sorters together. In order to do so, we will need to specialize the
class [`sorter_traits`](sorter-traits.md) for our `bubble_sorter` to explicitly
document the sorter's stability and the category of iterators it works with:

```cpp
namespace cppsort
{
    template<>
    struct sorter_traits<bubble_sorter>
    {
        using iterator_category = std::bidirectional_iterator_tag;
        static constexpr bool is_stable = true;
    };
}
```

## Handling custom comparison functions

Our `bubble_sort` algorithm currently uses `operator<` to compare the elements to
be sorted. However, we would like it to work with any suitable comparison function
instead, just like `std::sort`. Doing so is rather easy:

```cpp
template<
    typename BidirectionalIterator,
    typename StrictWeakOrdering
>
auto bubble_sort(BidirectionalIterator first, BidirectionalIterator last,
                 StrictWeakOrdering compare)
    -> void
{
    while (first != last--)
    {
        for (auto it = first ; it != last ; ++it)
        {
            auto next = std::next(it);
            if (compare(*next, *it))
            {
                std::iter_swap(it, next);
            }
        }
    }
}
```

**cpp-sort** is designed to handle both comparison and non-comparison sorters.
There isn't much to change to make `bubble_sorter` use the new addition to its
underlying sorting algorithm:

```cpp
struct bubble_sorter:
    cppsort::sorter_facade<bubble_sorter>
{
    using cppsort::sorter_facade<bubble_sorter>::operator();

    template<
        typename BidirectionalIterator,
        typename StrictWeakOrdering = std::less<>
    >
    auto operator()(BidirectionalIterator first, BidirectionalIterator last,
                    StrictWeakOrdering compare={}) const
        -> void
    {
        bubble_sort(first, last, compare);
    }
};
```

With this addition, a `bubble_sorter` instance can be called with a custom
comparison function or without one, defaulting to `std::less<>`. Note that
`sorter_facade` still generates the appropriate overloads so that the sorter
can still be called with either a pair of iterators, a pair of iterators and a
comparison function, a range or a range and a comparison functions. It aso makes
sure that an instance of `bubble_sorter` can be converted to a function pointer
corresponding to any of these four overloads of `operator()`.

## Using `bubble_sorter` with forward iterators

In its current state, `bubble_sort` isn't usable with forward iterators because of
the `last--` which requires bidirectional iterators. If we drop this backwards
iteration, we have to reintroduce the check to see whether the collection is sorted
which means that the algorithm will generally perform many useless comparisons all
over the place. Fortunately, we can use another technique: we know that when the
bubble sort traverses the collection for the *nth* time, it is supposed to perform
*n* comparisons. While decrementing a forward iterator isn't possible, we can still
compute the size of the collection to sort and then decrement it and perform the
correct number of comparisons:

```cpp
template<
    typename ForwardIterator,
    typename StrictWeakOrdering
>
auto bubble_sort(ForwardIterator first, ForwardIterator last,
                 StrictWeakOrdering compare)
    -> void
{
    auto size = std::distance(first, last);
    if (size < 2) return;

    while (--size)
    {
        ForwardIterator current = first;
        ForwardIterator next = std::next(current);
        for (std::size_t i = 0 ; i < size ; ++i)
        {
            if (compare(*next, *current))
            {
                std::iter_swap(current, next);
            }
            ++next;
            ++current;
        }
    }
}
```

The only change to make at the sorter level is to change its declared iterator
category in `sorter_traits` (and possibly the names of the template parameters
in the sorter so that they don't lie about the iterator category):

```cpp
namespace cppsort
{
    template<>
    struct sorter_traits<bubble_sorter>
    {
        using iterator_category = std::forward_iterator_tag;
        static constexpr bool is_stable = true;
    };
}
```

## Final optimizations

Our current version of `bubble_sort` has to compute the size of the collection to
sort prior to the actual sort. This is not optimal since some of the containers in
the standard library know their size and can provide it in O(1). **cpp-sort** makes
it possible to easily use this information: the function `utility::size` takes a
container and returns the results of the member function `size` if the container
has one, and `std::distance(std::begin(container), std::end(container))` instead.
With such a tool, we can rewrite `bubble_sort` and `bubble_sorter` so that they can
use this information when available:

```cpp
template<
    typename ForwardIterator,
    typename StrictWeakOrdering
>
auto bubble_sort(ForwardIterator first, StrictWeakOrdering compare,
                 std::size_t size)
    -> void
{
    if (size < 2) return;

    while (--size)
    {
        ForwardIterator current = first;
        ForwardIterator next = std::next(current);
        for (std::size_t i = 0 ; i < size ; ++i)
        {
            if (compare(*next, *current))
            {
                std::iter_swap(current, next);
            }
            ++next;
            ++current;
        }
    }
}

struct bubble_sorter:
    cppsort::sorter_facade<bubble_sorter>
{
    // Pair of iterators overload
    template<
        typename ForwardIterator,
        typename StrictWeakOrdering = std::less<>
    >
    auto operator()(ForwardIterator first, ForwardIterator last,
                    StrictWeakOrdering compare={}) const
        -> void
    {
        bubble_sort(first, compare, std::distance(first, last));
    }
    
    // Iterable overload
    template<
        typename ForwardIterable,
        typename StrictWeakOrdering = std::less<>
    >
    auto operator()(ForwardIterable& iterable, StrictWeakOrdering compare={}) const
        -> void
    {
        bubble_sort(
            std::begin(iterable), compare,
            cppsort::utility::size(iterable)
        );
    }
};
```

Note that we didn't import any `operator()` overload from `sorter_facade` in this
version of `bubble_sorter`: this is because they are unneeded, we already provide
all the interesting overloads. Moreover, they may make some overloads ambiguous and
we want to avoid that. `sorter_facade` still provides the full set of conversions
to function pointers.

And that's it: we have covered pretty much every interesting aspect of writing a
simple sorter and we have seen how to implement some small optimizations without
too much effort. I hope you enjoyed the tutorial, even if bubble sort is not the
most interesting sorting algorithm around. You can find the full implementation in
the examples folder :)
