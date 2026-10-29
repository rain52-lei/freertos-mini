/* 文件: stm32f4xx_hal_rcc_ex.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_rcc_ex.c
  * @author  MCD Application Team
  * @brief   Extension RCC HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities RCC extension peripheral:
  *           + Extended Peripheral Control functions
  *
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

/** @defgroup RCCEx RCCEx
  * @brief RCCEx HAL module driver
  * @{
  */

#ifdef HAL_RCC_MODULE_ENABLED                           /* 条件编译 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @addtogroup RCCEx_Private_Constants
  * @{
  */
/**
  * @}
  */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/** @defgroup RCCEx_Exported_Functions RCCEx Exported Functions
  *  @{
  */

/** @defgroup RCCEx_Exported_Functions_Group1 Extended Peripheral Control functions
 *  @brief  Extended Peripheral Control functions
 *
@verbatim
 ===============================================================================
                ##### Extended Peripheral Control functions  #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to control the RCC Clocks
    frequencies.
    [..]
    (@) Important note: Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to
        select the RTC clock source; in this case the Backup domain will be reset in
        order to modify the RTC Clock source, as consequence RTC registers (including
        the backup registers) and RCC_BDCR register are set to their reset values.

@endverbatim
  * @{
  */

#if defined(STM32F446xx)                                /* 条件编译 */
/**
  * @brief  Initializes the RCC extended peripherals clocks according to the specified
  *         parameters in the RCC_PeriphCLKInitTypeDef.
  * @param  PeriphClkInit pointer to an RCC_PeriphCLKInitTypeDef structure that
  *         contains the configuration information for the Extended Peripherals
  *         clocks(I2S, SAI, LTDC RTC and TIM).
  *
  * @note   Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select
  *         the RTC clock source; in this case the Backup domain will be reset in
  *         order to modify the RTC Clock source, as consequence RTC registers (including
  *         the backup registers) and RCC_BDCR register are set to their reset values.
  *
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCCEx_PeriphCLKConfig(RCC_PeriphCLKInitTypeDef  *PeriphClkInit)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart = 0U;                              /* 32位无符号变量/函数 */
  uint32_t tmpreg1 = 0U;                                /* 32位无符号变量/函数 */
  uint32_t plli2sp = 0U;                                /* 32位无符号变量/函数 */
  uint32_t plli2sq = 0U;                                /* 32位无符号变量/函数 */
  uint32_t plli2sr = 0U;                                /* 32位无符号变量/函数 */
  uint32_t pllsaip = 0U;                                /* 32位无符号变量/函数 */
  uint32_t pllsaiq = 0U;                                /* 32位无符号变量/函数 */
  uint32_t plli2sused = 0U;                             /* 32位无符号变量/函数 */
  uint32_t pllsaiused = 0U;                             /* 32位无符号变量/函数 */

  /* Check the peripheral clock selection parameters */
  assert_param(IS_RCC_PERIPHCLOCK(PeriphClkInit->PeriphClockSelection));  /* 执行语句 */

  /*------------------------ I2S APB1 configuration --------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_I2S_APB1) == (RCC_PERIPHCLK_I2S_APB1))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check the parameters */
    assert_param(IS_RCC_I2SAPB1CLKSOURCE(PeriphClkInit->I2sApb1ClockSelection));  /* 执行语句 */

    /* Configure I2S Clock source */
    __HAL_RCC_I2S_APB1_CONFIG(PeriphClkInit->I2sApb1ClockSelection);  /* HAL辅助宏/函数 */
    /* Enable the PLLI2S when it's used as clock source for I2S */
    if(PeriphClkInit->I2sApb1ClockSelection == RCC_I2SAPB1CLKSOURCE_PLLI2S)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      plli2sused = 1U;                                  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /*--------------------------------------------------------------------------*/

  /*---------------------------- I2S APB2 configuration ----------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_I2S_APB2) == (RCC_PERIPHCLK_I2S_APB2))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check the parameters */
    assert_param(IS_RCC_I2SAPB2CLKSOURCE(PeriphClkInit->I2sApb2ClockSelection));  /* 执行语句 */

    /* Configure I2S Clock source */
    __HAL_RCC_I2S_APB2_CONFIG(PeriphClkInit->I2sApb2ClockSelection);  /* HAL辅助宏/函数 */
    /* Enable the PLLI2S when it's used as clock source for I2S */
    if(PeriphClkInit->I2sApb2ClockSelection == RCC_I2SAPB2CLKSOURCE_PLLI2S)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      plli2sused = 1U;                                  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /*--------------------------------------------------------------------------*/

  /*--------------------------- SAI1 configuration ---------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SAI1) == (RCC_PERIPHCLK_SAI1))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check the parameters */
    assert_param(IS_RCC_SAI1CLKSOURCE(PeriphClkInit->Sai1ClockSelection));  /* 执行语句 */

    /* Configure SAI1 Clock source */
    __HAL_RCC_SAI1_CONFIG(PeriphClkInit->Sai1ClockSelection);  /* HAL辅助宏/函数 */
    /* Enable the PLLI2S when it's used as clock source for SAI */
    if(PeriphClkInit->Sai1ClockSelection == RCC_SAI1CLKSOURCE_PLLI2S)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      plli2sused = 1U;                                  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    /* Enable the PLLSAI when it's used as clock source for SAI */
    if(PeriphClkInit->Sai1ClockSelection == RCC_SAI1CLKSOURCE_PLLSAI)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      pllsaiused = 1U;                                  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /*--------------------------------------------------------------------------*/

  /*-------------------------- SAI2 configuration ----------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SAI2) == (RCC_PERIPHCLK_SAI2))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check the parameters */
    assert_param(IS_RCC_SAI2CLKSOURCE(PeriphClkInit->Sai2ClockSelection));  /* 执行语句 */

    /* Configure SAI2 Clock source */
    __HAL_RCC_SAI2_CONFIG(PeriphClkInit->Sai2ClockSelection);  /* HAL辅助宏/函数 */

    /* Enable the PLLI2S when it's used as clock source for SAI */
    if(PeriphClkInit->Sai2ClockSelection == RCC_SAI2CLKSOURCE_PLLI2S)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      plli2sused = 1U;                                  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    /* Enable the PLLSAI when it's used as clock source for SAI */
    if(PeriphClkInit->Sai2ClockSelection == RCC_SAI2CLKSOURCE_PLLSAI)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      pllsaiused = 1U;                                  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /*--------------------------------------------------------------------------*/

  /*----------------------------- RTC configuration --------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_RTC) == (RCC_PERIPHCLK_RTC))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check for RTC Parameters used to output RTCCLK */
    assert_param(IS_RCC_RTCCLKSOURCE(PeriphClkInit->RTCClockSelection));  /* 执行语句 */

    /* Enable Power Clock*/
    __HAL_RCC_PWR_CLK_ENABLE();                         /* HAL辅助宏/函数 */

    /* Enable write access to Backup domain */
    PWR->CR |= PWR_CR_DBP;                              /* 赋值或初始化 */

    /* Get tick */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */

    while((PWR->CR & PWR_CR_DBP) == RESET)              /* while循环 */
    {                                                   /* 代码块开始 */
      if((HAL_GetTick() - tickstart ) > RCC_DBP_TIMEOUT_VALUE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    /* Reset the Backup domain only if the RTC Clock source selection is modified from reset value */
    tmpreg1 = (RCC->BDCR & RCC_BDCR_RTCSEL);            /* 赋值或初始化 */
    if((tmpreg1 != 0x00000000U) && ((tmpreg1) != (PeriphClkInit->RTCClockSelection & RCC_BDCR_RTCSEL)))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Store the content of BDCR register before the reset of Backup Domain */
      tmpreg1 = (RCC->BDCR & ~(RCC_BDCR_RTCSEL));       /* 赋值或初始化 */
      /* RTC Clock selection can be changed only if the Backup Domain is reset */
      __HAL_RCC_BACKUPRESET_FORCE();                    /* HAL辅助宏/函数 */
      __HAL_RCC_BACKUPRESET_RELEASE();                  /* HAL辅助宏/函数 */
      /* Restore the Content of BDCR register */
      RCC->BDCR = tmpreg1;                              /* 赋值或初始化 */

      /* Wait for LSE reactivation if LSE was enable prior to Backup Domain reset */
      if(HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSEON))     /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Get tick */
        tickstart = HAL_GetTick();                      /* 赋值或初始化 */

        /* Wait till LSE is ready */
        while(__HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY) == RESET)  /* while循环 */
        {                                               /* 代码块开始 */
          if((HAL_GetTick() - tickstart ) > RCC_LSE_TIMEOUT_VALUE)  /* 条件判断 */
          {                                             /* 代码块开始 */
            return HAL_TIMEOUT;                         /* 返回值 */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    __HAL_RCC_RTC_CONFIG(PeriphClkInit->RTCClockSelection);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  /*--------------------------------------------------------------------------*/

  /*---------------------------- TIM configuration ---------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_TIM) == (RCC_PERIPHCLK_TIM))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Configure Timer Prescaler */
    __HAL_RCC_TIMCLKPRESCALER(PeriphClkInit->TIMPresSelection);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  /*--------------------------------------------------------------------------*/

  /*---------------------------- FMPI2C1 Configuration -----------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_FMPI2C1) == RCC_PERIPHCLK_FMPI2C1)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check the parameters */
    assert_param(IS_RCC_FMPI2C1CLKSOURCE(PeriphClkInit->Fmpi2c1ClockSelection));  /* 执行语句 */

    /* Configure the FMPI2C1 clock source */
    __HAL_RCC_FMPI2C1_CONFIG(PeriphClkInit->Fmpi2c1ClockSelection);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  /*--------------------------------------------------------------------------*/

  /*------------------------------ CEC Configuration -------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_CEC) == RCC_PERIPHCLK_CEC)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check the parameters */
    assert_param(IS_RCC_CECCLKSOURCE(PeriphClkInit->CecClockSelection));  /* 执行语句 */

    /* Configure the CEC clock source */
    __HAL_RCC_CEC_CONFIG(PeriphClkInit->CecClockSelection);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  /*--------------------------------------------------------------------------*/

  /*----------------------------- CLK48 Configuration ------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_CLK48) == RCC_PERIPHCLK_CLK48)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check the parameters */
    assert_param(IS_RCC_CLK48CLKSOURCE(PeriphClkInit->Clk48ClockSelection));  /* 执行语句 */

    /* Configure the CLK48 clock source */
    __HAL_RCC_CLK48_CONFIG(PeriphClkInit->Clk48ClockSelection);  /* HAL辅助宏/函数 */

    /* Enable the PLLSAI when it's used as clock source for CLK48 */
    if(PeriphClkInit->Clk48ClockSelection == RCC_CLK48CLKSOURCE_PLLSAIP)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      pllsaiused = 1U;                                  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /*--------------------------------------------------------------------------*/

  /*----------------------------- SDIO Configuration -------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SDIO) == RCC_PERIPHCLK_SDIO)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check the parameters */
    assert_param(IS_RCC_SDIOCLKSOURCE(PeriphClkInit->SdioClockSelection));  /* 执行语句 */

    /* Configure the SDIO clock source */
    __HAL_RCC_SDIO_CONFIG(PeriphClkInit->SdioClockSelection);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  /*--------------------------------------------------------------------------*/

  /*------------------------------ SPDIFRX Configuration ---------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SPDIFRX) == RCC_PERIPHCLK_SPDIFRX)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check the parameters */
    assert_param(IS_RCC_SPDIFRXCLKSOURCE(PeriphClkInit->SpdifClockSelection));  /* 执行语句 */

    /* Configure the SPDIFRX clock source */
    __HAL_RCC_SPDIFRX_CONFIG(PeriphClkInit->SpdifClockSelection);  /* HAL辅助宏/函数 */
    /* Enable the PLLI2S when it's used as clock source for SPDIFRX */
    if(PeriphClkInit->SpdifClockSelection == RCC_SPDIFRXCLKSOURCE_PLLI2SP)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      plli2sused = 1U;                                  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /*--------------------------------------------------------------------------*/

  /*---------------------------- PLLI2S Configuration ------------------------*/
  /* PLLI2S is configured when a peripheral will use it as source clock : SAI1, SAI2, I2S on APB1,
     I2S on APB2 or SPDIFRX */
  if((plli2sused == 1U) || (PeriphClkInit->PeriphClockSelection == RCC_PERIPHCLK_PLLI2S))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable the PLLI2S */
    __HAL_RCC_PLLI2S_DISABLE();                         /* HAL辅助宏/函数 */
    /* Get tick */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */
    /* Wait till PLLI2S is disabled */
    while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLI2SRDY)  != RESET)  /* while循环 */
    {                                                   /* 代码块开始 */
      if((HAL_GetTick() - tickstart ) > PLLI2S_TIMEOUT_VALUE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* return in case of Timeout detected */
        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* check for common PLLI2S Parameters */
    assert_param(IS_RCC_PLLI2SM_VALUE(PeriphClkInit->PLLI2S.PLLI2SM));  /* 执行语句 */
    assert_param(IS_RCC_PLLI2SN_VALUE(PeriphClkInit->PLLI2S.PLLI2SN));  /* 执行语句 */

    /*------ In Case of PLLI2S is selected as source clock for I2S -----------*/
    if(((((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_I2S_APB1) == RCC_PERIPHCLK_I2S_APB1) && (PeriphClkInit->I2sApb1ClockSelection == RCC_I2SAPB1CLKSOURCE_PLLI2S)) ||  /* 条件判断 */
       ((((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_I2S_APB2) == RCC_PERIPHCLK_I2S_APB2) && (PeriphClkInit->I2sApb2ClockSelection == RCC_I2SAPB2CLKSOURCE_PLLI2S)))  /* 源代码 */
    {                                                   /* 代码块开始 */
      /* check for Parameters */
      assert_param(IS_RCC_PLLI2SR_VALUE(PeriphClkInit->PLLI2S.PLLI2SR));  /* 执行语句 */

      /* Read PLLI2SP/PLLI2SQ value from PLLI2SCFGR register (this value is not needed for I2S configuration) */
      plli2sp = ((((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SP) >> RCC_PLLI2SCFGR_PLLI2SP_Pos) + 1U) << 1U);  /* 赋值或初始化 */
      plli2sq = ((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SQ) >> RCC_PLLI2SCFGR_PLLI2SQ_Pos);  /* 赋值或初始化 */
      /* Configure the PLLI2S division factors */
      /* PLLI2S_VCO = f(VCO clock) = f(PLLI2S clock input) * (PLLI2SN/PLLI2SM) */
      /* I2SCLK = f(PLLI2S clock output) = f(VCO clock) / PLLI2SR */
      __HAL_RCC_PLLI2S_CONFIG(PeriphClkInit->PLLI2S.PLLI2SM, PeriphClkInit->PLLI2S.PLLI2SN , plli2sp, plli2sq, PeriphClkInit->PLLI2S.PLLI2SR);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /*------- In Case of PLLI2S is selected as source clock for SAI ----------*/
    if(((((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SAI1) == RCC_PERIPHCLK_SAI1) && (PeriphClkInit->Sai1ClockSelection == RCC_SAI1CLKSOURCE_PLLI2S)) ||  /* 条件判断 */
       ((((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SAI2) == RCC_PERIPHCLK_SAI2) && (PeriphClkInit->Sai2ClockSelection == RCC_SAI2CLKSOURCE_PLLI2S)))  /* 源代码 */
    {                                                   /* 代码块开始 */
      /* Check for PLLI2S Parameters */
      assert_param(IS_RCC_PLLI2SQ_VALUE(PeriphClkInit->PLLI2S.PLLI2SQ));  /* 执行语句 */
      /* Check for PLLI2S/DIVQ parameters */
      assert_param(IS_RCC_PLLI2S_DIVQ_VALUE(PeriphClkInit->PLLI2SDivQ));  /* 执行语句 */

      /* Read PLLI2SP/PLLI2SR value from PLLI2SCFGR register (this value is not needed for SAI configuration) */
      plli2sp = ((((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SP) >> RCC_PLLI2SCFGR_PLLI2SP_Pos) + 1U) << 1U);  /* 赋值或初始化 */
      plli2sr = ((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SR) >> RCC_PLLI2SCFGR_PLLI2SR_Pos);  /* 赋值或初始化 */
      /* Configure the PLLI2S division factors */
      /* PLLI2S_VCO Input  = PLL_SOURCE/PLLI2SM */
      /* PLLI2S_VCO Output = PLLI2S_VCO Input * PLLI2SN */
      /* SAI_CLK(first level) = PLLI2S_VCO Output/PLLI2SQ */
      __HAL_RCC_PLLI2S_CONFIG(PeriphClkInit->PLLI2S.PLLI2SM, PeriphClkInit->PLLI2S.PLLI2SN , plli2sp, PeriphClkInit->PLLI2S.PLLI2SQ, plli2sr);  /* HAL辅助宏/函数 */

      /* SAI_CLK_x = SAI_CLK(first level)/PLLI2SDIVQ */
      __HAL_RCC_PLLI2S_PLLSAICLKDIVQ_CONFIG(PeriphClkInit->PLLI2SDivQ);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /*------ In Case of PLLI2S is selected as source clock for SPDIFRX -------*/
    if((((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SPDIFRX) == RCC_PERIPHCLK_SPDIFRX) && (PeriphClkInit->SpdifClockSelection == RCC_SPDIFRXCLKSOURCE_PLLI2SP))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* check for Parameters */
      assert_param(IS_RCC_PLLI2SP_VALUE(PeriphClkInit->PLLI2S.PLLI2SP));  /* 执行语句 */
      /* Read PLLI2SR value from PLLI2SCFGR register (this value is not need for SAI configuration) */
      plli2sq = ((((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SP) >> RCC_PLLI2SCFGR_PLLI2SP_Pos) + 1U) << 1U);  /* 赋值或初始化 */
      plli2sr = ((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SR) >> RCC_PLLI2SCFGR_PLLI2SR_Pos);  /* 赋值或初始化 */
      /* Configure the PLLI2S division factors */
      /* PLLI2S_VCO = f(VCO clock) = f(PLLI2S clock input) * (PLLI2SN/PLLI2SM) */
      /* SPDIFRXCLK = f(PLLI2S clock output) = f(VCO clock) / PLLI2SP */
      __HAL_RCC_PLLI2S_CONFIG(PeriphClkInit->PLLI2S.PLLI2SM, PeriphClkInit->PLLI2S.PLLI2SN , PeriphClkInit->PLLI2S.PLLI2SP, plli2sq, plli2sr);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

     /*----------------- In Case of PLLI2S is just selected  -----------------*/
    if((PeriphClkInit->PeriphClockSelection & RCC_PERIPHCLK_PLLI2S) == RCC_PERIPHCLK_PLLI2S)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Check for Parameters */
      assert_param(IS_RCC_PLLI2SP_VALUE(PeriphClkInit->PLLI2S.PLLI2SP));  /* 执行语句 */
      assert_param(IS_RCC_PLLI2SR_VALUE(PeriphClkInit->PLLI2S.PLLI2SR));  /* 执行语句 */
      assert_param(IS_RCC_PLLI2SQ_VALUE(PeriphClkInit->PLLI2S.PLLI2SQ));  /* 执行语句 */

      /* Configure the PLLI2S division factors */
      /* PLLI2S_VCO = f(VCO clock) = f(PLLI2S clock input) * (PLLI2SN/PLLI2SM) */
      __HAL_RCC_PLLI2S_CONFIG(PeriphClkInit->PLLI2S.PLLI2SM, PeriphClkInit->PLLI2S.PLLI2SN , PeriphClkInit->PLLI2S.PLLI2SP, PeriphClkInit->PLLI2S.PLLI2SQ, PeriphClkInit->PLLI2S.PLLI2SR);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Enable the PLLI2S */
    __HAL_RCC_PLLI2S_ENABLE();                          /* HAL辅助宏/函数 */
    /* Get tick */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */
    /* Wait till PLLI2S is ready */
    while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLI2SRDY)  == RESET)  /* while循环 */
    {                                                   /* 代码块开始 */
      if((HAL_GetTick() - tickstart ) > PLLI2S_TIMEOUT_VALUE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* return in case of Timeout detected */
        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /*--------------------------------------------------------------------------*/

  /*----------------------------- PLLSAI Configuration -----------------------*/
  /* PLLSAI is configured when a peripheral will use it as source clock : SAI1, SAI2, CLK48 or SDIO */
  if(pllsaiused == 1U)                                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable PLLSAI Clock */
    __HAL_RCC_PLLSAI_DISABLE();                         /* HAL辅助宏/函数 */
    /* Get tick */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */
    /* Wait till PLLSAI is disabled */
    while(__HAL_RCC_PLLSAI_GET_FLAG() != RESET)         /* while循环 */
    {                                                   /* 代码块开始 */
      if((HAL_GetTick() - tickstart ) > PLLSAI_TIMEOUT_VALUE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* return in case of Timeout detected */
        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Check the PLLSAI division factors */
    assert_param(IS_RCC_PLLSAIM_VALUE(PeriphClkInit->PLLSAI.PLLSAIM));  /* 执行语句 */
    assert_param(IS_RCC_PLLSAIN_VALUE(PeriphClkInit->PLLSAI.PLLSAIN));  /* 执行语句 */

    /*------ In Case of PLLSAI is selected as source clock for SAI -----------*/
    if(((((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SAI1) == RCC_PERIPHCLK_SAI1) && (PeriphClkInit->Sai1ClockSelection == RCC_SAI1CLKSOURCE_PLLSAI)) ||  /* 条件判断 */
       ((((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SAI2) == RCC_PERIPHCLK_SAI2) && (PeriphClkInit->Sai2ClockSelection == RCC_SAI2CLKSOURCE_PLLSAI)))  /* 源代码 */
    {                                                   /* 代码块开始 */
      /* check for PLLSAIQ Parameter */
      assert_param(IS_RCC_PLLSAIQ_VALUE(PeriphClkInit->PLLSAI.PLLSAIQ));  /* 执行语句 */
      /* check for PLLSAI/DIVQ Parameter */
      assert_param(IS_RCC_PLLSAI_DIVQ_VALUE(PeriphClkInit->PLLSAIDivQ));  /* 执行语句 */

      /* Read PLLSAIP value from PLLSAICFGR register (this value is not needed for SAI configuration) */
      pllsaip = ((((RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIP) >> RCC_PLLSAICFGR_PLLSAIP_Pos) + 1U) << 1U);  /* 赋值或初始化 */
      /* PLLSAI_VCO Input  = PLL_SOURCE/PLLM */
      /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN */
      /* SAI_CLK(first level) = PLLSAI_VCO Output/PLLSAIQ */
      __HAL_RCC_PLLSAI_CONFIG(PeriphClkInit->PLLSAI.PLLSAIM, PeriphClkInit->PLLSAI.PLLSAIN , pllsaip, PeriphClkInit->PLLSAI.PLLSAIQ, 0U);  /* HAL辅助宏/函数 */

      /* SAI_CLK_x = SAI_CLK(first level)/PLLSAIDIVQ */
      __HAL_RCC_PLLSAI_PLLSAICLKDIVQ_CONFIG(PeriphClkInit->PLLSAIDivQ);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /*------ In Case of PLLSAI is selected as source clock for CLK48 ---------*/
    /* In Case of PLLI2S is selected as source clock for CLK48 */
    if((((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_CLK48) == RCC_PERIPHCLK_CLK48) && (PeriphClkInit->Clk48ClockSelection == RCC_CLK48CLKSOURCE_PLLSAIP))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* check for Parameters */
      assert_param(IS_RCC_PLLSAIP_VALUE(PeriphClkInit->PLLSAI.PLLSAIP));  /* 执行语句 */
      /* Read PLLSAIQ value from PLLI2SCFGR register (this value is not need for SAI configuration) */
      pllsaiq = ((RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIQ) >> RCC_PLLSAICFGR_PLLSAIQ_Pos);  /* 赋值或初始化 */
      /* Configure the PLLSAI division factors */
      /* PLLSAI_VCO = f(VCO clock) = f(PLLSAI clock input) * (PLLI2SN/PLLSAIM) */
      /* 48CLK = f(PLLSAI clock output) = f(VCO clock) / PLLSAIP */
      __HAL_RCC_PLLSAI_CONFIG(PeriphClkInit->PLLSAI.PLLSAIM, PeriphClkInit->PLLSAI.PLLSAIN , PeriphClkInit->PLLSAI.PLLSAIP, pllsaiq, 0U);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Enable PLLSAI Clock */
    __HAL_RCC_PLLSAI_ENABLE();                          /* HAL辅助宏/函数 */
    /* Get tick */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */
    /* Wait till PLLSAI is ready */
    while(__HAL_RCC_PLLSAI_GET_FLAG() == RESET)         /* while循环 */
    {                                                   /* 代码块开始 */
      if((HAL_GetTick() - tickstart ) > PLLSAI_TIMEOUT_VALUE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* return in case of Timeout detected */
        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Get the RCC_PeriphCLKInitTypeDef according to the internal
  *         RCC configuration registers.
  * @param  PeriphClkInit pointer to an RCC_PeriphCLKInitTypeDef structure that
  *         will be configured.
  * @retval None
  */
void HAL_RCCEx_GetPeriphCLKConfig(RCC_PeriphCLKInitTypeDef  *PeriphClkInit)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  uint32_t tempreg;                                     /* 32位无符号变量/函数 */

  /* Set all possible values for the extended clock type parameter------------*/
  PeriphClkInit->PeriphClockSelection = RCC_PERIPHCLK_I2S_APB1 | RCC_PERIPHCLK_I2S_APB2 |\
                                        RCC_PERIPHCLK_SAI1     | RCC_PERIPHCLK_SAI2     |\
                                        RCC_PERIPHCLK_TIM      | RCC_PERIPHCLK_RTC      |\
                                        RCC_PERIPHCLK_CEC      | RCC_PERIPHCLK_FMPI2C1  |\
                                        RCC_PERIPHCLK_CLK48     | RCC_PERIPHCLK_SDIO     |\
                                        RCC_PERIPHCLK_SPDIFRX;  /* 时钟相关操作 */

  /* Get the PLLI2S Clock configuration --------------------------------------*/
  PeriphClkInit->PLLI2S.PLLI2SM = (uint32_t)((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SM) >> RCC_PLLI2SCFGR_PLLI2SM_Pos);  /* 赋值或初始化 */
  PeriphClkInit->PLLI2S.PLLI2SN = (uint32_t)((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SN) >> RCC_PLLI2SCFGR_PLLI2SN_Pos);  /* 赋值或初始化 */
  PeriphClkInit->PLLI2S.PLLI2SP = (uint32_t)((((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SP) >> RCC_PLLI2SCFGR_PLLI2SP_Pos) + 1U) << 1U);  /* 赋值或初始化 */
  PeriphClkInit->PLLI2S.PLLI2SQ = (uint32_t)((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SQ) >> RCC_PLLI2SCFGR_PLLI2SQ_Pos);  /* 赋值或初始化 */
  PeriphClkInit->PLLI2S.PLLI2SR = (uint32_t)((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SR) >> RCC_PLLI2SCFGR_PLLI2SR_Pos);  /* 赋值或初始化 */
  /* Get the PLLSAI Clock configuration --------------------------------------*/
  PeriphClkInit->PLLSAI.PLLSAIM = (uint32_t)((RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIM) >> RCC_PLLSAICFGR_PLLSAIM_Pos);  /* 赋值或初始化 */
  PeriphClkInit->PLLSAI.PLLSAIN = (uint32_t)((RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIN) >> RCC_PLLSAICFGR_PLLSAIN_Pos);  /* 赋值或初始化 */
  PeriphClkInit->PLLSAI.PLLSAIP = (uint32_t)((((RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIP) >> RCC_PLLSAICFGR_PLLSAIP_Pos) + 1U) << 1U);  /* 赋值或初始化 */
  PeriphClkInit->PLLSAI.PLLSAIQ = (uint32_t)((RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIQ) >> RCC_PLLSAICFGR_PLLSAIQ_Pos);  /* 赋值或初始化 */
  /* Get the PLLSAI/PLLI2S division factors ----------------------------------*/
  PeriphClkInit->PLLI2SDivQ = (uint32_t)((RCC->DCKCFGR & RCC_DCKCFGR_PLLI2SDIVQ) >> RCC_DCKCFGR_PLLI2SDIVQ_Pos);  /* 赋值或初始化 */
  PeriphClkInit->PLLSAIDivQ = (uint32_t)((RCC->DCKCFGR & RCC_DCKCFGR_PLLSAIDIVQ) >> RCC_DCKCFGR_PLLSAIDIVQ_Pos);  /* 赋值或初始化 */

  /* Get the SAI1 clock configuration ----------------------------------------*/
  PeriphClkInit->Sai1ClockSelection = __HAL_RCC_GET_SAI1_SOURCE();  /* 赋值或初始化 */

  /* Get the SAI2 clock configuration ----------------------------------------*/
  PeriphClkInit->Sai2ClockSelection = __HAL_RCC_GET_SAI2_SOURCE();  /* 赋值或初始化 */

  /* Get the I2S APB1 clock configuration ------------------------------------*/
  PeriphClkInit->I2sApb1ClockSelection = __HAL_RCC_GET_I2S_APB1_SOURCE();  /* 赋值或初始化 */

  /* Get the I2S APB2 clock configuration ------------------------------------*/
  PeriphClkInit->I2sApb2ClockSelection = __HAL_RCC_GET_I2S_APB2_SOURCE();  /* 赋值或初始化 */

  /* Get the RTC Clock configuration -----------------------------------------*/
  tempreg = (RCC->CFGR & RCC_CFGR_RTCPRE);              /* 赋值或初始化 */
  PeriphClkInit->RTCClockSelection = (uint32_t)((tempreg) | (RCC->BDCR & RCC_BDCR_RTCSEL));  /* 赋值或初始化 */

  /* Get the CEC clock configuration -----------------------------------------*/
  PeriphClkInit->CecClockSelection = __HAL_RCC_GET_CEC_SOURCE();  /* 赋值或初始化 */

  /* Get the FMPI2C1 clock configuration -------------------------------------*/
  PeriphClkInit->Fmpi2c1ClockSelection = __HAL_RCC_GET_FMPI2C1_SOURCE();  /* 赋值或初始化 */

  /* Get the CLK48 clock configuration ----------------------------------------*/
  PeriphClkInit->Clk48ClockSelection = __HAL_RCC_GET_CLK48_SOURCE();  /* 赋值或初始化 */

  /* Get the SDIO clock configuration ----------------------------------------*/
  PeriphClkInit->SdioClockSelection = __HAL_RCC_GET_SDIO_SOURCE();  /* 赋值或初始化 */

  /* Get the SPDIFRX clock configuration -------------------------------------*/
  PeriphClkInit->SpdifClockSelection = __HAL_RCC_GET_SPDIFRX_SOURCE();  /* 赋值或初始化 */

  /* Get the TIM Prescaler configuration -------------------------------------*/
  if ((RCC->DCKCFGR & RCC_DCKCFGR_TIMPRE) == RESET)     /* 条件判断 */
  {                                                     /* 代码块开始 */
    PeriphClkInit->TIMPresSelection = RCC_TIMPRES_DESACTIVATED;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    PeriphClkInit->TIMPresSelection = RCC_TIMPRES_ACTIVATED;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return the peripheral clock frequency for a given peripheral(SAI..)
  * @note   Return 0 if peripheral clock identifier not managed by this API
  * @param  PeriphClk Peripheral clock identifier
  *         This parameter can be one of the following values:
  *            @arg RCC_PERIPHCLK_SAI1: SAI1 peripheral clock
  *            @arg RCC_PERIPHCLK_SAI2: SAI2 peripheral clock
  *            @arg RCC_PERIPHCLK_I2S_APB1: I2S APB1 peripheral clock
  *            @arg RCC_PERIPHCLK_I2S_APB2: I2S APB2 peripheral clock
  * @retval Frequency in KHz
  */
uint32_t HAL_RCCEx_GetPeriphCLKFreq(uint32_t PeriphClk)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpreg1 = 0U;                                /* 32位无符号变量/函数 */
  /* This variable used to store the SAI clock frequency (value in Hz) */
  uint32_t frequency = 0U;                              /* 32位无符号变量/函数 */
  /* This variable used to store the VCO Input (value in Hz) */
  uint32_t vcoinput = 0U;                               /* 32位无符号变量/函数 */
  /* This variable used to store the SAI clock source */
  uint32_t saiclocksource = 0U;                         /* 32位无符号变量/函数 */
  uint32_t srcclk = 0U;                                 /* 32位无符号变量/函数 */
  /* This variable used to store the VCO Output (value in Hz) */
  uint32_t vcooutput = 0U;                              /* 32位无符号变量/函数 */
  switch (PeriphClk)                                    /* switch分支 */
  {                                                     /* 代码块开始 */
  case RCC_PERIPHCLK_SAI1:                              /* case标签 */
  case RCC_PERIPHCLK_SAI2:                              /* case标签 */
    {                                                   /* 代码块开始 */
      saiclocksource = RCC->DCKCFGR;                    /* 赋值或初始化 */
      saiclocksource &= (RCC_DCKCFGR_SAI1SRC | RCC_DCKCFGR_SAI2SRC);  /* 赋值或初始化 */
      switch (saiclocksource)                           /* switch分支 */
      {                                                 /* 代码块开始 */
      case 0U: /* PLLSAI is the clock source for SAI*/
        {                                               /* 代码块开始 */
          /* Configure the PLLSAI division factor */
          /* PLLSAI_VCO Input  = PLL_SOURCE/PLLSAIM */
          if((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLSOURCE_HSI)  /* 条件判断 */
          {                                             /* 代码块开始 */
            /* In Case the PLL Source is HSI (Internal Clock) */
            vcoinput = (HSI_VALUE / (uint32_t)(RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIM));  /* 赋值或初始化 */
          }                                             /* 代码块结束 */
          else                                          /* 否则 */
          {                                             /* 代码块开始 */
            /* In Case the PLL Source is HSE (External Clock) */
            vcoinput = ((HSE_VALUE / (uint32_t)(RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIM)));  /* 赋值或初始化 */
          }                                             /* 代码块结束 */
          /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN */
          /* SAI_CLK(first level) = PLLSAI_VCO Output/PLLSAIQ */
          tmpreg1 = (RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIQ) >> 24U;  /* 赋值或初始化 */
          frequency = (vcoinput * ((RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIN) >> 6U))/(tmpreg1);  /* 赋值或初始化 */

          /* SAI_CLK_x = SAI_CLK(first level)/PLLSAIDIVQ */
          tmpreg1 = (((RCC->DCKCFGR & RCC_DCKCFGR_PLLSAIDIVQ) >> 8U) + 1U);  /* 赋值或初始化 */
          frequency = frequency/(tmpreg1);              /* 赋值或初始化 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */
      case RCC_DCKCFGR_SAI1SRC_0: /* PLLI2S is the clock source for SAI*/
      case RCC_DCKCFGR_SAI2SRC_0: /* PLLI2S is the clock source for SAI*/
        {                                               /* 代码块开始 */
          /* Configure the PLLI2S division factor */
          /* PLLI2S_VCO Input  = PLL_SOURCE/PLLI2SM */
          if((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLSOURCE_HSI)  /* 条件判断 */
          {                                             /* 代码块开始 */
            /* In Case the PLL Source is HSI (Internal Clock) */
            vcoinput = (HSI_VALUE / (uint32_t)(RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SM));  /* 赋值或初始化 */
          }                                             /* 代码块结束 */
          else                                          /* 否则 */
          {                                             /* 代码块开始 */
            /* In Case the PLL Source is HSE (External Clock) */
            vcoinput = ((HSE_VALUE / (uint32_t)(RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SM)));  /* 赋值或初始化 */
          }                                             /* 代码块结束 */

          /* PLLI2S_VCO Output = PLLI2S_VCO Input * PLLI2SN */
          /* SAI_CLK(first level) = PLLI2S_VCO Output/PLLI2SQ */
          tmpreg1 = (RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SQ) >> 24U;  /* 赋值或初始化 */
          frequency = (vcoinput * ((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SN) >> 6U))/(tmpreg1);  /* 赋值或初始化 */

          /* SAI_CLK_x = SAI_CLK(first level)/PLLI2SDIVQ */
          tmpreg1 = ((RCC->DCKCFGR & RCC_DCKCFGR_PLLI2SDIVQ) + 1U);  /* 赋值或初始化 */
          frequency = frequency/(tmpreg1);              /* 赋值或初始化 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */
      case RCC_DCKCFGR_SAI1SRC_1: /* PLLR is the clock source for SAI*/
      case RCC_DCKCFGR_SAI2SRC_1: /* PLLR is the clock source for SAI*/
        {                                               /* 代码块开始 */
          /* Configure the PLLI2S division factor */
          /* PLL_VCO Input  = PLL_SOURCE/PLLM */
          if((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLSOURCE_HSI)  /* 条件判断 */
          {                                             /* 代码块开始 */
            /* In Case the PLL Source is HSI (Internal Clock) */
            vcoinput = (HSI_VALUE / (uint32_t)(RCC->PLLCFGR & RCC_PLLCFGR_PLLM));  /* 赋值或初始化 */
          }                                             /* 代码块结束 */
          else                                          /* 否则 */
          {                                             /* 代码块开始 */
            /* In Case the PLL Source is HSE (External Clock) */
            vcoinput = ((HSE_VALUE / (uint32_t)(RCC->PLLCFGR & RCC_PLLCFGR_PLLM)));  /* 赋值或初始化 */
          }                                             /* 代码块结束 */

          /* PLL_VCO Output = PLL_VCO Input * PLLN */
          /* SAI_CLK_x = PLL_VCO Output/PLLR */
          tmpreg1 = (RCC->PLLCFGR & RCC_PLLCFGR_PLLR) >> 28U;  /* 赋值或初始化 */
          frequency = (vcoinput * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6U))/(tmpreg1);  /* 赋值或初始化 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */
      case RCC_DCKCFGR_SAI1SRC: /* External clock is the clock source for SAI*/
        {                                               /* 代码块开始 */
          frequency = EXTERNAL_CLOCK_VALUE;             /* 赋值或初始化 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */
      case RCC_DCKCFGR_SAI2SRC: /* PLLSRC(HSE or HSI) is the clock source for SAI*/
        {                                               /* 代码块开始 */
          if((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLSOURCE_HSI)  /* 条件判断 */
          {                                             /* 代码块开始 */
            /* In Case the PLL Source is HSI (Internal Clock) */
            frequency = (uint32_t)(HSI_VALUE);          /* 赋值或初始化 */
          }                                             /* 代码块结束 */
          else                                          /* 否则 */
          {                                             /* 代码块开始 */
            /* In Case the PLL Source is HSE (External Clock) */
            frequency = (uint32_t)(HSE_VALUE);          /* 赋值或初始化 */
          }                                             /* 代码块结束 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */
      default :                                         /* default分支 */
        {                                               /* 代码块开始 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
  case RCC_PERIPHCLK_I2S_APB1:                          /* case标签 */
    {                                                   /* 代码块开始 */
      /* Get the current I2S source */
      srcclk = __HAL_RCC_GET_I2S_APB1_SOURCE();         /* 赋值或初始化 */
      switch (srcclk)                                   /* switch分支 */
      {                                                 /* 代码块开始 */
      /* Check if I2S clock selection is External clock mapped on the I2S_CKIN pin used as I2S clock */
      case RCC_I2SAPB1CLKSOURCE_EXT:                    /* case标签 */
        {                                               /* 代码块开始 */
          /* Set the I2S clock to the external clock  value */
          frequency = EXTERNAL_CLOCK_VALUE;             /* 赋值或初始化 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */
      /* Check if I2S clock selection is PLLI2S VCO output clock divided by PLLI2SR used as I2S clock */
      case RCC_I2SAPB1CLKSOURCE_PLLI2S:                 /* case标签 */
        {                                               /* 代码块开始 */
          /* Configure the PLLI2S division factor */
          /* PLLI2S_VCO Input  = PLL_SOURCE/PLLI2SM */
          if((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLSOURCE_HSE)  /* 条件判断 */
          {                                             /* 代码块开始 */
            /* Get the I2S source clock value */
            vcoinput = (uint32_t)(HSE_VALUE / (uint32_t)(RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SM));  /* 赋值或初始化 */
          }                                             /* 代码块结束 */
          else                                          /* 否则 */
          {                                             /* 代码块开始 */
            /* Get the I2S source clock value */
            vcoinput = (uint32_t)(HSI_VALUE / (uint32_t)(RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SM));  /* 赋值或初始化 */
          }                                             /* 代码块结束 */

          /* PLLI2S_VCO Output = PLLI2S_VCO Input * PLLI2SN */
          vcooutput = (uint32_t)(vcoinput * (((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SN) >> 6U) & (RCC_PLLI2SCFGR_PLLI2SN >> 6U)));  /* 赋值或初始化 */
          /* I2S_CLK = PLLI2S_VCO Output/PLLI2SR */
          frequency = (uint32_t)(vcooutput /(((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SR) >> 28U) & (RCC_PLLI2SCFGR_PLLI2SR >> 28U)));  /* 赋值或初始化 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */
      /* Check if I2S clock selection is PLL VCO Output divided by PLLR used as I2S clock */
      case RCC_I2SAPB1CLKSOURCE_PLLR:                   /* case标签 */
        {                                               /* 代码块开始 */
          /* Configure the PLL division factor R */
          /* PLL_VCO Input  = PLL_SOURCE/PLLM */
          if((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLSOURCE_HSE)  /* 条件判断 */
          {                                             /* 代码块开始 */
            /* Get the I2S source clock value */
            vcoinput = (uint32_t)(HSE_VALUE / (uint32_t)(RCC->PLLCFGR & RCC_PLLCFGR_PLLM));  /* 赋值或初始化 */
          }                                             /* 代码块结束 */
          else                                          /* 否则 */
          {                                             /* 代码块开始 */
            /* Get the I2S source clock value */
            vcoinput = (uint32_t)(HSI_VALUE / (uint32_t)(RCC->PLLCFGR & RCC_PLLCFGR_PLLM));  /* 赋值或初始化 */
          }                                             /* 代码块结束 */

          /* PLL_VCO Output = PLL_VCO Input * PLLN */
          vcooutput = (uint32_t)(vcoinput * (((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6U) & (RCC_PLLCFGR_PLLN >> 6U)));  /* 赋值或初始化 */
          /* I2S_CLK = PLL_VCO Output/PLLR */
          frequency = (uint32_t)(vcooutput /(((RCC->PLLCFGR & RCC_PLLCFGR_PLLR) >> 28U) & (RCC_PLLCFGR_PLLR >> 28U)));  /* 赋值或初始化 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */
      /* Check if I2S clock selection is HSI or HSE depending from PLL source Clock */
      case RCC_I2SAPB1CLKSOURCE_PLLSRC:                 /* case标签 */
        {                                               /* 代码块开始 */
          if((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLSOURCE_HSE)  /* 条件判断 */
          {                                             /* 代码块开始 */
            frequency = HSE_VALUE;                      /* 赋值或初始化 */
          }                                             /* 代码块结束 */
          else                                          /* 否则 */
          {                                             /* 代码块开始 */
            frequency = HSI_VALUE;                      /* 赋值或初始化 */
          }                                             /* 代码块结束 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */
        /* Clock not enabled for I2S*/
      default:                                          /* default分支 */
        {                                               /* 代码块开始 */
          frequency = 0U;                               /* 赋值或初始化 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
  case RCC_PERIPHCLK_I2S_APB2:                          /* case标签 */
    {                                                   /* 代码块开始 */
      /* Get the current I2S source */
      srcclk = __HAL_RCC_GET_I2S_APB2_SOURCE();         /* 赋值或初始化 */
      switch (srcclk)                                   /* switch分支 */
      {                                                 /* 代码块开始 */
        /* Check if I2S clock selection is External clock mapped on the I2S_CKIN pin used as I2S clock */
      case RCC_I2SAPB2CLKSOURCE_EXT:                    /* case标签 */
        {                                               /* 代码块开始 */
          /* Set the I2S clock to the external clock  value */
          frequency = EXTERNAL_CLOCK_VALUE;             /* 赋值或初始化 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */
        /* Check if I2S clock selection is PLLI2S VCO output clock divided by PLLI2SR used as I2S clock */
      case RCC_I2SAPB2CLKSOURCE_PLLI2S:                 /* case标签 */
        {                                               /* 代码块开始 */
          /* Configure the PLLI2S division factor */
          /* PLLI2S_VCO Input  = PLL_SOURCE/PLLI2SM */
          if((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLSOURCE_HSE)  /* 条件判断 */
          {                                             /* 代码块开始 */
            /* Get the I2S source clock value */
            vcoinput = (uint32_t)(HSE_VALUE / (uint32_t)(RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SM));  /* 赋值或初始化 */
          }                                             /* 代码块结束 */
          else                                          /* 否则 */
          {                                             /* 代码块开始 */
            /* Get the I2S source clock value */
            vcoinput = (uint32_t)(HSI_VALUE / (uint32_t)(RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SM));  /* 赋值或初始化 */
          }                                             /* 代码块结束 */

          /* PLLI2S_VCO Output = PLLI2S_VCO Input * PLLI2SN */
          vcooutput = (uint32_t)(vcoinput * (((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SN) >> 6U) & (RCC_PLLI2SCFGR_PLLI2SN >> 6U)));  /* 赋值或初始化 */
          /* I2S_CLK = PLLI2S_VCO Output/PLLI2SR */
          frequency = (uint32_t)(vcooutput /(((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SR) >> 28U) & (RCC_PLLI2SCFGR_PLLI2SR >> 28U)));  /* 赋值或初始化 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */
        /* Check if I2S clock selection is PLL VCO Output divided by PLLR used as I2S clock */
      case RCC_I2SAPB2CLKSOURCE_PLLR:                   /* case标签 */
        {                                               /* 代码块开始 */
          /* Configure the PLL division factor R */
          /* PLL_VCO Input  = PLL_SOURCE/PLLM */
          if((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLSOURCE_HSE)  /* 条件判断 */
          {                                             /* 代码块开始 */
            /* Get the I2S source clock value */
            vcoinput = (uint32_t)(HSE_VALUE / (uint32_t)(RCC->PLLCFGR & RCC_PLLCFGR_PLLM));  /* 赋值或初始化 */
          }                                             /* 代码块结束 */
          else                                          /* 否则 */
          {                                             /* 代码块开始 */
            /* Get the I2S source clock value */
            vcoinput = (uint32_t)(HSI_VALUE / (uint32_t)(RCC->PLLCFGR & RCC_PLLCFGR_PLLM));  /* 赋值或初始化 */
          }                                             /* 代码块结束 */

          /* PLL_VCO Output = PLL_VCO Input * PLLN */
          vcooutput = (uint32_t)(vcoinput * (((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6U) & (RCC_PLLCFGR_PLLN >> 6U)));  /* 赋值或初始化 */
          /* I2S_CLK = PLL_VCO Output/PLLR */
          frequency = (uint32_t)(vcooutput /(((RCC->PLLCFGR & RCC_PLLCFGR_PLLR) >> 28U) & (RCC_PLLCFGR_PLLR >> 28U)));  /* 赋值或初始化 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */
        /* Check if I2S clock selection is HSI or HSE depending from PLL source Clock */
      case RCC_I2SAPB2CLKSOURCE_PLLSRC:                 /* case标签 */
        {                                               /* 代码块开始 */
          if((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLSOURCE_HSE)  /* 条件判断 */
          {                                             /* 代码块开始 */
            frequency = HSE_VALUE;                      /* 赋值或初始化 */
          }                                             /* 代码块结束 */
          else                                          /* 否则 */
          {                                             /* 代码块开始 */
            frequency = HSI_VALUE;                      /* 赋值或初始化 */
          }                                             /* 代码块结束 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */
        /* Clock not enabled for I2S*/
      default:                                          /* default分支 */
        {                                               /* 代码块开始 */
          frequency = 0U;                               /* 赋值或初始化 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  return frequency;                                     /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* STM32F446xx */

#if defined(STM32F469xx) || defined(STM32F479xx)        /* 条件编译 */
/**
  * @brief  Initializes the RCC extended peripherals clocks according to the specified
  *         parameters in the RCC_PeriphCLKInitTypeDef.
  * @param  PeriphClkInit pointer to an RCC_PeriphCLKInitTypeDef structure that
  *         contains the configuration information for the Extended Peripherals
  *         clocks(I2S, SAI, LTDC, RTC and TIM).
  *
  * @note   Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select
  *         the RTC clock source; in this case the Backup domain will be reset in
  *         order to modify the RTC Clock source, as consequence RTC registers (including
  *         the backup registers) and RCC_BDCR register are set to their reset values.
  *
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCCEx_PeriphCLKConfig(RCC_PeriphCLKInitTypeDef  *PeriphClkInit)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart = 0U;                              /* 32位无符号变量/函数 */
  uint32_t tmpreg1 = 0U;                                /* 32位无符号变量/函数 */
  uint32_t pllsaip = 0U;                                /* 32位无符号变量/函数 */
  uint32_t pllsaiq = 0U;                                /* 32位无符号变量/函数 */
  uint32_t pllsair = 0U;                                /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_RCC_PERIPHCLOCK(PeriphClkInit->PeriphClockSelection));  /* 执行语句 */

  /*--------------------------- CLK48 Configuration --------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_CLK48) == RCC_PERIPHCLK_CLK48)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check the parameters */
    assert_param(IS_RCC_CLK48CLKSOURCE(PeriphClkInit->Clk48ClockSelection));  /* 执行语句 */

    /* Configure the CLK48 clock source */
    __HAL_RCC_CLK48_CONFIG(PeriphClkInit->Clk48ClockSelection);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  /*--------------------------------------------------------------------------*/

  /*------------------------------ SDIO Configuration ------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SDIO) == RCC_PERIPHCLK_SDIO)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check the parameters */
    assert_param(IS_RCC_SDIOCLKSOURCE(PeriphClkInit->SdioClockSelection));  /* 执行语句 */

    /* Configure the SDIO clock source */
    __HAL_RCC_SDIO_CONFIG(PeriphClkInit->SdioClockSelection);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  /*--------------------------------------------------------------------------*/

  /*----------------------- SAI/I2S Configuration (PLLI2S) -------------------*/
  /*------------------- Common configuration SAI/I2S -------------------------*/
  /* In Case of SAI or I2S Clock Configuration through PLLI2S, PLLI2SN division
     factor is common parameters for both peripherals */
  if((((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_I2S) == RCC_PERIPHCLK_I2S) ||  /* 条件判断 */
     (((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SAI_PLLI2S) == RCC_PERIPHCLK_SAI_PLLI2S) ||  /* 源代码 */
     (((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_PLLI2S) == RCC_PERIPHCLK_PLLI2S))  /* 源代码 */
  {                                                     /* 代码块开始 */
    /* check for Parameters */
    assert_param(IS_RCC_PLLI2SN_VALUE(PeriphClkInit->PLLI2S.PLLI2SN));  /* 执行语句 */

    /* Disable the PLLI2S */
    __HAL_RCC_PLLI2S_DISABLE();                         /* HAL辅助宏/函数 */
    /* Get tick */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */
    /* Wait till PLLI2S is disabled */
    while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLI2SRDY)  != RESET)  /* while循环 */
    {                                                   /* 代码块开始 */
      if((HAL_GetTick() - tickstart ) > PLLI2S_TIMEOUT_VALUE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* return in case of Timeout detected */
        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /*---------------------- I2S configuration -------------------------------*/
    /* In Case of I2S Clock Configuration through PLLI2S, PLLI2SR must be added
      only for I2S configuration */
    if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_I2S) == (RCC_PERIPHCLK_I2S))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* check for Parameters */
      assert_param(IS_RCC_PLLI2SR_VALUE(PeriphClkInit->PLLI2S.PLLI2SR));  /* 执行语句 */
      /* Configure the PLLI2S division factors */
      /* PLLI2S_VCO = f(VCO clock) = f(PLLI2S clock input) x (PLLI2SN/PLLM) */
      /* I2SCLK = f(PLLI2S clock output) = f(VCO clock) / PLLI2SR */
      __HAL_RCC_PLLI2S_CONFIG(PeriphClkInit->PLLI2S.PLLI2SN , PeriphClkInit->PLLI2S.PLLI2SR);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /*---------------------------- SAI configuration -------------------------*/
    /* In Case of SAI Clock Configuration through PLLI2S, PLLI2SQ and PLLI2S_DIVQ must
       be added only for SAI configuration */
    if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SAI_PLLI2S) == (RCC_PERIPHCLK_SAI_PLLI2S))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Check the PLLI2S division factors */
      assert_param(IS_RCC_PLLI2SQ_VALUE(PeriphClkInit->PLLI2S.PLLI2SQ));  /* 执行语句 */
      assert_param(IS_RCC_PLLI2S_DIVQ_VALUE(PeriphClkInit->PLLI2SDivQ));  /* 执行语句 */

      /* Read PLLI2SR value from PLLI2SCFGR register (this value is not need for SAI configuration) */
      tmpreg1 = ((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SR) >> RCC_PLLI2SCFGR_PLLI2SR_Pos);  /* 赋值或初始化 */
      /* Configure the PLLI2S division factors */
      /* PLLI2S_VCO Input  = PLL_SOURCE/PLLM */
      /* PLLI2S_VCO Output = PLLI2S_VCO Input * PLLI2SN */
      /* SAI_CLK(first level) = PLLI2S_VCO Output/PLLI2SQ */
      __HAL_RCC_PLLI2S_SAICLK_CONFIG(PeriphClkInit->PLLI2S.PLLI2SN , PeriphClkInit->PLLI2S.PLLI2SQ , tmpreg1);  /* HAL辅助宏/函数 */
      /* SAI_CLK_x = SAI_CLK(first level)/PLLI2SDIVQ */
      __HAL_RCC_PLLI2S_PLLSAICLKDIVQ_CONFIG(PeriphClkInit->PLLI2SDivQ);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /*----------------- In Case of PLLI2S is just selected  -----------------*/
    if((PeriphClkInit->PeriphClockSelection & RCC_PERIPHCLK_PLLI2S) == RCC_PERIPHCLK_PLLI2S)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Check for Parameters */
      assert_param(IS_RCC_PLLI2SQ_VALUE(PeriphClkInit->PLLI2S.PLLI2SQ));  /* 执行语句 */
      assert_param(IS_RCC_PLLI2SR_VALUE(PeriphClkInit->PLLI2S.PLLI2SR));  /* 执行语句 */

      /* Configure the PLLI2S multiplication and division factors */
      __HAL_RCC_PLLI2S_SAICLK_CONFIG(PeriphClkInit->PLLI2S.PLLI2SN, PeriphClkInit->PLLI2S.PLLI2SQ, PeriphClkInit->PLLI2S.PLLI2SR);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Enable the PLLI2S */
    __HAL_RCC_PLLI2S_ENABLE();                          /* HAL辅助宏/函数 */
    /* Get tick */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */
    /* Wait till PLLI2S is ready */
    while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLI2SRDY)  == RESET)  /* while循环 */
    {                                                   /* 代码块开始 */
      if((HAL_GetTick() - tickstart ) > PLLI2S_TIMEOUT_VALUE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* return in case of Timeout detected */
        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /*--------------------------------------------------------------------------*/

  /*----------------------- SAI/LTDC Configuration (PLLSAI) ------------------*/
  /*----------------------- Common configuration SAI/LTDC --------------------*/
  /* In Case of SAI, LTDC or CLK48 Clock Configuration through PLLSAI, PLLSAIN division
     factor is common parameters for these peripherals */
  if((((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SAI_PLLSAI) == RCC_PERIPHCLK_SAI_PLLSAI) ||  /* 条件判断 */
     (((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_LTDC) == RCC_PERIPHCLK_LTDC)             ||  /* 源代码 */
     ((((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_CLK48) == RCC_PERIPHCLK_CLK48)          &&  /* 源代码 */
      (PeriphClkInit->Clk48ClockSelection == RCC_CLK48CLKSOURCE_PLLSAIP)))  /* 源代码 */
  {                                                     /* 代码块开始 */
    /* Check the PLLSAI division factors */
    assert_param(IS_RCC_PLLSAIN_VALUE(PeriphClkInit->PLLSAI.PLLSAIN));  /* 执行语句 */

    /* Disable PLLSAI Clock */
    __HAL_RCC_PLLSAI_DISABLE();                         /* HAL辅助宏/函数 */
    /* Get tick */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */
    /* Wait till PLLSAI is disabled */
    while(__HAL_RCC_PLLSAI_GET_FLAG() != RESET)         /* while循环 */
    {                                                   /* 代码块开始 */
      if((HAL_GetTick() - tickstart ) > PLLSAI_TIMEOUT_VALUE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* return in case of Timeout detected */
        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /*---------------------------- SAI configuration -------------------------*/
    /* In Case of SAI Clock Configuration through PLLSAI, PLLSAIQ and PLLSAI_DIVQ must
       be added only for SAI configuration */
    if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SAI_PLLSAI) == (RCC_PERIPHCLK_SAI_PLLSAI))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      assert_param(IS_RCC_PLLSAIQ_VALUE(PeriphClkInit->PLLSAI.PLLSAIQ));  /* 执行语句 */
      assert_param(IS_RCC_PLLSAI_DIVQ_VALUE(PeriphClkInit->PLLSAIDivQ));  /* 执行语句 */

      /* Read PLLSAIP value from PLLSAICFGR register (this value is not needed for SAI configuration) */
      pllsaip = ((((RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIP) >> RCC_PLLSAICFGR_PLLSAIP_Pos) + 1U) << 1U);  /* 赋值或初始化 */
      /* Read PLLSAIR value from PLLSAICFGR register (this value is not need for SAI configuration) */
      pllsair = ((RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIR) >> RCC_PLLSAICFGR_PLLSAIR_Pos);  /* 赋值或初始化 */
      /* PLLSAI_VCO Input  = PLL_SOURCE/PLLM */
      /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN */
      /* SAI_CLK(first level) = PLLSAI_VCO Output/PLLSAIQ */
      __HAL_RCC_PLLSAI_CONFIG(PeriphClkInit->PLLSAI.PLLSAIN, pllsaip, PeriphClkInit->PLLSAI.PLLSAIQ, pllsair);  /* HAL辅助宏/函数 */
      /* SAI_CLK_x = SAI_CLK(first level)/PLLSAIDIVQ */
      __HAL_RCC_PLLSAI_PLLSAICLKDIVQ_CONFIG(PeriphClkInit->PLLSAIDivQ);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /*---------------------------- LTDC configuration ------------------------*/
    if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_LTDC) == (RCC_PERIPHCLK_LTDC))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      assert_param(IS_RCC_PLLSAIR_VALUE(PeriphClkInit->PLLSAI.PLLSAIR));  /* 执行语句 */
      assert_param(IS_RCC_PLLSAI_DIVR_VALUE(PeriphClkInit->PLLSAIDivR));  /* 执行语句 */

      /* Read PLLSAIP value from PLLSAICFGR register (this value is not needed for SAI configuration) */
      pllsaip = ((((RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIP) >> RCC_PLLSAICFGR_PLLSAIP_Pos) + 1U) << 1U);  /* 赋值或初始化 */
      /* Read PLLSAIQ value from PLLSAICFGR register (this value is not need for SAI configuration) */
      pllsaiq = ((RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIQ) >> RCC_PLLSAICFGR_PLLSAIQ_Pos);  /* 赋值或初始化 */
      /* PLLSAI_VCO Input  = PLL_SOURCE/PLLM */
      /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN */
      /* LTDC_CLK(first level) = PLLSAI_VCO Output/PLLSAIR */
      __HAL_RCC_PLLSAI_CONFIG(PeriphClkInit->PLLSAI.PLLSAIN, pllsaip, pllsaiq, PeriphClkInit->PLLSAI.PLLSAIR);  /* HAL辅助宏/函数 */
      /* LTDC_CLK = LTDC_CLK(first level)/PLLSAIDIVR */
      __HAL_RCC_PLLSAI_PLLSAICLKDIVR_CONFIG(PeriphClkInit->PLLSAIDivR);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /*---------------------------- CLK48 configuration ------------------------*/
    /* Configure the PLLSAI when it is used as clock source for CLK48 */
    if((((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_CLK48) == (RCC_PERIPHCLK_CLK48)) &&  /* 条件判断 */
       (PeriphClkInit->Clk48ClockSelection == RCC_CLK48CLKSOURCE_PLLSAIP))  /* 源代码 */
    {                                                   /* 代码块开始 */
      assert_param(IS_RCC_PLLSAIP_VALUE(PeriphClkInit->PLLSAI.PLLSAIP));  /* 执行语句 */

      /* Read PLLSAIQ value from PLLSAICFGR register (this value is not need for SAI configuration) */
      pllsaiq = ((RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIQ) >> RCC_PLLSAICFGR_PLLSAIQ_Pos);  /* 赋值或初始化 */
      /* Read PLLSAIR value from PLLSAICFGR register (this value is not need for SAI configuration) */
      pllsair = ((RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIR) >> RCC_PLLSAICFGR_PLLSAIR_Pos);  /* 赋值或初始化 */
      /* PLLSAI_VCO Input  = PLL_SOURCE/PLLM */
      /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN */
      /* CLK48_CLK(first level) = PLLSAI_VCO Output/PLLSAIP */
      __HAL_RCC_PLLSAI_CONFIG(PeriphClkInit->PLLSAI.PLLSAIN, PeriphClkInit->PLLSAI.PLLSAIP, pllsaiq, pllsair);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Enable PLLSAI Clock */
    __HAL_RCC_PLLSAI_ENABLE();                          /* HAL辅助宏/函数 */
    /* Get tick */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */
    /* Wait till PLLSAI is ready */
    while(__HAL_RCC_PLLSAI_GET_FLAG() == RESET)         /* while循环 */
    {                                                   /* 代码块开始 */
      if((HAL_GetTick() - tickstart ) > PLLSAI_TIMEOUT_VALUE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* return in case of Timeout detected */
        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /*--------------------------------------------------------------------------*/

  /*---------------------------- RTC configuration ---------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_RTC) == (RCC_PERIPHCLK_RTC))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check for RTC Parameters used to output RTCCLK */
    assert_param(IS_RCC_RTCCLKSOURCE(PeriphClkInit->RTCClockSelection));  /* 执行语句 */

    /* Enable Power Clock*/
    __HAL_RCC_PWR_CLK_ENABLE();                         /* HAL辅助宏/函数 */

    /* Enable write access to Backup domain */
    PWR->CR |= PWR_CR_DBP;                              /* 赋值或初始化 */

    /* Get tick */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */

    while((PWR->CR & PWR_CR_DBP) == RESET)              /* while循环 */
    {                                                   /* 代码块开始 */
      if((HAL_GetTick() - tickstart ) > RCC_DBP_TIMEOUT_VALUE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    /* Reset the Backup domain only if the RTC Clock source selection is modified from reset value */
    tmpreg1 = (RCC->BDCR & RCC_BDCR_RTCSEL);            /* 赋值或初始化 */
    if((tmpreg1 != 0x00000000U) && ((tmpreg1) != (PeriphClkInit->RTCClockSelection & RCC_BDCR_RTCSEL)))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Store the content of BDCR register before the reset of Backup Domain */
      tmpreg1 = (RCC->BDCR & ~(RCC_BDCR_RTCSEL));       /* 赋值或初始化 */
      /* RTC Clock selection can be changed only if the Backup Domain is reset */
      __HAL_RCC_BACKUPRESET_FORCE();                    /* HAL辅助宏/函数 */
      __HAL_RCC_BACKUPRESET_RELEASE();                  /* HAL辅助宏/函数 */
      /* Restore the Content of BDCR register */
      RCC->BDCR = tmpreg1;                              /* 赋值或初始化 */

      /* Wait for LSE reactivation if LSE was enable prior to Backup Domain reset */
      if(HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSEON))     /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Get tick */
        tickstart = HAL_GetTick();                      /* 赋值或初始化 */

        /* Wait till LSE is ready */
        while(__HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY) == RESET)  /* while循环 */
        {                                               /* 代码块开始 */
          if((HAL_GetTick() - tickstart ) > RCC_LSE_TIMEOUT_VALUE)  /* 条件判断 */
          {                                             /* 代码块开始 */
            return HAL_TIMEOUT;                         /* 返回值 */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    __HAL_RCC_RTC_CONFIG(PeriphClkInit->RTCClockSelection);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  /*--------------------------------------------------------------------------*/

  /*---------------------------- TIM configuration ---------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_TIM) == (RCC_PERIPHCLK_TIM))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    __HAL_RCC_TIMCLKPRESCALER(PeriphClkInit->TIMPresSelection);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configures the RCC_PeriphCLKInitTypeDef according to the internal
  * RCC configuration registers.
  * @param  PeriphClkInit pointer to an RCC_PeriphCLKInitTypeDef structure that
  *         will be configured.
  * @retval None
  */
void HAL_RCCEx_GetPeriphCLKConfig(RCC_PeriphCLKInitTypeDef  *PeriphClkInit)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  uint32_t tempreg;                                     /* 32位无符号变量/函数 */

  /* Set all possible values for the extended clock type parameter------------*/
  PeriphClkInit->PeriphClockSelection = RCC_PERIPHCLK_I2S        | RCC_PERIPHCLK_SAI_PLLSAI |\
                                        RCC_PERIPHCLK_SAI_PLLI2S | RCC_PERIPHCLK_LTDC       |\
                                        RCC_PERIPHCLK_TIM        | RCC_PERIPHCLK_RTC        |\
                                        RCC_PERIPHCLK_CLK48       | RCC_PERIPHCLK_SDIO;  /* 时钟相关操作 */

  /* Get the PLLI2S Clock configuration --------------------------------------*/
  PeriphClkInit->PLLI2S.PLLI2SN = (uint32_t)((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SN) >> RCC_PLLI2SCFGR_PLLI2SN_Pos);  /* 赋值或初始化 */
  PeriphClkInit->PLLI2S.PLLI2SR = (uint32_t)((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SR) >> RCC_PLLI2SCFGR_PLLI2SR_Pos);  /* 赋值或初始化 */
  PeriphClkInit->PLLI2S.PLLI2SQ = (uint32_t)((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SQ) >> RCC_PLLI2SCFGR_PLLI2SQ_Pos);  /* 赋值或初始化 */
  /* Get the PLLSAI Clock configuration --------------------------------------*/
  PeriphClkInit->PLLSAI.PLLSAIN = (uint32_t)((RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIN) >> RCC_PLLSAICFGR_PLLSAIN_Pos);  /* 赋值或初始化 */
  PeriphClkInit->PLLSAI.PLLSAIR = (uint32_t)((RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIR) >> RCC_PLLSAICFGR_PLLSAIR_Pos);  /* 赋值或初始化 */
  PeriphClkInit->PLLSAI.PLLSAIQ = (uint32_t)((RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIQ) >> RCC_PLLSAICFGR_PLLSAIQ_Pos);  /* 赋值或初始化 */
  /* Get the PLLSAI/PLLI2S division factors ----------------------------------*/
  PeriphClkInit->PLLI2SDivQ = (uint32_t)((RCC->DCKCFGR & RCC_DCKCFGR_PLLI2SDIVQ) >> RCC_DCKCFGR_PLLI2SDIVQ_Pos);  /* 赋值或初始化 */
  PeriphClkInit->PLLSAIDivQ = (uint32_t)((RCC->DCKCFGR & RCC_DCKCFGR_PLLSAIDIVQ) >> RCC_DCKCFGR_PLLSAIDIVQ_Pos);  /* 赋值或初始化 */
  PeriphClkInit->PLLSAIDivR = (uint32_t)(RCC->DCKCFGR & RCC_DCKCFGR_PLLSAIDIVR);  /* 赋值或初始化 */
  /* Get the RTC Clock configuration -----------------------------------------*/
  tempreg = (RCC->CFGR & RCC_CFGR_RTCPRE);              /* 赋值或初始化 */
  PeriphClkInit->RTCClockSelection = (uint32_t)((tempreg) | (RCC->BDCR & RCC_BDCR_RTCSEL));  /* 赋值或初始化 */

    /* Get the CLK48 clock configuration -------------------------------------*/
  PeriphClkInit->Clk48ClockSelection = __HAL_RCC_GET_CLK48_SOURCE();  /* 赋值或初始化 */

  /* Get the SDIO clock configuration ----------------------------------------*/
  PeriphClkInit->SdioClockSelection = __HAL_RCC_GET_SDIO_SOURCE();  /* 赋值或初始化 */

  if ((RCC->DCKCFGR & RCC_DCKCFGR_TIMPRE) == RESET)     /* 条件判断 */
  {                                                     /* 代码块开始 */
    PeriphClkInit->TIMPresSelection = RCC_TIMPRES_DESACTIVATED;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    PeriphClkInit->TIMPresSelection = RCC_TIMPRES_ACTIVATED;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return the peripheral clock frequency for a given peripheral(SAI..)
  * @note   Return 0 if peripheral clock identifier not managed by this API
  * @param  PeriphClk Peripheral clock identifier
  *         This parameter can be one of the following values:
  *            @arg RCC_PERIPHCLK_I2S: I2S peripheral clock
  * @retval Frequency in KHz
  */
uint32_t HAL_RCCEx_GetPeriphCLKFreq(uint32_t PeriphClk)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  /* This variable used to store the I2S clock frequency (value in Hz) */
  uint32_t frequency = 0U;                              /* 32位无符号变量/函数 */
  /* This variable used to store the VCO Input (value in Hz) */
  uint32_t vcoinput = 0U;                               /* 32位无符号变量/函数 */
  uint32_t srcclk = 0U;                                 /* 32位无符号变量/函数 */
  /* This variable used to store the VCO Output (value in Hz) */
  uint32_t vcooutput = 0U;                              /* 32位无符号变量/函数 */
  switch (PeriphClk)                                    /* switch分支 */
  {                                                     /* 代码块开始 */
  case RCC_PERIPHCLK_I2S:                               /* case标签 */
    {                                                   /* 代码块开始 */
      /* Get the current I2S source */
      srcclk = __HAL_RCC_GET_I2S_SOURCE();              /* 赋值或初始化 */
      switch (srcclk)                                   /* switch分支 */
      {                                                 /* 代码块开始 */
      /* Check if I2S clock selection is External clock mapped on the I2S_CKIN pin used as I2S clock */
      case RCC_I2SCLKSOURCE_EXT:                        /* case标签 */
        {                                               /* 代码块开始 */
          /* Set the I2S clock to the external clock  value */
          frequency = EXTERNAL_CLOCK_VALUE;             /* 赋值或初始化 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */
      /* Check if I2S clock selection is PLLI2S VCO output clock divided by PLLI2SR used as I2S clock */
      case RCC_I2SCLKSOURCE_PLLI2S:                     /* case标签 */
        {                                               /* 代码块开始 */
          /* Configure the PLLI2S division factor */
          /* PLLI2S_VCO Input  = PLL_SOURCE/PLLI2SM */
          if((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLSOURCE_HSE)  /* 条件判断 */
          {                                             /* 代码块开始 */
            /* Get the I2S source clock value */
            vcoinput = (uint32_t)(HSE_VALUE / (uint32_t)(RCC->PLLCFGR & RCC_PLLCFGR_PLLM));  /* 赋值或初始化 */
          }                                             /* 代码块结束 */
          else                                          /* 否则 */
          {                                             /* 代码块开始 */
            /* Get the I2S source clock value */
            vcoinput = (uint32_t)(HSI_VALUE / (uint32_t)(RCC->PLLCFGR & RCC_PLLCFGR_PLLM));  /* 赋值或初始化 */
          }                                             /* 代码块结束 */

          /* PLLI2S_VCO Output = PLLI2S_VCO Input * PLLI2SN */
          vcooutput = (uint32_t)(vcoinput * (((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SN) >> 6U) & (RCC_PLLI2SCFGR_PLLI2SN >> 6U)));  /* 赋值或初始化 */
          /* I2S_CLK = PLLI2S_VCO Output/PLLI2SR */
          frequency = (uint32_t)(vcooutput /(((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SR) >> 28U) & (RCC_PLLI2SCFGR_PLLI2SR >> 28U)));  /* 赋值或初始化 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */
        /* Clock not enabled for I2S*/
      default:                                          /* default分支 */
        {                                               /* 代码块开始 */
          frequency = 0U;                               /* 赋值或初始化 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  return frequency;                                     /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* STM32F469xx || STM32F479xx */

#if defined(STM32F412Zx) || defined(STM32F412Vx) || defined(STM32F412Rx) || defined(STM32F412Cx) || defined(STM32F413xx) || defined(STM32F423xx)  /* 条件编译 */
/**
  * @brief  Initializes the RCC extended peripherals clocks according to the specified
  *         parameters in the RCC_PeriphCLKInitTypeDef.
  * @param  PeriphClkInit pointer to an RCC_PeriphCLKInitTypeDef structure that
  *         contains the configuration information for the Extended Peripherals
  *         clocks(I2S, LTDC RTC and TIM).
  *
  * @note   Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select
  *         the RTC clock source; in this case the Backup domain will be reset in
  *         order to modify the RTC Clock source, as consequence RTC registers (including
  *         the backup registers) and RCC_BDCR register are set to their reset values.
  *
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCCEx_PeriphCLKConfig(RCC_PeriphCLKInitTypeDef  *PeriphClkInit)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart = 0U;                              /* 32位无符号变量/函数 */
  uint32_t tmpreg1 = 0U;                                /* 32位无符号变量/函数 */
#if defined(STM32F413xx) || defined(STM32F423xx)        /* 条件编译 */
  uint32_t plli2sq = 0U;                                /* 32位无符号变量/函数 */
#endif /* STM32F413xx || STM32F423xx */
  uint32_t plli2sused = 0U;                             /* 32位无符号变量/函数 */

  /* Check the peripheral clock selection parameters */
  assert_param(IS_RCC_PERIPHCLOCK(PeriphClkInit->PeriphClockSelection));  /* 执行语句 */

  /*----------------------------------- I2S APB1 configuration ---------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_I2S_APB1) == (RCC_PERIPHCLK_I2S_APB1))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check the parameters */
    assert_param(IS_RCC_I2SAPB1CLKSOURCE(PeriphClkInit->I2sApb1ClockSelection));  /* 执行语句 */

    /* Configure I2S Clock source */
    __HAL_RCC_I2S_APB1_CONFIG(PeriphClkInit->I2sApb1ClockSelection);  /* HAL辅助宏/函数 */
    /* Enable the PLLI2S when it's used as clock source for I2S */
    if(PeriphClkInit->I2sApb1ClockSelection == RCC_I2SAPB1CLKSOURCE_PLLI2S)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      plli2sused = 1U;                                  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /*--------------------------------------------------------------------------*/

  /*----------------------------------- I2S APB2 configuration ---------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_I2S_APB2) == (RCC_PERIPHCLK_I2S_APB2))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check the parameters */
    assert_param(IS_RCC_I2SAPB2CLKSOURCE(PeriphClkInit->I2sApb2ClockSelection));  /* 执行语句 */

    /* Configure I2S Clock source */
    __HAL_RCC_I2S_APB2_CONFIG(PeriphClkInit->I2sApb2ClockSelection);  /* HAL辅助宏/函数 */
    /* Enable the PLLI2S when it's used as clock source for I2S */
    if(PeriphClkInit->I2sApb2ClockSelection == RCC_I2SAPB2CLKSOURCE_PLLI2S)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      plli2sused = 1U;                                  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /*--------------------------------------------------------------------------*/

#if defined(STM32F413xx) || defined(STM32F423xx)        /* 条件编译 */
  /*----------------------- SAI1 Block A configuration -----------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SAIA) == (RCC_PERIPHCLK_SAIA))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check the parameters */
    assert_param(IS_RCC_SAIACLKSOURCE(PeriphClkInit->SaiAClockSelection));  /* 执行语句 */

    /* Configure SAI1 Clock source */
    __HAL_RCC_SAI_BLOCKACLKSOURCE_CONFIG(PeriphClkInit->SaiAClockSelection);  /* HAL辅助宏/函数 */
    /* Enable the PLLI2S when it's used as clock source for SAI */
    if(PeriphClkInit->SaiAClockSelection == RCC_SAIACLKSOURCE_PLLI2SR)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      plli2sused = 1U;                                  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    /* Enable the PLLSAI when it's used as clock source for SAI */
    if(PeriphClkInit->SaiAClockSelection == RCC_SAIACLKSOURCE_PLLR)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Check for PLL/DIVR parameters */
      assert_param(IS_RCC_PLL_DIVR_VALUE(PeriphClkInit->PLLDivR));  /* 执行语句 */

      /* SAI_CLK_x = SAI_CLK(first level)/PLLDIVR */
      __HAL_RCC_PLL_PLLSAICLKDIVR_CONFIG(PeriphClkInit->PLLDivR);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /*--------------------------------------------------------------------------*/

  /*---------------------- SAI1 Block B configuration ------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SAIB) == (RCC_PERIPHCLK_SAIB))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check the parameters */
    assert_param(IS_RCC_SAIBCLKSOURCE(PeriphClkInit->SaiBClockSelection));  /* 执行语句 */

    /* Configure SAI1 Clock source */
    __HAL_RCC_SAI_BLOCKBCLKSOURCE_CONFIG(PeriphClkInit->SaiBClockSelection);  /* HAL辅助宏/函数 */
    /* Enable the PLLI2S when it's used as clock source for SAI */
    if(PeriphClkInit->SaiBClockSelection == RCC_SAIBCLKSOURCE_PLLI2SR)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      plli2sused = 1U;                                  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    /* Enable the PLLSAI when it's used as clock source for SAI */
    if(PeriphClkInit->SaiBClockSelection == RCC_SAIBCLKSOURCE_PLLR)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Check for PLL/DIVR parameters */
      assert_param(IS_RCC_PLL_DIVR_VALUE(PeriphClkInit->PLLDivR));  /* 执行语句 */

      /* SAI_CLK_x = SAI_CLK(first level)/PLLDIVR */
      __HAL_RCC_PLL_PLLSAICLKDIVR_CONFIG(PeriphClkInit->PLLDivR);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /*--------------------------------------------------------------------------*/
#endif /* STM32F413xx || STM32F423xx */

  /*------------------------------------ RTC configuration -------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_RTC) == (RCC_PERIPHCLK_RTC))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check for RTC Parameters used to output RTCCLK */
    assert_param(IS_RCC_RTCCLKSOURCE(PeriphClkInit->RTCClockSelection));  /* 执行语句 */

    /* Enable Power Clock*/
    __HAL_RCC_PWR_CLK_ENABLE();                         /* HAL辅助宏/函数 */

    /* Enable write access to Backup domain */
    PWR->CR |= PWR_CR_DBP;                              /* 赋值或初始化 */

    /* Get tick */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */

    while((PWR->CR & PWR_CR_DBP) == RESET)              /* while循环 */
    {                                                   /* 代码块开始 */
      if((HAL_GetTick() - tickstart ) > RCC_DBP_TIMEOUT_VALUE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    /* Reset the Backup domain only if the RTC Clock source selection is modified from reset value */
    tmpreg1 = (RCC->BDCR & RCC_BDCR_RTCSEL);            /* 赋值或初始化 */
    if((tmpreg1 != 0x00000000U) && ((tmpreg1) != (PeriphClkInit->RTCClockSelection & RCC_BDCR_RTCSEL)))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Store the content of BDCR register before the reset of Backup Domain */
      tmpreg1 = (RCC->BDCR & ~(RCC_BDCR_RTCSEL));       /* 赋值或初始化 */
      /* RTC Clock selection can be changed only if the Backup Domain is reset */
      __HAL_RCC_BACKUPRESET_FORCE();                    /* HAL辅助宏/函数 */
      __HAL_RCC_BACKUPRESET_RELEASE();                  /* HAL辅助宏/函数 */
      /* Restore the Content of BDCR register */
      RCC->BDCR = tmpreg1;                              /* 赋值或初始化 */

      /* Wait for LSE reactivation if LSE was enable prior to Backup Domain reset */
      if(HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSEON))     /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Get tick */
        tickstart = HAL_GetTick();                      /* 赋值或初始化 */

        /* Wait till LSE is ready */
        while(__HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY) == RESET)  /* while循环 */
        {                                               /* 代码块开始 */
          if((HAL_GetTick() - tickstart ) > RCC_LSE_TIMEOUT_VALUE)  /* 条件判断 */
          {                                             /* 代码块开始 */
            return HAL_TIMEOUT;                         /* 返回值 */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    __HAL_RCC_RTC_CONFIG(PeriphClkInit->RTCClockSelection);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  /*--------------------------------------------------------------------------*/

  /*------------------------------------ TIM configuration -------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_TIM) == (RCC_PERIPHCLK_TIM))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Configure Timer Prescaler */
    __HAL_RCC_TIMCLKPRESCALER(PeriphClkInit->TIMPresSelection);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  /*--------------------------------------------------------------------------*/

  /*------------------------------------- FMPI2C1 Configuration --------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_FMPI2C1) == RCC_PERIPHCLK_FMPI2C1)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check the parameters */
    assert_param(IS_RCC_FMPI2C1CLKSOURCE(PeriphClkInit->Fmpi2c1ClockSelection));  /* 执行语句 */

    /* Configure the FMPI2C1 clock source */
    __HAL_RCC_FMPI2C1_CONFIG(PeriphClkInit->Fmpi2c1ClockSelection);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  /*--------------------------------------------------------------------------*/

  /*------------------------------------- CLK48 Configuration ----------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_CLK48) == RCC_PERIPHCLK_CLK48)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check the parameters */
    assert_param(IS_RCC_CLK48CLKSOURCE(PeriphClkInit->Clk48ClockSelection));  /* 执行语句 */

    /* Configure the SDIO clock source */
    __HAL_RCC_CLK48_CONFIG(PeriphClkInit->Clk48ClockSelection);  /* HAL辅助宏/函数 */

    /* Enable the PLLI2S when it's used as clock source for CLK48 */
    if(PeriphClkInit->Clk48ClockSelection == RCC_CLK48CLKSOURCE_PLLI2SQ)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      plli2sused = 1U;                                  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /*--------------------------------------------------------------------------*/

  /*------------------------------------- SDIO Configuration -----------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SDIO) == RCC_PERIPHCLK_SDIO)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check the parameters */
    assert_param(IS_RCC_SDIOCLKSOURCE(PeriphClkInit->SdioClockSelection));  /* 执行语句 */

    /* Configure the SDIO clock source */
    __HAL_RCC_SDIO_CONFIG(PeriphClkInit->SdioClockSelection);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  /*--------------------------------------------------------------------------*/

  /*-------------------------------------- PLLI2S Configuration --------------*/
  /* PLLI2S is configured when a peripheral will use it as source clock : I2S on APB1 or
     I2S on APB2*/
  if((plli2sused == 1U) || (PeriphClkInit->PeriphClockSelection == RCC_PERIPHCLK_PLLI2S))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable the PLLI2S */
    __HAL_RCC_PLLI2S_DISABLE();                         /* HAL辅助宏/函数 */
    /* Get tick */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */
    /* Wait till PLLI2S is disabled */
    while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLI2SRDY)  != RESET)  /* while循环 */
    {                                                   /* 代码块开始 */
      if((HAL_GetTick() - tickstart ) > PLLI2S_TIMEOUT_VALUE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* return in case of Timeout detected */
        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* check for common PLLI2S Parameters */
    assert_param(IS_RCC_PLLI2SCLKSOURCE(PeriphClkInit->PLLI2SSelection));  /* 执行语句 */
    assert_param(IS_RCC_PLLI2SM_VALUE(PeriphClkInit->PLLI2S.PLLI2SM));  /* 执行语句 */
    assert_param(IS_RCC_PLLI2SN_VALUE(PeriphClkInit->PLLI2S.PLLI2SN));  /* 执行语句 */
    /*-------------------- Set the PLL I2S clock -----------------------------*/
    __HAL_RCC_PLL_I2S_CONFIG(PeriphClkInit->PLLI2SSelection);  /* HAL辅助宏/函数 */

    /*------- In Case of PLLI2S is selected as source clock for I2S ----------*/
    if(((((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_I2S_APB1) == RCC_PERIPHCLK_I2S_APB1) && (PeriphClkInit->I2sApb1ClockSelection == RCC_I2SAPB1CLKSOURCE_PLLI2S)) ||  /* 条件判断 */
       ((((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_I2S_APB2) == RCC_PERIPHCLK_I2S_APB2) && (PeriphClkInit->I2sApb2ClockSelection == RCC_I2SAPB2CLKSOURCE_PLLI2S)) ||  /* 源代码 */
       ((((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_CLK48) == RCC_PERIPHCLK_CLK48) && (PeriphClkInit->Clk48ClockSelection == RCC_CLK48CLKSOURCE_PLLI2SQ)) ||  /* 源代码 */
       ((((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SDIO) == RCC_PERIPHCLK_SDIO) && (PeriphClkInit->SdioClockSelection == RCC_SDIOCLKSOURCE_CLK48) && (PeriphClkInit->Clk48ClockSelection == RCC_CLK48CLKSOURCE_PLLI2SQ)))  /* 源代码 */
    {                                                   /* 代码块开始 */
      /* check for Parameters */
      assert_param(IS_RCC_PLLI2SR_VALUE(PeriphClkInit->PLLI2S.PLLI2SR));  /* 执行语句 */
      assert_param(IS_RCC_PLLI2SQ_VALUE(PeriphClkInit->PLLI2S.PLLI2SQ));  /* 执行语句 */

      /* Configure the PLLI2S division factors */
      /* PLLI2S_VCO = f(VCO clock) = f(PLLI2S clock input) * (PLLI2SN/PLLI2SM)*/
      /* I2SCLK = f(PLLI2S clock output) = f(VCO clock) / PLLI2SR */
      __HAL_RCC_PLLI2S_CONFIG(PeriphClkInit->PLLI2S.PLLI2SM, PeriphClkInit->PLLI2S.PLLI2SN , PeriphClkInit->PLLI2S.PLLI2SQ, PeriphClkInit->PLLI2S.PLLI2SR);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

#if defined(STM32F413xx) || defined(STM32F423xx)        /* 条件编译 */
    /*------- In Case of PLLI2S is selected as source clock for SAI ----------*/
    if(((((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SAIA) == RCC_PERIPHCLK_SAIA) && (PeriphClkInit->SaiAClockSelection == RCC_SAIACLKSOURCE_PLLI2SR)) ||  /* 条件判断 */
       ((((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SAIB) == RCC_PERIPHCLK_SAIB) && (PeriphClkInit->SaiBClockSelection == RCC_SAIBCLKSOURCE_PLLI2SR)))  /* 源代码 */
    {                                                   /* 代码块开始 */
      /* Check for PLLI2S Parameters */
      assert_param(IS_RCC_PLLI2SR_VALUE(PeriphClkInit->PLLI2S.PLLI2SR));  /* 执行语句 */
      /* Check for PLLI2S/DIVR parameters */
      assert_param(IS_RCC_PLLI2S_DIVR_VALUE(PeriphClkInit->PLLI2SDivR));  /* 执行语句 */

      /* Read PLLI2SQ value from PLLI2SCFGR register (this value is not needed for SAI configuration) */
      plli2sq = ((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SQ) >> RCC_PLLI2SCFGR_PLLI2SQ_Pos);  /* 赋值或初始化 */
      /* Configure the PLLI2S division factors */
      /* PLLI2S_VCO Input  = PLL_SOURCE/PLLI2SM */
      /* PLLI2S_VCO Output = PLLI2S_VCO Input * PLLI2SN */
      /* SAI_CLK(first level) = PLLI2S_VCO Output/PLLI2SQ */
      __HAL_RCC_PLLI2S_CONFIG(PeriphClkInit->PLLI2S.PLLI2SM, PeriphClkInit->PLLI2S.PLLI2SN, plli2sq, PeriphClkInit->PLLI2S.PLLI2SR);  /* HAL辅助宏/函数 */

      /* SAI_CLK_x = SAI_CLK(first level)/PLLI2SDIVR */
      __HAL_RCC_PLLI2S_PLLSAICLKDIVR_CONFIG(PeriphClkInit->PLLI2SDivR);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
#endif /* STM32F413xx || STM32F423xx */

    /*----------------- In Case of PLLI2S is just selected  ------------------*/
    if((PeriphClkInit->PeriphClockSelection & RCC_PERIPHCLK_PLLI2S) == RCC_PERIPHCLK_PLLI2S)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Check for Parameters */
      assert_param(IS_RCC_PLLI2SR_VALUE(PeriphClkInit->PLLI2S.PLLI2SR));  /* 执行语句 */
      assert_param(IS_RCC_PLLI2SQ_VALUE(PeriphClkInit->PLLI2S.PLLI2SQ));  /* 执行语句 */

      /* Configure the PLLI2S division factors */
      /* PLLI2S_VCO = f(VCO clock) = f(PLLI2S clock input) * (PLLI2SN/PLLI2SM)*/
      /* SPDIFRXCLK = f(PLLI2S clock output) = f(VCO clock) / PLLI2SP */
      __HAL_RCC_PLLI2S_CONFIG(PeriphClkInit->PLLI2S.PLLI2SM, PeriphClkInit->PLLI2S.PLLI2SN , PeriphClkInit->PLLI2S.PLLI2SQ, PeriphClkInit->PLLI2S.PLLI2SR);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Enable the PLLI2S */
    __HAL_RCC_PLLI2S_ENABLE();                          /* HAL辅助宏/函数 */
    /* Get tick */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */
    /* Wait till PLLI2S is ready */
    while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLI2SRDY)  == RESET)  /* while循环 */
    {                                                   /* 代码块开始 */
      if((HAL_GetTick() - tickstart ) > PLLI2S_TIMEOUT_VALUE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* return in case of Timeout detected */
        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /*--------------------------------------------------------------------------*/

  /*-------------------- DFSDM1 clock source configuration -------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_DFSDM1) == RCC_PERIPHCLK_DFSDM1)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check the parameters */
    assert_param(IS_RCC_DFSDM1CLKSOURCE(PeriphClkInit->Dfsdm1ClockSelection));  /* 执行语句 */

    /* Configure the DFSDM1 interface clock source */
    __HAL_RCC_DFSDM1_CONFIG(PeriphClkInit->Dfsdm1ClockSelection);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  /*--------------------------------------------------------------------------*/

  /*-------------------- DFSDM1 Audio clock source configuration -------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_DFSDM1_AUDIO) == RCC_PERIPHCLK_DFSDM1_AUDIO)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check the parameters */
    assert_param(IS_RCC_DFSDM1AUDIOCLKSOURCE(PeriphClkInit->Dfsdm1AudioClockSelection));  /* 执行语句 */

    /* Configure the DFSDM1 Audio interface clock source */
    __HAL_RCC_DFSDM1AUDIO_CONFIG(PeriphClkInit->Dfsdm1AudioClockSelection);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  /*--------------------------------------------------------------------------*/

#if defined(STM32F413xx) || defined(STM32F423xx)        /* 条件编译 */
  /*-------------------- DFSDM2 clock source configuration -------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_DFSDM2) == RCC_PERIPHCLK_DFSDM2)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check the parameters */
    assert_param(IS_RCC_DFSDM2CLKSOURCE(PeriphClkInit->Dfsdm2ClockSelection));  /* 执行语句 */

    /* Configure the DFSDM1 interface clock source */
    __HAL_RCC_DFSDM2_CONFIG(PeriphClkInit->Dfsdm2ClockSelection);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  /*--------------------------------------------------------------------------*/

  /*-------------------- DFSDM2 Audio clock source configuration -------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_DFSDM2_AUDIO) == RCC_PERIPHCLK_DFSDM2_AUDIO)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check the parameters */
    assert_param(IS_RCC_DFSDM2AUDIOCLKSOURCE(PeriphClkInit->Dfsdm2AudioClockSelection));  /* 执行语句 */

    /* Configure the DFSDM1 Audio interface clock source */
    __HAL_RCC_DFSDM2AUDIO_CONFIG(PeriphClkInit->Dfsdm2AudioClockSelection);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  /*--------------------------------------------------------------------------*/

  /*---------------------------- LPTIM1 Configuration ------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_LPTIM1) == RCC_PERIPHCLK_LPTIM1)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check the parameters */
    assert_param(IS_RCC_LPTIM1CLKSOURCE(PeriphClkInit->Lptim1ClockSelection));  /* 执行语句 */

    /* Configure the LPTIM1 clock source */
    __HAL_RCC_LPTIM1_CONFIG(PeriphClkInit->Lptim1ClockSelection);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  /*--------------------------------------------------------------------------*/
#endif /* STM32F413xx || STM32F423xx */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Get the RCC_PeriphCLKInitTypeDef according to the internal
  *         RCC configuration registers.
  * @param  PeriphClkInit pointer to an RCC_PeriphCLKInitTypeDef structure that
  *         will be configured.
  * @retval None
  */
void HAL_RCCEx_GetPeriphCLKConfig(RCC_PeriphCLKInitTypeDef  *PeriphClkInit)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  uint32_t tempreg;                                     /* 32位无符号变量/函数 */

  /* Set all possible values for the extended clock type parameter------------*/
#if defined(STM32F413xx) || defined(STM32F423xx)        /* 条件编译 */
  PeriphClkInit->PeriphClockSelection = RCC_PERIPHCLK_I2S_APB1     | RCC_PERIPHCLK_I2S_APB2 |\
                                        RCC_PERIPHCLK_TIM          | RCC_PERIPHCLK_RTC      |\
                                        RCC_PERIPHCLK_FMPI2C1      | RCC_PERIPHCLK_CLK48    |\
                                        RCC_PERIPHCLK_SDIO         | RCC_PERIPHCLK_DFSDM1   |\
                                        RCC_PERIPHCLK_DFSDM1_AUDIO | RCC_PERIPHCLK_DFSDM2   |\
                                        RCC_PERIPHCLK_DFSDM2_AUDIO | RCC_PERIPHCLK_LPTIM1   |\
                                        RCC_PERIPHCLK_SAIA         | RCC_PERIPHCLK_SAIB;  /* 时钟相关操作 */
#else /* STM32F412Zx || STM32F412Vx || STM32F412Rx || STM32F412Cx */
  PeriphClkInit->PeriphClockSelection = RCC_PERIPHCLK_I2S_APB1 | RCC_PERIPHCLK_I2S_APB2 |\
                                        RCC_PERIPHCLK_TIM      | RCC_PERIPHCLK_RTC      |\
                                        RCC_PERIPHCLK_FMPI2C1  | RCC_PERIPHCLK_CLK48    |\
                                        RCC_PERIPHCLK_SDIO     | RCC_PERIPHCLK_DFSDM1   |\
                                        RCC_PERIPHCLK_DFSDM1_AUDIO;  /* 时钟相关操作 */
#endif /* STM32F413xx || STM32F423xx */



  /* Get the PLLI2S Clock configuration --------------------------------------*/
  PeriphClkInit->PLLI2S.PLLI2SM = (uint32_t)((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SM) >> RCC_PLLI2SCFGR_PLLI2SM_Pos);  /* 赋值或初始化 */
  PeriphClkInit->PLLI2S.PLLI2SN = (uint32_t)((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SN) >> RCC_PLLI2SCFGR_PLLI2SN_Pos);  /* 赋值或初始化 */
  PeriphClkInit->PLLI2S.PLLI2SQ = (uint32_t)((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SQ) >> RCC_PLLI2SCFGR_PLLI2SQ_Pos);  /* 赋值或初始化 */
  PeriphClkInit->PLLI2S.PLLI2SR = (uint32_t)((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SR) >> RCC_PLLI2SCFGR_PLLI2SR_Pos);  /* 赋值或初始化 */
#if defined(STM32F413xx) || defined(STM32F423xx)        /* 条件编译 */
  /* Get the PLL/PLLI2S division factors -------------------------------------*/
  PeriphClkInit->PLLI2SDivR = (uint32_t)((RCC->DCKCFGR & RCC_DCKCFGR_PLLI2SDIVR) >> RCC_DCKCFGR_PLLI2SDIVR_Pos);  /* 赋值或初始化 */
  PeriphClkInit->PLLDivR = (uint32_t)((RCC->DCKCFGR & RCC_DCKCFGR_PLLDIVR) >> RCC_DCKCFGR_PLLDIVR_Pos);  /* 赋值或初始化 */
#endif /* STM32F413xx || STM32F423xx */

  /* Get the I2S APB1 clock configuration ------------------------------------*/
  PeriphClkInit->I2sApb1ClockSelection = __HAL_RCC_GET_I2S_APB1_SOURCE();  /* 赋值或初始化 */

  /* Get the I2S APB2 clock configuration ------------------------------------*/
  PeriphClkInit->I2sApb2ClockSelection = __HAL_RCC_GET_I2S_APB2_SOURCE();  /* 赋值或初始化 */

  /* Get the RTC Clock configuration -----------------------------------------*/
  tempreg = (RCC->CFGR & RCC_CFGR_RTCPRE);              /* 赋值或初始化 */
  PeriphClkInit->RTCClockSelection = (uint32_t)((tempreg) | (RCC->BDCR & RCC_BDCR_RTCSEL));  /* 赋值或初始化 */

  /* Get the FMPI2C1 clock configuration -------------------------------------*/
  PeriphClkInit->Fmpi2c1ClockSelection = __HAL_RCC_GET_FMPI2C1_SOURCE();  /* 赋值或初始化 */

  /* Get the CLK48 clock configuration ---------------------------------------*/
  PeriphClkInit->Clk48ClockSelection = __HAL_RCC_GET_CLK48_SOURCE();  /* 赋值或初始化 */

  /* Get the SDIO clock configuration ----------------------------------------*/
  PeriphClkInit->SdioClockSelection = __HAL_RCC_GET_SDIO_SOURCE();  /* 赋值或初始化 */

  /* Get the DFSDM1 clock configuration --------------------------------------*/
  PeriphClkInit->Dfsdm1ClockSelection = __HAL_RCC_GET_DFSDM1_SOURCE();  /* 赋值或初始化 */

  /* Get the DFSDM1 Audio clock configuration --------------------------------*/
  PeriphClkInit->Dfsdm1AudioClockSelection = __HAL_RCC_GET_DFSDM1AUDIO_SOURCE();  /* 赋值或初始化 */

#if defined(STM32F413xx) || defined(STM32F423xx)        /* 条件编译 */
  /* Get the DFSDM2 clock configuration --------------------------------------*/
  PeriphClkInit->Dfsdm2ClockSelection = __HAL_RCC_GET_DFSDM2_SOURCE();  /* 赋值或初始化 */

  /* Get the DFSDM2 Audio clock configuration --------------------------------*/
  PeriphClkInit->Dfsdm2AudioClockSelection = __HAL_RCC_GET_DFSDM2AUDIO_SOURCE();  /* 赋值或初始化 */

  /* Get the LPTIM1 clock configuration --------------------------------------*/
  PeriphClkInit->Lptim1ClockSelection = __HAL_RCC_GET_LPTIM1_SOURCE();  /* 赋值或初始化 */

  /* Get the SAI1 Block Aclock configuration ---------------------------------*/
  PeriphClkInit->SaiAClockSelection = __HAL_RCC_GET_SAI_BLOCKA_SOURCE();  /* 赋值或初始化 */

  /* Get the SAI1 Block B clock configuration --------------------------------*/
  PeriphClkInit->SaiBClockSelection = __HAL_RCC_GET_SAI_BLOCKB_SOURCE();  /* 赋值或初始化 */
#endif /* STM32F413xx || STM32F423xx */

  /* Get the TIM Prescaler configuration -------------------------------------*/
  if ((RCC->DCKCFGR & RCC_DCKCFGR_TIMPRE) == RESET)     /* 条件判断 */
  {                                                     /* 代码块开始 */
    PeriphClkInit->TIMPresSelection = RCC_TIMPRES_DESACTIVATED;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    PeriphClkInit->TIMPresSelection = RCC_TIMPRES_ACTIVATED;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return the peripheral clock frequency for a given peripheral(I2S..)
  * @note   Return 0 if peripheral clock identifier not managed by this API
  * @param  PeriphClk Peripheral clock identifier
  *         This parameter can be one of the following values:
  *            @arg RCC_PERIPHCLK_I2S_APB1: I2S APB1 peripheral clock
  *            @arg RCC_PERIPHCLK_I2S_APB2: I2S APB2 peripheral clock
  * @retval Frequency in KHz
  */
uint32_t HAL_RCCEx_GetPeriphCLKFreq(uint32_t PeriphClk)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  /* This variable used to store the I2S clock frequency (value in Hz) */
  uint32_t frequency = 0U;                              /* 32位无符号变量/函数 */
  /* This variable used to store the VCO Input (value in Hz) */
  uint32_t vcoinput = 0U;                               /* 32位无符号变量/函数 */
  uint32_t srcclk = 0U;                                 /* 32位无符号变量/函数 */
  /* This variable used to store the VCO Output (value in Hz) */
  uint32_t vcooutput = 0U;                              /* 32位无符号变量/函数 */
  switch (PeriphClk)                                    /* switch分支 */
  {                                                     /* 代码块开始 */
  case RCC_PERIPHCLK_I2S_APB1:                          /* case标签 */
    {                                                   /* 代码块开始 */
      /* Get the current I2S source */
      srcclk = __HAL_RCC_GET_I2S_APB1_SOURCE();         /* 赋值或初始化 */
      switch (srcclk)                                   /* switch分支 */
      {                                                 /* 代码块开始 */
      /* Check if I2S clock selection is External clock mapped on the I2S_CKIN pin used as I2S clock */
      case RCC_I2SAPB1CLKSOURCE_EXT:                    /* case标签 */
        {                                               /* 代码块开始 */
          /* Set the I2S clock to the external clock  value */
          frequency = EXTERNAL_CLOCK_VALUE;             /* 赋值或初始化 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */
      /* Check if I2S clock selection is PLLI2S VCO output clock divided by PLLI2SR used as I2S clock */
      case RCC_I2SAPB1CLKSOURCE_PLLI2S:                 /* case标签 */
        {                                               /* 代码块开始 */
          if((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SSRC) == RCC_PLLI2SCFGR_PLLI2SSRC)  /* 条件判断 */
          {                                             /* 代码块开始 */
            /* Get the I2S source clock value */
            vcoinput = (uint32_t)(EXTERNAL_CLOCK_VALUE / (uint32_t)(RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SM));  /* 赋值或初始化 */
          }                                             /* 代码块结束 */
          else                                          /* 否则 */
          {                                             /* 代码块开始 */
            /* Configure the PLLI2S division factor */
            /* PLLI2S_VCO Input  = PLL_SOURCE/PLLI2SM */
            if((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLSOURCE_HSE)  /* 条件判断 */
            {                                           /* 代码块开始 */
              /* Get the I2S source clock value */
              vcoinput = (uint32_t)(HSE_VALUE / (uint32_t)(RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SM));  /* 赋值或初始化 */
            }                                           /* 代码块结束 */
            else                                        /* 否则 */
            {                                           /* 代码块开始 */
              /* Get the I2S source clock value */
              vcoinput = (uint32_t)(HSI_VALUE / (uint32_t)(RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SM));  /* 赋值或初始化 */
            }                                           /* 代码块结束 */
          }                                             /* 代码块结束 */
          /* PLLI2S_VCO Output = PLLI2S_VCO Input * PLLI2SN */
          vcooutput = (uint32_t)(vcoinput * (((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SN) >> 6U) & (RCC_PLLI2SCFGR_PLLI2SN >> 6U)));  /* 赋值或初始化 */
          /* I2S_CLK = PLLI2S_VCO Output/PLLI2SR */
          frequency = (uint32_t)(vcooutput /(((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SR) >> 28U) & (RCC_PLLI2SCFGR_PLLI2SR >> 28U)));  /* 赋值或初始化 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */
      /* Check if I2S clock selection is PLL VCO Output divided by PLLR used as I2S clock */
      case RCC_I2SAPB1CLKSOURCE_PLLR:                   /* case标签 */
        {                                               /* 代码块开始 */
          /* Configure the PLL division factor R */
          /* PLL_VCO Input  = PLL_SOURCE/PLLM */
          if((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLSOURCE_HSE)  /* 条件判断 */
          {                                             /* 代码块开始 */
            /* Get the I2S source clock value */
            vcoinput = (uint32_t)(HSE_VALUE / (uint32_t)(RCC->PLLCFGR & RCC_PLLCFGR_PLLM));  /* 赋值或初始化 */
          }                                             /* 代码块结束 */
          else                                          /* 否则 */
          {                                             /* 代码块开始 */
            /* Get the I2S source clock value */
            vcoinput = (uint32_t)(HSI_VALUE / (uint32_t)(RCC->PLLCFGR & RCC_PLLCFGR_PLLM));  /* 赋值或初始化 */
          }                                             /* 代码块结束 */

          /* PLL_VCO Output = PLL_VCO Input * PLLN */
          vcooutput = (uint32_t)(vcoinput * (((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6U) & (RCC_PLLCFGR_PLLN >> 6U)));  /* 赋值或初始化 */
          /* I2S_CLK = PLL_VCO Output/PLLR */
          frequency = (uint32_t)(vcooutput /(((RCC->PLLCFGR & RCC_PLLCFGR_PLLR) >> 28U) & (RCC_PLLCFGR_PLLR >> 28U)));  /* 赋值或初始化 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */
      /* Check if I2S clock selection is HSI or HSE depending from PLL source Clock */
      case RCC_I2SAPB1CLKSOURCE_PLLSRC:                 /* case标签 */
        {                                               /* 代码块开始 */
          if((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLSOURCE_HSE)  /* 条件判断 */
          {                                             /* 代码块开始 */
            frequency = HSE_VALUE;                      /* 赋值或初始化 */
          }                                             /* 代码块结束 */
          else                                          /* 否则 */
          {                                             /* 代码块开始 */
            frequency = HSI_VALUE;                      /* 赋值或初始化 */
          }                                             /* 代码块结束 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */
        /* Clock not enabled for I2S*/
      default:                                          /* default分支 */
        {                                               /* 代码块开始 */
          frequency = 0U;                               /* 赋值或初始化 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
  case RCC_PERIPHCLK_I2S_APB2:                          /* case标签 */
    {                                                   /* 代码块开始 */
      /* Get the current I2S source */
      srcclk = __HAL_RCC_GET_I2S_APB2_SOURCE();         /* 赋值或初始化 */
      switch (srcclk)                                   /* switch分支 */
      {                                                 /* 代码块开始 */
        /* Check if I2S clock selection is External clock mapped on the I2S_CKIN pin used as I2S clock */
      case RCC_I2SAPB2CLKSOURCE_EXT:                    /* case标签 */
        {                                               /* 代码块开始 */
          /* Set the I2S clock to the external clock  value */
          frequency = EXTERNAL_CLOCK_VALUE;             /* 赋值或初始化 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */
        /* Check if I2S clock selection is PLLI2S VCO output clock divided by PLLI2SR used as I2S clock */
      case RCC_I2SAPB2CLKSOURCE_PLLI2S:                 /* case标签 */
        {                                               /* 代码块开始 */
          if((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SSRC) == RCC_PLLI2SCFGR_PLLI2SSRC)  /* 条件判断 */
          {                                             /* 代码块开始 */
            /* Get the I2S source clock value */
            vcoinput = (uint32_t)(EXTERNAL_CLOCK_VALUE / (uint32_t)(RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SM));  /* 赋值或初始化 */
          }                                             /* 代码块结束 */
          else                                          /* 否则 */
          {                                             /* 代码块开始 */
            /* Configure the PLLI2S division factor */
            /* PLLI2S_VCO Input  = PLL_SOURCE/PLLI2SM */
            if((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLSOURCE_HSE)  /* 条件判断 */
            {                                           /* 代码块开始 */
              /* Get the I2S source clock value */
              vcoinput = (uint32_t)(HSE_VALUE / (uint32_t)(RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SM));  /* 赋值或初始化 */
            }                                           /* 代码块结束 */
            else                                        /* 否则 */
            {                                           /* 代码块开始 */
              /* Get the I2S source clock value */
              vcoinput = (uint32_t)(HSI_VALUE / (uint32_t)(RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SM));  /* 赋值或初始化 */
            }                                           /* 代码块结束 */
          }                                             /* 代码块结束 */
          /* PLLI2S_VCO Output = PLLI2S_VCO Input * PLLI2SN */
          vcooutput = (uint32_t)(vcoinput * (((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SN) >> 6U) & (RCC_PLLI2SCFGR_PLLI2SN >> 6U)));  /* 赋值或初始化 */
          /* I2S_CLK = PLLI2S_VCO Output/PLLI2SR */
          frequency = (uint32_t)(vcooutput /(((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SR) >> 28U) & (RCC_PLLI2SCFGR_PLLI2SR >> 28U)));  /* 赋值或初始化 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */
        /* Check if I2S clock selection is PLL VCO Output divided by PLLR used as I2S clock */
      case RCC_I2SAPB2CLKSOURCE_PLLR:                   /* case标签 */
        {                                               /* 代码块开始 */
          /* Configure the PLL division factor R */
          /* PLL_VCO Input  = PLL_SOURCE/PLLM */
          if((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLSOURCE_HSE)  /* 条件判断 */
          {                                             /* 代码块开始 */
            /* Get the I2S source clock value */
            vcoinput = (uint32_t)(HSE_VALUE / (uint32_t)(RCC->PLLCFGR & RCC_PLLCFGR_PLLM));  /* 赋值或初始化 */
          }                                             /* 代码块结束 */
          else                                          /* 否则 */
          {                                             /* 代码块开始 */
            /* Get the I2S source clock value */
            vcoinput = (uint32_t)(HSI_VALUE / (uint32_t)(RCC->PLLCFGR & RCC_PLLCFGR_PLLM));  /* 赋值或初始化 */
          }                                             /* 代码块结束 */

          /* PLL_VCO Output = PLL_VCO Input * PLLN */
          vcooutput = (uint32_t)(vcoinput * (((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6U) & (RCC_PLLCFGR_PLLN >> 6U)));  /* 赋值或初始化 */
          /* I2S_CLK = PLL_VCO Output/PLLR */
          frequency = (uint32_t)(vcooutput /(((RCC->PLLCFGR & RCC_PLLCFGR_PLLR) >> 28U) & (RCC_PLLCFGR_PLLR >> 28U)));  /* 赋值或初始化 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */
        /* Check if I2S clock selection is HSI or HSE depending from PLL source Clock */
      case RCC_I2SAPB2CLKSOURCE_PLLSRC:                 /* case标签 */
        {                                               /* 代码块开始 */
          if((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLSOURCE_HSE)  /* 条件判断 */
          {                                             /* 代码块开始 */
            frequency = HSE_VALUE;                      /* 赋值或初始化 */
          }                                             /* 代码块结束 */
          else                                          /* 否则 */
          {                                             /* 代码块开始 */
            frequency = HSI_VALUE;                      /* 赋值或初始化 */
          }                                             /* 代码块结束 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */
      /* Clock not enabled for I2S*/
      default:                                          /* default分支 */
        {                                               /* 代码块开始 */
          frequency = 0U;                               /* 赋值或初始化 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  return frequency;                                     /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* STM32F412Zx || STM32F412Vx || STM32F412Rx || STM32F412Cx || STM32F413xx || STM32F423xx */

#if defined(STM32F410Tx) || defined(STM32F410Cx) || defined(STM32F410Rx)  /* 条件编译 */
/**
  * @brief  Initializes the RCC extended peripherals clocks according to the specified parameters in the
  *         RCC_PeriphCLKInitTypeDef.
  * @param  PeriphClkInit pointer to an RCC_PeriphCLKInitTypeDef structure that
  *         contains the configuration information for the Extended Peripherals clocks(I2S and RTC clocks).
  *
  * @note   A caution to be taken when HAL_RCCEx_PeriphCLKConfig() is used to select RTC clock selection, in this case
  *         the Reset of Backup domain will be applied in order to modify the RTC Clock source as consequence all backup
  *        domain (RTC and RCC_BDCR register expect BKPSRAM) will be reset
  *
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCCEx_PeriphCLKConfig(RCC_PeriphCLKInitTypeDef  *PeriphClkInit)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart = 0U;                              /* 32位无符号变量/函数 */
  uint32_t tmpreg1 = 0U;                                /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_RCC_PERIPHCLOCK(PeriphClkInit->PeriphClockSelection));  /* 执行语句 */

  /*---------------------------- RTC configuration ---------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_RTC) == (RCC_PERIPHCLK_RTC))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check for RTC Parameters used to output RTCCLK */
    assert_param(IS_RCC_RTCCLKSOURCE(PeriphClkInit->RTCClockSelection));  /* 执行语句 */

    /* Enable Power Clock*/
    __HAL_RCC_PWR_CLK_ENABLE();                         /* HAL辅助宏/函数 */

    /* Enable write access to Backup domain */
    PWR->CR |= PWR_CR_DBP;                              /* 赋值或初始化 */

    /* Get tick */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */

    while((PWR->CR & PWR_CR_DBP) == RESET)              /* while循环 */
    {                                                   /* 代码块开始 */
      if((HAL_GetTick() - tickstart ) > RCC_DBP_TIMEOUT_VALUE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    /* Reset the Backup domain only if the RTC Clock source selection is modified from reset value */
    tmpreg1 = (RCC->BDCR & RCC_BDCR_RTCSEL);            /* 赋值或初始化 */
    if((tmpreg1 != 0x00000000U) && ((tmpreg1) != (PeriphClkInit->RTCClockSelection & RCC_BDCR_RTCSEL)))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Store the content of BDCR register before the reset of Backup Domain */
      tmpreg1 = (RCC->BDCR & ~(RCC_BDCR_RTCSEL));       /* 赋值或初始化 */
      /* RTC Clock selection can be changed only if the Backup Domain is reset */
      __HAL_RCC_BACKUPRESET_FORCE();                    /* HAL辅助宏/函数 */
      __HAL_RCC_BACKUPRESET_RELEASE();                  /* HAL辅助宏/函数 */
      /* Restore the Content of BDCR register */
      RCC->BDCR = tmpreg1;                              /* 赋值或初始化 */

      /* Wait for LSE reactivation if LSE was enable prior to Backup Domain reset */
      if(HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSEON))     /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Get tick */
        tickstart = HAL_GetTick();                      /* 赋值或初始化 */

        /* Wait till LSE is ready */
        while(__HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY) == RESET)  /* while循环 */
        {                                               /* 代码块开始 */
          if((HAL_GetTick() - tickstart ) > RCC_LSE_TIMEOUT_VALUE)  /* 条件判断 */
          {                                             /* 代码块开始 */
            return HAL_TIMEOUT;                         /* 返回值 */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    __HAL_RCC_RTC_CONFIG(PeriphClkInit->RTCClockSelection);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  /*--------------------------------------------------------------------------*/

  /*---------------------------- TIM configuration ---------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_TIM) == (RCC_PERIPHCLK_TIM))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    __HAL_RCC_TIMCLKPRESCALER(PeriphClkInit->TIMPresSelection);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  /*--------------------------------------------------------------------------*/

  /*---------------------------- FMPI2C1 Configuration -----------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_FMPI2C1) == RCC_PERIPHCLK_FMPI2C1)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check the parameters */
    assert_param(IS_RCC_FMPI2C1CLKSOURCE(PeriphClkInit->Fmpi2c1ClockSelection));  /* 执行语句 */

    /* Configure the FMPI2C1 clock source */
    __HAL_RCC_FMPI2C1_CONFIG(PeriphClkInit->Fmpi2c1ClockSelection);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  /*--------------------------------------------------------------------------*/

  /*---------------------------- LPTIM1 Configuration ------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_LPTIM1) == RCC_PERIPHCLK_LPTIM1)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check the parameters */
    assert_param(IS_RCC_LPTIM1CLKSOURCE(PeriphClkInit->Lptim1ClockSelection));  /* 执行语句 */

    /* Configure the LPTIM1 clock source */
    __HAL_RCC_LPTIM1_CONFIG(PeriphClkInit->Lptim1ClockSelection);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /*---------------------------- I2S Configuration ---------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_I2S) == RCC_PERIPHCLK_I2S)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check the parameters */
    assert_param(IS_RCC_I2SAPBCLKSOURCE(PeriphClkInit->I2SClockSelection));  /* 执行语句 */

    /* Configure the I2S clock source */
    __HAL_RCC_I2S_CONFIG(PeriphClkInit->I2SClockSelection);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configures the RCC_OscInitStruct according to the internal
  * RCC configuration registers.
  * @param  PeriphClkInit pointer to an RCC_PeriphCLKInitTypeDef structure that
  * will be configured.
  * @retval None
  */
void HAL_RCCEx_GetPeriphCLKConfig(RCC_PeriphCLKInitTypeDef  *PeriphClkInit)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  uint32_t tempreg;                                     /* 32位无符号变量/函数 */

  /* Set all possible values for the extended clock type parameter------------*/
  PeriphClkInit->PeriphClockSelection = RCC_PERIPHCLK_FMPI2C1 | RCC_PERIPHCLK_LPTIM1 | RCC_PERIPHCLK_TIM | RCC_PERIPHCLK_RTC;  /* 赋值或初始化 */

  tempreg = (RCC->CFGR & RCC_CFGR_RTCPRE);              /* 赋值或初始化 */
  PeriphClkInit->RTCClockSelection = (uint32_t)((tempreg) | (RCC->BDCR & RCC_BDCR_RTCSEL));  /* 赋值或初始化 */

  if ((RCC->DCKCFGR & RCC_DCKCFGR_TIMPRE) == RESET)     /* 条件判断 */
  {                                                     /* 代码块开始 */
    PeriphClkInit->TIMPresSelection = RCC_TIMPRES_DESACTIVATED;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    PeriphClkInit->TIMPresSelection = RCC_TIMPRES_ACTIVATED;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  /* Get the FMPI2C1 clock configuration -------------------------------------*/
  PeriphClkInit->Fmpi2c1ClockSelection = __HAL_RCC_GET_FMPI2C1_SOURCE();  /* 赋值或初始化 */

  /* Get the I2S clock configuration -----------------------------------------*/
  PeriphClkInit->I2SClockSelection = __HAL_RCC_GET_I2S_SOURCE();  /* 赋值或初始化 */


}                                                       /* 代码块结束 */
/**
  * @brief  Return the peripheral clock frequency for a given peripheral(SAI..)
  * @note   Return 0 if peripheral clock identifier not managed by this API
  * @param  PeriphClk Peripheral clock identifier
  *         This parameter can be one of the following values:
  *            @arg RCC_PERIPHCLK_I2S: I2S peripheral clock
  * @retval Frequency in KHz
  */
uint32_t HAL_RCCEx_GetPeriphCLKFreq(uint32_t PeriphClk)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  /* This variable used to store the I2S clock frequency (value in Hz) */
  uint32_t frequency = 0U;                              /* 32位无符号变量/函数 */
  /* This variable used to store the VCO Input (value in Hz) */
  uint32_t vcoinput = 0U;                               /* 32位无符号变量/函数 */
  uint32_t srcclk = 0U;                                 /* 32位无符号变量/函数 */
  /* This variable used to store the VCO Output (value in Hz) */
  uint32_t vcooutput = 0U;                              /* 32位无符号变量/函数 */
  switch (PeriphClk)                                    /* switch分支 */
  {                                                     /* 代码块开始 */
  case RCC_PERIPHCLK_I2S:                               /* case标签 */
    {                                                   /* 代码块开始 */
      /* Get the current I2S source */
      srcclk = __HAL_RCC_GET_I2S_SOURCE();              /* 赋值或初始化 */
      switch (srcclk)                                   /* switch分支 */
      {                                                 /* 代码块开始 */
      /* Check if I2S clock selection is External clock mapped on the I2S_CKIN pin used as I2S clock */
      case RCC_I2SAPBCLKSOURCE_EXT:                     /* case标签 */
        {                                               /* 代码块开始 */
          /* Set the I2S clock to the external clock  value */
          frequency = EXTERNAL_CLOCK_VALUE;             /* 赋值或初始化 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */
      /* Check if I2S clock selection is PLL VCO Output divided by PLLR used as I2S clock */
      case RCC_I2SAPBCLKSOURCE_PLLR:                    /* case标签 */
        {                                               /* 代码块开始 */
          /* Configure the PLL division factor R */
          /* PLL_VCO Input  = PLL_SOURCE/PLLM */
          if((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLSOURCE_HSE)  /* 条件判断 */
          {                                             /* 代码块开始 */
            /* Get the I2S source clock value */
            vcoinput = (uint32_t)(HSE_VALUE / (uint32_t)(RCC->PLLCFGR & RCC_PLLCFGR_PLLM));  /* 赋值或初始化 */
          }                                             /* 代码块结束 */
          else                                          /* 否则 */
          {                                             /* 代码块开始 */
            /* Get the I2S source clock value */
            vcoinput = (uint32_t)(HSI_VALUE / (uint32_t)(RCC->PLLCFGR & RCC_PLLCFGR_PLLM));  /* 赋值或初始化 */
          }                                             /* 代码块结束 */

          /* PLL_VCO Output = PLL_VCO Input * PLLN */
          vcooutput = (uint32_t)(vcoinput * (((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6U) & (RCC_PLLCFGR_PLLN >> 6U)));  /* 赋值或初始化 */
          /* I2S_CLK = PLL_VCO Output/PLLR */
          frequency = (uint32_t)(vcooutput /(((RCC->PLLCFGR & RCC_PLLCFGR_PLLR) >> 28U) & (RCC_PLLCFGR_PLLR >> 28U)));  /* 赋值或初始化 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */
      /* Check if I2S clock selection is HSI or HSE depending from PLL source Clock */
      case RCC_I2SAPBCLKSOURCE_PLLSRC:                  /* case标签 */
        {                                               /* 代码块开始 */
          if((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLSOURCE_HSE)  /* 条件判断 */
          {                                             /* 代码块开始 */
            frequency = HSE_VALUE;                      /* 赋值或初始化 */
          }                                             /* 代码块结束 */
          else                                          /* 否则 */
          {                                             /* 代码块开始 */
            frequency = HSI_VALUE;                      /* 赋值或初始化 */
          }                                             /* 代码块结束 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */
        /* Clock not enabled for I2S*/
      default:                                          /* default分支 */
        {                                               /* 代码块开始 */
          frequency = 0U;                               /* 赋值或初始化 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  return frequency;                                     /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* STM32F410Tx || STM32F410Cx || STM32F410Rx */

#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx)  /* 条件编译 */
/**
  * @brief  Initializes the RCC extended peripherals clocks according to the specified
  *         parameters in the RCC_PeriphCLKInitTypeDef.
  * @param  PeriphClkInit pointer to an RCC_PeriphCLKInitTypeDef structure that
  *         contains the configuration information for the Extended Peripherals
  *         clocks(I2S, SAI, LTDC RTC and TIM).
  *
  * @note   Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select
  *         the RTC clock source; in this case the Backup domain will be reset in
  *         order to modify the RTC Clock source, as consequence RTC registers (including
  *         the backup registers) and RCC_BDCR register are set to their reset values.
  *
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCCEx_PeriphCLKConfig(RCC_PeriphCLKInitTypeDef  *PeriphClkInit)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart = 0U;                              /* 32位无符号变量/函数 */
  uint32_t tmpreg1 = 0U;                                /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_RCC_PERIPHCLOCK(PeriphClkInit->PeriphClockSelection));  /* 执行语句 */

  /*----------------------- SAI/I2S Configuration (PLLI2S) -------------------*/
  /*----------------------- Common configuration SAI/I2S ---------------------*/
  /* In Case of SAI or I2S Clock Configuration through PLLI2S, PLLI2SN division
     factor is common parameters for both peripherals */
  if((((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_I2S) == RCC_PERIPHCLK_I2S) ||  /* 条件判断 */
     (((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SAI_PLLI2S) == RCC_PERIPHCLK_SAI_PLLI2S) ||  /* 源代码 */
     (((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_PLLI2S) == RCC_PERIPHCLK_PLLI2S))  /* 源代码 */
  {                                                     /* 代码块开始 */
    /* check for Parameters */
    assert_param(IS_RCC_PLLI2SN_VALUE(PeriphClkInit->PLLI2S.PLLI2SN));  /* 执行语句 */

    /* Disable the PLLI2S */
    __HAL_RCC_PLLI2S_DISABLE();                         /* HAL辅助宏/函数 */
    /* Get tick */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */
    /* Wait till PLLI2S is disabled */
    while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLI2SRDY)  != RESET)  /* while循环 */
    {                                                   /* 代码块开始 */
      if((HAL_GetTick() - tickstart ) > PLLI2S_TIMEOUT_VALUE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* return in case of Timeout detected */
        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /*---------------------------- I2S configuration -------------------------*/
    /* In Case of I2S Clock Configuration through PLLI2S, PLLI2SR must be added
      only for I2S configuration */
    if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_I2S) == (RCC_PERIPHCLK_I2S))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* check for Parameters */
      assert_param(IS_RCC_PLLI2SR_VALUE(PeriphClkInit->PLLI2S.PLLI2SR));  /* 执行语句 */
      /* Configure the PLLI2S division factors */
      /* PLLI2S_VCO = f(VCO clock) = f(PLLI2S clock input) * (PLLI2SN/PLLM) */
      /* I2SCLK = f(PLLI2S clock output) = f(VCO clock) / PLLI2SR */
      __HAL_RCC_PLLI2S_CONFIG(PeriphClkInit->PLLI2S.PLLI2SN , PeriphClkInit->PLLI2S.PLLI2SR);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /*---------------------------- SAI configuration -------------------------*/
    /* In Case of SAI Clock Configuration through PLLI2S, PLLI2SQ and PLLI2S_DIVQ must
       be added only for SAI configuration */
    if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SAI_PLLI2S) == (RCC_PERIPHCLK_SAI_PLLI2S))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Check the PLLI2S division factors */
      assert_param(IS_RCC_PLLI2SQ_VALUE(PeriphClkInit->PLLI2S.PLLI2SQ));  /* 执行语句 */
      assert_param(IS_RCC_PLLI2S_DIVQ_VALUE(PeriphClkInit->PLLI2SDivQ));  /* 执行语句 */

      /* Read PLLI2SR value from PLLI2SCFGR register (this value is not need for SAI configuration) */
      tmpreg1 = ((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SR) >> RCC_PLLI2SCFGR_PLLI2SR_Pos);  /* 赋值或初始化 */
      /* Configure the PLLI2S division factors */
      /* PLLI2S_VCO Input  = PLL_SOURCE/PLLM */
      /* PLLI2S_VCO Output = PLLI2S_VCO Input * PLLI2SN */
      /* SAI_CLK(first level) = PLLI2S_VCO Output/PLLI2SQ */
      __HAL_RCC_PLLI2S_SAICLK_CONFIG(PeriphClkInit->PLLI2S.PLLI2SN , PeriphClkInit->PLLI2S.PLLI2SQ , tmpreg1);  /* HAL辅助宏/函数 */
      /* SAI_CLK_x = SAI_CLK(first level)/PLLI2SDIVQ */
      __HAL_RCC_PLLI2S_PLLSAICLKDIVQ_CONFIG(PeriphClkInit->PLLI2SDivQ);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /*----------------- In Case of PLLI2S is just selected  -----------------*/
    if((PeriphClkInit->PeriphClockSelection & RCC_PERIPHCLK_PLLI2S) == RCC_PERIPHCLK_PLLI2S)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Check for Parameters */
      assert_param(IS_RCC_PLLI2SQ_VALUE(PeriphClkInit->PLLI2S.PLLI2SQ));  /* 执行语句 */
      assert_param(IS_RCC_PLLI2SR_VALUE(PeriphClkInit->PLLI2S.PLLI2SR));  /* 执行语句 */

      /* Configure the PLLI2S multiplication and division factors */
      __HAL_RCC_PLLI2S_SAICLK_CONFIG(PeriphClkInit->PLLI2S.PLLI2SN, PeriphClkInit->PLLI2S.PLLI2SQ, PeriphClkInit->PLLI2S.PLLI2SR);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Enable the PLLI2S */
    __HAL_RCC_PLLI2S_ENABLE();                          /* HAL辅助宏/函数 */
    /* Get tick */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */
    /* Wait till PLLI2S is ready */
    while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLI2SRDY)  == RESET)  /* while循环 */
    {                                                   /* 代码块开始 */
      if((HAL_GetTick() - tickstart ) > PLLI2S_TIMEOUT_VALUE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* return in case of Timeout detected */
        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /*--------------------------------------------------------------------------*/

  /*----------------------- SAI/LTDC Configuration (PLLSAI) ------------------*/
  /*----------------------- Common configuration SAI/LTDC --------------------*/
  /* In Case of SAI or LTDC Clock Configuration through PLLSAI, PLLSAIN division
     factor is common parameters for both peripherals */
  if((((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SAI_PLLSAI) == RCC_PERIPHCLK_SAI_PLLSAI) ||  /* 条件判断 */
     (((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_LTDC) == RCC_PERIPHCLK_LTDC))  /* 源代码 */
  {                                                     /* 代码块开始 */
    /* Check the PLLSAI division factors */
    assert_param(IS_RCC_PLLSAIN_VALUE(PeriphClkInit->PLLSAI.PLLSAIN));  /* 执行语句 */

    /* Disable PLLSAI Clock */
    __HAL_RCC_PLLSAI_DISABLE();                         /* HAL辅助宏/函数 */
    /* Get tick */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */
    /* Wait till PLLSAI is disabled */
    while(__HAL_RCC_PLLSAI_GET_FLAG() != RESET)         /* while循环 */
    {                                                   /* 代码块开始 */
      if((HAL_GetTick() - tickstart ) > PLLSAI_TIMEOUT_VALUE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* return in case of Timeout detected */
        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /*---------------------------- SAI configuration -------------------------*/
    /* In Case of SAI Clock Configuration through PLLSAI, PLLSAIQ and PLLSAI_DIVQ must
       be added only for SAI configuration */
    if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SAI_PLLSAI) == (RCC_PERIPHCLK_SAI_PLLSAI))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      assert_param(IS_RCC_PLLSAIQ_VALUE(PeriphClkInit->PLLSAI.PLLSAIQ));  /* 执行语句 */
      assert_param(IS_RCC_PLLSAI_DIVQ_VALUE(PeriphClkInit->PLLSAIDivQ));  /* 执行语句 */

      /* Read PLLSAIR value from PLLSAICFGR register (this value is not need for SAI configuration) */
      tmpreg1 = ((RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIR) >> RCC_PLLSAICFGR_PLLSAIR_Pos);  /* 赋值或初始化 */
      /* PLLSAI_VCO Input  = PLL_SOURCE/PLLM */
      /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN */
      /* SAI_CLK(first level) = PLLSAI_VCO Output/PLLSAIQ */
      __HAL_RCC_PLLSAI_CONFIG(PeriphClkInit->PLLSAI.PLLSAIN , PeriphClkInit->PLLSAI.PLLSAIQ, tmpreg1);  /* HAL辅助宏/函数 */
      /* SAI_CLK_x = SAI_CLK(first level)/PLLSAIDIVQ */
      __HAL_RCC_PLLSAI_PLLSAICLKDIVQ_CONFIG(PeriphClkInit->PLLSAIDivQ);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /*---------------------------- LTDC configuration ------------------------*/
    if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_LTDC) == (RCC_PERIPHCLK_LTDC))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      assert_param(IS_RCC_PLLSAIR_VALUE(PeriphClkInit->PLLSAI.PLLSAIR));  /* 执行语句 */
      assert_param(IS_RCC_PLLSAI_DIVR_VALUE(PeriphClkInit->PLLSAIDivR));  /* 执行语句 */

      /* Read PLLSAIR value from PLLSAICFGR register (this value is not need for SAI configuration) */
      tmpreg1 = ((RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIQ) >> RCC_PLLSAICFGR_PLLSAIQ_Pos);  /* 赋值或初始化 */
      /* PLLSAI_VCO Input  = PLL_SOURCE/PLLM */
      /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN */
      /* LTDC_CLK(first level) = PLLSAI_VCO Output/PLLSAIR */
      __HAL_RCC_PLLSAI_CONFIG(PeriphClkInit->PLLSAI.PLLSAIN , tmpreg1, PeriphClkInit->PLLSAI.PLLSAIR);  /* HAL辅助宏/函数 */
      /* LTDC_CLK = LTDC_CLK(first level)/PLLSAIDIVR */
      __HAL_RCC_PLLSAI_PLLSAICLKDIVR_CONFIG(PeriphClkInit->PLLSAIDivR);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
    /* Enable PLLSAI Clock */
    __HAL_RCC_PLLSAI_ENABLE();                          /* HAL辅助宏/函数 */
    /* Get tick */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */
    /* Wait till PLLSAI is ready */
    while(__HAL_RCC_PLLSAI_GET_FLAG() == RESET)         /* while循环 */
    {                                                   /* 代码块开始 */
      if((HAL_GetTick() - tickstart ) > PLLSAI_TIMEOUT_VALUE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* return in case of Timeout detected */
        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /*--------------------------------------------------------------------------*/

  /*---------------------------- RTC configuration ---------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_RTC) == (RCC_PERIPHCLK_RTC))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check for RTC Parameters used to output RTCCLK */
    assert_param(IS_RCC_RTCCLKSOURCE(PeriphClkInit->RTCClockSelection));  /* 执行语句 */

    /* Enable Power Clock*/
    __HAL_RCC_PWR_CLK_ENABLE();                         /* HAL辅助宏/函数 */

    /* Enable write access to Backup domain */
    PWR->CR |= PWR_CR_DBP;                              /* 赋值或初始化 */

    /* Get tick */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */

    while((PWR->CR & PWR_CR_DBP) == RESET)              /* while循环 */
    {                                                   /* 代码块开始 */
      if((HAL_GetTick() - tickstart ) > RCC_DBP_TIMEOUT_VALUE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    /* Reset the Backup domain only if the RTC Clock source selection is modified from reset value */
    tmpreg1 = (RCC->BDCR & RCC_BDCR_RTCSEL);            /* 赋值或初始化 */
    if((tmpreg1 != 0x00000000U) && ((tmpreg1) != (PeriphClkInit->RTCClockSelection & RCC_BDCR_RTCSEL)))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Store the content of BDCR register before the reset of Backup Domain */
      tmpreg1 = (RCC->BDCR & ~(RCC_BDCR_RTCSEL));       /* 赋值或初始化 */
      /* RTC Clock selection can be changed only if the Backup Domain is reset */
      __HAL_RCC_BACKUPRESET_FORCE();                    /* HAL辅助宏/函数 */
      __HAL_RCC_BACKUPRESET_RELEASE();                  /* HAL辅助宏/函数 */
      /* Restore the Content of BDCR register */
      RCC->BDCR = tmpreg1;                              /* 赋值或初始化 */

      /* Wait for LSE reactivation if LSE was enable prior to Backup Domain reset */
      if(HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSEON))     /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Get tick */
        tickstart = HAL_GetTick();                      /* 赋值或初始化 */

        /* Wait till LSE is ready */
        while(__HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY) == RESET)  /* while循环 */
        {                                               /* 代码块开始 */
          if((HAL_GetTick() - tickstart ) > RCC_LSE_TIMEOUT_VALUE)  /* 条件判断 */
          {                                             /* 代码块开始 */
            return HAL_TIMEOUT;                         /* 返回值 */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    __HAL_RCC_RTC_CONFIG(PeriphClkInit->RTCClockSelection);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  /*--------------------------------------------------------------------------*/

  /*---------------------------- TIM configuration ---------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_TIM) == (RCC_PERIPHCLK_TIM))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    __HAL_RCC_TIMCLKPRESCALER(PeriphClkInit->TIMPresSelection);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configures the PeriphClkInit according to the internal
  * RCC configuration registers.
  * @param  PeriphClkInit pointer to an RCC_PeriphCLKInitTypeDef structure that
  *         will be configured.
  * @retval None
  */
void HAL_RCCEx_GetPeriphCLKConfig(RCC_PeriphCLKInitTypeDef  *PeriphClkInit)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  uint32_t tempreg;                                     /* 32位无符号变量/函数 */

  /* Set all possible values for the extended clock type parameter------------*/
  PeriphClkInit->PeriphClockSelection = RCC_PERIPHCLK_I2S | RCC_PERIPHCLK_SAI_PLLSAI | RCC_PERIPHCLK_SAI_PLLI2S | RCC_PERIPHCLK_LTDC | RCC_PERIPHCLK_TIM | RCC_PERIPHCLK_RTC;  /* 赋值或初始化 */

  /* Get the PLLI2S Clock configuration -----------------------------------------------*/
  PeriphClkInit->PLLI2S.PLLI2SN = (uint32_t)((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SN) >> RCC_PLLI2SCFGR_PLLI2SN_Pos);  /* 赋值或初始化 */
  PeriphClkInit->PLLI2S.PLLI2SR = (uint32_t)((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SR) >> RCC_PLLI2SCFGR_PLLI2SR_Pos);  /* 赋值或初始化 */
  PeriphClkInit->PLLI2S.PLLI2SQ = (uint32_t)((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SQ) >> RCC_PLLI2SCFGR_PLLI2SQ_Pos);  /* 赋值或初始化 */
  /* Get the PLLSAI Clock configuration -----------------------------------------------*/
  PeriphClkInit->PLLSAI.PLLSAIN = (uint32_t)((RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIN) >> RCC_PLLSAICFGR_PLLSAIN_Pos);  /* 赋值或初始化 */
  PeriphClkInit->PLLSAI.PLLSAIR = (uint32_t)((RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIR) >> RCC_PLLSAICFGR_PLLSAIR_Pos);  /* 赋值或初始化 */
  PeriphClkInit->PLLSAI.PLLSAIQ = (uint32_t)((RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIQ) >> RCC_PLLSAICFGR_PLLSAIQ_Pos);  /* 赋值或初始化 */
  /* Get the PLLSAI/PLLI2S division factors -----------------------------------------------*/
  PeriphClkInit->PLLI2SDivQ = (uint32_t)((RCC->DCKCFGR & RCC_DCKCFGR_PLLI2SDIVQ) >> RCC_DCKCFGR_PLLI2SDIVQ_Pos);  /* 赋值或初始化 */
  PeriphClkInit->PLLSAIDivQ = (uint32_t)((RCC->DCKCFGR & RCC_DCKCFGR_PLLSAIDIVQ) >> RCC_DCKCFGR_PLLSAIDIVQ_Pos);  /* 赋值或初始化 */
  PeriphClkInit->PLLSAIDivR = (uint32_t)(RCC->DCKCFGR & RCC_DCKCFGR_PLLSAIDIVR);  /* 赋值或初始化 */
  /* Get the RTC Clock configuration -----------------------------------------------*/
  tempreg = (RCC->CFGR & RCC_CFGR_RTCPRE);              /* 赋值或初始化 */
  PeriphClkInit->RTCClockSelection = (uint32_t)((tempreg) | (RCC->BDCR & RCC_BDCR_RTCSEL));  /* 赋值或初始化 */

  if ((RCC->DCKCFGR & RCC_DCKCFGR_TIMPRE) == RESET)     /* 条件判断 */
  {                                                     /* 代码块开始 */
    PeriphClkInit->TIMPresSelection = RCC_TIMPRES_DESACTIVATED;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    PeriphClkInit->TIMPresSelection = RCC_TIMPRES_ACTIVATED;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return the peripheral clock frequency for a given peripheral(SAI..)
  * @note   Return 0 if peripheral clock identifier not managed by this API
  * @param  PeriphClk Peripheral clock identifier
  *         This parameter can be one of the following values:
  *            @arg RCC_PERIPHCLK_I2S: I2S peripheral clock
  * @retval Frequency in KHz
  */
uint32_t HAL_RCCEx_GetPeriphCLKFreq(uint32_t PeriphClk)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  /* This variable used to store the I2S clock frequency (value in Hz) */
  uint32_t frequency = 0U;                              /* 32位无符号变量/函数 */
  /* This variable used to store the VCO Input (value in Hz) */
  uint32_t vcoinput = 0U;                               /* 32位无符号变量/函数 */
  uint32_t srcclk = 0U;                                 /* 32位无符号变量/函数 */
  /* This variable used to store the VCO Output (value in Hz) */
  uint32_t vcooutput = 0U;                              /* 32位无符号变量/函数 */
  switch (PeriphClk)                                    /* switch分支 */
  {                                                     /* 代码块开始 */
  case RCC_PERIPHCLK_I2S:                               /* case标签 */
    {                                                   /* 代码块开始 */
      /* Get the current I2S source */
      srcclk = __HAL_RCC_GET_I2S_SOURCE();              /* 赋值或初始化 */
      switch (srcclk)                                   /* switch分支 */
      {                                                 /* 代码块开始 */
      /* Check if I2S clock selection is External clock mapped on the I2S_CKIN pin used as I2S clock */
      case RCC_I2SCLKSOURCE_EXT:                        /* case标签 */
        {                                               /* 代码块开始 */
          /* Set the I2S clock to the external clock  value */
          frequency = EXTERNAL_CLOCK_VALUE;             /* 赋值或初始化 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */
      /* Check if I2S clock selection is PLLI2S VCO output clock divided by PLLI2SR used as I2S clock */
      case RCC_I2SCLKSOURCE_PLLI2S:                     /* case标签 */
        {                                               /* 代码块开始 */
          /* Configure the PLLI2S division factor */
          /* PLLI2S_VCO Input  = PLL_SOURCE/PLLM */
          if((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLSOURCE_HSE)  /* 条件判断 */
          {                                             /* 代码块开始 */
            /* Get the I2S source clock value */
            vcoinput = (uint32_t)(HSE_VALUE / (uint32_t)(RCC->PLLCFGR & RCC_PLLCFGR_PLLM));  /* 赋值或初始化 */
          }                                             /* 代码块结束 */
          else                                          /* 否则 */
          {                                             /* 代码块开始 */
            /* Get the I2S source clock value */
            vcoinput = (uint32_t)(HSI_VALUE / (uint32_t)(RCC->PLLCFGR & RCC_PLLCFGR_PLLM));  /* 赋值或初始化 */
          }                                             /* 代码块结束 */

          /* PLLI2S_VCO Output = PLLI2S_VCO Input * PLLI2SN */
          vcooutput = (uint32_t)(vcoinput * (((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SN) >> 6U) & (RCC_PLLI2SCFGR_PLLI2SN >> 6U)));  /* 赋值或初始化 */
          /* I2S_CLK = PLLI2S_VCO Output/PLLI2SR */
          frequency = (uint32_t)(vcooutput /(((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SR) >> 28U) & (RCC_PLLI2SCFGR_PLLI2SR >> 28U)));  /* 赋值或初始化 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */
        /* Clock not enabled for I2S*/
      default:                                          /* default分支 */
        {                                               /* 代码块开始 */
          frequency = 0U;                               /* 赋值或初始化 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  return frequency;                                     /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx */

#if defined(STM32F405xx) || defined(STM32F415xx) || defined(STM32F407xx)|| defined(STM32F417xx) ||\
    defined(STM32F401xC) || defined(STM32F401xE) || defined(STM32F411xE)  /* 源代码 */
/**
  * @brief  Initializes the RCC extended peripherals clocks according to the specified parameters in the
  *         RCC_PeriphCLKInitTypeDef.
  * @param  PeriphClkInit pointer to an RCC_PeriphCLKInitTypeDef structure that
  *         contains the configuration information for the Extended Peripherals clocks(I2S and RTC clocks).
  *
  * @note   A caution to be taken when HAL_RCCEx_PeriphCLKConfig() is used to select RTC clock selection, in this case
  *         the Reset of Backup domain will be applied in order to modify the RTC Clock source as consequence all backup
  *        domain (RTC and RCC_BDCR register expect BKPSRAM) will be reset
  *
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCCEx_PeriphCLKConfig(RCC_PeriphCLKInitTypeDef  *PeriphClkInit)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart = 0U;                              /* 32位无符号变量/函数 */
  uint32_t tmpreg1 = 0U;                                /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_RCC_PERIPHCLOCK(PeriphClkInit->PeriphClockSelection));  /* 执行语句 */

  /*---------------------------- I2S configuration ---------------------------*/
  if((((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_I2S) == RCC_PERIPHCLK_I2S) ||  /* 条件判断 */
     (((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_PLLI2S) == RCC_PERIPHCLK_PLLI2S))  /* 源代码 */
  {                                                     /* 代码块开始 */
    /* check for Parameters */
    assert_param(IS_RCC_PLLI2SR_VALUE(PeriphClkInit->PLLI2S.PLLI2SR));  /* 执行语句 */
    assert_param(IS_RCC_PLLI2SN_VALUE(PeriphClkInit->PLLI2S.PLLI2SN));  /* 执行语句 */
#if defined(STM32F411xE)                                /* 条件编译 */
    assert_param(IS_RCC_PLLI2SM_VALUE(PeriphClkInit->PLLI2S.PLLI2SM));  /* 执行语句 */
#endif /* STM32F411xE */
    /* Disable the PLLI2S */
    __HAL_RCC_PLLI2S_DISABLE();                         /* HAL辅助宏/函数 */
    /* Get tick */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */
    /* Wait till PLLI2S is disabled */
    while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLI2SRDY)  != RESET)  /* while循环 */
    {                                                   /* 代码块开始 */
      if((HAL_GetTick() - tickstart ) > PLLI2S_TIMEOUT_VALUE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* return in case of Timeout detected */
        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

#if defined(STM32F411xE)                                /* 条件编译 */
    /* Configure the PLLI2S division factors */
    /* PLLI2S_VCO = f(VCO clock) = f(PLLI2S clock input) * (PLLI2SN/PLLI2SM) */
    /* I2SCLK = f(PLLI2S clock output) = f(VCO clock) / PLLI2SR */
    __HAL_RCC_PLLI2S_I2SCLK_CONFIG(PeriphClkInit->PLLI2S.PLLI2SM, PeriphClkInit->PLLI2S.PLLI2SN, PeriphClkInit->PLLI2S.PLLI2SR);  /* HAL辅助宏/函数 */
#else                                                   /* 条件编译否则 */
    /* Configure the PLLI2S division factors */
    /* PLLI2S_VCO = f(VCO clock) = f(PLLI2S clock input) * (PLLI2SN/PLLM) */
    /* I2SCLK = f(PLLI2S clock output) = f(VCO clock) / PLLI2SR */
    __HAL_RCC_PLLI2S_CONFIG(PeriphClkInit->PLLI2S.PLLI2SN , PeriphClkInit->PLLI2S.PLLI2SR);  /* HAL辅助宏/函数 */
#endif /* STM32F411xE */

    /* Enable the PLLI2S */
    __HAL_RCC_PLLI2S_ENABLE();                          /* HAL辅助宏/函数 */
    /* Get tick */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */
    /* Wait till PLLI2S is ready */
    while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLI2SRDY)  == RESET)  /* while循环 */
    {                                                   /* 代码块开始 */
      if((HAL_GetTick() - tickstart ) > PLLI2S_TIMEOUT_VALUE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* return in case of Timeout detected */
        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /*---------------------------- RTC configuration ---------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_RTC) == (RCC_PERIPHCLK_RTC))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check for RTC Parameters used to output RTCCLK */
    assert_param(IS_RCC_RTCCLKSOURCE(PeriphClkInit->RTCClockSelection));  /* 执行语句 */

    /* Enable Power Clock*/
    __HAL_RCC_PWR_CLK_ENABLE();                         /* HAL辅助宏/函数 */

    /* Enable write access to Backup domain */
    PWR->CR |= PWR_CR_DBP;                              /* 赋值或初始化 */

    /* Get tick */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */

    while((PWR->CR & PWR_CR_DBP) == RESET)              /* while循环 */
    {                                                   /* 代码块开始 */
      if((HAL_GetTick() - tickstart ) > RCC_DBP_TIMEOUT_VALUE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    /* Reset the Backup domain only if the RTC Clock source selection is modified from reset value */
    tmpreg1 = (RCC->BDCR & RCC_BDCR_RTCSEL);            /* 赋值或初始化 */
    if((tmpreg1 != 0x00000000U) && ((tmpreg1) != (PeriphClkInit->RTCClockSelection & RCC_BDCR_RTCSEL)))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Store the content of BDCR register before the reset of Backup Domain */
      tmpreg1 = (RCC->BDCR & ~(RCC_BDCR_RTCSEL));       /* 赋值或初始化 */
      /* RTC Clock selection can be changed only if the Backup Domain is reset */
      __HAL_RCC_BACKUPRESET_FORCE();                    /* HAL辅助宏/函数 */
      __HAL_RCC_BACKUPRESET_RELEASE();                  /* HAL辅助宏/函数 */
      /* Restore the Content of BDCR register */
      RCC->BDCR = tmpreg1;                              /* 赋值或初始化 */

      /* Wait for LSE reactivation if LSE was enable prior to Backup Domain reset */
      if(HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSEON))     /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Get tick */
        tickstart = HAL_GetTick();                      /* 赋值或初始化 */

        /* Wait till LSE is ready */
        while(__HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY) == RESET)  /* while循环 */
        {                                               /* 代码块开始 */
          if((HAL_GetTick() - tickstart ) > RCC_LSE_TIMEOUT_VALUE)  /* 条件判断 */
          {                                             /* 代码块开始 */
            return HAL_TIMEOUT;                         /* 返回值 */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    __HAL_RCC_RTC_CONFIG(PeriphClkInit->RTCClockSelection);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
#if defined(STM32F401xC) || defined(STM32F401xE) || defined(STM32F411xE)  /* 条件编译 */
  /*---------------------------- TIM configuration ---------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_TIM) == (RCC_PERIPHCLK_TIM))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    __HAL_RCC_TIMCLKPRESCALER(PeriphClkInit->TIMPresSelection);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
#endif /* STM32F401xC || STM32F401xE || STM32F411xE */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configures the RCC_OscInitStruct according to the internal
  * RCC configuration registers.
  * @param  PeriphClkInit pointer to an RCC_PeriphCLKInitTypeDef structure that
  * will be configured.
  * @retval None
  */
void HAL_RCCEx_GetPeriphCLKConfig(RCC_PeriphCLKInitTypeDef  *PeriphClkInit)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  uint32_t tempreg;                                     /* 32位无符号变量/函数 */

  /* Set all possible values for the extended clock type parameter------------*/
  PeriphClkInit->PeriphClockSelection = RCC_PERIPHCLK_I2S | RCC_PERIPHCLK_RTC;  /* 赋值或初始化 */

  /* Get the PLLI2S Clock configuration --------------------------------------*/
  PeriphClkInit->PLLI2S.PLLI2SN = (uint32_t)((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SN) >> RCC_PLLI2SCFGR_PLLI2SN_Pos);  /* 赋值或初始化 */
  PeriphClkInit->PLLI2S.PLLI2SR = (uint32_t)((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SR) >> RCC_PLLI2SCFGR_PLLI2SR_Pos);  /* 赋值或初始化 */
#if defined(STM32F411xE)                                /* 条件编译 */
  PeriphClkInit->PLLI2S.PLLI2SM = (uint32_t)(RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SM);  /* 赋值或初始化 */
#endif /* STM32F411xE */
  /* Get the RTC Clock configuration -----------------------------------------*/
  tempreg = (RCC->CFGR & RCC_CFGR_RTCPRE);              /* 赋值或初始化 */
  PeriphClkInit->RTCClockSelection = (uint32_t)((tempreg) | (RCC->BDCR & RCC_BDCR_RTCSEL));  /* 赋值或初始化 */

#if defined(STM32F401xC) || defined(STM32F401xE) || defined(STM32F411xE)  /* 条件编译 */
  /* Get the TIM Prescaler configuration -------------------------------------*/
  if ((RCC->DCKCFGR & RCC_DCKCFGR_TIMPRE) == RESET)     /* 条件判断 */
  {                                                     /* 代码块开始 */
    PeriphClkInit->TIMPresSelection = RCC_TIMPRES_DESACTIVATED;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    PeriphClkInit->TIMPresSelection = RCC_TIMPRES_ACTIVATED;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
#endif /* STM32F401xC || STM32F401xE || STM32F411xE */
}                                                       /* 代码块结束 */

/**
  * @brief  Return the peripheral clock frequency for a given peripheral(SAI..)
  * @note   Return 0 if peripheral clock identifier not managed by this API
  * @param  PeriphClk Peripheral clock identifier
  *         This parameter can be one of the following values:
  *            @arg RCC_PERIPHCLK_I2S: I2S peripheral clock
  * @retval Frequency in KHz
  */
uint32_t HAL_RCCEx_GetPeriphCLKFreq(uint32_t PeriphClk)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  /* This variable used to store the I2S clock frequency (value in Hz) */
  uint32_t frequency = 0U;                              /* 32位无符号变量/函数 */
  /* This variable used to store the VCO Input (value in Hz) */
  uint32_t vcoinput = 0U;                               /* 32位无符号变量/函数 */
  uint32_t srcclk = 0U;                                 /* 32位无符号变量/函数 */
  /* This variable used to store the VCO Output (value in Hz) */
  uint32_t vcooutput = 0U;                              /* 32位无符号变量/函数 */
  switch (PeriphClk)                                    /* switch分支 */
  {                                                     /* 代码块开始 */
  case RCC_PERIPHCLK_I2S:                               /* case标签 */
    {                                                   /* 代码块开始 */
      /* Get the current I2S source */
      srcclk = __HAL_RCC_GET_I2S_SOURCE();              /* 赋值或初始化 */
      switch (srcclk)                                   /* switch分支 */
      {                                                 /* 代码块开始 */
      /* Check if I2S clock selection is External clock mapped on the I2S_CKIN pin used as I2S clock */
      case RCC_I2SCLKSOURCE_EXT:                        /* case标签 */
        {                                               /* 代码块开始 */
          /* Set the I2S clock to the external clock  value */
          frequency = EXTERNAL_CLOCK_VALUE;             /* 赋值或初始化 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */
      /* Check if I2S clock selection is PLLI2S VCO output clock divided by PLLI2SR used as I2S clock */
      case RCC_I2SCLKSOURCE_PLLI2S:                     /* case标签 */
        {                                               /* 代码块开始 */
#if defined(STM32F411xE)                                /* 条件编译 */
          /* Configure the PLLI2S division factor */
          /* PLLI2S_VCO Input  = PLL_SOURCE/PLLI2SM */
          if((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLSOURCE_HSE)  /* 条件判断 */
          {                                             /* 代码块开始 */
            /* Get the I2S source clock value */
            vcoinput = (uint32_t)(HSE_VALUE / (uint32_t)(RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SM));  /* 赋值或初始化 */
          }                                             /* 代码块结束 */
          else                                          /* 否则 */
          {                                             /* 代码块开始 */
            /* Get the I2S source clock value */
            vcoinput = (uint32_t)(HSI_VALUE / (uint32_t)(RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SM));  /* 赋值或初始化 */
          }                                             /* 代码块结束 */
#else                                                   /* 条件编译否则 */
          /* Configure the PLLI2S division factor */
          /* PLLI2S_VCO Input  = PLL_SOURCE/PLLM */
          if((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLSOURCE_HSE)  /* 条件判断 */
          {                                             /* 代码块开始 */
            /* Get the I2S source clock value */
            vcoinput = (uint32_t)(HSE_VALUE / (uint32_t)(RCC->PLLCFGR & RCC_PLLCFGR_PLLM));  /* 赋值或初始化 */
          }                                             /* 代码块结束 */
          else                                          /* 否则 */
          {                                             /* 代码块开始 */
            /* Get the I2S source clock value */
            vcoinput = (uint32_t)(HSI_VALUE / (uint32_t)(RCC->PLLCFGR & RCC_PLLCFGR_PLLM));  /* 赋值或初始化 */
          }                                             /* 代码块结束 */
#endif /* STM32F411xE */
          /* PLLI2S_VCO Output = PLLI2S_VCO Input * PLLI2SN */
          vcooutput = (uint32_t)(vcoinput * (((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SN) >> 6U) & (RCC_PLLI2SCFGR_PLLI2SN >> 6U)));  /* 赋值或初始化 */
          /* I2S_CLK = PLLI2S_VCO Output/PLLI2SR */
          frequency = (uint32_t)(vcooutput /(((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SR) >> 28U) & (RCC_PLLI2SCFGR_PLLI2SR >> 28U)));  /* 赋值或初始化 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */
        /* Clock not enabled for I2S*/
      default:                                          /* default分支 */
        {                                               /* 代码块开始 */
          frequency = 0U;                               /* 赋值或初始化 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  return frequency;                                     /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* STM32F405xx || STM32F415xx || STM32F407xx || STM32F417xx || STM32F401xC || STM32F401xE  || STM32F411xE */

#if defined(STM32F410Tx) || defined(STM32F410Cx) || defined(STM32F410Rx) || defined(STM32F411xE) || defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx) || defined(STM32F412Zx) ||\
    defined(STM32F412Vx) || defined(STM32F412Rx) || defined(STM32F412Cx) || defined(STM32F413xx) || defined(STM32F423xx)  /* 源代码 */
/**
  * @brief  Select LSE mode
  *
  * @note   This mode is only available for STM32F410xx/STM32F411xx/STM32F446xx/STM32F469xx/STM32F479xx/STM32F412Zx/STM32F412Vx/STM32F412Rx/STM32F412Cx  devices.
  *
  * @param  Mode specifies the LSE mode.
  *          This parameter can be one of the following values:
  *            @arg RCC_LSE_LOWPOWER_MODE:  LSE oscillator in low power mode selection
  *            @arg RCC_LSE_HIGHDRIVE_MODE: LSE oscillator in High Drive mode selection
  * @retval None
  */
void HAL_RCCEx_SelectLSEMode(uint8_t Mode)              /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_RCC_LSE_MODE(Mode));                  /* 执行语句 */
  if(Mode == RCC_LSE_HIGHDRIVE_MODE)                    /* 条件判断 */
  {                                                     /* 代码块开始 */
    SET_BIT(RCC->BDCR, RCC_BDCR_LSEMOD);                /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    CLEAR_BIT(RCC->BDCR, RCC_BDCR_LSEMOD);              /* 执行语句 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

#endif /* STM32F410xx || STM32F411xE || STM32F446xx || STM32F469xx || STM32F479xx || STM32F412Zx || STM32F412Vx || STM32F412Rx || STM32F412Cx || STM32F413xx || STM32F423xx */

/** @defgroup RCCEx_Exported_Functions_Group2 Extended Clock management functions
 *  @brief  Extended Clock management functions
 *
@verbatim   
 ===============================================================================
                ##### Extended clock management functions  #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to control the 
    activation or deactivation of PLLI2S, PLLSAI.
@endverbatim
  * @{
  */

#if defined(RCC_PLLI2S_SUPPORT)                         /* 条件编译 */
/**
  * @brief  Enable PLLI2S.
  * @param  PLLI2SInit  pointer to an RCC_PLLI2SInitTypeDef structure that
  *         contains the configuration information for the PLLI2S
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCCEx_EnablePLLI2S(RCC_PLLI2SInitTypeDef  *PLLI2SInit)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */

  /* Check for parameters */
  assert_param(IS_RCC_PLLI2SN_VALUE(PLLI2SInit->PLLI2SN));  /* 执行语句 */
  assert_param(IS_RCC_PLLI2SR_VALUE(PLLI2SInit->PLLI2SR));  /* 执行语句 */
#if defined(RCC_PLLI2SCFGR_PLLI2SM)                     /* 条件编译 */
  assert_param(IS_RCC_PLLI2SM_VALUE(PLLI2SInit->PLLI2SM));  /* 执行语句 */
#endif /* RCC_PLLI2SCFGR_PLLI2SM */
#if defined(RCC_PLLI2SCFGR_PLLI2SP)                     /* 条件编译 */
  assert_param(IS_RCC_PLLI2SP_VALUE(PLLI2SInit->PLLI2SP));  /* 执行语句 */
#endif /* RCC_PLLI2SCFGR_PLLI2SP */
#if defined(RCC_PLLI2SCFGR_PLLI2SQ)                     /* 条件编译 */
  assert_param(IS_RCC_PLLI2SQ_VALUE(PLLI2SInit->PLLI2SQ));  /* 执行语句 */
#endif /* RCC_PLLI2SCFGR_PLLI2SQ */

  /* Disable the PLLI2S */
  __HAL_RCC_PLLI2S_DISABLE();                           /* HAL辅助宏/函数 */

  /* Wait till PLLI2S is disabled */
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */
  while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLI2SRDY) != RESET)  /* while循环 */
  {                                                     /* 代码块开始 */
    if((HAL_GetTick() - tickstart ) > PLLI2S_TIMEOUT_VALUE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* return in case of Timeout detected */
      return HAL_TIMEOUT;                               /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Configure the PLLI2S division factors */
#if defined(STM32F446xx)                                /* 条件编译 */
  /* PLLI2S_VCO = f(VCO clock) = f(PLLI2S clock input) * (PLLI2SN/PLLI2SM) */
  /* I2SPCLK = PLLI2S_VCO / PLLI2SP */
  /* I2SQCLK = PLLI2S_VCO / PLLI2SQ */
  /* I2SRCLK = PLLI2S_VCO / PLLI2SR */
  __HAL_RCC_PLLI2S_CONFIG(PLLI2SInit->PLLI2SM, PLLI2SInit->PLLI2SN, \
                          PLLI2SInit->PLLI2SP, PLLI2SInit->PLLI2SQ, PLLI2SInit->PLLI2SR);  /* 执行语句 */
#elif defined(STM32F412Zx) || defined(STM32F412Vx) || defined(STM32F412Rx) || defined(STM32F412Cx) ||\
      defined(STM32F413xx) || defined(STM32F423xx)      /* 源代码 */
  /* PLLI2S_VCO = f(VCO clock) = f(PLLI2S clock input) * (PLLI2SN/PLLI2SM)*/
  /* I2SQCLK = PLLI2S_VCO / PLLI2SQ */
  /* I2SRCLK = PLLI2S_VCO / PLLI2SR */
  __HAL_RCC_PLLI2S_CONFIG(PLLI2SInit->PLLI2SM, PLLI2SInit->PLLI2SN, \
                          PLLI2SInit->PLLI2SQ, PLLI2SInit->PLLI2SR);  /* 执行语句 */
#elif defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx) ||\
      defined(STM32F469xx) || defined(STM32F479xx)      /* 源代码 */
  /* PLLI2S_VCO = f(VCO clock) = f(PLLI2S clock input) * PLLI2SN */
  /* I2SQCLK = PLLI2S_VCO / PLLI2SQ */
  /* I2SRCLK = PLLI2S_VCO / PLLI2SR */
  __HAL_RCC_PLLI2S_SAICLK_CONFIG(PLLI2SInit->PLLI2SN, PLLI2SInit->PLLI2SQ, PLLI2SInit->PLLI2SR);  /* HAL辅助宏/函数 */
#elif defined(STM32F411xE)                              /* 条件编译否则如果 */
  /* PLLI2S_VCO = f(VCO clock) = f(PLLI2S clock input) * (PLLI2SN/PLLI2SM) */
  /* I2SRCLK = PLLI2S_VCO / PLLI2SR */
  __HAL_RCC_PLLI2S_I2SCLK_CONFIG(PLLI2SInit->PLLI2SM, PLLI2SInit->PLLI2SN, PLLI2SInit->PLLI2SR);  /* HAL辅助宏/函数 */
#else                                                   /* 条件编译否则 */
  /* PLLI2S_VCO = f(VCO clock) = f(PLLI2S clock input) x PLLI2SN */
  /* I2SRCLK = PLLI2S_VCO / PLLI2SR */
  __HAL_RCC_PLLI2S_CONFIG(PLLI2SInit->PLLI2SN, PLLI2SInit->PLLI2SR);  /* HAL辅助宏/函数 */
#endif /* STM32F446xx */

  /* Enable the PLLI2S */
  __HAL_RCC_PLLI2S_ENABLE();                            /* HAL辅助宏/函数 */

  /* Wait till PLLI2S is ready */
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */
  while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLI2SRDY) == RESET)  /* while循环 */
  {                                                     /* 代码块开始 */
    if((HAL_GetTick() - tickstart ) > PLLI2S_TIMEOUT_VALUE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* return in case of Timeout detected */
      return HAL_TIMEOUT;                               /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

 return HAL_OK;                                         /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Disable PLLI2S.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCCEx_DisablePLLI2S(void)         /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */

  /* Disable the PLLI2S */
  __HAL_RCC_PLLI2S_DISABLE();                           /* HAL辅助宏/函数 */

  /* Wait till PLLI2S is disabled */
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */
  while(READ_BIT(RCC->CR, RCC_CR_PLLI2SRDY) != RESET)   /* while循环 */
  {                                                     /* 代码块开始 */
    if((HAL_GetTick() - tickstart) > PLLI2S_TIMEOUT_VALUE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* return in case of Timeout detected */
      return HAL_TIMEOUT;                               /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

#endif /* RCC_PLLI2S_SUPPORT */

#if defined(RCC_PLLSAI_SUPPORT)                         /* 条件编译 */
/**
  * @brief  Enable PLLSAI.
  * @param  PLLSAIInit  pointer to an RCC_PLLSAIInitTypeDef structure that
  *         contains the configuration information for the PLLSAI
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCCEx_EnablePLLSAI(RCC_PLLSAIInitTypeDef  *PLLSAIInit)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */

  /* Check for parameters */
  assert_param(IS_RCC_PLLSAIN_VALUE(PLLSAIInit->PLLSAIN));  /* 执行语句 */
  assert_param(IS_RCC_PLLSAIQ_VALUE(PLLSAIInit->PLLSAIQ));  /* 执行语句 */
#if defined(RCC_PLLSAICFGR_PLLSAIM)                     /* 条件编译 */
  assert_param(IS_RCC_PLLSAIM_VALUE(PLLSAIInit->PLLSAIM));  /* 执行语句 */
#endif /* RCC_PLLSAICFGR_PLLSAIM */
#if defined(RCC_PLLSAICFGR_PLLSAIP)                     /* 条件编译 */
  assert_param(IS_RCC_PLLSAIP_VALUE(PLLSAIInit->PLLSAIP));  /* 执行语句 */
#endif /* RCC_PLLSAICFGR_PLLSAIP */
#if defined(RCC_PLLSAICFGR_PLLSAIR)                     /* 条件编译 */
  assert_param(IS_RCC_PLLSAIR_VALUE(PLLSAIInit->PLLSAIR));  /* 执行语句 */
#endif /* RCC_PLLSAICFGR_PLLSAIR */

  /* Disable the PLLSAI */
  __HAL_RCC_PLLSAI_DISABLE();                           /* HAL辅助宏/函数 */

  /* Wait till PLLSAI is disabled */
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */
  while(__HAL_RCC_PLLSAI_GET_FLAG() != RESET)           /* while循环 */
  {                                                     /* 代码块开始 */
    if((HAL_GetTick() - tickstart ) > PLLSAI_TIMEOUT_VALUE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* return in case of Timeout detected */
      return HAL_TIMEOUT;                               /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Configure the PLLSAI division factors */
#if defined(STM32F446xx)                                /* 条件编译 */
  /* PLLSAI_VCO = f(VCO clock) = f(PLLSAI clock input) * (PLLSAIN/PLLSAIM) */
  /* SAIPCLK = PLLSAI_VCO / PLLSAIP */
  /* SAIQCLK = PLLSAI_VCO / PLLSAIQ */
  /* SAIRCLK = PLLSAI_VCO / PLLSAIR */
  __HAL_RCC_PLLSAI_CONFIG(PLLSAIInit->PLLSAIM, PLLSAIInit->PLLSAIN, \
                          PLLSAIInit->PLLSAIP, PLLSAIInit->PLLSAIQ, 0U);  /* 执行语句 */
#elif defined(STM32F469xx) || defined(STM32F479xx)      /* 条件编译否则如果 */
  /* PLLSAI_VCO = f(VCO clock) = f(PLLSAI clock input) * PLLSAIN */
  /* SAIPCLK = PLLSAI_VCO / PLLSAIP */
  /* SAIQCLK = PLLSAI_VCO / PLLSAIQ */
  /* SAIRCLK = PLLSAI_VCO / PLLSAIR */
  __HAL_RCC_PLLSAI_CONFIG(PLLSAIInit->PLLSAIN, PLLSAIInit->PLLSAIP, \
                          PLLSAIInit->PLLSAIQ, PLLSAIInit->PLLSAIR);  /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* PLLSAI_VCO = f(VCO clock) = f(PLLSAI clock input) x PLLSAIN */
  /* SAIQCLK = PLLSAI_VCO / PLLSAIQ */
  /* SAIRCLK = PLLSAI_VCO / PLLSAIR */
  __HAL_RCC_PLLSAI_CONFIG(PLLSAIInit->PLLSAIN, PLLSAIInit->PLLSAIQ, PLLSAIInit->PLLSAIR);  /* HAL辅助宏/函数 */
#endif /* STM32F446xx */

  /* Enable the PLLSAI */
  __HAL_RCC_PLLSAI_ENABLE();                            /* HAL辅助宏/函数 */

  /* Wait till PLLSAI is ready */
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */
  while(__HAL_RCC_PLLSAI_GET_FLAG() == RESET)           /* while循环 */
  {                                                     /* 代码块开始 */
    if((HAL_GetTick() - tickstart ) > PLLSAI_TIMEOUT_VALUE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* return in case of Timeout detected */
      return HAL_TIMEOUT;                               /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

 return HAL_OK;                                         /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Disable PLLSAI.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCCEx_DisablePLLSAI(void)         /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */

  /* Disable the PLLSAI */
  __HAL_RCC_PLLSAI_DISABLE();                           /* HAL辅助宏/函数 */

  /* Wait till PLLSAI is disabled */
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */
  while(__HAL_RCC_PLLSAI_GET_FLAG() != RESET)           /* while循环 */
  {                                                     /* 代码块开始 */
    if((HAL_GetTick() - tickstart) > PLLSAI_TIMEOUT_VALUE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* return in case of Timeout detected */
      return HAL_TIMEOUT;                               /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

#endif /* RCC_PLLSAI_SUPPORT */

/**
  * @}
  */

#if defined(STM32F446xx)                                /* 条件编译 */
/**
  * @brief  Returns the SYSCLK frequency
  *
  * @note   This function implementation is valid only for STM32F446xx devices.
  * @note   This function add the PLL/PLLR System clock source
  *
  * @note   The system frequency computed by this function is not the real
  *         frequency in the chip. It is calculated based on the predefined
  *         constant and the selected clock source:
  * @note     If SYSCLK source is HSI, function returns values based on HSI_VALUE(*)
  * @note     If SYSCLK source is HSE, function returns values based on HSE_VALUE(**)
  * @note     If SYSCLK source is PLL or PLLR, function returns values based on HSE_VALUE(**)
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
uint32_t HAL_RCC_GetSysClockFreq(void)                  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t pllm = 0U;                                   /* 32位无符号变量/函数 */
  uint32_t pllvco = 0U;                                 /* 32位无符号变量/函数 */
  uint32_t pllp = 0U;                                   /* 32位无符号变量/函数 */
  uint32_t pllr = 0U;                                   /* 32位无符号变量/函数 */
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
    case RCC_CFGR_SWS_PLL:  /* PLL/PLLP used as system clock  source */
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
    case RCC_CFGR_SWS_PLLR:  /* PLL/PLLR used as system clock  source */
    {                                                   /* 代码块开始 */
      /* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLLM) * PLLN
      SYSCLK = PLL_VCO / PLLR */
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
      pllr = ((RCC->PLLCFGR & RCC_PLLCFGR_PLLR) >> RCC_PLLCFGR_PLLR_Pos);  /* 赋值或初始化 */

      sysclockfreq = pllvco/pllr;                       /* 赋值或初始化 */
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
#endif /* STM32F446xx */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @brief  Resets the RCC clock configuration to the default reset state.
  * @note   The default reset state of the clock configuration is given below:
  *            - HSI ON and used as system clock source
  *            - HSE, PLL, PLLI2S and PLLSAI OFF
  *            - AHB, APB1 and APB2 prescaler set to 1.
  *            - CSS, MCO1 and MCO2 OFF
  *            - All interrupts disabled
  * @note   This function doesn't modify the configuration of the
  *            - Peripheral clocks
  *            - LSI, LSE and RTC clocks
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCC_DeInit(void)                  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */

  /* Get Start Tick */
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  /* Set HSION bit to the reset value */
  SET_BIT(RCC->CR, RCC_CR_HSION);                       /* 执行语句 */

  /* Wait till HSI is ready */
  while (READ_BIT(RCC->CR, RCC_CR_HSIRDY) == RESET)     /* while循环 */
  {                                                     /* 代码块开始 */
    if ((HAL_GetTick() - tickstart) > HSI_TIMEOUT_VALUE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_TIMEOUT;                               /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Set HSITRIM[4:0] bits to the reset value */
  SET_BIT(RCC->CR, RCC_CR_HSITRIM_4);                   /* 执行语句 */

  /* Get Start Tick */
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  /* Reset CFGR register */
  CLEAR_REG(RCC->CFGR);                                 /* 执行语句 */

  /* Wait till clock switch is ready */
  while (READ_BIT(RCC->CFGR, RCC_CFGR_SWS) != RESET)    /* while循环 */
  {                                                     /* 代码块开始 */
    if ((HAL_GetTick() - tickstart) > CLOCKSWITCH_TIMEOUT_VALUE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_TIMEOUT;                               /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Get Start Tick */
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  /* Clear HSEON, HSEBYP and CSSON bits */
  CLEAR_BIT(RCC->CR, RCC_CR_HSEON | RCC_CR_HSEBYP | RCC_CR_CSSON);  /* 执行语句 */

  /* Wait till HSE is disabled */
  while (READ_BIT(RCC->CR, RCC_CR_HSERDY) != RESET)     /* while循环 */
  {                                                     /* 代码块开始 */
    if ((HAL_GetTick() - tickstart) > HSE_TIMEOUT_VALUE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_TIMEOUT;                               /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Get Start Tick */
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  /* Clear PLLON bit */
  CLEAR_BIT(RCC->CR, RCC_CR_PLLON);                     /* 执行语句 */

  /* Wait till PLL is disabled */
  while (READ_BIT(RCC->CR, RCC_CR_PLLRDY) != RESET)     /* while循环 */
  {                                                     /* 代码块开始 */
    if ((HAL_GetTick() - tickstart) > PLL_TIMEOUT_VALUE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_TIMEOUT;                               /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

#if defined(RCC_PLLI2S_SUPPORT)                         /* 条件编译 */
  /* Get Start Tick */
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  /* Reset PLLI2SON bit */
  CLEAR_BIT(RCC->CR, RCC_CR_PLLI2SON);                  /* 执行语句 */

  /* Wait till PLLI2S is disabled */
  while (READ_BIT(RCC->CR, RCC_CR_PLLI2SRDY) != RESET)  /* while循环 */
  {                                                     /* 代码块开始 */
    if ((HAL_GetTick() - tickstart) > PLLI2S_TIMEOUT_VALUE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_TIMEOUT;                               /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
#endif /* RCC_PLLI2S_SUPPORT */

#if defined(RCC_PLLSAI_SUPPORT)                         /* 条件编译 */
  /* Get Start Tick */
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  /* Reset PLLSAI bit */
  CLEAR_BIT(RCC->CR, RCC_CR_PLLSAION);                  /* 执行语句 */

  /* Wait till PLLSAI is disabled */
  while (READ_BIT(RCC->CR, RCC_CR_PLLSAIRDY) != RESET)  /* while循环 */
  {                                                     /* 代码块开始 */
    if ((HAL_GetTick() - tickstart) > PLLSAI_TIMEOUT_VALUE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_TIMEOUT;                               /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
#endif /* RCC_PLLSAI_SUPPORT */

  /* Once PLL, PLLI2S and PLLSAI are OFF, reset PLLCFGR register to default value */
#if defined(STM32F412Cx) || defined(STM32F412Rx) || defined(STM32F412Vx) || defined(STM32F412Zx) || defined(STM32F413xx) || \
    defined(STM32F423xx) || defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx)  /* 源代码 */
  RCC->PLLCFGR = RCC_PLLCFGR_PLLM_4 | RCC_PLLCFGR_PLLN_6 | RCC_PLLCFGR_PLLN_7 | RCC_PLLCFGR_PLLQ_2 | RCC_PLLCFGR_PLLR_1;  /* 赋值或初始化 */
#elif defined(STM32F410Tx) || defined(STM32F410Cx) || defined(STM32F410Rx)  /* 条件编译否则如果 */
  RCC->PLLCFGR = RCC_PLLCFGR_PLLR_0 | RCC_PLLCFGR_PLLR_1 | RCC_PLLCFGR_PLLR_2 | RCC_PLLCFGR_PLLM_4 | RCC_PLLCFGR_PLLN_6 | RCC_PLLCFGR_PLLN_7 | RCC_PLLCFGR_PLLQ_0 | RCC_PLLCFGR_PLLQ_1 | RCC_PLLCFGR_PLLQ_2 | RCC_PLLCFGR_PLLQ_3;  /* 赋值或初始化 */
#else                                                   /* 条件编译否则 */
  RCC->PLLCFGR = RCC_PLLCFGR_PLLM_4 | RCC_PLLCFGR_PLLN_6 | RCC_PLLCFGR_PLLN_7 | RCC_PLLCFGR_PLLQ_2;  /* 赋值或初始化 */
#endif /* STM32F412Cx || STM32F412Rx || STM32F412Vx || STM32F412Zx || STM32F413xx || STM32F423xx || STM32F446xx || STM32F469xx || STM32F479xx */

  /* Reset PLLI2SCFGR register to default value */
#if defined(STM32F412Cx) || defined(STM32F412Rx) || defined(STM32F412Vx) || defined(STM32F412Zx) || defined(STM32F413xx) || \
    defined(STM32F423xx) || defined(STM32F446xx)        /* 源代码 */
  RCC->PLLI2SCFGR = RCC_PLLI2SCFGR_PLLI2SM_4 | RCC_PLLI2SCFGR_PLLI2SN_6 | RCC_PLLI2SCFGR_PLLI2SN_7 | RCC_PLLI2SCFGR_PLLI2SQ_2 | RCC_PLLI2SCFGR_PLLI2SR_1;  /* 赋值或初始化 */
#elif defined(STM32F401xC) || defined(STM32F401xE) || defined(STM32F405xx) || defined(STM32F415xx) || defined(STM32F407xx) || defined(STM32F417xx)  /* 条件编译否则如果 */
  RCC->PLLI2SCFGR = RCC_PLLI2SCFGR_PLLI2SN_6 | RCC_PLLI2SCFGR_PLLI2SN_7 | RCC_PLLI2SCFGR_PLLI2SR_1;  /* 赋值或初始化 */
#elif defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx) || defined(STM32F469xx) || defined(STM32F479xx)  /* 条件编译否则如果 */
  RCC->PLLI2SCFGR = RCC_PLLI2SCFGR_PLLI2SN_6 | RCC_PLLI2SCFGR_PLLI2SN_7 | RCC_PLLI2SCFGR_PLLI2SQ_2 | RCC_PLLI2SCFGR_PLLI2SR_1;  /* 赋值或初始化 */
#elif defined(STM32F411xE)                              /* 条件编译否则如果 */
  RCC->PLLI2SCFGR = RCC_PLLI2SCFGR_PLLI2SM_4 | RCC_PLLI2SCFGR_PLLI2SN_6 | RCC_PLLI2SCFGR_PLLI2SN_7 | RCC_PLLI2SCFGR_PLLI2SR_1;  /* 赋值或初始化 */
#endif /* STM32F412Cx || STM32F412Rx || STM32F412Vx || STM32F412Zx || STM32F413xx || STM32F423xx || STM32F446xx */

  /* Reset PLLSAICFGR register */
#if defined(STM32F427xx) || defined(STM32F429xx) || defined(STM32F437xx) || defined(STM32F439xx) || defined(STM32F469xx) || defined(STM32F479xx)  /* 条件编译 */
  RCC->PLLSAICFGR = RCC_PLLSAICFGR_PLLSAIN_6 | RCC_PLLSAICFGR_PLLSAIN_7 | RCC_PLLSAICFGR_PLLSAIQ_2 | RCC_PLLSAICFGR_PLLSAIR_1;  /* 赋值或初始化 */
#elif defined(STM32F446xx)                              /* 条件编译否则如果 */
  RCC->PLLSAICFGR = RCC_PLLSAICFGR_PLLSAIM_4 | RCC_PLLSAICFGR_PLLSAIN_6 | RCC_PLLSAICFGR_PLLSAIN_7 | RCC_PLLSAICFGR_PLLSAIQ_2;  /* 赋值或初始化 */
#endif /* STM32F427xx || STM32F429xx || STM32F437xx || STM32F439xx || STM32F469xx || STM32F479xx */

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

  /* Update the SystemCoreClock global variable */
  SystemCoreClock = HSI_VALUE;                          /* 赋值或初始化 */

  /* Adapt Systick interrupt period */
  if(HAL_InitTick(uwTickPrio) != HAL_OK)                /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

#if defined(STM32F410Tx) || defined(STM32F410Cx) || defined(STM32F410Rx) || defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx) || defined(STM32F412Zx) ||\
    defined(STM32F412Vx) || defined(STM32F412Rx) || defined(STM32F412Cx) || defined(STM32F413xx) || defined(STM32F423xx)  /* 源代码 */
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
  * @note   This function add the PLL/PLLR factor management during PLL configuration this feature
  *         is only available in STM32F410xx/STM32F446xx/STM32F469xx/STM32F479xx/STM32F412Zx/STM32F412Vx/STM32F412Rx/STM32F412Cx devices
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCC_OscConfig(RCC_OscInitTypeDef  *RCC_OscInitStruct)  /* HAL状态类型函数 */
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
#if defined(STM32F446xx)                                /* 条件编译 */
    if((__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_CFGR_SWS_HSE)                                                                     ||\
      ((__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_CFGR_SWS_PLL) && ((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLCFGR_PLLSRC_HSE)) ||\
      ((__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_CFGR_SWS_PLLR) && ((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLCFGR_PLLSRC_HSE)))  /* 源代码 */
#else                                                   /* 条件编译否则 */
    if((__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_CFGR_SWS_HSE)                                                                     ||\
      ((__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_CFGR_SWS_PLL) && ((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLCFGR_PLLSRC_HSE)))  /* 源代码 */
#endif /* STM32F446xx */
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
        /* Get Start Tick*/
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
        /* Get Start Tick*/
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
#if defined(STM32F446xx)                                /* 条件编译 */
    if((__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_CFGR_SWS_HSI)                                                                     ||\
      ((__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_CFGR_SWS_PLL) && ((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLCFGR_PLLSRC_HSI)) ||\
      ((__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_CFGR_SWS_PLLR) && ((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLCFGR_PLLSRC_HSI)))  /* 源代码 */
#else                                                   /* 条件编译否则 */
    if((__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_CFGR_SWS_HSI)                                                                     ||\
      ((__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_CFGR_SWS_PLL) && ((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLCFGR_PLLSRC_HSI)))  /* 源代码 */
#endif /* STM32F446xx */
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

        /* Adjusts the Internal High Speed oscillator (HSI) calibration value.*/
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

      /* Get Start Tick*/
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
      /* Get Start Tick*/
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
        assert_param(IS_RCC_PLLR_VALUE(RCC_OscInitStruct->PLL.PLLR));  /* 执行语句 */

        /* Disable the main PLL. */
        __HAL_RCC_PLL_DISABLE();                        /* HAL辅助宏/函数 */

        /* Get Start Tick*/
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
                                 (RCC_OscInitStruct->PLL.PLLN << RCC_PLLCFGR_PLLN_Pos)                       | \
                                 (((RCC_OscInitStruct->PLL.PLLP >> 1U) - 1U) << RCC_PLLCFGR_PLLP_Pos)        | \
                                 (RCC_OscInitStruct->PLL.PLLQ << RCC_PLLCFGR_PLLQ_Pos)                       | \
                                 (RCC_OscInitStruct->PLL.PLLR << RCC_PLLCFGR_PLLR_Pos)));  /* 执行语句 */
        /* Enable the main PLL. */
        __HAL_RCC_PLL_ENABLE();                         /* HAL辅助宏/函数 */

        /* Get Start Tick*/
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

        /* Get Start Tick*/
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
  * @brief  Configures the RCC_OscInitStruct according to the internal
  * RCC configuration registers.
  * @param  RCC_OscInitStruct pointer to an RCC_OscInitTypeDef structure that will be configured.
  *
  * @note   This function is only available in case of STM32F410xx/STM32F446xx/STM32F469xx/STM32F479xx/STM32F412Zx/STM32F412Vx/STM32F412Rx/STM32F412Cx devices.
  * @note   This function add the PLL/PLLR factor management
  * @retval None
  */
void HAL_RCC_GetOscConfig(RCC_OscInitTypeDef  *RCC_OscInitStruct)  /* 函数定义或声明 */
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
  RCC_OscInitStruct->PLL.PLLR = (uint32_t)((RCC->PLLCFGR & RCC_PLLCFGR_PLLR) >> RCC_PLLCFGR_PLLR_Pos);  /* 时钟相关操作 */
}                                                       /* 代码块结束 */
#endif /* STM32F410xx || STM32F446xx || STM32F469xx || STM32F479xx || STM32F412Zx || STM32F412Vx || STM32F412Rx || STM32F412Cx || STM32F413xx || STM32F423xx */

#endif /* HAL_RCC_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
