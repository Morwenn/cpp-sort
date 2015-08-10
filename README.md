**cpp-sort** is a generic C++14 header-only sorting library. It revolves
around one main generic sorting interface and provides several small tools
to pick and/or design sorting algorithms. The main library's main function,
`cppsort::sort`, is available with the following include:

```cpp
#include <cpp-sort/sort.h>
```

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

**cpp-sort** library also provides *sorters* as well as *sorter adapters*
which can be used by `cppsort::sort` to sort a collection. Everything lives
in the `cppsort` namespace. You can read more about the available sorting
tools in the documentation:

* [`cppsort::sort`](doc/sort.md)
* [Sorters](doc/sorters.md)
* [Sorter adapters](doc/sorter-adapters.md)

There are also a few other utilities used by the library and made available
to the users, even though they're not sorting-related. You can read about them
in the following page:

* [Miscellaneous utilities](doc/utilities.md)

Benchmarks
==========

The following graph has been generated with the scripts found in the benchmarks
directory. It shows the time needed for one sorting algorithm to sort one million
shuffled `std::array` of sizes 0 to 15. The benchmarks compare `cppsort::sort`
(using the default sorter) to three other sorters available in the library:
`std::sort`, a Timsort and a pattern-defeating quicksort.

![shuffled](http://i.imgur.com/5U8Cilv.png)

These results were generated with MinGW g++ 5.1 with the compiler options
`-std=c++14 -O3`. More benchmarks will be made available in the future.
