# -*- coding: utf-8 -*-

# Copyright (c) 2018-2023 Morwenn
# SPDX-License-Identifier: MIT

import os.path

from conan import ConanFile
from conan.tools.build import check_min_cppstd
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout
from conan.tools.files import copy
from conan.tools.microsoft import is_msvc

required_conan_version = ">=1.50.0"


class CppSortConan(ConanFile):
    name = "cpp-sort"
    version = "1.15.0"
    description = "Sorting algorithms & related tools"
    license = "MIT"
    url = "https://github.com/Morwenn/cpp-sort"
    homepage = url
    topics = "cpp-sort", "sorting", "algorithms"
    author = "Morwenn <morwenn29@hotmail.fr>"

    # Minimal export (excludes tests, coverage, etc.)
    exports = ["LICENSE.txt", "NOTICE.txt"]
    exports_sources = [
        "include/*",
        "CMakeLists.txt",
        "cmake/cpp-sort-config.cmake.in"
    ]
    settings = "os", "compiler", "build_type", "arch"
    package_type = "header-library"
    no_copy_source = True

    def validate(self):
        if self.settings.get_safe("compiler.cppstd"):
            check_min_cppstd(self, 14)

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["BUILD_TESTING"] = "OFF"
        tc.generate()

    def package(self):
        # Install with CMake
        cmake = CMake(self)
        cmake.configure()
        cmake.install()

        # Copy license files
        for file in ["LICENSE.txt", "NOTICE.txt"]:
            copy(self, file, self.recipe_folder, os.path.join(self.package_folder, "licenses"), keep_path=False)

    def package_info(self):
        self.cpp_info.set_property("cmake_file_name", "cpp-sort")
        self.cpp_info.set_property("cmake_target_name", "cpp-sort::cpp-sort")
        self.cpp_info.bindirs = []
        self.cpp_info.libdirs = []

        if is_msvc(self):
            self.cpp_info.cxxflags = ["/permissive-", "/Zc:preprocessor"]

    def package_id(self):
        self.info.clear()  # Header-only
