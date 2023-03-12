#ifndef _OS_H
#define _OS_H

#include "main.h"
#include "buttons.h"
#include "os_api.h"

struct SystemEvent {
    enum __packed {
        SysEvent_BUTTON,
        SysEvent_VOLTAGE,
        SysEvent_CUSTOM,
        SysEvent_ALARM,
        SysEvent_SUSPEND,
        SysEvent_WAKEUP,
        SysEvent_SHUTDOWN,
        SysEvent_STARTUP
    } type;
    union {
        struct {
            Button_t btn;
            ButtonEvent_t ev;
        };
        uint16_t voltage;
    } data;
};

void OS_Init(void);
void OS_StartSheduler(void);
void OS_Wait(uint32_t ms);

#endif // _OS_H