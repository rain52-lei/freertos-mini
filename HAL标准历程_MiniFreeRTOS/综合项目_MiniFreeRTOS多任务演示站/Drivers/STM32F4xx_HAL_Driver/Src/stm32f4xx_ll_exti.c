/* 文件: stm32f4xx_ll_exti.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_ll_exti.c
  * @author  MCD Application Team
  * @brief   EXTI LL module driver.
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
#if defined(USE_FULL_LL_DRIVER)                         /* 条件编译 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_ll_exti.h"                          /* 包含 STM32/HAL 头文件 */
#ifdef  USE_FULL_ASSERT                                 /* 条件编译 */
#include "stm32_assert.h"                               /* 包含 STM32/HAL 头文件 */
#else                                                   /* 条件编译否则 */
#define assert_param(expr) ((void)0U)                   /* 定义宏 */
#endif                                                  /* 条件编译结束 */

/** @addtogroup STM32F4xx_LL_Driver
  * @{
  */

#if defined (EXTI)                                      /* 条件编译 */

/** @defgroup EXTI_LL EXTI
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/** @addtogroup EXTI_LL_Private_Macros
  * @{
  */

#define IS_LL_EXTI_LINE_0_31(__VALUE__)              (((__VALUE__) & ~LL_EXTI_LINE_ALL_0_31) == 0x00000000U)  /* 定义宏 */

#define IS_LL_EXTI_MODE(__VALUE__)                   (((__VALUE__) == LL_EXTI_MODE_IT)            \
                                                   || ((__VALUE__) == LL_EXTI_MODE_EVENT)         \
                                                   || ((__VALUE__) == LL_EXTI_MODE_IT_EVENT))  /* 源代码 */


#define IS_LL_EXTI_TRIGGER(__VALUE__)                (((__VALUE__) == LL_EXTI_TRIGGER_NONE)       \
                                                   || ((__VALUE__) == LL_EXTI_TRIGGER_RISING)     \
                                                   || ((__VALUE__) == LL_EXTI_TRIGGER_FALLING)    \
                                                   || ((__VALUE__) == LL_EXTI_TRIGGER_RISING_FALLING))  /* 源代码 */

/**
  * @}
  */

/* Private function prototypes -----------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/** @addtogroup EXTI_LL_Exported_Functions
  * @{
  */

/** @addtogroup EXTI_LL_EF_Init
  * @{
  */

/**
  * @brief  De-initialize the EXTI registers to their default reset values.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: EXTI registers are de-initialized
  *          - ERROR: not applicable
  */
uint32_t LL_EXTI_DeInit(void)                           /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  /* Interrupt mask register set to default reset values */
  LL_EXTI_WriteReg(IMR,   0x00000000U);                 /* 执行语句 */
  /* Event mask register set to default reset values */
  LL_EXTI_WriteReg(EMR,   0x00000000U);                 /* 执行语句 */
  /* Rising Trigger selection register set to default reset values */
  LL_EXTI_WriteReg(RTSR,  0x00000000U);                 /* 执行语句 */
  /* Falling Trigger selection register set to default reset values */
  LL_EXTI_WriteReg(FTSR,  0x00000000U);                 /* 执行语句 */
  /* Software interrupt event register set to default reset values */
  LL_EXTI_WriteReg(SWIER, 0x00000000U);                 /* 执行语句 */
  /* Pending register set to default reset values */
  LL_EXTI_WriteReg(PR,    0x00FFFFFFU);                 /* 执行语句 */

  return SUCCESS;                                       /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initialize the EXTI registers according to the specified parameters in EXTI_InitStruct.
  * @param  EXTI_InitStruct pointer to a @ref LL_EXTI_InitTypeDef structure.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: EXTI registers are initialized
  *          - ERROR: not applicable
  */
uint32_t LL_EXTI_Init(LL_EXTI_InitTypeDef *EXTI_InitStruct)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  ErrorStatus status = SUCCESS;                         /* 赋值或初始化 */
  /* Check the parameters */
  assert_param(IS_LL_EXTI_LINE_0_31(EXTI_InitStruct->Line_0_31));  /* 执行语句 */
  assert_param(IS_FUNCTIONAL_STATE(EXTI_InitStruct->LineCommand));  /* 执行语句 */
  assert_param(IS_LL_EXTI_MODE(EXTI_InitStruct->Mode));  /* 执行语句 */

  /* ENABLE LineCommand */
  if (EXTI_InitStruct->LineCommand != DISABLE)          /* 条件判断 */
  {                                                     /* 代码块开始 */
    assert_param(IS_LL_EXTI_TRIGGER(EXTI_InitStruct->Trigger));  /* 执行语句 */

    /* Configure EXTI Lines in range from 0 to 31 */
    if (EXTI_InitStruct->Line_0_31 != LL_EXTI_LINE_NONE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      switch (EXTI_InitStruct->Mode)                    /* switch分支 */
      {                                                 /* 代码块开始 */
        case LL_EXTI_MODE_IT:                           /* case标签 */
          /* First Disable Event on provided Lines */
          LL_EXTI_DisableEvent_0_31(EXTI_InitStruct->Line_0_31);  /* 执行语句 */
          /* Then Enable IT on provided Lines */
          LL_EXTI_EnableIT_0_31(EXTI_InitStruct->Line_0_31);  /* 执行语句 */
          break;                                        /* 跳出 */
        case LL_EXTI_MODE_EVENT:                        /* case标签 */
          /* First Disable IT on provided Lines */
          LL_EXTI_DisableIT_0_31(EXTI_InitStruct->Line_0_31);  /* 执行语句 */
          /* Then Enable Event on provided Lines */
          LL_EXTI_EnableEvent_0_31(EXTI_InitStruct->Line_0_31);  /* 执行语句 */
          break;                                        /* 跳出 */
        case LL_EXTI_MODE_IT_EVENT:                     /* case标签 */
          /* Directly Enable IT & Event on provided Lines */
          LL_EXTI_EnableIT_0_31(EXTI_InitStruct->Line_0_31);  /* 执行语句 */
          LL_EXTI_EnableEvent_0_31(EXTI_InitStruct->Line_0_31);  /* 执行语句 */
          break;                                        /* 跳出 */
        default:                                        /* default分支 */
          status = ERROR;                               /* 赋值或初始化 */
          break;                                        /* 跳出 */
      }                                                 /* 代码块结束 */
      if (EXTI_InitStruct->Trigger != LL_EXTI_TRIGGER_NONE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        switch (EXTI_InitStruct->Trigger)               /* switch分支 */
        {                                               /* 代码块开始 */
          case LL_EXTI_TRIGGER_RISING:                  /* case标签 */
            /* First Disable Falling Trigger on provided Lines */
            LL_EXTI_DisableFallingTrig_0_31(EXTI_InitStruct->Line_0_31);  /* 执行语句 */
            /* Then Enable Rising Trigger on provided Lines */
            LL_EXTI_EnableRisingTrig_0_31(EXTI_InitStruct->Line_0_31);  /* 执行语句 */
            break;                                      /* 跳出 */
          case LL_EXTI_TRIGGER_FALLING:                 /* case标签 */
            /* First Disable Rising Trigger on provided Lines */
            LL_EXTI_DisableRisingTrig_0_31(EXTI_InitStruct->Line_0_31);  /* 执行语句 */
            /* Then Enable Falling Trigger on provided Lines */
            LL_EXTI_EnableFallingTrig_0_31(EXTI_InitStruct->Line_0_31);  /* 执行语句 */
            break;                                      /* 跳出 */
          case LL_EXTI_TRIGGER_RISING_FALLING:          /* case标签 */
            LL_EXTI_EnableRisingTrig_0_31(EXTI_InitStruct->Line_0_31);  /* 执行语句 */
            LL_EXTI_EnableFallingTrig_0_31(EXTI_InitStruct->Line_0_31);  /* 执行语句 */
            break;                                      /* 跳出 */
          default:                                      /* default分支 */
            status = ERROR;                             /* 赋值或初始化 */
            break;                                      /* 跳出 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /* DISABLE LineCommand */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* De-configure EXTI Lines in range from 0 to 31 */
    LL_EXTI_DisableIT_0_31(EXTI_InitStruct->Line_0_31);  /* 执行语句 */
    LL_EXTI_DisableEvent_0_31(EXTI_InitStruct->Line_0_31);  /* 执行语句 */
  }                                                     /* 代码块结束 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Set each @ref LL_EXTI_InitTypeDef field to default value.
  * @param  EXTI_InitStruct Pointer to a @ref LL_EXTI_InitTypeDef structure.
  * @retval None
  */
void LL_EXTI_StructInit(LL_EXTI_InitTypeDef *EXTI_InitStruct)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  EXTI_InitStruct->Line_0_31      = LL_EXTI_LINE_NONE;  /* 赋值或初始化 */
  EXTI_InitStruct->LineCommand    = DISABLE;            /* 赋值或初始化 */
  EXTI_InitStruct->Mode           = LL_EXTI_MODE_IT;    /* 赋值或初始化 */
  EXTI_InitStruct->Trigger        = LL_EXTI_TRIGGER_FALLING;  /* 赋值或初始化 */
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

#endif /* defined (EXTI) */

/**
  * @}
  */

#endif /* USE_FULL_LL_DRIVER */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
