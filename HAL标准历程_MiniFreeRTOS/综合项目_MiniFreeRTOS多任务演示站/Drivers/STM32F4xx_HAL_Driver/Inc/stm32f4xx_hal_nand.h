/* 文件: stm32f4xx_hal_nand.h | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_nand.h
  * @author  MCD Application Team
  * @brief   Header file of NAND HAL module.
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
#ifndef __STM32F4xx_HAL_NAND_H                          /* 防止头文件重复包含 */
#define __STM32F4xx_HAL_NAND_H                          /* 定义包含保护宏 */

#ifdef __cplusplus                                      /* 条件编译 */
 extern "C" {                                           /* 外部符号声明 */
#endif                                                  /* 条件编译结束 */

/* Includes ------------------------------------------------------------------*/
#if defined(STM32F405xx) || defined(STM32F415xx) || defined(STM32F407xx) || defined(STM32F417xx)  /* 条件编译 */
  #include "stm32f4xx_ll_fsmc.h"                        /* 包含 STM32/HAL 头文件 */
#endif /* STM32F405xx || STM32F415xx || STM32F407xx || STM32F417xx */

#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx) ||\
    defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx)  /* 源代码 */
  #include "stm32f4xx_ll_fmc.h"                         /* 包含 STM32/HAL 头文件 */
#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx || STM32F446xx || STM32F469xx ||\
          STM32F479xx */

/** @addtogroup STM32F4xx_HAL_Driver
  * @{
  */

/** @addtogroup NAND
  * @{
  */ 

#if defined(STM32F405xx) || defined(STM32F415xx) || defined(STM32F407xx) || defined(STM32F417xx) || \
    defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx) || \
    defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx)  /* 源代码 */

/* Exported typedef ----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/** @defgroup NAND_Exported_Types NAND Exported Types
  * @{
  */

/** 
  * @brief  HAL NAND State structures definition
  */
typedef enum                                            /* 类型定义 */
{                                                       /* 代码块开始 */
  HAL_NAND_STATE_RESET     = 0x00U,  /*!< NAND not yet initialized or disabled */
  HAL_NAND_STATE_READY     = 0x01U,  /*!< NAND initialized and ready for use   */
  HAL_NAND_STATE_BUSY      = 0x02U,  /*!< NAND internal process is ongoing     */
  HAL_NAND_STATE_ERROR     = 0x03U   /*!< NAND error state                     */
}HAL_NAND_StateTypeDef;                                 /* 代码块结束 */

/** 
  * @brief  NAND Memory electronic signature Structure definition
  */
typedef struct                                          /* 类型定义 */
{                                                       /* 代码块开始 */
  /*<! NAND memory electronic signature maker and device IDs */

  uint8_t Maker_Id;                                     /* 8位无符号变量/函数 */

  uint8_t Device_Id;                                    /* 8位无符号变量/函数 */

  uint8_t Third_Id;                                     /* 8位无符号变量/函数 */

  uint8_t Fourth_Id;                                    /* 8位无符号变量/函数 */
}NAND_IDTypeDef;                                        /* 代码块结束 */

/** 
  * @brief  NAND Memory address Structure definition
  */
typedef struct                                          /* 类型定义 */
{                                                       /* 代码块开始 */
  uint16_t Page;   /*!< NAND memory Page address    */

  uint16_t Plane;   /*!< NAND memory Plane address  */

  uint16_t Block;  /*!< NAND memory Block address   */

}NAND_AddressTypeDef;                                   /* 代码块结束 */

/** 
  * @brief  NAND Memory info Structure definition
  */ 
typedef struct                                          /* 类型定义 */
{                                                       /* 代码块开始 */
  uint32_t        PageSize;              /*!< NAND memory page (without spare area) size measured in bytes 
                                              for 8 bits adressing or words for 16 bits addressing             */

  uint32_t        SpareAreaSize;         /*!< NAND memory spare area size measured in bytes 
                                              for 8 bits adressing or words for 16 bits addressing             */

  uint32_t        BlockSize;             /*!< NAND memory block size measured in number of pages               */

  uint32_t        BlockNbr;              /*!< NAND memory number of total blocks                               */

  uint32_t        PlaneNbr;              /*!< NAND memory number of planes                                     */

  uint32_t        PlaneSize;             /*!< NAND memory plane size measured in number of blocks               */

  FunctionalState ExtraCommandEnable;    /*!< NAND extra command needed for Page reading mode. This 
                                              parameter is mandatory for some NAND parts after the read 
                                              command (NAND_CMD_AREA_TRUE1) and before DATA reading sequence. 
                                              Example: Toshiba THTH58BYG3S0HBAI6.
                                              This parameter could be ENABLE or DISABLE
                                              Please check the Read Mode sequnece in the NAND device datasheet */
}NAND_DeviceConfigTypeDef;                              /* 代码块结束 */

/** 
  * @brief  NAND handle Structure definition
  */
#if (USE_HAL_NAND_REGISTER_CALLBACKS == 1)              /* 条件编译 */
typedef struct __NAND_HandleTypeDef                     /* 类型定义 */
#else                                                   /* 条件编译否则 */
typedef struct                                          /* 类型定义 */
#endif /* USE_HAL_NAND_REGISTER_CALLBACKS  */
{                                                       /* 代码块开始 */
  FMC_NAND_TypeDef               *Instance;  /*!< Register base address                                 */

  FMC_NAND_InitTypeDef           Init;       /*!< NAND device control configuration parameters          */

  HAL_LockTypeDef                Lock;       /*!< NAND locking object                                   */

  __IO HAL_NAND_StateTypeDef     State;      /*!< NAND device access state                              */

  NAND_DeviceConfigTypeDef       Config;     /*!< NAND phusical characteristic information structure    */

#if (USE_HAL_NAND_REGISTER_CALLBACKS == 1)              /* 条件编译 */
  void  (* MspInitCallback)        ( struct __NAND_HandleTypeDef * hnand);    /*!< NAND Msp Init callback              */
  void  (* MspDeInitCallback)      ( struct __NAND_HandleTypeDef * hnand);    /*!< NAND Msp DeInit callback            */
  void  (* ItCallback)             ( struct __NAND_HandleTypeDef * hnand);    /*!< NAND IT callback                    */
#endif                                                  /* 条件编译结束 */
} NAND_HandleTypeDef;                                   /* 代码块结束 */

#if (USE_HAL_NAND_REGISTER_CALLBACKS == 1)              /* 条件编译 */
/**
  * @brief  HAL NAND Callback ID enumeration definition
  */
typedef enum                                            /* 类型定义 */
{                                                       /* 代码块开始 */
  HAL_NAND_MSP_INIT_CB_ID       = 0x00U,  /*!< NAND MspInit Callback ID          */
  HAL_NAND_MSP_DEINIT_CB_ID     = 0x01U,  /*!< NAND MspDeInit Callback ID        */
  HAL_NAND_IT_CB_ID             = 0x02U   /*!< NAND IT Callback ID               */
}HAL_NAND_CallbackIDTypeDef;                            /* 代码块结束 */

/**
  * @brief  HAL NAND Callback pointer definition
  */
typedef void (*pNAND_CallbackTypeDef)(NAND_HandleTypeDef *hnand);  /* 类型定义 */
#endif                                                  /* 条件编译结束 */

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/* Exported macros ------------------------------------------------------------*/
/** @defgroup NAND_Exported_Macros NAND Exported Macros
 * @{
 */ 

/** @brief Reset NAND handle state
  * @param  __HANDLE__ specifies the NAND handle.
  * @retval None
  */
#if (USE_HAL_NAND_REGISTER_CALLBACKS == 1)              /* 条件编译 */
#define __HAL_NAND_RESET_HANDLE_STATE(__HANDLE__)         do {                                             \
                                                               (__HANDLE__)->State = HAL_NAND_STATE_RESET; \
                                                               (__HANDLE__)->MspInitCallback = NULL;       \
                                                               (__HANDLE__)->MspDeInitCallback = NULL;     \
                                                             } while(0)  /* 代码块结束 */
#else                                                   /* 条件编译否则 */
#define __HAL_NAND_RESET_HANDLE_STATE(__HANDLE__) ((__HANDLE__)->State = HAL_NAND_STATE_RESET)  /* 定义包含保护宏 */
#endif                                                  /* 条件编译结束 */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup NAND_Exported_Functions NAND Exported Functions
  * @{
  */

/** @addtogroup NAND_Exported_Functions_Group1 Initialization and de-initialization functions 
  * @{
  */

/* Initialization/de-initialization functions  ********************************/
/* Initialization/de-initialization functions  ********************************/
HAL_StatusTypeDef  HAL_NAND_Init(NAND_HandleTypeDef *hnand, FMC_NAND_PCC_TimingTypeDef *ComSpace_Timing, FMC_NAND_PCC_TimingTypeDef *AttSpace_Timing);  /* HAL状态类型函数 */
HAL_StatusTypeDef  HAL_NAND_DeInit(NAND_HandleTypeDef *hnand);  /* HAL状态类型函数 */

HAL_StatusTypeDef  HAL_NAND_ConfigDevice(NAND_HandleTypeDef *hnand, NAND_DeviceConfigTypeDef *pDeviceConfig);  /* HAL状态类型函数 */

HAL_StatusTypeDef  HAL_NAND_Read_ID(NAND_HandleTypeDef *hnand, NAND_IDTypeDef *pNAND_ID);  /* HAL状态类型函数 */

void               HAL_NAND_MspInit(NAND_HandleTypeDef *hnand);  /* 函数定义或声明 */
void               HAL_NAND_MspDeInit(NAND_HandleTypeDef *hnand);  /* 函数定义或声明 */
void               HAL_NAND_IRQHandler(NAND_HandleTypeDef *hnand);  /* 中断服务函数 */
void               HAL_NAND_ITCallback(NAND_HandleTypeDef *hnand);  /* 函数定义或声明 */

/**
  * @}
  */

/** @addtogroup NAND_Exported_Functions_Group2 Input and Output functions 
  * @{
  */

/* IO operation functions  ****************************************************/
HAL_StatusTypeDef  HAL_NAND_Reset(NAND_HandleTypeDef *hnand);  /* HAL状态类型函数 */

HAL_StatusTypeDef  HAL_NAND_Read_Page_8b(NAND_HandleTypeDef *hnand, NAND_AddressTypeDef *pAddress, uint8_t *pBuffer, uint32_t NumPageToRead);  /* HAL状态类型函数 */
HAL_StatusTypeDef  HAL_NAND_Write_Page_8b(NAND_HandleTypeDef *hnand, NAND_AddressTypeDef *pAddress, uint8_t *pBuffer, uint32_t NumPageToWrite);  /* HAL状态类型函数 */
HAL_StatusTypeDef  HAL_NAND_Read_SpareArea_8b(NAND_HandleTypeDef *hnand, NAND_AddressTypeDef *pAddress, uint8_t *pBuffer, uint32_t NumSpareAreaToRead);  /* HAL状态类型函数 */
HAL_StatusTypeDef  HAL_NAND_Write_SpareArea_8b(NAND_HandleTypeDef *hnand, NAND_AddressTypeDef *pAddress, uint8_t *pBuffer, uint32_t NumSpareAreaTowrite);  /* HAL状态类型函数 */

HAL_StatusTypeDef  HAL_NAND_Read_Page_16b(NAND_HandleTypeDef *hnand, NAND_AddressTypeDef *pAddress, uint16_t *pBuffer, uint32_t NumPageToRead);  /* HAL状态类型函数 */
HAL_StatusTypeDef  HAL_NAND_Write_Page_16b(NAND_HandleTypeDef *hnand, NAND_AddressTypeDef *pAddress, uint16_t *pBuffer, uint32_t NumPageToWrite);  /* HAL状态类型函数 */
HAL_StatusTypeDef  HAL_NAND_Read_SpareArea_16b(NAND_HandleTypeDef *hnand, NAND_AddressTypeDef *pAddress, uint16_t *pBuffer, uint32_t NumSpareAreaToRead);  /* HAL状态类型函数 */
HAL_StatusTypeDef  HAL_NAND_Write_SpareArea_16b(NAND_HandleTypeDef *hnand, NAND_AddressTypeDef *pAddress, uint16_t *pBuffer, uint32_t NumSpareAreaTowrite);  /* HAL状态类型函数 */

HAL_StatusTypeDef  HAL_NAND_Erase_Block(NAND_HandleTypeDef *hnand, NAND_AddressTypeDef *pAddress);  /* HAL状态类型函数 */

uint32_t           HAL_NAND_Read_Status(NAND_HandleTypeDef *hnand);  /* 32位无符号变量/函数 */
uint32_t           HAL_NAND_Address_Inc(NAND_HandleTypeDef *hnand, NAND_AddressTypeDef *pAddress);  /* 32位无符号变量/函数 */

#if (USE_HAL_NAND_REGISTER_CALLBACKS == 1)              /* 条件编译 */
/* NAND callback registering/unregistering */
HAL_StatusTypeDef  HAL_NAND_RegisterCallback(NAND_HandleTypeDef *hnand, HAL_NAND_CallbackIDTypeDef CallbackId, pNAND_CallbackTypeDef pCallback);  /* HAL状态类型函数 */
HAL_StatusTypeDef  HAL_NAND_UnRegisterCallback(NAND_HandleTypeDef *hnand, HAL_NAND_CallbackIDTypeDef CallbackId);  /* HAL状态类型函数 */
#endif                                                  /* 条件编译结束 */

/**
  * @}
  */

/** @addtogroup NAND_Exported_Functions_Group3 Peripheral Control functions 
  * @{
  */

/* NAND Control functions  ****************************************************/
HAL_StatusTypeDef  HAL_NAND_ECC_Enable(NAND_HandleTypeDef *hnand);  /* HAL状态类型函数 */
HAL_StatusTypeDef  HAL_NAND_ECC_Disable(NAND_HandleTypeDef *hnand);  /* HAL状态类型函数 */
HAL_StatusTypeDef  HAL_NAND_GetECC(NAND_HandleTypeDef *hnand, uint32_t *ECCval, uint32_t Timeout);  /* HAL状态类型函数 */

/**
  * @}
  */

/** @addtogroup NAND_Exported_Functions_Group4 Peripheral State functions 
  * @{
  */
/* NAND State functions *******************************************************/
HAL_NAND_StateTypeDef HAL_NAND_GetState(NAND_HandleTypeDef *hnand);  /* HAL库函数调用 */
/**
  * @}
  */

/**
  * @}
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/** @defgroup NAND_Private_Constants NAND Private Constants
  * @{
  */
#define NAND_DEVICE1               0x70000000U          /* 定义宏 */
#define NAND_DEVICE2               0x80000000U          /* 定义宏 */
#define NAND_WRITE_TIMEOUT         0x01000000U          /* 定义宏 */

#define CMD_AREA                   ((uint32_t)(1U<<16U))  /* A16 = CLE high */
#define ADDR_AREA                  ((uint32_t)(1U<<17U))  /* A17 = ALE high */

#define NAND_CMD_AREA_A            ((uint8_t)0x00)      /* 定义宏 */
#define NAND_CMD_AREA_B            ((uint8_t)0x01)      /* 定义宏 */
#define NAND_CMD_AREA_C            ((uint8_t)0x50)      /* 定义宏 */
#define NAND_CMD_AREA_TRUE1        ((uint8_t)0x30)      /* 定义宏 */

#define NAND_CMD_WRITE0            ((uint8_t)0x80)      /* 定义宏 */
#define NAND_CMD_WRITE_TRUE1       ((uint8_t)0x10)      /* 定义宏 */
#define NAND_CMD_ERASE0            ((uint8_t)0x60)      /* 定义宏 */
#define NAND_CMD_ERASE1            ((uint8_t)0xD0)      /* 定义宏 */
#define NAND_CMD_READID            ((uint8_t)0x90)      /* 定义宏 */
#define NAND_CMD_STATUS            ((uint8_t)0x70)      /* 定义宏 */
#define NAND_CMD_LOCK_STATUS       ((uint8_t)0x7A)      /* 定义宏 */
#define NAND_CMD_RESET             ((uint8_t)0xFF)      /* 定义宏 */

/* NAND memory status */
#define NAND_VALID_ADDRESS         0x00000100U          /* 定义宏 */
#define NAND_INVALID_ADDRESS       0x00000200U          /* 定义宏 */
#define NAND_TIMEOUT_ERROR         0x00000400U          /* 定义宏 */
#define NAND_BUSY                  0x00000000U          /* 定义宏 */
#define NAND_ERROR                 0x00000001U          /* 定义宏 */
#define NAND_READY                 0x00000040U          /* 定义宏 */
/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/** @defgroup NAND_Private_Macros NAND Private Macros
  * @{
  */

/**
  * @brief  NAND memory address computation.
  * @param  __ADDRESS__ NAND memory address.
  * @param  __HANDLE__ NAND handle.
  * @retval NAND Raw address value
  */
#define ARRAY_ADDRESS(__ADDRESS__ , __HANDLE__) ((__ADDRESS__)->Page + \
                         (((__ADDRESS__)->Block + (((__ADDRESS__)->Plane) * ((__HANDLE__)->Config.PlaneSize)))* ((__HANDLE__)->Config.BlockSize)))  /* 源代码 */

/**
  * @brief  NAND memory Column address computation.
  * @param  __HANDLE__ NAND handle.
  * @retval NAND Raw address value
  */
#define COLUMN_ADDRESS( __HANDLE__) ((__HANDLE__)->Config.PageSize)  /* 定义宏 */

/**
  * @brief  NAND memory address cycling.
  * @param  __ADDRESS__ NAND memory address.
  * @retval NAND address cycling value.
  */
#define ADDR_1ST_CYCLE(__ADDRESS__)       (uint8_t)(__ADDRESS__)              /* 1st addressing cycle */
#define ADDR_2ND_CYCLE(__ADDRESS__)       (uint8_t)((__ADDRESS__) >> 8)       /* 2nd addressing cycle */
#define ADDR_3RD_CYCLE(__ADDRESS__)       (uint8_t)((__ADDRESS__) >> 16)      /* 3rd addressing cycle */
#define ADDR_4TH_CYCLE(__ADDRESS__)       (uint8_t)((__ADDRESS__) >> 24)      /* 4th addressing cycle */

/**
  * @brief  NAND memory Columns cycling.
  * @param  __ADDRESS__ NAND memory address.
  * @retval NAND Column address cycling value.
  */
#define COLUMN_1ST_CYCLE(__ADDRESS__)       (uint8_t)(__ADDRESS__)              /* 1st Column addressing cycle */
#define COLUMN_2ND_CYCLE(__ADDRESS__)       (uint8_t)((__ADDRESS__) >> 8)       /* 2nd Column addressing cycle */

/**
  * @}
  */
#endif /* STM32F405xx || STM32F415xx || STM32F407xx || STM32F417xx ||\
          STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx ||\
          STM32F446xx || STM32F469xx || STM32F479xx */

/**
  * @}
  */
/**
  * @}
  */ 

/**
  * @}
  */ 

#ifdef __cplusplus                                      /* 条件编译 */
}                                                       /* 代码块结束 */
#endif                                                  /* 条件编译结束 */

#endif /* __STM32F4xx_HAL_NAND_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
