/* 文件: stm32f4xx_ll_utils.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_ll_utils.c
  * @author  MCD Application Team
  * @brief   UTILS LL module driver.
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
#include "stm32f4xx_ll_utils.h"                         /* 包含 STM32/HAL 头文件 */
#include "stm32f4xx_ll_rcc.h"                           /* 包含 STM32/HAL 头文件 */
#include "stm32f4xx_ll_system.h"                        /* 包含 STM32/HAL 头文件 */
#include "stm32f4xx_ll_pwr.h"                           /* 包含 STM32/HAL 头文件 */
#ifdef  USE_FULL_ASSERT                                 /* 条件编译 */
#include "stm32_assert.h"                               /* 包含 STM32/HAL 头文件 */
#else                                                   /* 条件编译否则 */
#define assert_param(expr) ((void)0U)                   /* 定义宏 */
#endif /* USE_FULL_ASSERT */

/** @addtogroup STM32F4xx_LL_Driver
  * @{
  */

/** @addtogroup UTILS_LL
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/** @addtogroup UTILS_LL_Private_Constants
  * @{
  */
#if defined(RCC_MAX_FREQUENCY_SCALE1)                   /* 条件编译 */
#define UTILS_MAX_FREQUENCY_SCALE1  RCC_MAX_FREQUENCY           /*!< Maximum frequency for system clock at power scale1, in Hz */
#endif /*RCC_MAX_FREQUENCY_SCALE1 */
#define UTILS_MAX_FREQUENCY_SCALE2  RCC_MAX_FREQUENCY_SCALE2    /*!< Maximum frequency for system clock at power scale2, in Hz */
#if defined(RCC_MAX_FREQUENCY_SCALE3)                   /* 条件编译 */
#define UTILS_MAX_FREQUENCY_SCALE3  RCC_MAX_FREQUENCY_SCALE3    /*!< Maximum frequency for system clock at power scale3, in Hz */
#endif /* MAX_FREQUENCY_SCALE3 */

/* Defines used for PLL range */
#define UTILS_PLLVCO_INPUT_MIN      RCC_PLLVCO_INPUT_MIN        /*!< Frequency min for PLLVCO input, in Hz   */
#define UTILS_PLLVCO_INPUT_MAX      RCC_PLLVCO_INPUT_MAX        /*!< Frequency max for PLLVCO input, in Hz   */
#define UTILS_PLLVCO_OUTPUT_MIN     RCC_PLLVCO_OUTPUT_MIN       /*!< Frequency min for PLLVCO output, in Hz  */
#define UTILS_PLLVCO_OUTPUT_MAX     RCC_PLLVCO_OUTPUT_MAX       /*!< Frequency max for PLLVCO output, in Hz  */

/* Defines used for HSE range */
#define UTILS_HSE_FREQUENCY_MIN      4000000U        /*!< Frequency min for HSE frequency, in Hz   */
#define UTILS_HSE_FREQUENCY_MAX     26000000U        /*!< Frequency max for HSE frequency, in Hz   */

/* Defines used for FLASH latency according to HCLK Frequency */
#if defined(FLASH_SCALE1_LATENCY1_FREQ)                 /* 条件编译 */
#define UTILS_SCALE1_LATENCY1_FREQ  FLASH_SCALE1_LATENCY1_FREQ /*!< HCLK frequency to set FLASH latency 1 in power scale 1 */
#endif                                                  /* 条件编译结束 */
#if defined(FLASH_SCALE1_LATENCY2_FREQ)                 /* 条件编译 */
#define UTILS_SCALE1_LATENCY2_FREQ  FLASH_SCALE1_LATENCY2_FREQ /*!< HCLK frequency to set FLASH latency 2 in power scale 1 */
#endif                                                  /* 条件编译结束 */
#if defined(FLASH_SCALE1_LATENCY3_FREQ)                 /* 条件编译 */
#define UTILS_SCALE1_LATENCY3_FREQ  FLASH_SCALE1_LATENCY3_FREQ /*!< HCLK frequency to set FLASH latency 3 in power scale 1 */
#endif                                                  /* 条件编译结束 */
#if defined(FLASH_SCALE1_LATENCY4_FREQ)                 /* 条件编译 */
#define UTILS_SCALE1_LATENCY4_FREQ  FLASH_SCALE1_LATENCY4_FREQ /*!< HCLK frequency to set FLASH latency 4 in power scale 1 */
#endif                                                  /* 条件编译结束 */
#if defined(FLASH_SCALE1_LATENCY5_FREQ)                 /* 条件编译 */
#define UTILS_SCALE1_LATENCY5_FREQ  FLASH_SCALE1_LATENCY5_FREQ /*!< HCLK frequency to set FLASH latency 5 in power scale 1 */
#endif                                                  /* 条件编译结束 */
#define UTILS_SCALE2_LATENCY1_FREQ  FLASH_SCALE2_LATENCY1_FREQ /*!< HCLK frequency to set FLASH latency 1 in power scale 2 */
#define UTILS_SCALE2_LATENCY2_FREQ  FLASH_SCALE2_LATENCY2_FREQ /*!< HCLK frequency to set FLASH latency 2 in power scale 2 */
#if defined(FLASH_SCALE2_LATENCY3_FREQ)                 /* 条件编译 */
#define UTILS_SCALE2_LATENCY3_FREQ  FLASH_SCALE2_LATENCY3_FREQ /*!< HCLK frequency to set FLASH latency 2 in power scale 2 */
#endif                                                  /* 条件编译结束 */
#if defined(FLASH_SCALE2_LATENCY4_FREQ)                 /* 条件编译 */
#define UTILS_SCALE2_LATENCY4_FREQ  FLASH_SCALE2_LATENCY4_FREQ /*!< HCLK frequency to set FLASH latency 4 in power scale 2 */
#endif                                                  /* 条件编译结束 */
#if defined(FLASH_SCALE2_LATENCY5_FREQ)                 /* 条件编译 */
#define UTILS_SCALE2_LATENCY5_FREQ  FLASH_SCALE2_LATENCY5_FREQ /*!< HCLK frequency to set FLASH latency 5 in power scale 2 */
#endif                                                  /* 条件编译结束 */
#if defined(FLASH_SCALE3_LATENCY1_FREQ)                 /* 条件编译 */
#define UTILS_SCALE3_LATENCY1_FREQ  FLASH_SCALE3_LATENCY1_FREQ /*!< HCLK frequency to set FLASH latency 1 in power scale 3 */
#endif                                                  /* 条件编译结束 */
#if defined(FLASH_SCALE3_LATENCY2_FREQ)                 /* 条件编译 */
#define UTILS_SCALE3_LATENCY2_FREQ  FLASH_SCALE3_LATENCY2_FREQ /*!< HCLK frequency to set FLASH latency 2 in power scale 3 */
#endif                                                  /* 条件编译结束 */
#if defined(FLASH_SCALE3_LATENCY3_FREQ)                 /* 条件编译 */
#define UTILS_SCALE3_LATENCY3_FREQ  FLASH_SCALE3_LATENCY3_FREQ /*!< HCLK frequency to set FLASH latency 3 in power scale 3 */
#endif                                                  /* 条件编译结束 */
#if defined(FLASH_SCALE3_LATENCY4_FREQ)                 /* 条件编译 */
#define UTILS_SCALE3_LATENCY4_FREQ  FLASH_SCALE3_LATENCY4_FREQ /*!< HCLK frequency to set FLASH latency 4 in power scale 3 */
#endif                                                  /* 条件编译结束 */
#if defined(FLASH_SCALE3_LATENCY5_FREQ)                 /* 条件编译 */
#define UTILS_SCALE3_LATENCY5_FREQ  FLASH_SCALE3_LATENCY5_FREQ /*!< HCLK frequency to set FLASH latency 5 in power scale 3 */
#endif                                                  /* 条件编译结束 */
/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/** @addtogroup UTILS_LL_Private_Macros
  * @{
  */
#define IS_LL_UTILS_SYSCLK_DIV(__VALUE__) (((__VALUE__) == LL_RCC_SYSCLK_DIV_1)   \
                                        || ((__VALUE__) == LL_RCC_SYSCLK_DIV_2)   \
                                        || ((__VALUE__) == LL_RCC_SYSCLK_DIV_4)   \
                                        || ((__VALUE__) == LL_RCC_SYSCLK_DIV_8)   \
                                        || ((__VALUE__) == LL_RCC_SYSCLK_DIV_16)  \
                                        || ((__VALUE__) == LL_RCC_SYSCLK_DIV_64)  \
                                        || ((__VALUE__) == LL_RCC_SYSCLK_DIV_128) \
                                        || ((__VALUE__) == LL_RCC_SYSCLK_DIV_256) \
                                        || ((__VALUE__) == LL_RCC_SYSCLK_DIV_512))  /* 源代码 */

#define IS_LL_UTILS_APB1_DIV(__VALUE__) (((__VALUE__) == LL_RCC_APB1_DIV_1) \
                                      || ((__VALUE__) == LL_RCC_APB1_DIV_2) \
                                      || ((__VALUE__) == LL_RCC_APB1_DIV_4) \
                                      || ((__VALUE__) == LL_RCC_APB1_DIV_8) \
                                      || ((__VALUE__) == LL_RCC_APB1_DIV_16))  /* 源代码 */

#define IS_LL_UTILS_APB2_DIV(__VALUE__) (((__VALUE__) == LL_RCC_APB2_DIV_1) \
                                      || ((__VALUE__) == LL_RCC_APB2_DIV_2) \
                                      || ((__VALUE__) == LL_RCC_APB2_DIV_4) \
                                      || ((__VALUE__) == LL_RCC_APB2_DIV_8) \
                                      || ((__VALUE__) == LL_RCC_APB2_DIV_16))  /* 源代码 */

#define IS_LL_UTILS_PLLM_VALUE(__VALUE__) (((__VALUE__) == LL_RCC_PLLM_DIV_2)  \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_3)  \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_4)  \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_5)  \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_6)  \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_7)  \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_8)  \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_9)  \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_10) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_11) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_12) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_13) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_14) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_15) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_16) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_17) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_18) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_19) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_20) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_21) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_22) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_23) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_24) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_25) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_26) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_27) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_28) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_29) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_30) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_31) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_32) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_33) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_34) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_35) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_36) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_37) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_38) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_39) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_40) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_41) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_42) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_43) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_44) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_45) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_46) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_47) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_48) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_49) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_50) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_51) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_52) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_53) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_54) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_55) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_56) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_57) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_58) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_59) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_60) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_61) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_62) \
                                        || ((__VALUE__) == LL_RCC_PLLM_DIV_63))  /* 源代码 */

#define IS_LL_UTILS_PLLN_VALUE(__VALUE__) ((RCC_PLLN_MIN_VALUE <= (__VALUE__)) && ((__VALUE__) <= RCC_PLLN_MAX_VALUE))  /* 定义宏 */

#define IS_LL_UTILS_PLLP_VALUE(__VALUE__) (((__VALUE__) == LL_RCC_PLLP_DIV_2) \
                                        || ((__VALUE__) == LL_RCC_PLLP_DIV_4) \
                                        || ((__VALUE__) == LL_RCC_PLLP_DIV_6) \
                                        || ((__VALUE__) == LL_RCC_PLLP_DIV_8))  /* 源代码 */

#define IS_LL_UTILS_PLLVCO_INPUT(__VALUE__)  ((UTILS_PLLVCO_INPUT_MIN <= (__VALUE__)) && ((__VALUE__) <= UTILS_PLLVCO_INPUT_MAX))  /* 定义宏 */

#define IS_LL_UTILS_PLLVCO_OUTPUT(__VALUE__) ((UTILS_PLLVCO_OUTPUT_MIN <= (__VALUE__)) && ((__VALUE__) <= UTILS_PLLVCO_OUTPUT_MAX))  /* 定义宏 */

#if !defined(RCC_MAX_FREQUENCY_SCALE1)                  /* 条件编译 */
#define IS_LL_UTILS_PLL_FREQUENCY(__VALUE__) ((LL_PWR_GetRegulVoltageScaling() == LL_PWR_REGU_VOLTAGE_SCALE2) ? ((__VALUE__) <= UTILS_MAX_FREQUENCY_SCALE2) : \
                                             ((__VALUE__) <= UTILS_MAX_FREQUENCY_SCALE3))  /* 源代码 */

#elif defined(RCC_MAX_FREQUENCY_SCALE3)                 /* 条件编译否则如果 */
#define IS_LL_UTILS_PLL_FREQUENCY(__VALUE__) ((LL_PWR_GetRegulVoltageScaling() == LL_PWR_REGU_VOLTAGE_SCALE1) ? ((__VALUE__) <= UTILS_MAX_FREQUENCY_SCALE1) : \
                                              (LL_PWR_GetRegulVoltageScaling() == LL_PWR_REGU_VOLTAGE_SCALE2) ? ((__VALUE__) <= UTILS_MAX_FREQUENCY_SCALE2) : \
                                              ((__VALUE__) <= UTILS_MAX_FREQUENCY_SCALE3))  /* 源代码 */

#else                                                   /* 条件编译否则 */
#define IS_LL_UTILS_PLL_FREQUENCY(__VALUE__) ((LL_PWR_GetRegulVoltageScaling() == LL_PWR_REGU_VOLTAGE_SCALE1) ? ((__VALUE__) <= UTILS_MAX_FREQUENCY_SCALE1) : \
                                             ((__VALUE__) <= UTILS_MAX_FREQUENCY_SCALE2))  /* 源代码 */

#endif /* RCC_MAX_FREQUENCY_SCALE1*/
#define IS_LL_UTILS_HSE_BYPASS(__STATE__) (((__STATE__) == LL_UTILS_HSEBYPASS_ON) \
                                        || ((__STATE__) == LL_UTILS_HSEBYPASS_OFF))  /* 源代码 */

#define IS_LL_UTILS_HSE_FREQUENCY(__FREQUENCY__) (((__FREQUENCY__) >= UTILS_HSE_FREQUENCY_MIN) && ((__FREQUENCY__) <= UTILS_HSE_FREQUENCY_MAX))  /* 定义宏 */
/**
  * @}
  */
/* Private function prototypes -----------------------------------------------*/
/** @defgroup UTILS_LL_Private_Functions UTILS Private functions
  * @{
  */
static uint32_t    UTILS_GetPLLOutputFrequency(uint32_t PLL_InputFrequency,  /* 静态变量/函数 */
                                               LL_UTILS_PLLInitTypeDef *UTILS_PLLInitStruct);  /* 执行语句 */
static ErrorStatus UTILS_EnablePLLAndSwitchSystem(uint32_t SYSCLK_Frequency, LL_UTILS_ClkInitTypeDef *UTILS_ClkInitStruct);  /* 静态变量/函数 */
static ErrorStatus UTILS_PLL_IsBusy(void);              /* 静态变量/函数 */
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup UTILS_LL_Exported_Functions
  * @{
  */

/** @addtogroup UTILS_LL_EF_DELAY
  * @{
  */

/**
  * @brief  This function configures the Cortex-M SysTick source to have 1ms time base.
  * @note   When a RTOS is used, it is recommended to avoid changing the Systick
  *         configuration by calling this function, for a delay use rather osDelay RTOS service.
  * @param  HCLKFrequency HCLK frequency in Hz
  * @note   HCLK frequency can be calculated thanks to RCC helper macro or function @ref LL_RCC_GetSystemClocksFreq
  * @retval None
  */
void LL_Init1msTick(uint32_t HCLKFrequency)             /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Use frequency provided in argument */
  LL_InitTick(HCLKFrequency, 1000U);                    /* 执行语句 */
}                                                       /* 代码块结束 */

/**
  * @brief  This function provides accurate delay (in milliseconds) based
  *         on SysTick counter flag
  * @note   When a RTOS is used, it is recommended to avoid using blocking delay
  *         and use rather osDelay service.
  * @note   To respect 1ms timebase, user should call @ref LL_Init1msTick function which
  *         will configure Systick to 1ms
  * @param  Delay specifies the delay time length, in milliseconds.
  * @retval None
  */
void LL_mDelay(uint32_t Delay)                          /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  __IO uint32_t  tmp = SysTick->CTRL;  /* Clear the COUNTFLAG first */
  /* Add this code to indicate that local variable is not used */
  ((void)tmp);                                          /* 执行语句 */

  /* Add a period to guaranty minimum wait */
  if(Delay < LL_MAX_DELAY)                              /* 条件判断 */
  {                                                     /* 代码块开始 */
    Delay++;                                            /* 执行语句 */
  }                                                     /* 代码块结束 */

  while (Delay)                                         /* while循环 */
  {                                                     /* 代码块开始 */
    if((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) != 0U)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      Delay--;                                          /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @addtogroup UTILS_EF_SYSTEM
  *  @brief    System Configuration functions
  *
  @verbatim
 ===============================================================================
           ##### System Configuration functions #####
 ===============================================================================
    [..]
         System, AHB and APB buses clocks configuration

         (+) The maximum frequency of the SYSCLK, HCLK, PCLK1 and PCLK2 is 180000000 Hz.
  @endverbatim
  @internal
             Depending on the device voltage range, the maximum frequency should be
             adapted accordingly to the Refenece manual.
  @endinternal
  * @{
  */

/**
  * @brief  This function sets directly SystemCoreClock CMSIS variable.
  * @note   Variable can be calculated also through SystemCoreClockUpdate function.
  * @param  HCLKFrequency HCLK frequency in Hz (can be calculated thanks to RCC helper macro)
  * @retval None
  */
void LL_SetSystemCoreClock(uint32_t HCLKFrequency)      /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* HCLK clock frequency */
  SystemCoreClock = HCLKFrequency;                      /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  Update number of Flash wait states in line with new frequency and current
            voltage range.
  * @note   This Function support ONLY devices with supply voltage (voltage range) between 2.7V and 3.6V
  * @param  HCLK_Frequency  HCLK frequency
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: Latency has been modified
  *          - ERROR: Latency cannot be modified
  */
ErrorStatus LL_SetFlashLatency(uint32_t HCLK_Frequency)  /* 源代码 */
{                                                       /* 代码块开始 */
  uint32_t timeout;                                     /* 32位无符号变量/函数 */
  uint32_t getlatency;                                  /* 32位无符号变量/函数 */
  uint32_t latency = LL_FLASH_LATENCY_0;  /* default value 0WS */
  ErrorStatus status = SUCCESS;                         /* 赋值或初始化 */


  /* Frequency cannot be equal to 0 */
  if(HCLK_Frequency == 0U)                              /* 条件判断 */
  {                                                     /* 代码块开始 */
    status = ERROR;                                     /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    if(LL_PWR_GetRegulVoltageScaling() == LL_PWR_REGU_VOLTAGE_SCALE1)  /* 条件判断 */
    {                                                   /* 代码块开始 */
#if defined (UTILS_SCALE1_LATENCY5_FREQ)                /* 条件编译 */
      if((HCLK_Frequency > UTILS_SCALE1_LATENCY5_FREQ)&&(latency == LL_FLASH_LATENCY_0))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        latency = LL_FLASH_LATENCY_5;                   /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
#endif /*UTILS_SCALE1_LATENCY5_FREQ */
#if defined (UTILS_SCALE1_LATENCY4_FREQ)                /* 条件编译 */
      if((HCLK_Frequency > UTILS_SCALE1_LATENCY4_FREQ)&&(latency == LL_FLASH_LATENCY_0))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        latency = LL_FLASH_LATENCY_4;                   /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
#endif /* UTILS_SCALE1_LATENCY4_FREQ */
#if defined (UTILS_SCALE1_LATENCY3_FREQ)                /* 条件编译 */
      if((HCLK_Frequency > UTILS_SCALE1_LATENCY3_FREQ)&&(latency == LL_FLASH_LATENCY_0))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        latency = LL_FLASH_LATENCY_3;                   /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
#endif /* UTILS_SCALE1_LATENCY3_FREQ */
#if defined (UTILS_SCALE1_LATENCY2_FREQ)                /* 条件编译 */
      if((HCLK_Frequency > UTILS_SCALE1_LATENCY2_FREQ)&&(latency == LL_FLASH_LATENCY_0))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        latency = LL_FLASH_LATENCY_2;                   /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        if((HCLK_Frequency > UTILS_SCALE1_LATENCY1_FREQ)&&(latency == LL_FLASH_LATENCY_0))  /* 条件判断 */
        {                                               /* 代码块开始 */
          latency = LL_FLASH_LATENCY_1;                 /* 赋值或初始化 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
#endif /* UTILS_SCALE1_LATENCY2_FREQ */
    }                                                   /* 代码块结束 */
    if(LL_PWR_GetRegulVoltageScaling() == LL_PWR_REGU_VOLTAGE_SCALE2)  /* 条件判断 */
    {                                                   /* 代码块开始 */
#if defined (UTILS_SCALE2_LATENCY5_FREQ)                /* 条件编译 */
      if((HCLK_Frequency > UTILS_SCALE2_LATENCY5_FREQ)&&(latency == LL_FLASH_LATENCY_0))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        latency = LL_FLASH_LATENCY_5;                   /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
#endif /*UTILS_SCALE1_LATENCY5_FREQ */
#if defined (UTILS_SCALE2_LATENCY4_FREQ)                /* 条件编译 */
      if((HCLK_Frequency > UTILS_SCALE2_LATENCY4_FREQ)&&(latency == LL_FLASH_LATENCY_0))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        latency = LL_FLASH_LATENCY_4;                   /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
#endif /*UTILS_SCALE1_LATENCY4_FREQ */
#if defined (UTILS_SCALE2_LATENCY3_FREQ)                /* 条件编译 */
      if((HCLK_Frequency > UTILS_SCALE2_LATENCY3_FREQ)&&(latency == LL_FLASH_LATENCY_0))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        latency = LL_FLASH_LATENCY_3;                   /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
#endif /*UTILS_SCALE1_LATENCY3_FREQ */
      if((HCLK_Frequency > UTILS_SCALE2_LATENCY2_FREQ)&&(latency == LL_FLASH_LATENCY_0))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        latency = LL_FLASH_LATENCY_2;                   /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        if((HCLK_Frequency > UTILS_SCALE2_LATENCY1_FREQ)&&(latency == LL_FLASH_LATENCY_0))  /* 条件判断 */
        {                                               /* 代码块开始 */
          latency = LL_FLASH_LATENCY_1;                 /* 赋值或初始化 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
#if defined (LL_PWR_REGU_VOLTAGE_SCALE3)                /* 条件编译 */
    if(LL_PWR_GetRegulVoltageScaling() == LL_PWR_REGU_VOLTAGE_SCALE3)  /* 条件判断 */
    {                                                   /* 代码块开始 */
#if defined (UTILS_SCALE3_LATENCY3_FREQ)                /* 条件编译 */
      if((HCLK_Frequency > UTILS_SCALE3_LATENCY3_FREQ)&&(latency == LL_FLASH_LATENCY_0))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        latency = LL_FLASH_LATENCY_3;                   /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
#endif /*UTILS_SCALE1_LATENCY3_FREQ */
#if defined (UTILS_SCALE3_LATENCY2_FREQ)                /* 条件编译 */
      if((HCLK_Frequency > UTILS_SCALE3_LATENCY2_FREQ)&&(latency == LL_FLASH_LATENCY_0))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        latency = LL_FLASH_LATENCY_2;                   /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        if((HCLK_Frequency > UTILS_SCALE3_LATENCY1_FREQ)&&(latency == LL_FLASH_LATENCY_0))  /* 条件判断 */
        {                                               /* 代码块开始 */
          latency = LL_FLASH_LATENCY_1;                 /* 赋值或初始化 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
#endif /*UTILS_SCALE1_LATENCY2_FREQ */
#endif /* LL_PWR_REGU_VOLTAGE_SCALE3 */

    LL_FLASH_SetLatency(latency);                       /* 执行语句 */
    /* Check that the new number of wait states is taken into account to access the Flash
       memory by reading the FLASH_ACR register */
    timeout = 2;                                        /* 赋值或初始化 */
    do                                                  /* do-while循环 */
    {                                                   /* 代码块开始 */
    /* Wait for Flash latency to be updated */
    getlatency = LL_FLASH_GetLatency();                 /* 赋值或初始化 */
    timeout--;                                          /* 执行语句 */
    } while ((getlatency != latency) && (timeout > 0));  /* 代码块结束 */

    if(getlatency != latency)                           /* 条件判断 */
    {                                                   /* 代码块开始 */
      status = ERROR;                                   /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      status = SUCCESS;                                 /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  This function configures system clock at maximum frequency with HSI as clock source of the PLL
  * @note   The application need to ensure that PLL is disabled.
  * @note   Function is based on the following formula:
  *         - PLL output frequency = (((HSI frequency / PLLM) * PLLN) / PLLP)
  *         - PLLM: ensure that the VCO input frequency ranges from @ref RCC_PLLVCO_INPUT_MIN to @ref RCC_PLLVCO_INPUT_MAX (PLLVCO_input = HSI frequency / PLLM)
  *         - PLLN: ensure that the VCO output frequency is between @ref RCC_PLLVCO_OUTPUT_MIN and @ref RCC_PLLVCO_OUTPUT_MAX (PLLVCO_output = PLLVCO_input * PLLN)
  *         - PLLP: ensure that max frequency at 180000000 Hz is reach (PLLVCO_output / PLLP)
  * @param  UTILS_PLLInitStruct pointer to a @ref LL_UTILS_PLLInitTypeDef structure that contains
  *                             the configuration information for the PLL.
  * @param  UTILS_ClkInitStruct pointer to a @ref LL_UTILS_ClkInitTypeDef structure that contains
  *                             the configuration information for the BUS prescalers.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: Max frequency configuration done
  *          - ERROR: Max frequency configuration not done
  */
ErrorStatus LL_PLL_ConfigSystemClock_HSI(LL_UTILS_PLLInitTypeDef *UTILS_PLLInitStruct,  /* 列表续行 */
                                         LL_UTILS_ClkInitTypeDef *UTILS_ClkInitStruct)  /* 源代码 */
{                                                       /* 代码块开始 */
  ErrorStatus status = SUCCESS;                         /* 赋值或初始化 */
  uint32_t pllfreq = 0U;                                /* 32位无符号变量/函数 */

  /* Check if one of the PLL is enabled */
  if(UTILS_PLL_IsBusy() == SUCCESS)                     /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Calculate the new PLL output frequency */
    pllfreq = UTILS_GetPLLOutputFrequency(HSI_VALUE, UTILS_PLLInitStruct);  /* 赋值或初始化 */

    /* Enable HSI if not enabled */
    if(LL_RCC_HSI_IsReady() != 1U)                      /* 条件判断 */
    {                                                   /* 代码块开始 */
      LL_RCC_HSI_Enable();                              /* 执行语句 */
      while (LL_RCC_HSI_IsReady() != 1U)                /* while循环 */
      {                                                 /* 代码块开始 */
        /* Wait for HSI ready */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Configure PLL */
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, UTILS_PLLInitStruct->PLLM, UTILS_PLLInitStruct->PLLN,  /* 列表续行 */
                                UTILS_PLLInitStruct->PLLP);  /* 执行语句 */

    /* Enable PLL and switch system clock to PLL */
    status = UTILS_EnablePLLAndSwitchSystem(pllfreq, UTILS_ClkInitStruct);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Current PLL configuration cannot be modified */
    status = ERROR;                                     /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  This function configures system clock with HSE as clock source of the PLL
  * @note   The application need to ensure that PLL is disabled.
  *         - PLL output frequency = (((HSI frequency / PLLM) * PLLN) / PLLP)
  *         - PLLM: ensure that the VCO input frequency ranges from @ref RCC_PLLVCO_INPUT_MIN to @ref RCC_PLLVCO_INPUT_MAX (PLLVCO_input = HSI frequency / PLLM)
  *         - PLLN: ensure that the VCO output frequency is between @ref RCC_PLLVCO_OUTPUT_MIN and @ref RCC_PLLVCO_OUTPUT_MAX (PLLVCO_output = PLLVCO_input * PLLN)
  *         - PLLP: ensure that max frequency at 180000000 Hz is reach (PLLVCO_output / PLLP)
  * @param  HSEFrequency Value between Min_Data = 4000000 and Max_Data = 26000000
  * @param  HSEBypass This parameter can be one of the following values:
  *         @arg @ref LL_UTILS_HSEBYPASS_ON
  *         @arg @ref LL_UTILS_HSEBYPASS_OFF
  * @param  UTILS_PLLInitStruct pointer to a @ref LL_UTILS_PLLInitTypeDef structure that contains
  *                             the configuration information for the PLL.
  * @param  UTILS_ClkInitStruct pointer to a @ref LL_UTILS_ClkInitTypeDef structure that contains
  *                             the configuration information for the BUS prescalers.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: Max frequency configuration done
  *          - ERROR: Max frequency configuration not done
  */
ErrorStatus LL_PLL_ConfigSystemClock_HSE(uint32_t HSEFrequency, uint32_t HSEBypass,  /* 列表续行 */
                                         LL_UTILS_PLLInitTypeDef *UTILS_PLLInitStruct, LL_UTILS_ClkInitTypeDef *UTILS_ClkInitStruct)  /* 源代码 */
{                                                       /* 代码块开始 */
  ErrorStatus status = SUCCESS;                         /* 赋值或初始化 */
  uint32_t pllfreq = 0U;                                /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_LL_UTILS_HSE_FREQUENCY(HSEFrequency));  /* 执行语句 */
  assert_param(IS_LL_UTILS_HSE_BYPASS(HSEBypass));      /* 执行语句 */

  /* Check if one of the PLL is enabled */
  if(UTILS_PLL_IsBusy() == SUCCESS)                     /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Calculate the new PLL output frequency */
    pllfreq = UTILS_GetPLLOutputFrequency(HSEFrequency, UTILS_PLLInitStruct);  /* 赋值或初始化 */

    /* Enable HSE if not enabled */
    if(LL_RCC_HSE_IsReady() != 1U)                      /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Check if need to enable HSE bypass feature or not */
      if(HSEBypass == LL_UTILS_HSEBYPASS_ON)            /* 条件判断 */
      {                                                 /* 代码块开始 */
        LL_RCC_HSE_EnableBypass();                      /* 执行语句 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        LL_RCC_HSE_DisableBypass();                     /* 执行语句 */
      }                                                 /* 代码块结束 */

      /* Enable HSE */
      LL_RCC_HSE_Enable();                              /* 执行语句 */
      while (LL_RCC_HSE_IsReady() != 1U)                /* while循环 */
      {                                                 /* 代码块开始 */
        /* Wait for HSE ready */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Configure PLL */
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, UTILS_PLLInitStruct->PLLM, UTILS_PLLInitStruct->PLLN,  /* 列表续行 */
                                UTILS_PLLInitStruct->PLLP);  /* 执行语句 */

    /* Enable PLL and switch system clock to PLL */
    status = UTILS_EnablePLLAndSwitchSystem(pllfreq, UTILS_ClkInitStruct);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Current PLL configuration cannot be modified */
    status = ERROR;                                     /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup UTILS_LL_Private_Functions
  * @{
  */
/**
  * @brief  Function to check that PLL can be modified
  * @param  PLL_InputFrequency  PLL input frequency (in Hz)
  * @param  UTILS_PLLInitStruct pointer to a @ref LL_UTILS_PLLInitTypeDef structure that contains
  *                             the configuration information for the PLL.
  * @retval PLL output frequency (in Hz)
  */
static uint32_t UTILS_GetPLLOutputFrequency(uint32_t PLL_InputFrequency, LL_UTILS_PLLInitTypeDef *UTILS_PLLInitStruct)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t pllfreq = 0U;                                /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_LL_UTILS_PLLM_VALUE(UTILS_PLLInitStruct->PLLM));  /* 执行语句 */
  assert_param(IS_LL_UTILS_PLLN_VALUE(UTILS_PLLInitStruct->PLLN));  /* 执行语句 */
  assert_param(IS_LL_UTILS_PLLP_VALUE(UTILS_PLLInitStruct->PLLP));  /* 执行语句 */

  /* Check different PLL parameters according to RM                          */
  /*  - PLLM: ensure that the VCO input frequency ranges from @ref UTILS_PLLVCO_INPUT_MIN to @ref UTILS_PLLVCO_INPUT_MAX MHz.   */
  pllfreq = PLL_InputFrequency / (UTILS_PLLInitStruct->PLLM & (RCC_PLLCFGR_PLLM >> RCC_PLLCFGR_PLLM_Pos));  /* 赋值或初始化 */
  assert_param(IS_LL_UTILS_PLLVCO_INPUT(pllfreq));      /* 执行语句 */

  /*  - PLLN: ensure that the VCO output frequency is between @ref UTILS_PLLVCO_OUTPUT_MIN and @ref UTILS_PLLVCO_OUTPUT_MAX .*/
  pllfreq = pllfreq * (UTILS_PLLInitStruct->PLLN & (RCC_PLLCFGR_PLLN >> RCC_PLLCFGR_PLLN_Pos));  /* 赋值或初始化 */
  assert_param(IS_LL_UTILS_PLLVCO_OUTPUT(pllfreq));     /* 执行语句 */

  /*  - PLLP: ensure that max frequency at @ref RCC_MAX_FREQUENCY Hz is reached     */
  pllfreq = pllfreq / (((UTILS_PLLInitStruct->PLLP >> RCC_PLLCFGR_PLLP_Pos) + 1) * 2);  /* 赋值或初始化 */
  assert_param(IS_LL_UTILS_PLL_FREQUENCY(pllfreq));     /* 执行语句 */

  return pllfreq;                                       /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Function to check that PLL can be modified
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: PLL modification can be done
  *          - ERROR: PLL is busy
  */
static ErrorStatus UTILS_PLL_IsBusy(void)               /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  ErrorStatus status = SUCCESS;                         /* 赋值或初始化 */

  /* Check if PLL is busy*/
  if(LL_RCC_PLL_IsReady() != 0U)                        /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* PLL configuration cannot be modified */
    status = ERROR;                                     /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

#if defined(RCC_PLLSAI_SUPPORT)                         /* 条件编译 */
  /* Check if PLLSAI is busy*/
  if(LL_RCC_PLLSAI_IsReady() != 0U)                     /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* PLLSAI1 configuration cannot be modified */
    status = ERROR;                                     /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
#endif /*RCC_PLLSAI_SUPPORT*/
#if defined(RCC_PLLI2S_SUPPORT)                         /* 条件编译 */
  /* Check if PLLI2S is busy*/
  if(LL_RCC_PLLI2S_IsReady() != 0U)                     /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* PLLI2S configuration cannot be modified */
    status = ERROR;                                     /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
#endif /*RCC_PLLI2S_SUPPORT*/
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Function to enable PLL and switch system clock to PLL
  * @param  SYSCLK_Frequency SYSCLK frequency
  * @param  UTILS_ClkInitStruct pointer to a @ref LL_UTILS_ClkInitTypeDef structure that contains
  *                             the configuration information for the BUS prescalers.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: No problem to switch system to PLL
  *          - ERROR: Problem to switch system to PLL
  */
static ErrorStatus UTILS_EnablePLLAndSwitchSystem(uint32_t SYSCLK_Frequency, LL_UTILS_ClkInitTypeDef *UTILS_ClkInitStruct)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  ErrorStatus status = SUCCESS;                         /* 赋值或初始化 */
  uint32_t hclk_frequency = 0U;                         /* 32位无符号变量/函数 */

  assert_param(IS_LL_UTILS_SYSCLK_DIV(UTILS_ClkInitStruct->AHBCLKDivider));  /* 执行语句 */
  assert_param(IS_LL_UTILS_APB1_DIV(UTILS_ClkInitStruct->APB1CLKDivider));  /* 执行语句 */
  assert_param(IS_LL_UTILS_APB2_DIV(UTILS_ClkInitStruct->APB2CLKDivider));  /* 执行语句 */

  /* Calculate HCLK frequency */
  hclk_frequency = __LL_RCC_CALC_HCLK_FREQ(SYSCLK_Frequency, UTILS_ClkInitStruct->AHBCLKDivider);  /* 赋值或初始化 */

  /* Increasing the number of wait states because of higher CPU frequency */
  if(SystemCoreClock < hclk_frequency)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Set FLASH latency to highest latency */
    status = LL_SetFlashLatency(hclk_frequency);        /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Update system clock configuration */
  if(status == SUCCESS)                                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable PLL */
    LL_RCC_PLL_Enable();                                /* 执行语句 */
    while (LL_RCC_PLL_IsReady() != 1U)                  /* while循环 */
    {                                                   /* 代码块开始 */
      /* Wait for PLL ready */
    }                                                   /* 代码块结束 */

    /* Sysclk activation on the main PLL */
    LL_RCC_SetAHBPrescaler(UTILS_ClkInitStruct->AHBCLKDivider);  /* 执行语句 */
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);   /* 执行语句 */
    while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)  /* while循环 */
    {                                                   /* 代码块开始 */
      /* Wait for system clock switch to PLL */
    }                                                   /* 代码块结束 */

    /* Set APB1 & APB2 prescaler*/
    LL_RCC_SetAPB1Prescaler(UTILS_ClkInitStruct->APB1CLKDivider);  /* 执行语句 */
    LL_RCC_SetAPB2Prescaler(UTILS_ClkInitStruct->APB2CLKDivider);  /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Decreasing the number of wait states because of lower CPU frequency */
  if(SystemCoreClock > hclk_frequency)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Set FLASH latency to lowest latency */
    status = LL_SetFlashLatency(hclk_frequency);        /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Update SystemCoreClock variable */
  if(status == SUCCESS)                                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    LL_SetSystemCoreClock(hclk_frequency);              /* 执行语句 */
  }                                                     /* 代码块结束 */

  return status;                                        /* 返回值 */
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
