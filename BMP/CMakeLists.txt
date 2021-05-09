cmake_minimum_required(VERSION 3.17)
project(BMP C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-std=c11")
add_executable(BMP main.c bmp_framework.h bmp_framework.c)