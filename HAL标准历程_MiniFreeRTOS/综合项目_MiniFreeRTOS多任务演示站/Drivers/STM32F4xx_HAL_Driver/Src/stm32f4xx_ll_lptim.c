/* 文件: stm32f4xx_ll_lptim.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_ll_lptim.c
  * @author  MCD Application Team
  * @brief   LPTIM LL module driver.
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
#include "stm32f4xx_ll_lptim.h"                         /* 包含 STM32/HAL 头文件 */
#include "stm32f4xx_ll_bus.h"                           /* 包含 STM32/HAL 头文件 */
#include "stm32f4xx_ll_rcc.h"                           /* 包含 STM32/HAL 头文件 */


#ifdef  USE_FULL_ASSERT                                 /* 条件编译 */
#include "stm32_assert.h"                               /* 包含 STM32/HAL 头文件 */
#else                                                   /* 条件编译否则 */
#define assert_param(expr) ((void)0U)                   /* 定义宏 */
#endif                                                  /* 条件编译结束 */

/** @addtogroup STM32F4xx_LL_Driver
  * @{
  */

#if defined (LPTIM1)                                    /* 条件编译 */

/** @addtogroup LPTIM_LL
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/** @addtogroup LPTIM_LL_Private_Macros
  * @{
  */
#define IS_LL_LPTIM_CLOCK_SOURCE(__VALUE__) (((__VALUE__) == LL_LPTIM_CLK_SOURCE_INTERNAL) \
                                             || ((__VALUE__) == LL_LPTIM_CLK_SOURCE_EXTERNAL))  /* 源代码 */

#define IS_LL_LPTIM_CLOCK_PRESCALER(__VALUE__) (((__VALUE__) == LL_LPTIM_PRESCALER_DIV1)   \
                                                || ((__VALUE__) == LL_LPTIM_PRESCALER_DIV2)   \
                                                || ((__VALUE__) == LL_LPTIM_PRESCALER_DIV4)   \
                                                || ((__VALUE__) == LL_LPTIM_PRESCALER_DIV8)   \
                                                || ((__VALUE__) == LL_LPTIM_PRESCALER_DIV16)  \
                                                || ((__VALUE__) == LL_LPTIM_PRESCALER_DIV32)  \
                                                || ((__VALUE__) == LL_LPTIM_PRESCALER_DIV64)  \
                                                || ((__VALUE__) == LL_LPTIM_PRESCALER_DIV128))  /* 源代码 */

#define IS_LL_LPTIM_WAVEFORM(__VALUE__) (((__VALUE__) == LL_LPTIM_OUTPUT_WAVEFORM_PWM) \
                                         || ((__VALUE__) == LL_LPTIM_OUTPUT_WAVEFORM_SETONCE))  /* 源代码 */

#define IS_LL_LPTIM_OUTPUT_POLARITY(__VALUE__) (((__VALUE__) == LL_LPTIM_OUTPUT_POLARITY_REGULAR) \
                                                || ((__VALUE__) == LL_LPTIM_OUTPUT_POLARITY_INVERSE))  /* 源代码 */
/**
  * @}
  */


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/** @defgroup LPTIM_Private_Functions LPTIM Private Functions
  * @{
  */
/**
  * @}
  */
/* Exported functions --------------------------------------------------------*/
/** @addtogroup LPTIM_LL_Exported_Functions
  * @{
  */

/** @addtogroup LPTIM_LL_EF_Init
  * @{
  */

/**
  * @brief  Set LPTIMx registers to their reset values.
  * @param  LPTIMx LP Timer instance
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: LPTIMx registers are de-initialized
  *          - ERROR: invalid LPTIMx instance
  */
ErrorStatus LL_LPTIM_DeInit(LPTIM_TypeDef *LPTIMx)      /* 源代码 */
{                                                       /* 代码块开始 */
  ErrorStatus result = SUCCESS;                         /* 赋值或初始化 */

  /* Check the parameters */
  assert_param(IS_LPTIM_INSTANCE(LPTIMx));              /* 执行语句 */

  if (LPTIMx == LPTIM1)                                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_LPTIM1);  /* 执行语句 */
    LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_LPTIM1);  /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    result = ERROR;                                     /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return result;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Set each fields of the LPTIM_InitStruct structure to its default
  *         value.
  * @param  LPTIM_InitStruct pointer to a @ref LL_LPTIM_InitTypeDef structure
  * @retval None
  */
void LL_LPTIM_StructInit(LL_LPTIM_InitTypeDef *LPTIM_InitStruct)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Set the default configuration */
  LPTIM_InitStruct->ClockSource = LL_LPTIM_CLK_SOURCE_INTERNAL;  /* 赋值或初始化 */
  LPTIM_InitStruct->Prescaler   = LL_LPTIM_PRESCALER_DIV1;  /* 赋值或初始化 */
  LPTIM_InitStruct->Waveform    = LL_LPTIM_OUTPUT_WAVEFORM_PWM;  /* 赋值或初始化 */
  LPTIM_InitStruct->Polarity    = LL_LPTIM_OUTPUT_POLARITY_REGULAR;  /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configure the LPTIMx peripheral according to the specified parameters.
  * @note LL_LPTIM_Init can only be called when the LPTIM instance is disabled.
  * @note LPTIMx can be disabled using unitary function @ref LL_LPTIM_Disable().
  * @param  LPTIMx LP Timer Instance
  * @param  LPTIM_InitStruct pointer to a @ref LL_LPTIM_InitTypeDef structure
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: LPTIMx instance has been initialized
  *          - ERROR: LPTIMx instance hasn't been initialized
  */
ErrorStatus LL_LPTIM_Init(LPTIM_TypeDef *LPTIMx, LL_LPTIM_InitTypeDef *LPTIM_InitStruct)  /* 源代码 */
{                                                       /* 代码块开始 */
  ErrorStatus result = SUCCESS;                         /* 赋值或初始化 */
  /* Check the parameters */
  assert_param(IS_LPTIM_INSTANCE(LPTIMx));              /* 执行语句 */
  assert_param(IS_LL_LPTIM_CLOCK_SOURCE(LPTIM_InitStruct->ClockSource));  /* 执行语句 */
  assert_param(IS_LL_LPTIM_CLOCK_PRESCALER(LPTIM_InitStruct->Prescaler));  /* 执行语句 */
  assert_param(IS_LL_LPTIM_WAVEFORM(LPTIM_InitStruct->Waveform));  /* 执行语句 */
  assert_param(IS_LL_LPTIM_OUTPUT_POLARITY(LPTIM_InitStruct->Polarity));  /* 执行语句 */

  /* The LPTIMx_CFGR register must only be modified when the LPTIM is disabled
     (ENABLE bit is reset to 0).
  */
  if (LL_LPTIM_IsEnabled(LPTIMx) == 1UL)                /* 条件判断 */
  {                                                     /* 代码块开始 */
    result = ERROR;                                     /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Set CKSEL bitfield according to ClockSource value */
    /* Set PRESC bitfield according to Prescaler value */
    /* Set WAVE bitfield according to Waveform value */
    /* Set WAVEPOL bitfield according to Polarity value */
    MODIFY_REG(LPTIMx->CFGR,                            /* 列表续行 */
               (LPTIM_CFGR_CKSEL | LPTIM_CFGR_PRESC | LPTIM_CFGR_WAVE | LPTIM_CFGR_WAVPOL),  /* 列表续行 */
               LPTIM_InitStruct->ClockSource | \
               LPTIM_InitStruct->Prescaler | \
               LPTIM_InitStruct->Waveform | \
               LPTIM_InitStruct->Polarity);             /* 执行语句 */
  }                                                     /* 代码块结束 */

  return result;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Disable the LPTIM instance
  * @rmtoll CR           ENABLE        LL_LPTIM_Disable
  * @param  LPTIMx Low-Power Timer instance
  * @note   The following sequence is required to solve LPTIM disable HW limitation.
  *         Please check Errata Sheet ES0335 for more details under "MCU may remain
  *         stuck in LPTIM interrupt when entering Stop mode" section.
  * @retval None
  */
void LL_LPTIM_Disable(LPTIM_TypeDef *LPTIMx)            /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  LL_RCC_ClocksTypeDef rcc_clock;                       /* 执行语句 */
  uint32_t tmpclksource = 0;                            /* 32位无符号变量/函数 */
  uint32_t tmpIER;                                      /* 32位无符号变量/函数 */
  uint32_t tmpCFGR;                                     /* 32位无符号变量/函数 */
  uint32_t tmpCMP;                                      /* 32位无符号变量/函数 */
  uint32_t tmpARR;                                      /* 32位无符号变量/函数 */
  uint32_t tmpOR;                                       /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_LPTIM_INSTANCE(LPTIMx));              /* 执行语句 */

  __disable_irq();                                      /* 执行语句 */

  /********** Save LPTIM Config *********/
  /* Save LPTIM source clock */
  switch ((uint32_t)LPTIMx)                             /* switch分支 */
  {                                                     /* 代码块开始 */
    case LPTIM1_BASE:                                   /* case标签 */
      tmpclksource = LL_RCC_GetLPTIMClockSource(LL_RCC_LPTIM1_CLKSOURCE);  /* 赋值或初始化 */
      break;                                            /* 跳出 */
    default:                                            /* default分支 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  /* Save LPTIM configuration registers */
  tmpIER = LPTIMx->IER;                                 /* 赋值或初始化 */
  tmpCFGR = LPTIMx->CFGR;                               /* 赋值或初始化 */
  tmpCMP = LPTIMx->CMP;                                 /* 赋值或初始化 */
  tmpARR = LPTIMx->ARR;                                 /* 赋值或初始化 */
  tmpOR = LPTIMx->OR;                                   /* 赋值或初始化 */

  /************* Reset LPTIM ************/
  (void)LL_LPTIM_DeInit(LPTIMx);                        /* 执行语句 */

  /********* Restore LPTIM Config *******/
  LL_RCC_GetSystemClocksFreq(&rcc_clock);               /* 执行语句 */

  if ((tmpCMP != 0UL) || (tmpARR != 0UL))               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Force LPTIM source kernel clock from APB */
    switch ((uint32_t)LPTIMx)                           /* switch分支 */
    {                                                   /* 代码块开始 */
      case LPTIM1_BASE:                                 /* case标签 */
        LL_RCC_SetLPTIMClockSource(LL_RCC_LPTIM1_CLKSOURCE_PCLK1);  /* 执行语句 */
        break;                                          /* 跳出 */
      default:                                          /* default分支 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */

    if (tmpCMP != 0UL)                                  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Restore CMP and ARR registers (LPTIM should be enabled first) */
      LPTIMx->CR |= LPTIM_CR_ENABLE;                    /* 赋值或初始化 */
      LPTIMx->CMP = tmpCMP;                             /* 赋值或初始化 */

      /* Polling on CMP write ok status after above restore operation */
      do                                                /* do-while循环 */
      {                                                 /* 代码块开始 */
        rcc_clock.SYSCLK_Frequency--; /* Used for timeout */
      } while (((LL_LPTIM_IsActiveFlag_CMPOK(LPTIMx) != 1UL)) && ((rcc_clock.SYSCLK_Frequency) > 0UL));  /* 代码块结束 */

      LL_LPTIM_ClearFlag_CMPOK(LPTIMx);                 /* 执行语句 */
    }                                                   /* 代码块结束 */

    if (tmpARR != 0UL)                                  /* 条件判断 */
    {                                                   /* 代码块开始 */
      LPTIMx->CR |= LPTIM_CR_ENABLE;                    /* 赋值或初始化 */
      LPTIMx->ARR = tmpARR;                             /* 赋值或初始化 */

      LL_RCC_GetSystemClocksFreq(&rcc_clock);           /* 执行语句 */
      /* Polling on ARR write ok status after above restore operation */
      do                                                /* do-while循环 */
      {                                                 /* 代码块开始 */
        rcc_clock.SYSCLK_Frequency--; /* Used for timeout */
      }                                                 /* 代码块结束 */
      while (((LL_LPTIM_IsActiveFlag_ARROK(LPTIMx) != 1UL)) && ((rcc_clock.SYSCLK_Frequency) > 0UL));  /* while循环 */

      LL_LPTIM_ClearFlag_ARROK(LPTIMx);                 /* 执行语句 */
    }                                                   /* 代码块结束 */


    /* Restore LPTIM source kernel clock */
    LL_RCC_SetLPTIMClockSource(tmpclksource);           /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Restore configuration registers (LPTIM should be disabled first) */
  LPTIMx->CR &= ~(LPTIM_CR_ENABLE);                     /* 赋值或初始化 */
  LPTIMx->IER = tmpIER;                                 /* 赋值或初始化 */
  LPTIMx->CFGR = tmpCFGR;                               /* 赋值或初始化 */
  LPTIMx->OR = tmpOR;                                   /* 赋值或初始化 */

  __enable_irq();                                       /* 执行语句 */
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

#endif /* LPTIM1 */

/**
  * @}
  */

#endif /* USE_FULL_LL_DRIVER */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
