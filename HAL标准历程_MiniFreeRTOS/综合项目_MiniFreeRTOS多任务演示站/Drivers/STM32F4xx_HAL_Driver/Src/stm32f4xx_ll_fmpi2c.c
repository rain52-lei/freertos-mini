/* 文件: stm32f4xx_ll_fmpi2c.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_ll_fmpi2c.c
  * @author  MCD Application Team
  * @brief   FMPI2C LL module driver.
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

#if defined(FMPI2C_CR1_PE)                              /* 条件编译 */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_ll_fmpi2c.h"                        /* 包含 STM32/HAL 头文件 */
#include "stm32f4xx_ll_bus.h"                           /* 包含 STM32/HAL 头文件 */
#ifdef  USE_FULL_ASSERT                                 /* 条件编译 */
#include "stm32_assert.h"                               /* 包含 STM32/HAL 头文件 */
#else                                                   /* 条件编译否则 */
#define assert_param(expr) ((void)0U)                   /* 定义宏 */
#endif                                                  /* 条件编译结束 */

/** @addtogroup STM32F4xx_LL_Driver
  * @{
  */

#if defined (FMPI2C1)                                   /* 条件编译 */

/** @defgroup FMPI2C_LL FMPI2C
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/** @addtogroup FMPI2C_LL_Private_Macros
  * @{
  */

#define IS_LL_FMPI2C_PERIPHERAL_MODE(__VALUE__)    (((__VALUE__) == LL_FMPI2C_MODE_I2C)          || \
                                                 ((__VALUE__) == LL_FMPI2C_MODE_SMBUS_HOST)   || \
                                                 ((__VALUE__) == LL_FMPI2C_MODE_SMBUS_DEVICE) || \
                                                 ((__VALUE__) == LL_FMPI2C_MODE_SMBUS_DEVICE_ARP))  /* 源代码 */

#define IS_LL_FMPI2C_ANALOG_FILTER(__VALUE__)      (((__VALUE__) == LL_FMPI2C_ANALOGFILTER_ENABLE) || \
                                                 ((__VALUE__) == LL_FMPI2C_ANALOGFILTER_DISABLE))  /* 源代码 */

#define IS_LL_FMPI2C_DIGITAL_FILTER(__VALUE__)     ((__VALUE__) <= 0x0000000FU)  /* 定义宏 */

#define IS_LL_FMPI2C_OWN_ADDRESS1(__VALUE__)       ((__VALUE__) <= 0x000003FFU)  /* 定义宏 */

#define IS_LL_FMPI2C_TYPE_ACKNOWLEDGE(__VALUE__)   (((__VALUE__) == LL_FMPI2C_ACK) || \
                                                 ((__VALUE__) == LL_FMPI2C_NACK))  /* 源代码 */

#define IS_LL_FMPI2C_OWN_ADDRSIZE(__VALUE__)       (((__VALUE__) == LL_FMPI2C_OWNADDRESS1_7BIT) || \
                                                 ((__VALUE__) == LL_FMPI2C_OWNADDRESS1_10BIT))  /* 源代码 */
/**
  * @}
  */

/* Private function prototypes -----------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/** @addtogroup FMPI2C_LL_Exported_Functions
  * @{
  */

/** @addtogroup FMPI2C_LL_EF_Init
  * @{
  */

/**
  * @brief  De-initialize the FMPI2C registers to their default reset values.
  * @param  FMPI2Cx FMPI2C Instance.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: FMPI2C registers are de-initialized
  *          - ERROR: FMPI2C registers are not de-initialized
  */
ErrorStatus LL_FMPI2C_DeInit(FMPI2C_TypeDef *FMPI2Cx)   /* 源代码 */
{                                                       /* 代码块开始 */
  ErrorStatus status = SUCCESS;                         /* 赋值或初始化 */

  /* Check the FMPI2C Instance FMPI2Cx */
  assert_param(IS_FMPI2C_ALL_INSTANCE(FMPI2Cx));        /* 执行语句 */

  if (FMPI2Cx == FMPI2C1)                               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Force reset of FMPI2C clock */
    LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_FMPI2C1);  /* 执行语句 */

    /* Release reset of FMPI2C clock */
    LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_FMPI2C1);  /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    status = ERROR;                                     /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initialize the FMPI2C registers according to the specified parameters in FMPI2C_InitStruct.
  * @param  FMPI2Cx FMPI2C Instance.
  * @param  FMPI2C_InitStruct pointer to a @ref LL_FMPI2C_InitTypeDef structure.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: FMPI2C registers are initialized
  *          - ERROR: Not applicable
  */
ErrorStatus LL_FMPI2C_Init(FMPI2C_TypeDef *FMPI2Cx, LL_FMPI2C_InitTypeDef *FMPI2C_InitStruct)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Check the FMPI2C Instance FMPI2Cx */
  assert_param(IS_FMPI2C_ALL_INSTANCE(FMPI2Cx));        /* 执行语句 */

  /* Check the FMPI2C parameters from FMPI2C_InitStruct */
  assert_param(IS_LL_FMPI2C_PERIPHERAL_MODE(FMPI2C_InitStruct->PeripheralMode));  /* 执行语句 */
  assert_param(IS_LL_FMPI2C_ANALOG_FILTER(FMPI2C_InitStruct->AnalogFilter));  /* 执行语句 */
  assert_param(IS_LL_FMPI2C_DIGITAL_FILTER(FMPI2C_InitStruct->DigitalFilter));  /* 执行语句 */
  assert_param(IS_LL_FMPI2C_OWN_ADDRESS1(FMPI2C_InitStruct->OwnAddress1));  /* 执行语句 */
  assert_param(IS_LL_FMPI2C_TYPE_ACKNOWLEDGE(FMPI2C_InitStruct->TypeAcknowledge));  /* 执行语句 */
  assert_param(IS_LL_FMPI2C_OWN_ADDRSIZE(FMPI2C_InitStruct->OwnAddrSize));  /* 执行语句 */

  /* Disable the selected FMPI2Cx Peripheral */
  LL_FMPI2C_Disable(FMPI2Cx);                           /* 执行语句 */

  /*---------------------------- FMPI2Cx CR1 Configuration ------------------------
   * Configure the analog and digital noise filters with parameters :
   * - AnalogFilter: FMPI2C_CR1_ANFOFF bit
   * - DigitalFilter: FMPI2C_CR1_DNF[3:0] bits
   */
  LL_FMPI2C_ConfigFilters(FMPI2Cx, FMPI2C_InitStruct->AnalogFilter, FMPI2C_InitStruct->DigitalFilter);  /* 执行语句 */

  /*---------------------------- FMPI2Cx TIMINGR Configuration --------------------
   * Configure the SDA setup, hold time and the SCL high, low period with parameter :
   * - Timing: FMPI2C_TIMINGR_PRESC[3:0], FMPI2C_TIMINGR_SCLDEL[3:0], FMPI2C_TIMINGR_SDADEL[3:0],
   *           FMPI2C_TIMINGR_SCLH[7:0] and FMPI2C_TIMINGR_SCLL[7:0] bits
   */
  LL_FMPI2C_SetTiming(FMPI2Cx, FMPI2C_InitStruct->Timing);  /* 执行语句 */

  /* Enable the selected FMPI2Cx Peripheral */
  LL_FMPI2C_Enable(FMPI2Cx);                            /* 执行语句 */

  /*---------------------------- FMPI2Cx OAR1 Configuration -----------------------
   * Disable, Configure and Enable FMPI2Cx device own address 1 with parameters :
   * - OwnAddress1:  FMPI2C_OAR1_OA1[9:0] bits
   * - OwnAddrSize:  FMPI2C_OAR1_OA1MODE bit
   */
  LL_FMPI2C_DisableOwnAddress1(FMPI2Cx);                /* 执行语句 */
  LL_FMPI2C_SetOwnAddress1(FMPI2Cx, FMPI2C_InitStruct->OwnAddress1, FMPI2C_InitStruct->OwnAddrSize);  /* 执行语句 */

  /* OwnAdress1 == 0 is reserved for General Call address */
  if (FMPI2C_InitStruct->OwnAddress1 != 0U)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    LL_FMPI2C_EnableOwnAddress1(FMPI2Cx);               /* 执行语句 */
  }                                                     /* 代码块结束 */

  /*---------------------------- FMPI2Cx MODE Configuration -----------------------
  * Configure FMPI2Cx peripheral mode with parameter :
   * - PeripheralMode: FMPI2C_CR1_SMBDEN and FMPI2C_CR1_SMBHEN bits
   */
  LL_FMPI2C_SetMode(FMPI2Cx, FMPI2C_InitStruct->PeripheralMode);  /* 执行语句 */

  /*---------------------------- FMPI2Cx CR2 Configuration ------------------------
   * Configure the ACKnowledge or Non ACKnowledge condition
   * after the address receive match code or next received byte with parameter :
   * - TypeAcknowledge: FMPI2C_CR2_NACK bit
   */
  LL_FMPI2C_AcknowledgeNextData(FMPI2Cx, FMPI2C_InitStruct->TypeAcknowledge);  /* 执行语句 */

  return SUCCESS;                                       /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Set each @ref LL_FMPI2C_InitTypeDef field to default value.
  * @param  FMPI2C_InitStruct Pointer to a @ref LL_FMPI2C_InitTypeDef structure.
  * @retval None
  */
void LL_FMPI2C_StructInit(LL_FMPI2C_InitTypeDef *FMPI2C_InitStruct)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Set FMPI2C_InitStruct fields to default values */
  FMPI2C_InitStruct->PeripheralMode  = LL_FMPI2C_MODE_I2C;  /* 赋值或初始化 */
  FMPI2C_InitStruct->Timing          = 0U;              /* 赋值或初始化 */
  FMPI2C_InitStruct->AnalogFilter    = LL_FMPI2C_ANALOGFILTER_ENABLE;  /* 赋值或初始化 */
  FMPI2C_InitStruct->DigitalFilter   = 0U;              /* 赋值或初始化 */
  FMPI2C_InitStruct->OwnAddress1     = 0U;              /* 赋值或初始化 */
  FMPI2C_InitStruct->TypeAcknowledge = LL_FMPI2C_NACK;  /* 赋值或初始化 */
  FMPI2C_InitStruct->OwnAddrSize     = LL_FMPI2C_OWNADDRESS1_7BIT;  /* 赋值或初始化 */
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

#endif /* FMPI2C1 */

/**
  * @}
  */

#endif /* FMPI2C_CR1_PE */
#endif /* USE_FULL_LL_DRIVER */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
