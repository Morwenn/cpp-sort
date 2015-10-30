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

**cpp-sort** also provides *sorters* and *fixed-size sorters* as well as *sorter
adapters* which can be used by `cppsort::sort` to sort a collection, as well as a
sorter facade to ease the construction of new sorters. It is possible to get some
information about the sorters and sorter adapters thanks to *sorter traits*. Every
class or function lives in the `cppsort` namespace. You can read more about the
available sorting tools in the documentation:

* [`cppsort::sort`](doc/sort.md)
* [Sorters](doc/sorters.md)
* [Fixed-size sorters](doc/fixed-sorters.md)
* [Sorter adapters](doc/sorter-adapters.md)
* [Sorter facade](doc/sorter-facade.md)
* [Sorter traits](doc/sorter-traits.md)

There are also a few other utilities used by the library and made available
to the users, even though they are not explicitly sorting-related. You can
read about them in the following page:

* [Miscellaneous utilities](doc/utilities.md)

While the primary goal of **cpp-sort** is to provide sorting algorithms, it has
also be designed to be extensible. The following tutorials may help you to write
custom sorters and sorter adapters:

* [Writing a sorter](doc/writing-sorter.md)
* [Writing a bubble sorter](doc/writing-bubble-sorter.md)

# Benchmarks

The following graph has been generated with a script found in the benchmarks
directory. It shows the time needed for one sorting algorithm to sort one million
shuffled `std::array<int, N>` of sizes 0 to 15. It compares the sorters generally
used to sort small arrays:

![shuffled int arrays](https://i.imgur.com/GaRHn9x.png)

These results were generated with MinGW g++ 5.2 with the compiler options
`-std=c++14 -O3 -fexpensive-optimizations -march=native`. More benchmarks will be
made available in the future.

# Thanks

Even though some parts of the library are [original research](doc/research.md) and
some others correspond to custom and unoriginal implementations of standard sorting
algorithms, **cpp-sort** also reuses a great deal of code from other open-source
projects, often slightly altered to integrate seamlessly into the library. Here is
a list of the external resources used to create this library. I hope that the many
different licenses are compatible. If it is not the case, please contact me (or
submit an issue) and we will see what can be done about it:

* Some of the algorithms used by `insertion_sorter` and `pdq_sorter` come from
Orson Peters' [pattern-defeating quicksort](https://github.com/orlp/pdqsort). Some
parts of the benchmarks come from there as well.

* The algorithm used by `tim_sorter` comes from Goro Fuji's (gfx) [implementation
of a Timsort](https://github.com/gfx/cpp-TimSort).

* The three algorithms used by `spread_sorter` come from Steven Ross [Boost.Sort
module](http://www.boost.org/doc/libs/1_59_0/libs/sort/doc/html/index.html) with
some modifications so that they do not depend on Boost anymore.

* The merge insertion sort used by `low_comparison_sorter` algorithm 5 is a C++
adaptation of a LISP implementation of the algorithm that I found in [an answer
on StackOverflow](http://stackoverflow.com/a/1935491/1364752).

* The algorithms 17 to 22 used by `sorting_network_sorter` correspond to the ones
found by Symmetry and Evolution based Network Sort Optimization (SENSO) publihed in
*Using Symmetry and Evolutionary Search to Minimize Sorting Networks* by Valsalam
and Miikkulainen.

* The algorithms 0 to 16 used by `sorting_network_sorter` have been generated with
Perl's [`Algorithm::Networksort` module](http://search.cpan.org/~jgamble/Algorithm-Networksort-1.30/lib/Algorithm/Networksort.pm).

* Some of the optimizations used by `sorting_network_sorter` come from [this
discussion](http://stackoverflow.com/q/2786899/1364752) on StackOverflow and are
backed by the article [*Applying Sorting Networks to Synthesize Optimized Sorting
Libraries*](http://arxiv.org/abs/1505.01962).

* The LaTeX scripts used to draw the sorting networks are modified versions of
kaayy's [`sortingnetwork.tex`](https://github.com/kaayy/kaayy-s-code-sinppets),
slightly adapted to be 0-based and draw the network from top to bottom.
