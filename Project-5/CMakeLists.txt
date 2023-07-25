cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)
project(IoT-port78--HIH4020-GP26--103AT2-GP27 C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(IoT-port78--HIH4020-GP26--103AT2-GP27
        IoT-port78--HIH4020-GP26--103AT2-GP27.c
        )

# pull in common dependencies
target_link_libraries(IoT-port78--HIH4020-GP26--103AT2-GP27 pico_stdlib hardware_adc)

# create map/bin/hex file etc.
pico_add_extra_outputs(IoT-port78--HIH4020-GP26--103AT2-GP27)
