#include <embedded_common.h>
#include <m95256.h>
#include <builtin_led.h>
#include <string.h>
#include <memory.h>

#define TEST_EEPROM_QUATER_ADDR 0x7000
#define TEST_EEPROM_HALF_ADDR   0x5000
#define TEST_EEPROM_START_ADDR  0x3500

void Global_Setup(void)
{
    blink(1, 2000);
    blink(M95256_Init(), 200);
    HAL_Delay(3000);
}

void Global_TearDown(void)
{
    M95256_SetHardwareProtection(false);
    M95256_SetWriteProtection(M95_WriteProtection_NONE);

    uint8_t byte;
    for (uint16_t addr = 0; addr < 0x8000; addr++) {
        M95256_ReadArray(addr, &byte, 1);
        if (byte != 0xFF) {
            byte = 0xFF;
            M95256_WriteArray(addr, &byte, 1);
        }
    }

    M95256_SetWriteProtection(M95_WriteProtection_WHOLE);
    M95256_SetHardwareProtection(true);
    BUILTIN_LED_ON();
}

void InfiniteLoop(void) {}

void setUp(void)
{
    M95256_SetHardwareProtection(false);
    M95256_SetWriteProtection(M95_WriteProtection_NONE);
}

void tearDown(void) 
{
    HAL_Delay(3000);
}

void Test_InitialMemoryState(void)
{
    blink(1, 3000);
    uint8_t content[128];

    blink(M95256_ReadArray(TEST_EEPROM_START_ADDR, content, sizeof(content)), 200);
    HAL_Delay(1000);
    for (int i = 0; i < sizeof(content); i++) {
        if (content[i] != 0xFF) {
            blink(1, 500);
            break;
        }
    }
}

void Test_WriteOneByte(void)
{
    blink(2, 3000);
    uint8_t b1 = 0xDA, b2 = 0;

    blink(M95256_WriteArray(TEST_EEPROM_START_ADDR, &b1, 1), 200);
    HAL_Delay(1000);
    blink(M95256_ReadArray(TEST_EEPROM_START_ADDR, &b2, 1), 200);
    HAL_Delay(1000);
    if (b2 != b1)
        blink(1, 500);
    HAL_Delay(1000);
    blink(M95256_ReadArray(TEST_EEPROM_START_ADDR - 1, &b2, 1), 200);
    HAL_Delay(1000);
    if (b2 == b1)
        blink(2, 500);
    HAL_Delay(1000);
    blink(M95256_ReadArray(TEST_EEPROM_START_ADDR + 1, &b2, 1), 200);
    HAL_Delay(1000);
    if (b2 == b1)
        blink(3, 500);
}

void Test_WriteArrayToOnePage(void)
{
    blink(3, 3000);
    uint8_t content[32];
    uint8_t actual[34];
    memset(content, 0xBC, sizeof(content));

    blink(M95256_WriteArray(TEST_EEPROM_START_ADDR + 1, content, sizeof(content)), 200);
    HAL_Delay(1000);
    blink(M95256_ReadArray(TEST_EEPROM_START_ADDR, actual, sizeof(content) + 2), 200);
    HAL_Delay(1000);
    if (actual[0] == content[0])
        blink(1, 500);
    HAL_Delay(1000);
    if (actual[33] == content[31])
        blink(2, 500);
    HAL_Delay(1000);
    for (int i = 0; i < sizeof(content); i++) {
        if (actual[i + 1] != 0xBC) {
            blink(3, 500);
            break;
        }
    }
}

void Test_WriteMultiplePages(void)
{
    blink(4, 3000);
    uint8_t content[128];
    uint8_t actual[130];
    memset(content, 0xEE, sizeof(content));

    blink(M95256_WriteArray(TEST_EEPROM_START_ADDR + 12, content, sizeof(content)), 200);
    HAL_Delay(1000);
    blink(M95256_ReadArray(TEST_EEPROM_START_ADDR + 11, actual, sizeof(content) + 2), 200);
    HAL_Delay(1000);
    if (actual[0] == content[0])
        blink(1, 500);
    HAL_Delay(1000);
    if (actual[129] == content[127])
        blink(2, 500);
    HAL_Delay(1000);
    for (int i = 0; i < sizeof(content); i++) {
        if (actual[i + 1] != 0xEE) {
            blink(3, 500);
            break;
        }
    }
}

void Test_QuaterProtection(void)
{
    blink(5, 3000);
    uint8_t b1 = 0x11;

    blink(M95256_WriteArray(TEST_EEPROM_QUATER_ADDR, &b1, 1), 200);
    HAL_Delay(1000);
    blink(M95256_SetWriteProtection(M95_WriteProtection_QUATER), 200);
    HAL_Delay(1000);
    b1 = 0x22;
    blink(M95256_WriteArray(TEST_EEPROM_QUATER_ADDR, &b1, 1), 200);  // should blink once
    HAL_Delay(1000);
    blink(M95256_ReadArray(TEST_EEPROM_QUATER_ADDR, &b1, 1), 200);
    HAL_Delay(1000);
    if (b1 != 0x11) {
        blink(1, 500);
    }
}

void Test_HardwareProtection(void)
{
    blink(6, 3000);
    
    blink(M95256_SetWriteProtection(M95_WriteProtection_WHOLE), 200);
    HAL_Delay(1000);
    M95256_SetHardwareProtection(true);
    blink(M95256_SetWriteProtection(M95_WriteProtection_NONE) == M95_Error_NONE, 500);
}

EXPORT_TEST_CASES(
    Test_InitialMemoryState,
    Test_WriteOneByte,
    Test_WriteArrayToOnePage,
    Test_WriteMultiplePages,
    Test_QuaterProtection,  // 1 fast blink
    Test_HardwareProtection
);
