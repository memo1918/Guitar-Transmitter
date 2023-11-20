add_executable(
	receiver
	src/main.cpp
	src/receiver/Receiver.cpp
)
target_link_libraries(receiver
	pico_stdlib
	hardware_gpio
	hardware_spi
	hardware_sync
	AudioPayload
	RF24
)
pico_add_extra_outputs(receiver)

pico_enable_stdio_usb(receiver 1)
