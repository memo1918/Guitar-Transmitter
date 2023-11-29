#include "Player.h"
#include <math.h>

Player::Player()
{
}

Player::~Player()
{
}

#define SAMPLES_PER_BUFFER 32

void Player::begin()
{
	bi_decl(bi_3pins_with_names(PICO_AUDIO_I2S_DATA_PIN, "I2S DIN", PICO_AUDIO_I2S_CLOCK_PIN_BASE, "I2S BCK", PICO_AUDIO_I2S_CLOCK_PIN_BASE + 1, "I2S LRCK"));

	static audio_format_t format = {
		.sample_freq = 44100,
		.format = AUDIO_BUFFER_FORMAT_PCM_S16,
		.channel_count = 1,
	};

	static audio_buffer_format_t producer_format = {
		.format = &format,
		.sample_stride = 2,
	};

	// TODO: correct size
	this->_pool = audio_new_producer_pool(&producer_format, 10, SAMPLES_PER_BUFFER);

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

	bool connected = audio_i2s_connect(this->_pool);
	assert(connected);
	audio_i2s_set_enabled(true);
}

void Player::play(AudioPayload &payload)
{
	audio_buffer_t *buffer = take_audio_buffer(this->_pool, true);
	int16_t *samples = (int16_t *)buffer->buffer->bytes;
	for (uint i = 0; i < buffer->max_sample_count; i++)
	{
		samples[i] = (int16_t)payload.bytes[i] * 256 - 32768;
	}
	buffer->sample_count = buffer->max_sample_count;
	give_audio_buffer(this->_pool, buffer);
}
