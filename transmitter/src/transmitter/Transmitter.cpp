#include "transmitter.h"
#include "AudioPayload.h"

Transmitter::Transmitter(queue_t &queue, RF24 &radio, uint8_t size) : _queue(queue), _radio(radio), _size(size), _payloadId(0)
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
	uint8_t payload[sizeof(AudioPayload)];
	bool success = queue_try_remove(&this->_queue, &payload[sizeof(AudioPayload::id)]);
	if (!success)
	{
		return;
	}
	payload[0] = this->_payloadId;
	this->_payloadId++;

	// this->_radio.write(payload, sizeof(AudioPayload));
	this->_radio.writeFast(payload, sizeof(AudioPayload));
}

uint8_t Transmitter::getPayloadSize()
{
	return sizeof(AudioPayload);
}
