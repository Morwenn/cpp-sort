**cpp-sort**'s testsuite uses the framework [Catch][1], and can be built from the project's root directory with the
following commands (requires [CMake][2] to be installed):

```bash
cmake .
make
testsuite/cpp-sort-testsuite;
```

If GCC or Clang sanitizers are availbale, `cmake` can be passed the configuration variable `-DSANITIZE=san` with any
value `san` such as `san` can be given to the compilers as `-fsanitize=san`.

The testsuite executable, being build with Catch, can notably take the option `--rng-seed` to seed the pseudo-random
number generators throughout the testsuite. If not provided, `0` will be used instead. See [Catch's documentation][3]
for more information about the possible testsuite options.


  [1]: https://github.com/philsquared/Catch
  [2]: https://cmake.org/
  [3]: https://github.com/philsquared/Catch/blob/master/docs/command-line.md
