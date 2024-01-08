#ifndef __AUDIO_PAYLOAD_H__
#define __AUDIO_PAYLOAD_H__

#include <stdint.h>

struct _AudioPayload
{
	uint8_t id;
	uint8_t bytes[31];
};

typedef struct _AudioPayload AudioPayload;

#endif // __AUDIO_PAYLOAD_H__
