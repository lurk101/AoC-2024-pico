cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 23)

include(../dayboard.cmake)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(day08 C CXX ASM)

pico_sdk_init()

add_executable(day08 day08.cpp)

pico_set_program_name(day08 "day08")

pico_enable_stdio_uart(day08 1)
pico_enable_stdio_usb(day08 0)

target_link_libraries(day08 pico_stdlib)

pico_add_extra_outputs(day08)
