**cpp-sort** is a generic C++14 header-only sorting library. Its goal is not
to replace `std::sort` but rather to *complete* it so that it can choose more
efficient algorithms in some situations. It does so by providing two additional
algorithms: `cppsort::sort` and `cppsort::sort_n`.

`cppsort::sort_n` takes an `std::size_t` template parameter corresponding to the
number of values to sort, and a random-access iterator corresponding to the
beginning of the range to sort. It then proceeds to sort the first `N` elements
of the range using optimal sorting algorithms for a fixed number of values (see
[sorting networks](https://en.wikipedia.org/wiki/Sorting_network) for example)
when possible, and falls back to `std::sort` instead when there is no specialized
algorithm.

`cppsort::sort` takes a random-access range and calls `std::sort` on it. If the
collection is an `std::array` or a fixed-size C array, it calls `sort_n` on it
instead since the size of the collection is known at compile-time. That way, the
library offers a common interface which will either pick `std::sort` or a dedicated
sorting algorithm depending on its input, striving to offer the most efficient
algorithm without any overhead.

Using it should be pretty trivial:

```cpp
#include <array>
#include <iostream>
#include <cpp-sort/sort.h>

int main()
{
    std::array<int, 5u> arr = { 5, 8, 3, 2, 9 };
    cppsort::sort(arr);
    
    // prints 2 3 5 8 9
    for (int val: arr)
    {
        std::cout << val << ' ';
    }
}
```

`cppsort::sort`
---------------

```cpp
template<
    typename RandomAccessIterable,
    typename Compare = std::less<>
>
auto sort(RandomAccessIterable& iterable, Compare&& compare={})
    -> void;

template<
    typename T,
    std::size_t N,
    typename Compare = std::less<>
>
auto sort(std::array<T, N>& array, Compare&& compare={})
    -> void;

template<
    typename T,
    std::size_t N,
    typename Compare = std::less<>
>
auto sort(T (&array)[N], Compare&& compare={})
    -> void;
```

This function takes a `RandomAccessIterable` collection and sorts it in-place in
ascending order. The function uses `std::less<>` to compare the elements, unless
specified otherwise.

The first version is a direct wrapper around `std::sort` while the overloads taking
a fixed-sized C array or an `std::array` call `cppsort::sort_n`. 

`cppsort::sort_n`
-----------------

```cpp
template<
    std::size_t N,
    typename RandomAccessIterator,
    typename Compare = std::less<>
>
auto sort_n(RandomAccessIterator begin, Compare&& compare={})
    -> void;
```

This function takes random-access iterator and proceeds to sort the elements of
the range `[begin, begin + N)` in-place in ascending order. The function uses
`std::less<>` to compare the elements unless specified otherwise.

The generic version of the algorithm is a direct call to the standard library
function `std::sort(begin, begin+N)`. However, the called is dispatched to specific
and more optimal sorting algorithms for some small values of `N`.

