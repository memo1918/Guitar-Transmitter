add_executable(
	receiver
	src/main.cpp
	src/player/Player.cpp
)
target_link_libraries(receiver
	pico_stdlib
	pico_audio_i2s
	hardware_gpio
	hardware_spi
	RF24
)

target_compile_definitions(
	receiver
	PRIVATE
	PICO_AUDIO_I2S_MONO_INPUT=1

)

pico_add_extra_outputs(receiver)

pico_enable_stdio_usb(receiver 1)
