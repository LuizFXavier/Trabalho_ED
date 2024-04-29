cmake_minimum_required(VERSION 3.27)

project(atv_1)

add_executable(atv_1
    src/hash.c
    src/technical.c
    src/main.c)

target_include_directories(atv_1 PRIVATE include)