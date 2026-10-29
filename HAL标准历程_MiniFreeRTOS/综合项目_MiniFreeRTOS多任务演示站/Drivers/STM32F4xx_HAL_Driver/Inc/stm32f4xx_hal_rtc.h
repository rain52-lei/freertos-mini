/* 文件: stm32f4xx_hal_rtc.h | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_rtc.h
  * @author  MCD Application Team
  * @brief   Header file of RTC HAL module.
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
#ifndef __STM32F4xx_HAL_RTC_H                           /* 防止头文件重复包含 */
#define __STM32F4xx_HAL_RTC_H                           /* 定义包含保护宏 */

#ifdef __cplusplus                                      /* 条件编译 */
 extern "C" {                                           /* 外部符号声明 */
#endif                                                  /* 条件编译结束 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal_def.h"                          /* 包含 STM32/HAL 头文件 */

/** @addtogroup STM32F4xx_HAL_Driver
  * @{
  */

/** @addtogroup RTC
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup RTC_Exported_Types RTC Exported Types
  * @{
  */

/**
  * @brief  HAL State structures definition
  */
typedef enum                                            /* 类型定义 */
{                                                       /* 代码块开始 */
  HAL_RTC_STATE_RESET             = 0x00U,  /*!< RTC not yet initialized or disabled */
  HAL_RTC_STATE_READY             = 0x01U,  /*!< RTC initialized and ready for use   */
  HAL_RTC_STATE_BUSY              = 0x02U,  /*!< RTC process is ongoing              */
  HAL_RTC_STATE_TIMEOUT           = 0x03U,  /*!< RTC timeout state                   */
  HAL_RTC_STATE_ERROR             = 0x04U   /*!< RTC error state                     */
}HAL_RTCStateTypeDef;                                   /* 代码块结束 */

/**
  * @brief  RTC Configuration Structure definition
  */
typedef struct                                          /* 类型定义 */
{                                                       /* 代码块开始 */
  uint32_t HourFormat;      /*!< Specifies the RTC Hour Format.
                                 This parameter can be a value of @ref RTC_Hour_Formats */

  uint32_t AsynchPrediv;    /*!< Specifies the RTC Asynchronous Predivider value.
                                 This parameter must be a number between Min_Data = 0x00 and Max_Data = 0x7F */

  uint32_t SynchPrediv;     /*!< Specifies the RTC Synchronous Predivider value.
                                 This parameter must be a number between Min_Data = 0x00 and Max_Data = 0x7FFFU */

  uint32_t OutPut;          /*!< Specifies which signal will be routed to the RTC output.
                                 This parameter can be a value of @ref RTC_Output_selection_Definitions */

  uint32_t OutPutPolarity;  /*!< Specifies the polarity of the output signal.
                                 This parameter can be a value of @ref RTC_Output_Polarity_Definitions */

  uint32_t OutPutType;      /*!< Specifies the RTC Output Pin mode.
                                 This parameter can be a value of @ref RTC_Output_Type_ALARM_OUT */
}RTC_InitTypeDef;                                       /* 代码块结束 */

/**
  * @brief  RTC Time structure definition
  */
typedef struct                                          /* 类型定义 */
{                                                       /* 代码块开始 */
  uint8_t Hours;            /*!< Specifies the RTC Time Hour.
                                 This parameter must be a number between Min_Data = 0 and Max_Data = 12 if the RTC_HourFormat_12 is selected.
                                 This parameter must be a number between Min_Data = 0 and Max_Data = 23 if the RTC_HourFormat_24 is selected  */

  uint8_t Minutes;          /*!< Specifies the RTC Time Minutes.
                                 This parameter must be a number between Min_Data = 0 and Max_Data = 59 */

  uint8_t Seconds;          /*!< Specifies the RTC Time Seconds.
                                 This parameter must be a number between Min_Data = 0 and Max_Data = 59 */

  uint8_t TimeFormat;       /*!< Specifies the RTC AM/PM Time.
                                 This parameter can be a value of @ref RTC_AM_PM_Definitions */

  uint32_t SubSeconds;     /*!< Specifies the RTC_SSR RTC Sub Second register content.
                                 This parameter corresponds to a time unit range between [0-1] Second
                                 with [1 Sec / SecondFraction +1] granularity */

  uint32_t SecondFraction;  /*!< Specifies the range or granularity of Sub Second register content
                                 corresponding to Synchronous pre-scaler factor value (PREDIV_S)
                                 This parameter corresponds to a time unit range between [0-1] Second
                                 with [1 Sec / SecondFraction +1] granularity.
                                 This field will be used only by HAL_RTC_GetTime function */

  uint32_t DayLightSaving;  /*!< This interface is deprecated. To manage Daylight Saving Time,
                                 please use HAL_RTC_DST_xxx functions */

  uint32_t StoreOperation;  /*!< This interface is deprecated. To manage Daylight Saving Time,
                                 please use HAL_RTC_DST_xxx functions */
}RTC_TimeTypeDef;                                       /* 代码块结束 */

/**
  * @brief  RTC Date structure definition
  */
typedef struct                                          /* 类型定义 */
{                                                       /* 代码块开始 */
  uint8_t WeekDay;  /*!< Specifies the RTC Date WeekDay.
                         This parameter can be a value of @ref RTC_WeekDay_Definitions */

  uint8_t Month;    /*!< Specifies the RTC Date Month (in BCD format).
                         This parameter can be a value of @ref RTC_Month_Date_Definitions */

  uint8_t Date;     /*!< Specifies the RTC Date.
                         This parameter must be a number between Min_Data = 1 and Max_Data = 31 */

  uint8_t Year;     /*!< Specifies the RTC Date Year.
                         This parameter must be a number between Min_Data = 0 and Max_Data = 99 */

}RTC_DateTypeDef;                                       /* 代码块结束 */

/**
  * @brief  RTC Alarm structure definition
  */
typedef struct                                          /* 类型定义 */
{                                                       /* 代码块开始 */
  RTC_TimeTypeDef AlarmTime;     /*!< Specifies the RTC Alarm Time members */

  uint32_t AlarmMask;            /*!< Specifies the RTC Alarm Masks.
                                      This parameter can be a value of @ref RTC_AlarmMask_Definitions */

  uint32_t AlarmSubSecondMask;   /*!< Specifies the RTC Alarm SubSeconds Masks.
                                      This parameter can be a value of @ref RTC_Alarm_Sub_Seconds_Masks_Definitions */

  uint32_t AlarmDateWeekDaySel;  /*!< Specifies the RTC Alarm is on Date or WeekDay.
                                     This parameter can be a value of @ref RTC_AlarmDateWeekDay_Definitions */

  uint8_t AlarmDateWeekDay;      /*!< Specifies the RTC Alarm Date/WeekDay.
                                      If the Alarm Date is selected, this parameter must be set to a value in the 1-31 range.
                                      If the Alarm WeekDay is selected, this parameter can be a value of @ref RTC_WeekDay_Definitions */

  uint32_t Alarm;                /*!< Specifies the alarm .
                                      This parameter can be a value of @ref RTC_Alarms_Definitions */
}RTC_AlarmTypeDef;                                      /* 代码块结束 */

/**
  * @brief  RTC Handle Structure definition
  */
#if (USE_HAL_RTC_REGISTER_CALLBACKS == 1)               /* 条件编译 */
typedef struct __RTC_HandleTypeDef                      /* 类型定义 */
#else                                                   /* 条件编译否则 */
typedef struct                                          /* 类型定义 */
#endif /* (USE_HAL_RTC_REGISTER_CALLBACKS) */
{                                                       /* 代码块开始 */
  RTC_TypeDef                 *Instance;  /*!< Register base address    */

  RTC_InitTypeDef             Init;       /*!< RTC required parameters  */

  HAL_LockTypeDef             Lock;       /*!< RTC locking object       */

  __IO HAL_RTCStateTypeDef    State;      /*!< Time communication state */

#if (USE_HAL_RTC_REGISTER_CALLBACKS == 1)               /* 条件编译 */
  void  (* AlarmAEventCallback)      ( struct __RTC_HandleTypeDef * hrtc);  /*!< RTC Alarm A Event callback         */

  void  (* AlarmBEventCallback)      ( struct __RTC_HandleTypeDef * hrtc);  /*!< RTC Alarm B Event callback         */

  void  (* TimeStampEventCallback)   ( struct __RTC_HandleTypeDef * hrtc);  /*!< RTC TimeStamp Event callback       */

  void  (* WakeUpTimerEventCallback) ( struct __RTC_HandleTypeDef * hrtc);  /*!< RTC WakeUpTimer Event callback     */

  void  (* Tamper1EventCallback)     ( struct __RTC_HandleTypeDef * hrtc);  /*!< RTC Tamper 1 Event callback        */

  void  (* Tamper2EventCallback)     ( struct __RTC_HandleTypeDef * hrtc);  /*!< RTC Tamper 2 Event callback        */

  void  (* MspInitCallback)          ( struct __RTC_HandleTypeDef * hrtc);  /*!< RTC Msp Init callback              */

  void  (* MspDeInitCallback)        ( struct __RTC_HandleTypeDef * hrtc);  /*!< RTC Msp DeInit callback            */

#endif /* (USE_HAL_RTC_REGISTER_CALLBACKS) */

}RTC_HandleTypeDef;                                     /* 代码块结束 */

#if (USE_HAL_RTC_REGISTER_CALLBACKS == 1)               /* 条件编译 */
/**
  * @brief  HAL RTC Callback ID enumeration definition
  */
typedef enum                                            /* 类型定义 */
{                                                       /* 代码块开始 */
  HAL_RTC_ALARM_A_EVENT_CB_ID           = 0x00u,    /*!< RTC Alarm A Event Callback ID       */
  HAL_RTC_ALARM_B_EVENT_CB_ID           = 0x01u,    /*!< RTC Alarm B Event Callback ID       */
  HAL_RTC_TIMESTAMP_EVENT_CB_ID         = 0x02u,    /*!< RTC TimeStamp Event Callback ID     */
  HAL_RTC_WAKEUPTIMER_EVENT_CB_ID       = 0x03u,    /*!< RTC Wake-Up Timer Event Callback ID */
  HAL_RTC_TAMPER1_EVENT_CB_ID           = 0x04u,    /*!< RTC Tamper 1 Callback ID            */
  HAL_RTC_TAMPER2_EVENT_CB_ID           = 0x05u,    /*!< RTC Tamper 2 Callback ID            */
  HAL_RTC_MSPINIT_CB_ID                 = 0x0Eu,    /*!< RTC Msp Init callback ID            */
  HAL_RTC_MSPDEINIT_CB_ID               = 0x0Fu     /*!< RTC Msp DeInit callback ID          */
}HAL_RTC_CallbackIDTypeDef;                             /* 代码块结束 */

/**
  * @brief  HAL RTC Callback pointer definition
  */
typedef  void (*pRTC_CallbackTypeDef)(RTC_HandleTypeDef * hrtc); /*!< pointer to an RTC callback function */
#endif /* USE_HAL_RTC_REGISTER_CALLBACKS */

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup RTC_Exported_Constants RTC Exported Constants
  * @{
  */

/** @defgroup RTC_Hour_Formats RTC Hour Formats
  * @{
  */
#define RTC_HOURFORMAT_24              0x00000000U      /* 定义宏 */
#define RTC_HOURFORMAT_12              0x00000040U      /* 定义宏 */
/**
  * @}
  */

/** @defgroup RTC_Output_selection_Definitions RTC Output Selection Definitions
  * @{
  */
#define RTC_OUTPUT_DISABLE             0x00000000U      /* 定义宏 */
#define RTC_OUTPUT_ALARMA              0x00200000U      /* 定义宏 */
#define RTC_OUTPUT_ALARMB              0x00400000U      /* 定义宏 */
#define RTC_OUTPUT_WAKEUP              0x00600000U      /* 定义宏 */
/**
  * @}
  */

/** @defgroup RTC_Output_Polarity_Definitions RTC Output Polarity Definitions
  * @{
  */
#define RTC_OUTPUT_POLARITY_HIGH       0x00000000U      /* 定义宏 */
#define RTC_OUTPUT_POLARITY_LOW        0x00100000U      /* 定义宏 */
/**
  * @}
  */

/** @defgroup RTC_Output_Type_ALARM_OUT RTC Output Type ALARM OUT
  * @{
  */
#define RTC_OUTPUT_TYPE_OPENDRAIN      0x00000000U      /* 定义宏 */
#define RTC_OUTPUT_TYPE_PUSHPULL       0x00040000U      /* 定义宏 */
/**
  * @}
  */

/** @defgroup RTC_AM_PM_Definitions RTC AM PM Definitions
  * @{
  */
#define RTC_HOURFORMAT12_AM            ((uint8_t)0x00)  /* 定义宏 */
#define RTC_HOURFORMAT12_PM            ((uint8_t)0x40)  /* 定义宏 */
/**
  * @}
  */

/** @defgroup RTC_DayLightSaving_Definitions RTC DayLight Saving Definitions
  * @{
  */
#define RTC_DAYLIGHTSAVING_SUB1H       0x00020000U      /* 定义宏 */
#define RTC_DAYLIGHTSAVING_ADD1H       0x00010000U      /* 定义宏 */
#define RTC_DAYLIGHTSAVING_NONE        0x00000000U      /* 定义宏 */
/**
  * @}
  */

/** @defgroup RTC_StoreOperation_Definitions RTC Store Operation Definitions
  * @{
  */
#define RTC_STOREOPERATION_RESET        0x00000000U     /* 定义宏 */
#define RTC_STOREOPERATION_SET          0x00040000U     /* 定义宏 */
/**
  * @}
  */

/** @defgroup RTC_Input_parameter_format_definitions RTC Input Parameter Format Definitions
  * @{
  */
#define RTC_FORMAT_BIN                      0x00000000U  /* 定义宏 */
#define RTC_FORMAT_BCD                      0x00000001U  /* 定义宏 */
/**
  * @}
  */

/** @defgroup RTC_Month_Date_Definitions RTC Month Date Definitions
  * @{
  */
/* Coded in BCD format */
#define RTC_MONTH_JANUARY              ((uint8_t)0x01)  /* 定义宏 */
#define RTC_MONTH_FEBRUARY             ((uint8_t)0x02)  /* 定义宏 */
#define RTC_MONTH_MARCH                ((uint8_t)0x03)  /* 定义宏 */
#define RTC_MONTH_APRIL                ((uint8_t)0x04)  /* 定义宏 */
#define RTC_MONTH_MAY                  ((uint8_t)0x05)  /* 定义宏 */
#define RTC_MONTH_JUNE                 ((uint8_t)0x06)  /* 定义宏 */
#define RTC_MONTH_JULY                 ((uint8_t)0x07)  /* 定义宏 */
#define RTC_MONTH_AUGUST               ((uint8_t)0x08)  /* 定义宏 */
#define RTC_MONTH_SEPTEMBER            ((uint8_t)0x09)  /* 定义宏 */
#define RTC_MONTH_OCTOBER              ((uint8_t)0x10)  /* 定义宏 */
#define RTC_MONTH_NOVEMBER             ((uint8_t)0x11)  /* 定义宏 */
#define RTC_MONTH_DECEMBER             ((uint8_t)0x12)  /* 定义宏 */
/**
  * @}
  */

/** @defgroup RTC_WeekDay_Definitions RTC WeekDay Definitions
  * @{
  */
#define RTC_WEEKDAY_MONDAY             ((uint8_t)0x01)  /* 定义宏 */
#define RTC_WEEKDAY_TUESDAY            ((uint8_t)0x02)  /* 定义宏 */
#define RTC_WEEKDAY_WEDNESDAY          ((uint8_t)0x03)  /* 定义宏 */
#define RTC_WEEKDAY_THURSDAY           ((uint8_t)0x04)  /* 定义宏 */
#define RTC_WEEKDAY_FRIDAY             ((uint8_t)0x05)  /* 定义宏 */
#define RTC_WEEKDAY_SATURDAY           ((uint8_t)0x06)  /* 定义宏 */
#define RTC_WEEKDAY_SUNDAY             ((uint8_t)0x07)  /* 定义宏 */
/**
  * @}
  */

/** @defgroup RTC_AlarmDateWeekDay_Definitions RTC Alarm Date WeekDay Definitions
  * @{
  */
#define RTC_ALARMDATEWEEKDAYSEL_DATE      0x00000000U   /* 定义宏 */
#define RTC_ALARMDATEWEEKDAYSEL_WEEKDAY   0x40000000U   /* 定义宏 */
/**
  * @}
  */

/** @defgroup RTC_AlarmMask_Definitions RTC Alarm Mask Definitions
  * @{
  */
#define RTC_ALARMMASK_NONE                0x00000000U   /* 定义宏 */
#define RTC_ALARMMASK_DATEWEEKDAY         RTC_ALRMAR_MSK4  /* 定义宏 */
#define RTC_ALARMMASK_HOURS               RTC_ALRMAR_MSK3  /* 定义宏 */
#define RTC_ALARMMASK_MINUTES             RTC_ALRMAR_MSK2  /* 定义宏 */
#define RTC_ALARMMASK_SECONDS             RTC_ALRMAR_MSK1  /* 定义宏 */
#define RTC_ALARMMASK_ALL                 0x80808080U   /* 定义宏 */
/**
  * @}
  */

/** @defgroup RTC_Alarms_Definitions RTC Alarms Definitions
  * @{
  */
#define RTC_ALARM_A                       RTC_CR_ALRAE  /* 定义宏 */
#define RTC_ALARM_B                       RTC_CR_ALRBE  /* 定义宏 */
/**
  * @}
  */

/** @defgroup RTC_Alarm_Sub_Seconds_Masks_Definitions RTC Alarm Sub Seconds Masks Definitions
  * @{
  */
#define RTC_ALARMSUBSECONDMASK_ALL         0x00000000U  /*!< All Alarm SS fields are masked.
                                                                        There is no comparison on sub seconds
                                                                        for Alarm */
#define RTC_ALARMSUBSECONDMASK_SS14_1      0x01000000U  /*!< SS[14:1] are don't care in Alarm
                                                                        comparison. Only SS[0] is compared.    */
#define RTC_ALARMSUBSECONDMASK_SS14_2      0x02000000U  /*!< SS[14:2] are don't care in Alarm
                                                                        comparison. Only SS[1:0] are compared  */
#define RTC_ALARMSUBSECONDMASK_SS14_3      0x03000000U  /*!< SS[14:3] are don't care in Alarm
                                                                        comparison. Only SS[2:0] are compared  */
#define RTC_ALARMSUBSECONDMASK_SS14_4      0x04000000U  /*!< SS[14:4] are don't care in Alarm
                                                                        comparison. Only SS[3:0] are compared  */
#define RTC_ALARMSUBSECONDMASK_SS14_5      0x05000000U  /*!< SS[14:5] are don't care in Alarm
                                                                        comparison. Only SS[4:0] are compared  */
#define RTC_ALARMSUBSECONDMASK_SS14_6      0x06000000U  /*!< SS[14:6] are don't care in Alarm
                                                                        comparison. Only SS[5:0] are compared  */
#define RTC_ALARMSUBSECONDMASK_SS14_7      0x07000000U  /*!< SS[14:7] are don't care in Alarm
                                                                        comparison. Only SS[6:0] are compared  */
#define RTC_ALARMSUBSECONDMASK_SS14_8      0x08000000U  /*!< SS[14:8] are don't care in Alarm
                                                                        comparison. Only SS[7:0] are compared  */
#define RTC_ALARMSUBSECONDMASK_SS14_9      0x09000000U  /*!< SS[14:9] are don't care in Alarm
                                                                        comparison. Only SS[8:0] are compared  */
#define RTC_ALARMSUBSECONDMASK_SS14_10     0x0A000000U  /*!< SS[14:10] are don't care in Alarm
                                                                        comparison. Only SS[9:0] are compared  */
#define RTC_ALARMSUBSECONDMASK_SS14_11     0x0B000000U  /*!< SS[14:11] are don't care in Alarm
                                                                        comparison. Only SS[10:0] are compared */
#define RTC_ALARMSUBSECONDMASK_SS14_12     0x0C000000U  /*!< SS[14:12] are don't care in Alarm
                                                                        comparison.Only SS[11:0] are compared  */
#define RTC_ALARMSUBSECONDMASK_SS14_13     0x0D000000U  /*!< SS[14:13] are don't care in Alarm
                                                                        comparison. Only SS[12:0] are compared */
#define RTC_ALARMSUBSECONDMASK_SS14        0x0E000000U  /*!< SS[14] is don't care in Alarm
                                                                        comparison.Only SS[13:0] are compared  */
#define RTC_ALARMSUBSECONDMASK_NONE        0x0F000000U  /*!< SS[14:0] are compared and must match
                                                                        to activate alarm. */
/**
  * @}
  */

/** @defgroup RTC_Interrupts_Definitions RTC Interrupts Definitions
  * @{
  */
#define RTC_IT_TS                         0x00008000U   /* 定义宏 */
#define RTC_IT_WUT                        0x00004000U   /* 定义宏 */
#define RTC_IT_ALRB                       0x00002000U   /* 定义宏 */
#define RTC_IT_ALRA                       0x00001000U   /* 定义宏 */
#define RTC_IT_TAMP                       0x00000004U /* Used only to Enable the Tamper Interrupt */
#define RTC_IT_TAMP1                      0x00020000U   /* 定义宏 */
#define RTC_IT_TAMP2                      0x00040000U   /* 定义宏 */
/**
  * @}
  */

/** @defgroup RTC_Flags_Definitions RTC Flags Definitions
  * @{
  */
#define RTC_FLAG_RECALPF                  0x00010000U   /* 定义宏 */
#define RTC_FLAG_TAMP2F                   0x00004000U   /* 定义宏 */
#define RTC_FLAG_TAMP1F                   0x00002000U   /* 定义宏 */
#define RTC_FLAG_TSOVF                    0x00001000U   /* 定义宏 */
#define RTC_FLAG_TSF                      0x00000800U   /* 定义宏 */
#define RTC_FLAG_WUTF                     0x00000400U   /* 定义宏 */
#define RTC_FLAG_ALRBF                    0x00000200U   /* 定义宏 */
#define RTC_FLAG_ALRAF                    0x00000100U   /* 定义宏 */
#define RTC_FLAG_INITF                    0x00000040U   /* 定义宏 */
#define RTC_FLAG_RSF                      0x00000020U   /* 定义宏 */
#define RTC_FLAG_INITS                    0x00000010U   /* 定义宏 */
#define RTC_FLAG_SHPF                     0x00000008U   /* 定义宏 */
#define RTC_FLAG_WUTWF                    0x00000004U   /* 定义宏 */
#define RTC_FLAG_ALRBWF                   0x00000002U   /* 定义宏 */
#define RTC_FLAG_ALRAWF                   0x00000001U   /* 定义宏 */
/**
  * @}
  */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/** @defgroup RTC_Exported_Macros RTC Exported Macros
  * @{
  */

/** @brief Reset RTC handle state
  * @param  __HANDLE__ specifies the RTC handle.
  * @retval None
  */
#if (USE_HAL_RTC_REGISTER_CALLBACKS == 1)               /* 条件编译 */
#define __HAL_RTC_RESET_HANDLE_STATE(__HANDLE__) do{\
                                                      (__HANDLE__)->State = HAL_RTC_STATE_RESET;\
                                                      (__HANDLE__)->MspInitCallback = NULL;\
                                                      (__HANDLE__)->MspDeInitCallback = NULL;\
                                                     }while(0u)  /* 代码块结束 */
#else                                                   /* 条件编译否则 */
#define __HAL_RTC_RESET_HANDLE_STATE(__HANDLE__) ((__HANDLE__)->State = HAL_RTC_STATE_RESET)  /* 定义包含保护宏 */
#endif /* USE_HAL_RTC_REGISTER_CALLBACKS */

/**
  * @brief  Disable the write protection for RTC registers.
  * @param  __HANDLE__ specifies the RTC handle.
  * @retval None
  */
#define __HAL_RTC_WRITEPROTECTION_DISABLE(__HANDLE__)             \
                        do{                                       \
                            (__HANDLE__)->Instance->WPR = 0xCAU;  \
                            (__HANDLE__)->Instance->WPR = 0x53U;  \
                          } while(0U)                   /* 代码块结束 */

/**
  * @brief  Enable the write protection for RTC registers.
  * @param  __HANDLE__ specifies the RTC handle.
  * @retval None
  */
#define __HAL_RTC_WRITEPROTECTION_ENABLE(__HANDLE__)              \
                        do{                                       \
                            (__HANDLE__)->Instance->WPR = 0xFFU;  \
                          } while(0U)                   /* 代码块结束 */

/**
  * @brief  Enable the RTC ALARMA peripheral.
  * @param  __HANDLE__ specifies the RTC handle.
  * @retval None
  */
#define __HAL_RTC_ALARMA_ENABLE(__HANDLE__)                           ((__HANDLE__)->Instance->CR |= (RTC_CR_ALRAE))  /* 定义包含保护宏 */

/**
  * @brief  Disable the RTC ALARMA peripheral.
  * @param  __HANDLE__ specifies the RTC handle.
  * @retval None
  */
#define __HAL_RTC_ALARMA_DISABLE(__HANDLE__)                          ((__HANDLE__)->Instance->CR &= ~(RTC_CR_ALRAE))  /* 定义包含保护宏 */

/**
  * @brief  Enable the RTC ALARMB peripheral.
  * @param  __HANDLE__ specifies the RTC handle.
  * @retval None
  */
#define __HAL_RTC_ALARMB_ENABLE(__HANDLE__)                           ((__HANDLE__)->Instance->CR |= (RTC_CR_ALRBE))  /* 定义包含保护宏 */

/**
  * @brief  Disable the RTC ALARMB peripheral.
  * @param  __HANDLE__ specifies the RTC handle.
  * @retval None
  */
#define __HAL_RTC_ALARMB_DISABLE(__HANDLE__)                          ((__HANDLE__)->Instance->CR &= ~(RTC_CR_ALRBE))  /* 定义包含保护宏 */

/**
  * @brief  Enable the RTC Alarm interrupt.
  * @param  __HANDLE__ specifies the RTC handle.
  * @param  __INTERRUPT__ specifies the RTC Alarm interrupt sources to be enabled or disabled.
  *          This parameter can be any combination of the following values:
  *             @arg RTC_IT_ALRA: Alarm A interrupt
  *             @arg RTC_IT_ALRB: Alarm B interrupt
  * @retval None
  */
#define __HAL_RTC_ALARM_ENABLE_IT(__HANDLE__, __INTERRUPT__)          ((__HANDLE__)->Instance->CR |= (__INTERRUPT__))  /* 定义包含保护宏 */

/**
  * @brief  Disable the RTC Alarm interrupt.
  * @param  __HANDLE__ specifies the RTC handle.
  * @param  __INTERRUPT__ specifies the RTC Alarm interrupt sources to be enabled or disabled.
  *         This parameter can be any combination of the following values:
  *            @arg RTC_IT_ALRA: Alarm A interrupt
  *            @arg RTC_IT_ALRB: Alarm B interrupt
  * @retval None
  */
#define __HAL_RTC_ALARM_DISABLE_IT(__HANDLE__, __INTERRUPT__)         ((__HANDLE__)->Instance->CR &= ~(__INTERRUPT__))  /* 定义包含保护宏 */

/**
  * @brief  Check whether the specified RTC Alarm interrupt has occurred or not.
  * @param  __HANDLE__ specifies the RTC handle.
  * @param  __INTERRUPT__ specifies the RTC Alarm interrupt to check.
  *         This parameter can be:
  *            @arg RTC_IT_ALRA: Alarm A interrupt
  *            @arg RTC_IT_ALRB: Alarm B interrupt
  * @retval None
  */
#define __HAL_RTC_ALARM_GET_IT(__HANDLE__, __INTERRUPT__)           (((((__HANDLE__)->Instance->ISR)& ((__INTERRUPT__)>> 4U)) != RESET)? SET : RESET)  /* 定义包含保护宏 */

/**
  * @brief  Get the selected RTC Alarm's flag status.
  * @param  __HANDLE__ specifies the RTC handle.
  * @param  __FLAG__ specifies the RTC Alarm Flag to check.
  *         This parameter can be:
  *            @arg RTC_FLAG_ALRAF
  *            @arg RTC_FLAG_ALRBF
  *            @arg RTC_FLAG_ALRAWF
  *            @arg RTC_FLAG_ALRBWF
  * @retval None
  */
#define __HAL_RTC_ALARM_GET_FLAG(__HANDLE__, __FLAG__)                (((((__HANDLE__)->Instance->ISR) & (__FLAG__)) != RESET)? SET : RESET)  /* 定义包含保护宏 */

/**
  * @brief  Clear the RTC Alarm's pending flags.
  * @param  __HANDLE__ specifies the RTC handle.
  * @param  __FLAG__ specifies the RTC Alarm Flag sources to be enabled or disabled.
  *          This parameter can be:
  *             @arg RTC_FLAG_ALRAF
  *             @arg RTC_FLAG_ALRBF
  * @retval None
  */
#define __HAL_RTC_ALARM_CLEAR_FLAG(__HANDLE__, __FLAG__)                  ((__HANDLE__)->Instance->ISR) = (~((__FLAG__) | RTC_ISR_INIT)|((__HANDLE__)->Instance->ISR & RTC_ISR_INIT))  /* 定义包含保护宏 */


/**
  * @brief  Check whether the specified RTC Alarm interrupt has been enabled or not.
  * @param  __HANDLE__ specifies the RTC handle.
  * @param  __INTERRUPT__ specifies the RTC Alarm interrupt sources to check.
  *         This parameter can be:
  *            @arg RTC_IT_ALRA: Alarm A interrupt
  *            @arg RTC_IT_ALRB: Alarm B interrupt
  * @retval None
  */
#define __HAL_RTC_ALARM_GET_IT_SOURCE(__HANDLE__, __INTERRUPT__)     (((((__HANDLE__)->Instance->CR) & (__INTERRUPT__)) != RESET) ? SET : RESET)  /* 定义包含保护宏 */

/**
  * @brief  Enable interrupt on the RTC Alarm associated Exti line.
  * @retval None
  */
#define __HAL_RTC_ALARM_EXTI_ENABLE_IT()            (EXTI->IMR |= RTC_EXTI_LINE_ALARM_EVENT)  /* 定义包含保护宏 */

/**
  * @brief  Disable interrupt on the RTC Alarm associated Exti line.
  * @retval None
  */
#define __HAL_RTC_ALARM_EXTI_DISABLE_IT()           (EXTI->IMR &= ~(RTC_EXTI_LINE_ALARM_EVENT))  /* 定义包含保护宏 */

/**
  * @brief  Enable event on the RTC Alarm associated Exti line.
  * @retval None.
  */
#define __HAL_RTC_ALARM_EXTI_ENABLE_EVENT()         (EXTI->EMR |= RTC_EXTI_LINE_ALARM_EVENT)  /* 定义包含保护宏 */

/**
  * @brief  Disable event on the RTC Alarm associated Exti line.
  * @retval None.
  */
#define __HAL_RTC_ALARM_EXTI_DISABLE_EVENT()         (EXTI->EMR &= ~(RTC_EXTI_LINE_ALARM_EVENT))  /* 定义包含保护宏 */

/**
  * @brief  Enable falling edge trigger on the RTC Alarm associated Exti line.
  * @retval None.
  */
#define __HAL_RTC_ALARM_EXTI_ENABLE_FALLING_EDGE()   (EXTI->FTSR |= RTC_EXTI_LINE_ALARM_EVENT)  /* 定义包含保护宏 */

/**
  * @brief  Disable falling edge trigger on the RTC Alarm associated Exti line.
  * @retval None.
  */
#define __HAL_RTC_ALARM_EXTI_DISABLE_FALLING_EDGE()  (EXTI->FTSR &= ~(RTC_EXTI_LINE_ALARM_EVENT))  /* 定义包含保护宏 */

/**
  * @brief  Enable rising edge trigger on the RTC Alarm associated Exti line.
  * @retval None.
  */
#define __HAL_RTC_ALARM_EXTI_ENABLE_RISING_EDGE()    (EXTI->RTSR |= RTC_EXTI_LINE_ALARM_EVENT)  /* 定义包含保护宏 */

/**
  * @brief  Disable rising edge trigger on the RTC Alarm associated Exti line.
  * @retval None.
  */
#define __HAL_RTC_ALARM_EXTI_DISABLE_RISING_EDGE()   (EXTI->RTSR &= ~(RTC_EXTI_LINE_ALARM_EVENT))  /* 定义包含保护宏 */

/**
  * @brief  Enable rising & falling edge trigger on the RTC Alarm associated Exti line.
  * @retval None.
  */
#define __HAL_RTC_ALARM_EXTI_ENABLE_RISING_FALLING_EDGE() do { __HAL_RTC_ALARM_EXTI_ENABLE_RISING_EDGE(); \
                                                               __HAL_RTC_ALARM_EXTI_ENABLE_FALLING_EDGE();\
                                                             } while(0U)  /* 代码块结束 */

/**
  * @brief  Disable rising & falling edge trigger on the RTC Alarm associated Exti line.
  * @retval None.
  */
#define __HAL_RTC_ALARM_EXTI_DISABLE_RISING_FALLING_EDGE() do { __HAL_RTC_ALARM_EXTI_DISABLE_RISING_EDGE();\
                                                                __HAL_RTC_ALARM_EXTI_DISABLE_FALLING_EDGE();\
                                                              } while(0U)  /* 代码块结束 */

/**
  * @brief Check whether the RTC Alarm associated Exti line interrupt flag is set or not.
  * @retval Line Status.
  */
#define __HAL_RTC_ALARM_EXTI_GET_FLAG()              (EXTI->PR & RTC_EXTI_LINE_ALARM_EVENT)  /* 定义包含保护宏 */

/**
  * @brief Clear the RTC Alarm associated Exti line flag.
  * @retval None.
  */
#define __HAL_RTC_ALARM_EXTI_CLEAR_FLAG()            (EXTI->PR = RTC_EXTI_LINE_ALARM_EVENT)  /* 定义包含保护宏 */

/**
  * @brief Generate a Software interrupt on RTC Alarm associated Exti line.
  * @retval None.
  */
#define __HAL_RTC_ALARM_EXTI_GENERATE_SWIT()         (EXTI->SWIER |= RTC_EXTI_LINE_ALARM_EVENT)  /* 定义包含保护宏 */
/**
  * @}
  */

/* Include RTC HAL Extension module */
#include "stm32f4xx_hal_rtc_ex.h"                       /* 包含 STM32/HAL 头文件 */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup RTC_Exported_Functions
  * @{
  */

/** @addtogroup RTC_Exported_Functions_Group1
  * @{
  */
/* Initialization and de-initialization functions  ****************************/
HAL_StatusTypeDef HAL_RTC_Init(RTC_HandleTypeDef *hrtc);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_RTC_DeInit(RTC_HandleTypeDef *hrtc);  /* HAL状态类型函数 */
void       HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc);    /* 函数定义或声明 */
void       HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc);  /* 函数定义或声明 */

/* Callbacks Register/UnRegister functions  ***********************************/
#if (USE_HAL_RTC_REGISTER_CALLBACKS == 1)               /* 条件编译 */
HAL_StatusTypeDef HAL_RTC_RegisterCallback(RTC_HandleTypeDef *hrtc, HAL_RTC_CallbackIDTypeDef CallbackID, pRTC_CallbackTypeDef pCallback);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_RTC_UnRegisterCallback(RTC_HandleTypeDef *hrtc, HAL_RTC_CallbackIDTypeDef CallbackID);  /* HAL状态类型函数 */
#endif /* USE_HAL_RTC_REGISTER_CALLBACKS */
/**
  * @}
  */

/** @addtogroup RTC_Exported_Functions_Group2
  * @{
  */
/* RTC Time and Date functions ************************************************/
HAL_StatusTypeDef HAL_RTC_SetTime(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, uint32_t Format);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_RTC_GetTime(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, uint32_t Format);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_RTC_SetDate(RTC_HandleTypeDef *hrtc, RTC_DateTypeDef *sDate, uint32_t Format);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_RTC_GetDate(RTC_HandleTypeDef *hrtc, RTC_DateTypeDef *sDate, uint32_t Format);  /* HAL状态类型函数 */
void              HAL_RTC_DST_Add1Hour(RTC_HandleTypeDef *hrtc);  /* 函数定义或声明 */
void              HAL_RTC_DST_Sub1Hour(RTC_HandleTypeDef *hrtc);  /* 函数定义或声明 */
void              HAL_RTC_DST_SetStoreOperation(RTC_HandleTypeDef *hrtc);  /* 函数定义或声明 */
void              HAL_RTC_DST_ClearStoreOperation(RTC_HandleTypeDef *hrtc);  /* 函数定义或声明 */
uint32_t          HAL_RTC_DST_ReadStoreOperation(RTC_HandleTypeDef *hrtc);  /* 32位无符号变量/函数 */
/**
  * @}
  */

/** @addtogroup RTC_Exported_Functions_Group3
  * @{
  */
/* RTC Alarm functions ********************************************************/
HAL_StatusTypeDef HAL_RTC_SetAlarm(RTC_HandleTypeDef *hrtc, RTC_AlarmTypeDef *sAlarm, uint32_t Format);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_RTC_SetAlarm_IT(RTC_HandleTypeDef *hrtc, RTC_AlarmTypeDef *sAlarm, uint32_t Format);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_RTC_DeactivateAlarm(RTC_HandleTypeDef *hrtc, uint32_t Alarm);  /* HAL状态类型函数 */
HAL_StatusTypeDef HAL_RTC_GetAlarm(RTC_HandleTypeDef *hrtc, RTC_AlarmTypeDef *sAlarm, uint32_t Alarm, uint32_t Format);  /* HAL状态类型函数 */
void                HAL_RTC_AlarmIRQHandler(RTC_HandleTypeDef *hrtc);  /* 函数定义或声明 */
HAL_StatusTypeDef   HAL_RTC_PollForAlarmAEvent(RTC_HandleTypeDef *hrtc, uint32_t Timeout);  /* HAL状态类型函数 */
void         HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc);  /* 函数定义或声明 */
/**
  * @}
  */

/** @addtogroup RTC_Exported_Functions_Group4
  * @{
  */
/* Peripheral Control functions ***********************************************/
HAL_StatusTypeDef   HAL_RTC_WaitForSynchro(RTC_HandleTypeDef* hrtc);  /* HAL状态类型函数 */
/**
  * @}
  */

/** @addtogroup RTC_Exported_Functions_Group5
  * @{
  */
/* Peripheral State functions *************************************************/
HAL_RTCStateTypeDef HAL_RTC_GetState(RTC_HandleTypeDef *hrtc);  /* HAL库函数调用 */
/**
  * @}
  */

/**
  * @}
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/** @defgroup RTC_Private_Constants RTC Private Constants
  * @{
  */
/* Masks Definition */
#define RTC_TR_RESERVED_MASK    0x007F7F7FU             /* 定义宏 */
#define RTC_DR_RESERVED_MASK    0x00FFFF3FU             /* 定义宏 */
#define RTC_INIT_MASK           0xFFFFFFFFU             /* 定义宏 */
#define RTC_RSF_MASK            0xFFFFFF5FU             /* 定义宏 */
#define RTC_FLAGS_MASK          ((uint32_t)(RTC_FLAG_TSOVF | RTC_FLAG_TSF | RTC_FLAG_WUTF | \
                                            RTC_FLAG_ALRBF | RTC_FLAG_ALRAF | RTC_FLAG_INITF | \
                                            RTC_FLAG_RSF | RTC_FLAG_INITS | RTC_FLAG_WUTWF | \
                                            RTC_FLAG_ALRBWF | RTC_FLAG_ALRAWF | RTC_FLAG_TAMP1F | \
                                            RTC_FLAG_RECALPF | RTC_FLAG_SHPF))  /* 源代码 */

#define RTC_TIMEOUT_VALUE       1000                    /* 定义宏 */

#define RTC_EXTI_LINE_ALARM_EVENT             ((uint32_t)EXTI_IMR_MR17)  /*!< External interrupt line 17 Connected to the RTC Alarm event */
/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/** @defgroup RTC_Private_Macros RTC Private Macros
  * @{
  */

/** @defgroup RTC_IS_RTC_Definitions RTC Private macros to check input parameters
  * @{
  */
#define IS_RTC_HOUR_FORMAT(FORMAT)     (((FORMAT) == RTC_HOURFORMAT_12) || \
                                        ((FORMAT) == RTC_HOURFORMAT_24))  /* 源代码 */
#define IS_RTC_OUTPUT(OUTPUT) (((OUTPUT) == RTC_OUTPUT_DISABLE) || \
                               ((OUTPUT) == RTC_OUTPUT_ALARMA)  || \
                               ((OUTPUT) == RTC_OUTPUT_ALARMB)  || \
                               ((OUTPUT) == RTC_OUTPUT_WAKEUP))  /* 源代码 */
#define IS_RTC_OUTPUT_POL(POL) (((POL) == RTC_OUTPUT_POLARITY_HIGH) || \
                                ((POL) == RTC_OUTPUT_POLARITY_LOW))  /* 源代码 */
#define IS_RTC_OUTPUT_TYPE(TYPE) (((TYPE) == RTC_OUTPUT_TYPE_OPENDRAIN) || \
                                  ((TYPE) == RTC_OUTPUT_TYPE_PUSHPULL))  /* 源代码 */
#define IS_RTC_HOUR12(HOUR)            (((HOUR) > 0U) && ((HOUR) <= 12U))  /* 定义宏 */
#define IS_RTC_HOUR24(HOUR)            ((HOUR) <= 23U)  /* 定义宏 */
#define IS_RTC_ASYNCH_PREDIV(PREDIV)   ((PREDIV) <= 0x7FU)  /* 定义宏 */
#define IS_RTC_SYNCH_PREDIV(PREDIV)    ((PREDIV) <= 0x7FFFU)  /* 定义宏 */
#define IS_RTC_MINUTES(MINUTES)        ((MINUTES) <= 59U)  /* 定义宏 */
#define IS_RTC_SECONDS(SECONDS)        ((SECONDS) <= 59U)  /* 定义宏 */

#define IS_RTC_HOURFORMAT12(PM)  (((PM) == RTC_HOURFORMAT12_AM) || ((PM) == RTC_HOURFORMAT12_PM))  /* 定义宏 */
#define IS_RTC_DAYLIGHT_SAVING(SAVE) (((SAVE) == RTC_DAYLIGHTSAVING_SUB1H) || \
                                      ((SAVE) == RTC_DAYLIGHTSAVING_ADD1H) || \
                                      ((SAVE) == RTC_DAYLIGHTSAVING_NONE))  /* 源代码 */
#define IS_RTC_STORE_OPERATION(OPERATION) (((OPERATION) == RTC_STOREOPERATION_RESET) || \
                                           ((OPERATION) == RTC_STOREOPERATION_SET))  /* 源代码 */
#define IS_RTC_FORMAT(FORMAT) (((FORMAT) == RTC_FORMAT_BIN) || ((FORMAT) == RTC_FORMAT_BCD))  /* 定义宏 */
#define IS_RTC_YEAR(YEAR)              ((YEAR) <= 99U)  /* 定义宏 */
#define IS_RTC_MONTH(MONTH)            (((MONTH) >= 1U) && ((MONTH) <= 12U))  /* 定义宏 */
#define IS_RTC_DATE(DATE)              (((DATE) >= 1U) && ((DATE) <= 31U))  /* 定义宏 */
#define IS_RTC_WEEKDAY(WEEKDAY) (((WEEKDAY) == RTC_WEEKDAY_MONDAY)    || \
                                 ((WEEKDAY) == RTC_WEEKDAY_TUESDAY)   || \
                                 ((WEEKDAY) == RTC_WEEKDAY_WEDNESDAY) || \
                                 ((WEEKDAY) == RTC_WEEKDAY_THURSDAY)  || \
                                 ((WEEKDAY) == RTC_WEEKDAY_FRIDAY)    || \
                                 ((WEEKDAY) == RTC_WEEKDAY_SATURDAY)  || \
                                 ((WEEKDAY) == RTC_WEEKDAY_SUNDAY))  /* 源代码 */
#define IS_RTC_ALARM_DATE_WEEKDAY_DATE(DATE) (((DATE) > 0U) && ((DATE) <= 31U))  /* 定义宏 */
#define IS_RTC_ALARM_DATE_WEEKDAY_WEEKDAY(WEEKDAY) (((WEEKDAY) == RTC_WEEKDAY_MONDAY)    || \
                                                    ((WEEKDAY) == RTC_WEEKDAY_TUESDAY)   || \
                                                    ((WEEKDAY) == RTC_WEEKDAY_WEDNESDAY) || \
                                                    ((WEEKDAY) == RTC_WEEKDAY_THURSDAY)  || \
                                                    ((WEEKDAY) == RTC_WEEKDAY_FRIDAY)    || \
                                                    ((WEEKDAY) == RTC_WEEKDAY_SATURDAY)  || \
                                                    ((WEEKDAY) == RTC_WEEKDAY_SUNDAY))  /* 源代码 */
#define IS_RTC_ALARM_DATE_WEEKDAY_SEL(SEL) (((SEL) == RTC_ALARMDATEWEEKDAYSEL_DATE) || \
                                            ((SEL) == RTC_ALARMDATEWEEKDAYSEL_WEEKDAY))  /* 源代码 */
#define IS_RTC_ALARM_MASK(MASK)  (((MASK) & 0x7F7F7F7FU) == (uint32_t)RESET)  /* 定义宏 */
#define IS_RTC_ALARM(ALARM)      (((ALARM) == RTC_ALARM_A) || ((ALARM) == RTC_ALARM_B))  /* 定义宏 */
#define IS_RTC_ALARM_SUB_SECOND_VALUE(VALUE) ((VALUE) <= 0x00007FFFU)  /* 定义宏 */

#define IS_RTC_ALARM_SUB_SECOND_MASK(MASK)   (((MASK) == RTC_ALARMSUBSECONDMASK_ALL) || \
                                              ((MASK) == RTC_ALARMSUBSECONDMASK_SS14_1) || \
                                              ((MASK) == RTC_ALARMSUBSECONDMASK_SS14_2) || \
                                              ((MASK) == RTC_ALARMSUBSECONDMASK_SS14_3) || \
                                              ((MASK) == RTC_ALARMSUBSECONDMASK_SS14_4) || \
                                              ((MASK) == RTC_ALARMSUBSECONDMASK_SS14_5) || \
                                              ((MASK) == RTC_ALARMSUBSECONDMASK_SS14_6) || \
                                              ((MASK) == RTC_ALARMSUBSECONDMASK_SS14_7) || \
                                              ((MASK) == RTC_ALARMSUBSECONDMASK_SS14_8) || \
                                              ((MASK) == RTC_ALARMSUBSECONDMASK_SS14_9) || \
                                              ((MASK) == RTC_ALARMSUBSECONDMASK_SS14_10) || \
                                              ((MASK) == RTC_ALARMSUBSECONDMASK_SS14_11) || \
                                              ((MASK) == RTC_ALARMSUBSECONDMASK_SS14_12) || \
                                              ((MASK) == RTC_ALARMSUBSECONDMASK_SS14_13) || \
                                              ((MASK) == RTC_ALARMSUBSECONDMASK_SS14) || \
                                              ((MASK) == RTC_ALARMSUBSECONDMASK_NONE))  /* 源代码 */
/**
  * @}
  */

/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/
/** @defgroup RTC_Private_Functions RTC Private Functions
  * @{
  */
HAL_StatusTypeDef  RTC_EnterInitMode(RTC_HandleTypeDef* hrtc);  /* HAL状态类型函数 */
uint8_t            RTC_ByteToBcd2(uint8_t Value);       /* 8位无符号变量/函数 */
uint8_t            RTC_Bcd2ToByte(uint8_t Value);       /* 8位无符号变量/函数 */
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

#endif /* __STM32F4xx_HAL_RTC_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
