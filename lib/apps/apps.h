#ifndef _APPS_H
#define _APPS_H

#include <os_api.h>

extern struct Application appHome;

extern struct Application *const APP_LIST[];
extern const int APP_NUM;

void AppDummyCallback(void);

#endif // _APPS_H