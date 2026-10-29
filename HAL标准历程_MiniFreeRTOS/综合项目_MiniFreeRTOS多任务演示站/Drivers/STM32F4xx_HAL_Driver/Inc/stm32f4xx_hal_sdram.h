/* 文件: stm32f4xx_hal_sdram.h | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_sdram.h
  * @author  MCD Application Team
  * @brief   Header file of SDRAM HAL module.
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
#ifndef __STM32F4xx_HAL_SDRAM_H                         /* 防止头文件重复包含 */
#define __STM32F4xx_HAL_SDRAM_H                         /* 定义包含保护宏 */

#ifdef __cplusplus                                      /* 条件编译 */
 extern "C" {                                           /* 外部符号声明 */
#endif                                                  /* 条件编译结束 */

#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx) ||\
    defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx)  /* 源代码 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_ll_fmc.h"                           /* 包含 STM32/HAL 头文件 */

/** @addtogroup STM32F4xx_HAL_Driver
  * @{
  */

/** @addtogroup SDRAM
  * @{
  */ 

/* Exported typedef ----------------------------------------------------------*/   
/** @defgroup SDRAM_Exported_Types SDRAM Exported Types
  * @{
  */

/** 
  * @brief  HAL SDRAM State structure definition  
  */ 
typedef enum                                            /* 类型定义 */
{                                                       /* 代码块开始 */
  HAL_SDRAM_STATE_RESET             = 0x00U,  /*!< SDRAM not yet initialized or disabled */
  HAL_SDRAM_STATE_READY             = 0x01U,  /*!< SDRAM initialized and ready for use   */
  HAL_SDRAM_STATE_BUSY              = 0x02U,  /*!< SDRAM internal process is ongoing     */
  HAL_SDRAM_STATE_ERROR             = 0x03U,  /*!< SDRAM error state                     */
  HAL_SDRAM_STATE_WRITE_PROTECTED   = 0x04U,  /*!< SDRAM device write protected          */
  HAL_SDRAM_STATE_PRECHARGED        = 0x05U   /*!< SDRAM device precharged               */

} HAL_SDRAM_StateTypeDef;                               /* 代码块结束 */

/**
  * @brief  SDRAM handle Structure definition
  */
#if (USE_HAL_SDRAM_REGISTER_CALLBACKS == 1)             /* 条件编译 */
typedef struct __SDRAM_HandleTypeDef                    /* 类型定义 */
#else                                                   /* 条件编译否则 */
typedef struct                                          /* 类型定义 */
#endif /* USE_HAL_SDRAM_REGISTER_CALLBACKS  */
{                                                       /* 代码块开始 */
  FMC_SDRAM_TypeDef             *Instance;  /*!< Register base address                 */

  FMC_SDRAM_InitTypeDef         Init;       /*!< SDRAM device configuration parameters */

  __IO HAL_SDRAM_StateTypeDef   State;      /*!< SDRAM access state                    */

  HAL_LockTypeDef               Lock;       /*!< SDRAM locking object                  */ 

  DMA_HandleTypeDef             *hdma;      /*!< Pointer DMA handler                   */

#if (USE_HAL_SDRAM_REGISTER_CALLBACKS == 1)             /* 条件编译 */
  void  (* MspInitCallback)        ( struct __SDRAM_HandleTypeDef * hsdram);    /*!< SDRAM Msp Init callback              */
  void  (* MspDeInitCallback)      ( struct __SDRAM_HandleTypeDef * hsdram);    /*!< SDRAM Msp DeInit callback            */
  void  (* RefreshErrorCallback)   ( struct __SDRAM_HandleTypeDef * hsdram);    /*!< SDRAM Refresh Error callback         */
  void  (* DmaXferCpltCallback)    ( DMA_HandleTypeDef * hdma);                 /*!< SDRAM DMA Xfer Complete callback     */
  void  (* DmaXferErrorCallback)   ( DMA_HandleTypeDef * hdma);                 /*!< SDRAM DMA Xfer Error callback        */
#endif                                                  /* 条件编译结束 */
} SDRAM_HandleTypeDef;                                  /* 代码块结束 */

#if (USE_HAL_SDRAM_REGISTER_CALLBACKS == 1)             /* 条件编译 */
/**
  * @brief  HAL SDRAM Callback ID enumeration definition
  */
typedef enum                                            /* 类型定义 */
{                                                       /* 代码块开始 */
  HAL_SDRAM_MSP_INIT_CB_ID       = 0x00U,  /*!< SDRAM MspInit Callback ID           */
  HAL_SDRAM_MSP_DEINIT_CB_ID     = 0x01U,  /*!< SDRAM MspDeInit Callback ID         */
  HAL_SDRAM_REFRESH_ERR_CB_ID    = 0x02U,  /*!< SDRAM Refresh Error Callback ID     */
  HAL_SDRAM_DMA_XFER_CPLT_CB_ID  = 0x03U,  /*!< SDRAM DMA Xfer Complete Callback ID */
  HAL_SDRAM_DMA_XFER_ERR_CB_ID   = 0x04U   /*!< SDRAM DMA Xfer Error Callback ID    */
}HAL_SDRAM_CallbackIDTypeDef;                           /* 代码块结束 */

/**
  * @brief  HAL SDRAM Callback pointer definition
  */
typedef void (*pSDRAM_CallbackTypeDef)(SDRAM_HandleTypeDef *hsdram);  /* 类型定义 */
typedef void (*pSDRAM_DmaCallbackTypeDef)(DMA_HandleTypeDef *hdma);  /* 类型定义 */
#endif                                                  /* 条件编译结束 */
/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/** @defgroup SDRAM_Exported_Macros SDRAM Exported Macros
  * @{
  */

/** @brief Reset SDRAM handle state
  * @param  __HANDLE__ specifies the SDRAM handle.
  * @retval None
  */
#if (USE_HAL_SDRAM_REGISTER_CALLBACKS == 1)             /* 条件编译 */
#define __HAL_SDRAM_RESET_HANDLE_STATE(__HANDLE__)        do {                                               \
                                                               (__HANDLE__)->State = HAL_SDRAM_STATE_RESET;  \
                                                               (__HANDLE__)->MspInitCallback = NULL;         \
                                                               (__HANDLE__)->MspDeInitCallback = NULL;       \
                                                             } while(0)  /* 代码块结束 */
#else                                                   /* 条件编译否则 */
#define __HAL_SDRAM_RESET_HANDLE_STATE(__HANDLE__) ((__HANDLE__)->State = HAL_SDRAM_STATE_RESET)  /* 定义包含保护宏 */
#endif                                                  /* 条件编译结束 */
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup SDRAM_Exported_Functions SDRAM Exported Functions
  * @{
  */

/** @addtogroup SDRAM_Exported_Functions_Group1 
  * @{
  */

/* Initialization/de-initialization functions *********************************/
HAL_StatusTypeDef HAL_SDRAM_Init(SDRAM_HandleTypeDef *hsdram, FMC_SDRAM_TimingTypeDef *Timing);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_SDRAM_DeInit(SDRAM_HandleTypeDef *hsdram);  /* HAL状态类型函数 */
void HAL_SDRAM_MspInit(SDRAM_HandleTypeDef *hsdram);    /* 函数定义或声明 */
void HAL_SDRAM_MspDeInit(SDRAM_HandleTypeDef *hsdram);  /* 函数定义或声明 */

void HAL_SDRAM_IRQHandler(SDRAM_HandleTypeDef *hsdram);  /* 中断服务函数 */
void HAL_SDRAM_RefreshErrorCallback(SDRAM_HandleTypeDef *hsdram);  /* 函数定义或声明 */
void HAL_SDRAM_DMA_XferCpltCallback(DMA_HandleTypeDef *hdma);  /* 函数定义或声明 */
void HAL_SDRAM_DMA_XferErrorCallback(DMA_HandleTypeDef *hdma);  /* 函数定义或声明 */
/**
  * @}
  */

/** @addtogroup SDRAM_Exported_Functions_Group2 
  * @{
  */
/* I/O operation functions ****************************************************/
HAL_StatusTypeDef HAL_SDRAM_Read_8b(SDRAM_HandleTypeDef *hsdram, uint32_t *pAddress, uint8_t *pDstBuffer, uint32_t BufferSize);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_SDRAM_Write_8b(SDRAM_HandleTypeDef *hsdram, uint32_t *pAddress, uint8_t *pSrcBuffer, uint32_t BufferSize);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_SDRAM_Read_16b(SDRAM_HandleTypeDef *hsdram, uint32_t *pAddress, uint16_t *pDstBuffer, uint32_t BufferSize);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_SDRAM_Write_16b(SDRAM_HandleTypeDef *hsdram, uint32_t *pAddress, uint16_t *pSrcBuffer, uint32_t BufferSize);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_SDRAM_Read_32b(SDRAM_HandleTypeDef *hsdram, uint32_t *pAddress, uint32_t *pDstBuffer, uint32_t BufferSize);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_SDRAM_Write_32b(SDRAM_HandleTypeDef *hsdram, uint32_t *pAddress, uint32_t *pSrcBuffer, uint32_t BufferSize);  /* HAL状态类型函数 */

HAL_StatusTypeDef HAL_SDRAM_Read_DMA(SDRAM_HandleTypeDef *hsdram, uint32_t * pAddress, uint32_t *pDstBuffer, uint32_t BufferSize);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_SDRAM_Write_DMA(SDRAM_HandleTypeDef *hsdram, uint32_t *pAddress, uint32_t *pSrcBuffer, uint32_t BufferSize);  /* HAL状态类型函数 */

#if (USE_HAL_SDRAM_REGISTER_CALLBACKS == 1)             /* 条件编译 */
/* SDRAM callback registering/unregistering */
HAL_StatusTypeDef HAL_SDRAM_RegisterCallback(SDRAM_HandleTypeDef *hsdram, HAL_SDRAM_CallbackIDTypeDef CallbackId, pSDRAM_CallbackTypeDef pCallback);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_SDRAM_UnRegisterCallback(SDRAM_HandleTypeDef *hsdram, HAL_SDRAM_CallbackIDTypeDef CallbackId);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_SDRAM_RegisterDmaCallback(SDRAM_HandleTypeDef *hsdram, HAL_SDRAM_CallbackIDTypeDef CallbackId, pSDRAM_DmaCallbackTypeDef pCallback);  /* HAL状态类型函数 */
#endif                                                  /* 条件编译结束 */

/**
  * @}
  */

/** @addtogroup SDRAM_Exported_Functions_Group3 
  * @{
  */
/* SDRAM Control functions  *****************************************************/
HAL_StatusTypeDef HAL_SDRAM_WriteProtection_Enable(SDRAM_HandleTypeDef *hsdram);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_SDRAM_WriteProtection_Disable(SDRAM_HandleTypeDef *hsdram);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_SDRAM_SendCommand(SDRAM_HandleTypeDef *hsdram, FMC_SDRAM_CommandTypeDef *Command, uint32_t Timeout);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_SDRAM_ProgramRefreshRate(SDRAM_HandleTypeDef *hsdram, uint32_t RefreshRate);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_SDRAM_SetAutoRefreshNumber(SDRAM_HandleTypeDef *hsdram, uint32_t AutoRefreshNumber);  /* HAL状态类型函数 */
uint32_t          HAL_SDRAM_GetModeStatus(SDRAM_HandleTypeDef *hsdram);  /* 32位无符号变量/函数 */
/**
  * @}
  */

/** @addtogroup SDRAM_Exported_Functions_Group4 
  * @{
  */
/* SDRAM State functions ********************************************************/
HAL_SDRAM_StateTypeDef  HAL_SDRAM_GetState(SDRAM_HandleTypeDef *hsdram);  /* HAL库函数调用 */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx || STM32F446xx || STM32F469xx || STM32F479xx */

/**
  * @}
  */

#ifdef __cplusplus                                      /* 条件编译 */
}                                                       /* 代码块结束 */
#endif                                                  /* 条件编译结束 */

#endif /* __STM32F4xx_HAL_SDRAM_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
