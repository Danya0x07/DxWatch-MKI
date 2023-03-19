#include "apps.h"

struct Application *const APP_LIST[] = {
    &appHome,
    &appMenu,
    &appStrobe,
    &appDateTime,
    &appAlarm
};

const int APP_NUM = sizeof(APP_LIST) / sizeof(APP_LIST[0]);

const char *const WEEKDAYS[7] = {
    "­", "‚â", "‘à", "—â", "â", "‘¡", "‚á"
};

const char *const LAYOUT_DATE = "??.??.??";
const char *const LAYOUT_TIME = "??:??";
const char *const LAYOUT_VOLTAGE = "?.?v";

const struct GfxImage IMG_USBSIGN = {
    .bitmap = (uint8_t[]){0x1C, 0x20, 0xFF, 0x08, 0x07, 0x00},
    .widthPx = 6,
    .heightPg = 1
};

const struct GfxImage IMG_LOCK = {
    .bitmap = (uint8_t[]){0xF0, 0xFE, 0xF3, 0xF1, 0xF1, 0xF3, 0xFE, 0xF0},
    .widthPx = 8,
    .heightPg = 1
};

const struct GfxImage IMG_ALARM = {
    .bitmap = (uint8_t[]){0x78, 0x4C, 0x87, 0xA5, 0x97, 0x4C, 0x78},
    .widthPx = 7,
    .heightPg = 1
};

int8_t screenDataBuffer[_Data_NUMOF];
uint8_t screenDataIndex;

void AppDummyCallback(void) 
{
    OS_StopCustomTimer();
}

void PrintDigit(uint8_t digit)
{
    GFX_PrintChar('0' + digit);
}

void ParseDigits(int8_t value, int8_t *tens, int8_t *units)
{
    *tens = value / 10;
    *units = value - *tens * 10;
}

void ParseTime(LL_RTC_TimeTypeDef *time, int8_t buff[4])
{
    ParseDigits(time->Hours, &buff[0], &buff[1]);
    ParseDigits(time->Minutes, &buff[2], &buff[3]);
}

void ParseDate(LL_RTC_DateTypeDef *date, int8_t buff[7])
{
    ParseDigits(date->Day, &buff[0], &buff[1]);
    ParseDigits(date->Month, &buff[2], &buff[3]);
    ParseDigits(date->Year, &buff[4], &buff[5]);
    buff[6] = date->WeekDay;
}