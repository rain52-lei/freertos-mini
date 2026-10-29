/* 文件: stm32f4xx_hal_i2s.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_i2s.c
  * @author  MCD Application Team
  * @brief   I2S HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Integrated Interchip Sound (I2S) peripheral:
  *           + Initialization and de-initialization functions
  *           + IO operation functions
  *           + Peripheral State and Errors functions
  @verbatim
 ===============================================================================
                  ##### How to use this driver #####
 ===============================================================================
 [..]
    The I2S HAL driver can be used as follow:

    (#) Declare a I2S_HandleTypeDef handle structure.
    (#) Initialize the I2S low level resources by implement the HAL_I2S_MspInit() API:
        (##) Enable the SPIx interface clock.
        (##) I2S pins configuration:
            (+++) Enable the clock for the I2S GPIOs.
            (+++) Configure these I2S pins as alternate function pull-up.
        (##) NVIC configuration if you need to use interrupt process (HAL_I2S_Transmit_IT()
             and HAL_I2S_Receive_IT() APIs).
            (+++) Configure the I2Sx interrupt priority.
            (+++) Enable the NVIC I2S IRQ handle.
        (##) DMA Configuration if you need to use DMA process (HAL_I2S_Transmit_DMA()
             and HAL_I2S_Receive_DMA() APIs:
            (+++) Declare a DMA handle structure for the Tx/Rx Stream/Channel.
            (+++) Enable the DMAx interface clock.
            (+++) Configure the declared DMA handle structure with the required Tx/Rx parameters.
            (+++) Configure the DMA Tx/Rx Stream/Channel.
            (+++) Associate the initialized DMA handle to the I2S DMA Tx/Rx handle.
            (+++) Configure the priority and enable the NVIC for the transfer complete interrupt on the
                  DMA Tx/Rx Stream/Channel.

   (#) Program the Mode, Standard, Data Format, MCLK Output, Audio frequency and Polarity
       using HAL_I2S_Init() function.

   -@- The specific I2S interrupts (Transmission complete interrupt,
       RXNE interrupt and Error Interrupts) will be managed using the macros
       __HAL_I2S_ENABLE_IT() and __HAL_I2S_DISABLE_IT() inside the transmit and receive process.
   -@- Make sure that either:
        (+@) I2S PLL clock is configured or
        (+@) External clock source is configured after setting correctly
             the define constant EXTERNAL_CLOCK_VALUE in the stm32f4xx_hal_conf.h file.

    (#) Three mode of operations are available within this driver :

   *** Polling mode IO operation ***
   =================================
   [..]
     (+) Send an amount of data in blocking mode using HAL_I2S_Transmit()
     (+) Receive an amount of data in blocking mode using HAL_I2S_Receive()

   *** Interrupt mode IO operation ***
   ===================================
   [..]
     (+) Send an amount of data in non blocking mode using HAL_I2S_Transmit_IT()
     (+) At transmission end of half transfer HAL_I2S_TxHalfCpltCallback is executed and user can
         add his own code by customization of function pointer HAL_I2S_TxHalfCpltCallback
     (+) At transmission end of transfer HAL_I2S_TxCpltCallback is executed and user can
         add his own code by customization of function pointer HAL_I2S_TxCpltCallback
     (+) Receive an amount of data in non blocking mode using HAL_I2S_Receive_IT()
     (+) At reception end of half transfer HAL_I2S_RxHalfCpltCallback is executed and user can
         add his own code by customization of function pointer HAL_I2S_RxHalfCpltCallback
     (+) At reception end of transfer HAL_I2S_RxCpltCallback is executed and user can
         add his own code by customization of function pointer HAL_I2S_RxCpltCallback
     (+) In case of transfer Error, HAL_I2S_ErrorCallback() function is executed and user can
         add his own code by customization of function pointer HAL_I2S_ErrorCallback

   *** DMA mode IO operation ***
   ==============================
   [..]
     (+) Send an amount of data in non blocking mode (DMA) using HAL_I2S_Transmit_DMA()
     (+) At transmission end of half transfer HAL_I2S_TxHalfCpltCallback is executed and user can
         add his own code by customization of function pointer HAL_I2S_TxHalfCpltCallback
     (+) At transmission end of transfer HAL_I2S_TxCpltCallback is executed and user can
         add his own code by customization of function pointer HAL_I2S_TxCpltCallback
     (+) Receive an amount of data in non blocking mode (DMA) using HAL_I2S_Receive_DMA()
     (+) At reception end of half transfer HAL_I2S_RxHalfCpltCallback is executed and user can
         add his own code by customization of function pointer HAL_I2S_RxHalfCpltCallback
     (+) At reception end of transfer HAL_I2S_RxCpltCallback is executed and user can
         add his own code by customization of function pointer HAL_I2S_RxCpltCallback
     (+) In case of transfer Error, HAL_I2S_ErrorCallback() function is executed and user can
         add his own code by customization of function pointer HAL_I2S_ErrorCallback
     (+) Pause the DMA Transfer using HAL_I2S_DMAPause()
     (+) Resume the DMA Transfer using HAL_I2S_DMAResume()
     (+) Stop the DMA Transfer using HAL_I2S_DMAStop()
         In Slave mode, if HAL_I2S_DMAStop is used to stop the communication, an error
         HAL_I2S_ERROR_BUSY_LINE_RX is raised as the master continue to transmit data.
         In this case __HAL_I2S_FLUSH_RX_DR macro must be used to flush the remaining data
         inside DR register and avoid using DeInit/Init process for the next transfer.

   *** I2S HAL driver macros list ***
   ===================================
   [..]
     Below the list of most used macros in I2S HAL driver.

      (+) __HAL_I2S_ENABLE: Enable the specified SPI peripheral (in I2S mode)
      (+) __HAL_I2S_DISABLE: Disable the specified SPI peripheral (in I2S mode)
      (+) __HAL_I2S_ENABLE_IT : Enable the specified I2S interrupts
      (+) __HAL_I2S_DISABLE_IT : Disable the specified I2S interrupts
      (+) __HAL_I2S_GET_FLAG: Check whether the specified I2S flag is set or not
      (+) __HAL_I2S_FLUSH_RX_DR: Read DR Register to Flush RX Data

    [..]
      (@) You can refer to the I2S HAL driver header file for more useful macros

   *** I2S HAL driver macros list ***
   ===================================
   [..]
       Callback registration:

      (#) The compilation flag USE_HAL_I2S_REGISTER_CALLBACKS when set to 1U
          allows the user to configure dynamically the driver callbacks.
          Use Functions HAL_I2S_RegisterCallback() to register an interrupt callback.

          Function HAL_I2S_RegisterCallback() allows to register following callbacks:
            (++) TxCpltCallback        : I2S Tx Completed callback
            (++) RxCpltCallback        : I2S Rx Completed callback
            (++) TxRxCpltCallback      : I2S TxRx Completed callback
            (++) TxHalfCpltCallback    : I2S Tx Half Completed callback
            (++) RxHalfCpltCallback    : I2S Rx Half Completed callback
            (++) ErrorCallback         : I2S Error callback
            (++) MspInitCallback       : I2S Msp Init callback
            (++) MspDeInitCallback     : I2S Msp DeInit callback
          This function takes as parameters the HAL peripheral handle, the Callback ID
          and a pointer to the user callback function.


      (#) Use function HAL_I2S_UnRegisterCallback to reset a callback to the default
          weak function.
          HAL_I2S_UnRegisterCallback takes as parameters the HAL peripheral handle,
          and the Callback ID.
          This function allows to reset following callbacks:
            (++) TxCpltCallback        : I2S Tx Completed callback
            (++) RxCpltCallback        : I2S Rx Completed callback
            (++) TxRxCpltCallback      : I2S TxRx Completed callback
            (++) TxHalfCpltCallback    : I2S Tx Half Completed callback
            (++) RxHalfCpltCallback    : I2S Rx Half Completed callback
            (++) ErrorCallback         : I2S Error callback
            (++) MspInitCallback       : I2S Msp Init callback
            (++) MspDeInitCallback     : I2S Msp DeInit callback

       [..]
       By default, after the HAL_I2S_Init() and when the state is HAL_I2S_STATE_RESET
       all callbacks are set to the corresponding weak functions:
       examples HAL_I2S_MasterTxCpltCallback(), HAL_I2S_MasterRxCpltCallback().
       Exception done for MspInit and MspDeInit functions that are
       reset to the legacy weak functions in the HAL_I2S_Init()/ HAL_I2S_DeInit() only when
       these callbacks are null (not registered beforehand).
       If MspInit or MspDeInit are not null, the HAL_I2S_Init()/ HAL_I2S_DeInit()
       keep and use the user MspInit/MspDeInit callbacks (registered beforehand) whatever the state.

       [..]
       Callbacks can be registered/unregistered in HAL_I2S_STATE_READY state only.
       Exception done MspInit/MspDeInit functions that can be registered/unregistered
       in HAL_I2S_STATE_READY or HAL_I2S_STATE_RESET state,
       thus registered (user) MspInit/DeInit callbacks can be used during the Init/DeInit.
       Then, the user first registers the MspInit/MspDeInit user callbacks
       using HAL_I2S_RegisterCallback() before calling HAL_I2S_DeInit()
       or HAL_I2S_Init() function.

       [..]
       When the compilation define USE_HAL_I2S_REGISTER_CALLBACKS is set to 0 or
       not defined, the callback registering feature is not available
       and weak (surcharged) callbacks are used.

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

#ifdef HAL_I2S_MODULE_ENABLED                           /* 条件编译 */

/** @addtogroup STM32F4xx_HAL_Driver
  * @{
  */

/** @defgroup I2S I2S
  * @brief I2S HAL module driver
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define I2S_TIMEOUT_FLAG          100U         /*!< Timeout 100 ms            */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/** @defgroup I2S_Private_Functions I2S Private Functions
  * @{
  */
static void               I2S_DMATxCplt(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void               I2S_DMATxHalfCplt(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void               I2S_DMARxCplt(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void               I2S_DMARxHalfCplt(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void               I2S_DMAError(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void               I2S_Transmit_IT(I2S_HandleTypeDef *hi2s);  /* 静态函数 */
static void               I2S_Receive_IT(I2S_HandleTypeDef *hi2s);  /* 静态函数 */
static void               I2S_IRQHandler(I2S_HandleTypeDef *hi2s);  /* 静态函数 */
static HAL_StatusTypeDef  I2S_WaitFlagStateUntilTimeout(I2S_HandleTypeDef *hi2s, uint32_t Flag, FlagStatus State,  /* 静态变量/函数 */
                                                        uint32_t Timeout);  /* 32位无符号变量/函数 */
/**
  * @}
  */

/* Exported functions ---------------------------------------------------------*/

/** @defgroup I2S_Exported_Functions I2S Exported Functions
  * @{
  */

/** @defgroup  I2S_Exported_Functions_Group1 Initialization and de-initialization functions
  *  @brief    Initialization and Configuration functions
  *
@verbatim
 ===============================================================================
              ##### Initialization and de-initialization functions #####
 ===============================================================================
    [..]  This subsection provides a set of functions allowing to initialize and
          de-initialize the I2Sx peripheral in simplex mode:

      (+) User must Implement HAL_I2S_MspInit() function in which he configures
          all related peripherals resources (CLOCK, GPIO, DMA, IT and NVIC ).

      (+) Call the function HAL_I2S_Init() to configure the selected device with
          the selected configuration:
        (++) Mode
        (++) Standard
        (++) Data Format
        (++) MCLK Output
        (++) Audio frequency
        (++) Polarity
        (++) Full duplex mode

     (+) Call the function HAL_I2S_DeInit() to restore the default configuration
          of the selected I2Sx peripheral.
  @endverbatim
  * @{
  */

/**
  * @brief  Initializes the I2S according to the specified parameters
  *         in the I2S_InitTypeDef and create the associated handle.
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2S_Init(I2S_HandleTypeDef *hi2s)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t i2sdiv;                                      /* 32位无符号变量/函数 */
  uint32_t i2sodd;                                      /* 32位无符号变量/函数 */
  uint32_t packetlength;                                /* 32位无符号变量/函数 */
  uint32_t tmp;                                         /* 32位无符号变量/函数 */
  uint32_t i2sclk;                                      /* 32位无符号变量/函数 */
#if defined (SPI_I2S_FULLDUPLEX_SUPPORT)                /* 条件编译 */
  uint16_t tmpreg;                                      /* 16位无符号变量/函数 */
#endif                                                  /* 条件编译结束 */

  /* Check the I2S handle allocation */
  if (hi2s == NULL)                                     /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the I2S parameters */
  assert_param(IS_I2S_ALL_INSTANCE(hi2s->Instance));    /* 执行语句 */
  assert_param(IS_I2S_MODE(hi2s->Init.Mode));           /* 执行语句 */
  assert_param(IS_I2S_STANDARD(hi2s->Init.Standard));   /* 执行语句 */
  assert_param(IS_I2S_DATA_FORMAT(hi2s->Init.DataFormat));  /* 执行语句 */
  assert_param(IS_I2S_MCLK_OUTPUT(hi2s->Init.MCLKOutput));  /* 执行语句 */
  assert_param(IS_I2S_AUDIO_FREQ(hi2s->Init.AudioFreq));  /* 执行语句 */
  assert_param(IS_I2S_CPOL(hi2s->Init.CPOL));           /* 执行语句 */
  assert_param(IS_I2S_CLOCKSOURCE(hi2s->Init.ClockSource));  /* 执行语句 */

  if (hi2s->State == HAL_I2S_STATE_RESET)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Allocate lock resource and initialize it */
    hi2s->Lock = HAL_UNLOCKED;                          /* 赋值或初始化 */

    /* Initialize Default I2S IrqHandler ISR */
    hi2s->IrqHandlerISR = I2S_IRQHandler;               /* 赋值或初始化 */

#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
    /* Init the I2S Callback settings */
    hi2s->TxCpltCallback       = HAL_I2S_TxCpltCallback;          /* Legacy weak TxCpltCallback       */
    hi2s->RxCpltCallback       = HAL_I2S_RxCpltCallback;          /* Legacy weak RxCpltCallback       */
#if defined (SPI_I2S_FULLDUPLEX_SUPPORT)                /* 条件编译 */
    hi2s->TxRxCpltCallback     = HAL_I2SEx_TxRxCpltCallback;      /* Legacy weak TxRxCpltCallback     */
#endif                                                  /* 条件编译结束 */
    hi2s->TxHalfCpltCallback   = HAL_I2S_TxHalfCpltCallback;      /* Legacy weak TxHalfCpltCallback   */
    hi2s->RxHalfCpltCallback   = HAL_I2S_RxHalfCpltCallback;      /* Legacy weak RxHalfCpltCallback   */
#if defined (SPI_I2S_FULLDUPLEX_SUPPORT)                /* 条件编译 */
    hi2s->TxRxHalfCpltCallback = HAL_I2SEx_TxRxHalfCpltCallback;  /* Legacy weak TxRxHalfCpltCallback */
#endif                                                  /* 条件编译结束 */
    hi2s->ErrorCallback        = HAL_I2S_ErrorCallback;           /* Legacy weak ErrorCallback        */

    if (hi2s->MspInitCallback == NULL)                  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hi2s->MspInitCallback = HAL_I2S_MspInit; /* Legacy weak MspInit  */
    }                                                   /* 代码块结束 */

    /* Init the low level hardware : GPIO, CLOCK, NVIC... */
    hi2s->MspInitCallback(hi2s);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /* Init the low level hardware : GPIO, CLOCK, CORTEX...etc */
    HAL_I2S_MspInit(hi2s);                              /* HAL库函数调用 */
#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */

  hi2s->State = HAL_I2S_STATE_BUSY;                     /* 赋值或初始化 */

  /*----------------------- SPIx I2SCFGR & I2SPR Configuration ----------------*/
  /* Clear I2SMOD, I2SE, I2SCFG, PCMSYNC, I2SSTD, CKPOL, DATLEN and CHLEN bits */
  CLEAR_BIT(hi2s->Instance->I2SCFGR, (SPI_I2SCFGR_CHLEN | SPI_I2SCFGR_DATLEN | SPI_I2SCFGR_CKPOL | \
                                      SPI_I2SCFGR_I2SSTD | SPI_I2SCFGR_PCMSYNC | SPI_I2SCFGR_I2SCFG | \
                                      SPI_I2SCFGR_I2SE | SPI_I2SCFGR_I2SMOD));  /* 执行语句 */
  hi2s->Instance->I2SPR = 0x0002U;                      /* 赋值或初始化 */

  /*----------------------- I2SPR: I2SDIV and ODD Calculation -----------------*/
  /* If the requested audio frequency is not the default, compute the prescaler */
  if (hi2s->Init.AudioFreq != I2S_AUDIOFREQ_DEFAULT)    /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check the frame length (For the Prescaler computing) ********************/
    if (hi2s->Init.DataFormat == I2S_DATAFORMAT_16B)    /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Packet length is 16 bits */
      packetlength = 16U;                               /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Packet length is 32 bits */
      packetlength = 32U;                               /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* I2S standard */
    if (hi2s->Init.Standard <= I2S_STANDARD_LSB)        /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* In I2S standard packet length is multiplied by 2 */
      packetlength = packetlength * 2U;                 /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Get the source clock value **********************************************/
#if defined(I2S_APB1_APB2_FEATURE)                      /* 条件编译 */
    if (IS_I2S_APB1_INSTANCE(hi2s->Instance))           /* 条件判断 */
    {                                                   /* 代码块开始 */
      i2sclk = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_I2S_APB1);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      i2sclk = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_I2S_APB2);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
#else                                                   /* 条件编译否则 */
    i2sclk = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_I2S);  /* 赋值或初始化 */
#endif                                                  /* 条件编译结束 */

    /* Compute the Real divider depending on the MCLK output state, with a floating point */
    if (hi2s->Init.MCLKOutput == I2S_MCLKOUTPUT_ENABLE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* MCLK output is enabled */
      if (hi2s->Init.DataFormat != I2S_DATAFORMAT_16B)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        tmp = (uint32_t)(((((i2sclk / (packetlength * 4U)) * 10U) / hi2s->Init.AudioFreq)) + 5U);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        tmp = (uint32_t)(((((i2sclk / (packetlength * 8U)) * 10U) / hi2s->Init.AudioFreq)) + 5U);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* MCLK output is disabled */
      tmp = (uint32_t)(((((i2sclk / packetlength) * 10U) / hi2s->Init.AudioFreq)) + 5U);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Remove the flatting point */
    tmp = tmp / 10U;                                    /* 赋值或初始化 */

    /* Check the parity of the divider */
    i2sodd = (uint32_t)(tmp & (uint32_t)1U);            /* 赋值或初始化 */

    /* Compute the i2sdiv prescaler */
    i2sdiv = (uint32_t)((tmp - i2sodd) / 2U);           /* 赋值或初始化 */

    /* Get the Mask for the Odd bit (SPI_I2SPR[8]) register */
    i2sodd = (uint32_t)(i2sodd << 8U);                  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Set the default values */
    i2sdiv = 2U;                                        /* 赋值或初始化 */
    i2sodd = 0U;                                        /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Test if the divider is 1 or 0 or greater than 0xFF */
  if ((i2sdiv < 2U) || (i2sdiv > 0xFFU))                /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Set the error code and execute error callback*/
    SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_PRESCALER);  /* 执行语句 */
    return  HAL_ERROR;                                  /* 返回值 */
  }                                                     /* 代码块结束 */

  /*----------------------- SPIx I2SCFGR & I2SPR Configuration ----------------*/

  /* Write to SPIx I2SPR register the computed value */
  hi2s->Instance->I2SPR = (uint32_t)((uint32_t)i2sdiv | (uint32_t)(i2sodd | (uint32_t)hi2s->Init.MCLKOutput));  /* 赋值或初始化 */

  /* Clear I2SMOD, I2SE, I2SCFG, PCMSYNC, I2SSTD, CKPOL, DATLEN and CHLEN bits */
  /* And configure the I2S with the I2S_InitStruct values                      */
  MODIFY_REG(hi2s->Instance->I2SCFGR, (SPI_I2SCFGR_CHLEN | SPI_I2SCFGR_DATLEN | \
                                       SPI_I2SCFGR_CKPOL | SPI_I2SCFGR_I2SSTD | \
                                       SPI_I2SCFGR_PCMSYNC | SPI_I2SCFGR_I2SCFG | \
                                       SPI_I2SCFGR_I2SE  | SPI_I2SCFGR_I2SMOD), \
             (SPI_I2SCFGR_I2SMOD | hi2s->Init.Mode | \
              hi2s->Init.Standard | hi2s->Init.DataFormat | \
              hi2s->Init.CPOL));                        /* 执行语句 */

#if defined(SPI_I2SCFGR_ASTRTEN)                        /* 条件编译 */
  if ((hi2s->Init.Standard == I2S_STANDARD_PCM_SHORT) || ((hi2s->Init.Standard == I2S_STANDARD_PCM_LONG)))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Write to SPIx I2SCFGR */
    SET_BIT(hi2s->Instance->I2SCFGR, SPI_I2SCFGR_ASTRTEN);  /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif /* SPI_I2SCFGR_ASTRTEN */

#if defined (SPI_I2S_FULLDUPLEX_SUPPORT)                /* 条件编译 */

  /* Configure the I2S extended if the full duplex mode is enabled */
  assert_param(IS_I2S_FULLDUPLEX_MODE(hi2s->Init.FullDuplexMode));  /* 执行语句 */

  if (hi2s->Init.FullDuplexMode == I2S_FULLDUPLEXMODE_ENABLE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Set FullDuplex I2S IrqHandler ISR if FULLDUPLEXMODE is enabled */
    hi2s->IrqHandlerISR = HAL_I2SEx_FullDuplex_IRQHandler;  /* 赋值或初始化 */

    /* Clear I2SMOD, I2SE, I2SCFG, PCMSYNC, I2SSTD, CKPOL, DATLEN and CHLEN bits */
    CLEAR_BIT(I2SxEXT(hi2s->Instance)->I2SCFGR, (SPI_I2SCFGR_CHLEN | SPI_I2SCFGR_DATLEN | SPI_I2SCFGR_CKPOL | \
                                                 SPI_I2SCFGR_I2SSTD | SPI_I2SCFGR_PCMSYNC | SPI_I2SCFGR_I2SCFG | \
                                                 SPI_I2SCFGR_I2SE | SPI_I2SCFGR_I2SMOD));  /* 执行语句 */
    I2SxEXT(hi2s->Instance)->I2SPR = 2U;                /* 赋值或初始化 */

    /* Get the I2SCFGR register value */
    tmpreg = I2SxEXT(hi2s->Instance)->I2SCFGR;          /* 赋值或初始化 */

    /* Get the mode to be configured for the extended I2S */
    if ((hi2s->Init.Mode == I2S_MODE_MASTER_TX) || (hi2s->Init.Mode == I2S_MODE_SLAVE_TX))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      tmp = I2S_MODE_SLAVE_RX;                          /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else /* I2S_MODE_MASTER_RX ||  I2S_MODE_SLAVE_RX */
    {                                                   /* 代码块开始 */
      tmp = I2S_MODE_SLAVE_TX;                          /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Configure the I2S Slave with the I2S Master parameter values */
    tmpreg |= (uint16_t)((uint16_t)SPI_I2SCFGR_I2SMOD    | \
                         (uint16_t)tmp                   | \
                         (uint16_t)hi2s->Init.Standard   | \
                         (uint16_t)hi2s->Init.DataFormat | \
                         (uint16_t)hi2s->Init.CPOL);    /* 执行语句 */

    /* Write to SPIx I2SCFGR */
    WRITE_REG(I2SxEXT(hi2s->Instance)->I2SCFGR, tmpreg);  /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif /* SPI_I2S_FULLDUPLEX_SUPPORT */

  hi2s->ErrorCode = HAL_I2S_ERROR_NONE;                 /* 赋值或初始化 */
  hi2s->State     = HAL_I2S_STATE_READY;                /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief DeInitializes the I2S peripheral
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2S_DeInit(I2S_HandleTypeDef *hi2s)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the I2S handle allocation */
  if (hi2s == NULL)                                     /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the parameters */
  assert_param(IS_I2S_ALL_INSTANCE(hi2s->Instance));    /* 执行语句 */

  hi2s->State = HAL_I2S_STATE_BUSY;                     /* 赋值或初始化 */

  /* Disable the I2S Peripheral Clock */
  __HAL_I2S_DISABLE(hi2s);                              /* HAL辅助宏/函数 */

#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
  if (hi2s->MspDeInitCallback == NULL)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hi2s->MspDeInitCallback = HAL_I2S_MspDeInit; /* Legacy weak MspDeInit  */
  }                                                     /* 代码块结束 */

  /* DeInit the low level hardware: GPIO, CLOCK, NVIC... */
  hi2s->MspDeInitCallback(hi2s);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* DeInit the low level hardware: GPIO, CLOCK, NVIC... */
  HAL_I2S_MspDeInit(hi2s);                              /* HAL库函数调用 */
#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */

  hi2s->ErrorCode = HAL_I2S_ERROR_NONE;                 /* 赋值或初始化 */
  hi2s->State     = HAL_I2S_STATE_RESET;                /* 赋值或初始化 */

  /* Release Lock */
  __HAL_UNLOCK(hi2s);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief I2S MSP Init
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval None
  */
__weak void HAL_I2S_MspInit(I2S_HandleTypeDef *hi2s)    /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hi2s);                                         /* 执行语句 */

  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_I2S_MspInit could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief I2S MSP DeInit
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval None
  */
__weak void HAL_I2S_MspDeInit(I2S_HandleTypeDef *hi2s)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hi2s);                                         /* 执行语句 */

  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_I2S_MspDeInit could be implemented in the user file
   */
}                                                       /* 代码块结束 */

#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
/**
  * @brief  Register a User I2S Callback
  *         To be used instead of the weak predefined callback
  * @param  hi2s Pointer to a I2S_HandleTypeDef structure that contains
  *                the configuration information for the specified I2S.
  * @param  CallbackID ID of the callback to be registered
  * @param  pCallback pointer to the Callback function
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2S_RegisterCallback(I2S_HandleTypeDef *hi2s, HAL_I2S_CallbackIDTypeDef CallbackID,  /* HAL状态类型函数 */
                                           pI2S_CallbackTypeDef pCallback)  /* 源代码 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  if (pCallback == NULL)                                /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hi2s->ErrorCode |= HAL_I2S_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  /* Process locked */
  __HAL_LOCK(hi2s);                                     /* HAL辅助宏/函数 */

  if (HAL_I2S_STATE_READY == hi2s->State)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_I2S_TX_COMPLETE_CB_ID :                  /* case标签 */
        hi2s->TxCpltCallback = pCallback;               /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_I2S_RX_COMPLETE_CB_ID :                  /* case标签 */
        hi2s->RxCpltCallback = pCallback;               /* 赋值或初始化 */
        break;                                          /* 跳出 */

#if defined (SPI_I2S_FULLDUPLEX_SUPPORT)                /* 条件编译 */
      case HAL_I2S_TX_RX_COMPLETE_CB_ID :               /* case标签 */
        hi2s->TxRxCpltCallback = pCallback;             /* 赋值或初始化 */
        break;                                          /* 跳出 */
#endif                                                  /* 条件编译结束 */

      case HAL_I2S_TX_HALF_COMPLETE_CB_ID :             /* case标签 */
        hi2s->TxHalfCpltCallback = pCallback;           /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_I2S_RX_HALF_COMPLETE_CB_ID :             /* case标签 */
        hi2s->RxHalfCpltCallback = pCallback;           /* 赋值或初始化 */
        break;                                          /* 跳出 */

#if defined (SPI_I2S_FULLDUPLEX_SUPPORT)                /* 条件编译 */
      case HAL_I2S_TX_RX_HALF_COMPLETE_CB_ID :          /* case标签 */
        hi2s->TxRxHalfCpltCallback = pCallback;         /* 赋值或初始化 */
        break;                                          /* 跳出 */
#endif                                                  /* 条件编译结束 */

      case HAL_I2S_ERROR_CB_ID :                        /* case标签 */
        hi2s->ErrorCallback = pCallback;                /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_I2S_MSPINIT_CB_ID :                      /* case标签 */
        hi2s->MspInitCallback = pCallback;              /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_I2S_MSPDEINIT_CB_ID :                    /* case标签 */
        hi2s->MspDeInitCallback = pCallback;            /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_INVALID_CALLBACK);  /* 执行语句 */

        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (HAL_I2S_STATE_RESET == hi2s->State)          /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_I2S_MSPINIT_CB_ID :                      /* case标签 */
        hi2s->MspInitCallback = pCallback;              /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_I2S_MSPDEINIT_CB_ID :                    /* case标签 */
        hi2s->MspDeInitCallback = pCallback;            /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_INVALID_CALLBACK);  /* 执行语句 */

        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_INVALID_CALLBACK);  /* 执行语句 */

    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hi2s);                                   /* HAL辅助宏/函数 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Unregister an I2S Callback
  *         I2S callback is redirected to the weak predefined callback
  * @param  hi2s Pointer to a I2S_HandleTypeDef structure that contains
  *                the configuration information for the specified I2S.
  * @param  CallbackID ID of the callback to be unregistered
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2S_UnRegisterCallback(I2S_HandleTypeDef *hi2s, HAL_I2S_CallbackIDTypeDef CallbackID)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Process locked */
  __HAL_LOCK(hi2s);                                     /* HAL辅助宏/函数 */

  if (HAL_I2S_STATE_READY == hi2s->State)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_I2S_TX_COMPLETE_CB_ID :                  /* case标签 */
        hi2s->TxCpltCallback = HAL_I2S_TxCpltCallback;                /* Legacy weak TxCpltCallback       */
        break;                                          /* 跳出 */

      case HAL_I2S_RX_COMPLETE_CB_ID :                  /* case标签 */
        hi2s->RxCpltCallback = HAL_I2S_RxCpltCallback;                /* Legacy weak RxCpltCallback       */
        break;                                          /* 跳出 */

#if defined (SPI_I2S_FULLDUPLEX_SUPPORT)                /* 条件编译 */
      case HAL_I2S_TX_RX_COMPLETE_CB_ID :               /* case标签 */
        hi2s->TxRxCpltCallback = HAL_I2SEx_TxRxCpltCallback;          /* Legacy weak TxRxCpltCallback     */
        break;                                          /* 跳出 */
#endif                                                  /* 条件编译结束 */

      case HAL_I2S_TX_HALF_COMPLETE_CB_ID :             /* case标签 */
        hi2s->TxHalfCpltCallback = HAL_I2S_TxHalfCpltCallback;        /* Legacy weak TxHalfCpltCallback   */
        break;                                          /* 跳出 */

      case HAL_I2S_RX_HALF_COMPLETE_CB_ID :             /* case标签 */
        hi2s->RxHalfCpltCallback = HAL_I2S_RxHalfCpltCallback;        /* Legacy weak RxHalfCpltCallback   */
        break;                                          /* 跳出 */

#if defined (SPI_I2S_FULLDUPLEX_SUPPORT)                /* 条件编译 */
      case HAL_I2S_TX_RX_HALF_COMPLETE_CB_ID :          /* case标签 */
        hi2s->TxRxHalfCpltCallback = HAL_I2SEx_TxRxHalfCpltCallback;  /* Legacy weak TxRxHalfCpltCallback */
        break;                                          /* 跳出 */
#endif                                                  /* 条件编译结束 */

      case HAL_I2S_ERROR_CB_ID :                        /* case标签 */
        hi2s->ErrorCallback = HAL_I2S_ErrorCallback;                  /* Legacy weak ErrorCallback        */
        break;                                          /* 跳出 */

      case HAL_I2S_MSPINIT_CB_ID :                      /* case标签 */
        hi2s->MspInitCallback = HAL_I2S_MspInit;                      /* Legacy weak MspInit              */
        break;                                          /* 跳出 */

      case HAL_I2S_MSPDEINIT_CB_ID :                    /* case标签 */
        hi2s->MspDeInitCallback = HAL_I2S_MspDeInit;                  /* Legacy weak MspDeInit            */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_INVALID_CALLBACK);  /* 执行语句 */

        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (HAL_I2S_STATE_RESET == hi2s->State)          /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_I2S_MSPINIT_CB_ID :                      /* case标签 */
        hi2s->MspInitCallback = HAL_I2S_MspInit;                      /* Legacy weak MspInit              */
        break;                                          /* 跳出 */

      case HAL_I2S_MSPDEINIT_CB_ID :                    /* case标签 */
        hi2s->MspDeInitCallback = HAL_I2S_MspDeInit;                  /* Legacy weak MspDeInit            */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_INVALID_CALLBACK);  /* 执行语句 */

        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_INVALID_CALLBACK);  /* 执行语句 */

    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hi2s);                                   /* HAL辅助宏/函数 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */
/**
  * @}
  */

/** @defgroup I2S_Exported_Functions_Group2 IO operation functions
  *  @brief Data transfers functions
  *
@verbatim
 ===============================================================================
                      ##### IO operation functions #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to manage the I2S data
    transfers.

    (#) There are two modes of transfer:
       (++) Blocking mode : The communication is performed in the polling mode.
            The status of all data processing is returned by the same function
            after finishing transfer.
       (++) No-Blocking mode : The communication is performed using Interrupts
            or DMA. These functions return the status of the transfer startup.
            The end of the data processing will be indicated through the
            dedicated I2S IRQ when using Interrupt mode or the DMA IRQ when
            using DMA mode.

    (#) Blocking mode functions are :
        (++) HAL_I2S_Transmit()
        (++) HAL_I2S_Receive()

    (#) No-Blocking mode functions with Interrupt are :
        (++) HAL_I2S_Transmit_IT()
        (++) HAL_I2S_Receive_IT()

    (#) No-Blocking mode functions with DMA are :
        (++) HAL_I2S_Transmit_DMA()
        (++) HAL_I2S_Receive_DMA()

    (#) A set of Transfer Complete Callbacks are provided in non Blocking mode:
        (++) HAL_I2S_TxCpltCallback()
        (++) HAL_I2S_RxCpltCallback()
        (++) HAL_I2S_ErrorCallback()

@endverbatim
  * @{
  */

/**
  * @brief  Transmit an amount of data in blocking mode
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @param  pData a 16-bit pointer to data buffer.
  * @param  Size number of data sample to be sent:
  * @note   When a 16-bit data frame or a 16-bit data frame extended is selected during the I2S
  *         configuration phase, the Size parameter means the number of 16-bit data length
  *         in the transaction and when a 24-bit data frame or a 32-bit data frame is selected
  *         the Size parameter means the number of 24-bit or 32-bit data length.
  * @param  Timeout Timeout duration
  * @note   The I2S is kept enabled at the end of transaction to avoid the clock de-synchronization
  *         between Master and Slave(example: audio streaming).
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2S_Transmit(I2S_HandleTypeDef *hi2s, uint16_t *pData, uint16_t Size, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpreg_cfgr;                                 /* 32位无符号变量/函数 */

  if ((pData == NULL) || (Size == 0U))                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return  HAL_ERROR;                                  /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Process Locked */
  __HAL_LOCK(hi2s);                                     /* HAL辅助宏/函数 */

  if (hi2s->State != HAL_I2S_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    __HAL_UNLOCK(hi2s);                                 /* HAL辅助宏/函数 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Set state and reset error code */
  hi2s->State = HAL_I2S_STATE_BUSY_TX;                  /* 赋值或初始化 */
  hi2s->ErrorCode = HAL_I2S_ERROR_NONE;                 /* 赋值或初始化 */
  hi2s->pTxBuffPtr = pData;                             /* 赋值或初始化 */

  tmpreg_cfgr = hi2s->Instance->I2SCFGR & (SPI_I2SCFGR_DATLEN | SPI_I2SCFGR_CHLEN);  /* 赋值或初始化 */

  if ((tmpreg_cfgr == I2S_DATAFORMAT_24B) || (tmpreg_cfgr == I2S_DATAFORMAT_32B))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hi2s->TxXferSize = (Size << 1U);                    /* 赋值或初始化 */
    hi2s->TxXferCount = (Size << 1U);                   /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    hi2s->TxXferSize = Size;                            /* 赋值或初始化 */
    hi2s->TxXferCount = Size;                           /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  tmpreg_cfgr = hi2s->Instance->I2SCFGR;                /* 赋值或初始化 */

  /* Check if the I2S is already enabled */
  if ((hi2s->Instance->I2SCFGR & SPI_I2SCFGR_I2SE) != SPI_I2SCFGR_I2SE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable I2S peripheral */
    __HAL_I2S_ENABLE(hi2s);                             /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Wait until TXE flag is set */
  if (I2S_WaitFlagStateUntilTimeout(hi2s, I2S_FLAG_TXE, SET, Timeout) != HAL_OK)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Set the error code */
    SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_TIMEOUT);    /* 执行语句 */
    hi2s->State = HAL_I2S_STATE_READY;                  /* 赋值或初始化 */
    __HAL_UNLOCK(hi2s);                                 /* HAL辅助宏/函数 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  while (hi2s->TxXferCount > 0U)                        /* while循环 */
  {                                                     /* 代码块开始 */
    hi2s->Instance->DR = (*hi2s->pTxBuffPtr);           /* 赋值或初始化 */
    hi2s->pTxBuffPtr++;                                 /* 执行语句 */
    hi2s->TxXferCount--;                                /* 执行语句 */

    /* Wait until TXE flag is set */
    if (I2S_WaitFlagStateUntilTimeout(hi2s, I2S_FLAG_TXE, SET, Timeout) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the error code */
      SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_TIMEOUT);  /* 执行语句 */
      hi2s->State = HAL_I2S_STATE_READY;                /* 赋值或初始化 */
      __HAL_UNLOCK(hi2s);                               /* HAL辅助宏/函数 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Check if an underrun occurs */
    if (__HAL_I2S_GET_FLAG(hi2s, I2S_FLAG_UDR) == SET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear underrun flag */
      __HAL_I2S_CLEAR_UDRFLAG(hi2s);                    /* HAL辅助宏/函数 */

      /* Set the error code */
      SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_UDR);      /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Check if Slave mode is selected */
  if (((tmpreg_cfgr & SPI_I2SCFGR_I2SCFG) == I2S_MODE_SLAVE_TX)  /* 条件判断 */
      || ((tmpreg_cfgr & SPI_I2SCFGR_I2SCFG) == I2S_MODE_SLAVE_RX))  /* 源代码 */
  {                                                     /* 代码块开始 */
    /* Wait until Busy flag is reset */
    if (I2S_WaitFlagStateUntilTimeout(hi2s, I2S_FLAG_BSY, RESET, Timeout) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the error code */
      SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_TIMEOUT);  /* 执行语句 */
      hi2s->State = HAL_I2S_STATE_READY;                /* 赋值或初始化 */
      __HAL_UNLOCK(hi2s);                               /* HAL辅助宏/函数 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  hi2s->State = HAL_I2S_STATE_READY;                    /* 赋值或初始化 */
  __HAL_UNLOCK(hi2s);                                   /* HAL辅助宏/函数 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Receive an amount of data in blocking mode
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @param  pData a 16-bit pointer to data buffer.
  * @param  Size number of data sample to be sent:
  * @note   When a 16-bit data frame or a 16-bit data frame extended is selected during the I2S
  *         configuration phase, the Size parameter means the number of 16-bit data length
  *         in the transaction and when a 24-bit data frame or a 32-bit data frame is selected
  *         the Size parameter means the number of 24-bit or 32-bit data length.
  * @param  Timeout Timeout duration
  * @note   The I2S is kept enabled at the end of transaction to avoid the clock de-synchronization
  *         between Master and Slave(example: audio streaming).
  * @note   In I2S Master Receiver mode, just after enabling the peripheral the clock will be generate
  *         in continuous way and as the I2S is not disabled at the end of the I2S transaction.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2S_Receive(I2S_HandleTypeDef *hi2s, uint16_t *pData, uint16_t Size, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpreg_cfgr;                                 /* 32位无符号变量/函数 */

  if ((pData == NULL) || (Size == 0U))                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return  HAL_ERROR;                                  /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Process Locked */
  __HAL_LOCK(hi2s);                                     /* HAL辅助宏/函数 */

  if (hi2s->State != HAL_I2S_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    __HAL_UNLOCK(hi2s);                                 /* HAL辅助宏/函数 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Set state and reset error code */
  hi2s->State = HAL_I2S_STATE_BUSY_RX;                  /* 赋值或初始化 */
  hi2s->ErrorCode = HAL_I2S_ERROR_NONE;                 /* 赋值或初始化 */
  hi2s->pRxBuffPtr = pData;                             /* 赋值或初始化 */

  tmpreg_cfgr = hi2s->Instance->I2SCFGR & (SPI_I2SCFGR_DATLEN | SPI_I2SCFGR_CHLEN);  /* 赋值或初始化 */

  if ((tmpreg_cfgr == I2S_DATAFORMAT_24B) || (tmpreg_cfgr == I2S_DATAFORMAT_32B))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hi2s->RxXferSize = (Size << 1U);                    /* 赋值或初始化 */
    hi2s->RxXferCount = (Size << 1U);                   /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    hi2s->RxXferSize = Size;                            /* 赋值或初始化 */
    hi2s->RxXferCount = Size;                           /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Check if the I2S is already enabled */
  if ((hi2s->Instance->I2SCFGR & SPI_I2SCFGR_I2SE) != SPI_I2SCFGR_I2SE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable I2S peripheral */
    __HAL_I2S_ENABLE(hi2s);                             /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Check if Master Receiver mode is selected */
  if ((hi2s->Instance->I2SCFGR & SPI_I2SCFGR_I2SCFG) == I2S_MODE_MASTER_RX)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Clear the Overrun Flag by a read operation on the SPI_DR register followed by a read
    access to the SPI_SR register. */
    __HAL_I2S_CLEAR_OVRFLAG(hi2s);                      /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Receive data */
  while (hi2s->RxXferCount > 0U)                        /* while循环 */
  {                                                     /* 代码块开始 */
    /* Wait until RXNE flag is set */
    if (I2S_WaitFlagStateUntilTimeout(hi2s, I2S_FLAG_RXNE, SET, Timeout) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the error code */
      SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_TIMEOUT);  /* 执行语句 */
      hi2s->State = HAL_I2S_STATE_READY;                /* 赋值或初始化 */
      __HAL_UNLOCK(hi2s);                               /* HAL辅助宏/函数 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    (*hi2s->pRxBuffPtr) = (uint16_t)hi2s->Instance->DR;  /* 赋值或初始化 */
    hi2s->pRxBuffPtr++;                                 /* 执行语句 */
    hi2s->RxXferCount--;                                /* 执行语句 */

    /* Check if an overrun occurs */
    if (__HAL_I2S_GET_FLAG(hi2s, I2S_FLAG_OVR) == SET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear overrun flag */
      __HAL_I2S_CLEAR_OVRFLAG(hi2s);                    /* HAL辅助宏/函数 */

      /* Set the error code */
      SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_OVR);      /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  hi2s->State = HAL_I2S_STATE_READY;                    /* 赋值或初始化 */
  __HAL_UNLOCK(hi2s);                                   /* HAL辅助宏/函数 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Transmit an amount of data in non-blocking mode with Interrupt
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @param  pData a 16-bit pointer to data buffer.
  * @param  Size number of data sample to be sent:
  * @note   When a 16-bit data frame or a 16-bit data frame extended is selected during the I2S
  *         configuration phase, the Size parameter means the number of 16-bit data length
  *         in the transaction and when a 24-bit data frame or a 32-bit data frame is selected
  *         the Size parameter means the number of 24-bit or 32-bit data length.
  * @note   The I2S is kept enabled at the end of transaction to avoid the clock de-synchronization
  *         between Master and Slave(example: audio streaming).
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2S_Transmit_IT(I2S_HandleTypeDef *hi2s, uint16_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpreg_cfgr;                                 /* 32位无符号变量/函数 */

  if ((pData == NULL) || (Size == 0U))                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return  HAL_ERROR;                                  /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Process Locked */
  __HAL_LOCK(hi2s);                                     /* HAL辅助宏/函数 */

  if (hi2s->State != HAL_I2S_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    __HAL_UNLOCK(hi2s);                                 /* HAL辅助宏/函数 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Set state and reset error code */
  hi2s->State = HAL_I2S_STATE_BUSY_TX;                  /* 赋值或初始化 */
  hi2s->ErrorCode = HAL_I2S_ERROR_NONE;                 /* 赋值或初始化 */
  hi2s->pTxBuffPtr = pData;                             /* 赋值或初始化 */

  tmpreg_cfgr = hi2s->Instance->I2SCFGR & (SPI_I2SCFGR_DATLEN | SPI_I2SCFGR_CHLEN);  /* 赋值或初始化 */

  if ((tmpreg_cfgr == I2S_DATAFORMAT_24B) || (tmpreg_cfgr == I2S_DATAFORMAT_32B))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hi2s->TxXferSize = (Size << 1U);                    /* 赋值或初始化 */
    hi2s->TxXferCount = (Size << 1U);                   /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    hi2s->TxXferSize = Size;                            /* 赋值或初始化 */
    hi2s->TxXferCount = Size;                           /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Enable TXE and ERR interrupt */
  __HAL_I2S_ENABLE_IT(hi2s, (I2S_IT_TXE | I2S_IT_ERR));  /* HAL辅助宏/函数 */

  /* Check if the I2S is already enabled */
  if ((hi2s->Instance->I2SCFGR & SPI_I2SCFGR_I2SE) != SPI_I2SCFGR_I2SE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable I2S peripheral */
    __HAL_I2S_ENABLE(hi2s);                             /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  __HAL_UNLOCK(hi2s);                                   /* HAL辅助宏/函数 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Receive an amount of data in non-blocking mode with Interrupt
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @param  pData a 16-bit pointer to the Receive data buffer.
  * @param  Size number of data sample to be sent:
  * @note   When a 16-bit data frame or a 16-bit data frame extended is selected during the I2S
  *         configuration phase, the Size parameter means the number of 16-bit data length
  *         in the transaction and when a 24-bit data frame or a 32-bit data frame is selected
  *         the Size parameter means the number of 24-bit or 32-bit data length.
  * @note   The I2S is kept enabled at the end of transaction to avoid the clock de-synchronization
  *         between Master and Slave(example: audio streaming).
  * @note   It is recommended to use DMA for the I2S receiver to avoid de-synchronization
  * between Master and Slave otherwise the I2S interrupt should be optimized.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2S_Receive_IT(I2S_HandleTypeDef *hi2s, uint16_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpreg_cfgr;                                 /* 32位无符号变量/函数 */

  if ((pData == NULL) || (Size == 0U))                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return  HAL_ERROR;                                  /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Process Locked */
  __HAL_LOCK(hi2s);                                     /* HAL辅助宏/函数 */

  if (hi2s->State != HAL_I2S_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    __HAL_UNLOCK(hi2s);                                 /* HAL辅助宏/函数 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Set state and reset error code */
  hi2s->State = HAL_I2S_STATE_BUSY_RX;                  /* 赋值或初始化 */
  hi2s->ErrorCode = HAL_I2S_ERROR_NONE;                 /* 赋值或初始化 */
  hi2s->pRxBuffPtr = pData;                             /* 赋值或初始化 */

  tmpreg_cfgr = hi2s->Instance->I2SCFGR & (SPI_I2SCFGR_DATLEN | SPI_I2SCFGR_CHLEN);  /* 赋值或初始化 */

  if ((tmpreg_cfgr == I2S_DATAFORMAT_24B) || (tmpreg_cfgr == I2S_DATAFORMAT_32B))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hi2s->RxXferSize = (Size << 1U);                    /* 赋值或初始化 */
    hi2s->RxXferCount = (Size << 1U);                   /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    hi2s->RxXferSize = Size;                            /* 赋值或初始化 */
    hi2s->RxXferCount = Size;                           /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Enable RXNE and ERR interrupt */
  __HAL_I2S_ENABLE_IT(hi2s, (I2S_IT_RXNE | I2S_IT_ERR));  /* HAL辅助宏/函数 */

  /* Check if the I2S is already enabled */
  if ((hi2s->Instance->I2SCFGR & SPI_I2SCFGR_I2SE) != SPI_I2SCFGR_I2SE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable I2S peripheral */
    __HAL_I2S_ENABLE(hi2s);                             /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  __HAL_UNLOCK(hi2s);                                   /* HAL辅助宏/函数 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Transmit an amount of data in non-blocking mode with DMA
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @param  pData a 16-bit pointer to the Transmit data buffer.
  * @param  Size number of data sample to be sent:
  * @note   When a 16-bit data frame or a 16-bit data frame extended is selected during the I2S
  *         configuration phase, the Size parameter means the number of 16-bit data length
  *         in the transaction and when a 24-bit data frame or a 32-bit data frame is selected
  *         the Size parameter means the number of 24-bit or 32-bit data length.
  * @note   The I2S is kept enabled at the end of transaction to avoid the clock de-synchronization
  *         between Master and Slave(example: audio streaming).
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2S_Transmit_DMA(I2S_HandleTypeDef *hi2s, uint16_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpreg_cfgr;                                 /* 32位无符号变量/函数 */

  if ((pData == NULL) || (Size == 0U))                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return  HAL_ERROR;                                  /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Process Locked */
  __HAL_LOCK(hi2s);                                     /* HAL辅助宏/函数 */

  if (hi2s->State != HAL_I2S_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    __HAL_UNLOCK(hi2s);                                 /* HAL辅助宏/函数 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Set state and reset error code */
  hi2s->State = HAL_I2S_STATE_BUSY_TX;                  /* 赋值或初始化 */
  hi2s->ErrorCode = HAL_I2S_ERROR_NONE;                 /* 赋值或初始化 */
  hi2s->pTxBuffPtr = pData;                             /* 赋值或初始化 */

  tmpreg_cfgr = hi2s->Instance->I2SCFGR & (SPI_I2SCFGR_DATLEN | SPI_I2SCFGR_CHLEN);  /* 赋值或初始化 */

  if ((tmpreg_cfgr == I2S_DATAFORMAT_24B) || (tmpreg_cfgr == I2S_DATAFORMAT_32B))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hi2s->TxXferSize = (Size << 1U);                    /* 赋值或初始化 */
    hi2s->TxXferCount = (Size << 1U);                   /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    hi2s->TxXferSize = Size;                            /* 赋值或初始化 */
    hi2s->TxXferCount = Size;                           /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Set the I2S Tx DMA Half transfer complete callback */
  hi2s->hdmatx->XferHalfCpltCallback = I2S_DMATxHalfCplt;  /* 赋值或初始化 */

  /* Set the I2S Tx DMA transfer complete callback */
  hi2s->hdmatx->XferCpltCallback = I2S_DMATxCplt;       /* 赋值或初始化 */

  /* Set the DMA error callback */
  hi2s->hdmatx->XferErrorCallback = I2S_DMAError;       /* 赋值或初始化 */

  /* Enable the Tx DMA Stream/Channel */
  if (HAL_OK != HAL_DMA_Start_IT(hi2s->hdmatx,          /* 条件判断 */
                                 (uint32_t)hi2s->pTxBuffPtr,  /* 列表续行 */
                                 (uint32_t)&hi2s->Instance->DR,  /* 列表续行 */
                                 hi2s->TxXferSize))     /* 源代码 */
  {                                                     /* 代码块开始 */
    /* Update SPI error code */
    SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_DMA);        /* 执行语句 */
    hi2s->State = HAL_I2S_STATE_READY;                  /* 赋值或初始化 */

    __HAL_UNLOCK(hi2s);                                 /* HAL辅助宏/函数 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check if the I2S is already enabled */
  if (HAL_IS_BIT_CLR(hi2s->Instance->I2SCFGR, SPI_I2SCFGR_I2SE))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable I2S peripheral */
    __HAL_I2S_ENABLE(hi2s);                             /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Check if the I2S Tx request is already enabled */
  if (HAL_IS_BIT_CLR(hi2s->Instance->CR2, SPI_CR2_TXDMAEN))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable Tx DMA Request */
    SET_BIT(hi2s->Instance->CR2, SPI_CR2_TXDMAEN);      /* 执行语句 */
  }                                                     /* 代码块结束 */

  __HAL_UNLOCK(hi2s);                                   /* HAL辅助宏/函数 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Receive an amount of data in non-blocking mode with DMA
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @param  pData a 16-bit pointer to the Receive data buffer.
  * @param  Size number of data sample to be sent:
  * @note   When a 16-bit data frame or a 16-bit data frame extended is selected during the I2S
  *         configuration phase, the Size parameter means the number of 16-bit data length
  *         in the transaction and when a 24-bit data frame or a 32-bit data frame is selected
  *         the Size parameter means the number of 24-bit or 32-bit data length.
  * @note   The I2S is kept enabled at the end of transaction to avoid the clock de-synchronization
  *         between Master and Slave(example: audio streaming).
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2S_Receive_DMA(I2S_HandleTypeDef *hi2s, uint16_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpreg_cfgr;                                 /* 32位无符号变量/函数 */

  if ((pData == NULL) || (Size == 0U))                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return  HAL_ERROR;                                  /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Process Locked */
  __HAL_LOCK(hi2s);                                     /* HAL辅助宏/函数 */

  if (hi2s->State != HAL_I2S_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    __HAL_UNLOCK(hi2s);                                 /* HAL辅助宏/函数 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Set state and reset error code */
  hi2s->State = HAL_I2S_STATE_BUSY_RX;                  /* 赋值或初始化 */
  hi2s->ErrorCode = HAL_I2S_ERROR_NONE;                 /* 赋值或初始化 */
  hi2s->pRxBuffPtr = pData;                             /* 赋值或初始化 */

  tmpreg_cfgr = hi2s->Instance->I2SCFGR & (SPI_I2SCFGR_DATLEN | SPI_I2SCFGR_CHLEN);  /* 赋值或初始化 */

  if ((tmpreg_cfgr == I2S_DATAFORMAT_24B) || (tmpreg_cfgr == I2S_DATAFORMAT_32B))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hi2s->RxXferSize = (Size << 1U);                    /* 赋值或初始化 */
    hi2s->RxXferCount = (Size << 1U);                   /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    hi2s->RxXferSize = Size;                            /* 赋值或初始化 */
    hi2s->RxXferCount = Size;                           /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Set the I2S Rx DMA Half transfer complete callback */
  hi2s->hdmarx->XferHalfCpltCallback = I2S_DMARxHalfCplt;  /* 赋值或初始化 */

  /* Set the I2S Rx DMA transfer complete callback */
  hi2s->hdmarx->XferCpltCallback = I2S_DMARxCplt;       /* 赋值或初始化 */

  /* Set the DMA error callback */
  hi2s->hdmarx->XferErrorCallback = I2S_DMAError;       /* 赋值或初始化 */

  /* Check if Master Receiver mode is selected */
  if ((hi2s->Instance->I2SCFGR & SPI_I2SCFGR_I2SCFG) == I2S_MODE_MASTER_RX)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Clear the Overrun Flag by a read operation to the SPI_DR register followed by a read
    access to the SPI_SR register. */
    __HAL_I2S_CLEAR_OVRFLAG(hi2s);                      /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Enable the Rx DMA Stream/Channel */
  if (HAL_OK != HAL_DMA_Start_IT(hi2s->hdmarx, (uint32_t)&hi2s->Instance->DR, (uint32_t)hi2s->pRxBuffPtr,  /* 条件判断 */
                                 hi2s->RxXferSize))     /* 源代码 */
  {                                                     /* 代码块开始 */
    /* Update SPI error code */
    SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_DMA);        /* 执行语句 */
    hi2s->State = HAL_I2S_STATE_READY;                  /* 赋值或初始化 */

    __HAL_UNLOCK(hi2s);                                 /* HAL辅助宏/函数 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check if the I2S is already enabled */
  if (HAL_IS_BIT_CLR(hi2s->Instance->I2SCFGR, SPI_I2SCFGR_I2SE))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable I2S peripheral */
    __HAL_I2S_ENABLE(hi2s);                             /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Check if the I2S Rx request is already enabled */
  if (HAL_IS_BIT_CLR(hi2s->Instance->CR2, SPI_CR2_RXDMAEN))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable Rx DMA Request */
    SET_BIT(hi2s->Instance->CR2, SPI_CR2_RXDMAEN);      /* 执行语句 */
  }                                                     /* 代码块结束 */

  __HAL_UNLOCK(hi2s);                                   /* HAL辅助宏/函数 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Pauses the audio DMA Stream/Channel playing from the Media.
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2S_DMAPause(I2S_HandleTypeDef *hi2s)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Process Locked */
  __HAL_LOCK(hi2s);                                     /* HAL辅助宏/函数 */

  if (hi2s->State == HAL_I2S_STATE_BUSY_TX)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable the I2S DMA Tx request */
    CLEAR_BIT(hi2s->Instance->CR2, SPI_CR2_TXDMAEN);    /* 执行语句 */
  }                                                     /* 代码块结束 */
  else if (hi2s->State == HAL_I2S_STATE_BUSY_RX)        /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* Disable the I2S DMA Rx request */
    CLEAR_BIT(hi2s->Instance->CR2, SPI_CR2_RXDMAEN);    /* 执行语句 */
  }                                                     /* 代码块结束 */
#if defined (SPI_I2S_FULLDUPLEX_SUPPORT)                /* 条件编译 */
  else if (hi2s->State == HAL_I2S_STATE_BUSY_TX_RX)     /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* Pause the audio file playing by disabling the I2S DMA request */
    CLEAR_BIT(hi2s->Instance->CR2, (SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN));  /* 执行语句 */
    CLEAR_BIT(I2SxEXT(hi2s->Instance)->CR2, (SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN));  /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif /* SPI_I2S_FULLDUPLEX_SUPPORT */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* nothing to do */
  }                                                     /* 代码块结束 */

  /* Process Unlocked */
  __HAL_UNLOCK(hi2s);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Resumes the audio DMA Stream/Channel playing from the Media.
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2S_DMAResume(I2S_HandleTypeDef *hi2s)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Process Locked */
  __HAL_LOCK(hi2s);                                     /* HAL辅助宏/函数 */

  if (hi2s->State == HAL_I2S_STATE_BUSY_TX)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable the I2S DMA Tx request */
    SET_BIT(hi2s->Instance->CR2, SPI_CR2_TXDMAEN);      /* 执行语句 */
  }                                                     /* 代码块结束 */
  else if (hi2s->State == HAL_I2S_STATE_BUSY_RX)        /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* Enable the I2S DMA Rx request */
    SET_BIT(hi2s->Instance->CR2, SPI_CR2_RXDMAEN);      /* 执行语句 */
  }                                                     /* 代码块结束 */
#if defined (SPI_I2S_FULLDUPLEX_SUPPORT)                /* 条件编译 */
  else if (hi2s->State == HAL_I2S_STATE_BUSY_TX_RX)     /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* Pause the audio file playing by disabling the I2S DMA request */
    SET_BIT(hi2s->Instance->CR2, (SPI_CR2_RXDMAEN | SPI_CR2_TXDMAEN));  /* 执行语句 */
    SET_BIT(I2SxEXT(hi2s->Instance)->CR2, (SPI_CR2_RXDMAEN | SPI_CR2_TXDMAEN));  /* 执行语句 */

    /* If the I2Sext peripheral is still not enabled, enable it */
    if ((I2SxEXT(hi2s->Instance)->I2SCFGR & SPI_I2SCFGR_I2SE) == 0U)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable I2Sext peripheral */
      __HAL_I2SEXT_ENABLE(hi2s);                        /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
#endif /* SPI_I2S_FULLDUPLEX_SUPPORT */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* nothing to do */
  }                                                     /* 代码块结束 */

  /* If the I2S peripheral is still not enabled, enable it */
  if (HAL_IS_BIT_CLR(hi2s->Instance->I2SCFGR, SPI_I2SCFGR_I2SE))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable I2S peripheral */
    __HAL_I2S_ENABLE(hi2s);                             /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Process Unlocked */
  __HAL_UNLOCK(hi2s);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Stops the audio DMA Stream/Channel playing from the Media.
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2S_DMAStop(I2S_HandleTypeDef *hi2s)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
#if defined (SPI_I2S_FULLDUPLEX_SUPPORT)                /* 条件编译 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */
#endif /* SPI_I2S_FULLDUPLEX_SUPPORT */
  HAL_StatusTypeDef errorcode = HAL_OK;                 /* HAL状态类型函数 */
  /* The Lock is not implemented on this API to allow the user application
     to call the HAL SPI API under callbacks HAL_I2S_TxCpltCallback() or HAL_I2S_RxCpltCallback()
     when calling HAL_DMA_Abort() API the DMA TX or RX Transfer complete interrupt is generated
     and the correspond call back is executed HAL_I2S_TxCpltCallback() or HAL_I2S_RxCpltCallback()
     */

  if ((hi2s->Init.Mode == I2S_MODE_MASTER_TX) || (hi2s->Init.Mode == I2S_MODE_SLAVE_TX))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Abort the I2S DMA tx Stream/Channel */
    if (hi2s->hdmatx != NULL)                           /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable the I2S DMA tx Stream/Channel */
      if (HAL_OK != HAL_DMA_Abort(hi2s->hdmatx))        /* 条件判断 */
      {                                                 /* 代码块开始 */
        SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_DMA);    /* 执行语句 */
        errorcode = HAL_ERROR;                          /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Wait until TXE flag is set */
    if (I2S_WaitFlagStateUntilTimeout(hi2s, I2S_FLAG_TXE, SET, I2S_TIMEOUT_FLAG) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the error code */
      SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_TIMEOUT);  /* 执行语句 */
      hi2s->State = HAL_I2S_STATE_READY;                /* 赋值或初始化 */
      errorcode   = HAL_ERROR;                          /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Wait until BSY flag is Reset */
    if (I2S_WaitFlagStateUntilTimeout(hi2s, I2S_FLAG_BSY, RESET, I2S_TIMEOUT_FLAG) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the error code */
      SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_TIMEOUT);  /* 执行语句 */
      hi2s->State = HAL_I2S_STATE_READY;                /* 赋值或初始化 */
      errorcode   = HAL_ERROR;                          /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Disable I2S peripheral */
    __HAL_I2S_DISABLE(hi2s);                            /* HAL辅助宏/函数 */

    /* Clear UDR flag */
    __HAL_I2S_CLEAR_UDRFLAG(hi2s);                      /* HAL辅助宏/函数 */

    /* Disable the I2S Tx DMA requests */
    CLEAR_BIT(hi2s->Instance->CR2, SPI_CR2_TXDMAEN);    /* 执行语句 */

#if defined (SPI_I2S_FULLDUPLEX_SUPPORT)                /* 条件编译 */

    if (hi2s->State == HAL_I2S_STATE_BUSY_TX_RX)        /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Abort the I2S DMA rx Stream/Channel */
      if (hi2s->hdmarx != NULL)                         /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Disable the I2S DMA rx Stream/Channel */
        if (HAL_OK != HAL_DMA_Abort(hi2s->hdmarx))      /* 条件判断 */
        {                                               /* 代码块开始 */
          SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_DMA);  /* 执行语句 */
          errorcode = HAL_ERROR;                        /* 赋值或初始化 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */

      /* Disable I2Sext peripheral */
      __HAL_I2SEXT_DISABLE(hi2s);                       /* HAL辅助宏/函数 */

      /* Clear OVR flag */
      __HAL_I2SEXT_CLEAR_OVRFLAG(hi2s);                 /* HAL辅助宏/函数 */

      /* Disable the I2SxEXT DMA request */
      CLEAR_BIT(I2SxEXT(hi2s->Instance)->CR2, SPI_CR2_RXDMAEN);  /* 执行语句 */

      if (hi2s->Init.Mode == I2S_MODE_SLAVE_TX)         /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Set the error code */
        SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_BUSY_LINE_RX);  /* 执行语句 */

        /* Set the I2S State ready */
        hi2s->State = HAL_I2S_STATE_READY;              /* 赋值或初始化 */
        errorcode = HAL_ERROR;                          /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Read DR to Flush RX Data */
        READ_REG(I2SxEXT(hi2s->Instance)->DR);          /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
#endif /* SPI_I2S_FULLDUPLEX_SUPPORT */
  }                                                     /* 代码块结束 */

  else if ((hi2s->Init.Mode == I2S_MODE_MASTER_RX) || (hi2s->Init.Mode == I2S_MODE_SLAVE_RX))  /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* Abort the I2S DMA rx Stream/Channel */
    if (hi2s->hdmarx != NULL)                           /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable the I2S DMA rx Stream/Channel */
      if (HAL_OK != HAL_DMA_Abort(hi2s->hdmarx))        /* 条件判断 */
      {                                                 /* 代码块开始 */
        SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_DMA);    /* 执行语句 */
        errorcode = HAL_ERROR;                          /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
#if defined (SPI_I2S_FULLDUPLEX_SUPPORT)                /* 条件编译 */

    if (hi2s->State == HAL_I2S_STATE_BUSY_TX_RX)        /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Abort the I2S DMA tx Stream/Channel */
      if (hi2s->hdmatx != NULL)                         /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Disable the I2S DMA tx Stream/Channel */
        if (HAL_OK != HAL_DMA_Abort(hi2s->hdmatx))      /* 条件判断 */
        {                                               /* 代码块开始 */
          SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_DMA);  /* 执行语句 */
          errorcode = HAL_ERROR;                        /* 赋值或初始化 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */

      tickstart = HAL_GetTick();                        /* 赋值或初始化 */

      /* Wait until TXE flag is set */
      while (__HAL_I2SEXT_GET_FLAG(hi2s, I2S_FLAG_TXE) != SET)  /* while循环 */
      {                                                 /* 代码块开始 */
        if (((HAL_GetTick() - tickstart) > I2S_TIMEOUT_FLAG))  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Set the error code */
          SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_TIMEOUT);  /* 执行语句 */

          /* Set the I2S State ready */
          hi2s->State = HAL_I2S_STATE_READY;            /* 赋值或初始化 */
          errorcode   = HAL_ERROR;                      /* 赋值或初始化 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */

      /* Wait until BSY flag is Reset */
      while (__HAL_I2SEXT_GET_FLAG(hi2s, I2S_FLAG_BSY) != RESET)  /* while循环 */
      {                                                 /* 代码块开始 */
        if (((HAL_GetTick() - tickstart) > I2S_TIMEOUT_FLAG))  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Set the error code */
          SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_TIMEOUT);  /* 执行语句 */

          /* Set the I2S State ready */
          hi2s->State = HAL_I2S_STATE_READY;            /* 赋值或初始化 */
          errorcode   = HAL_ERROR;                      /* 赋值或初始化 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */

      /* Disable I2Sext peripheral */
      __HAL_I2SEXT_DISABLE(hi2s);                       /* HAL辅助宏/函数 */

      /* Clear UDR flag */
      __HAL_I2SEXT_CLEAR_UDRFLAG(hi2s);                 /* HAL辅助宏/函数 */

      /* Disable the I2SxEXT DMA request */
      CLEAR_BIT(I2SxEXT(hi2s->Instance)->CR2, SPI_CR2_TXDMAEN);  /* 执行语句 */
    }                                                   /* 代码块结束 */
#endif /* SPI_I2S_FULLDUPLEX_SUPPORT */

    /* Disable I2S peripheral */
    __HAL_I2S_DISABLE(hi2s);                            /* HAL辅助宏/函数 */

    /* Clear OVR flag */
    __HAL_I2S_CLEAR_OVRFLAG(hi2s);                      /* HAL辅助宏/函数 */

    /* Disable the I2S Rx DMA request */
    CLEAR_BIT(hi2s->Instance->CR2, SPI_CR2_RXDMAEN);    /* 执行语句 */

    if (hi2s->Init.Mode == I2S_MODE_SLAVE_RX)           /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the error code */
      SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_BUSY_LINE_RX);  /* 执行语句 */

      /* Set the I2S State ready */
      hi2s->State = HAL_I2S_STATE_READY;                /* 赋值或初始化 */
      errorcode = HAL_ERROR;                            /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Read DR to Flush RX Data */
      READ_REG((hi2s->Instance)->DR);                   /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  hi2s->State = HAL_I2S_STATE_READY;                    /* 赋值或初始化 */

  return errorcode;                                     /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  This function handles I2S interrupt request.
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval None
  */
void HAL_I2S_IRQHandler(I2S_HandleTypeDef *hi2s)        /* 中断服务函数 */
{                                                       /* 代码块开始 */
  /* Call the IrqHandler ISR set during HAL_I2S_INIT */
  hi2s->IrqHandlerISR(hi2s);                            /* 执行语句 */
}                                                       /* 代码块结束 */

/**
  * @brief  Tx Transfer Half completed callbacks
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval None
  */
__weak void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hi2s);                                         /* 执行语句 */

  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_I2S_TxHalfCpltCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Tx Transfer completed callbacks
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval None
  */
__weak void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hi2s);                                         /* 执行语句 */

  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_I2S_TxCpltCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Rx Transfer half completed callbacks
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval None
  */
__weak void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hi2s);                                         /* 执行语句 */

  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_I2S_RxHalfCpltCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Rx Transfer completed callbacks
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval None
  */
__weak void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hi2s);                                         /* 执行语句 */

  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_I2S_RxCpltCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  I2S error callbacks
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval None
  */
__weak void HAL_I2S_ErrorCallback(I2S_HandleTypeDef *hi2s)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hi2s);                                         /* 执行语句 */

  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_I2S_ErrorCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup I2S_Exported_Functions_Group3 Peripheral State and Errors functions
  *  @brief   Peripheral State functions
  *
@verbatim
 ===============================================================================
                      ##### Peripheral State and Errors functions #####
 ===============================================================================
    [..]
    This subsection permits to get in run-time the status of the peripheral
    and the data flow.

@endverbatim
  * @{
  */

/**
  * @brief  Return the I2S state
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval HAL state
  */
HAL_I2S_StateTypeDef HAL_I2S_GetState(I2S_HandleTypeDef *hi2s)  /* HAL库函数调用 */
{                                                       /* 代码块开始 */
  return hi2s->State;                                   /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return the I2S error code
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval I2S Error Code
  */
uint32_t HAL_I2S_GetError(I2S_HandleTypeDef *hi2s)      /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  return hi2s->ErrorCode;                               /* 返回值 */
}                                                       /* 代码块结束 */
/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup I2S_Private_Functions I2S Private Functions
  * @{
  */
/**
  * @brief  DMA I2S transmit process complete callback
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *                the configuration information for the specified DMA module.
  * @retval None
  */
static void I2S_DMATxCplt(DMA_HandleTypeDef *hdma)      /* 静态函数 */
{                                                       /* 代码块开始 */
  I2S_HandleTypeDef *hi2s = (I2S_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent; /* Derogation MISRAC2012-Rule-11.5 */

  /* if DMA is configured in DMA_NORMAL Mode */
  if (hdma->Init.Mode == DMA_NORMAL)                    /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable Tx DMA Request */
    CLEAR_BIT(hi2s->Instance->CR2, SPI_CR2_TXDMAEN);    /* 执行语句 */

    hi2s->TxXferCount = 0U;                             /* 赋值或初始化 */
    hi2s->State = HAL_I2S_STATE_READY;                  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  /* Call user Tx complete callback */
#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
  hi2s->TxCpltCallback(hi2s);                           /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_I2S_TxCpltCallback(hi2s);                         /* HAL库函数调用 */
#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA I2S transmit process half complete callback
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *                the configuration information for the specified DMA module.
  * @retval None
  */
static void I2S_DMATxHalfCplt(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  I2S_HandleTypeDef *hi2s = (I2S_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent; /* Derogation MISRAC2012-Rule-11.5 */

  /* Call user Tx half complete callback */
#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
  hi2s->TxHalfCpltCallback(hi2s);                       /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_I2S_TxHalfCpltCallback(hi2s);                     /* HAL库函数调用 */
#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA I2S receive process complete callback
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *                the configuration information for the specified DMA module.
  * @retval None
  */
static void I2S_DMARxCplt(DMA_HandleTypeDef *hdma)      /* 静态函数 */
{                                                       /* 代码块开始 */
  I2S_HandleTypeDef *hi2s = (I2S_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent; /* Derogation MISRAC2012-Rule-11.5 */

  /* if DMA is configured in DMA_NORMAL Mode */
  if (hdma->Init.Mode == DMA_NORMAL)                    /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable Rx DMA Request */
    CLEAR_BIT(hi2s->Instance->CR2, SPI_CR2_RXDMAEN);    /* 执行语句 */
    hi2s->RxXferCount = 0U;                             /* 赋值或初始化 */
    hi2s->State = HAL_I2S_STATE_READY;                  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  /* Call user Rx complete callback */
#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
  hi2s->RxCpltCallback(hi2s);                           /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_I2S_RxCpltCallback(hi2s);                         /* HAL库函数调用 */
#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA I2S receive process half complete callback
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *                the configuration information for the specified DMA module.
  * @retval None
  */
static void I2S_DMARxHalfCplt(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  I2S_HandleTypeDef *hi2s = (I2S_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent; /* Derogation MISRAC2012-Rule-11.5 */

  /* Call user Rx half complete callback */
#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
  hi2s->RxHalfCpltCallback(hi2s);                       /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_I2S_RxHalfCpltCallback(hi2s);                     /* HAL库函数调用 */
#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA I2S communication error callback
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *                the configuration information for the specified DMA module.
  * @retval None
  */
static void I2S_DMAError(DMA_HandleTypeDef *hdma)       /* 静态函数 */
{                                                       /* 代码块开始 */
  I2S_HandleTypeDef *hi2s = (I2S_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent; /* Derogation MISRAC2012-Rule-11.5 */

  /* Disable Rx and Tx DMA Request */
  CLEAR_BIT(hi2s->Instance->CR2, (SPI_CR2_RXDMAEN | SPI_CR2_TXDMAEN));  /* 执行语句 */
  hi2s->TxXferCount = 0U;                               /* 赋值或初始化 */
  hi2s->RxXferCount = 0U;                               /* 赋值或初始化 */

  hi2s->State = HAL_I2S_STATE_READY;                    /* 赋值或初始化 */

  /* Set the error code and execute error callback*/
  SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_DMA);          /* 执行语句 */
  /* Call user error callback */
#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
  hi2s->ErrorCallback(hi2s);                            /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_I2S_ErrorCallback(hi2s);                          /* HAL库函数调用 */
#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */
}                                                       /* 代码块结束 */

/**
  * @brief  Transmit an amount of data in non-blocking mode with Interrupt
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval None
  */
static void I2S_Transmit_IT(I2S_HandleTypeDef *hi2s)    /* 静态函数 */
{                                                       /* 代码块开始 */
  /* Transmit data */
  hi2s->Instance->DR = (*hi2s->pTxBuffPtr);             /* 赋值或初始化 */
  hi2s->pTxBuffPtr++;                                   /* 执行语句 */
  hi2s->TxXferCount--;                                  /* 执行语句 */

  if (hi2s->TxXferCount == 0U)                          /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable TXE and ERR interrupt */
    __HAL_I2S_DISABLE_IT(hi2s, (I2S_IT_TXE | I2S_IT_ERR));  /* HAL辅助宏/函数 */

    hi2s->State = HAL_I2S_STATE_READY;                  /* 赋值或初始化 */
    /* Call user Tx complete callback */
#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
    hi2s->TxCpltCallback(hi2s);                         /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_I2S_TxCpltCallback(hi2s);                       /* HAL库函数调用 */
#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Receive an amount of data in non-blocking mode with Interrupt
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval None
  */
static void I2S_Receive_IT(I2S_HandleTypeDef *hi2s)     /* 静态函数 */
{                                                       /* 代码块开始 */
  /* Receive data */
  (*hi2s->pRxBuffPtr) = (uint16_t)hi2s->Instance->DR;   /* 赋值或初始化 */
  hi2s->pRxBuffPtr++;                                   /* 执行语句 */
  hi2s->RxXferCount--;                                  /* 执行语句 */

  if (hi2s->RxXferCount == 0U)                          /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable RXNE and ERR interrupt */
    __HAL_I2S_DISABLE_IT(hi2s, (I2S_IT_RXNE | I2S_IT_ERR));  /* HAL辅助宏/函数 */

    hi2s->State = HAL_I2S_STATE_READY;                  /* 赋值或初始化 */
    /* Call user Rx complete callback */
#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
    hi2s->RxCpltCallback(hi2s);                         /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_I2S_RxCpltCallback(hi2s);                       /* HAL库函数调用 */
#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  This function handles I2S interrupt request.
  * @param  hi2s: pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval None
  */
static void I2S_IRQHandler(I2S_HandleTypeDef *hi2s)     /* 静态函数 */
{                                                       /* 代码块开始 */
  __IO uint32_t i2ssr = hi2s->Instance->SR;             /* 赋值或初始化 */

  if (hi2s->State == HAL_I2S_STATE_BUSY_RX)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* I2S in mode Receiver ------------------------------------------------*/
    if (((i2ssr & I2S_FLAG_RXNE) == I2S_FLAG_RXNE) && (__HAL_I2S_GET_IT_SOURCE(hi2s, I2S_IT_RXNE) != RESET))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      I2S_Receive_IT(hi2s);                             /* 执行语句 */
    }                                                   /* 代码块结束 */

    /* I2S Overrun error interrupt occurred -------------------------------------*/
    if (((i2ssr & I2S_FLAG_OVR) == I2S_FLAG_OVR) && (__HAL_I2S_GET_IT_SOURCE(hi2s, I2S_IT_ERR) != RESET))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable RXNE and ERR interrupt */
      __HAL_I2S_DISABLE_IT(hi2s, (I2S_IT_RXNE | I2S_IT_ERR));  /* HAL辅助宏/函数 */

      /* Clear Overrun flag */
      __HAL_I2S_CLEAR_OVRFLAG(hi2s);                    /* HAL辅助宏/函数 */

      /* Set the I2S State ready */
      hi2s->State = HAL_I2S_STATE_READY;                /* 赋值或初始化 */


      /* Set the error code and execute error callback*/
      SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_OVR);      /* 执行语句 */
      /* Call user error callback */
#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
      hi2s->ErrorCallback(hi2s);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_I2S_ErrorCallback(hi2s);                      /* HAL库函数调用 */
#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  if (hi2s->State == HAL_I2S_STATE_BUSY_TX)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* I2S in mode Transmitter -----------------------------------------------*/
    if (((i2ssr & I2S_FLAG_TXE) == I2S_FLAG_TXE) && (__HAL_I2S_GET_IT_SOURCE(hi2s, I2S_IT_TXE) != RESET))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      I2S_Transmit_IT(hi2s);                            /* 执行语句 */
    }                                                   /* 代码块结束 */

    /* I2S Underrun error interrupt occurred --------------------------------*/
    if (((i2ssr & I2S_FLAG_UDR) == I2S_FLAG_UDR) && (__HAL_I2S_GET_IT_SOURCE(hi2s, I2S_IT_ERR) != RESET))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable TXE and ERR interrupt */
      __HAL_I2S_DISABLE_IT(hi2s, (I2S_IT_TXE | I2S_IT_ERR));  /* HAL辅助宏/函数 */

      /* Clear Underrun flag */
      __HAL_I2S_CLEAR_UDRFLAG(hi2s);                    /* HAL辅助宏/函数 */

      /* Set the I2S State ready */
      hi2s->State = HAL_I2S_STATE_READY;                /* 赋值或初始化 */

      /* Set the error code and execute error callback*/
      SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_UDR);      /* 执行语句 */
      /* Call user error callback */
#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
      hi2s->ErrorCallback(hi2s);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_I2S_ErrorCallback(hi2s);                      /* HAL库函数调用 */
#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  This function handles I2S Communication Timeout.
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @param  Flag Flag checked
  * @param  State Value of the flag expected
  * @param  Timeout Duration of the timeout
  * @retval HAL status
  */
static HAL_StatusTypeDef I2S_WaitFlagStateUntilTimeout(I2S_HandleTypeDef *hi2s, uint32_t Flag, FlagStatus State,  /* 静态变量/函数 */
                                                       uint32_t Timeout)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */

  /* Get tick */
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  /* Wait until flag is set to status*/
  while (((__HAL_I2S_GET_FLAG(hi2s, Flag)) ? SET : RESET) != State)  /* while循环 */
  {                                                     /* 代码块开始 */
    if (Timeout != HAL_MAX_DELAY)                       /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (((HAL_GetTick() - tickstart) >= Timeout) || (Timeout == 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Set the I2S State ready */
        hi2s->State = HAL_I2S_STATE_READY;              /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hi2s);                             /* HAL辅助宏/函数 */

        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
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

#endif /* HAL_I2S_MODULE_ENABLED */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
