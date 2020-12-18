If you have read the general tutorial about [[writing sorters|Writing a sorter]], you might be interested in a full concrete example. In this tutorial, we will see how to implement a simple [bubble sort](https://en.wikipedia.org/wiki/Bubble_sort) and how to write a `bubble_sorter` to wrap it. Step by step.

## The bubble sort algorithm

The bubble sort is one of the simplest sorting algorithms to implement: it repeatedly goes through a collection, comparing adjacent elements and switching them if they are not in order, until the collection is sorted. There are some very specific cases where it might be the ideal algorithm, but it is useless most of the time. Anyway, here is a basic implementation taking a pair of iterators like many standard library algorithms:

```cpp
template<typename RandomAccessIterator>
auto bubble_sort(RandomAccessIterator first, RandomAccessIterator last)
    -> void
{
    while (first != last--) {
        for (auto it = first ; it != last ; ++it) {
            if (*(it + 1) < *it) {
                std::iter_swap(it, it + 1);
            }
        }
    }
}
```

This version works with random-access iterators only. That said, making it work with bidirectional iterators too is only a matter of changing the `it + 1` into an `std::next(it)`:

```cpp
template<typename BidirectionalIterator>
auto bubble_sort(BidirectionalIterator first, BidirectionalIterator last)
    -> void
{
    while (first != last--) {
        for (auto it = first ; it != last ; ++it) {
            auto next = std::next(it);
            if (*next < *it) {
                std::iter_swap(it, next);
            }
        }
    }
}
```

Some versions of `bubble_sort` track whether swaps were actually performed during the traversal of the array and return early if no swap was made (it means that the array was already sorted). For the sake of simplicity, we won't add this check to our algorithm.

## A simple `bubble_sorter`

Now that we have a working `bubble_sort` algorithm, we will wrap it into a sorter so that it can benefit from the many tools available in **cpp-sort** when needed. Here is a very basic `bubble_sorter` implementation:

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

Unfortunately, **cpp-sort** requires sorters to implement range-based algorithms too in order to satisfy the `Sorter` concept. Implementing by hand the whole set of features is boring and error-prone. Fortunately, **cpp-sort** provides the utility class template [`sorter_facade`](https://github.com/Morwenn/cpp-sort/wiki/Sorter-facade) to automagically generate the missing features when an iterator-based `operator()` exists:

```cpp
struct bubble_sorter_impl
{
    template<typename BidirectionalIterator>
    auto operator()(BidirectionalIterator first, BidirectionalIterator last) const
        -> void
    {
        bubble_sort(first, last);
    }
};

using bubble_sorter = cppsort::sorter_facade<bubble_sorter_impl>;
```

Now our `bubble_sorter` satisfies the library's requirements and implements all the additional features without too much additional work. However, we might also want it to play nice with [`hybrid_adapter`](https://github.com/Morwenn/cpp-sort/wiki/Sorter-adapters#hybrid_adapter), the main building block which allows to aggregate different sorters together. In order to do so, we need to explicitly document the iterator category the sorter is designed to work with. We can do so by giving it an `iterator_category` type aliasing one of the standard iterator tags:

```cpp
struct bubble_sorter_impl
{
    template<typename BidirectionalIterator>
    auto operator()(BidirectionalIterator first, BidirectionalIterator last) const
        -> void
    {
        bubble_sort(first, last);
    }

    // Sorter traits
    using iterator_category = std::bidirectional_iterator_tag;
    using is_always_stable = std::true_type;
};
```

Documenting the stability of a sorter through `is_always_stable` is required if we want it to work with [`stable_adapter`](https://github.com/Morwenn/cpp-sort/wiki/Sorter-adapters#stable_adapter); this adapter transforms a sorter to make a stable sorter, but explicitly specifying that our `bubble_sorter` is stable will allow `stable_adapter` to skip the transformation and use the sorter as is. Even though it doesn't always matter, accessing these properties should be done via the class [`sorter_traits`](https://github.com/Morwenn/cpp-sort/wiki/Sorter-traits); some sorters don't embed these properties and specialize `sorter_traits` instead to provide them.

## Handling custom comparison functions

Our `bubble_sort` algorithm currently uses `operator<` to compare the elements to be sorted. To make it more generic, we would like it to work with any suitable comparison function instead, just like `std::sort`. Doing so is rather easy:

```cpp
template<
    typename BidirectionalIterator,
    typename StrictWeakOrdering
>
auto bubble_sort(BidirectionalIterator first, BidirectionalIterator last,
                 StrictWeakOrdering compare)
    -> void
{
    while (first != last--) {
        for (auto it = first ; it != last ; ++it) {
            auto next = std::next(it);
            if (compare(*next, *it)) {
                std::iter_swap(it, next);
            }
        }
    }
}
```

**cpp-sort** is designed to handle both comparison and non-comparison sorters. There isn't much to change to make `bubble_sorter` use the new addition to its underlying sorting algorithm:

```cpp
struct bubble_sorter_impl
{
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

    // Sorter traits
    using iterator_category = std::bidirectional_iterator_tag;
    using is_always_stable = std::true_type;
};
```

With this addition, a `bubble_sorter` instance can be called with a custom comparison function or without one, defaulting to `std::less<>` when none is provided. Note that `sorter_facade` generates the appropriate `operator()` overloads so that the sorter can still be called with either a pair of iterators, a pair of iterators and a comparison function, a collection, or a collection and a comparison function. It also ensures that an instance of `bubble_sorter` can be converted to a function pointer corresponding to any of these overloads.

It is possible to improve the comparison handling further by making the algorithm work out-of-the-box for pointer to member functions of the `lhs.compare_to(rhs)` kind. Transforming the passed comparison function with [`cppsort::utility::as_function`](https://github.com/Morwenn/cpp-sort/wiki/Miscellaneous-utilities#as_function) is enough to do the job:

```cpp
template<
    typename BidirectionalIterator,
    typename StrictWeakOrdering
>
auto bubble_sort(BidirectionalIterator first, BidirectionalIterator last,
                 StrictWeakOrdering compare)
    -> void
{
    auto&& comp = cppsort::utility::as_function(compare);

    while (first != last--) {
        for (auto it = first ; it != last ; ++it) {
            auto next = std::next(it);
            if (comp(*next, *it)) {
                std::iter_swap(it, next);
            }
        }
    }
}
```

Note that in C++17, it is preferred to use directly [`std::invoke`](https://en.cppreference.com/w/cpp/utility/functional/invoke) to call the comparison function instead of transforming it with `as_function`.

## Using `bubble_sorter` with forward iterators

In its current state, `bubble_sort` isn't usable with forward iterators because of the `last--` instruction, which requires bidirectional iterators. If we drop this backwards iteration, we have to reintroduce the check to see whether the collection is sorted, which means that the algorithm will generally perform many useless comparisons all over the place. Fortunately, we can use another technique: we know that when the bubble sort traverses the collection for the *nth* time, it is supposed to perform exactly *n* comparisons. While decrementing a forward iterator isn't possible, we can still compute the size of the collection to sort, then decrement it and perform the correct number of comparisons:

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

    auto&& comp = cppsort::utility::as_function(compare);

    while (--size) {
        ForwardIterator current = first;
        ForwardIterator next = std::next(current);
        for (std::size_t i = 0 ; i < size ; ++i) {
            if (comp(*next, *current)) {
                std::iter_swap(current, next);
            }
            ++next;
            ++current;
        }
    }
}
```

The only change to make at the sorter level is to change its declared iterator category, and possibly the names of the template parameters too so that they don't lie about the iterator category.

## Handling projection parameters

Projections are functions that can be used to "view" the values to sort differently during the comparison. Most of the comparison sorters in **cpp-sort** take an optional projection parameter. The bubble sort being a comparison sorter, it may be interesting to have it handle projections too. In order to do that, we will have to alter both the sorting algorithm and the sorter. Fortunately, the modifications are pretty straigthforward: in the algorithm, we only have to add another parameter and use it on the values that are being compared:

```cpp
template<
    typename ForwardIterator,
    typename StrictWeakOrdering,
    typename Projection
>
auto bubble_sort(ForwardIterator first, ForwardIterator last,
                 StrictWeakOrdering compare, Projection projection)
    -> void
{
    auto size = std::distance(first, last);
    if (size < 2) return;

    auto&& comp = cppsort::utility::as_function(compare);
    auto&& proj = cppsort::utility::as_function(projection);

    while (--size) {
        ForwardIterator current = first;
        ForwardIterator next = std::next(current);
        for (std::size_t i = 0 ; i < size ; ++i) {
            if (comp(proj(*next), proj(*current))) {
                std::iter_swap(current, next);
            }
            ++next;
            ++current;
        }
    }
}
```

Note the use of [`utility::as_function`](https://github.com/Morwenn/cpp-sort/wiki/Miscellaneous-utilities#as_function) to transform the projection parameter. While using the raw projection would have been enough in most scenarios, this line makes it possible to pass pointers to member data instead of functions so that the algorithm can sort the collection on a specific field; this is a rather powerful mechanism. Now, to the sorter:

```cpp
struct bubble_sorter_impl
{
    template<
        typename ForwardIterator,
        typename StrictWeakOrdering = std::less<>,
        typename Projection = cppsort::utility::identity,
        typename = std::enable_if_t<cppsort::is_projection_iterator_v<
            Projection, ForwardIterator, StrictWeakOrdering
        >>
    >
    auto operator()(ForwardIterator first, ForwardIterator last,
                    StrictWeakOrdering compare={}, Projection projection={}) const
        -> void
    {
        bubble_sort(first, last, compare, projection);
    }

    // Sorter traits
    using iterator_category = std::forward_iterator_tag;
    using is_always_stable = std::true_type;
};
```

We can see several improvements compared to the previous version: first of all, we added an optional projection parameter which defauts to [`utility::identity`](https://github.com/Morwenn/cpp-sort/wiki/Miscellaneous-utilities#miscellaneous-function-objects). This is a function object that returns a value as is so that the default behaviour of the algorithm is to run *as if* projections didn't exist. It is very likely to be optimized aways by the compiler anyway.

The second modification is one I wish we could do without (yet another thing that concepts would make more expressive): [`is_projection_iterator_v`](https://github.com/Morwenn/cpp-sort/wiki/Sorter-traits#is_projection-and-is_projection_iterator) is a trait that checks whether a projection function can be used on a dereferenced iterator. It also optionally checks that a given comparison function can be called with the result of two such projections. This trait exists to ensure that a sorter's `operator()` won't be called when these conditions are not satisfied, which may be crucial when aggregating sorters with [`hybrid_adapter`](https://github.com/Morwenn/cpp-sort/wiki/Sorter-adapters#hybrid_adapter).

Now that you know how to handle projections in your algorithm, here is the interesting part: you generally don't need to manually handle projections. The class template `sorter_facade` generates overloads of `operator()` taking projection functions that bake the projection into the comparison and forward that mix to the sorter implementation. In our implementation of `bubble_sort`, we always use the projection inside the comparison, so handling the projections by hand isn't giving us any optimization opportunity; we might as well implement just the comparison and add the small required SFINAE check:

```cpp
struct bubble_sorter_impl
{
    template<
        typename ForwardIterator,
        typename StrictWeakOrdering = std::less<>,
        typename = std::enable_if_t<not cppsort::is_projection_iterator_v<
            StrictWeakOrdering, ForwardIterator
        >>
    >
    auto operator()(ForwardIterator first, ForwardIterator last,
                    StrictWeakOrdering compare={}) const
        -> void
    {
        // don't forget to roll back bubble_sort too
        bubble_sort(first, last, compare);
    }

    // Sorter traits
    using iterator_category = std::forward_iterator_tag;
    using is_always_stable = std::true_type;
};
```

## Taking proxy iterators into account

Generic agorithms are good, more generic algorithms are better. The current `bubble_sort` can already be used to sort well-formed every sequence container from the standard library, but it still might not be able to sort everything. Some kinds of iterators obey special rules when it comes to moving or swapping the referenced elements, or at least that's what they should do [in the future](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0022r1.html). Such iterators are currently known as "proxy iterators" and provide custom `iter_swap` and `iter_move` functions meant to be found by argument-dependent lookup. The functions needed to mak the whole thing work are not available in any standard library yet, so **cpp-sort** provides [`utility::iter_move` and `utility::iter_swap`](https://github.com/Morwenn/cpp-sort/wiki/Miscellaneous-utilities#iter_move-and-iter_swap) to replace them. Sorting algorithms should import them in the current scope and perform an unqualified call whenever they need to move or swap dereferenced iterators.

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

    auto&& comp = cppsort::utility::as_function(compare);

    while (--size) {
        ForwardIterator current = first;
        ForwardIterator next = std::next(current);
        for (std::size_t i = 0 ; i < size ; ++i) {
            if (comp(*next, *current)) {
                using cppsort::utility::iter_swap;
                iter_swap(current, next);
            }
            ++next;
            ++current;
        }
    }
}
```

While this improvement shouldn't make a difference most of the time, implementing it is pretty straightforward, incurs no performance cost, and ensures that the algorithm is future-proof and ready for the next standards.

## Final optimizations

Our current version of `bubble_sort` has to compute the size of the collection to sort prior to the actual sort. This is not optimal since some of the containers in the standard library know their size and can provide it in O(1) time, while computing the distance between two iterators might be O(n) depending on their iterator category. **cpp-sort** makes it possible to easily use this information: the function [`utility::size`](https://github.com/Morwenn/cpp-sort/wiki/Miscellaneous-utilities#size) takes a container and returns the result of the member function `size` if the container has one, or `std::distance(std::begin(container), std::end(container))` instead if it doesn't have such a member function. This small tool allows us to rewrite `bubble_sort` and `bubble_sorter` so that they can take advantage of this information when available:

```cpp
template<
    typename ForwardIterator,
    typename StrictWeakOrdering
>
auto bubble_sort(ForwardIterator first, std::size_t size,
                 StrictWeakOrdering compare)
    -> void
{
    if (size < 2) return;

    auto&& comp = cppsort::utility::as_function(compare);

    while (--size) {
        ForwardIterator current = first;
        ForwardIterator next = std::next(current);
        for (std::size_t i = 0 ; i < size ; ++i) {
            if (compare(*next, *current)) {
                using cppsort::utility::iter_swap;
                iter_swap(current, next);
            }
            ++next;
            ++current;
        }
    }
}

struct bubble_sorter_impl
{
    // Pair of iterators overload
    template<
        typename ForwardIterator,
        typename StrictWeakOrdering = std::less<>,
        typename = std::enable_if_t<not cppsort::is_projection_iterator_v<
            StrictWeakOrdering, ForwardIterator
        >>
    >
    auto operator()(ForwardIterator first, ForwardIterator last,
                    StrictWeakOrdering compare={}) const
        -> void
    {
        bubble_sort(first, std::distance(first, last),
                    compare);
    }

    // Iterable overload
    template<
        typename ForwardIterable,
        typename StrictWeakOrdering = std::less<>,
        typename = std::enable_if_t<not cppsort::is_projection_v<
            StrictWeakOrdering, ForwardIterable
        >>
    >
    auto operator()(ForwardIterable&& iterable, StrictWeakOrdering compare={}) const
        -> void
    {
        bubble_sort(std::begin(iterable),
                    cppsort::utility::size(iterable),
                    compare);
    }

    // Sorter traits
    using iterator_category = std::forward_iterator_tag;
    using is_always_stable = std::true_type;
};
```

We used forwarding references to ensure that the range overload will work with lvalue ranges, but also with temporary span-like classes (*e.g. `gsl::span`). Note that `sorter_facade` will make sure to call the new `operator()` overload for ranges instead of dispatching the call to the overload that takes a pair of iterators when given a full collection, so everything should work smoothly enough.

## Instantiating the sorter

The sorter abstraction is useful, but most of the time we only need a sorting algorithms. Therefore, it might be a good idea to instantiate the `bubble_sorter` and to have a global `bubble_sort` instance, more versatile than the original `bubble_sort` algorithm. Declaring global function objects while avoiding ODR problems is a bit tricky pre-C++17 due to the lack of `inline` variables; after having put the original `bubble_sort` and `bubble_sorter_impl` in a `detail` namespace to avoid naming problems, we will create a global instance of `bubble_sorter` with the following pattern:

```cpp
// C++14
namespace
{
    constexpr auto&& bubble_sort
        = cppsort::utility::static_const<bubble_sorter>::value;
}

// C++17
inline constexpr auto&& bubble_sort = bubble_sorter{};
```

The utility [`static_const`](https://github.com/Morwenn/cpp-sort/wiki/Miscellaneous-utilities#static_const) is a variable template used to avoid ODR problem. Understanding the details is a bit tough; you can read [Eric Niebler's original article](https://ericniebler.com/2014/10/21/customization-point-design-in-c11-and-beyond/) about this pattern if you want to learn more about it. Basically it is a poor man's substitute to compensate the lack of `inline` variables pre-C++17.

## Better error messages

We now have a versatile `bubble_sorter`, able to handle many scenarios, and optimized as much as a bubble sort can be. It works really well... until it doesn't. **cpp-sort** has one major drawback (some would say more than one, but...): when not used correctly, the error messages are close to unreadable; forget one `const` and embrace the hundreds of lines of cryptic SFINAE error messages, and I really mean it! It sometimes took me far longer than it should have to find bugs. The sorter works properly, but we can still improve the way it fails...

Starting easy: we can use strong `typedef`s to hide some irrelevant template parameters and shorten some error messages a bit. In our case, we can make `bubble_sorter` *inherit* from `cppsort::sorter_facade<detail::bubble_sorter_impl>` instead of defining it as a type alias. It doesn't improve error messages that much, but at least they will use the name `bubble_sorter` until they have to display the full name.

```cpp
struct bubble_sorter:
    cppsort::sorter_facade<detail::bubble_sorter_impl>
{};
```

One small change that can greatly improve error messages is the addition of a static assertion in `operator()` to assert that the iterator category of the passed collection is compatible with that of the sorter. It doesn't have that much of an impact with `bubble_sorter` since we designed it to work with forward iterators (I hardly see why would anyone pass simple input iterators to it), but it's good practice anyway if you design sorters that only work with more restricted iterator categories. For example, passing an `std::list` to [`heap_sorter`](https://github.com/Morwenn/cpp-sort/wiki/Sorters#heap_sorter) used to spawn more than 70 lines of cryptic error messages with g++ 5.2 (basically, it failed when it encoutered an operation in the heapsort implementation not compatible with bidirectional iterators); with such a static assertion, it came down to 4 lines: the 4th line was the static assertion message, and the one above referenced the faulty call site, which is quite a big improvement.

```cpp
static_assert(
    std::is_base_of<
        std::forward_iterator_tag,
        typename std::iterator_traits<ForwardIterator>::iterator_category
    >::value,
    "bubble_sorter requires at least forward iterators"
);
```

Concepts would probably improve some error messages too, but will have wait for at least C++20. In the current state of the library, many error messages remain pretty noisy and tough to understand, and we can't realistically put static assertions all over the place because too many things rely on SFINAE. That is why even these small improvements to error messages matter; if one can't understand why something fails, they are less likely to fix the error.

And that's it: we have covered pretty much every interesting aspect of writing a simple comparison sorter and we have seen how to implement some small optimizations and care about error messages as well as how to enhance it with projections. I hope you enjoyed the tutorial, even if bubble sort is not the most interesting sorting algorithm around. You can find the full implementation in the examples folder :)
