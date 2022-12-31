#ifndef INC_DRIVERS_SH1106
#define INC_DRIVERS_SH1106

#include <main.h>

void SH1106_Init(void);

void SH1106_SetCursor(uint8_t row, uint8_t col);

void SH1106_WriteByte(uint8_t data);

void SH1106_WriteBytes(uint8_t *data, uint16_t len);

void SH1106_FullOn(bool on);

void SH1106_Invert(bool invert);

void SH1106_SetEnable(bool enable);

void SH1106_SetContrast(uint8_t contrast);

void SH1106_SetOffset(uint8_t offset);

#endif // INC_DRIVERS_SH1106