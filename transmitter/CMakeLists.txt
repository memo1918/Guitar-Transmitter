add_executable(transmitter src/main.cpp)
target_link_libraries(
	transmitter
	pico_stdlib
	hardware_adc
	hardware_dma
	hardware_spi
	hardware_sync
	hardware_gpio
	RF24
	SIG_ACQ
)

pico_add_extra_outputs(transmitter)

pico_enable_stdio_usb(transmitter 1)

add_subdirectory(signal_acquisition)
target_include_directories(transmitter PUBLIC signal_acquisition/include)