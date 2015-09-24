**cpp-sort** is a generic C++14 header-only sorting library. It revolves
around one main generic sorting interface and provides several small tools
to pick and/or design sorting algorithms. The library's main function,
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

**cpp-sort** also provides *sorters* as well as *sorter adapters* which can be
used by `cppsort::sort` to sort a collection, as well as a sorter base class to
ease the construction of new sorters. It is possible to get some information
about the sorters and sorter adapters thanks to *sorter traits*. Everything lives
in the `cppsort` namespace. You can read more about the available sorting tools
in the documentation:

* [`cppsort::sort`](doc/sort.md)
* [Sorters](doc/sorters.md)
* [Sorter adapters](doc/sorter-adapters.md)
* [Sorter base](doc/sorter-base.md)
* [Sorter traits](doc/sorter-traits.md)

There are also a few other utilities used by the library and made available
to the users, even though they are not explicitly sorting-related. You can
read about them in the following page:

* [Miscellaneous utilities](doc/utilities.md)

While the primary goal of **cpp-sort** is to provide sorting algorithms, it has
also be designed to be extensible. The following tutorials may help you to write
custom sorters and sorter adapters:

* [Writing a sorter](doc/writing-sorter.md)

# Benchmarks

The following graph has been generated with the scripts found in the benchmarks
directory. It shows the time needed for one sorting algorithm to sort one million
shuffled `std::array` of sizes 0 to 15. The benchmark compares `cppsort::sort`
(using the default sorter) to three other sorters available in the library:
an `std_sorter`, a `tim_sorter` and a `pdq_sorter`.

![shuffled](http://i.imgur.com/5U8Cilv.png)

These results were generated with MinGW g++ 5.1 with the compiler options
`-std=c++14 -O3 -march=native`. More benchmarks will be made available in
the future.

# Thanks

**cpp-sort** reuses a great deal of code coming from other projects, often
slightly altered to integrate seamlessly into the library. Here is a list of
resources I used to create this library. I hope that the different licenses
are compatible. If it is not the case, please contact me (or post an issue)
and we will see what can be done about it:

* The algorithms used by `insertion_sorter` and `pdq_sorter` come from Orson
Peters' [pattern-defeating quicksort](https://github.com/orlp/pdqsort). Some
parts of the benchmarks come from there as well.

* The algorithm used by `tim_sorter` comes from Goro Fuji's (gfx) [C++
implementation](https://github.com/gfx/cpp-TimSort) of TimSort.

* The three algorithms used by `spread_sorter` come from Steven Ross [Boost.Sort
module](http://www.boost.org/doc/libs/1_59_0/libs/sort/doc/html/index.html) with
some modifications so that they do no depend on Boost anymore.

* The algorithm used by `verge_sorter` comes from my own [implementation of
a vergesort](https://github.com/Morwenn/vergesort) (at least, I know that the
license for this one is compatible :p).

* The algorithm used to sort three values in-place is derived from the one
[found in libc++](https://github.com/llvm-mirror/libcxx/blob/master/include/algorithm#L3602).

* The other algorithms used by `small_array_adapter` to sort a fixed number of
values have been generated thanks to the `SWAP` macros that can be found on
[this page about sorting networks](http://pages.ripco.net/~jgamble/nw.html)
which itself generates its macros thanks to the Perl module `Algorithm::Networksort`.
