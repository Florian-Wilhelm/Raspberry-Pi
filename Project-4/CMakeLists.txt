cmake_minimum_required(VERSION 3.13)
set(PICO_BOARD pico_w)
include(pico_sdk_import.cmake)
project(bmp280 C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(bmp280
        bmp280.c
        )

# pull in common dependencies
target_link_libraries(bmp280 pico_stdlib hardware_i2c hardware_clocks)

# create map/bin/hex file etc.
pico_add_extra_outputs(bmp280)
