/* 文件: stm32f4xx_hal_sdram.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_sdram.c
  * @author  MCD Application Team
  * @brief   SDRAM HAL module driver.
  *          This file provides a generic firmware to drive SDRAM memories mounted 
  *          as external device.
  *         
  @verbatim
  ==============================================================================
                       ##### How to use this driver #####
  ============================================================================== 
  [..]
    This driver is a generic layered driver which contains a set of APIs used to 
    control SDRAM memories. It uses the FMC layer functions to interface 
    with SDRAM devices.  
    The following sequence should be followed to configure the FMC to interface
    with SDRAM memories: 

   (#) Declare a SDRAM_HandleTypeDef handle structure, for example:
          SDRAM_HandleTypeDef  hdsram 

       (++) Fill the SDRAM_HandleTypeDef handle "Init" field with the allowed 
            values of the structure member.

       (++) Fill the SDRAM_HandleTypeDef handle "Instance" field with a predefined 
            base register instance for NOR or SDRAM device 

   (#) Declare a FMC_SDRAM_TimingTypeDef structure; for example:
          FMC_SDRAM_TimingTypeDef  Timing;
      and fill its fields with the allowed values of the structure member.

   (#) Initialize the SDRAM Controller by calling the function HAL_SDRAM_Init(). This function
       performs the following sequence:

       (##) MSP hardware layer configuration using the function HAL_SDRAM_MspInit()
       (##) Control register configuration using the FMC SDRAM interface function 
            FMC_SDRAM_Init()
       (##) Timing register configuration using the FMC SDRAM interface function 
            FMC_SDRAM_Timing_Init()
       (##) Program the SDRAM external device by applying its initialization sequence
            according to the device plugged in your hardware. This step is mandatory
            for accessing the SDRAM device.   

   (#) At this stage you can perform read/write accesses from/to the memory connected 
       to the SDRAM Bank. You can perform either polling or DMA transfer using the
       following APIs:
       (++) HAL_SDRAM_Read()/HAL_SDRAM_Write() for polling read/write access
       (++) HAL_SDRAM_Read_DMA()/HAL_SDRAM_Write_DMA() for DMA read/write transfer

   (#) You can also control the SDRAM device by calling the control APIs HAL_SDRAM_WriteOperation_Enable()/
       HAL_SDRAM_WriteOperation_Disable() to respectively enable/disable the SDRAM write operation or 
       the function HAL_SDRAM_SendCommand() to send a specified command to the SDRAM
       device. The command to be sent must be configured with the FMC_SDRAM_CommandTypeDef 
       structure.   

   (#) You can continuously monitor the SDRAM device HAL state by calling the function
       HAL_SDRAM_GetState()

   *** Callback registration ***
    =============================================
    [..]
      The compilation define  USE_HAL_SDRAM_REGISTER_CALLBACKS when set to 1
      allows the user to configure dynamically the driver callbacks.

      Use Functions @ref HAL_SDRAM_RegisterCallback() to register a user callback,
      it allows to register following callbacks:
        (+) MspInitCallback    : SDRAM MspInit.
        (+) MspDeInitCallback  : SDRAM MspDeInit.
      This function takes as parameters the HAL peripheral handle, the Callback ID
      and a pointer to the user callback function.

      Use function @ref HAL_SDRAM_UnRegisterCallback() to reset a callback to the default
      weak (surcharged) function. It allows to reset following callbacks:
        (+) MspInitCallback    : SDRAM MspInit.
        (+) MspDeInitCallback  : SDRAM MspDeInit.
      This function) takes as parameters the HAL peripheral handle and the Callback ID.

      By default, after the @ref HAL_SDRAM_Init and if the state is HAL_SDRAM_STATE_RESET
      all callbacks are reset to the corresponding legacy weak (surcharged) functions.
      Exception done for MspInit and MspDeInit callbacks that are respectively
      reset to the legacy weak (surcharged) functions in the @ref HAL_SDRAM_Init
      and @ref  HAL_SDRAM_DeInit only when these callbacks are null (not registered beforehand).
      If not, MspInit or MspDeInit are not null, the @ref HAL_SDRAM_Init and @ref HAL_SDRAM_DeInit
      keep and use the user MspInit/MspDeInit callbacks (registered beforehand)

      Callbacks can be registered/unregistered in READY state only.
      Exception done for MspInit/MspDeInit callbacks that can be registered/unregistered
      in READY or RESET state, thus registered (user) MspInit/DeInit callbacks can be used
      during the Init/DeInit.
      In that case first register the MspInit/MspDeInit user callbacks
      using @ref HAL_SDRAM_RegisterCallback before calling @ref HAL_SDRAM_DeInit
      or @ref HAL_SDRAM_Init function.

      When The compilation define USE_HAL_SDRAM_REGISTER_CALLBACKS is set to 0 or
      not defined, the callback registering feature is not available
      and weak (surcharged) callbacks are used.

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

/** @defgroup SDRAM SDRAM
  * @brief SDRAM driver modules
  * @{
  */
#ifdef HAL_SDRAM_MODULE_ENABLED                         /* 条件编译 */
#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx) ||\
    defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx)  /* 源代码 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/    
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/** @defgroup SDRAM_Exported_Functions SDRAM Exported Functions
  * @{
  */

/** @defgroup SDRAM_Exported_Functions_Group1 Initialization and de-initialization functions 
  * @brief    Initialization and Configuration functions 
  *
  @verbatim    
  ==============================================================================
           ##### SDRAM Initialization and de_initialization functions #####
  ==============================================================================
  [..]  
    This section provides functions allowing to initialize/de-initialize
    the SDRAM memory

@endverbatim
  * @{
  */

/**
  * @brief  Performs the SDRAM device initialization sequence.
  * @param  hsdram pointer to a SDRAM_HandleTypeDef structure that contains
  *                the configuration information for SDRAM module.
  * @param  Timing Pointer to SDRAM control timing structure 
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SDRAM_Init(SDRAM_HandleTypeDef *hsdram, FMC_SDRAM_TimingTypeDef *Timing)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the SDRAM handle parameter */
  if(hsdram == NULL)                                    /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  if(hsdram->State == HAL_SDRAM_STATE_RESET)            /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Allocate lock resource and initialize it */
    hsdram->Lock = HAL_UNLOCKED;                        /* 赋值或初始化 */
#if (USE_HAL_SDRAM_REGISTER_CALLBACKS == 1)             /* 条件编译 */
    if(hsdram->MspInitCallback == NULL)                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      hsdram->MspInitCallback = HAL_SDRAM_MspInit;      /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    hsdram->RefreshErrorCallback = HAL_SDRAM_RefreshErrorCallback;  /* 赋值或初始化 */
    hsdram->DmaXferCpltCallback = HAL_SDRAM_DMA_XferCpltCallback;  /* 赋值或初始化 */
    hsdram->DmaXferErrorCallback = HAL_SDRAM_DMA_XferErrorCallback;  /* 赋值或初始化 */

    /* Init the low level hardware */
    hsdram->MspInitCallback(hsdram);                    /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /* Initialize the low level hardware (MSP) */
    HAL_SDRAM_MspInit(hsdram);                          /* HAL库函数调用 */
#endif                                                  /* 条件编译结束 */
  }                                                     /* 代码块结束 */

  /* Initialize the SDRAM controller state */
  hsdram->State = HAL_SDRAM_STATE_BUSY;                 /* 赋值或初始化 */

  /* Initialize SDRAM control Interface */
  FMC_SDRAM_Init(hsdram->Instance, &(hsdram->Init));    /* 执行语句 */

  /* Initialize SDRAM timing Interface */
  FMC_SDRAM_Timing_Init(hsdram->Instance, Timing, hsdram->Init.SDBank);   /* 执行语句 */

  /* Update the SDRAM controller state */
  hsdram->State = HAL_SDRAM_STATE_READY;                /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Perform the SDRAM device initialization sequence.
  * @param  hsdram pointer to a SDRAM_HandleTypeDef structure that contains
  *                the configuration information for SDRAM module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SDRAM_DeInit(SDRAM_HandleTypeDef *hsdram)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
#if (USE_HAL_SDRAM_REGISTER_CALLBACKS == 1)             /* 条件编译 */
  if(hsdram->MspDeInitCallback == NULL)                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    hsdram->MspDeInitCallback = HAL_SDRAM_MspDeInit;    /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* DeInit the low level hardware */
  hsdram->MspDeInitCallback(hsdram);                    /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* Initialize the low level hardware (MSP) */
  HAL_SDRAM_MspDeInit(hsdram);                          /* HAL库函数调用 */
#endif                                                  /* 条件编译结束 */

  /* Configure the SDRAM registers with their reset values */
  FMC_SDRAM_DeInit(hsdram->Instance, hsdram->Init.SDBank);  /* 执行语句 */

  /* Reset the SDRAM controller state */
  hsdram->State = HAL_SDRAM_STATE_RESET;                /* 赋值或初始化 */

  /* Release Lock */
  __HAL_UNLOCK(hsdram);                                 /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  SDRAM MSP Init.
  * @param  hsdram pointer to a SDRAM_HandleTypeDef structure that contains
  *                the configuration information for SDRAM module.
  * @retval None
  */
__weak void HAL_SDRAM_MspInit(SDRAM_HandleTypeDef *hsdram)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsdram);                                       /* 执行语句 */
  /* NOTE: This function Should not be modified, when the callback is needed,
            the HAL_SDRAM_MspInit could be implemented in the user file
   */ 
}                                                       /* 代码块结束 */

/**
  * @brief  SDRAM MSP DeInit.
  * @param  hsdram pointer to a SDRAM_HandleTypeDef structure that contains
  *                the configuration information for SDRAM module.
  * @retval None
  */
__weak void HAL_SDRAM_MspDeInit(SDRAM_HandleTypeDef *hsdram)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsdram);                                       /* 执行语句 */
  /* NOTE: This function Should not be modified, when the callback is needed,
            the HAL_SDRAM_MspDeInit could be implemented in the user file
   */ 
}                                                       /* 代码块结束 */

/**
  * @brief  This function handles SDRAM refresh error interrupt request.
  * @param  hsdram pointer to a SDRAM_HandleTypeDef structure that contains
  *                the configuration information for SDRAM module.
  * @retval HAL status
*/
void HAL_SDRAM_IRQHandler(SDRAM_HandleTypeDef *hsdram)  /* 中断服务函数 */
{                                                       /* 代码块开始 */
  /* Check SDRAM interrupt Rising edge flag */
  if(__FMC_SDRAM_GET_FLAG(hsdram->Instance, FMC_SDRAM_FLAG_REFRESH_IT))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* SDRAM refresh error interrupt callback */
#if (USE_HAL_SDRAM_REGISTER_CALLBACKS == 1)             /* 条件编译 */
    hsdram->RefreshErrorCallback(hsdram);               /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_SDRAM_RefreshErrorCallback(hsdram);             /* HAL库函数调用 */
#endif                                                  /* 条件编译结束 */

    /* Clear SDRAM refresh error interrupt pending bit */
    __FMC_SDRAM_CLEAR_FLAG(hsdram->Instance, FMC_SDRAM_FLAG_REFRESH_ERROR);  /* 执行语句 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  SDRAM Refresh error callback.
  * @param  hsdram pointer to a SDRAM_HandleTypeDef structure that contains
  *                the configuration information for SDRAM module. 
  * @retval None
  */
__weak void HAL_SDRAM_RefreshErrorCallback(SDRAM_HandleTypeDef *hsdram)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsdram);                                       /* 执行语句 */
  /* NOTE: This function Should not be modified, when the callback is needed,
            the HAL_SDRAM_RefreshErrorCallback could be implemented in the user file
   */ 
}                                                       /* 代码块结束 */

/**
  * @brief  DMA transfer complete callback.
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *                the configuration information for the specified DMA module.
  * @retval None
  */
__weak void HAL_SDRAM_DMA_XferCpltCallback(DMA_HandleTypeDef *hdma)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdma);                                         /* 执行语句 */
  /* NOTE: This function Should not be modified, when the callback is needed,
            the HAL_SDRAM_DMA_XferCpltCallback could be implemented in the user file
   */ 
}                                                       /* 代码块结束 */

/**
  * @brief  DMA transfer complete error callback.
  * @param  hdma DMA handle
  * @retval None
  */
__weak void HAL_SDRAM_DMA_XferErrorCallback(DMA_HandleTypeDef *hdma)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdma);                                         /* 执行语句 */
  /* NOTE: This function Should not be modified, when the callback is needed,
            the HAL_SDRAM_DMA_XferErrorCallback could be implemented in the user file
   */ 
}                                                       /* 代码块结束 */
/**
  * @}
  */

/** @defgroup SDRAM_Exported_Functions_Group2 Input and Output functions 
  * @brief    Input Output and memory control functions 
  *
  @verbatim    
  ==============================================================================
                    ##### SDRAM Input and Output functions #####
  ==============================================================================
  [..]  
    This section provides functions allowing to use and control the SDRAM memory

@endverbatim
  * @{
  */

/**
  * @brief  Reads 8-bit data buffer from the SDRAM memory.
  * @param  hsdram pointer to a SDRAM_HandleTypeDef structure that contains
  *                the configuration information for SDRAM module.
  * @param  pAddress Pointer to read start address
  * @param  pDstBuffer Pointer to destination buffer  
  * @param  BufferSize Size of the buffer to read from memory
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SDRAM_Read_8b(SDRAM_HandleTypeDef *hsdram, uint32_t *pAddress, uint8_t *pDstBuffer, uint32_t BufferSize)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  __IO uint8_t *pSdramAddress = (uint8_t *)pAddress;    /* 赋值或初始化 */

  /* Process Locked */
  __HAL_LOCK(hsdram);                                   /* HAL辅助宏/函数 */

  /* Check the SDRAM controller state */
  if(hsdram->State == HAL_SDRAM_STATE_BUSY)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
  else if(hsdram->State == HAL_SDRAM_STATE_PRECHARGED)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    return  HAL_ERROR;                                  /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Read data from source */
  for(; BufferSize != 0U; BufferSize--)                 /* for循环 */
  {                                                     /* 代码块开始 */
    *pDstBuffer = *(__IO uint8_t *)pSdramAddress;       /* 赋值或初始化 */
    pDstBuffer++;                                       /* 执行语句 */
    pSdramAddress++;                                    /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Process Unlocked */
  __HAL_UNLOCK(hsdram);                                 /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Writes 8-bit data buffer to SDRAM memory.
  * @param  hsdram pointer to a SDRAM_HandleTypeDef structure that contains
  *                the configuration information for SDRAM module.
  * @param  pAddress Pointer to write start address
  * @param  pSrcBuffer Pointer to source buffer to write  
  * @param  BufferSize Size of the buffer to write to memory
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SDRAM_Write_8b(SDRAM_HandleTypeDef *hsdram, uint32_t *pAddress, uint8_t *pSrcBuffer, uint32_t BufferSize)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  __IO uint8_t *pSdramAddress = (uint8_t *)pAddress;    /* 赋值或初始化 */
  uint32_t tmp = 0U;                                    /* 32位无符号变量/函数 */

  /* Process Locked */
  __HAL_LOCK(hsdram);                                   /* HAL辅助宏/函数 */

  /* Check the SDRAM controller state */
  tmp = hsdram->State;                                  /* 赋值或初始化 */

  if(tmp == HAL_SDRAM_STATE_BUSY)                       /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
  else if((tmp == HAL_SDRAM_STATE_PRECHARGED) || (tmp == HAL_SDRAM_STATE_WRITE_PROTECTED))  /* 否则如果 */
  {                                                     /* 代码块开始 */
    return  HAL_ERROR;                                  /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Write data to memory */
  for(; BufferSize != 0U; BufferSize--)                 /* for循环 */
  {                                                     /* 代码块开始 */
    *(__IO uint8_t *)pSdramAddress = *pSrcBuffer;       /* 赋值或初始化 */
    pSrcBuffer++;                                       /* 执行语句 */
    pSdramAddress++;                                    /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Process Unlocked */
  __HAL_UNLOCK(hsdram);                                 /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Reads 16-bit data buffer from the SDRAM memory. 
  * @param  hsdram pointer to a SDRAM_HandleTypeDef structure that contains
  *                the configuration information for SDRAM module.
  * @param  pAddress Pointer to read start address
  * @param  pDstBuffer Pointer to destination buffer  
  * @param  BufferSize Size of the buffer to read from memory
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SDRAM_Read_16b(SDRAM_HandleTypeDef *hsdram, uint32_t *pAddress, uint16_t *pDstBuffer, uint32_t BufferSize)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  __IO uint16_t *pSdramAddress = (uint16_t *)pAddress;  /* 赋值或初始化 */

  /* Process Locked */
  __HAL_LOCK(hsdram);                                   /* HAL辅助宏/函数 */

  /* Check the SDRAM controller state */
  if(hsdram->State == HAL_SDRAM_STATE_BUSY)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
  else if(hsdram->State == HAL_SDRAM_STATE_PRECHARGED)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    return  HAL_ERROR;                                  /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Read data from source */
  for(; BufferSize != 0U; BufferSize--)                 /* for循环 */
  {                                                     /* 代码块开始 */
    *pDstBuffer = *(__IO uint16_t *)pSdramAddress;      /* 赋值或初始化 */
    pDstBuffer++;                                       /* 执行语句 */
    pSdramAddress++;                                    /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Process Unlocked */
  __HAL_UNLOCK(hsdram);                                 /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Writes 16-bit data buffer to SDRAM memory. 
  * @param  hsdram pointer to a SDRAM_HandleTypeDef structure that contains
  *                the configuration information for SDRAM module.
  * @param  pAddress Pointer to write start address
  * @param  pSrcBuffer Pointer to source buffer to write  
  * @param  BufferSize Size of the buffer to write to memory
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SDRAM_Write_16b(SDRAM_HandleTypeDef *hsdram, uint32_t *pAddress, uint16_t *pSrcBuffer, uint32_t BufferSize)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  __IO uint16_t *pSdramAddress = (uint16_t *)pAddress;  /* 赋值或初始化 */
  uint32_t tmp = 0U;                                    /* 32位无符号变量/函数 */

  /* Process Locked */
  __HAL_LOCK(hsdram);                                   /* HAL辅助宏/函数 */

  /* Check the SDRAM controller state */
  tmp = hsdram->State;                                  /* 赋值或初始化 */

  if(tmp == HAL_SDRAM_STATE_BUSY)                       /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
  else if((tmp == HAL_SDRAM_STATE_PRECHARGED) || (tmp == HAL_SDRAM_STATE_WRITE_PROTECTED))  /* 否则如果 */
  {                                                     /* 代码块开始 */
    return  HAL_ERROR;                                  /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Write data to memory */
  for(; BufferSize != 0U; BufferSize--)                 /* for循环 */
  {                                                     /* 代码块开始 */
    *(__IO uint16_t *)pSdramAddress = *pSrcBuffer;      /* 赋值或初始化 */
    pSrcBuffer++;                                       /* 执行语句 */
    pSdramAddress++;                                    /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Process Unlocked */
  __HAL_UNLOCK(hsdram);                                 /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Reads 32-bit data buffer from the SDRAM memory. 
  * @param  hsdram pointer to a SDRAM_HandleTypeDef structure that contains
  *                the configuration information for SDRAM module.
  * @param  pAddress Pointer to read start address
  * @param  pDstBuffer Pointer to destination buffer  
  * @param  BufferSize Size of the buffer to read from memory
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SDRAM_Read_32b(SDRAM_HandleTypeDef *hsdram, uint32_t *pAddress, uint32_t *pDstBuffer, uint32_t BufferSize)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  __IO uint32_t *pSdramAddress = (uint32_t *)pAddress;  /* 赋值或初始化 */

  /* Process Locked */
  __HAL_LOCK(hsdram);                                   /* HAL辅助宏/函数 */

  /* Check the SDRAM controller state */
  if(hsdram->State == HAL_SDRAM_STATE_BUSY)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
  else if(hsdram->State == HAL_SDRAM_STATE_PRECHARGED)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    return  HAL_ERROR;                                  /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Read data from source */
  for(; BufferSize != 0U; BufferSize--)                 /* for循环 */
  {                                                     /* 代码块开始 */
    *pDstBuffer = *(__IO uint32_t *)pSdramAddress;      /* 赋值或初始化 */
    pDstBuffer++;                                       /* 执行语句 */
    pSdramAddress++;                                    /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Process Unlocked */
  __HAL_UNLOCK(hsdram);                                 /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Writes 32-bit data buffer to SDRAM memory. 
  * @param  hsdram pointer to a SDRAM_HandleTypeDef structure that contains
  *                the configuration information for SDRAM module.
  * @param  pAddress Pointer to write start address
  * @param  pSrcBuffer Pointer to source buffer to write  
  * @param  BufferSize Size of the buffer to write to memory
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SDRAM_Write_32b(SDRAM_HandleTypeDef *hsdram, uint32_t *pAddress, uint32_t *pSrcBuffer, uint32_t BufferSize)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  __IO uint32_t *pSdramAddress = (uint32_t *)pAddress;  /* 赋值或初始化 */
  uint32_t tmp = 0U;                                    /* 32位无符号变量/函数 */

  /* Process Locked */
  __HAL_LOCK(hsdram);                                   /* HAL辅助宏/函数 */

  /* Check the SDRAM controller state */
  tmp = hsdram->State;                                  /* 赋值或初始化 */

  if(tmp == HAL_SDRAM_STATE_BUSY)                       /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
  else if((tmp == HAL_SDRAM_STATE_PRECHARGED) || (tmp == HAL_SDRAM_STATE_WRITE_PROTECTED))  /* 否则如果 */
  {                                                     /* 代码块开始 */
    return  HAL_ERROR;                                  /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Write data to memory */
  for(; BufferSize != 0U; BufferSize--)                 /* for循环 */
  {                                                     /* 代码块开始 */
    *(__IO uint32_t *)pSdramAddress = *pSrcBuffer;      /* 赋值或初始化 */
    pSrcBuffer++;                                       /* 执行语句 */
    pSdramAddress++;                                    /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Process Unlocked */
  __HAL_UNLOCK(hsdram);                                 /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Reads a Words data from the SDRAM memory using DMA transfer. 
  * @param  hsdram pointer to a SDRAM_HandleTypeDef structure that contains
  *                the configuration information for SDRAM module.
  * @param  pAddress Pointer to read start address
  * @param  pDstBuffer Pointer to destination buffer  
  * @param  BufferSize Size of the buffer to read from memory
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SDRAM_Read_DMA(SDRAM_HandleTypeDef *hsdram, uint32_t *pAddress, uint32_t *pDstBuffer, uint32_t BufferSize)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmp = 0U;                                    /* 32位无符号变量/函数 */

  /* Process Locked */
  __HAL_LOCK(hsdram);                                   /* HAL辅助宏/函数 */

  /* Check the SDRAM controller state */  
  tmp = hsdram->State;                                  /* 赋值或初始化 */

  if(tmp == HAL_SDRAM_STATE_BUSY)                       /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
  else if(tmp == HAL_SDRAM_STATE_PRECHARGED)            /* 否则如果 */
  {                                                     /* 代码块开始 */
    return  HAL_ERROR;                                  /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Configure DMA user callbacks */
  hsdram->hdma->XferCpltCallback  = HAL_SDRAM_DMA_XferCpltCallback;  /* 赋值或初始化 */
  hsdram->hdma->XferErrorCallback = HAL_SDRAM_DMA_XferErrorCallback;  /* 赋值或初始化 */

  /* Enable the DMA Stream */
  HAL_DMA_Start_IT(hsdram->hdma, (uint32_t)pAddress, (uint32_t)pDstBuffer, (uint32_t)BufferSize);  /* HAL DMA操作 */

  /* Process Unlocked */
  __HAL_UNLOCK(hsdram);                                 /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Writes a Words data buffer to SDRAM memory using DMA transfer.
  * @param  hsdram pointer to a SDRAM_HandleTypeDef structure that contains
  *                the configuration information for SDRAM module.
  * @param  pAddress Pointer to write start address
  * @param  pSrcBuffer Pointer to source buffer to write  
  * @param  BufferSize Size of the buffer to write to memory
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SDRAM_Write_DMA(SDRAM_HandleTypeDef *hsdram, uint32_t *pAddress, uint32_t *pSrcBuffer, uint32_t BufferSize)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmp = 0U;                                    /* 32位无符号变量/函数 */

  /* Process Locked */
  __HAL_LOCK(hsdram);                                   /* HAL辅助宏/函数 */

  /* Check the SDRAM controller state */  
  tmp = hsdram->State;                                  /* 赋值或初始化 */

  if(tmp == HAL_SDRAM_STATE_BUSY)                       /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
  else if((tmp == HAL_SDRAM_STATE_PRECHARGED) || (tmp == HAL_SDRAM_STATE_WRITE_PROTECTED))  /* 否则如果 */
  {                                                     /* 代码块开始 */
    return  HAL_ERROR;                                  /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Configure DMA user callbacks */
  hsdram->hdma->XferCpltCallback  = HAL_SDRAM_DMA_XferCpltCallback;  /* 赋值或初始化 */
  hsdram->hdma->XferErrorCallback = HAL_SDRAM_DMA_XferErrorCallback;  /* 赋值或初始化 */

  /* Enable the DMA Stream */
  HAL_DMA_Start_IT(hsdram->hdma, (uint32_t)pSrcBuffer, (uint32_t)pAddress, (uint32_t)BufferSize);  /* HAL DMA操作 */

  /* Process Unlocked */
  __HAL_UNLOCK(hsdram);                                 /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

#if (USE_HAL_SDRAM_REGISTER_CALLBACKS == 1)             /* 条件编译 */
/**
  * @brief  Register a User SDRAM Callback
  *         To be used instead of the weak (surcharged) predefined callback
  * @param hsdram : SDRAM handle
  * @param CallbackId : ID of the callback to be registered
  *        This parameter can be one of the following values:
  *          @arg @ref HAL_SDRAM_MSP_INIT_CB_ID       SDRAM MspInit callback ID
  *          @arg @ref HAL_SDRAM_MSP_DEINIT_CB_ID     SDRAM MspDeInit callback ID
  *          @arg @ref HAL_SDRAM_REFRESH_ERR_CB_ID    SDRAM Refresh Error callback ID
  * @param pCallback : pointer to the Callback function
  * @retval status
  */
HAL_StatusTypeDef HAL_SDRAM_RegisterCallback (SDRAM_HandleTypeDef *hsdram, HAL_SDRAM_CallbackIDTypeDef CallbackId, pSDRAM_CallbackTypeDef pCallback)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */
  HAL_SDRAM_StateTypeDef state;                         /* HAL库函数调用 */

  if(pCallback == NULL)                                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Process locked */
  __HAL_LOCK(hsdram);                                   /* HAL辅助宏/函数 */

  state = hsdram->State;                                /* 赋值或初始化 */
  if((state == HAL_SDRAM_STATE_READY) || (state == HAL_SDRAM_STATE_WRITE_PROTECTED))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackId)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
    case HAL_SDRAM_MSP_INIT_CB_ID :                     /* case标签 */
      hsdram->MspInitCallback = pCallback;              /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_SDRAM_MSP_DEINIT_CB_ID :                   /* case标签 */
      hsdram->MspDeInitCallback = pCallback;            /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_SDRAM_REFRESH_ERR_CB_ID :                  /* case标签 */
      hsdram->RefreshErrorCallback = pCallback;         /* 赋值或初始化 */
      break;                                            /* 跳出 */
    default :                                           /* default分支 */
      /* update return status */
      status =  HAL_ERROR;                              /* 赋值或初始化 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if(hsdram->State == HAL_SDRAM_STATE_RESET)       /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackId)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
    case HAL_SDRAM_MSP_INIT_CB_ID :                     /* case标签 */
      hsdram->MspInitCallback = pCallback;              /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_SDRAM_MSP_DEINIT_CB_ID :                   /* case标签 */
      hsdram->MspDeInitCallback = pCallback;            /* 赋值或初始化 */
      break;                                            /* 跳出 */
    default :                                           /* default分支 */
      /* update return status */
      status =  HAL_ERROR;                              /* 赋值或初始化 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* update return status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hsdram);                                 /* HAL辅助宏/函数 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Unregister a User SDRAM Callback
  *         SDRAM Callback is redirected to the weak (surcharged) predefined callback
  * @param hsdram : SDRAM handle
  * @param CallbackId : ID of the callback to be unregistered
  *        This parameter can be one of the following values:
  *          @arg @ref HAL_SDRAM_MSP_INIT_CB_ID       SDRAM MspInit callback ID
  *          @arg @ref HAL_SDRAM_MSP_DEINIT_CB_ID     SDRAM MspDeInit callback ID
  *          @arg @ref HAL_SDRAM_REFRESH_ERR_CB_ID    SDRAM Refresh Error callback ID
  *          @arg @ref HAL_SDRAM_DMA_XFER_CPLT_CB_ID  SDRAM DMA Xfer Complete callback ID
  *          @arg @ref HAL_SDRAM_DMA_XFER_ERR_CB_ID   SDRAM DMA Xfer Error callback ID
  * @retval status
  */
HAL_StatusTypeDef HAL_SDRAM_UnRegisterCallback (SDRAM_HandleTypeDef *hsdram, HAL_SDRAM_CallbackIDTypeDef CallbackId)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */
  HAL_SDRAM_StateTypeDef state;                         /* HAL库函数调用 */

  /* Process locked */
  __HAL_LOCK(hsdram);                                   /* HAL辅助宏/函数 */

  state = hsdram->State;                                /* 赋值或初始化 */
  if((state == HAL_SDRAM_STATE_READY) || (state == HAL_SDRAM_STATE_WRITE_PROTECTED))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackId)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
    case HAL_SDRAM_MSP_INIT_CB_ID :                     /* case标签 */
      hsdram->MspInitCallback = HAL_SDRAM_MspInit;      /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_SDRAM_MSP_DEINIT_CB_ID :                   /* case标签 */
      hsdram->MspDeInitCallback = HAL_SDRAM_MspDeInit;  /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_SDRAM_REFRESH_ERR_CB_ID :                  /* case标签 */
      hsdram->RefreshErrorCallback = HAL_SDRAM_RefreshErrorCallback;  /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_SDRAM_DMA_XFER_CPLT_CB_ID :                /* case标签 */
      hsdram->DmaXferCpltCallback = HAL_SDRAM_DMA_XferCpltCallback;  /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_SDRAM_DMA_XFER_ERR_CB_ID :                 /* case标签 */
      hsdram->DmaXferErrorCallback = HAL_SDRAM_DMA_XferErrorCallback;  /* 赋值或初始化 */
      break;                                            /* 跳出 */
    default :                                           /* default分支 */
      /* update return status */
      status =  HAL_ERROR;                              /* 赋值或初始化 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if(hsdram->State == HAL_SDRAM_STATE_RESET)       /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackId)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
    case HAL_SDRAM_MSP_INIT_CB_ID :                     /* case标签 */
      hsdram->MspInitCallback = HAL_SDRAM_MspInit;      /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_SDRAM_MSP_DEINIT_CB_ID :                   /* case标签 */
      hsdram->MspDeInitCallback = HAL_SDRAM_MspDeInit;  /* 赋值或初始化 */
      break;                                            /* 跳出 */
    default :                                           /* default分支 */
      /* update return status */
      status =  HAL_ERROR;                              /* 赋值或初始化 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* update return status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hsdram);                                 /* HAL辅助宏/函数 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Register a User SDRAM Callback for DMA transfers
  *         To be used instead of the weak (surcharged) predefined callback
  * @param hsdram : SDRAM handle
  * @param CallbackId : ID of the callback to be registered
  *        This parameter can be one of the following values:
  *          @arg @ref HAL_SDRAM_DMA_XFER_CPLT_CB_ID  SDRAM DMA Xfer Complete callback ID
  *          @arg @ref HAL_SDRAM_DMA_XFER_ERR_CB_ID   SDRAM DMA Xfer Error callback ID
  * @param pCallback : pointer to the Callback function
  * @retval status
  */
HAL_StatusTypeDef HAL_SDRAM_RegisterDmaCallback(SDRAM_HandleTypeDef *hsdram, HAL_SDRAM_CallbackIDTypeDef CallbackId, pSDRAM_DmaCallbackTypeDef pCallback)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */
  HAL_SDRAM_StateTypeDef state;                         /* HAL库函数调用 */

  if(pCallback == NULL)                                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Process locked */
  __HAL_LOCK(hsdram);                                   /* HAL辅助宏/函数 */

  state = hsdram->State;                                /* 赋值或初始化 */
  if((state == HAL_SDRAM_STATE_READY) || (state == HAL_SDRAM_STATE_WRITE_PROTECTED))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackId)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
    case HAL_SDRAM_DMA_XFER_CPLT_CB_ID :                /* case标签 */
      hsdram->DmaXferCpltCallback = pCallback;          /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_SDRAM_DMA_XFER_ERR_CB_ID :                 /* case标签 */
      hsdram->DmaXferErrorCallback = pCallback;         /* 赋值或初始化 */
      break;                                            /* 跳出 */
    default :                                           /* default分支 */
      /* update return status */
      status =  HAL_ERROR;                              /* 赋值或初始化 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* update return status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hsdram);                                 /* HAL辅助宏/函数 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */
#endif                                                  /* 条件编译结束 */

/**
  * @}
  */

/** @defgroup SDRAM_Exported_Functions_Group3 Control functions 
 *  @brief   management functions 
 *
@verbatim   
  ==============================================================================
                         ##### SDRAM Control functions #####
  ==============================================================================  
  [..]
    This subsection provides a set of functions allowing to control dynamically
    the SDRAM interface.

@endverbatim
  * @{
  */

/**
  * @brief  Enables dynamically SDRAM write protection.
  * @param  hsdram pointer to a SDRAM_HandleTypeDef structure that contains
  *                the configuration information for SDRAM module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SDRAM_WriteProtection_Enable(SDRAM_HandleTypeDef *hsdram)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the SDRAM controller state */ 
  if(hsdram->State == HAL_SDRAM_STATE_BUSY)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Update the SDRAM state */
  hsdram->State = HAL_SDRAM_STATE_BUSY;                 /* 赋值或初始化 */

  /* Enable write protection */
  FMC_SDRAM_WriteProtection_Enable(hsdram->Instance, hsdram->Init.SDBank);  /* 执行语句 */

  /* Update the SDRAM state */
  hsdram->State = HAL_SDRAM_STATE_WRITE_PROTECTED;      /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Disables dynamically SDRAM write protection.
  * @param  hsdram pointer to a SDRAM_HandleTypeDef structure that contains
  *                the configuration information for SDRAM module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SDRAM_WriteProtection_Disable(SDRAM_HandleTypeDef *hsdram)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the SDRAM controller state */
  if(hsdram->State == HAL_SDRAM_STATE_BUSY)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Update the SDRAM state */
  hsdram->State = HAL_SDRAM_STATE_BUSY;                 /* 赋值或初始化 */

  /* Disable write protection */
  FMC_SDRAM_WriteProtection_Disable(hsdram->Instance, hsdram->Init.SDBank);  /* 执行语句 */

  /* Update the SDRAM state */
  hsdram->State = HAL_SDRAM_STATE_READY;                /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Sends Command to the SDRAM bank.
  * @param  hsdram pointer to a SDRAM_HandleTypeDef structure that contains
  *                the configuration information for SDRAM module.
  * @param  Command SDRAM command structure
  * @param  Timeout Timeout duration
  * @retval HAL status
  */  
HAL_StatusTypeDef HAL_SDRAM_SendCommand(SDRAM_HandleTypeDef *hsdram, FMC_SDRAM_CommandTypeDef *Command, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the SDRAM controller state */
  if(hsdram->State == HAL_SDRAM_STATE_BUSY)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Update the SDRAM state */
  hsdram->State = HAL_SDRAM_STATE_BUSY;                 /* 赋值或初始化 */

  /* Send SDRAM command */
  FMC_SDRAM_SendCommand(hsdram->Instance, Command, Timeout);  /* 执行语句 */

  /* Update the SDRAM controller state */
  if(Command->CommandMode == FMC_SDRAM_CMD_PALL)        /* 条件判断 */
  {                                                     /* 代码块开始 */
    hsdram->State = HAL_SDRAM_STATE_PRECHARGED;         /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    hsdram->State = HAL_SDRAM_STATE_READY;              /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Programs the SDRAM Memory Refresh rate.
  * @param  hsdram pointer to a SDRAM_HandleTypeDef structure that contains
  *                the configuration information for SDRAM module.  
  * @param  RefreshRate The SDRAM refresh rate value       
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SDRAM_ProgramRefreshRate(SDRAM_HandleTypeDef *hsdram, uint32_t RefreshRate)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the SDRAM controller state */
  if(hsdram->State == HAL_SDRAM_STATE_BUSY)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Update the SDRAM state */
  hsdram->State = HAL_SDRAM_STATE_BUSY;                 /* 赋值或初始化 */

  /* Program the refresh rate */
  FMC_SDRAM_ProgramRefreshRate(hsdram->Instance ,RefreshRate);  /* 执行语句 */

  /* Update the SDRAM state */
  hsdram->State = HAL_SDRAM_STATE_READY;                /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Sets the Number of consecutive SDRAM Memory auto Refresh commands.
  * @param  hsdram pointer to a SDRAM_HandleTypeDef structure that contains
  *                the configuration information for SDRAM module.  
  * @param  AutoRefreshNumber The SDRAM auto Refresh number       
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SDRAM_SetAutoRefreshNumber(SDRAM_HandleTypeDef *hsdram, uint32_t AutoRefreshNumber)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the SDRAM controller state */
  if(hsdram->State == HAL_SDRAM_STATE_BUSY)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Update the SDRAM state */
  hsdram->State = HAL_SDRAM_STATE_BUSY;                 /* 赋值或初始化 */

  /* Set the Auto-Refresh number */
  FMC_SDRAM_SetAutoRefreshNumber(hsdram->Instance ,AutoRefreshNumber);  /* 执行语句 */

  /* Update the SDRAM state */
  hsdram->State = HAL_SDRAM_STATE_READY;                /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Returns the SDRAM memory current mode.
  * @param  hsdram pointer to a SDRAM_HandleTypeDef structure that contains
  *                the configuration information for SDRAM module.
  * @retval The SDRAM memory mode.        
  */
uint32_t HAL_SDRAM_GetModeStatus(SDRAM_HandleTypeDef *hsdram)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  /* Return the SDRAM memory current mode */
  return(FMC_SDRAM_GetModeStatus(hsdram->Instance, hsdram->Init.SDBank));  /* 执行语句 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup SDRAM_Exported_Functions_Group4 State functions 
 *  @brief   Peripheral State functions 
 *
@verbatim   
  ==============================================================================
                      ##### SDRAM State functions #####
  ==============================================================================  
  [..]
    This subsection permits to get in run-time the status of the SDRAM controller 
    and the data flow.

@endverbatim
  * @{
  */

/**
  * @brief  Returns the SDRAM state.
  * @param  hsdram pointer to a SDRAM_HandleTypeDef structure that contains
  *                the configuration information for SDRAM module.
  * @retval HAL state
  */
HAL_SDRAM_StateTypeDef HAL_SDRAM_GetState(SDRAM_HandleTypeDef *hsdram)  /* HAL库函数调用 */
{                                                       /* 代码块开始 */
  return hsdram->State;                                 /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */    

/**
  * @}
  */
#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx || STM32F446xx || STM32F469xx || STM32F479xx */
#endif /* HAL_SDRAM_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
