/* 文件: stm32f4xx_hal_uart.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_uart.c
  * @author  MCD Application Team
  * @brief   UART HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Universal Asynchronous Receiver Transmitter Peripheral (UART).
  *           + Initialization and de-initialization functions
  *           + IO operation functions
  *           + Peripheral Control functions
  *           + Peripheral State and Errors functions
  @verbatim
  ==============================================================================
                        ##### How to use this driver #####
  ==============================================================================
  [..]
    The UART HAL driver can be used as follows:

    (#) Declare a UART_HandleTypeDef handle structure (eg. UART_HandleTypeDef huart).
    (#) Initialize the UART low level resources by implementing the HAL_UART_MspInit() API:
        (##) Enable the USARTx interface clock.
        (##) UART pins configuration:
            (+++) Enable the clock for the UART GPIOs.
            (+++) Configure these UART pins (TX as alternate function pull-up, RX as alternate function Input).
        (##) NVIC configuration if you need to use interrupt process (HAL_UART_Transmit_IT()
             and HAL_UART_Receive_IT() APIs):
            (+++) Configure the USARTx interrupt priority.
            (+++) Enable the NVIC USART IRQ handle.
        (##) DMA Configuration if you need to use DMA process (HAL_UART_Transmit_DMA()
             and HAL_UART_Receive_DMA() APIs):
            (+++) Declare a DMA handle structure for the Tx/Rx stream.
            (+++) Enable the DMAx interface clock.
            (+++) Configure the declared DMA handle structure with the required
                  Tx/Rx parameters.
            (+++) Configure the DMA Tx/Rx stream.
            (+++) Associate the initialized DMA handle to the UART DMA Tx/Rx handle.
            (+++) Configure the priority and enable the NVIC for the transfer complete
                  interrupt on the DMA Tx/Rx stream.
            (+++) Configure the USARTx interrupt priority and enable the NVIC USART IRQ handle
                  (used for last byte sending completion detection in DMA non circular mode)

    (#) Program the Baud Rate, Word Length, Stop Bit, Parity, Hardware
        flow control and Mode(Receiver/Transmitter) in the huart Init structure.

    (#) For the UART asynchronous mode, initialize the UART registers by calling
        the HAL_UART_Init() API.

    (#) For the UART Half duplex mode, initialize the UART registers by calling
        the HAL_HalfDuplex_Init() API.

    (#) For the LIN mode, initialize the UART registers by calling the HAL_LIN_Init() API.

    (#) For the Multi-Processor mode, initialize the UART registers by calling
        the HAL_MultiProcessor_Init() API.

     [..]
       (@) The specific UART interrupts (Transmission complete interrupt,
            RXNE interrupt and Error Interrupts) will be managed using the macros
            __HAL_UART_ENABLE_IT() and __HAL_UART_DISABLE_IT() inside the transmit
            and receive process.

     [..]
       (@) These APIs (HAL_UART_Init() and HAL_HalfDuplex_Init()) configure also the
            low level Hardware GPIO, CLOCK, CORTEX...etc) by calling the customized
            HAL_UART_MspInit() API.

    ##### Callback registration #####
    ==================================

    [..]
    The compilation define USE_HAL_UART_REGISTER_CALLBACKS when set to 1
    allows the user to configure dynamically the driver callbacks.

    [..]
    Use Function @ref HAL_UART_RegisterCallback() to register a user callback.
    Function @ref HAL_UART_RegisterCallback() allows to register following callbacks:
    (+) TxHalfCpltCallback        : Tx Half Complete Callback.
    (+) TxCpltCallback            : Tx Complete Callback.
    (+) RxHalfCpltCallback        : Rx Half Complete Callback.
    (+) RxCpltCallback            : Rx Complete Callback.
    (+) ErrorCallback             : Error Callback.
    (+) AbortCpltCallback         : Abort Complete Callback.
    (+) AbortTransmitCpltCallback : Abort Transmit Complete Callback.
    (+) AbortReceiveCpltCallback  : Abort Receive Complete Callback.
    (+) MspInitCallback           : UART MspInit.
    (+) MspDeInitCallback         : UART MspDeInit.
    This function takes as parameters the HAL peripheral handle, the Callback ID
    and a pointer to the user callback function.

    [..]
    Use function @ref HAL_UART_UnRegisterCallback() to reset a callback to the default
    weak (surcharged) function.
    @ref HAL_UART_UnRegisterCallback() takes as parameters the HAL peripheral handle,
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
    (+) MspInitCallback           : UART MspInit.
    (+) MspDeInitCallback         : UART MspDeInit.

    [..]
    For specific callback RxEventCallback, use dedicated registration/reset functions:
    respectively @ref HAL_UART_RegisterRxEventCallback() , @ref HAL_UART_UnRegisterRxEventCallback().

    [..]
    By default, after the @ref HAL_UART_Init() and when the state is HAL_UART_STATE_RESET
    all callbacks are set to the corresponding weak (surcharged) functions:
    examples @ref HAL_UART_TxCpltCallback(), @ref HAL_UART_RxHalfCpltCallback().
    Exception done for MspInit and MspDeInit functions that are respectively
    reset to the legacy weak (surcharged) functions in the @ref HAL_UART_Init()
    and @ref HAL_UART_DeInit() only when these callbacks are null (not registered beforehand).
    If not, MspInit or MspDeInit are not null, the @ref HAL_UART_Init() and @ref HAL_UART_DeInit()
    keep and use the user MspInit/MspDeInit callbacks (registered beforehand).

    [..]
    Callbacks can be registered/unregistered in HAL_UART_STATE_READY state only.
    Exception done MspInit/MspDeInit that can be registered/unregistered
    in HAL_UART_STATE_READY or HAL_UART_STATE_RESET state, thus registered (user)
    MspInit/DeInit callbacks can be used during the Init/DeInit.
    In that case first register the MspInit/MspDeInit user callbacks
    using @ref HAL_UART_RegisterCallback() before calling @ref HAL_UART_DeInit()
    or @ref HAL_UART_Init() function.

    [..]
    When The compilation define USE_HAL_UART_REGISTER_CALLBACKS is set to 0 or
    not defined, the callback registration feature is not available
    and weak (surcharged) callbacks are used.

     [..]
        Three operation modes are available within this driver :

     *** Polling mode IO operation ***
     =================================
     [..]
       (+) Send an amount of data in blocking mode using HAL_UART_Transmit()
       (+) Receive an amount of data in blocking mode using HAL_UART_Receive()

     *** Interrupt mode IO operation ***
     ===================================
     [..]
       (+) Send an amount of data in non blocking mode using HAL_UART_Transmit_IT()
       (+) At transmission end of transfer HAL_UART_TxCpltCallback is executed and user can
            add his own code by customization of function pointer HAL_UART_TxCpltCallback
       (+) Receive an amount of data in non blocking mode using HAL_UART_Receive_IT()
       (+) At reception end of transfer HAL_UART_RxCpltCallback is executed and user can
            add his own code by customization of function pointer HAL_UART_RxCpltCallback
       (+) In case of transfer Error, HAL_UART_ErrorCallback() function is executed and user can
            add his own code by customization of function pointer HAL_UART_ErrorCallback

     *** DMA mode IO operation ***
     ==============================
     [..]
       (+) Send an amount of data in non blocking mode (DMA) using HAL_UART_Transmit_DMA()
       (+) At transmission end of half transfer HAL_UART_TxHalfCpltCallback is executed and user can
            add his own code by customization of function pointer HAL_UART_TxHalfCpltCallback
       (+) At transmission end of transfer HAL_UART_TxCpltCallback is executed and user can
            add his own code by customization of function pointer HAL_UART_TxCpltCallback
       (+) Receive an amount of data in non blocking mode (DMA) using HAL_UART_Receive_DMA()
       (+) At reception end of half transfer HAL_UART_RxHalfCpltCallback is executed and user can
            add his own code by customization of function pointer HAL_UART_RxHalfCpltCallback
       (+) At reception end of transfer HAL_UART_RxCpltCallback is executed and user can
            add his own code by customization of function pointer HAL_UART_RxCpltCallback
       (+) In case of transfer Error, HAL_UART_ErrorCallback() function is executed and user can
            add his own code by customization of function pointer HAL_UART_ErrorCallback
       (+) Pause the DMA Transfer using HAL_UART_DMAPause()
       (+) Resume the DMA Transfer using HAL_UART_DMAResume()
       (+) Stop the DMA Transfer using HAL_UART_DMAStop()


    [..] This subsection also provides a set of additional functions providing enhanced reception
    services to user. (For example, these functions allow application to handle use cases
    where number of data to be received is unknown).

    (#) Compared to standard reception services which only consider number of received
        data elements as reception completion criteria, these functions also consider additional events
        as triggers for updating reception status to caller :
       (+) Detection of inactivity period (RX line has not been active for a given period).
          (++) RX inactivity detected by IDLE event, i.e. RX line has been in idle state (normally high state)
               for 1 frame time, after last received byte.

    (#) There are two mode of transfer:
       (+) Blocking mode: The reception is performed in polling mode, until either expected number of data is received,
           or till IDLE event occurs. Reception is handled only during function execution.
           When function exits, no data reception could occur. HAL status and number of actually received data elements,
           are returned by function after finishing transfer.
       (+) Non-Blocking mode: The reception is performed using Interrupts or DMA.
           These API's return the HAL status.
           The end of the data processing will be indicated through the
           dedicated UART IRQ when using Interrupt mode or the DMA IRQ when using DMA mode.
           The HAL_UARTEx_RxEventCallback() user callback will be executed during Receive process
           The HAL_UART_ErrorCallback()user callback will be executed when a reception error is detected.

    (#) Blocking mode API:
        (+) HAL_UARTEx_ReceiveToIdle()

    (#) Non-Blocking mode API with Interrupt:
        (+) HAL_UARTEx_ReceiveToIdle_IT()

    (#) Non-Blocking mode API with DMA:
        (+) HAL_UARTEx_ReceiveToIdle_DMA()


     *** UART HAL driver macros list ***
     =============================================
     [..]
       Below the list of most used macros in UART HAL driver.

      (+) __HAL_UART_ENABLE: Enable the UART peripheral
      (+) __HAL_UART_DISABLE: Disable the UART peripheral
      (+) __HAL_UART_GET_FLAG : Check whether the specified UART flag is set or not
      (+) __HAL_UART_CLEAR_FLAG : Clear the specified UART pending flag
      (+) __HAL_UART_ENABLE_IT: Enable the specified UART interrupt
      (+) __HAL_UART_DISABLE_IT: Disable the specified UART interrupt
      (+) __HAL_UART_GET_IT_SOURCE: Check whether the specified UART interrupt has occurred or not

     [..]
       (@) You can refer to the UART HAL driver header file for more useful macros

  @endverbatim
     [..]
       (@) Additional remark: If the parity is enabled, then the MSB bit of the data written
           in the data register is transmitted but is changed by the parity bit.
           Depending on the frame length defined by the M bit (8-bits or 9-bits),
           the possible UART frame formats are as listed in the following table:
    +-------------------------------------------------------------+
    |   M bit |  PCE bit  |            UART frame                 |
    |---------------------|---------------------------------------|
    |    0    |    0      |    | SB | 8 bit data | STB |          |
    |---------|-----------|---------------------------------------|
    |    0    |    1      |    | SB | 7 bit data | PB | STB |     |
    |---------|-----------|---------------------------------------|
    |    1    |    0      |    | SB | 9 bit data | STB |          |
    |---------|-----------|---------------------------------------|
    |    1    |    1      |    | SB | 8 bit data | PB | STB |     |
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

/** @defgroup UART UART
  * @brief HAL UART module driver
  * @{
  */
#ifdef HAL_UART_MODULE_ENABLED                          /* 条件编译 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @addtogroup UART_Private_Constants
  * @{
  */
/**
  * @}
  */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/** @addtogroup UART_Private_Functions  UART Private Functions
  * @{
  */

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)              /* 条件编译 */
void UART_InitCallbacksToDefault(UART_HandleTypeDef *huart);  /* 函数定义或声明 */
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
static void UART_EndTxTransfer(UART_HandleTypeDef *huart);  /* 静态函数 */
static void UART_EndRxTransfer(UART_HandleTypeDef *huart);  /* 静态函数 */
static void UART_DMATransmitCplt(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void UART_DMAReceiveCplt(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void UART_DMATxHalfCplt(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void UART_DMARxHalfCplt(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void UART_DMAError(DMA_HandleTypeDef *hdma);     /* 静态函数 */
static void UART_DMAAbortOnError(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void UART_DMATxAbortCallback(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void UART_DMARxAbortCallback(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void UART_DMATxOnlyAbortCallback(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void UART_DMARxOnlyAbortCallback(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static HAL_StatusTypeDef UART_Transmit_IT(UART_HandleTypeDef *huart);  /* 静态变量/函数 */
static HAL_StatusTypeDef UART_EndTransmit_IT(UART_HandleTypeDef *huart);  /* 静态变量/函数 */
static HAL_StatusTypeDef UART_Receive_IT(UART_HandleTypeDef *huart);  /* 静态变量/函数 */
static HAL_StatusTypeDef UART_WaitOnFlagUntilTimeout(UART_HandleTypeDef *huart, uint32_t Flag, FlagStatus Status, uint32_t Tickstart, uint32_t Timeout);  /* 静态变量/函数 */
static void UART_SetConfig(UART_HandleTypeDef *huart);  /* 静态函数 */

/**
  * @}
  */

/* Exported functions ---------------------------------------------------------*/
/** @defgroup UART_Exported_Functions UART Exported Functions
  * @{
  */

/** @defgroup UART_Exported_Functions_Group1 Initialization and de-initialization functions
  *  @brief    Initialization and Configuration functions
  *
@verbatim
 ===============================================================================
            ##### Initialization and Configuration functions #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to initialize the USARTx or the UARTy
    in asynchronous mode.
      (+) For the asynchronous mode only these parameters can be configured:
        (++) Baud Rate
        (++) Word Length
        (++) Stop Bit
        (++) Parity: If the parity is enabled, then the MSB bit of the data written
             in the data register is transmitted but is changed by the parity bit.
             Depending on the frame length defined by the M bit (8-bits or 9-bits),
             please refer to Reference manual for possible UART frame formats.
        (++) Hardware flow control
        (++) Receiver/transmitter modes
        (++) Over Sampling Method
    [..]
    The HAL_UART_Init(), HAL_HalfDuplex_Init(), HAL_LIN_Init() and HAL_MultiProcessor_Init() APIs
    follow respectively the UART asynchronous, UART Half duplex, LIN and Multi-Processor configuration
    procedures (details for the procedures are available in reference manual
    (RM0430 for STM32F4X3xx MCUs and RM0402 for STM32F412xx MCUs
     RM0383 for STM32F411xC/E MCUs and RM0401 for STM32F410xx MCUs
     RM0090 for STM32F4X5xx/STM32F4X7xx/STM32F429xx/STM32F439xx MCUs
     RM0390 for STM32F446xx MCUs and RM0386 for STM32F469xx/STM32F479xx MCUs)).

@endverbatim
  * @{
  */

/**
  * @brief  Initializes the UART mode according to the specified parameters in
  *         the UART_InitTypeDef and create the associated handle.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_UART_Init(UART_HandleTypeDef *huart)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the UART handle allocation */
  if (huart == NULL)                                    /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the parameters */
  if (huart->Init.HwFlowCtl != UART_HWCONTROL_NONE)     /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* The hardware flow control is available only for USART1, USART2, USART3 and USART6.
       Except for STM32F446xx devices, that is available for USART1, USART2, USART3, USART6, UART4 and UART5.
    */
    assert_param(IS_UART_HWFLOW_INSTANCE(huart->Instance));  /* 执行语句 */
    assert_param(IS_UART_HARDWARE_FLOW_CONTROL(huart->Init.HwFlowCtl));  /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    assert_param(IS_UART_INSTANCE(huart->Instance));    /* 执行语句 */
  }                                                     /* 代码块结束 */
  assert_param(IS_UART_WORD_LENGTH(huart->Init.WordLength));  /* 执行语句 */
  assert_param(IS_UART_OVERSAMPLING(huart->Init.OverSampling));  /* 执行语句 */

  if (huart->gState == HAL_UART_STATE_RESET)            /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Allocate lock resource and initialize it */
    huart->Lock = HAL_UNLOCKED;                         /* 赋值或初始化 */

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)              /* 条件编译 */
    UART_InitCallbacksToDefault(huart);                 /* 执行语句 */

    if (huart->MspInitCallback == NULL)                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      huart->MspInitCallback = HAL_UART_MspInit;        /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Init the low level hardware */
    huart->MspInitCallback(huart);                      /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /* Init the low level hardware : GPIO, CLOCK */
    HAL_UART_MspInit(huart);                            /* HAL 串口操作 */
#endif /* (USE_HAL_UART_REGISTER_CALLBACKS) */
  }                                                     /* 代码块结束 */

  huart->gState = HAL_UART_STATE_BUSY;                  /* 赋值或初始化 */

  /* Disable the peripheral */
  __HAL_UART_DISABLE(huart);                            /* HAL辅助宏/函数 */

  /* Set the UART Communication parameters */
  UART_SetConfig(huart);                                /* 执行语句 */

  /* In asynchronous mode, the following bits must be kept cleared:
     - LINEN and CLKEN bits in the USART_CR2 register,
     - SCEN, HDSEL and IREN  bits in the USART_CR3 register.*/
  CLEAR_BIT(huart->Instance->CR2, (USART_CR2_LINEN | USART_CR2_CLKEN));  /* 执行语句 */
  CLEAR_BIT(huart->Instance->CR3, (USART_CR3_SCEN | USART_CR3_HDSEL | USART_CR3_IREN));  /* 执行语句 */

  /* Enable the peripheral */
  __HAL_UART_ENABLE(huart);                             /* HAL辅助宏/函数 */

  /* Initialize the UART state */
  huart->ErrorCode = HAL_UART_ERROR_NONE;               /* 赋值或初始化 */
  huart->gState = HAL_UART_STATE_READY;                 /* 赋值或初始化 */
  huart->RxState = HAL_UART_STATE_READY;                /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initializes the half-duplex mode according to the specified
  *         parameters in the UART_InitTypeDef and create the associated handle.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_HalfDuplex_Init(UART_HandleTypeDef *huart)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the UART handle allocation */
  if (huart == NULL)                                    /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the parameters */
  assert_param(IS_UART_HALFDUPLEX_INSTANCE(huart->Instance));  /* 执行语句 */
  assert_param(IS_UART_WORD_LENGTH(huart->Init.WordLength));  /* 执行语句 */
  assert_param(IS_UART_OVERSAMPLING(huart->Init.OverSampling));  /* 执行语句 */

  if (huart->gState == HAL_UART_STATE_RESET)            /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Allocate lock resource and initialize it */
    huart->Lock = HAL_UNLOCKED;                         /* 赋值或初始化 */

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)              /* 条件编译 */
    UART_InitCallbacksToDefault(huart);                 /* 执行语句 */

    if (huart->MspInitCallback == NULL)                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      huart->MspInitCallback = HAL_UART_MspInit;        /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Init the low level hardware */
    huart->MspInitCallback(huart);                      /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /* Init the low level hardware : GPIO, CLOCK */
    HAL_UART_MspInit(huart);                            /* HAL 串口操作 */
#endif /* (USE_HAL_UART_REGISTER_CALLBACKS) */
  }                                                     /* 代码块结束 */

  huart->gState = HAL_UART_STATE_BUSY;                  /* 赋值或初始化 */

  /* Disable the peripheral */
  __HAL_UART_DISABLE(huart);                            /* HAL辅助宏/函数 */

  /* Set the UART Communication parameters */
  UART_SetConfig(huart);                                /* 执行语句 */

  /* In half-duplex mode, the following bits must be kept cleared:
     - LINEN and CLKEN bits in the USART_CR2 register,
     - SCEN and IREN bits in the USART_CR3 register.*/
  CLEAR_BIT(huart->Instance->CR2, (USART_CR2_LINEN | USART_CR2_CLKEN));  /* 执行语句 */
  CLEAR_BIT(huart->Instance->CR3, (USART_CR3_IREN | USART_CR3_SCEN));  /* 执行语句 */

  /* Enable the Half-Duplex mode by setting the HDSEL bit in the CR3 register */
  SET_BIT(huart->Instance->CR3, USART_CR3_HDSEL);       /* 执行语句 */

  /* Enable the peripheral */
  __HAL_UART_ENABLE(huart);                             /* HAL辅助宏/函数 */

  /* Initialize the UART state*/
  huart->ErrorCode = HAL_UART_ERROR_NONE;               /* 赋值或初始化 */
  huart->gState = HAL_UART_STATE_READY;                 /* 赋值或初始化 */
  huart->RxState = HAL_UART_STATE_READY;                /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initializes the LIN mode according to the specified
  *         parameters in the UART_InitTypeDef and create the associated handle.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @param  BreakDetectLength Specifies the LIN break detection length.
  *         This parameter can be one of the following values:
  *            @arg UART_LINBREAKDETECTLENGTH_10B: 10-bit break detection
  *            @arg UART_LINBREAKDETECTLENGTH_11B: 11-bit break detection
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_LIN_Init(UART_HandleTypeDef *huart, uint32_t BreakDetectLength)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the UART handle allocation */
  if (huart == NULL)                                    /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the LIN UART instance */
  assert_param(IS_UART_LIN_INSTANCE(huart->Instance));  /* 执行语句 */

  /* Check the Break detection length parameter */
  assert_param(IS_UART_LIN_BREAK_DETECT_LENGTH(BreakDetectLength));  /* 执行语句 */
  assert_param(IS_UART_LIN_WORD_LENGTH(huart->Init.WordLength));  /* 执行语句 */
  assert_param(IS_UART_LIN_OVERSAMPLING(huart->Init.OverSampling));  /* 执行语句 */

  if (huart->gState == HAL_UART_STATE_RESET)            /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Allocate lock resource and initialize it */
    huart->Lock = HAL_UNLOCKED;                         /* 赋值或初始化 */

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)              /* 条件编译 */
    UART_InitCallbacksToDefault(huart);                 /* 执行语句 */

    if (huart->MspInitCallback == NULL)                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      huart->MspInitCallback = HAL_UART_MspInit;        /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Init the low level hardware */
    huart->MspInitCallback(huart);                      /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /* Init the low level hardware : GPIO, CLOCK */
    HAL_UART_MspInit(huart);                            /* HAL 串口操作 */
#endif /* (USE_HAL_UART_REGISTER_CALLBACKS) */
  }                                                     /* 代码块结束 */

  huart->gState = HAL_UART_STATE_BUSY;                  /* 赋值或初始化 */

  /* Disable the peripheral */
  __HAL_UART_DISABLE(huart);                            /* HAL辅助宏/函数 */

  /* Set the UART Communication parameters */
  UART_SetConfig(huart);                                /* 执行语句 */

  /* In LIN mode, the following bits must be kept cleared:
     - CLKEN bits in the USART_CR2 register,
     - SCEN, HDSEL and IREN bits in the USART_CR3 register.*/
  CLEAR_BIT(huart->Instance->CR2, (USART_CR2_CLKEN));   /* 执行语句 */
  CLEAR_BIT(huart->Instance->CR3, (USART_CR3_HDSEL | USART_CR3_IREN | USART_CR3_SCEN));  /* 执行语句 */

  /* Enable the LIN mode by setting the LINEN bit in the CR2 register */
  SET_BIT(huart->Instance->CR2, USART_CR2_LINEN);       /* 执行语句 */

  /* Set the USART LIN Break detection length. */
  CLEAR_BIT(huart->Instance->CR2, USART_CR2_LBDL);      /* 执行语句 */
  SET_BIT(huart->Instance->CR2, BreakDetectLength);     /* 执行语句 */

  /* Enable the peripheral */
  __HAL_UART_ENABLE(huart);                             /* HAL辅助宏/函数 */

  /* Initialize the UART state*/
  huart->ErrorCode = HAL_UART_ERROR_NONE;               /* 赋值或初始化 */
  huart->gState = HAL_UART_STATE_READY;                 /* 赋值或初始化 */
  huart->RxState = HAL_UART_STATE_READY;                /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initializes the Multi-Processor mode according to the specified
  *         parameters in the UART_InitTypeDef and create the associated handle.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @param  Address USART address
  * @param  WakeUpMethod specifies the USART wake-up method.
  *         This parameter can be one of the following values:
  *            @arg UART_WAKEUPMETHOD_IDLELINE: Wake-up by an idle line detection
  *            @arg UART_WAKEUPMETHOD_ADDRESSMARK: Wake-up by an address mark
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MultiProcessor_Init(UART_HandleTypeDef *huart, uint8_t Address, uint32_t WakeUpMethod)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the UART handle allocation */
  if (huart == NULL)                                    /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the parameters */
  assert_param(IS_UART_INSTANCE(huart->Instance));      /* 执行语句 */

  /* Check the Address & wake up method parameters */
  assert_param(IS_UART_WAKEUPMETHOD(WakeUpMethod));     /* 执行语句 */
  assert_param(IS_UART_ADDRESS(Address));               /* 执行语句 */
  assert_param(IS_UART_WORD_LENGTH(huart->Init.WordLength));  /* 执行语句 */
  assert_param(IS_UART_OVERSAMPLING(huart->Init.OverSampling));  /* 执行语句 */

  if (huart->gState == HAL_UART_STATE_RESET)            /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Allocate lock resource and initialize it */
    huart->Lock = HAL_UNLOCKED;                         /* 赋值或初始化 */

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)              /* 条件编译 */
    UART_InitCallbacksToDefault(huart);                 /* 执行语句 */

    if (huart->MspInitCallback == NULL)                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      huart->MspInitCallback = HAL_UART_MspInit;        /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Init the low level hardware */
    huart->MspInitCallback(huart);                      /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /* Init the low level hardware : GPIO, CLOCK */
    HAL_UART_MspInit(huart);                            /* HAL 串口操作 */
#endif /* (USE_HAL_UART_REGISTER_CALLBACKS) */
  }                                                     /* 代码块结束 */

  huart->gState = HAL_UART_STATE_BUSY;                  /* 赋值或初始化 */

  /* Disable the peripheral */
  __HAL_UART_DISABLE(huart);                            /* HAL辅助宏/函数 */

  /* Set the UART Communication parameters */
  UART_SetConfig(huart);                                /* 执行语句 */

  /* In Multi-Processor mode, the following bits must be kept cleared:
     - LINEN and CLKEN bits in the USART_CR2 register,
     - SCEN, HDSEL and IREN  bits in the USART_CR3 register */
  CLEAR_BIT(huart->Instance->CR2, (USART_CR2_LINEN | USART_CR2_CLKEN));  /* 执行语句 */
  CLEAR_BIT(huart->Instance->CR3, (USART_CR3_SCEN | USART_CR3_HDSEL | USART_CR3_IREN));  /* 执行语句 */

  /* Set the USART address node */
  CLEAR_BIT(huart->Instance->CR2, USART_CR2_ADD);       /* 执行语句 */
  SET_BIT(huart->Instance->CR2, Address);               /* 执行语句 */

  /* Set the wake up method by setting the WAKE bit in the CR1 register */
  CLEAR_BIT(huart->Instance->CR1, USART_CR1_WAKE);      /* 执行语句 */
  SET_BIT(huart->Instance->CR1, WakeUpMethod);          /* 执行语句 */

  /* Enable the peripheral */
  __HAL_UART_ENABLE(huart);                             /* HAL辅助宏/函数 */

  /* Initialize the UART state */
  huart->ErrorCode = HAL_UART_ERROR_NONE;               /* 赋值或初始化 */
  huart->gState = HAL_UART_STATE_READY;                 /* 赋值或初始化 */
  huart->RxState = HAL_UART_STATE_READY;                /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  DeInitializes the UART peripheral.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_UART_DeInit(UART_HandleTypeDef *huart)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the UART handle allocation */
  if (huart == NULL)                                    /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the parameters */
  assert_param(IS_UART_INSTANCE(huart->Instance));      /* 执行语句 */

  huart->gState = HAL_UART_STATE_BUSY;                  /* 赋值或初始化 */

  /* Disable the Peripheral */
  __HAL_UART_DISABLE(huart);                            /* HAL辅助宏/函数 */

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)              /* 条件编译 */
  if (huart->MspDeInitCallback == NULL)                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    huart->MspDeInitCallback = HAL_UART_MspDeInit;      /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  /* DeInit the low level hardware */
  huart->MspDeInitCallback(huart);                      /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* DeInit the low level hardware */
  HAL_UART_MspDeInit(huart);                            /* HAL 串口操作 */
#endif /* (USE_HAL_UART_REGISTER_CALLBACKS) */

  huart->ErrorCode = HAL_UART_ERROR_NONE;               /* 赋值或初始化 */
  huart->gState = HAL_UART_STATE_RESET;                 /* 赋值或初始化 */
  huart->RxState = HAL_UART_STATE_RESET;                /* 赋值或初始化 */
  huart->ReceptionType = HAL_UART_RECEPTION_STANDARD;   /* 赋值或初始化 */

  /* Process Unlock */
  __HAL_UNLOCK(huart);                                  /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  UART MSP Init.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
__weak void HAL_UART_MspInit(UART_HandleTypeDef *huart)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);                                        /* 执行语句 */
  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_UART_MspInit could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  UART MSP DeInit.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
__weak void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);                                        /* 执行语句 */
  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_UART_MspDeInit could be implemented in the user file
   */
}                                                       /* 代码块结束 */

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)              /* 条件编译 */
/**
  * @brief  Register a User UART Callback
  *         To be used instead of the weak predefined callback
  * @param  huart uart handle
  * @param  CallbackID ID of the callback to be registered
  *         This parameter can be one of the following values:
  *           @arg @ref HAL_UART_TX_HALFCOMPLETE_CB_ID Tx Half Complete Callback ID
  *           @arg @ref HAL_UART_TX_COMPLETE_CB_ID Tx Complete Callback ID
  *           @arg @ref HAL_UART_RX_HALFCOMPLETE_CB_ID Rx Half Complete Callback ID
  *           @arg @ref HAL_UART_RX_COMPLETE_CB_ID Rx Complete Callback ID
  *           @arg @ref HAL_UART_ERROR_CB_ID Error Callback ID
  *           @arg @ref HAL_UART_ABORT_COMPLETE_CB_ID Abort Complete Callback ID
  *           @arg @ref HAL_UART_ABORT_TRANSMIT_COMPLETE_CB_ID Abort Transmit Complete Callback ID
  *           @arg @ref HAL_UART_ABORT_RECEIVE_COMPLETE_CB_ID Abort Receive Complete Callback ID
  *           @arg @ref HAL_UART_MSPINIT_CB_ID MspInit Callback ID
  *           @arg @ref HAL_UART_MSPDEINIT_CB_ID MspDeInit Callback ID
  * @param  pCallback pointer to the Callback function
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_UART_RegisterCallback(UART_HandleTypeDef *huart, HAL_UART_CallbackIDTypeDef CallbackID, pUART_CallbackTypeDef pCallback)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  if (pCallback == NULL)                                /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    huart->ErrorCode |= HAL_UART_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  /* Process locked */
  __HAL_LOCK(huart);                                    /* HAL辅助宏/函数 */

  if (huart->gState == HAL_UART_STATE_READY)            /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_UART_TX_HALFCOMPLETE_CB_ID :             /* case标签 */
        huart->TxHalfCpltCallback = pCallback;          /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_UART_TX_COMPLETE_CB_ID :                 /* case标签 */
        huart->TxCpltCallback = pCallback;              /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_UART_RX_HALFCOMPLETE_CB_ID :             /* case标签 */
        huart->RxHalfCpltCallback = pCallback;          /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_UART_RX_COMPLETE_CB_ID :                 /* case标签 */
        huart->RxCpltCallback = pCallback;              /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_UART_ERROR_CB_ID :                       /* case标签 */
        huart->ErrorCallback = pCallback;               /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_UART_ABORT_COMPLETE_CB_ID :              /* case标签 */
        huart->AbortCpltCallback = pCallback;           /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_UART_ABORT_TRANSMIT_COMPLETE_CB_ID :     /* case标签 */
        huart->AbortTransmitCpltCallback = pCallback;   /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_UART_ABORT_RECEIVE_COMPLETE_CB_ID :      /* case标签 */
        huart->AbortReceiveCpltCallback = pCallback;    /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_UART_MSPINIT_CB_ID :                     /* case标签 */
        huart->MspInitCallback = pCallback;             /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_UART_MSPDEINIT_CB_ID :                   /* case标签 */
        huart->MspDeInitCallback = pCallback;           /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        huart->ErrorCode |= HAL_UART_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (huart->gState == HAL_UART_STATE_RESET)       /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_UART_MSPINIT_CB_ID :                     /* case标签 */
        huart->MspInitCallback = pCallback;             /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_UART_MSPDEINIT_CB_ID :                   /* case标签 */
        huart->MspDeInitCallback = pCallback;           /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        huart->ErrorCode |= HAL_UART_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    huart->ErrorCode |= HAL_UART_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(huart);                                  /* HAL辅助宏/函数 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Unregister an UART Callback
  *         UART callaback is redirected to the weak predefined callback
  * @param  huart uart handle
  * @param  CallbackID ID of the callback to be unregistered
  *         This parameter can be one of the following values:
  *           @arg @ref HAL_UART_TX_HALFCOMPLETE_CB_ID Tx Half Complete Callback ID
  *           @arg @ref HAL_UART_TX_COMPLETE_CB_ID Tx Complete Callback ID
  *           @arg @ref HAL_UART_RX_HALFCOMPLETE_CB_ID Rx Half Complete Callback ID
  *           @arg @ref HAL_UART_RX_COMPLETE_CB_ID Rx Complete Callback ID
  *           @arg @ref HAL_UART_ERROR_CB_ID Error Callback ID
  *           @arg @ref HAL_UART_ABORT_COMPLETE_CB_ID Abort Complete Callback ID
  *           @arg @ref HAL_UART_ABORT_TRANSMIT_COMPLETE_CB_ID Abort Transmit Complete Callback ID
  *           @arg @ref HAL_UART_ABORT_RECEIVE_COMPLETE_CB_ID Abort Receive Complete Callback ID
  *           @arg @ref HAL_UART_MSPINIT_CB_ID MspInit Callback ID
  *           @arg @ref HAL_UART_MSPDEINIT_CB_ID MspDeInit Callback ID
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_UART_UnRegisterCallback(UART_HandleTypeDef *huart, HAL_UART_CallbackIDTypeDef CallbackID)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Process locked */
  __HAL_LOCK(huart);                                    /* HAL辅助宏/函数 */

  if (HAL_UART_STATE_READY == huart->gState)            /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_UART_TX_HALFCOMPLETE_CB_ID :             /* case标签 */
        huart->TxHalfCpltCallback = HAL_UART_TxHalfCpltCallback;               /* Legacy weak  TxHalfCpltCallback       */
        break;                                          /* 跳出 */

      case HAL_UART_TX_COMPLETE_CB_ID :                 /* case标签 */
        huart->TxCpltCallback = HAL_UART_TxCpltCallback;                       /* Legacy weak TxCpltCallback            */
        break;                                          /* 跳出 */

      case HAL_UART_RX_HALFCOMPLETE_CB_ID :             /* case标签 */
        huart->RxHalfCpltCallback = HAL_UART_RxHalfCpltCallback;               /* Legacy weak RxHalfCpltCallback        */
        break;                                          /* 跳出 */

      case HAL_UART_RX_COMPLETE_CB_ID :                 /* case标签 */
        huart->RxCpltCallback = HAL_UART_RxCpltCallback;                       /* Legacy weak RxCpltCallback            */
        break;                                          /* 跳出 */

      case HAL_UART_ERROR_CB_ID :                       /* case标签 */
        huart->ErrorCallback = HAL_UART_ErrorCallback;                         /* Legacy weak ErrorCallback             */
        break;                                          /* 跳出 */

      case HAL_UART_ABORT_COMPLETE_CB_ID :              /* case标签 */
        huart->AbortCpltCallback = HAL_UART_AbortCpltCallback;                 /* Legacy weak AbortCpltCallback         */
        break;                                          /* 跳出 */

      case HAL_UART_ABORT_TRANSMIT_COMPLETE_CB_ID :     /* case标签 */
        huart->AbortTransmitCpltCallback = HAL_UART_AbortTransmitCpltCallback; /* Legacy weak AbortTransmitCpltCallback */
        break;                                          /* 跳出 */

      case HAL_UART_ABORT_RECEIVE_COMPLETE_CB_ID :      /* case标签 */
        huart->AbortReceiveCpltCallback = HAL_UART_AbortReceiveCpltCallback;   /* Legacy weak AbortReceiveCpltCallback  */
        break;                                          /* 跳出 */

      case HAL_UART_MSPINIT_CB_ID :                     /* case标签 */
        huart->MspInitCallback = HAL_UART_MspInit;                             /* Legacy weak MspInitCallback           */
        break;                                          /* 跳出 */

      case HAL_UART_MSPDEINIT_CB_ID :                   /* case标签 */
        huart->MspDeInitCallback = HAL_UART_MspDeInit;                         /* Legacy weak MspDeInitCallback         */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        huart->ErrorCode |= HAL_UART_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (HAL_UART_STATE_RESET == huart->gState)       /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_UART_MSPINIT_CB_ID :                     /* case标签 */
        huart->MspInitCallback = HAL_UART_MspInit;      /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_UART_MSPDEINIT_CB_ID :                   /* case标签 */
        huart->MspDeInitCallback = HAL_UART_MspDeInit;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        huart->ErrorCode |= HAL_UART_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    huart->ErrorCode |= HAL_UART_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(huart);                                  /* HAL辅助宏/函数 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Register a User UART Rx Event Callback
  *         To be used instead of the weak predefined callback
  * @param  huart     Uart handle
  * @param  pCallback Pointer to the Rx Event Callback function
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_UART_RegisterRxEventCallback(UART_HandleTypeDef *huart, pUART_RxEventCallbackTypeDef pCallback)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  if (pCallback == NULL)                                /* 条件判断 */
  {                                                     /* 代码块开始 */
    huart->ErrorCode |= HAL_UART_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Process locked */
  __HAL_LOCK(huart);                                    /* HAL辅助宏/函数 */

  if (huart->gState == HAL_UART_STATE_READY)            /* 条件判断 */
  {                                                     /* 代码块开始 */
    huart->RxEventCallback = pCallback;                 /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    huart->ErrorCode |= HAL_UART_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(huart);                                  /* HAL辅助宏/函数 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  UnRegister the UART Rx Event Callback
  *         UART Rx Event Callback is redirected to the weak HAL_UARTEx_RxEventCallback() predefined callback
  * @param  huart     Uart handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_UART_UnRegisterRxEventCallback(UART_HandleTypeDef *huart)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Process locked */
  __HAL_LOCK(huart);                                    /* HAL辅助宏/函数 */

  if (huart->gState == HAL_UART_STATE_READY)            /* 条件判断 */
  {                                                     /* 代码块开始 */
    huart->RxEventCallback = HAL_UARTEx_RxEventCallback; /* Legacy weak UART Rx Event Callback  */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    huart->ErrorCode |= HAL_UART_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(huart);                                  /* HAL辅助宏/函数 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */

/**
  * @}
  */

/** @defgroup UART_Exported_Functions_Group2 IO operation functions
  *  @brief UART Transmit and Receive functions
  *
@verbatim
 ===============================================================================
                      ##### IO operation functions #####
 ===============================================================================
    This subsection provides a set of functions allowing to manage the UART asynchronous
    and Half duplex data transfers.

    (#) There are two modes of transfer:
       (+) Blocking mode: The communication is performed in polling mode.
           The HAL status of all data processing is returned by the same function
           after finishing transfer.
       (+) Non-Blocking mode: The communication is performed using Interrupts
           or DMA, these API's return the HAL status.
           The end of the data processing will be indicated through the
           dedicated UART IRQ when using Interrupt mode or the DMA IRQ when
           using DMA mode.
           The HAL_UART_TxCpltCallback(), HAL_UART_RxCpltCallback() user callbacks
           will be executed respectively at the end of the transmit or receive process
           The HAL_UART_ErrorCallback()user callback will be executed when a communication error is detected.

    (#) Blocking mode API's are :
        (+) HAL_UART_Transmit()
        (+) HAL_UART_Receive()

    (#) Non-Blocking mode API's with Interrupt are :
        (+) HAL_UART_Transmit_IT()
        (+) HAL_UART_Receive_IT()
        (+) HAL_UART_IRQHandler()

    (#) Non-Blocking mode API's with DMA are :
        (+) HAL_UART_Transmit_DMA()
        (+) HAL_UART_Receive_DMA()
        (+) HAL_UART_DMAPause()
        (+) HAL_UART_DMAResume()
        (+) HAL_UART_DMAStop()

    (#) A set of Transfer Complete Callbacks are provided in Non_Blocking mode:
        (+) HAL_UART_TxHalfCpltCallback()
        (+) HAL_UART_TxCpltCallback()
        (+) HAL_UART_RxHalfCpltCallback()
        (+) HAL_UART_RxCpltCallback()
        (+) HAL_UART_ErrorCallback()

    (#) Non-Blocking mode transfers could be aborted using Abort API's :
        (+) HAL_UART_Abort()
        (+) HAL_UART_AbortTransmit()
        (+) HAL_UART_AbortReceive()
        (+) HAL_UART_Abort_IT()
        (+) HAL_UART_AbortTransmit_IT()
        (+) HAL_UART_AbortReceive_IT()

    (#) For Abort services based on interrupts (HAL_UART_Abortxxx_IT), a set of Abort Complete Callbacks are provided:
        (+) HAL_UART_AbortCpltCallback()
        (+) HAL_UART_AbortTransmitCpltCallback()
        (+) HAL_UART_AbortReceiveCpltCallback()

    (#) A Rx Event Reception Callback (Rx event notification) is available for Non_Blocking modes of enhanced reception services:
        (+) HAL_UARTEx_RxEventCallback()

    (#) In Non-Blocking mode transfers, possible errors are split into 2 categories.
        Errors are handled as follows :
       (+) Error is considered as Recoverable and non blocking : Transfer could go till end, but error severity is
           to be evaluated by user : this concerns Frame Error, Parity Error or Noise Error in Interrupt mode reception .
           Received character is then retrieved and stored in Rx buffer, Error code is set to allow user to identify error type,
           and HAL_UART_ErrorCallback() user callback is executed. Transfer is kept ongoing on UART side.
           If user wants to abort it, Abort services should be called by user.
       (+) Error is considered as Blocking : Transfer could not be completed properly and is aborted.
           This concerns Overrun Error In Interrupt mode reception and all errors in DMA mode.
           Error code is set to allow user to identify error type, and HAL_UART_ErrorCallback() user callback is executed.

    -@- In the Half duplex communication, it is forbidden to run the transmit
        and receive process in parallel, the UART state HAL_UART_STATE_BUSY_TX_RX can't be useful.

@endverbatim
  * @{
  */

/**
  * @brief  Sends an amount of data in blocking mode.
  * @note   When UART parity is not enabled (PCE = 0), and Word Length is configured to 9 bits (M1-M0 = 01),
  *         the sent data is handled as a set of u16. In this case, Size must indicate the number
  *         of u16 provided through pData.
  * @param  huart Pointer to a UART_HandleTypeDef structure that contains
  *               the configuration information for the specified UART module.
  * @param  pData Pointer to data buffer (u8 or u16 data elements).
  * @param  Size  Amount of data elements (u8 or u16) to be sent
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint8_t  *pdata8bits;                                 /* 8位无符号变量/函数 */
  uint16_t *pdata16bits;                                /* 16位无符号变量/函数 */
  uint32_t tickstart = 0U;                              /* 32位无符号变量/函数 */

  /* Check that a Tx process is not already ongoing */
  if (huart->gState == HAL_UART_STATE_READY)            /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) || (Size == 0U))                /* 条件判断 */
    {                                                   /* 代码块开始 */
      return  HAL_ERROR;                                /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(huart);                                  /* HAL辅助宏/函数 */

    huart->ErrorCode = HAL_UART_ERROR_NONE;             /* 赋值或初始化 */
    huart->gState = HAL_UART_STATE_BUSY_TX;             /* 赋值或初始化 */

    /* Init tickstart for timeout management */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */

    huart->TxXferSize = Size;                           /* 赋值或初始化 */
    huart->TxXferCount = Size;                          /* 赋值或初始化 */

    /* In case of 9bits/No Parity transfer, pData needs to be handled as a uint16_t pointer */
    if ((huart->Init.WordLength == UART_WORDLENGTH_9B) && (huart->Init.Parity == UART_PARITY_NONE))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      pdata8bits  = NULL;                               /* 赋值或初始化 */
      pdata16bits = (uint16_t *) pData;                 /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      pdata8bits  = pData;                              /* 赋值或初始化 */
      pdata16bits = NULL;                               /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Process Unlocked */
    __HAL_UNLOCK(huart);                                /* HAL辅助宏/函数 */

    while (huart->TxXferCount > 0U)                     /* while循环 */
    {                                                   /* 代码块开始 */
      if (UART_WaitOnFlagUntilTimeout(huart, UART_FLAG_TXE, RESET, tickstart, Timeout) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
      if (pdata8bits == NULL)                           /* 条件判断 */
      {                                                 /* 代码块开始 */
        huart->Instance->DR = (uint16_t)(*pdata16bits & 0x01FFU);  /* 赋值或初始化 */
        pdata16bits++;                                  /* 执行语句 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        huart->Instance->DR = (uint8_t)(*pdata8bits & 0xFFU);  /* 赋值或初始化 */
        pdata8bits++;                                   /* 执行语句 */
      }                                                 /* 代码块结束 */
      huart->TxXferCount--;                             /* 执行语句 */
    }                                                   /* 代码块结束 */

    if (UART_WaitOnFlagUntilTimeout(huart, UART_FLAG_TC, RESET, tickstart, Timeout) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_TIMEOUT;                               /* 返回值 */
    }                                                   /* 代码块结束 */

    /* At end of Tx process, restore huart->gState to Ready */
    huart->gState = HAL_UART_STATE_READY;               /* 赋值或初始化 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Receives an amount of data in blocking mode.
  * @note   When UART parity is not enabled (PCE = 0), and Word Length is configured to 9 bits (M1-M0 = 01),
  *         the received data is handled as a set of u16. In this case, Size must indicate the number
  *         of u16 available through pData.
  * @param  huart Pointer to a UART_HandleTypeDef structure that contains
  *               the configuration information for the specified UART module.
  * @param  pData Pointer to data buffer (u8 or u16 data elements).
  * @param  Size  Amount of data elements (u8 or u16) to be received.
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_UART_Receive(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint8_t  *pdata8bits;                                 /* 8位无符号变量/函数 */
  uint16_t *pdata16bits;                                /* 16位无符号变量/函数 */
  uint32_t tickstart = 0U;                              /* 32位无符号变量/函数 */

  /* Check that a Rx process is not already ongoing */
  if (huart->RxState == HAL_UART_STATE_READY)           /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) || (Size == 0U))                /* 条件判断 */
    {                                                   /* 代码块开始 */
      return  HAL_ERROR;                                /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(huart);                                  /* HAL辅助宏/函数 */

    huart->ErrorCode = HAL_UART_ERROR_NONE;             /* 赋值或初始化 */
    huart->RxState = HAL_UART_STATE_BUSY_RX;            /* 赋值或初始化 */
    huart->ReceptionType = HAL_UART_RECEPTION_STANDARD;  /* 赋值或初始化 */

    /* Init tickstart for timeout management */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */

    huart->RxXferSize = Size;                           /* 赋值或初始化 */
    huart->RxXferCount = Size;                          /* 赋值或初始化 */

    /* In case of 9bits/No Parity transfer, pRxData needs to be handled as a uint16_t pointer */
    if ((huart->Init.WordLength == UART_WORDLENGTH_9B) && (huart->Init.Parity == UART_PARITY_NONE))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      pdata8bits  = NULL;                               /* 赋值或初始化 */
      pdata16bits = (uint16_t *) pData;                 /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      pdata8bits  = pData;                              /* 赋值或初始化 */
      pdata16bits = NULL;                               /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Process Unlocked */
    __HAL_UNLOCK(huart);                                /* HAL辅助宏/函数 */

    /* Check the remain data to be received */
    while (huart->RxXferCount > 0U)                     /* while循环 */
    {                                                   /* 代码块开始 */
      if (UART_WaitOnFlagUntilTimeout(huart, UART_FLAG_RXNE, RESET, tickstart, Timeout) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
      if (pdata8bits == NULL)                           /* 条件判断 */
      {                                                 /* 代码块开始 */
        *pdata16bits = (uint16_t)(huart->Instance->DR & 0x01FF);  /* 赋值或初始化 */
        pdata16bits++;                                  /* 执行语句 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        if ((huart->Init.WordLength == UART_WORDLENGTH_9B) || ((huart->Init.WordLength == UART_WORDLENGTH_8B) && (huart->Init.Parity == UART_PARITY_NONE)))  /* 条件判断 */
        {                                               /* 代码块开始 */
          *pdata8bits = (uint8_t)(huart->Instance->DR & (uint8_t)0x00FF);  /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          *pdata8bits = (uint8_t)(huart->Instance->DR & (uint8_t)0x007F);  /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        pdata8bits++;                                   /* 执行语句 */
      }                                                 /* 代码块结束 */
      huart->RxXferCount--;                             /* 执行语句 */
    }                                                   /* 代码块结束 */

    /* At end of Rx process, restore huart->RxState to Ready */
    huart->RxState = HAL_UART_STATE_READY;              /* 赋值或初始化 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Sends an amount of data in non blocking mode.
  * @note   When UART parity is not enabled (PCE = 0), and Word Length is configured to 9 bits (M1-M0 = 01),
  *         the sent data is handled as a set of u16. In this case, Size must indicate the number
  *         of u16 provided through pData.
  * @param  huart Pointer to a UART_HandleTypeDef structure that contains
  *               the configuration information for the specified UART module.
  * @param  pData Pointer to data buffer (u8 or u16 data elements).
  * @param  Size  Amount of data elements (u8 or u16) to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_UART_Transmit_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check that a Tx process is not already ongoing */
  if (huart->gState == HAL_UART_STATE_READY)            /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) || (Size == 0U))                /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(huart);                                  /* HAL辅助宏/函数 */

    huart->pTxBuffPtr = pData;                          /* 赋值或初始化 */
    huart->TxXferSize = Size;                           /* 赋值或初始化 */
    huart->TxXferCount = Size;                          /* 赋值或初始化 */

    huart->ErrorCode = HAL_UART_ERROR_NONE;             /* 赋值或初始化 */
    huart->gState = HAL_UART_STATE_BUSY_TX;             /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(huart);                                /* HAL辅助宏/函数 */

    /* Enable the UART Transmit data register empty Interrupt */
    __HAL_UART_ENABLE_IT(huart, UART_IT_TXE);           /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Receives an amount of data in non blocking mode.
  * @note   When UART parity is not enabled (PCE = 0), and Word Length is configured to 9 bits (M1-M0 = 01),
  *         the received data is handled as a set of u16. In this case, Size must indicate the number
  *         of u16 available through pData.
  * @param  huart Pointer to a UART_HandleTypeDef structure that contains
  *               the configuration information for the specified UART module.
  * @param  pData Pointer to data buffer (u8 or u16 data elements).
  * @param  Size  Amount of data elements (u8 or u16) to be received.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check that a Rx process is not already ongoing */
  if (huart->RxState == HAL_UART_STATE_READY)           /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) || (Size == 0U))                /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(huart);                                  /* HAL辅助宏/函数 */

    /* Set Reception type to Standard reception */
    huart->ReceptionType = HAL_UART_RECEPTION_STANDARD;  /* 赋值或初始化 */

    return(UART_Start_Receive_IT(huart, pData, Size));  /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Sends an amount of data in DMA mode.
  * @note   When UART parity is not enabled (PCE = 0), and Word Length is configured to 9 bits (M1-M0 = 01),
  *         the sent data is handled as a set of u16. In this case, Size must indicate the number
  *         of u16 provided through pData.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @param  pData Pointer to data buffer (u8 or u16 data elements).
  * @param  Size  Amount of data elements (u8 or u16) to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_UART_Transmit_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t *tmp;                                        /* 32位无符号变量/函数 */

  /* Check that a Tx process is not already ongoing */
  if (huart->gState == HAL_UART_STATE_READY)            /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) || (Size == 0U))                /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(huart);                                  /* HAL辅助宏/函数 */

    huart->pTxBuffPtr = pData;                          /* 赋值或初始化 */
    huart->TxXferSize = Size;                           /* 赋值或初始化 */
    huart->TxXferCount = Size;                          /* 赋值或初始化 */

    huart->ErrorCode = HAL_UART_ERROR_NONE;             /* 赋值或初始化 */
    huart->gState = HAL_UART_STATE_BUSY_TX;             /* 赋值或初始化 */

    /* Set the UART DMA transfer complete callback */
    huart->hdmatx->XferCpltCallback = UART_DMATransmitCplt;  /* 赋值或初始化 */

    /* Set the UART DMA Half transfer complete callback */
    huart->hdmatx->XferHalfCpltCallback = UART_DMATxHalfCplt;  /* 赋值或初始化 */

    /* Set the DMA error callback */
    huart->hdmatx->XferErrorCallback = UART_DMAError;   /* 赋值或初始化 */

    /* Set the DMA abort callback */
    huart->hdmatx->XferAbortCallback = NULL;            /* 赋值或初始化 */

    /* Enable the UART transmit DMA stream */
    tmp = (uint32_t *)&pData;                           /* 赋值或初始化 */
    HAL_DMA_Start_IT(huart->hdmatx, *(uint32_t *)tmp, (uint32_t)&huart->Instance->DR, Size);  /* HAL DMA操作 */

    /* Clear the TC flag in the SR register by writing 0 to it */
    __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_TC);         /* HAL辅助宏/函数 */

    /* Process Unlocked */
    __HAL_UNLOCK(huart);                                /* HAL辅助宏/函数 */

    /* Enable the DMA transfer for transmit request by setting the DMAT bit
       in the UART CR3 register */
    SET_BIT(huart->Instance->CR3, USART_CR3_DMAT);      /* 执行语句 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Receives an amount of data in DMA mode.
  * @note   When UART parity is not enabled (PCE = 0), and Word Length is configured to 9 bits (M1-M0 = 01),
  *         the received data is handled as a set of u16. In this case, Size must indicate the number
  *         of u16 available through pData.
  * @param  huart Pointer to a UART_HandleTypeDef structure that contains
  *               the configuration information for the specified UART module.
  * @param  pData Pointer to data buffer (u8 or u16 data elements).
  * @param  Size  Amount of data elements (u8 or u16) to be received.
  * @note   When the UART parity is enabled (PCE = 1) the received data contains the parity bit.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_UART_Receive_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check that a Rx process is not already ongoing */
  if (huart->RxState == HAL_UART_STATE_READY)           /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) || (Size == 0U))                /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(huart);                                  /* HAL辅助宏/函数 */

    /* Set Reception type to Standard reception */
    huart->ReceptionType = HAL_UART_RECEPTION_STANDARD;  /* 赋值或初始化 */

    return(UART_Start_Receive_DMA(huart, pData, Size));  /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief Pauses the DMA Transfer.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_UART_DMAPause(UART_HandleTypeDef *huart)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t dmarequest = 0x00U;                          /* 32位无符号变量/函数 */

  /* Process Locked */
  __HAL_LOCK(huart);                                    /* HAL辅助宏/函数 */

  dmarequest = HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAT);  /* 赋值或初始化 */
  if ((huart->gState == HAL_UART_STATE_BUSY_TX) && dmarequest)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable the UART DMA Tx request */
    CLEAR_BIT(huart->Instance->CR3, USART_CR3_DMAT);    /* 执行语句 */
  }                                                     /* 代码块结束 */

  dmarequest = HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAR);  /* 赋值或初始化 */
  if ((huart->RxState == HAL_UART_STATE_BUSY_RX) && dmarequest)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts */
    CLEAR_BIT(huart->Instance->CR1, USART_CR1_PEIE);    /* 执行语句 */
    CLEAR_BIT(huart->Instance->CR3, USART_CR3_EIE);     /* 执行语句 */

    /* Disable the UART DMA Rx request */
    CLEAR_BIT(huart->Instance->CR3, USART_CR3_DMAR);    /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Process Unlocked */
  __HAL_UNLOCK(huart);                                  /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief Resumes the DMA Transfer.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_UART_DMAResume(UART_HandleTypeDef *huart)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Process Locked */
  __HAL_LOCK(huart);                                    /* HAL辅助宏/函数 */

  if (huart->gState == HAL_UART_STATE_BUSY_TX)          /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable the UART DMA Tx request */
    SET_BIT(huart->Instance->CR3, USART_CR3_DMAT);      /* 执行语句 */
  }                                                     /* 代码块结束 */

  if (huart->RxState == HAL_UART_STATE_BUSY_RX)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Clear the Overrun flag before resuming the Rx transfer*/
    __HAL_UART_CLEAR_OREFLAG(huart);                    /* HAL辅助宏/函数 */

    /* Re-enable PE and ERR (Frame error, noise error, overrun error) interrupts */
    SET_BIT(huart->Instance->CR1, USART_CR1_PEIE);      /* 执行语句 */
    SET_BIT(huart->Instance->CR3, USART_CR3_EIE);       /* 执行语句 */

    /* Enable the UART DMA Rx request */
    SET_BIT(huart->Instance->CR3, USART_CR3_DMAR);      /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Process Unlocked */
  __HAL_UNLOCK(huart);                                  /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief Stops the DMA Transfer.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_UART_DMAStop(UART_HandleTypeDef *huart)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t dmarequest = 0x00U;                          /* 32位无符号变量/函数 */
  /* The Lock is not implemented on this API to allow the user application
     to call the HAL UART API under callbacks HAL_UART_TxCpltCallback() / HAL_UART_RxCpltCallback():
     when calling HAL_DMA_Abort() API the DMA TX/RX Transfer complete interrupt is generated
     and the correspond call back is executed HAL_UART_TxCpltCallback() / HAL_UART_RxCpltCallback()
     */

  /* Stop UART DMA Tx request if ongoing */
  dmarequest = HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAT);  /* 赋值或初始化 */
  if ((huart->gState == HAL_UART_STATE_BUSY_TX) && dmarequest)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    CLEAR_BIT(huart->Instance->CR3, USART_CR3_DMAT);    /* 执行语句 */

    /* Abort the UART DMA Tx stream */
    if (huart->hdmatx != NULL)                          /* 条件判断 */
    {                                                   /* 代码块开始 */
      HAL_DMA_Abort(huart->hdmatx);                     /* HAL DMA操作 */
    }                                                   /* 代码块结束 */
    UART_EndTxTransfer(huart);                          /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Stop UART DMA Rx request if ongoing */
  dmarequest = HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAR);  /* 赋值或初始化 */
  if ((huart->RxState == HAL_UART_STATE_BUSY_RX) && dmarequest)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    CLEAR_BIT(huart->Instance->CR3, USART_CR3_DMAR);    /* 执行语句 */

    /* Abort the UART DMA Rx stream */
    if (huart->hdmarx != NULL)                          /* 条件判断 */
    {                                                   /* 代码块开始 */
      HAL_DMA_Abort(huart->hdmarx);                     /* HAL DMA操作 */
    }                                                   /* 代码块结束 */
    UART_EndRxTransfer(huart);                          /* 执行语句 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief Receive an amount of data in blocking mode till either the expected number of data is received or an IDLE event occurs.
  * @note   HAL_OK is returned if reception is completed (expected number of data has been received)
  *         or if reception is stopped after IDLE event (less than the expected number of data has been received)
  *         In this case, RxLen output parameter indicates number of data available in reception buffer.
  * @note   When UART parity is not enabled (PCE = 0), and Word Length is configured to 9 bits (M = 01),
  *         the received data is handled as a set of uint16_t. In this case, Size must indicate the number
  *         of uint16_t available through pData.
  * @param huart   UART handle.
  * @param pData   Pointer to data buffer (uint8_t or uint16_t data elements).
  * @param Size    Amount of data elements (uint8_t or uint16_t) to be received.
  * @param RxLen   Number of data elements finally received (could be lower than Size, in case reception ends on IDLE event)
  * @param Timeout Timeout duration expressed in ms (covers the whole reception sequence).
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_UARTEx_ReceiveToIdle(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint16_t *RxLen, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint8_t  *pdata8bits;                                 /* 8位无符号变量/函数 */
  uint16_t *pdata16bits;                                /* 16位无符号变量/函数 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */

  /* Check that a Rx process is not already ongoing */
  if (huart->RxState == HAL_UART_STATE_READY)           /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) || (Size == 0U))                /* 条件判断 */
    {                                                   /* 代码块开始 */
      return  HAL_ERROR;                                /* 返回值 */
    }                                                   /* 代码块结束 */

    __HAL_LOCK(huart);                                  /* HAL辅助宏/函数 */

    huart->ErrorCode = HAL_UART_ERROR_NONE;             /* 赋值或初始化 */
    huart->RxState = HAL_UART_STATE_BUSY_RX;            /* 赋值或初始化 */
    huart->ReceptionType = HAL_UART_RECEPTION_TOIDLE;   /* 赋值或初始化 */

    /* Init tickstart for timeout management */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */

    huart->RxXferSize  = Size;                          /* 赋值或初始化 */
    huart->RxXferCount = Size;                          /* 赋值或初始化 */

    /* In case of 9bits/No Parity transfer, pRxData needs to be handled as a uint16_t pointer */
    if ((huart->Init.WordLength == UART_WORDLENGTH_9B) && (huart->Init.Parity == UART_PARITY_NONE))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      pdata8bits  = NULL;                               /* 赋值或初始化 */
      pdata16bits = (uint16_t *) pData;                 /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      pdata8bits  = pData;                              /* 赋值或初始化 */
      pdata16bits = NULL;                               /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    __HAL_UNLOCK(huart);                                /* HAL辅助宏/函数 */

    /* Initialize output number of received elements */
    *RxLen = 0U;                                        /* 赋值或初始化 */

    /* as long as data have to be received */
    while (huart->RxXferCount > 0U)                     /* while循环 */
    {                                                   /* 代码块开始 */
      /* Check if IDLE flag is set */
      if (__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE))   /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Clear IDLE flag in ISR */
        __HAL_UART_CLEAR_IDLEFLAG(huart);               /* HAL辅助宏/函数 */

        /* If Set, but no data ever received, clear flag without exiting loop */
        /* If Set, and data has already been received, this means Idle Event is valid : End reception */
        if (*RxLen > 0U)                                /* 条件判断 */
        {                                               /* 代码块开始 */
          huart->RxState = HAL_UART_STATE_READY;        /* 赋值或初始化 */

          return HAL_OK;                                /* 返回值 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */

      /* Check if RXNE flag is set */
      if (__HAL_UART_GET_FLAG(huart, UART_FLAG_RXNE))   /* 条件判断 */
      {                                                 /* 代码块开始 */
        if (pdata8bits == NULL)                         /* 条件判断 */
        {                                               /* 代码块开始 */
          *pdata16bits = (uint16_t)(huart->Instance->DR & (uint16_t)0x01FF);  /* 赋值或初始化 */
          pdata16bits++;                                /* 执行语句 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
           if ((huart->Init.WordLength == UART_WORDLENGTH_9B) || ((huart->Init.WordLength == UART_WORDLENGTH_8B) && (huart->Init.Parity == UART_PARITY_NONE)))  /* 条件判断 */
           {                                            /* 代码块开始 */
             *pdata8bits = (uint8_t)(huart->Instance->DR & (uint8_t)0x00FF);  /* 赋值或初始化 */
           }                                            /* 代码块结束 */
           else                                         /* 否则 */
           {                                            /* 代码块开始 */
             *pdata8bits = (uint8_t)(huart->Instance->DR & (uint8_t)0x007F);  /* 赋值或初始化 */
           }                                            /* 代码块结束 */

          pdata8bits++;                                 /* 执行语句 */
        }                                               /* 代码块结束 */
        /* Increment number of received elements */
        *RxLen += 1U;                                   /* 赋值或初始化 */
        huart->RxXferCount--;                           /* 执行语句 */
      }                                                 /* 代码块结束 */

      /* Check for the Timeout */
      if (Timeout != HAL_MAX_DELAY)                     /* 条件判断 */
      {                                                 /* 代码块开始 */
        if (((HAL_GetTick() - tickstart) > Timeout) || (Timeout == 0U))  /* 条件判断 */
        {                                               /* 代码块开始 */
          huart->RxState = HAL_UART_STATE_READY;        /* 赋值或初始化 */

          return HAL_TIMEOUT;                           /* 返回值 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Set number of received elements in output parameter : RxLen */
    *RxLen = huart->RxXferSize - huart->RxXferCount;    /* 赋值或初始化 */
    /* At end of Rx process, restore huart->RxState to Ready */
    huart->RxState = HAL_UART_STATE_READY;              /* 赋值或初始化 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief Receive an amount of data in interrupt mode till either the expected number of data is received or an IDLE event occurs.
  * @note   Reception is initiated by this function call. Further progress of reception is achieved thanks
  *         to UART interrupts raised by RXNE and IDLE events. Callback is called at end of reception indicating
  *         number of received data elements.
  * @note   When UART parity is not enabled (PCE = 0), and Word Length is configured to 9 bits (M = 01),
  *         the received data is handled as a set of uint16_t. In this case, Size must indicate the number
  *         of uint16_t available through pData.
  * @param huart UART handle.
  * @param pData Pointer to data buffer (uint8_t or uint16_t data elements).
  * @param Size  Amount of data elements (uint8_t or uint16_t) to be received.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_UARTEx_ReceiveToIdle_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status;                             /* HAL状态类型函数 */

  /* Check that a Rx process is not already ongoing */
  if (huart->RxState == HAL_UART_STATE_READY)           /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) || (Size == 0U))                /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    __HAL_LOCK(huart);                                  /* HAL辅助宏/函数 */

    /* Set Reception type to reception till IDLE Event*/
    huart->ReceptionType = HAL_UART_RECEPTION_TOIDLE;   /* 赋值或初始化 */

    status =  UART_Start_Receive_IT(huart, pData, Size);  /* 赋值或初始化 */

    /* Check Rx process has been successfully started */
    if (status == HAL_OK)                               /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (huart->ReceptionType == HAL_UART_RECEPTION_TOIDLE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        __HAL_UART_CLEAR_IDLEFLAG(huart);               /* HAL辅助宏/函数 */
        SET_BIT(huart->Instance->CR1, USART_CR1_IDLEIE);  /* 执行语句 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* In case of errors already pending when reception is started,
           Interrupts may have already been raised and lead to reception abortion.
           (Overrun error for instance).
           In such case Reception Type has been reset to HAL_UART_RECEPTION_STANDARD. */
        status = HAL_ERROR;                             /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    return status;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief Receive an amount of data in DMA mode till either the expected number of data is received or an IDLE event occurs.
  * @note   Reception is initiated by this function call. Further progress of reception is achieved thanks
  *         to DMA services, transferring automatically received data elements in user reception buffer and
  *         calling registered callbacks at half/end of reception. UART IDLE events are also used to consider
  *         reception phase as ended. In all cases, callback execution will indicate number of received data elements.
  * @note   When the UART parity is enabled (PCE = 1), the received data contain
  *         the parity bit (MSB position).
  * @note   When UART parity is not enabled (PCE = 0), and Word Length is configured to 9 bits (M = 01),
  *         the received data is handled as a set of uint16_t. In this case, Size must indicate the number
  *         of uint16_t available through pData.
  * @param huart UART handle.
  * @param pData Pointer to data buffer (uint8_t or uint16_t data elements).
  * @param Size  Amount of data elements (uint8_t or uint16_t) to be received.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_UARTEx_ReceiveToIdle_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status;                             /* HAL状态类型函数 */

  /* Check that a Rx process is not already ongoing */
  if (huart->RxState == HAL_UART_STATE_READY)           /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) || (Size == 0U))                /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    __HAL_LOCK(huart);                                  /* HAL辅助宏/函数 */

    /* Set Reception type to reception till IDLE Event*/
    huart->ReceptionType = HAL_UART_RECEPTION_TOIDLE;   /* 赋值或初始化 */

    status =  UART_Start_Receive_DMA(huart, pData, Size);  /* 赋值或初始化 */

    /* Check Rx process has been successfully started */
    if (status == HAL_OK)                               /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (huart->ReceptionType == HAL_UART_RECEPTION_TOIDLE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        __HAL_UART_CLEAR_IDLEFLAG(huart);               /* HAL辅助宏/函数 */
        SET_BIT(huart->Instance->CR1, USART_CR1_IDLEIE);  /* 执行语句 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* In case of errors already pending when reception is started,
           Interrupts may have already been raised and lead to reception abortion.
           (Overrun error for instance).
           In such case Reception Type has been reset to HAL_UART_RECEPTION_STANDARD. */
        status = HAL_ERROR;                             /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    return status;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Abort ongoing transfers (blocking mode).
  * @param  huart UART handle.
  * @note   This procedure could be used for aborting any ongoing transfer started in Interrupt or DMA mode.
  *         This procedure performs following operations :
  *           - Disable UART Interrupts (Tx and Rx)
  *           - Disable the DMA transfer in the peripheral register (if enabled)
  *           - Abort DMA transfer by calling HAL_DMA_Abort (in case of transfer in DMA mode)
  *           - Set handle State to READY
  * @note   This procedure is executed in blocking mode : when exiting function, Abort is considered as completed.
  * @retval HAL status
*/
HAL_StatusTypeDef HAL_UART_Abort(UART_HandleTypeDef *huart)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Disable TXEIE, TCIE, RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts */
  CLEAR_BIT(huart->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE | USART_CR1_TXEIE | USART_CR1_TCIE));  /* 执行语句 */
  CLEAR_BIT(huart->Instance->CR3, USART_CR3_EIE);       /* 执行语句 */

  /* If Reception till IDLE event was ongoing, disable IDLEIE interrupt */
  if (huart->ReceptionType == HAL_UART_RECEPTION_TOIDLE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    CLEAR_BIT(huart->Instance->CR1, (USART_CR1_IDLEIE));  /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Disable the UART DMA Tx request if enabled */
  if (HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAT))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    CLEAR_BIT(huart->Instance->CR3, USART_CR3_DMAT);    /* 执行语句 */

    /* Abort the UART DMA Tx stream: use blocking DMA Abort API (no callback) */
    if (huart->hdmatx != NULL)                          /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the UART DMA Abort callback to Null.
         No call back execution at end of DMA abort procedure */
      huart->hdmatx->XferAbortCallback = NULL;          /* 赋值或初始化 */

      if (HAL_DMA_Abort(huart->hdmatx) != HAL_OK)       /* 条件判断 */
      {                                                 /* 代码块开始 */
        if (HAL_DMA_GetError(huart->hdmatx) == HAL_DMA_ERROR_TIMEOUT)  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Set error code to DMA */
          huart->ErrorCode = HAL_UART_ERROR_DMA;        /* 赋值或初始化 */

          return HAL_TIMEOUT;                           /* 返回值 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Disable the UART DMA Rx request if enabled */
  if (HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAR))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    CLEAR_BIT(huart->Instance->CR3, USART_CR3_DMAR);    /* 执行语句 */

    /* Abort the UART DMA Rx stream: use blocking DMA Abort API (no callback) */
    if (huart->hdmarx != NULL)                          /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the UART DMA Abort callback to Null.
         No call back execution at end of DMA abort procedure */
      huart->hdmarx->XferAbortCallback = NULL;          /* 赋值或初始化 */

      if (HAL_DMA_Abort(huart->hdmarx) != HAL_OK)       /* 条件判断 */
      {                                                 /* 代码块开始 */
        if (HAL_DMA_GetError(huart->hdmarx) == HAL_DMA_ERROR_TIMEOUT)  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Set error code to DMA */
          huart->ErrorCode = HAL_UART_ERROR_DMA;        /* 赋值或初始化 */

          return HAL_TIMEOUT;                           /* 返回值 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Reset Tx and Rx transfer counters */
  huart->TxXferCount = 0x00U;                           /* 赋值或初始化 */
  huart->RxXferCount = 0x00U;                           /* 赋值或初始化 */

  /* Reset ErrorCode */
  huart->ErrorCode = HAL_UART_ERROR_NONE;               /* 赋值或初始化 */

  /* Restore huart->RxState and huart->gState to Ready */
  huart->RxState = HAL_UART_STATE_READY;                /* 赋值或初始化 */
  huart->gState = HAL_UART_STATE_READY;                 /* 赋值或初始化 */
  huart->ReceptionType = HAL_UART_RECEPTION_STANDARD;   /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Abort ongoing Transmit transfer (blocking mode).
  * @param  huart UART handle.
  * @note   This procedure could be used for aborting any ongoing Tx transfer started in Interrupt or DMA mode.
  *         This procedure performs following operations :
  *           - Disable UART Interrupts (Tx)
  *           - Disable the DMA transfer in the peripheral register (if enabled)
  *           - Abort DMA transfer by calling HAL_DMA_Abort (in case of transfer in DMA mode)
  *           - Set handle State to READY
  * @note   This procedure is executed in blocking mode : when exiting function, Abort is considered as completed.
  * @retval HAL status
*/
HAL_StatusTypeDef HAL_UART_AbortTransmit(UART_HandleTypeDef *huart)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Disable TXEIE and TCIE interrupts */
  CLEAR_BIT(huart->Instance->CR1, (USART_CR1_TXEIE | USART_CR1_TCIE));  /* 执行语句 */

  /* Disable the UART DMA Tx request if enabled */
  if (HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAT))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    CLEAR_BIT(huart->Instance->CR3, USART_CR3_DMAT);    /* 执行语句 */

    /* Abort the UART DMA Tx stream : use blocking DMA Abort API (no callback) */
    if (huart->hdmatx != NULL)                          /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the UART DMA Abort callback to Null.
         No call back execution at end of DMA abort procedure */
      huart->hdmatx->XferAbortCallback = NULL;          /* 赋值或初始化 */

      if (HAL_DMA_Abort(huart->hdmatx) != HAL_OK)       /* 条件判断 */
      {                                                 /* 代码块开始 */
        if (HAL_DMA_GetError(huart->hdmatx) == HAL_DMA_ERROR_TIMEOUT)  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Set error code to DMA */
          huart->ErrorCode = HAL_UART_ERROR_DMA;        /* 赋值或初始化 */

          return HAL_TIMEOUT;                           /* 返回值 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Reset Tx transfer counter */
  huart->TxXferCount = 0x00U;                           /* 赋值或初始化 */

  /* Restore huart->gState to Ready */
  huart->gState = HAL_UART_STATE_READY;                 /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Abort ongoing Receive transfer (blocking mode).
  * @param  huart UART handle.
  * @note   This procedure could be used for aborting any ongoing Rx transfer started in Interrupt or DMA mode.
  *         This procedure performs following operations :
  *           - Disable UART Interrupts (Rx)
  *           - Disable the DMA transfer in the peripheral register (if enabled)
  *           - Abort DMA transfer by calling HAL_DMA_Abort (in case of transfer in DMA mode)
  *           - Set handle State to READY
  * @note   This procedure is executed in blocking mode : when exiting function, Abort is considered as completed.
  * @retval HAL status
*/
HAL_StatusTypeDef HAL_UART_AbortReceive(UART_HandleTypeDef *huart)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Disable RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts */
  CLEAR_BIT(huart->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE));  /* 执行语句 */
  CLEAR_BIT(huart->Instance->CR3, USART_CR3_EIE);       /* 执行语句 */

  /* If Reception till IDLE event was ongoing, disable IDLEIE interrupt */
  if (huart->ReceptionType == HAL_UART_RECEPTION_TOIDLE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    CLEAR_BIT(huart->Instance->CR1, (USART_CR1_IDLEIE));  /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Disable the UART DMA Rx request if enabled */
  if (HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAR))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    CLEAR_BIT(huart->Instance->CR3, USART_CR3_DMAR);    /* 执行语句 */

    /* Abort the UART DMA Rx stream : use blocking DMA Abort API (no callback) */
    if (huart->hdmarx != NULL)                          /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the UART DMA Abort callback to Null.
         No call back execution at end of DMA abort procedure */
      huart->hdmarx->XferAbortCallback = NULL;          /* 赋值或初始化 */

      if (HAL_DMA_Abort(huart->hdmarx) != HAL_OK)       /* 条件判断 */
      {                                                 /* 代码块开始 */
        if (HAL_DMA_GetError(huart->hdmarx) == HAL_DMA_ERROR_TIMEOUT)  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Set error code to DMA */
          huart->ErrorCode = HAL_UART_ERROR_DMA;        /* 赋值或初始化 */

          return HAL_TIMEOUT;                           /* 返回值 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Reset Rx transfer counter */
  huart->RxXferCount = 0x00U;                           /* 赋值或初始化 */

  /* Restore huart->RxState to Ready */
  huart->RxState = HAL_UART_STATE_READY;                /* 赋值或初始化 */
  huart->ReceptionType = HAL_UART_RECEPTION_STANDARD;   /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Abort ongoing transfers (Interrupt mode).
  * @param  huart UART handle.
  * @note   This procedure could be used for aborting any ongoing transfer started in Interrupt or DMA mode.
  *         This procedure performs following operations :
  *           - Disable UART Interrupts (Tx and Rx)
  *           - Disable the DMA transfer in the peripheral register (if enabled)
  *           - Abort DMA transfer by calling HAL_DMA_Abort_IT (in case of transfer in DMA mode)
  *           - Set handle State to READY
  *           - At abort completion, call user abort complete callback
  * @note   This procedure is executed in Interrupt mode, meaning that abort procedure could be
  *         considered as completed only when user abort complete callback is executed (not when exiting function).
  * @retval HAL status
*/
HAL_StatusTypeDef HAL_UART_Abort_IT(UART_HandleTypeDef *huart)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t AbortCplt = 0x01U;                           /* 32位无符号变量/函数 */

  /* Disable TXEIE, TCIE, RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts */
  CLEAR_BIT(huart->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE | USART_CR1_TXEIE | USART_CR1_TCIE));  /* 执行语句 */
  CLEAR_BIT(huart->Instance->CR3, USART_CR3_EIE);       /* 执行语句 */

  /* If Reception till IDLE event was ongoing, disable IDLEIE interrupt */
  if (huart->ReceptionType == HAL_UART_RECEPTION_TOIDLE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    CLEAR_BIT(huart->Instance->CR1, (USART_CR1_IDLEIE));  /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* If DMA Tx and/or DMA Rx Handles are associated to UART Handle, DMA Abort complete callbacks should be initialised
     before any call to DMA Abort functions */
  /* DMA Tx Handle is valid */
  if (huart->hdmatx != NULL)                            /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Set DMA Abort Complete callback if UART DMA Tx request if enabled.
       Otherwise, set it to NULL */
    if (HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAT))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      huart->hdmatx->XferAbortCallback = UART_DMATxAbortCallback;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      huart->hdmatx->XferAbortCallback = NULL;          /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /* DMA Rx Handle is valid */
  if (huart->hdmarx != NULL)                            /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Set DMA Abort Complete callback if UART DMA Rx request if enabled.
       Otherwise, set it to NULL */
    if (HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAR))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      huart->hdmarx->XferAbortCallback = UART_DMARxAbortCallback;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      huart->hdmarx->XferAbortCallback = NULL;          /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Disable the UART DMA Tx request if enabled */
  if (HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAT))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable DMA Tx at UART level */
    CLEAR_BIT(huart->Instance->CR3, USART_CR3_DMAT);    /* 执行语句 */

    /* Abort the UART DMA Tx stream : use non blocking DMA Abort API (callback) */
    if (huart->hdmatx != NULL)                          /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* UART Tx DMA Abort callback has already been initialised :
         will lead to call HAL_UART_AbortCpltCallback() at end of DMA abort procedure */

      /* Abort DMA TX */
      if (HAL_DMA_Abort_IT(huart->hdmatx) != HAL_OK)    /* 条件判断 */
      {                                                 /* 代码块开始 */
        huart->hdmatx->XferAbortCallback = NULL;        /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        AbortCplt = 0x00U;                              /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Disable the UART DMA Rx request if enabled */
  if (HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAR))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    CLEAR_BIT(huart->Instance->CR3, USART_CR3_DMAR);    /* 执行语句 */

    /* Abort the UART DMA Rx stream : use non blocking DMA Abort API (callback) */
    if (huart->hdmarx != NULL)                          /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* UART Rx DMA Abort callback has already been initialised :
         will lead to call HAL_UART_AbortCpltCallback() at end of DMA abort procedure */

      /* Abort DMA RX */
      if (HAL_DMA_Abort_IT(huart->hdmarx) != HAL_OK)    /* 条件判断 */
      {                                                 /* 代码块开始 */
        huart->hdmarx->XferAbortCallback = NULL;        /* 赋值或初始化 */
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
    huart->TxXferCount = 0x00U;                         /* 赋值或初始化 */
    huart->RxXferCount = 0x00U;                         /* 赋值或初始化 */

    /* Reset ErrorCode */
    huart->ErrorCode = HAL_UART_ERROR_NONE;             /* 赋值或初始化 */

    /* Restore huart->gState and huart->RxState to Ready */
    huart->gState  = HAL_UART_STATE_READY;              /* 赋值或初始化 */
    huart->RxState = HAL_UART_STATE_READY;              /* 赋值或初始化 */
    huart->ReceptionType = HAL_UART_RECEPTION_STANDARD;  /* 赋值或初始化 */

    /* As no DMA to be aborted, call directly user Abort complete callback */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)              /* 条件编译 */
    /* Call registered Abort complete callback */
    huart->AbortCpltCallback(huart);                    /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /* Call legacy weak Abort complete callback */
    HAL_UART_AbortCpltCallback(huart);                  /* HAL 串口操作 */
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Abort ongoing Transmit transfer (Interrupt mode).
  * @param  huart UART handle.
  * @note   This procedure could be used for aborting any ongoing Tx transfer started in Interrupt or DMA mode.
  *         This procedure performs following operations :
  *           - Disable UART Interrupts (Tx)
  *           - Disable the DMA transfer in the peripheral register (if enabled)
  *           - Abort DMA transfer by calling HAL_DMA_Abort_IT (in case of transfer in DMA mode)
  *           - Set handle State to READY
  *           - At abort completion, call user abort complete callback
  * @note   This procedure is executed in Interrupt mode, meaning that abort procedure could be
  *         considered as completed only when user abort complete callback is executed (not when exiting function).
  * @retval HAL status
*/
HAL_StatusTypeDef HAL_UART_AbortTransmit_IT(UART_HandleTypeDef *huart)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Disable TXEIE and TCIE interrupts */
  CLEAR_BIT(huart->Instance->CR1, (USART_CR1_TXEIE | USART_CR1_TCIE));  /* 执行语句 */

  /* Disable the UART DMA Tx request if enabled */
  if (HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAT))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    CLEAR_BIT(huart->Instance->CR3, USART_CR3_DMAT);    /* 执行语句 */

    /* Abort the UART DMA Tx stream : use blocking DMA Abort API (no callback) */
    if (huart->hdmatx != NULL)                          /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the UART DMA Abort callback :
         will lead to call HAL_UART_AbortCpltCallback() at end of DMA abort procedure */
      huart->hdmatx->XferAbortCallback = UART_DMATxOnlyAbortCallback;  /* 赋值或初始化 */

      /* Abort DMA TX */
      if (HAL_DMA_Abort_IT(huart->hdmatx) != HAL_OK)    /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Call Directly huart->hdmatx->XferAbortCallback function in case of error */
        huart->hdmatx->XferAbortCallback(huart->hdmatx);  /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Reset Tx transfer counter */
      huart->TxXferCount = 0x00U;                       /* 赋值或初始化 */

      /* Restore huart->gState to Ready */
      huart->gState = HAL_UART_STATE_READY;             /* 赋值或初始化 */

      /* As no DMA to be aborted, call directly user Abort complete callback */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)              /* 条件编译 */
      /* Call registered Abort Transmit Complete Callback */
      huart->AbortTransmitCpltCallback(huart);          /* 执行语句 */
#else                                                   /* 条件编译否则 */
      /* Call legacy weak Abort Transmit Complete Callback */
      HAL_UART_AbortTransmitCpltCallback(huart);        /* HAL 串口操作 */
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Reset Tx transfer counter */
    huart->TxXferCount = 0x00U;                         /* 赋值或初始化 */

    /* Restore huart->gState to Ready */
    huart->gState = HAL_UART_STATE_READY;               /* 赋值或初始化 */

    /* As no DMA to be aborted, call directly user Abort complete callback */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)              /* 条件编译 */
    /* Call registered Abort Transmit Complete Callback */
    huart->AbortTransmitCpltCallback(huart);            /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /* Call legacy weak Abort Transmit Complete Callback */
    HAL_UART_AbortTransmitCpltCallback(huart);          /* HAL 串口操作 */
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Abort ongoing Receive transfer (Interrupt mode).
  * @param  huart UART handle.
  * @note   This procedure could be used for aborting any ongoing Rx transfer started in Interrupt or DMA mode.
  *         This procedure performs following operations :
  *           - Disable UART Interrupts (Rx)
  *           - Disable the DMA transfer in the peripheral register (if enabled)
  *           - Abort DMA transfer by calling HAL_DMA_Abort_IT (in case of transfer in DMA mode)
  *           - Set handle State to READY
  *           - At abort completion, call user abort complete callback
  * @note   This procedure is executed in Interrupt mode, meaning that abort procedure could be
  *         considered as completed only when user abort complete callback is executed (not when exiting function).
  * @retval HAL status
*/
HAL_StatusTypeDef HAL_UART_AbortReceive_IT(UART_HandleTypeDef *huart)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Disable RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts */
  CLEAR_BIT(huart->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE));  /* 执行语句 */
  CLEAR_BIT(huart->Instance->CR3, USART_CR3_EIE);       /* 执行语句 */

  /* If Reception till IDLE event was ongoing, disable IDLEIE interrupt */
  if (huart->ReceptionType == HAL_UART_RECEPTION_TOIDLE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    CLEAR_BIT(huart->Instance->CR1, (USART_CR1_IDLEIE));  /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Disable the UART DMA Rx request if enabled */
  if (HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAR))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    CLEAR_BIT(huart->Instance->CR3, USART_CR3_DMAR);    /* 执行语句 */

    /* Abort the UART DMA Rx stream : use blocking DMA Abort API (no callback) */
    if (huart->hdmarx != NULL)                          /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the UART DMA Abort callback :
         will lead to call HAL_UART_AbortCpltCallback() at end of DMA abort procedure */
      huart->hdmarx->XferAbortCallback = UART_DMARxOnlyAbortCallback;  /* 赋值或初始化 */

      /* Abort DMA RX */
      if (HAL_DMA_Abort_IT(huart->hdmarx) != HAL_OK)    /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Call Directly huart->hdmarx->XferAbortCallback function in case of error */
        huart->hdmarx->XferAbortCallback(huart->hdmarx);  /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Reset Rx transfer counter */
      huart->RxXferCount = 0x00U;                       /* 赋值或初始化 */

      /* Restore huart->RxState to Ready */
      huart->RxState = HAL_UART_STATE_READY;            /* 赋值或初始化 */
      huart->ReceptionType = HAL_UART_RECEPTION_STANDARD;  /* 赋值或初始化 */

      /* As no DMA to be aborted, call directly user Abort complete callback */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)              /* 条件编译 */
      /* Call registered Abort Receive Complete Callback */
      huart->AbortReceiveCpltCallback(huart);           /* 执行语句 */
#else                                                   /* 条件编译否则 */
      /* Call legacy weak Abort Receive Complete Callback */
      HAL_UART_AbortReceiveCpltCallback(huart);         /* HAL 串口操作 */
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Reset Rx transfer counter */
    huart->RxXferCount = 0x00U;                         /* 赋值或初始化 */

    /* Restore huart->RxState to Ready */
    huart->RxState = HAL_UART_STATE_READY;              /* 赋值或初始化 */
    huart->ReceptionType = HAL_UART_RECEPTION_STANDARD;  /* 赋值或初始化 */

    /* As no DMA to be aborted, call directly user Abort complete callback */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)              /* 条件编译 */
    /* Call registered Abort Receive Complete Callback */
    huart->AbortReceiveCpltCallback(huart);             /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /* Call legacy weak Abort Receive Complete Callback */
    HAL_UART_AbortReceiveCpltCallback(huart);           /* HAL 串口操作 */
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  This function handles UART interrupt request.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
void HAL_UART_IRQHandler(UART_HandleTypeDef *huart)     /* 中断服务函数 */
{                                                       /* 代码块开始 */
  uint32_t isrflags   = READ_REG(huart->Instance->SR);  /* 32位无符号变量/函数 */
  uint32_t cr1its     = READ_REG(huart->Instance->CR1);  /* 32位无符号变量/函数 */
  uint32_t cr3its     = READ_REG(huart->Instance->CR3);  /* 32位无符号变量/函数 */
  uint32_t errorflags = 0x00U;                          /* 32位无符号变量/函数 */
  uint32_t dmarequest = 0x00U;                          /* 32位无符号变量/函数 */

  /* If no error occurs */
  errorflags = (isrflags & (uint32_t)(USART_SR_PE | USART_SR_FE | USART_SR_ORE | USART_SR_NE));  /* 赋值或初始化 */
  if (errorflags == RESET)                              /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* UART in mode Receiver -------------------------------------------------*/
    if (((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      UART_Receive_IT(huart);                           /* 执行语句 */
      return;                                           /* 返回 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* If some errors occur */
  if ((errorflags != RESET) && (((cr3its & USART_CR3_EIE) != RESET) || ((cr1its & (USART_CR1_RXNEIE | USART_CR1_PEIE)) != RESET)))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* UART parity error interrupt occurred ----------------------------------*/
    if (((isrflags & USART_SR_PE) != RESET) && ((cr1its & USART_CR1_PEIE) != RESET))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      huart->ErrorCode |= HAL_UART_ERROR_PE;            /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* UART noise error interrupt occurred -----------------------------------*/
    if (((isrflags & USART_SR_NE) != RESET) && ((cr3its & USART_CR3_EIE) != RESET))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      huart->ErrorCode |= HAL_UART_ERROR_NE;            /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* UART frame error interrupt occurred -----------------------------------*/
    if (((isrflags & USART_SR_FE) != RESET) && ((cr3its & USART_CR3_EIE) != RESET))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      huart->ErrorCode |= HAL_UART_ERROR_FE;            /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* UART Over-Run interrupt occurred --------------------------------------*/
    if (((isrflags & USART_SR_ORE) != RESET) && (((cr1its & USART_CR1_RXNEIE) != RESET) || ((cr3its & USART_CR3_EIE) != RESET)))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      huart->ErrorCode |= HAL_UART_ERROR_ORE;           /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Call UART Error Call back function if need be --------------------------*/
    if (huart->ErrorCode != HAL_UART_ERROR_NONE)        /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* UART in mode Receiver -----------------------------------------------*/
      if (((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        UART_Receive_IT(huart);                         /* 执行语句 */
      }                                                 /* 代码块结束 */

      /* If Overrun error occurs, or if any error occurs in DMA mode reception,
         consider error as blocking */
      dmarequest = HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAR);  /* 赋值或初始化 */
      if (((huart->ErrorCode & HAL_UART_ERROR_ORE) != RESET) || dmarequest)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Blocking error : transfer is aborted
           Set the UART state ready to be able to start again the process,
           Disable Rx Interrupts, and disable Rx DMA request, if ongoing */
        UART_EndRxTransfer(huart);                      /* 执行语句 */

        /* Disable the UART DMA Rx request if enabled */
        if (HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAR))  /* 条件判断 */
        {                                               /* 代码块开始 */
          CLEAR_BIT(huart->Instance->CR3, USART_CR3_DMAR);  /* 执行语句 */

          /* Abort the UART DMA Rx stream */
          if (huart->hdmarx != NULL)                    /* 条件判断 */
          {                                             /* 代码块开始 */
            /* Set the UART DMA Abort callback :
               will lead to call HAL_UART_ErrorCallback() at end of DMA abort procedure */
            huart->hdmarx->XferAbortCallback = UART_DMAAbortOnError;  /* 赋值或初始化 */
            if (HAL_DMA_Abort_IT(huart->hdmarx) != HAL_OK)  /* 条件判断 */
            {                                           /* 代码块开始 */
              /* Call Directly XferAbortCallback function in case of error */
              huart->hdmarx->XferAbortCallback(huart->hdmarx);  /* 执行语句 */
            }                                           /* 代码块结束 */
          }                                             /* 代码块结束 */
          else                                          /* 否则 */
          {                                             /* 代码块开始 */
            /* Call user error callback */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)              /* 条件编译 */
            /*Call registered error callback*/
            huart->ErrorCallback(huart);                /* 执行语句 */
#else                                                   /* 条件编译否则 */
            /*Call legacy weak error callback*/
            HAL_UART_ErrorCallback(huart);              /* HAL 串口操作 */
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          /* Call user error callback */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)              /* 条件编译 */
          /*Call registered error callback*/
          huart->ErrorCallback(huart);                  /* 执行语句 */
#else                                                   /* 条件编译否则 */
          /*Call legacy weak error callback*/
          HAL_UART_ErrorCallback(huart);                /* HAL 串口操作 */
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Non Blocking error : transfer could go on.
           Error is notified to user through user error callback */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)              /* 条件编译 */
        /*Call registered error callback*/
        huart->ErrorCallback(huart);                    /* 执行语句 */
#else                                                   /* 条件编译否则 */
        /*Call legacy weak error callback*/
        HAL_UART_ErrorCallback(huart);                  /* HAL 串口操作 */
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */

        huart->ErrorCode = HAL_UART_ERROR_NONE;         /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    return;                                             /* 返回 */
  } /* End if some error occurs */

  /* Check current reception Mode :
     If Reception till IDLE event has been selected : */
  if (  (huart->ReceptionType == HAL_UART_RECEPTION_TOIDLE)  /* 条件判断 */
      &&((isrflags & USART_SR_IDLE) != 0U)              /* 源代码 */
      &&((cr1its & USART_SR_IDLE) != 0U))               /* 源代码 */
  {                                                     /* 代码块开始 */
    __HAL_UART_CLEAR_IDLEFLAG(huart);                   /* HAL辅助宏/函数 */

    /* Check if DMA mode is enabled in UART */
    if (HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAR))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* DMA mode enabled */
      /* Check received length : If all expected data are received, do nothing,
         (DMA cplt callback will be called).
         Otherwise, if at least one data has already been received, IDLE event is to be notified to user */
      uint16_t nb_remaining_rx_data = (uint16_t) __HAL_DMA_GET_COUNTER(huart->hdmarx);  /* 16位无符号变量/函数 */
      if (  (nb_remaining_rx_data > 0U)                 /* 条件判断 */
          &&(nb_remaining_rx_data < huart->RxXferSize))  /* 源代码 */
      {                                                 /* 代码块开始 */
        /* Reception is not complete */
        huart->RxXferCount = nb_remaining_rx_data;      /* 赋值或初始化 */

        /* In Normal mode, end DMA xfer and HAL UART Rx process*/
        if (huart->hdmarx->Init.Mode != DMA_CIRCULAR)   /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Disable PE and ERR (Frame error, noise error, overrun error) interrupts */
          CLEAR_BIT(huart->Instance->CR1, USART_CR1_PEIE);  /* 执行语句 */
          CLEAR_BIT(huart->Instance->CR3, USART_CR3_EIE);  /* 执行语句 */

          /* Disable the DMA transfer for the receiver request by resetting the DMAR bit
             in the UART CR3 register */
          CLEAR_BIT(huart->Instance->CR3, USART_CR3_DMAR);  /* 执行语句 */

          /* At end of Rx process, restore huart->RxState to Ready */
          huart->RxState = HAL_UART_STATE_READY;        /* 赋值或初始化 */
          huart->ReceptionType = HAL_UART_RECEPTION_STANDARD;  /* 赋值或初始化 */

          CLEAR_BIT(huart->Instance->CR1, USART_CR1_IDLEIE);  /* 执行语句 */

          /* Last bytes received, so no need as the abort is immediate */
          (void)HAL_DMA_Abort(huart->hdmarx);           /* 执行语句 */
        }                                               /* 代码块结束 */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)              /* 条件编译 */
        /*Call registered Rx Event callback*/
        huart->RxEventCallback(huart, (huart->RxXferSize - huart->RxXferCount));  /* 执行语句 */
#else                                                   /* 条件编译否则 */
        /*Call legacy weak Rx Event callback*/
        HAL_UARTEx_RxEventCallback(huart, (huart->RxXferSize - huart->RxXferCount));  /* HAL库函数调用 */
#endif                                                  /* 条件编译结束 */
      }                                                 /* 代码块结束 */
      return;                                           /* 返回 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* DMA mode not enabled */
      /* Check received length : If all expected data are received, do nothing.
         Otherwise, if at least one data has already been received, IDLE event is to be notified to user */
      uint16_t nb_rx_data = huart->RxXferSize - huart->RxXferCount;  /* 16位无符号变量/函数 */
      if (  (huart->RxXferCount > 0U)                   /* 条件判断 */
          &&(nb_rx_data > 0U) )                         /* 源代码 */
      {                                                 /* 代码块开始 */
        /* Disable the UART Parity Error Interrupt and RXNE interrupts */
        CLEAR_BIT(huart->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE));  /* 执行语句 */

        /* Disable the UART Error Interrupt: (Frame error, noise error, overrun error) */
        CLEAR_BIT(huart->Instance->CR3, USART_CR3_EIE);  /* 执行语句 */

        /* Rx process is completed, restore huart->RxState to Ready */
        huart->RxState = HAL_UART_STATE_READY;          /* 赋值或初始化 */
        huart->ReceptionType = HAL_UART_RECEPTION_STANDARD;  /* 赋值或初始化 */

        CLEAR_BIT(huart->Instance->CR1, USART_CR1_IDLEIE);  /* 执行语句 */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)              /* 条件编译 */
        /*Call registered Rx complete callback*/
        huart->RxEventCallback(huart, nb_rx_data);      /* 执行语句 */
#else                                                   /* 条件编译否则 */
        /*Call legacy weak Rx Event callback*/
        HAL_UARTEx_RxEventCallback(huart, nb_rx_data);  /* HAL库函数调用 */
#endif                                                  /* 条件编译结束 */
      }                                                 /* 代码块结束 */
      return;                                           /* 返回 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* UART in mode Transmitter ------------------------------------------------*/
  if (((isrflags & USART_SR_TXE) != RESET) && ((cr1its & USART_CR1_TXEIE) != RESET))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    UART_Transmit_IT(huart);                            /* 执行语句 */
    return;                                             /* 返回 */
  }                                                     /* 代码块结束 */

  /* UART in mode Transmitter end --------------------------------------------*/
  if (((isrflags & USART_SR_TC) != RESET) && ((cr1its & USART_CR1_TCIE) != RESET))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    UART_EndTransmit_IT(huart);                         /* 执行语句 */
    return;                                             /* 返回 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Tx Transfer completed callbacks.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
__weak void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);                                        /* 执行语句 */
  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_UART_TxCpltCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Tx Half Transfer completed callbacks.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
__weak void HAL_UART_TxHalfCpltCallback(UART_HandleTypeDef *huart)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);                                        /* 执行语句 */
  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_UART_TxHalfCpltCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Rx Transfer completed callbacks.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
__weak void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);                                        /* 执行语句 */
  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_UART_RxCpltCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Rx Half Transfer completed callbacks.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
__weak void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);                                        /* 执行语句 */
  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_UART_RxHalfCpltCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  UART error callbacks.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
__weak void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);                                        /* 执行语句 */
  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_UART_ErrorCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  UART Abort Complete callback.
  * @param  huart UART handle.
  * @retval None
  */
__weak void HAL_UART_AbortCpltCallback(UART_HandleTypeDef *huart)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);                                        /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_UART_AbortCpltCallback can be implemented in the user file.
   */
}                                                       /* 代码块结束 */

/**
  * @brief  UART Abort Complete callback.
  * @param  huart UART handle.
  * @retval None
  */
__weak void HAL_UART_AbortTransmitCpltCallback(UART_HandleTypeDef *huart)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);                                        /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_UART_AbortTransmitCpltCallback can be implemented in the user file.
   */
}                                                       /* 代码块结束 */

/**
  * @brief  UART Abort Receive Complete callback.
  * @param  huart UART handle.
  * @retval None
  */
__weak void HAL_UART_AbortReceiveCpltCallback(UART_HandleTypeDef *huart)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);                                        /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_UART_AbortReceiveCpltCallback can be implemented in the user file.
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Reception Event Callback (Rx event notification called after use of advanced reception service).
  * @param  huart UART handle
  * @param  Size  Number of data available in application reception buffer (indicates a position in
  *               reception buffer until which, data are available)
  * @retval None
  */
__weak void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);                                        /* 执行语句 */
  UNUSED(Size);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_UARTEx_RxEventCallback can be implemented in the user file.
   */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup UART_Exported_Functions_Group3 Peripheral Control functions
  *  @brief   UART control functions
  *
@verbatim
  ==============================================================================
                      ##### Peripheral Control functions #####
  ==============================================================================
  [..]
    This subsection provides a set of functions allowing to control the UART:
    (+) HAL_LIN_SendBreak() API can be helpful to transmit the break character.
    (+) HAL_MultiProcessor_EnterMuteMode() API can be helpful to enter the UART in mute mode.
    (+) HAL_MultiProcessor_ExitMuteMode() API can be helpful to exit the UART mute mode by software.
    (+) HAL_HalfDuplex_EnableTransmitter() API to enable the UART transmitter and disables the UART receiver in Half Duplex mode
    (+) HAL_HalfDuplex_EnableReceiver() API to enable the UART receiver and disables the UART transmitter in Half Duplex mode

@endverbatim
  * @{
  */

/**
  * @brief  Transmits break characters.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_LIN_SendBreak(UART_HandleTypeDef *huart)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_UART_INSTANCE(huart->Instance));      /* 执行语句 */

  /* Process Locked */
  __HAL_LOCK(huart);                                    /* HAL辅助宏/函数 */

  huart->gState = HAL_UART_STATE_BUSY;                  /* 赋值或初始化 */

  /* Send break characters */
  SET_BIT(huart->Instance->CR1, USART_CR1_SBK);         /* 执行语句 */

  huart->gState = HAL_UART_STATE_READY;                 /* 赋值或初始化 */

  /* Process Unlocked */
  __HAL_UNLOCK(huart);                                  /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Enters the UART in mute mode.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MultiProcessor_EnterMuteMode(UART_HandleTypeDef *huart)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_UART_INSTANCE(huart->Instance));      /* 执行语句 */

  /* Process Locked */
  __HAL_LOCK(huart);                                    /* HAL辅助宏/函数 */

  huart->gState = HAL_UART_STATE_BUSY;                  /* 赋值或初始化 */

  /* Enable the USART mute mode  by setting the RWU bit in the CR1 register */
  SET_BIT(huart->Instance->CR1, USART_CR1_RWU);         /* 执行语句 */

  huart->gState = HAL_UART_STATE_READY;                 /* 赋值或初始化 */

  /* Process Unlocked */
  __HAL_UNLOCK(huart);                                  /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Exits the UART mute mode: wake up software.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MultiProcessor_ExitMuteMode(UART_HandleTypeDef *huart)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_UART_INSTANCE(huart->Instance));      /* 执行语句 */

  /* Process Locked */
  __HAL_LOCK(huart);                                    /* HAL辅助宏/函数 */

  huart->gState = HAL_UART_STATE_BUSY;                  /* 赋值或初始化 */

  /* Disable the USART mute mode by clearing the RWU bit in the CR1 register */
  CLEAR_BIT(huart->Instance->CR1, USART_CR1_RWU);       /* 执行语句 */

  huart->gState = HAL_UART_STATE_READY;                 /* 赋值或初始化 */

  /* Process Unlocked */
  __HAL_UNLOCK(huart);                                  /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Enables the UART transmitter and disables the UART receiver.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_HalfDuplex_EnableTransmitter(UART_HandleTypeDef *huart)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpreg = 0x00U;                              /* 32位无符号变量/函数 */

  /* Process Locked */
  __HAL_LOCK(huart);                                    /* HAL辅助宏/函数 */

  huart->gState = HAL_UART_STATE_BUSY;                  /* 赋值或初始化 */

  /*-------------------------- USART CR1 Configuration -----------------------*/
  tmpreg = huart->Instance->CR1;                        /* 赋值或初始化 */

  /* Clear TE and RE bits */
  tmpreg &= (uint32_t)~((uint32_t)(USART_CR1_TE | USART_CR1_RE));  /* 赋值或初始化 */

  /* Enable the USART's transmit interface by setting the TE bit in the USART CR1 register */
  tmpreg |= (uint32_t)USART_CR1_TE;                     /* 赋值或初始化 */

  /* Write to USART CR1 */
  WRITE_REG(huart->Instance->CR1, (uint32_t)tmpreg);    /* 执行语句 */

  huart->gState = HAL_UART_STATE_READY;                 /* 赋值或初始化 */

  /* Process Unlocked */
  __HAL_UNLOCK(huart);                                  /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Enables the UART receiver and disables the UART transmitter.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_HalfDuplex_EnableReceiver(UART_HandleTypeDef *huart)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpreg = 0x00U;                              /* 32位无符号变量/函数 */

  /* Process Locked */
  __HAL_LOCK(huart);                                    /* HAL辅助宏/函数 */

  huart->gState = HAL_UART_STATE_BUSY;                  /* 赋值或初始化 */

  /*-------------------------- USART CR1 Configuration -----------------------*/
  tmpreg = huart->Instance->CR1;                        /* 赋值或初始化 */

  /* Clear TE and RE bits */
  tmpreg &= (uint32_t)~((uint32_t)(USART_CR1_TE | USART_CR1_RE));  /* 赋值或初始化 */

  /* Enable the USART's receive interface by setting the RE bit in the USART CR1 register */
  tmpreg |= (uint32_t)USART_CR1_RE;                     /* 赋值或初始化 */

  /* Write to USART CR1 */
  WRITE_REG(huart->Instance->CR1, (uint32_t)tmpreg);    /* 执行语句 */

  huart->gState = HAL_UART_STATE_READY;                 /* 赋值或初始化 */

  /* Process Unlocked */
  __HAL_UNLOCK(huart);                                  /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup UART_Exported_Functions_Group4 Peripheral State and Errors functions
  *  @brief   UART State and Errors functions
  *
@verbatim
  ==============================================================================
                 ##### Peripheral State and Errors functions #####
  ==============================================================================
 [..]
   This subsection provides a set of functions allowing to return the State of
   UART communication process, return Peripheral Errors occurred during communication
   process
   (+) HAL_UART_GetState() API can be helpful to check in run-time the state of the UART peripheral.
   (+) HAL_UART_GetError() check in run-time errors that could be occurred during communication.

@endverbatim
  * @{
  */

/**
  * @brief  Returns the UART state.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval HAL state
  */
HAL_UART_StateTypeDef HAL_UART_GetState(UART_HandleTypeDef *huart)  /* HAL 串口操作 */
{                                                       /* 代码块开始 */
  uint32_t temp1 = 0x00U, temp2 = 0x00U;                /* 32位无符号变量/函数 */
  temp1 = huart->gState;                                /* 赋值或初始化 */
  temp2 = huart->RxState;                               /* 赋值或初始化 */

  return (HAL_UART_StateTypeDef)(temp1 | temp2);        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return the UART error code
  * @param  huart Pointer to a UART_HandleTypeDef structure that contains
  *               the configuration information for the specified UART.
  * @retval UART Error Code
  */
uint32_t HAL_UART_GetError(UART_HandleTypeDef *huart)   /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  return huart->ErrorCode;                              /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup UART_Private_Functions UART Private Functions
  * @{
  */

/**
  * @brief  Initialize the callbacks to their default values.
  * @param  huart UART handle.
  * @retval none
  */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)              /* 条件编译 */
void UART_InitCallbacksToDefault(UART_HandleTypeDef *huart)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Init the UART Callback settings */
  huart->TxHalfCpltCallback        = HAL_UART_TxHalfCpltCallback;        /* Legacy weak TxHalfCpltCallback        */
  huart->TxCpltCallback            = HAL_UART_TxCpltCallback;            /* Legacy weak TxCpltCallback            */
  huart->RxHalfCpltCallback        = HAL_UART_RxHalfCpltCallback;        /* Legacy weak RxHalfCpltCallback        */
  huart->RxCpltCallback            = HAL_UART_RxCpltCallback;            /* Legacy weak RxCpltCallback            */
  huart->ErrorCallback             = HAL_UART_ErrorCallback;             /* Legacy weak ErrorCallback             */
  huart->AbortCpltCallback         = HAL_UART_AbortCpltCallback;         /* Legacy weak AbortCpltCallback         */
  huart->AbortTransmitCpltCallback = HAL_UART_AbortTransmitCpltCallback; /* Legacy weak AbortTransmitCpltCallback */
  huart->AbortReceiveCpltCallback  = HAL_UART_AbortReceiveCpltCallback;  /* Legacy weak AbortReceiveCpltCallback  */
  huart->RxEventCallback           = HAL_UARTEx_RxEventCallback;         /* Legacy weak RxEventCallback           */

}                                                       /* 代码块结束 */
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */

/**
  * @brief  DMA UART transmit process complete callback.
  * @param  hdma  Pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA module.
  * @retval None
  */
static void UART_DMATransmitCplt(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  UART_HandleTypeDef *huart = (UART_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;  /* 赋值或初始化 */
  /* DMA Normal mode*/
  if ((hdma->Instance->CR & DMA_SxCR_CIRC) == 0U)       /* 条件判断 */
  {                                                     /* 代码块开始 */
    huart->TxXferCount = 0x00U;                         /* 赋值或初始化 */

    /* Disable the DMA transfer for transmit request by setting the DMAT bit
       in the UART CR3 register */
    CLEAR_BIT(huart->Instance->CR3, USART_CR3_DMAT);    /* 执行语句 */

    /* Enable the UART Transmit Complete Interrupt */
    SET_BIT(huart->Instance->CR1, USART_CR1_TCIE);      /* 执行语句 */

  }                                                     /* 代码块结束 */
  /* DMA Circular mode */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)              /* 条件编译 */
    /*Call registered Tx complete callback*/
    huart->TxCpltCallback(huart);                       /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /*Call legacy weak Tx complete callback*/
    HAL_UART_TxCpltCallback(huart);                     /* HAL 串口操作 */
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief DMA UART transmit process half complete callback
  * @param  hdma  Pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA module.
  * @retval None
  */
static void UART_DMATxHalfCplt(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  UART_HandleTypeDef *huart = (UART_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;  /* 赋值或初始化 */

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)              /* 条件编译 */
  /*Call registered Tx complete callback*/
  huart->TxHalfCpltCallback(huart);                     /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /*Call legacy weak Tx complete callback*/
  HAL_UART_TxHalfCpltCallback(huart);                   /* HAL 串口操作 */
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA UART receive process complete callback.
  * @param  hdma  Pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA module.
  * @retval None
  */
static void UART_DMAReceiveCplt(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  UART_HandleTypeDef *huart = (UART_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;  /* 赋值或初始化 */
  /* DMA Normal mode*/
  if ((hdma->Instance->CR & DMA_SxCR_CIRC) == 0U)       /* 条件判断 */
  {                                                     /* 代码块开始 */
    huart->RxXferCount = 0U;                            /* 赋值或初始化 */

    /* Disable RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts */
    CLEAR_BIT(huart->Instance->CR1, USART_CR1_PEIE);    /* 执行语句 */
    CLEAR_BIT(huart->Instance->CR3, USART_CR3_EIE);     /* 执行语句 */

    /* Disable the DMA transfer for the receiver request by setting the DMAR bit
       in the UART CR3 register */
    CLEAR_BIT(huart->Instance->CR3, USART_CR3_DMAR);    /* 执行语句 */

    /* At end of Rx process, restore huart->RxState to Ready */
    huart->RxState = HAL_UART_STATE_READY;              /* 赋值或初始化 */

    /* If Reception till IDLE event has been selected, Disable IDLE Interrupt */
    if (huart->ReceptionType == HAL_UART_RECEPTION_TOIDLE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      CLEAR_BIT(huart->Instance->CR1, USART_CR1_IDLEIE);  /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Check current reception Mode :
     If Reception till IDLE event has been selected : use Rx Event callback */
  if (huart->ReceptionType == HAL_UART_RECEPTION_TOIDLE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)              /* 条件编译 */
    /*Call registered Rx Event callback*/
    huart->RxEventCallback(huart, huart->RxXferSize);   /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /*Call legacy weak Rx Event callback*/
    HAL_UARTEx_RxEventCallback(huart, huart->RxXferSize);  /* HAL库函数调用 */
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* In other cases : use Rx Complete callback */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)              /* 条件编译 */
    /*Call registered Rx complete callback*/
    huart->RxCpltCallback(huart);                       /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /*Call legacy weak Rx complete callback*/
    HAL_UART_RxCpltCallback(huart);                     /* HAL 串口操作 */
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief DMA UART receive process half complete callback
  * @param  hdma  Pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA module.
  * @retval None
  */
static void UART_DMARxHalfCplt(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  UART_HandleTypeDef *huart = (UART_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;  /* 赋值或初始化 */

  /* Check current reception Mode :
     If Reception till IDLE event has been selected : use Rx Event callback */
  if (huart->ReceptionType == HAL_UART_RECEPTION_TOIDLE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)              /* 条件编译 */
    /*Call registered Rx Event callback*/
    huart->RxEventCallback(huart, huart->RxXferSize/2U);  /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /*Call legacy weak Rx Event callback*/
    HAL_UARTEx_RxEventCallback(huart, huart->RxXferSize/2U);  /* HAL库函数调用 */
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* In other cases : use Rx Half Complete callback */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)              /* 条件编译 */
    /*Call registered Rx Half complete callback*/
    huart->RxHalfCpltCallback(huart);                   /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /*Call legacy weak Rx Half complete callback*/
    HAL_UART_RxHalfCpltCallback(huart);                 /* HAL 串口操作 */
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA UART communication error callback.
  * @param  hdma  Pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA module.
  * @retval None
  */
static void UART_DMAError(DMA_HandleTypeDef *hdma)      /* 静态函数 */
{                                                       /* 代码块开始 */
  uint32_t dmarequest = 0x00U;                          /* 32位无符号变量/函数 */
  UART_HandleTypeDef *huart = (UART_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;  /* 赋值或初始化 */

  /* Stop UART DMA Tx request if ongoing */
  dmarequest = HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAT);  /* 赋值或初始化 */
  if ((huart->gState == HAL_UART_STATE_BUSY_TX) && dmarequest)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    huart->TxXferCount = 0x00U;                         /* 赋值或初始化 */
    UART_EndTxTransfer(huart);                          /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Stop UART DMA Rx request if ongoing */
  dmarequest = HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAR);  /* 赋值或初始化 */
  if ((huart->RxState == HAL_UART_STATE_BUSY_RX) && dmarequest)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    huart->RxXferCount = 0x00U;                         /* 赋值或初始化 */
    UART_EndRxTransfer(huart);                          /* 执行语句 */
  }                                                     /* 代码块结束 */

  huart->ErrorCode |= HAL_UART_ERROR_DMA;               /* 赋值或初始化 */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)              /* 条件编译 */
  /*Call registered error callback*/
  huart->ErrorCallback(huart);                          /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /*Call legacy weak error callback*/
  HAL_UART_ErrorCallback(huart);                        /* HAL 串口操作 */
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
}                                                       /* 代码块结束 */

/**
  * @brief  This function handles UART Communication Timeout.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @param  Flag specifies the UART flag to check.
  * @param  Status The new Flag status (SET or RESET).
  * @param  Tickstart Tick start value
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
static HAL_StatusTypeDef UART_WaitOnFlagUntilTimeout(UART_HandleTypeDef *huart, uint32_t Flag, FlagStatus Status, uint32_t Tickstart, uint32_t Timeout)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  /* Wait until flag is set */
  while ((__HAL_UART_GET_FLAG(huart, Flag) ? SET : RESET) == Status)  /* while循环 */
  {                                                     /* 代码块开始 */
    /* Check for the Timeout */
    if (Timeout != HAL_MAX_DELAY)                       /* 条件判断 */
    {                                                   /* 代码块开始 */
      if ((Timeout == 0U) || ((HAL_GetTick() - Tickstart) > Timeout))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Disable TXE, RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts for the interrupt process */
        CLEAR_BIT(huart->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE | USART_CR1_TXEIE));  /* 执行语句 */
        CLEAR_BIT(huart->Instance->CR3, USART_CR3_EIE);  /* 执行语句 */

        huart->gState  = HAL_UART_STATE_READY;          /* 赋值或初始化 */
        huart->RxState = HAL_UART_STATE_READY;          /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(huart);                            /* HAL辅助宏/函数 */

        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Start Receive operation in interrupt mode.
  * @note   This function could be called by all HAL UART API providing reception in Interrupt mode.
  * @note   When calling this function, parameters validity is considered as already checked,
  *         i.e. Rx State, buffer address, ...
  *         UART Handle is assumed as Locked.
  * @param  huart UART handle.
  * @param  pData Pointer to data buffer (u8 or u16 data elements).
  * @param  Size  Amount of data elements (u8 or u16) to be received.
  * @retval HAL status
  */
HAL_StatusTypeDef UART_Start_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  huart->pRxBuffPtr = pData;                            /* 赋值或初始化 */
  huart->RxXferSize = Size;                             /* 赋值或初始化 */
  huart->RxXferCount = Size;                            /* 赋值或初始化 */

  huart->ErrorCode = HAL_UART_ERROR_NONE;               /* 赋值或初始化 */
  huart->RxState = HAL_UART_STATE_BUSY_RX;              /* 赋值或初始化 */

  /* Process Unlocked */
  __HAL_UNLOCK(huart);                                  /* HAL辅助宏/函数 */

  /* Enable the UART Parity Error Interrupt */
  __HAL_UART_ENABLE_IT(huart, UART_IT_PE);              /* HAL辅助宏/函数 */

  /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
  __HAL_UART_ENABLE_IT(huart, UART_IT_ERR);             /* HAL辅助宏/函数 */

  /* Enable the UART Data Register not empty Interrupt */
  __HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);            /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Start Receive operation in DMA mode.
  * @note   This function could be called by all HAL UART API providing reception in DMA mode.
  * @note   When calling this function, parameters validity is considered as already checked,
  *         i.e. Rx State, buffer address, ...
  *         UART Handle is assumed as Locked.
  * @param  huart UART handle.
  * @param  pData Pointer to data buffer (u8 or u16 data elements).
  * @param  Size  Amount of data elements (u8 or u16) to be received.
  * @retval HAL status
  */
HAL_StatusTypeDef UART_Start_Receive_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t *tmp;                                        /* 32位无符号变量/函数 */

  huart->pRxBuffPtr = pData;                            /* 赋值或初始化 */
  huart->RxXferSize = Size;                             /* 赋值或初始化 */

  huart->ErrorCode = HAL_UART_ERROR_NONE;               /* 赋值或初始化 */
  huart->RxState = HAL_UART_STATE_BUSY_RX;              /* 赋值或初始化 */

  /* Set the UART DMA transfer complete callback */
  huart->hdmarx->XferCpltCallback = UART_DMAReceiveCplt;  /* 赋值或初始化 */

  /* Set the UART DMA Half transfer complete callback */
  huart->hdmarx->XferHalfCpltCallback = UART_DMARxHalfCplt;  /* 赋值或初始化 */

  /* Set the DMA error callback */
  huart->hdmarx->XferErrorCallback = UART_DMAError;     /* 赋值或初始化 */

  /* Set the DMA abort callback */
  huart->hdmarx->XferAbortCallback = NULL;              /* 赋值或初始化 */

  /* Enable the DMA stream */
  tmp = (uint32_t *)&pData;                             /* 赋值或初始化 */
  HAL_DMA_Start_IT(huart->hdmarx, (uint32_t)&huart->Instance->DR, *(uint32_t *)tmp, Size);  /* HAL DMA操作 */

  /* Clear the Overrun flag just before enabling the DMA Rx request: can be mandatory for the second transfer */
  __HAL_UART_CLEAR_OREFLAG(huart);                      /* HAL辅助宏/函数 */

  /* Process Unlocked */
  __HAL_UNLOCK(huart);                                  /* HAL辅助宏/函数 */

  /* Enable the UART Parity Error Interrupt */
  SET_BIT(huart->Instance->CR1, USART_CR1_PEIE);        /* 执行语句 */

  /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
  SET_BIT(huart->Instance->CR3, USART_CR3_EIE);         /* 执行语句 */

  /* Enable the DMA transfer for the receiver request by setting the DMAR bit
  in the UART CR3 register */
  SET_BIT(huart->Instance->CR3, USART_CR3_DMAR);        /* 执行语句 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  End ongoing Tx transfer on UART peripheral (following error detection or Transmit completion).
  * @param  huart UART handle.
  * @retval None
  */
static void UART_EndTxTransfer(UART_HandleTypeDef *huart)  /* 静态函数 */
{                                                       /* 代码块开始 */
  /* Disable TXEIE and TCIE interrupts */
  CLEAR_BIT(huart->Instance->CR1, (USART_CR1_TXEIE | USART_CR1_TCIE));  /* 执行语句 */

  /* At end of Tx process, restore huart->gState to Ready */
  huart->gState = HAL_UART_STATE_READY;                 /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  End ongoing Rx transfer on UART peripheral (following error detection or Reception completion).
  * @param  huart UART handle.
  * @retval None
  */
static void UART_EndRxTransfer(UART_HandleTypeDef *huart)  /* 静态函数 */
{                                                       /* 代码块开始 */
  /* Disable RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts */
  CLEAR_BIT(huart->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE));  /* 执行语句 */
  CLEAR_BIT(huart->Instance->CR3, USART_CR3_EIE);       /* 执行语句 */

  /* In case of reception waiting for IDLE event, disable also the IDLE IE interrupt source */
  if (huart->ReceptionType == HAL_UART_RECEPTION_TOIDLE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    CLEAR_BIT(huart->Instance->CR1, USART_CR1_IDLEIE);  /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* At end of Rx process, restore huart->RxState to Ready */
  huart->RxState = HAL_UART_STATE_READY;                /* 赋值或初始化 */
  huart->ReceptionType = HAL_UART_RECEPTION_STANDARD;   /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA UART communication abort callback, when initiated by HAL services on Error
  *         (To be called at end of DMA Abort procedure following error occurrence).
  * @param  hdma  Pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA module.
  * @retval None
  */
static void UART_DMAAbortOnError(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  UART_HandleTypeDef *huart = (UART_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;  /* 赋值或初始化 */
  huart->RxXferCount = 0x00U;                           /* 赋值或初始化 */
  huart->TxXferCount = 0x00U;                           /* 赋值或初始化 */

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)              /* 条件编译 */
  /*Call registered error callback*/
  huart->ErrorCallback(huart);                          /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /*Call legacy weak error callback*/
  HAL_UART_ErrorCallback(huart);                        /* HAL 串口操作 */
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA UART Tx communication abort callback, when initiated by user
  *         (To be called at end of DMA Tx Abort procedure following user abort request).
  * @note   When this callback is executed, User Abort complete call back is called only if no
  *         Abort still ongoing for Rx DMA Handle.
  * @param  hdma  Pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA module.
  * @retval None
  */
static void UART_DMATxAbortCallback(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  UART_HandleTypeDef *huart = (UART_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;  /* 赋值或初始化 */

  huart->hdmatx->XferAbortCallback = NULL;              /* 赋值或初始化 */

  /* Check if an Abort process is still ongoing */
  if (huart->hdmarx != NULL)                            /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (huart->hdmarx->XferAbortCallback != NULL)       /* 条件判断 */
    {                                                   /* 代码块开始 */
      return;                                           /* 返回 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* No Abort process still ongoing : All DMA channels are aborted, call user Abort Complete callback */
  huart->TxXferCount = 0x00U;                           /* 赋值或初始化 */
  huart->RxXferCount = 0x00U;                           /* 赋值或初始化 */

  /* Reset ErrorCode */
  huart->ErrorCode = HAL_UART_ERROR_NONE;               /* 赋值或初始化 */

  /* Restore huart->gState and huart->RxState to Ready */
  huart->gState  = HAL_UART_STATE_READY;                /* 赋值或初始化 */
  huart->RxState = HAL_UART_STATE_READY;                /* 赋值或初始化 */
  huart->ReceptionType = HAL_UART_RECEPTION_STANDARD;   /* 赋值或初始化 */

  /* Call user Abort complete callback */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)              /* 条件编译 */
  /* Call registered Abort complete callback */
  huart->AbortCpltCallback(huart);                      /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* Call legacy weak Abort complete callback */
  HAL_UART_AbortCpltCallback(huart);                    /* HAL 串口操作 */
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA UART Rx communication abort callback, when initiated by user
  *         (To be called at end of DMA Rx Abort procedure following user abort request).
  * @note   When this callback is executed, User Abort complete call back is called only if no
  *         Abort still ongoing for Tx DMA Handle.
  * @param  hdma  Pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA module.
  * @retval None
  */
static void UART_DMARxAbortCallback(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  UART_HandleTypeDef *huart = (UART_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;  /* 赋值或初始化 */

  huart->hdmarx->XferAbortCallback = NULL;              /* 赋值或初始化 */

  /* Check if an Abort process is still ongoing */
  if (huart->hdmatx != NULL)                            /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (huart->hdmatx->XferAbortCallback != NULL)       /* 条件判断 */
    {                                                   /* 代码块开始 */
      return;                                           /* 返回 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* No Abort process still ongoing : All DMA channels are aborted, call user Abort Complete callback */
  huart->TxXferCount = 0x00U;                           /* 赋值或初始化 */
  huart->RxXferCount = 0x00U;                           /* 赋值或初始化 */

  /* Reset ErrorCode */
  huart->ErrorCode = HAL_UART_ERROR_NONE;               /* 赋值或初始化 */

  /* Restore huart->gState and huart->RxState to Ready */
  huart->gState  = HAL_UART_STATE_READY;                /* 赋值或初始化 */
  huart->RxState = HAL_UART_STATE_READY;                /* 赋值或初始化 */
  huart->ReceptionType = HAL_UART_RECEPTION_STANDARD;   /* 赋值或初始化 */

  /* Call user Abort complete callback */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)              /* 条件编译 */
  /* Call registered Abort complete callback */
  huart->AbortCpltCallback(huart);                      /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* Call legacy weak Abort complete callback */
  HAL_UART_AbortCpltCallback(huart);                    /* HAL 串口操作 */
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA UART Tx communication abort callback, when initiated by user by a call to
  *         HAL_UART_AbortTransmit_IT API (Abort only Tx transfer)
  *         (This callback is executed at end of DMA Tx Abort procedure following user abort request,
  *         and leads to user Tx Abort Complete callback execution).
  * @param  hdma  Pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA module.
  * @retval None
  */
static void UART_DMATxOnlyAbortCallback(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  UART_HandleTypeDef *huart = (UART_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;  /* 赋值或初始化 */

  huart->TxXferCount = 0x00U;                           /* 赋值或初始化 */

  /* Restore huart->gState to Ready */
  huart->gState = HAL_UART_STATE_READY;                 /* 赋值或初始化 */

  /* Call user Abort complete callback */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)              /* 条件编译 */
  /* Call registered Abort Transmit Complete Callback */
  huart->AbortTransmitCpltCallback(huart);              /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* Call legacy weak Abort Transmit Complete Callback */
  HAL_UART_AbortTransmitCpltCallback(huart);            /* HAL 串口操作 */
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA UART Rx communication abort callback, when initiated by user by a call to
  *         HAL_UART_AbortReceive_IT API (Abort only Rx transfer)
  *         (This callback is executed at end of DMA Rx Abort procedure following user abort request,
  *         and leads to user Rx Abort Complete callback execution).
  * @param  hdma  Pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA module.
  * @retval None
  */
static void UART_DMARxOnlyAbortCallback(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  UART_HandleTypeDef *huart = (UART_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;  /* 赋值或初始化 */

  huart->RxXferCount = 0x00U;                           /* 赋值或初始化 */

  /* Restore huart->RxState to Ready */
  huart->RxState = HAL_UART_STATE_READY;                /* 赋值或初始化 */
  huart->ReceptionType = HAL_UART_RECEPTION_STANDARD;   /* 赋值或初始化 */

  /* Call user Abort complete callback */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)              /* 条件编译 */
  /* Call registered Abort Receive Complete Callback */
  huart->AbortReceiveCpltCallback(huart);               /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* Call legacy weak Abort Receive Complete Callback */
  HAL_UART_AbortReceiveCpltCallback(huart);             /* HAL 串口操作 */
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
}                                                       /* 代码块结束 */

/**
  * @brief  Sends an amount of data in non blocking mode.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval HAL status
  */
static HAL_StatusTypeDef UART_Transmit_IT(UART_HandleTypeDef *huart)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  uint16_t *tmp;                                        /* 16位无符号变量/函数 */

  /* Check that a Tx process is ongoing */
  if (huart->gState == HAL_UART_STATE_BUSY_TX)          /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((huart->Init.WordLength == UART_WORDLENGTH_9B) && (huart->Init.Parity == UART_PARITY_NONE))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      tmp = (uint16_t *) huart->pTxBuffPtr;             /* 赋值或初始化 */
      huart->Instance->DR = (uint16_t)(*tmp & (uint16_t)0x01FF);  /* 赋值或初始化 */
      huart->pTxBuffPtr += 2U;                          /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      huart->Instance->DR = (uint8_t)(*huart->pTxBuffPtr++ & (uint8_t)0x00FF);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    if (--huart->TxXferCount == 0U)                     /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable the UART Transmit Complete Interrupt */
      __HAL_UART_DISABLE_IT(huart, UART_IT_TXE);        /* HAL辅助宏/函数 */

      /* Enable the UART Transmit Complete Interrupt */
      __HAL_UART_ENABLE_IT(huart, UART_IT_TC);          /* HAL辅助宏/函数 */
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
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval HAL status
  */
static HAL_StatusTypeDef UART_EndTransmit_IT(UART_HandleTypeDef *huart)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  /* Disable the UART Transmit Complete Interrupt */
  __HAL_UART_DISABLE_IT(huart, UART_IT_TC);             /* HAL辅助宏/函数 */

  /* Tx process is ended, restore huart->gState to Ready */
  huart->gState = HAL_UART_STATE_READY;                 /* 赋值或初始化 */

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)              /* 条件编译 */
  /*Call registered Tx complete callback*/
  huart->TxCpltCallback(huart);                         /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /*Call legacy weak Tx complete callback*/
  HAL_UART_TxCpltCallback(huart);                       /* HAL 串口操作 */
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Receives an amount of data in non blocking mode
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval HAL status
  */
static HAL_StatusTypeDef UART_Receive_IT(UART_HandleTypeDef *huart)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  uint8_t  *pdata8bits;                                 /* 8位无符号变量/函数 */
  uint16_t *pdata16bits;                                /* 16位无符号变量/函数 */

  /* Check that a Rx process is ongoing */
  if (huart->RxState == HAL_UART_STATE_BUSY_RX)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((huart->Init.WordLength == UART_WORDLENGTH_9B) && (huart->Init.Parity == UART_PARITY_NONE))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      pdata8bits  = NULL;                               /* 赋值或初始化 */
      pdata16bits = (uint16_t *) huart->pRxBuffPtr;     /* 赋值或初始化 */
      *pdata16bits = (uint16_t)(huart->Instance->DR & (uint16_t)0x01FF);  /* 赋值或初始化 */
      huart->pRxBuffPtr += 2U;                          /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      pdata8bits = (uint8_t *) huart->pRxBuffPtr;       /* 赋值或初始化 */
      pdata16bits  = NULL;                              /* 赋值或初始化 */

      if ((huart->Init.WordLength == UART_WORDLENGTH_9B) || ((huart->Init.WordLength == UART_WORDLENGTH_8B) && (huart->Init.Parity == UART_PARITY_NONE)))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        *pdata8bits = (uint8_t)(huart->Instance->DR & (uint8_t)0x00FF);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        *pdata8bits = (uint8_t)(huart->Instance->DR & (uint8_t)0x007F);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      huart->pRxBuffPtr += 1U;                          /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    if (--huart->RxXferCount == 0U)                     /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable the UART Data Register not empty Interrupt */
      __HAL_UART_DISABLE_IT(huart, UART_IT_RXNE);       /* HAL辅助宏/函数 */

      /* Disable the UART Parity Error Interrupt */
      __HAL_UART_DISABLE_IT(huart, UART_IT_PE);         /* HAL辅助宏/函数 */

      /* Disable the UART Error Interrupt: (Frame error, noise error, overrun error) */
      __HAL_UART_DISABLE_IT(huart, UART_IT_ERR);        /* HAL辅助宏/函数 */

      /* Rx process is completed, restore huart->RxState to Ready */
      huart->RxState = HAL_UART_STATE_READY;            /* 赋值或初始化 */

      /* Check current reception Mode :
         If Reception till IDLE event has been selected : */
      if (huart->ReceptionType == HAL_UART_RECEPTION_TOIDLE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Disable IDLE interrupt */
        CLEAR_BIT(huart->Instance->CR1, USART_CR1_IDLEIE);  /* 执行语句 */

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)              /* 条件编译 */
        /*Call registered Rx Event callback*/
        huart->RxEventCallback(huart, huart->RxXferSize);  /* 执行语句 */
#else                                                   /* 条件编译否则 */
        /*Call legacy weak Rx Event callback*/
        HAL_UARTEx_RxEventCallback(huart, huart->RxXferSize);  /* HAL库函数调用 */
#endif                                                  /* 条件编译结束 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
       /* Standard reception API called */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)		        /* 条件编译 */
       /*Call registered Rx complete callback*/
       huart->RxCpltCallback(huart);                    /* 执行语句 */
#else                                                   /* 条件编译否则 */
       /*Call legacy weak Rx complete callback*/
       HAL_UART_RxCpltCallback(huart);                  /* HAL 串口操作 */
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
      }                                                 /* 代码块结束 */
      huart->ReceptionType = HAL_UART_RECEPTION_STANDARD;  /* 赋值或初始化 */
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
  * @brief  Configures the UART peripheral.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
static void UART_SetConfig(UART_HandleTypeDef *huart)   /* 静态函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpreg;                                      /* 32位无符号变量/函数 */
  uint32_t pclk;                                        /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_UART_BAUDRATE(huart->Init.BaudRate));  /* 执行语句 */
  assert_param(IS_UART_STOPBITS(huart->Init.StopBits));  /* 执行语句 */
  assert_param(IS_UART_PARITY(huart->Init.Parity));     /* 执行语句 */
  assert_param(IS_UART_MODE(huart->Init.Mode));         /* 执行语句 */

  /*-------------------------- USART CR2 Configuration -----------------------*/
  /* Configure the UART Stop Bits: Set STOP[13:12] bits
     according to huart->Init.StopBits value */
  MODIFY_REG(huart->Instance->CR2, USART_CR2_STOP, huart->Init.StopBits);  /* 执行语句 */

  /*-------------------------- USART CR1 Configuration -----------------------*/
  /* Configure the UART Word Length, Parity and mode:
     Set the M bits according to huart->Init.WordLength value
     Set PCE and PS bits according to huart->Init.Parity value
     Set TE and RE bits according to huart->Init.Mode value
     Set OVER8 bit according to huart->Init.OverSampling value */

  tmpreg = (uint32_t)huart->Init.WordLength | huart->Init.Parity | huart->Init.Mode | huart->Init.OverSampling;  /* 赋值或初始化 */
  MODIFY_REG(huart->Instance->CR1,                      /* 列表续行 */
             (uint32_t)(USART_CR1_M | USART_CR1_PCE | USART_CR1_PS | USART_CR1_TE | USART_CR1_RE | USART_CR1_OVER8),  /* 列表续行 */
             tmpreg);                                   /* 执行语句 */

  /*-------------------------- USART CR3 Configuration -----------------------*/
  /* Configure the UART HFC: Set CTSE and RTSE bits according to huart->Init.HwFlowCtl value */
  MODIFY_REG(huart->Instance->CR3, (USART_CR3_RTSE | USART_CR3_CTSE), huart->Init.HwFlowCtl);  /* 执行语句 */


#if defined(USART6) && defined(UART9) && defined(UART10)  /* 条件编译 */
    if ((huart->Instance == USART1) || (huart->Instance == USART6) || (huart->Instance == UART9) || (huart->Instance == UART10))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      pclk = HAL_RCC_GetPCLK2Freq();                    /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
#elif defined(USART6)                                   /* 条件编译否则如果 */
    if ((huart->Instance == USART1) || (huart->Instance == USART6))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      pclk = HAL_RCC_GetPCLK2Freq();                    /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
#else                                                   /* 条件编译否则 */
    if (huart->Instance == USART1)                      /* 条件判断 */
    {                                                   /* 代码块开始 */
      pclk = HAL_RCC_GetPCLK2Freq();                    /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
#endif /* USART6 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      pclk = HAL_RCC_GetPCLK1Freq();                    /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  /*-------------------------- USART BRR Configuration ---------------------*/
  if (huart->Init.OverSampling == UART_OVERSAMPLING_8)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    huart->Instance->BRR = UART_BRR_SAMPLING8(pclk, huart->Init.BaudRate);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    huart->Instance->BRR = UART_BRR_SAMPLING16(pclk, huart->Init.BaudRate);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

#endif /* HAL_UART_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
