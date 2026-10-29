/* 文件: stm32f4xx_hal_cec.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_cec.c
  * @author  MCD Application Team
  * @brief   CEC HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the High Definition Multimedia Interface
  *          Consumer Electronics Control Peripheral (CEC).
  *           + Initialization and de-initialization function
  *           + IO operation function
  *           + Peripheral Control function
  *
  *
  @verbatim
 ===============================================================================
                        ##### How to use this driver #####
 ===============================================================================
    [..]
    The CEC HAL driver can be used as follow:

    (#) Declare a CEC_HandleTypeDef handle structure.
    (#) Initialize the CEC low level resources by implementing the HAL_CEC_MspInit ()API:
        (##) Enable the CEC interface clock.
        (##) CEC pins configuration:
            (+++) Enable the clock for the CEC GPIOs.
            (+++) Configure these CEC pins as alternate function pull-up.
        (##) NVIC configuration if you need to use interrupt process (HAL_CEC_Transmit_IT()
             and HAL_CEC_Receive_IT() APIs):
            (+++) Configure the CEC interrupt priority.
            (+++) Enable the NVIC CEC IRQ handle.
            (+++) The specific CEC interrupts (Transmission complete interrupt,
                  RXNE interrupt and Error Interrupts) will be managed using the macros
                  __HAL_CEC_ENABLE_IT() and __HAL_CEC_DISABLE_IT() inside the transmit
                  and receive process.

    (#) Program the Signal Free Time (SFT) and SFT option, Tolerance, reception stop in
        in case of Bit Rising Error, Error-Bit generation conditions, device logical
        address and Listen mode in the hcec Init structure.

    (#) Initialize the CEC registers by calling the HAL_CEC_Init() API.

  [..]
    (@) This API (HAL_CEC_Init()) configures also the low level Hardware (GPIO, CLOCK, CORTEX...etc)
        by calling the customed HAL_CEC_MspInit() API.
  *** Callback registration ***
  =============================================

  The compilation define  USE_HAL_CEC_REGISTER_CALLBACKS when set to 1
  allows the user to configure dynamically the driver callbacks.
  Use Functions @ref HAL_CEC_RegisterCallback() or HAL_CEC_RegisterXXXCallback()
  to register an interrupt callback.

  Function @ref HAL_CEC_RegisterCallback() allows to register following callbacks:
    (+) TxCpltCallback     : Tx Transfer completed callback.
    (+) ErrorCallback      : callback for error detection.
    (+) MspInitCallback    : CEC MspInit.
    (+) MspDeInitCallback  : CEC MspDeInit.
  This function takes as parameters the HAL peripheral handle, the Callback ID
  and a pointer to the user callback function.

  For specific callback HAL_CEC_RxCpltCallback use dedicated register callbacks
  @ref HAL_CEC_RegisterRxCpltCallback().

  Use function @ref HAL_CEC_UnRegisterCallback() to reset a callback to the default
  weak function.
  @ref HAL_CEC_UnRegisterCallback() takes as parameters the HAL peripheral handle,
  and the Callback ID.
  This function allows to reset following callbacks:
    (+) TxCpltCallback     : Tx Transfer completed callback.
    (+) ErrorCallback      : callback for error detection.
    (+) MspInitCallback    : CEC MspInit.
    (+) MspDeInitCallback  : CEC MspDeInit.

  For callback HAL_CEC_RxCpltCallback use dedicated unregister callback :
  @ref HAL_CEC_UnRegisterRxCpltCallback().

  By default, after the @ref HAL_CEC_Init() and when the state is HAL_CEC_STATE_RESET
  all callbacks are set to the corresponding weak functions :
  examples @ref HAL_CEC_TxCpltCallback() , @ref HAL_CEC_RxCpltCallback().
  Exception done for MspInit and MspDeInit functions that are
  reset to the legacy weak function in the @ref HAL_CEC_Init()/ @ref HAL_CEC_DeInit() only when
  these callbacks are null (not registered beforehand).
  if not, MspInit or MspDeInit are not null, the @ref HAL_CEC_Init() / @ref HAL_CEC_DeInit()
  keep and use the user MspInit/MspDeInit functions (registered beforehand)

  Callbacks can be registered/unregistered in HAL_CEC_STATE_READY state only.
  Exception done MspInit/MspDeInit callbacks that can be registered/unregistered
  in HAL_CEC_STATE_READY or HAL_CEC_STATE_RESET state,
  thus registered (user) MspInit/DeInit callbacks can be used during the Init/DeInit.
  In that case first register the MspInit/MspDeInit user callbacks
  using @ref HAL_CEC_RegisterCallback() before calling @ref HAL_CEC_DeInit()
  or @ref HAL_CEC_Init() function.

  When the compilation define USE_HAL_CEC_REGISTER_CALLBACKS is set to 0 or
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

/** @defgroup CEC CEC
  * @brief HAL CEC module driver
  * @{
  */
#ifdef HAL_CEC_MODULE_ENABLED                           /* 条件编译 */
#if defined (CEC)                                       /* 条件编译 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @defgroup CEC_Private_Constants CEC Private Constants
  * @{
  */
/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/** @defgroup CEC_Private_Functions CEC Private Functions
  * @{
  */
/**
  * @}
  */

/* Exported functions ---------------------------------------------------------*/

/** @defgroup CEC_Exported_Functions CEC Exported Functions
  * @{
  */

/** @defgroup CEC_Exported_Functions_Group1 Initialization and de-initialization functions
  *  @brief    Initialization and Configuration functions
  *
@verbatim
===============================================================================
            ##### Initialization and Configuration functions #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to initialize the CEC
      (+) The following parameters need to be configured:
        (++) SignalFreeTime
        (++) Tolerance
        (++) BRERxStop                 (RX stopped or not upon Bit Rising Error)
        (++) BREErrorBitGen            (Error-Bit generation in case of Bit Rising Error)
        (++) LBPEErrorBitGen           (Error-Bit generation in case of Long Bit Period Error)
        (++) BroadcastMsgNoErrorBitGen (Error-bit generation in case of broadcast message error)
        (++) SignalFreeTimeOption      (SFT Timer start definition)
        (++) OwnAddress                (CEC device address)
        (++) ListenMode

@endverbatim
  * @{
  */

/**
  * @brief Initializes the CEC mode according to the specified
  *         parameters in the CEC_InitTypeDef and creates the associated handle .
  * @param hcec CEC handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_CEC_Init(CEC_HandleTypeDef *hcec)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the CEC handle allocation */
  if ((hcec == NULL) || (hcec->Init.RxBuffer == NULL))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the parameters */
  assert_param(IS_CEC_ALL_INSTANCE(hcec->Instance));    /* 执行语句 */
  assert_param(IS_CEC_SIGNALFREETIME(hcec->Init.SignalFreeTime));  /* 执行语句 */
  assert_param(IS_CEC_TOLERANCE(hcec->Init.Tolerance));  /* 执行语句 */
  assert_param(IS_CEC_BRERXSTOP(hcec->Init.BRERxStop));  /* 执行语句 */
  assert_param(IS_CEC_BREERRORBITGEN(hcec->Init.BREErrorBitGen));  /* 执行语句 */
  assert_param(IS_CEC_LBPEERRORBITGEN(hcec->Init.LBPEErrorBitGen));  /* 执行语句 */
  assert_param(IS_CEC_BROADCASTERROR_NO_ERRORBIT_GENERATION(hcec->Init.BroadcastMsgNoErrorBitGen));  /* 执行语句 */
  assert_param(IS_CEC_SFTOP(hcec->Init.SignalFreeTimeOption));  /* 执行语句 */
  assert_param(IS_CEC_LISTENING_MODE(hcec->Init.ListenMode));  /* 执行语句 */
  assert_param(IS_CEC_OWN_ADDRESS(hcec->Init.OwnAddress));  /* 执行语句 */

#if (USE_HAL_CEC_REGISTER_CALLBACKS == 1)               /* 条件编译 */
  if (hcec->gState == HAL_CEC_STATE_RESET)              /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Allocate lock resource and initialize it */
    hcec->Lock = HAL_UNLOCKED;                          /* 赋值或初始化 */

    hcec->TxCpltCallback  = HAL_CEC_TxCpltCallback;  /* Legacy weak TxCpltCallback  */
    hcec->RxCpltCallback = HAL_CEC_RxCpltCallback;   /* Legacy weak RxCpltCallback */
    hcec->ErrorCallback = HAL_CEC_ErrorCallback;     /* Legacy weak ErrorCallback */

    if (hcec->MspInitCallback == NULL)                  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hcec->MspInitCallback = HAL_CEC_MspInit; /* Legacy weak MspInit  */
    }                                                   /* 代码块结束 */

    /* Init the low level hardware */
    hcec->MspInitCallback(hcec);                        /* 执行语句 */
  }                                                     /* 代码块结束 */
#else                                                   /* 条件编译否则 */
  if (hcec->gState == HAL_CEC_STATE_RESET)              /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Allocate lock resource and initialize it */
    hcec->Lock = HAL_UNLOCKED;                          /* 赋值或初始化 */
    /* Init the low level hardware : GPIO, CLOCK */
    HAL_CEC_MspInit(hcec);                              /* HAL库函数调用 */
  }                                                     /* 代码块结束 */
#endif /* USE_HAL_CEC_REGISTER_CALLBACKS */

  hcec->gState = HAL_CEC_STATE_BUSY;                    /* 赋值或初始化 */

  /* Disable the Peripheral */
  __HAL_CEC_DISABLE(hcec);                              /* HAL辅助宏/函数 */

  /* Write to CEC Control Register */
  hcec->Instance->CFGR = hcec->Init.SignalFreeTime | hcec->Init.Tolerance | hcec->Init.BRERxStop | \
                         hcec->Init.BREErrorBitGen | hcec->Init.LBPEErrorBitGen | hcec->Init.BroadcastMsgNoErrorBitGen | \
                         hcec->Init.SignalFreeTimeOption | ((uint32_t)(hcec->Init.OwnAddress) << 16U) | \
                         hcec->Init.ListenMode;         /* 执行语句 */

  /* Enable the following CEC Transmission/Reception interrupts as
    * well as the following CEC Transmission/Reception Errors interrupts
    * Rx Byte Received IT
    * End of Reception IT
    * Rx overrun
    * Rx bit rising error
    * Rx short bit period error
    * Rx long bit period error
    * Rx missing acknowledge
    * Tx Byte Request IT
    * End of Transmission IT
    * Tx Missing Acknowledge IT
    * Tx-Error IT
    * Tx-Buffer Underrun IT
    * Tx arbitration lost   */
  __HAL_CEC_ENABLE_IT(hcec, CEC_IT_RXBR | CEC_IT_RXEND | CEC_IER_RX_ALL_ERR | CEC_IT_TXBR | CEC_IT_TXEND |  /* HAL辅助宏/函数 */
                      CEC_IER_TX_ALL_ERR);              /* 执行语句 */

  /* Enable the CEC Peripheral */
  __HAL_CEC_ENABLE(hcec);                               /* HAL辅助宏/函数 */

  hcec->ErrorCode = HAL_CEC_ERROR_NONE;                 /* 赋值或初始化 */
  hcec->gState = HAL_CEC_STATE_READY;                   /* 赋值或初始化 */
  hcec->RxState = HAL_CEC_STATE_READY;                  /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief DeInitializes the CEC peripheral
  * @param hcec CEC handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_CEC_DeInit(CEC_HandleTypeDef *hcec)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the CEC handle allocation */
  if (hcec == NULL)                                     /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the parameters */
  assert_param(IS_CEC_ALL_INSTANCE(hcec->Instance));    /* 执行语句 */

  hcec->gState = HAL_CEC_STATE_BUSY;                    /* 赋值或初始化 */

#if (USE_HAL_CEC_REGISTER_CALLBACKS == 1)               /* 条件编译 */
  if (hcec->MspDeInitCallback == NULL)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hcec->MspDeInitCallback = HAL_CEC_MspDeInit; /* Legacy weak MspDeInit  */
  }                                                     /* 代码块结束 */

  /* DeInit the low level hardware */
  hcec->MspDeInitCallback(hcec);                        /* 执行语句 */

#else                                                   /* 条件编译否则 */
  /* DeInit the low level hardware */
  HAL_CEC_MspDeInit(hcec);                              /* HAL库函数调用 */
#endif /* USE_HAL_CEC_REGISTER_CALLBACKS */

  /* Disable the Peripheral */
  __HAL_CEC_DISABLE(hcec);                              /* HAL辅助宏/函数 */

  /* Clear Flags */
  __HAL_CEC_CLEAR_FLAG(hcec, CEC_FLAG_TXEND | CEC_FLAG_TXBR | CEC_FLAG_RXBR | CEC_FLAG_RXEND | CEC_ISR_ALL_ERROR);  /* HAL辅助宏/函数 */

  /* Disable the following CEC Transmission/Reception interrupts as
    * well as the following CEC Transmission/Reception Errors interrupts
    * Rx Byte Received IT
    * End of Reception IT
    * Rx overrun
    * Rx bit rising error
    * Rx short bit period error
    * Rx long bit period error
    * Rx missing acknowledge
    * Tx Byte Request IT
    * End of Transmission IT
    * Tx Missing Acknowledge IT
    * Tx-Error IT
    * Tx-Buffer Underrun IT
    * Tx arbitration lost   */
  __HAL_CEC_DISABLE_IT(hcec, CEC_IT_RXBR | CEC_IT_RXEND | CEC_IER_RX_ALL_ERR | CEC_IT_TXBR | CEC_IT_TXEND |  /* HAL辅助宏/函数 */
                       CEC_IER_TX_ALL_ERR);             /* 执行语句 */

  hcec->ErrorCode = HAL_CEC_ERROR_NONE;                 /* 赋值或初始化 */
  hcec->gState = HAL_CEC_STATE_RESET;                   /* 赋值或初始化 */
  hcec->RxState = HAL_CEC_STATE_RESET;                  /* 赋值或初始化 */

  /* Process Unlock */
  __HAL_UNLOCK(hcec);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief Initializes the Own Address of the CEC device
  * @param hcec CEC handle
  * @param  CEC_OwnAddress The CEC own address.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_CEC_SetDeviceAddress(CEC_HandleTypeDef *hcec, uint16_t CEC_OwnAddress)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_CEC_OWN_ADDRESS(CEC_OwnAddress));     /* 执行语句 */

  if ((hcec->gState == HAL_CEC_STATE_READY) && (hcec->RxState == HAL_CEC_STATE_READY))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Process Locked */
    __HAL_LOCK(hcec);                                   /* HAL辅助宏/函数 */

    hcec->gState = HAL_CEC_STATE_BUSY;                  /* 赋值或初始化 */

    /* Disable the Peripheral */
    __HAL_CEC_DISABLE(hcec);                            /* HAL辅助宏/函数 */

    if (CEC_OwnAddress != CEC_OWN_ADDRESS_NONE)         /* 条件判断 */
    {                                                   /* 代码块开始 */
      hcec->Instance->CFGR |= ((uint32_t)CEC_OwnAddress << 16);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hcec->Instance->CFGR &= ~(CEC_CFGR_OAR);          /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    hcec->gState = HAL_CEC_STATE_READY;                 /* 赋值或初始化 */
    hcec->ErrorCode = HAL_CEC_ERROR_NONE;               /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hcec);                                 /* HAL辅助宏/函数 */

    /* Enable the Peripheral */
    __HAL_CEC_ENABLE(hcec);                             /* HAL辅助宏/函数 */

    return  HAL_OK;                                     /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief CEC MSP Init
  * @param hcec CEC handle
  * @retval None
  */
__weak void HAL_CEC_MspInit(CEC_HandleTypeDef *hcec)    /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hcec);                                         /* 执行语句 */
  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_CEC_MspInit can be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief CEC MSP DeInit
  * @param hcec CEC handle
  * @retval None
  */
__weak void HAL_CEC_MspDeInit(CEC_HandleTypeDef *hcec)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hcec);                                         /* 执行语句 */
  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_CEC_MspDeInit can be implemented in the user file
   */
}                                                       /* 代码块结束 */
#if (USE_HAL_CEC_REGISTER_CALLBACKS == 1)               /* 条件编译 */
/**
  * @brief  Register a User CEC Callback
  *         To be used instead of the weak predefined callback
  * @param  hcec CEC handle
  * @param  CallbackID ID of the callback to be registered
  *         This parameter can be one of the following values:
  *          @arg @ref HAL_CEC_TX_CPLT_CB_ID Tx Complete callback ID
  *          @arg @ref HAL_CEC_ERROR_CB_ID Error callback ID
  *          @arg @ref HAL_CEC_MSPINIT_CB_ID MspInit callback ID
  *          @arg @ref HAL_CEC_MSPDEINIT_CB_ID MspDeInit callback ID
  * @param  pCallback pointer to the Callback function
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_CEC_RegisterCallback(CEC_HandleTypeDef *hcec, HAL_CEC_CallbackIDTypeDef CallbackID,  /* HAL状态类型函数 */
                                           pCEC_CallbackTypeDef pCallback)  /* 源代码 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  if (pCallback == NULL)                                /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hcec->ErrorCode |= HAL_CEC_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  /* Process locked */
  __HAL_LOCK(hcec);                                     /* HAL辅助宏/函数 */

  if (hcec->gState == HAL_CEC_STATE_READY)              /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_CEC_TX_CPLT_CB_ID :                      /* case标签 */
        hcec->TxCpltCallback = pCallback;               /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_CEC_ERROR_CB_ID :                        /* case标签 */
        hcec->ErrorCallback = pCallback;                /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_CEC_MSPINIT_CB_ID :                      /* case标签 */
        hcec->MspInitCallback = pCallback;              /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_CEC_MSPDEINIT_CB_ID :                    /* case标签 */
        hcec->MspDeInitCallback = pCallback;            /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hcec->ErrorCode |= HAL_CEC_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (hcec->gState == HAL_CEC_STATE_RESET)         /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_CEC_MSPINIT_CB_ID :                      /* case标签 */
        hcec->MspInitCallback = pCallback;              /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_CEC_MSPDEINIT_CB_ID :                    /* case标签 */
        hcec->MspDeInitCallback = pCallback;            /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hcec->ErrorCode |= HAL_CEC_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hcec->ErrorCode |= HAL_CEC_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hcec);                                   /* HAL辅助宏/函数 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Unregister an CEC Callback
  *         CEC callabck is redirected to the weak predefined callback
  * @param hcec uart handle
  * @param CallbackID ID of the callback to be unregistered
  *         This parameter can be one of the following values:
  *          @arg @ref HAL_CEC_TX_CPLT_CB_ID Tx Complete callback ID
  *          @arg @ref HAL_CEC_ERROR_CB_ID Error callback ID
  *          @arg @ref HAL_CEC_MSPINIT_CB_ID MspInit callback ID
  *          @arg @ref HAL_CEC_MSPDEINIT_CB_ID MspDeInit callback ID
  * @retval status
  */
HAL_StatusTypeDef HAL_CEC_UnRegisterCallback(CEC_HandleTypeDef *hcec, HAL_CEC_CallbackIDTypeDef CallbackID)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Process locked */
  __HAL_LOCK(hcec);                                     /* HAL辅助宏/函数 */

  if (hcec->gState == HAL_CEC_STATE_READY)              /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_CEC_TX_CPLT_CB_ID :                      /* case标签 */
        hcec->TxCpltCallback = HAL_CEC_TxCpltCallback;  /* Legacy weak  TxCpltCallback */
        break;                                          /* 跳出 */

      case HAL_CEC_ERROR_CB_ID :                        /* case标签 */
        hcec->ErrorCallback = HAL_CEC_ErrorCallback;  /* Legacy weak ErrorCallback   */
        break;                                          /* 跳出 */

      case HAL_CEC_MSPINIT_CB_ID :                      /* case标签 */
        hcec->MspInitCallback = HAL_CEC_MspInit;        /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_CEC_MSPDEINIT_CB_ID :                    /* case标签 */
        hcec->MspDeInitCallback = HAL_CEC_MspDeInit;    /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hcec->ErrorCode |= HAL_CEC_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (hcec->gState == HAL_CEC_STATE_RESET)         /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_CEC_MSPINIT_CB_ID :                      /* case标签 */
        hcec->MspInitCallback = HAL_CEC_MspInit;        /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_CEC_MSPDEINIT_CB_ID :                    /* case标签 */
        hcec->MspDeInitCallback = HAL_CEC_MspDeInit;    /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hcec->ErrorCode |= HAL_CEC_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hcec->ErrorCode |= HAL_CEC_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hcec);                                   /* HAL辅助宏/函数 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Register CEC RX complete Callback
  *         To be used instead of the weak HAL_CEC_RxCpltCallback() predefined callback
  * @param  hcec CEC handle
  * @param  pCallback pointer to the Rx transfer compelete Callback function
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_CEC_RegisterRxCpltCallback(CEC_HandleTypeDef *hcec, pCEC_RxCallbackTypeDef pCallback)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  if (pCallback == NULL)                                /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hcec->ErrorCode |= HAL_CEC_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  /* Process locked */
  __HAL_LOCK(hcec);                                     /* HAL辅助宏/函数 */

  if (HAL_CEC_STATE_READY == hcec->RxState)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    hcec->RxCpltCallback = pCallback;                   /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hcec->ErrorCode |= HAL_CEC_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hcec);                                   /* HAL辅助宏/函数 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  UnRegister CEC RX complete Callback
  *         CEC RX complete Callback is redirected to the weak HAL_CEC_RxCpltCallback() predefined callback
  * @param  hcec CEC handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_CEC_UnRegisterRxCpltCallback(CEC_HandleTypeDef *hcec)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Process locked */
  __HAL_LOCK(hcec);                                     /* HAL辅助宏/函数 */

  if (HAL_CEC_STATE_READY == hcec->RxState)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    hcec->RxCpltCallback = HAL_CEC_RxCpltCallback; /* Legacy weak  CEC RxCpltCallback  */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hcec->ErrorCode |= HAL_CEC_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hcec);                                   /* HAL辅助宏/函数 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* USE_HAL_CEC_REGISTER_CALLBACKS */

/**
  * @}
  */

/** @defgroup CEC_Exported_Functions_Group2 Input and Output operation functions
  *  @brief CEC Transmit/Receive functions
  *
@verbatim
 ===============================================================================
                      ##### IO operation functions #####
 ===============================================================================
    This subsection provides a set of functions allowing to manage the CEC data transfers.

    (#) The CEC handle must contain the initiator (TX side) and the destination (RX side)
        logical addresses (4-bit long addresses, 0xF for broadcast messages destination)

    (#) The communication is performed using Interrupts.
           These API's return the HAL status.
           The end of the data processing will be indicated through the
           dedicated CEC IRQ when using Interrupt mode.
           The HAL_CEC_TxCpltCallback(), HAL_CEC_RxCpltCallback() user callbacks
           will be executed respectively at the end of the transmit or Receive process
           The HAL_CEC_ErrorCallback() user callback will be executed when a communication
           error is detected

    (#) API's with Interrupt are :
         (+) HAL_CEC_Transmit_IT()
         (+) HAL_CEC_IRQHandler()

    (#) A set of User Callbacks are provided:
         (+) HAL_CEC_TxCpltCallback()
         (+) HAL_CEC_RxCpltCallback()
         (+) HAL_CEC_ErrorCallback()

@endverbatim
  * @{
  */

/**
  * @brief Send data in interrupt mode
  * @param hcec CEC handle
  * @param InitiatorAddress Initiator address
  * @param DestinationAddress destination logical address
  * @param pData pointer to input byte data buffer
  * @param Size amount of data to be sent in bytes (without counting the header).
  *              0 means only the header is sent (ping operation).
  *              Maximum TX size is 15 bytes (1 opcode and up to 14 operands).
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_CEC_Transmit_IT(CEC_HandleTypeDef *hcec, uint8_t InitiatorAddress, uint8_t DestinationAddress,  /* HAL状态类型函数 */
                                      uint8_t *pData, uint32_t Size)  /* 8位无符号变量/函数 */
{                                                       /* 代码块开始 */
  /* if the IP isn't already busy and if there is no previous transmission
     already pending due to arbitration lost */
  if (hcec->gState == HAL_CEC_STATE_READY)              /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) && (Size > 0U))                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      return  HAL_ERROR;                                /* 返回值 */
    }                                                   /* 代码块结束 */

    assert_param(IS_CEC_ADDRESS(DestinationAddress));   /* 执行语句 */
    assert_param(IS_CEC_ADDRESS(InitiatorAddress));     /* 执行语句 */
    assert_param(IS_CEC_MSGSIZE(Size));                 /* 执行语句 */

    /* Process Locked */
    __HAL_LOCK(hcec);                                   /* HAL辅助宏/函数 */
    hcec->pTxBuffPtr = pData;                           /* 赋值或初始化 */
    hcec->gState = HAL_CEC_STATE_BUSY_TX;               /* 赋值或初始化 */
    hcec->ErrorCode = HAL_CEC_ERROR_NONE;               /* 赋值或初始化 */

    /* initialize the number of bytes to send,
      * 0 means only one header is sent (ping operation) */
    hcec->TxXferCount = (uint16_t)Size;                 /* 赋值或初始化 */

    /* in case of no payload (Size = 0), sender is only pinging the system;
       Set TX End of Message (TXEOM) bit, must be set before writing data to TXDR */
    if (Size == 0U)                                     /* 条件判断 */
    {                                                   /* 代码块开始 */
      __HAL_CEC_LAST_BYTE_TX_SET(hcec);                 /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* send header block */
    hcec->Instance->TXDR = (uint32_t)(((uint32_t)InitiatorAddress << CEC_INITIATOR_LSB_POS) | DestinationAddress);  /* 赋值或初始化 */

    /* Set TX Start of Message  (TXSOM) bit */
    __HAL_CEC_FIRST_BYTE_TX_SET(hcec);                  /* HAL辅助宏/函数 */

    /* Process Unlocked */
    __HAL_UNLOCK(hcec);                                 /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */

  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief Get size of the received frame.
  * @param hcec CEC handle
  * @retval Frame size
  */
uint32_t HAL_CEC_GetLastReceivedFrameSize(CEC_HandleTypeDef *hcec)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  return hcec->RxXferSize;                              /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief Change Rx Buffer.
  * @param hcec CEC handle
  * @param Rxbuffer Rx Buffer
  * @note  This function can be called only inside the HAL_CEC_RxCpltCallback()
  * @retval Frame size
  */
void HAL_CEC_ChangeRxBuffer(CEC_HandleTypeDef *hcec, uint8_t *Rxbuffer)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  hcec->Init.RxBuffer = Rxbuffer;                       /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief This function handles CEC interrupt requests.
  * @param hcec CEC handle
  * @retval None
  */
void HAL_CEC_IRQHandler(CEC_HandleTypeDef *hcec)        /* 中断服务函数 */
{                                                       /* 代码块开始 */

  /* save interrupts register for further error or interrupts handling purposes */
  uint32_t reg;                                         /* 32位无符号变量/函数 */
  reg = hcec->Instance->ISR;                            /* 赋值或初始化 */


  /* ----------------------------Arbitration Lost Management----------------------------------*/
  /* CEC TX arbitration error interrupt occurred --------------------------------------*/
  if ((reg & CEC_FLAG_ARBLST) != 0U)                    /* 条件判断 */
  {                                                     /* 代码块开始 */
    hcec->ErrorCode = HAL_CEC_ERROR_ARBLST;             /* 赋值或初始化 */
    __HAL_CEC_CLEAR_FLAG(hcec, CEC_FLAG_ARBLST);        /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* ----------------------------Rx Management----------------------------------*/
  /* CEC RX byte received interrupt  ---------------------------------------------------*/
  if ((reg & CEC_FLAG_RXBR) != 0U)                      /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* reception is starting */
    hcec->RxState = HAL_CEC_STATE_BUSY_RX;              /* 赋值或初始化 */
    hcec->RxXferSize++;                                 /* 执行语句 */
    /* read received byte */
    *hcec->Init.RxBuffer = (uint8_t) hcec->Instance->RXDR;  /* 赋值或初始化 */
    hcec->Init.RxBuffer++;                              /* 执行语句 */
    __HAL_CEC_CLEAR_FLAG(hcec, CEC_FLAG_RXBR);          /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* CEC RX end received interrupt  ---------------------------------------------------*/
  if ((reg & CEC_FLAG_RXEND) != 0U)                     /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* clear IT */
    __HAL_CEC_CLEAR_FLAG(hcec, CEC_FLAG_RXEND);         /* HAL辅助宏/函数 */

    /* Rx process is completed, restore hcec->RxState to Ready */
    hcec->RxState = HAL_CEC_STATE_READY;                /* 赋值或初始化 */
    hcec->ErrorCode = HAL_CEC_ERROR_NONE;               /* 赋值或初始化 */
    hcec->Init.RxBuffer -= hcec->RxXferSize;            /* 赋值或初始化 */
#if (USE_HAL_CEC_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
    hcec->RxCpltCallback(hcec, hcec->RxXferSize);       /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_CEC_RxCpltCallback(hcec, hcec->RxXferSize);     /* HAL库函数调用 */
#endif /* USE_HAL_CEC_REGISTER_CALLBACKS */
    hcec->RxXferSize = 0U;                              /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* ----------------------------Tx Management----------------------------------*/
  /* CEC TX byte request interrupt ------------------------------------------------*/
  if ((reg & CEC_FLAG_TXBR) != 0U)                      /* 条件判断 */
  {                                                     /* 代码块开始 */
    --hcec->TxXferCount;                                /* 执行语句 */
    if (hcec->TxXferCount == 0U)                        /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* if this is the last byte transmission, set TX End of Message (TXEOM) bit */
      __HAL_CEC_LAST_BYTE_TX_SET(hcec);                 /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
    /* In all cases transmit the byte */
    hcec->Instance->TXDR = *hcec->pTxBuffPtr;           /* 赋值或初始化 */
    hcec->pTxBuffPtr++;                                 /* 执行语句 */
    /* clear Tx-Byte request flag */
    __HAL_CEC_CLEAR_FLAG(hcec, CEC_FLAG_TXBR);          /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* CEC TX end interrupt ------------------------------------------------*/
  if ((reg & CEC_FLAG_TXEND) != 0U)                     /* 条件判断 */
  {                                                     /* 代码块开始 */
    __HAL_CEC_CLEAR_FLAG(hcec, CEC_FLAG_TXEND);         /* HAL辅助宏/函数 */

    /* Tx process is ended, restore hcec->gState to Ready */
    hcec->gState = HAL_CEC_STATE_READY;                 /* 赋值或初始化 */
    /* Call the Process Unlocked before calling the Tx call back API to give the possibility to
    start again the Transmission under the Tx call back API */
    __HAL_UNLOCK(hcec);                                 /* HAL辅助宏/函数 */
    hcec->ErrorCode = HAL_CEC_ERROR_NONE;               /* 赋值或初始化 */
#if (USE_HAL_CEC_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
    hcec->TxCpltCallback(hcec);                         /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_CEC_TxCpltCallback(hcec);                       /* HAL库函数调用 */
#endif /* USE_HAL_CEC_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */

  /* ----------------------------Rx/Tx Error Management----------------------------------*/
  if ((reg & (CEC_ISR_RXOVR | CEC_ISR_BRE | CEC_ISR_SBPE | CEC_ISR_LBPE | CEC_ISR_RXACKE | CEC_ISR_TXUDR | CEC_ISR_TXERR |  /* 条件判断 */
              CEC_ISR_TXACKE)) != 0U)                   /* 源代码 */
  {                                                     /* 代码块开始 */
    hcec->ErrorCode = reg;                              /* 赋值或初始化 */
    __HAL_CEC_CLEAR_FLAG(hcec, HAL_CEC_ERROR_RXOVR | HAL_CEC_ERROR_BRE | CEC_FLAG_LBPE | CEC_FLAG_SBPE |  /* HAL辅助宏/函数 */
                         HAL_CEC_ERROR_RXACKE | HAL_CEC_ERROR_TXUDR | HAL_CEC_ERROR_TXERR | HAL_CEC_ERROR_TXACKE);  /* HAL库函数调用 */


    if ((reg & (CEC_ISR_RXOVR | CEC_ISR_BRE | CEC_ISR_SBPE | CEC_ISR_LBPE | CEC_ISR_RXACKE)) != 0U)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hcec->Init.RxBuffer -= hcec->RxXferSize;          /* 赋值或初始化 */
      hcec->RxXferSize = 0U;                            /* 赋值或初始化 */
      hcec->RxState = HAL_CEC_STATE_READY;              /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if (((reg & CEC_ISR_ARBLST) == 0U) && ((reg & (CEC_ISR_TXUDR | CEC_ISR_TXERR | CEC_ISR_TXACKE)) != 0U))  /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* Set the CEC state ready to be able to start again the process */
      hcec->gState = HAL_CEC_STATE_READY;               /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Nothing todo*/
    }                                                   /* 代码块结束 */
#if (USE_HAL_CEC_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
    hcec->ErrorCallback(hcec);                          /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /* Error  Call Back */
    HAL_CEC_ErrorCallback(hcec);                        /* HAL库函数调用 */
#endif /* USE_HAL_CEC_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Nothing todo*/
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief Tx Transfer completed callback
  * @param hcec CEC handle
  * @retval None
  */
__weak void HAL_CEC_TxCpltCallback(CEC_HandleTypeDef *hcec)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hcec);                                         /* 执行语句 */
  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_CEC_TxCpltCallback can be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief Rx Transfer completed callback
  * @param hcec CEC handle
  * @param RxFrameSize Size of frame
  * @retval None
  */
__weak void HAL_CEC_RxCpltCallback(CEC_HandleTypeDef *hcec, uint32_t RxFrameSize)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hcec);                                         /* 执行语句 */
  UNUSED(RxFrameSize);                                  /* 执行语句 */
  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_CEC_RxCpltCallback can be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief CEC error callbacks
  * @param hcec CEC handle
  * @retval None
  */
__weak void HAL_CEC_ErrorCallback(CEC_HandleTypeDef *hcec)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hcec);                                         /* 执行语句 */
  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_CEC_ErrorCallback can be implemented in the user file
   */
}                                                       /* 代码块结束 */
/**
  * @}
  */

/** @defgroup CEC_Exported_Functions_Group3 Peripheral Control function
  *  @brief   CEC control functions
  *
@verbatim
 ===============================================================================
                      ##### Peripheral Control function #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to control the CEC.
     (+) HAL_CEC_GetState() API can be helpful to check in run-time the state of the CEC peripheral.
     (+) HAL_CEC_GetError() API can be helpful to check in run-time the error of the CEC peripheral.
@endverbatim
  * @{
  */
/**
  * @brief return the CEC state
  * @param hcec pointer to a CEC_HandleTypeDef structure that contains
  *              the configuration information for the specified CEC module.
  * @retval HAL state
  */
HAL_CEC_StateTypeDef HAL_CEC_GetState(CEC_HandleTypeDef *hcec)  /* HAL库函数调用 */
{                                                       /* 代码块开始 */
  uint32_t temp1, temp2;                                /* 32位无符号变量/函数 */
  temp1 = hcec->gState;                                 /* 赋值或初始化 */
  temp2 = hcec->RxState;                                /* 赋值或初始化 */

  return (HAL_CEC_StateTypeDef)(temp1 | temp2);         /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return the CEC error code
  * @param  hcec  pointer to a CEC_HandleTypeDef structure that contains
  *              the configuration information for the specified CEC.
  * @retval CEC Error Code
  */
uint32_t HAL_CEC_GetError(CEC_HandleTypeDef *hcec)      /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  return hcec->ErrorCode;                               /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/**
  * @}
  */
#endif /* CEC */
#endif /* HAL_CEC_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
