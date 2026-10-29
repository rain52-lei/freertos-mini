/* 文件: stm32f4xx_hal_lptim.h | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_lptim.h
  * @author  MCD Application Team
  * @brief   Header file of LPTIM HAL module.
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
#ifndef STM32F4xx_HAL_LPTIM_H                           /* 防止头文件重复包含 */
#define STM32F4xx_HAL_LPTIM_H                           /* 定义宏 */

#ifdef __cplusplus                                      /* 条件编译 */
extern "C" {                                            /* 外部符号声明 */
#endif                                                  /* 条件编译结束 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal_def.h"                          /* 包含 STM32/HAL 头文件 */

/** @addtogroup STM32F4xx_HAL_Driver
  * @{
  */

#if defined (LPTIM1)                                    /* 条件编译 */

/** @addtogroup LPTIM
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup LPTIM_Exported_Types LPTIM Exported Types
  * @{
  */
#define LPTIM_EXTI_LINE_WAKEUPTIMER_EVENT  EXTI_IMR_MR23  /*!< External interrupt line 23 Connected to the LPTIM EXTI Line */

/**
  * @brief  LPTIM Clock configuration definition
  */
typedef struct                                          /* 类型定义 */
{                                                       /* 代码块开始 */
  uint32_t Source;         /*!< Selects the clock source.
                           This parameter can be a value of @ref LPTIM_Clock_Source   */

  uint32_t Prescaler;      /*!< Specifies the counter clock Prescaler.
                           This parameter can be a value of @ref LPTIM_Clock_Prescaler */

} LPTIM_ClockConfigTypeDef;                             /* 代码块结束 */

/**
  * @brief  LPTIM Clock configuration definition
  */
typedef struct                                          /* 类型定义 */
{                                                       /* 代码块开始 */
  uint32_t Polarity;      /*!< Selects the polarity of the active edge for the counter unit
                           if the ULPTIM input is selected.
                           Note: This parameter is used only when Ultra low power clock source is used.
                           Note: If the polarity is configured on 'both edges', an auxiliary clock
                           (one of the Low power oscillator) must be active.
                           This parameter can be a value of @ref LPTIM_Clock_Polarity */

  uint32_t SampleTime;     /*!< Selects the clock sampling time to configure the clock glitch filter.
                           Note: This parameter is used only when Ultra low power clock source is used.
                           This parameter can be a value of @ref LPTIM_Clock_Sample_Time */

} LPTIM_ULPClockConfigTypeDef;                          /* 代码块结束 */

/**
  * @brief  LPTIM Trigger configuration definition
  */
typedef struct                                          /* 类型定义 */
{                                                       /* 代码块开始 */
  uint32_t Source;        /*!< Selects the Trigger source.
                          This parameter can be a value of @ref LPTIM_Trigger_Source */

  uint32_t ActiveEdge;    /*!< Selects the Trigger active edge.
                          Note: This parameter is used only when an external trigger is used.
                          This parameter can be a value of @ref LPTIM_External_Trigger_Polarity */

  uint32_t SampleTime;    /*!< Selects the trigger sampling time to configure the clock glitch filter.
                          Note: This parameter is used only when an external trigger is used.
                          This parameter can be a value of @ref LPTIM_Trigger_Sample_Time  */
} LPTIM_TriggerConfigTypeDef;                           /* 代码块结束 */

/**
  * @brief  LPTIM Initialization Structure definition
  */
typedef struct                                          /* 类型定义 */
{                                                       /* 代码块开始 */
  LPTIM_ClockConfigTypeDef     Clock;               /*!< Specifies the clock parameters */

  LPTIM_ULPClockConfigTypeDef  UltraLowPowerClock;  /*!< Specifies the Ultra Low Power clock parameters */

  LPTIM_TriggerConfigTypeDef   Trigger;             /*!< Specifies the Trigger parameters */

  uint32_t                     OutputPolarity;      /*!< Specifies the Output polarity.
                                                    This parameter can be a value of @ref LPTIM_Output_Polarity */

  uint32_t                     UpdateMode;          /*!< Specifies whether the update of the autoreload and the compare
                                                    values is done immediately or after the end of current period.
                                                    This parameter can be a value of @ref LPTIM_Updating_Mode */

  uint32_t                     CounterSource;       /*!< Specifies whether the counter is incremented each internal event
                                                    or each external event.
                                                    This parameter can be a value of @ref LPTIM_Counter_Source */
} LPTIM_InitTypeDef;                                    /* 代码块结束 */

/**
  * @brief  HAL LPTIM State structure definition
  */
typedef enum                                            /* 类型定义 */
{                                                       /* 代码块开始 */
  HAL_LPTIM_STATE_RESET            = 0x00U,    /*!< Peripheral not yet initialized or disabled  */
  HAL_LPTIM_STATE_READY            = 0x01U,    /*!< Peripheral Initialized and ready for use    */
  HAL_LPTIM_STATE_BUSY             = 0x02U,    /*!< An internal process is ongoing              */
  HAL_LPTIM_STATE_TIMEOUT          = 0x03U,    /*!< Timeout state                               */
  HAL_LPTIM_STATE_ERROR            = 0x04U     /*!< Internal Process is ongoing                 */
} HAL_LPTIM_StateTypeDef;                               /* 代码块结束 */

/**
  * @brief  LPTIM handle Structure definition
  */
#if (USE_HAL_LPTIM_REGISTER_CALLBACKS == 1)             /* 条件编译 */
typedef struct __LPTIM_HandleTypeDef                    /* 类型定义 */
#else                                                   /* 条件编译否则 */
typedef struct                                          /* 类型定义 */
#endif /* USE_HAL_LPTIM_REGISTER_CALLBACKS */
{                                                       /* 代码块开始 */
  LPTIM_TypeDef                 *Instance;         /*!< Register base address     */

  LPTIM_InitTypeDef              Init;             /*!< LPTIM required parameters */

  HAL_StatusTypeDef              Status;           /*!< LPTIM peripheral status   */

  HAL_LockTypeDef                Lock;             /*!< LPTIM locking object      */

  __IO  HAL_LPTIM_StateTypeDef   State;            /*!< LPTIM peripheral state    */

#if (USE_HAL_LPTIM_REGISTER_CALLBACKS == 1)             /* 条件编译 */
  void (* MspInitCallback)(struct __LPTIM_HandleTypeDef *hlptim);            /*!< LPTIM Base Msp Init Callback                 */
  void (* MspDeInitCallback)(struct __LPTIM_HandleTypeDef *hlptim);          /*!< LPTIM Base Msp DeInit Callback               */
  void (* CompareMatchCallback)(struct __LPTIM_HandleTypeDef *hlptim);       /*!< Compare match Callback                       */
  void (* AutoReloadMatchCallback)(struct __LPTIM_HandleTypeDef *hlptim);    /*!< Auto-reload match Callback                   */
  void (* TriggerCallback)(struct __LPTIM_HandleTypeDef *hlptim);            /*!< External trigger event detection Callback    */
  void (* CompareWriteCallback)(struct __LPTIM_HandleTypeDef *hlptim);       /*!< Compare register write complete Callback     */
  void (* AutoReloadWriteCallback)(struct __LPTIM_HandleTypeDef *hlptim);    /*!< Auto-reload register write complete Callback */
  void (* DirectionUpCallback)(struct __LPTIM_HandleTypeDef *hlptim);        /*!< Up-counting direction change Callback        */
  void (* DirectionDownCallback)(struct __LPTIM_HandleTypeDef *hlptim);      /*!< Down-counting direction change Callback      */
#endif /* USE_HAL_LPTIM_REGISTER_CALLBACKS */
} LPTIM_HandleTypeDef;                                  /* 代码块结束 */

#if (USE_HAL_LPTIM_REGISTER_CALLBACKS == 1)             /* 条件编译 */
/**
  * @brief  HAL LPTIM Callback ID enumeration definition
  */
typedef enum                                            /* 类型定义 */
{                                                       /* 代码块开始 */
  HAL_LPTIM_MSPINIT_CB_ID          = 0x00U,    /*!< LPTIM Base Msp Init Callback ID                  */
  HAL_LPTIM_MSPDEINIT_CB_ID        = 0x01U,    /*!< LPTIM Base Msp DeInit Callback ID                */
  HAL_LPTIM_COMPARE_MATCH_CB_ID    = 0x02U,    /*!< Compare match Callback ID                        */
  HAL_LPTIM_AUTORELOAD_MATCH_CB_ID = 0x03U,    /*!< Auto-reload match Callback ID                    */
  HAL_LPTIM_TRIGGER_CB_ID          = 0x04U,    /*!< External trigger event detection Callback ID     */
  HAL_LPTIM_COMPARE_WRITE_CB_ID    = 0x05U,    /*!< Compare register write complete Callback ID      */
  HAL_LPTIM_AUTORELOAD_WRITE_CB_ID = 0x06U,    /*!< Auto-reload register write complete Callback ID  */
  HAL_LPTIM_DIRECTION_UP_CB_ID     = 0x07U,    /*!< Up-counting direction change Callback ID         */
  HAL_LPTIM_DIRECTION_DOWN_CB_ID   = 0x08U,    /*!< Down-counting direction change Callback ID       */
} HAL_LPTIM_CallbackIDTypeDef;                          /* 代码块结束 */

/**
  * @brief  HAL TIM Callback pointer definition
  */
typedef  void (*pLPTIM_CallbackTypeDef)(LPTIM_HandleTypeDef *hlptim);  /*!< pointer to the LPTIM callback function */

#endif /* USE_HAL_LPTIM_REGISTER_CALLBACKS */
/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup LPTIM_Exported_Constants LPTIM Exported Constants
  * @{
  */

/** @defgroup LPTIM_Clock_Source LPTIM Clock Source
  * @{
  */
#define LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC        0x00000000U  /* 定义宏 */
#define LPTIM_CLOCKSOURCE_ULPTIM                LPTIM_CFGR_CKSEL  /* 定义宏 */
/**
  * @}
  */

/** @defgroup LPTIM_Clock_Prescaler LPTIM Clock Prescaler
  * @{
  */
#define LPTIM_PRESCALER_DIV1                    0x00000000U  /* 定义宏 */
#define LPTIM_PRESCALER_DIV2                    LPTIM_CFGR_PRESC_0  /* 定义宏 */
#define LPTIM_PRESCALER_DIV4                    LPTIM_CFGR_PRESC_1  /* 定义宏 */
#define LPTIM_PRESCALER_DIV8                    (LPTIM_CFGR_PRESC_0 | LPTIM_CFGR_PRESC_1)  /* 定义宏 */
#define LPTIM_PRESCALER_DIV16                   LPTIM_CFGR_PRESC_2  /* 定义宏 */
#define LPTIM_PRESCALER_DIV32                   (LPTIM_CFGR_PRESC_0 | LPTIM_CFGR_PRESC_2)  /* 定义宏 */
#define LPTIM_PRESCALER_DIV64                   (LPTIM_CFGR_PRESC_1 | LPTIM_CFGR_PRESC_2)  /* 定义宏 */
#define LPTIM_PRESCALER_DIV128                  LPTIM_CFGR_PRESC  /* 定义宏 */
/**
  * @}
  */

/** @defgroup LPTIM_Output_Polarity LPTIM Output Polarity
  * @{
  */

#define LPTIM_OUTPUTPOLARITY_HIGH               0x00000000U  /* 定义宏 */
#define LPTIM_OUTPUTPOLARITY_LOW                LPTIM_CFGR_WAVPOL  /* 定义宏 */
/**
  * @}
  */

/** @defgroup LPTIM_Clock_Sample_Time LPTIM Clock Sample Time
  * @{
  */
#define LPTIM_CLOCKSAMPLETIME_DIRECTTRANSITION  0x00000000U  /* 定义宏 */
#define LPTIM_CLOCKSAMPLETIME_2TRANSITIONS      LPTIM_CFGR_CKFLT_0  /* 定义宏 */
#define LPTIM_CLOCKSAMPLETIME_4TRANSITIONS      LPTIM_CFGR_CKFLT_1  /* 定义宏 */
#define LPTIM_CLOCKSAMPLETIME_8TRANSITIONS      LPTIM_CFGR_CKFLT  /* 定义宏 */
/**
  * @}
  */

/** @defgroup LPTIM_Clock_Polarity LPTIM Clock Polarity
  * @{
  */
#define LPTIM_CLOCKPOLARITY_RISING              0x00000000U  /* 定义宏 */
#define LPTIM_CLOCKPOLARITY_FALLING             LPTIM_CFGR_CKPOL_0  /* 定义宏 */
#define LPTIM_CLOCKPOLARITY_RISING_FALLING      LPTIM_CFGR_CKPOL_1  /* 定义宏 */
/**
  * @}
  */

/** @defgroup LPTIM_Trigger_Source LPTIM Trigger Source
  * @{
  */
#define LPTIM_TRIGSOURCE_SOFTWARE               0x0000FFFFU  /* 定义宏 */
#define LPTIM_TRIGSOURCE_0                      0x00000000U  /* 定义宏 */
#define LPTIM_TRIGSOURCE_1                      LPTIM_CFGR_TRIGSEL_0  /* 定义宏 */
#define LPTIM_TRIGSOURCE_2                      LPTIM_CFGR_TRIGSEL_1  /* 定义宏 */
#define LPTIM_TRIGSOURCE_3                      (LPTIM_CFGR_TRIGSEL_0 | LPTIM_CFGR_TRIGSEL_1)  /* 定义宏 */
#define LPTIM_TRIGSOURCE_4                      LPTIM_CFGR_TRIGSEL_2  /* 定义宏 */
#define LPTIM_TRIGSOURCE_5                      (LPTIM_CFGR_TRIGSEL_0 | LPTIM_CFGR_TRIGSEL_2)  /* 定义宏 */
/**
  * @}
  */

/** @defgroup LPTIM_External_Trigger_Polarity LPTIM External Trigger Polarity
  * @{
  */
#define LPTIM_ACTIVEEDGE_RISING                LPTIM_CFGR_TRIGEN_0  /* 定义宏 */
#define LPTIM_ACTIVEEDGE_FALLING               LPTIM_CFGR_TRIGEN_1  /* 定义宏 */
#define LPTIM_ACTIVEEDGE_RISING_FALLING        LPTIM_CFGR_TRIGEN  /* 定义宏 */
/**
  * @}
  */

/** @defgroup LPTIM_Trigger_Sample_Time LPTIM Trigger Sample Time
  * @{
  */
#define LPTIM_TRIGSAMPLETIME_DIRECTTRANSITION  0x00000000U  /* 定义宏 */
#define LPTIM_TRIGSAMPLETIME_2TRANSITIONS      LPTIM_CFGR_TRGFLT_0  /* 定义宏 */
#define LPTIM_TRIGSAMPLETIME_4TRANSITIONS      LPTIM_CFGR_TRGFLT_1  /* 定义宏 */
#define LPTIM_TRIGSAMPLETIME_8TRANSITIONS      LPTIM_CFGR_TRGFLT  /* 定义宏 */
/**
  * @}
  */

/** @defgroup LPTIM_Updating_Mode LPTIM Updating Mode
  * @{
  */

#define LPTIM_UPDATE_IMMEDIATE                  0x00000000U  /* 定义宏 */
#define LPTIM_UPDATE_ENDOFPERIOD                LPTIM_CFGR_PRELOAD  /* 定义宏 */
/**
  * @}
  */

/** @defgroup LPTIM_Counter_Source LPTIM Counter Source
  * @{
  */

#define LPTIM_COUNTERSOURCE_INTERNAL            0x00000000U  /* 定义宏 */
#define LPTIM_COUNTERSOURCE_EXTERNAL            LPTIM_CFGR_COUNTMODE  /* 定义宏 */
/**
  * @}
  */

/** @defgroup LPTIM_Flag_Definition LPTIM Flags Definition
  * @{
  */

#define LPTIM_FLAG_DOWN                          LPTIM_ISR_DOWN  /* 定义宏 */
#define LPTIM_FLAG_UP                            LPTIM_ISR_UP  /* 定义宏 */
#define LPTIM_FLAG_ARROK                         LPTIM_ISR_ARROK  /* 定义宏 */
#define LPTIM_FLAG_CMPOK                         LPTIM_ISR_CMPOK  /* 定义宏 */
#define LPTIM_FLAG_EXTTRIG                       LPTIM_ISR_EXTTRIG  /* 定义宏 */
#define LPTIM_FLAG_ARRM                          LPTIM_ISR_ARRM  /* 定义宏 */
#define LPTIM_FLAG_CMPM                          LPTIM_ISR_CMPM  /* 定义宏 */
/**
  * @}
  */

/** @defgroup LPTIM_Interrupts_Definition LPTIM Interrupts Definition
  * @{
  */
#define LPTIM_IT_DOWN                            LPTIM_IER_DOWNIE  /* 定义宏 */
#define LPTIM_IT_UP                              LPTIM_IER_UPIE  /* 定义宏 */
#define LPTIM_IT_ARROK                           LPTIM_IER_ARROKIE  /* 定义宏 */
#define LPTIM_IT_CMPOK                           LPTIM_IER_CMPOKIE  /* 定义宏 */
#define LPTIM_IT_EXTTRIG                         LPTIM_IER_EXTTRIGIE  /* 定义宏 */
#define LPTIM_IT_ARRM                            LPTIM_IER_ARRMIE  /* 定义宏 */
#define LPTIM_IT_CMPM                            LPTIM_IER_CMPMIE  /* 定义宏 */
/**
  * @}
  */

/** @defgroup LPTIM_Option Register Definition
  * @{
  */
#define LPTIM_OP_PAD_AF                          0x00000000U  /* 定义宏 */
#define LPTIM_OP_PAD_PA4                         LPTIM_OR_LPT_IN1_RMP_0  /* 定义宏 */
#define LPTIM_OP_PAD_PB9                         LPTIM_OR_LPT_IN1_RMP_1  /* 定义宏 */
#define LPTIM_OP_TIM_DAC                         LPTIM_OR_LPT_IN1_RMP  /* 定义宏 */
/**
  * @}
  */

/**
  * @}
  */

/* Exported macros -----------------------------------------------------------*/
/** @defgroup LPTIM_Exported_Macros LPTIM Exported Macros
  * @{
  */

/** @brief Reset LPTIM handle state.
  * @param  __HANDLE__ LPTIM handle
  * @retval None
  */
#if (USE_HAL_LPTIM_REGISTER_CALLBACKS == 1)             /* 条件编译 */
#define __HAL_LPTIM_RESET_HANDLE_STATE(__HANDLE__) do {                                                        \
                                                        (__HANDLE__)->State             = HAL_LPTIM_STATE_RESET; \
                                                        (__HANDLE__)->MspInitCallback   = NULL;                  \
                                                        (__HANDLE__)->MspDeInitCallback = NULL;                  \
                                                      } while(0)  /* 代码块结束 */
#else                                                   /* 条件编译否则 */
#define __HAL_LPTIM_RESET_HANDLE_STATE(__HANDLE__) ((__HANDLE__)->State = HAL_LPTIM_STATE_RESET)  /* 定义包含保护宏 */
#endif /* USE_HAL_LPTIM_REGISTER_CALLBACKS */

/**
  * @brief  Enable the LPTIM peripheral.
  * @param  __HANDLE__ LPTIM handle
  * @retval None
  */
#define __HAL_LPTIM_ENABLE(__HANDLE__)   ((__HANDLE__)->Instance->CR |= (LPTIM_CR_ENABLE))  /* 定义包含保护宏 */

/**
  * @brief  Disable the LPTIM peripheral.
  * @param  __HANDLE__ LPTIM handle
  * @note   The following sequence is required to solve LPTIM disable HW limitation.
  *         Please check Errata Sheet ES0335 for more details under "MCU may remain
  *         stuck in LPTIM interrupt when entering Stop mode" section.
  * @note   Please call @ref HAL_LPTIM_GetState() after a call to __HAL_LPTIM_DISABLE to
  *         check for TIMEOUT.
  * @retval None
  */
#define __HAL_LPTIM_DISABLE(__HANDLE__)   LPTIM_Disable(__HANDLE__)  /* 定义包含保护宏 */

/**
  * @brief  Start the LPTIM peripheral in Continuous mode.
  * @param  __HANDLE__ LPTIM handle
  * @retval None
  */
#define __HAL_LPTIM_START_CONTINUOUS(__HANDLE__)  ((__HANDLE__)->Instance->CR |=  LPTIM_CR_CNTSTRT)  /* 定义包含保护宏 */
/**
  * @brief  Start the LPTIM peripheral in single mode.
  * @param  __HANDLE__ LPTIM handle
  * @retval None
  */
#define __HAL_LPTIM_START_SINGLE(__HANDLE__)      ((__HANDLE__)->Instance->CR |=  LPTIM_CR_SNGSTRT)  /* 定义包含保护宏 */

/**
  * @brief  Write the passed parameter in the Autoreload register.
  * @param  __HANDLE__ LPTIM handle
  * @param  __VALUE__ Autoreload value
  * @retval None
  * @note   The ARR register can only be modified when the LPTIM instance is enabled.
  */
#define __HAL_LPTIM_AUTORELOAD_SET(__HANDLE__ , __VALUE__)  ((__HANDLE__)->Instance->ARR =  (__VALUE__))  /* 定义包含保护宏 */

/**
  * @brief  Write the passed parameter in the Compare register.
  * @param  __HANDLE__ LPTIM handle
  * @param  __VALUE__ Compare value
  * @retval None
  * @note   The CMP register can only be modified when the LPTIM instance is enabled.
  */
#define __HAL_LPTIM_COMPARE_SET(__HANDLE__ , __VALUE__)     ((__HANDLE__)->Instance->CMP =  (__VALUE__))  /* 定义包含保护宏 */

/**
  * @brief  Check whether the specified LPTIM flag is set or not.
  * @param  __HANDLE__ LPTIM handle
  * @param  __FLAG__ LPTIM flag to check
  *            This parameter can be a value of:
  *            @arg LPTIM_FLAG_DOWN    : Counter direction change up Flag.
  *            @arg LPTIM_FLAG_UP      : Counter direction change down to up Flag.
  *            @arg LPTIM_FLAG_ARROK   : Autoreload register update OK Flag.
  *            @arg LPTIM_FLAG_CMPOK   : Compare register update OK Flag.
  *            @arg LPTIM_FLAG_EXTTRIG : External trigger edge event Flag.
  *            @arg LPTIM_FLAG_ARRM    : Autoreload match Flag.
  *            @arg LPTIM_FLAG_CMPM    : Compare match Flag.
  * @retval The state of the specified flag (SET or RESET).
  */
#define __HAL_LPTIM_GET_FLAG(__HANDLE__, __FLAG__)          (((__HANDLE__)->Instance->ISR &(__FLAG__)) == (__FLAG__))  /* 定义包含保护宏 */

/**
  * @brief  Clear the specified LPTIM flag.
  * @param  __HANDLE__ LPTIM handle.
  * @param  __FLAG__ LPTIM flag to clear.
  *            This parameter can be a value of:
  *            @arg LPTIM_FLAG_DOWN    : Counter direction change up Flag.
  *            @arg LPTIM_FLAG_UP      : Counter direction change down to up Flag.
  *            @arg LPTIM_FLAG_ARROK   : Autoreload register update OK Flag.
  *            @arg LPTIM_FLAG_CMPOK   : Compare register update OK Flag.
  *            @arg LPTIM_FLAG_EXTTRIG : External trigger edge event Flag.
  *            @arg LPTIM_FLAG_ARRM    : Autoreload match Flag.
  *            @arg LPTIM_FLAG_CMPM    : Compare match Flag.
  * @retval None.
  */
#define __HAL_LPTIM_CLEAR_FLAG(__HANDLE__, __FLAG__)        ((__HANDLE__)->Instance->ICR  = (__FLAG__))  /* 定义包含保护宏 */

/**
  * @brief  Enable the specified LPTIM interrupt.
  * @param  __HANDLE__ LPTIM handle.
  * @param  __INTERRUPT__ LPTIM interrupt to set.
  *            This parameter can be a value of:
  *            @arg LPTIM_IT_DOWN    : Counter direction change up Interrupt.
  *            @arg LPTIM_IT_UP      : Counter direction change down to up Interrupt.
  *            @arg LPTIM_IT_ARROK   : Autoreload register update OK Interrupt.
  *            @arg LPTIM_IT_CMPOK   : Compare register update OK Interrupt.
  *            @arg LPTIM_IT_EXTTRIG : External trigger edge event Interrupt.
  *            @arg LPTIM_IT_ARRM    : Autoreload match Interrupt.
  *            @arg LPTIM_IT_CMPM    : Compare match Interrupt.
  * @retval None.
  * @note   The LPTIM interrupts can only be enabled when the LPTIM instance is disabled.
  */
#define __HAL_LPTIM_ENABLE_IT(__HANDLE__, __INTERRUPT__)    ((__HANDLE__)->Instance->IER  |= (__INTERRUPT__))  /* 定义包含保护宏 */

/**
  * @brief  Disable the specified LPTIM interrupt.
  * @param  __HANDLE__ LPTIM handle.
  * @param  __INTERRUPT__ LPTIM interrupt to set.
  *            This parameter can be a value of:
  *            @arg LPTIM_IT_DOWN    : Counter direction change up Interrupt.
  *            @arg LPTIM_IT_UP      : Counter direction change down to up Interrupt.
  *            @arg LPTIM_IT_ARROK   : Autoreload register update OK Interrupt.
  *            @arg LPTIM_IT_CMPOK   : Compare register update OK Interrupt.
  *            @arg LPTIM_IT_EXTTRIG : External trigger edge event Interrupt.
  *            @arg LPTIM_IT_ARRM    : Autoreload match Interrupt.
  *            @arg LPTIM_IT_CMPM    : Compare match Interrupt.
  * @retval None.
  * @note   The LPTIM interrupts can only be disabled when the LPTIM instance is disabled.
  */
#define __HAL_LPTIM_DISABLE_IT(__HANDLE__, __INTERRUPT__)   ((__HANDLE__)->Instance->IER  &= (~(__INTERRUPT__)))  /* 定义包含保护宏 */

/**
  * @brief  Check whether the specified LPTIM interrupt source is enabled or not.
  * @param  __HANDLE__ LPTIM handle.
  * @param  __INTERRUPT__ LPTIM interrupt to check.
  *            This parameter can be a value of:
  *            @arg LPTIM_IT_DOWN    : Counter direction change up Interrupt.
  *            @arg LPTIM_IT_UP      : Counter direction change down to up Interrupt.
  *            @arg LPTIM_IT_ARROK   : Autoreload register update OK Interrupt.
  *            @arg LPTIM_IT_CMPOK   : Compare register update OK Interrupt.
  *            @arg LPTIM_IT_EXTTRIG : External trigger edge event Interrupt.
  *            @arg LPTIM_IT_ARRM    : Autoreload match Interrupt.
  *            @arg LPTIM_IT_CMPM    : Compare match Interrupt.
  * @retval Interrupt status.
  */

#define __HAL_LPTIM_GET_IT_SOURCE(__HANDLE__, __INTERRUPT__) ((((__HANDLE__)->Instance->IER\
                                                                & (__INTERRUPT__)) == (__INTERRUPT__)) ? SET : RESET)  /* 源代码 */

/** @brief  LPTIM Option Register
  * @param  __HANDLE__ LPTIM handle
  * @param   __VALUE__ This parameter can be a value of :
  *            @arg  LPTIM_OP_PAD_AF
  *            @arg  LPTIM_OP_PAD_PA4
  *            @arg  LPTIM_OP_PAD_PB9
  *            @arg  LPTIM_OP_TIM_DAC
  * @retval None
  */
#define __HAL_LPTIM_OPTR_CONFIG(__HANDLE__ , __VALUE__)     ((__HANDLE__)->Instance->OR  =  (__VALUE__))  /* 定义包含保护宏 */


/**
  * @brief  Enable interrupt on the LPTIM Wake-up Timer associated Exti line.
  * @retval None
  */
#define __HAL_LPTIM_WAKEUPTIMER_EXTI_ENABLE_IT()       (EXTI->IMR\
                                                        |= LPTIM_EXTI_LINE_WAKEUPTIMER_EVENT)  /* 源代码 */

/**
  * @brief  Disable interrupt on the LPTIM Wake-up Timer associated Exti line.
  * @retval None
  */
#define __HAL_LPTIM_WAKEUPTIMER_EXTI_DISABLE_IT()      (EXTI->IMR\
                                                        &= ~(LPTIM_EXTI_LINE_WAKEUPTIMER_EVENT))  /* 源代码 */

/**
  * @brief  Enable event on the LPTIM Wake-up Timer associated Exti line.
  * @retval None.
  */
#define __HAL_LPTIM_WAKEUPTIMER_EXTI_ENABLE_EVENT()    (EXTI->EMR\
                                                        |= LPTIM_EXTI_LINE_WAKEUPTIMER_EVENT)  /* 源代码 */

/**
  * @brief  Disable event on the LPTIM Wake-up Timer associated Exti line.
  * @retval None.
  */
#define __HAL_LPTIM_WAKEUPTIMER_EXTI_DISABLE_EVENT()   (EXTI->EMR\
                                                        &= ~(LPTIM_EXTI_LINE_WAKEUPTIMER_EVENT))  /* 源代码 */
#if defined(EXTI_IMR_MR23)                              /* 条件编译 */

/**
  * @brief  Enable falling edge trigger on the LPTIM Wake-up Timer associated Exti line.
  * @retval None.
  */
#define __HAL_LPTIM_WAKEUPTIMER_EXTI_ENABLE_FALLING_EDGE()   (EXTI->FTSR\
                                                              |= LPTIM_EXTI_LINE_WAKEUPTIMER_EVENT)  /* 源代码 */

/**
  * @brief  Disable falling edge trigger on the LPTIM Wake-up Timer associated Exti line.
  * @retval None.
  */
#define __HAL_LPTIM_WAKEUPTIMER_EXTI_DISABLE_FALLING_EDGE()  (EXTI->FTSR\
                                                              &= ~(LPTIM_EXTI_LINE_WAKEUPTIMER_EVENT))  /* 源代码 */

/**
  * @brief  Enable rising edge trigger on the LPTIM Wake-up Timer associated Exti line.
  * @retval None.
  */
#define __HAL_LPTIM_WAKEUPTIMER_EXTI_ENABLE_RISING_EDGE()    (EXTI->RTSR\
                                                              |= LPTIM_EXTI_LINE_WAKEUPTIMER_EVENT)  /* 源代码 */

/**
  * @brief  Disable rising edge trigger on the LPTIM Wake-up Timer associated Exti line.
  * @retval None.
  */
#define __HAL_LPTIM_WAKEUPTIMER_EXTI_DISABLE_RISING_EDGE()   (EXTI->RTSR\
                                                              &= ~(LPTIM_EXTI_LINE_WAKEUPTIMER_EVENT))  /* 源代码 */

/**
  * @brief  Enable rising & falling edge trigger on the LPTIM Wake-up Timer associated Exti line.
  * @retval None.
  */
#define __HAL_LPTIM_WAKEUPTIMER_EXTI_ENABLE_RISING_FALLING_EDGE() do{__HAL_LPTIM_WAKEUPTIMER_EXTI_ENABLE_RISING_EDGE();\
                                                                     __HAL_LPTIM_WAKEUPTIMER_EXTI_ENABLE_FALLING_EDGE();\
                                                                    }while(0)  /* 代码块结束 */

/**
  * @brief  Disable rising & falling edge trigger on the LPTIM Wake-up Timer associated Exti line.
  * @retval None.
  */
#define __HAL_LPTIM_WAKEUPTIMER_EXTI_DISABLE_RISING_FALLING_EDGE() do{__HAL_LPTIM_WAKEUPTIMER_EXTI_DISABLE_RISING_EDGE();\
                                                                       __HAL_LPTIM_WAKEUPTIMER_EXTI_DISABLE_FALLING_EDGE();\
                                                                     }while(0)  /* 代码块结束 */

/**
  * @brief Check whether the LPTIM Wake-up Timer associated Exti line interrupt flag is set or not.
  * @retval Line Status.
  */
#define __HAL_LPTIM_WAKEUPTIMER_EXTI_GET_FLAG()              (EXTI->PR\
                                                              & LPTIM_EXTI_LINE_WAKEUPTIMER_EVENT)  /* 源代码 */

/**
  * @brief Clear the LPTIM Wake-up Timer associated Exti line flag.
  * @retval None.
  */
#define __HAL_LPTIM_WAKEUPTIMER_EXTI_CLEAR_FLAG()            (EXTI->PR\
                                                              = LPTIM_EXTI_LINE_WAKEUPTIMER_EVENT)  /* 源代码 */

/**
  * @brief Generate a Software interrupt on the LPTIM Wake-up Timer associated Exti line.
  * @retval None.
  */
#define __HAL_LPTIM_WAKEUPTIMER_EXTI_GENERATE_SWIT()         (EXTI->SWIER\
                                                              |= LPTIM_EXTI_LINE_WAKEUPTIMER_EVENT)  /* 源代码 */
#endif /* EXTI_IMR_MR23 */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup LPTIM_Exported_Functions LPTIM Exported Functions
  * @{
  */

/** @addtogroup LPTIM_Exported_Functions_Group1
  *  @brief    Initialization and Configuration functions.
  * @{
  */
/* Initialization/de-initialization functions  ********************************/
HAL_StatusTypeDef HAL_LPTIM_Init(LPTIM_HandleTypeDef *hlptim);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_LPTIM_DeInit(LPTIM_HandleTypeDef *hlptim);  /* HAL状态类型函数 */

/* MSP functions  *************************************************************/
void HAL_LPTIM_MspInit(LPTIM_HandleTypeDef *hlptim);    /* 函数定义或声明 */
void HAL_LPTIM_MspDeInit(LPTIM_HandleTypeDef *hlptim);  /* 函数定义或声明 */
/**
  * @}
  */

/** @addtogroup LPTIM_Exported_Functions_Group2
  *  @brief   Start-Stop operation functions.
  * @{
  */
/* Start/Stop operation functions  *********************************************/
/* ################################# PWM Mode ################################*/
/* Blocking mode: Polling */
HAL_StatusTypeDef HAL_LPTIM_PWM_Start(LPTIM_HandleTypeDef *hlptim, uint32_t Period, uint32_t Pulse);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_LPTIM_PWM_Stop(LPTIM_HandleTypeDef *hlptim);  /* HAL状态类型函数 */
/* Non-Blocking mode: Interrupt */
HAL_StatusTypeDef HAL_LPTIM_PWM_Start_IT(LPTIM_HandleTypeDef *hlptim, uint32_t Period, uint32_t Pulse);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_LPTIM_PWM_Stop_IT(LPTIM_HandleTypeDef *hlptim);  /* HAL状态类型函数 */

/* ############################# One Pulse Mode ##############################*/
/* Blocking mode: Polling */
HAL_StatusTypeDef HAL_LPTIM_OnePulse_Start(LPTIM_HandleTypeDef *hlptim, uint32_t Period, uint32_t Pulse);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_LPTIM_OnePulse_Stop(LPTIM_HandleTypeDef *hlptim);  /* HAL状态类型函数 */
/* Non-Blocking mode: Interrupt */
HAL_StatusTypeDef HAL_LPTIM_OnePulse_Start_IT(LPTIM_HandleTypeDef *hlptim, uint32_t Period, uint32_t Pulse);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_LPTIM_OnePulse_Stop_IT(LPTIM_HandleTypeDef *hlptim);  /* HAL状态类型函数 */

/* ############################## Set once Mode ##############################*/
/* Blocking mode: Polling */
HAL_StatusTypeDef HAL_LPTIM_SetOnce_Start(LPTIM_HandleTypeDef *hlptim, uint32_t Period, uint32_t Pulse);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_LPTIM_SetOnce_Stop(LPTIM_HandleTypeDef *hlptim);  /* HAL状态类型函数 */
/* Non-Blocking mode: Interrupt */
HAL_StatusTypeDef HAL_LPTIM_SetOnce_Start_IT(LPTIM_HandleTypeDef *hlptim, uint32_t Period, uint32_t Pulse);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_LPTIM_SetOnce_Stop_IT(LPTIM_HandleTypeDef *hlptim);  /* HAL状态类型函数 */

/* ############################### Encoder Mode ##############################*/
/* Blocking mode: Polling */
HAL_StatusTypeDef HAL_LPTIM_Encoder_Start(LPTIM_HandleTypeDef *hlptim, uint32_t Period);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_LPTIM_Encoder_Stop(LPTIM_HandleTypeDef *hlptim);  /* HAL状态类型函数 */
/* Non-Blocking mode: Interrupt */
HAL_StatusTypeDef HAL_LPTIM_Encoder_Start_IT(LPTIM_HandleTypeDef *hlptim, uint32_t Period);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_LPTIM_Encoder_Stop_IT(LPTIM_HandleTypeDef *hlptim);  /* HAL状态类型函数 */

/* ############################# Time out  Mode ##############################*/
/* Blocking mode: Polling */
HAL_StatusTypeDef HAL_LPTIM_TimeOut_Start(LPTIM_HandleTypeDef *hlptim, uint32_t Period, uint32_t Timeout);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_LPTIM_TimeOut_Stop(LPTIM_HandleTypeDef *hlptim);  /* HAL状态类型函数 */
/* Non-Blocking mode: Interrupt */
HAL_StatusTypeDef HAL_LPTIM_TimeOut_Start_IT(LPTIM_HandleTypeDef *hlptim, uint32_t Period, uint32_t Timeout);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_LPTIM_TimeOut_Stop_IT(LPTIM_HandleTypeDef *hlptim);  /* HAL状态类型函数 */

/* ############################## Counter Mode ###############################*/
/* Blocking mode: Polling */
HAL_StatusTypeDef HAL_LPTIM_Counter_Start(LPTIM_HandleTypeDef *hlptim, uint32_t Period);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_LPTIM_Counter_Stop(LPTIM_HandleTypeDef *hlptim);  /* HAL状态类型函数 */
/* Non-Blocking mode: Interrupt */
HAL_StatusTypeDef HAL_LPTIM_Counter_Start_IT(LPTIM_HandleTypeDef *hlptim, uint32_t Period);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_LPTIM_Counter_Stop_IT(LPTIM_HandleTypeDef *hlptim);  /* HAL状态类型函数 */
/**
  * @}
  */

/** @addtogroup LPTIM_Exported_Functions_Group3
  *  @brief  Read operation functions.
  * @{
  */
/* Reading operation functions ************************************************/
uint32_t HAL_LPTIM_ReadCounter(LPTIM_HandleTypeDef *hlptim);  /* 32位无符号变量/函数 */
uint32_t HAL_LPTIM_ReadAutoReload(LPTIM_HandleTypeDef *hlptim);  /* 32位无符号变量/函数 */
uint32_t HAL_LPTIM_ReadCompare(LPTIM_HandleTypeDef *hlptim);  /* 32位无符号变量/函数 */
/**
  * @}
  */

/** @addtogroup LPTIM_Exported_Functions_Group4
  *  @brief  LPTIM IRQ handler and callback functions.
  * @{
  */
/* LPTIM IRQ functions  *******************************************************/
void HAL_LPTIM_IRQHandler(LPTIM_HandleTypeDef *hlptim);  /* 中断服务函数 */

/* CallBack functions  ********************************************************/
void HAL_LPTIM_CompareMatchCallback(LPTIM_HandleTypeDef *hlptim);  /* 函数定义或声明 */
void HAL_LPTIM_AutoReloadMatchCallback(LPTIM_HandleTypeDef *hlptim);  /* 函数定义或声明 */
void HAL_LPTIM_TriggerCallback(LPTIM_HandleTypeDef *hlptim);  /* 函数定义或声明 */
void HAL_LPTIM_CompareWriteCallback(LPTIM_HandleTypeDef *hlptim);  /* 函数定义或声明 */
void HAL_LPTIM_AutoReloadWriteCallback(LPTIM_HandleTypeDef *hlptim);  /* 函数定义或声明 */
void HAL_LPTIM_DirectionUpCallback(LPTIM_HandleTypeDef *hlptim);  /* 函数定义或声明 */
void HAL_LPTIM_DirectionDownCallback(LPTIM_HandleTypeDef *hlptim);  /* 函数定义或声明 */

/* Callbacks Register/UnRegister functions  ***********************************/
#if (USE_HAL_LPTIM_REGISTER_CALLBACKS == 1)             /* 条件编译 */
HAL_StatusTypeDef HAL_LPTIM_RegisterCallback(LPTIM_HandleTypeDef *lphtim, HAL_LPTIM_CallbackIDTypeDef CallbackID,  /* HAL状态类型函数 */
                                             pLPTIM_CallbackTypeDef pCallback);  /* 执行语句 */
HAL_StatusTypeDef HAL_LPTIM_UnRegisterCallback(LPTIM_HandleTypeDef *lphtim, HAL_LPTIM_CallbackIDTypeDef CallbackID);  /* HAL状态类型函数 */
#endif /* USE_HAL_LPTIM_REGISTER_CALLBACKS */
/**
  * @}
  */

/** @addtogroup LPTIM_Group5
  *  @brief   Peripheral State functions.
  * @{
  */
/* Peripheral State functions  ************************************************/
HAL_LPTIM_StateTypeDef HAL_LPTIM_GetState(LPTIM_HandleTypeDef *hlptim);  /* HAL库函数调用 */
/**
  * @}
  */

/**
  * @}
  */

/* Private types -------------------------------------------------------------*/
/** @defgroup LPTIM_Private_Types LPTIM Private Types
  * @{
  */

/**
  * @}
  */

/* Private variables ---------------------------------------------------------*/
/** @defgroup LPTIM_Private_Variables LPTIM Private Variables
  * @{
  */

/**
  * @}
  */

/* Private constants ---------------------------------------------------------*/
/** @defgroup LPTIM_Private_Constants LPTIM Private Constants
  * @{
  */

/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/** @defgroup LPTIM_Private_Macros LPTIM Private Macros
  * @{
  */

#define IS_LPTIM_CLOCK_SOURCE(__SOURCE__)       (((__SOURCE__) == LPTIM_CLOCKSOURCE_ULPTIM) || \
                                                 ((__SOURCE__) == LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC))  /* 源代码 */


#define IS_LPTIM_CLOCK_PRESCALER(__PRESCALER__) (((__PRESCALER__) ==  LPTIM_PRESCALER_DIV1  ) || \
                                                 ((__PRESCALER__) ==  LPTIM_PRESCALER_DIV2  ) || \
                                                 ((__PRESCALER__) ==  LPTIM_PRESCALER_DIV4  ) || \
                                                 ((__PRESCALER__) ==  LPTIM_PRESCALER_DIV8  ) || \
                                                 ((__PRESCALER__) ==  LPTIM_PRESCALER_DIV16 ) || \
                                                 ((__PRESCALER__) ==  LPTIM_PRESCALER_DIV32 ) || \
                                                 ((__PRESCALER__) ==  LPTIM_PRESCALER_DIV64 ) || \
                                                 ((__PRESCALER__) ==  LPTIM_PRESCALER_DIV128))  /* 源代码 */

#define IS_LPTIM_CLOCK_PRESCALERDIV1(__PRESCALER__) ((__PRESCALER__) ==  LPTIM_PRESCALER_DIV1)  /* 定义宏 */

#define IS_LPTIM_OUTPUT_POLARITY(__POLARITY__)  (((__POLARITY__) == LPTIM_OUTPUTPOLARITY_LOW ) || \
                                                 ((__POLARITY__) == LPTIM_OUTPUTPOLARITY_HIGH))  /* 源代码 */

#define IS_LPTIM_CLOCK_SAMPLE_TIME(__SAMPLETIME__) (((__SAMPLETIME__) == LPTIM_CLOCKSAMPLETIME_DIRECTTRANSITION) || \
                                                    ((__SAMPLETIME__) == LPTIM_CLOCKSAMPLETIME_2TRANSITIONS)     || \
                                                    ((__SAMPLETIME__) == LPTIM_CLOCKSAMPLETIME_4TRANSITIONS)     || \
                                                    ((__SAMPLETIME__) == LPTIM_CLOCKSAMPLETIME_8TRANSITIONS))  /* 源代码 */

#define IS_LPTIM_CLOCK_POLARITY(__POLARITY__)   (((__POLARITY__) == LPTIM_CLOCKPOLARITY_RISING)  || \
                                                 ((__POLARITY__) == LPTIM_CLOCKPOLARITY_FALLING) || \
                                                 ((__POLARITY__) == LPTIM_CLOCKPOLARITY_RISING_FALLING))  /* 源代码 */

#define IS_LPTIM_TRG_SOURCE(__TRIG__)           (((__TRIG__) == LPTIM_TRIGSOURCE_SOFTWARE) || \
                                                 ((__TRIG__) == LPTIM_TRIGSOURCE_0) || \
                                                 ((__TRIG__) == LPTIM_TRIGSOURCE_1) || \
                                                 ((__TRIG__) == LPTIM_TRIGSOURCE_2) || \
                                                 ((__TRIG__) == LPTIM_TRIGSOURCE_3) || \
                                                 ((__TRIG__) == LPTIM_TRIGSOURCE_4) || \
                                                 ((__TRIG__) == LPTIM_TRIGSOURCE_5))  /* 源代码 */

#define IS_LPTIM_EXT_TRG_POLARITY(__POLARITY__) (((__POLARITY__) == LPTIM_ACTIVEEDGE_RISING         ) || \
                                                 ((__POLARITY__) == LPTIM_ACTIVEEDGE_FALLING        ) || \
                                                 ((__POLARITY__) == LPTIM_ACTIVEEDGE_RISING_FALLING ))  /* 源代码 */

#define IS_LPTIM_TRIG_SAMPLE_TIME(__SAMPLETIME__) (((__SAMPLETIME__) == LPTIM_TRIGSAMPLETIME_DIRECTTRANSITION) || \
                                                   ((__SAMPLETIME__) == LPTIM_TRIGSAMPLETIME_2TRANSITIONS    ) || \
                                                   ((__SAMPLETIME__) == LPTIM_TRIGSAMPLETIME_4TRANSITIONS    ) || \
                                                   ((__SAMPLETIME__) == LPTIM_TRIGSAMPLETIME_8TRANSITIONS    ))  /* 源代码 */

#define IS_LPTIM_UPDATE_MODE(__MODE__)          (((__MODE__) == LPTIM_UPDATE_IMMEDIATE) || \
                                                 ((__MODE__) == LPTIM_UPDATE_ENDOFPERIOD))  /* 源代码 */

#define IS_LPTIM_COUNTER_SOURCE(__SOURCE__)     (((__SOURCE__) == LPTIM_COUNTERSOURCE_INTERNAL) || \
                                                 ((__SOURCE__) == LPTIM_COUNTERSOURCE_EXTERNAL))  /* 源代码 */

#define IS_LPTIM_AUTORELOAD(__AUTORELOAD__)     ((__AUTORELOAD__) <= 0x0000FFFFUL)  /* 定义宏 */

#define IS_LPTIM_COMPARE(__COMPARE__)           ((__COMPARE__) <= 0x0000FFFFUL)  /* 定义宏 */

#define IS_LPTIM_PERIOD(__PERIOD__)             ((__PERIOD__) <= 0x0000FFFFUL)  /* 定义宏 */

#define IS_LPTIM_PULSE(__PULSE__)               ((__PULSE__) <= 0x0000FFFFUL)  /* 定义宏 */

/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/
/** @defgroup LPTIM_Private_Functions LPTIM Private Functions
  * @{
  */
void LPTIM_Disable(LPTIM_HandleTypeDef *hlptim);        /* 函数定义或声明 */
/**
  * @}
  */

/**
  * @}
  */

#endif /* LPTIM1 */
/**
  * @}
  */

#ifdef __cplusplus                                      /* 条件编译 */
}                                                       /* 代码块结束 */
#endif                                                  /* 条件编译结束 */

#endif /* STM32F4xx_HAL_LPTIM_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
