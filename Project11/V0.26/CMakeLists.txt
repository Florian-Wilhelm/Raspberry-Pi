cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
project(thermal-imaging C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
pico_sdk_init()

add_executable(thermal-imaging
    thermal-imaging.c
    )

# pull in common dependencies
target_link_libraries(thermal-imaging hardware_clocks pico_stdlib hardware_i2c hardware_pwm)

# create map/bin/hex file etc.
pico_add_extra_outputs(thermal-imaging)
