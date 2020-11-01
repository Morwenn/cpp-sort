Someday, you might want to use a specific sorting algorithm that does not exist in **cpp-sort** (there are plenty of those) but still benefit from the tools available in this library. In order to do so, you would have to wrap such an algorithm into a sorter. In this tutorial, we will see the quirks involved in writing a good sorter to wrap a sorting algorithm. Writing a basic sorter is easy, but getting everything right to the details might be a bit tricky from time to time. This guide should help you to make the good choices (well, at least I hope so).

*This tutorial is mostly a collection of good practice, tips, and design considerations. For a full step-by-step tutorial to write a sorter, you can read [Writing a `bubble_sorter`](https://github.com/Morwenn/cpp-sort/wiki/writing-a-bubble_sorter).*

## Which kind of sorter

Writing a proper **cpp-sort**-compatible sorter generally implies some familiarity with library's [nomenclature](https://github.com/Morwenn/cpp-sort/wiki/Library-nomenclature): each term of art might imply design decisions and tricky things to take into account. These terms of art can be converted into a series of questions to answer before designing a sorter:
* Is it a *fixed-size sorter*? Is it meant to sort a collection of arbitrary size or a fixed number of values?
* Which *category of iterators* does it work with? Can it be used to sort an `std::forward_list`?
* Does it implement a *comparison sort*? Does it handle arbitrary comparison functions?
* If it is a comparison sorter, does it handle *projections* too?
* Is it a *type-specific sorter*? If so, which types does it work with?
* Is it *stable*? If not, does it have an obvious stable variant?
* Is it a *stateful sorter* or is it a *stateless sorter*?
* Is it a *buffered sorter*? Can it take advantage of a buffer of any size to improve performance?

This tutorial describes what should be taken into account when writing a sorter, depending on the answers given to the questions above. Some things are obvious, some are not, some may be a bit too clever in the end. If something ever seems too clever, not obvious enough or outright broken - or simply if you think that it could use a bit more detail -, don't hesitate to submit an issue.

## The most basic sorter

Let's put *fixed-size sorters* aside for a while since they are a really special breed of sorters, and concentrate on sorters meant to sort collections of any size. Let's assume that we already have the following `selection_sort` algorithm, which implements a [selection sort](https://en.wikipedia.org/wiki/Selection_sort) in the most straightforward way, taking a pair of iterators like many standard library algorithms, and sorting the corresponding range in-place:

```cpp
template<typename ForwardIterator>
auto selection_sort(ForwardIterator first, ForwardIterator last)
    -> void
{
    for (auto it = first ; it != last ; ++it) {
        auto selection = std::min_element(it, last);
        using std::iter_swap;
        iter_swap(selection, it); 
    }
}
```

In order to use it with **cpp-sort**, we will need to wrap it into a sorter object. Doing so is actually trivial; here is the sorter:

```cpp
struct selection_sorter_impl
{
    template<typename ForwardIterator>
    auto operator()(ForwardIterator first, ForwardIterator last) const
        -> void
    {
        selection_sort(first, last);
    }
};

struct selection_sorter:
    cppsort::sorter_facade<selection_sorter_impl>
{};
```

We just wrote what we call a *sorter implementation* and wrapped it into [`sorter_facade`](https://github.com/Morwenn/cpp-sort/wiki/Sorter-facade), a class template designed to provide additional features to a given sorter implementation. We only had to write an `operator()` that takes a pair of iterators and forwards it to `selection_sort`, yet the resulting `selection_sorter` also has an `operator()` overload which can be passed a full collection instead of a pair of iterators, and it is additionally convertible to function pointers of type `void(*)(Iterator, Iterator)` and `void(*)(Iterable&)`. This is what `sorter_facade` gives you for even the most basic sorter, without any effort.

Now, let's define a set of rules to apply when writing sorters. These rules don't *have* to be enforced, but enforcing them will ensure that a sorter will work smoothly with most tool available in this library. In this tutorial, every section will define a small set of rules instead of defining all of them at once without introducing the relevant concepts first. Fortunately, the simpler the sorter, the simpler the rules.

**Rule 1.1:** for any *sorter*, [`std::is_sorted`](http://en.cppreference.com/w/cpp/algorithm/is_sorted) called without a comparison function on the resulting range shall return `true` (note that this is not exactly true: floating point numbers are an example of types that will almost always cause problems).

**Rule 1.2:** a *sorter* shall be callable with either a pair of iterators or an iterable.

**Rule 1.3:** a *sorter implementation* shall provide at least an overload of `operator()` that takes a pair of iterators. Overloads of `operator()` taking an iterable can be provided as well when they add value to the *sorter* (optimization, correctness, better error messages...) but should never totally replace the overload taking a pair of iterators.

**Rule 1.4:** *sorters* shall be immutable and every overload of `operator()` shall explicitly be marked `const` (make sure to check twice: forgetting to `const`-qualify them can cause hundreds of lines of cryptic error messages). Some parts of the library *may* accept mutable sorters, but that's never guaranteed unless specified otherwise.

**Rule 1.5:** *sorter* implementers are encouraged but not required to provide a default instance of their *sorters* for convenience. `inline` variables (C++17) or the library's [`static_const`](https://github.com/Morwenn/cpp-sort/wiki/Miscellaneous-utilities#static_const) utility (C++14) can be used to avoid ODR-related problems.

## Category of iterators

When writing a sorter, one of the most important things to consider is the [category of iterators](http://en.cppreference.com/w/cpp/iterator) it is meant to work with. It directly influences the kinds of collections that the sorter will be able to sort. Sorters implement in-place sorting algorithms, therefore they can only sort forward iterators or more specific types. **cpp-sort** does more than document the sorter category a sorter is supposed to work with: it actually embeds the information directly into the *sorter implementation* itself. If we take the `selection_sorter` from the previous section, we can document its properties as follows:

```cpp
struct selection_sorter_impl
{
    template<typename ForwardIterator>
    auto operator()(ForwardIterator first, ForwardIterator last) const
        -> void
    { /* see above */ }

    // Sorter traits
    using iterator_category = std::forward_iterator_tag;
    using is_always_stable = std::false_type;
};
```

The standard library's [iterator tags](http://en.cppreference.com/w/cpp/iterator/iterator_tags) are used to document the iterator category supported by the sorter (stability is also documented but we'll come back to that later). It is a bit useful for error messages, but some other tools from the library rely of this information. For example [`hybrid_adapter`](https://github.com/Morwenn/cpp-sort/wiki/Sorter-adapters#hybrid_adapter) can take several sorters with different iterator categories and generate a new sorter that will call the appropriate sorter depending on the iterator category of the passed collection:

```cpp
using sorter = cppsort::hybrid_adapter<
    forward_sorter,
    bidirectional_sorter,
    random_access_sorter
>;

sorter{}(std::vector<int>{});        // calls random_access_sorter
sorter{}(std::forward_list<int>{});  // calls forward_sorter
sorter{}(std::list<int>{});          // calls bidirectional_sorter
```

Say you have several sorters with the same iterator category (`std::forward_iterator_tag`) but you know that one of them is better with forward iterators while the other one is better when it comes to random-access and bidirectional iterators. You can use [`rebind_iterator_category`](https://github.com/Morwenn/cpp-sort/wiki/Sorter-traits#rebind_iterator_category) so that the second sorter is considered as a bidirectional sorter instead:

```cpp
using sorter = cppsort::hybrid_adapter<
    forward_sorter1,
    cppsort::rebind_iterator_category<
        forward_sorter2,
        std::bidirectional_iterator_tag
    >
>;
```

If you ever need to access the iterator category of a sorter, don't directly ask for it, use  [sorter traits](https://github.com/Morwenn/cpp-sort/wiki/Sorter-traits#sorter_traits) instead. It shouldn't make a difference when using the regular sorters provided by **cpp-sort**, but the library's *fixed-size sorters* are an example of sorters for which the traits are not embedded in the sorter but provided as a specialization of `sorter_traits` (mainly for maintainability reasons).

As you can see, the iterator category supported by a given sorter is not only there for documentation. You have tools to play with it and tools that actually need the information, so make sure to get it right when designing your sorters.

**Rule 2.1:** to document the *iterator category* of a *sorter*, it shall be given an `iterator_category` type aliasing one of the standard iterators tags. If the sorter can't be altered, `sorter_traits` shall be specialized instead.

**Rule 2.2:** the *iterator category* of a *sorter* shall always be accessed via the library's `sorter_traits` or the corresponding `iterator_category` trait.

**Rule 2.3:** the *iterator category* of a *sorter* shall correspond to the least specific iterator category that the sorter can handle.

## Comparison sorters

Most sorting algorithms are [comparison sorts](https://en.wikipedia.org/wiki/Comparison_sort). It means that, to sort the elements of a collection, they repeatedly use a comparison function that returns whether two elements are already in order. The standard library's [`std::sort`](http://en.cppreference.com/w/cpp/algorithm/sort) implicitly uses an ADL-found `operator<` to compare two elements, but it also provides an overload which takes a user-provided comparison function to compare two elements. **cpp-sort** loosely follows this design (it defaults to `std::less<>` instead of `operator<`) and allows its sorters to take an additional parameter for user-provided comparison functions. Let's write a *sorter implementation* to wrap the three-parameter overload of `std::sort`:

```cpp
struct std_sorter_impl
{
    template<
        typename RandomAccessIterator,
        typename Compare = std::less<>
    >
    auto operator()(RandomAccessIterator first, RandomAccessIterator last
                    Compare compare={}) const
        -> void
    {
        std::sort(first, last, compare);
    }

    // Sorter traits
    using iterator_category = std::random_access_iterator_tag;
    using is_always_stable = std::false_type;
};

struct std_sorter:
    cppsort::sorter_facade<std_sorter_impl>
{};
```

Compared to the previous `selection_sorter_impl`, the only things we had to add was a template parameter defaulted to [`std::less<>`](http://en.cppreference.com/w/cpp/utility/functional/less_void) and a default-contructed (when not provided) parameter to `operator()`. As usual, [`sorter_facade`](https://github.com/Morwenn/cpp-sort/wiki/Sorter-facade) generates a bunch of additional features: it still adds the overload of `operator()` taking a single iterable, but also adds an overload taking an iterable and a comparison function. Basically, it ensures that you always only have to provide a single overload of `operator()`, and generates all the other ones as well as all the corresponding conversions to function pointers.

This kind of comparison sorters help to compare things that don't have an overloaded `operator<` or to compare things differently. For example, passing [`std::greater<>`](http://en.cppreference.com/w/cpp/utility/functional/greater_void) to a sorter instead of `std::less<>` sorts a collection in descending order:

```cpp
// Sort collection in reverse order with std::sort
cppsort::std_sort(collection, std::greater<>{});
```

It is worth noting that every *comparison sorter* provided by the library transforms the comparison parameter with [`utility::as_function`](https://github.com/Morwenn/cpp-sort/wiki/Miscellaneous-utilities#as_function) before actually using it. It allows to use pointers to member functions of the `lhs.compare_to(rhs)` kind out-of-the-box.

The rules for *comparison sorters* are but an extension to the rules defined for basic sorters. You will see that they are very similar.

**Rule 3.1:** a *comparison sorter* is also a *sorter*, which means that it shall be called without a comparison function and shall obey all the rules defined for regular *sorters*.

**Rule 3.2:** for any *comparison sorter* called with a specific comparison function, [`std::is_sorted`](http://en.cppreference.com/w/cpp/algorithm/is_sorted) called with the same comparison function on the resulting collection shall return `true`.

**Rule 3.3:** calling a *comparison sorter* with `std::less<>` or without a comparison function shall be strictly equivalent: calling `std::is_sorted` without a comparison function on the resulting collection shall return `true`.

**Rule 3.4:** a *comparison sorter* which can be called with a collection and a comparison function shall also be callable with two corresponding iterators and the same comparison function.

**Rule 3.5:** the comparison parameter always comes after the parameters corresponding to the collection to sort.

## Handling projections

The [Ranges TS](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4560.pdf) introduces the notion of callable *projections*, borrowed from the [Adobe Source Libraries](http://stlab.adobe.com/). A projection is a callable object that can be passed to an algorithm so that it "views" the values to be compared differently. For example, [`std::negate<>`](http://en.cppreference.com/w/cpp/utility/functional/negate_void) could be used to sort a collection of integers in descending order. Let's assume that our `selection_sort` algorithm from a while has been given a fourth parameter to handle projections; here is the corresponding *sorter implementation*: 

```cpp
struct selection_sorter_impl
{
    template<
        typename ForwardIterator,
        typename Compare = std::less<>,
        typename Projection = cppsort::utility::identity,
        typename = std::enable_if_t<cppsort::is_projection_iterator<
            Projection, ForwardIterator, Compare
        >>
    >
    auto operator()(ForwardIterator first, ForwardIterator last,
                    Compare compare={}, Projection projection={}) const
        -> void
    {
        selection_sort(first, last, compare, projection);
    }

    // Sorter traits
    using iterator_category = std::forward_iterator_tag;
    using is_always_stable = std::false_type;
};
```

As you can see, extending the sorter to handle projections is similar to extending it to support comparisons. Note that this sorter is both a *comparison sorter* and a *projection sorter*, but some sorters can also handle projections without handling comparisons. While the default function object for comparisons is `std::less`, the equivalent default function object for projections is [`utility::identity`](https://github.com/Morwenn/cpp-sort/wiki/Miscellaneous-utilities#miscellaneous-function-objects) (and probably `std::identity` in a future revision of the C++ standard): it simply returns the passed value as is.

The only subtle trick in the example above is the use of [`is_projection_iterator`](https://github.com/Morwenn/cpp-sort/wiki/Sorter-traits#is_projection-and-is_projection_iterator): this trait is used to disambiguate comparison functions from projection functions when a sorter can be called with both, and is not needed when the sorter is only a *projection sorter*. It is actually some kind of concept check and should go away when concepts make their way to the standard.

Note that most of the algorithms (actually, every *projection sorter* provided by the library) transform the projection parameter with [`utility::as_function`](https://github.com/Morwenn/cpp-sort/wiki/Miscellaneous-utilities#as_function) before actually using it. This small tool allows to use pointers to member data as projections; here is an example of how it can be used:

```cpp
struct wrapper { int value; }
std::vector<wrapper> vec = { {5}, {9}, {6}, {1}, {2}, {8}, {3}, {0}, {7}, {4} };
cppsort::selection_sort(vec, &wrapper::value);
```

Thanks to that small trick, the `selection_sorter` will sort `vec`, using the member data `wrapper::value` instead of a full `wrapper` instance (which cannot be compared) to perform the comparisons on.

Algorithms generally apply the projection on-the-fly to the values when they are compared. Some algorithms are a bit more subtle and project a specific object once when they know that its projection will be used extensively (*e.g.* quicksort projects a pivot once before calling the partition function). If projections are really expensive, one can still use [`schwartz_adapter`](https://github.com/Morwenn/cpp-sort/wiki/Sorter-adapters#schwartz_adapter), which projects every element of the collection once prior to the sorting.

Now that you know everything about implementing projections... it's time tell you that you don't actually need to implement them most of the time: if you create a *sorter implementation* with an `operator()` taking only two iterators and a comparison function, `sorter_facade` will create additional overloads that accept projection parameters, bake the projection into the comparison function and pass that hybrid to the appropriate overload of `operator()` in the *sorter implementation*. You only need to add a small SFINAE guard to disambiguate between comparison and  projection functions:

```cpp
struct selection_sorter_impl
{
    template<
        typename ForwardIterator,
        typename Compare = std::less<>,
        typename = std::enable_if_t<not cppsort::is_projection_iterator<
            Compare, ForwardIterator
        >>
    >
    auto operator()(ForwardIterator first, ForwardIterator last,
                    Compare compare={}) const
        -> void
    {
        selection_sort(first, last, compare /* yes, we also have to change that */);
    }

    // Sorter traits
    using iterator_category = std::forward_iterator_tag;
    using is_always_stable = std::false_type;
};
```

The general rules for *projection sorters* are really close to the ones for *comparison sorters*, and there are additional rules when a sorter is both at once. In the following section, assume a projection-enhanced `std::is_sorted`, even though no such function has been standardized yet (there is one in the Ranges TS).

**Rule 4.1:** a *projection sorter* is also a *sorter*, which means that it shall be called without a projection function and shall obey all the rules defined for regular *sorters*.

**Rule 4.2:** a sorter can be both a *comparison sorter* and a *projection sorter* at once. Such a sorter shall also obey all the rules defined for *sorters*, for *comparison sorters* and for *projection sorters*.

**Rule 4.3:** for any *projection sorter* called with a specific projection function, [`std::is_sorted`](http://en.cppreference.com/w/cpp/algorithm/is_sorted) called with `std::less<>` and the same projection function on the resulting collection shall return `true`. If the *projection sorter* is also a *comparison sorter*, for any such sorter called with a specific pair of comparison and projection function, `std::is_sorted` called with the same pair of functions on the resulting collection shall return `true`.

**Rule 4.4:** calling a *projection sorter* with [`utility::identity`](https://github.com/Morwenn/cpp-sort/wiki/Miscellaneous-utilities#miscellaneous-function-objects) or without a projection function shall be strictly equivalent: calling `std::is_sorted` without a comparison function and without a projection function on the resulting collection shall return `true`. If the *projection sorter* is also a *comparison sorter*, calling such a sorter with any valid combination of `std::less<>` and `utility::identity`, and calling it without any additional function should be strictly equivalent.

**Rule 4.5:** a *projection sorter* which can be called with a collection and a projection function shall also be callable with two corresponding iterators and the same projection function. If the *projection sorter* is also a *comparison sorter*, it shall also be callable with an iterable, a comparison function and a projection function.

**Rule 4.6:** the projection parameter always comes after the parameters corresponding to the collection to sort. If the *projection sorter* is also a *comparison sorter*, the projection parameter always comes after the comparison parameter.

**Rule 4.7 (warning):** there is no preferred style between providing a projection overload or relying on the comparison parameter alone to handle comparisons and projections at once. Some algorithms may reduce the number of calls to the projection function, but otherwise there is no reason to hard-code the projection support.

## Non-comparison sorters

While most of the well-known sorting algorithms handle user-provided comparison functions, some do not. It might be for several reasons: the sorter might wrap an algorithm that does not support such custom comparisons, or the algorithm may simply not be based on comparison (*e.g.* [radix sort](https://en.wikipedia.org/wiki/Radix_sort)). Let's assume that we have got our hands on a `counting_sort` function, implementing a [counting sort](https://en.wikipedia.org/wiki/Counting_sort) algorithm. We can trivially write a *sorter implementation* to wrap it:

```cpp
struct counting_sorter_impl
{
    template<typename ForwardIterator>
    auto operator()(ForwardIterator first, ForwardIterator last) const
        -> void
    {
        counting_sort(first, last);
    }

    // Sorter traits
    using iterator_category = std::random_access_iterator_tag;
    using is_always_stable = std::false_type;
};
```

Until there, everything is fine. However, imagine that the library where we found the `counting_sort` function also provides its evil twin, to which we will give the inventive name of `reverse_counting_sort`, meant to sort a collection of integers in descending order. We would like to take advantage of this function too, but all the rules defined in the previous sections make it pretty clear that we can't write a `reverse_counting_sorter` since such a sorter wouldn't satisfy the `std::is_sorted` guarantee that every sorter should satisfy. Note however that, after having reverse-sorted a collection of integers, the following assertion should hold:

```cpp
assert( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );
```

We would like our `counting_sorter` to reverse-sort a collection when given `std::greater<>` as a comparison function, but there is no way it can handle arbitrary comparison functions. Well... let's just make it handle specific comparison functions then:

```cpp
struct counting_sorter_impl
{
    template<typename ForwardIterator>
    auto operator()(ForwardIterator first, ForwardIterator last) const
        -> void
    {
        counting_sort(first, last);
    }

    template<typename ForwardIterator>
    auto operator()(ForwardIterator first, ForwardIterator last,
                    std::greater<>) const
        -> void
    {
        reverse_counting_sort(first, last);
    }

    // Sorter traits
    using iterator_category = std::random_access_iterator_tag;
    using is_always_stable = std::false_type;
};
```

With such an implementation, this sorter satisfies the *comparison sorter* concept when given an instance of `std::greater<>` without breaking any of the rules defined in the previous sections. Now it may seem a bit unfair for `std::less<>`... but actually [`sorter_facade`](https://github.com/Morwenn/cpp-sort/wiki/Sorter-facade) automagically generates several `operator()` overloads taking `std::less<>` when the provided *sorter implementation* doesn't handle it natively. Note that even though this section is about non-comparison sorters, the same applies to non-projection sorters (you could provide a specific overload for [`std::negate<>`](http://en.cppreference.com/w/cpp/utility/functional/negate_void) for a descending sort too), and `sorter_facade` would provide equivalent `operator()` overloads taking [`utility::identity`](https://github.com/Morwenn/cpp-sort/wiki/Miscellaneous-utilities#miscellaneous-function-objects) for *sorter implementations* that cannot handle it natively).

The most beautiful thing in my opinion is that no new rule is needed to support that model. All the rules previously defined guarantee that these specific overloads using standard function object as tags work. The only advice I can give is to try to use the most standard function objects as tags, or at least the ones that are the most likely to be used for the specific task. Since **cpp-sort** is heavily based on modern C++ features, it is designed to only work with the `void` specializations of the standard function objects from `<functional>`.

## Type-specific sorters

Now is time to remember that the counting sort doesn't handle comparisons because it's a *type-specific sorter*: it is only designed to handle integers, so giving it a collection of any other type will eventually end up not compiling. As of now, this information is implicit and does not appear anywhere in the interface. We could use a static assertion to make it clear that using anything else than an integer is not allowed, but using SFINAE to trigger a soft error instead is actually more interesting:

```cpp
struct counting_sorter_impl
{
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

    template<typename ForwardIterator>
    auto operator()(ForwardIterator first, ForwardIterator last,
                    std::greater<>) const
        -> std::enable_if_t<
            std::is_integral<
                typename std::iterator_traits<ForwardIterator>::value_type
            >::value
        >
    {
        reverse_counting_sort(first, last);
    }

    // Sorter traits
    using iterator_category = std::random_access_iterator_tag;
    using is_always_stable = std::false_type;
};
```

Using such a soft error mechanism instead of a hard one will make it possible to use this sorter together with [`hybrid_adapter`](https://github.com/Morwenn/cpp-sort/wiki/Sorter-adapters#hybrid_adapter) (as long as the sorter provides a valid `iterator_category`) so that it can fall back to another sorter when a collection not handled by `counting_sorter` is given to the resulting sorter:

```cpp
// This sorter will use couting_sorter if a random-access collection of
// integers is given to it with no comparison function, with std::less<>,
// or with std::greater<>, and will fall back to merge_sorter in every
// other case
using generic_sorter = cppsort::hybrid_adapter<
    counting_sorter,
    cppsort::merge_sorter
>;
```

Note that the aggregate above plays well: `counting_sorter` sorter will be called if `generic_sorter` is given a collection of integers and either `std::less<>`, `std::greater<>` or no comparison function at all. `cppsort::merge_sorter` will only be called if `counting_sorter` really has no means to sort the collection.

While type-specific sorters are, by their very nature, unable to generically handle comparison functions, it might be possible for some of them to handle projections. A simple `counting_sorter` can't handle them because it "discards" the original information and recreates integer values laters, but other sorters such as [`spread_sorter`](https://github.com/Morwenn/cpp-sort/wiki/Sorters#spread_sorter) keep the original items around. Typically, a projection-enhanced type-specific sorter will be able to handle collections of any type provided the projection function projects the items to a type originally handled by the sorter (*e.g.* `spread_sorter` handles any type projected to an integer, a floating point number or an `std::string`).

**Rule 5.1:** a *type-specific sorter*'s `operator()` shall use SFINAE instead of hard errors to avoid being considered during overload resolution when the value type of the collection to sort is not compatible.

## Stability

A sorting algorithm is said to be [stable](https://en.wikipedia.org/wiki/Sorting_algorithm#Stability) if it preserves the relative order of equivalent elements. **cpp-sort** documents the stability of every sorter by giving them an `is_always_stable` type aliasing a boolean specialization of `std::integer_constant`. This information should be accessed via [`sorter_traits`](https://github.com/Morwenn/cpp-sort/wiki/Sorter-traits#sorter_traits) or via the more specific [`is_always_stable`](https://github.com/Morwenn/cpp-sort/wiki/Sorter-traits#is_always_stable) type alias. The stability of a sorter is always either [`std::true_type`](http://en.cppreference.com/w/cpp/types/integral_constant) or `std::false_type`.

```cpp
using stability = cppsort::is_always_stable<cppsort::tim_sorter>;
```

The library contains a *sorter adapter* named [`stable_adapter`](https://github.com/Morwenn/cpp-sort/wiki/Sorter-adapters#stable_adapter) that can be used to obtain a stable sorter, no matter which sorter it is given. If the *adapted sorter* `Sorter` is guaranteed to always be stable (if it defines `is_always_stable` as `std::true_type`), then `stable_sorter<Sorter>` will use it directly, otherwise it will use `make_stable<Sorter>`, where `make_stable` is a sorter adapter that uses the starting position of the elements in the collection to sort to make the *adapted sorter* stable. This mechanism only works if the *adapted sorter* is able to handle *proxy iterators*.

Users are allowed to explicitly specialize `stable_adapter` to provide a stable sorter related to the original sorter. For example, if we have a `stable_selection_sorter` wrapping a stable selection sort algorithm, we can specialize `selection_sorter` as follows:

```cpp
namespace cppsort
{
    template<>
    struct stable_adapter<selection_sorter>:
        stable_selection_sorter
    {};
}
```

The library also contains another stability-related type trait, [`is_stable`](https://github.com/Morwenn/cpp-sort/wiki/Sorter-traits#is_stable), which tells whether a *sorter* is stable when called with a specific set of parameters. It doesn't make a difference for *sorters* alone, but become incredibly useful to predict whether a call to an *adapted sorter* is stable or not. This trait is specialized for the library's *sorter adapters* and used by [`stable_adapter`]. When not specialized, it uses a *sorter*'s `is_always_stable` property as the stability of every call.

**Rule 6.1:** to document the *stability* of a *sorter*, it shall be given an `is_always_stable` type aliasing either `std::true_type` or `std::false_type`. If the sorter can't be altered, `sorter_traits` shall be specialized instead.

**Rule 6.2:** the *stability* of a *sorter* shall always be accessed via the library's `sorter_traits` or the corresponding `is_always_stable` trait.

**Rule 6.3:** a *sorter*'s `is_always_stable` shall alias `std::true_type` if and only if every algorithm provided by the sorter is guaranteed to always be stable.

**Rule 6.4:** if the stability of a sorter depends on the it is called with, `is_sorter` should be specialized to provide more fine-grained stability information.

**Rule 6.5:** users are allowed to specialize the class template `stable_adapter` for any *sorter* in order to provide a stable version of the *adapted sorter*. *Note: `stable_adapter` should only be specialized when it makes sense, but there is no sane way to enforce this rule, so the decision to specialize it or not is left to the end user*.

**Rule 6.6:** any specialization of `stable_adapter` shall alias `is_always_stable` to `std::true_type`.

**Rule 6.7:** the interface of any specialization of `stable_adapter` shall match that of the *adapted sorter*.

**Rule 6.8:** the class template `make_stable` shall not be specialized.

**Rule 6.9:** it only makes sense to provide an unstable *sorter* when it might offer performance benefits over a stable version of the same sorter.

## Stateful sorters

To date (version 1.5.0), every *sorter* provided by **cpp-sort** is a *stateless sorter*, which means that it is an empty type which does not carry any state between construction and subsequent invocations. However, the library is also designed to handle *stateful sorters*: those are sorters which might not be empty and which might carry a state that can change between invocations (even though the invocation itself isn't allowed to change the state as per Rule 1.4).

There are no specific rules to differentiate the way *stateful* and *stateless sorters* are written, but the distinction matters because specific components of the library will act differently when exposed to a *stateful* or *stateless* sorter:
* `sorter_facade` only provides conversions to different kinds of function pointers when the *sorter implementation* is empty (stateless) and default-constructible.
* `adapter_storage` is stateless if and only if the wrapped sorter is empty and default-constructible.
* Most of the library's *sorter adapters* produce a stateless *resulting sorter* if and only if the every *adapted sorter* is itself stateless.
* `small_array_adapter` only accepts *stateless sorters*.

## Buffered sorters

Some sorting algorithms can take advantage of buffers of any size to improve their performance, generally to perform merge operations. However, depending on many parameters not known to a sorter's implementer, the best method used to get a buffer of reasonable size might not always be the same. To avoid this problem (and to avoid the duplication of similar algorithms), **cpp-sort** provides some tools to specify the way a buffer is allocated. Let's take a simple example: [SqrtSort](https://github.com/Mrrl/SqrtSort) is apparently a [GrailSort](https://github.com/Mrrl/GrailSort) whose exchange buffer has a size roughly equal to the square root of the size of the collection to sort. While these algorithms are presented as separate algorithms, one can simply pass a *buffer provider* template parameter to a `grail_sorter` and use it to define define `sqrt_sorter`:

```cpp
using sqrt_sorter = grail_sorter<dynamic_buffer<sqrt>>;
```

In this example, `dynamic_buffer` is one of the [*buffer providers*](https://github.com/Morwenn/cpp-sort/wiki/Miscellaneous-utilities#buffer-providers) that come with the library. A *buffer provider* is a class that describes how the buffer should be allocated; it has a nested class template `buffer<T>` that actually contains the allocated memory. While `sqrt_sorter` is a regular sorter, `grail_sorter` is a *buffered sorter*.

The library's `dynamic_buffer` takes what is called a *size policy*: this is a class with an overloaded `operator()` taking an instance of `std::size_t` and returning an instance of a type convertible to `std::size_t`. The library provides [some function objects](https://github.com/Morwenn/cpp-sort/wiki/Miscellaneous-utilities#miscellaneous-function-objects) that can be used as size policies. It also provides  a `fixed_buffer` which takes an `std::size_t` template parameter corresponding to the number of elements to allocate on the stack for the buffer. For example, [`block_sorter`](https://github.com/Morwenn/cpp-sort/wiki/Sorters#block_sorter) uses a fixed-size buffer of 512 elements unless told otherwise:

```cpp
template<
    typename BufferProvider = utility::fixed_buffer<512>
>
struct block_sorter;
```

**Rule 7.1:** a *buffer provider* is a class that has a nested class template named `buffer`. The nested class template shall be explicitly constructible from an instance of `std::size_t` and shall at least implement the methods `begin`, `end`, `data` and `size`, with semantics similar to those of the random-access containers in the standard library.

**Rule 7.2:** a *buffered sorter* is a class template which takes a *buffer provider* as a template parameter.

**Rule 7.3:** a valid specialization of a *buffered sorter* is a *sorter* and shall therefore obey all the rules defined for sorters. It can also be a *comparison sorter* and/or a *projection sorter*, in which case it shall obey the specific rules for these kinds of sorters too.

**Rule 7.4:** a *buffered sorter* should work even if the buffer provider fails to allocate the buffer, even if it degrades the performance. It shouldn't rely on a specific size of buffer to perform the sort.

## Fixed-size sorters

Sorters are generally designed to sort collections of arbitrary size. However, **cpp-sort** also includes [*fixed-size sorters*](https://github.com/Morwenn/cpp-sort/wiki/Fixed-size-sorters), especially designed to sort a fixed number of values. Among other things, those algorithms include [sorting networks](https://en.wikipedia.org/wiki/Sorting_network) and sorting algorithms designed to perform a minimal number of move or comparison operations. They mainly exist to sort small fixed-size arrays, when the sorting operation appears in a critical section of the code and needs to be as fast as possible.

A *fixed-size sorter* is not a sorter *per se*: it's a class template taking an `std::size_t` template parameter corresponding to the number of values to sort. Every specialization of such a template is an actual sorter, which obeys all the rules defined in the previous sections as long as the collection to sort has the right size. Let's have a look at what a `low_projections_sorter` - a fixed-size sorter designed to perform a minimal number of projections - may look like:

```cpp
template<std::size_t N>
struct low_projections_sorter_impl
{
    static_assert(
        N && false,
        "low_projections_sorter has no specialization for this size of N"
    );
};

template<std::size_t N>
struct low_projections_sorter:
    cppsort::sorter_facade<::low_projections_sorter_impl<N>>
{};
```

As you can see, the main body is empty. A *fixed-size sorter* is not a single sorting algorithm (even though a generic algorithm can be used in the main body), but a collection of algorithms designed to sort a fixed number of values. Our `low_projections_sorter` would probably do nothing for the sizes 0 and 1 and perform a simple compare-and-exchange operation for the size 2:

```cpp
template<>
struct low_projections_sorter_impl<0u>
{
    template<
        typename RandomAccessIterator,
        typename Compare = std::less<>,
        typename Projection = utility::identity
    >
    auto operator()(RandomAccessIterator, RandomAccessIterator,
                    Compare={}, Projection={}) const
        -> void
    {}
};

// Same for low_projections_sorter_impl<1u>

template<>
struct low_projections_sorter_impl<2u>
{
    template<
        typename RandomAccessIterator,
        typename Compare = std::less<>,
        typename Projection = utility::identity,
        typename = std::enable_if_t<cppsort::is_projection_iterator<
            Projection, RandomAccessIterator, Compare
        >>
    >
    auto operator()(RandomAccessIterator first, RandomAccessIterator,
                    Compare compare={}, Projection projection={}) const
        -> void
    {
        auto&& comp = cppsort::utility::as_function(compare);
        auto&& proj = cppsort::utility::as_function(projection);
        if (comp(proj(first[0u]), proj(first[1u]))) {
            using std::iter_swap;
            iter_swap(first, first + 1u);
        }
    }
};
```

We won't show other specializations here because it is rather tedious and takes some place in the tutorial, but the idea is clear: any valid specialization can be a full-fledged sorter when used properly and can also be both a *comparison sorter* and a *projection sorter* if needed. It might be interesting to know which specializations of a fixed-size sorter can be used; in order to provide this information and some more, one has to specialize the trait class template [`fixed_sorter_traits`](https://github.com/Morwenn/cpp-sort/wiki/Sorter-traits#fixed_sorter_traits):

```cpp
namespace cppsort
{
    template<std::size_t N>
    struct fixed_sorter_traits<low_projections_sorter>
    {
        // Usable with arrays of size 0 to 5
        using domain = std::make_index_sequence<6>;

        // Every specialization works with random-access iterators
        using iterator_category = std::random_access_iterator_tag;

        // Every specialization is stable
        using is_always_stable = std::true_type;
    };
}
```

If `domain` does not exist in the `fixed_sorter_traits` specialization, it means that every specialization of the fixed-size sorter is a valid sorter.

Using the specializations of a fixed-size sorter by hand is not the sweetest thing either; that is why the library provides the fixed-size sorter adapter [`small_array_adapter`](https://github.com/Morwenn/cpp-sort/wiki/Sorter-adapters#small_array_adapter) which takes a whole *fixed-size sorter* and almost transforms it into a sorter (the resulting class doesn't handle pairs of iterators). The resulting object, when given an instance of [`std::array`](http://en.cppreference.com/w/cpp/container/array) or a fixed-size C array, will sort it in-place with the specialization corresponding to the size of the passed array. To transform that into a full sorter able to handle anything, one needs to aggregate it with another sorter into a `hybrid_adapter`:

```cpp
using sorter = cppsort::hybrid_adapter<
    cppsort::small_array_adapter<
        low_projections_sorter,
        std::make_index_sequence<5>
    >,
    cppsort::verge_sorter
>;
```

In the example above, the resulting sorter will use our `low_projections_sorter` when given an `std::array` or a fixed-size C array of size less than 5, and fall back to [`verge_sorter`](https://github.com/Morwenn/cpp-sort/wiki/Sorters#verge_sorter) when given any other collection or a pair of iterators.

**Rule 8.1:** a *fixed-size sorter* is allowed but not required to work with fixed arrays of any size.

**Rule 8.2:** any valid specialization of a *fixed-size sorter* for a given size is a *sorter*, and shall therefore obey all the rules defined for sorters provided the collection to sort has the correct size. It can also be a *comparison sorter* and/or a *projection sorter*, in which case it shall obey the specific rules for these kinds of sorters too.

**Rule 8.3:** a *fixed-size sorter* shall specialize the class `cppsort::fixed_sorter_traits` if it needs to provide information about its domain, its iterator category or its stability. See the exact meaning of these types and how to define them in the dedicated part of the documentation.