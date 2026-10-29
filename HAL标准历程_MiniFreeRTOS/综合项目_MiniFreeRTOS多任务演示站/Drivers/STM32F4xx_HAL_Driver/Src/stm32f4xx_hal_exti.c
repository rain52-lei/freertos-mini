/* 文件: stm32f4xx_hal_exti.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_exti.c
  * @author  MCD Application Team
  * @brief   EXTI HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Extended Interrupts and events controller (EXTI) peripheral:
  *           + Initialization and de-initialization functions
  *           + IO operation functions
  *
  @verbatim
  ==============================================================================
                    ##### EXTI Peripheral features #####
  ==============================================================================
  [..]
    (+) Each Exti line can be configured within this driver.

    (+) Exti line can be configured in 3 different modes
        (++) Interrupt
        (++) Event
        (++) Both of them

    (+) Configurable Exti lines can be configured with 3 different triggers
        (++) Rising
        (++) Falling
        (++) Both of them

    (+) When set in interrupt mode, configurable Exti lines have two different
        interrupts pending registers which allow to distinguish which transition
        occurs:
        (++) Rising edge pending interrupt
        (++) Falling

    (+) Exti lines 0 to 15 are linked to gpio pin number 0 to 15. Gpio port can
        be selected through multiplexer.

                     ##### How to use this driver #####
  ==============================================================================
  [..]

    (#) Configure the EXTI line using HAL_EXTI_SetConfigLine().
        (++) Choose the interrupt line number by setting "Line" member from
             EXTI_ConfigTypeDef structure.
        (++) Configure the interrupt and/or event mode using "Mode" member from
             EXTI_ConfigTypeDef structure.
        (++) For configurable lines, configure rising and/or falling trigger
             "Trigger" member from EXTI_ConfigTypeDef structure.
        (++) For Exti lines linked to gpio, choose gpio port using "GPIOSel"
             member from GPIO_InitTypeDef structure.

    (#) Get current Exti configuration of a dedicated line using
        HAL_EXTI_GetConfigLine().
        (++) Provide exiting handle as parameter.
        (++) Provide pointer on EXTI_ConfigTypeDef structure as second parameter.

    (#) Clear Exti configuration of a dedicated line using HAL_EXTI_GetConfigLine().
        (++) Provide exiting handle as parameter.

    (#) Register callback to treat Exti interrupts using HAL_EXTI_RegisterCallback().
        (++) Provide exiting handle as first parameter.
        (++) Provide which callback will be registered using one value from
             EXTI_CallbackIDTypeDef.
        (++) Provide callback function pointer.

    (#) Get interrupt pending bit using HAL_EXTI_GetPending().

    (#) Clear interrupt pending bit using HAL_EXTI_GetPending().

    (#) Generate software interrupt using HAL_EXTI_GenerateSWI().

  @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
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

/** @addtogroup EXTI
  * @{
  */
/** MISRA C:2012 deviation rule has been granted for following rule:
  * Rule-18.1_b - Medium: Array `EXTICR' 1st subscript interval [0,7] may be out
  * of bounds [0,3] in following API :
  * HAL_EXTI_SetConfigLine
  * HAL_EXTI_GetConfigLine
  * HAL_EXTI_ClearConfigLine
  */

#ifdef HAL_EXTI_MODULE_ENABLED                          /* 条件编译 */

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/** @defgroup EXTI_Private_Constants EXTI Private Constants
  * @{
  */

/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/** @addtogroup EXTI_Exported_Functions
  * @{
  */

/** @addtogroup EXTI_Exported_Functions_Group1
  *  @brief    Configuration functions
  *
@verbatim
 ===============================================================================
              ##### Configuration functions #####
 ===============================================================================

@endverbatim
  * @{
  */

/**
  * @brief  Set configuration of a dedicated Exti line.
  * @param  hexti Exti handle.
  * @param  pExtiConfig Pointer on EXTI configuration to be set.
  * @retval HAL Status.
  */
HAL_StatusTypeDef HAL_EXTI_SetConfigLine(EXTI_HandleTypeDef *hexti, EXTI_ConfigTypeDef *pExtiConfig)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t regval;                                      /* 32位无符号变量/函数 */
  uint32_t linepos;                                     /* 32位无符号变量/函数 */
  uint32_t maskline;                                    /* 32位无符号变量/函数 */

  /* Check null pointer */
  if ((hexti == NULL) || (pExtiConfig == NULL))         /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check parameters */
  assert_param(IS_EXTI_LINE(pExtiConfig->Line));        /* 执行语句 */
  assert_param(IS_EXTI_MODE(pExtiConfig->Mode));        /* 执行语句 */

  /* Assign line number to handle */
  hexti->Line = pExtiConfig->Line;                      /* 赋值或初始化 */

  /* Compute line mask */
  linepos = (pExtiConfig->Line & EXTI_PIN_MASK);        /* 赋值或初始化 */
  maskline = (1uL << linepos);                          /* 赋值或初始化 */

  /* Configure triggers for configurable lines */
  if ((pExtiConfig->Line & EXTI_CONFIG) != 0x00u)       /* 条件判断 */
  {                                                     /* 代码块开始 */
    assert_param(IS_EXTI_TRIGGER(pExtiConfig->Trigger));  /* 执行语句 */

    /* Configure rising trigger */
    /* Mask or set line */
    if ((pExtiConfig->Trigger & EXTI_TRIGGER_RISING) != 0x00u)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      EXTI->RTSR |= maskline;                           /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      EXTI->RTSR &= ~maskline;                          /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Configure falling trigger */
    /* Mask or set line */
    if ((pExtiConfig->Trigger & EXTI_TRIGGER_FALLING) != 0x00u)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      EXTI->FTSR |= maskline;                           /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      EXTI->FTSR &= ~maskline;                          /* 赋值或初始化 */
    }                                                   /* 代码块结束 */


    /* Configure gpio port selection in case of gpio exti line */
    if ((pExtiConfig->Line & EXTI_GPIO) == EXTI_GPIO)   /* 条件判断 */
    {                                                   /* 代码块开始 */
      assert_param(IS_EXTI_GPIO_PORT(pExtiConfig->GPIOSel));  /* 执行语句 */
      assert_param(IS_EXTI_GPIO_PIN(linepos));          /* 执行语句 */

      regval = SYSCFG->EXTICR[linepos >> 2u];           /* 赋值或初始化 */
      regval &= ~(SYSCFG_EXTICR1_EXTI0 << (SYSCFG_EXTICR1_EXTI1_Pos * (linepos & 0x03u)));  /* 赋值或初始化 */
      regval |= (pExtiConfig->GPIOSel << (SYSCFG_EXTICR1_EXTI1_Pos * (linepos & 0x03u)));  /* 赋值或初始化 */
      SYSCFG->EXTICR[linepos >> 2u] = regval;           /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Configure interrupt mode : read current mode */
  /* Mask or set line */
  if ((pExtiConfig->Mode & EXTI_MODE_INTERRUPT) != 0x00u)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    EXTI->IMR |= maskline;                              /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    EXTI->IMR &= ~maskline;                             /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Configure event mode : read current mode */
  /* Mask or set line */
  if ((pExtiConfig->Mode & EXTI_MODE_EVENT) != 0x00u)   /* 条件判断 */
  {                                                     /* 代码块开始 */
    EXTI->EMR |= maskline;                              /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    EXTI->EMR &= ~maskline;                             /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Get configuration of a dedicated Exti line.
  * @param  hexti Exti handle.
  * @param  pExtiConfig Pointer on structure to store Exti configuration.
  * @retval HAL Status.
  */
HAL_StatusTypeDef HAL_EXTI_GetConfigLine(EXTI_HandleTypeDef *hexti, EXTI_ConfigTypeDef *pExtiConfig)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t regval;                                      /* 32位无符号变量/函数 */
  uint32_t linepos;                                     /* 32位无符号变量/函数 */
  uint32_t maskline;                                    /* 32位无符号变量/函数 */

  /* Check null pointer */
  if ((hexti == NULL) || (pExtiConfig == NULL))         /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the parameter */
  assert_param(IS_EXTI_LINE(hexti->Line));              /* 执行语句 */

  /* Store handle line number to configuration structure */
  pExtiConfig->Line = hexti->Line;                      /* 赋值或初始化 */

  /* Compute line mask */
  linepos = (pExtiConfig->Line & EXTI_PIN_MASK);        /* 赋值或初始化 */
  maskline = (1uL << linepos);                          /* 赋值或初始化 */

  /* 1] Get core mode : interrupt */

  /* Check if selected line is enable */
  if ((EXTI->IMR & maskline) != 0x00u)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    pExtiConfig->Mode = EXTI_MODE_INTERRUPT;            /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    pExtiConfig->Mode = EXTI_MODE_NONE;                 /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Get event mode */
  /* Check if selected line is enable */
  if ((EXTI->EMR & maskline) != 0x00u)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    pExtiConfig->Mode |= EXTI_MODE_EVENT;               /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* 2] Get trigger for configurable lines : rising */
  if ((pExtiConfig->Line & EXTI_CONFIG) != 0x00u)       /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check if configuration of selected line is enable */
    if ((EXTI->RTSR & maskline) != 0x00u)               /* 条件判断 */
    {                                                   /* 代码块开始 */
      pExtiConfig->Trigger = EXTI_TRIGGER_RISING;       /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      pExtiConfig->Trigger = EXTI_TRIGGER_NONE;         /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Get falling configuration */
    /* Check if configuration of selected line is enable */
    if ((EXTI->FTSR & maskline) != 0x00u)               /* 条件判断 */
    {                                                   /* 代码块开始 */
      pExtiConfig->Trigger |= EXTI_TRIGGER_FALLING;     /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Get Gpio port selection for gpio lines */
    if ((pExtiConfig->Line & EXTI_GPIO) == EXTI_GPIO)   /* 条件判断 */
    {                                                   /* 代码块开始 */
      assert_param(IS_EXTI_GPIO_PIN(linepos));          /* 执行语句 */

      regval = SYSCFG->EXTICR[linepos >> 2u];           /* 赋值或初始化 */
      pExtiConfig->GPIOSel = ((regval << (SYSCFG_EXTICR1_EXTI1_Pos * (3uL - (linepos & 0x03u)))) >> 24);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      pExtiConfig->GPIOSel = 0x00u;                     /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* No Trigger selected */
    pExtiConfig->Trigger = EXTI_TRIGGER_NONE;           /* 赋值或初始化 */
    pExtiConfig->GPIOSel = 0x00u;                       /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Clear whole configuration of a dedicated Exti line.
  * @param  hexti Exti handle.
  * @retval HAL Status.
  */
HAL_StatusTypeDef HAL_EXTI_ClearConfigLine(EXTI_HandleTypeDef *hexti)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t regval;                                      /* 32位无符号变量/函数 */
  uint32_t linepos;                                     /* 32位无符号变量/函数 */
  uint32_t maskline;                                    /* 32位无符号变量/函数 */

  /* Check null pointer */
  if (hexti == NULL)                                    /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the parameter */
  assert_param(IS_EXTI_LINE(hexti->Line));              /* 执行语句 */

  /* compute line mask */
  linepos = (hexti->Line & EXTI_PIN_MASK);              /* 赋值或初始化 */
  maskline = (1uL << linepos);                          /* 赋值或初始化 */

  /* 1] Clear interrupt mode */
  EXTI->IMR = (EXTI->IMR & ~maskline);                  /* 赋值或初始化 */

  /* 2] Clear event mode */
  EXTI->EMR = (EXTI->EMR & ~maskline);                  /* 赋值或初始化 */

  /* 3] Clear triggers in case of configurable lines */
  if ((hexti->Line & EXTI_CONFIG) != 0x00u)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    EXTI->RTSR = (EXTI->RTSR & ~maskline);              /* 赋值或初始化 */
    EXTI->FTSR = (EXTI->FTSR & ~maskline);              /* 赋值或初始化 */

    /* Get Gpio port selection for gpio lines */
    if ((hexti->Line & EXTI_GPIO) == EXTI_GPIO)         /* 条件判断 */
    {                                                   /* 代码块开始 */
      assert_param(IS_EXTI_GPIO_PIN(linepos));          /* 执行语句 */

      regval = SYSCFG->EXTICR[linepos >> 2u];           /* 赋值或初始化 */
      regval &= ~(SYSCFG_EXTICR1_EXTI0 << (SYSCFG_EXTICR1_EXTI1_Pos * (linepos & 0x03u)));  /* 赋值或初始化 */
      SYSCFG->EXTICR[linepos >> 2u] = regval;           /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Register callback for a dedicated Exti line.
  * @param  hexti Exti handle.
  * @param  CallbackID User callback identifier.
  *         This parameter can be one of @arg @ref EXTI_CallbackIDTypeDef values.
  * @param  pPendingCbfn function pointer to be stored as callback.
  * @retval HAL Status.
  */
HAL_StatusTypeDef HAL_EXTI_RegisterCallback(EXTI_HandleTypeDef *hexti, EXTI_CallbackIDTypeDef CallbackID, void (*pPendingCbfn)(void))  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  switch (CallbackID)                                   /* switch分支 */
  {                                                     /* 代码块开始 */
    case  HAL_EXTI_COMMON_CB_ID:                        /* case标签 */
      hexti->PendingCallback = pPendingCbfn;            /* 赋值或初始化 */
      break;                                            /* 跳出 */

    default:                                            /* default分支 */
      status = HAL_ERROR;                               /* 赋值或初始化 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Store line number as handle private field.
  * @param  hexti Exti handle.
  * @param  ExtiLine Exti line number.
  *         This parameter can be from 0 to @ref EXTI_LINE_NB.
  * @retval HAL Status.
  */
HAL_StatusTypeDef HAL_EXTI_GetHandle(EXTI_HandleTypeDef *hexti, uint32_t ExtiLine)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_EXTI_LINE(ExtiLine));                 /* 执行语句 */

  /* Check null pointer */
  if (hexti == NULL)                                    /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Store line number as handle private field */
    hexti->Line = ExtiLine;                             /* 赋值或初始化 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @addtogroup EXTI_Exported_Functions_Group2
  *  @brief EXTI IO functions.
  *
@verbatim
 ===============================================================================
                       ##### IO operation functions #####
 ===============================================================================

@endverbatim
  * @{
  */

/**
  * @brief  Handle EXTI interrupt request.
  * @param  hexti Exti handle.
  * @retval none.
  */
void HAL_EXTI_IRQHandler(EXTI_HandleTypeDef *hexti)     /* 中断服务函数 */
{                                                       /* 代码块开始 */
  uint32_t regval;                                      /* 32位无符号变量/函数 */
  uint32_t maskline;                                    /* 32位无符号变量/函数 */

  /* Compute line mask */
  maskline = (1uL << (hexti->Line & EXTI_PIN_MASK));    /* 赋值或初始化 */

  /* Get pending bit  */
  regval = (EXTI->PR & maskline);                       /* 赋值或初始化 */
  if (regval != 0x00u)                                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Clear pending bit */
    EXTI->PR = maskline;                                /* 赋值或初始化 */

    /* Call callback */
    if (hexti->PendingCallback != NULL)                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      hexti->PendingCallback();                         /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Get interrupt pending bit of a dedicated line.
  * @param  hexti Exti handle.
  * @param  Edge Specify which pending edge as to be checked.
  *         This parameter can be one of the following values:
  *           @arg @ref EXTI_TRIGGER_RISING_FALLING
  *         This parameter is kept for compatibility with other series.
  * @retval 1 if interrupt is pending else 0.
  */
uint32_t HAL_EXTI_GetPending(EXTI_HandleTypeDef *hexti, uint32_t Edge)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t regval;                                      /* 32位无符号变量/函数 */
  uint32_t linepos;                                     /* 32位无符号变量/函数 */
  uint32_t maskline;                                    /* 32位无符号变量/函数 */

  /* Check parameters */
  assert_param(IS_EXTI_LINE(hexti->Line));              /* 执行语句 */
  assert_param(IS_EXTI_CONFIG_LINE(hexti->Line));       /* 执行语句 */
  assert_param(IS_EXTI_PENDING_EDGE(Edge));             /* 执行语句 */

  /* Compute line mask */
  linepos = (hexti->Line & EXTI_PIN_MASK);              /* 赋值或初始化 */
  maskline = (1uL << linepos);                          /* 赋值或初始化 */

  /* return 1 if bit is set else 0 */
  regval = ((EXTI->PR & maskline) >> linepos);          /* 赋值或初始化 */
  return regval;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Clear interrupt pending bit of a dedicated line.
  * @param  hexti Exti handle.
  * @param  Edge Specify which pending edge as to be clear.
  *         This parameter can be one of the following values:
  *           @arg @ref EXTI_TRIGGER_RISING_FALLING
  *         This parameter is kept for compatibility with other series.
  * @retval None.
  */
void HAL_EXTI_ClearPending(EXTI_HandleTypeDef *hexti, uint32_t Edge)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  uint32_t maskline;                                    /* 32位无符号变量/函数 */

  /* Check parameters */
  assert_param(IS_EXTI_LINE(hexti->Line));              /* 执行语句 */
  assert_param(IS_EXTI_CONFIG_LINE(hexti->Line));       /* 执行语句 */
  assert_param(IS_EXTI_PENDING_EDGE(Edge));             /* 执行语句 */

  /* Compute line mask */
  maskline = (1uL << (hexti->Line & EXTI_PIN_MASK));    /* 赋值或初始化 */

  /* Clear Pending bit */
  EXTI->PR =  maskline;                                 /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  Generate a software interrupt for a dedicated line.
  * @param  hexti Exti handle.
  * @retval None.
  */
void HAL_EXTI_GenerateSWI(EXTI_HandleTypeDef *hexti)    /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  uint32_t maskline;                                    /* 32位无符号变量/函数 */

  /* Check parameters */
  assert_param(IS_EXTI_LINE(hexti->Line));              /* 执行语句 */
  assert_param(IS_EXTI_CONFIG_LINE(hexti->Line));       /* 执行语句 */

  /* Compute line mask */
  maskline = (1uL << (hexti->Line & EXTI_PIN_MASK));    /* 赋值或初始化 */

  /* Generate Software interrupt */
  EXTI->SWIER = maskline;                               /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/**
  * @}
  */

#endif /* HAL_EXTI_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
