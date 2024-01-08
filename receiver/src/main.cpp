#define CE_PIN 7		  // chip enable pin for the RF24
#define CSN_PIN 8		  // chip select pin for the RF24
#define IRQ_PIN 6		  // interrupt pin from the RF24
#define AUDIO_DIN_PIN 21  // data in pin for the i2s audio digtal analog converter
#define AUDIO_BCLK_PIN 22 // bit clock pin for the i2s audio digtal analog converter

#define AUDIOFRAME_SIZE 16
#define AUDIOBUFFER_SIZE 256
#define RF24_SPI_SPEED 10'000'000

#include <stdio.h>
#include <pico/stdlib.h>
#include <RF24.h>

#include "player/Player.h"

RF24 radio(CE_PIN, CSN_PIN, RF24_SPI_SPEED);
SPI spi;
Player player(AUDIOBUFFER_SIZE);

// The address used on RF24 (TX and RX) must be the same.
const uint64_t address = 0x314e6f646520;

uint payloadFrameId = 0;
uint8_t payload[256];
repeating_timer_t timer;

/**
 * @brief Setup the RF24 device for audio transmission
 *
 * The sender and receiver have to use the same settings. We decided to disable
 * the CRC check, because it is not needed for audio transmission. It is easier
 * to filter/compress a corrupted byte than to interpolate a complete frame.
 * We also disable the auto ack feature, because it takes way to long to try
 * retransmitt a missing or corrupted frame.
 */
void rf24Setup()
{
	radio.setPALevel(RF24_PA_MAX);
	radio.setPayloadSize(AUDIOFRAME_SIZE);
	radio.setDataRate(RF24_2MBPS);
	radio.setAutoAck(false);
	radio.disableCRC();
	radio.maskIRQ(true, true, false);

	// Open a reading pipe on the given address
	radio.openReadingPipe(0, address);
	radio.startListening();

	radio.printPrettyDetails();
	printf("[ INFO ] nrf25l01 setup completed\n");
}

/**
 * @brief Callback, when the the time for the next frame is reached
 *
 * @param rt the repeating timer that called this function
 * @return true in order  to keep the timer running
 */
bool onNextFrameExpected(repeating_timer_t *rt)
{
	payloadFrameId += AUDIOFRAME_SIZE;
	if (payloadFrameId >= AUDIOBUFFER_SIZE)
	{
		// play the next buffer if enough frames are received, to fill a buffer
		player.play(payload);
		// reset the buffer affter playing it, to avoid keeping old frames in case a frame is missing
		memset(payload, 127, AUDIOBUFFER_SIZE);
		// fill the buffer from the beginning
		payloadFrameId = 0;
	}
	return true;
}

int main()
{
	stdio_init_all();
	printf("[ BOOT ] Guitar-Transmitter - Receiver");

	// initalize the modules used to receive and play audio
	player.begin();
	spi.begin(spi0, 18, 19, 16);
	if (!radio.begin(&spi))
	{
		// in here the program will panic, if the nrf24l01 is not responding.
		// A possible reason for this is, that the nrf24l01 is not connected correctly.
		printf("[ ERROR ] the nrf24l01 is not responding\n");
		panic("[ ERROR ] the nrf24l01 was not initaziled successfully");
	}
	rf24Setup();
	// init the buffer with 127, which is the middle of the range of the
	// int8_t (0 to 255)
	memset(payload, 127, AUDIOBUFFER_SIZE);

	// setup the timer to call onNextFrameExpected every 1/1250 seconds, which
	// is the time for one audio frame of 16 bytes with a sample rate of 20kHz
	// 1/1250 = 0.0008 = 800us
	if (!add_repeating_timer_us(-1'000'000 / 1250, onNextFrameExpected, NULL, &timer))
	{
		panic("[ ERROR ] Failed to add timer\n");
	}

	while (true)
	{
		// check if a new frame was received, by the RF24
		if (radio.available())
		{
			// disable interrupts while reading from the RF24, to avoid start
			// to playing a frame, while it is still being read
			uint32_t state = save_and_disable_interrupts();
			// read the frame into the buffer, to the position of the current
			// payloadFrameId, which is incremented by AUDIOFRAME_SIZE at a
			// specific interval, to avoid an audio underrun in case of missing
			// a frame
			radio.read(&payload[payloadFrameId], AUDIOFRAME_SIZE);
			restore_interrupts(state);
		}
	}
}
