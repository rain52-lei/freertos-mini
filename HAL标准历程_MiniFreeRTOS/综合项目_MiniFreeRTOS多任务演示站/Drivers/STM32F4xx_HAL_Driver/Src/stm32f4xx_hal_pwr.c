/* 文件: stm32f4xx_hal_pwr.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_pwr.c
  * @author  MCD Application Team
  * @brief   PWR HAL module driver.
  *          This file provides firmware functions to manage the following 
  *          functionalities of the Power Controller (PWR) peripheral:
  *           + Initialization and de-initialization functions
  *           + Peripheral Control functions 
  *         
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

/** @defgroup PWR PWR
  * @brief PWR HAL module driver
  * @{
  */

#ifdef HAL_PWR_MODULE_ENABLED                           /* 条件编译 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @addtogroup PWR_Private_Constants
  * @{
  */

/** @defgroup PWR_PVD_Mode_Mask PWR PVD Mode Mask
  * @{
  */     
#define PVD_MODE_IT               0x00010000U           /* 定义宏 */
#define PVD_MODE_EVT              0x00020000U           /* 定义宏 */
#define PVD_RISING_EDGE           0x00000001U           /* 定义宏 */
#define PVD_FALLING_EDGE          0x00000002U           /* 定义宏 */
/**
  * @}
  */

/**
  * @}
  */    
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup PWR_Exported_Functions PWR Exported Functions
  * @{
  */

/** @defgroup PWR_Exported_Functions_Group1 Initialization and de-initialization functions 
  *  @brief    Initialization and de-initialization functions
  *
@verbatim
 ===============================================================================
              ##### Initialization and de-initialization functions #####
 ===============================================================================
    [..]
      After reset, the backup domain (RTC registers, RTC backup data 
      registers and backup SRAM) is protected against possible unwanted 
      write accesses. 
      To enable access to the RTC Domain and RTC registers, proceed as follows:
        (+) Enable the Power Controller (PWR) APB1 interface clock using the
            __HAL_RCC_PWR_CLK_ENABLE() macro.
        (+) Enable access to RTC domain using the HAL_PWR_EnableBkUpAccess() function.

@endverbatim
  * @{
  */

/**
  * @brief Deinitializes the HAL PWR peripheral registers to their default reset values.
  * @retval None
  */
void HAL_PWR_DeInit(void)                               /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  __HAL_RCC_PWR_FORCE_RESET();                          /* HAL辅助宏/函数 */
  __HAL_RCC_PWR_RELEASE_RESET();                        /* HAL辅助宏/函数 */
}                                                       /* 代码块结束 */

/**
  * @brief Enables access to the backup domain (RTC registers, RTC 
  *         backup data registers and backup SRAM).
  * @note If the HSE divided by 2, 3, ..31 is used as the RTC clock, the 
  *         Backup Domain Access should be kept enabled.
  * @note The following sequence is required to bypass the delay between
  *         DBP bit programming and the effective enabling  of the backup domain.
  *         Please check the Errata Sheet for more details under "Possible delay
  *         in backup domain protection disabling/enabling after programming the
  *         DBP bit" section.
  * @retval None
  */
void HAL_PWR_EnableBkUpAccess(void)                     /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  __IO uint32_t dummyread;                              /* 执行语句 */
  *(__IO uint32_t *) CR_DBP_BB = (uint32_t)ENABLE;      /* 赋值或初始化 */
  dummyread = PWR->CR;                                  /* 赋值或初始化 */
  UNUSED(dummyread);                                    /* 执行语句 */
}                                                       /* 代码块结束 */

/**
  * @brief Disables access to the backup domain (RTC registers, RTC 
  *         backup data registers and backup SRAM).
  * @note If the HSE divided by 2, 3, ..31 is used as the RTC clock, the 
  *         Backup Domain Access should be kept enabled.
  * @note The following sequence is required to bypass the delay between
  *         DBP bit programming and the effective disabling  of the backup domain.
  *         Please check the Errata Sheet for more details under "Possible delay
  *         in backup domain protection disabling/enabling after programming the
  *         DBP bit" section.
  * @retval None
  */
void HAL_PWR_DisableBkUpAccess(void)                    /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  __IO uint32_t dummyread;                              /* 执行语句 */
  *(__IO uint32_t *) CR_DBP_BB = (uint32_t)DISABLE;     /* 赋值或初始化 */
  dummyread = PWR->CR;                                  /* 赋值或初始化 */
  UNUSED(dummyread);                                    /* 执行语句 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup PWR_Exported_Functions_Group2 Peripheral Control functions 
  *  @brief Low Power modes configuration functions 
  *
@verbatim

 ===============================================================================
                 ##### Peripheral Control functions #####
 ===============================================================================

    *** PVD configuration ***
    =========================
    [..]
      (+) The PVD is used to monitor the VDD power supply by comparing it to a 
          threshold selected by the PVD Level (PLS[2:0] bits in the PWR_CR).
      (+) A PVDO flag is available to indicate if VDD/VDDA is higher or lower 
          than the PVD threshold. This event is internally connected to the EXTI 
          line16 and can generate an interrupt if enabled. This is done through
          __HAL_PWR_PVD_EXTI_ENABLE_IT() macro.
      (+) The PVD is stopped in Standby mode.

    *** Wake-up pin configuration ***
    ================================
    [..]
      (+) Wake-up pin is used to wake up the system from Standby mode. This pin is 
          forced in input pull-down configuration and is active on rising edges.
      (+) There is one Wake-up pin: Wake-up Pin 1 on PA.00.
	   (++) For STM32F446xx there are two Wake-Up pins: Pin1 on PA.00 and Pin2 on PC.13
           (++) For STM32F410xx/STM32F412xx/STM32F413xx/STM32F423xx  there are three Wake-Up pins: Pin1 on PA.00, Pin2 on PC.00 and Pin3 on PC.01 

    *** Low Power modes configuration ***
    =====================================
    [..]
      The devices feature 3 low-power modes:
      (+) Sleep mode: Cortex-M4 core stopped, peripherals kept running.
      (+) Stop mode: all clocks are stopped, regulator running, regulator 
          in low power mode
      (+) Standby mode: 1.2V domain powered off.

   *** Sleep mode ***
   ==================
    [..]
      (+) Entry:
        The Sleep mode is entered by using the HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI)
              functions with
          (++) PWR_SLEEPENTRY_WFI: enter SLEEP mode with WFI instruction
          (++) PWR_SLEEPENTRY_WFE: enter SLEEP mode with WFE instruction

      -@@- The Regulator parameter is not used for the STM32F4 family 
              and is kept as parameter just to maintain compatibility with the 
              lower power families (STM32L).
      (+) Exit:
        Any peripheral interrupt acknowledged by the nested vectored interrupt 
              controller (NVIC) can wake up the device from Sleep mode.

   *** Stop mode ***
   =================
    [..]
      In Stop mode, all clocks in the 1.2V domain are stopped, the PLL, the HSI,
      and the HSE RC oscillators are disabled. Internal SRAM and register contents 
      are preserved.
      The voltage regulator can be configured either in normal or low-power mode.
      To minimize the consumption In Stop mode, FLASH can be powered off before 
      entering the Stop mode using the HAL_PWREx_EnableFlashPowerDown() function.
      It can be switched on again by software after exiting the Stop mode using
      the HAL_PWREx_DisableFlashPowerDown() function. 

      (+) Entry:
         The Stop mode is entered using the HAL_PWR_EnterSTOPMode(PWR_MAINREGULATOR_ON) 
             function with:
          (++) Main regulator ON.
          (++) Low Power regulator ON.
      (+) Exit:
        Any EXTI Line (Internal or External) configured in Interrupt/Event mode.

   *** Standby mode ***
   ====================
    [..]
    (+)
      The Standby mode allows to achieve the lowest power consumption. It is based 
      on the Cortex-M4 deep sleep mode, with the voltage regulator disabled. 
      The 1.2V domain is consequently powered off. The PLL, the HSI oscillator and 
      the HSE oscillator are also switched off. SRAM and register contents are lost 
      except for the RTC registers, RTC backup registers, backup SRAM and Standby 
      circuitry.

      The voltage regulator is OFF.

      (++) Entry:
        (+++) The Standby mode is entered using the HAL_PWR_EnterSTANDBYMode() function.
      (++) Exit:
        (+++) WKUP pin rising edge, RTC alarm (Alarm A and Alarm B), RTC wake-up,
             tamper event, time-stamp event, external reset in NRST pin, IWDG reset.

   *** Auto-wake-up (AWU) from low-power mode ***
   =============================================
    [..]

     (+) The MCU can be woken up from low-power mode by an RTC Alarm event, an RTC 
      Wake-up event, a tamper event or a time-stamp event, without depending on 
      an external interrupt (Auto-wake-up mode).

      (+) RTC auto-wake-up (AWU) from the Stop and Standby modes

        (++) To wake up from the Stop mode with an RTC alarm event, it is necessary to 
              configure the RTC to generate the RTC alarm using the HAL_RTC_SetAlarm_IT() function.

        (++) To wake up from the Stop mode with an RTC Tamper or time stamp event, it 
             is necessary to configure the RTC to detect the tamper or time stamp event using the
                HAL_RTCEx_SetTimeStamp_IT() or HAL_RTCEx_SetTamper_IT() functions.

        (++) To wake up from the Stop mode with an RTC Wake-up event, it is necessary to
              configure the RTC to generate the RTC Wake-up event using the HAL_RTCEx_SetWakeUpTimer_IT() function.

@endverbatim
  * @{
  */

/**
  * @brief Configures the voltage threshold detected by the Power Voltage Detector(PVD).
  * @param sConfigPVD pointer to an PWR_PVDTypeDef structure that contains the configuration
  *        information for the PVD.
  * @note Refer to the electrical characteristics of your device datasheet for
  *         more details about the voltage threshold corresponding to each 
  *         detection level.
  * @retval None
  */
void HAL_PWR_ConfigPVD(PWR_PVDTypeDef *sConfigPVD)      /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_PWR_PVD_LEVEL(sConfigPVD->PVDLevel));  /* 执行语句 */
  assert_param(IS_PWR_PVD_MODE(sConfigPVD->Mode));      /* 执行语句 */

  /* Set PLS[7:5] bits according to PVDLevel value */
  MODIFY_REG(PWR->CR, PWR_CR_PLS, sConfigPVD->PVDLevel);  /* 执行语句 */

  /* Clear any previous config. Keep it clear if no event or IT mode is selected */
  __HAL_PWR_PVD_EXTI_DISABLE_EVENT();                   /* HAL辅助宏/函数 */
  __HAL_PWR_PVD_EXTI_DISABLE_IT();                      /* HAL辅助宏/函数 */
  __HAL_PWR_PVD_EXTI_DISABLE_RISING_EDGE();             /* HAL辅助宏/函数 */
  __HAL_PWR_PVD_EXTI_DISABLE_FALLING_EDGE();            /* HAL辅助宏/函数 */

  /* Configure interrupt mode */
  if((sConfigPVD->Mode & PVD_MODE_IT) == PVD_MODE_IT)   /* 条件判断 */
  {                                                     /* 代码块开始 */
    __HAL_PWR_PVD_EXTI_ENABLE_IT();                     /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Configure event mode */
  if((sConfigPVD->Mode & PVD_MODE_EVT) == PVD_MODE_EVT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    __HAL_PWR_PVD_EXTI_ENABLE_EVENT();                  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Configure the edge */
  if((sConfigPVD->Mode & PVD_RISING_EDGE) == PVD_RISING_EDGE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    __HAL_PWR_PVD_EXTI_ENABLE_RISING_EDGE();            /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  if((sConfigPVD->Mode & PVD_FALLING_EDGE) == PVD_FALLING_EDGE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    __HAL_PWR_PVD_EXTI_ENABLE_FALLING_EDGE();           /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief Enables the Power Voltage Detector(PVD).
  * @retval None
  */
void HAL_PWR_EnablePVD(void)                            /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  *(__IO uint32_t *) CR_PVDE_BB = (uint32_t)ENABLE;     /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief Disables the Power Voltage Detector(PVD).
  * @retval None
  */
void HAL_PWR_DisablePVD(void)                           /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  *(__IO uint32_t *) CR_PVDE_BB = (uint32_t)DISABLE;    /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief Enables the Wake-up PINx functionality.
  * @param WakeUpPinx Specifies the Power Wake-Up pin to enable.
  *         This parameter can be one of the following values:
  *           @arg PWR_WAKEUP_PIN1
  *           @arg PWR_WAKEUP_PIN2 available only on STM32F410xx/STM32F446xx/STM32F412xx/STM32F413xx/STM32F423xx devices
  *           @arg PWR_WAKEUP_PIN3 available only on STM32F410xx/STM32F412xx/STM32F413xx/STM32F423xx devices
  * @retval None
  */
void HAL_PWR_EnableWakeUpPin(uint32_t WakeUpPinx)       /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Check the parameter */
  assert_param(IS_PWR_WAKEUP_PIN(WakeUpPinx));          /* 执行语句 */

  /* Enable the wake up pin */
  SET_BIT(PWR->CSR, WakeUpPinx);                        /* 执行语句 */
}                                                       /* 代码块结束 */

/**
  * @brief Disables the Wake-up PINx functionality.
  * @param WakeUpPinx Specifies the Power Wake-Up pin to disable.
  *         This parameter can be one of the following values:
  *           @arg PWR_WAKEUP_PIN1
  *           @arg PWR_WAKEUP_PIN2 available only on STM32F410xx/STM32F446xx/STM32F412xx/STM32F413xx/STM32F423xx devices
  *           @arg PWR_WAKEUP_PIN3 available only on STM32F410xx/STM32F412xx/STM32F413xx/STM32F423xx devices
  * @retval None
  */
void HAL_PWR_DisableWakeUpPin(uint32_t WakeUpPinx)      /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Check the parameter */
  assert_param(IS_PWR_WAKEUP_PIN(WakeUpPinx));          /* 执行语句 */

  /* Disable the wake up pin */
  CLEAR_BIT(PWR->CSR, WakeUpPinx);                      /* 执行语句 */
}                                                       /* 代码块结束 */

/**
  * @brief Enters Sleep mode.
  *   
  * @note In Sleep mode, all I/O pins keep the same state as in Run mode.
  * 
  * @note In Sleep mode, the systick is stopped to avoid exit from this mode with
  *       systick interrupt when used as time base for Timeout 
  *                
  * @param Regulator Specifies the regulator state in SLEEP mode.
  *            This parameter can be one of the following values:
  *            @arg PWR_MAINREGULATOR_ON: SLEEP mode with regulator ON
  *            @arg PWR_LOWPOWERREGULATOR_ON: SLEEP mode with low power regulator ON
  * @note This parameter is not used for the STM32F4 family and is kept as parameter
  *       just to maintain compatibility with the lower power families.
  * @param SLEEPEntry Specifies if SLEEP mode in entered with WFI or WFE instruction.
  *          This parameter can be one of the following values:
  *            @arg PWR_SLEEPENTRY_WFI: enter SLEEP mode with WFI instruction
  *            @arg PWR_SLEEPENTRY_WFE: enter SLEEP mode with WFE instruction
  * @retval None
  */
void HAL_PWR_EnterSLEEPMode(uint32_t Regulator, uint8_t SLEEPEntry)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_PWR_REGULATOR(Regulator));            /* 执行语句 */
  assert_param(IS_PWR_SLEEP_ENTRY(SLEEPEntry));         /* 执行语句 */

  /* Clear SLEEPDEEP bit of Cortex System Control Register */
  CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));  /* 执行语句 */

  /* Select SLEEP mode entry -------------------------------------------------*/
  if(SLEEPEntry == PWR_SLEEPENTRY_WFI)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Request Wait For Interrupt */
    __WFI();                                            /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Request Wait For Event */
    __SEV();                                            /* 执行语句 */
    __WFE();                                            /* 执行语句 */
    __WFE();                                            /* 执行语句 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief Enters Stop mode. 
  * @note In Stop mode, all I/O pins keep the same state as in Run mode.
  * @note When exiting Stop mode by issuing an interrupt or a wake-up event, 
  *         the HSI RC oscillator is selected as system clock.
  * @note When the voltage regulator operates in low power mode, an additional 
  *         startup delay is incurred when waking up from Stop mode. 
  *         By keeping the internal regulator ON during Stop mode, the consumption 
  *         is higher although the startup time is reduced.    
  * @param Regulator Specifies the regulator state in Stop mode.
  *          This parameter can be one of the following values:
  *            @arg PWR_MAINREGULATOR_ON: Stop mode with regulator ON
  *            @arg PWR_LOWPOWERREGULATOR_ON: Stop mode with low power regulator ON
  * @param STOPEntry Specifies if Stop mode in entered with WFI or WFE instruction.
  *          This parameter can be one of the following values:
  *            @arg PWR_STOPENTRY_WFI: Enter Stop mode with WFI instruction
  *            @arg PWR_STOPENTRY_WFE: Enter Stop mode with WFE instruction
  * @retval None
  */
void HAL_PWR_EnterSTOPMode(uint32_t Regulator, uint8_t STOPEntry)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_PWR_REGULATOR(Regulator));            /* 执行语句 */
  assert_param(IS_PWR_STOP_ENTRY(STOPEntry));           /* 执行语句 */

  /* Select the regulator state in Stop mode: Set PDDS and LPDS bits according to PWR_Regulator value */
  MODIFY_REG(PWR->CR, (PWR_CR_PDDS | PWR_CR_LPDS), Regulator);  /* 执行语句 */

  /* Set SLEEPDEEP bit of Cortex System Control Register */
  SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));  /* 执行语句 */

  /* Select Stop mode entry --------------------------------------------------*/
  if(STOPEntry == PWR_STOPENTRY_WFI)                    /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Request Wait For Interrupt */
    __WFI();                                            /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Request Wait For Event */
    __SEV();                                            /* 执行语句 */
    __WFE();                                            /* 执行语句 */
    __WFE();                                            /* 执行语句 */
  }                                                     /* 代码块结束 */
  /* Reset SLEEPDEEP bit of Cortex System Control Register */
  CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));    /* 执行语句 */
}                                                       /* 代码块结束 */

/**
  * @brief Enters Standby mode.
  * @note In Standby mode, all I/O pins are high impedance except for:
  *          - Reset pad (still available) 
  *          - RTC_AF1 pin (PC13) if configured for tamper, time-stamp, RTC 
  *            Alarm out, or RTC clock calibration out.
  *          - RTC_AF2 pin (PI8) if configured for tamper or time-stamp.  
  *          - WKUP pin 1 (PA0) if enabled.       
  * @retval None
  */
void HAL_PWR_EnterSTANDBYMode(void)                     /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Select Standby mode */
  SET_BIT(PWR->CR, PWR_CR_PDDS);                        /* 执行语句 */

  /* Set SLEEPDEEP bit of Cortex System Control Register */
  SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));  /* 执行语句 */

  /* This option is used to ensure that store operations are completed */
#if defined ( __CC_ARM)                                 /* 条件编译 */
  __force_stores();                                     /* 执行语句 */
#endif                                                  /* 条件编译结束 */
  /* Request Wait For Interrupt */
  __WFI();                                              /* 执行语句 */
}                                                       /* 代码块结束 */

/**
  * @brief This function handles the PWR PVD interrupt request.
  * @note This API should be called under the PVD_IRQHandler().
  * @retval None
  */
void HAL_PWR_PVD_IRQHandler(void)                       /* 中断服务函数 */
{                                                       /* 代码块开始 */
  /* Check PWR Exti flag */
  if(__HAL_PWR_PVD_EXTI_GET_FLAG() != RESET)            /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* PWR PVD interrupt user callback */
    HAL_PWR_PVDCallback();                              /* HAL库函数调用 */

    /* Clear PWR Exti pending bit */
    __HAL_PWR_PVD_EXTI_CLEAR_FLAG();                    /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  PWR PVD interrupt callback
  * @retval None
  */
__weak void HAL_PWR_PVDCallback(void)                   /* 源代码 */
{                                                       /* 代码块开始 */
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_PWR_PVDCallback could be implemented in the user file
   */ 
}                                                       /* 代码块结束 */

/**
  * @brief Indicates Sleep-On-Exit when returning from Handler mode to Thread mode. 
  * @note Set SLEEPONEXIT bit of SCR register. When this bit is set, the processor 
  *       re-enters SLEEP mode when an interruption handling is over.
  *       Setting this bit is useful when the processor is expected to run only on
  *       interruptions handling.         
  * @retval None
  */
void HAL_PWR_EnableSleepOnExit(void)                    /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Set SLEEPONEXIT bit of Cortex System Control Register */
  SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPONEXIT_Msk));  /* 执行语句 */
}                                                       /* 代码块结束 */

/**
  * @brief Disables Sleep-On-Exit feature when returning from Handler mode to Thread mode. 
  * @note Clears SLEEPONEXIT bit of SCR register. When this bit is set, the processor 
  *       re-enters SLEEP mode when an interruption handling is over.          
  * @retval None
  */
void HAL_PWR_DisableSleepOnExit(void)                   /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Clear SLEEPONEXIT bit of Cortex System Control Register */
  CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPONEXIT_Msk));  /* 执行语句 */
}                                                       /* 代码块结束 */

/**
  * @brief Enables CORTEX M4 SEVONPEND bit. 
  * @note Sets SEVONPEND bit of SCR register. When this bit is set, this causes 
  *       WFE to wake up when an interrupt moves from inactive to pended.
  * @retval None
  */
void HAL_PWR_EnableSEVOnPend(void)                      /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Set SEVONPEND bit of Cortex System Control Register */
  SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SEVONPEND_Msk));  /* 执行语句 */
}                                                       /* 代码块结束 */

/**
  * @brief Disables CORTEX M4 SEVONPEND bit. 
  * @note Clears SEVONPEND bit of SCR register. When this bit is set, this causes 
  *       WFE to wake up when an interrupt moves from inactive to pended.         
  * @retval None
  */
void HAL_PWR_DisableSEVOnPend(void)                     /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Clear SEVONPEND bit of Cortex System Control Register */
  CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SEVONPEND_Msk));  /* 执行语句 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/**
  * @}
  */

#endif /* HAL_PWR_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
