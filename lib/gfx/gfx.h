#ifndef _GFX_H
#define _GFX_H

#include <libcommon.h>

struct GfxImage {
    const uint8_t *bitmap;
    uint8_t widthPx;
    uint8_t heightPg;
};

enum GfxFontSize {
    GfxFontSize_7X5 = 1,
    GfxFontSize_14X10,
    GfxFontSize_21X15
};

enum GfxImageScale {
    GfxImageScale_X1 = 1,
    GfxImageScale_X2,
    GfxImageScale_X3,
    GfxImageScale_X4,
    GfxImageScale_X5,
    GfxImageScale_X6,
    GfxImageScale_X7,
    GfxImageScale_X8
};

void GFX_SetupBrush(enum GfxFontSize fontSize, enum GfxImageScale imageScale, bool inverse);
void GFX_SetCursor(uint8_t x, uint8_t p);

void GFX_PrintChar(char c);
void GFX_PrintString(const char *s);
void GFX_PrintStringInFrame(uint8_t x0, uint8_t x1, uint8_t p1, const char *s);
void GFX_PrintDec(int n);
void GFX_PrintHex(int n);

void GFX_DrawImage(uint8_t x0, uint8_t p0, const struct GfxImage *image);

void GFX_Clear(void);
void GFX_ClearRect(uint8_t x0, uint8_t p0, uint8_t x1, uint8_t p1);

#endif