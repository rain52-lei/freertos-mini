/* 文件: stm32f4xx_hal_pwr.h | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_pwr.h
  * @author  MCD Application Team
  * @brief   Header file of PWR HAL module.
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
#ifndef __STM32F4xx_HAL_PWR_H                           /* 防止头文件重复包含 */
#define __STM32F4xx_HAL_PWR_H                           /* 定义包含保护宏 */

#ifdef __cplusplus                                      /* 条件编译 */
 extern "C" {                                           /* 外部符号声明 */
#endif                                                  /* 条件编译结束 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal_def.h"                          /* 包含 STM32/HAL 头文件 */

/** @addtogroup STM32F4xx_HAL_Driver
  * @{
  */

/** @addtogroup PWR
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/

/** @defgroup PWR_Exported_Types PWR Exported Types
  * @{
  */

/**
  * @brief  PWR PVD configuration structure definition
  */
typedef struct                                          /* 类型定义 */
{                                                       /* 代码块开始 */
  uint32_t PVDLevel;   /*!< PVDLevel: Specifies the PVD detection level.
                            This parameter can be a value of @ref PWR_PVD_detection_level */

  uint32_t Mode;      /*!< Mode: Specifies the operating mode for the selected pins.
                           This parameter can be a value of @ref PWR_PVD_Mode */
}PWR_PVDTypeDef;                                        /* 代码块结束 */

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup PWR_Exported_Constants PWR Exported Constants
  * @{
  */

/** @defgroup PWR_WakeUp_Pins PWR WakeUp Pins
  * @{
  */
#define PWR_WAKEUP_PIN1                 0x00000100U     /* 定义宏 */
/**
  * @}
  */

/** @defgroup PWR_PVD_detection_level PWR PVD detection level
  * @{
  */ 
#define PWR_PVDLEVEL_0                  PWR_CR_PLS_LEV0  /* 定义宏 */
#define PWR_PVDLEVEL_1                  PWR_CR_PLS_LEV1  /* 定义宏 */
#define PWR_PVDLEVEL_2                  PWR_CR_PLS_LEV2  /* 定义宏 */
#define PWR_PVDLEVEL_3                  PWR_CR_PLS_LEV3  /* 定义宏 */
#define PWR_PVDLEVEL_4                  PWR_CR_PLS_LEV4  /* 定义宏 */
#define PWR_PVDLEVEL_5                  PWR_CR_PLS_LEV5  /* 定义宏 */
#define PWR_PVDLEVEL_6                  PWR_CR_PLS_LEV6  /* 定义宏 */
#define PWR_PVDLEVEL_7                  PWR_CR_PLS_LEV7/* External input analog voltage 
                                                          (Compare internally to VREFINT) */
/**
  * @}
  */   

/** @defgroup PWR_PVD_Mode PWR PVD Mode
  * @{
  */
#define PWR_PVD_MODE_NORMAL                 0x00000000U   /*!< basic mode is used */
#define PWR_PVD_MODE_IT_RISING              0x00010001U   /*!< External Interrupt Mode with Rising edge trigger detection */
#define PWR_PVD_MODE_IT_FALLING             0x00010002U   /*!< External Interrupt Mode with Falling edge trigger detection */
#define PWR_PVD_MODE_IT_RISING_FALLING      0x00010003U   /*!< External Interrupt Mode with Rising/Falling edge trigger detection */
#define PWR_PVD_MODE_EVENT_RISING           0x00020001U   /*!< Event Mode with Rising edge trigger detection */
#define PWR_PVD_MODE_EVENT_FALLING          0x00020002U   /*!< Event Mode with Falling edge trigger detection */
#define PWR_PVD_MODE_EVENT_RISING_FALLING   0x00020003U   /*!< Event Mode with Rising/Falling edge trigger detection */
/**
  * @}
  */


/** @defgroup PWR_Regulator_state_in_STOP_mode PWR Regulator state in SLEEP/STOP mode
  * @{
  */
#define PWR_MAINREGULATOR_ON                        0x00000000U  /* 定义宏 */
#define PWR_LOWPOWERREGULATOR_ON                    PWR_CR_LPDS  /* 定义宏 */
/**
  * @}
  */

/** @defgroup PWR_SLEEP_mode_entry PWR SLEEP mode entry
  * @{
  */
#define PWR_SLEEPENTRY_WFI              ((uint8_t)0x01)  /* 定义宏 */
#define PWR_SLEEPENTRY_WFE              ((uint8_t)0x02)  /* 定义宏 */
/**
  * @}
  */

/** @defgroup PWR_STOP_mode_entry PWR STOP mode entry
  * @{
  */
#define PWR_STOPENTRY_WFI               ((uint8_t)0x01)  /* 定义宏 */
#define PWR_STOPENTRY_WFE               ((uint8_t)0x02)  /* 定义宏 */
/**
  * @}
  */

/** @defgroup PWR_Flag PWR Flag
  * @{
  */
#define PWR_FLAG_WU                     PWR_CSR_WUF     /* 定义宏 */
#define PWR_FLAG_SB                     PWR_CSR_SBF     /* 定义宏 */
#define PWR_FLAG_PVDO                   PWR_CSR_PVDO    /* 定义宏 */
#define PWR_FLAG_BRR                    PWR_CSR_BRR     /* 定义宏 */
#define PWR_FLAG_VOSRDY                 PWR_CSR_VOSRDY  /* 定义宏 */
/**
  * @}
  */

/**
  * @}
  */ 

/* Exported macro ------------------------------------------------------------*/
/** @defgroup PWR_Exported_Macro PWR Exported Macro
  * @{
  */

/** @brief  Check PWR flag is set or not.
  * @param  __FLAG__ specifies the flag to check.
  *           This parameter can be one of the following values:
  *            @arg PWR_FLAG_WU: Wake Up flag. This flag indicates that a wakeup event 
  *                  was received from the WKUP pin or from the RTC alarm (Alarm A 
  *                  or Alarm B), RTC Tamper event, RTC TimeStamp event or RTC Wakeup.
  *                  An additional wakeup event is detected if the WKUP pin is enabled 
  *                  (by setting the EWUP bit) when the WKUP pin level is already high.  
  *            @arg PWR_FLAG_SB: StandBy flag. This flag indicates that the system was
  *                  resumed from StandBy mode.    
  *            @arg PWR_FLAG_PVDO: PVD Output. This flag is valid only if PVD is enabled 
  *                  by the HAL_PWR_EnablePVD() function. The PVD is stopped by Standby mode 
  *                  For this reason, this bit is equal to 0 after Standby or reset
  *                  until the PVDE bit is set.
  *            @arg PWR_FLAG_BRR: Backup regulator ready flag. This bit is not reset 
  *                  when the device wakes up from Standby mode or by a system reset 
  *                  or power reset.  
  *            @arg PWR_FLAG_VOSRDY: This flag indicates that the Regulator voltage 
  *                 scaling output selection is ready.
  * @retval The new state of __FLAG__ (TRUE or FALSE).
  */
#define __HAL_PWR_GET_FLAG(__FLAG__) ((PWR->CSR & (__FLAG__)) == (__FLAG__))  /* 定义包含保护宏 */

/** @brief  Clear the PWR's pending flags.
  * @param  __FLAG__ specifies the flag to clear.
  *          This parameter can be one of the following values:
  *            @arg PWR_FLAG_WU: Wake Up flag
  *            @arg PWR_FLAG_SB: StandBy flag
  */
#define __HAL_PWR_CLEAR_FLAG(__FLAG__) (PWR->CR |=  (__FLAG__) << 2U)  /* 定义包含保护宏 */

/**
  * @brief Enable the PVD Exti Line 16.
  * @retval None.
  */
#define __HAL_PWR_PVD_EXTI_ENABLE_IT()   (EXTI->IMR |= (PWR_EXTI_LINE_PVD))  /* 定义包含保护宏 */

/**
  * @brief Disable the PVD EXTI Line 16.
  * @retval None.
  */
#define __HAL_PWR_PVD_EXTI_DISABLE_IT()  (EXTI->IMR &= ~(PWR_EXTI_LINE_PVD))  /* 定义包含保护宏 */

/**
  * @brief Enable event on PVD Exti Line 16.
  * @retval None.
  */
#define __HAL_PWR_PVD_EXTI_ENABLE_EVENT()   (EXTI->EMR |= (PWR_EXTI_LINE_PVD))  /* 定义包含保护宏 */

/**
  * @brief Disable event on PVD Exti Line 16.
  * @retval None.
  */
#define __HAL_PWR_PVD_EXTI_DISABLE_EVENT()  (EXTI->EMR &= ~(PWR_EXTI_LINE_PVD))  /* 定义包含保护宏 */

/**
  * @brief Enable the PVD Extended Interrupt Rising Trigger.
  * @retval None.
  */
#define __HAL_PWR_PVD_EXTI_ENABLE_RISING_EDGE()   SET_BIT(EXTI->RTSR, PWR_EXTI_LINE_PVD)  /* 定义包含保护宏 */

/**
  * @brief Disable the PVD Extended Interrupt Rising Trigger.
  * @retval None.
  */
#define __HAL_PWR_PVD_EXTI_DISABLE_RISING_EDGE()  CLEAR_BIT(EXTI->RTSR, PWR_EXTI_LINE_PVD)  /* 定义包含保护宏 */

/**
  * @brief Enable the PVD Extended Interrupt Falling Trigger.
  * @retval None.
  */
#define __HAL_PWR_PVD_EXTI_ENABLE_FALLING_EDGE()   SET_BIT(EXTI->FTSR, PWR_EXTI_LINE_PVD)  /* 定义包含保护宏 */


/**
  * @brief Disable the PVD Extended Interrupt Falling Trigger.
  * @retval None.
  */
#define __HAL_PWR_PVD_EXTI_DISABLE_FALLING_EDGE()  CLEAR_BIT(EXTI->FTSR, PWR_EXTI_LINE_PVD)  /* 定义包含保护宏 */


/**
  * @brief  PVD EXTI line configuration: set rising & falling edge trigger.
  * @retval None.
  */
#define __HAL_PWR_PVD_EXTI_ENABLE_RISING_FALLING_EDGE()   do{__HAL_PWR_PVD_EXTI_ENABLE_RISING_EDGE();\
                                                             __HAL_PWR_PVD_EXTI_ENABLE_FALLING_EDGE();\
                                                            }while(0U)  /* 代码块结束 */

/**
  * @brief Disable the PVD Extended Interrupt Rising & Falling Trigger.
  * This parameter can be:
  * @retval None.
  */
#define __HAL_PWR_PVD_EXTI_DISABLE_RISING_FALLING_EDGE()  do{__HAL_PWR_PVD_EXTI_DISABLE_RISING_EDGE();\
                                                             __HAL_PWR_PVD_EXTI_DISABLE_FALLING_EDGE();\
                                                            }while(0U)   /* 代码块结束 */

/**
  * @brief checks whether the specified PVD Exti interrupt flag is set or not.
  * @retval EXTI PVD Line Status.
  */
#define __HAL_PWR_PVD_EXTI_GET_FLAG()  (EXTI->PR & (PWR_EXTI_LINE_PVD))  /* 定义包含保护宏 */

/**
  * @brief Clear the PVD Exti flag.
  * @retval None.
  */
#define __HAL_PWR_PVD_EXTI_CLEAR_FLAG()  (EXTI->PR = (PWR_EXTI_LINE_PVD))  /* 定义包含保护宏 */

/**
  * @brief  Generates a Software interrupt on PVD EXTI line.
  * @retval None
  */
#define __HAL_PWR_PVD_EXTI_GENERATE_SWIT() (EXTI->SWIER |= (PWR_EXTI_LINE_PVD))  /* 定义包含保护宏 */

/**
  * @}
  */

/* Include PWR HAL Extension module */
#include "stm32f4xx_hal_pwr_ex.h"                       /* 包含 STM32/HAL 头文件 */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup PWR_Exported_Functions PWR Exported Functions
  * @{
  */

/** @addtogroup PWR_Exported_Functions_Group1 Initialization and de-initialization functions 
  * @{
  */
/* Initialization and de-initialization functions *****************************/
void HAL_PWR_DeInit(void);                              /* 函数定义或声明 */
void HAL_PWR_EnableBkUpAccess(void);                    /* 函数定义或声明 */
void HAL_PWR_DisableBkUpAccess(void);                   /* 函数定义或声明 */
/**
  * @}
  */

/** @addtogroup PWR_Exported_Functions_Group2 Peripheral Control functions 
  * @{
  */
/* Peripheral Control functions  **********************************************/
/* PVD configuration */
void HAL_PWR_ConfigPVD(PWR_PVDTypeDef *sConfigPVD);     /* 函数定义或声明 */
void HAL_PWR_EnablePVD(void);                           /* 函数定义或声明 */
void HAL_PWR_DisablePVD(void);                          /* 函数定义或声明 */

/* WakeUp pins configuration */
void HAL_PWR_EnableWakeUpPin(uint32_t WakeUpPinx);      /* 函数定义或声明 */
void HAL_PWR_DisableWakeUpPin(uint32_t WakeUpPinx);     /* 函数定义或声明 */

/* Low Power modes entry */
void HAL_PWR_EnterSTOPMode(uint32_t Regulator, uint8_t STOPEntry);  /* 函数定义或声明 */
void HAL_PWR_EnterSLEEPMode(uint32_t Regulator, uint8_t SLEEPEntry);  /* 函数定义或声明 */
void HAL_PWR_EnterSTANDBYMode(void);                    /* 函数定义或声明 */

/* Power PVD IRQ Handler */
void HAL_PWR_PVD_IRQHandler(void);                      /* 中断服务函数 */
void HAL_PWR_PVDCallback(void);                         /* 函数定义或声明 */

/* Cortex System Control functions  *******************************************/
void HAL_PWR_EnableSleepOnExit(void);                   /* 函数定义或声明 */
void HAL_PWR_DisableSleepOnExit(void);                  /* 函数定义或声明 */
void HAL_PWR_EnableSEVOnPend(void);                     /* 函数定义或声明 */
void HAL_PWR_DisableSEVOnPend(void);                    /* 函数定义或声明 */
/**
  * @}
  */

/**
  * @}
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/** @defgroup PWR_Private_Constants PWR Private Constants
  * @{
  */

/** @defgroup PWR_PVD_EXTI_Line PWR PVD EXTI Line
  * @{
  */
#define PWR_EXTI_LINE_PVD  ((uint32_t)EXTI_IMR_MR16)  /*!< External interrupt line 16 Connected to the PVD EXTI Line */
/**
  * @}
  */

/** @defgroup PWR_register_alias_address PWR Register alias address
  * @{
  */
/* ------------- PWR registers bit address in the alias region ---------------*/
#define PWR_OFFSET               (PWR_BASE - PERIPH_BASE)  /* 定义宏 */
#define PWR_CR_OFFSET            0x00U                  /* 定义宏 */
#define PWR_CSR_OFFSET           0x04U                  /* 定义宏 */
#define PWR_CR_OFFSET_BB         (PWR_OFFSET + PWR_CR_OFFSET)  /* 定义宏 */
#define PWR_CSR_OFFSET_BB        (PWR_OFFSET + PWR_CSR_OFFSET)  /* 定义宏 */
/**
  * @}
  */

/** @defgroup PWR_CR_register_alias PWR CR Register alias address
  * @{
  */
/* --- CR Register ---*/
/* Alias word address of DBP bit */
#define DBP_BIT_NUMBER   PWR_CR_DBP_Pos                 /* 定义宏 */
#define CR_DBP_BB        (uint32_t)(PERIPH_BB_BASE + (PWR_CR_OFFSET_BB * 32U) + (DBP_BIT_NUMBER * 4U))  /* 定义宏 */

/* Alias word address of PVDE bit */
#define PVDE_BIT_NUMBER  PWR_CR_PVDE_Pos                /* 定义宏 */
#define CR_PVDE_BB       (uint32_t)(PERIPH_BB_BASE + (PWR_CR_OFFSET_BB * 32U) + (PVDE_BIT_NUMBER * 4U))  /* 定义宏 */

/* Alias word address of VOS bit */
#define VOS_BIT_NUMBER  PWR_CR_VOS_Pos                  /* 定义宏 */
#define CR_VOS_BB      (uint32_t)(PERIPH_BB_BASE + (PWR_CR_OFFSET_BB * 32U) + (VOS_BIT_NUMBER * 4U))  /* 定义宏 */
/**
  * @}
  */

/** @defgroup PWR_CSR_register_alias PWR CSR Register alias address
  * @{
  */
/* --- CSR Register ---*/
/* Alias word address of EWUP bit */
#define EWUP_BIT_NUMBER  PWR_CSR_EWUP_Pos               /* 定义宏 */
#define CSR_EWUP_BB      (PERIPH_BB_BASE + (PWR_CSR_OFFSET_BB * 32U) + (EWUP_BIT_NUMBER * 4U))  /* 定义宏 */
/**
  * @}
  */

/**
  * @}
  */
/* Private macros ------------------------------------------------------------*/
/** @defgroup PWR_Private_Macros PWR Private Macros
  * @{
  */

/** @defgroup PWR_IS_PWR_Definitions PWR Private macros to check input parameters
  * @{
  */
#define IS_PWR_PVD_LEVEL(LEVEL) (((LEVEL) == PWR_PVDLEVEL_0) || ((LEVEL) == PWR_PVDLEVEL_1)|| \
                                 ((LEVEL) == PWR_PVDLEVEL_2) || ((LEVEL) == PWR_PVDLEVEL_3)|| \
                                 ((LEVEL) == PWR_PVDLEVEL_4) || ((LEVEL) == PWR_PVDLEVEL_5)|| \
                                 ((LEVEL) == PWR_PVDLEVEL_6) || ((LEVEL) == PWR_PVDLEVEL_7))  /* 源代码 */
#define IS_PWR_PVD_MODE(MODE) (((MODE) == PWR_PVD_MODE_IT_RISING)|| ((MODE) == PWR_PVD_MODE_IT_FALLING) || \
                              ((MODE) == PWR_PVD_MODE_IT_RISING_FALLING) || ((MODE) == PWR_PVD_MODE_EVENT_RISING) || \
                              ((MODE) == PWR_PVD_MODE_EVENT_FALLING) || ((MODE) == PWR_PVD_MODE_EVENT_RISING_FALLING) || \
                              ((MODE) == PWR_PVD_MODE_NORMAL))  /* 源代码 */
#define IS_PWR_REGULATOR(REGULATOR) (((REGULATOR) == PWR_MAINREGULATOR_ON) || \
                                     ((REGULATOR) == PWR_LOWPOWERREGULATOR_ON))  /* 源代码 */
#define IS_PWR_SLEEP_ENTRY(ENTRY) (((ENTRY) == PWR_SLEEPENTRY_WFI) || ((ENTRY) == PWR_SLEEPENTRY_WFE))  /* 定义宏 */
#define IS_PWR_STOP_ENTRY(ENTRY) (((ENTRY) == PWR_STOPENTRY_WFI) || ((ENTRY) == PWR_STOPENTRY_WFE))  /* 定义宏 */
/**
  * @}
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

#ifdef __cplusplus                                      /* 条件编译 */
}                                                       /* 代码块结束 */
#endif                                                  /* 条件编译结束 */


#endif /* __STM32F4xx_HAL_PWR_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
