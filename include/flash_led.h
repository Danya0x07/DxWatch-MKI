#ifndef _FLASH_LED_H
#define _FLASH_LED_H

#include "main.h"

#define FLASH_LED_ON()  LL_GPIO_SetOutputPin(FLASH_LED_GPIO_Port, FLASH_LED_Pin)
#define FLASH_LED_OFF() LL_GPIO_ResetOutputPin(FLASH_LED_GPIO_Port, FLASH_LED_Pin)
#define FLASH_LED_TOGGLE()  LL_GPIO_TogglePin(FLASH_LED_GPIO_Port, FLASH_LED_Pin)
#define FLASH_LED_IS_ON()   LL_GPIO_IsOutputPinSet(FLASH_LED_GPIO_Port, FLASH_LED_Pin)

#endif // _FLASH_LED_H