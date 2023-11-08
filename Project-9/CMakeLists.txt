cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)
project(hello-XOSC C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(hello-XOSC
        hello-XOSC.c
        )

# pull in common dependencies
target_link_libraries(hello-XOSC pico_stdlib hardware_i2c hardware_pwm hardware_clocks)

# create map/bin/hex file etc.
pico_add_extra_outputs(hello-XOSC)
