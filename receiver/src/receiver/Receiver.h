#ifndef __RECEIVER_H__
#define __RECEIVER_H__

#include <pico/util/queue.h>
#include <RF24.h>

class Receiver
{
private:
	queue_t &_queue;
	RF24 &_radio;
	uint8_t _size;
	uint8_t _led;

public:
	Receiver(queue_t &queue, RF24 &radio, uint8_t size, uint8_t led);
	~Receiver();

	void receiveAndEnqueue();
	uint8_t getPayloadSize();
};

#endif // __RECEIVER_H__
