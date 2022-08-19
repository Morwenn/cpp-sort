# Copyright (c) 2019-2022 Morwenn
# SPDX-License-Identifier: MIT

# Add a selection of warnings to a target
macro(cppsort_add_warnings target)
    if (MSVC)
        target_compile_options(${target} PRIVATE /W2)
    else()
        target_compile_options(${target} PRIVATE
            -Wall -Wextra -Wcast-align -Winline -Wmissing-declarations -Wmissing-include-dirs
            -Wnon-virtual-dtor -Wodr -Wpedantic -Wredundant-decls -Wundef -Wunreachable-code
            -Wno-inline -Wdouble-promotion
            $<$<CXX_COMPILER_ID:GNU>:-Wlogical-op -Wuseless-cast -Wzero-as-null-pointer-constant>
            $<$<CXX_COMPILER_ID:GNU>:-Wduplicated-branches -Wduplicated-cond>
        )
    endif()
endmacro()
