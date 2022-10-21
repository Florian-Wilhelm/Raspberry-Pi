cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)
project(pioBlinky C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(pio
        pio.c
        )
        
pico_generate_pio_header(pio ${CMAKE_CURRENT_LIST_DIR}/sqwave.pio)

# pull in common dependencies
target_link_libraries(pio pico_stdlib hardware_pio)

# create map/bin/hex file etc.
pico_add_extra_outputs(pio)
