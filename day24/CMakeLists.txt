cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 23)

include(../dayboard.cmake)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(day24 C CXX ASM)

pico_sdk_init()

add_executable(day24 day24.cpp)

pico_set_program_name(day24 "day24")

pico_enable_stdio_uart(day24 1)
pico_enable_stdio_usb(day24 0)

target_link_libraries(day24 pico_stdlib)

pico_add_extra_outputs(day24)
