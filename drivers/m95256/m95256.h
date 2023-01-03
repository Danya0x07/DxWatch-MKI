/** M95256-WMN SPI EEPROM driver. */

#ifndef INC_DRIVERS_M95256
#define INC_DRIVERS_M95256

#include <main.h>

#define M95_PAGE_SIZE   64

typedef enum {
    M95_Error_NONE = 0,
    M95_Error_LOCKED,
    M95_Error_TIMEOUT,
    M95_Error_NONSENSE
} M95_Error_t;

typedef enum {
    M95_WriteProtection_NONE = 0,
    M95_WriteProtection_QUATER,
    M95_WriteProtection_HALF,
    M95_WriteProtection_WHOLE
} M95_WriteProtection_t;

M95_Error_t M95256_Init(void);
M95_Error_t M95256_WriteArray(uint16_t addr, const uint8_t *arr, uint16_t len);
M95_Error_t M95256_ReadArray(uint16_t addr, uint8_t *arr, uint16_t len);
M95_Error_t M95256_SetWriteProtection(M95_WriteProtection_t protection);
void M95256_SetHardwareProtection(bool enable);

#endif // INC_DRIVERS_M95256