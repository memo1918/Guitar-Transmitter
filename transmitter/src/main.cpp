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
	printf("Guitar-Transmitter - Transmitter");

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

	radio.setPALevel(RF24_PA_HIGH);
	radio.setPayloadSize(sizeof(payload));
	radio.setDataRate(RF24_2MBPS);

	uint64_t address = 0x314e6f646520;
	// set the TX address of the RX node into the TX pipe
	radio.openWritingPipe(address); // always uses pipe 0
	// radio.setAutoAck(false);

	// set the RX address of the TX node into a RX pipe
	radio.openReadingPipe(1, address); // using pipe 1
	radio.stopListening();

	radio.printDetails();
	radio.printPrettyDetails();

	printf("nrf25l01 setup completed\n");
	sleep_ms(10000);

	while (true)
	{
		uint64_t start_timer = to_us_since_boot(get_absolute_time()); // start the timer
		bool report = radio.write(&payload, sizeof(payload));		  // transmit & save the report
		uint64_t end_timer = to_us_since_boot(get_absolute_time());	  // end the timer

		if (report)
		{
			// payload was delivered; print the payload sent & the timer result
			printf("Transmission successful! Time to transmit = %llu us. Sent: %f\n", end_timer - start_timer, payload);
		}
		else
		{
			// payload was not delivered
			printf("Transmission failed or timed out\n");
		}

		// increment float payload
		payload += 0.01;
		sleep_ms(200);
	}
}