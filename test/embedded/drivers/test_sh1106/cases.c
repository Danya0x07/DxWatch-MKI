#include "embedded_common.h"
#include <sh1106.h>
#include <string.h>

static void FillScreenWithDummyContent(void)
{
    for (uint8_t row = 0; row < SH1106_NUM_ROWS; row++) {
        SH1106_SetCursor(row, 0);
        for (uint8_t col = 0; col < SH1106_NUM_COLS; col++) {
            SH1106_WriteByte(0xF << 2);
        }
    }
}

static void ClearScreen(void)
{
    for (uint8_t row = 0; row < SH1106_NUM_ROWS; row++) {
        SH1106_SetCursor(row, 0);
        for (uint8_t col = 0; col < SH1106_NUM_COLS; col++) {
            SH1106_WriteByte(0);
        }
    }
}

void Global_Setup(void)
{
    SH1106_Init();
}

void Global_TearDown(void)
{
    SH1106_SetEnable(false);
}

void InfiniteLoop(void) {}

void setUp(void)
{
    ClearScreen();
    SH1106_SetContrast(0x7F);
    SH1106_SetFullOn(false);
    SH1106_SetInverse(false);
    SH1106_SetOffset(0);
    SH1106_SetEnable(true);
}

void tearDown(void)
{
    HAL_Delay(2000);
}

void Test_SetCursorAndWriteByte(void)
{
    SH1106_SetCursor(0, 0);
    SH1106_WriteByte(0x0F);

    SH1106_SetCursor(SH1106_MAX_ROW, 0);
    SH1106_WriteByte(0xF0);

    SH1106_SetCursor(0, SH1106_MAX_COL);
    SH1106_WriteByte(0x0F);

    SH1106_SetCursor(SH1106_MAX_ROW, SH1106_MAX_COL);
    SH1106_WriteByte(0xF0);
}

void Test_WriteBytes(void)
{
    uint8_t content[SH1106_NUM_COLS];

    memset(content, 0x0F, SH1106_NUM_COLS);
    SH1106_SetCursor(0, 0);
    SH1106_WriteBytes(content, SH1106_NUM_COLS);

    memset(content, 0xF0, SH1106_NUM_COLS);
    SH1106_SetCursor(SH1106_MAX_ROW, 0);
    SH1106_WriteBytes(content, SH1106_NUM_COLS);
}

void Test_PixelsInversion(void)
{
    FillScreenWithDummyContent();
    HAL_Delay(1000);
    SH1106_SetInverse(true);
    HAL_Delay(2000);
    SH1106_SetInverse(false);
}

void Test_FullOn(void)
{
    FillScreenWithDummyContent();
    HAL_Delay(1000);
    SH1106_SetFullOn(true);
}

void Test_ChangeContrast(void)
{
    FillScreenWithDummyContent();
    HAL_Delay(1000);
    for (uint16_t contrast = 0; contrast < 256; contrast += 5) {
        SH1106_SetContrast(contrast);
        HAL_Delay(100);
    }
}

void Test_ContentOffset(void)
{
    FillScreenWithDummyContent();
    HAL_Delay(1000);
    for (uint8_t offset = 0; offset < SH1106_NUM_ROWS * 8 / 2; offset++) {
        SH1106_SetOffset(offset);
        HAL_Delay(200);
    }
}

void Test_DisableEnable(void)
{
    FillScreenWithDummyContent();
    HAL_Delay(1000);
    SH1106_SetEnable(false);
    HAL_Delay(2000);
    SH1106_SetEnable(true);
}

EXPORT_TEST_CASES (
    Test_SetCursorAndWriteByte,
    Test_WriteBytes,
    Test_PixelsInversion,
    Test_FullOn,
    Test_ChangeContrast,
    Test_ContentOffset,
    Test_DisableEnable
);
