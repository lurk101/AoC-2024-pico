cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 23)

include(../dayboard.cmake)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(day23 C CXX ASM)

pico_sdk_init()

add_executable(day23 day23.cpp)

pico_set_program_name(day23 "day23")

pico_enable_stdio_uart(day23 1)
pico_enable_stdio_usb(day23 0)

target_link_libraries(day23 pico_stdlib)

pico_add_extra_outputs(day23)
