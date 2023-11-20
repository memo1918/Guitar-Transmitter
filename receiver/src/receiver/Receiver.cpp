#include "Receiver.h"

Receiver::Receiver(queue_t &queue, RF24 &radio, uint8_t size, uint8_t led) : _queue(queue), _radio(radio), _size(size), _led(led)
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
void Receiver::receiveAndEnqueue()
{
	uint8_t pipe;
	while (!this->_radio.available(&pipe)) // waits until payload is available, is there a payload? get the pipe number that recieved it
	{
		gpio_put(this->_led, true);
	}
	gpio_put(this->_led, false);

	uint8_t payload[this->getPayloadSize()];			 // get the size of the payload
	this->_radio.read(&payload, this->getPayloadSize()); // fetch payload from FIFO

	bool success = queue_try_add(&this->_queue, &payload);
	if (!success)
	{
		printf("[ WARNING ] the paylaod could not be added to the queu, and will silently thrown away");
	}
}

uint8_t Receiver::getPayloadSize()
{
	return this->_size;
}
