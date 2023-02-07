/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    rtc.h
  * @brief   This file contains all the function prototypes for
  *          the rtc.c file
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
#ifndef __RTC_H__
#define __RTC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_RTC_Init(void);

/* USER CODE BEGIN Prototypes */
void RTC_SetDateTime(LL_RTC_DateTypeDef *date,LL_RTC_TimeTypeDef *time);
void RTC_GetDateTime(LL_RTC_DateTypeDef *date,LL_RTC_TimeTypeDef *time);
void RTC_ALARM_Disable(void);
void RTC_ALARM_Enable(void);
bool RTC_ALARM_Enabled(void);
void RTC_ALARM_GetTime(LL_RTC_TimeTypeDef *time);
void RTC_ALARM_SetTime(LL_RTC_TimeTypeDef *time);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __RTC_H__ */

