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
#include <unistd.h>
#include <math.h>

#define PAYLOAD_SIZE 16
#define BUFFER_SIZE 256

RF24 radio(CE_PIN, CSN_PIN, 10'000'000);
SPI spi;
Receiver receiver(radio, PAYLOAD_SIZE);
Player player;
int bytesInBuffer = 0;

void rf24Setup()
{
	radio.setPALevel(RF24_PA_MAX);
	radio.setPayloadSize(PAYLOAD_SIZE);
	radio.setDataRate(RF24_2MBPS);
	radio.setAutoAck(false);
	radio.disableCRC();
	// radio.setCRCLength(RF24_CRC_8);
	radio.maskIRQ(true, true, false);

	uint64_t address = 0x314e6f646520;
	// radio.openWritingPipe(address); // always uses pipe 0
	//  set the RX address of the TX node into a RX pipe
	radio.openReadingPipe(0, address); // using pipe 1
	radio.startListening();

	radio.printPrettyDetails();
	printf("[ INFO ] nrf25l01 setup completed\n");
}

void onPayloadAvailable(uint gpio, uint32_t events)
{
	receiver.setDataAvailable();
}

uint payloadnum = 0;
uint8_t payload[256];
repeating_timer_t timer;

#define SINE_WAVE_TABLE_LEN 48
uint32_t pos = 0;
uint32_t pos_max = SINE_WAVE_TABLE_LEN;
uint vol = 255;
static int16_t sine_wave_table[SINE_WAVE_TABLE_LEN];
int tracePos = 0;

bool onNextFrameExpected(repeating_timer_t *rt)
{
	payloadnum += PAYLOAD_SIZE;
	if (payloadnum >= BUFFER_SIZE)
	{
		/*tracePos++;
		if (tracePos > 8)
		{
			for (int i = 0; i < 1024; i++)
			{
				printf("%d,", payload[i]);
			}
			printf("\n");
		}*/
		player.play(payload);
		memset(payload, 127, BUFFER_SIZE);
		payloadnum = 0;
	}
	return true;
}

int main()
{
	stdio_init_all();

	gpio_init(20);
	gpio_set_dir(20, GPIO_OUT);

	// gpio_set_irq_enabled_with_callback(IRQ_PIN, GPIO_IRQ_EDGE_FALL, true, onPayloadAvailable);
	printf("[ BOOT ] Guitar-Transmitter - Receiver");

	for (int i = 0; i < SINE_WAVE_TABLE_LEN; i++)
	{
		sine_wave_table[i] = 128 * cosf(i * 2 * (float)(M_PI / SINE_WAVE_TABLE_LEN)); // 45 bytes ber period
	}

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

	memset(payload, 127, BUFFER_SIZE);
	if (!add_repeating_timer_us(-1'000'000 / 1250, onNextFrameExpected, NULL, &timer))
	{
		printf("Failed to add timer\n");
		return 1;
	}

	while (true)
	{
		// if (receiver.isDataAvailable())
		//{
		if (radio.available())
		{
			uint32_t state = save_and_disable_interrupts();
			radio.read(&payload[payloadnum], PAYLOAD_SIZE);
			// memset(&payload[payloadnum], 255, 16);
			//  memcpy(&payload[payloadnum], &sine_wave_table[pos], 16);
			/*pos++;
			if (pos >= pos_max)
			{
				pos = 0;
			}*/
			restore_interrupts(state);
		}
		// receiver.read(&payload[payloadnum]);

		// memcpy(&payload[payloadnum], sine_wave_table, 16);
		//}
	}
}