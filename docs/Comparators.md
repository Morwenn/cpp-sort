While comparators are not inherent to sorting *per se*, most sorting algorithms (at least in this library) are *comparison sorts*, hence providing some common useful comparators along with the sorters can be useful. Every comparator in this module satisfies the [`BinaryPredicate`][binary-predicate] library concept.

Every non-refined comparator described below is also a [transparent comparator][transparent-comparator]. While this ability is not used by the library itself, it means that the comparators can be used with the standard library associative containers to compare heterogeneous objects without having to create temporaries.

*Changed in version 1.5.0:* every non-refined comparator is now a [transparent comparator][transparent-comparator].

### `projection_compare`

```cpp
#include <cpp-sort/comparators/projection_compare.h>
```

The class template `projection_compare` can be used to embed a comparison and a projection in a single comparison object, allowing to provide projection support to algorithms that only support comparisons, such as standard library algorithms prior to C++20. Both the passed comparison and projection functions can be [*Callable*][callable].

It is accompanied by a `make_projection_compare` function template to avoid having to pass the template parameters by hand.

**Example:**

```cpp
// Sort a family from older to younger member
std::vector<Person> family = { /* ... */ };
std::sort(family.begin(), family.end(), cppsort::make_projection_compare(std::greater<>{}, &Person::age));
```

*New in version 1.9.0*

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

*Changed in version 1.5.0:* `total_greater` and `total_less` are respectively of type `total_greater_t` and `total_less_t`.

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

*Changed in version 1.5.0:* `weak_greater` and `weak_less` are respectively of type `weak_greater_t` and `weak_less_t`.

### Partial order comparators

```cpp
#include <cpp-sort/comparators/partial_greater.h>
#include <cpp-sort/comparators/partial_less.h>
```

The comparators `partial_less` and `partial_order` are [customization points][custom-point] implementing a [partial order][partial-order], inspired by the similar functions described in [P0100][P0100]. The provided functions handle built-in integer out of the box (falling back to `weak_*` functions) and implement a partial order for floating point numbers using the built-in relational operators.

When it doesn't handle a type natively and ADL doesn't find any suitable `partial_less` function in a class namespace, `cppsort::partial_less` falls back to `cppsort::weak_less` since a weak order is also a partial order (it applies to the whole `partial_*` family of customization points).

Partial order comparators are considered as [generating branchless code][branchless-traits] when comparing instances of a type that satisfies [`std::is_arithmetic`][std-is-arithmetic].

*Changed in version 1.5.0:* `partial_greater` and `partial_less` are respectively of type `partial_greater_t` and `partial_less_t`.

### Natural order comparator

```cpp
#include <cpp-sort/comparators/natural_less.h>
```

The comparator `natural_less` is a [customization point][custom-point] that can be used to perform a [natural sort][natural-sort]. The function handles any two forward iterable sequences of `char` out of the box using [`std::isdigit`][std-is-digit] to identify digits (which includes `std::string`, `std::vector<char>` and `char[]`). Other character types and locales are currently not handled and it is unlikely that the library will evolve more than switching to `<locale>`'s `std::isdigit` instead of `<cctype>`'s one.

*Changed in version 1.5.0:* `natural_less` can compare heterogeneous types as long as they provide `begin` and `end` functions returning iterators to a sequence of `char`.

*Changed in version 1.5.0:* `natural_less` is an instance of type `natural_less_t`.

### Case-insensitive comparator

```cpp
#include <cpp-sort/comparators/case_insensitive_less.h>
```

The comparator `case_insensitive_less` is a [customization point][custom-point] that be used to perform a [case-insensitive][case-sensitivity] sort on collections. The function handles any forward iterable sequence of `char` or `wchar_t` out of the box using [`std::ctype::tolower`][to-lower] prior to character comparison. The customization point has two distinct signatures:

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

*Changed in version 1.5.0:* `case_insensitive_less` is an instance of type `case_insensitive_less_t`.


  [binary-predicate]: https://en.cppreference.com/w/cpp/concept/BinaryPredicate
  [branchless-traits]https://github.com/Morwenn/cpp-sort/wiki/Miscellaneous-utilities#branchless-traits
  [callable]: https://en.cppreference.com/w/cpp/named_req/Callable
  [case-sensitivity]: https://en.wikipedia.org/wiki/Case_sensitivity
  [cppcon2015-compare]: https://github.com/CppCon/CppCon2015/tree/master/Presentations/Comparison%20is%20not%20simple%2C%20but%20it%20can%20be%20simpler%20-%20Lawrence%20Crowl%20-%20CppCon%202015
  [custom-point]: https://ericniebler.com/2014/10/21/customization-point-design-in-c11-and-beyond/
  [natural-sort]: https://en.wikipedia.org/wiki/Natural_sort_order
  [P0100]: http://open-std.org/JTC1/SC22/WG21/docs/papers/2015/p0100r1.html
  [partial-order]: https://en.wikipedia.org/wiki/Partially_ordered_set#Formal_definition
  [refining]: https://github.com/Morwenn/cpp-sort/wiki/Refined-functions
  [std-is-arithmetic]: https://en.cppreference.com/w/cpp/types/is_arithmetic
  [std-is-digit]: https://en.cppreference.com/w/cpp/string/byte/isdigit
  [std-is-integral]: https://en.cppreference.com/w/cpp/types/is_integral
  [std-locale]: https://en.cppreference.com/w/cpp/locale/locale
  [to-lower]: https://en.cppreference.com/w/cpp/locale/ctype/tolower
  [total-order]: https://en.wikipedia.org/wiki/Total_order
  [transparent-comparator]: https://stackoverflow.com/q/20317413/1364752
  [weak-order]: https://en.wikipedia.org/wiki/Weak_ordering
