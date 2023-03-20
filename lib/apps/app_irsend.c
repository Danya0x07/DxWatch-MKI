#include "apps.h"
#include <builtin_led.h>
#include <ir_raw.h>
#include <ir_nec.h>
#include <stdio.h>

extern void DrawOption(uint8_t index);

static void DrawLayout(void)
{
    GFX_Clear();
    GFX_SetupBrush(GfxFontSize_7X5, GfxImageScale_X1, false);
    GFX_SetCursor(43, 0);
    GFX_PrintString("IR send");
}

static AppRetCode_t process(AppSignal_t signal, void *io)
{
    AppRetCode_t retCode = AppRetCode_OK;
    
    switch(signal)
    {
    case AppSignal_ENTRANCE:
        screenDataIndex = 0;
        prevPage = 0;
        DrawLayout();
        DrawOptions(screenDataIndex, IRDATA_MAX_SLOT, &prevPage, DrawOption);
        DrawMarker(screenDataIndex);
        break;

    case AppSignal_BTN0PRESS:
        retCode = AppRetCode_EXIT;
        *((struct Application **)io) = &appHome;
        break;

    case AppSignal_BTN1PRESS:
        if (screenDataIndex > 0)
            screenDataIndex--;
        else
            screenDataIndex = IRDATA_MAX_SLOT;
        DrawOptions(screenDataIndex, IRDATA_MAX_SLOT, &prevPage, DrawOption);
        DrawMarker(screenDataIndex);
        break;

    case AppSignal_BTN2PRESS:
        if (screenDataIndex < IRDATA_MAX_SLOT)
            screenDataIndex++;
        else
            screenDataIndex = 0;
        DrawOptions(screenDataIndex, IRDATA_MAX_SLOT, &prevPage, DrawOption);
        DrawMarker(screenDataIndex);
        break;

    case AppSignal_BTN3PRESS:
        LoadIRData(screenDataIndex);

        if (irData.protocol == IRProtocol_NEC) {
            NEC_BuildRaw(&irData.necPacket, irData.durations);
        }
        if (irData.protocol != IRProtocol_NONE) {
            GFX_DrawImage(14, 0, &IMG_DDOS);
            IR_SendRaw(irData.durations, irData.len);
            GFX_ClearRect(14, 0, 21, 0);
        }
        break;

    case AppSignal_LOWVOLTAGE:
        GFX_SetCursor(0, 0);
        GFX_PrintChar('!');
        break;

    case AppSignal_RTCALARM:
        GFX_DrawImage(6, 0, &IMG_ALARM);
        break;
    
    case AppSignal_SUSPEND:
        retCode = AppRetCode_STAYUP;
        break;
    
    default: break;
    }
    return retCode;
}

struct Application appIrsend = {
    .name = "IR send",
    .load = AppDummyCallback,
    .process = process,
    .save = AppDummyCallback
};