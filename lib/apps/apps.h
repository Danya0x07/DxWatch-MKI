#ifndef _APPS_H
#define _APPS_H

#include <os_api.h>
#include <gfx.h>
#include <rtc.h>
#include <ir_nec.h>

enum __packed ScreenData
{
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
    Data_W, // Actually weekday (0-6).
    _Data_NUMOF
};

extern int8_t screenDataBuffer[_Data_NUMOF];
extern uint8_t screenDataIndex;

extern uint8_t prevPage;

struct IRData
{
    enum __packed IRProtocol
    {
        IRProtocol_NONE = 0,
        IRProtocol_RAW,
        IRProtocol_NEC,
        _IRProtocol_NUMOF
    } protocol;
    int8_t len;
    union
    {
        struct NecPacket necPacket;
        uint16_t durations[100];
    };
};
extern struct IRData irData;

#define IRDATA_MAX_SLOT 20
#define ALARM_MAX_SLOT 0

extern struct Application appHome;
extern struct Application appStrobe;
extern struct Application appMenu;
extern struct Application appAlarm;
extern struct Application appDateTime;
extern struct Application appIrrecv;
extern struct Application appIrsend;

extern struct Application *const APP_LIST[];
extern const int APP_NUM;

extern const char *const WEEKDAYS[7];
extern const char *const LAYOUT_DATE;
extern const char *const LAYOUT_TIME;
extern const char *const LAYOUT_VOLTAGE;

extern const struct GfxImage IMG_USBSIGN;
extern const struct GfxImage IMG_LOCK;
extern const struct GfxImage IMG_ALARM;
extern const struct GfxImage IMG_DDOS;

void AppDummyCallback(void);

void PrintDigit(uint8_t digit);
void ParseDigits(int8_t value, int8_t *tens, int8_t *units);
void ParseTime(LL_RTC_TimeTypeDef *time, int8_t buff[4]);
void ParseDate(LL_RTC_DateTypeDef *date, int8_t buff[7]);

uint8_t CalculatePageBounds(uint8_t index, uint8_t maxIndex, uint8_t numPages, uint8_t *startIdx, uint8_t *endIdx);
void DrawOptions(uint8_t index, uint8_t maxIndex, uint8_t *prevPage, void (*DrawOption)(uint8_t idx));
void DrawMarker(uint8_t index);

//void SaveDateTime(const LL_RTC_DateTypeDef *date, const LL_RTC_TimeTypeDef *time);
//void LoadDateTime(LL_RTC_DateTypeDef *date, LL_RTC_TimeTypeDef *time);
void SaveAlarmTime(uint8_t slot, const LL_RTC_TimeTypeDef *time);
void LoadAlarmTime(uint8_t slot, LL_RTC_TimeTypeDef *time);
void SaveIRData(uint8_t slot);
void LoadIRData(uint8_t slot);
int IterateIRData(void (*callback)(uint8_t slot, void *), void *arg);

#endif // _APPS_H