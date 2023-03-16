#include "apps.h"

extern void ReadDateTime(int8_t buff[]);
extern int8_t MaxTimeValue(int8_t buff[], uint8_t index);

static void WriteDateTime(const int8_t buff[])
{
    LL_RTC_DateTypeDef date = {
        .Day = buff[Data_DH] * 10 + buff[Data_DL],
        .Month = buff[Data_MH] * 10 + buff[Data_ML],
        .Year = buff[Data_YH] * 10 + buff[Data_YL],
        .WeekDay = buff[Data_W] * 10 + buff[Data_W]
    };
    
    LL_RTC_TimeTypeDef time = {
        .Hours = buff[Data_HRH] * 10 + buff[Data_HRL], 
        .Minutes = buff[Data_MINH] * 10 + buff[Data_MINL]
    };

    RTC_SetDateTime(&date, &time);
}

static void DrawLayout(void)
{
    GFX_Clear();
    GFX_SetupBrush(GfxFontSize_14X10, GfxImageScale_X1, false);
    GFX_SetCursor(10, 0);
    GFX_PrintString("Date&Time");

    GFX_SetCursor(12, 2);
    GFX_PrintString("T: ");
    GFX_PrintString(LAYOUT_TIME);

    GFX_SetCursor(12, 4);
    GFX_PrintString("D:");
    GFX_SetupBrush(GfxFontSize_7X5, GfxImageScale_X1, false);
    GFX_SetCursor(48, 5);
    GFX_PrintString(LAYOUT_DATE);
}

static void DrawData(int8_t buff[])
{
    GFX_SetupBrush(GfxFontSize_14X10, GfxImageScale_X1, false);
    GFX_SetCursor(48, 2);
    PrintDigit(buff[Data_HRH]);
    PrintDigit(buff[Data_HRL]);
    GFX_SetCursor(84, 2);
    PrintDigit(buff[Data_MINH]);
    PrintDigit(buff[Data_MINL]);

    GFX_SetupBrush(GfxFontSize_7X5, GfxImageScale_X1, false);
    GFX_SetCursor(48, 5);
    PrintDigit(buff[Data_DH]);
    PrintDigit(buff[Data_DL]);
    GFX_SetCursor(66, 5);
    PrintDigit(buff[Data_MH]);
    PrintDigit(buff[Data_ML]);
    GFX_SetCursor(84, 5);
    PrintDigit(buff[Data_YH]);
    PrintDigit(buff[Data_YL]);
    GFX_SetCursor(12, 6);
    GFX_PrintString(WEEKDAYS[buff[Data_W] - 1]);
}

static void DrawIndexMark(uint8_t index)
{    
    GFX_ClearRect(0, 2, 11, 7);
    GFX_ClearRect(0, 7, 127, 7);
    GFX_SetupBrush(GfxFontSize_14X10, GfxImageScale_X1, false);

    if (index >= Data_HRH && index <= Data_MINL) {
        GFX_SetCursor(0, 2);
    } else if (index == Data_W) {
        GFX_SetCursor(0, 6);
    } else {
        GFX_SetCursor(0, 4);
    }
    GFX_PrintChar('>');

    GFX_SetupBrush(GfxFontSize_7X5, GfxImageScale_X1, false);
    if (index >= Data_HRH && index <= Data_MINL) {
        GFX_SetCursor(51 + 12 * (index - Data_HRH + (index > Data_HRL)), 7);
    } else if (index == Data_W) {
        GFX_SetCursor(15, 7);
    } else {
        GFX_SetCursor(48 + 6 * (index - Data_DH + (index > Data_DL) + (index > Data_ML)), 7);
    }
    GFX_PrintChar('^');
}

static uint8_t MaxDay(uint8_t month, int16_t year)
{
    uint8_t maxDay = 30;

    switch (month)
    {
    case LL_RTC_MONTH_JANUARY:
    case LL_RTC_MONTH_MARCH:
    case LL_RTC_MONTH_MAY:
    case LL_RTC_MONTH_JULY:
    case LL_RTC_MONTH_AUGUST:
    case LL_RTC_MONTH_OCTOBER:
    case LL_RTC_MONTH_DECEMBER:
        maxDay = 31;
        break;
    case LL_RTC_MONTH_APRIL:
    case LL_RTC_MONTH_JUNE:
    case LL_RTC_MONTH_SEPTEMBER:
    case LL_RTC_MONTH_NOVEMBER:
        maxDay = 30;
        break;
    case LL_RTC_MONTH_FEBRUARY:
        year += 2000;
        if ((year % 4 == 0) && (!(year % 100 == 0) || (year % 400 == 0)))
            maxDay = 29;
        else
            maxDay = 28;
    }
    return maxDay;
}

static int8_t MaxDateTimeValue(int8_t buff[], uint8_t index)
{
    uint8_t month = buff[Data_MH] * 10 + buff[Data_ML];
    uint16_t year = buff[Data_YH] * 10 + buff[Data_YL];
    uint8_t maxDay = MaxDay(month, year);
    
    switch (index)
    {
    case Data_HRH:
    case Data_HRL:
    case Data_MINH:
    case Data_MINL:
        return MaxTimeValue(buff, index);
    case Data_YH:
    case Data_YL: 
        return 9;
    case Data_MH:   return 1;
    case Data_ML:   return buff[Data_MH] == 1 ? 2 : 9;
    case Data_DH:   return maxDay / 10;
    case Data_DL:   
        return (buff[Data_DH] == 3 || month == LL_RTC_MONTH_FEBRUARY) ? maxDay % 10 : 9;
    case Data_W:    return 7;
    default: break;
    }
    return 0;
}

static int8_t MinDateTimeValue(int8_t buff[], uint8_t index)
{
    switch (index)
    {
    case Data_DL:   return buff[Data_DH] == 0 ? 1 : 0;
    case Data_W:    return 1;
    case Data_ML:   return buff[Data_MH] == 0 ? 1 : 0;
    
    default:
        break;
    }
    return 0;
}

static AppRetCode_t process(AppSignal_t signal, void *io)
{
    const enum ScreenData SET_ORDER[11] = {
        Data_HRH, Data_HRL, Data_MINH, Data_MINL,
        Data_YH, Data_YL, Data_MH, Data_ML, Data_DH, Data_DL,
        Data_W
    };
    
    AppRetCode_t retCode = AppRetCode_OK;
    uint8_t dataIndex = SET_ORDER[screenDataIndex];
    
    switch(signal)
    {
    case AppSignal_ENTRANCE:
        screenDataIndex = 0;
        dataIndex = SET_ORDER[screenDataIndex];
        DrawLayout();
        ReadDateTime(screenDataBuffer);
        DrawIndexMark(dataIndex);
        break;

    case AppSignal_BTN0PRESS:
        if (screenDataIndex > 0) {
            screenDataIndex--;
        } else {
            retCode = AppRetCode_EXIT;
            *((struct Application **)io) = &appHome;
        }
        dataIndex = SET_ORDER[screenDataIndex];
        DrawIndexMark(dataIndex);
        break;

    case AppSignal_BTN1PRESS:
        if (screenDataBuffer[dataIndex] > MinDateTimeValue(screenDataBuffer, dataIndex))
            screenDataBuffer[dataIndex]--;
        else
            screenDataBuffer[dataIndex] = MaxDateTimeValue(screenDataBuffer, dataIndex);
        break;

    case AppSignal_BTN2PRESS:
        if (screenDataBuffer[dataIndex] < MaxDateTimeValue(screenDataBuffer, dataIndex))
            screenDataBuffer[dataIndex]++;
        else
            screenDataBuffer[dataIndex] = MinDateTimeValue(screenDataBuffer, dataIndex);
        break;

    case AppSignal_BTN3PRESS:
        if (screenDataIndex < 10) {
            screenDataIndex++;
        } else {
            WriteDateTime(screenDataBuffer);
            retCode = AppRetCode_EXIT;
            *((struct Application **)io) = &appHome;
        }
        dataIndex = SET_ORDER[screenDataIndex];
        DrawIndexMark(dataIndex);
        break;

    case AppSignal_SUSPEND:
        retCode = AppRetCode_EXIT;
        *((struct Application **)io) = &appHome;
        break;
    
    default: 
        break;
    }
    DrawData(screenDataBuffer);
    return retCode;
}

struct Application appDateTime = {
    .name = "Date&Time",
    .load = AppDummyCallback,
    .process = process,
    .save = AppDummyCallback
};