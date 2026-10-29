/* 文件: stm32f4xx_ll_gpio.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_ll_gpio.c
  * @author  MCD Application Team
  * @brief   GPIO LL module driver.
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
#if defined(USE_FULL_LL_DRIVER)                         /* 条件编译 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_ll_gpio.h"                          /* 包含 STM32/HAL 头文件 */
#include "stm32f4xx_ll_bus.h"                           /* 包含 STM32/HAL 头文件 */
#ifdef  USE_FULL_ASSERT                                 /* 条件编译 */
#include "stm32_assert.h"                               /* 包含 STM32/HAL 头文件 */
#else                                                   /* 条件编译否则 */
#define assert_param(expr) ((void)0U)                   /* 定义宏 */
#endif                                                  /* 条件编译结束 */

/** @addtogroup STM32F4xx_LL_Driver
  * @{
  */

#if defined (GPIOA) || defined (GPIOB) || defined (GPIOC) || defined (GPIOD) || defined (GPIOE) || defined (GPIOF) || defined (GPIOG) || defined (GPIOH) || defined (GPIOI) || defined (GPIOJ) || defined (GPIOK)  /* 条件编译 */

/** @addtogroup GPIO_LL
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/** @addtogroup GPIO_LL_Private_Macros
  * @{
  */
#define IS_LL_GPIO_PIN(__VALUE__)          (((0x00000000U) < (__VALUE__)) && ((__VALUE__) <= (LL_GPIO_PIN_ALL)))  /* 定义宏 */

#define IS_LL_GPIO_MODE(__VALUE__)         (((__VALUE__) == LL_GPIO_MODE_INPUT)     ||\
                                            ((__VALUE__) == LL_GPIO_MODE_OUTPUT)    ||\
                                            ((__VALUE__) == LL_GPIO_MODE_ALTERNATE) ||\
                                            ((__VALUE__) == LL_GPIO_MODE_ANALOG))  /* 源代码 */

#define IS_LL_GPIO_OUTPUT_TYPE(__VALUE__)  (((__VALUE__) == LL_GPIO_OUTPUT_PUSHPULL)  ||\
                                            ((__VALUE__) == LL_GPIO_OUTPUT_OPENDRAIN))  /* 源代码 */

#define IS_LL_GPIO_SPEED(__VALUE__)        (((__VALUE__) == LL_GPIO_SPEED_FREQ_LOW)       ||\
                                            ((__VALUE__) == LL_GPIO_SPEED_FREQ_MEDIUM)    ||\
                                            ((__VALUE__) == LL_GPIO_SPEED_FREQ_HIGH)      ||\
                                            ((__VALUE__) == LL_GPIO_SPEED_FREQ_VERY_HIGH))  /* 源代码 */

#define IS_LL_GPIO_PULL(__VALUE__)         (((__VALUE__) == LL_GPIO_PULL_NO)   ||\
                                            ((__VALUE__) == LL_GPIO_PULL_UP)   ||\
                                            ((__VALUE__) == LL_GPIO_PULL_DOWN))  /* 源代码 */

#define IS_LL_GPIO_ALTERNATE(__VALUE__)    (((__VALUE__) == LL_GPIO_AF_0  )   ||\
                                            ((__VALUE__) == LL_GPIO_AF_1  )   ||\
                                            ((__VALUE__) == LL_GPIO_AF_2  )   ||\
                                            ((__VALUE__) == LL_GPIO_AF_3  )   ||\
                                            ((__VALUE__) == LL_GPIO_AF_4  )   ||\
                                            ((__VALUE__) == LL_GPIO_AF_5  )   ||\
                                            ((__VALUE__) == LL_GPIO_AF_6  )   ||\
                                            ((__VALUE__) == LL_GPIO_AF_7  )   ||\
                                            ((__VALUE__) == LL_GPIO_AF_8  )   ||\
                                            ((__VALUE__) == LL_GPIO_AF_9  )   ||\
                                            ((__VALUE__) == LL_GPIO_AF_10 )   ||\
                                            ((__VALUE__) == LL_GPIO_AF_11 )   ||\
                                            ((__VALUE__) == LL_GPIO_AF_12 )   ||\
                                            ((__VALUE__) == LL_GPIO_AF_13 )   ||\
                                            ((__VALUE__) == LL_GPIO_AF_14 )   ||\
                                            ((__VALUE__) == LL_GPIO_AF_15 ))  /* 源代码 */
/**
  * @}
  */

/* Private function prototypes -----------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/** @addtogroup GPIO_LL_Exported_Functions
  * @{
  */

/** @addtogroup GPIO_LL_EF_Init
  * @{
  */

/**
  * @brief  De-initialize GPIO registers (Registers restored to their default values).
  * @param  GPIOx GPIO Port
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: GPIO registers are de-initialized
  *          - ERROR:   Wrong GPIO Port
  */
ErrorStatus LL_GPIO_DeInit(GPIO_TypeDef *GPIOx)         /* 源代码 */
{                                                       /* 代码块开始 */
  ErrorStatus status = SUCCESS;                         /* 赋值或初始化 */

  /* Check the parameters */
  assert_param(IS_GPIO_ALL_INSTANCE(GPIOx));            /* 执行语句 */

  /* Force and Release reset on clock of GPIOx Port */
  if (GPIOx == GPIOA)                                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    LL_AHB1_GRP1_ForceReset(LL_AHB1_GRP1_PERIPH_GPIOA);  /* 执行语句 */
    LL_AHB1_GRP1_ReleaseReset(LL_AHB1_GRP1_PERIPH_GPIOA);  /* 执行语句 */
  }                                                     /* 代码块结束 */
  else if (GPIOx == GPIOB)                              /* 否则如果 */
  {                                                     /* 代码块开始 */
    LL_AHB1_GRP1_ForceReset(LL_AHB1_GRP1_PERIPH_GPIOB);  /* 执行语句 */
    LL_AHB1_GRP1_ReleaseReset(LL_AHB1_GRP1_PERIPH_GPIOB);  /* 执行语句 */
  }                                                     /* 代码块结束 */
  else if (GPIOx == GPIOC)                              /* 否则如果 */
  {                                                     /* 代码块开始 */
    LL_AHB1_GRP1_ForceReset(LL_AHB1_GRP1_PERIPH_GPIOC);  /* 执行语句 */
    LL_AHB1_GRP1_ReleaseReset(LL_AHB1_GRP1_PERIPH_GPIOC);  /* 执行语句 */
  }                                                     /* 代码块结束 */
#if defined(GPIOD)                                      /* 条件编译 */
  else if (GPIOx == GPIOD)                              /* 否则如果 */
  {                                                     /* 代码块开始 */
    LL_AHB1_GRP1_ForceReset(LL_AHB1_GRP1_PERIPH_GPIOD);  /* 执行语句 */
    LL_AHB1_GRP1_ReleaseReset(LL_AHB1_GRP1_PERIPH_GPIOD);  /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif /* GPIOD */
#if defined(GPIOE)                                      /* 条件编译 */
  else if (GPIOx == GPIOE)                              /* 否则如果 */
  {                                                     /* 代码块开始 */
    LL_AHB1_GRP1_ForceReset(LL_AHB1_GRP1_PERIPH_GPIOE);  /* 执行语句 */
    LL_AHB1_GRP1_ReleaseReset(LL_AHB1_GRP1_PERIPH_GPIOE);  /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif /* GPIOE */
#if defined(GPIOF)                                      /* 条件编译 */
  else if (GPIOx == GPIOF)                              /* 否则如果 */
  {                                                     /* 代码块开始 */
    LL_AHB1_GRP1_ForceReset(LL_AHB1_GRP1_PERIPH_GPIOF);  /* 执行语句 */
    LL_AHB1_GRP1_ReleaseReset(LL_AHB1_GRP1_PERIPH_GPIOF);  /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif /* GPIOF */
#if defined(GPIOG)                                      /* 条件编译 */
  else if (GPIOx == GPIOG)                              /* 否则如果 */
  {                                                     /* 代码块开始 */
    LL_AHB1_GRP1_ForceReset(LL_AHB1_GRP1_PERIPH_GPIOG);  /* 执行语句 */
    LL_AHB1_GRP1_ReleaseReset(LL_AHB1_GRP1_PERIPH_GPIOG);  /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif /* GPIOG */
#if defined(GPIOH)                                      /* 条件编译 */
  else if (GPIOx == GPIOH)                              /* 否则如果 */
  {                                                     /* 代码块开始 */
    LL_AHB1_GRP1_ForceReset(LL_AHB1_GRP1_PERIPH_GPIOH);  /* 执行语句 */
    LL_AHB1_GRP1_ReleaseReset(LL_AHB1_GRP1_PERIPH_GPIOH);  /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif /* GPIOH */
#if defined(GPIOI)                                      /* 条件编译 */
  else if (GPIOx == GPIOI)                              /* 否则如果 */
  {                                                     /* 代码块开始 */
    LL_AHB1_GRP1_ForceReset(LL_AHB1_GRP1_PERIPH_GPIOI);  /* 执行语句 */
    LL_AHB1_GRP1_ReleaseReset(LL_AHB1_GRP1_PERIPH_GPIOI);  /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif /* GPIOI */
#if defined(GPIOJ)                                      /* 条件编译 */
  else if (GPIOx == GPIOJ)                              /* 否则如果 */
  {                                                     /* 代码块开始 */
    LL_AHB1_GRP1_ForceReset(LL_AHB1_GRP1_PERIPH_GPIOJ);  /* 执行语句 */
    LL_AHB1_GRP1_ReleaseReset(LL_AHB1_GRP1_PERIPH_GPIOJ);  /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif /* GPIOJ */
#if defined(GPIOK)                                      /* 条件编译 */
  else if (GPIOx == GPIOK)                              /* 否则如果 */
  {                                                     /* 代码块开始 */
    LL_AHB1_GRP1_ForceReset(LL_AHB1_GRP1_PERIPH_GPIOK);  /* 执行语句 */
    LL_AHB1_GRP1_ReleaseReset(LL_AHB1_GRP1_PERIPH_GPIOK);  /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif /* GPIOK */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    status = ERROR;                                     /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return (status);                                      /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initialize GPIO registers according to the specified parameters in GPIO_InitStruct.
  * @param  GPIOx GPIO Port
  * @param  GPIO_InitStruct pointer to a @ref LL_GPIO_InitTypeDef structure
  *         that contains the configuration information for the specified GPIO peripheral.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: GPIO registers are initialized according to GPIO_InitStruct content
  *          - ERROR:   Not applicable
  */
ErrorStatus LL_GPIO_Init(GPIO_TypeDef *GPIOx, LL_GPIO_InitTypeDef *GPIO_InitStruct)  /* 源代码 */
{                                                       /* 代码块开始 */
  uint32_t pinpos     = 0x00000000U;                    /* 32位无符号变量/函数 */
  uint32_t currentpin = 0x00000000U;                    /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_GPIO_ALL_INSTANCE(GPIOx));            /* 执行语句 */
  assert_param(IS_LL_GPIO_PIN(GPIO_InitStruct->Pin));   /* 执行语句 */
  assert_param(IS_LL_GPIO_MODE(GPIO_InitStruct->Mode));  /* 执行语句 */
  assert_param(IS_LL_GPIO_PULL(GPIO_InitStruct->Pull));  /* 执行语句 */

  /* ------------------------- Configure the port pins ---------------- */
  /* Initialize  pinpos on first pin set */
  pinpos = POSITION_VAL(GPIO_InitStruct->Pin);          /* 赋值或初始化 */

  /* Configure the port pins */
  while (((GPIO_InitStruct->Pin) >> pinpos) != 0x00000000U)  /* while循环 */
  {                                                     /* 代码块开始 */
    /* Get current io position */
    currentpin = (GPIO_InitStruct->Pin) & (0x00000001U << pinpos);  /* 赋值或初始化 */

    if (currentpin)                                     /* 条件判断 */
    {                                                   /* 代码块开始 */

      if ((GPIO_InitStruct->Mode == LL_GPIO_MODE_OUTPUT) || (GPIO_InitStruct->Mode == LL_GPIO_MODE_ALTERNATE))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Check Speed mode parameters */
        assert_param(IS_LL_GPIO_SPEED(GPIO_InitStruct->Speed));  /* 执行语句 */

        /* Speed mode configuration */
        LL_GPIO_SetPinSpeed(GPIOx, currentpin, GPIO_InitStruct->Speed);  /* 执行语句 */

        /* Check Output mode parameters */
        assert_param(IS_LL_GPIO_OUTPUT_TYPE(GPIO_InitStruct->OutputType));  /* 执行语句 */

        /* Output mode configuration*/
        LL_GPIO_SetPinOutputType(GPIOx, currentpin, GPIO_InitStruct->OutputType);  /* 执行语句 */
      }                                                 /* 代码块结束 */

      /* Pull-up Pull down resistor configuration*/
      LL_GPIO_SetPinPull(GPIOx, currentpin, GPIO_InitStruct->Pull);  /* 执行语句 */

      if (GPIO_InitStruct->Mode == LL_GPIO_MODE_ALTERNATE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Check Alternate parameter */
        assert_param(IS_LL_GPIO_ALTERNATE(GPIO_InitStruct->Alternate));  /* 执行语句 */

        /* Speed mode configuration */
        if (POSITION_VAL(currentpin) < 0x00000008U)     /* 条件判断 */
        {                                               /* 代码块开始 */
          LL_GPIO_SetAFPin_0_7(GPIOx, currentpin, GPIO_InitStruct->Alternate);  /* 执行语句 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          LL_GPIO_SetAFPin_8_15(GPIOx, currentpin, GPIO_InitStruct->Alternate);  /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */

      /* Pin Mode configuration */
      LL_GPIO_SetPinMode(GPIOx, currentpin, GPIO_InitStruct->Mode);  /* 执行语句 */
    }                                                   /* 代码块结束 */
    pinpos++;                                           /* 执行语句 */
  }                                                     /* 代码块结束 */

  return (SUCCESS);                                     /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief Set each @ref LL_GPIO_InitTypeDef field to default value.
  * @param GPIO_InitStruct pointer to a @ref LL_GPIO_InitTypeDef structure
  *                          whose fields will be set to default values.
  * @retval None
  */

void LL_GPIO_StructInit(LL_GPIO_InitTypeDef *GPIO_InitStruct)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Reset GPIO init structure parameters values */
  GPIO_InitStruct->Pin        = LL_GPIO_PIN_ALL;        /* GPIO相关操作 */
  GPIO_InitStruct->Mode       = LL_GPIO_MODE_ANALOG;    /* GPIO相关操作 */
  GPIO_InitStruct->Speed      = LL_GPIO_SPEED_FREQ_LOW;  /* GPIO相关操作 */
  GPIO_InitStruct->OutputType = LL_GPIO_OUTPUT_PUSHPULL;  /* GPIO相关操作 */
  GPIO_InitStruct->Pull       = LL_GPIO_PULL_NO;        /* GPIO相关操作 */
  GPIO_InitStruct->Alternate  = LL_GPIO_AF_0;           /* GPIO相关操作 */
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

#endif /* defined (GPIOA) || defined (GPIOB) || defined (GPIOC) || defined (GPIOD) || defined (GPIOE) || defined (GPIOF) || defined (GPIOG) || defined (GPIOH) || defined (GPIOI) || defined (GPIOJ) || defined (GPIOK) */

/**
  * @}
  */

#endif /* USE_FULL_LL_DRIVER */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
