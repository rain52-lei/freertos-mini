/* 文件: stm32f4xx_hal_sram.h | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_sram.h
  * @author  MCD Application Team
  * @brief   Header file of SRAM HAL module.
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
#ifndef __STM32F4xx_HAL_SRAM_H                          /* 防止头文件重复包含 */
#define __STM32F4xx_HAL_SRAM_H                          /* 定义包含保护宏 */

#ifdef __cplusplus                                      /* 条件编译 */
 extern "C" {                                           /* 外部符号声明 */
#endif                                                  /* 条件编译结束 */

/* Includes ------------------------------------------------------------------*/
#if defined(STM32F405xx) || defined(STM32F415xx) || defined(STM32F407xx) || defined(STM32F417xx) || defined(STM32F412Zx) ||\
    defined(STM32F412Vx) || defined(STM32F412Rx) || defined(STM32F413xx) || defined(STM32F423xx)  /* 源代码 */
  #include "stm32f4xx_ll_fsmc.h"                        /* 包含 STM32/HAL 头文件 */
#endif /* STM32F405xx || STM32F415xx || STM32F407xx || STM32F417xx || STM32F412Zx || STM32F412Vx || STM32F412Rx || STM32F413xx || STM32F423xx */

#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx)|| defined(STM32F439xx) ||\
 defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx)  /* 源代码 */
  #include "stm32f4xx_ll_fmc.h"                         /* 包含 STM32/HAL 头文件 */
#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx || STM32F446xx || STM32F469xx || STM32F479xx */


/** @addtogroup STM32F4xx_HAL_Driver
  * @{
  */

#if defined(STM32F405xx) || defined(STM32F415xx) || defined(STM32F407xx) || defined(STM32F417xx) ||\
    defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx) ||\
    defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx) || defined(STM32F412Zx) ||\
    defined(STM32F412Vx) || defined(STM32F412Rx) || defined(STM32F413xx) || defined(STM32F423xx)  /* 源代码 */

/** @addtogroup SRAM
  * @{
  */ 

/* Exported typedef ----------------------------------------------------------*/

/** @defgroup SRAM_Exported_Types SRAM Exported Types
  * @{
  */ 
/** 
  * @brief  HAL SRAM State structures definition  
  */ 
typedef enum                                            /* 类型定义 */
{                                                       /* 代码块开始 */
  HAL_SRAM_STATE_RESET     = 0x00U,  /*!< SRAM not yet initialized or disabled           */
  HAL_SRAM_STATE_READY     = 0x01U,  /*!< SRAM initialized and ready for use             */
  HAL_SRAM_STATE_BUSY      = 0x02U,  /*!< SRAM internal process is ongoing               */
  HAL_SRAM_STATE_ERROR     = 0x03U,  /*!< SRAM error state                               */
  HAL_SRAM_STATE_PROTECTED = 0x04U   /*!< SRAM peripheral NORSRAM device write protected */

} HAL_SRAM_StateTypeDef;                                /* 代码块结束 */

/**
  * @brief  SRAM handle Structure definition
  */
#if (USE_HAL_SRAM_REGISTER_CALLBACKS == 1)              /* 条件编译 */
typedef struct __SRAM_HandleTypeDef                     /* 类型定义 */
#else                                                   /* 条件编译否则 */
typedef struct                                          /* 类型定义 */
#endif /* USE_HAL_SRAM_REGISTER_CALLBACKS  */	
{                                                       /* 代码块开始 */
  FMC_NORSRAM_TypeDef           *Instance;  /*!< Register base address                        */ 

  FMC_NORSRAM_EXTENDED_TypeDef  *Extended;  /*!< Extended mode register base address          */

  FMC_NORSRAM_InitTypeDef       Init;       /*!< SRAM device control configuration parameters */

  HAL_LockTypeDef               Lock;       /*!< SRAM locking object                          */ 

  __IO HAL_SRAM_StateTypeDef    State;      /*!< SRAM device access state                     */

  DMA_HandleTypeDef             *hdma;      /*!< Pointer DMA handler                          */

#if (USE_HAL_SRAM_REGISTER_CALLBACKS == 1)              /* 条件编译 */
  void  (* MspInitCallback)        ( struct __SRAM_HandleTypeDef * hsram);    /*!< SRAM Msp Init callback              */
  void  (* MspDeInitCallback)      ( struct __SRAM_HandleTypeDef * hsram);    /*!< SRAM Msp DeInit callback            */
  void  (* DmaXferCpltCallback)    ( DMA_HandleTypeDef * hdma);               /*!< SRAM DMA Xfer Complete callback     */
  void  (* DmaXferErrorCallback)   ( DMA_HandleTypeDef * hdma);               /*!< SRAM DMA Xfer Error callback        */
#endif                                                  /* 条件编译结束 */
} SRAM_HandleTypeDef;                                   /* 代码块结束 */

#if (USE_HAL_SRAM_REGISTER_CALLBACKS == 1)              /* 条件编译 */
/**
  * @brief  HAL SRAM Callback ID enumeration definition
  */
typedef enum                                            /* 类型定义 */
{                                                       /* 代码块开始 */
  HAL_SRAM_MSP_INIT_CB_ID       = 0x00U,  /*!< SRAM MspInit Callback ID           */
  HAL_SRAM_MSP_DEINIT_CB_ID     = 0x01U,  /*!< SRAM MspDeInit Callback ID         */
  HAL_SRAM_DMA_XFER_CPLT_CB_ID  = 0x02U,  /*!< SRAM DMA Xfer Complete Callback ID */
  HAL_SRAM_DMA_XFER_ERR_CB_ID   = 0x03U   /*!< SRAM DMA Xfer Complete Callback ID */
}HAL_SRAM_CallbackIDTypeDef;                            /* 代码块结束 */

/**
  * @brief  HAL SRAM Callback pointer definition
  */
typedef void (*pSRAM_CallbackTypeDef)(SRAM_HandleTypeDef *hsram);  /* 类型定义 */
typedef void (*pSRAM_DmaCallbackTypeDef)(DMA_HandleTypeDef *hdma);  /* 类型定义 */
#endif                                                  /* 条件编译结束 */
/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/** @defgroup SRAM_Exported_Macros SRAM Exported Macros
  * @{
  */
/** @brief Reset SRAM handle state
  * @param  __HANDLE__ SRAM handle
  * @retval None
  */
#if (USE_HAL_SRAM_REGISTER_CALLBACKS == 1)              /* 条件编译 */
#define __HAL_SRAM_RESET_HANDLE_STATE(__HANDLE__)         do {                                             \
                                                               (__HANDLE__)->State = HAL_SRAM_STATE_RESET; \
                                                               (__HANDLE__)->MspInitCallback = NULL;       \
                                                               (__HANDLE__)->MspDeInitCallback = NULL;     \
                                                             } while(0)  /* 代码块结束 */
#else                                                   /* 条件编译否则 */
#define __HAL_SRAM_RESET_HANDLE_STATE(__HANDLE__) ((__HANDLE__)->State = HAL_SRAM_STATE_RESET)  /* 定义包含保护宏 */
#endif                                                  /* 条件编译结束 */

/**
  * @}
  */
/* Exported functions --------------------------------------------------------*/

/** @addtogroup SRAM_Exported_Functions
 *  @{
 */

/** @addtogroup SRAM_Exported_Functions_Group1
 *  @{
 */
/* Initialization/de-initialization functions  **********************************/
HAL_StatusTypeDef HAL_SRAM_Init(SRAM_HandleTypeDef *hsram, FMC_NORSRAM_TimingTypeDef *Timing, FMC_NORSRAM_TimingTypeDef *ExtTiming);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_SRAM_DeInit(SRAM_HandleTypeDef *hsram);  /* HAL状态类型函数 */
void              HAL_SRAM_MspInit(SRAM_HandleTypeDef *hsram);  /* 函数定义或声明 */
void              HAL_SRAM_MspDeInit(SRAM_HandleTypeDef *hsram);  /* 函数定义或声明 */

void              HAL_SRAM_DMA_XferCpltCallback(DMA_HandleTypeDef *hdma);  /* 函数定义或声明 */
void              HAL_SRAM_DMA_XferErrorCallback(DMA_HandleTypeDef *hdma);  /* 函数定义或声明 */
/**
  * @}
  */ 

/** @addtogroup SRAM_Exported_Functions_Group2
 *  @{
 */
/* I/O operation functions  *****************************************************/
HAL_StatusTypeDef HAL_SRAM_Read_8b(SRAM_HandleTypeDef *hsram, uint32_t *pAddress, uint8_t *pDstBuffer, uint32_t BufferSize);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_SRAM_Write_8b(SRAM_HandleTypeDef *hsram, uint32_t *pAddress, uint8_t *pSrcBuffer, uint32_t BufferSize);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_SRAM_Read_16b(SRAM_HandleTypeDef *hsram, uint32_t *pAddress, uint16_t *pDstBuffer, uint32_t BufferSize);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_SRAM_Write_16b(SRAM_HandleTypeDef *hsram, uint32_t *pAddress, uint16_t *pSrcBuffer, uint32_t BufferSize);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_SRAM_Read_32b(SRAM_HandleTypeDef *hsram, uint32_t *pAddress, uint32_t *pDstBuffer, uint32_t BufferSize);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_SRAM_Write_32b(SRAM_HandleTypeDef *hsram, uint32_t *pAddress, uint32_t *pSrcBuffer, uint32_t BufferSize);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_SRAM_Read_DMA(SRAM_HandleTypeDef *hsram, uint32_t *pAddress, uint32_t *pDstBuffer, uint32_t BufferSize);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_SRAM_Write_DMA(SRAM_HandleTypeDef *hsram, uint32_t *pAddress, uint32_t *pSrcBuffer, uint32_t BufferSize);  /* HAL状态类型函数 */
#if (USE_HAL_SRAM_REGISTER_CALLBACKS == 1)              /* 条件编译 */
/* SRAM callback registering/unregistering */
HAL_StatusTypeDef HAL_SRAM_RegisterCallback(SRAM_HandleTypeDef *hsram, HAL_SRAM_CallbackIDTypeDef CallbackId, pSRAM_CallbackTypeDef pCallback);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_SRAM_UnRegisterCallback(SRAM_HandleTypeDef *hsram, HAL_SRAM_CallbackIDTypeDef CallbackId);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_SRAM_RegisterDmaCallback(SRAM_HandleTypeDef *hsram, HAL_SRAM_CallbackIDTypeDef CallbackId, pSRAM_DmaCallbackTypeDef pCallback);  /* HAL状态类型函数 */
#endif                                                  /* 条件编译结束 */
/**
  * @}
  */ 

/** @addtogroup SRAM_Exported_Functions_Group3
 *  @{
 */
/* SRAM Control functions  ******************************************************/
HAL_StatusTypeDef HAL_SRAM_WriteOperation_Enable(SRAM_HandleTypeDef *hsram);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_SRAM_WriteOperation_Disable(SRAM_HandleTypeDef *hsram);  /* HAL状态类型函数 */
/**
  * @}
  */ 

/** @addtogroup SRAM_Exported_Functions_Group4
 *  @{
 */
/* SRAM State functions *********************************************************/
HAL_SRAM_StateTypeDef HAL_SRAM_GetState(SRAM_HandleTypeDef *hsram);  /* HAL库函数调用 */
/**
  * @}
  */

/**
  * @}
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @}
  */ 

#endif /* STM32F405xx || STM32F415xx || STM32F407xx || STM32F417xx || STM32F427xx || STM32F437xx ||\
          STM32F429xx || STM32F439xx || STM32F446xx || STM32F469xx || STM32F479xx || STM32F412Zx ||\
          STM32F412Vx || STM32F412Rx || STM32F413xx || STM32F423xx */
/**
  * @}
  */
#ifdef __cplusplus                                      /* 条件编译 */
}                                                       /* 代码块结束 */
#endif                                                  /* 条件编译结束 */

#endif /* __STM32F4xx_HAL_SRAM_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
