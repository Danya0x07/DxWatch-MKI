#include "apps.h"
#include <builtin_led.h>
#include <ir_raw.h>
#include <ir_nec.h>
#include <irtim.h>

#define IR_CARRIER_MIN  32
#define IR_CARRIER_MAX  40

extern void DrawOption(uint8_t index);

static void DrawLayout(void)
{
    GFX_Clear();
    GFX_SetupBrush(GfxFontSize_7X5, GfxImageScale_X1, false);
    GFX_SetCursor(43, 0);
    GFX_PrintString("IR ddos");

    GFX_SetCursor(0, 3);
    GFX_PrintString("Carrier: ??kHz");
}

static void DrawData(void)
{
    GFX_SetCursor(54, 3);
    GFX_PrintDec(screenDataIndex);
}

static AppRetCode_t process(AppSignal_t signal, void *io)
{
    AppRetCode_t retCode = AppRetCode_OK;
    
    switch(signal)
    {
    case AppSignal_ENTRANCE:
        screenDataIndex = IRTIM_GetCarrierFrequency();
        DrawLayout();
        DrawData();
        break;

    case AppSignal_BTN0PRESS:
        retCode = AppRetCode_EXIT;
        *((struct Application **)io) = &appHome;
        break;

    case AppSignal_BTN1PRESS:
        if (screenDataIndex > IR_CARRIER_MIN)
            screenDataIndex--;
        else
            screenDataIndex = IR_CARRIER_MAX;
        IRTIM_SetCarrierFrequency(screenDataIndex);
        DrawData();
        break;

    case AppSignal_BTN2PRESS:
        if (screenDataIndex < IR_CARRIER_MAX)
            screenDataIndex++;
        else
            screenDataIndex = IR_CARRIER_MIN;
        IRTIM_SetCarrierFrequency(screenDataIndex);
        DrawData();
        break;

    case AppSignal_BTN3PRESS:
        if (IRTIM_CarrierEnabled()) {
            IRTIM_DisableCarrier();
            GFX_ClearRect(12, 0, 19, 0);
        } else {
            IRTIM_EnableCarrier();
            GFX_DrawImage(12, 0, &IMG_DDOS);
        }
        break;

    case AppSignal_LOWVOLTAGE:
        GFX_SetCursor(0, 0);
        GFX_PrintChar('!');
        break;

    case AppSignal_RTCALARM:
        GFX_DrawImage(6, 0, &IMG_ALARM);
        break;
    
    default: break;
    }
    return retCode;
}

struct Application appIrddos = {
    .name = "IR ddos",
    .load = AppDummyCallback,
    .process = process,
    .save = AppDummyCallback
};