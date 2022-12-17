cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)
project(geigerCounter C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(geigerCounter
        geigerCounter.c
        )

# pull in common dependencies
target_link_libraries(geigerCounter pico_stdlib hardware_i2c hardware_pwm)

# create map/bin/hex file etc.
pico_add_extra_outputs(geigerCounter)
