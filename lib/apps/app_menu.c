#include "apps.h"
#include <motor.h>
#include <flash_led.h>

#define NUM_PAGES   2

static void DrawOption(uint8_t index)
{
    index += 3;
    GFX_PrintString(APP_LIST[index]->name);
}

static void DrawLayout(void)
{
    GFX_Clear();
    GFX_SetupBrush(GfxFontSize_7X5, GfxImageScale_X1, false);
    GFX_SetCursor(52, 0);
    GFX_PrintString("Apps");
}

static AppRetCode_t process(AppSignal_t signal, void *io)
{
    AppRetCode_t retCode = AppRetCode_OK;
    
    switch (signal)
    {
    case AppSignal_ENTRANCE:
        screenDataIndex = 0;
        prevPage = 0xFF;
        DrawLayout();
        DrawOptions(screenDataIndex, APP_NUM - 4, &prevPage, DrawOption);
        DrawMarker(screenDataIndex);
        break;

    case AppSignal_RTCALARM:
        FLASH_LED_OFF();
        MOTOR_ON();
    case AppSignal_BTN0PRESS:
    case AppSignal_SUSPEND:
        retCode = AppRetCode_EXIT;
        *((struct Application **)io) = &appHome;
        break;

    case AppSignal_BTN1PRESS:
        if (screenDataIndex > 0) {
            screenDataIndex--;
            DrawOptions(screenDataIndex, APP_NUM - 4, &prevPage, DrawOption);
            DrawMarker(screenDataIndex);
        }
        break;

    case AppSignal_BTN2PRESS:
        if (screenDataIndex < APP_NUM - 4) {
            screenDataIndex++;
            DrawOptions(screenDataIndex, APP_NUM - 4, &prevPage, DrawOption);
            DrawMarker(screenDataIndex);
        }
        break;

    case AppSignal_BTN3PRESS:
        retCode = AppRetCode_EXIT;
        *((struct Application **)io) = APP_LIST[screenDataIndex + 3];
        break;
    
    default:
        break;
    }

    return retCode;
}

struct Application appMenu = {
    .name = "Apps",
    .load = AppDummyCallback,
    .process = process,
    .save = AppDummyCallback
};