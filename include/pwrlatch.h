#ifndef _PWRLATCH_H
#define _PWRLATCH_H

#include "main.h"

#define PWRLATCH_ON()   LL_GPIO_SetOutputPin(PWR_LATCH_GPIO_Port, PWR_LATCH_Pin)
#define PWRLATCH_OFF()  LL_GPIO_ResetOutputPin(PWR_LATCH_GPIO_Port, PWR_LATCH_Pin)
#define PWRLATCH_IS_ON()    LL_GPIO_IsOutputPinSet(PWR_LATCH_GPIO_Port, PWR_LATCH_Pin)

#endif // _PWRLATCH_H