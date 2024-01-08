#include "Player.h"

Player::Player(int size) : _size(size)
{
}

Player::~Player()
{
}

void Player::begin()
{
	// setup the audioformat used for playing
	static audio_format_t format = {
		.sample_freq = SAMPLE_FREQUECY,
		.format = AUDIO_BUFFER_FORMAT_PCM_S16,
		.channel_count = AUDIO_CHANNEL_COUNT,
	};

	static audio_buffer_format_t producer_format = {
		.format = &format,
		.sample_stride = AUDIO_SAMPLE_STRIDE,
	};

	// create the audio buffer pool, used for continous audio playing
	this->_pool = audio_new_producer_pool(&producer_format, AUDIO_BUFFER_COUNT, this->_size);

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

	// setup the audio i2s state machine with the given input format
	bool connected = audio_i2s_connect(this->_pool);
	assert(connected);
	audio_i2s_set_enabled(true);
}

void Player::play(uint8_t *payload)
{
	audio_buffer_t *buffer = take_audio_buffer(this->_pool, false);
	if (buffer == nullptr)
	{
		printf("[ WARNING ] Player: Unable to take audio buffer\n");
		return;
	}

	// reset the sample_count for the given buffer
	buffer->sample_count = 0;
	int16_t *samples = (int16_t *)buffer->buffer->bytes;
	for (uint16_t i = 0; i < this->_size; i++)
	{
		int16_t s = payload[i];
		// map the byte from uint8 (0 to 255) to int16 (-32768 to 32767)
		samples[i] = (s - 127) * 16;
	}
	buffer->sample_count = buffer->max_sample_count;
	give_audio_buffer(this->_pool, buffer);
}
