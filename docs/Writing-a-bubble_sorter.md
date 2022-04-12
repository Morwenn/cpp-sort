If you have read the general tutorial about [writing sorters][writing-a-sorter], you might be interested in a full concrete example. In this tutorial, we will see how to implement a simple [bubble sort][bubble-sort] and how to write a `bubble_sorter` to wrap it. Step by step.

## The bubble sort algorithm

Bubble sort is one of the simplest sorting algorithms to implement: it repeatedly goes through a collection, comparing adjacent elements and switching them if they are not in order, until the collection is sorted. There are some very specific cases where it might be the ideal algorithm, but most of the time you're better off using another algorithm. Here is a basic implementation taking a pair of iterators like many standard library algorithms:

```cpp
template<typename RandomAccessIterator>
auto bubble_sort(RandomAccessIterator first, RandomAccessIterator last)
    -> void
{
    while (first != last--) {
        for (auto it = first; it != last; ++it) {
            if (*(it + 1) < *it) {
                std::iter_swap(it, it + 1);
            }
        }
    }
}
```

This version only works with random-access iterators only. That said, lowering the accepting iterator category to bidirectional iterators is merely a matter of changing `it + 1` into a more generic `std::next(it)`:

```cpp
template<typename BidirectionalIterator>
auto bubble_sort(BidirectionalIterator first, BidirectionalIterator last)
    -> void
{
    while (first != last--) {
        for (auto it = first; it != last; ++it) {
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

Now that we have a working `bubble_sort` algorithm, we will wrap it into a sorter so that it can benefit from the many tools available in **cpp-sort**. Here is a very basic `bubble_sorter` implementation:

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

Unfortunately, **cpp-sort** requires sorters to implement range-based algorithms too in order to satisfy the *Sorter* requirements, and implementing the whole set of features by hand is boring and error-prone. Fortunately, **cpp-sort** provides [`sorter_facade`][sorter-facade], a class template to automagically generate the missing features when an iterator-based `operator()` is provided:

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

Now our `bubble_sorter` satisfies the library's requirements and implements all the additional features without too much additional work. We could stop there, but there is still some work ahead if we want it to play nice with all the features the library has to offer...

## Sorter traits

For example let's take [`hybrid_adapter`][hybrid-adapter], a [*sorter adapter*][sorter-adapters] which allows to aggregate different sorters together: it needs to know the iterator category of the sorters it aggregates. In order to provide that information, we need to explicitly document the iterator category our sorter is designed to work with by giving it an `iterator_category` type aliasing one of the standard iterator tags:

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

As you might have noticed, the snippet above also provides `is_always_stable`, a trait documenting the stability of the sorter which is notably used by another component: [`make_stable`][stable-adapter]. This adapter transforms any sorter into a stable sorter, but explicitly specifying that our `bubble_sorter` is stable always will allow `make_stable` to skip the transformation and use the sorter directly.

Those traits can be provided directly in the sorter for simplicity, but accessing these properties should be done via [`sorter_traits`][sorter-traits] and related facilities for a variety of reasons: some sorters notably don't embed these properties and specialize `sorter_traits` instead to provide them. As a result, `sorter_traits` should always be considered the main source of truth when querying for sorter porperties.

## Handling custom comparison functions

Our `bubble_sort` algorithm currently uses `operator<` to compare the elements to be sorted. To make it more generic, we would like it to work with any suitable comparison function instead, just like `std::sort`. Doing so is rather easy:

```cpp
template<typename BidirectionalIterator, typename Compare>
auto bubble_sort(BidirectionalIterator first, BidirectionalIterator last,
                 Compare compare)
    -> void
{
    while (first != last--) {
        for (auto it = first; it != last; ++it) {
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
        typename Compare = std::less<>
    >
    auto operator()(BidirectionalIterator first, BidirectionalIterator last,
                    Compare compare={}) const
        -> void
    {
        bubble_sort(first, last, std::move(compare));
    }

    // Sorter traits
    using iterator_category = std::bidirectional_iterator_tag;
    using is_always_stable = std::true_type;
};
```

With this addition, a `bubble_sorter` instance can be called with a custom comparison function or without one, defaulting to `std::less<>` when none is provided. Note that [`sorter_facade`][sorter-facade] generates the appropriate `operator()` overloads so that the sorter can still be called with either a pair of iterators or a range, with or without a comparison function. It also ensures that an instance of `bubble_sorter` can be converted to a function pointer corresponding to any of those overloads.

The handling of comparison functions can be further improved by making the algorithm work out-of-the-box for pointer to member functions of the `lhs.compare_to(rhs)` kind. This can be done either by transforming the passed comparison function with [`cppsort::utility::as_function`][as-function] or by using [`std::invoke`][std-invoke] (C++17 feature):

```cpp
template<typename BidirectionalIterator, typename Compare>
auto bubble_sort(BidirectionalIterator first, BidirectionalIterator last,
                 Compare compare)
    -> void
{
    auto&& comp = cppsort::utility::as_function(compare);

    while (first != last--) {
        for (auto it = first; it != last; ++it) {
            auto next = std::next(it);
            if (comp(*next, *it)) {
                std::iter_swap(it, next);
            }
        }
    }
}
```

## Using `bubble_sorter` with forward iterators

In its current state, `bubble_sort` isn't usable with forward iterators because of the `last--` operation, which requires bidirectional iterators. In order to drop this backwards iteration without performing lots of extra useless operations, we can use the following technique: we know that when bubble sort traverses the collection for the *nth* time, it is supposed to perform *size - n* comparisons (the last *n* elements are already in order). While decrementing a forward iterator isn't possible, we can still compute the size of the collection to sort then decrement it and perform the correct number of comparisons:

```cpp
template<typename ForwardIterator, typename Compare>
auto bubble_sort(ForwardIterator first, ForwardIterator last,
                 Compare compare)
    -> void
{
    auto size = std::distance(first, last);
    if (size < 2) return;

    auto&& comp = cppsort::utility::as_function(compare);

    while (--size) {
        auto current = first;
        auto next = std::next(current);
        for (std::size_t i = 0; i < size; ++i) {
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

[Projections][projections] are functions and function-like objects that can be used to "view" the values to sort differently during the comparison. Most of the comparison sorters in **cpp-sort** take an optional projection parameter. Our `bubble_sorter` being a comparison sorter, it may be interesting to have it handle projections too. In order to do that, we will have to alter both the sorting algorithm and the sorter. Fortunately, the modifications are pretty straigthforward: in the algorithm, we only have to add another parameter and use it on the values that are being compared:

```cpp
template<
    typename ForwardIterator,
    typename Compare,
    typename Projection
>
auto bubble_sort(ForwardIterator first, ForwardIterator last,
                 Compare compare, Projection projection)
    -> void
{
    auto size = std::distance(first, last);
    if (size < 2) return;

    auto&& comp = cppsort::utility::as_function(compare);
    auto&& proj = cppsort::utility::as_function(projection);

    while (--size) {
        auto current = first;
        auto next = std::next(current);
        for (std::size_t i = 0; i < size; ++i) {
            if (comp(proj(*next), proj(*current))) {
                std::iter_swap(current, next);
            }
            ++next;
            ++current;
        }
    }
}
```

Note the use of [`utility::as_function`][as-function] again to transform the projection parameter. While using the raw projection would have been enough in most scenarios, this line makes it possible to pass pointers to data members instead of functions to sort the collection on a specific field; this is a rather powerful mechanism. Now, to the sorter:

```cpp
struct bubble_sorter_impl
{
    template<
        typename ForwardIterator,
        typename Compare = std::less<>,
        typename Projection = cppsort::utility::identity,
        typename = std::enable_if_t<cppsort::is_projection_iterator_v<
            Projection, ForwardIterator, Compare
        >>
    >
    auto operator()(ForwardIterator first, ForwardIterator last,
                    Compare compare={}, Projection projection={}) const
        -> void
    {
        bubble_sort(first, last, std::move(compare), std::move(projection));
    }

    // Sorter traits
    using iterator_category = std::forward_iterator_tag;
    using is_always_stable = std::true_type;
};
```

We can see several improvements compared to the previous version: first of all, we added an optional projection parameter which defauts to [`utility::identity`][utility-identity] (in C++20 we would use [`std::identity`][std-identity]). This is a function object that takes a value and returns it as is so that the default behaviour of the algorithm is to run *as if* projections didn't exist. It is very likely to be optimized aways by the compiler.

The second modification is one I wish we could do without (but will have to live with until concepts): [`is_projection_iterator_v`][is-projection] is a trait that checks whether a projection function can be used on a dereferenced iterator. It also optionally checks that a given comparison function can be called with the result of two such projections. This trait exists to ensure that a sorter's `operator()` won't be called when these conditions are not satisfied, which may be crucial when aggregating sorters with [`hybrid_adapter`][hybrid-adapter].

Now that we saw how to handle projections in your algorithm, here is the interesting part: you generally don't need to manually handle projections. [`sorter_facade`][sorter-facade] generates overloads of `operator()` taking projection functions that bake the projection directly into the comparison and forward that mix to the sorter implementation. In our implementation of `bubble_sort`, we always use the projection inside the comparison, so handling the projections by hand isn't giving us any optimization opportunity; we might as well just implement the comparison and add the small required SFINAE check:

```cpp
struct bubble_sorter_impl
{
    template<
        typename ForwardIterator,
        typename Compare = std::less<>,
        typename = std::enable_if_t<not cppsort::is_projection_iterator_v<
            Compare, ForwardIterator
        >>
    >
    auto operator()(ForwardIterator first, ForwardIterator last,
                    Compare compare={}) const
        -> void
    {
        // Don't forget to roll back bubble_sort too
        bubble_sort(first, last, std::move(compare));
    }

    // Sorter traits
    using iterator_category = std::forward_iterator_tag;
    using is_always_stable = std::true_type;
};
```

## Proxy iterators

Generic agorithms are good, more generic algorithms are sometimes better. The current `bubble_sort` can already be used to sort every well-formed sequence container from the standard library, yet it still might not be able to sort everything: think of [`std::vector<bool>`][std-vector-bool] where you're not swapping actual values, but proxy objects representing the stored values - though implementations sometimes "make it work".

C++20 ranges introduces the notion of ["proxy iterators"][proxy-iterators], which are basically iterators that can't yield a proper reference to the object they point to, but instead yield a proxy object acting as a reference. In order to handle such iterators, C++20 introduces the *customization point objects* [`std::ranges::iter_move`][std-iter-move] and [`std::ranges::iter_swap`][std-iter-swap] which should be used instead of `std::move(*it)` and `std::iter_swap(it1, it2)` in generic algorithms that aim to support proxy iterators.

**cpp-sort** being a C++14 library, it can't rely on these CPOs and provides the utility functions [`utility::iter_move` and `utility::iter_swap`][utility-iter-move] to replace them. They are a bit cruder than their standard equivalents you have to import them into the current namespace and perform an unqualified call, *à la* `std::swap`.

```cpp
template<typename ForwardIterator, typename Compare>
auto bubble_sort(ForwardIterator first, ForwardIterator last,
                 Compare compare)
    -> void
{
    auto size = std::distance(first, last);
    if (size < 2) return;

    auto&& comp = cppsort::utility::as_function(compare);

    while (--size) {
        auto current = first;
        auto next = std::next(current);
        for (std::size_t i = 0; i < size; ++i) {
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

## Final optimizations

Our current version of `bubble_sort` has to compute the size of the collection to sort prior to the actual sort. This is not optimal since some containers such as [`std::list`][std-list] know their size and can provide it in O(1) time, while computing the distance between two iterators would be O(n) time. **cpp-sort** makes it possible to easily use this information: the function [`utility::size`][utility-size] takes a container and returns the result of the member function `size` if the container has one, or `std::distance(std::begin(container), std::end(container))` otherwise. This tool allows us to rewrite `bubble_sort` and `bubble_sorter` so that they can take advantage of this information when available:

```cpp
template<typename ForwardIterator, typename Compare>
auto bubble_sort(ForwardIterator first, std::size_t size,
                 Compare compare)
    -> void
{
    if (size < 2) return;

    auto&& comp = cppsort::utility::as_function(compare);

    while (--size) {
        auto current = first;
        auto next = std::next(current);
        for (std::size_t i = 0; i < size; ++i) {
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
        typename Compare = std::less<>,
        typename = std::enable_if_t<not cppsort::is_projection_iterator_v<
            Compare, ForwardIterator
        >>
    >
    auto operator()(ForwardIterator first, ForwardIterator last,
                    Compare compare={}) const
        -> void
    {
        bubble_sort(first, std::distance(first, last),
                    std::move(compare));
    }

    // Iterable overload
    template<
        typename ForwardIterable,
        typename Compare = std::less<>,
        typename = std::enable_if_t<not cppsort::is_projection_v<
            Compare, ForwardIterable
        >>
    >
    auto operator()(ForwardIterable&& iterable, Compare compare={}) const
        -> void
    {
        bubble_sort(std::begin(iterable),
                    cppsort::utility::size(iterable),
                    std::move(compare));
    }

    // Sorter traits
    using iterator_category = std::forward_iterator_tag;
    using is_always_stable = std::true_type;
};
```

We use forwarding references to ensure that the range overload works with lvalue ranges, but also with temporary [`std::span`][std-span]-like classes. Note that [`sorter_facade`][sorter-facade] will call the new `operator()` overload for ranges instead of dispatching the call to the overload that takes a pair of iterators when given a full collection, so everything should work smoothly enough.

## Instantiating the sorter

The sorter abstraction is useful, but most of the time we only need a sorting algorithm. Therefore, it might be a good idea to instantiate `bubble_sorter` and to have a global `bubble_sort` instance, more versatile than the original `bubble_sort` algorithm.

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

The combination of [`utility::static_const`][utility-static-const] with an anonymous namespace is a trick used to avoid ODR problems; you can read more about how and why it works in [Eric Niebler's original article](https://ericniebler.com/2014/10/21/customization-point-design-in-c11-and-beyond/). It is basically a poor man's substitute to compensate the lack of `inline` variables pre-C++17.

## Better error messages

We now have a versatile `bubble_sorter`, able to handle many scenarios, and optimized as much as a bubble sort can be without turning it into a different algorithm. It works really well... until it doesn't. **cpp-sort** has one major drawback there: when not used correctly, the error messages are often close to unreadable; forget one `const` and embrace the hundreds of lines of cryptic SFINAE error messages, and I really mean it!. The sorter works properly, but we can still somewhat improve the way it fails.

Starting easy: we can use strong `typedef`s to hide some irrelevant template parameters and shorten some error messages a bit. In our case, we can make `bubble_sorter` *inherit* from `cppsort::sorter_facade<detail::bubble_sorter_impl>` instead of defining it as a type alias. It doesn't improve error messages all that much, but at least they will show the name `bubble_sorter` as long as they have to display the full name.

```cpp
struct bubble_sorter:
    cppsort::sorter_facade<detail::bubble_sorter_impl>
{};
```

Another small change that can greatly improve error messages is the addition of a static assertion in `operator()` to assert that the iterator category of the passed collection is compatible with that of the sorter. It doesn't have that much of an impact with `bubble_sorter` since we designed it to work with forward iterators, but it's good practice anyway if you design sorters that only work with more restricted iterator categories. For example, passing an [`std::list`][std-list] to [`heap_sorter`][heap-sorter] used to spawn more than 70 lines of cryptic error messages with g++ 5.2 (basically, it failed when it encoutered an operation in the heapsort implementation not compatible with bidirectional iterators); with such a static assertion, it came down to 4 lines: the 4th line was the static assertion message, and the one above referenced the faulty call site, which is quite a big improvement.

```cpp
static_assert(
    std::is_base_of<
        iterator_category,
        typename std::iterator_traits<ForwardIterator>::iterator_category
    >::value,
    "bubble_sorter requires at least forward iterators"
);
```

Concepts might improve some error messages too, but they're out of scope for **cpp-sort** 1.x. In the current state of the library, many error messages remain pretty noisy and tough to understand, and we can't realistically put static assertions all over the place because too many things rely on SFINAE. That is why even these small improvements to error messages matter: if one can't understand why something fails, they are less likely to fix the error.

## Conclusion

That's it: we have covered pretty much every interesting aspect of writing a simple comparison sorter. I hope you enjoyed the tutorial, even if bubble sort is not the most interesting sorting algorithm around. You can find the full implementation in the examples folder :)


  [as-function]: Miscellaneous-utilities.md#as_function
  [bubble-sort]: https://en.wikipedia.org/wiki/Bubble_sort
  [bubble-sorter]: https://en.wikipedia.org/wiki/Bubble_sort
  [heap-sorter]: Sorters.md#heap_sorter
  [hybrid-adapter]: Sorter-adapters.md#hybrid_adapter
  [is-projection]: Sorter-traits.md#is_projection-and-is_projection_iterator
  [projections]: https://ezoeryou.github.io/blog/article/2019-01-22-ranges-projection.html
  [proxy-iterators]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0022r2.html
  [sorter-adapters]: Sorter-adapters.md
  [sorter-facade]: Sorter-facade.md
  [sorter-traits]: Sorter-traits.md#sorter_traits
  [stable-adapter]: Sorter-adapters.md#stable_adapter-make_stable-and-stable_t
  [std-identity]: https://en.cppreference.com/w/cpp/utility/functional/identity
  [std-invoke]: https://en.cppreference.com/w/cpp/utility/functional/invoke
  [std-iter-move]: https://en.cppreference.com/w/cpp/iterator/ranges/iter_move
  [std-iter-swap]: https://en.cppreference.com/w/cpp/iterator/ranges/iter_swap
  [std-list]: https://en.cppreference.com/w/cpp/container/list
  [std-span]: https://en.cppreference.com/w/cpp/container/span
  [std-vector-bool]: https://en.cppreference.com/w/cpp/container/vector_bool
  [utility-identity]: Miscellaneous-utilities.md#miscellaneous-function-objects
  [utility-iter-move]: Miscellaneous-utilities.md#iter_move-and-iter_swap
  [utility-size]: Miscellaneous-utilities.md#size
  [utility-static-const]: Miscellaneous-utilities.md#static_const
  [writing-a-sorter]: Writing-a-sorter.md
