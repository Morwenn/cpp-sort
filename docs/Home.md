Welcome to the **cpp-sort 1.10.0** documentation!

You probably read the introduction in the README, so I won't repeat it here. This wiki contains documentation about the library: basic documentation about the many sorting tools and how to use them, documentation about the additional utilities provided by the library and even some detailed tutorials if you ever want to write your own sorters or sorter adapters. This main page explains a few general things that didn't quite fit in other parts of the documentation.

If you find a library feature which isn't documented, don't hesitate to open an issue. It generally means that we either need to hide it in a subnamespace or to let it available and document it.

## General algorithm properties

### Exception safety

Every library function provides the no-leak guarantee, which means that all the memory allocated by components of the library will automatically be deallocated, even when an exception is thrown. The library itself should never throw exceptions, except `std::bad_alloc` when memory couldn't be allocated: in the different parts of the documentation, algorithms whose space complexity is worse than O(1) can throw `std::bad_alloc` unless the documentation explicitly mentions that they can't.

If the library throws any other exception, it will likely come from user code. The main functions that are likely to throw exceptions are the following ones:
* User-defined comparison functions
* User-defined projection functions
* Move & swap operations of the objects to sort
* Operations of the iterators/containers passed to the algorithms

**cpp-sort** makes no guarantee about the state of the passed collection after an exception is thrown while an algorithm runs. It might result in several objects in a moved-from state if an exception is thrown while elements of the collection to sort have been moved to a temporary buffer.

### Self-move and self-swap guarantees

**cpp-sort** strives to follow the standard library guidance when it comes to self-move and self-swap operations: when sorting a collection of `T`, the expectations are as follows:
* The algorithms in the library should never perform a blind self-move operation since there is no guarantee about the result of such an operation.
* However the different algorithms expect `T` to satisfy the C++20 [`swappable` concept][swappable]. This concepts requires that, for any value `x` of type `T`, the following code doesn't change the value of `x`:
    ```cpp
    using std::swap;
    swap(x, x);
    ```
    This means that some of the library's algorithms can actually perform self-swap operations, and it is the responsibility of the author of class `T` to make sure that such code works as expected.

*New in version 1.4.0*

### Determinism & reproducibility

So far every algorithm in the library is deterministic: for a given input, one should always get the exact same sequence of operations performed. It was a deliberate choice not to use algorithms such as random pivot quicksort or random sampling algorithms.

The library does not contain multithreaded algorithms either for now, further guaranteeing reproducibility.

## Library information & configuration

### Versioning

**cpp-sort** follows semantic versioning and provides the following macros in `<cpp-sort/version.h>` to check the current major, minor and patch versions:

```cpp
CPPSORT_VERSION_MAJOR
CPPSORT_VERSION_MINOR
CPPSORT_VERSION_PATCH
```

### Forward declarations

**cpp-sort** ships with a header that includes the forward declarations for the library's main components.

```cpp
#include <cpp-sort/fwd.h>
```

Including that header will provide forward declarations for sorters, fixed-size sorters, and sorter adapters.

### Deprecation warnings

Some old components undergo deprecation before being removed in the following major revisions of the library. Those components are marked as `[[deprecated]]` and thus will warn when used. You can silence the deprecation warnings by defining the preprocessor macro `CPPSORT_DISABLE_DEPRECATION_WARNINGS`.

*New in version 1.8.0*

### Assertions & audits

Some algorithms have assertions to guard against accidental logic issues (mostly in algorithms adapted from other projects), but they are disabled by default. You can enable these assertions by defining the preprocessor macro `CPPSORT_ENABLE_ASSERTIONS`. This new macro still honours `NDEBUG`, so assertions won't be enabled anyway if `NDEBUG` is defined.

A similar `CPPSORT_ENABLE_AUDITS` macro can be defined to enable audits: those are expensive assertions which are not enabled by `CPPSORT_ENABLE_ASSERTIONS` because they are too expensive, to the point that they might even change the complexity of some algorithms.

*New in version 1.6.0*

*New in version 1.9.0*: `CPPSORT_ENABLE_AUDITS`

## Miscellaneous

This wiki also includes a small section about the [[original research|Original research]] that happened during the conception of the library and the results of this research. While it is not needed to understand how the library works or how to use it, it may be of interest if you want to discover new things about sorting.

If you ever feel that this wiki is incomplete, that it needs more examples or more detailed explanation about stuff, you are welcome to report it and/or contribute.

*Always keep in mind that even if the library does contain production-ready algorithms, many of them are also experimental algorithms taken straight from research papers and reimplemented from scratch. If you are only interested in usable algorithms, you should look at the ones analyzed in the [[benchmarks]].*

Hope you have fun!

  [swappable]: https://en.cppreference.com/w/cpp/concepts/swappable
