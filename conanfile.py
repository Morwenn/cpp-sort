from conans import CMake, ConanFile


class CppSortConan(ConanFile):
    name = "cpp-sort"
    version = "1.4.0"
    license = "https://github.com/Morwenn/cpp-sort/blob/master/license.txt"
    url = "https://github.com/Morwenn/cpp-sort"
    author = "Morwenn <morwenn29@hotmail.fr>"
    description = "Additional sorting algorithms & related tools"
    exports_sources = ("include/*", "CMakeLists.txt", "cmake/*")
    exports = "license.txt"
    generators = "cmake", "txt"
    no_copy_source = True

    def package(self):
        cmake = CMake(self)
        cmake.definitions["BUILD_TESTING"] = "OFF"
        cmake.configure()
        cmake.install()

        self.copy("license*", dst="licenses", ignore_case=True, keep_path=False)

    def package_id(self):
        self.info.header_only()
