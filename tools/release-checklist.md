List of actions to perform when releasing a new cpp-sort version.

### During the development

The following things need be done prior to the release if they didn't happen during the main
development phase:
- [ ] Update the documentation.
- [ ] Update the releases notes.
- [ ] Keep track of the things that will be changed in 2.0.0.

### Before the release

- [ ] Check that all issues linked to the milestone are closed.
- [ ] Check `NOTICE.txt` and `README.md` conformance for stolen code.
- [ ] Make sure that tests pass and examples build.
- [ ] Regenerate the benchmarks.
- [ ] Replace occurences of the version number:
  - [ ] CMakeLists.txt
  - [ ] conanfile.py
  - [ ] README.md
  - [ ] version.h
- [ ] Find a name for the new version.
- [ ] Open a merge request, let the CI do its job.
- [ ] Merge `develop` into `master`.
- [ ] Publish the release, don't forget to target `master`.

### After the release

- [ ] Add the Zenodo badge to the release notes.
- [ ] Close the new version's milestone.
- [ ] Push the new version to Bincrafters.
- [ ] Add the new version to Conan Center Index.
- [ ] Brag about it where relevant.
- [ ] Merge master into 2.0.0-develop branch.
- [ ] Fix merge issues.
- [ ] Improve as needed with C++17 and C++20 features.
