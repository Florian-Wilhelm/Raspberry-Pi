cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)
project(DAC_MCP23016 C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(DAC_MCP23016
        DAC_MCP23016.c
        )

# pull in common dependencies
target_link_libraries(DAC_MCP23016 pico_stdlib hardware_i2c)

# create map/bin/hex file etc.
pico_add_extra_outputs(DAC_MCP23016)
