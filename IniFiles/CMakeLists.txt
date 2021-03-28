cmake_minimum_required(VERSION 3.17)
project(IniFiles C)

set(CMAKE_C_STANDARD 99)

add_executable(IniFiles main.c functions.h functions.c structures.c structures.h)