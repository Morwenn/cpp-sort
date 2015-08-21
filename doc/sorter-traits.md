Sorter traits
=============

```cpp
#include <cpp-sort/sorter_traits.h>
```

The class template `sorter_traits<Sorter>` contains information about
sorters and sorter adapters such as the kind of iterators accepted by
a sorter or whether it implements or not a stable sorting algorithm.

`sorter_traits`
---------------

Here is the basic definition of `sorter_traits`:

```cpp
template<typename Sorter>
struct sorter_traits {};
```

This class can be specialized for every sorter object and contains the
following fields:

* `using iterator_category = /* depends on the sorter */;`
* `static constexpr bool is_stable = /* depends on the context */;`

The non-specialized version of `sorter_traits` is defined but empty for
SFINAE friendliness.

`iterator_category`
-------------------

```cpp
template<typename Sorter>
using iterator_category = typename sorter_traits<Sorter>::iterator_category;
```

Some tools needs to know which category of iterators a sorting algorithm
can work with. Therefore, a well-defined sorter provides one of the standard
library [iterator tags](http://en.cppreference.com/w/cpp/iterator/iterator_tags)
in order to document that.

`is_stable`
-----------

```cpp
template<typename Sorter>
constexpr bool is_stable = sorter_traits<Sorter>::is_stable;
```

This variable template contains a boolean value which tells whether a sorting
algorithm is [stable](https://en.wikipedia.org/wiki/Sorting_algorithm#Stability)
or not. This information may be useful in some contexts.
