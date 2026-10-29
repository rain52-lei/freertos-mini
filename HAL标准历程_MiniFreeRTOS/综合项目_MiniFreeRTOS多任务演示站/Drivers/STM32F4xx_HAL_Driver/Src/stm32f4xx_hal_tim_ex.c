/* 文件: stm32f4xx_hal_tim_ex.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_tim_ex.c
  * @author  MCD Application Team
  * @brief   TIM HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Timer Extended peripheral:
  *           + Time Hall Sensor Interface Initialization
  *           + Time Hall Sensor Interface Start
  *           + Time Complementary signal break and dead time configuration
  *           + Time Master and Slave synchronization configuration
  *           + Timer remapping capabilities configuration
  @verbatim
  ==============================================================================
                      ##### TIMER Extended features #####
  ==============================================================================
  [..]
    The Timer Extended features include:
    (#) Complementary outputs with programmable dead-time for :
        (++) Output Compare
        (++) PWM generation (Edge and Center-aligned Mode)
        (++) One-pulse mode output
    (#) Synchronization circuit to control the timer with external signals and to
        interconnect several timers together.
    (#) Break input to put the timer output signals in reset state or in a known state.
    (#) Supports incremental (quadrature) encoder and hall-sensor circuitry for
        positioning purposes

            ##### How to use this driver #####
  ==============================================================================
    [..]
     (#) Initialize the TIM low level resources by implementing the following functions
         depending on the selected feature:
           (++) Hall Sensor output : HAL_TIMEx_HallSensor_MspInit()

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
         initialization function of this driver:
          (++) HAL_TIMEx_HallSensor_Init() and HAL_TIMEx_ConfigCommutEvent(): to use the
               Timer Hall Sensor Interface and the commutation event with the corresponding
               Interrupt and DMA request if needed (Note that One Timer is used to interface
               with the Hall sensor Interface and another Timer should be used to use
               the commutation event).

     (#) Activate the TIM peripheral using one of the start functions:
           (++) Complementary Output Compare : HAL_TIMEx_OCN_Start(), HAL_TIMEx_OCN_Start_DMA(), HAL_TIMEx_OCN_Start_IT()
           (++) Complementary PWM generation : HAL_TIMEx_PWMN_Start(), HAL_TIMEx_PWMN_Start_DMA(), HAL_TIMEx_PWMN_Start_IT()
           (++) Complementary One-pulse mode output : HAL_TIMEx_OnePulseN_Start(), HAL_TIMEx_OnePulseN_Start_IT()
           (++) Hall Sensor output : HAL_TIMEx_HallSensor_Start(), HAL_TIMEx_HallSensor_Start_DMA(), HAL_TIMEx_HallSensor_Start_IT().

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

/** @defgroup TIMEx TIMEx
  * @brief TIM Extended HAL module driver
  * @{
  */

#ifdef HAL_TIM_MODULE_ENABLED                           /* 条件编译 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void TIM_DMADelayPulseNCplt(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void TIM_DMAErrorCCxN(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void TIM_CCxNChannelCmd(TIM_TypeDef *TIMx, uint32_t Channel, uint32_t ChannelNState);  /* 静态函数 */

/* Exported functions --------------------------------------------------------*/
/** @defgroup TIMEx_Exported_Functions TIM Extended Exported Functions
  * @{
  */

/** @defgroup TIMEx_Exported_Functions_Group1 Extended Timer Hall Sensor functions
  * @brief    Timer Hall Sensor functions
  *
@verbatim
  ==============================================================================
                      ##### Timer Hall Sensor functions #####
  ==============================================================================
  [..]
    This section provides functions allowing to:
    (+) Initialize and configure TIM HAL Sensor.
    (+) De-initialize TIM HAL Sensor.
    (+) Start the Hall Sensor Interface.
    (+) Stop the Hall Sensor Interface.
    (+) Start the Hall Sensor Interface and enable interrupts.
    (+) Stop the Hall Sensor Interface and disable interrupts.
    (+) Start the Hall Sensor Interface and enable DMA transfers.
    (+) Stop the Hall Sensor Interface and disable DMA transfers.

@endverbatim
  * @{
  */
/**
  * @brief  Initializes the TIM Hall Sensor Interface and initialize the associated handle.
  * @note   When the timer instance is initialized in Hall Sensor Interface mode,
  *         timer channels 1 and channel 2 are reserved and cannot be used for
  *         other purpose.
  * @param  htim TIM Hall Sensor Interface handle
  * @param  sConfig TIM Hall Sensor configuration structure
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_HallSensor_Init(TIM_HandleTypeDef *htim, TIM_HallSensor_InitTypeDef *sConfig)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  TIM_OC_InitTypeDef OC_Config;                         /* 执行语句 */

  /* Check the TIM handle allocation */
  if (htim == NULL)                                     /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the parameters */
  assert_param(IS_TIM_HALL_SENSOR_INTERFACE_INSTANCE(htim->Instance));  /* 执行语句 */
  assert_param(IS_TIM_COUNTER_MODE(htim->Init.CounterMode));  /* 执行语句 */
  assert_param(IS_TIM_CLOCKDIVISION_DIV(htim->Init.ClockDivision));  /* 执行语句 */
  assert_param(IS_TIM_AUTORELOAD_PRELOAD(htim->Init.AutoReloadPreload));  /* 执行语句 */
  assert_param(IS_TIM_IC_POLARITY(sConfig->IC1Polarity));  /* 执行语句 */
  assert_param(IS_TIM_IC_PRESCALER(sConfig->IC1Prescaler));  /* 执行语句 */
  assert_param(IS_TIM_IC_FILTER(sConfig->IC1Filter));   /* 执行语句 */

  if (htim->State == HAL_TIM_STATE_RESET)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Allocate lock resource and initialize it */
    htim->Lock = HAL_UNLOCKED;                          /* 赋值或初始化 */

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)               /* 条件编译 */
    /* Reset interrupt callbacks to legacy week callbacks */
    TIM_ResetCallback(htim);                            /* 执行语句 */

    if (htim->HallSensor_MspInitCallback == NULL)       /* 条件判断 */
    {                                                   /* 代码块开始 */
      htim->HallSensor_MspInitCallback = HAL_TIMEx_HallSensor_MspInit;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    /* Init the low level hardware : GPIO, CLOCK, NVIC */
    htim->HallSensor_MspInitCallback(htim);             /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /* Init the low level hardware : GPIO, CLOCK, NVIC and DMA */
    HAL_TIMEx_HallSensor_MspInit(htim);                 /* HAL库函数调用 */
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */

  /* Set the TIM state */
  htim->State = HAL_TIM_STATE_BUSY;                     /* 赋值或初始化 */

  /* Configure the Time base in the Encoder Mode */
  TIM_Base_SetConfig(htim->Instance, &htim->Init);      /* 执行语句 */

  /* Configure the Channel 1 as Input Channel to interface with the three Outputs of the  Hall sensor */
  TIM_TI1_SetConfig(htim->Instance, sConfig->IC1Polarity, TIM_ICSELECTION_TRC, sConfig->IC1Filter);  /* 执行语句 */

  /* Reset the IC1PSC Bits */
  htim->Instance->CCMR1 &= ~TIM_CCMR1_IC1PSC;           /* 赋值或初始化 */
  /* Set the IC1PSC value */
  htim->Instance->CCMR1 |= sConfig->IC1Prescaler;       /* 赋值或初始化 */

  /* Enable the Hall sensor interface (XOR function of the three inputs) */
  htim->Instance->CR2 |= TIM_CR2_TI1S;                  /* 赋值或初始化 */

  /* Select the TIM_TS_TI1F_ED signal as Input trigger for the TIM */
  htim->Instance->SMCR &= ~TIM_SMCR_TS;                 /* 赋值或初始化 */
  htim->Instance->SMCR |= TIM_TS_TI1F_ED;               /* 赋值或初始化 */

  /* Use the TIM_TS_TI1F_ED signal to reset the TIM counter each edge detection */
  htim->Instance->SMCR &= ~TIM_SMCR_SMS;                /* 赋值或初始化 */
  htim->Instance->SMCR |= TIM_SLAVEMODE_RESET;          /* 赋值或初始化 */

  /* Program channel 2 in PWM 2 mode with the desired Commutation_Delay*/
  OC_Config.OCFastMode = TIM_OCFAST_DISABLE;            /* 赋值或初始化 */
  OC_Config.OCIdleState = TIM_OCIDLESTATE_RESET;        /* 赋值或初始化 */
  OC_Config.OCMode = TIM_OCMODE_PWM2;                   /* 赋值或初始化 */
  OC_Config.OCNIdleState = TIM_OCNIDLESTATE_RESET;      /* 赋值或初始化 */
  OC_Config.OCNPolarity = TIM_OCNPOLARITY_HIGH;         /* 赋值或初始化 */
  OC_Config.OCPolarity = TIM_OCPOLARITY_HIGH;           /* 赋值或初始化 */
  OC_Config.Pulse = sConfig->Commutation_Delay;         /* 赋值或初始化 */

  TIM_OC2_SetConfig(htim->Instance, &OC_Config);        /* 执行语句 */

  /* Select OC2REF as trigger output on TRGO: write the MMS bits in the TIMx_CR2
    register to 101 */
  htim->Instance->CR2 &= ~TIM_CR2_MMS;                  /* 赋值或初始化 */
  htim->Instance->CR2 |= TIM_TRGO_OC2REF;               /* 赋值或初始化 */

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
  * @brief  DeInitializes the TIM Hall Sensor interface
  * @param  htim TIM Hall Sensor Interface handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_HallSensor_DeInit(TIM_HandleTypeDef *htim)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_INSTANCE(htim->Instance));        /* 执行语句 */

  htim->State = HAL_TIM_STATE_BUSY;                     /* 赋值或初始化 */

  /* Disable the TIM Peripheral Clock */
  __HAL_TIM_DISABLE(htim);                              /* HAL辅助宏/函数 */

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)               /* 条件编译 */
  if (htim->HallSensor_MspDeInitCallback == NULL)       /* 条件判断 */
  {                                                     /* 代码块开始 */
    htim->HallSensor_MspDeInitCallback = HAL_TIMEx_HallSensor_MspDeInit;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  /* DeInit the low level hardware */
  htim->HallSensor_MspDeInitCallback(htim);             /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* DeInit the low level hardware: GPIO, CLOCK, NVIC */
  HAL_TIMEx_HallSensor_MspDeInit(htim);                 /* HAL库函数调用 */
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */

  /* Change the DMA burst operation state */
  htim->DMABurstState = HAL_DMA_BURST_STATE_RESET;      /* 赋值或初始化 */

  /* Change the TIM channels state */
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
  * @brief  Initializes the TIM Hall Sensor MSP.
  * @param  htim TIM Hall Sensor Interface handle
  * @retval None
  */
__weak void HAL_TIMEx_HallSensor_MspInit(TIM_HandleTypeDef *htim)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIMEx_HallSensor_MspInit could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  DeInitializes TIM Hall Sensor MSP.
  * @param  htim TIM Hall Sensor Interface handle
  * @retval None
  */
__weak void HAL_TIMEx_HallSensor_MspDeInit(TIM_HandleTypeDef *htim)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIMEx_HallSensor_MspDeInit could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Starts the TIM Hall Sensor Interface.
  * @param  htim TIM Hall Sensor Interface handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_HallSensor_Start(TIM_HandleTypeDef *htim)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpsmcr;                                     /* 32位无符号变量/函数 */
  HAL_TIM_ChannelStateTypeDef channel_1_state = TIM_CHANNEL_STATE_GET(htim, TIM_CHANNEL_1);  /* HAL 定时器操作 */
  HAL_TIM_ChannelStateTypeDef channel_2_state = TIM_CHANNEL_STATE_GET(htim, TIM_CHANNEL_2);  /* HAL 定时器操作 */
  HAL_TIM_ChannelStateTypeDef complementary_channel_1_state = TIM_CHANNEL_N_STATE_GET(htim, TIM_CHANNEL_1);  /* HAL 定时器操作 */
  HAL_TIM_ChannelStateTypeDef complementary_channel_2_state = TIM_CHANNEL_N_STATE_GET(htim, TIM_CHANNEL_2);  /* HAL 定时器操作 */

  /* Check the parameters */
  assert_param(IS_TIM_HALL_SENSOR_INTERFACE_INSTANCE(htim->Instance));  /* 执行语句 */

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

  /* Enable the Input Capture channel 1
  (in the Hall Sensor Interface the three possible channels that can be used are TIM_CHANNEL_1, TIM_CHANNEL_2 and TIM_CHANNEL_3) */
  TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);  /* 执行语句 */

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
  * @brief  Stops the TIM Hall sensor Interface.
  * @param  htim TIM Hall Sensor Interface handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_HallSensor_Stop(TIM_HandleTypeDef *htim)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_HALL_SENSOR_INTERFACE_INSTANCE(htim->Instance));  /* 执行语句 */

  /* Disable the Input Capture channels 1, 2 and 3
    (in the Hall Sensor Interface the three possible channels that can be used are TIM_CHANNEL_1, TIM_CHANNEL_2 and TIM_CHANNEL_3) */
  TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_DISABLE);  /* 执行语句 */

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
  * @brief  Starts the TIM Hall Sensor Interface in interrupt mode.
  * @param  htim TIM Hall Sensor Interface handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_HallSensor_Start_IT(TIM_HandleTypeDef *htim)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpsmcr;                                     /* 32位无符号变量/函数 */
  HAL_TIM_ChannelStateTypeDef channel_1_state = TIM_CHANNEL_STATE_GET(htim, TIM_CHANNEL_1);  /* HAL 定时器操作 */
  HAL_TIM_ChannelStateTypeDef channel_2_state = TIM_CHANNEL_STATE_GET(htim, TIM_CHANNEL_2);  /* HAL 定时器操作 */
  HAL_TIM_ChannelStateTypeDef complementary_channel_1_state = TIM_CHANNEL_N_STATE_GET(htim, TIM_CHANNEL_1);  /* HAL 定时器操作 */
  HAL_TIM_ChannelStateTypeDef complementary_channel_2_state = TIM_CHANNEL_N_STATE_GET(htim, TIM_CHANNEL_2);  /* HAL 定时器操作 */

  /* Check the parameters */
  assert_param(IS_TIM_HALL_SENSOR_INTERFACE_INSTANCE(htim->Instance));  /* 执行语句 */

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

  /* Enable the capture compare Interrupts 1 event */
  __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC1);                /* HAL辅助宏/函数 */

  /* Enable the Input Capture channel 1
    (in the Hall Sensor Interface the three possible channels that can be used are TIM_CHANNEL_1, TIM_CHANNEL_2 and TIM_CHANNEL_3) */
  TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);  /* 执行语句 */

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
  * @brief  Stops the TIM Hall Sensor Interface in interrupt mode.
  * @param  htim TIM Hall Sensor Interface handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_HallSensor_Stop_IT(TIM_HandleTypeDef *htim)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_HALL_SENSOR_INTERFACE_INSTANCE(htim->Instance));  /* 执行语句 */

  /* Disable the Input Capture channel 1
    (in the Hall Sensor Interface the three possible channels that can be used are TIM_CHANNEL_1, TIM_CHANNEL_2 and TIM_CHANNEL_3) */
  TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_DISABLE);  /* 执行语句 */

  /* Disable the capture compare Interrupts event */
  __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC1);               /* HAL辅助宏/函数 */

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
  * @brief  Starts the TIM Hall Sensor Interface in DMA mode.
  * @param  htim TIM Hall Sensor Interface handle
  * @param  pData The destination Buffer address.
  * @param  Length The length of data to be transferred from TIM peripheral to memory.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_HallSensor_Start_DMA(TIM_HandleTypeDef *htim, uint32_t *pData, uint16_t Length)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpsmcr;                                     /* 32位无符号变量/函数 */
  HAL_TIM_ChannelStateTypeDef channel_1_state = TIM_CHANNEL_STATE_GET(htim, TIM_CHANNEL_1);  /* HAL 定时器操作 */
  HAL_TIM_ChannelStateTypeDef complementary_channel_1_state = TIM_CHANNEL_N_STATE_GET(htim, TIM_CHANNEL_1);  /* HAL 定时器操作 */

  /* Check the parameters */
  assert_param(IS_TIM_HALL_SENSOR_INTERFACE_INSTANCE(htim->Instance));  /* 执行语句 */

  /* Set the TIM channel state */
  if ((channel_1_state == HAL_TIM_CHANNEL_STATE_BUSY)   /* 条件判断 */
      || (complementary_channel_1_state == HAL_TIM_CHANNEL_STATE_BUSY))  /* 源代码 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
  else if ((channel_1_state == HAL_TIM_CHANNEL_STATE_READY)  /* 否则如果 */
           && (complementary_channel_1_state == HAL_TIM_CHANNEL_STATE_READY))  /* 源代码 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) && (Length > 0U))               /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */
      TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Enable the Input Capture channel 1
    (in the Hall Sensor Interface the three possible channels that can be used are TIM_CHANNEL_1, TIM_CHANNEL_2 and TIM_CHANNEL_3) */
  TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);  /* 执行语句 */

  /* Set the DMA Input Capture 1 Callbacks */
  htim->hdma[TIM_DMA_ID_CC1]->XferCpltCallback = TIM_DMACaptureCplt;  /* 赋值或初始化 */
  htim->hdma[TIM_DMA_ID_CC1]->XferHalfCpltCallback = TIM_DMACaptureHalfCplt;  /* 赋值或初始化 */
  /* Set the DMA error callback */
  htim->hdma[TIM_DMA_ID_CC1]->XferErrorCallback = TIM_DMAError ;  /* 赋值或初始化 */

  /* Enable the DMA stream for Capture 1*/
  if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC1], (uint32_t)&htim->Instance->CCR1, (uint32_t)pData, Length) != HAL_OK)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Return error status */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  /* Enable the capture compare 1 Interrupt */
  __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC1);              /* HAL辅助宏/函数 */

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
  * @brief  Stops the TIM Hall Sensor Interface in DMA mode.
  * @param  htim TIM Hall Sensor Interface handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_HallSensor_Stop_DMA(TIM_HandleTypeDef *htim)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_HALL_SENSOR_INTERFACE_INSTANCE(htim->Instance));  /* 执行语句 */

  /* Disable the Input Capture channel 1
    (in the Hall Sensor Interface the three possible channels that can be used are TIM_CHANNEL_1, TIM_CHANNEL_2 and TIM_CHANNEL_3) */
  TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_DISABLE);  /* 执行语句 */


  /* Disable the capture compare Interrupts 1 event */
  __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC1);             /* HAL辅助宏/函数 */

  (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC1]);   /* 执行语句 */

  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);                              /* HAL辅助宏/函数 */

  /* Set the TIM channel state */
  TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
  TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup TIMEx_Exported_Functions_Group2 Extended Timer Complementary Output Compare functions
  *  @brief   Timer Complementary Output Compare functions
  *
@verbatim
  ==============================================================================
              ##### Timer Complementary Output Compare functions #####
  ==============================================================================
  [..]
    This section provides functions allowing to:
    (+) Start the Complementary Output Compare/PWM.
    (+) Stop the Complementary Output Compare/PWM.
    (+) Start the Complementary Output Compare/PWM and enable interrupts.
    (+) Stop the Complementary Output Compare/PWM and disable interrupts.
    (+) Start the Complementary Output Compare/PWM and enable DMA transfers.
    (+) Stop the Complementary Output Compare/PWM and disable DMA transfers.

@endverbatim
  * @{
  */

/**
  * @brief  Starts the TIM Output Compare signal generation on the complementary
  *         output.
  * @param  htim TIM Output Compare handle
  * @param  Channel TIM Channel to be enabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_OCN_Start(TIM_HandleTypeDef *htim, uint32_t Channel)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpsmcr;                                     /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, Channel));  /* 执行语句 */

  /* Check the TIM complementary channel state */
  if (TIM_CHANNEL_N_STATE_GET(htim, Channel) != HAL_TIM_CHANNEL_STATE_READY)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Set the TIM complementary channel state */
  TIM_CHANNEL_N_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */

  /* Enable the Capture compare channel N */
  TIM_CCxNChannelCmd(htim->Instance, Channel, TIM_CCxN_ENABLE);  /* 执行语句 */

  /* Enable the Main Output */
  __HAL_TIM_MOE_ENABLE(htim);                           /* HAL辅助宏/函数 */

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
  * @brief  Stops the TIM Output Compare signal generation on the complementary
  *         output.
  * @param  htim TIM handle
  * @param  Channel TIM Channel to be disabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_OCN_Stop(TIM_HandleTypeDef *htim, uint32_t Channel)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, Channel));  /* 执行语句 */

  /* Disable the Capture compare channel N */
  TIM_CCxNChannelCmd(htim->Instance, Channel, TIM_CCxN_DISABLE);  /* 执行语句 */

  /* Disable the Main Output */
  __HAL_TIM_MOE_DISABLE(htim);                          /* HAL辅助宏/函数 */

  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);                              /* HAL辅助宏/函数 */

  /* Set the TIM complementary channel state */
  TIM_CHANNEL_N_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Starts the TIM Output Compare signal generation in interrupt mode
  *         on the complementary output.
  * @param  htim TIM OC handle
  * @param  Channel TIM Channel to be enabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_OCN_Start_IT(TIM_HandleTypeDef *htim, uint32_t Channel)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpsmcr;                                     /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, Channel));  /* 执行语句 */

  /* Check the TIM complementary channel state */
  if (TIM_CHANNEL_N_STATE_GET(htim, Channel) != HAL_TIM_CHANNEL_STATE_READY)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Set the TIM complementary channel state */
  TIM_CHANNEL_N_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */

  switch (Channel)                                      /* switch分支 */
  {                                                     /* 代码块开始 */
    case TIM_CHANNEL_1:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Enable the TIM Output Compare interrupt */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC1);            /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_2:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Enable the TIM Output Compare interrupt */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC2);            /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_3:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Enable the TIM Output Compare interrupt */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC3);            /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */


    default:                                            /* default分支 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  /* Enable the TIM Break interrupt */
  __HAL_TIM_ENABLE_IT(htim, TIM_IT_BREAK);              /* HAL辅助宏/函数 */

  /* Enable the Capture compare channel N */
  TIM_CCxNChannelCmd(htim->Instance, Channel, TIM_CCxN_ENABLE);  /* 执行语句 */

  /* Enable the Main Output */
  __HAL_TIM_MOE_ENABLE(htim);                           /* HAL辅助宏/函数 */

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
  * @brief  Stops the TIM Output Compare signal generation in interrupt mode
  *         on the complementary output.
  * @param  htim TIM Output Compare handle
  * @param  Channel TIM Channel to be disabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_OCN_Stop_IT(TIM_HandleTypeDef *htim, uint32_t Channel)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpccer;                                     /* 32位无符号变量/函数 */
  /* Check the parameters */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, Channel));  /* 执行语句 */

  switch (Channel)                                      /* switch分支 */
  {                                                     /* 代码块开始 */
    case TIM_CHANNEL_1:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Disable the TIM Output Compare interrupt */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC1);           /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_2:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Disable the TIM Output Compare interrupt */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC2);           /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_3:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Disable the TIM Output Compare interrupt */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC3);           /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    default:                                            /* default分支 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  /* Disable the Capture compare channel N */
  TIM_CCxNChannelCmd(htim->Instance, Channel, TIM_CCxN_DISABLE);  /* 执行语句 */

  /* Disable the TIM Break interrupt (only if no more channel is active) */
  tmpccer = htim->Instance->CCER;                       /* 赋值或初始化 */
  if ((tmpccer & (TIM_CCER_CC1NE | TIM_CCER_CC2NE | TIM_CCER_CC3NE)) == (uint32_t)RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    __HAL_TIM_DISABLE_IT(htim, TIM_IT_BREAK);           /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Disable the Main Output */
  __HAL_TIM_MOE_DISABLE(htim);                          /* HAL辅助宏/函数 */

  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);                              /* HAL辅助宏/函数 */

  /* Set the TIM complementary channel state */
  TIM_CHANNEL_N_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Starts the TIM Output Compare signal generation in DMA mode
  *         on the complementary output.
  * @param  htim TIM Output Compare handle
  * @param  Channel TIM Channel to be enabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  * @param  pData The source Buffer address.
  * @param  Length The length of data to be transferred from memory to TIM peripheral
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_OCN_Start_DMA(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t *pData, uint16_t Length)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpsmcr;                                     /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, Channel));  /* 执行语句 */

  /* Set the TIM complementary channel state */
  if (TIM_CHANNEL_N_STATE_GET(htim, Channel) == HAL_TIM_CHANNEL_STATE_BUSY)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
  else if (TIM_CHANNEL_N_STATE_GET(htim, Channel) == HAL_TIM_CHANNEL_STATE_READY)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) && (Length > 0U))               /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
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
      /* Set the DMA compare callbacks */
      htim->hdma[TIM_DMA_ID_CC1]->XferCpltCallback = TIM_DMADelayPulseNCplt;  /* 赋值或初始化 */
      htim->hdma[TIM_DMA_ID_CC1]->XferHalfCpltCallback = TIM_DMADelayPulseHalfCplt;  /* 赋值或初始化 */

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC1]->XferErrorCallback = TIM_DMAErrorCCxN ;  /* 赋值或初始化 */

      /* Enable the DMA stream */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC1], (uint32_t)pData, (uint32_t)&htim->Instance->CCR1, Length) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Return error status */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
      /* Enable the TIM Output Compare DMA request */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC1);          /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_2:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Set the DMA compare callbacks */
      htim->hdma[TIM_DMA_ID_CC2]->XferCpltCallback = TIM_DMADelayPulseNCplt;  /* 赋值或初始化 */
      htim->hdma[TIM_DMA_ID_CC2]->XferHalfCpltCallback = TIM_DMADelayPulseHalfCplt;  /* 赋值或初始化 */

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC2]->XferErrorCallback = TIM_DMAErrorCCxN ;  /* 赋值或初始化 */

      /* Enable the DMA stream */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC2], (uint32_t)pData, (uint32_t)&htim->Instance->CCR2, Length) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Return error status */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
      /* Enable the TIM Output Compare DMA request */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC2);          /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_3:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Set the DMA compare callbacks */
      htim->hdma[TIM_DMA_ID_CC3]->XferCpltCallback = TIM_DMADelayPulseNCplt;  /* 赋值或初始化 */
      htim->hdma[TIM_DMA_ID_CC3]->XferHalfCpltCallback = TIM_DMADelayPulseHalfCplt;  /* 赋值或初始化 */

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC3]->XferErrorCallback = TIM_DMAErrorCCxN ;  /* 赋值或初始化 */

      /* Enable the DMA stream */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC3], (uint32_t)pData, (uint32_t)&htim->Instance->CCR3, Length) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Return error status */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
      /* Enable the TIM Output Compare DMA request */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC3);          /* HAL辅助宏/函数 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    default:                                            /* default分支 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  /* Enable the Capture compare channel N */
  TIM_CCxNChannelCmd(htim->Instance, Channel, TIM_CCxN_ENABLE);  /* 执行语句 */

  /* Enable the Main Output */
  __HAL_TIM_MOE_ENABLE(htim);                           /* HAL辅助宏/函数 */

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
  * @brief  Stops the TIM Output Compare signal generation in DMA mode
  *         on the complementary output.
  * @param  htim TIM Output Compare handle
  * @param  Channel TIM Channel to be disabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_OCN_Stop_DMA(TIM_HandleTypeDef *htim, uint32_t Channel)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, Channel));  /* 执行语句 */

  switch (Channel)                                      /* switch分支 */
  {                                                     /* 代码块开始 */
    case TIM_CHANNEL_1:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Disable the TIM Output Compare DMA request */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC1);         /* HAL辅助宏/函数 */
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC1]);  /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_2:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Disable the TIM Output Compare DMA request */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC2);         /* HAL辅助宏/函数 */
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC2]);  /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    case TIM_CHANNEL_3:                                 /* case标签 */
    {                                                   /* 代码块开始 */
      /* Disable the TIM Output Compare DMA request */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC3);         /* HAL辅助宏/函数 */
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC3]);  /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    default:                                            /* default分支 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  /* Disable the Capture compare channel N */
  TIM_CCxNChannelCmd(htim->Instance, Channel, TIM_CCxN_DISABLE);  /* 执行语句 */

  /* Disable the Main Output */
  __HAL_TIM_MOE_DISABLE(htim);                          /* HAL辅助宏/函数 */

  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);                              /* HAL辅助宏/函数 */

  /* Set the TIM complementary channel state */
  TIM_CHANNEL_N_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup TIMEx_Exported_Functions_Group3 Extended Timer Complementary PWM functions
  * @brief    Timer Complementary PWM functions
  *
@verbatim
  ==============================================================================
                 ##### Timer Complementary PWM functions #####
  ==============================================================================
  [..]
    This section provides functions allowing to:
    (+) Start the Complementary PWM.
    (+) Stop the Complementary PWM.
    (+) Start the Complementary PWM and enable interrupts.
    (+) Stop the Complementary PWM and disable interrupts.
    (+) Start the Complementary PWM and enable DMA transfers.
    (+) Stop the Complementary PWM and disable DMA transfers.
    (+) Start the Complementary Input Capture measurement.
    (+) Stop the Complementary Input Capture.
    (+) Start the Complementary Input Capture and enable interrupts.
    (+) Stop the Complementary Input Capture and disable interrupts.
    (+) Start the Complementary Input Capture and enable DMA transfers.
    (+) Stop the Complementary Input Capture and disable DMA transfers.
    (+) Start the Complementary One Pulse generation.
    (+) Stop the Complementary One Pulse.
    (+) Start the Complementary One Pulse and enable interrupts.
    (+) Stop the Complementary One Pulse and disable interrupts.

@endverbatim
  * @{
  */

/**
  * @brief  Starts the PWM signal generation on the complementary output.
  * @param  htim TIM handle
  * @param  Channel TIM Channel to be enabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_PWMN_Start(TIM_HandleTypeDef *htim, uint32_t Channel)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpsmcr;                                     /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, Channel));  /* 执行语句 */

  /* Check the TIM complementary channel state */
  if (TIM_CHANNEL_N_STATE_GET(htim, Channel) != HAL_TIM_CHANNEL_STATE_READY)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Set the TIM complementary channel state */
  TIM_CHANNEL_N_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_BUSY);  /* 执行语句 */

  /* Enable the complementary PWM output  */
  TIM_CCxNChannelCmd(htim->Instance, Channel, TIM_CCxN_ENABLE);  /* 执行语句 */

  /* Enable the Main Output */
  __HAL_TIM_MOE_ENABLE(htim);                           /* HAL辅助宏/函数 */

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
  * @brief  Stops the PWM signal generation on the complementary output.
  * @param  htim TIM handle
  * @param  Channel TIM Channel to be disabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_PWMN_Stop(TIM_HandleTypeDef *htim, uint32_t Channel)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, Channel));  /* 执行语句 */

  /* Disable the complementary PWM output  */
  TIM_CCxNChannelCmd(htim->Instance, Channel, TIM_CCxN_DISABLE);  /* 执行语句 */

  /* Disable the Main Output */
  __HAL_TIM_MOE_DISABLE(htim);                          /* HAL辅助宏/函数 */

  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);                              /* HAL辅助宏/函数 */

  /* Set the TIM complementary channel state */
  TIM_CHANNEL_N_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Starts the PWM signal generation in interrupt mode on the
  *         complementary output.
  * @param  htim TIM handle
  * @param  Channel TIM Channel to be disabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_PWMN_Start_IT(TIM_HandleTypeDef *htim, uint32_t Channel)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpsmcr;                                     /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, Channel));  /* 执行语句 */

  /* Check the TIM complementary channel state */
  if (TIM_CHANNEL_N_STATE_GET(htim, Channel) != HAL_TIM_CHANNEL_STATE_READY)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Set the TIM complementary channel state */
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

    default:                                            /* default分支 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  /* Enable the TIM Break interrupt */
  __HAL_TIM_ENABLE_IT(htim, TIM_IT_BREAK);              /* HAL辅助宏/函数 */

  /* Enable the complementary PWM output  */
  TIM_CCxNChannelCmd(htim->Instance, Channel, TIM_CCxN_ENABLE);  /* 执行语句 */

  /* Enable the Main Output */
  __HAL_TIM_MOE_ENABLE(htim);                           /* HAL辅助宏/函数 */

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
  * @brief  Stops the PWM signal generation in interrupt mode on the
  *         complementary output.
  * @param  htim TIM handle
  * @param  Channel TIM Channel to be disabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_PWMN_Stop_IT(TIM_HandleTypeDef *htim, uint32_t Channel)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpccer;                                     /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, Channel));  /* 执行语句 */

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

    default:                                            /* default分支 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  /* Disable the complementary PWM output  */
  TIM_CCxNChannelCmd(htim->Instance, Channel, TIM_CCxN_DISABLE);  /* 执行语句 */

  /* Disable the TIM Break interrupt (only if no more channel is active) */
  tmpccer = htim->Instance->CCER;                       /* 赋值或初始化 */
  if ((tmpccer & (TIM_CCER_CC1NE | TIM_CCER_CC2NE | TIM_CCER_CC3NE)) == (uint32_t)RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    __HAL_TIM_DISABLE_IT(htim, TIM_IT_BREAK);           /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Disable the Main Output */
  __HAL_TIM_MOE_DISABLE(htim);                          /* HAL辅助宏/函数 */

  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);                              /* HAL辅助宏/函数 */

  /* Set the TIM complementary channel state */
  TIM_CHANNEL_N_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Starts the TIM PWM signal generation in DMA mode on the
  *         complementary output
  * @param  htim TIM handle
  * @param  Channel TIM Channel to be enabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  * @param  pData The source Buffer address.
  * @param  Length The length of data to be transferred from memory to TIM peripheral
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_PWMN_Start_DMA(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t *pData, uint16_t Length)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpsmcr;                                     /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, Channel));  /* 执行语句 */

  /* Set the TIM complementary channel state */
  if (TIM_CHANNEL_N_STATE_GET(htim, Channel) == HAL_TIM_CHANNEL_STATE_BUSY)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
  else if (TIM_CHANNEL_N_STATE_GET(htim, Channel) == HAL_TIM_CHANNEL_STATE_READY)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) && (Length > 0U))               /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
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
      /* Set the DMA compare callbacks */
      htim->hdma[TIM_DMA_ID_CC1]->XferCpltCallback = TIM_DMADelayPulseNCplt;  /* 赋值或初始化 */
      htim->hdma[TIM_DMA_ID_CC1]->XferHalfCpltCallback = TIM_DMADelayPulseHalfCplt;  /* 赋值或初始化 */

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC1]->XferErrorCallback = TIM_DMAErrorCCxN ;  /* 赋值或初始化 */

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
      htim->hdma[TIM_DMA_ID_CC2]->XferCpltCallback = TIM_DMADelayPulseNCplt;  /* 赋值或初始化 */
      htim->hdma[TIM_DMA_ID_CC2]->XferHalfCpltCallback = TIM_DMADelayPulseHalfCplt;  /* 赋值或初始化 */

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC2]->XferErrorCallback = TIM_DMAErrorCCxN ;  /* 赋值或初始化 */

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
      htim->hdma[TIM_DMA_ID_CC3]->XferCpltCallback = TIM_DMADelayPulseNCplt;  /* 赋值或初始化 */
      htim->hdma[TIM_DMA_ID_CC3]->XferHalfCpltCallback = TIM_DMADelayPulseHalfCplt;  /* 赋值或初始化 */

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC3]->XferErrorCallback = TIM_DMAErrorCCxN ;  /* 赋值或初始化 */

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

    default:                                            /* default分支 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  /* Enable the complementary PWM output  */
  TIM_CCxNChannelCmd(htim->Instance, Channel, TIM_CCxN_ENABLE);  /* 执行语句 */

  /* Enable the Main Output */
  __HAL_TIM_MOE_ENABLE(htim);                           /* HAL辅助宏/函数 */

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
  * @brief  Stops the TIM PWM signal generation in DMA mode on the complementary
  *         output
  * @param  htim TIM handle
  * @param  Channel TIM Channel to be disabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_PWMN_Stop_DMA(TIM_HandleTypeDef *htim, uint32_t Channel)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, Channel));  /* 执行语句 */

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

    default:                                            /* default分支 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  /* Disable the complementary PWM output */
  TIM_CCxNChannelCmd(htim->Instance, Channel, TIM_CCxN_DISABLE);  /* 执行语句 */

  /* Disable the Main Output */
  __HAL_TIM_MOE_DISABLE(htim);                          /* HAL辅助宏/函数 */

  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);                              /* HAL辅助宏/函数 */

  /* Set the TIM complementary channel state */
  TIM_CHANNEL_N_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup TIMEx_Exported_Functions_Group4 Extended Timer Complementary One Pulse functions
  * @brief    Timer Complementary One Pulse functions
  *
@verbatim
  ==============================================================================
                ##### Timer Complementary One Pulse functions #####
  ==============================================================================
  [..]
    This section provides functions allowing to:
    (+) Start the Complementary One Pulse generation.
    (+) Stop the Complementary One Pulse.
    (+) Start the Complementary One Pulse and enable interrupts.
    (+) Stop the Complementary One Pulse and disable interrupts.

@endverbatim
  * @{
  */

/**
  * @brief  Starts the TIM One Pulse signal generation on the complementary
  *         output.
  * @note OutputChannel must match the pulse output channel chosen when calling 
  *       @ref HAL_TIM_OnePulse_ConfigChannel(). 
  * @param  htim TIM One Pulse handle
  * @param  OutputChannel pulse output channel to enable
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_OnePulseN_Start(TIM_HandleTypeDef *htim, uint32_t OutputChannel)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t input_channel = (OutputChannel == TIM_CHANNEL_1) ? TIM_CHANNEL_2 : TIM_CHANNEL_1;  /* 32位无符号变量/函数 */
  HAL_TIM_ChannelStateTypeDef channel_1_state = TIM_CHANNEL_STATE_GET(htim, TIM_CHANNEL_1);  /* HAL 定时器操作 */
  HAL_TIM_ChannelStateTypeDef channel_2_state = TIM_CHANNEL_STATE_GET(htim, TIM_CHANNEL_2);  /* HAL 定时器操作 */
  HAL_TIM_ChannelStateTypeDef complementary_channel_1_state = TIM_CHANNEL_N_STATE_GET(htim, TIM_CHANNEL_1);  /* HAL 定时器操作 */
  HAL_TIM_ChannelStateTypeDef complementary_channel_2_state = TIM_CHANNEL_N_STATE_GET(htim, TIM_CHANNEL_2);  /* HAL 定时器操作 */

  /* Check the parameters */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, OutputChannel));  /* 执行语句 */

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

  /* Enable the complementary One Pulse output channel and the Input Capture channel */
  TIM_CCxNChannelCmd(htim->Instance, OutputChannel, TIM_CCxN_ENABLE);  /* 执行语句 */
  TIM_CCxChannelCmd(htim->Instance, input_channel, TIM_CCx_ENABLE);  /* 执行语句 */

  /* Enable the Main Output */
  __HAL_TIM_MOE_ENABLE(htim);                           /* HAL辅助宏/函数 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Stops the TIM One Pulse signal generation on the complementary
  *         output.
  * @note OutputChannel must match the pulse output channel chosen when calling 
  *       @ref HAL_TIM_OnePulse_ConfigChannel(). 
  * @param  htim TIM One Pulse handle
  * @param  OutputChannel pulse output channel to disable
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_OnePulseN_Stop(TIM_HandleTypeDef *htim, uint32_t OutputChannel)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t input_channel = (OutputChannel == TIM_CHANNEL_1) ? TIM_CHANNEL_2 : TIM_CHANNEL_1;  /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, OutputChannel));  /* 执行语句 */

  /* Disable the complementary One Pulse output channel and the Input Capture channel */
  TIM_CCxNChannelCmd(htim->Instance, OutputChannel, TIM_CCxN_DISABLE);  /* 执行语句 */
  TIM_CCxChannelCmd(htim->Instance, input_channel, TIM_CCx_DISABLE);  /* 执行语句 */

  /* Disable the Main Output */
  __HAL_TIM_MOE_DISABLE(htim);                          /* HAL辅助宏/函数 */

  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);                              /* HAL辅助宏/函数 */

  /* Set the TIM  channels state */
  TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
  TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
  TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
  TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Starts the TIM One Pulse signal generation in interrupt mode on the
  *         complementary channel.
  * @note OutputChannel must match the pulse output channel chosen when calling 
  *       @ref HAL_TIM_OnePulse_ConfigChannel(). 
  * @param  htim TIM One Pulse handle
  * @param  OutputChannel pulse output channel to enable
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_OnePulseN_Start_IT(TIM_HandleTypeDef *htim, uint32_t OutputChannel)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t input_channel = (OutputChannel == TIM_CHANNEL_1) ? TIM_CHANNEL_2 : TIM_CHANNEL_1;  /* 32位无符号变量/函数 */
  HAL_TIM_ChannelStateTypeDef channel_1_state = TIM_CHANNEL_STATE_GET(htim, TIM_CHANNEL_1);  /* HAL 定时器操作 */
  HAL_TIM_ChannelStateTypeDef channel_2_state = TIM_CHANNEL_STATE_GET(htim, TIM_CHANNEL_2);  /* HAL 定时器操作 */
  HAL_TIM_ChannelStateTypeDef complementary_channel_1_state = TIM_CHANNEL_N_STATE_GET(htim, TIM_CHANNEL_1);  /* HAL 定时器操作 */
  HAL_TIM_ChannelStateTypeDef complementary_channel_2_state = TIM_CHANNEL_N_STATE_GET(htim, TIM_CHANNEL_2);  /* HAL 定时器操作 */

  /* Check the parameters */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, OutputChannel));  /* 执行语句 */

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

  /* Enable the TIM Capture/Compare 1 interrupt */
  __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC1);                /* HAL辅助宏/函数 */

  /* Enable the TIM Capture/Compare 2 interrupt */
  __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC2);                /* HAL辅助宏/函数 */

  /* Enable the complementary One Pulse output channel and the Input Capture channel */
  TIM_CCxNChannelCmd(htim->Instance, OutputChannel, TIM_CCxN_ENABLE);  /* 执行语句 */
  TIM_CCxChannelCmd(htim->Instance, input_channel, TIM_CCx_ENABLE);  /* 执行语句 */

  /* Enable the Main Output */
  __HAL_TIM_MOE_ENABLE(htim);                           /* HAL辅助宏/函数 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Stops the TIM One Pulse signal generation in interrupt mode on the
  *         complementary channel.
  * @note OutputChannel must match the pulse output channel chosen when calling 
  *       @ref HAL_TIM_OnePulse_ConfigChannel(). 
  * @param  htim TIM One Pulse handle
  * @param  OutputChannel pulse output channel to disable
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_OnePulseN_Stop_IT(TIM_HandleTypeDef *htim, uint32_t OutputChannel)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t input_channel = (OutputChannel == TIM_CHANNEL_1) ? TIM_CHANNEL_2 : TIM_CHANNEL_1;  /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, OutputChannel));  /* 执行语句 */

  /* Disable the TIM Capture/Compare 1 interrupt */
  __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC1);               /* HAL辅助宏/函数 */

  /* Disable the TIM Capture/Compare 2 interrupt */
  __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC2);               /* HAL辅助宏/函数 */

  /* Disable the complementary One Pulse output channel and the Input Capture channel */
  TIM_CCxNChannelCmd(htim->Instance, OutputChannel, TIM_CCxN_DISABLE);  /* 执行语句 */
  TIM_CCxChannelCmd(htim->Instance, input_channel, TIM_CCx_DISABLE);  /* 执行语句 */

  /* Disable the Main Output */
  __HAL_TIM_MOE_DISABLE(htim);                          /* HAL辅助宏/函数 */

  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);                              /* HAL辅助宏/函数 */

  /* Set the TIM  channels state */
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

/** @defgroup TIMEx_Exported_Functions_Group5 Extended Peripheral Control functions
  * @brief    Peripheral Control functions
  *
@verbatim
  ==============================================================================
                    ##### Peripheral Control functions #####
  ==============================================================================
  [..]
    This section provides functions allowing to:
      (+) Configure the commutation event in case of use of the Hall sensor interface.
      (+) Configure Output channels for OC and PWM mode.

      (+) Configure Complementary channels, break features and dead time.
      (+) Configure Master synchronization.
      (+) Configure timer remapping capabilities.

@endverbatim
  * @{
  */

/**
  * @brief  Configure the TIM commutation event sequence.
  * @note  This function is mandatory to use the commutation event in order to
  *        update the configuration at each commutation detection on the TRGI input of the Timer,
  *        the typical use of this feature is with the use of another Timer(interface Timer)
  *        configured in Hall sensor interface, this interface Timer will generate the
  *        commutation at its TRGO output (connected to Timer used in this function) each time
  *        the TI1 of the Interface Timer detect a commutation at its input TI1.
  * @param  htim TIM handle
  * @param  InputTrigger the Internal trigger corresponding to the Timer Interfacing with the Hall sensor
  *          This parameter can be one of the following values:
  *            @arg TIM_TS_ITR0: Internal trigger 0 selected
  *            @arg TIM_TS_ITR1: Internal trigger 1 selected
  *            @arg TIM_TS_ITR2: Internal trigger 2 selected
  *            @arg TIM_TS_ITR3: Internal trigger 3 selected
  *            @arg TIM_TS_NONE: No trigger is needed
  * @param  CommutationSource the Commutation Event source
  *          This parameter can be one of the following values:
  *            @arg TIM_COMMUTATION_TRGI: Commutation source is the TRGI of the Interface Timer
  *            @arg TIM_COMMUTATION_SOFTWARE:  Commutation source is set by software using the COMG bit
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_ConfigCommutEvent(TIM_HandleTypeDef *htim, uint32_t  InputTrigger,  /* HAL状态类型函数 */
                                              uint32_t  CommutationSource)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_COMMUTATION_EVENT_INSTANCE(htim->Instance));  /* 执行语句 */
  assert_param(IS_TIM_INTERNAL_TRIGGEREVENT_SELECTION(InputTrigger));  /* 执行语句 */

  __HAL_LOCK(htim);                                     /* HAL辅助宏/函数 */

  if ((InputTrigger == TIM_TS_ITR0) || (InputTrigger == TIM_TS_ITR1) ||  /* 条件判断 */
      (InputTrigger == TIM_TS_ITR2) || (InputTrigger == TIM_TS_ITR3))  /* 源代码 */
  {                                                     /* 代码块开始 */
    /* Select the Input trigger */
    htim->Instance->SMCR &= ~TIM_SMCR_TS;               /* 赋值或初始化 */
    htim->Instance->SMCR |= InputTrigger;               /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Select the Capture Compare preload feature */
  htim->Instance->CR2 |= TIM_CR2_CCPC;                  /* 赋值或初始化 */
  /* Select the Commutation event source */
  htim->Instance->CR2 &= ~TIM_CR2_CCUS;                 /* 赋值或初始化 */
  htim->Instance->CR2 |= CommutationSource;             /* 赋值或初始化 */

  /* Disable Commutation Interrupt */
  __HAL_TIM_DISABLE_IT(htim, TIM_IT_COM);               /* HAL辅助宏/函数 */

  /* Disable Commutation DMA request */
  __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_COM);             /* HAL辅助宏/函数 */

  __HAL_UNLOCK(htim);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configure the TIM commutation event sequence with interrupt.
  * @note  This function is mandatory to use the commutation event in order to
  *        update the configuration at each commutation detection on the TRGI input of the Timer,
  *        the typical use of this feature is with the use of another Timer(interface Timer)
  *        configured in Hall sensor interface, this interface Timer will generate the
  *        commutation at its TRGO output (connected to Timer used in this function) each time
  *        the TI1 of the Interface Timer detect a commutation at its input TI1.
  * @param  htim TIM handle
  * @param  InputTrigger the Internal trigger corresponding to the Timer Interfacing with the Hall sensor
  *          This parameter can be one of the following values:
  *            @arg TIM_TS_ITR0: Internal trigger 0 selected
  *            @arg TIM_TS_ITR1: Internal trigger 1 selected
  *            @arg TIM_TS_ITR2: Internal trigger 2 selected
  *            @arg TIM_TS_ITR3: Internal trigger 3 selected
  *            @arg TIM_TS_NONE: No trigger is needed
  * @param  CommutationSource the Commutation Event source
  *          This parameter can be one of the following values:
  *            @arg TIM_COMMUTATION_TRGI: Commutation source is the TRGI of the Interface Timer
  *            @arg TIM_COMMUTATION_SOFTWARE:  Commutation source is set by software using the COMG bit
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_ConfigCommutEvent_IT(TIM_HandleTypeDef *htim, uint32_t  InputTrigger,  /* HAL状态类型函数 */
                                                 uint32_t  CommutationSource)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_COMMUTATION_EVENT_INSTANCE(htim->Instance));  /* 执行语句 */
  assert_param(IS_TIM_INTERNAL_TRIGGEREVENT_SELECTION(InputTrigger));  /* 执行语句 */

  __HAL_LOCK(htim);                                     /* HAL辅助宏/函数 */

  if ((InputTrigger == TIM_TS_ITR0) || (InputTrigger == TIM_TS_ITR1) ||  /* 条件判断 */
      (InputTrigger == TIM_TS_ITR2) || (InputTrigger == TIM_TS_ITR3))  /* 源代码 */
  {                                                     /* 代码块开始 */
    /* Select the Input trigger */
    htim->Instance->SMCR &= ~TIM_SMCR_TS;               /* 赋值或初始化 */
    htim->Instance->SMCR |= InputTrigger;               /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Select the Capture Compare preload feature */
  htim->Instance->CR2 |= TIM_CR2_CCPC;                  /* 赋值或初始化 */
  /* Select the Commutation event source */
  htim->Instance->CR2 &= ~TIM_CR2_CCUS;                 /* 赋值或初始化 */
  htim->Instance->CR2 |= CommutationSource;             /* 赋值或初始化 */

  /* Disable Commutation DMA request */
  __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_COM);             /* HAL辅助宏/函数 */

  /* Enable the Commutation Interrupt */
  __HAL_TIM_ENABLE_IT(htim, TIM_IT_COM);                /* HAL辅助宏/函数 */

  __HAL_UNLOCK(htim);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configure the TIM commutation event sequence with DMA.
  * @note  This function is mandatory to use the commutation event in order to
  *        update the configuration at each commutation detection on the TRGI input of the Timer,
  *        the typical use of this feature is with the use of another Timer(interface Timer)
  *        configured in Hall sensor interface, this interface Timer will generate the
  *        commutation at its TRGO output (connected to Timer used in this function) each time
  *        the TI1 of the Interface Timer detect a commutation at its input TI1.
  * @note  The user should configure the DMA in his own software, in This function only the COMDE bit is set
  * @param  htim TIM handle
  * @param  InputTrigger the Internal trigger corresponding to the Timer Interfacing with the Hall sensor
  *          This parameter can be one of the following values:
  *            @arg TIM_TS_ITR0: Internal trigger 0 selected
  *            @arg TIM_TS_ITR1: Internal trigger 1 selected
  *            @arg TIM_TS_ITR2: Internal trigger 2 selected
  *            @arg TIM_TS_ITR3: Internal trigger 3 selected
  *            @arg TIM_TS_NONE: No trigger is needed
  * @param  CommutationSource the Commutation Event source
  *          This parameter can be one of the following values:
  *            @arg TIM_COMMUTATION_TRGI: Commutation source is the TRGI of the Interface Timer
  *            @arg TIM_COMMUTATION_SOFTWARE:  Commutation source is set by software using the COMG bit
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_ConfigCommutEvent_DMA(TIM_HandleTypeDef *htim, uint32_t  InputTrigger,  /* HAL状态类型函数 */
                                                  uint32_t  CommutationSource)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_COMMUTATION_EVENT_INSTANCE(htim->Instance));  /* 执行语句 */
  assert_param(IS_TIM_INTERNAL_TRIGGEREVENT_SELECTION(InputTrigger));  /* 执行语句 */

  __HAL_LOCK(htim);                                     /* HAL辅助宏/函数 */

  if ((InputTrigger == TIM_TS_ITR0) || (InputTrigger == TIM_TS_ITR1) ||  /* 条件判断 */
      (InputTrigger == TIM_TS_ITR2) || (InputTrigger == TIM_TS_ITR3))  /* 源代码 */
  {                                                     /* 代码块开始 */
    /* Select the Input trigger */
    htim->Instance->SMCR &= ~TIM_SMCR_TS;               /* 赋值或初始化 */
    htim->Instance->SMCR |= InputTrigger;               /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Select the Capture Compare preload feature */
  htim->Instance->CR2 |= TIM_CR2_CCPC;                  /* 赋值或初始化 */
  /* Select the Commutation event source */
  htim->Instance->CR2 &= ~TIM_CR2_CCUS;                 /* 赋值或初始化 */
  htim->Instance->CR2 |= CommutationSource;             /* 赋值或初始化 */

  /* Enable the Commutation DMA Request */
  /* Set the DMA Commutation Callback */
  htim->hdma[TIM_DMA_ID_COMMUTATION]->XferCpltCallback = TIMEx_DMACommutationCplt;  /* 赋值或初始化 */
  htim->hdma[TIM_DMA_ID_COMMUTATION]->XferHalfCpltCallback = TIMEx_DMACommutationHalfCplt;  /* 赋值或初始化 */
  /* Set the DMA error callback */
  htim->hdma[TIM_DMA_ID_COMMUTATION]->XferErrorCallback = TIM_DMAError;  /* 赋值或初始化 */

  /* Disable Commutation Interrupt */
  __HAL_TIM_DISABLE_IT(htim, TIM_IT_COM);               /* HAL辅助宏/函数 */

  /* Enable the Commutation DMA Request */
  __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_COM);              /* HAL辅助宏/函数 */

  __HAL_UNLOCK(htim);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configures the TIM in master mode.
  * @param  htim TIM handle.
  * @param  sMasterConfig pointer to a TIM_MasterConfigTypeDef structure that
  *         contains the selected trigger output (TRGO) and the Master/Slave
  *         mode.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_MasterConfigSynchronization(TIM_HandleTypeDef *htim,  /* HAL状态类型函数 */
                                                        TIM_MasterConfigTypeDef *sMasterConfig)  /* 源代码 */
{                                                       /* 代码块开始 */
  uint32_t tmpcr2;                                      /* 32位无符号变量/函数 */
  uint32_t tmpsmcr;                                     /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_TIM_MASTER_INSTANCE(htim->Instance));  /* 执行语句 */
  assert_param(IS_TIM_TRGO_SOURCE(sMasterConfig->MasterOutputTrigger));  /* 执行语句 */
  assert_param(IS_TIM_MSM_STATE(sMasterConfig->MasterSlaveMode));  /* 执行语句 */

  /* Check input state */
  __HAL_LOCK(htim);                                     /* HAL辅助宏/函数 */

  /* Change the handler state */
  htim->State = HAL_TIM_STATE_BUSY;                     /* 赋值或初始化 */

  /* Get the TIMx CR2 register value */
  tmpcr2 = htim->Instance->CR2;                         /* 赋值或初始化 */

  /* Get the TIMx SMCR register value */
  tmpsmcr = htim->Instance->SMCR;                       /* 赋值或初始化 */

  /* Reset the MMS Bits */
  tmpcr2 &= ~TIM_CR2_MMS;                               /* 赋值或初始化 */
  /* Select the TRGO source */
  tmpcr2 |=  sMasterConfig->MasterOutputTrigger;        /* 赋值或初始化 */

  /* Update TIMx CR2 */
  htim->Instance->CR2 = tmpcr2;                         /* 赋值或初始化 */

  if (IS_TIM_SLAVE_INSTANCE(htim->Instance))            /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Reset the MSM Bit */
    tmpsmcr &= ~TIM_SMCR_MSM;                           /* 赋值或初始化 */
    /* Set master mode */
    tmpsmcr |= sMasterConfig->MasterSlaveMode;          /* 赋值或初始化 */

    /* Update TIMx SMCR */
    htim->Instance->SMCR = tmpsmcr;                     /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Change the htim state */
  htim->State = HAL_TIM_STATE_READY;                    /* 赋值或初始化 */

  __HAL_UNLOCK(htim);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configures the Break feature, dead time, Lock level, OSSI/OSSR State
  *         and the AOE(automatic output enable).
  * @param  htim TIM handle
  * @param  sBreakDeadTimeConfig pointer to a TIM_ConfigBreakDeadConfigTypeDef structure that
  *         contains the BDTR Register configuration  information for the TIM peripheral.
  * @note   Interrupts can be generated when an active level is detected on the
  *         break input, the break 2 input or the system break input. Break
  *         interrupt can be enabled by calling the @ref __HAL_TIM_ENABLE_IT macro.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_ConfigBreakDeadTime(TIM_HandleTypeDef *htim,  /* HAL状态类型函数 */
                                                TIM_BreakDeadTimeConfigTypeDef *sBreakDeadTimeConfig)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Keep this variable initialized to 0 as it is used to configure BDTR register */
  uint32_t tmpbdtr = 0U;                                /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_TIM_BREAK_INSTANCE(htim->Instance));  /* 执行语句 */
  assert_param(IS_TIM_OSSR_STATE(sBreakDeadTimeConfig->OffStateRunMode));  /* 执行语句 */
  assert_param(IS_TIM_OSSI_STATE(sBreakDeadTimeConfig->OffStateIDLEMode));  /* 执行语句 */
  assert_param(IS_TIM_LOCK_LEVEL(sBreakDeadTimeConfig->LockLevel));  /* 执行语句 */
  assert_param(IS_TIM_DEADTIME(sBreakDeadTimeConfig->DeadTime));  /* 执行语句 */
  assert_param(IS_TIM_BREAK_STATE(sBreakDeadTimeConfig->BreakState));  /* 执行语句 */
  assert_param(IS_TIM_BREAK_POLARITY(sBreakDeadTimeConfig->BreakPolarity));  /* 执行语句 */
  assert_param(IS_TIM_AUTOMATIC_OUTPUT_STATE(sBreakDeadTimeConfig->AutomaticOutput));  /* 执行语句 */

  /* Check input state */
  __HAL_LOCK(htim);                                     /* HAL辅助宏/函数 */

  /* Set the Lock level, the Break enable Bit and the Polarity, the OSSR State,
     the OSSI State, the dead time value and the Automatic Output Enable Bit */

  /* Set the BDTR bits */
  MODIFY_REG(tmpbdtr, TIM_BDTR_DTG, sBreakDeadTimeConfig->DeadTime);  /* 执行语句 */
  MODIFY_REG(tmpbdtr, TIM_BDTR_LOCK, sBreakDeadTimeConfig->LockLevel);  /* 执行语句 */
  MODIFY_REG(tmpbdtr, TIM_BDTR_OSSI, sBreakDeadTimeConfig->OffStateIDLEMode);  /* 执行语句 */
  MODIFY_REG(tmpbdtr, TIM_BDTR_OSSR, sBreakDeadTimeConfig->OffStateRunMode);  /* 执行语句 */
  MODIFY_REG(tmpbdtr, TIM_BDTR_BKE, sBreakDeadTimeConfig->BreakState);  /* 执行语句 */
  MODIFY_REG(tmpbdtr, TIM_BDTR_BKP, sBreakDeadTimeConfig->BreakPolarity);  /* 执行语句 */
  MODIFY_REG(tmpbdtr, TIM_BDTR_AOE, sBreakDeadTimeConfig->AutomaticOutput);  /* 执行语句 */


  /* Set TIMx_BDTR */
  htim->Instance->BDTR = tmpbdtr;                       /* 赋值或初始化 */

  __HAL_UNLOCK(htim);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configures the TIMx Remapping input capabilities.
  * @param  htim TIM handle.
  * @param  Remap specifies the TIM remapping source.
  *         For TIM1, the parameter can have the following values:                   (**)
  *           @arg TIM_TIM1_TIM3_TRGO:  TIM1 ITR2 is connected to TIM3 TRGO
  *           @arg TIM_TIM1_LPTIM:      TIM1 ITR2 is connected to LPTIM1 output
  *
  *         For TIM2, the parameter can have the following values:                   (**)
  *           @arg TIM_TIM2_TIM8_TRGO:  TIM2 ITR1 is connected to TIM8 TRGO          (*)
  *           @arg TIM_TIM2_ETH_PTP:    TIM2 ITR1 is connected to PTP trigger output (*)
  *           @arg TIM_TIM2_USBFS_SOF:  TIM2 ITR1 is connected to OTG FS SOF
  *           @arg TIM_TIM2_USBHS_SOF:  TIM2 ITR1 is connected to OTG FS SOF
  *
  *         For TIM5, the parameter can have the following values:
  *           @arg TIM_TIM5_GPIO:       TIM5 TI4 is connected to GPIO
  *           @arg TIM_TIM5_LSI:        TIM5 TI4 is connected to LSI
  *           @arg TIM_TIM5_LSE:        TIM5 TI4 is connected to LSE
  *           @arg TIM_TIM5_RTC:        TIM5 TI4 is connected to the RTC wakeup interrupt
  *           @arg TIM_TIM5_TIM3_TRGO:  TIM5 ITR1 is connected to TIM3 TRGO          (*)
  *           @arg TIM_TIM5_LPTIM:      TIM5 ITR1 is connected to LPTIM1 output      (*)
  *
  *         For TIM9, the parameter can have the following values:                   (**)
  *           @arg TIM_TIM9_TIM3_TRGO:  TIM9 ITR1 is connected to TIM3 TRGO
  *           @arg TIM_TIM9_LPTIM:      TIM9 ITR1 is connected to LPTIM1 output
  *
  *         For TIM11, the parameter can have the following values:
  *           @arg TIM_TIM11_GPIO:     TIM11 TI1 is connected to GPIO
  *           @arg TIM_TIM11_HSE:      TIM11 TI1 is connected to HSE_RTC clock
  *           @arg TIM_TIM11_SPDIFRX:  TIM11 TI1 is connected to SPDIFRX_FRAME_SYNC  (*)
  *
  *         (*)  Value not defined in all devices. \n
  *         (**) Register not available in all devices.
  *
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_RemapConfig(TIM_HandleTypeDef *htim, uint32_t Remap)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  __HAL_LOCK(htim);                                     /* HAL辅助宏/函数 */

  /* Check parameters */
  assert_param(IS_TIM_REMAP(htim->Instance, Remap));    /* 执行语句 */

#if defined(LPTIM_OR_TIM1_ITR2_RMP) && defined(LPTIM_OR_TIM5_ITR1_RMP) && defined(LPTIM_OR_TIM9_ITR1_RMP)  /* 条件编译 */
  if ((Remap & LPTIM_REMAP_MASK) == LPTIM_REMAP_MASK)   /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Connect TIMx internal trigger to LPTIM1 output */
    __HAL_RCC_LPTIM1_CLK_ENABLE();                      /* HAL辅助宏/函数 */
    MODIFY_REG(LPTIM1->OR,                              /* 列表续行 */
               (LPTIM_OR_TIM1_ITR2_RMP | LPTIM_OR_TIM5_ITR1_RMP | LPTIM_OR_TIM9_ITR1_RMP),  /* 列表续行 */
               Remap & ~(LPTIM_REMAP_MASK));            /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Set the Timer remapping configuration */
    WRITE_REG(htim->Instance->OR, Remap);               /* 执行语句 */
  }                                                     /* 代码块结束 */
#else                                                   /* 条件编译否则 */
  /* Set the Timer remapping configuration */
  WRITE_REG(htim->Instance->OR, Remap);                 /* 执行语句 */
#endif /* LPTIM_OR_TIM1_ITR2_RMP &&  LPTIM_OR_TIM5_ITR1_RMP && LPTIM_OR_TIM9_ITR1_RMP */

  __HAL_UNLOCK(htim);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup TIMEx_Exported_Functions_Group6 Extended Callbacks functions
  * @brief    Extended Callbacks functions
  *
@verbatim
  ==============================================================================
                    ##### Extended Callbacks functions #####
  ==============================================================================
  [..]
    This section provides Extended TIM callback functions:
    (+) Timer Commutation callback
    (+) Timer Break callback

@endverbatim
  * @{
  */

/**
  * @brief  Hall commutation changed callback in non-blocking mode
  * @param  htim TIM handle
  * @retval None
  */
__weak void HAL_TIMEx_CommutCallback(TIM_HandleTypeDef *htim)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIMEx_CommutCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */
/**
  * @brief  Hall commutation changed half complete callback in non-blocking mode
  * @param  htim TIM handle
  * @retval None
  */
__weak void HAL_TIMEx_CommutHalfCpltCallback(TIM_HandleTypeDef *htim)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIMEx_CommutHalfCpltCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Hall Break detection callback in non-blocking mode
  * @param  htim TIM handle
  * @retval None
  */
__weak void HAL_TIMEx_BreakCallback(TIM_HandleTypeDef *htim)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIMEx_BreakCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */
/**
  * @}
  */

/** @defgroup TIMEx_Exported_Functions_Group7 Extended Peripheral State functions
  * @brief    Extended Peripheral State functions
  *
@verbatim
  ==============================================================================
                ##### Extended Peripheral State functions #####
  ==============================================================================
  [..]
    This subsection permits to get in run-time the status of the peripheral
    and the data flow.

@endverbatim
  * @{
  */

/**
  * @brief  Return the TIM Hall Sensor interface handle state.
  * @param  htim TIM Hall Sensor handle
  * @retval HAL state
  */
HAL_TIM_StateTypeDef HAL_TIMEx_HallSensor_GetState(TIM_HandleTypeDef *htim)  /* HAL 定时器操作 */
{                                                       /* 代码块开始 */
  return htim->State;                                   /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return actual state of the TIM complementary channel.
  * @param  htim TIM handle
  * @param  ChannelN TIM Complementary channel
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1
  *            @arg TIM_CHANNEL_2: TIM Channel 2
  *            @arg TIM_CHANNEL_3: TIM Channel 3
  * @retval TIM Complementary channel state
  */
HAL_TIM_ChannelStateTypeDef HAL_TIMEx_GetChannelNState(TIM_HandleTypeDef *htim,  uint32_t ChannelN)  /* HAL 定时器操作 */
{                                                       /* 代码块开始 */
  HAL_TIM_ChannelStateTypeDef channel_state;            /* HAL 定时器操作 */

  /* Check the parameters */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, ChannelN));  /* 执行语句 */

  channel_state = TIM_CHANNEL_N_STATE_GET(htim, ChannelN);  /* 赋值或初始化 */

  return channel_state;                                 /* 返回值 */
}                                                       /* 代码块结束 */
/**
  * @}
  */

/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/
/** @defgroup TIMEx_Private_Functions TIMEx Private Functions
  * @{
  */

/**
  * @brief  TIM DMA Commutation callback.
  * @param  hdma pointer to DMA handle.
  * @retval None
  */
void TIMEx_DMACommutationCplt(DMA_HandleTypeDef *hdma)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  TIM_HandleTypeDef *htim = (TIM_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;  /* 赋值或初始化 */

  /* Change the htim state */
  htim->State = HAL_TIM_STATE_READY;                    /* 赋值或初始化 */

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)               /* 条件编译 */
  htim->CommutationCallback(htim);                      /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_TIMEx_CommutCallback(htim);                       /* HAL库函数调用 */
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
}                                                       /* 代码块结束 */

/**
  * @brief  TIM DMA Commutation half complete callback.
  * @param  hdma pointer to DMA handle.
  * @retval None
  */
void TIMEx_DMACommutationHalfCplt(DMA_HandleTypeDef *hdma)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  TIM_HandleTypeDef *htim = (TIM_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;  /* 赋值或初始化 */

  /* Change the htim state */
  htim->State = HAL_TIM_STATE_READY;                    /* 赋值或初始化 */

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)               /* 条件编译 */
  htim->CommutationHalfCpltCallback(htim);              /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_TIMEx_CommutHalfCpltCallback(htim);               /* HAL库函数调用 */
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
}                                                       /* 代码块结束 */


/**
  * @brief  TIM DMA Delay Pulse complete callback (complementary channel).
  * @param  hdma pointer to DMA handle.
  * @retval None
  */
static void TIM_DMADelayPulseNCplt(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  TIM_HandleTypeDef *htim = (TIM_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;  /* 赋值或初始化 */

  if (hdma == htim->hdma[TIM_DMA_ID_CC1])               /* 条件判断 */
  {                                                     /* 代码块开始 */
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_1;           /* 赋值或初始化 */

    if (hdma->Init.Mode == DMA_NORMAL)                  /* 条件判断 */
    {                                                   /* 代码块开始 */
      TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (hdma == htim->hdma[TIM_DMA_ID_CC2])          /* 否则如果 */
  {                                                     /* 代码块开始 */
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_2;           /* 赋值或初始化 */

    if (hdma->Init.Mode == DMA_NORMAL)                  /* 条件判断 */
    {                                                   /* 代码块开始 */
      TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (hdma == htim->hdma[TIM_DMA_ID_CC3])          /* 否则如果 */
  {                                                     /* 代码块开始 */
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_3;           /* 赋值或初始化 */

    if (hdma->Init.Mode == DMA_NORMAL)                  /* 条件判断 */
    {                                                   /* 代码块开始 */
      TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_3, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (hdma == htim->hdma[TIM_DMA_ID_CC4])          /* 否则如果 */
  {                                                     /* 代码块开始 */
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_4;           /* 赋值或初始化 */

    if (hdma->Init.Mode == DMA_NORMAL)                  /* 条件判断 */
    {                                                   /* 代码块开始 */
      TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_4, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
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
  * @brief  TIM DMA error callback (complementary channel)
  * @param  hdma pointer to DMA handle.
  * @retval None
  */
static void TIM_DMAErrorCCxN(DMA_HandleTypeDef *hdma)   /* 静态函数 */
{                                                       /* 代码块开始 */
  TIM_HandleTypeDef *htim = (TIM_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;  /* 赋值或初始化 */

  if (hdma == htim->hdma[TIM_DMA_ID_CC1])               /* 条件判断 */
  {                                                     /* 代码块开始 */
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_1;           /* 赋值或初始化 */
    TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
  }                                                     /* 代码块结束 */
  else if (hdma == htim->hdma[TIM_DMA_ID_CC2])          /* 否则如果 */
  {                                                     /* 代码块开始 */
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_2;           /* 赋值或初始化 */
    TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
  }                                                     /* 代码块结束 */
  else if (hdma == htim->hdma[TIM_DMA_ID_CC3])          /* 否则如果 */
  {                                                     /* 代码块开始 */
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_3;           /* 赋值或初始化 */
    TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_3, HAL_TIM_CHANNEL_STATE_READY);  /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* nothing to do */
  }                                                     /* 代码块结束 */

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)               /* 条件编译 */
  htim->ErrorCallback(htim);                            /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_TIM_ErrorCallback(htim);                          /* HAL 定时器操作 */
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */

  htim->Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;       /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  Enables or disables the TIM Capture Compare Channel xN.
  * @param  TIMx to select the TIM peripheral
  * @param  Channel specifies the TIM Channel
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1
  *            @arg TIM_CHANNEL_2: TIM Channel 2
  *            @arg TIM_CHANNEL_3: TIM Channel 3
  * @param  ChannelNState specifies the TIM Channel CCxNE bit new state.
  *          This parameter can be: TIM_CCxN_ENABLE or TIM_CCxN_Disable.
  * @retval None
  */
static void TIM_CCxNChannelCmd(TIM_TypeDef *TIMx, uint32_t Channel, uint32_t ChannelNState)  /* 静态函数 */
{                                                       /* 代码块开始 */
  uint32_t tmp;                                         /* 32位无符号变量/函数 */

  tmp = TIM_CCER_CC1NE << (Channel & 0x1FU); /* 0x1FU = 31 bits max shift */

  /* Reset the CCxNE Bit */
  TIMx->CCER &=  ~tmp;                                  /* 赋值或初始化 */

  /* Set or reset the CCxNE Bit */
  TIMx->CCER |= (uint32_t)(ChannelNState << (Channel & 0x1FU)); /* 0x1FU = 31 bits max shift */
}                                                       /* 代码块结束 */
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
