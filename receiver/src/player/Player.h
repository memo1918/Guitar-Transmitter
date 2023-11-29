#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <stdio.h>
#include <pico/stdlib.h>
#include <pico/audio_i2s.h>
#include <hardware/clocks.h>
#include <hardware/structs/clocks.h>
#include <pico/binary_info.h>
#include <AudioPayload.h>

class Player
{
private:
	bool _isSetupComplete = false;
	audio_buffer_pool_t *_pool;

public:
	Player();
	~Player();
	void begin();
	void play(AudioPayload &payload);
};

#endif // __PLAYER_H__