/*
 * Copyright (c) 2020 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <iterator>
#include <string>

// Simple filesystem functions

auto safe_file_name(std::string filename)
    -> std::string
{
    char invalid_characters[] = {'/', '\\', ':', '*', '?', '"', '<', '>', '|'};

    // Replace characters to make a filename usable
    for (char& character : filename) {
        auto found_it = std::find(std::begin(invalid_characters),
                                  std::end(invalid_characters),
                                  character);
        if (found_it != std::end(invalid_characters)) {
            character = '_';
        }
    }

    return filename;
}
