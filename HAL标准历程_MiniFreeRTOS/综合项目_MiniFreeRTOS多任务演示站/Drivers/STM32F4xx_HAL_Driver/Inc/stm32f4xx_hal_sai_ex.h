/* 文件: stm32f4xx_hal_sai_ex.h | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_sai_ex.h
  * @author  MCD Application Team
  * @brief   Header file of SAI Extension HAL module.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4xx_HAL_SAI_EX_H                        /* 防止头文件重复包含 */
#define __STM32F4xx_HAL_SAI_EX_H                        /* 定义包含保护宏 */

#ifdef __cplusplus                                      /* 条件编译 */
extern "C" {                                            /* 外部符号声明 */
#endif                                                  /* 条件编译结束 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal_def.h"                          /* 包含 STM32/HAL 头文件 */

/** @addtogroup STM32F4xx_HAL_Driver
  * @{
  */

/** @addtogroup SAIEx
  * @{
  */

#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx) || \
    defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx) || defined(STM32F413xx) || \
    defined(STM32F423xx)                                /* 源代码 */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/** @defgroup SAI_Clock_Source  SAI Clock Source
  * @{
  */
#if defined(STM32F413xx) || defined(STM32F423xx)        /* 条件编译 */
#define SAI_CLKSOURCE_PLLI2S             0x00000000U    /* 定义宏 */
#define SAI_CLKSOURCE_EXT                0x00100000U    /* 定义宏 */
#define SAI_CLKSOURCE_PLLR               0x00200000U    /* 定义宏 */
#define SAI_CLKSOURCE_HS                 0x00300000U    /* 定义宏 */
#else                                                   /* 条件编译否则 */
#define SAI_CLKSOURCE_PLLSAI             0x00000000U    /* 定义宏 */
#define SAI_CLKSOURCE_PLLI2S             0x00100000U    /* 定义宏 */
#define SAI_CLKSOURCE_EXT                0x00200000U    /* 定义宏 */
#define SAI_CLKSOURCE_NA                 0x00400000U /*!< No applicable for STM32F446xx */
#endif                                                  /* 条件编译结束 */


/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup SAIEx_Exported_Functions
  * @{
  */

/** @addtogroup SAIEx_Exported_Functions_Group1
  * @{
  */

/* Extended features functions ************************************************/
void SAI_BlockSynchroConfig(SAI_HandleTypeDef *hsai);   /* 函数定义或声明 */
uint32_t SAI_GetInputClock(SAI_HandleTypeDef *hsai);    /* 32位无符号变量/函数 */
/**
  * @}
  */

/**
  * @}
  */
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
#if defined(STM32F413xx) || defined(STM32F423xx)        /* 条件编译 */
#define IS_SAI_CLK_SOURCE(SOURCE) (((SOURCE) == SAI_CLKSOURCE_PLLI2S) ||\
                                   ((SOURCE) == SAI_CLKSOURCE_EXT)||\
                                   ((SOURCE) == SAI_CLKSOURCE_PLLR)||\
                                   ((SOURCE) == SAI_CLKSOURCE_HS))  /* 源代码 */
#else                                                   /* 条件编译否则 */
#define IS_SAI_CLK_SOURCE(SOURCE) (((SOURCE) == SAI_CLKSOURCE_PLLSAI) ||\
                                   ((SOURCE) == SAI_CLKSOURCE_EXT)||\
                                   ((SOURCE) == SAI_CLKSOURCE_PLLI2S)||\
                                   ((SOURCE) == SAI_CLKSOURCE_NA))  /* 源代码 */
#endif                                                  /* 条件编译结束 */
/* Private functions ---------------------------------------------------------*/

#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx || STM32F446xx || STM32F469xx || STM32F479xx || STM32F413xx || STM32F423xx */
/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus                                      /* 条件编译 */
}                                                       /* 代码块结束 */
#endif                                                  /* 条件编译结束 */

#endif /* __STM32F4xx_HAL_SAI_EX_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
