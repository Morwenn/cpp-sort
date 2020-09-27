Sometimes, iterators are not enough and you want to use the full abilities of containers to sort them, like O(1) insertion for [`std::list`](http://en.cppreference.com/w/cpp/container/list) and [`std::forward_list`](http://en.cppreference.com/w/cpp/container/forward_list). **cpp-sort** makes it possible to enhance sorters so that they can recognize specific containers and use a dedicated altered version of the sorting algorithm to sort the container thanks to [`container_aware_adapter`](https://github.com/Morwenn/cpp-sort/wiki/Sorter-adapters#container_aware_adapter).

Let's get straight to example and enhance `selection_sort` for a custom list class to take advantage of the O(1) insertion. Here is the list implementation:

```cpp
namespace example
{
    template<typename T, typename Allocator>
    class list
    {
        // Exactly like std::list
    };
}
```

Second, we need a dedicated selection sort to stably sort our new list class, with full comparison support (no projection support because it would be bothersome without a projection-enhanced standard library):

```cpp
namespace example
{
    template<typename T, typename Allocator, typename Compare=std::less<>>
    auto list_selection_sort(list<T, Allocator>& collection, Compare compare={})
        -> void
    {
        auto&& comp = cppsort::utility::as_function(compare);

        auto it = std::begin(collection);
        auto last = std::end(collection);
        while (it != last) {
            auto min_it = std::min_element(it, last, comp);
            if (min_it == it) {
                ++it;
            } else {
                collection.splice(it, collection, min_it);
            }
        }
    }
}
```

As is, `container_aware_adapter` doesn't know about the sorting algorithm and can't use it. In order to find dedicated algorithm, it performs an ADL lookup to find a suitable `sort` function in the container's namespace. The `sort` function shall take the "overloaded" sorter as a first parameter, the container to sort as a second parameter, and comparison and projection functions may follow. Many overload generation rules from [`sorter_facade`](https://github.com/Morwenn/cpp-sort/wiki/Sorter-facade) are also implemented in `container_aware_adapter` to ensure that it will try to call the dedicated algorithm whenever it can. Here is the whole thing:

```cpp
namespace example
{
    template<typename T, typename Allocator>
    class list
    {
        // Exactly like std::list
    };

    template<typename T, typename Allocator, typename Compare=std::less<>>
    auto sort(cppsort::selection_sorter, list<T, Allocator>& collection,
              Compare compare={})
        -> void
    {
        auto&& comp = cppsort::utility::as_function(compare);

        auto it = std::begin(collection);
        auto last = std::end(collection);
        while (it != last) {
            auto min_it = std::min_element(it, last, comp);
            if (min_it == it) {
                ++it;
            } else {
                collection.splice(it, collection, min_it);
            }
        }
    }
}
```

There will be improvements to `container_aware_adapter` in the future to make `stable_sort` another customization point, and probably to make `sort` fall back to `stable_sort` when there is an ADL-found `stable_sort` but no ADL-found `sort`. However, it's a huge piece of work, so it probably won't be before a while.