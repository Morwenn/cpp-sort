[![Latest Release](https://img.shields.io/badge/release-cpp--sort%2F1.1.0-blue.svg)](https://github.com/Morwenn/cpp-sort/releases)
[![Build Status](https://travis-ci.org/Morwenn/cpp-sort.svg?branch=master)](https://travis-ci.org/Morwenn/cpp-sort)
[![License](http://img.shields.io/:license-mit-blue.svg)](http://doge.mit-license.org)
[![Code Coverage](https://codecov.io/gh/Morwenn/cpp-sort/branch/master/graph/badge.svg)](https://codecov.io/gh/Morwenn/cpp-sort)

> *It would be nice if only one or two of the sorting methods would dominate all of the others,
> regardless of application or the computer being used. But in fact, each method has its own
> peculiar virtues. [...] Thus we find that nearly all of the algorithms deserve to be remembered,
> since there are some applications in which they turn out to be best.*
> — Donald Knuth, The Art Of Computer Programming, Volume 3

**cpp-sort** is a generic C++14 header-only sorting library. It revolves
around one main generic sorting interface and provides several small tools
to pick and/or design sorting algorithms. Using its basic sorting features
should be trivial enough:

```cpp
#include <array>
#include <iostream>
#include <cpp-sort/sort.h>

int main()
{
    std::array<int, 5u> arr = { 5, 8, 3, 2, 9 };
    cppsort::sort(arr);
    
    // prints 2 3 5 8 9
    for (int val: arr) {
        std::cout << val << ' ';
    }
}
```

# The main features & the extra features

**cpp-sort** actually provides a full set of sorting-related features. Here are the main
building blocks of the library:
* Every sorting algorithm exists as a function object called a [sorter](https://github.com/Morwenn/cpp-sort/wiki/Sorters)
* Sorters can be wrapped in [sorter adapters](https://github.com/Morwenn/cpp-sort/wiki/Sorter-adapters) to augment their behaviour
* The library provides a [sorter facade](https://github.com/Morwenn/cpp-sort/wiki/Sorter-facade) to easily build sorters
* [Fixed-size sorters](https://github.com/Morwenn/cpp-sort/wiki/Fixed-size-sorters) can be used to efficiently sort tiny fixed-size collections
* [Measures of presortedness](https://github.com/Morwenn/cpp-sort/wiki/Measures-of-presortedness) can be used to evaluate the disorder in a collection

Here is a more complete example of what can be done with the library:

```cpp
#include <algorithm>
#include <cassert>
#include <forward_list>
#include <functional>
#include <iterator>
#include <vector>
#include <cpp-sort/adapters.h>
#include <cpp-sort/sort.h>
#include <cpp-sort/sorters.h>

int main()
{
    struct wrapper { int value; };

    std::forward_list<wrapper> li = { {5}, {8}, {3}, {2}, {9} };
    std::vector<wrapper> vec = { {5}, {8}, {3}, {2}, {9} };

    // When used, this sorter will use a pattern-defeating quicksort
    // to sort random-access collections, and a mergesort otherwise
    using sorter = cppsort::hybrid_adapter<
        cppsort::pdq_sorter,
        cppsort::merge_sorter
    >;
    sorter sort;

    // Sort li and vec in reverse order using their value member
    sort(li, std::greater<>{}, &wrapper::value);
    sort(vec, std::greater<>{}, &wrapper::value);

    assert(std::equal(
        std::begin(li), std::end(li),
        std::begin(vec), std::end(vec),
        [](auto& lhs, auto& rhs) { return lhs.value == rhs.value; }
    ));
}
```

Even when the sorting functions are used without the extra features, they still provide
some interesting guarantees (ideas often taken from the Ranges TS):
* They provide both an iterator and a range interface
* When possible, they accept a custom comparator parameter
* Most of them accept a projection parameter
* They correctly handle proxy iterators with `iter_swap` and `iter_move`
* They also work when iterators don't provide post-incrementation nor post-decrementation
* The value types of the collections to be sorted need not be default-constructible
* The value types of the collections to be sorted need not be copyable (only movable)
* Stateless sorters can be converted to a function pointer for each overloaded `operator()`
* Sorters are function objects: they can directly be passed as "overload sets" to other functions

You can read more about all the available tools and find some tutorials about using
and extending **cpp-sort** in [the wiki](https://github.com/Morwenn/cpp-sort/wiki).

# Benchmarks

The following graph has been generated with a script found in the benchmarks
directory. It shows the time needed for a sorting algorithm to sort one million
shuffled `std::array<int, N>` of sizes 0 to 15. It compares the sorters generally
used to sort small arrays:

![small shuffled int arrays](https://i.imgur.com/mpV0Qur.png)

These results were generated with MinGW g++ 6.1.0 with the compiler options
`-std=c++1z -O2 -march=native`. That benchmark is just an example to make this
introduction look good. You can find more commented benchmarks in the [dedicated
wiki page](https://github.com/Morwenn/cpp-sort/wiki/Benchmarks).

# Compiler support

**cpp-sort** currently requires C++14 support, and only works with g++5 and clang++3.8
or more recent versions of these compilers. So far, the build matrix + home tests give
the following supports:
* Works on Linux with both g++5 and clang++3.8
* Works on OSX with Xcode 8
* Works on Windows with MinGW-w64 g++5
* Works with both libstdc++ and libc++
* Passes Valgrind checks on Linux and OSX, with every tested compiler
* Passes undefined sanitizer checks on Linux with both g++ and clang++
* Passes address sanitizers checks on Linux with clang++

Last time I tried it did not work with MSVC 2017. Future development on the C++14 branch
will try to remain compatible with the compiler versions listed above. Some sanitizer tests
are not listed above, not because they trigger error, but because I didn't manage to make
them build properly.

The repository also contains an experimental [C++17 branch](https://github.com/Morwenn/cpp-sort/tree/c++17)
which requires the most recent versions of g++ and clang++, and will probably require even
more recent versions until the C++17 support of both compilers is stable enough. It is
worth noting that code written against the C++14 branch is not guaranteed to work with the
C++17 branch as the new language and standard library features replaced some of the utility
headers; those changes [are documented](https://github.com/Morwenn/cpp-sort/wiki/Changelog).
The C++17 branch already has more features than the C++14 one, and will at some point have
significant features that the C++14 branch won't have, such as proper handling of execution
policies to implement parallel sorting algorithms. At some later point in the future, the
C++17 branch will likely become the main branch, and the C++14 branch will only receive bug
fixes and smallish updates.

The long-term goal is to make the library evolve with the C++ standard, and the kind of
differences that already exist between the C++14 and C++17 branches will also exist between
the future branches. Some features such as concepts and standard ranges will likely shape the
future of the library.

# Thanks

> *I got a new car. I just need to put it together. They’re easier to steal piece by
> piece.*
> — Jarod Kintz, $3.33

Even though some parts of the library are [original research](https://github.com/Morwenn/cpp-sort/wiki/Original-research)
and some others correspond to custom and rather naive implementations of standard
sorting algorithms, **cpp-sort** also reuses a great deal of code and ideas from
open-source projects, often altered to integrate seamlessly into the library. Here
is a list of the external resources used to create this library. I hope that the
many different licenses are compatible. If it is not the case, please contact me
(or submit an issue) and we will see what can be done about it:

* Some of the algorithms used by `insertion_sorter` and `pdq_sorter` come from
Orson Peters' [pattern-defeating quicksort](https://github.com/orlp/pdqsort). Some
parts of the benchmarks come from there as well.

* The algorithm used by `tim_sorter` comes from Goro Fuji's (gfx) [implementation
of a Timsort](https://github.com/gfx/cpp-TimSort).

* The three algorithms used by `spread_sorter` come from Steven Ross [Boost.Sort
module](http://www.boost.org/doc/libs/1_59_0/libs/sort/doc/html/index.html).

* [`utility::as_function`](https://github.com/Morwenn/cpp-sort/wiki/Miscellaneous-utilities#as_function),
[`utility::static_const`](https://github.com/Morwenn/cpp-sort/wiki/Miscellaneous-utilities#static_const),
and several projection-enhanced helper algorithms come from Eric Niebler's [Range
v3](https://github.com/ericniebler/range-v3) library. Several ideas such as proxy
iterators, customization points and projections, as well as a few other utility
functions also come from that library or from the related articles and standard
C++ proposals.

* The algorithm used by `ska_sorter` comes from Malte Skarupke's [implementation](https://github.com/skarupke/ska_sort)
of his own [ska_sort](https://probablydance.com/2016/12/27/i-wrote-a-faster-sorting-algorithm/) algorithm.

* The algorithm used by `drop_merge_sorter` comes from Adrian Wielgosik [C++
reimplementation](https://github.com/adrian17/cpp-drop-merge-sort) of Emil Ernerfeldt's
[drop-merge sort](https://github.com/emilk/drop-merge-sort).

* Many enhanced standard algorithms are directly adapted from their counterparts
in [libc++](http://libcxx.llvm.org/), enhanced to handle both projections and
proxy iterators.

* The library internally uses an `inplace_merge` function that works with forward
iterators. Its implementation uses a merge algorithm proposed by Dudziński and Dydek,
and implemented by Alexander Stepanov and Paul McJones in their book [*Elements of
Programming*](http://www.elementsofprogramming.com/).

* The implementation of Dijkstra's smoothsort used by `smooth_sorter` has been
directly adapted from [Keith Schwarz's implementation](http://www.keithschwarz.com/interesting/code/?dir=smoothsort)
of the algorithm.

* The algorithm used by `block_sorter` has been adapted from BonzaiThePenguin's
[WikiSort](https://github.com/BonzaiThePenguin/WikiSort).

* The algorithm used by `grail_sorter` has been adapted from Mrrl's
[GrailSort](https://github.com/Mrrl/GrailSort), hence the name.

* The algorithms 0 to 16 used by `sorting_network_sorter` have been generated with
Perl's [`Algorithm::Networksort` module](http://search.cpan.org/~jgamble/Algorithm-Networksort-1.30/lib/Algorithm/Networksort.pm).

* The algorithms 17 and 18 used by `sorting_network_sorter` correspond to the ones found
by Symmetry and Evolution based Network Sort Optimization (SENSO) published in *Using
Symmetry and Evolutionary Search to Minimize Sorting Networks* by Valsalam and Miikkulainen.

* The algorithms 19, 20, 21, 22, 23, 24 and 28 used by `sorting_network_sorter` have
been found and proposed for inclusion by Bert Dobbelaere with his [SorterHunter project](https://github.com/bertdobbelaere/SorterHunter).
Huge thanks for this contribution :) You can find a full list of most well-known sorting
networks up to 32 inputs on his website.

* Some of the optimizations used by `sorting_network_sorter` come from [this
discussion](http://stackoverflow.com/q/2786899/1364752) on StackOverflow and are
backed by the article [*Applying Sorting Networks to Synthesize Optimized Sorting
Libraries*](http://arxiv.org/abs/1505.01962).

* The LaTeX scripts used to draw the sorting networks are modified versions of
kaayy's [`sortingnetwork.tex`](https://github.com/kaayy/kaayy-s-code-sinppets),
slightly adapted to be 0-based and draw the network from top to bottom.
