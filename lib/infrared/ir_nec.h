#ifndef _IR_NEC_H
#define _IR_NEC_H

#include <libcommon.h>

#define NEC_BITS	32
#define NEC_FRAME_LEN	(NEC_BITS * 2 + 3)  // 2 header durations, 1 last pulse

struct NecPacket {
	uint8_t address;
	uint8_t command;
};

typedef enum {
	NecParsingStatus_OK,
	NecParsingStatus_BITS_MISMATCH,
	NecParsingStatus_BAD_RAWLEN,
	NecParsingStatus_BAD_PULSE,
} NecParsingStatus_t;

NecParsingStatus_t NEC_ParseRaw(const uint16_t durations[], uint16_t len, struct NecPacket *packet);
void NEC_BuildRaw(const struct NecPacket *packet, uint16_t durations[]);

#endif // _IR_NEC_H