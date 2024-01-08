#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <stdio.h>
#include <pico/stdlib.h>
#include <pico/audio_i2s.h>

#define SAMPLE_FREQUECY 20'000
#define AUDIO_CHANNEL_COUNT 1
#define AUDIO_SAMPLE_STRIDE 2
#define AUDIO_BUFFER_COUNT 2

class Player
{
private:
	audio_buffer_pool_t *_pool;
	int _size;

public:
	/**
	 * @brief Construct a new Player:: Player object
	 *
	 * @param size the size of the audiobuffer in bytes - default is 256
	 */
	Player(int size);

	/**
	 * @brief Destroy the Player object
	 */
	~Player();

	/**
	 * @brief Setup the Player for playing audio
	 *
	 * This function has to be called before any audio can be played. The function
	 * should only be called once. If the Audio setup fails, the function will panic.
	 */
	void begin();

	/**
	 * @brief Play an audioframe
	 *
	 * Enques the given audioframe for playing.
	 *
	 * @param payload the audioframe to play. The Frame has to be 16 bytes with a sample rate of 20kHz
	 */
	void play(uint8_t *payload);
};

#endif // __PLAYER_H__