# -*- coding: utf-8 -*-

from conans import CMake, ConanFile


class CppSortConan(ConanFile):
    name = "cpp-sort"
    version = "1.6.0"
    description = "Additional sorting algorithms & related tools"
    topics = "conan", "cpp-sort", "sorting", "algorithms"
    url = "https://github.com/Morwenn/cpp-sort"
    homepage = url
    license = "MIT"
    author = "Morwenn <morwenn29@hotmail.fr>"

    # Minimal export (excludes tests, coverage, etc.)
    exports_sources = [
        "include/*",
        "CMakeLists.txt",
        "cmake/cpp-sort-config.cmake.in",
        "license.txt"
    ]
    no_copy_source = True

    def package(self):
        # Install with CMake
        cmake = CMake(self)
        cmake.definitions["BUILD_TESTING"] = "OFF"
        cmake.configure()
        cmake.install()
        cmake.patch_config_paths()

        # Copy license file
        self.copy("license.txt", dst="licenses")

    def package_id(self):
        self.info.header_only()
