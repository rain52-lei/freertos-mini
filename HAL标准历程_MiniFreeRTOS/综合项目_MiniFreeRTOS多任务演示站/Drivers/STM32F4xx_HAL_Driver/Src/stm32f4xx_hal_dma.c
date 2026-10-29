/* 文件: stm32f4xx_hal_dma.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_dma.c
  * @author  MCD Application Team
  * @brief   DMA HAL module driver.
  *    
  *          This file provides firmware functions to manage the following 
  *          functionalities of the Direct Memory Access (DMA) peripheral:
  *           + Initialization and de-initialization functions
  *           + IO operation functions
  *           + Peripheral State and errors functions
  @verbatim     
  ==============================================================================
                        ##### How to use this driver #####
  ==============================================================================
  [..]
   (#) Enable and configure the peripheral to be connected to the DMA Stream
       (except for internal SRAM/FLASH memories: no initialization is 
       necessary) please refer to Reference manual for connection between peripherals
       and DMA requests.

   (#) For a given Stream, program the required configuration through the following parameters:
       Transfer Direction, Source and Destination data formats, 
       Circular, Normal or peripheral flow control mode, Stream Priority level, 
       Source and Destination Increment mode, FIFO mode and its Threshold (if needed), 
       Burst mode for Source and/or Destination (if needed) using HAL_DMA_Init() function.

   -@-   Prior to HAL_DMA_Init() the clock must be enabled for DMA through the following macros:
         __HAL_RCC_DMA1_CLK_ENABLE() or __HAL_RCC_DMA2_CLK_ENABLE().

     *** Polling mode IO operation ***
     =================================
    [..]
          (+) Use HAL_DMA_Start() to start DMA transfer after the configuration of Source 
              address and destination address and the Length of data to be transferred.
          (+) Use HAL_DMA_PollForTransfer() to poll for the end of current transfer, in this  
              case a fixed Timeout can be configured by User depending from his application.
          (+) Use HAL_DMA_Abort() function to abort the current transfer.

     *** Interrupt mode IO operation ***
     ===================================
    [..]
          (+) Configure the DMA interrupt priority using HAL_NVIC_SetPriority()
          (+) Enable the DMA IRQ handler using HAL_NVIC_EnableIRQ() 
          (+) Use HAL_DMA_Start_IT() to start DMA transfer after the configuration of  
              Source address and destination address and the Length of data to be transferred. In this 
              case the DMA interrupt is configured 
          (+) Use HAL_DMA_IRQHandler() called under DMA_IRQHandler() Interrupt subroutine
          (+) At the end of data transfer HAL_DMA_IRQHandler() function is executed and user can 
              add his own function by customization of function pointer XferCpltCallback and 
              XferErrorCallback (i.e a member of DMA handle structure).
    [..]
     (#) Use HAL_DMA_GetState() function to return the DMA state and HAL_DMA_GetError() in case of error 
         detection.

     (#) Use HAL_DMA_Abort_IT() function to abort the current transfer

     -@-   In Memory-to-Memory transfer mode, Circular mode is not allowed.

     -@-   The FIFO is used mainly to reduce bus usage and to allow data packing/unpacking: it is
           possible to set different Data Sizes for the Peripheral and the Memory (ie. you can set
           Half-Word data size for the peripheral to access its data register and set Word data size
           for the Memory to gain in access time. Each two half words will be packed and written in
           a single access to a Word in the Memory).

     -@-   When FIFO is disabled, it is not allowed to configure different Data Sizes for Source
           and Destination. In this case the Peripheral Data Size will be applied to both Source
           and Destination.

     *** DMA HAL driver macros list ***
     =============================================
     [..]
       Below the list of most used macros in DMA HAL driver.

      (+) __HAL_DMA_ENABLE: Enable the specified DMA Stream.
      (+) __HAL_DMA_DISABLE: Disable the specified DMA Stream.
      (+) __HAL_DMA_GET_IT_SOURCE: Check whether the specified DMA Stream interrupt has occurred or not. 

     [..]
      (@) You can refer to the DMA HAL driver header file for more useful macros

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

/** @defgroup DMA DMA
  * @brief DMA HAL module driver
  * @{
  */

#ifdef HAL_DMA_MODULE_ENABLED                           /* 条件编译 */

/* Private types -------------------------------------------------------------*/
typedef struct                                          /* 类型定义 */
{                                                       /* 代码块开始 */
  __IO uint32_t ISR;   /*!< DMA interrupt status register */
  __IO uint32_t Reserved0;                              /* 执行语句 */
  __IO uint32_t IFCR;  /*!< DMA interrupt flag clear register */
} DMA_Base_Registers;                                   /* 代码块结束 */

/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/** @addtogroup DMA_Private_Constants
 * @{
 */
 #define HAL_TIMEOUT_DMA_ABORT    5U  /* 5 ms */
/**
  * @}
  */
/* Private macros ------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/** @addtogroup DMA_Private_Functions
  * @{
  */
static void DMA_SetConfig(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength);  /* 静态函数 */
static uint32_t DMA_CalcBaseAndBitshift(DMA_HandleTypeDef *hdma);  /* 静态变量/函数 */
static HAL_StatusTypeDef DMA_CheckFifoParam(DMA_HandleTypeDef *hdma);  /* 静态变量/函数 */

/**
  * @}
  */  

/* Exported functions ---------------------------------------------------------*/
/** @addtogroup DMA_Exported_Functions
  * @{
  */

/** @addtogroup DMA_Exported_Functions_Group1
  *
@verbatim
 ===============================================================================
             ##### Initialization and de-initialization functions  #####
 ===============================================================================
    [..]
    This section provides functions allowing to initialize the DMA Stream source
    and destination addresses, incrementation and data sizes, transfer direction, 
    circular/normal mode selection, memory-to-memory mode selection and Stream priority value.
    [..]
    The HAL_DMA_Init() function follows the DMA configuration procedures as described in
    reference manual.

@endverbatim
  * @{
  */

/**
  * @brief  Initialize the DMA according to the specified
  *         parameters in the DMA_InitTypeDef and create the associated handle.
  * @param  hdma Pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA Stream.  
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA_Init(DMA_HandleTypeDef *hdma)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmp = 0U;                                    /* 32位无符号变量/函数 */
  uint32_t tickstart = HAL_GetTick();                   /* 32位无符号变量/函数 */
  DMA_Base_Registers *regs;                             /* 执行语句 */

  /* Check the DMA peripheral state */
  if(hdma == NULL)                                      /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the parameters */
  assert_param(IS_DMA_STREAM_ALL_INSTANCE(hdma->Instance));  /* 执行语句 */
  assert_param(IS_DMA_CHANNEL(hdma->Init.Channel));     /* 执行语句 */
  assert_param(IS_DMA_DIRECTION(hdma->Init.Direction));  /* 执行语句 */
  assert_param(IS_DMA_PERIPHERAL_INC_STATE(hdma->Init.PeriphInc));  /* 执行语句 */
  assert_param(IS_DMA_MEMORY_INC_STATE(hdma->Init.MemInc));  /* 执行语句 */
  assert_param(IS_DMA_PERIPHERAL_DATA_SIZE(hdma->Init.PeriphDataAlignment));  /* 执行语句 */
  assert_param(IS_DMA_MEMORY_DATA_SIZE(hdma->Init.MemDataAlignment));  /* 执行语句 */
  assert_param(IS_DMA_MODE(hdma->Init.Mode));           /* 执行语句 */
  assert_param(IS_DMA_PRIORITY(hdma->Init.Priority));   /* 执行语句 */
  assert_param(IS_DMA_FIFO_MODE_STATE(hdma->Init.FIFOMode));  /* 执行语句 */
  /* Check the memory burst, peripheral burst and FIFO threshold parameters only
     when FIFO mode is enabled */
  if(hdma->Init.FIFOMode != DMA_FIFOMODE_DISABLE)       /* 条件判断 */
  {                                                     /* 代码块开始 */
    assert_param(IS_DMA_FIFO_THRESHOLD(hdma->Init.FIFOThreshold));  /* 执行语句 */
    assert_param(IS_DMA_MEMORY_BURST(hdma->Init.MemBurst));  /* 执行语句 */
    assert_param(IS_DMA_PERIPHERAL_BURST(hdma->Init.PeriphBurst));  /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Allocate lock resource */
  __HAL_UNLOCK(hdma);                                   /* HAL辅助宏/函数 */

  /* Change DMA peripheral state */
  hdma->State = HAL_DMA_STATE_BUSY;                     /* 赋值或初始化 */

  /* Disable the peripheral */
  __HAL_DMA_DISABLE(hdma);                              /* HAL辅助宏/函数 */

  /* Check if the DMA Stream is effectively disabled */
  while((hdma->Instance->CR & DMA_SxCR_EN) != RESET)    /* while循环 */
  {                                                     /* 代码块开始 */
    /* Check for the Timeout */
    if((HAL_GetTick() - tickstart ) > HAL_TIMEOUT_DMA_ABORT)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Update error code */
      hdma->ErrorCode = HAL_DMA_ERROR_TIMEOUT;          /* 赋值或初始化 */

      /* Change the DMA state */
      hdma->State = HAL_DMA_STATE_TIMEOUT;              /* 赋值或初始化 */

      return HAL_TIMEOUT;                               /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Get the CR register value */
  tmp = hdma->Instance->CR;                             /* 赋值或初始化 */

  /* Clear CHSEL, MBURST, PBURST, PL, MSIZE, PSIZE, MINC, PINC, CIRC, DIR, CT and DBM bits */
  tmp &= ((uint32_t)~(DMA_SxCR_CHSEL | DMA_SxCR_MBURST | DMA_SxCR_PBURST | \
                      DMA_SxCR_PL    | DMA_SxCR_MSIZE  | DMA_SxCR_PSIZE  | \
                      DMA_SxCR_MINC  | DMA_SxCR_PINC   | DMA_SxCR_CIRC   | \
                      DMA_SxCR_DIR   | DMA_SxCR_CT     | DMA_SxCR_DBM));  /* 执行语句 */

  /* Prepare the DMA Stream configuration */
  tmp |=  hdma->Init.Channel             | hdma->Init.Direction        |  /* 源代码 */
          hdma->Init.PeriphInc           | hdma->Init.MemInc           |  /* 源代码 */
          hdma->Init.PeriphDataAlignment | hdma->Init.MemDataAlignment |  /* 源代码 */
          hdma->Init.Mode                | hdma->Init.Priority;  /* 执行语句 */

  /* the Memory burst and peripheral burst are not used when the FIFO is disabled */
  if(hdma->Init.FIFOMode == DMA_FIFOMODE_ENABLE)        /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Get memory burst and peripheral burst */
    tmp |=  hdma->Init.MemBurst | hdma->Init.PeriphBurst;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Write to DMA Stream CR register */
  hdma->Instance->CR = tmp;                             /* 赋值或初始化 */

  /* Get the FCR register value */
  tmp = hdma->Instance->FCR;                            /* 赋值或初始化 */

  /* Clear Direct mode and FIFO threshold bits */
  tmp &= (uint32_t)~(DMA_SxFCR_DMDIS | DMA_SxFCR_FTH);  /* 赋值或初始化 */

  /* Prepare the DMA Stream FIFO configuration */
  tmp |= hdma->Init.FIFOMode;                           /* 赋值或初始化 */

  /* The FIFO threshold is not used when the FIFO mode is disabled */
  if(hdma->Init.FIFOMode == DMA_FIFOMODE_ENABLE)        /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Get the FIFO threshold */
    tmp |= hdma->Init.FIFOThreshold;                    /* 赋值或初始化 */

    /* Check compatibility between FIFO threshold level and size of the memory burst */
    /* for INCR4, INCR8, INCR16 bursts */
    if (hdma->Init.MemBurst != DMA_MBURST_SINGLE)       /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (DMA_CheckFifoParam(hdma) != HAL_OK)           /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Update error code */
        hdma->ErrorCode = HAL_DMA_ERROR_PARAM;          /* 赋值或初始化 */

        /* Change the DMA state */
        hdma->State = HAL_DMA_STATE_READY;              /* 赋值或初始化 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Write to DMA Stream FCR */
  hdma->Instance->FCR = tmp;                            /* 赋值或初始化 */

  /* Initialize StreamBaseAddress and StreamIndex parameters to be used to calculate
     DMA steam Base Address needed by HAL_DMA_IRQHandler() and HAL_DMA_PollForTransfer() */
  regs = (DMA_Base_Registers *)DMA_CalcBaseAndBitshift(hdma);  /* 赋值或初始化 */

  /* Clear all interrupt flags */
  regs->IFCR = 0x3FU << hdma->StreamIndex;              /* 赋值或初始化 */

  /* Initialize the error code */
  hdma->ErrorCode = HAL_DMA_ERROR_NONE;                 /* 赋值或初始化 */

  /* Initialize the DMA state */
  hdma->State = HAL_DMA_STATE_READY;                    /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  DeInitializes the DMA peripheral 
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA Stream.  
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA_DeInit(DMA_HandleTypeDef *hdma)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  DMA_Base_Registers *regs;                             /* 执行语句 */

  /* Check the DMA peripheral state */
  if(hdma == NULL)                                      /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the DMA peripheral state */
  if(hdma->State == HAL_DMA_STATE_BUSY)                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Return error status */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the parameters */
  assert_param(IS_DMA_STREAM_ALL_INSTANCE(hdma->Instance));  /* 执行语句 */

  /* Disable the selected DMA Streamx */
  __HAL_DMA_DISABLE(hdma);                              /* HAL辅助宏/函数 */

  /* Reset DMA Streamx control register */
  hdma->Instance->CR   = 0U;                            /* 赋值或初始化 */

  /* Reset DMA Streamx number of data to transfer register */
  hdma->Instance->NDTR = 0U;                            /* 赋值或初始化 */

  /* Reset DMA Streamx peripheral address register */
  hdma->Instance->PAR  = 0U;                            /* 赋值或初始化 */

  /* Reset DMA Streamx memory 0 address register */
  hdma->Instance->M0AR = 0U;                            /* 赋值或初始化 */

  /* Reset DMA Streamx memory 1 address register */
  hdma->Instance->M1AR = 0U;                            /* 赋值或初始化 */

  /* Reset DMA Streamx FIFO control register */
  hdma->Instance->FCR  = 0x00000021U;                   /* 赋值或初始化 */

  /* Get DMA steam Base Address */  
  regs = (DMA_Base_Registers *)DMA_CalcBaseAndBitshift(hdma);  /* 赋值或初始化 */

  /* Clean all callbacks */
  hdma->XferCpltCallback = NULL;                        /* 赋值或初始化 */
  hdma->XferHalfCpltCallback = NULL;                    /* 赋值或初始化 */
  hdma->XferM1CpltCallback = NULL;                      /* 赋值或初始化 */
  hdma->XferM1HalfCpltCallback = NULL;                  /* 赋值或初始化 */
  hdma->XferErrorCallback = NULL;                       /* 赋值或初始化 */
  hdma->XferAbortCallback = NULL;                       /* 赋值或初始化 */

  /* Clear all interrupt flags at correct offset within the register */
  regs->IFCR = 0x3FU << hdma->StreamIndex;              /* 赋值或初始化 */

  /* Reset the error code */
  hdma->ErrorCode = HAL_DMA_ERROR_NONE;                 /* 赋值或初始化 */

  /* Reset the DMA state */
  hdma->State = HAL_DMA_STATE_RESET;                    /* 赋值或初始化 */

  /* Release Lock */
  __HAL_UNLOCK(hdma);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @addtogroup DMA_Exported_Functions_Group2
  *
@verbatim   
 ===============================================================================
                      #####  IO operation functions  #####
 ===============================================================================
    [..]  This section provides functions allowing to:
      (+) Configure the source, destination address and data length and Start DMA transfer
      (+) Configure the source, destination address and data length and 
          Start DMA transfer with interrupt
      (+) Abort DMA transfer
      (+) Poll for transfer complete
      (+) Handle DMA interrupt request  

@endverbatim
  * @{
  */

/**
  * @brief  Starts the DMA Transfer.
  * @param  hdma       pointer to a DMA_HandleTypeDef structure that contains
  *                     the configuration information for the specified DMA Stream.
  * @param  SrcAddress The source memory Buffer address
  * @param  DstAddress The destination memory Buffer address
  * @param  DataLength The length of data to be transferred from source to destination
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA_Start(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Check the parameters */
  assert_param(IS_DMA_BUFFER_SIZE(DataLength));         /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hdma);                                     /* HAL辅助宏/函数 */

  if(HAL_DMA_STATE_READY == hdma->State)                /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Change DMA peripheral state */
    hdma->State = HAL_DMA_STATE_BUSY;                   /* 赋值或初始化 */

    /* Initialize the error code */
    hdma->ErrorCode = HAL_DMA_ERROR_NONE;               /* 赋值或初始化 */

    /* Configure the source, destination address and the data length */
    DMA_SetConfig(hdma, SrcAddress, DstAddress, DataLength);  /* 执行语句 */

    /* Enable the Peripheral */
    __HAL_DMA_ENABLE(hdma);                             /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Process unlocked */
    __HAL_UNLOCK(hdma);                                 /* HAL辅助宏/函数 */

    /* Return error status */
    status = HAL_BUSY;                                  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Start the DMA Transfer with interrupt enabled.
  * @param  hdma       pointer to a DMA_HandleTypeDef structure that contains
  *                     the configuration information for the specified DMA Stream.  
  * @param  SrcAddress The source memory Buffer address
  * @param  DstAddress The destination memory Buffer address
  * @param  DataLength The length of data to be transferred from source to destination
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA_Start_IT(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* calculate DMA base and stream number */
  DMA_Base_Registers *regs = (DMA_Base_Registers *)hdma->StreamBaseAddress;  /* 赋值或初始化 */

  /* Check the parameters */
  assert_param(IS_DMA_BUFFER_SIZE(DataLength));         /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hdma);                                     /* HAL辅助宏/函数 */

  if(HAL_DMA_STATE_READY == hdma->State)                /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Change DMA peripheral state */
    hdma->State = HAL_DMA_STATE_BUSY;                   /* 赋值或初始化 */

    /* Initialize the error code */
    hdma->ErrorCode = HAL_DMA_ERROR_NONE;               /* 赋值或初始化 */

    /* Configure the source, destination address and the data length */
    DMA_SetConfig(hdma, SrcAddress, DstAddress, DataLength);  /* 执行语句 */

    /* Clear all interrupt flags at correct offset within the register */
    regs->IFCR = 0x3FU << hdma->StreamIndex;            /* 赋值或初始化 */

    /* Enable Common interrupts*/
    hdma->Instance->CR  |= DMA_IT_TC | DMA_IT_TE | DMA_IT_DME;  /* 赋值或初始化 */

    if(hdma->XferHalfCpltCallback != NULL)              /* 条件判断 */
    {                                                   /* 代码块开始 */
      hdma->Instance->CR  |= DMA_IT_HT;                 /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Enable the Peripheral */
    __HAL_DMA_ENABLE(hdma);                             /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Process unlocked */
    __HAL_UNLOCK(hdma);	                                /* HAL辅助宏/函数 */

    /* Return error status */
    status = HAL_BUSY;                                  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Aborts the DMA Transfer.
  * @param  hdma   pointer to a DMA_HandleTypeDef structure that contains
  *                 the configuration information for the specified DMA Stream.
  *                   
  * @note  After disabling a DMA Stream, a check for wait until the DMA Stream is 
  *        effectively disabled is added. If a Stream is disabled 
  *        while a data transfer is ongoing, the current data will be transferred
  *        and the Stream will be effectively disabled only after the transfer of
  *        this single data is finished.  
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA_Abort(DMA_HandleTypeDef *hdma)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* calculate DMA base and stream number */
  DMA_Base_Registers *regs = (DMA_Base_Registers *)hdma->StreamBaseAddress;  /* 赋值或初始化 */

  uint32_t tickstart = HAL_GetTick();                   /* 32位无符号变量/函数 */

  if(hdma->State != HAL_DMA_STATE_BUSY)                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    hdma->ErrorCode = HAL_DMA_ERROR_NO_XFER;            /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hdma);                                 /* HAL辅助宏/函数 */

    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Disable all the transfer interrupts */
    hdma->Instance->CR  &= ~(DMA_IT_TC | DMA_IT_TE | DMA_IT_DME);  /* 赋值或初始化 */
    hdma->Instance->FCR &= ~(DMA_IT_FE);                /* 赋值或初始化 */

    if((hdma->XferHalfCpltCallback != NULL) || (hdma->XferM1HalfCpltCallback != NULL))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hdma->Instance->CR  &= ~(DMA_IT_HT);              /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Disable the stream */
    __HAL_DMA_DISABLE(hdma);                            /* HAL辅助宏/函数 */

    /* Check if the DMA Stream is effectively disabled */
    while((hdma->Instance->CR & DMA_SxCR_EN) != RESET)  /* while循环 */
    {                                                   /* 代码块开始 */
      /* Check for the Timeout */
      if((HAL_GetTick() - tickstart ) > HAL_TIMEOUT_DMA_ABORT)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Update error code */
        hdma->ErrorCode = HAL_DMA_ERROR_TIMEOUT;        /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hdma);                             /* HAL辅助宏/函数 */

        /* Change the DMA state */
        hdma->State = HAL_DMA_STATE_TIMEOUT;            /* 赋值或初始化 */

        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Clear all interrupt flags at correct offset within the register */
    regs->IFCR = 0x3FU << hdma->StreamIndex;            /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hdma);                                 /* HAL辅助宏/函数 */

    /* Change the DMA state*/
    hdma->State = HAL_DMA_STATE_READY;                  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Aborts the DMA Transfer in Interrupt mode.
  * @param  hdma   pointer to a DMA_HandleTypeDef structure that contains
  *                 the configuration information for the specified DMA Stream.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA_Abort_IT(DMA_HandleTypeDef *hdma)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  if(hdma->State != HAL_DMA_STATE_BUSY)                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    hdma->ErrorCode = HAL_DMA_ERROR_NO_XFER;            /* 赋值或初始化 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Set Abort State  */
    hdma->State = HAL_DMA_STATE_ABORT;                  /* 赋值或初始化 */

    /* Disable the stream */
    __HAL_DMA_DISABLE(hdma);                            /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Polling for transfer complete.
  * @param  hdma          pointer to a DMA_HandleTypeDef structure that contains
  *                        the configuration information for the specified DMA Stream.
  * @param  CompleteLevel Specifies the DMA level complete.
  * @note   The polling mode is kept in this version for legacy. it is recommanded to use the IT model instead.
  *         This model could be used for debug purpose.
  * @note   The HAL_DMA_PollForTransfer API cannot be used in circular and double buffering mode (automatic circular mode). 
  * @param  Timeout       Timeout duration.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA_PollForTransfer(DMA_HandleTypeDef *hdma, HAL_DMA_LevelCompleteTypeDef CompleteLevel, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */
  uint32_t mask_cpltlevel;                              /* 32位无符号变量/函数 */
  uint32_t tickstart = HAL_GetTick();                   /* 32位无符号变量/函数 */
  uint32_t tmpisr;                                      /* 32位无符号变量/函数 */

  /* calculate DMA base and stream number */
  DMA_Base_Registers *regs;                             /* 执行语句 */

  if(HAL_DMA_STATE_BUSY != hdma->State)                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* No transfer ongoing */
    hdma->ErrorCode = HAL_DMA_ERROR_NO_XFER;            /* 赋值或初始化 */
    __HAL_UNLOCK(hdma);                                 /* HAL辅助宏/函数 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Polling mode not supported in circular mode and double buffering mode */
  if ((hdma->Instance->CR & DMA_SxCR_CIRC) != RESET)    /* 条件判断 */
  {                                                     /* 代码块开始 */
    hdma->ErrorCode = HAL_DMA_ERROR_NOT_SUPPORTED;      /* 赋值或初始化 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Get the level transfer complete flag */
  if(CompleteLevel == HAL_DMA_FULL_TRANSFER)            /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Transfer Complete flag */
    mask_cpltlevel = DMA_FLAG_TCIF0_4 << hdma->StreamIndex;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Half Transfer Complete flag */
    mask_cpltlevel = DMA_FLAG_HTIF0_4 << hdma->StreamIndex;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  regs = (DMA_Base_Registers *)hdma->StreamBaseAddress;  /* 赋值或初始化 */
  tmpisr = regs->ISR;                                   /* 赋值或初始化 */

  while(((tmpisr & mask_cpltlevel) == RESET) && ((hdma->ErrorCode & HAL_DMA_ERROR_TE) == RESET))  /* while循环 */
  {                                                     /* 代码块开始 */
    /* Check for the Timeout (Not applicable in circular mode)*/
    if(Timeout != HAL_MAX_DELAY)                        /* 条件判断 */
    {                                                   /* 代码块开始 */
      if((Timeout == 0U)||((HAL_GetTick() - tickstart ) > Timeout))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Update error code */
        hdma->ErrorCode = HAL_DMA_ERROR_TIMEOUT;        /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hdma);                             /* HAL辅助宏/函数 */

        /* Change the DMA state */
        hdma->State = HAL_DMA_STATE_READY;              /* 赋值或初始化 */

        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Get the ISR register value */
    tmpisr = regs->ISR;                                 /* 赋值或初始化 */

    if((tmpisr & (DMA_FLAG_TEIF0_4 << hdma->StreamIndex)) != RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Update error code */
      hdma->ErrorCode |= HAL_DMA_ERROR_TE;              /* 赋值或初始化 */

      /* Clear the transfer error flag */
      regs->IFCR = DMA_FLAG_TEIF0_4 << hdma->StreamIndex;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    if((tmpisr & (DMA_FLAG_FEIF0_4 << hdma->StreamIndex)) != RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Update error code */
      hdma->ErrorCode |= HAL_DMA_ERROR_FE;              /* 赋值或初始化 */

      /* Clear the FIFO error flag */
      regs->IFCR = DMA_FLAG_FEIF0_4 << hdma->StreamIndex;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    if((tmpisr & (DMA_FLAG_DMEIF0_4 << hdma->StreamIndex)) != RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Update error code */
      hdma->ErrorCode |= HAL_DMA_ERROR_DME;             /* 赋值或初始化 */

      /* Clear the Direct Mode error flag */
      regs->IFCR = DMA_FLAG_DMEIF0_4 << hdma->StreamIndex;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  if(hdma->ErrorCode != HAL_DMA_ERROR_NONE)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    if((hdma->ErrorCode & HAL_DMA_ERROR_TE) != RESET)   /* 条件判断 */
    {                                                   /* 代码块开始 */
      HAL_DMA_Abort(hdma);                              /* HAL DMA操作 */

      /* Clear the half transfer and transfer complete flags */
      regs->IFCR = (DMA_FLAG_HTIF0_4 | DMA_FLAG_TCIF0_4) << hdma->StreamIndex;  /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hdma);                               /* HAL辅助宏/函数 */

      /* Change the DMA state */
      hdma->State= HAL_DMA_STATE_READY;                 /* 赋值或初始化 */

      return HAL_ERROR;                                 /* 返回值 */
   }                                                    /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Get the level transfer complete flag */
  if(CompleteLevel == HAL_DMA_FULL_TRANSFER)            /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Clear the half transfer and transfer complete flags */
    regs->IFCR = (DMA_FLAG_HTIF0_4 | DMA_FLAG_TCIF0_4) << hdma->StreamIndex;  /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hdma);                                 /* HAL辅助宏/函数 */

    hdma->State = HAL_DMA_STATE_READY;                  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Clear the half transfer and transfer complete flags */
    regs->IFCR = (DMA_FLAG_HTIF0_4) << hdma->StreamIndex;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Handles DMA interrupt request.
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA Stream.  
  * @retval None
  */
void HAL_DMA_IRQHandler(DMA_HandleTypeDef *hdma)        /* 中断服务函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpisr;                                      /* 32位无符号变量/函数 */
  __IO uint32_t count = 0U;                             /* 赋值或初始化 */
  uint32_t timeout = SystemCoreClock / 9600U;           /* 32位无符号变量/函数 */

  /* calculate DMA base and stream number */
  DMA_Base_Registers *regs = (DMA_Base_Registers *)hdma->StreamBaseAddress;  /* 赋值或初始化 */

  tmpisr = regs->ISR;                                   /* 赋值或初始化 */

  /* Transfer Error Interrupt management ***************************************/
  if ((tmpisr & (DMA_FLAG_TEIF0_4 << hdma->StreamIndex)) != RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if(__HAL_DMA_GET_IT_SOURCE(hdma, DMA_IT_TE) != RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable the transfer error interrupt */
      hdma->Instance->CR  &= ~(DMA_IT_TE);              /* 赋值或初始化 */

      /* Clear the transfer error flag */
      regs->IFCR = DMA_FLAG_TEIF0_4 << hdma->StreamIndex;  /* 赋值或初始化 */

      /* Update error code */
      hdma->ErrorCode |= HAL_DMA_ERROR_TE;              /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /* FIFO Error Interrupt management ******************************************/
  if ((tmpisr & (DMA_FLAG_FEIF0_4 << hdma->StreamIndex)) != RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if(__HAL_DMA_GET_IT_SOURCE(hdma, DMA_IT_FE) != RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear the FIFO error flag */
      regs->IFCR = DMA_FLAG_FEIF0_4 << hdma->StreamIndex;  /* 赋值或初始化 */

      /* Update error code */
      hdma->ErrorCode |= HAL_DMA_ERROR_FE;              /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /* Direct Mode Error Interrupt management ***********************************/
  if ((tmpisr & (DMA_FLAG_DMEIF0_4 << hdma->StreamIndex)) != RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if(__HAL_DMA_GET_IT_SOURCE(hdma, DMA_IT_DME) != RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear the direct mode error flag */
      regs->IFCR = DMA_FLAG_DMEIF0_4 << hdma->StreamIndex;  /* 赋值或初始化 */

      /* Update error code */
      hdma->ErrorCode |= HAL_DMA_ERROR_DME;             /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /* Half Transfer Complete Interrupt management ******************************/
  if ((tmpisr & (DMA_FLAG_HTIF0_4 << hdma->StreamIndex)) != RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if(__HAL_DMA_GET_IT_SOURCE(hdma, DMA_IT_HT) != RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear the half transfer complete flag */
      regs->IFCR = DMA_FLAG_HTIF0_4 << hdma->StreamIndex;  /* 赋值或初始化 */

      /* Multi_Buffering mode enabled */
      if(((hdma->Instance->CR) & (uint32_t)(DMA_SxCR_DBM)) != RESET)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Current memory buffer used is Memory 0 */
        if((hdma->Instance->CR & DMA_SxCR_CT) == RESET)  /* 条件判断 */
        {                                               /* 代码块开始 */
          if(hdma->XferHalfCpltCallback != NULL)        /* 条件判断 */
          {                                             /* 代码块开始 */
            /* Half transfer callback */
            hdma->XferHalfCpltCallback(hdma);           /* 执行语句 */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */
        /* Current memory buffer used is Memory 1 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          if(hdma->XferM1HalfCpltCallback != NULL)      /* 条件判断 */
          {                                             /* 代码块开始 */
            /* Half transfer callback */
            hdma->XferM1HalfCpltCallback(hdma);         /* 执行语句 */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Disable the half transfer interrupt if the DMA mode is not CIRCULAR */
        if((hdma->Instance->CR & DMA_SxCR_CIRC) == RESET)  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Disable the half transfer interrupt */
          hdma->Instance->CR  &= ~(DMA_IT_HT);          /* 赋值或初始化 */
        }                                               /* 代码块结束 */

        if(hdma->XferHalfCpltCallback != NULL)          /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Half transfer callback */
          hdma->XferHalfCpltCallback(hdma);             /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /* Transfer Complete Interrupt management ***********************************/
  if ((tmpisr & (DMA_FLAG_TCIF0_4 << hdma->StreamIndex)) != RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if(__HAL_DMA_GET_IT_SOURCE(hdma, DMA_IT_TC) != RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear the transfer complete flag */
      regs->IFCR = DMA_FLAG_TCIF0_4 << hdma->StreamIndex;  /* 赋值或初始化 */

      if(HAL_DMA_STATE_ABORT == hdma->State)            /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Disable all the transfer interrupts */
        hdma->Instance->CR  &= ~(DMA_IT_TC | DMA_IT_TE | DMA_IT_DME);  /* 赋值或初始化 */
        hdma->Instance->FCR &= ~(DMA_IT_FE);            /* 赋值或初始化 */

        if((hdma->XferHalfCpltCallback != NULL) || (hdma->XferM1HalfCpltCallback != NULL))  /* 条件判断 */
        {                                               /* 代码块开始 */
          hdma->Instance->CR  &= ~(DMA_IT_HT);          /* 赋值或初始化 */
        }                                               /* 代码块结束 */

        /* Clear all interrupt flags at correct offset within the register */
        regs->IFCR = 0x3FU << hdma->StreamIndex;        /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hdma);                             /* HAL辅助宏/函数 */

        /* Change the DMA state */
        hdma->State = HAL_DMA_STATE_READY;              /* 赋值或初始化 */

        if(hdma->XferAbortCallback != NULL)             /* 条件判断 */
        {                                               /* 代码块开始 */
          hdma->XferAbortCallback(hdma);                /* 执行语句 */
        }                                               /* 代码块结束 */
        return;                                         /* 返回 */
      }                                                 /* 代码块结束 */

      if(((hdma->Instance->CR) & (uint32_t)(DMA_SxCR_DBM)) != RESET)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Current memory buffer used is Memory 0 */
        if((hdma->Instance->CR & DMA_SxCR_CT) == RESET)  /* 条件判断 */
        {                                               /* 代码块开始 */
          if(hdma->XferM1CpltCallback != NULL)          /* 条件判断 */
          {                                             /* 代码块开始 */
            /* Transfer complete Callback for memory1 */
            hdma->XferM1CpltCallback(hdma);             /* 执行语句 */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */
        /* Current memory buffer used is Memory 1 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          if(hdma->XferCpltCallback != NULL)            /* 条件判断 */
          {                                             /* 代码块开始 */
            /* Transfer complete Callback for memory0 */
            hdma->XferCpltCallback(hdma);               /* 执行语句 */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      /* Disable the transfer complete interrupt if the DMA mode is not CIRCULAR */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        if((hdma->Instance->CR & DMA_SxCR_CIRC) == RESET)  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Disable the transfer complete interrupt */
          hdma->Instance->CR  &= ~(DMA_IT_TC);          /* 赋值或初始化 */

          /* Process Unlocked */
          __HAL_UNLOCK(hdma);                           /* HAL辅助宏/函数 */

          /* Change the DMA state */
          hdma->State = HAL_DMA_STATE_READY;            /* 赋值或初始化 */
        }                                               /* 代码块结束 */

        if(hdma->XferCpltCallback != NULL)              /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Transfer complete callback */
          hdma->XferCpltCallback(hdma);                 /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* manage error case */
  if(hdma->ErrorCode != HAL_DMA_ERROR_NONE)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    if((hdma->ErrorCode & HAL_DMA_ERROR_TE) != RESET)   /* 条件判断 */
    {                                                   /* 代码块开始 */
      hdma->State = HAL_DMA_STATE_ABORT;                /* 赋值或初始化 */

      /* Disable the stream */
      __HAL_DMA_DISABLE(hdma);                          /* HAL辅助宏/函数 */

      do                                                /* do-while循环 */
      {                                                 /* 代码块开始 */
        if (++count > timeout)                          /* 条件判断 */
        {                                               /* 代码块开始 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      while((hdma->Instance->CR & DMA_SxCR_EN) != RESET);  /* while循环 */

      /* Process Unlocked */
      __HAL_UNLOCK(hdma);                               /* HAL辅助宏/函数 */

      /* Change the DMA state */
      hdma->State = HAL_DMA_STATE_READY;                /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    if(hdma->XferErrorCallback != NULL)                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Transfer error callback */
      hdma->XferErrorCallback(hdma);                    /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Register callbacks
  * @param  hdma                 pointer to a DMA_HandleTypeDef structure that contains
  *                               the configuration information for the specified DMA Stream.
  * @param  CallbackID           User Callback identifer
  *                               a DMA_HandleTypeDef structure as parameter.
  * @param  pCallback            pointer to private callbacsk function which has pointer to 
  *                               a DMA_HandleTypeDef structure as parameter.
  * @retval HAL status
  */                      
HAL_StatusTypeDef HAL_DMA_RegisterCallback(DMA_HandleTypeDef *hdma, HAL_DMA_CallbackIDTypeDef CallbackID, void (* pCallback)(DMA_HandleTypeDef *_hdma))  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */

  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Process locked */
  __HAL_LOCK(hdma);                                     /* HAL辅助宏/函数 */

  if(HAL_DMA_STATE_READY == hdma->State)                /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
    case  HAL_DMA_XFER_CPLT_CB_ID:                      /* case标签 */
      hdma->XferCpltCallback = pCallback;               /* 赋值或初始化 */
      break;                                            /* 跳出 */

    case  HAL_DMA_XFER_HALFCPLT_CB_ID:                  /* case标签 */
      hdma->XferHalfCpltCallback = pCallback;           /* 赋值或初始化 */
      break;                                            /* 跳出 */

    case  HAL_DMA_XFER_M1CPLT_CB_ID:                    /* case标签 */
      hdma->XferM1CpltCallback = pCallback;             /* 赋值或初始化 */
      break;                                            /* 跳出 */

    case  HAL_DMA_XFER_M1HALFCPLT_CB_ID:                /* case标签 */
      hdma->XferM1HalfCpltCallback = pCallback;         /* 赋值或初始化 */
      break;                                            /* 跳出 */

    case  HAL_DMA_XFER_ERROR_CB_ID:                     /* case标签 */
      hdma->XferErrorCallback = pCallback;              /* 赋值或初始化 */
      break;                                            /* 跳出 */

    case  HAL_DMA_XFER_ABORT_CB_ID:                     /* case标签 */
      hdma->XferAbortCallback = pCallback;              /* 赋值或初始化 */
      break;                                            /* 跳出 */

    default:                                            /* default分支 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hdma);                                   /* HAL辅助宏/函数 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  UnRegister callbacks
  * @param  hdma                 pointer to a DMA_HandleTypeDef structure that contains
  *                               the configuration information for the specified DMA Stream.
  * @param  CallbackID           User Callback identifer
  *                               a HAL_DMA_CallbackIDTypeDef ENUM as parameter.
  * @retval HAL status
  */              
HAL_StatusTypeDef HAL_DMA_UnRegisterCallback(DMA_HandleTypeDef *hdma, HAL_DMA_CallbackIDTypeDef CallbackID)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Process locked */
  __HAL_LOCK(hdma);                                     /* HAL辅助宏/函数 */

  if(HAL_DMA_STATE_READY == hdma->State)                /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
    case  HAL_DMA_XFER_CPLT_CB_ID:                      /* case标签 */
      hdma->XferCpltCallback = NULL;                    /* 赋值或初始化 */
      break;                                            /* 跳出 */

    case  HAL_DMA_XFER_HALFCPLT_CB_ID:                  /* case标签 */
      hdma->XferHalfCpltCallback = NULL;                /* 赋值或初始化 */
      break;                                            /* 跳出 */

    case  HAL_DMA_XFER_M1CPLT_CB_ID:                    /* case标签 */
      hdma->XferM1CpltCallback = NULL;                  /* 赋值或初始化 */
      break;                                            /* 跳出 */

    case  HAL_DMA_XFER_M1HALFCPLT_CB_ID:                /* case标签 */
      hdma->XferM1HalfCpltCallback = NULL;              /* 赋值或初始化 */
      break;                                            /* 跳出 */

    case  HAL_DMA_XFER_ERROR_CB_ID:                     /* case标签 */
      hdma->XferErrorCallback = NULL;                   /* 赋值或初始化 */
      break;                                            /* 跳出 */

    case  HAL_DMA_XFER_ABORT_CB_ID:                     /* case标签 */
      hdma->XferAbortCallback = NULL;                   /* 赋值或初始化 */
      break;                                            /* 跳出 */

    case   HAL_DMA_XFER_ALL_CB_ID:                      /* case标签 */
      hdma->XferCpltCallback = NULL;                    /* 赋值或初始化 */
      hdma->XferHalfCpltCallback = NULL;                /* 赋值或初始化 */
      hdma->XferM1CpltCallback = NULL;                  /* 赋值或初始化 */
      hdma->XferM1HalfCpltCallback = NULL;              /* 赋值或初始化 */
      hdma->XferErrorCallback = NULL;                   /* 赋值或初始化 */
      hdma->XferAbortCallback = NULL;                   /* 赋值或初始化 */
      break;                                            /* 跳出 */

    default:                                            /* default分支 */
      status = HAL_ERROR;                               /* 赋值或初始化 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    status = HAL_ERROR;                                 /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hdma);                                   /* HAL辅助宏/函数 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @addtogroup DMA_Exported_Functions_Group3
  *
@verbatim
 ===============================================================================
                    ##### State and Errors functions #####
 ===============================================================================
    [..]
    This subsection provides functions allowing to
      (+) Check the DMA state
      (+) Get error code

@endverbatim
  * @{
  */

/**
  * @brief  Returns the DMA state.
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA Stream.
  * @retval HAL state
  */
HAL_DMA_StateTypeDef HAL_DMA_GetState(DMA_HandleTypeDef *hdma)  /* HAL DMA操作 */
{                                                       /* 代码块开始 */
  return hdma->State;                                   /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return the DMA error code
  * @param  hdma  pointer to a DMA_HandleTypeDef structure that contains
  *              the configuration information for the specified DMA Stream.
  * @retval DMA Error Code
  */
uint32_t HAL_DMA_GetError(DMA_HandleTypeDef *hdma)      /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  return hdma->ErrorCode;                               /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup DMA_Private_Functions
  * @{
  */

/**
  * @brief  Sets the DMA Transfer parameter.
  * @param  hdma       pointer to a DMA_HandleTypeDef structure that contains
  *                     the configuration information for the specified DMA Stream.
  * @param  SrcAddress The source memory Buffer address
  * @param  DstAddress The destination memory Buffer address
  * @param  DataLength The length of data to be transferred from source to destination
  * @retval HAL status
  */
static void DMA_SetConfig(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength)  /* 静态函数 */
{                                                       /* 代码块开始 */
  /* Clear DBM bit */
  hdma->Instance->CR &= (uint32_t)(~DMA_SxCR_DBM);      /* 赋值或初始化 */

  /* Configure DMA Stream data length */
  hdma->Instance->NDTR = DataLength;                    /* 赋值或初始化 */

  /* Memory to Peripheral */
  if((hdma->Init.Direction) == DMA_MEMORY_TO_PERIPH)    /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Configure DMA Stream destination address */
    hdma->Instance->PAR = DstAddress;                   /* 赋值或初始化 */

    /* Configure DMA Stream source address */
    hdma->Instance->M0AR = SrcAddress;                  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  /* Peripheral to Memory */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Configure DMA Stream source address */
    hdma->Instance->PAR = SrcAddress;                   /* 赋值或初始化 */

    /* Configure DMA Stream destination address */
    hdma->Instance->M0AR = DstAddress;                  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Returns the DMA Stream base address depending on stream number
  * @param  hdma       pointer to a DMA_HandleTypeDef structure that contains
  *                     the configuration information for the specified DMA Stream. 
  * @retval Stream base address
  */
static uint32_t DMA_CalcBaseAndBitshift(DMA_HandleTypeDef *hdma)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t stream_number = (((uint32_t)hdma->Instance & 0xFFU) - 16U) / 24U;  /* 32位无符号变量/函数 */

  /* lookup table for necessary bitshift of flags within status registers */
  static const uint8_t flagBitshiftOffset[8U] = {0U, 6U, 16U, 22U, 0U, 6U, 16U, 22U};  /* 静态常量 */
  hdma->StreamIndex = flagBitshiftOffset[stream_number];  /* 赋值或初始化 */

  if (stream_number > 3U)                               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* return pointer to HISR and HIFCR */
    hdma->StreamBaseAddress = (((uint32_t)hdma->Instance & (uint32_t)(~0x3FFU)) + 4U);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* return pointer to LISR and LIFCR */
    hdma->StreamBaseAddress = ((uint32_t)hdma->Instance & (uint32_t)(~0x3FFU));  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return hdma->StreamBaseAddress;                       /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Check compatibility between FIFO threshold level and size of the memory burst
  * @param  hdma       pointer to a DMA_HandleTypeDef structure that contains
  *                     the configuration information for the specified DMA Stream. 
  * @retval HAL status
  */
static HAL_StatusTypeDef DMA_CheckFifoParam(DMA_HandleTypeDef *hdma)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */
  uint32_t tmp = hdma->Init.FIFOThreshold;              /* 32位无符号变量/函数 */

  /* Memory Data size equal to Byte */
  if(hdma->Init.MemDataAlignment == DMA_MDATAALIGN_BYTE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (tmp)                                        /* switch分支 */
    {                                                   /* 代码块开始 */
    case DMA_FIFO_THRESHOLD_1QUARTERFULL:               /* case标签 */
    case DMA_FIFO_THRESHOLD_3QUARTERSFULL:              /* case标签 */
      if ((hdma->Init.MemBurst & DMA_SxCR_MBURST_1) == DMA_SxCR_MBURST_1)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        status = HAL_ERROR;                             /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */
    case DMA_FIFO_THRESHOLD_HALFFULL:                   /* case标签 */
      if (hdma->Init.MemBurst == DMA_MBURST_INC16)      /* 条件判断 */
      {                                                 /* 代码块开始 */
        status = HAL_ERROR;                             /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */
    case DMA_FIFO_THRESHOLD_FULL:                       /* case标签 */
      break;                                            /* 跳出 */
    default:                                            /* default分支 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Memory Data size equal to Half-Word */
  else if (hdma->Init.MemDataAlignment == DMA_MDATAALIGN_HALFWORD)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (tmp)                                        /* switch分支 */
    {                                                   /* 代码块开始 */
    case DMA_FIFO_THRESHOLD_1QUARTERFULL:               /* case标签 */
    case DMA_FIFO_THRESHOLD_3QUARTERSFULL:              /* case标签 */
      status = HAL_ERROR;                               /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case DMA_FIFO_THRESHOLD_HALFFULL:                   /* case标签 */
      if ((hdma->Init.MemBurst & DMA_SxCR_MBURST_1) == DMA_SxCR_MBURST_1)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        status = HAL_ERROR;                             /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */
    case DMA_FIFO_THRESHOLD_FULL:                       /* case标签 */
      if (hdma->Init.MemBurst == DMA_MBURST_INC16)      /* 条件判断 */
      {                                                 /* 代码块开始 */
        status = HAL_ERROR;                             /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */
    default:                                            /* default分支 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Memory Data size equal to Word */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    switch (tmp)                                        /* switch分支 */
    {                                                   /* 代码块开始 */
    case DMA_FIFO_THRESHOLD_1QUARTERFULL:               /* case标签 */
    case DMA_FIFO_THRESHOLD_HALFFULL:                   /* case标签 */
    case DMA_FIFO_THRESHOLD_3QUARTERSFULL:              /* case标签 */
      status = HAL_ERROR;                               /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case DMA_FIFO_THRESHOLD_FULL:                       /* case标签 */
      if ((hdma->Init.MemBurst & DMA_SxCR_MBURST_1) == DMA_SxCR_MBURST_1)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        status = HAL_ERROR;                             /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */
    default:                                            /* default分支 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

#endif /* HAL_DMA_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
