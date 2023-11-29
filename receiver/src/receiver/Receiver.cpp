#include "Receiver.h"

Receiver::Receiver(RF24 &radio, uint8_t size, uint8_t led) : _radio(radio), _size(size), _led(led)
{
	gpio_init(this->_led);
	gpio_set_dir(this->_led, GPIO_OUT);
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
void Receiver::read(AudioPayload &payload)
{
	uint8_t pipe;
	while (!this->_radio.available(&pipe)) // waits until payload is available, is there a payload? get the pipe number that recieved it
	{
		gpio_put(this->_led, true);
	}
	gpio_put(this->_led, false);

	this->_radio.read(&payload, this->getPayloadSize()); // fetch payload from FIFO
}

uint8_t Receiver::getPayloadSize()
{
	return this->_size;
}
