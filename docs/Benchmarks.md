*Note: this page is hardly ever updated and the graphs might not reflect the most recent algorithms or optimizations. It can be used as a quick guide but if you really need a fast algorithm for a specific use case, you better run your own benchmarks.*

Benchmarking is hard and I might not be doing it right. Moreover, benchmarking sorting algorithms highlights that the time needed to sort a collection of elements depends on several things: the type to sort, the size of collection, the cost of comparing two values, the cost of moving two values, the distribution of the values in the collection to sort, the collection itself, etc... The aim of this page is to help you choose a sorting algorithm depending on your needs. You can find two main kinds of benchmarks: the ones that compare algorithms against shuffled collections of different sizes, and the ones that compare algorithms against different data patterns for a given collection size.

All of the graphs on this page have been generated with slightly modified versions of the scripts found in the project's benchmarks folder. There are just too many things to check; if you ever want a specific benchmark, don't hesitate to ask for it.

# Random-access iterables

Most sorting algorithms are designed to work with random-access iterators, so this section is deemed to be bigger than the other ones. Note that many of the algorithms work better with contiguous iterators; the resuts for `std::vector` and `std::deque` are probably different.

## Unstable sort for `std::vector` with 10⁶ `int`

![](http://i.imgur.com/RcREbKQ.png)

As we can see, even though its algorithmic complexity is O(n log n), `heap_sorter` is simply too slow on average, no matter the pattern. `std_sorter` actually performs worse than expected (the libstdc++ one), especially for some patterns. The best sorting algorithms for this configuration are `pdq_sorter`, `quick_sorter`, `spread_sorter` and `verge_sorter`.

`spread_sorter` is by far the best sorter when it comes to sorting truly random collections and is excellent when the number of values in the collection is low (16 values in the examples). When there are pipe-organ patterns, `verge_sorter` seems to be the best match since it takes advantage of big subsequences sorted in ascending or descending order. That said, both of these sorters use additional memory; `pqd_sorter` is the best sorter if a small memory footprint is needed.

## Unstable sort for `std::vector` with 10⁷ `int`

![](http://i.imgur.com/HNMO7zb.png)

Compared to the previous graph, a few things have changed: while it's still better than anything else for random distributions, `spread_sorter` has bad performance for the *Alternating* distribution. My guess is that it has to do with the fact that it's a radix-sort kind of algorithm and the fact that the *Alternating* case has both negative values and the widest range of values among the distributions doesn't help. `heap_sorter` is still the slowest thing ever.

`std_sorter` seems to reach some kind of worst case behaviour for some distributions, making it less usable than the other unstable sorters in this test case. Actually `quick_sorter` should also have some quadratic worst cases, but the median-of-9 pivot selection makes it harder to find such worst cases.

## Unstable sort for `std::deque` with 10⁶ `int`

![](http://i.imgur.com/0qU9cH7.png)

The results for `std::deque` are close to those for `std::vector`. The most notable difference is that `heap_sorter` is even slower than for `std::vector` compared to the other sorters; apparently, random-access has a noticeable cost when the iterators are not contiguous. Otherwise, the conclusions are pretty much the same than with `std::vector`: when the distribution is truly random, `spread_sorter` is still the obvious winner, while `verge_sorter` beats distributions where big chunks of data are already sorted. `pdq_sorter` is still the best match when a small memory footprint is required. `std_sorter` still has some kind of worst case behaviour for the *Pipe organ*, *Push front* and *Push middle* patterns.

## Stable sort for `std::vector` with 10⁷ `long double`

![](https://i.imgur.com/iQNaaAR.png)

## Heap sorts for `std::vector` with 10⁶ `int`

![](https://i.imgur.com/JxKhs5c.png)

As we have seen in the previous section, `heap_sorter` tends to be really slow. Its main use is as a fallback for introsort-like sorters that perform log n quicksort steps before switching to a heapsort if the collection is not sorted (it ensures the O(n log n) complexity of introsort-like algorithms). **cpp-sort** actually provides several heap sorters with different properties: all of them build a heap then remove elements one by one to sort the collection, but the underlying heaps are different (implementation-defined heap for `heap_sorter` since it uses `std::make_heap`, but likely a regular maxheap, forest of Leonardo heaps for `smooth_sorter`, and forest of perfect balanced binary heaps for `poplar_sorter`).

The regular `heap_sorter` has by far the best worst case and average performance. `smooth_sorter` and `poplar_sorter` are adaptative as can be seen when the collection to sort is already almost sorted, but their average running time is just too slow to be usable. It is worth noting that `smooth_sorter` was consistently slower than anything else with `-O2` (the graph above has been generated with `-O3`).

# Bidirectional iterables

Sorting algorithms that handle non-random-access iterators are generally second class citizens, but **cpp-sort** still has a few ones. Obviously quadratic algorithms such as insertion sort or selection sort are ignored for these benchmarks. The most interesting part is that we can see how generic sorting algorithms perform compared to algorithms such as [`std::list::sort`](http://en.cppreference.com/w/cpp/container/list/sort) which are aware of the data structure they are sorting.

## Sort `std::list` with 10⁶ `int`

![](http://i.imgur.com/pNfEsC1.png)

First of all, note that `default_sorter` uses [`self_sort_adapter`](https://github.com/Morwenn/cpp-sort/wiki/Sorter-adapters#self_sort_adapter), which means that in this benchmark, the sorting algorithm used by `default_sorter` is actually `std::list::sort`. Apparently, `std::list::sort` is efficient for many patterns, but it doesn't handle random distributions that much and also has some problems with *Alternating (16 values)*. On the other hand, `quick_sorter` works really well when there are few values, but doesn't really handle patterns as well as the other algorithms. `verge_sorter` is rather good when there are not many runs in the collection to sort, and falls back on `quick_sorter` when the runs aren't big enough, which means that it's good for some patterns, but always slower than `quick_sorter` otherwise (it's still more than decent when there aren't many values). Finally, `merge_sorter` adapts rather well to many patterns, especially when the collection is already sorted to some degree in ascending order; it is by far the best algorithm to handle the *Shuffled* distribution.

## Sort `std::list` with 10⁶ `long double`

![](http://i.imgur.com/FSoNXH2.png)

The results are pretty much the same than with integers. The most important difference is that `std::list::sort` isn't much slower since it does not move/swap values but relinks nodes instead, so no matter the size of the types to sort, relinking always has the same cost. Therefore, it's a bit better than with integers compared to the other sorting algorithms; I expect it to be the better and better when the types to sort are more expensive to move around.

# Forward iterables

Even fewer sorters can handle forward iterators; just like with bidirectional iterators, we only care about the sorters that are not almost always quadratic, which means that we care about the same sorters than before, except `verge_sorter` since it sometimes needs to call `std::reverse` which requires bidirectional iterators.

## Sort `std::forward_list` with 10⁶ `int`

![](http://i.imgur.com/HsiE2e7.png)

As expected, `default_sorter` actually calls `std::forward_list::sort`, which is a bit better than with bidirectional iterators compared to the other sorting algorithms, probably because it has fewer nodes to relink. `quick_sorter` is still excellent when there are not many different values in the collection to sort but is less efficient than `std::forward_list::sort` for almost every specific pattern. `merge_sorter` is pretty good when the collection to sort is mostly ascending since it adapts to ascending runs; it has the best worst case among the tested algorithms.

## Sort `std::foward_list` with 10⁶ `long double`

![](http://i.imgur.com/CKg7Ip5.png)

There is almost no difference compared to the previous graph except the overall time. Just like with bidirectional iterators, I would expect `std::forward_list::sort` to perform gradually better when the objects to sort are more expensive to move around. The good performances of `merge_sorter` in all the benchmarks are due to the fact that it can allocate memory to perform the merges; it adapts to the memory it is given, but it degrades to a O(n log² n) algorithm when no additional memory is available.

# Measures of presortedness

![](https://i.imgur.com/pe6YvuH.png)

This graph shows the running time of the different [measures of presortedness](https://github.com/Morwenn/cpp-sort/wiki/Measures-of-presortedness) when run on sequences of random integers of various sizes. While *Par(X)* seems to beat every other measure, be careful: it is highly adaptative, but its complexity is O(n² log n), which means that it can be as slow as *Dis(X)* or *Osc(X)* for some inputs, if not even slower.



