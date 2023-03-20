#include "eeprom.h"

extern void vAssertCalled(char *file, uint32_t line);
#define DEBUG_LOGI(i, msg) vAssertCalled(msg, i)

void EEPROM_Init(void)
{
    M95_Error_t err = M95256_Init();
    if (err)
        DEBUG_LOGI(err, "EEPROM init");
    EEPROM_SetProtection(false);
}

void EEPROM_SetProtection(bool protection)
{
    M95_Error_t err;

    M95256_SetHardwareProtection(false);
    if (protection) {
        err = M95256_SetWriteProtection(M95_WriteProtection_WHOLE);
        M95256_SetHardwareProtection(true);
    } else {
        err = M95256_SetWriteProtection(M95_WriteProtection_NONE);
    }
    if (err)
        DEBUG_LOGI(err, "EEPROM sp");
}

void EEPROM_Write(uint16_t addr, const uint8_t *arr, uint16_t len)
{
    M95_Error_t err = M95256_WriteArray(addr, arr, len);
    if (err)
        DEBUG_LOGI(err, "EEPROM w");
}

void EEPROM_Read(uint16_t addr, uint8_t *arr, uint16_t len)
{
    M95_Error_t err = M95256_ReadArray(addr, arr, len);
    if (err)
        DEBUG_LOGI(err, "EEPROM r");
}
