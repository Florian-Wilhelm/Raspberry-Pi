cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)
project(HIH4020-humidityMeasurement C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(HIH4020-humidityMeasurement
        HIH4020-humidityMeasurement.c
        )

# pull in common dependencies
target_link_libraries(HIH4020-humidityMeasurement pico_stdlib hardware_adc)

# create map/bin/hex file etc.
pico_add_extra_outputs(HIH4020-humidityMeasurement)
