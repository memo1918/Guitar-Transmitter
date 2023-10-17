cmake_minimum_required(VERSION 3.12)

include(pico-sdk/pico_sdk_init.cmake)

project(guitar)
pico_sdk_init()

add_executable(main src/main.c)
target_link_libraries(main pico_stdlib hardware_adc)
pico_add_extra_outputs(main)

pico_enable_stdio_usb(main 1)
