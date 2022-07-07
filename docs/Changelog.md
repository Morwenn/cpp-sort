This page describes the features that change in **cpp-sort** depending on the C++ version with which it is compiled (C++14 or later) as well as the support for miscellaneous compiler extensions; for a full changelog between actual releases, you can check the dedicated [releases page][cpp-sort-releases].

## C++14 features

While **cpp-sort** theoretically requires a fully C++14-compliant compiler, a few standard features are either not available or deactivated in popular compilers and the library tries to take those into account if possible.

**Performance improvements:**
* Sized deallocation: this C++14 feature is not always available (Clang requires `-fsized-deallocation` for example) and standard allocation functions typically don't take advantage of it. However, if `__cpp_sized_deallocation` is defined and the global deallocations functions are replaced with overloads that take advantage of sized deallocation, then several sorters will explicitly try to take advantage of it.

## C++17 features

When compiled with C++17, **cpp-sort** might gain a few additional features depending on the level of C++17 support provided by the compiler. The availability of most of the features depends on the presence of corresponding [feature-testing macros][feature-test-macros]. The support for feature-testing macros being optional in C++17, it is possible that some of the features listed below aren't available even though the compiler is implements them. If it is the case and it is a problem for you, don't hesitate to open an issue so that we can explicitly support the given compiler.

**New features:**
* `string_spread_sort` now accepts [`std::string_view`][std-string-view] and sometimes `std::wstring_view`.

    This feature is made available through the check `__cplusplus > 201402L && __has_include(<string_view>)`.

* Sorter adapters have been updated to take advantage of deduction guides:

    ```cpp
    // C++14
    constexpr auto sort = schwartz_adapter<quick_sorter>{};
    // C++17
    constexpr auto sort = schwartz_adapter(quick_sort);
    ```

    This notably makes measures of presortedness more usable with the few sorter adapters that make sense for them:

    ```cpp
    // C++14
    auto rem = indirect_adapter<decltype(probe::rem)>{};
    // C++17
    auto rem = indirect_adapter(probe::rem);
    ```

    There is no specific check for this feature: the sorter adpater constructors have been written in such a way that implicit deduction guides work out-of-the-box.

* `indirect_adapter` and `out_of_place_adapter` return the result returned by the *adapter sorter*.

    This feature is made available through the check `__cpp_lib_uncaught_exceptions`.

* New [`function_constant`][cpp-sort-function-objects] utility to micro-optimize function pointers and class member pointers.

    ```cpp
    insertion_sort(collection, function_constant<&foo::bar>{});
    ```

    It sometimes results in fewer indirections than a raw `&foo::bar`, and can be subject to *empty base class optimization* when stored.

    This feature is available when the feature-testing macro `__cpp_nontype_template_parameter_auto` is defined.

* [`sorter_facade`][sorter-facade] range overloads can now be used in `constexpr` functions.

    There is no specific feature macro available to test this, it starts working when `std::begin` and `std::end` are `constexpr`.

## C++20 features

When compiled with C++20, **cpp-sort** might gain a few additional features depending on the level of C++20 support provided by the compiler. The availability of those features depends on the presence of corresponding [feature-testing macros][feature-test-macros] when possible, even though some checks are more granular. Don't hesitate to open an issue if your compiler and standard library supports one of those features but it doesn't seem to work in **cpp-sort**.

**New features:**
* When available, [`std::ranges::less`][std-ranges-less] and [`std::ranges::greater`][std-ranges-greater] benefit from dedicated support wherever [`std::less<>`][std-less-void] and [`std::greater<>`][std-greater-void] are supported, with equivalent semantics.

* [`utility::iter_swap`][utility-iter-move] can now be used in more `constexpr` functions thanks to [`std::swap`][std-swap] begin `constexpr`.

    The feature-test macro `__cpp_lib_constexpr_algorithms` can be used to check whether `std::swap` is `constexpr`.

## Other features

**cpp-sort** tries to take advantage of more than just standard features when possible by using implementation-specific tweaks to improve the user experience. The following improvements might be available depending on the your standard implementation:

**Additional features:**
* 128-bit integers support: [`counting_sorter`][counting-sorter] and [`ska_sorter`][ska-sorter] have dedicated support for 128-bit integers (`unsigned __int128` or `__uint128_t` and its signed counterpart), no matter whether the standard library is also instrumented for those types. This support should be available as long as the macro `__SIZEOF_INT128__` is defined.

**Performance improvements:**
* Bit manipulation intrinsics: there are a few places where bit tricks are used to perform a few operations faster. Some of those operations are made faster with bitwise manipulation intrinsics when those are available.

* Assumptions: some algorithms use assumptions in select places to make the compiler generate more efficient code. Whether such assumptions are available depends on the compiler.

* Vectorized algorithms: when compiled against the Microsoft STL, **cpp-sort** tries to take advantage of their vectorized algorithms when possible. This improves some algorithms when sorting contiguous collections of trivially copyable types.

* When using libstdc++, libc++ or the Microsoft STL, the return type of [`std::mem_fn`][std-mem-fn] is considered ["probably branchless"][branchless-traits] when it wraps a pointer to data member, which can improve the speed of [`pdq_sorter`][pdq-sorter] and everything that relies on it in some scenarios.


  [branchless-traits]: Miscellaneous-utilities.md#branchless-traits
  [counting-sorter]: Sorters.md#counting_sorter
  [cpp-sort-function-objects]: Miscellaneous-utilities.md#miscellaneous-function-objects
  [cpp-sort-releases]: https://github.com/Morwenn/cpp-sort/releases
  [feature-test-macros]: https://wg21.link/SD6
  [pdq-sorter]: Sorters.md#pdq_sorter
  [ska-sorter]: Sorters.md#ska_sorter
  [smooth-sorter]: Sorters.md#smooth-sorter_sorter
  [sorter-facade]: Sorter-facade.md
  [std-greater-void]: https://en.cppreference.com/w/cpp/utility/functional/greater_void
  [std-less-void]: https://en.cppreference.com/w/cpp/utility/functional/less_void
  [std-mem-fn]: https://en.cppreference.com/w/cpp/utility/functional/mem_fn
  [std-ranges-greater]: https://en.cppreference.com/w/cpp/utility/functional/ranges/greater
  [std-ranges-less]: https://en.cppreference.com/w/cpp/utility/functional/ranges/less
  [std-string-view]: https://en.cppreference.com/w/cpp/string/basic_string_view
  [std-swap]: https://en.cppreference.com/w/cpp/algorithm/swap
  [utility-iter-move]: Miscellaneous-utilities.md#iter_move-and-iter_swap
