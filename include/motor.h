#ifndef _MOTOR_H
#define _MOTOR_H

#include "main.h"

#define MOTOR_ON()  LL_GPIO_SetOutputPin(MOTOR_GPIO_Port, MOTOR_Pin)
#define MOTOR_OFF() LL_GPIO_ResetOutputPin(MOTOR_GPIO_Port, MOTOR_Pin)
#define MOTOR_IS_ON()   LL_GPIO_IsOutputPinSet(MOTOR_GPIO_Port, MOTOR_Pin)

#endif // _MOTOR_H