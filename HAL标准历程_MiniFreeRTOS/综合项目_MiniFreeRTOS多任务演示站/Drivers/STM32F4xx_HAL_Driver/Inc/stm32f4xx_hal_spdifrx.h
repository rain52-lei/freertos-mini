/* 文件: stm32f4xx_hal_spdifrx.h | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_spdifrx.h
  * @author  MCD Application Team
  * @brief   Header file of SPDIFRX HAL module.
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
#ifndef STM32F4xx_HAL_SPDIFRX_H                         /* 防止头文件重复包含 */
#define STM32F4xx_HAL_SPDIFRX_H                         /* 定义宏 */

#ifdef __cplusplus                                      /* 条件编译 */
 extern "C" {                                           /* 外部符号声明 */
#endif                                                  /* 条件编译结束 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal_def.h"                          /* 包含 STM32/HAL 头文件 */

#if defined(STM32F446xx)                                /* 条件编译 */
/** @addtogroup STM32F4xx_HAL_Driver
  * @{
  */
#if defined (SPDIFRX)                                   /* 条件编译 */

/** @addtogroup SPDIFRX
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup SPDIFRX_Exported_Types SPDIFRX Exported Types
  * @{
  */

/**
  * @brief SPDIFRX Init structure definition
  */
typedef struct                                          /* 类型定义 */
{                                                       /* 代码块开始 */
  uint32_t InputSelection;           /*!< Specifies the SPDIF input selection.
                                          This parameter can be a value of @ref SPDIFRX_Input_Selection */

  uint32_t Retries;                  /*!< Specifies the Maximum allowed re-tries during synchronization phase.
                                          This parameter can be a value of @ref SPDIFRX_Max_Retries */

  uint32_t WaitForActivity;          /*!< Specifies the wait for activity on SPDIF selected input.
                                          This parameter can be a value of @ref SPDIFRX_Wait_For_Activity. */

  uint32_t ChannelSelection;         /*!< Specifies whether the control flow will take the channel status from channel A or B.
                                          This parameter can be a value of @ref SPDIFRX_Channel_Selection */

  uint32_t DataFormat;               /*!< Specifies the Data samples format (LSB, MSB, ...).
                                          This parameter can be a value of @ref SPDIFRX_Data_Format */

  uint32_t StereoMode;               /*!< Specifies whether the peripheral is in stereo or mono mode.
                                          This parameter can be a value of @ref SPDIFRX_Stereo_Mode */

  uint32_t PreambleTypeMask;          /*!< Specifies whether The preamble type bits are copied or not into the received frame.
                                           This parameter can be a value of @ref SPDIFRX_PT_Mask */

  uint32_t ChannelStatusMask;        /*!< Specifies whether the channel status and user bits are copied or not into the received frame.
                                          This parameter can be a value of @ref SPDIFRX_ChannelStatus_Mask */

  uint32_t ValidityBitMask;          /*!< Specifies whether the validity bit is copied or not into the received frame.
                                          This parameter can be a value of @ref SPDIFRX_V_Mask */

  uint32_t ParityErrorMask;          /*!< Specifies whether the parity error bit is copied or not into the received frame.
                                          This parameter can be a value of @ref SPDIFRX_PE_Mask */
} SPDIFRX_InitTypeDef;                                  /* 代码块结束 */

/**
  * @brief SPDIFRX SetDataFormat structure definition
  */
typedef struct                                          /* 类型定义 */
{                                                       /* 代码块开始 */
  uint32_t DataFormat;               /*!< Specifies the Data samples format (LSB, MSB, ...).
                                          This parameter can be a value of @ref SPDIFRX_Data_Format */

  uint32_t StereoMode;               /*!< Specifies whether the peripheral is in stereo or mono mode.
                                          This parameter can be a value of @ref SPDIFRX_Stereo_Mode */

  uint32_t PreambleTypeMask;          /*!< Specifies whether The preamble type bits are copied or not into the received frame.
                                                                                   This parameter can be a value of @ref SPDIFRX_PT_Mask */

  uint32_t ChannelStatusMask;        /*!< Specifies whether the channel status and user bits are copied or not into the received frame.
                                                                                  This parameter can be a value of @ref SPDIFRX_ChannelStatus_Mask */

  uint32_t ValidityBitMask;          /*!< Specifies whether the validity bit is copied or not into the received frame.
                                                                                  This parameter can be a value of @ref SPDIFRX_V_Mask */

  uint32_t ParityErrorMask;          /*!< Specifies whether the parity error bit is copied or not into the received frame.
                                                                                  This parameter can be a value of @ref SPDIFRX_PE_Mask */

} SPDIFRX_SetDataFormatTypeDef;                         /* 代码块结束 */

/**
  * @brief  HAL State structures definition
  */
typedef enum                                            /* 类型定义 */
{                                                       /* 代码块开始 */
  HAL_SPDIFRX_STATE_RESET      = 0x00U,  /*!< SPDIFRX not yet initialized or disabled                */
  HAL_SPDIFRX_STATE_READY      = 0x01U,  /*!< SPDIFRX initialized and ready for use                  */
  HAL_SPDIFRX_STATE_BUSY       = 0x02U,  /*!< SPDIFRX internal process is ongoing                    */
  HAL_SPDIFRX_STATE_BUSY_RX    = 0x03U,  /*!< SPDIFRX internal Data Flow RX process is ongoing       */
  HAL_SPDIFRX_STATE_BUSY_CX    = 0x04U,  /*!< SPDIFRX internal Control Flow RX process is ongoing    */
  HAL_SPDIFRX_STATE_ERROR      = 0x07U   /*!< SPDIFRX error state                                    */
} HAL_SPDIFRX_StateTypeDef;                             /* 代码块结束 */

/**
  * @brief SPDIFRX handle Structure definition
  */
#if (USE_HAL_SPDIFRX_REGISTER_CALLBACKS == 1)           /* 条件编译 */
typedef struct __SPDIFRX_HandleTypeDef                  /* 类型定义 */
#else                                                   /* 条件编译否则 */
typedef struct                                          /* 类型定义 */
#endif                                                  /* 条件编译结束 */
{                                                       /* 代码块开始 */
  SPDIFRX_TypeDef            *Instance;    /* SPDIFRX registers base address */

  SPDIFRX_InitTypeDef        Init;         /* SPDIFRX communication parameters */

  uint32_t                   *pRxBuffPtr;  /* Pointer to SPDIFRX Rx transfer buffer */

  uint32_t                   *pCsBuffPtr;  /* Pointer to SPDIFRX Cx transfer buffer */

  __IO uint16_t              RxXferSize;   /* SPDIFRX Rx transfer size */

  __IO uint16_t              RxXferCount;  /* SPDIFRX Rx transfer counter
                                              (This field is initialized at the
                                               same value as transfer size at the
                                               beginning of the transfer and
                                               decremented when a sample is received.
                                               NbSamplesReceived = RxBufferSize-RxBufferCount) */

  __IO uint16_t              CsXferSize;   /* SPDIFRX Rx transfer size */

  __IO uint16_t              CsXferCount;  /* SPDIFRX Rx transfer counter
                                              (This field is initialized at the
                                               same value as transfer size at the
                                               beginning of the transfer and
                                               decremented when a sample is received.
                                               NbSamplesReceived = RxBufferSize-RxBufferCount) */

  DMA_HandleTypeDef          *hdmaCsRx;    /* SPDIFRX EC60958_channel_status and user_information DMA handle parameters */

  DMA_HandleTypeDef          *hdmaDrRx;    /* SPDIFRX Rx DMA handle parameters */

  __IO HAL_LockTypeDef       Lock;         /* SPDIFRX locking object */

  __IO HAL_SPDIFRX_StateTypeDef  State;    /* SPDIFRX communication state */

  __IO uint32_t  ErrorCode;                /* SPDIFRX Error code */

#if (USE_HAL_SPDIFRX_REGISTER_CALLBACKS == 1)           /* 条件编译 */
  void (*RxHalfCpltCallback)(struct __SPDIFRX_HandleTypeDef *hspdif);   /*!< SPDIFRX Data flow half completed callback */
  void (*RxCpltCallback)(struct __SPDIFRX_HandleTypeDef *hspdif);       /*!< SPDIFRX Data flow completed callback */
  void (*CxHalfCpltCallback)(struct __SPDIFRX_HandleTypeDef *hspdif);   /*!< SPDIFRX Control flow half completed callback */
  void (*CxCpltCallback)(struct __SPDIFRX_HandleTypeDef *hspdif);       /*!< SPDIFRX Control flow completed callback */
  void (*ErrorCallback)(struct __SPDIFRX_HandleTypeDef *hspdif);        /*!< SPDIFRX error callback */
  void (* MspInitCallback)( struct __SPDIFRX_HandleTypeDef * hspdif);   /*!< SPDIFRX Msp Init callback  */
  void (* MspDeInitCallback)( struct __SPDIFRX_HandleTypeDef * hspdif); /*!< SPDIFRX Msp DeInit callback  */
#endif /* USE_HAL_SPDIFRX_REGISTER_CALLBACKS */

} SPDIFRX_HandleTypeDef;                                /* 代码块结束 */
/**
  * @}
  */

#if (USE_HAL_SPDIFRX_REGISTER_CALLBACKS == 1)           /* 条件编译 */
/**
  * @brief  HAL SPDIFRX Callback ID enumeration definition
  */
typedef enum                                            /* 类型定义 */
{                                                       /* 代码块开始 */
  HAL_SPDIFRX_RX_HALF_CB_ID   = 0x00U,    /*!< SPDIFRX Data flow half completed callback ID */
  HAL_SPDIFRX_RX_CPLT_CB_ID   = 0x01U,    /*!< SPDIFRX Data flow completed callback */
  HAL_SPDIFRX_CX_HALF_CB_ID   = 0x02U,    /*!< SPDIFRX Control flow half completed callback */
  HAL_SPDIFRX_CX_CPLT_CB_ID   = 0x03U,    /*!< SPDIFRX Control flow completed callback */
  HAL_SPDIFRX_ERROR_CB_ID     = 0x04U,    /*!< SPDIFRX error callback */
  HAL_SPDIFRX_MSPINIT_CB_ID   = 0x05U,    /*!< SPDIFRX Msp Init callback ID     */
  HAL_SPDIFRX_MSPDEINIT_CB_ID = 0x06U     /*!< SPDIFRX Msp DeInit callback ID   */
}HAL_SPDIFRX_CallbackIDTypeDef;                         /* 代码块结束 */

/**
  * @brief  HAL SPDIFRX Callback pointer definition
  */
typedef  void (*pSPDIFRX_CallbackTypeDef)(SPDIFRX_HandleTypeDef * hspdif); /*!< pointer to an SPDIFRX callback function */
#endif /* USE_HAL_SPDIFRX_REGISTER_CALLBACKS */

/* Exported constants --------------------------------------------------------*/
/** @defgroup SPDIFRX_Exported_Constants SPDIFRX Exported Constants
  * @{
  */
/** @defgroup SPDIFRX_ErrorCode SPDIFRX Error Code
  * @{
  */
#define HAL_SPDIFRX_ERROR_NONE      ((uint32_t)0x00000000U)  /*!< No error           */
#define HAL_SPDIFRX_ERROR_TIMEOUT   ((uint32_t)0x00000001U)  /*!< Timeout error      */
#define HAL_SPDIFRX_ERROR_OVR       ((uint32_t)0x00000002U)  /*!< OVR error          */
#define HAL_SPDIFRX_ERROR_PE        ((uint32_t)0x00000004U)  /*!< Parity error       */
#define HAL_SPDIFRX_ERROR_DMA       ((uint32_t)0x00000008U)  /*!< DMA transfer error */
#define HAL_SPDIFRX_ERROR_UNKNOWN   ((uint32_t)0x00000010U)  /*!< Unknown Error error */
#if (USE_HAL_SPDIFRX_REGISTER_CALLBACKS == 1)           /* 条件编译 */
#define  HAL_SPDIFRX_ERROR_INVALID_CALLBACK   ((uint32_t)0x00000020U)    /*!< Invalid Callback error  */
#endif /* USE_HAL_SPDIFRX_REGISTER_CALLBACKS */
/**
  * @}
  */

/** @defgroup SPDIFRX_Input_Selection SPDIFRX Input Selection
  * @{
  */
#define SPDIFRX_INPUT_IN0   ((uint32_t)0x00000000U)     /* 定义宏 */
#define SPDIFRX_INPUT_IN1   ((uint32_t)0x00010000U)     /* 定义宏 */
#define SPDIFRX_INPUT_IN2   ((uint32_t)0x00020000U)     /* 定义宏 */
#define SPDIFRX_INPUT_IN3   ((uint32_t)0x00030000U)     /* 定义宏 */
/**
  * @}
  */

/** @defgroup SPDIFRX_Max_Retries SPDIFRX Maximum Retries
  * @{
  */
#define SPDIFRX_MAXRETRIES_NONE   ((uint32_t)0x00000000U)  /* 定义宏 */
#define SPDIFRX_MAXRETRIES_3      ((uint32_t)0x00001000U)  /* 定义宏 */
#define SPDIFRX_MAXRETRIES_15     ((uint32_t)0x00002000U)  /* 定义宏 */
#define SPDIFRX_MAXRETRIES_63     ((uint32_t)0x00003000U)  /* 定义宏 */
/**
  * @}
  */

/** @defgroup SPDIFRX_Wait_For_Activity SPDIFRX Wait For Activity
  * @{
  */
#define SPDIFRX_WAITFORACTIVITY_OFF   ((uint32_t)0x00000000U)  /* 定义宏 */
#define SPDIFRX_WAITFORACTIVITY_ON    ((uint32_t)SPDIFRX_CR_WFA)  /* 定义宏 */
/**
  * @}
  */

/** @defgroup SPDIFRX_PT_Mask SPDIFRX Preamble Type Mask
  * @{
  */
#define SPDIFRX_PREAMBLETYPEMASK_OFF    ((uint32_t)0x00000000U)  /* 定义宏 */
#define SPDIFRX_PREAMBLETYPEMASK_ON     ((uint32_t)SPDIFRX_CR_PTMSK)  /* 定义宏 */
/**
  * @}
  */

/** @defgroup SPDIFRX_ChannelStatus_Mask  SPDIFRX Channel Status Mask
  * @{
  */
#define SPDIFRX_CHANNELSTATUS_OFF   ((uint32_t)0x00000000U)        /* The channel status and user bits are copied into the SPDIF_DR */
#define SPDIFRX_CHANNELSTATUS_ON    ((uint32_t)SPDIFRX_CR_CUMSK)  /* The channel status and user bits are not copied into the SPDIF_DR, zeros are written instead*/
/**
  * @}
  */

/** @defgroup SPDIFRX_V_Mask SPDIFRX Validity Mask
* @{
*/
#define SPDIFRX_VALIDITYMASK_OFF    ((uint32_t)0x00000000U)  /* 定义宏 */
#define SPDIFRX_VALIDITYMASK_ON     ((uint32_t)SPDIFRX_CR_VMSK)  /* 定义宏 */
/**
  * @}
  */

/** @defgroup SPDIFRX_PE_Mask  SPDIFRX Parity Error Mask
  * @{
  */
#define SPDIFRX_PARITYERRORMASK_OFF   ((uint32_t)0x00000000U)  /* 定义宏 */
#define SPDIFRX_PARITYERRORMASK_ON    ((uint32_t)SPDIFRX_CR_PMSK)  /* 定义宏 */
/**
  * @}
  */

/** @defgroup SPDIFRX_Channel_Selection  SPDIFRX Channel Selection
  * @{
  */
#define SPDIFRX_CHANNEL_A   ((uint32_t)0x00000000U)     /* 定义宏 */
#define SPDIFRX_CHANNEL_B   ((uint32_t)SPDIFRX_CR_CHSEL)  /* 定义宏 */
/**
  * @}
  */

/** @defgroup SPDIFRX_Data_Format SPDIFRX Data Format
  * @{
  */
#define SPDIFRX_DATAFORMAT_LSB      ((uint32_t)0x00000000U)  /* 定义宏 */
#define SPDIFRX_DATAFORMAT_MSB      ((uint32_t)0x00000010U)  /* 定义宏 */
#define SPDIFRX_DATAFORMAT_32BITS   ((uint32_t)0x00000020U)  /* 定义宏 */
/**
  * @}
  */

/** @defgroup SPDIFRX_Stereo_Mode SPDIFRX Stereo Mode
  * @{
  */
#define SPDIFRX_STEREOMODE_DISABLE    ((uint32_t)0x00000000U)  /* 定义宏 */
#define SPDIFRX_STEREOMODE_ENABLE     ((uint32_t)SPDIFRX_CR_RXSTEO)  /* 定义宏 */
/**
  * @}
  */

/** @defgroup SPDIFRX_State SPDIFRX State
  * @{
  */

#define SPDIFRX_STATE_IDLE    ((uint32_t)0xFFFFFFFCU)   /* 定义宏 */
#define SPDIFRX_STATE_SYNC    ((uint32_t)0x00000001U)   /* 定义宏 */
#define SPDIFRX_STATE_RCV     ((uint32_t)SPDIFRX_CR_SPDIFEN)  /* 定义宏 */
/**
  * @}
  */

/** @defgroup SPDIFRX_Interrupts_Definition SPDIFRX Interrupts Definition
  * @{
  */
#define SPDIFRX_IT_RXNE       ((uint32_t)SPDIFRX_IMR_RXNEIE)  /* 定义宏 */
#define SPDIFRX_IT_CSRNE      ((uint32_t)SPDIFRX_IMR_CSRNEIE)  /* 定义宏 */
#define SPDIFRX_IT_PERRIE     ((uint32_t)SPDIFRX_IMR_PERRIE)  /* 定义宏 */
#define SPDIFRX_IT_OVRIE      ((uint32_t)SPDIFRX_IMR_OVRIE)  /* 定义宏 */
#define SPDIFRX_IT_SBLKIE     ((uint32_t)SPDIFRX_IMR_SBLKIE)  /* 定义宏 */
#define SPDIFRX_IT_SYNCDIE    ((uint32_t)SPDIFRX_IMR_SYNCDIE)  /* 定义宏 */
#define SPDIFRX_IT_IFEIE      ((uint32_t)SPDIFRX_IMR_IFEIE )  /* 定义宏 */
/**
  * @}
  */

/** @defgroup SPDIFRX_Flags_Definition SPDIFRX Flags Definition
  * @{
  */
#define SPDIFRX_FLAG_RXNE     ((uint32_t)SPDIFRX_SR_RXNE)  /* 定义宏 */
#define SPDIFRX_FLAG_CSRNE    ((uint32_t)SPDIFRX_SR_CSRNE)  /* 定义宏 */
#define SPDIFRX_FLAG_PERR     ((uint32_t)SPDIFRX_SR_PERR)  /* 定义宏 */
#define SPDIFRX_FLAG_OVR      ((uint32_t)SPDIFRX_SR_OVR)  /* 定义宏 */
#define SPDIFRX_FLAG_SBD      ((uint32_t)SPDIFRX_SR_SBD)  /* 定义宏 */
#define SPDIFRX_FLAG_SYNCD    ((uint32_t)SPDIFRX_SR_SYNCD)  /* 定义宏 */
#define SPDIFRX_FLAG_FERR     ((uint32_t)SPDIFRX_SR_FERR)  /* 定义宏 */
#define SPDIFRX_FLAG_SERR     ((uint32_t)SPDIFRX_SR_SERR)  /* 定义宏 */
#define SPDIFRX_FLAG_TERR     ((uint32_t)SPDIFRX_SR_TERR)  /* 定义宏 */
/**
  * @}
  */

/**
  * @}
  */

/* Exported macros -----------------------------------------------------------*/
/** @defgroup SPDIFRX_Exported_macros SPDIFRX Exported Macros
  * @{
  */

/** @brief  Reset SPDIFRX handle state
  * @param  __HANDLE__ SPDIFRX handle.
  * @retval None
  */
#if (USE_HAL_SPDIFRX_REGISTER_CALLBACKS == 1)           /* 条件编译 */
#define __HAL_SPDIFRX_RESET_HANDLE_STATE(__HANDLE__) do{\
                                                      (__HANDLE__)->State = HAL_SPDIFRX_STATE_RESET;\
                                                      (__HANDLE__)->MspInitCallback = NULL;\
                                                      (__HANDLE__)->MspDeInitCallback = NULL;\
                                                     }while(0)  /* 代码块结束 */
#else                                                   /* 条件编译否则 */
#define __HAL_SPDIFRX_RESET_HANDLE_STATE(__HANDLE__) ((__HANDLE__)->State = HAL_SPDIFRX_STATE_RESET)  /* 定义包含保护宏 */
#endif /* USE_HAL_SPDIFRX_REGISTER_CALLBACKS */

/** @brief  Disable the specified SPDIFRX peripheral (IDLE State).
  * @param  __HANDLE__ specifies the SPDIFRX Handle.
  * @retval None
  */
#define __HAL_SPDIFRX_IDLE(__HANDLE__) ((__HANDLE__)->Instance->CR &= SPDIFRX_STATE_IDLE)  /* 定义包含保护宏 */

/** @brief  Enable the specified SPDIFRX peripheral (SYNC State).
  * @param  __HANDLE__ specifies the SPDIFRX Handle.
  * @retval None
  */
#define __HAL_SPDIFRX_SYNC(__HANDLE__) ((__HANDLE__)->Instance->CR |= SPDIFRX_STATE_SYNC)  /* 定义包含保护宏 */


/** @brief  Enable the specified SPDIFRX peripheral (RCV State).
  * @param  __HANDLE__ specifies the SPDIFRX Handle.
  * @retval None
  */
#define __HAL_SPDIFRX_RCV(__HANDLE__) ((__HANDLE__)->Instance->CR |= SPDIFRX_STATE_RCV)  /* 定义包含保护宏 */


/** @brief  Enable or disable the specified SPDIFRX interrupts.
  * @param  __HANDLE__    specifies the SPDIFRX Handle.
  * @param  __INTERRUPT__ specifies the interrupt source to enable or disable.
  *        This parameter can be one of the following values:
  *            @arg SPDIFRX_IT_RXNE
  *            @arg SPDIFRX_IT_CSRNE
  *            @arg SPDIFRX_IT_PERRIE
  *            @arg SPDIFRX_IT_OVRIE
  *            @arg SPDIFRX_IT_SBLKIE
  *            @arg SPDIFRX_IT_SYNCDIE
  *            @arg SPDIFRX_IT_IFEIE
  * @retval None
  */
#define __HAL_SPDIFRX_ENABLE_IT(__HANDLE__, __INTERRUPT__) ((__HANDLE__)->Instance->IMR |= (__INTERRUPT__))  /* 定义包含保护宏 */
#define __HAL_SPDIFRX_DISABLE_IT(__HANDLE__, __INTERRUPT__) ((__HANDLE__)->Instance->IMR &= (uint16_t)(~(__INTERRUPT__)))  /* 定义包含保护宏 */

/** @brief  Checks if the specified SPDIFRX interrupt source is enabled or disabled.
  * @param  __HANDLE__    specifies the SPDIFRX Handle.
  * @param  __INTERRUPT__ specifies the SPDIFRX interrupt source to check.
  *          This parameter can be one of the following values:
  *            @arg SPDIFRX_IT_RXNE
  *            @arg SPDIFRX_IT_CSRNE
  *            @arg SPDIFRX_IT_PERRIE
  *            @arg SPDIFRX_IT_OVRIE
  *            @arg SPDIFRX_IT_SBLKIE
  *            @arg SPDIFRX_IT_SYNCDIE
  *            @arg SPDIFRX_IT_IFEIE
  * @retval The new state of __IT__ (TRUE or FALSE).
  */
#define __HAL_SPDIFRX_GET_IT_SOURCE(__HANDLE__, __INTERRUPT__) ((((__HANDLE__)->Instance->IMR & (__INTERRUPT__)) == (__INTERRUPT__)) ? SET : RESET)  /* 定义包含保护宏 */

/** @brief  Checks whether the specified SPDIFRX flag is set or not.
  * @param  __HANDLE__  specifies the SPDIFRX Handle.
  * @param  __FLAG__    specifies the flag to check.
  *        This parameter can be one of the following values:
  *            @arg SPDIFRX_FLAG_RXNE
  *            @arg SPDIFRX_FLAG_CSRNE
  *            @arg SPDIFRX_FLAG_PERR
  *            @arg SPDIFRX_FLAG_OVR
  *            @arg SPDIFRX_FLAG_SBD
  *            @arg SPDIFRX_FLAG_SYNCD
  *            @arg SPDIFRX_FLAG_FERR
  *            @arg SPDIFRX_FLAG_SERR
  *            @arg SPDIFRX_FLAG_TERR
  * @retval The new state of __FLAG__ (TRUE or FALSE).
  */
#define __HAL_SPDIFRX_GET_FLAG(__HANDLE__, __FLAG__) (((((__HANDLE__)->Instance->SR) & (__FLAG__)) == (__FLAG__)) ? SET : RESET)  /* 定义包含保护宏 */

/** @brief  Clears the specified SPDIFRX SR flag, in setting the proper IFCR register bit.
  * @param  __HANDLE__    specifies the USART Handle.
  * @param  __IT_CLEAR__  specifies the interrupt clear register flag that needs to be set
  *                       to clear the corresponding interrupt
  *          This parameter can be one of the following values:
  *            @arg SPDIFRX_FLAG_PERR
  *            @arg SPDIFRX_FLAG_OVR
  *            @arg SPDIFRX_SR_SBD
  *            @arg SPDIFRX_SR_SYNCD
  * @retval None
  */
#define __HAL_SPDIFRX_CLEAR_IT(__HANDLE__, __IT_CLEAR__) ((__HANDLE__)->Instance->IFCR = (uint32_t)(__IT_CLEAR__))  /* 定义包含保护宏 */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup SPDIFRX_Exported_Functions
  * @{
  */

/** @addtogroup SPDIFRX_Exported_Functions_Group1
  * @{
  */
/* Initialization/de-initialization functions  **********************************/
HAL_StatusTypeDef HAL_SPDIFRX_Init(SPDIFRX_HandleTypeDef *hspdif);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_SPDIFRX_DeInit (SPDIFRX_HandleTypeDef *hspdif);  /* HAL状态类型函数 */
void HAL_SPDIFRX_MspInit(SPDIFRX_HandleTypeDef *hspdif);  /* 函数定义或声明 */
void HAL_SPDIFRX_MspDeInit(SPDIFRX_HandleTypeDef *hspdif);  /* 函数定义或声明 */
HAL_StatusTypeDef HAL_SPDIFRX_SetDataFormat(SPDIFRX_HandleTypeDef *hspdif, SPDIFRX_SetDataFormatTypeDef  sDataFormat);  /* HAL状态类型函数 */

/* Callbacks Register/UnRegister functions  ***********************************/
#if (USE_HAL_SPDIFRX_REGISTER_CALLBACKS == 1)           /* 条件编译 */
HAL_StatusTypeDef HAL_SPDIFRX_RegisterCallback(SPDIFRX_HandleTypeDef *hspdif, HAL_SPDIFRX_CallbackIDTypeDef CallbackID, pSPDIFRX_CallbackTypeDef pCallback);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_SPDIFRX_UnRegisterCallback(SPDIFRX_HandleTypeDef *hspdif, HAL_SPDIFRX_CallbackIDTypeDef CallbackID);  /* HAL状态类型函数 */
#endif /* USE_HAL_SPDIFRX_REGISTER_CALLBACKS */
/**
  * @}
  */

/** @addtogroup SPDIFRX_Exported_Functions_Group2
  * @{
  */
/* I/O operation functions  ***************************************************/
 /* Blocking mode: Polling */
HAL_StatusTypeDef HAL_SPDIFRX_ReceiveDataFlow(SPDIFRX_HandleTypeDef *hspdif, uint32_t *pData, uint16_t Size, uint32_t Timeout);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_SPDIFRX_ReceiveControlFlow(SPDIFRX_HandleTypeDef *hspdif, uint32_t *pData, uint16_t Size, uint32_t Timeout);  /* HAL状态类型函数 */

/* Non-Blocking mode: Interrupt */
HAL_StatusTypeDef HAL_SPDIFRX_ReceiveControlFlow_IT(SPDIFRX_HandleTypeDef *hspdif, uint32_t *pData, uint16_t Size);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_SPDIFRX_ReceiveDataFlow_IT(SPDIFRX_HandleTypeDef *hspdif, uint32_t *pData, uint16_t Size);  /* HAL状态类型函数 */
void HAL_SPDIFRX_IRQHandler(SPDIFRX_HandleTypeDef *hspdif);  /* 中断服务函数 */

/* Non-Blocking mode: DMA */
HAL_StatusTypeDef HAL_SPDIFRX_ReceiveControlFlow_DMA(SPDIFRX_HandleTypeDef *hspdif, uint32_t *pData, uint16_t Size);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_SPDIFRX_ReceiveDataFlow_DMA(SPDIFRX_HandleTypeDef *hspdif, uint32_t *pData, uint16_t Size);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_SPDIFRX_DMAStop(SPDIFRX_HandleTypeDef *hspdif);  /* HAL状态类型函数 */

/* Callbacks used in non blocking modes (Interrupt and DMA) *******************/
void HAL_SPDIFRX_RxHalfCpltCallback(SPDIFRX_HandleTypeDef *hspdif);  /* 函数定义或声明 */
void HAL_SPDIFRX_RxCpltCallback(SPDIFRX_HandleTypeDef *hspdif);  /* 函数定义或声明 */
void HAL_SPDIFRX_ErrorCallback(SPDIFRX_HandleTypeDef *hspdif);  /* 函数定义或声明 */
void HAL_SPDIFRX_CxHalfCpltCallback(SPDIFRX_HandleTypeDef *hspdif);  /* 函数定义或声明 */
void HAL_SPDIFRX_CxCpltCallback(SPDIFRX_HandleTypeDef *hspdif);  /* 函数定义或声明 */
/**
  * @}
  */

/** @addtogroup SPDIFRX_Exported_Functions_Group3
  * @{
  */
/* Peripheral Control and State functions  ************************************/
HAL_SPDIFRX_StateTypeDef HAL_SPDIFRX_GetState(SPDIFRX_HandleTypeDef const * const hspdif);  /* HAL库函数调用 */
uint32_t HAL_SPDIFRX_GetError(SPDIFRX_HandleTypeDef const * const hspdif);  /* 32位无符号变量/函数 */
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
/** @defgroup SPDIFRX_Private_Macros SPDIFRX Private Macros
  * @{
  */
#define IS_SPDIFRX_INPUT_SELECT(INPUT)      (((INPUT) == SPDIFRX_INPUT_IN1) || \
                                            ((INPUT) == SPDIFRX_INPUT_IN2)  || \
                                            ((INPUT) == SPDIFRX_INPUT_IN3)  || \
                                            ((INPUT) == SPDIFRX_INPUT_IN0))  /* 源代码 */

#define IS_SPDIFRX_MAX_RETRIES(RET)         (((RET) == SPDIFRX_MAXRETRIES_NONE) || \
                                            ((RET) == SPDIFRX_MAXRETRIES_3)     || \
                                            ((RET) == SPDIFRX_MAXRETRIES_15)    || \
                                            ((RET) == SPDIFRX_MAXRETRIES_63))  /* 源代码 */

#define IS_SPDIFRX_WAIT_FOR_ACTIVITY(VAL)   (((VAL) == SPDIFRX_WAITFORACTIVITY_ON) || \
                                            ((VAL) == SPDIFRX_WAITFORACTIVITY_OFF))  /* 源代码 */

#define IS_PREAMBLE_TYPE_MASK(VAL)          (((VAL) == SPDIFRX_PREAMBLETYPEMASK_ON) || \
                                            ((VAL) == SPDIFRX_PREAMBLETYPEMASK_OFF))  /* 源代码 */

#define IS_VALIDITY_MASK(VAL)               (((VAL) == SPDIFRX_VALIDITYMASK_OFF) || \
                                            ((VAL) == SPDIFRX_VALIDITYMASK_ON))  /* 源代码 */

#define IS_PARITY_ERROR_MASK(VAL)           (((VAL) == SPDIFRX_PARITYERRORMASK_OFF) || \
                                            ((VAL) == SPDIFRX_PARITYERRORMASK_ON))  /* 源代码 */

#define IS_SPDIFRX_CHANNEL(CHANNEL)         (((CHANNEL) == SPDIFRX_CHANNEL_A) || \
                                            ((CHANNEL) == SPDIFRX_CHANNEL_B))  /* 源代码 */

#define IS_SPDIFRX_DATA_FORMAT(FORMAT)      (((FORMAT) == SPDIFRX_DATAFORMAT_LSB) || \
                                            ((FORMAT) == SPDIFRX_DATAFORMAT_MSB)  || \
                                            ((FORMAT) == SPDIFRX_DATAFORMAT_32BITS))  /* 源代码 */

#define IS_STEREO_MODE(MODE)                (((MODE) == SPDIFRX_STEREOMODE_DISABLE) || \
                                            ((MODE) == SPDIFRX_STEREOMODE_ENABLE))  /* 源代码 */

#define IS_CHANNEL_STATUS_MASK(VAL)         (((VAL) == SPDIFRX_CHANNELSTATUS_ON) || \
                                            ((VAL) == SPDIFRX_CHANNELSTATUS_OFF))  /* 源代码 */

/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/
/** @defgroup SPDIFRX_Private_Functions SPDIFRX Private Functions
  * @{
  */
/**
  * @}
  */

/**
  * @}
  */
#endif /* SPDIFRX */
/**
  * @}
  */
#endif /* STM32F446xx */

#ifdef __cplusplus                                      /* 条件编译 */
}                                                       /* 代码块结束 */
#endif                                                  /* 条件编译结束 */


#endif /* __STM32F4xx_HAL_SPDIFRX_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
