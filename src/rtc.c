/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    rtc.c
  * @brief   This file provides code for the configuration
  *          of the RTC instances.
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
/* Includes ------------------------------------------------------------------*/
#include "rtc.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* RTC init function */
void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  LL_RTC_InitTypeDef RTC_InitStruct = {0};
  LL_RTC_TimeTypeDef RTC_TimeStruct = {0};
  LL_RTC_DateTypeDef RTC_DateStruct = {0};

  /* Peripheral clock enable */
  LL_RCC_EnableRTC();

  /* RTC interrupt Init */
  NVIC_SetPriority(RTC_IRQn, 0);
  NVIC_EnableIRQ(RTC_IRQn);

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC and set the Time and Date
  */
  RTC_InitStruct.HourFormat = LL_RTC_HOURFORMAT_24HOUR;
  RTC_InitStruct.AsynchPrescaler = 124;
  RTC_InitStruct.SynchPrescaler = 3999;
  LL_RTC_Init(RTC, &RTC_InitStruct);
  LL_RTC_SetAsynchPrescaler(RTC, 124);
  LL_RTC_SetSynchPrescaler(RTC, 3999);

  /** Initialize RTC and set the Time and Date
  */
  if(LL_RTC_BAK_GetRegister(RTC, LL_RTC_BKP_DR0) != 0x32F2){

  RTC_TimeStruct.Hours = 9;
  RTC_TimeStruct.Minutes = 30;
  RTC_TimeStruct.Seconds = 5;
  LL_RTC_TIME_Init(RTC, LL_RTC_FORMAT_BIN, &RTC_TimeStruct);
  RTC_DateStruct.WeekDay = LL_RTC_WEEKDAY_MONDAY;
  RTC_DateStruct.Month = LL_RTC_MONTH_JANUARY;
  RTC_DateStruct.Day = 1;
  RTC_DateStruct.Year = 23;
  LL_RTC_DATE_Init(RTC, LL_RTC_FORMAT_BIN, &RTC_DateStruct);
    LL_RTC_BAK_SetRegister(RTC,LL_RTC_BKP_DR0,0x32F2);
  }

  /** Initialize RTC and set the Time and Date
  */
  if(LL_RTC_BAK_GetRegister(RTC, LL_RTC_BKP_DR0) != 0x32F2){

    LL_RTC_BAK_SetRegister(RTC,LL_RTC_BKP_DR0,0x32F2);
  }
  /* USER CODE BEGIN RTC_Init 2 */
  LL_RTC_AlarmTypeDef RTC_AlarmStruct = {0};

  RTC_AlarmStruct.AlarmDateWeekDaySel = LL_RTC_ALMA_DATEWEEKDAYSEL_WEEKDAY;
  RTC_AlarmStruct.AlarmDateWeekDay = LL_RTC_WEEKDAY_MONDAY;
  RTC_AlarmStruct.AlarmMask = LL_RTC_ALMA_MASK_DATEWEEKDAY;
  RTC_AlarmStruct.AlarmTime.TimeFormat = LL_RTC_TIME_FORMAT_AM_OR_24;
  LL_RTC_ALMA_Init(RTC, LL_RTC_FORMAT_BIN, &RTC_AlarmStruct);
  
  LL_RTC_DisableWriteProtection(RTC);
  LL_RTC_ALMA_Disable(RTC);
  LL_RTC_EnableWriteProtection(RTC);
  
  LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_17);
  LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_17);
  /* USER CODE END RTC_Init 2 */

}

/* USER CODE BEGIN 1 */

void RTC_SetDateTime(LL_RTC_DateTypeDef *date,LL_RTC_TimeTypeDef *time)
{	
    LL_RTC_TIME_Init(RTC, LL_RTC_FORMAT_BIN, time);
    LL_RTC_DATE_Init(RTC, LL_RTC_FORMAT_BIN, date);	
    LL_RTC_BAK_SetRegister(RTC, LL_RTC_BKP_DR0, 0x32F2);
}

void RTC_GetDateTime(LL_RTC_DateTypeDef *date,LL_RTC_TimeTypeDef *time)
{
    time->Hours   = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetHour(RTC));
    time->Minutes = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetMinute(RTC));
    time->Seconds = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetSecond(RTC));
    
    date->Year  = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetYear(RTC));
    date->Month = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetMonth(RTC));
    date->Day   = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetDay(RTC));
    date->WeekDay = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetWeekDay(RTC));
}

void RTC_ALARM_Disable(void)
{
    LL_RTC_DisableWriteProtection(RTC);
    LL_RTC_ALMA_Disable(RTC);
    LL_RTC_DisableIT_ALRA(RTC);
    LL_RTC_EnableWriteProtection(RTC);
}

void RTC_ALARM_Enable(void)
{
    LL_RTC_DisableWriteProtection(RTC);
    LL_RTC_ALMA_Enable(RTC);
    LL_RTC_ClearFlag_ALRA(RTC);
    LL_RTC_EnableIT_ALRA(RTC);
    LL_RTC_EnableWriteProtection(RTC);
}

bool RTC_ALARM_Enabled(void)
{
    return (READ_BIT(RTC->CR, RTC_CR_ALRAE) == (RTC_CR_ALRAE));
}

void RTC_ALARM_GetTime(LL_RTC_TimeTypeDef *time)
{
    time->TimeFormat = LL_RTC_TIME_FORMAT_AM_OR_24;
    time->Hours = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_ALMA_GetHour(RTC));
    time->Minutes = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_ALMA_GetMinute(RTC));
    time->Seconds = 0;
}

void RTC_ALARM_SetTime(LL_RTC_TimeTypeDef *time)
{
    LL_RTC_DisableWriteProtection(RTC);
    LL_RTC_ALMA_Disable(RTC);
    LL_RTC_ALMA_ConfigTime(RTC, LL_RTC_TIME_FORMAT_AM_OR_24,
                           __LL_RTC_CONVERT_BIN2BCD(time->Hours),
                           __LL_RTC_CONVERT_BIN2BCD(time->Minutes),
                           __LL_RTC_CONVERT_BIN2BCD(0));
    LL_RTC_EnableWriteProtection(RTC);
}
/* USER CODE END 1 */
