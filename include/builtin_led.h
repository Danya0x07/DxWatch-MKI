#ifndef _BUILTIN_LED_H
#define _BUILTIN_LED_H

#include "main.h"

#define BUILTIN_LED_ON()    LL_GPIO_SetOutputPin(BUILTIN_LED_GPIO_Port, BUILTIN_LED_Pin)
#define BUILTIN_LED_OFF()   LL_GPIO_ResetOutputPin(BUILTIN_LED_GPIO_Port, BUILTIN_LED_Pin)
#define BUILTIN_LED_TOGGLE()    LL_GPIO_TogglePin(BUILTIN_LED_GPIO_Port, BUILTIN_LED_Pin)

#endif // _BUILTIN_LED_H