/* 文件: stm32f4xx_ll_wwdg.h | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_ll_wwdg.h
  * @author  MCD Application Team
  * @brief   Header file of WWDG LL module.
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
#ifndef STM32F4xx_LL_WWDG_H                             /* 防止头文件重复包含 */
#define STM32F4xx_LL_WWDG_H                             /* 定义宏 */

#ifdef __cplusplus                                      /* 条件编译 */
extern "C" {                                            /* 外部符号声明 */
#endif                                                  /* 条件编译结束 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"                                  /* 包含 STM32/HAL 头文件 */

/** @addtogroup STM32F4xx_LL_Driver
  * @{
  */

#if defined (WWDG)                                      /* 条件编译 */

/** @defgroup WWDG_LL WWDG
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/** @defgroup WWDG_LL_Exported_Constants WWDG Exported Constants
  * @{
  */

/** @defgroup WWDG_LL_EC_IT IT Defines
  * @brief    IT defines which can be used with LL_WWDG_ReadReg and  LL_WWDG_WriteReg functions
  * @{
  */
#define LL_WWDG_CFR_EWI                     WWDG_CFR_EWI  /* 定义宏 */
/**
  * @}
  */

/** @defgroup WWDG_LL_EC_PRESCALER  PRESCALER
  * @{
  */
#define LL_WWDG_PRESCALER_1                 0x00000000u                                               /*!< WWDG counter clock = (PCLK1/4096)/1 */
#define LL_WWDG_PRESCALER_2                 WWDG_CFR_WDGTB_0                                          /*!< WWDG counter clock = (PCLK1/4096)/2 */
#define LL_WWDG_PRESCALER_4                 WWDG_CFR_WDGTB_1                                          /*!< WWDG counter clock = (PCLK1/4096)/4 */
#define LL_WWDG_PRESCALER_8                 (WWDG_CFR_WDGTB_0 | WWDG_CFR_WDGTB_1)                     /*!< WWDG counter clock = (PCLK1/4096)/8 */
/**
  * @}
  */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/** @defgroup WWDG_LL_Exported_Macros WWDG Exported Macros
  * @{
  */
/** @defgroup WWDG_LL_EM_WRITE_READ Common Write and read registers macros
  * @{
  */
/**
  * @brief  Write a value in WWDG register
  * @param  __INSTANCE__ WWDG Instance
  * @param  __REG__ Register to be written
  * @param  __VALUE__ Value to be written in the register
  * @retval None
  */
#define LL_WWDG_WriteReg(__INSTANCE__, __REG__, __VALUE__) WRITE_REG(__INSTANCE__->__REG__, (__VALUE__))  /* 定义宏 */

/**
  * @brief  Read a value in WWDG register
  * @param  __INSTANCE__ WWDG Instance
  * @param  __REG__ Register to be read
  * @retval Register value
  */
#define LL_WWDG_ReadReg(__INSTANCE__, __REG__) READ_REG(__INSTANCE__->__REG__)  /* 定义宏 */
/**
  * @}
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup WWDG_LL_Exported_Functions WWDG Exported Functions
  * @{
  */

/** @defgroup WWDG_LL_EF_Configuration Configuration
  * @{
  */
/**
  * @brief  Enable Window Watchdog. The watchdog is always disabled after a reset.
  * @note   It is enabled by setting the WDGA bit in the WWDG_CR register,
  *         then it cannot be disabled again except by a reset.
  *         This bit is set by software and only cleared by hardware after a reset.
  *         When WDGA = 1, the watchdog can generate a reset.
  * @rmtoll CR           WDGA          LL_WWDG_Enable
  * @param  WWDGx WWDG Instance
  * @retval None
  */
__STATIC_INLINE void LL_WWDG_Enable(WWDG_TypeDef *WWDGx)  /* 源代码 */
{                                                       /* 代码块开始 */
  SET_BIT(WWDGx->CR, WWDG_CR_WDGA);                     /* 执行语句 */
}                                                       /* 代码块结束 */

/**
  * @brief  Checks if Window Watchdog is enabled
  * @rmtoll CR           WDGA          LL_WWDG_IsEnabled
  * @param  WWDGx WWDG Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_WWDG_IsEnabled(WWDG_TypeDef *WWDGx)  /* 源代码 */
{                                                       /* 代码块开始 */
  return ((READ_BIT(WWDGx->CR, WWDG_CR_WDGA) == (WWDG_CR_WDGA)) ? 1UL : 0UL);  /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Set the Watchdog counter value to provided value (7-bits T[6:0])
  * @note   When writing to the WWDG_CR register, always write 1 in the MSB b6 to avoid generating an immediate reset
  *         This counter is decremented every (4096 x 2expWDGTB) PCLK cycles
  *         A reset is produced when it rolls over from 0x40 to 0x3F (bit T6 becomes cleared)
  *         Setting the counter lower then 0x40 causes an immediate reset (if WWDG enabled)
  * @rmtoll CR           T             LL_WWDG_SetCounter
  * @param  WWDGx WWDG Instance
  * @param  Counter 0..0x7F (7 bit counter value)
  * @retval None
  */
__STATIC_INLINE void LL_WWDG_SetCounter(WWDG_TypeDef *WWDGx, uint32_t Counter)  /* 源代码 */
{                                                       /* 代码块开始 */
  MODIFY_REG(WWDGx->CR, WWDG_CR_T, Counter);            /* 执行语句 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return current Watchdog Counter Value (7 bits counter value)
  * @rmtoll CR           T             LL_WWDG_GetCounter
  * @param  WWDGx WWDG Instance
  * @retval 7 bit Watchdog Counter value
  */
__STATIC_INLINE uint32_t LL_WWDG_GetCounter(WWDG_TypeDef *WWDGx)  /* 源代码 */
{                                                       /* 代码块开始 */
  return (READ_BIT(WWDGx->CR, WWDG_CR_T));              /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Set the time base of the prescaler (WDGTB).
  * @note   Prescaler is used to apply ratio on PCLK clock, so that Watchdog counter
  *         is decremented every (4096 x 2expWDGTB) PCLK cycles
  * @rmtoll CFR          WDGTB         LL_WWDG_SetPrescaler
  * @param  WWDGx WWDG Instance
  * @param  Prescaler This parameter can be one of the following values:
  *         @arg @ref LL_WWDG_PRESCALER_1
  *         @arg @ref LL_WWDG_PRESCALER_2
  *         @arg @ref LL_WWDG_PRESCALER_4
  *         @arg @ref LL_WWDG_PRESCALER_8
  * @retval None
  */
__STATIC_INLINE void LL_WWDG_SetPrescaler(WWDG_TypeDef *WWDGx, uint32_t Prescaler)  /* 源代码 */
{                                                       /* 代码块开始 */
  MODIFY_REG(WWDGx->CFR, WWDG_CFR_WDGTB, Prescaler);    /* 执行语句 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return current Watchdog Prescaler Value
  * @rmtoll CFR          WDGTB         LL_WWDG_GetPrescaler
  * @param  WWDGx WWDG Instance
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_WWDG_PRESCALER_1
  *         @arg @ref LL_WWDG_PRESCALER_2
  *         @arg @ref LL_WWDG_PRESCALER_4
  *         @arg @ref LL_WWDG_PRESCALER_8
  */
__STATIC_INLINE uint32_t LL_WWDG_GetPrescaler(WWDG_TypeDef *WWDGx)  /* 源代码 */
{                                                       /* 代码块开始 */
  return (READ_BIT(WWDGx->CFR, WWDG_CFR_WDGTB));        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Set the Watchdog Window value to be compared to the downcounter (7-bits W[6:0]).
  * @note   This window value defines when write in the WWDG_CR register
  *         to program Watchdog counter is allowed.
  *         Watchdog counter value update must occur only when the counter value
  *         is lower than the Watchdog window register value.
  *         Otherwise, a MCU reset is generated if the 7-bit Watchdog counter value
  *         (in the control register) is refreshed before the downcounter has reached
  *         the watchdog window register value.
  *         Physically is possible to set the Window lower then 0x40 but it is not recommended.
  *         To generate an immediate reset, it is possible to set the Counter lower than 0x40.
  * @rmtoll CFR          W             LL_WWDG_SetWindow
  * @param  WWDGx WWDG Instance
  * @param  Window 0x00..0x7F (7 bit Window value)
  * @retval None
  */
__STATIC_INLINE void LL_WWDG_SetWindow(WWDG_TypeDef *WWDGx, uint32_t Window)  /* 源代码 */
{                                                       /* 代码块开始 */
  MODIFY_REG(WWDGx->CFR, WWDG_CFR_W, Window);           /* 执行语句 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return current Watchdog Window Value (7 bits value)
  * @rmtoll CFR          W             LL_WWDG_GetWindow
  * @param  WWDGx WWDG Instance
  * @retval 7 bit Watchdog Window value
  */
__STATIC_INLINE uint32_t LL_WWDG_GetWindow(WWDG_TypeDef *WWDGx)  /* 源代码 */
{                                                       /* 代码块开始 */
  return (READ_BIT(WWDGx->CFR, WWDG_CFR_W));            /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup WWDG_LL_EF_FLAG_Management FLAG_Management
  * @{
  */
/**
  * @brief  Indicates if the WWDG Early Wakeup Interrupt Flag is set or not.
  * @note   This bit is set by hardware when the counter has reached the value 0x40.
  *         It must be cleared by software by writing 0.
  *         A write of 1 has no effect. This bit is also set if the interrupt is not enabled.
  * @rmtoll SR           EWIF          LL_WWDG_IsActiveFlag_EWKUP
  * @param  WWDGx WWDG Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_WWDG_IsActiveFlag_EWKUP(WWDG_TypeDef *WWDGx)  /* 源代码 */
{                                                       /* 代码块开始 */
  return ((READ_BIT(WWDGx->SR, WWDG_SR_EWIF) == (WWDG_SR_EWIF)) ? 1UL : 0UL);  /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Clear WWDG Early Wakeup Interrupt Flag (EWIF)
  * @rmtoll SR           EWIF          LL_WWDG_ClearFlag_EWKUP
  * @param  WWDGx WWDG Instance
  * @retval None
  */
__STATIC_INLINE void LL_WWDG_ClearFlag_EWKUP(WWDG_TypeDef *WWDGx)  /* 源代码 */
{                                                       /* 代码块开始 */
  WRITE_REG(WWDGx->SR, ~WWDG_SR_EWIF);                  /* 执行语句 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup WWDG_LL_EF_IT_Management IT_Management
  * @{
  */
/**
  * @brief  Enable the Early Wakeup Interrupt.
  * @note   When set, an interrupt occurs whenever the counter reaches value 0x40.
  *         This interrupt is only cleared by hardware after a reset
  * @rmtoll CFR          EWI           LL_WWDG_EnableIT_EWKUP
  * @param  WWDGx WWDG Instance
  * @retval None
  */
__STATIC_INLINE void LL_WWDG_EnableIT_EWKUP(WWDG_TypeDef *WWDGx)  /* 源代码 */
{                                                       /* 代码块开始 */
  SET_BIT(WWDGx->CFR, WWDG_CFR_EWI);                    /* 执行语句 */
}                                                       /* 代码块结束 */

/**
  * @brief  Check if Early Wakeup Interrupt is enabled
  * @rmtoll CFR          EWI           LL_WWDG_IsEnabledIT_EWKUP
  * @param  WWDGx WWDG Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_WWDG_IsEnabledIT_EWKUP(WWDG_TypeDef *WWDGx)  /* 源代码 */
{                                                       /* 代码块开始 */
  return ((READ_BIT(WWDGx->CFR, WWDG_CFR_EWI) == (WWDG_CFR_EWI)) ? 1UL : 0UL);  /* 返回值 */
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

#endif /* WWDG */

/**
  * @}
  */

#ifdef __cplusplus                                      /* 条件编译 */
}                                                       /* 代码块结束 */
#endif                                                  /* 条件编译结束 */

#endif /* STM32F4xx_LL_WWDG_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
