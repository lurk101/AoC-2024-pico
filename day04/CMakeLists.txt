cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

set(PICO_BOARD pico2)
set(PICO_PLATFORM rp2350)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(day04 C CXX ASM)

pico_sdk_init()

add_executable(day04 day04.cpp)

pico_set_program_name(day04 "day04")

pico_enable_stdio_uart(day04 1)
pico_enable_stdio_usb(day04 0)

target_link_libraries(day04 pico_stdlib)

pico_add_extra_outputs(day04)
