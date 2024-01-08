cmake_minimum_required(VERSION 3.12)

include(pico-sdk/pico_sdk_init.cmake)
include(pico-extras/external/pico_extras_import.cmake)

project(guitar)
pico_sdk_init()

add_subdirectory(lib)
add_subdirectory(transmitter)
add_subdirectory(receiver)
