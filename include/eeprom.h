#ifndef _EEPROM_H
#define _EEPROM_H

#include <m95256.h>

void EEPROM_Init(void);
void EEPROM_SetProtection(bool protection);
void EEPROM_Write(uint16_t addr, const uint8_t *arr, uint16_t len);
void EEPROM_Read(uint16_t addr, uint8_t *arr, uint16_t len);

#endif // _EEPROM_H