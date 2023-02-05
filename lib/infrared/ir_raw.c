#include "ir_raw.h"
#include "ir_port.h"
#include <string.h>

#define SEND_BEGINNING_DELAY    110

int IR_RecordRaw(uint16_t durations[], uint16_t maxlen, uint32_t beginingTimeoutMs, uint32_t pulseTimeoutUs)
{
	if (!EYE_IS_HIGH())  // Idle state of eye pin is high.
		return IR_BREAK;

	memset(durations, 0, maxlen);

	uint32_t now = MS_GET();
	while (EYE_IS_HIGH())
		if (MS_GET() - now >= beginingTimeoutMs)
			return IR_NODATA;

	// Frame started, now eye pin is low.
	bool pinState = 0;
	uint16_t len = 0;

	for (;;) {
		US_RESET();
		while (EYE_IS_HIGH() == pinState) {
			if (US_GET() >= pulseTimeoutUs) {
				if (pinState == 1)
					return len;
				else
					return IR_BREAK;
			}
		}
		if (len >= maxlen)
			return IR_OVERFLOW;

		durations[len++] = US_GET();
		pinState = !pinState;
	}
}

void IR_SendRaw(const uint16_t durations[], uint16_t len)
{
    CARRIER_ON();
    MS_WAIT(SEND_BEGINNING_DELAY);
    for (int i = 0; i < len; i++) {
        if (i & 1)
            LED_NOBLINK();
        else
            LED_BLINK();
        US_WAIT(durations[i]);
    }
    LED_NOBLINK();
    CARRIER_OFF();
}