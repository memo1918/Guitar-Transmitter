#define CE_PIN 7
#define CSN_PIN 8
#define IRQ_PIN 6

#include <stdio.h>
#include <pico/stdlib.h>
#include <RF24.h>

RF24 radio(CE_PIN, CSN_PIN, 1000000);
SPI spi;

float payload = 0.0;

int main()
{
	stdio_init_all();
	sleep_ms(10000);
	printf("Guitar-Transmitter - Receiver");

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
	radio.setPALevel(RF24_PA_LOW);
	radio.setPayloadSize(sizeof(payload));
	uint64_t address = 0x314e6f646520;
	// set the TX address of the RX node into the TX pipe
	radio.openWritingPipe(address); // always uses pipe 0

	// set the RX address of the TX node into a RX pipe
	radio.openReadingPipe(1, address); // using pipe 1
	radio.startListening();
	radio.setDataRate(RF24_250KBPS);

	printf("nrf25l01 setup completed\n");
	radio.printDetails();
	radio.printPrettyDetails();

	while (true)
	{
		uint8_t pipe;
		if (radio.available(&pipe))					// waits until payload is available
		{											// is there a payload? get the pipe number that recieved it
			uint8_t bytes = radio.getPayloadSize(); // get the size of the payload
			radio.read(&payload, bytes);			// fetch payload from FIFO

			// print the size of the payload, the pipe number, payload's value
			printf("Received %d bytes on pipe %d: %f\n", bytes, pipe, payload);
		}
	}
}