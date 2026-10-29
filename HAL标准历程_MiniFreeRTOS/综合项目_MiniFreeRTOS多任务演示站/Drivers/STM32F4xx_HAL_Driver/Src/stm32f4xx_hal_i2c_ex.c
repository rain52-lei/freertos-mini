/* 文件: stm32f4xx_hal_i2c_ex.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_i2c_ex.c
  * @author  MCD Application Team
  * @brief   I2C Extension HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of I2C extension peripheral:
  *           + Extension features functions
  *
  @verbatim
  ==============================================================================
               ##### I2C peripheral extension features  #####
  ==============================================================================

  [..] Comparing to other previous devices, the I2C interface for STM32F427xx/437xx/
       429xx/439xx devices contains the following additional features :

       (+) Possibility to disable or enable Analog Noise Filter
       (+) Use of a configured Digital Noise Filter

                     ##### How to use this driver #####
  ==============================================================================
  [..] This driver provides functions to configure Noise Filter
    (#) Configure I2C Analog noise filter using the function HAL_I2C_AnalogFilter_Config()
    (#) Configure I2C Digital noise filter using the function HAL_I2C_DigitalFilter_Config()

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

/** @defgroup I2CEx I2CEx
  * @brief I2C HAL module driver
  * @{
  */

#ifdef HAL_I2C_MODULE_ENABLED                           /* 条件编译 */

#if  defined(I2C_FLTR_ANOFF)&&defined(I2C_FLTR_DNF)     /* 条件编译 */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/** @defgroup I2CEx_Exported_Functions I2C Exported Functions
  * @{
  */


/** @defgroup I2CEx_Exported_Functions_Group1 Extension features functions
 *  @brief   Extension features functions
 *
@verbatim
 ===============================================================================
                      ##### Extension features functions #####
 ===============================================================================
    [..] This section provides functions allowing to:
      (+) Configure Noise Filters

@endverbatim
  * @{
  */

/**
  * @brief  Configures I2C Analog noise filter.
  * @param  hi2c pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2Cx peripheral.
  * @param  AnalogFilter new state of the Analog filter.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2CEx_ConfigAnalogFilter(I2C_HandleTypeDef *hi2c, uint32_t AnalogFilter)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_I2C_ALL_INSTANCE(hi2c->Instance));    /* 执行语句 */
  assert_param(IS_I2C_ANALOG_FILTER(AnalogFilter));     /* 执行语句 */

  if (hi2c->State == HAL_I2C_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    hi2c->State = HAL_I2C_STATE_BUSY;                   /* 赋值或初始化 */

    /* Disable the selected I2C peripheral */
    __HAL_I2C_DISABLE(hi2c);                            /* HAL辅助宏/函数 */

    /* Reset I2Cx ANOFF bit */
    hi2c->Instance->FLTR &= ~(I2C_FLTR_ANOFF);          /* 赋值或初始化 */

    /* Disable the analog filter */
    hi2c->Instance->FLTR |= AnalogFilter;               /* 赋值或初始化 */

    __HAL_I2C_ENABLE(hi2c);                             /* HAL辅助宏/函数 */

    hi2c->State = HAL_I2C_STATE_READY;                  /* 赋值或初始化 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configures I2C Digital noise filter.
  * @param  hi2c pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2Cx peripheral.
  * @param  DigitalFilter Coefficient of digital noise filter between 0x00 and 0x0F.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2CEx_ConfigDigitalFilter(I2C_HandleTypeDef *hi2c, uint32_t DigitalFilter)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint16_t tmpreg = 0;                                  /* 16位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_I2C_ALL_INSTANCE(hi2c->Instance));    /* 执行语句 */
  assert_param(IS_I2C_DIGITAL_FILTER(DigitalFilter));   /* 执行语句 */

  if (hi2c->State == HAL_I2C_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    hi2c->State = HAL_I2C_STATE_BUSY;                   /* 赋值或初始化 */

    /* Disable the selected I2C peripheral */
    __HAL_I2C_DISABLE(hi2c);                            /* HAL辅助宏/函数 */

    /* Get the old register value */
    tmpreg = hi2c->Instance->FLTR;                      /* 赋值或初始化 */

    /* Reset I2Cx DNF bit [3:0] */
    tmpreg &= ~(I2C_FLTR_DNF);                          /* 赋值或初始化 */

    /* Set I2Cx DNF coefficient */
    tmpreg |= DigitalFilter;                            /* 赋值或初始化 */

    /* Store the new register value */
    hi2c->Instance->FLTR = tmpreg;                      /* 赋值或初始化 */

    __HAL_I2C_ENABLE(hi2c);                             /* HAL辅助宏/函数 */

    hi2c->State = HAL_I2C_STATE_READY;                  /* 赋值或初始化 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/**
  * @}
  */
#endif                                                  /* 条件编译结束 */

#endif /* HAL_I2C_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
