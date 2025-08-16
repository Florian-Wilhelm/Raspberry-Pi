cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
project(pwm--motor-test C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
pico_sdk_init()

add_executable(pwm--motor-test
    pwm--motor-test.c
    )

# pull in common dependencies
target_link_libraries(pwm--motor-test pico_stdlib hardware_pwm hardware_clocks)

# create map/bin/hex file etc.
pico_add_extra_outputs(pwm--motor-test)
