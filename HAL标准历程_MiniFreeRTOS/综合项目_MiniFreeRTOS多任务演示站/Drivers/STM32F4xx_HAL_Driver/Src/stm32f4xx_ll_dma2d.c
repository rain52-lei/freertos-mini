/* 文件: stm32f4xx_ll_dma2d.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_ll_dma2d.c
  * @author  MCD Application Team
  * @brief   DMA2D LL module driver.
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
#if defined(USE_FULL_LL_DRIVER)                         /* 条件编译 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_ll_dma2d.h"                         /* 包含 STM32/HAL 头文件 */
#include "stm32f4xx_ll_bus.h"                           /* 包含 STM32/HAL 头文件 */
#ifdef  USE_FULL_ASSERT                                 /* 条件编译 */
#include "stm32_assert.h"                               /* 包含 STM32/HAL 头文件 */
#else                                                   /* 条件编译否则 */
#define assert_param(expr) ((void)0U)                   /* 定义宏 */
#endif  /* USE_FULL_ASSERT */

/** @addtogroup STM32F4xx_LL_Driver
  * @{
  */

#if defined (DMA2D)                                     /* 条件编译 */

/** @addtogroup DMA2D_LL
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/** @addtogroup DMA2D_LL_Private_Constants DMA2D Private Constants
  * @{
  */
#define LL_DMA2D_COLOR            0xFFU                                      /*!< Maximum output color setting                   */
#define LL_DMA2D_NUMBEROFLINES    DMA2D_NLR_NL                               /*!< Maximum number of lines                        */
#define LL_DMA2D_NUMBEROFPIXELS   (DMA2D_NLR_PL >> DMA2D_NLR_PL_Pos)         /*!< Maximum number of pixels per lines             */
#define LL_DMA2D_OFFSET_MAX       0x3FFFU                                    /*!< Maximum output line offset expressed in pixels */
#define LL_DMA2D_CLUTSIZE_MAX     0xFFU                                      /*!< Maximum CLUT size                              */
/**
  * @}
  */
/* Private macros ------------------------------------------------------------*/
/** @addtogroup DMA2D_LL_Private_Macros
  * @{
  */
#define IS_LL_DMA2D_MODE(MODE)          (((MODE) == LL_DMA2D_MODE_M2M)       || \
                                         ((MODE) == LL_DMA2D_MODE_M2M_PFC)   || \
                                         ((MODE) == LL_DMA2D_MODE_M2M_BLEND) || \
                                         ((MODE) == LL_DMA2D_MODE_R2M))  /* 源代码 */

#define IS_LL_DMA2D_OCMODE(MODE_ARGB)   (((MODE_ARGB) == LL_DMA2D_OUTPUT_MODE_ARGB8888) || \
                                         ((MODE_ARGB) == LL_DMA2D_OUTPUT_MODE_RGB888)   || \
                                         ((MODE_ARGB) == LL_DMA2D_OUTPUT_MODE_RGB565)   || \
                                         ((MODE_ARGB) == LL_DMA2D_OUTPUT_MODE_ARGB1555) || \
                                         ((MODE_ARGB) == LL_DMA2D_OUTPUT_MODE_ARGB4444))  /* 源代码 */

#define IS_LL_DMA2D_GREEN(GREEN)        ((GREEN) <= LL_DMA2D_COLOR)  /* 定义宏 */
#define IS_LL_DMA2D_RED(RED)            ((RED)   <= LL_DMA2D_COLOR)  /* 定义宏 */
#define IS_LL_DMA2D_BLUE(BLUE)          ((BLUE)  <= LL_DMA2D_COLOR)  /* 定义宏 */
#define IS_LL_DMA2D_ALPHA(ALPHA)        ((ALPHA) <= LL_DMA2D_COLOR)  /* 定义宏 */


#define IS_LL_DMA2D_OFFSET(OFFSET)      ((OFFSET) <= LL_DMA2D_OFFSET_MAX)  /* 定义宏 */

#define IS_LL_DMA2D_LINE(LINES)         ((LINES)  <= LL_DMA2D_NUMBEROFLINES)  /* 定义宏 */
#define IS_LL_DMA2D_PIXEL(PIXELS)       ((PIXELS) <= LL_DMA2D_NUMBEROFPIXELS)  /* 定义宏 */



#define IS_LL_DMA2D_LCMODE(MODE_ARGB)   (((MODE_ARGB) == LL_DMA2D_INPUT_MODE_ARGB8888) || \
                                         ((MODE_ARGB) == LL_DMA2D_INPUT_MODE_RGB888)   || \
                                         ((MODE_ARGB) == LL_DMA2D_INPUT_MODE_RGB565)   || \
                                         ((MODE_ARGB) == LL_DMA2D_INPUT_MODE_ARGB1555) || \
                                         ((MODE_ARGB) == LL_DMA2D_INPUT_MODE_ARGB4444) || \
                                         ((MODE_ARGB) == LL_DMA2D_INPUT_MODE_L8)       || \
                                         ((MODE_ARGB) == LL_DMA2D_INPUT_MODE_AL44)     || \
                                         ((MODE_ARGB) == LL_DMA2D_INPUT_MODE_AL88)     || \
                                         ((MODE_ARGB) == LL_DMA2D_INPUT_MODE_L4)       || \
                                         ((MODE_ARGB) == LL_DMA2D_INPUT_MODE_A8)       || \
                                         ((MODE_ARGB) == LL_DMA2D_INPUT_MODE_A4))  /* 源代码 */

#define IS_LL_DMA2D_CLUTCMODE(CLUTCMODE) (((CLUTCMODE) == LL_DMA2D_CLUT_COLOR_MODE_ARGB8888) || \
                                          ((CLUTCMODE) == LL_DMA2D_CLUT_COLOR_MODE_RGB888))  /* 源代码 */

#define IS_LL_DMA2D_CLUTSIZE(SIZE)      ((SIZE) <= LL_DMA2D_CLUTSIZE_MAX)  /* 定义宏 */

#define IS_LL_DMA2D_ALPHAMODE(MODE)     (((MODE) == LL_DMA2D_ALPHA_MODE_NO_MODIF) || \
                                         ((MODE) == LL_DMA2D_ALPHA_MODE_REPLACE)  || \
                                         ((MODE) == LL_DMA2D_ALPHA_MODE_COMBINE))  /* 源代码 */


/**
  * @}
  */

/* Private function prototypes -----------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/** @addtogroup DMA2D_LL_Exported_Functions
  * @{
  */

/** @addtogroup DMA2D_LL_EF_Init_Functions Initialization and De-initialization Functions
  * @{
  */

/**
  * @brief  De-initialize DMA2D registers (registers restored to their default values).
  * @param  DMA2Dx DMA2D Instance
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: DMA2D registers are de-initialized
  *          - ERROR: DMA2D registers are not de-initialized
  */
ErrorStatus LL_DMA2D_DeInit(DMA2D_TypeDef *DMA2Dx)      /* 源代码 */
{                                                       /* 代码块开始 */
  ErrorStatus status = SUCCESS;                         /* 赋值或初始化 */

  /* Check the parameters */
  assert_param(IS_DMA2D_ALL_INSTANCE(DMA2Dx));          /* 执行语句 */

  if (DMA2Dx == DMA2D)                                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Force reset of DMA2D clock */
    LL_AHB1_GRP1_ForceReset(LL_AHB1_GRP1_PERIPH_DMA2D);  /* 执行语句 */

    /* Release reset of DMA2D clock */
    LL_AHB1_GRP1_ReleaseReset(LL_AHB1_GRP1_PERIPH_DMA2D);  /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    status = ERROR;                                     /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return (status);                                      /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initialize DMA2D registers according to the specified parameters in DMA2D_InitStruct.
  * @note   DMA2D transfers must be disabled to set initialization bits in configuration registers,
  *         otherwise ERROR result is returned.
  * @param  DMA2Dx DMA2D Instance
  * @param  DMA2D_InitStruct  pointer to a LL_DMA2D_InitTypeDef structure
  *         that contains the configuration information for the specified DMA2D peripheral.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: DMA2D registers are initialized according to DMA2D_InitStruct content
  *          - ERROR: Issue occurred during DMA2D registers initialization
  */
ErrorStatus LL_DMA2D_Init(DMA2D_TypeDef *DMA2Dx, LL_DMA2D_InitTypeDef *DMA2D_InitStruct)  /* 源代码 */
{                                                       /* 代码块开始 */
  ErrorStatus status = ERROR;                           /* 赋值或初始化 */
  LL_DMA2D_ColorTypeDef dma2d_colorstruct;              /* 执行语句 */
  uint32_t tmp;                                         /* 32位无符号变量/函数 */
  uint32_t tmp1;                                        /* 32位无符号变量/函数 */
  uint32_t tmp2;                                        /* 32位无符号变量/函数 */
  uint32_t regMask;                                     /* 32位无符号变量/函数 */
  uint32_t regValue;                                    /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_DMA2D_ALL_INSTANCE(DMA2Dx));          /* 执行语句 */
  assert_param(IS_LL_DMA2D_MODE(DMA2D_InitStruct->Mode));  /* 执行语句 */
  assert_param(IS_LL_DMA2D_OCMODE(DMA2D_InitStruct->ColorMode));  /* 执行语句 */
  assert_param(IS_LL_DMA2D_LINE(DMA2D_InitStruct->NbrOfLines));  /* 执行语句 */
  assert_param(IS_LL_DMA2D_PIXEL(DMA2D_InitStruct->NbrOfPixelsPerLines));  /* 执行语句 */
  assert_param(IS_LL_DMA2D_GREEN(DMA2D_InitStruct->OutputGreen));  /* 执行语句 */
  assert_param(IS_LL_DMA2D_RED(DMA2D_InitStruct->OutputRed));  /* 执行语句 */
  assert_param(IS_LL_DMA2D_BLUE(DMA2D_InitStruct->OutputBlue));  /* 执行语句 */
  assert_param(IS_LL_DMA2D_ALPHA(DMA2D_InitStruct->OutputAlpha));  /* 执行语句 */
  assert_param(IS_LL_DMA2D_OFFSET(DMA2D_InitStruct->LineOffset));  /* 执行语句 */

  /* DMA2D transfers must be disabled to configure bits in initialization registers */
  tmp = LL_DMA2D_IsTransferOngoing(DMA2Dx);             /* 赋值或初始化 */
  tmp1 = LL_DMA2D_FGND_IsEnabledCLUTLoad(DMA2Dx);       /* 赋值或初始化 */
  tmp2 = LL_DMA2D_BGND_IsEnabledCLUTLoad(DMA2Dx);       /* 赋值或初始化 */
  if ((tmp == 0U) && (tmp1 == 0U) && (tmp2 == 0U))      /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* DMA2D CR register configuration -------------------------------------------*/
    LL_DMA2D_SetMode(DMA2Dx, DMA2D_InitStruct->Mode);   /* 执行语句 */

    /* DMA2D OPFCCR register configuration ---------------------------------------*/
    regMask = DMA2D_OPFCCR_CM;                          /* 赋值或初始化 */
    regValue = DMA2D_InitStruct->ColorMode;             /* 赋值或初始化 */




    MODIFY_REG(DMA2Dx->OPFCCR, regMask, regValue);      /* 执行语句 */

    /* DMA2D OOR register configuration ------------------------------------------*/
    LL_DMA2D_SetLineOffset(DMA2Dx, DMA2D_InitStruct->LineOffset);  /* 执行语句 */

    /* DMA2D NLR register configuration ------------------------------------------*/
    LL_DMA2D_ConfigSize(DMA2Dx, DMA2D_InitStruct->NbrOfLines, DMA2D_InitStruct->NbrOfPixelsPerLines);  /* 执行语句 */

    /* DMA2D OMAR register configuration ------------------------------------------*/
    LL_DMA2D_SetOutputMemAddr(DMA2Dx, DMA2D_InitStruct->OutputMemoryAddress);  /* 执行语句 */

    /* DMA2D OCOLR register configuration ------------------------------------------*/
    dma2d_colorstruct.ColorMode   = DMA2D_InitStruct->ColorMode;  /* 赋值或初始化 */
    dma2d_colorstruct.OutputBlue  = DMA2D_InitStruct->OutputBlue;  /* 赋值或初始化 */
    dma2d_colorstruct.OutputGreen = DMA2D_InitStruct->OutputGreen;  /* 赋值或初始化 */
    dma2d_colorstruct.OutputRed   = DMA2D_InitStruct->OutputRed;  /* 赋值或初始化 */
    dma2d_colorstruct.OutputAlpha = DMA2D_InitStruct->OutputAlpha;  /* 赋值或初始化 */
    LL_DMA2D_ConfigOutputColor(DMA2Dx, &dma2d_colorstruct);  /* 执行语句 */

    status = SUCCESS;                                   /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  /* If DMA2D transfers are not disabled, return ERROR */

  return (status);                                      /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief Set each @ref LL_DMA2D_InitTypeDef field to default value.
  * @param DMA2D_InitStruct  pointer to a @ref LL_DMA2D_InitTypeDef structure
  *                          whose fields will be set to default values.
  * @retval None
  */
void LL_DMA2D_StructInit(LL_DMA2D_InitTypeDef *DMA2D_InitStruct)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Set DMA2D_InitStruct fields to default values */
  DMA2D_InitStruct->Mode                = LL_DMA2D_MODE_M2M;  /* 赋值或初始化 */
  DMA2D_InitStruct->ColorMode           = LL_DMA2D_OUTPUT_MODE_ARGB8888;  /* 赋值或初始化 */
  DMA2D_InitStruct->NbrOfLines          = 0x0U;         /* 赋值或初始化 */
  DMA2D_InitStruct->NbrOfPixelsPerLines = 0x0U;         /* 赋值或初始化 */
  DMA2D_InitStruct->LineOffset          = 0x0U;         /* 赋值或初始化 */
  DMA2D_InitStruct->OutputBlue          = 0x0U;         /* 赋值或初始化 */
  DMA2D_InitStruct->OutputGreen         = 0x0U;         /* 赋值或初始化 */
  DMA2D_InitStruct->OutputRed           = 0x0U;         /* 赋值或初始化 */
  DMA2D_InitStruct->OutputAlpha         = 0x0U;         /* 赋值或初始化 */
  DMA2D_InitStruct->OutputMemoryAddress = 0x0U;         /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configure the foreground or background according to the specified parameters
  *         in the LL_DMA2D_LayerCfgTypeDef structure.
  * @param  DMA2Dx DMA2D Instance
  * @param  DMA2D_LayerCfg  pointer to a LL_DMA2D_LayerCfgTypeDef structure that contains
  *         the configuration information for the specified layer.
  * @param  LayerIdx  DMA2D Layer index.
  *                   This parameter can be one of the following values:
  *                   0(background) / 1(foreground)
  * @retval None
  */
void LL_DMA2D_ConfigLayer(DMA2D_TypeDef *DMA2Dx, LL_DMA2D_LayerCfgTypeDef *DMA2D_LayerCfg, uint32_t LayerIdx)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_LL_DMA2D_OFFSET(DMA2D_LayerCfg->LineOffset));  /* 执行语句 */
  assert_param(IS_LL_DMA2D_LCMODE(DMA2D_LayerCfg->ColorMode));  /* 执行语句 */
  assert_param(IS_LL_DMA2D_CLUTCMODE(DMA2D_LayerCfg->CLUTColorMode));  /* 执行语句 */
  assert_param(IS_LL_DMA2D_CLUTSIZE(DMA2D_LayerCfg->CLUTSize));  /* 执行语句 */
  assert_param(IS_LL_DMA2D_ALPHAMODE(DMA2D_LayerCfg->AlphaMode));  /* 执行语句 */
  assert_param(IS_LL_DMA2D_GREEN(DMA2D_LayerCfg->Green));  /* 执行语句 */
  assert_param(IS_LL_DMA2D_RED(DMA2D_LayerCfg->Red));   /* 执行语句 */
  assert_param(IS_LL_DMA2D_BLUE(DMA2D_LayerCfg->Blue));  /* 执行语句 */
  assert_param(IS_LL_DMA2D_ALPHA(DMA2D_LayerCfg->Alpha));  /* 执行语句 */


  if (LayerIdx == 0U)                                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Configure the background memory address */
    LL_DMA2D_BGND_SetMemAddr(DMA2Dx, DMA2D_LayerCfg->MemoryAddress);  /* 执行语句 */

    /* Configure the background line offset */
    LL_DMA2D_BGND_SetLineOffset(DMA2Dx, DMA2D_LayerCfg->LineOffset);  /* 执行语句 */

    /* Configure the background Alpha value, Alpha mode, CLUT size, CLUT Color mode and Color mode */
    MODIFY_REG(DMA2Dx->BGPFCCR, \
               (DMA2D_BGPFCCR_ALPHA | DMA2D_BGPFCCR_AM | DMA2D_BGPFCCR_CS | DMA2D_BGPFCCR_CCM | DMA2D_BGPFCCR_CM), \
               ((DMA2D_LayerCfg->Alpha << DMA2D_BGPFCCR_ALPHA_Pos) | DMA2D_LayerCfg->AlphaMode | \
                (DMA2D_LayerCfg->CLUTSize << DMA2D_BGPFCCR_CS_Pos) | DMA2D_LayerCfg->CLUTColorMode | \
                DMA2D_LayerCfg->ColorMode));            /* 执行语句 */

    /* Configure the background color */
    LL_DMA2D_BGND_SetColor(DMA2Dx, DMA2D_LayerCfg->Red, DMA2D_LayerCfg->Green, DMA2D_LayerCfg->Blue);  /* 执行语句 */

    /* Configure the background CLUT memory address */
    LL_DMA2D_BGND_SetCLUTMemAddr(DMA2Dx, DMA2D_LayerCfg->CLUTMemoryAddress);  /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Configure the foreground memory address */
    LL_DMA2D_FGND_SetMemAddr(DMA2Dx, DMA2D_LayerCfg->MemoryAddress);  /* 执行语句 */

    /* Configure the foreground line offset */
    LL_DMA2D_FGND_SetLineOffset(DMA2Dx, DMA2D_LayerCfg->LineOffset);  /* 执行语句 */

    /* Configure the foreground Alpha value, Alpha mode, CLUT size, CLUT Color mode and Color mode */
    MODIFY_REG(DMA2Dx->FGPFCCR, \
               (DMA2D_FGPFCCR_ALPHA | DMA2D_FGPFCCR_AM | DMA2D_FGPFCCR_CS | DMA2D_FGPFCCR_CCM | DMA2D_FGPFCCR_CM), \
               ((DMA2D_LayerCfg->Alpha << DMA2D_FGPFCCR_ALPHA_Pos) | DMA2D_LayerCfg->AlphaMode | \
                (DMA2D_LayerCfg->CLUTSize << DMA2D_FGPFCCR_CS_Pos) | DMA2D_LayerCfg->CLUTColorMode | \
                DMA2D_LayerCfg->ColorMode));            /* 执行语句 */

    /* Configure the foreground color */
    LL_DMA2D_FGND_SetColor(DMA2Dx, DMA2D_LayerCfg->Red, DMA2D_LayerCfg->Green, DMA2D_LayerCfg->Blue);  /* 执行语句 */

    /* Configure the foreground CLUT memory address */
    LL_DMA2D_FGND_SetCLUTMemAddr(DMA2Dx, DMA2D_LayerCfg->CLUTMemoryAddress);  /* 执行语句 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief Set each @ref LL_DMA2D_LayerCfgTypeDef field to default value.
  * @param DMA2D_LayerCfg  pointer to a @ref LL_DMA2D_LayerCfgTypeDef structure
  *                        whose fields will be set to default values.
  * @retval None
  */
void LL_DMA2D_LayerCfgStructInit(LL_DMA2D_LayerCfgTypeDef *DMA2D_LayerCfg)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Set DMA2D_LayerCfg fields to default values */
  DMA2D_LayerCfg->MemoryAddress      = 0x0U;            /* 赋值或初始化 */
  DMA2D_LayerCfg->ColorMode          = LL_DMA2D_INPUT_MODE_ARGB8888;  /* 赋值或初始化 */
  DMA2D_LayerCfg->LineOffset         = 0x0U;            /* 赋值或初始化 */
  DMA2D_LayerCfg->CLUTColorMode      = LL_DMA2D_CLUT_COLOR_MODE_ARGB8888;  /* 赋值或初始化 */
  DMA2D_LayerCfg->CLUTSize           = 0x0U;            /* 赋值或初始化 */
  DMA2D_LayerCfg->AlphaMode          = LL_DMA2D_ALPHA_MODE_NO_MODIF;  /* 赋值或初始化 */
  DMA2D_LayerCfg->Alpha              = 0x0U;            /* 赋值或初始化 */
  DMA2D_LayerCfg->Blue               = 0x0U;            /* 赋值或初始化 */
  DMA2D_LayerCfg->Green              = 0x0U;            /* 赋值或初始化 */
  DMA2D_LayerCfg->Red                = 0x0U;            /* 赋值或初始化 */
  DMA2D_LayerCfg->CLUTMemoryAddress  = 0x0U;            /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initialize DMA2D output color register according to the specified parameters
  *         in DMA2D_ColorStruct.
  * @param  DMA2Dx DMA2D Instance
  * @param  DMA2D_ColorStruct  pointer to a LL_DMA2D_ColorTypeDef structure that contains
  *         the color configuration information for the specified DMA2D peripheral.
  * @retval None
  */
void LL_DMA2D_ConfigOutputColor(DMA2D_TypeDef *DMA2Dx, LL_DMA2D_ColorTypeDef *DMA2D_ColorStruct)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  uint32_t outgreen;                                    /* 32位无符号变量/函数 */
  uint32_t outred;                                      /* 32位无符号变量/函数 */
  uint32_t outalpha;                                    /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_DMA2D_ALL_INSTANCE(DMA2Dx));          /* 执行语句 */
  assert_param(IS_LL_DMA2D_OCMODE(DMA2D_ColorStruct->ColorMode));  /* 执行语句 */
  assert_param(IS_LL_DMA2D_GREEN(DMA2D_ColorStruct->OutputGreen));  /* 执行语句 */
  assert_param(IS_LL_DMA2D_RED(DMA2D_ColorStruct->OutputRed));  /* 执行语句 */
  assert_param(IS_LL_DMA2D_BLUE(DMA2D_ColorStruct->OutputBlue));  /* 执行语句 */
  assert_param(IS_LL_DMA2D_ALPHA(DMA2D_ColorStruct->OutputAlpha));  /* 执行语句 */

  /* DMA2D OCOLR register configuration ------------------------------------------*/
  if (DMA2D_ColorStruct->ColorMode == LL_DMA2D_OUTPUT_MODE_ARGB8888)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    outgreen = DMA2D_ColorStruct->OutputGreen << 8U;    /* 赋值或初始化 */
    outred = DMA2D_ColorStruct->OutputRed << 16U;       /* 赋值或初始化 */
    outalpha = DMA2D_ColorStruct->OutputAlpha << 24U;   /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if (DMA2D_ColorStruct->ColorMode == LL_DMA2D_OUTPUT_MODE_RGB888)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    outgreen = DMA2D_ColorStruct->OutputGreen << 8U;    /* 赋值或初始化 */
    outred = DMA2D_ColorStruct->OutputRed << 16U;       /* 赋值或初始化 */
    outalpha = 0x00000000U;                             /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if (DMA2D_ColorStruct->ColorMode == LL_DMA2D_OUTPUT_MODE_RGB565)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    outgreen = DMA2D_ColorStruct->OutputGreen << 5U;    /* 赋值或初始化 */
    outred = DMA2D_ColorStruct->OutputRed << 11U;       /* 赋值或初始化 */
    outalpha = 0x00000000U;                             /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if (DMA2D_ColorStruct->ColorMode == LL_DMA2D_OUTPUT_MODE_ARGB1555)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    outgreen = DMA2D_ColorStruct->OutputGreen << 5U;    /* 赋值或初始化 */
    outred = DMA2D_ColorStruct->OutputRed << 10U;       /* 赋值或初始化 */
    outalpha = DMA2D_ColorStruct->OutputAlpha << 15U;   /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else /* ColorMode = LL_DMA2D_OUTPUT_MODE_ARGB4444 */
  {                                                     /* 代码块开始 */
    outgreen = DMA2D_ColorStruct->OutputGreen << 4U;    /* 赋值或初始化 */
    outred = DMA2D_ColorStruct->OutputRed << 8U;        /* 赋值或初始化 */
    outalpha = DMA2D_ColorStruct->OutputAlpha << 12U;   /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  LL_DMA2D_SetOutputColor(DMA2Dx, (outgreen | outred | DMA2D_ColorStruct->OutputBlue | outalpha));  /* 执行语句 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return DMA2D output Blue color.
  * @param  DMA2Dx DMA2D Instance.
  * @param  ColorMode This parameter can be one of the following values:
  *         @arg @ref LL_DMA2D_OUTPUT_MODE_ARGB8888
  *         @arg @ref LL_DMA2D_OUTPUT_MODE_RGB888
  *         @arg @ref LL_DMA2D_OUTPUT_MODE_RGB565
  *         @arg @ref LL_DMA2D_OUTPUT_MODE_ARGB1555
  *         @arg @ref LL_DMA2D_OUTPUT_MODE_ARGB4444
  * @retval Output Blue color value between Min_Data=0 and Max_Data=0xFF
  */
uint32_t LL_DMA2D_GetOutputBlueColor(DMA2D_TypeDef *DMA2Dx, uint32_t ColorMode)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t color;                                       /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_DMA2D_ALL_INSTANCE(DMA2Dx));          /* 执行语句 */
  assert_param(IS_LL_DMA2D_OCMODE(ColorMode));          /* 执行语句 */

  /* DMA2D OCOLR register reading ------------------------------------------*/
  if (ColorMode == LL_DMA2D_OUTPUT_MODE_ARGB8888)       /* 条件判断 */
  {                                                     /* 代码块开始 */
    color = (uint32_t)(READ_BIT(DMA2Dx->OCOLR, 0xFFU));  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if (ColorMode == LL_DMA2D_OUTPUT_MODE_RGB888)    /* 否则如果 */
  {                                                     /* 代码块开始 */
    color = (uint32_t)(READ_BIT(DMA2Dx->OCOLR, 0xFFU));  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if (ColorMode == LL_DMA2D_OUTPUT_MODE_RGB565)    /* 否则如果 */
  {                                                     /* 代码块开始 */
    color = (uint32_t)(READ_BIT(DMA2Dx->OCOLR, 0x1FU));  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if (ColorMode == LL_DMA2D_OUTPUT_MODE_ARGB1555)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    color = (uint32_t)(READ_BIT(DMA2Dx->OCOLR, 0x1FU));  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else /* ColorMode = LL_DMA2D_OUTPUT_MODE_ARGB4444 */
  {                                                     /* 代码块开始 */
    color = (uint32_t)(READ_BIT(DMA2Dx->OCOLR, 0xFU));  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return color;                                         /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return DMA2D output Green color.
  * @param  DMA2Dx DMA2D Instance.
  * @param  ColorMode This parameter can be one of the following values:
  *         @arg @ref LL_DMA2D_OUTPUT_MODE_ARGB8888
  *         @arg @ref LL_DMA2D_OUTPUT_MODE_RGB888
  *         @arg @ref LL_DMA2D_OUTPUT_MODE_RGB565
  *         @arg @ref LL_DMA2D_OUTPUT_MODE_ARGB1555
  *         @arg @ref LL_DMA2D_OUTPUT_MODE_ARGB4444
  * @retval Output Green color value between Min_Data=0 and Max_Data=0xFF
  */
uint32_t LL_DMA2D_GetOutputGreenColor(DMA2D_TypeDef *DMA2Dx, uint32_t ColorMode)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t color;                                       /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_DMA2D_ALL_INSTANCE(DMA2Dx));          /* 执行语句 */
  assert_param(IS_LL_DMA2D_OCMODE(ColorMode));          /* 执行语句 */

  /* DMA2D OCOLR register reading ------------------------------------------*/
  if (ColorMode == LL_DMA2D_OUTPUT_MODE_ARGB8888)       /* 条件判断 */
  {                                                     /* 代码块开始 */
    color = (uint32_t)(READ_BIT(DMA2Dx->OCOLR, 0xFF00U) >> 8U);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if (ColorMode == LL_DMA2D_OUTPUT_MODE_RGB888)    /* 否则如果 */
  {                                                     /* 代码块开始 */
    color = (uint32_t)(READ_BIT(DMA2Dx->OCOLR, 0xFF00U) >> 8U);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if (ColorMode == LL_DMA2D_OUTPUT_MODE_RGB565)    /* 否则如果 */
  {                                                     /* 代码块开始 */
    color = (uint32_t)(READ_BIT(DMA2Dx->OCOLR, 0x7E0U) >> 5U);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if (ColorMode == LL_DMA2D_OUTPUT_MODE_ARGB1555)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    color = (uint32_t)(READ_BIT(DMA2Dx->OCOLR, 0x3E0U) >> 5U);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else /* ColorMode = LL_DMA2D_OUTPUT_MODE_ARGB4444 */
  {                                                     /* 代码块开始 */
    color = (uint32_t)(READ_BIT(DMA2Dx->OCOLR, 0xF0U) >> 4U);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return color;                                         /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return DMA2D output Red color.
  * @param  DMA2Dx DMA2D Instance.
  * @param  ColorMode This parameter can be one of the following values:
  *         @arg @ref LL_DMA2D_OUTPUT_MODE_ARGB8888
  *         @arg @ref LL_DMA2D_OUTPUT_MODE_RGB888
  *         @arg @ref LL_DMA2D_OUTPUT_MODE_RGB565
  *         @arg @ref LL_DMA2D_OUTPUT_MODE_ARGB1555
  *         @arg @ref LL_DMA2D_OUTPUT_MODE_ARGB4444
  * @retval Output Red color value between Min_Data=0 and Max_Data=0xFF
  */
uint32_t LL_DMA2D_GetOutputRedColor(DMA2D_TypeDef *DMA2Dx, uint32_t ColorMode)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t color;                                       /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_DMA2D_ALL_INSTANCE(DMA2Dx));          /* 执行语句 */
  assert_param(IS_LL_DMA2D_OCMODE(ColorMode));          /* 执行语句 */

  /* DMA2D OCOLR register reading ------------------------------------------*/
  if (ColorMode == LL_DMA2D_OUTPUT_MODE_ARGB8888)       /* 条件判断 */
  {                                                     /* 代码块开始 */
    color = (uint32_t)(READ_BIT(DMA2Dx->OCOLR, 0xFF0000U) >> 16U);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if (ColorMode == LL_DMA2D_OUTPUT_MODE_RGB888)    /* 否则如果 */
  {                                                     /* 代码块开始 */
    color = (uint32_t)(READ_BIT(DMA2Dx->OCOLR, 0xFF0000U) >> 16U);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if (ColorMode == LL_DMA2D_OUTPUT_MODE_RGB565)    /* 否则如果 */
  {                                                     /* 代码块开始 */
    color = (uint32_t)(READ_BIT(DMA2Dx->OCOLR, 0xF800U) >> 11U);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if (ColorMode == LL_DMA2D_OUTPUT_MODE_ARGB1555)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    color = (uint32_t)(READ_BIT(DMA2Dx->OCOLR, 0x7C00U) >> 10U);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else /* ColorMode = LL_DMA2D_OUTPUT_MODE_ARGB4444 */
  {                                                     /* 代码块开始 */
    color = (uint32_t)(READ_BIT(DMA2Dx->OCOLR, 0xF00U) >> 8U);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return color;                                         /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return DMA2D output Alpha color.
  * @param  DMA2Dx DMA2D Instance.
  * @param  ColorMode This parameter can be one of the following values:
  *         @arg @ref LL_DMA2D_OUTPUT_MODE_ARGB8888
  *         @arg @ref LL_DMA2D_OUTPUT_MODE_RGB888
  *         @arg @ref LL_DMA2D_OUTPUT_MODE_RGB565
  *         @arg @ref LL_DMA2D_OUTPUT_MODE_ARGB1555
  *         @arg @ref LL_DMA2D_OUTPUT_MODE_ARGB4444
  * @retval Output Alpha color value between Min_Data=0 and Max_Data=0xFF
  */
uint32_t LL_DMA2D_GetOutputAlphaColor(DMA2D_TypeDef *DMA2Dx, uint32_t ColorMode)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t color;                                       /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_DMA2D_ALL_INSTANCE(DMA2Dx));          /* 执行语句 */
  assert_param(IS_LL_DMA2D_OCMODE(ColorMode));          /* 执行语句 */

  /* DMA2D OCOLR register reading ------------------------------------------*/
  if (ColorMode == LL_DMA2D_OUTPUT_MODE_ARGB8888)       /* 条件判断 */
  {                                                     /* 代码块开始 */
    color = (uint32_t)(READ_BIT(DMA2Dx->OCOLR, 0xFF000000U) >> 24U);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if ((ColorMode == LL_DMA2D_OUTPUT_MODE_RGB888) || (ColorMode == LL_DMA2D_OUTPUT_MODE_RGB565))  /* 否则如果 */
  {                                                     /* 代码块开始 */
    color = 0x0U;                                       /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if (ColorMode == LL_DMA2D_OUTPUT_MODE_ARGB1555)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    color = (uint32_t)(READ_BIT(DMA2Dx->OCOLR, 0x8000U) >> 15U);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else /* ColorMode = LL_DMA2D_OUTPUT_MODE_ARGB4444 */
  {                                                     /* 代码块开始 */
    color = (uint32_t)(READ_BIT(DMA2Dx->OCOLR, 0xF000U) >> 12U);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return color;                                         /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configure DMA2D transfer size.
  * @param  DMA2Dx DMA2D Instance
  * @param  NbrOfLines Value between Min_Data=0 and Max_Data=0xFFFF
  * @param  NbrOfPixelsPerLines Value between Min_Data=0 and Max_Data=0x3FFF
  * @retval None
  */
void LL_DMA2D_ConfigSize(DMA2D_TypeDef *DMA2Dx, uint32_t NbrOfLines, uint32_t NbrOfPixelsPerLines)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  MODIFY_REG(DMA2Dx->NLR, (DMA2D_NLR_PL | DMA2D_NLR_NL), \
             ((NbrOfPixelsPerLines << DMA2D_NLR_PL_Pos) | NbrOfLines));  /* 执行语句 */
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

#endif /* defined (DMA2D) */

/**
  * @}
  */

#endif /* USE_FULL_LL_DRIVER */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

