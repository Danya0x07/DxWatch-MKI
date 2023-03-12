#include "apps.h"
#include <os_api.h>
#include <gfx.h>
#include <rtc.h>
#include <motor.h>
#include <flash_led.h>

/*
switch()
{
case AppSignal_ENTRANCE:

    break;

case AppSignal_BTN0PRESS:

    break;

case AppSignal_BTN0RELEASE:

    break;

case AppSignal_BTN1PRESS:

    break;

case AppSignal_BTN1RELEASE:

    break;

case AppSignal_BTN2PRESS:

    break;

case AppSignal_BTN2RELEASE:

    break;

case AppSignal_BTN3PRESS:

    break;

case AppSignal_BTN3RELEASE:

    break;

case AppSignal_VOLTAGE:

    break;

case AppSignal_LOWVOLTAGE:

    break;

case AppSignal_RTCALARM:

    break;

case AppSignal_SUSPEND:

    break;

case AppSignal_WAKEUP:

    break;

case AppSignal_CUSTOM:

    break;
}
*/

static void DrawDateTime(void)
{
    LL_RTC_DateTypeDef date;
    LL_RTC_TimeTypeDef time;
    char buff[9];

    RTC_GetDateTime(&date, &time);

    GFX_SetupBrush(GfxFontSize_21X15, GfxImageScale_X1, false);
    GFX_SetCursor(1, 1);

    buff[0] = '0' + time.Hours / 10;
    buff[1] = '0' + time.Hours % 10;
    buff[2] = ':';
    buff[3] = '0' + time.Minutes / 10;
    buff[4] = '0' + time.Minutes % 10;
    buff[5] = '\0';
    GFX_PrintString(buff);

    GFX_SetupBrush(GfxFontSize_7X5, GfxImageScale_X1, false);
    GFX_SetCursor(6, 6);
    
    buff[0] = '0' + date.Day / 10;
    buff[1] = '0' + date.Day % 10;
    buff[2] = ':';
    buff[3] = '0' + date.Month / 10;
    buff[4] = '0' + date.Month % 10;
    buff[5] = ':';
    buff[6] = '0' + date.Year / 10;
    buff[7] = '0' + date.Year % 10;
    buff[8] = '\0';
    GFX_PrintString(buff);
}

static void DrawVoltage(uint16_t voltage)
{
    GFX_SetupBrush(GfxFontSize_7X5, GfxImageScale_X1, false);
    GFX_SetCursor(17, 0);
    GFX_PrintChar(voltage <= 34 ? '!' : voltage >= 49 ? '^' : ' ');
    GFX_PrintChar('0' + voltage / 10);
    GFX_PrintChar('.');
    GFX_PrintChar('0' + voltage % 10);
}

static inline void DrawAlarmSign(void)
{

}

static AppRetCode_t process(AppSignal_t signal, void *io)
{
    AppRetCode_t retCode = AppRetCode_OK;
    
    switch (signal)
    {
    case AppSignal_ENTRANCE:
        GFX_Clear();
        OS_StartCustomTimer(20000);
    case AppSignal_CUSTOM:
        DrawDateTime();
        break;

    case AppSignal_BTN3PRESS:
        if (MOTOR_IS_ON()) {
            MOTOR_OFF();
        } else {
            retCode = AppRetCode_EXIT;
            *((struct Application **)io) = &appHome;
        }
        break;

    case AppSignal_VOLTAGE:
    {
        uint16_t voltage = (uint16_t)io;
        DrawVoltage(voltage);
    }
        break;

    case AppSignal_LOWVOLTAGE:
        if (FLASH_LED_IS_ON())
            FLASH_LED_OFF();
        MOTOR_ON();
        HAL_Delay(200);
        MOTOR_OFF();
        break;

    case AppSignal_RTCALARM:
        if (FLASH_LED_IS_ON())
            FLASH_LED_OFF();
        MOTOR_ON();
        break;

    case AppSignal_SUSPEND:
        if (MOTOR_IS_ON()) {
            retCode = AppRetCode_STAYUP;
        } else{
            OS_StopCustomTimer();
        }
        break;

    case AppSignal_WAKEUP:
        DrawDateTime();
        OS_StartCustomTimer(20000);
        break;
    
    default:
        break;
    }

    return retCode;
}

struct Application appHome = {
    .name = "Home",
    .load = AppDummyCallback,
    .process = process,
    .save = AppDummyCallback
};
