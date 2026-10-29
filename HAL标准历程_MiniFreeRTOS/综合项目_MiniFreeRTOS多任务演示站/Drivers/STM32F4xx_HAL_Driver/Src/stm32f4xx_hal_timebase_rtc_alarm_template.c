/* 文件: stm32f4xx_hal_timebase_rtc_alarm_template.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_timebase_rtc_alarm_template.c 
  * @author  MCD Application Team
  * @brief   HAL time base based on the hardware RTC_ALARM Template.
  *
  *          This file override the native HAL time base functions (defined as weak)
  *          to use the RTC ALARM for time base generation:
  *           + Intializes the RTC peripheral to increment the seconds registers each 1ms
  *           + The alarm is configured to assert an interrupt when the RTC reaches 1ms 
  *           + HAL_IncTick is called at each Alarm event and the time is reset to 00:00:00
  *           + HSE (default), LSE or LSI can be selected as RTC clock source  
 @verbatim
  ==============================================================================
                        ##### How to use this driver #####
  ==============================================================================
    [..]
    This file must be copied to the application folder and modified as follows:
    (#) Rename it to 'stm32f4xx_hal_timebase_rtc_alarm.c'
    (#) Add this file and the RTC HAL drivers to your project and uncomment
       HAL_RTC_MODULE_ENABLED define in stm32f4xx_hal_conf.h 

    [..]
    (@) HAL RTC alarm and HAL RTC wakeup drivers can抰 be used with low power modes:
        The wake up capability of the RTC may be intrusive in case of prior low power mode
        configuration requiring different wake up sources.
        Application/Example behavior is no more guaranteed 
    (@) The stm32f4xx_hal_timebase_tim use is recommended for the Applications/Examples
          requiring low power modes

  @endverbatim
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

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"                              /* 包含 STM32/HAL 头文件 */
/** @addtogroup STM32F4xx_HAL_Driver
  * @{
  */

/** @defgroup HAL_TimeBase_RTC_Alarm_Template  HAL TimeBase RTC Alarm Template
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Uncomment the line below to select the appropriate RTC Clock source for your application: 
  + RTC_CLOCK_SOURCE_HSE: can be selected for applications requiring timing precision.
  + RTC_CLOCK_SOURCE_LSE: can be selected for applications with low constraint on timing
                          precision.
  + RTC_CLOCK_SOURCE_LSI: can be selected for applications with low constraint on timing
                          precision.
  */
#define RTC_CLOCK_SOURCE_HSE                            /* 定义宏 */
/* #define RTC_CLOCK_SOURCE_LSE */
/* #define RTC_CLOCK_SOURCE_LSI */

#ifdef RTC_CLOCK_SOURCE_HSE                             /* 条件编译 */
  #define RTC_ASYNCH_PREDIV       99U                   /* 定义宏 */
  #define RTC_SYNCH_PREDIV        9U                    /* 定义宏 */
  #define RCC_RTCCLKSOURCE_1MHZ   ((uint32_t)((uint32_t)RCC_BDCR_RTCSEL | (uint32_t)((HSE_VALUE/1000000U) << 16U)))  /* 定义宏 */
#else /* RTC_CLOCK_SOURCE_LSE || RTC_CLOCK_SOURCE_LSI */
  #define RTC_ASYNCH_PREDIV       0U                    /* 定义宏 */
  #define RTC_SYNCH_PREDIV        31U                   /* 定义宏 */
#endif /* RTC_CLOCK_SOURCE_HSE */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef        hRTC_Handle;                   /* 执行语句 */
/* Private function prototypes -----------------------------------------------*/
void RTC_Alarm_IRQHandler(void);                        /* 中断服务函数 */
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  This function configures the RTC_ALARMA as a time base source. 
  *         The time source is configured  to have 1ms time base with a dedicated 
  *         Tick interrupt priority. 
  * @note   This function is called  automatically at the beginning of program after
  *         reset by HAL_Init() or at any time when clock is configured, by HAL_RCC_ClockConfig().
  * @param  TickPriority Tick interrupt priority.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)   /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  __IO uint32_t counter = 0U;                           /* 赋值或初始化 */

  RCC_OscInitTypeDef        RCC_OscInitStruct;          /* 时钟相关操作 */
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;        /* 时钟相关操作 */
  HAL_StatusTypeDef     status;                         /* HAL状态类型函数 */

#ifdef RTC_CLOCK_SOURCE_LSE                             /* 条件编译 */
  /* Configue LSE as RTC clock soucre */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;  /* 时钟相关操作 */
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;        /* 时钟相关操作 */
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;              /* 时钟相关操作 */
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;  /* 赋值或初始化 */
#elif defined (RTC_CLOCK_SOURCE_LSI)                    /* 条件编译否则如果 */
  /* Configue LSI as RTC clock soucre */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI;  /* 时钟相关操作 */
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;        /* 时钟相关操作 */
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;              /* 时钟相关操作 */
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;  /* 赋值或初始化 */
#elif defined (RTC_CLOCK_SOURCE_HSE)                    /* 条件编译否则如果 */
  /* Configue HSE as RTC clock soucre */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;  /* 时钟相关操作 */
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;        /* 时钟相关操作 */
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;              /* 时钟相关操作 */
  /* Ensure that RTC is clocked by 1MHz */
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_1MHZ;  /* 赋值或初始化 */
#else                                                   /* 条件编译否则 */
#error Please select the RTC Clock source               /* 编译错误指示 */
#endif /* RTC_CLOCK_SOURCE_LSE */

  status = HAL_RCC_OscConfig(&RCC_OscInitStruct);       /* 赋值或初始化 */
  if (status == HAL_OK)                                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;  /* 赋值或初始化 */
    status = HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  if (status == HAL_OK)                                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable RTC Clock */
    __HAL_RCC_RTC_ENABLE();                             /* HAL辅助宏/函数 */
    /* The time base should be 1ms
       Time base = ((RTC_ASYNCH_PREDIV + 1) * (RTC_SYNCH_PREDIV + 1)) / RTC_CLOCK
       HSE as RTC clock
         Time base = ((99 + 1) * (9 + 1)) / 1MHz
                   = 1ms
       LSE as RTC clock
         Time base = ((31 + 1) * (0 + 1)) / 32.768KHz
                   = ~1ms
       LSI as RTC clock
         Time base = ((31 + 1) * (0 + 1)) / 32KHz
                   = 1ms
    */
    hRTC_Handle.Instance = RTC;                         /* 赋值或初始化 */
    hRTC_Handle.Init.HourFormat = RTC_HOURFORMAT_24;    /* 赋值或初始化 */
    hRTC_Handle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;  /* 赋值或初始化 */
    hRTC_Handle.Init.SynchPrediv = RTC_SYNCH_PREDIV;    /* 赋值或初始化 */
    hRTC_Handle.Init.OutPut = RTC_OUTPUT_DISABLE;       /* 赋值或初始化 */
    hRTC_Handle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;  /* 赋值或初始化 */
    hRTC_Handle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;  /* 赋值或初始化 */
    status = HAL_RTC_Init(&hRTC_Handle);                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  if (status == HAL_OK)                                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable the write protection for RTC registers */
    __HAL_RTC_WRITEPROTECTION_DISABLE(&hRTC_Handle);    /* HAL辅助宏/函数 */

    /* Disable the Alarm A interrupt */
    __HAL_RTC_ALARMA_DISABLE(&hRTC_Handle);             /* HAL辅助宏/函数 */

    /* Clear flag alarm A */
    __HAL_RTC_ALARM_CLEAR_FLAG(&hRTC_Handle, RTC_FLAG_ALRAF);  /* HAL辅助宏/函数 */

    counter = 0U;                                       /* 赋值或初始化 */
    /* Wait till RTC ALRAWF flag is set and if Time out is reached exit */
    while (__HAL_RTC_ALARM_GET_FLAG(&hRTC_Handle, RTC_FLAG_ALRAWF) == RESET)  /* while循环 */
    {                                                   /* 代码块开始 */
      if (counter++ == (SystemCoreClock / 48U)) /* Timeout = ~ 1s */
      {                                                 /* 代码块开始 */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  if (status == HAL_OK)                                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    hRTC_Handle.Instance->ALRMAR = (uint32_t)0x01U;     /* 赋值或初始化 */

    /* Configure the Alarm state: Enable Alarm */
    __HAL_RTC_ALARMA_ENABLE(&hRTC_Handle);              /* HAL辅助宏/函数 */
    /* Configure the Alarm interrupt */
    __HAL_RTC_ALARM_ENABLE_IT(&hRTC_Handle, RTC_IT_ALRA);  /* HAL辅助宏/函数 */

    /* RTC Alarm Interrupt Configuration: EXTI configuration */
    __HAL_RTC_ALARM_EXTI_ENABLE_IT();                   /* HAL辅助宏/函数 */
    __HAL_RTC_ALARM_EXTI_ENABLE_RISING_EDGE();          /* HAL辅助宏/函数 */

    /* Check if the Initialization mode is set */
    if ((hRTC_Handle.Instance->ISR & RTC_ISR_INITF) == (uint32_t)RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the Initialization mode */
      hRTC_Handle.Instance->ISR = (uint32_t)RTC_INIT_MASK;  /* 赋值或初始化 */
      counter = 0U;                                     /* 赋值或初始化 */
      while ((hRTC_Handle.Instance->ISR & RTC_ISR_INITF) == (uint32_t)RESET)  /* while循环 */
      {                                                 /* 代码块开始 */
        if (counter++ == (SystemCoreClock / 48U)) /* Timeout = ~ 1s */
        {                                               /* 代码块开始 */
          status = HAL_ERROR;                           /* 赋值或初始化 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  if (status == HAL_OK)                                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    hRTC_Handle.Instance->DR = 0U;                      /* 赋值或初始化 */
    hRTC_Handle.Instance->TR = 0U;                      /* 赋值或初始化 */

    hRTC_Handle.Instance->ISR &= (uint32_t)~RTC_ISR_INIT;  /* 赋值或初始化 */

    /* Enable the write protection for RTC registers */
    __HAL_RTC_WRITEPROTECTION_ENABLE(&hRTC_Handle);     /* HAL辅助宏/函数 */

    /* Enable the RTC Alarm Interrupt */
    HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);                 /* HAL 中断配置 */

    /* Configure the SysTick IRQ priority */
    if (TickPriority < (1UL << __NVIC_PRIO_BITS))       /* 条件判断 */
    {                                                   /* 代码块开始 */
      HAL_NVIC_SetPriority(RTC_Alarm_IRQn, TickPriority, 0U);  /* HAL 中断配置 */
      uwTickPrio = TickPriority;                        /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      status = HAL_ERROR;                               /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

  }                                                     /* 代码块结束 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Suspend Tick increment.
  * @note   Disable the tick increment by disabling RTC ALARM interrupt.
  * @retval None
  */
void HAL_SuspendTick(void)                              /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Disable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_DISABLE(&hRTC_Handle);      /* HAL辅助宏/函数 */
  /* Disable RTC ALARM update Interrupt */
  __HAL_RTC_ALARM_DISABLE_IT(&hRTC_Handle, RTC_IT_ALRA);  /* HAL辅助宏/函数 */
  /* Enable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_ENABLE(&hRTC_Handle);       /* HAL辅助宏/函数 */
}                                                       /* 代码块结束 */

/**
  * @brief  Resume Tick increment.
  * @note   Enable the tick increment by Enabling RTC ALARM interrupt.
  * @retval None
  */
void HAL_ResumeTick(void)                               /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Disable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_DISABLE(&hRTC_Handle);      /* HAL辅助宏/函数 */
  /* Enable RTC ALARM Update interrupt */
  __HAL_RTC_ALARM_ENABLE_IT(&hRTC_Handle, RTC_IT_ALRA);  /* HAL辅助宏/函数 */
  /* Enable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_ENABLE(&hRTC_Handle);       /* HAL辅助宏/函数 */
}                                                       /* 代码块结束 */

/**
  * @brief  ALARM A Event Callback in non blocking mode
  * @note   This function is called  when RTC_ALARM interrupt took place, inside
  * RTC_ALARM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  hrtc  RTC handle
  * @retval None
  */
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  __IO uint32_t counter = 0U;                           /* 赋值或初始化 */

  HAL_IncTick();                                        /* HAL库函数调用 */

  __HAL_RTC_WRITEPROTECTION_DISABLE(hrtc);              /* HAL辅助宏/函数 */

  /* Set the Initialization mode */
  hrtc->Instance->ISR = (uint32_t)RTC_INIT_MASK;        /* 赋值或初始化 */

  while((hrtc->Instance->ISR & RTC_ISR_INITF) == (uint32_t)RESET)  /* while循环 */
  {                                                     /* 代码块开始 */
    if(counter++ == (SystemCoreClock /48U)) /* Timeout = ~ 1s */
    {                                                   /* 代码块开始 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  hrtc->Instance->DR = 0U;                              /* 赋值或初始化 */
  hrtc->Instance->TR = 0U;                              /* 赋值或初始化 */

  hrtc->Instance->ISR &= (uint32_t)~RTC_ISR_INIT;       /* 赋值或初始化 */

  /* Enable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);               /* HAL辅助宏/函数 */
}                                                       /* 代码块结束 */

/**
  * @brief  This function handles RTC ALARM interrupt request.
  * @retval None
  */
void RTC_Alarm_IRQHandler(void)                         /* 中断服务函数 */
{                                                       /* 代码块开始 */
  HAL_RTC_AlarmIRQHandler(&hRTC_Handle);                /* HAL库函数调用 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
