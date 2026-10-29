/* 文件: stm32f4xx_hal_spdifrx.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_spdifrx.c
  * @author  MCD Application Team
  * @brief   This file provides firmware functions to manage the following
  *          functionalities of the SPDIFRX audio interface:
  *           + Initialization and Configuration
  *           + Data transfers functions
  *           + DMA transfers management
  *           + Interrupts and flags management
  @verbatim
 ===============================================================================
                  ##### How to use this driver #####
 ===============================================================================
 [..]
    The SPDIFRX HAL driver can be used as follow:

    (#) Declare SPDIFRX_HandleTypeDef handle structure.
    (#) Initialize the SPDIFRX low level resources by implement the HAL_SPDIFRX_MspInit() API:
        (##) Enable the SPDIFRX interface clock.
        (##) SPDIFRX pins configuration:
            (+++) Enable the clock for the SPDIFRX GPIOs.
            (+++) Configure these SPDIFRX pins as alternate function pull-up.
        (##) NVIC configuration if you need to use interrupt process (HAL_SPDIFRX_ReceiveControlFlow_IT() and HAL_SPDIFRX_ReceiveDataFlow_IT() API's).
            (+++) Configure the SPDIFRX interrupt priority.
            (+++) Enable the NVIC SPDIFRX IRQ handle.
        (##) DMA Configuration if you need to use DMA process (HAL_SPDIFRX_ReceiveDataFlow_DMA() and HAL_SPDIFRX_ReceiveControlFlow_DMA() API's).
            (+++) Declare a DMA handle structure for the reception of the Data Flow channel.
            (+++) Declare a DMA handle structure for the reception of the Control Flow channel.
            (+++) Enable the DMAx interface clock.
            (+++) Configure the declared DMA handle structure CtrlRx/DataRx with the required parameters.
            (+++) Configure the DMA Channel.
            (+++) Associate the initialized DMA handle to the SPDIFRX DMA CtrlRx/DataRx handle.
            (+++) Configure the priority and enable the NVIC for the transfer complete interrupt on the
                DMA CtrlRx/DataRx channel.

   (#) Program the input selection, re-tries number, wait for activity, channel status selection, data format, stereo mode and masking of user bits
       using HAL_SPDIFRX_Init() function.

   -@- The specific SPDIFRX interrupts (RXNE/CSRNE and Error Interrupts) will be managed using the macros
       __SPDIFRX_ENABLE_IT() and __SPDIFRX_DISABLE_IT() inside the receive process.
   -@- Make sure that ck_spdif clock is configured.

   (#) Three operation modes are available within this driver :

   *** Polling mode for reception operation (for debug purpose) ***
   ================================================================
   [..]
     (+) Receive data flow in blocking mode using HAL_SPDIFRX_ReceiveDataFlow()
     (+) Receive control flow of data in blocking mode using HAL_SPDIFRX_ReceiveControlFlow()

   *** Interrupt mode for reception operation ***
   =========================================
   [..]
     (+) Receive an amount of data (Data Flow) in non blocking mode using HAL_SPDIFRX_ReceiveDataFlow_IT()
     (+) Receive an amount of data (Control Flow) in non blocking mode using HAL_SPDIFRX_ReceiveControlFlow_IT()
     (+) At reception end of half transfer HAL_SPDIFRX_RxHalfCpltCallback is executed and user can
         add his own code by customization of function pointer HAL_SPDIFRX_RxHalfCpltCallback
     (+) At reception end of transfer HAL_SPDIFRX_RxCpltCallback is executed and user can
         add his own code by customization of function pointer HAL_SPDIFRX_RxCpltCallback
     (+) In case of transfer Error, HAL_SPDIFRX_ErrorCallback() function is executed and user can
         add his own code by customization of function pointer HAL_SPDIFRX_ErrorCallback

   *** DMA mode for reception operation ***
   ========================================
   [..]
     (+) Receive an amount of data (Data Flow) in non blocking mode (DMA) using HAL_SPDIFRX_ReceiveDataFlow_DMA()
     (+) Receive an amount of data (Control Flow) in non blocking mode (DMA) using HAL_SPDIFRX_ReceiveControlFlow_DMA()
     (+) At reception end of half transfer HAL_SPDIFRX_RxHalfCpltCallback is executed and user can
         add his own code by customization of function pointer HAL_SPDIFRX_RxHalfCpltCallback
     (+) At reception end of transfer HAL_SPDIFRX_RxCpltCallback is executed and user can
         add his own code by customization of function pointer HAL_SPDIFRX_RxCpltCallback
     (+) In case of transfer Error, HAL_SPDIFRX_ErrorCallback() function is executed and user can
         add his own code by customization of function pointer HAL_SPDIFRX_ErrorCallback
     (+) Stop the DMA Transfer using HAL_SPDIFRX_DMAStop()

   *** SPDIFRX HAL driver macros list ***
   =============================================
   [..]
     Below the list of most used macros in SPDIFRX HAL driver.
      (+) __HAL_SPDIFRX_IDLE: Disable the specified SPDIFRX peripheral (IDEL State)
      (+) __HAL_SPDIFRX_SYNC: Enable the synchronization state of the specified SPDIFRX peripheral (SYNC State)
      (+) __HAL_SPDIFRX_RCV: Enable the receive state of the specified SPDIFRX peripheral (RCV State)
      (+) __HAL_SPDIFRX_ENABLE_IT : Enable the specified SPDIFRX interrupts
      (+) __HAL_SPDIFRX_DISABLE_IT : Disable the specified SPDIFRX interrupts
      (+) __HAL_SPDIFRX_GET_FLAG: Check whether the specified SPDIFRX flag is set or not.

   [..]
      (@) You can refer to the SPDIFRX HAL driver header file for more useful macros

  *** Callback registration ***
  =============================================

  The compilation define  USE_HAL_SPDIFRX_REGISTER_CALLBACKS when set to 1
  allows the user to configure dynamically the driver callbacks.
  Use HAL_SPDIFRX_RegisterCallback() function to register an interrupt callback.

  The HAL_SPDIFRX_RegisterCallback() function allows to register the following callbacks:
    (+) RxHalfCpltCallback  : SPDIFRX Data flow half completed callback.
    (+) RxCpltCallback      : SPDIFRX Data flow completed callback.
    (+) CxHalfCpltCallback  : SPDIFRX Control flow half completed callback.
    (+) CxCpltCallback      : SPDIFRX Control flow completed callback.
    (+) ErrorCallback       : SPDIFRX error callback.
    (+) MspInitCallback     : SPDIFRX MspInit.
    (+) MspDeInitCallback   : SPDIFRX MspDeInit.
  This function takes as parameters the HAL peripheral handle, the Callback ID
  and a pointer to the user callback function.

  Use HAL_SPDIFRX_UnRegisterCallback() function to reset a callback to the default
  weak function.
  The HAL_SPDIFRX_UnRegisterCallback() function takes as parameters the HAL peripheral handle,
  and the Callback ID.
  This function allows to reset the following callbacks:
    (+) RxHalfCpltCallback  : SPDIFRX Data flow half completed callback.
    (+) RxCpltCallback      : SPDIFRX Data flow completed callback.
    (+) CxHalfCpltCallback  : SPDIFRX Control flow half completed callback.
    (+) CxCpltCallback      : SPDIFRX Control flow completed callback.
    (+) ErrorCallback       : SPDIFRX error callback.
    (+) MspInitCallback     : SPDIFRX MspInit.
    (+) MspDeInitCallback   : SPDIFRX MspDeInit.

  By default, after the HAL_SPDIFRX_Init() and when the state is HAL_SPDIFRX_STATE_RESET
  all callbacks are set to the corresponding weak functions :
  HAL_SPDIFRX_RxHalfCpltCallback() , HAL_SPDIFRX_RxCpltCallback(), HAL_SPDIFRX_CxHalfCpltCallback(),
  HAL_SPDIFRX_CxCpltCallback() and HAL_SPDIFRX_ErrorCallback()
  Exception done for MspInit and MspDeInit functions that are
  reset to the legacy weak function in the HAL_SPDIFRX_Init()/ HAL_SPDIFRX_DeInit() only when
  these callbacks pointers are NULL (not registered beforehand).
  If not, MspInit or MspDeInit callbacks pointers are not null, the HAL_SPDIFRX_Init() / HAL_SPDIFRX_DeInit()
  keep and use the user MspInit/MspDeInit functions (registered beforehand)

  Callbacks can be registered/unregistered in HAL_SPDIFRX_STATE_READY state only.
  Exception done MspInit/MspDeInit callbacks that can be registered/unregistered
  in HAL_SPDIFRX_STATE_READY or HAL_SPDIFRX_STATE_RESET state,
  thus registered (user) MspInit/DeInit callbacks can be used during the Init/DeInit.
  In that case first register the MspInit/MspDeInit user callbacks
  using HAL_SPDIFRX_RegisterCallback() before calling HAL_SPDIFRX_DeInit()
  or HAL_SPDIFRX_Init() function.

  When The compilation define USE_HAL_SPDIFRX_REGISTER_CALLBACKS is set to 0 or
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

/** @defgroup SPDIFRX SPDIFRX
  * @brief SPDIFRX HAL module driver
  * @{
  */

#ifdef HAL_SPDIFRX_MODULE_ENABLED                       /* 条件编译 */
#if defined (SPDIFRX)                                   /* 条件编译 */
#if defined(STM32F446xx)                                /* 条件编译 */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SPDIFRX_TIMEOUT_VALUE  0xFFFFU                  /* 定义宏 */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/** @addtogroup SPDIFRX_Private_Functions
  * @{
  */
static void  SPDIFRX_DMARxCplt(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void  SPDIFRX_DMARxHalfCplt(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void  SPDIFRX_DMACxCplt(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void  SPDIFRX_DMACxHalfCplt(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void  SPDIFRX_DMAError(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void  SPDIFRX_ReceiveControlFlow_IT(SPDIFRX_HandleTypeDef *hspdif);  /* 静态函数 */
static void  SPDIFRX_ReceiveDataFlow_IT(SPDIFRX_HandleTypeDef *hspdif);  /* 静态函数 */
static HAL_StatusTypeDef  SPDIFRX_WaitOnFlagUntilTimeout(SPDIFRX_HandleTypeDef *hspdif, uint32_t Flag, FlagStatus Status, uint32_t Timeout, uint32_t tickstart);  /* 静态变量/函数 */
/**
  * @}
  */
/* Exported functions ---------------------------------------------------------*/

/** @defgroup SPDIFRX_Exported_Functions SPDIFRX Exported Functions
  * @{
  */

/** @defgroup  SPDIFRX_Exported_Functions_Group1 Initialization and de-initialization functions
  *  @brief    Initialization and Configuration functions
  *
  @verbatim
  ===============================================================================
  ##### Initialization and de-initialization functions #####
  ===============================================================================
  [..]  This subsection provides a set of functions allowing to initialize and
        de-initialize the SPDIFRX peripheral:

  (+) User must Implement HAL_SPDIFRX_MspInit() function in which he configures
      all related peripherals resources (CLOCK, GPIO, DMA, IT and NVIC ).

  (+) Call the function HAL_SPDIFRX_Init() to configure the SPDIFRX peripheral with
      the selected configuration:
  (++) Input Selection (IN0, IN1,...)
  (++) Maximum allowed re-tries during synchronization phase
  (++) Wait for activity on SPDIF selected input
  (++) Channel status selection (from channel A or B)
  (++) Data format (LSB, MSB, ...)
  (++) Stereo mode
  (++) User bits masking (PT,C,U,V,...)

  (+) Call the function HAL_SPDIFRX_DeInit() to restore the default configuration
      of the selected SPDIFRXx peripheral.
  @endverbatim
  * @{
  */

/**
  * @brief Initializes the SPDIFRX according to the specified parameters
  *        in the SPDIFRX_InitTypeDef and create the associated handle.
  * @param hspdif SPDIFRX handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPDIFRX_Init(SPDIFRX_HandleTypeDef *hspdif)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpreg;                                      /* 32位无符号变量/函数 */

  /* Check the SPDIFRX handle allocation */
  if(hspdif == NULL)                                    /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the SPDIFRX parameters */
  assert_param(IS_STEREO_MODE(hspdif->Init.StereoMode));  /* 执行语句 */
  assert_param(IS_SPDIFRX_INPUT_SELECT(hspdif->Init.InputSelection));  /* 执行语句 */
  assert_param(IS_SPDIFRX_MAX_RETRIES(hspdif->Init.Retries));  /* 执行语句 */
  assert_param(IS_SPDIFRX_WAIT_FOR_ACTIVITY(hspdif->Init.WaitForActivity));  /* 执行语句 */
  assert_param(IS_SPDIFRX_CHANNEL(hspdif->Init.ChannelSelection));  /* 执行语句 */
  assert_param(IS_SPDIFRX_DATA_FORMAT(hspdif->Init.DataFormat));  /* 执行语句 */
  assert_param(IS_PREAMBLE_TYPE_MASK(hspdif->Init.PreambleTypeMask));  /* 执行语句 */
  assert_param(IS_CHANNEL_STATUS_MASK(hspdif->Init.ChannelStatusMask));  /* 执行语句 */
  assert_param(IS_VALIDITY_MASK(hspdif->Init.ValidityBitMask));  /* 执行语句 */
  assert_param(IS_PARITY_ERROR_MASK(hspdif->Init.ParityErrorMask));  /* 执行语句 */

#if (USE_HAL_SPDIFRX_REGISTER_CALLBACKS == 1)           /* 条件编译 */
  if(hspdif->State == HAL_SPDIFRX_STATE_RESET)          /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Allocate lock resource and initialize it */
    hspdif->Lock = HAL_UNLOCKED;                        /* 赋值或初始化 */

    hspdif->RxHalfCpltCallback  = HAL_SPDIFRX_RxHalfCpltCallback; /* Legacy weak RxHalfCpltCallback */
    hspdif->RxCpltCallback      = HAL_SPDIFRX_RxCpltCallback;     /* Legacy weak RxCpltCallback     */
    hspdif->CxHalfCpltCallback  = HAL_SPDIFRX_CxHalfCpltCallback; /* Legacy weak CxHalfCpltCallback */
    hspdif->CxCpltCallback      = HAL_SPDIFRX_CxCpltCallback;     /* Legacy weak CxCpltCallback     */
    hspdif->ErrorCallback       = HAL_SPDIFRX_ErrorCallback;      /* Legacy weak ErrorCallback      */

    if(hspdif->MspInitCallback == NULL)                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      hspdif->MspInitCallback = HAL_SPDIFRX_MspInit; /* Legacy weak MspInit  */
    }                                                   /* 代码块结束 */

    /* Init the low level hardware */
    hspdif->MspInitCallback(hspdif);                    /* 执行语句 */
  }                                                     /* 代码块结束 */
#else                                                   /* 条件编译否则 */
  if(hspdif->State == HAL_SPDIFRX_STATE_RESET)          /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Allocate lock resource and initialize it */
    hspdif->Lock = HAL_UNLOCKED;                        /* 赋值或初始化 */
    /* Init the low level hardware : GPIO, CLOCK, CORTEX...etc */
    HAL_SPDIFRX_MspInit(hspdif);                        /* HAL库函数调用 */
  }                                                     /* 代码块结束 */
#endif /* USE_HAL_SPDIFRX_REGISTER_CALLBACKS */

  /* SPDIFRX peripheral state is BUSY */
  hspdif->State = HAL_SPDIFRX_STATE_BUSY;               /* 赋值或初始化 */

  /* Disable SPDIFRX interface (IDLE State) */
  __HAL_SPDIFRX_IDLE(hspdif);                           /* HAL辅助宏/函数 */

  /* Reset the old SPDIFRX CR configuration */
  tmpreg = hspdif->Instance->CR;                        /* 赋值或初始化 */

  tmpreg &= ~(SPDIFRX_CR_RXSTEO  | SPDIFRX_CR_DRFMT  | SPDIFRX_CR_PMSK |  /* 源代码 */
              SPDIFRX_CR_VMSK | SPDIFRX_CR_CUMSK | SPDIFRX_CR_PTMSK  |  /* 源代码 */
              SPDIFRX_CR_CHSEL | SPDIFRX_CR_NBTR | SPDIFRX_CR_WFA |  /* 源代码 */
              SPDIFRX_CR_INSEL);                        /* 执行语句 */

  /* Sets the new configuration of the SPDIFRX peripheral */
  tmpreg |= (hspdif->Init.StereoMode |                  /* 源代码 */
             hspdif->Init.InputSelection |              /* 源代码 */
             hspdif->Init.Retries |                     /* 源代码 */
             hspdif->Init.WaitForActivity |             /* 源代码 */
             hspdif->Init.ChannelSelection |            /* 源代码 */
             hspdif->Init.DataFormat |                  /* 源代码 */
             hspdif->Init.PreambleTypeMask |            /* 源代码 */
             hspdif->Init.ChannelStatusMask |           /* 源代码 */
             hspdif->Init.ValidityBitMask |             /* 源代码 */
             hspdif->Init.ParityErrorMask               /* 源代码 */
             );                                         /* 执行语句 */


  hspdif->Instance->CR = tmpreg;                        /* 赋值或初始化 */

  hspdif->ErrorCode = HAL_SPDIFRX_ERROR_NONE;           /* 赋值或初始化 */

  /* SPDIFRX peripheral state is READY*/
  hspdif->State = HAL_SPDIFRX_STATE_READY;              /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief DeInitializes the SPDIFRX peripheral
  * @param hspdif SPDIFRX handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPDIFRX_DeInit(SPDIFRX_HandleTypeDef *hspdif)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the SPDIFRX handle allocation */
  if(hspdif == NULL)                                    /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the parameters */
  assert_param(IS_SPDIFRX_ALL_INSTANCE(hspdif->Instance));  /* 执行语句 */

  hspdif->State = HAL_SPDIFRX_STATE_BUSY;               /* 赋值或初始化 */

  /* Disable SPDIFRX interface (IDLE state) */
  __HAL_SPDIFRX_IDLE(hspdif);                           /* HAL辅助宏/函数 */

#if (USE_HAL_SPDIFRX_REGISTER_CALLBACKS == 1)           /* 条件编译 */
  if(hspdif->MspDeInitCallback == NULL)                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    hspdif->MspDeInitCallback = HAL_SPDIFRX_MspDeInit; /* Legacy weak MspDeInit  */
  }                                                     /* 代码块结束 */

  /* DeInit the low level hardware */
  hspdif->MspDeInitCallback(hspdif);                    /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* DeInit the low level hardware: GPIO, CLOCK, NVIC... */
  HAL_SPDIFRX_MspDeInit(hspdif);                        /* HAL库函数调用 */
#endif /* USE_HAL_SPDIFRX_REGISTER_CALLBACKS */

  hspdif->ErrorCode = HAL_SPDIFRX_ERROR_NONE;           /* 赋值或初始化 */

  /* SPDIFRX peripheral state is RESET*/
  hspdif->State = HAL_SPDIFRX_STATE_RESET;              /* 赋值或初始化 */

  /* Release Lock */
  __HAL_UNLOCK(hspdif);                                 /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief SPDIFRX MSP Init
  * @param hspdif SPDIFRX handle
  * @retval None
  */
__weak void HAL_SPDIFRX_MspInit(SPDIFRX_HandleTypeDef *hspdif)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hspdif);                                       /* 执行语句 */

  /* NOTE : This function Should not be modified, when the callback is needed,
  the HAL_SPDIFRX_MspInit could be implemented in the user file
  */
}                                                       /* 代码块结束 */

/**
  * @brief SPDIFRX MSP DeInit
  * @param hspdif SPDIFRX handle
  * @retval None
  */
__weak void HAL_SPDIFRX_MspDeInit(SPDIFRX_HandleTypeDef *hspdif)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hspdif);                                       /* 执行语句 */

  /* NOTE : This function Should not be modified, when the callback is needed,
  the HAL_SPDIFRX_MspDeInit could be implemented in the user file
  */
}                                                       /* 代码块结束 */

#if (USE_HAL_SPDIFRX_REGISTER_CALLBACKS == 1)           /* 条件编译 */
/**
  * @brief  Register a User SPDIFRX Callback
  *         To be used instead of the weak predefined callback
  * @param  hspdif SPDIFRX handle
  * @param  CallbackID ID of the callback to be registered
  *         This parameter can be one of the following values:
  *          @arg @ref HAL_SPDIFRX_RX_HALF_CB_ID    SPDIFRX Data flow half completed callback ID
  *          @arg @ref HAL_SPDIFRX_RX_CPLT_CB_ID    SPDIFRX Data flow completed callback ID
  *          @arg @ref HAL_SPDIFRX_CX_HALF_CB_ID    SPDIFRX Control flow half completed callback ID
  *          @arg @ref HAL_SPDIFRX_CX_CPLT_CB_ID    SPDIFRX Control flow completed callback ID
  *          @arg @ref HAL_SPDIFRX_ERROR_CB_ID      SPDIFRX error callback ID
  *          @arg @ref HAL_SPDIFRX_MSPINIT_CB_ID    MspInit callback ID
  *          @arg @ref HAL_SPDIFRX_MSPDEINIT_CB_ID  MspDeInit callback ID
  * @param  pCallback pointer to the Callback function
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPDIFRX_RegisterCallback(SPDIFRX_HandleTypeDef *hspdif, HAL_SPDIFRX_CallbackIDTypeDef CallbackID, pSPDIFRX_CallbackTypeDef pCallback)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  if(pCallback == NULL)                                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hspdif->ErrorCode |= HAL_SPDIFRX_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  /* Process locked */
  __HAL_LOCK(hspdif);                                   /* HAL辅助宏/函数 */

  if(HAL_SPDIFRX_STATE_READY == hspdif->State)          /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_SPDIFRX_RX_HALF_CB_ID :                  /* case标签 */
        hspdif->RxHalfCpltCallback = pCallback;         /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_SPDIFRX_RX_CPLT_CB_ID :                  /* case标签 */
        hspdif->RxCpltCallback = pCallback;             /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_SPDIFRX_CX_HALF_CB_ID :                  /* case标签 */
        hspdif->CxHalfCpltCallback = pCallback;         /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_SPDIFRX_CX_CPLT_CB_ID :                  /* case标签 */
        hspdif->CxCpltCallback = pCallback;             /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_SPDIFRX_ERROR_CB_ID :                    /* case标签 */
        hspdif->ErrorCallback = pCallback;              /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_SPDIFRX_MSPINIT_CB_ID :                  /* case标签 */
        hspdif->MspInitCallback = pCallback;            /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_SPDIFRX_MSPDEINIT_CB_ID :                /* case标签 */
        hspdif->MspDeInitCallback = pCallback;          /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hspdif->ErrorCode |= HAL_SPDIFRX_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if(HAL_SPDIFRX_STATE_RESET == hspdif->State)     /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_SPDIFRX_MSPINIT_CB_ID :                  /* case标签 */
        hspdif->MspInitCallback = pCallback;            /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_SPDIFRX_MSPDEINIT_CB_ID :                /* case标签 */
        hspdif->MspDeInitCallback = pCallback;          /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hspdif->ErrorCode |= HAL_SPDIFRX_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
       /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hspdif->ErrorCode |= HAL_SPDIFRX_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hspdif);                                 /* HAL辅助宏/函数 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Unregister a SPDIFRX Callback
  *         SPDIFRX callabck is redirected to the weak predefined callback
  * @param  hspdif SPDIFRX handle
  * @param  CallbackID ID of the callback to be unregistered
  *         This parameter can be one of the following values:
  *          @arg @ref HAL_SPDIFRX_RX_HALF_CB_ID    SPDIFRX Data flow half completed callback ID
  *          @arg @ref HAL_SPDIFRX_RX_CPLT_CB_ID    SPDIFRX Data flow completed callback ID
  *          @arg @ref HAL_SPDIFRX_CX_HALF_CB_ID    SPDIFRX Control flow half completed callback ID
  *          @arg @ref HAL_SPDIFRX_CX_CPLT_CB_ID    SPDIFRX Control flow completed callback ID
  *          @arg @ref HAL_SPDIFRX_ERROR_CB_ID      SPDIFRX error callback ID
  *          @arg @ref HAL_SPDIFRX_MSPINIT_CB_ID    MspInit callback ID
  *          @arg @ref HAL_SPDIFRX_MSPDEINIT_CB_ID  MspDeInit callback ID
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPDIFRX_UnRegisterCallback(SPDIFRX_HandleTypeDef *hspdif, HAL_SPDIFRX_CallbackIDTypeDef CallbackID)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
HAL_StatusTypeDef status = HAL_OK;                      /* HAL状态类型函数 */

  /* Process locked */
  __HAL_LOCK(hspdif);                                   /* HAL辅助宏/函数 */

  if(HAL_SPDIFRX_STATE_READY == hspdif->State)          /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_SPDIFRX_RX_HALF_CB_ID :                  /* case标签 */
        hspdif->RxHalfCpltCallback = HAL_SPDIFRX_RxHalfCpltCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_SPDIFRX_RX_CPLT_CB_ID :                  /* case标签 */
        hspdif->RxCpltCallback = HAL_SPDIFRX_RxCpltCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_SPDIFRX_CX_HALF_CB_ID :                  /* case标签 */
        hspdif->CxHalfCpltCallback = HAL_SPDIFRX_CxHalfCpltCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_SPDIFRX_CX_CPLT_CB_ID :                  /* case标签 */
        hspdif->CxCpltCallback = HAL_SPDIFRX_CxCpltCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_SPDIFRX_ERROR_CB_ID :                    /* case标签 */
        hspdif->ErrorCallback = HAL_SPDIFRX_ErrorCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hspdif->ErrorCode |= HAL_SPDIFRX_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
       /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if(HAL_SPDIFRX_STATE_RESET == hspdif->State)     /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_SPDIFRX_MSPINIT_CB_ID :                  /* case标签 */
        hspdif->MspInitCallback = HAL_SPDIFRX_MspInit;  /* Legacy weak MspInit  */
        break;                                          /* 跳出 */

      case HAL_SPDIFRX_MSPDEINIT_CB_ID :                /* case标签 */
        hspdif->MspDeInitCallback = HAL_SPDIFRX_MspDeInit;  /* Legacy weak MspInit  */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hspdif->ErrorCode |= HAL_SPDIFRX_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hspdif->ErrorCode |= HAL_SPDIFRX_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hspdif);                                 /* HAL辅助宏/函数 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

#endif /* USE_HAL_SPDIFRX_REGISTER_CALLBACKS */

/**
  * @brief Set the SPDIFRX  data format according to the specified parameters in the SPDIFRX_InitTypeDef.
  * @param hspdif SPDIFRX handle
  * @param sDataFormat SPDIFRX data format
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPDIFRX_SetDataFormat(SPDIFRX_HandleTypeDef *hspdif, SPDIFRX_SetDataFormatTypeDef sDataFormat)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpreg;                                      /* 32位无符号变量/函数 */

  /* Check the SPDIFRX handle allocation */
  if(hspdif == NULL)                                    /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the SPDIFRX parameters */
  assert_param(IS_STEREO_MODE(sDataFormat.StereoMode));  /* 执行语句 */
  assert_param(IS_SPDIFRX_DATA_FORMAT(sDataFormat.DataFormat));  /* 执行语句 */
  assert_param(IS_PREAMBLE_TYPE_MASK(sDataFormat.PreambleTypeMask));  /* 执行语句 */
  assert_param(IS_CHANNEL_STATUS_MASK(sDataFormat.ChannelStatusMask));  /* 执行语句 */
  assert_param(IS_VALIDITY_MASK(sDataFormat.ValidityBitMask));  /* 执行语句 */
  assert_param(IS_PARITY_ERROR_MASK(sDataFormat.ParityErrorMask));  /* 执行语句 */

  /* Reset the old SPDIFRX CR configuration */
  tmpreg = hspdif->Instance->CR;                        /* 赋值或初始化 */

  if(((tmpreg & SPDIFRX_STATE_RCV) == SPDIFRX_STATE_RCV) &&  /* 条件判断 */
     (((tmpreg & SPDIFRX_CR_DRFMT) != sDataFormat.DataFormat) ||  /* 源代码 */
      ((tmpreg & SPDIFRX_CR_RXSTEO) != sDataFormat.StereoMode)))  /* 源代码 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  tmpreg &= ~(SPDIFRX_CR_RXSTEO  | SPDIFRX_CR_DRFMT  | SPDIFRX_CR_PMSK |  /* 源代码 */
              SPDIFRX_CR_VMSK | SPDIFRX_CR_CUMSK | SPDIFRX_CR_PTMSK);  /* 执行语句 */

  /* Configure the new data format */
  tmpreg |= (sDataFormat.StereoMode |                   /* 源代码 */
             sDataFormat.DataFormat |                   /* 源代码 */
             sDataFormat.PreambleTypeMask |             /* 源代码 */
             sDataFormat.ChannelStatusMask |            /* 源代码 */
             sDataFormat.ValidityBitMask |              /* 源代码 */
             sDataFormat.ParityErrorMask);              /* 执行语句 */

  hspdif->Instance->CR = tmpreg;                        /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup SPDIFRX_Exported_Functions_Group2 IO operation functions
  *  @brief Data transfers functions
  *
@verbatim
===============================================================================
                     ##### IO operation functions #####
===============================================================================
    [..]
    This subsection provides a set of functions allowing to manage the SPDIFRX data
    transfers.

    (#) There is two mode of transfer:
        (++) Blocking mode : The communication is performed in the polling mode.
             The status of all data processing is returned by the same function
             after finishing transfer.
        (++) No-Blocking mode : The communication is performed using Interrupts
             or DMA. These functions return the status of the transfer start-up.
             The end of the data processing will be indicated through the
             dedicated SPDIFRX IRQ when using Interrupt mode or the DMA IRQ when
             using DMA mode.

    (#) Blocking mode functions are :
        (++) HAL_SPDIFRX_ReceiveDataFlow()
        (++) HAL_SPDIFRX_ReceiveControlFlow()
                (+@) Do not use blocking mode to receive both control and data flow at the same time.

    (#) No-Blocking mode functions with Interrupt are :
        (++) HAL_SPDIFRX_ReceiveControlFlow_IT()
        (++) HAL_SPDIFRX_ReceiveDataFlow_IT()

    (#) No-Blocking mode functions with DMA are :
        (++) HAL_SPDIFRX_ReceiveControlFlow_DMA()
        (++) HAL_SPDIFRX_ReceiveDataFlow_DMA()

    (#) A set of Transfer Complete Callbacks are provided in No_Blocking mode:
        (++) HAL_SPDIFRX_RxCpltCallback()
        (++) HAL_SPDIFRX_CxCpltCallback()

@endverbatim
* @{
*/

/**
  * @brief  Receives an amount of data (Data Flow) in blocking mode.
  * @param  hspdif pointer to SPDIFRX_HandleTypeDef structure that contains
  *                 the configuration information for SPDIFRX module.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be received
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPDIFRX_ReceiveDataFlow(SPDIFRX_HandleTypeDef *hspdif, uint32_t *pData, uint16_t Size, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */
  uint16_t sizeCounter = Size;                          /* 16位无符号变量/函数 */
  uint32_t *pTmpBuf = pData;                            /* 32位无符号变量/函数 */

  if((pData == NULL ) || (Size == 0U))                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return  HAL_ERROR;                                  /* 返回值 */
  }                                                     /* 代码块结束 */

  if(hspdif->State == HAL_SPDIFRX_STATE_READY)          /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Process Locked */
    __HAL_LOCK(hspdif);                                 /* HAL辅助宏/函数 */

    hspdif->State = HAL_SPDIFRX_STATE_BUSY;             /* 赋值或初始化 */

    /* Start synchronisation */
    __HAL_SPDIFRX_SYNC(hspdif);                         /* HAL辅助宏/函数 */

    /* Get tick */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */

    /* Wait until SYNCD flag is set */
    if(SPDIFRX_WaitOnFlagUntilTimeout(hspdif, SPDIFRX_FLAG_SYNCD, RESET, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_TIMEOUT;                               /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Start reception */
    __HAL_SPDIFRX_RCV(hspdif);                          /* HAL辅助宏/函数 */

    /* Receive data flow */
    while(sizeCounter > 0U)                             /* while循环 */
    {                                                   /* 代码块开始 */
      /* Get tick */
      tickstart = HAL_GetTick();                        /* 赋值或初始化 */

      /* Wait until RXNE flag is set */
      if(SPDIFRX_WaitOnFlagUntilTimeout(hspdif, SPDIFRX_FLAG_RXNE, RESET, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */

      (*pTmpBuf) = hspdif->Instance->DR;                /* 赋值或初始化 */
      pTmpBuf++;                                        /* 执行语句 */
      sizeCounter--;                                    /* 执行语句 */
    }                                                   /* 代码块结束 */

    /* SPDIFRX ready */
    hspdif->State = HAL_SPDIFRX_STATE_READY;            /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hspdif);                               /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Receives an amount of data (Control Flow) in blocking mode.
  * @param  hspdif pointer to a SPDIFRX_HandleTypeDef structure that contains
  *                 the configuration information for SPDIFRX module.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be received
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPDIFRX_ReceiveControlFlow(SPDIFRX_HandleTypeDef *hspdif, uint32_t *pData, uint16_t Size, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */
  uint16_t sizeCounter = Size;                          /* 16位无符号变量/函数 */
  uint32_t *pTmpBuf = pData;                            /* 32位无符号变量/函数 */

  if((pData == NULL ) || (Size == 0U))                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return  HAL_ERROR;                                  /* 返回值 */
  }                                                     /* 代码块结束 */

  if(hspdif->State == HAL_SPDIFRX_STATE_READY)          /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Process Locked */
    __HAL_LOCK(hspdif);                                 /* HAL辅助宏/函数 */

    hspdif->State = HAL_SPDIFRX_STATE_BUSY;             /* 赋值或初始化 */

    /* Start synchronization */
    __HAL_SPDIFRX_SYNC(hspdif);                         /* HAL辅助宏/函数 */

    /* Get tick */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */

    /* Wait until SYNCD flag is set */
    if(SPDIFRX_WaitOnFlagUntilTimeout(hspdif, SPDIFRX_FLAG_SYNCD, RESET, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_TIMEOUT;                               /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Start reception */
    __HAL_SPDIFRX_RCV(hspdif);                          /* HAL辅助宏/函数 */

    /* Receive control flow */
    while(sizeCounter > 0U)                             /* while循环 */
    {                                                   /* 代码块开始 */
      /* Get tick */
      tickstart = HAL_GetTick();                        /* 赋值或初始化 */

      /* Wait until CSRNE flag is set */
      if(SPDIFRX_WaitOnFlagUntilTimeout(hspdif, SPDIFRX_FLAG_CSRNE, RESET, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */

      (*pTmpBuf) = hspdif->Instance->CSR;               /* 赋值或初始化 */
      pTmpBuf++;                                        /* 执行语句 */
      sizeCounter--;                                    /* 执行语句 */
    }                                                   /* 代码块结束 */

    /* SPDIFRX ready */
    hspdif->State = HAL_SPDIFRX_STATE_READY;            /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hspdif);                               /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief Receive an amount of data (Data Flow) in non-blocking mode with Interrupt
  * @param hspdif SPDIFRX handle
  * @param pData a 32-bit pointer to the Receive data buffer.
  * @param Size number of data sample to be received .
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPDIFRX_ReceiveDataFlow_IT(SPDIFRX_HandleTypeDef *hspdif, uint32_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t count = SPDIFRX_TIMEOUT_VALUE * (SystemCoreClock / 24U / 1000U);  /* 32位无符号变量/函数 */

  const HAL_SPDIFRX_StateTypeDef tempState = hspdif->State;  /* 常量定义 */

  if((tempState == HAL_SPDIFRX_STATE_READY) || (tempState == HAL_SPDIFRX_STATE_BUSY_CX))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if((pData == NULL) || (Size == 0U))                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hspdif);                                 /* HAL辅助宏/函数 */

    hspdif->pRxBuffPtr = pData;                         /* 赋值或初始化 */
    hspdif->RxXferSize = Size;                          /* 赋值或初始化 */
    hspdif->RxXferCount = Size;                         /* 赋值或初始化 */

    hspdif->ErrorCode = HAL_SPDIFRX_ERROR_NONE;         /* 赋值或初始化 */

    /* Check if a receive process is ongoing or not */
    hspdif->State = HAL_SPDIFRX_STATE_BUSY_RX;          /* 赋值或初始化 */

    /* Enable the SPDIFRX  PE Error Interrupt */
    __HAL_SPDIFRX_ENABLE_IT(hspdif, SPDIFRX_IT_PERRIE);  /* HAL辅助宏/函数 */

    /* Enable the SPDIFRX  OVR Error Interrupt */
    __HAL_SPDIFRX_ENABLE_IT(hspdif, SPDIFRX_IT_OVRIE);  /* HAL辅助宏/函数 */

    /* Enable the SPDIFRX RXNE interrupt */
    __HAL_SPDIFRX_ENABLE_IT(hspdif, SPDIFRX_IT_RXNE);   /* HAL辅助宏/函数 */

    if((SPDIFRX->CR & SPDIFRX_CR_SPDIFEN) != SPDIFRX_STATE_RCV)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Start synchronization */
      __HAL_SPDIFRX_SYNC(hspdif);                       /* HAL辅助宏/函数 */

      /* Wait until SYNCD flag is set */
      do                                                /* do-while循环 */
      {                                                 /* 代码块开始 */
        if (count == 0U)                                /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Disable TXE, RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts for the interrupt process */
          __HAL_SPDIFRX_DISABLE_IT(hspdif, SPDIFRX_IT_RXNE);  /* HAL辅助宏/函数 */
          __HAL_SPDIFRX_DISABLE_IT(hspdif, SPDIFRX_IT_CSRNE);  /* HAL辅助宏/函数 */
          __HAL_SPDIFRX_DISABLE_IT(hspdif, SPDIFRX_IT_PERRIE);  /* HAL辅助宏/函数 */
          __HAL_SPDIFRX_DISABLE_IT(hspdif, SPDIFRX_IT_OVRIE);  /* HAL辅助宏/函数 */
          __HAL_SPDIFRX_DISABLE_IT(hspdif, SPDIFRX_IT_SBLKIE);  /* HAL辅助宏/函数 */
          __HAL_SPDIFRX_DISABLE_IT(hspdif, SPDIFRX_IT_SYNCDIE);  /* HAL辅助宏/函数 */
          __HAL_SPDIFRX_DISABLE_IT(hspdif, SPDIFRX_IT_IFEIE);  /* HAL辅助宏/函数 */

          hspdif->State= HAL_SPDIFRX_STATE_READY;       /* 赋值或初始化 */

          /* Process Unlocked */
          __HAL_UNLOCK(hspdif);                         /* HAL辅助宏/函数 */

          return HAL_TIMEOUT;                           /* 返回值 */
        }                                               /* 代码块结束 */
        count--;                                        /* 执行语句 */
      } while (__HAL_SPDIFRX_GET_FLAG(hspdif, SPDIFRX_FLAG_SYNCD) == RESET);  /* 代码块结束 */

      /* Start reception */
      __HAL_SPDIFRX_RCV(hspdif);                        /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Process Unlocked */
    __HAL_UNLOCK(hspdif);                               /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief Receive an amount of data (Control Flow) with Interrupt
  * @param hspdif SPDIFRX handle
  * @param pData a 32-bit pointer to the Receive data buffer.
  * @param Size number of data sample (Control Flow) to be received
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPDIFRX_ReceiveControlFlow_IT(SPDIFRX_HandleTypeDef *hspdif, uint32_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t count = SPDIFRX_TIMEOUT_VALUE * (SystemCoreClock / 24U / 1000U);  /* 32位无符号变量/函数 */

  const HAL_SPDIFRX_StateTypeDef tempState = hspdif->State;  /* 常量定义 */

  if((tempState == HAL_SPDIFRX_STATE_READY) || (tempState == HAL_SPDIFRX_STATE_BUSY_RX))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if((pData == NULL ) || (Size == 0U))                /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hspdif);                                 /* HAL辅助宏/函数 */

    hspdif->pCsBuffPtr = pData;                         /* 赋值或初始化 */
    hspdif->CsXferSize = Size;                          /* 赋值或初始化 */
    hspdif->CsXferCount = Size;                         /* 赋值或初始化 */

    hspdif->ErrorCode = HAL_SPDIFRX_ERROR_NONE;         /* 赋值或初始化 */

    /* Check if a receive process is ongoing or not */
    hspdif->State = HAL_SPDIFRX_STATE_BUSY_CX;          /* 赋值或初始化 */

    /* Enable the SPDIFRX PE Error Interrupt */
    __HAL_SPDIFRX_ENABLE_IT(hspdif, SPDIFRX_IT_PERRIE);  /* HAL辅助宏/函数 */

    /* Enable the SPDIFRX OVR Error Interrupt */
    __HAL_SPDIFRX_ENABLE_IT(hspdif, SPDIFRX_IT_OVRIE);  /* HAL辅助宏/函数 */

    /* Enable the SPDIFRX CSRNE interrupt */
    __HAL_SPDIFRX_ENABLE_IT(hspdif, SPDIFRX_IT_CSRNE);  /* HAL辅助宏/函数 */

    if((SPDIFRX->CR & SPDIFRX_CR_SPDIFEN) != SPDIFRX_STATE_RCV)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Start synchronization */
      __HAL_SPDIFRX_SYNC(hspdif);                       /* HAL辅助宏/函数 */

      /* Wait until SYNCD flag is set */
      do                                                /* do-while循环 */
      {                                                 /* 代码块开始 */
        if (count == 0U)                                /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Disable TXE, RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts for the interrupt process */
          __HAL_SPDIFRX_DISABLE_IT(hspdif, SPDIFRX_IT_RXNE);  /* HAL辅助宏/函数 */
          __HAL_SPDIFRX_DISABLE_IT(hspdif, SPDIFRX_IT_CSRNE);  /* HAL辅助宏/函数 */
          __HAL_SPDIFRX_DISABLE_IT(hspdif, SPDIFRX_IT_PERRIE);  /* HAL辅助宏/函数 */
          __HAL_SPDIFRX_DISABLE_IT(hspdif, SPDIFRX_IT_OVRIE);  /* HAL辅助宏/函数 */
          __HAL_SPDIFRX_DISABLE_IT(hspdif, SPDIFRX_IT_SBLKIE);  /* HAL辅助宏/函数 */
          __HAL_SPDIFRX_DISABLE_IT(hspdif, SPDIFRX_IT_SYNCDIE);  /* HAL辅助宏/函数 */
          __HAL_SPDIFRX_DISABLE_IT(hspdif, SPDIFRX_IT_IFEIE);  /* HAL辅助宏/函数 */

          hspdif->State= HAL_SPDIFRX_STATE_READY;       /* 赋值或初始化 */

          /* Process Unlocked */
          __HAL_UNLOCK(hspdif);                         /* HAL辅助宏/函数 */

          return HAL_TIMEOUT;                           /* 返回值 */
        }                                               /* 代码块结束 */
        count--;                                        /* 执行语句 */
      } while (__HAL_SPDIFRX_GET_FLAG(hspdif, SPDIFRX_FLAG_SYNCD) == RESET);  /* 代码块结束 */

      /* Start reception */
      __HAL_SPDIFRX_RCV(hspdif);                        /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Process Unlocked */
    __HAL_UNLOCK(hspdif);                               /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief Receive an amount of data (Data Flow) mode with DMA
  * @param hspdif SPDIFRX handle
  * @param pData a 32-bit pointer to the Receive data buffer.
  * @param Size number of data sample to be received
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPDIFRX_ReceiveDataFlow_DMA(SPDIFRX_HandleTypeDef *hspdif, uint32_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t count = SPDIFRX_TIMEOUT_VALUE * (SystemCoreClock / 24U / 1000U);  /* 32位无符号变量/函数 */

  const HAL_SPDIFRX_StateTypeDef tempState = hspdif->State;  /* 常量定义 */

  if((pData == NULL) || (Size == 0U))                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    return  HAL_ERROR;                                  /* 返回值 */
  }                                                     /* 代码块结束 */

  if((tempState == HAL_SPDIFRX_STATE_READY) || (tempState == HAL_SPDIFRX_STATE_BUSY_CX))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Process Locked */
    __HAL_LOCK(hspdif);                                 /* HAL辅助宏/函数 */

    hspdif->pRxBuffPtr = pData;                         /* 赋值或初始化 */
    hspdif->RxXferSize = Size;                          /* 赋值或初始化 */
    hspdif->RxXferCount = Size;                         /* 赋值或初始化 */

    hspdif->ErrorCode = HAL_SPDIFRX_ERROR_NONE;         /* 赋值或初始化 */
    hspdif->State = HAL_SPDIFRX_STATE_BUSY_RX;          /* 赋值或初始化 */

    /* Set the SPDIFRX Rx DMA Half transfer complete callback */
    hspdif->hdmaDrRx->XferHalfCpltCallback = SPDIFRX_DMARxHalfCplt;  /* 赋值或初始化 */

    /* Set the SPDIFRX Rx DMA transfer complete callback */
    hspdif->hdmaDrRx->XferCpltCallback = SPDIFRX_DMARxCplt;  /* 赋值或初始化 */

    /* Set the DMA error callback */
    hspdif->hdmaDrRx->XferErrorCallback = SPDIFRX_DMAError;  /* 赋值或初始化 */

    /* Enable the DMA request */
    if(HAL_DMA_Start_IT(hspdif->hdmaDrRx, (uint32_t)&hspdif->Instance->DR, (uint32_t)hspdif->pRxBuffPtr, Size) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set SPDIFRX error */
      hspdif->ErrorCode = HAL_SPDIFRX_ERROR_DMA;        /* 赋值或初始化 */

      /* Set SPDIFRX state */
      hspdif->State = HAL_SPDIFRX_STATE_ERROR;          /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hspdif);                             /* HAL辅助宏/函数 */

      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Enable RXDMAEN bit in SPDIFRX CR register for data flow reception*/
    hspdif->Instance->CR |= SPDIFRX_CR_RXDMAEN;         /* 赋值或初始化 */

    if((SPDIFRX->CR & SPDIFRX_CR_SPDIFEN) != SPDIFRX_STATE_RCV)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Start synchronization */
      __HAL_SPDIFRX_SYNC(hspdif);                       /* HAL辅助宏/函数 */

      /* Wait until SYNCD flag is set */
      do                                                /* do-while循环 */
      {                                                 /* 代码块开始 */
        if (count == 0U)                                /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Disable TXE, RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts for the interrupt process */
          __HAL_SPDIFRX_DISABLE_IT(hspdif, SPDIFRX_IT_RXNE);  /* HAL辅助宏/函数 */
          __HAL_SPDIFRX_DISABLE_IT(hspdif, SPDIFRX_IT_CSRNE);  /* HAL辅助宏/函数 */
          __HAL_SPDIFRX_DISABLE_IT(hspdif, SPDIFRX_IT_PERRIE);  /* HAL辅助宏/函数 */
          __HAL_SPDIFRX_DISABLE_IT(hspdif, SPDIFRX_IT_OVRIE);  /* HAL辅助宏/函数 */
          __HAL_SPDIFRX_DISABLE_IT(hspdif, SPDIFRX_IT_SBLKIE);  /* HAL辅助宏/函数 */
          __HAL_SPDIFRX_DISABLE_IT(hspdif, SPDIFRX_IT_SYNCDIE);  /* HAL辅助宏/函数 */
          __HAL_SPDIFRX_DISABLE_IT(hspdif, SPDIFRX_IT_IFEIE);  /* HAL辅助宏/函数 */

          hspdif->State= HAL_SPDIFRX_STATE_READY;       /* 赋值或初始化 */

          /* Process Unlocked */
          __HAL_UNLOCK(hspdif);                         /* HAL辅助宏/函数 */

          return HAL_TIMEOUT;                           /* 返回值 */
        }                                               /* 代码块结束 */
        count--;                                        /* 执行语句 */
      } while (__HAL_SPDIFRX_GET_FLAG(hspdif, SPDIFRX_FLAG_SYNCD) == RESET);  /* 代码块结束 */

      /* Start reception */
      __HAL_SPDIFRX_RCV(hspdif);                        /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Process Unlocked */
    __HAL_UNLOCK(hspdif);                               /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief Receive an amount of data (Control Flow) with DMA
  * @param hspdif SPDIFRX handle
  * @param pData a 32-bit pointer to the Receive data buffer.
  * @param Size number of data (Control Flow) sample to be received
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPDIFRX_ReceiveControlFlow_DMA(SPDIFRX_HandleTypeDef *hspdif, uint32_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t count = SPDIFRX_TIMEOUT_VALUE * (SystemCoreClock / 24U / 1000U);  /* 32位无符号变量/函数 */

  const HAL_SPDIFRX_StateTypeDef tempState = hspdif->State;  /* 常量定义 */

  if((pData == NULL) || (Size == 0U))                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    return  HAL_ERROR;                                  /* 返回值 */
  }                                                     /* 代码块结束 */

  if((tempState == HAL_SPDIFRX_STATE_READY) || (tempState == HAL_SPDIFRX_STATE_BUSY_RX))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hspdif->pCsBuffPtr = pData;                         /* 赋值或初始化 */
    hspdif->CsXferSize = Size;                          /* 赋值或初始化 */
    hspdif->CsXferCount = Size;                         /* 赋值或初始化 */

    /* Process Locked */
    __HAL_LOCK(hspdif);                                 /* HAL辅助宏/函数 */

    hspdif->ErrorCode = HAL_SPDIFRX_ERROR_NONE;         /* 赋值或初始化 */
    hspdif->State = HAL_SPDIFRX_STATE_BUSY_CX;          /* 赋值或初始化 */

    /* Set the SPDIFRX Rx DMA Half transfer complete callback */
    hspdif->hdmaCsRx->XferHalfCpltCallback = SPDIFRX_DMACxHalfCplt;  /* 赋值或初始化 */

    /* Set the SPDIFRX Rx DMA transfer complete callback */
    hspdif->hdmaCsRx->XferCpltCallback = SPDIFRX_DMACxCplt;  /* 赋值或初始化 */

    /* Set the DMA error callback */
    hspdif->hdmaCsRx->XferErrorCallback = SPDIFRX_DMAError;  /* 赋值或初始化 */

    /* Enable the DMA request */
    if(HAL_DMA_Start_IT(hspdif->hdmaCsRx, (uint32_t)&hspdif->Instance->CSR, (uint32_t)hspdif->pCsBuffPtr, Size) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set SPDIFRX error */
      hspdif->ErrorCode = HAL_SPDIFRX_ERROR_DMA;        /* 赋值或初始化 */

      /* Set SPDIFRX state */
      hspdif->State = HAL_SPDIFRX_STATE_ERROR;          /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hspdif);                             /* HAL辅助宏/函数 */

      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Enable CBDMAEN bit in SPDIFRX CR register for control flow reception*/
    hspdif->Instance->CR |= SPDIFRX_CR_CBDMAEN;         /* 赋值或初始化 */

    if((SPDIFRX->CR & SPDIFRX_CR_SPDIFEN) != SPDIFRX_STATE_RCV)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Start synchronization */
      __HAL_SPDIFRX_SYNC(hspdif);                       /* HAL辅助宏/函数 */

      /* Wait until SYNCD flag is set */
      do                                                /* do-while循环 */
      {                                                 /* 代码块开始 */
        if (count == 0U)                                /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Disable TXE, RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts for the interrupt process */
          __HAL_SPDIFRX_DISABLE_IT(hspdif, SPDIFRX_IT_RXNE);  /* HAL辅助宏/函数 */
          __HAL_SPDIFRX_DISABLE_IT(hspdif, SPDIFRX_IT_CSRNE);  /* HAL辅助宏/函数 */
          __HAL_SPDIFRX_DISABLE_IT(hspdif, SPDIFRX_IT_PERRIE);  /* HAL辅助宏/函数 */
          __HAL_SPDIFRX_DISABLE_IT(hspdif, SPDIFRX_IT_OVRIE);  /* HAL辅助宏/函数 */
          __HAL_SPDIFRX_DISABLE_IT(hspdif, SPDIFRX_IT_SBLKIE);  /* HAL辅助宏/函数 */
          __HAL_SPDIFRX_DISABLE_IT(hspdif, SPDIFRX_IT_SYNCDIE);  /* HAL辅助宏/函数 */
          __HAL_SPDIFRX_DISABLE_IT(hspdif, SPDIFRX_IT_IFEIE);  /* HAL辅助宏/函数 */

          hspdif->State= HAL_SPDIFRX_STATE_READY;       /* 赋值或初始化 */

          /* Process Unlocked */
          __HAL_UNLOCK(hspdif);                         /* HAL辅助宏/函数 */

          return HAL_TIMEOUT;                           /* 返回值 */
        }                                               /* 代码块结束 */
        count--;                                        /* 执行语句 */
      } while (__HAL_SPDIFRX_GET_FLAG(hspdif, SPDIFRX_FLAG_SYNCD) == RESET);  /* 代码块结束 */

      /* Start reception */
      __HAL_SPDIFRX_RCV(hspdif);                        /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Process Unlocked */
    __HAL_UNLOCK(hspdif);                               /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief stop the audio stream receive from the Media.
  * @param hspdif SPDIFRX handle
  * @retval None
  */
HAL_StatusTypeDef HAL_SPDIFRX_DMAStop(SPDIFRX_HandleTypeDef *hspdif)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Process Locked */
  __HAL_LOCK(hspdif);                                   /* HAL辅助宏/函数 */

  /* Disable the SPDIFRX DMA requests */
  hspdif->Instance->CR &= (uint16_t)(~SPDIFRX_CR_RXDMAEN);  /* 赋值或初始化 */
  hspdif->Instance->CR &= (uint16_t)(~SPDIFRX_CR_CBDMAEN);  /* 赋值或初始化 */

  /* Disable the SPDIFRX DMA channel */
  __HAL_DMA_DISABLE(hspdif->hdmaDrRx);                  /* HAL辅助宏/函数 */
  __HAL_DMA_DISABLE(hspdif->hdmaCsRx);                  /* HAL辅助宏/函数 */

  /* Disable SPDIFRX peripheral */
  __HAL_SPDIFRX_IDLE(hspdif);                           /* HAL辅助宏/函数 */

  hspdif->State = HAL_SPDIFRX_STATE_READY;              /* 赋值或初始化 */

  /* Process Unlocked */
  __HAL_UNLOCK(hspdif);                                 /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  This function handles SPDIFRX interrupt request.
  * @param  hspdif SPDIFRX handle
  * @retval HAL status
  */
void HAL_SPDIFRX_IRQHandler(SPDIFRX_HandleTypeDef *hspdif)  /* 中断服务函数 */
{                                                       /* 代码块开始 */
  uint32_t itFlag   = hspdif->Instance->SR;             /* 32位无符号变量/函数 */
  uint32_t itSource = hspdif->Instance->IMR;            /* 32位无符号变量/函数 */

  /* SPDIFRX in mode Data Flow Reception */
  if(((itFlag & SPDIFRX_FLAG_RXNE) == SPDIFRX_FLAG_RXNE) && ((itSource &  SPDIFRX_IT_RXNE) == SPDIFRX_IT_RXNE))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    __HAL_SPDIFRX_CLEAR_IT(hspdif, SPDIFRX_IT_RXNE);    /* HAL辅助宏/函数 */
    SPDIFRX_ReceiveDataFlow_IT(hspdif);                 /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* SPDIFRX in mode Control Flow Reception */
  if(((itFlag & SPDIFRX_FLAG_CSRNE) == SPDIFRX_FLAG_CSRNE) && ((itSource &  SPDIFRX_IT_CSRNE) == SPDIFRX_IT_CSRNE))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    __HAL_SPDIFRX_CLEAR_IT(hspdif, SPDIFRX_IT_CSRNE);   /* HAL辅助宏/函数 */
    SPDIFRX_ReceiveControlFlow_IT(hspdif);              /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* SPDIFRX Overrun error interrupt occurred */
  if(((itFlag & SPDIFRX_FLAG_OVR) == SPDIFRX_FLAG_OVR) && ((itSource &  SPDIFRX_IT_OVRIE) == SPDIFRX_IT_OVRIE))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    __HAL_SPDIFRX_CLEAR_IT(hspdif, SPDIFRX_IT_OVRIE);   /* HAL辅助宏/函数 */

    /* Change the SPDIFRX error code */
    hspdif->ErrorCode |= HAL_SPDIFRX_ERROR_OVR;         /* 赋值或初始化 */

    /* the transfer is not stopped */
    HAL_SPDIFRX_ErrorCallback(hspdif);                  /* HAL库函数调用 */
  }                                                     /* 代码块结束 */

  /* SPDIFRX Parity error interrupt occurred */
  if(((itFlag & SPDIFRX_FLAG_PERR) == SPDIFRX_FLAG_PERR) && ((itSource &  SPDIFRX_IT_PERRIE) == SPDIFRX_IT_PERRIE))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    __HAL_SPDIFRX_CLEAR_IT(hspdif, SPDIFRX_IT_PERRIE);  /* HAL辅助宏/函数 */

    /* Change the SPDIFRX error code */
    hspdif->ErrorCode |= HAL_SPDIFRX_ERROR_PE;          /* 赋值或初始化 */

    /* the transfer is not stopped */
    HAL_SPDIFRX_ErrorCallback(hspdif);                  /* HAL库函数调用 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief Rx Transfer (Data flow) half completed callbacks
  * @param hspdif SPDIFRX handle
  * @retval None
  */
__weak void HAL_SPDIFRX_RxHalfCpltCallback(SPDIFRX_HandleTypeDef *hspdif)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hspdif);                                       /* 执行语句 */

  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_SPDIFRX_RxCpltCallback could be implemented in the user file
  */
}                                                       /* 代码块结束 */

/**
  * @brief Rx Transfer (Data flow) completed callbacks
  * @param hspdif SPDIFRX handle
  * @retval None
  */
__weak void HAL_SPDIFRX_RxCpltCallback(SPDIFRX_HandleTypeDef *hspdif)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hspdif);                                       /* 执行语句 */

  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_SPDIFRX_RxCpltCallback could be implemented in the user file
  */
}                                                       /* 代码块结束 */

/**
  * @brief Rx (Control flow) Transfer half completed callbacks
  * @param hspdif SPDIFRX handle
  * @retval None
  */
__weak void HAL_SPDIFRX_CxHalfCpltCallback(SPDIFRX_HandleTypeDef *hspdif)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hspdif);                                       /* 执行语句 */

  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_SPDIFRX_RxCpltCallback could be implemented in the user file
  */
}                                                       /* 代码块结束 */

/**
  * @brief Rx Transfer (Control flow) completed callbacks
  * @param hspdif SPDIFRX handle
  * @retval None
  */
__weak void HAL_SPDIFRX_CxCpltCallback(SPDIFRX_HandleTypeDef *hspdif)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hspdif);                                       /* 执行语句 */

  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_SPDIFRX_RxCpltCallback could be implemented in the user file
  */
}                                                       /* 代码块结束 */

/**
  * @brief SPDIFRX error callbacks
  * @param hspdif SPDIFRX handle
  * @retval None
  */
__weak void HAL_SPDIFRX_ErrorCallback(SPDIFRX_HandleTypeDef *hspdif)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hspdif);                                       /* 执行语句 */

  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_SPDIFRX_ErrorCallback could be implemented in the user file
  */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup SPDIFRX_Exported_Functions_Group3 Peripheral State and Errors functions
  *  @brief   Peripheral State functions
  *
@verbatim
===============================================================================
##### Peripheral State and Errors functions #####
===============================================================================
[..]
This subsection permit to get in run-time the status of the peripheral
and the data flow.

@endverbatim
  * @{
  */

/**
  * @brief  Return the SPDIFRX state
  * @param  hspdif SPDIFRX handle
  * @retval HAL state
  */
HAL_SPDIFRX_StateTypeDef HAL_SPDIFRX_GetState(SPDIFRX_HandleTypeDef const * const hspdif)  /* HAL库函数调用 */
{                                                       /* 代码块开始 */
  return hspdif->State;                                 /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return the SPDIFRX error code
  * @param  hspdif SPDIFRX handle
  * @retval SPDIFRX Error Code
  */
uint32_t HAL_SPDIFRX_GetError(SPDIFRX_HandleTypeDef const * const hspdif)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  return hspdif->ErrorCode;                             /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/**
  * @brief DMA SPDIFRX receive process (Data flow) complete callback
  * @param hdma DMA handle
  * @retval None
  */
static void SPDIFRX_DMARxCplt(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  SPDIFRX_HandleTypeDef* hspdif = ( SPDIFRX_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;  /* 赋值或初始化 */

  /* Disable Rx DMA Request */
  if(hdma->Init.Mode != DMA_CIRCULAR)                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    hspdif->Instance->CR &= (uint16_t)(~SPDIFRX_CR_RXDMAEN);  /* 赋值或初始化 */
    hspdif->RxXferCount = 0;                            /* 赋值或初始化 */
    hspdif->State = HAL_SPDIFRX_STATE_READY;            /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
#if (USE_HAL_SPDIFRX_REGISTER_CALLBACKS == 1)           /* 条件编译 */
  hspdif->RxCpltCallback(hspdif);                       /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_SPDIFRX_RxCpltCallback(hspdif);                   /* HAL库函数调用 */
#endif /* USE_HAL_SPDIFRX_REGISTER_CALLBACKS */
}                                                       /* 代码块结束 */

/**
  * @brief DMA SPDIFRX receive process (Data flow) half complete callback
  * @param hdma DMA handle
  * @retval None
  */
static void SPDIFRX_DMARxHalfCplt(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  SPDIFRX_HandleTypeDef* hspdif = (SPDIFRX_HandleTypeDef*)((DMA_HandleTypeDef*)hdma)->Parent;  /* 赋值或初始化 */

#if (USE_HAL_SPDIFRX_REGISTER_CALLBACKS == 1)           /* 条件编译 */
  hspdif->RxHalfCpltCallback(hspdif);                   /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_SPDIFRX_RxHalfCpltCallback(hspdif);               /* HAL库函数调用 */
#endif /* USE_HAL_SPDIFRX_REGISTER_CALLBACKS */
}                                                       /* 代码块结束 */


/**
  * @brief DMA SPDIFRX receive process (Control flow) complete callback
  * @param hdma DMA handle
  * @retval None
  */
static void SPDIFRX_DMACxCplt(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  SPDIFRX_HandleTypeDef* hspdif = ( SPDIFRX_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;  /* 赋值或初始化 */

  /* Disable Cb DMA Request */
  hspdif->Instance->CR &= (uint16_t)(~SPDIFRX_CR_CBDMAEN);  /* 赋值或初始化 */
  hspdif->CsXferCount = 0;                              /* 赋值或初始化 */

  hspdif->State = HAL_SPDIFRX_STATE_READY;              /* 赋值或初始化 */
#if (USE_HAL_SPDIFRX_REGISTER_CALLBACKS == 1)           /* 条件编译 */
  hspdif->CxCpltCallback(hspdif);                       /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_SPDIFRX_CxCpltCallback(hspdif);                   /* HAL库函数调用 */
#endif /* USE_HAL_SPDIFRX_REGISTER_CALLBACKS */
}                                                       /* 代码块结束 */

/**
  * @brief DMA SPDIFRX receive process (Control flow) half complete callback
  * @param hdma DMA handle
  * @retval None
  */
static void SPDIFRX_DMACxHalfCplt(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  SPDIFRX_HandleTypeDef* hspdif = (SPDIFRX_HandleTypeDef*)((DMA_HandleTypeDef*)hdma)->Parent;  /* 赋值或初始化 */

#if (USE_HAL_SPDIFRX_REGISTER_CALLBACKS == 1)           /* 条件编译 */
  hspdif->CxHalfCpltCallback(hspdif);                   /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_SPDIFRX_CxHalfCpltCallback(hspdif);               /* HAL库函数调用 */
#endif /* USE_HAL_SPDIFRX_REGISTER_CALLBACKS */
}                                                       /* 代码块结束 */

/**
  * @brief DMA SPDIFRX communication error callback
  * @param hdma DMA handle
  * @retval None
  */
static void SPDIFRX_DMAError(DMA_HandleTypeDef *hdma)   /* 静态函数 */
{                                                       /* 代码块开始 */
  SPDIFRX_HandleTypeDef* hspdif = ( SPDIFRX_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;  /* 赋值或初始化 */

  /* Disable Rx and Cb DMA Request */
  hspdif->Instance->CR &= (uint16_t)(~(SPDIFRX_CR_RXDMAEN | SPDIFRX_CR_CBDMAEN));  /* 赋值或初始化 */
  hspdif->RxXferCount = 0;                              /* 赋值或初始化 */

  hspdif->State= HAL_SPDIFRX_STATE_READY;               /* 赋值或初始化 */

  /* Set the error code and execute error callback*/
  hspdif->ErrorCode |= HAL_SPDIFRX_ERROR_DMA;           /* 赋值或初始化 */

#if (USE_HAL_SPDIFRX_REGISTER_CALLBACKS == 1)           /* 条件编译 */
  /* The transfer is not stopped */
  hspdif->ErrorCallback(hspdif);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* The transfer is not stopped */
  HAL_SPDIFRX_ErrorCallback(hspdif);                    /* HAL库函数调用 */
#endif /* USE_HAL_SPDIFRX_REGISTER_CALLBACKS */
}                                                       /* 代码块结束 */

/**
  * @brief Receive an amount of data (Data Flow) with Interrupt
  * @param hspdif SPDIFRX handle
  * @retval None
  */
static void SPDIFRX_ReceiveDataFlow_IT(SPDIFRX_HandleTypeDef *hspdif)  /* 静态函数 */
{                                                       /* 代码块开始 */
  /* Receive data */
  (*hspdif->pRxBuffPtr) = hspdif->Instance->DR;         /* 赋值或初始化 */
  hspdif->pRxBuffPtr++;                                 /* 执行语句 */
  hspdif->RxXferCount--;                                /* 执行语句 */

  if(hspdif->RxXferCount == 0U)                         /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable RXNE/PE and OVR interrupts */
    __HAL_SPDIFRX_DISABLE_IT(hspdif, SPDIFRX_IT_OVRIE | SPDIFRX_IT_PERRIE | SPDIFRX_IT_RXNE);  /* HAL辅助宏/函数 */

    hspdif->State = HAL_SPDIFRX_STATE_READY;            /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hspdif);                               /* HAL辅助宏/函数 */

#if (USE_HAL_SPDIFRX_REGISTER_CALLBACKS == 1)           /* 条件编译 */
  hspdif->RxCpltCallback(hspdif);                       /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_SPDIFRX_RxCpltCallback(hspdif);                   /* HAL库函数调用 */
#endif /* USE_HAL_SPDIFRX_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief Receive an amount of data (Control Flow) with Interrupt
  * @param hspdif SPDIFRX handle
  * @retval None
  */
static void SPDIFRX_ReceiveControlFlow_IT(SPDIFRX_HandleTypeDef *hspdif)  /* 静态函数 */
{                                                       /* 代码块开始 */
  /* Receive data */
  (*hspdif->pCsBuffPtr) = hspdif->Instance->CSR;        /* 赋值或初始化 */
  hspdif->pCsBuffPtr++;                                 /* 执行语句 */
  hspdif->CsXferCount--;                                /* 执行语句 */

  if(hspdif->CsXferCount == 0U)                         /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable CSRNE interrupt */
    __HAL_SPDIFRX_DISABLE_IT(hspdif, SPDIFRX_IT_CSRNE);  /* HAL辅助宏/函数 */

    hspdif->State = HAL_SPDIFRX_STATE_READY;            /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hspdif);                               /* HAL辅助宏/函数 */

#if (USE_HAL_SPDIFRX_REGISTER_CALLBACKS == 1)           /* 条件编译 */
  hspdif->CxCpltCallback(hspdif);                       /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_SPDIFRX_CxCpltCallback(hspdif);                   /* HAL库函数调用 */
#endif /* USE_HAL_SPDIFRX_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief This function handles SPDIFRX Communication Timeout.
  * @param hspdif SPDIFRX handle
  * @param Flag Flag checked
  * @param Status Value of the flag expected
  * @param Timeout Duration of the timeout
  * @param tickstart Tick start value
  * @retval HAL status
  */
static HAL_StatusTypeDef SPDIFRX_WaitOnFlagUntilTimeout(SPDIFRX_HandleTypeDef *hspdif, uint32_t Flag, FlagStatus Status, uint32_t Timeout, uint32_t tickstart)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  /* Wait until flag is set */
  while(__HAL_SPDIFRX_GET_FLAG(hspdif, Flag) == Status)  /* while循环 */
  {                                                     /* 代码块开始 */
    /* Check for the Timeout */
    if(Timeout != HAL_MAX_DELAY)                        /* 条件判断 */
    {                                                   /* 代码块开始 */
      if(((HAL_GetTick() - tickstart ) > Timeout) || (Timeout == 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Disable TXE, RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts for the interrupt process */
        __HAL_SPDIFRX_DISABLE_IT(hspdif, SPDIFRX_IT_RXNE);  /* HAL辅助宏/函数 */
        __HAL_SPDIFRX_DISABLE_IT(hspdif, SPDIFRX_IT_CSRNE);  /* HAL辅助宏/函数 */
        __HAL_SPDIFRX_DISABLE_IT(hspdif, SPDIFRX_IT_PERRIE);  /* HAL辅助宏/函数 */
        __HAL_SPDIFRX_DISABLE_IT(hspdif, SPDIFRX_IT_OVRIE);  /* HAL辅助宏/函数 */
        __HAL_SPDIFRX_DISABLE_IT(hspdif, SPDIFRX_IT_SBLKIE);  /* HAL辅助宏/函数 */
        __HAL_SPDIFRX_DISABLE_IT(hspdif, SPDIFRX_IT_SYNCDIE);  /* HAL辅助宏/函数 */
        __HAL_SPDIFRX_DISABLE_IT(hspdif, SPDIFRX_IT_IFEIE);  /* HAL辅助宏/函数 */

        hspdif->State= HAL_SPDIFRX_STATE_READY;         /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hspdif);                           /* HAL辅助宏/函数 */

        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */
#endif /* STM32F446xx */

#endif /* SPDIFRX */
#endif /* HAL_SPDIFRX_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
