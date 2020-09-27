*New in version 1.7.0*

Sometimes one might need to apply several transformations to the elements of a collection before comparing them. To support this use case, some projection functions in **cpp-sort** can be composed with `operator|`

```cpp
struct my_negate:
    cppsort::utility::projection_base
{
    int operator()(int value) const
    {
        return -value;
    }
};
```

Making a function object inherit from `cppsort::utility::projection_base` allows it to benefit from the `operator|` overload used to compose projections; the projection inheriting from that class can appear on any side of the operator, and the other argument can be any suitable [*Callable*][callable]. Here is an example of what is possible with the custom projection defined above:

```cpp
// Create a vector of wrapper
struct wrapper { int value; };
std::vector<wrapper> vec = { /* ... */ };

my_negate projection;
// Applies &wrapper::value to the elements of vec, then my_negate
cppsort::poplar_sort(vec, &wrapper::value | projection);
```

The object returned by the utility function [`cppsort::utility::as_projection`][as_projection] also inherits from `cppsort::utility::projection_base`, making `as_projection` the proper function to turn any suitable projection into a projection composable with `operator|`.


  [as_projection]: https://github.com/Morwenn/cpp-sort/wiki/Miscellaneous-utilities#as_comparison-and-as_projection
  [callable]: https://en.cppreference.com/w/cpp/named_req/Callable