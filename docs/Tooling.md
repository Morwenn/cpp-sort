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

The project's CMake files offers some options, though they are mainly used to configure the test suite and examples:
* `CPPSORT_BUILD_TESTING`: whether to build the test suite, defaults to `ON`.
* `CPPSORT_BUILD_EXAMPLES`: whether to build the examples, defaults to `OFF`. 
* `CPPSORT_ENABLE_COVERAGE`: whether to produce code coverage information when building the test suite, defaults to `OFF`.
* `CPPSORT_USE_VALGRIND`: whether to run the test suite through Valgrind, defaults to `OFF`.
* `CPPSORT_SANITIZE`: comma-separated list of values to pass to the `-fsanitize` flag of compilers that support it, defaults to an empty string.
* `CPPSORT_STATIC_TESTS`: when `ON`, some tests are executed at compile time instead of runtime, defaults to `OFF`.

Some of those options also exist without the `CPPSORT_` prefix, but they are deprecated. For compatibility reasons, the options with the `CPPSORT_` prefix default to the values of the equivalent unprefixed options.

*New in version 1.6.0:* added the option `BUILD_EXAMPLES`.

*New in version 1.9.0:* options with the `CPPSORT_` prefix.

*New in version 1.13.0:* added the option `CPPSORT_STATIC_TESTS`.

***WARNING:** options without a `CPPSORT_` prefixed are deprecated in version 1.9.0 and removed in version 2.0.0.*

[Catch2][catch2] 3.0.0-preview4 or greater is required to build the tests: if a suitable version has been installed on the system it will be used, otherwise the latest suitable Catch2 release will be downloaded.

*Changed in version 1.7.0:* if a suitable Catch2 version is found on the system, it will be used.

*Changed in version 1.13.0:* cpp-sort now requires Catch2 version 3.0.0-preview4 instead of 2.6.0.

## Conan

**cpp-sort** is available directly on [Conan Center][conan-center]. You can find the different versions available with the following command:

```sh
conan search cpp-sort --remote=conan-center
```

And then install any version to your local cache as follows (here with version 1.13.2):

```sh
conan install cpp-sort/1.13.2
```

The packages downloaded from conan-center are minimal and only contain the files required to use **cpp-sort** as a library: the headers, CMake files and licensing information. If you need anything else you have to build your own package with the `conanfile.py` available in this repository.

## Gollum

[Gollum][gollum], if installed, can be used to browse this documentation offline:

1. Navigate to the main `cpp-sort` directory in the command line
2. `gollum --page-file-dir docs --ref <branch-name>`
3. Visit http://localhost:4567/Home

This can notably used to browse old versions of the documentation. It seems however that `--ref` doesn't understand Git tags, so you have to create a proper branch from the version tag you want to browse beforehand.

Due to slight markup differences, some pages might not fully render correctly but it should nonetheless be a better experience than navigaitng the Markdown files by hand.


  [catch2]: https://github.com/catchorg/Catch2
  [cmake]: https://cmake.org/
  [conan]: https://conan.io/
  [conan-center]: https://conan.io/center/cpp-sort
  [gollum]: https://github.com/gollum/gollum
