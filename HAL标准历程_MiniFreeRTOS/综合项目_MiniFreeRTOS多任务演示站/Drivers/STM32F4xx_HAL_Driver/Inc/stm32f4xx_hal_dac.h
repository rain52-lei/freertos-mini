/* 文件: stm32f4xx_hal_dac.h | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_dac.h
  * @author  MCD Application Team
  * @brief   Header file of DAC HAL module.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32F4xx_HAL_DAC_H                             /* 防止头文件重复包含 */
#define STM32F4xx_HAL_DAC_H                             /* 定义宏 */

#ifdef __cplusplus                                      /* 条件编译 */
extern "C" {                                            /* 外部符号声明 */
#endif                                                  /* 条件编译结束 */

/** @addtogroup STM32F4xx_HAL_Driver
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal_def.h"                          /* 包含 STM32/HAL 头文件 */

#if defined(DAC)                                        /* 条件编译 */

/** @addtogroup DAC
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** @defgroup DAC_Exported_Types DAC Exported Types
  * @{
  */

/**
  * @brief  HAL State structures definition
  */
typedef enum                                            /* 类型定义 */
{                                                       /* 代码块开始 */
  HAL_DAC_STATE_RESET             = 0x00U,  /*!< DAC not yet initialized or disabled  */
  HAL_DAC_STATE_READY             = 0x01U,  /*!< DAC initialized and ready for use    */
  HAL_DAC_STATE_BUSY              = 0x02U,  /*!< DAC internal processing is ongoing   */
  HAL_DAC_STATE_TIMEOUT           = 0x03U,  /*!< DAC timeout state                    */
  HAL_DAC_STATE_ERROR             = 0x04U   /*!< DAC error state                      */

} HAL_DAC_StateTypeDef;                                 /* 代码块结束 */

/**
  * @brief  DAC handle Structure definition
  */
#if (USE_HAL_DAC_REGISTER_CALLBACKS == 1)               /* 条件编译 */
typedef struct __DAC_HandleTypeDef                      /* 类型定义 */
#else                                                   /* 条件编译否则 */
typedef struct                                          /* 类型定义 */
#endif /* USE_HAL_DAC_REGISTER_CALLBACKS */
{                                                       /* 代码块开始 */
  DAC_TypeDef                 *Instance;     /*!< Register base address             */

  __IO HAL_DAC_StateTypeDef   State;         /*!< DAC communication state           */

  HAL_LockTypeDef             Lock;          /*!< DAC locking object                */

  DMA_HandleTypeDef           *DMA_Handle1;  /*!< Pointer DMA handler for channel 1 */

  DMA_HandleTypeDef           *DMA_Handle2;  /*!< Pointer DMA handler for channel 2 */

  __IO uint32_t               ErrorCode;     /*!< DAC Error code                    */

#if (USE_HAL_DAC_REGISTER_CALLBACKS == 1)               /* 条件编译 */
  void (* ConvCpltCallbackCh1)(struct __DAC_HandleTypeDef *hdac);  /* 函数定义或声明 */
  void (* ConvHalfCpltCallbackCh1)(struct __DAC_HandleTypeDef *hdac);  /* 函数定义或声明 */
  void (* ErrorCallbackCh1)(struct __DAC_HandleTypeDef *hdac);  /* 函数定义或声明 */
  void (* DMAUnderrunCallbackCh1)(struct __DAC_HandleTypeDef *hdac);  /* 函数定义或声明 */
#if defined(DAC_CHANNEL2_SUPPORT)                       /* 条件编译 */
  void (* ConvCpltCallbackCh2)(struct __DAC_HandleTypeDef *hdac);  /* 函数定义或声明 */
  void (* ConvHalfCpltCallbackCh2)(struct __DAC_HandleTypeDef *hdac);  /* 函数定义或声明 */
  void (* ErrorCallbackCh2)(struct __DAC_HandleTypeDef *hdac);  /* 函数定义或声明 */
  void (* DMAUnderrunCallbackCh2)(struct __DAC_HandleTypeDef *hdac);  /* 函数定义或声明 */
#endif /* DAC_CHANNEL2_SUPPORT */

  void (* MspInitCallback)(struct __DAC_HandleTypeDef *hdac);  /* 函数定义或声明 */
  void (* MspDeInitCallback)(struct __DAC_HandleTypeDef *hdac);  /* 函数定义或声明 */
#endif /* USE_HAL_DAC_REGISTER_CALLBACKS */

} DAC_HandleTypeDef;                                    /* 代码块结束 */

/**
  * @brief   DAC Configuration regular Channel structure definition
  */
typedef struct                                          /* 类型定义 */
{                                                       /* 代码块开始 */
  uint32_t DAC_Trigger;                  /*!< Specifies the external trigger for the selected DAC channel.
                                              This parameter can be a value of @ref DAC_trigger_selection */

  uint32_t DAC_OutputBuffer;             /*!< Specifies whether the DAC channel output buffer is enabled or disabled.
                                               This parameter can be a value of @ref DAC_output_buffer */

} DAC_ChannelConfTypeDef;                               /* 代码块结束 */

#if (USE_HAL_DAC_REGISTER_CALLBACKS == 1)               /* 条件编译 */
/**
  * @brief  HAL DAC Callback ID enumeration definition
  */
typedef enum                                            /* 类型定义 */
{                                                       /* 代码块开始 */
  HAL_DAC_CH1_COMPLETE_CB_ID                 = 0x00U,  /*!< DAC CH1 Complete Callback ID      */
  HAL_DAC_CH1_HALF_COMPLETE_CB_ID            = 0x01U,  /*!< DAC CH1 half Complete Callback ID */
  HAL_DAC_CH1_ERROR_ID                       = 0x02U,  /*!< DAC CH1 error Callback ID         */
  HAL_DAC_CH1_UNDERRUN_CB_ID                 = 0x03U,  /*!< DAC CH1 underrun Callback ID      */
#if defined(DAC_CHANNEL2_SUPPORT)                       /* 条件编译 */
  HAL_DAC_CH2_COMPLETE_CB_ID                 = 0x04U,  /*!< DAC CH2 Complete Callback ID      */
  HAL_DAC_CH2_HALF_COMPLETE_CB_ID            = 0x05U,  /*!< DAC CH2 half Complete Callback ID */
  HAL_DAC_CH2_ERROR_ID                       = 0x06U,  /*!< DAC CH2 error Callback ID         */
  HAL_DAC_CH2_UNDERRUN_CB_ID                 = 0x07U,  /*!< DAC CH2 underrun Callback ID      */
#endif /* DAC_CHANNEL2_SUPPORT */
  HAL_DAC_MSPINIT_CB_ID                      = 0x08U,  /*!< DAC MspInit Callback ID           */
  HAL_DAC_MSPDEINIT_CB_ID                    = 0x09U,  /*!< DAC MspDeInit Callback ID         */
  HAL_DAC_ALL_CB_ID                          = 0x0AU   /*!< DAC All ID                        */
} HAL_DAC_CallbackIDTypeDef;                            /* 代码块结束 */

/**
  * @brief  HAL DAC Callback pointer definition
  */
typedef void (*pDAC_CallbackTypeDef)(DAC_HandleTypeDef *hdac);  /* 类型定义 */
#endif /* USE_HAL_DAC_REGISTER_CALLBACKS */

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/

/** @defgroup DAC_Exported_Constants DAC Exported Constants
  * @{
  */

/** @defgroup DAC_Error_Code DAC Error Code
  * @{
  */
#define  HAL_DAC_ERROR_NONE              0x00U    /*!< No error                          */
#define  HAL_DAC_ERROR_DMAUNDERRUNCH1    0x01U    /*!< DAC channel1 DMA underrun error   */
#if defined(DAC_CHANNEL2_SUPPORT)                       /* 条件编译 */
#define  HAL_DAC_ERROR_DMAUNDERRUNCH2    0x02U    /*!< DAC channel2 DMA underrun error   */
#endif /* DAC_CHANNEL2_SUPPORT */
#define  HAL_DAC_ERROR_DMA               0x04U    /*!< DMA error                         */
#define  HAL_DAC_ERROR_TIMEOUT           0x08U    /*!< Timeout error                     */
#if (USE_HAL_DAC_REGISTER_CALLBACKS == 1)               /* 条件编译 */
#define HAL_DAC_ERROR_INVALID_CALLBACK   0x10U    /*!< Invalid callback error            */
#endif /* USE_HAL_DAC_REGISTER_CALLBACKS */

/**
  * @}
  */

/** @defgroup DAC_trigger_selection DAC trigger selection
  * @{
  */
#define DAC_TRIGGER_NONE                0x00000000UL                                                     /*!< Conversion is automatic once the DAC1_DHRxxxx register has been loaded, and not by external trigger */
#define DAC_TRIGGER_T2_TRGO             (DAC_CR_TSEL1_2                                   | DAC_CR_TEN1) /*!< TIM2 TRGO selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_T4_TRGO             (DAC_CR_TSEL1_2                  | DAC_CR_TSEL1_0 | DAC_CR_TEN1) /*!< TIM4 TRGO selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_T5_TRGO             (                 DAC_CR_TSEL1_1 | DAC_CR_TSEL1_0 | DAC_CR_TEN1) /*!< TIM3 TRGO selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_T6_TRGO             (                                                   DAC_CR_TEN1) /*!< Conversion started by software trigger for DAC channel */
#define DAC_TRIGGER_T7_TRGO             (                 DAC_CR_TSEL1_1                  | DAC_CR_TEN1) /*!< TIM7 TRGO selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_T8_TRGO             (                                  DAC_CR_TSEL1_0 | DAC_CR_TEN1) /*!< TIM8 TRGO selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_EXT_IT9             (DAC_CR_TSEL1_2 | DAC_CR_TSEL1_1                  | DAC_CR_TEN1) /*!< EXTI Line9 event selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_SOFTWARE            (DAC_CR_TSEL1                                     | DAC_CR_TEN1) /*!< Conversion started by software trigger for DAC channel */

/**
  * @}
  */

/** @defgroup DAC_output_buffer DAC output buffer
  * @{
  */
#define DAC_OUTPUTBUFFER_ENABLE            0x00000000U  /* 定义宏 */
#define DAC_OUTPUTBUFFER_DISABLE           (DAC_CR_BOFF1)  /* 定义宏 */

/**
  * @}
  */

/** @defgroup DAC_Channel_selection DAC Channel selection
  * @{
  */
#define DAC_CHANNEL_1                      0x00000000U  /* 定义宏 */
#if defined(DAC_CHANNEL2_SUPPORT)                       /* 条件编译 */
#define DAC_CHANNEL_2                      0x00000010U  /* 定义宏 */
#endif /* DAC_CHANNEL2_SUPPORT */
/**
  * @}
  */

/** @defgroup DAC_data_alignment DAC data alignment
  * @{
  */
#define DAC_ALIGN_12B_R                    0x00000000U  /* 定义宏 */
#define DAC_ALIGN_12B_L                    0x00000004U  /* 定义宏 */
#define DAC_ALIGN_8B_R                     0x00000008U  /* 定义宏 */

/**
  * @}
  */

/** @defgroup DAC_flags_definition DAC flags definition
  * @{
  */
#define DAC_FLAG_DMAUDR1                   (DAC_SR_DMAUDR1)  /* 定义宏 */
#if defined(DAC_CHANNEL2_SUPPORT)                       /* 条件编译 */
#define DAC_FLAG_DMAUDR2                   (DAC_SR_DMAUDR2)  /* 定义宏 */
#endif /* DAC_CHANNEL2_SUPPORT */

/**
  * @}
  */

/** @defgroup DAC_IT_definition  DAC IT definition
  * @{
  */
#define DAC_IT_DMAUDR1                   (DAC_SR_DMAUDR1)  /* 定义宏 */
#if defined(DAC_CHANNEL2_SUPPORT)                       /* 条件编译 */
#define DAC_IT_DMAUDR2                   (DAC_SR_DMAUDR2)  /* 定义宏 */
#endif /* DAC_CHANNEL2_SUPPORT */

/**
  * @}
  */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/

/** @defgroup DAC_Exported_Macros DAC Exported Macros
  * @{
  */

/** @brief Reset DAC handle state.
  * @param  __HANDLE__ specifies the DAC handle.
  * @retval None
  */
#if (USE_HAL_DAC_REGISTER_CALLBACKS == 1)               /* 条件编译 */
#define __HAL_DAC_RESET_HANDLE_STATE(__HANDLE__) do {                                                        \
                                                      (__HANDLE__)->State             = HAL_DAC_STATE_RESET; \
                                                      (__HANDLE__)->MspInitCallback   = NULL;                \
                                                      (__HANDLE__)->MspDeInitCallback = NULL;                \
                                                     } while(0)  /* 代码块结束 */
#else                                                   /* 条件编译否则 */
#define __HAL_DAC_RESET_HANDLE_STATE(__HANDLE__) ((__HANDLE__)->State = HAL_DAC_STATE_RESET)  /* 定义包含保护宏 */
#endif /* USE_HAL_DAC_REGISTER_CALLBACKS */

/** @brief Enable the DAC channel.
  * @param  __HANDLE__ specifies the DAC handle.
  * @param  __DAC_Channel__ specifies the DAC channel
  * @retval None
  */
#define __HAL_DAC_ENABLE(__HANDLE__, __DAC_Channel__) \
  ((__HANDLE__)->Instance->CR |=  (DAC_CR_EN1 << ((__DAC_Channel__) & 0x10UL)))  /* 源代码 */

/** @brief Disable the DAC channel.
  * @param  __HANDLE__ specifies the DAC handle
  * @param  __DAC_Channel__ specifies the DAC channel.
  * @retval None
  */
#define __HAL_DAC_DISABLE(__HANDLE__, __DAC_Channel__) \
  ((__HANDLE__)->Instance->CR &=  ~(DAC_CR_EN1 << ((__DAC_Channel__) & 0x10UL)))  /* 源代码 */

/** @brief Set DHR12R1 alignment.
  * @param  __ALIGNMENT__ specifies the DAC alignment
  * @retval None
  */
#define DAC_DHR12R1_ALIGNMENT(__ALIGNMENT__) (0x00000008UL + (__ALIGNMENT__))  /* 定义宏 */

#if defined(DAC_CHANNEL2_SUPPORT)                       /* 条件编译 */
/** @brief  Set DHR12R2 alignment.
  * @param  __ALIGNMENT__ specifies the DAC alignment
  * @retval None
  */
#define DAC_DHR12R2_ALIGNMENT(__ALIGNMENT__) (0x00000014UL + (__ALIGNMENT__))  /* 定义宏 */
#endif /* DAC_CHANNEL2_SUPPORT */

/** @brief  Set DHR12RD alignment.
  * @param  __ALIGNMENT__ specifies the DAC alignment
  * @retval None
  */
#define DAC_DHR12RD_ALIGNMENT(__ALIGNMENT__) (0x00000020UL + (__ALIGNMENT__))  /* 定义宏 */

/** @brief Enable the DAC interrupt.
  * @param  __HANDLE__ specifies the DAC handle
  * @param  __INTERRUPT__ specifies the DAC interrupt.
  *          This parameter can be any combination of the following values:
  *            @arg DAC_IT_DMAUDR1 DAC channel 1 DMA underrun interrupt
  *            @arg DAC_IT_DMAUDR2 DAC channel 2 DMA underrun interrupt
  * @retval None
  */
#define __HAL_DAC_ENABLE_IT(__HANDLE__, __INTERRUPT__) (((__HANDLE__)->Instance->CR) |= (__INTERRUPT__))  /* 定义包含保护宏 */

/** @brief Disable the DAC interrupt.
  * @param  __HANDLE__ specifies the DAC handle
  * @param  __INTERRUPT__ specifies the DAC interrupt.
  *          This parameter can be any combination of the following values:
  *            @arg DAC_IT_DMAUDR1 DAC channel 1 DMA underrun interrupt
  *            @arg DAC_IT_DMAUDR2 DAC channel 2 DMA underrun interrupt
  * @retval None
  */
#define __HAL_DAC_DISABLE_IT(__HANDLE__, __INTERRUPT__) (((__HANDLE__)->Instance->CR) &= ~(__INTERRUPT__))  /* 定义包含保护宏 */

/** @brief  Check whether the specified DAC interrupt source is enabled or not.
  * @param __HANDLE__ DAC handle
  * @param __INTERRUPT__ DAC interrupt source to check
  *          This parameter can be any combination of the following values:
  *            @arg DAC_IT_DMAUDR1 DAC channel 1 DMA underrun interrupt
  *            @arg DAC_IT_DMAUDR2 DAC channel 2 DMA underrun interrupt
  * @retval State of interruption (SET or RESET)
  */
#define __HAL_DAC_GET_IT_SOURCE(__HANDLE__, __INTERRUPT__) (((__HANDLE__)->Instance->CR\
                                                             & (__INTERRUPT__)) == (__INTERRUPT__))  /* 源代码 */

/** @brief  Get the selected DAC's flag status.
  * @param  __HANDLE__ specifies the DAC handle.
  * @param  __FLAG__ specifies the DAC flag to get.
  *          This parameter can be any combination of the following values:
  *            @arg DAC_FLAG_DMAUDR1 DAC channel 1 DMA underrun flag
  *            @arg DAC_FLAG_DMAUDR2 DAC channel 2 DMA underrun flag
  * @retval None
  */
#define __HAL_DAC_GET_FLAG(__HANDLE__, __FLAG__) ((((__HANDLE__)->Instance->SR) & (__FLAG__)) == (__FLAG__))  /* 定义包含保护宏 */

/** @brief  Clear the DAC's flag.
  * @param  __HANDLE__ specifies the DAC handle.
  * @param  __FLAG__ specifies the DAC flag to clear.
  *          This parameter can be any combination of the following values:
  *            @arg DAC_FLAG_DMAUDR1 DAC channel 1 DMA underrun flag
  *            @arg DAC_FLAG_DMAUDR2 DAC channel 2 DMA underrun flag
  * @retval None
  */
#define __HAL_DAC_CLEAR_FLAG(__HANDLE__, __FLAG__) (((__HANDLE__)->Instance->SR) = (__FLAG__))  /* 定义包含保护宏 */

/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/

/** @defgroup DAC_Private_Macros DAC Private Macros
  * @{
  */
#define IS_DAC_OUTPUT_BUFFER_STATE(STATE) (((STATE) == DAC_OUTPUTBUFFER_ENABLE) || \
                                           ((STATE) == DAC_OUTPUTBUFFER_DISABLE))  /* 源代码 */

#if defined(DAC_CHANNEL2_SUPPORT)                       /* 条件编译 */
#define IS_DAC_CHANNEL(CHANNEL) (((CHANNEL) == DAC_CHANNEL_1) || \
                                 ((CHANNEL) == DAC_CHANNEL_2))  /* 源代码 */
#else                                                   /* 条件编译否则 */
#define IS_DAC_CHANNEL(CHANNEL)  ((CHANNEL) == DAC_CHANNEL_1)  /* 定义宏 */
#endif /* DAC_CHANNEL2_SUPPORT */

#define IS_DAC_ALIGN(ALIGN) (((ALIGN) == DAC_ALIGN_12B_R) || \
                             ((ALIGN) == DAC_ALIGN_12B_L) || \
                             ((ALIGN) == DAC_ALIGN_8B_R))  /* 源代码 */

#define IS_DAC_DATA(DATA) ((DATA) <= 0xFFF0UL)          /* 定义宏 */

/**
  * @}
  */

/* Include DAC HAL Extended module */
#include "stm32f4xx_hal_dac_ex.h"                       /* 包含 STM32/HAL 头文件 */

/* Exported functions --------------------------------------------------------*/

/** @addtogroup DAC_Exported_Functions
  * @{
  */

/** @addtogroup DAC_Exported_Functions_Group1
  * @{
  */
/* Initialization and de-initialization functions *****************************/
HAL_StatusTypeDef HAL_DAC_Init(DAC_HandleTypeDef *hdac);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_DAC_DeInit(DAC_HandleTypeDef *hdac);  /* HAL状态类型函数 */
void HAL_DAC_MspInit(DAC_HandleTypeDef *hdac);          /* 函数定义或声明 */
void HAL_DAC_MspDeInit(DAC_HandleTypeDef *hdac);        /* 函数定义或声明 */

/**
  * @}
  */

/** @addtogroup DAC_Exported_Functions_Group2
  * @{
  */
/* IO operation functions *****************************************************/
HAL_StatusTypeDef HAL_DAC_Start(DAC_HandleTypeDef *hdac, uint32_t Channel);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_DAC_Stop(DAC_HandleTypeDef *hdac, uint32_t Channel);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_DAC_Start_DMA(DAC_HandleTypeDef *hdac, uint32_t Channel, uint32_t *pData, uint32_t Length,  /* HAL状态类型函数 */
                                    uint32_t Alignment);  /* 32位无符号变量/函数 */
HAL_StatusTypeDef HAL_DAC_Stop_DMA(DAC_HandleTypeDef *hdac, uint32_t Channel);  /* HAL状态类型函数 */
void HAL_DAC_IRQHandler(DAC_HandleTypeDef *hdac);       /* 中断服务函数 */
HAL_StatusTypeDef HAL_DAC_SetValue(DAC_HandleTypeDef *hdac, uint32_t Channel, uint32_t Alignment, uint32_t Data);  /* HAL状态类型函数 */

void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef *hdac);  /* 函数定义或声明 */
void HAL_DAC_ConvHalfCpltCallbackCh1(DAC_HandleTypeDef *hdac);  /* 函数定义或声明 */
void HAL_DAC_ErrorCallbackCh1(DAC_HandleTypeDef *hdac);  /* 函数定义或声明 */
void HAL_DAC_DMAUnderrunCallbackCh1(DAC_HandleTypeDef *hdac);  /* 函数定义或声明 */

#if (USE_HAL_DAC_REGISTER_CALLBACKS == 1)               /* 条件编译 */
/* DAC callback registering/unregistering */
HAL_StatusTypeDef     HAL_DAC_RegisterCallback(DAC_HandleTypeDef *hdac, HAL_DAC_CallbackIDTypeDef CallbackID,  /* HAL状态类型函数 */
                                               pDAC_CallbackTypeDef pCallback);  /* 执行语句 */
HAL_StatusTypeDef     HAL_DAC_UnRegisterCallback(DAC_HandleTypeDef *hdac, HAL_DAC_CallbackIDTypeDef CallbackID);  /* HAL状态类型函数 */
#endif /* USE_HAL_DAC_REGISTER_CALLBACKS */

/**
  * @}
  */

/** @addtogroup DAC_Exported_Functions_Group3
  * @{
  */
/* Peripheral Control functions ***********************************************/
uint32_t HAL_DAC_GetValue(DAC_HandleTypeDef *hdac, uint32_t Channel);  /* 32位无符号变量/函数 */
HAL_StatusTypeDef HAL_DAC_ConfigChannel(DAC_HandleTypeDef *hdac, DAC_ChannelConfTypeDef *sConfig, uint32_t Channel);  /* HAL状态类型函数 */
/**
  * @}
  */

/** @addtogroup DAC_Exported_Functions_Group4
  * @{
  */
/* Peripheral State and Error functions ***************************************/
HAL_DAC_StateTypeDef HAL_DAC_GetState(DAC_HandleTypeDef *hdac);  /* HAL库函数调用 */
uint32_t HAL_DAC_GetError(DAC_HandleTypeDef *hdac);     /* 32位无符号变量/函数 */

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup DAC_Private_Functions DAC Private Functions
  * @{
  */
void DAC_DMAConvCpltCh1(DMA_HandleTypeDef *hdma);       /* 函数定义或声明 */
void DAC_DMAErrorCh1(DMA_HandleTypeDef *hdma);          /* 函数定义或声明 */
void DAC_DMAHalfConvCpltCh1(DMA_HandleTypeDef *hdma);   /* 函数定义或声明 */
/**
  * @}
  */

/**
  * @}
  */

#endif /* DAC */

/**
  * @}
  */

#ifdef __cplusplus                                      /* 条件编译 */
}                                                       /* 代码块结束 */
#endif                                                  /* 条件编译结束 */


#endif /* STM32F4xx_HAL_DAC_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
