add_executable(transmitter src/main.cpp)
target_link_libraries(
	transmitter
	pico_stdlib
	hardware_spi
	hardware_gpio
	freertos_kernel
	freertos_config
	RF24
)
pico_add_extra_outputs(transmitter)

pico_enable_stdio_usb(transmitter 1)
