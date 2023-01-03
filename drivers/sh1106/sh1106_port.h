#ifndef _DRIVERS_SH1106_PORT
#define _DRIVERS_SH1106_PORT

#include <spi.h>

#define CS_LOW()    LL_GPIO_ResetOutputPin(CS_GPIO_Port, CS_Pin)
#define CS_HIGH()   LL_GPIO_SetOutputPin(CS_GPIO_Port, CS_Pin)
#define DC_LOW()    LL_GPIO_ResetOutputPin(DC_GPIO_Port, DC_Pin)
#define DC_HIGH()   LL_GPIO_SetOutputPin(DC_GPIO_Port, DC_Pin)

#endif