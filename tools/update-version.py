# -*- coding: utf-8 -*-

# Copyright (c) 2022-2024 Morwenn
# SPDX-License-Identifier: MIT

import argparse
import datetime
import fileinput
import re
import textwrap
from pathlib import Path


def read_version_number(version_file_path: Path) -> str:
    parts = {}
    with version_file_path.open(encoding='utf-8') as fd:
        for line in fd:
            if res := re.search(r"#define CPPSORT_VERSION_(?P<kind>[A-Z]+) (?P<value>\d+)\n", line):
                kind, value = res.groups()
                parts[kind] = value
    return f"{parts['MAJOR']}.{parts['MINOR']}.{parts['PATCH']}"


def write_version_h(version_file_path: Path, version: str) -> None:
    major, minor, patch = version.split('.')
    year = datetime.datetime.now().year

    text = textwrap.dedent(f"""
        /*
         * Copyright (c) 2018-{year} Morwenn
         * SPDX-License-Identifier: MIT
         */
        #ifndef CPPSORT_VERSION_H_
        #define CPPSORT_VERSION_H_

        // Semantic versioning macros

        #define CPPSORT_VERSION_MAJOR {major}
        #define CPPSORT_VERSION_MINOR {minor}
        #define CPPSORT_VERSION_PATCH {patch}

        #endif // CPPSORT_VERSION_H_
    """)

    with version_file_path.open('w', encoding='utf-8') as fd:
        fd.write(text[1:])


def replace_version_number(paths: list[Path], old_version: str, new_version: str) -> None:
    copyright_regex = r"# Copyright \(c\) (?P<first_year>\d{4})-\d{4} Morwenn"
    current_year = datetime.datetime.now().year

    with fileinput.FileInput(files=paths, inplace=True) as input:
        for line in input:
            if res := re.search(copyright_regex, line):
                print(f"# Copyright (c) {res.group('first_year')}-{current_year} Morwenn")
            else:
                print(line.replace(old_version, new_version), end='')


def main():
    # Declare and parse arguments
    parser = argparse.ArgumentParser(description="Script to update relevant files with a new version number")
    parser.add_argument("new_version", help="new library version")
    parser.add_argument("--root", help="root of the library", default=Path(__file__).parents[1])
    args = parser.parse_args()

    root = Path(args.root)
    version_file = root / 'include' / 'cpp-sort' / 'version.h'

    # Isolate old and new version numbers
    old_version = read_version_number(version_file)
    new_version = args.new_version
    print(old_version, new_version)

    # TODO: error if new version < old version unless --force/-f

    # Replace the version number in appropriate files
    write_version_h(version_file, new_version)
    paths = [
        root / 'conanfile.py',
        root / 'CMakeLists.txt',
        root / 'README.md',
        root / 'docs' / 'Home.md',
        root / 'docs' / 'Tooling.md',
    ]
    replace_version_number(paths, old_version, new_version)


if __name__ == '__main__':
    main()