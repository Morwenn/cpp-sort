# Copyright (c) 2019-2020 Morwenn
# SPDX-License-Identifier: MIT

# Add a selection of warnings to a target
macro(cppsort_add_warnings target)
    if (MSVC)
        target_compile_options(${target} PRIVATE /W2)
    else()
        target_compile_options(${target} PRIVATE
            -Wall -Wextra -Wcast-align -Winline -Wmissing-declarations -Wmissing-include-dirs
            -Wnon-virtual-dtor -Wodr -Wpedantic -Wredundant-decls -Wundef -Wunreachable-code
            $<$<CXX_COMPILER_ID:GNU>:-Wlogical-op -Wuseless-cast -Wzero-as-null-pointer-constant>
            # The warning when initializing an std::array is just too much of a bother
            $<$<CXX_COMPILER_ID:Clang>:-Wno-missing-braces>
        )
    endif()
endmacro()
