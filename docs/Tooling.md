**cpp-sort** offers some tooling support out-of-the-box, notably for [CMake][cmake] and [Conan][conan].

The library's repository does contain files specific to other tools, but most of those exist to properly instrument the continuous integration, so they are not documented here. If you have use for such files outside of continuous integration, please open a documentation issue.

## CMake

### Using cpp-sort

**cpp-sort** can be installed via CMake, in which case it exports a `cpp-sort::cpp-sort` target and all the files required for a basic integration. Once it has been installed on the system, the following lines should be enough to use it as a dependency:

```cmake
find_package(cpp-sort REQUIRED CONFIG)
target_link_libraries(my-target PRIVATE cpp-sort::cpp-sort)
```

If you don't want to install **cpp-sort** directly, it can still be used directly as a subdirectory:

```cmake
add_subdirectory(third_party/cpp-sort)
target_link_libraries(my-target PRIVATE cpp-sort::cpp-sort)
```

*New in version 1.6.0:* cpp-sort can be used directly with `add_subdirectory`.

### Building cpp-sort

The project's CMake files do offer some options, but they are mainly used to configure the test suite and the examples:
* `CPPSORT_BUILD_TESTING`: whether to build the test suite, defaults to `ON`.
* `CPPSORT_BUILD_EXAMPLES`: whether to build the examples, defaults to `OFF`. 
* `CPPSORT_ENABLE_COVERAGE`: whether to produce code coverage information when building the test suite, defaults to `OFF`.
* `CPPSORT_USE_VALGRIND`: whether to run the test suite through Valgrind, defaults to `OFF`.
* `CPPSORT_SANITIZE`: values to pass to the `-fsanitize` falgs of compilers that supports them, default to empty.

The same options exist without the `CPPSORT_` prefix exist, but are deprecated. For compatibility reasons, the options with the `CPPSORT_` prefix default the values of the equivalent unprefixed options.

*New in version 1.6.0:* added the option `BUILD_EXAMPLES`.

*New in version 1.9.0:* options with the `CPPSORT_` prefix.

***WARNING:** options without a `CPPSORT_` prefixed are deprecated in version 1.9.0 and removed in version 2.0.0.*

[Catch2][catch2] 3.0.0-preview4 or greater is required to build the tests: if a suitable version has been installed on the system it will be used, otherwise the latest suitable Catch2 release will be downloaded.

*Changed in version 1.7.0:* if a suitable Catch2 version is found on the system, it will be used.

*Changed in version 1.13.0 (BREAKING):* cpp-sort now requires Catch2 version 3.0.0-preview4 instead of 2.6.0.

## Conan

**cpp-sort** is available directly on [Conan Center][conan-center]. You can find the different versions available with the following command:

```sh
conan search cpp-sort --remote=conan-center
```

And then install any version to your local cache as follows (here with version 1.12.1):

```sh
conan install cpp-sort/1.12.1
```

The packages downloaded from conan-center are minimal and only contain the files required to use **cpp-sort** as a library: the headers, CMake files and licensing information. If you need anything else you have to build your own package with the `conanfile.py` available in this repository.


  [catch2]: https://github.com/catchorg/Catch2
  [cmake]: https://cmake.org/
  [conan]: https://conan.io/
  [conan-center]: https://conan.io/center/cpp-sort
