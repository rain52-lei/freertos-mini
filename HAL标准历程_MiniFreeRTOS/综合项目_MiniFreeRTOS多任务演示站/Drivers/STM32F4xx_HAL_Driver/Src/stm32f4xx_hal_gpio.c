/* 文件: stm32f4xx_hal_gpio.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_gpio.c
  * @author  MCD Application Team
  * @brief   GPIO HAL module driver.
  *          This file provides firmware functions to manage the following 
  *          functionalities of the General Purpose Input/Output (GPIO) peripheral:
  *           + Initialization and de-initialization functions
  *           + IO operation functions
  *
  @verbatim
  ==============================================================================
                    ##### GPIO Peripheral features #####
  ==============================================================================
  [..] 
  Subject to the specific hardware characteristics of each I/O port listed in the datasheet, each
  port bit of the General Purpose IO (GPIO) Ports, can be individually configured by software
  in several modes:
  (+) Input mode 
  (+) Analog mode
  (+) Output mode
  (+) Alternate function mode
  (+) External interrupt/event lines

  [..]  
  During and just after reset, the alternate functions and external interrupt  
  lines are not active and the I/O ports are configured in input floating mode.

  [..]   
  All GPIO pins have weak internal pull-up and pull-down resistors, which can be 
  activated or not.

  [..]
  In Output or Alternate mode, each IO can be configured on open-drain or push-pull
  type and the IO speed can be selected depending on the VDD value.

  [..]  
  All ports have external interrupt/event capability. To use external interrupt 
  lines, the port must be configured in input mode. All available GPIO pins are 
  connected to the 16 external interrupt/event lines from EXTI0 to EXTI15.

  [..]
  The external interrupt/event controller consists of up to 23 edge detectors 
  (16 lines are connected to GPIO) for generating event/interrupt requests (each 
  input line can be independently configured to select the type (interrupt or event) 
  and the corresponding trigger event (rising or falling or both). Each line can 
  also be masked independently. 

                     ##### How to use this driver #####
  ==============================================================================  
  [..]
    (#) Enable the GPIO AHB clock using the following function: __HAL_RCC_GPIOx_CLK_ENABLE(). 

    (#) Configure the GPIO pin(s) using HAL_GPIO_Init().
        (++) Configure the IO mode using "Mode" member from GPIO_InitTypeDef structure
        (++) Activate Pull-up, Pull-down resistor using "Pull" member from GPIO_InitTypeDef 
             structure.
        (++) In case of Output or alternate function mode selection: the speed is 
             configured through "Speed" member from GPIO_InitTypeDef structure.
        (++) In alternate mode is selection, the alternate function connected to the IO
             is configured through "Alternate" member from GPIO_InitTypeDef structure.
        (++) Analog mode is required when a pin is to be used as ADC channel 
             or DAC output.
        (++) In case of external interrupt/event selection the "Mode" member from 
             GPIO_InitTypeDef structure select the type (interrupt or event) and 
             the corresponding trigger event (rising or falling or both).

    (#) In case of external interrupt/event mode selection, configure NVIC IRQ priority 
        mapped to the EXTI line using HAL_NVIC_SetPriority() and enable it using
        HAL_NVIC_EnableIRQ().

    (#) To get the level of a pin configured in input mode use HAL_GPIO_ReadPin().

    (#) To set/reset the level of a pin configured in output mode use 
        HAL_GPIO_WritePin()/HAL_GPIO_TogglePin().

    (#) To lock pin configuration until next reset use HAL_GPIO_LockPin().


    (#) During and just after reset, the alternate functions are not 
        active and the GPIO pins are configured in input floating mode (except JTAG
        pins).

    (#) The LSE oscillator pins OSC32_IN and OSC32_OUT can be used as general purpose 
        (PC14 and PC15, respectively) when the LSE oscillator is off. The LSE has 
        priority over the GPIO function.

    (#) The HSE oscillator pins OSC_IN/OSC_OUT can be used as 
        general purpose PH0 and PH1, respectively, when the HSE oscillator is off. 
        The HSE has priority over the GPIO function.

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

/** @defgroup GPIO GPIO
  * @brief GPIO HAL module driver
  * @{
  */

#ifdef HAL_GPIO_MODULE_ENABLED                          /* 条件编译 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @addtogroup GPIO_Private_Constants GPIO Private Constants
  * @{
  */

#define GPIO_NUMBER           16U                       /* 定义宏 */
/**
  * @}
  */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/** @defgroup GPIO_Exported_Functions GPIO Exported Functions
  * @{
  */

/** @defgroup GPIO_Exported_Functions_Group1 Initialization and de-initialization functions
  *  @brief    Initialization and Configuration functions
  *
@verbatim    
 ===============================================================================
              ##### Initialization and de-initialization functions #####
 ===============================================================================
  [..]
    This section provides functions allowing to initialize and de-initialize the GPIOs
    to be ready for use.

@endverbatim
  * @{
  */


/**
  * @brief  Initializes the GPIOx peripheral according to the specified parameters in the GPIO_Init.
  * @param  GPIOx where x can be (A..K) to select the GPIO peripheral for STM32F429X device or
  *                      x can be (A..I) to select the GPIO peripheral for STM32F40XX and STM32F427X devices.
  * @param  GPIO_Init pointer to a GPIO_InitTypeDef structure that contains
  *         the configuration information for the specified GPIO peripheral.
  * @retval None
  */
void HAL_GPIO_Init(GPIO_TypeDef  *GPIOx, GPIO_InitTypeDef *GPIO_Init)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  uint32_t position;                                    /* 32位无符号变量/函数 */
  uint32_t ioposition = 0x00U;                          /* 32位无符号变量/函数 */
  uint32_t iocurrent = 0x00U;                           /* 32位无符号变量/函数 */
  uint32_t temp = 0x00U;                                /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_GPIO_ALL_INSTANCE(GPIOx));            /* 执行语句 */
  assert_param(IS_GPIO_PIN(GPIO_Init->Pin));            /* 执行语句 */
  assert_param(IS_GPIO_MODE(GPIO_Init->Mode));          /* 执行语句 */
  assert_param(IS_GPIO_PULL(GPIO_Init->Pull));          /* 执行语句 */

  /* Configure the port pins */
  for(position = 0U; position < GPIO_NUMBER; position++)  /* for循环 */
  {                                                     /* 代码块开始 */
    /* Get the IO position */
    ioposition = 0x01U << position;                     /* 赋值或初始化 */
    /* Get the current IO position */
    iocurrent = (uint32_t)(GPIO_Init->Pin) & ioposition;  /* 赋值或初始化 */

    if(iocurrent == ioposition)                         /* 条件判断 */
    {                                                   /* 代码块开始 */
      /*--------------------- GPIO Mode Configuration ------------------------*/
      /* In case of Output or Alternate function mode selection */
      if(((GPIO_Init->Mode & GPIO_MODE) == MODE_OUTPUT) || \
          (GPIO_Init->Mode & GPIO_MODE) == MODE_AF)     /* 源代码 */
      {                                                 /* 代码块开始 */
        /* Check the Speed parameter */
        assert_param(IS_GPIO_SPEED(GPIO_Init->Speed));  /* 执行语句 */
        /* Configure the IO Speed */
        temp = GPIOx->OSPEEDR;                          /* 赋值或初始化 */
        temp &= ~(GPIO_OSPEEDER_OSPEEDR0 << (position * 2U));  /* 赋值或初始化 */
        temp |= (GPIO_Init->Speed << (position * 2U));  /* 赋值或初始化 */
        GPIOx->OSPEEDR = temp;                          /* 赋值或初始化 */

        /* Configure the IO Output Type */
        temp = GPIOx->OTYPER;                           /* 赋值或初始化 */
        temp &= ~(GPIO_OTYPER_OT_0 << position) ;       /* 赋值或初始化 */
        temp |= (((GPIO_Init->Mode & GPIO_OUTPUT_TYPE) >> 4U) << position);  /* 赋值或初始化 */
        GPIOx->OTYPER = temp;                           /* 赋值或初始化 */
       }                                                /* 代码块结束 */

      if((GPIO_Init->Mode & GPIO_MODE) != MODE_ANALOG)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Activate the Pull-up or Pull down resistor for the current IO */
        temp = GPIOx->PUPDR;                            /* 赋值或初始化 */
        temp &= ~(GPIO_PUPDR_PUPDR0 << (position * 2U));  /* 赋值或初始化 */
        temp |= ((GPIO_Init->Pull) << (position * 2U));  /* 赋值或初始化 */
        GPIOx->PUPDR = temp;                            /* 赋值或初始化 */
      }                                                 /* 代码块结束 */

      /* In case of Alternate function mode selection */
      if((GPIO_Init->Mode & GPIO_MODE) == MODE_AF)      /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Check the Alternate function parameter */
        assert_param(IS_GPIO_AF(GPIO_Init->Alternate));  /* 执行语句 */
        /* Configure Alternate function mapped with the current IO */
        temp = GPIOx->AFR[position >> 3U];              /* 赋值或初始化 */
        temp &= ~(0xFU << ((uint32_t)(position & 0x07U) * 4U)) ;  /* 赋值或初始化 */
        temp |= ((uint32_t)(GPIO_Init->Alternate) << (((uint32_t)position & 0x07U) * 4U));  /* 赋值或初始化 */
        GPIOx->AFR[position >> 3U] = temp;              /* 赋值或初始化 */
      }                                                 /* 代码块结束 */

      /* Configure IO Direction mode (Input, Output, Alternate or Analog) */
      temp = GPIOx->MODER;                              /* 赋值或初始化 */
      temp &= ~(GPIO_MODER_MODER0 << (position * 2U));  /* 赋值或初始化 */
      temp |= ((GPIO_Init->Mode & GPIO_MODE) << (position * 2U));  /* 赋值或初始化 */
      GPIOx->MODER = temp;                              /* 赋值或初始化 */

      /*--------------------- EXTI Mode Configuration ------------------------*/
      /* Configure the External Interrupt or event for the current IO */
      if((GPIO_Init->Mode & EXTI_MODE) == EXTI_MODE)    /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Enable SYSCFG Clock */
        __HAL_RCC_SYSCFG_CLK_ENABLE();                  /* HAL辅助宏/函数 */

        temp = SYSCFG->EXTICR[position >> 2U];          /* 赋值或初始化 */
        temp &= ~(0x0FU << (4U * (position & 0x03U)));  /* 赋值或初始化 */
        temp |= ((uint32_t)(GPIO_GET_INDEX(GPIOx)) << (4U * (position & 0x03U)));  /* 赋值或初始化 */
        SYSCFG->EXTICR[position >> 2U] = temp;          /* 赋值或初始化 */

        /* Clear EXTI line configuration */
        temp = EXTI->IMR;                               /* 赋值或初始化 */
        temp &= ~((uint32_t)iocurrent);                 /* 赋值或初始化 */
        if((GPIO_Init->Mode & GPIO_MODE_IT) == GPIO_MODE_IT)  /* 条件判断 */
        {                                               /* 代码块开始 */
          temp |= iocurrent;                            /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        EXTI->IMR = temp;                               /* 赋值或初始化 */

        temp = EXTI->EMR;                               /* 赋值或初始化 */
        temp &= ~((uint32_t)iocurrent);                 /* 赋值或初始化 */
        if((GPIO_Init->Mode & GPIO_MODE_EVT) == GPIO_MODE_EVT)  /* 条件判断 */
        {                                               /* 代码块开始 */
          temp |= iocurrent;                            /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        EXTI->EMR = temp;                               /* 赋值或初始化 */

        /* Clear Rising Falling edge configuration */
        temp = EXTI->RTSR;                              /* 赋值或初始化 */
        temp &= ~((uint32_t)iocurrent);                 /* 赋值或初始化 */
        if((GPIO_Init->Mode & RISING_EDGE) == RISING_EDGE)  /* 条件判断 */
        {                                               /* 代码块开始 */
          temp |= iocurrent;                            /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        EXTI->RTSR = temp;                              /* 赋值或初始化 */

        temp = EXTI->FTSR;                              /* 赋值或初始化 */
        temp &= ~((uint32_t)iocurrent);                 /* 赋值或初始化 */
        if((GPIO_Init->Mode & FALLING_EDGE) == FALLING_EDGE)  /* 条件判断 */
        {                                               /* 代码块开始 */
          temp |= iocurrent;                            /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        EXTI->FTSR = temp;                              /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  De-initializes the GPIOx peripheral registers to their default reset values.
  * @param  GPIOx where x can be (A..K) to select the GPIO peripheral for STM32F429X device or
  *                      x can be (A..I) to select the GPIO peripheral for STM32F40XX and STM32F427X devices.
  * @param  GPIO_Pin specifies the port bit to be written.
  *          This parameter can be one of GPIO_PIN_x where x can be (0..15).
  * @retval None
  */
void HAL_GPIO_DeInit(GPIO_TypeDef  *GPIOx, uint32_t GPIO_Pin)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  uint32_t position;                                    /* 32位无符号变量/函数 */
  uint32_t ioposition = 0x00U;                          /* 32位无符号变量/函数 */
  uint32_t iocurrent = 0x00U;                           /* 32位无符号变量/函数 */
  uint32_t tmp = 0x00U;                                 /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_GPIO_ALL_INSTANCE(GPIOx));            /* 执行语句 */

  /* Configure the port pins */
  for(position = 0U; position < GPIO_NUMBER; position++)  /* for循环 */
  {                                                     /* 代码块开始 */
    /* Get the IO position */
    ioposition = 0x01U << position;                     /* 赋值或初始化 */
    /* Get the current IO position */
    iocurrent = (GPIO_Pin) & ioposition;                /* 赋值或初始化 */

    if(iocurrent == ioposition)                         /* 条件判断 */
    {                                                   /* 代码块开始 */
      /*------------------------- EXTI Mode Configuration --------------------*/
      tmp = SYSCFG->EXTICR[position >> 2U];             /* 赋值或初始化 */
      tmp &= (0x0FU << (4U * (position & 0x03U)));      /* 赋值或初始化 */
      if(tmp == ((uint32_t)(GPIO_GET_INDEX(GPIOx)) << (4U * (position & 0x03U))))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Clear EXTI line configuration */
        EXTI->IMR &= ~((uint32_t)iocurrent);            /* 赋值或初始化 */
        EXTI->EMR &= ~((uint32_t)iocurrent);            /* 赋值或初始化 */

        /* Clear Rising Falling edge configuration */
        EXTI->RTSR &= ~((uint32_t)iocurrent);           /* 赋值或初始化 */
        EXTI->FTSR &= ~((uint32_t)iocurrent);           /* 赋值或初始化 */

        /* Configure the External Interrupt or event for the current IO */
        tmp = 0x0FU << (4U * (position & 0x03U));       /* 赋值或初始化 */
        SYSCFG->EXTICR[position >> 2U] &= ~tmp;         /* 赋值或初始化 */
      }                                                 /* 代码块结束 */

      /*------------------------- GPIO Mode Configuration --------------------*/
      /* Configure IO Direction in Input Floating Mode */
      GPIOx->MODER &= ~(GPIO_MODER_MODER0 << (position * 2U));  /* 赋值或初始化 */

      /* Configure the default Alternate Function in current IO */
      GPIOx->AFR[position >> 3U] &= ~(0xFU << ((uint32_t)(position & 0x07U) * 4U)) ;  /* 赋值或初始化 */

      /* Deactivate the Pull-up and Pull-down resistor for the current IO */
      GPIOx->PUPDR &= ~(GPIO_PUPDR_PUPDR0 << (position * 2U));  /* 赋值或初始化 */

      /* Configure the default value IO Output Type */
      GPIOx->OTYPER  &= ~(GPIO_OTYPER_OT_0 << position) ;  /* 赋值或初始化 */

      /* Configure the default value for IO Speed */
      GPIOx->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR0 << (position * 2U));  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup GPIO_Exported_Functions_Group2 IO operation functions 
  *  @brief   GPIO Read and Write
  *
@verbatim
 ===============================================================================
                       ##### IO operation functions #####
 ===============================================================================

@endverbatim
  * @{
  */

/**
  * @brief  Reads the specified input port pin.
  * @param  GPIOx where x can be (A..K) to select the GPIO peripheral for STM32F429X device or
  *                      x can be (A..I) to select the GPIO peripheral for STM32F40XX and STM32F427X devices.
  * @param  GPIO_Pin specifies the port bit to read.
  *         This parameter can be GPIO_PIN_x where x can be (0..15).
  * @retval The input port pin value.
  */
GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)  /* GPIO相关操作 */
{                                                       /* 代码块开始 */
  GPIO_PinState bitstatus;                              /* GPIO相关操作 */

  /* Check the parameters */
  assert_param(IS_GPIO_PIN(GPIO_Pin));                  /* 执行语句 */

  if((GPIOx->IDR & GPIO_Pin) != (uint32_t)GPIO_PIN_RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    bitstatus = GPIO_PIN_SET;                           /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    bitstatus = GPIO_PIN_RESET;                         /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  return bitstatus;                                     /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Sets or clears the selected data port bit.
  *
  * @note   This function uses GPIOx_BSRR register to allow atomic read/modify
  *         accesses. In this way, there is no risk of an IRQ occurring between
  *         the read and the modify access.
  *
  * @param  GPIOx where x can be (A..K) to select the GPIO peripheral for STM32F429X device or
  *                      x can be (A..I) to select the GPIO peripheral for STM32F40XX and STM32F427X devices.
  * @param  GPIO_Pin specifies the port bit to be written.
  *          This parameter can be one of GPIO_PIN_x where x can be (0..15).
  * @param  PinState specifies the value to be written to the selected bit.
  *          This parameter can be one of the GPIO_PinState enum values:
  *            @arg GPIO_PIN_RESET: to clear the port pin
  *            @arg GPIO_PIN_SET: to set the port pin
  * @retval None
  */
void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_GPIO_PIN(GPIO_Pin));                  /* 执行语句 */
  assert_param(IS_GPIO_PIN_ACTION(PinState));           /* 执行语句 */

  if(PinState != GPIO_PIN_RESET)                        /* 条件判断 */
  {                                                     /* 代码块开始 */
    GPIOx->BSRR = GPIO_Pin;                             /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    GPIOx->BSRR = (uint32_t)GPIO_Pin << 16U;            /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Toggles the specified GPIO pins.
  * @param  GPIOx Where x can be (A..K) to select the GPIO peripheral for STM32F429X device or
  *                      x can be (A..I) to select the GPIO peripheral for STM32F40XX and STM32F427X devices.
  * @param  GPIO_Pin Specifies the pins to be toggled.
  * @retval None
  */
void HAL_GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  uint32_t odr;                                         /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_GPIO_PIN(GPIO_Pin));                  /* 执行语句 */

  /* get current Ouput Data Register value */
  odr = GPIOx->ODR;                                     /* 赋值或初始化 */

  /* Set selected pins that were at low level, and reset ones that were high */
  GPIOx->BSRR = ((odr & GPIO_Pin) << GPIO_NUMBER) | (~odr & GPIO_Pin);  /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  Locks GPIO Pins configuration registers.
  * @note   The locked registers are GPIOx_MODER, GPIOx_OTYPER, GPIOx_OSPEEDR,
  *         GPIOx_PUPDR, GPIOx_AFRL and GPIOx_AFRH.
  * @note   The configuration of the locked GPIO pins can no longer be modified
  *         until the next reset.
  * @param  GPIOx where x can be (A..F) to select the GPIO peripheral for STM32F4 family
  * @param  GPIO_Pin specifies the port bit to be locked.
  *         This parameter can be any combination of GPIO_PIN_x where x can be (0..15).
  * @retval None
  */
HAL_StatusTypeDef HAL_GPIO_LockPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  __IO uint32_t tmp = GPIO_LCKR_LCKK;                   /* 赋值或初始化 */

  /* Check the parameters */
  assert_param(IS_GPIO_PIN(GPIO_Pin));                  /* 执行语句 */

  /* Apply lock key write sequence */
  tmp |= GPIO_Pin;                                      /* 赋值或初始化 */
  /* Set LCKx bit(s): LCKK='1' + LCK[15-0] */
  GPIOx->LCKR = tmp;                                    /* 赋值或初始化 */
  /* Reset LCKx bit(s): LCKK='0' + LCK[15-0] */
  GPIOx->LCKR = GPIO_Pin;                               /* 赋值或初始化 */
  /* Set LCKx bit(s): LCKK='1' + LCK[15-0] */
  GPIOx->LCKR = tmp;                                    /* 赋值或初始化 */
  /* Read LCKR register. This read is mandatory to complete key lock sequence */
  tmp = GPIOx->LCKR;                                    /* 赋值或初始化 */

  /* Read again in order to confirm lock is active */
 if((GPIOx->LCKR & GPIO_LCKR_LCKK) != RESET)            /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  This function handles EXTI interrupt request.
  * @param  GPIO_Pin Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_IRQHandler(uint16_t GPIO_Pin)        /* 中断服务函数 */
{                                                       /* 代码块开始 */
  /* EXTI line interrupt detected */
  if(__HAL_GPIO_EXTI_GET_IT(GPIO_Pin) != RESET)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);                 /* HAL辅助宏/函数 */
    HAL_GPIO_EXTI_Callback(GPIO_Pin);                   /* HAL GPIO操作 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin Specifies the pins connected EXTI line
  * @retval None
  */
__weak void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)   /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(GPIO_Pin);                                     /* 执行语句 */
  /* NOTE: This function Should not be modified, when the callback is needed,
           the HAL_GPIO_EXTI_Callback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @}
  */


/**
  * @}
  */

#endif /* HAL_GPIO_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
