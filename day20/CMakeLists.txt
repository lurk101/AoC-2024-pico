cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 23)

include(../dayboard.cmake)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(day20 C CXX ASM)

pico_sdk_init()

add_executable(day20 day20.cpp)

pico_set_program_name(day20 "day20")

pico_enable_stdio_uart(day20 1)
pico_enable_stdio_usb(day20 0)

target_link_libraries(day20 pico_stdlib)

pico_add_extra_outputs(day20)
