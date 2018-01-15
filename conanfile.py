from conans import ConanFile


class CppSortConan(ConanFile):
    name = "cpp-sort"
    version = "1.0.0"
    license = "MIT"
    url = "https://github.com/Morwenn/cpp-sort"
    description = "Additional sorting algorithms & related tools"
    exports_sources = "include/*"
    no_copy_source = True

    def package(self):
        self.copy(pattern="*", src="include", dst="include")

    def package_id(self):
        self.info.header_only()
