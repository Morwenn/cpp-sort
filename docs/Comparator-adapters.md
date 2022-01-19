The comparator adapters below are classes mainly designed to wrap binary predicates and should themselves satisfy the [*BinaryPredicate*][binary-predicate] named requirement.

All adapters below are composed of two elements:
* A class template that wraps a comparator and is itself a comparator (ex: `not_fn_t`, `flip_t`).
* A function template that simplifies the construction and sometimes implements optimizations (ex: `not_fn`, `flip`).

The optimizations performed by the function templates are of the "unwrapping" kind, with a goal to reduce the nesting of templates in the library and to eventually reduce the overall number of instantiated templates.

```cpp
auto cmp = std::less{};
auto cmp2 = cppsort::flip(cmp); // cppsort::flip_t<std::less<>>
auto cmp3 = cppsort::flip(cmp2); // std::less<>
```

Those unwrappings are meant to be simple and only intended to work with "well-formed" functions that don't go against simple logic rules. If the passed comparators are too tricky and absolutely need the extra wrapping, then it is advised to use the adapter template classes directly instead of the construction functions.

### `flip`

```cpp
#include <cpp-sort/comparators/flip.h>
```

The class template `flip_t` is a function object which, when called, passes the arguments in reversed order to the *Callable* it holds with and returns the result. It is named after the [`flip`][prelude-flip] function from Haskell's Prelude module.

`flip_t<F>` has the following member functions:

```cpp
// Construction
explicit constexpr flip_t(const F& func);
explicit constexpr flip_t(F&& func);

// Call with flipped arguments
template<typename T1, typename T2>
constexpr auto operator()(T1&& x, T2&& y) &/const&/&&/const&&
    noexcept(noexcept(std::invoke(base(), std::forward<T2>(y), std::forward<T1>(x))))
    -> decltype(std::invoke(base(), std::forward<T2>(y), std::forward<T1>(x)));

// Retrieve the passed callable
constexpr auto base() const
    -> F;
```

`cppsort::flip` takes a *Callable* of type `F` and returns an instance of `flip_t<std::decay_t<F>>` except in the following cases:
* When given `flip_t<F>`, it returns `F`.
* When given `not_fn_t<flip_t<F>>`, it returns `not_fn_t<F>`.

`flip_t<F>` is [*transparent*][transparent-comparator] when `F` is *transparent*.

`flip_t<F>` is considered [branchless][branchless-traits] when `F` is considered branchless.

*New in version 1.13.0*

### `not_fn`

```cpp
#include <cpp-sort/comparators/not_fn.h>
```

The class template `not_fn_t` is roughly equivalent to the one returned by the C++17 [`std::not_fn`][std-not-fn], a function object which, when called, returns the negation of the *Callable* it holds.

`not_fn_t<F>` has the following member functions:

```cpp
// Construction
explicit constexpr not_fn_t(const F& func);
explicit constexpr not_fn_t(F&& func);

// Call and negate
template<typename... Args>
constexpr auto operator()(Args&&... args) &/const&/&&/const&&
    noexcept(noexcept(not std::invoke(base(), std::forward<Args>(args)...)))
    -> decltype(not std::invoke(base(), std::forward<Args>(args)...));

// Retrieve the passed callable
constexpr auto base() const
    -> F;
```

`cppsort::not_fn` takes a *Callable* of type `F` and returns an instance of `not_fn_t<std::decay_t<F>>` except in the following cases:
* When given `not_fn_t<F>`, it returns `F`.
* When given `flip_t<not_fn_t<F>>`, it returns `flip_t<F>`.

`not_fn_t<F>` is [*transparent*][transparent-comparator] when `F` is *transparent*.

*New in version 1.13.0*

### `projection_compare`

```cpp
#include <cpp-sort/comparators/projection_compare.h>
```

The class template `projection_compare` can be used to embed a comparison and a projection in a single comparison object, allowing to provide projection support to algorithms that only support comparisons, such as standard library algorithms prior to C++20. Both the passed comparison and projection functions can be [*Callable*][callable].

`projection_compare` is [*transparent*][transparent-comparator] when the passed comparison and projection are both *transparent*.

It is accompanied by a `make_projection_compare` function template to avoid having to pass the template parameters by hand.

**Example:**

```cpp
// Sort a family from older to younger member
std::vector<Person> family = { /* ... */ };
std::sort(family.begin(), family.end(), cppsort::make_projection_compare(std::greater<>{}, &Person::age));
```

`cppsort::make_projection_compare` takes two *Callable* of types `C` and `P` and returns an instance of `projection_compare<std::decay_t<C>, std::decay_t<P>>` except in the following cases:
* When `std::decay_t<P>` is of type [`utility::identity`][utility-identity] or [`std::identity`][std-identity], it returns `C` directly.

`projection_compare` is considered [branchless][branchless-traits] when the projection it wraps is considered branchless and the comparison it wraps is considered branchless when called with the result of the projection.

*New in version 1.9.0*

*Changed in version 1.13.0:* `make_projection_compare` now returns the comparison directly when the passed projection is an identity function object.


  [binary-predicate]: https://en.cppreference.com/w/cpp/concept/BinaryPredicate
  [branchless-traits]: https://github.com/Morwenn/cpp-sort/wiki/Miscellaneous-utilities#branchless-traits
  [callable]: https://en.cppreference.com/w/cpp/named_req/Callable
  [flip-prelude]: https://hackage.haskell.org/package/base-4.16.0.0/docs/Prelude.html#v:flip
  [std-identity]: https://en.cppreference.com/w/cpp/utility/functional/identity
  [std-not-fn]: https://en.cppreference.com/w/cpp/utility/functional/not_fn
  [transparent-comparator]: https://stackoverflow.com/q/20317413/1364752
  [utility-identity]: https://github.com/Morwenn/cpp-sort/wiki/Miscellaneous-utilities#miscellaneous-function-objects
