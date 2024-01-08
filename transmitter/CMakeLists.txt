add_executable(
	transmitter
	src/main.cpp
	src/signal_acquisition/signal_acq.cpp
)

target_link_libraries(
	transmitter
	pico_stdlib
	hardware_adc
	hardware_dma
	hardware_spi
	hardware_gpio
	RF24
)

pico_add_extra_outputs(transmitter)

pico_enable_stdio_usb(transmitter 1)
