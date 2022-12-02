The comparators described below can be used as needed with [*sorters*][sorters] and [*sorter adapters*][sorter-adapters] that accept comparisons. Every comparator in this module satisfies the [`BinaryPredicate`][binary-predicate] library concept.

Every non-refined comparator described below is also a [transparent comparator][transparent-func]. While this ability is not used by the library itself, it means that the comparators can be used with the standard library associative containers to compare heterogeneous objects without having to create temporaries.

### Total order comparators

```cpp
#include <cpp-sort/comparators/total_greater.h>
#include <cpp-sort/comparators/total_less.h>
```

The comparators `total_less` and `total_order` are [customization points][custom-point] implementing a [total order][total-order], inspired by the similar functions described in [P0100][P0100]. The provided functions handle built-in integer out of the box (using the built-in relational operators) and implement IEEE 754's `totalOrder` for floating point numbers (from lesser to greater):
* positive quiet NaNs
* positive signaling NaNs
* positive infinity
* positive reals
* positive zero
* negative zero
* negative reals
* negative infinity
* negative signaling NaNs
* negative quiet NaNs

That said, the comparators are currently unable to discriminate between quiet and signaling NaNs, so they compare equivalent. When it doesn't handle a type natively and ADL doesn't find any suitable `total_less` function in a class namespace, `cppsort::total_less` does *not* fall back to `operator<`; see [P0100][P0100] for the rationale (it applies to the whole `total_*` family of customization points).

Total order comparators are considered as [generating branchless code][branchless-traits] when comparing instances of a type that satisfies [`std::is_integral`][std-is-integral].

### Weak order comparators

```cpp
#include <cpp-sort/comparators/weak_greater.h>
#include <cpp-sort/comparators/weak_less.h>
```

The comparators `weak_less` and `weak_order` are [customization points][custom-point] implementing a [weak order][weak-order], inspired by the similar functions described in [P0100][P0100]. The provided functions handle built-in integer out of the box (falling back to `total_*` functions) and implement a weak order for floating point numbers as suggested by Lawrence Crowl in his [CppCon 2015 presentation][cppcon2015-compare] (from lesser to greater):
* all positive NaNs equivalent
* positive infinity
* positive reals
* all zeros equivalent
* negative reals
* negative infinity
* all negative NaNs equivalent

When it doesn't handle a type natively and ADL doesn't find any suitable `weak_less` function in a class namespace, `cppsort::weak_less` falls back to `cppsort::total_less` since a total order is also a weak order (it applies to the whole `weak_*` family of customization points).

Weak order comparators are considered as [generating branchless code][branchless-traits] when comparing instances of a type that satisfies [`std::is_integral`][std-is-integral].

### Partial order comparators

```cpp
#include <cpp-sort/comparators/partial_greater.h>
#include <cpp-sort/comparators/partial_less.h>
```

The comparators `partial_less` and `partial_order` are [customization points][custom-point] implementing a [partial order][partial-order], inspired by the similar functions described in [P0100][P0100]. The provided functions handle built-in integer out of the box (falling back to `weak_*` functions) and implement a partial order for floating point numbers using the built-in relational operators.

When it doesn't handle a type natively and ADL doesn't find any suitable `partial_less` function in a class namespace, `cppsort::partial_less` falls back to `cppsort::weak_less` since a weak order is also a partial order (it applies to the whole `partial_*` family of customization points).

Partial order comparators are considered as [generating branchless code][branchless-traits] when comparing instances of a type that satisfies [`std::is_arithmetic`][std-is-arithmetic].

### Natural order comparator

```cpp
#include <cpp-sort/comparators/natural_less.h>
```

The comparator `natural_less` is a [customization point][custom-point] that can be used to perform a [natural sort][natural-sort]. The function handles any two forward ranges of `char` out of the box using [`std::isdigit`][std-is-digit] to identify digits (which includes `std::string`, `std::vector<char>` and `char[]`). Other character types and locales are currently not handled and it is unlikely that the library will evolve more than switching to `<locale>`'s `std::isdigit` instead of `<cctype>`'s one.

### Case-insensitive comparator

```cpp
#include <cpp-sort/comparators/case_insensitive_less.h>
```

The comparator `case_insensitive_less` is a [customization point][custom-point] that be used to perform a [case-insensitive][case-sensitivity] sort on collections. The function handles any forward range of `char` or `wchar_t` out of the box using [`std::ctype::tolower`][to-lower] prior to character comparison. The customization point has two distinct signatures:

```cpp
template<typename T>
bool case_insensitive_less(const T& lhs, const T& rhs);

template<typename T>
bool case_insensitive_less(const T& lhs, const T& rhs, const std::locale& loc);
```

`case_insensitive_less` can indeed be passed an [`std::locale`][std-locale] that will be used to perform the `tolower` operation. If it's not passed a locale, it will use the global locale instead:

```cpp
// Use global locale
sort(sequence, case_insensitive_less);

// Use classic C locale
sort(sequence, case_insensitive_less(std::locale::classic()));
```

The two-parameter version of the customization point calls the three-parameter one with `std::locale::global()` as the third argument, so customizing only the three-parameter version will also extend the customization to the two-parameter one.

*This comparator can be [refined][refining] for a specific type to provide better performance.*


  [binary-predicate]: https://en.cppreference.com/w/cpp/concept/BinaryPredicate
  [branchless-traits]: Miscellaneous-utilities.md#branchless-traits
  [callable]: https://en.cppreference.com/w/cpp/named_req/Callable
  [case-sensitivity]: https://en.wikipedia.org/wiki/Case_sensitivity
  [cppcon2015-compare]: https://github.com/CppCon/CppCon2015/tree/master/Presentations/Comparison%20is%20not%20simple%2C%20but%20it%20can%20be%20simpler%20-%20Lawrence%20Crowl%20-%20CppCon%202015
  [custom-point]: https://ericniebler.com/2014/10/21/customization-point-design-in-c11-and-beyond/
  [natural-sort]: https://en.wikipedia.org/wiki/Natural_sort_order
  [P0100]: http://open-std.org/JTC1/SC22/WG21/docs/papers/2015/p0100r1.html
  [partial-order]: https://en.wikipedia.org/wiki/Partially_ordered_set#Formal_definition
  [refining]: Refined-functions.md
  [sorter-adapters]: Sorter-adapters.md
  [sorters]: Sorters.md
  [std-is-arithmetic]: https://en.cppreference.com/w/cpp/types/is_arithmetic
  [std-is-digit]: https://en.cppreference.com/w/cpp/string/byte/isdigit
  [std-is-integral]: https://en.cppreference.com/w/cpp/types/is_integral
  [std-locale]: https://en.cppreference.com/w/cpp/locale/locale
  [to-lower]: https://en.cppreference.com/w/cpp/locale/ctype/tolower
  [total-order]: https://en.wikipedia.org/wiki/Total_order
  [transparent-func]: Comparators-and-projections.md#Transparent-function-objects
  [weak-order]: https://en.wikipedia.org/wiki/Weak_ordering
