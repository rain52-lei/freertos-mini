/* 文件: stm32f4xx_ll_iwdg.h | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_ll_iwdg.h
  * @author  MCD Application Team
  * @brief   Header file of IWDG LL module.
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
#ifndef STM32F4xx_LL_IWDG_H                             /* 防止头文件重复包含 */
#define STM32F4xx_LL_IWDG_H                             /* 定义宏 */

#ifdef __cplusplus                                      /* 条件编译 */
extern "C" {                                            /* 外部符号声明 */
#endif                                                  /* 条件编译结束 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"                                  /* 包含 STM32/HAL 头文件 */

/** @addtogroup STM32F4xx_LL_Driver
  * @{
  */

#if defined(IWDG)                                       /* 条件编译 */

/** @defgroup IWDG_LL IWDG
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private constants ---------------------------------------------------------*/
/** @defgroup IWDG_LL_Private_Constants IWDG Private Constants
  * @{
  */
#define LL_IWDG_KEY_RELOAD                 0x0000AAAAU               /*!< IWDG Reload Counter Enable   */
#define LL_IWDG_KEY_ENABLE                 0x0000CCCCU               /*!< IWDG Peripheral Enable       */
#define LL_IWDG_KEY_WR_ACCESS_ENABLE       0x00005555U               /*!< IWDG KR Write Access Enable  */
#define LL_IWDG_KEY_WR_ACCESS_DISABLE      0x00000000U               /*!< IWDG KR Write Access Disable */
/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/** @defgroup IWDG_LL_Exported_Constants IWDG Exported Constants
  * @{
  */

/** @defgroup IWDG_LL_EC_GET_FLAG Get Flags Defines
  * @brief    Flags defines which can be used with LL_IWDG_ReadReg function
  * @{
  */
#define LL_IWDG_SR_PVU                     IWDG_SR_PVU                           /*!< Watchdog prescaler value update */
#define LL_IWDG_SR_RVU                     IWDG_SR_RVU                           /*!< Watchdog counter reload value update */
/**
  * @}
  */

/** @defgroup IWDG_LL_EC_PRESCALER  Prescaler Divider
  * @{
  */
#define LL_IWDG_PRESCALER_4                0x00000000U                           /*!< Divider by 4   */
#define LL_IWDG_PRESCALER_8                (IWDG_PR_PR_0)                        /*!< Divider by 8   */
#define LL_IWDG_PRESCALER_16               (IWDG_PR_PR_1)                        /*!< Divider by 16  */
#define LL_IWDG_PRESCALER_32               (IWDG_PR_PR_1 | IWDG_PR_PR_0)         /*!< Divider by 32  */
#define LL_IWDG_PRESCALER_64               (IWDG_PR_PR_2)                        /*!< Divider by 64  */
#define LL_IWDG_PRESCALER_128              (IWDG_PR_PR_2 | IWDG_PR_PR_0)         /*!< Divider by 128 */
#define LL_IWDG_PRESCALER_256              (IWDG_PR_PR_2 | IWDG_PR_PR_1)         /*!< Divider by 256 */
/**
  * @}
  */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/** @defgroup IWDG_LL_Exported_Macros IWDG Exported Macros
  * @{
  */

/** @defgroup IWDG_LL_EM_WRITE_READ Common Write and read registers Macros
  * @{
  */

/**
  * @brief  Write a value in IWDG register
  * @param  __INSTANCE__ IWDG Instance
  * @param  __REG__ Register to be written
  * @param  __VALUE__ Value to be written in the register
  * @retval None
  */
#define LL_IWDG_WriteReg(__INSTANCE__, __REG__, __VALUE__) WRITE_REG(__INSTANCE__->__REG__, (__VALUE__))  /* 定义宏 */

/**
  * @brief  Read a value in IWDG register
  * @param  __INSTANCE__ IWDG Instance
  * @param  __REG__ Register to be read
  * @retval Register value
  */
#define LL_IWDG_ReadReg(__INSTANCE__, __REG__) READ_REG(__INSTANCE__->__REG__)  /* 定义宏 */
/**
  * @}
  */

/**
  * @}
  */


/* Exported functions --------------------------------------------------------*/
/** @defgroup IWDG_LL_Exported_Functions IWDG Exported Functions
  * @{
  */
/** @defgroup IWDG_LL_EF_Configuration Configuration
  * @{
  */

/**
  * @brief  Start the Independent Watchdog
  * @note   Except if the hardware watchdog option is selected
  * @rmtoll KR           KEY           LL_IWDG_Enable
  * @param  IWDGx IWDG Instance
  * @retval None
  */
__STATIC_INLINE void LL_IWDG_Enable(IWDG_TypeDef *IWDGx)  /* 源代码 */
{                                                       /* 代码块开始 */
  WRITE_REG(IWDGx->KR, LL_IWDG_KEY_ENABLE);             /* 执行语句 */
}                                                       /* 代码块结束 */

/**
  * @brief  Reloads IWDG counter with value defined in the reload register
  * @rmtoll KR           KEY           LL_IWDG_ReloadCounter
  * @param  IWDGx IWDG Instance
  * @retval None
  */
__STATIC_INLINE void LL_IWDG_ReloadCounter(IWDG_TypeDef *IWDGx)  /* 源代码 */
{                                                       /* 代码块开始 */
  WRITE_REG(IWDGx->KR, LL_IWDG_KEY_RELOAD);             /* 执行语句 */
}                                                       /* 代码块结束 */

/**
  * @brief  Enable write access to IWDG_PR, IWDG_RLR and IWDG_WINR registers
  * @rmtoll KR           KEY           LL_IWDG_EnableWriteAccess
  * @param  IWDGx IWDG Instance
  * @retval None
  */
__STATIC_INLINE void LL_IWDG_EnableWriteAccess(IWDG_TypeDef *IWDGx)  /* 源代码 */
{                                                       /* 代码块开始 */
  WRITE_REG(IWDGx->KR, LL_IWDG_KEY_WR_ACCESS_ENABLE);   /* 执行语句 */
}                                                       /* 代码块结束 */

/**
  * @brief  Disable write access to IWDG_PR, IWDG_RLR and IWDG_WINR registers
  * @rmtoll KR           KEY           LL_IWDG_DisableWriteAccess
  * @param  IWDGx IWDG Instance
  * @retval None
  */
__STATIC_INLINE void LL_IWDG_DisableWriteAccess(IWDG_TypeDef *IWDGx)  /* 源代码 */
{                                                       /* 代码块开始 */
  WRITE_REG(IWDGx->KR, LL_IWDG_KEY_WR_ACCESS_DISABLE);  /* 执行语句 */
}                                                       /* 代码块结束 */

/**
  * @brief  Select the prescaler of the IWDG
  * @rmtoll PR           PR            LL_IWDG_SetPrescaler
  * @param  IWDGx IWDG Instance
  * @param  Prescaler This parameter can be one of the following values:
  *         @arg @ref LL_IWDG_PRESCALER_4
  *         @arg @ref LL_IWDG_PRESCALER_8
  *         @arg @ref LL_IWDG_PRESCALER_16
  *         @arg @ref LL_IWDG_PRESCALER_32
  *         @arg @ref LL_IWDG_PRESCALER_64
  *         @arg @ref LL_IWDG_PRESCALER_128
  *         @arg @ref LL_IWDG_PRESCALER_256
  * @retval None
  */
__STATIC_INLINE void LL_IWDG_SetPrescaler(IWDG_TypeDef *IWDGx, uint32_t Prescaler)  /* 源代码 */
{                                                       /* 代码块开始 */
  WRITE_REG(IWDGx->PR, IWDG_PR_PR & Prescaler);         /* 执行语句 */
}                                                       /* 代码块结束 */

/**
  * @brief  Get the selected prescaler of the IWDG
  * @rmtoll PR           PR            LL_IWDG_GetPrescaler
  * @param  IWDGx IWDG Instance
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_IWDG_PRESCALER_4
  *         @arg @ref LL_IWDG_PRESCALER_8
  *         @arg @ref LL_IWDG_PRESCALER_16
  *         @arg @ref LL_IWDG_PRESCALER_32
  *         @arg @ref LL_IWDG_PRESCALER_64
  *         @arg @ref LL_IWDG_PRESCALER_128
  *         @arg @ref LL_IWDG_PRESCALER_256
  */
__STATIC_INLINE uint32_t LL_IWDG_GetPrescaler(IWDG_TypeDef *IWDGx)  /* 源代码 */
{                                                       /* 代码块开始 */
  return (READ_REG(IWDGx->PR));                         /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Specify the IWDG down-counter reload value
  * @rmtoll RLR          RL            LL_IWDG_SetReloadCounter
  * @param  IWDGx IWDG Instance
  * @param  Counter Value between Min_Data=0 and Max_Data=0x0FFF
  * @retval None
  */
__STATIC_INLINE void LL_IWDG_SetReloadCounter(IWDG_TypeDef *IWDGx, uint32_t Counter)  /* 源代码 */
{                                                       /* 代码块开始 */
  WRITE_REG(IWDGx->RLR, IWDG_RLR_RL & Counter);         /* 执行语句 */
}                                                       /* 代码块结束 */

/**
  * @brief  Get the specified IWDG down-counter reload value
  * @rmtoll RLR          RL            LL_IWDG_GetReloadCounter
  * @param  IWDGx IWDG Instance
  * @retval Value between Min_Data=0 and Max_Data=0x0FFF
  */
__STATIC_INLINE uint32_t LL_IWDG_GetReloadCounter(IWDG_TypeDef *IWDGx)  /* 源代码 */
{                                                       /* 代码块开始 */
  return (READ_REG(IWDGx->RLR));                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup IWDG_LL_EF_FLAG_Management FLAG_Management
  * @{
  */

/**
  * @brief  Check if flag Prescaler Value Update is set or not
  * @rmtoll SR           PVU           LL_IWDG_IsActiveFlag_PVU
  * @param  IWDGx IWDG Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_IWDG_IsActiveFlag_PVU(IWDG_TypeDef *IWDGx)  /* 源代码 */
{                                                       /* 代码块开始 */
  return ((READ_BIT(IWDGx->SR, IWDG_SR_PVU) == (IWDG_SR_PVU)) ? 1UL : 0UL);  /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Check if flag Reload Value Update is set or not
  * @rmtoll SR           RVU           LL_IWDG_IsActiveFlag_RVU
  * @param  IWDGx IWDG Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_IWDG_IsActiveFlag_RVU(IWDG_TypeDef *IWDGx)  /* 源代码 */
{                                                       /* 代码块开始 */
  return ((READ_BIT(IWDGx->SR, IWDG_SR_RVU) == (IWDG_SR_RVU)) ? 1UL : 0UL);  /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Check if flags Prescaler & Reload Value Update are reset or not
  * @rmtoll SR           PVU           LL_IWDG_IsReady\n
  *         SR           RVU           LL_IWDG_IsReady
  * @param  IWDGx IWDG Instance
  * @retval State of bits (1 or 0).
  */
__STATIC_INLINE uint32_t LL_IWDG_IsReady(IWDG_TypeDef *IWDGx)  /* 源代码 */
{                                                       /* 代码块开始 */
  return ((READ_BIT(IWDGx->SR, IWDG_SR_PVU | IWDG_SR_RVU) == 0U) ? 1UL : 0UL);  /* 返回值 */
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

#endif /* IWDG */

/**
  * @}
  */

#ifdef __cplusplus                                      /* 条件编译 */
}                                                       /* 代码块结束 */
#endif                                                  /* 条件编译结束 */

#endif /* STM32F4xx_LL_IWDG_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
