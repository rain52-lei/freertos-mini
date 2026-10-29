/* 文件: stm32f4xx_hal_def.h | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_def.h
  * @author  MCD Application Team
  * @brief   This file contains HAL common defines, enumeration, macros and 
  *          structures definitions. 
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
#ifndef __STM32F4xx_HAL_DEF                             /* 防止头文件重复包含 */
#define __STM32F4xx_HAL_DEF                             /* 定义包含保护宏 */

#ifdef __cplusplus                                      /* 条件编译 */
 extern "C" {                                           /* 外部符号声明 */
#endif                                                  /* 条件编译结束 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"                                  /* 包含 STM32/HAL 头文件 */
#include "Legacy/stm32_hal_legacy.h"                    /* 包含头文件 */
#include <stddef.h>                                     /* 包含 C 标准库头文件 */

/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  HAL Status structures definition  
  */  
typedef enum                                            /* 类型定义 */
{                                                       /* 代码块开始 */
  HAL_OK       = 0x00U,                                 /* HAL库函数调用 */
  HAL_ERROR    = 0x01U,                                 /* HAL库函数调用 */
  HAL_BUSY     = 0x02U,                                 /* HAL库函数调用 */
  HAL_TIMEOUT  = 0x03U                                  /* HAL库函数调用 */
} HAL_StatusTypeDef;                                    /* 代码块结束 */

/** 
  * @brief  HAL Lock structures definition  
  */
typedef enum                                            /* 类型定义 */
{                                                       /* 代码块开始 */
  HAL_UNLOCKED = 0x00U,                                 /* HAL库函数调用 */
  HAL_LOCKED   = 0x01U                                  /* HAL库函数调用 */
} HAL_LockTypeDef;                                      /* 代码块结束 */

/* Exported macro ------------------------------------------------------------*/

#define UNUSED(X) (void)X      /* To avoid gcc/g++ warnings */

#define HAL_MAX_DELAY      0xFFFFFFFFU                  /* 定义宏 */

#define HAL_IS_BIT_SET(REG, BIT)         (((REG) & (BIT)) == (BIT))  /* 定义宏 */
#define HAL_IS_BIT_CLR(REG, BIT)         (((REG) & (BIT)) == 0U)  /* 定义宏 */

#define __HAL_LINKDMA(__HANDLE__, __PPP_DMA_FIELD__, __DMA_HANDLE__)               \
                        do{                                                      \
                              (__HANDLE__)->__PPP_DMA_FIELD__ = &(__DMA_HANDLE__); \
                              (__DMA_HANDLE__).Parent = (__HANDLE__);             \
                          } while(0U)                   /* 代码块结束 */

/** @brief Reset the Handle's State field.
  * @param __HANDLE__ specifies the Peripheral Handle.
  * @note  This macro can be used for the following purpose: 
  *          - When the Handle is declared as local variable; before passing it as parameter
  *            to HAL_PPP_Init() for the first time, it is mandatory to use this macro 
  *            to set to 0 the Handle's "State" field.
  *            Otherwise, "State" field may have any random value and the first time the function 
  *            HAL_PPP_Init() is called, the low level hardware initialization will be missed
  *            (i.e. HAL_PPP_MspInit() will not be executed).
  *          - When there is a need to reconfigure the low level hardware: instead of calling
  *            HAL_PPP_DeInit() then HAL_PPP_Init(), user can make a call to this macro then HAL_PPP_Init().
  *            In this later function, when the Handle's "State" field is set to 0, it will execute the function
  *            HAL_PPP_MspInit() which will reconfigure the low level hardware.
  * @retval None
  */
#define __HAL_RESET_HANDLE_STATE(__HANDLE__) ((__HANDLE__)->State = 0U)  /* 定义包含保护宏 */

#if (USE_RTOS == 1U)                                    /* 条件编译 */
  /* Reserved for future use */
  #error "USE_RTOS should be 0 in the current HAL release"  /* 编译错误指示 */
#else                                                   /* 条件编译否则 */
  #define __HAL_LOCK(__HANDLE__)                                           \
                                do{                                        \
                                    if((__HANDLE__)->Lock == HAL_LOCKED)   \
                                    {                                      \
                                       return HAL_BUSY;                    \
                                    }                                      \
                                    else                                   \
                                    {                                      \
                                       (__HANDLE__)->Lock = HAL_LOCKED;    \
                                    }                                      \
                                  }while (0U)           /* 代码块结束 */

  #define __HAL_UNLOCK(__HANDLE__)                                          \
                                  do{                                       \
                                      (__HANDLE__)->Lock = HAL_UNLOCKED;    \
                                    }while (0U)         /* 代码块结束 */
#endif /* USE_RTOS */

#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050) /* ARM Compiler V6 */
  #ifndef __weak                                        /* 防止头文件重复包含 */
    #define __weak  __attribute__((weak))               /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef __packed                                      /* 防止头文件重复包含 */
    #define __packed  __attribute__((packed))           /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
#elif defined ( __GNUC__ ) && !defined (__CC_ARM) /* GNU Compiler */
  #ifndef __weak                                        /* 防止头文件重复包含 */
    #define __weak   __attribute__((weak))              /* 定义包含保护宏 */
  #endif /* __weak */
  #ifndef __packed                                      /* 防止头文件重复包含 */
    #define __packed __attribute__((__packed__))        /* 定义包含保护宏 */
  #endif /* __packed */
#endif /* __GNUC__ */


/* Macro to get variable aligned on 4-bytes, for __ICCARM__ the directive "#pragma data_alignment=4" must be used instead */
#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050) /* ARM Compiler V6 */
  #ifndef __ALIGN_BEGIN                                 /* 防止头文件重复包含 */
    #define __ALIGN_BEGIN                               /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef __ALIGN_END                                   /* 防止头文件重复包含 */
    #define __ALIGN_END      __attribute__ ((aligned (4)))  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
#elif defined ( __GNUC__ ) && !defined (__CC_ARM) /* GNU Compiler */
  #ifndef __ALIGN_END                                   /* 防止头文件重复包含 */
#define __ALIGN_END    __attribute__ ((aligned (4)))    /* 定义包含保护宏 */
  #endif /* __ALIGN_END */
  #ifndef __ALIGN_BEGIN                                 /* 防止头文件重复包含 */
    #define __ALIGN_BEGIN                               /* 定义包含保护宏 */
  #endif /* __ALIGN_BEGIN */
#else                                                   /* 条件编译否则 */
  #ifndef __ALIGN_END                                   /* 防止头文件重复包含 */
    #define __ALIGN_END                                 /* 定义包含保护宏 */
  #endif /* __ALIGN_END */
  #ifndef __ALIGN_BEGIN                                 /* 防止头文件重复包含 */
    #if defined   (__CC_ARM)      /* ARM Compiler V5*/
#define __ALIGN_BEGIN    __align(4)                     /* 定义包含保护宏 */
    #elif defined (__ICCARM__)    /* IAR Compiler */
      #define __ALIGN_BEGIN                             /* 定义包含保护宏 */
    #endif /* __CC_ARM */
  #endif /* __ALIGN_BEGIN */
#endif /* __GNUC__ */


/** 
  * @brief  __RAM_FUNC definition
  */ 
#if defined ( __CC_ARM   ) || (defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))  /* 条件编译 */
/* ARM Compiler V4/V5 and V6
   --------------------------
   RAM functions are defined using the toolchain options. 
   Functions that are executed in RAM should reside in a separate source module.
   Using the 'Options for File' dialog you can simply change the 'Code / Const' 
   area of a module to a memory space in physical RAM.
   Available memory areas are declared in the 'Target' tab of the 'Options for Target'
   dialog. 
*/
#define __RAM_FUNC                                      /* 定义包含保护宏 */

#elif defined ( __ICCARM__ )                            /* 条件编译否则如果 */
/* ICCARM Compiler
   ---------------
   RAM functions are defined using a specific toolchain keyword "__ramfunc". 
*/
#define __RAM_FUNC __ramfunc                            /* 定义包含保护宏 */

#elif defined   (  __GNUC__  )                          /* 条件编译否则如果 */
/* GNU Compiler
   ------------
  RAM functions are defined using a specific toolchain attribute 
   "__attribute__((section(".RamFunc")))".
*/
#define __RAM_FUNC __attribute__((section(".RamFunc")))  /* 定义包含保护宏 */

#endif                                                  /* 条件编译结束 */

/** 
  * @brief  __NOINLINE definition
  */ 
#if defined ( __CC_ARM   ) || (defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)) || defined   (  __GNUC__  )  /* 条件编译 */
/* ARM V4/V5 and V6 & GNU Compiler
   -------------------------------
*/
#define __NOINLINE __attribute__ ( (noinline) )         /* 定义包含保护宏 */

#elif defined ( __ICCARM__ )                            /* 条件编译否则如果 */
/* ICCARM Compiler
   ---------------
*/
#define __NOINLINE _Pragma("optimize = no_inline")      /* 定义包含保护宏 */

#endif                                                  /* 条件编译结束 */

#ifdef __cplusplus                                      /* 条件编译 */
}                                                       /* 代码块结束 */
#endif                                                  /* 条件编译结束 */

#endif /* ___STM32F4xx_HAL_DEF */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
