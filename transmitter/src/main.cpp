#define CE_PIN 7
#define CSN_PIN 8
#define IRQ_PIN 6

#include <stdio.h>
#include <pico/stdlib.h>
#include <RF24.h>
#include <pico.h>
#include <pico/util/queue.h>
#include <hardware/sync.h>
#include <pico/lock_core.h>
#include "transmitter/Transmitter.h"

#include "signal_acquisition/signal_acq.h"

RF24 radio(CE_PIN, CSN_PIN, 1000000);
SPI spi;
queue_t queue;
Transmitter transmitter(queue, radio);

void rf24Setup()
{
	radio.setPALevel(RF24_PA_HIGH);
	radio.setPayloadSize(sizeof(uint8_t));
	radio.setDataRate(RF24_2MBPS);

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

	queue_init(&queue, 1, 1024);
	sig_acq_init(&queue);

	sleep_ms(10000);
	printf("Guitar-Transmitter - Transmitter");

	queue_init(&queue, sizeof(uint8_t), 1);

	spi.begin(spi0, 18, 19, 16);
	if (!radio.begin(&spi))
	{
		printf("[ ERROR ] the nrf24l01 is not responding\n");
		while (true)
		{
			printf("[ ERROR ] the nrf24l01 was not initaziled successfully ans will not tried again\n");
			sleep_ms(500);
		}
	}
	rf24Setup();

	while (true)
	{
		uint8_t payload = 14;
		bool success = queue_try_add(&queue, &payload);
		if (!success)
		{
			printf("could not add to queue!\n");
		}

		transmitter.run();
		sleep_ms(200);
	}
}