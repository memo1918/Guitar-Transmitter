#include "transmitter.h"
#include "AudioPayload.h"

static uint8_t payload[sizeof(AudioPayload)];

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
	queue_remove_blocking(&this->_queue, &payload[sizeof(AudioPayload::id)]);
	//bool success = queue_try_remove(&this->_queue, &payload[sizeof(AudioPayload::id)]); // 400ns
	//printf("packet number: %d\n", payload[1]);
	payload[0] = this->_payloadId;
	this->_payloadId++;

	gpio_put(20, true);

	//this->_radio.write(payload, sizeof(AudioPayload)); // 700us
	this->_radio.startFastWrite(payload, 32, 0); // 170us
	//this->_radio.writeFast(payload, sizeof(AudioPayload)); // 300us
	gpio_put(20, false);

}

uint8_t Transmitter::getPayloadSize()
{
	return sizeof(AudioPayload);
}
