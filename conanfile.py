# -*- coding: utf-8 -*-

# Copyright (c) 2018-2021 Morwenn
# SPDX-License-Identifier: MIT

from conans import CMake, ConanFile


class CppSortConan(ConanFile):
    name = "cpp-sort"
    version = "1.9.0"
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
        if self.settings.get_safe("compiler.cppstd"):
            tools.check_min_cppstd(self, 14)

    def package(self):
        # Install with CMake
        cmake = CMake(self)
        cmake.definitions["BUILD_TESTING"] = "OFF"
        cmake.configure()
        cmake.install()
        cmake.patch_config_paths()

        # Copy license files
        for file in ["LICENSE.txt", "NOTICE.txt"]:
            self.copy(file, dst="licenses")

    def package_id(self):
        self.info.header_only()
