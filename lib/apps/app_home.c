#include "apps.h"
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

static bool locked = true;
static AppSignal_t unlockSequence[] = {
    AppSignal_BTN1PRESS,
    AppSignal_BTN2PRESS,
    AppSignal_BTN1RELEASE,
    AppSignal_BTN0PRESS,
    AppSignal_BTN2RELEASE,
    AppSignal_BTN0RELEASE
};
static uint8_t unlockSequenceLength = sizeof(unlockSequence) / sizeof(unlockSequence[0]);

static void DrawDateTime(void)
{
    LL_RTC_DateTypeDef date;
    LL_RTC_TimeTypeDef time;
    char buff[9];

    RTC_GetDateTime(&date, &time);

    GFX_SetupBrush(GfxFontSize_21X15, GfxImageScale_X1, false);
    GFX_SetCursor(24, 3);

    buff[0] = '0' + time.Hours / 10;
    buff[1] = '0' + time.Hours % 10;
    buff[2] = ':';
    buff[3] = '0' + time.Minutes / 10;
    buff[4] = '0' + time.Minutes % 10;
    buff[5] = '\0';
    GFX_PrintString(buff);

    GFX_SetupBrush(GfxFontSize_14X10, GfxImageScale_X1, false);
    GFX_SetCursor(54, 1);
    GFX_PrintString(WEEKDAYS[date.WeekDay - 1]);

    GFX_SetupBrush(GfxFontSize_7X5, GfxImageScale_X1, false);
    GFX_SetCursor(46, 6);
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
    GFX_SetCursor(96, 0);
    if (voltage >= 44)
        GFX_DrawImage(96, 0, &IMG_USBSIGN);
    else if (voltage < 34)
        GFX_PrintChar('!');
    else
        GFX_PrintChar(' ');
    GFX_PrintChar('0' + voltage / 10);
    GFX_PrintChar('.');
    GFX_PrintChar('0' + voltage % 10);
    GFX_PrintChar('v');
}

static inline void DrawAlarmSign(void)
{
    GFX_SetupBrush(GfxFontSize_7X5, GfxImageScale_X1, false);
    if (RTC_ALARM_Enabled())
        GFX_DrawImage(0, 0, &IMG_ALARM);
    else
        GFX_ClearRect(0, 0, 7, 0);
}

static void DrawLockSign(void)
{
    GFX_SetupBrush(GfxFontSize_7X5, GfxImageScale_X1, false);
    if (locked)
        GFX_DrawImage(0, 7, &IMG_LOCK);
    else
        GFX_ClearRect(0, 7, IMG_LOCK.widthPx - 1, 7);
}

static void AttemptToUnlock(AppSignal_t signal)
{
    static uint8_t index = 0;

    if (signal == unlockSequence[index]) {
        index++;
        if (index == unlockSequenceLength) {
            locked = false;
            index = 0;
            DrawLockSign();
            FLASH_LED_OFF();
        }
    } else {
        index = 0;
    }
}

static AppRetCode_t process(AppSignal_t signal, void *io)
{
    AppRetCode_t retCode = AppRetCode_OK;
    static bool quickActionMayBe = false;
    
    switch (signal)
    {
    case AppSignal_ENTRANCE:
        quickActionMayBe = false;
        GFX_Clear();
        OS_StartCustomTimer(20000);
        DrawAlarmSign();
        DrawLockSign();
    case AppSignal_CUSTOM:
        DrawDateTime();
        break;
    
    case AppSignal_BTN0PRESS:
    case AppSignal_BTN0RELEASE:
    case AppSignal_BTN2RELEASE:
    case AppSignal_BTN3RELEASE:
        if (locked) {
            AttemptToUnlock(signal);
        }
        break;

    case AppSignal_BTN1PRESS:
        if (locked) {
            AttemptToUnlock(signal);
        } else {
            quickActionMayBe = true;
        }
        break;

    case AppSignal_BTN1RELEASE:
        if (locked) {
            AttemptToUnlock(signal);
        } else {
            quickActionMayBe = false;
        }
        break;

    case AppSignal_BTN2PRESS:
        if (locked) {
            AttemptToUnlock(signal);
        } else {
            if (quickActionMayBe && !MOTOR_IS_ON()) {
                FLASH_LED_TOGGLE();
            } else {
                locked = true;
                DrawLockSign();
            }
        }
        break;

    case AppSignal_BTN3PRESS:
        if (MOTOR_IS_ON()) {
            MOTOR_OFF();
        } else if (locked) {
            AttemptToUnlock(signal);
        } else {
            retCode = AppRetCode_EXIT;
            *((struct Application **)io) = quickActionMayBe ? &appStrobe : &appHome;
        }
        break;

    case AppSignal_VOLTAGE:
    {
        uint16_t voltage = (uint16_t)((uintptr_t)io);
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
