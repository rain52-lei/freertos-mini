/* 文件: stm32f4xx_hal_rcc.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_rcc.c
  * @author  MCD Application Team
  * @brief   RCC HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Reset and Clock Control (RCC) peripheral:
  *           + Initialization and de-initialization functions
  *           + Peripheral Control functions
  *
  @verbatim
  ==============================================================================
                      ##### RCC specific features #####
  ==============================================================================
    [..]
      After reset the device is running from Internal High Speed oscillator
      (HSI 16MHz) with Flash 0 wait state, Flash prefetch buffer, D-Cache
      and I-Cache are disabled, and all peripherals are off except internal
      SRAM, Flash and JTAG.
      (+) There is no prescaler on High speed (AHB) and Low speed (APB) busses;
          all peripherals mapped on these busses are running at HSI speed.
      (+) The clock for all peripherals is switched off, except the SRAM and FLASH.
      (+) All GPIOs are in input floating state, except the JTAG pins which
          are assigned to be used for debug purpose.

    [..]
      Once the device started from reset, the user application has to:
      (+) Configure the clock source to be used to drive the System clock
          (if the application needs higher frequency/performance)
      (+) Configure the System clock frequency and Flash settings
      (+) Configure the AHB and APB busses prescalers
      (+) Enable the clock for the peripheral(s) to be used
      (+) Configure the clock source(s) for peripherals which clocks are not
          derived from the System clock (I2S, RTC, ADC, USB OTG FS/SDIO/RNG)

                      ##### RCC Limitations #####
  ==============================================================================
    [..]
      A delay between an RCC peripheral clock enable and the effective peripheral
      enabling should be taken into account in order to manage the peripheral read/write
      from/to registers.
      (+) This delay depends on the peripheral mapping.
      (+) If peripheral is mapped on AHB: the delay is 2 AHB clock cycle
          after the clock enable bit is set on the hardware register
      (+) If peripheral is mapped on APB: the delay is 2 APB clock cycle
          after the clock enable bit is set on the hardware register

    [..]
      Implemented Workaround:
      (+) For AHB & APB peripherals, a dummy read to the peripheral register has been
          inserted in each __HAL_RCC_PPP_CLK_ENABLE() macro.

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

/** @defgroup RCC RCC
  * @brief RCC HAL module driver
  * @{
  */

#ifdef HAL_RCC_MODULE_ENABLED                           /* 条件编译 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @addtogroup RCC_Private_Constants
  * @{
  */

/* Private macro -------------------------------------------------------------*/
#define __MCO1_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()  /* 定义包含保护宏 */
#define MCO1_GPIO_PORT        GPIOA                     /* 定义宏 */
#define MCO1_PIN              GPIO_PIN_8                /* 定义宏 */

#define __MCO2_CLK_ENABLE()   __HAL_RCC_GPIOC_CLK_ENABLE()  /* 定义包含保护宏 */
#define MCO2_GPIO_PORT         GPIOC                    /* 定义宏 */
#define MCO2_PIN               GPIO_PIN_9               /* 定义宏 */
/**
  * @}
  */

/* Private variables ---------------------------------------------------------*/
/** @defgroup RCC_Private_Variables RCC Private Variables
  * @{
  */
/**
  * @}
  */
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup RCC_Exported_Functions RCC Exported Functions
  *  @{
  */

/** @defgroup RCC_Exported_Functions_Group1 Initialization and de-initialization functions
 *  @brief    Initialization and Configuration functions
 *
@verbatim
 ===============================================================================
           ##### Initialization and de-initialization functions #####
 ===============================================================================
    [..]
      This section provides functions allowing to configure the internal/external oscillators
      (HSE, HSI, LSE, LSI, PLL, CSS and MCO) and the System busses clocks (SYSCLK, AHB, APB1
       and APB2).

    [..] Internal/external clock and PLL configuration
         (#) HSI (high-speed internal), 16 MHz factory-trimmed RC used directly or through
             the PLL as System clock source.

         (#) LSI (low-speed internal), 32 KHz low consumption RC used as IWDG and/or RTC
             clock source.

         (#) HSE (high-speed external), 4 to 26 MHz crystal oscillator used directly or
             through the PLL as System clock source. Can be used also as RTC clock source.

         (#) LSE (low-speed external), 32 KHz oscillator used as RTC clock source.

         (#) PLL (clocked by HSI or HSE), featuring two different output clocks:
           (++) The first output is used to generate the high speed system clock (up to 168 MHz)
           (++) The second output is used to generate the clock for the USB OTG FS (48 MHz),
                the random analog generator (<=48 MHz) and the SDIO (<= 48 MHz).

         (#) CSS (Clock security system), once enable using the macro __HAL_RCC_CSS_ENABLE()
             and if a HSE clock failure occurs(HSE used directly or through PLL as System
             clock source), the System clocks automatically switched to HSI and an interrupt
             is generated if enabled. The interrupt is linked to the Cortex-M4 NMI
             (Non-Maskable Interrupt) exception vector.

         (#) MCO1 (microcontroller clock output), used to output HSI, LSE, HSE or PLL
             clock (through a configurable prescaler) on PA8 pin.

         (#) MCO2 (microcontroller clock output), used to output HSE, PLL, SYSCLK or PLLI2S
             clock (through a configurable prescaler) on PC9 pin.

    [..] System, AHB and APB busses clocks configuration
         (#) Several clock sources can be used to drive the System clock (SYSCLK): HSI,
             HSE and PLL.
             The AHB clock (HCLK) is derived from System clock through configurable
             prescaler and used to clock the CPU, memory and peripherals mapped
             on AHB bus (DMA, GPIO...). APB1 (PCLK1) and APB2 (PCLK2) clocks are derived
             from AHB clock through configurable prescalers and used to clock
             the peripherals mapped on these busses. You can use
             "HAL_RCC_GetSysClockFreq()" function to retrieve the frequencies of these clocks.

         (#) For the STM32F405xx/07xx and STM32F415xx/17xx devices, the maximum
             frequency of the SYSCLK and HCLK is 168 MHz, PCLK2 84 MHz and PCLK1 42 MHz.
             Depending on the device voltage range, the maximum frequency should
             be adapted accordingly (refer to the product datasheets for more details).

         (#) For the STM32F42xxx, STM32F43xxx, STM32F446xx, STM32F469xx and STM32F479xx devices,
             the maximum frequency of the SYSCLK and HCLK is 180 MHz, PCLK2 90 MHz and PCLK1 45 MHz.
             Depending on the device voltage range, the maximum frequency should
             be adapted accordingly (refer to the product datasheets for more details).

         (#) For the STM32F401xx, the maximum frequency of the SYSCLK and HCLK is 84 MHz,
             PCLK2 84 MHz and PCLK1 42 MHz.
             Depending on the device voltage range, the maximum frequency should
             be adapted accordingly (refer to the product datasheets for more details).

         (#) For the STM32F41xxx, the maximum frequency of the SYSCLK and HCLK is 100 MHz,
             PCLK2 100 MHz and PCLK1 50 MHz.
             Depending on the device voltage range, the maximum frequency should
             be adapted accordingly (refer to the product datasheets for more details).

@endverbatim
  * @{
  */

/**
  * @brief  Resets the RCC clock configuration to the default reset state.
  * @note   The default reset state of the clock configuration is given below:
  *            - HSI ON and used as system clock source
  *            - HSE and PLL OFF
  *            - AHB, APB1 and APB2 prescaler set to 1.
  *            - CSS, MCO1 and MCO2 OFF
  *            - All interrupts disabled
  * @note   This function doesn't modify the configuration of the
  *            - Peripheral clocks
  *            - LSI, LSE and RTC clocks
  * @retval HAL status
  */
__weak HAL_StatusTypeDef HAL_RCC_DeInit(void)           /* 源代码 */
{                                                       /* 代码块开始 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initializes the RCC Oscillators according to the specified parameters in the
  *         RCC_OscInitTypeDef.
  * @param  RCC_OscInitStruct pointer to an RCC_OscInitTypeDef structure that
  *         contains the configuration information for the RCC Oscillators.
  * @note   The PLL is not disabled when used as system clock.
  * @note   Transitions LSE Bypass to LSE On and LSE On to LSE Bypass are not
  *         supported by this API. User should request a transition to LSE Off
  *         first and then LSE On or LSE Bypass.
  * @note   Transition HSE Bypass to HSE On and HSE On to HSE Bypass are not
  *         supported by this API. User should request a transition to HSE Off
  *         first and then HSE On or HSE Bypass.
  * @retval HAL status
  */
__weak HAL_StatusTypeDef HAL_RCC_OscConfig(RCC_OscInitTypeDef  *RCC_OscInitStruct)  /* 源代码 */
{                                                       /* 代码块开始 */
  uint32_t tickstart, pll_config;                       /* 32位无符号变量/函数 */

  /* Check Null pointer */
  if(RCC_OscInitStruct == NULL)                         /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the parameters */
  assert_param(IS_RCC_OSCILLATORTYPE(RCC_OscInitStruct->OscillatorType));  /* 执行语句 */
  /*------------------------------- HSE Configuration ------------------------*/
  if(((RCC_OscInitStruct->OscillatorType) & RCC_OSCILLATORTYPE_HSE) == RCC_OSCILLATORTYPE_HSE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check the parameters */
    assert_param(IS_RCC_HSE(RCC_OscInitStruct->HSEState));  /* 执行语句 */
    /* When the HSE is used as system clock or clock source for PLL in these cases HSE will not disabled */
    if((__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_CFGR_SWS_HSE) ||\
      ((__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_CFGR_SWS_PLL) && ((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLCFGR_PLLSRC_HSE)))  /* 源代码 */
    {                                                   /* 代码块开始 */
      if((__HAL_RCC_GET_FLAG(RCC_FLAG_HSERDY) != RESET) && (RCC_OscInitStruct->HSEState == RCC_HSE_OFF))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Set the new HSE configuration ---------------------------------------*/
      __HAL_RCC_HSE_CONFIG(RCC_OscInitStruct->HSEState);  /* HAL辅助宏/函数 */

      /* Check the HSE State */
      if((RCC_OscInitStruct->HSEState) != RCC_HSE_OFF)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Get Start Tick */
        tickstart = HAL_GetTick();                      /* 赋值或初始化 */

        /* Wait till HSE is ready */
        while(__HAL_RCC_GET_FLAG(RCC_FLAG_HSERDY) == RESET)  /* while循环 */
        {                                               /* 代码块开始 */
          if((HAL_GetTick() - tickstart ) > HSE_TIMEOUT_VALUE)  /* 条件判断 */
          {                                             /* 代码块开始 */
            return HAL_TIMEOUT;                         /* 返回值 */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Get Start Tick */
        tickstart = HAL_GetTick();                      /* 赋值或初始化 */

        /* Wait till HSE is bypassed or disabled */
        while(__HAL_RCC_GET_FLAG(RCC_FLAG_HSERDY) != RESET)  /* while循环 */
        {                                               /* 代码块开始 */
          if((HAL_GetTick() - tickstart ) > HSE_TIMEOUT_VALUE)  /* 条件判断 */
          {                                             /* 代码块开始 */
            return HAL_TIMEOUT;                         /* 返回值 */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /*----------------------------- HSI Configuration --------------------------*/
  if(((RCC_OscInitStruct->OscillatorType) & RCC_OSCILLATORTYPE_HSI) == RCC_OSCILLATORTYPE_HSI)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check the parameters */
    assert_param(IS_RCC_HSI(RCC_OscInitStruct->HSIState));  /* 执行语句 */
    assert_param(IS_RCC_CALIBRATION_VALUE(RCC_OscInitStruct->HSICalibrationValue));  /* 执行语句 */

    /* Check if HSI is used as system clock or as PLL source when PLL is selected as system clock */
    if((__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_CFGR_SWS_HSI) ||\
      ((__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_CFGR_SWS_PLL) && ((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLCFGR_PLLSRC_HSI)))  /* 源代码 */
    {                                                   /* 代码块开始 */
      /* When HSI is used as system clock it will not disabled */
      if((__HAL_RCC_GET_FLAG(RCC_FLAG_HSIRDY) != RESET) && (RCC_OscInitStruct->HSIState != RCC_HSI_ON))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
      /* Otherwise, just the calibration is allowed */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Adjusts the Internal High Speed oscillator (HSI) calibration value.*/
        __HAL_RCC_HSI_CALIBRATIONVALUE_ADJUST(RCC_OscInitStruct->HSICalibrationValue);  /* HAL辅助宏/函数 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Check the HSI State */
      if((RCC_OscInitStruct->HSIState)!= RCC_HSI_OFF)   /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Enable the Internal High Speed oscillator (HSI). */
        __HAL_RCC_HSI_ENABLE();                         /* HAL辅助宏/函数 */

        /* Get Start Tick*/
        tickstart = HAL_GetTick();                      /* 赋值或初始化 */

        /* Wait till HSI is ready */
        while(__HAL_RCC_GET_FLAG(RCC_FLAG_HSIRDY) == RESET)  /* while循环 */
        {                                               /* 代码块开始 */
          if((HAL_GetTick() - tickstart ) > HSI_TIMEOUT_VALUE)  /* 条件判断 */
          {                                             /* 代码块开始 */
            return HAL_TIMEOUT;                         /* 返回值 */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */

        /* Adjusts the Internal High Speed oscillator (HSI) calibration value. */
        __HAL_RCC_HSI_CALIBRATIONVALUE_ADJUST(RCC_OscInitStruct->HSICalibrationValue);  /* HAL辅助宏/函数 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Disable the Internal High Speed oscillator (HSI). */
        __HAL_RCC_HSI_DISABLE();                        /* HAL辅助宏/函数 */

        /* Get Start Tick*/
        tickstart = HAL_GetTick();                      /* 赋值或初始化 */

        /* Wait till HSI is ready */
        while(__HAL_RCC_GET_FLAG(RCC_FLAG_HSIRDY) != RESET)  /* while循环 */
        {                                               /* 代码块开始 */
          if((HAL_GetTick() - tickstart ) > HSI_TIMEOUT_VALUE)  /* 条件判断 */
          {                                             /* 代码块开始 */
            return HAL_TIMEOUT;                         /* 返回值 */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /*------------------------------ LSI Configuration -------------------------*/
  if(((RCC_OscInitStruct->OscillatorType) & RCC_OSCILLATORTYPE_LSI) == RCC_OSCILLATORTYPE_LSI)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check the parameters */
    assert_param(IS_RCC_LSI(RCC_OscInitStruct->LSIState));  /* 执行语句 */

    /* Check the LSI State */
    if((RCC_OscInitStruct->LSIState)!= RCC_LSI_OFF)     /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable the Internal Low Speed oscillator (LSI). */
      __HAL_RCC_LSI_ENABLE();                           /* HAL辅助宏/函数 */

      /* Get Start Tick*/
      tickstart = HAL_GetTick();                        /* 赋值或初始化 */

      /* Wait till LSI is ready */
      while(__HAL_RCC_GET_FLAG(RCC_FLAG_LSIRDY) == RESET)  /* while循环 */
      {                                                 /* 代码块开始 */
        if((HAL_GetTick() - tickstart ) > LSI_TIMEOUT_VALUE)  /* 条件判断 */
        {                                               /* 代码块开始 */
          return HAL_TIMEOUT;                           /* 返回值 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Disable the Internal Low Speed oscillator (LSI). */
      __HAL_RCC_LSI_DISABLE();                          /* HAL辅助宏/函数 */

      /* Get Start Tick */
      tickstart = HAL_GetTick();                        /* 赋值或初始化 */

      /* Wait till LSI is ready */
      while(__HAL_RCC_GET_FLAG(RCC_FLAG_LSIRDY) != RESET)  /* while循环 */
      {                                                 /* 代码块开始 */
        if((HAL_GetTick() - tickstart ) > LSI_TIMEOUT_VALUE)  /* 条件判断 */
        {                                               /* 代码块开始 */
          return HAL_TIMEOUT;                           /* 返回值 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /*------------------------------ LSE Configuration -------------------------*/
  if(((RCC_OscInitStruct->OscillatorType) & RCC_OSCILLATORTYPE_LSE) == RCC_OSCILLATORTYPE_LSE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    FlagStatus       pwrclkchanged = RESET;             /* 赋值或初始化 */

    /* Check the parameters */
    assert_param(IS_RCC_LSE(RCC_OscInitStruct->LSEState));  /* 执行语句 */

    /* Update LSE configuration in Backup Domain control register    */
    /* Requires to enable write access to Backup Domain of necessary */
    if(__HAL_RCC_PWR_IS_CLK_DISABLED())                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      __HAL_RCC_PWR_CLK_ENABLE();                       /* HAL辅助宏/函数 */
      pwrclkchanged = SET;                              /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    if(HAL_IS_BIT_CLR(PWR->CR, PWR_CR_DBP))             /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable write access to Backup domain */
      SET_BIT(PWR->CR, PWR_CR_DBP);                     /* 执行语句 */

      /* Wait for Backup domain Write protection disable */
      tickstart = HAL_GetTick();                        /* 赋值或初始化 */

      while(HAL_IS_BIT_CLR(PWR->CR, PWR_CR_DBP))        /* while循环 */
      {                                                 /* 代码块开始 */
        if((HAL_GetTick() - tickstart) > RCC_DBP_TIMEOUT_VALUE)  /* 条件判断 */
        {                                               /* 代码块开始 */
          return HAL_TIMEOUT;                           /* 返回值 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Set the new LSE configuration -----------------------------------------*/
    __HAL_RCC_LSE_CONFIG(RCC_OscInitStruct->LSEState);  /* HAL辅助宏/函数 */
    /* Check the LSE State */
    if((RCC_OscInitStruct->LSEState) != RCC_LSE_OFF)    /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Get Start Tick*/
      tickstart = HAL_GetTick();                        /* 赋值或初始化 */

      /* Wait till LSE is ready */
      while(__HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY) == RESET)  /* while循环 */
      {                                                 /* 代码块开始 */
        if((HAL_GetTick() - tickstart ) > RCC_LSE_TIMEOUT_VALUE)  /* 条件判断 */
        {                                               /* 代码块开始 */
          return HAL_TIMEOUT;                           /* 返回值 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Get Start Tick */
      tickstart = HAL_GetTick();                        /* 赋值或初始化 */

      /* Wait till LSE is ready */
      while(__HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY) != RESET)  /* while循环 */
      {                                                 /* 代码块开始 */
        if((HAL_GetTick() - tickstart ) > RCC_LSE_TIMEOUT_VALUE)  /* 条件判断 */
        {                                               /* 代码块开始 */
          return HAL_TIMEOUT;                           /* 返回值 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Restore clock configuration if changed */
    if(pwrclkchanged == SET)                            /* 条件判断 */
    {                                                   /* 代码块开始 */
      __HAL_RCC_PWR_CLK_DISABLE();                      /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /*-------------------------------- PLL Configuration -----------------------*/
  /* Check the parameters */
  assert_param(IS_RCC_PLL(RCC_OscInitStruct->PLL.PLLState));  /* 执行语句 */
  if ((RCC_OscInitStruct->PLL.PLLState) != RCC_PLL_NONE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check if the PLL is used as system clock or not */
    if(__HAL_RCC_GET_SYSCLK_SOURCE() != RCC_CFGR_SWS_PLL)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      if((RCC_OscInitStruct->PLL.PLLState) == RCC_PLL_ON)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Check the parameters */
        assert_param(IS_RCC_PLLSOURCE(RCC_OscInitStruct->PLL.PLLSource));  /* 执行语句 */
        assert_param(IS_RCC_PLLM_VALUE(RCC_OscInitStruct->PLL.PLLM));  /* 执行语句 */
        assert_param(IS_RCC_PLLN_VALUE(RCC_OscInitStruct->PLL.PLLN));  /* 执行语句 */
        assert_param(IS_RCC_PLLP_VALUE(RCC_OscInitStruct->PLL.PLLP));  /* 执行语句 */
        assert_param(IS_RCC_PLLQ_VALUE(RCC_OscInitStruct->PLL.PLLQ));  /* 执行语句 */

        /* Disable the main PLL. */
        __HAL_RCC_PLL_DISABLE();                        /* HAL辅助宏/函数 */

        /* Get Start Tick */
        tickstart = HAL_GetTick();                      /* 赋值或初始化 */

        /* Wait till PLL is ready */
        while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLRDY) != RESET)  /* while循环 */
        {                                               /* 代码块开始 */
          if((HAL_GetTick() - tickstart ) > PLL_TIMEOUT_VALUE)  /* 条件判断 */
          {                                             /* 代码块开始 */
            return HAL_TIMEOUT;                         /* 返回值 */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */

        /* Configure the main PLL clock source, multiplication and division factors. */
        WRITE_REG(RCC->PLLCFGR, (RCC_OscInitStruct->PLL.PLLSource                                            | \
                                 RCC_OscInitStruct->PLL.PLLM                                                 | \
                                 (RCC_OscInitStruct->PLL.PLLN << RCC_PLLCFGR_PLLN_Pos)             | \
                                 (((RCC_OscInitStruct->PLL.PLLP >> 1U) - 1U) << RCC_PLLCFGR_PLLP_Pos) | \
                                 (RCC_OscInitStruct->PLL.PLLQ << RCC_PLLCFGR_PLLQ_Pos)));  /* 执行语句 */
        /* Enable the main PLL. */
        __HAL_RCC_PLL_ENABLE();                         /* HAL辅助宏/函数 */

        /* Get Start Tick */
        tickstart = HAL_GetTick();                      /* 赋值或初始化 */

        /* Wait till PLL is ready */
        while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLRDY) == RESET)  /* while循环 */
        {                                               /* 代码块开始 */
          if((HAL_GetTick() - tickstart ) > PLL_TIMEOUT_VALUE)  /* 条件判断 */
          {                                             /* 代码块开始 */
            return HAL_TIMEOUT;                         /* 返回值 */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Disable the main PLL. */
        __HAL_RCC_PLL_DISABLE();                        /* HAL辅助宏/函数 */

        /* Get Start Tick */
        tickstart = HAL_GetTick();                      /* 赋值或初始化 */

        /* Wait till PLL is ready */
        while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLRDY) != RESET)  /* while循环 */
        {                                               /* 代码块开始 */
          if((HAL_GetTick() - tickstart ) > PLL_TIMEOUT_VALUE)  /* 条件判断 */
          {                                             /* 代码块开始 */
            return HAL_TIMEOUT;                         /* 返回值 */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Check if there is a request to disable the PLL used as System clock source */
      if((RCC_OscInitStruct->PLL.PLLState) == RCC_PLL_OFF)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Do not return HAL_ERROR if request repeats the current configuration */
        pll_config = RCC->PLLCFGR;                      /* 赋值或初始化 */
#if defined (RCC_PLLCFGR_PLLR)                          /* 条件编译 */
        if (((RCC_OscInitStruct->PLL.PLLState) == RCC_PLL_OFF) ||  /* 条件判断 */
            (READ_BIT(pll_config, RCC_PLLCFGR_PLLSRC) != RCC_OscInitStruct->PLL.PLLSource) ||  /* 源代码 */
            (READ_BIT(pll_config, RCC_PLLCFGR_PLLM) != (RCC_OscInitStruct->PLL.PLLM) << RCC_PLLCFGR_PLLM_Pos) ||  /* 源代码 */
            (READ_BIT(pll_config, RCC_PLLCFGR_PLLN) != (RCC_OscInitStruct->PLL.PLLN) << RCC_PLLCFGR_PLLN_Pos) ||  /* 源代码 */
            (READ_BIT(pll_config, RCC_PLLCFGR_PLLP) != (((RCC_OscInitStruct->PLL.PLLP >> 1U) - 1U)) << RCC_PLLCFGR_PLLP_Pos) ||  /* 源代码 */
            (READ_BIT(pll_config, RCC_PLLCFGR_PLLQ) != (RCC_OscInitStruct->PLL.PLLQ << RCC_PLLCFGR_PLLQ_Pos)) ||  /* 源代码 */
            (READ_BIT(pll_config, RCC_PLLCFGR_PLLR) != (RCC_OscInitStruct->PLL.PLLR << RCC_PLLCFGR_PLLR_Pos)))  /* 源代码 */
#else                                                   /* 条件编译否则 */
        if (((RCC_OscInitStruct->PLL.PLLState) == RCC_PLL_OFF) ||  /* 条件判断 */
            (READ_BIT(pll_config, RCC_PLLCFGR_PLLSRC) != RCC_OscInitStruct->PLL.PLLSource) ||  /* 源代码 */
            (READ_BIT(pll_config, RCC_PLLCFGR_PLLM) != (RCC_OscInitStruct->PLL.PLLM) << RCC_PLLCFGR_PLLM_Pos) ||  /* 源代码 */
            (READ_BIT(pll_config, RCC_PLLCFGR_PLLN) != (RCC_OscInitStruct->PLL.PLLN) << RCC_PLLCFGR_PLLN_Pos) ||  /* 源代码 */
            (READ_BIT(pll_config, RCC_PLLCFGR_PLLP) != (((RCC_OscInitStruct->PLL.PLLP >> 1U) - 1U)) << RCC_PLLCFGR_PLLP_Pos) ||  /* 源代码 */
            (READ_BIT(pll_config, RCC_PLLCFGR_PLLQ) != (RCC_OscInitStruct->PLL.PLLQ << RCC_PLLCFGR_PLLQ_Pos)))  /* 源代码 */
#endif                                                  /* 条件编译结束 */
        {                                               /* 代码块开始 */
          return HAL_ERROR;                             /* 返回值 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initializes the CPU, AHB and APB busses clocks according to the specified
  *         parameters in the RCC_ClkInitStruct.
  * @param  RCC_ClkInitStruct pointer to an RCC_OscInitTypeDef structure that
  *         contains the configuration information for the RCC peripheral.
  * @param  FLatency FLASH Latency, this parameter depend on device selected
  *
  * @note   The SystemCoreClock CMSIS variable is used to store System Clock Frequency
  *         and updated by HAL_RCC_GetHCLKFreq() function called within this function
  *
  * @note   The HSI is used (enabled by hardware) as system clock source after
  *         startup from Reset, wake-up from STOP and STANDBY mode, or in case
  *         of failure of the HSE used directly or indirectly as system clock
  *         (if the Clock Security System CSS is enabled).
  *
  * @note   A switch from one clock source to another occurs only if the target
  *         clock source is ready (clock stable after startup delay or PLL locked).
  *         If a clock source which is not yet ready is selected, the switch will
  *         occur when the clock source will be ready.
  *
  * @note   Depending on the device voltage range, the software has to set correctly
  *         HPRE[3:0] bits to ensure that HCLK not exceed the maximum allowed frequency
  *         (for more details refer to section above "Initialization/de-initialization functions")
  * @retval None
  */
HAL_StatusTypeDef HAL_RCC_ClockConfig(RCC_ClkInitTypeDef  *RCC_ClkInitStruct, uint32_t FLatency)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */

  /* Check Null pointer */
  if(RCC_ClkInitStruct == NULL)                         /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the parameters */
  assert_param(IS_RCC_CLOCKTYPE(RCC_ClkInitStruct->ClockType));  /* 执行语句 */
  assert_param(IS_FLASH_LATENCY(FLatency));             /* 执行语句 */

  /* To correctly read data from FLASH memory, the number of wait states (LATENCY)
    must be correctly programmed according to the frequency of the CPU clock
    (HCLK) and the supply voltage of the device. */

  /* Increasing the number of wait states because of higher CPU frequency */
  if(FLatency > __HAL_FLASH_GET_LATENCY())              /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Program the new number of wait states to the LATENCY bits in the FLASH_ACR register */
    __HAL_FLASH_SET_LATENCY(FLatency);                  /* HAL辅助宏/函数 */

    /* Check that the new number of wait states is taken into account to access the Flash
    memory by reading the FLASH_ACR register */
    if(__HAL_FLASH_GET_LATENCY() != FLatency)           /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /*-------------------------- HCLK Configuration --------------------------*/
  if(((RCC_ClkInitStruct->ClockType) & RCC_CLOCKTYPE_HCLK) == RCC_CLOCKTYPE_HCLK)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Set the highest APBx dividers in order to ensure that we do not go through
       a non-spec phase whatever we decrease or increase HCLK. */
    if(((RCC_ClkInitStruct->ClockType) & RCC_CLOCKTYPE_PCLK1) == RCC_CLOCKTYPE_PCLK1)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_HCLK_DIV16);  /* 执行语句 */
    }                                                   /* 代码块结束 */

    if(((RCC_ClkInitStruct->ClockType) & RCC_CLOCKTYPE_PCLK2) == RCC_CLOCKTYPE_PCLK2)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, (RCC_HCLK_DIV16 << 3));  /* 执行语句 */
    }                                                   /* 代码块结束 */

    assert_param(IS_RCC_HCLK(RCC_ClkInitStruct->AHBCLKDivider));  /* 执行语句 */
    MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_ClkInitStruct->AHBCLKDivider);  /* 执行语句 */
  }                                                     /* 代码块结束 */

  /*------------------------- SYSCLK Configuration ---------------------------*/
  if(((RCC_ClkInitStruct->ClockType) & RCC_CLOCKTYPE_SYSCLK) == RCC_CLOCKTYPE_SYSCLK)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    assert_param(IS_RCC_SYSCLKSOURCE(RCC_ClkInitStruct->SYSCLKSource));  /* 执行语句 */

    /* HSE is selected as System Clock Source */
    if(RCC_ClkInitStruct->SYSCLKSource == RCC_SYSCLKSOURCE_HSE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Check the HSE ready flag */
      if(__HAL_RCC_GET_FLAG(RCC_FLAG_HSERDY) == RESET)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    /* PLL is selected as System Clock Source */
    else if((RCC_ClkInitStruct->SYSCLKSource == RCC_SYSCLKSOURCE_PLLCLK)   ||  /* 否则如果 */
            (RCC_ClkInitStruct->SYSCLKSource == RCC_SYSCLKSOURCE_PLLRCLK))  /* 源代码 */
    {                                                   /* 代码块开始 */
      /* Check the PLL ready flag */
      if(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLRDY) == RESET)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    /* HSI is selected as System Clock Source */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Check the HSI ready flag */
      if(__HAL_RCC_GET_FLAG(RCC_FLAG_HSIRDY) == RESET)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    __HAL_RCC_SYSCLK_CONFIG(RCC_ClkInitStruct->SYSCLKSource);  /* HAL辅助宏/函数 */

    /* Get Start Tick */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */

    while (__HAL_RCC_GET_SYSCLK_SOURCE() != (RCC_ClkInitStruct->SYSCLKSource << RCC_CFGR_SWS_Pos))  /* while循环 */
    {                                                   /* 代码块开始 */
      if ((HAL_GetTick() - tickstart) > CLOCKSWITCH_TIMEOUT_VALUE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Decreasing the number of wait states because of lower CPU frequency */
  if(FLatency < __HAL_FLASH_GET_LATENCY())              /* 条件判断 */
  {                                                     /* 代码块开始 */
     /* Program the new number of wait states to the LATENCY bits in the FLASH_ACR register */
    __HAL_FLASH_SET_LATENCY(FLatency);                  /* HAL辅助宏/函数 */

    /* Check that the new number of wait states is taken into account to access the Flash
    memory by reading the FLASH_ACR register */
    if(__HAL_FLASH_GET_LATENCY() != FLatency)           /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /*-------------------------- PCLK1 Configuration ---------------------------*/
  if(((RCC_ClkInitStruct->ClockType) & RCC_CLOCKTYPE_PCLK1) == RCC_CLOCKTYPE_PCLK1)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    assert_param(IS_RCC_PCLK(RCC_ClkInitStruct->APB1CLKDivider));  /* 执行语句 */
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_ClkInitStruct->APB1CLKDivider);  /* 执行语句 */
  }                                                     /* 代码块结束 */

  /*-------------------------- PCLK2 Configuration ---------------------------*/
  if(((RCC_ClkInitStruct->ClockType) & RCC_CLOCKTYPE_PCLK2) == RCC_CLOCKTYPE_PCLK2)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    assert_param(IS_RCC_PCLK(RCC_ClkInitStruct->APB2CLKDivider));  /* 执行语句 */
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, ((RCC_ClkInitStruct->APB2CLKDivider) << 3U));  /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Update the SystemCoreClock global variable */
  SystemCoreClock = HAL_RCC_GetSysClockFreq() >> AHBPrescTable[(RCC->CFGR & RCC_CFGR_HPRE)>> RCC_CFGR_HPRE_Pos];  /* 赋值或初始化 */

  /* Configure the source of time base considering new system clocks settings */
  HAL_InitTick (uwTickPrio);                            /* HAL库函数调用 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup RCC_Exported_Functions_Group2 Peripheral Control functions
 *  @brief   RCC clocks control functions
 *
@verbatim
 ===============================================================================
                      ##### Peripheral Control functions #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to control the RCC Clocks
    frequencies.

@endverbatim
  * @{
  */

/**
  * @brief  Selects the clock source to output on MCO1 pin(PA8) or on MCO2 pin(PC9).
  * @note   PA8/PC9 should be configured in alternate function mode.
  * @param  RCC_MCOx specifies the output direction for the clock source.
  *          This parameter can be one of the following values:
  *            @arg RCC_MCO1: Clock source to output on MCO1 pin(PA8).
  *            @arg RCC_MCO2: Clock source to output on MCO2 pin(PC9).
  * @param  RCC_MCOSource specifies the clock source to output.
  *          This parameter can be one of the following values:
  *            @arg RCC_MCO1SOURCE_HSI: HSI clock selected as MCO1 source
  *            @arg RCC_MCO1SOURCE_LSE: LSE clock selected as MCO1 source
  *            @arg RCC_MCO1SOURCE_HSE: HSE clock selected as MCO1 source
  *            @arg RCC_MCO1SOURCE_PLLCLK: main PLL clock selected as MCO1 source
  *            @arg RCC_MCO2SOURCE_SYSCLK: System clock (SYSCLK) selected as MCO2 source
  *            @arg RCC_MCO2SOURCE_PLLI2SCLK: PLLI2S clock selected as MCO2 source, available for all STM32F4 devices except STM32F410xx
  *            @arg RCC_MCO2SOURCE_I2SCLK: I2SCLK clock selected as MCO2 source, available only for STM32F410Rx devices
  *            @arg RCC_MCO2SOURCE_HSE: HSE clock selected as MCO2 source
  *            @arg RCC_MCO2SOURCE_PLLCLK: main PLL clock selected as MCO2 source
  * @param  RCC_MCODiv specifies the MCOx prescaler.
  *          This parameter can be one of the following values:
  *            @arg RCC_MCODIV_1: no division applied to MCOx clock
  *            @arg RCC_MCODIV_2: division by 2 applied to MCOx clock
  *            @arg RCC_MCODIV_3: division by 3 applied to MCOx clock
  *            @arg RCC_MCODIV_4: division by 4 applied to MCOx clock
  *            @arg RCC_MCODIV_5: division by 5 applied to MCOx clock
  * @note  For STM32F410Rx devices to output I2SCLK clock on MCO2 you should have
  *        at last one of the SPI clocks enabled (SPI1, SPI2 or SPI5).
  * @retval None
  */
void HAL_RCC_MCOConfig(uint32_t RCC_MCOx, uint32_t RCC_MCOSource, uint32_t RCC_MCODiv)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  GPIO_InitTypeDef GPIO_InitStruct;                     /* GPIO相关操作 */
  /* Check the parameters */
  assert_param(IS_RCC_MCO(RCC_MCOx));                   /* 执行语句 */
  assert_param(IS_RCC_MCODIV(RCC_MCODiv));              /* 执行语句 */
  /* RCC_MCO1 */
  if(RCC_MCOx == RCC_MCO1)                              /* 条件判断 */
  {                                                     /* 代码块开始 */
    assert_param(IS_RCC_MCO1SOURCE(RCC_MCOSource));     /* 执行语句 */

    /* MCO1 Clock Enable */
    __MCO1_CLK_ENABLE();                                /* 执行语句 */

    /* Configure the MCO1 pin in alternate function mode */
    GPIO_InitStruct.Pin = MCO1_PIN;                     /* GPIO相关操作 */
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;             /* GPIO相关操作 */
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;  /* GPIO相关操作 */
    GPIO_InitStruct.Pull = GPIO_NOPULL;                 /* GPIO相关操作 */
    GPIO_InitStruct.Alternate = GPIO_AF0_MCO;           /* GPIO相关操作 */
    HAL_GPIO_Init(MCO1_GPIO_PORT, &GPIO_InitStruct);    /* HAL GPIO操作 */

    /* Mask MCO1 and MCO1PRE[2:0] bits then Select MCO1 clock source and prescaler */
    MODIFY_REG(RCC->CFGR, (RCC_CFGR_MCO1 | RCC_CFGR_MCO1PRE), (RCC_MCOSource | RCC_MCODiv));  /* 执行语句 */

   /* This RCC MCO1 enable feature is available only on STM32F410xx devices */
#if defined(RCC_CFGR_MCO1EN)                            /* 条件编译 */
    __HAL_RCC_MCO1_ENABLE();                            /* HAL辅助宏/函数 */
#endif /* RCC_CFGR_MCO1EN */
  }                                                     /* 代码块结束 */
#if defined(RCC_CFGR_MCO2)                              /* 条件编译 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    assert_param(IS_RCC_MCO2SOURCE(RCC_MCOSource));     /* 执行语句 */

    /* MCO2 Clock Enable */
    __MCO2_CLK_ENABLE();                                /* 执行语句 */

    /* Configure the MCO2 pin in alternate function mode */
    GPIO_InitStruct.Pin = MCO2_PIN;                     /* GPIO相关操作 */
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;             /* GPIO相关操作 */
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;  /* GPIO相关操作 */
    GPIO_InitStruct.Pull = GPIO_NOPULL;                 /* GPIO相关操作 */
    GPIO_InitStruct.Alternate = GPIO_AF0_MCO;           /* GPIO相关操作 */
    HAL_GPIO_Init(MCO2_GPIO_PORT, &GPIO_InitStruct);    /* HAL GPIO操作 */

    /* Mask MCO2 and MCO2PRE[2:0] bits then Select MCO2 clock source and prescaler */
    MODIFY_REG(RCC->CFGR, (RCC_CFGR_MCO2 | RCC_CFGR_MCO2PRE), (RCC_MCOSource | (RCC_MCODiv << 3U)));  /* 执行语句 */

   /* This RCC MCO2 enable feature is available only on STM32F410Rx devices */
#if defined(RCC_CFGR_MCO2EN)                            /* 条件编译 */
    __HAL_RCC_MCO2_ENABLE();                            /* HAL辅助宏/函数 */
#endif /* RCC_CFGR_MCO2EN */
  }                                                     /* 代码块结束 */
#endif /* RCC_CFGR_MCO2 */
}                                                       /* 代码块结束 */

/**
  * @brief  Enables the Clock Security System.
  * @note   If a failure is detected on the HSE oscillator clock, this oscillator
  *         is automatically disabled and an interrupt is generated to inform the
  *         software about the failure (Clock Security System Interrupt, CSSI),
  *         allowing the MCU to perform rescue operations. The CSSI is linked to
  *         the Cortex-M4 NMI (Non-Maskable Interrupt) exception vector.
  * @retval None
  */
void HAL_RCC_EnableCSS(void)                            /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  *(__IO uint32_t *) RCC_CR_CSSON_BB = (uint32_t)ENABLE;  /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  Disables the Clock Security System.
  * @retval None
  */
void HAL_RCC_DisableCSS(void)                           /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  *(__IO uint32_t *) RCC_CR_CSSON_BB = (uint32_t)DISABLE;  /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  Returns the SYSCLK frequency
  *
  * @note   The system frequency computed by this function is not the real
  *         frequency in the chip. It is calculated based on the predefined
  *         constant and the selected clock source:
  * @note     If SYSCLK source is HSI, function returns values based on HSI_VALUE(*)
  * @note     If SYSCLK source is HSE, function returns values based on HSE_VALUE(**)
  * @note     If SYSCLK source is PLL, function returns values based on HSE_VALUE(**)
  *           or HSI_VALUE(*) multiplied/divided by the PLL factors.
  * @note     (*) HSI_VALUE is a constant defined in stm32f4xx_hal_conf.h file (default value
  *               16 MHz) but the real value may vary depending on the variations
  *               in voltage and temperature.
  * @note     (**) HSE_VALUE is a constant defined in stm32f4xx_hal_conf.h file (default value
  *                25 MHz), user has to ensure that HSE_VALUE is same as the real
  *                frequency of the crystal used. Otherwise, this function may
  *                have wrong result.
  *
  * @note   The result of this function could be not correct when using fractional
  *         value for HSE crystal.
  *
  * @note   This function can be used by the user application to compute the
  *         baudrate for the communication peripherals or configure other parameters.
  *
  * @note   Each time SYSCLK changes, this function must be called to update the
  *         right SYSCLK value. Otherwise, any configuration based on this function will be incorrect.
  *
  *
  * @retval SYSCLK frequency
  */
__weak uint32_t HAL_RCC_GetSysClockFreq(void)           /* 源代码 */
{                                                       /* 代码块开始 */
  uint32_t pllm = 0U, pllvco = 0U, pllp = 0U;           /* 32位无符号变量/函数 */
  uint32_t sysclockfreq = 0U;                           /* 32位无符号变量/函数 */

  /* Get SYSCLK source -------------------------------------------------------*/
  switch (RCC->CFGR & RCC_CFGR_SWS)                     /* switch分支 */
  {                                                     /* 代码块开始 */
    case RCC_CFGR_SWS_HSI:  /* HSI used as system clock source */
    {                                                   /* 代码块开始 */
      sysclockfreq = HSI_VALUE;                         /* 赋值或初始化 */
       break;                                           /* 跳出 */
    }                                                   /* 代码块结束 */
    case RCC_CFGR_SWS_HSE:  /* HSE used as system clock  source */
    {                                                   /* 代码块开始 */
      sysclockfreq = HSE_VALUE;                         /* 赋值或初始化 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
    case RCC_CFGR_SWS_PLL:  /* PLL used as system clock  source */
    {                                                   /* 代码块开始 */
      /* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLLM) * PLLN
      SYSCLK = PLL_VCO / PLLP */
      pllm = RCC->PLLCFGR & RCC_PLLCFGR_PLLM;           /* 赋值或初始化 */
      if(__HAL_RCC_GET_PLL_OSCSOURCE() != RCC_PLLSOURCE_HSI)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* HSE used as PLL clock source */
        pllvco = (uint32_t) ((((uint64_t) HSE_VALUE * ((uint64_t) ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> RCC_PLLCFGR_PLLN_Pos)))) / (uint64_t)pllm);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* HSI used as PLL clock source */
        pllvco = (uint32_t) ((((uint64_t) HSI_VALUE * ((uint64_t) ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> RCC_PLLCFGR_PLLN_Pos)))) / (uint64_t)pllm);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      pllp = ((((RCC->PLLCFGR & RCC_PLLCFGR_PLLP) >> RCC_PLLCFGR_PLLP_Pos) + 1U) *2U);  /* 赋值或初始化 */

      sysclockfreq = pllvco/pllp;                       /* 赋值或初始化 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
    default:                                            /* default分支 */
    {                                                   /* 代码块开始 */
      sysclockfreq = HSI_VALUE;                         /* 赋值或初始化 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  return sysclockfreq;                                  /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Returns the HCLK frequency
  * @note   Each time HCLK changes, this function must be called to update the
  *         right HCLK value. Otherwise, any configuration based on this function will be incorrect.
  *
  * @note   The SystemCoreClock CMSIS variable is used to store System Clock Frequency
  *         and updated within this function
  * @retval HCLK frequency
  */
uint32_t HAL_RCC_GetHCLKFreq(void)                      /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  return SystemCoreClock;                               /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Returns the PCLK1 frequency
  * @note   Each time PCLK1 changes, this function must be called to update the
  *         right PCLK1 value. Otherwise, any configuration based on this function will be incorrect.
  * @retval PCLK1 frequency
  */
uint32_t HAL_RCC_GetPCLK1Freq(void)                     /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  /* Get HCLK source and Compute PCLK1 frequency ---------------------------*/
  return (HAL_RCC_GetHCLKFreq() >> APBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE1)>> RCC_CFGR_PPRE1_Pos]);  /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Returns the PCLK2 frequency
  * @note   Each time PCLK2 changes, this function must be called to update the
  *         right PCLK2 value. Otherwise, any configuration based on this function will be incorrect.
  * @retval PCLK2 frequency
  */
uint32_t HAL_RCC_GetPCLK2Freq(void)                     /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  /* Get HCLK source and Compute PCLK2 frequency ---------------------------*/
  return (HAL_RCC_GetHCLKFreq()>> APBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE2)>> RCC_CFGR_PPRE2_Pos]);  /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configures the RCC_OscInitStruct according to the internal
  * RCC configuration registers.
  * @param  RCC_OscInitStruct pointer to an RCC_OscInitTypeDef structure that
  * will be configured.
  * @retval None
  */
__weak void HAL_RCC_GetOscConfig(RCC_OscInitTypeDef  *RCC_OscInitStruct)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Set all possible values for the Oscillator type parameter ---------------*/
  RCC_OscInitStruct->OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSE | RCC_OSCILLATORTYPE_LSI;  /* 时钟相关操作 */

  /* Get the HSE configuration -----------------------------------------------*/
  if((RCC->CR &RCC_CR_HSEBYP) == RCC_CR_HSEBYP)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    RCC_OscInitStruct->HSEState = RCC_HSE_BYPASS;       /* 时钟相关操作 */
  }                                                     /* 代码块结束 */
  else if((RCC->CR &RCC_CR_HSEON) == RCC_CR_HSEON)      /* 否则如果 */
  {                                                     /* 代码块开始 */
    RCC_OscInitStruct->HSEState = RCC_HSE_ON;           /* 时钟相关操作 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    RCC_OscInitStruct->HSEState = RCC_HSE_OFF;          /* 时钟相关操作 */
  }                                                     /* 代码块结束 */

  /* Get the HSI configuration -----------------------------------------------*/
  if((RCC->CR &RCC_CR_HSION) == RCC_CR_HSION)           /* 条件判断 */
  {                                                     /* 代码块开始 */
    RCC_OscInitStruct->HSIState = RCC_HSI_ON;           /* 时钟相关操作 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    RCC_OscInitStruct->HSIState = RCC_HSI_OFF;          /* 时钟相关操作 */
  }                                                     /* 代码块结束 */

  RCC_OscInitStruct->HSICalibrationValue = (uint32_t)((RCC->CR &RCC_CR_HSITRIM) >> RCC_CR_HSITRIM_Pos);  /* 时钟相关操作 */

  /* Get the LSE configuration -----------------------------------------------*/
  if((RCC->BDCR &RCC_BDCR_LSEBYP) == RCC_BDCR_LSEBYP)   /* 条件判断 */
  {                                                     /* 代码块开始 */
    RCC_OscInitStruct->LSEState = RCC_LSE_BYPASS;       /* 时钟相关操作 */
  }                                                     /* 代码块结束 */
  else if((RCC->BDCR &RCC_BDCR_LSEON) == RCC_BDCR_LSEON)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    RCC_OscInitStruct->LSEState = RCC_LSE_ON;           /* 时钟相关操作 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    RCC_OscInitStruct->LSEState = RCC_LSE_OFF;          /* 时钟相关操作 */
  }                                                     /* 代码块结束 */

  /* Get the LSI configuration -----------------------------------------------*/
  if((RCC->CSR &RCC_CSR_LSION) == RCC_CSR_LSION)        /* 条件判断 */
  {                                                     /* 代码块开始 */
    RCC_OscInitStruct->LSIState = RCC_LSI_ON;           /* 时钟相关操作 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    RCC_OscInitStruct->LSIState = RCC_LSI_OFF;          /* 时钟相关操作 */
  }                                                     /* 代码块结束 */

  /* Get the PLL configuration -----------------------------------------------*/
  if((RCC->CR &RCC_CR_PLLON) == RCC_CR_PLLON)           /* 条件判断 */
  {                                                     /* 代码块开始 */
    RCC_OscInitStruct->PLL.PLLState = RCC_PLL_ON;       /* 时钟相关操作 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    RCC_OscInitStruct->PLL.PLLState = RCC_PLL_OFF;      /* 时钟相关操作 */
  }                                                     /* 代码块结束 */
  RCC_OscInitStruct->PLL.PLLSource = (uint32_t)(RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC);  /* 时钟相关操作 */
  RCC_OscInitStruct->PLL.PLLM = (uint32_t)(RCC->PLLCFGR & RCC_PLLCFGR_PLLM);  /* 时钟相关操作 */
  RCC_OscInitStruct->PLL.PLLN = (uint32_t)((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> RCC_PLLCFGR_PLLN_Pos);  /* 时钟相关操作 */
  RCC_OscInitStruct->PLL.PLLP = (uint32_t)((((RCC->PLLCFGR & RCC_PLLCFGR_PLLP) + RCC_PLLCFGR_PLLP_0) << 1U) >> RCC_PLLCFGR_PLLP_Pos);  /* 时钟相关操作 */
  RCC_OscInitStruct->PLL.PLLQ = (uint32_t)((RCC->PLLCFGR & RCC_PLLCFGR_PLLQ) >> RCC_PLLCFGR_PLLQ_Pos);  /* 时钟相关操作 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configures the RCC_ClkInitStruct according to the internal
  * RCC configuration registers.
  * @param  RCC_ClkInitStruct pointer to an RCC_ClkInitTypeDef structure that
  * will be configured.
  * @param  pFLatency Pointer on the Flash Latency.
  * @retval None
  */
void HAL_RCC_GetClockConfig(RCC_ClkInitTypeDef  *RCC_ClkInitStruct, uint32_t *pFLatency)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Set all possible values for the Clock type parameter --------------------*/
  RCC_ClkInitStruct->ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;  /* 时钟相关操作 */

  /* Get the SYSCLK configuration --------------------------------------------*/
  RCC_ClkInitStruct->SYSCLKSource = (uint32_t)(RCC->CFGR & RCC_CFGR_SW);  /* 时钟相关操作 */

  /* Get the HCLK configuration ----------------------------------------------*/
  RCC_ClkInitStruct->AHBCLKDivider = (uint32_t)(RCC->CFGR & RCC_CFGR_HPRE);  /* 时钟相关操作 */

  /* Get the APB1 configuration ----------------------------------------------*/
  RCC_ClkInitStruct->APB1CLKDivider = (uint32_t)(RCC->CFGR & RCC_CFGR_PPRE1);  /* 时钟相关操作 */

  /* Get the APB2 configuration ----------------------------------------------*/
  RCC_ClkInitStruct->APB2CLKDivider = (uint32_t)((RCC->CFGR & RCC_CFGR_PPRE2) >> 3U);  /* 时钟相关操作 */

  /* Get the Flash Wait State (Latency) configuration ------------------------*/
  *pFLatency = (uint32_t)(FLASH->ACR & FLASH_ACR_LATENCY);  /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief This function handles the RCC CSS interrupt request.
  * @note This API should be called under the NMI_Handler().
  * @retval None
  */
void HAL_RCC_NMI_IRQHandler(void)                       /* 中断服务函数 */
{                                                       /* 代码块开始 */
  /* Check RCC CSSF flag  */
  if(__HAL_RCC_GET_IT(RCC_IT_CSS))                      /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* RCC Clock Security System interrupt user callback */
    HAL_RCC_CSSCallback();                              /* HAL 时钟配置 */

    /* Clear RCC CSS pending bit */
    __HAL_RCC_CLEAR_IT(RCC_IT_CSS);                     /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  RCC Clock Security System interrupt callback
  * @retval None
  */
__weak void HAL_RCC_CSSCallback(void)                   /* 源代码 */
{                                                       /* 代码块开始 */
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_RCC_CSSCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/**
  * @}
  */

#endif /* HAL_RCC_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
