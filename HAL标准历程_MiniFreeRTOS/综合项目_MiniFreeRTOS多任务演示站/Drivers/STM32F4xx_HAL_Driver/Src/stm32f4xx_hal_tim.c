/* 文件: stm32f4xx_hal_tim.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_tim.c
  * @author  MCD Application Team
  * @brief   TIM HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Timer (TIM) peripheral:
  *           + TIM Time Base Initialization
  *           + TIM Time Base Start
  *           + TIM Time Base Start Interruption
  *           + TIM Time Base Start DMA
  *           + TIM Output Compare/PWM Initialization
  *           + TIM Output Compare/PWM Channel Configuration
  *           + TIM Output Compare/PWM  Start
  *           + TIM Output Compare/PWM  Start Interruption
  *           + TIM Output Compare/PWM Start DMA
  *           + TIM Input Capture Initialization
  *           + TIM Input Capture Channel Configuration
  *           + TIM Input Capture Start
  *           + TIM Input Capture Start Interruption
  *           + TIM Input Capture Start DMA
  *           + TIM One Pulse Initialization
  *           + TIM One Pulse Channel Configuration
  *           + TIM One Pulse Start
  *           + TIM Encoder Interface Initialization
  *           + TIM Encoder Interface Start
  *           + TIM Encoder Interface Start Interruption
  *           + TIM Encoder Interface Start DMA
  *           + Commutation Event configuration with Interruption and DMA
  *           + TIM OCRef clear configuration
  *           + TIM External Clock configuration
  @verbatim
  ==============================================================================
                      ##### TIMER Generic features #####
  ==============================================================================
  [..] The Timer features include:
       (#) 16-bit up, down, up/down auto-reload counter.
       (#) 16-bit programmable prescaler allowing dividing (also on the fly) the
           counter clock frequency either by any factor between 1 and 65536.
       (#) Up to 4 independent channels for:
           (++) Input Capture
           (++) Output Compare
           (++) PWM generation (Edge and Center-aligned Mode)
           (++) One-pulse mode output
       (#) Synchronization circuit to control the timer with external signals and to interconnect
            several timers together.
       (#) Supports incremental encoder for positioning purposes

            ##### How to use this driver #####
  ==============================================================================
    [..]
     (#) Initialize the TIM low level resources by implementing the following functions
         depending on the selected feature:
           (++) Time Base : HAL_TIM_Base_MspInit()
           (++) Input Capture : HAL_TIM_IC_MspInit()
           (++) Output Compare : HAL_TIM_OC_MspInit()
           (++) PWM generation : HAL_TIM_PWM_MspInit()
           (++) One-pulse mode output : HAL_TIM_OnePulse_MspInit()
           (++) Encoder mode output : HAL_TIM_Encoder_MspInit()

     (#) Initialize the TIM low level resources :
        (##) Enable the TIM interface clock using __HAL_RCC_TIMx_CLK_ENABLE();
        (##) TIM pins configuration
            (+++) Enable the clock for the TIM GPIOs using the following function:
             __HAL_RCC_GPIOx_CLK_ENABLE();
            (+++) Configure these TIM pins in Alternate function mode using HAL_GPIO_Init();

     (#) The external Clock can be configured, if needed (the default clock is the
         internal clock from the APBx), using the following function:
         HAL_TIM_ConfigClockSource, the clock configuration should be done before
         any start function.

     (#) Configure the TIM in the desired functioning mode using one of the
       Initialization function of this driver:
       (++) HAL_TIM_Base_Init: to use the Timer to generate a simple time base
       (++) HAL_TIM_OC_Init and HAL_TIM_OC_ConfigChannel: to use the Timer to generate an
            Output Compare signal.
       (++) HAL_TIM_PWM_Init and HAL_TIM_PWM_ConfigChannel: to use the Timer to generate a
            PWM signal.
       (++) HAL_TIM_IC_Init and HAL_TIM_IC_ConfigChannel: to use the Timer to measure an
            external signal.
       (++) HAL_TIM_OnePulse_Init and HAL_TIM_OnePulse_ConfigChannel: to use the Timer
            in One Pulse Mode.
       (++) HAL_TIM_Encoder_Init: to use the Timer Encoder Interface.

     (#) Activate the TIM peripheral using one of the start functions depending from the feature used:
           (++) Time Base : HAL_TIM_Base_Start(), HAL_TIM_Base_Start_DMA(), HAL_TIM_Base_Start_IT()
           (++) Input Capture :  HAL_TIM_IC_Start(), HAL_TIM_IC_Start_DMA(), HAL_TIM_IC_Start_IT()
           (++) Output Compare : HAL_TIM_OC_Start(), HAL_TIM_OC_Start_DMA(), HAL_TIM_OC_Start_IT()
           (++) PWM generation : HAL_TIM_PWM_Start(), HAL_TIM_PWM_Start_DMA(), HAL_TIM_PWM_Start_IT()
           (++) One-pulse mode output : HAL_TIM_OnePulse_Start(), HAL_TIM_OnePulse_Start_IT()
           (++) Encoder mode output : HAL_TIM_Encoder_Start(), HAL_TIM_Encoder_Start_DMA(), HAL_TIM_Encoder_Start_IT().

     (#) The DMA Burst is managed with the two following functions:
         HAL_TIM_DMABurst_WriteStart()
         HAL_TIM_DMABurst_ReadStart()

    *** Callback registration ***
  =============================================

  [..]
  The compilation define  USE_HAL_TIM_REGISTER_CALLBACKS when set to 1
  allows the user to configure dynamically the driver callbacks.

  [..]
  Use Function @ref HAL_TIM_RegisterCallback() to register a callback.
  @ref HAL_TIM_RegisterCallback() takes as parameters the HAL peripheral handle,
  the Callback ID and a pointer to the user callback function.

  [..]
  Use function @ref HAL_TIM_UnRegisterCallback() to reset a callback to the default
  weak function.
  @ref HAL_TIM_UnRegisterCallback takes as parameters the HAL peripheral handle,
  and the Callback ID.

  [..]
  These functions allow to register/unregister following callbacks:
    (+) Base_MspInitCallback              : TIM Base Msp Init Callback.
    (+) Base_MspDeInitCallback            : TIM Base Msp DeInit Callback.
    (+) IC_MspInitCallback                : TIM IC Msp Init Callback.
    (+) IC_MspDeInitCallback              : TIM IC Msp DeInit Callback.
    (+) OC_MspInitCallback                : TIM OC Msp Init Callback.
    (+) OC_MspDeInitCallback              : TIM OC Msp DeInit Callback.
    (+) PWM_MspInitCallback               : TIM PWM Msp Init Callback.
    (+) PWM_MspDeInitCallback             : TIM PWM Msp DeInit Callback.
    (+) OnePulse_MspInitCallback          : TIM One Pulse Msp Init Callback.
    (+) OnePulse_MspDeInitCallback        : TIM One Pulse Msp DeInit Callback.
    (+) Encoder_MspInitCallback           : TIM Encoder Msp Init Callback.
    (+) Encoder_MspDeInitCallback         : TIM Encoder Msp DeInit Callback.
    (+) HallSensor_MspInitCallback        : TIM Hall Sensor Msp Init Callback.
    (+) HallSensor_MspDeInitCallback      : TIM Hall Sensor Msp DeInit Callback.
    (+) PeriodElapsedCallback             : TIM Period Elapsed Callback.
    (+) PeriodElapsedHalfCpltCallback     : TIM Period Elapsed half complete Callback.
    (+) TriggerCallback                   : TIM Trigger Callback.
    (+) TriggerHalfCpltCallback           : TIM Trigger half complete Callback.
    (+) IC_CaptureCallback                : TIM Input Capture Callback.
    (+) IC_CaptureHalfCpltCallback        : TIM Input Capture half complete Callback.
    (+) OC_DelayElapsedCallback           : TIM Output Compare Delay Elapsed Callback.
    (+) PWM_PulseFinishedCallback         : TIM PWM Pulse Finished Callback.
    (+) PWM_PulseFinishedHalfCpltCallback : TIM PWM Pulse Finished half complete Callback.
    (+) ErrorCallback                     : TIM Error Callback.
    (+) CommutationCallback               : TIM Commutation Callback.
    (+) CommutationHalfCpltCallback       : TIM Commutation half complete Callback.
    (+) BreakCallback                     : TIM Break Callback.

  [..]
By default, after the Init and when the state is HAL_TIM_STATE_RESET
all interrupt callbacks are set to the corresponding weak functions:
  examples @ref HAL_TIM_TriggerCallback(), @ref HAL_TIM_ErrorCallback().

  [..]
  Exception done for MspInit and MspDeInit functions that are reset to the legacy weak
  functionalities in the Init / DeInit only when these callbacks are null
  (not registered beforehand). If not, MspInit or MspDeInit are not null, the Init / DeInit
    keep and use the user MspInit / MspDeInit callbacks(registered beforehand)

  [..]
    Callbacks can be registered / unregistered in HAL_TIM_STATE_READY state only.
    Exception done MspInit / MspDeInit that can be registered / unregistered
    in HAL_TIM_STATE_READY or HAL_TIM_STATE_RESET state,
    thus registered(user) MspInit / DeInit callbacks can be used during the Init / DeInit.
  In that case first register the MspInit/MspDeInit user callbacks
      using @ref HAL_TIM_RegisterCallback() before calling DeInit or Init function.

  [..]
      When The compilation define USE_HAL_TIM_REGISTER_CALLBACKS is set to 0 or
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

/** @defgroup TIM TIM
  * @brief TIM HAL module driver
  * @{
  */

#ifdef HAL_TIM_MODULE_ENABLED                           /* 条件编译 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/** @addtogroup TIM_Private_Functions
  * @{
  */
static void TIM_OC1_SetConfig(TIM_TypeDef *TIMx, TIM_OC_InitTypeDef *OC_Config);  /* 静态函数 */
static void TIM_OC3_SetConfig(TIM_TypeDef *TIMx, TIM_OC_InitTypeDef *OC_Config);  /* 静态函数 */
static void TIM_OC4_SetConfig(TIM_TypeDef *TIMx, TIM_OC_InitTypeDef *OC_Config);  /* 静态函数 */
static void TIM_TI1_ConfigInputStage(TIM_TypeDef *TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICFilter);  /* 静态函数 */
static void TIM_TI2_SetConfig(TIM_TypeDef *TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICSelection,  /* 静态函数 */
                              uint32_t TIM_ICFilter);   /* 32位无符号变量/函数 */
static void TIM_TI2_ConfigInputStage(TIM_TypeDef *TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICFilter);  /* 静态函数 */
static void TIM_TI3_SetConfig(TIM_TypeDef *TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICSelection,  /* 静态函数 */
                              uint32_t TIM_ICFilter);   /* 32位无符号变量/函数 */
static void TIM_TI4_SetConfig(TIM_TypeDef *TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICSelection,  /* 静态函数 */
                              uint32_t TIM_ICFilter);   /* 32位无符号变量/函数 */
static void TIM_ITRx_SetConfig(TIM_TypeDef *TIMx, uint32_t InputTriggerSource);  /* 静态函数 */
static void TIM_DMAPeriodElapsedCplt(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void TIM_DMAPeriodElapsedHalfCplt(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void TIM_DMADelayPulseCplt(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void TIM_DMATriggerCplt(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void TIM_DMATriggerHalfCplt(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static HAL_StatusTypeDef TIM_SlaveTimer_SetConfig(TIM_HandleTypeDef *htim,  /* 静态变量/函数 */
                                                  TIM_SlaveConfigTypeDef *sSlaveConfig);  /* 执行语句 */
/**
  * @}
  */
/* Exported functions --------------------------------------------------------*/

/** @defgroup TIM_Exported_Functions TIM Exported Functions
  * @{
  */

/** @defgroup TIM_Exported_Functions_Group1 TIM Time Base functions
  *  @brief    Time Base functions
  *
@verbatim
  ==============================================================================
              ##### Time Base functions #####
  ==============================================================================
  [..]
    This section provides functions allowing to:
    (+) Initialize and configure the TIM base.
    (+) De-initialize the TIM base.
    (+) Start the Time Base.
    (+) Stop the Time Base.
    (+) Start the Time Base and enable interrupt.
    (+) Stop the Time Base and disable interrupt.
    (+) Start the Time Base and enable DMA transfer.
    (+) Stop the Time Base and disable DMA transfer.

@endverbatim
  * @{
  */
/**
  * @brief  Initializes the TIM Time base Unit according to the specified
  *         parameters in the TIM_HandleTypeDef and initialize the associated handle.
  * @note   Switching from Center Aligned counter mode to Edge counter mode (or reverse)
  *         requires a timer reset to avoid unexpected direction
  *         due to DIR bit readonly in center aligned mode.
  *         Ex: call @ref HAL_TIM_Base_DeInit() before HAL_TIM_Base_Init()
  * @param  htim TIM Base handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_Base_Init(TIM_HandleTypeDef *htim)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the TIM handle allocation */
  if (htim == NULL)                                     /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the parameters */
  assert_param(IS_TIM_INSTANCE(htim->Instance));        /* 执行语句 */
  assert_param(IS_TIM_COUNTER_MODE(htim->Init.CounterMode));  /* 执行语句 */
  assert_param(IS_TIM_CLOCKDIVISION_DIV(htim->Init.ClockDivision));  /* 执行语句 */
  assert_param(IS_TIM_AUTORELOAD_PRELOAD(htim->Init.AutoReloadPreload));  /* 执行语句 */

  if (htim->State == HAL_TIM_STATE_RESET)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Allocate lock resource and initialize it */
    htim->Lock = HAL_UNLOCKED;                          /* 赋值或初始化 */

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)               /* 条件编译 */
    /* Reset interrupt callbacks to legacy weak callbacks */
    TIM_ResetCallback(htim);                            /* 执行语句 */

    if (htim->Base_MspInitCallback == NULL)             /* 条件判断 */
    {                                                   /* 代码块开始 */
      htim->Base_MspInitCallback = HAL_TIM_Base_MspInit;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    /* Init the low level hardware : GPIO, CLOCK, NVIC */
    htim->Base_MspInitCallback(htim);                   /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /* Init the low level hardware : GPIO, CLOCK, NVIC */
    HAL_TIM_Base_MspInit(htim);                         /* HAL 定时器操作 */
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */

  /* Set the TIM state */
  htim->State = HAL_TIM_STATE_BUSY;                     /* 赋值或初始化 */

  /* Set the Time Base configuration */
  TIM_Base_SetConfig(htim->Instance, &htim->Init);      /* 执行语句 */

  /* Initialize the DMA burst operation state */
  htim->DMABurstState = HAL_DMA_BURST_STATE_READY;      /* 赋值或初始化 */

  /* Initialize the TIM channels state */
  TIM_CHANNEL_STATE_SET_ALL(htim, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
  TIM_CHANNEL_N_STATE_SET_ALL(htim, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */

  /* Initialize the TIM state*/
  htim->State = HAL_TIM_STATE_READY;                    /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  DeInitializes the TIM Base peripheral
  * @param  htim TIM Base handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_Base_DeInit(TIM_HandleTypeDef *htim)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_INSTANCE(htim->Instance));        /* 执行语句 */

  htim->State = HAL_TIM_STATE_BUSY;                     /* 赋值或初始化 */

  /* Disable the TIM Peripheral Clock */
  __HAL_TIM_DISABLE(htim);                              /* HAL辅助宏/函数 */

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)               /* 条件编译 */
  if (htim->Base_MspDeInitCallback == NULL)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    htim->Base_MspDeInitCallback = HAL_TIM_Base_MspDeInit;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  /* DeInit the low level hardware */
  htim->Base_MspDeInitCallback(htim);                   /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* DeInit the low level hardware: GPIO, CLOCK, NVIC */
  HAL_TIM_Base_MspDeInit(htim);                         /* HAL 定时器操作 */
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */

  /* Change the DMA burst operation state */
  htim->DMABurstState = HAL_DMA_BURST_STATE_RESET;      /* 赋值或初始化 */

  /* Change the TIM channels state */
  TIM_CHANNEL_STATE_SET_ALL(htim, HAL_TIM_CHANNEL_STATE_RESET);  /* 执行语句 */
  TIM_CHANNEL_N_STATE_SET_ALL(htim, HAL_TIM_CHANNEL_STATE_RESET);  /* 执行语句 */

  /* Change TIM state */
  htim->State = HAL_TIM_STATE_RESET;                    /* 赋值或初始化 */

  /* Release Lock */
  __HAL_UNLOCK(htim);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initializes the TIM Base MSP.
  * @param  htim TIM Base handle
  * @retval None
  */
__weak void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_Base_MspInit could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  DeInitializes TIM Base MSP.
  * @param  htim TIM Base handle
  * @retval None
  */
__weak void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *htim)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_Base_MspDeInit could be implemented in the user file
   */
}                                                       /* 代码块结束 */


/**
  * @brief  Starts the TIM Base generation.
  * @param  htim TIM Base handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_Base_Start(TIM_HandleTypeDef *htim)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpsmcr;                                     /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_TIM_INSTANCE(htim->Instance));        /* 执行语句 */

  /* Check the TIM state */
  if (htim->State != HAL_TIM_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Set the TIM state */
  htim->State = HAL_TIM_STATE_BUSY;                     /* 赋值或初始化 */

  /* Enable the Peripheral, except in trigger mode where enable is automatically done with trigger */
  if (IS_TIM_SLAVE_INSTANCE(htim->Instance))            /* 条件判断 */
  {                                                     /* 代码块开始 */
    tmpsmcr = htim->Instance->SMCR & TIM_SMCR_SMS;      /* 赋值或初始化 */
    if (!IS_TIM_SLAVEMODE_TRIGGER_ENABLED(tmpsmcr))     /* 条件判断 */
    {                                                   /* 代码块开始 */
      __HAL_TIM_ENABLE(htim);                           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    __HAL_TIM_ENABLE(htim);                             /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Stops the TIM Base generation.
  * @param  htim TIM Base handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_Base_Stop(TIM_HandleTypeDef *htim)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_INSTANCE(htim->Instance));        /* 执行语句 */

  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);                              /* HAL辅助宏/函数 */

  /* Set the TIM state */
  htim->State = HAL_TIM_STATE_READY;                    /* 赋值或初始化 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Starts the TIM Base generation in interrupt mode.
  * @param  htim TIM Base handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_Base_Start_IT(TIM_HandleTypeDef *htim)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpsmcr;                                     /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_TIM_INSTANCE(htim->Instance));        /* 执行语句 */

  /* Check the TIM state */
  if (htim->State != HAL_TIM_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Set the TIM state */
  htim->State = HAL_TIM_STATE_BUSY;                     /* 赋值或初始化 */

  /* Enable the TIM Update interrupt */
  __HAL_TIM_ENABLE_IT(htim, TIM_IT_UPDATE);             /* HAL辅助宏/函数 */

  /* Enable the Peripheral, except in trigger mode where enable is automatically done with trigger */
  if (IS_TIM_SLAVE_INSTANCE(htim->Instance))            /* 条件判断 */
  {                                                     /* 代码块开始 */
    tmpsmcr = htim->Instance->SMCR & TIM_SMCR_SMS;      /* 赋值或初始化 */
    if (!IS_TIM_SLAVEMODE_TRIGGER_ENABLED(tmpsmcr))     /* 条件判断 */
    {                                                   /* 代码块开始 */
      __HAL_TIM_ENABLE(htim);                           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    __HAL_TIM_ENABLE(htim);                             /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Stops the TIM Base generation in interrupt mode.
  * @param  htim TIM Base handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_Base_Stop_IT(TIM_HandleTypeDef *htim)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_INSTANCE(htim->Instance));        /* 执行语句 */

  /* Disable the TIM Update interrupt */
  __HAL_TIM_DISABLE_IT(htim, TIM_IT_UPDATE);            /* HAL辅助宏/函数 */

  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);                              /* HAL辅助宏/函数 */

  /* Set the TIM state */
  htim->State = HAL_TIM_STATE_READY;                    /* 赋值或初始化 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Starts the TIM Base generation in DMA mode.
  * @param  htim TIM Base handle
  * @param  pData The source Buffer address.
  * @param  Length The length of data to be transferred from memory to peripheral.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_Base_Start_DMA(TIM_HandleTypeDef *htim, uint32_t *pData, uint16_t Length)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpsmcr;                                     /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_TIM_DMA_INSTANCE(htim->Instance));    /* 执行语句 */

  /* Set the TIM state */
  if (htim->State == HAL_TIM_STATE_BUSY)                /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
  else if (htim->State == HAL_TIM_STATE_READY)          /* 否则如果 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) && (Length > 0U))               /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      htim->State = HAL_TIM_STATE_BUSY;                 /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Set the DMA Period elapsed callbacks */
  htim->hdma[TIM_DMA_ID_UPDATE]->XferCpltCallback = TIM_DMAPeriodElapsedCplt;  /* 赋值或初始化 */
  htim->hdma[TIM_DMA_ID_UPDATE]->XferHalfCpltCallback = TIM_DMAPeriodElapsedHalfCplt;  /* 赋值或初始化 */

  /* Set the DMA error callback */
  htim->hdma[TIM_DMA_ID_UPDATE]->XferErrorCallback = TIM_DMAError ;  /* 赋值或初始化 */

  /* Enable the DMA stream */
  if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_UPDATE], (uint32_t)pData, (uint32_t)&htim->Instance->ARR, Length) != HAL_OK)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Return error status */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Enable the TIM Update DMA request */
  __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_UPDATE);           /* HAL辅助宏/函数 */

  /* Enable the Peripheral, except in trigger mode where enable is automatically done with trigger */
  if (IS_TIM_SLAVE_INSTANCE(htim->Instance))            /* 条件判断 */
  {                                                     /* 代码块开始 */
    tmpsmcr = htim->Instance->SMCR & TIM_SMCR_SMS;      /* 赋值或初始化 */
    if (!IS_TIM_SLAVEMODE_TRIGGER_ENABLED(tmpsmcr))     /* 条件判断 */
    {                                                   /* 代码块开始 */
      __HAL_TIM_ENABLE(htim);                           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    __HAL_TIM_ENABLE(htim);                             /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Stops the TIM Base generation in DMA mode.
  * @param  htim TIM Base handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_Base_Stop_DMA(TIM_HandleTypeDef *htim)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_DMA_INSTANCE(htim->Instance));    /* 执行语句 */

  /* Disable the TIM Update DMA request */
  __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_UPDATE);          /* HAL辅助宏/函数 */

  (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_UPDATE]);  /* 执行语句 */

  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);                              /* HAL辅助宏/函数 */

  /* Set the TIM state */
  htim->State = HAL_TIM_STATE_READY;                    /* 赋值或初始化 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup TIM_Exported_Functions_Group2 TIM Output Compare functions
  *  @brief    TIM Output Compare functions
  *
@verbatim
  ==============================================================================
                  ##### TIM Output Compare functions #####
  ==============================================================================
  [..]
    This section provides functions allowing to:
    (+) Initialize and configure the TIM Output Compare.
    (+) De-initialize the TIM Output Compare.
    (+) Start the TIM Output Compare.
    (+) Stop the TIM Output Compare.
    (+) Start the TIM Output Compare and enable interrupt.
    (+) Stop the TIM Output Compare and disable interrupt.
    (+) Start the TIM Output Compare and enable DMA transfer.
    (+) Stop the TIM Output Compare and disable DMA transfer.

@endverbatim
  * @{
  */
/**
  * @brief  Initializes the TIM Output Compare according to the specified
  *         parameters in the TIM_HandleTypeDef and initializes the associated handle.
  * @note   Switching from Center Aligned counter mode to Edge counter mode (or reverse)
  *         requires a timer reset to avoid unexpected direction
  *         due to DIR bit readonly in center aligned mode.
  *         Ex: call @ref HAL_TIM_OC_DeInit() before HAL_TIM_OC_Init()
  * @param  htim TIM Output Compare handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_OC_Init(TIM_HandleTypeDef *htim)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the TIM handle allocation */
  if (htim == NULL)                                     /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the parameters */
  assert_param(IS_TIM_INSTANCE(htim->Instance));        /* 执行语句 */
  assert_param(IS_TIM_COUNTER_MODE(htim->Init.CounterMode));  /* 执行语句 */
  assert_param(IS_TIM_CLOCKDIVISION_DIV(htim->Init.ClockDivision));  /* 执行语句 */
  assert_param(IS_TIM_AUTORELOAD_PRELOAD(htim->Init.AutoReloadPreload));  /* 执行语句 */

  if (htim->State == HAL_TIM_STATE_RESET)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Allocate lock resource and initialize it */
    htim->Lock = HAL_UNLOCKED;                          /* 赋值或初始化 */

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)               /* 条件编译 */
    /* Reset interrupt callbacks to legacy weak callbacks */
    TIM_ResetCallback(htim);                            /* 执行语句 */

    if (htim->OC_MspInitCallback == NULL)               /* 条件判断 */
    {                                                   /* 代码块开始 */
      htim->OC_MspInitCallback = HAL_TIM_OC_MspInit;    /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    /* Init the low level hardware : GPIO, CLOCK, NVIC */
    htim->OC_MspInitCallback(htim);                     /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /* Init the low level hardware : GPIO, CLOCK, NVIC and DMA */
    HAL_TIM_OC_MspInit(htim);                           /* HAL 定时器操作 */
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */

  /* Set the TIM state */
  htim->State = HAL_TIM_STATE_BUSY;                     /* 赋值或初始化 */

  /* Init the base time for the Output Compare */
  TIM_Base_SetConfig(htim->Instance,  &htim->Init);     /* 执行语句 */

  /* Initialize the DMA burst operation state */
  htim->DMABurstState = HAL_DMA_BURST_STATE_READY;      /* 赋值或初始化 */

  /* Initialize the TIM channels state */
  TIM_CHANNEL_STATE_SET_ALL(htim, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
  TIM_CHANNEL_N_STATE_SET_ALL(htim, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */

  /* Initialize the TIM state*/
  htim->State = HAL_TIM_STATE_READY;                    /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  DeInitializes the TIM peripheral
  * @param  htim TIM Output Compare handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_OC_DeInit(TIM_HandleTypeDef *htim)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_INSTANCE(htim->Instance));        /* 执行语句 */

  htim->State = HAL_TIM_STATE_BUSY;                     /* 赋值或初始化 */

  /* Disable the TIM Peripheral Clock */
  __HAL_TIM_DISABLE(htim);                              /* HAL辅助宏/函数 */

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)               /* 条件编译 */
  if (htim->OC_MspDeInitCallback == NULL)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    htim->OC_MspDeInitCallback = HAL_TIM_OC_MspDeInit;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  /* DeInit the low level hardware */
  htim->OC_MspDeInitCallback(htim);                     /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* DeInit the low level hardware: GPIO, CLOCK, NVIC and DMA */
  HAL_TIM_OC_MspDeInit(htim);                           /* HAL 定时器操作 */
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */

  /* Change the DMA burst operation state */
  htim->DMABurstState = HAL_DMA_BURST_STATE_RESET;      /* 赋值或初始化 */

  /* Change the TIM channels state */
  TIM_CHANNEL_STATE_SET_ALL(htim, HAL_TIM_CHANNEL_STATE_RESET);  /* 执行语句 */
  TIM_CHANNEL_N_STATE_SET_ALL(htim, HAL_TIM_CHANNEL_STATE_RESET);  /* 执行语句 */

  /* Change TIM state */
  htim->State = HAL_TIM_STATE_RESET;                    /* 赋值或初始化 */

  /* Release Lock */
  __HAL_UNLOCK(htim);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initializes the TIM Output Compare MSP.
  * @param  htim TIM Output Compare handle
  * @retval None
  */
__weak void HAL_TIM_OC_MspInit(TIM_HandleTypeDef *htim)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_OC_MspInit could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  DeInitializes TIM Output Compare MSP.
  * @param  htim TIM Output Compare handle
  * @retval None
  */
__weak void HAL_TIM_OC_MspDeInit(TIM_HandleTypeDef *htim)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_OC_MspDeInit could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Starts the TIM Output Compare signal generation.
  * @param  htim TIM Output Compare handle
  * @param  Channel TIM Channel to be enabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_OC_Start(TIM_HandleTypeDef *htim, uint32_t Channel)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpsmcr;                                     /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));  /* 执行语句 */

  /* Check the TIM channel state */
  if (TIM_CHANNEL_STATE_GET(htim, Channel) != HAL_TIM_CHANNEL_STATE_READY)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Set the TIM channel state */
  TIM_CHANNEL_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */

  /* Enable the Output compare channel */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_ENABLE);  /* 执行语句 */

  if (IS_TIM_BREAK_INSTANCE(htim->Instance) != RESET)   /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable the main output */
    __HAL_TIM_MOE_ENABLE(htim);                         /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Enable the Peripheral, except in trigger mode where enable is automatically done with trigger */
  if (IS_TIM_SLAVE_INSTANCE(htim->Instance))            /* 条件判断 */
  {                                                     /* 代码块开始 */
    tmpsmcr = htim->Instance->SMCR & TIM_SMCR_SMS;      /* 赋值或初始化 */
    if (!IS_TIM_SLAVEMODE_TRIGGER_ENABLED(tmpsmcr))     /* 条件判断 */
    {                                                   /* 代码块开始 */
      __HAL_TIM_ENABLE(htim);                           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    __HAL_TIM_ENABLE(htim);                             /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Stops the TIM Output Compare signal generation.
  * @param  htim TIM Output Compare handle
  * @param  Channel TIM Channel to be disabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_OC_Stop(TIM_HandleTypeDef *htim, uint32_t Channel)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));  /* 执行语句 */

  /* Disable the Output compare channel */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_DISABLE);  /* 执行语句 */

  if (IS_TIM_BREAK_INSTANCE(htim->Instance) != RESET)   /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable the Main Output */
    __HAL_TIM_MOE_DISABLE(htim);                        /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);                              /* HAL辅助宏/函数 */

  /* Set the TIM channel state */
  TIM_CHANNEL_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Starts the TIM Output Compare signal generation in interrupt mode.
  * @param  htim TIM Output Compare handle
  * @param  Channel TIM Channel to be enabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_OC_Start_IT(TIM_HandleTypeDef *htim, uint32_t Channel)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpsmcr;                                     /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));  /* 执行语句 */

  /* Check the TIM channel state */
  if (TIM_CHANNEL_STATE_GET(htim, Channel) != HAL_TIM_CHANNEL_STATE_READY)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Set the TIM channel state */
  TIM_CHANNEL_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */

  switch (Channel)                                      /* switch分支 */
  {                                                     /* 代码块开始 */
    case TIM_CHANNEL_1:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Enable the TIM Capture/Compare 1 interrupt */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC1);            /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_2:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Enable the TIM Capture/Compare 2 interrupt */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC2);            /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_3:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Enable the TIM Capture/Compare 3 interrupt */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC3);            /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_4:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Enable the TIM Capture/Compare 4 interrupt */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC4);            /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    default:                                            /* default分支 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  /* Enable the Output compare channel */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_ENABLE);  /* 执行语句 */

  if (IS_TIM_BREAK_INSTANCE(htim->Instance) != RESET)   /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable the main output */
    __HAL_TIM_MOE_ENABLE(htim);                         /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Enable the Peripheral, except in trigger mode where enable is automatically done with trigger */
  if (IS_TIM_SLAVE_INSTANCE(htim->Instance))            /* 条件判断 */
  {                                                     /* 代码块开始 */
    tmpsmcr = htim->Instance->SMCR & TIM_SMCR_SMS;      /* 赋值或初始化 */
    if (!IS_TIM_SLAVEMODE_TRIGGER_ENABLED(tmpsmcr))     /* 条件判断 */
    {                                                   /* 代码块开始 */
      __HAL_TIM_ENABLE(htim);                           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    __HAL_TIM_ENABLE(htim);                             /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Stops the TIM Output Compare signal generation in interrupt mode.
  * @param  htim TIM Output Compare handle
  * @param  Channel TIM Channel to be disabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_OC_Stop_IT(TIM_HandleTypeDef *htim, uint32_t Channel)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));  /* 执行语句 */

  switch (Channel)                                      /* switch分支 */
  {                                                     /* 代码块开始 */
    case TIM_CHANNEL_1:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Disable the TIM Capture/Compare 1 interrupt */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC1);           /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_2:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Disable the TIM Capture/Compare 2 interrupt */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC2);           /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_3:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Disable the TIM Capture/Compare 3 interrupt */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC3);           /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_4:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Disable the TIM Capture/Compare 4 interrupt */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC4);           /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    default:                                            /* default分支 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  /* Disable the Output compare channel */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_DISABLE);  /* 执行语句 */

  if (IS_TIM_BREAK_INSTANCE(htim->Instance) != RESET)   /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable the Main Output */
    __HAL_TIM_MOE_DISABLE(htim);                        /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);                              /* HAL辅助宏/函数 */

  /* Set the TIM channel state */
  TIM_CHANNEL_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Starts the TIM Output Compare signal generation in DMA mode.
  * @param  htim TIM Output Compare handle
  * @param  Channel TIM Channel to be enabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @param  pData The source Buffer address.
  * @param  Length The length of data to be transferred from memory to TIM peripheral
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_OC_Start_DMA(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t *pData, uint16_t Length)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpsmcr;                                     /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));  /* 执行语句 */

  /* Set the TIM channel state */
  if (TIM_CHANNEL_STATE_GET(htim, Channel) == HAL_TIM_CHANNEL_STATE_BUSY)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
  else if (TIM_CHANNEL_STATE_GET(htim, Channel) == HAL_TIM_CHANNEL_STATE_READY)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) && (Length > 0U))               /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      TIM_CHANNEL_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  switch (Channel)                                      /* switch分支 */
  {                                                     /* 代码块开始 */
    case TIM_CHANNEL_1:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Set the DMA compare callbacks */
      htim->hdma[TIM_DMA_ID_CC1]->XferCpltCallback = TIM_DMADelayPulseCplt;  /* 赋值或初始化 */
      htim->hdma[TIM_DMA_ID_CC1]->XferHalfCpltCallback = TIM_DMADelayPulseHalfCplt;  /* 赋值或初始化 */

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC1]->XferErrorCallback = TIM_DMAError ;  /* 赋值或初始化 */

      /* Enable the DMA stream */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC1], (uint32_t)pData, (uint32_t)&htim->Instance->CCR1, Length) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Return error status */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */

      /* Enable the TIM Capture/Compare 1 DMA request */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC1);          /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_2:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Set the DMA compare callbacks */
      htim->hdma[TIM_DMA_ID_CC2]->XferCpltCallback = TIM_DMADelayPulseCplt;  /* 赋值或初始化 */
      htim->hdma[TIM_DMA_ID_CC2]->XferHalfCpltCallback = TIM_DMADelayPulseHalfCplt;  /* 赋值或初始化 */

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC2]->XferErrorCallback = TIM_DMAError ;  /* 赋值或初始化 */

      /* Enable the DMA stream */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC2], (uint32_t)pData, (uint32_t)&htim->Instance->CCR2, Length) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Return error status */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */

      /* Enable the TIM Capture/Compare 2 DMA request */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC2);          /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_3:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Set the DMA compare callbacks */
      htim->hdma[TIM_DMA_ID_CC3]->XferCpltCallback = TIM_DMADelayPulseCplt;  /* 赋值或初始化 */
      htim->hdma[TIM_DMA_ID_CC3]->XferHalfCpltCallback = TIM_DMADelayPulseHalfCplt;  /* 赋值或初始化 */

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC3]->XferErrorCallback = TIM_DMAError ;  /* 赋值或初始化 */

      /* Enable the DMA stream */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC3], (uint32_t)pData, (uint32_t)&htim->Instance->CCR3, Length) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Return error status */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
      /* Enable the TIM Capture/Compare 3 DMA request */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC3);          /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_4:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Set the DMA compare callbacks */
      htim->hdma[TIM_DMA_ID_CC4]->XferCpltCallback = TIM_DMADelayPulseCplt;  /* 赋值或初始化 */
      htim->hdma[TIM_DMA_ID_CC4]->XferHalfCpltCallback = TIM_DMADelayPulseHalfCplt;  /* 赋值或初始化 */

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC4]->XferErrorCallback = TIM_DMAError ;  /* 赋值或初始化 */

      /* Enable the DMA stream */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC4], (uint32_t)pData, (uint32_t)&htim->Instance->CCR4, Length) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Return error status */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
      /* Enable the TIM Capture/Compare 4 DMA request */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC4);          /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    default:                                            /* default分支 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  /* Enable the Output compare channel */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_ENABLE);  /* 执行语句 */

  if (IS_TIM_BREAK_INSTANCE(htim->Instance) != RESET)   /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable the main output */
    __HAL_TIM_MOE_ENABLE(htim);                         /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Enable the Peripheral, except in trigger mode where enable is automatically done with trigger */
  if (IS_TIM_SLAVE_INSTANCE(htim->Instance))            /* 条件判断 */
  {                                                     /* 代码块开始 */
    tmpsmcr = htim->Instance->SMCR & TIM_SMCR_SMS;      /* 赋值或初始化 */
    if (!IS_TIM_SLAVEMODE_TRIGGER_ENABLED(tmpsmcr))     /* 条件判断 */
    {                                                   /* 代码块开始 */
      __HAL_TIM_ENABLE(htim);                           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    __HAL_TIM_ENABLE(htim);                             /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Stops the TIM Output Compare signal generation in DMA mode.
  * @param  htim TIM Output Compare handle
  * @param  Channel TIM Channel to be disabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_OC_Stop_DMA(TIM_HandleTypeDef *htim, uint32_t Channel)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));  /* 执行语句 */

  switch (Channel)                                      /* switch分支 */
  {                                                     /* 代码块开始 */
    case TIM_CHANNEL_1:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Disable the TIM Capture/Compare 1 DMA request */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC1);         /* HAL辅助宏/函数 */
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC1]);  /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_2:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Disable the TIM Capture/Compare 2 DMA request */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC2);         /* HAL辅助宏/函数 */
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC2]);  /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_3:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Disable the TIM Capture/Compare 3 DMA request */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC3);         /* HAL辅助宏/函数 */
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC3]);  /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_4:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Disable the TIM Capture/Compare 4 interrupt */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC4);         /* HAL辅助宏/函数 */
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC4]);  /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    default:                                            /* default分支 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  /* Disable the Output compare channel */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_DISABLE);  /* 执行语句 */

  if (IS_TIM_BREAK_INSTANCE(htim->Instance) != RESET)   /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable the Main Output */
    __HAL_TIM_MOE_DISABLE(htim);                        /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);                              /* HAL辅助宏/函数 */

  /* Set the TIM channel state */
  TIM_CHANNEL_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup TIM_Exported_Functions_Group3 TIM PWM functions
  *  @brief    TIM PWM functions
  *
@verbatim
  ==============================================================================
                          ##### TIM PWM functions #####
  ==============================================================================
  [..]
    This section provides functions allowing to:
    (+) Initialize and configure the TIM PWM.
    (+) De-initialize the TIM PWM.
    (+) Start the TIM PWM.
    (+) Stop the TIM PWM.
    (+) Start the TIM PWM and enable interrupt.
    (+) Stop the TIM PWM and disable interrupt.
    (+) Start the TIM PWM and enable DMA transfer.
    (+) Stop the TIM PWM and disable DMA transfer.

@endverbatim
  * @{
  */
/**
  * @brief  Initializes the TIM PWM Time Base according to the specified
  *         parameters in the TIM_HandleTypeDef and initializes the associated handle.
  * @note   Switching from Center Aligned counter mode to Edge counter mode (or reverse)
  *         requires a timer reset to avoid unexpected direction
  *         due to DIR bit readonly in center aligned mode.
  *         Ex: call @ref HAL_TIM_PWM_DeInit() before HAL_TIM_PWM_Init()
  * @param  htim TIM PWM handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_PWM_Init(TIM_HandleTypeDef *htim)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the TIM handle allocation */
  if (htim == NULL)                                     /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the parameters */
  assert_param(IS_TIM_INSTANCE(htim->Instance));        /* 执行语句 */
  assert_param(IS_TIM_COUNTER_MODE(htim->Init.CounterMode));  /* 执行语句 */
  assert_param(IS_TIM_CLOCKDIVISION_DIV(htim->Init.ClockDivision));  /* 执行语句 */
  assert_param(IS_TIM_AUTORELOAD_PRELOAD(htim->Init.AutoReloadPreload));  /* 执行语句 */

  if (htim->State == HAL_TIM_STATE_RESET)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Allocate lock resource and initialize it */
    htim->Lock = HAL_UNLOCKED;                          /* 赋值或初始化 */

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)               /* 条件编译 */
    /* Reset interrupt callbacks to legacy weak callbacks */
    TIM_ResetCallback(htim);                            /* 执行语句 */

    if (htim->PWM_MspInitCallback == NULL)              /* 条件判断 */
    {                                                   /* 代码块开始 */
      htim->PWM_MspInitCallback = HAL_TIM_PWM_MspInit;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    /* Init the low level hardware : GPIO, CLOCK, NVIC */
    htim->PWM_MspInitCallback(htim);                    /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /* Init the low level hardware : GPIO, CLOCK, NVIC and DMA */
    HAL_TIM_PWM_MspInit(htim);                          /* HAL 定时器操作 */
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */

  /* Set the TIM state */
  htim->State = HAL_TIM_STATE_BUSY;                     /* 赋值或初始化 */

  /* Init the base time for the PWM */
  TIM_Base_SetConfig(htim->Instance, &htim->Init);      /* 执行语句 */

  /* Initialize the DMA burst operation state */
  htim->DMABurstState = HAL_DMA_BURST_STATE_READY;      /* 赋值或初始化 */

  /* Initialize the TIM channels state */
  TIM_CHANNEL_STATE_SET_ALL(htim, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
  TIM_CHANNEL_N_STATE_SET_ALL(htim, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */

  /* Initialize the TIM state*/
  htim->State = HAL_TIM_STATE_READY;                    /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  DeInitializes the TIM peripheral
  * @param  htim TIM PWM handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_PWM_DeInit(TIM_HandleTypeDef *htim)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_INSTANCE(htim->Instance));        /* 执行语句 */

  htim->State = HAL_TIM_STATE_BUSY;                     /* 赋值或初始化 */

  /* Disable the TIM Peripheral Clock */
  __HAL_TIM_DISABLE(htim);                              /* HAL辅助宏/函数 */

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)               /* 条件编译 */
  if (htim->PWM_MspDeInitCallback == NULL)              /* 条件判断 */
  {                                                     /* 代码块开始 */
    htim->PWM_MspDeInitCallback = HAL_TIM_PWM_MspDeInit;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  /* DeInit the low level hardware */
  htim->PWM_MspDeInitCallback(htim);                    /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* DeInit the low level hardware: GPIO, CLOCK, NVIC and DMA */
  HAL_TIM_PWM_MspDeInit(htim);                          /* HAL 定时器操作 */
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */

  /* Change the DMA burst operation state */
  htim->DMABurstState = HAL_DMA_BURST_STATE_RESET;      /* 赋值或初始化 */

  /* Change the TIM channels state */
  TIM_CHANNEL_STATE_SET_ALL(htim, HAL_TIM_CHANNEL_STATE_RESET);  /* 执行语句 */
  TIM_CHANNEL_N_STATE_SET_ALL(htim, HAL_TIM_CHANNEL_STATE_RESET);  /* 执行语句 */

  /* Change TIM state */
  htim->State = HAL_TIM_STATE_RESET;                    /* 赋值或初始化 */

  /* Release Lock */
  __HAL_UNLOCK(htim);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initializes the TIM PWM MSP.
  * @param  htim TIM PWM handle
  * @retval None
  */
__weak void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_PWM_MspInit could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  DeInitializes TIM PWM MSP.
  * @param  htim TIM PWM handle
  * @retval None
  */
__weak void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef *htim)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_PWM_MspDeInit could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Starts the PWM signal generation.
  * @param  htim TIM handle
  * @param  Channel TIM Channels to be enabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_PWM_Start(TIM_HandleTypeDef *htim, uint32_t Channel)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpsmcr;                                     /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));  /* 执行语句 */

  /* Check the TIM channel state */
  if (TIM_CHANNEL_STATE_GET(htim, Channel) != HAL_TIM_CHANNEL_STATE_READY)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Set the TIM channel state */
  TIM_CHANNEL_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */

  /* Enable the Capture compare channel */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_ENABLE);  /* 执行语句 */

  if (IS_TIM_BREAK_INSTANCE(htim->Instance) != RESET)   /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable the main output */
    __HAL_TIM_MOE_ENABLE(htim);                         /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Enable the Peripheral, except in trigger mode where enable is automatically done with trigger */
  if (IS_TIM_SLAVE_INSTANCE(htim->Instance))            /* 条件判断 */
  {                                                     /* 代码块开始 */
    tmpsmcr = htim->Instance->SMCR & TIM_SMCR_SMS;      /* 赋值或初始化 */
    if (!IS_TIM_SLAVEMODE_TRIGGER_ENABLED(tmpsmcr))     /* 条件判断 */
    {                                                   /* 代码块开始 */
      __HAL_TIM_ENABLE(htim);                           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    __HAL_TIM_ENABLE(htim);                             /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Stops the PWM signal generation.
  * @param  htim TIM PWM handle
  * @param  Channel TIM Channels to be disabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_PWM_Stop(TIM_HandleTypeDef *htim, uint32_t Channel)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));  /* 执行语句 */

  /* Disable the Capture compare channel */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_DISABLE);  /* 执行语句 */

  if (IS_TIM_BREAK_INSTANCE(htim->Instance) != RESET)   /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable the Main Output */
    __HAL_TIM_MOE_DISABLE(htim);                        /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);                              /* HAL辅助宏/函数 */

  /* Set the TIM channel state */
  TIM_CHANNEL_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Starts the PWM signal generation in interrupt mode.
  * @param  htim TIM PWM handle
  * @param  Channel TIM Channel to be enabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_PWM_Start_IT(TIM_HandleTypeDef *htim, uint32_t Channel)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpsmcr;                                     /* 32位无符号变量/函数 */
  /* Check the parameters */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));  /* 执行语句 */

  /* Check the TIM channel state */
  if (TIM_CHANNEL_STATE_GET(htim, Channel) != HAL_TIM_CHANNEL_STATE_READY)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Set the TIM channel state */
  TIM_CHANNEL_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */

  switch (Channel)                                      /* switch分支 */
  {                                                     /* 代码块开始 */
    case TIM_CHANNEL_1:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Enable the TIM Capture/Compare 1 interrupt */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC1);            /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_2:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Enable the TIM Capture/Compare 2 interrupt */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC2);            /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_3:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Enable the TIM Capture/Compare 3 interrupt */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC3);            /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_4:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Enable the TIM Capture/Compare 4 interrupt */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC4);            /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    default:                                            /* default分支 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  /* Enable the Capture compare channel */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_ENABLE);  /* 执行语句 */

  if (IS_TIM_BREAK_INSTANCE(htim->Instance) != RESET)   /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable the main output */
    __HAL_TIM_MOE_ENABLE(htim);                         /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Enable the Peripheral, except in trigger mode where enable is automatically done with trigger */
  if (IS_TIM_SLAVE_INSTANCE(htim->Instance))            /* 条件判断 */
  {                                                     /* 代码块开始 */
    tmpsmcr = htim->Instance->SMCR & TIM_SMCR_SMS;      /* 赋值或初始化 */
    if (!IS_TIM_SLAVEMODE_TRIGGER_ENABLED(tmpsmcr))     /* 条件判断 */
    {                                                   /* 代码块开始 */
      __HAL_TIM_ENABLE(htim);                           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    __HAL_TIM_ENABLE(htim);                             /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Stops the PWM signal generation in interrupt mode.
  * @param  htim TIM PWM handle
  * @param  Channel TIM Channels to be disabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_PWM_Stop_IT(TIM_HandleTypeDef *htim, uint32_t Channel)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));  /* 执行语句 */

  switch (Channel)                                      /* switch分支 */
  {                                                     /* 代码块开始 */
    case TIM_CHANNEL_1:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Disable the TIM Capture/Compare 1 interrupt */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC1);           /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_2:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Disable the TIM Capture/Compare 2 interrupt */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC2);           /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_3:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Disable the TIM Capture/Compare 3 interrupt */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC3);           /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_4:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Disable the TIM Capture/Compare 4 interrupt */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC4);           /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    default:                                            /* default分支 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  /* Disable the Capture compare channel */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_DISABLE);  /* 执行语句 */

  if (IS_TIM_BREAK_INSTANCE(htim->Instance) != RESET)   /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable the Main Output */
    __HAL_TIM_MOE_DISABLE(htim);                        /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);                              /* HAL辅助宏/函数 */

  /* Set the TIM channel state */
  TIM_CHANNEL_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Starts the TIM PWM signal generation in DMA mode.
  * @param  htim TIM PWM handle
  * @param  Channel TIM Channels to be enabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @param  pData The source Buffer address.
  * @param  Length The length of data to be transferred from memory to TIM peripheral
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_PWM_Start_DMA(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t *pData, uint16_t Length)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpsmcr;                                     /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));  /* 执行语句 */

  /* Set the TIM channel state */
  if (TIM_CHANNEL_STATE_GET(htim, Channel) == HAL_TIM_CHANNEL_STATE_BUSY)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
  else if (TIM_CHANNEL_STATE_GET(htim, Channel) == HAL_TIM_CHANNEL_STATE_READY)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) && (Length > 0U))               /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      TIM_CHANNEL_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  switch (Channel)                                      /* switch分支 */
  {                                                     /* 代码块开始 */
    case TIM_CHANNEL_1:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Set the DMA compare callbacks */
      htim->hdma[TIM_DMA_ID_CC1]->XferCpltCallback = TIM_DMADelayPulseCplt;  /* 赋值或初始化 */
      htim->hdma[TIM_DMA_ID_CC1]->XferHalfCpltCallback = TIM_DMADelayPulseHalfCplt;  /* 赋值或初始化 */

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC1]->XferErrorCallback = TIM_DMAError ;  /* 赋值或初始化 */

      /* Enable the DMA stream */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC1], (uint32_t)pData, (uint32_t)&htim->Instance->CCR1, Length) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Return error status */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */

      /* Enable the TIM Capture/Compare 1 DMA request */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC1);          /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_2:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Set the DMA compare callbacks */
      htim->hdma[TIM_DMA_ID_CC2]->XferCpltCallback = TIM_DMADelayPulseCplt;  /* 赋值或初始化 */
      htim->hdma[TIM_DMA_ID_CC2]->XferHalfCpltCallback = TIM_DMADelayPulseHalfCplt;  /* 赋值或初始化 */

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC2]->XferErrorCallback = TIM_DMAError ;  /* 赋值或初始化 */

      /* Enable the DMA stream */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC2], (uint32_t)pData, (uint32_t)&htim->Instance->CCR2, Length) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Return error status */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
      /* Enable the TIM Capture/Compare 2 DMA request */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC2);          /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_3:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Set the DMA compare callbacks */
      htim->hdma[TIM_DMA_ID_CC3]->XferCpltCallback = TIM_DMADelayPulseCplt;  /* 赋值或初始化 */
      htim->hdma[TIM_DMA_ID_CC3]->XferHalfCpltCallback = TIM_DMADelayPulseHalfCplt;  /* 赋值或初始化 */

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC3]->XferErrorCallback = TIM_DMAError ;  /* 赋值或初始化 */

      /* Enable the DMA stream */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC3], (uint32_t)pData, (uint32_t)&htim->Instance->CCR3, Length) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Return error status */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
      /* Enable the TIM Output Capture/Compare 3 request */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC3);          /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_4:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Set the DMA compare callbacks */
      htim->hdma[TIM_DMA_ID_CC4]->XferCpltCallback = TIM_DMADelayPulseCplt;  /* 赋值或初始化 */
      htim->hdma[TIM_DMA_ID_CC4]->XferHalfCpltCallback = TIM_DMADelayPulseHalfCplt;  /* 赋值或初始化 */

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC4]->XferErrorCallback = TIM_DMAError ;  /* 赋值或初始化 */

      /* Enable the DMA stream */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC4], (uint32_t)pData, (uint32_t)&htim->Instance->CCR4, Length) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Return error status */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
      /* Enable the TIM Capture/Compare 4 DMA request */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC4);          /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    default:                                            /* default分支 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  /* Enable the Capture compare channel */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_ENABLE);  /* 执行语句 */

  if (IS_TIM_BREAK_INSTANCE(htim->Instance) != RESET)   /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable the main output */
    __HAL_TIM_MOE_ENABLE(htim);                         /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Enable the Peripheral, except in trigger mode where enable is automatically done with trigger */
  if (IS_TIM_SLAVE_INSTANCE(htim->Instance))            /* 条件判断 */
  {                                                     /* 代码块开始 */
    tmpsmcr = htim->Instance->SMCR & TIM_SMCR_SMS;      /* 赋值或初始化 */
    if (!IS_TIM_SLAVEMODE_TRIGGER_ENABLED(tmpsmcr))     /* 条件判断 */
    {                                                   /* 代码块开始 */
      __HAL_TIM_ENABLE(htim);                           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    __HAL_TIM_ENABLE(htim);                             /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Stops the TIM PWM signal generation in DMA mode.
  * @param  htim TIM PWM handle
  * @param  Channel TIM Channels to be disabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_PWM_Stop_DMA(TIM_HandleTypeDef *htim, uint32_t Channel)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));  /* 执行语句 */

  switch (Channel)                                      /* switch分支 */
  {                                                     /* 代码块开始 */
    case TIM_CHANNEL_1:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Disable the TIM Capture/Compare 1 DMA request */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC1);         /* HAL辅助宏/函数 */
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC1]);  /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_2:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Disable the TIM Capture/Compare 2 DMA request */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC2);         /* HAL辅助宏/函数 */
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC2]);  /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_3:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Disable the TIM Capture/Compare 3 DMA request */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC3);         /* HAL辅助宏/函数 */
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC3]);  /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_4:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Disable the TIM Capture/Compare 4 interrupt */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC4);         /* HAL辅助宏/函数 */
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC4]);  /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    default:                                            /* default分支 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  /* Disable the Capture compare channel */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_DISABLE);  /* 执行语句 */

  if (IS_TIM_BREAK_INSTANCE(htim->Instance) != RESET)   /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable the Main Output */
    __HAL_TIM_MOE_DISABLE(htim);                        /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);                              /* HAL辅助宏/函数 */

  /* Set the TIM channel state */
  TIM_CHANNEL_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup TIM_Exported_Functions_Group4 TIM Input Capture functions
  *  @brief    TIM Input Capture functions
  *
@verbatim
  ==============================================================================
              ##### TIM Input Capture functions #####
  ==============================================================================
 [..]
   This section provides functions allowing to:
   (+) Initialize and configure the TIM Input Capture.
   (+) De-initialize the TIM Input Capture.
   (+) Start the TIM Input Capture.
   (+) Stop the TIM Input Capture.
   (+) Start the TIM Input Capture and enable interrupt.
   (+) Stop the TIM Input Capture and disable interrupt.
   (+) Start the TIM Input Capture and enable DMA transfer.
   (+) Stop the TIM Input Capture and disable DMA transfer.

@endverbatim
  * @{
  */
/**
  * @brief  Initializes the TIM Input Capture Time base according to the specified
  *         parameters in the TIM_HandleTypeDef and initializes the associated handle.
  * @note   Switching from Center Aligned counter mode to Edge counter mode (or reverse)
  *         requires a timer reset to avoid unexpected direction
  *         due to DIR bit readonly in center aligned mode.
  *         Ex: call @ref HAL_TIM_IC_DeInit() before HAL_TIM_IC_Init()
  * @param  htim TIM Input Capture handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_IC_Init(TIM_HandleTypeDef *htim)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the TIM handle allocation */
  if (htim == NULL)                                     /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the parameters */
  assert_param(IS_TIM_INSTANCE(htim->Instance));        /* 执行语句 */
  assert_param(IS_TIM_COUNTER_MODE(htim->Init.CounterMode));  /* 执行语句 */
  assert_param(IS_TIM_CLOCKDIVISION_DIV(htim->Init.ClockDivision));  /* 执行语句 */
  assert_param(IS_TIM_AUTORELOAD_PRELOAD(htim->Init.AutoReloadPreload));  /* 执行语句 */

  if (htim->State == HAL_TIM_STATE_RESET)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Allocate lock resource and initialize it */
    htim->Lock = HAL_UNLOCKED;                          /* 赋值或初始化 */

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)               /* 条件编译 */
    /* Reset interrupt callbacks to legacy weak callbacks */
    TIM_ResetCallback(htim);                            /* 执行语句 */

    if (htim->IC_MspInitCallback == NULL)               /* 条件判断 */
    {                                                   /* 代码块开始 */
      htim->IC_MspInitCallback = HAL_TIM_IC_MspInit;    /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    /* Init the low level hardware : GPIO, CLOCK, NVIC */
    htim->IC_MspInitCallback(htim);                     /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /* Init the low level hardware : GPIO, CLOCK, NVIC and DMA */
    HAL_TIM_IC_MspInit(htim);                           /* HAL 定时器操作 */
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */

  /* Set the TIM state */
  htim->State = HAL_TIM_STATE_BUSY;                     /* 赋值或初始化 */

  /* Init the base time for the input capture */
  TIM_Base_SetConfig(htim->Instance, &htim->Init);      /* 执行语句 */

  /* Initialize the DMA burst operation state */
  htim->DMABurstState = HAL_DMA_BURST_STATE_READY;      /* 赋值或初始化 */

  /* Initialize the TIM channels state */
  TIM_CHANNEL_STATE_SET_ALL(htim, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
  TIM_CHANNEL_N_STATE_SET_ALL(htim, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */

  /* Initialize the TIM state*/
  htim->State = HAL_TIM_STATE_READY;                    /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  DeInitializes the TIM peripheral
  * @param  htim TIM Input Capture handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_IC_DeInit(TIM_HandleTypeDef *htim)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_INSTANCE(htim->Instance));        /* 执行语句 */

  htim->State = HAL_TIM_STATE_BUSY;                     /* 赋值或初始化 */

  /* Disable the TIM Peripheral Clock */
  __HAL_TIM_DISABLE(htim);                              /* HAL辅助宏/函数 */

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)               /* 条件编译 */
  if (htim->IC_MspDeInitCallback == NULL)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    htim->IC_MspDeInitCallback = HAL_TIM_IC_MspDeInit;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  /* DeInit the low level hardware */
  htim->IC_MspDeInitCallback(htim);                     /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* DeInit the low level hardware: GPIO, CLOCK, NVIC and DMA */
  HAL_TIM_IC_MspDeInit(htim);                           /* HAL 定时器操作 */
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */

  /* Change the DMA burst operation state */
  htim->DMABurstState = HAL_DMA_BURST_STATE_RESET;      /* 赋值或初始化 */

  /* Change the TIM channels state */
  TIM_CHANNEL_STATE_SET_ALL(htim, HAL_TIM_CHANNEL_STATE_RESET);  /* 执行语句 */
  TIM_CHANNEL_N_STATE_SET_ALL(htim, HAL_TIM_CHANNEL_STATE_RESET);  /* 执行语句 */

  /* Change TIM state */
  htim->State = HAL_TIM_STATE_RESET;                    /* 赋值或初始化 */

  /* Release Lock */
  __HAL_UNLOCK(htim);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initializes the TIM Input Capture MSP.
  * @param  htim TIM Input Capture handle
  * @retval None
  */
__weak void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_IC_MspInit could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  DeInitializes TIM Input Capture MSP.
  * @param  htim TIM handle
  * @retval None
  */
__weak void HAL_TIM_IC_MspDeInit(TIM_HandleTypeDef *htim)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_IC_MspDeInit could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Starts the TIM Input Capture measurement.
  * @param  htim TIM Input Capture handle
  * @param  Channel TIM Channels to be enabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_IC_Start(TIM_HandleTypeDef *htim, uint32_t Channel)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpsmcr;                                     /* 32位无符号变量/函数 */
  HAL_TIM_ChannelStateTypeDef channel_state = TIM_CHANNEL_STATE_GET(htim, Channel);  /* HAL 定时器操作 */
  HAL_TIM_ChannelStateTypeDef complementary_channel_state = TIM_CHANNEL_N_STATE_GET(htim, Channel);  /* HAL 定时器操作 */

  /* Check the parameters */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));  /* 执行语句 */

  /* Check the TIM channel state */
  if ((channel_state != HAL_TIM_CHANNEL_STATE_READY)    /* 条件判断 */
      || (complementary_channel_state != HAL_TIM_CHANNEL_STATE_READY))  /* 源代码 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Set the TIM channel state */
  TIM_CHANNEL_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */
  TIM_CHANNEL_N_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */

  /* Enable the Input Capture channel */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_ENABLE);  /* 执行语句 */

  /* Enable the Peripheral, except in trigger mode where enable is automatically done with trigger */
  if (IS_TIM_SLAVE_INSTANCE(htim->Instance))            /* 条件判断 */
  {                                                     /* 代码块开始 */
    tmpsmcr = htim->Instance->SMCR & TIM_SMCR_SMS;      /* 赋值或初始化 */
    if (!IS_TIM_SLAVEMODE_TRIGGER_ENABLED(tmpsmcr))     /* 条件判断 */
    {                                                   /* 代码块开始 */
      __HAL_TIM_ENABLE(htim);                           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    __HAL_TIM_ENABLE(htim);                             /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Stops the TIM Input Capture measurement.
  * @param  htim TIM Input Capture handle
  * @param  Channel TIM Channels to be disabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_IC_Stop(TIM_HandleTypeDef *htim, uint32_t Channel)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));  /* 执行语句 */

  /* Disable the Input Capture channel */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_DISABLE);  /* 执行语句 */

  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);                              /* HAL辅助宏/函数 */

  /* Set the TIM channel state */
  TIM_CHANNEL_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
  TIM_CHANNEL_N_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Starts the TIM Input Capture measurement in interrupt mode.
  * @param  htim TIM Input Capture handle
  * @param  Channel TIM Channels to be enabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_IC_Start_IT(TIM_HandleTypeDef *htim, uint32_t Channel)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpsmcr;                                     /* 32位无符号变量/函数 */
  HAL_TIM_ChannelStateTypeDef channel_state = TIM_CHANNEL_STATE_GET(htim, Channel);  /* HAL 定时器操作 */
  HAL_TIM_ChannelStateTypeDef complementary_channel_state = TIM_CHANNEL_N_STATE_GET(htim, Channel);  /* HAL 定时器操作 */

  /* Check the parameters */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));  /* 执行语句 */

  /* Check the TIM channel state */
  if ((channel_state != HAL_TIM_CHANNEL_STATE_READY)    /* 条件判断 */
      || (complementary_channel_state != HAL_TIM_CHANNEL_STATE_READY))  /* 源代码 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Set the TIM channel state */
  TIM_CHANNEL_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */
  TIM_CHANNEL_N_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */

  switch (Channel)                                      /* switch分支 */
  {                                                     /* 代码块开始 */
    case TIM_CHANNEL_1:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Enable the TIM Capture/Compare 1 interrupt */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC1);            /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_2:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Enable the TIM Capture/Compare 2 interrupt */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC2);            /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_3:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Enable the TIM Capture/Compare 3 interrupt */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC3);            /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_4:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Enable the TIM Capture/Compare 4 interrupt */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC4);            /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    default:                                            /* default分支 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */
  /* Enable the Input Capture channel */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_ENABLE);  /* 执行语句 */

  /* Enable the Peripheral, except in trigger mode where enable is automatically done with trigger */
  if (IS_TIM_SLAVE_INSTANCE(htim->Instance))            /* 条件判断 */
  {                                                     /* 代码块开始 */
    tmpsmcr = htim->Instance->SMCR & TIM_SMCR_SMS;      /* 赋值或初始化 */
    if (!IS_TIM_SLAVEMODE_TRIGGER_ENABLED(tmpsmcr))     /* 条件判断 */
    {                                                   /* 代码块开始 */
      __HAL_TIM_ENABLE(htim);                           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    __HAL_TIM_ENABLE(htim);                             /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Stops the TIM Input Capture measurement in interrupt mode.
  * @param  htim TIM Input Capture handle
  * @param  Channel TIM Channels to be disabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_IC_Stop_IT(TIM_HandleTypeDef *htim, uint32_t Channel)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));  /* 执行语句 */

  switch (Channel)                                      /* switch分支 */
  {                                                     /* 代码块开始 */
    case TIM_CHANNEL_1:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Disable the TIM Capture/Compare 1 interrupt */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC1);           /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_2:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Disable the TIM Capture/Compare 2 interrupt */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC2);           /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_3:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Disable the TIM Capture/Compare 3 interrupt */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC3);           /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_4:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Disable the TIM Capture/Compare 4 interrupt */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC4);           /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    default:                                            /* default分支 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  /* Disable the Input Capture channel */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_DISABLE);  /* 执行语句 */

  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);                              /* HAL辅助宏/函数 */

  /* Set the TIM channel state */
  TIM_CHANNEL_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
  TIM_CHANNEL_N_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Starts the TIM Input Capture measurement in DMA mode.
  * @param  htim TIM Input Capture handle
  * @param  Channel TIM Channels to be enabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @param  pData The destination Buffer address.
  * @param  Length The length of data to be transferred from TIM peripheral to memory.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_IC_Start_DMA(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t *pData, uint16_t Length)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpsmcr;                                     /* 32位无符号变量/函数 */
  HAL_TIM_ChannelStateTypeDef channel_state = TIM_CHANNEL_STATE_GET(htim, Channel);  /* HAL 定时器操作 */
  HAL_TIM_ChannelStateTypeDef complementary_channel_state = TIM_CHANNEL_N_STATE_GET(htim, Channel);  /* HAL 定时器操作 */

  /* Check the parameters */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));  /* 执行语句 */
  assert_param(IS_TIM_DMA_CC_INSTANCE(htim->Instance));  /* 执行语句 */

  /* Set the TIM channel state */
  if ((channel_state == HAL_TIM_CHANNEL_STATE_BUSY)     /* 条件判断 */
      || (complementary_channel_state == HAL_TIM_CHANNEL_STATE_BUSY))  /* 源代码 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
  else if ((channel_state == HAL_TIM_CHANNEL_STATE_READY)  /* 否则如果 */
           && (complementary_channel_state == HAL_TIM_CHANNEL_STATE_READY))  /* 源代码 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) && (Length > 0U))               /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      TIM_CHANNEL_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */
      TIM_CHANNEL_N_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  switch (Channel)                                      /* switch分支 */
  {                                                     /* 代码块开始 */
    case TIM_CHANNEL_1:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Set the DMA capture callbacks */
      htim->hdma[TIM_DMA_ID_CC1]->XferCpltCallback = TIM_DMACaptureCplt;  /* 赋值或初始化 */
      htim->hdma[TIM_DMA_ID_CC1]->XferHalfCpltCallback = TIM_DMACaptureHalfCplt;  /* 赋值或初始化 */

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC1]->XferErrorCallback = TIM_DMAError ;  /* 赋值或初始化 */

      /* Enable the DMA stream */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC1], (uint32_t)&htim->Instance->CCR1, (uint32_t)pData, Length) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Return error status */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
      /* Enable the TIM Capture/Compare 1 DMA request */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC1);          /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_2:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Set the DMA capture callbacks */
      htim->hdma[TIM_DMA_ID_CC2]->XferCpltCallback = TIM_DMACaptureCplt;  /* 赋值或初始化 */
      htim->hdma[TIM_DMA_ID_CC2]->XferHalfCpltCallback = TIM_DMACaptureHalfCplt;  /* 赋值或初始化 */

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC2]->XferErrorCallback = TIM_DMAError ;  /* 赋值或初始化 */

      /* Enable the DMA stream */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC2], (uint32_t)&htim->Instance->CCR2, (uint32_t)pData, Length) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Return error status */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
      /* Enable the TIM Capture/Compare 2  DMA request */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC2);          /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_3:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Set the DMA capture callbacks */
      htim->hdma[TIM_DMA_ID_CC3]->XferCpltCallback = TIM_DMACaptureCplt;  /* 赋值或初始化 */
      htim->hdma[TIM_DMA_ID_CC3]->XferHalfCpltCallback = TIM_DMACaptureHalfCplt;  /* 赋值或初始化 */

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC3]->XferErrorCallback = TIM_DMAError ;  /* 赋值或初始化 */

      /* Enable the DMA stream */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC3], (uint32_t)&htim->Instance->CCR3, (uint32_t)pData, Length) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Return error status */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
      /* Enable the TIM Capture/Compare 3  DMA request */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC3);          /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_4:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Set the DMA capture callbacks */
      htim->hdma[TIM_DMA_ID_CC4]->XferCpltCallback = TIM_DMACaptureCplt;  /* 赋值或初始化 */
      htim->hdma[TIM_DMA_ID_CC4]->XferHalfCpltCallback = TIM_DMACaptureHalfCplt;  /* 赋值或初始化 */

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC4]->XferErrorCallback = TIM_DMAError ;  /* 赋值或初始化 */

      /* Enable the DMA stream */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC4], (uint32_t)&htim->Instance->CCR4, (uint32_t)pData, Length) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Return error status */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
      /* Enable the TIM Capture/Compare 4  DMA request */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC4);          /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    default:                                            /* default分支 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  /* Enable the Input Capture channel */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_ENABLE);  /* 执行语句 */

  /* Enable the Peripheral, except in trigger mode where enable is automatically done with trigger */
  if (IS_TIM_SLAVE_INSTANCE(htim->Instance))            /* 条件判断 */
  {                                                     /* 代码块开始 */
    tmpsmcr = htim->Instance->SMCR & TIM_SMCR_SMS;      /* 赋值或初始化 */
    if (!IS_TIM_SLAVEMODE_TRIGGER_ENABLED(tmpsmcr))     /* 条件判断 */
    {                                                   /* 代码块开始 */
      __HAL_TIM_ENABLE(htim);                           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    __HAL_TIM_ENABLE(htim);                             /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Stops the TIM Input Capture measurement in DMA mode.
  * @param  htim TIM Input Capture handle
  * @param  Channel TIM Channels to be disabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_IC_Stop_DMA(TIM_HandleTypeDef *htim, uint32_t Channel)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));  /* 执行语句 */
  assert_param(IS_TIM_DMA_CC_INSTANCE(htim->Instance));  /* 执行语句 */

  /* Disable the Input Capture channel */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_DISABLE);  /* 执行语句 */

  switch (Channel)                                      /* switch分支 */
  {                                                     /* 代码块开始 */
    case TIM_CHANNEL_1:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Disable the TIM Capture/Compare 1 DMA request */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC1);         /* HAL辅助宏/函数 */
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC1]);  /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_2:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Disable the TIM Capture/Compare 2 DMA request */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC2);         /* HAL辅助宏/函数 */
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC2]);  /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_3:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Disable the TIM Capture/Compare 3  DMA request */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC3);         /* HAL辅助宏/函数 */
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC3]);  /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_4:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Disable the TIM Capture/Compare 4  DMA request */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC4);         /* HAL辅助宏/函数 */
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC4]);  /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    default:                                            /* default分支 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);                              /* HAL辅助宏/函数 */

  /* Set the TIM channel state */
  TIM_CHANNEL_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
  TIM_CHANNEL_N_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */
/**
  * @}
  */

/** @defgroup TIM_Exported_Functions_Group5 TIM One Pulse functions
  *  @brief    TIM One Pulse functions
  *
@verbatim
  ==============================================================================
                        ##### TIM One Pulse functions #####
  ==============================================================================
  [..]
    This section provides functions allowing to:
    (+) Initialize and configure the TIM One Pulse.
    (+) De-initialize the TIM One Pulse.
    (+) Start the TIM One Pulse.
    (+) Stop the TIM One Pulse.
    (+) Start the TIM One Pulse and enable interrupt.
    (+) Stop the TIM One Pulse and disable interrupt.
    (+) Start the TIM One Pulse and enable DMA transfer.
    (+) Stop the TIM One Pulse and disable DMA transfer.

@endverbatim
  * @{
  */
/**
  * @brief  Initializes the TIM One Pulse Time Base according to the specified
  *         parameters in the TIM_HandleTypeDef and initializes the associated handle.
  * @note   Switching from Center Aligned counter mode to Edge counter mode (or reverse)
  *         requires a timer reset to avoid unexpected direction
  *         due to DIR bit readonly in center aligned mode.
  *         Ex: call @ref HAL_TIM_OnePulse_DeInit() before HAL_TIM_OnePulse_Init()
  * @note   When the timer instance is initialized in One Pulse mode, timer
  *         channels 1 and channel 2 are reserved and cannot be used for other
  *         purpose.
  * @param  htim TIM One Pulse handle
  * @param  OnePulseMode Select the One pulse mode.
  *         This parameter can be one of the following values:
  *            @arg TIM_OPMODE_SINGLE: Only one pulse will be generated.
  *            @arg TIM_OPMODE_REPETITIVE: Repetitive pulses will be generated.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_OnePulse_Init(TIM_HandleTypeDef *htim, uint32_t OnePulseMode)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the TIM handle allocation */
  if (htim == NULL)                                     /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the parameters */
  assert_param(IS_TIM_INSTANCE(htim->Instance));        /* 执行语句 */
  assert_param(IS_TIM_COUNTER_MODE(htim->Init.CounterMode));  /* 执行语句 */
  assert_param(IS_TIM_CLOCKDIVISION_DIV(htim->Init.ClockDivision));  /* 执行语句 */
  assert_param(IS_TIM_OPM_MODE(OnePulseMode));          /* 执行语句 */
  assert_param(IS_TIM_AUTORELOAD_PRELOAD(htim->Init.AutoReloadPreload));  /* 执行语句 */

  if (htim->State == HAL_TIM_STATE_RESET)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Allocate lock resource and initialize it */
    htim->Lock = HAL_UNLOCKED;                          /* 赋值或初始化 */

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)               /* 条件编译 */
    /* Reset interrupt callbacks to legacy weak callbacks */
    TIM_ResetCallback(htim);                            /* 执行语句 */

    if (htim->OnePulse_MspInitCallback == NULL)         /* 条件判断 */
    {                                                   /* 代码块开始 */
      htim->OnePulse_MspInitCallback = HAL_TIM_OnePulse_MspInit;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    /* Init the low level hardware : GPIO, CLOCK, NVIC */
    htim->OnePulse_MspInitCallback(htim);               /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /* Init the low level hardware : GPIO, CLOCK, NVIC and DMA */
    HAL_TIM_OnePulse_MspInit(htim);                     /* HAL 定时器操作 */
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */

  /* Set the TIM state */
  htim->State = HAL_TIM_STATE_BUSY;                     /* 赋值或初始化 */

  /* Configure the Time base in the One Pulse Mode */
  TIM_Base_SetConfig(htim->Instance, &htim->Init);      /* 执行语句 */

  /* Reset the OPM Bit */
  htim->Instance->CR1 &= ~TIM_CR1_OPM;                  /* 赋值或初始化 */

  /* Configure the OPM Mode */
  htim->Instance->CR1 |= OnePulseMode;                  /* 赋值或初始化 */

  /* Initialize the DMA burst operation state */
  htim->DMABurstState = HAL_DMA_BURST_STATE_READY;      /* 赋值或初始化 */

  /* Initialize the TIM channels state */
  TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
  TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
  TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
  TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */

  /* Initialize the TIM state*/
  htim->State = HAL_TIM_STATE_READY;                    /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  DeInitializes the TIM One Pulse
  * @param  htim TIM One Pulse handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_OnePulse_DeInit(TIM_HandleTypeDef *htim)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_INSTANCE(htim->Instance));        /* 执行语句 */

  htim->State = HAL_TIM_STATE_BUSY;                     /* 赋值或初始化 */

  /* Disable the TIM Peripheral Clock */
  __HAL_TIM_DISABLE(htim);                              /* HAL辅助宏/函数 */

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)               /* 条件编译 */
  if (htim->OnePulse_MspDeInitCallback == NULL)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    htim->OnePulse_MspDeInitCallback = HAL_TIM_OnePulse_MspDeInit;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  /* DeInit the low level hardware */
  htim->OnePulse_MspDeInitCallback(htim);               /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* DeInit the low level hardware: GPIO, CLOCK, NVIC */
  HAL_TIM_OnePulse_MspDeInit(htim);                     /* HAL 定时器操作 */
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */

  /* Change the DMA burst operation state */
  htim->DMABurstState = HAL_DMA_BURST_STATE_RESET;      /* 赋值或初始化 */

  /* Set the TIM channel state */
  TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_RESET);  /* 执行语句 */
  TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_RESET);  /* 执行语句 */
  TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_RESET);  /* 执行语句 */
  TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_RESET);  /* 执行语句 */

  /* Change TIM state */
  htim->State = HAL_TIM_STATE_RESET;                    /* 赋值或初始化 */

  /* Release Lock */
  __HAL_UNLOCK(htim);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initializes the TIM One Pulse MSP.
  * @param  htim TIM One Pulse handle
  * @retval None
  */
__weak void HAL_TIM_OnePulse_MspInit(TIM_HandleTypeDef *htim)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_OnePulse_MspInit could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  DeInitializes TIM One Pulse MSP.
  * @param  htim TIM One Pulse handle
  * @retval None
  */
__weak void HAL_TIM_OnePulse_MspDeInit(TIM_HandleTypeDef *htim)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_OnePulse_MspDeInit could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Starts the TIM One Pulse signal generation.
  * @note Though OutputChannel parameter is deprecated and ignored by the function
  *        it has been kept to avoid HAL_TIM API compatibility break.
  * @note The pulse output channel is determined when calling 
  *       @ref HAL_TIM_OnePulse_ConfigChannel(). 
  * @param  htim TIM One Pulse handle
  * @param  OutputChannel See note above
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_OnePulse_Start(TIM_HandleTypeDef *htim, uint32_t OutputChannel)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_TIM_ChannelStateTypeDef channel_1_state = TIM_CHANNEL_STATE_GET(htim, TIM_CHANNEL_1);  /* HAL 定时器操作 */
  HAL_TIM_ChannelStateTypeDef channel_2_state = TIM_CHANNEL_STATE_GET(htim, TIM_CHANNEL_2);  /* HAL 定时器操作 */
  HAL_TIM_ChannelStateTypeDef complementary_channel_1_state = TIM_CHANNEL_N_STATE_GET(htim, TIM_CHANNEL_1);  /* HAL 定时器操作 */
  HAL_TIM_ChannelStateTypeDef complementary_channel_2_state = TIM_CHANNEL_N_STATE_GET(htim, TIM_CHANNEL_2);  /* HAL 定时器操作 */

  /* Prevent unused argument(s) compilation warning */
  UNUSED(OutputChannel);                                /* 执行语句 */

  /* Check the TIM channels state */
  if ((channel_1_state != HAL_TIM_CHANNEL_STATE_READY)  /* 条件判断 */
      || (channel_2_state != HAL_TIM_CHANNEL_STATE_READY)  /* 源代码 */
      || (complementary_channel_1_state != HAL_TIM_CHANNEL_STATE_READY)  /* 源代码 */
      || (complementary_channel_2_state != HAL_TIM_CHANNEL_STATE_READY))  /* 源代码 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Set the TIM channels state */
  TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */
  TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */
  TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */
  TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */

  /* Enable the Capture compare and the Input Capture channels
    (in the OPM Mode the two possible channels that can be used are TIM_CHANNEL_1 and TIM_CHANNEL_2)
    if TIM_CHANNEL_1 is used as output, the TIM_CHANNEL_2 will be used as input and
    if TIM_CHANNEL_1 is used as input, the TIM_CHANNEL_2 will be used as output
    whatever the combination, the TIM_CHANNEL_1 and TIM_CHANNEL_2 should be enabled together

    No need to enable the counter, it's enabled automatically by hardware
    (the counter starts in response to a stimulus and generate a pulse */

  TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);  /* 执行语句 */
  TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_ENABLE);  /* 执行语句 */

  if (IS_TIM_BREAK_INSTANCE(htim->Instance) != RESET)   /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable the main output */
    __HAL_TIM_MOE_ENABLE(htim);                         /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Stops the TIM One Pulse signal generation.
  * @note Though OutputChannel parameter is deprecated and ignored by the function
  *        it has been kept to avoid HAL_TIM API compatibility break.
  * @note The pulse output channel is determined when calling 
  *       @ref HAL_TIM_OnePulse_ConfigChannel(). 
  * @param  htim TIM One Pulse handle
  * @param  OutputChannel See note above
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_OnePulse_Stop(TIM_HandleTypeDef *htim, uint32_t OutputChannel)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(OutputChannel);                                /* 执行语句 */

  /* Disable the Capture compare and the Input Capture channels
  (in the OPM Mode the two possible channels that can be used are TIM_CHANNEL_1 and TIM_CHANNEL_2)
  if TIM_CHANNEL_1 is used as output, the TIM_CHANNEL_2 will be used as input and
  if TIM_CHANNEL_1 is used as input, the TIM_CHANNEL_2 will be used as output
  whatever the combination, the TIM_CHANNEL_1 and TIM_CHANNEL_2 should be disabled together */

  TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_DISABLE);  /* 执行语句 */
  TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_DISABLE);  /* 执行语句 */

  if (IS_TIM_BREAK_INSTANCE(htim->Instance) != RESET)   /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable the Main Output */
    __HAL_TIM_MOE_DISABLE(htim);                        /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);                              /* HAL辅助宏/函数 */

  /* Set the TIM channels state */
  TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
  TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
  TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
  TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Starts the TIM One Pulse signal generation in interrupt mode.
  * @note Though OutputChannel parameter is deprecated and ignored by the function
  *        it has been kept to avoid HAL_TIM API compatibility break.
  * @note The pulse output channel is determined when calling 
  *       @ref HAL_TIM_OnePulse_ConfigChannel(). 
  * @param  htim TIM One Pulse handle
  * @param  OutputChannel See note above
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_OnePulse_Start_IT(TIM_HandleTypeDef *htim, uint32_t OutputChannel)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_TIM_ChannelStateTypeDef channel_1_state = TIM_CHANNEL_STATE_GET(htim, TIM_CHANNEL_1);  /* HAL 定时器操作 */
  HAL_TIM_ChannelStateTypeDef channel_2_state = TIM_CHANNEL_STATE_GET(htim, TIM_CHANNEL_2);  /* HAL 定时器操作 */
  HAL_TIM_ChannelStateTypeDef complementary_channel_1_state = TIM_CHANNEL_N_STATE_GET(htim, TIM_CHANNEL_1);  /* HAL 定时器操作 */
  HAL_TIM_ChannelStateTypeDef complementary_channel_2_state = TIM_CHANNEL_N_STATE_GET(htim, TIM_CHANNEL_2);  /* HAL 定时器操作 */

  /* Prevent unused argument(s) compilation warning */
  UNUSED(OutputChannel);                                /* 执行语句 */

  /* Check the TIM channels state */
  if ((channel_1_state != HAL_TIM_CHANNEL_STATE_READY)  /* 条件判断 */
      || (channel_2_state != HAL_TIM_CHANNEL_STATE_READY)  /* 源代码 */
      || (complementary_channel_1_state != HAL_TIM_CHANNEL_STATE_READY)  /* 源代码 */
      || (complementary_channel_2_state != HAL_TIM_CHANNEL_STATE_READY))  /* 源代码 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Set the TIM channels state */
  TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */
  TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */
  TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */
  TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */

  /* Enable the Capture compare and the Input Capture channels
    (in the OPM Mode the two possible channels that can be used are TIM_CHANNEL_1 and TIM_CHANNEL_2)
    if TIM_CHANNEL_1 is used as output, the TIM_CHANNEL_2 will be used as input and
    if TIM_CHANNEL_1 is used as input, the TIM_CHANNEL_2 will be used as output
    whatever the combination, the TIM_CHANNEL_1 and TIM_CHANNEL_2 should be enabled together

    No need to enable the counter, it's enabled automatically by hardware
    (the counter starts in response to a stimulus and generate a pulse */

  /* Enable the TIM Capture/Compare 1 interrupt */
  __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC1);                /* HAL辅助宏/函数 */

  /* Enable the TIM Capture/Compare 2 interrupt */
  __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC2);                /* HAL辅助宏/函数 */

  TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);  /* 执行语句 */
  TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_ENABLE);  /* 执行语句 */

  if (IS_TIM_BREAK_INSTANCE(htim->Instance) != RESET)   /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable the main output */
    __HAL_TIM_MOE_ENABLE(htim);                         /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Stops the TIM One Pulse signal generation in interrupt mode.
  * @note Though OutputChannel parameter is deprecated and ignored by the function
  *        it has been kept to avoid HAL_TIM API compatibility break.
  * @note The pulse output channel is determined when calling 
  *       @ref HAL_TIM_OnePulse_ConfigChannel(). 
  * @param  htim TIM One Pulse handle
  * @param  OutputChannel See note above
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_OnePulse_Stop_IT(TIM_HandleTypeDef *htim, uint32_t OutputChannel)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(OutputChannel);                                /* 执行语句 */

  /* Disable the TIM Capture/Compare 1 interrupt */
  __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC1);               /* HAL辅助宏/函数 */

  /* Disable the TIM Capture/Compare 2 interrupt */
  __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC2);               /* HAL辅助宏/函数 */

  /* Disable the Capture compare and the Input Capture channels
  (in the OPM Mode the two possible channels that can be used are TIM_CHANNEL_1 and TIM_CHANNEL_2)
  if TIM_CHANNEL_1 is used as output, the TIM_CHANNEL_2 will be used as input and
  if TIM_CHANNEL_1 is used as input, the TIM_CHANNEL_2 will be used as output
  whatever the combination, the TIM_CHANNEL_1 and TIM_CHANNEL_2 should be disabled together */
  TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_DISABLE);  /* 执行语句 */
  TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_DISABLE);  /* 执行语句 */

  if (IS_TIM_BREAK_INSTANCE(htim->Instance) != RESET)   /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable the Main Output */
    __HAL_TIM_MOE_DISABLE(htim);                        /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);                              /* HAL辅助宏/函数 */

  /* Set the TIM channels state */
  TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
  TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
  TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
  TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup TIM_Exported_Functions_Group6 TIM Encoder functions
  *  @brief    TIM Encoder functions
  *
@verbatim
  ==============================================================================
                          ##### TIM Encoder functions #####
  ==============================================================================
  [..]
    This section provides functions allowing to:
    (+) Initialize and configure the TIM Encoder.
    (+) De-initialize the TIM Encoder.
    (+) Start the TIM Encoder.
    (+) Stop the TIM Encoder.
    (+) Start the TIM Encoder and enable interrupt.
    (+) Stop the TIM Encoder and disable interrupt.
    (+) Start the TIM Encoder and enable DMA transfer.
    (+) Stop the TIM Encoder and disable DMA transfer.

@endverbatim
  * @{
  */
/**
  * @brief  Initializes the TIM Encoder Interface and initialize the associated handle.
  * @note   Switching from Center Aligned counter mode to Edge counter mode (or reverse)
  *         requires a timer reset to avoid unexpected direction
  *         due to DIR bit readonly in center aligned mode.
  *         Ex: call @ref HAL_TIM_Encoder_DeInit() before HAL_TIM_Encoder_Init()
  * @note   Encoder mode and External clock mode 2 are not compatible and must not be selected together
  *         Ex: A call for @ref HAL_TIM_Encoder_Init will erase the settings of @ref HAL_TIM_ConfigClockSource
  *         using TIM_CLOCKSOURCE_ETRMODE2 and vice versa
  * @note   When the timer instance is initialized in Encoder mode, timer
  *         channels 1 and channel 2 are reserved and cannot be used for other
  *         purpose.
  * @param  htim TIM Encoder Interface handle
  * @param  sConfig TIM Encoder Interface configuration structure
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_Encoder_Init(TIM_HandleTypeDef *htim,  TIM_Encoder_InitTypeDef *sConfig)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpsmcr;                                     /* 32位无符号变量/函数 */
  uint32_t tmpccmr1;                                    /* 32位无符号变量/函数 */
  uint32_t tmpccer;                                     /* 32位无符号变量/函数 */

  /* Check the TIM handle allocation */
  if (htim == NULL)                                     /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the parameters */
  assert_param(IS_TIM_ENCODER_INTERFACE_INSTANCE(htim->Instance));  /* 执行语句 */
  assert_param(IS_TIM_COUNTER_MODE(htim->Init.CounterMode));  /* 执行语句 */
  assert_param(IS_TIM_CLOCKDIVISION_DIV(htim->Init.ClockDivision));  /* 执行语句 */
  assert_param(IS_TIM_AUTORELOAD_PRELOAD(htim->Init.AutoReloadPreload));  /* 执行语句 */
  assert_param(IS_TIM_ENCODER_MODE(sConfig->EncoderMode));  /* 执行语句 */
  assert_param(IS_TIM_IC_SELECTION(sConfig->IC1Selection));  /* 执行语句 */
  assert_param(IS_TIM_IC_SELECTION(sConfig->IC2Selection));  /* 执行语句 */
  assert_param(IS_TIM_ENCODERINPUT_POLARITY(sConfig->IC1Polarity));  /* 执行语句 */
  assert_param(IS_TIM_ENCODERINPUT_POLARITY(sConfig->IC2Polarity));  /* 执行语句 */
  assert_param(IS_TIM_IC_PRESCALER(sConfig->IC1Prescaler));  /* 执行语句 */
  assert_param(IS_TIM_IC_PRESCALER(sConfig->IC2Prescaler));  /* 执行语句 */
  assert_param(IS_TIM_IC_FILTER(sConfig->IC1Filter));   /* 执行语句 */
  assert_param(IS_TIM_IC_FILTER(sConfig->IC2Filter));   /* 执行语句 */

  if (htim->State == HAL_TIM_STATE_RESET)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Allocate lock resource and initialize it */
    htim->Lock = HAL_UNLOCKED;                          /* 赋值或初始化 */

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)               /* 条件编译 */
    /* Reset interrupt callbacks to legacy weak callbacks */
    TIM_ResetCallback(htim);                            /* 执行语句 */

    if (htim->Encoder_MspInitCallback == NULL)          /* 条件判断 */
    {                                                   /* 代码块开始 */
      htim->Encoder_MspInitCallback = HAL_TIM_Encoder_MspInit;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    /* Init the low level hardware : GPIO, CLOCK, NVIC */
    htim->Encoder_MspInitCallback(htim);                /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /* Init the low level hardware : GPIO, CLOCK, NVIC and DMA */
    HAL_TIM_Encoder_MspInit(htim);                      /* HAL 定时器操作 */
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */

  /* Set the TIM state */
  htim->State = HAL_TIM_STATE_BUSY;                     /* 赋值或初始化 */

  /* Reset the SMS and ECE bits */
  htim->Instance->SMCR &= ~(TIM_SMCR_SMS | TIM_SMCR_ECE);  /* 赋值或初始化 */

  /* Configure the Time base in the Encoder Mode */
  TIM_Base_SetConfig(htim->Instance, &htim->Init);      /* 执行语句 */

  /* Get the TIMx SMCR register value */
  tmpsmcr = htim->Instance->SMCR;                       /* 赋值或初始化 */

  /* Get the TIMx CCMR1 register value */
  tmpccmr1 = htim->Instance->CCMR1;                     /* 赋值或初始化 */

  /* Get the TIMx CCER register value */
  tmpccer = htim->Instance->CCER;                       /* 赋值或初始化 */

  /* Set the encoder Mode */
  tmpsmcr |= sConfig->EncoderMode;                      /* 赋值或初始化 */

  /* Select the Capture Compare 1 and the Capture Compare 2 as input */
  tmpccmr1 &= ~(TIM_CCMR1_CC1S | TIM_CCMR1_CC2S);       /* 赋值或初始化 */
  tmpccmr1 |= (sConfig->IC1Selection | (sConfig->IC2Selection << 8U));  /* 赋值或初始化 */

  /* Set the Capture Compare 1 and the Capture Compare 2 prescalers and filters */
  tmpccmr1 &= ~(TIM_CCMR1_IC1PSC | TIM_CCMR1_IC2PSC);   /* 赋值或初始化 */
  tmpccmr1 &= ~(TIM_CCMR1_IC1F | TIM_CCMR1_IC2F);       /* 赋值或初始化 */
  tmpccmr1 |= sConfig->IC1Prescaler | (sConfig->IC2Prescaler << 8U);  /* 赋值或初始化 */
  tmpccmr1 |= (sConfig->IC1Filter << 4U) | (sConfig->IC2Filter << 12U);  /* 赋值或初始化 */

  /* Set the TI1 and the TI2 Polarities */
  tmpccer &= ~(TIM_CCER_CC1P | TIM_CCER_CC2P);          /* 赋值或初始化 */
  tmpccer &= ~(TIM_CCER_CC1NP | TIM_CCER_CC2NP);        /* 赋值或初始化 */
  tmpccer |= sConfig->IC1Polarity | (sConfig->IC2Polarity << 4U);  /* 赋值或初始化 */

  /* Write to TIMx SMCR */
  htim->Instance->SMCR = tmpsmcr;                       /* 赋值或初始化 */

  /* Write to TIMx CCMR1 */
  htim->Instance->CCMR1 = tmpccmr1;                     /* 赋值或初始化 */

  /* Write to TIMx CCER */
  htim->Instance->CCER = tmpccer;                       /* 赋值或初始化 */

  /* Initialize the DMA burst operation state */
  htim->DMABurstState = HAL_DMA_BURST_STATE_READY;      /* 赋值或初始化 */

  /* Set the TIM channels state */
  TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
  TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
  TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
  TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */

  /* Initialize the TIM state*/
  htim->State = HAL_TIM_STATE_READY;                    /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */


/**
  * @brief  DeInitializes the TIM Encoder interface
  * @param  htim TIM Encoder Interface handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_Encoder_DeInit(TIM_HandleTypeDef *htim)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_INSTANCE(htim->Instance));        /* 执行语句 */

  htim->State = HAL_TIM_STATE_BUSY;                     /* 赋值或初始化 */

  /* Disable the TIM Peripheral Clock */
  __HAL_TIM_DISABLE(htim);                              /* HAL辅助宏/函数 */

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)               /* 条件编译 */
  if (htim->Encoder_MspDeInitCallback == NULL)          /* 条件判断 */
  {                                                     /* 代码块开始 */
    htim->Encoder_MspDeInitCallback = HAL_TIM_Encoder_MspDeInit;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  /* DeInit the low level hardware */
  htim->Encoder_MspDeInitCallback(htim);                /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* DeInit the low level hardware: GPIO, CLOCK, NVIC */
  HAL_TIM_Encoder_MspDeInit(htim);                      /* HAL 定时器操作 */
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */

  /* Change the DMA burst operation state */
  htim->DMABurstState = HAL_DMA_BURST_STATE_RESET;      /* 赋值或初始化 */

  /* Set the TIM channels state */
  TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_RESET);  /* 执行语句 */
  TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_RESET);  /* 执行语句 */
  TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_RESET);  /* 执行语句 */
  TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_RESET);  /* 执行语句 */

  /* Change TIM state */
  htim->State = HAL_TIM_STATE_RESET;                    /* 赋值或初始化 */

  /* Release Lock */
  __HAL_UNLOCK(htim);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initializes the TIM Encoder Interface MSP.
  * @param  htim TIM Encoder Interface handle
  * @retval None
  */
__weak void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef *htim)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_Encoder_MspInit could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  DeInitializes TIM Encoder Interface MSP.
  * @param  htim TIM Encoder Interface handle
  * @retval None
  */
__weak void HAL_TIM_Encoder_MspDeInit(TIM_HandleTypeDef *htim)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_Encoder_MspDeInit could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Starts the TIM Encoder Interface.
  * @param  htim TIM Encoder Interface handle
  * @param  Channel TIM Channels to be enabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_ALL: TIM Channel 1 and TIM Channel 2 are selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_Encoder_Start(TIM_HandleTypeDef *htim, uint32_t Channel)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_TIM_ChannelStateTypeDef channel_1_state = TIM_CHANNEL_STATE_GET(htim, TIM_CHANNEL_1);  /* HAL 定时器操作 */
  HAL_TIM_ChannelStateTypeDef channel_2_state = TIM_CHANNEL_STATE_GET(htim, TIM_CHANNEL_2);  /* HAL 定时器操作 */
  HAL_TIM_ChannelStateTypeDef complementary_channel_1_state = TIM_CHANNEL_N_STATE_GET(htim, TIM_CHANNEL_1);  /* HAL 定时器操作 */
  HAL_TIM_ChannelStateTypeDef complementary_channel_2_state = TIM_CHANNEL_N_STATE_GET(htim, TIM_CHANNEL_2);  /* HAL 定时器操作 */

  /* Check the parameters */
  assert_param(IS_TIM_ENCODER_INTERFACE_INSTANCE(htim->Instance));  /* 执行语句 */

  /* Set the TIM channel(s) state */
  if (Channel == TIM_CHANNEL_1)                         /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((channel_1_state != HAL_TIM_CHANNEL_STATE_READY)  /* 条件判断 */
        || (complementary_channel_1_state != HAL_TIM_CHANNEL_STATE_READY))  /* 源代码 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */
      TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (Channel == TIM_CHANNEL_2)                    /* 否则如果 */
  {                                                     /* 代码块开始 */
    if ((channel_2_state != HAL_TIM_CHANNEL_STATE_READY)  /* 条件判断 */
        || (complementary_channel_2_state != HAL_TIM_CHANNEL_STATE_READY))  /* 源代码 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */
      TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    if ((channel_1_state != HAL_TIM_CHANNEL_STATE_READY)  /* 条件判断 */
        || (channel_2_state != HAL_TIM_CHANNEL_STATE_READY)  /* 源代码 */
        || (complementary_channel_1_state != HAL_TIM_CHANNEL_STATE_READY)  /* 源代码 */
        || (complementary_channel_2_state != HAL_TIM_CHANNEL_STATE_READY))  /* 源代码 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */
      TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */
      TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */
      TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Enable the encoder interface channels */
  switch (Channel)                                      /* switch分支 */
  {                                                     /* 代码块开始 */
    case TIM_CHANNEL_1:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);  /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_2:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_ENABLE);  /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    default :                                           /* default分支 */
    {                                                   /* 代码块开始 */
      TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);  /* 执行语句 */
      TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_ENABLE);  /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /* Enable the Peripheral */
  __HAL_TIM_ENABLE(htim);                               /* HAL辅助宏/函数 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Stops the TIM Encoder Interface.
  * @param  htim TIM Encoder Interface handle
  * @param  Channel TIM Channels to be disabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_ALL: TIM Channel 1 and TIM Channel 2 are selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_Encoder_Stop(TIM_HandleTypeDef *htim, uint32_t Channel)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_ENCODER_INTERFACE_INSTANCE(htim->Instance));  /* 执行语句 */

  /* Disable the Input Capture channels 1 and 2
    (in the EncoderInterface the two possible channels that can be used are TIM_CHANNEL_1 and TIM_CHANNEL_2) */
  switch (Channel)                                      /* switch分支 */
  {                                                     /* 代码块开始 */
    case TIM_CHANNEL_1:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_DISABLE);  /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_2:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_DISABLE);  /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    default :                                           /* default分支 */
    {                                                   /* 代码块开始 */
      TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_DISABLE);  /* 执行语句 */
      TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_DISABLE);  /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);                              /* HAL辅助宏/函数 */

  /* Set the TIM channel(s) state */
  if ((Channel == TIM_CHANNEL_1) || (Channel == TIM_CHANNEL_2))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    TIM_CHANNEL_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
    TIM_CHANNEL_N_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
    TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
    TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
    TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Starts the TIM Encoder Interface in interrupt mode.
  * @param  htim TIM Encoder Interface handle
  * @param  Channel TIM Channels to be enabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_ALL: TIM Channel 1 and TIM Channel 2 are selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_Encoder_Start_IT(TIM_HandleTypeDef *htim, uint32_t Channel)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_TIM_ChannelStateTypeDef channel_1_state = TIM_CHANNEL_STATE_GET(htim, TIM_CHANNEL_1);  /* HAL 定时器操作 */
  HAL_TIM_ChannelStateTypeDef channel_2_state = TIM_CHANNEL_STATE_GET(htim, TIM_CHANNEL_2);  /* HAL 定时器操作 */
  HAL_TIM_ChannelStateTypeDef complementary_channel_1_state = TIM_CHANNEL_N_STATE_GET(htim, TIM_CHANNEL_1);  /* HAL 定时器操作 */
  HAL_TIM_ChannelStateTypeDef complementary_channel_2_state = TIM_CHANNEL_N_STATE_GET(htim, TIM_CHANNEL_2);  /* HAL 定时器操作 */

  /* Check the parameters */
  assert_param(IS_TIM_ENCODER_INTERFACE_INSTANCE(htim->Instance));  /* 执行语句 */

  /* Set the TIM channel(s) state */
  if (Channel == TIM_CHANNEL_1)                         /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((channel_1_state != HAL_TIM_CHANNEL_STATE_READY)  /* 条件判断 */
        || (complementary_channel_1_state != HAL_TIM_CHANNEL_STATE_READY))  /* 源代码 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */
      TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (Channel == TIM_CHANNEL_2)                    /* 否则如果 */
  {                                                     /* 代码块开始 */
    if ((channel_2_state != HAL_TIM_CHANNEL_STATE_READY)  /* 条件判断 */
        || (complementary_channel_2_state != HAL_TIM_CHANNEL_STATE_READY))  /* 源代码 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */
      TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    if ((channel_1_state != HAL_TIM_CHANNEL_STATE_READY)  /* 条件判断 */
        || (channel_2_state != HAL_TIM_CHANNEL_STATE_READY)  /* 源代码 */
        || (complementary_channel_1_state != HAL_TIM_CHANNEL_STATE_READY)  /* 源代码 */
        || (complementary_channel_2_state != HAL_TIM_CHANNEL_STATE_READY))  /* 源代码 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */
      TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */
      TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */
      TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Enable the encoder interface channels */
  /* Enable the capture compare Interrupts 1 and/or 2 */
  switch (Channel)                                      /* switch分支 */
  {                                                     /* 代码块开始 */
    case TIM_CHANNEL_1:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);  /* 执行语句 */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC1);            /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_2:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_ENABLE);  /* 执行语句 */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC2);            /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    default :                                           /* default分支 */
    {                                                   /* 代码块开始 */
      TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);  /* 执行语句 */
      TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_ENABLE);  /* 执行语句 */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC1);            /* HAL辅助宏/函数 */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC2);            /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Enable the Peripheral */
  __HAL_TIM_ENABLE(htim);                               /* HAL辅助宏/函数 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Stops the TIM Encoder Interface in interrupt mode.
  * @param  htim TIM Encoder Interface handle
  * @param  Channel TIM Channels to be disabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_ALL: TIM Channel 1 and TIM Channel 2 are selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_Encoder_Stop_IT(TIM_HandleTypeDef *htim, uint32_t Channel)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_ENCODER_INTERFACE_INSTANCE(htim->Instance));  /* 执行语句 */

  /* Disable the Input Capture channels 1 and 2
    (in the EncoderInterface the two possible channels that can be used are TIM_CHANNEL_1 and TIM_CHANNEL_2) */
  if (Channel == TIM_CHANNEL_1)                         /* 条件判断 */
  {                                                     /* 代码块开始 */
    TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_DISABLE);  /* 执行语句 */

    /* Disable the capture compare Interrupts 1 */
    __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC1);             /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  else if (Channel == TIM_CHANNEL_2)                    /* 否则如果 */
  {                                                     /* 代码块开始 */
    TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_DISABLE);  /* 执行语句 */

    /* Disable the capture compare Interrupts 2 */
    __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC2);             /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_DISABLE);  /* 执行语句 */
    TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_DISABLE);  /* 执行语句 */

    /* Disable the capture compare Interrupts 1 and 2 */
    __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC1);             /* HAL辅助宏/函数 */
    __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC2);             /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);                              /* HAL辅助宏/函数 */

  /* Set the TIM channel(s) state */
  if ((Channel == TIM_CHANNEL_1) || (Channel == TIM_CHANNEL_2))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    TIM_CHANNEL_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
    TIM_CHANNEL_N_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
    TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
    TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
    TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Starts the TIM Encoder Interface in DMA mode.
  * @param  htim TIM Encoder Interface handle
  * @param  Channel TIM Channels to be enabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_ALL: TIM Channel 1 and TIM Channel 2 are selected
  * @param  pData1 The destination Buffer address for IC1.
  * @param  pData2 The destination Buffer address for IC2.
  * @param  Length The length of data to be transferred from TIM peripheral to memory.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_Encoder_Start_DMA(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t *pData1,  /* HAL状态类型函数 */
                                            uint32_t *pData2, uint16_t Length)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  HAL_TIM_ChannelStateTypeDef channel_1_state = TIM_CHANNEL_STATE_GET(htim, TIM_CHANNEL_1);  /* HAL 定时器操作 */
  HAL_TIM_ChannelStateTypeDef channel_2_state = TIM_CHANNEL_STATE_GET(htim, TIM_CHANNEL_2);  /* HAL 定时器操作 */
  HAL_TIM_ChannelStateTypeDef complementary_channel_1_state = TIM_CHANNEL_N_STATE_GET(htim, TIM_CHANNEL_1);  /* HAL 定时器操作 */
  HAL_TIM_ChannelStateTypeDef complementary_channel_2_state = TIM_CHANNEL_N_STATE_GET(htim, TIM_CHANNEL_2);  /* HAL 定时器操作 */

  /* Check the parameters */
  assert_param(IS_TIM_ENCODER_INTERFACE_INSTANCE(htim->Instance));  /* 执行语句 */

  /* Set the TIM channel(s) state */
  if (Channel == TIM_CHANNEL_1)                         /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((channel_1_state == HAL_TIM_CHANNEL_STATE_BUSY)  /* 条件判断 */
        || (complementary_channel_1_state == HAL_TIM_CHANNEL_STATE_BUSY))  /* 源代码 */
    {                                                   /* 代码块开始 */
      return HAL_BUSY;                                  /* 返回值 */
    }                                                   /* 代码块结束 */
    else if ((channel_1_state == HAL_TIM_CHANNEL_STATE_READY)  /* 否则如果 */
             && (complementary_channel_1_state == HAL_TIM_CHANNEL_STATE_READY))  /* 源代码 */
    {                                                   /* 代码块开始 */
      if ((pData1 == NULL) && (Length > 0U))            /* 条件判断 */
      {                                                 /* 代码块开始 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */
        TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (Channel == TIM_CHANNEL_2)                    /* 否则如果 */
  {                                                     /* 代码块开始 */
    if ((channel_2_state == HAL_TIM_CHANNEL_STATE_BUSY)  /* 条件判断 */
        || (complementary_channel_2_state == HAL_TIM_CHANNEL_STATE_BUSY))  /* 源代码 */
    {                                                   /* 代码块开始 */
      return HAL_BUSY;                                  /* 返回值 */
    }                                                   /* 代码块结束 */
    else if ((channel_2_state == HAL_TIM_CHANNEL_STATE_READY)  /* 否则如果 */
             && (complementary_channel_2_state == HAL_TIM_CHANNEL_STATE_READY))  /* 源代码 */
    {                                                   /* 代码块开始 */
      if ((pData2 == NULL) && (Length > 0U))            /* 条件判断 */
      {                                                 /* 代码块开始 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */
        TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    if ((channel_1_state == HAL_TIM_CHANNEL_STATE_BUSY)  /* 条件判断 */
        || (channel_2_state == HAL_TIM_CHANNEL_STATE_BUSY)  /* 源代码 */
        || (complementary_channel_1_state == HAL_TIM_CHANNEL_STATE_BUSY)  /* 源代码 */
        || (complementary_channel_2_state == HAL_TIM_CHANNEL_STATE_BUSY))  /* 源代码 */
    {                                                   /* 代码块开始 */
      return HAL_BUSY;                                  /* 返回值 */
    }                                                   /* 代码块结束 */
    else if ((channel_1_state == HAL_TIM_CHANNEL_STATE_READY)  /* 否则如果 */
             && (channel_2_state == HAL_TIM_CHANNEL_STATE_READY)  /* 源代码 */
             && (complementary_channel_1_state == HAL_TIM_CHANNEL_STATE_READY)  /* 源代码 */
             && (complementary_channel_2_state == HAL_TIM_CHANNEL_STATE_READY))  /* 源代码 */
    {                                                   /* 代码块开始 */
      if ((((pData1 == NULL) || (pData2 == NULL))) && (Length > 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */
        TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */
        TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */
        TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  switch (Channel)                                      /* switch分支 */
  {                                                     /* 代码块开始 */
    case TIM_CHANNEL_1:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Set the DMA capture callbacks */
      htim->hdma[TIM_DMA_ID_CC1]->XferCpltCallback = TIM_DMACaptureCplt;  /* 赋值或初始化 */
      htim->hdma[TIM_DMA_ID_CC1]->XferHalfCpltCallback = TIM_DMACaptureHalfCplt;  /* 赋值或初始化 */

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC1]->XferErrorCallback = TIM_DMAError ;  /* 赋值或初始化 */

      /* Enable the DMA stream */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC1], (uint32_t)&htim->Instance->CCR1, (uint32_t)pData1, Length) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Return error status */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
      /* Enable the TIM Input Capture DMA request */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC1);          /* HAL辅助宏/函数 */

      /* Enable the Peripheral */
      __HAL_TIM_ENABLE(htim);                           /* HAL辅助宏/函数 */

      /* Enable the Capture compare channel */
      TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);  /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_2:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Set the DMA capture callbacks */
      htim->hdma[TIM_DMA_ID_CC2]->XferCpltCallback = TIM_DMACaptureCplt;  /* 赋值或初始化 */
      htim->hdma[TIM_DMA_ID_CC2]->XferHalfCpltCallback = TIM_DMACaptureHalfCplt;  /* 赋值或初始化 */

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC2]->XferErrorCallback = TIM_DMAError;  /* 赋值或初始化 */
      /* Enable the DMA stream */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC2], (uint32_t)&htim->Instance->CCR2, (uint32_t)pData2, Length) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Return error status */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
      /* Enable the TIM Input Capture  DMA request */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC2);          /* HAL辅助宏/函数 */

      /* Enable the Peripheral */
      __HAL_TIM_ENABLE(htim);                           /* HAL辅助宏/函数 */

      /* Enable the Capture compare channel */
      TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_ENABLE);  /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_ALL:                               /* case标签 */
    {                                                   /* 代码块开始 */
      /* Set the DMA capture callbacks */
      htim->hdma[TIM_DMA_ID_CC1]->XferCpltCallback = TIM_DMACaptureCplt;  /* 赋值或初始化 */
      htim->hdma[TIM_DMA_ID_CC1]->XferHalfCpltCallback = TIM_DMACaptureHalfCplt;  /* 赋值或初始化 */

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC1]->XferErrorCallback = TIM_DMAError ;  /* 赋值或初始化 */

      /* Enable the DMA stream */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC1], (uint32_t)&htim->Instance->CCR1, (uint32_t)pData1, Length) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Return error status */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */

      /* Set the DMA capture callbacks */
      htim->hdma[TIM_DMA_ID_CC2]->XferCpltCallback = TIM_DMACaptureCplt;  /* 赋值或初始化 */
      htim->hdma[TIM_DMA_ID_CC2]->XferHalfCpltCallback = TIM_DMACaptureHalfCplt;  /* 赋值或初始化 */

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC2]->XferErrorCallback = TIM_DMAError ;  /* 赋值或初始化 */

      /* Enable the DMA stream */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC2], (uint32_t)&htim->Instance->CCR2, (uint32_t)pData2, Length) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Return error status */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
      /* Enable the Peripheral */
      __HAL_TIM_ENABLE(htim);                           /* HAL辅助宏/函数 */

      /* Enable the Capture compare channel */
      TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);  /* 执行语句 */
      TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_ENABLE);  /* 执行语句 */

      /* Enable the TIM Input Capture  DMA request */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC1);          /* HAL辅助宏/函数 */
      /* Enable the TIM Input Capture  DMA request */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC2);          /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    default:                                            /* default分支 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Stops the TIM Encoder Interface in DMA mode.
  * @param  htim TIM Encoder Interface handle
  * @param  Channel TIM Channels to be enabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_ALL: TIM Channel 1 and TIM Channel 2 are selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_Encoder_Stop_DMA(TIM_HandleTypeDef *htim, uint32_t Channel)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_ENCODER_INTERFACE_INSTANCE(htim->Instance));  /* 执行语句 */

  /* Disable the Input Capture channels 1 and 2
    (in the EncoderInterface the two possible channels that can be used are TIM_CHANNEL_1 and TIM_CHANNEL_2) */
  if (Channel == TIM_CHANNEL_1)                         /* 条件判断 */
  {                                                     /* 代码块开始 */
    TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_DISABLE);  /* 执行语句 */

    /* Disable the capture compare DMA Request 1 */
    __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC1);           /* HAL辅助宏/函数 */
    (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC1]);  /* 执行语句 */
  }                                                     /* 代码块结束 */
  else if (Channel == TIM_CHANNEL_2)                    /* 否则如果 */
  {                                                     /* 代码块开始 */
    TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_DISABLE);  /* 执行语句 */

    /* Disable the capture compare DMA Request 2 */
    __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC2);           /* HAL辅助宏/函数 */
    (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC2]);  /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_DISABLE);  /* 执行语句 */
    TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_DISABLE);  /* 执行语句 */

    /* Disable the capture compare DMA Request 1 and 2 */
    __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC1);           /* HAL辅助宏/函数 */
    __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC2);           /* HAL辅助宏/函数 */
    (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC1]);  /* 执行语句 */
    (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC2]);  /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);                              /* HAL辅助宏/函数 */

  /* Set the TIM channel(s) state */
  if ((Channel == TIM_CHANNEL_1) || (Channel == TIM_CHANNEL_2))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    TIM_CHANNEL_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
    TIM_CHANNEL_N_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
    TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
    TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
    TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */
/** @defgroup TIM_Exported_Functions_Group7 TIM IRQ handler management
  *  @brief    TIM IRQ handler management
  *
@verbatim
  ==============================================================================
                        ##### IRQ handler management #####
  ==============================================================================
  [..]
    This section provides Timer IRQ handler function.

@endverbatim
  * @{
  */
/**
  * @brief  This function handles TIM interrupts requests.
  * @param  htim TIM  handle
  * @retval None
  */
void HAL_TIM_IRQHandler(TIM_HandleTypeDef *htim)        /* 中断服务函数 */
{                                                       /* 代码块开始 */
  /* Capture compare 1 event */
  if (__HAL_TIM_GET_FLAG(htim, TIM_FLAG_CC1) != RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (__HAL_TIM_GET_IT_SOURCE(htim, TIM_IT_CC1) != RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      {                                                 /* 代码块开始 */
        __HAL_TIM_CLEAR_IT(htim, TIM_IT_CC1);           /* HAL辅助宏/函数 */
        htim->Channel = HAL_TIM_ACTIVE_CHANNEL_1;       /* 赋值或初始化 */

        /* Input capture event */
        if ((htim->Instance->CCMR1 & TIM_CCMR1_CC1S) != 0x00U)  /* 条件判断 */
        {                                               /* 代码块开始 */
#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)               /* 条件编译 */
          htim->IC_CaptureCallback(htim);               /* 执行语句 */
#else                                                   /* 条件编译否则 */
          HAL_TIM_IC_CaptureCallback(htim);             /* HAL 定时器操作 */
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
        }                                               /* 代码块结束 */
        /* Output compare event */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)               /* 条件编译 */
          htim->OC_DelayElapsedCallback(htim);          /* 执行语句 */
          htim->PWM_PulseFinishedCallback(htim);        /* 执行语句 */
#else                                                   /* 条件编译否则 */
          HAL_TIM_OC_DelayElapsedCallback(htim);        /* HAL 定时器操作 */
          HAL_TIM_PWM_PulseFinishedCallback(htim);      /* HAL 定时器操作 */
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
        }                                               /* 代码块结束 */
        htim->Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /* Capture compare 2 event */
  if (__HAL_TIM_GET_FLAG(htim, TIM_FLAG_CC2) != RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (__HAL_TIM_GET_IT_SOURCE(htim, TIM_IT_CC2) != RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      __HAL_TIM_CLEAR_IT(htim, TIM_IT_CC2);             /* HAL辅助宏/函数 */
      htim->Channel = HAL_TIM_ACTIVE_CHANNEL_2;         /* 赋值或初始化 */
      /* Input capture event */
      if ((htim->Instance->CCMR1 & TIM_CCMR1_CC2S) != 0x00U)  /* 条件判断 */
      {                                                 /* 代码块开始 */
#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)               /* 条件编译 */
        htim->IC_CaptureCallback(htim);                 /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_TIM_IC_CaptureCallback(htim);               /* HAL 定时器操作 */
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
      }                                                 /* 代码块结束 */
      /* Output compare event */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)               /* 条件编译 */
        htim->OC_DelayElapsedCallback(htim);            /* 执行语句 */
        htim->PWM_PulseFinishedCallback(htim);          /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_TIM_OC_DelayElapsedCallback(htim);          /* HAL 定时器操作 */
        HAL_TIM_PWM_PulseFinishedCallback(htim);        /* HAL 定时器操作 */
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
      }                                                 /* 代码块结束 */
      htim->Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;   /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /* Capture compare 3 event */
  if (__HAL_TIM_GET_FLAG(htim, TIM_FLAG_CC3) != RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (__HAL_TIM_GET_IT_SOURCE(htim, TIM_IT_CC3) != RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      __HAL_TIM_CLEAR_IT(htim, TIM_IT_CC3);             /* HAL辅助宏/函数 */
      htim->Channel = HAL_TIM_ACTIVE_CHANNEL_3;         /* 赋值或初始化 */
      /* Input capture event */
      if ((htim->Instance->CCMR2 & TIM_CCMR2_CC3S) != 0x00U)  /* 条件判断 */
      {                                                 /* 代码块开始 */
#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)               /* 条件编译 */
        htim->IC_CaptureCallback(htim);                 /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_TIM_IC_CaptureCallback(htim);               /* HAL 定时器操作 */
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
      }                                                 /* 代码块结束 */
      /* Output compare event */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)               /* 条件编译 */
        htim->OC_DelayElapsedCallback(htim);            /* 执行语句 */
        htim->PWM_PulseFinishedCallback(htim);          /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_TIM_OC_DelayElapsedCallback(htim);          /* HAL 定时器操作 */
        HAL_TIM_PWM_PulseFinishedCallback(htim);        /* HAL 定时器操作 */
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
      }                                                 /* 代码块结束 */
      htim->Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;   /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /* Capture compare 4 event */
  if (__HAL_TIM_GET_FLAG(htim, TIM_FLAG_CC4) != RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (__HAL_TIM_GET_IT_SOURCE(htim, TIM_IT_CC4) != RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      __HAL_TIM_CLEAR_IT(htim, TIM_IT_CC4);             /* HAL辅助宏/函数 */
      htim->Channel = HAL_TIM_ACTIVE_CHANNEL_4;         /* 赋值或初始化 */
      /* Input capture event */
      if ((htim->Instance->CCMR2 & TIM_CCMR2_CC4S) != 0x00U)  /* 条件判断 */
      {                                                 /* 代码块开始 */
#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)               /* 条件编译 */
        htim->IC_CaptureCallback(htim);                 /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_TIM_IC_CaptureCallback(htim);               /* HAL 定时器操作 */
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
      }                                                 /* 代码块结束 */
      /* Output compare event */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)               /* 条件编译 */
        htim->OC_DelayElapsedCallback(htim);            /* 执行语句 */
        htim->PWM_PulseFinishedCallback(htim);          /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_TIM_OC_DelayElapsedCallback(htim);          /* HAL 定时器操作 */
        HAL_TIM_PWM_PulseFinishedCallback(htim);        /* HAL 定时器操作 */
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
      }                                                 /* 代码块结束 */
      htim->Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;   /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /* TIM Update event */
  if (__HAL_TIM_GET_FLAG(htim, TIM_FLAG_UPDATE) != RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (__HAL_TIM_GET_IT_SOURCE(htim, TIM_IT_UPDATE) != RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      __HAL_TIM_CLEAR_IT(htim, TIM_IT_UPDATE);          /* HAL辅助宏/函数 */
#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)               /* 条件编译 */
      htim->PeriodElapsedCallback(htim);                /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_TIM_PeriodElapsedCallback(htim);              /* HAL 定时器操作 */
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /* TIM Break input event */
  if (__HAL_TIM_GET_FLAG(htim, TIM_FLAG_BREAK) != RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (__HAL_TIM_GET_IT_SOURCE(htim, TIM_IT_BREAK) != RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      __HAL_TIM_CLEAR_IT(htim, TIM_IT_BREAK);           /* HAL辅助宏/函数 */
#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)               /* 条件编译 */
      htim->BreakCallback(htim);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_TIMEx_BreakCallback(htim);                    /* HAL库函数调用 */
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /* TIM Trigger detection event */
  if (__HAL_TIM_GET_FLAG(htim, TIM_FLAG_TRIGGER) != RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (__HAL_TIM_GET_IT_SOURCE(htim, TIM_IT_TRIGGER) != RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      __HAL_TIM_CLEAR_IT(htim, TIM_IT_TRIGGER);         /* HAL辅助宏/函数 */
#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)               /* 条件编译 */
      htim->TriggerCallback(htim);                      /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_TIM_TriggerCallback(htim);                    /* HAL 定时器操作 */
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /* TIM commutation event */
  if (__HAL_TIM_GET_FLAG(htim, TIM_FLAG_COM) != RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (__HAL_TIM_GET_IT_SOURCE(htim, TIM_IT_COM) != RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      __HAL_TIM_CLEAR_IT(htim, TIM_FLAG_COM);           /* HAL辅助宏/函数 */
#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)               /* 条件编译 */
      htim->CommutationCallback(htim);                  /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_TIMEx_CommutCallback(htim);                   /* HAL库函数调用 */
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup TIM_Exported_Functions_Group8 TIM Peripheral Control functions
  *  @brief    TIM Peripheral Control functions
  *
@verbatim
  ==============================================================================
                   ##### Peripheral Control functions #####
  ==============================================================================
 [..]
   This section provides functions allowing to:
      (+) Configure The Input Output channels for OC, PWM, IC or One Pulse mode.
      (+) Configure External Clock source.
      (+) Configure Complementary channels, break features and dead time.
      (+) Configure Master and the Slave synchronization.
      (+) Configure the DMA Burst Mode.

@endverbatim
  * @{
  */

/**
  * @brief  Initializes the TIM Output Compare Channels according to the specified
  *         parameters in the TIM_OC_InitTypeDef.
  * @param  htim TIM Output Compare handle
  * @param  sConfig TIM Output Compare configuration structure
  * @param  Channel TIM Channels to configure
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_OC_ConfigChannel(TIM_HandleTypeDef *htim,  /* HAL状态类型函数 */
                                           TIM_OC_InitTypeDef *sConfig,  /* 列表续行 */
                                           uint32_t Channel)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_CHANNELS(Channel));               /* 执行语句 */
  assert_param(IS_TIM_OC_MODE(sConfig->OCMode));        /* 执行语句 */
  assert_param(IS_TIM_OC_POLARITY(sConfig->OCPolarity));  /* 执行语句 */

  /* Process Locked */
  __HAL_LOCK(htim);                                     /* HAL辅助宏/函数 */

  switch (Channel)                                      /* switch分支 */
  {                                                     /* 代码块开始 */
    case TIM_CHANNEL_1:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Check the parameters */
      assert_param(IS_TIM_CC1_INSTANCE(htim->Instance));  /* 执行语句 */

      /* Configure the TIM Channel 1 in Output Compare */
      TIM_OC1_SetConfig(htim->Instance, sConfig);       /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_2:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Check the parameters */
      assert_param(IS_TIM_CC2_INSTANCE(htim->Instance));  /* 执行语句 */

      /* Configure the TIM Channel 2 in Output Compare */
      TIM_OC2_SetConfig(htim->Instance, sConfig);       /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_3:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Check the parameters */
      assert_param(IS_TIM_CC3_INSTANCE(htim->Instance));  /* 执行语句 */

      /* Configure the TIM Channel 3 in Output Compare */
      TIM_OC3_SetConfig(htim->Instance, sConfig);       /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_4:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Check the parameters */
      assert_param(IS_TIM_CC4_INSTANCE(htim->Instance));  /* 执行语句 */

      /* Configure the TIM Channel 4 in Output Compare */
      TIM_OC4_SetConfig(htim->Instance, sConfig);       /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    default:                                            /* default分支 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  __HAL_UNLOCK(htim);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initializes the TIM Input Capture Channels according to the specified
  *         parameters in the TIM_IC_InitTypeDef.
  * @param  htim TIM IC handle
  * @param  sConfig TIM Input Capture configuration structure
  * @param  Channel TIM Channel to configure
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_IC_ConfigChannel(TIM_HandleTypeDef *htim, TIM_IC_InitTypeDef *sConfig, uint32_t Channel)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_CC1_INSTANCE(htim->Instance));    /* 执行语句 */
  assert_param(IS_TIM_IC_POLARITY(sConfig->ICPolarity));  /* 执行语句 */
  assert_param(IS_TIM_IC_SELECTION(sConfig->ICSelection));  /* 执行语句 */
  assert_param(IS_TIM_IC_PRESCALER(sConfig->ICPrescaler));  /* 执行语句 */
  assert_param(IS_TIM_IC_FILTER(sConfig->ICFilter));    /* 执行语句 */

  /* Process Locked */
  __HAL_LOCK(htim);                                     /* HAL辅助宏/函数 */

  if (Channel == TIM_CHANNEL_1)                         /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* TI1 Configuration */
    TIM_TI1_SetConfig(htim->Instance,                   /* 列表续行 */
                      sConfig->ICPolarity,              /* 列表续行 */
                      sConfig->ICSelection,             /* 列表续行 */
                      sConfig->ICFilter);               /* 执行语句 */

    /* Reset the IC1PSC Bits */
    htim->Instance->CCMR1 &= ~TIM_CCMR1_IC1PSC;         /* 赋值或初始化 */

    /* Set the IC1PSC value */
    htim->Instance->CCMR1 |= sConfig->ICPrescaler;      /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if (Channel == TIM_CHANNEL_2)                    /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* TI2 Configuration */
    assert_param(IS_TIM_CC2_INSTANCE(htim->Instance));  /* 执行语句 */

    TIM_TI2_SetConfig(htim->Instance,                   /* 列表续行 */
                      sConfig->ICPolarity,              /* 列表续行 */
                      sConfig->ICSelection,             /* 列表续行 */
                      sConfig->ICFilter);               /* 执行语句 */

    /* Reset the IC2PSC Bits */
    htim->Instance->CCMR1 &= ~TIM_CCMR1_IC2PSC;         /* 赋值或初始化 */

    /* Set the IC2PSC value */
    htim->Instance->CCMR1 |= (sConfig->ICPrescaler << 8U);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if (Channel == TIM_CHANNEL_3)                    /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* TI3 Configuration */
    assert_param(IS_TIM_CC3_INSTANCE(htim->Instance));  /* 执行语句 */

    TIM_TI3_SetConfig(htim->Instance,                   /* 列表续行 */
                      sConfig->ICPolarity,              /* 列表续行 */
                      sConfig->ICSelection,             /* 列表续行 */
                      sConfig->ICFilter);               /* 执行语句 */

    /* Reset the IC3PSC Bits */
    htim->Instance->CCMR2 &= ~TIM_CCMR2_IC3PSC;         /* 赋值或初始化 */

    /* Set the IC3PSC value */
    htim->Instance->CCMR2 |= sConfig->ICPrescaler;      /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* TI4 Configuration */
    assert_param(IS_TIM_CC4_INSTANCE(htim->Instance));  /* 执行语句 */

    TIM_TI4_SetConfig(htim->Instance,                   /* 列表续行 */
                      sConfig->ICPolarity,              /* 列表续行 */
                      sConfig->ICSelection,             /* 列表续行 */
                      sConfig->ICFilter);               /* 执行语句 */

    /* Reset the IC4PSC Bits */
    htim->Instance->CCMR2 &= ~TIM_CCMR2_IC4PSC;         /* 赋值或初始化 */

    /* Set the IC4PSC value */
    htim->Instance->CCMR2 |= (sConfig->ICPrescaler << 8U);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  __HAL_UNLOCK(htim);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initializes the TIM PWM  channels according to the specified
  *         parameters in the TIM_OC_InitTypeDef.
  * @param  htim TIM PWM handle
  * @param  sConfig TIM PWM configuration structure
  * @param  Channel TIM Channels to be configured
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_PWM_ConfigChannel(TIM_HandleTypeDef *htim,  /* HAL状态类型函数 */
                                            TIM_OC_InitTypeDef *sConfig,  /* 列表续行 */
                                            uint32_t Channel)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_CHANNELS(Channel));               /* 执行语句 */
  assert_param(IS_TIM_PWM_MODE(sConfig->OCMode));       /* 执行语句 */
  assert_param(IS_TIM_OC_POLARITY(sConfig->OCPolarity));  /* 执行语句 */
  assert_param(IS_TIM_FAST_STATE(sConfig->OCFastMode));  /* 执行语句 */

  /* Process Locked */
  __HAL_LOCK(htim);                                     /* HAL辅助宏/函数 */

  switch (Channel)                                      /* switch分支 */
  {                                                     /* 代码块开始 */
    case TIM_CHANNEL_1:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Check the parameters */
      assert_param(IS_TIM_CC1_INSTANCE(htim->Instance));  /* 执行语句 */

      /* Configure the Channel 1 in PWM mode */
      TIM_OC1_SetConfig(htim->Instance, sConfig);       /* 执行语句 */

      /* Set the Preload enable bit for channel1 */
      htim->Instance->CCMR1 |= TIM_CCMR1_OC1PE;         /* 赋值或初始化 */

      /* Configure the Output Fast mode */
      htim->Instance->CCMR1 &= ~TIM_CCMR1_OC1FE;        /* 赋值或初始化 */
      htim->Instance->CCMR1 |= sConfig->OCFastMode;     /* 赋值或初始化 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_2:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Check the parameters */
      assert_param(IS_TIM_CC2_INSTANCE(htim->Instance));  /* 执行语句 */

      /* Configure the Channel 2 in PWM mode */
      TIM_OC2_SetConfig(htim->Instance, sConfig);       /* 执行语句 */

      /* Set the Preload enable bit for channel2 */
      htim->Instance->CCMR1 |= TIM_CCMR1_OC2PE;         /* 赋值或初始化 */

      /* Configure the Output Fast mode */
      htim->Instance->CCMR1 &= ~TIM_CCMR1_OC2FE;        /* 赋值或初始化 */
      htim->Instance->CCMR1 |= sConfig->OCFastMode << 8U;  /* 赋值或初始化 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_3:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Check the parameters */
      assert_param(IS_TIM_CC3_INSTANCE(htim->Instance));  /* 执行语句 */

      /* Configure the Channel 3 in PWM mode */
      TIM_OC3_SetConfig(htim->Instance, sConfig);       /* 执行语句 */

      /* Set the Preload enable bit for channel3 */
      htim->Instance->CCMR2 |= TIM_CCMR2_OC3PE;         /* 赋值或初始化 */

      /* Configure the Output Fast mode */
      htim->Instance->CCMR2 &= ~TIM_CCMR2_OC3FE;        /* 赋值或初始化 */
      htim->Instance->CCMR2 |= sConfig->OCFastMode;     /* 赋值或初始化 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_4:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Check the parameters */
      assert_param(IS_TIM_CC4_INSTANCE(htim->Instance));  /* 执行语句 */

      /* Configure the Channel 4 in PWM mode */
      TIM_OC4_SetConfig(htim->Instance, sConfig);       /* 执行语句 */

      /* Set the Preload enable bit for channel4 */
      htim->Instance->CCMR2 |= TIM_CCMR2_OC4PE;         /* 赋值或初始化 */

      /* Configure the Output Fast mode */
      htim->Instance->CCMR2 &= ~TIM_CCMR2_OC4FE;        /* 赋值或初始化 */
      htim->Instance->CCMR2 |= sConfig->OCFastMode << 8U;  /* 赋值或初始化 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    default:                                            /* default分支 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  __HAL_UNLOCK(htim);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initializes the TIM One Pulse Channels according to the specified
  *         parameters in the TIM_OnePulse_InitTypeDef.
  * @param  htim TIM One Pulse handle
  * @param  sConfig TIM One Pulse configuration structure
  * @param  OutputChannel TIM output channel to configure
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  * @param  InputChannel TIM input Channel to configure
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  * @note  To output a waveform with a minimum delay user can enable the fast
  *        mode by calling the @ref __HAL_TIM_ENABLE_OCxFAST macro. Then CCx
  *        output is forced in response to the edge detection on TIx input,
  *        without taking in account the comparison.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_OnePulse_ConfigChannel(TIM_HandleTypeDef *htim,  TIM_OnePulse_InitTypeDef *sConfig,  /* HAL状态类型函数 */
                                                 uint32_t OutputChannel,  uint32_t InputChannel)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  TIM_OC_InitTypeDef temp1;                             /* 执行语句 */

  /* Check the parameters */
  assert_param(IS_TIM_OPM_CHANNELS(OutputChannel));     /* 执行语句 */
  assert_param(IS_TIM_OPM_CHANNELS(InputChannel));      /* 执行语句 */

  if (OutputChannel != InputChannel)                    /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Process Locked */
    __HAL_LOCK(htim);                                   /* HAL辅助宏/函数 */

    htim->State = HAL_TIM_STATE_BUSY;                   /* 赋值或初始化 */

    /* Extract the Output compare configuration from sConfig structure */
    temp1.OCMode = sConfig->OCMode;                     /* 赋值或初始化 */
    temp1.Pulse = sConfig->Pulse;                       /* 赋值或初始化 */
    temp1.OCPolarity = sConfig->OCPolarity;             /* 赋值或初始化 */
    temp1.OCNPolarity = sConfig->OCNPolarity;           /* 赋值或初始化 */
    temp1.OCIdleState = sConfig->OCIdleState;           /* 赋值或初始化 */
    temp1.OCNIdleState = sConfig->OCNIdleState;         /* 赋值或初始化 */

    switch (OutputChannel)                              /* switch分支 */
    {                                                   /* 代码块开始 */
      case TIM_CHANNEL_1:                               /* case标签 */
      {                                                 /* 代码块开始 */
        assert_param(IS_TIM_CC1_INSTANCE(htim->Instance));  /* 执行语句 */

        TIM_OC1_SetConfig(htim->Instance, &temp1);      /* 执行语句 */
        break;                                          /* 跳出 */
      }                                                 /* 代码块结束 */
      case TIM_CHANNEL_2:                               /* case标签 */
      {                                                 /* 代码块开始 */
        assert_param(IS_TIM_CC2_INSTANCE(htim->Instance));  /* 执行语句 */

        TIM_OC2_SetConfig(htim->Instance, &temp1);      /* 执行语句 */
        break;                                          /* 跳出 */
      }                                                 /* 代码块结束 */
      default:                                          /* default分支 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */

    switch (InputChannel)                               /* switch分支 */
    {                                                   /* 代码块开始 */
      case TIM_CHANNEL_1:                               /* case标签 */
      {                                                 /* 代码块开始 */
        assert_param(IS_TIM_CC1_INSTANCE(htim->Instance));  /* 执行语句 */

        TIM_TI1_SetConfig(htim->Instance, sConfig->ICPolarity,  /* 列表续行 */
                          sConfig->ICSelection, sConfig->ICFilter);  /* 执行语句 */

        /* Reset the IC1PSC Bits */
        htim->Instance->CCMR1 &= ~TIM_CCMR1_IC1PSC;     /* 赋值或初始化 */

        /* Select the Trigger source */
        htim->Instance->SMCR &= ~TIM_SMCR_TS;           /* 赋值或初始化 */
        htim->Instance->SMCR |= TIM_TS_TI1FP1;          /* 赋值或初始化 */

        /* Select the Slave Mode */
        htim->Instance->SMCR &= ~TIM_SMCR_SMS;          /* 赋值或初始化 */
        htim->Instance->SMCR |= TIM_SLAVEMODE_TRIGGER;  /* 赋值或初始化 */
        break;                                          /* 跳出 */
      }                                                 /* 代码块结束 */
      case TIM_CHANNEL_2:                               /* case标签 */
      {                                                 /* 代码块开始 */
        assert_param(IS_TIM_CC2_INSTANCE(htim->Instance));  /* 执行语句 */

        TIM_TI2_SetConfig(htim->Instance, sConfig->ICPolarity,  /* 列表续行 */
                          sConfig->ICSelection, sConfig->ICFilter);  /* 执行语句 */

        /* Reset the IC2PSC Bits */
        htim->Instance->CCMR1 &= ~TIM_CCMR1_IC2PSC;     /* 赋值或初始化 */

        /* Select the Trigger source */
        htim->Instance->SMCR &= ~TIM_SMCR_TS;           /* 赋值或初始化 */
        htim->Instance->SMCR |= TIM_TS_TI2FP2;          /* 赋值或初始化 */

        /* Select the Slave Mode */
        htim->Instance->SMCR &= ~TIM_SMCR_SMS;          /* 赋值或初始化 */
        htim->Instance->SMCR |= TIM_SLAVEMODE_TRIGGER;  /* 赋值或初始化 */
        break;                                          /* 跳出 */
      }                                                 /* 代码块结束 */

      default:                                          /* default分支 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */

    htim->State = HAL_TIM_STATE_READY;                  /* 赋值或初始化 */

    __HAL_UNLOCK(htim);                                 /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configure the DMA Burst to transfer Data from the memory to the TIM peripheral
  * @param  htim TIM handle
  * @param  BurstBaseAddress TIM Base address from where the DMA  will start the Data write
  *         This parameter can be one of the following values:
  *            @arg TIM_DMABASE_CR1
  *            @arg TIM_DMABASE_CR2
  *            @arg TIM_DMABASE_SMCR
  *            @arg TIM_DMABASE_DIER
  *            @arg TIM_DMABASE_SR
  *            @arg TIM_DMABASE_EGR
  *            @arg TIM_DMABASE_CCMR1
  *            @arg TIM_DMABASE_CCMR2
  *            @arg TIM_DMABASE_CCER
  *            @arg TIM_DMABASE_CNT
  *            @arg TIM_DMABASE_PSC
  *            @arg TIM_DMABASE_ARR
  *            @arg TIM_DMABASE_RCR
  *            @arg TIM_DMABASE_CCR1
  *            @arg TIM_DMABASE_CCR2
  *            @arg TIM_DMABASE_CCR3
  *            @arg TIM_DMABASE_CCR4
  *            @arg TIM_DMABASE_BDTR
  * @param  BurstRequestSrc TIM DMA Request sources
  *         This parameter can be one of the following values:
  *            @arg TIM_DMA_UPDATE: TIM update Interrupt source
  *            @arg TIM_DMA_CC1: TIM Capture Compare 1 DMA source
  *            @arg TIM_DMA_CC2: TIM Capture Compare 2 DMA source
  *            @arg TIM_DMA_CC3: TIM Capture Compare 3 DMA source
  *            @arg TIM_DMA_CC4: TIM Capture Compare 4 DMA source
  *            @arg TIM_DMA_COM: TIM Commutation DMA source
  *            @arg TIM_DMA_TRIGGER: TIM Trigger DMA source
  * @param  BurstBuffer The Buffer address.
  * @param  BurstLength DMA Burst length. This parameter can be one value
  *         between: TIM_DMABURSTLENGTH_1TRANSFER and TIM_DMABURSTLENGTH_18TRANSFERS.
  * @note   This function should be used only when BurstLength is equal to DMA data transfer length.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_DMABurst_WriteStart(TIM_HandleTypeDef *htim, uint32_t BurstBaseAddress,  /* HAL状态类型函数 */
                                              uint32_t BurstRequestSrc, uint32_t *BurstBuffer, uint32_t  BurstLength)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  return HAL_TIM_DMABurst_MultiWriteStart(htim, BurstBaseAddress, BurstRequestSrc, BurstBuffer, BurstLength,  /* 返回值 */
                                          ((BurstLength) >> 8U) + 1U);  /* 执行语句 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configure the DMA Burst to transfer multiple Data from the memory to the TIM peripheral
  * @param  htim TIM handle
  * @param  BurstBaseAddress TIM Base address from where the DMA will start the Data write
  *         This parameter can be one of the following values:
  *            @arg TIM_DMABASE_CR1
  *            @arg TIM_DMABASE_CR2
  *            @arg TIM_DMABASE_SMCR
  *            @arg TIM_DMABASE_DIER
  *            @arg TIM_DMABASE_SR
  *            @arg TIM_DMABASE_EGR
  *            @arg TIM_DMABASE_CCMR1
  *            @arg TIM_DMABASE_CCMR2
  *            @arg TIM_DMABASE_CCER
  *            @arg TIM_DMABASE_CNT
  *            @arg TIM_DMABASE_PSC
  *            @arg TIM_DMABASE_ARR
  *            @arg TIM_DMABASE_RCR
  *            @arg TIM_DMABASE_CCR1
  *            @arg TIM_DMABASE_CCR2
  *            @arg TIM_DMABASE_CCR3
  *            @arg TIM_DMABASE_CCR4
  *            @arg TIM_DMABASE_BDTR
  * @param  BurstRequestSrc TIM DMA Request sources
  *         This parameter can be one of the following values:
  *            @arg TIM_DMA_UPDATE: TIM update Interrupt source
  *            @arg TIM_DMA_CC1: TIM Capture Compare 1 DMA source
  *            @arg TIM_DMA_CC2: TIM Capture Compare 2 DMA source
  *            @arg TIM_DMA_CC3: TIM Capture Compare 3 DMA source
  *            @arg TIM_DMA_CC4: TIM Capture Compare 4 DMA source
  *            @arg TIM_DMA_COM: TIM Commutation DMA source
  *            @arg TIM_DMA_TRIGGER: TIM Trigger DMA source
  * @param  BurstBuffer The Buffer address.
  * @param  BurstLength DMA Burst length. This parameter can be one value
  *         between: TIM_DMABURSTLENGTH_1TRANSFER and TIM_DMABURSTLENGTH_18TRANSFERS.
  * @param  DataLength Data length. This parameter can be one value
  *         between 1 and 0xFFFF.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_DMABurst_MultiWriteStart(TIM_HandleTypeDef *htim, uint32_t BurstBaseAddress,  /* HAL状态类型函数 */
                                                   uint32_t BurstRequestSrc, uint32_t *BurstBuffer,  /* 32位无符号变量/函数 */
                                                   uint32_t  BurstLength,  uint32_t  DataLength)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_DMABURST_INSTANCE(htim->Instance));  /* 执行语句 */
  assert_param(IS_TIM_DMA_BASE(BurstBaseAddress));      /* 执行语句 */
  assert_param(IS_TIM_DMA_SOURCE(BurstRequestSrc));     /* 执行语句 */
  assert_param(IS_TIM_DMA_LENGTH(BurstLength));         /* 执行语句 */
  assert_param(IS_TIM_DMA_DATA_LENGTH(DataLength));     /* 执行语句 */

  if (htim->DMABurstState == HAL_DMA_BURST_STATE_BUSY)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
  else if (htim->DMABurstState == HAL_DMA_BURST_STATE_READY)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    if ((BurstBuffer == NULL) && (BurstLength > 0U))    /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      htim->DMABurstState = HAL_DMA_BURST_STATE_BUSY;   /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* nothing to do */
  }                                                     /* 代码块结束 */
  switch (BurstRequestSrc)                              /* switch分支 */
  {                                                     /* 代码块开始 */
    case TIM_DMA_UPDATE:                                /* case标签 */
    {                                                   /* 代码块开始 */
      /* Set the DMA Period elapsed callbacks */
      htim->hdma[TIM_DMA_ID_UPDATE]->XferCpltCallback = TIM_DMAPeriodElapsedCplt;  /* 赋值或初始化 */
      htim->hdma[TIM_DMA_ID_UPDATE]->XferHalfCpltCallback = TIM_DMAPeriodElapsedHalfCplt;  /* 赋值或初始化 */

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_UPDATE]->XferErrorCallback = TIM_DMAError ;  /* 赋值或初始化 */

      /* Enable the DMA stream */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_UPDATE], (uint32_t)BurstBuffer,  /* 条件判断 */
                         (uint32_t)&htim->Instance->DMAR, DataLength) != HAL_OK)  /* 源代码 */
      {                                                 /* 代码块开始 */
        /* Return error status */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
    case TIM_DMA_CC1:                                   /* case标签 */
    {                                                   /* 代码块开始 */
      /* Set the DMA compare callbacks */
      htim->hdma[TIM_DMA_ID_CC1]->XferCpltCallback = TIM_DMADelayPulseCplt;  /* 赋值或初始化 */
      htim->hdma[TIM_DMA_ID_CC1]->XferHalfCpltCallback = TIM_DMADelayPulseHalfCplt;  /* 赋值或初始化 */

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC1]->XferErrorCallback = TIM_DMAError ;  /* 赋值或初始化 */

      /* Enable the DMA stream */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC1], (uint32_t)BurstBuffer,  /* 条件判断 */
                         (uint32_t)&htim->Instance->DMAR, DataLength) != HAL_OK)  /* 源代码 */
      {                                                 /* 代码块开始 */
        /* Return error status */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
    case TIM_DMA_CC2:                                   /* case标签 */
    {                                                   /* 代码块开始 */
      /* Set the DMA compare callbacks */
      htim->hdma[TIM_DMA_ID_CC2]->XferCpltCallback = TIM_DMADelayPulseCplt;  /* 赋值或初始化 */
      htim->hdma[TIM_DMA_ID_CC2]->XferHalfCpltCallback = TIM_DMADelayPulseHalfCplt;  /* 赋值或初始化 */

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC2]->XferErrorCallback = TIM_DMAError ;  /* 赋值或初始化 */

      /* Enable the DMA stream */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC2], (uint32_t)BurstBuffer,  /* 条件判断 */
                         (uint32_t)&htim->Instance->DMAR, DataLength) != HAL_OK)  /* 源代码 */
      {                                                 /* 代码块开始 */
        /* Return error status */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
    case TIM_DMA_CC3:                                   /* case标签 */
    {                                                   /* 代码块开始 */
      /* Set the DMA compare callbacks */
      htim->hdma[TIM_DMA_ID_CC3]->XferCpltCallback = TIM_DMADelayPulseCplt;  /* 赋值或初始化 */
      htim->hdma[TIM_DMA_ID_CC3]->XferHalfCpltCallback = TIM_DMADelayPulseHalfCplt;  /* 赋值或初始化 */

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC3]->XferErrorCallback = TIM_DMAError ;  /* 赋值或初始化 */

      /* Enable the DMA stream */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC3], (uint32_t)BurstBuffer,  /* 条件判断 */
                         (uint32_t)&htim->Instance->DMAR, DataLength) != HAL_OK)  /* 源代码 */
      {                                                 /* 代码块开始 */
        /* Return error status */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
    case TIM_DMA_CC4:                                   /* case标签 */
    {                                                   /* 代码块开始 */
      /* Set the DMA compare callbacks */
      htim->hdma[TIM_DMA_ID_CC4]->XferCpltCallback = TIM_DMADelayPulseCplt;  /* 赋值或初始化 */
      htim->hdma[TIM_DMA_ID_CC4]->XferHalfCpltCallback = TIM_DMADelayPulseHalfCplt;  /* 赋值或初始化 */

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC4]->XferErrorCallback = TIM_DMAError ;  /* 赋值或初始化 */

      /* Enable the DMA stream */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC4], (uint32_t)BurstBuffer,  /* 条件判断 */
                         (uint32_t)&htim->Instance->DMAR, DataLength) != HAL_OK)  /* 源代码 */
      {                                                 /* 代码块开始 */
        /* Return error status */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
    case TIM_DMA_COM:                                   /* case标签 */
    {                                                   /* 代码块开始 */
      /* Set the DMA commutation callbacks */
      htim->hdma[TIM_DMA_ID_COMMUTATION]->XferCpltCallback =  TIMEx_DMACommutationCplt;  /* 赋值或初始化 */
      htim->hdma[TIM_DMA_ID_COMMUTATION]->XferHalfCpltCallback =  TIMEx_DMACommutationHalfCplt;  /* 赋值或初始化 */

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_COMMUTATION]->XferErrorCallback = TIM_DMAError ;  /* 赋值或初始化 */

      /* Enable the DMA stream */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_COMMUTATION], (uint32_t)BurstBuffer,  /* 条件判断 */
                         (uint32_t)&htim->Instance->DMAR, DataLength) != HAL_OK)  /* 源代码 */
      {                                                 /* 代码块开始 */
        /* Return error status */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
    case TIM_DMA_TRIGGER:                               /* case标签 */
    {                                                   /* 代码块开始 */
      /* Set the DMA trigger callbacks */
      htim->hdma[TIM_DMA_ID_TRIGGER]->XferCpltCallback = TIM_DMATriggerCplt;  /* 赋值或初始化 */
      htim->hdma[TIM_DMA_ID_TRIGGER]->XferHalfCpltCallback = TIM_DMATriggerHalfCplt;  /* 赋值或初始化 */

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_TRIGGER]->XferErrorCallback = TIM_DMAError ;  /* 赋值或初始化 */

      /* Enable the DMA stream */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_TRIGGER], (uint32_t)BurstBuffer,  /* 条件判断 */
                         (uint32_t)&htim->Instance->DMAR, DataLength) != HAL_OK)  /* 源代码 */
      {                                                 /* 代码块开始 */
        /* Return error status */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
    default:                                            /* default分支 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  /* Configure the DMA Burst Mode */
  htim->Instance->DCR = (BurstBaseAddress | BurstLength);  /* 赋值或初始化 */
  /* Enable the TIM DMA Request */
  __HAL_TIM_ENABLE_DMA(htim, BurstRequestSrc);          /* HAL辅助宏/函数 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Stops the TIM DMA Burst mode
  * @param  htim TIM handle
  * @param  BurstRequestSrc TIM DMA Request sources to disable
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_DMABurst_WriteStop(TIM_HandleTypeDef *htim, uint32_t BurstRequestSrc)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_DMA_SOURCE(BurstRequestSrc));     /* 执行语句 */

  /* Abort the DMA transfer (at least disable the DMA stream) */
  switch (BurstRequestSrc)                              /* switch分支 */
  {                                                     /* 代码块开始 */
    case TIM_DMA_UPDATE:                                /* case标签 */
    {                                                   /* 代码块开始 */
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_UPDATE]);  /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
    case TIM_DMA_CC1:                                   /* case标签 */
    {                                                   /* 代码块开始 */
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC1]);  /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
    case TIM_DMA_CC2:                                   /* case标签 */
    {                                                   /* 代码块开始 */
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC2]);  /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
    case TIM_DMA_CC3:                                   /* case标签 */
    {                                                   /* 代码块开始 */
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC3]);  /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
    case TIM_DMA_CC4:                                   /* case标签 */
    {                                                   /* 代码块开始 */
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC4]);  /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
    case TIM_DMA_COM:                                   /* case标签 */
    {                                                   /* 代码块开始 */
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_COMMUTATION]);  /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
    case TIM_DMA_TRIGGER:                               /* case标签 */
    {                                                   /* 代码块开始 */
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_TRIGGER]);  /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
    default:                                            /* default分支 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  /* Disable the TIM Update DMA request */
  __HAL_TIM_DISABLE_DMA(htim, BurstRequestSrc);         /* HAL辅助宏/函数 */

  /* Change the DMA burst operation state */
  htim->DMABurstState = HAL_DMA_BURST_STATE_READY;      /* 赋值或初始化 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configure the DMA Burst to transfer Data from the TIM peripheral to the memory
  * @param  htim TIM handle
  * @param  BurstBaseAddress TIM Base address from where the DMA  will start the Data read
  *         This parameter can be one of the following values:
  *            @arg TIM_DMABASE_CR1
  *            @arg TIM_DMABASE_CR2
  *            @arg TIM_DMABASE_SMCR
  *            @arg TIM_DMABASE_DIER
  *            @arg TIM_DMABASE_SR
  *            @arg TIM_DMABASE_EGR
  *            @arg TIM_DMABASE_CCMR1
  *            @arg TIM_DMABASE_CCMR2
  *            @arg TIM_DMABASE_CCER
  *            @arg TIM_DMABASE_CNT
  *            @arg TIM_DMABASE_PSC
  *            @arg TIM_DMABASE_ARR
  *            @arg TIM_DMABASE_RCR
  *            @arg TIM_DMABASE_CCR1
  *            @arg TIM_DMABASE_CCR2
  *            @arg TIM_DMABASE_CCR3
  *            @arg TIM_DMABASE_CCR4
  *            @arg TIM_DMABASE_BDTR
  * @param  BurstRequestSrc TIM DMA Request sources
  *         This parameter can be one of the following values:
  *            @arg TIM_DMA_UPDATE: TIM update Interrupt source
  *            @arg TIM_DMA_CC1: TIM Capture Compare 1 DMA source
  *            @arg TIM_DMA_CC2: TIM Capture Compare 2 DMA source
  *            @arg TIM_DMA_CC3: TIM Capture Compare 3 DMA source
  *            @arg TIM_DMA_CC4: TIM Capture Compare 4 DMA source
  *            @arg TIM_DMA_COM: TIM Commutation DMA source
  *            @arg TIM_DMA_TRIGGER: TIM Trigger DMA source
  * @param  BurstBuffer The Buffer address.
  * @param  BurstLength DMA Burst length. This parameter can be one value
  *         between: TIM_DMABURSTLENGTH_1TRANSFER and TIM_DMABURSTLENGTH_18TRANSFERS.
  * @note   This function should be used only when BurstLength is equal to DMA data transfer length.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_DMABurst_ReadStart(TIM_HandleTypeDef *htim, uint32_t BurstBaseAddress,  /* HAL状态类型函数 */
                                             uint32_t BurstRequestSrc, uint32_t  *BurstBuffer, uint32_t  BurstLength)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  return HAL_TIM_DMABurst_MultiReadStart(htim, BurstBaseAddress, BurstRequestSrc, BurstBuffer, BurstLength,  /* 返回值 */
                                         ((BurstLength) >> 8U) + 1U);  /* 执行语句 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configure the DMA Burst to transfer Data from the TIM peripheral to the memory
  * @param  htim TIM handle
  * @param  BurstBaseAddress TIM Base address from where the DMA  will start the Data read
  *         This parameter can be one of the following values:
  *            @arg TIM_DMABASE_CR1
  *            @arg TIM_DMABASE_CR2
  *            @arg TIM_DMABASE_SMCR
  *            @arg TIM_DMABASE_DIER
  *            @arg TIM_DMABASE_SR
  *            @arg TIM_DMABASE_EGR
  *            @arg TIM_DMABASE_CCMR1
  *            @arg TIM_DMABASE_CCMR2
  *            @arg TIM_DMABASE_CCER
  *            @arg TIM_DMABASE_CNT
  *            @arg TIM_DMABASE_PSC
  *            @arg TIM_DMABASE_ARR
  *            @arg TIM_DMABASE_RCR
  *            @arg TIM_DMABASE_CCR1
  *            @arg TIM_DMABASE_CCR2
  *            @arg TIM_DMABASE_CCR3
  *            @arg TIM_DMABASE_CCR4
  *            @arg TIM_DMABASE_BDTR
  * @param  BurstRequestSrc TIM DMA Request sources
  *         This parameter can be one of the following values:
  *            @arg TIM_DMA_UPDATE: TIM update Interrupt source
  *            @arg TIM_DMA_CC1: TIM Capture Compare 1 DMA source
  *            @arg TIM_DMA_CC2: TIM Capture Compare 2 DMA source
  *            @arg TIM_DMA_CC3: TIM Capture Compare 3 DMA source
  *            @arg TIM_DMA_CC4: TIM Capture Compare 4 DMA source
  *            @arg TIM_DMA_COM: TIM Commutation DMA source
  *            @arg TIM_DMA_TRIGGER: TIM Trigger DMA source
  * @param  BurstBuffer The Buffer address.
  * @param  BurstLength DMA Burst length. This parameter can be one value
  *         between: TIM_DMABURSTLENGTH_1TRANSFER and TIM_DMABURSTLENGTH_18TRANSFERS.
  * @param  DataLength Data length. This parameter can be one value
  *         between 1 and 0xFFFF.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_DMABurst_MultiReadStart(TIM_HandleTypeDef *htim, uint32_t BurstBaseAddress,  /* HAL状态类型函数 */
                                                  uint32_t BurstRequestSrc, uint32_t  *BurstBuffer,  /* 32位无符号变量/函数 */
                                                  uint32_t  BurstLength, uint32_t  DataLength)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_DMABURST_INSTANCE(htim->Instance));  /* 执行语句 */
  assert_param(IS_TIM_DMA_BASE(BurstBaseAddress));      /* 执行语句 */
  assert_param(IS_TIM_DMA_SOURCE(BurstRequestSrc));     /* 执行语句 */
  assert_param(IS_TIM_DMA_LENGTH(BurstLength));         /* 执行语句 */
  assert_param(IS_TIM_DMA_DATA_LENGTH(DataLength));     /* 执行语句 */

  if (htim->DMABurstState == HAL_DMA_BURST_STATE_BUSY)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
  else if (htim->DMABurstState == HAL_DMA_BURST_STATE_READY)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    if ((BurstBuffer == NULL) && (BurstLength > 0U))    /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      htim->DMABurstState = HAL_DMA_BURST_STATE_BUSY;   /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* nothing to do */
  }                                                     /* 代码块结束 */
  switch (BurstRequestSrc)                              /* switch分支 */
  {                                                     /* 代码块开始 */
    case TIM_DMA_UPDATE:                                /* case标签 */
    {                                                   /* 代码块开始 */
      /* Set the DMA Period elapsed callbacks */
      htim->hdma[TIM_DMA_ID_UPDATE]->XferCpltCallback = TIM_DMAPeriodElapsedCplt;  /* 赋值或初始化 */
      htim->hdma[TIM_DMA_ID_UPDATE]->XferHalfCpltCallback = TIM_DMAPeriodElapsedHalfCplt;  /* 赋值或初始化 */

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_UPDATE]->XferErrorCallback = TIM_DMAError ;  /* 赋值或初始化 */

      /* Enable the DMA stream */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_UPDATE], (uint32_t)&htim->Instance->DMAR, (uint32_t)BurstBuffer,  /* 条件判断 */
                         DataLength) != HAL_OK)         /* 源代码 */
      {                                                 /* 代码块开始 */
        /* Return error status */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
    case TIM_DMA_CC1:                                   /* case标签 */
    {                                                   /* 代码块开始 */
      /* Set the DMA capture callbacks */
      htim->hdma[TIM_DMA_ID_CC1]->XferCpltCallback = TIM_DMACaptureCplt;  /* 赋值或初始化 */
      htim->hdma[TIM_DMA_ID_CC1]->XferHalfCpltCallback = TIM_DMACaptureHalfCplt;  /* 赋值或初始化 */

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC1]->XferErrorCallback = TIM_DMAError ;  /* 赋值或初始化 */

      /* Enable the DMA stream */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC1], (uint32_t)&htim->Instance->DMAR, (uint32_t)BurstBuffer,  /* 条件判断 */
                         DataLength) != HAL_OK)         /* 源代码 */
      {                                                 /* 代码块开始 */
        /* Return error status */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
    case TIM_DMA_CC2:                                   /* case标签 */
    {                                                   /* 代码块开始 */
      /* Set the DMA capture callbacks */
      htim->hdma[TIM_DMA_ID_CC2]->XferCpltCallback = TIM_DMACaptureCplt;  /* 赋值或初始化 */
      htim->hdma[TIM_DMA_ID_CC2]->XferHalfCpltCallback = TIM_DMACaptureHalfCplt;  /* 赋值或初始化 */

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC2]->XferErrorCallback = TIM_DMAError ;  /* 赋值或初始化 */

      /* Enable the DMA stream */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC2], (uint32_t)&htim->Instance->DMAR, (uint32_t)BurstBuffer,  /* 条件判断 */
                         DataLength) != HAL_OK)         /* 源代码 */
      {                                                 /* 代码块开始 */
        /* Return error status */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
    case TIM_DMA_CC3:                                   /* case标签 */
    {                                                   /* 代码块开始 */
      /* Set the DMA capture callbacks */
      htim->hdma[TIM_DMA_ID_CC3]->XferCpltCallback = TIM_DMACaptureCplt;  /* 赋值或初始化 */
      htim->hdma[TIM_DMA_ID_CC3]->XferHalfCpltCallback = TIM_DMACaptureHalfCplt;  /* 赋值或初始化 */

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC3]->XferErrorCallback = TIM_DMAError ;  /* 赋值或初始化 */

      /* Enable the DMA stream */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC3], (uint32_t)&htim->Instance->DMAR, (uint32_t)BurstBuffer,  /* 条件判断 */
                         DataLength) != HAL_OK)         /* 源代码 */
      {                                                 /* 代码块开始 */
        /* Return error status */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
    case TIM_DMA_CC4:                                   /* case标签 */
    {                                                   /* 代码块开始 */
      /* Set the DMA capture callbacks */
      htim->hdma[TIM_DMA_ID_CC4]->XferCpltCallback = TIM_DMACaptureCplt;  /* 赋值或初始化 */
      htim->hdma[TIM_DMA_ID_CC4]->XferHalfCpltCallback = TIM_DMACaptureHalfCplt;  /* 赋值或初始化 */

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC4]->XferErrorCallback = TIM_DMAError ;  /* 赋值或初始化 */

      /* Enable the DMA stream */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC4], (uint32_t)&htim->Instance->DMAR, (uint32_t)BurstBuffer,  /* 条件判断 */
                         DataLength) != HAL_OK)         /* 源代码 */
      {                                                 /* 代码块开始 */
        /* Return error status */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
    case TIM_DMA_COM:                                   /* case标签 */
    {                                                   /* 代码块开始 */
      /* Set the DMA commutation callbacks */
      htim->hdma[TIM_DMA_ID_COMMUTATION]->XferCpltCallback =  TIMEx_DMACommutationCplt;  /* 赋值或初始化 */
      htim->hdma[TIM_DMA_ID_COMMUTATION]->XferHalfCpltCallback =  TIMEx_DMACommutationHalfCplt;  /* 赋值或初始化 */

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_COMMUTATION]->XferErrorCallback = TIM_DMAError ;  /* 赋值或初始化 */

      /* Enable the DMA stream */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_COMMUTATION], (uint32_t)&htim->Instance->DMAR, (uint32_t)BurstBuffer,  /* 条件判断 */
                         DataLength) != HAL_OK)         /* 源代码 */
      {                                                 /* 代码块开始 */
        /* Return error status */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
    case TIM_DMA_TRIGGER:                               /* case标签 */
    {                                                   /* 代码块开始 */
      /* Set the DMA trigger callbacks */
      htim->hdma[TIM_DMA_ID_TRIGGER]->XferCpltCallback = TIM_DMATriggerCplt;  /* 赋值或初始化 */
      htim->hdma[TIM_DMA_ID_TRIGGER]->XferHalfCpltCallback = TIM_DMATriggerHalfCplt;  /* 赋值或初始化 */

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_TRIGGER]->XferErrorCallback = TIM_DMAError ;  /* 赋值或初始化 */

      /* Enable the DMA stream */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_TRIGGER], (uint32_t)&htim->Instance->DMAR, (uint32_t)BurstBuffer,  /* 条件判断 */
                         DataLength) != HAL_OK)         /* 源代码 */
      {                                                 /* 代码块开始 */
        /* Return error status */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
    default:                                            /* default分支 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  /* Configure the DMA Burst Mode */
  htim->Instance->DCR = (BurstBaseAddress | BurstLength);  /* 赋值或初始化 */

  /* Enable the TIM DMA Request */
  __HAL_TIM_ENABLE_DMA(htim, BurstRequestSrc);          /* HAL辅助宏/函数 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Stop the DMA burst reading
  * @param  htim TIM handle
  * @param  BurstRequestSrc TIM DMA Request sources to disable.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_DMABurst_ReadStop(TIM_HandleTypeDef *htim, uint32_t BurstRequestSrc)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_DMA_SOURCE(BurstRequestSrc));     /* 执行语句 */

  /* Abort the DMA transfer (at least disable the DMA stream) */
  switch (BurstRequestSrc)                              /* switch分支 */
  {                                                     /* 代码块开始 */
    case TIM_DMA_UPDATE:                                /* case标签 */
    {                                                   /* 代码块开始 */
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_UPDATE]);  /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
    case TIM_DMA_CC1:                                   /* case标签 */
    {                                                   /* 代码块开始 */
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC1]);  /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
    case TIM_DMA_CC2:                                   /* case标签 */
    {                                                   /* 代码块开始 */
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC2]);  /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
    case TIM_DMA_CC3:                                   /* case标签 */
    {                                                   /* 代码块开始 */
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC3]);  /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
    case TIM_DMA_CC4:                                   /* case标签 */
    {                                                   /* 代码块开始 */
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC4]);  /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
    case TIM_DMA_COM:                                   /* case标签 */
    {                                                   /* 代码块开始 */
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_COMMUTATION]);  /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
    case TIM_DMA_TRIGGER:                               /* case标签 */
    {                                                   /* 代码块开始 */
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_TRIGGER]);  /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
    default:                                            /* default分支 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  /* Disable the TIM Update DMA request */
  __HAL_TIM_DISABLE_DMA(htim, BurstRequestSrc);         /* HAL辅助宏/函数 */

  /* Change the DMA burst operation state */
  htim->DMABurstState = HAL_DMA_BURST_STATE_READY;      /* 赋值或初始化 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Generate a software event
  * @param  htim TIM handle
  * @param  EventSource specifies the event source.
  *          This parameter can be one of the following values:
  *            @arg TIM_EVENTSOURCE_UPDATE: Timer update Event source
  *            @arg TIM_EVENTSOURCE_CC1: Timer Capture Compare 1 Event source
  *            @arg TIM_EVENTSOURCE_CC2: Timer Capture Compare 2 Event source
  *            @arg TIM_EVENTSOURCE_CC3: Timer Capture Compare 3 Event source
  *            @arg TIM_EVENTSOURCE_CC4: Timer Capture Compare 4 Event source
  *            @arg TIM_EVENTSOURCE_COM: Timer COM event source
  *            @arg TIM_EVENTSOURCE_TRIGGER: Timer Trigger Event source
  *            @arg TIM_EVENTSOURCE_BREAK: Timer Break event source
  * @note   Basic timers can only generate an update event.
  * @note   TIM_EVENTSOURCE_COM is relevant only with advanced timer instances.
  * @note   TIM_EVENTSOURCE_BREAK are relevant only for timer instances
  *         supporting a break input.
  * @retval HAL status
  */

HAL_StatusTypeDef HAL_TIM_GenerateEvent(TIM_HandleTypeDef *htim, uint32_t EventSource)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_INSTANCE(htim->Instance));        /* 执行语句 */
  assert_param(IS_TIM_EVENT_SOURCE(EventSource));       /* 执行语句 */

  /* Process Locked */
  __HAL_LOCK(htim);                                     /* HAL辅助宏/函数 */

  /* Change the TIM state */
  htim->State = HAL_TIM_STATE_BUSY;                     /* 赋值或初始化 */

  /* Set the event sources */
  htim->Instance->EGR = EventSource;                    /* 赋值或初始化 */

  /* Change the TIM state */
  htim->State = HAL_TIM_STATE_READY;                    /* 赋值或初始化 */

  __HAL_UNLOCK(htim);                                   /* HAL辅助宏/函数 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configures the OCRef clear feature
  * @param  htim TIM handle
  * @param  sClearInputConfig pointer to a TIM_ClearInputConfigTypeDef structure that
  *         contains the OCREF clear feature and parameters for the TIM peripheral.
  * @param  Channel specifies the TIM Channel
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1
  *            @arg TIM_CHANNEL_2: TIM Channel 2
  *            @arg TIM_CHANNEL_3: TIM Channel 3
  *            @arg TIM_CHANNEL_4: TIM Channel 4
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_ConfigOCrefClear(TIM_HandleTypeDef *htim,  /* HAL状态类型函数 */
                                           TIM_ClearInputConfigTypeDef *sClearInputConfig,  /* 列表续行 */
                                           uint32_t Channel)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_OCXREF_CLEAR_INSTANCE(htim->Instance));  /* 执行语句 */
  assert_param(IS_TIM_CLEARINPUT_SOURCE(sClearInputConfig->ClearInputSource));  /* 执行语句 */

  /* Process Locked */
  __HAL_LOCK(htim);                                     /* HAL辅助宏/函数 */

  htim->State = HAL_TIM_STATE_BUSY;                     /* 赋值或初始化 */

  switch (sClearInputConfig->ClearInputSource)          /* switch分支 */
  {                                                     /* 代码块开始 */
    case TIM_CLEARINPUTSOURCE_NONE:                     /* case标签 */
    {                                                   /* 代码块开始 */
      /* Clear the OCREF clear selection bit and the the ETR Bits */
      CLEAR_BIT(htim->Instance->SMCR, (TIM_SMCR_ETF | TIM_SMCR_ETPS | TIM_SMCR_ECE | TIM_SMCR_ETP));  /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CLEARINPUTSOURCE_ETR:                      /* case标签 */
    {                                                   /* 代码块开始 */
      /* Check the parameters */
      assert_param(IS_TIM_CLEARINPUT_POLARITY(sClearInputConfig->ClearInputPolarity));  /* 执行语句 */
      assert_param(IS_TIM_CLEARINPUT_PRESCALER(sClearInputConfig->ClearInputPrescaler));  /* 执行语句 */
      assert_param(IS_TIM_CLEARINPUT_FILTER(sClearInputConfig->ClearInputFilter));  /* 执行语句 */

      /* When OCRef clear feature is used with ETR source, ETR prescaler must be off */
      if (sClearInputConfig->ClearInputPrescaler != TIM_CLEARINPUTPRESCALER_DIV1)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        htim->State = HAL_TIM_STATE_READY;              /* 赋值或初始化 */
        __HAL_UNLOCK(htim);                             /* HAL辅助宏/函数 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */

      TIM_ETR_SetConfig(htim->Instance,                 /* 列表续行 */
                        sClearInputConfig->ClearInputPrescaler,  /* 列表续行 */
                        sClearInputConfig->ClearInputPolarity,  /* 列表续行 */
                        sClearInputConfig->ClearInputFilter);  /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    default:                                            /* default分支 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  switch (Channel)                                      /* switch分支 */
  {                                                     /* 代码块开始 */
    case TIM_CHANNEL_1:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      if (sClearInputConfig->ClearInputState != (uint32_t)DISABLE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Enable the OCREF clear feature for Channel 1 */
        SET_BIT(htim->Instance->CCMR1, TIM_CCMR1_OC1CE);  /* 执行语句 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Disable the OCREF clear feature for Channel 1 */
        CLEAR_BIT(htim->Instance->CCMR1, TIM_CCMR1_OC1CE);  /* 执行语句 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
    case TIM_CHANNEL_2:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      if (sClearInputConfig->ClearInputState != (uint32_t)DISABLE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Enable the OCREF clear feature for Channel 2 */
        SET_BIT(htim->Instance->CCMR1, TIM_CCMR1_OC2CE);  /* 执行语句 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Disable the OCREF clear feature for Channel 2 */
        CLEAR_BIT(htim->Instance->CCMR1, TIM_CCMR1_OC2CE);  /* 执行语句 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
    case TIM_CHANNEL_3:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      if (sClearInputConfig->ClearInputState != (uint32_t)DISABLE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Enable the OCREF clear feature for Channel 3 */
        SET_BIT(htim->Instance->CCMR2, TIM_CCMR2_OC3CE);  /* 执行语句 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Disable the OCREF clear feature for Channel 3 */
        CLEAR_BIT(htim->Instance->CCMR2, TIM_CCMR2_OC3CE);  /* 执行语句 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
    case TIM_CHANNEL_4:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      if (sClearInputConfig->ClearInputState != (uint32_t)DISABLE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Enable the OCREF clear feature for Channel 4 */
        SET_BIT(htim->Instance->CCMR2, TIM_CCMR2_OC4CE);  /* 执行语句 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Disable the OCREF clear feature for Channel 4 */
        CLEAR_BIT(htim->Instance->CCMR2, TIM_CCMR2_OC4CE);  /* 执行语句 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
    default:                                            /* default分支 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  htim->State = HAL_TIM_STATE_READY;                    /* 赋值或初始化 */

  __HAL_UNLOCK(htim);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief   Configures the clock source to be used
  * @param  htim TIM handle
  * @param  sClockSourceConfig pointer to a TIM_ClockConfigTypeDef structure that
  *         contains the clock source information for the TIM peripheral.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_ConfigClockSource(TIM_HandleTypeDef *htim, TIM_ClockConfigTypeDef *sClockSourceConfig)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpsmcr;                                     /* 32位无符号变量/函数 */

  /* Process Locked */
  __HAL_LOCK(htim);                                     /* HAL辅助宏/函数 */

  htim->State = HAL_TIM_STATE_BUSY;                     /* 赋值或初始化 */

  /* Check the parameters */
  assert_param(IS_TIM_CLOCKSOURCE(sClockSourceConfig->ClockSource));  /* 执行语句 */

  /* Reset the SMS, TS, ECE, ETPS and ETRF bits */
  tmpsmcr = htim->Instance->SMCR;                       /* 赋值或初始化 */
  tmpsmcr &= ~(TIM_SMCR_SMS | TIM_SMCR_TS);             /* 赋值或初始化 */
  tmpsmcr &= ~(TIM_SMCR_ETF | TIM_SMCR_ETPS | TIM_SMCR_ECE | TIM_SMCR_ETP);  /* 赋值或初始化 */
  htim->Instance->SMCR = tmpsmcr;                       /* 赋值或初始化 */

  switch (sClockSourceConfig->ClockSource)              /* switch分支 */
  {                                                     /* 代码块开始 */
    case TIM_CLOCKSOURCE_INTERNAL:                      /* case标签 */
    {                                                   /* 代码块开始 */
      assert_param(IS_TIM_INSTANCE(htim->Instance));    /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CLOCKSOURCE_ETRMODE1:                      /* case标签 */
    {                                                   /* 代码块开始 */
      /* Check whether or not the timer instance supports external trigger input mode 1 (ETRF)*/
      assert_param(IS_TIM_CLOCKSOURCE_ETRMODE1_INSTANCE(htim->Instance));  /* 执行语句 */

      /* Check ETR input conditioning related parameters */
      assert_param(IS_TIM_CLOCKPRESCALER(sClockSourceConfig->ClockPrescaler));  /* 执行语句 */
      assert_param(IS_TIM_CLOCKPOLARITY(sClockSourceConfig->ClockPolarity));  /* 执行语句 */
      assert_param(IS_TIM_CLOCKFILTER(sClockSourceConfig->ClockFilter));  /* 执行语句 */

      /* Configure the ETR Clock source */
      TIM_ETR_SetConfig(htim->Instance,                 /* 列表续行 */
                        sClockSourceConfig->ClockPrescaler,  /* 列表续行 */
                        sClockSourceConfig->ClockPolarity,  /* 列表续行 */
                        sClockSourceConfig->ClockFilter);  /* 执行语句 */

      /* Select the External clock mode1 and the ETRF trigger */
      tmpsmcr = htim->Instance->SMCR;                   /* 赋值或初始化 */
      tmpsmcr |= (TIM_SLAVEMODE_EXTERNAL1 | TIM_CLOCKSOURCE_ETRMODE1);  /* 赋值或初始化 */
      /* Write to TIMx SMCR */
      htim->Instance->SMCR = tmpsmcr;                   /* 赋值或初始化 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CLOCKSOURCE_ETRMODE2:                      /* case标签 */
    {                                                   /* 代码块开始 */
      /* Check whether or not the timer instance supports external trigger input mode 2 (ETRF)*/
      assert_param(IS_TIM_CLOCKSOURCE_ETRMODE2_INSTANCE(htim->Instance));  /* 执行语句 */

      /* Check ETR input conditioning related parameters */
      assert_param(IS_TIM_CLOCKPRESCALER(sClockSourceConfig->ClockPrescaler));  /* 执行语句 */
      assert_param(IS_TIM_CLOCKPOLARITY(sClockSourceConfig->ClockPolarity));  /* 执行语句 */
      assert_param(IS_TIM_CLOCKFILTER(sClockSourceConfig->ClockFilter));  /* 执行语句 */

      /* Configure the ETR Clock source */
      TIM_ETR_SetConfig(htim->Instance,                 /* 列表续行 */
                        sClockSourceConfig->ClockPrescaler,  /* 列表续行 */
                        sClockSourceConfig->ClockPolarity,  /* 列表续行 */
                        sClockSourceConfig->ClockFilter);  /* 执行语句 */
      /* Enable the External clock mode2 */
      htim->Instance->SMCR |= TIM_SMCR_ECE;             /* 赋值或初始化 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CLOCKSOURCE_TI1:                           /* case标签 */
    {                                                   /* 代码块开始 */
      /* Check whether or not the timer instance supports external clock mode 1 */
      assert_param(IS_TIM_CLOCKSOURCE_TIX_INSTANCE(htim->Instance));  /* 执行语句 */

      /* Check TI1 input conditioning related parameters */
      assert_param(IS_TIM_CLOCKPOLARITY(sClockSourceConfig->ClockPolarity));  /* 执行语句 */
      assert_param(IS_TIM_CLOCKFILTER(sClockSourceConfig->ClockFilter));  /* 执行语句 */

      TIM_TI1_ConfigInputStage(htim->Instance,          /* 列表续行 */
                               sClockSourceConfig->ClockPolarity,  /* 列表续行 */
                               sClockSourceConfig->ClockFilter);  /* 执行语句 */
      TIM_ITRx_SetConfig(htim->Instance, TIM_CLOCKSOURCE_TI1);  /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CLOCKSOURCE_TI2:                           /* case标签 */
    {                                                   /* 代码块开始 */
      /* Check whether or not the timer instance supports external clock mode 1 (ETRF)*/
      assert_param(IS_TIM_CLOCKSOURCE_TIX_INSTANCE(htim->Instance));  /* 执行语句 */

      /* Check TI2 input conditioning related parameters */
      assert_param(IS_TIM_CLOCKPOLARITY(sClockSourceConfig->ClockPolarity));  /* 执行语句 */
      assert_param(IS_TIM_CLOCKFILTER(sClockSourceConfig->ClockFilter));  /* 执行语句 */

      TIM_TI2_ConfigInputStage(htim->Instance,          /* 列表续行 */
                               sClockSourceConfig->ClockPolarity,  /* 列表续行 */
                               sClockSourceConfig->ClockFilter);  /* 执行语句 */
      TIM_ITRx_SetConfig(htim->Instance, TIM_CLOCKSOURCE_TI2);  /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CLOCKSOURCE_TI1ED:                         /* case标签 */
    {                                                   /* 代码块开始 */
      /* Check whether or not the timer instance supports external clock mode 1 */
      assert_param(IS_TIM_CLOCKSOURCE_TIX_INSTANCE(htim->Instance));  /* 执行语句 */

      /* Check TI1 input conditioning related parameters */
      assert_param(IS_TIM_CLOCKPOLARITY(sClockSourceConfig->ClockPolarity));  /* 执行语句 */
      assert_param(IS_TIM_CLOCKFILTER(sClockSourceConfig->ClockFilter));  /* 执行语句 */

      TIM_TI1_ConfigInputStage(htim->Instance,          /* 列表续行 */
                               sClockSourceConfig->ClockPolarity,  /* 列表续行 */
                               sClockSourceConfig->ClockFilter);  /* 执行语句 */
      TIM_ITRx_SetConfig(htim->Instance, TIM_CLOCKSOURCE_TI1ED);  /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CLOCKSOURCE_ITR0:                          /* case标签 */
    case TIM_CLOCKSOURCE_ITR1:                          /* case标签 */
    case TIM_CLOCKSOURCE_ITR2:                          /* case标签 */
    case TIM_CLOCKSOURCE_ITR3:                          /* case标签 */
      {                                                 /* 代码块开始 */
        /* Check whether or not the timer instance supports internal trigger input */
        assert_param(IS_TIM_CLOCKSOURCE_ITRX_INSTANCE(htim->Instance));  /* 执行语句 */

        TIM_ITRx_SetConfig(htim->Instance, sClockSourceConfig->ClockSource);  /* 执行语句 */
        break;                                          /* 跳出 */
      }                                                 /* 代码块结束 */

    default:                                            /* default分支 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */
  htim->State = HAL_TIM_STATE_READY;                    /* 赋值或初始化 */

  __HAL_UNLOCK(htim);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Selects the signal connected to the TI1 input: direct from CH1_input
  *         or a XOR combination between CH1_input, CH2_input & CH3_input
  * @param  htim TIM handle.
  * @param  TI1_Selection Indicate whether or not channel 1 is connected to the
  *         output of a XOR gate.
  *          This parameter can be one of the following values:
  *            @arg TIM_TI1SELECTION_CH1: The TIMx_CH1 pin is connected to TI1 input
  *            @arg TIM_TI1SELECTION_XORCOMBINATION: The TIMx_CH1, CH2 and CH3
  *            pins are connected to the TI1 input (XOR combination)
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_ConfigTI1Input(TIM_HandleTypeDef *htim, uint32_t TI1_Selection)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpcr2;                                      /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_TIM_XOR_INSTANCE(htim->Instance));    /* 执行语句 */
  assert_param(IS_TIM_TI1SELECTION(TI1_Selection));     /* 执行语句 */

  /* Get the TIMx CR2 register value */
  tmpcr2 = htim->Instance->CR2;                         /* 赋值或初始化 */

  /* Reset the TI1 selection */
  tmpcr2 &= ~TIM_CR2_TI1S;                              /* 赋值或初始化 */

  /* Set the TI1 selection */
  tmpcr2 |= TI1_Selection;                              /* 赋值或初始化 */

  /* Write to TIMxCR2 */
  htim->Instance->CR2 = tmpcr2;                         /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configures the TIM in Slave mode
  * @param  htim TIM handle.
  * @param  sSlaveConfig pointer to a TIM_SlaveConfigTypeDef structure that
  *         contains the selected trigger (internal trigger input, filtered
  *         timer input or external trigger input) and the Slave mode
  *         (Disable, Reset, Gated, Trigger, External clock mode 1).
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_SlaveConfigSynchro(TIM_HandleTypeDef *htim, TIM_SlaveConfigTypeDef *sSlaveConfig)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_SLAVE_INSTANCE(htim->Instance));  /* 执行语句 */
  assert_param(IS_TIM_SLAVE_MODE(sSlaveConfig->SlaveMode));  /* 执行语句 */
  assert_param(IS_TIM_TRIGGER_SELECTION(sSlaveConfig->InputTrigger));  /* 执行语句 */

  __HAL_LOCK(htim);                                     /* HAL辅助宏/函数 */

  htim->State = HAL_TIM_STATE_BUSY;                     /* 赋值或初始化 */

  if (TIM_SlaveTimer_SetConfig(htim, sSlaveConfig) != HAL_OK)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    htim->State = HAL_TIM_STATE_READY;                  /* 赋值或初始化 */
    __HAL_UNLOCK(htim);                                 /* HAL辅助宏/函数 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Disable Trigger Interrupt */
  __HAL_TIM_DISABLE_IT(htim, TIM_IT_TRIGGER);           /* HAL辅助宏/函数 */

  /* Disable Trigger DMA request */
  __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_TRIGGER);         /* HAL辅助宏/函数 */

  htim->State = HAL_TIM_STATE_READY;                    /* 赋值或初始化 */

  __HAL_UNLOCK(htim);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configures the TIM in Slave mode in interrupt mode
  * @param  htim TIM handle.
  * @param  sSlaveConfig pointer to a TIM_SlaveConfigTypeDef structure that
  *         contains the selected trigger (internal trigger input, filtered
  *         timer input or external trigger input) and the Slave mode
  *         (Disable, Reset, Gated, Trigger, External clock mode 1).
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_SlaveConfigSynchro_IT(TIM_HandleTypeDef *htim,  /* HAL状态类型函数 */
                                                TIM_SlaveConfigTypeDef *sSlaveConfig)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_SLAVE_INSTANCE(htim->Instance));  /* 执行语句 */
  assert_param(IS_TIM_SLAVE_MODE(sSlaveConfig->SlaveMode));  /* 执行语句 */
  assert_param(IS_TIM_TRIGGER_SELECTION(sSlaveConfig->InputTrigger));  /* 执行语句 */

  __HAL_LOCK(htim);                                     /* HAL辅助宏/函数 */

  htim->State = HAL_TIM_STATE_BUSY;                     /* 赋值或初始化 */

  if (TIM_SlaveTimer_SetConfig(htim, sSlaveConfig) != HAL_OK)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    htim->State = HAL_TIM_STATE_READY;                  /* 赋值或初始化 */
    __HAL_UNLOCK(htim);                                 /* HAL辅助宏/函数 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Enable Trigger Interrupt */
  __HAL_TIM_ENABLE_IT(htim, TIM_IT_TRIGGER);            /* HAL辅助宏/函数 */

  /* Disable Trigger DMA request */
  __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_TRIGGER);         /* HAL辅助宏/函数 */

  htim->State = HAL_TIM_STATE_READY;                    /* 赋值或初始化 */

  __HAL_UNLOCK(htim);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Read the captured value from Capture Compare unit
  * @param  htim TIM handle.
  * @param  Channel TIM Channels to be enabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @retval Captured value
  */
uint32_t HAL_TIM_ReadCapturedValue(TIM_HandleTypeDef *htim, uint32_t Channel)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpreg = 0U;                                 /* 32位无符号变量/函数 */

  switch (Channel)                                      /* switch分支 */
  {                                                     /* 代码块开始 */
    case TIM_CHANNEL_1:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Check the parameters */
      assert_param(IS_TIM_CC1_INSTANCE(htim->Instance));  /* 执行语句 */

      /* Return the capture 1 value */
      tmpreg =  htim->Instance->CCR1;                   /* 赋值或初始化 */

      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
    case TIM_CHANNEL_2:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Check the parameters */
      assert_param(IS_TIM_CC2_INSTANCE(htim->Instance));  /* 执行语句 */

      /* Return the capture 2 value */
      tmpreg =   htim->Instance->CCR2;                  /* 赋值或初始化 */

      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_3:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Check the parameters */
      assert_param(IS_TIM_CC3_INSTANCE(htim->Instance));  /* 执行语句 */

      /* Return the capture 3 value */
      tmpreg =   htim->Instance->CCR3;                  /* 赋值或初始化 */

      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_4:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Check the parameters */
      assert_param(IS_TIM_CC4_INSTANCE(htim->Instance));  /* 执行语句 */

      /* Return the capture 4 value */
      tmpreg =   htim->Instance->CCR4;                  /* 赋值或初始化 */

      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    default:                                            /* default分支 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  return tmpreg;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup TIM_Exported_Functions_Group9 TIM Callbacks functions
  *  @brief    TIM Callbacks functions
  *
@verbatim
  ==============================================================================
                        ##### TIM Callbacks functions #####
  ==============================================================================
 [..]
   This section provides TIM callback functions:
   (+) TIM Period elapsed callback
   (+) TIM Output Compare callback
   (+) TIM Input capture callback
   (+) TIM Trigger callback
   (+) TIM Error callback

@endverbatim
  * @{
  */

/**
  * @brief  Period elapsed callback in non-blocking mode
  * @param  htim TIM handle
  * @retval None
  */
__weak void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_PeriodElapsedCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Period elapsed half complete callback in non-blocking mode
  * @param  htim TIM handle
  * @retval None
  */
__weak void HAL_TIM_PeriodElapsedHalfCpltCallback(TIM_HandleTypeDef *htim)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_PeriodElapsedHalfCpltCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Output Compare callback in non-blocking mode
  * @param  htim TIM OC handle
  * @retval None
  */
__weak void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_OC_DelayElapsedCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Input Capture callback in non-blocking mode
  * @param  htim TIM IC handle
  * @retval None
  */
__weak void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_IC_CaptureCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Input Capture half complete callback in non-blocking mode
  * @param  htim TIM IC handle
  * @retval None
  */
__weak void HAL_TIM_IC_CaptureHalfCpltCallback(TIM_HandleTypeDef *htim)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_IC_CaptureHalfCpltCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  PWM Pulse finished callback in non-blocking mode
  * @param  htim TIM handle
  * @retval None
  */
__weak void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_PWM_PulseFinishedCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  PWM Pulse finished half complete callback in non-blocking mode
  * @param  htim TIM handle
  * @retval None
  */
__weak void HAL_TIM_PWM_PulseFinishedHalfCpltCallback(TIM_HandleTypeDef *htim)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_PWM_PulseFinishedHalfCpltCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Hall Trigger detection callback in non-blocking mode
  * @param  htim TIM handle
  * @retval None
  */
__weak void HAL_TIM_TriggerCallback(TIM_HandleTypeDef *htim)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_TriggerCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Hall Trigger detection half complete callback in non-blocking mode
  * @param  htim TIM handle
  * @retval None
  */
__weak void HAL_TIM_TriggerHalfCpltCallback(TIM_HandleTypeDef *htim)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_TriggerHalfCpltCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Timer error callback in non-blocking mode
  * @param  htim TIM handle
  * @retval None
  */
__weak void HAL_TIM_ErrorCallback(TIM_HandleTypeDef *htim)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_ErrorCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)               /* 条件编译 */
/**
  * @brief  Register a User TIM callback to be used instead of the weak predefined callback
  * @param htim tim handle
  * @param CallbackID ID of the callback to be registered
  *        This parameter can be one of the following values:
  *          @arg @ref HAL_TIM_BASE_MSPINIT_CB_ID Base MspInit Callback ID
  *          @arg @ref HAL_TIM_BASE_MSPDEINIT_CB_ID Base MspDeInit Callback ID
  *          @arg @ref HAL_TIM_IC_MSPINIT_CB_ID IC MspInit Callback ID
  *          @arg @ref HAL_TIM_IC_MSPDEINIT_CB_ID IC MspDeInit Callback ID
  *          @arg @ref HAL_TIM_OC_MSPINIT_CB_ID OC MspInit Callback ID
  *          @arg @ref HAL_TIM_OC_MSPDEINIT_CB_ID OC MspDeInit Callback ID
  *          @arg @ref HAL_TIM_PWM_MSPINIT_CB_ID PWM MspInit Callback ID
  *          @arg @ref HAL_TIM_PWM_MSPDEINIT_CB_ID PWM MspDeInit Callback ID
  *          @arg @ref HAL_TIM_ONE_PULSE_MSPINIT_CB_ID One Pulse MspInit Callback ID
  *          @arg @ref HAL_TIM_ONE_PULSE_MSPDEINIT_CB_ID One Pulse MspDeInit Callback ID
  *          @arg @ref HAL_TIM_ENCODER_MSPINIT_CB_ID Encoder MspInit Callback ID
  *          @arg @ref HAL_TIM_ENCODER_MSPDEINIT_CB_ID Encoder MspDeInit Callback ID
  *          @arg @ref HAL_TIM_HALL_SENSOR_MSPINIT_CB_ID Hall Sensor MspInit Callback ID
  *          @arg @ref HAL_TIM_HALL_SENSOR_MSPDEINIT_CB_ID Hall Sensor MspDeInit Callback ID
  *          @arg @ref HAL_TIM_PERIOD_ELAPSED_CB_ID Period Elapsed Callback ID
  *          @arg @ref HAL_TIM_PERIOD_ELAPSED_HALF_CB_ID Period Elapsed half complete Callback ID
  *          @arg @ref HAL_TIM_TRIGGER_CB_ID Trigger Callback ID
  *          @arg @ref HAL_TIM_TRIGGER_HALF_CB_ID Trigger half complete Callback ID
  *          @arg @ref HAL_TIM_IC_CAPTURE_CB_ID Input Capture Callback ID
  *          @arg @ref HAL_TIM_IC_CAPTURE_HALF_CB_ID Input Capture half complete Callback ID
  *          @arg @ref HAL_TIM_OC_DELAY_ELAPSED_CB_ID Output Compare Delay Elapsed Callback ID
  *          @arg @ref HAL_TIM_PWM_PULSE_FINISHED_CB_ID PWM Pulse Finished Callback ID
  *          @arg @ref HAL_TIM_PWM_PULSE_FINISHED_HALF_CB_ID PWM Pulse Finished half complete Callback ID
  *          @arg @ref HAL_TIM_ERROR_CB_ID Error Callback ID
  *          @arg @ref HAL_TIM_COMMUTATION_CB_ID Commutation Callback ID
  *          @arg @ref HAL_TIM_COMMUTATION_HALF_CB_ID Commutation half complete Callback ID
  *          @arg @ref HAL_TIM_BREAK_CB_ID Break Callback ID
  *          @param pCallback pointer to the callback function
  *          @retval status
  */
HAL_StatusTypeDef HAL_TIM_RegisterCallback(TIM_HandleTypeDef *htim, HAL_TIM_CallbackIDTypeDef CallbackID,  /* HAL状态类型函数 */
                                           pTIM_CallbackTypeDef pCallback)  /* 源代码 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  if (pCallback == NULL)                                /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  /* Process locked */
  __HAL_LOCK(htim);                                     /* HAL辅助宏/函数 */

  if (htim->State == HAL_TIM_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_TIM_BASE_MSPINIT_CB_ID :                 /* case标签 */
        htim->Base_MspInitCallback                 = pCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_TIM_BASE_MSPDEINIT_CB_ID :               /* case标签 */
        htim->Base_MspDeInitCallback               = pCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_TIM_IC_MSPINIT_CB_ID :                   /* case标签 */
        htim->IC_MspInitCallback                   = pCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_TIM_IC_MSPDEINIT_CB_ID :                 /* case标签 */
        htim->IC_MspDeInitCallback                 = pCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_TIM_OC_MSPINIT_CB_ID :                   /* case标签 */
        htim->OC_MspInitCallback                   = pCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_TIM_OC_MSPDEINIT_CB_ID :                 /* case标签 */
        htim->OC_MspDeInitCallback                 = pCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_TIM_PWM_MSPINIT_CB_ID :                  /* case标签 */
        htim->PWM_MspInitCallback                  = pCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_TIM_PWM_MSPDEINIT_CB_ID :                /* case标签 */
        htim->PWM_MspDeInitCallback                = pCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_TIM_ONE_PULSE_MSPINIT_CB_ID :            /* case标签 */
        htim->OnePulse_MspInitCallback             = pCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_TIM_ONE_PULSE_MSPDEINIT_CB_ID :          /* case标签 */
        htim->OnePulse_MspDeInitCallback           = pCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_TIM_ENCODER_MSPINIT_CB_ID :              /* case标签 */
        htim->Encoder_MspInitCallback              = pCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_TIM_ENCODER_MSPDEINIT_CB_ID :            /* case标签 */
        htim->Encoder_MspDeInitCallback            = pCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_TIM_HALL_SENSOR_MSPINIT_CB_ID :          /* case标签 */
        htim->HallSensor_MspInitCallback           = pCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_TIM_HALL_SENSOR_MSPDEINIT_CB_ID :        /* case标签 */
        htim->HallSensor_MspDeInitCallback         = pCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_TIM_PERIOD_ELAPSED_CB_ID :               /* case标签 */
        htim->PeriodElapsedCallback                = pCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_TIM_PERIOD_ELAPSED_HALF_CB_ID :          /* case标签 */
        htim->PeriodElapsedHalfCpltCallback        = pCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_TIM_TRIGGER_CB_ID :                      /* case标签 */
        htim->TriggerCallback                      = pCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_TIM_TRIGGER_HALF_CB_ID :                 /* case标签 */
        htim->TriggerHalfCpltCallback              = pCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_TIM_IC_CAPTURE_CB_ID :                   /* case标签 */
        htim->IC_CaptureCallback                   = pCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_TIM_IC_CAPTURE_HALF_CB_ID :              /* case标签 */
        htim->IC_CaptureHalfCpltCallback           = pCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_TIM_OC_DELAY_ELAPSED_CB_ID :             /* case标签 */
        htim->OC_DelayElapsedCallback              = pCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_TIM_PWM_PULSE_FINISHED_CB_ID :           /* case标签 */
        htim->PWM_PulseFinishedCallback            = pCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_TIM_PWM_PULSE_FINISHED_HALF_CB_ID :      /* case标签 */
        htim->PWM_PulseFinishedHalfCpltCallback    = pCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_TIM_ERROR_CB_ID :                        /* case标签 */
        htim->ErrorCallback                        = pCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_TIM_COMMUTATION_CB_ID :                  /* case标签 */
        htim->CommutationCallback                  = pCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_TIM_COMMUTATION_HALF_CB_ID :             /* case标签 */
        htim->CommutationHalfCpltCallback          = pCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_TIM_BREAK_CB_ID :                        /* case标签 */
        htim->BreakCallback                        = pCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (htim->State == HAL_TIM_STATE_RESET)          /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_TIM_BASE_MSPINIT_CB_ID :                 /* case标签 */
        htim->Base_MspInitCallback         = pCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_TIM_BASE_MSPDEINIT_CB_ID :               /* case标签 */
        htim->Base_MspDeInitCallback       = pCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_TIM_IC_MSPINIT_CB_ID :                   /* case标签 */
        htim->IC_MspInitCallback           = pCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_TIM_IC_MSPDEINIT_CB_ID :                 /* case标签 */
        htim->IC_MspDeInitCallback         = pCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_TIM_OC_MSPINIT_CB_ID :                   /* case标签 */
        htim->OC_MspInitCallback           = pCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_TIM_OC_MSPDEINIT_CB_ID :                 /* case标签 */
        htim->OC_MspDeInitCallback         = pCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_TIM_PWM_MSPINIT_CB_ID :                  /* case标签 */
        htim->PWM_MspInitCallback          = pCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_TIM_PWM_MSPDEINIT_CB_ID :                /* case标签 */
        htim->PWM_MspDeInitCallback        = pCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_TIM_ONE_PULSE_MSPINIT_CB_ID :            /* case标签 */
        htim->OnePulse_MspInitCallback     = pCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_TIM_ONE_PULSE_MSPDEINIT_CB_ID :          /* case标签 */
        htim->OnePulse_MspDeInitCallback   = pCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_TIM_ENCODER_MSPINIT_CB_ID :              /* case标签 */
        htim->Encoder_MspInitCallback      = pCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_TIM_ENCODER_MSPDEINIT_CB_ID :            /* case标签 */
        htim->Encoder_MspDeInitCallback    = pCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_TIM_HALL_SENSOR_MSPINIT_CB_ID :          /* case标签 */
        htim->HallSensor_MspInitCallback   = pCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_TIM_HALL_SENSOR_MSPDEINIT_CB_ID :        /* case标签 */
        htim->HallSensor_MspDeInitCallback = pCallback;  /* 赋值或初始化 */
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
  __HAL_UNLOCK(htim);                                   /* HAL辅助宏/函数 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Unregister a TIM callback
  *         TIM callback is redirected to the weak predefined callback
  * @param htim tim handle
  * @param CallbackID ID of the callback to be unregistered
  *        This parameter can be one of the following values:
  *          @arg @ref HAL_TIM_BASE_MSPINIT_CB_ID Base MspInit Callback ID
  *          @arg @ref HAL_TIM_BASE_MSPDEINIT_CB_ID Base MspDeInit Callback ID
  *          @arg @ref HAL_TIM_IC_MSPINIT_CB_ID IC MspInit Callback ID
  *          @arg @ref HAL_TIM_IC_MSPDEINIT_CB_ID IC MspDeInit Callback ID
  *          @arg @ref HAL_TIM_OC_MSPINIT_CB_ID OC MspInit Callback ID
  *          @arg @ref HAL_TIM_OC_MSPDEINIT_CB_ID OC MspDeInit Callback ID
  *          @arg @ref HAL_TIM_PWM_MSPINIT_CB_ID PWM MspInit Callback ID
  *          @arg @ref HAL_TIM_PWM_MSPDEINIT_CB_ID PWM MspDeInit Callback ID
  *          @arg @ref HAL_TIM_ONE_PULSE_MSPINIT_CB_ID One Pulse MspInit Callback ID
  *          @arg @ref HAL_TIM_ONE_PULSE_MSPDEINIT_CB_ID One Pulse MspDeInit Callback ID
  *          @arg @ref HAL_TIM_ENCODER_MSPINIT_CB_ID Encoder MspInit Callback ID
  *          @arg @ref HAL_TIM_ENCODER_MSPDEINIT_CB_ID Encoder MspDeInit Callback ID
  *          @arg @ref HAL_TIM_HALL_SENSOR_MSPINIT_CB_ID Hall Sensor MspInit Callback ID
  *          @arg @ref HAL_TIM_HALL_SENSOR_MSPDEINIT_CB_ID Hall Sensor MspDeInit Callback ID
  *          @arg @ref HAL_TIM_PERIOD_ELAPSED_CB_ID Period Elapsed Callback ID
  *          @arg @ref HAL_TIM_PERIOD_ELAPSED_HALF_CB_ID Period Elapsed half complete Callback ID
  *          @arg @ref HAL_TIM_TRIGGER_CB_ID Trigger Callback ID
  *          @arg @ref HAL_TIM_TRIGGER_HALF_CB_ID Trigger half complete Callback ID
  *          @arg @ref HAL_TIM_IC_CAPTURE_CB_ID Input Capture Callback ID
  *          @arg @ref HAL_TIM_IC_CAPTURE_HALF_CB_ID Input Capture half complete Callback ID
  *          @arg @ref HAL_TIM_OC_DELAY_ELAPSED_CB_ID Output Compare Delay Elapsed Callback ID
  *          @arg @ref HAL_TIM_PWM_PULSE_FINISHED_CB_ID PWM Pulse Finished Callback ID
  *          @arg @ref HAL_TIM_PWM_PULSE_FINISHED_HALF_CB_ID PWM Pulse Finished half complete Callback ID
  *          @arg @ref HAL_TIM_ERROR_CB_ID Error Callback ID
  *          @arg @ref HAL_TIM_COMMUTATION_CB_ID Commutation Callback ID
  *          @arg @ref HAL_TIM_COMMUTATION_HALF_CB_ID Commutation half complete Callback ID
  *          @arg @ref HAL_TIM_BREAK_CB_ID Break Callback ID
  *          @retval status
  */
HAL_StatusTypeDef HAL_TIM_UnRegisterCallback(TIM_HandleTypeDef *htim, HAL_TIM_CallbackIDTypeDef CallbackID)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Process locked */
  __HAL_LOCK(htim);                                     /* HAL辅助宏/函数 */

  if (htim->State == HAL_TIM_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_TIM_BASE_MSPINIT_CB_ID :                 /* case标签 */
        htim->Base_MspInitCallback              = HAL_TIM_Base_MspInit;                      /* Legacy weak Base MspInit Callback */
        break;                                          /* 跳出 */

      case HAL_TIM_BASE_MSPDEINIT_CB_ID :               /* case标签 */
        htim->Base_MspDeInitCallback            = HAL_TIM_Base_MspDeInit;                    /* Legacy weak Base Msp DeInit Callback */
        break;                                          /* 跳出 */

      case HAL_TIM_IC_MSPINIT_CB_ID :                   /* case标签 */
        htim->IC_MspInitCallback                = HAL_TIM_IC_MspInit;                        /* Legacy weak IC Msp Init Callback */
        break;                                          /* 跳出 */

      case HAL_TIM_IC_MSPDEINIT_CB_ID :                 /* case标签 */
        htim->IC_MspDeInitCallback              = HAL_TIM_IC_MspDeInit;                      /* Legacy weak IC Msp DeInit Callback */
        break;                                          /* 跳出 */

      case HAL_TIM_OC_MSPINIT_CB_ID :                   /* case标签 */
        htim->OC_MspInitCallback                = HAL_TIM_OC_MspInit;                        /* Legacy weak OC Msp Init Callback */
        break;                                          /* 跳出 */

      case HAL_TIM_OC_MSPDEINIT_CB_ID :                 /* case标签 */
        htim->OC_MspDeInitCallback              = HAL_TIM_OC_MspDeInit;                      /* Legacy weak OC Msp DeInit Callback */
        break;                                          /* 跳出 */

      case HAL_TIM_PWM_MSPINIT_CB_ID :                  /* case标签 */
        htim->PWM_MspInitCallback               = HAL_TIM_PWM_MspInit;                       /* Legacy weak PWM Msp Init Callback */
        break;                                          /* 跳出 */

      case HAL_TIM_PWM_MSPDEINIT_CB_ID :                /* case标签 */
        htim->PWM_MspDeInitCallback             = HAL_TIM_PWM_MspDeInit;                     /* Legacy weak PWM Msp DeInit Callback */
        break;                                          /* 跳出 */

      case HAL_TIM_ONE_PULSE_MSPINIT_CB_ID :            /* case标签 */
        htim->OnePulse_MspInitCallback          = HAL_TIM_OnePulse_MspInit;                  /* Legacy weak One Pulse Msp Init Callback */
        break;                                          /* 跳出 */

      case HAL_TIM_ONE_PULSE_MSPDEINIT_CB_ID :          /* case标签 */
        htim->OnePulse_MspDeInitCallback        = HAL_TIM_OnePulse_MspDeInit;                /* Legacy weak One Pulse Msp DeInit Callback */
        break;                                          /* 跳出 */

      case HAL_TIM_ENCODER_MSPINIT_CB_ID :              /* case标签 */
        htim->Encoder_MspInitCallback           = HAL_TIM_Encoder_MspInit;                   /* Legacy weak Encoder Msp Init Callback */
        break;                                          /* 跳出 */

      case HAL_TIM_ENCODER_MSPDEINIT_CB_ID :            /* case标签 */
        htim->Encoder_MspDeInitCallback         = HAL_TIM_Encoder_MspDeInit;                 /* Legacy weak Encoder Msp DeInit Callback */
        break;                                          /* 跳出 */

      case HAL_TIM_HALL_SENSOR_MSPINIT_CB_ID :          /* case标签 */
        htim->HallSensor_MspInitCallback        = HAL_TIMEx_HallSensor_MspInit;              /* Legacy weak Hall Sensor Msp Init Callback */
        break;                                          /* 跳出 */

      case HAL_TIM_HALL_SENSOR_MSPDEINIT_CB_ID :        /* case标签 */
        htim->HallSensor_MspDeInitCallback      = HAL_TIMEx_HallSensor_MspDeInit;            /* Legacy weak Hall Sensor Msp DeInit Callback */
        break;                                          /* 跳出 */

      case HAL_TIM_PERIOD_ELAPSED_CB_ID :               /* case标签 */
        htim->PeriodElapsedCallback             = HAL_TIM_PeriodElapsedCallback;             /* Legacy weak Period Elapsed Callback */
        break;                                          /* 跳出 */

      case HAL_TIM_PERIOD_ELAPSED_HALF_CB_ID :          /* case标签 */
        htim->PeriodElapsedHalfCpltCallback     = HAL_TIM_PeriodElapsedHalfCpltCallback;     /* Legacy weak Period Elapsed half complete Callback */
        break;                                          /* 跳出 */

      case HAL_TIM_TRIGGER_CB_ID :                      /* case标签 */
        htim->TriggerCallback                   = HAL_TIM_TriggerCallback;                   /* Legacy weak Trigger Callback */
        break;                                          /* 跳出 */

      case HAL_TIM_TRIGGER_HALF_CB_ID :                 /* case标签 */
        htim->TriggerHalfCpltCallback           = HAL_TIM_TriggerHalfCpltCallback;           /* Legacy weak Trigger half complete Callback */
        break;                                          /* 跳出 */

      case HAL_TIM_IC_CAPTURE_CB_ID :                   /* case标签 */
        htim->IC_CaptureCallback                = HAL_TIM_IC_CaptureCallback;                /* Legacy weak IC Capture Callback */
        break;                                          /* 跳出 */

      case HAL_TIM_IC_CAPTURE_HALF_CB_ID :              /* case标签 */
        htim->IC_CaptureHalfCpltCallback        = HAL_TIM_IC_CaptureHalfCpltCallback;        /* Legacy weak IC Capture half complete Callback */
        break;                                          /* 跳出 */

      case HAL_TIM_OC_DELAY_ELAPSED_CB_ID :             /* case标签 */
        htim->OC_DelayElapsedCallback           = HAL_TIM_OC_DelayElapsedCallback;           /* Legacy weak OC Delay Elapsed Callback */
        break;                                          /* 跳出 */

      case HAL_TIM_PWM_PULSE_FINISHED_CB_ID :           /* case标签 */
        htim->PWM_PulseFinishedCallback         = HAL_TIM_PWM_PulseFinishedCallback;         /* Legacy weak PWM Pulse Finished Callback */
        break;                                          /* 跳出 */

      case HAL_TIM_PWM_PULSE_FINISHED_HALF_CB_ID :      /* case标签 */
        htim->PWM_PulseFinishedHalfCpltCallback = HAL_TIM_PWM_PulseFinishedHalfCpltCallback; /* Legacy weak PWM Pulse Finished half complete Callback */
        break;                                          /* 跳出 */

      case HAL_TIM_ERROR_CB_ID :                        /* case标签 */
        htim->ErrorCallback                     = HAL_TIM_ErrorCallback;                     /* Legacy weak Error Callback */
        break;                                          /* 跳出 */

      case HAL_TIM_COMMUTATION_CB_ID :                  /* case标签 */
        htim->CommutationCallback               = HAL_TIMEx_CommutCallback;                  /* Legacy weak Commutation Callback */
        break;                                          /* 跳出 */

      case HAL_TIM_COMMUTATION_HALF_CB_ID :             /* case标签 */
        htim->CommutationHalfCpltCallback       = HAL_TIMEx_CommutHalfCpltCallback;          /* Legacy weak Commutation half complete Callback */
        break;                                          /* 跳出 */

      case HAL_TIM_BREAK_CB_ID :                        /* case标签 */
        htim->BreakCallback                     = HAL_TIMEx_BreakCallback;                   /* Legacy weak Break Callback */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (htim->State == HAL_TIM_STATE_RESET)          /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_TIM_BASE_MSPINIT_CB_ID :                 /* case标签 */
        htim->Base_MspInitCallback         = HAL_TIM_Base_MspInit;              /* Legacy weak Base MspInit Callback */
        break;                                          /* 跳出 */

      case HAL_TIM_BASE_MSPDEINIT_CB_ID :               /* case标签 */
        htim->Base_MspDeInitCallback       = HAL_TIM_Base_MspDeInit;            /* Legacy weak Base Msp DeInit Callback */
        break;                                          /* 跳出 */

      case HAL_TIM_IC_MSPINIT_CB_ID :                   /* case标签 */
        htim->IC_MspInitCallback           = HAL_TIM_IC_MspInit;                /* Legacy weak IC Msp Init Callback */
        break;                                          /* 跳出 */

      case HAL_TIM_IC_MSPDEINIT_CB_ID :                 /* case标签 */
        htim->IC_MspDeInitCallback         = HAL_TIM_IC_MspDeInit;              /* Legacy weak IC Msp DeInit Callback */
        break;                                          /* 跳出 */

      case HAL_TIM_OC_MSPINIT_CB_ID :                   /* case标签 */
        htim->OC_MspInitCallback           = HAL_TIM_OC_MspInit;                /* Legacy weak OC Msp Init Callback */
        break;                                          /* 跳出 */

      case HAL_TIM_OC_MSPDEINIT_CB_ID :                 /* case标签 */
        htim->OC_MspDeInitCallback         = HAL_TIM_OC_MspDeInit;              /* Legacy weak OC Msp DeInit Callback */
        break;                                          /* 跳出 */

      case HAL_TIM_PWM_MSPINIT_CB_ID :                  /* case标签 */
        htim->PWM_MspInitCallback          = HAL_TIM_PWM_MspInit;               /* Legacy weak PWM Msp Init Callback */
        break;                                          /* 跳出 */

      case HAL_TIM_PWM_MSPDEINIT_CB_ID :                /* case标签 */
        htim->PWM_MspDeInitCallback        = HAL_TIM_PWM_MspDeInit;             /* Legacy weak PWM Msp DeInit Callback */
        break;                                          /* 跳出 */

      case HAL_TIM_ONE_PULSE_MSPINIT_CB_ID :            /* case标签 */
        htim->OnePulse_MspInitCallback     = HAL_TIM_OnePulse_MspInit;          /* Legacy weak One Pulse Msp Init Callback */
        break;                                          /* 跳出 */

      case HAL_TIM_ONE_PULSE_MSPDEINIT_CB_ID :          /* case标签 */
        htim->OnePulse_MspDeInitCallback   = HAL_TIM_OnePulse_MspDeInit;        /* Legacy weak One Pulse Msp DeInit Callback */
        break;                                          /* 跳出 */

      case HAL_TIM_ENCODER_MSPINIT_CB_ID :              /* case标签 */
        htim->Encoder_MspInitCallback      = HAL_TIM_Encoder_MspInit;           /* Legacy weak Encoder Msp Init Callback */
        break;                                          /* 跳出 */

      case HAL_TIM_ENCODER_MSPDEINIT_CB_ID :            /* case标签 */
        htim->Encoder_MspDeInitCallback    = HAL_TIM_Encoder_MspDeInit;         /* Legacy weak Encoder Msp DeInit Callback */
        break;                                          /* 跳出 */

      case HAL_TIM_HALL_SENSOR_MSPINIT_CB_ID :          /* case标签 */
        htim->HallSensor_MspInitCallback   = HAL_TIMEx_HallSensor_MspInit;      /* Legacy weak Hall Sensor Msp Init Callback */
        break;                                          /* 跳出 */

      case HAL_TIM_HALL_SENSOR_MSPDEINIT_CB_ID :        /* case标签 */
        htim->HallSensor_MspDeInitCallback = HAL_TIMEx_HallSensor_MspDeInit;    /* Legacy weak Hall Sensor Msp DeInit Callback */
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
  __HAL_UNLOCK(htim);                                   /* HAL辅助宏/函数 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */

/**
  * @}
  */

/** @defgroup TIM_Exported_Functions_Group10 TIM Peripheral State functions
  *  @brief   TIM Peripheral State functions
  *
@verbatim
  ==============================================================================
                        ##### Peripheral State functions #####
  ==============================================================================
    [..]
    This subsection permits to get in run-time the status of the peripheral
    and the data flow.

@endverbatim
  * @{
  */

/**
  * @brief  Return the TIM Base handle state.
  * @param  htim TIM Base handle
  * @retval HAL state
  */
HAL_TIM_StateTypeDef HAL_TIM_Base_GetState(TIM_HandleTypeDef *htim)  /* HAL 定时器操作 */
{                                                       /* 代码块开始 */
  return htim->State;                                   /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return the TIM OC handle state.
  * @param  htim TIM Output Compare handle
  * @retval HAL state
  */
HAL_TIM_StateTypeDef HAL_TIM_OC_GetState(TIM_HandleTypeDef *htim)  /* HAL 定时器操作 */
{                                                       /* 代码块开始 */
  return htim->State;                                   /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return the TIM PWM handle state.
  * @param  htim TIM handle
  * @retval HAL state
  */
HAL_TIM_StateTypeDef HAL_TIM_PWM_GetState(TIM_HandleTypeDef *htim)  /* HAL 定时器操作 */
{                                                       /* 代码块开始 */
  return htim->State;                                   /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return the TIM Input Capture handle state.
  * @param  htim TIM IC handle
  * @retval HAL state
  */
HAL_TIM_StateTypeDef HAL_TIM_IC_GetState(TIM_HandleTypeDef *htim)  /* HAL 定时器操作 */
{                                                       /* 代码块开始 */
  return htim->State;                                   /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return the TIM One Pulse Mode handle state.
  * @param  htim TIM OPM handle
  * @retval HAL state
  */
HAL_TIM_StateTypeDef HAL_TIM_OnePulse_GetState(TIM_HandleTypeDef *htim)  /* HAL 定时器操作 */
{                                                       /* 代码块开始 */
  return htim->State;                                   /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return the TIM Encoder Mode handle state.
  * @param  htim TIM Encoder Interface handle
  * @retval HAL state
  */
HAL_TIM_StateTypeDef HAL_TIM_Encoder_GetState(TIM_HandleTypeDef *htim)  /* HAL 定时器操作 */
{                                                       /* 代码块开始 */
  return htim->State;                                   /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return the TIM Encoder Mode handle state.
  * @param  htim TIM handle
  * @retval Active channel
  */
HAL_TIM_ActiveChannel HAL_TIM_GetActiveChannel(TIM_HandleTypeDef *htim)  /* HAL 定时器操作 */
{                                                       /* 代码块开始 */
  return htim->Channel;                                 /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return actual state of the TIM channel.
  * @param  htim TIM handle
  * @param  Channel TIM Channel
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1
  *            @arg TIM_CHANNEL_2: TIM Channel 2
  *            @arg TIM_CHANNEL_3: TIM Channel 3
  *            @arg TIM_CHANNEL_4: TIM Channel 4
  *            @arg TIM_CHANNEL_5: TIM Channel 5
  *            @arg TIM_CHANNEL_6: TIM Channel 6
  * @retval TIM Channel state
  */
HAL_TIM_ChannelStateTypeDef HAL_TIM_GetChannelState(TIM_HandleTypeDef *htim,  uint32_t Channel)  /* HAL 定时器操作 */
{                                                       /* 代码块开始 */
  HAL_TIM_ChannelStateTypeDef channel_state;            /* HAL 定时器操作 */

  /* Check the parameters */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));  /* 执行语句 */

  channel_state = TIM_CHANNEL_STATE_GET(htim, Channel);  /* 赋值或初始化 */

  return channel_state;                                 /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return actual state of a DMA burst operation.
  * @param  htim TIM handle
  * @retval DMA burst state
  */
HAL_TIM_DMABurstStateTypeDef HAL_TIM_DMABurstState(TIM_HandleTypeDef *htim)  /* HAL 定时器操作 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_DMABURST_INSTANCE(htim->Instance));  /* 执行语句 */

  return htim->DMABurstState;                           /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup TIM_Private_Functions TIM Private Functions
  * @{
  */

/**
  * @brief  TIM DMA error callback
  * @param  hdma pointer to DMA handle.
  * @retval None
  */
void TIM_DMAError(DMA_HandleTypeDef *hdma)              /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  TIM_HandleTypeDef *htim = (TIM_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;  /* 赋值或初始化 */

  if (hdma == htim->hdma[TIM_DMA_ID_CC1])               /* 条件判断 */
  {                                                     /* 代码块开始 */
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_1;           /* 赋值或初始化 */
    TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
  }                                                     /* 代码块结束 */
  else if (hdma == htim->hdma[TIM_DMA_ID_CC2])          /* 否则如果 */
  {                                                     /* 代码块开始 */
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_2;           /* 赋值或初始化 */
    TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
  }                                                     /* 代码块结束 */
  else if (hdma == htim->hdma[TIM_DMA_ID_CC3])          /* 否则如果 */
  {                                                     /* 代码块开始 */
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_3;           /* 赋值或初始化 */
    TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_3, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
  }                                                     /* 代码块结束 */
  else if (hdma == htim->hdma[TIM_DMA_ID_CC4])          /* 否则如果 */
  {                                                     /* 代码块开始 */
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_4;           /* 赋值或初始化 */
    TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_4, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    htim->State = HAL_TIM_STATE_READY;                  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)               /* 条件编译 */
  htim->ErrorCallback(htim);                            /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_TIM_ErrorCallback(htim);                          /* HAL 定时器操作 */
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */

  htim->Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;       /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  TIM DMA Delay Pulse complete callback.
  * @param  hdma pointer to DMA handle.
  * @retval None
  */
static void TIM_DMADelayPulseCplt(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  TIM_HandleTypeDef *htim = (TIM_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;  /* 赋值或初始化 */

  if (hdma == htim->hdma[TIM_DMA_ID_CC1])               /* 条件判断 */
  {                                                     /* 代码块开始 */
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_1;           /* 赋值或初始化 */

    if (hdma->Init.Mode == DMA_NORMAL)                  /* 条件判断 */
    {                                                   /* 代码块开始 */
      TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (hdma == htim->hdma[TIM_DMA_ID_CC2])          /* 否则如果 */
  {                                                     /* 代码块开始 */
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_2;           /* 赋值或初始化 */

    if (hdma->Init.Mode == DMA_NORMAL)                  /* 条件判断 */
    {                                                   /* 代码块开始 */
      TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (hdma == htim->hdma[TIM_DMA_ID_CC3])          /* 否则如果 */
  {                                                     /* 代码块开始 */
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_3;           /* 赋值或初始化 */

    if (hdma->Init.Mode == DMA_NORMAL)                  /* 条件判断 */
    {                                                   /* 代码块开始 */
      TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_3, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (hdma == htim->hdma[TIM_DMA_ID_CC4])          /* 否则如果 */
  {                                                     /* 代码块开始 */
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_4;           /* 赋值或初始化 */

    if (hdma->Init.Mode == DMA_NORMAL)                  /* 条件判断 */
    {                                                   /* 代码块开始 */
      TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_4, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* nothing to do */
  }                                                     /* 代码块结束 */

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)               /* 条件编译 */
  htim->PWM_PulseFinishedCallback(htim);                /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_TIM_PWM_PulseFinishedCallback(htim);              /* HAL 定时器操作 */
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */

  htim->Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;       /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  TIM DMA Delay Pulse half complete callback.
  * @param  hdma pointer to DMA handle.
  * @retval None
  */
void TIM_DMADelayPulseHalfCplt(DMA_HandleTypeDef *hdma)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  TIM_HandleTypeDef *htim = (TIM_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;  /* 赋值或初始化 */

  if (hdma == htim->hdma[TIM_DMA_ID_CC1])               /* 条件判断 */
  {                                                     /* 代码块开始 */
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_1;           /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if (hdma == htim->hdma[TIM_DMA_ID_CC2])          /* 否则如果 */
  {                                                     /* 代码块开始 */
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_2;           /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if (hdma == htim->hdma[TIM_DMA_ID_CC3])          /* 否则如果 */
  {                                                     /* 代码块开始 */
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_3;           /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if (hdma == htim->hdma[TIM_DMA_ID_CC4])          /* 否则如果 */
  {                                                     /* 代码块开始 */
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_4;           /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* nothing to do */
  }                                                     /* 代码块结束 */

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)               /* 条件编译 */
  htim->PWM_PulseFinishedHalfCpltCallback(htim);        /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_TIM_PWM_PulseFinishedHalfCpltCallback(htim);      /* HAL 定时器操作 */
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */

  htim->Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;       /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  TIM DMA Capture complete callback.
  * @param  hdma pointer to DMA handle.
  * @retval None
  */
void TIM_DMACaptureCplt(DMA_HandleTypeDef *hdma)        /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  TIM_HandleTypeDef *htim = (TIM_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;  /* 赋值或初始化 */

  if (hdma == htim->hdma[TIM_DMA_ID_CC1])               /* 条件判断 */
  {                                                     /* 代码块开始 */
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_1;           /* 赋值或初始化 */

    if (hdma->Init.Mode == DMA_NORMAL)                  /* 条件判断 */
    {                                                   /* 代码块开始 */
      TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
      TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (hdma == htim->hdma[TIM_DMA_ID_CC2])          /* 否则如果 */
  {                                                     /* 代码块开始 */
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_2;           /* 赋值或初始化 */

    if (hdma->Init.Mode == DMA_NORMAL)                  /* 条件判断 */
    {                                                   /* 代码块开始 */
      TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
      TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (hdma == htim->hdma[TIM_DMA_ID_CC3])          /* 否则如果 */
  {                                                     /* 代码块开始 */
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_3;           /* 赋值或初始化 */

    if (hdma->Init.Mode == DMA_NORMAL)                  /* 条件判断 */
    {                                                   /* 代码块开始 */
      TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_3, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
      TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_3, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (hdma == htim->hdma[TIM_DMA_ID_CC4])          /* 否则如果 */
  {                                                     /* 代码块开始 */
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_4;           /* 赋值或初始化 */

    if (hdma->Init.Mode == DMA_NORMAL)                  /* 条件判断 */
    {                                                   /* 代码块开始 */
      TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_4, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
      TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_4, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* nothing to do */
  }                                                     /* 代码块结束 */

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)               /* 条件编译 */
  htim->IC_CaptureCallback(htim);                       /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_TIM_IC_CaptureCallback(htim);                     /* HAL 定时器操作 */
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */

  htim->Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;       /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  TIM DMA Capture half complete callback.
  * @param  hdma pointer to DMA handle.
  * @retval None
  */
void TIM_DMACaptureHalfCplt(DMA_HandleTypeDef *hdma)    /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  TIM_HandleTypeDef *htim = (TIM_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;  /* 赋值或初始化 */

  if (hdma == htim->hdma[TIM_DMA_ID_CC1])               /* 条件判断 */
  {                                                     /* 代码块开始 */
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_1;           /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if (hdma == htim->hdma[TIM_DMA_ID_CC2])          /* 否则如果 */
  {                                                     /* 代码块开始 */
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_2;           /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if (hdma == htim->hdma[TIM_DMA_ID_CC3])          /* 否则如果 */
  {                                                     /* 代码块开始 */
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_3;           /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if (hdma == htim->hdma[TIM_DMA_ID_CC4])          /* 否则如果 */
  {                                                     /* 代码块开始 */
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_4;           /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* nothing to do */
  }                                                     /* 代码块结束 */

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)               /* 条件编译 */
  htim->IC_CaptureHalfCpltCallback(htim);               /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_TIM_IC_CaptureHalfCpltCallback(htim);             /* HAL 定时器操作 */
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */

  htim->Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;       /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  TIM DMA Period Elapse complete callback.
  * @param  hdma pointer to DMA handle.
  * @retval None
  */
static void TIM_DMAPeriodElapsedCplt(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  TIM_HandleTypeDef *htim = (TIM_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;  /* 赋值或初始化 */

  if (htim->hdma[TIM_DMA_ID_UPDATE]->Init.Mode == DMA_NORMAL)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    htim->State = HAL_TIM_STATE_READY;                  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)               /* 条件编译 */
  htim->PeriodElapsedCallback(htim);                    /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_TIM_PeriodElapsedCallback(htim);                  /* HAL 定时器操作 */
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
}                                                       /* 代码块结束 */

/**
  * @brief  TIM DMA Period Elapse half complete callback.
  * @param  hdma pointer to DMA handle.
  * @retval None
  */
static void TIM_DMAPeriodElapsedHalfCplt(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  TIM_HandleTypeDef *htim = (TIM_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;  /* 赋值或初始化 */

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)               /* 条件编译 */
  htim->PeriodElapsedHalfCpltCallback(htim);            /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_TIM_PeriodElapsedHalfCpltCallback(htim);          /* HAL 定时器操作 */
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
}                                                       /* 代码块结束 */

/**
  * @brief  TIM DMA Trigger callback.
  * @param  hdma pointer to DMA handle.
  * @retval None
  */
static void TIM_DMATriggerCplt(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  TIM_HandleTypeDef *htim = (TIM_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;  /* 赋值或初始化 */

  if (htim->hdma[TIM_DMA_ID_TRIGGER]->Init.Mode == DMA_NORMAL)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    htim->State = HAL_TIM_STATE_READY;                  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)               /* 条件编译 */
  htim->TriggerCallback(htim);                          /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_TIM_TriggerCallback(htim);                        /* HAL 定时器操作 */
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
}                                                       /* 代码块结束 */

/**
  * @brief  TIM DMA Trigger half complete callback.
  * @param  hdma pointer to DMA handle.
  * @retval None
  */
static void TIM_DMATriggerHalfCplt(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  TIM_HandleTypeDef *htim = (TIM_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;  /* 赋值或初始化 */

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)               /* 条件编译 */
  htim->TriggerHalfCpltCallback(htim);                  /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_TIM_TriggerHalfCpltCallback(htim);                /* HAL 定时器操作 */
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
}                                                       /* 代码块结束 */

/**
  * @brief  Time Base configuration
  * @param  TIMx TIM peripheral
  * @param  Structure TIM Base configuration structure
  * @retval None
  */
void TIM_Base_SetConfig(TIM_TypeDef *TIMx, TIM_Base_InitTypeDef *Structure)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  uint32_t tmpcr1;                                      /* 32位无符号变量/函数 */
  tmpcr1 = TIMx->CR1;                                   /* 赋值或初始化 */

  /* Set TIM Time Base Unit parameters ---------------------------------------*/
  if (IS_TIM_COUNTER_MODE_SELECT_INSTANCE(TIMx))        /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Select the Counter Mode */
    tmpcr1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS);             /* 赋值或初始化 */
    tmpcr1 |= Structure->CounterMode;                   /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if (IS_TIM_CLOCK_DIVISION_INSTANCE(TIMx))             /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Set the clock division */
    tmpcr1 &= ~TIM_CR1_CKD;                             /* 赋值或初始化 */
    tmpcr1 |= (uint32_t)Structure->ClockDivision;       /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Set the auto-reload preload */
  MODIFY_REG(tmpcr1, TIM_CR1_ARPE, Structure->AutoReloadPreload);  /* 执行语句 */

  TIMx->CR1 = tmpcr1;                                   /* 赋值或初始化 */

  /* Set the Autoreload value */
  TIMx->ARR = (uint32_t)Structure->Period ;             /* 赋值或初始化 */

  /* Set the Prescaler value */
  TIMx->PSC = Structure->Prescaler;                     /* 赋值或初始化 */

  if (IS_TIM_REPETITION_COUNTER_INSTANCE(TIMx))         /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Set the Repetition Counter value */
    TIMx->RCR = Structure->RepetitionCounter;           /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Generate an update event to reload the Prescaler
     and the repetition counter (only for advanced timer) value immediately */
  TIMx->EGR = TIM_EGR_UG;                               /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  Timer Output Compare 1 configuration
  * @param  TIMx to select the TIM peripheral
  * @param  OC_Config The output configuration structure
  * @retval None
  */
static void TIM_OC1_SetConfig(TIM_TypeDef *TIMx, TIM_OC_InitTypeDef *OC_Config)  /* 静态函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpccmrx;                                    /* 32位无符号变量/函数 */
  uint32_t tmpccer;                                     /* 32位无符号变量/函数 */
  uint32_t tmpcr2;                                      /* 32位无符号变量/函数 */

  /* Disable the Channel 1: Reset the CC1E Bit */
  TIMx->CCER &= ~TIM_CCER_CC1E;                         /* 赋值或初始化 */

  /* Get the TIMx CCER register value */
  tmpccer = TIMx->CCER;                                 /* 赋值或初始化 */
  /* Get the TIMx CR2 register value */
  tmpcr2 =  TIMx->CR2;                                  /* 赋值或初始化 */

  /* Get the TIMx CCMR1 register value */
  tmpccmrx = TIMx->CCMR1;                               /* 赋值或初始化 */

  /* Reset the Output Compare Mode Bits */
  tmpccmrx &= ~TIM_CCMR1_OC1M;                          /* 赋值或初始化 */
  tmpccmrx &= ~TIM_CCMR1_CC1S;                          /* 赋值或初始化 */
  /* Select the Output Compare Mode */
  tmpccmrx |= OC_Config->OCMode;                        /* 赋值或初始化 */

  /* Reset the Output Polarity level */
  tmpccer &= ~TIM_CCER_CC1P;                            /* 赋值或初始化 */
  /* Set the Output Compare Polarity */
  tmpccer |= OC_Config->OCPolarity;                     /* 赋值或初始化 */

  if (IS_TIM_CCXN_INSTANCE(TIMx, TIM_CHANNEL_1))        /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check parameters */
    assert_param(IS_TIM_OCN_POLARITY(OC_Config->OCNPolarity));  /* 执行语句 */

    /* Reset the Output N Polarity level */
    tmpccer &= ~TIM_CCER_CC1NP;                         /* 赋值或初始化 */
    /* Set the Output N Polarity */
    tmpccer |= OC_Config->OCNPolarity;                  /* 赋值或初始化 */
    /* Reset the Output N State */
    tmpccer &= ~TIM_CCER_CC1NE;                         /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if (IS_TIM_BREAK_INSTANCE(TIMx))                      /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check parameters */
    assert_param(IS_TIM_OCNIDLE_STATE(OC_Config->OCNIdleState));  /* 执行语句 */
    assert_param(IS_TIM_OCIDLE_STATE(OC_Config->OCIdleState));  /* 执行语句 */

    /* Reset the Output Compare and Output Compare N IDLE State */
    tmpcr2 &= ~TIM_CR2_OIS1;                            /* 赋值或初始化 */
    tmpcr2 &= ~TIM_CR2_OIS1N;                           /* 赋值或初始化 */
    /* Set the Output Idle state */
    tmpcr2 |= OC_Config->OCIdleState;                   /* 赋值或初始化 */
    /* Set the Output N Idle state */
    tmpcr2 |= OC_Config->OCNIdleState;                  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Write to TIMx CR2 */
  TIMx->CR2 = tmpcr2;                                   /* 赋值或初始化 */

  /* Write to TIMx CCMR1 */
  TIMx->CCMR1 = tmpccmrx;                               /* 赋值或初始化 */

  /* Set the Capture Compare Register value */
  TIMx->CCR1 = OC_Config->Pulse;                        /* 赋值或初始化 */

  /* Write to TIMx CCER */
  TIMx->CCER = tmpccer;                                 /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  Timer Output Compare 2 configuration
  * @param  TIMx to select the TIM peripheral
  * @param  OC_Config The output configuration structure
  * @retval None
  */
void TIM_OC2_SetConfig(TIM_TypeDef *TIMx, TIM_OC_InitTypeDef *OC_Config)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  uint32_t tmpccmrx;                                    /* 32位无符号变量/函数 */
  uint32_t tmpccer;                                     /* 32位无符号变量/函数 */
  uint32_t tmpcr2;                                      /* 32位无符号变量/函数 */

  /* Disable the Channel 2: Reset the CC2E Bit */
  TIMx->CCER &= ~TIM_CCER_CC2E;                         /* 赋值或初始化 */

  /* Get the TIMx CCER register value */
  tmpccer = TIMx->CCER;                                 /* 赋值或初始化 */
  /* Get the TIMx CR2 register value */
  tmpcr2 =  TIMx->CR2;                                  /* 赋值或初始化 */

  /* Get the TIMx CCMR1 register value */
  tmpccmrx = TIMx->CCMR1;                               /* 赋值或初始化 */

  /* Reset the Output Compare mode and Capture/Compare selection Bits */
  tmpccmrx &= ~TIM_CCMR1_OC2M;                          /* 赋值或初始化 */
  tmpccmrx &= ~TIM_CCMR1_CC2S;                          /* 赋值或初始化 */

  /* Select the Output Compare Mode */
  tmpccmrx |= (OC_Config->OCMode << 8U);                /* 赋值或初始化 */

  /* Reset the Output Polarity level */
  tmpccer &= ~TIM_CCER_CC2P;                            /* 赋值或初始化 */
  /* Set the Output Compare Polarity */
  tmpccer |= (OC_Config->OCPolarity << 4U);             /* 赋值或初始化 */

  if (IS_TIM_CCXN_INSTANCE(TIMx, TIM_CHANNEL_2))        /* 条件判断 */
  {                                                     /* 代码块开始 */
    assert_param(IS_TIM_OCN_POLARITY(OC_Config->OCNPolarity));  /* 执行语句 */

    /* Reset the Output N Polarity level */
    tmpccer &= ~TIM_CCER_CC2NP;                         /* 赋值或初始化 */
    /* Set the Output N Polarity */
    tmpccer |= (OC_Config->OCNPolarity << 4U);          /* 赋值或初始化 */
    /* Reset the Output N State */
    tmpccer &= ~TIM_CCER_CC2NE;                         /* 赋值或初始化 */

  }                                                     /* 代码块结束 */

  if (IS_TIM_BREAK_INSTANCE(TIMx))                      /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check parameters */
    assert_param(IS_TIM_OCNIDLE_STATE(OC_Config->OCNIdleState));  /* 执行语句 */
    assert_param(IS_TIM_OCIDLE_STATE(OC_Config->OCIdleState));  /* 执行语句 */

    /* Reset the Output Compare and Output Compare N IDLE State */
    tmpcr2 &= ~TIM_CR2_OIS2;                            /* 赋值或初始化 */
    tmpcr2 &= ~TIM_CR2_OIS2N;                           /* 赋值或初始化 */
    /* Set the Output Idle state */
    tmpcr2 |= (OC_Config->OCIdleState << 2U);           /* 赋值或初始化 */
    /* Set the Output N Idle state */
    tmpcr2 |= (OC_Config->OCNIdleState << 2U);          /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Write to TIMx CR2 */
  TIMx->CR2 = tmpcr2;                                   /* 赋值或初始化 */

  /* Write to TIMx CCMR1 */
  TIMx->CCMR1 = tmpccmrx;                               /* 赋值或初始化 */

  /* Set the Capture Compare Register value */
  TIMx->CCR2 = OC_Config->Pulse;                        /* 赋值或初始化 */

  /* Write to TIMx CCER */
  TIMx->CCER = tmpccer;                                 /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  Timer Output Compare 3 configuration
  * @param  TIMx to select the TIM peripheral
  * @param  OC_Config The output configuration structure
  * @retval None
  */
static void TIM_OC3_SetConfig(TIM_TypeDef *TIMx, TIM_OC_InitTypeDef *OC_Config)  /* 静态函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpccmrx;                                    /* 32位无符号变量/函数 */
  uint32_t tmpccer;                                     /* 32位无符号变量/函数 */
  uint32_t tmpcr2;                                      /* 32位无符号变量/函数 */

  /* Disable the Channel 3: Reset the CC2E Bit */
  TIMx->CCER &= ~TIM_CCER_CC3E;                         /* 赋值或初始化 */

  /* Get the TIMx CCER register value */
  tmpccer = TIMx->CCER;                                 /* 赋值或初始化 */
  /* Get the TIMx CR2 register value */
  tmpcr2 =  TIMx->CR2;                                  /* 赋值或初始化 */

  /* Get the TIMx CCMR2 register value */
  tmpccmrx = TIMx->CCMR2;                               /* 赋值或初始化 */

  /* Reset the Output Compare mode and Capture/Compare selection Bits */
  tmpccmrx &= ~TIM_CCMR2_OC3M;                          /* 赋值或初始化 */
  tmpccmrx &= ~TIM_CCMR2_CC3S;                          /* 赋值或初始化 */
  /* Select the Output Compare Mode */
  tmpccmrx |= OC_Config->OCMode;                        /* 赋值或初始化 */

  /* Reset the Output Polarity level */
  tmpccer &= ~TIM_CCER_CC3P;                            /* 赋值或初始化 */
  /* Set the Output Compare Polarity */
  tmpccer |= (OC_Config->OCPolarity << 8U);             /* 赋值或初始化 */

  if (IS_TIM_CCXN_INSTANCE(TIMx, TIM_CHANNEL_3))        /* 条件判断 */
  {                                                     /* 代码块开始 */
    assert_param(IS_TIM_OCN_POLARITY(OC_Config->OCNPolarity));  /* 执行语句 */

    /* Reset the Output N Polarity level */
    tmpccer &= ~TIM_CCER_CC3NP;                         /* 赋值或初始化 */
    /* Set the Output N Polarity */
    tmpccer |= (OC_Config->OCNPolarity << 8U);          /* 赋值或初始化 */
    /* Reset the Output N State */
    tmpccer &= ~TIM_CCER_CC3NE;                         /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if (IS_TIM_BREAK_INSTANCE(TIMx))                      /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check parameters */
    assert_param(IS_TIM_OCNIDLE_STATE(OC_Config->OCNIdleState));  /* 执行语句 */
    assert_param(IS_TIM_OCIDLE_STATE(OC_Config->OCIdleState));  /* 执行语句 */

    /* Reset the Output Compare and Output Compare N IDLE State */
    tmpcr2 &= ~TIM_CR2_OIS3;                            /* 赋值或初始化 */
    tmpcr2 &= ~TIM_CR2_OIS3N;                           /* 赋值或初始化 */
    /* Set the Output Idle state */
    tmpcr2 |= (OC_Config->OCIdleState << 4U);           /* 赋值或初始化 */
    /* Set the Output N Idle state */
    tmpcr2 |= (OC_Config->OCNIdleState << 4U);          /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Write to TIMx CR2 */
  TIMx->CR2 = tmpcr2;                                   /* 赋值或初始化 */

  /* Write to TIMx CCMR2 */
  TIMx->CCMR2 = tmpccmrx;                               /* 赋值或初始化 */

  /* Set the Capture Compare Register value */
  TIMx->CCR3 = OC_Config->Pulse;                        /* 赋值或初始化 */

  /* Write to TIMx CCER */
  TIMx->CCER = tmpccer;                                 /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  Timer Output Compare 4 configuration
  * @param  TIMx to select the TIM peripheral
  * @param  OC_Config The output configuration structure
  * @retval None
  */
static void TIM_OC4_SetConfig(TIM_TypeDef *TIMx, TIM_OC_InitTypeDef *OC_Config)  /* 静态函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpccmrx;                                    /* 32位无符号变量/函数 */
  uint32_t tmpccer;                                     /* 32位无符号变量/函数 */
  uint32_t tmpcr2;                                      /* 32位无符号变量/函数 */

  /* Disable the Channel 4: Reset the CC4E Bit */
  TIMx->CCER &= ~TIM_CCER_CC4E;                         /* 赋值或初始化 */

  /* Get the TIMx CCER register value */
  tmpccer = TIMx->CCER;                                 /* 赋值或初始化 */
  /* Get the TIMx CR2 register value */
  tmpcr2 =  TIMx->CR2;                                  /* 赋值或初始化 */

  /* Get the TIMx CCMR2 register value */
  tmpccmrx = TIMx->CCMR2;                               /* 赋值或初始化 */

  /* Reset the Output Compare mode and Capture/Compare selection Bits */
  tmpccmrx &= ~TIM_CCMR2_OC4M;                          /* 赋值或初始化 */
  tmpccmrx &= ~TIM_CCMR2_CC4S;                          /* 赋值或初始化 */

  /* Select the Output Compare Mode */
  tmpccmrx |= (OC_Config->OCMode << 8U);                /* 赋值或初始化 */

  /* Reset the Output Polarity level */
  tmpccer &= ~TIM_CCER_CC4P;                            /* 赋值或初始化 */
  /* Set the Output Compare Polarity */
  tmpccer |= (OC_Config->OCPolarity << 12U);            /* 赋值或初始化 */

  if (IS_TIM_BREAK_INSTANCE(TIMx))                      /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check parameters */
    assert_param(IS_TIM_OCIDLE_STATE(OC_Config->OCIdleState));  /* 执行语句 */

    /* Reset the Output Compare IDLE State */
    tmpcr2 &= ~TIM_CR2_OIS4;                            /* 赋值或初始化 */

    /* Set the Output Idle state */
    tmpcr2 |= (OC_Config->OCIdleState << 6U);           /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Write to TIMx CR2 */
  TIMx->CR2 = tmpcr2;                                   /* 赋值或初始化 */

  /* Write to TIMx CCMR2 */
  TIMx->CCMR2 = tmpccmrx;                               /* 赋值或初始化 */

  /* Set the Capture Compare Register value */
  TIMx->CCR4 = OC_Config->Pulse;                        /* 赋值或初始化 */

  /* Write to TIMx CCER */
  TIMx->CCER = tmpccer;                                 /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  Slave Timer configuration function
  * @param  htim TIM handle
  * @param  sSlaveConfig Slave timer configuration
  * @retval None
  */
static HAL_StatusTypeDef TIM_SlaveTimer_SetConfig(TIM_HandleTypeDef *htim,  /* 静态变量/函数 */
                                                  TIM_SlaveConfigTypeDef *sSlaveConfig)  /* 源代码 */
{                                                       /* 代码块开始 */
  uint32_t tmpsmcr;                                     /* 32位无符号变量/函数 */
  uint32_t tmpccmr1;                                    /* 32位无符号变量/函数 */
  uint32_t tmpccer;                                     /* 32位无符号变量/函数 */

  /* Get the TIMx SMCR register value */
  tmpsmcr = htim->Instance->SMCR;                       /* 赋值或初始化 */

  /* Reset the Trigger Selection Bits */
  tmpsmcr &= ~TIM_SMCR_TS;                              /* 赋值或初始化 */
  /* Set the Input Trigger source */
  tmpsmcr |= sSlaveConfig->InputTrigger;                /* 赋值或初始化 */

  /* Reset the slave mode Bits */
  tmpsmcr &= ~TIM_SMCR_SMS;                             /* 赋值或初始化 */
  /* Set the slave mode */
  tmpsmcr |= sSlaveConfig->SlaveMode;                   /* 赋值或初始化 */

  /* Write to TIMx SMCR */
  htim->Instance->SMCR = tmpsmcr;                       /* 赋值或初始化 */

  /* Configure the trigger prescaler, filter, and polarity */
  switch (sSlaveConfig->InputTrigger)                   /* switch分支 */
  {                                                     /* 代码块开始 */
    case TIM_TS_ETRF:                                   /* case标签 */
    {                                                   /* 代码块开始 */
      /* Check the parameters */
      assert_param(IS_TIM_CLOCKSOURCE_ETRMODE1_INSTANCE(htim->Instance));  /* 执行语句 */
      assert_param(IS_TIM_TRIGGERPRESCALER(sSlaveConfig->TriggerPrescaler));  /* 执行语句 */
      assert_param(IS_TIM_TRIGGERPOLARITY(sSlaveConfig->TriggerPolarity));  /* 执行语句 */
      assert_param(IS_TIM_TRIGGERFILTER(sSlaveConfig->TriggerFilter));  /* 执行语句 */
      /* Configure the ETR Trigger source */
      TIM_ETR_SetConfig(htim->Instance,                 /* 列表续行 */
                        sSlaveConfig->TriggerPrescaler,  /* 列表续行 */
                        sSlaveConfig->TriggerPolarity,  /* 列表续行 */
                        sSlaveConfig->TriggerFilter);   /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_TS_TI1F_ED:                                /* case标签 */
    {                                                   /* 代码块开始 */
      /* Check the parameters */
      assert_param(IS_TIM_CC1_INSTANCE(htim->Instance));  /* 执行语句 */
      assert_param(IS_TIM_TRIGGERFILTER(sSlaveConfig->TriggerFilter));  /* 执行语句 */

      if (sSlaveConfig->SlaveMode == TIM_SLAVEMODE_GATED)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */

      /* Disable the Channel 1: Reset the CC1E Bit */
      tmpccer = htim->Instance->CCER;                   /* 赋值或初始化 */
      htim->Instance->CCER &= ~TIM_CCER_CC1E;           /* 赋值或初始化 */
      tmpccmr1 = htim->Instance->CCMR1;                 /* 赋值或初始化 */

      /* Set the filter */
      tmpccmr1 &= ~TIM_CCMR1_IC1F;                      /* 赋值或初始化 */
      tmpccmr1 |= ((sSlaveConfig->TriggerFilter) << 4U);  /* 赋值或初始化 */

      /* Write to TIMx CCMR1 and CCER registers */
      htim->Instance->CCMR1 = tmpccmr1;                 /* 赋值或初始化 */
      htim->Instance->CCER = tmpccer;                   /* 赋值或初始化 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_TS_TI1FP1:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Check the parameters */
      assert_param(IS_TIM_CC1_INSTANCE(htim->Instance));  /* 执行语句 */
      assert_param(IS_TIM_TRIGGERPOLARITY(sSlaveConfig->TriggerPolarity));  /* 执行语句 */
      assert_param(IS_TIM_TRIGGERFILTER(sSlaveConfig->TriggerFilter));  /* 执行语句 */

      /* Configure TI1 Filter and Polarity */
      TIM_TI1_ConfigInputStage(htim->Instance,          /* 列表续行 */
                               sSlaveConfig->TriggerPolarity,  /* 列表续行 */
                               sSlaveConfig->TriggerFilter);  /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_TS_TI2FP2:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Check the parameters */
      assert_param(IS_TIM_CC2_INSTANCE(htim->Instance));  /* 执行语句 */
      assert_param(IS_TIM_TRIGGERPOLARITY(sSlaveConfig->TriggerPolarity));  /* 执行语句 */
      assert_param(IS_TIM_TRIGGERFILTER(sSlaveConfig->TriggerFilter));  /* 执行语句 */

      /* Configure TI2 Filter and Polarity */
      TIM_TI2_ConfigInputStage(htim->Instance,          /* 列表续行 */
                               sSlaveConfig->TriggerPolarity,  /* 列表续行 */
                               sSlaveConfig->TriggerFilter);  /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_TS_ITR0:                                   /* case标签 */
    case TIM_TS_ITR1:                                   /* case标签 */
    case TIM_TS_ITR2:                                   /* case标签 */
    case TIM_TS_ITR3:                                   /* case标签 */
      {                                                 /* 代码块开始 */
        /* Check the parameter */
        assert_param(IS_TIM_CC2_INSTANCE(htim->Instance));  /* 执行语句 */
        break;                                          /* 跳出 */
      }                                                 /* 代码块结束 */

    default:                                            /* default分支 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configure the TI1 as Input.
  * @param  TIMx to select the TIM peripheral.
  * @param  TIM_ICPolarity The Input Polarity.
  *          This parameter can be one of the following values:
  *            @arg TIM_ICPOLARITY_RISING
  *            @arg TIM_ICPOLARITY_FALLING
  *            @arg TIM_ICPOLARITY_BOTHEDGE
  * @param  TIM_ICSelection specifies the input to be used.
  *          This parameter can be one of the following values:
  *            @arg TIM_ICSELECTION_DIRECTTI: TIM Input 1 is selected to be connected to IC1.
  *            @arg TIM_ICSELECTION_INDIRECTTI: TIM Input 1 is selected to be connected to IC2.
  *            @arg TIM_ICSELECTION_TRC: TIM Input 1 is selected to be connected to TRC.
  * @param  TIM_ICFilter Specifies the Input Capture Filter.
  *          This parameter must be a value between 0x00 and 0x0F.
  * @retval None
  * @note TIM_ICFilter and TIM_ICPolarity are not used in INDIRECT mode as TI2FP1
  *       (on channel2 path) is used as the input signal. Therefore CCMR1 must be
  *        protected against un-initialized filter and polarity values.
  */
void TIM_TI1_SetConfig(TIM_TypeDef *TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICSelection,  /* 函数定义或声明 */
                       uint32_t TIM_ICFilter)           /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpccmr1;                                    /* 32位无符号变量/函数 */
  uint32_t tmpccer;                                     /* 32位无符号变量/函数 */

  /* Disable the Channel 1: Reset the CC1E Bit */
  TIMx->CCER &= ~TIM_CCER_CC1E;                         /* 赋值或初始化 */
  tmpccmr1 = TIMx->CCMR1;                               /* 赋值或初始化 */
  tmpccer = TIMx->CCER;                                 /* 赋值或初始化 */

  /* Select the Input */
  if (IS_TIM_CC2_INSTANCE(TIMx) != RESET)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    tmpccmr1 &= ~TIM_CCMR1_CC1S;                        /* 赋值或初始化 */
    tmpccmr1 |= TIM_ICSelection;                        /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    tmpccmr1 |= TIM_CCMR1_CC1S_0;                       /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Set the filter */
  tmpccmr1 &= ~TIM_CCMR1_IC1F;                          /* 赋值或初始化 */
  tmpccmr1 |= ((TIM_ICFilter << 4U) & TIM_CCMR1_IC1F);  /* 赋值或初始化 */

  /* Select the Polarity and set the CC1E Bit */
  tmpccer &= ~(TIM_CCER_CC1P | TIM_CCER_CC1NP);         /* 赋值或初始化 */
  tmpccer |= (TIM_ICPolarity & (TIM_CCER_CC1P | TIM_CCER_CC1NP));  /* 赋值或初始化 */

  /* Write to TIMx CCMR1 and CCER registers */
  TIMx->CCMR1 = tmpccmr1;                               /* 赋值或初始化 */
  TIMx->CCER = tmpccer;                                 /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configure the Polarity and Filter for TI1.
  * @param  TIMx to select the TIM peripheral.
  * @param  TIM_ICPolarity The Input Polarity.
  *          This parameter can be one of the following values:
  *            @arg TIM_ICPOLARITY_RISING
  *            @arg TIM_ICPOLARITY_FALLING
  *            @arg TIM_ICPOLARITY_BOTHEDGE
  * @param  TIM_ICFilter Specifies the Input Capture Filter.
  *          This parameter must be a value between 0x00 and 0x0F.
  * @retval None
  */
static void TIM_TI1_ConfigInputStage(TIM_TypeDef *TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICFilter)  /* 静态函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpccmr1;                                    /* 32位无符号变量/函数 */
  uint32_t tmpccer;                                     /* 32位无符号变量/函数 */

  /* Disable the Channel 1: Reset the CC1E Bit */
  tmpccer = TIMx->CCER;                                 /* 赋值或初始化 */
  TIMx->CCER &= ~TIM_CCER_CC1E;                         /* 赋值或初始化 */
  tmpccmr1 = TIMx->CCMR1;                               /* 赋值或初始化 */

  /* Set the filter */
  tmpccmr1 &= ~TIM_CCMR1_IC1F;                          /* 赋值或初始化 */
  tmpccmr1 |= (TIM_ICFilter << 4U);                     /* 赋值或初始化 */

  /* Select the Polarity and set the CC1E Bit */
  tmpccer &= ~(TIM_CCER_CC1P | TIM_CCER_CC1NP);         /* 赋值或初始化 */
  tmpccer |= TIM_ICPolarity;                            /* 赋值或初始化 */

  /* Write to TIMx CCMR1 and CCER registers */
  TIMx->CCMR1 = tmpccmr1;                               /* 赋值或初始化 */
  TIMx->CCER = tmpccer;                                 /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configure the TI2 as Input.
  * @param  TIMx to select the TIM peripheral
  * @param  TIM_ICPolarity The Input Polarity.
  *          This parameter can be one of the following values:
  *            @arg TIM_ICPOLARITY_RISING
  *            @arg TIM_ICPOLARITY_FALLING
  *            @arg TIM_ICPOLARITY_BOTHEDGE
  * @param  TIM_ICSelection specifies the input to be used.
  *          This parameter can be one of the following values:
  *            @arg TIM_ICSELECTION_DIRECTTI: TIM Input 2 is selected to be connected to IC2.
  *            @arg TIM_ICSELECTION_INDIRECTTI: TIM Input 2 is selected to be connected to IC1.
  *            @arg TIM_ICSELECTION_TRC: TIM Input 2 is selected to be connected to TRC.
  * @param  TIM_ICFilter Specifies the Input Capture Filter.
  *          This parameter must be a value between 0x00 and 0x0F.
  * @retval None
  * @note TIM_ICFilter and TIM_ICPolarity are not used in INDIRECT mode as TI1FP2
  *       (on channel1 path) is used as the input signal. Therefore CCMR1 must be
  *        protected against un-initialized filter and polarity values.
  */
static void TIM_TI2_SetConfig(TIM_TypeDef *TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICSelection,  /* 静态函数 */
                              uint32_t TIM_ICFilter)    /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpccmr1;                                    /* 32位无符号变量/函数 */
  uint32_t tmpccer;                                     /* 32位无符号变量/函数 */

  /* Disable the Channel 2: Reset the CC2E Bit */
  TIMx->CCER &= ~TIM_CCER_CC2E;                         /* 赋值或初始化 */
  tmpccmr1 = TIMx->CCMR1;                               /* 赋值或初始化 */
  tmpccer = TIMx->CCER;                                 /* 赋值或初始化 */

  /* Select the Input */
  tmpccmr1 &= ~TIM_CCMR1_CC2S;                          /* 赋值或初始化 */
  tmpccmr1 |= (TIM_ICSelection << 8U);                  /* 赋值或初始化 */

  /* Set the filter */
  tmpccmr1 &= ~TIM_CCMR1_IC2F;                          /* 赋值或初始化 */
  tmpccmr1 |= ((TIM_ICFilter << 12U) & TIM_CCMR1_IC2F);  /* 赋值或初始化 */

  /* Select the Polarity and set the CC2E Bit */
  tmpccer &= ~(TIM_CCER_CC2P | TIM_CCER_CC2NP);         /* 赋值或初始化 */
  tmpccer |= ((TIM_ICPolarity << 4U) & (TIM_CCER_CC2P | TIM_CCER_CC2NP));  /* 赋值或初始化 */

  /* Write to TIMx CCMR1 and CCER registers */
  TIMx->CCMR1 = tmpccmr1 ;                              /* 赋值或初始化 */
  TIMx->CCER = tmpccer;                                 /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configure the Polarity and Filter for TI2.
  * @param  TIMx to select the TIM peripheral.
  * @param  TIM_ICPolarity The Input Polarity.
  *          This parameter can be one of the following values:
  *            @arg TIM_ICPOLARITY_RISING
  *            @arg TIM_ICPOLARITY_FALLING
  *            @arg TIM_ICPOLARITY_BOTHEDGE
  * @param  TIM_ICFilter Specifies the Input Capture Filter.
  *          This parameter must be a value between 0x00 and 0x0F.
  * @retval None
  */
static void TIM_TI2_ConfigInputStage(TIM_TypeDef *TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICFilter)  /* 静态函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpccmr1;                                    /* 32位无符号变量/函数 */
  uint32_t tmpccer;                                     /* 32位无符号变量/函数 */

  /* Disable the Channel 2: Reset the CC2E Bit */
  TIMx->CCER &= ~TIM_CCER_CC2E;                         /* 赋值或初始化 */
  tmpccmr1 = TIMx->CCMR1;                               /* 赋值或初始化 */
  tmpccer = TIMx->CCER;                                 /* 赋值或初始化 */

  /* Set the filter */
  tmpccmr1 &= ~TIM_CCMR1_IC2F;                          /* 赋值或初始化 */
  tmpccmr1 |= (TIM_ICFilter << 12U);                    /* 赋值或初始化 */

  /* Select the Polarity and set the CC2E Bit */
  tmpccer &= ~(TIM_CCER_CC2P | TIM_CCER_CC2NP);         /* 赋值或初始化 */
  tmpccer |= (TIM_ICPolarity << 4U);                    /* 赋值或初始化 */

  /* Write to TIMx CCMR1 and CCER registers */
  TIMx->CCMR1 = tmpccmr1 ;                              /* 赋值或初始化 */
  TIMx->CCER = tmpccer;                                 /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configure the TI3 as Input.
  * @param  TIMx to select the TIM peripheral
  * @param  TIM_ICPolarity The Input Polarity.
  *          This parameter can be one of the following values:
  *            @arg TIM_ICPOLARITY_RISING
  *            @arg TIM_ICPOLARITY_FALLING
  *            @arg TIM_ICPOLARITY_BOTHEDGE
  * @param  TIM_ICSelection specifies the input to be used.
  *          This parameter can be one of the following values:
  *            @arg TIM_ICSELECTION_DIRECTTI: TIM Input 3 is selected to be connected to IC3.
  *            @arg TIM_ICSELECTION_INDIRECTTI: TIM Input 3 is selected to be connected to IC4.
  *            @arg TIM_ICSELECTION_TRC: TIM Input 3 is selected to be connected to TRC.
  * @param  TIM_ICFilter Specifies the Input Capture Filter.
  *          This parameter must be a value between 0x00 and 0x0F.
  * @retval None
  * @note TIM_ICFilter and TIM_ICPolarity are not used in INDIRECT mode as TI3FP4
  *       (on channel1 path) is used as the input signal. Therefore CCMR2 must be
  *        protected against un-initialized filter and polarity values.
  */
static void TIM_TI3_SetConfig(TIM_TypeDef *TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICSelection,  /* 静态函数 */
                              uint32_t TIM_ICFilter)    /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpccmr2;                                    /* 32位无符号变量/函数 */
  uint32_t tmpccer;                                     /* 32位无符号变量/函数 */

  /* Disable the Channel 3: Reset the CC3E Bit */
  TIMx->CCER &= ~TIM_CCER_CC3E;                         /* 赋值或初始化 */
  tmpccmr2 = TIMx->CCMR2;                               /* 赋值或初始化 */
  tmpccer = TIMx->CCER;                                 /* 赋值或初始化 */

  /* Select the Input */
  tmpccmr2 &= ~TIM_CCMR2_CC3S;                          /* 赋值或初始化 */
  tmpccmr2 |= TIM_ICSelection;                          /* 赋值或初始化 */

  /* Set the filter */
  tmpccmr2 &= ~TIM_CCMR2_IC3F;                          /* 赋值或初始化 */
  tmpccmr2 |= ((TIM_ICFilter << 4U) & TIM_CCMR2_IC3F);  /* 赋值或初始化 */

  /* Select the Polarity and set the CC3E Bit */
  tmpccer &= ~(TIM_CCER_CC3P | TIM_CCER_CC3NP);         /* 赋值或初始化 */
  tmpccer |= ((TIM_ICPolarity << 8U) & (TIM_CCER_CC3P | TIM_CCER_CC3NP));  /* 赋值或初始化 */

  /* Write to TIMx CCMR2 and CCER registers */
  TIMx->CCMR2 = tmpccmr2;                               /* 赋值或初始化 */
  TIMx->CCER = tmpccer;                                 /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configure the TI4 as Input.
  * @param  TIMx to select the TIM peripheral
  * @param  TIM_ICPolarity The Input Polarity.
  *          This parameter can be one of the following values:
  *            @arg TIM_ICPOLARITY_RISING
  *            @arg TIM_ICPOLARITY_FALLING
  *            @arg TIM_ICPOLARITY_BOTHEDGE
  * @param  TIM_ICSelection specifies the input to be used.
  *          This parameter can be one of the following values:
  *            @arg TIM_ICSELECTION_DIRECTTI: TIM Input 4 is selected to be connected to IC4.
  *            @arg TIM_ICSELECTION_INDIRECTTI: TIM Input 4 is selected to be connected to IC3.
  *            @arg TIM_ICSELECTION_TRC: TIM Input 4 is selected to be connected to TRC.
  * @param  TIM_ICFilter Specifies the Input Capture Filter.
  *          This parameter must be a value between 0x00 and 0x0F.
  * @note TIM_ICFilter and TIM_ICPolarity are not used in INDIRECT mode as TI4FP3
  *       (on channel1 path) is used as the input signal. Therefore CCMR2 must be
  *        protected against un-initialized filter and polarity values.
  * @retval None
  */
static void TIM_TI4_SetConfig(TIM_TypeDef *TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICSelection,  /* 静态函数 */
                              uint32_t TIM_ICFilter)    /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpccmr2;                                    /* 32位无符号变量/函数 */
  uint32_t tmpccer;                                     /* 32位无符号变量/函数 */

  /* Disable the Channel 4: Reset the CC4E Bit */
  TIMx->CCER &= ~TIM_CCER_CC4E;                         /* 赋值或初始化 */
  tmpccmr2 = TIMx->CCMR2;                               /* 赋值或初始化 */
  tmpccer = TIMx->CCER;                                 /* 赋值或初始化 */

  /* Select the Input */
  tmpccmr2 &= ~TIM_CCMR2_CC4S;                          /* 赋值或初始化 */
  tmpccmr2 |= (TIM_ICSelection << 8U);                  /* 赋值或初始化 */

  /* Set the filter */
  tmpccmr2 &= ~TIM_CCMR2_IC4F;                          /* 赋值或初始化 */
  tmpccmr2 |= ((TIM_ICFilter << 12U) & TIM_CCMR2_IC4F);  /* 赋值或初始化 */

  /* Select the Polarity and set the CC4E Bit */
  tmpccer &= ~(TIM_CCER_CC4P | TIM_CCER_CC4NP);         /* 赋值或初始化 */
  tmpccer |= ((TIM_ICPolarity << 12U) & (TIM_CCER_CC4P | TIM_CCER_CC4NP));  /* 赋值或初始化 */

  /* Write to TIMx CCMR2 and CCER registers */
  TIMx->CCMR2 = tmpccmr2;                               /* 赋值或初始化 */
  TIMx->CCER = tmpccer ;                                /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  Selects the Input Trigger source
  * @param  TIMx to select the TIM peripheral
  * @param  InputTriggerSource The Input Trigger source.
  *          This parameter can be one of the following values:
  *            @arg TIM_TS_ITR0: Internal Trigger 0
  *            @arg TIM_TS_ITR1: Internal Trigger 1
  *            @arg TIM_TS_ITR2: Internal Trigger 2
  *            @arg TIM_TS_ITR3: Internal Trigger 3
  *            @arg TIM_TS_TI1F_ED: TI1 Edge Detector
  *            @arg TIM_TS_TI1FP1: Filtered Timer Input 1
  *            @arg TIM_TS_TI2FP2: Filtered Timer Input 2
  *            @arg TIM_TS_ETRF: External Trigger input
  * @retval None
  */
static void TIM_ITRx_SetConfig(TIM_TypeDef *TIMx, uint32_t InputTriggerSource)  /* 静态函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpsmcr;                                     /* 32位无符号变量/函数 */

  /* Get the TIMx SMCR register value */
  tmpsmcr = TIMx->SMCR;                                 /* 赋值或初始化 */
  /* Reset the TS Bits */
  tmpsmcr &= ~TIM_SMCR_TS;                              /* 赋值或初始化 */
  /* Set the Input Trigger source and the slave mode*/
  tmpsmcr |= (InputTriggerSource | TIM_SLAVEMODE_EXTERNAL1);  /* 赋值或初始化 */
  /* Write to TIMx SMCR */
  TIMx->SMCR = tmpsmcr;                                 /* 赋值或初始化 */
}                                                       /* 代码块结束 */
/**
  * @brief  Configures the TIMx External Trigger (ETR).
  * @param  TIMx to select the TIM peripheral
  * @param  TIM_ExtTRGPrescaler The external Trigger Prescaler.
  *          This parameter can be one of the following values:
  *            @arg TIM_ETRPRESCALER_DIV1: ETRP Prescaler OFF.
  *            @arg TIM_ETRPRESCALER_DIV2: ETRP frequency divided by 2.
  *            @arg TIM_ETRPRESCALER_DIV4: ETRP frequency divided by 4.
  *            @arg TIM_ETRPRESCALER_DIV8: ETRP frequency divided by 8.
  * @param  TIM_ExtTRGPolarity The external Trigger Polarity.
  *          This parameter can be one of the following values:
  *            @arg TIM_ETRPOLARITY_INVERTED: active low or falling edge active.
  *            @arg TIM_ETRPOLARITY_NONINVERTED: active high or rising edge active.
  * @param  ExtTRGFilter External Trigger Filter.
  *          This parameter must be a value between 0x00 and 0x0F
  * @retval None
  */
void TIM_ETR_SetConfig(TIM_TypeDef *TIMx, uint32_t TIM_ExtTRGPrescaler,  /* 函数定义或声明 */
                       uint32_t TIM_ExtTRGPolarity, uint32_t ExtTRGFilter)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpsmcr;                                     /* 32位无符号变量/函数 */

  tmpsmcr = TIMx->SMCR;                                 /* 赋值或初始化 */

  /* Reset the ETR Bits */
  tmpsmcr &= ~(TIM_SMCR_ETF | TIM_SMCR_ETPS | TIM_SMCR_ECE | TIM_SMCR_ETP);  /* 赋值或初始化 */

  /* Set the Prescaler, the Filter value and the Polarity */
  tmpsmcr |= (uint32_t)(TIM_ExtTRGPrescaler | (TIM_ExtTRGPolarity | (ExtTRGFilter << 8U)));  /* 赋值或初始化 */

  /* Write to TIMx SMCR */
  TIMx->SMCR = tmpsmcr;                                 /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  Enables or disables the TIM Capture Compare Channel x.
  * @param  TIMx to select the TIM peripheral
  * @param  Channel specifies the TIM Channel
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1
  *            @arg TIM_CHANNEL_2: TIM Channel 2
  *            @arg TIM_CHANNEL_3: TIM Channel 3
  *            @arg TIM_CHANNEL_4: TIM Channel 4
  * @param  ChannelState specifies the TIM Channel CCxE bit new state.
  *          This parameter can be: TIM_CCx_ENABLE or TIM_CCx_DISABLE.
  * @retval None
  */
void TIM_CCxChannelCmd(TIM_TypeDef *TIMx, uint32_t Channel, uint32_t ChannelState)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  uint32_t tmp;                                         /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_TIM_CC1_INSTANCE(TIMx));              /* 执行语句 */
  assert_param(IS_TIM_CHANNELS(Channel));               /* 执行语句 */

  tmp = TIM_CCER_CC1E << (Channel & 0x1FU); /* 0x1FU = 31 bits max shift */

  /* Reset the CCxE Bit */
  TIMx->CCER &= ~tmp;                                   /* 赋值或初始化 */

  /* Set or reset the CCxE Bit */
  TIMx->CCER |= (uint32_t)(ChannelState << (Channel & 0x1FU)); /* 0x1FU = 31 bits max shift */
}                                                       /* 代码块结束 */

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)               /* 条件编译 */
/**
  * @brief  Reset interrupt callbacks to the legacy weak callbacks.
  * @param  htim pointer to a TIM_HandleTypeDef structure that contains
  *                the configuration information for TIM module.
  * @retval None
  */
void TIM_ResetCallback(TIM_HandleTypeDef *htim)         /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Reset the TIM callback to the legacy weak callbacks */
  htim->PeriodElapsedCallback             = HAL_TIM_PeriodElapsedCallback;             /* Legacy weak PeriodElapsedCallback             */
  htim->PeriodElapsedHalfCpltCallback     = HAL_TIM_PeriodElapsedHalfCpltCallback;     /* Legacy weak PeriodElapsedHalfCpltCallback     */
  htim->TriggerCallback                   = HAL_TIM_TriggerCallback;                   /* Legacy weak TriggerCallback                   */
  htim->TriggerHalfCpltCallback           = HAL_TIM_TriggerHalfCpltCallback;           /* Legacy weak TriggerHalfCpltCallback           */
  htim->IC_CaptureCallback                = HAL_TIM_IC_CaptureCallback;                /* Legacy weak IC_CaptureCallback                */
  htim->IC_CaptureHalfCpltCallback        = HAL_TIM_IC_CaptureHalfCpltCallback;        /* Legacy weak IC_CaptureHalfCpltCallback        */
  htim->OC_DelayElapsedCallback           = HAL_TIM_OC_DelayElapsedCallback;           /* Legacy weak OC_DelayElapsedCallback           */
  htim->PWM_PulseFinishedCallback         = HAL_TIM_PWM_PulseFinishedCallback;         /* Legacy weak PWM_PulseFinishedCallback         */
  htim->PWM_PulseFinishedHalfCpltCallback = HAL_TIM_PWM_PulseFinishedHalfCpltCallback; /* Legacy weak PWM_PulseFinishedHalfCpltCallback */
  htim->ErrorCallback                     = HAL_TIM_ErrorCallback;                     /* Legacy weak ErrorCallback                     */
  htim->CommutationCallback               = HAL_TIMEx_CommutCallback;                  /* Legacy weak CommutationCallback               */
  htim->CommutationHalfCpltCallback       = HAL_TIMEx_CommutHalfCpltCallback;          /* Legacy weak CommutationHalfCpltCallback       */
  htim->BreakCallback                     = HAL_TIMEx_BreakCallback;                   /* Legacy weak BreakCallback                     */
}                                                       /* 代码块结束 */
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */

/**
  * @}
  */

#endif /* HAL_TIM_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
