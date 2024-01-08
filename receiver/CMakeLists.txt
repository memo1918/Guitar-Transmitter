add_executable(
	receiver
	src/main.cpp
	src/receiver/Receiver.cpp
	src/player/Player.cpp
)
target_link_libraries(receiver
	pico_stdlib
	pico_multicore
	pico_audio_i2s
	hardware_gpio
	hardware_spi
	hardware_sync
	AudioPayload
	RF24
)

target_compile_definitions(
	receiver
	PRIVATE
	PICO_AUDIO_I2S_MONO_INPUT=1
	#PICO_AUDIO_I2S_MONO_OUTPUT=1
	PICO_AUDIO_BUFFERS_PER_CHANNEL=5
)

pico_add_extra_outputs(receiver)

pico_enable_stdio_usb(receiver 1)
