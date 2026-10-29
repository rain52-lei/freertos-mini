/* 文件: stm32f4xx_ll_rtc.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_ll_rtc.c
  * @author  MCD Application Team
  * @brief   RTC LL module driver.
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
#if defined(USE_FULL_LL_DRIVER)                         /* 条件编译 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_ll_rtc.h"                           /* 包含 STM32/HAL 头文件 */
#include "stm32f4xx_ll_cortex.h"                        /* 包含 STM32/HAL 头文件 */
#ifdef  USE_FULL_ASSERT                                 /* 条件编译 */
#include "stm32_assert.h"                               /* 包含 STM32/HAL 头文件 */
#else                                                   /* 条件编译否则 */
#define assert_param(expr) ((void)0U)                   /* 定义宏 */
#endif                                                  /* 条件编译结束 */

/** @addtogroup STM32F4xx_LL_Driver
  * @{
  */

#if defined(RTC)                                        /* 条件编译 */

/** @addtogroup RTC_LL
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/** @addtogroup RTC_LL_Private_Constants
  * @{
  */
/* Default values used for prescaler */
#define RTC_ASYNCH_PRESC_DEFAULT     0x0000007FU        /* 定义宏 */
#define RTC_SYNCH_PRESC_DEFAULT      0x000000FFU        /* 定义宏 */

/* Values used for timeout */
#define RTC_INITMODE_TIMEOUT         1000U /* 1s when tick set to 1ms */
#define RTC_SYNCHRO_TIMEOUT          1000U /* 1s when tick set to 1ms */
/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/** @addtogroup RTC_LL_Private_Macros
  * @{
  */

#define IS_LL_RTC_HOURFORMAT(__VALUE__) (((__VALUE__) == LL_RTC_HOURFORMAT_24HOUR) \
                                      || ((__VALUE__) == LL_RTC_HOURFORMAT_AMPM))  /* 源代码 */

#define IS_LL_RTC_ASYNCH_PREDIV(__VALUE__)   ((__VALUE__) <= 0x7FU)  /* 定义宏 */

#define IS_LL_RTC_SYNCH_PREDIV(__VALUE__)    ((__VALUE__) <= 0x7FFFU)  /* 定义宏 */

#define IS_LL_RTC_FORMAT(__VALUE__) (((__VALUE__) == LL_RTC_FORMAT_BIN) \
                                  || ((__VALUE__) == LL_RTC_FORMAT_BCD))  /* 源代码 */

#define IS_LL_RTC_TIME_FORMAT(__VALUE__) (((__VALUE__) == LL_RTC_TIME_FORMAT_AM_OR_24) \
                                       || ((__VALUE__) == LL_RTC_TIME_FORMAT_PM))  /* 源代码 */

#define IS_LL_RTC_HOUR12(__HOUR__)            (((__HOUR__) > 0U) && ((__HOUR__) <= 12U))  /* 定义宏 */
#define IS_LL_RTC_HOUR24(__HOUR__)            ((__HOUR__) <= 23U)  /* 定义宏 */
#define IS_LL_RTC_MINUTES(__MINUTES__)        ((__MINUTES__) <= 59U)  /* 定义宏 */
#define IS_LL_RTC_SECONDS(__SECONDS__)        ((__SECONDS__) <= 59U)  /* 定义宏 */

#define IS_LL_RTC_WEEKDAY(__VALUE__) (((__VALUE__) == LL_RTC_WEEKDAY_MONDAY) \
                                   || ((__VALUE__) == LL_RTC_WEEKDAY_TUESDAY) \
                                   || ((__VALUE__) == LL_RTC_WEEKDAY_WEDNESDAY) \
                                   || ((__VALUE__) == LL_RTC_WEEKDAY_THURSDAY) \
                                   || ((__VALUE__) == LL_RTC_WEEKDAY_FRIDAY) \
                                   || ((__VALUE__) == LL_RTC_WEEKDAY_SATURDAY) \
                                   || ((__VALUE__) == LL_RTC_WEEKDAY_SUNDAY))  /* 源代码 */

#define IS_LL_RTC_DAY(__DAY__)    (((__DAY__) >= 1U) && ((__DAY__) <= 31U))  /* 定义宏 */

#define IS_LL_RTC_MONTH(__VALUE__) (((__VALUE__) == LL_RTC_MONTH_JANUARY) \
                                 || ((__VALUE__) == LL_RTC_MONTH_FEBRUARY) \
                                 || ((__VALUE__) == LL_RTC_MONTH_MARCH) \
                                 || ((__VALUE__) == LL_RTC_MONTH_APRIL) \
                                 || ((__VALUE__) == LL_RTC_MONTH_MAY) \
                                 || ((__VALUE__) == LL_RTC_MONTH_JUNE) \
                                 || ((__VALUE__) == LL_RTC_MONTH_JULY) \
                                 || ((__VALUE__) == LL_RTC_MONTH_AUGUST) \
                                 || ((__VALUE__) == LL_RTC_MONTH_SEPTEMBER) \
                                 || ((__VALUE__) == LL_RTC_MONTH_OCTOBER) \
                                 || ((__VALUE__) == LL_RTC_MONTH_NOVEMBER) \
                                 || ((__VALUE__) == LL_RTC_MONTH_DECEMBER))  /* 源代码 */

#define IS_LL_RTC_YEAR(__YEAR__) ((__YEAR__) <= 99U)    /* 定义宏 */

#define IS_LL_RTC_ALMA_MASK(__VALUE__) (((__VALUE__) == LL_RTC_ALMA_MASK_NONE) \
                                     || ((__VALUE__) == LL_RTC_ALMA_MASK_DATEWEEKDAY) \
                                     || ((__VALUE__) == LL_RTC_ALMA_MASK_HOURS) \
                                     || ((__VALUE__) == LL_RTC_ALMA_MASK_MINUTES) \
                                     || ((__VALUE__) == LL_RTC_ALMA_MASK_SECONDS) \
                                     || ((__VALUE__) == LL_RTC_ALMA_MASK_ALL))  /* 源代码 */

#define IS_LL_RTC_ALMB_MASK(__VALUE__) (((__VALUE__) == LL_RTC_ALMB_MASK_NONE) \
                                     || ((__VALUE__) == LL_RTC_ALMB_MASK_DATEWEEKDAY) \
                                     || ((__VALUE__) == LL_RTC_ALMB_MASK_HOURS) \
                                     || ((__VALUE__) == LL_RTC_ALMB_MASK_MINUTES) \
                                     || ((__VALUE__) == LL_RTC_ALMB_MASK_SECONDS) \
                                     || ((__VALUE__) == LL_RTC_ALMB_MASK_ALL))  /* 源代码 */


#define IS_LL_RTC_ALMA_DATE_WEEKDAY_SEL(__SEL__) (((__SEL__) == LL_RTC_ALMA_DATEWEEKDAYSEL_DATE) || \
                                                  ((__SEL__) == LL_RTC_ALMA_DATEWEEKDAYSEL_WEEKDAY))  /* 源代码 */

#define IS_LL_RTC_ALMB_DATE_WEEKDAY_SEL(__SEL__) (((__SEL__) == LL_RTC_ALMB_DATEWEEKDAYSEL_DATE) || \
                                                  ((__SEL__) == LL_RTC_ALMB_DATEWEEKDAYSEL_WEEKDAY))  /* 源代码 */


/**
  * @}
  */
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/** @addtogroup RTC_LL_Exported_Functions
  * @{
  */

/** @addtogroup RTC_LL_EF_Init
  * @{
  */

/**
  * @brief  De-Initializes the RTC registers to their default reset values.
  * @note   This function doesn't reset the RTC Clock source and RTC Backup Data
  *         registers.
  * @param  RTCx RTC Instance
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: RTC registers are de-initialized
  *          - ERROR: RTC registers are not de-initialized
  */
ErrorStatus LL_RTC_DeInit(RTC_TypeDef *RTCx)            /* 源代码 */
{                                                       /* 代码块开始 */
  ErrorStatus status = ERROR;                           /* 赋值或初始化 */

  /* Check the parameter */
  assert_param(IS_RTC_ALL_INSTANCE(RTCx));              /* 执行语句 */

  /* Disable the write protection for RTC registers */
  LL_RTC_DisableWriteProtection(RTCx);                  /* 执行语句 */

  /* Set Initialization mode */
  if (LL_RTC_EnterInitMode(RTCx) != ERROR)              /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Reset TR, DR and CR registers */
    LL_RTC_WriteReg(RTCx, TR,       0x00000000U);       /* 执行语句 */
#if defined(RTC_WAKEUP_SUPPORT)                         /* 条件编译 */
    LL_RTC_WriteReg(RTCx, WUTR,     RTC_WUTR_WUT);      /* 执行语句 */
#endif /* RTC_WAKEUP_SUPPORT */
    LL_RTC_WriteReg(RTCx, DR  ,     (RTC_DR_WDU_0 | RTC_DR_MU_0 | RTC_DR_DU_0));  /* 执行语句 */
    /* Reset All CR bits except CR[2:0] */
#if defined(RTC_WAKEUP_SUPPORT)                         /* 条件编译 */
    LL_RTC_WriteReg(RTCx, CR, (LL_RTC_ReadReg(RTCx, CR) & RTC_CR_WUCKSEL));  /* 执行语句 */
#else                                                   /* 条件编译否则 */
    LL_RTC_WriteReg(RTCx, CR, 0x00000000U);             /* 执行语句 */
#endif /* RTC_WAKEUP_SUPPORT */
    LL_RTC_WriteReg(RTCx, PRER,     (RTC_PRER_PREDIV_A | RTC_SYNCH_PRESC_DEFAULT));  /* 执行语句 */
    LL_RTC_WriteReg(RTCx, ALRMAR,   0x00000000U);       /* 执行语句 */
    LL_RTC_WriteReg(RTCx, ALRMBR,   0x00000000U);       /* 执行语句 */
    LL_RTC_WriteReg(RTCx, SHIFTR,   0x00000000U);       /* 执行语句 */
    LL_RTC_WriteReg(RTCx, CALR,     0x00000000U);       /* 执行语句 */
    LL_RTC_WriteReg(RTCx, ALRMASSR, 0x00000000U);       /* 执行语句 */
    LL_RTC_WriteReg(RTCx, ALRMBSSR, 0x00000000U);       /* 执行语句 */

    /* Reset ISR register and exit initialization mode */
    LL_RTC_WriteReg(RTCx, ISR,      0x00000000U);       /* 执行语句 */

    /* Reset Tamper and alternate functions configuration register */
    LL_RTC_WriteReg(RTCx, TAFCR, 0x00000000U);          /* 执行语句 */

    /* Wait till the RTC RSF flag is set */
    status = LL_RTC_WaitForSynchro(RTCx);               /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Enable the write protection for RTC registers */
  LL_RTC_EnableWriteProtection(RTCx);                   /* 执行语句 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initializes the RTC registers according to the specified parameters
  *         in RTC_InitStruct.
  * @param  RTCx RTC Instance
  * @param  RTC_InitStruct pointer to a @ref LL_RTC_InitTypeDef structure that contains
  *         the configuration information for the RTC peripheral.
  * @note   The RTC Prescaler register is write protected and can be written in
  *         initialization mode only.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: RTC registers are initialized
  *          - ERROR: RTC registers are not initialized
  */
ErrorStatus LL_RTC_Init(RTC_TypeDef *RTCx, LL_RTC_InitTypeDef *RTC_InitStruct)  /* 源代码 */
{                                                       /* 代码块开始 */
  ErrorStatus status = ERROR;                           /* 赋值或初始化 */

  /* Check the parameters */
  assert_param(IS_RTC_ALL_INSTANCE(RTCx));              /* 执行语句 */
  assert_param(IS_LL_RTC_HOURFORMAT(RTC_InitStruct->HourFormat));  /* 执行语句 */
  assert_param(IS_LL_RTC_ASYNCH_PREDIV(RTC_InitStruct->AsynchPrescaler));  /* 执行语句 */
  assert_param(IS_LL_RTC_SYNCH_PREDIV(RTC_InitStruct->SynchPrescaler));  /* 执行语句 */

  /* Disable the write protection for RTC registers */
  LL_RTC_DisableWriteProtection(RTCx);                  /* 执行语句 */

  /* Set Initialization mode */
  if (LL_RTC_EnterInitMode(RTCx) != ERROR)              /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Set Hour Format */
    LL_RTC_SetHourFormat(RTCx, RTC_InitStruct->HourFormat);  /* 执行语句 */

    /* Configure Synchronous and Asynchronous prescaler factor */
    LL_RTC_SetSynchPrescaler(RTCx, RTC_InitStruct->SynchPrescaler);  /* 执行语句 */
    LL_RTC_SetAsynchPrescaler(RTCx, RTC_InitStruct->AsynchPrescaler);  /* 执行语句 */

    /* Exit Initialization mode */
    LL_RTC_DisableInitMode(RTCx);                       /* 执行语句 */

    status = SUCCESS;                                   /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  /* Enable the write protection for RTC registers */
  LL_RTC_EnableWriteProtection(RTCx);                   /* 执行语句 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Set each @ref LL_RTC_InitTypeDef field to default value.
  * @param  RTC_InitStruct pointer to a @ref LL_RTC_InitTypeDef structure which will be initialized.
  * @retval None
  */
void LL_RTC_StructInit(LL_RTC_InitTypeDef *RTC_InitStruct)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Set RTC_InitStruct fields to default values */
  RTC_InitStruct->HourFormat      = LL_RTC_HOURFORMAT_24HOUR;  /* 赋值或初始化 */
  RTC_InitStruct->AsynchPrescaler = RTC_ASYNCH_PRESC_DEFAULT;  /* 赋值或初始化 */
  RTC_InitStruct->SynchPrescaler  = RTC_SYNCH_PRESC_DEFAULT;  /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  Set the RTC current time.
  * @param  RTCx RTC Instance
  * @param  RTC_Format This parameter can be one of the following values:
  *         @arg @ref LL_RTC_FORMAT_BIN
  *         @arg @ref LL_RTC_FORMAT_BCD
  * @param  RTC_TimeStruct pointer to a RTC_TimeTypeDef structure that contains
  *                        the time configuration information for the RTC.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: RTC Time register is configured
  *          - ERROR: RTC Time register is not configured
  */
ErrorStatus LL_RTC_TIME_Init(RTC_TypeDef *RTCx, uint32_t RTC_Format, LL_RTC_TimeTypeDef *RTC_TimeStruct)  /* 源代码 */
{                                                       /* 代码块开始 */
  ErrorStatus status = ERROR;                           /* 赋值或初始化 */

  /* Check the parameters */
  assert_param(IS_RTC_ALL_INSTANCE(RTCx));              /* 执行语句 */
  assert_param(IS_LL_RTC_FORMAT(RTC_Format));           /* 执行语句 */

  if (RTC_Format == LL_RTC_FORMAT_BIN)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (LL_RTC_GetHourFormat(RTCx) != LL_RTC_HOURFORMAT_24HOUR)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      assert_param(IS_LL_RTC_HOUR12(RTC_TimeStruct->Hours));  /* 执行语句 */
      assert_param(IS_LL_RTC_TIME_FORMAT(RTC_TimeStruct->TimeFormat));  /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      RTC_TimeStruct->TimeFormat = 0x00U;               /* 赋值或初始化 */
      assert_param(IS_LL_RTC_HOUR24(RTC_TimeStruct->Hours));  /* 执行语句 */
    }                                                   /* 代码块结束 */
    assert_param(IS_LL_RTC_MINUTES(RTC_TimeStruct->Minutes));  /* 执行语句 */
    assert_param(IS_LL_RTC_SECONDS(RTC_TimeStruct->Seconds));  /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    if (LL_RTC_GetHourFormat(RTCx) != LL_RTC_HOURFORMAT_24HOUR)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      assert_param(IS_LL_RTC_HOUR12(__LL_RTC_CONVERT_BCD2BIN(RTC_TimeStruct->Hours)));  /* 执行语句 */
      assert_param(IS_LL_RTC_TIME_FORMAT(RTC_TimeStruct->TimeFormat));  /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      RTC_TimeStruct->TimeFormat = 0x00U;               /* 赋值或初始化 */
      assert_param(IS_LL_RTC_HOUR24(__LL_RTC_CONVERT_BCD2BIN(RTC_TimeStruct->Hours)));  /* 执行语句 */
    }                                                   /* 代码块结束 */
    assert_param(IS_LL_RTC_MINUTES(__LL_RTC_CONVERT_BCD2BIN(RTC_TimeStruct->Minutes)));  /* 执行语句 */
    assert_param(IS_LL_RTC_SECONDS(__LL_RTC_CONVERT_BCD2BIN(RTC_TimeStruct->Seconds)));  /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Disable the write protection for RTC registers */
  LL_RTC_DisableWriteProtection(RTCx);                  /* 执行语句 */

  /* Set Initialization mode */
  if (LL_RTC_EnterInitMode(RTCx) != ERROR)              /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check the input parameters format */
    if (RTC_Format != LL_RTC_FORMAT_BIN)                /* 条件判断 */
    {                                                   /* 代码块开始 */
      LL_RTC_TIME_Config(RTCx, RTC_TimeStruct->TimeFormat, RTC_TimeStruct->Hours,  /* 列表续行 */
                         RTC_TimeStruct->Minutes, RTC_TimeStruct->Seconds);  /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      LL_RTC_TIME_Config(RTCx, RTC_TimeStruct->TimeFormat, __LL_RTC_CONVERT_BIN2BCD(RTC_TimeStruct->Hours),  /* 列表续行 */
                         __LL_RTC_CONVERT_BIN2BCD(RTC_TimeStruct->Minutes),  /* 列表续行 */
                         __LL_RTC_CONVERT_BIN2BCD(RTC_TimeStruct->Seconds));  /* 执行语句 */
    }                                                   /* 代码块结束 */

    /* Exit Initialization mode */
    LL_RTC_DisableInitMode(RTC);                        /* 执行语句 */

    /* If  RTC_CR_BYPSHAD bit = 0, wait for synchro else this check is not needed */
    if (LL_RTC_IsShadowRegBypassEnabled(RTCx) == 0U)    /* 条件判断 */
    {                                                   /* 代码块开始 */
      status = LL_RTC_WaitForSynchro(RTCx);             /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      status = SUCCESS;                                 /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /* Enable the write protection for RTC registers */
  LL_RTC_EnableWriteProtection(RTCx);                   /* 执行语句 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Set each @ref LL_RTC_TimeTypeDef field to default value (Time = 00h:00min:00sec).
  * @param  RTC_TimeStruct pointer to a @ref LL_RTC_TimeTypeDef structure which will be initialized.
  * @retval None
  */
void LL_RTC_TIME_StructInit(LL_RTC_TimeTypeDef *RTC_TimeStruct)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Time = 00h:00min:00sec */
  RTC_TimeStruct->TimeFormat = LL_RTC_TIME_FORMAT_AM_OR_24;  /* 赋值或初始化 */
  RTC_TimeStruct->Hours      = 0U;                      /* 赋值或初始化 */
  RTC_TimeStruct->Minutes    = 0U;                      /* 赋值或初始化 */
  RTC_TimeStruct->Seconds    = 0U;                      /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  Set the RTC current date.
  * @param  RTCx RTC Instance
  * @param  RTC_Format This parameter can be one of the following values:
  *         @arg @ref LL_RTC_FORMAT_BIN
  *         @arg @ref LL_RTC_FORMAT_BCD
  * @param  RTC_DateStruct pointer to a RTC_DateTypeDef structure that contains
  *                         the date configuration information for the RTC.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: RTC Day register is configured
  *          - ERROR: RTC Day register is not configured
  */
ErrorStatus LL_RTC_DATE_Init(RTC_TypeDef *RTCx, uint32_t RTC_Format, LL_RTC_DateTypeDef *RTC_DateStruct)  /* 源代码 */
{                                                       /* 代码块开始 */
  ErrorStatus status = ERROR;                           /* 赋值或初始化 */

  /* Check the parameters */
  assert_param(IS_RTC_ALL_INSTANCE(RTCx));              /* 执行语句 */
  assert_param(IS_LL_RTC_FORMAT(RTC_Format));           /* 执行语句 */

  if ((RTC_Format == LL_RTC_FORMAT_BIN) && ((RTC_DateStruct->Month & 0x10U) == 0x10U))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    RTC_DateStruct->Month = (RTC_DateStruct->Month & (uint32_t)~(0x10U)) + 0x0AU;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  if (RTC_Format == LL_RTC_FORMAT_BIN)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    assert_param(IS_LL_RTC_YEAR(RTC_DateStruct->Year));  /* 执行语句 */
    assert_param(IS_LL_RTC_MONTH(RTC_DateStruct->Month));  /* 执行语句 */
    assert_param(IS_LL_RTC_DAY(RTC_DateStruct->Day));   /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    assert_param(IS_LL_RTC_YEAR(__LL_RTC_CONVERT_BCD2BIN(RTC_DateStruct->Year)));  /* 执行语句 */
    assert_param(IS_LL_RTC_MONTH(__LL_RTC_CONVERT_BCD2BIN(RTC_DateStruct->Month)));  /* 执行语句 */
    assert_param(IS_LL_RTC_DAY(__LL_RTC_CONVERT_BCD2BIN(RTC_DateStruct->Day)));  /* 执行语句 */
  }                                                     /* 代码块结束 */
  assert_param(IS_LL_RTC_WEEKDAY(RTC_DateStruct->WeekDay));  /* 执行语句 */

  /* Disable the write protection for RTC registers */
  LL_RTC_DisableWriteProtection(RTCx);                  /* 执行语句 */

  /* Set Initialization mode */
  if (LL_RTC_EnterInitMode(RTCx) != ERROR)              /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check the input parameters format */
    if (RTC_Format != LL_RTC_FORMAT_BIN)                /* 条件判断 */
    {                                                   /* 代码块开始 */
      LL_RTC_DATE_Config(RTCx, RTC_DateStruct->WeekDay, RTC_DateStruct->Day, RTC_DateStruct->Month, RTC_DateStruct->Year);  /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      LL_RTC_DATE_Config(RTCx, RTC_DateStruct->WeekDay, __LL_RTC_CONVERT_BIN2BCD(RTC_DateStruct->Day),  /* 列表续行 */
                         __LL_RTC_CONVERT_BIN2BCD(RTC_DateStruct->Month), __LL_RTC_CONVERT_BIN2BCD(RTC_DateStruct->Year));  /* 执行语句 */
    }                                                   /* 代码块结束 */

    /* Exit Initialization mode */
    LL_RTC_DisableInitMode(RTC);                        /* 执行语句 */

    /* If  RTC_CR_BYPSHAD bit = 0, wait for synchro else this check is not needed */
    if (LL_RTC_IsShadowRegBypassEnabled(RTCx) == 0U)    /* 条件判断 */
    {                                                   /* 代码块开始 */
      status = LL_RTC_WaitForSynchro(RTCx);             /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      status = SUCCESS;                                 /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /* Enable the write protection for RTC registers */
  LL_RTC_EnableWriteProtection(RTCx);                   /* 执行语句 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Set each @ref LL_RTC_DateTypeDef field to default value (date = Monday, January 01 xx00)
  * @param  RTC_DateStruct pointer to a @ref LL_RTC_DateTypeDef structure which will be initialized.
  * @retval None
  */
void LL_RTC_DATE_StructInit(LL_RTC_DateTypeDef *RTC_DateStruct)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Monday, January 01 xx00 */
  RTC_DateStruct->WeekDay = LL_RTC_WEEKDAY_MONDAY;      /* 赋值或初始化 */
  RTC_DateStruct->Day     = 1U;                         /* 赋值或初始化 */
  RTC_DateStruct->Month   = LL_RTC_MONTH_JANUARY;       /* 赋值或初始化 */
  RTC_DateStruct->Year    = 0U;                         /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  Set the RTC Alarm A.
  * @note   The Alarm register can only be written when the corresponding Alarm
  *         is disabled (Use @ref LL_RTC_ALMA_Disable function).
  * @param  RTCx RTC Instance
  * @param  RTC_Format This parameter can be one of the following values:
  *         @arg @ref LL_RTC_FORMAT_BIN
  *         @arg @ref LL_RTC_FORMAT_BCD
  * @param  RTC_AlarmStruct pointer to a @ref LL_RTC_AlarmTypeDef structure that
  *                         contains the alarm configuration parameters.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: ALARMA registers are configured
  *          - ERROR: ALARMA registers are not configured
  */
ErrorStatus LL_RTC_ALMA_Init(RTC_TypeDef *RTCx, uint32_t RTC_Format, LL_RTC_AlarmTypeDef *RTC_AlarmStruct)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_RTC_ALL_INSTANCE(RTCx));              /* 执行语句 */
  assert_param(IS_LL_RTC_FORMAT(RTC_Format));           /* 执行语句 */
  assert_param(IS_LL_RTC_ALMA_MASK(RTC_AlarmStruct->AlarmMask));  /* 执行语句 */
  assert_param(IS_LL_RTC_ALMA_DATE_WEEKDAY_SEL(RTC_AlarmStruct->AlarmDateWeekDaySel));  /* 执行语句 */

  if (RTC_Format == LL_RTC_FORMAT_BIN)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (LL_RTC_GetHourFormat(RTCx) != LL_RTC_HOURFORMAT_24HOUR)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      assert_param(IS_LL_RTC_HOUR12(RTC_AlarmStruct->AlarmTime.Hours));  /* 执行语句 */
      assert_param(IS_LL_RTC_TIME_FORMAT(RTC_AlarmStruct->AlarmTime.TimeFormat));  /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      RTC_AlarmStruct->AlarmTime.TimeFormat = 0x00U;    /* 赋值或初始化 */
      assert_param(IS_LL_RTC_HOUR24(RTC_AlarmStruct->AlarmTime.Hours));  /* 执行语句 */
    }                                                   /* 代码块结束 */
    assert_param(IS_LL_RTC_MINUTES(RTC_AlarmStruct->AlarmTime.Minutes));  /* 执行语句 */
    assert_param(IS_LL_RTC_SECONDS(RTC_AlarmStruct->AlarmTime.Seconds));  /* 执行语句 */

    if (RTC_AlarmStruct->AlarmDateWeekDaySel == LL_RTC_ALMA_DATEWEEKDAYSEL_DATE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      assert_param(IS_LL_RTC_DAY(RTC_AlarmStruct->AlarmDateWeekDay));  /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      assert_param(IS_LL_RTC_WEEKDAY(RTC_AlarmStruct->AlarmDateWeekDay));  /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    if (LL_RTC_GetHourFormat(RTCx) != LL_RTC_HOURFORMAT_24HOUR)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      assert_param(IS_LL_RTC_HOUR12(__LL_RTC_CONVERT_BCD2BIN(RTC_AlarmStruct->AlarmTime.Hours)));  /* 执行语句 */
      assert_param(IS_LL_RTC_TIME_FORMAT(RTC_AlarmStruct->AlarmTime.TimeFormat));  /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      RTC_AlarmStruct->AlarmTime.TimeFormat = 0x00U;    /* 赋值或初始化 */
      assert_param(IS_LL_RTC_HOUR24(__LL_RTC_CONVERT_BCD2BIN(RTC_AlarmStruct->AlarmTime.Hours)));  /* 执行语句 */
    }                                                   /* 代码块结束 */

    assert_param(IS_LL_RTC_MINUTES(__LL_RTC_CONVERT_BCD2BIN(RTC_AlarmStruct->AlarmTime.Minutes)));  /* 执行语句 */
    assert_param(IS_LL_RTC_SECONDS(__LL_RTC_CONVERT_BCD2BIN(RTC_AlarmStruct->AlarmTime.Seconds)));  /* 执行语句 */

    if (RTC_AlarmStruct->AlarmDateWeekDaySel == LL_RTC_ALMA_DATEWEEKDAYSEL_DATE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      assert_param(IS_LL_RTC_DAY(__LL_RTC_CONVERT_BCD2BIN(RTC_AlarmStruct->AlarmDateWeekDay)));  /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      assert_param(IS_LL_RTC_WEEKDAY(__LL_RTC_CONVERT_BCD2BIN(RTC_AlarmStruct->AlarmDateWeekDay)));  /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Disable the write protection for RTC registers */
  LL_RTC_DisableWriteProtection(RTCx);                  /* 执行语句 */

  /* Select weekday selection */
  if (RTC_AlarmStruct->AlarmDateWeekDaySel == LL_RTC_ALMA_DATEWEEKDAYSEL_DATE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Set the date for ALARM */
    LL_RTC_ALMA_DisableWeekday(RTCx);                   /* 执行语句 */
    if (RTC_Format != LL_RTC_FORMAT_BIN)                /* 条件判断 */
    {                                                   /* 代码块开始 */
      LL_RTC_ALMA_SetDay(RTCx, RTC_AlarmStruct->AlarmDateWeekDay);  /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      LL_RTC_ALMA_SetDay(RTCx, __LL_RTC_CONVERT_BIN2BCD(RTC_AlarmStruct->AlarmDateWeekDay));  /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Set the week day for ALARM */
    LL_RTC_ALMA_EnableWeekday(RTCx);                    /* 执行语句 */
    LL_RTC_ALMA_SetWeekDay(RTCx, RTC_AlarmStruct->AlarmDateWeekDay);  /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Configure the Alarm register */
  if (RTC_Format != LL_RTC_FORMAT_BIN)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    LL_RTC_ALMA_ConfigTime(RTCx, RTC_AlarmStruct->AlarmTime.TimeFormat, RTC_AlarmStruct->AlarmTime.Hours,  /* 列表续行 */
                           RTC_AlarmStruct->AlarmTime.Minutes, RTC_AlarmStruct->AlarmTime.Seconds);  /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    LL_RTC_ALMA_ConfigTime(RTCx, RTC_AlarmStruct->AlarmTime.TimeFormat,  /* 列表续行 */
                           __LL_RTC_CONVERT_BIN2BCD(RTC_AlarmStruct->AlarmTime.Hours),  /* 列表续行 */
                           __LL_RTC_CONVERT_BIN2BCD(RTC_AlarmStruct->AlarmTime.Minutes),  /* 列表续行 */
                           __LL_RTC_CONVERT_BIN2BCD(RTC_AlarmStruct->AlarmTime.Seconds));  /* 执行语句 */
  }                                                     /* 代码块结束 */
  /* Set ALARM mask */
  LL_RTC_ALMA_SetMask(RTCx, RTC_AlarmStruct->AlarmMask);  /* 执行语句 */

  /* Enable the write protection for RTC registers */
  LL_RTC_EnableWriteProtection(RTCx);                   /* 执行语句 */

  return SUCCESS;                                       /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Set the RTC Alarm B.
  * @note   The Alarm register can only be written when the corresponding Alarm
  *         is disabled (@ref LL_RTC_ALMB_Disable function).
  * @param  RTCx RTC Instance
  * @param  RTC_Format This parameter can be one of the following values:
  *         @arg @ref LL_RTC_FORMAT_BIN
  *         @arg @ref LL_RTC_FORMAT_BCD
  * @param  RTC_AlarmStruct pointer to a @ref LL_RTC_AlarmTypeDef structure that
  *                         contains the alarm configuration parameters.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: ALARMB registers are configured
  *          - ERROR: ALARMB registers are not configured
  */
ErrorStatus LL_RTC_ALMB_Init(RTC_TypeDef *RTCx, uint32_t RTC_Format, LL_RTC_AlarmTypeDef *RTC_AlarmStruct)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_RTC_ALL_INSTANCE(RTCx));              /* 执行语句 */
  assert_param(IS_LL_RTC_FORMAT(RTC_Format));           /* 执行语句 */
  assert_param(IS_LL_RTC_ALMB_MASK(RTC_AlarmStruct->AlarmMask));  /* 执行语句 */
  assert_param(IS_LL_RTC_ALMB_DATE_WEEKDAY_SEL(RTC_AlarmStruct->AlarmDateWeekDaySel));  /* 执行语句 */

  if (RTC_Format == LL_RTC_FORMAT_BIN)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (LL_RTC_GetHourFormat(RTCx) != LL_RTC_HOURFORMAT_24HOUR)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      assert_param(IS_LL_RTC_HOUR12(RTC_AlarmStruct->AlarmTime.Hours));  /* 执行语句 */
      assert_param(IS_LL_RTC_TIME_FORMAT(RTC_AlarmStruct->AlarmTime.TimeFormat));  /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      RTC_AlarmStruct->AlarmTime.TimeFormat = 0x00U;    /* 赋值或初始化 */
      assert_param(IS_LL_RTC_HOUR24(RTC_AlarmStruct->AlarmTime.Hours));  /* 执行语句 */
    }                                                   /* 代码块结束 */
    assert_param(IS_LL_RTC_MINUTES(RTC_AlarmStruct->AlarmTime.Minutes));  /* 执行语句 */
    assert_param(IS_LL_RTC_SECONDS(RTC_AlarmStruct->AlarmTime.Seconds));  /* 执行语句 */

    if (RTC_AlarmStruct->AlarmDateWeekDaySel == LL_RTC_ALMB_DATEWEEKDAYSEL_DATE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      assert_param(IS_LL_RTC_DAY(RTC_AlarmStruct->AlarmDateWeekDay));  /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      assert_param(IS_LL_RTC_WEEKDAY(RTC_AlarmStruct->AlarmDateWeekDay));  /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    if (LL_RTC_GetHourFormat(RTCx) != LL_RTC_HOURFORMAT_24HOUR)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      assert_param(IS_LL_RTC_HOUR12(__LL_RTC_CONVERT_BCD2BIN(RTC_AlarmStruct->AlarmTime.Hours)));  /* 执行语句 */
      assert_param(IS_LL_RTC_TIME_FORMAT(RTC_AlarmStruct->AlarmTime.TimeFormat));  /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      RTC_AlarmStruct->AlarmTime.TimeFormat = 0x00U;    /* 赋值或初始化 */
      assert_param(IS_LL_RTC_HOUR24(__LL_RTC_CONVERT_BCD2BIN(RTC_AlarmStruct->AlarmTime.Hours)));  /* 执行语句 */
    }                                                   /* 代码块结束 */

    assert_param(IS_LL_RTC_MINUTES(__LL_RTC_CONVERT_BCD2BIN(RTC_AlarmStruct->AlarmTime.Minutes)));  /* 执行语句 */
    assert_param(IS_LL_RTC_SECONDS(__LL_RTC_CONVERT_BCD2BIN(RTC_AlarmStruct->AlarmTime.Seconds)));  /* 执行语句 */

    if (RTC_AlarmStruct->AlarmDateWeekDaySel == LL_RTC_ALMB_DATEWEEKDAYSEL_DATE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      assert_param(IS_LL_RTC_DAY(__LL_RTC_CONVERT_BCD2BIN(RTC_AlarmStruct->AlarmDateWeekDay)));  /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      assert_param(IS_LL_RTC_WEEKDAY(__LL_RTC_CONVERT_BCD2BIN(RTC_AlarmStruct->AlarmDateWeekDay)));  /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Disable the write protection for RTC registers */
  LL_RTC_DisableWriteProtection(RTCx);                  /* 执行语句 */

  /* Select weekday selection */
  if (RTC_AlarmStruct->AlarmDateWeekDaySel == LL_RTC_ALMB_DATEWEEKDAYSEL_DATE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Set the date for ALARM */
    LL_RTC_ALMB_DisableWeekday(RTCx);                   /* 执行语句 */
    if (RTC_Format != LL_RTC_FORMAT_BIN)                /* 条件判断 */
    {                                                   /* 代码块开始 */
      LL_RTC_ALMB_SetDay(RTCx, RTC_AlarmStruct->AlarmDateWeekDay);  /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      LL_RTC_ALMB_SetDay(RTCx, __LL_RTC_CONVERT_BIN2BCD(RTC_AlarmStruct->AlarmDateWeekDay));  /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Set the week day for ALARM */
    LL_RTC_ALMB_EnableWeekday(RTCx);                    /* 执行语句 */
    LL_RTC_ALMB_SetWeekDay(RTCx, RTC_AlarmStruct->AlarmDateWeekDay);  /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Configure the Alarm register */
  if (RTC_Format != LL_RTC_FORMAT_BIN)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    LL_RTC_ALMB_ConfigTime(RTCx, RTC_AlarmStruct->AlarmTime.TimeFormat, RTC_AlarmStruct->AlarmTime.Hours,  /* 列表续行 */
                           RTC_AlarmStruct->AlarmTime.Minutes, RTC_AlarmStruct->AlarmTime.Seconds);  /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    LL_RTC_ALMB_ConfigTime(RTCx, RTC_AlarmStruct->AlarmTime.TimeFormat,  /* 列表续行 */
                           __LL_RTC_CONVERT_BIN2BCD(RTC_AlarmStruct->AlarmTime.Hours),  /* 列表续行 */
                           __LL_RTC_CONVERT_BIN2BCD(RTC_AlarmStruct->AlarmTime.Minutes),  /* 列表续行 */
                           __LL_RTC_CONVERT_BIN2BCD(RTC_AlarmStruct->AlarmTime.Seconds));  /* 执行语句 */
  }                                                     /* 代码块结束 */
  /* Set ALARM mask */
  LL_RTC_ALMB_SetMask(RTCx, RTC_AlarmStruct->AlarmMask);  /* 执行语句 */

  /* Enable the write protection for RTC registers */
  LL_RTC_EnableWriteProtection(RTCx);                   /* 执行语句 */

  return SUCCESS;                                       /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Set each @ref LL_RTC_AlarmTypeDef of ALARMA field to default value (Time = 00h:00mn:00sec /
  *         Day = 1st day of the month/Mask = all fields are masked).
  * @param  RTC_AlarmStruct pointer to a @ref LL_RTC_AlarmTypeDef structure which will be initialized.
  * @retval None
  */
void LL_RTC_ALMA_StructInit(LL_RTC_AlarmTypeDef *RTC_AlarmStruct)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Alarm Time Settings : Time = 00h:00mn:00sec */
  RTC_AlarmStruct->AlarmTime.TimeFormat = LL_RTC_ALMA_TIME_FORMAT_AM;  /* 赋值或初始化 */
  RTC_AlarmStruct->AlarmTime.Hours      = 0U;           /* 赋值或初始化 */
  RTC_AlarmStruct->AlarmTime.Minutes    = 0U;           /* 赋值或初始化 */
  RTC_AlarmStruct->AlarmTime.Seconds    = 0U;           /* 赋值或初始化 */

  /* Alarm Day Settings : Day = 1st day of the month */
  RTC_AlarmStruct->AlarmDateWeekDaySel = LL_RTC_ALMA_DATEWEEKDAYSEL_DATE;  /* 赋值或初始化 */
  RTC_AlarmStruct->AlarmDateWeekDay    = 1U;            /* 赋值或初始化 */

  /* Alarm Masks Settings : Mask =  all fields are not masked */
  RTC_AlarmStruct->AlarmMask           = LL_RTC_ALMA_MASK_NONE;  /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  Set each @ref LL_RTC_AlarmTypeDef of ALARMA field to default value (Time = 00h:00mn:00sec /
  *         Day = 1st day of the month/Mask = all fields are masked).
  * @param  RTC_AlarmStruct pointer to a @ref LL_RTC_AlarmTypeDef structure which will be initialized.
  * @retval None
  */
void LL_RTC_ALMB_StructInit(LL_RTC_AlarmTypeDef *RTC_AlarmStruct)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Alarm Time Settings : Time = 00h:00mn:00sec */
  RTC_AlarmStruct->AlarmTime.TimeFormat = LL_RTC_ALMB_TIME_FORMAT_AM;  /* 赋值或初始化 */
  RTC_AlarmStruct->AlarmTime.Hours      = 0U;           /* 赋值或初始化 */
  RTC_AlarmStruct->AlarmTime.Minutes    = 0U;           /* 赋值或初始化 */
  RTC_AlarmStruct->AlarmTime.Seconds    = 0U;           /* 赋值或初始化 */

  /* Alarm Day Settings : Day = 1st day of the month */
  RTC_AlarmStruct->AlarmDateWeekDaySel = LL_RTC_ALMB_DATEWEEKDAYSEL_DATE;  /* 赋值或初始化 */
  RTC_AlarmStruct->AlarmDateWeekDay    = 1U;            /* 赋值或初始化 */

  /* Alarm Masks Settings : Mask =  all fields are not masked */
  RTC_AlarmStruct->AlarmMask           = LL_RTC_ALMB_MASK_NONE;  /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  Enters the RTC Initialization mode.
  * @note   The RTC Initialization mode is write protected, use the
  *         @ref LL_RTC_DisableWriteProtection before calling this function.
  * @param  RTCx RTC Instance
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: RTC is in Init mode
  *          - ERROR: RTC is not in Init mode
  */
ErrorStatus LL_RTC_EnterInitMode(RTC_TypeDef *RTCx)     /* 源代码 */
{                                                       /* 代码块开始 */
  __IO uint32_t timeout = RTC_INITMODE_TIMEOUT;         /* 赋值或初始化 */
  ErrorStatus status = SUCCESS;                         /* 赋值或初始化 */
  uint32_t tmp = 0U;                                    /* 32位无符号变量/函数 */

  /* Check the parameter */
  assert_param(IS_RTC_ALL_INSTANCE(RTCx));              /* 执行语句 */

  /* Check if the Initialization mode is set */
  if (LL_RTC_IsActiveFlag_INIT(RTCx) == 0U)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Set the Initialization mode */
    LL_RTC_EnableInitMode(RTCx);                        /* 执行语句 */

    /* Wait till RTC is in INIT state and if Time out is reached exit */
    tmp = LL_RTC_IsActiveFlag_INIT(RTCx);               /* 赋值或初始化 */
    while ((timeout != 0U) && (tmp != 1U))              /* while循环 */
    {                                                   /* 代码块开始 */
      if (LL_SYSTICK_IsActiveCounterFlag() == 1U)       /* 条件判断 */
      {                                                 /* 代码块开始 */
        timeout --;                                     /* 执行语句 */
      }                                                 /* 代码块结束 */
      tmp = LL_RTC_IsActiveFlag_INIT(RTCx);             /* 赋值或初始化 */
      if (timeout == 0U)                                /* 条件判断 */
      {                                                 /* 代码块开始 */
        status = ERROR;                                 /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Exit the RTC Initialization mode.
  * @note   When the initialization sequence is complete, the calendar restarts
  *         counting after 4 RTCCLK cycles.
  * @note   The RTC Initialization mode is write protected, use the
  *         @ref LL_RTC_DisableWriteProtection before calling this function.
  * @param  RTCx RTC Instance
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: RTC exited from in Init mode
  *          - ERROR: Not applicable
  */
ErrorStatus LL_RTC_ExitInitMode(RTC_TypeDef *RTCx)      /* 源代码 */
{                                                       /* 代码块开始 */
  /* Check the parameter */
  assert_param(IS_RTC_ALL_INSTANCE(RTCx));              /* 执行语句 */

  /* Disable initialization mode */
  LL_RTC_DisableInitMode(RTCx);                         /* 执行语句 */

  return SUCCESS;                                       /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Waits until the RTC Time and Day registers (RTC_TR and RTC_DR) are
  *         synchronized with RTC APB clock.
  * @note   The RTC Resynchronization mode is write protected, use the
  *         @ref LL_RTC_DisableWriteProtection before calling this function.
  * @note   To read the calendar through the shadow registers after Calendar
  *         initialization, calendar update or after wakeup from low power modes
  *         the software must first clear the RSF flag.
  *         The software must then wait until it is set again before reading
  *         the calendar, which means that the calendar registers have been
  *         correctly copied into the RTC_TR and RTC_DR shadow registers.
  * @param  RTCx RTC Instance
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: RTC registers are synchronised
  *          - ERROR: RTC registers are not synchronised
  */
ErrorStatus LL_RTC_WaitForSynchro(RTC_TypeDef *RTCx)    /* 源代码 */
{                                                       /* 代码块开始 */
  __IO uint32_t timeout = RTC_SYNCHRO_TIMEOUT;          /* 赋值或初始化 */
  ErrorStatus status = SUCCESS;                         /* 赋值或初始化 */
  uint32_t tmp = 0U;                                    /* 32位无符号变量/函数 */

  /* Check the parameter */
  assert_param(IS_RTC_ALL_INSTANCE(RTCx));              /* 执行语句 */

  /* Clear RSF flag */
  LL_RTC_ClearFlag_RS(RTCx);                            /* 执行语句 */

  /* Wait the registers to be synchronised */
  tmp = LL_RTC_IsActiveFlag_RS(RTCx);                   /* 赋值或初始化 */
  while ((timeout != 0U) && (tmp != 0U))                /* while循环 */
  {                                                     /* 代码块开始 */
    if (LL_SYSTICK_IsActiveCounterFlag() == 1U)         /* 条件判断 */
    {                                                   /* 代码块开始 */
      timeout--;                                        /* 执行语句 */
    }                                                   /* 代码块结束 */
    tmp = LL_RTC_IsActiveFlag_RS(RTCx);                 /* 赋值或初始化 */
    if (timeout == 0U)                                  /* 条件判断 */
    {                                                   /* 代码块开始 */
      status = ERROR;                                   /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  if (status != ERROR)                                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    timeout = RTC_SYNCHRO_TIMEOUT;                      /* 赋值或初始化 */
    tmp = LL_RTC_IsActiveFlag_RS(RTCx);                 /* 赋值或初始化 */
    while ((timeout != 0U) && (tmp != 1U))              /* while循环 */
    {                                                   /* 代码块开始 */
      if (LL_SYSTICK_IsActiveCounterFlag() == 1U)       /* 条件判断 */
      {                                                 /* 代码块开始 */
        timeout--;                                      /* 执行语句 */
      }                                                 /* 代码块结束 */
      tmp = LL_RTC_IsActiveFlag_RS(RTCx);               /* 赋值或初始化 */
      if (timeout == 0U)                                /* 条件判断 */
      {                                                 /* 代码块开始 */
        status = ERROR;                                 /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  return (status);                                      /* 返回值 */
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

#endif /* defined(RTC) */

/**
  * @}
  */

#endif /* USE_FULL_LL_DRIVER */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
