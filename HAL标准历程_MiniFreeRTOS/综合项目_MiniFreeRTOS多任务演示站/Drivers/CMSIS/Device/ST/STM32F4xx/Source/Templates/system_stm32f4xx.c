/* 文件: system_stm32f4xx.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    system_stm32f4xx.c
  * @author  MCD Application Team
  * @brief   CMSIS Cortex-M4 Device Peripheral Access Layer System Source File.
  *
  *   This file provides two functions and one global variable to be called from 
  *   user application:
  *      - SystemInit(): This function is called at startup just after reset and 
  *                      before branch to main program. This call is made inside
  *                      the "startup_stm32f4xx.s" file.
  *
  *      - SystemCoreClock variable: Contains the core clock (HCLK), it can be used
  *                                  by the user application to setup the SysTick 
  *                                  timer or configure other parameters.
  *                                     
  *      - SystemCoreClockUpdate(): Updates the variable SystemCoreClock and must
  *                                 be called whenever the core clock is changed
  *                                 during program execution.
  *
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

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup stm32f4xx_system
  * @{
  */  

/** @addtogroup STM32F4xx_System_Private_Includes
  * @{
  */


#include "stm32f4xx.h"                                  /* 包含 STM32/HAL 头文件 */

#if !defined  (HSE_VALUE)                               /* 条件编译 */
  #define HSE_VALUE    ((uint32_t)25000000) /*!< Default value of the External oscillator in Hz */
#endif /* HSE_VALUE */

#if !defined  (HSI_VALUE)                               /* 条件编译 */
  #define HSI_VALUE    ((uint32_t)16000000) /*!< Value of the Internal oscillator in Hz*/
#endif /* HSI_VALUE */

/**
  * @}
  */

/** @addtogroup STM32F4xx_System_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32F4xx_System_Private_Defines
  * @{
  */

/************************* Miscellaneous Configuration ************************/
/*!< Uncomment the following line if you need to use external SRAM or SDRAM as data memory  */
#if defined(STM32F405xx) || defined(STM32F415xx) || defined(STM32F407xx) || defined(STM32F417xx)\
 || defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx)\
 || defined(STM32F469xx) || defined(STM32F479xx) || defined(STM32F412Zx) || defined(STM32F412Vx)  /* 源代码 */
/* #define DATA_IN_ExtSRAM */
#endif /* STM32F40xxx || STM32F41xxx || STM32F42xxx || STM32F43xxx || STM32F469xx || STM32F479xx ||\
          STM32F412Zx || STM32F412Vx */

#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx)\
 || defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx)  /* 源代码 */
/* #define DATA_IN_ExtSDRAM */
#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx || STM32F446xx || STM32F469xx ||\
          STM32F479xx */

/* Note: Following vector table addresses must be defined in line with linker
         configuration. */
/*!< Uncomment the following line if you need to relocate the vector table
     anywhere in Flash or Sram, else the vector table is kept at the automatic
     remap of boot address selected */
/* #define USER_VECT_TAB_ADDRESS */

#if defined(USER_VECT_TAB_ADDRESS)                      /* 条件编译 */
/*!< Uncomment the following line if you need to relocate your vector Table
     in Sram else user remap will be done in Flash. */
/* #define VECT_TAB_SRAM */
#if defined(VECT_TAB_SRAM)                              /* 条件编译 */
#define VECT_TAB_BASE_ADDRESS   SRAM_BASE       /*!< Vector Table base address field.
                                                     This value must be a multiple of 0x200. */
#define VECT_TAB_OFFSET         0x00000000U     /*!< Vector Table base offset field.
                                                     This value must be a multiple of 0x200. */
#else                                                   /* 条件编译否则 */
#define VECT_TAB_BASE_ADDRESS   FLASH_BASE      /*!< Vector Table base address field.
                                                     This value must be a multiple of 0x200. */
#define VECT_TAB_OFFSET         0x00000000U     /*!< Vector Table base offset field.
                                                     This value must be a multiple of 0x200. */
#endif /* VECT_TAB_SRAM */
#endif /* USER_VECT_TAB_ADDRESS */
/******************************************************************************/

/**
  * @}
  */

/** @addtogroup STM32F4xx_System_Private_Macros
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32F4xx_System_Private_Variables
  * @{
  */
  /* This variable is updated in three ways:
      1) by calling CMSIS function SystemCoreClockUpdate()
      2) by calling HAL API function HAL_RCC_GetHCLKFreq()
      3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency 
         Note: If you use this function to configure the system clock; then there
               is no need to call the 2 first functions listed above, since SystemCoreClock
               variable is updated automatically.
  */
uint32_t SystemCoreClock = 16000000;                    /* 32位无符号变量/函数 */
const uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};  /* 常量定义 */
const uint8_t APBPrescTable[8]  = {0, 0, 0, 0, 1, 2, 3, 4};  /* 常量定义 */
/**
  * @}
  */

/** @addtogroup STM32F4xx_System_Private_FunctionPrototypes
  * @{
  */

#if defined (DATA_IN_ExtSRAM) || defined (DATA_IN_ExtSDRAM)  /* 条件编译 */
  static void SystemInit_ExtMemCtl(void);               /* 静态函数 */
#endif /* DATA_IN_ExtSRAM || DATA_IN_ExtSDRAM */

/**
  * @}
  */

/** @addtogroup STM32F4xx_System_Private_Functions
  * @{
  */

/**
  * @brief  Setup the microcontroller system
  *         Initialize the FPU setting, vector table location and External memory 
  *         configuration.
  * @param  None
  * @retval None
  */
void SystemInit(void)                                   /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* FPU settings ------------------------------------------------------------*/
  #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)         /* 条件编译 */
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  /* set CP10 and CP11 Full Access */
  #endif                                                /* 条件编译结束 */

#if defined (DATA_IN_ExtSRAM) || defined (DATA_IN_ExtSDRAM)  /* 条件编译 */
  SystemInit_ExtMemCtl();                               /* 执行语句 */
#endif /* DATA_IN_ExtSRAM || DATA_IN_ExtSDRAM */

  /* Configure the Vector Table location -------------------------------------*/
#if defined(USER_VECT_TAB_ADDRESS)                      /* 条件编译 */
  SCB->VTOR = VECT_TAB_BASE_ADDRESS | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM */
#endif /* USER_VECT_TAB_ADDRESS */
}                                                       /* 代码块结束 */

/**
   * @brief  Update SystemCoreClock variable according to Clock Register Values.
  *         The SystemCoreClock variable contains the core clock (HCLK), it can
  *         be used by the user application to setup the SysTick timer or configure
  *         other parameters.
  *           
  * @note   Each time the core clock (HCLK) changes, this function must be called
  *         to update SystemCoreClock variable value. Otherwise, any configuration
  *         based on this variable will be incorrect.         
  *     
  * @note   - The system frequency computed by this function is not the real 
  *           frequency in the chip. It is calculated based on the predefined 
  *           constant and the selected clock source:
  *             
  *           - If SYSCLK source is HSI, SystemCoreClock will contain the HSI_VALUE(*)
  *                                              
  *           - If SYSCLK source is HSE, SystemCoreClock will contain the HSE_VALUE(**)
  *                          
  *           - If SYSCLK source is PLL, SystemCoreClock will contain the HSE_VALUE(**) 
  *             or HSI_VALUE(*) multiplied/divided by the PLL factors.
  *         
  *         (*) HSI_VALUE is a constant defined in stm32f4xx_hal_conf.h file (default value
  *             16 MHz) but the real value may vary depending on the variations
  *             in voltage and temperature.   
  *    
  *         (**) HSE_VALUE is a constant defined in stm32f4xx_hal_conf.h file (its value
  *              depends on the application requirements), user has to ensure that HSE_VALUE
  *              is same as the real frequency of the crystal used. Otherwise, this function
  *              may have wrong result.
  *                
  *         - The result of this function could be not correct when using fractional
  *           value for HSE crystal.
  *     
  * @param  None
  * @retval None
  */
void SystemCoreClockUpdate(void)                        /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  uint32_t tmp = 0, pllvco = 0, pllp = 2, pllsource = 0, pllm = 2;  /* 32位无符号变量/函数 */

  /* Get SYSCLK source -------------------------------------------------------*/
  tmp = RCC->CFGR & RCC_CFGR_SWS;                       /* 赋值或初始化 */

  switch (tmp)                                          /* switch分支 */
  {                                                     /* 代码块开始 */
    case 0x00:  /* HSI used as system clock source */
      SystemCoreClock = HSI_VALUE;                      /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case 0x04:  /* HSE used as system clock source */
      SystemCoreClock = HSE_VALUE;                      /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case 0x08:  /* PLL used as system clock source */

      /* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_M) * PLL_N
         SYSCLK = PLL_VCO / PLL_P
         */    
      pllsource = (RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) >> 22;  /* 赋值或初始化 */
      pllm = RCC->PLLCFGR & RCC_PLLCFGR_PLLM;           /* 赋值或初始化 */

      if (pllsource != 0)                               /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* HSE used as PLL clock source */
        pllvco = (HSE_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* HSI used as PLL clock source */
        pllvco = (HSI_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */

      pllp = (((RCC->PLLCFGR & RCC_PLLCFGR_PLLP) >>16) + 1 ) *2;  /* 赋值或初始化 */
      SystemCoreClock = pllvco/pllp;                    /* 赋值或初始化 */
      break;                                            /* 跳出 */
    default:                                            /* default分支 */
      SystemCoreClock = HSI_VALUE;                      /* 赋值或初始化 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */
  /* Compute HCLK frequency --------------------------------------------------*/
  /* Get HCLK prescaler */
  tmp = AHBPrescTable[((RCC->CFGR & RCC_CFGR_HPRE) >> 4)];  /* 赋值或初始化 */
  /* HCLK frequency */
  SystemCoreClock >>= tmp;                              /* 赋值或初始化 */
}                                                       /* 代码块结束 */

#if defined (DATA_IN_ExtSRAM) && defined (DATA_IN_ExtSDRAM)  /* 条件编译 */
#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx)\
 || defined(STM32F469xx) || defined(STM32F479xx)        /* 源代码 */
/**
  * @brief  Setup the external memory controller.
  *         Called in startup_stm32f4xx.s before jump to main.
  *         This function configures the external memories (SRAM/SDRAM)
  *         This SRAM/SDRAM will be used as program data memory (including heap and stack).
  * @param  None
  * @retval None
  */
void SystemInit_ExtMemCtl(void)                         /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  __IO uint32_t tmp = 0x00;                             /* 赋值或初始化 */

  register uint32_t tmpreg = 0, timeout = 0xFFFF;       /* 赋值或初始化 */
  register __IO uint32_t index;                         /* 执行语句 */

  /* Enable GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH and GPIOI interface clock */
  RCC->AHB1ENR |= 0x000001F8;                           /* 赋值或初始化 */

  /* Delay after an RCC peripheral clock enabling */
  tmp = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN);    /* 赋值或初始化 */

  /* Connect PDx pins to FMC Alternate function */
  GPIOD->AFR[0]  = 0x00CCC0CC;                          /* 赋值或初始化 */
  GPIOD->AFR[1]  = 0xCCCCCCCC;                          /* 赋值或初始化 */
  /* Configure PDx pins in Alternate function mode */  
  GPIOD->MODER   = 0xAAAA0A8A;                          /* 赋值或初始化 */
  /* Configure PDx pins speed to 100 MHz */  
  GPIOD->OSPEEDR = 0xFFFF0FCF;                          /* 赋值或初始化 */
  /* Configure PDx pins Output type to push-pull */  
  GPIOD->OTYPER  = 0x00000000;                          /* 赋值或初始化 */
  /* No pull-up, pull-down for PDx pins */ 
  GPIOD->PUPDR   = 0x00000000;                          /* 赋值或初始化 */

  /* Connect PEx pins to FMC Alternate function */
  GPIOE->AFR[0]  = 0xC00CC0CC;                          /* 赋值或初始化 */
  GPIOE->AFR[1]  = 0xCCCCCCCC;                          /* 赋值或初始化 */
  /* Configure PEx pins in Alternate function mode */ 
  GPIOE->MODER   = 0xAAAA828A;                          /* 赋值或初始化 */
  /* Configure PEx pins speed to 100 MHz */ 
  GPIOE->OSPEEDR = 0xFFFFC3CF;                          /* 赋值或初始化 */
  /* Configure PEx pins Output type to push-pull */  
  GPIOE->OTYPER  = 0x00000000;                          /* 赋值或初始化 */
  /* No pull-up, pull-down for PEx pins */ 
  GPIOE->PUPDR   = 0x00000000;                          /* 赋值或初始化 */

  /* Connect PFx pins to FMC Alternate function */
  GPIOF->AFR[0]  = 0xCCCCCCCC;                          /* 赋值或初始化 */
  GPIOF->AFR[1]  = 0xCCCCCCCC;                          /* 赋值或初始化 */
  /* Configure PFx pins in Alternate function mode */   
  GPIOF->MODER   = 0xAA800AAA;                          /* 赋值或初始化 */
  /* Configure PFx pins speed to 50 MHz */ 
  GPIOF->OSPEEDR = 0xAA800AAA;                          /* 赋值或初始化 */
  /* Configure PFx pins Output type to push-pull */  
  GPIOF->OTYPER  = 0x00000000;                          /* 赋值或初始化 */
  /* No pull-up, pull-down for PFx pins */ 
  GPIOF->PUPDR   = 0x00000000;                          /* 赋值或初始化 */

  /* Connect PGx pins to FMC Alternate function */
  GPIOG->AFR[0]  = 0xCCCCCCCC;                          /* 赋值或初始化 */
  GPIOG->AFR[1]  = 0xCCCCCCCC;                          /* 赋值或初始化 */
  /* Configure PGx pins in Alternate function mode */ 
  GPIOG->MODER   = 0xAAAAAAAA;                          /* 赋值或初始化 */
  /* Configure PGx pins speed to 50 MHz */ 
  GPIOG->OSPEEDR = 0xAAAAAAAA;                          /* 赋值或初始化 */
  /* Configure PGx pins Output type to push-pull */  
  GPIOG->OTYPER  = 0x00000000;                          /* 赋值或初始化 */
  /* No pull-up, pull-down for PGx pins */ 
  GPIOG->PUPDR   = 0x00000000;                          /* 赋值或初始化 */

  /* Connect PHx pins to FMC Alternate function */
  GPIOH->AFR[0]  = 0x00C0CC00;                          /* 赋值或初始化 */
  GPIOH->AFR[1]  = 0xCCCCCCCC;                          /* 赋值或初始化 */
  /* Configure PHx pins in Alternate function mode */ 
  GPIOH->MODER   = 0xAAAA08A0;                          /* 赋值或初始化 */
  /* Configure PHx pins speed to 50 MHz */ 
  GPIOH->OSPEEDR = 0xAAAA08A0;                          /* 赋值或初始化 */
  /* Configure PHx pins Output type to push-pull */  
  GPIOH->OTYPER  = 0x00000000;                          /* 赋值或初始化 */
  /* No pull-up, pull-down for PHx pins */ 
  GPIOH->PUPDR   = 0x00000000;                          /* 赋值或初始化 */

  /* Connect PIx pins to FMC Alternate function */
  GPIOI->AFR[0]  = 0xCCCCCCCC;                          /* 赋值或初始化 */
  GPIOI->AFR[1]  = 0x00000CC0;                          /* 赋值或初始化 */
  /* Configure PIx pins in Alternate function mode */ 
  GPIOI->MODER   = 0x0028AAAA;                          /* 赋值或初始化 */
  /* Configure PIx pins speed to 50 MHz */ 
  GPIOI->OSPEEDR = 0x0028AAAA;                          /* 赋值或初始化 */
  /* Configure PIx pins Output type to push-pull */  
  GPIOI->OTYPER  = 0x00000000;                          /* 赋值或初始化 */
  /* No pull-up, pull-down for PIx pins */ 
  GPIOI->PUPDR   = 0x00000000;                          /* 赋值或初始化 */

/*-- FMC Configuration -------------------------------------------------------*/
  /* Enable the FMC interface clock */
  RCC->AHB3ENR |= 0x00000001;                           /* 赋值或初始化 */
  /* Delay after an RCC peripheral clock enabling */
  tmp = READ_BIT(RCC->AHB3ENR, RCC_AHB3ENR_FMCEN);      /* 赋值或初始化 */

  FMC_Bank5_6->SDCR[0] = 0x000019E4;                    /* 赋值或初始化 */
  FMC_Bank5_6->SDTR[0] = 0x01115351;                    /* 赋值或初始化 */

  /* SDRAM initialization sequence */
  /* Clock enable command */
  FMC_Bank5_6->SDCMR = 0x00000011;                      /* 赋值或初始化 */
  tmpreg = FMC_Bank5_6->SDSR & 0x00000020;              /* 赋值或初始化 */
  while((tmpreg != 0) && (timeout-- > 0))               /* while循环 */
  {                                                     /* 代码块开始 */
    tmpreg = FMC_Bank5_6->SDSR & 0x00000020;            /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Delay */
  for (index = 0; index<1000; index++);                 /* for循环 */

  /* PALL command */
  FMC_Bank5_6->SDCMR = 0x00000012;                      /* 赋值或初始化 */
  tmpreg = FMC_Bank5_6->SDSR & 0x00000020;              /* 赋值或初始化 */
  timeout = 0xFFFF;                                     /* 赋值或初始化 */
  while((tmpreg != 0) && (timeout-- > 0))               /* while循环 */
  {                                                     /* 代码块开始 */
    tmpreg = FMC_Bank5_6->SDSR & 0x00000020;            /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Auto refresh command */
  FMC_Bank5_6->SDCMR = 0x00000073;                      /* 赋值或初始化 */
  tmpreg = FMC_Bank5_6->SDSR & 0x00000020;              /* 赋值或初始化 */
  timeout = 0xFFFF;                                     /* 赋值或初始化 */
  while((tmpreg != 0) && (timeout-- > 0))               /* while循环 */
  {                                                     /* 代码块开始 */
    tmpreg = FMC_Bank5_6->SDSR & 0x00000020;            /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* MRD register program */
  FMC_Bank5_6->SDCMR = 0x00046014;                      /* 赋值或初始化 */
  tmpreg = FMC_Bank5_6->SDSR & 0x00000020;              /* 赋值或初始化 */
  timeout = 0xFFFF;                                     /* 赋值或初始化 */
  while((tmpreg != 0) && (timeout-- > 0))               /* while循环 */
  {                                                     /* 代码块开始 */
    tmpreg = FMC_Bank5_6->SDSR & 0x00000020;            /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Set refresh count */
  tmpreg = FMC_Bank5_6->SDRTR;                          /* 赋值或初始化 */
  FMC_Bank5_6->SDRTR = (tmpreg | (0x0000027C<<1));      /* 赋值或初始化 */

  /* Disable write protection */
  tmpreg = FMC_Bank5_6->SDCR[0];                        /* 赋值或初始化 */
  FMC_Bank5_6->SDCR[0] = (tmpreg & 0xFFFFFDFF);         /* 赋值或初始化 */

#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx)  /* 条件编译 */
  /* Configure and enable Bank1_SRAM2 */
  FMC_Bank1->BTCR[2]  = 0x00001011;                     /* 赋值或初始化 */
  FMC_Bank1->BTCR[3]  = 0x00000201;                     /* 赋值或初始化 */
  FMC_Bank1E->BWTR[2] = 0x0fffffff;                     /* 赋值或初始化 */
#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx */ 
#if defined(STM32F469xx) || defined(STM32F479xx)        /* 条件编译 */
  /* Configure and enable Bank1_SRAM2 */
  FMC_Bank1->BTCR[2]  = 0x00001091;                     /* 赋值或初始化 */
  FMC_Bank1->BTCR[3]  = 0x00110212;                     /* 赋值或初始化 */
  FMC_Bank1E->BWTR[2] = 0x0fffffff;                     /* 赋值或初始化 */
#endif /* STM32F469xx || STM32F479xx */

  (void)(tmp);                                          /* 执行语句 */
}                                                       /* 代码块结束 */
#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx || STM32F469xx || STM32F479xx */
#elif defined (DATA_IN_ExtSRAM) || defined (DATA_IN_ExtSDRAM)  /* 条件编译否则如果 */
/**
  * @brief  Setup the external memory controller.
  *         Called in startup_stm32f4xx.s before jump to main.
  *         This function configures the external memories (SRAM/SDRAM)
  *         This SRAM/SDRAM will be used as program data memory (including heap and stack).
  * @param  None
  * @retval None
  */
void SystemInit_ExtMemCtl(void)                         /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  __IO uint32_t tmp = 0x00;                             /* 赋值或初始化 */
#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx)\
 || defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx)  /* 源代码 */
#if defined (DATA_IN_ExtSDRAM)                          /* 条件编译 */
  register uint32_t tmpreg = 0, timeout = 0xFFFF;       /* 赋值或初始化 */
  register __IO uint32_t index;                         /* 执行语句 */

#if defined(STM32F446xx)                                /* 条件编译 */
  /* Enable GPIOA, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG interface
      clock */
  RCC->AHB1ENR |= 0x0000007D;                           /* 赋值或初始化 */
#else                                                   /* 条件编译否则 */
  /* Enable GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH and GPIOI interface 
      clock */
  RCC->AHB1ENR |= 0x000001F8;                           /* 赋值或初始化 */
#endif /* STM32F446xx */  
  /* Delay after an RCC peripheral clock enabling */
  tmp = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN);    /* 赋值或初始化 */

#if defined(STM32F446xx)                                /* 条件编译 */
  /* Connect PAx pins to FMC Alternate function */
  GPIOA->AFR[0]  |= 0xC0000000;                         /* 赋值或初始化 */
  GPIOA->AFR[1]  |= 0x00000000;                         /* 赋值或初始化 */
  /* Configure PDx pins in Alternate function mode */
  GPIOA->MODER   |= 0x00008000;                         /* 赋值或初始化 */
  /* Configure PDx pins speed to 50 MHz */
  GPIOA->OSPEEDR |= 0x00008000;                         /* 赋值或初始化 */
  /* Configure PDx pins Output type to push-pull */
  GPIOA->OTYPER  |= 0x00000000;                         /* 赋值或初始化 */
  /* No pull-up, pull-down for PDx pins */
  GPIOA->PUPDR   |= 0x00000000;                         /* 赋值或初始化 */

  /* Connect PCx pins to FMC Alternate function */
  GPIOC->AFR[0]  |= 0x00CC0000;                         /* 赋值或初始化 */
  GPIOC->AFR[1]  |= 0x00000000;                         /* 赋值或初始化 */
  /* Configure PDx pins in Alternate function mode */
  GPIOC->MODER   |= 0x00000A00;                         /* 赋值或初始化 */
  /* Configure PDx pins speed to 50 MHz */
  GPIOC->OSPEEDR |= 0x00000A00;                         /* 赋值或初始化 */
  /* Configure PDx pins Output type to push-pull */
  GPIOC->OTYPER  |= 0x00000000;                         /* 赋值或初始化 */
  /* No pull-up, pull-down for PDx pins */
  GPIOC->PUPDR   |= 0x00000000;                         /* 赋值或初始化 */
#endif /* STM32F446xx */

  /* Connect PDx pins to FMC Alternate function */
  GPIOD->AFR[0]  = 0x000000CC;                          /* 赋值或初始化 */
  GPIOD->AFR[1]  = 0xCC000CCC;                          /* 赋值或初始化 */
  /* Configure PDx pins in Alternate function mode */  
  GPIOD->MODER   = 0xA02A000A;                          /* 赋值或初始化 */
  /* Configure PDx pins speed to 50 MHz */  
  GPIOD->OSPEEDR = 0xA02A000A;                          /* 赋值或初始化 */
  /* Configure PDx pins Output type to push-pull */  
  GPIOD->OTYPER  = 0x00000000;                          /* 赋值或初始化 */
  /* No pull-up, pull-down for PDx pins */ 
  GPIOD->PUPDR   = 0x00000000;                          /* 赋值或初始化 */

  /* Connect PEx pins to FMC Alternate function */
  GPIOE->AFR[0]  = 0xC00000CC;                          /* 赋值或初始化 */
  GPIOE->AFR[1]  = 0xCCCCCCCC;                          /* 赋值或初始化 */
  /* Configure PEx pins in Alternate function mode */ 
  GPIOE->MODER   = 0xAAAA800A;                          /* 赋值或初始化 */
  /* Configure PEx pins speed to 50 MHz */ 
  GPIOE->OSPEEDR = 0xAAAA800A;                          /* 赋值或初始化 */
  /* Configure PEx pins Output type to push-pull */  
  GPIOE->OTYPER  = 0x00000000;                          /* 赋值或初始化 */
  /* No pull-up, pull-down for PEx pins */ 
  GPIOE->PUPDR   = 0x00000000;                          /* 赋值或初始化 */

  /* Connect PFx pins to FMC Alternate function */
  GPIOF->AFR[0]  = 0xCCCCCCCC;                          /* 赋值或初始化 */
  GPIOF->AFR[1]  = 0xCCCCCCCC;                          /* 赋值或初始化 */
  /* Configure PFx pins in Alternate function mode */   
  GPIOF->MODER   = 0xAA800AAA;                          /* 赋值或初始化 */
  /* Configure PFx pins speed to 50 MHz */ 
  GPIOF->OSPEEDR = 0xAA800AAA;                          /* 赋值或初始化 */
  /* Configure PFx pins Output type to push-pull */  
  GPIOF->OTYPER  = 0x00000000;                          /* 赋值或初始化 */
  /* No pull-up, pull-down for PFx pins */ 
  GPIOF->PUPDR   = 0x00000000;                          /* 赋值或初始化 */

  /* Connect PGx pins to FMC Alternate function */
  GPIOG->AFR[0]  = 0xCCCCCCCC;                          /* 赋值或初始化 */
  GPIOG->AFR[1]  = 0xCCCCCCCC;                          /* 赋值或初始化 */
  /* Configure PGx pins in Alternate function mode */ 
  GPIOG->MODER   = 0xAAAAAAAA;                          /* 赋值或初始化 */
  /* Configure PGx pins speed to 50 MHz */ 
  GPIOG->OSPEEDR = 0xAAAAAAAA;                          /* 赋值或初始化 */
  /* Configure PGx pins Output type to push-pull */  
  GPIOG->OTYPER  = 0x00000000;                          /* 赋值或初始化 */
  /* No pull-up, pull-down for PGx pins */ 
  GPIOG->PUPDR   = 0x00000000;                          /* 赋值或初始化 */

#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx)\
 || defined(STM32F469xx) || defined(STM32F479xx)        /* 源代码 */
  /* Connect PHx pins to FMC Alternate function */
  GPIOH->AFR[0]  = 0x00C0CC00;                          /* 赋值或初始化 */
  GPIOH->AFR[1]  = 0xCCCCCCCC;                          /* 赋值或初始化 */
  /* Configure PHx pins in Alternate function mode */ 
  GPIOH->MODER   = 0xAAAA08A0;                          /* 赋值或初始化 */
  /* Configure PHx pins speed to 50 MHz */ 
  GPIOH->OSPEEDR = 0xAAAA08A0;                          /* 赋值或初始化 */
  /* Configure PHx pins Output type to push-pull */  
  GPIOH->OTYPER  = 0x00000000;                          /* 赋值或初始化 */
  /* No pull-up, pull-down for PHx pins */ 
  GPIOH->PUPDR   = 0x00000000;                          /* 赋值或初始化 */

  /* Connect PIx pins to FMC Alternate function */
  GPIOI->AFR[0]  = 0xCCCCCCCC;                          /* 赋值或初始化 */
  GPIOI->AFR[1]  = 0x00000CC0;                          /* 赋值或初始化 */
  /* Configure PIx pins in Alternate function mode */ 
  GPIOI->MODER   = 0x0028AAAA;                          /* 赋值或初始化 */
  /* Configure PIx pins speed to 50 MHz */ 
  GPIOI->OSPEEDR = 0x0028AAAA;                          /* 赋值或初始化 */
  /* Configure PIx pins Output type to push-pull */  
  GPIOI->OTYPER  = 0x00000000;                          /* 赋值或初始化 */
  /* No pull-up, pull-down for PIx pins */ 
  GPIOI->PUPDR   = 0x00000000;                          /* 赋值或初始化 */
#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx || STM32F469xx || STM32F479xx */

/*-- FMC Configuration -------------------------------------------------------*/
  /* Enable the FMC interface clock */
  RCC->AHB3ENR |= 0x00000001;                           /* 赋值或初始化 */
  /* Delay after an RCC peripheral clock enabling */
  tmp = READ_BIT(RCC->AHB3ENR, RCC_AHB3ENR_FMCEN);      /* 赋值或初始化 */

  /* Configure and enable SDRAM bank1 */
#if defined(STM32F446xx)                                /* 条件编译 */
  FMC_Bank5_6->SDCR[0] = 0x00001954;                    /* 赋值或初始化 */
#else                                                   /* 条件编译否则 */
  FMC_Bank5_6->SDCR[0] = 0x000019E4;                    /* 赋值或初始化 */
#endif /* STM32F446xx */
  FMC_Bank5_6->SDTR[0] = 0x01115351;                    /* 赋值或初始化 */

  /* SDRAM initialization sequence */
  /* Clock enable command */
  FMC_Bank5_6->SDCMR = 0x00000011;                      /* 赋值或初始化 */
  tmpreg = FMC_Bank5_6->SDSR & 0x00000020;              /* 赋值或初始化 */
  while((tmpreg != 0) && (timeout-- > 0))               /* while循环 */
  {                                                     /* 代码块开始 */
    tmpreg = FMC_Bank5_6->SDSR & 0x00000020;            /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Delay */
  for (index = 0; index<1000; index++);                 /* for循环 */

  /* PALL command */
  FMC_Bank5_6->SDCMR = 0x00000012;                      /* 赋值或初始化 */
  tmpreg = FMC_Bank5_6->SDSR & 0x00000020;              /* 赋值或初始化 */
  timeout = 0xFFFF;                                     /* 赋值或初始化 */
  while((tmpreg != 0) && (timeout-- > 0))               /* while循环 */
  {                                                     /* 代码块开始 */
    tmpreg = FMC_Bank5_6->SDSR & 0x00000020;            /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Auto refresh command */
#if defined(STM32F446xx)                                /* 条件编译 */
  FMC_Bank5_6->SDCMR = 0x000000F3;                      /* 赋值或初始化 */
#else                                                   /* 条件编译否则 */
  FMC_Bank5_6->SDCMR = 0x00000073;                      /* 赋值或初始化 */
#endif /* STM32F446xx */
  tmpreg = FMC_Bank5_6->SDSR & 0x00000020;              /* 赋值或初始化 */
  timeout = 0xFFFF;                                     /* 赋值或初始化 */
  while((tmpreg != 0) && (timeout-- > 0))               /* while循环 */
  {                                                     /* 代码块开始 */
    tmpreg = FMC_Bank5_6->SDSR & 0x00000020;            /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* MRD register program */
#if defined(STM32F446xx)                                /* 条件编译 */
  FMC_Bank5_6->SDCMR = 0x00044014;                      /* 赋值或初始化 */
#else                                                   /* 条件编译否则 */
  FMC_Bank5_6->SDCMR = 0x00046014;                      /* 赋值或初始化 */
#endif /* STM32F446xx */
  tmpreg = FMC_Bank5_6->SDSR & 0x00000020;              /* 赋值或初始化 */
  timeout = 0xFFFF;                                     /* 赋值或初始化 */
  while((tmpreg != 0) && (timeout-- > 0))               /* while循环 */
  {                                                     /* 代码块开始 */
    tmpreg = FMC_Bank5_6->SDSR & 0x00000020;            /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Set refresh count */
  tmpreg = FMC_Bank5_6->SDRTR;                          /* 赋值或初始化 */
#if defined(STM32F446xx)                                /* 条件编译 */
  FMC_Bank5_6->SDRTR = (tmpreg | (0x0000050C<<1));      /* 赋值或初始化 */
#else                                                   /* 条件编译否则 */
  FMC_Bank5_6->SDRTR = (tmpreg | (0x0000027C<<1));      /* 赋值或初始化 */
#endif /* STM32F446xx */

  /* Disable write protection */
  tmpreg = FMC_Bank5_6->SDCR[0];                        /* 赋值或初始化 */
  FMC_Bank5_6->SDCR[0] = (tmpreg & 0xFFFFFDFF);         /* 赋值或初始化 */
#endif /* DATA_IN_ExtSDRAM */
#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx || STM32F446xx || STM32F469xx || STM32F479xx */

#if defined(STM32F405xx) || defined(STM32F415xx) || defined(STM32F407xx) || defined(STM32F417xx)\
 || defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx)\
 || defined(STM32F469xx) || defined(STM32F479xx) || defined(STM32F412Zx) || defined(STM32F412Vx)  /* 源代码 */

#if defined(DATA_IN_ExtSRAM)                            /* 条件编译 */
/*-- GPIOs Configuration -----------------------------------------------------*/
   /* Enable GPIOD, GPIOE, GPIOF and GPIOG interface clock */
  RCC->AHB1ENR   |= 0x00000078;                         /* 赋值或初始化 */
  /* Delay after an RCC peripheral clock enabling */
  tmp = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN);    /* 赋值或初始化 */

  /* Connect PDx pins to FMC Alternate function */
  GPIOD->AFR[0]  = 0x00CCC0CC;                          /* 赋值或初始化 */
  GPIOD->AFR[1]  = 0xCCCCCCCC;                          /* 赋值或初始化 */
  /* Configure PDx pins in Alternate function mode */  
  GPIOD->MODER   = 0xAAAA0A8A;                          /* 赋值或初始化 */
  /* Configure PDx pins speed to 100 MHz */  
  GPIOD->OSPEEDR = 0xFFFF0FCF;                          /* 赋值或初始化 */
  /* Configure PDx pins Output type to push-pull */  
  GPIOD->OTYPER  = 0x00000000;                          /* 赋值或初始化 */
  /* No pull-up, pull-down for PDx pins */ 
  GPIOD->PUPDR   = 0x00000000;                          /* 赋值或初始化 */

  /* Connect PEx pins to FMC Alternate function */
  GPIOE->AFR[0]  = 0xC00CC0CC;                          /* 赋值或初始化 */
  GPIOE->AFR[1]  = 0xCCCCCCCC;                          /* 赋值或初始化 */
  /* Configure PEx pins in Alternate function mode */ 
  GPIOE->MODER   = 0xAAAA828A;                          /* 赋值或初始化 */
  /* Configure PEx pins speed to 100 MHz */ 
  GPIOE->OSPEEDR = 0xFFFFC3CF;                          /* 赋值或初始化 */
  /* Configure PEx pins Output type to push-pull */  
  GPIOE->OTYPER  = 0x00000000;                          /* 赋值或初始化 */
  /* No pull-up, pull-down for PEx pins */ 
  GPIOE->PUPDR   = 0x00000000;                          /* 赋值或初始化 */

  /* Connect PFx pins to FMC Alternate function */
  GPIOF->AFR[0]  = 0x00CCCCCC;                          /* 赋值或初始化 */
  GPIOF->AFR[1]  = 0xCCCC0000;                          /* 赋值或初始化 */
  /* Configure PFx pins in Alternate function mode */   
  GPIOF->MODER   = 0xAA000AAA;                          /* 赋值或初始化 */
  /* Configure PFx pins speed to 100 MHz */ 
  GPIOF->OSPEEDR = 0xFF000FFF;                          /* 赋值或初始化 */
  /* Configure PFx pins Output type to push-pull */  
  GPIOF->OTYPER  = 0x00000000;                          /* 赋值或初始化 */
  /* No pull-up, pull-down for PFx pins */ 
  GPIOF->PUPDR   = 0x00000000;                          /* 赋值或初始化 */

  /* Connect PGx pins to FMC Alternate function */
  GPIOG->AFR[0]  = 0x00CCCCCC;                          /* 赋值或初始化 */
  GPIOG->AFR[1]  = 0x000000C0;                          /* 赋值或初始化 */
  /* Configure PGx pins in Alternate function mode */ 
  GPIOG->MODER   = 0x00085AAA;                          /* 赋值或初始化 */
  /* Configure PGx pins speed to 100 MHz */ 
  GPIOG->OSPEEDR = 0x000CAFFF;                          /* 赋值或初始化 */
  /* Configure PGx pins Output type to push-pull */  
  GPIOG->OTYPER  = 0x00000000;                          /* 赋值或初始化 */
  /* No pull-up, pull-down for PGx pins */ 
  GPIOG->PUPDR   = 0x00000000;                          /* 赋值或初始化 */

/*-- FMC/FSMC Configuration --------------------------------------------------*/
  /* Enable the FMC/FSMC interface clock */
  RCC->AHB3ENR         |= 0x00000001;                   /* 赋值或初始化 */

#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx)  /* 条件编译 */
  /* Delay after an RCC peripheral clock enabling */
  tmp = READ_BIT(RCC->AHB3ENR, RCC_AHB3ENR_FMCEN);      /* 赋值或初始化 */
  /* Configure and enable Bank1_SRAM2 */
  FMC_Bank1->BTCR[2]  = 0x00001011;                     /* 赋值或初始化 */
  FMC_Bank1->BTCR[3]  = 0x00000201;                     /* 赋值或初始化 */
  FMC_Bank1E->BWTR[2] = 0x0fffffff;                     /* 赋值或初始化 */
#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx */ 
#if defined(STM32F469xx) || defined(STM32F479xx)        /* 条件编译 */
  /* Delay after an RCC peripheral clock enabling */
  tmp = READ_BIT(RCC->AHB3ENR, RCC_AHB3ENR_FMCEN);      /* 赋值或初始化 */
  /* Configure and enable Bank1_SRAM2 */
  FMC_Bank1->BTCR[2]  = 0x00001091;                     /* 赋值或初始化 */
  FMC_Bank1->BTCR[3]  = 0x00110212;                     /* 赋值或初始化 */
  FMC_Bank1E->BWTR[2] = 0x0fffffff;                     /* 赋值或初始化 */
#endif /* STM32F469xx || STM32F479xx */
#if defined(STM32F405xx) || defined(STM32F415xx) || defined(STM32F407xx)|| defined(STM32F417xx)\
   || defined(STM32F412Zx) || defined(STM32F412Vx)      /* 源代码 */
  /* Delay after an RCC peripheral clock enabling */
  tmp = READ_BIT(RCC->AHB3ENR, RCC_AHB3ENR_FSMCEN);     /* 赋值或初始化 */
  /* Configure and enable Bank1_SRAM2 */
  FSMC_Bank1->BTCR[2]  = 0x00001011;                    /* 赋值或初始化 */
  FSMC_Bank1->BTCR[3]  = 0x00000201;                    /* 赋值或初始化 */
  FSMC_Bank1E->BWTR[2] = 0x0FFFFFFF;                    /* 赋值或初始化 */
#endif /* STM32F405xx || STM32F415xx || STM32F407xx || STM32F417xx || STM32F412Zx || STM32F412Vx */

#endif /* DATA_IN_ExtSRAM */
#endif /* STM32F405xx || STM32F415xx || STM32F407xx || STM32F417xx || STM32F427xx || STM32F437xx ||\
          STM32F429xx || STM32F439xx || STM32F469xx || STM32F479xx || STM32F412Zx || STM32F412Vx  */ 
  (void)(tmp);                                          /* 执行语句 */
}                                                       /* 代码块结束 */
#endif /* DATA_IN_ExtSRAM && DATA_IN_ExtSDRAM */
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
