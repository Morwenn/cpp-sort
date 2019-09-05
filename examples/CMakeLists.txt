# Quick & dirty script to compile the examples
foreach(filename bubble_sorter.cpp list_selection_sorter.cpp)
    get_filename_component(name ${filename} NAME_WE)
    add_executable(${name} ${filename})
    target_link_libraries(${name} PRIVATE
        Catch2::Catch2
        cpp-sort::cpp-sort
    )
    target_compile_options(${name} PRIVATE
        -Wall -Wextra -Wcast-align -Winline -Wmissing-declarations -Wmissing-include-dirs
        -Wnon-virtual-dtor -Wodr -Wpedantic -Wredundant-decls -Wundef -Wunreachable-code
        $<$<CXX_COMPILER_ID:GNU>:-Wlogical-op -Wuseless-cast -Wzero-as-null-pointer-constant>
        # The warning when initializing an std::array is just too much of a bother
        $<$<CXX_COMPILER_ID:Clang>:-Wno-missing-braces>
    )
endforeach()
