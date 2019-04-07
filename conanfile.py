from conans import ConanFile


class CppSortConan(ConanFile):
    name = "cpp-sort"
    version = "1.4.0"
    settings = "compiler"
    license = "https://github.com/Morwenn/cpp-sort/blob/master/license.txt"
    url = "https://github.com/Morwenn/cpp-sort"
    author = "Morwenn <morwenn29@hotmail.fr>"
    description = "Additional sorting algorithms & related tools"
    exports_sources = "include/*"
    exports = "license.txt"
    no_copy_source = True

    def configure(self):
        if self.settings.compiler == "Visual Studio":
            raise Exception("Visual Studio is not supported.")

    def package(self):
        self.copy("license*", dst="licenses", ignore_case=True, keep_path=False)
        self.copy(pattern="*", src="include", dst="include")

    def package_id(self):
        self.info.header_only()
