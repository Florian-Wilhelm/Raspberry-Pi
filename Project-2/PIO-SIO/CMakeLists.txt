cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
project(pio-7segment C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
pico_sdk_init()

add_executable(pio-7segment
    pio-7segment.c
    )
    
pico_generate_pio_header(pio-7segment ${CMAKE_CURRENT_LIST_DIR}/ABCD.pio)

# pull in common dependencies
target_link_libraries(pio-7segment pico_stdlib hardware_pio)

# create map/bin/hex file etc.
pico_add_extra_outputs(pio-7segment)
