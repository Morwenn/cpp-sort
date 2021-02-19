Also known as *measures of disorder*, the *measures of presortedness* are algorithms used to tell how much a sequence is already sorted, or how much disorder there is in it. Some adaptive sorting algorithms are known to take advantage of the order already present in a sequence, and happen to be "optimal" with regard to some measures of presortedness.

Measures of presortedness were formally defined by Manilla in *Measures of presortedness and optimal sorting algorithms*. Here is the formal definition as reformulated by La rocca & Cantone in [*NeatSort - A practical adaptive algorithm*][neatsort]:

> Given two sequences *X* and *Y* of distinct elements, a measure of disorder *M* is a function that satisfies the following properties:
>
> 1. If *X* is sorted, then *M*(*X*) = 0
> 2. If *X* and *Y* are order isomorphic, then *M*(*X*) = *M*(*Y*)
> 3. If *X* is a subset of *Y*, then *M*(*X*) ≤ *M*(*Y*)
> 4. If every element of *X* is smaller than every element of *Y*, then *M*(*X.Y*) ≤ *M*(*X*) + *M*(*Y*)
> 5. *M*({*x*}.*X*) ≤ |*X*| + *M*(*X*) for every natural integer *X*

A few measures of presortedness described in the research papers actually return 1 when *X* is already sorted, thus violating the first property above. We implement these measures in a such way that they return 0 instead, generally by subtracting 1 from the result of the described operation.

---

In **cpp-sort**, measures of presortedness are implemented as instances of some specific function objects; they take an iterable or a pair of iterators and return how much disorder there is in the sequence according to the measure. Just like sorters, measures of presortedness can handle custom comparison and projection functions, and with the same degree of freedom when it comes to how they can be called:

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

Every measure of presortedness lives in the subnamespace `cppsort::probe`. Even though all of them are available in their own header, it is possible to include all of them at once with the following include:

```cpp
#include <cpp-sort/probes.h>
```

Measures of presortedness are pretty formalized, so the names of the functions in the library are short and correspond to the ones used in the litterature. As per this litterature, we will use the symbols *X* to represent the analyzed sequence, and *n* to represent the size of that sequence.

### *Dis*

```cpp
#include <cpp-sort/probes/dis.h>
```

Computes the maximum distance determined by an inversion.

**Worst case:** *n* - 1 when *X* is sorted in reverse order.

| Complexity  | Memory      | Iterators     |
| ----------- | ----------- | ------------- |
| n²          | 1           | Forward       |

*Warning: this algorithm might be noticeably slower when the passed iterable is not random-access.*

*Changed in version 1.8.0:* `probe::dis` is now O(n²) instead of accidentally being O(n³) when passed forward or bidirectional iterators.

### *Enc*

```cpp
#include <cpp-sort/probes/enc.h>
```

Computes the number of encroaching lists that can be extracted from *X* minus one (see Skiena's *Encroaching lists as a measure of presortedness*).

**Worst case:** *n* / 2 - 1 when the values already extracted from *X* constitute stronger bounds than the values yet to be extracted (for example the sequence {0 9 1 8 2 7 3 6 4 5} will trigger the worst case).

| Complexity  | Memory      | Iterators     |
| ----------- | ----------- | ------------- |
| n log n     | n           | Forward       |

### *Exc*

```cpp
#include <cpp-sort/probes/exc.h>
```

Computes the minimum number of exchanges required to sort *X*, which corresponds to *n* minus the number of cycles in the sequence. A cycle corresponds to a number of elements in a sequence that need to be rotated to be in their sorted position; for example, let {2, 4, 0, 6, 3, 1, 5} be a sequence, the cycles are {0, 2} and {1, 3, 4, 5, 6} so *Exc*(*X*) = *n* - 2 = 5.

**Worst case:** *n* - 1 when every element in *X* is one element away from its sorted position.

| Complexity  | Memory      | Iterators     |
| ----------- | ----------- | ------------- |
| n log n     | n           | Forward       |

*Warning: this algorithm might be noticeably slower when the passed iterable is not random-access.*

### *Ham*

```cpp
#include <cpp-sort/probes/ham.h>
```

Computes the number of elements in *X* that are not in their sorted position, which corresponds to the [Hamming distance][hamming-distance] between *X* and its sorted permutation.

**Worst case:** *n* when every element in *X* is one element away from its sorted position.

| Complexity  | Memory      | Iterators     |
| ----------- | ----------- | ------------- |
| n log n     | n           | Forward       |

### *Inv*

```cpp
#include <cpp-sort/probes/inv.h>
```

Computes the number of inversions in *X*, where an inversion corresponds to a pair (a, b) of elements not in order. For example, the sequence {2, 1, 3, 0} has 4 inversions: (2, 1), (2, 0), (1, 0) and (3, 0).

**Worst case:** returns *n* * (*n* - 1) / 2 when *X* is sorted in reverse order.

| Complexity  | Memory      | Iterators     |
| ----------- | ----------- | ------------- |
| n log n     | n           | Forward       |

### *Max*

```cpp
#include <cpp-sort/probes/max.h>
```

Computes the maximum distance an element in *X* must travel to find its sorted position.

**Worst case:** *n* - 1 when *X* is sorted in reverse order.

| Complexity  | Memory      | Iterators     |
| ----------- | ----------- | ------------- |
| n log n     | n           | Forward       |

*Warning: this algorithm might be noticeably slower when the passed iterable is not random-access.*

### *Mono*

```cpp
#include <cpp-sort/probes/mono.h>
```

Computes the number of non-increasing and non-decreasing runs in *X* minus one.

The measure of presortedness is slightly different from its original description in [*Sort Race*][sort-race] by Zhang, Meng and Liang:
* It subtracts 1 from the number of runs, thus returning 0 when *X* is sorted
* It explicitly handles non-increasing and non-decreasing runs, not only the strictly increasing or decreasing ones

**Worst case:** *n* / 2 - 1 when *X* is a sequence of elements that are alternatively greater then lesser than their previous neighbour.

| Complexity  | Memory      | Iterators     |
| ----------- | ----------- | ------------- |
| n           | 1           | Forward       |

*New in version 1.1.0*

### *Osc*

```cpp
#include <cpp-sort/probes/osc.h>
```

Computes the *Oscillation* measure described by Levcopoulos and Petersson in *Adaptive Heapsort*.

**Worst case:** (*n* * (*n* - 2) - 1) / 2 when the values in *X* are strongly oscillating.

| Complexity  | Memory      | Iterators     |
| ----------- | ----------- | ------------- |
| n²          | 1           | Forward       |

### *Par*

```cpp
#include <cpp-sort/probes/par.h>
```

Computes the *Par* measure described by Estivill-Castro and Wood in *A New Measure of Presortedness* as follows:

> *Par(X)* = min { *p* | *X* is *p*-sorted }

The following definition is also given to determine whether a sequence is *p*-sorted:

> *X* is *p*-sorted iff for all *i*, *j* ∈ {1, 2, ..., *n*}, *i* - *j* > *p* implies *Xj* ≤ *Xi*. 

**Worst case:** (*n* - 1) when the last element of *X* is smaller than the first one.

| Complexity  | Memory      | Iterators     |
| ----------- | ----------- | ------------- |
| n² log n    | 1           | Random-access |

### *Rem*

```cpp
#include <cpp-sort/probes/rem.h>
```

Computes the minimum number of elements that must be removed from *X* to obtain a sorted subsequence, which corresponds to *n* minus the size of the [longest non-decreasing subsequence][longest-increasing-subsequence] of *X*.

**Worst case:** *n* - 1 when *X* is sorted in reverse order.

| Complexity  | Memory      | Iterators     |
| ----------- | ----------- | ------------- |
| n log n     | n           | Forward       |

### *Runs*

```cpp
#include <cpp-sort/probes/runs.h>
```

Computes the number of non-decreasing runs in *X* minus one.

**Worst case:** *n* - 1 when *X* is sorted in reverse order.

| Complexity  | Memory      | Iterators     |
| ----------- | ----------- | ------------- |
| n           | 1           | Forward       |

### *SUS*

```cpp
#include <cpp-sort/probes/sus.h>
```

Computes the minimum number of non-decreasing subsequences (of possibly not adjacent elements) into which *X* can be partitioned. It happens to correspond to the size of the [longest decreasing subsequence][longest-increasing-subsequence] of *X*.

*SUS* stands for *Shuffled Up-Sequences* and was introduced in *Sorting Shuffled Monotone Sequences* by Levcopoulos and Petersson.

**Worst case:** *n* - 1 when *X* is sorted in reverse order.

| Complexity  | Memory      | Iterators     |
| ----------- | ----------- | ------------- |
| n log n     | n           | Forward       |

*New in version 1.10.0*


  [hamming-distance]: https://en.wikipedia.org/wiki/Hamming_distance
  [longest-increasing-subsequence]: https://en.wikipedia.org/wiki/Longest_increasing_subsequence
  [neatsort]: https://arxiv.org/pdf/1407.6183.pdf
  [sort-race]: https://arxiv.org/ftp/arxiv/papers/1609/1609.04471.pdf
