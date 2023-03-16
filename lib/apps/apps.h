#ifndef _APPS_H
#define _APPS_H

#include <os_api.h>
#include <gfx.h>
#include <rtc.h>

extern struct Application appHome;
extern struct Application appMenu;
extern struct Application appStrobe;
extern struct Application appAlarm;
extern struct Application appDateTime;
extern struct Application *const APP_LIST[];
extern const int APP_NUM;

extern const char *const WEEKDAYS[7];
extern const char *const LAYOUT_DATE;
extern const char *const LAYOUT_TIME;
extern const char *const LAYOUT_VOLTAGE;

extern const struct GfxImage IMG_USBSIGN;
extern const struct GfxImage IMG_LOCK;
extern const struct GfxImage IMG_ALARM;

enum __packed ScreenData {
    Data_S, // Alarm status
    Data_HRH,
    Data_HRL,
    Data_MINH,
    Data_MINL,
    Data_DH,
    Data_DL,
    Data_MH,
    Data_ML,
    Data_YH,
    Data_YL,
    Data_W,  // Actually weekday (0-6).
    _Data_NUMOF
};

extern int8_t screenDataBuffer[_Data_NUMOF];
extern uint8_t screenDataIndex;

void AppDummyCallback(void);

void PrintDigit(uint8_t digit);
void ParseDigits(int8_t value, int8_t *tens, int8_t *units);
void ParseTime(LL_RTC_TimeTypeDef *time, int8_t buff[4]);
void ParseDate(LL_RTC_DateTypeDef *date, int8_t buff[7]);

#endif // _APPS_H