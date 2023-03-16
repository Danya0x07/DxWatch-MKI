#include "apps.h"
#include <motor.h>
#include <flash_led.h>

static void DrawLayout(void)
{
    GFX_Clear();
    GFX_SetupBrush(GfxFontSize_7X5, GfxImageScale_X1, false);
    GFX_SetCursor(52, 0);
    GFX_PrintString("Apps");
}

static void DrawOptions(uint8_t index)
{
    static uint8_t prevIndex;

    if (index != prevIndex) {
        uint8_t startIdx = 3, endIdx = 9;

        if (3 <= index && index <= 9)
            startIdx = 3, endIdx = 9;
        else if (10 <= index && index <= 16)
            startIdx = 10, endIdx = 16;
        else if (17 <= index && index <= 23)
            startIdx = 17, endIdx = 23;
        
        if (endIdx > APP_NUM - 1)
            endIdx = APP_NUM - 1;
        
        GFX_ClearRect(6, 1, 127, 7);
        for (uint8_t i = startIdx, j = 1; i <= endIdx; i++, j++) {
            GFX_SetCursor(6, j);
            GFX_PrintString(APP_LIST[i]->name);
        }
        prevIndex = index;
    }
}

static void DrawMarker(uint8_t index)
{
    GFX_ClearRect(0, 1, 5, 7);
    while (index > 9)
        index -= 7;
    index -= 2;
    GFX_SetCursor(0, index);
    GFX_PrintChar('>');
}

static AppRetCode_t process(AppSignal_t signal, void *io)
{
    AppRetCode_t retCode = AppRetCode_OK;
    
    switch (signal)
    {
    case AppSignal_ENTRANCE:
        screenDataIndex = 3;
        DrawLayout();
        DrawOptions(screenDataIndex);
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
        if (screenDataIndex > 3) {
            screenDataIndex--;
            DrawOptions(screenDataIndex);
            DrawMarker(screenDataIndex);
        }
        break;

    case AppSignal_BTN2PRESS:
        if (screenDataIndex < APP_NUM - 1) {
            screenDataIndex++;
            DrawOptions(screenDataIndex);
            DrawMarker(screenDataIndex);
        }
        break;

    case AppSignal_BTN3PRESS:
        retCode = AppRetCode_EXIT;
        *((struct Application **)io) = APP_LIST[screenDataIndex];
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