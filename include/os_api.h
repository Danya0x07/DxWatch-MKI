#ifndef _OS_API_H
#define _OS_API_H

#include "libcommon.h"

typedef enum __packed {
    AppSignal_ENTRANCE,
    AppSignal_BTN0PRESS,
    AppSignal_BTN0RELEASE,
    AppSignal_BTN1PRESS,
    AppSignal_BTN1RELEASE,
    AppSignal_BTN2PRESS,
    AppSignal_BTN2RELEASE,
    AppSignal_BTN3PRESS,
    AppSignal_BTN3RELEASE,
    AppSignal_VOLTAGE,
    AppSignal_LOWVOLTAGE,
    AppSignal_RTCALARM,
    AppSignal_SUSPEND,
    AppSignal_WAKEUP,
    AppSignal_CUSTOM
} AppSignal_t;

typedef enum __packed {
    AppRetCode_OK,
    AppRetCode_EXIT,
    AppRetCode_STAYUP,
    AppRetCode_BAD
} AppRetCode_t;

struct Application {
    const char *const name;
    void (*load)(void);  // Call on startup
    AppRetCode_t (*process)(AppSignal_t signal, void *io);
    void (*save)(void);  // Call before shutdown
};

void OS_StartCustomTimer(uint32_t period);
void OS_StopCustomTimer(void);

#endif // _OS_API_H