Most sorting algorithms in **cpp-sort** accept comparison and/or projection parameters. The library therefore considers these kinds of functions to be first-class citizens too and provides dedicated comparators, projections and tools to combine them and to solve common related problems.

All the functions and classes in **cpp-sort** that take comparison or projection functions as parameters expect [*Callable*][callable] parameters, which correspond to anything that can be used as the first parameter of [`std::invoke`](http://en.cppreference.com/w/cpp/utility/functional/invoke). This allows to pass entities such as pointers to members or pointer to member functions to the sorting algorithms; it should work out-of-the-box without any wrapping needed on the user side.

### Related utilities

Several of the [miscellaneous utilities][utilities] provided by the library are meant to interact with comparators and projections:
- [`as_comparison` and `as_projection`](https://github.com/Morwenn/cpp-sort/wiki/Miscellaneous-utilities#as_comparison-and-as_projection) are used to make it explicit whether an ambiguous function object should be used for comparison or for projection.
- [`as_function`](https://github.com/Morwenn/cpp-sort/wiki/Miscellaneous-utilities#as_function) can be used to turn any [*Callable*][callable] into an object invokable with regular parentheses.
- [`is_probably_branchless_comparison` and `is_probably_branchless_projection`](https://github.com/Morwenn/cpp-sort/wiki/Miscellaneous-utilities#branchless-traits) are type traits that can be used to mark whether functions are likely to be branchless when called with a specific type.
- [`identity`](https://github.com/Morwenn/cpp-sort/wiki/Miscellaneous-utilities#miscellaneous-function-objects) is the default projection returning the argument it is passed without modifying it.

### LWG3031

Sorters and adapters in the library accept comparators taking their parameters by non-`const` reference, and should work as expected as long as comparators do not actually modify their parameters in a way that affects the sort consistency. This is mostly meant to support legacy comparators, but it also covers more unusual use cases such as when a comparator needs to update the compared objects to store additional information (preferably fields that do not affect the result of the comparison).

This additional guarantee is allowed by the resolution of [LWG3031][lwg3031]. However when a comparator can take its parameters by both `const` and non-`const` reference, it is required to return consistent results no matter which overload is used (see the LWG issue for an example of inconsistent results).

*New in version 1.7.0*


  [callable]: https://en.cppreference.com/w/cpp/named_req/Callable
  [lwg3031]: https://wg21.link/LWG3031
  [utilities]: https://github.com/Morwenn/cpp-sort/wiki/Miscellaneous-utilities
