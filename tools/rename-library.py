# -*- coding: utf-8 -*-

# Copyright (c) 2020 Morwenn
# SPDX-License-Identifier: MIT

"""
This script is mainly used to simplify benchmarks between different
versions of the library: it allows to rename every mention of the
library name, which helps to include two versions side-by-side
without conflicts, and to compare them in the same benchmark.
"""

import argparse
import fileinput
import fnmatch
import os
import sys

import pygit2

# Modify this dict to change the words to replace
REPLACEMENT_LIST = {
    'cpp-sort': 'cpp-sort-old',
    'cppsort': 'cppsort_old',
    'CPPSORT': 'CPPSORT_OLD'
}


def main():
    parser = argparse.ArgumentParser(description="Rename all the parts of the library that matter")
    parser.add_argument('root', type=str)
    args = parser.parse_args()

    repo_root = os.path.abspath(args.root)
    repo = pygit2.Repository(repo_root)
    os.chdir(repo_root)

    ##################################################    
    # Rename the library mentions in code
    
    # Files we are interested into match one of these patterns
    filename_patterns = [
        "*.h",
        "*.cpp",
        "CMakeLists.txt"
    ]

    for root, dirs, files in os.walk(repo_root):
        dirs[:] = [dir_path for dir_path in dirs
                   if not repo.path_is_ignored(dir_path)]

        for file in files:
            # Filter files
            if repo.path_is_ignored(file):
                continue
            if not any(fnmatch.fnmatch(file, pattern)
                       for pattern in filename_patterns):
                continue
            print("Modifying:", os.path.join(root, file))
            
            # Replace references to cpp-sort
            with fileinput.FileInput(os.path.join(root, file), inplace=True) as fd:
                for line in fd:
                    for old_str, new_str in REPLACEMENT_LIST.items():
                        line = line.replace(old_str, new_str)
                    print(line, end='')

    ##################################################
    # Rename the main include directory
    
    # Directory is removed after the files have been modified to make
    # sure that the .gitignore is valid when modifying said files
    old_dirname = os.path.join(repo_root, 'include', 'cpp-sort')
    new_dirname = os.path.join(repo_root, 'include', REPLACEMENT_LIST['cpp-sort'])
    os.rename(old_dirname, new_dirname)

if __name__ == '__main__':
    main()
