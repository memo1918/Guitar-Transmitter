# Introduction

## Inspiration and Purpose

Electrical instruments were invented in the 1950s, but this step into the future also brought new problems. Electronic amplification was only possible with a cable connecting the amplifier and the instrument. As three of our four group members were musicians, we aimed to make the cable obsolete. Our vision was to provide future musicians with a seamless means of expressing their creativity, unrestricted by the limitations of cable movement. As our group consists mainly of guitarists, we have decided to focus on developing a wireless transceiver – a device allowing the attachment of a transmitter to the guitar and a receiver to the amplifier. This device will allow musicians to move freely without the need for a cable by plugging the transmitter into the guitar and the receiver into the amplifier.

## Evaluation Platform

### Hardware

Our hardware dependencies are as follows:

- Raspberry Pi Pico; <https://datasheets.raspberrypi.com/pico/pico-datasheet.pdf>
- Pi Pico built in ADC (Analog-to-Digital Converter from the rp2040),
- Radio frequency transceiver NRF24L01; <https://www.mouser.com/datasheet/2/297/nRF24L01_Product_Specification_v2_0-9199.pdf>
- DAC (digital-to-analog converter) MAX98357; <https://www.analog.com/media/en/technical-documentation/data-sheets/max98357a-max98357b.pdf>
- Raspberry Pi Debug Probe; <https://datasheets.raspberrypi.com/debug/raspberry-pi-debug-probe-product-brief.pdf>
- Guitar amplifier; <https://www.thomann.de/de/boss_katana_100_head_mkii.htm>

### Software

In our developing environment we use Visual Studio Code along with [Pico SDK](https://github.com/raspberrypi/pico-sdk) and OpenOCD.  The Project is built with CMake in C/C++ and relies on the Pico SDK. We used [OpneOCD](https://openocd.org) to connect the Pico Debug Probe and be able to debug the Pico and flash the system over the debug probe. To communicate with the RF24 radio module we used the RF24 library. The NRF24L01 has lots of different settings and modes to operate on, because the library implements writing the settings over the SPI interface the RF24 provides, we decided to use it. Ther alternative would be to implement the interface the chip provides on our self on the SPI interface the Pico SDK provides. For the I2S audio output we used a part of the Pico Extras. Mainly because we needed a custom implementation for the I2S protocol on the GPIO’s which does not exist on the Pico SDK, neither is natively supported on the Pico. The [Pico Extras](https://github.com/raspberrypi/pico-extras) contains an implementation of the [pio program](https://github.com/raspberrypi/pico-extras/blob/master/src/rp2_common/pico_audio_i2s/audio_i2s.pio) for an I2S interface.

So, to get an overview of the dependencies we use:

- Pico SDK; <https://github.com/raspberrypi/pico-sdk>
- Pico Extras; <https://github.com/raspberrypi/pico-extras/blob/master/src/rp2_common/pico_audio_i2s/audio_i2s.pio>
- RF24; <https://github.com/nRF24/RF24>
- OpenOCD (optional; for debugging); <https://openocd.org/>
