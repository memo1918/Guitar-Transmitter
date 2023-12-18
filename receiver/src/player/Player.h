#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <stdio.h>
#include <pico/stdlib.h>
#include <pico/audio_i2s.h>
#include <pico/util/queue.h>
#include <hardware/clocks.h>
#include <hardware/structs/clocks.h>
#include <pico/binary_info.h>
#include <AudioPayload.h>
#include <hardware/pio.h>
#include <hardware/dma.h>

class Player
{
private:
	bool _isSetupComplete = false;
	audio_buffer_pool_t *_pool;
	uint8_t _lastPacketId = 0;
	audio_buffer_t *_currentBuffer = nullptr;

public:
	Player();
	~Player();
	void begin();
	// void run();
	void play(uint8_t *payload);
};

#endif // __PLAYER_H__