*Presorters* are function objects that implement procedures meant to reduce the disorder in a sequence,
that can be run before an adaptive sorting algorithm.

The concept has existed in a fairly ad-hoc fashion for long while,
be it only because most sorting algorithms are naturally composed of operations that reduce disorder.
It was later formalized by Hsien-Kuei Hwang, Bo-Yin Yang and Yeong-Nan Yeh in *Presorting algorithms: an average-case point of view*:

> *Instead of passively measuring the quantity of presortedness of the input,
> [a presorting algorithm] actively "creates" sortedness of the input by performing certain simple procedures
> with an aim to decrease the quantity of some [measure of disorder][measures-of-disorder].*

The paper takes the stance that measurably and deterministically reducing disorder only makes sense when considering the average-case point of view.

TODO: example, explain better

## Presorters

Presorters below follow the same design decisions as [sorters][sorters]:
they are function objects that implement the *unified sorting interface*,
and any additional configuration happens through parameters passed to the constructor.

Concretely, they can be seen as "sorters that don't sort": the interface is exactly the same.

Every `*_presorter` class is accompanied by a corresponding `*_presort` instance that can be used as a function.

### `max_heap_presorter`

```cpp
#include <cpp-sort/presorters/max_heap_presorter.h>
```

Builds a [max heap][binary-heap], similar to calling [`std::make_heap`][std-make-heap] on the input range.

| Time        | Memory      | Stable      | Iterators     |
| ----------- | ----------- | ----------- | ------------- |
| n           | 1           | No          | Random-access |


### `min_heap_presorter`

```cpp
#include <cpp-sort/presorters/min_heap_presorter.h>
```

Builds a [min heap][binary-heap], similar to calling [`std::make_heap`][std-make-heap] on the input range with [`std::greater`][std-greater] as a comparator.

| Time        | Memory      | Stable      | Iterators     |
| ----------- | ----------- | ----------- | ------------- |
| n           | 1           | No          | Random-access |


### `poplar_heap_presorter`

```cpp
#include <cpp-sort/presorters/poplar_heap_presorter.h>
```

Turns the range into a [poplar heap][poplar-heap].

| Time        | Memory      | Stable      | Iterators     |
| ----------- | ----------- | ----------- | ------------- |
| n           | 1           | No          | Random-access |


### `reverse_descending_runs_presorter`

```cpp
#include <cpp-sort/presorters/reverse_descending_runs_presorter.h>
```

The presorter recognizes ascending and descending runs in the sequence in a manner similar to [`probe::mono`][probe-mono],
and reverses the descending runs it finds.

A major difference with `probe::mono` is that it detects strictly descending runs instead of non-ascending ones.
The rationale for that difference is that it allows to preserve stability when reversing runs.
It can be made similar to `probe::mono` by passing it [`std::less_equal`][std-less-equal]:

```cpp
std::vector<int> sequence = { 1, 2, 3, 4, 7, 6, 6, 5, 4, 4, 2 };
cppsort::reverse_descending_runs_presort(sequence, std::less_equal{});
std::println(sequence); // [1, 2, 3, 4, 7, 2, 4, 4, 5, 6, 6]
```

Given an arbitrary predicate, the same "less-then-or-equal" behavior can be obtained by passing said predicate through [`cppsort::flip`][cppsort-flip] and [`cppsort::not_fn`][cppsort-not-fn].

When using such a "less-than-or-equal" predicate, `reverse_descending_runs_presorter` turns a sequence $X$ into a sequence $Y$ such as $\mathit{Runs}(Y) = \mathit{Mono}(X)$.
In other words, when run before a $\mathit{Runs}$-adaptive sorting algorithm,
the combination of both operations is a $\mathit{Mono}$-adaptive sorting algorithm.

| Time        | Memory      | Stable      | Iterators     |
| ----------- | ----------- | ----------- | ------------- |
| n           | 1           | Yes         | Bidirectional |


  [cppsort-flip]: https://codeberg.org/Morwenn/cpp-sort/wiki/Comparator-adapters#flip
  [cppsort-not-fn]: https://codeberg.org/Morwenn/cpp-sort/wiki/Comparator-adapters#not_fn
  [binary-heap]: https://en.wikipedia.org/wiki/Binary_heap
  [measures-of-disorder]: https://codeberg.org/Morwenn/cpp-sort/wiki/Measures-of-disorder
  [poplar-heap]: https://github.com/Morwenn/poplar-heap
  [probe-mono]: https://codeberg.org/Morwenn/cpp-sort/wiki/Measures-of-disorder#mono
  [probe-runs]: https://codeberg.org/Morwenn/cpp-sort/wiki/Measures-of-disorder#runs
  [sorters]: Sorters.md
  [std-greater]: https://cppreference.com/cpp/utility/functional/greater
  [std-less-equal]: https://cppreference.com/cpp/utility/functional/less_equal
  [std-make-heap]: https://cppreference.com/cpp/algorithm/make_heap
