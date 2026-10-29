/* 文件: stm32f4xx_hal_ltdc.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_ltdc.c
  * @author  MCD Application Team
  * @brief   LTDC HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the LTDC peripheral:
  *           + Initialization and de-initialization functions
  *           + IO operation functions
  *           + Peripheral Control functions
  *           + Peripheral State and Errors functions
  *
  @verbatim
  ==============================================================================
                        ##### How to use this driver #####
  ==============================================================================
     [..]
     The LTDC HAL driver can be used as follows:

     (#) Declare a LTDC_HandleTypeDef handle structure, for example: LTDC_HandleTypeDef  hltdc;

     (#) Initialize the LTDC low level resources by implementing the HAL_LTDC_MspInit() API:
         (##) Enable the LTDC interface clock
         (##) NVIC configuration if you need to use interrupt process
             (+++) Configure the LTDC interrupt priority
             (+++) Enable the NVIC LTDC IRQ Channel

     (#) Initialize the required configuration through the following parameters:
         the LTDC timing, the horizontal and vertical polarity, the pixel clock polarity,
         Data Enable polarity and the LTDC background color value using HAL_LTDC_Init() function

     *** Configuration ***
     =========================
     [..]
     (#) Program the required configuration through the following parameters:
         the pixel format, the blending factors, input alpha value, the window size
         and the image size using HAL_LTDC_ConfigLayer() function for foreground
         or/and background layer.

     (#) Optionally, configure and enable the CLUT using HAL_LTDC_ConfigCLUT() and
         HAL_LTDC_EnableCLUT functions.

     (#) Optionally, enable the Dither using HAL_LTDC_EnableDither().

     (#) Optionally, configure and enable the Color keying using HAL_LTDC_ConfigColorKeying()
         and HAL_LTDC_EnableColorKeying functions.

     (#) Optionally, configure LineInterrupt using HAL_LTDC_ProgramLineEvent()
         function

     (#) If needed, reconfigure and change the pixel format value, the alpha value
         value, the window size, the window position and the layer start address
         for foreground or/and background layer using respectively the following
         functions: HAL_LTDC_SetPixelFormat(), HAL_LTDC_SetAlpha(), HAL_LTDC_SetWindowSize(),
         HAL_LTDC_SetWindowPosition() and HAL_LTDC_SetAddress().

     (#) Variant functions with _NoReload suffix allows to set the LTDC configuration/settings without immediate reload.
         This is useful in case when the program requires to modify serval LTDC settings (on one or both layers)
         then applying(reload) these settings in one shot by calling the function HAL_LTDC_Reload().

         After calling the _NoReload functions to set different color/format/layer settings,
         the program shall call the function HAL_LTDC_Reload() to apply(reload) these settings.
         Function HAL_LTDC_Reload() can be called with the parameter ReloadType set to LTDC_RELOAD_IMMEDIATE if
         an immediate reload is required.
         Function HAL_LTDC_Reload() can be called with the parameter ReloadType set to LTDC_RELOAD_VERTICAL_BLANKING if
         the reload should be done in the next vertical blanking period,
         this option allows to avoid display flicker by applying the new settings during the vertical blanking period.


     (#) To control LTDC state you can use the following function: HAL_LTDC_GetState()

     *** LTDC HAL driver macros list ***
     =============================================
     [..]
       Below the list of most used macros in LTDC HAL driver.

      (+) __HAL_LTDC_ENABLE: Enable the LTDC.
      (+) __HAL_LTDC_DISABLE: Disable the LTDC.
      (+) __HAL_LTDC_LAYER_ENABLE: Enable an LTDC Layer.
      (+) __HAL_LTDC_LAYER_DISABLE: Disable an LTDC Layer.
      (+) __HAL_LTDC_RELOAD_IMMEDIATE_CONFIG: Reload  Layer Configuration.
      (+) __HAL_LTDC_GET_FLAG: Get the LTDC pending flags.
      (+) __HAL_LTDC_CLEAR_FLAG: Clear the LTDC pending flags.
      (+) __HAL_LTDC_ENABLE_IT: Enable the specified LTDC interrupts.
      (+) __HAL_LTDC_DISABLE_IT: Disable the specified LTDC interrupts.
      (+) __HAL_LTDC_GET_IT_SOURCE: Check whether the specified LTDC interrupt has occurred or not.

     [..]
       (@) You can refer to the LTDC HAL driver header file for more useful macros


     *** Callback registration ***
     =============================================
     [..]
     The compilation define  USE_HAL_LTDC_REGISTER_CALLBACKS when set to 1
     allows the user to configure dynamically the driver callbacks.
     Use function HAL_LTDC_RegisterCallback() to register a callback.

    [..]
    Function HAL_LTDC_RegisterCallback() allows to register following callbacks:
      (+) LineEventCallback   : LTDC Line Event Callback.
      (+) ReloadEventCallback : LTDC Reload Event Callback.
      (+) ErrorCallback       : LTDC Error Callback
      (+) MspInitCallback     : LTDC MspInit.
      (+) MspDeInitCallback   : LTDC MspDeInit.
    [..]
    This function takes as parameters the HAL peripheral handle, the callback ID
    and a pointer to the user callback function.

    [..]
    Use function HAL_LTDC_UnRegisterCallback() to reset a callback to the default
    weak function.
    HAL_LTDC_UnRegisterCallback() takes as parameters the HAL peripheral handle
    and the callback ID.
    [..]
    This function allows to reset following callbacks:
      (+) LineEventCallback   : LTDC Line Event Callback
      (+) ReloadEventCallback : LTDC Reload Event Callback
      (+) ErrorCallback       : LTDC Error Callback
      (+) MspInitCallback     : LTDC MspInit
      (+) MspDeInitCallback   : LTDC MspDeInit.

    [..]
    By default, after the HAL_LTDC_Init and when the state is HAL_LTDC_STATE_RESET
    all callbacks are set to the corresponding weak functions:
    examples HAL_LTDC_LineEventCallback(), HAL_LTDC_ErrorCallback().
    Exception done for MspInit and MspDeInit functions that are
    reset to the legacy weak (surcharged) functions in the HAL_LTDC_Init() and HAL_LTDC_DeInit()
    only when these callbacks are null (not registered beforehand).
    If not, MspInit or MspDeInit are not null, the HAL_LTDC_Init() and HAL_LTDC_DeInit()
    keep and use the user MspInit/MspDeInit callbacks (registered beforehand).

    [..]
    Callbacks can be registered/unregistered in HAL_LTDC_STATE_READY state only.
    Exception done MspInit/MspDeInit that can be registered/unregistered
    in HAL_LTDC_STATE_READY or HAL_LTDC_STATE_RESET state,
    thus registered (user) MspInit/DeInit callbacks can be used during the Init/DeInit.
    In that case first register the MspInit/MspDeInit user callbacks
    using HAL_LTDC_RegisterCallback() before calling HAL_LTDC_DeInit()
    or HAL_LTDC_Init() function.

    [..]
    When the compilation define USE_HAL_LTDC_REGISTER_CALLBACKS is set to 0 or
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

#ifdef HAL_LTDC_MODULE_ENABLED                          /* 条件编译 */

#if defined (LTDC)                                      /* 条件编译 */

/** @defgroup LTDC LTDC
  * @brief LTDC HAL module driver
  * @{
  */


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void LTDC_SetConfig(LTDC_HandleTypeDef *hltdc, LTDC_LayerCfgTypeDef *pLayerCfg, uint32_t LayerIdx);  /* 静态函数 */
/* Private functions ---------------------------------------------------------*/

/** @defgroup LTDC_Exported_Functions LTDC Exported Functions
  * @{
  */

/** @defgroup LTDC_Exported_Functions_Group1 Initialization and Configuration functions
  *  @brief   Initialization and Configuration functions
  *
@verbatim
 ===============================================================================
                ##### Initialization and Configuration functions #####
 ===============================================================================
    [..]  This section provides functions allowing to:
      (+) Initialize and configure the LTDC
      (+) De-initialize the LTDC

@endverbatim
  * @{
  */

/**
  * @brief  Initialize the LTDC according to the specified parameters in the LTDC_InitTypeDef.
  * @param  hltdc  pointer to a LTDC_HandleTypeDef structure that contains
  *                the configuration information for the LTDC.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_LTDC_Init(LTDC_HandleTypeDef *hltdc)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmp, tmp1;                                   /* 32位无符号变量/函数 */

  /* Check the LTDC peripheral state */
  if (hltdc == NULL)                                    /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check function parameters */
  assert_param(IS_LTDC_ALL_INSTANCE(hltdc->Instance));  /* 执行语句 */
  assert_param(IS_LTDC_HSYNC(hltdc->Init.HorizontalSync));  /* 执行语句 */
  assert_param(IS_LTDC_VSYNC(hltdc->Init.VerticalSync));  /* 执行语句 */
  assert_param(IS_LTDC_AHBP(hltdc->Init.AccumulatedHBP));  /* 执行语句 */
  assert_param(IS_LTDC_AVBP(hltdc->Init.AccumulatedVBP));  /* 执行语句 */
  assert_param(IS_LTDC_AAH(hltdc->Init.AccumulatedActiveH));  /* 执行语句 */
  assert_param(IS_LTDC_AAW(hltdc->Init.AccumulatedActiveW));  /* 执行语句 */
  assert_param(IS_LTDC_TOTALH(hltdc->Init.TotalHeigh));  /* 执行语句 */
  assert_param(IS_LTDC_TOTALW(hltdc->Init.TotalWidth));  /* 执行语句 */
  assert_param(IS_LTDC_HSPOL(hltdc->Init.HSPolarity));  /* 执行语句 */
  assert_param(IS_LTDC_VSPOL(hltdc->Init.VSPolarity));  /* 执行语句 */
  assert_param(IS_LTDC_DEPOL(hltdc->Init.DEPolarity));  /* 执行语句 */
  assert_param(IS_LTDC_PCPOL(hltdc->Init.PCPolarity));  /* 执行语句 */

#if (USE_HAL_LTDC_REGISTER_CALLBACKS == 1)              /* 条件编译 */
  if (hltdc->State == HAL_LTDC_STATE_RESET)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Allocate lock resource and initialize it */
    hltdc->Lock = HAL_UNLOCKED;                         /* 赋值或初始化 */

    /* Reset the LTDC callback to the legacy weak callbacks */
    hltdc->LineEventCallback   = HAL_LTDC_LineEventCallback;    /* Legacy weak LineEventCallback    */
    hltdc->ReloadEventCallback = HAL_LTDC_ReloadEventCallback;  /* Legacy weak ReloadEventCallback  */
    hltdc->ErrorCallback       = HAL_LTDC_ErrorCallback;        /* Legacy weak ErrorCallback        */

    if (hltdc->MspInitCallback == NULL)                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      hltdc->MspInitCallback = HAL_LTDC_MspInit;        /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    /* Init the low level hardware */
    hltdc->MspInitCallback(hltdc);                      /* 执行语句 */
  }                                                     /* 代码块结束 */
#else                                                   /* 条件编译否则 */
  if (hltdc->State == HAL_LTDC_STATE_RESET)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Allocate lock resource and initialize it */
    hltdc->Lock = HAL_UNLOCKED;                         /* 赋值或初始化 */
    /* Init the low level hardware */
    HAL_LTDC_MspInit(hltdc);                            /* HAL库函数调用 */
  }                                                     /* 代码块结束 */
#endif /* USE_HAL_LTDC_REGISTER_CALLBACKS */

  /* Change LTDC peripheral state */
  hltdc->State = HAL_LTDC_STATE_BUSY;                   /* 赋值或初始化 */

  /* Configure the HS, VS, DE and PC polarity */
  hltdc->Instance->GCR &= ~(LTDC_GCR_HSPOL | LTDC_GCR_VSPOL | LTDC_GCR_DEPOL | LTDC_GCR_PCPOL);  /* 赋值或初始化 */
  hltdc->Instance->GCR |= (uint32_t)(hltdc->Init.HSPolarity | hltdc->Init.VSPolarity | \
                                     hltdc->Init.DEPolarity | hltdc->Init.PCPolarity);  /* 执行语句 */

  /* Set Synchronization size */
  hltdc->Instance->SSCR &= ~(LTDC_SSCR_VSH | LTDC_SSCR_HSW);  /* 赋值或初始化 */
  tmp = (hltdc->Init.HorizontalSync << 16U);            /* 赋值或初始化 */
  hltdc->Instance->SSCR |= (tmp | hltdc->Init.VerticalSync);  /* 赋值或初始化 */

  /* Set Accumulated Back porch */
  hltdc->Instance->BPCR &= ~(LTDC_BPCR_AVBP | LTDC_BPCR_AHBP);  /* 赋值或初始化 */
  tmp = (hltdc->Init.AccumulatedHBP << 16U);            /* 赋值或初始化 */
  hltdc->Instance->BPCR |= (tmp | hltdc->Init.AccumulatedVBP);  /* 赋值或初始化 */

  /* Set Accumulated Active Width */
  hltdc->Instance->AWCR &= ~(LTDC_AWCR_AAH | LTDC_AWCR_AAW);  /* 赋值或初始化 */
  tmp = (hltdc->Init.AccumulatedActiveW << 16U);        /* 赋值或初始化 */
  hltdc->Instance->AWCR |= (tmp | hltdc->Init.AccumulatedActiveH);  /* 赋值或初始化 */

  /* Set Total Width */
  hltdc->Instance->TWCR &= ~(LTDC_TWCR_TOTALH | LTDC_TWCR_TOTALW);  /* 赋值或初始化 */
  tmp = (hltdc->Init.TotalWidth << 16U);                /* 赋值或初始化 */
  hltdc->Instance->TWCR |= (tmp | hltdc->Init.TotalHeigh);  /* 赋值或初始化 */

  /* Set the background color value */
  tmp = ((uint32_t)(hltdc->Init.Backcolor.Green) << 8U);  /* 赋值或初始化 */
  tmp1 = ((uint32_t)(hltdc->Init.Backcolor.Red) << 16U);  /* 赋值或初始化 */
  hltdc->Instance->BCCR &= ~(LTDC_BCCR_BCBLUE | LTDC_BCCR_BCGREEN | LTDC_BCCR_BCRED);  /* 赋值或初始化 */
  hltdc->Instance->BCCR |= (tmp1 | tmp | hltdc->Init.Backcolor.Blue);  /* 赋值或初始化 */

  /* Enable the Transfer Error and FIFO underrun interrupts */
  __HAL_LTDC_ENABLE_IT(hltdc, LTDC_IT_TE | LTDC_IT_FU);  /* HAL辅助宏/函数 */

  /* Enable LTDC by setting LTDCEN bit */
  __HAL_LTDC_ENABLE(hltdc);                             /* HAL辅助宏/函数 */

  /* Initialize the error code */
  hltdc->ErrorCode = HAL_LTDC_ERROR_NONE;               /* 赋值或初始化 */

  /* Initialize the LTDC state*/
  hltdc->State = HAL_LTDC_STATE_READY;                  /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  De-initialize the LTDC peripheral.
  * @param  hltdc  pointer to a LTDC_HandleTypeDef structure that contains
  *                the configuration information for the LTDC.
  * @retval None
  */

HAL_StatusTypeDef HAL_LTDC_DeInit(LTDC_HandleTypeDef *hltdc)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
#if (USE_HAL_LTDC_REGISTER_CALLBACKS == 1)              /* 条件编译 */
  if (hltdc->MspDeInitCallback == NULL)                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    hltdc->MspDeInitCallback = HAL_LTDC_MspDeInit;      /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  /* DeInit the low level hardware */
  hltdc->MspDeInitCallback(hltdc);                      /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* DeInit the low level hardware */
  HAL_LTDC_MspDeInit(hltdc);                            /* HAL库函数调用 */
#endif /* USE_HAL_LTDC_REGISTER_CALLBACKS */

  /* Initialize the error code */
  hltdc->ErrorCode = HAL_LTDC_ERROR_NONE;               /* 赋值或初始化 */

  /* Initialize the LTDC state*/
  hltdc->State = HAL_LTDC_STATE_RESET;                  /* 赋值或初始化 */

  /* Release Lock */
  __HAL_UNLOCK(hltdc);                                  /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initialize the LTDC MSP.
  * @param  hltdc  pointer to a LTDC_HandleTypeDef structure that contains
  *                the configuration information for the LTDC.
  * @retval None
  */
__weak void HAL_LTDC_MspInit(LTDC_HandleTypeDef *hltdc)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hltdc);                                        /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_LTDC_MspInit could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  De-initialize the LTDC MSP.
  * @param  hltdc  pointer to a LTDC_HandleTypeDef structure that contains
  *                the configuration information for the LTDC.
  * @retval None
  */
__weak void HAL_LTDC_MspDeInit(LTDC_HandleTypeDef *hltdc)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hltdc);                                        /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_LTDC_MspDeInit could be implemented in the user file
   */
}                                                       /* 代码块结束 */

#if (USE_HAL_LTDC_REGISTER_CALLBACKS == 1)              /* 条件编译 */
/**
  * @brief  Register a User LTDC Callback
  *         To be used instead of the weak predefined callback
  * @param hltdc ltdc handle
  * @param CallbackID ID of the callback to be registered
  *        This parameter can be one of the following values:
  *          @arg @ref HAL_LTDC_LINE_EVENT_CB_ID Line Event Callback ID
  *          @arg @ref HAL_LTDC_RELOAD_EVENT_CB_ID Reload Event Callback ID
  *          @arg @ref HAL_LTDC_ERROR_CB_ID Error Callback ID
  *          @arg @ref HAL_LTDC_MSPINIT_CB_ID MspInit callback ID
  *          @arg @ref HAL_LTDC_MSPDEINIT_CB_ID MspDeInit callback ID
  * @param pCallback pointer to the Callback function
  * @retval status
  */
HAL_StatusTypeDef HAL_LTDC_RegisterCallback(LTDC_HandleTypeDef *hltdc, HAL_LTDC_CallbackIDTypeDef CallbackID, pLTDC_CallbackTypeDef pCallback)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  if (pCallback == NULL)                                /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hltdc->ErrorCode |= HAL_LTDC_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  /* Process locked */
  __HAL_LOCK(hltdc);                                    /* HAL辅助宏/函数 */

  if (hltdc->State == HAL_LTDC_STATE_READY)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_LTDC_LINE_EVENT_CB_ID :                  /* case标签 */
        hltdc->LineEventCallback = pCallback;           /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_LTDC_RELOAD_EVENT_CB_ID :                /* case标签 */
        hltdc->ReloadEventCallback = pCallback;         /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_LTDC_ERROR_CB_ID :                       /* case标签 */
        hltdc->ErrorCallback = pCallback;               /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_LTDC_MSPINIT_CB_ID :                     /* case标签 */
        hltdc->MspInitCallback = pCallback;             /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_LTDC_MSPDEINIT_CB_ID :                   /* case标签 */
        hltdc->MspDeInitCallback = pCallback;           /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hltdc->ErrorCode |= HAL_LTDC_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (hltdc->State == HAL_LTDC_STATE_RESET)        /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_LTDC_MSPINIT_CB_ID :                     /* case标签 */
        hltdc->MspInitCallback = pCallback;             /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_LTDC_MSPDEINIT_CB_ID :                   /* case标签 */
        hltdc->MspDeInitCallback = pCallback;           /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hltdc->ErrorCode |= HAL_LTDC_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hltdc->ErrorCode |= HAL_LTDC_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hltdc);                                  /* HAL辅助宏/函数 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Unregister an LTDC Callback
  *         LTDC callabck is redirected to the weak predefined callback
  * @param hltdc ltdc handle
  * @param CallbackID ID of the callback to be unregistered
  *        This parameter can be one of the following values:
  *          @arg @ref HAL_LTDC_LINE_EVENT_CB_ID Line Event Callback ID
  *          @arg @ref HAL_LTDC_RELOAD_EVENT_CB_ID Reload Event Callback ID
  *          @arg @ref HAL_LTDC_ERROR_CB_ID Error Callback ID
  *          @arg @ref HAL_LTDC_MSPINIT_CB_ID MspInit callback ID
  *          @arg @ref HAL_LTDC_MSPDEINIT_CB_ID MspDeInit callback ID
  * @retval status
  */
HAL_StatusTypeDef HAL_LTDC_UnRegisterCallback(LTDC_HandleTypeDef *hltdc, HAL_LTDC_CallbackIDTypeDef CallbackID)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Process locked */
  __HAL_LOCK(hltdc);                                    /* HAL辅助宏/函数 */

  if (hltdc->State == HAL_LTDC_STATE_READY)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_LTDC_LINE_EVENT_CB_ID :                  /* case标签 */
        hltdc->LineEventCallback = HAL_LTDC_LineEventCallback;      /* Legacy weak LineEventCallback    */
        break;                                          /* 跳出 */

      case HAL_LTDC_RELOAD_EVENT_CB_ID :                /* case标签 */
        hltdc->ReloadEventCallback = HAL_LTDC_ReloadEventCallback;  /* Legacy weak ReloadEventCallback  */
        break;                                          /* 跳出 */

      case HAL_LTDC_ERROR_CB_ID :                       /* case标签 */
        hltdc->ErrorCallback       = HAL_LTDC_ErrorCallback;        /* Legacy weak ErrorCallback        */
        break;                                          /* 跳出 */

      case HAL_LTDC_MSPINIT_CB_ID :                     /* case标签 */
        hltdc->MspInitCallback = HAL_LTDC_MspInit;                  /* Legcay weak MspInit Callback     */
        break;                                          /* 跳出 */

      case HAL_LTDC_MSPDEINIT_CB_ID :                   /* case标签 */
        hltdc->MspDeInitCallback = HAL_LTDC_MspDeInit;              /* Legcay weak MspDeInit Callback     */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hltdc->ErrorCode |= HAL_LTDC_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (hltdc->State == HAL_LTDC_STATE_RESET)        /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_LTDC_MSPINIT_CB_ID :                     /* case标签 */
        hltdc->MspInitCallback = HAL_LTDC_MspInit;                  /* Legcay weak MspInit Callback     */
        break;                                          /* 跳出 */

      case HAL_LTDC_MSPDEINIT_CB_ID :                   /* case标签 */
        hltdc->MspDeInitCallback = HAL_LTDC_MspDeInit;              /* Legcay weak MspDeInit Callback     */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hltdc->ErrorCode |= HAL_LTDC_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hltdc->ErrorCode |= HAL_LTDC_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hltdc);                                  /* HAL辅助宏/函数 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* USE_HAL_LTDC_REGISTER_CALLBACKS */

/**
  * @}
  */

/** @defgroup LTDC_Exported_Functions_Group2 IO operation functions
  *  @brief   IO operation functions
  *
@verbatim
 ===============================================================================
                      #####  IO operation functions  #####
 ===============================================================================
    [..]  This section provides function allowing to:
      (+) Handle LTDC interrupt request

@endverbatim
  * @{
  */
/**
  * @brief  Handle LTDC interrupt request.
  * @param  hltdc  pointer to a LTDC_HandleTypeDef structure that contains
  *                the configuration information for the LTDC.
  * @retval HAL status
  */
void HAL_LTDC_IRQHandler(LTDC_HandleTypeDef *hltdc)     /* 中断服务函数 */
{                                                       /* 代码块开始 */
  uint32_t isrflags  = READ_REG(hltdc->Instance->ISR);  /* 32位无符号变量/函数 */
  uint32_t itsources = READ_REG(hltdc->Instance->IER);  /* 32位无符号变量/函数 */

  /* Transfer Error Interrupt management ***************************************/
  if (((isrflags & LTDC_ISR_TERRIF) != 0U) && ((itsources & LTDC_IER_TERRIE) != 0U))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable the transfer Error interrupt */
    __HAL_LTDC_DISABLE_IT(hltdc, LTDC_IT_TE);           /* HAL辅助宏/函数 */

    /* Clear the transfer error flag */
    __HAL_LTDC_CLEAR_FLAG(hltdc, LTDC_FLAG_TE);         /* HAL辅助宏/函数 */

    /* Update error code */
    hltdc->ErrorCode |= HAL_LTDC_ERROR_TE;              /* 赋值或初始化 */

    /* Change LTDC state */
    hltdc->State = HAL_LTDC_STATE_ERROR;                /* 赋值或初始化 */

    /* Process unlocked */
    __HAL_UNLOCK(hltdc);                                /* HAL辅助宏/函数 */

    /* Transfer error Callback */
#if (USE_HAL_LTDC_REGISTER_CALLBACKS == 1)              /* 条件编译 */
    /*Call registered error callback*/
    hltdc->ErrorCallback(hltdc);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /* Call legacy error callback*/
    HAL_LTDC_ErrorCallback(hltdc);                      /* HAL库函数调用 */
#endif /* USE_HAL_LTDC_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */

  /* FIFO underrun Interrupt management ***************************************/
  if (((isrflags & LTDC_ISR_FUIF) != 0U) && ((itsources & LTDC_IER_FUIE) != 0U))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable the FIFO underrun interrupt */
    __HAL_LTDC_DISABLE_IT(hltdc, LTDC_IT_FU);           /* HAL辅助宏/函数 */

    /* Clear the FIFO underrun flag */
    __HAL_LTDC_CLEAR_FLAG(hltdc, LTDC_FLAG_FU);         /* HAL辅助宏/函数 */

    /* Update error code */
    hltdc->ErrorCode |= HAL_LTDC_ERROR_FU;              /* 赋值或初始化 */

    /* Change LTDC state */
    hltdc->State = HAL_LTDC_STATE_ERROR;                /* 赋值或初始化 */

    /* Process unlocked */
    __HAL_UNLOCK(hltdc);                                /* HAL辅助宏/函数 */

    /* Transfer error Callback */
#if (USE_HAL_LTDC_REGISTER_CALLBACKS == 1)              /* 条件编译 */
    /*Call registered error callback*/
    hltdc->ErrorCallback(hltdc);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /* Call legacy error callback*/
    HAL_LTDC_ErrorCallback(hltdc);                      /* HAL库函数调用 */
#endif /* USE_HAL_LTDC_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */

  /* Line Interrupt management ************************************************/
  if (((isrflags & LTDC_ISR_LIF) != 0U) && ((itsources & LTDC_IER_LIE) != 0U))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable the Line interrupt */
    __HAL_LTDC_DISABLE_IT(hltdc, LTDC_IT_LI);           /* HAL辅助宏/函数 */

    /* Clear the Line interrupt flag */
    __HAL_LTDC_CLEAR_FLAG(hltdc, LTDC_FLAG_LI);         /* HAL辅助宏/函数 */

    /* Change LTDC state */
    hltdc->State = HAL_LTDC_STATE_READY;                /* 赋值或初始化 */

    /* Process unlocked */
    __HAL_UNLOCK(hltdc);                                /* HAL辅助宏/函数 */

    /* Line interrupt Callback */
#if (USE_HAL_LTDC_REGISTER_CALLBACKS == 1)              /* 条件编译 */
    /*Call registered Line Event callback */
    hltdc->LineEventCallback(hltdc);                    /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /*Call Legacy Line Event callback */
    HAL_LTDC_LineEventCallback(hltdc);                  /* HAL库函数调用 */
#endif /* USE_HAL_LTDC_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */

  /* Register reload Interrupt management ***************************************/
  if (((isrflags & LTDC_ISR_RRIF) != 0U) && ((itsources & LTDC_IER_RRIE) != 0U))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable the register reload interrupt */
    __HAL_LTDC_DISABLE_IT(hltdc, LTDC_IT_RR);           /* HAL辅助宏/函数 */

    /* Clear the register reload flag */
    __HAL_LTDC_CLEAR_FLAG(hltdc, LTDC_FLAG_RR);         /* HAL辅助宏/函数 */

    /* Change LTDC state */
    hltdc->State = HAL_LTDC_STATE_READY;                /* 赋值或初始化 */

    /* Process unlocked */
    __HAL_UNLOCK(hltdc);                                /* HAL辅助宏/函数 */

    /* Reload interrupt Callback */
#if (USE_HAL_LTDC_REGISTER_CALLBACKS == 1)              /* 条件编译 */
    /*Call registered reload Event callback */
    hltdc->ReloadEventCallback(hltdc);                  /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /*Call Legacy Reload Event callback */
    HAL_LTDC_ReloadEventCallback(hltdc);                /* HAL库函数调用 */
#endif /* USE_HAL_LTDC_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Error LTDC callback.
  * @param  hltdc  pointer to a LTDC_HandleTypeDef structure that contains
  *                the configuration information for the LTDC.
  * @retval None
  */
__weak void HAL_LTDC_ErrorCallback(LTDC_HandleTypeDef *hltdc)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hltdc);                                        /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_LTDC_ErrorCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Line Event callback.
  * @param  hltdc  pointer to a LTDC_HandleTypeDef structure that contains
  *                the configuration information for the LTDC.
  * @retval None
  */
__weak void HAL_LTDC_LineEventCallback(LTDC_HandleTypeDef *hltdc)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hltdc);                                        /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_LTDC_LineEventCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Reload Event callback.
  * @param  hltdc  pointer to a LTDC_HandleTypeDef structure that contains
  *                the configuration information for the LTDC.
  * @retval None
  */
__weak void HAL_LTDC_ReloadEventCallback(LTDC_HandleTypeDef *hltdc)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hltdc);                                        /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_LTDC_ReloadEvenCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup LTDC_Exported_Functions_Group3 Peripheral Control functions
  *  @brief    Peripheral Control functions
  *
@verbatim
 ===============================================================================
                    ##### Peripheral Control functions #####
 ===============================================================================
    [..]  This section provides functions allowing to:
      (+) Configure the LTDC foreground or/and background parameters.
      (+) Set the active layer.
      (+) Configure the color keying.
      (+) Configure the C-LUT.
      (+) Enable / Disable the color keying.
      (+) Enable / Disable the C-LUT.
      (+) Update the layer position.
      (+) Update the layer size.
      (+) Update pixel format on the fly.
      (+) Update transparency on the fly.
      (+) Update address on the fly.

@endverbatim
  * @{
  */

/**
  * @brief  Configure the LTDC Layer according to the specified
  *         parameters in the LTDC_InitTypeDef and create the associated handle.
  * @param  hltdc      pointer to a LTDC_HandleTypeDef structure that contains
  *                    the configuration information for the LTDC.
  * @param  pLayerCfg  pointer to a LTDC_LayerCfgTypeDef structure that contains
  *                    the configuration information for the Layer.
  * @param  LayerIdx  LTDC Layer index.
  *                    This parameter can be one of the following values:
  *                    LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1)
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_LTDC_ConfigLayer(LTDC_HandleTypeDef *hltdc, LTDC_LayerCfgTypeDef *pLayerCfg, uint32_t LayerIdx)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_LTDC_LAYER(LayerIdx));                /* 执行语句 */
  assert_param(IS_LTDC_HCONFIGST(pLayerCfg->WindowX0));  /* 执行语句 */
  assert_param(IS_LTDC_HCONFIGSP(pLayerCfg->WindowX1));  /* 执行语句 */
  assert_param(IS_LTDC_VCONFIGST(pLayerCfg->WindowY0));  /* 执行语句 */
  assert_param(IS_LTDC_VCONFIGSP(pLayerCfg->WindowY1));  /* 执行语句 */
  assert_param(IS_LTDC_PIXEL_FORMAT(pLayerCfg->PixelFormat));  /* 执行语句 */
  assert_param(IS_LTDC_ALPHA(pLayerCfg->Alpha));        /* 执行语句 */
  assert_param(IS_LTDC_ALPHA(pLayerCfg->Alpha0));       /* 执行语句 */
  assert_param(IS_LTDC_BLENDING_FACTOR1(pLayerCfg->BlendingFactor1));  /* 执行语句 */
  assert_param(IS_LTDC_BLENDING_FACTOR2(pLayerCfg->BlendingFactor2));  /* 执行语句 */
  assert_param(IS_LTDC_CFBLL(pLayerCfg->ImageWidth));   /* 执行语句 */
  assert_param(IS_LTDC_CFBLNBR(pLayerCfg->ImageHeight));  /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hltdc);                                    /* HAL辅助宏/函数 */

  /* Change LTDC peripheral state */
  hltdc->State = HAL_LTDC_STATE_BUSY;                   /* 赋值或初始化 */

  /* Copy new layer configuration into handle structure */
  hltdc->LayerCfg[LayerIdx] = *pLayerCfg;               /* 赋值或初始化 */

  /* Configure the LTDC Layer */
  LTDC_SetConfig(hltdc, pLayerCfg, LayerIdx);           /* 执行语句 */

  /* Set the Immediate Reload type */
  hltdc->Instance->SRCR = LTDC_SRCR_IMR;                /* 赋值或初始化 */

  /* Initialize the LTDC state*/
  hltdc->State  = HAL_LTDC_STATE_READY;                 /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hltdc);                                  /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configure the color keying.
  * @param  hltdc     pointer to a LTDC_HandleTypeDef structure that contains
  *                   the configuration information for the LTDC.
  * @param  RGBValue  the color key value
  * @param  LayerIdx  LTDC Layer index.
  *                   This parameter can be one of the following values:
  *                   LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1)
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_LTDC_ConfigColorKeying(LTDC_HandleTypeDef *hltdc, uint32_t RGBValue, uint32_t LayerIdx)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_LTDC_LAYER(LayerIdx));                /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hltdc);                                    /* HAL辅助宏/函数 */

  /* Change LTDC peripheral state */
  hltdc->State = HAL_LTDC_STATE_BUSY;                   /* 赋值或初始化 */

  /* Configure the default color values */
  LTDC_LAYER(hltdc, LayerIdx)->CKCR &=  ~(LTDC_LxCKCR_CKBLUE | LTDC_LxCKCR_CKGREEN | LTDC_LxCKCR_CKRED);  /* 赋值或初始化 */
  LTDC_LAYER(hltdc, LayerIdx)->CKCR  = RGBValue;        /* 赋值或初始化 */

  /* Set the Immediate Reload type */
  hltdc->Instance->SRCR = LTDC_SRCR_IMR;                /* 赋值或初始化 */

  /* Change the LTDC state*/
  hltdc->State = HAL_LTDC_STATE_READY;                  /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hltdc);                                  /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Load the color lookup table.
  * @param  hltdc     pointer to a LTDC_HandleTypeDef structure that contains
  *                   the configuration information for the LTDC.
  * @param  pCLUT     pointer to the color lookup table address.
  * @param  CLUTSize  the color lookup table size.
  * @param  LayerIdx  LTDC Layer index.
  *                   This parameter can be one of the following values:
  *                   LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1)
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_LTDC_ConfigCLUT(LTDC_HandleTypeDef *hltdc, uint32_t *pCLUT, uint32_t CLUTSize, uint32_t LayerIdx)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmp;                                         /* 32位无符号变量/函数 */
  uint32_t counter;                                     /* 32位无符号变量/函数 */
  uint32_t *pcolorlut = pCLUT;                          /* 32位无符号变量/函数 */
  /* Check the parameters */
  assert_param(IS_LTDC_LAYER(LayerIdx));                /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hltdc);                                    /* HAL辅助宏/函数 */

  /* Change LTDC peripheral state */
  hltdc->State = HAL_LTDC_STATE_BUSY;                   /* 赋值或初始化 */

  for (counter = 0U; (counter < CLUTSize); counter++)   /* for循环 */
  {                                                     /* 代码块开始 */
    if (hltdc->LayerCfg[LayerIdx].PixelFormat == LTDC_PIXEL_FORMAT_AL44)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      tmp  = (((counter + (16U*counter)) << 24U) | ((uint32_t)(*pcolorlut) & 0xFFU) | ((uint32_t)(*pcolorlut) & 0xFF00U) | ((uint32_t)(*pcolorlut) & 0xFF0000U));  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      tmp  = ((counter << 24U) | ((uint32_t)(*pcolorlut) & 0xFFU) | ((uint32_t)(*pcolorlut) & 0xFF00U) | ((uint32_t)(*pcolorlut) & 0xFF0000U));  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    pcolorlut++;                                        /* 执行语句 */

    /* Specifies the C-LUT address and RGB value */
    LTDC_LAYER(hltdc, LayerIdx)->CLUTWR  = tmp;         /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Change the LTDC state*/
  hltdc->State = HAL_LTDC_STATE_READY;                  /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hltdc);                                  /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Enable the color keying.
  * @param  hltdc     pointer to a LTDC_HandleTypeDef structure that contains
  *                   the configuration information for the LTDC.
  * @param  LayerIdx  LTDC Layer index.
  *                   This parameter can be one of the following values:
  *                   LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1)
  * @retval  HAL status
  */
HAL_StatusTypeDef HAL_LTDC_EnableColorKeying(LTDC_HandleTypeDef *hltdc, uint32_t LayerIdx)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_LTDC_LAYER(LayerIdx));                /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hltdc);                                    /* HAL辅助宏/函数 */

  /* Change LTDC peripheral state */
  hltdc->State = HAL_LTDC_STATE_BUSY;                   /* 赋值或初始化 */

  /* Enable LTDC color keying by setting COLKEN bit */
  LTDC_LAYER(hltdc, LayerIdx)->CR |= (uint32_t)LTDC_LxCR_COLKEN;  /* 赋值或初始化 */

  /* Set the Immediate Reload type */
  hltdc->Instance->SRCR = LTDC_SRCR_IMR;                /* 赋值或初始化 */

  /* Change the LTDC state*/
  hltdc->State = HAL_LTDC_STATE_READY;                  /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hltdc);                                  /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Disable the color keying.
  * @param  hltdc     pointer to a LTDC_HandleTypeDef structure that contains
  *                   the configuration information for the LTDC.
  * @param  LayerIdx  LTDC Layer index.
  *                   This parameter can be one of the following values:
  *                   LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1)
  * @retval  HAL status
  */
HAL_StatusTypeDef HAL_LTDC_DisableColorKeying(LTDC_HandleTypeDef *hltdc, uint32_t LayerIdx)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_LTDC_LAYER(LayerIdx));                /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hltdc);                                    /* HAL辅助宏/函数 */

  /* Change LTDC peripheral state */
  hltdc->State = HAL_LTDC_STATE_BUSY;                   /* 赋值或初始化 */

  /* Disable LTDC color keying by setting COLKEN bit */
  LTDC_LAYER(hltdc, LayerIdx)->CR &= ~(uint32_t)LTDC_LxCR_COLKEN;  /* 赋值或初始化 */

  /* Set the Immediate Reload type */
  hltdc->Instance->SRCR = LTDC_SRCR_IMR;                /* 赋值或初始化 */

  /* Change the LTDC state*/
  hltdc->State = HAL_LTDC_STATE_READY;                  /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hltdc);                                  /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Enable the color lookup table.
  * @param  hltdc     pointer to a LTDC_HandleTypeDef structure that contains
  *                   the configuration information for the LTDC.
  * @param  LayerIdx  LTDC Layer index.
  *                   This parameter can be one of the following values:
  *                   LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1)
  * @retval  HAL status
  */
HAL_StatusTypeDef HAL_LTDC_EnableCLUT(LTDC_HandleTypeDef *hltdc, uint32_t LayerIdx)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_LTDC_LAYER(LayerIdx));                /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hltdc);                                    /* HAL辅助宏/函数 */

  /* Change LTDC peripheral state */
  hltdc->State = HAL_LTDC_STATE_BUSY;                   /* 赋值或初始化 */

  /* Enable LTDC color lookup table by setting CLUTEN bit */
  LTDC_LAYER(hltdc, LayerIdx)->CR |= (uint32_t)LTDC_LxCR_CLUTEN;  /* 赋值或初始化 */

  /* Set the Immediate Reload type */
  hltdc->Instance->SRCR = LTDC_SRCR_IMR;                /* 赋值或初始化 */

  /* Change the LTDC state*/
  hltdc->State = HAL_LTDC_STATE_READY;                  /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hltdc);                                  /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Disable the color lookup table.
  * @param  hltdc     pointer to a LTDC_HandleTypeDef structure that contains
  *                   the configuration information for the LTDC.
  * @param  LayerIdx  LTDC Layer index.
  *                   This parameter can be one of the following values:
  *                   LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1)
  * @retval  HAL status
  */
HAL_StatusTypeDef HAL_LTDC_DisableCLUT(LTDC_HandleTypeDef *hltdc, uint32_t LayerIdx)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_LTDC_LAYER(LayerIdx));                /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hltdc);                                    /* HAL辅助宏/函数 */

  /* Change LTDC peripheral state */
  hltdc->State = HAL_LTDC_STATE_BUSY;                   /* 赋值或初始化 */

  /* Disable LTDC color lookup table by setting CLUTEN bit */
  LTDC_LAYER(hltdc, LayerIdx)->CR &= ~(uint32_t)LTDC_LxCR_CLUTEN;  /* 赋值或初始化 */

  /* Set the Immediate Reload type */
  hltdc->Instance->SRCR = LTDC_SRCR_IMR;                /* 赋值或初始化 */

  /* Change the LTDC state*/
  hltdc->State = HAL_LTDC_STATE_READY;                  /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hltdc);                                  /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Enable Dither.
  * @param  hltdc  pointer to a LTDC_HandleTypeDef structure that contains
  *                the configuration information for the LTDC.
  * @retval  HAL status
  */

HAL_StatusTypeDef HAL_LTDC_EnableDither(LTDC_HandleTypeDef *hltdc)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Process locked */
  __HAL_LOCK(hltdc);                                    /* HAL辅助宏/函数 */

  /* Change LTDC peripheral state */
  hltdc->State = HAL_LTDC_STATE_BUSY;                   /* 赋值或初始化 */

  /* Enable Dither by setting DTEN bit */
  LTDC->GCR |= (uint32_t)LTDC_GCR_DEN;                  /* 赋值或初始化 */

  /* Change the LTDC state*/
  hltdc->State = HAL_LTDC_STATE_READY;                  /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hltdc);                                  /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Disable Dither.
  * @param  hltdc  pointer to a LTDC_HandleTypeDef structure that contains
  *                the configuration information for the LTDC.
  * @retval  HAL status
  */

HAL_StatusTypeDef HAL_LTDC_DisableDither(LTDC_HandleTypeDef *hltdc)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Process locked */
  __HAL_LOCK(hltdc);                                    /* HAL辅助宏/函数 */

  /* Change LTDC peripheral state */
  hltdc->State = HAL_LTDC_STATE_BUSY;                   /* 赋值或初始化 */

  /* Disable Dither by setting DTEN bit */
  LTDC->GCR &= ~(uint32_t)LTDC_GCR_DEN;                 /* 赋值或初始化 */

  /* Change the LTDC state*/
  hltdc->State = HAL_LTDC_STATE_READY;                  /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hltdc);                                  /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Set the LTDC window size.
  * @param  hltdc     pointer to a LTDC_HandleTypeDef structure that contains
  *                   the configuration information for the LTDC.
  * @param  XSize     LTDC Pixel per line
  * @param  YSize     LTDC Line number
  * @param  LayerIdx  LTDC Layer index.
  *                   This parameter can be one of the following values:
  *                   LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1)
  * @retval  HAL status
  */
HAL_StatusTypeDef HAL_LTDC_SetWindowSize(LTDC_HandleTypeDef *hltdc, uint32_t XSize, uint32_t YSize, uint32_t LayerIdx)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  LTDC_LayerCfgTypeDef *pLayerCfg;                      /* 执行语句 */

  /* Check the parameters (Layers parameters)*/
  assert_param(IS_LTDC_LAYER(LayerIdx));                /* 执行语句 */
  assert_param(IS_LTDC_CFBLL(XSize));                   /* 执行语句 */
  assert_param(IS_LTDC_CFBLNBR(YSize));                 /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hltdc);                                    /* HAL辅助宏/函数 */

  /* Change LTDC peripheral state */
  hltdc->State = HAL_LTDC_STATE_BUSY;                   /* 赋值或初始化 */

  /* Get layer configuration from handle structure */
  pLayerCfg = &hltdc->LayerCfg[LayerIdx];               /* 赋值或初始化 */

  /* update horizontal stop */
  pLayerCfg->WindowX1 = XSize + pLayerCfg->WindowX0;    /* 赋值或初始化 */

  /* update vertical stop */
  pLayerCfg->WindowY1 = YSize + pLayerCfg->WindowY0;    /* 赋值或初始化 */

  /* Reconfigures the color frame buffer pitch in byte */
  pLayerCfg->ImageWidth = XSize;                        /* 赋值或初始化 */

  /* Reconfigures the frame buffer line number */
  pLayerCfg->ImageHeight = YSize;                       /* 赋值或初始化 */

  /* Set LTDC parameters */
  LTDC_SetConfig(hltdc, pLayerCfg, LayerIdx);           /* 执行语句 */

  /* Set the Immediate Reload type */
  hltdc->Instance->SRCR = LTDC_SRCR_IMR;                /* 赋值或初始化 */

  /* Change the LTDC state*/
  hltdc->State = HAL_LTDC_STATE_READY;                  /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hltdc);                                  /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Set the LTDC window position.
  * @param  hltdc     pointer to a LTDC_HandleTypeDef structure that contains
  *                   the configuration information for the LTDC.
  * @param  X0        LTDC window X offset
  * @param  Y0        LTDC window Y offset
  * @param  LayerIdx  LTDC Layer index.
  *                         This parameter can be one of the following values:
  *                         LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1)
  * @retval  HAL status
  */
HAL_StatusTypeDef HAL_LTDC_SetWindowPosition(LTDC_HandleTypeDef *hltdc, uint32_t X0, uint32_t Y0, uint32_t LayerIdx)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  LTDC_LayerCfgTypeDef *pLayerCfg;                      /* 执行语句 */

  /* Check the parameters */
  assert_param(IS_LTDC_LAYER(LayerIdx));                /* 执行语句 */
  assert_param(IS_LTDC_CFBLL(X0));                      /* 执行语句 */
  assert_param(IS_LTDC_CFBLNBR(Y0));                    /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hltdc);                                    /* HAL辅助宏/函数 */

  /* Change LTDC peripheral state */
  hltdc->State = HAL_LTDC_STATE_BUSY;                   /* 赋值或初始化 */

  /* Get layer configuration from handle structure */
  pLayerCfg = &hltdc->LayerCfg[LayerIdx];               /* 赋值或初始化 */

  /* update horizontal start/stop */
  pLayerCfg->WindowX0 = X0;                             /* 赋值或初始化 */
  pLayerCfg->WindowX1 = X0 + pLayerCfg->ImageWidth;     /* 赋值或初始化 */

  /* update vertical start/stop */
  pLayerCfg->WindowY0 = Y0;                             /* 赋值或初始化 */
  pLayerCfg->WindowY1 = Y0 + pLayerCfg->ImageHeight;    /* 赋值或初始化 */

  /* Set LTDC parameters */
  LTDC_SetConfig(hltdc, pLayerCfg, LayerIdx);           /* 执行语句 */

  /* Set the Immediate Reload type */
  hltdc->Instance->SRCR = LTDC_SRCR_IMR;                /* 赋值或初始化 */

  /* Change the LTDC state*/
  hltdc->State = HAL_LTDC_STATE_READY;                  /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hltdc);                                  /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Reconfigure the pixel format.
  * @param  hltdc        pointer to a LTDC_HandleTypeDef structure that contains
  *                      the configuration information for the LTDC.
  * @param  Pixelformat  new pixel format value.
  * @param  LayerIdx     LTDC Layer index.
  *                      This parameter can be one of the following values:
  *                      LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1).
  * @retval  HAL status
  */
HAL_StatusTypeDef HAL_LTDC_SetPixelFormat(LTDC_HandleTypeDef *hltdc, uint32_t Pixelformat, uint32_t LayerIdx)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  LTDC_LayerCfgTypeDef *pLayerCfg;                      /* 执行语句 */

  /* Check the parameters */
  assert_param(IS_LTDC_PIXEL_FORMAT(Pixelformat));      /* 执行语句 */
  assert_param(IS_LTDC_LAYER(LayerIdx));                /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hltdc);                                    /* HAL辅助宏/函数 */

  /* Change LTDC peripheral state */
  hltdc->State = HAL_LTDC_STATE_BUSY;                   /* 赋值或初始化 */

  /* Get layer configuration from handle structure */
  pLayerCfg = &hltdc->LayerCfg[LayerIdx];               /* 赋值或初始化 */

  /* Reconfigure the pixel format */
  pLayerCfg->PixelFormat = Pixelformat;                 /* 赋值或初始化 */

  /* Set LTDC parameters */
  LTDC_SetConfig(hltdc, pLayerCfg, LayerIdx);           /* 执行语句 */

  /* Set the Immediate Reload type */
  hltdc->Instance->SRCR = LTDC_SRCR_IMR;                /* 赋值或初始化 */

  /* Change the LTDC state*/
  hltdc->State = HAL_LTDC_STATE_READY;                  /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hltdc);                                  /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Reconfigure the layer alpha value.
  * @param  hltdc     pointer to a LTDC_HandleTypeDef structure that contains
  *                   the configuration information for the LTDC.
  * @param  Alpha     new alpha value.
  * @param  LayerIdx  LTDC Layer index.
  *                   This parameter can be one of the following values:
  *                   LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1)
  * @retval  HAL status
  */
HAL_StatusTypeDef HAL_LTDC_SetAlpha(LTDC_HandleTypeDef *hltdc, uint32_t Alpha, uint32_t LayerIdx)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  LTDC_LayerCfgTypeDef *pLayerCfg;                      /* 执行语句 */

  /* Check the parameters */
  assert_param(IS_LTDC_ALPHA(Alpha));                   /* 执行语句 */
  assert_param(IS_LTDC_LAYER(LayerIdx));                /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hltdc);                                    /* HAL辅助宏/函数 */

  /* Change LTDC peripheral state */
  hltdc->State = HAL_LTDC_STATE_BUSY;                   /* 赋值或初始化 */

  /* Get layer configuration from handle structure */
  pLayerCfg = &hltdc->LayerCfg[LayerIdx];               /* 赋值或初始化 */

  /* Reconfigure the Alpha value */
  pLayerCfg->Alpha = Alpha;                             /* 赋值或初始化 */

  /* Set LTDC parameters */
  LTDC_SetConfig(hltdc, pLayerCfg, LayerIdx);           /* 执行语句 */

  /* Set the Immediate Reload type */
  hltdc->Instance->SRCR = LTDC_SRCR_IMR;                /* 赋值或初始化 */

  /* Change the LTDC state*/
  hltdc->State = HAL_LTDC_STATE_READY;                  /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hltdc);                                  /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */
/**
  * @brief  Reconfigure the frame buffer Address.
  * @param  hltdc     pointer to a LTDC_HandleTypeDef structure that contains
  *                   the configuration information for the LTDC.
  * @param  Address   new address value.
  * @param  LayerIdx  LTDC Layer index.
  *                   This parameter can be one of the following values:
  *                   LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1).
  * @retval  HAL status
  */
HAL_StatusTypeDef HAL_LTDC_SetAddress(LTDC_HandleTypeDef *hltdc, uint32_t Address, uint32_t LayerIdx)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  LTDC_LayerCfgTypeDef *pLayerCfg;                      /* 执行语句 */

  /* Check the parameters */
  assert_param(IS_LTDC_LAYER(LayerIdx));                /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hltdc);                                    /* HAL辅助宏/函数 */

  /* Change LTDC peripheral state */
  hltdc->State = HAL_LTDC_STATE_BUSY;                   /* 赋值或初始化 */

  /* Get layer configuration from handle structure */
  pLayerCfg = &hltdc->LayerCfg[LayerIdx];               /* 赋值或初始化 */

  /* Reconfigure the Address */
  pLayerCfg->FBStartAdress = Address;                   /* 赋值或初始化 */

  /* Set LTDC parameters */
  LTDC_SetConfig(hltdc, pLayerCfg, LayerIdx);           /* 执行语句 */

  /* Set the Immediate Reload type */
  hltdc->Instance->SRCR = LTDC_SRCR_IMR;                /* 赋值或初始化 */

  /* Change the LTDC state*/
  hltdc->State = HAL_LTDC_STATE_READY;                  /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hltdc);                                  /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Function used to reconfigure the pitch for specific cases where the attached LayerIdx buffer have a width that is
  *         larger than the one intended to be displayed on screen. Example of a buffer 800x480 attached to layer for which we
  *         want to read and display on screen only a portion 320x240 taken in the center of the buffer. The pitch in pixels
  *         will be in that case 800 pixels and not 320 pixels as initially configured by previous call to HAL_LTDC_ConfigLayer().
  * @note   This function should be called only after a previous call to HAL_LTDC_ConfigLayer() to modify the default pitch
  *         configured by HAL_LTDC_ConfigLayer() when required (refer to example described just above).
  * @param  hltdc              pointer to a LTDC_HandleTypeDef structure that contains
  *                            the configuration information for the LTDC.
  * @param  LinePitchInPixels  New line pitch in pixels to configure for LTDC layer 'LayerIdx'.
  * @param  LayerIdx           LTDC layer index concerned by the modification of line pitch.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_LTDC_SetPitch(LTDC_HandleTypeDef *hltdc, uint32_t LinePitchInPixels, uint32_t LayerIdx)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmp;                                         /* 32位无符号变量/函数 */
  uint32_t pitchUpdate;                                 /* 32位无符号变量/函数 */
  uint32_t pixelFormat;                                 /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_LTDC_LAYER(LayerIdx));                /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hltdc);                                    /* HAL辅助宏/函数 */

  /* Change LTDC peripheral state */
  hltdc->State = HAL_LTDC_STATE_BUSY;                   /* 赋值或初始化 */

  /* get LayerIdx used pixel format */
  pixelFormat = hltdc->LayerCfg[LayerIdx].PixelFormat;  /* 赋值或初始化 */

  if (pixelFormat == LTDC_PIXEL_FORMAT_ARGB8888)        /* 条件判断 */
  {                                                     /* 代码块开始 */
    tmp = 4U;                                           /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if (pixelFormat == LTDC_PIXEL_FORMAT_RGB888)     /* 否则如果 */
  {                                                     /* 代码块开始 */
    tmp = 3U;                                           /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if ((pixelFormat == LTDC_PIXEL_FORMAT_ARGB4444) || \
           (pixelFormat == LTDC_PIXEL_FORMAT_RGB565)   || \
           (pixelFormat == LTDC_PIXEL_FORMAT_ARGB1555) || \
           (pixelFormat == LTDC_PIXEL_FORMAT_AL88))     /* 源代码 */
  {                                                     /* 代码块开始 */
    tmp = 2U;                                           /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    tmp = 1U;                                           /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  pitchUpdate = ((LinePitchInPixels * tmp) << 16U);     /* 赋值或初始化 */

  /* Clear previously set standard pitch */
  LTDC_LAYER(hltdc, LayerIdx)->CFBLR &= ~LTDC_LxCFBLR_CFBP;  /* 赋值或初始化 */

  /* Set the Reload type as immediate update of LTDC pitch configured above */
  LTDC->SRCR |= LTDC_SRCR_IMR;                          /* 赋值或初始化 */

  /* Set new line pitch value */
  LTDC_LAYER(hltdc, LayerIdx)->CFBLR |= pitchUpdate;    /* 赋值或初始化 */

  /* Set the Reload type as immediate update of LTDC pitch configured above */
  LTDC->SRCR |= LTDC_SRCR_IMR;                          /* 赋值或初始化 */

  /* Change the LTDC state*/
  hltdc->State = HAL_LTDC_STATE_READY;                  /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hltdc);                                  /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Define the position of the line interrupt.
  * @param  hltdc   pointer to a LTDC_HandleTypeDef structure that contains
  *                 the configuration information for the LTDC.
  * @param  Line    Line Interrupt Position.
  * @note   User application may resort to HAL_LTDC_LineEventCallback() at line interrupt generation.
  * @retval  HAL status
  */
HAL_StatusTypeDef HAL_LTDC_ProgramLineEvent(LTDC_HandleTypeDef *hltdc, uint32_t Line)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_LTDC_LIPOS(Line));                    /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hltdc);                                    /* HAL辅助宏/函数 */

  /* Change LTDC peripheral state */
  hltdc->State = HAL_LTDC_STATE_BUSY;                   /* 赋值或初始化 */

  /* Disable the Line interrupt */
  __HAL_LTDC_DISABLE_IT(hltdc, LTDC_IT_LI);             /* HAL辅助宏/函数 */

  /* Set the Line Interrupt position */
  LTDC->LIPCR = (uint32_t)Line;                         /* 赋值或初始化 */

  /* Enable the Line interrupt */
  __HAL_LTDC_ENABLE_IT(hltdc, LTDC_IT_LI);              /* HAL辅助宏/函数 */

  /* Change the LTDC state*/
  hltdc->State = HAL_LTDC_STATE_READY;                  /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hltdc);                                  /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Reload LTDC Layers configuration.
  * @param  hltdc      pointer to a LTDC_HandleTypeDef structure that contains
  *                    the configuration information for the LTDC.
  * @param  ReloadType This parameter can be one of the following values :
  *                      LTDC_RELOAD_IMMEDIATE : Immediate Reload
  *                      LTDC_RELOAD_VERTICAL_BLANKING  : Reload in the next Vertical Blanking
  * @note   User application may resort to HAL_LTDC_ReloadEventCallback() at reload interrupt generation.
  * @retval  HAL status
  */
HAL_StatusTypeDef  HAL_LTDC_Reload(LTDC_HandleTypeDef *hltdc, uint32_t ReloadType)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_LTDC_RELOAD(ReloadType));             /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hltdc);                                    /* HAL辅助宏/函数 */

  /* Change LTDC peripheral state */
  hltdc->State = HAL_LTDC_STATE_BUSY;                   /* 赋值或初始化 */

  /* Enable the Reload interrupt */
  __HAL_LTDC_ENABLE_IT(hltdc, LTDC_IT_RR);              /* HAL辅助宏/函数 */

  /* Apply Reload type */
  hltdc->Instance->SRCR = ReloadType;                   /* 赋值或初始化 */

  /* Change the LTDC state*/
  hltdc->State = HAL_LTDC_STATE_READY;                  /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hltdc);                                  /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configure the LTDC Layer according to the specified without reloading
  *         parameters in the LTDC_InitTypeDef and create the associated handle.
  *         Variant of the function HAL_LTDC_ConfigLayer without immediate reload.
  * @param  hltdc      pointer to a LTDC_HandleTypeDef structure that contains
  *                    the configuration information for the LTDC.
  * @param  pLayerCfg  pointer to a LTDC_LayerCfgTypeDef structure that contains
  *                    the configuration information for the Layer.
  * @param  LayerIdx   LTDC Layer index.
  *                    This parameter can be one of the following values:
  *                    LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1)
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_LTDC_ConfigLayer_NoReload(LTDC_HandleTypeDef *hltdc, LTDC_LayerCfgTypeDef *pLayerCfg, uint32_t LayerIdx)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_LTDC_LAYER(LayerIdx));                /* 执行语句 */
  assert_param(IS_LTDC_HCONFIGST(pLayerCfg->WindowX0));  /* 执行语句 */
  assert_param(IS_LTDC_HCONFIGSP(pLayerCfg->WindowX1));  /* 执行语句 */
  assert_param(IS_LTDC_VCONFIGST(pLayerCfg->WindowY0));  /* 执行语句 */
  assert_param(IS_LTDC_VCONFIGSP(pLayerCfg->WindowY1));  /* 执行语句 */
  assert_param(IS_LTDC_PIXEL_FORMAT(pLayerCfg->PixelFormat));  /* 执行语句 */
  assert_param(IS_LTDC_ALPHA(pLayerCfg->Alpha));        /* 执行语句 */
  assert_param(IS_LTDC_ALPHA(pLayerCfg->Alpha0));       /* 执行语句 */
  assert_param(IS_LTDC_BLENDING_FACTOR1(pLayerCfg->BlendingFactor1));  /* 执行语句 */
  assert_param(IS_LTDC_BLENDING_FACTOR2(pLayerCfg->BlendingFactor2));  /* 执行语句 */
  assert_param(IS_LTDC_CFBLL(pLayerCfg->ImageWidth));   /* 执行语句 */
  assert_param(IS_LTDC_CFBLNBR(pLayerCfg->ImageHeight));  /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hltdc);                                    /* HAL辅助宏/函数 */

  /* Change LTDC peripheral state */
  hltdc->State = HAL_LTDC_STATE_BUSY;                   /* 赋值或初始化 */

  /* Copy new layer configuration into handle structure */
  hltdc->LayerCfg[LayerIdx] = *pLayerCfg;               /* 赋值或初始化 */

  /* Configure the LTDC Layer */
  LTDC_SetConfig(hltdc, pLayerCfg, LayerIdx);           /* 执行语句 */

  /* Initialize the LTDC state*/
  hltdc->State  = HAL_LTDC_STATE_READY;                 /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hltdc);                                  /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Set the LTDC window size without reloading.
  *         Variant of the function HAL_LTDC_SetWindowSize without immediate reload.
  * @param  hltdc     pointer to a LTDC_HandleTypeDef structure that contains
  *                   the configuration information for the LTDC.
  * @param  XSize     LTDC Pixel per line
  * @param  YSize     LTDC Line number
  * @param  LayerIdx  LTDC Layer index.
  *                   This parameter can be one of the following values:
  *                   LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1)
  * @retval  HAL status
  */
HAL_StatusTypeDef HAL_LTDC_SetWindowSize_NoReload(LTDC_HandleTypeDef *hltdc, uint32_t XSize, uint32_t YSize, uint32_t LayerIdx)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  LTDC_LayerCfgTypeDef *pLayerCfg;                      /* 执行语句 */

  /* Check the parameters (Layers parameters)*/
  assert_param(IS_LTDC_LAYER(LayerIdx));                /* 执行语句 */
  assert_param(IS_LTDC_CFBLL(XSize));                   /* 执行语句 */
  assert_param(IS_LTDC_CFBLNBR(YSize));                 /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hltdc);                                    /* HAL辅助宏/函数 */

  /* Change LTDC peripheral state */
  hltdc->State = HAL_LTDC_STATE_BUSY;                   /* 赋值或初始化 */

  /* Get layer configuration from handle structure */
  pLayerCfg = &hltdc->LayerCfg[LayerIdx];               /* 赋值或初始化 */

  /* update horizontal stop */
  pLayerCfg->WindowX1 = XSize + pLayerCfg->WindowX0;    /* 赋值或初始化 */

  /* update vertical stop */
  pLayerCfg->WindowY1 = YSize + pLayerCfg->WindowY0;    /* 赋值或初始化 */

  /* Reconfigures the color frame buffer pitch in byte */
  pLayerCfg->ImageWidth = XSize;                        /* 赋值或初始化 */

  /* Reconfigures the frame buffer line number */
  pLayerCfg->ImageHeight = YSize;                       /* 赋值或初始化 */

  /* Set LTDC parameters */
  LTDC_SetConfig(hltdc, pLayerCfg, LayerIdx);           /* 执行语句 */

  /* Change the LTDC state*/
  hltdc->State = HAL_LTDC_STATE_READY;                  /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hltdc);                                  /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Set the LTDC window position without reloading.
  *         Variant of the function HAL_LTDC_SetWindowPosition without immediate reload.
  * @param  hltdc     pointer to a LTDC_HandleTypeDef structure that contains
  *                   the configuration information for the LTDC.
  * @param  X0        LTDC window X offset
  * @param  Y0        LTDC window Y offset
  * @param  LayerIdx  LTDC Layer index.
  *                         This parameter can be one of the following values:
  *                         LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1)
  * @retval  HAL status
  */
HAL_StatusTypeDef HAL_LTDC_SetWindowPosition_NoReload(LTDC_HandleTypeDef *hltdc, uint32_t X0, uint32_t Y0, uint32_t LayerIdx)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  LTDC_LayerCfgTypeDef *pLayerCfg;                      /* 执行语句 */

  /* Check the parameters */
  assert_param(IS_LTDC_LAYER(LayerIdx));                /* 执行语句 */
  assert_param(IS_LTDC_CFBLL(X0));                      /* 执行语句 */
  assert_param(IS_LTDC_CFBLNBR(Y0));                    /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hltdc);                                    /* HAL辅助宏/函数 */

  /* Change LTDC peripheral state */
  hltdc->State = HAL_LTDC_STATE_BUSY;                   /* 赋值或初始化 */

  /* Get layer configuration from handle structure */
  pLayerCfg = &hltdc->LayerCfg[LayerIdx];               /* 赋值或初始化 */

  /* update horizontal start/stop */
  pLayerCfg->WindowX0 = X0;                             /* 赋值或初始化 */
  pLayerCfg->WindowX1 = X0 + pLayerCfg->ImageWidth;     /* 赋值或初始化 */

  /* update vertical start/stop */
  pLayerCfg->WindowY0 = Y0;                             /* 赋值或初始化 */
  pLayerCfg->WindowY1 = Y0 + pLayerCfg->ImageHeight;    /* 赋值或初始化 */

  /* Set LTDC parameters */
  LTDC_SetConfig(hltdc, pLayerCfg, LayerIdx);           /* 执行语句 */

  /* Change the LTDC state*/
  hltdc->State = HAL_LTDC_STATE_READY;                  /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hltdc);                                  /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Reconfigure the pixel format without reloading.
  *         Variant of the function HAL_LTDC_SetPixelFormat without immediate reload.
  * @param  hltdc        pointer to a LTDC_HandleTypeDfef structure that contains
  *                      the configuration information for the LTDC.
  * @param  Pixelformat  new pixel format value.
  * @param  LayerIdx     LTDC Layer index.
  *                      This parameter can be one of the following values:
  *                      LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1).
  * @retval  HAL status
  */
HAL_StatusTypeDef HAL_LTDC_SetPixelFormat_NoReload(LTDC_HandleTypeDef *hltdc, uint32_t Pixelformat, uint32_t LayerIdx)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  LTDC_LayerCfgTypeDef *pLayerCfg;                      /* 执行语句 */

  /* Check the parameters */
  assert_param(IS_LTDC_PIXEL_FORMAT(Pixelformat));      /* 执行语句 */
  assert_param(IS_LTDC_LAYER(LayerIdx));                /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hltdc);                                    /* HAL辅助宏/函数 */

  /* Change LTDC peripheral state */
  hltdc->State = HAL_LTDC_STATE_BUSY;                   /* 赋值或初始化 */

  /* Get layer configuration from handle structure */
  pLayerCfg = &hltdc->LayerCfg[LayerIdx];               /* 赋值或初始化 */

  /* Reconfigure the pixel format */
  pLayerCfg->PixelFormat = Pixelformat;                 /* 赋值或初始化 */

  /* Set LTDC parameters */
  LTDC_SetConfig(hltdc, pLayerCfg, LayerIdx);           /* 执行语句 */

  /* Change the LTDC state*/
  hltdc->State = HAL_LTDC_STATE_READY;                  /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hltdc);                                  /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Reconfigure the layer alpha value without reloading.
  *         Variant of the function HAL_LTDC_SetAlpha without immediate reload.
  * @param  hltdc     pointer to a LTDC_HandleTypeDef structure that contains
  *                   the configuration information for the LTDC.
  * @param  Alpha     new alpha value.
  * @param  LayerIdx  LTDC Layer index.
  *                   This parameter can be one of the following values:
  *                   LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1)
  * @retval  HAL status
  */
HAL_StatusTypeDef HAL_LTDC_SetAlpha_NoReload(LTDC_HandleTypeDef *hltdc, uint32_t Alpha, uint32_t LayerIdx)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  LTDC_LayerCfgTypeDef *pLayerCfg;                      /* 执行语句 */

  /* Check the parameters */
  assert_param(IS_LTDC_ALPHA(Alpha));                   /* 执行语句 */
  assert_param(IS_LTDC_LAYER(LayerIdx));                /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hltdc);                                    /* HAL辅助宏/函数 */

  /* Change LTDC peripheral state */
  hltdc->State = HAL_LTDC_STATE_BUSY;                   /* 赋值或初始化 */

  /* Get layer configuration from handle structure */
  pLayerCfg = &hltdc->LayerCfg[LayerIdx];               /* 赋值或初始化 */

  /* Reconfigure the Alpha value */
  pLayerCfg->Alpha = Alpha;                             /* 赋值或初始化 */

  /* Set LTDC parameters */
  LTDC_SetConfig(hltdc, pLayerCfg, LayerIdx);           /* 执行语句 */

  /* Change the LTDC state*/
  hltdc->State = HAL_LTDC_STATE_READY;                  /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hltdc);                                  /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Reconfigure the frame buffer Address without reloading.
  *         Variant of the function HAL_LTDC_SetAddress without immediate reload.
  * @param  hltdc     pointer to a LTDC_HandleTypeDef structure that contains
  *                   the configuration information for the LTDC.
  * @param  Address   new address value.
  * @param  LayerIdx  LTDC Layer index.
  *                   This parameter can be one of the following values:
  *                   LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1).
  * @retval  HAL status
  */
HAL_StatusTypeDef HAL_LTDC_SetAddress_NoReload(LTDC_HandleTypeDef *hltdc, uint32_t Address, uint32_t LayerIdx)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  LTDC_LayerCfgTypeDef *pLayerCfg;                      /* 执行语句 */

  /* Check the parameters */
  assert_param(IS_LTDC_LAYER(LayerIdx));                /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hltdc);                                    /* HAL辅助宏/函数 */

  /* Change LTDC peripheral state */
  hltdc->State = HAL_LTDC_STATE_BUSY;                   /* 赋值或初始化 */

  /* Get layer configuration from handle structure */
  pLayerCfg = &hltdc->LayerCfg[LayerIdx];               /* 赋值或初始化 */

  /* Reconfigure the Address */
  pLayerCfg->FBStartAdress = Address;                   /* 赋值或初始化 */

  /* Set LTDC parameters */
  LTDC_SetConfig(hltdc, pLayerCfg, LayerIdx);           /* 执行语句 */

  /* Change the LTDC state*/
  hltdc->State = HAL_LTDC_STATE_READY;                  /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hltdc);                                  /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Function used to reconfigure the pitch for specific cases where the attached LayerIdx buffer have a width that is
  *         larger than the one intended to be displayed on screen. Example of a buffer 800x480 attached to layer for which we
  *         want to read and display on screen only a portion 320x240 taken in the center of the buffer. The pitch in pixels
  *         will be in that case 800 pixels and not 320 pixels as initially configured by previous call to HAL_LTDC_ConfigLayer().
  * @note   This function should be called only after a previous call to HAL_LTDC_ConfigLayer() to modify the default pitch
  *         configured by HAL_LTDC_ConfigLayer() when required (refer to example described just above).
  *         Variant of the function HAL_LTDC_SetPitch without immediate reload.
  * @param  hltdc              pointer to a LTDC_HandleTypeDef structure that contains
  *                            the configuration information for the LTDC.
  * @param  LinePitchInPixels  New line pitch in pixels to configure for LTDC layer 'LayerIdx'.
  * @param  LayerIdx           LTDC layer index concerned by the modification of line pitch.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_LTDC_SetPitch_NoReload(LTDC_HandleTypeDef *hltdc, uint32_t LinePitchInPixels, uint32_t LayerIdx)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmp;                                         /* 32位无符号变量/函数 */
  uint32_t pitchUpdate;                                 /* 32位无符号变量/函数 */
  uint32_t pixelFormat;                                 /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_LTDC_LAYER(LayerIdx));                /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hltdc);                                    /* HAL辅助宏/函数 */

  /* Change LTDC peripheral state */
  hltdc->State = HAL_LTDC_STATE_BUSY;                   /* 赋值或初始化 */

  /* get LayerIdx used pixel format */
  pixelFormat = hltdc->LayerCfg[LayerIdx].PixelFormat;  /* 赋值或初始化 */

  if (pixelFormat == LTDC_PIXEL_FORMAT_ARGB8888)        /* 条件判断 */
  {                                                     /* 代码块开始 */
    tmp = 4U;                                           /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if (pixelFormat == LTDC_PIXEL_FORMAT_RGB888)     /* 否则如果 */
  {                                                     /* 代码块开始 */
    tmp = 3U;                                           /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if ((pixelFormat == LTDC_PIXEL_FORMAT_ARGB4444) || \
           (pixelFormat == LTDC_PIXEL_FORMAT_RGB565)   || \
           (pixelFormat == LTDC_PIXEL_FORMAT_ARGB1555) || \
           (pixelFormat == LTDC_PIXEL_FORMAT_AL88))     /* 源代码 */
  {                                                     /* 代码块开始 */
    tmp = 2U;                                           /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    tmp = 1U;                                           /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  pitchUpdate = ((LinePitchInPixels * tmp) << 16U);     /* 赋值或初始化 */

  /* Clear previously set standard pitch */
  LTDC_LAYER(hltdc, LayerIdx)->CFBLR &= ~LTDC_LxCFBLR_CFBP;  /* 赋值或初始化 */

  /* Set new line pitch value */
  LTDC_LAYER(hltdc, LayerIdx)->CFBLR |= pitchUpdate;    /* 赋值或初始化 */

  /* Change the LTDC state*/
  hltdc->State = HAL_LTDC_STATE_READY;                  /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hltdc);                                  /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */


/**
  * @brief  Configure the color keying without reloading.
  *         Variant of the function HAL_LTDC_ConfigColorKeying without immediate reload.
  * @param  hltdc     pointer to a LTDC_HandleTypeDef structure that contains
  *                   the configuration information for the LTDC.
  * @param  RGBValue the color key value
  * @param  LayerIdx  LTDC Layer index.
  *                   This parameter can be one of the following values:
  *                   LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1)
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_LTDC_ConfigColorKeying_NoReload(LTDC_HandleTypeDef *hltdc, uint32_t RGBValue, uint32_t LayerIdx)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_LTDC_LAYER(LayerIdx));                /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hltdc);                                    /* HAL辅助宏/函数 */

  /* Change LTDC peripheral state */
  hltdc->State = HAL_LTDC_STATE_BUSY;                   /* 赋值或初始化 */

  /* Configure the default color values */
  LTDC_LAYER(hltdc, LayerIdx)->CKCR &=  ~(LTDC_LxCKCR_CKBLUE | LTDC_LxCKCR_CKGREEN | LTDC_LxCKCR_CKRED);  /* 赋值或初始化 */
  LTDC_LAYER(hltdc, LayerIdx)->CKCR  = RGBValue;        /* 赋值或初始化 */

  /* Change the LTDC state*/
  hltdc->State = HAL_LTDC_STATE_READY;                  /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hltdc);                                  /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Enable the color keying without reloading.
  *         Variant of the function HAL_LTDC_EnableColorKeying without immediate reload.
  * @param  hltdc     pointer to a LTDC_HandleTypeDef structure that contains
  *                   the configuration information for the LTDC.
  * @param  LayerIdx  LTDC Layer index.
  *                   This parameter can be one of the following values:
  *                   LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1)
  * @retval  HAL status
  */
HAL_StatusTypeDef HAL_LTDC_EnableColorKeying_NoReload(LTDC_HandleTypeDef *hltdc, uint32_t LayerIdx)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_LTDC_LAYER(LayerIdx));                /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hltdc);                                    /* HAL辅助宏/函数 */

  /* Change LTDC peripheral state */
  hltdc->State = HAL_LTDC_STATE_BUSY;                   /* 赋值或初始化 */

  /* Enable LTDC color keying by setting COLKEN bit */
  LTDC_LAYER(hltdc, LayerIdx)->CR |= (uint32_t)LTDC_LxCR_COLKEN;  /* 赋值或初始化 */

  /* Change the LTDC state*/
  hltdc->State = HAL_LTDC_STATE_READY;                  /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hltdc);                                  /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Disable the color keying without reloading.
  *         Variant of the function HAL_LTDC_DisableColorKeying without immediate reload.
  * @param  hltdc     pointer to a LTDC_HandleTypeDef structure that contains
  *                   the configuration information for the LTDC.
  * @param  LayerIdx  LTDC Layer index.
  *                   This parameter can be one of the following values:
  *                   LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1)
  * @retval  HAL status
  */
HAL_StatusTypeDef HAL_LTDC_DisableColorKeying_NoReload(LTDC_HandleTypeDef *hltdc, uint32_t LayerIdx)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_LTDC_LAYER(LayerIdx));                /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hltdc);                                    /* HAL辅助宏/函数 */

  /* Change LTDC peripheral state */
  hltdc->State = HAL_LTDC_STATE_BUSY;                   /* 赋值或初始化 */

  /* Disable LTDC color keying by setting COLKEN bit */
  LTDC_LAYER(hltdc, LayerIdx)->CR &= ~(uint32_t)LTDC_LxCR_COLKEN;  /* 赋值或初始化 */

  /* Change the LTDC state*/
  hltdc->State = HAL_LTDC_STATE_READY;                  /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hltdc);                                  /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Enable the color lookup table without reloading.
  *         Variant of the function HAL_LTDC_EnableCLUT without immediate reload.
  * @param  hltdc     pointer to a LTDC_HandleTypeDef structure that contains
  *                   the configuration information for the LTDC.
  * @param  LayerIdx  LTDC Layer index.
  *                   This parameter can be one of the following values:
  *                   LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1)
  * @retval  HAL status
  */
HAL_StatusTypeDef HAL_LTDC_EnableCLUT_NoReload(LTDC_HandleTypeDef *hltdc, uint32_t LayerIdx)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_LTDC_LAYER(LayerIdx));                /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hltdc);                                    /* HAL辅助宏/函数 */

  /* Change LTDC peripheral state */
  hltdc->State = HAL_LTDC_STATE_BUSY;                   /* 赋值或初始化 */

  /* Disable LTDC color lookup table by setting CLUTEN bit */
  LTDC_LAYER(hltdc, LayerIdx)->CR |= (uint32_t)LTDC_LxCR_CLUTEN;  /* 赋值或初始化 */

  /* Change the LTDC state*/
  hltdc->State = HAL_LTDC_STATE_READY;                  /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hltdc);                                  /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Disable the color lookup table without reloading.
  *         Variant of the function HAL_LTDC_DisableCLUT without immediate reload.
  * @param  hltdc     pointer to a LTDC_HandleTypeDef structure that contains
  *                   the configuration information for the LTDC.
  * @param  LayerIdx  LTDC Layer index.
  *                   This parameter can be one of the following values:
  *                   LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1)
  * @retval  HAL status
  */
HAL_StatusTypeDef HAL_LTDC_DisableCLUT_NoReload(LTDC_HandleTypeDef *hltdc, uint32_t LayerIdx)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_LTDC_LAYER(LayerIdx));                /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hltdc);                                    /* HAL辅助宏/函数 */

  /* Change LTDC peripheral state */
  hltdc->State = HAL_LTDC_STATE_BUSY;                   /* 赋值或初始化 */

  /* Disable LTDC color lookup table by setting CLUTEN bit */
  LTDC_LAYER(hltdc, LayerIdx)->CR &= ~(uint32_t)LTDC_LxCR_CLUTEN;  /* 赋值或初始化 */

  /* Change the LTDC state*/
  hltdc->State = HAL_LTDC_STATE_READY;                  /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hltdc);                                  /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup LTDC_Exported_Functions_Group4 Peripheral State and Errors functions
  *  @brief    Peripheral State and Errors functions
  *
@verbatim
 ===============================================================================
                  ##### Peripheral State and Errors functions #####
 ===============================================================================
    [..]
    This subsection provides functions allowing to
      (+) Check the LTDC handle state.
      (+) Get the LTDC handle error code.

@endverbatim
  * @{
  */

/**
  * @brief  Return the LTDC handle state.
  * @param  hltdc  pointer to a LTDC_HandleTypeDef structure that contains
  *                the configuration information for the LTDC.
  * @retval HAL state
  */
HAL_LTDC_StateTypeDef HAL_LTDC_GetState(LTDC_HandleTypeDef *hltdc)  /* HAL库函数调用 */
{                                                       /* 代码块开始 */
  return hltdc->State;                                  /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return the LTDC handle error code.
  * @param  hltdc  pointer to a LTDC_HandleTypeDef structure that contains
  *               the configuration information for the LTDC.
  * @retval LTDC Error Code
  */
uint32_t HAL_LTDC_GetError(LTDC_HandleTypeDef *hltdc)   /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  return hltdc->ErrorCode;                              /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup LTDC_Private_Functions LTDC Private Functions
  * @{
  */

/**
  * @brief  Configure the LTDC peripheral
  * @param  hltdc     Pointer to a LTDC_HandleTypeDef structure that contains
  *                   the configuration information for the LTDC.
  * @param  pLayerCfg Pointer LTDC Layer Configuration structure
  * @param  LayerIdx  LTDC Layer index.
  *                   This parameter can be one of the following values: LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1)
  * @retval None
  */
static void LTDC_SetConfig(LTDC_HandleTypeDef *hltdc, LTDC_LayerCfgTypeDef *pLayerCfg, uint32_t LayerIdx)  /* 静态函数 */
{                                                       /* 代码块开始 */
  uint32_t tmp;                                         /* 32位无符号变量/函数 */
  uint32_t tmp1;                                        /* 32位无符号变量/函数 */
  uint32_t tmp2;                                        /* 32位无符号变量/函数 */

  /* Configure the horizontal start and stop position */
  tmp = ((pLayerCfg->WindowX1 + ((hltdc->Instance->BPCR & LTDC_BPCR_AHBP) >> 16U)) << 16U);  /* 赋值或初始化 */
  LTDC_LAYER(hltdc, LayerIdx)->WHPCR &= ~(LTDC_LxWHPCR_WHSTPOS | LTDC_LxWHPCR_WHSPPOS);  /* 赋值或初始化 */
  LTDC_LAYER(hltdc, LayerIdx)->WHPCR = ((pLayerCfg->WindowX0 + ((hltdc->Instance->BPCR & LTDC_BPCR_AHBP) >> 16U) + 1U) | tmp);  /* 赋值或初始化 */

  /* Configure the vertical start and stop position */
  tmp = ((pLayerCfg->WindowY1 + (hltdc->Instance->BPCR & LTDC_BPCR_AVBP)) << 16U);  /* 赋值或初始化 */
  LTDC_LAYER(hltdc, LayerIdx)->WVPCR &= ~(LTDC_LxWVPCR_WVSTPOS | LTDC_LxWVPCR_WVSPPOS);  /* 赋值或初始化 */
  LTDC_LAYER(hltdc, LayerIdx)->WVPCR  = ((pLayerCfg->WindowY0 + (hltdc->Instance->BPCR & LTDC_BPCR_AVBP) + 1U) | tmp);  /* 赋值或初始化 */

  /* Specifies the pixel format */
  LTDC_LAYER(hltdc, LayerIdx)->PFCR &= ~(LTDC_LxPFCR_PF);  /* 赋值或初始化 */
  LTDC_LAYER(hltdc, LayerIdx)->PFCR = (pLayerCfg->PixelFormat);  /* 赋值或初始化 */

  /* Configure the default color values */
  tmp = ((uint32_t)(pLayerCfg->Backcolor.Green) << 8U);  /* 赋值或初始化 */
  tmp1 = ((uint32_t)(pLayerCfg->Backcolor.Red) << 16U);  /* 赋值或初始化 */
  tmp2 = (pLayerCfg->Alpha0 << 24U);                    /* 赋值或初始化 */
  LTDC_LAYER(hltdc, LayerIdx)->DCCR &= ~(LTDC_LxDCCR_DCBLUE | LTDC_LxDCCR_DCGREEN | LTDC_LxDCCR_DCRED | LTDC_LxDCCR_DCALPHA);  /* 赋值或初始化 */
  LTDC_LAYER(hltdc, LayerIdx)->DCCR = (pLayerCfg->Backcolor.Blue | tmp | tmp1 | tmp2);  /* 赋值或初始化 */

  /* Specifies the constant alpha value */
  LTDC_LAYER(hltdc, LayerIdx)->CACR &= ~(LTDC_LxCACR_CONSTA);  /* 赋值或初始化 */
  LTDC_LAYER(hltdc, LayerIdx)->CACR = (pLayerCfg->Alpha);  /* 赋值或初始化 */

  /* Specifies the blending factors */
  LTDC_LAYER(hltdc, LayerIdx)->BFCR &= ~(LTDC_LxBFCR_BF2 | LTDC_LxBFCR_BF1);  /* 赋值或初始化 */
  LTDC_LAYER(hltdc, LayerIdx)->BFCR = (pLayerCfg->BlendingFactor1 | pLayerCfg->BlendingFactor2);  /* 赋值或初始化 */

  /* Configure the color frame buffer start address */
  LTDC_LAYER(hltdc, LayerIdx)->CFBAR &= ~(LTDC_LxCFBAR_CFBADD);  /* 赋值或初始化 */
  LTDC_LAYER(hltdc, LayerIdx)->CFBAR = (pLayerCfg->FBStartAdress);  /* 赋值或初始化 */

  if (pLayerCfg->PixelFormat == LTDC_PIXEL_FORMAT_ARGB8888)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    tmp = 4U;                                           /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if (pLayerCfg->PixelFormat == LTDC_PIXEL_FORMAT_RGB888)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    tmp = 3U;                                           /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if ((pLayerCfg->PixelFormat == LTDC_PIXEL_FORMAT_ARGB4444) || \
           (pLayerCfg->PixelFormat == LTDC_PIXEL_FORMAT_RGB565)   || \
           (pLayerCfg->PixelFormat == LTDC_PIXEL_FORMAT_ARGB1555) || \
           (pLayerCfg->PixelFormat == LTDC_PIXEL_FORMAT_AL88))  /* 源代码 */
  {                                                     /* 代码块开始 */
    tmp = 2U;                                           /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    tmp = 1U;                                           /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Configure the color frame buffer pitch in byte */
  LTDC_LAYER(hltdc, LayerIdx)->CFBLR  &= ~(LTDC_LxCFBLR_CFBLL | LTDC_LxCFBLR_CFBP);  /* 赋值或初始化 */
  LTDC_LAYER(hltdc, LayerIdx)->CFBLR  = (((pLayerCfg->ImageWidth * tmp) << 16U) | (((pLayerCfg->WindowX1 - pLayerCfg->WindowX0) * tmp)  + 3U));  /* 赋值或初始化 */
  /* Configure the frame buffer line number */
  LTDC_LAYER(hltdc, LayerIdx)->CFBLNR  &= ~(LTDC_LxCFBLNR_CFBLNBR);  /* 赋值或初始化 */
  LTDC_LAYER(hltdc, LayerIdx)->CFBLNR  = (pLayerCfg->ImageHeight);  /* 赋值或初始化 */

  /* Enable LTDC_Layer by setting LEN bit */
  LTDC_LAYER(hltdc, LayerIdx)->CR |= (uint32_t)LTDC_LxCR_LEN;  /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @}
  */


/**
  * @}
  */

#endif /* LTDC */

#endif /* HAL_LTDC_MODULE_ENABLED */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
