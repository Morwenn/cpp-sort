Miscellaneous utilities
=======================

The following utilities are available in the directory `cpp-sort/utility`
and live in the namespace `cppsort::utility`. While not directly related
to sorting, they might be useful.

The detection framework
-----------------------

```cpp
#include <cpp-sort/utility/detection.h>
```

**cpp-sort** implements the [detection framework](http://en.cppreference.com/w/cpp/experimental/is_detected)
proposed by [N4502](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4502.pdf)
which should be available in the Library Fundamentals TS v2.

Since the documentation is already available online, it won't be repeated it
here. Note that this header also provides the types `void_t` and `nonesuch` which
are used to implement the detection framework.

`has_sort_method`
-----------------

```cpp
#include <cpp-sort/utility/has_sort_method.h>
```

The variable template `has_sort_method<typename Iterable>` equals `true` if
the type `Iterable` has a method name `sort` which can be called without any
parameter and equals `false` otherwise.

```cpp
template<typename Iterable>
constexpr bool has_sort_method = /* implementation-defined */;

`is_in_range`
-------------

```cpp
#include <cpp-sort/utility/is_in_range.h>
```

The variable template `is_in_range<std::size_t Value, std::size_t... Values>`
equals `true` if `Value` exists into the non-type template parameter pack
`Values` and equals `false` otherwise.

```cpp
template<std::size_t Value, std::size_t... Values>
constexpr bool is_in_pack = /* implementation-defined */;
```

`is_sorter_for`
---------------

```cpp
#include <cpp-sort/utility/is_sorter_for.h>
```

The variable template `is_sorter_for<typename Sorter, typename Iterable>`
equals `true` if the type `Sorter` is a [*sorter*](sorters.md) and equals
`false` otherwise. In our case, a `Sorter` is a type that can be called
with an `Iterable&`. It mainly exists for SFINAE purpose.

```cpp
template<typename Sorter, typename Iterable>
constexpr bool is_sorter_for = /* implementation-defined */;
```
