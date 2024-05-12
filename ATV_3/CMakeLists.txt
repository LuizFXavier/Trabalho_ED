cmake_minimum_required(VERSION 3.27)

project(atv_3)

add_executable(atv_3
    src/kd-tree.c
    src/cidade.c
    src/hash.c
    src/hash_LL.c
    src/technical.c
    src/main.c)
target_link_libraries(atv_3 m)
target_include_directories(atv_3 PRIVATE include)