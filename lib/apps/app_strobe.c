#include "apps.h"
#include <gfx.h>
#include <flash_led.h>
#include <motor.h>
//#include <stdlib.h>

static AppRetCode_t process(AppSignal_t signal, void *io)
{
    AppRetCode_t retCode = AppRetCode_OK;

    switch(signal)
    {
    case AppSignal_ENTRANCE:
        GFX_Clear();
        if (MOTOR_IS_ON())
            MOTOR_OFF();
        OS_StartCustomTimer(200);
        break;

    case AppSignal_BTN3PRESS:
        OS_StopCustomTimer();
        FLASH_LED_OFF();
        retCode = AppRetCode_EXIT;
        *((struct Application **)io) = &appHome;
        break;

    case AppSignal_SUSPEND:
        retCode = AppRetCode_STAYUP;
        break;

    case AppSignal_CUSTOM:
        FLASH_LED_TOGGLE();
        OS_StartCustomTimer(50);
        break;
    
    default:
        break;
    }

    return retCode;
}

struct Application appStrobe = {
    .name = "Strobe",
    .load = AppDummyCallback,
    .process = process,
    .save = AppDummyCallback
};