#include "embedded_common.h"
#include <builtin_led.h>

void blink(int times, int duration)
{
    while (times--) {
        BUILTIN_LED_ON();
        HAL_Delay(duration);
        BUILTIN_LED_OFF();
        HAL_Delay(duration / 2);
    }
}