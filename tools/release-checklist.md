List of actions to perform when releasing a new cpp-sort version.

### During the development

- [ ] Update the documentation.
- [ ] Update the releases notes.
- [ ] Udate `NOTICE.txt` and `README.md` when stealing code.
- [ ] Keep track of the things that will be changed in 2.0.0.

### Before the release

- [ ] Check that there aren't warnings left in the latest CI logs.
- [ ] Check that all issues linked to the milestone are closed.
- [ ] Check `NOTICE.txt` and `README.md` conformance for stolen code.
- [ ] Make sure that tests pass and examples build.
- [ ] Regenerate the benchmarks as needed.
- [ ] Bump the version number with tools/update-version.py.
- [ ] Verify that the Conan recipe works.
- [ ] Try to open `docs` with the latest version of Gollum.
- [ ] Find a name for the new version.
- [ ] Open a merge request, let the CI do its job.
- [ ] Merge `develop` into `master`.
- [ ] Publish the release, don't forget to target `master`.

### After the release

- [ ] Add the Zenodo badge to the release notes.
- [ ] Close the new version's milestone.
- [ ] Check that the documentation was correctly uploaded.
- [ ] Add the new version to Conan Center Index.
- [ ] Brag about it where relevant.
