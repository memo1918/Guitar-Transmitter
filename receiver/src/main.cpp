#define CE_PIN 7
#define CSN_PIN 8
#define IRQ_PIN 6
#define RECEIVE_LED_PIN 20
#define AUDIO_DIN_PIN 21
#define AUDIO_BCLK_PIN 22

#include <stdio.h>
#include <pico/stdlib.h>
#include <pico/util/queue.h>
#include <pico/multicore.h>
#include <pico/util/queue.h>
#include <AudioPayload.h>
#include <RF24.h>
#include "receiver/Receiver.h"
#include "player/Player.h"

RF24 radio(CE_PIN, CSN_PIN, 10'000'000);
SPI spi;
Receiver receiver(radio, RECEIVE_LED_PIN);
queue_t queue;
Player player(queue);
int bytesInBuffer = 0;

void rf24Setup()
{
	radio.setPALevel(RF24_PA_HIGH);
	radio.setPayloadSize(receiver.getPayloadSize());
	radio.setDataRate(RF24_2MBPS);
	radio.setAutoAck(false);
	radio.setCRCLength(RF24_CRC_8);

	uint64_t address = 0x314e6f646520;
	radio.openWritingPipe(address); // always uses pipe 0
	// set the RX address of the TX node into a RX pipe
	radio.openReadingPipe(1, address); // using pipe 1
	radio.startListening();

	radio.printPrettyDetails();
	printf("[ INFO ] nrf25l01 setup completed\n");
}

void onPayloadAvailable(uint gpio, uint32_t events)
{
	receiver.setDataAvailable();
}

int main()
{
	stdio_init_all();
	queue_init(&queue, sizeof(uint8_t), 24000);

	gpio_set_irq_enabled_with_callback(IRQ_PIN, GPIO_IRQ_EDGE_FALL, true, onPayloadAvailable);
	printf("[ BOOT ] Guitar-Transmitter - Receiver");

	player.begin();

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

	AudioPayload payload;
	uint8_t lastPayloadId = 0;
	bool play = false;
	while (true)
	{
		if (receiver.isDataAvailable())
		{
			bool success = receiver.read(&payload);
			if (!success)
			{
				printf("[ ERROR ] Receiver: Unable to read payload\n");
				continue;
			}
			if (payload.id == lastPayloadId)
			{
				continue;
			}
			gpio_put(RECEIVE_LED_PIN, true);

			lastPayloadId = payload.id;

			for (uint i = 0; i < sizeof(AudioPayload::bytes); i++)
			{
				bool success = queue_try_add(&queue, &payload.bytes[i]);
				if (!success)
				{
					printf("[ WARNING ] Receiver: Audio overflow occoured \n");
					break;
				}
				bytesInBuffer++;
			}
			if (bytesInBuffer > 10000)
			{
				play = true;
			}

			// player.play(payload);
			gpio_put(RECEIVE_LED_PIN, false);
		}

		if (play)
		{
			player.run();
			bytesInBuffer -= 256;
		}
	}
}