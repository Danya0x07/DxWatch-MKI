/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    irtim.c
  * @brief   This file provides code for the configuration
  *          of the IRTIM instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "irtim.h"

/* USER CODE BEGIN 0 */
#define TIM_CARR    TIM17
#define TIM_ENV     TIM16
/* USER CODE END 0 */

/* IRTIM init function */
void MX_IRTIM_Init(void)
{

  /* USER CODE BEGIN IRTIM_Init 0 */

  /* USER CODE END IRTIM_Init 0 */

  /* USER CODE BEGIN IRTIM_Init 1 */

  /* USER CODE END IRTIM_Init 1 */
  /* USER CODE BEGIN IRTIM_Init 2 */
    // Otherwise IR led will be continiously on till first call of IRTIM_DisableCarrier().
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_13, LL_GPIO_MODE_OUTPUT);
  /* USER CODE END IRTIM_Init 2 */

}

/* USER CODE BEGIN 1 */

void IRTIM_SetCarrierFrequency(uint8_t kHz)
{
	uint16_t timerInputkHz = HAL_RCC_GetSysClockFreq() / (LL_TIM_GetPrescaler(TIM_CARR) + 1) / 1000;
    uint16_t arr = timerInputkHz / kHz - 1;
	LL_TIM_SetAutoReload(TIM_CARR, arr);
	LL_TIM_OC_SetCompareCH1(TIM_CARR, arr * 3 / 5);
}

void IRTIM_EnableCarrier(void)
{
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_13, LL_GPIO_MODE_ALTERNATE);
	TIM_ENV->CCMR1 &= ~TIM_CCMR1_OC1M_0;
	LL_TIM_EnableCounter(TIM_CARR);
	LL_TIM_EnableCounter(TIM_ENV);
}

void IRTIM_DisableCarrier(void)
{
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_13, LL_GPIO_MODE_OUTPUT);
	LL_TIM_DisableCounter(TIM_ENV);
	LL_TIM_DisableCounter(TIM_CARR);
    LL_TIM_SetCounter(TIM_CARR, 0);
    LL_TIM_SetCounter(TIM_ENV, 0);
}

void IRTIM_SetLedState(bool state)
{
	if (state)
		TIM_ENV->CCMR1 |= TIM_CCMR1_OC1M_0;
	else
		TIM_ENV->CCMR1 &= ~TIM_CCMR1_OC1M_0;
}
/* USER CODE END 1 */
