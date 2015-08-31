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

`any` and `all`
---------------

```cpp
#include <cpp-sort/utility/any_all.h>
```

The `constexpr` functions `any` and `all` take several boolean parameters and
respectively return whether any of them is `true` or whether all of them are
`true`.

```cpp
template<typename... Bools>
constexpr bool any(bool head, Bools... tail);

template<typename... Bools>
constexpr bool all(bool head, Bools... tail);
```

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
```

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

`make_integer_range`
--------------------

```cpp
#include <cpp-sort/utility/make_integer_range.h>
```

`make_integer_range` is a classe that can be used where an [`std::integer_sequence`](http://en.cppreference.com/w/cpp/utility/integer_sequence)
can be used. An `integer_range` takes a type template parameter that shall be
an integer type, then three integer template parameters which correspond to the
beginning of the range, the end of the range and the « step ».

```cpp
template<
    typename Integer,
    Integer Begin,
    Integer End,
    Integer Step = 1
>
using make_integer_range = /* implementation-defined */;
```

`make_index_range` is a specialization of `integer_range` for `std::size_t`.

```cpp
template<
    std::size_t Begin,
    std::size_t End,
    std::size_t Step = 1u
>
using make_index_range = make_integer_range<std::size_t, Begin, End, Step>;
```

`size`
------

```cpp
#include <cpp-sort/utility/size.h>
```

`size` is a function that can be used to get the size of an iterable. It is equivalent
to the C++17 function [`std::size`](http://en.cppreference.com/w/cpp/iterator/size)
but has an additional tweak so that, if the iterable is not a fixed-size C array and
doesn't have a `size` method, it calls `std::distance(std::begin(iter), std::end(iter))`
on the iterable. Therefore, this function can also be used for `std::forard_list` as
well as some implementations of ranges.
