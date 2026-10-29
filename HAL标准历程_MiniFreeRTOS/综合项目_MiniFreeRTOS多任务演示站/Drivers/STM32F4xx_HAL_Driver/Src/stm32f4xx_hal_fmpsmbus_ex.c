/* 文件: stm32f4xx_hal_fmpsmbus_ex.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_fmpsmbus_ex.c
  * @author  MCD Application Team
  * @brief   FMPSMBUS Extended HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of FMPSMBUS Extended peripheral:
  *           + Extended features functions
  *
  @verbatim
  ==============================================================================
               ##### FMPSMBUS peripheral Extended features  #####
  ==============================================================================

  [..] Comparing to other previous devices, the FMPSMBUS interface for STM32F4xx
       devices contains the following additional features

       (+) Disable or enable Fast Mode Plus

                     ##### How to use this driver #####
  ==============================================================================
    (#) Configure the enable or disable of fast mode plus driving capability using the functions :
          (++) HAL_FMPSMBUSEx_EnableFastModePlus()
          (++) HAL_FMPSMBUSEx_DisableFastModePlus()
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

/** @defgroup FMPSMBUSEx FMPSMBUSEx
  * @brief FMPSMBUS Extended HAL module driver
  * @{
  */

#ifdef HAL_FMPSMBUS_MODULE_ENABLED                      /* 条件编译 */
#if defined(FMPI2C_CR1_PE)                              /* 条件编译 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup FMPSMBUSEx_Exported_Functions FMPSMBUS Extended Exported Functions
  * @{
  */

/** @defgroup FMPSMBUSEx_Exported_Functions_Group1 Extended features functions
  * @brief    Extended features functions
 *
@verbatim
 ===============================================================================
                      ##### Extended features functions #####
 ===============================================================================
    [..] This section provides functions allowing to:

      (+) Configure Fast Mode Plus

@endverbatim
  * @{
  */

/**
  * @brief Enable the FMPSMBUS fast mode plus driving capability.
  * @param ConfigFastModePlus Selects the pin.
  *   This parameter can be one of the @ref FMPSMBUSEx_FastModePlus values
  * @note  For FMPI2C1, fast mode plus driving capability can be enabled on all selected
  *        FMPI2C1 pins using FMPI2C_FASTMODEPLUS_FMPI2C1 parameter or independently
  *        on each one of the following pins PB6, PB7, PB8 and PB9.
  * @note  For remaining FMPI2C1 pins (PA14, PA15...) fast mode plus driving capability
  *        can be enabled only by using FMPI2C_FASTMODEPLUS_FMPI2C1 parameter.
  * @retval None
  */
void HAL_FMPSMBUSEx_EnableFastModePlus(uint32_t ConfigFastModePlus)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Check the parameter */
  assert_param(IS_FMPSMBUS_FASTMODEPLUS(ConfigFastModePlus));  /* 执行语句 */

  /* Enable SYSCFG clock */
  __HAL_RCC_SYSCFG_CLK_ENABLE();                        /* HAL辅助宏/函数 */

  /* Enable fast mode plus driving capability for selected pin */
  SET_BIT(SYSCFG->CFGR, (uint32_t)ConfigFastModePlus);  /* 执行语句 */
}                                                       /* 代码块结束 */

/**
  * @brief Disable the FMPSMBUS fast mode plus driving capability.
  * @param ConfigFastModePlus Selects the pin.
  *   This parameter can be one of the @ref FMPSMBUSEx_FastModePlus values
  * @note  For FMPI2C1, fast mode plus driving capability can be disabled on all selected
  *        FMPI2C1 pins using FMPI2C_FASTMODEPLUS_FMPI2C1 parameter or independently
  *        on each one of the following pins PB6, PB7, PB8 and PB9.
  * @note  For remaining FMPI2C1 pins (PA14, PA15...) fast mode plus driving capability
  *        can be disabled only by using FMPI2C_FASTMODEPLUS_FMPI2C1 parameter.
  * @retval None
  */
void HAL_FMPSMBUSEx_DisableFastModePlus(uint32_t ConfigFastModePlus)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Check the parameter */
  assert_param(IS_FMPSMBUS_FASTMODEPLUS(ConfigFastModePlus));  /* 执行语句 */

  /* Enable SYSCFG clock */
  __HAL_RCC_SYSCFG_CLK_ENABLE();                        /* HAL辅助宏/函数 */

  /* Disable fast mode plus driving capability for selected pin */
  CLEAR_BIT(SYSCFG->CFGR, (uint32_t)ConfigFastModePlus);  /* 执行语句 */
}                                                       /* 代码块结束 */


/**
  * @}
  */

/**
  * @}
  */

#endif /* FMPI2C_CR1_PE */
#endif /* HAL_FMPSMBUS_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
