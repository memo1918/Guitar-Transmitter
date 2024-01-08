#include "Receiver.h"

Receiver::Receiver(RF24 &radio, uint8_t size) : _radio(radio), _size(size)
{
}

Receiver::~Receiver()
{
}

/**
 * @brief trys to receive a single item from the radio. The function will block
 * unitl it received something. If the queue is not full the payload will
 * placed into the queue. If the queue is full, the payload will be thrown away
 * silently.
 */
bool Receiver::read(uint8_t *data)
{
	if (this->_radio.available()) // waits until payload is available, is there a payload? get the pipe number that recieved it
	{
		this->_radio.read(data, this->getPayloadSize());
		this->_isDataAvailable = false;
		return true;
	}
	return false;
}

uint8_t Receiver::getPayloadSize()
{
	return this->_size;
}

bool Receiver::isDataAvailable()
{
	return this->_isDataAvailable;
}

void Receiver::setDataAvailable()
{
	this->_isDataAvailable = true;
}
