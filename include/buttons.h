#ifndef _BUTTONS_H
#define _BUTTONS_H

#include "libcommon.h"

typedef enum __packed {BTN0, BTN1, BTN2, BTN3} Button_t;

typedef enum __packed {
    ButtonEvent_NOTHING,
    ButtonEvent_PRESS,
    ButtonEvent_RELEASE
} ButtonEvent_t;

bool Button_IsPressed(Button_t btn);
ButtonEvent_t Button_GetEvent(Button_t btn);

void Button0_EnableInterrupt(void);
void Button0_DisableInterrupt(void);

#endif // _BUTTONS_H