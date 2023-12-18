#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
}

#define SAMPLES_PER_BUFFER 256

void Player::begin()
{
	bi_decl(bi_3pins_with_names(PICO_AUDIO_I2S_DATA_PIN, "I2S DIN", PICO_AUDIO_I2S_CLOCK_PIN_BASE, "I2S BCK", PICO_AUDIO_I2S_CLOCK_PIN_BASE + 1, "I2S LRCK"));

	static audio_format_t format = {
		.sample_freq = 20000,
		.format = AUDIO_BUFFER_FORMAT_PCM_S16,
		.channel_count = 1,
	};

	static audio_buffer_format_t producer_format = {
		.format = &format,
		.sample_stride = 2,
	};

	// TODO: correct size
	this->_pool = audio_new_producer_pool(&producer_format, 2, SAMPLES_PER_BUFFER);

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

static uint8_t trace[24500];
static uint tracePos = 0;

/*void Player::run()
{
	if (this->_currentBuffer == nullptr)
	{
		this->_currentBuffer = take_audio_buffer(this->_pool, true);
		if (this->_currentBuffer == nullptr)
		{
			return;
		}
		this->_currentBuffer->sample_count = 0;
	}

	audio_buffer_t *buffer = this->_currentBuffer;
	int16_t *samples = (int16_t *)buffer->buffer->bytes;

	for (uint i = buffer->sample_count; i < buffer->max_sample_count; i++)
	{
		uint8_t data;
		bool success = queue_try_remove(&_queue, &data);
		if (!success)
		{
			volatile bool noData = true;
			return;

			// printf("[ WARNING ] Player: Audio underrun occoured\n");
			//  continue;
			//  return;
		}

		// samples[buffer->sample_count] = (vol * sine_wave_table[pos >> 16u]) >> 8u;
		samples[buffer->sample_count] = (data * 256) - 32768;
		buffer->sample_count++;
		pos += step;
		if (pos >= pos_max)
			pos -= pos_max;
	}

	give_audio_buffer(this->_pool, buffer);
	this->_currentBuffer = nullptr;
}*/

void Player::play(uint8_t *payload)
{
	gpio_put(20, true);
	audio_buffer_t *buffer = take_audio_buffer(this->_pool, false);
	if (buffer == nullptr)
	{
		printf("[ WARNING ] Player: Unable to take audio buffer\n");
		return;
	}

	buffer->sample_count = 0;

	int16_t *samples = (int16_t *)buffer->buffer->bytes;

	for (uint16_t i = 0; i < SAMPLES_PER_BUFFER; i++)
	{
		// map payload bytes from 0-255 from -32768 to 32767
		// samples[buffer->sample_count] = (int32_t)(payload.bytes[i] * 256) - 32768;

		// int8_t sample = payload[i] - 127;
		// samples[i] =

		int16_t s = payload[i];
		samples[i] = (s - 127) * 16;
		// samples[i] = 4080;
		//    samples[i] = sine_wave_table[pos];
		//     samples[i] = (vol * sine_wave_table[pos >> 16u]) >> 8u;

		/* if (payload[i] == 0)
		{
			if (i == 0)
			{
				samples[i] = samples[SAMPLES_PER_BUFFER];
				payload[i] = payload[SAMPLES_PER_BUFFER];
			}
			else
			{
				samples[i] = samples[i - 1];
				payload[i] = payload[i - 1];
			}
		}*/

		// pos++;
		// if (pos >= pos_max) {
		//	pos = 0;
		//}
		// if (buffer->sample_count == buffer->max_sample_count)
		//{
		//	give_audio_buffer(this->_pool, buffer);
		//	this->_currentBuffer = nullptr;
		//  buffer = take_audio_buffer(this->_pool, true);
		//  buffer->sample_count = 0;
		//}
	}
	buffer->sample_count = buffer->max_sample_count;
	give_audio_buffer(this->_pool, buffer);
	gpio_put(20, false);

	// this->_currentBuffer = buffer;
}
