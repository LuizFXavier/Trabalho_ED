cmake_minimum_required(VERSION 3.27)

project(atv_2)

add_executable(atv_2
    src/kd-tree.c
    src/cidade.c
    src/hash.c
    src/technical.c
    src/main.c)
target_link_libraries(atv_2 m)
target_include_directories(atv_2 PRIVATE include)