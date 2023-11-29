#define CE_PIN 7
#define CSN_PIN 8
#define IRQ_PIN 6
#define RECEIVE_LED_PIN 20
#define AUDIO_DIN_PIN 21
#define AUDIO_BCLK_PIN 22

#include <stdio.h>
#include <pico/stdlib.h>
// #include <pico/util/queue.h>
#include <AudioPayload.h>
// #include <RF24.h>
// #include "receiver/Receiver.h"
#include "player/Player.h"

// RF24 radio(CE_PIN, CSN_PIN, 1000000);
// SPI spi;
// queue_t queue;
/// Receiver receiver(queue, radio, sizeof(AudioPayload), RECEIVE_LED_PIN);
Player player;

uint8_t payload = 0;

/* void rf24Setup()
{
	radio.setPALevel(RF24_PA_HIGH);
	radio.setPayloadSize(receiver.getPayloadSize());
	radio.setDataRate(RF24_2MBPS);
	radio.setAutoAck(false);

	uint64_t address = 0x314e6f646520;
	radio.openWritingPipe(address); // always uses pipe 0
	// set the RX address of the TX node into a RX pipe
	radio.openReadingPipe(1, address); // using pipe 1
	radio.startListening();

	radio.printPrettyDetails();
	printf("[ INFO ] nrf25l01 setup completed\n");
}*/

int main()
{
	stdio_init_all();
	// queue_init(&queue, receiver.getPayloadSize(), 1024);

	printf("[ BOOT ] Guitar-Transmitter - Receiver");

	player.begin();

	/* spi.begin(spi0, 18, 19, 16);
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
		// player.play();
		receiver.receiveAndEnqueue();
		AudioPayload payload;
		queue_remove_blocking(&queue, &payload);
	}*/
}