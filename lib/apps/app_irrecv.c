#include "apps.h"
#include <builtin_led.h>
#include <ir_raw.h>
#include <ir_nec.h>
#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>

void DrawOption(uint8_t index)
{
    static const char *const PROTOCOLS_STR[] = {"---", "RAW", "NEC"};
    LoadIRData(index);
    
    GFX_PrintString(PROTOCOLS_STR[irData.protocol]);
    GFX_PrintChar(' ');
    switch (irData.protocol)
    {
    default:
    case IRProtocol_NONE:
        //snprintf(buff, sizeof(buff), " %02x%02x", irData.necPacket.address, irData.necPacket.command);
        if (irData.len == IR_OVERFLOW) {
            GFX_PrintString("ovf");
        } else if (irData.len == IR_BREAK) {
            GFX_PrintString("brk");
        }
        break;
    
    case IRProtocol_RAW:
        GFX_PrintDec(irData.len);
        break;
    
    case IRProtocol_NEC:
        GFX_PrintChar('$');
        GFX_PrintHex(irData.necPacket.address >> 4);
        GFX_PrintHex(irData.necPacket.address & 0xF);
        GFX_PrintHex(irData.necPacket.command >> 4);
        GFX_PrintHex(irData.necPacket.command & 0xF);
    }
}

static void DrawLayout(void)
{
    GFX_Clear();
    GFX_SetupBrush(GfxFontSize_7X5, GfxImageScale_X1, false);
    GFX_SetCursor(43, 0);
    GFX_PrintString("IR recv");
}

static AppRetCode_t process(AppSignal_t signal, void *io)
{
    AppRetCode_t retCode = AppRetCode_OK;
    
    switch(signal)
    {
    case AppSignal_ENTRANCE:
        screenDataIndex = 0;
        irData.protocol = IRProtocol_NONE;
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
    {
        BUILTIN_LED_ON();
        irData.len = IR_RecordRaw(irData.durations, sizeof(irData.durations) / sizeof(uint16_t), 3000, 10000);
        BUILTIN_LED_OFF();

        struct NecPacket packet;
        NecParsingStatus_t status = NEC_ParseRaw(irData.durations, irData.len, &packet);
        
        if (irData.len < 0) {
            irData.protocol = IRProtocol_NONE;
        } else if (status == NecParsingStatus_OK) {
            irData.protocol = IRProtocol_NEC;
            irData.necPacket = packet;
        } else {
            irData.protocol = IRProtocol_RAW;
        }
        SaveIRData(screenDataIndex);
        prevPage = 0;
        DrawOptions(screenDataIndex, IRDATA_MAX_SLOT, &prevPage, DrawOption);
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

struct Application appIrrecv = {
    .name = "IR recv",
    .load = AppDummyCallback,
    .process = process,
    .save = AppDummyCallback
};