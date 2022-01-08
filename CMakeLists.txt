cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(spd_rw C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

add_compile_options(-Wall
        -Wno-format
        -Wno-unused-function
        -Wno-maybe-uninitialized
        )

add_subdirectory(examples)
