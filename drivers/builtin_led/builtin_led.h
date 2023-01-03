#ifndef INC_DRIVERS_BUILTIN_LED
#define INC_DRIVERS_BUILTIN_LED

#include <main.h>

#define BUILTIN_LED_ON()    LL_GPIO_SetOutputPin(LED_BUILTIN_GPIO_Port, LED_BUILTIN_Pin)
#define BUILTIN_LED_OFF()   LL_GPIO_ResetOutputPin(LED_BUILTIN_GPIO_Port, LED_BUILTIN_Pin)
#define BUILTIN_LED_TOGGLE()    LL_GPIO_TogglePin(LED_BUILTIN_GPIO_Port, LED_BUILTIN_Pin)

#endif // INC_DRIVERS_BUILTIN_LED