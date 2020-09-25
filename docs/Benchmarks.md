*Note: this page is hardly ever updated and the graphs might not reflect the most recent algorithms or optimizations. It can be used as a quick guide but if you really need a fast algorithm for a specific use case, you better run your own benchmarks.*

*Last major update: 1.8.0 release.*

Benchmarking is hard and I might not be doing it right. Moreover, benchmarking sorting algorithms highlights that the time needed to sort a collection of elements depends on several things: the type to sort, the size of the collection, the cost of comparing two values, the cost of moving an element, the patterns formed by the distribution of the values in the collection to sort, the type of the collection itself, etc. The aim of this page is to help you choose a sorting algorithm depending on your needs. You can find two main kinds of benchmarks: the ones that compare algorithms against shuffled collections of different sizes, and the ones that compare algorithms against different data patterns for a given collection size.

It is worth noting that most benchmarks on this page use collections of `double`: the idea is to sort collections of a simple enough type without getting numbers skewed by the impressive amount of optimizations that compilers are able to perform for integer types. While not perfect, `double` (without NaNs or infinities) falls into the "cheap enough to compare, cheap enough to move" category that most of the benchmarks here target.

All of the graphs on this page have been generated with slightly modified versions of the scripts found in the project's benchmarks folder. There are just too many things to check; if you ever want a specific benchmark, don't hesitate to ask for it.

*The benchmarks were run on Windows 10 with 64-bit MinGW-w64 g++10.1, with the flags -O3 -march=native -std=c++2a.*

# Random-access iterables

Most sorting algorithms are designed to work with random-access iterators, so this section is deemed to be bigger than the other ones. Note that many of the algorithms work better with contiguous iterators; the results for `std::vector` and `std::deque` are probably different.

## Unstable sorts

Unstable sorts are the most common sorting algorithms, and unstable sorts on random-access iterators are generally the fastest comparison sorts. If you don't know what algorithm you know, it's probably one of these ones.

![Benchmark speed of unstable sorts with increasing size for std::vector<double>](https://i.imgur.com/6Jfj768.png)
![Benchmark speed of unstable sorts with increasing size for std::deque<double>](https://i.imgur.com/HUiK2jf.png)

The plots above show a few general tendencies:
* `selection_sort` is O(n²) and doesn't scale.
* The three heap sorts are consistently the slowest.
* `ska_sort` is almost always the fastest.

The quicksort derivatives and the hybrid radix sorts are generally the fastest of the lot, yet `drop_merge_sort` seems to offer interesting speedups for `std::deque` despite not being designed to be the fastest on truly shuffled data. Part of the explanation is that it uses `pdq_sort` in a buffer underneath, which might be faster for `std::deque` than truly sorting in-place.

![Benchmark unstable sorts over different patterns for std::vector<double>](https://i.imgur.com/LL7iCQd.png)
![Benchmark unstable sorts over different patterns for std::deque<double>](https://i.imgur.com/4rkTNeq.png)

A few random takeways:
* All the algorithms are more or less adaptive, not always for the same patterns.
* `ska_sort` beats all the other algorithms on average, and almost systematically beats the other hybrid radix sort, `spread_sort`.
* `quick_merge_sort` is slow, but it wasn't specifically designed for random-access iterators.
* libstdc++ `std::sort` doesn't adapt well to some patterns meant to defeat median-of-3 quicksort.
* `verge_sort` adapts well to most patterns (but it was kind of designed to beat this specific benchmark).
* `pdq_sort` is the best unstable sorting algorithm there which doesn't allocate any heap memory.
* `drop_merge_sort` and `split_sort` were not designed to beat those patterns yet they are surprisingly good on average.

## Stable sorts

Pretty much all stable sorts in the library are different flavours of merge sort with sligthly different properties. Most of them allocate additional merge memory, and a good number of those also have a fallback algorithm that makes them run in O(n log²n) instead of O(n log n) when no extra heap memory is available.

![Benchmark speed of stable sorts with increasing size for std::vector<double>](https://i.imgur.com/vRW1zcs.png)
![Benchmark speed of stable sorts with increasing size for std::deque<double>](https://i.imgur.com/CQePcBh.png)

`insertion_sort` being O(n²) it's not surprising that it doesn't perform well in such a benchmark. All the other sorting algorithms display roughly equivalent and rather tight curves.

![Benchmark stable sorts over different patterns for std::vector<double>](https://i.imgur.com/bRQ5cu5.png)
![Benchmark stable sorts over different patterns for std::deque<double>](https://i.imgur.com/fHIZB5L.png)

These plots highlight a few important things:
* `spin_sort` consistently beats pretty much anything else.
* `block_sort` and `grail_sort` in this benchmark run with O(1) extra memory, which makes them decent stable sorting algorithms for their category.
* Interestingly `stable_adapter(pdq_sort)` is among the best algorithms, despite the overhead of making `pdq_sort` artificially stable.

## Heap sorts

I decided to include a dedicated category for heap sorts, because I find this class of algorithms interesting. Frankly, it's difficult to beat a plain `heap_sort` in this category, despite it being otherwise noticeably slower than quicksort derivatives or hybrid radix sorts. Heap sorts are not used a lot on their own, but they are often used to ensure the O(n log n) complexity of introsort-like algorithms, so having good heap sorts is generally desirable.

![Benchmark heap sorts over different patterns for std::vector<double>](https://i.imgur.com/1rY17x4.png)
![Benchmark heap sorts over different patterns for std::deque<double>](https://i.imgur.com/gWe7j6m.png)

The analysis is pretty simple here:
* `heap_sort` is generally the best solution, but it is less adaptive than the other sorts.
* `poplar_sort` is slower for `std::vector` than for `std::deque`, which makes me suspect a codegen issue somewhere.
* As a result `smooth_sort` and `poplar_sort` beat each other depending on the type of the collection to sort.

# Bidirectional iterables

Sorting algorithms that handle non-random-access iterators are often second class citizens, but **cpp-sort** still provides a few ones. The most interesting part is that we can see how generic sorting algorithms perform compared to algorithms such as [`std::list::sort`](http://en.cppreference.com/w/cpp/container/list/sort) which are aware of the data structure they are sorting.

![Benchmark speed of sorts with increasing size for std::list<double>](https://i.imgur.com/Z2BDhpz.png)

For elements as small as `double`, there are two clear winners here: `drop_merge_sort` and `out_of_place_adapter(pdq_sort)`. Both have in common the fact that they move a part of the collection (or the whole collection) to a contiguous memory buffer and sort it there using `pdq_sort`. The only difference is that `drop_merge_sort` does that "accidentally" while `out_of_place_adapter` was specifically introduced to sort into a contiguous memory buffer and move back for speed.

![Benchmark sorts over different patterns for std::list<double>](https://i.imgur.com/6EftqN7.png)

`out_of_place_adapter(pdq_sort)` was not included in this benchmark, because it adapts to patterns the same way `pdq_sort` does. Comments can be added for these results:
* `std::list::sort` would require elements more expensive to move for node relinking to be faster than move-based algorithms.
* `drop_merge_sort` adapts well to every pattern and shows that out-of-place sorting really is the best thing here.
* `quick_sort` doesn't scale well with patterns.
* `quick_merge_sort` does an honorable job for an algorithm that doesn't allocate any extra heap memory.

# Forward iterables

Even fewer sorters can handle forward iterators. `out_of_place_adapter(pdq_sort)` was not included in the patterns benchmark, because it adapts to patterns the same way `pdq_sort` does.

![Benchmark speed of sorts with increasing size for std::forward_list<double>](https://i.imgur.com/Ly4kbaN.png)
![Benchmark sorts over different patterns for std::forward_list<double>](https://i.imgur.com/bWZRega.png)

The results are roughly the same than with bidirectional iterables:
* `std::forward_list::sort` doesn't scale well unless moves are expensive.
* Sorting out-of-place is faster than anything else.
* If no extra heap memory is available, `quick_merge_sort` is the only O(n log n) algorithm that can be used, and does a fine job despite never being excellent.

# Sorting under specific constraints

This category will highlight the advantages of some sorters in sorting scenarios with specific constraints, and how they can beat otherwise faster general-purpose algorithms once the rules of the sorting game change. A typical example is how `merge_insertion_sort` is among your best friends when you need is to perform the smallest possible number of comparisons to sort a collection - that said that specific sorter won't be part of these benchmarks, because it is abysmally slow for any conceivable real world scenario.

## Integer sorting

Integer sorting is a rather specific scenario for which many solutions exist: counting sorts, radix sorts, algorithms optimized to take advantage of branchless comparisons, etc.

![Benchmark speed of integer sorts with increasing size for std::vector<int>](https://i.imgur.com/zuCAkIf.png)
![Benchmark integer sorts over different patterns for std::vector<int>](https://i.imgur.com/20uDwTM.png)

`counting_sort` appears as a clear winner here but with a catch: its speed depends on the difference between the smaller and the greater integers in the collection to sort. In the benchmarks above the integer values scale with the size of the collection, but if a collection contains just a few elements with a big difference of the minimum and maximum values, `counting_sort` won't be a good solution.

`spread_sort` and `ska_sort` are two hybrid radix sorts, which are less impacted by the values than `counting_sort`. Of those two `ska_sort` is the clear winner. `pdq_sort` is interesting because it performs almost as well as hybrix radix sorts for integers despite being a comparison sort, which makes it an extremely versatile general-purpose algorithm - it is still more affected than the other algorithms by the data patterns, but as we can see all of the algorithms above are affected by patterns.

## Small array sorters

Some sorting algorithms are particularly suited to sort very small collections: the ones provided by `<cpp-sort/fixed_sorters.h>`, but also the very simple ones such as `insertion_sort` or `selection_sort`. Most other sorting algorithms fallback to one of these when sorting a small collection.

![Benchmark speed of small sorts with increasing size for std::array<int>](https://i.imgur.com/dOa3vyl.png)
![Benchmark speed of small sorts with increasing size for std::array<long double>](https://i.imgur.com/4WRtPYP.png)

As far as only speed matters, sorting networks tend to win in these artificial benchmarks, but in a real world scenario the cost of loading the network code for a specific size again and again tends to make them slower. A sorting network can be fast when it is used over and over again.

The spikes in the otherwise smooth sorting networks curve when sorting arrays of integers are weird: they don't exist for the `long double` benchmark but are consistent across runs for the `int` scenario. Interestingly enough those spikes seem to follow the `insertion_sort` curve.

`low_moves_sorter` uses a modified selection sort above a small threshold, which might explain why the artefacts in the two curves have similar shapes.

# Measures of presortedness

This benchmark for [measures of presortedness](https://github.com/Morwenn/cpp-sort/wiki/Measures-of-presortedness) is small and only intends to show the cost that these tools might incur. It is not meant to be exhaustive in any way.

![Benchmark speed of measures of presortedness for increasing size for std::vector<int>](https://i.imgur.com/5XniqE1.png)

While the graph reasonably shows the relative cost of the different measures of presortedness, there are a few hidden traps:
* *Par(X)* seems to beat every other measure, but it is a highly adaptative O(n² log n) algorithm, whose theoretical worst case might be the worst of all measures of presortedness.
* *Dis(X)* looks like a O(n) algorithm in this graph, but it is actually a O(n²) algorithm with extremely efficient short-circuits in most cases. Its worst case would put it closer from *Osc(X)*.
