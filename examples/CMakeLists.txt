# Copyright (c) 2019-2022 Morwenn
# SPDX-License-Identifier: MIT

include(cpp-sort-utils)

# Quick & dirty script to compile the examples
foreach(filename bubble_sorter.cpp list_selection_sorter.cpp randomizing_adapter.cpp readme_example.cpp)
    get_filename_component(name ${filename} NAME_WE)
    add_executable(${name} ${filename})
    target_link_libraries(${name} PRIVATE cpp-sort::cpp-sort)
    cppsort_add_warnings(${name})
endforeach()
