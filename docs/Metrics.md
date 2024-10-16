*New in version 1.15.0*

*Metrics* are special kinds of [*sorter adapters*][sorter-adapters] that can be used to compute properties about a sorting algorithm, such as the number of comparisons performed while sorting a given collection, or the time it took to sort. As *sorter adapters*, metrics follow the *unified sorting interface*, and *do* sort the passed collection before returning the result.

The anatomy of a metric adapter is thus close to that of any *sorter adapter* with the following additional properties:
* The result of the *adapted sorter*'s is replaced with an instance of [`cppsort:::utility::metric`][utility-metrics-tools], which is a small wrapper type encapsulating a value.
* It exposes a `tag_t` type aliasing the tag of the returned metric value.
* It exposes a `metric_t` type aliasing the returned metric type.

The metrics tag can be any type, generally ending with the `_tag` suffix, and can be either empty or contain freeform static metadata about the kind of metric that uses it. Future versions of **cpp-sort** might standardize some tag fields.

## Available metrics

All available metrics live in the subnamespace `cppsort::metrics`. Even though all of them are available in their own standalone header, it is possible to include all of them at once with the following include:

```cpp
#include <cpp-sort/metrics.h>
```

All of the metrics headers also includes `<cpp-sort/utility/metrics_tools.h>`.

*Warning: none of these metrics are thread-safe.*

### `comparisons`

```cpp
#include <cpp-sort/metrics/comparisons.h>
```

Computes the number of comparisons performed by the *adapted sorter*.

This is done by wrapping the passed comparison function. As such it only works with sorters that accept a comparison, and bypasses components that special-case specific comparison objects, though it does pretend [to be branchless][branchless-traits] when the wrapped comparison function is.

```cpp
template<
    typename Sorter,
    typename CountType = std::size_t
>
struct comparisons;
```

Returns an instance of `utility::metric<CountType, comparisons_tag>`.

*Changed in version 1.16.0:* `comparisons` now honours [`is_probably_branchless_comparison`][branchless-traits] to better represent the branches taken by the analyzed comparison function.

### `moves`

```cpp
#include <cpp-sort/metrics/moves.h>
```

Computes the number of moves performed by the *adapted sorter*: it takes both the number of calls to the move constructor and to the move assignment operator of an object into account. A swap operation is considered equivalent to three moves.

The tool currently works by creating a vector of a wrapper type which counts its moves, and moving the sorted contents back to the original collection, with the following implications:
* Said vector's iterators mimic the iterator category of the range passed to the *resulting sorter*: as such, behaviours that depend on the iterator _category_ are respected, but behaviours that depends on a specific iterator _type_ are not.
* Sorters that call operations specific to some types might return a result that is not representative of how they actually perform: this is due to the wrapper not benefiting from the specializations.
* Projection support is mandatory: `metrics::moves` passes a projection to the sorter in order to convert the wrapper type to its underlying type.

```cpp
template<
    typename Sorter,
    typename CountType = std::size_t
>
struct moves;
```

Returns an instance of `utility::metric<CountType, moves_tag>`.

*New in version 1.16.0*

*Changed in version 1.17.0:* `metrics::moves` now mimicks the iterator category of the passed range passed to the *resulting sorter*, leading to more accurate results.

### `projections`

```cpp
#include <cpp-sort/metrics/projections.h>
```

Computes the number of projections performed by the *adapted sorter*.

This is done by wrapping the passed projection function. As such it only works with sorters that accept a projection, and bypasses components that special-case specific projection objects, though it does pretend [to be branchless][branchless-traits] when the wrapped projection function is.

```cpp
template<
    typename Sorter,
    typename CountType = std::size_t
>
struct projections;
```

Returns an instance of `utility::metric<CountType, projections_tag>`.

*Changed in version 1.16.0:* `projections` now honours [`is_probably_branchless_projection`][branchless-traits] to better represent the branches taken by the analyzed projection function.

### `running_time`

```cpp
#include <cpp-sort/metrics/running_time.h>
```

Computes the time it takes for the *adapted sorter* to sort a collection, using a steady clock by default.

```cpp
template<
    typename Sorter,
    typename DurationType = typename std::chrono::steady_clock::duration
>
struct running_time;
```

Returns an instance of `utility::metric<DurationType, running_type_tag>`.


  [branchless-traits]: Miscellaneous-utilities.md#branchless-traits
  [sorter-adapters]: Sorter-adapters.md
  [utility-metrics-tools]: Miscellaneous-utilities.md#metrics-tools
