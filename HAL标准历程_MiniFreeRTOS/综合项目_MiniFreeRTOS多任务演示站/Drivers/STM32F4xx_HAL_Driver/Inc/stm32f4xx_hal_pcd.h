/* 文件: stm32f4xx_hal_pcd.h | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_pcd.h
  * @author  MCD Application Team
  * @brief   Header file of PCD HAL module.
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
#ifndef STM32F4xx_HAL_PCD_H                             /* 防止头文件重复包含 */
#define STM32F4xx_HAL_PCD_H                             /* 定义宏 */

#ifdef __cplusplus                                      /* 条件编译 */
extern "C" {                                            /* 外部符号声明 */
#endif                                                  /* 条件编译结束 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_ll_usb.h"                           /* 包含 STM32/HAL 头文件 */

#if defined (USB_OTG_FS) || defined (USB_OTG_HS)        /* 条件编译 */

/** @addtogroup STM32F4xx_HAL_Driver
  * @{
  */

/** @addtogroup PCD
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup PCD_Exported_Types PCD Exported Types
  * @{
  */

/**
  * @brief  PCD State structure definition
  */
typedef enum                                            /* 类型定义 */
{                                                       /* 代码块开始 */
  HAL_PCD_STATE_RESET   = 0x00,                         /* HAL库函数调用 */
  HAL_PCD_STATE_READY   = 0x01,                         /* HAL库函数调用 */
  HAL_PCD_STATE_ERROR   = 0x02,                         /* HAL库函数调用 */
  HAL_PCD_STATE_BUSY    = 0x03,                         /* HAL库函数调用 */
  HAL_PCD_STATE_TIMEOUT = 0x04                          /* HAL库函数调用 */
} PCD_StateTypeDef;                                     /* 代码块结束 */

/* Device LPM suspend state */
typedef enum                                            /* 类型定义 */
{                                                       /* 代码块开始 */
  LPM_L0 = 0x00, /* on */
  LPM_L1 = 0x01, /* LPM L1 sleep */
  LPM_L2 = 0x02, /* suspend */
  LPM_L3 = 0x03, /* off */
} PCD_LPM_StateTypeDef;                                 /* 代码块结束 */

typedef enum                                            /* 类型定义 */
{                                                       /* 代码块开始 */
  PCD_LPM_L0_ACTIVE = 0x00, /* on */
  PCD_LPM_L1_ACTIVE = 0x01, /* LPM L1 sleep */
} PCD_LPM_MsgTypeDef;                                   /* 代码块结束 */

typedef enum                                            /* 类型定义 */
{                                                       /* 代码块开始 */
  PCD_BCD_ERROR                     = 0xFF,             /* 列表续行 */
  PCD_BCD_CONTACT_DETECTION         = 0xFE,             /* 列表续行 */
  PCD_BCD_STD_DOWNSTREAM_PORT       = 0xFD,             /* 列表续行 */
  PCD_BCD_CHARGING_DOWNSTREAM_PORT  = 0xFC,             /* 列表续行 */
  PCD_BCD_DEDICATED_CHARGING_PORT   = 0xFB,             /* 列表续行 */
  PCD_BCD_DISCOVERY_COMPLETED       = 0x00,             /* 列表续行 */

} PCD_BCD_MsgTypeDef;                                   /* 代码块结束 */

#if defined (USB_OTG_FS) || defined (USB_OTG_HS)        /* 条件编译 */
typedef USB_OTG_GlobalTypeDef  PCD_TypeDef;             /* 类型定义 */
typedef USB_OTG_CfgTypeDef     PCD_InitTypeDef;         /* 类型定义 */
typedef USB_OTG_EPTypeDef      PCD_EPTypeDef;           /* 类型定义 */
#endif /* defined (USB_OTG_FS) || defined (USB_OTG_HS) */

/**
  * @brief  PCD Handle Structure definition
  */
#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
typedef struct __PCD_HandleTypeDef                      /* 类型定义 */
#else                                                   /* 条件编译否则 */
typedef struct                                          /* 类型定义 */
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */
{                                                       /* 代码块开始 */
  PCD_TypeDef             *Instance;   /*!< Register base address             */
  PCD_InitTypeDef         Init;        /*!< PCD required parameters           */
  __IO uint8_t            USB_Address; /*!< USB Address                       */
  PCD_EPTypeDef           IN_ep[16];   /*!< IN endpoint parameters            */
  PCD_EPTypeDef           OUT_ep[16];  /*!< OUT endpoint parameters           */
  HAL_LockTypeDef         Lock;        /*!< PCD peripheral status             */
  __IO PCD_StateTypeDef   State;       /*!< PCD communication state           */
  __IO  uint32_t          ErrorCode;   /*!< PCD Error code                    */
  uint32_t                Setup[12];   /*!< Setup packet buffer               */
  PCD_LPM_StateTypeDef    LPM_State;   /*!< LPM State                         */
  uint32_t                BESL;                         /* 32位无符号变量/函数 */


  uint32_t lpm_active;                 /*!< Enable or disable the Link Power Management .
                                       This parameter can be set to ENABLE or DISABLE        */

  uint32_t battery_charging_active;    /*!< Enable or disable Battery charging.
                                       This parameter can be set to ENABLE or DISABLE        */
  void                    *pData;      /*!< Pointer to upper stack Handler */

#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
  void (* SOFCallback)(struct __PCD_HandleTypeDef *hpcd);                              /*!< USB OTG PCD SOF callback                */
  void (* SetupStageCallback)(struct __PCD_HandleTypeDef *hpcd);                       /*!< USB OTG PCD Setup Stage callback        */
  void (* ResetCallback)(struct __PCD_HandleTypeDef *hpcd);                            /*!< USB OTG PCD Reset callback              */
  void (* SuspendCallback)(struct __PCD_HandleTypeDef *hpcd);                          /*!< USB OTG PCD Suspend callback            */
  void (* ResumeCallback)(struct __PCD_HandleTypeDef *hpcd);                           /*!< USB OTG PCD Resume callback             */
  void (* ConnectCallback)(struct __PCD_HandleTypeDef *hpcd);                          /*!< USB OTG PCD Connect callback            */
  void (* DisconnectCallback)(struct __PCD_HandleTypeDef *hpcd);                       /*!< USB OTG PCD Disconnect callback         */

  void (* DataOutStageCallback)(struct __PCD_HandleTypeDef *hpcd, uint8_t epnum);      /*!< USB OTG PCD Data OUT Stage callback     */
  void (* DataInStageCallback)(struct __PCD_HandleTypeDef *hpcd, uint8_t epnum);       /*!< USB OTG PCD Data IN Stage callback      */
  void (* ISOOUTIncompleteCallback)(struct __PCD_HandleTypeDef *hpcd, uint8_t epnum);  /*!< USB OTG PCD ISO OUT Incomplete callback */
  void (* ISOINIncompleteCallback)(struct __PCD_HandleTypeDef *hpcd, uint8_t epnum);   /*!< USB OTG PCD ISO IN Incomplete callback  */
  void (* BCDCallback)(struct __PCD_HandleTypeDef *hpcd, PCD_BCD_MsgTypeDef msg);      /*!< USB OTG PCD BCD callback                */
  void (* LPMCallback)(struct __PCD_HandleTypeDef *hpcd, PCD_LPM_MsgTypeDef msg);      /*!< USB OTG PCD LPM callback                */

  void (* MspInitCallback)(struct __PCD_HandleTypeDef *hpcd);                          /*!< USB OTG PCD Msp Init callback           */
  void (* MspDeInitCallback)(struct __PCD_HandleTypeDef *hpcd);                        /*!< USB OTG PCD Msp DeInit callback         */
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */
} PCD_HandleTypeDef;                                    /* 代码块结束 */

/**
  * @}
  */

/* Include PCD HAL Extended module */
#include "stm32f4xx_hal_pcd_ex.h"                       /* 包含 STM32/HAL 头文件 */

/* Exported constants --------------------------------------------------------*/
/** @defgroup PCD_Exported_Constants PCD Exported Constants
  * @{
  */

/** @defgroup PCD_Speed PCD Speed
  * @{
  */
#define PCD_SPEED_HIGH               USBD_HS_SPEED      /* 定义宏 */
#define PCD_SPEED_HIGH_IN_FULL       USBD_HSINFS_SPEED  /* 定义宏 */
#define PCD_SPEED_FULL               USBD_FS_SPEED      /* 定义宏 */
/**
  * @}
  */

/** @defgroup PCD_PHY_Module PCD PHY Module
  * @{
  */
#define PCD_PHY_ULPI                 1U                 /* 定义宏 */
#define PCD_PHY_EMBEDDED             2U                 /* 定义宏 */
#define PCD_PHY_UTMI                 3U                 /* 定义宏 */
/**
  * @}
  */

/** @defgroup PCD_Error_Code_definition PCD Error Code definition
  * @brief  PCD Error Code definition
  * @{
  */
#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
#define  HAL_PCD_ERROR_INVALID_CALLBACK                        (0x00000010U)    /*!< Invalid Callback error  */
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */

/**
  * @}
  */

/**
  * @}
  */

/* Exported macros -----------------------------------------------------------*/
/** @defgroup PCD_Exported_Macros PCD Exported Macros
  *  @brief macros to handle interrupts and specific clock configurations
  * @{
  */
#if defined (USB_OTG_FS) || defined (USB_OTG_HS)        /* 条件编译 */
#define __HAL_PCD_ENABLE(__HANDLE__)                       (void)USB_EnableGlobalInt ((__HANDLE__)->Instance)  /* 定义包含保护宏 */
#define __HAL_PCD_DISABLE(__HANDLE__)                      (void)USB_DisableGlobalInt ((__HANDLE__)->Instance)  /* 定义包含保护宏 */

#define __HAL_PCD_GET_FLAG(__HANDLE__, __INTERRUPT__)      ((USB_ReadInterrupts((__HANDLE__)->Instance) & (__INTERRUPT__)) == (__INTERRUPT__))  /* 定义包含保护宏 */
#define __HAL_PCD_CLEAR_FLAG(__HANDLE__, __INTERRUPT__)    (((__HANDLE__)->Instance->GINTSTS) &=  (__INTERRUPT__))  /* 定义包含保护宏 */
#define __HAL_PCD_IS_INVALID_INTERRUPT(__HANDLE__)         (USB_ReadInterrupts((__HANDLE__)->Instance) == 0U)  /* 定义包含保护宏 */


#define __HAL_PCD_UNGATE_PHYCLOCK(__HANDLE__)       *(__IO uint32_t *)((uint32_t)((__HANDLE__)->Instance) + USB_OTG_PCGCCTL_BASE) &= ~(USB_OTG_PCGCCTL_STOPCLK)  /* 定义包含保护宏 */

#define __HAL_PCD_GATE_PHYCLOCK(__HANDLE__)         *(__IO uint32_t *)((uint32_t)((__HANDLE__)->Instance) + USB_OTG_PCGCCTL_BASE) |= USB_OTG_PCGCCTL_STOPCLK  /* 定义包含保护宏 */

#define __HAL_PCD_IS_PHY_SUSPENDED(__HANDLE__)      ((*(__IO uint32_t *)((uint32_t)((__HANDLE__)->Instance) + USB_OTG_PCGCCTL_BASE)) & 0x10U)  /* 定义包含保护宏 */

#define __HAL_USB_OTG_HS_WAKEUP_EXTI_ENABLE_IT()    EXTI->IMR |= (USB_OTG_HS_WAKEUP_EXTI_LINE)  /* 定义包含保护宏 */
#define __HAL_USB_OTG_HS_WAKEUP_EXTI_DISABLE_IT()   EXTI->IMR &= ~(USB_OTG_HS_WAKEUP_EXTI_LINE)  /* 定义包含保护宏 */
#define __HAL_USB_OTG_HS_WAKEUP_EXTI_GET_FLAG()     EXTI->PR & (USB_OTG_HS_WAKEUP_EXTI_LINE)  /* 定义包含保护宏 */
#define __HAL_USB_OTG_HS_WAKEUP_EXTI_CLEAR_FLAG()   EXTI->PR = (USB_OTG_HS_WAKEUP_EXTI_LINE)  /* 定义包含保护宏 */

#define __HAL_USB_OTG_HS_WAKEUP_EXTI_ENABLE_RISING_EDGE() \
  do { \
    EXTI->FTSR &= ~(USB_OTG_HS_WAKEUP_EXTI_LINE); \
    EXTI->RTSR |= USB_OTG_HS_WAKEUP_EXTI_LINE; \
  } while(0U)                                           /* 代码块结束 */
#define __HAL_USB_OTG_FS_WAKEUP_EXTI_ENABLE_IT()    EXTI->IMR |= USB_OTG_FS_WAKEUP_EXTI_LINE  /* 定义包含保护宏 */
#define __HAL_USB_OTG_FS_WAKEUP_EXTI_DISABLE_IT()   EXTI->IMR &= ~(USB_OTG_FS_WAKEUP_EXTI_LINE)  /* 定义包含保护宏 */
#define __HAL_USB_OTG_FS_WAKEUP_EXTI_GET_FLAG()     EXTI->PR & (USB_OTG_FS_WAKEUP_EXTI_LINE)  /* 定义包含保护宏 */
#define __HAL_USB_OTG_FS_WAKEUP_EXTI_CLEAR_FLAG()   EXTI->PR = USB_OTG_FS_WAKEUP_EXTI_LINE  /* 定义包含保护宏 */

#define __HAL_USB_OTG_FS_WAKEUP_EXTI_ENABLE_RISING_EDGE() \
  do { \
    EXTI->FTSR &= ~(USB_OTG_FS_WAKEUP_EXTI_LINE); \
    EXTI->RTSR |= USB_OTG_FS_WAKEUP_EXTI_LINE; \
  } while(0U)                                           /* 代码块结束 */
#endif /* defined (USB_OTG_FS) || defined (USB_OTG_HS) */


/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup PCD_Exported_Functions PCD Exported Functions
  * @{
  */

/* Initialization/de-initialization functions  ********************************/
/** @addtogroup PCD_Exported_Functions_Group1 Initialization and de-initialization functions
  * @{
  */
HAL_StatusTypeDef HAL_PCD_Init(PCD_HandleTypeDef *hpcd);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_PCD_DeInit(PCD_HandleTypeDef *hpcd);  /* HAL状态类型函数 */
void HAL_PCD_MspInit(PCD_HandleTypeDef *hpcd);          /* 函数定义或声明 */
void HAL_PCD_MspDeInit(PCD_HandleTypeDef *hpcd);        /* 函数定义或声明 */

#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
/** @defgroup HAL_PCD_Callback_ID_enumeration_definition HAL USB OTG PCD Callback ID enumeration definition
  * @brief  HAL USB OTG PCD Callback ID enumeration definition
  * @{
  */
typedef enum                                            /* 类型定义 */
{                                                       /* 代码块开始 */
  HAL_PCD_SOF_CB_ID          = 0x01,      /*!< USB PCD SOF callback ID          */
  HAL_PCD_SETUPSTAGE_CB_ID   = 0x02,      /*!< USB PCD Setup Stage callback ID  */
  HAL_PCD_RESET_CB_ID        = 0x03,      /*!< USB PCD Reset callback ID        */
  HAL_PCD_SUSPEND_CB_ID      = 0x04,      /*!< USB PCD Suspend callback ID      */
  HAL_PCD_RESUME_CB_ID       = 0x05,      /*!< USB PCD Resume callback ID       */
  HAL_PCD_CONNECT_CB_ID      = 0x06,      /*!< USB PCD Connect callback ID      */
  HAL_PCD_DISCONNECT_CB_ID   = 0x07,      /*!< USB PCD Disconnect callback ID   */

  HAL_PCD_MSPINIT_CB_ID      = 0x08,      /*!< USB PCD MspInit callback ID      */
  HAL_PCD_MSPDEINIT_CB_ID    = 0x09       /*!< USB PCD MspDeInit callback ID    */

} HAL_PCD_CallbackIDTypeDef;                            /* 代码块结束 */
/**
  * @}
  */

/** @defgroup HAL_PCD_Callback_pointer_definition HAL USB OTG PCD Callback pointer definition
  * @brief  HAL USB OTG PCD Callback pointer definition
  * @{
  */

typedef void (*pPCD_CallbackTypeDef)(PCD_HandleTypeDef *hpcd);                                   /*!< pointer to a common USB OTG PCD callback function  */
typedef void (*pPCD_DataOutStageCallbackTypeDef)(PCD_HandleTypeDef *hpcd, uint8_t epnum);        /*!< pointer to USB OTG PCD Data OUT Stage callback     */
typedef void (*pPCD_DataInStageCallbackTypeDef)(PCD_HandleTypeDef *hpcd, uint8_t epnum);         /*!< pointer to USB OTG PCD Data IN Stage callback      */
typedef void (*pPCD_IsoOutIncpltCallbackTypeDef)(PCD_HandleTypeDef *hpcd, uint8_t epnum);        /*!< pointer to USB OTG PCD ISO OUT Incomplete callback */
typedef void (*pPCD_IsoInIncpltCallbackTypeDef)(PCD_HandleTypeDef *hpcd, uint8_t epnum);         /*!< pointer to USB OTG PCD ISO IN Incomplete callback  */
typedef void (*pPCD_LpmCallbackTypeDef)(PCD_HandleTypeDef *hpcd, PCD_LPM_MsgTypeDef msg);        /*!< pointer to USB OTG PCD LPM callback                */
typedef void (*pPCD_BcdCallbackTypeDef)(PCD_HandleTypeDef *hpcd, PCD_BCD_MsgTypeDef msg);        /*!< pointer to USB OTG PCD BCD callback                */

/**
  * @}
  */

HAL_StatusTypeDef HAL_PCD_RegisterCallback(PCD_HandleTypeDef *hpcd,  /* HAL状态类型函数 */
                                           HAL_PCD_CallbackIDTypeDef CallbackID,  /* HAL库函数调用 */
                                           pPCD_CallbackTypeDef pCallback);  /* 执行语句 */

HAL_StatusTypeDef HAL_PCD_UnRegisterCallback(PCD_HandleTypeDef *hpcd,  /* HAL状态类型函数 */
                                             HAL_PCD_CallbackIDTypeDef CallbackID);  /* HAL库函数调用 */

HAL_StatusTypeDef HAL_PCD_RegisterDataOutStageCallback(PCD_HandleTypeDef *hpcd,  /* HAL状态类型函数 */
                                                       pPCD_DataOutStageCallbackTypeDef pCallback);  /* 执行语句 */

HAL_StatusTypeDef HAL_PCD_UnRegisterDataOutStageCallback(PCD_HandleTypeDef *hpcd);  /* HAL状态类型函数 */

HAL_StatusTypeDef HAL_PCD_RegisterDataInStageCallback(PCD_HandleTypeDef *hpcd,  /* HAL状态类型函数 */
                                                      pPCD_DataInStageCallbackTypeDef pCallback);  /* 执行语句 */

HAL_StatusTypeDef HAL_PCD_UnRegisterDataInStageCallback(PCD_HandleTypeDef *hpcd);  /* HAL状态类型函数 */

HAL_StatusTypeDef HAL_PCD_RegisterIsoOutIncpltCallback(PCD_HandleTypeDef *hpcd,  /* HAL状态类型函数 */
                                                       pPCD_IsoOutIncpltCallbackTypeDef pCallback);  /* 执行语句 */

HAL_StatusTypeDef HAL_PCD_UnRegisterIsoOutIncpltCallback(PCD_HandleTypeDef *hpcd);  /* HAL状态类型函数 */

HAL_StatusTypeDef HAL_PCD_RegisterIsoInIncpltCallback(PCD_HandleTypeDef *hpcd,  /* HAL状态类型函数 */
                                                      pPCD_IsoInIncpltCallbackTypeDef pCallback);  /* 执行语句 */

HAL_StatusTypeDef HAL_PCD_UnRegisterIsoInIncpltCallback(PCD_HandleTypeDef *hpcd);  /* HAL状态类型函数 */

HAL_StatusTypeDef HAL_PCD_RegisterBcdCallback(PCD_HandleTypeDef *hpcd,  /* HAL状态类型函数 */
                                              pPCD_BcdCallbackTypeDef pCallback);  /* 执行语句 */

HAL_StatusTypeDef HAL_PCD_UnRegisterBcdCallback(PCD_HandleTypeDef *hpcd);  /* HAL状态类型函数 */

HAL_StatusTypeDef HAL_PCD_RegisterLpmCallback(PCD_HandleTypeDef *hpcd,  /* HAL状态类型函数 */
                                              pPCD_LpmCallbackTypeDef pCallback);  /* 执行语句 */

HAL_StatusTypeDef HAL_PCD_UnRegisterLpmCallback(PCD_HandleTypeDef *hpcd);  /* HAL状态类型函数 */
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */
/**
  * @}
  */

/* I/O operation functions  ***************************************************/
/* Non-Blocking mode: Interrupt */
/** @addtogroup PCD_Exported_Functions_Group2 Input and Output operation functions
  * @{
  */
HAL_StatusTypeDef HAL_PCD_Start(PCD_HandleTypeDef *hpcd);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_PCD_Stop(PCD_HandleTypeDef *hpcd);  /* HAL状态类型函数 */
void HAL_PCD_IRQHandler(PCD_HandleTypeDef *hpcd);       /* 中断服务函数 */
void HAL_PCD_WKUP_IRQHandler(PCD_HandleTypeDef *hpcd);  /* 中断服务函数 */

void HAL_PCD_SOFCallback(PCD_HandleTypeDef *hpcd);      /* 函数定义或声明 */
void HAL_PCD_SetupStageCallback(PCD_HandleTypeDef *hpcd);  /* 函数定义或声明 */
void HAL_PCD_ResetCallback(PCD_HandleTypeDef *hpcd);    /* 函数定义或声明 */
void HAL_PCD_SuspendCallback(PCD_HandleTypeDef *hpcd);  /* 函数定义或声明 */
void HAL_PCD_ResumeCallback(PCD_HandleTypeDef *hpcd);   /* 函数定义或声明 */
void HAL_PCD_ConnectCallback(PCD_HandleTypeDef *hpcd);  /* 函数定义或声明 */
void HAL_PCD_DisconnectCallback(PCD_HandleTypeDef *hpcd);  /* 函数定义或声明 */

void HAL_PCD_DataOutStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum);  /* 函数定义或声明 */
void HAL_PCD_DataInStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum);  /* 函数定义或声明 */
void HAL_PCD_ISOOUTIncompleteCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum);  /* 函数定义或声明 */
void HAL_PCD_ISOINIncompleteCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum);  /* 函数定义或声明 */
/**
  * @}
  */

/* Peripheral Control functions  **********************************************/
/** @addtogroup PCD_Exported_Functions_Group3 Peripheral Control functions
  * @{
  */
HAL_StatusTypeDef HAL_PCD_DevConnect(PCD_HandleTypeDef *hpcd);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_PCD_DevDisconnect(PCD_HandleTypeDef *hpcd);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_PCD_SetAddress(PCD_HandleTypeDef *hpcd, uint8_t address);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_PCD_EP_Open(PCD_HandleTypeDef *hpcd, uint8_t ep_addr,  /* HAL状态类型函数 */
                                  uint16_t ep_mps, uint8_t ep_type);  /* 16位无符号变量/函数 */

HAL_StatusTypeDef HAL_PCD_EP_Close(PCD_HandleTypeDef *hpcd, uint8_t ep_addr);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_PCD_EP_Receive(PCD_HandleTypeDef *hpcd, uint8_t ep_addr,  /* HAL状态类型函数 */
                                     uint8_t *pBuf, uint32_t len);  /* 8位无符号变量/函数 */

HAL_StatusTypeDef HAL_PCD_EP_Transmit(PCD_HandleTypeDef *hpcd, uint8_t ep_addr,  /* HAL状态类型函数 */
                                      uint8_t *pBuf, uint32_t len);  /* 8位无符号变量/函数 */


HAL_StatusTypeDef HAL_PCD_EP_SetStall(PCD_HandleTypeDef *hpcd, uint8_t ep_addr);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_PCD_EP_ClrStall(PCD_HandleTypeDef *hpcd, uint8_t ep_addr);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_PCD_EP_Flush(PCD_HandleTypeDef *hpcd, uint8_t ep_addr);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_PCD_ActivateRemoteWakeup(PCD_HandleTypeDef *hpcd);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_PCD_DeActivateRemoteWakeup(PCD_HandleTypeDef *hpcd);  /* HAL状态类型函数 */

uint32_t          HAL_PCD_EP_GetRxCount(PCD_HandleTypeDef *hpcd, uint8_t ep_addr);  /* 32位无符号变量/函数 */
/**
  * @}
  */

/* Peripheral State functions  ************************************************/
/** @addtogroup PCD_Exported_Functions_Group4 Peripheral State functions
  * @{
  */
PCD_StateTypeDef HAL_PCD_GetState(PCD_HandleTypeDef *hpcd);  /* 执行语句 */
/**
  * @}
  */

/**
  * @}
  */

/* Private constants ---------------------------------------------------------*/
/** @defgroup PCD_Private_Constants PCD Private Constants
  * @{
  */
/** @defgroup USB_EXTI_Line_Interrupt USB EXTI line interrupt
  * @{
  */
#if defined (USB_OTG_FS) || defined (USB_OTG_HS)        /* 条件编译 */
#define USB_OTG_FS_WAKEUP_EXTI_LINE                                   (0x1U << 18)  /*!< USB FS EXTI Line WakeUp Interrupt */
#define USB_OTG_HS_WAKEUP_EXTI_LINE                                   (0x1U << 20)  /*!< USB HS EXTI Line WakeUp Interrupt */
#endif /* defined (USB_OTG_FS) || defined (USB_OTG_HS) */


/**
  * @}
  */
/**
  * @}
  */

#if defined (USB_OTG_FS) || defined (USB_OTG_HS)        /* 条件编译 */
#ifndef USB_OTG_DOEPINT_OTEPSPR                         /* 防止头文件重复包含 */
#define USB_OTG_DOEPINT_OTEPSPR                (0x1UL << 5)      /*!< Status Phase Received interrupt */
#endif                                                  /* 条件编译结束 */

#ifndef USB_OTG_DOEPMSK_OTEPSPRM                        /* 防止头文件重复包含 */
#define USB_OTG_DOEPMSK_OTEPSPRM               (0x1UL << 5)      /*!< Setup Packet Received interrupt mask */
#endif                                                  /* 条件编译结束 */

#ifndef USB_OTG_DOEPINT_NAK                             /* 防止头文件重复包含 */
#define USB_OTG_DOEPINT_NAK                    (0x1UL << 13)      /*!< NAK interrupt */
#endif                                                  /* 条件编译结束 */

#ifndef USB_OTG_DOEPMSK_NAKM                            /* 防止头文件重复包含 */
#define USB_OTG_DOEPMSK_NAKM                   (0x1UL << 13)      /*!< OUT Packet NAK interrupt mask */
#endif                                                  /* 条件编译结束 */

#ifndef USB_OTG_DOEPINT_STPKTRX                         /* 防止头文件重复包含 */
#define USB_OTG_DOEPINT_STPKTRX                (0x1UL << 15)      /*!< Setup Packet Received interrupt */
#endif                                                  /* 条件编译结束 */

#ifndef USB_OTG_DOEPMSK_NYETM                           /* 防止头文件重复包含 */
#define USB_OTG_DOEPMSK_NYETM                  (0x1UL << 14)      /*!< Setup Packet Received interrupt mask */
#endif                                                  /* 条件编译结束 */
#endif /* defined (USB_OTG_FS) || defined (USB_OTG_HS) */

/* Private macros ------------------------------------------------------------*/
/** @defgroup PCD_Private_Macros PCD Private Macros
  * @{
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
#endif /* defined (USB_OTG_FS) || defined (USB_OTG_HS) */

#ifdef __cplusplus                                      /* 条件编译 */
}                                                       /* 代码块结束 */
#endif                                                  /* 条件编译结束 */

#endif /* STM32F4xx_HAL_PCD_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
