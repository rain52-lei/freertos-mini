/* 文件: stm32f4xx_hal_dsi.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_dsi.c
  * @author  MCD Application Team
  * @brief   DSI HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the DSI peripheral:
  *           + Initialization and de-initialization functions
  *           + IO operation functions
  *           + Peripheral Control functions
  *           + Peripheral State and Errors functions
  @verbatim
  ==============================================================================
                        ##### How to use this driver #####
  ==============================================================================
  [..]
    The DSI HAL driver can be used as follows:

    (#) Declare a DSI_HandleTypeDef handle structure, for example: DSI_HandleTypeDef  hdsi;

    (#) Initialize the DSI low level resources by implementing the HAL_DSI_MspInit() API:
        (##) Enable the DSI interface clock
        (##) NVIC configuration if you need to use interrupt process
            (+++) Configure the DSI interrupt priority
            (+++) Enable the NVIC DSI IRQ Channel

    (#) Initialize the DSI Host peripheral, the required PLL parameters, number of lances and
        TX Escape clock divider by calling the HAL_DSI_Init() API which calls HAL_DSI_MspInit().

    *** Configuration ***
    =========================
    [..]
    (#) Use HAL_DSI_ConfigAdaptedCommandMode() function to configure the DSI host in adapted
        command mode.

    (#) When operating in video mode , use HAL_DSI_ConfigVideoMode() to configure the DSI host.

    (#) Function HAL_DSI_ConfigCommand() is used to configure the DSI commands behavior in low power mode.

    (#) To configure the DSI PHY timings parameters, use function HAL_DSI_ConfigPhyTimer().

    (#) The DSI Host can be started/stopped using respectively functions HAL_DSI_Start() and HAL_DSI_Stop().
        Functions HAL_DSI_ShortWrite(), HAL_DSI_LongWrite() and HAL_DSI_Read() allows respectively
        to write DSI short packets, long packets and to read DSI packets.

    (#) The DSI Host Offers two Low power modes :
        (++) Low Power Mode on data lanes only: Only DSI data lanes are shut down.
            It is possible to enter/exit from this mode using respectively functions HAL_DSI_EnterULPMData()
            and HAL_DSI_ExitULPMData()

        (++) Low Power Mode on data and clock lanes : All DSI lanes are shut down including data and clock lanes.
            It is possible to enter/exit from this mode using respectively functions HAL_DSI_EnterULPM()
            and HAL_DSI_ExitULPM()

    (#) To control DSI state you can use the following function: HAL_DSI_GetState()

    *** Error management ***
    ========================
    [..]
    (#) User can select the DSI errors to be reported/monitored using function HAL_DSI_ConfigErrorMonitor()
        When an error occurs, the callback HAL_DSI_ErrorCallback() is asserted and then user can retrieve
        the error code by calling function HAL_DSI_GetError()

    *** DSI HAL driver macros list ***
    =============================================
    [..]
       Below the list of most used macros in DSI HAL driver.

      (+) __HAL_DSI_ENABLE: Enable the DSI Host.
      (+) __HAL_DSI_DISABLE: Disable the DSI Host.
      (+) __HAL_DSI_WRAPPER_ENABLE: Enables the DSI wrapper.
      (+) __HAL_DSI_WRAPPER_DISABLE: Disable the DSI wrapper.
      (+) __HAL_DSI_PLL_ENABLE: Enables the DSI PLL.
      (+) __HAL_DSI_PLL_DISABLE: Disables the DSI PLL.
      (+) __HAL_DSI_REG_ENABLE: Enables the DSI regulator.
      (+) __HAL_DSI_REG_DISABLE: Disables the DSI regulator.
      (+) __HAL_DSI_GET_FLAG: Get the DSI pending flags.
      (+) __HAL_DSI_CLEAR_FLAG: Clears the DSI pending flags.
      (+) __HAL_DSI_ENABLE_IT: Enables the specified DSI interrupts.
      (+) __HAL_DSI_DISABLE_IT: Disables the specified DSI interrupts.
      (+) __HAL_DSI_GET_IT_SOURCE: Checks whether the specified DSI interrupt source is enabled or not.

    [..]
      (@) You can refer to the DSI HAL driver header file for more useful macros

    *** Callback registration ***
    =============================================
    [..]
    The compilation define  USE_HAL_DSI_REGISTER_CALLBACKS when set to 1
    allows the user to configure dynamically the driver callbacks.
    Use Function HAL_DSI_RegisterCallback() to register a callback.

    [..]
    Function HAL_DSI_RegisterCallback() allows to register following callbacks:
      (+) TearingEffectCallback : DSI Tearing Effect Callback.
      (+) EndOfRefreshCallback  : DSI End Of Refresh Callback.
      (+) ErrorCallback         : DSI Error Callback
      (+) MspInitCallback       : DSI MspInit.
      (+) MspDeInitCallback     : DSI MspDeInit.
    [..]
    This function takes as parameters the HAL peripheral handle, the callback ID
    and a pointer to the user callback function.

    [..]
    Use function HAL_DSI_UnRegisterCallback() to reset a callback to the default
    weak function.
    HAL_DSI_UnRegisterCallback takes as parameters the HAL peripheral handle,
    and the callback ID.
    [..]
    This function allows to reset following callbacks:
      (+) TearingEffectCallback : DSI Tearing Effect Callback.
      (+) EndOfRefreshCallback  : DSI End Of Refresh Callback.
      (+) ErrorCallback         : DSI Error Callback
      (+) MspInitCallback       : DSI MspInit.
      (+) MspDeInitCallback     : DSI MspDeInit.

    [..]
    By default, after the HAL_DSI_Init and when the state is HAL_DSI_STATE_RESET
    all callbacks are set to the corresponding weak functions:
    examples HAL_DSI_TearingEffectCallback(), HAL_DSI_EndOfRefreshCallback().
    Exception done for MspInit and MspDeInit functions that are respectively
    reset to the legacy weak (surcharged) functions in the HAL_DSI_Init()
    and HAL_DSI_DeInit() only when these callbacks are null (not registered beforehand).
    If not, MspInit or MspDeInit are not null, the HAL_DSI_Init() and HAL_DSI_DeInit()
    keep and use the user MspInit/MspDeInit callbacks (registered beforehand).

    [..]
    Callbacks can be registered/unregistered in HAL_DSI_STATE_READY state only.
    Exception done MspInit/MspDeInit that can be registered/unregistered
    in HAL_DSI_STATE_READY or HAL_DSI_STATE_RESET state,
    thus registered (user) MspInit/DeInit callbacks can be used during the Init/DeInit.
    In that case first register the MspInit/MspDeInit user callbacks
    using HAL_DSI_RegisterCallback() before calling HAL_DSI_DeInit()
    or HAL_DSI_Init() function.

    [..]
    When The compilation define USE_HAL_DSI_REGISTER_CALLBACKS is set to 0 or
    not defined, the callback registration feature is not available and all callbacks
    are set to the corresponding weak functions.

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

#ifdef HAL_DSI_MODULE_ENABLED                           /* 条件编译 */

#if defined(DSI)                                        /* 条件编译 */

/** @addtogroup DSI
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/** @addtogroup DSI_Private_Constants
  * @{
  */
#define DSI_TIMEOUT_VALUE ((uint32_t)1000U)  /* 1s */

#define DSI_ERROR_ACK_MASK (DSI_ISR0_AE0 | DSI_ISR0_AE1 | DSI_ISR0_AE2 | DSI_ISR0_AE3 | \
                            DSI_ISR0_AE4 | DSI_ISR0_AE5 | DSI_ISR0_AE6 | DSI_ISR0_AE7 | \
                            DSI_ISR0_AE8 | DSI_ISR0_AE9 | DSI_ISR0_AE10 | DSI_ISR0_AE11 | \
                            DSI_ISR0_AE12 | DSI_ISR0_AE13 | DSI_ISR0_AE14 | DSI_ISR0_AE15)  /* 源代码 */
#define DSI_ERROR_PHY_MASK (DSI_ISR0_PE0 | DSI_ISR0_PE1 | DSI_ISR0_PE2 | DSI_ISR0_PE3 | DSI_ISR0_PE4)  /* 定义宏 */
#define DSI_ERROR_TX_MASK  DSI_ISR1_TOHSTX              /* 定义宏 */
#define DSI_ERROR_RX_MASK  DSI_ISR1_TOLPRX              /* 定义宏 */
#define DSI_ERROR_ECC_MASK (DSI_ISR1_ECCSE | DSI_ISR1_ECCME)  /* 定义宏 */
#define DSI_ERROR_CRC_MASK DSI_ISR1_CRCE                /* 定义宏 */
#define DSI_ERROR_PSE_MASK DSI_ISR1_PSE                 /* 定义宏 */
#define DSI_ERROR_EOT_MASK DSI_ISR1_EOTPE               /* 定义宏 */
#define DSI_ERROR_OVF_MASK DSI_ISR1_LPWRE               /* 定义宏 */
#define DSI_ERROR_GEN_MASK (DSI_ISR1_GCWRE | DSI_ISR1_GPWRE | DSI_ISR1_GPTXE | DSI_ISR1_GPRDE | DSI_ISR1_GPRXE)  /* 定义宏 */
/**
  * @}
  */

/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void DSI_ConfigPacketHeader(DSI_TypeDef *DSIx, uint32_t ChannelID, uint32_t DataType, uint32_t Data0,  /* 静态函数 */
                                   uint32_t Data1);     /* 32位无符号变量/函数 */

static HAL_StatusTypeDef DSI_ShortWrite(DSI_HandleTypeDef *hdsi,  /* 静态变量/函数 */
                                     uint32_t ChannelID,  /* 32位无符号变量/函数 */
                                     uint32_t Mode,     /* 32位无符号变量/函数 */
                                     uint32_t Param1,   /* 32位无符号变量/函数 */
                                     uint32_t Param2);  /* 32位无符号变量/函数 */

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Generic DSI packet header configuration
  * @param  DSIx  Pointer to DSI register base
  * @param  ChannelID Virtual channel ID of the header packet
  * @param  DataType  Packet data type of the header packet
  *                   This parameter can be any value of :
  *                      @arg DSI_SHORT_WRITE_PKT_Data_Type
  *                      @arg DSI_LONG_WRITE_PKT_Data_Type
  *                      @arg DSI_SHORT_READ_PKT_Data_Type
  *                      @arg DSI_MAX_RETURN_PKT_SIZE
  * @param  Data0  Word count LSB
  * @param  Data1  Word count MSB
  * @retval None
  */
static void DSI_ConfigPacketHeader(DSI_TypeDef *DSIx,   /* 静态函数 */
                                   uint32_t ChannelID,  /* 32位无符号变量/函数 */
                                   uint32_t DataType,   /* 32位无符号变量/函数 */
                                   uint32_t Data0,      /* 32位无符号变量/函数 */
                                   uint32_t Data1)      /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  /* Update the DSI packet header with new information */
  DSIx->GHCR = (DataType | (ChannelID << 6U) | (Data0 << 8U) | (Data1 << 16U));  /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  write short DCS or short Generic command
  * @param  hdsi  pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @param  ChannelID  Virtual channel ID.
  * @param  Mode  DSI short packet data type.
  *               This parameter can be any value of @arg DSI_SHORT_WRITE_PKT_Data_Type.
  * @param  Param1  DSC command or first generic parameter.
  *                 This parameter can be any value of @arg DSI_DCS_Command or a
  *                 generic command code.
  * @param  Param2  DSC parameter or second generic parameter.
  * @retval HAL status
  */
static HAL_StatusTypeDef DSI_ShortWrite(DSI_HandleTypeDef *hdsi,  /* 静态变量/函数 */
                                        uint32_t ChannelID,  /* 32位无符号变量/函数 */
                                        uint32_t Mode,  /* 32位无符号变量/函数 */
                                        uint32_t Param1,  /* 32位无符号变量/函数 */
                                        uint32_t Param2)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */

  /* Get tick */
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  /* Wait for Command FIFO Empty */
  while((hdsi->Instance->GPSR & DSI_GPSR_CMDFE) == 0U)  /* while循环 */
  {                                                     /* 代码块开始 */
    /* Check for the Timeout */
    if((HAL_GetTick() - tickstart ) > DSI_TIMEOUT_VALUE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_TIMEOUT;                               /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Configure the packet to send a short DCS command with 0 or 1 parameter */
  /* Update the DSI packet header with new information */
  hdsi->Instance->GHCR = (Mode | (ChannelID << 6U) | (Param1 << 8U) | (Param2 << 16U));  /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup DSI_Exported_Functions
  * @{
  */

/** @defgroup DSI_Group1 Initialization and Configuration functions
  *  @brief   Initialization and Configuration functions
  *
@verbatim
 ===============================================================================
                ##### Initialization and Configuration functions #####
 ===============================================================================
    [..]  This section provides functions allowing to:
      (+) Initialize and configure the DSI
      (+) De-initialize the DSI

@endverbatim
  * @{
  */

/**
  * @brief  Initializes the DSI according to the specified
  *         parameters in the DSI_InitTypeDef and create the associated handle.
  * @param  hdsi  pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @param  PLLInit  pointer to a DSI_PLLInitTypeDef structure that contains
  *                  the PLL Clock structure definition for the DSI.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DSI_Init(DSI_HandleTypeDef *hdsi, DSI_PLLInitTypeDef *PLLInit)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */
  uint32_t unitIntervalx4;                              /* 32位无符号变量/函数 */
  uint32_t tempIDF;                                     /* 32位无符号变量/函数 */

  /* Check the DSI handle allocation */
  if (hdsi == NULL)                                     /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check function parameters */
  assert_param(IS_DSI_PLL_NDIV(PLLInit->PLLNDIV));      /* 执行语句 */
  assert_param(IS_DSI_PLL_IDF(PLLInit->PLLIDF));        /* 执行语句 */
  assert_param(IS_DSI_PLL_ODF(PLLInit->PLLODF));        /* 执行语句 */
  assert_param(IS_DSI_AUTO_CLKLANE_CONTROL(hdsi->Init.AutomaticClockLaneControl));  /* 执行语句 */
  assert_param(IS_DSI_NUMBER_OF_LANES(hdsi->Init.NumberOfLanes));  /* 执行语句 */

#if (USE_HAL_DSI_REGISTER_CALLBACKS == 1)               /* 条件编译 */
  if (hdsi->State == HAL_DSI_STATE_RESET)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Reset the DSI callback to the legacy weak callbacks */
    hdsi->TearingEffectCallback = HAL_DSI_TearingEffectCallback; /* Legacy weak TearingEffectCallback */
    hdsi->EndOfRefreshCallback  = HAL_DSI_EndOfRefreshCallback;  /* Legacy weak EndOfRefreshCallback  */
    hdsi->ErrorCallback         = HAL_DSI_ErrorCallback;         /* Legacy weak ErrorCallback         */

    if (hdsi->MspInitCallback == NULL)                  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hdsi->MspInitCallback = HAL_DSI_MspInit;          /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    /* Initialize the low level hardware */
    hdsi->MspInitCallback(hdsi);                        /* 执行语句 */
  }                                                     /* 代码块结束 */
#else                                                   /* 条件编译否则 */
  if (hdsi->State == HAL_DSI_STATE_RESET)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Initialize the low level hardware */
    HAL_DSI_MspInit(hdsi);                              /* HAL库函数调用 */
  }                                                     /* 代码块结束 */
#endif /* USE_HAL_DSI_REGISTER_CALLBACKS */

  /* Change DSI peripheral state */
  hdsi->State = HAL_DSI_STATE_BUSY;                     /* 赋值或初始化 */

  /**************** Turn on the regulator and enable the DSI PLL ****************/

  /* Enable the regulator */
  __HAL_DSI_REG_ENABLE(hdsi);                           /* HAL辅助宏/函数 */

  /* Get tick */
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  /* Wait until the regulator is ready */
  while (__HAL_DSI_GET_FLAG(hdsi, DSI_FLAG_RRS) == 0U)  /* while循环 */
  {                                                     /* 代码块开始 */
    /* Check for the Timeout */
    if ((HAL_GetTick() - tickstart) > DSI_TIMEOUT_VALUE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_TIMEOUT;                               /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Set the PLL division factors */
  hdsi->Instance->WRPCR &= ~(DSI_WRPCR_PLL_NDIV | DSI_WRPCR_PLL_IDF | DSI_WRPCR_PLL_ODF);  /* 赋值或初始化 */
  hdsi->Instance->WRPCR |= (((PLLInit->PLLNDIV) << 2U) | ((PLLInit->PLLIDF) << 11U) | ((PLLInit->PLLODF) << 16U));  /* 赋值或初始化 */

  /* Enable the DSI PLL */
  __HAL_DSI_PLL_ENABLE(hdsi);                           /* HAL辅助宏/函数 */

  /* Get tick */
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  /* Wait for the lock of the PLL */
  while (__HAL_DSI_GET_FLAG(hdsi, DSI_FLAG_PLLLS) == 0U)  /* while循环 */
  {                                                     /* 代码块开始 */
    /* Check for the Timeout */
    if ((HAL_GetTick() - tickstart) > DSI_TIMEOUT_VALUE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_TIMEOUT;                               /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /*************************** Set the PHY parameters ***************************/

  /* D-PHY clock and digital enable*/
  hdsi->Instance->PCTLR |= (DSI_PCTLR_CKE | DSI_PCTLR_DEN);  /* 赋值或初始化 */

  /* Clock lane configuration */
  hdsi->Instance->CLCR &= ~(DSI_CLCR_DPCC | DSI_CLCR_ACR);  /* 赋值或初始化 */
  hdsi->Instance->CLCR |= (DSI_CLCR_DPCC | hdsi->Init.AutomaticClockLaneControl);  /* 赋值或初始化 */

  /* Configure the number of active data lanes */
  hdsi->Instance->PCONFR &= ~DSI_PCONFR_NL;             /* 赋值或初始化 */
  hdsi->Instance->PCONFR |= hdsi->Init.NumberOfLanes;   /* 赋值或初始化 */

  /************************ Set the DSI clock parameters ************************/

  /* Set the TX escape clock division factor */
  hdsi->Instance->CCR &= ~DSI_CCR_TXECKDIV;             /* 赋值或初始化 */
  hdsi->Instance->CCR |= hdsi->Init.TXEscapeCkdiv;      /* 赋值或初始化 */

  /* Calculate the bit period in high-speed mode in unit of 0.25 ns (UIX4) */
  /* The equation is : UIX4 = IntegerPart( (1000/F_PHY_Mhz) * 4 )          */
  /* Where : F_PHY_Mhz = (NDIV * HSE_Mhz) / (IDF * ODF)                    */
  tempIDF = (PLLInit->PLLIDF > 0U) ? PLLInit->PLLIDF : 1U;  /* 赋值或初始化 */
  unitIntervalx4 = (4000000U * tempIDF * ((1UL << (0x3U & PLLInit->PLLODF)))) / ((HSE_VALUE / 1000U) * PLLInit->PLLNDIV);  /* 赋值或初始化 */

  /* Set the bit period in high-speed mode */
  hdsi->Instance->WPCR[0U] &= ~DSI_WPCR0_UIX4;          /* 赋值或初始化 */
  hdsi->Instance->WPCR[0U] |= unitIntervalx4;           /* 赋值或初始化 */

  /****************************** Error management *****************************/

  /* Disable all error interrupts and reset the Error Mask */
  hdsi->Instance->IER[0U] = 0U;                         /* 赋值或初始化 */
  hdsi->Instance->IER[1U] = 0U;                         /* 赋值或初始化 */
  hdsi->ErrorMsk = 0U;                                  /* 赋值或初始化 */

  /* Initialise the error code */
  hdsi->ErrorCode = HAL_DSI_ERROR_NONE;                 /* 赋值或初始化 */

  /* Initialize the DSI state*/
  hdsi->State = HAL_DSI_STATE_READY;                    /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  De-initializes the DSI peripheral registers to their default reset
  *         values.
  * @param  hdsi  pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DSI_DeInit(DSI_HandleTypeDef *hdsi)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the DSI handle allocation */
  if (hdsi == NULL)                                     /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Change DSI peripheral state */
  hdsi->State = HAL_DSI_STATE_BUSY;                     /* 赋值或初始化 */

  /* Disable the DSI wrapper */
  __HAL_DSI_WRAPPER_DISABLE(hdsi);                      /* HAL辅助宏/函数 */

  /* Disable the DSI host */
  __HAL_DSI_DISABLE(hdsi);                              /* HAL辅助宏/函数 */

  /* D-PHY clock and digital disable */
  hdsi->Instance->PCTLR &= ~(DSI_PCTLR_CKE | DSI_PCTLR_DEN);  /* 赋值或初始化 */

  /* Turn off the DSI PLL */
  __HAL_DSI_PLL_DISABLE(hdsi);                          /* HAL辅助宏/函数 */

  /* Disable the regulator */
  __HAL_DSI_REG_DISABLE(hdsi);                          /* HAL辅助宏/函数 */

#if (USE_HAL_DSI_REGISTER_CALLBACKS == 1)               /* 条件编译 */
  if (hdsi->MspDeInitCallback == NULL)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hdsi->MspDeInitCallback = HAL_DSI_MspDeInit;        /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  /* DeInit the low level hardware */
  hdsi->MspDeInitCallback(hdsi);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* DeInit the low level hardware */
  HAL_DSI_MspDeInit(hdsi);                              /* HAL库函数调用 */
#endif /* USE_HAL_DSI_REGISTER_CALLBACKS */

  /* Initialise the error code */
  hdsi->ErrorCode = HAL_DSI_ERROR_NONE;                 /* 赋值或初始化 */

  /* Initialize the DSI state*/
  hdsi->State = HAL_DSI_STATE_RESET;                    /* 赋值或初始化 */

  /* Release Lock */
  __HAL_UNLOCK(hdsi);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Enable the error monitor flags
  * @param  hdsi  pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @param  ActiveErrors  indicates which error interrupts will be enabled.
  *                      This parameter can be any combination of @arg DSI_Error_Data_Type.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DSI_ConfigErrorMonitor(DSI_HandleTypeDef *hdsi, uint32_t ActiveErrors)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Process locked */
  __HAL_LOCK(hdsi);                                     /* HAL辅助宏/函数 */

  hdsi->Instance->IER[0U] = 0U;                         /* 赋值或初始化 */
  hdsi->Instance->IER[1U] = 0U;                         /* 赋值或初始化 */

  /* Store active errors to the handle */
  hdsi->ErrorMsk = ActiveErrors;                        /* 赋值或初始化 */

  if ((ActiveErrors & HAL_DSI_ERROR_ACK) != 0U)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable the interrupt generation on selected errors */
    hdsi->Instance->IER[0U] |= DSI_ERROR_ACK_MASK;      /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if ((ActiveErrors & HAL_DSI_ERROR_PHY) != 0U)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable the interrupt generation on selected errors */
    hdsi->Instance->IER[0U] |= DSI_ERROR_PHY_MASK;      /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if ((ActiveErrors & HAL_DSI_ERROR_TX) != 0U)          /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable the interrupt generation on selected errors */
    hdsi->Instance->IER[1U] |= DSI_ERROR_TX_MASK;       /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if ((ActiveErrors & HAL_DSI_ERROR_RX) != 0U)          /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable the interrupt generation on selected errors */
    hdsi->Instance->IER[1U] |= DSI_ERROR_RX_MASK;       /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if ((ActiveErrors & HAL_DSI_ERROR_ECC) != 0U)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable the interrupt generation on selected errors */
    hdsi->Instance->IER[1U] |= DSI_ERROR_ECC_MASK;      /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if ((ActiveErrors & HAL_DSI_ERROR_CRC) != 0U)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable the interrupt generation on selected errors */
    hdsi->Instance->IER[1U] |= DSI_ERROR_CRC_MASK;      /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if ((ActiveErrors & HAL_DSI_ERROR_PSE) != 0U)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable the interrupt generation on selected errors */
    hdsi->Instance->IER[1U] |= DSI_ERROR_PSE_MASK;      /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if ((ActiveErrors & HAL_DSI_ERROR_EOT) != 0U)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable the interrupt generation on selected errors */
    hdsi->Instance->IER[1U] |= DSI_ERROR_EOT_MASK;      /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if ((ActiveErrors & HAL_DSI_ERROR_OVF) != 0U)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable the interrupt generation on selected errors */
    hdsi->Instance->IER[1U] |= DSI_ERROR_OVF_MASK;      /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if ((ActiveErrors & HAL_DSI_ERROR_GEN) != 0U)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable the interrupt generation on selected errors */
    hdsi->Instance->IER[1U] |= DSI_ERROR_GEN_MASK;      /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Process Unlocked */
  __HAL_UNLOCK(hdsi);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initializes the DSI MSP.
  * @param  hdsi  pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @retval None
  */
__weak void HAL_DSI_MspInit(DSI_HandleTypeDef *hdsi)    /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdsi);                                         /* 执行语句 */
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_DSI_MspInit could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  De-initializes the DSI MSP.
  * @param  hdsi  pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @retval None
  */
__weak void HAL_DSI_MspDeInit(DSI_HandleTypeDef *hdsi)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdsi);                                         /* 执行语句 */
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_DSI_MspDeInit could be implemented in the user file
   */
}                                                       /* 代码块结束 */

#if (USE_HAL_DSI_REGISTER_CALLBACKS == 1)               /* 条件编译 */
/**
  * @brief  Register a User DSI Callback
  *         To be used instead of the weak predefined callback
  * @param hdsi dsi handle
  * @param CallbackID ID of the callback to be registered
  *        This parameter can be one of the following values:
  *          @arg HAL_DSI_TEARING_EFFECT_CB_ID Tearing Effect Callback ID
  *          @arg HAL_DSI_ENDOF_REFRESH_CB_ID End Of Refresh Callback ID
  *          @arg HAL_DSI_ERROR_CB_ID Error Callback ID
  *          @arg HAL_DSI_MSPINIT_CB_ID MspInit callback ID
  *          @arg HAL_DSI_MSPDEINIT_CB_ID MspDeInit callback ID
  * @param pCallback pointer to the Callback function
  * @retval status
  */
HAL_StatusTypeDef HAL_DSI_RegisterCallback(DSI_HandleTypeDef *hdsi, HAL_DSI_CallbackIDTypeDef CallbackID,  /* HAL状态类型函数 */
                                           pDSI_CallbackTypeDef pCallback)  /* 源代码 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  if (pCallback == NULL)                                /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hdsi->ErrorCode |= HAL_DSI_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  /* Process locked */
  __HAL_LOCK(hdsi);                                     /* HAL辅助宏/函数 */

  if (hdsi->State == HAL_DSI_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_DSI_TEARING_EFFECT_CB_ID :               /* case标签 */
        hdsi->TearingEffectCallback = pCallback;        /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_DSI_ENDOF_REFRESH_CB_ID :                /* case标签 */
        hdsi->EndOfRefreshCallback = pCallback;         /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_DSI_ERROR_CB_ID :                        /* case标签 */
        hdsi->ErrorCallback = pCallback;                /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_DSI_MSPINIT_CB_ID :                      /* case标签 */
        hdsi->MspInitCallback = pCallback;              /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_DSI_MSPDEINIT_CB_ID :                    /* case标签 */
        hdsi->MspDeInitCallback = pCallback;            /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hdsi->ErrorCode |= HAL_DSI_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (hdsi->State == HAL_DSI_STATE_RESET)          /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_DSI_MSPINIT_CB_ID :                      /* case标签 */
        hdsi->MspInitCallback = pCallback;              /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_DSI_MSPDEINIT_CB_ID :                    /* case标签 */
        hdsi->MspDeInitCallback = pCallback;            /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hdsi->ErrorCode |= HAL_DSI_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hdsi->ErrorCode |= HAL_DSI_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hdsi);                                   /* HAL辅助宏/函数 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Unregister a DSI Callback
  *         DSI callabck is redirected to the weak predefined callback
  * @param hdsi dsi handle
  * @param CallbackID ID of the callback to be unregistered
  *        This parameter can be one of the following values:
  *          @arg HAL_DSI_TEARING_EFFECT_CB_ID Tearing Effect Callback ID
  *          @arg HAL_DSI_ENDOF_REFRESH_CB_ID End Of Refresh Callback ID
  *          @arg HAL_DSI_ERROR_CB_ID Error Callback ID
  *          @arg HAL_DSI_MSPINIT_CB_ID MspInit callback ID
  *          @arg HAL_DSI_MSPDEINIT_CB_ID MspDeInit callback ID
  * @retval status
  */
HAL_StatusTypeDef HAL_DSI_UnRegisterCallback(DSI_HandleTypeDef *hdsi, HAL_DSI_CallbackIDTypeDef CallbackID)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Process locked */
  __HAL_LOCK(hdsi);                                     /* HAL辅助宏/函数 */

  if (hdsi->State == HAL_DSI_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_DSI_TEARING_EFFECT_CB_ID :               /* case标签 */
        hdsi->TearingEffectCallback = HAL_DSI_TearingEffectCallback; /* Legacy weak TearingEffectCallback */
        break;                                          /* 跳出 */

      case HAL_DSI_ENDOF_REFRESH_CB_ID :                /* case标签 */
        hdsi->EndOfRefreshCallback = HAL_DSI_EndOfRefreshCallback;   /* Legacy weak EndOfRefreshCallback  */
        break;                                          /* 跳出 */

      case HAL_DSI_ERROR_CB_ID :                        /* case标签 */
        hdsi->ErrorCallback        = HAL_DSI_ErrorCallback;          /* Legacy weak ErrorCallback        */
        break;                                          /* 跳出 */

      case HAL_DSI_MSPINIT_CB_ID :                      /* case标签 */
        hdsi->MspInitCallback = HAL_DSI_MspInit;                     /* Legcay weak MspInit Callback     */
        break;                                          /* 跳出 */

      case HAL_DSI_MSPDEINIT_CB_ID :                    /* case标签 */
        hdsi->MspDeInitCallback = HAL_DSI_MspDeInit;                 /* Legcay weak MspDeInit Callback   */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hdsi->ErrorCode |= HAL_DSI_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (hdsi->State == HAL_DSI_STATE_RESET)          /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_DSI_MSPINIT_CB_ID :                      /* case标签 */
        hdsi->MspInitCallback = HAL_DSI_MspInit;                  /* Legcay weak MspInit Callback   */
        break;                                          /* 跳出 */

      case HAL_DSI_MSPDEINIT_CB_ID :                    /* case标签 */
        hdsi->MspDeInitCallback = HAL_DSI_MspDeInit;              /* Legcay weak MspDeInit Callback */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hdsi->ErrorCode |= HAL_DSI_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hdsi->ErrorCode |= HAL_DSI_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hdsi);                                   /* HAL辅助宏/函数 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* USE_HAL_DSI_REGISTER_CALLBACKS */

/**
  * @}
  */

/** @defgroup DSI_Group2 IO operation functions
  *  @brief    IO operation functions
  *
@verbatim
 ===============================================================================
                      #####  IO operation functions  #####
 ===============================================================================
    [..]  This section provides function allowing to:
      (+) Handle DSI interrupt request

@endverbatim
  * @{
  */
/**
  * @brief  Handles DSI interrupt request.
  * @param  hdsi  pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @retval HAL status
  */
void HAL_DSI_IRQHandler(DSI_HandleTypeDef *hdsi)        /* 中断服务函数 */
{                                                       /* 代码块开始 */
  uint32_t ErrorStatus0, ErrorStatus1;                  /* 32位无符号变量/函数 */

  /* Tearing Effect Interrupt management ***************************************/
  if (__HAL_DSI_GET_FLAG(hdsi, DSI_FLAG_TE) != 0U)      /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (__HAL_DSI_GET_IT_SOURCE(hdsi, DSI_IT_TE) != 0U)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear the Tearing Effect Interrupt Flag */
      __HAL_DSI_CLEAR_FLAG(hdsi, DSI_FLAG_TE);          /* HAL辅助宏/函数 */

      /* Tearing Effect Callback */
#if (USE_HAL_DSI_REGISTER_CALLBACKS == 1)               /* 条件编译 */
      /*Call registered Tearing Effect callback */
      hdsi->TearingEffectCallback(hdsi);                /* 执行语句 */
#else                                                   /* 条件编译否则 */
      /*Call legacy Tearing Effect callback*/
      HAL_DSI_TearingEffectCallback(hdsi);              /* HAL库函数调用 */
#endif /* USE_HAL_DSI_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* End of Refresh Interrupt management ***************************************/
  if (__HAL_DSI_GET_FLAG(hdsi, DSI_FLAG_ER) != 0U)      /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (__HAL_DSI_GET_IT_SOURCE(hdsi, DSI_IT_ER) != 0U)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear the End of Refresh Interrupt Flag */
      __HAL_DSI_CLEAR_FLAG(hdsi, DSI_FLAG_ER);          /* HAL辅助宏/函数 */

      /* End of Refresh Callback */
#if (USE_HAL_DSI_REGISTER_CALLBACKS == 1)               /* 条件编译 */
      /*Call registered End of refresh callback */
      hdsi->EndOfRefreshCallback(hdsi);                 /* 执行语句 */
#else                                                   /* 条件编译否则 */
      /*Call Legacy End of refresh callback */
      HAL_DSI_EndOfRefreshCallback(hdsi);               /* HAL库函数调用 */
#endif /* USE_HAL_DSI_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Error Interrupts management ***********************************************/
  if (hdsi->ErrorMsk != 0U)                             /* 条件判断 */
  {                                                     /* 代码块开始 */
    ErrorStatus0 = hdsi->Instance->ISR[0U];             /* 赋值或初始化 */
    ErrorStatus0 &= hdsi->Instance->IER[0U];            /* 赋值或初始化 */
    ErrorStatus1 = hdsi->Instance->ISR[1U];             /* 赋值或初始化 */
    ErrorStatus1 &= hdsi->Instance->IER[1U];            /* 赋值或初始化 */

    if ((ErrorStatus0 & DSI_ERROR_ACK_MASK) != 0U)      /* 条件判断 */
    {                                                   /* 代码块开始 */
      hdsi->ErrorCode |= HAL_DSI_ERROR_ACK;             /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    if ((ErrorStatus0 & DSI_ERROR_PHY_MASK) != 0U)      /* 条件判断 */
    {                                                   /* 代码块开始 */
      hdsi->ErrorCode |= HAL_DSI_ERROR_PHY;             /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    if ((ErrorStatus1 & DSI_ERROR_TX_MASK) != 0U)       /* 条件判断 */
    {                                                   /* 代码块开始 */
      hdsi->ErrorCode |= HAL_DSI_ERROR_TX;              /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    if ((ErrorStatus1 & DSI_ERROR_RX_MASK) != 0U)       /* 条件判断 */
    {                                                   /* 代码块开始 */
      hdsi->ErrorCode |= HAL_DSI_ERROR_RX;              /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    if ((ErrorStatus1 & DSI_ERROR_ECC_MASK) != 0U)      /* 条件判断 */
    {                                                   /* 代码块开始 */
      hdsi->ErrorCode |= HAL_DSI_ERROR_ECC;             /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    if ((ErrorStatus1 & DSI_ERROR_CRC_MASK) != 0U)      /* 条件判断 */
    {                                                   /* 代码块开始 */
      hdsi->ErrorCode |= HAL_DSI_ERROR_CRC;             /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    if ((ErrorStatus1 & DSI_ERROR_PSE_MASK) != 0U)      /* 条件判断 */
    {                                                   /* 代码块开始 */
      hdsi->ErrorCode |= HAL_DSI_ERROR_PSE;             /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    if ((ErrorStatus1 & DSI_ERROR_EOT_MASK) != 0U)      /* 条件判断 */
    {                                                   /* 代码块开始 */
      hdsi->ErrorCode |= HAL_DSI_ERROR_EOT;             /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    if ((ErrorStatus1 & DSI_ERROR_OVF_MASK) != 0U)      /* 条件判断 */
    {                                                   /* 代码块开始 */
      hdsi->ErrorCode |= HAL_DSI_ERROR_OVF;             /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    if ((ErrorStatus1 & DSI_ERROR_GEN_MASK) != 0U)      /* 条件判断 */
    {                                                   /* 代码块开始 */
      hdsi->ErrorCode |= HAL_DSI_ERROR_GEN;             /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Check only selected errors */
    if (hdsi->ErrorCode != HAL_DSI_ERROR_NONE)          /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* DSI error interrupt callback */
#if (USE_HAL_DSI_REGISTER_CALLBACKS == 1)               /* 条件编译 */
      /*Call registered Error callback */
      hdsi->ErrorCallback(hdsi);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
      /*Call Legacy Error callback */
      HAL_DSI_ErrorCallback(hdsi);                      /* HAL库函数调用 */
#endif /* USE_HAL_DSI_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Tearing Effect DSI callback.
  * @param  hdsi  pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @retval None
  */
__weak void HAL_DSI_TearingEffectCallback(DSI_HandleTypeDef *hdsi)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdsi);                                         /* 执行语句 */
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_DSI_TearingEffectCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  End of Refresh DSI callback.
  * @param  hdsi  pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @retval None
  */
__weak void HAL_DSI_EndOfRefreshCallback(DSI_HandleTypeDef *hdsi)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdsi);                                         /* 执行语句 */
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_DSI_EndOfRefreshCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Operation Error DSI callback.
  * @param  hdsi  pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @retval None
  */
__weak void HAL_DSI_ErrorCallback(DSI_HandleTypeDef *hdsi)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdsi);                                         /* 执行语句 */
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_DSI_ErrorCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup DSI_Group3 Peripheral Control functions
  *  @brief    Peripheral Control functions
  *
@verbatim
 ===============================================================================
                    ##### Peripheral Control functions #####
 ===============================================================================
    [..]  This section provides functions allowing to:
      (+) Configure the Generic interface read-back Virtual Channel ID
      (+) Select video mode and configure the corresponding parameters
      (+) Configure command transmission mode: High-speed or Low-power
      (+) Configure the flow control
      (+) Configure the DSI PHY timer
      (+) Configure the DSI HOST timeout
      (+) Configure the DSI HOST timeout
      (+) Start/Stop the DSI module
      (+) Refresh the display in command mode
      (+) Controls the display color mode in Video mode
      (+) Control the display shutdown in Video mode
      (+) write short DCS or short Generic command
      (+) write long DCS or long Generic command
      (+) Read command (DCS or generic)
      (+) Enter/Exit the Ultra Low Power Mode on data only (D-PHY PLL running)
      (+) Enter/Exit the Ultra Low Power Mode on data only and clock (D-PHY PLL turned off)
      (+) Start/Stop test pattern generation
      (+) Slew-Rate And Delay Tuning
      (+) Low-Power Reception Filter Tuning
      (+) Activate an additional current path on all lanes to meet the SDDTx parameter
      (+) Custom lane pins configuration
      (+) Set custom timing for the PHY
      (+) Force the Clock/Data Lane in TX Stop Mode
      (+) Force LP Receiver in Low-Power Mode
      (+) Force Data Lanes in RX Mode after a BTA
      (+) Enable a pull-down on the lanes to prevent from floating states when unused
      (+) Switch off the contention detection on data lanes

@endverbatim
  * @{
  */

/**
  * @brief  Configure the Generic interface read-back Virtual Channel ID.
  * @param  hdsi  pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @param  VirtualChannelID  Virtual channel ID
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DSI_SetGenericVCID(DSI_HandleTypeDef *hdsi, uint32_t VirtualChannelID)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Process locked */
  __HAL_LOCK(hdsi);                                     /* HAL辅助宏/函数 */

  /* Update the GVCID register */
  hdsi->Instance->GVCIDR &= ~DSI_GVCIDR_VCID;           /* 赋值或初始化 */
  hdsi->Instance->GVCIDR |= VirtualChannelID;           /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hdsi);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Select video mode and configure the corresponding parameters
  * @param  hdsi  pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @param  VidCfg pointer to a DSI_VidCfgTypeDef structure that contains
  *                the DSI video mode configuration parameters
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DSI_ConfigVideoMode(DSI_HandleTypeDef *hdsi, DSI_VidCfgTypeDef *VidCfg)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Process locked */
  __HAL_LOCK(hdsi);                                     /* HAL辅助宏/函数 */

  /* Check the parameters */
  assert_param(IS_DSI_COLOR_CODING(VidCfg->ColorCoding));  /* 执行语句 */
  assert_param(IS_DSI_VIDEO_MODE_TYPE(VidCfg->Mode));   /* 执行语句 */
  assert_param(IS_DSI_LP_COMMAND(VidCfg->LPCommandEnable));  /* 执行语句 */
  assert_param(IS_DSI_LP_HFP(VidCfg->LPHorizontalFrontPorchEnable));  /* 执行语句 */
  assert_param(IS_DSI_LP_HBP(VidCfg->LPHorizontalBackPorchEnable));  /* 执行语句 */
  assert_param(IS_DSI_LP_VACTIVE(VidCfg->LPVerticalActiveEnable));  /* 执行语句 */
  assert_param(IS_DSI_LP_VFP(VidCfg->LPVerticalFrontPorchEnable));  /* 执行语句 */
  assert_param(IS_DSI_LP_VBP(VidCfg->LPVerticalBackPorchEnable));  /* 执行语句 */
  assert_param(IS_DSI_LP_VSYNC(VidCfg->LPVerticalSyncActiveEnable));  /* 执行语句 */
  assert_param(IS_DSI_FBTAA(VidCfg->FrameBTAAcknowledgeEnable));  /* 执行语句 */
  assert_param(IS_DSI_DE_POLARITY(VidCfg->DEPolarity));  /* 执行语句 */
  assert_param(IS_DSI_VSYNC_POLARITY(VidCfg->VSPolarity));  /* 执行语句 */
  assert_param(IS_DSI_HSYNC_POLARITY(VidCfg->HSPolarity));  /* 执行语句 */
  /* Check the LooselyPacked variant only in 18-bit mode */
  if (VidCfg->ColorCoding == DSI_RGB666)                /* 条件判断 */
  {                                                     /* 代码块开始 */
    assert_param(IS_DSI_LOOSELY_PACKED(VidCfg->LooselyPacked));  /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Select video mode by resetting CMDM and DSIM bits */
  hdsi->Instance->MCR &= ~DSI_MCR_CMDM;                 /* 赋值或初始化 */
  hdsi->Instance->WCFGR &= ~DSI_WCFGR_DSIM;             /* 赋值或初始化 */

  /* Configure the video mode transmission type */
  hdsi->Instance->VMCR &= ~DSI_VMCR_VMT;                /* 赋值或初始化 */
  hdsi->Instance->VMCR |= VidCfg->Mode;                 /* 赋值或初始化 */

  /* Configure the video packet size */
  hdsi->Instance->VPCR &= ~DSI_VPCR_VPSIZE;             /* 赋值或初始化 */
  hdsi->Instance->VPCR |= VidCfg->PacketSize;           /* 赋值或初始化 */

  /* Set the chunks number to be transmitted through the DSI link */
  hdsi->Instance->VCCR &= ~DSI_VCCR_NUMC;               /* 赋值或初始化 */
  hdsi->Instance->VCCR |= VidCfg->NumberOfChunks;       /* 赋值或初始化 */

  /* Set the size of the null packet */
  hdsi->Instance->VNPCR &= ~DSI_VNPCR_NPSIZE;           /* 赋值或初始化 */
  hdsi->Instance->VNPCR |= VidCfg->NullPacketSize;      /* 赋值或初始化 */

  /* Select the virtual channel for the LTDC interface traffic */
  hdsi->Instance->LVCIDR &= ~DSI_LVCIDR_VCID;           /* 赋值或初始化 */
  hdsi->Instance->LVCIDR |= VidCfg->VirtualChannelID;   /* 赋值或初始化 */

  /* Configure the polarity of control signals */
  hdsi->Instance->LPCR &= ~(DSI_LPCR_DEP | DSI_LPCR_VSP | DSI_LPCR_HSP);  /* 赋值或初始化 */
  hdsi->Instance->LPCR |= (VidCfg->DEPolarity | VidCfg->VSPolarity | VidCfg->HSPolarity);  /* 赋值或初始化 */

  /* Select the color coding for the host */
  hdsi->Instance->LCOLCR &= ~DSI_LCOLCR_COLC;           /* 赋值或初始化 */
  hdsi->Instance->LCOLCR |= VidCfg->ColorCoding;        /* 赋值或初始化 */

  /* Select the color coding for the wrapper */
  hdsi->Instance->WCFGR &= ~DSI_WCFGR_COLMUX;           /* 赋值或初始化 */
  hdsi->Instance->WCFGR |= ((VidCfg->ColorCoding) << 1U);  /* 赋值或初始化 */

  /* Enable/disable the loosely packed variant to 18-bit configuration */
  if (VidCfg->ColorCoding == DSI_RGB666)                /* 条件判断 */
  {                                                     /* 代码块开始 */
    hdsi->Instance->LCOLCR &= ~DSI_LCOLCR_LPE;          /* 赋值或初始化 */
    hdsi->Instance->LCOLCR |= VidCfg->LooselyPacked;    /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Set the Horizontal Synchronization Active (HSA) in lane byte clock cycles */
  hdsi->Instance->VHSACR &= ~DSI_VHSACR_HSA;            /* 赋值或初始化 */
  hdsi->Instance->VHSACR |= VidCfg->HorizontalSyncActive;  /* 赋值或初始化 */

  /* Set the Horizontal Back Porch (HBP) in lane byte clock cycles */
  hdsi->Instance->VHBPCR &= ~DSI_VHBPCR_HBP;            /* 赋值或初始化 */
  hdsi->Instance->VHBPCR |= VidCfg->HorizontalBackPorch;  /* 赋值或初始化 */

  /* Set the total line time (HLINE=HSA+HBP+HACT+HFP) in lane byte clock cycles */
  hdsi->Instance->VLCR &= ~DSI_VLCR_HLINE;              /* 赋值或初始化 */
  hdsi->Instance->VLCR |= VidCfg->HorizontalLine;       /* 赋值或初始化 */

  /* Set the Vertical Synchronization Active (VSA) */
  hdsi->Instance->VVSACR &= ~DSI_VVSACR_VSA;            /* 赋值或初始化 */
  hdsi->Instance->VVSACR |= VidCfg->VerticalSyncActive;  /* 赋值或初始化 */

  /* Set the Vertical Back Porch (VBP)*/
  hdsi->Instance->VVBPCR &= ~DSI_VVBPCR_VBP;            /* 赋值或初始化 */
  hdsi->Instance->VVBPCR |= VidCfg->VerticalBackPorch;  /* 赋值或初始化 */

  /* Set the Vertical Front Porch (VFP)*/
  hdsi->Instance->VVFPCR &= ~DSI_VVFPCR_VFP;            /* 赋值或初始化 */
  hdsi->Instance->VVFPCR |= VidCfg->VerticalFrontPorch;  /* 赋值或初始化 */

  /* Set the Vertical Active period*/
  hdsi->Instance->VVACR &= ~DSI_VVACR_VA;               /* 赋值或初始化 */
  hdsi->Instance->VVACR |= VidCfg->VerticalActive;      /* 赋值或初始化 */

  /* Configure the command transmission mode */
  hdsi->Instance->VMCR &= ~DSI_VMCR_LPCE;               /* 赋值或初始化 */
  hdsi->Instance->VMCR |= VidCfg->LPCommandEnable;      /* 赋值或初始化 */

  /* Low power largest packet size */
  hdsi->Instance->LPMCR &= ~DSI_LPMCR_LPSIZE;           /* 赋值或初始化 */
  hdsi->Instance->LPMCR |= ((VidCfg->LPLargestPacketSize) << 16U);  /* 赋值或初始化 */

  /* Low power VACT largest packet size */
  hdsi->Instance->LPMCR &= ~DSI_LPMCR_VLPSIZE;          /* 赋值或初始化 */
  hdsi->Instance->LPMCR |= VidCfg->LPVACTLargestPacketSize;  /* 赋值或初始化 */

  /* Enable LP transition in HFP period */
  hdsi->Instance->VMCR &= ~DSI_VMCR_LPHFPE;             /* 赋值或初始化 */
  hdsi->Instance->VMCR |= VidCfg->LPHorizontalFrontPorchEnable;  /* 赋值或初始化 */

  /* Enable LP transition in HBP period */
  hdsi->Instance->VMCR &= ~DSI_VMCR_LPHBPE;             /* 赋值或初始化 */
  hdsi->Instance->VMCR |= VidCfg->LPHorizontalBackPorchEnable;  /* 赋值或初始化 */

  /* Enable LP transition in VACT period */
  hdsi->Instance->VMCR &= ~DSI_VMCR_LPVAE;              /* 赋值或初始化 */
  hdsi->Instance->VMCR |= VidCfg->LPVerticalActiveEnable;  /* 赋值或初始化 */

  /* Enable LP transition in VFP period */
  hdsi->Instance->VMCR &= ~DSI_VMCR_LPVFPE;             /* 赋值或初始化 */
  hdsi->Instance->VMCR |= VidCfg->LPVerticalFrontPorchEnable;  /* 赋值或初始化 */

  /* Enable LP transition in VBP period */
  hdsi->Instance->VMCR &= ~DSI_VMCR_LPVBPE;             /* 赋值或初始化 */
  hdsi->Instance->VMCR |= VidCfg->LPVerticalBackPorchEnable;  /* 赋值或初始化 */

  /* Enable LP transition in vertical sync period */
  hdsi->Instance->VMCR &= ~DSI_VMCR_LPVSAE;             /* 赋值或初始化 */
  hdsi->Instance->VMCR |= VidCfg->LPVerticalSyncActiveEnable;  /* 赋值或初始化 */

  /* Enable the request for an acknowledge response at the end of a frame */
  hdsi->Instance->VMCR &= ~DSI_VMCR_FBTAAE;             /* 赋值或初始化 */
  hdsi->Instance->VMCR |= VidCfg->FrameBTAAcknowledgeEnable;  /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hdsi);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Select adapted command mode and configure the corresponding parameters
  * @param  hdsi  pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @param  CmdCfg  pointer to a DSI_CmdCfgTypeDef structure that contains
  *                 the DSI command mode configuration parameters
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DSI_ConfigAdaptedCommandMode(DSI_HandleTypeDef *hdsi, DSI_CmdCfgTypeDef *CmdCfg)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Process locked */
  __HAL_LOCK(hdsi);                                     /* HAL辅助宏/函数 */

  /* Check the parameters */
  assert_param(IS_DSI_COLOR_CODING(CmdCfg->ColorCoding));  /* 执行语句 */
  assert_param(IS_DSI_TE_SOURCE(CmdCfg->TearingEffectSource));  /* 执行语句 */
  assert_param(IS_DSI_TE_POLARITY(CmdCfg->TearingEffectPolarity));  /* 执行语句 */
  assert_param(IS_DSI_AUTOMATIC_REFRESH(CmdCfg->AutomaticRefresh));  /* 执行语句 */
  assert_param(IS_DSI_VS_POLARITY(CmdCfg->VSyncPol));   /* 执行语句 */
  assert_param(IS_DSI_TE_ACK_REQUEST(CmdCfg->TEAcknowledgeRequest));  /* 执行语句 */
  assert_param(IS_DSI_DE_POLARITY(CmdCfg->DEPolarity));  /* 执行语句 */
  assert_param(IS_DSI_VSYNC_POLARITY(CmdCfg->VSPolarity));  /* 执行语句 */
  assert_param(IS_DSI_HSYNC_POLARITY(CmdCfg->HSPolarity));  /* 执行语句 */

  /* Select command mode by setting CMDM and DSIM bits */
  hdsi->Instance->MCR |= DSI_MCR_CMDM;                  /* 赋值或初始化 */
  hdsi->Instance->WCFGR &= ~DSI_WCFGR_DSIM;             /* 赋值或初始化 */
  hdsi->Instance->WCFGR |= DSI_WCFGR_DSIM;              /* 赋值或初始化 */

  /* Select the virtual channel for the LTDC interface traffic */
  hdsi->Instance->LVCIDR &= ~DSI_LVCIDR_VCID;           /* 赋值或初始化 */
  hdsi->Instance->LVCIDR |= CmdCfg->VirtualChannelID;   /* 赋值或初始化 */

  /* Configure the polarity of control signals */
  hdsi->Instance->LPCR &= ~(DSI_LPCR_DEP | DSI_LPCR_VSP | DSI_LPCR_HSP);  /* 赋值或初始化 */
  hdsi->Instance->LPCR |= (CmdCfg->DEPolarity | CmdCfg->VSPolarity | CmdCfg->HSPolarity);  /* 赋值或初始化 */

  /* Select the color coding for the host */
  hdsi->Instance->LCOLCR &= ~DSI_LCOLCR_COLC;           /* 赋值或初始化 */
  hdsi->Instance->LCOLCR |= CmdCfg->ColorCoding;        /* 赋值或初始化 */

  /* Select the color coding for the wrapper */
  hdsi->Instance->WCFGR &= ~DSI_WCFGR_COLMUX;           /* 赋值或初始化 */
  hdsi->Instance->WCFGR |= ((CmdCfg->ColorCoding) << 1U);  /* 赋值或初始化 */

  /* Configure the maximum allowed size for write memory command */
  hdsi->Instance->LCCR &= ~DSI_LCCR_CMDSIZE;            /* 赋值或初始化 */
  hdsi->Instance->LCCR |= CmdCfg->CommandSize;          /* 赋值或初始化 */

  /* Configure the tearing effect source and polarity and select the refresh mode */
  hdsi->Instance->WCFGR &= ~(DSI_WCFGR_TESRC | DSI_WCFGR_TEPOL | DSI_WCFGR_AR | DSI_WCFGR_VSPOL);  /* 赋值或初始化 */
  hdsi->Instance->WCFGR |= (CmdCfg->TearingEffectSource | CmdCfg->TearingEffectPolarity | CmdCfg->AutomaticRefresh |  /* 源代码 */
                            CmdCfg->VSyncPol);          /* 执行语句 */

  /* Configure the tearing effect acknowledge request */
  hdsi->Instance->CMCR &= ~DSI_CMCR_TEARE;              /* 赋值或初始化 */
  hdsi->Instance->CMCR |= CmdCfg->TEAcknowledgeRequest;  /* 赋值或初始化 */

  /* Enable the Tearing Effect interrupt */
  __HAL_DSI_ENABLE_IT(hdsi, DSI_IT_TE);                 /* HAL辅助宏/函数 */

  /* Enable the End of Refresh interrupt */
  __HAL_DSI_ENABLE_IT(hdsi, DSI_IT_ER);                 /* HAL辅助宏/函数 */

  /* Process unlocked */
  __HAL_UNLOCK(hdsi);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configure command transmission mode: High-speed or Low-power
  *         and enable/disable acknowledge request after packet transmission
  * @param  hdsi  pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @param  LPCmd  pointer to a DSI_LPCmdTypeDef structure that contains
  *                the DSI command transmission mode configuration parameters
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DSI_ConfigCommand(DSI_HandleTypeDef *hdsi, DSI_LPCmdTypeDef *LPCmd)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Process locked */
  __HAL_LOCK(hdsi);                                     /* HAL辅助宏/函数 */

  assert_param(IS_DSI_LP_GSW0P(LPCmd->LPGenShortWriteNoP));  /* 执行语句 */
  assert_param(IS_DSI_LP_GSW1P(LPCmd->LPGenShortWriteOneP));  /* 执行语句 */
  assert_param(IS_DSI_LP_GSW2P(LPCmd->LPGenShortWriteTwoP));  /* 执行语句 */
  assert_param(IS_DSI_LP_GSR0P(LPCmd->LPGenShortReadNoP));  /* 执行语句 */
  assert_param(IS_DSI_LP_GSR1P(LPCmd->LPGenShortReadOneP));  /* 执行语句 */
  assert_param(IS_DSI_LP_GSR2P(LPCmd->LPGenShortReadTwoP));  /* 执行语句 */
  assert_param(IS_DSI_LP_GLW(LPCmd->LPGenLongWrite));   /* 执行语句 */
  assert_param(IS_DSI_LP_DSW0P(LPCmd->LPDcsShortWriteNoP));  /* 执行语句 */
  assert_param(IS_DSI_LP_DSW1P(LPCmd->LPDcsShortWriteOneP));  /* 执行语句 */
  assert_param(IS_DSI_LP_DSR0P(LPCmd->LPDcsShortReadNoP));  /* 执行语句 */
  assert_param(IS_DSI_LP_DLW(LPCmd->LPDcsLongWrite));   /* 执行语句 */
  assert_param(IS_DSI_LP_MRDP(LPCmd->LPMaxReadPacket));  /* 执行语句 */
  assert_param(IS_DSI_ACK_REQUEST(LPCmd->AcknowledgeRequest));  /* 执行语句 */

  /* Select High-speed or Low-power for command transmission */
  hdsi->Instance->CMCR &= ~(DSI_CMCR_GSW0TX | \
                            DSI_CMCR_GSW1TX | \
                            DSI_CMCR_GSW2TX | \
                            DSI_CMCR_GSR0TX | \
                            DSI_CMCR_GSR1TX | \
                            DSI_CMCR_GSR2TX | \
                            DSI_CMCR_GLWTX  | \
                            DSI_CMCR_DSW0TX | \
                            DSI_CMCR_DSW1TX | \
                            DSI_CMCR_DSR0TX | \
                            DSI_CMCR_DLWTX  | \
                            DSI_CMCR_MRDPS);            /* 执行语句 */
  hdsi->Instance->CMCR |= (LPCmd->LPGenShortWriteNoP  | \
                           LPCmd->LPGenShortWriteOneP | \
                           LPCmd->LPGenShortWriteTwoP | \
                           LPCmd->LPGenShortReadNoP   | \
                           LPCmd->LPGenShortReadOneP  | \
                           LPCmd->LPGenShortReadTwoP  | \
                           LPCmd->LPGenLongWrite      | \
                           LPCmd->LPDcsShortWriteNoP  | \
                           LPCmd->LPDcsShortWriteOneP | \
                           LPCmd->LPDcsShortReadNoP   | \
                           LPCmd->LPDcsLongWrite      | \
                           LPCmd->LPMaxReadPacket);     /* 执行语句 */

  /* Configure the acknowledge request after each packet transmission */
  hdsi->Instance->CMCR &= ~DSI_CMCR_ARE;                /* 赋值或初始化 */
  hdsi->Instance->CMCR |= LPCmd->AcknowledgeRequest;    /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hdsi);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configure the flow control parameters
  * @param  hdsi  pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @param  FlowControl  flow control feature(s) to be enabled.
  *                      This parameter can be any combination of @arg DSI_FlowControl.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DSI_ConfigFlowControl(DSI_HandleTypeDef *hdsi, uint32_t FlowControl)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Process locked */
  __HAL_LOCK(hdsi);                                     /* HAL辅助宏/函数 */

  /* Check the parameters */
  assert_param(IS_DSI_FLOW_CONTROL(FlowControl));       /* 执行语句 */

  /* Set the DSI Host Protocol Configuration Register */
  hdsi->Instance->PCR &= ~DSI_FLOW_CONTROL_ALL;         /* 赋值或初始化 */
  hdsi->Instance->PCR |= FlowControl;                   /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hdsi);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configure the DSI PHY timer parameters
  * @param  hdsi  pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @param  PhyTimers  DSI_PHY_TimerTypeDef structure that contains
  *                    the DSI PHY timing parameters
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DSI_ConfigPhyTimer(DSI_HandleTypeDef *hdsi, DSI_PHY_TimerTypeDef *PhyTimers)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t maxTime;                                     /* 32位无符号变量/函数 */
  /* Process locked */
  __HAL_LOCK(hdsi);                                     /* HAL辅助宏/函数 */

  maxTime = (PhyTimers->ClockLaneLP2HSTime > PhyTimers->ClockLaneHS2LPTime) ? PhyTimers->ClockLaneLP2HSTime :  /* 标签 */
            PhyTimers->ClockLaneHS2LPTime;              /* 执行语句 */

  /* Clock lane timer configuration */

  /* In Automatic Clock Lane control mode, the DSI Host can turn off the clock lane between two
     High-Speed transmission.
     To do so, the DSI Host calculates the time required for the clock lane to change from HighSpeed
     to Low-Power and from Low-Power to High-Speed.
     This timings are configured by the HS2LP_TIME and LP2HS_TIME in the DSI Host Clock Lane Timer Configuration Register (DSI_CLTCR).
     But the DSI Host is not calculating LP2HS_TIME + HS2LP_TIME but 2 x HS2LP_TIME.

     Workaround : Configure HS2LP_TIME and LP2HS_TIME with the same value being the max of HS2LP_TIME or LP2HS_TIME.
    */
  hdsi->Instance->CLTCR &= ~(DSI_CLTCR_LP2HS_TIME | DSI_CLTCR_HS2LP_TIME);  /* 赋值或初始化 */
  hdsi->Instance->CLTCR |= (maxTime | ((maxTime) << 16U));  /* 赋值或初始化 */

  /* Data lane timer configuration */
  hdsi->Instance->DLTCR &= ~(DSI_DLTCR_MRD_TIME | DSI_DLTCR_LP2HS_TIME | DSI_DLTCR_HS2LP_TIME);  /* 赋值或初始化 */
  hdsi->Instance->DLTCR |= (PhyTimers->DataLaneMaxReadTime | ((PhyTimers->DataLaneLP2HSTime) << 16U) | ((  /* 源代码 */
                              PhyTimers->DataLaneHS2LPTime) << 24U));  /* 执行语句 */

  /* Configure the wait period to request HS transmission after a stop state */
  hdsi->Instance->PCONFR &= ~DSI_PCONFR_SW_TIME;        /* 赋值或初始化 */
  hdsi->Instance->PCONFR |= ((PhyTimers->StopWaitTime) << 8U);  /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hdsi);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configure the DSI HOST timeout parameters
  * @param  hdsi  pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @param  HostTimeouts  DSI_HOST_TimeoutTypeDef structure that contains
  *                       the DSI host timeout parameters
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DSI_ConfigHostTimeouts(DSI_HandleTypeDef *hdsi, DSI_HOST_TimeoutTypeDef *HostTimeouts)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Process locked */
  __HAL_LOCK(hdsi);                                     /* HAL辅助宏/函数 */

  /* Set the timeout clock division factor */
  hdsi->Instance->CCR &= ~DSI_CCR_TOCKDIV;              /* 赋值或初始化 */
  hdsi->Instance->CCR |= ((HostTimeouts->TimeoutCkdiv) << 8U);  /* 赋值或初始化 */

  /* High-speed transmission timeout */
  hdsi->Instance->TCCR[0U] &= ~DSI_TCCR0_HSTX_TOCNT;    /* 赋值或初始化 */
  hdsi->Instance->TCCR[0U] |= ((HostTimeouts->HighSpeedTransmissionTimeout) << 16U);  /* 赋值或初始化 */

  /* Low-power reception timeout */
  hdsi->Instance->TCCR[0U] &= ~DSI_TCCR0_LPRX_TOCNT;    /* 赋值或初始化 */
  hdsi->Instance->TCCR[0U] |= HostTimeouts->LowPowerReceptionTimeout;  /* 赋值或初始化 */

  /* High-speed read timeout */
  hdsi->Instance->TCCR[1U] &= ~DSI_TCCR1_HSRD_TOCNT;    /* 赋值或初始化 */
  hdsi->Instance->TCCR[1U] |= HostTimeouts->HighSpeedReadTimeout;  /* 赋值或初始化 */

  /* Low-power read timeout */
  hdsi->Instance->TCCR[2U] &= ~DSI_TCCR2_LPRD_TOCNT;    /* 赋值或初始化 */
  hdsi->Instance->TCCR[2U] |= HostTimeouts->LowPowerReadTimeout;  /* 赋值或初始化 */

  /* High-speed write timeout */
  hdsi->Instance->TCCR[3U] &= ~DSI_TCCR3_HSWR_TOCNT;    /* 赋值或初始化 */
  hdsi->Instance->TCCR[3U] |= HostTimeouts->HighSpeedWriteTimeout;  /* 赋值或初始化 */

  /* High-speed write presp mode */
  hdsi->Instance->TCCR[3U] &= ~DSI_TCCR3_PM;            /* 赋值或初始化 */
  hdsi->Instance->TCCR[3U] |= HostTimeouts->HighSpeedWritePrespMode;  /* 赋值或初始化 */

  /* Low-speed write timeout */
  hdsi->Instance->TCCR[4U] &= ~DSI_TCCR4_LPWR_TOCNT;    /* 赋值或初始化 */
  hdsi->Instance->TCCR[4U] |= HostTimeouts->LowPowerWriteTimeout;  /* 赋值或初始化 */

  /* BTA timeout */
  hdsi->Instance->TCCR[5U] &= ~DSI_TCCR5_BTA_TOCNT;     /* 赋值或初始化 */
  hdsi->Instance->TCCR[5U] |= HostTimeouts->BTATimeout;  /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hdsi);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Start the DSI module
  * @param  hdsi  pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DSI_Start(DSI_HandleTypeDef *hdsi)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Process locked */
  __HAL_LOCK(hdsi);                                     /* HAL辅助宏/函数 */

  /* Enable the DSI host */
  __HAL_DSI_ENABLE(hdsi);                               /* HAL辅助宏/函数 */

  /* Enable the DSI wrapper */
  __HAL_DSI_WRAPPER_ENABLE(hdsi);                       /* HAL辅助宏/函数 */

  /* Process unlocked */
  __HAL_UNLOCK(hdsi);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Stop the DSI module
  * @param  hdsi  pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DSI_Stop(DSI_HandleTypeDef *hdsi)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Process locked */
  __HAL_LOCK(hdsi);                                     /* HAL辅助宏/函数 */

  /* Disable the DSI host */
  __HAL_DSI_DISABLE(hdsi);                              /* HAL辅助宏/函数 */

  /* Disable the DSI wrapper */
  __HAL_DSI_WRAPPER_DISABLE(hdsi);                      /* HAL辅助宏/函数 */

  /* Process unlocked */
  __HAL_UNLOCK(hdsi);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Refresh the display in command mode
  * @param  hdsi  pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DSI_Refresh(DSI_HandleTypeDef *hdsi)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Process locked */
  __HAL_LOCK(hdsi);                                     /* HAL辅助宏/函数 */

  /* Update the display */
  hdsi->Instance->WCR |= DSI_WCR_LTDCEN;                /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hdsi);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Controls the display color mode in Video mode
  * @param  hdsi  pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @param  ColorMode  Color mode (full or 8-colors).
  *                    This parameter can be any value of @arg DSI_Color_Mode
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DSI_ColorMode(DSI_HandleTypeDef *hdsi, uint32_t ColorMode)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Process locked */
  __HAL_LOCK(hdsi);                                     /* HAL辅助宏/函数 */

  /* Check the parameters */
  assert_param(IS_DSI_COLOR_MODE(ColorMode));           /* 执行语句 */

  /* Update the display color mode */
  hdsi->Instance->WCR &= ~DSI_WCR_COLM;                 /* 赋值或初始化 */
  hdsi->Instance->WCR |= ColorMode;                     /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hdsi);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Control the display shutdown in Video mode
  * @param  hdsi  pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @param  Shutdown  Shut-down (Display-ON or Display-OFF).
  *                   This parameter can be any value of @arg DSI_ShutDown
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DSI_Shutdown(DSI_HandleTypeDef *hdsi, uint32_t Shutdown)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Process locked */
  __HAL_LOCK(hdsi);                                     /* HAL辅助宏/函数 */

  /* Check the parameters */
  assert_param(IS_DSI_SHUT_DOWN(Shutdown));             /* 执行语句 */

  /* Update the display Shutdown */
  hdsi->Instance->WCR &= ~DSI_WCR_SHTDN;                /* 赋值或初始化 */
  hdsi->Instance->WCR |= Shutdown;                      /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hdsi);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  write short DCS or short Generic command
  * @param  hdsi  pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @param  ChannelID  Virtual channel ID.
  * @param  Mode  DSI short packet data type.
  *               This parameter can be any value of @arg DSI_SHORT_WRITE_PKT_Data_Type.
  * @param  Param1  DSC command or first generic parameter.
  *                 This parameter can be any value of @arg DSI_DCS_Command or a
  *                 generic command code.
  * @param  Param2  DSC parameter or second generic parameter.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DSI_ShortWrite(DSI_HandleTypeDef *hdsi,  /* HAL状态类型函数 */
                                     uint32_t ChannelID,  /* 32位无符号变量/函数 */
                                     uint32_t Mode,     /* 32位无符号变量/函数 */
                                     uint32_t Param1,   /* 32位无符号变量/函数 */
                                     uint32_t Param2)   /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status;                             /* HAL状态类型函数 */
  /* Check the parameters */
  assert_param(IS_DSI_SHORT_WRITE_PACKET_TYPE(Mode));   /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hdsi);                                     /* HAL辅助宏/函数 */

   status = DSI_ShortWrite(hdsi, ChannelID, Mode, Param1, Param2);  /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hdsi);                                   /* HAL辅助宏/函数 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  write long DCS or long Generic command
  * @param  hdsi  pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @param  ChannelID  Virtual channel ID.
  * @param  Mode  DSI long packet data type.
  *               This parameter can be any value of @arg DSI_LONG_WRITE_PKT_Data_Type.
  * @param  NbParams  Number of parameters.
  * @param  Param1  DSC command or first generic parameter.
  *                 This parameter can be any value of @arg DSI_DCS_Command or a
  *                 generic command code
  * @param  ParametersTable  Pointer to parameter values table.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DSI_LongWrite(DSI_HandleTypeDef *hdsi,  /* HAL状态类型函数 */
                                    uint32_t ChannelID,  /* 32位无符号变量/函数 */
                                    uint32_t Mode,      /* 32位无符号变量/函数 */
                                    uint32_t NbParams,  /* 32位无符号变量/函数 */
                                    uint32_t Param1,    /* 32位无符号变量/函数 */
                                    uint8_t *ParametersTable)  /* 8位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t uicounter, nbBytes, count;                   /* 32位无符号变量/函数 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */
  uint32_t fifoword;                                    /* 32位无符号变量/函数 */
  uint8_t *pparams = ParametersTable;                   /* 8位无符号变量/函数 */

  /* Process locked */
  __HAL_LOCK(hdsi);                                     /* HAL辅助宏/函数 */

  /* Check the parameters */
  assert_param(IS_DSI_LONG_WRITE_PACKET_TYPE(Mode));    /* 执行语句 */

  /* Get tick */
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  /* Wait for Command FIFO Empty */
  while ((hdsi->Instance->GPSR & DSI_GPSR_CMDFE) == 0U)  /* while循环 */
  {                                                     /* 代码块开始 */
    /* Check for the Timeout */
    if ((HAL_GetTick() - tickstart) > DSI_TIMEOUT_VALUE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Process Unlocked */
      __HAL_UNLOCK(hdsi);                               /* HAL辅助宏/函数 */

      return HAL_TIMEOUT;                               /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Set the DCS code on payload byte 1, and the other parameters on the write FIFO command*/
  fifoword = Param1;                                    /* 赋值或初始化 */
  nbBytes = (NbParams < 3U) ? NbParams : 3U;            /* 赋值或初始化 */

  for (count = 0U; count < nbBytes; count++)            /* for循环 */
  {                                                     /* 代码块开始 */
    fifoword |= (((uint32_t)(*(pparams + count))) << (8U + (8U * count)));  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  hdsi->Instance->GPDR = fifoword;                      /* 赋值或初始化 */

  uicounter = NbParams - nbBytes;                       /* 赋值或初始化 */
  pparams += nbBytes;                                   /* 赋值或初始化 */
  /* Set the Next parameters on the write FIFO command*/
  while (uicounter != 0U)                               /* while循环 */
  {                                                     /* 代码块开始 */
    nbBytes = (uicounter < 4U) ? uicounter : 4U;        /* 赋值或初始化 */
    fifoword = 0U;                                      /* 赋值或初始化 */
    for (count = 0U; count < nbBytes; count++)          /* for循环 */
    {                                                   /* 代码块开始 */
      fifoword |= (((uint32_t)(*(pparams + count))) << (8U * count));  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    hdsi->Instance->GPDR = fifoword;                    /* 赋值或初始化 */

    uicounter -= nbBytes;                               /* 赋值或初始化 */
    pparams += nbBytes;                                 /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Configure the packet to send a long DCS command */
  DSI_ConfigPacketHeader(hdsi->Instance,                /* 列表续行 */
                         ChannelID,                     /* 列表续行 */
                         Mode,                          /* 列表续行 */
                         ((NbParams + 1U) & 0x00FFU),   /* 列表续行 */
                         (((NbParams + 1U) & 0xFF00U) >> 8U));  /* 执行语句 */

  /* Process unlocked */
  __HAL_UNLOCK(hdsi);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Read command (DCS or generic)
  * @param  hdsi  pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @param  ChannelNbr  Virtual channel ID
  * @param  Array pointer to a buffer to store the payload of a read back operation.
  * @param  Size  Data size to be read (in byte).
  * @param  Mode  DSI read packet data type.
  *               This parameter can be any value of @arg DSI_SHORT_READ_PKT_Data_Type.
  * @param  DCSCmd  DCS get/read command.
  * @param  ParametersTable  Pointer to parameter values table.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DSI_Read(DSI_HandleTypeDef *hdsi,  /* HAL状态类型函数 */
                               uint32_t ChannelNbr,     /* 32位无符号变量/函数 */
                               uint8_t *Array,          /* 8位无符号变量/函数 */
                               uint32_t Size,           /* 32位无符号变量/函数 */
                               uint32_t Mode,           /* 32位无符号变量/函数 */
                               uint32_t DCSCmd,         /* 32位无符号变量/函数 */
                               uint8_t *ParametersTable)  /* 8位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */
  uint8_t *pdata = Array;                               /* 8位无符号变量/函数 */
  uint32_t datasize = Size;                             /* 32位无符号变量/函数 */
  uint32_t fifoword;                                    /* 32位无符号变量/函数 */
  uint32_t nbbytes;                                     /* 32位无符号变量/函数 */
  uint32_t count;                                       /* 32位无符号变量/函数 */

  /* Process locked */
  __HAL_LOCK(hdsi);                                     /* HAL辅助宏/函数 */

  /* Check the parameters */
  assert_param(IS_DSI_READ_PACKET_TYPE(Mode));          /* 执行语句 */

  if (datasize > 2U)                                    /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* set max return packet size */
    if (DSI_ShortWrite(hdsi, ChannelNbr, DSI_MAX_RETURN_PKT_SIZE, ((datasize) & 0xFFU),  /* 条件判断 */
                           (((datasize) >> 8U) & 0xFFU)) != HAL_OK)  /* 源代码 */
    {                                                   /* 代码块开始 */
      /* Process Unlocked */
      __HAL_UNLOCK(hdsi);                               /* HAL辅助宏/函数 */

      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Configure the packet to read command */
  if (Mode == DSI_DCS_SHORT_PKT_READ)                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    DSI_ConfigPacketHeader(hdsi->Instance, ChannelNbr, Mode, DCSCmd, 0U);  /* 执行语句 */
  }                                                     /* 代码块结束 */
  else if (Mode == DSI_GEN_SHORT_PKT_READ_P0)           /* 否则如果 */
  {                                                     /* 代码块开始 */
    DSI_ConfigPacketHeader(hdsi->Instance, ChannelNbr, Mode, 0U, 0U);  /* 执行语句 */
  }                                                     /* 代码块结束 */
  else if (Mode == DSI_GEN_SHORT_PKT_READ_P1)           /* 否则如果 */
  {                                                     /* 代码块开始 */
    DSI_ConfigPacketHeader(hdsi->Instance, ChannelNbr, Mode, ParametersTable[0U], 0U);  /* 执行语句 */
  }                                                     /* 代码块结束 */
  else if (Mode == DSI_GEN_SHORT_PKT_READ_P2)           /* 否则如果 */
  {                                                     /* 代码块开始 */
    DSI_ConfigPacketHeader(hdsi->Instance, ChannelNbr, Mode, ParametersTable[0U], ParametersTable[1U]);  /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Process Unlocked */
    __HAL_UNLOCK(hdsi);                                 /* HAL辅助宏/函数 */

    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Get tick */
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  /* If DSI fifo is not empty, read requested bytes */
  while (((int32_t)(datasize)) > 0)                     /* while循环 */
  {                                                     /* 代码块开始 */
    if ((hdsi->Instance->GPSR & DSI_GPSR_PRDFE) == 0U)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      fifoword = hdsi->Instance->GPDR;                  /* 赋值或初始化 */
      nbbytes = (datasize < 4U) ? datasize : 4U;        /* 赋值或初始化 */

      for (count = 0U; count < nbbytes; count++)        /* for循环 */
      {                                                 /* 代码块开始 */
        *pdata = (uint8_t)(fifoword >> (8U * count));   /* 赋值或初始化 */
        pdata++;                                        /* 执行语句 */
        datasize--;                                     /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Check for the Timeout */
    if ((HAL_GetTick() - tickstart) > DSI_TIMEOUT_VALUE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Process Unlocked */
      __HAL_UNLOCK(hdsi);                               /* HAL辅助宏/函数 */

      return HAL_TIMEOUT;                               /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Process unlocked */
  __HAL_UNLOCK(hdsi);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Enter the ULPM (Ultra Low Power Mode) with the D-PHY PLL running
  *         (only data lanes are in ULPM)
  * @param  hdsi  pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DSI_EnterULPMData(DSI_HandleTypeDef *hdsi)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */

  /* Process locked */
  __HAL_LOCK(hdsi);                                     /* HAL辅助宏/函数 */

  /* ULPS Request on Data Lanes */
  hdsi->Instance->PUCR |= DSI_PUCR_URDL;                /* 赋值或初始化 */

  /* Get tick */
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  /* Wait until the D-PHY active lanes enter into ULPM */
  if ((hdsi->Instance->PCONFR & DSI_PCONFR_NL) == DSI_ONE_DATA_LANE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    while ((hdsi->Instance->PSR & DSI_PSR_UAN0) != 0U)  /* while循环 */
    {                                                   /* 代码块开始 */
      /* Check for the Timeout */
      if ((HAL_GetTick() - tickstart) > DSI_TIMEOUT_VALUE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Process Unlocked */
        __HAL_UNLOCK(hdsi);                             /* HAL辅助宏/函数 */

        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if ((hdsi->Instance->PCONFR & DSI_PCONFR_NL) == DSI_TWO_DATA_LANES)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    while ((hdsi->Instance->PSR & (DSI_PSR_UAN0 | DSI_PSR_UAN1)) != 0U)  /* while循环 */
    {                                                   /* 代码块开始 */
      /* Check for the Timeout */
      if ((HAL_GetTick() - tickstart) > DSI_TIMEOUT_VALUE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Process Unlocked */
        __HAL_UNLOCK(hdsi);                             /* HAL辅助宏/函数 */

        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Process unlocked */
    __HAL_UNLOCK(hdsi);                                 /* HAL辅助宏/函数 */

    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Process unlocked */
  __HAL_UNLOCK(hdsi);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Exit the ULPM (Ultra Low Power Mode) with the D-PHY PLL running
  *         (only data lanes are in ULPM)
  * @param  hdsi  pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DSI_ExitULPMData(DSI_HandleTypeDef *hdsi)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */

  /* Process locked */
  __HAL_LOCK(hdsi);                                     /* HAL辅助宏/函数 */

  /* Exit ULPS on Data Lanes */
  hdsi->Instance->PUCR |= DSI_PUCR_UEDL;                /* 赋值或初始化 */

  /* Get tick */
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  /* Wait until all active lanes exit ULPM */
  if ((hdsi->Instance->PCONFR & DSI_PCONFR_NL) == DSI_ONE_DATA_LANE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    while ((hdsi->Instance->PSR & DSI_PSR_UAN0) != DSI_PSR_UAN0)  /* while循环 */
    {                                                   /* 代码块开始 */
      /* Check for the Timeout */
      if ((HAL_GetTick() - tickstart) > DSI_TIMEOUT_VALUE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Process Unlocked */
        __HAL_UNLOCK(hdsi);                             /* HAL辅助宏/函数 */

        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if ((hdsi->Instance->PCONFR & DSI_PCONFR_NL) == DSI_TWO_DATA_LANES)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    while ((hdsi->Instance->PSR & (DSI_PSR_UAN0 | DSI_PSR_UAN1)) != (DSI_PSR_UAN0 | DSI_PSR_UAN1))  /* while循环 */
    {                                                   /* 代码块开始 */
      /* Check for the Timeout */
      if ((HAL_GetTick() - tickstart) > DSI_TIMEOUT_VALUE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Process Unlocked */
        __HAL_UNLOCK(hdsi);                             /* HAL辅助宏/函数 */

        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Process unlocked */
    __HAL_UNLOCK(hdsi);                                 /* HAL辅助宏/函数 */

    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* wait for 1 ms*/
  HAL_Delay(1U);                                        /* HAL库函数调用 */

  /* De-assert the ULPM requests and the ULPM exit bits */
  hdsi->Instance->PUCR = 0U;                            /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hdsi);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Enter the ULPM (Ultra Low Power Mode) with the D-PHY PLL turned off
  *         (both data and clock lanes are in ULPM)
  * @param  hdsi  pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DSI_EnterULPM(DSI_HandleTypeDef *hdsi)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */

  /* Process locked */
  __HAL_LOCK(hdsi);                                     /* HAL辅助宏/函数 */

  /* Clock lane configuration: no more HS request */
  hdsi->Instance->CLCR &= ~DSI_CLCR_DPCC;               /* 赋值或初始化 */

  /* Use system PLL as byte lane clock source before stopping DSIPHY clock source */
  __HAL_RCC_DSI_CONFIG(RCC_DSICLKSOURCE_PLLR);          /* HAL辅助宏/函数 */

  /* ULPS Request on Clock and Data Lanes */
  hdsi->Instance->PUCR |= (DSI_PUCR_URCL | DSI_PUCR_URDL);  /* 赋值或初始化 */

  /* Get tick */
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  /* Wait until all active lanes exit ULPM */
  if ((hdsi->Instance->PCONFR & DSI_PCONFR_NL) == DSI_ONE_DATA_LANE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    while ((hdsi->Instance->PSR & (DSI_PSR_UAN0 | DSI_PSR_UANC)) != 0U)  /* while循环 */
    {                                                   /* 代码块开始 */
      /* Check for the Timeout */
      if ((HAL_GetTick() - tickstart) > DSI_TIMEOUT_VALUE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Process Unlocked */
        __HAL_UNLOCK(hdsi);                             /* HAL辅助宏/函数 */

        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if ((hdsi->Instance->PCONFR & DSI_PCONFR_NL) == DSI_TWO_DATA_LANES)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    while ((hdsi->Instance->PSR & (DSI_PSR_UAN0 | DSI_PSR_UAN1 | DSI_PSR_UANC)) != 0U)  /* while循环 */
    {                                                   /* 代码块开始 */
      /* Check for the Timeout */
      if ((HAL_GetTick() - tickstart) > DSI_TIMEOUT_VALUE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Process Unlocked */
        __HAL_UNLOCK(hdsi);                             /* HAL辅助宏/函数 */

        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Process unlocked */
    __HAL_UNLOCK(hdsi);                                 /* HAL辅助宏/函数 */

    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Turn off the DSI PLL */
  __HAL_DSI_PLL_DISABLE(hdsi);                          /* HAL辅助宏/函数 */

  /* Process unlocked */
  __HAL_UNLOCK(hdsi);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Exit the ULPM (Ultra Low Power Mode) with the D-PHY PLL turned off
  *         (both data and clock lanes are in ULPM)
  * @param  hdsi  pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DSI_ExitULPM(DSI_HandleTypeDef *hdsi)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */

  /* Process locked */
  __HAL_LOCK(hdsi);                                     /* HAL辅助宏/函数 */

  /* Turn on the DSI PLL */
  __HAL_DSI_PLL_ENABLE(hdsi);                           /* HAL辅助宏/函数 */

  /* Get tick */
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  /* Wait for the lock of the PLL */
  while (__HAL_DSI_GET_FLAG(hdsi, DSI_FLAG_PLLLS) == 0U)  /* while循环 */
  {                                                     /* 代码块开始 */
    /* Check for the Timeout */
    if ((HAL_GetTick() - tickstart) > DSI_TIMEOUT_VALUE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Process Unlocked */
      __HAL_UNLOCK(hdsi);                               /* HAL辅助宏/函数 */

      return HAL_TIMEOUT;                               /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Exit ULPS on Clock and Data Lanes */
  hdsi->Instance->PUCR |= (DSI_PUCR_UECL | DSI_PUCR_UEDL);  /* 赋值或初始化 */

  /* Get tick */
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  /* Wait until all active lanes exit ULPM */
  if ((hdsi->Instance->PCONFR & DSI_PCONFR_NL) == DSI_ONE_DATA_LANE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    while ((hdsi->Instance->PSR & (DSI_PSR_UAN0 | DSI_PSR_UANC)) != (DSI_PSR_UAN0 | DSI_PSR_UANC))  /* while循环 */
    {                                                   /* 代码块开始 */
      /* Check for the Timeout */
      if ((HAL_GetTick() - tickstart) > DSI_TIMEOUT_VALUE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Process Unlocked */
        __HAL_UNLOCK(hdsi);                             /* HAL辅助宏/函数 */

        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if ((hdsi->Instance->PCONFR & DSI_PCONFR_NL) == DSI_TWO_DATA_LANES)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    while ((hdsi->Instance->PSR & (DSI_PSR_UAN0 | DSI_PSR_UAN1 | DSI_PSR_UANC)) != (DSI_PSR_UAN0 | DSI_PSR_UAN1 |  /* while循环 */
                                                                                    DSI_PSR_UANC))  /* 源代码 */
    {                                                   /* 代码块开始 */
      /* Check for the Timeout */
      if ((HAL_GetTick() - tickstart) > DSI_TIMEOUT_VALUE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Process Unlocked */
        __HAL_UNLOCK(hdsi);                             /* HAL辅助宏/函数 */

        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Process unlocked */
    __HAL_UNLOCK(hdsi);                                 /* HAL辅助宏/函数 */

    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* wait for 1 ms */
  HAL_Delay(1U);                                        /* HAL库函数调用 */

  /* De-assert the ULPM requests and the ULPM exit bits */
  hdsi->Instance->PUCR = 0U;                            /* 赋值或初始化 */

  /* Switch the lanbyteclock source in the RCC from system PLL to D-PHY */
  __HAL_RCC_DSI_CONFIG(RCC_DSICLKSOURCE_DSIPHY);        /* HAL辅助宏/函数 */

  /* Restore clock lane configuration to HS */
  hdsi->Instance->CLCR |= DSI_CLCR_DPCC;                /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hdsi);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Start test pattern generation
  * @param  hdsi  pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @param  Mode  Pattern generator mode
  *          This parameter can be one of the following values:
  *           0 : Color bars (horizontal or vertical)
  *           1 : BER pattern (vertical only)
  * @param  Orientation  Pattern generator orientation
  *          This parameter can be one of the following values:
  *           0 : Vertical color bars
  *           1 : Horizontal color bars
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DSI_PatternGeneratorStart(DSI_HandleTypeDef *hdsi, uint32_t Mode, uint32_t Orientation)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Process locked */
  __HAL_LOCK(hdsi);                                     /* HAL辅助宏/函数 */

  /* Configure pattern generator mode and orientation */
  hdsi->Instance->VMCR &= ~(DSI_VMCR_PGM | DSI_VMCR_PGO);  /* 赋值或初始化 */
  hdsi->Instance->VMCR |= ((Mode << 20U) | (Orientation << 24U));  /* 赋值或初始化 */

  /* Enable pattern generator by setting PGE bit */
  hdsi->Instance->VMCR |= DSI_VMCR_PGE;                 /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hdsi);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Stop test pattern generation
  * @param  hdsi  pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DSI_PatternGeneratorStop(DSI_HandleTypeDef *hdsi)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Process locked */
  __HAL_LOCK(hdsi);                                     /* HAL辅助宏/函数 */

  /* Disable pattern generator by clearing PGE bit */
  hdsi->Instance->VMCR &= ~DSI_VMCR_PGE;                /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hdsi);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Set Slew-Rate And Delay Tuning
  * @param  hdsi  pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @param  CommDelay  Communication delay to be adjusted.
  *                    This parameter can be any value of @arg DSI_Communication_Delay
  * @param  Lane  select between clock or data lanes.
  *               This parameter can be any value of @arg DSI_Lane_Group
  * @param  Value  Custom value of the slew-rate or delay
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DSI_SetSlewRateAndDelayTuning(DSI_HandleTypeDef *hdsi, uint32_t CommDelay, uint32_t Lane,  /* HAL状态类型函数 */
                                                    uint32_t Value)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  /* Process locked */
  __HAL_LOCK(hdsi);                                     /* HAL辅助宏/函数 */

  /* Check function parameters */
  assert_param(IS_DSI_COMMUNICATION_DELAY(CommDelay));  /* 执行语句 */
  assert_param(IS_DSI_LANE_GROUP(Lane));                /* 执行语句 */

  switch (CommDelay)                                    /* switch分支 */
  {                                                     /* 代码块开始 */
    case DSI_SLEW_RATE_HSTX:                            /* case标签 */
      if (Lane == DSI_CLOCK_LANE)                       /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* High-Speed Transmission Slew Rate Control on Clock Lane */
        hdsi->Instance->WPCR[1U] &= ~DSI_WPCR1_HSTXSRCCL;  /* 赋值或初始化 */
        hdsi->Instance->WPCR[1U] |= Value << 16U;       /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else if (Lane == DSI_DATA_LANES)                  /* 否则如果 */
      {                                                 /* 代码块开始 */
        /* High-Speed Transmission Slew Rate Control on Data Lanes */
        hdsi->Instance->WPCR[1U] &= ~DSI_WPCR1_HSTXSRCDL;  /* 赋值或初始化 */
        hdsi->Instance->WPCR[1U] |= Value << 18U;       /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Process unlocked */
        __HAL_UNLOCK(hdsi);                             /* HAL辅助宏/函数 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */
    case DSI_SLEW_RATE_LPTX:                            /* case标签 */
      if (Lane == DSI_CLOCK_LANE)                       /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Low-Power transmission Slew Rate Compensation on Clock Lane */
        hdsi->Instance->WPCR[1U] &= ~DSI_WPCR1_LPSRCCL;  /* 赋值或初始化 */
        hdsi->Instance->WPCR[1U] |= Value << 6U;        /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else if (Lane == DSI_DATA_LANES)                  /* 否则如果 */
      {                                                 /* 代码块开始 */
        /* Low-Power transmission Slew Rate Compensation on Data Lanes */
        hdsi->Instance->WPCR[1U] &= ~DSI_WPCR1_LPSRCDL;  /* 赋值或初始化 */
        hdsi->Instance->WPCR[1U] |= Value << 8U;        /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Process unlocked */
        __HAL_UNLOCK(hdsi);                             /* HAL辅助宏/函数 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */
    case DSI_HS_DELAY:                                  /* case标签 */
      if (Lane == DSI_CLOCK_LANE)                       /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* High-Speed Transmission Delay on Clock Lane */
        hdsi->Instance->WPCR[1U] &= ~DSI_WPCR1_HSTXDCL;  /* 赋值或初始化 */
        hdsi->Instance->WPCR[1U] |= Value;              /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else if (Lane == DSI_DATA_LANES)                  /* 否则如果 */
      {                                                 /* 代码块开始 */
        /* High-Speed Transmission Delay on Data Lanes */
        hdsi->Instance->WPCR[1U] &= ~DSI_WPCR1_HSTXDDL;  /* 赋值或初始化 */
        hdsi->Instance->WPCR[1U] |= Value << 2U;        /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Process unlocked */
        __HAL_UNLOCK(hdsi);                             /* HAL辅助宏/函数 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */
    default:                                            /* default分支 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  /* Process unlocked */
  __HAL_UNLOCK(hdsi);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Low-Power Reception Filter Tuning
  * @param  hdsi  pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @param  Frequency  cutoff frequency of low-pass filter at the input of LPRX
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DSI_SetLowPowerRXFilter(DSI_HandleTypeDef *hdsi, uint32_t Frequency)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Process locked */
  __HAL_LOCK(hdsi);                                     /* HAL辅助宏/函数 */

  /* Low-Power RX low-pass Filtering Tuning */
  hdsi->Instance->WPCR[1U] &= ~DSI_WPCR1_LPRXFT;        /* 赋值或初始化 */
  hdsi->Instance->WPCR[1U] |= Frequency << 25U;         /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hdsi);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Activate an additional current path on all lanes to meet the SDDTx parameter
  *         defined in the MIPI D-PHY specification
  * @param  hdsi  pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @param  State  ENABLE or DISABLE
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DSI_SetSDD(DSI_HandleTypeDef *hdsi, FunctionalState State)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Process locked */
  __HAL_LOCK(hdsi);                                     /* HAL辅助宏/函数 */

  /* Check function parameters */
  assert_param(IS_FUNCTIONAL_STATE(State));             /* 执行语句 */

  /* Activate/Disactivate additional current path on all lanes */
  hdsi->Instance->WPCR[1U] &= ~DSI_WPCR1_SDDC;          /* 赋值或初始化 */
  hdsi->Instance->WPCR[1U] |= ((uint32_t)State << 12U);  /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hdsi);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Custom lane pins configuration
  * @param  hdsi  pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @param  CustomLane  Function to be applied on selected lane.
  *                     This parameter can be any value of @arg DSI_CustomLane
  * @param  Lane  select between clock or data lane 0 or data lane 1.
  *               This parameter can be any value of @arg DSI_Lane_Select
  * @param  State  ENABLE or DISABLE
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DSI_SetLanePinsConfiguration(DSI_HandleTypeDef *hdsi, uint32_t CustomLane, uint32_t Lane,  /* HAL状态类型函数 */
                                                   FunctionalState State)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Process locked */
  __HAL_LOCK(hdsi);                                     /* HAL辅助宏/函数 */

  /* Check function parameters */
  assert_param(IS_DSI_CUSTOM_LANE(CustomLane));         /* 执行语句 */
  assert_param(IS_DSI_LANE(Lane));                      /* 执行语句 */
  assert_param(IS_FUNCTIONAL_STATE(State));             /* 执行语句 */

  switch (CustomLane)                                   /* switch分支 */
  {                                                     /* 代码块开始 */
    case DSI_SWAP_LANE_PINS:                            /* case标签 */
      if (Lane == DSI_CLK_LANE)                         /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Swap pins on clock lane */
        hdsi->Instance->WPCR[0U] &= ~DSI_WPCR0_SWCL;    /* 赋值或初始化 */
        hdsi->Instance->WPCR[0U] |= ((uint32_t)State << 6U);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else if (Lane == DSI_DATA_LANE0)                  /* 否则如果 */
      {                                                 /* 代码块开始 */
        /* Swap pins on data lane 0 */
        hdsi->Instance->WPCR[0U] &= ~DSI_WPCR0_SWDL0;   /* 赋值或初始化 */
        hdsi->Instance->WPCR[0U] |= ((uint32_t)State << 7U);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else if (Lane == DSI_DATA_LANE1)                  /* 否则如果 */
      {                                                 /* 代码块开始 */
        /* Swap pins on data lane 1 */
        hdsi->Instance->WPCR[0U] &= ~DSI_WPCR0_SWDL1;   /* 赋值或初始化 */
        hdsi->Instance->WPCR[0U] |= ((uint32_t)State << 8U);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Process unlocked */
        __HAL_UNLOCK(hdsi);                             /* HAL辅助宏/函数 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */
    case DSI_INVERT_HS_SIGNAL:                          /* case标签 */
      if (Lane == DSI_CLK_LANE)                         /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Invert HS signal on clock lane */
        hdsi->Instance->WPCR[0U] &= ~DSI_WPCR0_HSICL;   /* 赋值或初始化 */
        hdsi->Instance->WPCR[0U] |= ((uint32_t)State << 9U);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else if (Lane == DSI_DATA_LANE0)                  /* 否则如果 */
      {                                                 /* 代码块开始 */
        /* Invert HS signal on data lane 0 */
        hdsi->Instance->WPCR[0U] &= ~DSI_WPCR0_HSIDL0;  /* 赋值或初始化 */
        hdsi->Instance->WPCR[0U] |= ((uint32_t)State << 10U);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else if (Lane == DSI_DATA_LANE1)                  /* 否则如果 */
      {                                                 /* 代码块开始 */
        /* Invert HS signal on data lane 1 */
        hdsi->Instance->WPCR[0U] &= ~DSI_WPCR0_HSIDL1;  /* 赋值或初始化 */
        hdsi->Instance->WPCR[0U] |= ((uint32_t)State << 11U);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Process unlocked */
        __HAL_UNLOCK(hdsi);                             /* HAL辅助宏/函数 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */
    default:                                            /* default分支 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  /* Process unlocked */
  __HAL_UNLOCK(hdsi);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Set custom timing for the PHY
  * @param  hdsi  pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @param  Timing  PHY timing to be adjusted.
  *                 This parameter can be any value of @arg DSI_PHY_Timing
  * @param  State  ENABLE or DISABLE
  * @param  Value  Custom value of the timing
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DSI_SetPHYTimings(DSI_HandleTypeDef *hdsi, uint32_t Timing, FunctionalState State, uint32_t Value)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Process locked */
  __HAL_LOCK(hdsi);                                     /* HAL辅助宏/函数 */

  /* Check function parameters */
  assert_param(IS_DSI_PHY_TIMING(Timing));              /* 执行语句 */
  assert_param(IS_FUNCTIONAL_STATE(State));             /* 执行语句 */

  switch (Timing)                                       /* switch分支 */
  {                                                     /* 代码块开始 */
    case DSI_TCLK_POST:                                 /* case标签 */
      /* Enable/Disable custom timing setting */
      hdsi->Instance->WPCR[0U] &= ~DSI_WPCR0_TCLKPOSTEN;  /* 赋值或初始化 */
      hdsi->Instance->WPCR[0U] |= ((uint32_t)State << 27U);  /* 赋值或初始化 */

      if (State != DISABLE)                             /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Set custom value */
        hdsi->Instance->WPCR[4U] &= ~DSI_WPCR4_TCLKPOST;  /* 赋值或初始化 */
        hdsi->Instance->WPCR[4U] |= Value & DSI_WPCR4_TCLKPOST;  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */

      break;                                            /* 跳出 */
    case DSI_TLPX_CLK:                                  /* case标签 */
      /* Enable/Disable custom timing setting */
      hdsi->Instance->WPCR[0U] &= ~DSI_WPCR0_TLPXCEN;   /* 赋值或初始化 */
      hdsi->Instance->WPCR[0U] |= ((uint32_t)State << 26U);  /* 赋值或初始化 */

      if (State != DISABLE)                             /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Set custom value */
        hdsi->Instance->WPCR[3U] &= ~DSI_WPCR3_TLPXC;   /* 赋值或初始化 */
        hdsi->Instance->WPCR[3U] |= (Value << 24U) & DSI_WPCR3_TLPXC;  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */

      break;                                            /* 跳出 */
    case DSI_THS_EXIT:                                  /* case标签 */
      /* Enable/Disable custom timing setting */
      hdsi->Instance->WPCR[0U] &= ~DSI_WPCR0_THSEXITEN;  /* 赋值或初始化 */
      hdsi->Instance->WPCR[0U] |= ((uint32_t)State << 25U);  /* 赋值或初始化 */

      if (State != DISABLE)                             /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Set custom value */
        hdsi->Instance->WPCR[3U] &= ~DSI_WPCR3_THSEXIT;  /* 赋值或初始化 */
        hdsi->Instance->WPCR[3U] |= (Value << 16U) & DSI_WPCR3_THSEXIT;  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */

      break;                                            /* 跳出 */
    case DSI_TLPX_DATA:                                 /* case标签 */
      /* Enable/Disable custom timing setting */
      hdsi->Instance->WPCR[0U] &= ~DSI_WPCR0_TLPXDEN;   /* 赋值或初始化 */
      hdsi->Instance->WPCR[0U] |= ((uint32_t)State << 24U);  /* 赋值或初始化 */

      if (State != DISABLE)                             /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Set custom value */
        hdsi->Instance->WPCR[3U] &= ~DSI_WPCR3_TLPXD;   /* 赋值或初始化 */
        hdsi->Instance->WPCR[3U] |= (Value << 8U) & DSI_WPCR3_TLPXD;  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */

      break;                                            /* 跳出 */
    case DSI_THS_ZERO:                                  /* case标签 */
      /* Enable/Disable custom timing setting */
      hdsi->Instance->WPCR[0U] &= ~DSI_WPCR0_THSZEROEN;  /* 赋值或初始化 */
      hdsi->Instance->WPCR[0U] |= ((uint32_t)State << 23U);  /* 赋值或初始化 */

      if (State != DISABLE)                             /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Set custom value */
        hdsi->Instance->WPCR[3U] &= ~DSI_WPCR3_THSZERO;  /* 赋值或初始化 */
        hdsi->Instance->WPCR[3U] |= Value & DSI_WPCR3_THSZERO;  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */

      break;                                            /* 跳出 */
    case DSI_THS_TRAIL:                                 /* case标签 */
      /* Enable/Disable custom timing setting */
      hdsi->Instance->WPCR[0U] &= ~DSI_WPCR0_THSTRAILEN;  /* 赋值或初始化 */
      hdsi->Instance->WPCR[0U] |= ((uint32_t)State << 22U);  /* 赋值或初始化 */

      if (State != DISABLE)                             /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Set custom value */
        hdsi->Instance->WPCR[2U] &= ~DSI_WPCR2_THSTRAIL;  /* 赋值或初始化 */
        hdsi->Instance->WPCR[2U] |= (Value << 24U) & DSI_WPCR2_THSTRAIL;  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */

      break;                                            /* 跳出 */
    case DSI_THS_PREPARE:                               /* case标签 */
      /* Enable/Disable custom timing setting */
      hdsi->Instance->WPCR[0U] &= ~DSI_WPCR0_THSPREPEN;  /* 赋值或初始化 */
      hdsi->Instance->WPCR[0U] |= ((uint32_t)State << 21U);  /* 赋值或初始化 */

      if (State != DISABLE)                             /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Set custom value */
        hdsi->Instance->WPCR[2U] &= ~DSI_WPCR2_THSPREP;  /* 赋值或初始化 */
        hdsi->Instance->WPCR[2U] |= (Value << 16U) & DSI_WPCR2_THSPREP;  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */

      break;                                            /* 跳出 */
    case DSI_TCLK_ZERO:                                 /* case标签 */
      /* Enable/Disable custom timing setting */
      hdsi->Instance->WPCR[0U] &= ~DSI_WPCR0_TCLKZEROEN;  /* 赋值或初始化 */
      hdsi->Instance->WPCR[0U] |= ((uint32_t)State << 20U);  /* 赋值或初始化 */

      if (State != DISABLE)                             /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Set custom value */
        hdsi->Instance->WPCR[2U] &= ~DSI_WPCR2_TCLKZERO;  /* 赋值或初始化 */
        hdsi->Instance->WPCR[2U] |= (Value << 8U) & DSI_WPCR2_TCLKZERO;  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */

      break;                                            /* 跳出 */
    case DSI_TCLK_PREPARE:                              /* case标签 */
      /* Enable/Disable custom timing setting */
      hdsi->Instance->WPCR[0U] &= ~DSI_WPCR0_TCLKPREPEN;  /* 赋值或初始化 */
      hdsi->Instance->WPCR[0U] |= ((uint32_t)State << 19U);  /* 赋值或初始化 */

      if (State != DISABLE)                             /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Set custom value */
        hdsi->Instance->WPCR[2U] &= ~DSI_WPCR2_TCLKPREP;  /* 赋值或初始化 */
        hdsi->Instance->WPCR[2U] |= Value & DSI_WPCR2_TCLKPREP;  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */

      break;                                            /* 跳出 */
    default:                                            /* default分支 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  /* Process unlocked */
  __HAL_UNLOCK(hdsi);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Force the Clock/Data Lane in TX Stop Mode
  * @param  hdsi  pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @param  Lane  select between clock or data lanes.
  *               This parameter can be any value of @arg DSI_Lane_Group
  * @param  State  ENABLE or DISABLE
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DSI_ForceTXStopMode(DSI_HandleTypeDef *hdsi, uint32_t Lane, FunctionalState State)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Process locked */
  __HAL_LOCK(hdsi);                                     /* HAL辅助宏/函数 */

  /* Check function parameters */
  assert_param(IS_DSI_LANE_GROUP(Lane));                /* 执行语句 */
  assert_param(IS_FUNCTIONAL_STATE(State));             /* 执行语句 */

  if (Lane == DSI_CLOCK_LANE)                           /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Force/Unforce the Clock Lane in TX Stop Mode */
    hdsi->Instance->WPCR[0U] &= ~DSI_WPCR0_FTXSMCL;     /* 赋值或初始化 */
    hdsi->Instance->WPCR[0U] |= ((uint32_t)State << 12U);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if (Lane == DSI_DATA_LANES)                      /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* Force/Unforce the Data Lanes in TX Stop Mode */
    hdsi->Instance->WPCR[0U] &= ~DSI_WPCR0_FTXSMDL;     /* 赋值或初始化 */
    hdsi->Instance->WPCR[0U] |= ((uint32_t)State << 13U);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Process unlocked */
    __HAL_UNLOCK(hdsi);                                 /* HAL辅助宏/函数 */

    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Process unlocked */
  __HAL_UNLOCK(hdsi);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Force LP Receiver in Low-Power Mode
  * @param  hdsi  pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @param  State  ENABLE or DISABLE
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DSI_ForceRXLowPower(DSI_HandleTypeDef *hdsi, FunctionalState State)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Process locked */
  __HAL_LOCK(hdsi);                                     /* HAL辅助宏/函数 */

  /* Check function parameters */
  assert_param(IS_FUNCTIONAL_STATE(State));             /* 执行语句 */

  /* Force/Unforce LP Receiver in Low-Power Mode */
  hdsi->Instance->WPCR[1U] &= ~DSI_WPCR1_FLPRXLPM;      /* 赋值或初始化 */
  hdsi->Instance->WPCR[1U] |= ((uint32_t)State << 22U);  /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hdsi);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Force Data Lanes in RX Mode after a BTA
  * @param  hdsi  pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @param  State  ENABLE or DISABLE
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DSI_ForceDataLanesInRX(DSI_HandleTypeDef *hdsi, FunctionalState State)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Process locked */
  __HAL_LOCK(hdsi);                                     /* HAL辅助宏/函数 */

  /* Check function parameters */
  assert_param(IS_FUNCTIONAL_STATE(State));             /* 执行语句 */

  /* Force Data Lanes in RX Mode */
  hdsi->Instance->WPCR[0U] &= ~DSI_WPCR0_TDDL;          /* 赋值或初始化 */
  hdsi->Instance->WPCR[0U] |= ((uint32_t)State << 16U);  /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hdsi);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Enable a pull-down on the lanes to prevent from floating states when unused
  * @param  hdsi  pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @param  State  ENABLE or DISABLE
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DSI_SetPullDown(DSI_HandleTypeDef *hdsi, FunctionalState State)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Process locked */
  __HAL_LOCK(hdsi);                                     /* HAL辅助宏/函数 */

  /* Check function parameters */
  assert_param(IS_FUNCTIONAL_STATE(State));             /* 执行语句 */

  /* Enable/Disable pull-down on lanes */
  hdsi->Instance->WPCR[0U] &= ~DSI_WPCR0_PDEN;          /* 赋值或初始化 */
  hdsi->Instance->WPCR[0U] |= ((uint32_t)State << 18U);  /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hdsi);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Switch off the contention detection on data lanes
  * @param  hdsi  pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @param  State  ENABLE or DISABLE
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DSI_SetContentionDetectionOff(DSI_HandleTypeDef *hdsi, FunctionalState State)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Process locked */
  __HAL_LOCK(hdsi);                                     /* HAL辅助宏/函数 */

  /* Check function parameters */
  assert_param(IS_FUNCTIONAL_STATE(State));             /* 执行语句 */

  /* Contention Detection on Data Lanes OFF */
  hdsi->Instance->WPCR[0U] &= ~DSI_WPCR0_CDOFFDL;       /* 赋值或初始化 */
  hdsi->Instance->WPCR[0U] |= ((uint32_t)State << 14U);  /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hdsi);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup DSI_Group4 Peripheral State and Errors functions
  *  @brief    Peripheral State and Errors functions
  *
@verbatim
 ===============================================================================
                  ##### Peripheral State and Errors functions #####
 ===============================================================================
    [..]
    This subsection provides functions allowing to
      (+) Check the DSI state.
      (+) Get error code.

@endverbatim
  * @{
  */

/**
  * @brief  Return the DSI state
  * @param  hdsi  pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @retval HAL state
  */
HAL_DSI_StateTypeDef HAL_DSI_GetState(DSI_HandleTypeDef *hdsi)  /* HAL库函数调用 */
{                                                       /* 代码块开始 */
  return hdsi->State;                                   /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return the DSI error code
  * @param  hdsi  pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @retval DSI Error Code
  */
uint32_t HAL_DSI_GetError(DSI_HandleTypeDef *hdsi)      /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  /* Get the error code */
  return hdsi->ErrorCode;                               /* 返回值 */
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

#endif /* DSI */

#endif /* HAL_DSI_MODULE_ENABLED */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
