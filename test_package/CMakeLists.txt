# Copyright (c) 2018-2023 Morwenn
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 2.8.11)

project(test_package LANGUAGES CXX)

find_package(cpp-sort REQUIRED CONFIG)

add_executable(${PROJECT_NAME} cpp-sort-integrity.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE cpp-sort::cpp-sort)
