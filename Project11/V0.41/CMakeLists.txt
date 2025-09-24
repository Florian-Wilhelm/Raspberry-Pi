cmake_minimum_required(VERSION 3.13)
set(PICO_BOARD pico_w)
include(pico_sdk_import.cmake)
project(thermal-imaging C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(thermal-imaging
    thermal-imaging.c
    )
    
target_include_directories(thermal-imaging PRIVATE
${CMAKE_CURRENT_LIST_DIR}
)

# pull in common dependencies
target_link_libraries(thermal-imaging pico_stdlib hardware_i2c pico_cyw43_arch_lwip_threadsafe_background)

# create map/bin/hex file etc.
pico_add_extra_outputs(thermal-imaging)
