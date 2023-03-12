#include "apps.h"

struct Application *const APP_LIST[] = {
    &appHome
};

const int APP_NUM = sizeof(APP_LIST) / sizeof(APP_LIST[0]);

const char *WEEKDAYS[7] = {
    "è≠", "Ç‚", "ë‡", "ó‚", "è‚", "ë°", "Ç·"
};

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

void AppDummyCallback(void) 
{
    OS_StopCustomTimer();
}