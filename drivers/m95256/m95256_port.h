#ifndef INC_DRIVERS_M95256_PORT_H
#define INC_DRIVERS_M95256_PORT_H

#include <spi.h>

#define S_LOW()     LL_GPIO_ResetOutputPin(S_GPIO_Port, S_Pin)
#define S_HIGH()    LL_GPIO_SetOutputPin(S_GPIO_Port, S_Pin)
#define W_LOW()     LL_GPIO_ResetOutputPin(W_GPIO_Port, W_Pin)
#define W_HIGH()    LL_GPIO_SetOutputPin(W_GPIO_Port, W_Pin)
#define H_LOW()     LL_GPIO_ResetOutputPin(HOLD_GPIO_Port, HOLD_Pin)
#define H_HIGH()    LL_GPIO_SetOutputPin(HOLD_GPIO_Port, HOLD_Pin)

#define W_IS_LOW()  (!LL_GPIO_IsOutputPinSet(W_GPIO_Port, W_Pin))
#define GET_MS()    HAL_GetTick()

#endif // INC_DRIVERS_M95256_PORT_H