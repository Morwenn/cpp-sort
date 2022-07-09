Most sorting algorithms in **cpp-sort** accept comparison and/or projection parameters. The library therefore considers these kinds of functions to be first-class citizens too and provides dedicated [comparators][comparators], [comparator adapters][comparator-adapters] projections and tools to combine them and to solve common problems in the domain.

All the functions and classes in **cpp-sort** that take comparison or projection functions as parameters expect [*Callable*][callable] parameters, which correspond to anything that can be used as the first parameter of [`std::invoke`][std-invoke]. This allows to pass entities such as pointers to members or pointer to member functions to the sorting algorithms; it should work out-of-the-box without any wrapping needed on the user side.

### Related utilities

Several of the [miscellaneous utilities][utilities] provided by the library are meant to interact with comparators and projections:
- [`as_comparison` and `as_projection`][as-comparison-as-projection] are used to make it explicit whether an ambiguous function object should be used for comparison or for projection.
- [`as_function`][as-function] can be used to turn any [*Callable*][callable] into an object invokable with regular parentheses.
- [`is_probably_branchless_comparison` and `is_probably_branchless_projection`][branchless-traits] are type traits that can be used to mark whether functions are likely to be branchless when called with a specific type.

### Transparent function objects

When possible, comparison and projection objects available in **cpp-sort** are *transparent function objects*. This means that:
* They accept arguments of arbitrary types (generally via forwarding references).
* They perfectly forward their arguments to avoid unnecessary copy operations or conversions.
* They have a public member type named `is_transparent` (which generally aliases `void` for convenience, but the exact type is irrelevant).

The concept of transparent function objects was introduced in C++14 for [`std::less<>`][std-less-void] and friends as an optimization mechanism. Nothing in this library actively cares about whether a function object is transparent or not, but some standard library components do: for example [`std::set::find`][std-set-find] and [`std::set::lower_bound`][std-set-lower-bound] use this information to avoid needless conversions of the parameters to their key type.

### LWG3031

Sorters and adapters in the library accept comparators taking their parameters by non-`const` reference, and should work as expected as long as comparators do not actually modify their parameters in a way that affects the sort consistency. This is mostly meant to support legacy comparators, but it also covers more unusual use cases such as when a comparator needs to update the compared objects to store additional information (preferably fields that do not affect the result of the comparison).

This additional guarantee is allowed by the resolution of [LWG3031][lwg3031]. However when a comparator can take its parameters by both `const` and non-`const` reference, it is required to return consistent results no matter which overload is used (see the LWG issue for an example of inconsistent results).


  [as-comparison-as-projection]: Miscellaneous-utilities.md#as_comparison-and-as_projection
  [as-function]: Miscellaneous-utilities.md#as_function
  [branchless-traits]: Miscellaneous-utilities.md#branchless-traits
  [callable]: https://en.cppreference.com/w/cpp/named_req/Callable
  [comparator-adapters]: Comparator-adapters.md
  [comparators]: Comparators.md
  [lwg3031]: https://wg21.link/LWG3031
  [std-invoke]: https://en.cppreference.com/w/cpp/utility/functional/invoke
  [std-less-void]: https://en.cppreference.com/w/cpp/utility/functional/less_void
  [std-set-find]: https://en.cppreference.com/w/cpp/container/set/find
  [std-set-lower-bound]: https://en.cppreference.com/w/cpp/container/set/lower_bound
  [utilities]: Miscellaneous-utilities.md
