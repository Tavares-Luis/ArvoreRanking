cmake_minimum_required(VERSION 3.10)
project(untitled7 C)

set(CMAKE_C_STANDARD 11)

add_executable(untitled7 src/main.c
        src/avl.c
        include/avl.h
        src/rubroNegra.c
        include/rubroNegra.h)