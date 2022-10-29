**cpp-sort** internally reimplements lots of small components of the standard library to better suit it needs, and exposes some of them in its public interface. Those components are available in the namespace `cppsort::mstd` (for *Modified `std::`) and can often be used as drop-in replacements for their standard library counterparts.

Every reimplemented component resides in `<cpp-sort/mstd/HEADER.h>` where `HEADER` is the name of the standard library header where the component would be found. For example, `std::forward_iterator` resides in `<iterator>`, the reimplementation is available in `<cpp-sort/mstd/iterator.h>`. Components are reimplemented as needed, therefore even if a specific components has been reimplemented, there is no guarantee that a very similar component has also been reimplemented.

### `<cpp-sort/mstd/type_traits.h>`

```cpp
template<bool Condition, typename True, typename False>
using conditional_t = /* implementation-defined */;
template<bool Condition, typename T=void>
using enable_if_t = /* implementation-defined */;
```

Those type traits are implemented in such a way that they only ever instantiate two templates: one when `Condition` is `true` and one when it is `false` thanks to a technique described in the [`kvasir::mpl` blog][kvasir-conditional].

```cpp
template<typename T>
struct is_integral;
template<typename T>
struct is_signed;
template<typename T>
struct is_unsigned;

template<typename T>
inline constexpr bool is_integral_v = is_integral<T>::value;
template<typename T>
inline constexpr bool is_signed_v = is_signed<T>::value;
template<typename T>
inline constexpr bool is_unsigned_v = is_unsigned<T>::value;
```

When the front-end supports `__int128_t` and `__uint128_t`, these traits detects them as integral types even when the standard library is not instrumented to recognize them, which is typically the case when using clang-cl, or when using GCC in strict standard mode. When special support is not needed, the standard library ones are directly imported in the `mstd` namespace.

```cpp
template<typename T>
struct make_signed;
template<typename T>
struct make_unsigned;
template<typename T>
using make_signed_t = typename make_signed<T>::type;
template<typename T>
using make_unsigned_t = typename make_unsigned<T>::type;
```

Similarly to the previous traits, those exist to guarantee `__int128_t` and `__uint128_t` when the front-end supports such types.

### `<cpp-sort/mstd/concepts.h>`

```cpp
template<typename T>
concept integral = is_integral_v<T>;
template <typename T>
concept signed_integral = integral<T> && is_signed_v<T>;
template <typename T>
concept unsigned_integral = integral<T> && is_unsigned_v<T>;
```

Just like the type traits they are built upon, those concepts take into account `__int128_t` and `__uint128_t` supported. It is worth noting that those concepts never alias or subsume the standard library ones, in order to avoid potential subtle issues.

### `<cpp-sort/mstd/iterator.h>`

```cpp
template<typename Indirect>
concept indirectly_readable = /* implementation-defined */;
```

Unlike [`std::indirectly_readable`][std-indirectly-readable], this concept imposes no requirement that `std::ranges::iter_move` be usable at all, and drops the [`std::common_reference_with`][std-common-reference-with] rules too. The main reason behind that change is that **cpp-sort** relies on a slightly different iterator model where `iter_move(it)` can return a type unrelated to that returned by `*it`, leading to incompatibilities with the stricter standard iterator model. The library's model was conceived while standard ranges were still a work in progress, hence the difference.

It is worth nothing that the concept is currently incomplete and might get stricter in the future as the exact rules needed for the **cpp-sort** iterator model to work become clearer.

```cpp
template<typename T>
struct incrementable_traits;
template<typename Iterator>
using iter_difference_t = /* implementation-defined */;
```

The only difference with these traits and their standard counterparts is the unconditional support for `__int128_t` as a first-class `difference_type` when the frontend supports it. When dedicated support is not needed, the corresponding `std::` traits are simply imported into the `mstd::` namespace.

```cpp
template<typename Iterator>
concept weakly_incrementable = /* implementation-defined */;
template<typename Iterator>
concept incrementable =
    std::regular<Iterator> &&
    mstd::weakly_incrementable<Iterator>;
```

The main difference with [`std::weakly_incrementable`][std-weakly-incrementable] and [`std::incrementable`][std-incrementable] is that the `mstd` reimplementations do not require iterators to provide a postfix `operator++` at all.

```cpp
template<typename Sentinel, typename Iterator>
concept sentinel_for = /* implementation-defined */;
template<typename Sentinel, typename Iterator>
concept sized_sentinel_for = /* implementation-defined */;
```

Similar to their standard library counterparts. It is worth noting that `sized_sentinel_for` uses [`std::disable_sized_sentinel_for`][std-sized-sentinel-for] and does not reimplement that standard trait, so that user code providing specializations for the standard trait are also taken into account by **cpp-sort**.

```cpp
template<typename Iterator>
concept input_or_output_iterator = /* implementation-defined */;
template<typename Iterator>
concept input_iterator = /* implementation-defined */;
template<typename Iterator>
concept forward_iterator = /* implementation-defined */;
template<typename Iterator>
concept bidirectional_iterator = /* implementation-defined */;
template<typename Iterator>
concept random_access_iterator = /* implementation-defined */;
template<typename Iterator>
concept contiguous_iterator = /* implementation-defined */;
```

The `mstd` iterator concepts rely on the previously described concepts, and are thus affected by the same differences. The only additional deviation from the standard library concepts is that `bidirectional_iterator` does not require a postfix `operator--` to exist at all.

### `<cpp-sort/mstd/ranges.h>`

This header provides the following reimplementations of standard library components:
* `mstd::begin`
* `mstd::end`
* `mstd::size`
* `mstd::distance`
* `mstd::data`
* `mstd::range`
* `mstd::sized_range`
* `mstd::input_range`
* `mstd::forward_range`
* `mstd::bidirectional_range`
* `mstd::random_access_range`
* `mstd::contiguous_range`
* `mstd::iterator_t`
* `mstd::sentinel_t`
* `mstd::range_difference_t`

They are mostly equivalent to the `std::ranges` ones, but rely on the reimplemented `mstd::` components instead, leading to the differences documented in the previous sections. The differences, albeit small, are viral - as a result this list is expected to grow as more concepts are needed in the library's user-facing components.


  [kvasir-conditional]: https://odinthenerd.blogspot.com/2017/03/start-simple-with-conditional-why.html
  [std-common-reference-with]: https://en.cppreference.com/w/cpp/concepts/common_reference_with
  [std-incrementable]: https://en.cppreference.com/w/cpp/iterator/incrementable
  [std-indirectly-readable]: https://en.cppreference.com/w/cpp/iterator/indirectly_readable
  [std-iter-move]: https://en.cppreference.com/w/cpp/iterator/ranges/iter_move
  [std-sized-sentinel-for]: https://en.cppreference.com/w/cpp/iterator/sized_sentinel_for
  [std-weakly-incrementable]: https://en.cppreference.com/w/cpp/iterator/weakly_incrementable
