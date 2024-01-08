#ifndef __RECEIVER_H__
#define __RECEIVER_H__

#include <pico/util/queue.h>
#include <RF24.h>
#include <AudioPayload.h>

class Receiver
{
private:
	RF24 &_radio;
	uint8_t _size;
	bool _isDataAvailable;

public:
	Receiver(RF24 &radio, uint8_t size);
	~Receiver();

	bool read(uint8_t *data);
	uint8_t getPayloadSize();

	void setDataAvailable();
	bool isDataAvailable();
};

#endif // __RECEIVER_H__
