### Refining comparison and projection functions

Type-agnostic comparators *à la* `std::less<>` are handy compared to the old `std::less<T>` because they allow to use the same comparator everywhere without having to care about the type that is being compared before comparison time. Since the type is deduced by the function object's `operator()`, specifying it by hand is a bit boring and redundant.

However, it is sometimes useful to have the type information prior to the comparison (or projection). For example, a case-insensitive comparison needs to call `std::use_facet<std::ctype<CharT>>` to know how to compare the given sequence of characters, which might be an expensive operation (at least according to [*How to do case-insensitive string comparison*][case-insensitive-strcmp] by Matt Austern). That call could be performed once prior to the sorting if the comparator knew the character type (char, wchar_t, etc...).

To keep the handiness of type-agnostic comparators, while avoidind performance problems, some comparators and projections in **cpp-sort** have a `refine` method that takes a type template parameter and returns a new function object that takes advantage of the type information (*e.g.* a typed [`case_insensitive_less`][case-insensitive-less]) makes a single call to `std::use_facet<std::ctype<CharT>>` when constructed):

```cpp
auto typed_compare = compare.refine<T>();
```

### Automatic refinement with `sorter_facade`

Explicitly calling the `refine` method can still be considered bothersome, so `sorter_facade` already handles it: if the passed comparison or projection function has a suitable `refine` method, this method is automatically called with the `value_type` of the passed iterator or container, and the result is passed to the *sorter implementation* instead of the original type-agnostic function. This allows to keep the terseness of type-agnostic comparison and projection functions while benefiting from the potential optimizations inherent to the type information.

### Refining API

```cpp
#include <cpp-sort/refined.h>
```

This small header provides the essential utily needed to used refined comparisons and projection functions: the `refined` function template takes a template parameter `T` and a function `func`, then it returns `func.template refine<T>()` if `func` has a valid `refine` method, and returns `func` otherwise.

```cpp
template<typename T, typename Function>
auto refined(Function&& func)
    noexcept(noexcept(/* returned expression */))
    -> /* see above */;
```

Additionally, the type trait `refined_t` allows to easily deduce the type of a given comparison or projection function once refined for a given type.

```cpp
template<typename T, typename Function>
using refined_t = std::remove_cvref_t<
    decltype(refined<std::decay_t<T>>(std::declval<Function&>()))
>;
```


  [case-insensitive-less]: https://github.com/Morwenn/cpp-sort/wiki/Comparators#case-insensitive-comparator
  [case-insensitive-strcmp]: http://lafstern.org/matt/col2_new.pdf