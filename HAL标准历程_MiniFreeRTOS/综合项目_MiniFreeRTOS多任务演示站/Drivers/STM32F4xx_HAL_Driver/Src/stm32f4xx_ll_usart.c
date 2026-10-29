/* 文件: stm32f4xx_ll_usart.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_ll_usart.c
  * @author  MCD Application Team
  * @brief   USART LL module driver.
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
#include "stm32f4xx_ll_usart.h"                         /* 包含 STM32/HAL 头文件 */
#include "stm32f4xx_ll_rcc.h"                           /* 包含 STM32/HAL 头文件 */
#include "stm32f4xx_ll_bus.h"                           /* 包含 STM32/HAL 头文件 */
#ifdef  USE_FULL_ASSERT                                 /* 条件编译 */
#include "stm32_assert.h"                               /* 包含 STM32/HAL 头文件 */
#else                                                   /* 条件编译否则 */
#define assert_param(expr) ((void)0U)                   /* 定义宏 */
#endif                                                  /* 条件编译结束 */

/** @addtogroup STM32F4xx_LL_Driver
  * @{
  */

#if defined (USART1) || defined (USART2) || defined (USART3) || defined (USART6) || defined (UART4) || defined (UART5) || defined (UART7) || defined (UART8) || defined (UART9) || defined (UART10)  /* 条件编译 */

/** @addtogroup USART_LL
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/** @addtogroup USART_LL_Private_Constants
  * @{
  */

/**
  * @}
  */


/* Private macros ------------------------------------------------------------*/
/** @addtogroup USART_LL_Private_Macros
  * @{
  */

/* __BAUDRATE__ The maximum Baud Rate is derived from the maximum clock available
 *              divided by the smallest oversampling used on the USART (i.e. 8)    */
#define IS_LL_USART_BAUDRATE(__BAUDRATE__) ((__BAUDRATE__) <= 12500000U)  /* 定义宏 */

/* __VALUE__ In case of oversampling by 16 and 8, BRR content must be greater than or equal to 16d. */
#define IS_LL_USART_BRR_MIN(__VALUE__) ((__VALUE__) >= 16U)  /* 定义宏 */

#define IS_LL_USART_DIRECTION(__VALUE__) (((__VALUE__) == LL_USART_DIRECTION_NONE) \
                                       || ((__VALUE__) == LL_USART_DIRECTION_RX) \
                                       || ((__VALUE__) == LL_USART_DIRECTION_TX) \
                                       || ((__VALUE__) == LL_USART_DIRECTION_TX_RX))  /* 源代码 */

#define IS_LL_USART_PARITY(__VALUE__) (((__VALUE__) == LL_USART_PARITY_NONE) \
                                    || ((__VALUE__) == LL_USART_PARITY_EVEN) \
                                    || ((__VALUE__) == LL_USART_PARITY_ODD))  /* 源代码 */

#define IS_LL_USART_DATAWIDTH(__VALUE__) (((__VALUE__) == LL_USART_DATAWIDTH_8B) \
                                       || ((__VALUE__) == LL_USART_DATAWIDTH_9B))  /* 源代码 */

#define IS_LL_USART_OVERSAMPLING(__VALUE__) (((__VALUE__) == LL_USART_OVERSAMPLING_16) \
                                          || ((__VALUE__) == LL_USART_OVERSAMPLING_8))  /* 源代码 */

#define IS_LL_USART_LASTBITCLKOUTPUT(__VALUE__) (((__VALUE__) == LL_USART_LASTCLKPULSE_NO_OUTPUT) \
                                              || ((__VALUE__) == LL_USART_LASTCLKPULSE_OUTPUT))  /* 源代码 */

#define IS_LL_USART_CLOCKPHASE(__VALUE__) (((__VALUE__) == LL_USART_PHASE_1EDGE) \
                                        || ((__VALUE__) == LL_USART_PHASE_2EDGE))  /* 源代码 */

#define IS_LL_USART_CLOCKPOLARITY(__VALUE__) (((__VALUE__) == LL_USART_POLARITY_LOW) \
                                           || ((__VALUE__) == LL_USART_POLARITY_HIGH))  /* 源代码 */

#define IS_LL_USART_CLOCKOUTPUT(__VALUE__) (((__VALUE__) == LL_USART_CLOCK_DISABLE) \
                                         || ((__VALUE__) == LL_USART_CLOCK_ENABLE))  /* 源代码 */

#define IS_LL_USART_STOPBITS(__VALUE__) (((__VALUE__) == LL_USART_STOPBITS_0_5) \
                                      || ((__VALUE__) == LL_USART_STOPBITS_1) \
                                      || ((__VALUE__) == LL_USART_STOPBITS_1_5) \
                                      || ((__VALUE__) == LL_USART_STOPBITS_2))  /* 源代码 */

#define IS_LL_USART_HWCONTROL(__VALUE__) (((__VALUE__) == LL_USART_HWCONTROL_NONE) \
                                       || ((__VALUE__) == LL_USART_HWCONTROL_RTS) \
                                       || ((__VALUE__) == LL_USART_HWCONTROL_CTS) \
                                       || ((__VALUE__) == LL_USART_HWCONTROL_RTS_CTS))  /* 源代码 */

/**
  * @}
  */

/* Private function prototypes -----------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/** @addtogroup USART_LL_Exported_Functions
  * @{
  */

/** @addtogroup USART_LL_EF_Init
  * @{
  */

/**
  * @brief  De-initialize USART registers (Registers restored to their default values).
  * @param  USARTx USART Instance
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: USART registers are de-initialized
  *          - ERROR: USART registers are not de-initialized
  */
ErrorStatus LL_USART_DeInit(USART_TypeDef *USARTx)      /* 源代码 */
{                                                       /* 代码块开始 */
  ErrorStatus status = SUCCESS;                         /* 赋值或初始化 */

  /* Check the parameters */
  assert_param(IS_UART_INSTANCE(USARTx));               /* 执行语句 */

  if (USARTx == USART1)                                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Force reset of USART clock */
    LL_APB2_GRP1_ForceReset(LL_APB2_GRP1_PERIPH_USART1);  /* 执行语句 */

    /* Release reset of USART clock */
    LL_APB2_GRP1_ReleaseReset(LL_APB2_GRP1_PERIPH_USART1);  /* 执行语句 */
  }                                                     /* 代码块结束 */
  else if (USARTx == USART2)                            /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* Force reset of USART clock */
    LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_USART2);  /* 执行语句 */

    /* Release reset of USART clock */
    LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_USART2);  /* 执行语句 */
  }                                                     /* 代码块结束 */
#if defined(USART3)                                     /* 条件编译 */
  else if (USARTx == USART3)                            /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* Force reset of USART clock */
    LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_USART3);  /* 执行语句 */

    /* Release reset of USART clock */
    LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_USART3);  /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif /* USART3 */
#if defined(USART6)                                     /* 条件编译 */
  else if (USARTx == USART6)                            /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* Force reset of USART clock */
    LL_APB2_GRP1_ForceReset(LL_APB2_GRP1_PERIPH_USART6);  /* 执行语句 */

    /* Release reset of USART clock */
    LL_APB2_GRP1_ReleaseReset(LL_APB2_GRP1_PERIPH_USART6);  /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif /* USART6 */
#if defined(UART4)                                      /* 条件编译 */
  else if (USARTx == UART4)                             /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* Force reset of UART clock */
    LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_UART4);  /* 执行语句 */

    /* Release reset of UART clock */
    LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_UART4);  /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif /* UART4 */
#if defined(UART5)                                      /* 条件编译 */
  else if (USARTx == UART5)                             /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* Force reset of UART clock */
    LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_UART5);  /* 执行语句 */

    /* Release reset of UART clock */
    LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_UART5);  /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif /* UART5 */
#if defined(UART7)                                      /* 条件编译 */
  else if (USARTx == UART7)                             /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* Force reset of UART clock */
    LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_UART7);  /* 执行语句 */

    /* Release reset of UART clock */
    LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_UART7);  /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif /* UART7 */
#if defined(UART8)                                      /* 条件编译 */
  else if (USARTx == UART8)                             /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* Force reset of UART clock */
    LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_UART8);  /* 执行语句 */

    /* Release reset of UART clock */
    LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_UART8);  /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif /* UART8 */
#if defined(UART9)                                      /* 条件编译 */
  else if (USARTx == UART9)                             /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* Force reset of UART clock */
    LL_APB2_GRP1_ForceReset(LL_APB2_GRP1_PERIPH_UART9);  /* 执行语句 */

    /* Release reset of UART clock */
    LL_APB2_GRP1_ReleaseReset(LL_APB2_GRP1_PERIPH_UART9);  /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif /* UART9 */
#if defined(UART10)                                     /* 条件编译 */
  else if (USARTx == UART10)                            /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* Force reset of UART clock */
    LL_APB2_GRP1_ForceReset(LL_APB2_GRP1_PERIPH_UART10);  /* 执行语句 */

    /* Release reset of UART clock */
    LL_APB2_GRP1_ReleaseReset(LL_APB2_GRP1_PERIPH_UART10);  /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif /* UART10 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    status = ERROR;                                     /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return (status);                                      /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initialize USART registers according to the specified
  *         parameters in USART_InitStruct.
  * @note   As some bits in USART configuration registers can only be written when the USART is disabled (USART_CR1_UE bit =0),
  *         USART IP should be in disabled state prior calling this function. Otherwise, ERROR result will be returned.
  * @note   Baud rate value stored in USART_InitStruct BaudRate field, should be valid (different from 0).
  * @param  USARTx USART Instance
  * @param  USART_InitStruct pointer to a LL_USART_InitTypeDef structure
  *         that contains the configuration information for the specified USART peripheral.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: USART registers are initialized according to USART_InitStruct content
  *          - ERROR: Problem occurred during USART Registers initialization
  */
ErrorStatus LL_USART_Init(USART_TypeDef *USARTx, LL_USART_InitTypeDef *USART_InitStruct)  /* 源代码 */
{                                                       /* 代码块开始 */
  ErrorStatus status = ERROR;                           /* 赋值或初始化 */
  uint32_t periphclk = LL_RCC_PERIPH_FREQUENCY_NO;      /* 32位无符号变量/函数 */
  LL_RCC_ClocksTypeDef rcc_clocks;                      /* 执行语句 */

  /* Check the parameters */
  assert_param(IS_UART_INSTANCE(USARTx));               /* 执行语句 */
  assert_param(IS_LL_USART_BAUDRATE(USART_InitStruct->BaudRate));  /* 执行语句 */
  assert_param(IS_LL_USART_DATAWIDTH(USART_InitStruct->DataWidth));  /* 执行语句 */
  assert_param(IS_LL_USART_STOPBITS(USART_InitStruct->StopBits));  /* 执行语句 */
  assert_param(IS_LL_USART_PARITY(USART_InitStruct->Parity));  /* 执行语句 */
  assert_param(IS_LL_USART_DIRECTION(USART_InitStruct->TransferDirection));  /* 执行语句 */
  assert_param(IS_LL_USART_HWCONTROL(USART_InitStruct->HardwareFlowControl));  /* 执行语句 */
  assert_param(IS_LL_USART_OVERSAMPLING(USART_InitStruct->OverSampling));  /* 执行语句 */

  /* USART needs to be in disabled state, in order to be able to configure some bits in
     CRx registers */
  if (LL_USART_IsEnabled(USARTx) == 0U)                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    /*---------------------------- USART CR1 Configuration -----------------------
     * Configure USARTx CR1 (USART Word Length, Parity, Mode and Oversampling bits) with parameters:
     * - DataWidth:          USART_CR1_M bits according to USART_InitStruct->DataWidth value
     * - Parity:             USART_CR1_PCE, USART_CR1_PS bits according to USART_InitStruct->Parity value
     * - TransferDirection:  USART_CR1_TE, USART_CR1_RE bits according to USART_InitStruct->TransferDirection value
     * - Oversampling:       USART_CR1_OVER8 bit according to USART_InitStruct->OverSampling value.
     */
    MODIFY_REG(USARTx->CR1,                             /* 列表续行 */
               (USART_CR1_M | USART_CR1_PCE | USART_CR1_PS |  /* 源代码 */
                USART_CR1_TE | USART_CR1_RE | USART_CR1_OVER8),  /* 列表续行 */
               (USART_InitStruct->DataWidth | USART_InitStruct->Parity |  /* 源代码 */
                USART_InitStruct->TransferDirection | USART_InitStruct->OverSampling));  /* 执行语句 */

    /*---------------------------- USART CR2 Configuration -----------------------
     * Configure USARTx CR2 (Stop bits) with parameters:
     * - Stop Bits:          USART_CR2_STOP bits according to USART_InitStruct->StopBits value.
     * - CLKEN, CPOL, CPHA and LBCL bits are to be configured using LL_USART_ClockInit().
     */
    LL_USART_SetStopBitsLength(USARTx, USART_InitStruct->StopBits);  /* 执行语句 */

    /*---------------------------- USART CR3 Configuration -----------------------
     * Configure USARTx CR3 (Hardware Flow Control) with parameters:
     * - HardwareFlowControl: USART_CR3_RTSE, USART_CR3_CTSE bits according to USART_InitStruct->HardwareFlowControl value.
     */
    LL_USART_SetHWFlowCtrl(USARTx, USART_InitStruct->HardwareFlowControl);  /* 执行语句 */

    /*---------------------------- USART BRR Configuration -----------------------
     * Retrieve Clock frequency used for USART Peripheral
     */
    LL_RCC_GetSystemClocksFreq(&rcc_clocks);            /* 执行语句 */
    if (USARTx == USART1)                               /* 条件判断 */
    {                                                   /* 代码块开始 */
      periphclk = rcc_clocks.PCLK2_Frequency;           /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if (USARTx == USART2)                          /* 否则如果 */
    {                                                   /* 代码块开始 */
      periphclk = rcc_clocks.PCLK1_Frequency;           /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
#if defined(USART3)                                     /* 条件编译 */
    else if (USARTx == USART3)                          /* 否则如果 */
    {                                                   /* 代码块开始 */
      periphclk = rcc_clocks.PCLK1_Frequency;           /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
#endif /* USART3 */
#if defined(USART6)                                     /* 条件编译 */
    else if (USARTx == USART6)                          /* 否则如果 */
    {                                                   /* 代码块开始 */
      periphclk = rcc_clocks.PCLK2_Frequency;           /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
#endif /* USART6 */
#if defined(UART4)                                      /* 条件编译 */
    else if (USARTx == UART4)                           /* 否则如果 */
    {                                                   /* 代码块开始 */
      periphclk = rcc_clocks.PCLK1_Frequency;           /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
#endif /* UART4 */
#if defined(UART5)                                      /* 条件编译 */
    else if (USARTx == UART5)                           /* 否则如果 */
    {                                                   /* 代码块开始 */
      periphclk = rcc_clocks.PCLK1_Frequency;           /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
#endif /* UART5 */
#if defined(UART7)                                      /* 条件编译 */
    else if (USARTx == UART7)                           /* 否则如果 */
    {                                                   /* 代码块开始 */
      periphclk = rcc_clocks.PCLK1_Frequency;           /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
#endif /* UART7 */
#if defined(UART8)                                      /* 条件编译 */
    else if (USARTx == UART8)                           /* 否则如果 */
    {                                                   /* 代码块开始 */
      periphclk = rcc_clocks.PCLK1_Frequency;           /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
#endif /* UART8 */
#if defined(UART9)                                      /* 条件编译 */
    else if (USARTx == UART9)                           /* 否则如果 */
    {                                                   /* 代码块开始 */
      periphclk = rcc_clocks.PCLK2_Frequency;           /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
#endif /* UART9 */
#if defined(UART10)                                     /* 条件编译 */
    else if (USARTx == UART10)                          /* 否则如果 */
    {                                                   /* 代码块开始 */
      periphclk = rcc_clocks.PCLK2_Frequency;           /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
#endif /* UART10 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Nothing to do, as error code is already assigned to ERROR value */
    }                                                   /* 代码块结束 */

    /* Configure the USART Baud Rate :
       - valid baud rate value (different from 0) is required
       - Peripheral clock as returned by RCC service, should be valid (different from 0).
    */
    if ((periphclk != LL_RCC_PERIPH_FREQUENCY_NO)       /* 条件判断 */
        && (USART_InitStruct->BaudRate != 0U))          /* 源代码 */
    {                                                   /* 代码块开始 */
      status = SUCCESS;                                 /* 赋值或初始化 */
      LL_USART_SetBaudRate(USARTx,                      /* 列表续行 */
                           periphclk,                   /* 列表续行 */
                           USART_InitStruct->OverSampling,  /* 列表续行 */
                           USART_InitStruct->BaudRate);  /* 执行语句 */

      /* Check BRR is greater than or equal to 16d */
      assert_param(IS_LL_USART_BRR_MIN(USARTx->BRR));   /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /* Endif (=> USART not in Disabled state => return ERROR) */

  return (status);                                      /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief Set each @ref LL_USART_InitTypeDef field to default value.
  * @param USART_InitStruct Pointer to a @ref LL_USART_InitTypeDef structure
  *                         whose fields will be set to default values.
  * @retval None
  */

void LL_USART_StructInit(LL_USART_InitTypeDef *USART_InitStruct)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Set USART_InitStruct fields to default values */
  USART_InitStruct->BaudRate            = 9600U;        /* 赋值或初始化 */
  USART_InitStruct->DataWidth           = LL_USART_DATAWIDTH_8B;  /* 赋值或初始化 */
  USART_InitStruct->StopBits            = LL_USART_STOPBITS_1;  /* 赋值或初始化 */
  USART_InitStruct->Parity              = LL_USART_PARITY_NONE ;  /* 赋值或初始化 */
  USART_InitStruct->TransferDirection   = LL_USART_DIRECTION_TX_RX;  /* 赋值或初始化 */
  USART_InitStruct->HardwareFlowControl = LL_USART_HWCONTROL_NONE;  /* 赋值或初始化 */
  USART_InitStruct->OverSampling        = LL_USART_OVERSAMPLING_16;  /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initialize USART Clock related settings according to the
  *         specified parameters in the USART_ClockInitStruct.
  * @note   As some bits in USART configuration registers can only be written when the USART is disabled (USART_CR1_UE bit =0),
  *         USART IP should be in disabled state prior calling this function. Otherwise, ERROR result will be returned.
  * @param  USARTx USART Instance
  * @param  USART_ClockInitStruct Pointer to a @ref LL_USART_ClockInitTypeDef structure
  *         that contains the Clock configuration information for the specified USART peripheral.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: USART registers related to Clock settings are initialized according to USART_ClockInitStruct content
  *          - ERROR: Problem occurred during USART Registers initialization
  */
ErrorStatus LL_USART_ClockInit(USART_TypeDef *USARTx, LL_USART_ClockInitTypeDef *USART_ClockInitStruct)  /* 源代码 */
{                                                       /* 代码块开始 */
  ErrorStatus status = SUCCESS;                         /* 赋值或初始化 */

  /* Check USART Instance and Clock signal output parameters */
  assert_param(IS_UART_INSTANCE(USARTx));               /* 执行语句 */
  assert_param(IS_LL_USART_CLOCKOUTPUT(USART_ClockInitStruct->ClockOutput));  /* 执行语句 */

  /* USART needs to be in disabled state, in order to be able to configure some bits in
     CRx registers */
  if (LL_USART_IsEnabled(USARTx) == 0U)                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    /*---------------------------- USART CR2 Configuration -----------------------*/
    /* If Clock signal has to be output */
    if (USART_ClockInitStruct->ClockOutput == LL_USART_CLOCK_DISABLE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Deactivate Clock signal delivery :
       * - Disable Clock Output:        USART_CR2_CLKEN cleared
       */
      LL_USART_DisableSCLKOutput(USARTx);               /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Ensure USART instance is USART capable */
      assert_param(IS_USART_INSTANCE(USARTx));          /* 执行语句 */

      /* Check clock related parameters */
      assert_param(IS_LL_USART_CLOCKPOLARITY(USART_ClockInitStruct->ClockPolarity));  /* 执行语句 */
      assert_param(IS_LL_USART_CLOCKPHASE(USART_ClockInitStruct->ClockPhase));  /* 执行语句 */
      assert_param(IS_LL_USART_LASTBITCLKOUTPUT(USART_ClockInitStruct->LastBitClockPulse));  /* 执行语句 */

      /*---------------------------- USART CR2 Configuration -----------------------
       * Configure USARTx CR2 (Clock signal related bits) with parameters:
       * - Enable Clock Output:         USART_CR2_CLKEN set
       * - Clock Polarity:              USART_CR2_CPOL bit according to USART_ClockInitStruct->ClockPolarity value
       * - Clock Phase:                 USART_CR2_CPHA bit according to USART_ClockInitStruct->ClockPhase value
       * - Last Bit Clock Pulse Output: USART_CR2_LBCL bit according to USART_ClockInitStruct->LastBitClockPulse value.
       */
      MODIFY_REG(USARTx->CR2,                           /* 列表续行 */
                 USART_CR2_CLKEN | USART_CR2_CPHA | USART_CR2_CPOL | USART_CR2_LBCL,  /* 列表续行 */
                 USART_CR2_CLKEN | USART_ClockInitStruct->ClockPolarity |  /* 源代码 */
                 USART_ClockInitStruct->ClockPhase | USART_ClockInitStruct->LastBitClockPulse);  /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /* Else (USART not in Disabled state => return ERROR */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    status = ERROR;                                     /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return (status);                                      /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief Set each field of a @ref LL_USART_ClockInitTypeDef type structure to default value.
  * @param USART_ClockInitStruct Pointer to a @ref LL_USART_ClockInitTypeDef structure
  *                              whose fields will be set to default values.
  * @retval None
  */
void LL_USART_ClockStructInit(LL_USART_ClockInitTypeDef *USART_ClockInitStruct)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Set LL_USART_ClockInitStruct fields with default values */
  USART_ClockInitStruct->ClockOutput       = LL_USART_CLOCK_DISABLE;  /* 赋值或初始化 */
  USART_ClockInitStruct->ClockPolarity     = LL_USART_POLARITY_LOW;            /* Not relevant when ClockOutput = LL_USART_CLOCK_DISABLE */
  USART_ClockInitStruct->ClockPhase        = LL_USART_PHASE_1EDGE;             /* Not relevant when ClockOutput = LL_USART_CLOCK_DISABLE */
  USART_ClockInitStruct->LastBitClockPulse = LL_USART_LASTCLKPULSE_NO_OUTPUT;  /* Not relevant when ClockOutput = LL_USART_CLOCK_DISABLE */
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

#endif /* USART1 || USART2 || USART3 || USART6 || UART4 || UART5 || UART7 || UART8 || UART9 || UART10 */

/**
  * @}
  */

#endif /* USE_FULL_LL_DRIVER */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

