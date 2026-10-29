/* 文件: stm32f4xx_it.h | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    Templates/Inc/stm32f4xx_it.h 
  * @author  MCD Application Team
  * @brief   This file contains the headers of the interrupt handlers.
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
#ifndef __STM32F4xx_IT_H                                /* 防止头文件重复包含 */
#define __STM32F4xx_IT_H                                /* 定义包含保护宏 */

#ifdef __cplusplus                                      /* 条件编译 */
 extern "C" {                                           /* 外部符号声明 */
#endif                                                  /* 条件编译结束 */

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void NMI_Handler(void);                                 /* 函数定义或声明 */
void HardFault_Handler(void);                           /* 函数定义或声明 */
void MemManage_Handler(void);                           /* 函数定义或声明 */
void BusFault_Handler(void);                            /* 函数定义或声明 */
void UsageFault_Handler(void);                          /* 函数定义或声明 */
void SVC_Handler(void);                                 /* 函数定义或声明 */
void DebugMon_Handler(void);                            /* 函数定义或声明 */
void PendSV_Handler(void);                              /* PendSV任务切换中断 */
void SysTick_Handler(void);                             /* SysTick中断服务 */

#ifdef __cplusplus                                      /* 条件编译 */
}                                                       /* 代码块结束 */
#endif                                                  /* 条件编译结束 */

#endif                                                  /* 条件编译结束 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
