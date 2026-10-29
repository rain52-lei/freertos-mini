/* 文件: stm32f4xx_hal_ltdc_ex.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_ltdc_ex.c
  * @author  MCD Application Team
  * @brief   LTDC Extension HAL module driver.
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

#if defined(HAL_LTDC_MODULE_ENABLED) && defined(HAL_DSI_MODULE_ENABLED)  /* 条件编译 */

#if defined (LTDC) && defined (DSI)                     /* 条件编译 */

/** @defgroup LTDCEx LTDCEx
  * @brief LTDC HAL module driver
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/** @defgroup LTDCEx_Exported_Functions LTDC Extended Exported Functions
  * @{
  */

/** @defgroup LTDCEx_Exported_Functions_Group1 Initialization and Configuration functions
  *  @brief   Initialization and Configuration functions
  *
@verbatim
 ===============================================================================
                ##### Initialization and Configuration functions #####
 ===============================================================================
    [..]  This section provides functions allowing to:
      (+) Initialize and configure the LTDC

@endverbatim
  * @{
  */

/**
  * @brief  Retrieve common parameters from DSI Video mode configuration structure
  * @param  hltdc   pointer to a LTDC_HandleTypeDef structure that contains
  *                 the configuration information for the LTDC.
  * @param  VidCfg  pointer to a DSI_VidCfgTypeDef structure that contains
  *                 the DSI video mode configuration parameters
  * @note   The implementation of this function is taking into account the LTDC
  *         polarities inversion as described in the current LTDC specification
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_LTDCEx_StructInitFromVideoConfig(LTDC_HandleTypeDef *hltdc, DSI_VidCfgTypeDef *VidCfg)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Retrieve signal polarities from DSI */

  /* The following polarity is inverted:
                     LTDC_DEPOLARITY_AL <-> LTDC_DEPOLARITY_AH */

  /* Note 1 : Code in line w/ Current LTDC specification */
  hltdc->Init.DEPolarity = (VidCfg->DEPolarity == DSI_DATA_ENABLE_ACTIVE_HIGH) ? LTDC_DEPOLARITY_AL : LTDC_DEPOLARITY_AH;  /* 赋值或初始化 */
  hltdc->Init.VSPolarity = (VidCfg->VSPolarity == DSI_VSYNC_ACTIVE_HIGH) ? LTDC_VSPOLARITY_AH : LTDC_VSPOLARITY_AL;  /* 赋值或初始化 */
  hltdc->Init.HSPolarity = (VidCfg->HSPolarity == DSI_HSYNC_ACTIVE_HIGH) ? LTDC_HSPOLARITY_AH : LTDC_HSPOLARITY_AL;  /* 赋值或初始化 */

  /* Note 2: Code to be used in case LTDC polarities inversion updated in the specification */
  /* hltdc->Init.DEPolarity = VidCfg->DEPolarity << 29;
     hltdc->Init.VSPolarity = VidCfg->VSPolarity << 29;
     hltdc->Init.HSPolarity = VidCfg->HSPolarity << 29; */

  /* Retrieve vertical timing parameters from DSI */
  hltdc->Init.VerticalSync       = VidCfg->VerticalSyncActive - 1U;  /* 赋值或初始化 */
  hltdc->Init.AccumulatedVBP     = VidCfg->VerticalSyncActive + VidCfg->VerticalBackPorch - 1U;  /* 赋值或初始化 */
  hltdc->Init.AccumulatedActiveH = VidCfg->VerticalSyncActive + VidCfg->VerticalBackPorch + VidCfg->VerticalActive - 1U;  /* 赋值或初始化 */
  hltdc->Init.TotalHeigh         = VidCfg->VerticalSyncActive + VidCfg->VerticalBackPorch + VidCfg->VerticalActive + VidCfg->VerticalFrontPorch - 1U;  /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Retrieve common parameters from DSI Adapted command mode configuration structure
  * @param  hltdc   pointer to a LTDC_HandleTypeDef structure that contains
  *                 the configuration information for the LTDC.
  * @param  CmdCfg  pointer to a DSI_CmdCfgTypeDef structure that contains
  *                 the DSI command mode configuration parameters
  * @note   The implementation of this function is taking into account the LTDC
  *         polarities inversion as described in the current LTDC specification
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_LTDCEx_StructInitFromAdaptedCommandConfig(LTDC_HandleTypeDef *hltdc, DSI_CmdCfgTypeDef *CmdCfg)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Retrieve signal polarities from DSI */

  /* The following polarities are inverted:
                     LTDC_DEPOLARITY_AL <-> LTDC_DEPOLARITY_AH
                     LTDC_VSPOLARITY_AL <-> LTDC_VSPOLARITY_AH
                     LTDC_HSPOLARITY_AL <-> LTDC_HSPOLARITY_AH)*/

  /* Note 1 : Code in line w/ Current LTDC specification */
  hltdc->Init.DEPolarity = (CmdCfg->DEPolarity == DSI_DATA_ENABLE_ACTIVE_HIGH) ? LTDC_DEPOLARITY_AL : LTDC_DEPOLARITY_AH;  /* 赋值或初始化 */
  hltdc->Init.VSPolarity = (CmdCfg->VSPolarity == DSI_VSYNC_ACTIVE_HIGH) ? LTDC_VSPOLARITY_AL : LTDC_VSPOLARITY_AH;  /* 赋值或初始化 */
  hltdc->Init.HSPolarity = (CmdCfg->HSPolarity == DSI_HSYNC_ACTIVE_HIGH) ? LTDC_HSPOLARITY_AL : LTDC_HSPOLARITY_AH;  /* 赋值或初始化 */

  /* Note 2: Code to be used in case LTDC polarities inversion updated in the specification */
  /* hltdc->Init.DEPolarity = CmdCfg->DEPolarity << 29;
     hltdc->Init.VSPolarity = CmdCfg->VSPolarity << 29;
     hltdc->Init.HSPolarity = CmdCfg->HSPolarity << 29; */

  return HAL_OK;                                        /* 返回值 */
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

#endif /* LTDC && DSI */

#endif /* HAL_LTCD_MODULE_ENABLED && HAL_DSI_MODULE_ENABLED */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
