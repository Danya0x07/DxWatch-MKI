#ifndef _APPS_H
#define _APPS_H

#include <os_api.h>
#include <gfx.h>

extern struct Application appHome;
extern struct Application *const APP_LIST[];
extern const int APP_NUM;

extern const char *WEEKDAYS[7];
const struct GfxImage IMG_USBSIGN;
const struct GfxImage IMG_LOCK;
const struct GfxImage IMG_ALARM;

void AppDummyCallback(void);

#endif // _APPS_H