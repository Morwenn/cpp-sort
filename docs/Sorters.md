**cpp-sort** uses function objects called *sorters* instead of regular function templates in order to implement sorting algorithms. The library provides two categories of sorters: comparison sorters, that sort a collection with using a weak order comparator, and type-specific sorters that use other strategies to sort collections, generally restricting their domain to specific types. Every comparison sorter as well as some type-specific sorters may also take an additional projection parameter, allowing the algorithm to "view" the data to sort through an on-the-fly transformation. (see *unified sorting interface*)

While these function objects offer little more than regular sorting functions by themselves, they can be used together with *[sorter adapters][sorter-adapters]* to craft more elaborate sorters. Every sorter is available in its own file. However, all the available sorters can be included at once with the following line:

```cpp
#include <cpp-sort/sorters.h>
```

For every `foobar_sorter` described in this page, there is a corresponding `foobar_sort` global instance that allows not to care about the sorter abstraction as long as it is not needed (the instances are usable as regular function templates). The only sorter without a corresponding global instance is [`default_sorter`][default-sorter] since it mainly exists as a fallback sorter for the functions [`cppsort::sort` and `cppsort::stable_sort`][sorting-functions] when they are called without an explicit sorter.

If you want to read more about sorters and/or write your own one, then you should have a look at [the dedicated page][writing-a-sorter] or at [a specific example][writing-a-bubble-sorter].

## Comparison sorters

The following sorters are available and should work with any type for which `std::less` works and should accept any weak order comparison function:

### `adaptive_shivers_sorter`

```cpp
#include <cpp-sort/sorters/adaptive_shivers_sorter.h>
```

Implements an [adative ShiversSort][adaptive-shivers-sort], a *k*-aware natural mergesort with a better worst case than [timsort][timsort] described by Vincent Jugé in *Adaptive Shivers Sort: An Alternative Sorting Algorithm*.

| Best        | Average     | Worst       | Memory      | Stable      | Iterators     |
| ----------- | ----------- | ----------- | ----------- | ----------- | ------------- |
| n           | n log n     | n log n     | n           | Yes         | Random-access |

While the sorting algorithm is stable and the complexity guarantees are good enough, this sorter is rather slow compared to the some other ones when the data distribution is random. That said, it would probably be a good choice when comparing data is expensive, but moving it is inexpensive (this is the use case for which it was designed).

*New in version 1.13.0*

### `block_sorter<>`

```cpp
#include <cpp-sort/sorters/block_sorter.h>
```

***WARNING:** `block_sorter` is deprecated in version 1.11.0 and removed in version 2.0.0, use [`wiki_sorter`][wiki-sorter] instead.*

### `cartesian_tree_sorter`

```cpp
#include <cpp-sort/sorters/cartesian_tree_sorter.h>
```

Implements a [Cartesian tree sort][cartesian-tree-sort], a rather slow but highly adaptive algorithm described by C. Levcopoulos and O. Petersson in *Heapsort - Adapted for Presorted Files*.

| Best        | Average     | Worst       | Memory      | Stable      | Iterators     |
| ----------- | ----------- | ----------- | ----------- | ----------- | ------------- |
| n           | n log n     | n log n     | n           | No          | Forward       |

*New in version 1.10.0*

*Changed in version 1.11.0:* `cartesian_tree_sorter` now works with forward iterators. It used to only work with random-access iterators.

### `d_ary_heap_sorter`

```cpp
#include <cpp-sort/sorters/d_ary_heap_sorter.h>
```

Implements a heapsort algorithm based on a [*d*-ary heap][d-ary-heap]: the value of *D* is an integer template parameter that can be passed to either `d_ary_heap_sort` or `d_ary_heap_sorter`. That value cannot be smaller than 2.

| Best        | Average     | Worst       | Memory      | Stable      | Iterators     |
| ----------- | ----------- | ----------- | ----------- | ----------- | ------------- |
| n log n     | n log n     | n log n     | 1           | No          | Random-access |

*d*-ary heapsorts are more complicated than their binary counterparts, but storing more elements in each node can improve locality of reference and improve the execution speed. This sorter, unlike [`heap_sorter`][heap-sorter], does not implement a [bottom-up node searching strategy][bottom-up-heapsort].

*New in version 1.14.0*

### `default_sorter`

```cpp
#include <cpp-sort/sorters/default_sorter.h>
```

***WARNING:** `default_sorter` is deprecated in version 1.8.0 and removed in version 2.0.0, see [issue #168][issue-168] for the rationale.*

Sorter striving to use a sorting algorithm as optimized as possible. It is the fallback sorter used by [`cppsort::sort`][cppsort-sort] when no sorter is given. The current implementation defines it as follows:

```cpp
struct default_sorter:
    self_sort_adapter<
        hybrid_adapter<
            small_array_adapter<
                low_comparisons_sorter,
                std::make_index_sequence<14u>
            >,
            quick_sorter,
            pdq_sorter
        >
    >
{};
```

The adapter [`stable_adapter`][stable-adapter] has an explicit specialization for `default_sorter` defined as follows:

```cpp
template<>
struct stable_adapter<default_sorter>:
    merge_sorter
{};
```

*Deprecated in version 1.8.0*

*Removed in version 2.0.0*

### `drop_merge_sorter`

```cpp
#include <cpp-sort/sorters/drop_merge_sorter.h>
```

Implements a [drop-merge sort][drop-merge-sort].

| Best        | Average     | Worst       | Memory      | Stable      | Iterators     |
| ----------- | ----------- | ----------- | ----------- | ----------- | ------------- |
| n           | n log n     | n log n     | n           | No          | Bidirectional |

Drop-merge sort is a [*Rem*-adaptive][probe-rem] sorting algorithm. While it is not as good as other sorting algorithms to sort shuffled data, it is excellent when more than 80% of the data is already ordered according to *Rem*.

### `grail_sorter<>`

```cpp
#include <cpp-sort/sorters/grail_sorter.h>
```

Implements a [Grail sort][grailsort].

| Best        | Average     | Worst       | Memory      | Stable      | Iterators     |
| ----------- | ----------- | ----------- | ----------- | ----------- | ------------- |
| ?           | n log n     | n log n     | 1           | Yes         | Random-access |

`grail_sorter` is a *buffered sorter* whose default specialization achieves O(1) auxiliary memory by not actualy allocating any additional memomry. The memory complexity may change depending of the *buffer provider* passed to it.

```cpp
template<
    typename BufferProvider = utility::fixed_buffer<0>
>
struct grail_sorter;
```

Whether this sorter works with types that are not default-constructible depends on the memory allocation strategy of the buffer provider. The default specialization works with such types.

*Changed in version 1.5.0:* `grail_sorter` now handles comparison and projection objects that aren't default-constructible.

### `heap_sorter`

```cpp
#include <cpp-sort/sorters/heap_sorter.h>
```

Implements a [bottom-up heapsort][bottom-up-heapsort].

| Best        | Average     | Worst       | Memory      | Stable      | Iterators     |
| ----------- | ----------- | ----------- | ----------- | ----------- | ------------- |
| n log n     | n log n     | n log n     | 1           | No          | Random-access |

### `insertion_sorter`

```cpp
#include <cpp-sort/sorters/insertion_sorter.h>
```

Implements an [insertion sort][insertion-sort].

| Best        | Average     | Worst       | Memory      | Stable      | Iterators     |
| ----------- | ----------- | ----------- | ----------- | ----------- | ------------- |
| n           | n²          | n²          | 1           | Yes         | Bidirectional |

This sorter also has the following dedicated algorithms when used together with [`container_aware_adapter`][container-aware-adapter]:

| Container           | Best        | Average     | Worst       | Memory      | Stable      |
| ------------------- | ----------- | ----------- | ----------- | ----------- | ----------- |
| `std::list`         | n           | n²          | n²          | 1           | Yes         |
| `std::forward_list` | n           | n²          | n²          | 1           | Yes         |

None of the container-aware algorithms invalidates iterators.

### `mel_sorter`

```cpp
#include <cpp-sort/sorters/mel_sorter.h>
```

Implements melsort, a rather slow but *Enc*-adaptive algorithm described by S. Skiena in *Encroaching lists as a measure of presortedness*.

*MEL* stands for *Merge Encroaching Lists*.

| Best        | Average     | Worst       | Memory      | Stable      | Iterators     |
| ----------- | ----------- | ----------- | ----------- | ----------- | ------------- |
| n           | n log n     | n log n     | n           | No          | Forward       |

This sorter also has the following dedicated algorithms when used together with [`container_aware_adapter`][container-aware-adapter]:

| Container           | Best        | Average     | Worst       | Memory      | Stable      |
| ------------------- | ----------- | ----------- | ----------- | ----------- | ----------- |
| `std::list`         | n           | n log n     | n log n     | n           | No          |
| `std::forward_list` | n           | n log n     | n log n     | n           | No          |

None of the container-aware algorithms invalidates iterators.

*New in version 1.10.0*

### `merge_insertion_sorter`

```cpp
#include <cpp-sort/sorters/merge_insertion_sorter.h>
```

Implements the Ford-Johnson merge-insertion sort. This algorithm isn't meant to actually be used and is mostly interesting from a computer science point of view: for really small collections, it has an optimal worst case for the number of comparisons performed. It has indeed been proved that for some sizes, no algorithm can perform fewer comparisons. That said, the algorithm has a rather big memory overhead and performs many move operations; it is really too slow for any real world use.

| Best        | Average     | Worst       | Memory      | Stable      | Iterators     |
| ----------- | ----------- | ----------- | ----------- | ----------- | ------------- |
| ?           | n log n     | n log n     | n           | No          | Random-access |

*Until version 1.7.0:* the algorithm used GNU's [`bitmap_allocator`][bitmap-allocator] with `std::list` when possible instead of the default allocator, leading to speed improvements up to 25%.

*Changed in version 1.7.0:* the `std::list` used by the algorithm has been replaced with a custom list implementation whose speed does not depend on the availability of some allocator, and which should be faster than the previous implementation in any case (still not anywhere near fast).

### `merge_sorter`

```cpp
#include <cpp-sort/sorters/merge_sorter.h>
```

Implements a [merge sort][merge-sort].

| Best        | Average     | Worst       | Memory      | Stable      | Iterators     |
| ----------- | ----------- | ----------- | ----------- | ----------- | ------------- |
| n log n     | n log n     | n log n     | n           | Yes         | Forward       |
| n log n     | n log n     | n log² n    | log n       | Yes         | Forward       |

When additional memory is available, `merge_sorter` runs in O(n log n), however if there is no additional memory available, it uses a O(n log² n) algorithm instead. The merging algorithm is memory adaptive, so even if it can only allocate a bit of memory instead of all the memory it needs, it will still take advantage of this additional memory. This memory scheme means that this sorter can't throw `std::bad_alloc`.

This sorter also has the following dedicated algorithms when used together with [`container_aware_adapter`][container-aware-adapter]:

| Container           | Best        | Average     | Worst       | Memory      | Stable      |
| ------------------- | ----------- | ----------- | ----------- | ----------- | ----------- |
| `std::list`         | n log n     | n log n     | n log n     | log n       | Yes         |
| `std::forward_list` | n log n     | n log n     | n log n     | log n       | Yes         |

None of the container-aware algorithms invalidates iterators.

### `pdq_sorter`

```cpp
#include <cpp-sort/sorters/pdq_sorter.h>
```

Implements a [pattern-defeating quicksort][pdqsort].

| Best        | Average     | Worst       | Memory      | Stable      | Iterators     |
| ----------- | ----------- | ----------- | ----------- | ----------- | ------------- |
| n           | n log n     | n log n     | log n       | No          | Random-access |

`pdq_sorter` uses a more performant partitioning algorithm under the hood if the comparison and projection functions generate branchless code. You can provide this information to the algorithm by specializing the library's [branchless traits][branchless-traits] for the given comparison/type or projection/type pairs if they aren't arleady handled natively by the library.

This sorter can't throw `std::bad_alloc`.

### `poplar_sorter`

```cpp
#include <cpp-sort/sorters/poplar_sorter.h>
```

Implements a *poplar sort*, which is a [heapsort][heapsort] derivate described by Coenraad Bron and Wim H. Hesselink in *Smoothsort revisited*. It builds a forest of perfect max heaps whose roots are stored on the right, then unheaps the elements to sort the collection.

| Best        | Average     | Worst       | Memory      | Stable      | Iterators     |
| ----------- | ----------- | ----------- | ----------- | ----------- | ------------- |
| n log n     | n log n     | n log n     | log n       | No          | Random-access |

This sorter is a bit faster or a bit slower than `smooth_sorter` depending on the patterns in the data to sort. I don't think it has any real advantage over `heap_sorter` in production code.

### `quick_merge_sorter`

```cpp
#include <cpp-sort/sorters/quick_merge_sorter.h>
```

Implements a flavour of [QuickMergesort][quick-mergesort].

| Best        | Average     | Worst       | Memory      | Stable      | Iterators     |
| ----------- | ----------- | ----------- | ----------- | ----------- | ------------- |
| n           | n log n     | n log n     | log n       | No          | Random-access |
| n           | n log n     | n log n     | log² n      | No          | Bidirectional |
| n           | n log² n    | n log² n    | log² n      | No          | Forward       |

QuickMergesort is an algorithm that performs a quicksort-like partition and tries to use mergesort on the bigger partition, using the smaller one as a swap buffer used for the merge operation when possible. The flavour of QuickMergesort used by `quick_merge_sorter` uses a [selection algorithm][selection-algorithm] to split the collection into partitions containing 2/3 and 1/3 of the elements respectively. This allows to use an internal mergesort of the biggest partition (2/3 of the elements) using the other partition (1/3 of the elements) as a swap buffer.

The change in time complexity for forward iterators is due to the partitioning algorithm being O(n log n) instead of O(n). The space complexity is dominated by the stack recursion in the selection algorithms:
* log n for the random-access version, which uses Andrei Alexandrescu's [*AdaptiveQuickselect*][adaptive-quickselect].
* log² n for the forward and bidirectional versions, which use the mutually recursive [introselect][introselect] algorithm.

This sorter can't throw `std::bad_alloc`.

*New in version 1.2.0*

*Changed in version 1.9.0:* the random-access version now runs in O(n log n) instead of accidentally running in O(n²).

### `quick_sorter`

```cpp
#include <cpp-sort/sorters/quick_sorter.h>
```

Implements a [quicksort][quicksort].

| Best        | Average     | Worst       | Memory      | Stable      | Iterators     |
| ----------- | ----------- | ----------- | ----------- | ----------- | ------------- |
| n           | n log n     | n log n     | log² n      | No          | Bidirectional |
| n           | n log² n    | n log² n    | log² n      | No          | Forward       |

Despite the name, this sorter actually implements some flavour of introsort: if quicksort performs more than 2*log(n) steps, it falls back to a [median-of-medians][median-of-medians] pivot selection instead of the usual median-of-9 one. The median-of-medians selection being mutually recursive with an introselect algorithm explains the use of log²n stack memory.

This sorter can't throw `std::bad_alloc`.

*Changed in version 1.2.0:* `quick_sorter` used to run in O(n²), but a fallback to median-of-medians pivot selection was introduced to make it run in O(n log n) or O(n log² n) depending of the iterator category, the tradeoff being the log² n space used by stack recursion (as opposed to the previous log n one).

### `selection_sorter`

```cpp
#include <cpp-sort/sorters/selection_sorter.h>
```

Implements a [selection sort][selection-sort].

| Best        | Average     | Worst       | Memory      | Stable      | Iterators     |
| ----------- | ----------- | ----------- | ----------- | ----------- | ------------- |
| n²          | n²          | n²          | 1           | No          | Forward       |

This sorter also has the following dedicated algorithms when used together with [`container_aware_adapter`][container-aware-adapter]:

| Container           | Best        | Average     | Worst       | Memory      | Stable      |
| ------------------- | ----------- | ----------- | ----------- | ----------- | ----------- |
| `std::list`         | n²          | n²          | n²          | 1           | Yes         |
| `std::forward_list` | n²          | n²          | n²          | 1           | Yes         |

None of the container-aware algorithms invalidates iterators.

### `slab_sorter`

```cpp
#include <cpp-sort/sorters/slab_sorter.h>
```

Implements a variant of slabsort, a rather slow but highly adaptive algorithm described by C. Levcopoulos and O. Petersson in *Sorting Shuffled Monotone Sequences*.

| Best        | Average     | Worst       | Memory      | Stable      | Iterators     |
| ----------- | ----------- | ----------- | ----------- | ----------- | ------------- |
| n           | n log n     | n log n     | n           | No          | Bidirectional |

This algorithm actually uses a rather big amount of memory but scales better than other O(n log n) algorithms of the library described as "slow" when the collections get bigger.

*New in version 1.10.0*

*Changed in version 1.12.1:* `slab_sorter` now works with bidirectional iterators.

### `smooth_sorter`

```cpp
#include <cpp-sort/sorters/smooth_sorter.h>
```

Implements a [smoothsort][smoothsort].

| Best        | Average     | Worst       | Memory      | Stable      | Iterators     |
| ----------- | ----------- | ----------- | ----------- | ----------- | ------------- |
| n           | n log n     | n log n     | 1           | No          | Random-Access |

While the complexity guarantees of this algorithm are optimal, this smoothsort isn't actually *fast* in practice. Except for some specific patterns (where `tim_sorter`, `pdq_sorter` or `verge_sorter` are still faster anyway), it is always almost twice as slow as `heap_sorter`. Huge collections and/or huge objects may make a difference, but I have yet to see a case where this is a useful sorting algorithm.

### `spin_sorter`

```cpp
#include <cpp-sort/sorters/spin_sorter.h>
```

Implements a [spinsort][spinsort].

| Best        | Average     | Worst       | Memory      | Stable      | Iterators     |
| ----------- | ----------- | ----------- | ----------- | ----------- | ------------- |
| n           | n log n     | n log n     | n           | Yes         | Random-Access |

*New in version 1.6.0*

### `splay_sorter`

```cpp
#include <cpp-sort/sorters/splay_sorter.h>
```

Implements a [splaysort][splaysort].

| Best        | Average     | Worst       | Memory      | Stable      | Iterators     |
| ----------- | ----------- | ----------- | ----------- | ----------- | ------------- |
| n           | n log n     | n log n     | n           | Yes         | Forward       |

*New in version 1.15.0*

### `split_sorter`

```cpp
#include <cpp-sort/sorters/split_sorter.h>
```

Implements an in-place *SplitSort* as descirbed in *Splitsort — an adaptive sorting algorithm* by C. Levcopoulos and O. Petersson. This library implements the simpler "in-place" version of the algorithm described in the paper.

| Best        | Average     | Worst       | Memory      | Stable      | Iterators     |
| ----------- | ----------- | ----------- | ----------- | ----------- | ------------- |
| n           | n log n     | n log n     | n           | No          | Random-Access |

SplitSort is a [*Rem*-adaptive][probe-rem] sorting algorithm and shares many similarities with drop-merge sort but has the following differences:
* It only works with random-access iterators.
* While it uses O(n) extra memory to merge some elements, it can run perfectly fine with O(1) extra memory.
* Benchmarks shows that drop-merge sort is better when few elements aren't in place, but SplitSort has a lower overhead on random data while still performing better than most general-purpose sorting algorithms when the data is already somewhat sorted.

This sorter can't throw `std::bad_alloc`.

*New in version 1.4.0*

### `std_sorter`

```cpp
#include <cpp-sort/sorters/std_sorter.h>
```

Uses the standard library [`std::sort`][std-sort] to sort a collection. While the complexity guarantees are only partial in the standard, here is what's expected:

| Best        | Average     | Worst       | Memory      | Stable      | Iterators     |
| ----------- | ----------- | ----------- | ----------- | ----------- | ------------- |
| n           | n log n     | n² *        | log n       | No          | Random-access |

\* *`std::sort` is mandated by the standard to be O(n log n), but the libc++ implementation of the algorithm - despite non-trivial optimizations - [is still O(n²)](https://bugs.llvm.org/show_bug.cgi?id=20837). If you are using another standard library implementation then `std_sorter` should be O(n log n) for randon-access iterators, as expected.*

The adapter [`stable_adapter`][stable-adapter] has an explicit specialization for `std_sorter` which calls [`std::stable_sort`][std-stable-sort] instead. Its complexity depends on whether it can allocate additional memory or not. While the complexity guarantees are only partial in the standard, here is what's expected:

| Best        | Average     | Worst       | Memory      | Stable      | Iterators     |
| ----------- | ----------- | ----------- | ----------- | ----------- | ------------- |
| n log n     | n log n     | n log n     | n           | Yes         | Random-access |
| n log² n    | n log² n    | n log² n    | 1           | Yes         | Random-access |

`std::sort` and `std::stable_sort` are likely not able to handle proxy iterators, therefore trying to use `std_sorter` with code that relies on proxy iterators (*e.g.* [`schwartz_adapter`][schwartz-adapter]) is deemed to cause errors. However, some standard libraries provide overloads of standard algorithms for some containers; for example, libc++ has an overload of `std::sort` for bit iterators, which means that `std_sorter` could be the best choice to sort an [`std::vector<bool>`][std-vector-bool].

This sorter can't throw `std::bad_alloc`.

### `tim_sorter`

```cpp
#include <cpp-sort/sorters/tim_sorter.h>
```

Implements a [timsort][timsort].

| Best        | Average     | Worst       | Memory      | Stable      | Iterators     |
| ----------- | ----------- | ----------- | ----------- | ----------- | ------------- |
| n           | n log n     | n log n     | n           | Yes         | Random-access |

While the sorting algorithm is stable and the complexity guarantees are good enough, this sorter is rather slow compared to the some other ones when the data distribution is random. That said, it would probably be a good choice when comparing data is expensive, but moving it is inexpensive (this is the use case for which it was designed).

*Changed in version 1.5.0:* `tim_sorter` now handles comparison and projection objects that aren't default-constructible.

### `verge_sorter`

```cpp
#include <cpp-sort/sorters/verge_sorter.h>
```

Implements a [vergesort][vergesort] algorithm backed by a quicksort derivative.

| Best        | Average     | Worst             | Memory      | Stable      | Iterators     |
| ----------- | ----------- | ----------------- | ----------- | ----------- | ------------- |
| n           | n log n     | n log n log log n | n           | No          | Random-access |
| n           | n log n     | n log n           | log n       | No          | Random-access |
| n           | n log n     | n log n log log n | n           | No          | Bidirectional |
| n           | n log n     | n log n           | log² n      | No          | Bidirectional |

Vergesort is a [*Runs*-adaptive][probe-runs] algorithm (including descending runs) as long as the size of those runs is greater than *n / log n*; when the runs are smaller, it falls back to another sorting algorithm to sort them (pdqsort for random-access iterators, QuickMergesort otherwise).

Vergesort's complexity is bound either by its optimization layer or by the fallback sorter's complexity:
* When it doesn't find big runs, the complexity is bound by the fallback sorter: depending on the category of iterators you can refer to the tables of either `pdq_sorter` or `quick_merge_sorter`.
* When it does find big runs, vergesort's complexity is bound by the merging phase of its optimization layer. In such a case, `inplace_merge` is used to merge the runs: it will use additional memory if any is available, in which case vergesort is O(n log n). If there isn't much extra memory available, it may still require O(log n) extra memory (and thus raise an `std::bad_alloc` if there isn't that much memory available) in which case the complexity falls to O(n log n log log n). It should not happen that much, and the additional *log log n* factor is likely irrelevant for most real-world applications.

When wrapped into [`stable_adapter`][stable-adapter], it has a slightly different behaviour: it detects strictly descending runs instead of non-ascending ones, and wraps the fallback sorter with `stable_t`. This make the specialization stable, and faster than just using `make_stable`.

*Changed in version 1.6.0:* when sorting a collection made of bidirectional iterators, `verge_sorter` falls back to `quick_merge_sorter` instead of `quick_sorter`.

*New in version 1.9.0:* explicit specialization for `stable_adapter<verge_sorter>`.

### `wiki_sorter<>`

```cpp
#include <cpp-sort/sorters/wiki_sorter.h>
```

Implements [WikiSort][wiki-sort], a kind of [block sort][block-sort].

| Best        | Average     | Worst       | Memory      | Stable      | Iterators     |
| ----------- | ----------- | ----------- | ----------- | ----------- | ------------- |
| n           | n log n     | n log n     | 1           | Yes         | Random-access |

`wiki_sorter` is a *buffered sorter* whose default specialization allocates a fixed buffer of 512 elements to achieve O(1) auxiliary memory. This memory complexity may change depending of the *buffer provider* passed to it.

```cpp
template<
    typename BufferProvider = utility::fixed_buffer<512>
>
struct wiki_sorter;
```

Whether this sorter works with types that are not default-constructible depends on the memory allocation strategy of the *buffer provider*. The default specialization does not work with such types.

*New in version 1.11.0*

## Type-specific sorters

The following sorters are available but will only work for some specific types instead of using a user-provided comparison function. Some of them also accept projections as long as the result of the projection can be handled by the sorter.

### `counting_sorter`

```cpp
#include <cpp-sort/sorters/counting_sorter.h>
```

`counting_sorter` implements a simple [counting sort][counting-sort]. This sorter also supports reverse sorting with [`std::greater<>`][std-greater-void] or [`std::ranges::greater`][std-ranges-greater].

| Best        | Average     | Worst       | Memory      | Stable      | Iterators     |
| ----------- | ----------- | ----------- | ----------- | ----------- | ------------- |
| n           | n+r         | n+r         | n+r         | No*         | Forward       |

This sorter works with any type satisfying the trait `std::is_integral` (as well as `[un]signed __int128` even when the standard library isn't properly instrumented to handle them). It can be insanely faster than other sorting algorithms when there are only a few different values in a tight range (*e.g.* values between 0 and 100 in an array of 10000 elements), but will be far too slow and eat too much memory if the range is wider than the number of elements (*e.g.* an array with two elements whose values are 0 and 100000). No memory is used if the collection is already sorted.

\* *Since the original integers are discarded and overwritten, whether the algorithm is stable or not does not mean much. Moreover, it can only sort integers, so the potential stability problems shouldn't even be observable.*

*Changed in version 1.6.0:* support for `[un]signed __int128`.

*Changed in version 1.9.0:* conditional support for [`std::ranges::greater`][std-ranges-greater].

### `ska_sorter`

```cpp
#include <cpp-sort/sorters/ska_sorter.h>
```

`ska_sorter` implements a [ska_sort][ska-sort].

| Best        | Average     | Worst       | Memory      | Stable      | Iterators     |
| ----------- | ----------- | ----------- | ----------- | ----------- | ------------- |
| n           | n           | n log n     | ?           | No          | Random-access |

Even though it isn't based on comparison, `ska_sorter` can sort a variety of types in ascending order:
* Any type satisfying the trait `std::is_integral`.
* `signed __int128` and `unsigned __int128` when available, even when they don't satisfy `std::is_integral`.
* `float` and `double` if they satisfy the trait `std::numeric_limits::is_iec559`, and if their sizes are respectively the same as those of `std::uint32_t` and `std::uin64_t`.
* Any `std::pair` or `std::tuple` provided the return type of `std::get<>` is also handled by `ska_sort`.
* Any type implementing `operator[]` provided its return type is also handled by `ska_sort` (this includes standard strings and random-access collections).

This sorter accepts projections, as long as `ska_sorter` can handle the return type of the projection.

*Changed in version 1.2.0:* support for `[un]signed __int128`.

### `spread_sorter`

```cpp
#include <cpp-sort/sorters/spread_sorter.h>
#include <cpp-sort/sorters/spread_sorter/integer_spread_sorter.h>
#include <cpp-sort/sorters/spread_sorter/float_spread_sorter.h>
#include <cpp-sort/sorters/spread_sorter/string_spread_sorter.h>
```

`spread_sorter` implements a [spreadsort][spreadsort].

| Best        | Average     | Worst       | Memory      | Stable      | Iterators     |
| ----------- | ----------- | ----------- | ----------- | ----------- | ------------- |
| n           | n*(k/d)     | n*(k/s+d)   | n*(k/d)     | No          | Random-access |

It comes into three main flavours (available individually if needed):

* `integer_spread_sorter` works with any type satisfying the trait `std::is_integral`.
* `float_spread_sorter` works with any type satisfying the trait `std::numeric_limits::is_iec559` whose size is the same as `std::uint32_t` or `std::uin64_t`.
* `string_spread_sorter` works with `std::string` and `std::wstring` (if `wchar_t` is 2 bytes). This sorter also supports reverse sorting with `std::greater<>` and `std::ranges::greater`. In C++17 it also works with `std::string_view` and `std::wstring_view` (if `wchar_t` is 2 bytes).

These sorters accept projections as long as their simplest form can handle the result of the projection. The three of them are aggregated into one main sorter the following way:

```cpp
struct spread_sorter:
    hybrid_adapter<
        integer_spread_sorter,
        float_spread_sorter,
        string_spread_sorter
    >
{};
```

*Changed in version 1.9.0:* conditional support for [`std::ranges::greater`][std-ranges-greater].


  [adaptive-quickselect]: https://arxiv.org/abs/1606.00484
  [adaptive-shivers-sort]: https://arxiv.org/abs/1809.08411
  [bitmap-allocator]: https://gcc.gnu.org/onlinedocs/libstdc++/manual/bitmap_allocator.html
  [block-sort]: https://en.wikipedia.org/wiki/Block_sort
  [bottom-up-heapsort]: https://en.wikipedia.org/wiki/Heapsort#Bottom-up_heapsort
  [branchless-traits]: Miscellaneous-utilities.md#branchless-traits
  [cartesian-tree-sort]: https://en.wikipedia.org/wiki/Cartesian_tree#Application_in_sorting
  [container-aware-adapter]: Sorter-adapters.md#container_aware_adapter
  [counting-sort]: https://en.wikipedia.org/wiki/Counting_sort
  [cppsort-sort]: Sorting-functions.md#cppsortsort
  [d-ary-heap]: https://en.wikipedia.org/wiki/D-ary_heap
  [default-sorter]: Sorters.md#default_sorter
  [drop-merge-sort]: https://github.com/emilk/drop-merge-sort
  [grailsort]: https://github.com/Mrrl/GrailSort
  [heapsort]: https://en.wikipedia.org/wiki/Heapsort
  [heap-sorter]: Sorters.md#heap_sorter
  [insertion-sort]: https://en.wikipedia.org/wiki/Insertion_sort
  [introselect]: https://en.wikipedia.org/wiki/Introselect
  [issue-168]: https://github.com/Morwenn/cpp-sort/issues/168
  [median-of-medians]: https://en.wikipedia.org/wiki/Median_of_medians
  [merge-sort]: https://en.wikipedia.org/wiki/Merge_sort
  [pdqsort]: https://github.com/orlp/pdqsort
  [probe-rem]: Measures-of-presortedness.md#rem
  [probe-runs]: Measures-of-presortedness.md#runs
  [quick-mergesort]: https://arxiv.org/abs/1307.3033
  [quicksort]: https://en.wikipedia.org/wiki/Quicksort
  [schwartz-adapter]: Sorter-adapters.md#schwartz_adapter
  [selection-algorithm]: https://en.wikipedia.org/wiki/Selection_algorithm
  [selection-sort]: https://en.wikipedia.org/wiki/Selection_sort
  [ska-sort]: https://probablydance.com/2016/12/27/i-wrote-a-faster-sorting-algorithm/
  [smoothsort]: https://en.wikipedia.org/wiki/Smoothsort
  [sorter-adapters]: Sorter-adapters.md
  [sorting-functions]: Sorting-functions.md
  [spinsort]: https://www.boost.org/doc/libs/1_80_0/libs/sort/doc/html/sort/single_thread/spinsort.html
  [splaysort]: https://en.wikipedia.org/wiki/Splaysort
  [spreadsort]: https://en.wikipedia.org/wiki/Spreadsort
  [stable-adapter]: Sorter-adapters.md#stable_adapter-make_stable-and-stable_t
  [std-greater-void]: https://en.cppreference.com/w/cpp/utility/functional/greater_void
  [std-ranges-greater]: https://en.cppreference.com/w/cpp/utility/functional/ranges/greater
  [std-sort]: https://en.cppreference.com/w/cpp/algorithm/sort
  [std-stable-sort]: https://en.cppreference.com/w/cpp/algorithm/stable_sort
  [std-vector-bool]: https://en.cppreference.com/w/cpp/container/vector_bool
  [timsort]: https://en.wikipedia.org/wiki/Timsort
  [vergesort]: https://github.com/Morwenn/vergesort
  [wiki-sort]: https://github.com/BonzaiThePenguin/WikiSort
  [wiki-sorter]: Sorters.md#wiki_sorter
  [writing-a-sorter]: Writing-a-sorter.md
  [writing-a-bubble-sorter]: Writing-a-bubble_sorter.md
