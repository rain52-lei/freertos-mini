/* 文件: stm32f4xx_hal_rtc.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_rtc.c
  * @author  MCD Application Team
  * @brief   RTC HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Real Time Clock (RTC) peripheral:
  *           + Initialization and de-initialization functions
  *           + RTC Time and Date functions
  *           + RTC Alarm functions
  *           + Peripheral Control functions
  *           + Peripheral State functions
  *
  @verbatim
  ==============================================================================
              ##### Backup Domain Operating Condition #####
  ==============================================================================
  [..] The real-time clock (RTC), the RTC backup registers, and the backup
       SRAM (BKP SRAM) can be powered from the VBAT voltage when the main
       VDD supply is powered off.
       To retain the content of the RTC backup registers, backup SRAM, and supply
       the RTC when VDD is turned off, VBAT pin can be connected to an optional
       standby voltage supplied by a battery or by another source.

  [..] To allow the RTC operating even when the main digital supply (VDD) is turned
       off, the VBAT pin powers the following blocks:
    (#) The RTC
    (#) The LSE oscillator
    (#) The backup SRAM when the low power backup regulator is enabled
    (#) PC13 to PC15 I/Os, plus PI8 I/O (when available)

  [..] When the backup domain is supplied by VDD (analog switch connected to VDD),
       the following pins are available:
    (#) PC14 and PC15 can be used as either GPIO or LSE pins
    (#) PC13 can be used as a GPIO or as the RTC_AF1 pin
    (#) PI8 can be used as a GPIO or as the RTC_AF2 pin

  [..] When the backup domain is supplied by VBAT (analog switch connected to VBAT
       because VDD is not present), the following pins are available:
    (#) PC14 and PC15 can be used as LSE pins only
    (#) PC13 can be used as the RTC_AF1 pin
    (#) PI8 can be used as the RTC_AF2 pin

                   ##### Backup Domain Reset #####
  ==================================================================
  [..] The backup domain reset sets all RTC registers and the RCC_BDCR register
       to their reset values. The BKPSRAM is not affected by this reset. The only
       way to reset the BKPSRAM is through the Flash interface by requesting
       a protection level change from 1 to 0.
  [..] A backup domain reset is generated when one of the following events occurs:
    (#) Software reset, triggered by setting the BDRST bit in the
        RCC Backup domain control register (RCC_BDCR).
    (#) VDD or VBAT power on, if both supplies have previously been powered off.

                   ##### Backup Domain Access #####
  ==================================================================
  [..] After reset, the backup domain (RTC registers, RTC backup data
       registers and backup SRAM) is protected against possible unwanted write
       accesses.
  [..] To enable access to the RTC Domain and RTC registers, proceed as follows:
    (+) Enable the Power Controller (PWR) APB1 interface clock using the
        __HAL_RCC_PWR_CLK_ENABLE() function.
    (+) Enable access to RTC domain using the HAL_PWR_EnableBkUpAccess() function.
    (+) Select the RTC clock source using the __HAL_RCC_RTC_CONFIG() function.
    (+) Enable RTC Clock using the __HAL_RCC_RTC_ENABLE() function.


                  ##### How to use this driver #####
  ==================================================================
  [..]
    (+) Enable the RTC domain access (see description in the section above).
    (+) Configure the RTC Prescaler (Asynchronous and Synchronous) and RTC hour
        format using the HAL_RTC_Init() function.

  *** Time and Date configuration ***
  ===================================
  [..]
    (+) To configure the RTC Calendar (Time and Date) use the HAL_RTC_SetTime()
        and HAL_RTC_SetDate() functions.
    (+) To read the RTC Calendar, use the HAL_RTC_GetTime() and HAL_RTC_GetDate() functions.

  *** Alarm configuration ***
  ===========================
  [..]
    (+) To configure the RTC Alarm use the HAL_RTC_SetAlarm() function.
        You can also configure the RTC Alarm with interrupt mode using the HAL_RTC_SetAlarm_IT() function.
    (+) To read the RTC Alarm, use the HAL_RTC_GetAlarm() function.

                  ##### RTC and low power modes #####
  ==================================================================
  [..] The MCU can be woken up from a low power mode by an RTC alternate
       function.
  [..] The RTC alternate functions are the RTC alarms (Alarm A and Alarm B),
       RTC wake-up, RTC tamper event detection and RTC time stamp event detection.
       These RTC alternate functions can wake up the system from the Stop and
       Standby low power modes.
  [..] The system can also wake up from low power modes without depending
       on an external interrupt (Auto-wake-up mode), by using the RTC alarm
       or the RTC wake-up events.
  [..] The RTC provides a programmable time base for waking up from the
       Stop or Standby mode at regular intervals.
       Wake-up from STOP and STANDBY modes is possible only when the RTC clock source
       is LSE or LSI.

  *** Callback registration ***
  =============================================

  [..]
  The compilation define  USE_HAL_RTC_REGISTER_CALLBACKS when set to 1
  allows the user to configure dynamically the driver callbacks.
  Use Function @ref HAL_RTC_RegisterCallback() to register an interrupt callback.

  [..]
  Function @ref HAL_RTC_RegisterCallback() allows to register following callbacks:
    (+) AlarmAEventCallback          : RTC Alarm A Event callback.
    (+) AlarmBEventCallback          : RTC Alarm B Event callback.
    (+) TimeStampEventCallback       : RTC TimeStamp Event callback.
    (+) WakeUpTimerEventCallback     : RTC WakeUpTimer Event callback.
    (+) Tamper1EventCallback         : RTC Tamper 1 Event callback.
    (+) Tamper2EventCallback         : RTC Tamper 2 Event callback.
    (+) MspInitCallback              : RTC MspInit callback.
    (+) MspDeInitCallback            : RTC MspDeInit callback.
  [..]
  This function takes as parameters the HAL peripheral handle, the Callback ID
  and a pointer to the user callback function.

  [..]
  Use function @ref HAL_RTC_UnRegisterCallback() to reset a callback to the default
  weak function.
  @ref HAL_RTC_UnRegisterCallback() takes as parameters the HAL peripheral handle,
  and the Callback ID.
  This function allows to reset following callbacks:
    (+) AlarmAEventCallback          : RTC Alarm A Event callback.
    (+) AlarmBEventCallback          : RTC Alarm B Event callback.
    (+) TimeStampEventCallback       : RTC TimeStamp Event callback.
    (+) WakeUpTimerEventCallback     : RTC WakeUpTimer Event callback.
    (+) Tamper1EventCallback         : RTC Tamper 1 Event callback.
    (+) Tamper2EventCallback         : RTC Tamper 2 Event callback.
    (+) MspInitCallback              : RTC MspInit callback.
    (+) MspDeInitCallback            : RTC MspDeInit callback.

  [..]
  By default, after the @ref HAL_RTC_Init() and when the state is HAL_RTC_STATE_RESET,
  all callbacks are set to the corresponding weak functions :
  examples @ref AlarmAEventCallback(), @ref WakeUpTimerEventCallback().
  Exception done for MspInit and MspDeInit callbacks that are reset to the legacy weak function
  in the @ref HAL_RTC_Init()/@ref HAL_RTC_DeInit() only when these callbacks are null
  (not registered beforehand).
  If not, MspInit or MspDeInit are not null, @ref HAL_RTC_Init()/@ref HAL_RTC_DeInit()
  keep and use the user MspInit/MspDeInit callbacks (registered beforehand)

  [..]
  Callbacks can be registered/unregistered in HAL_RTC_STATE_READY state only.
  Exception done MspInit/MspDeInit that can be registered/unregistered
  in HAL_RTC_STATE_READY or HAL_RTC_STATE_RESET state,
  thus registered (user) MspInit/DeInit callbacks can be used during the Init/DeInit.
  In that case first register the MspInit/MspDeInit user callbacks
  using @ref HAL_RTC_RegisterCallback() before calling @ref HAL_RTC_DeInit()
  or @ref HAL_RTC_Init() function.

  [..]
  When The compilation define USE_HAL_RTC_REGISTER_CALLBACKS is set to 0 or
  not defined, the callback registration feature is not available and all callbacks
  are set to the corresponding weak functions.
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

/** @defgroup RTC RTC
  * @brief RTC HAL module driver
  * @{
  */

#ifdef HAL_RTC_MODULE_ENABLED                           /* 条件编译 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/** @defgroup RTC_Exported_Functions RTC Exported Functions
  * @{
  */

/** @defgroup RTC_Exported_Functions_Group1 Initialization and de-initialization functions
 *  @brief    Initialization and Configuration functions
 *
@verbatim
 ===============================================================================
              ##### Initialization and de-initialization functions #####
 ===============================================================================
   [..] This section provides functions allowing to initialize and configure the
         RTC Prescaler (Synchronous and Asynchronous), RTC Hour format, disable
         RTC registers Write protection, enter and exit the RTC initialization mode,
         RTC registers synchronization check and reference clock detection enable.
         (#) The RTC Prescaler is programmed to generate the RTC 1Hz time base.
             It is split into 2 programmable prescalers to minimize power consumption.
             (++) A 7-bit asynchronous prescaler and a 13-bit synchronous prescaler.
             (++) When both prescalers are used, it is recommended to configure the
                 asynchronous prescaler to a high value to minimize power consumption.
         (#) All RTC registers are Write protected. Writing to the RTC registers
             is enabled by writing a key into the Write Protection register, RTC_WPR.
         (#) To configure the RTC Calendar, user application should enter
             initialization mode. In this mode, the calendar counter is stopped
             and its value can be updated. When the initialization sequence is
             complete, the calendar restarts counting after 4 RTCCLK cycles.
         (#) To read the calendar through the shadow registers after Calendar
             initialization, calendar update or after wake-up from low power modes
             the software must first clear the RSF flag. The software must then
             wait until it is set again before reading the calendar, which means
             that the calendar registers have been correctly copied into the
             RTC_TR and RTC_DR shadow registers.The HAL_RTC_WaitForSynchro() function
             implements the above software sequence (RSF clear and RSF check).

@endverbatim
  * @{
  */

/**
  * @brief  Initializes the RTC peripheral
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTC_Init(RTC_HandleTypeDef *hrtc)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the RTC peripheral state */
  if(hrtc == NULL)                                      /* 条件判断 */
  {                                                     /* 代码块开始 */
     return HAL_ERROR;                                  /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the parameters */
  assert_param(IS_RTC_HOUR_FORMAT(hrtc->Init.HourFormat));  /* 执行语句 */
  assert_param(IS_RTC_ASYNCH_PREDIV(hrtc->Init.AsynchPrediv));  /* 执行语句 */
  assert_param(IS_RTC_SYNCH_PREDIV(hrtc->Init.SynchPrediv));  /* 执行语句 */
  assert_param (IS_RTC_OUTPUT(hrtc->Init.OutPut));      /* 执行语句 */
  assert_param (IS_RTC_OUTPUT_POL(hrtc->Init.OutPutPolarity));  /* 执行语句 */
  assert_param(IS_RTC_OUTPUT_TYPE(hrtc->Init.OutPutType));  /* 执行语句 */

#if (USE_HAL_RTC_REGISTER_CALLBACKS == 1)               /* 条件编译 */
  if(hrtc->State == HAL_RTC_STATE_RESET)                /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Allocate lock resource and initialize it */
    hrtc->Lock = HAL_UNLOCKED;                          /* 赋值或初始化 */

    hrtc->AlarmAEventCallback          =  HAL_RTC_AlarmAEventCallback;        /* Legacy weak AlarmAEventCallback      */
    hrtc->AlarmBEventCallback          =  HAL_RTCEx_AlarmBEventCallback;      /* Legacy weak AlarmBEventCallback      */
    hrtc->TimeStampEventCallback       =  HAL_RTCEx_TimeStampEventCallback;   /* Legacy weak TimeStampEventCallback   */
    hrtc->WakeUpTimerEventCallback     =  HAL_RTCEx_WakeUpTimerEventCallback; /* Legacy weak WakeUpTimerEventCallback */
    hrtc->Tamper1EventCallback         =  HAL_RTCEx_Tamper1EventCallback;     /* Legacy weak Tamper1EventCallback     */
    hrtc->Tamper2EventCallback         =  HAL_RTCEx_Tamper2EventCallback;     /* Legacy weak Tamper2EventCallback     */

    if(hrtc->MspInitCallback == NULL)                   /* 条件判断 */
    {                                                   /* 代码块开始 */
      hrtc->MspInitCallback = HAL_RTC_MspInit;          /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    /* Init the low level hardware */
    hrtc->MspInitCallback(hrtc);                        /* 执行语句 */

    if(hrtc->MspDeInitCallback == NULL)                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      hrtc->MspDeInitCallback = HAL_RTC_MspDeInit;      /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
#else                                                   /* 条件编译否则 */
  if(hrtc->State == HAL_RTC_STATE_RESET)                /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Allocate lock resource and initialize it */
    hrtc->Lock = HAL_UNLOCKED;                          /* 赋值或初始化 */

    /* Initialize RTC MSP */
    HAL_RTC_MspInit(hrtc);                              /* HAL库函数调用 */
  }                                                     /* 代码块结束 */
#endif /* (USE_HAL_RTC_REGISTER_CALLBACKS) */

  /* Set RTC state */
  hrtc->State = HAL_RTC_STATE_BUSY;                     /* 赋值或初始化 */

  /* Disable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_DISABLE(hrtc);              /* HAL辅助宏/函数 */

  /* Set Initialization mode */
  if(RTC_EnterInitMode(hrtc) != HAL_OK)                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable the write protection for RTC registers */
    __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);             /* HAL辅助宏/函数 */

    /* Set RTC state */
    hrtc->State = HAL_RTC_STATE_ERROR;                  /* 赋值或初始化 */

    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Clear RTC_CR FMT, OSEL and POL Bits */
    hrtc->Instance->CR &= ((uint32_t)~(RTC_CR_FMT | RTC_CR_OSEL | RTC_CR_POL));  /* 赋值或初始化 */
    /* Set RTC_CR register */
    hrtc->Instance->CR |= (uint32_t)(hrtc->Init.HourFormat | hrtc->Init.OutPut | hrtc->Init.OutPutPolarity);  /* 赋值或初始化 */

    /* Configure the RTC PRER */
    hrtc->Instance->PRER = (uint32_t)(hrtc->Init.SynchPrediv);  /* 赋值或初始化 */
    hrtc->Instance->PRER |= (uint32_t)(hrtc->Init.AsynchPrediv << 16U);  /* 赋值或初始化 */

    /* Exit Initialization mode */
    hrtc->Instance->ISR &= (uint32_t)~RTC_ISR_INIT;     /* 赋值或初始化 */

    /* If  CR_BYPSHAD bit = 0, wait for synchro else this check is not needed */
    if((hrtc->Instance->CR & RTC_CR_BYPSHAD) == RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      if(HAL_RTC_WaitForSynchro(hrtc) != HAL_OK)        /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Enable the write protection for RTC registers */
        __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);         /* HAL辅助宏/函数 */

        hrtc->State = HAL_RTC_STATE_ERROR;              /* 赋值或初始化 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    hrtc->Instance->TAFCR &= (uint32_t)~RTC_TAFCR_ALARMOUTTYPE;  /* 赋值或初始化 */
    hrtc->Instance->TAFCR |= (uint32_t)(hrtc->Init.OutPutType);  /* 赋值或初始化 */

    /* Enable the write protection for RTC registers */
    __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);             /* HAL辅助宏/函数 */

    /* Set RTC state */
    hrtc->State = HAL_RTC_STATE_READY;                  /* 赋值或初始化 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  DeInitializes the RTC peripheral
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @note   This function doesn't reset the RTC Backup Data registers.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTC_DeInit(RTC_HandleTypeDef *hrtc)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart = 0U;                              /* 32位无符号变量/函数 */

  /* Set RTC state */
  hrtc->State = HAL_RTC_STATE_BUSY;                     /* 赋值或初始化 */

  /* Disable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_DISABLE(hrtc);              /* HAL辅助宏/函数 */

  /* Set Initialization mode */
  if(RTC_EnterInitMode(hrtc) != HAL_OK)                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable the write protection for RTC registers */
    __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);             /* HAL辅助宏/函数 */

    /* Set RTC state */
    hrtc->State = HAL_RTC_STATE_ERROR;                  /* 赋值或初始化 */

    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Reset TR, DR and CR registers */
    hrtc->Instance->TR = 0x00000000U;                   /* 赋值或初始化 */
    hrtc->Instance->DR = 0x00002101U;                   /* 赋值或初始化 */
    /* Reset All CR bits except CR[2:0] */
    hrtc->Instance->CR &= 0x00000007U;                  /* 赋值或初始化 */

    /* Get tick */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */

    /* Wait till WUTWF flag is set and if Time out is reached exit */
    while(((hrtc->Instance->ISR) & RTC_ISR_WUTWF) == (uint32_t)RESET)  /* while循环 */
    {                                                   /* 代码块开始 */
      if((HAL_GetTick() - tickstart ) > RTC_TIMEOUT_VALUE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Enable the write protection for RTC registers */
        __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);         /* HAL辅助宏/函数 */

        /* Set RTC state */
        hrtc->State = HAL_RTC_STATE_TIMEOUT;            /* 赋值或初始化 */

        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Reset all RTC CR register bits */
    hrtc->Instance->CR &= 0x00000000U;                  /* 赋值或初始化 */
    hrtc->Instance->WUTR = 0x0000FFFFU;                 /* 赋值或初始化 */
    hrtc->Instance->PRER = 0x007F00FFU;                 /* 赋值或初始化 */
    hrtc->Instance->CALIBR = 0x00000000U;               /* 赋值或初始化 */
    hrtc->Instance->ALRMAR = 0x00000000U;               /* 赋值或初始化 */
    hrtc->Instance->ALRMBR = 0x00000000U;               /* 赋值或初始化 */
    hrtc->Instance->SHIFTR = 0x00000000U;               /* 赋值或初始化 */
    hrtc->Instance->CALR = 0x00000000U;                 /* 赋值或初始化 */
    hrtc->Instance->ALRMASSR = 0x00000000U;             /* 赋值或初始化 */
    hrtc->Instance->ALRMBSSR = 0x00000000U;             /* 赋值或初始化 */

    /* Reset ISR register and exit initialization mode */
    hrtc->Instance->ISR = 0x00000000U;                  /* 赋值或初始化 */

    /* Reset Tamper and alternate functions configuration register */
    hrtc->Instance->TAFCR = 0x00000000U;                /* 赋值或初始化 */

    /* If  RTC_CR_BYPSHAD bit = 0, wait for synchro else this check is not needed */
    if((hrtc->Instance->CR & RTC_CR_BYPSHAD) == RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      if(HAL_RTC_WaitForSynchro(hrtc) != HAL_OK)        /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Enable the write protection for RTC registers */
        __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);         /* HAL辅助宏/函数 */

        hrtc->State = HAL_RTC_STATE_ERROR;              /* 赋值或初始化 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Enable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);               /* HAL辅助宏/函数 */

#if (USE_HAL_RTC_REGISTER_CALLBACKS == 1)               /* 条件编译 */
  if(hrtc->MspDeInitCallback == NULL)                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    hrtc->MspDeInitCallback = HAL_RTC_MspDeInit;        /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* DeInit the low level hardware: CLOCK, NVIC.*/
  hrtc->MspDeInitCallback(hrtc);                        /* 执行语句 */

#else                                                   /* 条件编译否则 */
  /* De-Initialize RTC MSP */
  HAL_RTC_MspDeInit(hrtc);                              /* HAL库函数调用 */
#endif /* (USE_HAL_RTC_REGISTER_CALLBACKS) */

  hrtc->State = HAL_RTC_STATE_RESET;                    /* 赋值或初始化 */

  /* Release Lock */
  __HAL_UNLOCK(hrtc);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

#if (USE_HAL_RTC_REGISTER_CALLBACKS == 1)               /* 条件编译 */
/**
  * @brief  Register a User RTC Callback
  *         To be used instead of the weak predefined callback
  * @param  hrtc RTC handle
  * @param  CallbackID ID of the callback to be registered
  *         This parameter can be one of the following values:
  *          @arg @ref HAL_RTC_ALARM_A_EVENT_CB_ID          Alarm A Event Callback ID
  *          @arg @ref HAL_RTC_ALARM_B_EVENT_CB_ID          Alarm B Event Callback ID
  *          @arg @ref HAL_RTC_TIMESTAMP_EVENT_CB_ID        TimeStamp Event Callback ID
  *          @arg @ref HAL_RTC_WAKEUPTIMER_EVENT_CB_ID      Wake-Up Timer Event Callback ID
  *          @arg @ref HAL_RTC_TAMPER1_EVENT_CB_ID          Tamper 1 Callback ID
  *          @arg @ref HAL_RTC_TAMPER2_EVENT_CB_ID          Tamper 2 Callback ID
  *          @arg @ref HAL_RTC_MSPINIT_CB_ID                Msp Init callback ID
  *          @arg @ref HAL_RTC_MSPDEINIT_CB_ID              Msp DeInit callback ID
  * @param  pCallback pointer to the Callback function
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTC_RegisterCallback(RTC_HandleTypeDef *hrtc, HAL_RTC_CallbackIDTypeDef CallbackID, pRTC_CallbackTypeDef pCallback)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  if(pCallback == NULL)                                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Process locked */
  __HAL_LOCK(hrtc);                                     /* HAL辅助宏/函数 */

  if(HAL_RTC_STATE_READY == hrtc->State)                /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
    case HAL_RTC_ALARM_A_EVENT_CB_ID :                  /* case标签 */
      hrtc->AlarmAEventCallback = pCallback;            /* 赋值或初始化 */
      break;                                            /* 跳出 */

    case HAL_RTC_ALARM_B_EVENT_CB_ID :                  /* case标签 */
      hrtc->AlarmBEventCallback = pCallback;            /* 赋值或初始化 */
      break;                                            /* 跳出 */

    case HAL_RTC_TIMESTAMP_EVENT_CB_ID :                /* case标签 */
      hrtc->TimeStampEventCallback = pCallback;         /* 赋值或初始化 */
      break;                                            /* 跳出 */

    case HAL_RTC_WAKEUPTIMER_EVENT_CB_ID :              /* case标签 */
      hrtc->WakeUpTimerEventCallback = pCallback;       /* 赋值或初始化 */
      break;                                            /* 跳出 */

    case HAL_RTC_TAMPER1_EVENT_CB_ID :                  /* case标签 */
      hrtc->Tamper1EventCallback = pCallback;           /* 赋值或初始化 */
      break;                                            /* 跳出 */

    case HAL_RTC_TAMPER2_EVENT_CB_ID :                  /* case标签 */
      hrtc->Tamper2EventCallback = pCallback;           /* 赋值或初始化 */
      break;                                            /* 跳出 */

   case HAL_RTC_MSPINIT_CB_ID :                         /* case标签 */
      hrtc->MspInitCallback = pCallback;                /* 赋值或初始化 */
      break;                                            /* 跳出 */

   case HAL_RTC_MSPDEINIT_CB_ID :                       /* case标签 */
      hrtc->MspDeInitCallback = pCallback;              /* 赋值或初始化 */
      break;                                            /* 跳出 */

    default :                                           /* default分支 */
     /* Return error status */
      status =  HAL_ERROR;                              /* 赋值或初始化 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if(HAL_RTC_STATE_RESET == hrtc->State)           /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
    case HAL_RTC_MSPINIT_CB_ID :                        /* case标签 */
      hrtc->MspInitCallback = pCallback;                /* 赋值或初始化 */
      break;                                            /* 跳出 */

   case HAL_RTC_MSPDEINIT_CB_ID :                       /* case标签 */
      hrtc->MspDeInitCallback = pCallback;              /* 赋值或初始化 */
      break;                                            /* 跳出 */

    default :                                           /* default分支 */
     /* Return error status */
      status =  HAL_ERROR;                              /* 赋值或初始化 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hrtc);                                   /* HAL辅助宏/函数 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Unregister an RTC Callback
  *         RTC callabck is redirected to the weak predefined callback
  * @param  hrtc RTC handle
  * @param  CallbackID ID of the callback to be unregistered
  *         This parameter can be one of the following values:
  *          @arg @ref HAL_RTC_ALARM_A_EVENT_CB_ID          Alarm A Event Callback ID
  *          @arg @ref HAL_RTC_ALARM_B_EVENT_CB_ID          Alarm B Event Callback ID
  *          @arg @ref HAL_RTC_TIMESTAMP_EVENT_CB_ID        TimeStamp Event Callback ID
  *          @arg @ref HAL_RTC_WAKEUPTIMER_EVENT_CB_ID      Wake-Up Timer Event Callback ID
  *          @arg @ref HAL_RTC_TAMPER1_EVENT_CB_ID          Tamper 1 Callback ID
  *          @arg @ref HAL_RTC_TAMPER2_EVENT_CB_ID          Tamper 2 Callback ID
  *          @arg @ref HAL_RTC_MSPINIT_CB_ID Msp Init callback ID
  *          @arg @ref HAL_RTC_MSPDEINIT_CB_ID Msp DeInit callback ID
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTC_UnRegisterCallback(RTC_HandleTypeDef *hrtc, HAL_RTC_CallbackIDTypeDef CallbackID)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Process locked */
  __HAL_LOCK(hrtc);                                     /* HAL辅助宏/函数 */

  if(HAL_RTC_STATE_READY == hrtc->State)                /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
    case HAL_RTC_ALARM_A_EVENT_CB_ID :                  /* case标签 */
      hrtc->AlarmAEventCallback = HAL_RTC_AlarmAEventCallback;         /* Legacy weak AlarmAEventCallback    */
      break;                                            /* 跳出 */

    case HAL_RTC_ALARM_B_EVENT_CB_ID :                  /* case标签 */
      hrtc->AlarmBEventCallback = HAL_RTCEx_AlarmBEventCallback;          /* Legacy weak AlarmBEventCallback */
      break;                                            /* 跳出 */

    case HAL_RTC_TIMESTAMP_EVENT_CB_ID :                /* case标签 */
      hrtc->TimeStampEventCallback = HAL_RTCEx_TimeStampEventCallback;    /* Legacy weak TimeStampEventCallback    */
      break;                                            /* 跳出 */

    case HAL_RTC_WAKEUPTIMER_EVENT_CB_ID :              /* case标签 */
      hrtc->WakeUpTimerEventCallback = HAL_RTCEx_WakeUpTimerEventCallback; /* Legacy weak WakeUpTimerEventCallback */
      break;                                            /* 跳出 */

    case HAL_RTC_TAMPER1_EVENT_CB_ID :                  /* case标签 */
      hrtc->Tamper1EventCallback = HAL_RTCEx_Tamper1EventCallback;         /* Legacy weak Tamper1EventCallback   */
      break;                                            /* 跳出 */

    case HAL_RTC_TAMPER2_EVENT_CB_ID :                  /* case标签 */
      hrtc->Tamper2EventCallback = HAL_RTCEx_Tamper2EventCallback;         /* Legacy weak Tamper2EventCallback         */
      break;                                            /* 跳出 */

    case HAL_RTC_MSPINIT_CB_ID :                        /* case标签 */
      hrtc->MspInitCallback = HAL_RTC_MspInit;          /* 赋值或初始化 */
      break;                                            /* 跳出 */

    case HAL_RTC_MSPDEINIT_CB_ID :                      /* case标签 */
      hrtc->MspDeInitCallback = HAL_RTC_MspDeInit;      /* 赋值或初始化 */
      break;                                            /* 跳出 */

    default :                                           /* default分支 */
     /* Return error status */
      status =  HAL_ERROR;                              /* 赋值或初始化 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if(HAL_RTC_STATE_RESET == hrtc->State)           /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
    case HAL_RTC_MSPINIT_CB_ID :                        /* case标签 */
      hrtc->MspInitCallback = HAL_RTC_MspInit;          /* 赋值或初始化 */
      break;                                            /* 跳出 */

    case HAL_RTC_MSPDEINIT_CB_ID :                      /* case标签 */
      hrtc->MspDeInitCallback = HAL_RTC_MspDeInit;      /* 赋值或初始化 */
      break;                                            /* 跳出 */

    default :                                           /* default分支 */
     /* Return error status */
      status =  HAL_ERROR;                              /* 赋值或初始化 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hrtc);                                   /* HAL辅助宏/函数 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* USE_HAL_RTC_REGISTER_CALLBACKS */

/**
  * @brief  Initializes the RTC MSP.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @retval None
  */
__weak void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc)    /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hrtc);                                         /* 执行语句 */
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_RTC_MspInit could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  DeInitializes the RTC MSP.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @retval None
  */
__weak void HAL_RTC_MspDeInit(RTC_HandleTypeDef* hrtc)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hrtc);                                         /* 执行语句 */
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_RTC_MspDeInit could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup RTC_Exported_Functions_Group2 RTC Time and Date functions
 *  @brief   RTC Time and Date functions
 *
@verbatim
 ===============================================================================
                 ##### RTC Time and Date functions #####
 ===============================================================================

 [..] This section provides functions allowing to configure Time and Date features

@endverbatim
  * @{
  */

/**
  * @brief  Sets RTC current time.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @param  sTime Pointer to Time structure
  * @param  Format Specifies the format of the entered parameters.
  *          This parameter can be one of the following values:
  *            @arg RTC_FORMAT_BIN: Binary data format
  *            @arg RTC_FORMAT_BCD: BCD data format
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTC_SetTime(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, uint32_t Format)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpreg = 0U;                                 /* 32位无符号变量/函数 */

 /* Check the parameters */
  assert_param(IS_RTC_FORMAT(Format));                  /* 执行语句 */
  assert_param(IS_RTC_DAYLIGHT_SAVING(sTime->DayLightSaving));  /* 执行语句 */
  assert_param(IS_RTC_STORE_OPERATION(sTime->StoreOperation));  /* 执行语句 */

  /* Process Locked */
  __HAL_LOCK(hrtc);                                     /* HAL辅助宏/函数 */

  hrtc->State = HAL_RTC_STATE_BUSY;                     /* 赋值或初始化 */

  if(Format == RTC_FORMAT_BIN)                          /* 条件判断 */
  {                                                     /* 代码块开始 */
    if((hrtc->Instance->CR & RTC_CR_FMT) != (uint32_t)RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      assert_param(IS_RTC_HOUR12(sTime->Hours));        /* 执行语句 */
      assert_param(IS_RTC_HOURFORMAT12(sTime->TimeFormat));  /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      sTime->TimeFormat = 0x00U;                        /* 赋值或初始化 */
      assert_param(IS_RTC_HOUR24(sTime->Hours));        /* 执行语句 */
    }                                                   /* 代码块结束 */
    assert_param(IS_RTC_MINUTES(sTime->Minutes));       /* 执行语句 */
    assert_param(IS_RTC_SECONDS(sTime->Seconds));       /* 执行语句 */

    tmpreg = (uint32_t)(((uint32_t)RTC_ByteToBcd2(sTime->Hours) << 16U) | \
                        ((uint32_t)RTC_ByteToBcd2(sTime->Minutes) << 8U) | \
                        ((uint32_t)RTC_ByteToBcd2(sTime->Seconds)) | \
                        (((uint32_t)sTime->TimeFormat) << 16U));  /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    if((hrtc->Instance->CR & RTC_CR_FMT) != (uint32_t)RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      assert_param(IS_RTC_HOUR12(RTC_Bcd2ToByte(sTime->Hours)));  /* 执行语句 */
      assert_param(IS_RTC_HOURFORMAT12(sTime->TimeFormat));  /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      sTime->TimeFormat = 0x00U;                        /* 赋值或初始化 */
      assert_param(IS_RTC_HOUR24(RTC_Bcd2ToByte(sTime->Hours)));  /* 执行语句 */
    }                                                   /* 代码块结束 */
    assert_param(IS_RTC_MINUTES(RTC_Bcd2ToByte(sTime->Minutes)));  /* 执行语句 */
    assert_param(IS_RTC_SECONDS(RTC_Bcd2ToByte(sTime->Seconds)));  /* 执行语句 */
    tmpreg = (((uint32_t)(sTime->Hours) << 16U) | \
              ((uint32_t)(sTime->Minutes) << 8U) | \
              ((uint32_t)sTime->Seconds) | \
              ((uint32_t)(sTime->TimeFormat) << 16U));  /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Disable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_DISABLE(hrtc);              /* HAL辅助宏/函数 */

  /* Set Initialization mode */
  if(RTC_EnterInitMode(hrtc) != HAL_OK)                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable the write protection for RTC registers */
    __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);             /* HAL辅助宏/函数 */

    /* Set RTC state */
    hrtc->State = HAL_RTC_STATE_ERROR;                  /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hrtc);                                 /* HAL辅助宏/函数 */

    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Set the RTC_TR register */
    hrtc->Instance->TR = (uint32_t)(tmpreg & RTC_TR_RESERVED_MASK);  /* 赋值或初始化 */

    /* This interface is deprecated. To manage Daylight Saving Time, please use HAL_RTC_DST_xxx functions */
    hrtc->Instance->CR &= (uint32_t)~RTC_CR_BCK;        /* 赋值或初始化 */

    /* This interface is deprecated. To manage Daylight Saving Time, please use HAL_RTC_DST_xxx functions */
    hrtc->Instance->CR |= (uint32_t)(sTime->DayLightSaving | sTime->StoreOperation);  /* 赋值或初始化 */

    /* Exit Initialization mode */
    hrtc->Instance->ISR &= (uint32_t)~RTC_ISR_INIT;     /* 赋值或初始化 */

    /* If  CR_BYPSHAD bit = 0, wait for synchro else this check is not needed */
    if((hrtc->Instance->CR & RTC_CR_BYPSHAD) == RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      if(HAL_RTC_WaitForSynchro(hrtc) != HAL_OK)        /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Enable the write protection for RTC registers */
        __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);         /* HAL辅助宏/函数 */

        hrtc->State = HAL_RTC_STATE_ERROR;              /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hrtc);                             /* HAL辅助宏/函数 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Enable the write protection for RTC registers */
    __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);             /* HAL辅助宏/函数 */

   hrtc->State = HAL_RTC_STATE_READY;                   /* 赋值或初始化 */

   __HAL_UNLOCK(hrtc);                                  /* HAL辅助宏/函数 */

   return HAL_OK;                                       /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Gets RTC current time.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @param  sTime Pointer to Time structure
  * @param  Format Specifies the format of the entered parameters.
  *          This parameter can be one of the following values:
  *            @arg RTC_FORMAT_BIN: Binary data format
  *            @arg RTC_FORMAT_BCD: BCD data format
  * @note  You can use SubSeconds and SecondFraction (sTime structure fields returned) to convert SubSeconds
  *        value in second fraction ratio with time unit following generic formula:
  *        Second fraction ratio * time_unit= [(SecondFraction-SubSeconds)/(SecondFraction+1)] * time_unit
  *        This conversion can be performed only if no shift operation is pending (ie. SHFP=0) when PREDIV_S >= SS
  * @note You must call HAL_RTC_GetDate() after HAL_RTC_GetTime() to unlock the values
  *        in the higher-order calendar shadow registers to ensure consistency between the time and date values.
  *        Reading RTC current time locks the values in calendar shadow registers until current date is read.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTC_GetTime(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, uint32_t Format)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpreg = 0U;                                 /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_RTC_FORMAT(Format));                  /* 执行语句 */

  /* Get subseconds structure field from the corresponding register */
  sTime->SubSeconds = (uint32_t)(hrtc->Instance->SSR);  /* 赋值或初始化 */

  /* Get SecondFraction structure field from the corresponding register field*/
  sTime->SecondFraction = (uint32_t)(hrtc->Instance->PRER & RTC_PRER_PREDIV_S);  /* 赋值或初始化 */

  /* Get the TR register */
  tmpreg = (uint32_t)(hrtc->Instance->TR & RTC_TR_RESERVED_MASK);  /* 赋值或初始化 */

  /* Fill the structure fields with the read parameters */
  sTime->Hours = (uint8_t)((tmpreg & (RTC_TR_HT | RTC_TR_HU)) >> 16U);  /* 赋值或初始化 */
  sTime->Minutes = (uint8_t)((tmpreg & (RTC_TR_MNT | RTC_TR_MNU)) >> 8U);  /* 赋值或初始化 */
  sTime->Seconds = (uint8_t)(tmpreg & (RTC_TR_ST | RTC_TR_SU));  /* 赋值或初始化 */
  sTime->TimeFormat = (uint8_t)((tmpreg & (RTC_TR_PM)) >> 16U);  /* 赋值或初始化 */

  /* Check the input parameters format */
  if(Format == RTC_FORMAT_BIN)                          /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Convert the time structure parameters to Binary format */
    sTime->Hours = (uint8_t)RTC_Bcd2ToByte(sTime->Hours);  /* 赋值或初始化 */
    sTime->Minutes = (uint8_t)RTC_Bcd2ToByte(sTime->Minutes);  /* 赋值或初始化 */
    sTime->Seconds = (uint8_t)RTC_Bcd2ToByte(sTime->Seconds);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Sets RTC current date.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @param  sDate Pointer to date structure
  * @param  Format specifies the format of the entered parameters.
  *          This parameter can be one of the following values:
  *            @arg RTC_FORMAT_BIN: Binary data format
  *            @arg RTC_FORMAT_BCD: BCD data format
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTC_SetDate(RTC_HandleTypeDef *hrtc, RTC_DateTypeDef *sDate, uint32_t Format)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t datetmpreg = 0U;                             /* 32位无符号变量/函数 */

 /* Check the parameters */
  assert_param(IS_RTC_FORMAT(Format));                  /* 执行语句 */

 /* Process Locked */
 __HAL_LOCK(hrtc);                                      /* HAL辅助宏/函数 */

  hrtc->State = HAL_RTC_STATE_BUSY;                     /* 赋值或初始化 */

  if((Format == RTC_FORMAT_BIN) && ((sDate->Month & 0x10U) == 0x10U))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    sDate->Month = (uint8_t)((sDate->Month & (uint8_t)~(0x10U)) + (uint8_t)0x0AU);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  assert_param(IS_RTC_WEEKDAY(sDate->WeekDay));         /* 执行语句 */

  if(Format == RTC_FORMAT_BIN)                          /* 条件判断 */
  {                                                     /* 代码块开始 */
    assert_param(IS_RTC_YEAR(sDate->Year));             /* 执行语句 */
    assert_param(IS_RTC_MONTH(sDate->Month));           /* 执行语句 */
    assert_param(IS_RTC_DATE(sDate->Date));             /* 执行语句 */

   datetmpreg = (((uint32_t)RTC_ByteToBcd2(sDate->Year) << 16U) | \
                 ((uint32_t)RTC_ByteToBcd2(sDate->Month) << 8U) | \
                 ((uint32_t)RTC_ByteToBcd2(sDate->Date)) | \
                 ((uint32_t)sDate->WeekDay << 13U));    /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    assert_param(IS_RTC_YEAR(RTC_Bcd2ToByte(sDate->Year)));  /* 执行语句 */
    assert_param(IS_RTC_MONTH(RTC_Bcd2ToByte(sDate->Month)));  /* 执行语句 */
    assert_param(IS_RTC_DATE(RTC_Bcd2ToByte(sDate->Date)));  /* 执行语句 */

    datetmpreg = ((((uint32_t)sDate->Year) << 16U) | \
                  (((uint32_t)sDate->Month) << 8U) | \
                  ((uint32_t)sDate->Date) | \
                  (((uint32_t)sDate->WeekDay) << 13U));  /* 执行语句 */
  }                                                     /* 代码块结束 */

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
    /* Set the RTC_DR register */
    hrtc->Instance->DR = (uint32_t)(datetmpreg & RTC_DR_RESERVED_MASK);  /* 赋值或初始化 */

    /* Exit Initialization mode */
    hrtc->Instance->ISR &= (uint32_t)~RTC_ISR_INIT;     /* 赋值或初始化 */

    /* If  CR_BYPSHAD bit = 0, wait for synchro else this check is not needed */
    if((hrtc->Instance->CR & RTC_CR_BYPSHAD) == RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      if(HAL_RTC_WaitForSynchro(hrtc) != HAL_OK)        /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Enable the write protection for RTC registers */
        __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);         /* HAL辅助宏/函数 */

        hrtc->State = HAL_RTC_STATE_ERROR;              /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hrtc);                             /* HAL辅助宏/函数 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Enable the write protection for RTC registers */
    __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);             /* HAL辅助宏/函数 */

    hrtc->State = HAL_RTC_STATE_READY ;                 /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hrtc);                                 /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Gets RTC current date.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @param  sDate Pointer to Date structure
  * @param  Format Specifies the format of the entered parameters.
  *          This parameter can be one of the following values:
  *            @arg RTC_FORMAT_BIN:  Binary data format
  *            @arg RTC_FORMAT_BCD:  BCD data format
  * @note You must call HAL_RTC_GetDate() after HAL_RTC_GetTime() to unlock the values
  * in the higher-order calendar shadow registers to ensure consistency between the time and date values.
  * Reading RTC current time locks the values in calendar shadow registers until Current date is read.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTC_GetDate(RTC_HandleTypeDef *hrtc, RTC_DateTypeDef *sDate, uint32_t Format)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t datetmpreg = 0U;                             /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_RTC_FORMAT(Format));                  /* 执行语句 */

  /* Get the DR register */
  datetmpreg = (uint32_t)(hrtc->Instance->DR & RTC_DR_RESERVED_MASK);  /* 赋值或初始化 */

  /* Fill the structure fields with the read parameters */
  sDate->Year = (uint8_t)((datetmpreg & (RTC_DR_YT | RTC_DR_YU)) >> 16U);  /* 赋值或初始化 */
  sDate->Month = (uint8_t)((datetmpreg & (RTC_DR_MT | RTC_DR_MU)) >> 8U);  /* 赋值或初始化 */
  sDate->Date = (uint8_t)(datetmpreg & (RTC_DR_DT | RTC_DR_DU));  /* 赋值或初始化 */
  sDate->WeekDay = (uint8_t)((datetmpreg & (RTC_DR_WDU)) >> 13U);  /* 赋值或初始化 */

  /* Check the input parameters format */
  if(Format == RTC_FORMAT_BIN)                          /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Convert the date structure parameters to Binary format */
    sDate->Year = (uint8_t)RTC_Bcd2ToByte(sDate->Year);  /* 赋值或初始化 */
    sDate->Month = (uint8_t)RTC_Bcd2ToByte(sDate->Month);  /* 赋值或初始化 */
    sDate->Date = (uint8_t)RTC_Bcd2ToByte(sDate->Date);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup RTC_Exported_Functions_Group3 RTC Alarm functions
 *  @brief   RTC Alarm functions
 *
@verbatim
 ===============================================================================
                 ##### RTC Alarm functions #####
 ===============================================================================

 [..] This section provides functions allowing to configure Alarm feature

@endverbatim
  * @{
  */
/**
  * @brief  Sets the specified RTC Alarm.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @param  sAlarm Pointer to Alarm structure
  * @param  Format Specifies the format of the entered parameters.
  *          This parameter can be one of the following values:
  *             @arg RTC_FORMAT_BIN: Binary data format
  *             @arg RTC_FORMAT_BCD: BCD data format
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTC_SetAlarm(RTC_HandleTypeDef *hrtc, RTC_AlarmTypeDef *sAlarm, uint32_t Format)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart = 0U;                              /* 32位无符号变量/函数 */
  uint32_t tmpreg = 0U, subsecondtmpreg = 0U;           /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_RTC_FORMAT(Format));                  /* 执行语句 */
  assert_param(IS_RTC_ALARM(sAlarm->Alarm));            /* 执行语句 */
  assert_param(IS_RTC_ALARM_MASK(sAlarm->AlarmMask));   /* 执行语句 */
  assert_param(IS_RTC_ALARM_DATE_WEEKDAY_SEL(sAlarm->AlarmDateWeekDaySel));  /* 执行语句 */
  assert_param(IS_RTC_ALARM_SUB_SECOND_VALUE(sAlarm->AlarmTime.SubSeconds));  /* 执行语句 */
  assert_param(IS_RTC_ALARM_SUB_SECOND_MASK(sAlarm->AlarmSubSecondMask));  /* 执行语句 */

  /* Process Locked */
  __HAL_LOCK(hrtc);                                     /* HAL辅助宏/函数 */

  hrtc->State = HAL_RTC_STATE_BUSY;                     /* 赋值或初始化 */

  if(Format == RTC_FORMAT_BIN)                          /* 条件判断 */
  {                                                     /* 代码块开始 */
    if((hrtc->Instance->CR & RTC_CR_FMT) != (uint32_t)RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      assert_param(IS_RTC_HOUR12(sAlarm->AlarmTime.Hours));  /* 执行语句 */
      assert_param(IS_RTC_HOURFORMAT12(sAlarm->AlarmTime.TimeFormat));  /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      sAlarm->AlarmTime.TimeFormat = 0x00U;             /* 赋值或初始化 */
      assert_param(IS_RTC_HOUR24(sAlarm->AlarmTime.Hours));  /* 执行语句 */
    }                                                   /* 代码块结束 */
    assert_param(IS_RTC_MINUTES(sAlarm->AlarmTime.Minutes));  /* 执行语句 */
    assert_param(IS_RTC_SECONDS(sAlarm->AlarmTime.Seconds));  /* 执行语句 */

    if(sAlarm->AlarmDateWeekDaySel == RTC_ALARMDATEWEEKDAYSEL_DATE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      assert_param(IS_RTC_ALARM_DATE_WEEKDAY_DATE(sAlarm->AlarmDateWeekDay));  /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      assert_param(IS_RTC_ALARM_DATE_WEEKDAY_WEEKDAY(sAlarm->AlarmDateWeekDay));  /* 执行语句 */
    }                                                   /* 代码块结束 */

    tmpreg = (((uint32_t)RTC_ByteToBcd2(sAlarm->AlarmTime.Hours) << 16U) | \
              ((uint32_t)RTC_ByteToBcd2(sAlarm->AlarmTime.Minutes) << 8U) | \
              ((uint32_t)RTC_ByteToBcd2(sAlarm->AlarmTime.Seconds)) | \
              ((uint32_t)(sAlarm->AlarmTime.TimeFormat) << 16U) | \
              ((uint32_t)RTC_ByteToBcd2(sAlarm->AlarmDateWeekDay) << 24U) | \
              ((uint32_t)sAlarm->AlarmDateWeekDaySel) | \
              ((uint32_t)sAlarm->AlarmMask));           /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    if((hrtc->Instance->CR & RTC_CR_FMT) != (uint32_t)RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      assert_param(IS_RTC_HOUR12(RTC_Bcd2ToByte(sAlarm->AlarmTime.Hours)));  /* 执行语句 */
      assert_param(IS_RTC_HOURFORMAT12(sAlarm->AlarmTime.TimeFormat));  /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      sAlarm->AlarmTime.TimeFormat = 0x00U;             /* 赋值或初始化 */
      assert_param(IS_RTC_HOUR24(RTC_Bcd2ToByte(sAlarm->AlarmTime.Hours)));  /* 执行语句 */
    }                                                   /* 代码块结束 */

    assert_param(IS_RTC_MINUTES(RTC_Bcd2ToByte(sAlarm->AlarmTime.Minutes)));  /* 执行语句 */
    assert_param(IS_RTC_SECONDS(RTC_Bcd2ToByte(sAlarm->AlarmTime.Seconds)));  /* 执行语句 */

    if(sAlarm->AlarmDateWeekDaySel == RTC_ALARMDATEWEEKDAYSEL_DATE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      assert_param(IS_RTC_ALARM_DATE_WEEKDAY_DATE(RTC_Bcd2ToByte(sAlarm->AlarmDateWeekDay)));  /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      assert_param(IS_RTC_ALARM_DATE_WEEKDAY_WEEKDAY(RTC_Bcd2ToByte(sAlarm->AlarmDateWeekDay)));  /* 执行语句 */
    }                                                   /* 代码块结束 */

    tmpreg = (((uint32_t)(sAlarm->AlarmTime.Hours) << 16U) | \
              ((uint32_t)(sAlarm->AlarmTime.Minutes) << 8U) | \
              ((uint32_t) sAlarm->AlarmTime.Seconds) | \
              ((uint32_t)(sAlarm->AlarmTime.TimeFormat) << 16U) | \
              ((uint32_t)(sAlarm->AlarmDateWeekDay) << 24U) | \
              ((uint32_t)sAlarm->AlarmDateWeekDaySel) | \
              ((uint32_t)sAlarm->AlarmMask));           /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Configure the Alarm A or Alarm B Sub Second registers */
  subsecondtmpreg = (uint32_t)((uint32_t)(sAlarm->AlarmTime.SubSeconds) | (uint32_t)(sAlarm->AlarmSubSecondMask));  /* 赋值或初始化 */

  /* Disable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_DISABLE(hrtc);              /* HAL辅助宏/函数 */

  /* Configure the Alarm register */
  if(sAlarm->Alarm == RTC_ALARM_A)                      /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable the Alarm A interrupt */
    __HAL_RTC_ALARMA_DISABLE(hrtc);                     /* HAL辅助宏/函数 */

    /* In case of interrupt mode is used, the interrupt source must disabled */
    __HAL_RTC_ALARM_DISABLE_IT(hrtc, RTC_IT_ALRA);      /* HAL辅助宏/函数 */

    /* Get tick */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */

    /* Wait till RTC ALRAWF flag is set and if Time out is reached exit */
    while(__HAL_RTC_ALARM_GET_FLAG(hrtc, RTC_FLAG_ALRAWF) == RESET)  /* while循环 */
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

    hrtc->Instance->ALRMAR = (uint32_t)tmpreg;          /* 赋值或初始化 */
    /* Configure the Alarm A Sub Second register */
    hrtc->Instance->ALRMASSR = subsecondtmpreg;         /* 赋值或初始化 */
    /* Configure the Alarm state: Enable Alarm */
    __HAL_RTC_ALARMA_ENABLE(hrtc);                      /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Disable the Alarm B interrupt */
    __HAL_RTC_ALARMB_DISABLE(hrtc);                     /* HAL辅助宏/函数 */

    /* In case of interrupt mode is used, the interrupt source must disabled */
    __HAL_RTC_ALARM_DISABLE_IT(hrtc, RTC_IT_ALRB);      /* HAL辅助宏/函数 */

    /* Get tick */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */

    /* Wait till RTC ALRBWF flag is set and if Time out is reached exit */
    while(__HAL_RTC_ALARM_GET_FLAG(hrtc, RTC_FLAG_ALRBWF) == RESET)  /* while循环 */
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

    hrtc->Instance->ALRMBR = (uint32_t)tmpreg;          /* 赋值或初始化 */
    /* Configure the Alarm B Sub Second register */
    hrtc->Instance->ALRMBSSR = subsecondtmpreg;         /* 赋值或初始化 */
    /* Configure the Alarm state: Enable Alarm */
    __HAL_RTC_ALARMB_ENABLE(hrtc);                      /* HAL辅助宏/函数 */
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
  * @brief  Sets the specified RTC Alarm with Interrupt
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @param  sAlarm Pointer to Alarm structure
  * @param  Format Specifies the format of the entered parameters.
  *          This parameter can be one of the following values:
  *             @arg RTC_FORMAT_BIN: Binary data format
  *             @arg RTC_FORMAT_BCD: BCD data format
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTC_SetAlarm_IT(RTC_HandleTypeDef *hrtc, RTC_AlarmTypeDef *sAlarm, uint32_t Format)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpreg = 0U, subsecondtmpreg = 0U;           /* 32位无符号变量/函数 */
  __IO uint32_t count = RTC_TIMEOUT_VALUE  * (SystemCoreClock / 32U / 1000U) ;  /* 赋值或初始化 */

  /* Check the parameters */
  assert_param(IS_RTC_FORMAT(Format));                  /* 执行语句 */
  assert_param(IS_RTC_ALARM(sAlarm->Alarm));            /* 执行语句 */
  assert_param(IS_RTC_ALARM_MASK(sAlarm->AlarmMask));   /* 执行语句 */
  assert_param(IS_RTC_ALARM_DATE_WEEKDAY_SEL(sAlarm->AlarmDateWeekDaySel));  /* 执行语句 */
  assert_param(IS_RTC_ALARM_SUB_SECOND_VALUE(sAlarm->AlarmTime.SubSeconds));  /* 执行语句 */
  assert_param(IS_RTC_ALARM_SUB_SECOND_MASK(sAlarm->AlarmSubSecondMask));  /* 执行语句 */

  /* Process Locked */
  __HAL_LOCK(hrtc);                                     /* HAL辅助宏/函数 */

  hrtc->State = HAL_RTC_STATE_BUSY;                     /* 赋值或初始化 */

  if(Format == RTC_FORMAT_BIN)                          /* 条件判断 */
  {                                                     /* 代码块开始 */
    if((hrtc->Instance->CR & RTC_CR_FMT) != (uint32_t)RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      assert_param(IS_RTC_HOUR12(sAlarm->AlarmTime.Hours));  /* 执行语句 */
      assert_param(IS_RTC_HOURFORMAT12(sAlarm->AlarmTime.TimeFormat));  /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      sAlarm->AlarmTime.TimeFormat = 0x00U;             /* 赋值或初始化 */
      assert_param(IS_RTC_HOUR24(sAlarm->AlarmTime.Hours));  /* 执行语句 */
    }                                                   /* 代码块结束 */
    assert_param(IS_RTC_MINUTES(sAlarm->AlarmTime.Minutes));  /* 执行语句 */
    assert_param(IS_RTC_SECONDS(sAlarm->AlarmTime.Seconds));  /* 执行语句 */

    if(sAlarm->AlarmDateWeekDaySel == RTC_ALARMDATEWEEKDAYSEL_DATE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      assert_param(IS_RTC_ALARM_DATE_WEEKDAY_DATE(sAlarm->AlarmDateWeekDay));  /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      assert_param(IS_RTC_ALARM_DATE_WEEKDAY_WEEKDAY(sAlarm->AlarmDateWeekDay));  /* 执行语句 */
    }                                                   /* 代码块结束 */
    tmpreg = (((uint32_t)RTC_ByteToBcd2(sAlarm->AlarmTime.Hours) << 16U) | \
              ((uint32_t)RTC_ByteToBcd2(sAlarm->AlarmTime.Minutes) << 8U) | \
              ((uint32_t)RTC_ByteToBcd2(sAlarm->AlarmTime.Seconds)) | \
              ((uint32_t)(sAlarm->AlarmTime.TimeFormat) << 16U) | \
              ((uint32_t)RTC_ByteToBcd2(sAlarm->AlarmDateWeekDay) << 24U) | \
              ((uint32_t)sAlarm->AlarmDateWeekDaySel) | \
              ((uint32_t)sAlarm->AlarmMask));           /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    if((hrtc->Instance->CR & RTC_CR_FMT) != (uint32_t)RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      assert_param(IS_RTC_HOUR12(RTC_Bcd2ToByte(sAlarm->AlarmTime.Hours)));  /* 执行语句 */
      assert_param(IS_RTC_HOURFORMAT12(sAlarm->AlarmTime.TimeFormat));  /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      sAlarm->AlarmTime.TimeFormat = 0x00U;             /* 赋值或初始化 */
      assert_param(IS_RTC_HOUR24(RTC_Bcd2ToByte(sAlarm->AlarmTime.Hours)));  /* 执行语句 */
    }                                                   /* 代码块结束 */

    assert_param(IS_RTC_MINUTES(RTC_Bcd2ToByte(sAlarm->AlarmTime.Minutes)));  /* 执行语句 */
    assert_param(IS_RTC_SECONDS(RTC_Bcd2ToByte(sAlarm->AlarmTime.Seconds)));  /* 执行语句 */

    if(sAlarm->AlarmDateWeekDaySel == RTC_ALARMDATEWEEKDAYSEL_DATE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      assert_param(IS_RTC_ALARM_DATE_WEEKDAY_DATE(RTC_Bcd2ToByte(sAlarm->AlarmDateWeekDay)));  /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      assert_param(IS_RTC_ALARM_DATE_WEEKDAY_WEEKDAY(RTC_Bcd2ToByte(sAlarm->AlarmDateWeekDay)));  /* 执行语句 */
    }                                                   /* 代码块结束 */
    tmpreg = (((uint32_t)(sAlarm->AlarmTime.Hours) << 16U) | \
              ((uint32_t)(sAlarm->AlarmTime.Minutes) << 8U) | \
              ((uint32_t) sAlarm->AlarmTime.Seconds) | \
              ((uint32_t)(sAlarm->AlarmTime.TimeFormat) << 16U) | \
              ((uint32_t)(sAlarm->AlarmDateWeekDay) << 24U) | \
              ((uint32_t)sAlarm->AlarmDateWeekDaySel) | \
              ((uint32_t)sAlarm->AlarmMask));           /* 执行语句 */
  }                                                     /* 代码块结束 */
  /* Configure the Alarm A or Alarm B Sub Second registers */
  subsecondtmpreg = (uint32_t)((uint32_t)(sAlarm->AlarmTime.SubSeconds) | (uint32_t)(sAlarm->AlarmSubSecondMask));  /* 赋值或初始化 */

  /* Disable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_DISABLE(hrtc);              /* HAL辅助宏/函数 */

  /* Configure the Alarm register */
  if(sAlarm->Alarm == RTC_ALARM_A)                      /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable the Alarm A interrupt */
    __HAL_RTC_ALARMA_DISABLE(hrtc);                     /* HAL辅助宏/函数 */

    /* Clear flag alarm A */
    __HAL_RTC_ALARM_CLEAR_FLAG(hrtc, RTC_FLAG_ALRAF);   /* HAL辅助宏/函数 */

    /* Wait till RTC ALRAWF flag is set and if Time out is reached exit */
    do                                                  /* do-while循环 */
    {                                                   /* 代码块开始 */
      if (count-- == 0U)                                /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Enable the write protection for RTC registers */
        __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);         /* HAL辅助宏/函数 */

        hrtc->State = HAL_RTC_STATE_TIMEOUT;            /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hrtc);                             /* HAL辅助宏/函数 */

        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    while (__HAL_RTC_ALARM_GET_FLAG(hrtc, RTC_FLAG_ALRAWF) == RESET);  /* while循环 */

    hrtc->Instance->ALRMAR = (uint32_t)tmpreg;          /* 赋值或初始化 */
    /* Configure the Alarm A Sub Second register */
    hrtc->Instance->ALRMASSR = subsecondtmpreg;         /* 赋值或初始化 */
    /* Configure the Alarm state: Enable Alarm */
    __HAL_RTC_ALARMA_ENABLE(hrtc);                      /* HAL辅助宏/函数 */
    /* Configure the Alarm interrupt */
    __HAL_RTC_ALARM_ENABLE_IT(hrtc,RTC_IT_ALRA);        /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Disable the Alarm B interrupt */
    __HAL_RTC_ALARMB_DISABLE(hrtc);                     /* HAL辅助宏/函数 */

    /* Clear flag alarm B */
    __HAL_RTC_ALARM_CLEAR_FLAG(hrtc, RTC_FLAG_ALRBF);   /* HAL辅助宏/函数 */

    /* Wait till RTC ALRBWF flag is set and if Time out is reached exit */
    do                                                  /* do-while循环 */
    {                                                   /* 代码块开始 */
      if (count-- == 0U)                                /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Enable the write protection for RTC registers */
        __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);         /* HAL辅助宏/函数 */

        hrtc->State = HAL_RTC_STATE_TIMEOUT;            /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hrtc);                             /* HAL辅助宏/函数 */

        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    while (__HAL_RTC_ALARM_GET_FLAG(hrtc, RTC_FLAG_ALRBWF) == RESET);  /* while循环 */

    hrtc->Instance->ALRMBR = (uint32_t)tmpreg;          /* 赋值或初始化 */
    /* Configure the Alarm B Sub Second register */
    hrtc->Instance->ALRMBSSR = subsecondtmpreg;         /* 赋值或初始化 */
    /* Configure the Alarm state: Enable Alarm */
    __HAL_RTC_ALARMB_ENABLE(hrtc);                      /* HAL辅助宏/函数 */
    /* Configure the Alarm interrupt */
    __HAL_RTC_ALARM_ENABLE_IT(hrtc, RTC_IT_ALRB);       /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* RTC Alarm Interrupt Configuration: EXTI configuration */
  __HAL_RTC_ALARM_EXTI_ENABLE_IT();                     /* HAL辅助宏/函数 */

  EXTI->RTSR |= RTC_EXTI_LINE_ALARM_EVENT;              /* 赋值或初始化 */

  /* Enable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);               /* HAL辅助宏/函数 */

  hrtc->State = HAL_RTC_STATE_READY;                    /* 赋值或初始化 */

  /* Process Unlocked */
  __HAL_UNLOCK(hrtc);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Deactivate the specified RTC Alarm
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @param  Alarm Specifies the Alarm.
  *          This parameter can be one of the following values:
  *            @arg RTC_ALARM_A:  AlarmA
  *            @arg RTC_ALARM_B:  AlarmB
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTC_DeactivateAlarm(RTC_HandleTypeDef *hrtc, uint32_t Alarm)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart = 0U;                              /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_RTC_ALARM(Alarm));                    /* 执行语句 */

  /* Process Locked */
  __HAL_LOCK(hrtc);                                     /* HAL辅助宏/函数 */

  hrtc->State = HAL_RTC_STATE_BUSY;                     /* 赋值或初始化 */

  /* Disable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_DISABLE(hrtc);              /* HAL辅助宏/函数 */

  if(Alarm == RTC_ALARM_A)                              /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* AlarmA */
    __HAL_RTC_ALARMA_DISABLE(hrtc);                     /* HAL辅助宏/函数 */

    /* In case of interrupt mode is used, the interrupt source must disabled */
    __HAL_RTC_ALARM_DISABLE_IT(hrtc, RTC_IT_ALRA);      /* HAL辅助宏/函数 */

    /* Get tick */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */

    /* Wait till RTC ALRxWF flag is set and if Time out is reached exit */
    while(__HAL_RTC_ALARM_GET_FLAG(hrtc, RTC_FLAG_ALRAWF) == RESET)  /* while循环 */
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
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* AlarmB */
    __HAL_RTC_ALARMB_DISABLE(hrtc);                     /* HAL辅助宏/函数 */

    /* In case of interrupt mode is used, the interrupt source must disabled */
    __HAL_RTC_ALARM_DISABLE_IT(hrtc,RTC_IT_ALRB);       /* HAL辅助宏/函数 */

    /* Get tick */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */

    /* Wait till RTC ALRxWF flag is set and if Time out is reached exit */
    while(__HAL_RTC_ALARM_GET_FLAG(hrtc, RTC_FLAG_ALRBWF) == RESET)  /* while循环 */
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
  /* Enable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);               /* HAL辅助宏/函数 */

  hrtc->State = HAL_RTC_STATE_READY;                    /* 赋值或初始化 */

  /* Process Unlocked */
  __HAL_UNLOCK(hrtc);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Gets the RTC Alarm value and masks.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @param  sAlarm Pointer to Date structure
  * @param  Alarm Specifies the Alarm.
  *          This parameter can be one of the following values:
  *             @arg RTC_ALARM_A: AlarmA
  *             @arg RTC_ALARM_B: AlarmB
  * @param  Format Specifies the format of the entered parameters.
  *          This parameter can be one of the following values:
  *             @arg RTC_FORMAT_BIN: Binary data format
  *             @arg RTC_FORMAT_BCD: BCD data format
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTC_GetAlarm(RTC_HandleTypeDef *hrtc, RTC_AlarmTypeDef *sAlarm, uint32_t Alarm, uint32_t Format)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpreg = 0U, subsecondtmpreg = 0U;           /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_RTC_FORMAT(Format));                  /* 执行语句 */
  assert_param(IS_RTC_ALARM(Alarm));                    /* 执行语句 */

  if(Alarm == RTC_ALARM_A)                              /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* AlarmA */
    sAlarm->Alarm = RTC_ALARM_A;                        /* 赋值或初始化 */

    tmpreg = (uint32_t)(hrtc->Instance->ALRMAR);        /* 赋值或初始化 */
    subsecondtmpreg = (uint32_t)((hrtc->Instance->ALRMASSR ) & RTC_ALRMASSR_SS);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    sAlarm->Alarm = RTC_ALARM_B;                        /* 赋值或初始化 */

    tmpreg = (uint32_t)(hrtc->Instance->ALRMBR);        /* 赋值或初始化 */
    subsecondtmpreg = (uint32_t)((hrtc->Instance->ALRMBSSR) & RTC_ALRMBSSR_SS);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Fill the structure with the read parameters */
  sAlarm->AlarmTime.Hours = (uint32_t)((tmpreg & (RTC_ALRMAR_HT | RTC_ALRMAR_HU)) >> 16U);  /* 赋值或初始化 */
  sAlarm->AlarmTime.Minutes = (uint32_t)((tmpreg & (RTC_ALRMAR_MNT | RTC_ALRMAR_MNU)) >> 8U);  /* 赋值或初始化 */
  sAlarm->AlarmTime.Seconds = (uint32_t)(tmpreg & (RTC_ALRMAR_ST | RTC_ALRMAR_SU));  /* 赋值或初始化 */
  sAlarm->AlarmTime.TimeFormat = (uint32_t)((tmpreg & RTC_ALRMAR_PM) >> 16U);  /* 赋值或初始化 */
  sAlarm->AlarmTime.SubSeconds = (uint32_t) subsecondtmpreg;  /* 赋值或初始化 */
  sAlarm->AlarmDateWeekDay = (uint32_t)((tmpreg & (RTC_ALRMAR_DT | RTC_ALRMAR_DU)) >> 24U);  /* 赋值或初始化 */
  sAlarm->AlarmDateWeekDaySel = (uint32_t)(tmpreg & RTC_ALRMAR_WDSEL);  /* 赋值或初始化 */
  sAlarm->AlarmMask = (uint32_t)(tmpreg & RTC_ALARMMASK_ALL);  /* 赋值或初始化 */

  if(Format == RTC_FORMAT_BIN)                          /* 条件判断 */
  {                                                     /* 代码块开始 */
    sAlarm->AlarmTime.Hours = RTC_Bcd2ToByte(sAlarm->AlarmTime.Hours);  /* 赋值或初始化 */
    sAlarm->AlarmTime.Minutes = RTC_Bcd2ToByte(sAlarm->AlarmTime.Minutes);  /* 赋值或初始化 */
    sAlarm->AlarmTime.Seconds = RTC_Bcd2ToByte(sAlarm->AlarmTime.Seconds);  /* 赋值或初始化 */
    sAlarm->AlarmDateWeekDay = RTC_Bcd2ToByte(sAlarm->AlarmDateWeekDay);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  This function handles Alarm interrupt request.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @retval None
  */
void HAL_RTC_AlarmIRQHandler(RTC_HandleTypeDef* hrtc)   /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Get the AlarmA interrupt source enable status */
  if(__HAL_RTC_ALARM_GET_IT_SOURCE(hrtc, RTC_IT_ALRA) != (uint32_t)RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Get the pending status of the AlarmA Interrupt */
    if(__HAL_RTC_ALARM_GET_FLAG(hrtc, RTC_FLAG_ALRAF) != (uint32_t)RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* AlarmA callback */
    #if (USE_HAL_RTC_REGISTER_CALLBACKS == 1)           /* 条件编译 */
      hrtc->AlarmAEventCallback(hrtc);                  /* 执行语句 */
    #else                                               /* 条件编译否则 */
      HAL_RTC_AlarmAEventCallback(hrtc);                /* HAL库函数调用 */
    #endif /* USE_HAL_RTC_REGISTER_CALLBACKS */

      /* Clear the AlarmA interrupt pending bit */
      __HAL_RTC_ALARM_CLEAR_FLAG(hrtc,RTC_FLAG_ALRAF);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Get the AlarmB interrupt source enable status */
  if(__HAL_RTC_ALARM_GET_IT_SOURCE(hrtc, RTC_IT_ALRB) != (uint32_t)RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Get the pending status of the AlarmB Interrupt */
    if(__HAL_RTC_ALARM_GET_FLAG(hrtc, RTC_FLAG_ALRBF) != (uint32_t)RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* AlarmB callback */
    #if (USE_HAL_RTC_REGISTER_CALLBACKS == 1)           /* 条件编译 */
      hrtc->AlarmBEventCallback(hrtc);                  /* 执行语句 */
    #else                                               /* 条件编译否则 */
      HAL_RTCEx_AlarmBEventCallback(hrtc);              /* HAL库函数调用 */
    #endif /* USE_HAL_RTC_REGISTER_CALLBACKS */

      /* Clear the AlarmB interrupt pending bit */
      __HAL_RTC_ALARM_CLEAR_FLAG(hrtc,RTC_FLAG_ALRBF);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Clear the EXTI's line Flag for RTC Alarm */
  __HAL_RTC_ALARM_EXTI_CLEAR_FLAG();                    /* HAL辅助宏/函数 */

  /* Change RTC state */
  hrtc->State = HAL_RTC_STATE_READY;                    /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  Alarm A callback.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @retval None
  */
__weak void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hrtc);                                         /* 执行语句 */
  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_RTC_AlarmAEventCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  This function handles AlarmA Polling request.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTC_PollForAlarmAEvent(RTC_HandleTypeDef *hrtc, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart = 0U;                              /* 32位无符号变量/函数 */

    /* Get tick */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */

  while(__HAL_RTC_ALARM_GET_FLAG(hrtc, RTC_FLAG_ALRAF) == RESET)  /* while循环 */
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

  /* Clear the Alarm interrupt pending bit */
  __HAL_RTC_ALARM_CLEAR_FLAG(hrtc, RTC_FLAG_ALRAF);     /* HAL辅助宏/函数 */

  /* Change RTC state */
  hrtc->State = HAL_RTC_STATE_READY;                    /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup RTC_Exported_Functions_Group4 Peripheral Control functions
 *  @brief   Peripheral Control functions
 *
@verbatim
 ===============================================================================
                     ##### Peripheral Control functions #####
 ===============================================================================
    [..]
    This subsection provides functions allowing to
      (+) Wait for RTC Time and Date Synchronization

@endverbatim
  * @{
  */

/**
  * @brief  Waits until the RTC Time and Date registers (RTC_TR and RTC_DR) are
  *         synchronized with RTC APB clock.
  * @note   The RTC Resynchronization mode is write protected, use the
  *         __HAL_RTC_WRITEPROTECTION_DISABLE() before calling this function.
  * @note   To read the calendar through the shadow registers after Calendar
  *         initialization, calendar update or after wake-up from low power modes
  *         the software must first clear the RSF flag.
  *         The software must then wait until it is set again before reading
  *         the calendar, which means that the calendar registers have been
  *         correctly copied into the RTC_TR and RTC_DR shadow registers.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTC_WaitForSynchro(RTC_HandleTypeDef* hrtc)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart = 0U;                              /* 32位无符号变量/函数 */

  /* Clear RSF flag */
  hrtc->Instance->ISR &= (uint32_t)RTC_RSF_MASK;        /* 赋值或初始化 */

    /* Get tick */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */

  /* Wait the registers to be synchronised */
  while((hrtc->Instance->ISR & RTC_ISR_RSF) == (uint32_t)RESET)  /* while循环 */
  {                                                     /* 代码块开始 */
    if((HAL_GetTick() - tickstart ) > RTC_TIMEOUT_VALUE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_TIMEOUT;                               /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup RTC_Exported_Functions_Group5 Peripheral State functions
 *  @brief   Peripheral State functions
 *
@verbatim
 ===============================================================================
                     ##### Peripheral State functions #####
 ===============================================================================
    [..]
    This subsection provides functions allowing to
      (+) Get RTC state

@endverbatim
  * @{
  */
/**
  * @brief  Returns the RTC state.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @retval HAL state
  */
HAL_RTCStateTypeDef HAL_RTC_GetState(RTC_HandleTypeDef* hrtc)  /* HAL库函数调用 */
{                                                       /* 代码块开始 */
  return hrtc->State;                                   /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Daylight Saving Time, Add one hour to the calendar in one single operation
  *         without going through the initialization procedure.
  * @param  hrtc RTC handle
  * @retval None
  */
void HAL_RTC_DST_Add1Hour(RTC_HandleTypeDef *hrtc)      /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  UNUSED(hrtc);                                         /* 执行语句 */
  __HAL_RTC_WRITEPROTECTION_DISABLE(hrtc);              /* HAL辅助宏/函数 */
  SET_BIT(RTC->CR, RTC_CR_ADD1H);                       /* 执行语句 */
  __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);               /* HAL辅助宏/函数 */
}                                                       /* 代码块结束 */

/**
  * @brief  Daylight Saving Time, Substract one hour from the calendar in one
  *         single operation without going through the initialization procedure.
  * @param  hrtc RTC handle
  * @retval None
  */
void HAL_RTC_DST_Sub1Hour(RTC_HandleTypeDef *hrtc)      /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  UNUSED(hrtc);                                         /* 执行语句 */
  __HAL_RTC_WRITEPROTECTION_DISABLE(hrtc);              /* HAL辅助宏/函数 */
  SET_BIT(RTC->CR, RTC_CR_SUB1H);                       /* 执行语句 */
  __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);               /* HAL辅助宏/函数 */
}                                                       /* 代码块结束 */

/**
  * @brief  Daylight Saving Time, Set the store operation bit.
  * @note   It can be used by the software in order to memorize the DST status.
  * @param  hrtc RTC handle
  * @retval None
  */
void HAL_RTC_DST_SetStoreOperation(RTC_HandleTypeDef *hrtc)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  UNUSED(hrtc);                                         /* 执行语句 */
  __HAL_RTC_WRITEPROTECTION_DISABLE(hrtc);              /* HAL辅助宏/函数 */
  SET_BIT(RTC->CR, RTC_CR_BKP);                         /* 执行语句 */
  __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);               /* HAL辅助宏/函数 */
}                                                       /* 代码块结束 */

/**
  * @brief  Daylight Saving Time, Clear the store operation bit.
  * @param  hrtc RTC handle
  * @retval None
  */
void HAL_RTC_DST_ClearStoreOperation(RTC_HandleTypeDef *hrtc)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  UNUSED(hrtc);                                         /* 执行语句 */
  __HAL_RTC_WRITEPROTECTION_DISABLE(hrtc);              /* HAL辅助宏/函数 */
  CLEAR_BIT(RTC->CR, RTC_CR_BKP);                       /* 执行语句 */
  __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);               /* HAL辅助宏/函数 */
}                                                       /* 代码块结束 */

/**
  * @brief  Daylight Saving Time, Read the store operation bit.
  * @param  hrtc RTC handle
  * @retval operation see RTC_StoreOperation_Definitions
  */
uint32_t HAL_RTC_DST_ReadStoreOperation(RTC_HandleTypeDef *hrtc)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  UNUSED(hrtc);                                         /* 执行语句 */
  return READ_BIT(RTC->CR, RTC_CR_BKP);                 /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/**
  * @brief  Enters the RTC Initialization mode.
  * @note   The RTC Initialization mode is write protected, use the
  *         __HAL_RTC_WRITEPROTECTION_DISABLE() before calling this function.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @retval HAL status
  */
HAL_StatusTypeDef RTC_EnterInitMode(RTC_HandleTypeDef* hrtc)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart = 0U;                              /* 32位无符号变量/函数 */

  /* Check if the Initialization mode is set */
  if((hrtc->Instance->ISR & RTC_ISR_INITF) == (uint32_t)RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Set the Initialization mode */
    hrtc->Instance->ISR = (uint32_t)RTC_INIT_MASK;      /* 赋值或初始化 */

    /* Get tick */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */

    /* Wait till RTC is in INIT state and if Time out is reached exit */
    while((hrtc->Instance->ISR & RTC_ISR_INITF) == (uint32_t)RESET)  /* while循环 */
    {                                                   /* 代码块开始 */
      if((HAL_GetTick() - tickstart ) > RTC_TIMEOUT_VALUE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */


/**
  * @brief  Converts a 2 digit decimal to BCD format.
  * @param  Value Byte to be converted
  * @retval Converted byte
  */
uint8_t RTC_ByteToBcd2(uint8_t Value)                   /* 8位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t bcdhigh = 0U;                                /* 32位无符号变量/函数 */

  while(Value >= 10U)                                   /* while循环 */
  {                                                     /* 代码块开始 */
    bcdhigh++;                                          /* 执行语句 */
    Value -= 10U;                                       /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return  ((uint8_t)(bcdhigh << 4U) | Value);           /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Converts from 2 digit BCD to Binary.
  * @param  Value BCD value to be converted
  * @retval Converted word
  */
uint8_t RTC_Bcd2ToByte(uint8_t Value)                   /* 8位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t tmp = 0U;                                    /* 32位无符号变量/函数 */
  tmp = ((uint8_t)(Value & (uint8_t)0xF0) >> (uint8_t)0x4) * 10;  /* 赋值或初始化 */
  return (tmp + (Value & (uint8_t)0x0F));               /* 返回值 */
}                                                       /* 代码块结束 */

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
