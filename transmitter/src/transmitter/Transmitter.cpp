#include "transmitter.h"

Transmitter::Transmitter(queue_t &queue, RF24 &radio, uint8_t size) : _queue(queue), _radio(radio), _size(size)
{
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
	uint8_t payload[this->getPayloadSize()];
	bool success = queue_try_remove(&this->_queue, &payload);
	if (!success)
	{
		return;
	}

	uint64_t start_timer = to_us_since_boot(get_absolute_time()); // start the timer
	// bool report = this->_radio.write(payload, this->getPayloadSize()); // transmit & save the report)
	bool report = this->_radio.writeFast(payload, this->getPayloadSize());
	uint64_t end_timer = to_us_since_boot(get_absolute_time()); // end the timer

	// TODO: handle report
	if (report)
	{
		// payload was delivered; print the payload sent & the timer result
		// printf("Transmission successful! Time to transmit = %llu us. Sent: %f\n", end_timer - start_timer, payload);
	}
	else
	{
		// payload was not delivered
		// printf("Transmission failed or timed out\n");
	}
}

uint8_t Transmitter::getPayloadSize()
{
	return this->_size;
}
