/* 文件: stm32f4xx_hal_wwdg.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_wwdg.c
  * @author  MCD Application Team
  * @brief   WWDG HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Window Watchdog (WWDG) peripheral:
  *           + Initialization and Configuration functions
  *           + IO operation functions
  @verbatim
  ==============================================================================
                      ##### WWDG Specific features #####
  ==============================================================================
  [..]
    Once enabled the WWDG generates a system reset on expiry of a programmed
    time period, unless the program refreshes the counter (T[6;0] downcounter)
    before reaching 0x3F value (i.e. a reset is generated when the counter
    value rolls down from 0x40 to 0x3F).

    (+) An MCU reset is also generated if the counter value is refreshed
        before the counter has reached the refresh window value. This
        implies that the counter must be refreshed in a limited window.
    (+) Once enabled the WWDG cannot be disabled except by a system reset.
    (+) If required by application, an Early Wakeup Interrupt can be triggered
        in order to be warned before WWDG expiration. The Early Wakeup Interrupt
        (EWI) can be used if specific safety operations or data logging must
        be performed before the actual reset is generated. When the downcounter
        reaches 0x40, interrupt occurs. This mechanism requires WWDG interrupt
        line to be enabled in NVIC. Once enabled, EWI interrupt cannot be
        disabled except by a system reset.
    (+) WWDGRST flag in RCC CSR register can be used to inform when a WWDG
        reset occurs.
    (+) The WWDG counter input clock is derived from the APB clock divided
        by a programmable prescaler.
    (+) WWDG clock (Hz) = PCLK1 / (4096 * Prescaler)
    (+) WWDG timeout (mS) = 1000 * (T[5;0] + 1) / WWDG clock (Hz)
        where T[5;0] are the lowest 6 bits of Counter.
    (+) WWDG Counter refresh is allowed between the following limits :
        (++) min time (mS) = 1000 * (Counter - Window) / WWDG clock
        (++) max time (mS) = 1000 * (Counter - 0x40) / WWDG clock
    (+) Typical values:
        (++) Counter min (T[5;0] = 0x00) at 42MHz (PCLK1) with zero prescaler:
             max timeout before reset: approximately 97.52祍
        (++) Counter max (T[5;0] = 0x3F) at 42MHz (PCLK1) with prescaler
             dividing by 8:
             max timeout before reset: approximately 49.93ms

                     ##### How to use this driver #####
  ==============================================================================

    *** Common driver usage ***
    ===========================

  [..]
    (+) Enable WWDG APB1 clock using __HAL_RCC_WWDG_CLK_ENABLE().
    (+) Configure the WWDG prescaler, refresh window value, counter value and early
        interrupt status using HAL_WWDG_Init() function. This will automatically
        enable WWDG and start its downcounter. Time reference can be taken from 
        function exit. Care must be taken to provide a counter value
        greater than 0x40 to prevent generation of immediate reset.
    (+) If the Early Wakeup Interrupt (EWI) feature is enabled, an interrupt is
        generated when the counter reaches 0x40. When HAL_WWDG_IRQHandler is
        triggered by the interrupt service routine, flag will be automatically
        cleared and HAL_WWDG_WakeupCallback user callback will be executed. User
        can add his own code by customization of callback HAL_WWDG_WakeupCallback.
    (+) Then the application program must refresh the WWDG counter at regular
        intervals during normal operation to prevent an MCU reset, using
        HAL_WWDG_Refresh() function. This operation must occur only when
        the counter is lower than the refresh window value already programmed.

    *** Callback registration ***
    =============================

  [..]
    The compilation define USE_HAL_WWDG_REGISTER_CALLBACKS when set to 1 allows
    the user to configure dynamically the driver callbacks. Use Functions
    HAL_WWDG_RegisterCallback() to register a user callback.

    (+) Function HAL_WWDG_RegisterCallback() allows to register following
        callbacks:
        (++) EwiCallback : callback for Early WakeUp Interrupt.
        (++) MspInitCallback : WWDG MspInit.
    This function takes as parameters the HAL peripheral handle, the Callback ID
    and a pointer to the user callback function.

    (+) Use function HAL_WWDG_UnRegisterCallback() to reset a callback to
    the default weak (surcharged) function. HAL_WWDG_UnRegisterCallback()
    takes as parameters the HAL peripheral handle and the Callback ID.
    This function allows to reset following callbacks:
        (++) EwiCallback : callback for  Early WakeUp Interrupt.
        (++) MspInitCallback : WWDG MspInit.

    [..]
    When calling HAL_WWDG_Init function, callbacks are reset to the
    corresponding legacy weak (surcharged) functions:
    HAL_WWDG_EarlyWakeupCallback() and HAL_WWDG_MspInit() only if they have
    not been registered before.

    [..]
    When compilation define USE_HAL_WWDG_REGISTER_CALLBACKS is set to 0 or
    not defined, the callback registering feature is not available
    and weak (surcharged) callbacks are used.

    *** WWDG HAL driver macros list ***
    ===================================
    [..]
      Below the list of available macros in WWDG HAL driver.
      (+) __HAL_WWDG_ENABLE: Enable the WWDG peripheral
      (+) __HAL_WWDG_GET_FLAG: Get the selected WWDG's flag status
      (+) __HAL_WWDG_CLEAR_FLAG: Clear the WWDG's pending flags
      (+) __HAL_WWDG_ENABLE_IT: Enable the WWDG early wakeup interrupt

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

#ifdef HAL_WWDG_MODULE_ENABLED                          /* 条件编译 */
/** @defgroup WWDG WWDG
  * @brief WWDG HAL module driver.
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/** @defgroup WWDG_Exported_Functions WWDG Exported Functions
  * @{
  */

/** @defgroup WWDG_Exported_Functions_Group1 Initialization and Configuration functions
  *  @brief    Initialization and Configuration functions.
  *
@verbatim
  ==============================================================================
          ##### Initialization and Configuration functions #####
  ==============================================================================
  [..]
    This section provides functions allowing to:
      (+) Initialize and start the WWDG according to the specified parameters
          in the WWDG_InitTypeDef of associated handle.
      (+) Initialize the WWDG MSP.

@endverbatim
  * @{
  */

/**
  * @brief  Initialize the WWDG according to the specified.
  *         parameters in the WWDG_InitTypeDef of  associated handle.
  * @param  hwwdg  pointer to a WWDG_HandleTypeDef structure that contains
  *                the configuration information for the specified WWDG module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_WWDG_Init(WWDG_HandleTypeDef *hwwdg)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the WWDG handle allocation */
  if (hwwdg == NULL)                                    /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the parameters */
  assert_param(IS_WWDG_ALL_INSTANCE(hwwdg->Instance));  /* 执行语句 */
  assert_param(IS_WWDG_PRESCALER(hwwdg->Init.Prescaler));  /* 执行语句 */
  assert_param(IS_WWDG_WINDOW(hwwdg->Init.Window));     /* 执行语句 */
  assert_param(IS_WWDG_COUNTER(hwwdg->Init.Counter));   /* 执行语句 */
  assert_param(IS_WWDG_EWI_MODE(hwwdg->Init.EWIMode));  /* 执行语句 */

#if (USE_HAL_WWDG_REGISTER_CALLBACKS == 1)              /* 条件编译 */
  /* Reset Callback pointers */
  if (hwwdg->EwiCallback == NULL)                       /* 条件判断 */
  {                                                     /* 代码块开始 */
    hwwdg->EwiCallback = HAL_WWDG_EarlyWakeupCallback;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if (hwwdg->MspInitCallback == NULL)                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    hwwdg->MspInitCallback = HAL_WWDG_MspInit;          /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Init the low level hardware */
  hwwdg->MspInitCallback(hwwdg);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* Init the low level hardware */
  HAL_WWDG_MspInit(hwwdg);                              /* HAL库函数调用 */
#endif /* USE_HAL_WWDG_REGISTER_CALLBACKS */

  /* Set WWDG Counter */
  WRITE_REG(hwwdg->Instance->CR, (WWDG_CR_WDGA | hwwdg->Init.Counter));  /* 执行语句 */

  /* Set WWDG Prescaler and Window */
  WRITE_REG(hwwdg->Instance->CFR, (hwwdg->Init.EWIMode | hwwdg->Init.Prescaler | hwwdg->Init.Window));  /* 执行语句 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */


/**
  * @brief  Initialize the WWDG MSP.
  * @param  hwwdg  pointer to a WWDG_HandleTypeDef structure that contains
  *                the configuration information for the specified WWDG module.
  * @note   When rewriting this function in user file, mechanism may be added
  *         to avoid multiple initialize when HAL_WWDG_Init function is called
  *         again to change parameters.
  * @retval None
  */
__weak void HAL_WWDG_MspInit(WWDG_HandleTypeDef *hwwdg)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hwwdg);                                        /* 执行语句 */

  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_WWDG_MspInit could be implemented in the user file
   */
}                                                       /* 代码块结束 */


#if (USE_HAL_WWDG_REGISTER_CALLBACKS == 1)              /* 条件编译 */
/**
  * @brief  Register a User WWDG Callback
  *         To be used instead of the weak (surcharged) predefined callback
  * @param  hwwdg WWDG handle
  * @param  CallbackID ID of the callback to be registered
  *         This parameter can be one of the following values:
  *           @arg @ref HAL_WWDG_EWI_CB_ID Early WakeUp Interrupt Callback ID
  *           @arg @ref HAL_WWDG_MSPINIT_CB_ID MspInit callback ID
  * @param  pCallback pointer to the Callback function
  * @retval status
  */
HAL_StatusTypeDef HAL_WWDG_RegisterCallback(WWDG_HandleTypeDef *hwwdg, HAL_WWDG_CallbackIDTypeDef CallbackID,  /* HAL状态类型函数 */
                                            pWWDG_CallbackTypeDef pCallback)  /* 源代码 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  if (pCallback == NULL)                                /* 条件判断 */
  {                                                     /* 代码块开始 */
    status = HAL_ERROR;                                 /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_WWDG_EWI_CB_ID:                          /* case标签 */
        hwwdg->EwiCallback = pCallback;                 /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_WWDG_MSPINIT_CB_ID:                      /* case标签 */
        hwwdg->MspInitCallback = pCallback;             /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default:                                          /* default分支 */
        status = HAL_ERROR;                             /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */


/**
  * @brief  Unregister a WWDG Callback
  *         WWDG Callback is redirected to the weak (surcharged) predefined callback
  * @param  hwwdg WWDG handle
  * @param  CallbackID ID of the callback to be registered
  *         This parameter can be one of the following values:
  *           @arg @ref HAL_WWDG_EWI_CB_ID Early WakeUp Interrupt Callback ID
  *           @arg @ref HAL_WWDG_MSPINIT_CB_ID MspInit callback ID
  * @retval status
  */
HAL_StatusTypeDef HAL_WWDG_UnRegisterCallback(WWDG_HandleTypeDef *hwwdg, HAL_WWDG_CallbackIDTypeDef CallbackID)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  switch (CallbackID)                                   /* switch分支 */
  {                                                     /* 代码块开始 */
    case HAL_WWDG_EWI_CB_ID:                            /* case标签 */
      hwwdg->EwiCallback = HAL_WWDG_EarlyWakeupCallback;  /* 赋值或初始化 */
      break;                                            /* 跳出 */

    case HAL_WWDG_MSPINIT_CB_ID:                        /* case标签 */
      hwwdg->MspInitCallback = HAL_WWDG_MspInit;        /* 赋值或初始化 */
      break;                                            /* 跳出 */

    default:                                            /* default分支 */
      status = HAL_ERROR;                               /* 赋值或初始化 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* USE_HAL_WWDG_REGISTER_CALLBACKS */

/**
  * @}
  */

/** @defgroup WWDG_Exported_Functions_Group2 IO operation functions
  *  @brief    IO operation functions
  *
@verbatim
  ==============================================================================
                      ##### IO operation functions #####
  ==============================================================================
  [..]
    This section provides functions allowing to:
    (+) Refresh the WWDG.
    (+) Handle WWDG interrupt request and associated function callback.

@endverbatim
  * @{
  */

/**
  * @brief  Refresh the WWDG.
  * @param  hwwdg  pointer to a WWDG_HandleTypeDef structure that contains
  *                the configuration information for the specified WWDG module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_WWDG_Refresh(WWDG_HandleTypeDef *hwwdg)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Write to WWDG CR the WWDG Counter value to refresh with */
  WRITE_REG(hwwdg->Instance->CR, (hwwdg->Init.Counter));  /* 执行语句 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Handle WWDG interrupt request.
  * @note   The Early Wakeup Interrupt (EWI) can be used if specific safety operations
  *         or data logging must be performed before the actual reset is generated.
  *         The EWI interrupt is enabled by calling HAL_WWDG_Init function with
  *         EWIMode set to WWDG_EWI_ENABLE.
  *         When the downcounter reaches the value 0x40, and EWI interrupt is
  *         generated and the corresponding Interrupt Service Routine (ISR) can
  *         be used to trigger specific actions (such as communications or data
  *         logging), before resetting the device.
  * @param  hwwdg  pointer to a WWDG_HandleTypeDef structure that contains
  *                the configuration information for the specified WWDG module.
  * @retval None
  */
void HAL_WWDG_IRQHandler(WWDG_HandleTypeDef *hwwdg)     /* 中断服务函数 */
{                                                       /* 代码块开始 */
  /* Check if Early Wakeup Interrupt is enable */
  if (__HAL_WWDG_GET_IT_SOURCE(hwwdg, WWDG_IT_EWI) != RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check if WWDG Early Wakeup Interrupt occurred */
    if (__HAL_WWDG_GET_FLAG(hwwdg, WWDG_FLAG_EWIF) != RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear the WWDG Early Wakeup flag */
      __HAL_WWDG_CLEAR_FLAG(hwwdg, WWDG_FLAG_EWIF);     /* HAL辅助宏/函数 */

#if (USE_HAL_WWDG_REGISTER_CALLBACKS == 1)              /* 条件编译 */
      /* Early Wakeup registered callback */
      hwwdg->EwiCallback(hwwdg);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
      /* Early Wakeup callback */
      HAL_WWDG_EarlyWakeupCallback(hwwdg);              /* HAL库函数调用 */
#endif /* USE_HAL_WWDG_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */


/**
  * @brief  WWDG Early Wakeup callback.
  * @param  hwwdg  pointer to a WWDG_HandleTypeDef structure that contains
  *                the configuration information for the specified WWDG module.
  * @retval None
  */
__weak void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef *hwwdg)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hwwdg);                                        /* 执行语句 */

  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_WWDG_EarlyWakeupCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/**
  * @}
  */

#endif /* HAL_WWDG_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
