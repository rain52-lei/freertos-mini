/* 文件: stm32f4xx_ll_dma.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_ll_dma.c
  * @author  MCD Application Team
  * @brief   DMA LL module driver.
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
#include "stm32f4xx_ll_dma.h"                           /* 包含 STM32/HAL 头文件 */
#include "stm32f4xx_ll_bus.h"                           /* 包含 STM32/HAL 头文件 */
#ifdef  USE_FULL_ASSERT                                 /* 条件编译 */
#include "stm32_assert.h"                               /* 包含 STM32/HAL 头文件 */
#else                                                   /* 条件编译否则 */
#define assert_param(expr) ((void)0U)                   /* 定义宏 */
#endif                                                  /* 条件编译结束 */

/** @addtogroup STM32F4xx_LL_Driver
  * @{
  */

#if defined (DMA1) || defined (DMA2)                    /* 条件编译 */

/** @defgroup DMA_LL DMA
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/** @addtogroup DMA_LL_Private_Macros
  * @{
  */
#define IS_LL_DMA_DIRECTION(__VALUE__)          (((__VALUE__) == LL_DMA_DIRECTION_PERIPH_TO_MEMORY) || \
                                                 ((__VALUE__) == LL_DMA_DIRECTION_MEMORY_TO_PERIPH) || \
                                                 ((__VALUE__) == LL_DMA_DIRECTION_MEMORY_TO_MEMORY))  /* 源代码 */

#define IS_LL_DMA_MODE(__VALUE__)               (((__VALUE__) == LL_DMA_MODE_NORMAL)    || \
                                                 ((__VALUE__) == LL_DMA_MODE_CIRCULAR)  || \
                                                 ((__VALUE__) == LL_DMA_MODE_PFCTRL))  /* 源代码 */

#define IS_LL_DMA_PERIPHINCMODE(__VALUE__)      (((__VALUE__) == LL_DMA_PERIPH_INCREMENT) || \
                                                 ((__VALUE__) == LL_DMA_PERIPH_NOINCREMENT))  /* 源代码 */

#define IS_LL_DMA_MEMORYINCMODE(__VALUE__)      (((__VALUE__) == LL_DMA_MEMORY_INCREMENT) || \
                                                 ((__VALUE__) == LL_DMA_MEMORY_NOINCREMENT))  /* 源代码 */

#define IS_LL_DMA_PERIPHDATASIZE(__VALUE__)     (((__VALUE__) == LL_DMA_PDATAALIGN_BYTE)      || \
                                                 ((__VALUE__) == LL_DMA_PDATAALIGN_HALFWORD)  || \
                                                 ((__VALUE__) == LL_DMA_PDATAALIGN_WORD))  /* 源代码 */

#define IS_LL_DMA_MEMORYDATASIZE(__VALUE__)     (((__VALUE__) == LL_DMA_MDATAALIGN_BYTE)      || \
                                                 ((__VALUE__) == LL_DMA_MDATAALIGN_HALFWORD)  || \
                                                 ((__VALUE__) == LL_DMA_MDATAALIGN_WORD))  /* 源代码 */

#define IS_LL_DMA_NBDATA(__VALUE__)             ((__VALUE__)  <= 0x0000FFFFU)  /* 定义宏 */

#define IS_LL_DMA_CHANNEL(__VALUE__)            (((__VALUE__) == LL_DMA_CHANNEL_0)  || \
                                                 ((__VALUE__) == LL_DMA_CHANNEL_1)  || \
                                                 ((__VALUE__) == LL_DMA_CHANNEL_2)  || \
                                                 ((__VALUE__) == LL_DMA_CHANNEL_3)  || \
                                                 ((__VALUE__) == LL_DMA_CHANNEL_4)  || \
                                                 ((__VALUE__) == LL_DMA_CHANNEL_5)  || \
                                                 ((__VALUE__) == LL_DMA_CHANNEL_6)  || \
                                                 ((__VALUE__) == LL_DMA_CHANNEL_7))  /* 源代码 */

#define IS_LL_DMA_PRIORITY(__VALUE__)           (((__VALUE__) == LL_DMA_PRIORITY_LOW)    || \
                                                 ((__VALUE__) == LL_DMA_PRIORITY_MEDIUM) || \
                                                 ((__VALUE__) == LL_DMA_PRIORITY_HIGH)   || \
                                                 ((__VALUE__) == LL_DMA_PRIORITY_VERYHIGH))  /* 源代码 */

#define IS_LL_DMA_ALL_STREAM_INSTANCE(INSTANCE, STREAM)   ((((INSTANCE) == DMA1) && \
                                                           (((STREAM) == LL_DMA_STREAM_0) || \
                                                            ((STREAM) == LL_DMA_STREAM_1) || \
                                                            ((STREAM) == LL_DMA_STREAM_2) || \
                                                            ((STREAM) == LL_DMA_STREAM_3) || \
                                                            ((STREAM) == LL_DMA_STREAM_4) || \
                                                            ((STREAM) == LL_DMA_STREAM_5) || \
                                                            ((STREAM) == LL_DMA_STREAM_6) || \
                                                            ((STREAM) == LL_DMA_STREAM_7) || \
                                                            ((STREAM) == LL_DMA_STREAM_ALL))) ||\
                                                            (((INSTANCE) == DMA2) && \
                                                          (((STREAM) == LL_DMA_STREAM_0) || \
                                                           ((STREAM) == LL_DMA_STREAM_1) || \
                                                           ((STREAM) == LL_DMA_STREAM_2) || \
                                                           ((STREAM) == LL_DMA_STREAM_3) || \
                                                           ((STREAM) == LL_DMA_STREAM_4) || \
                                                           ((STREAM) == LL_DMA_STREAM_5) || \
                                                           ((STREAM) == LL_DMA_STREAM_6) || \
                                                           ((STREAM) == LL_DMA_STREAM_7) || \
                                                           ((STREAM) == LL_DMA_STREAM_ALL))))  /* 源代码 */

#define IS_LL_DMA_FIFO_MODE_STATE(STATE) (((STATE) == LL_DMA_FIFOMODE_DISABLE ) || \
                                          ((STATE) == LL_DMA_FIFOMODE_ENABLE))  /* 源代码 */

#define IS_LL_DMA_FIFO_THRESHOLD(THRESHOLD) (((THRESHOLD) == LL_DMA_FIFOTHRESHOLD_1_4) || \
                                             ((THRESHOLD) == LL_DMA_FIFOTHRESHOLD_1_2)  || \
                                             ((THRESHOLD) == LL_DMA_FIFOTHRESHOLD_3_4)  || \
                                             ((THRESHOLD) == LL_DMA_FIFOTHRESHOLD_FULL))  /* 源代码 */

#define IS_LL_DMA_MEMORY_BURST(BURST) (((BURST) == LL_DMA_MBURST_SINGLE) || \
                                       ((BURST) == LL_DMA_MBURST_INC4)   || \
                                       ((BURST) == LL_DMA_MBURST_INC8)   || \
                                       ((BURST) == LL_DMA_MBURST_INC16))  /* 源代码 */

#define IS_LL_DMA_PERIPHERAL_BURST(BURST) (((BURST) == LL_DMA_PBURST_SINGLE) || \
                                           ((BURST) == LL_DMA_PBURST_INC4)   || \
                                           ((BURST) == LL_DMA_PBURST_INC8)   || \
                                           ((BURST) == LL_DMA_PBURST_INC16))  /* 源代码 */

/**
  * @}
  */

/* Private function prototypes -----------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/** @addtogroup DMA_LL_Exported_Functions
  * @{
  */

/** @addtogroup DMA_LL_EF_Init
  * @{
  */

/**
  * @brief  De-initialize the DMA registers to their default reset values.
  * @param  DMAx DMAx Instance
  * @param  Stream This parameter can be one of the following values:
  *         @arg @ref LL_DMA_STREAM_0
  *         @arg @ref LL_DMA_STREAM_1
  *         @arg @ref LL_DMA_STREAM_2
  *         @arg @ref LL_DMA_STREAM_3
  *         @arg @ref LL_DMA_STREAM_4
  *         @arg @ref LL_DMA_STREAM_5
  *         @arg @ref LL_DMA_STREAM_6
  *         @arg @ref LL_DMA_STREAM_7
  *         @arg @ref LL_DMA_STREAM_ALL
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: DMA registers are de-initialized
  *          - ERROR: DMA registers are not de-initialized
  */
uint32_t LL_DMA_DeInit(DMA_TypeDef *DMAx, uint32_t Stream)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  DMA_Stream_TypeDef *tmp = (DMA_Stream_TypeDef *)DMA1_Stream0;  /* 赋值或初始化 */
  ErrorStatus status = SUCCESS;                         /* 赋值或初始化 */

  /* Check the DMA Instance DMAx and Stream parameters*/
  assert_param(IS_LL_DMA_ALL_STREAM_INSTANCE(DMAx, Stream));  /* 执行语句 */

  if (Stream == LL_DMA_STREAM_ALL)                      /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (DMAx == DMA1)                                   /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Force reset of DMA clock */
      LL_AHB1_GRP1_ForceReset(LL_AHB1_GRP1_PERIPH_DMA1);  /* 执行语句 */

      /* Release reset of DMA clock */
      LL_AHB1_GRP1_ReleaseReset(LL_AHB1_GRP1_PERIPH_DMA1);  /* 执行语句 */
    }                                                   /* 代码块结束 */
    else if (DMAx == DMA2)                              /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* Force reset of DMA clock */
      LL_AHB1_GRP1_ForceReset(LL_AHB1_GRP1_PERIPH_DMA2);  /* 执行语句 */

      /* Release reset of DMA clock */
      LL_AHB1_GRP1_ReleaseReset(LL_AHB1_GRP1_PERIPH_DMA2);  /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      status = ERROR;                                   /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Disable the selected Stream */
    LL_DMA_DisableStream(DMAx,Stream);                  /* 执行语句 */

    /* Get the DMA Stream Instance */
    tmp = (DMA_Stream_TypeDef *)(__LL_DMA_GET_STREAM_INSTANCE(DMAx, Stream));  /* 赋值或初始化 */

    /* Reset DMAx_Streamy configuration register */
    LL_DMA_WriteReg(tmp, CR, 0U);                       /* 执行语句 */

    /* Reset DMAx_Streamy remaining bytes register */
    LL_DMA_WriteReg(tmp, NDTR, 0U);                     /* 执行语句 */

    /* Reset DMAx_Streamy peripheral address register */
    LL_DMA_WriteReg(tmp, PAR, 0U);                      /* 执行语句 */

    /* Reset DMAx_Streamy memory address register */
    LL_DMA_WriteReg(tmp, M0AR, 0U);                     /* 执行语句 */

    /* Reset DMAx_Streamy memory address register */
    LL_DMA_WriteReg(tmp, M1AR, 0U);                     /* 执行语句 */

    /* Reset DMAx_Streamy FIFO control register */
    LL_DMA_WriteReg(tmp, FCR, 0x00000021U);             /* 执行语句 */

    /* Reset Channel register field for DMAx Stream*/
    LL_DMA_SetChannelSelection(DMAx, Stream, LL_DMA_CHANNEL_0);  /* 执行语句 */

    if(Stream == LL_DMA_STREAM_0)                       /* 条件判断 */
    {                                                   /* 代码块开始 */
       /* Reset the Stream0 pending flags */
       DMAx->LIFCR = 0x0000003FU;                       /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if(Stream == LL_DMA_STREAM_1)                  /* 否则如果 */
    {                                                   /* 代码块开始 */
       /* Reset the Stream1 pending flags */
       DMAx->LIFCR = 0x00000F40U;                       /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if(Stream == LL_DMA_STREAM_2)                  /* 否则如果 */
    {                                                   /* 代码块开始 */
       /* Reset the Stream2 pending flags */
       DMAx->LIFCR = 0x003F0000U;                       /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if(Stream == LL_DMA_STREAM_3)                  /* 否则如果 */
    {                                                   /* 代码块开始 */
       /* Reset the Stream3 pending flags */
       DMAx->LIFCR = 0x0F400000U;                       /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if(Stream == LL_DMA_STREAM_4)                  /* 否则如果 */
    {                                                   /* 代码块开始 */
       /* Reset the Stream4 pending flags */
       DMAx->HIFCR = 0x0000003FU;                       /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if(Stream == LL_DMA_STREAM_5)                  /* 否则如果 */
    {                                                   /* 代码块开始 */
       /* Reset the Stream5 pending flags */
       DMAx->HIFCR = 0x00000F40U;                       /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if(Stream == LL_DMA_STREAM_6)                  /* 否则如果 */
    {                                                   /* 代码块开始 */
       /* Reset the Stream6 pending flags */
       DMAx->HIFCR = 0x003F0000U;                       /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if(Stream == LL_DMA_STREAM_7)                  /* 否则如果 */
    {                                                   /* 代码块开始 */
       /* Reset the Stream7 pending flags */
       DMAx->HIFCR = 0x0F400000U;                       /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      status = ERROR;                                   /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initialize the DMA registers according to the specified parameters in DMA_InitStruct.
  * @note   To convert DMAx_Streamy Instance to DMAx Instance and Streamy, use helper macros :
  *         @arg @ref __LL_DMA_GET_INSTANCE
  *         @arg @ref __LL_DMA_GET_STREAM
  * @param  DMAx DMAx Instance
  * @param  Stream This parameter can be one of the following values:
  *         @arg @ref LL_DMA_STREAM_0
  *         @arg @ref LL_DMA_STREAM_1
  *         @arg @ref LL_DMA_STREAM_2
  *         @arg @ref LL_DMA_STREAM_3
  *         @arg @ref LL_DMA_STREAM_4
  *         @arg @ref LL_DMA_STREAM_5
  *         @arg @ref LL_DMA_STREAM_6
  *         @arg @ref LL_DMA_STREAM_7
  * @param  DMA_InitStruct pointer to a @ref LL_DMA_InitTypeDef structure.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: DMA registers are initialized
  *          - ERROR: Not applicable
  */
uint32_t LL_DMA_Init(DMA_TypeDef *DMAx, uint32_t Stream, LL_DMA_InitTypeDef *DMA_InitStruct)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  /* Check the DMA Instance DMAx and Stream parameters*/
  assert_param(IS_LL_DMA_ALL_STREAM_INSTANCE(DMAx, Stream));  /* 执行语句 */

  /* Check the DMA parameters from DMA_InitStruct */
  assert_param(IS_LL_DMA_DIRECTION(DMA_InitStruct->Direction));  /* 执行语句 */
  assert_param(IS_LL_DMA_MODE(DMA_InitStruct->Mode));   /* 执行语句 */
  assert_param(IS_LL_DMA_PERIPHINCMODE(DMA_InitStruct->PeriphOrM2MSrcIncMode));  /* 执行语句 */
  assert_param(IS_LL_DMA_MEMORYINCMODE(DMA_InitStruct->MemoryOrM2MDstIncMode));  /* 执行语句 */
  assert_param(IS_LL_DMA_PERIPHDATASIZE(DMA_InitStruct->PeriphOrM2MSrcDataSize));  /* 执行语句 */
  assert_param(IS_LL_DMA_MEMORYDATASIZE(DMA_InitStruct->MemoryOrM2MDstDataSize));  /* 执行语句 */
  assert_param(IS_LL_DMA_NBDATA(DMA_InitStruct->NbData));  /* 执行语句 */
  assert_param(IS_LL_DMA_CHANNEL(DMA_InitStruct->Channel));  /* 执行语句 */
  assert_param(IS_LL_DMA_PRIORITY(DMA_InitStruct->Priority));  /* 执行语句 */
  assert_param(IS_LL_DMA_FIFO_MODE_STATE(DMA_InitStruct->FIFOMode));  /* 执行语句 */
  /* Check the memory burst, peripheral burst and FIFO threshold parameters only
     when FIFO mode is enabled */
  if(DMA_InitStruct->FIFOMode != LL_DMA_FIFOMODE_DISABLE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    assert_param(IS_LL_DMA_FIFO_THRESHOLD(DMA_InitStruct->FIFOThreshold));  /* 执行语句 */
    assert_param(IS_LL_DMA_MEMORY_BURST(DMA_InitStruct->MemBurst));  /* 执行语句 */
    assert_param(IS_LL_DMA_PERIPHERAL_BURST(DMA_InitStruct->PeriphBurst));  /* 执行语句 */
  }                                                     /* 代码块结束 */

  /*---------------------------- DMAx SxCR Configuration ------------------------
   * Configure DMAx_Streamy: data transfer direction, data transfer mode,
   *                          peripheral and memory increment mode,
   *                          data size alignment and  priority level with parameters :
   * - Direction:      DMA_SxCR_DIR[1:0] bits
   * - Mode:           DMA_SxCR_CIRC bit
   * - PeriphOrM2MSrcIncMode:  DMA_SxCR_PINC bit
   * - MemoryOrM2MDstIncMode:  DMA_SxCR_MINC bit
   * - PeriphOrM2MSrcDataSize: DMA_SxCR_PSIZE[1:0] bits
   * - MemoryOrM2MDstDataSize: DMA_SxCR_MSIZE[1:0] bits
   * - Priority:               DMA_SxCR_PL[1:0] bits
   */
  LL_DMA_ConfigTransfer(DMAx, Stream, DMA_InitStruct->Direction | \
                        DMA_InitStruct->Mode                    | \
                        DMA_InitStruct->PeriphOrM2MSrcIncMode   | \
                        DMA_InitStruct->MemoryOrM2MDstIncMode   | \
                        DMA_InitStruct->PeriphOrM2MSrcDataSize  | \
                        DMA_InitStruct->MemoryOrM2MDstDataSize  | \
                        DMA_InitStruct->Priority        /* 源代码 */
                        );                              /* 执行语句 */

  if(DMA_InitStruct->FIFOMode != LL_DMA_FIFOMODE_DISABLE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /*---------------------------- DMAx SxFCR Configuration ------------------------
     * Configure DMAx_Streamy:  fifo mode and fifo threshold with parameters :
     * - FIFOMode:                DMA_SxFCR_DMDIS bit
     * - FIFOThreshold:           DMA_SxFCR_FTH[1:0] bits
     */
    LL_DMA_ConfigFifo(DMAx, Stream, DMA_InitStruct->FIFOMode, DMA_InitStruct->FIFOThreshold);     /* 执行语句 */

    /*---------------------------- DMAx SxCR Configuration --------------------------
     * Configure DMAx_Streamy:  memory burst transfer with parameters :
     * - MemBurst:                DMA_SxCR_MBURST[1:0] bits
     */
    LL_DMA_SetMemoryBurstxfer(DMAx,Stream,DMA_InitStruct->MemBurst);   /* 执行语句 */

    /*---------------------------- DMAx SxCR Configuration --------------------------
     * Configure DMAx_Streamy:  peripheral burst transfer with parameters :
     * - PeriphBurst:             DMA_SxCR_PBURST[1:0] bits
     */
    LL_DMA_SetPeriphBurstxfer(DMAx,Stream,DMA_InitStruct->PeriphBurst);  /* 执行语句 */
  }                                                     /* 代码块结束 */

  /*-------------------------- DMAx SxM0AR Configuration --------------------------
   * Configure the memory or destination base address with parameter :
   * - MemoryOrM2MDstAddress:     DMA_SxM0AR_M0A[31:0] bits
   */
  LL_DMA_SetMemoryAddress(DMAx, Stream, DMA_InitStruct->MemoryOrM2MDstAddress);  /* 执行语句 */

  /*-------------------------- DMAx SxPAR Configuration ---------------------------
   * Configure the peripheral or source base address with parameter :
   * - PeriphOrM2MSrcAddress:     DMA_SxPAR_PA[31:0] bits
   */
  LL_DMA_SetPeriphAddress(DMAx, Stream, DMA_InitStruct->PeriphOrM2MSrcAddress);  /* 执行语句 */

  /*--------------------------- DMAx SxNDTR Configuration -------------------------
   * Configure the peripheral base address with parameter :
   * - NbData:                    DMA_SxNDT[15:0] bits
   */
  LL_DMA_SetDataLength(DMAx, Stream, DMA_InitStruct->NbData);  /* 执行语句 */

  /*--------------------------- DMA SxCR_CHSEL Configuration ----------------------
   * Configure the peripheral base address with parameter :
   * - PeriphRequest:             DMA_SxCR_CHSEL[2:0] bits
   */
  LL_DMA_SetChannelSelection(DMAx, Stream, DMA_InitStruct->Channel);  /* 执行语句 */

  return SUCCESS;                                       /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Set each @ref LL_DMA_InitTypeDef field to default value.
  * @param  DMA_InitStruct Pointer to a @ref LL_DMA_InitTypeDef structure.
  * @retval None
  */
void LL_DMA_StructInit(LL_DMA_InitTypeDef *DMA_InitStruct)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Set DMA_InitStruct fields to default values */
  DMA_InitStruct->PeriphOrM2MSrcAddress  = 0x00000000U;  /* 赋值或初始化 */
  DMA_InitStruct->MemoryOrM2MDstAddress  = 0x00000000U;  /* 赋值或初始化 */
  DMA_InitStruct->Direction              = LL_DMA_DIRECTION_PERIPH_TO_MEMORY;  /* 赋值或初始化 */
  DMA_InitStruct->Mode                   = LL_DMA_MODE_NORMAL;  /* 赋值或初始化 */
  DMA_InitStruct->PeriphOrM2MSrcIncMode  = LL_DMA_PERIPH_NOINCREMENT;  /* 赋值或初始化 */
  DMA_InitStruct->MemoryOrM2MDstIncMode  = LL_DMA_MEMORY_NOINCREMENT;  /* 赋值或初始化 */
  DMA_InitStruct->PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE;  /* 赋值或初始化 */
  DMA_InitStruct->MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE;  /* 赋值或初始化 */
  DMA_InitStruct->NbData                 = 0x00000000U;  /* 赋值或初始化 */
  DMA_InitStruct->Channel                = LL_DMA_CHANNEL_0;  /* 赋值或初始化 */
  DMA_InitStruct->Priority               = LL_DMA_PRIORITY_LOW;  /* 赋值或初始化 */
  DMA_InitStruct->FIFOMode               = LL_DMA_FIFOMODE_DISABLE;  /* 赋值或初始化 */
  DMA_InitStruct->FIFOThreshold          = LL_DMA_FIFOTHRESHOLD_1_4;  /* 赋值或初始化 */
  DMA_InitStruct->MemBurst               = LL_DMA_MBURST_SINGLE;  /* 赋值或初始化 */
  DMA_InitStruct->PeriphBurst            = LL_DMA_PBURST_SINGLE;  /* 赋值或初始化 */
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

#endif /* DMA1 || DMA2 */

/**
  * @}
  */

#endif /* USE_FULL_LL_DRIVER */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
