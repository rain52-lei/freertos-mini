/* 文件: stm32f4xx_hal_sai_ex.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_sai_ex.c
  * @author  MCD Application Team
  * @brief   SAI Extension HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of SAI extension peripheral:
  *           + Extension features functions
  *
  @verbatim
  ==============================================================================
               ##### SAI peripheral extension features  #####
  ==============================================================================

  [..] Comparing to other previous devices, the SAI interface for STM32F446xx
       devices contains the following additional features :

       (+) Possibility to be clocked from PLLR

                     ##### How to use this driver #####
  ==============================================================================
  [..] This driver provides functions to manage several sources to clock SAI

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

/** @defgroup SAIEx SAIEx
  * @brief SAI Extension HAL module driver
  * @{
  */

#ifdef HAL_SAI_MODULE_ENABLED                           /* 条件编译 */

#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx) || \
    defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx) || defined(STM32F413xx) || \
    defined(STM32F423xx)                                /* 源代码 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* SAI registers Masks */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup SAI_Private_Functions  SAI Private Functions
  * @{
  */
/**
 * @}
 */

/* Exported functions --------------------------------------------------------*/
/** @defgroup SAIEx_Exported_Functions SAI Extended Exported Functions
  * @{
  */

/** @defgroup SAIEx_Exported_Functions_Group1 Extension features functions
  *  @brief   Extension features functions
  *
@verbatim
 ===============================================================================
                       ##### Extension features Functions #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to manage the possible
    SAI clock sources.

@endverbatim
  * @{
  */

/**
  * @brief  Configure SAI Block synchronization mode
  * @param  hsai pointer to a SAI_HandleTypeDef structure that contains
  *               the configuration information for SAI module.
  * @retval SAI Clock Input
  */
void SAI_BlockSynchroConfig(SAI_HandleTypeDef *hsai)    /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  uint32_t tmpregisterGCR;                              /* 32位无符号变量/函数 */

#if defined(STM32F446xx)                                /* 条件编译 */
  /* This setting must be done with both audio block (A & B) disabled         */
  switch (hsai->Init.SynchroExt)                        /* switch分支 */
  {                                                     /* 代码块开始 */
    case SAI_SYNCEXT_DISABLE :                          /* case标签 */
      tmpregisterGCR = 0U;                              /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case SAI_SYNCEXT_OUTBLOCKA_ENABLE :                 /* case标签 */
      tmpregisterGCR = SAI_GCR_SYNCOUT_0;               /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case SAI_SYNCEXT_OUTBLOCKB_ENABLE :                 /* case标签 */
      tmpregisterGCR = SAI_GCR_SYNCOUT_1;               /* 赋值或初始化 */
      break;                                            /* 跳出 */
    default:                                            /* default分支 */
      tmpregisterGCR = 0U;                              /* 赋值或初始化 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  if ((hsai->Init.Synchro) == SAI_SYNCHRONOUS_EXT_SAI2)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    tmpregisterGCR |= SAI_GCR_SYNCIN_0;                 /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if ((hsai->Instance == SAI1_Block_A) || (hsai->Instance == SAI1_Block_B))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    SAI1->GCR = tmpregisterGCR;                         /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    SAI2->GCR = tmpregisterGCR;                         /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
#endif /* STM32F446xx */
#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx) || \
    defined(STM32F469xx) || defined(STM32F479xx) || defined(STM32F413xx) || defined(STM32F423xx)  /* 源代码 */
  /* This setting must be done with both audio block (A & B) disabled         */
  switch (hsai->Init.SynchroExt)                        /* switch分支 */
  {                                                     /* 代码块开始 */
    case SAI_SYNCEXT_DISABLE :                          /* case标签 */
      tmpregisterGCR = 0U;                              /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case SAI_SYNCEXT_OUTBLOCKA_ENABLE :                 /* case标签 */
      tmpregisterGCR = SAI_GCR_SYNCOUT_0;               /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case SAI_SYNCEXT_OUTBLOCKB_ENABLE :                 /* case标签 */
      tmpregisterGCR = SAI_GCR_SYNCOUT_1;               /* 赋值或初始化 */
      break;                                            /* 跳出 */
    default:                                            /* default分支 */
      tmpregisterGCR = 0U;                              /* 赋值或初始化 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */
  SAI1->GCR = tmpregisterGCR;                           /* 赋值或初始化 */
#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx || STM32F469xx || STM32F479xx || STM32F413xx || STM32F423xx */
}                                                       /* 代码块结束 */
/**
* @brief  Get SAI Input Clock based on SAI source clock selection
* @param  hsai pointer to a SAI_HandleTypeDef structure that contains
*               the configuration information for SAI module.
* @retval SAI Clock Input
*/
uint32_t SAI_GetInputClock(SAI_HandleTypeDef *hsai)     /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  /* This variable used to store the SAI_CK_x (value in Hz) */
  uint32_t saiclocksource = 0U;                         /* 32位无符号变量/函数 */

#if defined(STM32F446xx)                                /* 条件编译 */
  if ((hsai->Instance == SAI1_Block_A) || (hsai->Instance == SAI1_Block_B))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    saiclocksource = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_SAI1);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else /* SAI2_Block_A || SAI2_Block_B*/
  {                                                     /* 代码块开始 */
    saiclocksource = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_SAI2);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
#endif /* STM32F446xx */
#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx) || \
  defined(STM32F469xx) || defined(STM32F479xx) || defined(STM32F413xx) || defined(STM32F423xx)  /* 源代码 */
  uint32_t vcoinput = 0U, tmpreg = 0U;                  /* 32位无符号变量/函数 */

  /* Check the SAI Block parameters */
  assert_param(IS_SAI_CLK_SOURCE(hsai->Init.ClockSource));  /* 执行语句 */

  /* SAI Block clock source selection */
  if (hsai->Instance == SAI1_Block_A)                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    __HAL_RCC_SAI_BLOCKACLKSOURCE_CONFIG(hsai->Init.ClockSource);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    __HAL_RCC_SAI_BLOCKBCLKSOURCE_CONFIG((uint32_t)(hsai->Init.ClockSource << 2U));  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* VCO Input Clock value calculation */
  if ((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLSOURCE_HSI)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* In Case the PLL Source is HSI (Internal Clock) */
    vcoinput = (HSI_VALUE / (uint32_t)(RCC->PLLCFGR & RCC_PLLCFGR_PLLM));  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* In Case the PLL Source is HSE (External Clock) */
    vcoinput = ((HSE_VALUE / (uint32_t)(RCC->PLLCFGR & RCC_PLLCFGR_PLLM)));  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
#if defined(STM32F413xx) || defined(STM32F423xx)        /* 条件编译 */
  /* SAI_CLK_x : SAI Block Clock configuration for different clock sources selected */
  if (hsai->Init.ClockSource == SAI_CLKSOURCE_PLLR)     /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Configure the PLLI2S division factor */
    /* PLL_VCO Input  = PLL_SOURCE/PLLM */
    /* PLL_VCO Output = PLL_VCO Input * PLLN */
    /* SAI_CLK(first level) = PLL_VCO Output/PLLR */
    tmpreg = (RCC->PLLCFGR & RCC_PLLCFGR_PLLR) >> 28U;  /* 赋值或初始化 */
    saiclocksource = (vcoinput * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6U)) / (tmpreg);  /* 赋值或初始化 */

    /* SAI_CLK_x = SAI_CLK(first level)/PLLDIVR */
    tmpreg = (((RCC->DCKCFGR & RCC_DCKCFGR_PLLDIVR) >> 8U) + 1U);  /* 赋值或初始化 */

    saiclocksource = saiclocksource / (tmpreg);         /* 赋值或初始化 */

  }                                                     /* 代码块结束 */
  else if (hsai->Init.ClockSource == SAI_CLKSOURCE_PLLI2S)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* Configure the PLLI2S division factor */
    /* PLLI2S_VCO Input  = PLL_SOURCE/PLLM */
    /* PLLI2S_VCO Output = PLLI2S_VCO Input * PLLI2SN */
    /* SAI_CLK(first level) = PLLI2S_VCO Output/PLLI2SR */
    tmpreg = (RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SR) >> 28U;  /* 赋值或初始化 */
    saiclocksource = (vcoinput * ((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SN) >> 6U)) / (tmpreg);  /* 赋值或初始化 */

    /* SAI_CLK_x = SAI_CLK(first level)/PLLI2SDIVR */
    tmpreg = ((RCC->DCKCFGR & RCC_DCKCFGR_PLLI2SDIVR) + 1U);  /* 赋值或初始化 */
    saiclocksource = saiclocksource / (tmpreg);         /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if (hsai->Init.ClockSource == SAI_CLKSOURCE_HS)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    if ((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLSOURCE_HSE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Get the I2S source clock value */
      saiclocksource = (uint32_t)(HSE_VALUE);           /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Get the I2S source clock value */
      saiclocksource = (uint32_t)(HSI_VALUE);           /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else /* sConfig->ClockSource == SAI_CLKSource_Ext */
  {                                                     /* 代码块开始 */
    saiclocksource = EXTERNAL_CLOCK_VALUE;              /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
#else                                                   /* 条件编译否则 */
  /* SAI_CLK_x : SAI Block Clock configuration for different clock sources selected */
  if (hsai->Init.ClockSource == SAI_CLKSOURCE_PLLSAI)   /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Configure the PLLI2S division factor */
    /* PLLSAI_VCO Input  = PLL_SOURCE/PLLM */
    /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN */
    /* SAI_CLK(first level) = PLLSAI_VCO Output/PLLSAIQ */
    tmpreg = (RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIQ) >> 24U;  /* 赋值或初始化 */
    saiclocksource = (vcoinput * ((RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIN) >> 6U)) / (tmpreg);  /* 赋值或初始化 */

    /* SAI_CLK_x = SAI_CLK(first level)/PLLSAIDIVQ */
    tmpreg = (((RCC->DCKCFGR & RCC_DCKCFGR_PLLSAIDIVQ) >> 8U) + 1U);  /* 赋值或初始化 */
    saiclocksource = saiclocksource / (tmpreg);         /* 赋值或初始化 */

  }                                                     /* 代码块结束 */
  else if (hsai->Init.ClockSource == SAI_CLKSOURCE_PLLI2S)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* Configure the PLLI2S division factor */
    /* PLLI2S_VCO Input  = PLL_SOURCE/PLLM */
    /* PLLI2S_VCO Output = PLLI2S_VCO Input * PLLI2SN */
    /* SAI_CLK(first level) = PLLI2S_VCO Output/PLLI2SQ */
    tmpreg = (RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SQ) >> 24U;  /* 赋值或初始化 */
    saiclocksource = (vcoinput * ((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SN) >> 6U)) / (tmpreg);  /* 赋值或初始化 */

    /* SAI_CLK_x = SAI_CLK(first level)/PLLI2SDIVQ */
    tmpreg = ((RCC->DCKCFGR & RCC_DCKCFGR_PLLI2SDIVQ) + 1U);  /* 赋值或初始化 */
    saiclocksource = saiclocksource / (tmpreg);         /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else /* sConfig->ClockSource == SAI_CLKSource_Ext */
  {                                                     /* 代码块开始 */
    /* Enable the External Clock selection */
    __HAL_RCC_I2S_CONFIG(RCC_I2SCLKSOURCE_EXT);         /* HAL辅助宏/函数 */

    saiclocksource = EXTERNAL_CLOCK_VALUE;              /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
#endif /* STM32F413xx || STM32F423xx */
#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx || STM32F469xx || STM32F479xx || STM32F413xx || STM32F423xx */
  /* the return result is the value of SAI clock */
  return saiclocksource;                                /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/**
  * @}
  */

#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx  || STM32F446xx || STM32F469xx || STM32F479xx || STM32F413xx || STM32F423xx */
#endif /* HAL_SAI_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
