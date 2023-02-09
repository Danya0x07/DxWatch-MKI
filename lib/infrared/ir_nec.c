#include "ir_nec.h"

#define NEC_HDR_PULSE	9000
#define NEC_HDR_SPACE	4500
#define NEC_SHORT_INTERVAL	560
#define NEC_LONG_INTERVAL	1690
#define NEC_PULSE_INACCURACY	200

typedef enum {
    NecPulse_SHORT,
    NecPulse_LONG,
    NecPulse_HDRSHORT,
    NecPulse_HDRLONG,
    NecPulse_UNSUPPORTED
} NecPulse_t;

extern bool _MatchDuration(int16_t actual, int16_t expectation, int16_t inaccuracy);

static NecPulse_t GetPulseType(uint16_t duration)
{
    if (duration > 0x7FFF)
        return NecPulse_UNSUPPORTED;

    int16_t _duration = duration;

    if (_MatchDuration(_duration, NEC_HDR_PULSE, NEC_PULSE_INACCURACY)) {
        return NecPulse_HDRLONG;
    }
    if (_MatchDuration(_duration, NEC_HDR_SPACE, NEC_PULSE_INACCURACY)) {
        return NecPulse_HDRSHORT;
    }
    if (_MatchDuration(_duration, NEC_SHORT_INTERVAL, NEC_PULSE_INACCURACY)) {
        return NecPulse_SHORT;
    }
    if (_MatchDuration(_duration, NEC_LONG_INTERVAL, NEC_PULSE_INACCURACY)) {
        return NecPulse_LONG;
    }
    return NecPulse_UNSUPPORTED;
}

NecParsingStatus_t NEC_ParseRaw(const uint16_t durations[], uint16_t len, struct NecPacket *packet)
{
    if (len != NEC_FRAME_LEN)
        return NecParsingStatus_BAD_RAWLEN;

    if (GetPulseType(durations[0]) != NecPulse_HDRLONG || GetPulseType(durations[1]) != NecPulse_HDRSHORT)
        return NecParsingStatus_BAD_PULSE;

    NecPulse_t pulseType;
    uint32_t content = 0;
    for (uint16_t i = 0, idx; i < NEC_BITS; i++) {
        idx = 2 + (i << 1);  // 2, 4, ... 64

        /* Pulses are always short (except the preambula header pulse), 
         * pauses can be short or long. 
         */
        pulseType = GetPulseType(durations[idx]);
        if (pulseType != NecPulse_SHORT) {
            return NecParsingStatus_BAD_PULSE;
        }

        pulseType = GetPulseType(durations[idx + 1]);
        if (pulseType == NecPulse_SHORT) {
            // 0 bit
        }
        else if (pulseType == NecPulse_LONG) {
            content |= (1 << i);
        }
        else {
            return NecParsingStatus_BAD_PULSE;
        }
    }

    uint8_t b1 = content & 0xFF;
    uint8_t b2 = (content >> 8) & 0xFF;
    uint8_t b3 = (content >> 16) & 0xFF;
    uint8_t b4 = (content >> 24) & 0xFF;
    if ((uint8_t)~b2 != b1 || (uint8_t)~b4 != b3)
        return NecParsingStatus_BITS_MISMATCH;

    if (GetPulseType(durations[NEC_FRAME_LEN - 1]) != NecPulse_SHORT)
        return NecParsingStatus_BAD_PULSE;

    packet->address = b1;
    packet->command = b3;
    return NecParsingStatus_OK;
}

void NEC_BuildRaw(const struct NecPacket *packet, uint16_t durations[])
{
    uint8_t b1 = packet->address;
    uint8_t b2 = ~b1;
    uint8_t b3 = packet->command;
    uint8_t b4 = ~b3;
    uint32_t content = b1 | (b2 << 8) | (b3 << 16) | (b4 << 24);

    durations[0] = NEC_HDR_PULSE;
    durations[1] = NEC_HDR_SPACE;
    for (uint16_t i = 0, idx; i < NEC_BITS; i++) {
        idx = 2 + (i << 1);  // 2, 4, ... 64
        durations[idx] = NEC_SHORT_INTERVAL;
        durations[idx + 1] = (content & (1 << i)) ? NEC_LONG_INTERVAL : NEC_SHORT_INTERVAL;
    }
    durations[NEC_FRAME_LEN - 1] = NEC_SHORT_INTERVAL;
}