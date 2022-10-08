# -*- coding: utf-8 -*-

# Copyright (c) 2018-2022 Morwenn
# SPDX-License-Identifier: MIT

import os.path

from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout
from conan.tools.files import copy
from conans import tools

required_conan_version = ">=1.50.0"


class CppSortConan(ConanFile):
    name = "cpp-sort"
    version = "1.13.2"
    description = "Additional sorting algorithms & related tools"
    topics = "conan", "cpp-sort", "sorting", "algorithms"
    url = "https://github.com/Morwenn/cpp-sort"
    homepage = url
    license = "MIT"
    author = "Morwenn <morwenn29@hotmail.fr>"

    # Minimal export (excludes tests, coverage, etc.)
    exports = ["LICENSE.txt", "NOTICE.txt"]
    exports_sources = [
        "include/*",
        "CMakeLists.txt",
        "cmake/cpp-sort-config.cmake.in"
    ]
    no_copy_source = True
    settings = "os", "compiler", "build_type", "arch"

    def validate(self):
        if self.info.settings.get_safe("compiler.cppstd"):
            tools.check_min_cppstd(self, 14)

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
        self.cpp_info.names["cmake_find_package"] = "cpp-sort"
        self.cpp_info.names["cmake_find_package_multi"] = "cpp-sort"
        if self.info.settings.compiler == "Visual Studio":
            self.cpp_info.cxxflags = ["/permissive-"]

    def package_id(self):
        self.info.clear()  # Header-only
