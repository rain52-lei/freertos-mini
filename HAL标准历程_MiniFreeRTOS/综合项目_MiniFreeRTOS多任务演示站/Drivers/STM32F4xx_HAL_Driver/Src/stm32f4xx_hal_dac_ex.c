/* 文件: stm32f4xx_hal_dac_ex.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_dac_ex.c
  * @author  MCD Application Team
  * @brief   Extended DAC HAL module driver.
  *          This file provides firmware functions to manage the extended
  *          functionalities of the DAC peripheral.
  *
  *
  @verbatim
  ==============================================================================
                      ##### How to use this driver #####
  ==============================================================================
    [..]

     *** Dual mode IO operation ***
     ==============================
     [..]
      (+) When Dual mode is enabled (i.e. DAC Channel1 and Channel2 are used simultaneously) :
          Use HAL_DACEx_DualGetValue() to get digital data to be converted and use
          HAL_DACEx_DualSetValue() to set digital value to converted simultaneously in
          Channel 1 and Channel 2.

     *** Signal generation operation ***
     ===================================
     [..]
      (+) Use HAL_DACEx_TriangleWaveGenerate() to generate Triangle signal.
      (+) Use HAL_DACEx_NoiseWaveGenerate() to generate Noise signal.

 @endverbatim
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


/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"                              /* 包含 STM32/HAL 头文件 */

/** @addtogroup STM32F4xx_HAL_Driver
  * @{
  */

#ifdef HAL_DAC_MODULE_ENABLED                           /* 条件编译 */

#if defined(DAC)                                        /* 条件编译 */

/** @defgroup DACEx DACEx
  * @brief DAC Extended HAL module driver
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/** @defgroup DACEx_Exported_Functions DACEx Exported Functions
  * @{
  */

/** @defgroup DACEx_Exported_Functions_Group2 IO operation functions
  *  @brief    Extended IO operation functions
  *
@verbatim
  ==============================================================================
                 ##### Extended features functions #####
  ==============================================================================
    [..]  This section provides functions allowing to:
      (+) Start conversion.
      (+) Stop conversion.
      (+) Start conversion and enable DMA transfer.
      (+) Stop conversion and disable DMA transfer.
      (+) Get result of conversion.
      (+) Get result of dual mode conversion.

@endverbatim
  * @{
  */

#if defined(DAC_CHANNEL2_SUPPORT)                       /* 条件编译 */
/**
  * @brief  Enables DAC and starts conversion of both channels.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DACEx_DualStart(DAC_HandleTypeDef *hdac)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmp_swtrig = 0UL;                            /* 32位无符号变量/函数 */


  /* Process locked */
  __HAL_LOCK(hdac);                                     /* HAL辅助宏/函数 */

  /* Change DAC state */
  hdac->State = HAL_DAC_STATE_BUSY;                     /* 赋值或初始化 */

  /* Enable the Peripheral */
  __HAL_DAC_ENABLE(hdac, DAC_CHANNEL_1);                /* HAL辅助宏/函数 */
  __HAL_DAC_ENABLE(hdac, DAC_CHANNEL_2);                /* HAL辅助宏/函数 */

  /* Check if software trigger enabled */
  if ((hdac->Instance->CR & (DAC_CR_TEN1 | DAC_CR_TSEL1)) == DAC_TRIGGER_SOFTWARE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    tmp_swtrig |= DAC_SWTRIGR_SWTRIG1;                  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  if ((hdac->Instance->CR & (DAC_CR_TEN2 | DAC_CR_TSEL2)) == (DAC_TRIGGER_SOFTWARE << (DAC_CHANNEL_2 & 0x10UL)))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    tmp_swtrig |= DAC_SWTRIGR_SWTRIG2;                  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  /* Enable the selected DAC software conversion*/
  SET_BIT(hdac->Instance->SWTRIGR, tmp_swtrig);         /* 执行语句 */

  /* Change DAC state */
  hdac->State = HAL_DAC_STATE_READY;                    /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hdac);                                   /* HAL辅助宏/函数 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Disables DAC and stop conversion of both channels.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DACEx_DualStop(DAC_HandleTypeDef *hdac)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */

  /* Disable the Peripheral */
  __HAL_DAC_DISABLE(hdac, DAC_CHANNEL_1);               /* HAL辅助宏/函数 */
  __HAL_DAC_DISABLE(hdac, DAC_CHANNEL_2);               /* HAL辅助宏/函数 */

  /* Change DAC state */
  hdac->State = HAL_DAC_STATE_READY;                    /* 赋值或初始化 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

#endif /* DAC_CHANNEL2_SUPPORT */

/**
  * @brief  Enable or disable the selected DAC channel wave generation.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @param  Channel The selected DAC channel.
  *          This parameter can be one of the following values:
  *            @arg DAC_CHANNEL_1: DAC Channel1 selected
  *            @arg DAC_CHANNEL_2: DAC Channel2 selected
  * @param  Amplitude Select max triangle amplitude.
  *          This parameter can be one of the following values:
  *            @arg DAC_TRIANGLEAMPLITUDE_1: Select max triangle amplitude of 1
  *            @arg DAC_TRIANGLEAMPLITUDE_3: Select max triangle amplitude of 3
  *            @arg DAC_TRIANGLEAMPLITUDE_7: Select max triangle amplitude of 7
  *            @arg DAC_TRIANGLEAMPLITUDE_15: Select max triangle amplitude of 15
  *            @arg DAC_TRIANGLEAMPLITUDE_31: Select max triangle amplitude of 31
  *            @arg DAC_TRIANGLEAMPLITUDE_63: Select max triangle amplitude of 63
  *            @arg DAC_TRIANGLEAMPLITUDE_127: Select max triangle amplitude of 127
  *            @arg DAC_TRIANGLEAMPLITUDE_255: Select max triangle amplitude of 255
  *            @arg DAC_TRIANGLEAMPLITUDE_511: Select max triangle amplitude of 511
  *            @arg DAC_TRIANGLEAMPLITUDE_1023: Select max triangle amplitude of 1023
  *            @arg DAC_TRIANGLEAMPLITUDE_2047: Select max triangle amplitude of 2047
  *            @arg DAC_TRIANGLEAMPLITUDE_4095: Select max triangle amplitude of 4095
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DACEx_TriangleWaveGenerate(DAC_HandleTypeDef *hdac, uint32_t Channel, uint32_t Amplitude)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_DAC_CHANNEL(Channel));                /* 执行语句 */
  assert_param(IS_DAC_LFSR_UNMASK_TRIANGLE_AMPLITUDE(Amplitude));  /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hdac);                                     /* HAL辅助宏/函数 */

  /* Change DAC state */
  hdac->State = HAL_DAC_STATE_BUSY;                     /* 赋值或初始化 */

  /* Enable the triangle wave generation for the selected DAC channel */
  MODIFY_REG(hdac->Instance->CR, ((DAC_CR_WAVE1) | (DAC_CR_MAMP1)) << (Channel & 0x10UL),  /* 列表续行 */
             (DAC_CR_WAVE1_1 | Amplitude) << (Channel & 0x10UL));  /* 执行语句 */

  /* Change DAC state */
  hdac->State = HAL_DAC_STATE_READY;                    /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hdac);                                   /* HAL辅助宏/函数 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Enable or disable the selected DAC channel wave generation.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @param  Channel The selected DAC channel.
  *          This parameter can be one of the following values:
  *            @arg DAC_CHANNEL_1: DAC Channel1 selected
  *            @arg DAC_CHANNEL_2: DAC Channel2 selected
  * @param  Amplitude Unmask DAC channel LFSR for noise wave generation.
  *          This parameter can be one of the following values:
  *            @arg DAC_LFSRUNMASK_BIT0: Unmask DAC channel LFSR bit0 for noise wave generation
  *            @arg DAC_LFSRUNMASK_BITS1_0: Unmask DAC channel LFSR bit[1:0] for noise wave generation
  *            @arg DAC_LFSRUNMASK_BITS2_0: Unmask DAC channel LFSR bit[2:0] for noise wave generation
  *            @arg DAC_LFSRUNMASK_BITS3_0: Unmask DAC channel LFSR bit[3:0] for noise wave generation
  *            @arg DAC_LFSRUNMASK_BITS4_0: Unmask DAC channel LFSR bit[4:0] for noise wave generation
  *            @arg DAC_LFSRUNMASK_BITS5_0: Unmask DAC channel LFSR bit[5:0] for noise wave generation
  *            @arg DAC_LFSRUNMASK_BITS6_0: Unmask DAC channel LFSR bit[6:0] for noise wave generation
  *            @arg DAC_LFSRUNMASK_BITS7_0: Unmask DAC channel LFSR bit[7:0] for noise wave generation
  *            @arg DAC_LFSRUNMASK_BITS8_0: Unmask DAC channel LFSR bit[8:0] for noise wave generation
  *            @arg DAC_LFSRUNMASK_BITS9_0: Unmask DAC channel LFSR bit[9:0] for noise wave generation
  *            @arg DAC_LFSRUNMASK_BITS10_0: Unmask DAC channel LFSR bit[10:0] for noise wave generation
  *            @arg DAC_LFSRUNMASK_BITS11_0: Unmask DAC channel LFSR bit[11:0] for noise wave generation
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DACEx_NoiseWaveGenerate(DAC_HandleTypeDef *hdac, uint32_t Channel, uint32_t Amplitude)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_DAC_CHANNEL(Channel));                /* 执行语句 */
  assert_param(IS_DAC_LFSR_UNMASK_TRIANGLE_AMPLITUDE(Amplitude));  /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hdac);                                     /* HAL辅助宏/函数 */

  /* Change DAC state */
  hdac->State = HAL_DAC_STATE_BUSY;                     /* 赋值或初始化 */

  /* Enable the noise wave generation for the selected DAC channel */
  MODIFY_REG(hdac->Instance->CR, ((DAC_CR_WAVE1) | (DAC_CR_MAMP1)) << (Channel & 0x10UL),  /* 列表续行 */
             (DAC_CR_WAVE1_0 | Amplitude) << (Channel & 0x10UL));  /* 执行语句 */

  /* Change DAC state */
  hdac->State = HAL_DAC_STATE_READY;                    /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hdac);                                   /* HAL辅助宏/函数 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

#if defined(DAC_CHANNEL2_SUPPORT)                       /* 条件编译 */
/**
  * @brief  Set the specified data holding register value for dual DAC channel.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *               the configuration information for the specified DAC.
  * @param  Alignment Specifies the data alignment for dual channel DAC.
  *          This parameter can be one of the following values:
  *            DAC_ALIGN_8B_R: 8bit right data alignment selected
  *            DAC_ALIGN_12B_L: 12bit left data alignment selected
  *            DAC_ALIGN_12B_R: 12bit right data alignment selected
  * @param  Data1 Data for DAC Channel1 to be loaded in the selected data holding register.
  * @param  Data2 Data for DAC Channel2 to be loaded in the selected data  holding register.
  * @note   In dual mode, a unique register access is required to write in both
  *          DAC channels at the same time.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DACEx_DualSetValue(DAC_HandleTypeDef *hdac, uint32_t Alignment, uint32_t Data1, uint32_t Data2)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t data;                                        /* 32位无符号变量/函数 */
  uint32_t tmp;                                         /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_DAC_ALIGN(Alignment));                /* 执行语句 */
  assert_param(IS_DAC_DATA(Data1));                     /* 执行语句 */
  assert_param(IS_DAC_DATA(Data2));                     /* 执行语句 */

  /* Calculate and set dual DAC data holding register value */
  if (Alignment == DAC_ALIGN_8B_R)                      /* 条件判断 */
  {                                                     /* 代码块开始 */
    data = ((uint32_t)Data2 << 8U) | Data1;             /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    data = ((uint32_t)Data2 << 16U) | Data1;            /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  tmp = (uint32_t)hdac->Instance;                       /* 赋值或初始化 */
  tmp += DAC_DHR12RD_ALIGNMENT(Alignment);              /* 赋值或初始化 */

  /* Set the dual DAC selected data holding register */
  *(__IO uint32_t *)tmp = data;                         /* 赋值或初始化 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Conversion complete callback in non-blocking mode for Channel2.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @retval None
  */
__weak void HAL_DACEx_ConvCpltCallbackCh2(DAC_HandleTypeDef *hdac)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdac);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_DACEx_ConvCpltCallbackCh2 could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Conversion half DMA transfer callback in non-blocking mode for Channel2.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @retval None
  */
__weak void HAL_DACEx_ConvHalfCpltCallbackCh2(DAC_HandleTypeDef *hdac)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdac);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_DACEx_ConvHalfCpltCallbackCh2 could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Error DAC callback for Channel2.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @retval None
  */
__weak void HAL_DACEx_ErrorCallbackCh2(DAC_HandleTypeDef *hdac)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdac);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_DACEx_ErrorCallbackCh2 could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA underrun DAC callback for Channel2.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @retval None
  */
__weak void HAL_DACEx_DMAUnderrunCallbackCh2(DAC_HandleTypeDef *hdac)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdac);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_DACEx_DMAUnderrunCallbackCh2 could be implemented in the user file
   */
}                                                       /* 代码块结束 */
#endif /* DAC_CHANNEL2_SUPPORT */


/**
  * @}
  */

/** @defgroup DACEx_Exported_Functions_Group3 Peripheral Control functions
  *  @brief    Extended Peripheral Control functions
  *
@verbatim
  ==============================================================================
             ##### Peripheral Control functions #####
  ==============================================================================
    [..]  This section provides functions allowing to:
      (+) Set the specified data holding register value for DAC channel.

@endverbatim
  * @{
  */

#if defined(DAC_CHANNEL2_SUPPORT)                       /* 条件编译 */
/**
  * @brief  Return the last data output value of the selected DAC channel.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @retval The selected DAC channel data output value.
  */
uint32_t HAL_DACEx_DualGetValue(DAC_HandleTypeDef *hdac)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t tmp = 0UL;                                   /* 32位无符号变量/函数 */

  tmp |= hdac->Instance->DOR1;                          /* 赋值或初始化 */

  tmp |= hdac->Instance->DOR2 << 16UL;                  /* 赋值或初始化 */

  /* Returns the DAC channel data output register value */
  return tmp;                                           /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* DAC_CHANNEL2_SUPPORT */

/**
  * @}
  */
/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/
/** @defgroup DACEx_Private_Functions DACEx private functions
  *  @brief    Extended private functions
  * @{
  */

#if defined(DAC_CHANNEL2_SUPPORT)                       /* 条件编译 */
/**
  * @brief  DMA conversion complete callback.
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *                the configuration information for the specified DMA module.
  * @retval None
  */
void DAC_DMAConvCpltCh2(DMA_HandleTypeDef *hdma)        /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  DAC_HandleTypeDef *hdac = (DAC_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;  /* 赋值或初始化 */

#if (USE_HAL_DAC_REGISTER_CALLBACKS == 1)               /* 条件编译 */
  hdac->ConvCpltCallbackCh2(hdac);                      /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_DACEx_ConvCpltCallbackCh2(hdac);                  /* HAL库函数调用 */
#endif /* USE_HAL_DAC_REGISTER_CALLBACKS */

  hdac->State = HAL_DAC_STATE_READY;                    /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA half transfer complete callback.
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *                the configuration information for the specified DMA module.
  * @retval None
  */
void DAC_DMAHalfConvCpltCh2(DMA_HandleTypeDef *hdma)    /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  DAC_HandleTypeDef *hdac = (DAC_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;  /* 赋值或初始化 */
  /* Conversion complete callback */
#if (USE_HAL_DAC_REGISTER_CALLBACKS == 1)               /* 条件编译 */
  hdac->ConvHalfCpltCallbackCh2(hdac);                  /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_DACEx_ConvHalfCpltCallbackCh2(hdac);              /* HAL库函数调用 */
#endif /* USE_HAL_DAC_REGISTER_CALLBACKS */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA error callback.
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *                the configuration information for the specified DMA module.
  * @retval None
  */
void DAC_DMAErrorCh2(DMA_HandleTypeDef *hdma)           /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  DAC_HandleTypeDef *hdac = (DAC_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;  /* 赋值或初始化 */

  /* Set DAC error code to DMA error */
  hdac->ErrorCode |= HAL_DAC_ERROR_DMA;                 /* 赋值或初始化 */

#if (USE_HAL_DAC_REGISTER_CALLBACKS == 1)               /* 条件编译 */
  hdac->ErrorCallbackCh2(hdac);                         /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_DACEx_ErrorCallbackCh2(hdac);                     /* HAL库函数调用 */
#endif /* USE_HAL_DAC_REGISTER_CALLBACKS */

  hdac->State = HAL_DAC_STATE_READY;                    /* 赋值或初始化 */
}                                                       /* 代码块结束 */
#endif /* DAC_CHANNEL2_SUPPORT */

/**
  * @}
  */

/**
  * @}
  */

#endif /* DAC */

#endif /* HAL_DAC_MODULE_ENABLED */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
