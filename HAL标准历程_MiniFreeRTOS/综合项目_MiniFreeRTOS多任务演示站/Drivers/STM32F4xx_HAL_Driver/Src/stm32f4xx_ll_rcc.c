/* 文件: stm32f4xx_ll_rcc.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_ll_rcc.c
  * @author  MCD Application Team
  * @brief   RCC LL module driver.
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
#include "stm32f4xx_ll_rcc.h"                           /* 包含 STM32/HAL 头文件 */
#ifdef  USE_FULL_ASSERT                                 /* 条件编译 */
  #include "stm32_assert.h"                             /* 包含 STM32/HAL 头文件 */
#else                                                   /* 条件编译否则 */
  #define assert_param(expr) ((void)0U)                 /* 定义宏 */
#endif                                                  /* 条件编译结束 */
/** @addtogroup STM32F4xx_LL_Driver
  * @{
  */

#if defined(RCC)                                        /* 条件编译 */

/** @addtogroup RCC_LL
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/** @addtogroup RCC_LL_Private_Macros
  * @{
  */
#if defined(FMPI2C1)                                    /* 条件编译 */
#define IS_LL_RCC_FMPI2C_CLKSOURCE(__VALUE__)     ((__VALUE__) == LL_RCC_FMPI2C1_CLKSOURCE)  /* 定义宏 */
#endif /* FMPI2C1 */

#if defined(LPTIM1)                                     /* 条件编译 */
#define IS_LL_RCC_LPTIM_CLKSOURCE(__VALUE__)  (((__VALUE__) == LL_RCC_LPTIM1_CLKSOURCE))  /* 定义宏 */
#endif /* LPTIM1 */

#if defined(SAI1)                                       /* 条件编译 */
#if defined(RCC_DCKCFGR_SAI1SRC)                        /* 条件编译 */
#define IS_LL_RCC_SAI_CLKSOURCE(__VALUE__)    (((__VALUE__) == LL_RCC_SAI1_CLKSOURCE) \
                                            || ((__VALUE__) == LL_RCC_SAI2_CLKSOURCE))  /* 源代码 */
#elif defined(RCC_DCKCFGR_SAI1ASRC)                     /* 条件编译否则如果 */
#define IS_LL_RCC_SAI_CLKSOURCE(__VALUE__)    (((__VALUE__) == LL_RCC_SAI1_A_CLKSOURCE) \
                                            || ((__VALUE__) == LL_RCC_SAI1_B_CLKSOURCE))  /* 源代码 */
#endif /* RCC_DCKCFGR_SAI1SRC */
#endif /* SAI1 */

#if defined(SDIO)                                       /* 条件编译 */
#define IS_LL_RCC_SDIO_CLKSOURCE(__VALUE__)  (((__VALUE__) == LL_RCC_SDIO_CLKSOURCE))  /* 定义宏 */
#endif /* SDIO */

#if defined(RNG)                                        /* 条件编译 */
#define IS_LL_RCC_RNG_CLKSOURCE(__VALUE__)    (((__VALUE__) == LL_RCC_RNG_CLKSOURCE))  /* 定义宏 */
#endif /* RNG */

#if defined(USB_OTG_FS) || defined(USB_OTG_HS)          /* 条件编译 */
#define IS_LL_RCC_USB_CLKSOURCE(__VALUE__)    (((__VALUE__) == LL_RCC_USB_CLKSOURCE))  /* 定义宏 */
#endif /* USB_OTG_FS || USB_OTG_HS */

#if defined(DFSDM2_Channel0)                            /* 条件编译 */
#define IS_LL_RCC_DFSDM_CLKSOURCE(__VALUE__)    (((__VALUE__) == LL_RCC_DFSDM1_CLKSOURCE))  /* 定义宏 */

#define IS_LL_RCC_DFSDM_AUDIO_CLKSOURCE(__VALUE__)    (((__VALUE__) == LL_RCC_DFSDM1_AUDIO_CLKSOURCE) \
                                                    || ((__VALUE__) == LL_RCC_DFSDM2_AUDIO_CLKSOURCE))  /* 源代码 */
#elif defined(DFSDM1_Channel0)                          /* 条件编译否则如果 */
#define IS_LL_RCC_DFSDM_CLKSOURCE(__VALUE__)    (((__VALUE__) == LL_RCC_DFSDM1_CLKSOURCE))  /* 定义宏 */

#define IS_LL_RCC_DFSDM_AUDIO_CLKSOURCE(__VALUE__)    (((__VALUE__) == LL_RCC_DFSDM1_AUDIO_CLKSOURCE))  /* 定义宏 */
#endif /* DFSDM2_Channel0 */

#if defined(RCC_DCKCFGR_I2S2SRC)                        /* 条件编译 */
#define IS_LL_RCC_I2S_CLKSOURCE(__VALUE__)    (((__VALUE__) == LL_RCC_I2S1_CLKSOURCE) \
                                            || ((__VALUE__) == LL_RCC_I2S2_CLKSOURCE))  /* 源代码 */
#else                                                   /* 条件编译否则 */
#define IS_LL_RCC_I2S_CLKSOURCE(__VALUE__)    (((__VALUE__) == LL_RCC_I2S1_CLKSOURCE))  /* 定义宏 */
#endif /* RCC_DCKCFGR_I2S2SRC */

#if defined(CEC)                                        /* 条件编译 */
#define IS_LL_RCC_CEC_CLKSOURCE(__VALUE__)    (((__VALUE__) == LL_RCC_CEC_CLKSOURCE))  /* 定义宏 */
#endif /* CEC */

#if defined(DSI)                                        /* 条件编译 */
#define IS_LL_RCC_DSI_CLKSOURCE(__VALUE__)    (((__VALUE__) == LL_RCC_DSI_CLKSOURCE))  /* 定义宏 */
#endif /* DSI */

#if defined(LTDC)                                       /* 条件编译 */
#define IS_LL_RCC_LTDC_CLKSOURCE(__VALUE__)    (((__VALUE__) == LL_RCC_LTDC_CLKSOURCE))  /* 定义宏 */
#endif /* LTDC */

#if defined(SPDIFRX)                                    /* 条件编译 */
#define IS_LL_RCC_SPDIFRX_CLKSOURCE(__VALUE__)    (((__VALUE__) == LL_RCC_SPDIFRX1_CLKSOURCE))  /* 定义宏 */
#endif /* SPDIFRX */
/**
  * @}
  */

/* Private function prototypes -----------------------------------------------*/
/** @defgroup RCC_LL_Private_Functions RCC Private functions
  * @{
  */
uint32_t RCC_GetSystemClockFreq(void);                  /* 32位无符号变量/函数 */
uint32_t RCC_GetHCLKClockFreq(uint32_t SYSCLK_Frequency);  /* 32位无符号变量/函数 */
uint32_t RCC_GetPCLK1ClockFreq(uint32_t HCLK_Frequency);  /* 32位无符号变量/函数 */
uint32_t RCC_GetPCLK2ClockFreq(uint32_t HCLK_Frequency);  /* 32位无符号变量/函数 */
uint32_t RCC_PLL_GetFreqDomain_SYS(uint32_t SYSCLK_Source);  /* 32位无符号变量/函数 */
uint32_t RCC_PLL_GetFreqDomain_48M(void);               /* 32位无符号变量/函数 */
#if defined(RCC_DCKCFGR_I2SSRC) || defined(RCC_DCKCFGR_I2S1SRC)  /* 条件编译 */
uint32_t RCC_PLL_GetFreqDomain_I2S(void);               /* 32位无符号变量/函数 */
#endif /* RCC_DCKCFGR_I2SSRC || RCC_DCKCFGR_I2S1SRC */
#if defined(SPDIFRX)                                    /* 条件编译 */
uint32_t RCC_PLL_GetFreqDomain_SPDIFRX(void);           /* 32位无符号变量/函数 */
#endif /* SPDIFRX */
#if defined(RCC_PLLCFGR_PLLR)                           /* 条件编译 */
#if defined(SAI1)                                       /* 条件编译 */
uint32_t RCC_PLL_GetFreqDomain_SAI(void);               /* 32位无符号变量/函数 */
#endif /* SAI1 */
#endif /* RCC_PLLCFGR_PLLR */
#if defined(DSI)                                        /* 条件编译 */
uint32_t RCC_PLL_GetFreqDomain_DSI(void);               /* 32位无符号变量/函数 */
#endif /* DSI */
#if defined(RCC_PLLSAI_SUPPORT)                         /* 条件编译 */
uint32_t RCC_PLLSAI_GetFreqDomain_SAI(void);            /* 32位无符号变量/函数 */
#if defined(RCC_PLLSAICFGR_PLLSAIP)                     /* 条件编译 */
uint32_t RCC_PLLSAI_GetFreqDomain_48M(void);            /* 32位无符号变量/函数 */
#endif /* RCC_PLLSAICFGR_PLLSAIP */
#if defined(LTDC)                                       /* 条件编译 */
uint32_t RCC_PLLSAI_GetFreqDomain_LTDC(void);           /* 32位无符号变量/函数 */
#endif /* LTDC */
#endif /* RCC_PLLSAI_SUPPORT */
#if defined(RCC_PLLI2S_SUPPORT)                         /* 条件编译 */
uint32_t RCC_PLLI2S_GetFreqDomain_I2S(void);            /* 32位无符号变量/函数 */
#if defined(RCC_PLLI2SCFGR_PLLI2SQ) && !defined(RCC_DCKCFGR_PLLI2SDIVQ)  /* 条件编译 */
uint32_t RCC_PLLI2S_GetFreqDomain_48M(void);            /* 32位无符号变量/函数 */
#endif /* RCC_PLLI2SCFGR_PLLI2SQ && !RCC_DCKCFGR_PLLI2SDIVQ */
#if defined(SAI1)                                       /* 条件编译 */
uint32_t RCC_PLLI2S_GetFreqDomain_SAI(void);            /* 32位无符号变量/函数 */
#endif /* SAI1 */
#if defined(SPDIFRX)                                    /* 条件编译 */
uint32_t RCC_PLLI2S_GetFreqDomain_SPDIFRX(void);        /* 32位无符号变量/函数 */
#endif /* SPDIFRX */
#endif /* RCC_PLLI2S_SUPPORT */
/**
  * @}
  */


/* Exported functions --------------------------------------------------------*/
/** @addtogroup RCC_LL_Exported_Functions
  * @{
  */

/** @addtogroup RCC_LL_EF_Init
  * @{
  */

/**
  * @brief  Reset the RCC clock configuration to the default reset state.
  * @note   The default reset state of the clock configuration is given below:
  *         - HSI ON and used as system clock source
  *         - HSE and PLL OFF
  *         - AHB, APB1 and APB2 prescaler set to 1.
  *         - CSS, MCO OFF
  *         - All interrupts disabled
  * @note   This function doesn't modify the configuration of the
  *         - Peripheral clocks
  *         - LSI, LSE and RTC clocks
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: RCC registers are de-initialized
  *          - ERROR: not applicable
  */
ErrorStatus LL_RCC_DeInit(void)                         /* 源代码 */
{                                                       /* 代码块开始 */
  __IO uint32_t vl_mask;                                /* 执行语句 */

  /* Set HSION bit */
  LL_RCC_HSI_Enable();                                  /* 执行语句 */

  /* Wait for HSI READY bit */
  while(LL_RCC_HSI_IsReady() != 1U)                     /* while循环 */
  {}                                                    /* 代码块结束 */

  /* Reset CFGR register */
  LL_RCC_WriteReg(CFGR, 0x00000000U);                   /* 执行语句 */

  /* Read CR register */
  vl_mask = LL_RCC_ReadReg(CR);                         /* 赋值或初始化 */

  /* Reset HSEON, HSEBYP, PLLON, CSSON bits */
  CLEAR_BIT(vl_mask,                                    /* 列表续行 */
            (RCC_CR_HSEON | RCC_CR_HSEBYP | RCC_CR_PLLON | RCC_CR_CSSON));  /* 执行语句 */

#if defined(RCC_PLLSAI_SUPPORT)                         /* 条件编译 */
  /* Reset PLLSAION bit */
  CLEAR_BIT(vl_mask, RCC_CR_PLLSAION);                  /* 执行语句 */
#endif /* RCC_PLLSAI_SUPPORT */

#if defined(RCC_PLLI2S_SUPPORT)                         /* 条件编译 */
  /* Reset PLLI2SON bit */
  CLEAR_BIT(vl_mask, RCC_CR_PLLI2SON);                  /* 执行语句 */
#endif /* RCC_PLLI2S_SUPPORT */

  /* Write new value in CR register */
  LL_RCC_WriteReg(CR, vl_mask);                         /* 执行语句 */

  /* Set HSITRIM bits to the reset value*/
  LL_RCC_HSI_SetCalibTrimming(0x10U);                   /* 执行语句 */

  /* Wait for PLL READY bit to be reset */
  while(LL_RCC_PLL_IsReady() != 0U)                     /* while循环 */
  {}                                                    /* 代码块结束 */

  /* Reset PLLCFGR register */
  LL_RCC_WriteReg(PLLCFGR, RCC_PLLCFGR_RST_VALUE);      /* 执行语句 */

#if defined(RCC_PLLI2S_SUPPORT)                         /* 条件编译 */
  /* Reset PLLI2SCFGR register */
  LL_RCC_WriteReg(PLLI2SCFGR, RCC_PLLI2SCFGR_RST_VALUE);  /* 执行语句 */
#endif /* RCC_PLLI2S_SUPPORT */

#if defined(RCC_PLLSAI_SUPPORT)                         /* 条件编译 */
  /* Reset PLLSAICFGR register */
  LL_RCC_WriteReg(PLLSAICFGR, RCC_PLLSAICFGR_RST_VALUE);  /* 执行语句 */
#endif /* RCC_PLLSAI_SUPPORT */

  /* Disable all interrupts */
  CLEAR_BIT(RCC->CIR, RCC_CIR_LSIRDYIE | RCC_CIR_LSERDYIE | RCC_CIR_HSIRDYIE | RCC_CIR_HSERDYIE | RCC_CIR_PLLRDYIE);  /* 执行语句 */

#if defined(RCC_CIR_PLLI2SRDYIE)                        /* 条件编译 */
  CLEAR_BIT(RCC->CIR, RCC_CIR_PLLI2SRDYIE);             /* 执行语句 */
#endif /* RCC_CIR_PLLI2SRDYIE */

#if defined(RCC_CIR_PLLSAIRDYIE)                        /* 条件编译 */
  CLEAR_BIT(RCC->CIR, RCC_CIR_PLLSAIRDYIE);             /* 执行语句 */
#endif /* RCC_CIR_PLLSAIRDYIE */

  /* Clear all interrupt flags */
  SET_BIT(RCC->CIR, RCC_CIR_LSIRDYC | RCC_CIR_LSERDYC | RCC_CIR_HSIRDYC | RCC_CIR_HSERDYC | RCC_CIR_PLLRDYC | RCC_CIR_CSSC);  /* 执行语句 */

#if defined(RCC_CIR_PLLI2SRDYC)                         /* 条件编译 */
  SET_BIT(RCC->CIR, RCC_CIR_PLLI2SRDYC);                /* 执行语句 */
#endif /* RCC_CIR_PLLI2SRDYC */

#if defined(RCC_CIR_PLLSAIRDYC)                         /* 条件编译 */
  SET_BIT(RCC->CIR, RCC_CIR_PLLSAIRDYC);                /* 执行语句 */
#endif /* RCC_CIR_PLLSAIRDYC */

  /* Clear LSION bit */
  CLEAR_BIT(RCC->CSR, RCC_CSR_LSION);                   /* 执行语句 */

  /* Reset all CSR flags */
  SET_BIT(RCC->CSR, RCC_CSR_RMVF);                      /* 执行语句 */

  return SUCCESS;                                       /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @addtogroup RCC_LL_EF_Get_Freq
  * @brief  Return the frequencies of different on chip clocks;  System, AHB, APB1 and APB2 buses clocks
  *         and different peripheral clocks available on the device.
  * @note   If SYSCLK source is HSI, function returns values based on HSI_VALUE(**)
  * @note   If SYSCLK source is HSE, function returns values based on HSE_VALUE(***)
  * @note   If SYSCLK source is PLL, function returns values based on HSE_VALUE(***)
  *         or HSI_VALUE(**) multiplied/divided by the PLL factors.
  * @note   (**) HSI_VALUE is a constant defined in this file (default value
  *              16 MHz) but the real value may vary depending on the variations
  *              in voltage and temperature.
  * @note   (***) HSE_VALUE is a constant defined in this file (default value
  *               25 MHz), user has to ensure that HSE_VALUE is same as the real
  *               frequency of the crystal used. Otherwise, this function may
  *               have wrong result.
  * @note   The result of this function could be incorrect when using fractional
  *         value for HSE crystal.
  * @note   This function can be used by the user application to compute the
  *         baud-rate for the communication peripherals or configure other parameters.
  * @{
  */

/**
  * @brief  Return the frequencies of different on chip clocks;  System, AHB, APB1 and APB2 buses clocks
  * @note   Each time SYSCLK, HCLK, PCLK1 and/or PCLK2 clock changes, this function
  *         must be called to update structure fields. Otherwise, any
  *         configuration based on this function will be incorrect.
  * @param  RCC_Clocks pointer to a @ref LL_RCC_ClocksTypeDef structure which will hold the clocks frequencies
  * @retval None
  */
void LL_RCC_GetSystemClocksFreq(LL_RCC_ClocksTypeDef *RCC_Clocks)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Get SYSCLK frequency */
  RCC_Clocks->SYSCLK_Frequency = RCC_GetSystemClockFreq();  /* 时钟相关操作 */

  /* HCLK clock frequency */
  RCC_Clocks->HCLK_Frequency   = RCC_GetHCLKClockFreq(RCC_Clocks->SYSCLK_Frequency);  /* 时钟相关操作 */

  /* PCLK1 clock frequency */
  RCC_Clocks->PCLK1_Frequency  = RCC_GetPCLK1ClockFreq(RCC_Clocks->HCLK_Frequency);  /* 时钟相关操作 */

  /* PCLK2 clock frequency */
  RCC_Clocks->PCLK2_Frequency  = RCC_GetPCLK2ClockFreq(RCC_Clocks->HCLK_Frequency);  /* 时钟相关操作 */
}                                                       /* 代码块结束 */

#if defined(FMPI2C1)                                    /* 条件编译 */
/**
  * @brief  Return FMPI2Cx clock frequency
  * @param  FMPI2CxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_FMPI2C1_CLKSOURCE
  * @retval FMPI2C clock frequency (in Hz)
  *         - @ref  LL_RCC_PERIPH_FREQUENCY_NO indicates that HSI oscillator is not ready
  */
uint32_t LL_RCC_GetFMPI2CClockFreq(uint32_t FMPI2CxSource)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t FMPI2C_frequency = LL_RCC_PERIPH_FREQUENCY_NO;  /* 32位无符号变量/函数 */

  /* Check parameter */
  assert_param(IS_LL_RCC_FMPI2C_CLKSOURCE(FMPI2CxSource));  /* 执行语句 */

  if (FMPI2CxSource == LL_RCC_FMPI2C1_CLKSOURCE)        /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* FMPI2C1 CLK clock frequency */
    switch (LL_RCC_GetFMPI2CClockSource(FMPI2CxSource))  /* switch分支 */
    {                                                   /* 代码块开始 */
      case LL_RCC_FMPI2C1_CLKSOURCE_SYSCLK: /* FMPI2C1 Clock is System Clock */
        FMPI2C_frequency = RCC_GetSystemClockFreq();    /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case LL_RCC_FMPI2C1_CLKSOURCE_HSI:    /* FMPI2C1 Clock is HSI Osc. */
        if (LL_RCC_HSI_IsReady())                       /* 条件判断 */
        {                                               /* 代码块开始 */
          FMPI2C_frequency = HSI_VALUE;                 /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        break;                                          /* 跳出 */

      case LL_RCC_FMPI2C1_CLKSOURCE_PCLK1:  /* FMPI2C1 Clock is PCLK1 */
      default:                                          /* default分支 */
        FMPI2C_frequency = RCC_GetPCLK1ClockFreq(RCC_GetHCLKClockFreq(RCC_GetSystemClockFreq()));  /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  return FMPI2C_frequency;                              /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* FMPI2C1 */

/**
  * @brief  Return I2Sx clock frequency
  * @param  I2SxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_I2S1_CLKSOURCE
  *         @arg @ref LL_RCC_I2S2_CLKSOURCE (*)
  *
  *         (*) value not defined in all devices.
  * @retval I2S clock frequency (in Hz)
  *         - @ref  LL_RCC_PERIPH_FREQUENCY_NO indicates that oscillator is not ready
  */
uint32_t LL_RCC_GetI2SClockFreq(uint32_t I2SxSource)    /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t i2s_frequency = LL_RCC_PERIPH_FREQUENCY_NO;  /* 32位无符号变量/函数 */

  /* Check parameter */
  assert_param(IS_LL_RCC_I2S_CLKSOURCE(I2SxSource));    /* 执行语句 */

  if (I2SxSource == LL_RCC_I2S1_CLKSOURCE)              /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* I2S1 CLK clock frequency */
    switch (LL_RCC_GetI2SClockSource(I2SxSource))       /* switch分支 */
    {                                                   /* 代码块开始 */
#if defined(RCC_PLLI2S_SUPPORT)                         /* 条件编译 */
      case LL_RCC_I2S1_CLKSOURCE_PLLI2S:       /* I2S1 Clock is PLLI2S */
        if (LL_RCC_PLLI2S_IsReady())                    /* 条件判断 */
        {                                               /* 代码块开始 */
          i2s_frequency = RCC_PLLI2S_GetFreqDomain_I2S();  /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        break;                                          /* 跳出 */
#endif /* RCC_PLLI2S_SUPPORT */

#if defined(RCC_DCKCFGR_I2SSRC) || defined(RCC_DCKCFGR_I2S1SRC)  /* 条件编译 */
      case LL_RCC_I2S1_CLKSOURCE_PLL:          /* I2S1 Clock is PLL */
        if (LL_RCC_PLL_IsReady())                       /* 条件判断 */
        {                                               /* 代码块开始 */
          i2s_frequency = RCC_PLL_GetFreqDomain_I2S();  /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        break;                                          /* 跳出 */

      case LL_RCC_I2S1_CLKSOURCE_PLLSRC:       /* I2S1 Clock is PLL Main source */
        switch (LL_RCC_PLL_GetMainSource())             /* switch分支 */
        {                                               /* 代码块开始 */
           case LL_RCC_PLLSOURCE_HSE:          /* I2S1 Clock is HSE Osc. */
             if (LL_RCC_HSE_IsReady())                  /* 条件判断 */
             {                                          /* 代码块开始 */
               i2s_frequency = HSE_VALUE;               /* 赋值或初始化 */
             }                                          /* 代码块结束 */
             break;                                     /* 跳出 */

           case LL_RCC_PLLSOURCE_HSI:          /* I2S1 Clock is HSI Osc. */
           default:                                     /* default分支 */
             if (LL_RCC_HSI_IsReady())                  /* 条件判断 */
             {                                          /* 代码块开始 */
               i2s_frequency = HSI_VALUE;               /* 赋值或初始化 */
             }                                          /* 代码块结束 */
             break;                                     /* 跳出 */
        }                                               /* 代码块结束 */
        break;                                          /* 跳出 */
#endif /* RCC_DCKCFGR_I2SSRC || RCC_DCKCFGR_I2S1SRC */

      case LL_RCC_I2S1_CLKSOURCE_PIN:          /* I2S1 Clock is External clock */
      default:                                          /* default分支 */
        i2s_frequency = EXTERNAL_CLOCK_VALUE;           /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
#if defined(RCC_DCKCFGR_I2S2SRC)                        /* 条件编译 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* I2S2 CLK clock frequency */
    switch (LL_RCC_GetI2SClockSource(I2SxSource))       /* switch分支 */
    {                                                   /* 代码块开始 */
      case LL_RCC_I2S2_CLKSOURCE_PLLI2S:       /* I2S2 Clock is PLLI2S */
        if (LL_RCC_PLLI2S_IsReady())                    /* 条件判断 */
        {                                               /* 代码块开始 */
          i2s_frequency = RCC_PLLI2S_GetFreqDomain_I2S();  /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        break;                                          /* 跳出 */

      case LL_RCC_I2S2_CLKSOURCE_PLL:          /* I2S2 Clock is PLL */
        if (LL_RCC_PLL_IsReady())                       /* 条件判断 */
        {                                               /* 代码块开始 */
          i2s_frequency = RCC_PLL_GetFreqDomain_I2S();  /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        break;                                          /* 跳出 */

      case LL_RCC_I2S2_CLKSOURCE_PLLSRC:       /* I2S2 Clock is PLL Main source */
        switch (LL_RCC_PLL_GetMainSource())             /* switch分支 */
        {                                               /* 代码块开始 */
           case LL_RCC_PLLSOURCE_HSE:          /* I2S2 Clock is HSE Osc. */
             if (LL_RCC_HSE_IsReady())                  /* 条件判断 */
             {                                          /* 代码块开始 */
               i2s_frequency = HSE_VALUE;               /* 赋值或初始化 */
             }                                          /* 代码块结束 */
             break;                                     /* 跳出 */

           case LL_RCC_PLLSOURCE_HSI:          /* I2S2 Clock is HSI Osc. */
           default:                                     /* default分支 */
             if (LL_RCC_HSI_IsReady())                  /* 条件判断 */
             {                                          /* 代码块开始 */
               i2s_frequency = HSI_VALUE;               /* 赋值或初始化 */
             }                                          /* 代码块结束 */
             break;                                     /* 跳出 */
        }                                               /* 代码块结束 */
        break;                                          /* 跳出 */

      case LL_RCC_I2S2_CLKSOURCE_PIN:          /* I2S2 Clock is External clock */
      default:                                          /* default分支 */
        i2s_frequency = EXTERNAL_CLOCK_VALUE;           /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
#endif /* RCC_DCKCFGR_I2S2SRC */

  return i2s_frequency;                                 /* 返回值 */
}                                                       /* 代码块结束 */

#if defined(LPTIM1)                                     /* 条件编译 */
/**
  * @brief  Return LPTIMx clock frequency
  * @param  LPTIMxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_LPTIM1_CLKSOURCE
  * @retval LPTIM clock frequency (in Hz)
  *         - @ref  LL_RCC_PERIPH_FREQUENCY_NO indicates that oscillator (HSI, LSI or LSE) is not ready
  */
uint32_t LL_RCC_GetLPTIMClockFreq(uint32_t LPTIMxSource)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t lptim_frequency = LL_RCC_PERIPH_FREQUENCY_NO;  /* 32位无符号变量/函数 */

  /* Check parameter */
  assert_param(IS_LL_RCC_LPTIM_CLKSOURCE(LPTIMxSource));  /* 执行语句 */

  if (LPTIMxSource == LL_RCC_LPTIM1_CLKSOURCE)          /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* LPTIM1CLK clock frequency */
    switch (LL_RCC_GetLPTIMClockSource(LPTIMxSource))   /* switch分支 */
    {                                                   /* 代码块开始 */
      case LL_RCC_LPTIM1_CLKSOURCE_LSI:    /* LPTIM1 Clock is LSI Osc. */
        if (LL_RCC_LSI_IsReady())                       /* 条件判断 */
        {                                               /* 代码块开始 */
          lptim_frequency = LSI_VALUE;                  /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        break;                                          /* 跳出 */

      case LL_RCC_LPTIM1_CLKSOURCE_HSI:    /* LPTIM1 Clock is HSI Osc. */
        if (LL_RCC_HSI_IsReady())                       /* 条件判断 */
        {                                               /* 代码块开始 */
          lptim_frequency = HSI_VALUE;                  /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        break;                                          /* 跳出 */

      case LL_RCC_LPTIM1_CLKSOURCE_LSE:    /* LPTIM1 Clock is LSE Osc. */
        if (LL_RCC_LSE_IsReady())                       /* 条件判断 */
        {                                               /* 代码块开始 */
          lptim_frequency = LSE_VALUE;                  /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        break;                                          /* 跳出 */

      case LL_RCC_LPTIM1_CLKSOURCE_PCLK1:  /* LPTIM1 Clock is PCLK1 */
      default:                                          /* default分支 */
        lptim_frequency = RCC_GetPCLK1ClockFreq(RCC_GetHCLKClockFreq(RCC_GetSystemClockFreq()));  /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  return lptim_frequency;                               /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* LPTIM1 */

#if defined(SAI1)                                       /* 条件编译 */
/**
  * @brief  Return SAIx clock frequency
  * @param  SAIxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_SAI1_CLKSOURCE (*)
  *         @arg @ref LL_RCC_SAI2_CLKSOURCE (*)
  *         @arg @ref LL_RCC_SAI1_A_CLKSOURCE (*)
  *         @arg @ref LL_RCC_SAI1_B_CLKSOURCE (*)
  *
  *         (*) value not defined in all devices.
  * @retval SAI clock frequency (in Hz)
  *         - @ref  LL_RCC_PERIPH_FREQUENCY_NO indicates that oscillator is not ready
  */
uint32_t LL_RCC_GetSAIClockFreq(uint32_t SAIxSource)    /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t sai_frequency = LL_RCC_PERIPH_FREQUENCY_NO;  /* 32位无符号变量/函数 */

  /* Check parameter */
  assert_param(IS_LL_RCC_SAI_CLKSOURCE(SAIxSource));    /* 执行语句 */

#if defined(RCC_DCKCFGR_SAI1SRC)                        /* 条件编译 */
  if ((SAIxSource == LL_RCC_SAI1_CLKSOURCE) || (SAIxSource == LL_RCC_SAI2_CLKSOURCE))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* SAI1CLK clock frequency */
    switch (LL_RCC_GetSAIClockSource(SAIxSource))       /* switch分支 */
    {                                                   /* 代码块开始 */
      case LL_RCC_SAI1_CLKSOURCE_PLLSAI:     /* PLLSAI clock used as SAI1 clock source */
      case LL_RCC_SAI2_CLKSOURCE_PLLSAI:     /* PLLSAI clock used as SAI2 clock source */
        if (LL_RCC_PLLSAI_IsReady())                    /* 条件判断 */
        {                                               /* 代码块开始 */
          sai_frequency = RCC_PLLSAI_GetFreqDomain_SAI();  /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        break;                                          /* 跳出 */

      case LL_RCC_SAI1_CLKSOURCE_PLLI2S:     /* PLLI2S clock used as SAI1 clock source */
      case LL_RCC_SAI2_CLKSOURCE_PLLI2S:     /* PLLI2S clock used as SAI2 clock source */
        if (LL_RCC_PLLI2S_IsReady())                    /* 条件判断 */
        {                                               /* 代码块开始 */
          sai_frequency = RCC_PLLI2S_GetFreqDomain_SAI();  /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        break;                                          /* 跳出 */

      case LL_RCC_SAI1_CLKSOURCE_PLL:        /* PLL clock used as SAI1 clock source */
      case LL_RCC_SAI2_CLKSOURCE_PLL:        /* PLL clock used as SAI2 clock source */
        if (LL_RCC_PLL_IsReady())                       /* 条件判断 */
        {                                               /* 代码块开始 */
          sai_frequency = RCC_PLL_GetFreqDomain_SAI();  /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        break;                                          /* 跳出 */

      case LL_RCC_SAI2_CLKSOURCE_PLLSRC:                /* case标签 */
        switch (LL_RCC_PLL_GetMainSource())             /* switch分支 */
        {                                               /* 代码块开始 */
           case LL_RCC_PLLSOURCE_HSE:        /* HSE clock used as SAI2 clock source */
             if (LL_RCC_HSE_IsReady())                  /* 条件判断 */
             {                                          /* 代码块开始 */
               sai_frequency = HSE_VALUE;               /* 赋值或初始化 */
             }                                          /* 代码块结束 */
             break;                                     /* 跳出 */

           case LL_RCC_PLLSOURCE_HSI:        /* HSI clock used as SAI2 clock source */
           default:                                     /* default分支 */
             if (LL_RCC_HSI_IsReady())                  /* 条件判断 */
             {                                          /* 代码块开始 */
               sai_frequency = HSI_VALUE;               /* 赋值或初始化 */
             }                                          /* 代码块结束 */
             break;                                     /* 跳出 */
        }                                               /* 代码块结束 */
        break;                                          /* 跳出 */

      case LL_RCC_SAI1_CLKSOURCE_PIN:        /* External input clock used as SAI1 clock source */
      default:                                          /* default分支 */
        sai_frequency = EXTERNAL_CLOCK_VALUE;           /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
#endif /* RCC_DCKCFGR_SAI1SRC */
#if defined(RCC_DCKCFGR_SAI1ASRC)                       /* 条件编译 */
  if ((SAIxSource == LL_RCC_SAI1_A_CLKSOURCE) || (SAIxSource == LL_RCC_SAI1_B_CLKSOURCE))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* SAI1CLK clock frequency */
    switch (LL_RCC_GetSAIClockSource(SAIxSource))       /* switch分支 */
    {                                                   /* 代码块开始 */
#if defined(RCC_PLLSAI_SUPPORT)                         /* 条件编译 */
      case LL_RCC_SAI1_A_CLKSOURCE_PLLSAI:     /* PLLSAI clock used as SAI1 Block A clock source */
      case LL_RCC_SAI1_B_CLKSOURCE_PLLSAI:     /* PLLSAI clock used as SAI1 Block B clock source */
        if (LL_RCC_PLLSAI_IsReady())                    /* 条件判断 */
        {                                               /* 代码块开始 */
          sai_frequency = RCC_PLLSAI_GetFreqDomain_SAI();  /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        break;                                          /* 跳出 */
#endif /* RCC_PLLSAI_SUPPORT */

      case LL_RCC_SAI1_A_CLKSOURCE_PLLI2S:     /* PLLI2S clock used as SAI1 Block A clock source */
      case LL_RCC_SAI1_B_CLKSOURCE_PLLI2S:     /* PLLI2S clock used as SAI1 Block B clock source */
        if (LL_RCC_PLLI2S_IsReady())                    /* 条件判断 */
        {                                               /* 代码块开始 */
          sai_frequency = RCC_PLLI2S_GetFreqDomain_SAI();  /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        break;                                          /* 跳出 */

#if defined(RCC_SAI1A_PLLSOURCE_SUPPORT)                /* 条件编译 */
      case LL_RCC_SAI1_A_CLKSOURCE_PLL:        /* PLL clock used as SAI1 Block A clock source */
      case LL_RCC_SAI1_B_CLKSOURCE_PLL:        /* PLL clock used as SAI1 Block B clock source */
        if (LL_RCC_PLL_IsReady())                       /* 条件判断 */
        {                                               /* 代码块开始 */
          sai_frequency = RCC_PLL_GetFreqDomain_SAI();  /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        break;                                          /* 跳出 */

      case LL_RCC_SAI1_A_CLKSOURCE_PLLSRC:              /* case标签 */
      case LL_RCC_SAI1_B_CLKSOURCE_PLLSRC:              /* case标签 */
        switch (LL_RCC_PLL_GetMainSource())             /* switch分支 */
        {                                               /* 代码块开始 */
           case LL_RCC_PLLSOURCE_HSE:          /* HSE clock used as SAI1 Block A or B clock source */
             if (LL_RCC_HSE_IsReady())                  /* 条件判断 */
             {                                          /* 代码块开始 */
               sai_frequency = HSE_VALUE;               /* 赋值或初始化 */
             }                                          /* 代码块结束 */
             break;                                     /* 跳出 */

           case LL_RCC_PLLSOURCE_HSI:          /* HSI clock used as SAI1 Block A or B clock source */
           default:                                     /* default分支 */
             if (LL_RCC_HSI_IsReady())                  /* 条件判断 */
             {                                          /* 代码块开始 */
               sai_frequency = HSI_VALUE;               /* 赋值或初始化 */
             }                                          /* 代码块结束 */
             break;                                     /* 跳出 */
        }                                               /* 代码块结束 */
        break;                                          /* 跳出 */
#endif /* RCC_SAI1A_PLLSOURCE_SUPPORT */

      case LL_RCC_SAI1_A_CLKSOURCE_PIN:        /* External input clock used as SAI1 Block A clock source */
      case LL_RCC_SAI1_B_CLKSOURCE_PIN:        /* External input clock used as SAI1 Block B clock source */
      default:                                          /* default分支 */
        sai_frequency = EXTERNAL_CLOCK_VALUE;           /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
#endif /* RCC_DCKCFGR_SAI1ASRC */

  return sai_frequency;                                 /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* SAI1 */

#if defined(SDIO)                                       /* 条件编译 */
/**
  * @brief  Return SDIOx clock frequency
  * @param  SDIOxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_SDIO_CLKSOURCE
  * @retval SDIO clock frequency (in Hz)
  *         - @ref  LL_RCC_PERIPH_FREQUENCY_NO indicates that oscillator is not ready
  */
uint32_t LL_RCC_GetSDIOClockFreq(uint32_t SDIOxSource)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t SDIO_frequency = LL_RCC_PERIPH_FREQUENCY_NO;  /* 32位无符号变量/函数 */

  /* Check parameter */
  assert_param(IS_LL_RCC_SDIO_CLKSOURCE(SDIOxSource));  /* 执行语句 */

  if (SDIOxSource == LL_RCC_SDIO_CLKSOURCE)             /* 条件判断 */
  {                                                     /* 代码块开始 */
#if defined(RCC_DCKCFGR_SDIOSEL) || defined(RCC_DCKCFGR2_SDIOSEL)  /* 条件编译 */
    /* SDIOCLK clock frequency */
    switch (LL_RCC_GetSDIOClockSource(SDIOxSource))     /* switch分支 */
    {                                                   /* 代码块开始 */
      case LL_RCC_SDIO_CLKSOURCE_PLL48CLK:         /* PLL48M clock used as SDIO clock source */
        switch (LL_RCC_GetCK48MClockSource(LL_RCC_CK48M_CLKSOURCE))  /* switch分支 */
        {                                               /* 代码块开始 */
          case LL_RCC_CK48M_CLKSOURCE_PLL:         /* PLL clock used as 48Mhz domain clock */
            if (LL_RCC_PLL_IsReady())                   /* 条件判断 */
            {                                           /* 代码块开始 */
              SDIO_frequency = RCC_PLL_GetFreqDomain_48M();  /* 赋值或初始化 */
            }                                           /* 代码块结束 */
          break;                                        /* 跳出 */

#if defined(RCC_PLLSAI_SUPPORT)                         /* 条件编译 */
          case LL_RCC_CK48M_CLKSOURCE_PLLSAI:      /* PLLSAI clock used as 48Mhz domain clock */
          default:                                      /* default分支 */
            if (LL_RCC_PLLSAI_IsReady())                /* 条件判断 */
            {                                           /* 代码块开始 */
              SDIO_frequency = RCC_PLLSAI_GetFreqDomain_48M();  /* 赋值或初始化 */
            }                                           /* 代码块结束 */
            break;                                      /* 跳出 */
#endif /* RCC_PLLSAI_SUPPORT */

#if defined(RCC_PLLI2SCFGR_PLLI2SQ) && !defined(RCC_DCKCFGR_PLLI2SDIVQ)  /* 条件编译 */
          case LL_RCC_CK48M_CLKSOURCE_PLLI2S:      /* PLLI2S clock used as 48Mhz domain clock */
          default:                                      /* default分支 */
            if (LL_RCC_PLLI2S_IsReady())                /* 条件判断 */
            {                                           /* 代码块开始 */
              SDIO_frequency = RCC_PLLI2S_GetFreqDomain_48M();  /* 赋值或初始化 */
            }                                           /* 代码块结束 */
            break;                                      /* 跳出 */
#endif /* RCC_PLLI2SCFGR_PLLI2SQ && !RCC_DCKCFGR_PLLI2SDIVQ */
        }                                               /* 代码块结束 */
        break;                                          /* 跳出 */

      case LL_RCC_SDIO_CLKSOURCE_SYSCLK:           /* PLL clock used as SDIO clock source */
      default:                                          /* default分支 */
      SDIO_frequency = RCC_GetSystemClockFreq();        /* 赋值或初始化 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
#else                                                   /* 条件编译否则 */
    /* PLL clock used as 48Mhz domain clock */
    if (LL_RCC_PLL_IsReady())                           /* 条件判断 */
    {                                                   /* 代码块开始 */
      SDIO_frequency = RCC_PLL_GetFreqDomain_48M();     /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
#endif /* RCC_DCKCFGR_SDIOSEL || RCC_DCKCFGR2_SDIOSEL */
  }                                                     /* 代码块结束 */

  return SDIO_frequency;                                /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* SDIO */

#if defined(RNG)                                        /* 条件编译 */
/**
  * @brief  Return RNGx clock frequency
  * @param  RNGxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_RNG_CLKSOURCE
  * @retval RNG clock frequency (in Hz)
  *         - @ref  LL_RCC_PERIPH_FREQUENCY_NO indicates that oscillator is not ready
  */
uint32_t LL_RCC_GetRNGClockFreq(uint32_t RNGxSource)    /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t rng_frequency = LL_RCC_PERIPH_FREQUENCY_NO;  /* 32位无符号变量/函数 */

  /* Check parameter */
  assert_param(IS_LL_RCC_RNG_CLKSOURCE(RNGxSource));    /* 执行语句 */

#if defined(RCC_DCKCFGR_CK48MSEL) || defined(RCC_DCKCFGR2_CK48MSEL)  /* 条件编译 */
  /* RNGCLK clock frequency */
  switch (LL_RCC_GetRNGClockSource(RNGxSource))         /* switch分支 */
  {                                                     /* 代码块开始 */
#if defined(RCC_PLLI2SCFGR_PLLI2SQ) && !defined(RCC_DCKCFGR_PLLI2SDIVQ)  /* 条件编译 */
    case LL_RCC_RNG_CLKSOURCE_PLLI2S:        /* PLLI2S clock used as RNG clock source */
      if (LL_RCC_PLLI2S_IsReady())                      /* 条件判断 */
      {                                                 /* 代码块开始 */
        rng_frequency = RCC_PLLI2S_GetFreqDomain_48M();  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */
#endif /* RCC_PLLI2SCFGR_PLLI2SQ && !RCC_DCKCFGR_PLLI2SDIVQ */

#if defined(RCC_PLLSAI_SUPPORT)                         /* 条件编译 */
    case LL_RCC_RNG_CLKSOURCE_PLLSAI:        /* PLLSAI clock used as RNG clock source */
      if (LL_RCC_PLLSAI_IsReady())                      /* 条件判断 */
      {                                                 /* 代码块开始 */
        rng_frequency = RCC_PLLSAI_GetFreqDomain_48M();  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */
#endif /* RCC_PLLSAI_SUPPORT */

    case LL_RCC_RNG_CLKSOURCE_PLL:           /* PLL clock used as RNG clock source */
    default:                                            /* default分支 */
      if (LL_RCC_PLL_IsReady())                         /* 条件判断 */
      {                                                 /* 代码块开始 */
        rng_frequency = RCC_PLL_GetFreqDomain_48M();    /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */
#else                                                   /* 条件编译否则 */
  /* PLL clock used as RNG clock source */
  if (LL_RCC_PLL_IsReady())                             /* 条件判断 */
  {                                                     /* 代码块开始 */
    rng_frequency = RCC_PLL_GetFreqDomain_48M();        /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
#endif /* RCC_DCKCFGR_CK48MSEL || RCC_DCKCFGR2_CK48MSEL */

  return rng_frequency;                                 /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* RNG */

#if defined(CEC)                                        /* 条件编译 */
/**
  * @brief  Return CEC clock frequency
  * @param  CECxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_CEC_CLKSOURCE
  * @retval CEC clock frequency (in Hz)
  *         - @ref  LL_RCC_PERIPH_FREQUENCY_NO indicates that oscillator (HSI or LSE) is not ready
  */
uint32_t LL_RCC_GetCECClockFreq(uint32_t CECxSource)    /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t cec_frequency = LL_RCC_PERIPH_FREQUENCY_NO;  /* 32位无符号变量/函数 */

  /* Check parameter */
  assert_param(IS_LL_RCC_CEC_CLKSOURCE(CECxSource));    /* 执行语句 */

  /* CECCLK clock frequency */
  switch (LL_RCC_GetCECClockSource(CECxSource))         /* switch分支 */
  {                                                     /* 代码块开始 */
    case LL_RCC_CEC_CLKSOURCE_LSE:           /* CEC Clock is LSE Osc. */
      if (LL_RCC_LSE_IsReady())                         /* 条件判断 */
      {                                                 /* 代码块开始 */
        cec_frequency = LSE_VALUE;                      /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */

    case LL_RCC_CEC_CLKSOURCE_HSI_DIV488:    /* CEC Clock is HSI Osc. */
    default:                                            /* default分支 */
      if (LL_RCC_HSI_IsReady())                         /* 条件判断 */
      {                                                 /* 代码块开始 */
        cec_frequency = HSI_VALUE/488U;                 /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  return cec_frequency;                                 /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* CEC */

#if defined(USB_OTG_FS) || defined(USB_OTG_HS)          /* 条件编译 */
/**
  * @brief  Return USBx clock frequency
  * @param  USBxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_USB_CLKSOURCE
  * @retval USB clock frequency (in Hz)
  *         - @ref  LL_RCC_PERIPH_FREQUENCY_NO indicates that oscillator is not ready
  */
uint32_t LL_RCC_GetUSBClockFreq(uint32_t USBxSource)    /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t usb_frequency = LL_RCC_PERIPH_FREQUENCY_NO;  /* 32位无符号变量/函数 */

  /* Check parameter */
  assert_param(IS_LL_RCC_USB_CLKSOURCE(USBxSource));    /* 执行语句 */

#if defined(RCC_DCKCFGR_CK48MSEL) || defined(RCC_DCKCFGR2_CK48MSEL)  /* 条件编译 */
  /* USBCLK clock frequency */
  switch (LL_RCC_GetUSBClockSource(USBxSource))         /* switch分支 */
  {                                                     /* 代码块开始 */
#if defined(RCC_PLLI2SCFGR_PLLI2SQ) && !defined(RCC_DCKCFGR_PLLI2SDIVQ)  /* 条件编译 */
    case LL_RCC_USB_CLKSOURCE_PLLI2S:       /* PLLI2S clock used as USB clock source */
      if (LL_RCC_PLLI2S_IsReady())                      /* 条件判断 */
      {                                                 /* 代码块开始 */
        usb_frequency = RCC_PLLI2S_GetFreqDomain_48M();  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */

#endif /* RCC_PLLI2SCFGR_PLLI2SQ && !RCC_DCKCFGR_PLLI2SDIVQ */

#if defined(RCC_PLLSAI_SUPPORT)                         /* 条件编译 */
    case LL_RCC_USB_CLKSOURCE_PLLSAI:       /* PLLSAI clock used as USB clock source */
      if (LL_RCC_PLLSAI_IsReady())                      /* 条件判断 */
      {                                                 /* 代码块开始 */
        usb_frequency = RCC_PLLSAI_GetFreqDomain_48M();  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */
#endif /* RCC_PLLSAI_SUPPORT */

    case LL_RCC_USB_CLKSOURCE_PLL:          /* PLL clock used as USB clock source */
    default:                                            /* default分支 */
      if (LL_RCC_PLL_IsReady())                         /* 条件判断 */
      {                                                 /* 代码块开始 */
        usb_frequency = RCC_PLL_GetFreqDomain_48M();    /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */
#else                                                   /* 条件编译否则 */
  /* PLL clock used as USB clock source */
  if (LL_RCC_PLL_IsReady())                             /* 条件判断 */
  {                                                     /* 代码块开始 */
    usb_frequency = RCC_PLL_GetFreqDomain_48M();        /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
#endif /* RCC_DCKCFGR_CK48MSEL || RCC_DCKCFGR2_CK48MSEL */

  return usb_frequency;                                 /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* USB_OTG_FS || USB_OTG_HS */

#if defined(DFSDM1_Channel0)                            /* 条件编译 */
/**
  * @brief  Return DFSDMx clock frequency
  * @param  DFSDMxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_DFSDM1_CLKSOURCE
  *         @arg @ref LL_RCC_DFSDM2_CLKSOURCE (*)
  *
  *         (*) value not defined in all devices.
  * @retval DFSDM clock frequency (in Hz)
  */
uint32_t LL_RCC_GetDFSDMClockFreq(uint32_t DFSDMxSource)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t dfsdm_frequency = LL_RCC_PERIPH_FREQUENCY_NO;  /* 32位无符号变量/函数 */

  /* Check parameter */
  assert_param(IS_LL_RCC_DFSDM_CLKSOURCE(DFSDMxSource));  /* 执行语句 */

  if (DFSDMxSource == LL_RCC_DFSDM1_CLKSOURCE)          /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* DFSDM1CLK clock frequency */
    switch (LL_RCC_GetDFSDMClockSource(DFSDMxSource))   /* switch分支 */
    {                                                   /* 代码块开始 */
      case LL_RCC_DFSDM1_CLKSOURCE_SYSCLK:      /* DFSDM1 Clock is SYSCLK */
        dfsdm_frequency = RCC_GetSystemClockFreq();     /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case LL_RCC_DFSDM1_CLKSOURCE_PCLK2:       /* DFSDM1 Clock is PCLK2 */
      default:                                          /* default分支 */
        dfsdm_frequency = RCC_GetPCLK2ClockFreq(RCC_GetHCLKClockFreq(RCC_GetSystemClockFreq()));  /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
#if defined(DFSDM2_Channel0)                            /* 条件编译 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* DFSDM2CLK clock frequency */
    switch (LL_RCC_GetDFSDMClockSource(DFSDMxSource))   /* switch分支 */
    {                                                   /* 代码块开始 */
      case LL_RCC_DFSDM2_CLKSOURCE_SYSCLK:      /* DFSDM2 Clock is SYSCLK */
        dfsdm_frequency = RCC_GetSystemClockFreq();     /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case LL_RCC_DFSDM2_CLKSOURCE_PCLK2:       /* DFSDM2 Clock is PCLK2 */
      default:                                          /* default分支 */
        dfsdm_frequency = RCC_GetPCLK2ClockFreq(RCC_GetHCLKClockFreq(RCC_GetSystemClockFreq()));  /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
#endif /* DFSDM2_Channel0 */

  return dfsdm_frequency;                               /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return DFSDMx Audio clock frequency
  * @param  DFSDMxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_DFSDM1_AUDIO_CLKSOURCE
  *         @arg @ref LL_RCC_DFSDM2_AUDIO_CLKSOURCE (*)
  *
  *         (*) value not defined in all devices.
  * @retval DFSDM clock frequency (in Hz)
  *         - @ref  LL_RCC_PERIPH_FREQUENCY_NO indicates that oscillator is not ready
  */
uint32_t LL_RCC_GetDFSDMAudioClockFreq(uint32_t DFSDMxSource)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t dfsdm_frequency = LL_RCC_PERIPH_FREQUENCY_NO;  /* 32位无符号变量/函数 */

  /* Check parameter */
  assert_param(IS_LL_RCC_DFSDM_AUDIO_CLKSOURCE(DFSDMxSource));  /* 执行语句 */

  if (DFSDMxSource == LL_RCC_DFSDM1_AUDIO_CLKSOURCE)    /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* DFSDM1CLK clock frequency */
    switch (LL_RCC_GetDFSDMAudioClockSource(DFSDMxSource))  /* switch分支 */
    {                                                   /* 代码块开始 */
      case LL_RCC_DFSDM1_AUDIO_CLKSOURCE_I2S1:     /* I2S1 clock used as DFSDM1 clock */
        dfsdm_frequency = LL_RCC_GetI2SClockFreq(LL_RCC_I2S1_CLKSOURCE);  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case LL_RCC_DFSDM1_AUDIO_CLKSOURCE_I2S2:     /* I2S2 clock used as DFSDM1 clock */
      default:                                          /* default分支 */
        dfsdm_frequency = LL_RCC_GetI2SClockFreq(LL_RCC_I2S2_CLKSOURCE);  /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
#if defined(DFSDM2_Channel0)                            /* 条件编译 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* DFSDM2CLK clock frequency */
    switch (LL_RCC_GetDFSDMAudioClockSource(DFSDMxSource))  /* switch分支 */
    {                                                   /* 代码块开始 */
      case LL_RCC_DFSDM2_AUDIO_CLKSOURCE_I2S1:     /* I2S1 clock used as DFSDM2 clock */
        dfsdm_frequency = LL_RCC_GetI2SClockFreq(LL_RCC_I2S1_CLKSOURCE);  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case LL_RCC_DFSDM2_AUDIO_CLKSOURCE_I2S2:     /* I2S2 clock used as DFSDM2 clock */
      default:                                          /* default分支 */
        dfsdm_frequency = LL_RCC_GetI2SClockFreq(LL_RCC_I2S2_CLKSOURCE);  /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
#endif /* DFSDM2_Channel0 */

  return dfsdm_frequency;                               /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* DFSDM1_Channel0 */

#if defined(DSI)                                        /* 条件编译 */
/**
  * @brief  Return DSI clock frequency
  * @param  DSIxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_DSI_CLKSOURCE
  * @retval DSI clock frequency (in Hz)
  *         - @ref  LL_RCC_PERIPH_FREQUENCY_NO indicates that oscillator is not ready
  *         - @ref  LL_RCC_PERIPH_FREQUENCY_NA indicates that external clock is used
  */
uint32_t LL_RCC_GetDSIClockFreq(uint32_t DSIxSource)    /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t dsi_frequency = LL_RCC_PERIPH_FREQUENCY_NO;  /* 32位无符号变量/函数 */

  /* Check parameter */
  assert_param(IS_LL_RCC_DSI_CLKSOURCE(DSIxSource));    /* 执行语句 */

  /* DSICLK clock frequency */
  switch (LL_RCC_GetDSIClockSource(DSIxSource))         /* switch分支 */
  {                                                     /* 代码块开始 */
    case LL_RCC_DSI_CLKSOURCE_PLL:     /* DSI Clock is PLL Osc. */
      if (LL_RCC_PLL_IsReady())                         /* 条件判断 */
      {                                                 /* 代码块开始 */
        dsi_frequency = RCC_PLL_GetFreqDomain_DSI();    /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */

    case LL_RCC_DSI_CLKSOURCE_PHY:    /* DSI Clock is DSI physical clock. */
    default:                                            /* default分支 */
      dsi_frequency = LL_RCC_PERIPH_FREQUENCY_NA;       /* 赋值或初始化 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  return dsi_frequency;                                 /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* DSI */

#if defined(LTDC)                                       /* 条件编译 */
/**
  * @brief  Return LTDC clock frequency
  * @param  LTDCxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_LTDC_CLKSOURCE
  * @retval LTDC clock frequency (in Hz)
  *         - @ref  LL_RCC_PERIPH_FREQUENCY_NO indicates that oscillator PLLSAI is not ready
  */
uint32_t LL_RCC_GetLTDCClockFreq(uint32_t LTDCxSource)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t ltdc_frequency = LL_RCC_PERIPH_FREQUENCY_NO;  /* 32位无符号变量/函数 */

  /* Check parameter */
  assert_param(IS_LL_RCC_LTDC_CLKSOURCE(LTDCxSource));  /* 执行语句 */

  if (LL_RCC_PLLSAI_IsReady())                          /* 条件判断 */
  {                                                     /* 代码块开始 */
     ltdc_frequency = RCC_PLLSAI_GetFreqDomain_LTDC();  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return ltdc_frequency;                                /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* LTDC */

#if defined(SPDIFRX)                                    /* 条件编译 */
/**
  * @brief  Return SPDIFRX clock frequency
  * @param  SPDIFRXxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_SPDIFRX1_CLKSOURCE
  * @retval SPDIFRX clock frequency (in Hz)
  *         - @ref  LL_RCC_PERIPH_FREQUENCY_NO indicates that oscillator is not ready
  */
uint32_t LL_RCC_GetSPDIFRXClockFreq(uint32_t SPDIFRXxSource)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t spdifrx_frequency = LL_RCC_PERIPH_FREQUENCY_NO;  /* 32位无符号变量/函数 */

  /* Check parameter */
  assert_param(IS_LL_RCC_SPDIFRX_CLKSOURCE(SPDIFRXxSource));  /* 执行语句 */

  /* SPDIFRX1CLK clock frequency */
  switch (LL_RCC_GetSPDIFRXClockSource(SPDIFRXxSource))  /* switch分支 */
  {                                                     /* 代码块开始 */
    case LL_RCC_SPDIFRX1_CLKSOURCE_PLLI2S:  /* SPDIFRX Clock is PLLI2S Osc. */
      if (LL_RCC_PLLI2S_IsReady())                      /* 条件判断 */
      {                                                 /* 代码块开始 */
        spdifrx_frequency = RCC_PLLI2S_GetFreqDomain_SPDIFRX();  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */

    case LL_RCC_SPDIFRX1_CLKSOURCE_PLL:     /* SPDIFRX Clock is PLL Osc. */
    default:                                            /* default分支 */
      if (LL_RCC_PLL_IsReady())                         /* 条件判断 */
      {                                                 /* 代码块开始 */
        spdifrx_frequency = RCC_PLL_GetFreqDomain_SPDIFRX();  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  return spdifrx_frequency;                             /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* SPDIFRX */

/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup RCC_LL_Private_Functions
  * @{
  */

/**
  * @brief  Return SYSTEM clock frequency
  * @retval SYSTEM clock frequency (in Hz)
  */
uint32_t RCC_GetSystemClockFreq(void)                   /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t frequency = 0U;                              /* 32位无符号变量/函数 */

  /* Get SYSCLK source -------------------------------------------------------*/
  switch (LL_RCC_GetSysClkSource())                     /* switch分支 */
  {                                                     /* 代码块开始 */
    case LL_RCC_SYS_CLKSOURCE_STATUS_HSI:  /* HSI used as system clock  source */
      frequency = HSI_VALUE;                            /* 赋值或初始化 */
      break;                                            /* 跳出 */

    case LL_RCC_SYS_CLKSOURCE_STATUS_HSE:  /* HSE used as system clock  source */
      frequency = HSE_VALUE;                            /* 赋值或初始化 */
      break;                                            /* 跳出 */

    case LL_RCC_SYS_CLKSOURCE_STATUS_PLL:  /* PLL used as system clock  source */
      frequency = RCC_PLL_GetFreqDomain_SYS(LL_RCC_SYS_CLKSOURCE_STATUS_PLL);  /* 赋值或初始化 */
      break;                                            /* 跳出 */

#if defined(RCC_PLLR_SYSCLK_SUPPORT)                    /* 条件编译 */
    case LL_RCC_SYS_CLKSOURCE_STATUS_PLLR: /* PLLR used as system clock  source */
      frequency = RCC_PLL_GetFreqDomain_SYS(LL_RCC_SYS_CLKSOURCE_STATUS_PLLR);  /* 赋值或初始化 */
      break;                                            /* 跳出 */
#endif /* RCC_PLLR_SYSCLK_SUPPORT */

    default:                                            /* default分支 */
      frequency = HSI_VALUE;                            /* 赋值或初始化 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  return frequency;                                     /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return HCLK clock frequency
  * @param  SYSCLK_Frequency SYSCLK clock frequency
  * @retval HCLK clock frequency (in Hz)
  */
uint32_t RCC_GetHCLKClockFreq(uint32_t SYSCLK_Frequency)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  /* HCLK clock frequency */
  return __LL_RCC_CALC_HCLK_FREQ(SYSCLK_Frequency, LL_RCC_GetAHBPrescaler());  /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return PCLK1 clock frequency
  * @param  HCLK_Frequency HCLK clock frequency
  * @retval PCLK1 clock frequency (in Hz)
  */
uint32_t RCC_GetPCLK1ClockFreq(uint32_t HCLK_Frequency)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  /* PCLK1 clock frequency */
  return __LL_RCC_CALC_PCLK1_FREQ(HCLK_Frequency, LL_RCC_GetAPB1Prescaler());  /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return PCLK2 clock frequency
  * @param  HCLK_Frequency HCLK clock frequency
  * @retval PCLK2 clock frequency (in Hz)
  */
uint32_t RCC_GetPCLK2ClockFreq(uint32_t HCLK_Frequency)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  /* PCLK2 clock frequency */
  return __LL_RCC_CALC_PCLK2_FREQ(HCLK_Frequency, LL_RCC_GetAPB2Prescaler());  /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return PLL clock frequency used for system domain
  * @param  SYSCLK_Source System clock source
  * @retval PLL clock frequency (in Hz)
  */
uint32_t RCC_PLL_GetFreqDomain_SYS(uint32_t SYSCLK_Source)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t pllinputfreq = 0U, pllsource = 0U, plloutputfreq = 0U;  /* 32位无符号变量/函数 */

  /* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLLM) * PLLN
     SYSCLK = PLL_VCO / (PLLP or PLLR)
  */
  pllsource = LL_RCC_PLL_GetMainSource();               /* 赋值或初始化 */

  switch (pllsource)                                    /* switch分支 */
  {                                                     /* 代码块开始 */
    case LL_RCC_PLLSOURCE_HSI:  /* HSI used as PLL clock source */
      pllinputfreq = HSI_VALUE;                         /* 赋值或初始化 */
      break;                                            /* 跳出 */

    case LL_RCC_PLLSOURCE_HSE:  /* HSE used as PLL clock source */
      pllinputfreq = HSE_VALUE;                         /* 赋值或初始化 */
      break;                                            /* 跳出 */

    default:                                            /* default分支 */
      pllinputfreq = HSI_VALUE;                         /* 赋值或初始化 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  if (SYSCLK_Source == LL_RCC_SYS_CLKSOURCE_STATUS_PLL)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    plloutputfreq = __LL_RCC_CALC_PLLCLK_FREQ(pllinputfreq, LL_RCC_PLL_GetDivider(),  /* 列表续行 */
                                        LL_RCC_PLL_GetN(), LL_RCC_PLL_GetP());  /* 执行语句 */
  }                                                     /* 代码块结束 */
#if defined(RCC_PLLR_SYSCLK_SUPPORT)                    /* 条件编译 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    plloutputfreq = __LL_RCC_CALC_PLLRCLK_FREQ(pllinputfreq, LL_RCC_PLL_GetDivider(),  /* 列表续行 */
                                        LL_RCC_PLL_GetN(), LL_RCC_PLL_GetR());  /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif /* RCC_PLLR_SYSCLK_SUPPORT */

  return plloutputfreq;                                 /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return PLL clock frequency used for 48 MHz domain
  * @retval PLL clock frequency (in Hz)
  */
uint32_t RCC_PLL_GetFreqDomain_48M(void)                /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t pllinputfreq = 0U, pllsource = 0U;           /* 32位无符号变量/函数 */

  /* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLLM ) * PLLN
     48M Domain clock = PLL_VCO / PLLQ
  */
  pllsource = LL_RCC_PLL_GetMainSource();               /* 赋值或初始化 */

  switch (pllsource)                                    /* switch分支 */
  {                                                     /* 代码块开始 */
    case LL_RCC_PLLSOURCE_HSI:  /* HSI used as PLL clock source */
      pllinputfreq = HSI_VALUE;                         /* 赋值或初始化 */
      break;                                            /* 跳出 */

    case LL_RCC_PLLSOURCE_HSE:  /* HSE used as PLL clock source */
      pllinputfreq = HSE_VALUE;                         /* 赋值或初始化 */
      break;                                            /* 跳出 */

    default:                                            /* default分支 */
      pllinputfreq = HSI_VALUE;                         /* 赋值或初始化 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */
  return __LL_RCC_CALC_PLLCLK_48M_FREQ(pllinputfreq, LL_RCC_PLL_GetDivider(),  /* 返回值 */
                                        LL_RCC_PLL_GetN(), LL_RCC_PLL_GetQ());  /* 执行语句 */
}                                                       /* 代码块结束 */

#if defined(DSI)                                        /* 条件编译 */
/**
  * @brief  Return PLL clock frequency used for DSI clock
  * @retval PLL clock frequency (in Hz)
  */
uint32_t RCC_PLL_GetFreqDomain_DSI(void)                /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t pllinputfreq = 0U, pllsource = 0U;           /* 32位无符号变量/函数 */

  /* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLLM) * PLLN
     DSICLK = PLL_VCO / PLLR
  */
  pllsource = LL_RCC_PLL_GetMainSource();               /* 赋值或初始化 */

  switch (pllsource)                                    /* switch分支 */
  {                                                     /* 代码块开始 */
    case LL_RCC_PLLSOURCE_HSE:  /* HSE used as PLL clock source */
      pllinputfreq = HSE_VALUE;                         /* 赋值或初始化 */
      break;                                            /* 跳出 */

    case LL_RCC_PLLSOURCE_HSI:  /* HSI used as PLL clock source */
    default:                                            /* default分支 */
      pllinputfreq = HSI_VALUE;                         /* 赋值或初始化 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */
  return __LL_RCC_CALC_PLLCLK_DSI_FREQ(pllinputfreq, LL_RCC_PLL_GetDivider(),  /* 返回值 */
                                        LL_RCC_PLL_GetN(), LL_RCC_PLL_GetR());  /* 执行语句 */
}                                                       /* 代码块结束 */
#endif /* DSI */

#if defined(RCC_DCKCFGR_I2SSRC) || defined(RCC_DCKCFGR_I2S1SRC)  /* 条件编译 */
/**
  * @brief  Return PLL clock frequency used for I2S clock
  * @retval PLL clock frequency (in Hz)
  */
uint32_t RCC_PLL_GetFreqDomain_I2S(void)                /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t pllinputfreq = 0U, pllsource = 0U;           /* 32位无符号变量/函数 */

  /* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLLM) * PLLN
     I2SCLK = PLL_VCO / PLLR
  */
  pllsource = LL_RCC_PLL_GetMainSource();               /* 赋值或初始化 */

  switch (pllsource)                                    /* switch分支 */
  {                                                     /* 代码块开始 */
    case LL_RCC_PLLSOURCE_HSE:  /* HSE used as PLL clock source */
      pllinputfreq = HSE_VALUE;                         /* 赋值或初始化 */
      break;                                            /* 跳出 */

    case LL_RCC_PLLSOURCE_HSI:  /* HSI used as PLL clock source */
    default:                                            /* default分支 */
      pllinputfreq = HSI_VALUE;                         /* 赋值或初始化 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */
  return __LL_RCC_CALC_PLLCLK_I2S_FREQ(pllinputfreq, LL_RCC_PLL_GetDivider(),  /* 返回值 */
                                        LL_RCC_PLL_GetN(), LL_RCC_PLL_GetR());  /* 执行语句 */
}                                                       /* 代码块结束 */
#endif /* RCC_DCKCFGR_I2SSRC || RCC_DCKCFGR_I2S1SRC */

#if defined(SPDIFRX)                                    /* 条件编译 */
/**
  * @brief  Return PLL clock frequency used for SPDIFRX clock
  * @retval PLL clock frequency (in Hz)
  */
uint32_t RCC_PLL_GetFreqDomain_SPDIFRX(void)            /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t pllinputfreq = 0U, pllsource = 0U;           /* 32位无符号变量/函数 */

  /* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLLM) * PLLN
     SPDIFRXCLK = PLL_VCO / PLLR
  */
  pllsource = LL_RCC_PLL_GetMainSource();               /* 赋值或初始化 */

  switch (pllsource)                                    /* switch分支 */
  {                                                     /* 代码块开始 */
    case LL_RCC_PLLSOURCE_HSE:  /* HSE used as PLL clock source */
      pllinputfreq = HSE_VALUE;                         /* 赋值或初始化 */
      break;                                            /* 跳出 */

    case LL_RCC_PLLSOURCE_HSI:  /* HSI used as PLL clock source */
    default:                                            /* default分支 */
      pllinputfreq = HSI_VALUE;                         /* 赋值或初始化 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */
  return __LL_RCC_CALC_PLLCLK_SPDIFRX_FREQ(pllinputfreq, LL_RCC_PLL_GetDivider(),  /* 返回值 */
                                        LL_RCC_PLL_GetN(), LL_RCC_PLL_GetR());  /* 执行语句 */
}                                                       /* 代码块结束 */
#endif /* SPDIFRX */

#if defined(RCC_PLLCFGR_PLLR)                           /* 条件编译 */
#if defined(SAI1)                                       /* 条件编译 */
/**
  * @brief  Return PLL clock frequency used for SAI clock
  * @retval PLL clock frequency (in Hz)
  */
uint32_t RCC_PLL_GetFreqDomain_SAI(void)                /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t pllinputfreq = 0U, pllsource = 0U, plloutputfreq = 0U;  /* 32位无符号变量/函数 */

  /* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLLM) * PLLN
     SAICLK = (PLL_VCO / PLLR) / PLLDIVR
     or
     SAICLK = PLL_VCO / PLLR
  */
  pllsource = LL_RCC_PLL_GetMainSource();               /* 赋值或初始化 */

  switch (pllsource)                                    /* switch分支 */
  {                                                     /* 代码块开始 */
    case LL_RCC_PLLSOURCE_HSE:  /* HSE used as PLL clock source */
      pllinputfreq = HSE_VALUE;                         /* 赋值或初始化 */
      break;                                            /* 跳出 */

    case LL_RCC_PLLSOURCE_HSI:  /* HSI used as PLL clock source */
    default:                                            /* default分支 */
      pllinputfreq = HSI_VALUE;                         /* 赋值或初始化 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

#if defined(RCC_DCKCFGR_PLLDIVR)                        /* 条件编译 */
  plloutputfreq = __LL_RCC_CALC_PLLCLK_SAI_FREQ(pllinputfreq, LL_RCC_PLL_GetDivider(),  /* 列表续行 */
                                        LL_RCC_PLL_GetN(), LL_RCC_PLL_GetR(), LL_RCC_PLL_GetDIVR());  /* 执行语句 */
#else                                                   /* 条件编译否则 */
  plloutputfreq = __LL_RCC_CALC_PLLCLK_SAI_FREQ(pllinputfreq, LL_RCC_PLL_GetDivider(),  /* 列表续行 */
                                        LL_RCC_PLL_GetN(), LL_RCC_PLL_GetR());  /* 执行语句 */
#endif /* RCC_DCKCFGR_PLLDIVR */

  return plloutputfreq;                                 /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* SAI1 */
#endif /* RCC_PLLCFGR_PLLR */

#if defined(RCC_PLLSAI_SUPPORT)                         /* 条件编译 */
/**
  * @brief  Return PLLSAI clock frequency used for SAI domain
  * @retval PLLSAI clock frequency (in Hz)
  */
uint32_t RCC_PLLSAI_GetFreqDomain_SAI(void)             /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t pllinputfreq = 0U, pllsource = 0U;           /* 32位无符号变量/函数 */

  /* PLLSAI_VCO = (HSE_VALUE or HSI_VALUE / PLLSAIM) * PLLSAIN
     SAI domain clock  = (PLLSAI_VCO / PLLSAIQ) / PLLSAIDIVQ
  */
  pllsource = LL_RCC_PLL_GetMainSource();               /* 赋值或初始化 */

  switch (pllsource)                                    /* switch分支 */
  {                                                     /* 代码块开始 */
    case LL_RCC_PLLSOURCE_HSI:  /* HSI used as PLLSAI clock source */
      pllinputfreq = HSI_VALUE;                         /* 赋值或初始化 */
      break;                                            /* 跳出 */

    case LL_RCC_PLLSOURCE_HSE:  /* HSE used as PLLSAI clock source */
      pllinputfreq = HSE_VALUE;                         /* 赋值或初始化 */
      break;                                            /* 跳出 */

    default:                                            /* default分支 */
      pllinputfreq = HSI_VALUE;                         /* 赋值或初始化 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */
  return __LL_RCC_CALC_PLLSAI_SAI_FREQ(pllinputfreq, LL_RCC_PLLSAI_GetDivider(),  /* 返回值 */
                                        LL_RCC_PLLSAI_GetN(), LL_RCC_PLLSAI_GetQ(), LL_RCC_PLLSAI_GetDIVQ());  /* 执行语句 */
}                                                       /* 代码块结束 */

#if defined(RCC_PLLSAICFGR_PLLSAIP)                     /* 条件编译 */
/**
  * @brief  Return PLLSAI clock frequency used for 48Mhz domain
  * @retval PLLSAI clock frequency (in Hz)
  */
uint32_t RCC_PLLSAI_GetFreqDomain_48M(void)             /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t pllinputfreq = 0U, pllsource = 0U;           /* 32位无符号变量/函数 */

  /* PLLSAI_VCO = (HSE_VALUE or HSI_VALUE / PLLSAIM) * PLLSAIN
     48M Domain clock  = PLLSAI_VCO / PLLSAIP
  */
  pllsource = LL_RCC_PLL_GetMainSource();               /* 赋值或初始化 */

  switch (pllsource)                                    /* switch分支 */
  {                                                     /* 代码块开始 */
    case LL_RCC_PLLSOURCE_HSI:  /* HSI used as PLLSAI clock source */
      pllinputfreq = HSI_VALUE;                         /* 赋值或初始化 */
      break;                                            /* 跳出 */

    case LL_RCC_PLLSOURCE_HSE:  /* HSE used as PLLSAI clock source */
      pllinputfreq = HSE_VALUE;                         /* 赋值或初始化 */
      break;                                            /* 跳出 */

    default:                                            /* default分支 */
      pllinputfreq = HSI_VALUE;                         /* 赋值或初始化 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */
  return __LL_RCC_CALC_PLLSAI_48M_FREQ(pllinputfreq, LL_RCC_PLLSAI_GetDivider(),  /* 返回值 */
                                        LL_RCC_PLLSAI_GetN(), LL_RCC_PLLSAI_GetP());  /* 执行语句 */
}                                                       /* 代码块结束 */
#endif /* RCC_PLLSAICFGR_PLLSAIP */

#if defined(LTDC)                                       /* 条件编译 */
/**
  * @brief  Return PLLSAI clock frequency used for LTDC domain
  * @retval PLLSAI clock frequency (in Hz)
  */
uint32_t RCC_PLLSAI_GetFreqDomain_LTDC(void)            /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t pllinputfreq = 0U, pllsource = 0U;           /* 32位无符号变量/函数 */

  /* PLLSAI_VCO = (HSE_VALUE or HSI_VALUE / PLLSAIM) * PLLSAIN
     LTDC Domain clock  = (PLLSAI_VCO / PLLSAIR) / PLLSAIDIVR
  */
  pllsource = LL_RCC_PLL_GetMainSource();               /* 赋值或初始化 */

  switch (pllsource)                                    /* switch分支 */
  {                                                     /* 代码块开始 */
    case LL_RCC_PLLSOURCE_HSI:  /* HSI used as PLLSAI clock source */
      pllinputfreq = HSI_VALUE;                         /* 赋值或初始化 */
      break;                                            /* 跳出 */

    case LL_RCC_PLLSOURCE_HSE:  /* HSE used as PLLSAI clock source */
      pllinputfreq = HSE_VALUE;                         /* 赋值或初始化 */
      break;                                            /* 跳出 */

    default:                                            /* default分支 */
      pllinputfreq = HSI_VALUE;                         /* 赋值或初始化 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */
  return __LL_RCC_CALC_PLLSAI_LTDC_FREQ(pllinputfreq, LL_RCC_PLLSAI_GetDivider(),  /* 返回值 */
                                        LL_RCC_PLLSAI_GetN(), LL_RCC_PLLSAI_GetR(), LL_RCC_PLLSAI_GetDIVR());  /* 执行语句 */
}                                                       /* 代码块结束 */
#endif /* LTDC */
#endif /* RCC_PLLSAI_SUPPORT */

#if defined(RCC_PLLI2S_SUPPORT)                         /* 条件编译 */
#if defined(SAI1)                                       /* 条件编译 */
/**
  * @brief  Return PLLI2S clock frequency used for SAI domains
  * @retval PLLI2S clock frequency (in Hz)
  */
uint32_t RCC_PLLI2S_GetFreqDomain_SAI(void)             /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t plli2sinputfreq = 0U, plli2ssource = 0U, plli2soutputfreq = 0U;  /* 32位无符号变量/函数 */

  /* PLLI2S_VCO = (HSE_VALUE or HSI_VALUE / PLLI2SM) * PLLI2SN
     SAI domain clock  = (PLLI2S_VCO / PLLI2SQ) / PLLI2SDIVQ
     or
     SAI domain clock  = (PLLI2S_VCO / PLLI2SR) / PLLI2SDIVR
  */
  plli2ssource = LL_RCC_PLLI2S_GetMainSource();         /* 赋值或初始化 */

  switch (plli2ssource)                                 /* switch分支 */
  {                                                     /* 代码块开始 */
    case LL_RCC_PLLSOURCE_HSE:     /* HSE used as PLLI2S clock source */
      plli2sinputfreq = HSE_VALUE;                      /* 赋值或初始化 */
      break;                                            /* 跳出 */

#if defined(RCC_PLLI2SCFGR_PLLI2SSRC)                   /* 条件编译 */
    case LL_RCC_PLLI2SSOURCE_PIN:  /* External pin input clock used as PLLI2S clock source */
      plli2sinputfreq = EXTERNAL_CLOCK_VALUE;           /* 赋值或初始化 */
      break;                                            /* 跳出 */
#endif /* RCC_PLLI2SCFGR_PLLI2SSRC */

    case LL_RCC_PLLSOURCE_HSI:     /* HSI used as PLLI2S clock source */
    default:                                            /* default分支 */
      plli2sinputfreq = HSI_VALUE;                      /* 赋值或初始化 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

#if defined(RCC_DCKCFGR_PLLI2SDIVQ)                     /* 条件编译 */
  plli2soutputfreq = __LL_RCC_CALC_PLLI2S_SAI_FREQ(plli2sinputfreq, LL_RCC_PLLI2S_GetDivider(),  /* 列表续行 */
                                          LL_RCC_PLLI2S_GetN(), LL_RCC_PLLI2S_GetQ(), LL_RCC_PLLI2S_GetDIVQ());  /* 执行语句 */
#else                                                   /* 条件编译否则 */
  plli2soutputfreq = __LL_RCC_CALC_PLLI2S_SAI_FREQ(plli2sinputfreq, LL_RCC_PLLI2S_GetDivider(),  /* 列表续行 */
                                          LL_RCC_PLLI2S_GetN(), LL_RCC_PLLI2S_GetR(), LL_RCC_PLLI2S_GetDIVR());  /* 执行语句 */
#endif /* RCC_DCKCFGR_PLLI2SDIVQ */

  return plli2soutputfreq;                              /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* SAI1 */

#if defined(SPDIFRX)                                    /* 条件编译 */
/**
  * @brief  Return PLLI2S clock frequency used for SPDIFRX domain
  * @retval PLLI2S clock frequency (in Hz)
  */
uint32_t RCC_PLLI2S_GetFreqDomain_SPDIFRX(void)         /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t pllinputfreq = 0U, pllsource = 0U;           /* 32位无符号变量/函数 */

  /* PLLI2S_VCO = (HSE_VALUE or HSI_VALUE / PLLI2SM) * PLLI2SN
     SPDIFRX Domain clock  = PLLI2S_VCO / PLLI2SP
  */
  pllsource = LL_RCC_PLLI2S_GetMainSource();            /* 赋值或初始化 */

  switch (pllsource)                                    /* switch分支 */
  {                                                     /* 代码块开始 */
    case LL_RCC_PLLSOURCE_HSE:  /* HSE used as PLLI2S clock source */
      pllinputfreq = HSE_VALUE;                         /* 赋值或初始化 */
      break;                                            /* 跳出 */

    case LL_RCC_PLLSOURCE_HSI:  /* HSI used as PLLI2S clock source */
    default:                                            /* default分支 */
      pllinputfreq = HSI_VALUE;                         /* 赋值或初始化 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  return __LL_RCC_CALC_PLLI2S_SPDIFRX_FREQ(pllinputfreq, LL_RCC_PLLI2S_GetDivider(),  /* 返回值 */
                                           LL_RCC_PLLI2S_GetN(), LL_RCC_PLLI2S_GetP());  /* 执行语句 */
}                                                       /* 代码块结束 */
#endif /* SPDIFRX */

/**
  * @brief  Return PLLI2S clock frequency used for I2S domain
  * @retval PLLI2S clock frequency (in Hz)
  */
uint32_t RCC_PLLI2S_GetFreqDomain_I2S(void)             /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t plli2sinputfreq = 0U, plli2ssource = 0U, plli2soutputfreq = 0U;  /* 32位无符号变量/函数 */

  /* PLLI2S_VCO = (HSE_VALUE or HSI_VALUE / PLLI2SM) * PLLI2SN
     I2S Domain clock  = PLLI2S_VCO / PLLI2SR
  */
  plli2ssource = LL_RCC_PLLI2S_GetMainSource();         /* 赋值或初始化 */

  switch (plli2ssource)                                 /* switch分支 */
  {                                                     /* 代码块开始 */
    case LL_RCC_PLLSOURCE_HSE:     /* HSE used as PLLI2S clock source */
      plli2sinputfreq = HSE_VALUE;                      /* 赋值或初始化 */
      break;                                            /* 跳出 */

#if defined(RCC_PLLI2SCFGR_PLLI2SSRC)                   /* 条件编译 */
    case LL_RCC_PLLI2SSOURCE_PIN:  /* External pin input clock used as PLLI2S clock source */
      plli2sinputfreq = EXTERNAL_CLOCK_VALUE;           /* 赋值或初始化 */
      break;                                            /* 跳出 */
#endif /* RCC_PLLI2SCFGR_PLLI2SSRC */

    case LL_RCC_PLLSOURCE_HSI:     /* HSI used as PLLI2S clock source */
    default:                                            /* default分支 */
      plli2sinputfreq = HSI_VALUE;                      /* 赋值或初始化 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  plli2soutputfreq = __LL_RCC_CALC_PLLI2S_I2S_FREQ(plli2sinputfreq, LL_RCC_PLLI2S_GetDivider(),  /* 列表续行 */
                                                   LL_RCC_PLLI2S_GetN(), LL_RCC_PLLI2S_GetR());  /* 执行语句 */

  return plli2soutputfreq;                              /* 返回值 */
}                                                       /* 代码块结束 */

#if defined(RCC_PLLI2SCFGR_PLLI2SQ) && !defined(RCC_DCKCFGR_PLLI2SDIVQ)  /* 条件编译 */
/**
  * @brief  Return PLLI2S clock frequency used for 48Mhz domain
  * @retval PLLI2S clock frequency (in Hz)
  */
uint32_t RCC_PLLI2S_GetFreqDomain_48M(void)             /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t plli2sinputfreq = 0U, plli2ssource = 0U, plli2soutputfreq = 0U;  /* 32位无符号变量/函数 */

  /* PLL48M_VCO = (HSE_VALUE or HSI_VALUE / PLLI2SM) * PLLI2SN
     48M Domain clock  = PLLI2S_VCO / PLLI2SQ
  */
  plli2ssource = LL_RCC_PLLI2S_GetMainSource();         /* 赋值或初始化 */

  switch (plli2ssource)                                 /* switch分支 */
  {                                                     /* 代码块开始 */
    case LL_RCC_PLLSOURCE_HSE:     /* HSE used as PLLI2S clock source */
      plli2sinputfreq = HSE_VALUE;                      /* 赋值或初始化 */
      break;                                            /* 跳出 */

#if defined(RCC_PLLI2SCFGR_PLLI2SSRC)                   /* 条件编译 */
    case LL_RCC_PLLI2SSOURCE_PIN:  /* External pin input clock used as PLLI2S clock source */
      plli2sinputfreq = EXTERNAL_CLOCK_VALUE;           /* 赋值或初始化 */
      break;                                            /* 跳出 */
#endif /* RCC_PLLI2SCFGR_PLLI2SSRC */

    case LL_RCC_PLLSOURCE_HSI:     /* HSI used as PLLI2S clock source */
    default:                                            /* default分支 */
      plli2sinputfreq = HSI_VALUE;                      /* 赋值或初始化 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  plli2soutputfreq = __LL_RCC_CALC_PLLI2S_48M_FREQ(plli2sinputfreq, LL_RCC_PLLI2S_GetDivider(),  /* 列表续行 */
                                                   LL_RCC_PLLI2S_GetN(), LL_RCC_PLLI2S_GetQ());  /* 执行语句 */

  return plli2soutputfreq;                              /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* RCC_PLLI2SCFGR_PLLI2SQ && !RCC_DCKCFGR_PLLI2SDIVQ */
#endif /* RCC_PLLI2S_SUPPORT */
/**
  * @}
  */

/**
  * @}
  */

#endif /* defined(RCC) */

/**
  * @}
  */

#endif /* USE_FULL_LL_DRIVER */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
