#include "apps.h"

struct Application *const APP_LIST[] = {
    &appHome
};

const int APP_NUM = sizeof(APP_LIST) / sizeof(APP_LIST[0]);

void AppDummyCallback(void) {}