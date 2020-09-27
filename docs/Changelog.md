This page describes the features that change in **cpp-sort** depending on the C++ version with which it is compiled (C++14 or later) as well as the support for miscellaneous compiler extensions; for a full changelog between actual releases, you can check the dedicated [releases page](https://github.com/Morwenn/cpp-sort/releases).

*The notes in this page are only valid for the latest versions of the 1.x and 2.x branches. If you are using an older version of the library, some of them might not apply.*

## C++14 features

While **cpp-sort** theoretically requires a fully C++14-compliant compiler, a few standard features are either not available or deactivated in popular compilers and the library tries to take those into account if possible.

**Performance improvements:**
* Sized deallocation: this C++14 feature is not always available (Clang requires `-fsized-deallocation` for example) and standard allocation functions typically don't take advantage of it. However, if `__cpp_sized_deallocation` is defined and the global deallocations functions are replaced with overloads that take advantage of sized deallocation, then several sorters will explicitly try to take advantage of it.

## C++17 features

When compiled with C++17, **cpp-sort** might gain a few additional features depending on the level of C++17 support provided by the compiler. The availability of most of the features depend on the presence of corresponding [feature-testing macros](https://wg21.link/SD6). The support for feature-testing macros being optional, it is possible that one of the features listed below isn't available even though the compiler is supposed to provide enough C++17 features to support it. If it is the case and it is a problem for you, don't hesitate to open an issue so that we can explicitly support the given compiler.

**New features:**
* `string_spread_sort` now accepts [`std::string_view`](https://en.cppreference.com/w/cpp/string/basic_string_view) and sometimes `std::wstring_view`.

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

* New [`function_constant`](https://github.com/Morwenn/cpp-sort/wiki/Miscellaneous-utilities#miscellaneous-function-objects) utility to micro-optimize function pointers and class member pointers.

    ```cpp
    insertion_sort(collection, function_constant<&foo::bar>{});
    ```

    It sometimes results in fewer indirections than a raw `&foo::bar`, and can be subject to *empty base class optimization* when stored.

    This feature is available when the feature-testing macro `__cpp_nontype_template_parameter_auto` is defined.

* The function pointer conversion operators of `sorter_facade` are now `constexpr` when possible.

   This feature is made available through the check `__cpp_constexpr >= 201603`.

**Correctness improvements:**
* Some handy C++17 type traits such as `std::is_invocable` are manually reimplemented in C++14 mode while they are used as is in C++17 mode if available. It's likely that the C++17 implementation covers more corner cases and is thus more often correct than the manual C++14 implementation.

    The C++17 traits are used as is when the feature-test macro `__cpp_lib_is_invocable` is defined.

## Other features

**cpp-sort** tries to take advantage of more than just standard features when possible, and also to provide extended support for some compiler-specific extensions. Below is a list of the impact that non-standard features might have on the library:

**Extension-specific support:**
* 128-bit integers support: `ska_sorter` has dedicated support for 128-bit integers (`unsigned __int128` or `__uint128_t` and its signed counterpart), no matter whether the standard library is also instrumented for those types. This support should be available as long as `__SIZEOF_INT128__` is defined by the compiler.

**Performance improvements:**
* Additional allocators: `merge_insertion_sorter` can be somewhat more performant when libstdc++'s [`bitmap_allocator`](https://gcc.gnu.org/onlinedocs/libstdc++/manual/bitmap_allocator.html) is available.

    This improvement is made available through the check `__has_include(<ext/bitmap_allocator.h>)`, which means that it should be available for every compiler where `__has_include` and libstdc++ are available (old and new Clang, and more recent GCC).

    *Changed in version 1.7.0:* `merge_insertion_sorter` uses a custom list implementation which does not need to take advantage of `bitmap_allocator` anymore.

* Bit manipulation intrinsics: there are a few places where bit tricks are used to perform a few operations faster. Some of those operations are made faster with bitwise manipulation intrinsics when those are available.
