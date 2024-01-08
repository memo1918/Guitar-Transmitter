#include <stdio.h>
#include <pico/stdlib.h>
#include <RF24.h>
#include <pico.h>
#include <pico/util/queue.h>
#include <hardware/sync.h>
#include <pico/lock_core.h>

#include "signal_acquisition/signal_acq.h"

#define CE_PIN 7  // chip enable pin for the transceiver (controlls RX or TX role)
#define CSN_PIN 8 // SPI chip select

#define SCK 18 // Serial Clock
#define TX 19  // MOSI Master Out / Slave In
#define RX 16  // MISO Master In  / Slave Out

#define AUDIO_FREQUENCY 20'000 // Sample Rate of the complete system

#define CAPTURE_BUFFER_SIZE 16
static uint8_t capture_buffer_a[CAPTURE_BUFFER_SIZE];
static uint8_t capture_buffer_b[CAPTURE_BUFFER_SIZE];

static RF24 radio(CE_PIN, CSN_PIN, 10'000'000);
static SPI spi;

/**
 * configures the nrf24l01 transceiver
 */
void rf24Setup()
{
	radio.setPALevel(RF24_PA_MAX); // (PA) Power Amplifier on the transceiver
	radio.setPayloadSize(16);	   // each payload contains 16 bytes of audio data
	radio.setDataRate(RF24_2MBPS); // (MBPS) Mega Bit Per Second
	radio.setAutoAck(false);	   // (ACK) acknowledge
	radio.disableCRC();			   // (CRC) cyclic redundancy check for receiving payloads

	uint64_t address = 0x314e6f646520; // set the TX address of the RX node into the TX pipe
	radio.openWritingPipe(address);	   // always uses pipe 0
	radio.stopListening();			   // Trasmit only

	radio.printPrettyDetails();
	printf("[ INFO ] nrf25l01 setup completed\n");
}

int main()
{
	stdio_init_all(); // enables important standart input/output functionality espasily the serial port

	printf("Guitar-Transmitter - Transmitter");

	spi.begin(spi0, SCK, TX, RX); // start the communication over SPI to communicate with the transceiver
	if (!radio.begin(&spi))
	{
		printf("[ ERROR ] the nrf24l01 is not responding\n");
		while (true)
		{
			printf("[ ERROR ] the nrf24l01 was not initaziled successfully and will not tried again\n");
			sleep_ms(500);
		}
	}

	rf24Setup(); // configures the nrf24l01 transceiver

	auto dma_channel = sig_acq_init(AUDIO_FREQUENCY, capture_buffer_a, CAPTURE_BUFFER_SIZE);

	// main transmission routine
	while (true)
	{
		// switch from a buffer to b to send parallel a while b is filling
		dma_channel_set_write_addr(dma_channel, capture_buffer_b, true);

		uint32_t int_state = save_and_disable_interrupts();
		radio.startFastWrite(capture_buffer_a, CAPTURE_BUFFER_SIZE, true); // send buffer a with the transceiver
		restore_interrupts(int_state);

		dma_channel_wait_for_finish_blocking(dma_channel); // wait till buffer b is filled

		// switch from b buffer to a to send parallel b while a is now filling
		dma_channel_set_write_addr(dma_channel, capture_buffer_a, true);

		int_state = save_and_disable_interrupts();
		radio.startFastWrite(capture_buffer_b, CAPTURE_BUFFER_SIZE, true); // send buffer b with the transceiver
		restore_interrupts(int_state);

		dma_channel_wait_for_finish_blocking(dma_channel); // wait till buffer a is filled
	}
}