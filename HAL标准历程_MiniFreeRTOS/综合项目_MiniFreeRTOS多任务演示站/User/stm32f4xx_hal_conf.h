/* 文件: stm32f4xx_hal_conf.h | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_conf.h
  * @author  MCD Application Team
  * @brief   HAL configuration file
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4xx_HAL_CONF_H                          /* 防止头文件重复包含 */
#define __STM32F4xx_HAL_CONF_H                          /* 定义包含保护宏 */

#ifdef __cplusplus                                      /* 条件编译 */
 extern "C" {                                           /* 外部符号声明 */
#endif                                                  /* 条件编译结束 */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* ########################## Module Selection ############################## */
/**
  * @brief This is the list of modules to be used in the HAL driver 
  */
#define HAL_MODULE_ENABLED                              /* 定义宏 */
#define HAL_ADC_MODULE_ENABLED                          /* 定义宏 */
#define HAL_CAN_MODULE_ENABLED                          /* 定义宏 */
/* #define HAL_CAN_LEGACY_MODULE_ENABLED */
#define HAL_CRC_MODULE_ENABLED                          /* 定义宏 */
#define HAL_CRYP_MODULE_ENABLED                         /* 定义宏 */
#define HAL_DAC_MODULE_ENABLED                          /* 定义宏 */
#define HAL_DCMI_MODULE_ENABLED                         /* 定义宏 */
#define HAL_DMA_MODULE_ENABLED                          /* 定义宏 */
/* #define HAL_DMA2D_MODULE_ENABLED */
/* #define HAL_ETH_MODULE_ENABLED */
#define HAL_EXTI_MODULE_ENABLED                         /* 定义宏 */
#define HAL_FLASH_MODULE_ENABLED                        /* 定义宏 */
#define HAL_NAND_MODULE_ENABLED                         /* 定义宏 */
#define HAL_NOR_MODULE_ENABLED                          /* 定义宏 */
#define HAL_PCCARD_MODULE_ENABLED                       /* 定义宏 */
#define HAL_SRAM_MODULE_ENABLED                         /* 定义宏 */
/* #define HAL_SDRAM_MODULE_ENABLED */
#define HAL_HASH_MODULE_ENABLED                         /* 定义宏 */
#define HAL_GPIO_MODULE_ENABLED                         /* 定义宏 */
#define HAL_I2C_MODULE_ENABLED                          /* 定义宏 */
#define HAL_I2S_MODULE_ENABLED                          /* 定义宏 */
#define HAL_IWDG_MODULE_ENABLED                         /* 定义宏 */
/* #define HAL_LTDC_MODULE_ENABLED */
#define HAL_PWR_MODULE_ENABLED                          /* 定义宏 */
#define HAL_RCC_MODULE_ENABLED                          /* 定义宏 */
#define HAL_RNG_MODULE_ENABLED                          /* 定义宏 */
#define HAL_RTC_MODULE_ENABLED                          /* 定义宏 */
/* #define HAL_SAI_MODULE_ENABLED */
#define HAL_SD_MODULE_ENABLED                           /* 定义宏 */
#define HAL_SPI_MODULE_ENABLED                          /* 定义宏 */
#define HAL_TIM_MODULE_ENABLED                          /* 定义宏 */
#define HAL_UART_MODULE_ENABLED                         /* 定义宏 */
#define HAL_USART_MODULE_ENABLED                        /* 定义宏 */
#define HAL_IRDA_MODULE_ENABLED                         /* 定义宏 */
#define HAL_SMARTCARD_MODULE_ENABLED                    /* 定义宏 */
#define HAL_WWDG_MODULE_ENABLED                         /* 定义宏 */
#define HAL_CORTEX_MODULE_ENABLED                       /* 定义宏 */
#define HAL_PCD_MODULE_ENABLED                          /* 定义宏 */
#define HAL_HCD_MODULE_ENABLED                          /* 定义宏 */


/* ########################## HSE/HSI Values adaptation ##################### */
/**
  * @brief Adjust the value of External High Speed oscillator (HSE) used in your application.
  *        This value is used by the RCC HAL module to compute the system frequency
  *        (when HSE is used as system clock source, directly or through the PLL).  
  */
#if !defined  (HSE_VALUE)                               /* 条件编译 */
  #define HSE_VALUE    (8000000U)                       /* 定义宏 */
#endif                                                  /* 条件编译结束 */

#if !defined  (HSE_STARTUP_TIMEOUT)                     /* 条件编译 */
  #define HSE_STARTUP_TIMEOUT    (100U)                 /* 定义宏 */
#endif                                                  /* 条件编译结束 */

/**
  * @brief Internal High Speed oscillator (HSI) value.
  *        This value is used by the RCC HAL module to compute the system frequency
  *        (when HSI is used as system clock source, directly or through the PLL). 
  */
#if !defined  (HSI_VALUE)                               /* 条件编译 */
  #define HSI_VALUE    (16000000U)                      /* 定义宏 */
#endif                                                  /* 条件编译结束 */

/**
  * @brief Internal Low Speed oscillator (LSI) value.
  */
#if !defined  (LSI_VALUE)                               /* 条件编译 */
 #define LSI_VALUE  (32000U)                            /* 定义宏 */
#endif /* LSI_VALUE */                      /*!< Value of the Internal Low Speed oscillator in Hz
                                             The real value may vary depending on the variations
                                             in voltage and temperature.  */
/**
  * @brief External Low Speed oscillator (LSE) value.
  */
#if !defined  (LSE_VALUE)                               /* 条件编译 */
 #define LSE_VALUE  (32768U)                            /* 定义宏 */
#endif                                                  /* 条件编译结束 */

#if !defined  (LSE_STARTUP_TIMEOUT)                     /* 条件编译 */
  #define LSE_STARTUP_TIMEOUT    (5000U)                /* 定义宏 */
#endif                                                  /* 条件编译结束 */

/**
  * @brief External clock source for I2S peripheral
  *        This value is used by the I2S HAL module to compute the I2S clock source 
  *        frequency, this source is inserted directly through I2S_CKIN pad. 
  */
#if !defined  (EXTERNAL_CLOCK_VALUE)                    /* 条件编译 */
  #define EXTERNAL_CLOCK_VALUE    (12288000U)           /* 定义宏 */
#endif                                                  /* 条件编译结束 */

/* Tip: To avoid modifying this file each time you need to use different HSE,
   ===  you can define the HSE value in your toolchain compiler preprocessor. */

/* ########################### System Configuration ######################### */
/**
  * @brief This is the HAL system configuration section
  */     
#define  VDD_VALUE                    (3300U)           /* 定义宏 */
#define  TICK_INT_PRIORITY            (0x0FU)           /* 定义宏 */
#define  USE_RTOS                     0U                /* 定义宏 */
#define  PREFETCH_ENABLE              0U /* The prefetch will be enabled in SystemClock_Config(), depending on the used 
                                            STM32F405/415/07/417 device: RevA (prefetch must be off) or RevZ (prefetch can be on/off) */
#define  INSTRUCTION_CACHE_ENABLE     1U                /* 定义宏 */
#define  DATA_CACHE_ENABLE            1U                /* 定义宏 */

#define  USE_HAL_ADC_REGISTER_CALLBACKS         0U      /* 定义宏 */
#define  USE_HAL_CAN_REGISTER_CALLBACKS         0U      /* 定义宏 */
#define  USE_HAL_CEC_REGISTER_CALLBACKS         0U      /* 定义宏 */
#define  USE_HAL_CRYP_REGISTER_CALLBACKS        0U      /* 定义宏 */
#define  USE_HAL_DAC_REGISTER_CALLBACKS         0U      /* 定义宏 */
#define  USE_HAL_DCMI_REGISTER_CALLBACKS        0U      /* 定义宏 */
#define  USE_HAL_DFSDM_REGISTER_CALLBACKS       0U      /* 定义宏 */
#define  USE_HAL_DMA2D_REGISTER_CALLBACKS       0U      /* 定义宏 */
#define  USE_HAL_DSI_REGISTER_CALLBACKS         0U      /* 定义宏 */
#define  USE_HAL_ETH_REGISTER_CALLBACKS         0U      /* 定义宏 */
#define  USE_HAL_HASH_REGISTER_CALLBACKS        0U      /* 定义宏 */
#define  USE_HAL_HCD_REGISTER_CALLBACKS         0U      /* 定义宏 */
#define  USE_HAL_I2C_REGISTER_CALLBACKS         0U      /* 定义宏 */
#define  USE_HAL_FMPI2C_REGISTER_CALLBACKS      0U      /* 定义宏 */
#define  USE_HAL_I2S_REGISTER_CALLBACKS         0U      /* 定义宏 */
#define  USE_HAL_IRDA_REGISTER_CALLBACKS        0U      /* 定义宏 */
#define  USE_HAL_LPTIM_REGISTER_CALLBACKS       0U      /* 定义宏 */
#define  USE_HAL_LTDC_REGISTER_CALLBACKS        0U      /* 定义宏 */
#define  USE_HAL_MMC_REGISTER_CALLBACKS         0U      /* 定义宏 */
#define  USE_HAL_NAND_REGISTER_CALLBACKS        0U      /* 定义宏 */
#define  USE_HAL_NOR_REGISTER_CALLBACKS         0U      /* 定义宏 */
#define  USE_HAL_PCCARD_REGISTER_CALLBACKS      0U      /* 定义宏 */
#define  USE_HAL_PCD_REGISTER_CALLBACKS         0U      /* 定义宏 */
#define  USE_HAL_QSPI_REGISTER_CALLBACKS        0U      /* 定义宏 */
#define  USE_HAL_RNG_REGISTER_CALLBACKS         0U      /* 定义宏 */
#define  USE_HAL_RTC_REGISTER_CALLBACKS         0U      /* 定义宏 */
#define  USE_HAL_SAI_REGISTER_CALLBACKS         0U      /* 定义宏 */
#define  USE_HAL_SD_REGISTER_CALLBACKS          0U      /* 定义宏 */
#define  USE_HAL_SMARTCARD_REGISTER_CALLBACKS   0U      /* 定义宏 */
#define  USE_HAL_SDRAM_REGISTER_CALLBACKS       0U      /* 定义宏 */
#define  USE_HAL_SRAM_REGISTER_CALLBACKS        0U      /* 定义宏 */
#define  USE_HAL_SPDIFRX_REGISTER_CALLBACKS     0U      /* 定义宏 */
#define  USE_HAL_SMBUS_REGISTER_CALLBACKS       0U      /* 定义宏 */
#define  USE_HAL_SPI_REGISTER_CALLBACKS         0U      /* 定义宏 */
#define  USE_HAL_TIM_REGISTER_CALLBACKS         0U      /* 定义宏 */
#define  USE_HAL_UART_REGISTER_CALLBACKS        0U      /* 定义宏 */
#define  USE_HAL_USART_REGISTER_CALLBACKS       0U      /* 定义宏 */
#define  USE_HAL_WWDG_REGISTER_CALLBACKS        0U      /* 定义宏 */

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* ################## SPI peripheral configuration ########################## */

/* CRC FEATURE: Use to activate CRC feature inside HAL SPI Driver
* Activated: CRC code is present inside driver
* Deactivated: CRC code cleaned from driver
*/

#define USE_SPI_CRC                     1U              /* 定义宏 */

/* Includes ------------------------------------------------------------------*/
/**
  * @brief Include module's header file 
  */

#ifdef HAL_RCC_MODULE_ENABLED                           /* 条件编译 */
  #include "stm32f4xx_hal_rcc.h"                        /* 包含 STM32/HAL 头文件 */
#endif                                                  /* 条件编译结束 */

#ifdef HAL_EXTI_MODULE_ENABLED                          /* 条件编译 */
  #include "stm32f4xx_hal_exti.h"                       /* 包含 STM32/HAL 头文件 */
#endif                                                  /* 条件编译结束 */

#ifdef HAL_GPIO_MODULE_ENABLED                          /* 条件编译 */
  #include "stm32f4xx_hal_gpio.h"                       /* 包含 STM32/HAL 头文件 */
#endif                                                  /* 条件编译结束 */

#ifdef HAL_DMA_MODULE_ENABLED                           /* 条件编译 */
  #include "stm32f4xx_hal_dma.h"                        /* 包含 STM32/HAL 头文件 */
#endif                                                  /* 条件编译结束 */

#ifdef HAL_CORTEX_MODULE_ENABLED                        /* 条件编译 */
  #include "stm32f4xx_hal_cortex.h"
#endif                                                  /* 条件编译结束 */

#ifdef HAL_ADC_MODULE_ENABLED                           /* 条件编译 */
  #include "stm32f4xx_hal_adc.h"                        /* 包含 STM32/HAL 头文件 */
#endif                                                  /* 条件编译结束 */

#ifdef HAL_CAN_MODULE_ENABLED                           /* 条件编译 */
  #include "stm32f4xx_hal_can.h"                        /* 包含 STM32/HAL 头文件 */
#endif                                                  /* 条件编译结束 */

#ifdef HAL_CAN_LEGACY_MODULE_ENABLED                    /* 条件编译 */
  #include "stm32f4xx_hal_can_legacy.h"
#endif                                                  /* 条件编译结束 */

#ifdef HAL_CRC_MODULE_ENABLED                           /* 条件编译 */
  #include "stm32f4xx_hal_crc.h"                        /* 包含 STM32/HAL 头文件 */
#endif                                                  /* 条件编译结束 */

#ifdef HAL_CRYP_MODULE_ENABLED                          /* 条件编译 */
  #include "stm32f4xx_hal_cryp.h"                       /* 包含 STM32/HAL 头文件 */
#endif                                                  /* 条件编译结束 */

#ifdef HAL_DMA2D_MODULE_ENABLED                         /* 条件编译 */
  #include "stm32f4xx_hal_dma2d.h"                      /* 包含 STM32/HAL 头文件 */
#endif                                                  /* 条件编译结束 */

#ifdef HAL_DAC_MODULE_ENABLED                           /* 条件编译 */
  #include "stm32f4xx_hal_dac.h"                        /* 包含 STM32/HAL 头文件 */
#endif                                                  /* 条件编译结束 */

#ifdef HAL_DCMI_MODULE_ENABLED                          /* 条件编译 */
  #include "stm32f4xx_hal_dcmi.h"                       /* 包含 STM32/HAL 头文件 */
#endif                                                  /* 条件编译结束 */

#ifdef HAL_ETH_MODULE_ENABLED                           /* 条件编译 */
  #include "stm32f4xx_hal_eth.h"                        /* 包含 STM32/HAL 头文件 */
#endif                                                  /* 条件编译结束 */

#ifdef HAL_FLASH_MODULE_ENABLED                         /* 条件编译 */
  #include "stm32f4xx_hal_flash.h"                      /* 包含 STM32/HAL 头文件 */
#endif                                                  /* 条件编译结束 */

#ifdef HAL_SRAM_MODULE_ENABLED                          /* 条件编译 */
  #include "stm32f4xx_hal_sram.h"                       /* 包含 STM32/HAL 头文件 */
#endif                                                  /* 条件编译结束 */

#ifdef HAL_NOR_MODULE_ENABLED                           /* 条件编译 */
  #include "stm32f4xx_hal_nor.h"                        /* 包含 STM32/HAL 头文件 */
#endif                                                  /* 条件编译结束 */

#ifdef HAL_NAND_MODULE_ENABLED                          /* 条件编译 */
  #include "stm32f4xx_hal_nand.h"                       /* 包含 STM32/HAL 头文件 */
#endif                                                  /* 条件编译结束 */

#ifdef HAL_PCCARD_MODULE_ENABLED                        /* 条件编译 */
  #include "stm32f4xx_hal_pccard.h"
#endif                                                  /* 条件编译结束 */

#ifdef HAL_SDRAM_MODULE_ENABLED                         /* 条件编译 */
  #include "stm32f4xx_hal_sdram.h"                      /* 包含 STM32/HAL 头文件 */
#endif                                                  /* 条件编译结束 */

#ifdef HAL_HASH_MODULE_ENABLED                          /* 条件编译 */
 #include "stm32f4xx_hal_hash.h"                        /* 包含 STM32/HAL 头文件 */
#endif                                                  /* 条件编译结束 */

#ifdef HAL_I2C_MODULE_ENABLED                           /* 条件编译 */
 #include "stm32f4xx_hal_i2c.h"                         /* 包含 STM32/HAL 头文件 */
#endif                                                  /* 条件编译结束 */

#ifdef HAL_I2S_MODULE_ENABLED                           /* 条件编译 */
 #include "stm32f4xx_hal_i2s.h"                         /* 包含 STM32/HAL 头文件 */
#endif                                                  /* 条件编译结束 */

#ifdef HAL_IWDG_MODULE_ENABLED                          /* 条件编译 */
 #include "stm32f4xx_hal_iwdg.h"                        /* 包含 STM32/HAL 头文件 */
#endif                                                  /* 条件编译结束 */

#ifdef HAL_LTDC_MODULE_ENABLED                          /* 条件编译 */
 #include "stm32f4xx_hal_ltdc.h"                        /* 包含 STM32/HAL 头文件 */
#endif                                                  /* 条件编译结束 */

#ifdef HAL_PWR_MODULE_ENABLED                           /* 条件编译 */
 #include "stm32f4xx_hal_pwr.h"                         /* 包含 STM32/HAL 头文件 */
#endif                                                  /* 条件编译结束 */

#ifdef HAL_RNG_MODULE_ENABLED                           /* 条件编译 */
 #include "stm32f4xx_hal_rng.h"                         /* 包含 STM32/HAL 头文件 */
#endif                                                  /* 条件编译结束 */

#ifdef HAL_RTC_MODULE_ENABLED                           /* 条件编译 */
 #include "stm32f4xx_hal_rtc.h"                         /* 包含 STM32/HAL 头文件 */
#endif                                                  /* 条件编译结束 */

#ifdef HAL_SAI_MODULE_ENABLED                           /* 条件编译 */
 #include "stm32f4xx_hal_sai.h"                         /* 包含 STM32/HAL 头文件 */
#endif                                                  /* 条件编译结束 */

#ifdef HAL_SD_MODULE_ENABLED                            /* 条件编译 */
 #include "stm32f4xx_hal_sd.h"                          /* 包含 STM32/HAL 头文件 */
#endif                                                  /* 条件编译结束 */

#ifdef HAL_SPI_MODULE_ENABLED                           /* 条件编译 */
 #include "stm32f4xx_hal_spi.h"                         /* 包含 STM32/HAL 头文件 */
#endif                                                  /* 条件编译结束 */

#ifdef HAL_TIM_MODULE_ENABLED                           /* 条件编译 */
 #include "stm32f4xx_hal_tim.h"                         /* 包含 STM32/HAL 头文件 */
#endif                                                  /* 条件编译结束 */

#ifdef HAL_UART_MODULE_ENABLED                          /* 条件编译 */
 #include "stm32f4xx_hal_uart.h"                        /* 包含 STM32/HAL 头文件 */
#endif                                                  /* 条件编译结束 */

#ifdef HAL_USART_MODULE_ENABLED                         /* 条件编译 */
 #include "stm32f4xx_hal_usart.h"                       /* 包含 STM32/HAL 头文件 */
#endif                                                  /* 条件编译结束 */

#ifdef HAL_IRDA_MODULE_ENABLED                          /* 条件编译 */
 #include "stm32f4xx_hal_irda.h"                        /* 包含 STM32/HAL 头文件 */
#endif                                                  /* 条件编译结束 */

#ifdef HAL_SMARTCARD_MODULE_ENABLED                     /* 条件编译 */
 #include "stm32f4xx_hal_smartcard.h"
#endif                                                  /* 条件编译结束 */

#ifdef HAL_WWDG_MODULE_ENABLED                          /* 条件编译 */
 #include "stm32f4xx_hal_wwdg.h"                        /* 包含 STM32/HAL 头文件 */
#endif                                                  /* 条件编译结束 */

#ifdef HAL_PCD_MODULE_ENABLED                           /* 条件编译 */
 #include "stm32f4xx_hal_pcd.h"                         /* 包含 STM32/HAL 头文件 */
#endif                                                  /* 条件编译结束 */

#ifdef HAL_HCD_MODULE_ENABLED                           /* 条件编译 */
 #include "stm32f4xx_hal_hcd.h"                         /* 包含 STM32/HAL 头文件 */
#endif                                                  /* 条件编译结束 */

/* Exported macro ------------------------------------------------------------*/
#ifdef  USE_FULL_ASSERT                                 /* 条件编译 */
/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function
  *         which reports the name of the source file and the source
  *         line number of the call that failed. 
  *         If expr is true, it returns no value.
  * @retval None
  */
  #define assert_param(expr) ((expr) ? (void)0U : assert_failed((uint8_t *)__FILE__, __LINE__))  /* 定义宏 */
/* Exported functions ------------------------------------------------------- */
  void assert_failed(uint8_t* file, uint32_t line);     /* 函数定义或声明 */
#else                                                   /* 条件编译否则 */
  #define assert_param(expr) ((void)0U)                 /* 定义宏 */
#endif                                                  /* 条件编译结束 */


#ifdef __cplusplus                                      /* 条件编译 */
}                                                       /* 代码块结束 */
#endif                                                  /* 条件编译结束 */

#endif                                                  /* 条件编译结束 */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
