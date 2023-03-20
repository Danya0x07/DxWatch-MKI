/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

#include "stm32f0xx_ll_crs.h"
#include "stm32f0xx_ll_rcc.h"
#include "stm32f0xx_ll_bus.h"
#include "stm32f0xx_ll_system.h"
#include "stm32f0xx_ll_exti.h"
#include "stm32f0xx_ll_cortex.h"
#include "stm32f0xx_ll_utils.h"
#include "stm32f0xx_ll_pwr.h"
#include "stm32f0xx_ll_dma.h"
#include "stm32f0xx_ll_rtc.h"
#include "stm32f0xx_ll_spi.h"
#include "stm32f0xx_ll_tim.h"
#include "stm32f0xx_ll_usart.h"
#include "stm32f0xx_ll_gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BUILTIN_LED_Pin LL_GPIO_PIN_0
#define BUILTIN_LED_GPIO_Port GPIOA
#define M95_W_Pin LL_GPIO_PIN_1
#define M95_W_GPIO_Port GPIOA
#define M95_S_Pin LL_GPIO_PIN_2
#define M95_S_GPIO_Port GPIOA
#define M95_HOLD_Pin LL_GPIO_PIN_3
#define M95_HOLD_GPIO_Port GPIOA
#define SH1106_CS_Pin LL_GPIO_PIN_4
#define SH1106_CS_GPIO_Port GPIOA
#define SH1106_DC_Pin LL_GPIO_PIN_0
#define SH1106_DC_GPIO_Port GPIOB
#define BATTERY_Pin LL_GPIO_PIN_1
#define BATTERY_GPIO_Port GPIOB
#define MOTOR_Pin LL_GPIO_PIN_8
#define MOTOR_GPIO_Port GPIOA
#define BTN_1_Pin LL_GPIO_PIN_15
#define BTN_1_GPIO_Port GPIOA
#define BTN_0_Pin LL_GPIO_PIN_3
#define BTN_0_GPIO_Port GPIOB
#define BTN_0_EXTI_IRQn EXTI2_3_IRQn
#define IR_EYE_Pin LL_GPIO_PIN_4
#define IR_EYE_GPIO_Port GPIOB
#define PWR_LATCH_Pin LL_GPIO_PIN_5
#define PWR_LATCH_GPIO_Port GPIOB
#define FLASH_LED_Pin LL_GPIO_PIN_6
#define FLASH_LED_GPIO_Port GPIOB
#define BTN_2_Pin LL_GPIO_PIN_7
#define BTN_2_GPIO_Port GPIOB
#define BTN_3_Pin LL_GPIO_PIN_8
#define BTN_3_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
