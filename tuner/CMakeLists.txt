add_executable(tuner 
src/main.cpp
src/signal_acquisition/signal_acq.cpp
)
target_link_libraries(tuner
	pico_stdlib
	hardware_spi
	hardware_gpio
    pico_multicore
	hardware_adc
	hardware_dma
	RF24
	YIN
)
pico_add_extra_outputs(tuner)

pico_enable_stdio_usb(tuner 1)

target_include_directories(tuner PUBLIC src/yin)

# target_link_libraries(receiver PUBLIC yin)

add_subdirectory(src/yin)
