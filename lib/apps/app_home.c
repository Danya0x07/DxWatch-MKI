#include "apps.h"
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

void ReadDateTime(int8_t buff[])
{
    LL_RTC_DateTypeDef date;
    LL_RTC_TimeTypeDef time;

    RTC_GetDateTime(&date, &time);

    ParseTime(&time, &buff[Data_HRH]);
    ParseDate(&date, &buff[Data_DH]);
}

static void DrawLayout(void)
{
    GFX_Clear();
    GFX_SetupBrush(GfxFontSize_21X15, GfxImageScale_X1, false);
    GFX_SetCursor(24, 3);
    GFX_PrintString(LAYOUT_TIME);

    GFX_SetupBrush(GfxFontSize_7X5, GfxImageScale_X1, false);
    GFX_SetCursor(46, 6);
    GFX_PrintString(LAYOUT_DATE);

    GFX_SetCursor(102, 0);
    GFX_PrintString(LAYOUT_VOLTAGE);

    if (RTC_ALARM_Enabled())
        GFX_DrawImage(0, 0, &IMG_ALARM);
    else
        GFX_ClearRect(0, 0, 7, 0);
}

static void DrawData(int8_t buff[])
{
    GFX_SetupBrush(GfxFontSize_21X15, GfxImageScale_X1, false);
    GFX_SetCursor(24, 3);
    PrintDigit(buff[Data_HRH]);
    PrintDigit(buff[Data_HRL]);
    GFX_SetCursor(78, 3);
    PrintDigit(buff[Data_MINH]);
    PrintDigit(buff[Data_MINL]);

    GFX_SetupBrush(GfxFontSize_14X10, GfxImageScale_X1, false);
    GFX_SetCursor(54, 1);
    GFX_PrintString(WEEKDAYS[buff[Data_W] - 1]);

    GFX_SetupBrush(GfxFontSize_7X5, GfxImageScale_X1, false);
    GFX_SetCursor(46, 6);
    PrintDigit(buff[Data_DH]);
    PrintDigit(buff[Data_DL]);
    GFX_SetCursor(64, 6);
    PrintDigit(buff[Data_MH]);
    PrintDigit(buff[Data_ML]);
    GFX_SetCursor(82, 6);
    PrintDigit(buff[Data_YH]);
    PrintDigit(buff[Data_YL]);
}

static void DrawVoltage(uint8_t voltage)
{
    GFX_SetupBrush(GfxFontSize_7X5, GfxImageScale_X1, false);
    GFX_SetCursor(96, 0);
    if (voltage >= 44)
        GFX_DrawImage(96, 0, &IMG_USBSIGN);
    else if (voltage < 34)
        GFX_PrintChar('!');
    else
        GFX_PrintChar(' ');
    GFX_SetCursor(102, 0);
    PrintDigit(voltage / 10);
    GFX_SetCursor(114, 0);
    PrintDigit(voltage % 10);
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
        DrawLayout();
        DrawLockSign();
        OS_StartCustomTimer(20000);
    case AppSignal_CUSTOM:
        ReadDateTime(screenDataBuffer);
        DrawData(screenDataBuffer);
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
            *((struct Application **)io) = quickActionMayBe ? &appStrobe : &appMenu;
        }
        break;

    case AppSignal_VOLTAGE:
        DrawVoltage((uint8_t)((uintptr_t)io));
        break;

    case AppSignal_LOWVOLTAGE:
        if (FLASH_LED_IS_ON())
            FLASH_LED_OFF();
        MOTOR_ON();
        HAL_Delay(200);
        MOTOR_OFF();
        break;

    case AppSignal_RTCALARM:
        ReadDateTime(screenDataBuffer);
        DrawData(screenDataBuffer);
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
        ReadDateTime(screenDataBuffer);
        DrawData(screenDataBuffer);
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
