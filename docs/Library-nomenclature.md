**cpp-sort** deals with many concepts related to sorting and algorithms in general. This section tries to briefly explain the many things that you may encounter while using it, in alphabetical order:

* *Buffered sorter*: some sorting algorithms optionally use a buffer where they store elements to improve the performance of the sort. Some of them, such as block sort, will manage to sort the collection regardless of the actual size of the buffer, which will only have on influence on the performance of the sort. A buffered sorter is a sorter that takes a *buffer provider* template parameter that tells how the temporary buffer should be allocated, and uses this provider to create the buffer. A *buffer provider* is a class that has a nested `buffer` class which implements a set of basic operations (construction with a size, `begin`, `end` and `size`). Implementing a buffer provider is a bit tricky, but using them should be easy enough:

        using sorter = cppsort::block_sorter<
            cppsort::utility::fixed_buffer<512>
        >;

* *Comparison function*: most of the sorting algorithms in the library are comparison sorts. It means that the algorithm uses a comparison function to know the order of the elements and sort them accordingly; such a comparison function shall take two values and have a return type convertible to `bool`. The available sorting algorithms transform comparison functions on the fly so that some pointers to member functions can also be used as comparison functions, as if called with [`std::invoke`](http://en.cppreference.com/w/cpp/utility/functional/invoke). The default comparison function used by the sorting algorithms is [`std::less<>`](http://en.cppreference.com/w/cpp/utility/functional/less_void). Many sorters can take a comparison function as an additional parameter. For example, using `std::greater<>` instead of the default comparison function would sort a collection in descending order.

        cppsort::heap_sort(collection, std::greater<>{});

    Some algorithms don't accept such an additional parameter. It may be because they implement a non-comparison sort instead, a sorting algorithm that uses other properties of the elements to perform the sort rather than a comparison function (for example a [radix sort](https://en.wikipedia.org/wiki/Radix_sort)).

    The library provides a set of additional [comparators](https://github.com/Morwenn/cpp-sort/wiki/Comparators) generally corresponding to common ways to compare common types.

* *Fixed-size sorter*:  [[fixed-size sorters]] are a special breed of sorters designed to sort a fixed number of values. While they try their best to be full-fledge sorters, they are definitely not full-fledge sorters and probably don't blend as well as one would like into the library. Their main advantage is that they can be more performant than regular sorters in some specific scenarios.

* *Iterator category*: the C++ standard defines [several categories of iterators](http://en.cppreference.com/w/cpp/iterator) such as forward iterators, bidirectional iterators or random-access iterators. The standard library uses [iterator tags](http://en.cppreference.com/w/cpp/iterator/iterator_tags) to document the category of an iterator. These categories are important since algorithms are designed to work with some categories of iterators and not with other categories, and those in this library are not different: in-place sorting needs at least forward iterators. You can use the [`iterator_category`](https://github.com/Morwenn/cpp-sort/wiki/Sorter-traits#iterator_category) sorter trait to get the least constrained iterator category associated with a sorter.

        using category = cppsort::iterator_category<cppsort::merge_sorter>;

    Note that the *sorters* (and virtually bery algorithm) in **cpp-sort** accept iterators that do not implement post-increment and post-decrement operations. The iterator categories accepted by the library are thus less restrictive than the ones mandated for the standard library.

* *Measure of presortedness*: also known as a *measure of disorder*, it corresponds to an algorithm telling how much a collection is already sorted. There isn't a single way to tell how much a collection is already sorted, one can for example count the number of inversions or the number of elements to remove to get a sorted subsequence. The main advantage of measures of presortedness are that some algorithms, known as *adaptative algorithms*, are known to be optimal for some of these measures, which means that they can advantage of the order that already exists in the collection in some way. **cpp-sort** provides a number of [[measures of presortedness]] in the namespace `cppsort::probe`:

        auto max_inversion = cppsort::probe::dis(collection);

* *Projection*: some sorters accept a projection as an additional parameter. A projection is a unary function that allows to "view" the values of a collection differently. For example it may allow to sort a collection of values on a specific field. The available sorting algorithms transform projections on the fly so that pointers to member data can also be used as projections. Projections were pioneered by the [Adobe Source Libraries](http://stlab.adobe.com/) and appear in the C++20 [Constrained algorithms](https://en.cppreference.com/w/cpp/algorithm/ranges).

        struct wrapper { int value; };
        std::vector<wrapper> collection = { /* ... */ };
        cppsort::heap_sort(collection, &wrapper::value);

    Every *comparison sorter* is also a *projection sorter*, but there are also projection-only sorters, such as  [`spread_sorter`](https://github.com/Morwenn/cpp-sort/wiki/Sorters).

* *Proxy iterator*: sometimes `std::move` and `std::swap` are not enough to correctly move values around, and we need to know more about the iterators in order to perform the appropriate operation. It's typically the case with proxy iterators: iterators whose `reference` type is not actually a reference type (*e.g.* `std::vector<bool>::reference`). Traditional algorithms don't play well with these types, however there are [standard proposals](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0022r1.html) to solve the problem by introducing a function named `iter_move` and making it as well as `iter_swap` customization points. No proposal has been accepted yet, so standard libraries don't handle proxy iterators; however every sorter in **cpp-sort** can actually handle such iterators (except `std_sorter` and `std_stable_sorter`). The library exposes the functions [`utility::iter_move` and `utility::iter_swap`](https://github.com/Morwenn/cpp-sort/wiki/Miscellaneous-utilities#iter_move-and-iter_swap) in case you also need to make your own algorithms handle proxy iterators.

* *Sorter*: [[sorters]] are the protagonists in this library. They are function objects implementing specific sorting algorithms. Their `operator()` is overloaded so that it can handle iterables or pairs of iterators, and conditionally overloaded so that it can handle user-provided comparison and/or projection functions.

        cppsort::pdq_sorter{}(std::begin(collection), std::end(collection),
                              std::greater<>{}, &wrapper::value);

* *Sorter adapter*: [[sorter adapters]] are class templates that take one or several sorters and produce a new sorter from the parameters. What a sorter adapter can do is not constrained, but they are generally expected to behave like sorters themselves. For example, **cpp-sort** contains adapters to count the number of comparisons performed by a sorting algorithms or to aggregate several sorters together. The best way to learn more about them is still to read the dedicated section in the documentation. 

* *Stability*: a sorting algorithm is *stable* if it preserves the relative order of equivalent elements. While it does not matter when the equivalence relationship is also an equality relationship, it may have its importance in other situations. It is possible to query whether a sorter is guaranteed to always use a stable sorting algorithm with the [`is_always_stable`](https://github.com/Morwenn/cpp-sort/wiki/Sorter-traits#is_always_stable) sorter trait.

        using stability = cppsort::is_stable<cppsort::tim_sorter>;

    It is possible have have more fine-grained information about the stability of a sorter with the [`is_stable`](https://github.com/Morwenn/cpp-sort/wiki/Sorter-traits#is_stable) type trait, which tells whether a sorter is stable when called with a specific set of parameters.

        using sorter = cppsort::self_sort_adapter<cppsort::verge_sorter>;
        using stability1 = cppsort::is_stable<sorter(std::list<int>&)>; // stable
        using stability2 = cppsort::is_stable<sorter(std::vector<int>&)>; // unstable

    The library also provides the adapter [`stable_adapter`](https://github.com/Morwenn/cpp-sort/wiki/sorting-function) to obtain a stable sorter corresponding to the passed sorter. When calling the adapted sorter yields an unstable sorting algorithm, the utility adapter `make_stable` is used to transform it into a stable sorting algorithm, providing the underlying algorithm handles proxy iterators.

* *Stateful/Stateless sorter*: a sorter either carries a state or not; when it does so it is called a *stateful* sorter, otherwise it is called a *stateless* sorter. Most of the *sorters* in the library are stateless sorters.

    Stateless sorters are generally empty default-constructible types. If they indeed satisfy these guarantees, then several components of the library will provide space optimizations, and some wrapping components will also provide overloaded operators to be turned into several kinds of function pointers. Therefore, authors of *stateless sorters* are encouraged to also make them empty and default-constructible to benefit from the full powers of the library.

* *Type-specific sorter*: some non-comparison sorters such as the [`spread_sorter`](https://github.com/Morwenn/cpp-sort/wiki/Sorters#spread_sorter) implement specific sorting algorithms which only work with some specific types (for example integers or strings).