#ifndef _IR_RAW_H
#define _IR_RAW_H

#include <libcommon.h>

#define IR_NODATA   (-1)
#define IR_OVERFLOW (-2)
#define IR_BREAK    (-3)

/* Return number of recorded durations if success, or error code if error. */
int IR_RecordRaw(uint16_t durations[], int16_t maxlen, uint32_t beginingTimeoutMs, uint32_t pulseTimeoutUs);
void IR_SendRaw(const uint16_t durations[], uint16_t len);

#endif // _IR_RAW_H