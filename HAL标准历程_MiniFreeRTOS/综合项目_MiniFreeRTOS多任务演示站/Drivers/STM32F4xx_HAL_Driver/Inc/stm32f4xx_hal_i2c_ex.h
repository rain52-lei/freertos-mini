/* 文件: stm32f4xx_hal_i2c_ex.h | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_i2c_ex.h
  * @author  MCD Application Team
  * @brief   Header file of I2C HAL Extension module.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4xx_HAL_I2C_EX_H                        /* 防止头文件重复包含 */
#define __STM32F4xx_HAL_I2C_EX_H                        /* 定义包含保护宏 */

#ifdef __cplusplus                                      /* 条件编译 */
extern "C" {                                            /* 外部符号声明 */
#endif                                                  /* 条件编译结束 */

#if  defined(I2C_FLTR_ANOFF)&&defined(I2C_FLTR_DNF)     /* 条件编译 */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal_def.h"                          /* 包含 STM32/HAL 头文件 */

/** @addtogroup STM32F4xx_HAL_Driver
  * @{
  */

/** @addtogroup I2CEx
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/** @defgroup I2CEx_Exported_Constants I2C Exported Constants
  * @{
  */

/** @defgroup I2CEx_Analog_Filter I2C Analog Filter
  * @{
  */
#define I2C_ANALOGFILTER_ENABLE        0x00000000U      /* 定义宏 */
#define I2C_ANALOGFILTER_DISABLE       I2C_FLTR_ANOFF   /* 定义宏 */
/**
  * @}
  */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/** @addtogroup I2CEx_Exported_Functions
  * @{
  */

/** @addtogroup I2CEx_Exported_Functions_Group1
  * @{
  */
/* Peripheral Control functions  ************************************************/
HAL_StatusTypeDef HAL_I2CEx_ConfigAnalogFilter(I2C_HandleTypeDef *hi2c, uint32_t AnalogFilter);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_I2CEx_ConfigDigitalFilter(I2C_HandleTypeDef *hi2c, uint32_t DigitalFilter);  /* HAL状态类型函数 */
/**
  * @}
  */

/**
  * @}
  */
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/** @defgroup I2CEx_Private_Constants I2C Private Constants
  * @{
  */

/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/** @defgroup I2CEx_Private_Macros I2C Private Macros
  * @{
  */
#define IS_I2C_ANALOG_FILTER(FILTER) (((FILTER) == I2C_ANALOGFILTER_ENABLE) || \
                                      ((FILTER) == I2C_ANALOGFILTER_DISABLE))  /* 源代码 */
#define IS_I2C_DIGITAL_FILTER(FILTER)   ((FILTER) <= 0x0000000FU)  /* 定义宏 */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#endif                                                  /* 条件编译结束 */

#ifdef __cplusplus                                      /* 条件编译 */
}                                                       /* 代码块结束 */
#endif                                                  /* 条件编译结束 */

#endif /* __STM32F4xx_HAL_I2C_EX_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
