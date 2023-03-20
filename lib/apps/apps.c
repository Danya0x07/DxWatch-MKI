#include "apps.h"
#include <eeprom.h>
#include <memory.h>

#define DATETIME_EEPROM_BASE_ADDR 0
#define ALARM_EEPROM_BASE_ADDR 0x10
#define IRDATA_EEPROM_BASE_ADDR 0x1000

struct Application *const APP_LIST[] = {
    &appHome,
    &appMenu,
    &appStrobe,
    &appDateTime,
    &appAlarm,
    &appIrrecv,
    &appIrsend,
};

const int APP_NUM = sizeof(APP_LIST) / sizeof(APP_LIST[0]);

const char *const WEEKDAYS[7] = {
    "­", "‚â", "‘à", "—â", "â", "‘¡", "‚á"};

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

const struct GfxImage IMG_DDOS = {
    .bitmap = (uint8_t[]){0x24, 0x12, 0x49, 0x69, 0x49, 0x12, 0x24},
    .widthPx = 7,
    .heightPg = 1
};

int8_t screenDataBuffer[_Data_NUMOF];
uint8_t screenDataIndex;

uint8_t prevPage = 0xFF;
struct IRData irData;

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

uint8_t CalculatePageBounds(uint8_t index, uint8_t maxIndex, uint8_t numPages, uint8_t *startIdx, uint8_t *endIdx)
{
    for (uint8_t i = 0; i < numPages; i++) {
        *startIdx = 0 + 7 * i;
        *endIdx = 6 + 7 * i;
        if (*startIdx <= index && index <= *endIdx) {
            if (*endIdx > maxIndex)
                *endIdx = maxIndex;
            return i + 1;
        }
    }
    return 0;
}

void DrawOptions(uint8_t index, uint8_t maxIndex, uint8_t *prevPage, void (*DrawOption)(uint8_t idx))
{
    uint8_t page, startIdx, endIdx;

    page = CalculatePageBounds(index, maxIndex, 3, &startIdx, &endIdx);

    if (page != *prevPage) {
        GFX_SetupBrush(GfxFontSize_7X5, GfxImageScale_X1, false);
        GFX_ClearRect(6, 1, 127, 7);
        for (uint8_t i = startIdx, j = 1; i <= endIdx; i++, j++) {
            GFX_SetCursor(6, j);
            DrawOption(i);
        }
        GFX_SetCursor(110, 0);
        GFX_PrintDec(page);
        *prevPage = page;
    }
}

void DrawMarker(uint8_t index)
{
    GFX_ClearRect(0, 1, 5, 7);
    while (index > 6)
        index -= 7;
    index += 1;
    GFX_SetCursor(0, index);
    GFX_PrintChar('>');
}

/*
void SaveDateTime(const LL_RTC_DateTypeDef *date, const LL_RTC_TimeTypeDef *time)
{
    uint16_t addr = DATETIME_EEPROM_BASE_ADDR;

    //EEPROM_SetProtection(false);
    EEPROM_Write(addr, (const uint8_t *)date, sizeof(*date));
    addr += sizeof(*date);
    EEPROM_Write(addr, (const uint8_t *)time, sizeof(*time));
    //EEPROM_SetProtection(true);
}

void LoadDateTime(LL_RTC_DateTypeDef *date, LL_RTC_TimeTypeDef *time)
{
    uint16_t addr = DATETIME_EEPROM_BASE_ADDR;

    EEPROM_Read(addr, (uint8_t *)date, sizeof(*date));
    addr += sizeof(*date);
    EEPROM_Read(addr, (uint8_t *)time, sizeof(*time));
    time->TimeFormat = LL_RTC_TIME_FORMAT_AM_OR_24;
    time->Seconds = 0;
}
*/

void SaveAlarmTime(uint8_t slot, const LL_RTC_TimeTypeDef *time)
{
    if (slot > ALARM_MAX_SLOT)
        return;

    uint16_t addr = ALARM_EEPROM_BASE_ADDR + slot * sizeof(*time);

    LL_RTC_TimeTypeDef prevTime;
    LoadAlarmTime(slot, &prevTime);

    if (time->Hours != prevTime.Hours || time->Minutes != prevTime.Minutes) {
        EEPROM_SetProtection(false);
        HAL_Delay(5);
        EEPROM_Write(addr, (const uint8_t *)time, sizeof(*time));
        EEPROM_SetProtection(true);
    }
}

void LoadAlarmTime(uint8_t slot, LL_RTC_TimeTypeDef *time)
{
    if (slot > ALARM_MAX_SLOT)
        return;

    uint16_t addr = ALARM_EEPROM_BASE_ADDR + slot * sizeof(*time);
    EEPROM_Read(addr, (uint8_t *)time, sizeof(*time));
    time->TimeFormat = LL_RTC_TIME_FORMAT_AM_OR_24;
    time->Seconds = 0;
}

void SaveIRData(uint8_t slot)
{
    if (slot > IRDATA_MAX_SLOT)
        return;

    uint16_t addr = IRDATA_EEPROM_BASE_ADDR + (uint16_t)slot * sizeof(irData);
    struct IRData entry;

    EEPROM_Read(addr, (uint8_t *)&entry, sizeof(entry));

    if (memcmp(&irData, &entry, sizeof(irData))) {
        EEPROM_SetProtection(false);
        HAL_Delay(5);
        EEPROM_Write(addr, (const uint8_t *)&irData, sizeof(irData));
        EEPROM_SetProtection(true);
    }
}

void LoadIRData(uint8_t slot)
{
    if (slot > IRDATA_MAX_SLOT)
        return;

    uint16_t addr = IRDATA_EEPROM_BASE_ADDR + (uint16_t)slot * sizeof(irData);

    EEPROM_Read(addr, (uint8_t *)&irData, sizeof(irData));
    
    if (irData.len < 0 || irData.len > 100 || irData.protocol >= _IRProtocol_NUMOF) {
        irData.protocol = IRProtocol_NONE;
    }
}

int IterateIRData(void (*callback)(uint8_t slot, void *), void *arg)
{

    int activeSlots = 0;

    for (uint8_t slot_ = 0; slot_ <= IRDATA_MAX_SLOT; slot_++)
    {
        LoadIRData(slot_);
        callback(slot_, arg);
    }

    return activeSlots;
}
