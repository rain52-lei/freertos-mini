/* 文件: stm32f4xx_hal_dcmi_ex.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_dcmi_ex.c
  * @author  MCD Application Team
  * @brief   DCMI Extension HAL module driver
  *          This file provides firmware functions to manage the following
  *          functionalities of DCMI extension peripheral:
  *           + Extension features functions
  *
  @verbatim
  ==============================================================================
               ##### DCMI peripheral extension features  #####
  ==============================================================================

  [..] Comparing to other previous devices, the DCMI interface for STM32F446xx
       devices contains the following additional features :

       (+) Support of Black and White cameras

                     ##### How to use this driver #####
  ==============================================================================
  [..] This driver provides functions to manage the Black and White feature

  @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
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
/** @defgroup DCMIEx DCMIEx
  * @brief DCMI Extended HAL module driver
  * @{
  */

#ifdef HAL_DCMI_MODULE_ENABLED                          /* 条件编译 */

#if defined(STM32F407xx) || defined(STM32F417xx) || defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) ||\
    defined(STM32F439xx) || defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx)  /* 源代码 */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/** @defgroup DCMIEx_Exported_Functions DCMI Extended Exported Functions
  * @{
  */

/**
  * @}
  */

/** @addtogroup DCMI_Exported_Functions_Group1 Initialization and Configuration functions
  * @{
  */

/**
  * @brief  Initializes the DCMI according to the specified
  *         parameters in the DCMI_InitTypeDef and create the associated handle.
  * @param  hdcmi pointer to a DCMI_HandleTypeDef structure that contains
  *                the configuration information for DCMI.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DCMI_Init(DCMI_HandleTypeDef *hdcmi)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the DCMI peripheral state */
  if(hdcmi == NULL)                                     /* 条件判断 */
  {                                                     /* 代码块开始 */
     return HAL_ERROR;                                  /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check function parameters */
  assert_param(IS_DCMI_ALL_INSTANCE(hdcmi->Instance));  /* 执行语句 */
  assert_param(IS_DCMI_PCKPOLARITY(hdcmi->Init.PCKPolarity));  /* 执行语句 */
  assert_param(IS_DCMI_VSPOLARITY(hdcmi->Init.VSPolarity));  /* 执行语句 */
  assert_param(IS_DCMI_HSPOLARITY(hdcmi->Init.HSPolarity));  /* 执行语句 */
  assert_param(IS_DCMI_SYNCHRO(hdcmi->Init.SynchroMode));  /* 执行语句 */
  assert_param(IS_DCMI_CAPTURE_RATE(hdcmi->Init.CaptureRate));  /* 执行语句 */
  assert_param(IS_DCMI_EXTENDED_DATA(hdcmi->Init.ExtendedDataMode));  /* 执行语句 */
  assert_param(IS_DCMI_MODE_JPEG(hdcmi->Init.JPEGMode));  /* 执行语句 */
#if defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx)  /* 条件编译 */
  assert_param(IS_DCMI_BYTE_SELECT_MODE(hdcmi->Init.ByteSelectMode));  /* 执行语句 */
  assert_param(IS_DCMI_BYTE_SELECT_START(hdcmi->Init.ByteSelectStart));  /* 执行语句 */
  assert_param(IS_DCMI_LINE_SELECT_MODE(hdcmi->Init.LineSelectMode));  /* 执行语句 */
  assert_param(IS_DCMI_LINE_SELECT_START(hdcmi->Init.LineSelectStart));  /* 执行语句 */
#endif /* STM32F446xx || STM32F469xx || STM32F479xx */
  if(hdcmi->State == HAL_DCMI_STATE_RESET)              /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Allocate lock resource and initialize it */
    hdcmi->Lock = HAL_UNLOCKED;                         /* 赋值或初始化 */
    /* Init the low level hardware */
  /* Init the DCMI Callback settings */
#if (USE_HAL_DCMI_REGISTER_CALLBACKS == 1)              /* 条件编译 */
    hdcmi->FrameEventCallback = HAL_DCMI_FrameEventCallback; /* Legacy weak FrameEventCallback  */
    hdcmi->VsyncEventCallback = HAL_DCMI_VsyncEventCallback; /* Legacy weak VsyncEventCallback  */
    hdcmi->LineEventCallback  = HAL_DCMI_LineEventCallback;  /* Legacy weak LineEventCallback   */
    hdcmi->ErrorCallback      = HAL_DCMI_ErrorCallback;      /* Legacy weak ErrorCallback       */

    if(hdcmi->MspInitCallback == NULL)                  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Legacy weak MspInit Callback        */
      hdcmi->MspInitCallback = HAL_DCMI_MspInit;        /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    /* Initialize the low level hardware (MSP) */
    hdcmi->MspInitCallback(hdcmi);                      /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /* Init the low level hardware : GPIO, CLOCK, NVIC and DMA */
    HAL_DCMI_MspInit(hdcmi);                            /* HAL库函数调用 */
#endif /* (USE_HAL_DCMI_REGISTER_CALLBACKS) */
    HAL_DCMI_MspInit(hdcmi);                            /* HAL库函数调用 */
  }                                                     /* 代码块结束 */

  /* Change the DCMI state */
  hdcmi->State = HAL_DCMI_STATE_BUSY;                   /* 赋值或初始化 */
                          /* Configures the HS, VS, DE and PC polarity */
  hdcmi->Instance->CR &= ~(DCMI_CR_PCKPOL | DCMI_CR_HSPOL  | DCMI_CR_VSPOL  | DCMI_CR_EDM_0 |\
                           DCMI_CR_EDM_1  | DCMI_CR_FCRC_0 | DCMI_CR_FCRC_1 | DCMI_CR_JPEG  |\
                           DCMI_CR_ESS                  /* 源代码 */
#if defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx)  /* 条件编译 */
                           | DCMI_CR_BSM_0 | DCMI_CR_BSM_1 | DCMI_CR_OEBS |\
                           DCMI_CR_LSM | DCMI_CR_OELS   /* 源代码 */
#endif /* STM32F446xx || STM32F469xx || STM32F479xx */
                           );                           /* 执行语句 */
  hdcmi->Instance->CR |=  (uint32_t)(hdcmi->Init.SynchroMode | hdcmi->Init.CaptureRate |\
                                     hdcmi->Init.VSPolarity  | hdcmi->Init.HSPolarity  |\
                                     hdcmi->Init.PCKPolarity | hdcmi->Init.ExtendedDataMode |\
                                     hdcmi->Init.JPEGMode  /* 源代码 */
#if defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx)  /* 条件编译 */
                                     | hdcmi->Init.ByteSelectMode |\
                                     hdcmi->Init.ByteSelectStart | hdcmi->Init.LineSelectMode |\
                                     hdcmi->Init.LineSelectStart  /* 源代码 */
#endif /* STM32F446xx || STM32F469xx || STM32F479xx */
                                     );                 /* 执行语句 */
  if(hdcmi->Init.SynchroMode == DCMI_SYNCHRO_EMBEDDED)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hdcmi->Instance->ESCR = (((uint32_t)hdcmi->Init.SyncroCode.FrameStartCode)    |  /* 源代码 */
                             ((uint32_t)hdcmi->Init.SyncroCode.LineStartCode << DCMI_POSITION_ESCR_LSC)|  /* 源代码 */
                             ((uint32_t)hdcmi->Init.SyncroCode.LineEndCode << DCMI_POSITION_ESCR_LEC) |  /* 源代码 */
                             ((uint32_t)hdcmi->Init.SyncroCode.FrameEndCode << DCMI_POSITION_ESCR_FEC));  /* 执行语句 */

  }                                                     /* 代码块结束 */

  /* Enable the Line, Vsync, Error and Overrun interrupts */
  __HAL_DCMI_ENABLE_IT(hdcmi, DCMI_IT_LINE | DCMI_IT_VSYNC | DCMI_IT_ERR | DCMI_IT_OVR);  /* HAL辅助宏/函数 */

  /* Update error code */
  hdcmi->ErrorCode = HAL_DCMI_ERROR_NONE;               /* 赋值或初始化 */

  /* Initialize the DCMI state*/
  hdcmi->State  = HAL_DCMI_STATE_READY;                 /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */
#endif /* STM32F407xx || STM32F417xx || STM32F427xx || STM32F437xx || STM32F429xx ||\
          STM32F439xx || STM32F446xx || STM32F469xx || STM32F479xx */
#endif /* HAL_DCMI_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
