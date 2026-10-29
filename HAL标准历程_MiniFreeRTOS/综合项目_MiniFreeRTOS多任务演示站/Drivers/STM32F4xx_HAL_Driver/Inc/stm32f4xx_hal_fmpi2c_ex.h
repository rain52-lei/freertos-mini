/* 文件: stm32f4xx_hal_fmpi2c_ex.h | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_fmpi2c_ex.h
  * @author  MCD Application Team
  * @brief   Header file of FMPI2C HAL Extended module.
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
#ifndef STM32F4xx_HAL_FMPI2C_EX_H                       /* 防止头文件重复包含 */
#define STM32F4xx_HAL_FMPI2C_EX_H                       /* 定义宏 */

#ifdef __cplusplus                                      /* 条件编译 */
extern "C" {                                            /* 外部符号声明 */
#endif                                                  /* 条件编译结束 */

#if defined(FMPI2C_CR1_PE)                              /* 条件编译 */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal_def.h"                          /* 包含 STM32/HAL 头文件 */

/** @addtogroup STM32F4xx_HAL_Driver
  * @{
  */

/** @addtogroup FMPI2CEx
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/** @defgroup FMPI2CEx_Exported_Constants FMPI2C Extended Exported Constants
  * @{
  */

/** @defgroup FMPI2CEx_Analog_Filter FMPI2C Extended Analog Filter
  * @{
  */
#define FMPI2C_ANALOGFILTER_ENABLE         0x00000000U  /* 定义宏 */
#define FMPI2C_ANALOGFILTER_DISABLE        FMPI2C_CR1_ANFOFF  /* 定义宏 */
/**
  * @}
  */

/** @defgroup FMPI2CEx_FastModePlus FMPI2C Extended Fast Mode Plus
  * @{
  */
#define FMPI2C_FASTMODEPLUS_SCL            SYSCFG_CFGR_FMPI2C1_SCL  /*!< Enable Fast Mode Plus on FMPI2C1 SCL pins       */
#define FMPI2C_FASTMODEPLUS_SDA            SYSCFG_CFGR_FMPI2C1_SDA  /*!< Enable Fast Mode Plus on FMPI2C1 SDA pins       */
/**
  * @}
  */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/** @defgroup FMPI2CEx_Exported_Macros FMPI2C Extended Exported Macros
  * @{
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup FMPI2CEx_Exported_Functions FMPI2C Extended Exported Functions
  * @{
  */

/** @addtogroup FMPI2CEx_Exported_Functions_Group1 FMPI2C Extended Filter Mode Functions
  * @{
  */
/* Peripheral Control functions  ************************************************/
HAL_StatusTypeDef HAL_FMPI2CEx_ConfigAnalogFilter(FMPI2C_HandleTypeDef *hfmpi2c, uint32_t AnalogFilter);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_FMPI2CEx_ConfigDigitalFilter(FMPI2C_HandleTypeDef *hfmpi2c, uint32_t DigitalFilter);  /* HAL状态类型函数 */
/**
  * @}
  */

/** @addtogroup FMPI2CEx_Exported_Functions_Group2 FMPI2C Extended WakeUp Mode Functions
  * @{
  */
/**
  * @}
  */

/** @addtogroup FMPI2CEx_Exported_Functions_Group3 FMPI2C Extended FastModePlus Functions
  * @{
  */
void HAL_FMPI2CEx_EnableFastModePlus(uint32_t ConfigFastModePlus);  /* 函数定义或声明 */
void HAL_FMPI2CEx_DisableFastModePlus(uint32_t ConfigFastModePlus);  /* 函数定义或声明 */
/**
  * @}
  */


/**
  * @}
  */

/* Private constants ---------------------------------------------------------*/
/** @defgroup FMPI2CEx_Private_Constants FMPI2C Extended Private Constants
  * @{
  */

/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/** @defgroup FMPI2CEx_Private_Macro FMPI2C Extended Private Macros
  * @{
  */
#define IS_FMPI2C_ANALOG_FILTER(FILTER)    (((FILTER) == FMPI2C_ANALOGFILTER_ENABLE) || \
                                         ((FILTER) == FMPI2C_ANALOGFILTER_DISABLE))  /* 源代码 */

#define IS_FMPI2C_DIGITAL_FILTER(FILTER)   ((FILTER) <= 0x0000000FU)  /* 定义宏 */

#define IS_FMPI2C_FASTMODEPLUS(__CONFIG__) ((((__CONFIG__) & (FMPI2C_FASTMODEPLUS_SCL)) == FMPI2C_FASTMODEPLUS_SCL) || \
                                         (((__CONFIG__) & (FMPI2C_FASTMODEPLUS_SDA)) == FMPI2C_FASTMODEPLUS_SDA))  /* 源代码 */
/**
  * @}
  */

/* Private Functions ---------------------------------------------------------*/
/** @defgroup FMPI2CEx_Private_Functions FMPI2C Extended Private Functions
  * @{
  */
/* Private functions are defined in stm32f4xx_hal_fmpfmpi2c_ex.c file */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#endif /* FMPI2C_CR1_PE */
#ifdef __cplusplus                                      /* 条件编译 */
}                                                       /* 代码块结束 */
#endif                                                  /* 条件编译结束 */

#endif /* STM32F4xx_HAL_FMPI2C_EX_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
