/* 文件: stm32f4xx_hal_smartcard.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_smartcard.c
  * @author  MCD Application Team
  * @brief   SMARTCARD HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the SMARTCARD peripheral:
  *           + Initialization and de-initialization functions
  *           + IO operation functions
  *           + Peripheral Control functions
  *           + Peripheral State and Error functions
  *
  @verbatim
  ==============================================================================
                     ##### How to use this driver #####
  ==============================================================================
    [..]
      The SMARTCARD HAL driver can be used as follows:

    (#) Declare a SMARTCARD_HandleTypeDef handle structure.
    (#) Initialize the SMARTCARD low level resources by implementing the HAL_SMARTCARD_MspInit() API:
        (##) Enable the interface clock of the USARTx associated to the SMARTCARD.
        (##) SMARTCARD pins configuration:
            (+++) Enable the clock for the SMARTCARD GPIOs.
            (+++) Configure SMARTCARD pins as alternate function pull-up.
        (##) NVIC configuration if you need to use interrupt process (HAL_SMARTCARD_Transmit_IT()
             and HAL_SMARTCARD_Receive_IT() APIs):
            (+++) Configure the USARTx interrupt priority.
            (+++) Enable the NVIC USART IRQ handle.
        (##) DMA Configuration if you need to use DMA process (HAL_SMARTCARD_Transmit_DMA()
             and HAL_SMARTCARD_Receive_DMA() APIs):
            (+++) Declare a DMA handle structure for the Tx/Rx stream.
            (+++) Enable the DMAx interface clock.
            (+++) Configure the declared DMA handle structure with the required Tx/Rx parameters.
            (+++) Configure the DMA Tx/Rx stream.
            (+++) Associate the initialized DMA handle to the SMARTCARD DMA Tx/Rx handle.
            (+++) Configure the priority and enable the NVIC for the transfer complete interrupt on the DMA Tx/Rx stream.
            (+++) Configure the USARTx interrupt priority and enable the NVIC USART IRQ handle
                  (used for last byte sending completion detection in DMA non circular mode)

    (#) Program the Baud Rate, Word Length , Stop Bit, Parity, Hardware
        flow control and Mode(Receiver/Transmitter) in the SMARTCARD Init structure.

    (#) Initialize the SMARTCARD registers by calling the HAL_SMARTCARD_Init() API:
        (++) These APIs configure also the low level Hardware GPIO, CLOCK, CORTEX...etc)
             by calling the customized HAL_SMARTCARD_MspInit() API.
    [..]
    (@) The specific SMARTCARD interrupts (Transmission complete interrupt,
        RXNE interrupt and Error Interrupts) will be managed using the macros
        __HAL_SMARTCARD_ENABLE_IT() and __HAL_SMARTCARD_DISABLE_IT() inside the transmit and receive process.

    [..]
    Three operation modes are available within this driver :

    *** Polling mode IO operation ***
    =================================
    [..]
      (+) Send an amount of data in blocking mode using HAL_SMARTCARD_Transmit()
      (+) Receive an amount of data in blocking mode using HAL_SMARTCARD_Receive()

    *** Interrupt mode IO operation ***
    ===================================
    [..]
      (+) Send an amount of data in non blocking mode using HAL_SMARTCARD_Transmit_IT()
      (+) At transmission end of transfer HAL_SMARTCARD_TxCpltCallback is executed and user can
          add his own code by customization of function pointer HAL_SMARTCARD_TxCpltCallback
      (+) Receive an amount of data in non blocking mode using HAL_SMARTCARD_Receive_IT()
      (+) At reception end of transfer HAL_SMARTCARD_RxCpltCallback is executed and user can
          add his own code by customization of function pointer HAL_SMARTCARD_RxCpltCallback
      (+) In case of transfer Error, HAL_SMARTCARD_ErrorCallback() function is executed and user can
          add his own code by customization of function pointer HAL_SMARTCARD_ErrorCallback

    *** DMA mode IO operation ***
    ==============================
    [..]
      (+) Send an amount of data in non blocking mode (DMA) using HAL_SMARTCARD_Transmit_DMA()
      (+) At transmission end of transfer HAL_SMARTCARD_TxCpltCallback is executed and user can
          add his own code by customization of function pointer HAL_SMARTCARD_TxCpltCallback
      (+) Receive an amount of data in non blocking mode (DMA) using HAL_SMARTCARD_Receive_DMA()
      (+) At reception end of transfer HAL_SMARTCARD_RxCpltCallback is executed and user can
          add his own code by customization of function pointer HAL_SMARTCARD_RxCpltCallback
      (+) In case of transfer Error, HAL_SMARTCARD_ErrorCallback() function is executed and user can
          add his own code by customization of function pointer HAL_SMARTCARD_ErrorCallback

    *** SMARTCARD HAL driver macros list ***
    ========================================
    [..]
      Below the list of most used macros in SMARTCARD HAL driver.

      (+) __HAL_SMARTCARD_ENABLE: Enable the SMARTCARD peripheral
      (+) __HAL_SMARTCARD_DISABLE: Disable the SMARTCARD peripheral
      (+) __HAL_SMARTCARD_GET_FLAG : Check whether the specified SMARTCARD flag is set or not
      (+) __HAL_SMARTCARD_CLEAR_FLAG : Clear the specified SMARTCARD pending flag
      (+) __HAL_SMARTCARD_ENABLE_IT: Enable the specified SMARTCARD interrupt
      (+) __HAL_SMARTCARD_DISABLE_IT: Disable the specified SMARTCARD interrupt

    [..]
      (@) You can refer to the SMARTCARD HAL driver header file for more useful macros

    ##### Callback registration #####
    ==================================

    [..]
    The compilation define USE_HAL_SMARTCARD_REGISTER_CALLBACKS when set to 1
    allows the user to configure dynamically the driver callbacks.

    [..]
    Use Function @ref HAL_SMARTCARD_RegisterCallback() to register a user callback.
    Function @ref HAL_SMARTCARD_RegisterCallback() allows to register following callbacks:
    (+) TxCpltCallback            : Tx Complete Callback.
    (+) RxCpltCallback            : Rx Complete Callback.
    (+) ErrorCallback             : Error Callback.
    (+) AbortCpltCallback         : Abort Complete Callback.
    (+) AbortTransmitCpltCallback : Abort Transmit Complete Callback.
    (+) AbortReceiveCpltCallback  : Abort Receive Complete Callback.
    (+) MspInitCallback           : SMARTCARD MspInit.
    (+) MspDeInitCallback         : SMARTCARD MspDeInit.
    This function takes as parameters the HAL peripheral handle, the Callback ID
    and a pointer to the user callback function.

    [..]
    Use function @ref HAL_SMARTCARD_UnRegisterCallback() to reset a callback to the default
    weak (surcharged) function.
    @ref HAL_SMARTCARD_UnRegisterCallback() takes as parameters the HAL peripheral handle,
    and the Callback ID.
    This function allows to reset following callbacks:
    (+) TxCpltCallback            : Tx Complete Callback.
    (+) RxCpltCallback            : Rx Complete Callback.
    (+) ErrorCallback             : Error Callback.
    (+) AbortCpltCallback         : Abort Complete Callback.
    (+) AbortTransmitCpltCallback : Abort Transmit Complete Callback.
    (+) AbortReceiveCpltCallback  : Abort Receive Complete Callback.
    (+) MspInitCallback           : SMARTCARD MspInit.
    (+) MspDeInitCallback         : SMARTCARD MspDeInit.

    [..]
    By default, after the @ref HAL_SMARTCARD_Init() and when the state is HAL_SMARTCARD_STATE_RESET
    all callbacks are set to the corresponding weak (surcharged) functions:
    examples @ref HAL_SMARTCARD_TxCpltCallback(), @ref HAL_SMARTCARD_RxCpltCallback().
    Exception done for MspInit and MspDeInit functions that are respectively
    reset to the legacy weak (surcharged) functions in the @ref HAL_SMARTCARD_Init()
    and @ref HAL_SMARTCARD_DeInit() only when these callbacks are null (not registered beforehand).
    If not, MspInit or MspDeInit are not null, the @ref HAL_SMARTCARD_Init() and @ref HAL_SMARTCARD_DeInit()
    keep and use the user MspInit/MspDeInit callbacks (registered beforehand).

    [..]
    Callbacks can be registered/unregistered in HAL_SMARTCARD_STATE_READY state only.
    Exception done MspInit/MspDeInit that can be registered/unregistered
    in HAL_SMARTCARD_STATE_READY or HAL_SMARTCARD_STATE_RESET state, thus registered (user)
    MspInit/DeInit callbacks can be used during the Init/DeInit.
    In that case first register the MspInit/MspDeInit user callbacks
    using @ref HAL_SMARTCARD_RegisterCallback() before calling @ref HAL_SMARTCARD_DeInit()
    or @ref HAL_SMARTCARD_Init() function.

    [..]
    When The compilation define USE_HAL_SMARTCARD_REGISTER_CALLBACKS is set to 0 or
    not defined, the callback registration feature is not available
    and weak (surcharged) callbacks are used.

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

/** @defgroup SMARTCARD SMARTCARD
  * @brief HAL SMARTCARD module driver
  * @{
  */
#ifdef HAL_SMARTCARD_MODULE_ENABLED                     /* 条件编译 */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @addtogroup SMARTCARD_Private_Constants
  * @{
  */
/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/** @addtogroup SMARTCARD_Private_Functions
  * @{
  */
#if (USE_HAL_SMARTCARD_REGISTER_CALLBACKS == 1)         /* 条件编译 */
void SMARTCARD_InitCallbacksToDefault(SMARTCARD_HandleTypeDef *hsc);  /* 函数定义或声明 */
#endif /* USE_HAL_SMARTCARD_REGISTER_CALLBACKS */
static void SMARTCARD_EndTxTransfer(SMARTCARD_HandleTypeDef *hsc);  /* 静态函数 */
static void SMARTCARD_EndRxTransfer(SMARTCARD_HandleTypeDef *hsc);  /* 静态函数 */
static void SMARTCARD_SetConfig (SMARTCARD_HandleTypeDef *hsc);  /* 静态函数 */
static HAL_StatusTypeDef SMARTCARD_Transmit_IT(SMARTCARD_HandleTypeDef *hsc);  /* 静态变量/函数 */
static HAL_StatusTypeDef SMARTCARD_EndTransmit_IT(SMARTCARD_HandleTypeDef *hsc);  /* 静态变量/函数 */
static HAL_StatusTypeDef SMARTCARD_Receive_IT(SMARTCARD_HandleTypeDef *hsc);  /* 静态变量/函数 */
static void SMARTCARD_DMATransmitCplt(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void SMARTCARD_DMAReceiveCplt(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void SMARTCARD_DMAError(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void SMARTCARD_DMAAbortOnError(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void SMARTCARD_DMATxAbortCallback(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void SMARTCARD_DMARxAbortCallback(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void SMARTCARD_DMATxOnlyAbortCallback(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void SMARTCARD_DMARxOnlyAbortCallback(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static HAL_StatusTypeDef SMARTCARD_WaitOnFlagUntilTimeout(SMARTCARD_HandleTypeDef *hsc, uint32_t Flag, FlagStatus Status, uint32_t Tickstart, uint32_t Timeout);  /* 静态变量/函数 */
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup SMARTCARD_Exported_Functions SMARTCARD Exported Functions
  * @{
  */

/** @defgroup SMARTCARD_Exported_Functions_Group1 SmartCard Initialization and de-initialization functions
  *  @brief    Initialization and Configuration functions
  *
@verbatim
  ==============================================================================
              ##### Initialization and Configuration functions #####
  ==============================================================================
  [..]
  This subsection provides a set of functions allowing to initialize the USART
  in Smartcard mode.
  [..]
  The Smartcard interface is designed to support asynchronous protocol Smartcards as
  defined in the ISO 7816-3 standard.
  [..]
  The USART can provide a clock to the smartcard through the SCLK output.
  In smartcard mode, SCLK is not associated to the communication but is simply derived
  from the internal peripheral input clock through a 5-bit prescaler.
  [..]
  (+) For the Smartcard mode only these parameters can be configured:
      (++) Baud Rate
      (++) Word Length => Should be 9 bits (8 bits + parity)
      (++) Stop Bit
      (++) Parity: => Should be enabled
      (++) USART polarity
      (++) USART phase
      (++) USART LastBit
      (++) Receiver/transmitter modes
      (++) Prescaler
      (++) GuardTime
      (++) NACKState: The Smartcard NACK state

     (+) Recommended SmartCard interface configuration to get the Answer to Reset from the Card:
        (++) Word Length = 9 Bits
        (++) 1.5 Stop Bit
        (++) Even parity
        (++) BaudRate = 12096 baud
        (++) Tx and Rx enabled
  [..]
  Please refer to the ISO 7816-3 specification for more details.

  [..]
   (@) It is also possible to choose 0.5 stop bit for receiving but it is recommended
       to use 1.5 stop bits for both transmitting and receiving to avoid switching
       between the two configurations.
  [..]
    The HAL_SMARTCARD_Init() function follows the USART  SmartCard configuration
    procedures (details for the procedures are available in reference manual
    (RM0430 for STM32F4X3xx MCUs and RM0402 for STM32F412xx MCUs
     RM0383 for STM32F411xC/E MCUs and RM0401 for STM32F410xx MCUs
     RM0090 for STM32F4X5xx/STM32F4X7xx/STM32F429xx/STM32F439xx MCUs
     RM0390 for STM32F446xx MCUs and RM0386 for STM32F469xx/STM32F479xx MCUs)).

@endverbatim

  The SMARTCARD frame format is given in the following table:
       +-------------------------------------------------------------+
       |   M bit |  PCE bit  |        SMARTCARD frame                |
       |---------------------|---------------------------------------|
       |    1    |    1      |    | SB | 8 bit data | PB | STB |     |
       +-------------------------------------------------------------+
  * @{
  */

/**
  * @brief  Initializes the SmartCard mode according to the specified
  *         parameters in the SMARTCARD_InitTypeDef and create the associated handle.
  * @param  hsc    Pointer to a SMARTCARD_HandleTypeDef structure that contains
  *                the configuration information for SMARTCARD module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SMARTCARD_Init(SMARTCARD_HandleTypeDef *hsc)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the SMARTCARD handle allocation */
  if(hsc == NULL)                                       /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the parameters */
  assert_param(IS_SMARTCARD_INSTANCE(hsc->Instance));   /* 执行语句 */
  assert_param(IS_SMARTCARD_NACK_STATE(hsc->Init.NACKState));  /* 执行语句 */

  if(hsc->gState == HAL_SMARTCARD_STATE_RESET)          /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Allocate lock resource and initialize it */
    hsc->Lock = HAL_UNLOCKED;                           /* 赋值或初始化 */

#if USE_HAL_SMARTCARD_REGISTER_CALLBACKS == 1           /* 条件编译 */
    SMARTCARD_InitCallbacksToDefault(hsc);              /* 执行语句 */

    if (hsc->MspInitCallback == NULL)                   /* 条件判断 */
    {                                                   /* 代码块开始 */
      hsc->MspInitCallback = HAL_SMARTCARD_MspInit;     /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Init the low level hardware */
    hsc->MspInitCallback(hsc);                          /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /* Init the low level hardware : GPIO, CLOCK */
    HAL_SMARTCARD_MspInit(hsc);                         /* HAL库函数调用 */
#endif /* USE_HAL_SMARTCARD_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */

  hsc->gState = HAL_SMARTCARD_STATE_BUSY;               /* 赋值或初始化 */

  /* Set the Prescaler */
  MODIFY_REG(hsc->Instance->GTPR, USART_GTPR_PSC, hsc->Init.Prescaler);  /* 执行语句 */

  /* Set the Guard Time */
  MODIFY_REG(hsc->Instance->GTPR, USART_GTPR_GT, ((hsc->Init.GuardTime)<<8U));  /* 执行语句 */

  /* Set the Smartcard Communication parameters */
  SMARTCARD_SetConfig(hsc);                             /* 执行语句 */

  /* In SmartCard mode, the following bits must be kept cleared:
  - LINEN bit in the USART_CR2 register
  - HDSEL and IREN bits in the USART_CR3 register.*/
  CLEAR_BIT(hsc->Instance->CR2, USART_CR2_LINEN);       /* 执行语句 */
  CLEAR_BIT(hsc->Instance->CR3, (USART_CR3_IREN | USART_CR3_HDSEL));  /* 执行语句 */

  /* Enable the SMARTCARD Parity Error Interrupt */
  SET_BIT(hsc->Instance->CR1, USART_CR1_PEIE);          /* 执行语句 */

  /* Enable the SMARTCARD Framing Error Interrupt */
  SET_BIT(hsc->Instance->CR3, USART_CR3_EIE);           /* 执行语句 */

  /* Enable the Peripheral */
  __HAL_SMARTCARD_ENABLE(hsc);                          /* HAL辅助宏/函数 */

  /* Configure the Smartcard NACK state */
  MODIFY_REG(hsc->Instance->CR3, USART_CR3_NACK, hsc->Init.NACKState);  /* 执行语句 */

  /* Enable the SC mode by setting the SCEN bit in the CR3 register */
  hsc->Instance->CR3 |= (USART_CR3_SCEN);               /* 赋值或初始化 */

  /* Initialize the SMARTCARD state*/
  hsc->ErrorCode = HAL_SMARTCARD_ERROR_NONE;            /* 赋值或初始化 */
  hsc->gState= HAL_SMARTCARD_STATE_READY;               /* 赋值或初始化 */
  hsc->RxState= HAL_SMARTCARD_STATE_READY;              /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief DeInitializes the USART SmartCard peripheral
  * @param  hsc    Pointer to a SMARTCARD_HandleTypeDef structure that contains
  *                the configuration information for SMARTCARD module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SMARTCARD_DeInit(SMARTCARD_HandleTypeDef *hsc)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the SMARTCARD handle allocation */
  if(hsc == NULL)                                       /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the parameters */
  assert_param(IS_SMARTCARD_INSTANCE(hsc->Instance));   /* 执行语句 */

  hsc->gState = HAL_SMARTCARD_STATE_BUSY;               /* 赋值或初始化 */

  /* Disable the Peripheral */
  __HAL_SMARTCARD_DISABLE(hsc);                         /* HAL辅助宏/函数 */

  /* DeInit the low level hardware */
#if USE_HAL_SMARTCARD_REGISTER_CALLBACKS == 1           /* 条件编译 */
  if (hsc->MspDeInitCallback == NULL)                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    hsc->MspDeInitCallback = HAL_SMARTCARD_MspDeInit;   /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  /* DeInit the low level hardware */
  hsc->MspDeInitCallback(hsc);                          /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_SMARTCARD_MspDeInit(hsc);                         /* HAL库函数调用 */
#endif /* USE_HAL_SMARTCARD_REGISTER_CALLBACKS */

  hsc->ErrorCode = HAL_SMARTCARD_ERROR_NONE;            /* 赋值或初始化 */
  hsc->gState = HAL_SMARTCARD_STATE_RESET;              /* 赋值或初始化 */
  hsc->RxState = HAL_SMARTCARD_STATE_RESET;             /* 赋值或初始化 */

  /* Release Lock */
  __HAL_UNLOCK(hsc);                                    /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  SMARTCARD MSP Init
  * @param  hsc    Pointer to a SMARTCARD_HandleTypeDef structure that contains
  *                the configuration information for SMARTCARD module.
  * @retval None
  */
__weak void HAL_SMARTCARD_MspInit(SMARTCARD_HandleTypeDef *hsc)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsc);                                          /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SMARTCARD_MspInit can be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief SMARTCARD MSP DeInit
  * @param  hsc    Pointer to a SMARTCARD_HandleTypeDef structure that contains
  *                the configuration information for SMARTCARD module.
  * @retval None
  */
__weak void HAL_SMARTCARD_MspDeInit(SMARTCARD_HandleTypeDef *hsc)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsc);                                          /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SMARTCARD_MspDeInit can be implemented in the user file
   */
}                                                       /* 代码块结束 */

#if (USE_HAL_SMARTCARD_REGISTER_CALLBACKS == 1)         /* 条件编译 */
/**
  * @brief  Register a User SMARTCARD Callback
  *         To be used instead of the weak predefined callback
  * @param  hsc smartcard handle
  * @param  CallbackID ID of the callback to be registered
  *         This parameter can be one of the following values:
  *           @arg @ref HAL_SMARTCARD_TX_COMPLETE_CB_ID Tx Complete Callback ID
  *           @arg @ref HAL_SMARTCARD_RX_COMPLETE_CB_ID Rx Complete Callback ID
  *           @arg @ref HAL_SMARTCARD_ERROR_CB_ID Error Callback ID
  *           @arg @ref HAL_SMARTCARD_ABORT_COMPLETE_CB_ID Abort Complete Callback ID
  *           @arg @ref HAL_SMARTCARD_ABORT_TRANSMIT_COMPLETE_CB_ID Abort Transmit Complete Callback ID
  *           @arg @ref HAL_SMARTCARD_ABORT_RECEIVE_COMPLETE_CB_ID Abort Receive Complete Callback ID
  *           @arg @ref HAL_SMARTCARD_MSPINIT_CB_ID MspInit Callback ID
  *           @arg @ref HAL_SMARTCARD_MSPDEINIT_CB_ID MspDeInit Callback ID
  * @param  pCallback pointer to the Callback function
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SMARTCARD_RegisterCallback(SMARTCARD_HandleTypeDef *hsc, HAL_SMARTCARD_CallbackIDTypeDef CallbackID, pSMARTCARD_CallbackTypeDef pCallback)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  if (pCallback == NULL)                                /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hsc->ErrorCode |= HAL_SMARTCARD_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  /* Process locked */
  __HAL_LOCK(hsc);                                      /* HAL辅助宏/函数 */

  if (hsc->gState == HAL_SMARTCARD_STATE_READY)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */

      case HAL_SMARTCARD_TX_COMPLETE_CB_ID :            /* case标签 */
        hsc->TxCpltCallback = pCallback;                /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_SMARTCARD_RX_COMPLETE_CB_ID :            /* case标签 */
        hsc->RxCpltCallback = pCallback;                /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_SMARTCARD_ERROR_CB_ID :                  /* case标签 */
        hsc->ErrorCallback = pCallback;                 /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_SMARTCARD_ABORT_COMPLETE_CB_ID :         /* case标签 */
        hsc->AbortCpltCallback = pCallback;             /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_SMARTCARD_ABORT_TRANSMIT_COMPLETE_CB_ID :  /* case标签 */
        hsc->AbortTransmitCpltCallback = pCallback;     /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_SMARTCARD_ABORT_RECEIVE_COMPLETE_CB_ID :  /* case标签 */
        hsc->AbortReceiveCpltCallback = pCallback;      /* 赋值或初始化 */
        break;                                          /* 跳出 */


      case HAL_SMARTCARD_MSPINIT_CB_ID :                /* case标签 */
        hsc->MspInitCallback = pCallback;               /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_SMARTCARD_MSPDEINIT_CB_ID :              /* case标签 */
        hsc->MspDeInitCallback = pCallback;             /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hsc->ErrorCode |= HAL_SMARTCARD_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (hsc->gState == HAL_SMARTCARD_STATE_RESET)    /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_SMARTCARD_MSPINIT_CB_ID :                /* case标签 */
        hsc->MspInitCallback = pCallback;               /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_SMARTCARD_MSPDEINIT_CB_ID :              /* case标签 */
        hsc->MspDeInitCallback = pCallback;             /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hsc->ErrorCode |= HAL_SMARTCARD_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hsc->ErrorCode |= HAL_SMARTCARD_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hsc);                                    /* HAL辅助宏/函数 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Unregister an SMARTCARD callback
  *         SMARTCARD callback is redirected to the weak predefined callback
  * @param  hsc smartcard handle
  * @param  CallbackID ID of the callback to be unregistered
  *         This parameter can be one of the following values:
  *           @arg @ref HAL_SMARTCARD_TX_COMPLETE_CB_ID Tx Complete Callback ID
  *           @arg @ref HAL_SMARTCARD_RX_COMPLETE_CB_ID Rx Complete Callback ID
  *           @arg @ref HAL_SMARTCARD_ERROR_CB_ID Error Callback ID
  *           @arg @ref HAL_SMARTCARD_ABORT_COMPLETE_CB_ID Abort Complete Callback ID
  *           @arg @ref HAL_SMARTCARD_ABORT_TRANSMIT_COMPLETE_CB_ID Abort Transmit Complete Callback ID
  *           @arg @ref HAL_SMARTCARD_ABORT_RECEIVE_COMPLETE_CB_ID Abort Receive Complete Callback ID
  *           @arg @ref HAL_SMARTCARD_MSPINIT_CB_ID MspInit Callback ID
  *           @arg @ref HAL_SMARTCARD_MSPDEINIT_CB_ID MspDeInit Callback ID
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SMARTCARD_UnRegisterCallback(SMARTCARD_HandleTypeDef *hsc, HAL_SMARTCARD_CallbackIDTypeDef CallbackID)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Process locked */
  __HAL_LOCK(hsc);                                      /* HAL辅助宏/函数 */

  if (HAL_SMARTCARD_STATE_READY == hsc->gState)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_SMARTCARD_TX_COMPLETE_CB_ID :            /* case标签 */
        hsc->TxCpltCallback = HAL_SMARTCARD_TxCpltCallback;                       /* Legacy weak TxCpltCallback            */
        break;                                          /* 跳出 */

      case HAL_SMARTCARD_RX_COMPLETE_CB_ID :            /* case标签 */
        hsc->RxCpltCallback = HAL_SMARTCARD_RxCpltCallback;                       /* Legacy weak RxCpltCallback            */
        break;                                          /* 跳出 */

      case HAL_SMARTCARD_ERROR_CB_ID :                  /* case标签 */
        hsc->ErrorCallback = HAL_SMARTCARD_ErrorCallback;                         /* Legacy weak ErrorCallback             */
        break;                                          /* 跳出 */

      case HAL_SMARTCARD_ABORT_COMPLETE_CB_ID :         /* case标签 */
        hsc->AbortCpltCallback = HAL_SMARTCARD_AbortCpltCallback;                 /* Legacy weak AbortCpltCallback         */
        break;                                          /* 跳出 */

      case HAL_SMARTCARD_ABORT_TRANSMIT_COMPLETE_CB_ID :  /* case标签 */
        hsc->AbortTransmitCpltCallback = HAL_SMARTCARD_AbortTransmitCpltCallback; /* Legacy weak AbortTransmitCpltCallback */
        break;                                          /* 跳出 */

      case HAL_SMARTCARD_ABORT_RECEIVE_COMPLETE_CB_ID :  /* case标签 */
        hsc->AbortReceiveCpltCallback = HAL_SMARTCARD_AbortReceiveCpltCallback;   /* Legacy weak AbortReceiveCpltCallback  */
        break;                                          /* 跳出 */


      case HAL_SMARTCARD_MSPINIT_CB_ID :                /* case标签 */
        hsc->MspInitCallback = HAL_SMARTCARD_MspInit;                             /* Legacy weak MspInitCallback           */
        break;                                          /* 跳出 */

      case HAL_SMARTCARD_MSPDEINIT_CB_ID :              /* case标签 */
        hsc->MspDeInitCallback = HAL_SMARTCARD_MspDeInit;                         /* Legacy weak MspDeInitCallback         */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hsc->ErrorCode |= HAL_SMARTCARD_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (HAL_SMARTCARD_STATE_RESET == hsc->gState)    /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_SMARTCARD_MSPINIT_CB_ID :                /* case标签 */
        hsc->MspInitCallback = HAL_SMARTCARD_MspInit;   /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_SMARTCARD_MSPDEINIT_CB_ID :              /* case标签 */
        hsc->MspDeInitCallback = HAL_SMARTCARD_MspDeInit;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hsc->ErrorCode |= HAL_SMARTCARD_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hsc->ErrorCode |= HAL_SMARTCARD_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hsc);                                    /* HAL辅助宏/函数 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* USE_HAL_SMARTCARD_REGISTER_CALLBACKS */

/**
  * @}
  */

/** @defgroup SMARTCARD_Exported_Functions_Group2 IO operation functions
  * @brief    SMARTCARD Transmit and Receive functions
  *
@verbatim
 ===============================================================================
                      ##### IO operation functions #####
 ===============================================================================
 [..]
   This subsection provides a set of functions allowing to manage the SMARTCARD data transfers.

 [..]
    (#) Smartcard is a single wire half duplex communication protocol.
    The Smartcard interface is designed to support asynchronous protocol Smartcards as
    defined in the ISO 7816-3 standard.
    (#) The USART should be configured as:
       (++) 8 bits plus parity: where M=1 and PCE=1 in the USART_CR1 register
       (++) 1.5 stop bits when transmitting and receiving: where STOP=11 in the USART_CR2 register.

    (#) There are two modes of transfer:
       (++) Blocking mode: The communication is performed in polling mode.
            The HAL status of all data processing is returned by the same function
            after finishing transfer.
       (++) Non Blocking mode: The communication is performed using Interrupts
           or DMA, These APIs return the HAL status.
           The end of the data processing will be indicated through the
           dedicated SMARTCARD IRQ when using Interrupt mode or the DMA IRQ when
           using DMA mode.
           The HAL_SMARTCARD_TxCpltCallback(), HAL_SMARTCARD_RxCpltCallback() user callbacks
           will be executed respectively at the end of the Transmit or Receive process
           The HAL_SMARTCARD_ErrorCallback() user callback will be executed when a communication error is detected

    (#) Blocking mode APIs are :
        (++) HAL_SMARTCARD_Transmit()
        (++) HAL_SMARTCARD_Receive()

    (#) Non Blocking mode APIs with Interrupt are :
        (++) HAL_SMARTCARD_Transmit_IT()
        (++) HAL_SMARTCARD_Receive_IT()
        (++) HAL_SMARTCARD_IRQHandler()

    (#) Non Blocking mode functions with DMA are :
        (++) HAL_SMARTCARD_Transmit_DMA()
        (++) HAL_SMARTCARD_Receive_DMA()

    (#) A set of Transfer Complete Callbacks are provided in non Blocking mode:
        (++) HAL_SMARTCARD_TxCpltCallback()
        (++) HAL_SMARTCARD_RxCpltCallback()
        (++) HAL_SMARTCARD_ErrorCallback()

    (#) Non-Blocking mode transfers could be aborted using Abort API's :
        (+) HAL_SMARTCARD_Abort()
        (+) HAL_SMARTCARD_AbortTransmit()
        (+) HAL_SMARTCARD_AbortReceive()
        (+) HAL_SMARTCARD_Abort_IT()
        (+) HAL_SMARTCARD_AbortTransmit_IT()
        (+) HAL_SMARTCARD_AbortReceive_IT()

    (#) For Abort services based on interrupts (HAL_SMARTCARD_Abortxxx_IT), a set of Abort Complete Callbacks are provided:
        (+) HAL_SMARTCARD_AbortCpltCallback()
        (+) HAL_SMARTCARD_AbortTransmitCpltCallback()
        (+) HAL_SMARTCARD_AbortReceiveCpltCallback()

    (#) In Non-Blocking mode transfers, possible errors are split into 2 categories.
        Errors are handled as follows :
       (+) Error is considered as Recoverable and non blocking : Transfer could go till end, but error severity is
           to be evaluated by user : this concerns Frame Error, Parity Error or Noise Error in Interrupt mode reception .
           Received character is then retrieved and stored in Rx buffer, Error code is set to allow user to identify error type,
           and HAL_SMARTCARD_ErrorCallback() user callback is executed. Transfer is kept ongoing on SMARTCARD side.
           If user wants to abort it, Abort services should be called by user.
       (+) Error is considered as Blocking : Transfer could not be completed properly and is aborted.
           This concerns Frame Error in Interrupt mode transmission, Overrun Error in Interrupt mode reception and all errors in DMA mode.
           Error code is set to allow user to identify error type, and HAL_SMARTCARD_ErrorCallback() user callback is executed.

@endverbatim
  * @{
  */

/**
  * @brief Send an amount of data in blocking mode
  * @param  hsc    Pointer to a SMARTCARD_HandleTypeDef structure that contains
  *                the configuration information for SMARTCARD module.
  * @param  pData  Pointer to data buffer
  * @param  Size   Amount of data to be sent
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SMARTCARD_Transmit(SMARTCARD_HandleTypeDef *hsc, uint8_t *pData, uint16_t Size, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint8_t *tmp = pData;                                 /* 8位无符号变量/函数 */
  uint32_t tickstart = 0U;                              /* 32位无符号变量/函数 */

  if(hsc->gState == HAL_SMARTCARD_STATE_READY)          /* 条件判断 */
  {                                                     /* 代码块开始 */
    if((pData == NULL) || (Size == 0U))                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      return  HAL_ERROR;                                /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hsc);                                    /* HAL辅助宏/函数 */

    hsc->ErrorCode = HAL_SMARTCARD_ERROR_NONE;          /* 赋值或初始化 */
    hsc->gState = HAL_SMARTCARD_STATE_BUSY_TX;          /* 赋值或初始化 */

    /* Init tickstart for timeout management */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */

    hsc->TxXferSize = Size;                             /* 赋值或初始化 */
    hsc->TxXferCount = Size;                            /* 赋值或初始化 */
    while(hsc->TxXferCount > 0U)                        /* while循环 */
    {                                                   /* 代码块开始 */
      hsc->TxXferCount--;                               /* 执行语句 */
      if(SMARTCARD_WaitOnFlagUntilTimeout(hsc, SMARTCARD_FLAG_TXE, RESET, tickstart, Timeout) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
      hsc->Instance->DR = (uint8_t)(*tmp & 0xFFU);      /* 赋值或初始化 */
      tmp++;                                            /* 执行语句 */
    }                                                   /* 代码块结束 */

    if(SMARTCARD_WaitOnFlagUntilTimeout(hsc, SMARTCARD_FLAG_TC, RESET, tickstart, Timeout) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_TIMEOUT;                               /* 返回值 */
    }                                                   /* 代码块结束 */

	/* At end of Tx process, restore hsc->gState to Ready */
    hsc->gState = HAL_SMARTCARD_STATE_READY;            /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hsc);                                  /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief Receive an amount of data in blocking mode
  * @param  hsc    Pointer to a SMARTCARD_HandleTypeDef structure that contains
  *                the configuration information for SMARTCARD module.
  * @param  pData  Pointer to data buffer
  * @param  Size   Amount of data to be received
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SMARTCARD_Receive(SMARTCARD_HandleTypeDef *hsc, uint8_t *pData, uint16_t Size, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint8_t  *tmp = pData;                                /* 8位无符号变量/函数 */
  uint32_t tickstart = 0U;                              /* 32位无符号变量/函数 */

  if(hsc->RxState == HAL_SMARTCARD_STATE_READY)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    if((pData == NULL) || (Size == 0U))                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      return  HAL_ERROR;                                /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hsc);                                    /* HAL辅助宏/函数 */

    hsc->ErrorCode = HAL_SMARTCARD_ERROR_NONE;          /* 赋值或初始化 */
    hsc->RxState = HAL_SMARTCARD_STATE_BUSY_RX;         /* 赋值或初始化 */

    /* Init tickstart for timeout management */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */

    hsc->RxXferSize = Size;                             /* 赋值或初始化 */
    hsc->RxXferCount = Size;                            /* 赋值或初始化 */

    /* Check the remain data to be received */
    while(hsc->RxXferCount > 0U)                        /* while循环 */
    {                                                   /* 代码块开始 */
      hsc->RxXferCount--;                               /* 执行语句 */
      if(SMARTCARD_WaitOnFlagUntilTimeout(hsc, SMARTCARD_FLAG_RXNE, RESET, tickstart, Timeout) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
      *tmp = (uint8_t)(hsc->Instance->DR & (uint8_t)0xFFU);  /* 赋值或初始化 */
      tmp++;                                            /* 执行语句 */
    }                                                   /* 代码块结束 */

    /* At end of Rx process, restore hsc->RxState to Ready */
    hsc->RxState = HAL_SMARTCARD_STATE_READY;           /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hsc);                                  /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief Send an amount of data in non blocking mode
  * @param  hsc    Pointer to a SMARTCARD_HandleTypeDef structure that contains
  *                the configuration information for SMARTCARD module.
  * @param  pData  Pointer to data buffer
  * @param  Size   Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SMARTCARD_Transmit_IT(SMARTCARD_HandleTypeDef *hsc, uint8_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check that a Tx process is not already ongoing */
  if(hsc->gState == HAL_SMARTCARD_STATE_READY)          /* 条件判断 */
  {                                                     /* 代码块开始 */
    if((pData == NULL) || (Size == 0U))                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hsc);                                    /* HAL辅助宏/函数 */

    hsc->pTxBuffPtr = pData;                            /* 赋值或初始化 */
    hsc->TxXferSize = Size;                             /* 赋值或初始化 */
    hsc->TxXferCount = Size;                            /* 赋值或初始化 */

    hsc->ErrorCode = HAL_SMARTCARD_ERROR_NONE;          /* 赋值或初始化 */
    hsc->gState = HAL_SMARTCARD_STATE_BUSY_TX;          /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hsc);                                  /* HAL辅助宏/函数 */

    /* Enable the SMARTCARD Parity Error Interrupt */
    SET_BIT(hsc->Instance->CR1, USART_CR1_PEIE);        /* 执行语句 */

    /* Disable the SMARTCARD Error Interrupt: (Frame error, noise error, overrun error) */
    CLEAR_BIT(hsc->Instance->CR3, USART_CR3_EIE);       /* 执行语句 */

    /* Enable the SMARTCARD Transmit data register empty Interrupt */
    SET_BIT(hsc->Instance->CR1, USART_CR1_TXEIE);       /* 执行语句 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief Receive an amount of data in non blocking mode
  * @param  hsc    Pointer to a SMARTCARD_HandleTypeDef structure that contains
  *                the configuration information for SMARTCARD module.
  * @param  pData  Pointer to data buffer
  * @param  Size   Amount of data to be received
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SMARTCARD_Receive_IT(SMARTCARD_HandleTypeDef *hsc, uint8_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check that a Rx process is not already ongoing */
  if(hsc->RxState == HAL_SMARTCARD_STATE_READY)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    if((pData == NULL) || (Size == 0U))                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hsc);                                    /* HAL辅助宏/函数 */

    hsc->pRxBuffPtr = pData;                            /* 赋值或初始化 */
    hsc->RxXferSize = Size;                             /* 赋值或初始化 */
    hsc->RxXferCount = Size;                            /* 赋值或初始化 */

    hsc->ErrorCode = HAL_SMARTCARD_ERROR_NONE;          /* 赋值或初始化 */
    hsc->RxState = HAL_SMARTCARD_STATE_BUSY_RX;         /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hsc);                                  /* HAL辅助宏/函数 */

    /* Enable the SMARTCARD Parity Error and Data Register not empty Interrupts */
    SET_BIT(hsc->Instance->CR1, USART_CR1_PEIE| USART_CR1_RXNEIE);  /* 执行语句 */

    /* Enable the SMARTCARD Error Interrupt: (Frame error, noise error, overrun error) */
    SET_BIT(hsc->Instance->CR3, USART_CR3_EIE);         /* 执行语句 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief Send an amount of data in non blocking mode
  * @param  hsc    Pointer to a SMARTCARD_HandleTypeDef structure that contains
  *                the configuration information for SMARTCARD module.
  * @param  pData  Pointer to data buffer
  * @param  Size   Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SMARTCARD_Transmit_DMA(SMARTCARD_HandleTypeDef *hsc, uint8_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t *tmp;                                        /* 32位无符号变量/函数 */

  /* Check that a Tx process is not already ongoing */
  if(hsc->gState == HAL_SMARTCARD_STATE_READY)          /* 条件判断 */
  {                                                     /* 代码块开始 */
    if((pData == NULL) || (Size == 0U))                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hsc);                                    /* HAL辅助宏/函数 */

    hsc->pTxBuffPtr = pData;                            /* 赋值或初始化 */
    hsc->TxXferSize = Size;                             /* 赋值或初始化 */
    hsc->TxXferCount = Size;                            /* 赋值或初始化 */

    hsc->ErrorCode = HAL_SMARTCARD_ERROR_NONE;          /* 赋值或初始化 */
    hsc->gState = HAL_SMARTCARD_STATE_BUSY_TX;          /* 赋值或初始化 */

    /* Set the SMARTCARD DMA transfer complete callback */
    hsc->hdmatx->XferCpltCallback = SMARTCARD_DMATransmitCplt;  /* 赋值或初始化 */

    /* Set the DMA error callback */
    hsc->hdmatx->XferErrorCallback = SMARTCARD_DMAError;  /* 赋值或初始化 */

    /* Set the DMA abort callback */
    hsc->hdmatx->XferAbortCallback = NULL;              /* 赋值或初始化 */

    /* Enable the SMARTCARD transmit DMA stream */
    tmp = (uint32_t*)&pData;                            /* 赋值或初始化 */
    HAL_DMA_Start_IT(hsc->hdmatx, *(uint32_t*)tmp, (uint32_t)&hsc->Instance->DR, Size);  /* HAL DMA操作 */

     /* Clear the TC flag in the SR register by writing 0 to it */
    __HAL_SMARTCARD_CLEAR_FLAG(hsc, SMARTCARD_FLAG_TC);  /* HAL辅助宏/函数 */

    /* Process Unlocked */
    __HAL_UNLOCK(hsc);                                  /* HAL辅助宏/函数 */

    /* Enable the DMA transfer for transmit request by setting the DMAT bit
    in the SMARTCARD CR3 register */
    SET_BIT(hsc->Instance->CR3, USART_CR3_DMAT);        /* 执行语句 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief Receive an amount of data in non blocking mode
  * @param  hsc    Pointer to a SMARTCARD_HandleTypeDef structure that contains
  *                the configuration information for SMARTCARD module.
  * @param  pData  Pointer to data buffer
  * @param  Size   Amount of data to be received
  * @note   When the SMARTCARD parity is enabled (PCE = 1) the data received contain the parity bit.s
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SMARTCARD_Receive_DMA(SMARTCARD_HandleTypeDef *hsc, uint8_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t *tmp;                                        /* 32位无符号变量/函数 */

  /* Check that a Rx process is not already ongoing */
  if(hsc->RxState == HAL_SMARTCARD_STATE_READY)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    if((pData == NULL) || (Size == 0U))                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hsc);                                    /* HAL辅助宏/函数 */

    hsc->pRxBuffPtr = pData;                            /* 赋值或初始化 */
    hsc->RxXferSize = Size;                             /* 赋值或初始化 */

    hsc->ErrorCode = HAL_SMARTCARD_ERROR_NONE;          /* 赋值或初始化 */
    hsc->RxState = HAL_SMARTCARD_STATE_BUSY_RX;         /* 赋值或初始化 */

    /* Set the SMARTCARD DMA transfer complete callback */
    hsc->hdmarx->XferCpltCallback = SMARTCARD_DMAReceiveCplt;  /* 赋值或初始化 */

    /* Set the DMA error callback */
    hsc->hdmarx->XferErrorCallback = SMARTCARD_DMAError;  /* 赋值或初始化 */

    /* Set the DMA abort callback */
    hsc->hdmatx->XferAbortCallback = NULL;              /* 赋值或初始化 */

    /* Enable the DMA stream */
    tmp = (uint32_t*)&pData;                            /* 赋值或初始化 */
    HAL_DMA_Start_IT(hsc->hdmarx, (uint32_t)&hsc->Instance->DR, *(uint32_t*)tmp, Size);  /* HAL DMA操作 */

    /* Clear the Overrun flag just before enabling the DMA Rx request: can be mandatory for the second transfer */
    __HAL_SMARTCARD_CLEAR_OREFLAG(hsc);                 /* HAL辅助宏/函数 */

    /* Process Unlocked */
    __HAL_UNLOCK(hsc);                                  /* HAL辅助宏/函数 */

    /* Enable the SMARTCARD Parity Error Interrupt */
    SET_BIT(hsc->Instance->CR1, USART_CR1_PEIE);        /* 执行语句 */

    /* Enable the SMARTCARD Error Interrupt: (Frame error, noise error, overrun error) */
    SET_BIT(hsc->Instance->CR3, USART_CR3_EIE);         /* 执行语句 */

    /* Enable the DMA transfer for the receiver request by setting the DMAR bit
    in the SMARTCARD CR3 register */
    SET_BIT(hsc->Instance->CR3, USART_CR3_DMAR);        /* 执行语句 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Abort ongoing transfers (blocking mode).
  * @param  hsc SMARTCARD handle.
  * @note   This procedure could be used for aborting any ongoing transfer started in Interrupt or DMA mode.
  *         This procedure performs following operations :
  *           - Disable PPP Interrupts
  *           - Disable the DMA transfer in the peripheral register (if enabled)
  *           - Abort DMA transfer by calling HAL_DMA_Abort (in case of transfer in DMA mode)
  *           - Set handle State to READY
  * @note   This procedure is executed in blocking mode : when exiting function, Abort is considered as completed.
  * @retval HAL status
*/
HAL_StatusTypeDef HAL_SMARTCARD_Abort(SMARTCARD_HandleTypeDef *hsc)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Disable TXEIE, TCIE, RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts */
  CLEAR_BIT(hsc->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE | USART_CR1_TXEIE | USART_CR1_TCIE));  /* 执行语句 */
  CLEAR_BIT(hsc->Instance->CR3, USART_CR3_EIE);         /* 执行语句 */

  /* Disable the SMARTCARD DMA Tx request if enabled */
  if(HAL_IS_BIT_SET(hsc->Instance->CR3, USART_CR3_DMAT))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    CLEAR_BIT(hsc->Instance->CR3, USART_CR3_DMAT);      /* 执行语句 */

    /* Abort the SMARTCARD DMA Tx channel : use blocking DMA Abort API (no callback) */
    if(hsc->hdmatx != NULL)                             /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the SMARTCARD DMA Abort callback to Null.
         No call back execution at end of DMA abort procedure */
      hsc->hdmatx->XferAbortCallback = NULL;            /* 赋值或初始化 */

      HAL_DMA_Abort(hsc->hdmatx);                       /* HAL DMA操作 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Disable the SMARTCARD DMA Rx request if enabled */
  if(HAL_IS_BIT_SET(hsc->Instance->CR3, USART_CR3_DMAR))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    CLEAR_BIT(hsc->Instance->CR3, USART_CR3_DMAR);      /* 执行语句 */

    /* Abort the SMARTCARD DMA Rx channel : use blocking DMA Abort API (no callback) */
    if(hsc->hdmarx != NULL)                             /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the SMARTCARD DMA Abort callback to Null.
         No call back execution at end of DMA abort procedure */
      hsc->hdmarx->XferAbortCallback = NULL;            /* 赋值或初始化 */

      HAL_DMA_Abort(hsc->hdmarx);                       /* HAL DMA操作 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Reset Tx and Rx transfer counters */
  hsc->TxXferCount = 0x00U;                             /* 赋值或初始化 */
  hsc->RxXferCount = 0x00U;                             /* 赋值或初始化 */

  /* Reset ErrorCode */
  hsc->ErrorCode = HAL_SMARTCARD_ERROR_NONE;            /* 赋值或初始化 */

  /* Restore hsc->RxState and hsc->gState to Ready */
  hsc->RxState = HAL_SMARTCARD_STATE_READY;             /* 赋值或初始化 */
  hsc->gState = HAL_SMARTCARD_STATE_READY;              /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Abort ongoing Transmit transfer (blocking mode).
  * @param  hsc SMARTCARD handle.
  * @note   This procedure could be used for aborting any ongoing transfer started in Interrupt or DMA mode.
  *         This procedure performs following operations :
  *           - Disable SMARTCARD Interrupts (Tx)
  *           - Disable the DMA transfer in the peripheral register (if enabled)
  *           - Abort DMA transfer by calling HAL_DMA_Abort (in case of transfer in DMA mode)
  *           - Set handle State to READY
  * @note   This procedure is executed in blocking mode : when exiting function, Abort is considered as completed.
  * @retval HAL status
*/
HAL_StatusTypeDef HAL_SMARTCARD_AbortTransmit(SMARTCARD_HandleTypeDef *hsc)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Disable TXEIE and TCIE interrupts */
  CLEAR_BIT(hsc->Instance->CR1, (USART_CR1_TXEIE | USART_CR1_TCIE));  /* 执行语句 */

  /* Disable the SMARTCARD DMA Tx request if enabled */
  if(HAL_IS_BIT_SET(hsc->Instance->CR3, USART_CR3_DMAT))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    CLEAR_BIT(hsc->Instance->CR3, USART_CR3_DMAT);      /* 执行语句 */

    /* Abort the SMARTCARD DMA Tx channel : use blocking DMA Abort API (no callback) */
    if(hsc->hdmatx != NULL)                             /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the SMARTCARD DMA Abort callback to Null.
         No call back execution at end of DMA abort procedure */
      hsc->hdmatx->XferAbortCallback = NULL;            /* 赋值或初始化 */

      HAL_DMA_Abort(hsc->hdmatx);                       /* HAL DMA操作 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Reset Tx transfer counter */
  hsc->TxXferCount = 0x00U;                             /* 赋值或初始化 */

  /* Restore hsc->gState to Ready */
  hsc->gState = HAL_SMARTCARD_STATE_READY;              /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Abort ongoing Receive transfer (blocking mode).
  * @param  hsc SMARTCARD handle.
  * @note   This procedure could be used for aborting any ongoing transfer started in Interrupt or DMA mode.
  *         This procedure performs following operations :
  *           - Disable PPP Interrupts
  *           - Disable the DMA transfer in the peripheral register (if enabled)
  *           - Abort DMA transfer by calling HAL_DMA_Abort (in case of transfer in DMA mode)
  *           - Set handle State to READY
  * @note   This procedure is executed in blocking mode : when exiting function, Abort is considered as completed.
  * @retval HAL status
*/
HAL_StatusTypeDef HAL_SMARTCARD_AbortReceive(SMARTCARD_HandleTypeDef *hsc)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Disable RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts */
  CLEAR_BIT(hsc->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE));  /* 执行语句 */
  CLEAR_BIT(hsc->Instance->CR3, USART_CR3_EIE);         /* 执行语句 */

  /* Disable the SMARTCARD DMA Rx request if enabled */
  if(HAL_IS_BIT_SET(hsc->Instance->CR3, USART_CR3_DMAR))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    CLEAR_BIT(hsc->Instance->CR3, USART_CR3_DMAR);      /* 执行语句 */

    /* Abort the SMARTCARD DMA Rx channel : use blocking DMA Abort API (no callback) */
    if(hsc->hdmarx != NULL)                             /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the SMARTCARD DMA Abort callback to Null.
         No call back execution at end of DMA abort procedure */
      hsc->hdmarx->XferAbortCallback = NULL;            /* 赋值或初始化 */

      HAL_DMA_Abort(hsc->hdmarx);                       /* HAL DMA操作 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Reset Rx transfer counter */
  hsc->RxXferCount = 0x00U;                             /* 赋值或初始化 */

  /* Restore hsc->RxState to Ready */
  hsc->RxState = HAL_SMARTCARD_STATE_READY;             /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Abort ongoing transfers (Interrupt mode).
  * @param  hsc SMARTCARD handle.
  * @note   This procedure could be used for aborting any ongoing transfer started in Interrupt or DMA mode.
  *         This procedure performs following operations :
  *           - Disable PPP Interrupts
  *           - Disable the DMA transfer in the peripheral register (if enabled)
  *           - Abort DMA transfer by calling HAL_DMA_Abort_IT (in case of transfer in DMA mode)
  *           - Set handle State to READY
  *           - At abort completion, call user abort complete callback
  * @note   This procedure is executed in Interrupt mode, meaning that abort procedure could be
  *         considered as completed only when user abort complete callback is executed (not when exiting function).
  * @retval HAL status
*/
HAL_StatusTypeDef HAL_SMARTCARD_Abort_IT(SMARTCARD_HandleTypeDef *hsc)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t AbortCplt = 0x01U;                           /* 32位无符号变量/函数 */

  /* Disable TXEIE, TCIE, RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts */
  CLEAR_BIT(hsc->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE | USART_CR1_TXEIE | USART_CR1_TCIE));  /* 执行语句 */
  CLEAR_BIT(hsc->Instance->CR3, USART_CR3_EIE);         /* 执行语句 */

  /* If DMA Tx and/or DMA Rx Handles are associated to SMARTCARD Handle, DMA Abort complete callbacks should be initialised
     before any call to DMA Abort functions */
  /* DMA Tx Handle is valid */
  if(hsc->hdmatx != NULL)                               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Set DMA Abort Complete callback if SMARTCARD DMA Tx request if enabled.
       Otherwise, set it to NULL */
    if(HAL_IS_BIT_SET(hsc->Instance->CR3, USART_CR3_DMAT))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hsc->hdmatx->XferAbortCallback = SMARTCARD_DMATxAbortCallback;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hsc->hdmatx->XferAbortCallback = NULL;            /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /* DMA Rx Handle is valid */
  if(hsc->hdmarx != NULL)                               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Set DMA Abort Complete callback if SMARTCARD DMA Rx request if enabled.
       Otherwise, set it to NULL */
    if(HAL_IS_BIT_SET(hsc->Instance->CR3, USART_CR3_DMAR))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hsc->hdmarx->XferAbortCallback = SMARTCARD_DMARxAbortCallback;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hsc->hdmarx->XferAbortCallback = NULL;            /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Disable the SMARTCARD DMA Tx request if enabled */
  if(HAL_IS_BIT_SET(hsc->Instance->CR3, USART_CR3_DMAT))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable DMA Tx at SMARTCARD level */
    CLEAR_BIT(hsc->Instance->CR3, USART_CR3_DMAT);      /* 执行语句 */

    /* Abort the SMARTCARD DMA Tx channel : use non blocking DMA Abort API (callback) */
    if(hsc->hdmatx != NULL)                             /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* SMARTCARD Tx DMA Abort callback has already been initialised :
         will lead to call HAL_SMARTCARD_AbortCpltCallback() at end of DMA abort procedure */

      /* Abort DMA TX */
      if(HAL_DMA_Abort_IT(hsc->hdmatx) != HAL_OK)       /* 条件判断 */
      {                                                 /* 代码块开始 */
        hsc->hdmatx->XferAbortCallback = NULL;          /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        AbortCplt = 0x00U;                              /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Disable the SMARTCARD DMA Rx request if enabled */
  if(HAL_IS_BIT_SET(hsc->Instance->CR3, USART_CR3_DMAR))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    CLEAR_BIT(hsc->Instance->CR3, USART_CR3_DMAR);      /* 执行语句 */

    /* Abort the SMARTCARD DMA Rx channel : use non blocking DMA Abort API (callback) */
    if(hsc->hdmarx != NULL)                             /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* SMARTCARD Rx DMA Abort callback has already been initialised :
         will lead to call HAL_SMARTCARD_AbortCpltCallback() at end of DMA abort procedure */

      /* Abort DMA RX */
      if(HAL_DMA_Abort_IT(hsc->hdmarx) != HAL_OK)       /* 条件判断 */
      {                                                 /* 代码块开始 */
        hsc->hdmarx->XferAbortCallback = NULL;          /* 赋值或初始化 */
        AbortCplt = 0x01U;                              /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        AbortCplt = 0x00U;                              /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* if no DMA abort complete callback execution is required => call user Abort Complete callback */
  if(AbortCplt == 0x01U)                                /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Reset Tx and Rx transfer counters */
    hsc->TxXferCount = 0x00U;                           /* 赋值或初始化 */
    hsc->RxXferCount = 0x00U;                           /* 赋值或初始化 */

    /* Reset ErrorCode */
    hsc->ErrorCode = HAL_SMARTCARD_ERROR_NONE;          /* 赋值或初始化 */

    /* Restore hsc->gState and hsc->RxState to Ready */
    hsc->gState  = HAL_SMARTCARD_STATE_READY;           /* 赋值或初始化 */
    hsc->RxState = HAL_SMARTCARD_STATE_READY;           /* 赋值或初始化 */

    /* As no DMA to be aborted, call directly user Abort complete callback */
#if (USE_HAL_SMARTCARD_REGISTER_CALLBACKS == 1)         /* 条件编译 */
    /* Call registered Abort complete callback */
    hsc->AbortCpltCallback(hsc);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /* Call legacy weak Abort complete callback */
    HAL_SMARTCARD_AbortCpltCallback(hsc);               /* HAL库函数调用 */
#endif /* USE_HAL_SMARTCARD_REGISTER_CALLBACK */
  }                                                     /* 代码块结束 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Abort ongoing Transmit transfer (Interrupt mode).
  * @param  hsc SMARTCARD handle.
  * @note   This procedure could be used for aborting any ongoing transfer started in Interrupt or DMA mode.
  *         This procedure performs following operations :
  *           - Disable SMARTCARD Interrupts (Tx)
  *           - Disable the DMA transfer in the peripheral register (if enabled)
  *           - Abort DMA transfer by calling HAL_DMA_Abort_IT (in case of transfer in DMA mode)
  *           - Set handle State to READY
  *           - At abort completion, call user abort complete callback
  * @note   This procedure is executed in Interrupt mode, meaning that abort procedure could be
  *         considered as completed only when user abort complete callback is executed (not when exiting function).
  * @retval HAL status
*/
HAL_StatusTypeDef HAL_SMARTCARD_AbortTransmit_IT(SMARTCARD_HandleTypeDef *hsc)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Disable TXEIE and TCIE interrupts */
  CLEAR_BIT(hsc->Instance->CR1, (USART_CR1_TXEIE | USART_CR1_TCIE));  /* 执行语句 */

  /* Disable the SMARTCARD DMA Tx request if enabled */
  if(HAL_IS_BIT_SET(hsc->Instance->CR3, USART_CR3_DMAT))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    CLEAR_BIT(hsc->Instance->CR3, USART_CR3_DMAT);      /* 执行语句 */

    /* Abort the SMARTCARD DMA Tx channel : use blocking DMA Abort API (no callback) */
    if(hsc->hdmatx != NULL)                             /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the SMARTCARD DMA Abort callback :
         will lead to call HAL_SMARTCARD_AbortCpltCallback() at end of DMA abort procedure */
      hsc->hdmatx->XferAbortCallback = SMARTCARD_DMATxOnlyAbortCallback;  /* 赋值或初始化 */

      /* Abort DMA TX */
      if(HAL_DMA_Abort_IT(hsc->hdmatx) != HAL_OK)       /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Call Directly hsc->hdmatx->XferAbortCallback function in case of error */
        hsc->hdmatx->XferAbortCallback(hsc->hdmatx);    /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Reset Tx transfer counter */
      hsc->TxXferCount = 0x00U;                         /* 赋值或初始化 */

      /* Restore hsc->gState to Ready */
      hsc->gState = HAL_SMARTCARD_STATE_READY;          /* 赋值或初始化 */

      /* As no DMA to be aborted, call directly user Abort complete callback */
#if (USE_HAL_SMARTCARD_REGISTER_CALLBACKS == 1)         /* 条件编译 */
      /* Call registered Abort Transmit Complete Callback */
      hsc->AbortTransmitCpltCallback(hsc);              /* 执行语句 */
#else                                                   /* 条件编译否则 */
      /* Call legacy weak Abort Transmit Complete Callback */
      HAL_SMARTCARD_AbortTransmitCpltCallback(hsc);     /* HAL库函数调用 */
#endif /* USE_HAL_SMARTCARD_REGISTER_CALLBACK */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Reset Tx transfer counter */
    hsc->TxXferCount = 0x00U;                           /* 赋值或初始化 */

    /* Restore hsc->gState to Ready */
    hsc->gState = HAL_SMARTCARD_STATE_READY;            /* 赋值或初始化 */

    /* As no DMA to be aborted, call directly user Abort complete callback */
#if (USE_HAL_SMARTCARD_REGISTER_CALLBACKS == 1)         /* 条件编译 */
    /* Call registered Abort Transmit Complete Callback */
    hsc->AbortTransmitCpltCallback(hsc);                /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /* Call legacy weak Abort Transmit Complete Callback */
    HAL_SMARTCARD_AbortTransmitCpltCallback(hsc);       /* HAL库函数调用 */
#endif /* USE_HAL_SMARTCARD_REGISTER_CALLBACK */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Abort ongoing Receive transfer (Interrupt mode).
  * @param  hsc SMARTCARD handle.
  * @note   This procedure could be used for aborting any ongoing transfer started in Interrupt or DMA mode.
  *         This procedure performs following operations :
  *           - Disable SMARTCARD Interrupts (Rx)
  *           - Disable the DMA transfer in the peripheral register (if enabled)
  *           - Abort DMA transfer by calling HAL_DMA_Abort_IT (in case of transfer in DMA mode)
  *           - Set handle State to READY
  *           - At abort completion, call user abort complete callback
  * @note   This procedure is executed in Interrupt mode, meaning that abort procedure could be
  *         considered as completed only when user abort complete callback is executed (not when exiting function).
  * @retval HAL status
*/
HAL_StatusTypeDef HAL_SMARTCARD_AbortReceive_IT(SMARTCARD_HandleTypeDef *hsc)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Disable RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts */
  CLEAR_BIT(hsc->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE));  /* 执行语句 */
  CLEAR_BIT(hsc->Instance->CR3, USART_CR3_EIE);         /* 执行语句 */

  /* Disable the SMARTCARD DMA Rx request if enabled */
  if(HAL_IS_BIT_SET(hsc->Instance->CR3, USART_CR3_DMAR))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    CLEAR_BIT(hsc->Instance->CR3, USART_CR3_DMAR);      /* 执行语句 */

    /* Abort the SMARTCARD DMA Rx channel : use blocking DMA Abort API (no callback) */
    if(hsc->hdmarx != NULL)                             /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the SMARTCARD DMA Abort callback :
         will lead to call HAL_SMARTCARD_AbortCpltCallback() at end of DMA abort procedure */
      hsc->hdmarx->XferAbortCallback = SMARTCARD_DMARxOnlyAbortCallback;  /* 赋值或初始化 */

      /* Abort DMA RX */
      if(HAL_DMA_Abort_IT(hsc->hdmarx) != HAL_OK)       /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Call Directly hsc->hdmarx->XferAbortCallback function in case of error */
        hsc->hdmarx->XferAbortCallback(hsc->hdmarx);    /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Reset Rx transfer counter */
      hsc->RxXferCount = 0x00U;                         /* 赋值或初始化 */

      /* Restore hsc->RxState to Ready */
      hsc->RxState = HAL_SMARTCARD_STATE_READY;         /* 赋值或初始化 */

      /* As no DMA to be aborted, call directly user Abort complete callback */
#if (USE_HAL_SMARTCARD_REGISTER_CALLBACKS == 1)         /* 条件编译 */
      /* Call registered Abort Receive Complete Callback */
      hsc->AbortReceiveCpltCallback(hsc);               /* 执行语句 */
#else                                                   /* 条件编译否则 */
      /* Call legacy weak Abort Receive Complete Callback */
      HAL_SMARTCARD_AbortReceiveCpltCallback(hsc);      /* HAL库函数调用 */
#endif /* USE_HAL_SMARTCARD_REGISTER_CALLBACK */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Reset Rx transfer counter */
    hsc->RxXferCount = 0x00U;                           /* 赋值或初始化 */

    /* Restore hsc->RxState to Ready */
    hsc->RxState = HAL_SMARTCARD_STATE_READY;           /* 赋值或初始化 */

    /* As no DMA to be aborted, call directly user Abort complete callback */
#if (USE_HAL_SMARTCARD_REGISTER_CALLBACKS == 1)         /* 条件编译 */
    /* Call registered Abort Receive Complete Callback */
    hsc->AbortReceiveCpltCallback(hsc);                 /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /* Call legacy weak Abort Receive Complete Callback */
    HAL_SMARTCARD_AbortReceiveCpltCallback(hsc);        /* HAL库函数调用 */
#endif /* USE_HAL_SMARTCARD_REGISTER_CALLBACK */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief This function handles SMARTCARD interrupt request.
  * @param  hsc    Pointer to a SMARTCARD_HandleTypeDef structure that contains
  *                the configuration information for SMARTCARD module.
  * @retval None
  */
void HAL_SMARTCARD_IRQHandler(SMARTCARD_HandleTypeDef *hsc)  /* 中断服务函数 */
{                                                       /* 代码块开始 */
  uint32_t isrflags   = READ_REG(hsc->Instance->SR);    /* 32位无符号变量/函数 */
  uint32_t cr1its     = READ_REG(hsc->Instance->CR1);   /* 32位无符号变量/函数 */
  uint32_t cr3its     = READ_REG(hsc->Instance->CR3);   /* 32位无符号变量/函数 */
  uint32_t dmarequest = 0x00U;                          /* 32位无符号变量/函数 */
  uint32_t errorflags = 0x00U;                          /* 32位无符号变量/函数 */

  /* If no error occurs */
  errorflags = (isrflags & (uint32_t)(USART_SR_PE | USART_SR_FE | USART_SR_ORE | USART_SR_NE));  /* 赋值或初始化 */
  if(errorflags == RESET)                               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* SMARTCARD in mode Receiver -------------------------------------------------*/
    if(((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      SMARTCARD_Receive_IT(hsc);                        /* 执行语句 */
      return;                                           /* 返回 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* If some errors occur */
  if((errorflags != RESET) && (((cr3its & USART_CR3_EIE) != RESET) || ((cr1its & (USART_CR1_RXNEIE | USART_CR1_PEIE)) != RESET)))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* SMARTCARD parity error interrupt occurred ---------------------------*/
    if(((isrflags & SMARTCARD_FLAG_PE) != RESET) && ((cr1its & USART_CR1_PEIE) != RESET))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hsc->ErrorCode |= HAL_SMARTCARD_ERROR_PE;         /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* SMARTCARD frame error interrupt occurred ----------------------------*/
    if(((isrflags & SMARTCARD_FLAG_FE) != RESET) && ((cr3its & USART_CR3_EIE) != RESET))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hsc->ErrorCode |= HAL_SMARTCARD_ERROR_FE;         /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* SMARTCARD noise error interrupt occurred ----------------------------*/
    if(((isrflags & SMARTCARD_FLAG_NE) != RESET) && ((cr3its & USART_CR3_EIE) != RESET))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hsc->ErrorCode |= HAL_SMARTCARD_ERROR_NE;         /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* SMARTCARD Over-Run interrupt occurred -------------------------------*/
    if(((isrflags & SMARTCARD_FLAG_ORE) != RESET) && (((cr1its & USART_CR1_RXNEIE) != RESET) || ((cr3its & USART_CR3_EIE) != RESET)))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hsc->ErrorCode |= HAL_SMARTCARD_ERROR_ORE;        /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    /* Call the Error call Back in case of Errors --------------------------*/
    if(hsc->ErrorCode != HAL_SMARTCARD_ERROR_NONE)      /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* SMARTCARD in mode Receiver ----------------------------------------*/
      if(((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        SMARTCARD_Receive_IT(hsc);                      /* 执行语句 */
      }                                                 /* 代码块结束 */

      /* If Overrun error occurs, or if any error occurs in DMA mode reception,
         consider error as blocking */
      dmarequest = HAL_IS_BIT_SET(hsc->Instance->CR3, USART_CR3_DMAR);  /* 赋值或初始化 */
      if(((hsc->ErrorCode & HAL_SMARTCARD_ERROR_ORE) != RESET) || dmarequest)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Blocking error : transfer is aborted
          Set the SMARTCARD state ready to be able to start again the process,
          Disable Rx Interrupts, and disable Rx DMA request, if ongoing */
        SMARTCARD_EndRxTransfer(hsc);                   /* 执行语句 */
        /* Disable the SMARTCARD DMA Rx request if enabled */
        if(HAL_IS_BIT_SET(hsc->Instance->CR3, USART_CR3_DMAR))  /* 条件判断 */
        {                                               /* 代码块开始 */
          CLEAR_BIT(hsc->Instance->CR3, USART_CR3_DMAR);  /* 执行语句 */

          /* Abort the SMARTCARD DMA Rx channel */
          if(hsc->hdmarx != NULL)                       /* 条件判断 */
          {                                             /* 代码块开始 */
            /* Set the SMARTCARD DMA Abort callback :
              will lead to call HAL_SMARTCARD_ErrorCallback() at end of DMA abort procedure */
            hsc->hdmarx->XferAbortCallback = SMARTCARD_DMAAbortOnError;  /* 赋值或初始化 */

           if(HAL_DMA_Abort_IT(hsc->hdmarx) != HAL_OK)  /* 条件判断 */
            {                                           /* 代码块开始 */
              /* Call Directly XferAbortCallback function in case of error */
              hsc->hdmarx->XferAbortCallback(hsc->hdmarx);  /* 执行语句 */
            }                                           /* 代码块结束 */
          }                                             /* 代码块结束 */
          else                                          /* 否则 */
          {                                             /* 代码块开始 */
#if (USE_HAL_SMARTCARD_REGISTER_CALLBACKS == 1)         /* 条件编译 */
            /* Call registered user error callback */
            hsc->ErrorCallback(hsc);                    /* 执行语句 */
#else                                                   /* 条件编译否则 */
            /* Call legacy weak user error callback */
            HAL_SMARTCARD_ErrorCallback(hsc);           /* HAL库函数调用 */
#endif /* USE_HAL_SMARTCARD_REGISTER_CALLBACK */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
#if (USE_HAL_SMARTCARD_REGISTER_CALLBACKS == 1)         /* 条件编译 */
          /* Call registered user error callback */
          hsc->ErrorCallback(hsc);                      /* 执行语句 */
#else                                                   /* 条件编译否则 */
          /* Call legacy weak user error callback */
          HAL_SMARTCARD_ErrorCallback(hsc);             /* HAL库函数调用 */
#endif /* USE_HAL_SMARTCARD_REGISTER_CALLBACK */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Non Blocking error : transfer could go on.
           Error is notified to user through user error callback */
#if (USE_HAL_SMARTCARD_REGISTER_CALLBACKS == 1)         /* 条件编译 */
        /* Call registered user error callback */
        hsc->ErrorCallback(hsc);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
        /* Call legacy weak user error callback */
        HAL_SMARTCARD_ErrorCallback(hsc);               /* HAL库函数调用 */
#endif /* USE_HAL_SMARTCARD_REGISTER_CALLBACK */
        hsc->ErrorCode = HAL_SMARTCARD_ERROR_NONE;      /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    return;                                             /* 返回 */
  } /* End if some error occurs */

  /* SMARTCARD in mode Transmitter ------------------------------------------*/
  if(((isrflags & SMARTCARD_FLAG_TXE) != RESET) && ((cr1its & USART_CR1_TXEIE) != RESET))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    SMARTCARD_Transmit_IT(hsc);                         /* 执行语句 */
    return;                                             /* 返回 */
  }                                                     /* 代码块结束 */

  /* SMARTCARD in mode Transmitter (transmission end) -----------------------*/
  if(((isrflags & SMARTCARD_FLAG_TC) != RESET) && ((cr1its & USART_CR1_TCIE) != RESET))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    SMARTCARD_EndTransmit_IT(hsc);                      /* 执行语句 */
    return;                                             /* 返回 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief Tx Transfer completed callbacks
  * @param  hsc    Pointer to a SMARTCARD_HandleTypeDef structure that contains
  *                the configuration information for SMARTCARD module.
  * @retval None
  */
__weak void HAL_SMARTCARD_TxCpltCallback(SMARTCARD_HandleTypeDef *hsc)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsc);                                          /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SMARTCARD_TxCpltCallback can be implemented in the user file.
   */
}                                                       /* 代码块结束 */

/**
  * @brief Rx Transfer completed callback
  * @param  hsc    Pointer to a SMARTCARD_HandleTypeDef structure that contains
  *                the configuration information for SMARTCARD module.
  * @retval None
  */
__weak void HAL_SMARTCARD_RxCpltCallback(SMARTCARD_HandleTypeDef *hsc)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsc);                                          /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SMARTCARD_RxCpltCallback can be implemented in the user file.
   */
}                                                       /* 代码块结束 */

/**
  * @brief SMARTCARD error callback
  * @param  hsc    Pointer to a SMARTCARD_HandleTypeDef structure that contains
  *                the configuration information for SMARTCARD module.
  * @retval None
  */
__weak void HAL_SMARTCARD_ErrorCallback(SMARTCARD_HandleTypeDef *hsc)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsc);                                          /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SMARTCARD_ErrorCallback can be implemented in the user file.
   */
}                                                       /* 代码块结束 */

/**
  * @brief  SMARTCARD Abort Complete callback.
  * @param  hsc SMARTCARD handle.
  * @retval None
  */
__weak void HAL_SMARTCARD_AbortCpltCallback (SMARTCARD_HandleTypeDef *hsc)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsc);                                          /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SMARTCARD_AbortCpltCallback can be implemented in the user file.
   */
}                                                       /* 代码块结束 */

/**
  * @brief  SMARTCARD Abort Transmit Complete callback.
  * @param  hsc SMARTCARD handle.
  * @retval None
  */
__weak void HAL_SMARTCARD_AbortTransmitCpltCallback (SMARTCARD_HandleTypeDef *hsc)  /* 源代码 */
{                                                       /* 代码块开始 */
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hsc);                                        /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SMARTCARD_AbortTransmitCpltCallback can be implemented in the user file.
   */
}                                                       /* 代码块结束 */

/**
  * @brief  SMARTCARD Abort Receive Complete callback.
  * @param  hsc SMARTCARD handle.
  * @retval None
  */
__weak void HAL_SMARTCARD_AbortReceiveCpltCallback (SMARTCARD_HandleTypeDef *hsc)  /* 源代码 */
{                                                       /* 代码块开始 */
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hsc);                                        /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SMARTCARD_AbortReceiveCpltCallback can be implemented in the user file.
   */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup SMARTCARD_Exported_Functions_Group3 Peripheral State and Errors functions
  *  @brief   SMARTCARD State and Errors functions
  *
@verbatim
 ===============================================================================
                ##### Peripheral State and Errors functions #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to control the SmartCard.
     (+) HAL_SMARTCARD_GetState() API can be helpful to check in run-time the state of the SmartCard peripheral.
     (+) HAL_SMARTCARD_GetError() check in run-time errors that could be occurred during communication.
@endverbatim
  * @{
  */

/**
  * @brief Return the SMARTCARD handle state
  * @param  hsc    Pointer to a SMARTCARD_HandleTypeDef structure that contains
  *                the configuration information for SMARTCARD module.
  * @retval HAL state
  */
HAL_SMARTCARD_StateTypeDef HAL_SMARTCARD_GetState(SMARTCARD_HandleTypeDef *hsc)  /* HAL库函数调用 */
{                                                       /* 代码块开始 */
  uint32_t temp1= 0x00U, temp2 = 0x00U;                 /* 32位无符号变量/函数 */
  temp1 = hsc->gState;                                  /* 赋值或初始化 */
  temp2 = hsc->RxState;                                 /* 赋值或初始化 */

  return (HAL_SMARTCARD_StateTypeDef)(temp1 | temp2);   /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return the SMARTCARD error code
  * @param  hsc  Pointer to a SMARTCARD_HandleTypeDef structure that contains
  *              the configuration information for the specified SMARTCARD.
  * @retval SMARTCARD Error Code
  */
uint32_t HAL_SMARTCARD_GetError(SMARTCARD_HandleTypeDef *hsc)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  return hsc->ErrorCode;                                /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup SMARTCARD_Private_Functions SMARTCARD Private Functions
  * @{
  */

#if (USE_HAL_SMARTCARD_REGISTER_CALLBACKS == 1)         /* 条件编译 */
/**
  * @brief  Initialize the callbacks to their default values.
  * @param  hsc SMARTCARD handle.
  * @retval none
  */
void SMARTCARD_InitCallbacksToDefault(SMARTCARD_HandleTypeDef *hsc)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Init the SMARTCARD Callback settings */
  hsc->TxCpltCallback            = HAL_SMARTCARD_TxCpltCallback;            /* Legacy weak TxCpltCallback            */
  hsc->RxCpltCallback            = HAL_SMARTCARD_RxCpltCallback;            /* Legacy weak RxCpltCallback            */
  hsc->ErrorCallback             = HAL_SMARTCARD_ErrorCallback;             /* Legacy weak ErrorCallback             */
  hsc->AbortCpltCallback         = HAL_SMARTCARD_AbortCpltCallback;         /* Legacy weak AbortCpltCallback         */
  hsc->AbortTransmitCpltCallback = HAL_SMARTCARD_AbortTransmitCpltCallback; /* Legacy weak AbortTransmitCpltCallback */
  hsc->AbortReceiveCpltCallback  = HAL_SMARTCARD_AbortReceiveCpltCallback;  /* Legacy weak AbortReceiveCpltCallback  */

}                                                       /* 代码块结束 */
#endif /* USE_HAL_SMARTCARD_REGISTER_CALLBACKS */

/**
  * @brief DMA SMARTCARD transmit process complete callback
  * @param  hdma   Pointer to a DMA_HandleTypeDef structure that contains
  *                the configuration information for the specified DMA module.
  * @retval None
  */
static void SMARTCARD_DMATransmitCplt(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  SMARTCARD_HandleTypeDef* hsc = ( SMARTCARD_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;  /* 赋值或初始化 */

  hsc->TxXferCount = 0U;                                /* 赋值或初始化 */

  /* Disable the DMA transfer for transmit request by setting the DMAT bit
     in the USART CR3 register */
  CLEAR_BIT(hsc->Instance->CR3, USART_CR3_DMAT);        /* 执行语句 */

  /* Enable the SMARTCARD Transmit Complete Interrupt */
  SET_BIT(hsc->Instance->CR1, USART_CR1_TCIE);          /* 执行语句 */
}                                                       /* 代码块结束 */

/**
  * @brief DMA SMARTCARD receive process complete callback
  * @param  hdma   Pointer to a DMA_HandleTypeDef structure that contains
  *                the configuration information for the specified DMA module.
  * @retval None
  */
static void SMARTCARD_DMAReceiveCplt(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  SMARTCARD_HandleTypeDef* hsc = ( SMARTCARD_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;  /* 赋值或初始化 */

  hsc->RxXferCount = 0U;                                /* 赋值或初始化 */

  /* Disable RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts */
  CLEAR_BIT(hsc->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE));  /* 执行语句 */
  CLEAR_BIT(hsc->Instance->CR3, USART_CR3_EIE);         /* 执行语句 */

  /* Disable the DMA transfer for the receiver request by setting the DMAR bit
     in the USART CR3 register */
  CLEAR_BIT(hsc->Instance->CR3, USART_CR3_DMAR);        /* 执行语句 */

  /* At end of Rx process, restore hsc->RxState to Ready */
  hsc->RxState = HAL_SMARTCARD_STATE_READY;             /* 赋值或初始化 */

#if (USE_HAL_SMARTCARD_REGISTER_CALLBACKS == 1)         /* 条件编译 */
  /* Call registered Rx complete callback */
  hsc->RxCpltCallback(hsc);                             /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* Call legacy weak Rx complete callback */
  HAL_SMARTCARD_RxCpltCallback(hsc);                    /* HAL库函数调用 */
#endif /* USE_HAL_SMARTCARD_REGISTER_CALLBACK */
}                                                       /* 代码块结束 */

/**
  * @brief DMA SMARTCARD communication error callback
  * @param  hdma   Pointer to a DMA_HandleTypeDef structure that contains
  *                the configuration information for the specified DMA module.
  * @retval None
  */
static void SMARTCARD_DMAError(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  uint32_t dmarequest = 0x00U;                          /* 32位无符号变量/函数 */
  SMARTCARD_HandleTypeDef* hsc = ( SMARTCARD_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;  /* 赋值或初始化 */
  hsc->RxXferCount = 0U;                                /* 赋值或初始化 */
  hsc->TxXferCount = 0U;                                /* 赋值或初始化 */
  hsc->ErrorCode = HAL_SMARTCARD_ERROR_DMA;             /* 赋值或初始化 */

  /* Stop SMARTCARD DMA Tx request if ongoing */
  dmarequest = HAL_IS_BIT_SET(hsc->Instance->CR3, USART_CR3_DMAT);  /* 赋值或初始化 */
  if((hsc->gState == HAL_SMARTCARD_STATE_BUSY_TX) && dmarequest)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    SMARTCARD_EndTxTransfer(hsc);                       /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Stop SMARTCARD DMA Rx request if ongoing */
  dmarequest = HAL_IS_BIT_SET(hsc->Instance->CR3, USART_CR3_DMAR);  /* 赋值或初始化 */
  if((hsc->RxState == HAL_SMARTCARD_STATE_BUSY_RX) && dmarequest)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    SMARTCARD_EndRxTransfer(hsc);                       /* 执行语句 */
  }                                                     /* 代码块结束 */

#if (USE_HAL_SMARTCARD_REGISTER_CALLBACKS == 1)         /* 条件编译 */
  /* Call registered user error callback */
  hsc->ErrorCallback(hsc);                              /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* Call legacy weak user error callback */
  HAL_SMARTCARD_ErrorCallback(hsc);                     /* HAL库函数调用 */
#endif /* USE_HAL_SMARTCARD_REGISTER_CALLBACK */
}                                                       /* 代码块结束 */

/**
  * @brief  This function handles SMARTCARD Communication Timeout.
  * @param  hsc    Pointer to a SMARTCARD_HandleTypeDef structure that contains
  *                the configuration information for SMARTCARD module.
  * @param  Flag   Specifies the SMARTCARD flag to check.
  * @param  Status The new Flag status (SET or RESET).
  * @param  Timeout Timeout duration
  * @param  Tickstart Tick start value
  * @retval HAL status
  */
static HAL_StatusTypeDef SMARTCARD_WaitOnFlagUntilTimeout(SMARTCARD_HandleTypeDef *hsc, uint32_t Flag, FlagStatus Status, uint32_t Tickstart, uint32_t Timeout)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  /* Wait until flag is set */
  while((__HAL_SMARTCARD_GET_FLAG(hsc, Flag) ? SET : RESET) == Status)  /* while循环 */
  {                                                     /* 代码块开始 */
    /* Check for the Timeout */
    if(Timeout != HAL_MAX_DELAY)                        /* 条件判断 */
    {                                                   /* 代码块开始 */
      if((Timeout == 0U)||((HAL_GetTick() - Tickstart ) > Timeout))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Disable TXE and RXNE interrupts for the interrupt process */
        CLEAR_BIT(hsc->Instance->CR1, USART_CR1_TXEIE);  /* 执行语句 */
        CLEAR_BIT(hsc->Instance->CR1, USART_CR1_RXNEIE);  /* 执行语句 */

        hsc->gState= HAL_SMARTCARD_STATE_READY;         /* 赋值或初始化 */
        hsc->RxState= HAL_SMARTCARD_STATE_READY;        /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hsc);                              /* HAL辅助宏/函数 */

        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  End ongoing Tx transfer on SMARTCARD peripheral (following error detection or Transmit completion).
  * @param  hsc    Pointer to a SMARTCARD_HandleTypeDef structure that contains
  *                the configuration information for SMARTCARD module.
  * @retval None
  */
static void SMARTCARD_EndTxTransfer(SMARTCARD_HandleTypeDef *hsc)  /* 静态函数 */
{                                                       /* 代码块开始 */
  /* At end of Tx process, restore hsc->gState to Ready */
  hsc->gState = HAL_SMARTCARD_STATE_READY;              /* 赋值或初始化 */

  /* Disable TXEIE and TCIE interrupts */
  CLEAR_BIT(hsc->Instance->CR1, (USART_CR1_TXEIE | USART_CR1_TCIE));  /* 执行语句 */
}                                                       /* 代码块结束 */


/**
  * @brief  End ongoing Rx transfer on SMARTCARD peripheral (following error detection or Reception completion).
  * @param  hsc    Pointer to a SMARTCARD_HandleTypeDef structure that contains
  *                the configuration information for SMARTCARD module.
  * @retval None
  */
static void SMARTCARD_EndRxTransfer(SMARTCARD_HandleTypeDef *hsc)  /* 静态函数 */
{                                                       /* 代码块开始 */
  /* At end of Rx process, restore hsc->RxState to Ready */
  hsc->RxState = HAL_SMARTCARD_STATE_READY;             /* 赋值或初始化 */

  /* Disable RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts */
  CLEAR_BIT(hsc->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE));  /* 执行语句 */
  CLEAR_BIT(hsc->Instance->CR3, USART_CR3_EIE);         /* 执行语句 */
}                                                       /* 代码块结束 */

/**
  * @brief Send an amount of data in non blocking mode
  * @param  hsc    Pointer to a SMARTCARD_HandleTypeDef structure that contains
  *                the configuration information for SMARTCARD module.
  * @retval HAL status
  */
static HAL_StatusTypeDef SMARTCARD_Transmit_IT(SMARTCARD_HandleTypeDef *hsc)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */

  /* Check that a Tx process is ongoing */
  if(hsc->gState == HAL_SMARTCARD_STATE_BUSY_TX)        /* 条件判断 */
  {                                                     /* 代码块开始 */
    hsc->Instance->DR = (uint8_t)(*hsc->pTxBuffPtr & 0xFFU);  /* 赋值或初始化 */
    hsc->pTxBuffPtr++;                                  /* 执行语句 */

    if(--hsc->TxXferCount == 0U)                        /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable the SMARTCARD Transmit data register empty Interrupt */
      CLEAR_BIT(hsc->Instance->CR1, USART_CR1_TXEIE);   /* 执行语句 */

      /* Enable the SMARTCARD Transmit Complete Interrupt */
      SET_BIT(hsc->Instance->CR1, USART_CR1_TCIE);      /* 执行语句 */
    }                                                   /* 代码块结束 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Wraps up transmission in non blocking mode.
  * @param  hsc    Pointer to a SMARTCARD_HandleTypeDef structure that contains
  *                the configuration information for the specified SMARTCARD module.
  * @retval HAL status
  */
static HAL_StatusTypeDef SMARTCARD_EndTransmit_IT(SMARTCARD_HandleTypeDef *hsc)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  /* Disable the SMARTCARD Transmit Complete Interrupt */
  CLEAR_BIT(hsc->Instance->CR1, USART_CR1_TCIE);        /* 执行语句 */

  /* Disable the SMARTCARD Error Interrupt: (Frame error, noise error, overrun error) */
  CLEAR_BIT(hsc->Instance->CR3, USART_CR3_EIE);         /* 执行语句 */

  /* Tx process is ended, restore hsc->gState to Ready */
  hsc->gState = HAL_SMARTCARD_STATE_READY;              /* 赋值或初始化 */

#if (USE_HAL_SMARTCARD_REGISTER_CALLBACKS == 1)         /* 条件编译 */
  /* Call registered Tx complete callback */
  hsc->TxCpltCallback(hsc);                             /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* Call legacy weak Tx complete callback */
  HAL_SMARTCARD_TxCpltCallback(hsc);                    /* HAL库函数调用 */
#endif /* USE_HAL_SMARTCARD_REGISTER_CALLBACK */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief Receive an amount of data in non blocking mode
  * @param  hsc    Pointer to a SMARTCARD_HandleTypeDef structure that contains
  *                the configuration information for SMARTCARD module.
  * @retval HAL status
  */
static HAL_StatusTypeDef SMARTCARD_Receive_IT(SMARTCARD_HandleTypeDef *hsc)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */

  /* Check that a Rx process is ongoing */
  if(hsc->RxState == HAL_SMARTCARD_STATE_BUSY_RX)       /* 条件判断 */
  {                                                     /* 代码块开始 */
    *hsc->pRxBuffPtr = (uint8_t)(hsc->Instance->DR & (uint8_t)0xFFU);  /* 赋值或初始化 */
    hsc->pRxBuffPtr++;                                  /* 执行语句 */

    if(--hsc->RxXferCount == 0U)                        /* 条件判断 */
    {                                                   /* 代码块开始 */
      CLEAR_BIT(hsc->Instance->CR1, USART_CR1_RXNEIE);  /* 执行语句 */

      /* Disable the SMARTCARD Parity Error Interrupt */
      CLEAR_BIT(hsc->Instance->CR1, USART_CR1_PEIE);    /* 执行语句 */

      /* Disable the SMARTCARD Error Interrupt: (Frame error, noise error, overrun error) */
      CLEAR_BIT(hsc->Instance->CR3, USART_CR3_EIE);     /* 执行语句 */

      /* Rx process is completed, restore hsc->RxState to Ready */
      hsc->RxState = HAL_SMARTCARD_STATE_READY;         /* 赋值或初始化 */

#if (USE_HAL_SMARTCARD_REGISTER_CALLBACKS == 1)         /* 条件编译 */
      /* Call registered Rx complete callback */
      hsc->RxCpltCallback(hsc);                         /* 执行语句 */
#else                                                   /* 条件编译否则 */
      /* Call legacy weak Rx complete callback */
      HAL_SMARTCARD_RxCpltCallback(hsc);                /* HAL库函数调用 */
#endif /* USE_HAL_SMARTCARD_REGISTER_CALLBACK */

      return HAL_OK;                                    /* 返回值 */
    }                                                   /* 代码块结束 */
    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA SMARTCARD communication abort callback, when initiated by HAL services on Error
  *         (To be called at end of DMA Abort procedure following error occurrence).
  * @param  hdma DMA handle.
  * @retval None
  */
static void SMARTCARD_DMAAbortOnError(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  SMARTCARD_HandleTypeDef* hsc = (SMARTCARD_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;  /* 赋值或初始化 */
  hsc->RxXferCount = 0x00U;                             /* 赋值或初始化 */
  hsc->TxXferCount = 0x00U;                             /* 赋值或初始化 */

#if (USE_HAL_SMARTCARD_REGISTER_CALLBACKS == 1)         /* 条件编译 */
  /* Call registered user error callback */
  hsc->ErrorCallback(hsc);                              /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* Call legacy weak user error callback */
  HAL_SMARTCARD_ErrorCallback(hsc);                     /* HAL库函数调用 */
#endif /* USE_HAL_SMARTCARD_REGISTER_CALLBACK */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA SMARTCARD Tx communication abort callback, when initiated by user
  *         (To be called at end of DMA Tx Abort procedure following user abort request).
  * @note   When this callback is executed, User Abort complete call back is called only if no
  *         Abort still ongoing for Rx DMA Handle.
  * @param  hdma DMA handle.
  * @retval None
  */
static void SMARTCARD_DMATxAbortCallback(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  SMARTCARD_HandleTypeDef* hsc = ( SMARTCARD_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;  /* 赋值或初始化 */

  hsc->hdmatx->XferAbortCallback = NULL;                /* 赋值或初始化 */

  /* Check if an Abort process is still ongoing */
  if(hsc->hdmarx != NULL)                               /* 条件判断 */
  {                                                     /* 代码块开始 */
    if(hsc->hdmarx->XferAbortCallback != NULL)          /* 条件判断 */
    {                                                   /* 代码块开始 */
      return;                                           /* 返回 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* No Abort process still ongoing : All DMA channels are aborted, call user Abort Complete callback */
  hsc->TxXferCount = 0x00U;                             /* 赋值或初始化 */
  hsc->RxXferCount = 0x00U;                             /* 赋值或初始化 */

  /* Reset ErrorCode */
  hsc->ErrorCode = HAL_SMARTCARD_ERROR_NONE;            /* 赋值或初始化 */

  /* Restore hsc->gState and hsc->RxState to Ready */
  hsc->gState  = HAL_SMARTCARD_STATE_READY;             /* 赋值或初始化 */
  hsc->RxState = HAL_SMARTCARD_STATE_READY;             /* 赋值或初始化 */

#if (USE_HAL_SMARTCARD_REGISTER_CALLBACKS == 1)         /* 条件编译 */
  /* Call registered Abort complete callback */
  hsc->AbortCpltCallback(hsc);                          /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* Call legacy weak Abort complete callback */
  HAL_SMARTCARD_AbortCpltCallback(hsc);                 /* HAL库函数调用 */
#endif /* USE_HAL_SMARTCARD_REGISTER_CALLBACK */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA SMARTCARD Rx communication abort callback, when initiated by user
  *         (To be called at end of DMA Rx Abort procedure following user abort request).
  * @note   When this callback is executed, User Abort complete call back is called only if no
  *         Abort still ongoing for Tx DMA Handle.
  * @param  hdma DMA handle.
  * @retval None
  */
static void SMARTCARD_DMARxAbortCallback(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  SMARTCARD_HandleTypeDef* hsc = ( SMARTCARD_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;  /* 赋值或初始化 */

  hsc->hdmarx->XferAbortCallback = NULL;                /* 赋值或初始化 */

  /* Check if an Abort process is still ongoing */
  if(hsc->hdmatx != NULL)                               /* 条件判断 */
  {                                                     /* 代码块开始 */
    if(hsc->hdmatx->XferAbortCallback != NULL)          /* 条件判断 */
    {                                                   /* 代码块开始 */
      return;                                           /* 返回 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* No Abort process still ongoing : All DMA channels are aborted, call user Abort Complete callback */
  hsc->TxXferCount = 0x00U;                             /* 赋值或初始化 */
  hsc->RxXferCount = 0x00U;                             /* 赋值或初始化 */

  /* Reset ErrorCode */
  hsc->ErrorCode = HAL_SMARTCARD_ERROR_NONE;            /* 赋值或初始化 */

  /* Restore hsc->gState and hsc->RxState to Ready */
  hsc->gState  = HAL_SMARTCARD_STATE_READY;             /* 赋值或初始化 */
  hsc->RxState = HAL_SMARTCARD_STATE_READY;             /* 赋值或初始化 */

#if (USE_HAL_SMARTCARD_REGISTER_CALLBACKS == 1)         /* 条件编译 */
  /* Call registered Abort complete callback */
  hsc->AbortCpltCallback(hsc);                          /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* Call legacy weak Abort complete callback */
  HAL_SMARTCARD_AbortCpltCallback(hsc);                 /* HAL库函数调用 */
#endif /* USE_HAL_SMARTCARD_REGISTER_CALLBACK */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA SMARTCARD Tx communication abort callback, when initiated by user by a call to
  *         HAL_SMARTCARD_AbortTransmit_IT API (Abort only Tx transfer)
  *         (This callback is executed at end of DMA Tx Abort procedure following user abort request,
  *         and leads to user Tx Abort Complete callback execution).
  * @param  hdma DMA handle.
  * @retval None
  */
static void SMARTCARD_DMATxOnlyAbortCallback(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  SMARTCARD_HandleTypeDef* hsc = ( SMARTCARD_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;  /* 赋值或初始化 */

  hsc->TxXferCount = 0x00U;                             /* 赋值或初始化 */

  /* Restore hsc->gState to Ready */
  hsc->gState = HAL_SMARTCARD_STATE_READY;              /* 赋值或初始化 */

#if (USE_HAL_SMARTCARD_REGISTER_CALLBACKS == 1)         /* 条件编译 */
  /* Call registered Abort Transmit Complete Callback */
  hsc->AbortTransmitCpltCallback(hsc);                  /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* Call legacy weak Abort Transmit Complete Callback */
  HAL_SMARTCARD_AbortTransmitCpltCallback(hsc);         /* HAL库函数调用 */
#endif /* USE_HAL_SMARTCARD_REGISTER_CALLBACK */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA SMARTCARD Rx communication abort callback, when initiated by user by a call to
  *         HAL_SMARTCARD_AbortReceive_IT API (Abort only Rx transfer)
  *         (This callback is executed at end of DMA Rx Abort procedure following user abort request,
  *         and leads to user Rx Abort Complete callback execution).
  * @param  hdma DMA handle.
  * @retval None
  */
static void SMARTCARD_DMARxOnlyAbortCallback(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  SMARTCARD_HandleTypeDef* hsc = ( SMARTCARD_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;  /* 赋值或初始化 */

  hsc->RxXferCount = 0x00U;                             /* 赋值或初始化 */

  /* Restore hsc->RxState to Ready */
  hsc->RxState = HAL_SMARTCARD_STATE_READY;             /* 赋值或初始化 */

#if (USE_HAL_SMARTCARD_REGISTER_CALLBACKS == 1)         /* 条件编译 */
  /* Call registered Abort Receive Complete Callback */
  hsc->AbortReceiveCpltCallback(hsc);                   /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* Call legacy weak Abort Receive Complete Callback */
  HAL_SMARTCARD_AbortReceiveCpltCallback(hsc);          /* HAL库函数调用 */
#endif /* USE_HAL_SMARTCARD_REGISTER_CALLBACK */
}                                                       /* 代码块结束 */

/**
  * @brief Configure the SMARTCARD peripheral
  * @param  hsc    Pointer to a SMARTCARD_HandleTypeDef structure that contains
  *                the configuration information for SMARTCARD module.
  * @retval None
  */
static void SMARTCARD_SetConfig(SMARTCARD_HandleTypeDef *hsc)  /* 静态函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpreg = 0x00U;                              /* 32位无符号变量/函数 */
  uint32_t pclk;                                        /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_SMARTCARD_INSTANCE(hsc->Instance));   /* 执行语句 */
  assert_param(IS_SMARTCARD_POLARITY(hsc->Init.CLKPolarity));  /* 执行语句 */
  assert_param(IS_SMARTCARD_PHASE(hsc->Init.CLKPhase));  /* 执行语句 */
  assert_param(IS_SMARTCARD_LASTBIT(hsc->Init.CLKLastBit));  /* 执行语句 */
  assert_param(IS_SMARTCARD_BAUDRATE(hsc->Init.BaudRate));  /* 执行语句 */
  assert_param(IS_SMARTCARD_WORD_LENGTH(hsc->Init.WordLength));  /* 执行语句 */
  assert_param(IS_SMARTCARD_STOPBITS(hsc->Init.StopBits));  /* 执行语句 */
  assert_param(IS_SMARTCARD_PARITY(hsc->Init.Parity));  /* 执行语句 */
  assert_param(IS_SMARTCARD_MODE(hsc->Init.Mode));      /* 执行语句 */
  assert_param(IS_SMARTCARD_NACK_STATE(hsc->Init.NACKState));  /* 执行语句 */

  /* The LBCL, CPOL and CPHA bits have to be selected when both the transmitter and the
     receiver are disabled (TE=RE=0) to ensure that the clock pulses function correctly. */
  CLEAR_BIT(hsc->Instance->CR1, (USART_CR1_TE | USART_CR1_RE));  /* 执行语句 */

  /*---------------------------- USART CR2 Configuration ---------------------*/
  tmpreg = hsc->Instance->CR2;                          /* 赋值或初始化 */
  /* Clear CLKEN, CPOL, CPHA and LBCL bits */
  tmpreg &= (uint32_t)~((uint32_t)(USART_CR2_CPHA | USART_CR2_CPOL | USART_CR2_CLKEN | USART_CR2_LBCL));  /* 赋值或初始化 */
  /* Configure the SMARTCARD Clock, CPOL, CPHA and LastBit -----------------------*/
  /* Set CPOL bit according to hsc->Init.CLKPolarity value */
  /* Set CPHA bit according to hsc->Init.CLKPhase value */
  /* Set LBCL bit according to hsc->Init.CLKLastBit value */
  /* Set Stop Bits: Set STOP[13:12] bits according to hsc->Init.StopBits value */
  tmpreg |= (uint32_t)(USART_CR2_CLKEN | hsc->Init.CLKPolarity |  /* 源代码 */
                      hsc->Init.CLKPhase| hsc->Init.CLKLastBit | hsc->Init.StopBits);  /* 执行语句 */
  /* Write to USART CR2 */
  WRITE_REG(hsc->Instance->CR2, (uint32_t)tmpreg);      /* 执行语句 */

  tmpreg = hsc->Instance->CR2;                          /* 赋值或初始化 */

  /* Clear STOP[13:12] bits */
  tmpreg &= (uint32_t)~((uint32_t)USART_CR2_STOP);      /* 赋值或初始化 */

  /* Set Stop Bits: Set STOP[13:12] bits according to hsc->Init.StopBits value */
  tmpreg |= (uint32_t)(hsc->Init.StopBits);             /* 赋值或初始化 */

  /* Write to USART CR2 */
  WRITE_REG(hsc->Instance->CR2, (uint32_t)tmpreg);      /* 执行语句 */

  /*-------------------------- USART CR1 Configuration -----------------------*/
  tmpreg = hsc->Instance->CR1;                          /* 赋值或初始化 */

  /* Clear M, PCE, PS, TE and RE bits */
  tmpreg &= (uint32_t)~((uint32_t)(USART_CR1_M | USART_CR1_PCE | USART_CR1_PS | USART_CR1_TE | \
                                   USART_CR1_RE));      /* 执行语句 */

  /* Configure the SMARTCARD Word Length, Parity and mode:
     Set the M bits according to hsc->Init.WordLength value
     Set PCE and PS bits according to hsc->Init.Parity value
     Set TE and RE bits according to hsc->Init.Mode value */
  tmpreg |= (uint32_t)hsc->Init.WordLength | hsc->Init.Parity | hsc->Init.Mode;  /* 赋值或初始化 */

  /* Write to USART CR1 */
  WRITE_REG(hsc->Instance->CR1, (uint32_t)tmpreg);      /* 执行语句 */

  /*-------------------------- USART CR3 Configuration -----------------------*/
  /* Clear CTSE and RTSE bits */
  CLEAR_BIT(hsc->Instance->CR3, (USART_CR3_RTSE | USART_CR3_CTSE));  /* 执行语句 */

  /*-------------------------- USART BRR Configuration -----------------------*/
#if defined(USART6)                                     /* 条件编译 */
  if((hsc->Instance == USART1) || (hsc->Instance == USART6))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    pclk = HAL_RCC_GetPCLK2Freq();                      /* 赋值或初始化 */
    hsc->Instance->BRR = SMARTCARD_BRR(pclk, hsc->Init.BaudRate);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
#else                                                   /* 条件编译否则 */
  if(hsc->Instance == USART1)                           /* 条件判断 */
  {                                                     /* 代码块开始 */
    pclk = HAL_RCC_GetPCLK2Freq();                      /* 赋值或初始化 */
    hsc->Instance->BRR = SMARTCARD_BRR(pclk, hsc->Init.BaudRate);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
#endif /* USART6 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    pclk = HAL_RCC_GetPCLK1Freq();                      /* 赋值或初始化 */
    hsc->Instance->BRR = SMARTCARD_BRR(pclk, hsc->Init.BaudRate);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

#endif /* HAL_SMARTCARD_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
