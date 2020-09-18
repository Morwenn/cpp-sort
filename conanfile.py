# -*- coding: utf-8 -*-

from conans import CMake, ConanFile


class CppSortConan(ConanFile):
    name = "cpp-sort"
    version = "1.7.0"
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

    def package(self):
        # Install with CMake
        cmake = CMake(self)
        cmake.definitions["BUILD_TESTING"] = "OFF"
        cmake.configure()
        cmake.install()
        cmake.patch_config_paths()

        # Copy license files
        self.copy("LICENSE.txt", dst="licenses")
        self.copy("NOTICE.txt", dst="licenses")

    def package_id(self):
        self.info.header_only()
