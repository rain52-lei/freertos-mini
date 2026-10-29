/* 文件: stm32f4xx_hal_rtc_ex.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_rtc_ex.c
  * @author  MCD Application Team
  * @brief   RTC HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Real Time Clock (RTC) Extension peripheral:
  *           + RTC Time Stamp functions
  *           + RTC Tamper functions
  *           + RTC Wake-up functions
  *           + Extension Control functions
  *           + Extension RTC features functions
  *
  @verbatim
  ==============================================================================
                  ##### How to use this driver #####
  ==============================================================================
  [..]
    (+) Enable the RTC domain access.
    (+) Configure the RTC Prescaler (Asynchronous and Synchronous) and RTC hour
        format using the HAL_RTC_Init() function.

  *** RTC Wake-up configuration ***
  ================================
  [..]
    (+) To configure the RTC Wake-up Clock source and Counter use the HAL_RTCEx_SetWakeUpTimer()
        function. You can also configure the RTC Wake-up timer in interrupt mode
        using the HAL_RTCEx_SetWakeUpTimer_IT() function.
    (+) To read the RTC Wake-up Counter register, use the HAL_RTCEx_GetWakeUpTimer()
        function.

  *** TimeStamp configuration ***
  ===============================
  [..]
    (+) Configure the RTC_AFx trigger and enable the RTC TimeStamp using the
        HAL_RTCEx_SetTimeStamp() function. You can also configure the RTC TimeStamp with
        interrupt mode using the HAL_RTCEx_SetTimeStamp_IT() function.
    (+) To read the RTC TimeStamp Time and Date register, use the HAL_RTCEx_GetTimeStamp()
        function.
    (+) The TIMESTAMP alternate function can be mapped either to RTC_AF1 (PC13)
        or RTC_AF2 (PI8 or PA0 only for STM32F446xx devices) depending on the value of TSINSEL bit in
        RTC_TAFCR register. The corresponding pin is also selected by HAL_RTCEx_SetTimeStamp()
        or HAL_RTCEx_SetTimeStamp_IT() function.

  *** Tamper configuration ***
  ============================
  [..]
    (+) Enable the RTC Tamper and configure the Tamper filter count, trigger Edge
        or Level according to the Tamper filter (if equal to 0 Edge else Level)
        value, sampling frequency, precharge or discharge and Pull-UP using the
        HAL_RTCEx_SetTamper() function. You can configure RTC Tamper in interrupt
        mode using HAL_RTCEx_SetTamper_IT() function.
    (+) The TAMPER1 alternate function can be mapped either to RTC_AF1 (PC13)
        or RTC_AF2 (PI8 or PA0 only for STM32F446xx devices) depending on the value of TAMP1INSEL bit in
        RTC_TAFCR register. The corresponding pin is also selected by HAL_RTCEx_SetTamper()
        or HAL_RTCEx_SetTamper_IT() function.

  *** Backup Data Registers configuration ***
  ===========================================
  [..]
    (+) To write to the RTC Backup Data registers, use the HAL_RTCEx_BKUPWrite()
        function.
    (+) To read the RTC Backup Data registers, use the HAL_RTCEx_BKUPRead()
        function.

   @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"                              /* 包含 STM32/HAL 头文件 */

/** @addtogroup STM32F4xx_HAL_Driver
  * @{
  */

/** @defgroup RTCEx RTCEx
  * @brief RTC HAL module driver
  * @{
  */

#ifdef HAL_RTC_MODULE_ENABLED                           /* 条件编译 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup RTCEx_Exported_Functions RTCEx Exported Functions
  * @{
  */

/** @defgroup RTCEx_Exported_Functions_Group1 RTC TimeStamp and Tamper functions
 *  @brief   RTC TimeStamp and Tamper functions
 *
@verbatim
 ===============================================================================
                 ##### RTC TimeStamp and Tamper functions #####
 ===============================================================================

 [..] This section provides functions allowing to configure TimeStamp feature

@endverbatim
  * @{
  */

/**
  * @brief  Sets TimeStamp.
  * @note   This API must be called before enabling the TimeStamp feature.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @param  TimeStampEdge Specifies the pin edge on which the TimeStamp is
  *         activated.
  *          This parameter can be one of the following values:
  *             @arg RTC_TIMESTAMPEDGE_RISING: the Time stamp event occurs on the
  *                                        rising edge of the related pin.
  *             @arg RTC_TIMESTAMPEDGE_FALLING: the Time stamp event occurs on the
  *                                         falling edge of the related pin.
  * @param  RTC_TimeStampPin specifies the RTC TimeStamp Pin.
  *          This parameter can be one of the following values:
  *             @arg RTC_TIMESTAMPPIN_DEFAULT: PC13 is selected as RTC TimeStamp Pin.
  *             @arg RTC_TIMESTAMPPIN_POS1: PI8/PA0 is selected as RTC TimeStamp Pin.
  *             (not applicable in the case of STM32F412xx, STM32F413xx and STM32F423xx devices)
  *             (PI8 for all STM32 devices except for STM32F446xx devices the PA0 is used)
  *             @arg RTC_TIMESTAMPPIN_PA0: PA0 is selected as RTC TimeStamp Pin only for STM32F446xx devices
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTCEx_SetTimeStamp(RTC_HandleTypeDef *hrtc, uint32_t TimeStampEdge, uint32_t RTC_TimeStampPin)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpreg = 0U;                                 /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_TIMESTAMP_EDGE(TimeStampEdge));       /* 执行语句 */
  assert_param(IS_RTC_TIMESTAMP_PIN(RTC_TimeStampPin));  /* 执行语句 */

  /* Process Locked */
  __HAL_LOCK(hrtc);                                     /* HAL辅助宏/函数 */

  hrtc->State = HAL_RTC_STATE_BUSY;                     /* 赋值或初始化 */

  /* Get the RTC_CR register and clear the bits to be configured */
  tmpreg = (uint32_t)(hrtc->Instance->CR & (uint32_t)~(RTC_CR_TSEDGE | RTC_CR_TSE));  /* 赋值或初始化 */

  tmpreg|= TimeStampEdge;                               /* 赋值或初始化 */

  /* Disable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_DISABLE(hrtc);              /* HAL辅助宏/函数 */

  hrtc->Instance->TAFCR &= (uint32_t)~RTC_TAFCR_TSINSEL;  /* 赋值或初始化 */
  hrtc->Instance->TAFCR |= (uint32_t)(RTC_TimeStampPin);  /* 赋值或初始化 */

  /* Configure the Time Stamp TSEDGE and Enable bits */
  hrtc->Instance->CR = (uint32_t)tmpreg;                /* 赋值或初始化 */

  __HAL_RTC_TIMESTAMP_ENABLE(hrtc);                     /* HAL辅助宏/函数 */

  /* Enable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);               /* HAL辅助宏/函数 */

  /* Change RTC state */
  hrtc->State = HAL_RTC_STATE_READY;                    /* 赋值或初始化 */

  /* Process Unlocked */
  __HAL_UNLOCK(hrtc);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Sets TimeStamp with Interrupt.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @note   This API must be called before enabling the TimeStamp feature.
  * @param  TimeStampEdge Specifies the pin edge on which the TimeStamp is
  *         activated.
  *          This parameter can be one of the following values:
  *             @arg RTC_TIMESTAMPEDGE_RISING: the Time stamp event occurs on the
  *                                        rising edge of the related pin.
  *             @arg RTC_TIMESTAMPEDGE_FALLING: the Time stamp event occurs on the
  *                                         falling edge of the related pin.
  * @param  RTC_TimeStampPin Specifies the RTC TimeStamp Pin.
  *          This parameter can be one of the following values:
  *             @arg RTC_TIMESTAMPPIN_DEFAULT: PC13 is selected as RTC TimeStamp Pin.
  *             @arg RTC_TIMESTAMPPIN_PI8: PI8 is selected as RTC TimeStamp Pin. (not applicable in the case of STM32F446xx, STM32F412xx, STM32F413xx and STM32F423xx devices)
  *             @arg RTC_TIMESTAMPPIN_PA0: PA0 is selected as RTC TimeStamp Pin only for STM32F446xx devices
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTCEx_SetTimeStamp_IT(RTC_HandleTypeDef *hrtc, uint32_t TimeStampEdge, uint32_t RTC_TimeStampPin)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpreg = 0U;                                 /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_TIMESTAMP_EDGE(TimeStampEdge));       /* 执行语句 */
  assert_param(IS_RTC_TIMESTAMP_PIN(RTC_TimeStampPin));  /* 执行语句 */

  /* Process Locked */
  __HAL_LOCK(hrtc);                                     /* HAL辅助宏/函数 */

  hrtc->State = HAL_RTC_STATE_BUSY;                     /* 赋值或初始化 */

  /* Get the RTC_CR register and clear the bits to be configured */
  tmpreg = (uint32_t)(hrtc->Instance->CR & (uint32_t)~(RTC_CR_TSEDGE | RTC_CR_TSE));  /* 赋值或初始化 */

  tmpreg |= TimeStampEdge;                              /* 赋值或初始化 */

  /* Disable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_DISABLE(hrtc);              /* HAL辅助宏/函数 */

  /* Configure the Time Stamp TSEDGE and Enable bits */
  hrtc->Instance->CR = (uint32_t)tmpreg;                /* 赋值或初始化 */

  hrtc->Instance->TAFCR &= (uint32_t)~RTC_TAFCR_TSINSEL;  /* 赋值或初始化 */
  hrtc->Instance->TAFCR |= (uint32_t)(RTC_TimeStampPin);  /* 赋值或初始化 */

  /* Clear RTC Timestamp flag */
  __HAL_RTC_TIMESTAMP_CLEAR_FLAG(hrtc, RTC_FLAG_TSF);   /* HAL辅助宏/函数 */

  __HAL_RTC_TIMESTAMP_ENABLE(hrtc);                     /* HAL辅助宏/函数 */

  /* Enable IT timestamp */
  __HAL_RTC_TIMESTAMP_ENABLE_IT(hrtc,RTC_IT_TS);        /* HAL辅助宏/函数 */

  /* RTC timestamp Interrupt Configuration: EXTI configuration */
  __HAL_RTC_TAMPER_TIMESTAMP_EXTI_ENABLE_IT();          /* HAL辅助宏/函数 */

  EXTI->RTSR |= RTC_EXTI_LINE_TAMPER_TIMESTAMP_EVENT;   /* 赋值或初始化 */

  /* Enable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);               /* HAL辅助宏/函数 */

  hrtc->State = HAL_RTC_STATE_READY;                    /* 赋值或初始化 */

  /* Process Unlocked */
  __HAL_UNLOCK(hrtc);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Deactivates TimeStamp.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTCEx_DeactivateTimeStamp(RTC_HandleTypeDef *hrtc)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpreg = 0U;                                 /* 32位无符号变量/函数 */

  /* Process Locked */
  __HAL_LOCK(hrtc);                                     /* HAL辅助宏/函数 */

  hrtc->State = HAL_RTC_STATE_BUSY;                     /* 赋值或初始化 */

  /* Disable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_DISABLE(hrtc);              /* HAL辅助宏/函数 */

  /* In case of interrupt mode is used, the interrupt source must disabled */
  __HAL_RTC_TIMESTAMP_DISABLE_IT(hrtc, RTC_IT_TS);      /* HAL辅助宏/函数 */

  /* Get the RTC_CR register and clear the bits to be configured */
  tmpreg = (uint32_t)(hrtc->Instance->CR & (uint32_t)~(RTC_CR_TSEDGE | RTC_CR_TSE));  /* 赋值或初始化 */

  /* Configure the Time Stamp TSEDGE and Enable bits */
  hrtc->Instance->CR = (uint32_t)tmpreg;                /* 赋值或初始化 */

  /* Enable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);               /* HAL辅助宏/函数 */

  hrtc->State = HAL_RTC_STATE_READY;                    /* 赋值或初始化 */

  /* Process Unlocked */
  __HAL_UNLOCK(hrtc);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Gets the RTC TimeStamp value.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @param  sTimeStamp Pointer to Time structure
  * @param  sTimeStampDate Pointer to Date structure
  * @param  Format specifies the format of the entered parameters.
  *          This parameter can be one of the following values:
  *             RTC_FORMAT_BIN: Binary data format
  *             RTC_FORMAT_BCD: BCD data format
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTCEx_GetTimeStamp(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef* sTimeStamp, RTC_DateTypeDef* sTimeStampDate, uint32_t Format)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmptime = 0U, tmpdate = 0U;                  /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_RTC_FORMAT(Format));                  /* 执行语句 */

  /* Get the TimeStamp time and date registers values */
  tmptime = (uint32_t)(hrtc->Instance->TSTR & RTC_TR_RESERVED_MASK);  /* 赋值或初始化 */
  tmpdate = (uint32_t)(hrtc->Instance->TSDR & RTC_DR_RESERVED_MASK);  /* 赋值或初始化 */

  /* Fill the Time structure fields with the read parameters */
  sTimeStamp->Hours = (uint8_t)((tmptime & (RTC_TR_HT | RTC_TR_HU)) >> 16U);  /* 赋值或初始化 */
  sTimeStamp->Minutes = (uint8_t)((tmptime & (RTC_TR_MNT | RTC_TR_MNU)) >> 8U);  /* 赋值或初始化 */
  sTimeStamp->Seconds = (uint8_t)(tmptime & (RTC_TR_ST | RTC_TR_SU));  /* 赋值或初始化 */
  sTimeStamp->TimeFormat = (uint8_t)((tmptime & (RTC_TR_PM)) >> 16U);  /* 赋值或初始化 */
  sTimeStamp->SubSeconds = (uint32_t) hrtc->Instance->TSSSR;  /* 赋值或初始化 */

  /* Fill the Date structure fields with the read parameters */
  sTimeStampDate->Year = 0U;                            /* 赋值或初始化 */
  sTimeStampDate->Month = (uint8_t)((tmpdate & (RTC_DR_MT | RTC_DR_MU)) >> 8U);  /* 赋值或初始化 */
  sTimeStampDate->Date = (uint8_t)(tmpdate & (RTC_DR_DT | RTC_DR_DU));  /* 赋值或初始化 */
  sTimeStampDate->WeekDay = (uint8_t)((tmpdate & (RTC_DR_WDU)) >> 13U);  /* 赋值或初始化 */

  /* Check the input parameters format */
  if(Format == RTC_FORMAT_BIN)                          /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Convert the TimeStamp structure parameters to Binary format */
    sTimeStamp->Hours = (uint8_t)RTC_Bcd2ToByte(sTimeStamp->Hours);  /* 赋值或初始化 */
    sTimeStamp->Minutes = (uint8_t)RTC_Bcd2ToByte(sTimeStamp->Minutes);  /* 赋值或初始化 */
    sTimeStamp->Seconds = (uint8_t)RTC_Bcd2ToByte(sTimeStamp->Seconds);  /* 赋值或初始化 */

    /* Convert the DateTimeStamp structure parameters to Binary format */
    sTimeStampDate->Month = (uint8_t)RTC_Bcd2ToByte(sTimeStampDate->Month);  /* 赋值或初始化 */
    sTimeStampDate->Date = (uint8_t)RTC_Bcd2ToByte(sTimeStampDate->Date);  /* 赋值或初始化 */
    sTimeStampDate->WeekDay = (uint8_t)RTC_Bcd2ToByte(sTimeStampDate->WeekDay);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Clear the TIMESTAMP Flag */
  __HAL_RTC_TIMESTAMP_CLEAR_FLAG(hrtc, RTC_FLAG_TSF);   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Sets Tamper
  * @note   By calling this API we disable the tamper interrupt for all tampers.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @param  sTamper Pointer to Tamper Structure.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTCEx_SetTamper(RTC_HandleTypeDef *hrtc, RTC_TamperTypeDef* sTamper)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpreg = 0U;                                 /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_RTC_TAMPER(sTamper->Tamper));         /* 执行语句 */
  assert_param(IS_RTC_TAMPER_PIN(sTamper->PinSelection));  /* 执行语句 */
  assert_param(IS_RTC_TAMPER_TRIGGER(sTamper->Trigger));  /* 执行语句 */
  assert_param(IS_RTC_TAMPER_FILTER(sTamper->Filter));  /* 执行语句 */
  assert_param(IS_RTC_TAMPER_SAMPLING_FREQ(sTamper->SamplingFrequency));  /* 执行语句 */
  assert_param(IS_RTC_TAMPER_PRECHARGE_DURATION(sTamper->PrechargeDuration));  /* 执行语句 */
  assert_param(IS_RTC_TAMPER_PULLUP_STATE(sTamper->TamperPullUp));  /* 执行语句 */
  assert_param(IS_RTC_TAMPER_TIMESTAMPONTAMPER_DETECTION(sTamper->TimeStampOnTamperDetection));  /* 执行语句 */

  /* Process Locked */
  __HAL_LOCK(hrtc);                                     /* HAL辅助宏/函数 */

  hrtc->State = HAL_RTC_STATE_BUSY;                     /* 赋值或初始化 */

  if(sTamper->Trigger != RTC_TAMPERTRIGGER_RISINGEDGE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    sTamper->Trigger = (uint32_t)(sTamper->Tamper << 1U);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  tmpreg = ((uint32_t)sTamper->Tamper | (uint32_t)sTamper->PinSelection | (uint32_t)sTamper->Trigger  |\
            (uint32_t)sTamper->Filter | (uint32_t)sTamper->SamplingFrequency | (uint32_t)sTamper->PrechargeDuration |\
            (uint32_t)sTamper->TamperPullUp | sTamper->TimeStampOnTamperDetection);  /* 执行语句 */

  hrtc->Instance->TAFCR &= (uint32_t)~((uint32_t)sTamper->Tamper | (uint32_t)(sTamper->Tamper << 1U) | (uint32_t)RTC_TAFCR_TAMPTS |\
                                       (uint32_t)RTC_TAFCR_TAMPFREQ | (uint32_t)RTC_TAFCR_TAMPFLT | (uint32_t)RTC_TAFCR_TAMPPRCH |\
                                       (uint32_t)RTC_TAFCR_TAMPPUDIS | (uint32_t)RTC_TAFCR_TAMPINSEL | (uint32_t)RTC_TAFCR_TAMPIE);  /* 执行语句 */

  hrtc->Instance->TAFCR |= tmpreg;                      /* 赋值或初始化 */

  hrtc->State = HAL_RTC_STATE_READY;                    /* 赋值或初始化 */

  /* Process Unlocked */
  __HAL_UNLOCK(hrtc);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Sets Tamper with interrupt.
  * @note   By calling this API we force the tamper interrupt for all tampers.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @param  sTamper Pointer to RTC Tamper.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTCEx_SetTamper_IT(RTC_HandleTypeDef *hrtc, RTC_TamperTypeDef* sTamper)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpreg = 0U;                                 /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_RTC_TAMPER(sTamper->Tamper));         /* 执行语句 */
  assert_param(IS_RTC_TAMPER_PIN(sTamper->PinSelection));  /* 执行语句 */
  assert_param(IS_RTC_TAMPER_TRIGGER(sTamper->Trigger));  /* 执行语句 */
  assert_param(IS_RTC_TAMPER_FILTER(sTamper->Filter));  /* 执行语句 */
  assert_param(IS_RTC_TAMPER_SAMPLING_FREQ(sTamper->SamplingFrequency));  /* 执行语句 */
  assert_param(IS_RTC_TAMPER_PRECHARGE_DURATION(sTamper->PrechargeDuration));  /* 执行语句 */
  assert_param(IS_RTC_TAMPER_PULLUP_STATE(sTamper->TamperPullUp));  /* 执行语句 */
  assert_param(IS_RTC_TAMPER_TIMESTAMPONTAMPER_DETECTION(sTamper->TimeStampOnTamperDetection));  /* 执行语句 */

  /* Process Locked */
  __HAL_LOCK(hrtc);                                     /* HAL辅助宏/函数 */

  hrtc->State = HAL_RTC_STATE_BUSY;                     /* 赋值或初始化 */

  /* Configure the tamper trigger */
  if(sTamper->Trigger != RTC_TAMPERTRIGGER_RISINGEDGE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    sTamper->Trigger = (uint32_t)(sTamper->Tamper << 1U);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  tmpreg = ((uint32_t)sTamper->Tamper | (uint32_t)sTamper->PinSelection | (uint32_t)sTamper->Trigger  |\
            (uint32_t)sTamper->Filter | (uint32_t)sTamper->SamplingFrequency | (uint32_t)sTamper->PrechargeDuration |\
            (uint32_t)sTamper->TamperPullUp | sTamper->TimeStampOnTamperDetection);  /* 执行语句 */

  hrtc->Instance->TAFCR &= (uint32_t)~((uint32_t)sTamper->Tamper | (uint32_t)(sTamper->Tamper << 1U) | (uint32_t)RTC_TAFCR_TAMPTS |\
                                       (uint32_t)RTC_TAFCR_TAMPFREQ | (uint32_t)RTC_TAFCR_TAMPFLT | (uint32_t)RTC_TAFCR_TAMPPRCH |\
                                       (uint32_t)RTC_TAFCR_TAMPPUDIS | (uint32_t)RTC_TAFCR_TAMPINSEL);  /* 执行语句 */

  hrtc->Instance->TAFCR |= tmpreg;                      /* 赋值或初始化 */

  /* Configure the Tamper Interrupt in the RTC_TAFCR */
  hrtc->Instance->TAFCR |= (uint32_t)RTC_TAFCR_TAMPIE;  /* 赋值或初始化 */

  if(sTamper->Tamper == RTC_TAMPER_1)                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Clear RTC Tamper 1 flag */
    __HAL_RTC_TAMPER_CLEAR_FLAG(hrtc, RTC_FLAG_TAMP1F);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Clear RTC Tamper 2 flag */
    __HAL_RTC_TAMPER_CLEAR_FLAG(hrtc, RTC_FLAG_TAMP2F);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* RTC Tamper Interrupt Configuration: EXTI configuration */
  __HAL_RTC_TAMPER_TIMESTAMP_EXTI_ENABLE_IT();          /* HAL辅助宏/函数 */

  EXTI->RTSR |= RTC_EXTI_LINE_TAMPER_TIMESTAMP_EVENT;   /* 赋值或初始化 */

  hrtc->State = HAL_RTC_STATE_READY;                    /* 赋值或初始化 */

  /* Process Unlocked */
  __HAL_UNLOCK(hrtc);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Deactivates Tamper.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @param  Tamper Selected tamper pin.
  *          This parameter can be RTC_Tamper_1 and/or RTC_TAMPER_2.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTCEx_DeactivateTamper(RTC_HandleTypeDef *hrtc, uint32_t Tamper)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  assert_param(IS_RTC_TAMPER(Tamper));                  /* 执行语句 */

  /* Process Locked */
  __HAL_LOCK(hrtc);                                     /* HAL辅助宏/函数 */

  hrtc->State = HAL_RTC_STATE_BUSY;                     /* 赋值或初始化 */

  /* Disable the selected Tamper pin */
  hrtc->Instance->TAFCR &= (uint32_t)~Tamper;           /* 赋值或初始化 */

  hrtc->State = HAL_RTC_STATE_READY;                    /* 赋值或初始化 */

  /* Process Unlocked */
  __HAL_UNLOCK(hrtc);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  This function handles TimeStamp interrupt request.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @retval None
  */
void HAL_RTCEx_TamperTimeStampIRQHandler(RTC_HandleTypeDef *hrtc)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Get the TimeStamp interrupt source enable status */
  if(__HAL_RTC_TIMESTAMP_GET_IT_SOURCE(hrtc, RTC_IT_TS) != (uint32_t)RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Get the pending status of the TIMESTAMP Interrupt */
    if(__HAL_RTC_TIMESTAMP_GET_FLAG(hrtc, RTC_FLAG_TSF) != (uint32_t)RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* TIMESTAMP callback */
#if (USE_HAL_RTC_REGISTER_CALLBACKS == 1)               /* 条件编译 */
      hrtc->TimeStampEventCallback(hrtc);               /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_RTCEx_TimeStampEventCallback(hrtc);           /* HAL库函数调用 */
#endif /* USE_HAL_RTC_REGISTER_CALLBACKS */

      /* Clear the TIMESTAMP interrupt pending bit */
      __HAL_RTC_TIMESTAMP_CLEAR_FLAG(hrtc,RTC_FLAG_TSF);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Get the Tamper1 interrupt source enable status */
  if(__HAL_RTC_TAMPER_GET_IT_SOURCE(hrtc, RTC_IT_TAMP) != (uint32_t)RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Get the pending status of the Tamper1 Interrupt */
    if(__HAL_RTC_TAMPER_GET_FLAG(hrtc, RTC_FLAG_TAMP1F) != (uint32_t)RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Tamper callback */
#if (USE_HAL_RTC_REGISTER_CALLBACKS == 1)               /* 条件编译 */
      hrtc->Tamper1EventCallback(hrtc);                 /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_RTCEx_Tamper1EventCallback(hrtc);             /* HAL库函数调用 */
#endif /* USE_HAL_RTC_REGISTER_CALLBACKS */

      /* Clear the Tamper interrupt pending bit */
      __HAL_RTC_TAMPER_CLEAR_FLAG(hrtc,RTC_FLAG_TAMP1F);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Get the Tamper2 interrupt source enable status */
  if(__HAL_RTC_TAMPER_GET_IT_SOURCE(hrtc, RTC_IT_TAMP) != (uint32_t)RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Get the pending status of the Tamper2 Interrupt */
    if(__HAL_RTC_TAMPER_GET_FLAG(hrtc, RTC_FLAG_TAMP2F) != (uint32_t)RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Tamper callback */
#if (USE_HAL_RTC_REGISTER_CALLBACKS == 1)               /* 条件编译 */
      hrtc->Tamper2EventCallback(hrtc);                 /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_RTCEx_Tamper2EventCallback(hrtc);             /* HAL库函数调用 */
#endif /* USE_HAL_RTC_REGISTER_CALLBACKS */

      /* Clear the Tamper interrupt pending bit */
      __HAL_RTC_TAMPER_CLEAR_FLAG(hrtc, RTC_FLAG_TAMP2F);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Clear the EXTI's Flag for RTC TimeStamp and Tamper */
  __HAL_RTC_TAMPER_TIMESTAMP_EXTI_CLEAR_FLAG();         /* HAL辅助宏/函数 */

  /* Change RTC state */
  hrtc->State = HAL_RTC_STATE_READY;                    /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  TimeStamp callback.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @retval None
  */
__weak void HAL_RTCEx_TimeStampEventCallback(RTC_HandleTypeDef *hrtc)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hrtc);                                         /* 执行语句 */
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_RTC_TimeStampEventCallback could be implemented in the user file
  */
}                                                       /* 代码块结束 */

/**
  * @brief  Tamper 1 callback.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @retval None
  */
__weak void HAL_RTCEx_Tamper1EventCallback(RTC_HandleTypeDef *hrtc)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hrtc);                                         /* 执行语句 */
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_RTC_Tamper1EventCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Tamper 2 callback.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @retval None
  */
__weak void HAL_RTCEx_Tamper2EventCallback(RTC_HandleTypeDef *hrtc)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hrtc);                                         /* 执行语句 */
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_RTC_Tamper2EventCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  This function handles TimeStamp polling request.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTCEx_PollForTimeStampEvent(RTC_HandleTypeDef *hrtc, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart = 0U;                              /* 32位无符号变量/函数 */

  /* Get tick */
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  while(__HAL_RTC_TIMESTAMP_GET_FLAG(hrtc, RTC_FLAG_TSF) == RESET)  /* while循环 */
  {                                                     /* 代码块开始 */
    if(__HAL_RTC_TIMESTAMP_GET_FLAG(hrtc, RTC_FLAG_TSOVF) != RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear the TIMESTAMP Overrun Flag */
      __HAL_RTC_TIMESTAMP_CLEAR_FLAG(hrtc, RTC_FLAG_TSOVF);  /* HAL辅助宏/函数 */

      /* Change TIMESTAMP state */
      hrtc->State = HAL_RTC_STATE_ERROR;                /* 赋值或初始化 */

      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    if(Timeout != HAL_MAX_DELAY)                        /* 条件判断 */
    {                                                   /* 代码块开始 */
      if((Timeout == 0U)||((HAL_GetTick() - tickstart ) > Timeout))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        hrtc->State = HAL_RTC_STATE_TIMEOUT;            /* 赋值或初始化 */
        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Change RTC state */
  hrtc->State = HAL_RTC_STATE_READY;                    /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  This function handles Tamper1 Polling.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTCEx_PollForTamper1Event(RTC_HandleTypeDef *hrtc, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart = 0U;                              /* 32位无符号变量/函数 */

  /* Get tick */
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  /* Get the status of the Interrupt */
  while(__HAL_RTC_TAMPER_GET_FLAG(hrtc, RTC_FLAG_TAMP1F)== RESET)  /* while循环 */
  {                                                     /* 代码块开始 */
    if(Timeout != HAL_MAX_DELAY)                        /* 条件判断 */
    {                                                   /* 代码块开始 */
      if((Timeout == 0U)||((HAL_GetTick() - tickstart ) > Timeout))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        hrtc->State = HAL_RTC_STATE_TIMEOUT;            /* 赋值或初始化 */
        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Clear the Tamper Flag */
  __HAL_RTC_TAMPER_CLEAR_FLAG(hrtc,RTC_FLAG_TAMP1F);    /* HAL辅助宏/函数 */

  /* Change RTC state */
  hrtc->State = HAL_RTC_STATE_READY;                    /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  This function handles Tamper2 Polling.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTCEx_PollForTamper2Event(RTC_HandleTypeDef *hrtc, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart = 0U;                              /* 32位无符号变量/函数 */

  /* Get tick */
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  /* Get the status of the Interrupt */
  while(__HAL_RTC_TAMPER_GET_FLAG(hrtc, RTC_FLAG_TAMP2F) == RESET)  /* while循环 */
  {                                                     /* 代码块开始 */
    if(Timeout != HAL_MAX_DELAY)                        /* 条件判断 */
    {                                                   /* 代码块开始 */
      if((Timeout == 0U)||((HAL_GetTick() - tickstart ) > Timeout))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        hrtc->State = HAL_RTC_STATE_TIMEOUT;            /* 赋值或初始化 */
        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Clear the Tamper Flag */
  __HAL_RTC_TAMPER_CLEAR_FLAG(hrtc,RTC_FLAG_TAMP2F);    /* HAL辅助宏/函数 */

  /* Change RTC state */
  hrtc->State = HAL_RTC_STATE_READY;                    /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup RTCEx_Exported_Functions_Group2 RTC Wake-up functions
 *  @brief   RTC Wake-up functions
 *
@verbatim
 ===============================================================================
                        ##### RTC Wake-up functions #####
 ===============================================================================

 [..] This section provides functions allowing to configure Wake-up feature

@endverbatim
  * @{
  */

/**
  * @brief  Sets wake up timer.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @param  WakeUpCounter Wake up counter
  * @param  WakeUpClock Wake up clock
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTCEx_SetWakeUpTimer(RTC_HandleTypeDef *hrtc, uint32_t WakeUpCounter, uint32_t WakeUpClock)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart = 0U;                              /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_RTC_WAKEUP_CLOCK(WakeUpClock));       /* 执行语句 */
  assert_param(IS_RTC_WAKEUP_COUNTER(WakeUpCounter));   /* 执行语句 */

  /* Process Locked */
  __HAL_LOCK(hrtc);                                     /* HAL辅助宏/函数 */

  hrtc->State = HAL_RTC_STATE_BUSY;                     /* 赋值或初始化 */

  /* Disable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_DISABLE(hrtc);              /* HAL辅助宏/函数 */

  /*Check RTC WUTWF flag is reset only when wake up timer enabled*/
  if((hrtc->Instance->CR & RTC_CR_WUTE) != RESET)       /* 条件判断 */
  {                                                     /* 代码块开始 */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */

    /* Wait till RTC WUTWF flag is reset and if Time out is reached exit */
    while(__HAL_RTC_WAKEUPTIMER_GET_FLAG(hrtc, RTC_FLAG_WUTWF) == SET)  /* while循环 */
    {                                                   /* 代码块开始 */
      if((HAL_GetTick() - tickstart ) > RTC_TIMEOUT_VALUE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Enable the write protection for RTC registers */
        __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);         /* HAL辅助宏/函数 */

        hrtc->State = HAL_RTC_STATE_TIMEOUT;            /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hrtc);                             /* HAL辅助宏/函数 */

        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  __HAL_RTC_WAKEUPTIMER_DISABLE(hrtc);                  /* HAL辅助宏/函数 */

  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  /* Wait till RTC WUTWF flag is set and if Time out is reached exit */
  while(__HAL_RTC_WAKEUPTIMER_GET_FLAG(hrtc, RTC_FLAG_WUTWF) == RESET)  /* while循环 */
  {                                                     /* 代码块开始 */
    if((HAL_GetTick() - tickstart ) > RTC_TIMEOUT_VALUE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable the write protection for RTC registers */
      __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);           /* HAL辅助宏/函数 */

      hrtc->State = HAL_RTC_STATE_TIMEOUT;              /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hrtc);                               /* HAL辅助宏/函数 */

      return HAL_TIMEOUT;                               /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Clear the Wake-up Timer clock source bits in CR register */
  hrtc->Instance->CR &= (uint32_t)~RTC_CR_WUCKSEL;      /* 赋值或初始化 */

  /* Configure the clock source */
  hrtc->Instance->CR |= (uint32_t)WakeUpClock;          /* 赋值或初始化 */

  /* Configure the Wake-up Timer counter */
  hrtc->Instance->WUTR = (uint32_t)WakeUpCounter;       /* 赋值或初始化 */

   /* Enable the Wake-up Timer */
  __HAL_RTC_WAKEUPTIMER_ENABLE(hrtc);                   /* HAL辅助宏/函数 */

  /* Enable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);               /* HAL辅助宏/函数 */

  hrtc->State = HAL_RTC_STATE_READY;                    /* 赋值或初始化 */

  /* Process Unlocked */
  __HAL_UNLOCK(hrtc);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Sets wake up timer with interrupt
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @param  WakeUpCounter Wake up counter
  * @param  WakeUpClock Wake up clock
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTCEx_SetWakeUpTimer_IT(RTC_HandleTypeDef *hrtc, uint32_t WakeUpCounter, uint32_t WakeUpClock)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  __IO uint32_t count;                                  /* 执行语句 */

  /* Check the parameters */
  assert_param(IS_RTC_WAKEUP_CLOCK(WakeUpClock));       /* 执行语句 */
  assert_param(IS_RTC_WAKEUP_COUNTER(WakeUpCounter));   /* 执行语句 */

  /* Process Locked */
  __HAL_LOCK(hrtc);                                     /* HAL辅助宏/函数 */

  hrtc->State = HAL_RTC_STATE_BUSY;                     /* 赋值或初始化 */

  /* Disable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_DISABLE(hrtc);              /* HAL辅助宏/函数 */

  /* Check RTC WUTWF flag is reset only when wake up timer enabled */
  if((hrtc->Instance->CR & RTC_CR_WUTE) != RESET)       /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Wait till RTC WUTWF flag is reset and if Time out is reached exit */
    count = RTC_TIMEOUT_VALUE  * (SystemCoreClock / 32U / 1000U);  /* 赋值或初始化 */
    do                                                  /* do-while循环 */
    {                                                   /* 代码块开始 */
      if(count-- == 0U)                                 /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Enable the write protection for RTC registers */
        __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);         /* HAL辅助宏/函数 */

        hrtc->State = HAL_RTC_STATE_TIMEOUT;            /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hrtc);                             /* HAL辅助宏/函数 */

        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    while(__HAL_RTC_WAKEUPTIMER_GET_FLAG(hrtc, RTC_FLAG_WUTWF) == SET);  /* while循环 */
  }                                                     /* 代码块结束 */

  __HAL_RTC_WAKEUPTIMER_DISABLE(hrtc);                  /* HAL辅助宏/函数 */

  /* Wait till RTC WUTWF flag is set and if Time out is reached exit */
  count = RTC_TIMEOUT_VALUE  * (SystemCoreClock / 32U / 1000U);  /* 赋值或初始化 */
  do                                                    /* do-while循环 */
  {                                                     /* 代码块开始 */
    if(count-- == 0U)                                   /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable the write protection for RTC registers */
      __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);           /* HAL辅助宏/函数 */

      hrtc->State = HAL_RTC_STATE_TIMEOUT;              /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hrtc);                               /* HAL辅助宏/函数 */

      return HAL_TIMEOUT;                               /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  while(__HAL_RTC_WAKEUPTIMER_GET_FLAG(hrtc, RTC_FLAG_WUTWF) == RESET);  /* while循环 */

  /* Configure the Wake-up Timer counter */
  hrtc->Instance->WUTR = (uint32_t)WakeUpCounter;       /* 赋值或初始化 */

  /* Clear the Wake-up Timer clock source bits in CR register */
  hrtc->Instance->CR &= (uint32_t)~RTC_CR_WUCKSEL;      /* 赋值或初始化 */

  /* Configure the clock source */
  hrtc->Instance->CR |= (uint32_t)WakeUpClock;          /* 赋值或初始化 */

  /* RTC WakeUpTimer Interrupt Configuration: EXTI configuration */
  __HAL_RTC_WAKEUPTIMER_EXTI_ENABLE_IT();               /* HAL辅助宏/函数 */

  EXTI->RTSR |= RTC_EXTI_LINE_WAKEUPTIMER_EVENT;        /* 赋值或初始化 */

  /* Clear RTC Wake Up timer Flag */
  __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(hrtc, RTC_FLAG_WUTF);  /* HAL辅助宏/函数 */

  /* Configure the Interrupt in the RTC_CR register */
  __HAL_RTC_WAKEUPTIMER_ENABLE_IT(hrtc,RTC_IT_WUT);     /* HAL辅助宏/函数 */

  /* Enable the Wake-up Timer */
  __HAL_RTC_WAKEUPTIMER_ENABLE(hrtc);                   /* HAL辅助宏/函数 */

  /* Enable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);               /* HAL辅助宏/函数 */

  hrtc->State = HAL_RTC_STATE_READY;                    /* 赋值或初始化 */

  /* Process Unlocked */
  __HAL_UNLOCK(hrtc);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Deactivates wake up timer counter.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @retval HAL status
  */
uint32_t HAL_RTCEx_DeactivateWakeUpTimer(RTC_HandleTypeDef *hrtc)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart = 0U;                              /* 32位无符号变量/函数 */

  /* Process Locked */
  __HAL_LOCK(hrtc);                                     /* HAL辅助宏/函数 */

  hrtc->State = HAL_RTC_STATE_BUSY;                     /* 赋值或初始化 */

  /* Disable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_DISABLE(hrtc);              /* HAL辅助宏/函数 */

  /* Disable the Wake-up Timer */
  __HAL_RTC_WAKEUPTIMER_DISABLE(hrtc);                  /* HAL辅助宏/函数 */

  /* In case of interrupt mode is used, the interrupt source must disabled */
  __HAL_RTC_WAKEUPTIMER_DISABLE_IT(hrtc,RTC_IT_WUT);    /* HAL辅助宏/函数 */

  /* Get tick */
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  /* Wait till RTC WUTWF flag is set and if Time out is reached exit */
  while(__HAL_RTC_WAKEUPTIMER_GET_FLAG(hrtc, RTC_FLAG_WUTWF) == RESET)  /* while循环 */
  {                                                     /* 代码块开始 */
    if((HAL_GetTick() - tickstart ) > RTC_TIMEOUT_VALUE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable the write protection for RTC registers */
      __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);           /* HAL辅助宏/函数 */

      hrtc->State = HAL_RTC_STATE_TIMEOUT;              /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hrtc);                               /* HAL辅助宏/函数 */

      return HAL_TIMEOUT;                               /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Enable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);               /* HAL辅助宏/函数 */

  hrtc->State = HAL_RTC_STATE_READY;                    /* 赋值或初始化 */

  /* Process Unlocked */
  __HAL_UNLOCK(hrtc);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Gets wake up timer counter.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @retval Counter value
  */
uint32_t HAL_RTCEx_GetWakeUpTimer(RTC_HandleTypeDef *hrtc)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  /* Get the counter value */
  return ((uint32_t)(hrtc->Instance->WUTR & RTC_WUTR_WUT));  /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  This function handles Wake Up Timer interrupt request.
  * @note   Unlike alarm interrupt line (shared by AlarmA and AlarmB) and tamper
  *         interrupt line (shared by timestamp and tampers) wakeup timer
  *         interrupt line is exclusive to the wakeup timer.
  *         There is no need in this case to check on the interrupt enable
  *         status via __HAL_RTC_WAKEUPTIMER_GET_IT_SOURCE().
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @retval None
  */
void HAL_RTCEx_WakeUpTimerIRQHandler(RTC_HandleTypeDef *hrtc)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Get the pending status of the WAKEUPTIMER Interrupt */
  if(__HAL_RTC_WAKEUPTIMER_GET_FLAG(hrtc, RTC_FLAG_WUTF) != (uint32_t)RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* WAKEUPTIMER callback */
#if (USE_HAL_RTC_REGISTER_CALLBACKS == 1)               /* 条件编译 */
    hrtc->WakeUpTimerEventCallback(hrtc);               /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_RTCEx_WakeUpTimerEventCallback(hrtc);           /* HAL库函数调用 */
#endif /* USE_HAL_RTC_REGISTER_CALLBACKS */

    /* Clear the WAKEUPTIMER interrupt pending bit */
    __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(hrtc, RTC_FLAG_WUTF);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Clear the EXTI's line Flag for RTC WakeUpTimer */
  __HAL_RTC_WAKEUPTIMER_EXTI_CLEAR_FLAG();              /* HAL辅助宏/函数 */

  /* Change RTC state */
  hrtc->State = HAL_RTC_STATE_READY;                    /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  Wake Up Timer callback.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @retval None
  */
__weak void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hrtc);                                         /* 执行语句 */
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_RTC_WakeUpTimerEventCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  This function handles Wake Up Timer Polling.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTCEx_PollForWakeUpTimerEvent(RTC_HandleTypeDef *hrtc, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart = 0U;                              /* 32位无符号变量/函数 */

  /* Get tick */
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  while(__HAL_RTC_WAKEUPTIMER_GET_FLAG(hrtc, RTC_FLAG_WUTF) == RESET)  /* while循环 */
  {                                                     /* 代码块开始 */
    if(Timeout != HAL_MAX_DELAY)                        /* 条件判断 */
    {                                                   /* 代码块开始 */
      if((Timeout == 0U)||((HAL_GetTick() - tickstart ) > Timeout))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        hrtc->State = HAL_RTC_STATE_TIMEOUT;            /* 赋值或初始化 */

        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Clear the WAKEUPTIMER Flag */
  __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(hrtc, RTC_FLAG_WUTF);  /* HAL辅助宏/函数 */

  /* Change RTC state */
  hrtc->State = HAL_RTC_STATE_READY;                    /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */


/** @defgroup RTCEx_Exported_Functions_Group3 Extension Peripheral Control functions
 *  @brief   Extension Peripheral Control functions
 *
@verbatim
 ===============================================================================
              ##### Extension Peripheral Control functions #####
 ===============================================================================
    [..]
    This subsection provides functions allowing to
      (+) Write a data in a specified RTC Backup data register
      (+) Read a data in a specified RTC Backup data register
      (+) Set the Coarse calibration parameters.
      (+) Deactivate the Coarse calibration parameters
      (+) Set the Smooth calibration parameters.
      (+) Configure the Synchronization Shift Control Settings.
      (+) Configure the Calibration Pinout (RTC_CALIB) Selection (1Hz or 512Hz).
      (+) Deactivate the Calibration Pinout (RTC_CALIB) Selection (1Hz or 512Hz).
      (+) Enable the RTC reference clock detection.
      (+) Disable the RTC reference clock detection.
      (+) Enable the Bypass Shadow feature.
      (+) Disable the Bypass Shadow feature.

@endverbatim
  * @{
  */

/**
  * @brief  Writes a data in a specified RTC Backup data register.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @param  BackupRegister RTC Backup data Register number.
  *          This parameter can be: RTC_BKP_DRx where x can be from 0 to 19 to
  *                                 specify the register.
  * @param  Data Data to be written in the specified RTC Backup data register.
  * @retval None
  */
void HAL_RTCEx_BKUPWrite(RTC_HandleTypeDef *hrtc, uint32_t BackupRegister, uint32_t Data)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  uint32_t tmp = 0U;                                    /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_RTC_BKP(BackupRegister));             /* 执行语句 */

  tmp = (uint32_t)&(hrtc->Instance->BKP0R);             /* 赋值或初始化 */
  tmp += (BackupRegister * 4U);                         /* 赋值或初始化 */

  /* Write the specified register */
  *(__IO uint32_t *)tmp = (uint32_t)Data;               /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  Reads data from the specified RTC Backup data Register.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @param  BackupRegister RTC Backup data Register number.
  *          This parameter can be: RTC_BKP_DRx where x can be from 0 to 19 to
  *                                 specify the register.
  * @retval Read value
  */
uint32_t HAL_RTCEx_BKUPRead(RTC_HandleTypeDef *hrtc, uint32_t BackupRegister)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t tmp = 0U;                                    /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_RTC_BKP(BackupRegister));             /* 执行语句 */

  tmp = (uint32_t)&(hrtc->Instance->BKP0R);             /* 赋值或初始化 */
  tmp += (BackupRegister * 4U);                         /* 赋值或初始化 */

  /* Read the specified register */
  return (*(__IO uint32_t *)tmp);                       /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Sets the Coarse calibration parameters.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @param  CalibSign Specifies the sign of the coarse calibration value.
  *          This parameter can be  one of the following values :
  *             @arg RTC_CALIBSIGN_POSITIVE: The value sign is positive
  *             @arg RTC_CALIBSIGN_NEGATIVE: The value sign is negative
  * @param  Value value of coarse calibration expressed in ppm (coded on 5 bits).
  *
  * @note   This Calibration value should be between 0 and 63 when using negative
  *         sign with a 2-ppm step.
  *
  * @note   This Calibration value should be between 0 and 126 when using positive
  *         sign with a 4-ppm step.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTCEx_SetCoarseCalib(RTC_HandleTypeDef* hrtc, uint32_t CalibSign, uint32_t Value)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_RTC_CALIB_SIGN(CalibSign));           /* 执行语句 */
  assert_param(IS_RTC_CALIB_VALUE(Value));              /* 执行语句 */

  /* Process Locked */
  __HAL_LOCK(hrtc);                                     /* HAL辅助宏/函数 */

  hrtc->State = HAL_RTC_STATE_BUSY;                     /* 赋值或初始化 */

  /* Disable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_DISABLE(hrtc);              /* HAL辅助宏/函数 */

  /* Set Initialization mode */
  if(RTC_EnterInitMode(hrtc) != HAL_OK)                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable the write protection for RTC registers */
    __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);             /* HAL辅助宏/函数 */

    /* Set RTC state*/
    hrtc->State = HAL_RTC_STATE_ERROR;                  /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hrtc);                                 /* HAL辅助宏/函数 */

    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Enable the Coarse Calibration */
    __HAL_RTC_COARSE_CALIB_ENABLE(hrtc);                /* HAL辅助宏/函数 */

    /* Set the coarse calibration value */
    hrtc->Instance->CALIBR = (uint32_t)(CalibSign|Value);  /* 赋值或初始化 */

    /* Exit Initialization mode */
    hrtc->Instance->ISR &= (uint32_t)~RTC_ISR_INIT;     /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Enable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);               /* HAL辅助宏/函数 */

  /* Change state */
  hrtc->State = HAL_RTC_STATE_READY;                    /* 赋值或初始化 */

  /* Process Unlocked */
  __HAL_UNLOCK(hrtc);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Deactivates the Coarse calibration parameters.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTCEx_DeactivateCoarseCalib(RTC_HandleTypeDef* hrtc)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Process Locked */
  __HAL_LOCK(hrtc);                                     /* HAL辅助宏/函数 */

  hrtc->State = HAL_RTC_STATE_BUSY;                     /* 赋值或初始化 */

  /* Disable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_DISABLE(hrtc);              /* HAL辅助宏/函数 */

  /* Set Initialization mode */
  if(RTC_EnterInitMode(hrtc) != HAL_OK)                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable the write protection for RTC registers */
    __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);             /* HAL辅助宏/函数 */

    /* Set RTC state*/
    hrtc->State = HAL_RTC_STATE_ERROR;                  /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hrtc);                                 /* HAL辅助宏/函数 */

    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Enable the Coarse Calibration */
    __HAL_RTC_COARSE_CALIB_DISABLE(hrtc);               /* HAL辅助宏/函数 */

    /* Exit Initialization mode */
    hrtc->Instance->ISR &= (uint32_t)~RTC_ISR_INIT;     /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Enable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);               /* HAL辅助宏/函数 */

  /* Change state */
  hrtc->State = HAL_RTC_STATE_READY;                    /* 赋值或初始化 */

  /* Process Unlocked */
  __HAL_UNLOCK(hrtc);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Sets the Smooth calibration parameters.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @param  SmoothCalibPeriod Select the Smooth Calibration Period.
  *          This parameter can be can be one of the following values :
  *             @arg RTC_SMOOTHCALIB_PERIOD_32SEC: The smooth calibration period is 32s.
  *             @arg RTC_SMOOTHCALIB_PERIOD_16SEC: The smooth calibration period is 16s.
  *             @arg RTC_SMOOTHCALIB_PERIOD_8SEC: The smooth calibration period is 8s.
  * @param  SmoothCalibPlusPulses Select to Set or reset the CALP bit.
  *          This parameter can be one of the following values:
  *             @arg RTC_SMOOTHCALIB_PLUSPULSES_SET: Add one RTCCLK pulse every 2*11 pulses.
  *             @arg RTC_SMOOTHCALIB_PLUSPULSES_RESET: No RTCCLK pulses are added.
  * @param  SmouthCalibMinusPulsesValue Select the value of CALM[80] bits.
  *          This parameter can be one any value from 0 to 0x000001FF.
  * @note   To deactivate the smooth calibration, the field SmoothCalibPlusPulses
  *         must be equal to SMOOTHCALIB_PLUSPULSES_RESET and the field
  *         SmouthCalibMinusPulsesValue must be equal to 0.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTCEx_SetSmoothCalib(RTC_HandleTypeDef* hrtc, uint32_t SmoothCalibPeriod, uint32_t SmoothCalibPlusPulses, uint32_t SmouthCalibMinusPulsesValue)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart = 0U;                              /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_RTC_SMOOTH_CALIB_PERIOD(SmoothCalibPeriod));  /* 执行语句 */
  assert_param(IS_RTC_SMOOTH_CALIB_PLUS(SmoothCalibPlusPulses));  /* 执行语句 */
  assert_param(IS_RTC_SMOOTH_CALIB_MINUS(SmouthCalibMinusPulsesValue));  /* 执行语句 */

  /* Process Locked */
  __HAL_LOCK(hrtc);                                     /* HAL辅助宏/函数 */

  hrtc->State = HAL_RTC_STATE_BUSY;                     /* 赋值或初始化 */

  /* Disable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_DISABLE(hrtc);              /* HAL辅助宏/函数 */

  /* check if a calibration is pending*/
  if((hrtc->Instance->ISR & RTC_ISR_RECALPF) != RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
  /* Get tick */
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

    /* check if a calibration is pending*/
    while((hrtc->Instance->ISR & RTC_ISR_RECALPF) != RESET)  /* while循环 */
    {                                                   /* 代码块开始 */
      if((HAL_GetTick() - tickstart ) > RTC_TIMEOUT_VALUE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Enable the write protection for RTC registers */
        __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);         /* HAL辅助宏/函数 */

        /* Change RTC state */
        hrtc->State = HAL_RTC_STATE_TIMEOUT;            /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hrtc);                             /* HAL辅助宏/函数 */

        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Configure the Smooth calibration settings */
  hrtc->Instance->CALR = (uint32_t)((uint32_t)SmoothCalibPeriod | (uint32_t)SmoothCalibPlusPulses | (uint32_t)SmouthCalibMinusPulsesValue);  /* 赋值或初始化 */

  /* Enable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);               /* HAL辅助宏/函数 */

  /* Change RTC state */
  hrtc->State = HAL_RTC_STATE_READY;                    /* 赋值或初始化 */

  /* Process Unlocked */
  __HAL_UNLOCK(hrtc);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configures the Synchronization Shift Control Settings.
  * @note   When REFCKON is set, firmware must not write to Shift control register.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @param  ShiftAdd1S Select to add or not 1 second to the time calendar.
  *          This parameter can be one of the following values :
  *             @arg RTC_SHIFTADD1S_SET: Add one second to the clock calendar.
  *             @arg RTC_SHIFTADD1S_RESET: No effect.
  * @param  ShiftSubFS Select the number of Second Fractions to substitute.
  *          This parameter can be one any value from 0 to 0x7FFF.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTCEx_SetSynchroShift(RTC_HandleTypeDef* hrtc, uint32_t ShiftAdd1S, uint32_t ShiftSubFS)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart = 0U;                              /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_RTC_SHIFT_ADD1S(ShiftAdd1S));         /* 执行语句 */
  assert_param(IS_RTC_SHIFT_SUBFS(ShiftSubFS));         /* 执行语句 */

  /* Process Locked */
  __HAL_LOCK(hrtc);                                     /* HAL辅助宏/函数 */

  hrtc->State = HAL_RTC_STATE_BUSY;                     /* 赋值或初始化 */

  /* Disable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_DISABLE(hrtc);              /* HAL辅助宏/函数 */

  /* Get tick */
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

    /* Wait until the shift is completed*/
    while((hrtc->Instance->ISR & RTC_ISR_SHPF) != RESET)  /* while循环 */
    {                                                   /* 代码块开始 */
      if((HAL_GetTick() - tickstart ) > RTC_TIMEOUT_VALUE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Enable the write protection for RTC registers */
        __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);         /* HAL辅助宏/函数 */

        hrtc->State = HAL_RTC_STATE_TIMEOUT;            /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hrtc);                             /* HAL辅助宏/函数 */

        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Check if the reference clock detection is disabled */
    if((hrtc->Instance->CR & RTC_CR_REFCKON) == RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Configure the Shift settings */
      hrtc->Instance->SHIFTR = (uint32_t)(uint32_t)(ShiftSubFS) | (uint32_t)(ShiftAdd1S);  /* 赋值或初始化 */

      /* If  RTC_CR_BYPSHAD bit = 0, wait for synchro else this check is not needed */
      if((hrtc->Instance->CR & RTC_CR_BYPSHAD) == RESET)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        if(HAL_RTC_WaitForSynchro(hrtc) != HAL_OK)      /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Enable the write protection for RTC registers */
          __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);       /* HAL辅助宏/函数 */

          hrtc->State = HAL_RTC_STATE_ERROR;            /* 赋值或初始化 */

          /* Process Unlocked */
          __HAL_UNLOCK(hrtc);                           /* HAL辅助宏/函数 */

          return HAL_ERROR;                             /* 返回值 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Enable the write protection for RTC registers */
      __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);           /* HAL辅助宏/函数 */

      /* Change RTC state */
      hrtc->State = HAL_RTC_STATE_ERROR;                /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hrtc);                               /* HAL辅助宏/函数 */

      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

  /* Enable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);               /* HAL辅助宏/函数 */

  /* Change RTC state */
  hrtc->State = HAL_RTC_STATE_READY;                    /* 赋值或初始化 */

  /* Process Unlocked */
  __HAL_UNLOCK(hrtc);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configures the Calibration Pinout (RTC_CALIB) Selection (1Hz or 512Hz).
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @param  CalibOutput Select the Calibration output Selection .
  *          This parameter can be one of the following values:
  *             @arg RTC_CALIBOUTPUT_512HZ: A signal has a regular waveform at 512Hz.
  *             @arg RTC_CALIBOUTPUT_1HZ: A signal has a regular waveform at 1Hz.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTCEx_SetCalibrationOutPut(RTC_HandleTypeDef* hrtc, uint32_t CalibOutput)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_RTC_CALIB_OUTPUT(CalibOutput));       /* 执行语句 */

  /* Process Locked */
  __HAL_LOCK(hrtc);                                     /* HAL辅助宏/函数 */

  hrtc->State = HAL_RTC_STATE_BUSY;                     /* 赋值或初始化 */

  /* Disable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_DISABLE(hrtc);              /* HAL辅助宏/函数 */

  /* Clear flags before config */
  hrtc->Instance->CR &= (uint32_t)~RTC_CR_COSEL;        /* 赋值或初始化 */

  /* Configure the RTC_CR register */
  hrtc->Instance->CR |= (uint32_t)CalibOutput;          /* 赋值或初始化 */

  __HAL_RTC_CALIBRATION_OUTPUT_ENABLE(hrtc);            /* HAL辅助宏/函数 */

  /* Enable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);               /* HAL辅助宏/函数 */

  /* Change RTC state */
  hrtc->State = HAL_RTC_STATE_READY;                    /* 赋值或初始化 */

  /* Process Unlocked */
  __HAL_UNLOCK(hrtc);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Deactivates the Calibration Pinout (RTC_CALIB) Selection (1Hz or 512Hz).
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTCEx_DeactivateCalibrationOutPut(RTC_HandleTypeDef* hrtc)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Process Locked */
  __HAL_LOCK(hrtc);                                     /* HAL辅助宏/函数 */

  hrtc->State = HAL_RTC_STATE_BUSY;                     /* 赋值或初始化 */

  /* Disable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_DISABLE(hrtc);              /* HAL辅助宏/函数 */

  __HAL_RTC_CALIBRATION_OUTPUT_DISABLE(hrtc);           /* HAL辅助宏/函数 */

  /* Enable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);               /* HAL辅助宏/函数 */

  /* Change RTC state */
  hrtc->State = HAL_RTC_STATE_READY;                    /* 赋值或初始化 */

  /* Process Unlocked */
  __HAL_UNLOCK(hrtc);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Enables the RTC reference clock detection.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTCEx_SetRefClock(RTC_HandleTypeDef* hrtc)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Process Locked */
  __HAL_LOCK(hrtc);                                     /* HAL辅助宏/函数 */

  hrtc->State = HAL_RTC_STATE_BUSY;                     /* 赋值或初始化 */

  /* Disable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_DISABLE(hrtc);              /* HAL辅助宏/函数 */

  /* Set Initialization mode */
  if(RTC_EnterInitMode(hrtc) != HAL_OK)                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable the write protection for RTC registers */
    __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);             /* HAL辅助宏/函数 */

    /* Set RTC state*/
    hrtc->State = HAL_RTC_STATE_ERROR;                  /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hrtc);                                 /* HAL辅助宏/函数 */

    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    __HAL_RTC_CLOCKREF_DETECTION_ENABLE(hrtc);          /* HAL辅助宏/函数 */

    /* Exit Initialization mode */
    hrtc->Instance->ISR &= (uint32_t)~RTC_ISR_INIT;     /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Enable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);               /* HAL辅助宏/函数 */

   /* Change RTC state */
  hrtc->State = HAL_RTC_STATE_READY;                    /* 赋值或初始化 */

  /* Process Unlocked */
  __HAL_UNLOCK(hrtc);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Disable the RTC reference clock detection.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTCEx_DeactivateRefClock(RTC_HandleTypeDef* hrtc)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Process Locked */
  __HAL_LOCK(hrtc);                                     /* HAL辅助宏/函数 */

  hrtc->State = HAL_RTC_STATE_BUSY;                     /* 赋值或初始化 */

  /* Disable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_DISABLE(hrtc);              /* HAL辅助宏/函数 */

  /* Set Initialization mode */
  if(RTC_EnterInitMode(hrtc) != HAL_OK)                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable the write protection for RTC registers */
    __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);             /* HAL辅助宏/函数 */

    /* Set RTC state*/
    hrtc->State = HAL_RTC_STATE_ERROR;                  /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hrtc);                                 /* HAL辅助宏/函数 */

    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    __HAL_RTC_CLOCKREF_DETECTION_DISABLE(hrtc);         /* HAL辅助宏/函数 */

    /* Exit Initialization mode */
    hrtc->Instance->ISR &= (uint32_t)~RTC_ISR_INIT;     /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Enable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);               /* HAL辅助宏/函数 */

  /* Change RTC state */
  hrtc->State = HAL_RTC_STATE_READY;                    /* 赋值或初始化 */

  /* Process Unlocked */
  __HAL_UNLOCK(hrtc);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Enables the Bypass Shadow feature.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @note   When the Bypass Shadow is enabled the calendar value are taken
  *         directly from the Calendar counter.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTCEx_EnableBypassShadow(RTC_HandleTypeDef* hrtc)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Process Locked */
  __HAL_LOCK(hrtc);                                     /* HAL辅助宏/函数 */

  hrtc->State = HAL_RTC_STATE_BUSY;                     /* 赋值或初始化 */

  /* Disable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_DISABLE(hrtc);              /* HAL辅助宏/函数 */

  /* Set the BYPSHAD bit */
  hrtc->Instance->CR |= (uint8_t)RTC_CR_BYPSHAD;        /* 赋值或初始化 */

  /* Enable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);               /* HAL辅助宏/函数 */

  /* Change RTC state */
  hrtc->State = HAL_RTC_STATE_READY;                    /* 赋值或初始化 */

  /* Process Unlocked */
  __HAL_UNLOCK(hrtc);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Disables the Bypass Shadow feature.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @note   When the Bypass Shadow is enabled the calendar value are taken
  *         directly from the Calendar counter.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTCEx_DisableBypassShadow(RTC_HandleTypeDef* hrtc)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Process Locked */
  __HAL_LOCK(hrtc);                                     /* HAL辅助宏/函数 */

  hrtc->State = HAL_RTC_STATE_BUSY;                     /* 赋值或初始化 */

  /* Disable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_DISABLE(hrtc);              /* HAL辅助宏/函数 */

  /* Reset the BYPSHAD bit */
  hrtc->Instance->CR &= (uint8_t)~RTC_CR_BYPSHAD;       /* 赋值或初始化 */

  /* Enable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);               /* HAL辅助宏/函数 */

  /* Change RTC state */
  hrtc->State = HAL_RTC_STATE_READY;                    /* 赋值或初始化 */

  /* Process Unlocked */
  __HAL_UNLOCK(hrtc);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

  /** @defgroup RTCEx_Exported_Functions_Group4 Extended features functions
 *  @brief    Extended features functions
 *
@verbatim
 ===============================================================================
                 ##### Extended features functions #####
 ===============================================================================
    [..]  This section provides functions allowing to:
      (+) RTC Alarm B callback
      (+) RTC Poll for Alarm B request

@endverbatim
  * @{
  */

/**
  * @brief  Alarm B callback.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @retval None
  */
__weak void HAL_RTCEx_AlarmBEventCallback(RTC_HandleTypeDef *hrtc)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hrtc);                                         /* 执行语句 */
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_RTC_AlarmBEventCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  This function handles AlarmB Polling request.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTCEx_PollForAlarmBEvent(RTC_HandleTypeDef *hrtc, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart = 0U;                              /* 32位无符号变量/函数 */

  /* Get tick */
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  while(__HAL_RTC_ALARM_GET_FLAG(hrtc, RTC_FLAG_ALRBF) == RESET)  /* while循环 */
  {                                                     /* 代码块开始 */
    if(Timeout != HAL_MAX_DELAY)                        /* 条件判断 */
    {                                                   /* 代码块开始 */
      if((Timeout == 0U)||((HAL_GetTick() - tickstart ) > Timeout))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        hrtc->State = HAL_RTC_STATE_TIMEOUT;            /* 赋值或初始化 */
        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Clear the Alarm Flag */
  __HAL_RTC_ALARM_CLEAR_FLAG(hrtc, RTC_FLAG_ALRBF);     /* HAL辅助宏/函数 */

  /* Change RTC state */
  hrtc->State = HAL_RTC_STATE_READY;                    /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/**
  * @}
  */

#endif /* HAL_RTC_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
