#ifndef _DRIVERS_M95256_PORT_H
#define _DRIVERS_M95256_PORT_H

#include <spi.h>
#include <tim.h>

#define S_LOW()     LL_GPIO_ResetOutputPin(M95_S_GPIO_Port, M95_S_Pin)
#define S_HIGH()    LL_GPIO_SetOutputPin(M95_S_GPIO_Port, M95_S_Pin)
#define W_LOW()     LL_GPIO_ResetOutputPin(M95_W_GPIO_Port, M95_W_Pin)
#define W_HIGH()    LL_GPIO_SetOutputPin(M95_W_GPIO_Port, M95_W_Pin)
#define W_IS_LOW()  (!LL_GPIO_IsOutputPinSet(M95_W_GPIO_Port, M95_W_Pin))
#define H_LOW()     LL_GPIO_ResetOutputPin(M95_HOLD_GPIO_Port, M95_HOLD_Pin)
#define H_HIGH()    LL_GPIO_SetOutputPin(M95_HOLD_GPIO_Port, M95_HOLD_Pin)

#define MS_GET()    HAL_GetTick()
#define MS_WAIT(ms) HAL_Delay((ms))

#endif // _DRIVERS_M95256_PORT_H