#ifndef _IR_PORT_H
#define _IR_PORT_H

#ifndef PIO_UNIT_TESTING

#include <gpio.h>
#include <irtim.h>
#include <tim.h>

#define EYE_IS_HIGH()	LL_GPIO_IsInputPinSet(IR_EYE_GPIO_Port, IR_EYE_Pin)
#define CARRIER_ON()    IRTIM_EnableCarrier()
#define CARRIER_OFF()   IRTIM_DisableCarrier()
#define LED_BLINK()     IRTIM_SetLedState(1)
#define LED_NOBLINK()   IRTIM_SetLedState(0)

#define US_RESET()	LL_TIM_SetCounter(TIM14, 0)
#define US_GET()	LL_TIM_GetCounter(TIM14)
#define US_WAIT(us) DelayUs((us))
#define MS_GET()    HAL_GetTick()
#define MS_WAIT(ms) HAL_Delay((ms))

#else

#define EYE_IS_HIGH()	0
#define CARRIER_ON()
#define CARRIER_OFF()
#define LED_BLINK()
#define LED_NOBLINK()

#define US_RESET()
#define US_GET()	0
#define US_WAIT(us)
#define MS_GET()    0
#define MS_WAIT(ms)

#endif // PIO_UNIT_TESTING

#endif // _IR_PORT_H