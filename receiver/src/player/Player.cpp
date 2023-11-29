#include "Player.h"
#include <math.h>

Player::Player()
{
}

Player::~Player()
{
}

#define SINE_WAVE_TABLE_LEN 24
#define SAMPLES_PER_BUFFER 256

static int16_t sine_wave_table[SINE_WAVE_TABLE_LEN];

void Player::begin()
{
	bi_decl(bi_3pins_with_names(PICO_AUDIO_I2S_DATA_PIN, "I2S DIN", PICO_AUDIO_I2S_CLOCK_PIN_BASE, "I2S BCK", PICO_AUDIO_I2S_CLOCK_PIN_BASE + 1, "I2S LRCK"));

	for (int i = 0; i < SINE_WAVE_TABLE_LEN; i++)
	{
		sine_wave_table[i] = 32767 * cosf(i * 2 * (float)(M_PI / SINE_WAVE_TABLE_LEN));
	}

	static audio_format_t format = {
		.sample_freq = 24000,
		.format = AUDIO_BUFFER_FORMAT_PCM_S16,
		.channel_count = 1,
	};

	static audio_buffer_format_t producer_format = {
		.format = &format,
		.sample_stride = 2,
	};

	// TODO: correct size
	audio_buffer_pool_t *pool = audio_new_producer_pool(&producer_format, 3, SAMPLES_PER_BUFFER);

	audio_i2s_config_t config = {
		.data_pin = PICO_AUDIO_I2S_DATA_PIN,
		.clock_pin_base = PICO_AUDIO_I2S_CLOCK_PIN_BASE,
		.dma_channel = 0,
		.pio_sm = 0,
	};

	const audio_format_t *output_format = audio_i2s_setup(&format, &config);
	if (output_format == nullptr)
	{
		panic("[ ERROR ] Player: Unable to open audio device.\n");
	}

	bool connected = audio_i2s_connect(pool);
	assert(connected);
	audio_i2s_set_enabled(true);

	// Test - cause of nullptr
	uint32_t pos = 0;
	// uint32_t step = 0x100000;
	// uint32_t pos_max = 0x10000 * SINE_WAVE_TABLE_LEN;
	uint32_t pos_max = SINE_WAVE_TABLE_LEN;

	while (true)
	{
		audio_buffer_t *buffer = take_audio_buffer(pool, true);
		int16_t *samples = (int16_t *)buffer->buffer->bytes;
		for (uint i = 0; i < buffer->max_sample_count; i++)
		{
			samples[i] = (255 * sine_wave_table[pos]) >> 8u;
			pos++;
			if (pos >= pos_max)
			{
				pos = 0;
			}
		}
		buffer->sample_count = buffer->max_sample_count;
		give_audio_buffer(pool, buffer);
	}
}
