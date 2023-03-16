#include "apps.h"

static void ReadAlarmParams(int8_t buff[])
{
    LL_RTC_TimeTypeDef time;

    RTC_ALARM_GetTime(&time);
    ParseTime(&time, &buff[Data_HRH]);
    buff[Data_S] = RTC_ALARM_Enabled();
}

static void WriteAlarmParams(const int8_t buff[])
{
    LL_RTC_TimeTypeDef alarmTime = {
        .Hours = buff[Data_HRH] * 10 + buff[Data_HRL], 
        .Minutes = buff[Data_MINH] * 10 + buff[Data_MINL]
    };

    RTC_ALARM_SetTime(&alarmTime);
    if (buff[Data_S])
        RTC_ALARM_Enable();
    else
        RTC_ALARM_Disable();
}

static void DrawLayout(void)
{
    GFX_Clear();
    GFX_SetupBrush(GfxFontSize_14X10, GfxImageScale_X1, false);
    GFX_SetCursor(10, 0);
    GFX_PrintString("Alarm set");

    GFX_SetCursor(0, 2);
    GFX_PrintString("Status:");
    GFX_SetCursor(34, 4);
    GFX_PrintString(LAYOUT_TIME);
}

static void DrawData(int8_t buff[])
{
    GFX_SetupBrush(GfxFontSize_14X10, GfxImageScale_X1, false);
    GFX_SetCursor(96, 2);
    PrintDigit(buff[Data_S]);
    GFX_SetCursor(34, 4);
    PrintDigit(buff[Data_HRH]);
    PrintDigit(buff[Data_HRL]);
    GFX_SetCursor(70, 4);
    PrintDigit(buff[Data_MINH]);
    PrintDigit(buff[Data_MINL]);
}

static void DrawIndexMark(uint8_t index)
{    
    GFX_ClearRect(34, 6, 120, 6);
    GFX_ClearRect(108, 2, 120, 3);
    GFX_SetupBrush(GfxFontSize_14X10, GfxImageScale_X1, false);

    if (index == Data_S) {
        GFX_SetCursor(108, 2);
        GFX_PrintChar('<');
    } else {
        GFX_SetCursor(34 + 12 * ((index - Data_HRH) + (index > Data_HRL)), 6);
        GFX_PrintChar('^');
    }
}

int8_t MaxTimeValue(int8_t buff[], uint8_t index)
{
    switch (index)
    {
    case Data_HRH: return 2;
    case Data_HRL: return buff[Data_HRH] >= 2 ? 3 : 9;
    case Data_MINH: return 5;
    case Data_MINL: return 9;
    case Data_S: return 1;
    }
    return 0;
}

static AppRetCode_t process(AppSignal_t signal, void *io)
{
    AppRetCode_t retCode = AppRetCode_OK;
    
    switch(signal)
    {
    case AppSignal_ENTRANCE:
        screenDataIndex = 0;
        DrawLayout();
        ReadAlarmParams(screenDataBuffer);
        break;

    case AppSignal_BTN0PRESS:
        if (screenDataIndex > 0) {
            screenDataIndex--;
        } else {
            retCode = AppRetCode_EXIT;
            *((struct Application **)io) = &appHome;
        }
        break;

    case AppSignal_BTN1PRESS:
        if (screenDataBuffer[screenDataIndex] > 0)
            screenDataBuffer[screenDataIndex]--;
        else
            screenDataBuffer[screenDataIndex] = MaxTimeValue(screenDataBuffer, screenDataIndex);
        break;

    case AppSignal_BTN2PRESS:
        if (screenDataBuffer[screenDataIndex] < MaxTimeValue(screenDataBuffer, screenDataIndex))
            screenDataBuffer[screenDataIndex]++;
        else
            screenDataBuffer[screenDataIndex] = 0;
        break;

    case AppSignal_BTN3PRESS:
        if (screenDataIndex < 4 && screenDataBuffer[Data_S] != 0) {
            screenDataIndex++;
        } else {
            WriteAlarmParams(screenDataBuffer);
            retCode = AppRetCode_EXIT;
            *((struct Application **)io) = &appHome;
        }
        break;

    case AppSignal_SUSPEND:
        retCode = AppRetCode_EXIT;
        *((struct Application **)io) = &appHome;
        break;
    
    default: 
        break;
    }
    DrawData(screenDataBuffer);
    DrawIndexMark(screenDataIndex);
    return retCode;
}

struct Application appAlarm = {
    .name = "Alarm set",
    .load = AppDummyCallback,
    .process = process,
    .save = AppDummyCallback
};