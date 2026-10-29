/* 文件: stm32f4xx_hal_irda.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_irda.c
  * @author  MCD Application Team
  * @brief   IRDA HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the IrDA SIR ENDEC block (IrDA):
  *           + Initialization and de-initialization functions
  *           + IO operation functions
  *           + Peripheral Control functions
  *           + Peripheral State and Errors functions
  @verbatim
  ==============================================================================
                        ##### How to use this driver #####
  ==============================================================================
  [..]
    The IRDA HAL driver can be used as follows:

    (#) Declare a IRDA_HandleTypeDef handle structure (eg. IRDA_HandleTypeDef hirda).
    (#) Initialize the IRDA low level resources by implementing the HAL_IRDA_MspInit() API:
        (##) Enable the USARTx interface clock.
        (##) IRDA pins configuration:
            (+++) Enable the clock for the IRDA GPIOs.
            (+++) Configure IRDA pins as alternate function pull-up.
        (##) NVIC configuration if you need to use interrupt process (HAL_IRDA_Transmit_IT()
             and HAL_IRDA_Receive_IT() APIs):
            (+++) Configure the USARTx interrupt priority.
            (+++) Enable the NVIC USART IRQ handle.
        (##) DMA Configuration if you need to use DMA process (HAL_IRDA_Transmit_DMA()
             and HAL_IRDA_Receive_DMA() APIs):
            (+++) Declare a DMA handle structure for the Tx/Rx stream.
            (+++) Enable the DMAx interface clock.
            (+++) Configure the declared DMA handle structure with the required Tx/Rx parameters.
            (+++) Configure the DMA Tx/Rx stream.
            (+++) Associate the initialized DMA handle to the IRDA DMA Tx/Rx handle.
            (+++) Configure the priority and enable the NVIC for the transfer complete interrupt on the DMA Tx/Rx stream.
            (+++) Configure the IRDAx interrupt priority and enable the NVIC USART IRQ handle
                  (used for last byte sending completion detection in DMA non circular mode)

    (#) Program the Baud Rate, Word Length, Parity, IrDA Mode, Prescaler
        and Mode(Receiver/Transmitter) in the hirda Init structure.

    (#) Initialize the IRDA registers by calling the HAL_IRDA_Init() API:
        (++) This API configures also the low level Hardware GPIO, CLOCK, CORTEX...etc)
             by calling the customized HAL_IRDA_MspInit() API.

         -@@- The specific IRDA interrupts (Transmission complete interrupt,
             RXNE interrupt and Error Interrupts) will be managed using the macros
             __HAL_IRDA_ENABLE_IT() and __HAL_IRDA_DISABLE_IT() inside the transmit and receive process.

    (#) Three operation modes are available within this driver :

    *** Polling mode IO operation ***
    =================================
    [..]
      (+) Send an amount of data in blocking mode using HAL_IRDA_Transmit()
      (+) Receive an amount of data in blocking mode using HAL_IRDA_Receive()

    *** Interrupt mode IO operation ***
    ===================================
    [..]
      (+) Send an amount of data in non blocking mode using HAL_IRDA_Transmit_IT()
      (+) At transmission end of transfer HAL_IRDA_TxCpltCallback is executed and user can
           add his own code by customization of function pointer HAL_IRDA_TxCpltCallback
      (+) Receive an amount of data in non blocking mode using HAL_IRDA_Receive_IT()
      (+) At reception end of transfer HAL_IRDA_RxCpltCallback is executed and user can
           add his own code by customization of function pointer HAL_IRDA_RxCpltCallback
      (+) In case of transfer Error, HAL_IRDA_ErrorCallback() function is executed and user can
           add his own code by customization of function pointer HAL_IRDA_ErrorCallback

    *** DMA mode IO operation ***
    =============================
    [..]
      (+) Send an amount of data in non blocking mode (DMA) using HAL_IRDA_Transmit_DMA()
      (+) At transmission end of half transfer HAL_IRDA_TxHalfCpltCallback is executed and user can
            add his own code by customization of function pointer HAL_IRDA_TxHalfCpltCallback
      (+) At transmission end of transfer HAL_IRDA_TxCpltCallback is executed and user can
           add his own code by customization of function pointer HAL_IRDA_TxCpltCallback
      (+) Receive an amount of data in non blocking mode (DMA) using HAL_IRDA_Receive_DMA()
      (+) At reception end of half transfer HAL_IRDA_RxHalfCpltCallback is executed and user can
            add his own code by customization of function pointer HAL_IRDA_RxHalfCpltCallback
      (+) At reception end of transfer HAL_IRDA_RxCpltCallback is executed and user can
           add his own code by customization of function pointer HAL_IRDA_RxCpltCallback
      (+) In case of transfer Error, HAL_IRDA_ErrorCallback() function is executed and user can
           add his own code by customization of function pointer HAL_IRDA_ErrorCallback
      (+) Pause the DMA Transfer using HAL_IRDA_DMAPause()
      (+) Resume the DMA Transfer using HAL_IRDA_DMAResume()
      (+) Stop the DMA Transfer using HAL_IRDA_DMAStop()

    *** IRDA HAL driver macros list ***
    ===================================
    [..]
      Below the list of most used macros in IRDA HAL driver.

       (+) __HAL_IRDA_ENABLE: Enable the IRDA peripheral
       (+) __HAL_IRDA_DISABLE: Disable the IRDA peripheral
       (+) __HAL_IRDA_GET_FLAG : Check whether the specified IRDA flag is set or not
       (+) __HAL_IRDA_CLEAR_FLAG : Clear the specified IRDA pending flag
       (+) __HAL_IRDA_ENABLE_IT: Enable the specified IRDA interrupt
       (+) __HAL_IRDA_DISABLE_IT: Disable the specified IRDA interrupt
       (+) __HAL_IRDA_GET_IT_SOURCE: Check whether the specified IRDA interrupt has occurred or not

    [..]
     (@) You can refer to the IRDA HAL driver header file for more useful macros

    ##### Callback registration #####
    ==================================

    [..]
      The compilation define USE_HAL_IRDA_REGISTER_CALLBACKS when set to 1
      allows the user to configure dynamically the driver callbacks.

    [..]
      Use Function @ref HAL_IRDA_RegisterCallback() to register a user callback.
      Function @ref HAL_IRDA_RegisterCallback() allows to register following callbacks:
       (+) TxHalfCpltCallback        : Tx Half Complete Callback.
       (+) TxCpltCallback            : Tx Complete Callback.
       (+) RxHalfCpltCallback        : Rx Half Complete Callback.
       (+) RxCpltCallback            : Rx Complete Callback.
       (+) ErrorCallback             : Error Callback.
       (+) AbortCpltCallback         : Abort Complete Callback.
       (+) AbortTransmitCpltCallback : Abort Transmit Complete Callback.
       (+) AbortReceiveCpltCallback  : Abort Receive Complete Callback.
       (+) MspInitCallback           : IRDA MspInit.
       (+) MspDeInitCallback         : IRDA MspDeInit.
      This function takes as parameters the HAL peripheral handle, the Callback ID
      and a pointer to the user callback function.

    [..]
      Use function @ref HAL_IRDA_UnRegisterCallback() to reset a callback to the default
      weak (surcharged) function.
      @ref HAL_IRDA_UnRegisterCallback() takes as parameters the HAL peripheral handle,
      and the Callback ID.
      This function allows to reset following callbacks:
       (+) TxHalfCpltCallback        : Tx Half Complete Callback.
       (+) TxCpltCallback            : Tx Complete Callback.
       (+) RxHalfCpltCallback        : Rx Half Complete Callback.
       (+) RxCpltCallback            : Rx Complete Callback.
       (+) ErrorCallback             : Error Callback.
       (+) AbortCpltCallback         : Abort Complete Callback.
       (+) AbortTransmitCpltCallback : Abort Transmit Complete Callback.
       (+) AbortReceiveCpltCallback  : Abort Receive Complete Callback.
       (+) MspInitCallback           : IRDA MspInit.
       (+) MspDeInitCallback         : IRDA MspDeInit.

    [..]
      By default, after the @ref HAL_IRDA_Init() and when the state is HAL_IRDA_STATE_RESET
      all callbacks are set to the corresponding weak (surcharged) functions:
      examples @ref HAL_IRDA_TxCpltCallback(), @ref HAL_IRDA_RxHalfCpltCallback().
      Exception done for MspInit and MspDeInit functions that are respectively
      reset to the legacy weak (surcharged) functions in the @ref HAL_IRDA_Init()
      and @ref HAL_IRDA_DeInit() only when these callbacks are null (not registered beforehand).
      If not, MspInit or MspDeInit are not null, the @ref HAL_IRDA_Init() and @ref HAL_IRDA_DeInit()
      keep and use the user MspInit/MspDeInit callbacks (registered beforehand).

    [..]
      Callbacks can be registered/unregistered in HAL_IRDA_STATE_READY state only.
      Exception done MspInit/MspDeInit that can be registered/unregistered
      in HAL_IRDA_STATE_READY or HAL_IRDA_STATE_RESET state, thus registered (user)
      MspInit/DeInit callbacks can be used during the Init/DeInit.
      In that case first register the MspInit/MspDeInit user callbacks
      using @ref HAL_IRDA_RegisterCallback() before calling @ref HAL_IRDA_DeInit()
      or @ref HAL_IRDA_Init() function.

    [..]
      When The compilation define USE_HAL_IRDA_REGISTER_CALLBACKS is set to 0 or
      not defined, the callback registration feature is not available
      and weak (surcharged) callbacks are used.

  @endverbatim
     [..]
       (@) Additionnal remark: If the parity is enabled, then the MSB bit of the data written
           in the data register is transmitted but is changed by the parity bit.
           Depending on the frame length defined by the M bit (8-bits or 9-bits),
           the possible IRDA frame formats are as listed in the following table:
    +-------------------------------------------------------------+
    |   M bit |  PCE bit  |            IRDA frame                 |
    |---------------------|---------------------------------------|
    |    0    |    0      |    | SB | 8 bit data | 1 STB |        |
    |---------|-----------|---------------------------------------|
    |    0    |    1      |    | SB | 7 bit data | PB | 1 STB |   |
    |---------|-----------|---------------------------------------|
    |    1    |    0      |    | SB | 9 bit data | 1 STB |        |
    |---------|-----------|---------------------------------------|
    |    1    |    1      |    | SB | 8 bit data | PB | 1 STB |   |
    +-------------------------------------------------------------+
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

/** @defgroup IRDA IRDA
  * @brief HAL IRDA module driver
  * @{
  */

#ifdef HAL_IRDA_MODULE_ENABLED                          /* 条件编译 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/** @addtogroup IRDA_Private_Functions
  * @{
  */
#if (USE_HAL_IRDA_REGISTER_CALLBACKS == 1)              /* 条件编译 */
void IRDA_InitCallbacksToDefault(IRDA_HandleTypeDef *hirda);  /* 函数定义或声明 */
#endif /* USE_HAL_IRDA_REGISTER_CALLBACKS */
static void IRDA_SetConfig(IRDA_HandleTypeDef *hirda);  /* 静态函数 */
static HAL_StatusTypeDef IRDA_Transmit_IT(IRDA_HandleTypeDef *hirda);  /* 静态变量/函数 */
static HAL_StatusTypeDef IRDA_EndTransmit_IT(IRDA_HandleTypeDef *hirda);  /* 静态变量/函数 */
static HAL_StatusTypeDef IRDA_Receive_IT(IRDA_HandleTypeDef *hirda);  /* 静态变量/函数 */
static void IRDA_DMATransmitCplt(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void IRDA_DMATransmitHalfCplt(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void IRDA_DMAReceiveCplt(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void IRDA_DMAReceiveHalfCplt(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void IRDA_DMAError(DMA_HandleTypeDef *hdma);     /* 静态函数 */
static void IRDA_DMAAbortOnError(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void IRDA_DMATxAbortCallback(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void IRDA_DMARxAbortCallback(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void IRDA_DMATxOnlyAbortCallback(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void IRDA_DMARxOnlyAbortCallback(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static HAL_StatusTypeDef IRDA_WaitOnFlagUntilTimeout(IRDA_HandleTypeDef *hirda, uint32_t Flag, FlagStatus Status, uint32_t Tickstart, uint32_t Timeout);  /* 静态变量/函数 */
static void IRDA_EndTxTransfer(IRDA_HandleTypeDef *hirda);  /* 静态函数 */
static void IRDA_EndRxTransfer(IRDA_HandleTypeDef *hirda);  /* 静态函数 */
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup IRDA_Exported_Functions IrDA Exported Functions
  * @{
  */

/** @defgroup IRDA_Exported_Functions_Group1 IrDA Initialization and de-initialization functions
  *  @brief    Initialization and Configuration functions
  *
@verbatim

  ==============================================================================
            ##### Initialization and Configuration functions #####
  ==============================================================================
    [..]
    This subsection provides a set of functions allowing to initialize the USARTx or the UARTy
    in asynchronous IrDA mode.
      (+) For the asynchronous mode only these parameters can be configured:
        (++) BaudRate
        (++) WordLength
        (++) Parity: If the parity is enabled, then the MSB bit of the data written
             in the data register is transmitted but is changed by the parity bit.
             Depending on the frame length defined by the M bit (8-bits or 9-bits),
             please refer to Reference manual for possible IRDA frame formats.
        (++) Prescaler: A pulse of width less than two and greater than one PSC period(s) may or may
             not be rejected. The receiver set up time should be managed by software. The IrDA physical layer
             specification specifies a minimum of 10 ms delay between transmission and
             reception (IrDA is a half duplex protocol).
        (++) Mode: Receiver/transmitter modes
        (++) IrDAMode: the IrDA can operate in the Normal mode or in the Low power mode.
    [..]
    The HAL_IRDA_Init() API follows IRDA configuration procedures (details for the procedures
    are available in reference manual).

@endverbatim
  * @{
  */

/**
  * @brief  Initializes the IRDA mode according to the specified
  *         parameters in the IRDA_InitTypeDef and create the associated handle.
  * @param  hirda  Pointer to a IRDA_HandleTypeDef structure that contains
  *                the configuration information for the specified IRDA module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_IRDA_Init(IRDA_HandleTypeDef *hirda)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the IRDA handle allocation */
  if (hirda == NULL)                                    /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the IRDA instance parameters */
  assert_param(IS_IRDA_INSTANCE(hirda->Instance));      /* 执行语句 */
  /* Check the IRDA mode parameter in the IRDA handle */
  assert_param(IS_IRDA_POWERMODE(hirda->Init.IrDAMode));  /* 执行语句 */

  if (hirda->gState == HAL_IRDA_STATE_RESET)            /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Allocate lock resource and initialize it */
    hirda->Lock = HAL_UNLOCKED;                         /* 赋值或初始化 */

#if USE_HAL_IRDA_REGISTER_CALLBACKS == 1                /* 条件编译 */
    IRDA_InitCallbacksToDefault(hirda);                 /* 执行语句 */

    if (hirda->MspInitCallback == NULL)                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      hirda->MspInitCallback = HAL_IRDA_MspInit;        /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Init the low level hardware */
    hirda->MspInitCallback(hirda);                      /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /* Init the low level hardware : GPIO, CLOCK */
    HAL_IRDA_MspInit(hirda);                            /* HAL库函数调用 */
#endif /* USE_HAL_IRDA_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */

  hirda->gState = HAL_IRDA_STATE_BUSY;                  /* 赋值或初始化 */

  /* Disable the IRDA peripheral */
  __HAL_IRDA_DISABLE(hirda);                            /* HAL辅助宏/函数 */

  /* Set the IRDA communication parameters */
  IRDA_SetConfig(hirda);                                /* 执行语句 */

  /* In IrDA mode, the following bits must be kept cleared:
  - LINEN, STOP and CLKEN bits in the USART_CR2 register,
  - SCEN and HDSEL bits in the USART_CR3 register.*/
  CLEAR_BIT(hirda->Instance->CR2, (USART_CR2_LINEN | USART_CR2_STOP | USART_CR2_CLKEN));  /* 执行语句 */
  CLEAR_BIT(hirda->Instance->CR3, (USART_CR3_SCEN | USART_CR3_HDSEL));  /* 执行语句 */

  /* Enable the IRDA peripheral */
  __HAL_IRDA_ENABLE(hirda);                             /* HAL辅助宏/函数 */

  /* Set the prescaler */
  MODIFY_REG(hirda->Instance->GTPR, USART_GTPR_PSC, hirda->Init.Prescaler);  /* 执行语句 */

  /* Configure the IrDA mode */
  MODIFY_REG(hirda->Instance->CR3, USART_CR3_IRLP, hirda->Init.IrDAMode);  /* 执行语句 */

  /* Enable the IrDA mode by setting the IREN bit in the CR3 register */
  SET_BIT(hirda->Instance->CR3, USART_CR3_IREN);        /* 执行语句 */

  /* Initialize the IRDA state*/
  hirda->ErrorCode = HAL_IRDA_ERROR_NONE;               /* 赋值或初始化 */
  hirda->gState = HAL_IRDA_STATE_READY;                 /* 赋值或初始化 */
  hirda->RxState = HAL_IRDA_STATE_READY;                /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  DeInitializes the IRDA peripheral
  * @param  hirda  Pointer to a IRDA_HandleTypeDef structure that contains
  *                the configuration information for the specified IRDA module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_IRDA_DeInit(IRDA_HandleTypeDef *hirda)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the IRDA handle allocation */
  if (hirda == NULL)                                    /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the parameters */
  assert_param(IS_IRDA_INSTANCE(hirda->Instance));      /* 执行语句 */

  hirda->gState = HAL_IRDA_STATE_BUSY;                  /* 赋值或初始化 */

  /* Disable the Peripheral */
  __HAL_IRDA_DISABLE(hirda);                            /* HAL辅助宏/函数 */

  /* DeInit the low level hardware */
#if USE_HAL_IRDA_REGISTER_CALLBACKS == 1                /* 条件编译 */
  if (hirda->MspDeInitCallback == NULL)                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    hirda->MspDeInitCallback = HAL_IRDA_MspDeInit;      /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  /* DeInit the low level hardware */
  hirda->MspDeInitCallback(hirda);                      /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_IRDA_MspDeInit(hirda);                            /* HAL库函数调用 */
#endif /* USE_HAL_IRDA_REGISTER_CALLBACKS */

  hirda->ErrorCode = HAL_IRDA_ERROR_NONE;               /* 赋值或初始化 */

  hirda->gState = HAL_IRDA_STATE_RESET;                 /* 赋值或初始化 */
  hirda->RxState = HAL_IRDA_STATE_RESET;                /* 赋值或初始化 */

  /* Release Lock */
  __HAL_UNLOCK(hirda);                                  /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  IRDA MSP Init.
  * @param  hirda  Pointer to a IRDA_HandleTypeDef structure that contains
  *                the configuration information for the specified IRDA module.
  * @retval None
  */
__weak void HAL_IRDA_MspInit(IRDA_HandleTypeDef *hirda)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hirda);                                        /* 执行语句 */

  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_IRDA_MspInit can be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  IRDA MSP DeInit.
  * @param  hirda  Pointer to a IRDA_HandleTypeDef structure that contains
  *                the configuration information for the specified IRDA module.
  * @retval None
  */
__weak void HAL_IRDA_MspDeInit(IRDA_HandleTypeDef *hirda)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hirda);                                        /* 执行语句 */

  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_IRDA_MspDeInit can be implemented in the user file
   */
}                                                       /* 代码块结束 */

#if (USE_HAL_IRDA_REGISTER_CALLBACKS == 1)              /* 条件编译 */
/**
  * @brief  Register a User IRDA Callback
  *         To be used instead of the weak predefined callback
  * @param  hirda irda handle
  * @param  CallbackID ID of the callback to be registered
  *         This parameter can be one of the following values:
  *           @arg @ref HAL_IRDA_TX_HALFCOMPLETE_CB_ID Tx Half Complete Callback ID
  *           @arg @ref HAL_IRDA_TX_COMPLETE_CB_ID Tx Complete Callback ID
  *           @arg @ref HAL_IRDA_RX_HALFCOMPLETE_CB_ID Rx Half Complete Callback ID
  *           @arg @ref HAL_IRDA_RX_COMPLETE_CB_ID Rx Complete Callback ID
  *           @arg @ref HAL_IRDA_ERROR_CB_ID Error Callback ID
  *           @arg @ref HAL_IRDA_ABORT_COMPLETE_CB_ID Abort Complete Callback ID
  *           @arg @ref HAL_IRDA_ABORT_TRANSMIT_COMPLETE_CB_ID Abort Transmit Complete Callback ID
  *           @arg @ref HAL_IRDA_ABORT_RECEIVE_COMPLETE_CB_ID Abort Receive Complete Callback ID
  *           @arg @ref HAL_IRDA_MSPINIT_CB_ID MspInit Callback ID
  *           @arg @ref HAL_IRDA_MSPDEINIT_CB_ID MspDeInit Callback ID
  * @param  pCallback pointer to the Callback function
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_IRDA_RegisterCallback(IRDA_HandleTypeDef *hirda, HAL_IRDA_CallbackIDTypeDef CallbackID, pIRDA_CallbackTypeDef pCallback)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  if (pCallback == NULL)                                /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hirda->ErrorCode |= HAL_IRDA_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  /* Process locked */
  __HAL_LOCK(hirda);                                    /* HAL辅助宏/函数 */

  if (hirda->gState == HAL_IRDA_STATE_READY)            /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_IRDA_TX_HALFCOMPLETE_CB_ID :             /* case标签 */
        hirda->TxHalfCpltCallback = pCallback;          /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_IRDA_TX_COMPLETE_CB_ID :                 /* case标签 */
        hirda->TxCpltCallback = pCallback;              /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_IRDA_RX_HALFCOMPLETE_CB_ID :             /* case标签 */
        hirda->RxHalfCpltCallback = pCallback;          /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_IRDA_RX_COMPLETE_CB_ID :                 /* case标签 */
        hirda->RxCpltCallback = pCallback;              /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_IRDA_ERROR_CB_ID :                       /* case标签 */
        hirda->ErrorCallback = pCallback;               /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_IRDA_ABORT_COMPLETE_CB_ID :              /* case标签 */
        hirda->AbortCpltCallback = pCallback;           /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_IRDA_ABORT_TRANSMIT_COMPLETE_CB_ID :     /* case标签 */
        hirda->AbortTransmitCpltCallback = pCallback;   /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_IRDA_ABORT_RECEIVE_COMPLETE_CB_ID :      /* case标签 */
        hirda->AbortReceiveCpltCallback = pCallback;    /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_IRDA_MSPINIT_CB_ID :                     /* case标签 */
        hirda->MspInitCallback = pCallback;             /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_IRDA_MSPDEINIT_CB_ID :                   /* case标签 */
        hirda->MspDeInitCallback = pCallback;           /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hirda->ErrorCode |= HAL_IRDA_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (hirda->gState == HAL_IRDA_STATE_RESET)       /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_IRDA_MSPINIT_CB_ID :                     /* case标签 */
        hirda->MspInitCallback = pCallback;             /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_IRDA_MSPDEINIT_CB_ID :                   /* case标签 */
        hirda->MspDeInitCallback = pCallback;           /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hirda->ErrorCode |= HAL_IRDA_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hirda->ErrorCode |= HAL_IRDA_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hirda);                                  /* HAL辅助宏/函数 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Unregister an IRDA callback
  *         IRDA callback is redirected to the weak predefined callback
  * @param  hirda irda handle
  * @param  CallbackID ID of the callback to be unregistered
  *         This parameter can be one of the following values:
  *           @arg @ref HAL_IRDA_TX_HALFCOMPLETE_CB_ID Tx Half Complete Callback ID
  *           @arg @ref HAL_IRDA_TX_COMPLETE_CB_ID Tx Complete Callback ID
  *           @arg @ref HAL_IRDA_RX_HALFCOMPLETE_CB_ID Rx Half Complete Callback ID
  *           @arg @ref HAL_IRDA_RX_COMPLETE_CB_ID Rx Complete Callback ID
  *           @arg @ref HAL_IRDA_ERROR_CB_ID Error Callback ID
  *           @arg @ref HAL_IRDA_ABORT_COMPLETE_CB_ID Abort Complete Callback ID
  *           @arg @ref HAL_IRDA_ABORT_TRANSMIT_COMPLETE_CB_ID Abort Transmit Complete Callback ID
  *           @arg @ref HAL_IRDA_ABORT_RECEIVE_COMPLETE_CB_ID Abort Receive Complete Callback ID
  *           @arg @ref HAL_IRDA_MSPINIT_CB_ID MspInit Callback ID
  *           @arg @ref HAL_IRDA_MSPDEINIT_CB_ID MspDeInit Callback ID
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_IRDA_UnRegisterCallback(IRDA_HandleTypeDef *hirda, HAL_IRDA_CallbackIDTypeDef CallbackID)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Process locked */
  __HAL_LOCK(hirda);                                    /* HAL辅助宏/函数 */

  if (HAL_IRDA_STATE_READY == hirda->gState)            /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_IRDA_TX_HALFCOMPLETE_CB_ID :             /* case标签 */
        hirda->TxHalfCpltCallback = HAL_IRDA_TxHalfCpltCallback;               /* Legacy weak  TxHalfCpltCallback       */
        break;                                          /* 跳出 */

      case HAL_IRDA_TX_COMPLETE_CB_ID :                 /* case标签 */
        hirda->TxCpltCallback = HAL_IRDA_TxCpltCallback;                       /* Legacy weak TxCpltCallback            */
        break;                                          /* 跳出 */

      case HAL_IRDA_RX_HALFCOMPLETE_CB_ID :             /* case标签 */
        hirda->RxHalfCpltCallback = HAL_IRDA_RxHalfCpltCallback;               /* Legacy weak RxHalfCpltCallback        */
        break;                                          /* 跳出 */

      case HAL_IRDA_RX_COMPLETE_CB_ID :                 /* case标签 */
        hirda->RxCpltCallback = HAL_IRDA_RxCpltCallback;                       /* Legacy weak RxCpltCallback            */
        break;                                          /* 跳出 */

      case HAL_IRDA_ERROR_CB_ID :                       /* case标签 */
        hirda->ErrorCallback = HAL_IRDA_ErrorCallback;                         /* Legacy weak ErrorCallback             */
        break;                                          /* 跳出 */

      case HAL_IRDA_ABORT_COMPLETE_CB_ID :              /* case标签 */
        hirda->AbortCpltCallback = HAL_IRDA_AbortCpltCallback;                 /* Legacy weak AbortCpltCallback         */
        break;                                          /* 跳出 */

      case HAL_IRDA_ABORT_TRANSMIT_COMPLETE_CB_ID :     /* case标签 */
        hirda->AbortTransmitCpltCallback = HAL_IRDA_AbortTransmitCpltCallback; /* Legacy weak AbortTransmitCpltCallback */
        break;                                          /* 跳出 */

      case HAL_IRDA_ABORT_RECEIVE_COMPLETE_CB_ID :      /* case标签 */
        hirda->AbortReceiveCpltCallback = HAL_IRDA_AbortReceiveCpltCallback;   /* Legacy weak AbortReceiveCpltCallback  */
        break;                                          /* 跳出 */

      case HAL_IRDA_MSPINIT_CB_ID :                     /* case标签 */
        hirda->MspInitCallback = HAL_IRDA_MspInit;                             /* Legacy weak MspInitCallback           */
        break;                                          /* 跳出 */

      case HAL_IRDA_MSPDEINIT_CB_ID :                   /* case标签 */
        hirda->MspDeInitCallback = HAL_IRDA_MspDeInit;                         /* Legacy weak MspDeInitCallback         */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hirda->ErrorCode |= HAL_IRDA_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (HAL_IRDA_STATE_RESET == hirda->gState)       /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_IRDA_MSPINIT_CB_ID :                     /* case标签 */
        hirda->MspInitCallback = HAL_IRDA_MspInit;      /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_IRDA_MSPDEINIT_CB_ID :                   /* case标签 */
        hirda->MspDeInitCallback = HAL_IRDA_MspDeInit;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hirda->ErrorCode |= HAL_IRDA_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hirda->ErrorCode |= HAL_IRDA_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hirda);                                  /* HAL辅助宏/函数 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* USE_HAL_IRDA_REGISTER_CALLBACKS */

/**
  * @}
  */

/** @defgroup IRDA_Exported_Functions_Group2 IO operation functions
  *  @brief   IRDA Transmit and Receive functions
  *
@verbatim
  ==============================================================================
                      ##### IO operation functions #####
  ==============================================================================
    [..]
    This subsection provides a set of functions allowing to manage the IRDA data transfers.
    IrDA is a half duplex communication protocol. If the Transmitter is busy, any data
    on the IrDA receive line will be ignored by the IrDA decoder and if the Receiver
    is busy, data on the TX from the USART to IrDA will not be encoded by IrDA.
    While receiving data, transmission should be avoided as the data to be transmitted
    could be corrupted.

    (#) There are two modes of transfer:
       (++) Blocking mode: The communication is performed in polling mode.
            The HAL status of all data processing is returned by the same function
            after finishing transfer.
       (++) Non-Blocking mode: The communication is performed using Interrupts
           or DMA, these API's return the HAL status.
           The end of the data processing will be indicated through the
           dedicated IRDA IRQ when using Interrupt mode or the DMA IRQ when
           using DMA mode.
           The HAL_IRDA_TxCpltCallback(), HAL_IRDA_RxCpltCallback() user callbacks
           will be executed respectively at the end of the Transmit or Receive process
           The HAL_IRDA_ErrorCallback() user callback will be executed when a communication error is detected

    (#) Blocking mode APIs are :
        (++) HAL_IRDA_Transmit()
        (++) HAL_IRDA_Receive()

    (#) Non Blocking mode APIs with Interrupt are :
        (++) HAL_IRDA_Transmit_IT()
        (++) HAL_IRDA_Receive_IT()
        (++) HAL_IRDA_IRQHandler()

    (#) Non Blocking mode functions with DMA are :
        (++) HAL_IRDA_Transmit_DMA()
        (++) HAL_IRDA_Receive_DMA()
        (++) HAL_IRDA_DMAPause()
        (++) HAL_IRDA_DMAResume()
        (++) HAL_IRDA_DMAStop()

    (#) A set of Transfer Complete Callbacks are provided in Non Blocking mode:
        (++) HAL_IRDA_TxHalfCpltCallback()
        (++) HAL_IRDA_TxCpltCallback()
        (++) HAL_IRDA_RxHalfCpltCallback()
        (++) HAL_IRDA_RxCpltCallback()
        (++) HAL_IRDA_ErrorCallback()

    (#) Non-Blocking mode transfers could be aborted using Abort API's :
        (+) HAL_IRDA_Abort()
        (+) HAL_IRDA_AbortTransmit()
        (+) HAL_IRDA_AbortReceive()
        (+) HAL_IRDA_Abort_IT()
        (+) HAL_IRDA_AbortTransmit_IT()
        (+) HAL_IRDA_AbortReceive_IT()

    (#) For Abort services based on interrupts (HAL_IRDA_Abortxxx_IT), a set of Abort Complete Callbacks are provided:
        (+) HAL_IRDA_AbortCpltCallback()
        (+) HAL_IRDA_AbortTransmitCpltCallback()
        (+) HAL_IRDA_AbortReceiveCpltCallback()

    (#) In Non-Blocking mode transfers, possible errors are split into 2 categories.
        Errors are handled as follows :
        (+) Error is considered as Recoverable and non blocking : Transfer could go till end, but error severity is
            to be evaluated by user : this concerns Frame Error, Parity Error or Noise Error in Interrupt mode reception .
            Received character is then retrieved and stored in Rx buffer, Error code is set to allow user to identify error type,
            and HAL_IRDA_ErrorCallback() user callback is executed. Transfer is kept ongoing on IRDA side.
            If user wants to abort it, Abort services should be called by user.
        (+) Error is considered as Blocking : Transfer could not be completed properly and is aborted.
            This concerns Overrun Error In Interrupt mode reception and all errors in DMA mode.
            Error code is set to allow user to identify error type, and HAL_IRDA_ErrorCallback() user callback is executed.

@endverbatim
  * @{
  */

/**
  * @brief Sends an amount of data in blocking mode.
  * @note  When UART parity is not enabled (PCE = 0), and Word Length is configured to 9 bits (M1-M0 = 01),
  *        the sent data is handled as a set of u16. In this case, Size must reflect the number
  *        of u16 available through pData.
  * @param hirda Pointer to a IRDA_HandleTypeDef structure that contains
  *              the configuration information for the specified IRDA module.
  * @param pData Pointer to data buffer (u8 or u16 data elements).
  * @param Size  Amount of data elements (u8 or u16) to be sent.
  * @param Timeout Specify timeout value.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_IRDA_Transmit(IRDA_HandleTypeDef *hirda, uint8_t *pData, uint16_t Size, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint16_t *tmp;                                        /* 16位无符号变量/函数 */
  uint32_t tickstart = 0U;                              /* 32位无符号变量/函数 */

  /* Check that a Tx process is not already ongoing */
  if (hirda->gState == HAL_IRDA_STATE_READY)            /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) || (Size == 0U))                /* 条件判断 */
    {                                                   /* 代码块开始 */
      return  HAL_ERROR;                                /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hirda);                                  /* HAL辅助宏/函数 */

    hirda->ErrorCode = HAL_IRDA_ERROR_NONE;             /* 赋值或初始化 */
    hirda->gState = HAL_IRDA_STATE_BUSY_TX;             /* 赋值或初始化 */

    /* Init tickstart for timeout managment*/
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */

    hirda->TxXferSize = Size;                           /* 赋值或初始化 */
    hirda->TxXferCount = Size;                          /* 赋值或初始化 */
    while (hirda->TxXferCount > 0U)                     /* while循环 */
    {                                                   /* 代码块开始 */
      hirda->TxXferCount--;                             /* 执行语句 */
      if (hirda->Init.WordLength == IRDA_WORDLENGTH_9B)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        if (IRDA_WaitOnFlagUntilTimeout(hirda, IRDA_FLAG_TXE, RESET, tickstart, Timeout) != HAL_OK)  /* 条件判断 */
        {                                               /* 代码块开始 */
          return HAL_TIMEOUT;                           /* 返回值 */
        }                                               /* 代码块结束 */
        tmp = (uint16_t *) pData;                       /* 赋值或初始化 */
        hirda->Instance->DR = (*tmp & (uint16_t)0x01FF);  /* 赋值或初始化 */
        if (hirda->Init.Parity == IRDA_PARITY_NONE)     /* 条件判断 */
        {                                               /* 代码块开始 */
          pData += 2U;                                  /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          pData += 1U;                                  /* 赋值或初始化 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        if (IRDA_WaitOnFlagUntilTimeout(hirda, IRDA_FLAG_TXE, RESET, tickstart, Timeout) != HAL_OK)  /* 条件判断 */
        {                                               /* 代码块开始 */
          return HAL_TIMEOUT;                           /* 返回值 */
        }                                               /* 代码块结束 */
        hirda->Instance->DR = (*pData++ & (uint8_t)0xFF);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    if (IRDA_WaitOnFlagUntilTimeout(hirda, IRDA_FLAG_TC, RESET, tickstart, Timeout) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_TIMEOUT;                               /* 返回值 */
    }                                                   /* 代码块结束 */

    /* At end of Tx process, restore hirda->gState to Ready */
    hirda->gState = HAL_IRDA_STATE_READY;               /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hirda);                                /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief Receive an amount of data in blocking mode.
  * @note  When UART parity is not enabled (PCE = 0), and Word Length is configured to 9 bits (M1-M0 = 01),
  *        the received data is handled as a set of u16. In this case, Size must reflect the number
  *        of u16 available through pData.
  * @param hirda Pointer to a IRDA_HandleTypeDef structure that contains
  *              the configuration information for the specified IRDA module.
  * @param pData Pointer to data buffer (u8 or u16 data elements).
  * @param Size  Amount of data elements (u8 or u16) to be received.
  * @param Timeout Specify timeout value
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_IRDA_Receive(IRDA_HandleTypeDef *hirda, uint8_t *pData, uint16_t Size, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint16_t *tmp;                                        /* 16位无符号变量/函数 */
  uint32_t tickstart = 0U;                              /* 32位无符号变量/函数 */

  /* Check that a Rx process is not already ongoing */
  if (hirda->RxState == HAL_IRDA_STATE_READY)           /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) || (Size == 0U))                /* 条件判断 */
    {                                                   /* 代码块开始 */
      return  HAL_ERROR;                                /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hirda);                                  /* HAL辅助宏/函数 */

    hirda->ErrorCode = HAL_IRDA_ERROR_NONE;             /* 赋值或初始化 */
    hirda->RxState = HAL_IRDA_STATE_BUSY_RX;            /* 赋值或初始化 */

    /* Init tickstart for timeout managment*/
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */

    hirda->RxXferSize = Size;                           /* 赋值或初始化 */
    hirda->RxXferCount = Size;                          /* 赋值或初始化 */

    /* Check the remain data to be received */
    while (hirda->RxXferCount > 0U)                     /* while循环 */
    {                                                   /* 代码块开始 */
      hirda->RxXferCount--;                             /* 执行语句 */

      if (hirda->Init.WordLength == IRDA_WORDLENGTH_9B)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        if (IRDA_WaitOnFlagUntilTimeout(hirda, IRDA_FLAG_RXNE, RESET, tickstart, Timeout) != HAL_OK)  /* 条件判断 */
        {                                               /* 代码块开始 */
          return HAL_TIMEOUT;                           /* 返回值 */
        }                                               /* 代码块结束 */
        tmp = (uint16_t *) pData ;                      /* 赋值或初始化 */
        if (hirda->Init.Parity == IRDA_PARITY_NONE)     /* 条件判断 */
        {                                               /* 代码块开始 */
          *tmp = (uint16_t)(hirda->Instance->DR & (uint16_t)0x01FF);  /* 赋值或初始化 */
          pData += 2U;                                  /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          *tmp = (uint16_t)(hirda->Instance->DR & (uint16_t)0x00FF);  /* 赋值或初始化 */
          pData += 1U;                                  /* 赋值或初始化 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        if (IRDA_WaitOnFlagUntilTimeout(hirda, IRDA_FLAG_RXNE, RESET, tickstart, Timeout) != HAL_OK)  /* 条件判断 */
        {                                               /* 代码块开始 */
          return HAL_TIMEOUT;                           /* 返回值 */
        }                                               /* 代码块结束 */
        if (hirda->Init.Parity == IRDA_PARITY_NONE)     /* 条件判断 */
        {                                               /* 代码块开始 */
          *pData++ = (uint8_t)(hirda->Instance->DR & (uint8_t)0x00FF);  /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          *pData++ = (uint8_t)(hirda->Instance->DR & (uint8_t)0x007F);  /* 赋值或初始化 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* At end of Rx process, restore hirda->RxState to Ready */
    hirda->RxState = HAL_IRDA_STATE_READY;              /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hirda);                                /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief Send an amount of data in non blocking mode.
  * @note  When UART parity is not enabled (PCE = 0), and Word Length is configured to 9 bits (M1-M0 = 01),
  *        the sent data is handled as a set of u16. In this case, Size must reflect the number
  *        of u16 available through pData.
  * @param hirda Pointer to a IRDA_HandleTypeDef structure that contains
  *              the configuration information for the specified IRDA module.
  * @param pData Pointer to data buffer (u8 or u16 data elements).
  * @param Size  Amount of data elements (u8 or u16) to be sent.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_IRDA_Transmit_IT(IRDA_HandleTypeDef *hirda, uint8_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check that a Tx process is not already ongoing */
  if (hirda->gState == HAL_IRDA_STATE_READY)            /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) || (Size == 0U))                /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hirda);                                  /* HAL辅助宏/函数 */

    hirda->pTxBuffPtr = pData;                          /* 赋值或初始化 */
    hirda->TxXferSize = Size;                           /* 赋值或初始化 */
    hirda->TxXferCount = Size;                          /* 赋值或初始化 */

    hirda->ErrorCode = HAL_IRDA_ERROR_NONE;             /* 赋值或初始化 */
    hirda->gState = HAL_IRDA_STATE_BUSY_TX;             /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hirda);                                /* HAL辅助宏/函数 */

    /* Enable the IRDA Transmit Data Register Empty Interrupt */
    SET_BIT(hirda->Instance->CR1, USART_CR1_TXEIE);     /* 执行语句 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief Receive an amount of data in non blocking mode.
  * @note  When UART parity is not enabled (PCE = 0), and Word Length is configured to 9 bits (M1-M0 = 01),
  *        the received data is handled as a set of u16. In this case, Size must reflect the number
  *        of u16 available through pData.
  * @param hirda Pointer to a IRDA_HandleTypeDef structure that contains
  *              the configuration information for the specified IRDA module.
  * @param pData Pointer to data buffer (u8 or u16 data elements).
  * @param Size  Amount of data elements (u8 or u16) to be received.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_IRDA_Receive_IT(IRDA_HandleTypeDef *hirda, uint8_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check that a Rx process is not already ongoing */
  if (hirda->RxState == HAL_IRDA_STATE_READY)           /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) || (Size == 0U))                /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hirda);                                  /* HAL辅助宏/函数 */

    hirda->pRxBuffPtr = pData;                          /* 赋值或初始化 */
    hirda->RxXferSize = Size;                           /* 赋值或初始化 */
    hirda->RxXferCount = Size;                          /* 赋值或初始化 */

    hirda->ErrorCode = HAL_IRDA_ERROR_NONE;             /* 赋值或初始化 */
    hirda->RxState = HAL_IRDA_STATE_BUSY_RX;            /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hirda);                                /* HAL辅助宏/函数 */

    /* Enable the IRDA Parity Error and Data Register Not Empty Interrupts */
    SET_BIT(hirda->Instance->CR1, USART_CR1_PEIE | USART_CR1_RXNEIE);  /* 执行语句 */

    /* Enable the IRDA Error Interrupt: (Frame error, Noise error, Overrun error) */
    SET_BIT(hirda->Instance->CR3, USART_CR3_EIE);       /* 执行语句 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief Send an amount of data in DMA mode.
  * @note  When UART parity is not enabled (PCE = 0), and Word Length is configured to 9 bits (M1-M0 = 01),
  *        the sent data is handled as a set of u16. In this case, Size must reflect the number
  *        of u16 available through pData.
  * @param hirda Pointer to a IRDA_HandleTypeDef structure that contains
  *              the configuration information for the specified IRDA module.
  * @param pData Pointer to data buffer (u8 or u16 data elements).
  * @param Size  Amount of data elements (u8 or u16) to be sent.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_IRDA_Transmit_DMA(IRDA_HandleTypeDef *hirda, uint8_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t *tmp;                                        /* 32位无符号变量/函数 */

  /* Check that a Tx process is not already ongoing */
  if (hirda->gState == HAL_IRDA_STATE_READY)            /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) || (Size == 0U))                /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hirda);                                  /* HAL辅助宏/函数 */

    hirda->pTxBuffPtr = pData;                          /* 赋值或初始化 */
    hirda->TxXferSize = Size;                           /* 赋值或初始化 */
    hirda->TxXferCount = Size;                          /* 赋值或初始化 */

    hirda->ErrorCode = HAL_IRDA_ERROR_NONE;             /* 赋值或初始化 */
    hirda->gState = HAL_IRDA_STATE_BUSY_TX;             /* 赋值或初始化 */

    /* Set the IRDA DMA transfer complete callback */
    hirda->hdmatx->XferCpltCallback = IRDA_DMATransmitCplt;  /* 赋值或初始化 */

    /* Set the IRDA DMA half transfer complete callback */
    hirda->hdmatx->XferHalfCpltCallback = IRDA_DMATransmitHalfCplt;  /* 赋值或初始化 */

    /* Set the DMA error callback */
    hirda->hdmatx->XferErrorCallback = IRDA_DMAError;   /* 赋值或初始化 */

    /* Set the DMA abort callback */
    hirda->hdmatx->XferAbortCallback = NULL;            /* 赋值或初始化 */

    /* Enable the IRDA transmit DMA stream */
    tmp = (uint32_t *)&pData;                           /* 赋值或初始化 */
    HAL_DMA_Start_IT(hirda->hdmatx, *(uint32_t *)tmp, (uint32_t)&hirda->Instance->DR, Size);  /* HAL DMA操作 */

    /* Clear the TC flag in the SR register by writing 0 to it */
    __HAL_IRDA_CLEAR_FLAG(hirda, IRDA_FLAG_TC);         /* HAL辅助宏/函数 */

    /* Process Unlocked */
    __HAL_UNLOCK(hirda);                                /* HAL辅助宏/函数 */

    /* Enable the DMA transfer for transmit request by setting the DMAT bit
    in the USART CR3 register */
    SET_BIT(hirda->Instance->CR3, USART_CR3_DMAT);      /* 执行语句 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief Receives an amount of data in DMA mode.
  * @note  When UART parity is not enabled (PCE = 0), and Word Length is configured to 9 bits (M1-M0 = 01),
  *        the received data is handled as a set of u16. In this case, Size must reflect the number
  *        of u16 available through pData.
  * @param hirda Pointer to a IRDA_HandleTypeDef structure that contains
  *              the configuration information for the specified IRDA module.
  * @param pData Pointer to data buffer (u8 or u16 data elements).
  * @param Size  Amount of data elements (u8 or u16) to be received.
  * @note   When the IRDA parity is enabled (PCE = 1) the data received contain the parity bit.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_IRDA_Receive_DMA(IRDA_HandleTypeDef *hirda, uint8_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t *tmp;                                        /* 32位无符号变量/函数 */

  /* Check that a Rx process is not already ongoing */
  if (hirda->RxState == HAL_IRDA_STATE_READY)           /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) || (Size == 0U))                /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hirda);                                  /* HAL辅助宏/函数 */

    hirda->pRxBuffPtr = pData;                          /* 赋值或初始化 */
    hirda->RxXferSize = Size;                           /* 赋值或初始化 */

    hirda->ErrorCode = HAL_IRDA_ERROR_NONE;             /* 赋值或初始化 */
    hirda->RxState = HAL_IRDA_STATE_BUSY_RX;            /* 赋值或初始化 */

    /* Set the IRDA DMA transfer complete callback */
    hirda->hdmarx->XferCpltCallback = IRDA_DMAReceiveCplt;  /* 赋值或初始化 */

    /* Set the IRDA DMA half transfer complete callback */
    hirda->hdmarx->XferHalfCpltCallback = IRDA_DMAReceiveHalfCplt;  /* 赋值或初始化 */

    /* Set the DMA error callback */
    hirda->hdmarx->XferErrorCallback = IRDA_DMAError;   /* 赋值或初始化 */

    /* Set the DMA abort callback */
    hirda->hdmarx->XferAbortCallback = NULL;            /* 赋值或初始化 */

    /* Enable the DMA stream */
    tmp = (uint32_t *)&pData;                           /* 赋值或初始化 */
    HAL_DMA_Start_IT(hirda->hdmarx, (uint32_t)&hirda->Instance->DR, *(uint32_t *)tmp, Size);  /* HAL DMA操作 */

    /* Clear the Overrun flag just before enabling the DMA Rx request: can be mandatory for the second transfer */
    __HAL_IRDA_CLEAR_OREFLAG(hirda);                    /* HAL辅助宏/函数 */

    /* Process Unlocked */
    __HAL_UNLOCK(hirda);                                /* HAL辅助宏/函数 */

    /* Enable the IRDA Parity Error Interrupt */
    SET_BIT(hirda->Instance->CR1, USART_CR1_PEIE);      /* 执行语句 */

    /* Enable the IRDA Error Interrupt: (Frame error, Noise error, Overrun error) */
    SET_BIT(hirda->Instance->CR3, USART_CR3_EIE);       /* 执行语句 */

    /* Enable the DMA transfer for the receiver request by setting the DMAR bit
    in the USART CR3 register */
    SET_BIT(hirda->Instance->CR3, USART_CR3_DMAR);      /* 执行语句 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief Pauses the DMA Transfer.
  * @param  hirda  Pointer to a IRDA_HandleTypeDef structure that contains
  *                the configuration information for the specified IRDA module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_IRDA_DMAPause(IRDA_HandleTypeDef *hirda)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t dmarequest = 0x00U;                          /* 32位无符号变量/函数 */

  /* Process Locked */
  __HAL_LOCK(hirda);                                    /* HAL辅助宏/函数 */

  dmarequest = HAL_IS_BIT_SET(hirda->Instance->CR3, USART_CR3_DMAT);  /* 赋值或初始化 */
  if ((hirda->gState == HAL_IRDA_STATE_BUSY_TX) && dmarequest)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable the IRDA DMA Tx request */
    CLEAR_BIT(hirda->Instance->CR3, USART_CR3_DMAT);    /* 执行语句 */
  }                                                     /* 代码块结束 */

  dmarequest = HAL_IS_BIT_SET(hirda->Instance->CR3, USART_CR3_DMAR);  /* 赋值或初始化 */
  if ((hirda->RxState == HAL_IRDA_STATE_BUSY_RX) && dmarequest)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable PE and ERR (Frame error, noise error, overrun error) interrupts */
    CLEAR_BIT(hirda->Instance->CR1, USART_CR1_PEIE);    /* 执行语句 */
    CLEAR_BIT(hirda->Instance->CR3, USART_CR3_EIE);     /* 执行语句 */

    /* Disable the IRDA DMA Rx request */
    CLEAR_BIT(hirda->Instance->CR3, USART_CR3_DMAR);    /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Process Unlocked */
  __HAL_UNLOCK(hirda);                                  /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief Resumes the DMA Transfer.
  * @param  hirda  Pointer to a IRDA_HandleTypeDef structure that contains
  *                the configuration information for the specified IRDA module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_IRDA_DMAResume(IRDA_HandleTypeDef *hirda)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Process Locked */
  __HAL_LOCK(hirda);                                    /* HAL辅助宏/函数 */

  if (hirda->gState == HAL_IRDA_STATE_BUSY_TX)          /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable the IRDA DMA Tx request */
    SET_BIT(hirda->Instance->CR3, USART_CR3_DMAT);      /* 执行语句 */
  }                                                     /* 代码块结束 */

  if (hirda->RxState == HAL_IRDA_STATE_BUSY_RX)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Clear the Overrun flag before resuming the Rx transfer */
    __HAL_IRDA_CLEAR_OREFLAG(hirda);                    /* HAL辅助宏/函数 */

    /* Reenable PE and ERR (Frame error, noise error, overrun error) interrupts */
    SET_BIT(hirda->Instance->CR1, USART_CR1_PEIE);      /* 执行语句 */
    SET_BIT(hirda->Instance->CR3, USART_CR3_EIE);       /* 执行语句 */

    /* Enable the IRDA DMA Rx request */
    SET_BIT(hirda->Instance->CR3, USART_CR3_DMAR);      /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Process Unlocked */
  __HAL_UNLOCK(hirda);                                  /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief Stops the DMA Transfer.
  * @param  hirda  Pointer to a IRDA_HandleTypeDef structure that contains
  *                the configuration information for the specified IRDA module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_IRDA_DMAStop(IRDA_HandleTypeDef *hirda)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t dmarequest = 0x00U;                          /* 32位无符号变量/函数 */
  /* The Lock is not implemented on this API to allow the user application
     to call the HAL IRDA API under callbacks HAL_IRDA_TxCpltCallback() / HAL_IRDA_RxCpltCallback():
     when calling HAL_DMA_Abort() API the DMA TX/RX Transfer complete interrupt is generated
     and the correspond call back is executed HAL_IRDA_TxCpltCallback() / HAL_IRDA_RxCpltCallback()
  */

  /* Stop IRDA DMA Tx request if ongoing */
  dmarequest = HAL_IS_BIT_SET(hirda->Instance->CR3, USART_CR3_DMAT);  /* 赋值或初始化 */
  if ((hirda->gState == HAL_IRDA_STATE_BUSY_TX) && dmarequest)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    CLEAR_BIT(hirda->Instance->CR3, USART_CR3_DMAT);    /* 执行语句 */

    /* Abort the IRDA DMA Tx channel */
    if (hirda->hdmatx != NULL)                          /* 条件判断 */
    {                                                   /* 代码块开始 */
      HAL_DMA_Abort(hirda->hdmatx);                     /* HAL DMA操作 */
    }                                                   /* 代码块结束 */
    IRDA_EndTxTransfer(hirda);                          /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Stop IRDA DMA Rx request if ongoing */
  dmarequest = HAL_IS_BIT_SET(hirda->Instance->CR3, USART_CR3_DMAR);  /* 赋值或初始化 */
  if ((hirda->RxState == HAL_IRDA_STATE_BUSY_RX) && dmarequest)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    CLEAR_BIT(hirda->Instance->CR3, USART_CR3_DMAR);    /* 执行语句 */

    /* Abort the IRDA DMA Rx channel */
    if (hirda->hdmarx != NULL)                          /* 条件判断 */
    {                                                   /* 代码块开始 */
      HAL_DMA_Abort(hirda->hdmarx);                     /* HAL DMA操作 */
    }                                                   /* 代码块结束 */
    IRDA_EndRxTransfer(hirda);                          /* 执行语句 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Abort ongoing transfers (blocking mode).
  * @param  hirda IRDA handle.
  * @note   This procedure could be used for aborting any ongoing transfer started in Interrupt or DMA mode.
  *         This procedure performs following operations :
  *           - Disable PPP Interrupts
  *           - Disable the DMA transfer in the peripheral register (if enabled)
  *           - Abort DMA transfer by calling HAL_DMA_Abort (in case of transfer in DMA mode)
  *           - Set handle State to READY
  * @note   This procedure is executed in blocking mode : when exiting function, Abort is considered as completed.
  * @retval HAL status
*/
HAL_StatusTypeDef HAL_IRDA_Abort(IRDA_HandleTypeDef *hirda)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Disable TXEIE, TCIE, RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts */
  CLEAR_BIT(hirda->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE | USART_CR1_TXEIE | USART_CR1_TCIE));  /* 执行语句 */
  CLEAR_BIT(hirda->Instance->CR3, USART_CR3_EIE);       /* 执行语句 */

  /* Disable the IRDA DMA Tx request if enabled */
  if (HAL_IS_BIT_SET(hirda->Instance->CR3, USART_CR3_DMAT))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    CLEAR_BIT(hirda->Instance->CR3, USART_CR3_DMAT);    /* 执行语句 */

    /* Abort the IRDA DMA Tx channel : use blocking DMA Abort API (no callback) */
    if (hirda->hdmatx != NULL)                          /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the IRDA DMA Abort callback to Null.
         No call back execution at end of DMA abort procedure */
      hirda->hdmatx->XferAbortCallback = NULL;          /* 赋值或初始化 */

      HAL_DMA_Abort(hirda->hdmatx);                     /* HAL DMA操作 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Disable the IRDA DMA Rx request if enabled */
  if (HAL_IS_BIT_SET(hirda->Instance->CR3, USART_CR3_DMAR))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    CLEAR_BIT(hirda->Instance->CR3, USART_CR3_DMAR);    /* 执行语句 */

    /* Abort the IRDA DMA Rx channel : use blocking DMA Abort API (no callback) */
    if (hirda->hdmarx != NULL)                          /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the IRDA DMA Abort callback to Null.
         No call back execution at end of DMA abort procedure */
      hirda->hdmarx->XferAbortCallback = NULL;          /* 赋值或初始化 */

      HAL_DMA_Abort(hirda->hdmarx);                     /* HAL DMA操作 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Reset Tx and Rx transfer counters */
  hirda->TxXferCount = 0x00U;                           /* 赋值或初始化 */
  hirda->RxXferCount = 0x00U;                           /* 赋值或初始化 */

  /* Reset ErrorCode */
  hirda->ErrorCode = HAL_IRDA_ERROR_NONE;               /* 赋值或初始化 */

  /* Restore hirda->RxState and hirda->gState to Ready */
  hirda->RxState = HAL_IRDA_STATE_READY;                /* 赋值或初始化 */
  hirda->gState = HAL_IRDA_STATE_READY;                 /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Abort ongoing Transmit transfer (blocking mode).
  * @param  hirda IRDA handle.
  * @note   This procedure could be used for aborting any ongoing transfer started in Interrupt or DMA mode.
  *         This procedure performs following operations :
  *           - Disable PPP Interrupts
  *           - Disable the DMA transfer in the peripheral register (if enabled)
  *           - Abort DMA transfer by calling HAL_DMA_Abort (in case of transfer in DMA mode)
  *           - Set handle State to READY
  * @note   This procedure is executed in blocking mode : when exiting function, Abort is considered as completed.
  * @retval HAL status
*/
HAL_StatusTypeDef HAL_IRDA_AbortTransmit(IRDA_HandleTypeDef *hirda)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Disable TXEIE and TCIE interrupts */
  CLEAR_BIT(hirda->Instance->CR1, (USART_CR1_TXEIE | USART_CR1_TCIE));  /* 执行语句 */

  /* Disable the IRDA DMA Tx request if enabled */
  if (HAL_IS_BIT_SET(hirda->Instance->CR3, USART_CR3_DMAT))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    CLEAR_BIT(hirda->Instance->CR3, USART_CR3_DMAT);    /* 执行语句 */

    /* Abort the IRDA DMA Tx channel : use blocking DMA Abort API (no callback) */
    if (hirda->hdmatx != NULL)                          /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the IRDA DMA Abort callback to Null.
         No call back execution at end of DMA abort procedure */
      hirda->hdmatx->XferAbortCallback = NULL;          /* 赋值或初始化 */

      HAL_DMA_Abort(hirda->hdmatx);                     /* HAL DMA操作 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Reset Tx transfer counter */
  hirda->TxXferCount = 0x00U;                           /* 赋值或初始化 */

  /* Restore hirda->gState to Ready */
  hirda->gState = HAL_IRDA_STATE_READY;                 /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Abort ongoing Receive transfer (blocking mode).
  * @param  hirda IRDA handle.
  * @note   This procedure could be used for aborting any ongoing transfer started in Interrupt or DMA mode.
  *         This procedure performs following operations :
  *           - Disable PPP Interrupts
  *           - Disable the DMA transfer in the peripheral register (if enabled)
  *           - Abort DMA transfer by calling HAL_DMA_Abort (in case of transfer in DMA mode)
  *           - Set handle State to READY
  * @note   This procedure is executed in blocking mode : when exiting function, Abort is considered as completed.
  * @retval HAL status
*/
HAL_StatusTypeDef HAL_IRDA_AbortReceive(IRDA_HandleTypeDef *hirda)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Disable RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts */
  CLEAR_BIT(hirda->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE));  /* 执行语句 */
  CLEAR_BIT(hirda->Instance->CR3, USART_CR3_EIE);       /* 执行语句 */

  /* Disable the IRDA DMA Rx request if enabled */
  if (HAL_IS_BIT_SET(hirda->Instance->CR3, USART_CR3_DMAR))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    CLEAR_BIT(hirda->Instance->CR3, USART_CR3_DMAR);    /* 执行语句 */

    /* Abort the IRDA DMA Rx channel : use blocking DMA Abort API (no callback) */
    if (hirda->hdmarx != NULL)                          /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the IRDA DMA Abort callback to Null.
         No call back execution at end of DMA abort procedure */
      hirda->hdmarx->XferAbortCallback = NULL;          /* 赋值或初始化 */

      HAL_DMA_Abort(hirda->hdmarx);                     /* HAL DMA操作 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Reset Rx transfer counter */
  hirda->RxXferCount = 0x00U;                           /* 赋值或初始化 */

  /* Restore hirda->RxState to Ready */
  hirda->RxState = HAL_IRDA_STATE_READY;                /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Abort ongoing transfers (Interrupt mode).
  * @param  hirda IRDA handle.
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
HAL_StatusTypeDef HAL_IRDA_Abort_IT(IRDA_HandleTypeDef *hirda)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t AbortCplt = 0x01U;                           /* 32位无符号变量/函数 */

  /* Disable TXEIE, TCIE, RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts */
  CLEAR_BIT(hirda->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE | USART_CR1_TXEIE | USART_CR1_TCIE));  /* 执行语句 */
  CLEAR_BIT(hirda->Instance->CR3, USART_CR3_EIE);       /* 执行语句 */

  /* If DMA Tx and/or DMA Rx Handles are associated to IRDA Handle, DMA Abort complete callbacks should be initialised
     before any call to DMA Abort functions */
  /* DMA Tx Handle is valid */
  if (hirda->hdmatx != NULL)                            /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Set DMA Abort Complete callback if IRDA DMA Tx request if enabled.
       Otherwise, set it to NULL */
    if (HAL_IS_BIT_SET(hirda->Instance->CR3, USART_CR3_DMAT))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hirda->hdmatx->XferAbortCallback = IRDA_DMATxAbortCallback;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hirda->hdmatx->XferAbortCallback = NULL;          /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /* DMA Rx Handle is valid */
  if (hirda->hdmarx != NULL)                            /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Set DMA Abort Complete callback if IRDA DMA Rx request if enabled.
       Otherwise, set it to NULL */
    if (HAL_IS_BIT_SET(hirda->Instance->CR3, USART_CR3_DMAR))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hirda->hdmarx->XferAbortCallback = IRDA_DMARxAbortCallback;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hirda->hdmarx->XferAbortCallback = NULL;          /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Disable the IRDA DMA Tx request if enabled */
  if (HAL_IS_BIT_SET(hirda->Instance->CR3, USART_CR3_DMAT))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable DMA Tx at IRDA level */
    CLEAR_BIT(hirda->Instance->CR3, USART_CR3_DMAT);    /* 执行语句 */

    /* Abort the IRDA DMA Tx channel : use non blocking DMA Abort API (callback) */
    if (hirda->hdmatx != NULL)                          /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* IRDA Tx DMA Abort callback has already been initialised :
         will lead to call HAL_IRDA_AbortCpltCallback() at end of DMA abort procedure */

      /* Abort DMA TX */
      if (HAL_DMA_Abort_IT(hirda->hdmatx) != HAL_OK)    /* 条件判断 */
      {                                                 /* 代码块开始 */
        hirda->hdmatx->XferAbortCallback = NULL;        /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        AbortCplt = 0x00U;                              /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Disable the IRDA DMA Rx request if enabled */
  if (HAL_IS_BIT_SET(hirda->Instance->CR3, USART_CR3_DMAR))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    CLEAR_BIT(hirda->Instance->CR3, USART_CR3_DMAR);    /* 执行语句 */

    /* Abort the IRDA DMA Rx channel : use non blocking DMA Abort API (callback) */
    if (hirda->hdmarx != NULL)                          /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* IRDA Rx DMA Abort callback has already been initialised :
         will lead to call HAL_IRDA_AbortCpltCallback() at end of DMA abort procedure */

      /* Abort DMA RX */
      if (HAL_DMA_Abort_IT(hirda->hdmarx) != HAL_OK)    /* 条件判断 */
      {                                                 /* 代码块开始 */
        hirda->hdmarx->XferAbortCallback = NULL;        /* 赋值或初始化 */
        AbortCplt = 0x01U;                              /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        AbortCplt = 0x00U;                              /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* if no DMA abort complete callback execution is required => call user Abort Complete callback */
  if (AbortCplt == 0x01U)                               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Reset Tx and Rx transfer counters */
    hirda->TxXferCount = 0x00U;                         /* 赋值或初始化 */
    hirda->RxXferCount = 0x00U;                         /* 赋值或初始化 */

    /* Reset ErrorCode */
    hirda->ErrorCode = HAL_IRDA_ERROR_NONE;             /* 赋值或初始化 */

    /* Restore hirda->gState and hirda->RxState to Ready */
    hirda->gState  = HAL_IRDA_STATE_READY;              /* 赋值或初始化 */
    hirda->RxState = HAL_IRDA_STATE_READY;              /* 赋值或初始化 */

    /* As no DMA to be aborted, call directly user Abort complete callback */
#if (USE_HAL_IRDA_REGISTER_CALLBACKS == 1)              /* 条件编译 */
    /* Call registered Abort complete callback */
    hirda->AbortCpltCallback(hirda);                    /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /* Call legacy weak Abort complete callback */
    HAL_IRDA_AbortCpltCallback(hirda);                  /* HAL库函数调用 */
#endif /* USE_HAL_IRDA_REGISTER_CALLBACK */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Abort ongoing Transmit transfer (Interrupt mode).
  * @param  hirda IRDA handle.
  * @note   This procedure could be used for aborting any ongoing transfer started in Interrupt or DMA mode.
  *         This procedure performs following operations :
  *           - Disable IRDA Interrupts (Tx)
  *           - Disable the DMA transfer in the peripheral register (if enabled)
  *           - Abort DMA transfer by calling HAL_DMA_Abort_IT (in case of transfer in DMA mode)
  *           - Set handle State to READY
  *           - At abort completion, call user abort complete callback
  * @note   This procedure is executed in Interrupt mode, meaning that abort procedure could be
  *         considered as completed only when user abort complete callback is executed (not when exiting function).
  * @retval HAL status
*/
HAL_StatusTypeDef HAL_IRDA_AbortTransmit_IT(IRDA_HandleTypeDef *hirda)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Disable TXEIE and TCIE interrupts */
  CLEAR_BIT(hirda->Instance->CR1, (USART_CR1_TXEIE | USART_CR1_TCIE));  /* 执行语句 */

  /* Disable the IRDA DMA Tx request if enabled */
  if (HAL_IS_BIT_SET(hirda->Instance->CR3, USART_CR3_DMAT))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    CLEAR_BIT(hirda->Instance->CR3, USART_CR3_DMAT);    /* 执行语句 */

    /* Abort the IRDA DMA Tx channel : use non blocking DMA Abort API (callback) */
    if (hirda->hdmatx != NULL)                          /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the IRDA DMA Abort callback :
         will lead to call HAL_IRDA_AbortCpltCallback() at end of DMA abort procedure */
      hirda->hdmatx->XferAbortCallback = IRDA_DMATxOnlyAbortCallback;  /* 赋值或初始化 */

      /* Abort DMA TX */
      if (HAL_DMA_Abort_IT(hirda->hdmatx) != HAL_OK)    /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Call Directly hirda->hdmatx->XferAbortCallback function in case of error */
        hirda->hdmatx->XferAbortCallback(hirda->hdmatx);  /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Reset Tx transfer counter */
      hirda->TxXferCount = 0x00U;                       /* 赋值或初始化 */

      /* Restore hirda->gState to Ready */
      hirda->gState = HAL_IRDA_STATE_READY;             /* 赋值或初始化 */

      /* As no DMA to be aborted, call directly user Abort complete callback */
#if (USE_HAL_IRDA_REGISTER_CALLBACKS == 1)              /* 条件编译 */
      /* Call registered Abort Transmit Complete Callback */
      hirda->AbortTransmitCpltCallback(hirda);          /* 执行语句 */
#else                                                   /* 条件编译否则 */
      /* Call legacy weak Abort Transmit Complete Callback */
      HAL_IRDA_AbortTransmitCpltCallback(hirda);        /* HAL库函数调用 */
#endif /* USE_HAL_IRDA_REGISTER_CALLBACK */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Reset Tx transfer counter */
    hirda->TxXferCount = 0x00U;                         /* 赋值或初始化 */

    /* Restore hirda->gState to Ready */
    hirda->gState = HAL_IRDA_STATE_READY;               /* 赋值或初始化 */

    /* As no DMA to be aborted, call directly user Abort complete callback */
#if (USE_HAL_IRDA_REGISTER_CALLBACKS == 1)              /* 条件编译 */
    /* Call registered Abort Transmit Complete Callback */
    hirda->AbortTransmitCpltCallback(hirda);            /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /* Call legacy weak Abort Transmit Complete Callback */
    HAL_IRDA_AbortTransmitCpltCallback(hirda);          /* HAL库函数调用 */
#endif /* USE_HAL_IRDA_REGISTER_CALLBACK */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Abort ongoing Receive transfer (Interrupt mode).
  * @param  hirda IRDA handle.
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
HAL_StatusTypeDef HAL_IRDA_AbortReceive_IT(IRDA_HandleTypeDef *hirda)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Disable RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts */
  CLEAR_BIT(hirda->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE));  /* 执行语句 */
  CLEAR_BIT(hirda->Instance->CR3, USART_CR3_EIE);       /* 执行语句 */

  /* Disable the IRDA DMA Rx request if enabled */
  if (HAL_IS_BIT_SET(hirda->Instance->CR3, USART_CR3_DMAR))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    CLEAR_BIT(hirda->Instance->CR3, USART_CR3_DMAR);    /* 执行语句 */

    /* Abort the IRDA DMA Rx channel : use non blocking DMA Abort API (callback) */
    if (hirda->hdmarx != NULL)                          /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the IRDA DMA Abort callback :
         will lead to call HAL_IRDA_AbortCpltCallback() at end of DMA abort procedure */
      hirda->hdmarx->XferAbortCallback = IRDA_DMARxOnlyAbortCallback;  /* 赋值或初始化 */

      /* Abort DMA RX */
      if (HAL_DMA_Abort_IT(hirda->hdmarx) != HAL_OK)    /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Call Directly hirda->hdmarx->XferAbortCallback function in case of error */
        hirda->hdmarx->XferAbortCallback(hirda->hdmarx);  /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Reset Rx transfer counter */
      hirda->RxXferCount = 0x00U;                       /* 赋值或初始化 */

      /* Restore hirda->RxState to Ready */
      hirda->RxState = HAL_IRDA_STATE_READY;            /* 赋值或初始化 */

      /* As no DMA to be aborted, call directly user Abort complete callback */
#if (USE_HAL_IRDA_REGISTER_CALLBACKS == 1)              /* 条件编译 */
      /* Call registered Abort Receive Complete Callback */
      hirda->AbortReceiveCpltCallback(hirda);           /* 执行语句 */
#else                                                   /* 条件编译否则 */
      /* Call legacy weak Abort Receive Complete Callback */
      HAL_IRDA_AbortReceiveCpltCallback(hirda);         /* HAL库函数调用 */
#endif /* USE_HAL_IRDA_REGISTER_CALLBACK */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Reset Rx transfer counter */
    hirda->RxXferCount = 0x00U;                         /* 赋值或初始化 */

    /* Restore hirda->RxState to Ready */
    hirda->RxState = HAL_IRDA_STATE_READY;              /* 赋值或初始化 */

    /* As no DMA to be aborted, call directly user Abort complete callback */
#if (USE_HAL_IRDA_REGISTER_CALLBACKS == 1)              /* 条件编译 */
    /* Call registered Abort Receive Complete Callback */
    hirda->AbortReceiveCpltCallback(hirda);             /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /* Call legacy weak Abort Receive Complete Callback */
    HAL_IRDA_AbortReceiveCpltCallback(hirda);           /* HAL库函数调用 */
#endif /* USE_HAL_IRDA_REGISTER_CALLBACK */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  This function handles IRDA interrupt request.
  * @param  hirda  Pointer to a IRDA_HandleTypeDef structure that contains
  *                the configuration information for the specified IRDA module.
  * @retval None
  */
void HAL_IRDA_IRQHandler(IRDA_HandleTypeDef *hirda)     /* 中断服务函数 */
{                                                       /* 代码块开始 */
  uint32_t isrflags   = READ_REG(hirda->Instance->SR);  /* 32位无符号变量/函数 */
  uint32_t cr1its     = READ_REG(hirda->Instance->CR1);  /* 32位无符号变量/函数 */
  uint32_t cr3its     = READ_REG(hirda->Instance->CR3);  /* 32位无符号变量/函数 */
  uint32_t errorflags = 0x00U;                          /* 32位无符号变量/函数 */
  uint32_t dmarequest = 0x00U;                          /* 32位无符号变量/函数 */

  /* If no error occurs */
  errorflags = (isrflags & (uint32_t)(USART_SR_PE | USART_SR_FE | USART_SR_ORE | USART_SR_NE));  /* 赋值或初始化 */
  if (errorflags == RESET)                              /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* IRDA in mode Receiver -----------------------------------------------*/
    if (((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      IRDA_Receive_IT(hirda);                           /* 执行语句 */
      return;                                           /* 返回 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* If some errors occur */
  if ((errorflags != RESET) && (((cr3its & USART_CR3_EIE) != RESET) || ((cr1its & (USART_CR1_RXNEIE | USART_CR1_PEIE)) != RESET)))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* IRDA parity error interrupt occurred -------------------------------*/
    if (((isrflags & USART_SR_PE) != RESET) && ((cr1its & USART_CR1_PEIE) != RESET))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hirda->ErrorCode |= HAL_IRDA_ERROR_PE;            /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* IRDA noise error interrupt occurred --------------------------------*/
    if (((isrflags & USART_SR_NE) != RESET) && ((cr3its & USART_CR3_EIE) != RESET))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hirda->ErrorCode |= HAL_IRDA_ERROR_NE;            /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* IRDA frame error interrupt occurred --------------------------------*/
    if (((isrflags & USART_SR_FE) != RESET) && ((cr3its & USART_CR3_EIE) != RESET))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hirda->ErrorCode |= HAL_IRDA_ERROR_FE;            /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* IRDA Over-Run interrupt occurred -----------------------------------*/
    if (((isrflags & USART_SR_ORE) != RESET) && (((cr1its & USART_CR1_RXNEIE) != RESET) || ((cr3its & USART_CR3_EIE) != RESET)))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hirda->ErrorCode |= HAL_IRDA_ERROR_ORE;           /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    /* Call IRDA Error Call back function if need be -----------------------*/
    if (hirda->ErrorCode != HAL_IRDA_ERROR_NONE)        /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* IRDA in mode Receiver ---------------------------------------------*/
      if (((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        IRDA_Receive_IT(hirda);                         /* 执行语句 */
      }                                                 /* 代码块结束 */

      /* If Overrun error occurs, or if any error occurs in DMA mode reception,
         consider error as blocking */
      dmarequest = HAL_IS_BIT_SET(hirda->Instance->CR3, USART_CR3_DMAR);  /* 赋值或初始化 */
      if (((hirda->ErrorCode & HAL_IRDA_ERROR_ORE) != RESET) || dmarequest)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Blocking error : transfer is aborted
           Set the IRDA state ready to be able to start again the process,
           Disable Rx Interrupts, and disable Rx DMA request, if ongoing */
        IRDA_EndRxTransfer(hirda);                      /* 执行语句 */

        /* Disable the IRDA DMA Rx request if enabled */
        if (HAL_IS_BIT_SET(hirda->Instance->CR3, USART_CR3_DMAR))  /* 条件判断 */
        {                                               /* 代码块开始 */
          CLEAR_BIT(hirda->Instance->CR3, USART_CR3_DMAR);  /* 执行语句 */

          /* Abort the IRDA DMA Rx channel */
          if (hirda->hdmarx != NULL)                    /* 条件判断 */
          {                                             /* 代码块开始 */
            /* Set the IRDA DMA Abort callback :
            will lead to call HAL_IRDA_ErrorCallback() at end of DMA abort procedure */
            hirda->hdmarx->XferAbortCallback = IRDA_DMAAbortOnError;  /* 赋值或初始化 */

            /* Abort DMA RX */
            if (HAL_DMA_Abort_IT(hirda->hdmarx) != HAL_OK)  /* 条件判断 */
            {                                           /* 代码块开始 */
              /* Call Directly XferAbortCallback function in case of error */
              hirda->hdmarx->XferAbortCallback(hirda->hdmarx);  /* 执行语句 */
            }                                           /* 代码块结束 */
          }                                             /* 代码块结束 */
          else                                          /* 否则 */
          {                                             /* 代码块开始 */
#if (USE_HAL_IRDA_REGISTER_CALLBACKS == 1)              /* 条件编译 */
            /* Call registered user error callback */
            hirda->ErrorCallback(hirda);                /* 执行语句 */
#else                                                   /* 条件编译否则 */
            /* Call legacy weak user error callback */
            HAL_IRDA_ErrorCallback(hirda);              /* HAL库函数调用 */
#endif /* USE_HAL_IRDA_REGISTER_CALLBACK */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
#if (USE_HAL_IRDA_REGISTER_CALLBACKS == 1)              /* 条件编译 */
          /* Call registered user error callback */
          hirda->ErrorCallback(hirda);                  /* 执行语句 */
#else                                                   /* 条件编译否则 */
          /* Call legacy weak user error callback */
          HAL_IRDA_ErrorCallback(hirda);                /* HAL库函数调用 */
#endif /* USE_HAL_IRDA_REGISTER_CALLBACK */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Non Blocking error : transfer could go on.
           Error is notified to user through user error callback */
#if (USE_HAL_IRDA_REGISTER_CALLBACKS == 1)              /* 条件编译 */
        /* Call registered user error callback */
        hirda->ErrorCallback(hirda);                    /* 执行语句 */
#else                                                   /* 条件编译否则 */
        /* Call legacy weak user error callback */
        HAL_IRDA_ErrorCallback(hirda);                  /* HAL库函数调用 */
#endif /* USE_HAL_IRDA_REGISTER_CALLBACK */

        hirda->ErrorCode = HAL_IRDA_ERROR_NONE;         /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    return;                                             /* 返回 */
  } /* End if some error occurs */

  /* IRDA in mode Transmitter ------------------------------------------------*/
  if (((isrflags & USART_SR_TXE) != RESET) && ((cr1its & USART_CR1_TXEIE) != RESET))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    IRDA_Transmit_IT(hirda);                            /* 执行语句 */
    return;                                             /* 返回 */
  }                                                     /* 代码块结束 */

  /* IRDA in mode Transmitter end --------------------------------------------*/
  if (((isrflags & USART_SR_TC) != RESET) && ((cr1its & USART_CR1_TCIE) != RESET))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    IRDA_EndTransmit_IT(hirda);                         /* 执行语句 */
    return;                                             /* 返回 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Tx Transfer complete callback.
  * @param  hirda  Pointer to a IRDA_HandleTypeDef structure that contains
  *                the configuration information for the specified IRDA module.
  * @retval None
  */
__weak void HAL_IRDA_TxCpltCallback(IRDA_HandleTypeDef *hirda)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hirda);                                        /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_IRDA_TxCpltCallback can be implemented in the user file.
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Tx Half Transfer completed callback.
  * @param  hirda  Pointer to a IRDA_HandleTypeDef structure that contains
  *                the configuration information for the specified USART module.
  * @retval None
  */
__weak void HAL_IRDA_TxHalfCpltCallback(IRDA_HandleTypeDef *hirda)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hirda);                                        /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_IRDA_TxHalfCpltCallback can be implemented in the user file.
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Rx Transfer complete callback.
  * @param  hirda  Pointer to a IRDA_HandleTypeDef structure that contains
  *                the configuration information for the specified IRDA module.
  * @retval None
  */
__weak void HAL_IRDA_RxCpltCallback(IRDA_HandleTypeDef *hirda)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hirda);                                        /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_IRDA_RxCpltCallback can be implemented in the user file.
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Rx Half Transfer complete callback.
  * @param  hirda  Pointer to a IRDA_HandleTypeDef structure that contains
  *                the configuration information for the specified IRDA module.
  * @retval None
  */
__weak void HAL_IRDA_RxHalfCpltCallback(IRDA_HandleTypeDef *hirda)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hirda);                                        /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_IRDA_RxHalfCpltCallback can be implemented in the user file.
   */
}                                                       /* 代码块结束 */

/**
  * @brief  IRDA error callback.
  * @param  hirda  Pointer to a IRDA_HandleTypeDef structure that contains
  *                the configuration information for the specified IRDA module.
  * @retval None
  */
__weak void HAL_IRDA_ErrorCallback(IRDA_HandleTypeDef *hirda)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hirda);                                        /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_IRDA_ErrorCallback can be implemented in the user file.
   */
}                                                       /* 代码块结束 */

/**
  * @brief  IRDA Abort Complete callback.
  * @param  hirda Pointer to a IRDA_HandleTypeDef structure that contains
  *                the configuration information for the specified IRDA module.
  * @retval None
  */
__weak void HAL_IRDA_AbortCpltCallback(IRDA_HandleTypeDef *hirda)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hirda);                                        /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_IRDA_AbortCpltCallback can be implemented in the user file.
   */
}                                                       /* 代码块结束 */

/**
  * @brief  IRDA Abort Transmit Complete callback.
  * @param  hirda Pointer to a IRDA_HandleTypeDef structure that contains
  *                the configuration information for the specified IRDA module.
  * @retval None
  */
__weak void HAL_IRDA_AbortTransmitCpltCallback(IRDA_HandleTypeDef *hirda)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hirda);                                        /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_IRDA_AbortTransmitCpltCallback can be implemented in the user file.
   */
}                                                       /* 代码块结束 */

/**
  * @brief  IRDA Abort Receive Complete callback.
  * @param  hirda Pointer to a IRDA_HandleTypeDef structure that contains
  *                the configuration information for the specified IRDA module.
  * @retval None
  */
__weak void HAL_IRDA_AbortReceiveCpltCallback(IRDA_HandleTypeDef *hirda)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hirda);                                        /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_IRDA_AbortReceiveCpltCallback can be implemented in the user file.
   */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup IRDA_Exported_Functions_Group3 Peripheral State and Errors functions
  *  @brief   IRDA State and Errors functions
  *
@verbatim
  ==============================================================================
                  ##### Peripheral State and Errors functions #####
  ==============================================================================
  [..]
    This subsection provides a set of functions allowing to return the State of IrDA
    communication process and also return Peripheral Errors occurred during communication process
     (+) HAL_IRDA_GetState() API can be helpful to check in run-time the state of the IrDA peripheral.
     (+) HAL_IRDA_GetError() check in run-time errors that could be occurred during communication.

@endverbatim
  * @{
  */

/**
  * @brief  Return the IRDA state.
  * @param  hirda  Pointer to a IRDA_HandleTypeDef structure that contains
  *                the configuration information for the specified IRDA.
  * @retval HAL state
  */
HAL_IRDA_StateTypeDef HAL_IRDA_GetState(IRDA_HandleTypeDef *hirda)  /* HAL库函数调用 */
{                                                       /* 代码块开始 */
  uint32_t temp1 = 0x00U, temp2 = 0x00U;                /* 32位无符号变量/函数 */
  temp1 = hirda->gState;                                /* 赋值或初始化 */
  temp2 = hirda->RxState;                               /* 赋值或初始化 */

  return (HAL_IRDA_StateTypeDef)(temp1 | temp2);        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return the IRDA error code
  * @param  hirda  Pointer to a IRDA_HandleTypeDef structure that contains
  *              the configuration information for the specified IRDA.
  * @retval IRDA Error Code
  */
uint32_t HAL_IRDA_GetError(IRDA_HandleTypeDef *hirda)   /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  return hirda->ErrorCode;                              /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup IRDA_Private_Functions IRDA Private Functions
  * @{
  */

#if (USE_HAL_IRDA_REGISTER_CALLBACKS == 1)              /* 条件编译 */
/**
  * @brief  Initialize the callbacks to their default values.
  * @param  hirda IRDA handle.
  * @retval none
  */
void IRDA_InitCallbacksToDefault(IRDA_HandleTypeDef *hirda)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Init the IRDA Callback settings */
  hirda->TxHalfCpltCallback        = HAL_IRDA_TxHalfCpltCallback;        /* Legacy weak TxHalfCpltCallback        */
  hirda->TxCpltCallback            = HAL_IRDA_TxCpltCallback;            /* Legacy weak TxCpltCallback            */
  hirda->RxHalfCpltCallback        = HAL_IRDA_RxHalfCpltCallback;        /* Legacy weak RxHalfCpltCallback        */
  hirda->RxCpltCallback            = HAL_IRDA_RxCpltCallback;            /* Legacy weak RxCpltCallback            */
  hirda->ErrorCallback             = HAL_IRDA_ErrorCallback;             /* Legacy weak ErrorCallback             */
  hirda->AbortCpltCallback         = HAL_IRDA_AbortCpltCallback;         /* Legacy weak AbortCpltCallback         */
  hirda->AbortTransmitCpltCallback = HAL_IRDA_AbortTransmitCpltCallback; /* Legacy weak AbortTransmitCpltCallback */
  hirda->AbortReceiveCpltCallback  = HAL_IRDA_AbortReceiveCpltCallback;  /* Legacy weak AbortReceiveCpltCallback  */

}                                                       /* 代码块结束 */
#endif /* USE_HAL_IRDA_REGISTER_CALLBACKS */

/**
  * @brief  DMA IRDA transmit process complete callback.
  * @param  hdma  Pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA.
  * @retval None
  */
static void IRDA_DMATransmitCplt(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  IRDA_HandleTypeDef *hirda = (IRDA_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;  /* 赋值或初始化 */
  /* DMA Normal mode */
  if ((hdma->Instance->CR & DMA_SxCR_CIRC) == 0U)       /* 条件判断 */
  {                                                     /* 代码块开始 */
    hirda->TxXferCount = 0U;                            /* 赋值或初始化 */

    /* Disable the DMA transfer for transmit request by resetting the DMAT bit
       in the IRDA CR3 register */
    CLEAR_BIT(hirda->Instance->CR3, USART_CR3_DMAT);    /* 执行语句 */

    /* Enable the IRDA Transmit Complete Interrupt */
    SET_BIT(hirda->Instance->CR1, USART_CR1_TCIE);      /* 执行语句 */
  }                                                     /* 代码块结束 */
  /* DMA Circular mode */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
#if (USE_HAL_IRDA_REGISTER_CALLBACKS == 1)              /* 条件编译 */
    /* Call registered Tx complete callback */
    hirda->TxCpltCallback(hirda);                       /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /* Call legacy weak Tx complete callback */
    HAL_IRDA_TxCpltCallback(hirda);                     /* HAL库函数调用 */
#endif /* USE_HAL_IRDA_REGISTER_CALLBACK */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief DMA IRDA receive process half complete callback
  * @param  hdma  Pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA.
  * @retval None
  */
static void IRDA_DMATransmitHalfCplt(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  IRDA_HandleTypeDef *hirda = (IRDA_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;  /* 赋值或初始化 */

#if (USE_HAL_IRDA_REGISTER_CALLBACKS == 1)              /* 条件编译 */
  /* Call registered Tx Half complete callback */
  hirda->TxHalfCpltCallback(hirda);                     /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* Call legacy weak Tx complete callback */
  HAL_IRDA_TxHalfCpltCallback(hirda);                   /* HAL库函数调用 */
#endif /* USE_HAL_IRDA_REGISTER_CALLBACK */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA IRDA receive process complete callback.
  * @param  hdma  Pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA.
  * @retval None
  */
static void IRDA_DMAReceiveCplt(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  IRDA_HandleTypeDef *hirda = (IRDA_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;  /* 赋值或初始化 */

  /* DMA Normal mode */
  if ((hdma->Instance->CR & DMA_SxCR_CIRC) == 0U)       /* 条件判断 */
  {                                                     /* 代码块开始 */
    hirda->RxXferCount = 0U;                            /* 赋值或初始化 */

    /* Disable PE and ERR (Frame error, noise error, overrun error) interrupts */
    CLEAR_BIT(hirda->Instance->CR1, USART_CR1_PEIE);    /* 执行语句 */
    CLEAR_BIT(hirda->Instance->CR3, USART_CR3_EIE);     /* 执行语句 */

    /* Disable the DMA transfer for the receiver request by resetting the DMAR bit
       in the IRDA CR3 register */
    CLEAR_BIT(hirda->Instance->CR3, USART_CR3_DMAR);    /* 执行语句 */

    /* At end of Rx process, restore hirda->RxState to Ready */
    hirda->RxState = HAL_IRDA_STATE_READY;              /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

#if (USE_HAL_IRDA_REGISTER_CALLBACKS == 1)              /* 条件编译 */
  /* Call registered Rx complete callback */
  hirda->RxCpltCallback(hirda);                         /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* Call legacy weak Rx complete callback */
  HAL_IRDA_RxCpltCallback(hirda);                       /* HAL库函数调用 */
#endif /* USE_HAL_IRDA_REGISTER_CALLBACKS */
}                                                       /* 代码块结束 */

/**
  * @brief DMA IRDA receive process half complete callback.
  * @param  hdma  Pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA.
  * @retval None
  */
static void IRDA_DMAReceiveHalfCplt(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  IRDA_HandleTypeDef *hirda = (IRDA_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;  /* 赋值或初始化 */

#if (USE_HAL_IRDA_REGISTER_CALLBACKS == 1)              /* 条件编译 */
  /*Call registered Rx Half complete callback*/
  hirda->RxHalfCpltCallback(hirda);                     /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* Call legacy weak Rx Half complete callback */
  HAL_IRDA_RxHalfCpltCallback(hirda);                   /* HAL库函数调用 */
#endif /* USE_HAL_IRDA_REGISTER_CALLBACK */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA IRDA communication error callback.
  * @param  hdma  Pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA.
  * @retval None
  */
static void IRDA_DMAError(DMA_HandleTypeDef *hdma)      /* 静态函数 */
{                                                       /* 代码块开始 */
  uint32_t dmarequest = 0x00U;                          /* 32位无符号变量/函数 */
  IRDA_HandleTypeDef *hirda = (IRDA_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;  /* 赋值或初始化 */

  /* Stop IRDA DMA Tx request if ongoing */
  dmarequest = HAL_IS_BIT_SET(hirda->Instance->CR3, USART_CR3_DMAT);  /* 赋值或初始化 */
  if ((hirda->gState == HAL_IRDA_STATE_BUSY_TX) && dmarequest)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hirda->TxXferCount = 0U;                            /* 赋值或初始化 */
    IRDA_EndTxTransfer(hirda);                          /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Stop IRDA DMA Rx request if ongoing */
  dmarequest = HAL_IS_BIT_SET(hirda->Instance->CR3, USART_CR3_DMAR);  /* 赋值或初始化 */
  if ((hirda->RxState == HAL_IRDA_STATE_BUSY_RX) && dmarequest)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hirda->RxXferCount = 0U;                            /* 赋值或初始化 */
    IRDA_EndRxTransfer(hirda);                          /* 执行语句 */
  }                                                     /* 代码块结束 */

  hirda->ErrorCode |= HAL_IRDA_ERROR_DMA;               /* 赋值或初始化 */

#if (USE_HAL_IRDA_REGISTER_CALLBACKS == 1)              /* 条件编译 */
  /* Call registered user error callback */
  hirda->ErrorCallback(hirda);                          /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* Call legacy weak user error callback */
  HAL_IRDA_ErrorCallback(hirda);                        /* HAL库函数调用 */
#endif /* USE_HAL_IRDA_REGISTER_CALLBACK */
}                                                       /* 代码块结束 */

/**
  * @brief  This function handles IRDA Communication Timeout.
  * @param  hirda  Pointer to a IRDA_HandleTypeDef structure that contains
  *                the configuration information for the specified IRDA.
  * @param  Flag specifies the IRDA flag to check.
  * @param  Status The new Flag status (SET or RESET).
  * @param  Tickstart Tick start value
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
static HAL_StatusTypeDef IRDA_WaitOnFlagUntilTimeout(IRDA_HandleTypeDef *hirda, uint32_t Flag, FlagStatus Status, uint32_t Tickstart, uint32_t Timeout)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  /* Wait until flag is set */
  while ((__HAL_IRDA_GET_FLAG(hirda, Flag) ? SET : RESET) == Status)  /* while循环 */
  {                                                     /* 代码块开始 */
    /* Check for the Timeout */
    if (Timeout != HAL_MAX_DELAY)                       /* 条件判断 */
    {                                                   /* 代码块开始 */
      if ((Timeout == 0U) || ((HAL_GetTick() - Tickstart) > Timeout))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Disable TXE, RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts for the interrupt process */
        CLEAR_BIT(hirda->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE | USART_CR1_TXEIE));  /* 执行语句 */
        CLEAR_BIT(hirda->Instance->CR3, USART_CR3_EIE);  /* 执行语句 */

        hirda->gState  = HAL_IRDA_STATE_READY;          /* 赋值或初始化 */
        hirda->RxState = HAL_IRDA_STATE_READY;          /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hirda);                            /* HAL辅助宏/函数 */

        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  End ongoing Tx transfer on IRDA peripheral (following error detection or Transmit completion).
  * @param  hirda IRDA handle.
  * @retval None
  */
static void IRDA_EndTxTransfer(IRDA_HandleTypeDef *hirda)  /* 静态函数 */
{                                                       /* 代码块开始 */
  /* Disable TXEIE and TCIE interrupts */
  CLEAR_BIT(hirda->Instance->CR1, (USART_CR1_TXEIE | USART_CR1_TCIE));  /* 执行语句 */

  /* At end of Tx process, restore hirda->gState to Ready */
  hirda->gState = HAL_IRDA_STATE_READY;                 /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  End ongoing Rx transfer on IRDA peripheral (following error detection or Reception completion).
  * @param  hirda IRDA handle.
  * @retval None
  */
static void IRDA_EndRxTransfer(IRDA_HandleTypeDef *hirda)  /* 静态函数 */
{                                                       /* 代码块开始 */
  /* Disable RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts */
  CLEAR_BIT(hirda->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE));  /* 执行语句 */
  CLEAR_BIT(hirda->Instance->CR3, USART_CR3_EIE);       /* 执行语句 */

  /* At end of Rx process, restore hirda->RxState to Ready */
  hirda->RxState = HAL_IRDA_STATE_READY;                /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA IRDA communication abort callback, when initiated by HAL services on Error
  *         (To be called at end of DMA Abort procedure following error occurrence).
  * @param  hdma DMA handle.
  * @retval None
  */
static void IRDA_DMAAbortOnError(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  IRDA_HandleTypeDef *hirda = (IRDA_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;  /* 赋值或初始化 */
  hirda->RxXferCount = 0x00U;                           /* 赋值或初始化 */
  hirda->TxXferCount = 0x00U;                           /* 赋值或初始化 */

#if (USE_HAL_IRDA_REGISTER_CALLBACKS == 1)              /* 条件编译 */
  /* Call registered user error callback */
  hirda->ErrorCallback(hirda);                          /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* Call legacy weak user error callback */
  HAL_IRDA_ErrorCallback(hirda);                        /* HAL库函数调用 */
#endif /* USE_HAL_IRDA_REGISTER_CALLBACK */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA IRDA Tx communication abort callback, when initiated by user
  *         (To be called at end of DMA Tx Abort procedure following user abort request).
  * @note   When this callback is executed, User Abort complete call back is called only if no
  *         Abort still ongoing for Rx DMA Handle.
  * @param  hdma DMA handle.
  * @retval None
  */
static void IRDA_DMATxAbortCallback(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  IRDA_HandleTypeDef *hirda = (IRDA_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;  /* 赋值或初始化 */

  hirda->hdmatx->XferAbortCallback = NULL;              /* 赋值或初始化 */

  /* Check if an Abort process is still ongoing */
  if (hirda->hdmarx != NULL)                            /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (hirda->hdmarx->XferAbortCallback != NULL)       /* 条件判断 */
    {                                                   /* 代码块开始 */
      return;                                           /* 返回 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* No Abort process still ongoing : All DMA channels are aborted, call user Abort Complete callback */
  hirda->TxXferCount = 0x00U;                           /* 赋值或初始化 */
  hirda->RxXferCount = 0x00U;                           /* 赋值或初始化 */

  /* Reset ErrorCode */
  hirda->ErrorCode = HAL_IRDA_ERROR_NONE;               /* 赋值或初始化 */

  /* Restore hirda->gState and hirda->RxState to Ready */
  hirda->gState  = HAL_IRDA_STATE_READY;                /* 赋值或初始化 */
  hirda->RxState = HAL_IRDA_STATE_READY;                /* 赋值或初始化 */

  /* Call user Abort complete callback */
#if (USE_HAL_IRDA_REGISTER_CALLBACKS == 1)              /* 条件编译 */
  /* Call registered Abort complete callback */
  hirda->AbortCpltCallback(hirda);                      /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* Call legacy weak Abort complete callback */
  HAL_IRDA_AbortCpltCallback(hirda);                    /* HAL库函数调用 */
#endif /* USE_HAL_IRDA_REGISTER_CALLBACK */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA IRDA Rx communication abort callback, when initiated by user
  *         (To be called at end of DMA Rx Abort procedure following user abort request).
  * @note   When this callback is executed, User Abort complete call back is called only if no
  *         Abort still ongoing for Tx DMA Handle.
  * @param  hdma DMA handle.
  * @retval None
  */
static void IRDA_DMARxAbortCallback(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  IRDA_HandleTypeDef *hirda = (IRDA_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;  /* 赋值或初始化 */

  hirda->hdmarx->XferAbortCallback = NULL;              /* 赋值或初始化 */

  /* Check if an Abort process is still ongoing */
  if (hirda->hdmatx != NULL)                            /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (hirda->hdmatx->XferAbortCallback != NULL)       /* 条件判断 */
    {                                                   /* 代码块开始 */
      return;                                           /* 返回 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* No Abort process still ongoing : All DMA channels are aborted, call user Abort Complete callback */
  hirda->TxXferCount = 0x00U;                           /* 赋值或初始化 */
  hirda->RxXferCount = 0x00U;                           /* 赋值或初始化 */

  /* Reset ErrorCode */
  hirda->ErrorCode = HAL_IRDA_ERROR_NONE;               /* 赋值或初始化 */

  /* Restore hirda->gState and hirda->RxState to Ready */
  hirda->gState  = HAL_IRDA_STATE_READY;                /* 赋值或初始化 */
  hirda->RxState = HAL_IRDA_STATE_READY;                /* 赋值或初始化 */

  /* Call user Abort complete callback */
#if (USE_HAL_IRDA_REGISTER_CALLBACKS == 1)              /* 条件编译 */
  /* Call registered Abort complete callback */
  hirda->AbortCpltCallback(hirda);                      /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* Call legacy weak Abort complete callback */
  HAL_IRDA_AbortCpltCallback(hirda);                    /* HAL库函数调用 */
#endif /* USE_HAL_IRDA_REGISTER_CALLBACK */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA IRDA Tx communication abort callback, when initiated by user by a call to
  *         HAL_IRDA_AbortTransmit_IT API (Abort only Tx transfer)
  *         (This callback is executed at end of DMA Tx Abort procedure following user abort request,
  *         and leads to user Tx Abort Complete callback execution).
  * @param  hdma DMA handle.
  * @retval None
  */
static void IRDA_DMATxOnlyAbortCallback(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  IRDA_HandleTypeDef *hirda = (IRDA_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;  /* 赋值或初始化 */

  hirda->TxXferCount = 0x00U;                           /* 赋值或初始化 */

  /* Restore hirda->gState to Ready */
  hirda->gState = HAL_IRDA_STATE_READY;                 /* 赋值或初始化 */

  /* Call user Abort complete callback */
#if (USE_HAL_IRDA_REGISTER_CALLBACKS == 1)              /* 条件编译 */
  /* Call registered Abort Transmit Complete Callback */
  hirda->AbortTransmitCpltCallback(hirda);              /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* Call legacy weak Abort Transmit Complete Callback */
  HAL_IRDA_AbortTransmitCpltCallback(hirda);            /* HAL库函数调用 */
#endif /* USE_HAL_IRDA_REGISTER_CALLBACK */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA IRDA Rx communication abort callback, when initiated by user by a call to
  *         HAL_IRDA_AbortReceive_IT API (Abort only Rx transfer)
  *         (This callback is executed at end of DMA Rx Abort procedure following user abort request,
  *         and leads to user Rx Abort Complete callback execution).
  * @param  hdma DMA handle.
  * @retval None
  */
static void IRDA_DMARxOnlyAbortCallback(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  IRDA_HandleTypeDef *hirda = (IRDA_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;  /* 赋值或初始化 */

  hirda->RxXferCount = 0x00U;                           /* 赋值或初始化 */

  /* Restore hirda->RxState to Ready */
  hirda->RxState = HAL_IRDA_STATE_READY;                /* 赋值或初始化 */

  /* Call user Abort complete callback */
#if (USE_HAL_IRDA_REGISTER_CALLBACKS == 1)              /* 条件编译 */
  /* Call registered Abort Receive Complete Callback */
  hirda->AbortReceiveCpltCallback(hirda);               /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* Call legacy weak Abort Receive Complete Callback */
  HAL_IRDA_AbortReceiveCpltCallback(hirda);             /* HAL库函数调用 */
#endif /* USE_HAL_IRDA_REGISTER_CALLBACK */
}                                                       /* 代码块结束 */

/**
 * @brief  Send an amount of data in non blocking mode.
 * @param  hirda  Pointer to a IRDA_HandleTypeDef structure that contains
 *                the configuration information for the specified IRDA module.
 * @retval HAL status
 */
static HAL_StatusTypeDef IRDA_Transmit_IT(IRDA_HandleTypeDef *hirda)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  uint16_t *tmp;                                        /* 16位无符号变量/函数 */

  /* Check that a Tx process is ongoing */
  if (hirda->gState == HAL_IRDA_STATE_BUSY_TX)          /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (hirda->Init.WordLength == IRDA_WORDLENGTH_9B)   /* 条件判断 */
    {                                                   /* 代码块开始 */
      tmp = (uint16_t *) hirda->pTxBuffPtr;             /* 赋值或初始化 */
      hirda->Instance->DR = (uint16_t)(*tmp & (uint16_t)0x01FF);  /* 赋值或初始化 */
      if (hirda->Init.Parity == IRDA_PARITY_NONE)       /* 条件判断 */
      {                                                 /* 代码块开始 */
        hirda->pTxBuffPtr += 2U;                        /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        hirda->pTxBuffPtr += 1U;                        /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hirda->Instance->DR = (uint8_t)(*hirda->pTxBuffPtr++ & (uint8_t)0x00FF);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    if (--hirda->TxXferCount == 0U)                     /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable the IRDA Transmit Data Register Empty Interrupt */
      CLEAR_BIT(hirda->Instance->CR1, USART_CR1_TXEIE);  /* 执行语句 */

      /* Enable the IRDA Transmit Complete Interrupt */
      SET_BIT(hirda->Instance->CR1, USART_CR1_TCIE);    /* 执行语句 */
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
  * @param  hirda  Pointer to a IRDA_HandleTypeDef structure that contains
  *                the configuration information for the specified IRDA module.
  * @retval HAL status
  */
static HAL_StatusTypeDef IRDA_EndTransmit_IT(IRDA_HandleTypeDef *hirda)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  /* Disable the IRDA Transmit Complete Interrupt */
  CLEAR_BIT(hirda->Instance->CR1, USART_CR1_TCIE);      /* 执行语句 */

  /* Disable the IRDA Error Interrupt: (Frame error, noise error, overrun error) */
  CLEAR_BIT(hirda->Instance->CR3, USART_CR3_EIE);       /* 执行语句 */

  /* Tx process is ended, restore hirda->gState to Ready */
  hirda->gState = HAL_IRDA_STATE_READY;                 /* 赋值或初始化 */

#if (USE_HAL_IRDA_REGISTER_CALLBACKS == 1)              /* 条件编译 */
  /* Call registered Tx complete callback */
  hirda->TxCpltCallback(hirda);                         /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* Call legacy weak Tx complete callback */
  HAL_IRDA_TxCpltCallback(hirda);                       /* HAL库函数调用 */
#endif /* USE_HAL_IRDA_REGISTER_CALLBACK */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Receives an amount of data in non blocking mode.
  * @param  hirda  Pointer to a IRDA_HandleTypeDef structure that contains
  *                the configuration information for the specified IRDA module.
  * @retval HAL status
  */
static HAL_StatusTypeDef IRDA_Receive_IT(IRDA_HandleTypeDef *hirda)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  uint16_t *tmp;                                        /* 16位无符号变量/函数 */
  uint16_t  uhdata;                                     /* 16位无符号变量/函数 */

  /* Check that a Rx process is ongoing */
  if (hirda->RxState == HAL_IRDA_STATE_BUSY_RX)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    uhdata = (uint16_t) READ_REG(hirda->Instance->DR);  /* 赋值或初始化 */
    if (hirda->Init.WordLength == IRDA_WORDLENGTH_9B)   /* 条件判断 */
    {                                                   /* 代码块开始 */
      tmp = (uint16_t *) hirda->pRxBuffPtr;             /* 赋值或初始化 */
      if (hirda->Init.Parity == IRDA_PARITY_NONE)       /* 条件判断 */
      {                                                 /* 代码块开始 */
        *tmp = (uint16_t)(uhdata & (uint16_t)0x01FF);   /* 赋值或初始化 */
        hirda->pRxBuffPtr += 2U;                        /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        *tmp = (uint16_t)(uhdata & (uint16_t)0x00FF);   /* 赋值或初始化 */
        hirda->pRxBuffPtr += 1U;                        /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      if (hirda->Init.Parity == IRDA_PARITY_NONE)       /* 条件判断 */
      {                                                 /* 代码块开始 */
        *hirda->pRxBuffPtr++ = (uint8_t)(uhdata & (uint8_t)0x00FF);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        *hirda->pRxBuffPtr++ = (uint8_t)(uhdata & (uint8_t)0x007F);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    if (--hirda->RxXferCount == 0U)                     /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable the IRDA Data Register not empty Interrupt */
      CLEAR_BIT(hirda->Instance->CR1, USART_CR1_RXNEIE);  /* 执行语句 */

      /* Disable the IRDA Parity Error Interrupt */
      CLEAR_BIT(hirda->Instance->CR1, USART_CR1_PEIE);  /* 执行语句 */

      /* Disable the IRDA Error Interrupt: (Frame error, noise error, overrun error) */
      CLEAR_BIT(hirda->Instance->CR3, USART_CR3_EIE);   /* 执行语句 */

      /* Rx process is completed, restore hirda->RxState to Ready */
      hirda->RxState = HAL_IRDA_STATE_READY;            /* 赋值或初始化 */

#if (USE_HAL_IRDA_REGISTER_CALLBACKS == 1)              /* 条件编译 */
      /* Call registered Rx complete callback */
      hirda->RxCpltCallback(hirda);                     /* 执行语句 */
#else                                                   /* 条件编译否则 */
      /* Call legacy weak Rx complete callback */
      HAL_IRDA_RxCpltCallback(hirda);                   /* HAL库函数调用 */
#endif /* USE_HAL_IRDA_REGISTER_CALLBACKS */

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
  * @brief  Configures the IRDA peripheral.
  * @param  hirda  Pointer to a IRDA_HandleTypeDef structure that contains
  *                the configuration information for the specified IRDA module.
  * @retval None
  */
static void IRDA_SetConfig(IRDA_HandleTypeDef *hirda)   /* 静态函数 */
{                                                       /* 代码块开始 */
  uint32_t pclk;                                        /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_IRDA_INSTANCE(hirda->Instance));      /* 执行语句 */
  assert_param(IS_IRDA_BAUDRATE(hirda->Init.BaudRate));  /* 执行语句 */
  assert_param(IS_IRDA_WORD_LENGTH(hirda->Init.WordLength));  /* 执行语句 */
  assert_param(IS_IRDA_PARITY(hirda->Init.Parity));     /* 执行语句 */
  assert_param(IS_IRDA_MODE(hirda->Init.Mode));         /* 执行语句 */
  assert_param(IS_IRDA_POWERMODE(hirda->Init.IrDAMode));  /* 执行语句 */

  /*-------------------------- USART CR2 Configuration ------------------------*/
  /* Clear STOP[13:12] bits */
  CLEAR_BIT(hirda->Instance->CR2, USART_CR2_STOP);      /* 执行语句 */

  /*-------------------------- USART CR1 Configuration -----------------------*/
  /* Clear M, PCE, PS, TE and RE bits */
  CLEAR_BIT(hirda->Instance->CR1, (USART_CR1_M | USART_CR1_PCE | USART_CR1_PS | USART_CR1_TE | USART_CR1_RE));  /* 执行语句 */

  /* Configure the USART Word Length, Parity and mode:
     Set the M bits according to hirda->Init.WordLength value
     Set PCE and PS bits according to hirda->Init.Parity value
     Set TE and RE bits according to hirda->Init.Mode value */
  /* Write to USART CR1 */
  SET_BIT(hirda->Instance->CR1, (hirda->Init.WordLength | hirda->Init.Parity | hirda->Init.Mode));  /* 执行语句 */

  /*-------------------------- USART CR3 Configuration -----------------------*/
  /* Clear CTSE and RTSE bits */
  CLEAR_BIT(hirda->Instance->CR3, (USART_CR3_RTSE | USART_CR3_CTSE));  /* 执行语句 */

  /*-------------------------- USART BRR Configuration -----------------------*/
#if defined(USART6) && defined(UART9) && defined(UART10)  /* 条件编译 */
   if ((hirda->Instance == USART1) || (hirda->Instance == USART6) || (hirda->Instance == UART9) || (hirda->Instance == UART10))  /* 条件判断 */
   {                                                    /* 代码块开始 */
    pclk = HAL_RCC_GetPCLK2Freq();                      /* 赋值或初始化 */
    SET_BIT(hirda->Instance->BRR, IRDA_BRR(pclk, hirda->Init.BaudRate));  /* 执行语句 */
   }                                                    /* 代码块结束 */
#elif defined(USART6)                                   /* 条件编译否则如果 */
  if((hirda->Instance == USART1) || (hirda->Instance == USART6))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    pclk = HAL_RCC_GetPCLK2Freq();                      /* 赋值或初始化 */
    SET_BIT(hirda->Instance->BRR, IRDA_BRR(pclk, hirda->Init.BaudRate));  /* 执行语句 */
  }                                                     /* 代码块结束 */
#else                                                   /* 条件编译否则 */
  if(hirda->Instance == USART1)                         /* 条件判断 */
  {                                                     /* 代码块开始 */
    pclk = HAL_RCC_GetPCLK2Freq();                      /* 赋值或初始化 */
    SET_BIT(hirda->Instance->BRR, IRDA_BRR(pclk, hirda->Init.BaudRate));  /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif /* USART6 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    pclk = HAL_RCC_GetPCLK1Freq();                      /* 赋值或初始化 */
    SET_BIT(hirda->Instance->BRR, IRDA_BRR(pclk, hirda->Init.BaudRate));  /* 执行语句 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

#endif /* HAL_IRDA_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
