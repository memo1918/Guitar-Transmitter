add_executable(transmitter src/main.c)
target_link_libraries(transmitter pico_stdlib hardware_adc)
pico_add_extra_outputs(transmitter)

pico_enable_stdio_usb(transmitter 1)
