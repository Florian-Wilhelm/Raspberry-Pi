cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)
project(IoT-Pico--HIH4020-GP26-103AT2-GP27-SFH300-GP28 C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(IoT-Pico--HIH4020-GP26-103AT2-GP27-SFH300-GP28
        IoT-Pico--HIH4020-GP26-103AT2-GP27-SFH300-GP28.c
        )

# pull in common dependencies
target_link_libraries(IoT-Pico--HIH4020-GP26-103AT2-GP27-SFH300-GP28 pico_stdlib hardware_adc)

# create map/bin/hex file etc.
pico_add_extra_outputs(IoT-Pico--HIH4020-GP26-103AT2-GP27-SFH300-GP28)
