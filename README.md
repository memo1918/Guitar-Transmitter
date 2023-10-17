# Guitar Transmitter

Team-Members: Kai Mayer, Kevin Reifschneider, Maximilian Knobloch, Mehmet Sahin Uces

This project contains the source code and documentation for the cableless guitar transmitter. You can find a short description of what we are building in the following block:

As part of the course Embedded Systems (WS23/24) we are building a cableless guitar transmitter. We have a sender and a receiver that communicate over 2,4 GHz radio frequency. Plug in the sender into your guitar and the receiver into your amplifier, put both on and start playing. The sender will digitalize the input signal from the guitar and send it to the receiver. The receiver will turn the signal into an analog signal and put it into the amplifier. The reaction time to digitalize, send, receive and turn back into an analog signal again should take less than 5 ms.

## Hardware requirements

- 4x Raspberry Pi Pico ([Reichelt](https://www.reichelt.de/raspberry-pi-pico-rp2040-cortex-m0-microusb-rasp-pi-pico-p295706.html)) **4.55€ / Stk.**
- 4x Rasberry Pi Debug Probe ([Reichelt](https://www.reichelt.de/raspberry-pi-debug-probe-rpi-debug-probe-p343288.html)) **12,40€ / Stk.**
alternative: extra 4x Rasberry Pi Picos are also useable as a debugging probe.
- 4x RF Transceiver: NRF24L01 ([AZ-Delivery](https://www.az-delivery.de/en/products/3x-nrf24l01-mit-2-4-ghz)) **10.99€ / 5.Stk**
- 2x 6,35mm Jackplug male mono ([Maltronic](https://maltronic.de/detail/index/sArticle/35598)) **4,50€ / Stk.**
- 1x Digital-to-Analog Converter ([Reichelt](https://www.reichelt.de/de/de/entwicklerboards-dac-konverter-mcp4725-debo-dac-mcp4725-p334976.html)) **4,99€ / Stk.**
- 2x Batterycase ([Reichelt](https://www.reichelt.de/batteriehalter-fuer-3-microzellen-aaa-geschlossen-halter-3xaaa-p44627.html?&trstct=pol_0&nbc=1)) **2,15€ / Stk.**

   *^Links for reference*

## Hardware overview

![Hardware overview](./images/hardware-overview.png)

## Tasks

//TODO: add tasks

## Setup

### Mac OS

Tools:

```bash
brew install cmake 
brew install --cask gcc-arm-embedded
brew install picotool 
```

Build:

```bash
rm -rf build
cmake -B build -S .
make -C build
picotool load build/main.uf2
```

### Windows

Tools:

```powershell
winget install -e --id Kitware.CMake
winget install -e --id Arm.GnuArmEmbeddedToolchain
```

Build:

```powershell
rm -rf build
cmake -B build -S .
make -C build
```
