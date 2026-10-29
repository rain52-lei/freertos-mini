/* 文件: stm32f4xx_hal_dma2d.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_dma2d.c
  * @author  MCD Application Team
  * @brief   DMA2D HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the DMA2D peripheral:
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
      (#) Program the required configuration through the following parameters:
          the transfer mode, the output color mode and the output offset using
          HAL_DMA2D_Init() function.

      (#) Program the required configuration through the following parameters:
          the input color mode, the input color, the input alpha value, the alpha mode,
          the red/blue swap mode, the inverted alpha mode and the input offset using
          HAL_DMA2D_ConfigLayer() function for foreground or/and background layer.

     *** Polling mode IO operation ***
     =================================
    [..]
       (#) Configure pdata parameter (explained hereafter), destination and data length
           and enable the transfer using HAL_DMA2D_Start().
       (#) Wait for end of transfer using HAL_DMA2D_PollForTransfer(), at this stage
           user can specify the value of timeout according to his end application.

     *** Interrupt mode IO operation ***
     ===================================
     [..]
       (#) Configure pdata parameter, destination and data length and enable
           the transfer using HAL_DMA2D_Start_IT().
       (#) Use HAL_DMA2D_IRQHandler() called under DMA2D_IRQHandler() interrupt subroutine.
       (#) At the end of data transfer HAL_DMA2D_IRQHandler() function is executed and user can
           add his own function by customization of function pointer XferCpltCallback (member
           of DMA2D handle structure).
       (#) In case of error, the HAL_DMA2D_IRQHandler() function calls the callback
           XferErrorCallback.

         -@-   In Register-to-Memory transfer mode, pdata parameter is the register
               color, in Memory-to-memory or Memory-to-Memory with pixel format
               conversion pdata is the source address.

         -@-   Configure the foreground source address, the background source address,
               the destination and data length then Enable the transfer using
               HAL_DMA2D_BlendingStart() in polling mode and HAL_DMA2D_BlendingStart_IT()
               in interrupt mode.

         -@-   HAL_DMA2D_BlendingStart() and HAL_DMA2D_BlendingStart_IT() functions
               are used if the memory to memory with blending transfer mode is selected.

      (#) Optionally, configure and enable the CLUT using HAL_DMA2D_CLUTLoad() in polling
          mode or HAL_DMA2D_CLUTLoad_IT() in interrupt mode.

      (#) Optionally, configure the line watermark in using the API HAL_DMA2D_ProgramLineEvent().

      (#) Optionally, configure the dead time value in the AHB clock cycle inserted between two
          consecutive accesses on the AHB master port in using the API HAL_DMA2D_ConfigDeadTime()
          and enable/disable the functionality  with the APIs HAL_DMA2D_EnableDeadTime() or
          HAL_DMA2D_DisableDeadTime().

      (#) The transfer can be suspended, resumed and aborted using the following
          functions: HAL_DMA2D_Suspend(), HAL_DMA2D_Resume(), HAL_DMA2D_Abort().

      (#) The CLUT loading can be suspended, resumed and aborted using the following
          functions: HAL_DMA2D_CLUTLoading_Suspend(), HAL_DMA2D_CLUTLoading_Resume(),
          HAL_DMA2D_CLUTLoading_Abort().

      (#) To control the DMA2D state, use the following function: HAL_DMA2D_GetState().

      (#) To read the DMA2D error code, use the following function: HAL_DMA2D_GetError().

     *** DMA2D HAL driver macros list ***
     =============================================
     [..]
       Below the list of most used macros in DMA2D HAL driver :

      (+) __HAL_DMA2D_ENABLE: Enable the DMA2D peripheral.
      (+) __HAL_DMA2D_GET_FLAG: Get the DMA2D pending flags.
      (+) __HAL_DMA2D_CLEAR_FLAG: Clear the DMA2D pending flags.
      (+) __HAL_DMA2D_ENABLE_IT: Enable the specified DMA2D interrupts.
      (+) __HAL_DMA2D_DISABLE_IT: Disable the specified DMA2D interrupts.
      (+) __HAL_DMA2D_GET_IT_SOURCE: Check whether the specified DMA2D interrupt is enabled or not.

     *** Callback registration ***
     ===================================
     [..]
      (#) The compilation define  USE_HAL_DMA2D_REGISTER_CALLBACKS when set to 1
          allows the user to configure dynamically the driver callbacks.
          Use function @ref HAL_DMA2D_RegisterCallback() to register a user callback.

      (#) Function @ref HAL_DMA2D_RegisterCallback() allows to register following callbacks:
            (+) XferCpltCallback : callback for transfer complete.
            (+) XferErrorCallback : callback for transfer error.
            (+) LineEventCallback : callback for line event.
            (+) CLUTLoadingCpltCallback : callback for CLUT loading completion.
            (+) MspInitCallback    : DMA2D MspInit.
            (+) MspDeInitCallback  : DMA2D MspDeInit.
          This function takes as parameters the HAL peripheral handle, the Callback ID
          and a pointer to the user callback function.

      (#) Use function @ref HAL_DMA2D_UnRegisterCallback() to reset a callback to the default
          weak (surcharged) function.
          @ref HAL_DMA2D_UnRegisterCallback() takes as parameters the HAL peripheral handle,
          and the Callback ID.
          This function allows to reset following callbacks:
            (+) XferCpltCallback : callback for transfer complete.
            (+) XferErrorCallback : callback for transfer error.
            (+) LineEventCallback : callback for line event.
            (+) CLUTLoadingCpltCallback : callback for CLUT loading completion.
            (+) MspInitCallback    : DMA2D MspInit.
            (+) MspDeInitCallback  : DMA2D MspDeInit.

      (#) By default, after the @ref HAL_DMA2D_Init and if the state is HAL_DMA2D_STATE_RESET
          all callbacks are reset to the corresponding legacy weak (surcharged) functions:
          examples @ref HAL_DMA2D_LineEventCallback(), @ref HAL_DMA2D_CLUTLoadingCpltCallback()
          Exception done for MspInit and MspDeInit callbacks that are respectively
          reset to the legacy weak (surcharged) functions in the @ref HAL_DMA2D_Init
          and @ref HAL_DMA2D_DeInit only when these callbacks are null (not registered beforehand)
          If not, MspInit or MspDeInit are not null, the @ref HAL_DMA2D_Init and @ref HAL_DMA2D_DeInit
          keep and use the user MspInit/MspDeInit callbacks (registered beforehand).

          Exception as well for Transfer Completion and Transfer Error callbacks that are not defined
          as weak (surcharged) functions. They must be defined by the user to be resorted to.

          Callbacks can be registered/unregistered in READY state only.
          Exception done for MspInit/MspDeInit callbacks that can be registered/unregistered
          in READY or RESET state, thus registered (user) MspInit/DeInit callbacks can be used
          during the Init/DeInit.
          In that case first register the MspInit/MspDeInit user callbacks
          using @ref HAL_DMA2D_RegisterCallback before calling @ref HAL_DMA2D_DeInit
          or @ref HAL_DMA2D_Init function.

          When The compilation define USE_HAL_DMA2D_REGISTER_CALLBACKS is set to 0 or
          not defined, the callback registering feature is not available
          and weak (surcharged) callbacks are used.

     [..]
      (@) You can refer to the DMA2D HAL driver header file for more useful macros

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

#ifdef HAL_DMA2D_MODULE_ENABLED                         /* 条件编译 */
#if defined (DMA2D)                                     /* 条件编译 */

/** @addtogroup STM32F4xx_HAL_Driver
  * @{
  */

/** @defgroup DMA2D  DMA2D
  * @brief DMA2D HAL module driver
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @defgroup DMA2D_Private_Constants DMA2D Private Constants
  * @{
  */

/** @defgroup DMA2D_TimeOut DMA2D Time Out
  * @{
  */
#define DMA2D_TIMEOUT_ABORT           (1000U)  /*!<  1s  */
#define DMA2D_TIMEOUT_SUSPEND         (1000U)  /*!<  1s  */
/**
  * @}
  */

/**
  * @}
  */

/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/** @addtogroup DMA2D_Private_Functions DMA2D Private Functions
  * @{
  */
static void DMA2D_SetConfig(DMA2D_HandleTypeDef *hdma2d, uint32_t pdata, uint32_t DstAddress, uint32_t Width,  /* 静态函数 */
                            uint32_t Height);           /* 32位无符号变量/函数 */
/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/** @defgroup DMA2D_Exported_Functions DMA2D Exported Functions
  * @{
  */

/** @defgroup DMA2D_Exported_Functions_Group1 Initialization and de-initialization functions
  *  @brief   Initialization and Configuration functions
  *
@verbatim
 ===============================================================================
                ##### Initialization and Configuration functions #####
 ===============================================================================
    [..]  This section provides functions allowing to:
      (+) Initialize and configure the DMA2D
      (+) De-initialize the DMA2D

@endverbatim
  * @{
  */

/**
  * @brief  Initialize the DMA2D according to the specified
  *         parameters in the DMA2D_InitTypeDef and create the associated handle.
  * @param  hdma2d pointer to a DMA2D_HandleTypeDef structure that contains
  *                 the configuration information for the DMA2D.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA2D_Init(DMA2D_HandleTypeDef *hdma2d)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the DMA2D peripheral state */
  if (hdma2d == NULL)                                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the parameters */
  assert_param(IS_DMA2D_ALL_INSTANCE(hdma2d->Instance));  /* 执行语句 */
  assert_param(IS_DMA2D_MODE(hdma2d->Init.Mode));       /* 执行语句 */
  assert_param(IS_DMA2D_CMODE(hdma2d->Init.ColorMode));  /* 执行语句 */
  assert_param(IS_DMA2D_OFFSET(hdma2d->Init.OutputOffset));  /* 执行语句 */

#if (USE_HAL_DMA2D_REGISTER_CALLBACKS == 1)             /* 条件编译 */
  if (hdma2d->State == HAL_DMA2D_STATE_RESET)           /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Reset Callback pointers in HAL_DMA2D_STATE_RESET only */
    hdma2d->LineEventCallback       = HAL_DMA2D_LineEventCallback;  /* 赋值或初始化 */
    hdma2d->CLUTLoadingCpltCallback = HAL_DMA2D_CLUTLoadingCpltCallback;  /* 赋值或初始化 */
    if (hdma2d->MspInitCallback == NULL)                /* 条件判断 */
    {                                                   /* 代码块开始 */
      hdma2d->MspInitCallback = HAL_DMA2D_MspInit;      /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Init the low level hardware */
    hdma2d->MspInitCallback(hdma2d);                    /* 执行语句 */
  }                                                     /* 代码块结束 */
#else                                                   /* 条件编译否则 */
  if (hdma2d->State == HAL_DMA2D_STATE_RESET)           /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Allocate lock resource and initialize it */
    hdma2d->Lock = HAL_UNLOCKED;                        /* 赋值或初始化 */
    /* Init the low level hardware */
    HAL_DMA2D_MspInit(hdma2d);                          /* HAL库函数调用 */
  }                                                     /* 代码块结束 */
#endif /* (USE_HAL_DMA2D_REGISTER_CALLBACKS) */

  /* Change DMA2D peripheral state */
  hdma2d->State = HAL_DMA2D_STATE_BUSY;                 /* 赋值或初始化 */

  /* DMA2D CR register configuration -------------------------------------------*/
  MODIFY_REG(hdma2d->Instance->CR, DMA2D_CR_MODE, hdma2d->Init.Mode);  /* 执行语句 */

  /* DMA2D OPFCCR register configuration ---------------------------------------*/
  MODIFY_REG(hdma2d->Instance->OPFCCR, DMA2D_OPFCCR_CM, hdma2d->Init.ColorMode);  /* 执行语句 */

  /* DMA2D OOR register configuration ------------------------------------------*/
  MODIFY_REG(hdma2d->Instance->OOR, DMA2D_OOR_LO, hdma2d->Init.OutputOffset);  /* 执行语句 */


  /* Update error code */
  hdma2d->ErrorCode = HAL_DMA2D_ERROR_NONE;             /* 赋值或初始化 */

  /* Initialize the DMA2D state*/
  hdma2d->State  = HAL_DMA2D_STATE_READY;               /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Deinitializes the DMA2D peripheral registers to their default reset
  *         values.
  * @param  hdma2d pointer to a DMA2D_HandleTypeDef structure that contains
  *                 the configuration information for the DMA2D.
  * @retval None
  */

HAL_StatusTypeDef HAL_DMA2D_DeInit(DMA2D_HandleTypeDef *hdma2d)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */

  /* Check the DMA2D peripheral state */
  if (hdma2d == NULL)                                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Before aborting any DMA2D transfer or CLUT loading, check
     first whether or not DMA2D clock is enabled */
  if (__HAL_RCC_DMA2D_IS_CLK_ENABLED())                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Abort DMA2D transfer if any */
    if ((hdma2d->Instance->CR & DMA2D_CR_START) == DMA2D_CR_START)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (HAL_DMA2D_Abort(hdma2d) != HAL_OK)            /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Issue when aborting DMA2D transfer */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Abort background CLUT loading if any */
      if ((hdma2d->Instance->BGPFCCR & DMA2D_BGPFCCR_START) == DMA2D_BGPFCCR_START)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        if (HAL_DMA2D_CLUTLoading_Abort(hdma2d, 0U) != HAL_OK)  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Issue when aborting background CLUT loading */
          return HAL_ERROR;                             /* 返回值 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Abort foreground CLUT loading if any */
        if ((hdma2d->Instance->FGPFCCR & DMA2D_FGPFCCR_START) == DMA2D_FGPFCCR_START)  /* 条件判断 */
        {                                               /* 代码块开始 */
          if (HAL_DMA2D_CLUTLoading_Abort(hdma2d, 1U) != HAL_OK)  /* 条件判断 */
          {                                             /* 代码块开始 */
            /* Issue when aborting foreground CLUT loading */
            return HAL_ERROR;                           /* 返回值 */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Reset DMA2D control registers*/
  hdma2d->Instance->CR       =    0U;                   /* 赋值或初始化 */
  hdma2d->Instance->IFCR     = 0x3FU;                   /* 赋值或初始化 */
  hdma2d->Instance->FGOR     =    0U;                   /* 赋值或初始化 */
  hdma2d->Instance->BGOR     =    0U;                   /* 赋值或初始化 */
  hdma2d->Instance->FGPFCCR  =    0U;                   /* 赋值或初始化 */
  hdma2d->Instance->BGPFCCR  =    0U;                   /* 赋值或初始化 */
  hdma2d->Instance->OPFCCR   =    0U;                   /* 赋值或初始化 */

#if (USE_HAL_DMA2D_REGISTER_CALLBACKS == 1)             /* 条件编译 */

  if (hdma2d->MspDeInitCallback == NULL)                /* 条件判断 */
  {                                                     /* 代码块开始 */
    hdma2d->MspDeInitCallback = HAL_DMA2D_MspDeInit;    /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* DeInit the low level hardware */
  hdma2d->MspDeInitCallback(hdma2d);                    /* 执行语句 */

#else                                                   /* 条件编译否则 */
  /* Carry on with de-initialization of low level hardware */
  HAL_DMA2D_MspDeInit(hdma2d);                          /* HAL库函数调用 */
#endif /* (USE_HAL_DMA2D_REGISTER_CALLBACKS) */

  /* Update error code */
  hdma2d->ErrorCode = HAL_DMA2D_ERROR_NONE;             /* 赋值或初始化 */

  /* Initialize the DMA2D state*/
  hdma2d->State  = HAL_DMA2D_STATE_RESET;               /* 赋值或初始化 */

  /* Release Lock */
  __HAL_UNLOCK(hdma2d);                                 /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initializes the DMA2D MSP.
  * @param  hdma2d pointer to a DMA2D_HandleTypeDef structure that contains
  *                 the configuration information for the DMA2D.
  * @retval None
  */
__weak void HAL_DMA2D_MspInit(DMA2D_HandleTypeDef *hdma2d)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdma2d);                                       /* 执行语句 */

  /* NOTE : This function should not be modified; when the callback is needed,
            the HAL_DMA2D_MspInit can be implemented in the user file.
   */
}                                                       /* 代码块结束 */

/**
  * @brief  DeInitializes the DMA2D MSP.
  * @param  hdma2d pointer to a DMA2D_HandleTypeDef structure that contains
  *                 the configuration information for the DMA2D.
  * @retval None
  */
__weak void HAL_DMA2D_MspDeInit(DMA2D_HandleTypeDef *hdma2d)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdma2d);                                       /* 执行语句 */

  /* NOTE : This function should not be modified; when the callback is needed,
            the HAL_DMA2D_MspDeInit can be implemented in the user file.
   */
}                                                       /* 代码块结束 */

#if (USE_HAL_DMA2D_REGISTER_CALLBACKS == 1)             /* 条件编译 */
/**
  * @brief  Register a User DMA2D Callback
  *         To be used instead of the weak (surcharged) predefined callback
  * @param hdma2d DMA2D handle
  * @param CallbackID ID of the callback to be registered
  *        This parameter can be one of the following values:
  *          @arg @ref HAL_DMA2D_TRANSFERCOMPLETE_CB_ID DMA2D transfer complete Callback ID
  *          @arg @ref HAL_DMA2D_TRANSFERERROR_CB_ID DMA2D transfer error Callback ID
  *          @arg @ref HAL_DMA2D_LINEEVENT_CB_ID DMA2D line event Callback ID
  *          @arg @ref HAL_DMA2D_CLUTLOADINGCPLT_CB_ID DMA2D CLUT loading completion Callback ID
  *          @arg @ref HAL_DMA2D_MSPINIT_CB_ID DMA2D MspInit callback ID
  *          @arg @ref HAL_DMA2D_MSPDEINIT_CB_ID DMA2D MspDeInit callback ID
  * @param pCallback pointer to the Callback function
  * @note No weak predefined callbacks are defined for HAL_DMA2D_TRANSFERCOMPLETE_CB_ID or HAL_DMA2D_TRANSFERERROR_CB_ID
  * @retval status
  */
HAL_StatusTypeDef HAL_DMA2D_RegisterCallback(DMA2D_HandleTypeDef *hdma2d, HAL_DMA2D_CallbackIDTypeDef CallbackID,  /* HAL状态类型函数 */
                                             pDMA2D_CallbackTypeDef pCallback)  /* 源代码 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  if (pCallback == NULL)                                /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hdma2d->ErrorCode |= HAL_DMA2D_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  /* Process locked */
  __HAL_LOCK(hdma2d);                                   /* HAL辅助宏/函数 */

  if (HAL_DMA2D_STATE_READY == hdma2d->State)           /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_DMA2D_TRANSFERCOMPLETE_CB_ID :           /* case标签 */
        hdma2d->XferCpltCallback = pCallback;           /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_DMA2D_TRANSFERERROR_CB_ID :              /* case标签 */
        hdma2d->XferErrorCallback = pCallback;          /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_DMA2D_LINEEVENT_CB_ID :                  /* case标签 */
        hdma2d->LineEventCallback = pCallback;          /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_DMA2D_CLUTLOADINGCPLT_CB_ID :            /* case标签 */
        hdma2d->CLUTLoadingCpltCallback = pCallback;    /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_DMA2D_MSPINIT_CB_ID :                    /* case标签 */
        hdma2d->MspInitCallback = pCallback;            /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_DMA2D_MSPDEINIT_CB_ID :                  /* case标签 */
        hdma2d->MspDeInitCallback = pCallback;          /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hdma2d->ErrorCode |= HAL_DMA2D_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
        /* update return status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (HAL_DMA2D_STATE_RESET == hdma2d->State)      /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_DMA2D_MSPINIT_CB_ID :                    /* case标签 */
        hdma2d->MspInitCallback = pCallback;            /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_DMA2D_MSPDEINIT_CB_ID :                  /* case标签 */
        hdma2d->MspDeInitCallback = pCallback;          /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hdma2d->ErrorCode |= HAL_DMA2D_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
        /* update return status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hdma2d->ErrorCode |= HAL_DMA2D_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
    /* update return status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hdma2d);                                 /* HAL辅助宏/函数 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Unregister a DMA2D Callback
  *         DMA2D Callback is redirected to the weak (surcharged) predefined callback
  * @param hdma2d DMA2D handle
  * @param CallbackID ID of the callback to be unregistered
  *        This parameter can be one of the following values:
  *          @arg @ref HAL_DMA2D_TRANSFERCOMPLETE_CB_ID DMA2D transfer complete Callback ID
  *          @arg @ref HAL_DMA2D_TRANSFERERROR_CB_ID DMA2D transfer error Callback ID
  *          @arg @ref HAL_DMA2D_LINEEVENT_CB_ID DMA2D line event Callback ID
  *          @arg @ref HAL_DMA2D_CLUTLOADINGCPLT_CB_ID DMA2D CLUT loading completion Callback ID
  *          @arg @ref HAL_DMA2D_MSPINIT_CB_ID DMA2D MspInit callback ID
  *          @arg @ref HAL_DMA2D_MSPDEINIT_CB_ID DMA2D MspDeInit callback ID
  * @note No weak predefined callbacks are defined for HAL_DMA2D_TRANSFERCOMPLETE_CB_ID or HAL_DMA2D_TRANSFERERROR_CB_ID
  * @retval status
  */
HAL_StatusTypeDef HAL_DMA2D_UnRegisterCallback(DMA2D_HandleTypeDef *hdma2d, HAL_DMA2D_CallbackIDTypeDef CallbackID)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Process locked */
  __HAL_LOCK(hdma2d);                                   /* HAL辅助宏/函数 */

  if (HAL_DMA2D_STATE_READY == hdma2d->State)           /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_DMA2D_TRANSFERCOMPLETE_CB_ID :           /* case标签 */
        hdma2d->XferCpltCallback = NULL;                /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_DMA2D_TRANSFERERROR_CB_ID :              /* case标签 */
        hdma2d->XferErrorCallback = NULL;               /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_DMA2D_LINEEVENT_CB_ID :                  /* case标签 */
        hdma2d->LineEventCallback = HAL_DMA2D_LineEventCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_DMA2D_CLUTLOADINGCPLT_CB_ID :            /* case标签 */
        hdma2d->CLUTLoadingCpltCallback = HAL_DMA2D_CLUTLoadingCpltCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_DMA2D_MSPINIT_CB_ID :                    /* case标签 */
        hdma2d->MspInitCallback = HAL_DMA2D_MspInit; /* Legacy weak (surcharged) Msp Init */
        break;                                          /* 跳出 */

      case HAL_DMA2D_MSPDEINIT_CB_ID :                  /* case标签 */
        hdma2d->MspDeInitCallback = HAL_DMA2D_MspDeInit; /* Legacy weak (surcharged) Msp DeInit */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hdma2d->ErrorCode |= HAL_DMA2D_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
        /* update return status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (HAL_DMA2D_STATE_RESET == hdma2d->State)      /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_DMA2D_MSPINIT_CB_ID :                    /* case标签 */
        hdma2d->MspInitCallback = HAL_DMA2D_MspInit;   /* Legacy weak (surcharged) Msp Init */
        break;                                          /* 跳出 */

      case HAL_DMA2D_MSPDEINIT_CB_ID :                  /* case标签 */
        hdma2d->MspDeInitCallback = HAL_DMA2D_MspDeInit;  /* Legacy weak (surcharged) Msp DeInit */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hdma2d->ErrorCode |= HAL_DMA2D_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
        /* update return status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hdma2d->ErrorCode |= HAL_DMA2D_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
    /* update return status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hdma2d);                                 /* HAL辅助宏/函数 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* USE_HAL_DMA2D_REGISTER_CALLBACKS */

/**
  * @}
  */


/** @defgroup DMA2D_Exported_Functions_Group2 IO operation functions
  *  @brief   IO operation functions
  *
@verbatim
 ===============================================================================
                      #####  IO operation functions  #####
 ===============================================================================
    [..]  This section provides functions allowing to:
      (+) Configure the pdata, destination address and data size then
          start the DMA2D transfer.
      (+) Configure the source for foreground and background, destination address
          and data size then start a MultiBuffer DMA2D transfer.
      (+) Configure the pdata, destination address and data size then
          start the DMA2D transfer with interrupt.
      (+) Configure the source for foreground and background, destination address
          and data size then start a MultiBuffer DMA2D transfer with interrupt.
      (+) Abort DMA2D transfer.
      (+) Suspend DMA2D transfer.
      (+) Resume DMA2D transfer.
      (+) Enable CLUT transfer.
      (+) Configure CLUT loading then start transfer in polling mode.
      (+) Configure CLUT loading then start transfer in interrupt mode.
      (+) Abort DMA2D CLUT loading.
      (+) Suspend DMA2D CLUT loading.
      (+) Resume DMA2D CLUT loading.
      (+) Poll for transfer complete.
      (+) handle DMA2D interrupt request.
      (+) Transfer watermark callback.
      (+) CLUT Transfer Complete callback.


@endverbatim
  * @{
  */

/**
  * @brief  Start the DMA2D Transfer.
  * @param  hdma2d     Pointer to a DMA2D_HandleTypeDef structure that contains
  *                     the configuration information for the DMA2D.
  * @param  pdata      Configure the source memory Buffer address if
  *                     Memory-to-Memory or Memory-to-Memory with pixel format
  *                     conversion mode is selected, or configure
  *                     the color value if Register-to-Memory mode is selected.
  * @param  DstAddress The destination memory Buffer address.
  * @param  Width      The width of data to be transferred from source
  *                    to destination (expressed in number of pixels per line).
  * @param  Height     The height of data to be transferred from source to destination (expressed in number of lines).
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA2D_Start(DMA2D_HandleTypeDef *hdma2d, uint32_t pdata, uint32_t DstAddress, uint32_t Width,  /* HAL状态类型函数 */
                                  uint32_t Height)      /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_DMA2D_LINE(Height));                  /* 执行语句 */
  assert_param(IS_DMA2D_PIXEL(Width));                  /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hdma2d);                                   /* HAL辅助宏/函数 */

  /* Change DMA2D peripheral state */
  hdma2d->State = HAL_DMA2D_STATE_BUSY;                 /* 赋值或初始化 */

  /* Configure the source, destination address and the data size */
  DMA2D_SetConfig(hdma2d, pdata, DstAddress, Width, Height);  /* 执行语句 */

  /* Enable the Peripheral */
  __HAL_DMA2D_ENABLE(hdma2d);                           /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Start the DMA2D Transfer with interrupt enabled.
  * @param  hdma2d     Pointer to a DMA2D_HandleTypeDef structure that contains
  *                     the configuration information for the DMA2D.
  * @param  pdata      Configure the source memory Buffer address if
  *                     the Memory-to-Memory or Memory-to-Memory with pixel format
  *                     conversion mode is selected, or configure
  *                     the color value if Register-to-Memory mode is selected.
  * @param  DstAddress The destination memory Buffer address.
  * @param  Width      The width of data to be transferred from source
  *                    to destination (expressed in number of pixels per line).
  * @param  Height     The height of data to be transferred from source to destination (expressed in number of lines).
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA2D_Start_IT(DMA2D_HandleTypeDef *hdma2d, uint32_t pdata, uint32_t DstAddress, uint32_t Width,  /* HAL状态类型函数 */
                                     uint32_t Height)   /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_DMA2D_LINE(Height));                  /* 执行语句 */
  assert_param(IS_DMA2D_PIXEL(Width));                  /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hdma2d);                                   /* HAL辅助宏/函数 */

  /* Change DMA2D peripheral state */
  hdma2d->State = HAL_DMA2D_STATE_BUSY;                 /* 赋值或初始化 */

  /* Configure the source, destination address and the data size */
  DMA2D_SetConfig(hdma2d, pdata, DstAddress, Width, Height);  /* 执行语句 */

  /* Enable the transfer complete, transfer error and configuration error interrupts */
  __HAL_DMA2D_ENABLE_IT(hdma2d, DMA2D_IT_TC | DMA2D_IT_TE | DMA2D_IT_CE);  /* HAL辅助宏/函数 */

  /* Enable the Peripheral */
  __HAL_DMA2D_ENABLE(hdma2d);                           /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Start the multi-source DMA2D Transfer.
  * @param  hdma2d      Pointer to a DMA2D_HandleTypeDef structure that contains
  *                      the configuration information for the DMA2D.
  * @param  SrcAddress1 The source memory Buffer address for the foreground layer.
  * @param  SrcAddress2 The source memory Buffer address for the background layer.
  * @param  DstAddress  The destination memory Buffer address.
  * @param  Width       The width of data to be transferred from source
  *                     to destination (expressed in number of pixels per line).
  * @param  Height      The height of data to be transferred from source to destination (expressed in number of lines).
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA2D_BlendingStart(DMA2D_HandleTypeDef *hdma2d, uint32_t SrcAddress1, uint32_t  SrcAddress2,  /* HAL状态类型函数 */
                                          uint32_t DstAddress, uint32_t Width,  uint32_t Height)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_DMA2D_LINE(Height));                  /* 执行语句 */
  assert_param(IS_DMA2D_PIXEL(Width));                  /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hdma2d);                                   /* HAL辅助宏/函数 */

  /* Change DMA2D peripheral state */
  hdma2d->State = HAL_DMA2D_STATE_BUSY;                 /* 赋值或初始化 */

  /* Configure DMA2D Stream source2 address */
  WRITE_REG(hdma2d->Instance->BGMAR, SrcAddress2);      /* 执行语句 */

  /* Configure the source, destination address and the data size */
  DMA2D_SetConfig(hdma2d, SrcAddress1, DstAddress, Width, Height);  /* 执行语句 */

  /* Enable the Peripheral */
  __HAL_DMA2D_ENABLE(hdma2d);                           /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Start the multi-source DMA2D Transfer with interrupt enabled.
  * @param  hdma2d     Pointer to a DMA2D_HandleTypeDef structure that contains
  *                     the configuration information for the DMA2D.
  * @param  SrcAddress1 The source memory Buffer address for the foreground layer.
  * @param  SrcAddress2 The source memory Buffer address for the background layer.
  * @param  DstAddress  The destination memory Buffer address.
  * @param  Width       The width of data to be transferred from source
  *                     to destination (expressed in number of pixels per line).
  * @param  Height      The height of data to be transferred from source to destination (expressed in number of lines).
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA2D_BlendingStart_IT(DMA2D_HandleTypeDef *hdma2d, uint32_t SrcAddress1, uint32_t  SrcAddress2,  /* HAL状态类型函数 */
                                             uint32_t DstAddress, uint32_t Width,  uint32_t Height)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_DMA2D_LINE(Height));                  /* 执行语句 */
  assert_param(IS_DMA2D_PIXEL(Width));                  /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hdma2d);                                   /* HAL辅助宏/函数 */

  /* Change DMA2D peripheral state */
  hdma2d->State = HAL_DMA2D_STATE_BUSY;                 /* 赋值或初始化 */

  /* Configure DMA2D Stream source2 address */
  WRITE_REG(hdma2d->Instance->BGMAR, SrcAddress2);      /* 执行语句 */

  /* Configure the source, destination address and the data size */
  DMA2D_SetConfig(hdma2d, SrcAddress1, DstAddress, Width, Height);  /* 执行语句 */

  /* Enable the transfer complete, transfer error and configuration error interrupts */
  __HAL_DMA2D_ENABLE_IT(hdma2d, DMA2D_IT_TC | DMA2D_IT_TE | DMA2D_IT_CE);  /* HAL辅助宏/函数 */

  /* Enable the Peripheral */
  __HAL_DMA2D_ENABLE(hdma2d);                           /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Abort the DMA2D Transfer.
  * @param  hdma2d  pointer to a DMA2D_HandleTypeDef structure that contains
  *                  the configuration information for the DMA2D.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA2D_Abort(DMA2D_HandleTypeDef *hdma2d)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */

  /* Abort the DMA2D transfer */
  /* START bit is reset to make sure not to set it again, in the event the HW clears it
     between the register read and the register write by the CPU (writing 0 has no
     effect on START bitvalue) */
  MODIFY_REG(hdma2d->Instance->CR, DMA2D_CR_ABORT | DMA2D_CR_START, DMA2D_CR_ABORT);  /* 执行语句 */

  /* Get tick */
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  /* Check if the DMA2D is effectively disabled */
  while ((hdma2d->Instance->CR & DMA2D_CR_START) != 0U)  /* while循环 */
  {                                                     /* 代码块开始 */
    if ((HAL_GetTick() - tickstart) > DMA2D_TIMEOUT_ABORT)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Update error code */
      hdma2d->ErrorCode |= HAL_DMA2D_ERROR_TIMEOUT;     /* 赋值或初始化 */

      /* Change the DMA2D state */
      hdma2d->State = HAL_DMA2D_STATE_TIMEOUT;          /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hdma2d);                             /* HAL辅助宏/函数 */

      return HAL_TIMEOUT;                               /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Disable the Transfer Complete, Transfer Error and Configuration Error interrupts */
  __HAL_DMA2D_DISABLE_IT(hdma2d, DMA2D_IT_TC | DMA2D_IT_TE | DMA2D_IT_CE);  /* HAL辅助宏/函数 */

  /* Change the DMA2D state*/
  hdma2d->State = HAL_DMA2D_STATE_READY;                /* 赋值或初始化 */

  /* Process Unlocked */
  __HAL_UNLOCK(hdma2d);                                 /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Suspend the DMA2D Transfer.
  * @param  hdma2d pointer to a DMA2D_HandleTypeDef structure that contains
  *                 the configuration information for the DMA2D.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA2D_Suspend(DMA2D_HandleTypeDef *hdma2d)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */

  /* Suspend the DMA2D transfer */
  /* START bit is reset to make sure not to set it again, in the event the HW clears it
     between the register read and the register write by the CPU (writing 0 has no
     effect on START bitvalue). */
  MODIFY_REG(hdma2d->Instance->CR, DMA2D_CR_SUSP | DMA2D_CR_START, DMA2D_CR_SUSP);  /* 执行语句 */

  /* Get tick */
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  /* Check if the DMA2D is effectively suspended */
  while ((hdma2d->Instance->CR & (DMA2D_CR_SUSP | DMA2D_CR_START)) == DMA2D_CR_START)  /* while循环 */
  {                                                     /* 代码块开始 */
    if ((HAL_GetTick() - tickstart) > DMA2D_TIMEOUT_SUSPEND)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Update error code */
      hdma2d->ErrorCode |= HAL_DMA2D_ERROR_TIMEOUT;     /* 赋值或初始化 */

      /* Change the DMA2D state */
      hdma2d->State = HAL_DMA2D_STATE_TIMEOUT;          /* 赋值或初始化 */

      return HAL_TIMEOUT;                               /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Check whether or not a transfer is actually suspended and change the DMA2D state accordingly */
  if ((hdma2d->Instance->CR & DMA2D_CR_START) != 0U)    /* 条件判断 */
  {                                                     /* 代码块开始 */
    hdma2d->State = HAL_DMA2D_STATE_SUSPEND;            /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Make sure SUSP bit is cleared since it is meaningless
       when no transfer is on-going */
    CLEAR_BIT(hdma2d->Instance->CR, DMA2D_CR_SUSP);     /* 执行语句 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Resume the DMA2D Transfer.
  * @param  hdma2d pointer to a DMA2D_HandleTypeDef structure that contains
  *                 the configuration information for the DMA2D.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA2D_Resume(DMA2D_HandleTypeDef *hdma2d)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the SUSP and START bits */
  if ((hdma2d->Instance->CR & (DMA2D_CR_SUSP | DMA2D_CR_START)) == (DMA2D_CR_SUSP | DMA2D_CR_START))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Ongoing transfer is suspended: change the DMA2D state before resuming */
    hdma2d->State = HAL_DMA2D_STATE_BUSY;               /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Resume the DMA2D transfer */
  /* START bit is reset to make sure not to set it again, in the event the HW clears it
     between the register read and the register write by the CPU (writing 0 has no
     effect on START bitvalue). */
  CLEAR_BIT(hdma2d->Instance->CR, (DMA2D_CR_SUSP | DMA2D_CR_START));  /* 执行语句 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */


/**
  * @brief  Enable the DMA2D CLUT Transfer.
  * @param  hdma2d   Pointer to a DMA2D_HandleTypeDef structure that contains
  *                   the configuration information for the DMA2D.
  * @param  LayerIdx DMA2D Layer index.
  *                   This parameter can be one of the following values:
  *                   DMA2D_BACKGROUND_LAYER(0) / DMA2D_FOREGROUND_LAYER(1)
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA2D_EnableCLUT(DMA2D_HandleTypeDef *hdma2d, uint32_t LayerIdx)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_DMA2D_LAYER(LayerIdx));               /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hdma2d);                                   /* HAL辅助宏/函数 */

  /* Change DMA2D peripheral state */
  hdma2d->State = HAL_DMA2D_STATE_BUSY;                 /* 赋值或初始化 */

  if (LayerIdx == DMA2D_BACKGROUND_LAYER)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable the background CLUT loading */
    SET_BIT(hdma2d->Instance->BGPFCCR, DMA2D_BGPFCCR_START);  /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Enable the foreground CLUT loading */
    SET_BIT(hdma2d->Instance->FGPFCCR, DMA2D_FGPFCCR_START);  /* 执行语句 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Start DMA2D CLUT Loading.
  * @param  hdma2d   Pointer to a DMA2D_HandleTypeDef structure that contains
  *                   the configuration information for the DMA2D.
  * @param  CLUTCfg  Pointer to a DMA2D_CLUTCfgTypeDef structure that contains
  *                   the configuration information for the color look up table.
  * @param  LayerIdx DMA2D Layer index.
  *                   This parameter can be one of the following values:
  *                   DMA2D_BACKGROUND_LAYER(0) / DMA2D_FOREGROUND_LAYER(1)
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA2D_CLUTStartLoad(DMA2D_HandleTypeDef *hdma2d, DMA2D_CLUTCfgTypeDef *CLUTCfg, uint32_t LayerIdx)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_DMA2D_LAYER(LayerIdx));               /* 执行语句 */
  assert_param(IS_DMA2D_CLUT_CM(CLUTCfg->CLUTColorMode));  /* 执行语句 */
  assert_param(IS_DMA2D_CLUT_SIZE(CLUTCfg->Size));      /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hdma2d);                                   /* HAL辅助宏/函数 */

  /* Change DMA2D peripheral state */
  hdma2d->State = HAL_DMA2D_STATE_BUSY;                 /* 赋值或初始化 */

  /* Configure the CLUT of the background DMA2D layer */
  if (LayerIdx == DMA2D_BACKGROUND_LAYER)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Write background CLUT memory address */
    WRITE_REG(hdma2d->Instance->BGCMAR, (uint32_t)CLUTCfg->pCLUT);  /* 执行语句 */

    /* Write background CLUT size and CLUT color mode */
    MODIFY_REG(hdma2d->Instance->BGPFCCR, (DMA2D_BGPFCCR_CS | DMA2D_BGPFCCR_CCM),  /* 列表续行 */
               ((CLUTCfg->Size << DMA2D_BGPFCCR_CS_Pos) | (CLUTCfg->CLUTColorMode << DMA2D_BGPFCCR_CCM_Pos)));  /* 执行语句 */

    /* Enable the CLUT loading for the background */
    SET_BIT(hdma2d->Instance->BGPFCCR, DMA2D_BGPFCCR_START);  /* 执行语句 */
  }                                                     /* 代码块结束 */
  /* Configure the CLUT of the foreground DMA2D layer */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Write foreground CLUT memory address */
    WRITE_REG(hdma2d->Instance->FGCMAR, (uint32_t)CLUTCfg->pCLUT);  /* 执行语句 */

    /* Write foreground CLUT size and CLUT color mode */
    MODIFY_REG(hdma2d->Instance->FGPFCCR, (DMA2D_FGPFCCR_CS | DMA2D_FGPFCCR_CCM),  /* 列表续行 */
               ((CLUTCfg->Size << DMA2D_FGPFCCR_CS_Pos) | (CLUTCfg->CLUTColorMode << DMA2D_FGPFCCR_CCM_Pos)));  /* 执行语句 */

    /* Enable the CLUT loading for the foreground */
    SET_BIT(hdma2d->Instance->FGPFCCR, DMA2D_FGPFCCR_START);  /* 执行语句 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Start DMA2D CLUT Loading with interrupt enabled.
  * @param  hdma2d   Pointer to a DMA2D_HandleTypeDef structure that contains
  *                   the configuration information for the DMA2D.
  * @param  CLUTCfg  Pointer to a DMA2D_CLUTCfgTypeDef structure that contains
  *                   the configuration information for the color look up table.
  * @param  LayerIdx DMA2D Layer index.
  *                   This parameter can be one of the following values:
  *                   DMA2D_BACKGROUND_LAYER(0) / DMA2D_FOREGROUND_LAYER(1)
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA2D_CLUTStartLoad_IT(DMA2D_HandleTypeDef *hdma2d, DMA2D_CLUTCfgTypeDef *CLUTCfg,  /* HAL状态类型函数 */
                                             uint32_t LayerIdx)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_DMA2D_LAYER(LayerIdx));               /* 执行语句 */
  assert_param(IS_DMA2D_CLUT_CM(CLUTCfg->CLUTColorMode));  /* 执行语句 */
  assert_param(IS_DMA2D_CLUT_SIZE(CLUTCfg->Size));      /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hdma2d);                                   /* HAL辅助宏/函数 */

  /* Change DMA2D peripheral state */
  hdma2d->State = HAL_DMA2D_STATE_BUSY;                 /* 赋值或初始化 */

  /* Configure the CLUT of the background DMA2D layer */
  if (LayerIdx == DMA2D_BACKGROUND_LAYER)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Write background CLUT memory address */
    WRITE_REG(hdma2d->Instance->BGCMAR, (uint32_t)CLUTCfg->pCLUT);  /* 执行语句 */

    /* Write background CLUT size and CLUT color mode */
    MODIFY_REG(hdma2d->Instance->BGPFCCR, (DMA2D_BGPFCCR_CS | DMA2D_BGPFCCR_CCM),  /* 列表续行 */
               ((CLUTCfg->Size << DMA2D_BGPFCCR_CS_Pos) | (CLUTCfg->CLUTColorMode << DMA2D_BGPFCCR_CCM_Pos)));  /* 执行语句 */

    /* Enable the CLUT Transfer Complete, transfer Error, configuration Error and CLUT Access Error interrupts */
    __HAL_DMA2D_ENABLE_IT(hdma2d, DMA2D_IT_CTC | DMA2D_IT_TE | DMA2D_IT_CE | DMA2D_IT_CAE);  /* HAL辅助宏/函数 */

    /* Enable the CLUT loading for the background */
    SET_BIT(hdma2d->Instance->BGPFCCR, DMA2D_BGPFCCR_START);  /* 执行语句 */
  }                                                     /* 代码块结束 */
  /* Configure the CLUT of the foreground DMA2D layer */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Write foreground CLUT memory address */
    WRITE_REG(hdma2d->Instance->FGCMAR, (uint32_t)CLUTCfg->pCLUT);  /* 执行语句 */

    /* Write foreground CLUT size and CLUT color mode */
    MODIFY_REG(hdma2d->Instance->FGPFCCR, (DMA2D_FGPFCCR_CS | DMA2D_FGPFCCR_CCM),  /* 列表续行 */
               ((CLUTCfg->Size << DMA2D_FGPFCCR_CS_Pos) | (CLUTCfg->CLUTColorMode << DMA2D_FGPFCCR_CCM_Pos)));  /* 执行语句 */

    /* Enable the CLUT Transfer Complete, transfer Error, configuration Error and CLUT Access Error interrupts */
    __HAL_DMA2D_ENABLE_IT(hdma2d, DMA2D_IT_CTC | DMA2D_IT_TE | DMA2D_IT_CE | DMA2D_IT_CAE);  /* HAL辅助宏/函数 */

    /* Enable the CLUT loading for the foreground */
    SET_BIT(hdma2d->Instance->FGPFCCR, DMA2D_FGPFCCR_START);  /* 执行语句 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Start DMA2D CLUT Loading.
  * @param  hdma2d   Pointer to a DMA2D_HandleTypeDef structure that contains
  *                   the configuration information for the DMA2D.
  * @param  CLUTCfg  Pointer to a DMA2D_CLUTCfgTypeDef structure that contains
  *                   the configuration information for the color look up table.
  * @param  LayerIdx DMA2D Layer index.
  *                   This parameter can be one of the following values:
  *                   DMA2D_BACKGROUND_LAYER(0) / DMA2D_FOREGROUND_LAYER(1)
  * @note API obsolete and maintained for compatibility with legacy. User is
  *      invited to resort to HAL_DMA2D_CLUTStartLoad() instead to benefit from
  *      code compactness, code size and improved heap usage.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA2D_CLUTLoad(DMA2D_HandleTypeDef *hdma2d, DMA2D_CLUTCfgTypeDef CLUTCfg, uint32_t LayerIdx)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_DMA2D_LAYER(LayerIdx));               /* 执行语句 */
  assert_param(IS_DMA2D_CLUT_CM(CLUTCfg.CLUTColorMode));  /* 执行语句 */
  assert_param(IS_DMA2D_CLUT_SIZE(CLUTCfg.Size));       /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hdma2d);                                   /* HAL辅助宏/函数 */

  /* Change DMA2D peripheral state */
  hdma2d->State = HAL_DMA2D_STATE_BUSY;                 /* 赋值或初始化 */

  /* Configure the CLUT of the background DMA2D layer */
  if (LayerIdx == DMA2D_BACKGROUND_LAYER)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Write background CLUT memory address */
    WRITE_REG(hdma2d->Instance->BGCMAR, (uint32_t)CLUTCfg.pCLUT);  /* 执行语句 */

    /* Write background CLUT size and CLUT color mode */
    MODIFY_REG(hdma2d->Instance->BGPFCCR, (DMA2D_BGPFCCR_CS | DMA2D_BGPFCCR_CCM),  /* 列表续行 */
               ((CLUTCfg.Size << DMA2D_BGPFCCR_CS_Pos) | (CLUTCfg.CLUTColorMode << DMA2D_BGPFCCR_CCM_Pos)));  /* 执行语句 */

    /* Enable the CLUT loading for the background */
    SET_BIT(hdma2d->Instance->BGPFCCR, DMA2D_BGPFCCR_START);  /* 执行语句 */
  }                                                     /* 代码块结束 */
  /* Configure the CLUT of the foreground DMA2D layer */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Write foreground CLUT memory address */
    WRITE_REG(hdma2d->Instance->FGCMAR, (uint32_t)CLUTCfg.pCLUT);  /* 执行语句 */

    /* Write foreground CLUT size and CLUT color mode */
    MODIFY_REG(hdma2d->Instance->FGPFCCR, (DMA2D_FGPFCCR_CS | DMA2D_FGPFCCR_CCM),  /* 列表续行 */
               ((CLUTCfg.Size << DMA2D_FGPFCCR_CS_Pos) | (CLUTCfg.CLUTColorMode << DMA2D_FGPFCCR_CCM_Pos)));  /* 执行语句 */

    /* Enable the CLUT loading for the foreground */
    SET_BIT(hdma2d->Instance->FGPFCCR, DMA2D_FGPFCCR_START);  /* 执行语句 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Start DMA2D CLUT Loading with interrupt enabled.
  * @param  hdma2d   Pointer to a DMA2D_HandleTypeDef structure that contains
  *                   the configuration information for the DMA2D.
  * @param  CLUTCfg  Pointer to a DMA2D_CLUTCfgTypeDef structure that contains
  *                   the configuration information for the color look up table.
  * @param  LayerIdx DMA2D Layer index.
  *                   This parameter can be one of the following values:
  *                   DMA2D_BACKGROUND_LAYER(0) / DMA2D_FOREGROUND_LAYER(1)
  * @note API obsolete and maintained for compatibility with legacy. User is
  *      invited to resort to HAL_DMA2D_CLUTStartLoad_IT() instead to benefit
  *      from code compactness, code size and improved heap usage.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA2D_CLUTLoad_IT(DMA2D_HandleTypeDef *hdma2d, DMA2D_CLUTCfgTypeDef CLUTCfg, uint32_t LayerIdx)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_DMA2D_LAYER(LayerIdx));               /* 执行语句 */
  assert_param(IS_DMA2D_CLUT_CM(CLUTCfg.CLUTColorMode));  /* 执行语句 */
  assert_param(IS_DMA2D_CLUT_SIZE(CLUTCfg.Size));       /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hdma2d);                                   /* HAL辅助宏/函数 */

  /* Change DMA2D peripheral state */
  hdma2d->State = HAL_DMA2D_STATE_BUSY;                 /* 赋值或初始化 */

  /* Configure the CLUT of the background DMA2D layer */
  if (LayerIdx == DMA2D_BACKGROUND_LAYER)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Write background CLUT memory address */
    WRITE_REG(hdma2d->Instance->BGCMAR, (uint32_t)CLUTCfg.pCLUT);  /* 执行语句 */

    /* Write background CLUT size and CLUT color mode */
    MODIFY_REG(hdma2d->Instance->BGPFCCR, (DMA2D_BGPFCCR_CS | DMA2D_BGPFCCR_CCM),  /* 列表续行 */
               ((CLUTCfg.Size << DMA2D_BGPFCCR_CS_Pos) | (CLUTCfg.CLUTColorMode << DMA2D_BGPFCCR_CCM_Pos)));  /* 执行语句 */

    /* Enable the CLUT Transfer Complete, transfer Error, configuration Error and CLUT Access Error interrupts */
    __HAL_DMA2D_ENABLE_IT(hdma2d, DMA2D_IT_CTC | DMA2D_IT_TE | DMA2D_IT_CE | DMA2D_IT_CAE);  /* HAL辅助宏/函数 */

    /* Enable the CLUT loading for the background */
    SET_BIT(hdma2d->Instance->BGPFCCR, DMA2D_BGPFCCR_START);  /* 执行语句 */
  }                                                     /* 代码块结束 */
  /* Configure the CLUT of the foreground DMA2D layer */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Write foreground CLUT memory address */
    WRITE_REG(hdma2d->Instance->FGCMAR, (uint32_t)CLUTCfg.pCLUT);  /* 执行语句 */

    /* Write foreground CLUT size and CLUT color mode */
    MODIFY_REG(hdma2d->Instance->FGPFCCR, (DMA2D_FGPFCCR_CS | DMA2D_FGPFCCR_CCM),  /* 列表续行 */
               ((CLUTCfg.Size << DMA2D_FGPFCCR_CS_Pos) | (CLUTCfg.CLUTColorMode << DMA2D_FGPFCCR_CCM_Pos)));  /* 执行语句 */

    /* Enable the CLUT Transfer Complete, transfer Error, configuration Error and CLUT Access Error interrupts */
    __HAL_DMA2D_ENABLE_IT(hdma2d, DMA2D_IT_CTC | DMA2D_IT_TE | DMA2D_IT_CE | DMA2D_IT_CAE);  /* HAL辅助宏/函数 */

    /* Enable the CLUT loading for the foreground */
    SET_BIT(hdma2d->Instance->FGPFCCR, DMA2D_FGPFCCR_START);  /* 执行语句 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Abort the DMA2D CLUT loading.
  * @param  hdma2d  Pointer to a DMA2D_HandleTypeDef structure that contains
  *                  the configuration information for the DMA2D.
  * @param  LayerIdx DMA2D Layer index.
  *                   This parameter can be one of the following values:
  *                   DMA2D_BACKGROUND_LAYER(0) / DMA2D_FOREGROUND_LAYER(1)
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA2D_CLUTLoading_Abort(DMA2D_HandleTypeDef *hdma2d, uint32_t LayerIdx)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */
  const __IO uint32_t *reg =  &(hdma2d->Instance->BGPFCCR);  /* by default, point at background register */

  /* Abort the CLUT loading */
  SET_BIT(hdma2d->Instance->CR, DMA2D_CR_ABORT);        /* 执行语句 */

  /* If foreground CLUT loading is considered, update local variables */
  if (LayerIdx == DMA2D_FOREGROUND_LAYER)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    reg  = &(hdma2d->Instance->FGPFCCR);                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */


  /* Get tick */
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  /* Check if the CLUT loading is aborted */
  while ((*reg & DMA2D_BGPFCCR_START) != 0U)            /* while循环 */
  {                                                     /* 代码块开始 */
    if ((HAL_GetTick() - tickstart) > DMA2D_TIMEOUT_ABORT)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Update error code */
      hdma2d->ErrorCode |= HAL_DMA2D_ERROR_TIMEOUT;     /* 赋值或初始化 */

      /* Change the DMA2D state */
      hdma2d->State = HAL_DMA2D_STATE_TIMEOUT;          /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hdma2d);                             /* HAL辅助宏/函数 */

      return HAL_TIMEOUT;                               /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Disable the CLUT Transfer Complete, Transfer Error, Configuration Error and CLUT Access Error interrupts */
  __HAL_DMA2D_DISABLE_IT(hdma2d, DMA2D_IT_CTC | DMA2D_IT_TE | DMA2D_IT_CE | DMA2D_IT_CAE);  /* HAL辅助宏/函数 */

  /* Change the DMA2D state*/
  hdma2d->State = HAL_DMA2D_STATE_READY;                /* 赋值或初始化 */

  /* Process Unlocked */
  __HAL_UNLOCK(hdma2d);                                 /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Suspend the DMA2D CLUT loading.
  * @param  hdma2d Pointer to a DMA2D_HandleTypeDef structure that contains
  *                 the configuration information for the DMA2D.
  * @param  LayerIdx DMA2D Layer index.
  *                   This parameter can be one of the following values:
  *                   DMA2D_BACKGROUND_LAYER(0) / DMA2D_FOREGROUND_LAYER(1)
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA2D_CLUTLoading_Suspend(DMA2D_HandleTypeDef *hdma2d, uint32_t LayerIdx)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */
  uint32_t loadsuspended;                               /* 32位无符号变量/函数 */
  const __IO uint32_t *reg =  &(hdma2d->Instance->BGPFCCR);  /* by default, point at background register */

  /* Suspend the CLUT loading */
  SET_BIT(hdma2d->Instance->CR, DMA2D_CR_SUSP);         /* 执行语句 */

  /* If foreground CLUT loading is considered, update local variables */
  if (LayerIdx == DMA2D_FOREGROUND_LAYER)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    reg  = &(hdma2d->Instance->FGPFCCR);                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Get tick */
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  /* Check if the CLUT loading is suspended */
  /* 1st condition: Suspend Check */
  loadsuspended = ((hdma2d->Instance->CR & DMA2D_CR_SUSP) == DMA2D_CR_SUSP) ? 1UL : 0UL;  /* 赋值或初始化 */
  /* 2nd condition: Not Start Check */
  loadsuspended |= ((*reg & DMA2D_BGPFCCR_START) != DMA2D_BGPFCCR_START) ? 1UL : 0UL;  /* 赋值或初始化 */
  while (loadsuspended == 0UL)                          /* while循环 */
  {                                                     /* 代码块开始 */
    if ((HAL_GetTick() - tickstart) > DMA2D_TIMEOUT_SUSPEND)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Update error code */
      hdma2d->ErrorCode |= HAL_DMA2D_ERROR_TIMEOUT;     /* 赋值或初始化 */

      /* Change the DMA2D state */
      hdma2d->State = HAL_DMA2D_STATE_TIMEOUT;          /* 赋值或初始化 */

      return HAL_TIMEOUT;                               /* 返回值 */
    }                                                   /* 代码块结束 */
    /* 1st condition: Suspend Check */
    loadsuspended = ((hdma2d->Instance->CR & DMA2D_CR_SUSP) == DMA2D_CR_SUSP) ? 1UL : 0UL;  /* 赋值或初始化 */
    /* 2nd condition: Not Start Check */
    loadsuspended |= ((*reg & DMA2D_BGPFCCR_START) != DMA2D_BGPFCCR_START) ? 1UL : 0UL;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Check whether or not a transfer is actually suspended and change the DMA2D state accordingly */
  if ((*reg & DMA2D_BGPFCCR_START) != 0U)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    hdma2d->State = HAL_DMA2D_STATE_SUSPEND;            /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Make sure SUSP bit is cleared since it is meaningless
       when no transfer is on-going */
    CLEAR_BIT(hdma2d->Instance->CR, DMA2D_CR_SUSP);     /* 执行语句 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Resume the DMA2D CLUT loading.
  * @param  hdma2d pointer to a DMA2D_HandleTypeDef structure that contains
  *                 the configuration information for the DMA2D.
  * @param  LayerIdx DMA2D Layer index.
  *                   This parameter can be one of the following values:
  *                   DMA2D_BACKGROUND_LAYER(0) / DMA2D_FOREGROUND_LAYER(1)
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA2D_CLUTLoading_Resume(DMA2D_HandleTypeDef *hdma2d, uint32_t LayerIdx)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the SUSP and START bits for background or foreground CLUT loading */
  if (LayerIdx == DMA2D_BACKGROUND_LAYER)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Background CLUT loading suspension check */
    if ((hdma2d->Instance->CR & DMA2D_CR_SUSP) == DMA2D_CR_SUSP)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      if ((hdma2d->Instance->BGPFCCR & DMA2D_BGPFCCR_START) == DMA2D_BGPFCCR_START)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Ongoing CLUT loading is suspended: change the DMA2D state before resuming */
        hdma2d->State = HAL_DMA2D_STATE_BUSY;           /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Foreground CLUT loading suspension check */
    if ((hdma2d->Instance->CR & DMA2D_CR_SUSP) == DMA2D_CR_SUSP)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      if ((hdma2d->Instance->FGPFCCR & DMA2D_FGPFCCR_START) == DMA2D_FGPFCCR_START)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Ongoing CLUT loading is suspended: change the DMA2D state before resuming */
        hdma2d->State = HAL_DMA2D_STATE_BUSY;           /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Resume the CLUT loading */
  CLEAR_BIT(hdma2d->Instance->CR, DMA2D_CR_SUSP);       /* 执行语句 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */


/**

  * @brief  Polling for transfer complete or CLUT loading.
  * @param  hdma2d Pointer to a DMA2D_HandleTypeDef structure that contains
  *                 the configuration information for the DMA2D.
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA2D_PollForTransfer(DMA2D_HandleTypeDef *hdma2d, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */
  uint32_t layer_start;                                 /* 32位无符号变量/函数 */
  __IO uint32_t isrflags = 0x0U;                        /* 赋值或初始化 */

  /* Polling for DMA2D transfer */
  if ((hdma2d->Instance->CR & DMA2D_CR_START) != 0U)    /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Get tick */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */

    while (__HAL_DMA2D_GET_FLAG(hdma2d, DMA2D_FLAG_TC) == 0U)  /* while循环 */
    {                                                   /* 代码块开始 */
      isrflags = READ_REG(hdma2d->Instance->ISR);       /* 赋值或初始化 */
      if ((isrflags & (DMA2D_FLAG_CE | DMA2D_FLAG_TE)) != 0U)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        if ((isrflags & DMA2D_FLAG_CE) != 0U)           /* 条件判断 */
        {                                               /* 代码块开始 */
          hdma2d->ErrorCode |= HAL_DMA2D_ERROR_CE;      /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        if ((isrflags & DMA2D_FLAG_TE) != 0U)           /* 条件判断 */
        {                                               /* 代码块开始 */
          hdma2d->ErrorCode |= HAL_DMA2D_ERROR_TE;      /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        /* Clear the transfer and configuration error flags */
        __HAL_DMA2D_CLEAR_FLAG(hdma2d, DMA2D_FLAG_CE | DMA2D_FLAG_TE);  /* HAL辅助宏/函数 */

        /* Change DMA2D state */
        hdma2d->State = HAL_DMA2D_STATE_ERROR;          /* 赋值或初始化 */

        /* Process unlocked */
        __HAL_UNLOCK(hdma2d);                           /* HAL辅助宏/函数 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
      /* Check for the Timeout */
      if (Timeout != HAL_MAX_DELAY)                     /* 条件判断 */
      {                                                 /* 代码块开始 */
        if (((HAL_GetTick() - tickstart) > Timeout) || (Timeout == 0U))  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Update error code */
          hdma2d->ErrorCode |= HAL_DMA2D_ERROR_TIMEOUT;  /* 赋值或初始化 */

          /* Change the DMA2D state */
          hdma2d->State = HAL_DMA2D_STATE_TIMEOUT;      /* 赋值或初始化 */

          /* Process unlocked */
          __HAL_UNLOCK(hdma2d);                         /* HAL辅助宏/函数 */

          return HAL_TIMEOUT;                           /* 返回值 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /* Polling for CLUT loading (foreground or background) */
  layer_start = hdma2d->Instance->FGPFCCR & DMA2D_FGPFCCR_START;  /* 赋值或初始化 */
  layer_start |= hdma2d->Instance->BGPFCCR & DMA2D_BGPFCCR_START;  /* 赋值或初始化 */
  if (layer_start != 0U)                                /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Get tick */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */

    while (__HAL_DMA2D_GET_FLAG(hdma2d, DMA2D_FLAG_CTC) == 0U)  /* while循环 */
    {                                                   /* 代码块开始 */
      isrflags = READ_REG(hdma2d->Instance->ISR);       /* 赋值或初始化 */
      if ((isrflags & (DMA2D_FLAG_CAE | DMA2D_FLAG_CE | DMA2D_FLAG_TE)) != 0U)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        if ((isrflags & DMA2D_FLAG_CAE) != 0U)          /* 条件判断 */
        {                                               /* 代码块开始 */
          hdma2d->ErrorCode |= HAL_DMA2D_ERROR_CAE;     /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        if ((isrflags & DMA2D_FLAG_CE) != 0U)           /* 条件判断 */
        {                                               /* 代码块开始 */
          hdma2d->ErrorCode |= HAL_DMA2D_ERROR_CE;      /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        if ((isrflags & DMA2D_FLAG_TE) != 0U)           /* 条件判断 */
        {                                               /* 代码块开始 */
          hdma2d->ErrorCode |= HAL_DMA2D_ERROR_TE;      /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        /* Clear the CLUT Access Error, Configuration Error and Transfer Error flags */
        __HAL_DMA2D_CLEAR_FLAG(hdma2d, DMA2D_FLAG_CAE | DMA2D_FLAG_CE | DMA2D_FLAG_TE);  /* HAL辅助宏/函数 */

        /* Change DMA2D state */
        hdma2d->State = HAL_DMA2D_STATE_ERROR;          /* 赋值或初始化 */

        /* Process unlocked */
        __HAL_UNLOCK(hdma2d);                           /* HAL辅助宏/函数 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
      /* Check for the Timeout */
      if (Timeout != HAL_MAX_DELAY)                     /* 条件判断 */
      {                                                 /* 代码块开始 */
        if (((HAL_GetTick() - tickstart) > Timeout) || (Timeout == 0U))  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Update error code */
          hdma2d->ErrorCode |= HAL_DMA2D_ERROR_TIMEOUT;  /* 赋值或初始化 */

          /* Change the DMA2D state */
          hdma2d->State = HAL_DMA2D_STATE_TIMEOUT;      /* 赋值或初始化 */

          /* Process unlocked */
          __HAL_UNLOCK(hdma2d);                         /* HAL辅助宏/函数 */

          return HAL_TIMEOUT;                           /* 返回值 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Clear the transfer complete and CLUT loading flags */
  __HAL_DMA2D_CLEAR_FLAG(hdma2d, DMA2D_FLAG_TC | DMA2D_FLAG_CTC);  /* HAL辅助宏/函数 */

  /* Change DMA2D state */
  hdma2d->State = HAL_DMA2D_STATE_READY;                /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hdma2d);                                 /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */
/**
  * @brief  Handle DMA2D interrupt request.
  * @param  hdma2d Pointer to a DMA2D_HandleTypeDef structure that contains
  *                 the configuration information for the DMA2D.
  * @retval HAL status
  */
void HAL_DMA2D_IRQHandler(DMA2D_HandleTypeDef *hdma2d)  /* 中断服务函数 */
{                                                       /* 代码块开始 */
  uint32_t isrflags = READ_REG(hdma2d->Instance->ISR);  /* 32位无符号变量/函数 */
  uint32_t crflags = READ_REG(hdma2d->Instance->CR);    /* 32位无符号变量/函数 */

  /* Transfer Error Interrupt management ***************************************/
  if ((isrflags & DMA2D_FLAG_TE) != 0U)                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((crflags & DMA2D_IT_TE) != 0U)                  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable the transfer Error interrupt */
      __HAL_DMA2D_DISABLE_IT(hdma2d, DMA2D_IT_TE);      /* HAL辅助宏/函数 */

      /* Update error code */
      hdma2d->ErrorCode |= HAL_DMA2D_ERROR_TE;          /* 赋值或初始化 */

      /* Clear the transfer error flag */
      __HAL_DMA2D_CLEAR_FLAG(hdma2d, DMA2D_FLAG_TE);    /* HAL辅助宏/函数 */

      /* Change DMA2D state */
      hdma2d->State = HAL_DMA2D_STATE_ERROR;            /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hdma2d);                             /* HAL辅助宏/函数 */

      if (hdma2d->XferErrorCallback != NULL)            /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Transfer error Callback */
        hdma2d->XferErrorCallback(hdma2d);              /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /* Configuration Error Interrupt management **********************************/
  if ((isrflags & DMA2D_FLAG_CE) != 0U)                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((crflags & DMA2D_IT_CE) != 0U)                  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable the Configuration Error interrupt */
      __HAL_DMA2D_DISABLE_IT(hdma2d, DMA2D_IT_CE);      /* HAL辅助宏/函数 */

      /* Clear the Configuration error flag */
      __HAL_DMA2D_CLEAR_FLAG(hdma2d, DMA2D_FLAG_CE);    /* HAL辅助宏/函数 */

      /* Update error code */
      hdma2d->ErrorCode |= HAL_DMA2D_ERROR_CE;          /* 赋值或初始化 */

      /* Change DMA2D state */
      hdma2d->State = HAL_DMA2D_STATE_ERROR;            /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hdma2d);                             /* HAL辅助宏/函数 */

      if (hdma2d->XferErrorCallback != NULL)            /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Transfer error Callback */
        hdma2d->XferErrorCallback(hdma2d);              /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /* CLUT access Error Interrupt management ***********************************/
  if ((isrflags & DMA2D_FLAG_CAE) != 0U)                /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((crflags & DMA2D_IT_CAE) != 0U)                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable the CLUT access error interrupt */
      __HAL_DMA2D_DISABLE_IT(hdma2d, DMA2D_IT_CAE);     /* HAL辅助宏/函数 */

      /* Clear the CLUT access error flag */
      __HAL_DMA2D_CLEAR_FLAG(hdma2d, DMA2D_FLAG_CAE);   /* HAL辅助宏/函数 */

      /* Update error code */
      hdma2d->ErrorCode |= HAL_DMA2D_ERROR_CAE;         /* 赋值或初始化 */

      /* Change DMA2D state */
      hdma2d->State = HAL_DMA2D_STATE_ERROR;            /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hdma2d);                             /* HAL辅助宏/函数 */

      if (hdma2d->XferErrorCallback != NULL)            /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Transfer error Callback */
        hdma2d->XferErrorCallback(hdma2d);              /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /* Transfer watermark Interrupt management **********************************/
  if ((isrflags & DMA2D_FLAG_TW) != 0U)                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((crflags & DMA2D_IT_TW) != 0U)                  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable the transfer watermark interrupt */
      __HAL_DMA2D_DISABLE_IT(hdma2d, DMA2D_IT_TW);      /* HAL辅助宏/函数 */

      /* Clear the transfer watermark flag */
      __HAL_DMA2D_CLEAR_FLAG(hdma2d, DMA2D_FLAG_TW);    /* HAL辅助宏/函数 */

      /* Transfer watermark Callback */
#if (USE_HAL_DMA2D_REGISTER_CALLBACKS == 1)             /* 条件编译 */
      hdma2d->LineEventCallback(hdma2d);                /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_DMA2D_LineEventCallback(hdma2d);              /* HAL库函数调用 */
#endif /* USE_HAL_DMA2D_REGISTER_CALLBACKS */

    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /* Transfer Complete Interrupt management ************************************/
  if ((isrflags & DMA2D_FLAG_TC) != 0U)                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((crflags & DMA2D_IT_TC) != 0U)                  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable the transfer complete interrupt */
      __HAL_DMA2D_DISABLE_IT(hdma2d, DMA2D_IT_TC);      /* HAL辅助宏/函数 */

      /* Clear the transfer complete flag */
      __HAL_DMA2D_CLEAR_FLAG(hdma2d, DMA2D_FLAG_TC);    /* HAL辅助宏/函数 */

      /* Update error code */
      hdma2d->ErrorCode |= HAL_DMA2D_ERROR_NONE;        /* 赋值或初始化 */

      /* Change DMA2D state */
      hdma2d->State = HAL_DMA2D_STATE_READY;            /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hdma2d);                             /* HAL辅助宏/函数 */

      if (hdma2d->XferCpltCallback != NULL)             /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Transfer complete Callback */
        hdma2d->XferCpltCallback(hdma2d);               /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /* CLUT Transfer Complete Interrupt management ******************************/
  if ((isrflags & DMA2D_FLAG_CTC) != 0U)                /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((crflags & DMA2D_IT_CTC) != 0U)                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable the CLUT transfer complete interrupt */
      __HAL_DMA2D_DISABLE_IT(hdma2d, DMA2D_IT_CTC);     /* HAL辅助宏/函数 */

      /* Clear the CLUT transfer complete flag */
      __HAL_DMA2D_CLEAR_FLAG(hdma2d, DMA2D_FLAG_CTC);   /* HAL辅助宏/函数 */

      /* Update error code */
      hdma2d->ErrorCode |= HAL_DMA2D_ERROR_NONE;        /* 赋值或初始化 */

      /* Change DMA2D state */
      hdma2d->State = HAL_DMA2D_STATE_READY;            /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hdma2d);                             /* HAL辅助宏/函数 */

      /* CLUT Transfer complete Callback */
#if (USE_HAL_DMA2D_REGISTER_CALLBACKS == 1)             /* 条件编译 */
      hdma2d->CLUTLoadingCpltCallback(hdma2d);          /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_DMA2D_CLUTLoadingCpltCallback(hdma2d);        /* HAL库函数调用 */
#endif /* USE_HAL_DMA2D_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

}                                                       /* 代码块结束 */

/**
  * @brief  Transfer watermark callback.
  * @param  hdma2d pointer to a DMA2D_HandleTypeDef structure that contains
  *                 the configuration information for the DMA2D.
  * @retval None
  */
__weak void HAL_DMA2D_LineEventCallback(DMA2D_HandleTypeDef *hdma2d)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdma2d);                                       /* 执行语句 */

  /* NOTE : This function should not be modified; when the callback is needed,
            the HAL_DMA2D_LineEventCallback can be implemented in the user file.
   */
}                                                       /* 代码块结束 */

/**
  * @brief  CLUT Transfer Complete callback.
  * @param  hdma2d pointer to a DMA2D_HandleTypeDef structure that contains
  *                 the configuration information for the DMA2D.
  * @retval None
  */
__weak void HAL_DMA2D_CLUTLoadingCpltCallback(DMA2D_HandleTypeDef *hdma2d)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdma2d);                                       /* 执行语句 */

  /* NOTE : This function should not be modified; when the callback is needed,
            the HAL_DMA2D_CLUTLoadingCpltCallback can be implemented in the user file.
   */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup DMA2D_Exported_Functions_Group3 Peripheral Control functions
  *  @brief    Peripheral Control functions
  *
@verbatim
 ===============================================================================
                    ##### Peripheral Control functions #####
 ===============================================================================
    [..]  This section provides functions allowing to:
      (+) Configure the DMA2D foreground or background layer parameters.
      (+) Configure the DMA2D CLUT transfer.
      (+) Configure the line watermark
      (+) Configure the dead time value.
      (+) Enable or disable the dead time value functionality.


@endverbatim
  * @{
  */

/**
  * @brief  Configure the DMA2D Layer according to the specified
  *         parameters in the DMA2D_HandleTypeDef.
  * @param  hdma2d Pointer to a DMA2D_HandleTypeDef structure that contains
  *                 the configuration information for the DMA2D.
  * @param  LayerIdx DMA2D Layer index.
  *                   This parameter can be one of the following values:
  *                   DMA2D_BACKGROUND_LAYER(0) / DMA2D_FOREGROUND_LAYER(1)
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA2D_ConfigLayer(DMA2D_HandleTypeDef *hdma2d, uint32_t LayerIdx)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  DMA2D_LayerCfgTypeDef *pLayerCfg;                     /* 执行语句 */
  uint32_t regMask;                                     /* 32位无符号变量/函数 */
  uint32_t regValue;                                    /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_DMA2D_LAYER(LayerIdx));               /* 执行语句 */
  assert_param(IS_DMA2D_OFFSET(hdma2d->LayerCfg[LayerIdx].InputOffset));  /* 执行语句 */
  if (hdma2d->Init.Mode != DMA2D_R2M)                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    assert_param(IS_DMA2D_INPUT_COLOR_MODE(hdma2d->LayerCfg[LayerIdx].InputColorMode));  /* 执行语句 */
    if (hdma2d->Init.Mode != DMA2D_M2M)                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      assert_param(IS_DMA2D_ALPHA_MODE(hdma2d->LayerCfg[LayerIdx].AlphaMode));  /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Process locked */
  __HAL_LOCK(hdma2d);                                   /* HAL辅助宏/函数 */

  /* Change DMA2D peripheral state */
  hdma2d->State = HAL_DMA2D_STATE_BUSY;                 /* 赋值或初始化 */

  pLayerCfg = &hdma2d->LayerCfg[LayerIdx];              /* 赋值或初始化 */

  /* Prepare the value to be written to the BGPFCCR or FGPFCCR register */
  regValue = pLayerCfg->InputColorMode | (pLayerCfg->AlphaMode << DMA2D_BGPFCCR_AM_Pos);  /* 赋值或初始化 */
  regMask  = DMA2D_BGPFCCR_CM | DMA2D_BGPFCCR_AM | DMA2D_BGPFCCR_ALPHA;  /* 赋值或初始化 */


  if ((pLayerCfg->InputColorMode == DMA2D_INPUT_A4) || (pLayerCfg->InputColorMode == DMA2D_INPUT_A8))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    regValue |= (pLayerCfg->InputAlpha & DMA2D_BGPFCCR_ALPHA);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    regValue |= (pLayerCfg->InputAlpha << DMA2D_BGPFCCR_ALPHA_Pos);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Configure the background DMA2D layer */
  if (LayerIdx == DMA2D_BACKGROUND_LAYER)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Write DMA2D BGPFCCR register */
    MODIFY_REG(hdma2d->Instance->BGPFCCR, regMask, regValue);  /* 执行语句 */

    /* DMA2D BGOR register configuration -------------------------------------*/
    WRITE_REG(hdma2d->Instance->BGOR, pLayerCfg->InputOffset);  /* 执行语句 */

    /* DMA2D BGCOLR register configuration -------------------------------------*/
    if ((pLayerCfg->InputColorMode == DMA2D_INPUT_A4) || (pLayerCfg->InputColorMode == DMA2D_INPUT_A8))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      WRITE_REG(hdma2d->Instance->BGCOLR, pLayerCfg->InputAlpha & (DMA2D_BGCOLR_BLUE | DMA2D_BGCOLR_GREEN | \
                                                                   DMA2D_BGCOLR_RED));  /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /* Configure the foreground DMA2D layer */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */


    /* Write DMA2D FGPFCCR register */
    MODIFY_REG(hdma2d->Instance->FGPFCCR, regMask, regValue);  /* 执行语句 */

    /* DMA2D FGOR register configuration -------------------------------------*/
    WRITE_REG(hdma2d->Instance->FGOR, pLayerCfg->InputOffset);  /* 执行语句 */

    /* DMA2D FGCOLR register configuration -------------------------------------*/
    if ((pLayerCfg->InputColorMode == DMA2D_INPUT_A4) || (pLayerCfg->InputColorMode == DMA2D_INPUT_A8))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      WRITE_REG(hdma2d->Instance->FGCOLR, pLayerCfg->InputAlpha & (DMA2D_FGCOLR_BLUE | DMA2D_FGCOLR_GREEN | \
                                                                   DMA2D_FGCOLR_RED));  /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /* Initialize the DMA2D state*/
  hdma2d->State = HAL_DMA2D_STATE_READY;                /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hdma2d);                                 /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configure the DMA2D CLUT Transfer.
  * @param  hdma2d   Pointer to a DMA2D_HandleTypeDef structure that contains
  *                   the configuration information for the DMA2D.
  * @param  CLUTCfg  Pointer to a DMA2D_CLUTCfgTypeDef structure that contains
  *                   the configuration information for the color look up table.
  * @param  LayerIdx DMA2D Layer index.
  *                   This parameter can be one of the following values:
  *                   DMA2D_BACKGROUND_LAYER(0) / DMA2D_FOREGROUND_LAYER(1)
  * @note API obsolete and maintained for compatibility with legacy. User is invited
  *      to resort to HAL_DMA2D_CLUTStartLoad() instead to benefit from code compactness,
  *      code size and improved heap usage.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA2D_ConfigCLUT(DMA2D_HandleTypeDef *hdma2d, DMA2D_CLUTCfgTypeDef CLUTCfg, uint32_t LayerIdx)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_DMA2D_LAYER(LayerIdx));               /* 执行语句 */
  assert_param(IS_DMA2D_CLUT_CM(CLUTCfg.CLUTColorMode));  /* 执行语句 */
  assert_param(IS_DMA2D_CLUT_SIZE(CLUTCfg.Size));       /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hdma2d);                                   /* HAL辅助宏/函数 */

  /* Change DMA2D peripheral state */
  hdma2d->State = HAL_DMA2D_STATE_BUSY;                 /* 赋值或初始化 */

  /* Configure the CLUT of the background DMA2D layer */
  if (LayerIdx == DMA2D_BACKGROUND_LAYER)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Write background CLUT memory address */
    WRITE_REG(hdma2d->Instance->BGCMAR, (uint32_t)CLUTCfg.pCLUT);  /* 执行语句 */

    /* Write background CLUT size and CLUT color mode */
    MODIFY_REG(hdma2d->Instance->BGPFCCR, (DMA2D_BGPFCCR_CS | DMA2D_BGPFCCR_CCM),  /* 列表续行 */
               ((CLUTCfg.Size << DMA2D_BGPFCCR_CS_Pos) | (CLUTCfg.CLUTColorMode << DMA2D_BGPFCCR_CCM_Pos)));  /* 执行语句 */
  }                                                     /* 代码块结束 */
  /* Configure the CLUT of the foreground DMA2D layer */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Write foreground CLUT memory address */
    WRITE_REG(hdma2d->Instance->FGCMAR, (uint32_t)CLUTCfg.pCLUT);  /* 执行语句 */

    /* Write foreground CLUT size and CLUT color mode */
    MODIFY_REG(hdma2d->Instance->FGPFCCR, (DMA2D_FGPFCCR_CS | DMA2D_FGPFCCR_CCM),  /* 列表续行 */
               ((CLUTCfg.Size << DMA2D_FGPFCCR_CS_Pos) | (CLUTCfg.CLUTColorMode << DMA2D_FGPFCCR_CCM_Pos)));  /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Set the DMA2D state to Ready*/
  hdma2d->State = HAL_DMA2D_STATE_READY;                /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hdma2d);                                 /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */


/**
  * @brief  Configure the line watermark.
  * @param  hdma2d Pointer to a DMA2D_HandleTypeDef structure that contains
  *                 the configuration information for the DMA2D.
  * @param  Line   Line Watermark configuration (maximum 16-bit long value expected).
  * @note   HAL_DMA2D_ProgramLineEvent() API enables the transfer watermark interrupt.
  * @note   The transfer watermark interrupt is disabled once it has occurred.
  * @retval HAL status
  */

HAL_StatusTypeDef HAL_DMA2D_ProgramLineEvent(DMA2D_HandleTypeDef *hdma2d, uint32_t Line)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  if (Line > DMA2D_LWR_LW)                              /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Process locked */
    __HAL_LOCK(hdma2d);                                 /* HAL辅助宏/函数 */

    /* Change DMA2D peripheral state */
    hdma2d->State = HAL_DMA2D_STATE_BUSY;               /* 赋值或初始化 */

    /* Sets the Line watermark configuration */
    WRITE_REG(hdma2d->Instance->LWR, Line);             /* 执行语句 */

    /* Enable the Line interrupt */
    __HAL_DMA2D_ENABLE_IT(hdma2d, DMA2D_IT_TW);         /* HAL辅助宏/函数 */

    /* Initialize the DMA2D state*/
    hdma2d->State = HAL_DMA2D_STATE_READY;              /* 赋值或初始化 */

    /* Process unlocked */
    __HAL_UNLOCK(hdma2d);                               /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief Enable DMA2D dead time feature.
  * @param hdma2d DMA2D handle.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA2D_EnableDeadTime(DMA2D_HandleTypeDef *hdma2d)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Process Locked */
  __HAL_LOCK(hdma2d);                                   /* HAL辅助宏/函数 */

  hdma2d->State = HAL_DMA2D_STATE_BUSY;                 /* 赋值或初始化 */

  /* Set DMA2D_AMTCR EN bit */
  SET_BIT(hdma2d->Instance->AMTCR, DMA2D_AMTCR_EN);     /* 执行语句 */

  hdma2d->State = HAL_DMA2D_STATE_READY;                /* 赋值或初始化 */

  /* Process Unlocked */
  __HAL_UNLOCK(hdma2d);                                 /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief Disable DMA2D dead time feature.
  * @param hdma2d DMA2D handle.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA2D_DisableDeadTime(DMA2D_HandleTypeDef *hdma2d)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Process Locked */
  __HAL_LOCK(hdma2d);                                   /* HAL辅助宏/函数 */

  hdma2d->State = HAL_DMA2D_STATE_BUSY;                 /* 赋值或初始化 */

  /* Clear DMA2D_AMTCR EN bit */
  CLEAR_BIT(hdma2d->Instance->AMTCR, DMA2D_AMTCR_EN);   /* 执行语句 */

  hdma2d->State = HAL_DMA2D_STATE_READY;                /* 赋值或初始化 */

  /* Process Unlocked */
  __HAL_UNLOCK(hdma2d);                                 /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief Configure dead time.
  * @note The dead time value represents the guaranteed minimum number of cycles between
  *       two consecutive transactions on the AHB bus.
  * @param hdma2d DMA2D handle.
  * @param DeadTime dead time value.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA2D_ConfigDeadTime(DMA2D_HandleTypeDef *hdma2d, uint8_t DeadTime)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Process Locked */
  __HAL_LOCK(hdma2d);                                   /* HAL辅助宏/函数 */

  hdma2d->State = HAL_DMA2D_STATE_BUSY;                 /* 赋值或初始化 */

  /* Set DMA2D_AMTCR DT field */
  MODIFY_REG(hdma2d->Instance->AMTCR, DMA2D_AMTCR_DT, (((uint32_t) DeadTime) << DMA2D_AMTCR_DT_Pos));  /* 执行语句 */

  hdma2d->State = HAL_DMA2D_STATE_READY;                /* 赋值或初始化 */

  /* Process Unlocked */
  __HAL_UNLOCK(hdma2d);                                 /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */


/** @defgroup DMA2D_Exported_Functions_Group4 Peripheral State and Error functions
  *  @brief    Peripheral State functions
  *
@verbatim
 ===============================================================================
                  ##### Peripheral State and Errors functions #####
 ===============================================================================
    [..]
    This subsection provides functions allowing to:
      (+) Get the DMA2D state
      (+) Get the DMA2D error code

@endverbatim
  * @{
  */

/**
  * @brief  Return the DMA2D state
  * @param  hdma2d pointer to a DMA2D_HandleTypeDef structure that contains
  *                 the configuration information for the DMA2D.
  * @retval HAL state
  */
HAL_DMA2D_StateTypeDef HAL_DMA2D_GetState(DMA2D_HandleTypeDef *hdma2d)  /* HAL库函数调用 */
{                                                       /* 代码块开始 */
  return hdma2d->State;                                 /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return the DMA2D error code
  * @param  hdma2d  pointer to a DMA2D_HandleTypeDef structure that contains
  *               the configuration information for DMA2D.
  * @retval DMA2D Error Code
  */
uint32_t HAL_DMA2D_GetError(DMA2D_HandleTypeDef *hdma2d)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  return hdma2d->ErrorCode;                             /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/**
  * @}
  */


/** @defgroup DMA2D_Private_Functions DMA2D Private Functions
  * @{
  */

/**
  * @brief  Set the DMA2D transfer parameters.
  * @param  hdma2d     Pointer to a DMA2D_HandleTypeDef structure that contains
  *                     the configuration information for the specified DMA2D.
  * @param  pdata      The source memory Buffer address
  * @param  DstAddress The destination memory Buffer address
  * @param  Width      The width of data to be transferred from source to destination.
  * @param  Height     The height of data to be transferred from source to destination.
  * @retval HAL status
  */
static void DMA2D_SetConfig(DMA2D_HandleTypeDef *hdma2d, uint32_t pdata, uint32_t DstAddress, uint32_t Width,  /* 静态函数 */
                            uint32_t Height)            /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t tmp;                                         /* 32位无符号变量/函数 */
  uint32_t tmp1;                                        /* 32位无符号变量/函数 */
  uint32_t tmp2;                                        /* 32位无符号变量/函数 */
  uint32_t tmp3;                                        /* 32位无符号变量/函数 */
  uint32_t tmp4;                                        /* 32位无符号变量/函数 */

  /* Configure DMA2D data size */
  MODIFY_REG(hdma2d->Instance->NLR, (DMA2D_NLR_NL | DMA2D_NLR_PL), (Height | (Width << DMA2D_NLR_PL_Pos)));  /* 执行语句 */

  /* Configure DMA2D destination address */
  WRITE_REG(hdma2d->Instance->OMAR, DstAddress);        /* 执行语句 */

  /* Register to memory DMA2D mode selected */
  if (hdma2d->Init.Mode == DMA2D_R2M)                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    tmp1 = pdata & DMA2D_OCOLR_ALPHA_1;                 /* 赋值或初始化 */
    tmp2 = pdata & DMA2D_OCOLR_RED_1;                   /* 赋值或初始化 */
    tmp3 = pdata & DMA2D_OCOLR_GREEN_1;                 /* 赋值或初始化 */
    tmp4 = pdata & DMA2D_OCOLR_BLUE_1;                  /* 赋值或初始化 */

    /* Prepare the value to be written to the OCOLR register according to the color mode */
    if (hdma2d->Init.ColorMode == DMA2D_OUTPUT_ARGB8888)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      tmp = (tmp3 | tmp2 | tmp1 | tmp4);                /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if (hdma2d->Init.ColorMode == DMA2D_OUTPUT_RGB888)  /* 否则如果 */
    {                                                   /* 代码块开始 */
      tmp = (tmp3 | tmp2 | tmp4);                       /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if (hdma2d->Init.ColorMode == DMA2D_OUTPUT_RGB565)  /* 否则如果 */
    {                                                   /* 代码块开始 */
      tmp2 = (tmp2 >> 19U);                             /* 赋值或初始化 */
      tmp3 = (tmp3 >> 10U);                             /* 赋值或初始化 */
      tmp4 = (tmp4 >> 3U);                              /* 赋值或初始化 */
      tmp  = ((tmp3 << 5U) | (tmp2 << 11U) | tmp4);     /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if (hdma2d->Init.ColorMode == DMA2D_OUTPUT_ARGB1555)  /* 否则如果 */
    {                                                   /* 代码块开始 */
      tmp1 = (tmp1 >> 31U);                             /* 赋值或初始化 */
      tmp2 = (tmp2 >> 19U);                             /* 赋值或初始化 */
      tmp3 = (tmp3 >> 11U);                             /* 赋值或初始化 */
      tmp4 = (tmp4 >> 3U);                              /* 赋值或初始化 */
      tmp  = ((tmp3 << 5U) | (tmp2 << 10U) | (tmp1 << 15U) | tmp4);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else /* Dhdma2d->Init.ColorMode = DMA2D_OUTPUT_ARGB4444 */
    {                                                   /* 代码块开始 */
      tmp1 = (tmp1 >> 28U);                             /* 赋值或初始化 */
      tmp2 = (tmp2 >> 20U);                             /* 赋值或初始化 */
      tmp3 = (tmp3 >> 12U);                             /* 赋值或初始化 */
      tmp4 = (tmp4 >> 4U);                              /* 赋值或初始化 */
      tmp  = ((tmp3 << 4U) | (tmp2 << 8U) | (tmp1 << 12U) | tmp4);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    /* Write to DMA2D OCOLR register */
    WRITE_REG(hdma2d->Instance->OCOLR, tmp);            /* 执行语句 */
  }                                                     /* 代码块结束 */
  else /* M2M, M2M_PFC or M2M_Blending DMA2D Mode */
  {                                                     /* 代码块开始 */
    /* Configure DMA2D source address */
    WRITE_REG(hdma2d->Instance->FGMAR, pdata);          /* 执行语句 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
#endif /* DMA2D */
#endif /* HAL_DMA2D_MODULE_ENABLED */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
