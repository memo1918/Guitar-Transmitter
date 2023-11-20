#include "transmitter.h"

Transmitter::Transmitter(queue_t &queue, RF24 &radio, uint8_t size, uint8_t led) : _queue(queue), _radio(radio), _size(size), _led(led)
{
	gpio_init(this->_led);
	gpio_set_dir(this->_led, GPIO_OUT);
}

Transmitter::~Transmitter()
{
}

/**
 * @brief reads a singel item from the queue. If the read wass succesfull, the item will be send over the nRF24.
 * If the read was not successfull, or the queue is empty the method will exit immediatly. If the read was succesfull
 * the transmission report will be printed to standard output.
 */
void Transmitter::readAndSend()
{
	gpio_put(this->_led, true);
	uint8_t payload[this->getPayloadSize()];
	bool success = queue_try_remove(&this->_queue, &payload);
	if (!success)
	{
		return;
	}
	// ignore return type (success), cause it will be always true, because auto-ack is disabled
	this->_radio.writeFast(payload, this->getPayloadSize());
	gpio_put(this->_led, false);
}

uint8_t Transmitter::getPayloadSize()
{
	return this->_size;
}
