Also known as *measures of disorder*, the *measures of presortedness* are algorithms used to tell how much a sequence is already sorted, or how much disorder there is in it.

Given a measure of presortedness *M*, a comparison sort is said to be *M*-optimal if it takes a number of comparisons that is within a constant factor of the lower bound.

## Formal definition

Measures of presortedness were formally defined by H. Mannila in *Measures of presortedness and optimal sorting algorithms*. Here is the formal definition as reformulated by M. La Rocca & D. Cantone in [*NeatSort - A practical adaptive algorithm*][neatsort]:

> Given two sequences *X* and *Y* of distinct elements, a measure of disorder *M* is a function that satisfies the following properties:
>
> 1. If *X* is sorted, then *M*(*X*) = 0
> 2. If *X* and *Y* are order isomorphic, then *M*(*X*) = *M*(*Y*)
> 3. If *X* is a subset of *Y*, then *M*(*X*) ≤ *M*(*Y*)
> 4. If every element of *X* is smaller than every element of *Y*, then *M*(*X.Y*) ≤ *M*(*X*) + *M*(*Y*)
> 5. *M*({*x*}.*X*) ≤ |*X*| + *M*(*X*) for every natural integer *X*

A few measures of presortedness described in the research papers actually return 1 when *X* is already sorted, thus violating the first property above. We implement these measures in a such way that they return 0 instead, generally by subtracting 1 from the result of the described operation.

### Partial ordering of measures of presortedness

La Rocca & Cantone also define a partial order on measures of presortedness as follows:

Let *M1* and *M2* be two measures of presortedness.
- *M1* is algorithmically finer than *M2* if and only if any *M1*-optimal algorithm is also *M2*-optimal.
- *M1* and *M2* are algorithmically equivalent (denoted *M1*≡*M2* in the graph below) if and only if *M1* is algorithmically finer than *M2* and *M2* is algorithmically finer than *M1*.

The graph below shows the partial ordering of several measures of presortedness:
- *Reg* is algorithmically finest measure of presortedness.
- *m₀* is a measure of presortedness that always returns 0.
- *m₀₁* is a measure of presortedness that returns 0 when *X* is sorted and 1 otherwise.

![Partial ordering of measures of presortedness](images/mops-partial-ordering.png)

This graph is a modified version of the one in *A framework for adaptive sorting* by O. Petersson and A. Moffat. The relations of *Mono* are empirically derived [original research][original-research] and incomplete (unknown relations with *Max*, *Osc* and *SUS*).

The measures of presortedness in bold in the graph are available in **cpp-sort**, the others are not.

## Measures of presortedness in cpp-sort

In **cpp-sort**, measures of presortedness are implemented as instances of some specific function objects. They take an iterable or a pair of iterators and return how much disorder there is in the sequence according to the measure. Just like sorters, measures of presortedness can handle custom comparison and projection functions, and with the same degree of freedom when it comes to how they can be called:

```cpp
using namespace cppsort;
auto a = probe::inv(collection);
auto b = probe::rem(vec.begin(), vec.end());
auto c = probe::ham(li, std::greater<>{});
auto d = probe::runs(integers, std::negate<>{});
```

Note however that these algorithms can be expensive. Using them before an actual sorting algorithm has no interest at all; they are meant to be profiling tools: when sorting is a critical part of your application, you can use these measures on typical data and check whether it is mostly sorted according to one measure or another, then you may be able to find a sorting algorithm known to be optimal with regard to this specific measure.

Measures of presortedness can be used with the *sorter adapters* from the library. Even though most of the adapters are meaningless with measures of presortedness, some of them can still be used to mitigate space and time:

```cpp
// C++14
auto inv = cppsort::indirect_adapter<decltype(cppsort::probe::inv)>{};

// C++17
auto inv = cppsort::indirect_adapter(cppsort::probe::inv);
```

All measures of presortedness live in the subnamespace `cppsort::probe`. Even though all of them are available in their own header, it is possible to include all of them at once with the following include:

```cpp
#include <cpp-sort/probes.h>
```

### `max_for_size`

All measures of presortedness in the library have the following `static` member function:

```cpp
template<typename Integer>
static constexpr auto max_for_size(Integer n)
    -> Integer;
```

It takes an integer `n` and returns the maximum value that the measure of presortedness might return for a collection of size `n`.

*New in version 1.10.0*

## Available measures of presortedness

Measures of presortedness are pretty formalized, so the names of the functions in the library are short and correspond to the ones used in the literature.

In the following descriptions we use *X* to represent the input sequence, and |*X*| to represent the size of that sequence.

### *Block*

```cpp
#include <cpp-sort/probes/block.h>
```

Computes the number of elements in a sequence that aren't followed by the same element in the sorted sequence.

Our implementation is slightly different from the original description in *Sublinear merging and natural mergesort* by S. Carlsson, C. Levcopoulos and O. Petersson:
* It doesn't add 1 to the general result, thus returning 0 when *X* is sorted - therefore respecting the Mannila definition of a MOP.
* It explicitly handles elements that compare equivalent, while the original formal definition makes it difficult.

| Complexity  | Memory      | Iterators     |
| ----------- | ----------- | ------------- |
| n log n     | n           | Forward       |

`max_for_size`: |*X*| - 1 when *X* is sorted in reverse order.

*New in version 1.12.0*

### *Dis*

```cpp
#include <cpp-sort/probes/dis.h>
```

Computes the maximum distance determined by an inversion.

| Complexity  | Memory      | Iterators     |
| ----------- | ----------- | ------------- |
| n           | n           | Bidirectional |
| n log n     | 1           | Forward       |

When enough memory is available, `probe::dis` runs in O(n), otherwise it falls back to an O(n log n) algorithm that does not require extra memory. If forward iterators are passed, the O(n log n) algorithm is always used.

`max_for_size`: |*X*| - 1 when the last element of *X* is smaller than the first one.

*Changed in version 1.8.0:* `probe::dis` is now O(n²) instead of accidentally being O(n³) when passed forward or bidirectional iterators.

*Changed in version 1.12.0:* `probe::dis` is now O(n log n) instead of O(n²). When sorting bidirectional iterators, if enough heap memory is available, it runs in O(n) time and O(n) space.

### *Enc*

```cpp
#include <cpp-sort/probes/enc.h>
```

Computes the number of encroaching lists that can be extracted from *X* minus one (see *Encroaching lists as a measure of presortedness* by S. Skiena).

| Complexity  | Memory      | Iterators     |
| ----------- | ----------- | ------------- |
| n log n     | n           | Forward       |

`max_for_size`: (|*X*| + 1) / 2 - 1 when the values already extracted from *X* constitute stronger bounds than the values yet to be extracted (for example the sequence {0 9 1 8 2 7 3 6 4 5} will trigger the worst case).

### *Exc*

```cpp
#include <cpp-sort/probes/exc.h>
```

Computes the minimum number of exchanges required to sort *X*, which corresponds to |*X*| minus the number of cycles in the sequence. A cycle corresponds to a number of elements in a sequence that need to be rotated to be in their sorted position; for example, let {2, 4, 0, 6, 3, 1, 5} be a sequence, the cycles are {0, 2} and {1, 3, 4, 5, 6} so *Exc*(*X*) = |*X*| - 2 = 5.

| Complexity  | Memory      | Iterators     |
| ----------- | ----------- | ------------- |
| n log n     | n           | Forward       |

`max_for_size`: |*X*| - 1 when every element in *X* is one element away from its sorted position.

*Warning: this algorithm might be noticeably slower when the passed iterable is not random-access.*

### *Ham*

```cpp
#include <cpp-sort/probes/ham.h>
```

Computes the number of elements in *X* that are not in their sorted position, which corresponds to the [Hamming distance][hamming-distance] between *X* and its sorted permutation.

| Complexity  | Memory      | Iterators     |
| ----------- | ----------- | ------------- |
| n log n     | n           | Forward       |

`max_for_size`: |*X*| when every element in *X* is one element away from its sorted position.

### *Inv*

```cpp
#include <cpp-sort/probes/inv.h>
```

Computes the number of inversions in *X*, where an inversion corresponds to a pair (a, b) of elements not in order. For example, the sequence {2, 1, 3, 0} has 4 inversions: (2, 1), (2, 0), (1, 0) and (3, 0).

| Complexity  | Memory      | Iterators     |
| ----------- | ----------- | ------------- |
| n log n     | n           | Forward       |

`max_for_size`: |*X*| * (|*X*| - 1) / 2 when *X* is sorted in reverse order.

### *Max*

```cpp
#include <cpp-sort/probes/max.h>
```

Computes the maximum distance an element in *X* must travel to find its sorted position.

| Complexity  | Memory      | Iterators     |
| ----------- | ----------- | ------------- |
| n log n     | n           | Forward       |

`max_for_size`: |*X*| - 1 when *X* is sorted in reverse order.

*Warning: this algorithm might be noticeably slower when the passed iterable is not random-access.*

### *Mono*

```cpp
#include <cpp-sort/probes/mono.h>
```

Computes the number of non-increasing and non-decreasing consecutive runs of adjacent elements that need to be removed from *X* to make it sorted

The measure of presortedness is slightly different from its original description in [*Sort Race*][sort-race] by H. Zhang, B. Meng and Y. Liang:
* It subtracts 1 from the number of runs, thus returning 0 when *X* is sorted.
* It explicitly handles non-increasing and non-decreasing runs, not only the strictly increasing or decreasing ones.

| Complexity  | Memory      | Iterators     |
| ----------- | ----------- | ------------- |
| n           | 1           | Forward       |

`max_for_size`: (|*X*| + 1) / 2 - 1 when *X* is a sequence of elements that are alternatively greater then lesser than their previous neighbour.

*New in version 1.1.0*

### *Osc*

```cpp
#include <cpp-sort/probes/osc.h>
```

Computes the *Oscillation* measure described by C. Levcopoulos and O. Petersson in *Adaptive Heapsort*.

| Complexity  | Memory      | Iterators     |
| ----------- | ----------- | ------------- |
| n log n     | n           | Forward       |
| n²          | 1           | Forward       |

When there isn't enough extra memory available, `probe::osc` falls back to an in-place O(n²) algorithm.

`max_for_size`: (|*X*| * (|*X*| - 2) - 1) / 2 when the values in *X* are strongly oscillating.

***WARNING:** the O(n²) fallback of `probe::osc` is deprecated since version 1.12.0 and removed in version 2.0.0.*

*Changed in version 1.12.0:* `probe::osc` is now O(n log n) instead of O(n²) but now also requires O(n) memory. The O(n²) is kept for backward compatibility but will be removed in the future.

### *Rem*

```cpp
#include <cpp-sort/probes/rem.h>
```

Computes the minimum number of elements that must be removed from *X* to obtain a sorted subsequence, which corresponds to |*X*| minus the size of the [longest non-decreasing subsequence][longest-increasing-subsequence] of *X*.

| Complexity  | Memory      | Iterators     |
| ----------- | ----------- | ------------- |
| n log n     | n           | Forward       |

`max_for_size`: |*X*| - 1 when *X* is sorted in reverse order.

### *Runs*

```cpp
#include <cpp-sort/probes/runs.h>
```

Computes the number of non-decreasing runs in *X* minus one.

| Complexity  | Memory      | Iterators     |
| ----------- | ----------- | ------------- |
| n           | 1           | Forward       |

`max_for_size`: |*X*| - 1 when *X* is sorted in reverse order.

### *SUS*

```cpp
#include <cpp-sort/probes/sus.h>
```

Computes the minimum number of non-decreasing subsequences (of possibly not adjacent elements) into which *X* can be partitioned. It happens to correspond to the size of the [longest decreasing subsequence][longest-increasing-subsequence] of *X*.

*SUS* stands for *Shuffled Up-Sequences* and was introduced in *Sorting Shuffled Monotone Sequences* by C. Levcopoulos and O. Petersson.

| Complexity  | Memory      | Iterators     |
| ----------- | ----------- | ------------- |
| n log n     | n           | Forward       |

`max_for_size`: |*X*| - 1 when *X* is sorted in reverse order.

*New in version 1.10.0*

## Other measures of presortedness

Some additional measures of presortedness how been described in the literature but do not appear in the partial ordering graph. This section describes some of them but is not an exhaustive list.

### *Par*

*Par* was described by V. Estivill-Castro and D. Wood in *A New Measure of Presortedness* as follows:

> *Par(X)* = min { *p* | *X* is *p*-sorted }

The following definition is also given to determine whether a sequence is *p*-sorted:

> *X* is *p*-sorted iff for all *i*, *j* ∈ {1, 2, ..., |*X*|}, *i* - *j* > *p* implies *Xj* ≤ *Xi*. 

*Right invariant metrics and measures of presortedness* by V. Estivill-Castro, H. Mannila and D. Wood mentions that:

> In fact, *Par*(*X*) = *Dis*(*X*), for all *X*.

In their subsequent papers, those authors consistently use *Dis* instead of *Par*, often accompanied by a link to *A New Measure of Presortedness*.

### *Radius*

T. Altman and Y. Igarashi mention the concept of *k*-sortedness and the measure *Radius*(*X*) in *Roughly Sorting: Sequential and Parallel Approach*. However *k*-sortedness is the same as *p*-sortedness, and *Radius* is just another name for *Par* (and thus for *Dis*).


  [hamming-distance]: https://en.wikipedia.org/wiki/Hamming_distance
  [longest-increasing-subsequence]: https://en.wikipedia.org/wiki/Longest_increasing_subsequence
  [neatsort]: https://arxiv.org/pdf/1407.6183.pdf
  [original-research]: Original-research.md#partial-ordering-of-mono
  [probe-dis]: Measures-of-presortedness.md#dis
  [sort-race]: https://arxiv.org/ftp/arxiv/papers/1609/1609.04471.pdf
