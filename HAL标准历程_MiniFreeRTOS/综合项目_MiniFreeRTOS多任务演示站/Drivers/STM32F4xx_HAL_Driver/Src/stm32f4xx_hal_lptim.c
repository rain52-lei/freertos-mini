/* 文件: stm32f4xx_hal_lptim.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_lptim.c
  * @author  MCD Application Team
  * @brief   LPTIM HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Low Power Timer (LPTIM) peripheral:
  *           + Initialization and de-initialization functions.
  *           + Start/Stop operation functions in polling mode.
  *           + Start/Stop operation functions in interrupt mode.
  *           + Reading operation functions.
  *           + Peripheral State functions.
  *
  @verbatim
  ==============================================================================
                     ##### How to use this driver #####
  ==============================================================================
    [..]
      The LPTIM HAL driver can be used as follows:

      (#)Initialize the LPTIM low level resources by implementing the
        HAL_LPTIM_MspInit():
         (++) Enable the LPTIM interface clock using __HAL_RCC_LPTIMx_CLK_ENABLE().
         (++) In case of using interrupts (e.g. HAL_LPTIM_PWM_Start_IT()):
             (+++) Configure the LPTIM interrupt priority using HAL_NVIC_SetPriority().
             (+++) Enable the LPTIM IRQ handler using HAL_NVIC_EnableIRQ().
             (+++) In LPTIM IRQ handler, call HAL_LPTIM_IRQHandler().

      (#)Initialize the LPTIM HAL using HAL_LPTIM_Init(). This function
         configures mainly:
         (++) The instance: LPTIM1.
         (++) Clock: the counter clock.
             (+++) Source   : it can be either the ULPTIM input (IN1) or one of
                              the internal clock; (APB, LSE or LSI).
             (+++) Prescaler: select the clock divider.
         (++)  UltraLowPowerClock : To be used only if the ULPTIM is selected
               as counter clock source.
             (+++) Polarity:   polarity of the active edge for the counter unit
                               if the ULPTIM input is selected.
             (+++) SampleTime: clock sampling time to configure the clock glitch
                               filter.
         (++) Trigger: How the counter start.
             (+++) Source: trigger can be software or one of the hardware triggers.
             (+++) ActiveEdge : only for hardware trigger.
             (+++) SampleTime : trigger sampling time to configure the trigger
                                glitch filter.
         (++) OutputPolarity : 2 opposite polarities are possible.
         (++) UpdateMode: specifies whether the update of the autoreload and
              the compare values is done immediately or after the end of current
              period.

      (#)Six modes are available:

         (++) PWM Mode: To generate a PWM signal with specified period and pulse,
         call HAL_LPTIM_PWM_Start() or HAL_LPTIM_PWM_Start_IT() for interruption
         mode.

         (++) One Pulse Mode: To generate pulse with specified width in response
         to a stimulus, call HAL_LPTIM_OnePulse_Start() or
         HAL_LPTIM_OnePulse_Start_IT() for interruption mode.

         (++) Set once Mode: In this mode, the output changes the level (from
         low level to high level if the output polarity is configured high, else
         the opposite) when a compare match occurs. To start this mode, call
         HAL_LPTIM_SetOnce_Start() or HAL_LPTIM_SetOnce_Start_IT() for
         interruption mode.

         (++) Encoder Mode: To use the encoder interface call
         HAL_LPTIM_Encoder_Start() or HAL_LPTIM_Encoder_Start_IT() for
         interruption mode. Only available for LPTIM1 instance.

         (++) Time out Mode: an active edge on one selected trigger input rests
         the counter. The first trigger event will start the timer, any
         successive trigger event will reset the counter and the timer will
         restart. To start this mode call HAL_LPTIM_TimeOut_Start_IT() or
         HAL_LPTIM_TimeOut_Start_IT() for interruption mode.

         (++) Counter Mode: counter can be used to count external events on
         the LPTIM Input1 or it can be used to count internal clock cycles.
         To start this mode, call HAL_LPTIM_Counter_Start() or
         HAL_LPTIM_Counter_Start_IT() for interruption mode.


      (#) User can stop any process by calling the corresponding API:
          HAL_LPTIM_Xxx_Stop() or HAL_LPTIM_Xxx_Stop_IT() if the process is
          already started in interruption mode.

      (#) De-initialize the LPTIM peripheral using HAL_LPTIM_DeInit().

    *** Callback registration ***
  =============================================
  [..]
  The compilation define  USE_HAL_LPTIM_REGISTER_CALLBACKS when set to 1
  allows the user to configure dynamically the driver callbacks.
  [..]
  Use Function @ref HAL_LPTIM_RegisterCallback() to register a callback.
  @ref HAL_LPTIM_RegisterCallback() takes as parameters the HAL peripheral handle,
  the Callback ID and a pointer to the user callback function.
  [..]
  Use function @ref HAL_LPTIM_UnRegisterCallback() to reset a callback to the
  default weak function.
  @ref HAL_LPTIM_UnRegisterCallback takes as parameters the HAL peripheral handle,
  and the Callback ID.
  [..]
  These functions allow to register/unregister following callbacks:

    (+) MspInitCallback         : LPTIM Base Msp Init Callback.
    (+) MspDeInitCallback       : LPTIM Base Msp DeInit Callback.
    (+) CompareMatchCallback    : Compare match Callback.
    (+) AutoReloadMatchCallback : Auto-reload match Callback.
    (+) TriggerCallback         : External trigger event detection Callback.
    (+) CompareWriteCallback    : Compare register write complete Callback.
    (+) AutoReloadWriteCallback : Auto-reload register write complete Callback.
    (+) DirectionUpCallback     : Up-counting direction change Callback.
    (+) DirectionDownCallback   : Down-counting direction change Callback.

  [..]
  By default, after the Init and when the state is HAL_LPTIM_STATE_RESET
  all interrupt callbacks are set to the corresponding weak functions:
  examples @ref HAL_LPTIM_TriggerCallback(), @ref HAL_LPTIM_CompareMatchCallback().

  [..]
  Exception done for MspInit and MspDeInit functions that are reset to the legacy weak
  functionalities in the Init/DeInit only when these callbacks are null
  (not registered beforehand). If not, MspInit or MspDeInit are not null, the Init/DeInit
  keep and use the user MspInit/MspDeInit callbacks (registered beforehand)

  [..]
  Callbacks can be registered/unregistered in HAL_LPTIM_STATE_READY state only.
  Exception done MspInit/MspDeInit that can be registered/unregistered
  in HAL_LPTIM_STATE_READY or HAL_LPTIM_STATE_RESET state,
  thus registered (user) MspInit/DeInit callbacks can be used during the Init/DeInit.
  In that case first register the MspInit/MspDeInit user callbacks
  using @ref HAL_LPTIM_RegisterCallback() before calling DeInit or Init function.

  [..]
  When The compilation define USE_HAL_LPTIM_REGISTER_CALLBACKS is set to 0 or
  not defined, the callback registration feature is not available and all callbacks
  are set to the corresponding weak functions.

  @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
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

/** @defgroup LPTIM LPTIM
  * @brief LPTIM HAL module driver.
  * @{
  */

#ifdef HAL_LPTIM_MODULE_ENABLED                         /* 条件编译 */

#if defined (LPTIM1)                                    /* 条件编译 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @addtogroup LPTIM_Private_Constants
  * @{
  */
#define TIMEOUT                                     1000UL /* Timeout is 1s */
/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
#if (USE_HAL_LPTIM_REGISTER_CALLBACKS == 1)             /* 条件编译 */
static void LPTIM_ResetCallback(LPTIM_HandleTypeDef *lptim);  /* 静态函数 */
#endif /* USE_HAL_LPTIM_REGISTER_CALLBACKS */
static HAL_StatusTypeDef LPTIM_WaitForFlag(LPTIM_HandleTypeDef *hlptim, uint32_t flag);  /* 静态变量/函数 */

/* Exported functions --------------------------------------------------------*/

/** @defgroup LPTIM_Exported_Functions LPTIM Exported Functions
  * @{
  */

/** @defgroup LPTIM_Exported_Functions_Group1 Initialization/de-initialization functions
  *  @brief    Initialization and Configuration functions.
  *
@verbatim
  ==============================================================================
              ##### Initialization and de-initialization functions #####
  ==============================================================================
    [..]  This section provides functions allowing to:
      (+) Initialize the LPTIM according to the specified parameters in the
          LPTIM_InitTypeDef and initialize the associated handle.
      (+) DeInitialize the LPTIM peripheral.
      (+) Initialize the LPTIM MSP.
      (+) DeInitialize the LPTIM MSP.

@endverbatim
  * @{
  */

/**
  * @brief  Initialize the LPTIM according to the specified parameters in the
  *         LPTIM_InitTypeDef and initialize the associated handle.
  * @param  hlptim LPTIM handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_LPTIM_Init(LPTIM_HandleTypeDef *hlptim)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpcfgr;                                     /* 32位无符号变量/函数 */

  /* Check the LPTIM handle allocation */
  if (hlptim == NULL)                                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the parameters */
  assert_param(IS_LPTIM_INSTANCE(hlptim->Instance));    /* 执行语句 */

  assert_param(IS_LPTIM_CLOCK_SOURCE(hlptim->Init.Clock.Source));  /* 执行语句 */
  assert_param(IS_LPTIM_CLOCK_PRESCALER(hlptim->Init.Clock.Prescaler));  /* 执行语句 */
  if ((hlptim->Init.Clock.Source == LPTIM_CLOCKSOURCE_ULPTIM)   /* 条件判断 */
   || (hlptim->Init.CounterSource == LPTIM_COUNTERSOURCE_EXTERNAL))  /* 源代码 */
  {                                                     /* 代码块开始 */
    assert_param(IS_LPTIM_CLOCK_POLARITY(hlptim->Init.UltraLowPowerClock.Polarity));  /* 执行语句 */
    assert_param(IS_LPTIM_CLOCK_SAMPLE_TIME(hlptim->Init.UltraLowPowerClock.SampleTime));  /* 执行语句 */
  }                                                     /* 代码块结束 */
  assert_param(IS_LPTIM_TRG_SOURCE(hlptim->Init.Trigger.Source));  /* 执行语句 */
  if (hlptim->Init.Trigger.Source != LPTIM_TRIGSOURCE_SOFTWARE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    assert_param(IS_LPTIM_EXT_TRG_POLARITY(hlptim->Init.Trigger.ActiveEdge));  /* 执行语句 */
    assert_param(IS_LPTIM_TRIG_SAMPLE_TIME(hlptim->Init.Trigger.SampleTime));  /* 执行语句 */
  }                                                     /* 代码块结束 */
  assert_param(IS_LPTIM_OUTPUT_POLARITY(hlptim->Init.OutputPolarity));  /* 执行语句 */
  assert_param(IS_LPTIM_UPDATE_MODE(hlptim->Init.UpdateMode));  /* 执行语句 */
  assert_param(IS_LPTIM_COUNTER_SOURCE(hlptim->Init.CounterSource));  /* 执行语句 */

  if (hlptim->State == HAL_LPTIM_STATE_RESET)           /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Allocate lock resource and initialize it */
    hlptim->Lock = HAL_UNLOCKED;                        /* 赋值或初始化 */

#if (USE_HAL_LPTIM_REGISTER_CALLBACKS == 1)             /* 条件编译 */
    /* Reset interrupt callbacks to legacy weak callbacks */
    LPTIM_ResetCallback(hlptim);                        /* 执行语句 */

    if (hlptim->MspInitCallback == NULL)                /* 条件判断 */
    {                                                   /* 代码块开始 */
      hlptim->MspInitCallback = HAL_LPTIM_MspInit;      /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Init the low level hardware : GPIO, CLOCK, NVIC */
    hlptim->MspInitCallback(hlptim);                    /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /* Init the low level hardware : GPIO, CLOCK, NVIC */
    HAL_LPTIM_MspInit(hlptim);                          /* HAL库函数调用 */
#endif /* USE_HAL_LPTIM_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */

  /* Change the LPTIM state */
  hlptim->State = HAL_LPTIM_STATE_BUSY;                 /* 赋值或初始化 */

  /* Get the LPTIMx CFGR value */
  tmpcfgr = hlptim->Instance->CFGR;                     /* 赋值或初始化 */

  if ((hlptim->Init.Clock.Source == LPTIM_CLOCKSOURCE_ULPTIM)   /* 条件判断 */
   || (hlptim->Init.CounterSource == LPTIM_COUNTERSOURCE_EXTERNAL))  /* 源代码 */
  {                                                     /* 代码块开始 */
    tmpcfgr &= (uint32_t)(~(LPTIM_CFGR_CKPOL | LPTIM_CFGR_CKFLT));  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  if (hlptim->Init.Trigger.Source != LPTIM_TRIGSOURCE_SOFTWARE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    tmpcfgr &= (uint32_t)(~(LPTIM_CFGR_TRGFLT | LPTIM_CFGR_TRIGSEL));  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Clear CKSEL, PRESC, TRIGEN, TRGFLT, WAVPOL, PRELOAD & COUNTMODE bits */
  tmpcfgr &= (uint32_t)(~(LPTIM_CFGR_CKSEL | LPTIM_CFGR_TRIGEN | LPTIM_CFGR_PRELOAD |  /* 源代码 */
                          LPTIM_CFGR_WAVPOL | LPTIM_CFGR_PRESC | LPTIM_CFGR_COUNTMODE));  /* 执行语句 */

  /* Set initialization parameters */
  tmpcfgr |= (hlptim->Init.Clock.Source    |            /* 源代码 */
              hlptim->Init.Clock.Prescaler |            /* 源代码 */
              hlptim->Init.OutputPolarity  |            /* 源代码 */
              hlptim->Init.UpdateMode      |            /* 源代码 */
              hlptim->Init.CounterSource);              /* 执行语句 */

  /* Glitch filters for internal triggers and  external inputs are configured
   * only if an internal clock source is provided to the LPTIM
   */
  if (hlptim->Init.Clock.Source == LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    tmpcfgr |= (hlptim->Init.Trigger.SampleTime |       /* 源代码 */
                hlptim->Init.UltraLowPowerClock.SampleTime);  /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Configure LPTIM external clock polarity and digital filter */
  if ((hlptim->Init.Clock.Source == LPTIM_CLOCKSOURCE_ULPTIM)   /* 条件判断 */
   || (hlptim->Init.CounterSource == LPTIM_COUNTERSOURCE_EXTERNAL))  /* 源代码 */
  {                                                     /* 代码块开始 */
    tmpcfgr |= (hlptim->Init.UltraLowPowerClock.Polarity |  /* 源代码 */
                hlptim->Init.UltraLowPowerClock.SampleTime);  /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Configure LPTIM external trigger */
  if (hlptim->Init.Trigger.Source != LPTIM_TRIGSOURCE_SOFTWARE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable External trigger and set the trigger source */
    tmpcfgr |= (hlptim->Init.Trigger.Source     |       /* 源代码 */
                hlptim->Init.Trigger.ActiveEdge |       /* 源代码 */
                hlptim->Init.Trigger.SampleTime);       /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Write to LPTIMx CFGR */
  hlptim->Instance->CFGR = tmpcfgr;                     /* 赋值或初始化 */

  /* Change the LPTIM state */
  hlptim->State = HAL_LPTIM_STATE_READY;                /* 赋值或初始化 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  DeInitialize the LPTIM peripheral.
  * @param  hlptim LPTIM handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_LPTIM_DeInit(LPTIM_HandleTypeDef *hlptim)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the LPTIM handle allocation */
  if (hlptim == NULL)                                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Change the LPTIM state */
  hlptim->State = HAL_LPTIM_STATE_BUSY;                 /* 赋值或初始化 */

  /* Disable the LPTIM Peripheral Clock */
  __HAL_LPTIM_DISABLE(hlptim);                          /* HAL辅助宏/函数 */

  if (HAL_LPTIM_GetState(hlptim) == HAL_LPTIM_STATE_TIMEOUT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_TIMEOUT;                                 /* 返回值 */
  }                                                     /* 代码块结束 */

#if (USE_HAL_LPTIM_REGISTER_CALLBACKS == 1)             /* 条件编译 */
  if (hlptim->MspDeInitCallback == NULL)                /* 条件判断 */
  {                                                     /* 代码块开始 */
    hlptim->MspDeInitCallback = HAL_LPTIM_MspDeInit;    /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* DeInit the low level hardware: CLOCK, NVIC.*/
  hlptim->MspDeInitCallback(hlptim);                    /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* DeInit the low level hardware: CLOCK, NVIC.*/
  HAL_LPTIM_MspDeInit(hlptim);                          /* HAL库函数调用 */
#endif /* USE_HAL_LPTIM_REGISTER_CALLBACKS */

  /* Change the LPTIM state */
  hlptim->State = HAL_LPTIM_STATE_RESET;                /* 赋值或初始化 */

  /* Release Lock */
  __HAL_UNLOCK(hlptim);                                 /* HAL辅助宏/函数 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initialize the LPTIM MSP.
  * @param  hlptim LPTIM handle
  * @retval None
  */
__weak void HAL_LPTIM_MspInit(LPTIM_HandleTypeDef *hlptim)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hlptim);                                       /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_LPTIM_MspInit could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  DeInitialize LPTIM MSP.
  * @param  hlptim LPTIM handle
  * @retval None
  */
__weak void HAL_LPTIM_MspDeInit(LPTIM_HandleTypeDef *hlptim)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hlptim);                                       /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_LPTIM_MspDeInit could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup LPTIM_Exported_Functions_Group2 LPTIM Start-Stop operation functions
  *  @brief   Start-Stop operation functions.
  *
@verbatim
  ==============================================================================
                ##### LPTIM Start Stop operation functions #####
  ==============================================================================
    [..]  This section provides functions allowing to:
      (+) Start the PWM mode.
      (+) Stop the PWM mode.
      (+) Start the One pulse mode.
      (+) Stop the One pulse mode.
      (+) Start the Set once mode.
      (+) Stop the Set once mode.
      (+) Start the Encoder mode.
      (+) Stop the Encoder mode.
      (+) Start the Timeout mode.
      (+) Stop the Timeout mode.
      (+) Start the Counter mode.
      (+) Stop the Counter mode.


@endverbatim
  * @{
  */

/**
  * @brief  Start the LPTIM PWM generation.
  * @param  hlptim LPTIM handle
  * @param  Period Specifies the Autoreload value.
  *         This parameter must be a value between 0x0000 and 0xFFFF.
  * @param  Pulse Specifies the compare value.
  *         This parameter must be a value between 0x0000 and 0xFFFF.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_LPTIM_PWM_Start(LPTIM_HandleTypeDef *hlptim, uint32_t Period, uint32_t Pulse)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_LPTIM_INSTANCE(hlptim->Instance));    /* 执行语句 */
  assert_param(IS_LPTIM_PERIOD(Period));                /* 执行语句 */
  assert_param(IS_LPTIM_PULSE(Pulse));                  /* 执行语句 */

  /* Set the LPTIM state */
  hlptim->State = HAL_LPTIM_STATE_BUSY;                 /* 赋值或初始化 */

  /* Reset WAVE bit to set PWM mode */
  hlptim->Instance->CFGR &= ~LPTIM_CFGR_WAVE;           /* 赋值或初始化 */

  /* Enable the Peripheral */
  __HAL_LPTIM_ENABLE(hlptim);                           /* HAL辅助宏/函数 */

  /* Clear flag */
  __HAL_LPTIM_CLEAR_FLAG(hlptim, LPTIM_FLAG_ARROK);     /* HAL辅助宏/函数 */

  /* Load the period value in the autoreload register */
  __HAL_LPTIM_AUTORELOAD_SET(hlptim, Period);           /* HAL辅助宏/函数 */

  /* Wait for the completion of the write operation to the LPTIM_ARR register */
  if (LPTIM_WaitForFlag(hlptim, LPTIM_FLAG_ARROK) == HAL_TIMEOUT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_TIMEOUT;                                 /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Clear flag */
  __HAL_LPTIM_CLEAR_FLAG(hlptim, LPTIM_FLAG_CMPOK);     /* HAL辅助宏/函数 */

  /* Load the pulse value in the compare register */
  __HAL_LPTIM_COMPARE_SET(hlptim, Pulse);               /* HAL辅助宏/函数 */

  /* Wait for the completion of the write operation to the LPTIM_CMP register */
  if (LPTIM_WaitForFlag(hlptim, LPTIM_FLAG_CMPOK) == HAL_TIMEOUT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_TIMEOUT;                                 /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Start timer in continuous mode */
  __HAL_LPTIM_START_CONTINUOUS(hlptim);                 /* HAL辅助宏/函数 */

  /* Change the TIM state*/
  hlptim->State = HAL_LPTIM_STATE_READY;                /* 赋值或初始化 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Stop the LPTIM PWM generation.
  * @param  hlptim LPTIM handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_LPTIM_PWM_Stop(LPTIM_HandleTypeDef *hlptim)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_LPTIM_INSTANCE(hlptim->Instance));    /* 执行语句 */

  /* Set the LPTIM state */
  hlptim->State = HAL_LPTIM_STATE_BUSY;                 /* 赋值或初始化 */

  /* Disable the Peripheral */
  __HAL_LPTIM_DISABLE(hlptim);                          /* HAL辅助宏/函数 */

  if (HAL_LPTIM_GetState(hlptim) == HAL_LPTIM_STATE_TIMEOUT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_TIMEOUT;                                 /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Change the TIM state*/
  hlptim->State = HAL_LPTIM_STATE_READY;                /* 赋值或初始化 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Start the LPTIM PWM generation in interrupt mode.
  * @param  hlptim LPTIM handle
  * @param  Period Specifies the Autoreload value.
  *         This parameter must be a value between 0x0000 and 0xFFFF
  * @param  Pulse Specifies the compare value.
  *         This parameter must be a value between 0x0000 and 0xFFFF
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_LPTIM_PWM_Start_IT(LPTIM_HandleTypeDef *hlptim, uint32_t Period, uint32_t Pulse)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_LPTIM_INSTANCE(hlptim->Instance));    /* 执行语句 */
  assert_param(IS_LPTIM_PERIOD(Period));                /* 执行语句 */
  assert_param(IS_LPTIM_PULSE(Pulse));                  /* 执行语句 */

  /* Set the LPTIM state */
  hlptim->State = HAL_LPTIM_STATE_BUSY;                 /* 赋值或初始化 */

  /* Reset WAVE bit to set PWM mode */
  hlptim->Instance->CFGR &= ~LPTIM_CFGR_WAVE;           /* 赋值或初始化 */

  /* Enable the Peripheral */
  __HAL_LPTIM_ENABLE(hlptim);                           /* HAL辅助宏/函数 */

  /* Clear flag */
  __HAL_LPTIM_CLEAR_FLAG(hlptim, LPTIM_FLAG_ARROK);     /* HAL辅助宏/函数 */

  /* Load the period value in the autoreload register */
  __HAL_LPTIM_AUTORELOAD_SET(hlptim, Period);           /* HAL辅助宏/函数 */

  /* Wait for the completion of the write operation to the LPTIM_ARR register */
  if (LPTIM_WaitForFlag(hlptim, LPTIM_FLAG_ARROK) == HAL_TIMEOUT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_TIMEOUT;                                 /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Clear flag */
  __HAL_LPTIM_CLEAR_FLAG(hlptim, LPTIM_FLAG_CMPOK);     /* HAL辅助宏/函数 */

  /* Load the pulse value in the compare register */
  __HAL_LPTIM_COMPARE_SET(hlptim, Pulse);               /* HAL辅助宏/函数 */

  /* Wait for the completion of the write operation to the LPTIM_CMP register */
  if (LPTIM_WaitForFlag(hlptim, LPTIM_FLAG_CMPOK) == HAL_TIMEOUT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_TIMEOUT;                                 /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Disable the Peripheral */
  __HAL_LPTIM_DISABLE(hlptim);                          /* HAL辅助宏/函数 */

  if (HAL_LPTIM_GetState(hlptim) == HAL_LPTIM_STATE_TIMEOUT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_TIMEOUT;                                 /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Enable Autoreload write complete interrupt */
  __HAL_LPTIM_ENABLE_IT(hlptim, LPTIM_IT_ARROK);        /* HAL辅助宏/函数 */

  /* Enable Compare write complete interrupt */
  __HAL_LPTIM_ENABLE_IT(hlptim, LPTIM_IT_CMPOK);        /* HAL辅助宏/函数 */

  /* Enable Autoreload match interrupt */
  __HAL_LPTIM_ENABLE_IT(hlptim, LPTIM_IT_ARRM);         /* HAL辅助宏/函数 */

  /* Enable Compare match interrupt */
  __HAL_LPTIM_ENABLE_IT(hlptim, LPTIM_IT_CMPM);         /* HAL辅助宏/函数 */

  /* If external trigger source is used, then enable external trigger interrupt */
  if ((hlptim->Init.Trigger.Source) != LPTIM_TRIGSOURCE_SOFTWARE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable external trigger interrupt */
    __HAL_LPTIM_ENABLE_IT(hlptim, LPTIM_IT_EXTTRIG);    /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Enable the Peripheral */
  __HAL_LPTIM_ENABLE(hlptim);                           /* HAL辅助宏/函数 */

  /* Start timer in continuous mode */
  __HAL_LPTIM_START_CONTINUOUS(hlptim);                 /* HAL辅助宏/函数 */

  /* Change the TIM state*/
  hlptim->State = HAL_LPTIM_STATE_READY;                /* 赋值或初始化 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Stop the LPTIM PWM generation in interrupt mode.
  * @param  hlptim LPTIM handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_LPTIM_PWM_Stop_IT(LPTIM_HandleTypeDef *hlptim)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_LPTIM_INSTANCE(hlptim->Instance));    /* 执行语句 */

  /* Set the LPTIM state */
  hlptim->State = HAL_LPTIM_STATE_BUSY;                 /* 赋值或初始化 */

  /* Disable the Peripheral */
  __HAL_LPTIM_DISABLE(hlptim);                          /* HAL辅助宏/函数 */

  if (HAL_LPTIM_GetState(hlptim) == HAL_LPTIM_STATE_TIMEOUT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_TIMEOUT;                                 /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Disable Autoreload write complete interrupt */
  __HAL_LPTIM_DISABLE_IT(hlptim, LPTIM_IT_ARROK);       /* HAL辅助宏/函数 */

  /* Disable Compare write complete interrupt */
  __HAL_LPTIM_DISABLE_IT(hlptim, LPTIM_IT_CMPOK);       /* HAL辅助宏/函数 */

  /* Disable Autoreload match interrupt */
  __HAL_LPTIM_DISABLE_IT(hlptim, LPTIM_IT_ARRM);        /* HAL辅助宏/函数 */

  /* Disable Compare match interrupt */
  __HAL_LPTIM_DISABLE_IT(hlptim, LPTIM_IT_CMPM);        /* HAL辅助宏/函数 */

  /* If external trigger source is used, then disable external trigger interrupt */
  if ((hlptim->Init.Trigger.Source) != LPTIM_TRIGSOURCE_SOFTWARE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable external trigger interrupt */
    __HAL_LPTIM_DISABLE_IT(hlptim, LPTIM_IT_EXTTRIG);   /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Change the TIM state*/
  hlptim->State = HAL_LPTIM_STATE_READY;                /* 赋值或初始化 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Start the LPTIM One pulse generation.
  * @param  hlptim LPTIM handle
  * @param  Period Specifies the Autoreload value.
  *         This parameter must be a value between 0x0000 and 0xFFFF.
  * @param  Pulse Specifies the compare value.
  *         This parameter must be a value between 0x0000 and 0xFFFF.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_LPTIM_OnePulse_Start(LPTIM_HandleTypeDef *hlptim, uint32_t Period, uint32_t Pulse)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_LPTIM_INSTANCE(hlptim->Instance));    /* 执行语句 */
  assert_param(IS_LPTIM_PERIOD(Period));                /* 执行语句 */
  assert_param(IS_LPTIM_PULSE(Pulse));                  /* 执行语句 */

  /* Set the LPTIM state */
  hlptim->State = HAL_LPTIM_STATE_BUSY;                 /* 赋值或初始化 */

  /* Reset WAVE bit to set one pulse mode */
  hlptim->Instance->CFGR &= ~LPTIM_CFGR_WAVE;           /* 赋值或初始化 */

  /* Enable the Peripheral */
  __HAL_LPTIM_ENABLE(hlptim);                           /* HAL辅助宏/函数 */

  /* Clear flag */
  __HAL_LPTIM_CLEAR_FLAG(hlptim, LPTIM_FLAG_ARROK);     /* HAL辅助宏/函数 */

  /* Load the period value in the autoreload register */
  __HAL_LPTIM_AUTORELOAD_SET(hlptim, Period);           /* HAL辅助宏/函数 */

  /* Wait for the completion of the write operation to the LPTIM_ARR register */
  if (LPTIM_WaitForFlag(hlptim, LPTIM_FLAG_ARROK) == HAL_TIMEOUT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_TIMEOUT;                                 /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Clear flag */
  __HAL_LPTIM_CLEAR_FLAG(hlptim, LPTIM_FLAG_CMPOK);     /* HAL辅助宏/函数 */

  /* Load the pulse value in the compare register */
  __HAL_LPTIM_COMPARE_SET(hlptim, Pulse);               /* HAL辅助宏/函数 */

  /* Wait for the completion of the write operation to the LPTIM_CMP register */
  if (LPTIM_WaitForFlag(hlptim, LPTIM_FLAG_CMPOK) == HAL_TIMEOUT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_TIMEOUT;                                 /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Start timer in single (one shot) mode */
  __HAL_LPTIM_START_SINGLE(hlptim);                     /* HAL辅助宏/函数 */

  /* Change the TIM state*/
  hlptim->State = HAL_LPTIM_STATE_READY;                /* 赋值或初始化 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Stop the LPTIM One pulse generation.
  * @param  hlptim LPTIM handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_LPTIM_OnePulse_Stop(LPTIM_HandleTypeDef *hlptim)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_LPTIM_INSTANCE(hlptim->Instance));    /* 执行语句 */

  /* Set the LPTIM state */
  hlptim->State = HAL_LPTIM_STATE_BUSY;                 /* 赋值或初始化 */

  /* Disable the Peripheral */
  __HAL_LPTIM_DISABLE(hlptim);                          /* HAL辅助宏/函数 */

  if (HAL_LPTIM_GetState(hlptim) == HAL_LPTIM_STATE_TIMEOUT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_TIMEOUT;                                 /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Change the TIM state*/
  hlptim->State = HAL_LPTIM_STATE_READY;                /* 赋值或初始化 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Start the LPTIM One pulse generation in interrupt mode.
  * @param  hlptim LPTIM handle
  * @param  Period Specifies the Autoreload value.
  *         This parameter must be a value between 0x0000 and 0xFFFF.
  * @param  Pulse Specifies the compare value.
  *         This parameter must be a value between 0x0000 and 0xFFFF.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_LPTIM_OnePulse_Start_IT(LPTIM_HandleTypeDef *hlptim, uint32_t Period, uint32_t Pulse)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_LPTIM_INSTANCE(hlptim->Instance));    /* 执行语句 */
  assert_param(IS_LPTIM_PERIOD(Period));                /* 执行语句 */
  assert_param(IS_LPTIM_PULSE(Pulse));                  /* 执行语句 */

  /* Set the LPTIM state */
  hlptim->State = HAL_LPTIM_STATE_BUSY;                 /* 赋值或初始化 */

  /* Reset WAVE bit to set one pulse mode */
  hlptim->Instance->CFGR &= ~LPTIM_CFGR_WAVE;           /* 赋值或初始化 */

  /* Enable the Peripheral */
  __HAL_LPTIM_ENABLE(hlptim);                           /* HAL辅助宏/函数 */

  /* Clear flag */
  __HAL_LPTIM_CLEAR_FLAG(hlptim, LPTIM_FLAG_ARROK);     /* HAL辅助宏/函数 */

  /* Load the period value in the autoreload register */
  __HAL_LPTIM_AUTORELOAD_SET(hlptim, Period);           /* HAL辅助宏/函数 */

  /* Wait for the completion of the write operation to the LPTIM_ARR register */
  if (LPTIM_WaitForFlag(hlptim, LPTIM_FLAG_ARROK) == HAL_TIMEOUT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_TIMEOUT;                                 /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Clear flag */
  __HAL_LPTIM_CLEAR_FLAG(hlptim, LPTIM_FLAG_CMPOK);     /* HAL辅助宏/函数 */

  /* Load the pulse value in the compare register */
  __HAL_LPTIM_COMPARE_SET(hlptim, Pulse);               /* HAL辅助宏/函数 */

  /* Wait for the completion of the write operation to the LPTIM_CMP register */
  if (LPTIM_WaitForFlag(hlptim, LPTIM_FLAG_CMPOK) == HAL_TIMEOUT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_TIMEOUT;                                 /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Disable the Peripheral */
  __HAL_LPTIM_DISABLE(hlptim);                          /* HAL辅助宏/函数 */

  if (HAL_LPTIM_GetState(hlptim) == HAL_LPTIM_STATE_TIMEOUT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_TIMEOUT;                                 /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Enable Autoreload write complete interrupt */
  __HAL_LPTIM_ENABLE_IT(hlptim, LPTIM_IT_ARROK);        /* HAL辅助宏/函数 */

  /* Enable Compare write complete interrupt */
  __HAL_LPTIM_ENABLE_IT(hlptim, LPTIM_IT_CMPOK);        /* HAL辅助宏/函数 */

  /* Enable Autoreload match interrupt */
  __HAL_LPTIM_ENABLE_IT(hlptim, LPTIM_IT_ARRM);         /* HAL辅助宏/函数 */

  /* Enable Compare match interrupt */
  __HAL_LPTIM_ENABLE_IT(hlptim, LPTIM_IT_CMPM);         /* HAL辅助宏/函数 */

  /* If external trigger source is used, then enable external trigger interrupt */
  if ((hlptim->Init.Trigger.Source) != LPTIM_TRIGSOURCE_SOFTWARE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable external trigger interrupt */
    __HAL_LPTIM_ENABLE_IT(hlptim, LPTIM_IT_EXTTRIG);    /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Enable the Peripheral */
  __HAL_LPTIM_ENABLE(hlptim);                           /* HAL辅助宏/函数 */

  /* Start timer in single (one shot) mode */
  __HAL_LPTIM_START_SINGLE(hlptim);                     /* HAL辅助宏/函数 */

  /* Change the TIM state*/
  hlptim->State = HAL_LPTIM_STATE_READY;                /* 赋值或初始化 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Stop the LPTIM One pulse generation in interrupt mode.
  * @param  hlptim LPTIM handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_LPTIM_OnePulse_Stop_IT(LPTIM_HandleTypeDef *hlptim)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_LPTIM_INSTANCE(hlptim->Instance));    /* 执行语句 */

  /* Set the LPTIM state */
  hlptim->State = HAL_LPTIM_STATE_BUSY;                 /* 赋值或初始化 */

  /* Disable the Peripheral */
  __HAL_LPTIM_DISABLE(hlptim);                          /* HAL辅助宏/函数 */

  if (HAL_LPTIM_GetState(hlptim) == HAL_LPTIM_STATE_TIMEOUT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_TIMEOUT;                                 /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Disable Autoreload write complete interrupt */
  __HAL_LPTIM_DISABLE_IT(hlptim, LPTIM_IT_ARROK);       /* HAL辅助宏/函数 */

  /* Disable Compare write complete interrupt */
  __HAL_LPTIM_DISABLE_IT(hlptim, LPTIM_IT_CMPOK);       /* HAL辅助宏/函数 */

  /* Disable Autoreload match interrupt */
  __HAL_LPTIM_DISABLE_IT(hlptim, LPTIM_IT_ARRM);        /* HAL辅助宏/函数 */

  /* Disable Compare match interrupt */
  __HAL_LPTIM_DISABLE_IT(hlptim, LPTIM_IT_CMPM);        /* HAL辅助宏/函数 */

  /* If external trigger source is used, then disable external trigger interrupt */
  if ((hlptim->Init.Trigger.Source) != LPTIM_TRIGSOURCE_SOFTWARE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable external trigger interrupt */
    __HAL_LPTIM_DISABLE_IT(hlptim, LPTIM_IT_EXTTRIG);   /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Change the TIM state*/
  hlptim->State = HAL_LPTIM_STATE_READY;                /* 赋值或初始化 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Start the LPTIM in Set once mode.
  * @param  hlptim LPTIM handle
  * @param  Period Specifies the Autoreload value.
  *         This parameter must be a value between 0x0000 and 0xFFFF.
  * @param  Pulse Specifies the compare value.
  *         This parameter must be a value between 0x0000 and 0xFFFF.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_LPTIM_SetOnce_Start(LPTIM_HandleTypeDef *hlptim, uint32_t Period, uint32_t Pulse)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_LPTIM_INSTANCE(hlptim->Instance));    /* 执行语句 */
  assert_param(IS_LPTIM_PERIOD(Period));                /* 执行语句 */
  assert_param(IS_LPTIM_PULSE(Pulse));                  /* 执行语句 */

  /* Set the LPTIM state */
  hlptim->State = HAL_LPTIM_STATE_BUSY;                 /* 赋值或初始化 */

  /* Set WAVE bit to enable the set once mode */
  hlptim->Instance->CFGR |= LPTIM_CFGR_WAVE;            /* 赋值或初始化 */

  /* Enable the Peripheral */
  __HAL_LPTIM_ENABLE(hlptim);                           /* HAL辅助宏/函数 */

  /* Clear flag */
  __HAL_LPTIM_CLEAR_FLAG(hlptim, LPTIM_FLAG_ARROK);     /* HAL辅助宏/函数 */

  /* Load the period value in the autoreload register */
  __HAL_LPTIM_AUTORELOAD_SET(hlptim, Period);           /* HAL辅助宏/函数 */

  /* Wait for the completion of the write operation to the LPTIM_ARR register */
  if (LPTIM_WaitForFlag(hlptim, LPTIM_FLAG_ARROK) == HAL_TIMEOUT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_TIMEOUT;                                 /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Clear flag */
  __HAL_LPTIM_CLEAR_FLAG(hlptim, LPTIM_FLAG_CMPOK);     /* HAL辅助宏/函数 */

  /* Load the pulse value in the compare register */
  __HAL_LPTIM_COMPARE_SET(hlptim, Pulse);               /* HAL辅助宏/函数 */

  /* Wait for the completion of the write operation to the LPTIM_CMP register */
  if (LPTIM_WaitForFlag(hlptim, LPTIM_FLAG_CMPOK) == HAL_TIMEOUT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_TIMEOUT;                                 /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Start timer in single (one shot) mode */
  __HAL_LPTIM_START_SINGLE(hlptim);                     /* HAL辅助宏/函数 */

  /* Change the TIM state*/
  hlptim->State = HAL_LPTIM_STATE_READY;                /* 赋值或初始化 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Stop the LPTIM Set once mode.
  * @param  hlptim LPTIM handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_LPTIM_SetOnce_Stop(LPTIM_HandleTypeDef *hlptim)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_LPTIM_INSTANCE(hlptim->Instance));    /* 执行语句 */

  /* Set the LPTIM state */
  hlptim->State = HAL_LPTIM_STATE_BUSY;                 /* 赋值或初始化 */

  /* Disable the Peripheral */
  __HAL_LPTIM_DISABLE(hlptim);                          /* HAL辅助宏/函数 */

  if (HAL_LPTIM_GetState(hlptim) == HAL_LPTIM_STATE_TIMEOUT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_TIMEOUT;                                 /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Change the TIM state*/
  hlptim->State = HAL_LPTIM_STATE_READY;                /* 赋值或初始化 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Start the LPTIM Set once mode in interrupt mode.
  * @param  hlptim LPTIM handle
  * @param  Period Specifies the Autoreload value.
  *         This parameter must be a value between 0x0000 and 0xFFFF.
  * @param  Pulse Specifies the compare value.
  *         This parameter must be a value between 0x0000 and 0xFFFF.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_LPTIM_SetOnce_Start_IT(LPTIM_HandleTypeDef *hlptim, uint32_t Period, uint32_t Pulse)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_LPTIM_INSTANCE(hlptim->Instance));    /* 执行语句 */
  assert_param(IS_LPTIM_PERIOD(Period));                /* 执行语句 */
  assert_param(IS_LPTIM_PULSE(Pulse));                  /* 执行语句 */

  /* Set the LPTIM state */
  hlptim->State = HAL_LPTIM_STATE_BUSY;                 /* 赋值或初始化 */

  /* Set WAVE bit to enable the set once mode */
  hlptim->Instance->CFGR |= LPTIM_CFGR_WAVE;            /* 赋值或初始化 */

  /* Enable the Peripheral */
  __HAL_LPTIM_ENABLE(hlptim);                           /* HAL辅助宏/函数 */

  /* Clear flag */
  __HAL_LPTIM_CLEAR_FLAG(hlptim, LPTIM_FLAG_ARROK);     /* HAL辅助宏/函数 */

  /* Load the period value in the autoreload register */
  __HAL_LPTIM_AUTORELOAD_SET(hlptim, Period);           /* HAL辅助宏/函数 */

  /* Wait for the completion of the write operation to the LPTIM_ARR register */
  if (LPTIM_WaitForFlag(hlptim, LPTIM_FLAG_ARROK) == HAL_TIMEOUT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_TIMEOUT;                                 /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Clear flag */
  __HAL_LPTIM_CLEAR_FLAG(hlptim, LPTIM_FLAG_CMPOK);     /* HAL辅助宏/函数 */

  /* Load the pulse value in the compare register */
  __HAL_LPTIM_COMPARE_SET(hlptim, Pulse);               /* HAL辅助宏/函数 */

  /* Wait for the completion of the write operation to the LPTIM_CMP register */
  if (LPTIM_WaitForFlag(hlptim, LPTIM_FLAG_CMPOK) == HAL_TIMEOUT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_TIMEOUT;                                 /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Disable the Peripheral */
  __HAL_LPTIM_DISABLE(hlptim);                          /* HAL辅助宏/函数 */

  if (HAL_LPTIM_GetState(hlptim) == HAL_LPTIM_STATE_TIMEOUT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_TIMEOUT;                                 /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Enable Autoreload write complete interrupt */
  __HAL_LPTIM_ENABLE_IT(hlptim, LPTIM_IT_ARROK);        /* HAL辅助宏/函数 */

  /* Enable Compare write complete interrupt */
  __HAL_LPTIM_ENABLE_IT(hlptim, LPTIM_IT_CMPOK);        /* HAL辅助宏/函数 */

  /* Enable Autoreload match interrupt */
  __HAL_LPTIM_ENABLE_IT(hlptim, LPTIM_IT_ARRM);         /* HAL辅助宏/函数 */

  /* Enable Compare match interrupt */
  __HAL_LPTIM_ENABLE_IT(hlptim, LPTIM_IT_CMPM);         /* HAL辅助宏/函数 */

  /* If external trigger source is used, then enable external trigger interrupt */
  if ((hlptim->Init.Trigger.Source) != LPTIM_TRIGSOURCE_SOFTWARE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable external trigger interrupt */
    __HAL_LPTIM_ENABLE_IT(hlptim, LPTIM_IT_EXTTRIG);    /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Enable the Peripheral */
  __HAL_LPTIM_ENABLE(hlptim);                           /* HAL辅助宏/函数 */

  /* Start timer in single (one shot) mode */
  __HAL_LPTIM_START_SINGLE(hlptim);                     /* HAL辅助宏/函数 */

  /* Change the TIM state*/
  hlptim->State = HAL_LPTIM_STATE_READY;                /* 赋值或初始化 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Stop the LPTIM Set once mode in interrupt mode.
  * @param  hlptim LPTIM handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_LPTIM_SetOnce_Stop_IT(LPTIM_HandleTypeDef *hlptim)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_LPTIM_INSTANCE(hlptim->Instance));    /* 执行语句 */

  /* Set the LPTIM state */
  hlptim->State = HAL_LPTIM_STATE_BUSY;                 /* 赋值或初始化 */

  /* Disable the Peripheral */
  __HAL_LPTIM_DISABLE(hlptim);                          /* HAL辅助宏/函数 */

  if (HAL_LPTIM_GetState(hlptim) == HAL_LPTIM_STATE_TIMEOUT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_TIMEOUT;                                 /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Disable Autoreload write complete interrupt */
  __HAL_LPTIM_DISABLE_IT(hlptim, LPTIM_IT_ARROK);       /* HAL辅助宏/函数 */

  /* Disable Compare write complete interrupt */
  __HAL_LPTIM_DISABLE_IT(hlptim, LPTIM_IT_CMPOK);       /* HAL辅助宏/函数 */

  /* Disable Autoreload match interrupt */
  __HAL_LPTIM_DISABLE_IT(hlptim, LPTIM_IT_ARRM);        /* HAL辅助宏/函数 */

  /* Disable Compare match interrupt */
  __HAL_LPTIM_DISABLE_IT(hlptim, LPTIM_IT_CMPM);        /* HAL辅助宏/函数 */

  /* If external trigger source is used, then disable external trigger interrupt */
  if ((hlptim->Init.Trigger.Source) != LPTIM_TRIGSOURCE_SOFTWARE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable external trigger interrupt */
    __HAL_LPTIM_DISABLE_IT(hlptim, LPTIM_IT_EXTTRIG);   /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Change the TIM state*/
  hlptim->State = HAL_LPTIM_STATE_READY;                /* 赋值或初始化 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Start the Encoder interface.
  * @param  hlptim LPTIM handle
  * @param  Period Specifies the Autoreload value.
  *         This parameter must be a value between 0x0000 and 0xFFFF.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_LPTIM_Encoder_Start(LPTIM_HandleTypeDef *hlptim, uint32_t Period)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t          tmpcfgr;                            /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_LPTIM_INSTANCE(hlptim->Instance));    /* 执行语句 */
  assert_param(IS_LPTIM_PERIOD(Period));                /* 执行语句 */
  assert_param(hlptim->Init.Clock.Source == LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC);  /* 赋值或初始化 */
  assert_param(hlptim->Init.Clock.Prescaler == LPTIM_PRESCALER_DIV1);  /* 赋值或初始化 */
  assert_param(IS_LPTIM_CLOCK_POLARITY(hlptim->Init.UltraLowPowerClock.Polarity));  /* 执行语句 */

  /* Set the LPTIM state */
  hlptim->State = HAL_LPTIM_STATE_BUSY;                 /* 赋值或初始化 */

  /* Get the LPTIMx CFGR value */
  tmpcfgr = hlptim->Instance->CFGR;                     /* 赋值或初始化 */

  /* Clear CKPOL bits */
  tmpcfgr &= (uint32_t)(~LPTIM_CFGR_CKPOL);             /* 赋值或初始化 */

  /* Set Input polarity */
  tmpcfgr |=  hlptim->Init.UltraLowPowerClock.Polarity;  /* 赋值或初始化 */

  /* Write to LPTIMx CFGR */
  hlptim->Instance->CFGR = tmpcfgr;                     /* 赋值或初始化 */

  /* Set ENC bit to enable the encoder interface */
  hlptim->Instance->CFGR |= LPTIM_CFGR_ENC;             /* 赋值或初始化 */

  /* Enable the Peripheral */
  __HAL_LPTIM_ENABLE(hlptim);                           /* HAL辅助宏/函数 */

  /* Clear flag */
  __HAL_LPTIM_CLEAR_FLAG(hlptim, LPTIM_FLAG_ARROK);     /* HAL辅助宏/函数 */

  /* Load the period value in the autoreload register */
  __HAL_LPTIM_AUTORELOAD_SET(hlptim, Period);           /* HAL辅助宏/函数 */

  /* Wait for the completion of the write operation to the LPTIM_ARR register */
  if (LPTIM_WaitForFlag(hlptim, LPTIM_FLAG_ARROK) == HAL_TIMEOUT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_TIMEOUT;                                 /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Start timer in continuous mode */
  __HAL_LPTIM_START_CONTINUOUS(hlptim);                 /* HAL辅助宏/函数 */

  /* Change the TIM state*/
  hlptim->State = HAL_LPTIM_STATE_READY;                /* 赋值或初始化 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Stop the Encoder interface.
  * @param  hlptim LPTIM handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_LPTIM_Encoder_Stop(LPTIM_HandleTypeDef *hlptim)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_LPTIM_INSTANCE(hlptim->Instance));    /* 执行语句 */

  /* Set the LPTIM state */
  hlptim->State = HAL_LPTIM_STATE_BUSY;                 /* 赋值或初始化 */

  /* Disable the Peripheral */
  __HAL_LPTIM_DISABLE(hlptim);                          /* HAL辅助宏/函数 */

  if (HAL_LPTIM_GetState(hlptim) == HAL_LPTIM_STATE_TIMEOUT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_TIMEOUT;                                 /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Reset ENC bit to disable the encoder interface */
  hlptim->Instance->CFGR &= ~LPTIM_CFGR_ENC;            /* 赋值或初始化 */

  /* Change the TIM state*/
  hlptim->State = HAL_LPTIM_STATE_READY;                /* 赋值或初始化 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Start the Encoder interface in interrupt mode.
  * @param  hlptim LPTIM handle
  * @param  Period Specifies the Autoreload value.
  *         This parameter must be a value between 0x0000 and 0xFFFF.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_LPTIM_Encoder_Start_IT(LPTIM_HandleTypeDef *hlptim, uint32_t Period)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t          tmpcfgr;                            /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_LPTIM_INSTANCE(hlptim->Instance));    /* 执行语句 */
  assert_param(IS_LPTIM_PERIOD(Period));                /* 执行语句 */
  assert_param(hlptim->Init.Clock.Source == LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC);  /* 赋值或初始化 */
  assert_param(hlptim->Init.Clock.Prescaler == LPTIM_PRESCALER_DIV1);  /* 赋值或初始化 */
  assert_param(IS_LPTIM_CLOCK_POLARITY(hlptim->Init.UltraLowPowerClock.Polarity));  /* 执行语句 */

  /* Set the LPTIM state */
  hlptim->State = HAL_LPTIM_STATE_BUSY;                 /* 赋值或初始化 */

  /* Configure edge sensitivity for encoder mode */
  /* Get the LPTIMx CFGR value */
  tmpcfgr = hlptim->Instance->CFGR;                     /* 赋值或初始化 */

  /* Clear CKPOL bits */
  tmpcfgr &= (uint32_t)(~LPTIM_CFGR_CKPOL);             /* 赋值或初始化 */

  /* Set Input polarity */
  tmpcfgr |=  hlptim->Init.UltraLowPowerClock.Polarity;  /* 赋值或初始化 */

  /* Write to LPTIMx CFGR */
  hlptim->Instance->CFGR = tmpcfgr;                     /* 赋值或初始化 */

  /* Set ENC bit to enable the encoder interface */
  hlptim->Instance->CFGR |= LPTIM_CFGR_ENC;             /* 赋值或初始化 */

  /* Enable the Peripheral */
  __HAL_LPTIM_ENABLE(hlptim);                           /* HAL辅助宏/函数 */

  /* Clear flag */
  __HAL_LPTIM_CLEAR_FLAG(hlptim, LPTIM_FLAG_ARROK);     /* HAL辅助宏/函数 */

  /* Load the period value in the autoreload register */
  __HAL_LPTIM_AUTORELOAD_SET(hlptim, Period);           /* HAL辅助宏/函数 */

  /* Wait for the completion of the write operation to the LPTIM_ARR register */
  if (LPTIM_WaitForFlag(hlptim, LPTIM_FLAG_ARROK) == HAL_TIMEOUT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_TIMEOUT;                                 /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Disable the Peripheral */
  __HAL_LPTIM_DISABLE(hlptim);                          /* HAL辅助宏/函数 */

  if (HAL_LPTIM_GetState(hlptim) == HAL_LPTIM_STATE_TIMEOUT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_TIMEOUT;                                 /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Enable "switch to down direction" interrupt */
  __HAL_LPTIM_ENABLE_IT(hlptim, LPTIM_IT_DOWN);         /* HAL辅助宏/函数 */

  /* Enable "switch to up direction" interrupt */
  __HAL_LPTIM_ENABLE_IT(hlptim, LPTIM_IT_UP);           /* HAL辅助宏/函数 */

  /* Enable the Peripheral */
  __HAL_LPTIM_ENABLE(hlptim);                           /* HAL辅助宏/函数 */

  /* Start timer in continuous mode */
  __HAL_LPTIM_START_CONTINUOUS(hlptim);                 /* HAL辅助宏/函数 */

  /* Change the TIM state*/
  hlptim->State = HAL_LPTIM_STATE_READY;                /* 赋值或初始化 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Stop the Encoder interface in interrupt mode.
  * @param  hlptim LPTIM handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_LPTIM_Encoder_Stop_IT(LPTIM_HandleTypeDef *hlptim)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_LPTIM_INSTANCE(hlptim->Instance));    /* 执行语句 */

  /* Set the LPTIM state */
  hlptim->State = HAL_LPTIM_STATE_BUSY;                 /* 赋值或初始化 */

  /* Disable the Peripheral */
  __HAL_LPTIM_DISABLE(hlptim);                          /* HAL辅助宏/函数 */

  if (HAL_LPTIM_GetState(hlptim) == HAL_LPTIM_STATE_TIMEOUT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_TIMEOUT;                                 /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Reset ENC bit to disable the encoder interface */
  hlptim->Instance->CFGR &= ~LPTIM_CFGR_ENC;            /* 赋值或初始化 */

  /* Disable "switch to down direction" interrupt */
  __HAL_LPTIM_DISABLE_IT(hlptim, LPTIM_IT_DOWN);        /* HAL辅助宏/函数 */

  /* Disable "switch to up direction" interrupt */
  __HAL_LPTIM_DISABLE_IT(hlptim, LPTIM_IT_UP);          /* HAL辅助宏/函数 */

  /* Change the TIM state*/
  hlptim->State = HAL_LPTIM_STATE_READY;                /* 赋值或初始化 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Start the Timeout function.
  * @note   The first trigger event will start the timer, any successive
  *         trigger event will reset the counter and the timer restarts.
  * @param  hlptim LPTIM handle
  * @param  Period Specifies the Autoreload value.
  *         This parameter must be a value between 0x0000 and 0xFFFF.
  * @param  Timeout Specifies the TimeOut value to reset the counter.
  *         This parameter must be a value between 0x0000 and 0xFFFF.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_LPTIM_TimeOut_Start(LPTIM_HandleTypeDef *hlptim, uint32_t Period, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_LPTIM_INSTANCE(hlptim->Instance));    /* 执行语句 */
  assert_param(IS_LPTIM_PERIOD(Period));                /* 执行语句 */
  assert_param(IS_LPTIM_PULSE(Timeout));                /* 执行语句 */

  /* Set the LPTIM state */
  hlptim->State = HAL_LPTIM_STATE_BUSY;                 /* 赋值或初始化 */

  /* Set TIMOUT bit to enable the timeout function */
  hlptim->Instance->CFGR |= LPTIM_CFGR_TIMOUT;          /* 赋值或初始化 */

  /* Enable the Peripheral */
  __HAL_LPTIM_ENABLE(hlptim);                           /* HAL辅助宏/函数 */

  /* Clear flag */
  __HAL_LPTIM_CLEAR_FLAG(hlptim, LPTIM_FLAG_ARROK);     /* HAL辅助宏/函数 */

  /* Load the period value in the autoreload register */
  __HAL_LPTIM_AUTORELOAD_SET(hlptim, Period);           /* HAL辅助宏/函数 */

  /* Wait for the completion of the write operation to the LPTIM_ARR register */
  if (LPTIM_WaitForFlag(hlptim, LPTIM_FLAG_ARROK) == HAL_TIMEOUT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_TIMEOUT;                                 /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Clear flag */
  __HAL_LPTIM_CLEAR_FLAG(hlptim, LPTIM_FLAG_CMPOK);     /* HAL辅助宏/函数 */

  /* Load the Timeout value in the compare register */
  __HAL_LPTIM_COMPARE_SET(hlptim, Timeout);             /* HAL辅助宏/函数 */

  /* Wait for the completion of the write operation to the LPTIM_CMP register */
  if (LPTIM_WaitForFlag(hlptim, LPTIM_FLAG_CMPOK) == HAL_TIMEOUT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_TIMEOUT;                                 /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Start timer in continuous mode */
  __HAL_LPTIM_START_CONTINUOUS(hlptim);                 /* HAL辅助宏/函数 */

  /* Change the TIM state*/
  hlptim->State = HAL_LPTIM_STATE_READY;                /* 赋值或初始化 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Stop the Timeout function.
  * @param  hlptim LPTIM handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_LPTIM_TimeOut_Stop(LPTIM_HandleTypeDef *hlptim)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_LPTIM_INSTANCE(hlptim->Instance));    /* 执行语句 */

  /* Set the LPTIM state */
  hlptim->State = HAL_LPTIM_STATE_BUSY;                 /* 赋值或初始化 */

  /* Disable the Peripheral */
  __HAL_LPTIM_DISABLE(hlptim);                          /* HAL辅助宏/函数 */

  if (HAL_LPTIM_GetState(hlptim) == HAL_LPTIM_STATE_TIMEOUT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_TIMEOUT;                                 /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Reset TIMOUT bit to enable the timeout function */
  hlptim->Instance->CFGR &= ~LPTIM_CFGR_TIMOUT;         /* 赋值或初始化 */

  /* Change the TIM state*/
  hlptim->State = HAL_LPTIM_STATE_READY;                /* 赋值或初始化 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Start the Timeout function in interrupt mode.
  * @note   The first trigger event will start the timer, any successive
  *         trigger event will reset the counter and the timer restarts.
  * @param  hlptim LPTIM handle
  * @param  Period Specifies the Autoreload value.
  *         This parameter must be a value between 0x0000 and 0xFFFF.
  * @param  Timeout Specifies the TimeOut value to reset the counter.
  *         This parameter must be a value between 0x0000 and 0xFFFF.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_LPTIM_TimeOut_Start_IT(LPTIM_HandleTypeDef *hlptim, uint32_t Period, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_LPTIM_INSTANCE(hlptim->Instance));    /* 执行语句 */
  assert_param(IS_LPTIM_PERIOD(Period));                /* 执行语句 */
  assert_param(IS_LPTIM_PULSE(Timeout));                /* 执行语句 */

  /* Set the LPTIM state */
  hlptim->State = HAL_LPTIM_STATE_BUSY;                 /* 赋值或初始化 */

  /* Enable EXTI Line interrupt on the LPTIM Wake-up Timer */
  __HAL_LPTIM_WAKEUPTIMER_EXTI_ENABLE_IT();             /* HAL辅助宏/函数 */
#if defined(EXTI_IMR_MR23)                              /* 条件编译 */
  /* Enable rising edge trigger on the LPTIM Wake-up Timer Exti line */
  __HAL_LPTIM_WAKEUPTIMER_EXTI_ENABLE_RISING_EDGE();    /* HAL辅助宏/函数 */
#endif /* EXTI_IMR_MR23 */

  /* Set TIMOUT bit to enable the timeout function */
  hlptim->Instance->CFGR |= LPTIM_CFGR_TIMOUT;          /* 赋值或初始化 */

  /* Enable the Peripheral */
  __HAL_LPTIM_ENABLE(hlptim);                           /* HAL辅助宏/函数 */

  /* Clear flag */
  __HAL_LPTIM_CLEAR_FLAG(hlptim, LPTIM_FLAG_ARROK);     /* HAL辅助宏/函数 */

  /* Load the period value in the autoreload register */
  __HAL_LPTIM_AUTORELOAD_SET(hlptim, Period);           /* HAL辅助宏/函数 */

  /* Wait for the completion of the write operation to the LPTIM_ARR register */
  if (LPTIM_WaitForFlag(hlptim, LPTIM_FLAG_ARROK) == HAL_TIMEOUT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_TIMEOUT;                                 /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Clear flag */
  __HAL_LPTIM_CLEAR_FLAG(hlptim, LPTIM_FLAG_CMPOK);     /* HAL辅助宏/函数 */

  /* Load the Timeout value in the compare register */
  __HAL_LPTIM_COMPARE_SET(hlptim, Timeout);             /* HAL辅助宏/函数 */

  /* Wait for the completion of the write operation to the LPTIM_CMP register */
  if (LPTIM_WaitForFlag(hlptim, LPTIM_FLAG_CMPOK) == HAL_TIMEOUT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_TIMEOUT;                                 /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Disable the Peripheral */
  __HAL_LPTIM_DISABLE(hlptim);                          /* HAL辅助宏/函数 */

  if (HAL_LPTIM_GetState(hlptim) == HAL_LPTIM_STATE_TIMEOUT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_TIMEOUT;                                 /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Enable Compare match interrupt */
  __HAL_LPTIM_ENABLE_IT(hlptim, LPTIM_IT_CMPM);         /* HAL辅助宏/函数 */

  /* Enable the Peripheral */
  __HAL_LPTIM_ENABLE(hlptim);                           /* HAL辅助宏/函数 */

  /* Start timer in continuous mode */
  __HAL_LPTIM_START_CONTINUOUS(hlptim);                 /* HAL辅助宏/函数 */

  /* Change the TIM state*/
  hlptim->State = HAL_LPTIM_STATE_READY;                /* 赋值或初始化 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Stop the Timeout function in interrupt mode.
  * @param  hlptim LPTIM handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_LPTIM_TimeOut_Stop_IT(LPTIM_HandleTypeDef *hlptim)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_LPTIM_INSTANCE(hlptim->Instance));    /* 执行语句 */

  /* Set the LPTIM state */
  hlptim->State = HAL_LPTIM_STATE_BUSY;                 /* 赋值或初始化 */
#if defined(EXTI_IMR_MR23)                              /* 条件编译 */
  /* Disable rising edge trigger on the LPTIM Wake-up Timer Exti line */
  __HAL_LPTIM_WAKEUPTIMER_EXTI_DISABLE_RISING_EDGE();   /* HAL辅助宏/函数 */
#endif /* EXTI_IMR_MR23 */

  /* Disable EXTI Line interrupt on the LPTIM Wake-up Timer */
  __HAL_LPTIM_WAKEUPTIMER_EXTI_DISABLE_IT();            /* HAL辅助宏/函数 */

  /* Disable the Peripheral */
  __HAL_LPTIM_DISABLE(hlptim);                          /* HAL辅助宏/函数 */

  if (HAL_LPTIM_GetState(hlptim) == HAL_LPTIM_STATE_TIMEOUT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_TIMEOUT;                                 /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Reset TIMOUT bit to enable the timeout function */
  hlptim->Instance->CFGR &= ~LPTIM_CFGR_TIMOUT;         /* 赋值或初始化 */

  /* Disable Compare match interrupt */
  __HAL_LPTIM_DISABLE_IT(hlptim, LPTIM_IT_CMPM);        /* HAL辅助宏/函数 */

  /* Change the TIM state*/
  hlptim->State = HAL_LPTIM_STATE_READY;                /* 赋值或初始化 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Start the Counter mode.
  * @param  hlptim LPTIM handle
  * @param  Period Specifies the Autoreload value.
  *         This parameter must be a value between 0x0000 and 0xFFFF.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_LPTIM_Counter_Start(LPTIM_HandleTypeDef *hlptim, uint32_t Period)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_LPTIM_INSTANCE(hlptim->Instance));    /* 执行语句 */
  assert_param(IS_LPTIM_PERIOD(Period));                /* 执行语句 */

  /* Set the LPTIM state */
  hlptim->State = HAL_LPTIM_STATE_BUSY;                 /* 赋值或初始化 */

  /* If clock source is not ULPTIM clock and counter source is external, then it must not be prescaled */
  if ((hlptim->Init.Clock.Source != LPTIM_CLOCKSOURCE_ULPTIM)  /* 条件判断 */
      && (hlptim->Init.CounterSource == LPTIM_COUNTERSOURCE_EXTERNAL))  /* 源代码 */
  {                                                     /* 代码块开始 */
    /* Check if clock is prescaled */
    assert_param(IS_LPTIM_CLOCK_PRESCALERDIV1(hlptim->Init.Clock.Prescaler));  /* 执行语句 */
    /* Set clock prescaler to 0 */
    hlptim->Instance->CFGR &= ~LPTIM_CFGR_PRESC;        /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Enable the Peripheral */
  __HAL_LPTIM_ENABLE(hlptim);                           /* HAL辅助宏/函数 */

  /* Clear flag */
  __HAL_LPTIM_CLEAR_FLAG(hlptim, LPTIM_FLAG_ARROK);     /* HAL辅助宏/函数 */

  /* Load the period value in the autoreload register */
  __HAL_LPTIM_AUTORELOAD_SET(hlptim, Period);           /* HAL辅助宏/函数 */

  /* Wait for the completion of the write operation to the LPTIM_ARR register */
  if (LPTIM_WaitForFlag(hlptim, LPTIM_FLAG_ARROK) == HAL_TIMEOUT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_TIMEOUT;                                 /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Start timer in continuous mode */
  __HAL_LPTIM_START_CONTINUOUS(hlptim);                 /* HAL辅助宏/函数 */

  /* Change the TIM state*/
  hlptim->State = HAL_LPTIM_STATE_READY;                /* 赋值或初始化 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Stop the Counter mode.
  * @param  hlptim LPTIM handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_LPTIM_Counter_Stop(LPTIM_HandleTypeDef *hlptim)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_LPTIM_INSTANCE(hlptim->Instance));    /* 执行语句 */

  /* Set the LPTIM state */
  hlptim->State = HAL_LPTIM_STATE_BUSY;                 /* 赋值或初始化 */

  /* Disable the Peripheral */
  __HAL_LPTIM_DISABLE(hlptim);                          /* HAL辅助宏/函数 */

  if (HAL_LPTIM_GetState(hlptim) == HAL_LPTIM_STATE_TIMEOUT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_TIMEOUT;                                 /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Change the TIM state*/
  hlptim->State = HAL_LPTIM_STATE_READY;                /* 赋值或初始化 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Start the Counter mode in interrupt mode.
  * @param  hlptim LPTIM handle
  * @param  Period Specifies the Autoreload value.
  *         This parameter must be a value between 0x0000 and 0xFFFF.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_LPTIM_Counter_Start_IT(LPTIM_HandleTypeDef *hlptim, uint32_t Period)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_LPTIM_INSTANCE(hlptim->Instance));    /* 执行语句 */
  assert_param(IS_LPTIM_PERIOD(Period));                /* 执行语句 */

  /* Set the LPTIM state */
  hlptim->State = HAL_LPTIM_STATE_BUSY;                 /* 赋值或初始化 */

  /* Enable EXTI Line interrupt on the LPTIM Wake-up Timer */
  __HAL_LPTIM_WAKEUPTIMER_EXTI_ENABLE_IT();             /* HAL辅助宏/函数 */
#if defined(EXTI_IMR_MR23)                              /* 条件编译 */
  /* Enable rising edge trigger on the LPTIM Wake-up Timer Exti line */
  __HAL_LPTIM_WAKEUPTIMER_EXTI_ENABLE_RISING_EDGE();    /* HAL辅助宏/函数 */
#endif /* EXTI_IMR_MR23 */

  /* If clock source is not ULPTIM clock and counter source is external, then it must not be prescaled */
  if ((hlptim->Init.Clock.Source != LPTIM_CLOCKSOURCE_ULPTIM)  /* 条件判断 */
      && (hlptim->Init.CounterSource == LPTIM_COUNTERSOURCE_EXTERNAL))  /* 源代码 */
  {                                                     /* 代码块开始 */
    /* Check if clock is prescaled */
    assert_param(IS_LPTIM_CLOCK_PRESCALERDIV1(hlptim->Init.Clock.Prescaler));  /* 执行语句 */
    /* Set clock prescaler to 0 */
    hlptim->Instance->CFGR &= ~LPTIM_CFGR_PRESC;        /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Enable the Peripheral */
  __HAL_LPTIM_ENABLE(hlptim);                           /* HAL辅助宏/函数 */

  /* Clear flag */
  __HAL_LPTIM_CLEAR_FLAG(hlptim, LPTIM_FLAG_ARROK);     /* HAL辅助宏/函数 */

  /* Load the period value in the autoreload register */
  __HAL_LPTIM_AUTORELOAD_SET(hlptim, Period);           /* HAL辅助宏/函数 */

  /* Wait for the completion of the write operation to the LPTIM_ARR register */
  if (LPTIM_WaitForFlag(hlptim, LPTIM_FLAG_ARROK) == HAL_TIMEOUT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_TIMEOUT;                                 /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Disable the Peripheral */
  __HAL_LPTIM_DISABLE(hlptim);                          /* HAL辅助宏/函数 */

  if (HAL_LPTIM_GetState(hlptim) == HAL_LPTIM_STATE_TIMEOUT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_TIMEOUT;                                 /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Enable Autoreload write complete interrupt */
  __HAL_LPTIM_ENABLE_IT(hlptim, LPTIM_IT_ARROK);        /* HAL辅助宏/函数 */

  /* Enable Autoreload match interrupt */
  __HAL_LPTIM_ENABLE_IT(hlptim, LPTIM_IT_ARRM);         /* HAL辅助宏/函数 */

  /* Enable the Peripheral */
  __HAL_LPTIM_ENABLE(hlptim);                           /* HAL辅助宏/函数 */

  /* Start timer in continuous mode */
  __HAL_LPTIM_START_CONTINUOUS(hlptim);                 /* HAL辅助宏/函数 */

  /* Change the TIM state*/
  hlptim->State = HAL_LPTIM_STATE_READY;                /* 赋值或初始化 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Stop the Counter mode in interrupt mode.
  * @param  hlptim LPTIM handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_LPTIM_Counter_Stop_IT(LPTIM_HandleTypeDef *hlptim)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_LPTIM_INSTANCE(hlptim->Instance));    /* 执行语句 */

  /* Set the LPTIM state */
  hlptim->State = HAL_LPTIM_STATE_BUSY;                 /* 赋值或初始化 */
#if defined(EXTI_IMR_MR23)                              /* 条件编译 */
  /* Disable rising edge trigger on the LPTIM Wake-up Timer Exti line */
  __HAL_LPTIM_WAKEUPTIMER_EXTI_DISABLE_RISING_EDGE();   /* HAL辅助宏/函数 */
#endif /* EXTI_IMR_MR23 */

  /* Disable EXTI Line interrupt on the LPTIM Wake-up Timer */
  __HAL_LPTIM_WAKEUPTIMER_EXTI_DISABLE_IT();            /* HAL辅助宏/函数 */

  /* Disable the Peripheral */
  __HAL_LPTIM_DISABLE(hlptim);                          /* HAL辅助宏/函数 */

  if (HAL_LPTIM_GetState(hlptim) == HAL_LPTIM_STATE_TIMEOUT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_TIMEOUT;                                 /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Disable Autoreload write complete interrupt */
  __HAL_LPTIM_DISABLE_IT(hlptim, LPTIM_IT_ARROK);       /* HAL辅助宏/函数 */

  /* Disable Autoreload match interrupt */
  __HAL_LPTIM_DISABLE_IT(hlptim, LPTIM_IT_ARRM);        /* HAL辅助宏/函数 */
  /* Change the TIM state*/
  hlptim->State = HAL_LPTIM_STATE_READY;                /* 赋值或初始化 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup LPTIM_Exported_Functions_Group3 LPTIM Read operation functions
  *  @brief  Read operation functions.
  *
@verbatim
  ==============================================================================
                  ##### LPTIM Read operation functions #####
  ==============================================================================
[..]  This section provides LPTIM Reading functions.
      (+) Read the counter value.
      (+) Read the period (Auto-reload) value.
      (+) Read the pulse (Compare)value.
@endverbatim
  * @{
  */

/**
  * @brief  Return the current counter value.
  * @param  hlptim LPTIM handle
  * @retval Counter value.
  */
uint32_t HAL_LPTIM_ReadCounter(LPTIM_HandleTypeDef *hlptim)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_LPTIM_INSTANCE(hlptim->Instance));    /* 执行语句 */

  return (hlptim->Instance->CNT);                       /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return the current Autoreload (Period) value.
  * @param  hlptim LPTIM handle
  * @retval Autoreload value.
  */
uint32_t HAL_LPTIM_ReadAutoReload(LPTIM_HandleTypeDef *hlptim)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_LPTIM_INSTANCE(hlptim->Instance));    /* 执行语句 */

  return (hlptim->Instance->ARR);                       /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return the current Compare (Pulse) value.
  * @param  hlptim LPTIM handle
  * @retval Compare value.
  */
uint32_t HAL_LPTIM_ReadCompare(LPTIM_HandleTypeDef *hlptim)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_LPTIM_INSTANCE(hlptim->Instance));    /* 执行语句 */

  return (hlptim->Instance->CMP);                       /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup LPTIM_Exported_Functions_Group4 LPTIM IRQ handler and callbacks
  *  @brief  LPTIM  IRQ handler.
  *
@verbatim
  ==============================================================================
                      ##### LPTIM IRQ handler and callbacks  #####
  ==============================================================================
[..]  This section provides LPTIM IRQ handler and callback functions called within
      the IRQ handler:
   (+) LPTIM interrupt request handler
   (+) Compare match Callback
   (+) Auto-reload match Callback
   (+) External trigger event detection Callback
   (+) Compare register write complete Callback
   (+) Auto-reload register write complete Callback
   (+) Up-counting direction change Callback
   (+) Down-counting direction change Callback

@endverbatim
  * @{
  */

/**
  * @brief  Handle LPTIM interrupt request.
  * @param  hlptim LPTIM handle
  * @retval None
  */
void HAL_LPTIM_IRQHandler(LPTIM_HandleTypeDef *hlptim)  /* 中断服务函数 */
{                                                       /* 代码块开始 */
  /* Compare match interrupt */
  if (__HAL_LPTIM_GET_FLAG(hlptim, LPTIM_FLAG_CMPM) != RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (__HAL_LPTIM_GET_IT_SOURCE(hlptim, LPTIM_IT_CMPM) != RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear Compare match flag */
      __HAL_LPTIM_CLEAR_FLAG(hlptim, LPTIM_FLAG_CMPM);  /* HAL辅助宏/函数 */

      /* Compare match Callback */
#if (USE_HAL_LPTIM_REGISTER_CALLBACKS == 1)             /* 条件编译 */
      hlptim->CompareMatchCallback(hlptim);             /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_LPTIM_CompareMatchCallback(hlptim);           /* HAL库函数调用 */
#endif /* USE_HAL_LPTIM_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Autoreload match interrupt */
  if (__HAL_LPTIM_GET_FLAG(hlptim, LPTIM_FLAG_ARRM) != RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (__HAL_LPTIM_GET_IT_SOURCE(hlptim, LPTIM_IT_ARRM) != RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear Autoreload match flag */
      __HAL_LPTIM_CLEAR_FLAG(hlptim, LPTIM_FLAG_ARRM);  /* HAL辅助宏/函数 */

      /* Autoreload match Callback */
#if (USE_HAL_LPTIM_REGISTER_CALLBACKS == 1)             /* 条件编译 */
      hlptim->AutoReloadMatchCallback(hlptim);          /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_LPTIM_AutoReloadMatchCallback(hlptim);        /* HAL库函数调用 */
#endif /* USE_HAL_LPTIM_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Trigger detected interrupt */
  if (__HAL_LPTIM_GET_FLAG(hlptim, LPTIM_FLAG_EXTTRIG) != RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (__HAL_LPTIM_GET_IT_SOURCE(hlptim, LPTIM_IT_EXTTRIG) != RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear Trigger detected flag */
      __HAL_LPTIM_CLEAR_FLAG(hlptim, LPTIM_FLAG_EXTTRIG);  /* HAL辅助宏/函数 */

      /* Trigger detected callback */
#if (USE_HAL_LPTIM_REGISTER_CALLBACKS == 1)             /* 条件编译 */
      hlptim->TriggerCallback(hlptim);                  /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_LPTIM_TriggerCallback(hlptim);                /* HAL库函数调用 */
#endif /* USE_HAL_LPTIM_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Compare write interrupt */
  if (__HAL_LPTIM_GET_FLAG(hlptim, LPTIM_FLAG_CMPOK) != RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (__HAL_LPTIM_GET_IT_SOURCE(hlptim, LPTIM_IT_CMPOK) != RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear Compare write flag */
      __HAL_LPTIM_CLEAR_FLAG(hlptim, LPTIM_FLAG_CMPOK);  /* HAL辅助宏/函数 */

      /* Compare write Callback */
#if (USE_HAL_LPTIM_REGISTER_CALLBACKS == 1)             /* 条件编译 */
      hlptim->CompareWriteCallback(hlptim);             /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_LPTIM_CompareWriteCallback(hlptim);           /* HAL库函数调用 */
#endif /* USE_HAL_LPTIM_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Autoreload write interrupt */
  if (__HAL_LPTIM_GET_FLAG(hlptim, LPTIM_FLAG_ARROK) != RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (__HAL_LPTIM_GET_IT_SOURCE(hlptim, LPTIM_IT_ARROK) != RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear Autoreload write flag */
      __HAL_LPTIM_CLEAR_FLAG(hlptim, LPTIM_FLAG_ARROK);  /* HAL辅助宏/函数 */

      /* Autoreload write Callback */
#if (USE_HAL_LPTIM_REGISTER_CALLBACKS == 1)             /* 条件编译 */
      hlptim->AutoReloadWriteCallback(hlptim);          /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_LPTIM_AutoReloadWriteCallback(hlptim);        /* HAL库函数调用 */
#endif /* USE_HAL_LPTIM_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Direction counter changed from Down to Up interrupt */
  if (__HAL_LPTIM_GET_FLAG(hlptim, LPTIM_FLAG_UP) != RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (__HAL_LPTIM_GET_IT_SOURCE(hlptim, LPTIM_IT_UP) != RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear Direction counter changed from Down to Up flag */
      __HAL_LPTIM_CLEAR_FLAG(hlptim, LPTIM_FLAG_UP);    /* HAL辅助宏/函数 */

      /* Direction counter changed from Down to Up Callback */
#if (USE_HAL_LPTIM_REGISTER_CALLBACKS == 1)             /* 条件编译 */
      hlptim->DirectionUpCallback(hlptim);              /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_LPTIM_DirectionUpCallback(hlptim);            /* HAL库函数调用 */
#endif /* USE_HAL_LPTIM_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Direction counter changed from Up to Down interrupt */
  if (__HAL_LPTIM_GET_FLAG(hlptim, LPTIM_FLAG_DOWN) != RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (__HAL_LPTIM_GET_IT_SOURCE(hlptim, LPTIM_IT_DOWN) != RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear Direction counter changed from Up to Down flag */
      __HAL_LPTIM_CLEAR_FLAG(hlptim, LPTIM_FLAG_DOWN);  /* HAL辅助宏/函数 */

      /* Direction counter changed from Up to Down Callback */
#if (USE_HAL_LPTIM_REGISTER_CALLBACKS == 1)             /* 条件编译 */
      hlptim->DirectionDownCallback(hlptim);            /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_LPTIM_DirectionDownCallback(hlptim);          /* HAL库函数调用 */
#endif /* USE_HAL_LPTIM_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
#if defined(EXTI_IMR_MR23)                              /* 条件编译 */
  __HAL_LPTIM_WAKEUPTIMER_EXTI_CLEAR_FLAG();            /* HAL辅助宏/函数 */
#endif /* EXTI_IMR_MR23 */
}                                                       /* 代码块结束 */

/**
  * @brief  Compare match callback in non-blocking mode.
  * @param  hlptim LPTIM handle
  * @retval None
  */
__weak void HAL_LPTIM_CompareMatchCallback(LPTIM_HandleTypeDef *hlptim)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hlptim);                                       /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_LPTIM_CompareMatchCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Autoreload match callback in non-blocking mode.
  * @param  hlptim LPTIM handle
  * @retval None
  */
__weak void HAL_LPTIM_AutoReloadMatchCallback(LPTIM_HandleTypeDef *hlptim)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hlptim);                                       /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_LPTIM_AutoReloadMatchCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Trigger detected callback in non-blocking mode.
  * @param  hlptim LPTIM handle
  * @retval None
  */
__weak void HAL_LPTIM_TriggerCallback(LPTIM_HandleTypeDef *hlptim)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hlptim);                                       /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_LPTIM_TriggerCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Compare write callback in non-blocking mode.
  * @param  hlptim LPTIM handle
  * @retval None
  */
__weak void HAL_LPTIM_CompareWriteCallback(LPTIM_HandleTypeDef *hlptim)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hlptim);                                       /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_LPTIM_CompareWriteCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Autoreload write callback in non-blocking mode.
  * @param  hlptim LPTIM handle
  * @retval None
  */
__weak void HAL_LPTIM_AutoReloadWriteCallback(LPTIM_HandleTypeDef *hlptim)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hlptim);                                       /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_LPTIM_AutoReloadWriteCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Direction counter changed from Down to Up callback in non-blocking mode.
  * @param  hlptim LPTIM handle
  * @retval None
  */
__weak void HAL_LPTIM_DirectionUpCallback(LPTIM_HandleTypeDef *hlptim)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hlptim);                                       /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_LPTIM_DirectionUpCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Direction counter changed from Up to Down callback in non-blocking mode.
  * @param  hlptim LPTIM handle
  * @retval None
  */
__weak void HAL_LPTIM_DirectionDownCallback(LPTIM_HandleTypeDef *hlptim)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hlptim);                                       /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_LPTIM_DirectionDownCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

#if (USE_HAL_LPTIM_REGISTER_CALLBACKS == 1)             /* 条件编译 */
/**
  * @brief  Register a User LPTIM callback to be used instead of the weak predefined callback
  * @param hlptim LPTIM handle
  * @param CallbackID ID of the callback to be registered
  *        This parameter can be one of the following values:
  *          @arg @ref HAL_LPTIM_MSPINIT_CB_ID          LPTIM Base Msp Init Callback ID
  *          @arg @ref HAL_LPTIM_MSPDEINIT_CB_ID        LPTIM Base Msp DeInit Callback ID
  *          @arg @ref HAL_LPTIM_COMPARE_MATCH_CB_ID    Compare match Callback ID
  *          @arg @ref HAL_LPTIM_AUTORELOAD_MATCH_CB_ID Auto-reload match Callback ID
  *          @arg @ref HAL_LPTIM_TRIGGER_CB_ID          External trigger event detection Callback ID
  *          @arg @ref HAL_LPTIM_COMPARE_WRITE_CB_ID    Compare register write complete Callback ID
  *          @arg @ref HAL_LPTIM_AUTORELOAD_WRITE_CB_ID Auto-reload register write complete Callback ID
  *          @arg @ref HAL_LPTIM_DIRECTION_UP_CB_ID     Up-counting direction change Callback ID
  *          @arg @ref HAL_LPTIM_DIRECTION_DOWN_CB_ID   Down-counting direction change Callback ID
  * @param pCallback pointer to the callback function
  * @retval status
  */
HAL_StatusTypeDef HAL_LPTIM_RegisterCallback(LPTIM_HandleTypeDef        *hlptim,  /* HAL状态类型函数 */
                                             HAL_LPTIM_CallbackIDTypeDef CallbackID,  /* HAL库函数调用 */
                                             pLPTIM_CallbackTypeDef      pCallback)  /* 源代码 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  if (pCallback == NULL)                                /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Process locked */
  __HAL_LOCK(hlptim);                                   /* HAL辅助宏/函数 */

  if (hlptim->State == HAL_LPTIM_STATE_READY)           /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_LPTIM_MSPINIT_CB_ID :                    /* case标签 */
        hlptim->MspInitCallback = pCallback;            /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_LPTIM_MSPDEINIT_CB_ID :                  /* case标签 */
        hlptim->MspDeInitCallback = pCallback;          /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_LPTIM_COMPARE_MATCH_CB_ID :              /* case标签 */
        hlptim->CompareMatchCallback = pCallback;       /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_LPTIM_AUTORELOAD_MATCH_CB_ID :           /* case标签 */
        hlptim->AutoReloadMatchCallback = pCallback;    /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_LPTIM_TRIGGER_CB_ID :                    /* case标签 */
        hlptim->TriggerCallback = pCallback;            /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_LPTIM_COMPARE_WRITE_CB_ID :              /* case标签 */
        hlptim->CompareWriteCallback = pCallback;       /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_LPTIM_AUTORELOAD_WRITE_CB_ID :           /* case标签 */
        hlptim->AutoReloadWriteCallback = pCallback;    /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_LPTIM_DIRECTION_UP_CB_ID :               /* case标签 */
        hlptim->DirectionUpCallback = pCallback;        /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_LPTIM_DIRECTION_DOWN_CB_ID :             /* case标签 */
        hlptim->DirectionDownCallback = pCallback;      /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (hlptim->State == HAL_LPTIM_STATE_RESET)      /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_LPTIM_MSPINIT_CB_ID :                    /* case标签 */
        hlptim->MspInitCallback = pCallback;            /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_LPTIM_MSPDEINIT_CB_ID :                  /* case标签 */
        hlptim->MspDeInitCallback = pCallback;          /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hlptim);                                 /* HAL辅助宏/函数 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Unregister a LPTIM callback
  *         LLPTIM callback is redirected to the weak predefined callback
  * @param hlptim LPTIM handle
  * @param CallbackID ID of the callback to be unregistered
  *        This parameter can be one of the following values:
  *          @arg @ref HAL_LPTIM_MSPINIT_CB_ID          LPTIM Base Msp Init Callback ID
  *          @arg @ref HAL_LPTIM_MSPDEINIT_CB_ID        LPTIM Base Msp DeInit Callback ID
  *          @arg @ref HAL_LPTIM_COMPARE_MATCH_CB_ID    Compare match Callback ID
  *          @arg @ref HAL_LPTIM_AUTORELOAD_MATCH_CB_ID Auto-reload match Callback ID
  *          @arg @ref HAL_LPTIM_TRIGGER_CB_ID          External trigger event detection Callback ID
  *          @arg @ref HAL_LPTIM_COMPARE_WRITE_CB_ID    Compare register write complete Callback ID
  *          @arg @ref HAL_LPTIM_AUTORELOAD_WRITE_CB_ID Auto-reload register write complete Callback ID
  *          @arg @ref HAL_LPTIM_DIRECTION_UP_CB_ID     Up-counting direction change Callback ID
  *          @arg @ref HAL_LPTIM_DIRECTION_DOWN_CB_ID   Down-counting direction change Callback ID
  * @retval status
  */
HAL_StatusTypeDef HAL_LPTIM_UnRegisterCallback(LPTIM_HandleTypeDef        *hlptim,  /* HAL状态类型函数 */
                                               HAL_LPTIM_CallbackIDTypeDef CallbackID)  /* HAL库函数调用 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Process locked */
  __HAL_LOCK(hlptim);                                   /* HAL辅助宏/函数 */

  if (hlptim->State == HAL_LPTIM_STATE_READY)           /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_LPTIM_MSPINIT_CB_ID :                    /* case标签 */
        hlptim->MspInitCallback = HAL_LPTIM_MspInit;                          /* Legacy weak MspInit Callback */
        break;                                          /* 跳出 */

      case HAL_LPTIM_MSPDEINIT_CB_ID :                  /* case标签 */
        hlptim->MspDeInitCallback = HAL_LPTIM_MspDeInit;                       /* Legacy weak Msp DeInit Callback */
        break;                                          /* 跳出 */

      case HAL_LPTIM_COMPARE_MATCH_CB_ID :              /* case标签 */
        hlptim->CompareMatchCallback = HAL_LPTIM_CompareMatchCallback;         /* Legacy weak Compare match Callback */
        break;                                          /* 跳出 */

      case HAL_LPTIM_AUTORELOAD_MATCH_CB_ID :           /* case标签 */
        hlptim->AutoReloadMatchCallback = HAL_LPTIM_AutoReloadMatchCallback;   /* Legacy weak Auto-reload match Callback */
        break;                                          /* 跳出 */

      case HAL_LPTIM_TRIGGER_CB_ID :                    /* case标签 */
        hlptim->TriggerCallback = HAL_LPTIM_TriggerCallback;                   /* Legacy weak External trigger event detection Callback */
        break;                                          /* 跳出 */

      case HAL_LPTIM_COMPARE_WRITE_CB_ID :              /* case标签 */
        hlptim->CompareWriteCallback = HAL_LPTIM_CompareWriteCallback;         /* Legacy weak Compare register write complete Callback */
        break;                                          /* 跳出 */

      case HAL_LPTIM_AUTORELOAD_WRITE_CB_ID :           /* case标签 */
        hlptim->AutoReloadWriteCallback = HAL_LPTIM_AutoReloadWriteCallback;   /* Legacy weak Auto-reload register write complete Callback */
        break;                                          /* 跳出 */

      case HAL_LPTIM_DIRECTION_UP_CB_ID :               /* case标签 */
        hlptim->DirectionUpCallback = HAL_LPTIM_DirectionUpCallback;           /* Legacy weak Up-counting direction change Callback */
        break;                                          /* 跳出 */

      case HAL_LPTIM_DIRECTION_DOWN_CB_ID :             /* case标签 */
        hlptim->DirectionDownCallback = HAL_LPTIM_DirectionDownCallback;       /* Legacy weak Down-counting direction change Callback */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (hlptim->State == HAL_LPTIM_STATE_RESET)      /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_LPTIM_MSPINIT_CB_ID :                    /* case标签 */
        hlptim->MspInitCallback = HAL_LPTIM_MspInit;                           /* Legacy weak MspInit Callback */
        break;                                          /* 跳出 */

      case HAL_LPTIM_MSPDEINIT_CB_ID :                  /* case标签 */
        hlptim->MspDeInitCallback = HAL_LPTIM_MspDeInit;                        /* Legacy weak Msp DeInit Callback */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hlptim);                                 /* HAL辅助宏/函数 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* USE_HAL_LPTIM_REGISTER_CALLBACKS */

/**
  * @}
  */

/** @defgroup LPTIM_Group5 Peripheral State functions
  *  @brief   Peripheral State functions.
  *
@verbatim
  ==============================================================================
                      ##### Peripheral State functions #####
  ==============================================================================
    [..]
    This subsection permits to get in run-time the status of the peripheral.

@endverbatim
  * @{
  */

/**
  * @brief  Return the LPTIM handle state.
  * @param  hlptim LPTIM handle
  * @retval HAL state
  */
HAL_LPTIM_StateTypeDef HAL_LPTIM_GetState(LPTIM_HandleTypeDef *hlptim)  /* HAL库函数调用 */
{                                                       /* 代码块开始 */
  /* Return LPTIM handle state */
  return hlptim->State;                                 /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */


/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/

/** @defgroup LPTIM_Private_Functions LPTIM Private Functions
  * @{
  */
#if (USE_HAL_LPTIM_REGISTER_CALLBACKS == 1)             /* 条件编译 */
/**
  * @brief  Reset interrupt callbacks to the legacy weak callbacks.
  * @param  lptim pointer to a LPTIM_HandleTypeDef structure that contains
  *                the configuration information for LPTIM module.
  * @retval None
  */
static void LPTIM_ResetCallback(LPTIM_HandleTypeDef *lptim)  /* 静态函数 */
{                                                       /* 代码块开始 */
  /* Reset the LPTIM callback to the legacy weak callbacks */
  lptim->CompareMatchCallback    = HAL_LPTIM_CompareMatchCallback;    /* Compare match Callback                       */
  lptim->AutoReloadMatchCallback = HAL_LPTIM_AutoReloadMatchCallback; /* Auto-reload match Callback                   */
  lptim->TriggerCallback         = HAL_LPTIM_TriggerCallback;         /* External trigger event detection Callback    */
  lptim->CompareWriteCallback    = HAL_LPTIM_CompareWriteCallback;    /* Compare register write complete Callback     */
  lptim->AutoReloadWriteCallback = HAL_LPTIM_AutoReloadWriteCallback; /* Auto-reload register write complete Callback */
  lptim->DirectionUpCallback     = HAL_LPTIM_DirectionUpCallback;     /* Up-counting direction change Callback        */
  lptim->DirectionDownCallback   = HAL_LPTIM_DirectionDownCallback;   /* Down-counting direction change Callback      */
}                                                       /* 代码块结束 */
#endif /* USE_HAL_LPTIM_REGISTER_CALLBACKS */

/**
  * @brief  LPTimer Wait for flag set
  * @param  hlptim pointer to a LPTIM_HandleTypeDef structure that contains
  *                the configuration information for LPTIM module.
  * @param  flag   The lptim flag
  * @retval HAL status
  */
static HAL_StatusTypeDef LPTIM_WaitForFlag(LPTIM_HandleTypeDef *hlptim, uint32_t flag)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef result = HAL_OK;                    /* HAL状态类型函数 */
  uint32_t count = TIMEOUT * (SystemCoreClock / 20UL / 1000UL);  /* 32位无符号变量/函数 */
  do                                                    /* do-while循环 */
  {                                                     /* 代码块开始 */
    count--;                                            /* 执行语句 */
    if (count == 0UL)                                   /* 条件判断 */
    {                                                   /* 代码块开始 */
      result = HAL_TIMEOUT;                             /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  } while ((!(__HAL_LPTIM_GET_FLAG((hlptim), (flag)))) && (count != 0UL));  /* 代码块结束 */

  return result;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Disable LPTIM HW instance.
  * @param  hlptim pointer to a LPTIM_HandleTypeDef structure that contains
  *                the configuration information for LPTIM module.
  * @note   The following sequence is required to solve LPTIM disable HW limitation.
  *         Please check Errata Sheet ES0335 for more details under "MCU may remain
  *         stuck in LPTIM interrupt when entering Stop mode" section.
  * @retval None
  */
void LPTIM_Disable(LPTIM_HandleTypeDef *hlptim)         /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  uint32_t tmpclksource = 0;                            /* 32位无符号变量/函数 */
  uint32_t tmpIER;                                      /* 32位无符号变量/函数 */
  uint32_t tmpCFGR;                                     /* 32位无符号变量/函数 */
  uint32_t tmpCMP;                                      /* 32位无符号变量/函数 */
  uint32_t tmpARR;                                      /* 32位无符号变量/函数 */
  uint32_t tmpOR;                                       /* 32位无符号变量/函数 */

  __disable_irq();                                      /* 执行语句 */

  /*********** Save LPTIM Config ***********/
  /* Save LPTIM source clock */
  switch ((uint32_t)hlptim->Instance)                   /* switch分支 */
  {                                                     /* 代码块开始 */
    case LPTIM1_BASE:                                   /* case标签 */
      tmpclksource = __HAL_RCC_GET_LPTIM1_SOURCE();     /* 赋值或初始化 */
      break;                                            /* 跳出 */
    default:                                            /* default分支 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  /* Save LPTIM configuration registers */
  tmpIER = hlptim->Instance->IER;                       /* 赋值或初始化 */
  tmpCFGR = hlptim->Instance->CFGR;                     /* 赋值或初始化 */
  tmpCMP = hlptim->Instance->CMP;                       /* 赋值或初始化 */
  tmpARR = hlptim->Instance->ARR;                       /* 赋值或初始化 */
  tmpOR = hlptim->Instance->OR;                         /* 赋值或初始化 */

  /*********** Reset LPTIM ***********/
  switch ((uint32_t)hlptim->Instance)                   /* switch分支 */
  {                                                     /* 代码块开始 */
    case LPTIM1_BASE:                                   /* case标签 */
      __HAL_RCC_LPTIM1_FORCE_RESET();                   /* HAL辅助宏/函数 */
      __HAL_RCC_LPTIM1_RELEASE_RESET();                 /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    default:                                            /* default分支 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  /*********** Restore LPTIM Config ***********/
  if ((tmpCMP != 0UL) || (tmpARR != 0UL))               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Force LPTIM source kernel clock from APB */
    switch ((uint32_t)hlptim->Instance)                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case LPTIM1_BASE:                                 /* case标签 */
        __HAL_RCC_LPTIM1_CONFIG(RCC_LPTIM1CLKSOURCE_PCLK1);  /* HAL辅助宏/函数 */
        break;                                          /* 跳出 */
      default:                                          /* default分支 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */

    if (tmpCMP != 0UL)                                  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Restore CMP register (LPTIM should be enabled first) */
      hlptim->Instance->CR |= LPTIM_CR_ENABLE;          /* 赋值或初始化 */
      hlptim->Instance->CMP = tmpCMP;                   /* 赋值或初始化 */

      /* Wait for the completion of the write operation to the LPTIM_CMP register */
      if (LPTIM_WaitForFlag(hlptim, LPTIM_FLAG_CMPOK) == HAL_TIMEOUT)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        hlptim->State = HAL_LPTIM_STATE_TIMEOUT;        /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      __HAL_LPTIM_CLEAR_FLAG(hlptim, LPTIM_FLAG_CMPOK);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    if (tmpARR != 0UL)                                  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Restore ARR register (LPTIM should be enabled first) */
      hlptim->Instance->CR |= LPTIM_CR_ENABLE;          /* 赋值或初始化 */
      hlptim->Instance->ARR = tmpARR;                   /* 赋值或初始化 */

      /* Wait for the completion of the write operation to the LPTIM_ARR register */
      if (LPTIM_WaitForFlag(hlptim, LPTIM_FLAG_ARROK) == HAL_TIMEOUT)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        hlptim->State = HAL_LPTIM_STATE_TIMEOUT;        /* 赋值或初始化 */
      }                                                 /* 代码块结束 */

      __HAL_LPTIM_CLEAR_FLAG(hlptim, LPTIM_FLAG_ARROK);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Restore LPTIM source kernel clock */
    switch ((uint32_t)hlptim->Instance)                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case LPTIM1_BASE:                                 /* case标签 */
        __HAL_RCC_LPTIM1_CONFIG(tmpclksource);          /* HAL辅助宏/函数 */
        break;                                          /* 跳出 */
      default:                                          /* default分支 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Restore configuration registers (LPTIM should be disabled first) */
  hlptim->Instance->CR &= ~(LPTIM_CR_ENABLE);           /* 赋值或初始化 */
  hlptim->Instance->IER = tmpIER;                       /* 赋值或初始化 */
  hlptim->Instance->CFGR = tmpCFGR;                     /* 赋值或初始化 */
  hlptim->Instance->OR = tmpOR;                         /* 赋值或初始化 */

  __enable_irq();                                       /* 执行语句 */
}                                                       /* 代码块结束 */
/**
  * @}
  */
#endif /* LPTIM1 */

#endif /* HAL_LPTIM_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
