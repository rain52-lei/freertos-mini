/* 文件: stm32f4xx_ll_tim.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_ll_tim.c
  * @author  MCD Application Team
  * @brief   TIM LL module driver.
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
#include "stm32f4xx_ll_tim.h"                           /* 包含 STM32/HAL 头文件 */
#include "stm32f4xx_ll_bus.h"                           /* 包含 STM32/HAL 头文件 */

#ifdef  USE_FULL_ASSERT                                 /* 条件编译 */
#include "stm32_assert.h"                               /* 包含 STM32/HAL 头文件 */
#else                                                   /* 条件编译否则 */
#define assert_param(expr) ((void)0U)                   /* 定义宏 */
#endif /* USE_FULL_ASSERT */

/** @addtogroup STM32F4xx_LL_Driver
  * @{
  */

#if defined (TIM1) || defined (TIM2) || defined (TIM3) || defined (TIM4) || defined (TIM5) || defined (TIM6) || defined (TIM7) || defined (TIM8) || defined (TIM9) || defined (TIM10) || defined (TIM11) || defined (TIM12) || defined (TIM13) || defined (TIM14)  /* 条件编译 */

/** @addtogroup TIM_LL
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/** @addtogroup TIM_LL_Private_Macros
  * @{
  */
#define IS_LL_TIM_COUNTERMODE(__VALUE__) (((__VALUE__) == LL_TIM_COUNTERMODE_UP) \
                                          || ((__VALUE__) == LL_TIM_COUNTERMODE_DOWN) \
                                          || ((__VALUE__) == LL_TIM_COUNTERMODE_CENTER_UP) \
                                          || ((__VALUE__) == LL_TIM_COUNTERMODE_CENTER_DOWN) \
                                          || ((__VALUE__) == LL_TIM_COUNTERMODE_CENTER_UP_DOWN))  /* 源代码 */

#define IS_LL_TIM_CLOCKDIVISION(__VALUE__) (((__VALUE__) == LL_TIM_CLOCKDIVISION_DIV1) \
                                            || ((__VALUE__) == LL_TIM_CLOCKDIVISION_DIV2) \
                                            || ((__VALUE__) == LL_TIM_CLOCKDIVISION_DIV4))  /* 源代码 */

#define IS_LL_TIM_OCMODE(__VALUE__) (((__VALUE__) == LL_TIM_OCMODE_FROZEN) \
                                     || ((__VALUE__) == LL_TIM_OCMODE_ACTIVE) \
                                     || ((__VALUE__) == LL_TIM_OCMODE_INACTIVE) \
                                     || ((__VALUE__) == LL_TIM_OCMODE_TOGGLE) \
                                     || ((__VALUE__) == LL_TIM_OCMODE_FORCED_INACTIVE) \
                                     || ((__VALUE__) == LL_TIM_OCMODE_FORCED_ACTIVE) \
                                     || ((__VALUE__) == LL_TIM_OCMODE_PWM1) \
                                     || ((__VALUE__) == LL_TIM_OCMODE_PWM2))  /* 源代码 */

#define IS_LL_TIM_OCSTATE(__VALUE__) (((__VALUE__) == LL_TIM_OCSTATE_DISABLE) \
                                      || ((__VALUE__) == LL_TIM_OCSTATE_ENABLE))  /* 源代码 */

#define IS_LL_TIM_OCPOLARITY(__VALUE__) (((__VALUE__) == LL_TIM_OCPOLARITY_HIGH) \
                                         || ((__VALUE__) == LL_TIM_OCPOLARITY_LOW))  /* 源代码 */

#define IS_LL_TIM_OCIDLESTATE(__VALUE__) (((__VALUE__) == LL_TIM_OCIDLESTATE_LOW) \
                                          || ((__VALUE__) == LL_TIM_OCIDLESTATE_HIGH))  /* 源代码 */

#define IS_LL_TIM_ACTIVEINPUT(__VALUE__) (((__VALUE__) == LL_TIM_ACTIVEINPUT_DIRECTTI) \
                                          || ((__VALUE__) == LL_TIM_ACTIVEINPUT_INDIRECTTI) \
                                          || ((__VALUE__) == LL_TIM_ACTIVEINPUT_TRC))  /* 源代码 */

#define IS_LL_TIM_ICPSC(__VALUE__) (((__VALUE__) == LL_TIM_ICPSC_DIV1) \
                                    || ((__VALUE__) == LL_TIM_ICPSC_DIV2) \
                                    || ((__VALUE__) == LL_TIM_ICPSC_DIV4) \
                                    || ((__VALUE__) == LL_TIM_ICPSC_DIV8))  /* 源代码 */

#define IS_LL_TIM_IC_FILTER(__VALUE__) (((__VALUE__) == LL_TIM_IC_FILTER_FDIV1) \
                                        || ((__VALUE__) == LL_TIM_IC_FILTER_FDIV1_N2) \
                                        || ((__VALUE__) == LL_TIM_IC_FILTER_FDIV1_N4) \
                                        || ((__VALUE__) == LL_TIM_IC_FILTER_FDIV1_N8) \
                                        || ((__VALUE__) == LL_TIM_IC_FILTER_FDIV2_N6) \
                                        || ((__VALUE__) == LL_TIM_IC_FILTER_FDIV2_N8) \
                                        || ((__VALUE__) == LL_TIM_IC_FILTER_FDIV4_N6) \
                                        || ((__VALUE__) == LL_TIM_IC_FILTER_FDIV4_N8) \
                                        || ((__VALUE__) == LL_TIM_IC_FILTER_FDIV8_N6) \
                                        || ((__VALUE__) == LL_TIM_IC_FILTER_FDIV8_N8) \
                                        || ((__VALUE__) == LL_TIM_IC_FILTER_FDIV16_N5) \
                                        || ((__VALUE__) == LL_TIM_IC_FILTER_FDIV16_N6) \
                                        || ((__VALUE__) == LL_TIM_IC_FILTER_FDIV16_N8) \
                                        || ((__VALUE__) == LL_TIM_IC_FILTER_FDIV32_N5) \
                                        || ((__VALUE__) == LL_TIM_IC_FILTER_FDIV32_N6) \
                                        || ((__VALUE__) == LL_TIM_IC_FILTER_FDIV32_N8))  /* 源代码 */

#define IS_LL_TIM_IC_POLARITY(__VALUE__) (((__VALUE__) == LL_TIM_IC_POLARITY_RISING) \
                                          || ((__VALUE__) == LL_TIM_IC_POLARITY_FALLING) \
                                          || ((__VALUE__) == LL_TIM_IC_POLARITY_BOTHEDGE))  /* 源代码 */

#define IS_LL_TIM_ENCODERMODE(__VALUE__) (((__VALUE__) == LL_TIM_ENCODERMODE_X2_TI1) \
                                          || ((__VALUE__) == LL_TIM_ENCODERMODE_X2_TI2) \
                                          || ((__VALUE__) == LL_TIM_ENCODERMODE_X4_TI12))  /* 源代码 */

#define IS_LL_TIM_IC_POLARITY_ENCODER(__VALUE__) (((__VALUE__) == LL_TIM_IC_POLARITY_RISING) \
                                                  || ((__VALUE__) == LL_TIM_IC_POLARITY_FALLING))  /* 源代码 */

#define IS_LL_TIM_OSSR_STATE(__VALUE__) (((__VALUE__) == LL_TIM_OSSR_DISABLE) \
                                         || ((__VALUE__) == LL_TIM_OSSR_ENABLE))  /* 源代码 */

#define IS_LL_TIM_OSSI_STATE(__VALUE__) (((__VALUE__) == LL_TIM_OSSI_DISABLE) \
                                         || ((__VALUE__) == LL_TIM_OSSI_ENABLE))  /* 源代码 */

#define IS_LL_TIM_LOCK_LEVEL(__VALUE__) (((__VALUE__) == LL_TIM_LOCKLEVEL_OFF) \
                                         || ((__VALUE__) == LL_TIM_LOCKLEVEL_1)   \
                                         || ((__VALUE__) == LL_TIM_LOCKLEVEL_2)   \
                                         || ((__VALUE__) == LL_TIM_LOCKLEVEL_3))  /* 源代码 */

#define IS_LL_TIM_BREAK_STATE(__VALUE__) (((__VALUE__) == LL_TIM_BREAK_DISABLE) \
                                          || ((__VALUE__) == LL_TIM_BREAK_ENABLE))  /* 源代码 */

#define IS_LL_TIM_BREAK_POLARITY(__VALUE__) (((__VALUE__) == LL_TIM_BREAK_POLARITY_LOW) \
                                             || ((__VALUE__) == LL_TIM_BREAK_POLARITY_HIGH))  /* 源代码 */

#define IS_LL_TIM_AUTOMATIC_OUTPUT_STATE(__VALUE__) (((__VALUE__) == LL_TIM_AUTOMATICOUTPUT_DISABLE) \
                                                     || ((__VALUE__) == LL_TIM_AUTOMATICOUTPUT_ENABLE))  /* 源代码 */
/**
  * @}
  */


/* Private function prototypes -----------------------------------------------*/
/** @defgroup TIM_LL_Private_Functions TIM Private Functions
  * @{
  */
static ErrorStatus OC1Config(TIM_TypeDef *TIMx, LL_TIM_OC_InitTypeDef *TIM_OCInitStruct);  /* 静态变量/函数 */
static ErrorStatus OC2Config(TIM_TypeDef *TIMx, LL_TIM_OC_InitTypeDef *TIM_OCInitStruct);  /* 静态变量/函数 */
static ErrorStatus OC3Config(TIM_TypeDef *TIMx, LL_TIM_OC_InitTypeDef *TIM_OCInitStruct);  /* 静态变量/函数 */
static ErrorStatus OC4Config(TIM_TypeDef *TIMx, LL_TIM_OC_InitTypeDef *TIM_OCInitStruct);  /* 静态变量/函数 */
static ErrorStatus IC1Config(TIM_TypeDef *TIMx, LL_TIM_IC_InitTypeDef *TIM_ICInitStruct);  /* 静态变量/函数 */
static ErrorStatus IC2Config(TIM_TypeDef *TIMx, LL_TIM_IC_InitTypeDef *TIM_ICInitStruct);  /* 静态变量/函数 */
static ErrorStatus IC3Config(TIM_TypeDef *TIMx, LL_TIM_IC_InitTypeDef *TIM_ICInitStruct);  /* 静态变量/函数 */
static ErrorStatus IC4Config(TIM_TypeDef *TIMx, LL_TIM_IC_InitTypeDef *TIM_ICInitStruct);  /* 静态变量/函数 */
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup TIM_LL_Exported_Functions
  * @{
  */

/** @addtogroup TIM_LL_EF_Init
  * @{
  */

/**
  * @brief  Set TIMx registers to their reset values.
  * @param  TIMx Timer instance
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: TIMx registers are de-initialized
  *          - ERROR: invalid TIMx instance
  */
ErrorStatus LL_TIM_DeInit(TIM_TypeDef *TIMx)            /* 源代码 */
{                                                       /* 代码块开始 */
  ErrorStatus result = SUCCESS;                         /* 赋值或初始化 */

  /* Check the parameters */
  assert_param(IS_TIM_INSTANCE(TIMx));                  /* 执行语句 */

  if (TIMx == TIM1)                                     /* 条件判断 */
  {                                                     /* 代码块开始 */
    LL_APB2_GRP1_ForceReset(LL_APB2_GRP1_PERIPH_TIM1);  /* 执行语句 */
    LL_APB2_GRP1_ReleaseReset(LL_APB2_GRP1_PERIPH_TIM1);  /* 执行语句 */
  }                                                     /* 代码块结束 */
#if defined(TIM2)                                       /* 条件编译 */
  else if (TIMx == TIM2)                                /* 否则如果 */
  {                                                     /* 代码块开始 */
    LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_TIM2);  /* 执行语句 */
    LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_TIM2);  /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif                                                  /* 条件编译结束 */
#if defined(TIM3)                                       /* 条件编译 */
  else if (TIMx == TIM3)                                /* 否则如果 */
  {                                                     /* 代码块开始 */
    LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_TIM3);  /* 执行语句 */
    LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_TIM3);  /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif                                                  /* 条件编译结束 */
#if defined(TIM4)                                       /* 条件编译 */
  else if (TIMx == TIM4)                                /* 否则如果 */
  {                                                     /* 代码块开始 */
    LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_TIM4);  /* 执行语句 */
    LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_TIM4);  /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif                                                  /* 条件编译结束 */
#if defined(TIM5)                                       /* 条件编译 */
  else if (TIMx == TIM5)                                /* 否则如果 */
  {                                                     /* 代码块开始 */
    LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_TIM5);  /* 执行语句 */
    LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_TIM5);  /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif                                                  /* 条件编译结束 */
#if defined(TIM6)                                       /* 条件编译 */
  else if (TIMx == TIM6)                                /* 否则如果 */
  {                                                     /* 代码块开始 */
    LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_TIM6);  /* 执行语句 */
    LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_TIM6);  /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif                                                  /* 条件编译结束 */
#if defined (TIM7)                                      /* 条件编译 */
  else if (TIMx == TIM7)                                /* 否则如果 */
  {                                                     /* 代码块开始 */
    LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_TIM7);  /* 执行语句 */
    LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_TIM7);  /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif                                                  /* 条件编译结束 */
#if defined(TIM8)                                       /* 条件编译 */
  else if (TIMx == TIM8)                                /* 否则如果 */
  {                                                     /* 代码块开始 */
    LL_APB2_GRP1_ForceReset(LL_APB2_GRP1_PERIPH_TIM8);  /* 执行语句 */
    LL_APB2_GRP1_ReleaseReset(LL_APB2_GRP1_PERIPH_TIM8);  /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif                                                  /* 条件编译结束 */
#if defined(TIM9)                                       /* 条件编译 */
  else if (TIMx == TIM9)                                /* 否则如果 */
  {                                                     /* 代码块开始 */
    LL_APB2_GRP1_ForceReset(LL_APB2_GRP1_PERIPH_TIM9);  /* 执行语句 */
    LL_APB2_GRP1_ReleaseReset(LL_APB2_GRP1_PERIPH_TIM9);  /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif                                                  /* 条件编译结束 */
#if defined(TIM10)                                      /* 条件编译 */
  else if (TIMx == TIM10)                               /* 否则如果 */
  {                                                     /* 代码块开始 */
    LL_APB2_GRP1_ForceReset(LL_APB2_GRP1_PERIPH_TIM10);  /* 执行语句 */
    LL_APB2_GRP1_ReleaseReset(LL_APB2_GRP1_PERIPH_TIM10);  /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif                                                  /* 条件编译结束 */
#if defined(TIM11)                                      /* 条件编译 */
  else if (TIMx == TIM11)                               /* 否则如果 */
  {                                                     /* 代码块开始 */
    LL_APB2_GRP1_ForceReset(LL_APB2_GRP1_PERIPH_TIM11);  /* 执行语句 */
    LL_APB2_GRP1_ReleaseReset(LL_APB2_GRP1_PERIPH_TIM11);  /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif                                                  /* 条件编译结束 */
#if defined(TIM12)                                      /* 条件编译 */
  else if (TIMx == TIM12)                               /* 否则如果 */
  {                                                     /* 代码块开始 */
    LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_TIM12);  /* 执行语句 */
    LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_TIM12);  /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif                                                  /* 条件编译结束 */
#if defined(TIM13)                                      /* 条件编译 */
  else if (TIMx == TIM13)                               /* 否则如果 */
  {                                                     /* 代码块开始 */
    LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_TIM13);  /* 执行语句 */
    LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_TIM13);  /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif                                                  /* 条件编译结束 */
#if defined(TIM14)                                      /* 条件编译 */
  else if (TIMx == TIM14)                               /* 否则如果 */
  {                                                     /* 代码块开始 */
    LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_TIM14);  /* 执行语句 */
    LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_TIM14);  /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif                                                  /* 条件编译结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    result = ERROR;                                     /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return result;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Set the fields of the time base unit configuration data structure
  *         to their default values.
  * @param  TIM_InitStruct pointer to a @ref LL_TIM_InitTypeDef structure (time base unit configuration data structure)
  * @retval None
  */
void LL_TIM_StructInit(LL_TIM_InitTypeDef *TIM_InitStruct)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Set the default configuration */
  TIM_InitStruct->Prescaler         = (uint16_t)0x0000;  /* 赋值或初始化 */
  TIM_InitStruct->CounterMode       = LL_TIM_COUNTERMODE_UP;  /* 赋值或初始化 */
  TIM_InitStruct->Autoreload        = 0xFFFFFFFFU;      /* 赋值或初始化 */
  TIM_InitStruct->ClockDivision     = LL_TIM_CLOCKDIVISION_DIV1;  /* 赋值或初始化 */
  TIM_InitStruct->RepetitionCounter = 0x00000000U;      /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configure the TIMx time base unit.
  * @param  TIMx Timer Instance
  * @param  TIM_InitStruct pointer to a @ref LL_TIM_InitTypeDef structure (TIMx time base unit configuration data structure)
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: TIMx registers are de-initialized
  *          - ERROR: not applicable
  */
ErrorStatus LL_TIM_Init(TIM_TypeDef *TIMx, LL_TIM_InitTypeDef *TIM_InitStruct)  /* 源代码 */
{                                                       /* 代码块开始 */
  uint32_t tmpcr1;                                      /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_TIM_INSTANCE(TIMx));                  /* 执行语句 */
  assert_param(IS_LL_TIM_COUNTERMODE(TIM_InitStruct->CounterMode));  /* 执行语句 */
  assert_param(IS_LL_TIM_CLOCKDIVISION(TIM_InitStruct->ClockDivision));  /* 执行语句 */

  tmpcr1 = LL_TIM_ReadReg(TIMx, CR1);                   /* 赋值或初始化 */

  if (IS_TIM_COUNTER_MODE_SELECT_INSTANCE(TIMx))        /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Select the Counter Mode */
    MODIFY_REG(tmpcr1, (TIM_CR1_DIR | TIM_CR1_CMS), TIM_InitStruct->CounterMode);  /* 执行语句 */
  }                                                     /* 代码块结束 */

  if (IS_TIM_CLOCK_DIVISION_INSTANCE(TIMx))             /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Set the clock division */
    MODIFY_REG(tmpcr1, TIM_CR1_CKD, TIM_InitStruct->ClockDivision);  /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Write to TIMx CR1 */
  LL_TIM_WriteReg(TIMx, CR1, tmpcr1);                   /* 执行语句 */

  /* Set the Autoreload value */
  LL_TIM_SetAutoReload(TIMx, TIM_InitStruct->Autoreload);  /* 执行语句 */

  /* Set the Prescaler value */
  LL_TIM_SetPrescaler(TIMx, TIM_InitStruct->Prescaler);  /* 执行语句 */

  if (IS_TIM_REPETITION_COUNTER_INSTANCE(TIMx))         /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Set the Repetition Counter value */
    LL_TIM_SetRepetitionCounter(TIMx, TIM_InitStruct->RepetitionCounter);  /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Generate an update event to reload the Prescaler
     and the repetition counter value (if applicable) immediately */
  LL_TIM_GenerateEvent_UPDATE(TIMx);                    /* 执行语句 */

  return SUCCESS;                                       /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Set the fields of the TIMx output channel configuration data
  *         structure to their default values.
  * @param  TIM_OC_InitStruct pointer to a @ref LL_TIM_OC_InitTypeDef structure (the output channel configuration data structure)
  * @retval None
  */
void LL_TIM_OC_StructInit(LL_TIM_OC_InitTypeDef *TIM_OC_InitStruct)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Set the default configuration */
  TIM_OC_InitStruct->OCMode       = LL_TIM_OCMODE_FROZEN;  /* 赋值或初始化 */
  TIM_OC_InitStruct->OCState      = LL_TIM_OCSTATE_DISABLE;  /* 赋值或初始化 */
  TIM_OC_InitStruct->OCNState     = LL_TIM_OCSTATE_DISABLE;  /* 赋值或初始化 */
  TIM_OC_InitStruct->CompareValue = 0x00000000U;        /* 赋值或初始化 */
  TIM_OC_InitStruct->OCPolarity   = LL_TIM_OCPOLARITY_HIGH;  /* 赋值或初始化 */
  TIM_OC_InitStruct->OCNPolarity  = LL_TIM_OCPOLARITY_HIGH;  /* 赋值或初始化 */
  TIM_OC_InitStruct->OCIdleState  = LL_TIM_OCIDLESTATE_LOW;  /* 赋值或初始化 */
  TIM_OC_InitStruct->OCNIdleState = LL_TIM_OCIDLESTATE_LOW;  /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configure the TIMx output channel.
  * @param  TIMx Timer Instance
  * @param  Channel This parameter can be one of the following values:
  *         @arg @ref LL_TIM_CHANNEL_CH1
  *         @arg @ref LL_TIM_CHANNEL_CH2
  *         @arg @ref LL_TIM_CHANNEL_CH3
  *         @arg @ref LL_TIM_CHANNEL_CH4
  * @param  TIM_OC_InitStruct pointer to a @ref LL_TIM_OC_InitTypeDef structure (TIMx output channel configuration data structure)
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: TIMx output channel is initialized
  *          - ERROR: TIMx output channel is not initialized
  */
ErrorStatus LL_TIM_OC_Init(TIM_TypeDef *TIMx, uint32_t Channel, LL_TIM_OC_InitTypeDef *TIM_OC_InitStruct)  /* 源代码 */
{                                                       /* 代码块开始 */
  ErrorStatus result = ERROR;                           /* 赋值或初始化 */

  switch (Channel)                                      /* switch分支 */
  {                                                     /* 代码块开始 */
    case LL_TIM_CHANNEL_CH1:                            /* case标签 */
      result = OC1Config(TIMx, TIM_OC_InitStruct);      /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case LL_TIM_CHANNEL_CH2:                            /* case标签 */
      result = OC2Config(TIMx, TIM_OC_InitStruct);      /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case LL_TIM_CHANNEL_CH3:                            /* case标签 */
      result = OC3Config(TIMx, TIM_OC_InitStruct);      /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case LL_TIM_CHANNEL_CH4:                            /* case标签 */
      result = OC4Config(TIMx, TIM_OC_InitStruct);      /* 赋值或初始化 */
      break;                                            /* 跳出 */
    default:                                            /* default分支 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  return result;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Set the fields of the TIMx input channel configuration data
  *         structure to their default values.
  * @param  TIM_ICInitStruct pointer to a @ref LL_TIM_IC_InitTypeDef structure (the input channel configuration data structure)
  * @retval None
  */
void LL_TIM_IC_StructInit(LL_TIM_IC_InitTypeDef *TIM_ICInitStruct)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Set the default configuration */
  TIM_ICInitStruct->ICPolarity    = LL_TIM_IC_POLARITY_RISING;  /* 赋值或初始化 */
  TIM_ICInitStruct->ICActiveInput = LL_TIM_ACTIVEINPUT_DIRECTTI;  /* 赋值或初始化 */
  TIM_ICInitStruct->ICPrescaler   = LL_TIM_ICPSC_DIV1;  /* 赋值或初始化 */
  TIM_ICInitStruct->ICFilter      = LL_TIM_IC_FILTER_FDIV1;  /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configure the TIMx input channel.
  * @param  TIMx Timer Instance
  * @param  Channel This parameter can be one of the following values:
  *         @arg @ref LL_TIM_CHANNEL_CH1
  *         @arg @ref LL_TIM_CHANNEL_CH2
  *         @arg @ref LL_TIM_CHANNEL_CH3
  *         @arg @ref LL_TIM_CHANNEL_CH4
  * @param  TIM_IC_InitStruct pointer to a @ref LL_TIM_IC_InitTypeDef structure (TIMx input channel configuration data structure)
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: TIMx output channel is initialized
  *          - ERROR: TIMx output channel is not initialized
  */
ErrorStatus LL_TIM_IC_Init(TIM_TypeDef *TIMx, uint32_t Channel, LL_TIM_IC_InitTypeDef *TIM_IC_InitStruct)  /* 源代码 */
{                                                       /* 代码块开始 */
  ErrorStatus result = ERROR;                           /* 赋值或初始化 */

  switch (Channel)                                      /* switch分支 */
  {                                                     /* 代码块开始 */
    case LL_TIM_CHANNEL_CH1:                            /* case标签 */
      result = IC1Config(TIMx, TIM_IC_InitStruct);      /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case LL_TIM_CHANNEL_CH2:                            /* case标签 */
      result = IC2Config(TIMx, TIM_IC_InitStruct);      /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case LL_TIM_CHANNEL_CH3:                            /* case标签 */
      result = IC3Config(TIMx, TIM_IC_InitStruct);      /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case LL_TIM_CHANNEL_CH4:                            /* case标签 */
      result = IC4Config(TIMx, TIM_IC_InitStruct);      /* 赋值或初始化 */
      break;                                            /* 跳出 */
    default:                                            /* default分支 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  return result;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Fills each TIM_EncoderInitStruct field with its default value
  * @param  TIM_EncoderInitStruct pointer to a @ref LL_TIM_ENCODER_InitTypeDef structure (encoder interface configuration data structure)
  * @retval None
  */
void LL_TIM_ENCODER_StructInit(LL_TIM_ENCODER_InitTypeDef *TIM_EncoderInitStruct)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Set the default configuration */
  TIM_EncoderInitStruct->EncoderMode    = LL_TIM_ENCODERMODE_X2_TI1;  /* 赋值或初始化 */
  TIM_EncoderInitStruct->IC1Polarity    = LL_TIM_IC_POLARITY_RISING;  /* 赋值或初始化 */
  TIM_EncoderInitStruct->IC1ActiveInput = LL_TIM_ACTIVEINPUT_DIRECTTI;  /* 赋值或初始化 */
  TIM_EncoderInitStruct->IC1Prescaler   = LL_TIM_ICPSC_DIV1;  /* 赋值或初始化 */
  TIM_EncoderInitStruct->IC1Filter      = LL_TIM_IC_FILTER_FDIV1;  /* 赋值或初始化 */
  TIM_EncoderInitStruct->IC2Polarity    = LL_TIM_IC_POLARITY_RISING;  /* 赋值或初始化 */
  TIM_EncoderInitStruct->IC2ActiveInput = LL_TIM_ACTIVEINPUT_DIRECTTI;  /* 赋值或初始化 */
  TIM_EncoderInitStruct->IC2Prescaler   = LL_TIM_ICPSC_DIV1;  /* 赋值或初始化 */
  TIM_EncoderInitStruct->IC2Filter      = LL_TIM_IC_FILTER_FDIV1;  /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configure the encoder interface of the timer instance.
  * @param  TIMx Timer Instance
  * @param  TIM_EncoderInitStruct pointer to a @ref LL_TIM_ENCODER_InitTypeDef structure (TIMx encoder interface configuration data structure)
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: TIMx registers are de-initialized
  *          - ERROR: not applicable
  */
ErrorStatus LL_TIM_ENCODER_Init(TIM_TypeDef *TIMx, LL_TIM_ENCODER_InitTypeDef *TIM_EncoderInitStruct)  /* 源代码 */
{                                                       /* 代码块开始 */
  uint32_t tmpccmr1;                                    /* 32位无符号变量/函数 */
  uint32_t tmpccer;                                     /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_TIM_ENCODER_INTERFACE_INSTANCE(TIMx));  /* 执行语句 */
  assert_param(IS_LL_TIM_ENCODERMODE(TIM_EncoderInitStruct->EncoderMode));  /* 执行语句 */
  assert_param(IS_LL_TIM_IC_POLARITY_ENCODER(TIM_EncoderInitStruct->IC1Polarity));  /* 执行语句 */
  assert_param(IS_LL_TIM_ACTIVEINPUT(TIM_EncoderInitStruct->IC1ActiveInput));  /* 执行语句 */
  assert_param(IS_LL_TIM_ICPSC(TIM_EncoderInitStruct->IC1Prescaler));  /* 执行语句 */
  assert_param(IS_LL_TIM_IC_FILTER(TIM_EncoderInitStruct->IC1Filter));  /* 执行语句 */
  assert_param(IS_LL_TIM_IC_POLARITY_ENCODER(TIM_EncoderInitStruct->IC2Polarity));  /* 执行语句 */
  assert_param(IS_LL_TIM_ACTIVEINPUT(TIM_EncoderInitStruct->IC2ActiveInput));  /* 执行语句 */
  assert_param(IS_LL_TIM_ICPSC(TIM_EncoderInitStruct->IC2Prescaler));  /* 执行语句 */
  assert_param(IS_LL_TIM_IC_FILTER(TIM_EncoderInitStruct->IC2Filter));  /* 执行语句 */

  /* Disable the CC1 and CC2: Reset the CC1E and CC2E Bits */
  TIMx->CCER &= (uint32_t)~(TIM_CCER_CC1E | TIM_CCER_CC2E);  /* 赋值或初始化 */

  /* Get the TIMx CCMR1 register value */
  tmpccmr1 = LL_TIM_ReadReg(TIMx, CCMR1);               /* 赋值或初始化 */

  /* Get the TIMx CCER register value */
  tmpccer = LL_TIM_ReadReg(TIMx, CCER);                 /* 赋值或初始化 */

  /* Configure TI1 */
  tmpccmr1 &= (uint32_t)~(TIM_CCMR1_CC1S | TIM_CCMR1_IC1F  | TIM_CCMR1_IC1PSC);  /* 赋值或初始化 */
  tmpccmr1 |= (uint32_t)(TIM_EncoderInitStruct->IC1ActiveInput >> 16U);  /* 赋值或初始化 */
  tmpccmr1 |= (uint32_t)(TIM_EncoderInitStruct->IC1Filter >> 16U);  /* 赋值或初始化 */
  tmpccmr1 |= (uint32_t)(TIM_EncoderInitStruct->IC1Prescaler >> 16U);  /* 赋值或初始化 */

  /* Configure TI2 */
  tmpccmr1 &= (uint32_t)~(TIM_CCMR1_CC2S | TIM_CCMR1_IC2F  | TIM_CCMR1_IC2PSC);  /* 赋值或初始化 */
  tmpccmr1 |= (uint32_t)(TIM_EncoderInitStruct->IC2ActiveInput >> 8U);  /* 赋值或初始化 */
  tmpccmr1 |= (uint32_t)(TIM_EncoderInitStruct->IC2Filter >> 8U);  /* 赋值或初始化 */
  tmpccmr1 |= (uint32_t)(TIM_EncoderInitStruct->IC2Prescaler >> 8U);  /* 赋值或初始化 */

  /* Set TI1 and TI2 polarity and enable TI1 and TI2 */
  tmpccer &= (uint32_t)~(TIM_CCER_CC1P | TIM_CCER_CC1NP | TIM_CCER_CC2P | TIM_CCER_CC2NP);  /* 赋值或初始化 */
  tmpccer |= (uint32_t)(TIM_EncoderInitStruct->IC1Polarity);  /* 赋值或初始化 */
  tmpccer |= (uint32_t)(TIM_EncoderInitStruct->IC2Polarity << 4U);  /* 赋值或初始化 */
  tmpccer |= (uint32_t)(TIM_CCER_CC1E | TIM_CCER_CC2E);  /* 赋值或初始化 */

  /* Set encoder mode */
  LL_TIM_SetEncoderMode(TIMx, TIM_EncoderInitStruct->EncoderMode);  /* 执行语句 */

  /* Write to TIMx CCMR1 */
  LL_TIM_WriteReg(TIMx, CCMR1, tmpccmr1);               /* 执行语句 */

  /* Write to TIMx CCER */
  LL_TIM_WriteReg(TIMx, CCER, tmpccer);                 /* 执行语句 */

  return SUCCESS;                                       /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Set the fields of the TIMx Hall sensor interface configuration data
  *         structure to their default values.
  * @param  TIM_HallSensorInitStruct pointer to a @ref LL_TIM_HALLSENSOR_InitTypeDef structure (HALL sensor interface configuration data structure)
  * @retval None
  */
void LL_TIM_HALLSENSOR_StructInit(LL_TIM_HALLSENSOR_InitTypeDef *TIM_HallSensorInitStruct)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Set the default configuration */
  TIM_HallSensorInitStruct->IC1Polarity       = LL_TIM_IC_POLARITY_RISING;  /* 赋值或初始化 */
  TIM_HallSensorInitStruct->IC1Prescaler      = LL_TIM_ICPSC_DIV1;  /* 赋值或初始化 */
  TIM_HallSensorInitStruct->IC1Filter         = LL_TIM_IC_FILTER_FDIV1;  /* 赋值或初始化 */
  TIM_HallSensorInitStruct->CommutationDelay  = 0U;     /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configure the Hall sensor interface of the timer instance.
  * @note TIMx CH1, CH2 and CH3 inputs connected through a XOR
  *       to the TI1 input channel
  * @note TIMx slave mode controller is configured in reset mode.
          Selected internal trigger is TI1F_ED.
  * @note Channel 1 is configured as input, IC1 is mapped on TRC.
  * @note Captured value stored in TIMx_CCR1 correspond to the time elapsed
  *       between 2 changes on the inputs. It gives information about motor speed.
  * @note Channel 2 is configured in output PWM 2 mode.
  * @note Compare value stored in TIMx_CCR2 corresponds to the commutation delay.
  * @note OC2REF is selected as trigger output on TRGO.
  * @note LL_TIM_IC_POLARITY_BOTHEDGE must not be used for TI1 when it is used
  *       when TIMx operates in Hall sensor interface mode.
  * @param  TIMx Timer Instance
  * @param  TIM_HallSensorInitStruct pointer to a @ref LL_TIM_HALLSENSOR_InitTypeDef structure (TIMx HALL sensor interface configuration data structure)
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: TIMx registers are de-initialized
  *          - ERROR: not applicable
  */
ErrorStatus LL_TIM_HALLSENSOR_Init(TIM_TypeDef *TIMx, LL_TIM_HALLSENSOR_InitTypeDef *TIM_HallSensorInitStruct)  /* 源代码 */
{                                                       /* 代码块开始 */
  uint32_t tmpcr2;                                      /* 32位无符号变量/函数 */
  uint32_t tmpccmr1;                                    /* 32位无符号变量/函数 */
  uint32_t tmpccer;                                     /* 32位无符号变量/函数 */
  uint32_t tmpsmcr;                                     /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_TIM_HALL_SENSOR_INTERFACE_INSTANCE(TIMx));  /* 执行语句 */
  assert_param(IS_LL_TIM_IC_POLARITY_ENCODER(TIM_HallSensorInitStruct->IC1Polarity));  /* 执行语句 */
  assert_param(IS_LL_TIM_ICPSC(TIM_HallSensorInitStruct->IC1Prescaler));  /* 执行语句 */
  assert_param(IS_LL_TIM_IC_FILTER(TIM_HallSensorInitStruct->IC1Filter));  /* 执行语句 */

  /* Disable the CC1 and CC2: Reset the CC1E and CC2E Bits */
  TIMx->CCER &= (uint32_t)~(TIM_CCER_CC1E | TIM_CCER_CC2E);  /* 赋值或初始化 */

  /* Get the TIMx CR2 register value */
  tmpcr2 = LL_TIM_ReadReg(TIMx, CR2);                   /* 赋值或初始化 */

  /* Get the TIMx CCMR1 register value */
  tmpccmr1 = LL_TIM_ReadReg(TIMx, CCMR1);               /* 赋值或初始化 */

  /* Get the TIMx CCER register value */
  tmpccer = LL_TIM_ReadReg(TIMx, CCER);                 /* 赋值或初始化 */

  /* Get the TIMx SMCR register value */
  tmpsmcr = LL_TIM_ReadReg(TIMx, SMCR);                 /* 赋值或初始化 */

  /* Connect TIMx_CH1, CH2 and CH3 pins to the TI1 input */
  tmpcr2 |= TIM_CR2_TI1S;                               /* 赋值或初始化 */

  /* OC2REF signal is used as trigger output (TRGO) */
  tmpcr2 |= LL_TIM_TRGO_OC2REF;                         /* 赋值或初始化 */

  /* Configure the slave mode controller */
  tmpsmcr &= (uint32_t)~(TIM_SMCR_TS | TIM_SMCR_SMS);   /* 赋值或初始化 */
  tmpsmcr |= LL_TIM_TS_TI1F_ED;                         /* 赋值或初始化 */
  tmpsmcr |= LL_TIM_SLAVEMODE_RESET;                    /* 赋值或初始化 */

  /* Configure input channel 1 */
  tmpccmr1 &= (uint32_t)~(TIM_CCMR1_CC1S | TIM_CCMR1_IC1F  | TIM_CCMR1_IC1PSC);  /* 赋值或初始化 */
  tmpccmr1 |= (uint32_t)(LL_TIM_ACTIVEINPUT_TRC >> 16U);  /* 赋值或初始化 */
  tmpccmr1 |= (uint32_t)(TIM_HallSensorInitStruct->IC1Filter >> 16U);  /* 赋值或初始化 */
  tmpccmr1 |= (uint32_t)(TIM_HallSensorInitStruct->IC1Prescaler >> 16U);  /* 赋值或初始化 */

  /* Configure input channel 2 */
  tmpccmr1 &= (uint32_t)~(TIM_CCMR1_OC2M | TIM_CCMR1_OC2FE  | TIM_CCMR1_OC2PE  | TIM_CCMR1_OC2CE);  /* 赋值或初始化 */
  tmpccmr1 |= (uint32_t)(LL_TIM_OCMODE_PWM2 << 8U);     /* 赋值或初始化 */

  /* Set Channel 1 polarity and enable Channel 1 and Channel2 */
  tmpccer &= (uint32_t)~(TIM_CCER_CC1P | TIM_CCER_CC1NP | TIM_CCER_CC2P | TIM_CCER_CC2NP);  /* 赋值或初始化 */
  tmpccer |= (uint32_t)(TIM_HallSensorInitStruct->IC1Polarity);  /* 赋值或初始化 */
  tmpccer |= (uint32_t)(TIM_CCER_CC1E | TIM_CCER_CC2E);  /* 赋值或初始化 */

  /* Write to TIMx CR2 */
  LL_TIM_WriteReg(TIMx, CR2, tmpcr2);                   /* 执行语句 */

  /* Write to TIMx SMCR */
  LL_TIM_WriteReg(TIMx, SMCR, tmpsmcr);                 /* 执行语句 */

  /* Write to TIMx CCMR1 */
  LL_TIM_WriteReg(TIMx, CCMR1, tmpccmr1);               /* 执行语句 */

  /* Write to TIMx CCER */
  LL_TIM_WriteReg(TIMx, CCER, tmpccer);                 /* 执行语句 */

  /* Write to TIMx CCR2 */
  LL_TIM_OC_SetCompareCH2(TIMx, TIM_HallSensorInitStruct->CommutationDelay);  /* 执行语句 */

  return SUCCESS;                                       /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Set the fields of the Break and Dead Time configuration data structure
  *         to their default values.
  * @param  TIM_BDTRInitStruct pointer to a @ref LL_TIM_BDTR_InitTypeDef structure (Break and Dead Time configuration data structure)
  * @retval None
  */
void LL_TIM_BDTR_StructInit(LL_TIM_BDTR_InitTypeDef *TIM_BDTRInitStruct)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Set the default configuration */
  TIM_BDTRInitStruct->OSSRState       = LL_TIM_OSSR_DISABLE;  /* 赋值或初始化 */
  TIM_BDTRInitStruct->OSSIState       = LL_TIM_OSSI_DISABLE;  /* 赋值或初始化 */
  TIM_BDTRInitStruct->LockLevel       = LL_TIM_LOCKLEVEL_OFF;  /* 赋值或初始化 */
  TIM_BDTRInitStruct->DeadTime        = (uint8_t)0x00;  /* 赋值或初始化 */
  TIM_BDTRInitStruct->BreakState      = LL_TIM_BREAK_DISABLE;  /* 赋值或初始化 */
  TIM_BDTRInitStruct->BreakPolarity   = LL_TIM_BREAK_POLARITY_LOW;  /* 赋值或初始化 */
  TIM_BDTRInitStruct->AutomaticOutput = LL_TIM_AUTOMATICOUTPUT_DISABLE;  /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configure the Break and Dead Time feature of the timer instance.
  * @note As the bits AOE, BKP, BKE, OSSR, OSSI and DTG[7:0] can be write-locked
  *  depending on the LOCK configuration, it can be necessary to configure all of
  *  them during the first write access to the TIMx_BDTR register.
  * @note Macro IS_TIM_BREAK_INSTANCE(TIMx) can be used to check whether or not
  *       a timer instance provides a break input.
  * @param  TIMx Timer Instance
  * @param  TIM_BDTRInitStruct pointer to a @ref LL_TIM_BDTR_InitTypeDef structure (Break and Dead Time configuration data structure)
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: Break and Dead Time is initialized
  *          - ERROR: not applicable
  */
ErrorStatus LL_TIM_BDTR_Init(TIM_TypeDef *TIMx, LL_TIM_BDTR_InitTypeDef *TIM_BDTRInitStruct)  /* 源代码 */
{                                                       /* 代码块开始 */
  uint32_t tmpbdtr = 0;                                 /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_TIM_BREAK_INSTANCE(TIMx));            /* 执行语句 */
  assert_param(IS_LL_TIM_OSSR_STATE(TIM_BDTRInitStruct->OSSRState));  /* 执行语句 */
  assert_param(IS_LL_TIM_OSSI_STATE(TIM_BDTRInitStruct->OSSIState));  /* 执行语句 */
  assert_param(IS_LL_TIM_LOCK_LEVEL(TIM_BDTRInitStruct->LockLevel));  /* 执行语句 */
  assert_param(IS_LL_TIM_BREAK_STATE(TIM_BDTRInitStruct->BreakState));  /* 执行语句 */
  assert_param(IS_LL_TIM_BREAK_POLARITY(TIM_BDTRInitStruct->BreakPolarity));  /* 执行语句 */
  assert_param(IS_LL_TIM_AUTOMATIC_OUTPUT_STATE(TIM_BDTRInitStruct->AutomaticOutput));  /* 执行语句 */

  /* Set the Lock level, the Break enable Bit and the Polarity, the OSSR State,
  the OSSI State, the dead time value and the Automatic Output Enable Bit */

  /* Set the BDTR bits */
  MODIFY_REG(tmpbdtr, TIM_BDTR_DTG, TIM_BDTRInitStruct->DeadTime);  /* 执行语句 */
  MODIFY_REG(tmpbdtr, TIM_BDTR_LOCK, TIM_BDTRInitStruct->LockLevel);  /* 执行语句 */
  MODIFY_REG(tmpbdtr, TIM_BDTR_OSSI, TIM_BDTRInitStruct->OSSIState);  /* 执行语句 */
  MODIFY_REG(tmpbdtr, TIM_BDTR_OSSR, TIM_BDTRInitStruct->OSSRState);  /* 执行语句 */
  MODIFY_REG(tmpbdtr, TIM_BDTR_BKE, TIM_BDTRInitStruct->BreakState);  /* 执行语句 */
  MODIFY_REG(tmpbdtr, TIM_BDTR_BKP, TIM_BDTRInitStruct->BreakPolarity);  /* 执行语句 */
  MODIFY_REG(tmpbdtr, TIM_BDTR_AOE, TIM_BDTRInitStruct->AutomaticOutput);  /* 执行语句 */
  MODIFY_REG(tmpbdtr, TIM_BDTR_MOE, TIM_BDTRInitStruct->AutomaticOutput);  /* 执行语句 */

  /* Set TIMx_BDTR */
  LL_TIM_WriteReg(TIMx, BDTR, tmpbdtr);                 /* 执行语句 */

  return SUCCESS;                                       /* 返回值 */
}                                                       /* 代码块结束 */
/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup TIM_LL_Private_Functions TIM Private Functions
  *  @brief   Private functions
  * @{
  */
/**
  * @brief  Configure the TIMx output channel 1.
  * @param  TIMx Timer Instance
  * @param  TIM_OCInitStruct pointer to the the TIMx output channel 1 configuration data structure
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: TIMx registers are de-initialized
  *          - ERROR: not applicable
  */
static ErrorStatus OC1Config(TIM_TypeDef *TIMx, LL_TIM_OC_InitTypeDef *TIM_OCInitStruct)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpccmr1;                                    /* 32位无符号变量/函数 */
  uint32_t tmpccer;                                     /* 32位无符号变量/函数 */
  uint32_t tmpcr2;                                      /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_TIM_CC1_INSTANCE(TIMx));              /* 执行语句 */
  assert_param(IS_LL_TIM_OCMODE(TIM_OCInitStruct->OCMode));  /* 执行语句 */
  assert_param(IS_LL_TIM_OCSTATE(TIM_OCInitStruct->OCState));  /* 执行语句 */
  assert_param(IS_LL_TIM_OCPOLARITY(TIM_OCInitStruct->OCPolarity));  /* 执行语句 */
  assert_param(IS_LL_TIM_OCSTATE(TIM_OCInitStruct->OCNState));  /* 执行语句 */
  assert_param(IS_LL_TIM_OCPOLARITY(TIM_OCInitStruct->OCNPolarity));  /* 执行语句 */

  /* Disable the Channel 1: Reset the CC1E Bit */
  CLEAR_BIT(TIMx->CCER, TIM_CCER_CC1E);                 /* 执行语句 */

  /* Get the TIMx CCER register value */
  tmpccer = LL_TIM_ReadReg(TIMx, CCER);                 /* 赋值或初始化 */

  /* Get the TIMx CR2 register value */
  tmpcr2 = LL_TIM_ReadReg(TIMx, CR2);                   /* 赋值或初始化 */

  /* Get the TIMx CCMR1 register value */
  tmpccmr1 = LL_TIM_ReadReg(TIMx, CCMR1);               /* 赋值或初始化 */

  /* Reset Capture/Compare selection Bits */
  CLEAR_BIT(tmpccmr1, TIM_CCMR1_CC1S);                  /* 执行语句 */

  /* Set the Output Compare Mode */
  MODIFY_REG(tmpccmr1, TIM_CCMR1_OC1M, TIM_OCInitStruct->OCMode);  /* 执行语句 */

  /* Set the Output Compare Polarity */
  MODIFY_REG(tmpccer, TIM_CCER_CC1P, TIM_OCInitStruct->OCPolarity);  /* 执行语句 */

  /* Set the Output State */
  MODIFY_REG(tmpccer, TIM_CCER_CC1E, TIM_OCInitStruct->OCState);  /* 执行语句 */

  if (IS_TIM_BREAK_INSTANCE(TIMx))                      /* 条件判断 */
  {                                                     /* 代码块开始 */
    assert_param(IS_LL_TIM_OCIDLESTATE(TIM_OCInitStruct->OCNIdleState));  /* 执行语句 */
    assert_param(IS_LL_TIM_OCIDLESTATE(TIM_OCInitStruct->OCIdleState));  /* 执行语句 */

    /* Set the complementary output Polarity */
    MODIFY_REG(tmpccer, TIM_CCER_CC1NP, TIM_OCInitStruct->OCNPolarity << 2U);  /* 执行语句 */

    /* Set the complementary output State */
    MODIFY_REG(tmpccer, TIM_CCER_CC1NE, TIM_OCInitStruct->OCNState << 2U);  /* 执行语句 */

    /* Set the Output Idle state */
    MODIFY_REG(tmpcr2, TIM_CR2_OIS1, TIM_OCInitStruct->OCIdleState);  /* 执行语句 */

    /* Set the complementary output Idle state */
    MODIFY_REG(tmpcr2, TIM_CR2_OIS1N, TIM_OCInitStruct->OCNIdleState << 1U);  /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Write to TIMx CR2 */
  LL_TIM_WriteReg(TIMx, CR2, tmpcr2);                   /* 执行语句 */

  /* Write to TIMx CCMR1 */
  LL_TIM_WriteReg(TIMx, CCMR1, tmpccmr1);               /* 执行语句 */

  /* Set the Capture Compare Register value */
  LL_TIM_OC_SetCompareCH1(TIMx, TIM_OCInitStruct->CompareValue);  /* 执行语句 */

  /* Write to TIMx CCER */
  LL_TIM_WriteReg(TIMx, CCER, tmpccer);                 /* 执行语句 */

  return SUCCESS;                                       /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configure the TIMx output channel 2.
  * @param  TIMx Timer Instance
  * @param  TIM_OCInitStruct pointer to the the TIMx output channel 2 configuration data structure
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: TIMx registers are de-initialized
  *          - ERROR: not applicable
  */
static ErrorStatus OC2Config(TIM_TypeDef *TIMx, LL_TIM_OC_InitTypeDef *TIM_OCInitStruct)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpccmr1;                                    /* 32位无符号变量/函数 */
  uint32_t tmpccer;                                     /* 32位无符号变量/函数 */
  uint32_t tmpcr2;                                      /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_TIM_CC2_INSTANCE(TIMx));              /* 执行语句 */
  assert_param(IS_LL_TIM_OCMODE(TIM_OCInitStruct->OCMode));  /* 执行语句 */
  assert_param(IS_LL_TIM_OCSTATE(TIM_OCInitStruct->OCState));  /* 执行语句 */
  assert_param(IS_LL_TIM_OCPOLARITY(TIM_OCInitStruct->OCPolarity));  /* 执行语句 */
  assert_param(IS_LL_TIM_OCSTATE(TIM_OCInitStruct->OCNState));  /* 执行语句 */
  assert_param(IS_LL_TIM_OCPOLARITY(TIM_OCInitStruct->OCNPolarity));  /* 执行语句 */

  /* Disable the Channel 2: Reset the CC2E Bit */
  CLEAR_BIT(TIMx->CCER, TIM_CCER_CC2E);                 /* 执行语句 */

  /* Get the TIMx CCER register value */
  tmpccer =  LL_TIM_ReadReg(TIMx, CCER);                /* 赋值或初始化 */

  /* Get the TIMx CR2 register value */
  tmpcr2 = LL_TIM_ReadReg(TIMx, CR2);                   /* 赋值或初始化 */

  /* Get the TIMx CCMR1 register value */
  tmpccmr1 = LL_TIM_ReadReg(TIMx, CCMR1);               /* 赋值或初始化 */

  /* Reset Capture/Compare selection Bits */
  CLEAR_BIT(tmpccmr1, TIM_CCMR1_CC2S);                  /* 执行语句 */

  /* Select the Output Compare Mode */
  MODIFY_REG(tmpccmr1, TIM_CCMR1_OC2M, TIM_OCInitStruct->OCMode << 8U);  /* 执行语句 */

  /* Set the Output Compare Polarity */
  MODIFY_REG(tmpccer, TIM_CCER_CC2P, TIM_OCInitStruct->OCPolarity << 4U);  /* 执行语句 */

  /* Set the Output State */
  MODIFY_REG(tmpccer, TIM_CCER_CC2E, TIM_OCInitStruct->OCState << 4U);  /* 执行语句 */

  if (IS_TIM_BREAK_INSTANCE(TIMx))                      /* 条件判断 */
  {                                                     /* 代码块开始 */
    assert_param(IS_LL_TIM_OCIDLESTATE(TIM_OCInitStruct->OCNIdleState));  /* 执行语句 */
    assert_param(IS_LL_TIM_OCIDLESTATE(TIM_OCInitStruct->OCIdleState));  /* 执行语句 */

    /* Set the complementary output Polarity */
    MODIFY_REG(tmpccer, TIM_CCER_CC2NP, TIM_OCInitStruct->OCNPolarity << 6U);  /* 执行语句 */

    /* Set the complementary output State */
    MODIFY_REG(tmpccer, TIM_CCER_CC2NE, TIM_OCInitStruct->OCNState << 6U);  /* 执行语句 */

    /* Set the Output Idle state */
    MODIFY_REG(tmpcr2, TIM_CR2_OIS2, TIM_OCInitStruct->OCIdleState << 2U);  /* 执行语句 */

    /* Set the complementary output Idle state */
    MODIFY_REG(tmpcr2, TIM_CR2_OIS2N, TIM_OCInitStruct->OCNIdleState << 3U);  /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Write to TIMx CR2 */
  LL_TIM_WriteReg(TIMx, CR2, tmpcr2);                   /* 执行语句 */

  /* Write to TIMx CCMR1 */
  LL_TIM_WriteReg(TIMx, CCMR1, tmpccmr1);               /* 执行语句 */

  /* Set the Capture Compare Register value */
  LL_TIM_OC_SetCompareCH2(TIMx, TIM_OCInitStruct->CompareValue);  /* 执行语句 */

  /* Write to TIMx CCER */
  LL_TIM_WriteReg(TIMx, CCER, tmpccer);                 /* 执行语句 */

  return SUCCESS;                                       /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configure the TIMx output channel 3.
  * @param  TIMx Timer Instance
  * @param  TIM_OCInitStruct pointer to the the TIMx output channel 3 configuration data structure
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: TIMx registers are de-initialized
  *          - ERROR: not applicable
  */
static ErrorStatus OC3Config(TIM_TypeDef *TIMx, LL_TIM_OC_InitTypeDef *TIM_OCInitStruct)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpccmr2;                                    /* 32位无符号变量/函数 */
  uint32_t tmpccer;                                     /* 32位无符号变量/函数 */
  uint32_t tmpcr2;                                      /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_TIM_CC3_INSTANCE(TIMx));              /* 执行语句 */
  assert_param(IS_LL_TIM_OCMODE(TIM_OCInitStruct->OCMode));  /* 执行语句 */
  assert_param(IS_LL_TIM_OCSTATE(TIM_OCInitStruct->OCState));  /* 执行语句 */
  assert_param(IS_LL_TIM_OCPOLARITY(TIM_OCInitStruct->OCPolarity));  /* 执行语句 */
  assert_param(IS_LL_TIM_OCSTATE(TIM_OCInitStruct->OCNState));  /* 执行语句 */
  assert_param(IS_LL_TIM_OCPOLARITY(TIM_OCInitStruct->OCNPolarity));  /* 执行语句 */

  /* Disable the Channel 3: Reset the CC3E Bit */
  CLEAR_BIT(TIMx->CCER, TIM_CCER_CC3E);                 /* 执行语句 */

  /* Get the TIMx CCER register value */
  tmpccer =  LL_TIM_ReadReg(TIMx, CCER);                /* 赋值或初始化 */

  /* Get the TIMx CR2 register value */
  tmpcr2 = LL_TIM_ReadReg(TIMx, CR2);                   /* 赋值或初始化 */

  /* Get the TIMx CCMR2 register value */
  tmpccmr2 = LL_TIM_ReadReg(TIMx, CCMR2);               /* 赋值或初始化 */

  /* Reset Capture/Compare selection Bits */
  CLEAR_BIT(tmpccmr2, TIM_CCMR2_CC3S);                  /* 执行语句 */

  /* Select the Output Compare Mode */
  MODIFY_REG(tmpccmr2, TIM_CCMR2_OC3M, TIM_OCInitStruct->OCMode);  /* 执行语句 */

  /* Set the Output Compare Polarity */
  MODIFY_REG(tmpccer, TIM_CCER_CC3P, TIM_OCInitStruct->OCPolarity << 8U);  /* 执行语句 */

  /* Set the Output State */
  MODIFY_REG(tmpccer, TIM_CCER_CC3E, TIM_OCInitStruct->OCState << 8U);  /* 执行语句 */

  if (IS_TIM_BREAK_INSTANCE(TIMx))                      /* 条件判断 */
  {                                                     /* 代码块开始 */
    assert_param(IS_LL_TIM_OCIDLESTATE(TIM_OCInitStruct->OCNIdleState));  /* 执行语句 */
    assert_param(IS_LL_TIM_OCIDLESTATE(TIM_OCInitStruct->OCIdleState));  /* 执行语句 */

    /* Set the complementary output Polarity */
    MODIFY_REG(tmpccer, TIM_CCER_CC3NP, TIM_OCInitStruct->OCNPolarity << 10U);  /* 执行语句 */

    /* Set the complementary output State */
    MODIFY_REG(tmpccer, TIM_CCER_CC3NE, TIM_OCInitStruct->OCNState << 10U);  /* 执行语句 */

    /* Set the Output Idle state */
    MODIFY_REG(tmpcr2, TIM_CR2_OIS3, TIM_OCInitStruct->OCIdleState << 4U);  /* 执行语句 */

    /* Set the complementary output Idle state */
    MODIFY_REG(tmpcr2, TIM_CR2_OIS3N, TIM_OCInitStruct->OCNIdleState << 5U);  /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Write to TIMx CR2 */
  LL_TIM_WriteReg(TIMx, CR2, tmpcr2);                   /* 执行语句 */

  /* Write to TIMx CCMR2 */
  LL_TIM_WriteReg(TIMx, CCMR2, tmpccmr2);               /* 执行语句 */

  /* Set the Capture Compare Register value */
  LL_TIM_OC_SetCompareCH3(TIMx, TIM_OCInitStruct->CompareValue);  /* 执行语句 */

  /* Write to TIMx CCER */
  LL_TIM_WriteReg(TIMx, CCER, tmpccer);                 /* 执行语句 */

  return SUCCESS;                                       /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configure the TIMx output channel 4.
  * @param  TIMx Timer Instance
  * @param  TIM_OCInitStruct pointer to the the TIMx output channel 4 configuration data structure
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: TIMx registers are de-initialized
  *          - ERROR: not applicable
  */
static ErrorStatus OC4Config(TIM_TypeDef *TIMx, LL_TIM_OC_InitTypeDef *TIM_OCInitStruct)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpccmr2;                                    /* 32位无符号变量/函数 */
  uint32_t tmpccer;                                     /* 32位无符号变量/函数 */
  uint32_t tmpcr2;                                      /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_TIM_CC4_INSTANCE(TIMx));              /* 执行语句 */
  assert_param(IS_LL_TIM_OCMODE(TIM_OCInitStruct->OCMode));  /* 执行语句 */
  assert_param(IS_LL_TIM_OCSTATE(TIM_OCInitStruct->OCState));  /* 执行语句 */
  assert_param(IS_LL_TIM_OCPOLARITY(TIM_OCInitStruct->OCPolarity));  /* 执行语句 */
  assert_param(IS_LL_TIM_OCPOLARITY(TIM_OCInitStruct->OCNPolarity));  /* 执行语句 */
  assert_param(IS_LL_TIM_OCSTATE(TIM_OCInitStruct->OCNState));  /* 执行语句 */

  /* Disable the Channel 4: Reset the CC4E Bit */
  CLEAR_BIT(TIMx->CCER, TIM_CCER_CC4E);                 /* 执行语句 */

  /* Get the TIMx CCER register value */
  tmpccer = LL_TIM_ReadReg(TIMx, CCER);                 /* 赋值或初始化 */

  /* Get the TIMx CR2 register value */
  tmpcr2 =  LL_TIM_ReadReg(TIMx, CR2);                  /* 赋值或初始化 */

  /* Get the TIMx CCMR2 register value */
  tmpccmr2 = LL_TIM_ReadReg(TIMx, CCMR2);               /* 赋值或初始化 */

  /* Reset Capture/Compare selection Bits */
  CLEAR_BIT(tmpccmr2, TIM_CCMR2_CC4S);                  /* 执行语句 */

  /* Select the Output Compare Mode */
  MODIFY_REG(tmpccmr2, TIM_CCMR2_OC4M, TIM_OCInitStruct->OCMode << 8U);  /* 执行语句 */

  /* Set the Output Compare Polarity */
  MODIFY_REG(tmpccer, TIM_CCER_CC4P, TIM_OCInitStruct->OCPolarity << 12U);  /* 执行语句 */

  /* Set the Output State */
  MODIFY_REG(tmpccer, TIM_CCER_CC4E, TIM_OCInitStruct->OCState << 12U);  /* 执行语句 */

  if (IS_TIM_BREAK_INSTANCE(TIMx))                      /* 条件判断 */
  {                                                     /* 代码块开始 */
    assert_param(IS_LL_TIM_OCIDLESTATE(TIM_OCInitStruct->OCNIdleState));  /* 执行语句 */
    assert_param(IS_LL_TIM_OCIDLESTATE(TIM_OCInitStruct->OCIdleState));  /* 执行语句 */

    /* Set the Output Idle state */
    MODIFY_REG(tmpcr2, TIM_CR2_OIS4, TIM_OCInitStruct->OCIdleState << 6U);  /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Write to TIMx CR2 */
  LL_TIM_WriteReg(TIMx, CR2, tmpcr2);                   /* 执行语句 */

  /* Write to TIMx CCMR2 */
  LL_TIM_WriteReg(TIMx, CCMR2, tmpccmr2);               /* 执行语句 */

  /* Set the Capture Compare Register value */
  LL_TIM_OC_SetCompareCH4(TIMx, TIM_OCInitStruct->CompareValue);  /* 执行语句 */

  /* Write to TIMx CCER */
  LL_TIM_WriteReg(TIMx, CCER, tmpccer);                 /* 执行语句 */

  return SUCCESS;                                       /* 返回值 */
}                                                       /* 代码块结束 */


/**
  * @brief  Configure the TIMx input channel 1.
  * @param  TIMx Timer Instance
  * @param  TIM_ICInitStruct pointer to the the TIMx input channel 1 configuration data structure
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: TIMx registers are de-initialized
  *          - ERROR: not applicable
  */
static ErrorStatus IC1Config(TIM_TypeDef *TIMx, LL_TIM_IC_InitTypeDef *TIM_ICInitStruct)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_CC1_INSTANCE(TIMx));              /* 执行语句 */
  assert_param(IS_LL_TIM_IC_POLARITY(TIM_ICInitStruct->ICPolarity));  /* 执行语句 */
  assert_param(IS_LL_TIM_ACTIVEINPUT(TIM_ICInitStruct->ICActiveInput));  /* 执行语句 */
  assert_param(IS_LL_TIM_ICPSC(TIM_ICInitStruct->ICPrescaler));  /* 执行语句 */
  assert_param(IS_LL_TIM_IC_FILTER(TIM_ICInitStruct->ICFilter));  /* 执行语句 */

  /* Disable the Channel 1: Reset the CC1E Bit */
  TIMx->CCER &= (uint32_t)~TIM_CCER_CC1E;               /* 赋值或初始化 */

  /* Select the Input and set the filter and the prescaler value */
  MODIFY_REG(TIMx->CCMR1,                               /* 列表续行 */
             (TIM_CCMR1_CC1S | TIM_CCMR1_IC1F | TIM_CCMR1_IC1PSC),  /* 列表续行 */
             (TIM_ICInitStruct->ICActiveInput | TIM_ICInitStruct->ICFilter | TIM_ICInitStruct->ICPrescaler) >> 16U);  /* 执行语句 */

  /* Select the Polarity and set the CC1E Bit */
  MODIFY_REG(TIMx->CCER,                                /* 列表续行 */
             (TIM_CCER_CC1P | TIM_CCER_CC1NP),          /* 列表续行 */
             (TIM_ICInitStruct->ICPolarity | TIM_CCER_CC1E));  /* 执行语句 */

  return SUCCESS;                                       /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configure the TIMx input channel 2.
  * @param  TIMx Timer Instance
  * @param  TIM_ICInitStruct pointer to the the TIMx input channel 2 configuration data structure
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: TIMx registers are de-initialized
  *          - ERROR: not applicable
  */
static ErrorStatus IC2Config(TIM_TypeDef *TIMx, LL_TIM_IC_InitTypeDef *TIM_ICInitStruct)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_CC2_INSTANCE(TIMx));              /* 执行语句 */
  assert_param(IS_LL_TIM_IC_POLARITY(TIM_ICInitStruct->ICPolarity));  /* 执行语句 */
  assert_param(IS_LL_TIM_ACTIVEINPUT(TIM_ICInitStruct->ICActiveInput));  /* 执行语句 */
  assert_param(IS_LL_TIM_ICPSC(TIM_ICInitStruct->ICPrescaler));  /* 执行语句 */
  assert_param(IS_LL_TIM_IC_FILTER(TIM_ICInitStruct->ICFilter));  /* 执行语句 */

  /* Disable the Channel 2: Reset the CC2E Bit */
  TIMx->CCER &= (uint32_t)~TIM_CCER_CC2E;               /* 赋值或初始化 */

  /* Select the Input and set the filter and the prescaler value */
  MODIFY_REG(TIMx->CCMR1,                               /* 列表续行 */
             (TIM_CCMR1_CC2S | TIM_CCMR1_IC2F | TIM_CCMR1_IC2PSC),  /* 列表续行 */
             (TIM_ICInitStruct->ICActiveInput | TIM_ICInitStruct->ICFilter | TIM_ICInitStruct->ICPrescaler) >> 8U);  /* 执行语句 */

  /* Select the Polarity and set the CC2E Bit */
  MODIFY_REG(TIMx->CCER,                                /* 列表续行 */
             (TIM_CCER_CC2P | TIM_CCER_CC2NP),          /* 列表续行 */
             ((TIM_ICInitStruct->ICPolarity << 4U) | TIM_CCER_CC2E));  /* 执行语句 */

  return SUCCESS;                                       /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configure the TIMx input channel 3.
  * @param  TIMx Timer Instance
  * @param  TIM_ICInitStruct pointer to the the TIMx input channel 3 configuration data structure
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: TIMx registers are de-initialized
  *          - ERROR: not applicable
  */
static ErrorStatus IC3Config(TIM_TypeDef *TIMx, LL_TIM_IC_InitTypeDef *TIM_ICInitStruct)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_CC3_INSTANCE(TIMx));              /* 执行语句 */
  assert_param(IS_LL_TIM_IC_POLARITY(TIM_ICInitStruct->ICPolarity));  /* 执行语句 */
  assert_param(IS_LL_TIM_ACTIVEINPUT(TIM_ICInitStruct->ICActiveInput));  /* 执行语句 */
  assert_param(IS_LL_TIM_ICPSC(TIM_ICInitStruct->ICPrescaler));  /* 执行语句 */
  assert_param(IS_LL_TIM_IC_FILTER(TIM_ICInitStruct->ICFilter));  /* 执行语句 */

  /* Disable the Channel 3: Reset the CC3E Bit */
  TIMx->CCER &= (uint32_t)~TIM_CCER_CC3E;               /* 赋值或初始化 */

  /* Select the Input and set the filter and the prescaler value */
  MODIFY_REG(TIMx->CCMR2,                               /* 列表续行 */
             (TIM_CCMR2_CC3S | TIM_CCMR2_IC3F | TIM_CCMR2_IC3PSC),  /* 列表续行 */
             (TIM_ICInitStruct->ICActiveInput | TIM_ICInitStruct->ICFilter | TIM_ICInitStruct->ICPrescaler) >> 16U);  /* 执行语句 */

  /* Select the Polarity and set the CC3E Bit */
  MODIFY_REG(TIMx->CCER,                                /* 列表续行 */
             (TIM_CCER_CC3P | TIM_CCER_CC3NP),          /* 列表续行 */
             ((TIM_ICInitStruct->ICPolarity << 8U) | TIM_CCER_CC3E));  /* 执行语句 */

  return SUCCESS;                                       /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configure the TIMx input channel 4.
  * @param  TIMx Timer Instance
  * @param  TIM_ICInitStruct pointer to the the TIMx input channel 4 configuration data structure
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: TIMx registers are de-initialized
  *          - ERROR: not applicable
  */
static ErrorStatus IC4Config(TIM_TypeDef *TIMx, LL_TIM_IC_InitTypeDef *TIM_ICInitStruct)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_TIM_CC4_INSTANCE(TIMx));              /* 执行语句 */
  assert_param(IS_LL_TIM_IC_POLARITY(TIM_ICInitStruct->ICPolarity));  /* 执行语句 */
  assert_param(IS_LL_TIM_ACTIVEINPUT(TIM_ICInitStruct->ICActiveInput));  /* 执行语句 */
  assert_param(IS_LL_TIM_ICPSC(TIM_ICInitStruct->ICPrescaler));  /* 执行语句 */
  assert_param(IS_LL_TIM_IC_FILTER(TIM_ICInitStruct->ICFilter));  /* 执行语句 */

  /* Disable the Channel 4: Reset the CC4E Bit */
  TIMx->CCER &= (uint32_t)~TIM_CCER_CC4E;               /* 赋值或初始化 */

  /* Select the Input and set the filter and the prescaler value */
  MODIFY_REG(TIMx->CCMR2,                               /* 列表续行 */
             (TIM_CCMR2_CC4S | TIM_CCMR2_IC4F | TIM_CCMR2_IC4PSC),  /* 列表续行 */
             (TIM_ICInitStruct->ICActiveInput | TIM_ICInitStruct->ICFilter | TIM_ICInitStruct->ICPrescaler) >> 8U);  /* 执行语句 */

  /* Select the Polarity and set the CC2E Bit */
  MODIFY_REG(TIMx->CCER,                                /* 列表续行 */
             (TIM_CCER_CC4P | TIM_CCER_CC4NP),          /* 列表续行 */
             ((TIM_ICInitStruct->ICPolarity << 12U) | TIM_CCER_CC4E));  /* 执行语句 */

  return SUCCESS;                                       /* 返回值 */
}                                                       /* 代码块结束 */


/**
  * @}
  */

/**
  * @}
  */

#endif /* TIM1 || TIM2 || TIM3 || TIM4 || TIM5 || TIM6 || TIM7 || TIM8 || TIM9 || TIM10 || TIM11 || TIM12 || TIM13 || TIM14 */

/**
  * @}
  */

#endif /* USE_FULL_LL_DRIVER */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
