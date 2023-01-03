#ifndef _DRIVERS_SH1106_PORT
#define _DRIVERS_SH1106_PORT

#include <spi.h>

#define CS_LOW()    LL_GPIO_ResetOutputPin(SH1106_CS_GPIO_Port, SH1106_CS_Pin)
#define CS_HIGH()   LL_GPIO_SetOutputPin(SH1106_CS_GPIO_Port, SH1106_CS_Pin)
#define DC_LOW()    LL_GPIO_ResetOutputPin(SH1106_DC_GPIO_Port, SH1106_DC_Pin)
#define DC_HIGH()   LL_GPIO_SetOutputPin(SH1106_DC_GPIO_Port, SH1106_DC_Pin)

#endif