In this tutorial, we will see how to write a `randomizing_adapter`, a [sorter adapter][sorter-adapters] that shuffles the collection to sort prior to sorting it with the *adapted sorter*.

*Note: this tutorial assumes that you have already followed the one about [writing a `bubble_sorter`][writing-a-bubble-sorter] since most of the techniques described in it also apply to writing sorter adapters.*

## Why shuffle the collection prior to sorting it?

The main purpose of sorting is to bring order to a collection of elements, which might question the legitimacy of why anyone would want to shuffle said collection prior to sorting it. It turns out that there are several answers to that question.

### Breaking quicksort-adverse patterns

Depending on how it picks its pivot, a quicksort algorithm might run in quadratic time even on almost-sorted collections though these quicksort-adverse patterns generally don't occur on "random" data. The argument has been made that there is often pre-existing order in real life scenarios, so the collection passed to quicksort should be shuffled.

The `randomizing_adapter` we are about to write allows to seamlessly wrap a naive `quick_sorter` to implement that trick:

```cpp
using randomizing_quicksort = randomizing_adapter<quick_sorter>;
```

Obviously such a trick does not guarantee a O(n log n) runtime for the resulting sorter, but when applied to data with pre-existing order, it does make a quadratic runtime much less likely.

*Note: the library's [`quick_sorter`][quick-sorter] already has a guaranteed O(n log n) worst case runtime, which would make such a trick unneeded anyway.*

### Hyrum's law

An arguably more useful use for a `randomizing_adapter` would be to avoid becoming a victim of [Hyrum's law][hyrums-law].

> With a sufficient number of users of an API,
> it does not matter what you promise in the contract:
> all observable behaviors of your system
> will be depended on by somebody.

Danila Kutenin rightfully mentions that [changing `std::sort` is harder than meets the eye][changing-std-sort], the main reason being that pieces of code accidentally rely on the observable yet not guaranteed properties of [`std::sort`][std-sort], namely the order of *equivalent elements*. The article gives [golden tests][golden-tests] as an example of things that might break when changing a sorting algorithm.

In order to make it less likely for users to rely on the order of *equivalent elements*, the author proposes to shuffle the collection prior to sorting it debug mode. This makes the order of equivalent elements non deterministic, which in turns can purposefuly break code accidentally relying on this order.

It might seem at first that **cpp-sort**'s algorithms are not vulnerable to such changes since the name of the algorithm is part of sorter's name, but the truth is that their implementation still changes, and a user of the library might still want to swap a sorter for another one and suffer the same fate.

The solution for libc++ was to guard `std::sort` with a `_LIBCPP_DEBUG_RANDOMIZE_RANGE` macro, a solution more in the spirit of **cpp-sort** is to provide an adapter for users who might want to add such a safeguard.

## A basic `randomizing_adapter`

Writing a sorter adapter is very similar to writing a sorter:
* Provide a suitable `operator()`.
* Document the [category of iterators][iterator-category] it accepts.
* Optionally document its stability guarantees.

In the case of `randomizing_adapter`, all of those are quite simple:
* The algorithm is really just "shuffle the collection, then call the wrapped sorter".
* We are using `std::shuffle`, so it only accepts random-access iterators.
* Shuffling ensures that the sort is never stable.

```cpp
template<typename Sorter>
struct randomizing_adapter
{
    template<typename RandomAccessIterator, typename... Args>
    auto operator()(RandomAccessIterator begin, RandomAccessIterator end, Args&&... args) const
        -> decltype(Sorter{}(begin, end, std::forward<Args>(args)...))
    {
        thread_local std::random_device device;
        thread_local std::minstd_rand engine(device());
        std::shuffle(begin, end, engine);
        return Sorter{}(begin, end, std::forward<Args>(args)...);
    }

    using iterator_category = std::random_access_iterator_tag;
    using is_always_stable = std::false_type;
};
```

When possible, a proper *sorter adapter* is expected to be callable with the same combination of parameters than the *sorter* it wraps, which often calls for an implementation class later wrapped in [`sorter_facade`][sorter-facade] and adding the proper SFINAE constraints to `operator()`. For this simple adapter we don't need to go to such lengths and instead rely on [expression SFINAE][sfinae] in the return type expression to transitively benefit from the sorter.

## Returned value

There is currently no strict rule about what a sorter adapter should return (this is actually a [open design issue][issue-134]), though the general wisdom is that an adapter should transparently provide as many features as the sorter it adapts when it reasonably can. The idea is that replacing the sorter by its wrapped counterpart should be easy.

We don't have a specific use for the return channel of `randomizing_adapter` and it is simple to make it transitively return whatever the wrapped sorter returns - and even convenient -, so I decided to do just that.

## Construction

A sorter adapter should be exmplicitly constructible from an instance of the sorter it adapts, so we need to give it appropriate constructors:

```cpp
randomizing_adapter() = default;
constexpr explicit randomizing_adapter(Sorter) {}
```

It might seem useless since our adapter does not store an instance of the sorter it wraps, but it adds expressiveness with [class template argument deduction][ctad]:

```cpp
auto sort = randomizing_adapter(cppsort::poplar_sorter);
```

## `utility::adapter_storage`

As previously mentioned, our `randomizing_adapter` currently does not store the adapted sorter even though it might be desirable since sorters can be stateful. **cpp-sort** provides the class template [`utility::adapter_storage`][adapter-storage] that adapters can inherit from to take care of storing a sorter instance.

```cpp
template<typename Sorter>
struct randomizing_adapter:
    cppsort::utility::adapter_storage<Sorter>
{
    randomizing_adapter() = default;

    constexpr explicit randomizing_adapter(Sorter sorter):
        cppsort::utility::adapter_storage<Sorter>(std::move(sorter))
    {}

    template<typename RandomAccessIterator, typename... Args>
    auto operator()(RandomAccessIterator begin, RandomAccessIterator end, Args&&... args) const
        -> decltype(this->get()(begin, end, std::forward<Args>(args)...))
    {
        thread_local std::random_device device;
        thread_local std::minstd_rand engine(device());
        std::shuffle(begin, end, engine);
        return this->get()(begin, end, std::forward<Args>(args)...);
    }
};
```

`adapter_storage<Sorter>` is constructed with an instance of `Sorter` and has value semantics: it holds a copy of the sorter, not a reference to it. The stored sorter can be accessed via the `get()` method.

It is special-cased for empty sorters: when constructed with one, it doesn't store it and instead default-constructs a new instance when `get()` is called. The lack of storage allows the adapter to be converted to a function pointer when it stores an empty sorter.

## Polishing it a bit

`randomizing_adapter` is already usable as is, but it can still benefit from small improvements like those we gave to `bubble_sorter` [in the other tutorial][writing-a-bubble-sorter]:
* Add an `operator()` overload accepting an iterable to benefit from O(1) `.size()` functions.
* Add a `static_assert` to make it clear when it isn't given random-access iterators.

## Conclusion

We have seen how to write a simple sorter adapter which gives users the option to harden their code against Hyrum's law in a more granular fashion that simply sticking a macro in every sorter.

This one was pretty straightforward, but writing adapters can occasionally be much more challenging. Good adapters can ideally replace the sorter they wrap without effort, and there's generally more to it than meets the eye.

The full implementation can be found in the `examples` folder.


  [adapter-storage]: Miscellaneous-utilities.md#adapter_storage
  [changing-std-sort]: https://danlark.org/2022/04/20/changing-stdsort-at-googles-scale-and-beyond/
  [ctad]: https://en.cppreference.com/w/cpp/language/class_template_argument_deduction
  [golden-tests]: https://en.wikipedia.org/wiki/Characterization_test
  [hyrums-law]: https://www.hyrumslaw.com/
  [issue-134]: https://github.com/Morwenn/cpp-sort/issues/134
  [iterator-category]: https://en.cppreference.com/w/cpp/iterator
  [proxy-iterators]: https://wg21.link/P0022
  [quick-sorter]: Sorters.md#quick_sorter
  [sfinae]: https://en.cppreference.com/w/cpp/language/sfinae
  [sorter-adapters]: Sorter-adapters.md
  [sorter-facade]: Sorter-facade.md
  [sorter-traits]: Sorter-traits.md#sorter_traits
  [writing-a-sorter]: Writing-a-sorter.md
  [writing-a-bubble-sorter]: Writing-a-bubble_sorter.md
