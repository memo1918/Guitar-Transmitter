add_executable(receiver src/main.c)
target_link_libraries(receiver pico_stdlib hardware_adc)
pico_add_extra_outputs(receiver)

pico_enable_stdio_usb(receiver 1)
