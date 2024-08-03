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
flip_t() = default;
explicit constexpr flip_t(const F& func);
explicit constexpr flip_t(F&& func);

// Call with flipped arguments
template<typename Self, typename T1, typename T2>
constexpr auto operator()(this Self&& self, T1&& x, T2&& y)
    noexcept(noexcept(std::invoke(self.base(), std::forward<T2>(y), std::forward<T1>(x))))
    -> decltype(std::invoke(self.base(), std::forward<T2>(y), std::forward<T1>(x)));

// Retrieve the passed callable
constexpr auto base() const
    -> F;
```

`cppsort::flip` takes a *Callable* `f` of type `F` and returns an instance of `flip_t<std::decay_t<F>>` except in the following cases:
* When given a `flip_t<F>`, it returns a `F`.
* When given a `not_fn_t<flip_t<F>>`, it returns a `not_fn_t<F>`.
* When given a `projection_compare_t` it returns `projection_compare(flip(f.comparison()), f.projection())`.

`flip_t<F>` is [*transparent*][transparent-func] when `F` is *transparent*.

`flip_t<F>` is considered [branchless][branchless-traits] when `F` is considered branchless.

### `not_fn`

```cpp
#include <cpp-sort/comparators/not_fn.h>
```

The class template `not_fn_t` is roughly equivalent to the one returned by the C++17 [`std::not_fn`][std-not-fn], a function object which, when called, returns the negation of the *Callable* it holds.

`not_fn_t<F>` has the following member functions:

```cpp
// Construction
not_fn_t() = default;
explicit constexpr not_fn_t(const F& func);
explicit constexpr not_fn_t(F&& func);

// Call and negate
template<typename Self, typename... Args>
constexpr auto operator()(this Self&& self, Args&&... args) &/const&/&&/const&&
    noexcept(noexcept(not std::invoke(self.base(), std::forward<Args>(args)...)))
    -> decltype(not std::invoke(self.base(), std::forward<Args>(args)...));

// Retrieve the passed callable
constexpr auto base() const
    -> F;
```

`cppsort::not_fn` takes a *Callable* `f` of type `F` and returns an instance of `not_fn_t<std::decay_t<F>>` except in the following cases:
* When given a `not_fn_t<F>`, it returns a `F`.
* When given a `flip_t<not_fn_t<F>>`, it returns a `flip_t<F>`.
* When given a `projection_compare_t` it returns `projection_compare(not_fn(f.comparison()), f.projection())`.

`not_fn_t<F>` is [*transparent*][transparent-func] when `F` is *transparent*.

### `projection_compare`

```cpp
#include <cpp-sort/comparators/projection_compare.h>
```

The class template `projection_compare_t` can be used to embed a comparison and a projection in a single comparison object, allowing to provide projection support to algorithms that only support comparisons, such as standard library algorithms prior to C++20. Both the passed comparison and projection functions can be [*Callable*][callable].

It is accompanied by a `projection_compare` function template to avoid having to pass the template parameters by hand.

**Example:**

```cpp
// Sort a family from older to younger member
std::vector<Person> family = { /* ... */ };
std::sort(family.begin(), family.end(), cppsort::make_projection_compare(std::greater<>{}, &Person::age));
```

`projection_compare_t<C, P>` has the following member functions:

```cpp
// Construction
projection_compare_t() = default;
projection_compare_t(C comp, P proj);

// Call
template<typename Self, typename T1, typename T2>
constexpr auto operator()(this Self&& self, T1&& x, T2&& y)
    noexcept(noexcept(std::invoke(self.comparison(),
                                  std::invoke(self.projection(), std::forward<T1>(x)),
                                  std::invoke(self.projection(), std::forward<T1>(y)))))
    -> decltype(std::invoke(self.comparison(),
                            std::invoke(self.projection(), std::forward<T1>(x)),
                            std::invoke(self.projection(), std::forward<T1>(y))));

// Retrieve the passed comparison
constexpr auto comparison() const
    -> C;

// Retrieve the passed projection
constexpr auto projection() const
    -> P;
```

`cppsort::projection_compare` takes two *Callable* of types `C` and `P` and returns an instance of `projection_compare_t<std::decay_t<C>, std::decay_t<P>>` except in the following cases:
* When `std::decay_t<P>` is of type [`std::identity`][std-identity], it returns `C` directly.

`projection_compare_t` is [*transparent*][transparent-func] when the passed comparison and projection are both *transparent*.

`projection_compare_t` is considered [branchless][branchless-traits] when the projection it wraps is considered branchless and the comparison it wraps is considered branchless when called with the result of the projection.


  [binary-predicate]: https://en.cppreference.com/w/cpp/concept/BinaryPredicate
  [branchless-traits]: Miscellaneous-utilities.md#branchless-traits
  [callable]: https://en.cppreference.com/w/cpp/named_req/Callable
  [prelude-flip]: https://hackage.haskell.org/package/base-4.16.0.0/docs/Prelude.html#v:flip
  [std-identity]: https://en.cppreference.com/w/cpp/utility/functional/identity
  [std-not-fn]: https://en.cppreference.com/w/cpp/utility/functional/not_fn
  [transparent-func]: Comparators-and-projections.md#Transparent-function-objects
