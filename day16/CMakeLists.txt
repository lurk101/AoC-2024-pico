cmake_minimum_required(VERSION 3.16)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 23)

include(../dayboard.cmake)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(day16 C CXX ASM)

pico_sdk_init()

add_executable(day16 day16.c)

pico_set_program_name(day16 "day16")

pico_enable_stdio_uart(day16 1)
pico_enable_stdio_usb(day16 0)

target_link_libraries(day16 pico_stdlib)

pico_add_extra_outputs(day16)
