#ifndef __TRANSMITTER_H__
#define __TRANSMITTER_H__

#include <pico/util/queue.h>
#include <RF24.h>

class Transmitter
{
private:
	queue_t &_queue;
	RF24 &_radio;
	uint8_t _size;

public:
	Transmitter(queue_t &queue, RF24 &radio, uint8_t size);
	~Transmitter();

	void readAndSend();
	uint8_t getPayloadSize();
};

#endif // __TRANSMITTER_H__
