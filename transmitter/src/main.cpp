#define CE_PIN 7
#define CSN_PIN 8
#define IRQ_PIN 6
// #define AUDIO_FREQUENCY 44'100 // 44,1 kHz
#define AUDIO_FREQUENCY 20'000

#include <stdio.h>
#include <pico/stdlib.h>
#include <RF24.h>
#include <pico.h>
#include <pico/util/queue.h>
#include <hardware/sync.h>
#include <pico/lock_core.h>
#include <AudioPayload.h>
#include "transmitter/Transmitter.h"

#include "signal_acquisition/signal_acq.h"

RF24 radio(CE_PIN, CSN_PIN, 10'000'000);
SPI spi;
queue_t queue;
Transmitter transmitter(queue, radio, sizeof(AudioPayload::bytes));

void rf24Setup()
{
	radio.setPALevel(RF24_PA_MAX);
	// radio.setPayloadSize(transmitter.getPayloadSize());
	radio.setPayloadSize(16);
	radio.setDataRate(RF24_2MBPS);
	radio.setAutoAck(false);
	radio.setCRCLength(RF24_CRC_8);

	uint64_t address = 0x314e6f646520;
	// set the TX address of the RX node into the TX pipe
	radio.openWritingPipe(address); // always uses pipe 0
	// set the RX address of the TX node into a RX pipe
	radio.openReadingPipe(1, address); // using pipe 1
	radio.stopListening();			   // Trasmit only

	radio.printPrettyDetails();
	printf("[ INFO ] nrf25l01 setup completed\n");
}

int main()
{
	stdio_init_all();
	queue_init(&queue, sizeof(AudioPayload::bytes), 1024);

	gpio_init(20);
	gpio_set_dir(20, GPIO_OUT);

	printf("Guitar-Transmitter - Transmitter");

	spi.begin(spi0, 18, 19, 16);
	if (!radio.begin(&spi))
	{
		// printf("[ ERROR ] the nrf24l01 is not responding\n");
		while (true)
		{
			// printf("[ ERROR ] the nrf24l01 was not initaziled successfully ans will not tried again\n");
			sleep_ms(500);
		}
	}
	rf24Setup();

	sig_acq_init(&queue, AUDIO_FREQUENCY, &radio);
	while (true)
	{
		tight_loop_contents();
		// transmitter.readAndSend();
	}
}