/* 文件: stm32f4xx_hal_spi.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_spi.c
  * @author  MCD Application Team
  * @brief   SPI HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Serial Peripheral Interface (SPI) peripheral:
  *           + Initialization and de-initialization functions
  *           + IO operation functions
  *           + Peripheral Control functions
  *           + Peripheral State functions
  *
  @verbatim
  ==============================================================================
                        ##### How to use this driver #####
  ==============================================================================
    [..]
      The SPI HAL driver can be used as follows:

      (#) Declare a SPI_HandleTypeDef handle structure, for example:
          SPI_HandleTypeDef  hspi;

      (#)Initialize the SPI low level resources by implementing the HAL_SPI_MspInit() API:
          (##) Enable the SPIx interface clock
          (##) SPI pins configuration
              (+++) Enable the clock for the SPI GPIOs
              (+++) Configure these SPI pins as alternate function push-pull
          (##) NVIC configuration if you need to use interrupt process
              (+++) Configure the SPIx interrupt priority
              (+++) Enable the NVIC SPI IRQ handle
          (##) DMA Configuration if you need to use DMA process
              (+++) Declare a DMA_HandleTypeDef handle structure for the transmit or receive Stream/Channel
              (+++) Enable the DMAx clock
              (+++) Configure the DMA handle parameters
              (+++) Configure the DMA Tx or Rx Stream/Channel
              (+++) Associate the initialized hdma_tx(or _rx)  handle to the hspi DMA Tx or Rx handle
              (+++) Configure the priority and enable the NVIC for the transfer complete interrupt on the DMA Tx or Rx Stream/Channel

      (#) Program the Mode, BidirectionalMode , Data size, Baudrate Prescaler, NSS
          management, Clock polarity and phase, FirstBit and CRC configuration in the hspi Init structure.

      (#) Initialize the SPI registers by calling the HAL_SPI_Init() API:
          (++) This API configures also the low level Hardware GPIO, CLOCK, CORTEX...etc)
              by calling the customized HAL_SPI_MspInit() API.
     [..]
       Circular mode restriction:
      (#) The DMA circular mode cannot be used when the SPI is configured in these modes:
          (##) Master 2Lines RxOnly
          (##) Master 1Line Rx
      (#) The CRC feature is not managed when the DMA circular mode is enabled
      (#) When the SPI DMA Pause/Stop features are used, we must use the following APIs
          the HAL_SPI_DMAPause()/ HAL_SPI_DMAStop() only under the SPI callbacks
     [..]
       Master Receive mode restriction:
      (#) In Master unidirectional receive-only mode (MSTR =1, BIDIMODE=0, RXONLY=1) or
          bidirectional receive mode (MSTR=1, BIDIMODE=1, BIDIOE=0), to ensure that the SPI
          does not initiate a new transfer the following procedure has to be respected:
          (##) HAL_SPI_DeInit()
          (##) HAL_SPI_Init()
     [..]
       Callback registration:

      (#) The compilation flag USE_HAL_SPI_REGISTER_CALLBACKS when set to 1U
          allows the user to configure dynamically the driver callbacks.
          Use Functions HAL_SPI_RegisterCallback() to register an interrupt callback.

          Function HAL_SPI_RegisterCallback() allows to register following callbacks:
            (++) TxCpltCallback        : SPI Tx Completed callback
            (++) RxCpltCallback        : SPI Rx Completed callback
            (++) TxRxCpltCallback      : SPI TxRx Completed callback
            (++) TxHalfCpltCallback    : SPI Tx Half Completed callback
            (++) RxHalfCpltCallback    : SPI Rx Half Completed callback
            (++) TxRxHalfCpltCallback  : SPI TxRx Half Completed callback
            (++) ErrorCallback         : SPI Error callback
            (++) AbortCpltCallback     : SPI Abort callback
            (++) MspInitCallback       : SPI Msp Init callback
            (++) MspDeInitCallback     : SPI Msp DeInit callback
          This function takes as parameters the HAL peripheral handle, the Callback ID
          and a pointer to the user callback function.


      (#) Use function HAL_SPI_UnRegisterCallback to reset a callback to the default
          weak function.
          HAL_SPI_UnRegisterCallback takes as parameters the HAL peripheral handle,
          and the Callback ID.
          This function allows to reset following callbacks:
            (++) TxCpltCallback        : SPI Tx Completed callback
            (++) RxCpltCallback        : SPI Rx Completed callback
            (++) TxRxCpltCallback      : SPI TxRx Completed callback
            (++) TxHalfCpltCallback    : SPI Tx Half Completed callback
            (++) RxHalfCpltCallback    : SPI Rx Half Completed callback
            (++) TxRxHalfCpltCallback  : SPI TxRx Half Completed callback
            (++) ErrorCallback         : SPI Error callback
            (++) AbortCpltCallback     : SPI Abort callback
            (++) MspInitCallback       : SPI Msp Init callback
            (++) MspDeInitCallback     : SPI Msp DeInit callback

       [..]
       By default, after the HAL_SPI_Init() and when the state is HAL_SPI_STATE_RESET
       all callbacks are set to the corresponding weak functions:
       examples HAL_SPI_MasterTxCpltCallback(), HAL_SPI_MasterRxCpltCallback().
       Exception done for MspInit and MspDeInit functions that are
       reset to the legacy weak functions in the HAL_SPI_Init()/ HAL_SPI_DeInit() only when
       these callbacks are null (not registered beforehand).
       If MspInit or MspDeInit are not null, the HAL_SPI_Init()/ HAL_SPI_DeInit()
       keep and use the user MspInit/MspDeInit callbacks (registered beforehand) whatever the state.

       [..]
       Callbacks can be registered/unregistered in HAL_SPI_STATE_READY state only.
       Exception done MspInit/MspDeInit functions that can be registered/unregistered
       in HAL_SPI_STATE_READY or HAL_SPI_STATE_RESET state,
       thus registered (user) MspInit/DeInit callbacks can be used during the Init/DeInit.
       Then, the user first registers the MspInit/MspDeInit user callbacks
       using HAL_SPI_RegisterCallback() before calling HAL_SPI_DeInit()
       or HAL_SPI_Init() function.

       [..]
       When the compilation define USE_HAL_PPP_REGISTER_CALLBACKS is set to 0 or
       not defined, the callback registering feature is not available
       and weak (surcharged) callbacks are used.

     [..]
       Using the HAL it is not possible to reach all supported SPI frequency with the different SPI Modes,
       the following table resume the max SPI frequency reached with data size 8bits/16bits,
         according to frequency of the APBx Peripheral Clock (fPCLK) used by the SPI instance.

  @endverbatim

  Additional table :

       DataSize = SPI_DATASIZE_8BIT:
       +----------------------------------------------------------------------------------------------+
       |         |                | 2Lines Fullduplex   |     2Lines RxOnly    |         1Line        |
       | Process | Transfer mode  |---------------------|----------------------|----------------------|
       |         |                |  Master  |  Slave   |  Master   |  Slave   |  Master   |  Slave   |
       |==============================================================================================|
       |    T    |     Polling    | Fpclk/2  | Fpclk/2  |    NA     |    NA    |    NA     |   NA     |
       |    X    |----------------|----------|----------|-----------|----------|-----------|----------|
       |    /    |     Interrupt  | Fpclk/4  | Fpclk/8  |    NA     |    NA    |    NA     |   NA     |
       |    R    |----------------|----------|----------|-----------|----------|-----------|----------|
       |    X    |       DMA      | Fpclk/2  | Fpclk/2  |    NA     |    NA    |    NA     |   NA     |
       |=========|================|==========|==========|===========|==========|===========|==========|
       |         |     Polling    | Fpclk/2  | Fpclk/2  | Fpclk/64  | Fpclk/2  | Fpclk/64  | Fpclk/2  |
       |         |----------------|----------|----------|-----------|----------|-----------|----------|
       |    R    |     Interrupt  | Fpclk/8  | Fpclk/8  | Fpclk/64  | Fpclk/2  | Fpclk/64  | Fpclk/2  |
       |    X    |----------------|----------|----------|-----------|----------|-----------|----------|
       |         |       DMA      | Fpclk/2  | Fpclk/2  | Fpclk/64  | Fpclk/2  | Fpclk/128 | Fpclk/2  |
       |=========|================|==========|==========|===========|==========|===========|==========|
       |         |     Polling    | Fpclk/2  | Fpclk/4  |     NA    |    NA    | Fpclk/2   | Fpclk/64 |
       |         |----------------|----------|----------|-----------|----------|-----------|----------|
       |    T    |     Interrupt  | Fpclk/2  | Fpclk/4  |     NA    |    NA    | Fpclk/2   | Fpclk/64 |
       |    X    |----------------|----------|----------|-----------|----------|-----------|----------|
       |         |       DMA      | Fpclk/2  | Fpclk/2  |     NA    |    NA    | Fpclk/2   | Fpclk/128|
       +----------------------------------------------------------------------------------------------+

       DataSize = SPI_DATASIZE_16BIT:
       +----------------------------------------------------------------------------------------------+
       |         |                | 2Lines Fullduplex   |     2Lines RxOnly    |         1Line        |
       | Process | Transfer mode  |---------------------|----------------------|----------------------|
       |         |                |  Master  |  Slave   |  Master   |  Slave   |  Master   |  Slave   |
       |==============================================================================================|
       |    T    |     Polling    | Fpclk/2  | Fpclk/2  |    NA     |    NA    |    NA     |   NA     |
       |    X    |----------------|----------|----------|-----------|----------|-----------|----------|
       |    /    |     Interrupt  | Fpclk/4  | Fpclk/4  |    NA     |    NA    |    NA     |   NA     |
       |    R    |----------------|----------|----------|-----------|----------|-----------|----------|
       |    X    |       DMA      | Fpclk/2  | Fpclk/2  |    NA     |    NA    |    NA     |   NA     |
       |=========|================|==========|==========|===========|==========|===========|==========|
       |         |     Polling    | Fpclk/2  | Fpclk/2  | Fpclk/64  | Fpclk/2  | Fpclk/32  | Fpclk/2  |
       |         |----------------|----------|----------|-----------|----------|-----------|----------|
       |    R    |     Interrupt  | Fpclk/4  | Fpclk/4  | Fpclk/64  | Fpclk/2  | Fpclk/64  | Fpclk/2  |
       |    X    |----------------|----------|----------|-----------|----------|-----------|----------|
       |         |       DMA      | Fpclk/2  | Fpclk/2  | Fpclk/64  | Fpclk/2  | Fpclk/128 | Fpclk/2  |
       |=========|================|==========|==========|===========|==========|===========|==========|
       |         |     Polling    | Fpclk/2  | Fpclk/2  |     NA    |    NA    | Fpclk/2   | Fpclk/32 |
       |         |----------------|----------|----------|-----------|----------|-----------|----------|
       |    T    |     Interrupt  | Fpclk/2  | Fpclk/2  |     NA    |    NA    | Fpclk/2   | Fpclk/64 |
       |    X    |----------------|----------|----------|-----------|----------|-----------|----------|
       |         |       DMA      | Fpclk/2  | Fpclk/2  |     NA    |    NA    | Fpclk/2   | Fpclk/128|
       +----------------------------------------------------------------------------------------------+
       @note The max SPI frequency depend on SPI data size (8bits, 16bits),
             SPI mode(2 Lines fullduplex, 2 lines RxOnly, 1 line TX/RX) and Process mode (Polling, IT, DMA).
       @note
            (#) TX/RX processes are HAL_SPI_TransmitReceive(), HAL_SPI_TransmitReceive_IT() and HAL_SPI_TransmitReceive_DMA()
            (#) RX processes are HAL_SPI_Receive(), HAL_SPI_Receive_IT() and HAL_SPI_Receive_DMA()
            (#) TX processes are HAL_SPI_Transmit(), HAL_SPI_Transmit_IT() and HAL_SPI_Transmit_DMA()

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

/** @defgroup SPI SPI
  * @brief SPI HAL module driver
  * @{
  */
#ifdef HAL_SPI_MODULE_ENABLED                           /* 条件编译 */

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/** @defgroup SPI_Private_Constants SPI Private Constants
  * @{
  */
#define SPI_DEFAULT_TIMEOUT 100U                        /* 定义宏 */
#define SPI_BSY_FLAG_WORKAROUND_TIMEOUT 1000U /*!< Timeout 1000 µs             */
/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/** @defgroup SPI_Private_Functions SPI Private Functions
  * @{
  */
static void SPI_DMATransmitCplt(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void SPI_DMAReceiveCplt(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void SPI_DMATransmitReceiveCplt(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void SPI_DMAHalfTransmitCplt(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void SPI_DMAHalfReceiveCplt(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void SPI_DMAHalfTransmitReceiveCplt(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void SPI_DMAError(DMA_HandleTypeDef *hdma);      /* 静态函数 */
static void SPI_DMAAbortOnError(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void SPI_DMATxAbortCallback(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void SPI_DMARxAbortCallback(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static HAL_StatusTypeDef SPI_WaitFlagStateUntilTimeout(SPI_HandleTypeDef *hspi, uint32_t Flag, FlagStatus State,  /* 静态变量/函数 */
                                                       uint32_t Timeout, uint32_t Tickstart);  /* 32位无符号变量/函数 */
static void SPI_TxISR_8BIT(struct __SPI_HandleTypeDef *hspi);  /* 静态函数 */
static void SPI_TxISR_16BIT(struct __SPI_HandleTypeDef *hspi);  /* 静态函数 */
static void SPI_RxISR_8BIT(struct __SPI_HandleTypeDef *hspi);  /* 静态函数 */
static void SPI_RxISR_16BIT(struct __SPI_HandleTypeDef *hspi);  /* 静态函数 */
static void SPI_2linesRxISR_8BIT(struct __SPI_HandleTypeDef *hspi);  /* 静态函数 */
static void SPI_2linesTxISR_8BIT(struct __SPI_HandleTypeDef *hspi);  /* 静态函数 */
static void SPI_2linesTxISR_16BIT(struct __SPI_HandleTypeDef *hspi);  /* 静态函数 */
static void SPI_2linesRxISR_16BIT(struct __SPI_HandleTypeDef *hspi);  /* 静态函数 */
#if (USE_SPI_CRC != 0U)                                 /* 条件编译 */
static void SPI_RxISR_8BITCRC(struct __SPI_HandleTypeDef *hspi);  /* 静态函数 */
static void SPI_RxISR_16BITCRC(struct __SPI_HandleTypeDef *hspi);  /* 静态函数 */
static void SPI_2linesRxISR_8BITCRC(struct __SPI_HandleTypeDef *hspi);  /* 静态函数 */
static void SPI_2linesRxISR_16BITCRC(struct __SPI_HandleTypeDef *hspi);  /* 静态函数 */
#endif /* USE_SPI_CRC */
static void SPI_AbortRx_ISR(SPI_HandleTypeDef *hspi);   /* 静态函数 */
static void SPI_AbortTx_ISR(SPI_HandleTypeDef *hspi);   /* 静态函数 */
static void SPI_CloseRxTx_ISR(SPI_HandleTypeDef *hspi);  /* 静态函数 */
static void SPI_CloseRx_ISR(SPI_HandleTypeDef *hspi);   /* 静态函数 */
static void SPI_CloseTx_ISR(SPI_HandleTypeDef *hspi);   /* 静态函数 */
static HAL_StatusTypeDef SPI_EndRxTransaction(SPI_HandleTypeDef *hspi, uint32_t Timeout, uint32_t Tickstart);  /* 静态变量/函数 */
static HAL_StatusTypeDef SPI_EndRxTxTransaction(SPI_HandleTypeDef *hspi, uint32_t Timeout, uint32_t Tickstart);  /* 静态变量/函数 */
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup SPI_Exported_Functions SPI Exported Functions
  * @{
  */

/** @defgroup SPI_Exported_Functions_Group1 Initialization and de-initialization functions
  *  @brief    Initialization and Configuration functions
  *
@verbatim
 ===============================================================================
              ##### Initialization and de-initialization functions #####
 ===============================================================================
    [..]  This subsection provides a set of functions allowing to initialize and
          de-initialize the SPIx peripheral:

      (+) User must implement HAL_SPI_MspInit() function in which he configures
          all related peripherals resources (CLOCK, GPIO, DMA, IT and NVIC ).

      (+) Call the function HAL_SPI_Init() to configure the selected device with
          the selected configuration:
        (++) Mode
        (++) Direction
        (++) Data Size
        (++) Clock Polarity and Phase
        (++) NSS Management
        (++) BaudRate Prescaler
        (++) FirstBit
        (++) TIMode
        (++) CRC Calculation
        (++) CRC Polynomial if CRC enabled

      (+) Call the function HAL_SPI_DeInit() to restore the default configuration
          of the selected SPIx peripheral.

@endverbatim
  * @{
  */

/**
  * @brief  Initialize the SPI according to the specified parameters
  *         in the SPI_InitTypeDef and initialize the associated handle.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPI_Init(SPI_HandleTypeDef *hspi)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the SPI handle allocation */
  if (hspi == NULL)                                     /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the parameters */
  assert_param(IS_SPI_ALL_INSTANCE(hspi->Instance));    /* 执行语句 */
  assert_param(IS_SPI_MODE(hspi->Init.Mode));           /* 执行语句 */
  assert_param(IS_SPI_DIRECTION(hspi->Init.Direction));  /* 执行语句 */
  assert_param(IS_SPI_DATASIZE(hspi->Init.DataSize));   /* 执行语句 */
  assert_param(IS_SPI_NSS(hspi->Init.NSS));             /* 执行语句 */
  assert_param(IS_SPI_BAUDRATE_PRESCALER(hspi->Init.BaudRatePrescaler));  /* 执行语句 */
  assert_param(IS_SPI_FIRST_BIT(hspi->Init.FirstBit));  /* 执行语句 */
  assert_param(IS_SPI_TIMODE(hspi->Init.TIMode));       /* 执行语句 */
  if (hspi->Init.TIMode == SPI_TIMODE_DISABLE)          /* 条件判断 */
  {                                                     /* 代码块开始 */
    assert_param(IS_SPI_CPOL(hspi->Init.CLKPolarity));  /* 执行语句 */
    assert_param(IS_SPI_CPHA(hspi->Init.CLKPhase));     /* 执行语句 */

    if (hspi->Init.Mode == SPI_MODE_MASTER)             /* 条件判断 */
    {                                                   /* 代码块开始 */
      assert_param(IS_SPI_BAUDRATE_PRESCALER(hspi->Init.BaudRatePrescaler));  /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Baudrate prescaler not use in Motoraola Slave mode. force to default value */
      hspi->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    assert_param(IS_SPI_BAUDRATE_PRESCALER(hspi->Init.BaudRatePrescaler));  /* 执行语句 */

    /* Force polarity and phase to TI protocaol requirements */
    hspi->Init.CLKPolarity = SPI_POLARITY_LOW;          /* 赋值或初始化 */
    hspi->Init.CLKPhase    = SPI_PHASE_1EDGE;           /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
#if (USE_SPI_CRC != 0U)                                 /* 条件编译 */
  assert_param(IS_SPI_CRC_CALCULATION(hspi->Init.CRCCalculation));  /* 执行语句 */
  if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    assert_param(IS_SPI_CRC_POLYNOMIAL(hspi->Init.CRCPolynomial));  /* 执行语句 */
  }                                                     /* 代码块结束 */
#else                                                   /* 条件编译否则 */
  hspi->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;  /* 赋值或初始化 */
#endif /* USE_SPI_CRC */

  if (hspi->State == HAL_SPI_STATE_RESET)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Allocate lock resource and initialize it */
    hspi->Lock = HAL_UNLOCKED;                          /* 赋值或初始化 */

#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
    /* Init the SPI Callback settings */
    hspi->TxCpltCallback       = HAL_SPI_TxCpltCallback;       /* Legacy weak TxCpltCallback       */
    hspi->RxCpltCallback       = HAL_SPI_RxCpltCallback;       /* Legacy weak RxCpltCallback       */
    hspi->TxRxCpltCallback     = HAL_SPI_TxRxCpltCallback;     /* Legacy weak TxRxCpltCallback     */
    hspi->TxHalfCpltCallback   = HAL_SPI_TxHalfCpltCallback;   /* Legacy weak TxHalfCpltCallback   */
    hspi->RxHalfCpltCallback   = HAL_SPI_RxHalfCpltCallback;   /* Legacy weak RxHalfCpltCallback   */
    hspi->TxRxHalfCpltCallback = HAL_SPI_TxRxHalfCpltCallback; /* Legacy weak TxRxHalfCpltCallback */
    hspi->ErrorCallback        = HAL_SPI_ErrorCallback;        /* Legacy weak ErrorCallback        */
    hspi->AbortCpltCallback    = HAL_SPI_AbortCpltCallback;    /* Legacy weak AbortCpltCallback    */

    if (hspi->MspInitCallback == NULL)                  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hspi->MspInitCallback = HAL_SPI_MspInit; /* Legacy weak MspInit  */
    }                                                   /* 代码块结束 */

    /* Init the low level hardware : GPIO, CLOCK, NVIC... */
    hspi->MspInitCallback(hspi);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /* Init the low level hardware : GPIO, CLOCK, NVIC... */
    HAL_SPI_MspInit(hspi);                              /* HAL SPI操作 */
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */

  hspi->State = HAL_SPI_STATE_BUSY;                     /* 赋值或初始化 */

  /* Disable the selected SPI peripheral */
  __HAL_SPI_DISABLE(hspi);                              /* HAL辅助宏/函数 */

  /*----------------------- SPIx CR1 & CR2 Configuration ---------------------*/
  /* Configure : SPI Mode, Communication Mode, Data size, Clock polarity and phase, NSS management,
  Communication speed, First bit and CRC calculation state */
  WRITE_REG(hspi->Instance->CR1, ((hspi->Init.Mode & (SPI_CR1_MSTR | SPI_CR1_SSI)) |  /* 源代码 */
                                  (hspi->Init.Direction & (SPI_CR1_RXONLY | SPI_CR1_BIDIMODE)) |  /* 源代码 */
                                  (hspi->Init.DataSize & SPI_CR1_DFF) |  /* 源代码 */
                                  (hspi->Init.CLKPolarity & SPI_CR1_CPOL) |  /* 源代码 */
                                  (hspi->Init.CLKPhase & SPI_CR1_CPHA) |  /* 源代码 */
                                  (hspi->Init.NSS & SPI_CR1_SSM) |  /* 源代码 */
                                  (hspi->Init.BaudRatePrescaler & SPI_CR1_BR_Msk) |  /* 源代码 */
                                  (hspi->Init.FirstBit  & SPI_CR1_LSBFIRST) |  /* 源代码 */
                                  (hspi->Init.CRCCalculation & SPI_CR1_CRCEN)));  /* 执行语句 */

  /* Configure : NSS management, TI Mode */
  WRITE_REG(hspi->Instance->CR2, (((hspi->Init.NSS >> 16U) & SPI_CR2_SSOE) | (hspi->Init.TIMode & SPI_CR2_FRF)));  /* 执行语句 */

#if (USE_SPI_CRC != 0U)                                 /* 条件编译 */
  /*---------------------------- SPIx CRCPOLY Configuration ------------------*/
  /* Configure : CRC Polynomial */
  if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    WRITE_REG(hspi->Instance->CRCPR, (hspi->Init.CRCPolynomial & SPI_CRCPR_CRCPOLY_Msk));  /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif /* USE_SPI_CRC */

#if defined(SPI_I2SCFGR_I2SMOD)                         /* 条件编译 */
  /* Activate the SPI mode (Make sure that I2SMOD bit in I2SCFGR register is reset) */
  CLEAR_BIT(hspi->Instance->I2SCFGR, SPI_I2SCFGR_I2SMOD);  /* 执行语句 */
#endif /* SPI_I2SCFGR_I2SMOD */

  hspi->ErrorCode = HAL_SPI_ERROR_NONE;                 /* 赋值或初始化 */
  hspi->State     = HAL_SPI_STATE_READY;                /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  De-Initialize the SPI peripheral.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPI_DeInit(SPI_HandleTypeDef *hspi)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the SPI handle allocation */
  if (hspi == NULL)                                     /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check SPI Instance parameter */
  assert_param(IS_SPI_ALL_INSTANCE(hspi->Instance));    /* 执行语句 */

  hspi->State = HAL_SPI_STATE_BUSY;                     /* 赋值或初始化 */

  /* Disable the SPI Peripheral Clock */
  __HAL_SPI_DISABLE(hspi);                              /* HAL辅助宏/函数 */

#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
  if (hspi->MspDeInitCallback == NULL)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hspi->MspDeInitCallback = HAL_SPI_MspDeInit; /* Legacy weak MspDeInit  */
  }                                                     /* 代码块结束 */

  /* DeInit the low level hardware: GPIO, CLOCK, NVIC... */
  hspi->MspDeInitCallback(hspi);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* DeInit the low level hardware: GPIO, CLOCK, NVIC... */
  HAL_SPI_MspDeInit(hspi);                              /* HAL SPI操作 */
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */

  hspi->ErrorCode = HAL_SPI_ERROR_NONE;                 /* 赋值或初始化 */
  hspi->State = HAL_SPI_STATE_RESET;                    /* 赋值或初始化 */

  /* Release Lock */
  __HAL_UNLOCK(hspi);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initialize the SPI MSP.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
__weak void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)    /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hspi);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SPI_MspInit should be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  De-Initialize the SPI MSP.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
__weak void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hspi);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SPI_MspDeInit should be implemented in the user file
   */
}                                                       /* 代码块结束 */

#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
/**
  * @brief  Register a User SPI Callback
  *         To be used instead of the weak predefined callback
  * @param  hspi Pointer to a SPI_HandleTypeDef structure that contains
  *                the configuration information for the specified SPI.
  * @param  CallbackID ID of the callback to be registered
  * @param  pCallback pointer to the Callback function
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPI_RegisterCallback(SPI_HandleTypeDef *hspi, HAL_SPI_CallbackIDTypeDef CallbackID,  /* HAL状态类型函数 */
                                           pSPI_CallbackTypeDef pCallback)  /* 源代码 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  if (pCallback == NULL)                                /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hspi->ErrorCode |= HAL_SPI_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  /* Process locked */
  __HAL_LOCK(hspi);                                     /* HAL辅助宏/函数 */

  if (HAL_SPI_STATE_READY == hspi->State)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_SPI_TX_COMPLETE_CB_ID :                  /* case标签 */
        hspi->TxCpltCallback = pCallback;               /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_SPI_RX_COMPLETE_CB_ID :                  /* case标签 */
        hspi->RxCpltCallback = pCallback;               /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_SPI_TX_RX_COMPLETE_CB_ID :               /* case标签 */
        hspi->TxRxCpltCallback = pCallback;             /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_SPI_TX_HALF_COMPLETE_CB_ID :             /* case标签 */
        hspi->TxHalfCpltCallback = pCallback;           /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_SPI_RX_HALF_COMPLETE_CB_ID :             /* case标签 */
        hspi->RxHalfCpltCallback = pCallback;           /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_SPI_TX_RX_HALF_COMPLETE_CB_ID :          /* case标签 */
        hspi->TxRxHalfCpltCallback = pCallback;         /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_SPI_ERROR_CB_ID :                        /* case标签 */
        hspi->ErrorCallback = pCallback;                /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_SPI_ABORT_CB_ID :                        /* case标签 */
        hspi->AbortCpltCallback = pCallback;            /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_SPI_MSPINIT_CB_ID :                      /* case标签 */
        hspi->MspInitCallback = pCallback;              /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_SPI_MSPDEINIT_CB_ID :                    /* case标签 */
        hspi->MspDeInitCallback = pCallback;            /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_INVALID_CALLBACK);  /* 执行语句 */

        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (HAL_SPI_STATE_RESET == hspi->State)          /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_SPI_MSPINIT_CB_ID :                      /* case标签 */
        hspi->MspInitCallback = pCallback;              /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_SPI_MSPDEINIT_CB_ID :                    /* case标签 */
        hspi->MspDeInitCallback = pCallback;            /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_INVALID_CALLBACK);  /* 执行语句 */

        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_INVALID_CALLBACK);  /* 执行语句 */

    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hspi);                                   /* HAL辅助宏/函数 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Unregister an SPI Callback
  *         SPI callback is redirected to the weak predefined callback
  * @param  hspi Pointer to a SPI_HandleTypeDef structure that contains
  *                the configuration information for the specified SPI.
  * @param  CallbackID ID of the callback to be unregistered
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPI_UnRegisterCallback(SPI_HandleTypeDef *hspi, HAL_SPI_CallbackIDTypeDef CallbackID)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Process locked */
  __HAL_LOCK(hspi);                                     /* HAL辅助宏/函数 */

  if (HAL_SPI_STATE_READY == hspi->State)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_SPI_TX_COMPLETE_CB_ID :                  /* case标签 */
        hspi->TxCpltCallback = HAL_SPI_TxCpltCallback;             /* Legacy weak TxCpltCallback       */
        break;                                          /* 跳出 */

      case HAL_SPI_RX_COMPLETE_CB_ID :                  /* case标签 */
        hspi->RxCpltCallback = HAL_SPI_RxCpltCallback;             /* Legacy weak RxCpltCallback       */
        break;                                          /* 跳出 */

      case HAL_SPI_TX_RX_COMPLETE_CB_ID :               /* case标签 */
        hspi->TxRxCpltCallback = HAL_SPI_TxRxCpltCallback;         /* Legacy weak TxRxCpltCallback     */
        break;                                          /* 跳出 */

      case HAL_SPI_TX_HALF_COMPLETE_CB_ID :             /* case标签 */
        hspi->TxHalfCpltCallback = HAL_SPI_TxHalfCpltCallback;     /* Legacy weak TxHalfCpltCallback   */
        break;                                          /* 跳出 */

      case HAL_SPI_RX_HALF_COMPLETE_CB_ID :             /* case标签 */
        hspi->RxHalfCpltCallback = HAL_SPI_RxHalfCpltCallback;     /* Legacy weak RxHalfCpltCallback   */
        break;                                          /* 跳出 */

      case HAL_SPI_TX_RX_HALF_COMPLETE_CB_ID :          /* case标签 */
        hspi->TxRxHalfCpltCallback = HAL_SPI_TxRxHalfCpltCallback; /* Legacy weak TxRxHalfCpltCallback */
        break;                                          /* 跳出 */

      case HAL_SPI_ERROR_CB_ID :                        /* case标签 */
        hspi->ErrorCallback = HAL_SPI_ErrorCallback;               /* Legacy weak ErrorCallback        */
        break;                                          /* 跳出 */

      case HAL_SPI_ABORT_CB_ID :                        /* case标签 */
        hspi->AbortCpltCallback = HAL_SPI_AbortCpltCallback;       /* Legacy weak AbortCpltCallback    */
        break;                                          /* 跳出 */

      case HAL_SPI_MSPINIT_CB_ID :                      /* case标签 */
        hspi->MspInitCallback = HAL_SPI_MspInit;                   /* Legacy weak MspInit              */
        break;                                          /* 跳出 */

      case HAL_SPI_MSPDEINIT_CB_ID :                    /* case标签 */
        hspi->MspDeInitCallback = HAL_SPI_MspDeInit;               /* Legacy weak MspDeInit            */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_INVALID_CALLBACK);  /* 执行语句 */

        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (HAL_SPI_STATE_RESET == hspi->State)          /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_SPI_MSPINIT_CB_ID :                      /* case标签 */
        hspi->MspInitCallback = HAL_SPI_MspInit;                   /* Legacy weak MspInit              */
        break;                                          /* 跳出 */

      case HAL_SPI_MSPDEINIT_CB_ID :                    /* case标签 */
        hspi->MspDeInitCallback = HAL_SPI_MspDeInit;               /* Legacy weak MspDeInit            */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_INVALID_CALLBACK);  /* 执行语句 */

        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_INVALID_CALLBACK);  /* 执行语句 */

    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hspi);                                   /* HAL辅助宏/函数 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
/**
  * @}
  */

/** @defgroup SPI_Exported_Functions_Group2 IO operation functions
  *  @brief   Data transfers functions
  *
@verbatim
  ==============================================================================
                      ##### IO operation functions #####
 ===============================================================================
 [..]
    This subsection provides a set of functions allowing to manage the SPI
    data transfers.

    [..] The SPI supports master and slave mode :

    (#) There are two modes of transfer:
       (++) Blocking mode: The communication is performed in polling mode.
            The HAL status of all data processing is returned by the same function
            after finishing transfer.
       (++) No-Blocking mode: The communication is performed using Interrupts
            or DMA, These APIs return the HAL status.
            The end of the data processing will be indicated through the
            dedicated SPI IRQ when using Interrupt mode or the DMA IRQ when
            using DMA mode.
            The HAL_SPI_TxCpltCallback(), HAL_SPI_RxCpltCallback() and HAL_SPI_TxRxCpltCallback() user callbacks
            will be executed respectively at the end of the transmit or Receive process
            The HAL_SPI_ErrorCallback()user callback will be executed when a communication error is detected

    (#) APIs provided for these 2 transfer modes (Blocking mode or Non blocking mode using either Interrupt or DMA)
        exist for 1Line (simplex) and 2Lines (full duplex) modes.

@endverbatim
  * @{
  */

/**
  * @brief  Transmit an amount of data in blocking mode.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @param  pData pointer to data buffer
  * @param  Size amount of data to be sent
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPI_Transmit(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */
  HAL_StatusTypeDef errorcode = HAL_OK;                 /* HAL状态类型函数 */
  uint16_t initial_TxXferCount;                         /* 16位无符号变量/函数 */

  /* Check Direction parameter */
  assert_param(IS_SPI_DIRECTION_2LINES_OR_1LINE(hspi->Init.Direction));  /* 执行语句 */

  /* Process Locked */
  __HAL_LOCK(hspi);                                     /* HAL辅助宏/函数 */

  /* Init tickstart for timeout management*/
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */
  initial_TxXferCount = Size;                           /* 赋值或初始化 */

  if (hspi->State != HAL_SPI_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    errorcode = HAL_BUSY;                               /* 赋值或初始化 */
    goto error;                                         /* 跳转 */
  }                                                     /* 代码块结束 */

  if ((pData == NULL) || (Size == 0U))                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    errorcode = HAL_ERROR;                              /* 赋值或初始化 */
    goto error;                                         /* 跳转 */
  }                                                     /* 代码块结束 */

  /* Set the transaction information */
  hspi->State       = HAL_SPI_STATE_BUSY_TX;            /* 赋值或初始化 */
  hspi->ErrorCode   = HAL_SPI_ERROR_NONE;               /* 赋值或初始化 */
  hspi->pTxBuffPtr  = (uint8_t *)pData;                 /* 赋值或初始化 */
  hspi->TxXferSize  = Size;                             /* 赋值或初始化 */
  hspi->TxXferCount = Size;                             /* 赋值或初始化 */

  /*Init field not used in handle to zero */
  hspi->pRxBuffPtr  = (uint8_t *)NULL;                  /* 赋值或初始化 */
  hspi->RxXferSize  = 0U;                               /* 赋值或初始化 */
  hspi->RxXferCount = 0U;                               /* 赋值或初始化 */
  hspi->TxISR       = NULL;                             /* 赋值或初始化 */
  hspi->RxISR       = NULL;                             /* 赋值或初始化 */

  /* Configure communication direction : 1Line */
  if (hspi->Init.Direction == SPI_DIRECTION_1LINE)      /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable SPI Peripheral before set 1Line direction (BIDIOE bit) */
    __HAL_SPI_DISABLE(hspi);                            /* HAL辅助宏/函数 */
    SPI_1LINE_TX(hspi);                                 /* 执行语句 */
  }                                                     /* 代码块结束 */

#if (USE_SPI_CRC != 0U)                                 /* 条件编译 */
  /* Reset CRC Calculation */
  if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    SPI_RESET_CRC(hspi);                                /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif /* USE_SPI_CRC */

  /* Check if the SPI is already enabled */
  if ((hspi->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable SPI peripheral */
    __HAL_SPI_ENABLE(hspi);                             /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Transmit data in 16 Bit mode */
  if (hspi->Init.DataSize == SPI_DATASIZE_16BIT)        /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((hspi->Init.Mode == SPI_MODE_SLAVE) || (initial_TxXferCount == 0x01U))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hspi->Instance->DR = *((uint16_t *)hspi->pTxBuffPtr);  /* 赋值或初始化 */
      hspi->pTxBuffPtr += sizeof(uint16_t);             /* 赋值或初始化 */
      hspi->TxXferCount--;                              /* 执行语句 */
    }                                                   /* 代码块结束 */
    /* Transmit data in 16 Bit mode */
    while (hspi->TxXferCount > 0U)                      /* while循环 */
    {                                                   /* 代码块开始 */
      /* Wait until TXE flag is set to send data */
      if (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_TXE))       /* 条件判断 */
      {                                                 /* 代码块开始 */
        hspi->Instance->DR = *((uint16_t *)hspi->pTxBuffPtr);  /* 赋值或初始化 */
        hspi->pTxBuffPtr += sizeof(uint16_t);           /* 赋值或初始化 */
        hspi->TxXferCount--;                            /* 执行语句 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Timeout management */
        if ((((HAL_GetTick() - tickstart) >=  Timeout) && (Timeout != HAL_MAX_DELAY)) || (Timeout == 0U))  /* 条件判断 */
        {                                               /* 代码块开始 */
          errorcode = HAL_TIMEOUT;                      /* 赋值或初始化 */
          goto error;                                   /* 跳转 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /* Transmit data in 8 Bit mode */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    if ((hspi->Init.Mode == SPI_MODE_SLAVE) || (initial_TxXferCount == 0x01U))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      *((__IO uint8_t *)&hspi->Instance->DR) = (*hspi->pTxBuffPtr);  /* 赋值或初始化 */
      hspi->pTxBuffPtr += sizeof(uint8_t);              /* 赋值或初始化 */
      hspi->TxXferCount--;                              /* 执行语句 */
    }                                                   /* 代码块结束 */
    while (hspi->TxXferCount > 0U)                      /* while循环 */
    {                                                   /* 代码块开始 */
      /* Wait until TXE flag is set to send data */
      if (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_TXE))       /* 条件判断 */
      {                                                 /* 代码块开始 */
        *((__IO uint8_t *)&hspi->Instance->DR) = (*hspi->pTxBuffPtr);  /* 赋值或初始化 */
        hspi->pTxBuffPtr += sizeof(uint8_t);            /* 赋值或初始化 */
        hspi->TxXferCount--;                            /* 执行语句 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Timeout management */
        if ((((HAL_GetTick() - tickstart) >=  Timeout) && (Timeout != HAL_MAX_DELAY)) || (Timeout == 0U))  /* 条件判断 */
        {                                               /* 代码块开始 */
          errorcode = HAL_TIMEOUT;                      /* 赋值或初始化 */
          goto error;                                   /* 跳转 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
#if (USE_SPI_CRC != 0U)                                 /* 条件编译 */
  /* Enable CRC Transmission */
  if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    SET_BIT(hspi->Instance->CR1, SPI_CR1_CRCNEXT);      /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif /* USE_SPI_CRC */

  /* Check the end of the transaction */
  if (SPI_EndRxTxTransaction(hspi, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hspi->ErrorCode = HAL_SPI_ERROR_FLAG;               /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Clear overrun flag in 2 Lines communication mode because received is not read */
  if (hspi->Init.Direction == SPI_DIRECTION_2LINES)     /* 条件判断 */
  {                                                     /* 代码块开始 */
    __HAL_SPI_CLEAR_OVRFLAG(hspi);                      /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  if (hspi->ErrorCode != HAL_SPI_ERROR_NONE)            /* 条件判断 */
  {                                                     /* 代码块开始 */
    errorcode = HAL_ERROR;                              /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

error:                                                  /* 标签 */
  hspi->State = HAL_SPI_STATE_READY;                    /* 赋值或初始化 */
  /* Process Unlocked */
  __HAL_UNLOCK(hspi);                                   /* HAL辅助宏/函数 */
  return errorcode;                                     /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Receive an amount of data in blocking mode.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @param  pData pointer to data buffer
  * @param  Size amount of data to be received
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPI_Receive(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
#if (USE_SPI_CRC != 0U)                                 /* 条件编译 */
  __IO uint32_t tmpreg = 0U;                            /* 赋值或初始化 */
#endif /* USE_SPI_CRC */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */
  HAL_StatusTypeDef errorcode = HAL_OK;                 /* HAL状态类型函数 */

  if ((hspi->Init.Mode == SPI_MODE_MASTER) && (hspi->Init.Direction == SPI_DIRECTION_2LINES))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hspi->State = HAL_SPI_STATE_BUSY_RX;                /* 赋值或初始化 */
    /* Call transmit-receive function to send Dummy data on Tx line and generate clock on CLK line */
    return HAL_SPI_TransmitReceive(hspi, pData, pData, Size, Timeout);  /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Process Locked */
  __HAL_LOCK(hspi);                                     /* HAL辅助宏/函数 */

  /* Init tickstart for timeout management*/
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  if (hspi->State != HAL_SPI_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    errorcode = HAL_BUSY;                               /* 赋值或初始化 */
    goto error;                                         /* 跳转 */
  }                                                     /* 代码块结束 */

  if ((pData == NULL) || (Size == 0U))                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    errorcode = HAL_ERROR;                              /* 赋值或初始化 */
    goto error;                                         /* 跳转 */
  }                                                     /* 代码块结束 */

  /* Set the transaction information */
  hspi->State       = HAL_SPI_STATE_BUSY_RX;            /* 赋值或初始化 */
  hspi->ErrorCode   = HAL_SPI_ERROR_NONE;               /* 赋值或初始化 */
  hspi->pRxBuffPtr  = (uint8_t *)pData;                 /* 赋值或初始化 */
  hspi->RxXferSize  = Size;                             /* 赋值或初始化 */
  hspi->RxXferCount = Size;                             /* 赋值或初始化 */

  /*Init field not used in handle to zero */
  hspi->pTxBuffPtr  = (uint8_t *)NULL;                  /* 赋值或初始化 */
  hspi->TxXferSize  = 0U;                               /* 赋值或初始化 */
  hspi->TxXferCount = 0U;                               /* 赋值或初始化 */
  hspi->RxISR       = NULL;                             /* 赋值或初始化 */
  hspi->TxISR       = NULL;                             /* 赋值或初始化 */

#if (USE_SPI_CRC != 0U)                                 /* 条件编译 */
  /* Reset CRC Calculation */
  if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    SPI_RESET_CRC(hspi);                                /* 执行语句 */
    /* this is done to handle the CRCNEXT before the latest data */
    hspi->RxXferCount--;                                /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif /* USE_SPI_CRC */

  /* Configure communication direction: 1Line */
  if (hspi->Init.Direction == SPI_DIRECTION_1LINE)      /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable SPI Peripheral before set 1Line direction (BIDIOE bit) */
    __HAL_SPI_DISABLE(hspi);                            /* HAL辅助宏/函数 */
    SPI_1LINE_RX(hspi);                                 /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Check if the SPI is already enabled */
  if ((hspi->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable SPI peripheral */
    __HAL_SPI_ENABLE(hspi);                             /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Receive data in 8 Bit mode */
  if (hspi->Init.DataSize == SPI_DATASIZE_8BIT)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Transfer loop */
    while (hspi->RxXferCount > 0U)                      /* while循环 */
    {                                                   /* 代码块开始 */
      /* Check the RXNE flag */
      if (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_RXNE))      /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* read the received data */
        (* (uint8_t *)hspi->pRxBuffPtr) = *(__IO uint8_t *)&hspi->Instance->DR;  /* 赋值或初始化 */
        hspi->pRxBuffPtr += sizeof(uint8_t);            /* 赋值或初始化 */
        hspi->RxXferCount--;                            /* 执行语句 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Timeout management */
        if ((((HAL_GetTick() - tickstart) >=  Timeout) && (Timeout != HAL_MAX_DELAY)) || (Timeout == 0U))  /* 条件判断 */
        {                                               /* 代码块开始 */
          errorcode = HAL_TIMEOUT;                      /* 赋值或初始化 */
          goto error;                                   /* 跳转 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Transfer loop */
    while (hspi->RxXferCount > 0U)                      /* while循环 */
    {                                                   /* 代码块开始 */
      /* Check the RXNE flag */
      if (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_RXNE))      /* 条件判断 */
      {                                                 /* 代码块开始 */
        *((uint16_t *)hspi->pRxBuffPtr) = (uint16_t)hspi->Instance->DR;  /* 赋值或初始化 */
        hspi->pRxBuffPtr += sizeof(uint16_t);           /* 赋值或初始化 */
        hspi->RxXferCount--;                            /* 执行语句 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Timeout management */
        if ((((HAL_GetTick() - tickstart) >=  Timeout) && (Timeout != HAL_MAX_DELAY)) || (Timeout == 0U))  /* 条件判断 */
        {                                               /* 代码块开始 */
          errorcode = HAL_TIMEOUT;                      /* 赋值或初始化 */
          goto error;                                   /* 跳转 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

#if (USE_SPI_CRC != 0U)                                 /* 条件编译 */
  /* Handle the CRC Transmission */
  if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* freeze the CRC before the latest data */
    SET_BIT(hspi->Instance->CR1, SPI_CR1_CRCNEXT);      /* 执行语句 */

    /* Read the latest data */
    if (SPI_WaitFlagStateUntilTimeout(hspi, SPI_FLAG_RXNE, SET, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* the latest data has not been received */
      errorcode = HAL_TIMEOUT;                          /* 赋值或初始化 */
      goto error;                                       /* 跳转 */
    }                                                   /* 代码块结束 */

    /* Receive last data in 16 Bit mode */
    if (hspi->Init.DataSize == SPI_DATASIZE_16BIT)      /* 条件判断 */
    {                                                   /* 代码块开始 */
      *((uint16_t *)hspi->pRxBuffPtr) = (uint16_t)hspi->Instance->DR;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    /* Receive last data in 8 Bit mode */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      (*(uint8_t *)hspi->pRxBuffPtr) = *(__IO uint8_t *)&hspi->Instance->DR;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Wait the CRC data */
    if (SPI_WaitFlagStateUntilTimeout(hspi, SPI_FLAG_RXNE, SET, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_CRC);      /* 执行语句 */
      errorcode = HAL_TIMEOUT;                          /* 赋值或初始化 */
      goto error;                                       /* 跳转 */
    }                                                   /* 代码块结束 */

    /* Read CRC to Flush DR and RXNE flag */
    tmpreg = READ_REG(hspi->Instance->DR);              /* 赋值或初始化 */
    /* To avoid GCC warning */
    UNUSED(tmpreg);                                     /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif /* USE_SPI_CRC */

  /* Check the end of the transaction */
  if (SPI_EndRxTransaction(hspi, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hspi->ErrorCode = HAL_SPI_ERROR_FLAG;               /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

#if (USE_SPI_CRC != 0U)                                 /* 条件编译 */
  /* Check if CRC error occurred */
  if (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_CRCERR))        /* 条件判断 */
  {                                                     /* 代码块开始 */
    SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_CRC);        /* 执行语句 */
    __HAL_SPI_CLEAR_CRCERRFLAG(hspi);                   /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
#endif /* USE_SPI_CRC */

  if (hspi->ErrorCode != HAL_SPI_ERROR_NONE)            /* 条件判断 */
  {                                                     /* 代码块开始 */
    errorcode = HAL_ERROR;                              /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

error :                                                 /* 标签 */
  hspi->State = HAL_SPI_STATE_READY;                    /* 赋值或初始化 */
  __HAL_UNLOCK(hspi);                                   /* HAL辅助宏/函数 */
  return errorcode;                                     /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Transmit and Receive an amount of data in blocking mode.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @param  pTxData pointer to transmission data buffer
  * @param  pRxData pointer to reception data buffer
  * @param  Size amount of data to be sent and received
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPI_TransmitReceive(SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size,  /* HAL状态类型函数 */
                                          uint32_t Timeout)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
#if (USE_SPI_CRC != 0U)                                 /* 条件编译 */
  __IO uint32_t tmpreg = 0U;                            /* 赋值或初始化 */
#endif /* USE_SPI_CRC */
  uint16_t             initial_TxXferCount;             /* 16位无符号变量/函数 */
  uint32_t             tmp_mode;                        /* 32位无符号变量/函数 */
  HAL_SPI_StateTypeDef tmp_state;                       /* HAL SPI操作 */
  uint32_t             tickstart;                       /* 32位无符号变量/函数 */

  /* Variable used to alternate Rx and Tx during transfer */
  uint32_t             txallowed = 1U;                  /* 32位无符号变量/函数 */
  HAL_StatusTypeDef    errorcode = HAL_OK;              /* HAL状态类型函数 */

  /* Check Direction parameter */
  assert_param(IS_SPI_DIRECTION_2LINES(hspi->Init.Direction));  /* 执行语句 */

  /* Process Locked */
  __HAL_LOCK(hspi);                                     /* HAL辅助宏/函数 */

  /* Init tickstart for timeout management*/
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  /* Init temporary variables */
  tmp_state           = hspi->State;                    /* 赋值或初始化 */
  tmp_mode            = hspi->Init.Mode;                /* 赋值或初始化 */
  initial_TxXferCount = Size;                           /* 赋值或初始化 */

  if (!((tmp_state == HAL_SPI_STATE_READY) || \
        ((tmp_mode == SPI_MODE_MASTER) && (hspi->Init.Direction == SPI_DIRECTION_2LINES) && (tmp_state == HAL_SPI_STATE_BUSY_RX))))  /* 源代码 */
  {                                                     /* 代码块开始 */
    errorcode = HAL_BUSY;                               /* 赋值或初始化 */
    goto error;                                         /* 跳转 */
  }                                                     /* 代码块结束 */

  if ((pTxData == NULL) || (pRxData == NULL) || (Size == 0U))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    errorcode = HAL_ERROR;                              /* 赋值或初始化 */
    goto error;                                         /* 跳转 */
  }                                                     /* 代码块结束 */

  /* Don't overwrite in case of HAL_SPI_STATE_BUSY_RX */
  if (hspi->State != HAL_SPI_STATE_BUSY_RX)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    hspi->State = HAL_SPI_STATE_BUSY_TX_RX;             /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Set the transaction information */
  hspi->ErrorCode   = HAL_SPI_ERROR_NONE;               /* 赋值或初始化 */
  hspi->pRxBuffPtr  = (uint8_t *)pRxData;               /* 赋值或初始化 */
  hspi->RxXferCount = Size;                             /* 赋值或初始化 */
  hspi->RxXferSize  = Size;                             /* 赋值或初始化 */
  hspi->pTxBuffPtr  = (uint8_t *)pTxData;               /* 赋值或初始化 */
  hspi->TxXferCount = Size;                             /* 赋值或初始化 */
  hspi->TxXferSize  = Size;                             /* 赋值或初始化 */

  /*Init field not used in handle to zero */
  hspi->RxISR       = NULL;                             /* 赋值或初始化 */
  hspi->TxISR       = NULL;                             /* 赋值或初始化 */

#if (USE_SPI_CRC != 0U)                                 /* 条件编译 */
  /* Reset CRC Calculation */
  if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    SPI_RESET_CRC(hspi);                                /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif /* USE_SPI_CRC */

  /* Check if the SPI is already enabled */
  if ((hspi->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable SPI peripheral */
    __HAL_SPI_ENABLE(hspi);                             /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Transmit and Receive data in 16 Bit mode */
  if (hspi->Init.DataSize == SPI_DATASIZE_16BIT)        /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((hspi->Init.Mode == SPI_MODE_SLAVE) || (initial_TxXferCount == 0x01U))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hspi->Instance->DR = *((uint16_t *)hspi->pTxBuffPtr);  /* 赋值或初始化 */
      hspi->pTxBuffPtr += sizeof(uint16_t);             /* 赋值或初始化 */
      hspi->TxXferCount--;                              /* 执行语句 */
    }                                                   /* 代码块结束 */
    while ((hspi->TxXferCount > 0U) || (hspi->RxXferCount > 0U))  /* while循环 */
    {                                                   /* 代码块开始 */
      /* Check TXE flag */
      if ((__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_TXE)) && (hspi->TxXferCount > 0U) && (txallowed == 1U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        hspi->Instance->DR = *((uint16_t *)hspi->pTxBuffPtr);  /* 赋值或初始化 */
        hspi->pTxBuffPtr += sizeof(uint16_t);           /* 赋值或初始化 */
        hspi->TxXferCount--;                            /* 执行语句 */
        /* Next Data is a reception (Rx). Tx not allowed */
        txallowed = 0U;                                 /* 赋值或初始化 */

#if (USE_SPI_CRC != 0U)                                 /* 条件编译 */
        /* Enable CRC Transmission */
        if ((hspi->TxXferCount == 0U) && (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE))  /* 条件判断 */
        {                                               /* 代码块开始 */
          SET_BIT(hspi->Instance->CR1, SPI_CR1_CRCNEXT);  /* 执行语句 */
        }                                               /* 代码块结束 */
#endif /* USE_SPI_CRC */
      }                                                 /* 代码块结束 */

      /* Check RXNE flag */
      if ((__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_RXNE)) && (hspi->RxXferCount > 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        *((uint16_t *)hspi->pRxBuffPtr) = (uint16_t)hspi->Instance->DR;  /* 赋值或初始化 */
        hspi->pRxBuffPtr += sizeof(uint16_t);           /* 赋值或初始化 */
        hspi->RxXferCount--;                            /* 执行语句 */
        /* Next Data is a Transmission (Tx). Tx is allowed */
        txallowed = 1U;                                 /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      if (((HAL_GetTick() - tickstart) >=  Timeout) && (Timeout != HAL_MAX_DELAY))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        errorcode = HAL_TIMEOUT;                        /* 赋值或初始化 */
        goto error;                                     /* 跳转 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /* Transmit and Receive data in 8 Bit mode */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    if ((hspi->Init.Mode == SPI_MODE_SLAVE) || (initial_TxXferCount == 0x01U))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      *((__IO uint8_t *)&hspi->Instance->DR) = (*hspi->pTxBuffPtr);  /* 赋值或初始化 */
      hspi->pTxBuffPtr += sizeof(uint8_t);              /* 赋值或初始化 */
      hspi->TxXferCount--;                              /* 执行语句 */
    }                                                   /* 代码块结束 */
    while ((hspi->TxXferCount > 0U) || (hspi->RxXferCount > 0U))  /* while循环 */
    {                                                   /* 代码块开始 */
      /* Check TXE flag */
      if ((__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_TXE)) && (hspi->TxXferCount > 0U) && (txallowed == 1U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        *(__IO uint8_t *)&hspi->Instance->DR = (*hspi->pTxBuffPtr);  /* 赋值或初始化 */
        hspi->pTxBuffPtr++;                             /* 执行语句 */
        hspi->TxXferCount--;                            /* 执行语句 */
        /* Next Data is a reception (Rx). Tx not allowed */
        txallowed = 0U;                                 /* 赋值或初始化 */

#if (USE_SPI_CRC != 0U)                                 /* 条件编译 */
        /* Enable CRC Transmission */
        if ((hspi->TxXferCount == 0U) && (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE))  /* 条件判断 */
        {                                               /* 代码块开始 */
          SET_BIT(hspi->Instance->CR1, SPI_CR1_CRCNEXT);  /* 执行语句 */
        }                                               /* 代码块结束 */
#endif /* USE_SPI_CRC */
      }                                                 /* 代码块结束 */

      /* Wait until RXNE flag is reset */
      if ((__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_RXNE)) && (hspi->RxXferCount > 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        (*(uint8_t *)hspi->pRxBuffPtr) = hspi->Instance->DR;  /* 赋值或初始化 */
        hspi->pRxBuffPtr++;                             /* 执行语句 */
        hspi->RxXferCount--;                            /* 执行语句 */
        /* Next Data is a Transmission (Tx). Tx is allowed */
        txallowed = 1U;                                 /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      if ((((HAL_GetTick() - tickstart) >=  Timeout) && ((Timeout != HAL_MAX_DELAY))) || (Timeout == 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        errorcode = HAL_TIMEOUT;                        /* 赋值或初始化 */
        goto error;                                     /* 跳转 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

#if (USE_SPI_CRC != 0U)                                 /* 条件编译 */
  /* Read CRC from DR to close CRC calculation process */
  if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Wait until TXE flag */
    if (SPI_WaitFlagStateUntilTimeout(hspi, SPI_FLAG_RXNE, SET, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Error on the CRC reception */
      SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_CRC);      /* 执行语句 */
      errorcode = HAL_TIMEOUT;                          /* 赋值或初始化 */
      goto error;                                       /* 跳转 */
    }                                                   /* 代码块结束 */
    /* Read CRC */
    tmpreg = READ_REG(hspi->Instance->DR);              /* 赋值或初始化 */
    /* To avoid GCC warning */
    UNUSED(tmpreg);                                     /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Check if CRC error occurred */
  if (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_CRCERR))        /* 条件判断 */
  {                                                     /* 代码块开始 */
    SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_CRC);        /* 执行语句 */
    /* Clear CRC Flag */
    __HAL_SPI_CLEAR_CRCERRFLAG(hspi);                   /* HAL辅助宏/函数 */

    errorcode = HAL_ERROR;                              /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
#endif /* USE_SPI_CRC */

  /* Check the end of the transaction */
  if (SPI_EndRxTxTransaction(hspi, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    errorcode = HAL_ERROR;                              /* 赋值或初始化 */
    hspi->ErrorCode = HAL_SPI_ERROR_FLAG;               /* 赋值或初始化 */
    goto error;                                         /* 跳转 */
  }                                                     /* 代码块结束 */

  /* Clear overrun flag in 2 Lines communication mode because received is not read */
  if (hspi->Init.Direction == SPI_DIRECTION_2LINES)     /* 条件判断 */
  {                                                     /* 代码块开始 */
    __HAL_SPI_CLEAR_OVRFLAG(hspi);                      /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

error :                                                 /* 标签 */
  hspi->State = HAL_SPI_STATE_READY;                    /* 赋值或初始化 */
  __HAL_UNLOCK(hspi);                                   /* HAL辅助宏/函数 */
  return errorcode;                                     /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Transmit an amount of data in non-blocking mode with Interrupt.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @param  pData pointer to data buffer
  * @param  Size amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPI_Transmit_IT(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef errorcode = HAL_OK;                 /* HAL状态类型函数 */

  /* Check Direction parameter */
  assert_param(IS_SPI_DIRECTION_2LINES_OR_1LINE(hspi->Init.Direction));  /* 执行语句 */

  /* Process Locked */
  __HAL_LOCK(hspi);                                     /* HAL辅助宏/函数 */

  if ((pData == NULL) || (Size == 0U))                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    errorcode = HAL_ERROR;                              /* 赋值或初始化 */
    goto error;                                         /* 跳转 */
  }                                                     /* 代码块结束 */

  if (hspi->State != HAL_SPI_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    errorcode = HAL_BUSY;                               /* 赋值或初始化 */
    goto error;                                         /* 跳转 */
  }                                                     /* 代码块结束 */

  /* Set the transaction information */
  hspi->State       = HAL_SPI_STATE_BUSY_TX;            /* 赋值或初始化 */
  hspi->ErrorCode   = HAL_SPI_ERROR_NONE;               /* 赋值或初始化 */
  hspi->pTxBuffPtr  = (uint8_t *)pData;                 /* 赋值或初始化 */
  hspi->TxXferSize  = Size;                             /* 赋值或初始化 */
  hspi->TxXferCount = Size;                             /* 赋值或初始化 */

  /* Init field not used in handle to zero */
  hspi->pRxBuffPtr  = (uint8_t *)NULL;                  /* 赋值或初始化 */
  hspi->RxXferSize  = 0U;                               /* 赋值或初始化 */
  hspi->RxXferCount = 0U;                               /* 赋值或初始化 */
  hspi->RxISR       = NULL;                             /* 赋值或初始化 */

  /* Set the function for IT treatment */
  if (hspi->Init.DataSize > SPI_DATASIZE_8BIT)          /* 条件判断 */
  {                                                     /* 代码块开始 */
    hspi->TxISR = SPI_TxISR_16BIT;                      /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    hspi->TxISR = SPI_TxISR_8BIT;                       /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Configure communication direction : 1Line */
  if (hspi->Init.Direction == SPI_DIRECTION_1LINE)      /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable SPI Peripheral before set 1Line direction (BIDIOE bit) */
    __HAL_SPI_DISABLE(hspi);                            /* HAL辅助宏/函数 */
    SPI_1LINE_TX(hspi);                                 /* 执行语句 */
  }                                                     /* 代码块结束 */

#if (USE_SPI_CRC != 0U)                                 /* 条件编译 */
  /* Reset CRC Calculation */
  if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    SPI_RESET_CRC(hspi);                                /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif /* USE_SPI_CRC */

  /* Enable TXE and ERR interrupt */
  __HAL_SPI_ENABLE_IT(hspi, (SPI_IT_TXE | SPI_IT_ERR));  /* HAL辅助宏/函数 */


  /* Check if the SPI is already enabled */
  if ((hspi->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable SPI peripheral */
    __HAL_SPI_ENABLE(hspi);                             /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

error :                                                 /* 标签 */
  __HAL_UNLOCK(hspi);                                   /* HAL辅助宏/函数 */
  return errorcode;                                     /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Receive an amount of data in non-blocking mode with Interrupt.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @param  pData pointer to data buffer
  * @param  Size amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPI_Receive_IT(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef errorcode = HAL_OK;                 /* HAL状态类型函数 */

  if ((hspi->Init.Direction == SPI_DIRECTION_2LINES) && (hspi->Init.Mode == SPI_MODE_MASTER))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hspi->State = HAL_SPI_STATE_BUSY_RX;                /* 赋值或初始化 */
    /* Call transmit-receive function to send Dummy data on Tx line and generate clock on CLK line */
    return HAL_SPI_TransmitReceive_IT(hspi, pData, pData, Size);  /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Process Locked */
  __HAL_LOCK(hspi);                                     /* HAL辅助宏/函数 */

  if (hspi->State != HAL_SPI_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    errorcode = HAL_BUSY;                               /* 赋值或初始化 */
    goto error;                                         /* 跳转 */
  }                                                     /* 代码块结束 */

  if ((pData == NULL) || (Size == 0U))                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    errorcode = HAL_ERROR;                              /* 赋值或初始化 */
    goto error;                                         /* 跳转 */
  }                                                     /* 代码块结束 */

  /* Set the transaction information */
  hspi->State       = HAL_SPI_STATE_BUSY_RX;            /* 赋值或初始化 */
  hspi->ErrorCode   = HAL_SPI_ERROR_NONE;               /* 赋值或初始化 */
  hspi->pRxBuffPtr  = (uint8_t *)pData;                 /* 赋值或初始化 */
  hspi->RxXferSize  = Size;                             /* 赋值或初始化 */
  hspi->RxXferCount = Size;                             /* 赋值或初始化 */

  /* Init field not used in handle to zero */
  hspi->pTxBuffPtr  = (uint8_t *)NULL;                  /* 赋值或初始化 */
  hspi->TxXferSize  = 0U;                               /* 赋值或初始化 */
  hspi->TxXferCount = 0U;                               /* 赋值或初始化 */
  hspi->TxISR       = NULL;                             /* 赋值或初始化 */

  /* Set the function for IT treatment */
  if (hspi->Init.DataSize > SPI_DATASIZE_8BIT)          /* 条件判断 */
  {                                                     /* 代码块开始 */
    hspi->RxISR = SPI_RxISR_16BIT;                      /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    hspi->RxISR = SPI_RxISR_8BIT;                       /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Configure communication direction : 1Line */
  if (hspi->Init.Direction == SPI_DIRECTION_1LINE)      /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable SPI Peripheral before set 1Line direction (BIDIOE bit) */
    __HAL_SPI_DISABLE(hspi);                            /* HAL辅助宏/函数 */
    SPI_1LINE_RX(hspi);                                 /* 执行语句 */
  }                                                     /* 代码块结束 */

#if (USE_SPI_CRC != 0U)                                 /* 条件编译 */
  /* Reset CRC Calculation */
  if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    SPI_RESET_CRC(hspi);                                /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif /* USE_SPI_CRC */

  /* Enable TXE and ERR interrupt */
  __HAL_SPI_ENABLE_IT(hspi, (SPI_IT_RXNE | SPI_IT_ERR));  /* HAL辅助宏/函数 */

  /* Note : The SPI must be enabled after unlocking current process
            to avoid the risk of SPI interrupt handle execution before current
            process unlock */

  /* Check if the SPI is already enabled */
  if ((hspi->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable SPI peripheral */
    __HAL_SPI_ENABLE(hspi);                             /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

error :                                                 /* 标签 */
  /* Process Unlocked */
  __HAL_UNLOCK(hspi);                                   /* HAL辅助宏/函数 */
  return errorcode;                                     /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Transmit and Receive an amount of data in non-blocking mode with Interrupt.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @param  pTxData pointer to transmission data buffer
  * @param  pRxData pointer to reception data buffer
  * @param  Size amount of data to be sent and received
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPI_TransmitReceive_IT(SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t             tmp_mode;                        /* 32位无符号变量/函数 */
  HAL_SPI_StateTypeDef tmp_state;                       /* HAL SPI操作 */
  HAL_StatusTypeDef    errorcode = HAL_OK;              /* HAL状态类型函数 */

  /* Check Direction parameter */
  assert_param(IS_SPI_DIRECTION_2LINES(hspi->Init.Direction));  /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hspi);                                     /* HAL辅助宏/函数 */

  /* Init temporary variables */
  tmp_state           = hspi->State;                    /* 赋值或初始化 */
  tmp_mode            = hspi->Init.Mode;                /* 赋值或初始化 */

  if (!((tmp_state == HAL_SPI_STATE_READY) || \
        ((tmp_mode == SPI_MODE_MASTER) && (hspi->Init.Direction == SPI_DIRECTION_2LINES) && (tmp_state == HAL_SPI_STATE_BUSY_RX))))  /* 源代码 */
  {                                                     /* 代码块开始 */
    errorcode = HAL_BUSY;                               /* 赋值或初始化 */
    goto error;                                         /* 跳转 */
  }                                                     /* 代码块结束 */

  if ((pTxData == NULL) || (pRxData == NULL) || (Size == 0U))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    errorcode = HAL_ERROR;                              /* 赋值或初始化 */
    goto error;                                         /* 跳转 */
  }                                                     /* 代码块结束 */

  /* Don't overwrite in case of HAL_SPI_STATE_BUSY_RX */
  if (hspi->State != HAL_SPI_STATE_BUSY_RX)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    hspi->State = HAL_SPI_STATE_BUSY_TX_RX;             /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Set the transaction information */
  hspi->ErrorCode   = HAL_SPI_ERROR_NONE;               /* 赋值或初始化 */
  hspi->pTxBuffPtr  = (uint8_t *)pTxData;               /* 赋值或初始化 */
  hspi->TxXferSize  = Size;                             /* 赋值或初始化 */
  hspi->TxXferCount = Size;                             /* 赋值或初始化 */
  hspi->pRxBuffPtr  = (uint8_t *)pRxData;               /* 赋值或初始化 */
  hspi->RxXferSize  = Size;                             /* 赋值或初始化 */
  hspi->RxXferCount = Size;                             /* 赋值或初始化 */

  /* Set the function for IT treatment */
  if (hspi->Init.DataSize > SPI_DATASIZE_8BIT)          /* 条件判断 */
  {                                                     /* 代码块开始 */
    hspi->RxISR     = SPI_2linesRxISR_16BIT;            /* 赋值或初始化 */
    hspi->TxISR     = SPI_2linesTxISR_16BIT;            /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    hspi->RxISR     = SPI_2linesRxISR_8BIT;             /* 赋值或初始化 */
    hspi->TxISR     = SPI_2linesTxISR_8BIT;             /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

#if (USE_SPI_CRC != 0U)                                 /* 条件编译 */
  /* Reset CRC Calculation */
  if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    SPI_RESET_CRC(hspi);                                /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif /* USE_SPI_CRC */

  /* Enable TXE, RXNE and ERR interrupt */
  __HAL_SPI_ENABLE_IT(hspi, (SPI_IT_TXE | SPI_IT_RXNE | SPI_IT_ERR));  /* HAL辅助宏/函数 */

  /* Check if the SPI is already enabled */
  if ((hspi->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable SPI peripheral */
    __HAL_SPI_ENABLE(hspi);                             /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

error :                                                 /* 标签 */
  /* Process Unlocked */
  __HAL_UNLOCK(hspi);                                   /* HAL辅助宏/函数 */
  return errorcode;                                     /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Transmit an amount of data in non-blocking mode with DMA.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @param  pData pointer to data buffer
  * @param  Size amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPI_Transmit_DMA(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef errorcode = HAL_OK;                 /* HAL状态类型函数 */

  /* Check tx dma handle */
  assert_param(IS_SPI_DMA_HANDLE(hspi->hdmatx));        /* 执行语句 */

  /* Check Direction parameter */
  assert_param(IS_SPI_DIRECTION_2LINES_OR_1LINE(hspi->Init.Direction));  /* 执行语句 */

  /* Process Locked */
  __HAL_LOCK(hspi);                                     /* HAL辅助宏/函数 */

  if (hspi->State != HAL_SPI_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    errorcode = HAL_BUSY;                               /* 赋值或初始化 */
    goto error;                                         /* 跳转 */
  }                                                     /* 代码块结束 */

  if ((pData == NULL) || (Size == 0U))                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    errorcode = HAL_ERROR;                              /* 赋值或初始化 */
    goto error;                                         /* 跳转 */
  }                                                     /* 代码块结束 */

  /* Set the transaction information */
  hspi->State       = HAL_SPI_STATE_BUSY_TX;            /* 赋值或初始化 */
  hspi->ErrorCode   = HAL_SPI_ERROR_NONE;               /* 赋值或初始化 */
  hspi->pTxBuffPtr  = (uint8_t *)pData;                 /* 赋值或初始化 */
  hspi->TxXferSize  = Size;                             /* 赋值或初始化 */
  hspi->TxXferCount = Size;                             /* 赋值或初始化 */

  /* Init field not used in handle to zero */
  hspi->pRxBuffPtr  = (uint8_t *)NULL;                  /* 赋值或初始化 */
  hspi->TxISR       = NULL;                             /* 赋值或初始化 */
  hspi->RxISR       = NULL;                             /* 赋值或初始化 */
  hspi->RxXferSize  = 0U;                               /* 赋值或初始化 */
  hspi->RxXferCount = 0U;                               /* 赋值或初始化 */

  /* Configure communication direction : 1Line */
  if (hspi->Init.Direction == SPI_DIRECTION_1LINE)      /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable SPI Peripheral before set 1Line direction (BIDIOE bit) */
    __HAL_SPI_DISABLE(hspi);                            /* HAL辅助宏/函数 */
    SPI_1LINE_TX(hspi);                                 /* 执行语句 */
  }                                                     /* 代码块结束 */

#if (USE_SPI_CRC != 0U)                                 /* 条件编译 */
  /* Reset CRC Calculation */
  if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    SPI_RESET_CRC(hspi);                                /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif /* USE_SPI_CRC */

  /* Set the SPI TxDMA Half transfer complete callback */
  hspi->hdmatx->XferHalfCpltCallback = SPI_DMAHalfTransmitCplt;  /* 赋值或初始化 */

  /* Set the SPI TxDMA transfer complete callback */
  hspi->hdmatx->XferCpltCallback = SPI_DMATransmitCplt;  /* 赋值或初始化 */

  /* Set the DMA error callback */
  hspi->hdmatx->XferErrorCallback = SPI_DMAError;       /* 赋值或初始化 */

  /* Set the DMA AbortCpltCallback */
  hspi->hdmatx->XferAbortCallback = NULL;               /* 赋值或初始化 */

  /* Enable the Tx DMA Stream/Channel */
  if (HAL_OK != HAL_DMA_Start_IT(hspi->hdmatx, (uint32_t)hspi->pTxBuffPtr, (uint32_t)&hspi->Instance->DR,  /* 条件判断 */
                                 hspi->TxXferCount))    /* 源代码 */
  {                                                     /* 代码块开始 */
    /* Update SPI error code */
    SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_DMA);        /* 执行语句 */
    errorcode = HAL_ERROR;                              /* 赋值或初始化 */

    hspi->State = HAL_SPI_STATE_READY;                  /* 赋值或初始化 */
    goto error;                                         /* 跳转 */
  }                                                     /* 代码块结束 */

  /* Check if the SPI is already enabled */
  if ((hspi->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable SPI peripheral */
    __HAL_SPI_ENABLE(hspi);                             /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Enable the SPI Error Interrupt Bit */
  __HAL_SPI_ENABLE_IT(hspi, (SPI_IT_ERR));              /* HAL辅助宏/函数 */

  /* Enable Tx DMA Request */
  SET_BIT(hspi->Instance->CR2, SPI_CR2_TXDMAEN);        /* 执行语句 */

error :                                                 /* 标签 */
  /* Process Unlocked */
  __HAL_UNLOCK(hspi);                                   /* HAL辅助宏/函数 */
  return errorcode;                                     /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Receive an amount of data in non-blocking mode with DMA.
  * @note   In case of MASTER mode and SPI_DIRECTION_2LINES direction, hdmatx shall be defined.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @param  pData pointer to data buffer
  * @note   When the CRC feature is enabled the pData Length must be Size + 1.
  * @param  Size amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPI_Receive_DMA(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef errorcode = HAL_OK;                 /* HAL状态类型函数 */

  /* Check rx dma handle */
  assert_param(IS_SPI_DMA_HANDLE(hspi->hdmarx));        /* 执行语句 */

  if ((hspi->Init.Direction == SPI_DIRECTION_2LINES) && (hspi->Init.Mode == SPI_MODE_MASTER))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hspi->State = HAL_SPI_STATE_BUSY_RX;                /* 赋值或初始化 */

    /* Check tx dma handle */
    assert_param(IS_SPI_DMA_HANDLE(hspi->hdmatx));      /* 执行语句 */

    /* Call transmit-receive function to send Dummy data on Tx line and generate clock on CLK line */
    return HAL_SPI_TransmitReceive_DMA(hspi, pData, pData, Size);  /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Process Locked */
  __HAL_LOCK(hspi);                                     /* HAL辅助宏/函数 */

  if (hspi->State != HAL_SPI_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    errorcode = HAL_BUSY;                               /* 赋值或初始化 */
    goto error;                                         /* 跳转 */
  }                                                     /* 代码块结束 */

  if ((pData == NULL) || (Size == 0U))                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    errorcode = HAL_ERROR;                              /* 赋值或初始化 */
    goto error;                                         /* 跳转 */
  }                                                     /* 代码块结束 */

  /* Set the transaction information */
  hspi->State       = HAL_SPI_STATE_BUSY_RX;            /* 赋值或初始化 */
  hspi->ErrorCode   = HAL_SPI_ERROR_NONE;               /* 赋值或初始化 */
  hspi->pRxBuffPtr  = (uint8_t *)pData;                 /* 赋值或初始化 */
  hspi->RxXferSize  = Size;                             /* 赋值或初始化 */
  hspi->RxXferCount = Size;                             /* 赋值或初始化 */

  /*Init field not used in handle to zero */
  hspi->RxISR       = NULL;                             /* 赋值或初始化 */
  hspi->TxISR       = NULL;                             /* 赋值或初始化 */
  hspi->TxXferSize  = 0U;                               /* 赋值或初始化 */
  hspi->TxXferCount = 0U;                               /* 赋值或初始化 */

  /* Configure communication direction : 1Line */
  if (hspi->Init.Direction == SPI_DIRECTION_1LINE)      /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable SPI Peripheral before set 1Line direction (BIDIOE bit) */
    __HAL_SPI_DISABLE(hspi);                            /* HAL辅助宏/函数 */
    SPI_1LINE_RX(hspi);                                 /* 执行语句 */
  }                                                     /* 代码块结束 */

#if (USE_SPI_CRC != 0U)                                 /* 条件编译 */
  /* Reset CRC Calculation */
  if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    SPI_RESET_CRC(hspi);                                /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif /* USE_SPI_CRC */

  /* Set the SPI RxDMA Half transfer complete callback */
  hspi->hdmarx->XferHalfCpltCallback = SPI_DMAHalfReceiveCplt;  /* 赋值或初始化 */

  /* Set the SPI Rx DMA transfer complete callback */
  hspi->hdmarx->XferCpltCallback = SPI_DMAReceiveCplt;  /* 赋值或初始化 */

  /* Set the DMA error callback */
  hspi->hdmarx->XferErrorCallback = SPI_DMAError;       /* 赋值或初始化 */

  /* Set the DMA AbortCpltCallback */
  hspi->hdmarx->XferAbortCallback = NULL;               /* 赋值或初始化 */

  /* Enable the Rx DMA Stream/Channel  */
  if (HAL_OK != HAL_DMA_Start_IT(hspi->hdmarx, (uint32_t)&hspi->Instance->DR, (uint32_t)hspi->pRxBuffPtr,  /* 条件判断 */
                                 hspi->RxXferCount))    /* 源代码 */
  {                                                     /* 代码块开始 */
    /* Update SPI error code */
    SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_DMA);        /* 执行语句 */
    errorcode = HAL_ERROR;                              /* 赋值或初始化 */

    hspi->State = HAL_SPI_STATE_READY;                  /* 赋值或初始化 */
    goto error;                                         /* 跳转 */
  }                                                     /* 代码块结束 */

  /* Check if the SPI is already enabled */
  if ((hspi->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable SPI peripheral */
    __HAL_SPI_ENABLE(hspi);                             /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Enable the SPI Error Interrupt Bit */
  __HAL_SPI_ENABLE_IT(hspi, (SPI_IT_ERR));              /* HAL辅助宏/函数 */

  /* Enable Rx DMA Request */
  SET_BIT(hspi->Instance->CR2, SPI_CR2_RXDMAEN);        /* 执行语句 */

error:                                                  /* 标签 */
  /* Process Unlocked */
  __HAL_UNLOCK(hspi);                                   /* HAL辅助宏/函数 */
  return errorcode;                                     /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Transmit and Receive an amount of data in non-blocking mode with DMA.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @param  pTxData pointer to transmission data buffer
  * @param  pRxData pointer to reception data buffer
  * @note   When the CRC feature is enabled the pRxData Length must be Size + 1
  * @param  Size amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPI_TransmitReceive_DMA(SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData,  /* HAL状态类型函数 */
                                              uint16_t Size)  /* 16位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t             tmp_mode;                        /* 32位无符号变量/函数 */
  HAL_SPI_StateTypeDef tmp_state;                       /* HAL SPI操作 */
  HAL_StatusTypeDef errorcode = HAL_OK;                 /* HAL状态类型函数 */

  /* Check rx & tx dma handles */
  assert_param(IS_SPI_DMA_HANDLE(hspi->hdmarx));        /* 执行语句 */
  assert_param(IS_SPI_DMA_HANDLE(hspi->hdmatx));        /* 执行语句 */

  /* Check Direction parameter */
  assert_param(IS_SPI_DIRECTION_2LINES(hspi->Init.Direction));  /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hspi);                                     /* HAL辅助宏/函数 */

  /* Init temporary variables */
  tmp_state           = hspi->State;                    /* 赋值或初始化 */
  tmp_mode            = hspi->Init.Mode;                /* 赋值或初始化 */

  if (!((tmp_state == HAL_SPI_STATE_READY) ||           /* 条件判断 */
        ((tmp_mode == SPI_MODE_MASTER) && (hspi->Init.Direction == SPI_DIRECTION_2LINES) && (tmp_state == HAL_SPI_STATE_BUSY_RX))))  /* 源代码 */
  {                                                     /* 代码块开始 */
    errorcode = HAL_BUSY;                               /* 赋值或初始化 */
    goto error;                                         /* 跳转 */
  }                                                     /* 代码块结束 */

  if ((pTxData == NULL) || (pRxData == NULL) || (Size == 0U))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    errorcode = HAL_ERROR;                              /* 赋值或初始化 */
    goto error;                                         /* 跳转 */
  }                                                     /* 代码块结束 */

  /* Don't overwrite in case of HAL_SPI_STATE_BUSY_RX */
  if (hspi->State != HAL_SPI_STATE_BUSY_RX)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    hspi->State = HAL_SPI_STATE_BUSY_TX_RX;             /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Set the transaction information */
  hspi->ErrorCode   = HAL_SPI_ERROR_NONE;               /* 赋值或初始化 */
  hspi->pTxBuffPtr  = (uint8_t *)pTxData;               /* 赋值或初始化 */
  hspi->TxXferSize  = Size;                             /* 赋值或初始化 */
  hspi->TxXferCount = Size;                             /* 赋值或初始化 */
  hspi->pRxBuffPtr  = (uint8_t *)pRxData;               /* 赋值或初始化 */
  hspi->RxXferSize  = Size;                             /* 赋值或初始化 */
  hspi->RxXferCount = Size;                             /* 赋值或初始化 */

  /* Init field not used in handle to zero */
  hspi->RxISR       = NULL;                             /* 赋值或初始化 */
  hspi->TxISR       = NULL;                             /* 赋值或初始化 */

#if (USE_SPI_CRC != 0U)                                 /* 条件编译 */
  /* Reset CRC Calculation */
  if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    SPI_RESET_CRC(hspi);                                /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif /* USE_SPI_CRC */

  /* Check if we are in Rx only or in Rx/Tx Mode and configure the DMA transfer complete callback */
  if (hspi->State == HAL_SPI_STATE_BUSY_RX)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Set the SPI Rx DMA Half transfer complete callback */
    hspi->hdmarx->XferHalfCpltCallback = SPI_DMAHalfReceiveCplt;  /* 赋值或初始化 */
    hspi->hdmarx->XferCpltCallback     = SPI_DMAReceiveCplt;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Set the SPI Tx/Rx DMA Half transfer complete callback */
    hspi->hdmarx->XferHalfCpltCallback = SPI_DMAHalfTransmitReceiveCplt;  /* 赋值或初始化 */
    hspi->hdmarx->XferCpltCallback     = SPI_DMATransmitReceiveCplt;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Set the DMA error callback */
  hspi->hdmarx->XferErrorCallback = SPI_DMAError;       /* 赋值或初始化 */

  /* Set the DMA AbortCpltCallback */
  hspi->hdmarx->XferAbortCallback = NULL;               /* 赋值或初始化 */

  /* Enable the Rx DMA Stream/Channel  */
  if (HAL_OK != HAL_DMA_Start_IT(hspi->hdmarx, (uint32_t)&hspi->Instance->DR, (uint32_t)hspi->pRxBuffPtr,  /* 条件判断 */
                                 hspi->RxXferCount))    /* 源代码 */
  {                                                     /* 代码块开始 */
    /* Update SPI error code */
    SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_DMA);        /* 执行语句 */
    errorcode = HAL_ERROR;                              /* 赋值或初始化 */

    hspi->State = HAL_SPI_STATE_READY;                  /* 赋值或初始化 */
    goto error;                                         /* 跳转 */
  }                                                     /* 代码块结束 */

  /* Enable Rx DMA Request */
  SET_BIT(hspi->Instance->CR2, SPI_CR2_RXDMAEN);        /* 执行语句 */

  /* Set the SPI Tx DMA transfer complete callback as NULL because the communication closing
  is performed in DMA reception complete callback  */
  hspi->hdmatx->XferHalfCpltCallback = NULL;            /* 赋值或初始化 */
  hspi->hdmatx->XferCpltCallback     = NULL;            /* 赋值或初始化 */
  hspi->hdmatx->XferErrorCallback    = NULL;            /* 赋值或初始化 */
  hspi->hdmatx->XferAbortCallback    = NULL;            /* 赋值或初始化 */

  /* Enable the Tx DMA Stream/Channel  */
  if (HAL_OK != HAL_DMA_Start_IT(hspi->hdmatx, (uint32_t)hspi->pTxBuffPtr, (uint32_t)&hspi->Instance->DR,  /* 条件判断 */
                                 hspi->TxXferCount))    /* 源代码 */
  {                                                     /* 代码块开始 */
    /* Update SPI error code */
    SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_DMA);        /* 执行语句 */
    errorcode = HAL_ERROR;                              /* 赋值或初始化 */

    hspi->State = HAL_SPI_STATE_READY;                  /* 赋值或初始化 */
    goto error;                                         /* 跳转 */
  }                                                     /* 代码块结束 */

  /* Check if the SPI is already enabled */
  if ((hspi->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable SPI peripheral */
    __HAL_SPI_ENABLE(hspi);                             /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  /* Enable the SPI Error Interrupt Bit */
  __HAL_SPI_ENABLE_IT(hspi, (SPI_IT_ERR));              /* HAL辅助宏/函数 */

  /* Enable Tx DMA Request */
  SET_BIT(hspi->Instance->CR2, SPI_CR2_TXDMAEN);        /* 执行语句 */

error :                                                 /* 标签 */
  /* Process Unlocked */
  __HAL_UNLOCK(hspi);                                   /* HAL辅助宏/函数 */
  return errorcode;                                     /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Abort ongoing transfer (blocking mode).
  * @param  hspi SPI handle.
  * @note   This procedure could be used for aborting any ongoing transfer (Tx and Rx),
  *         started in Interrupt or DMA mode.
  *         This procedure performs following operations :
  *           - Disable SPI Interrupts (depending of transfer direction)
  *           - Disable the DMA transfer in the peripheral register (if enabled)
  *           - Abort DMA transfer by calling HAL_DMA_Abort (in case of transfer in DMA mode)
  *           - Set handle State to READY
  * @note   This procedure is executed in blocking mode : when exiting function, Abort is considered as completed.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPI_Abort(SPI_HandleTypeDef *hspi)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef errorcode;                          /* HAL状态类型函数 */
  __IO uint32_t count;                                  /* 执行语句 */
  __IO uint32_t resetcount;                             /* 执行语句 */

  /* Initialized local variable  */
  errorcode = HAL_OK;                                   /* 赋值或初始化 */
  resetcount = SPI_DEFAULT_TIMEOUT * (SystemCoreClock / 24U / 1000U);  /* 赋值或初始化 */
  count = resetcount;                                   /* 赋值或初始化 */

  /* Clear ERRIE interrupt to avoid error interrupts generation during Abort procedure */
  CLEAR_BIT(hspi->Instance->CR2, SPI_CR2_ERRIE);        /* 执行语句 */

  /* Disable TXEIE, RXNEIE and ERRIE(mode fault event, overrun error, TI frame error) interrupts */
  if (HAL_IS_BIT_SET(hspi->Instance->CR2, SPI_CR2_TXEIE))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hspi->TxISR = SPI_AbortTx_ISR;                      /* 赋值或初始化 */
    /* Wait HAL_SPI_STATE_ABORT state */
    do                                                  /* do-while循环 */
    {                                                   /* 代码块开始 */
      if (count == 0U)                                  /* 条件判断 */
      {                                                 /* 代码块开始 */
        SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_ABORT);  /* 执行语句 */
        break;                                          /* 跳出 */
      }                                                 /* 代码块结束 */
      count--;                                          /* 执行语句 */
    } while (hspi->State != HAL_SPI_STATE_ABORT);       /* 代码块结束 */
    /* Reset Timeout Counter */
    count = resetcount;                                 /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if (HAL_IS_BIT_SET(hspi->Instance->CR2, SPI_CR2_RXNEIE))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hspi->RxISR = SPI_AbortRx_ISR;                      /* 赋值或初始化 */
    /* Wait HAL_SPI_STATE_ABORT state */
    do                                                  /* do-while循环 */
    {                                                   /* 代码块开始 */
      if (count == 0U)                                  /* 条件判断 */
      {                                                 /* 代码块开始 */
        SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_ABORT);  /* 执行语句 */
        break;                                          /* 跳出 */
      }                                                 /* 代码块结束 */
      count--;                                          /* 执行语句 */
    } while (hspi->State != HAL_SPI_STATE_ABORT);       /* 代码块结束 */
    /* Reset Timeout Counter */
    count = resetcount;                                 /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Disable the SPI DMA Tx request if enabled */
  if (HAL_IS_BIT_SET(hspi->Instance->CR2, SPI_CR2_TXDMAEN))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Abort the SPI DMA Tx Stream/Channel : use blocking DMA Abort API (no callback) */
    if (hspi->hdmatx != NULL)                           /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the SPI DMA Abort callback :
      will lead to call HAL_SPI_AbortCpltCallback() at end of DMA abort procedure */
      hspi->hdmatx->XferAbortCallback = NULL;           /* 赋值或初始化 */

      /* Abort DMA Tx Handle linked to SPI Peripheral */
      if (HAL_DMA_Abort(hspi->hdmatx) != HAL_OK)        /* 条件判断 */
      {                                                 /* 代码块开始 */
        hspi->ErrorCode = HAL_SPI_ERROR_ABORT;          /* 赋值或初始化 */
      }                                                 /* 代码块结束 */

      /* Disable Tx DMA Request */
      CLEAR_BIT(hspi->Instance->CR2, (SPI_CR2_TXDMAEN));  /* 执行语句 */

      /* Wait until TXE flag is set */
      do                                                /* do-while循环 */
      {                                                 /* 代码块开始 */
        if (count == 0U)                                /* 条件判断 */
        {                                               /* 代码块开始 */
          SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_ABORT);  /* 执行语句 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */
        count--;                                        /* 执行语句 */
      } while ((hspi->Instance->SR & SPI_FLAG_TXE) == RESET);  /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Disable the SPI DMA Rx request if enabled */
  if (HAL_IS_BIT_SET(hspi->Instance->CR2, SPI_CR2_RXDMAEN))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Abort the SPI DMA Rx Stream/Channel : use blocking DMA Abort API (no callback) */
    if (hspi->hdmarx != NULL)                           /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the SPI DMA Abort callback :
      will lead to call HAL_SPI_AbortCpltCallback() at end of DMA abort procedure */
      hspi->hdmarx->XferAbortCallback = NULL;           /* 赋值或初始化 */

      /* Abort DMA Rx Handle linked to SPI Peripheral */
      if (HAL_DMA_Abort(hspi->hdmarx) != HAL_OK)        /* 条件判断 */
      {                                                 /* 代码块开始 */
        hspi->ErrorCode = HAL_SPI_ERROR_ABORT;          /* 赋值或初始化 */
      }                                                 /* 代码块结束 */

      /* Disable peripheral */
      __HAL_SPI_DISABLE(hspi);                          /* HAL辅助宏/函数 */

      /* Disable Rx DMA Request */
      CLEAR_BIT(hspi->Instance->CR2, (SPI_CR2_RXDMAEN));  /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /* Reset Tx and Rx transfer counters */
  hspi->RxXferCount = 0U;                               /* 赋值或初始化 */
  hspi->TxXferCount = 0U;                               /* 赋值或初始化 */

  /* Check error during Abort procedure */
  if (hspi->ErrorCode == HAL_SPI_ERROR_ABORT)           /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* return HAL_Error in case of error during Abort procedure */
    errorcode = HAL_ERROR;                              /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Reset errorCode */
    hspi->ErrorCode = HAL_SPI_ERROR_NONE;               /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Clear the Error flags in the SR register */
  __HAL_SPI_CLEAR_OVRFLAG(hspi);                        /* HAL辅助宏/函数 */
  __HAL_SPI_CLEAR_FREFLAG(hspi);                        /* HAL辅助宏/函数 */

  /* Restore hspi->state to ready */
  hspi->State = HAL_SPI_STATE_READY;                    /* 赋值或初始化 */

  return errorcode;                                     /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Abort ongoing transfer (Interrupt mode).
  * @param  hspi SPI handle.
  * @note   This procedure could be used for aborting any ongoing transfer (Tx and Rx),
  *         started in Interrupt or DMA mode.
  *         This procedure performs following operations :
  *           - Disable SPI Interrupts (depending of transfer direction)
  *           - Disable the DMA transfer in the peripheral register (if enabled)
  *           - Abort DMA transfer by calling HAL_DMA_Abort_IT (in case of transfer in DMA mode)
  *           - Set handle State to READY
  *           - At abort completion, call user abort complete callback
  * @note   This procedure is executed in Interrupt mode, meaning that abort procedure could be
  *         considered as completed only when user abort complete callback is executed (not when exiting function).
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPI_Abort_IT(SPI_HandleTypeDef *hspi)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef errorcode;                          /* HAL状态类型函数 */
  uint32_t abortcplt ;                                  /* 32位无符号变量/函数 */
  __IO uint32_t count;                                  /* 执行语句 */
  __IO uint32_t resetcount;                             /* 执行语句 */

  /* Initialized local variable  */
  errorcode = HAL_OK;                                   /* 赋值或初始化 */
  abortcplt = 1U;                                       /* 赋值或初始化 */
  resetcount = SPI_DEFAULT_TIMEOUT * (SystemCoreClock / 24U / 1000U);  /* 赋值或初始化 */
  count = resetcount;                                   /* 赋值或初始化 */

  /* Clear ERRIE interrupt to avoid error interrupts generation during Abort procedure */
  CLEAR_BIT(hspi->Instance->CR2, SPI_CR2_ERRIE);        /* 执行语句 */

  /* Change Rx and Tx Irq Handler to Disable TXEIE, RXNEIE and ERRIE interrupts */
  if (HAL_IS_BIT_SET(hspi->Instance->CR2, SPI_CR2_TXEIE))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hspi->TxISR = SPI_AbortTx_ISR;                      /* 赋值或初始化 */
    /* Wait HAL_SPI_STATE_ABORT state */
    do                                                  /* do-while循环 */
    {                                                   /* 代码块开始 */
      if (count == 0U)                                  /* 条件判断 */
      {                                                 /* 代码块开始 */
        SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_ABORT);  /* 执行语句 */
        break;                                          /* 跳出 */
      }                                                 /* 代码块结束 */
      count--;                                          /* 执行语句 */
    } while (hspi->State != HAL_SPI_STATE_ABORT);       /* 代码块结束 */
    /* Reset Timeout Counter */
    count = resetcount;                                 /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if (HAL_IS_BIT_SET(hspi->Instance->CR2, SPI_CR2_RXNEIE))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hspi->RxISR = SPI_AbortRx_ISR;                      /* 赋值或初始化 */
    /* Wait HAL_SPI_STATE_ABORT state */
    do                                                  /* do-while循环 */
    {                                                   /* 代码块开始 */
      if (count == 0U)                                  /* 条件判断 */
      {                                                 /* 代码块开始 */
        SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_ABORT);  /* 执行语句 */
        break;                                          /* 跳出 */
      }                                                 /* 代码块结束 */
      count--;                                          /* 执行语句 */
    } while (hspi->State != HAL_SPI_STATE_ABORT);       /* 代码块结束 */
    /* Reset Timeout Counter */
    count = resetcount;                                 /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* If DMA Tx and/or DMA Rx Handles are associated to SPI Handle, DMA Abort complete callbacks should be initialised
     before any call to DMA Abort functions */
  /* DMA Tx Handle is valid */
  if (hspi->hdmatx != NULL)                             /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Set DMA Abort Complete callback if UART DMA Tx request if enabled.
       Otherwise, set it to NULL */
    if (HAL_IS_BIT_SET(hspi->Instance->CR2, SPI_CR2_TXDMAEN))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hspi->hdmatx->XferAbortCallback = SPI_DMATxAbortCallback;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hspi->hdmatx->XferAbortCallback = NULL;           /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /* DMA Rx Handle is valid */
  if (hspi->hdmarx != NULL)                             /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Set DMA Abort Complete callback if UART DMA Rx request if enabled.
       Otherwise, set it to NULL */
    if (HAL_IS_BIT_SET(hspi->Instance->CR2, SPI_CR2_RXDMAEN))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hspi->hdmarx->XferAbortCallback = SPI_DMARxAbortCallback;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hspi->hdmarx->XferAbortCallback = NULL;           /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Disable the SPI DMA Tx request if enabled */
  if (HAL_IS_BIT_SET(hspi->Instance->CR2, SPI_CR2_TXDMAEN))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Abort the SPI DMA Tx Stream/Channel */
    if (hspi->hdmatx != NULL)                           /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Abort DMA Tx Handle linked to SPI Peripheral */
      if (HAL_DMA_Abort_IT(hspi->hdmatx) != HAL_OK)     /* 条件判断 */
      {                                                 /* 代码块开始 */
        hspi->hdmatx->XferAbortCallback = NULL;         /* 赋值或初始化 */
        hspi->ErrorCode = HAL_SPI_ERROR_ABORT;          /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        abortcplt = 0U;                                 /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /* Disable the SPI DMA Rx request if enabled */
  if (HAL_IS_BIT_SET(hspi->Instance->CR2, SPI_CR2_RXDMAEN))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Abort the SPI DMA Rx Stream/Channel */
    if (hspi->hdmarx != NULL)                           /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Abort DMA Rx Handle linked to SPI Peripheral */
      if (HAL_DMA_Abort_IT(hspi->hdmarx) !=  HAL_OK)    /* 条件判断 */
      {                                                 /* 代码块开始 */
        hspi->hdmarx->XferAbortCallback = NULL;         /* 赋值或初始化 */
        hspi->ErrorCode = HAL_SPI_ERROR_ABORT;          /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        abortcplt = 0U;                                 /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  if (abortcplt == 1U)                                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Reset Tx and Rx transfer counters */
    hspi->RxXferCount = 0U;                             /* 赋值或初始化 */
    hspi->TxXferCount = 0U;                             /* 赋值或初始化 */

    /* Check error during Abort procedure */
    if (hspi->ErrorCode == HAL_SPI_ERROR_ABORT)         /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* return HAL_Error in case of error during Abort procedure */
      errorcode = HAL_ERROR;                            /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Reset errorCode */
      hspi->ErrorCode = HAL_SPI_ERROR_NONE;             /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Clear the Error flags in the SR register */
    __HAL_SPI_CLEAR_OVRFLAG(hspi);                      /* HAL辅助宏/函数 */
    __HAL_SPI_CLEAR_FREFLAG(hspi);                      /* HAL辅助宏/函数 */

    /* Restore hspi->State to Ready */
    hspi->State = HAL_SPI_STATE_READY;                  /* 赋值或初始化 */

    /* As no DMA to be aborted, call directly user Abort complete callback */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
    hspi->AbortCpltCallback(hspi);                      /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_SPI_AbortCpltCallback(hspi);                    /* HAL SPI操作 */
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */

  return errorcode;                                     /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Pause the DMA Transfer.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for the specified SPI module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPI_DMAPause(SPI_HandleTypeDef *hspi)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Process Locked */
  __HAL_LOCK(hspi);                                     /* HAL辅助宏/函数 */

  /* Disable the SPI DMA Tx & Rx requests */
  CLEAR_BIT(hspi->Instance->CR2, SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN);  /* 执行语句 */

  /* Process Unlocked */
  __HAL_UNLOCK(hspi);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Resume the DMA Transfer.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for the specified SPI module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPI_DMAResume(SPI_HandleTypeDef *hspi)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Process Locked */
  __HAL_LOCK(hspi);                                     /* HAL辅助宏/函数 */

  /* Enable the SPI DMA Tx & Rx requests */
  SET_BIT(hspi->Instance->CR2, SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN);  /* 执行语句 */

  /* Process Unlocked */
  __HAL_UNLOCK(hspi);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Stop the DMA Transfer.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for the specified SPI module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPI_DMAStop(SPI_HandleTypeDef *hspi)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef errorcode = HAL_OK;                 /* HAL状态类型函数 */
  /* The Lock is not implemented on this API to allow the user application
     to call the HAL SPI API under callbacks HAL_SPI_TxCpltCallback() or HAL_SPI_RxCpltCallback() or HAL_SPI_TxRxCpltCallback():
     when calling HAL_DMA_Abort() API the DMA TX/RX Transfer complete interrupt is generated
     and the correspond call back is executed HAL_SPI_TxCpltCallback() or HAL_SPI_RxCpltCallback() or HAL_SPI_TxRxCpltCallback()
     */

  /* Abort the SPI DMA tx Stream/Channel  */
  if (hspi->hdmatx != NULL)                             /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (HAL_OK != HAL_DMA_Abort(hspi->hdmatx))          /* 条件判断 */
    {                                                   /* 代码块开始 */
      SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_DMA);      /* 执行语句 */
      errorcode = HAL_ERROR;                            /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /* Abort the SPI DMA rx Stream/Channel  */
  if (hspi->hdmarx != NULL)                             /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (HAL_OK != HAL_DMA_Abort(hspi->hdmarx))          /* 条件判断 */
    {                                                   /* 代码块开始 */
      SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_DMA);      /* 执行语句 */
      errorcode = HAL_ERROR;                            /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Disable the SPI DMA Tx & Rx requests */
  CLEAR_BIT(hspi->Instance->CR2, SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN);  /* 执行语句 */
  hspi->State = HAL_SPI_STATE_READY;                    /* 赋值或初始化 */
  return errorcode;                                     /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Handle SPI interrupt request.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for the specified SPI module.
  * @retval None
  */
void HAL_SPI_IRQHandler(SPI_HandleTypeDef *hspi)        /* 中断服务函数 */
{                                                       /* 代码块开始 */
  uint32_t itsource = hspi->Instance->CR2;              /* 32位无符号变量/函数 */
  uint32_t itflag   = hspi->Instance->SR;               /* 32位无符号变量/函数 */

  /* SPI in mode Receiver ----------------------------------------------------*/
  if ((SPI_CHECK_FLAG(itflag, SPI_FLAG_OVR) == RESET) &&  /* 条件判断 */
      (SPI_CHECK_FLAG(itflag, SPI_FLAG_RXNE) != RESET) && (SPI_CHECK_IT_SOURCE(itsource, SPI_IT_RXNE) != RESET))  /* 源代码 */
  {                                                     /* 代码块开始 */
    hspi->RxISR(hspi);                                  /* 执行语句 */
    return;                                             /* 返回 */
  }                                                     /* 代码块结束 */

  /* SPI in mode Transmitter -------------------------------------------------*/
  if ((SPI_CHECK_FLAG(itflag, SPI_FLAG_TXE) != RESET) && (SPI_CHECK_IT_SOURCE(itsource, SPI_IT_TXE) != RESET))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hspi->TxISR(hspi);                                  /* 执行语句 */
    return;                                             /* 返回 */
  }                                                     /* 代码块结束 */

  /* SPI in Error Treatment --------------------------------------------------*/
  if (((SPI_CHECK_FLAG(itflag, SPI_FLAG_MODF) != RESET) || (SPI_CHECK_FLAG(itflag, SPI_FLAG_OVR) != RESET)  /* 条件判断 */
       || (SPI_CHECK_FLAG(itflag, SPI_FLAG_FRE) != RESET)) && (SPI_CHECK_IT_SOURCE(itsource, SPI_IT_ERR) != RESET))  /* 源代码 */
  {                                                     /* 代码块开始 */
    /* SPI Overrun error interrupt occurred ----------------------------------*/
    if (SPI_CHECK_FLAG(itflag, SPI_FLAG_OVR) != RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (hspi->State != HAL_SPI_STATE_BUSY_TX)         /* 条件判断 */
      {                                                 /* 代码块开始 */
        SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_OVR);    /* 执行语句 */
        __HAL_SPI_CLEAR_OVRFLAG(hspi);                  /* HAL辅助宏/函数 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        __HAL_SPI_CLEAR_OVRFLAG(hspi);                  /* HAL辅助宏/函数 */
        return;                                         /* 返回 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* SPI Mode Fault error interrupt occurred -------------------------------*/
    if (SPI_CHECK_FLAG(itflag, SPI_FLAG_MODF) != RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_MODF);     /* 执行语句 */
      __HAL_SPI_CLEAR_MODFFLAG(hspi);                   /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* SPI Frame error interrupt occurred ------------------------------------*/
    if (SPI_CHECK_FLAG(itflag, SPI_FLAG_FRE) != RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_FRE);      /* 执行语句 */
      __HAL_SPI_CLEAR_FREFLAG(hspi);                    /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    if (hspi->ErrorCode != HAL_SPI_ERROR_NONE)          /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable all interrupts */
      __HAL_SPI_DISABLE_IT(hspi, SPI_IT_RXNE | SPI_IT_TXE | SPI_IT_ERR);  /* HAL辅助宏/函数 */

      hspi->State = HAL_SPI_STATE_READY;                /* 赋值或初始化 */
      /* Disable the SPI DMA requests if enabled */
      if ((HAL_IS_BIT_SET(itsource, SPI_CR2_TXDMAEN)) || (HAL_IS_BIT_SET(itsource, SPI_CR2_RXDMAEN)))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        CLEAR_BIT(hspi->Instance->CR2, (SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN));  /* 执行语句 */

        /* Abort the SPI DMA Rx channel */
        if (hspi->hdmarx != NULL)                       /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Set the SPI DMA Abort callback :
          will lead to call HAL_SPI_ErrorCallback() at end of DMA abort procedure */
          hspi->hdmarx->XferAbortCallback = SPI_DMAAbortOnError;  /* 赋值或初始化 */
          if (HAL_OK != HAL_DMA_Abort_IT(hspi->hdmarx))  /* 条件判断 */
          {                                             /* 代码块开始 */
            SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_ABORT);  /* 执行语句 */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */
        /* Abort the SPI DMA Tx channel */
        if (hspi->hdmatx != NULL)                       /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Set the SPI DMA Abort callback :
          will lead to call HAL_SPI_ErrorCallback() at end of DMA abort procedure */
          hspi->hdmatx->XferAbortCallback = SPI_DMAAbortOnError;  /* 赋值或初始化 */
          if (HAL_OK != HAL_DMA_Abort_IT(hspi->hdmatx))  /* 条件判断 */
          {                                             /* 代码块开始 */
            SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_ABORT);  /* 执行语句 */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Call user error callback */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
        hspi->ErrorCallback(hspi);                      /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_SPI_ErrorCallback(hspi);                    /* HAL SPI操作 */
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    return;                                             /* 返回 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Tx Transfer completed callback.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
__weak void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hspi);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SPI_TxCpltCallback should be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Rx Transfer completed callback.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
__weak void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hspi);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SPI_RxCpltCallback should be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Tx and Rx Transfer completed callback.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
__weak void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hspi);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SPI_TxRxCpltCallback should be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Tx Half Transfer completed callback.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
__weak void HAL_SPI_TxHalfCpltCallback(SPI_HandleTypeDef *hspi)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hspi);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SPI_TxHalfCpltCallback should be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Rx Half Transfer completed callback.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
__weak void HAL_SPI_RxHalfCpltCallback(SPI_HandleTypeDef *hspi)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hspi);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SPI_RxHalfCpltCallback() should be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Tx and Rx Half Transfer callback.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
__weak void HAL_SPI_TxRxHalfCpltCallback(SPI_HandleTypeDef *hspi)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hspi);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SPI_TxRxHalfCpltCallback() should be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  SPI error callback.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
__weak void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hspi);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SPI_ErrorCallback should be implemented in the user file
   */
  /* NOTE : The ErrorCode parameter in the hspi handle is updated by the SPI processes
            and user can use HAL_SPI_GetError() API to check the latest error occurred
   */
}                                                       /* 代码块结束 */

/**
  * @brief  SPI Abort Complete callback.
  * @param  hspi SPI handle.
  * @retval None
  */
__weak void HAL_SPI_AbortCpltCallback(SPI_HandleTypeDef *hspi)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hspi);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SPI_AbortCpltCallback can be implemented in the user file.
   */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup SPI_Exported_Functions_Group3 Peripheral State and Errors functions
  * @brief   SPI control functions
  *
@verbatim
 ===============================================================================
                      ##### Peripheral State and Errors functions #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to control the SPI.
     (+) HAL_SPI_GetState() API can be helpful to check in run-time the state of the SPI peripheral
     (+) HAL_SPI_GetError() check in run-time Errors occurring during communication
@endverbatim
  * @{
  */

/**
  * @brief  Return the SPI handle state.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval SPI state
  */
HAL_SPI_StateTypeDef HAL_SPI_GetState(SPI_HandleTypeDef *hspi)  /* HAL SPI操作 */
{                                                       /* 代码块开始 */
  /* Return SPI handle state */
  return hspi->State;                                   /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return the SPI error code.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval SPI error code in bitmap format
  */
uint32_t HAL_SPI_GetError(SPI_HandleTypeDef *hspi)      /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  /* Return SPI ErrorCode */
  return hspi->ErrorCode;                               /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup SPI_Private_Functions
  * @brief   Private functions
  * @{
  */

/**
  * @brief  DMA SPI transmit process complete callback.
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA module.
  * @retval None
  */
static void SPI_DMATransmitCplt(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef *)(((DMA_HandleTypeDef *)hdma)->Parent); /* Derogation MISRAC2012-Rule-11.5 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */

  /* Init tickstart for timeout management*/
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  /* DMA Normal Mode */
  if ((hdma->Instance->CR & DMA_SxCR_CIRC) != DMA_SxCR_CIRC)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable ERR interrupt */
    __HAL_SPI_DISABLE_IT(hspi, SPI_IT_ERR);             /* HAL辅助宏/函数 */

    /* Disable Tx DMA Request */
    CLEAR_BIT(hspi->Instance->CR2, SPI_CR2_TXDMAEN);    /* 执行语句 */

    /* Check the end of the transaction */
    if (SPI_EndRxTxTransaction(hspi, SPI_DEFAULT_TIMEOUT, tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_FLAG);     /* 执行语句 */
    }                                                   /* 代码块结束 */

    /* Clear overrun flag in 2 Lines communication mode because received data is not read */
    if (hspi->Init.Direction == SPI_DIRECTION_2LINES)   /* 条件判断 */
    {                                                   /* 代码块开始 */
      __HAL_SPI_CLEAR_OVRFLAG(hspi);                    /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    hspi->TxXferCount = 0U;                             /* 赋值或初始化 */
    hspi->State = HAL_SPI_STATE_READY;                  /* 赋值或初始化 */

    if (hspi->ErrorCode != HAL_SPI_ERROR_NONE)          /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Call user error callback */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
      hspi->ErrorCallback(hspi);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_SPI_ErrorCallback(hspi);                      /* HAL SPI操作 */
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
      return;                                           /* 返回 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /* Call user Tx complete callback */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
  hspi->TxCpltCallback(hspi);                           /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_SPI_TxCpltCallback(hspi);                         /* HAL SPI操作 */
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA SPI receive process complete callback.
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA module.
  * @retval None
  */
static void SPI_DMAReceiveCplt(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef *)(((DMA_HandleTypeDef *)hdma)->Parent); /* Derogation MISRAC2012-Rule-11.5 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */
#if (USE_SPI_CRC != 0U)                                 /* 条件编译 */
  __IO uint32_t tmpreg = 0U;                            /* 赋值或初始化 */
#endif /* USE_SPI_CRC */

  /* Init tickstart for timeout management*/
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  /* DMA Normal Mode */
  if ((hdma->Instance->CR & DMA_SxCR_CIRC) != DMA_SxCR_CIRC)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable ERR interrupt */
    __HAL_SPI_DISABLE_IT(hspi, SPI_IT_ERR);             /* HAL辅助宏/函数 */

#if (USE_SPI_CRC != 0U)                                 /* 条件编译 */
    /* CRC handling */
    if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Wait until RXNE flag */
      if (SPI_WaitFlagStateUntilTimeout(hspi, SPI_FLAG_RXNE, SET, SPI_DEFAULT_TIMEOUT, tickstart) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Error on the CRC reception */
        SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_CRC);    /* 执行语句 */
      }                                                 /* 代码块结束 */
      /* Read CRC */
      tmpreg = READ_REG(hspi->Instance->DR);            /* 赋值或初始化 */
      /* To avoid GCC warning */
      UNUSED(tmpreg);                                   /* 执行语句 */
    }                                                   /* 代码块结束 */
#endif /* USE_SPI_CRC */

    /* Check if we are in Master RX 2 line mode */
    if ((hspi->Init.Direction == SPI_DIRECTION_2LINES) && (hspi->Init.Mode == SPI_MODE_MASTER))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable Rx/Tx DMA Request (done by default to handle the case master rx direction 2 lines) */
      CLEAR_BIT(hspi->Instance->CR2, SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN);  /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Normal case */
      CLEAR_BIT(hspi->Instance->CR2, SPI_CR2_RXDMAEN);  /* 执行语句 */
    }                                                   /* 代码块结束 */

    /* Check the end of the transaction */
    if (SPI_EndRxTransaction(hspi, SPI_DEFAULT_TIMEOUT, tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hspi->ErrorCode = HAL_SPI_ERROR_FLAG;             /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    hspi->RxXferCount = 0U;                             /* 赋值或初始化 */
    hspi->State = HAL_SPI_STATE_READY;                  /* 赋值或初始化 */

#if (USE_SPI_CRC != 0U)                                 /* 条件编译 */
    /* Check if CRC error occurred */
    if (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_CRCERR))      /* 条件判断 */
    {                                                   /* 代码块开始 */
      SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_CRC);      /* 执行语句 */
      __HAL_SPI_CLEAR_CRCERRFLAG(hspi);                 /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
#endif /* USE_SPI_CRC */

    if (hspi->ErrorCode != HAL_SPI_ERROR_NONE)          /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Call user error callback */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
      hspi->ErrorCallback(hspi);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_SPI_ErrorCallback(hspi);                      /* HAL SPI操作 */
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
      return;                                           /* 返回 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /* Call user Rx complete callback */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
  hspi->RxCpltCallback(hspi);                           /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_SPI_RxCpltCallback(hspi);                         /* HAL SPI操作 */
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA SPI transmit receive process complete callback.
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA module.
  * @retval None
  */
static void SPI_DMATransmitReceiveCplt(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef *)(((DMA_HandleTypeDef *)hdma)->Parent); /* Derogation MISRAC2012-Rule-11.5 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */
#if (USE_SPI_CRC != 0U)                                 /* 条件编译 */
  __IO uint32_t tmpreg = 0U;                            /* 赋值或初始化 */
#endif /* USE_SPI_CRC */

  /* Init tickstart for timeout management*/
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  /* DMA Normal Mode */
  if ((hdma->Instance->CR & DMA_SxCR_CIRC) != DMA_SxCR_CIRC)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable ERR interrupt */
    __HAL_SPI_DISABLE_IT(hspi, SPI_IT_ERR);             /* HAL辅助宏/函数 */

#if (USE_SPI_CRC != 0U)                                 /* 条件编译 */
    /* CRC handling */
    if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Wait the CRC data */
      if (SPI_WaitFlagStateUntilTimeout(hspi, SPI_FLAG_RXNE, SET, SPI_DEFAULT_TIMEOUT, tickstart) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_CRC);    /* 执行语句 */
      }                                                 /* 代码块结束 */
      /* Read CRC to Flush DR and RXNE flag */
      tmpreg = READ_REG(hspi->Instance->DR);            /* 赋值或初始化 */
      /* To avoid GCC warning */
      UNUSED(tmpreg);                                   /* 执行语句 */
    }                                                   /* 代码块结束 */
#endif /* USE_SPI_CRC */

    /* Check the end of the transaction */
    if (SPI_EndRxTxTransaction(hspi, SPI_DEFAULT_TIMEOUT, tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_FLAG);     /* 执行语句 */
    }                                                   /* 代码块结束 */

    /* Disable Rx/Tx DMA Request */
    CLEAR_BIT(hspi->Instance->CR2, SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN);  /* 执行语句 */

    hspi->TxXferCount = 0U;                             /* 赋值或初始化 */
    hspi->RxXferCount = 0U;                             /* 赋值或初始化 */
    hspi->State = HAL_SPI_STATE_READY;                  /* 赋值或初始化 */

#if (USE_SPI_CRC != 0U)                                 /* 条件编译 */
    /* Check if CRC error occurred */
    if (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_CRCERR))      /* 条件判断 */
    {                                                   /* 代码块开始 */
      SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_CRC);      /* 执行语句 */
      __HAL_SPI_CLEAR_CRCERRFLAG(hspi);                 /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
#endif /* USE_SPI_CRC */

    if (hspi->ErrorCode != HAL_SPI_ERROR_NONE)          /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Call user error callback */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
      hspi->ErrorCallback(hspi);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_SPI_ErrorCallback(hspi);                      /* HAL SPI操作 */
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
      return;                                           /* 返回 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /* Call user TxRx complete callback */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
  hspi->TxRxCpltCallback(hspi);                         /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_SPI_TxRxCpltCallback(hspi);                       /* HAL SPI操作 */
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA SPI half transmit process complete callback.
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA module.
  * @retval None
  */
static void SPI_DMAHalfTransmitCplt(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef *)(((DMA_HandleTypeDef *)hdma)->Parent); /* Derogation MISRAC2012-Rule-11.5 */

  /* Call user Tx half complete callback */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
  hspi->TxHalfCpltCallback(hspi);                       /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_SPI_TxHalfCpltCallback(hspi);                     /* HAL SPI操作 */
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA SPI half receive process complete callback
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA module.
  * @retval None
  */
static void SPI_DMAHalfReceiveCplt(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef *)(((DMA_HandleTypeDef *)hdma)->Parent); /* Derogation MISRAC2012-Rule-11.5 */

  /* Call user Rx half complete callback */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
  hspi->RxHalfCpltCallback(hspi);                       /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_SPI_RxHalfCpltCallback(hspi);                     /* HAL SPI操作 */
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA SPI half transmit receive process complete callback.
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA module.
  * @retval None
  */
static void SPI_DMAHalfTransmitReceiveCplt(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef *)(((DMA_HandleTypeDef *)hdma)->Parent); /* Derogation MISRAC2012-Rule-11.5 */

  /* Call user TxRx half complete callback */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
  hspi->TxRxHalfCpltCallback(hspi);                     /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_SPI_TxRxHalfCpltCallback(hspi);                   /* HAL SPI操作 */
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA SPI communication error callback.
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA module.
  * @retval None
  */
static void SPI_DMAError(DMA_HandleTypeDef *hdma)       /* 静态函数 */
{                                                       /* 代码块开始 */
  SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef *)(((DMA_HandleTypeDef *)hdma)->Parent); /* Derogation MISRAC2012-Rule-11.5 */

  /* Stop the disable DMA transfer on SPI side */
  CLEAR_BIT(hspi->Instance->CR2, SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN);  /* 执行语句 */

  SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_DMA);          /* 执行语句 */
  hspi->State = HAL_SPI_STATE_READY;                    /* 赋值或初始化 */
  /* Call user error callback */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
  hspi->ErrorCallback(hspi);                            /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_SPI_ErrorCallback(hspi);                          /* HAL SPI操作 */
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA SPI communication abort callback, when initiated by HAL services on Error
  *         (To be called at end of DMA Abort procedure following error occurrence).
  * @param  hdma DMA handle.
  * @retval None
  */
static void SPI_DMAAbortOnError(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef *)(((DMA_HandleTypeDef *)hdma)->Parent); /* Derogation MISRAC2012-Rule-11.5 */
  hspi->RxXferCount = 0U;                               /* 赋值或初始化 */
  hspi->TxXferCount = 0U;                               /* 赋值或初始化 */

  /* Call user error callback */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
  hspi->ErrorCallback(hspi);                            /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_SPI_ErrorCallback(hspi);                          /* HAL SPI操作 */
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA SPI Tx communication abort callback, when initiated by user
  *         (To be called at end of DMA Tx Abort procedure following user abort request).
  * @note   When this callback is executed, User Abort complete call back is called only if no
  *         Abort still ongoing for Rx DMA Handle.
  * @param  hdma DMA handle.
  * @retval None
  */
static void SPI_DMATxAbortCallback(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef *)(((DMA_HandleTypeDef *)hdma)->Parent); /* Derogation MISRAC2012-Rule-11.5 */
  __IO uint32_t count;                                  /* 执行语句 */

  hspi->hdmatx->XferAbortCallback = NULL;               /* 赋值或初始化 */
  count = SPI_DEFAULT_TIMEOUT * (SystemCoreClock / 24U / 1000U);  /* 赋值或初始化 */

  /* Disable Tx DMA Request */
  CLEAR_BIT(hspi->Instance->CR2, SPI_CR2_TXDMAEN);      /* 执行语句 */

  /* Wait until TXE flag is set */
  do                                                    /* do-while循环 */
  {                                                     /* 代码块开始 */
    if (count == 0U)                                    /* 条件判断 */
    {                                                   /* 代码块开始 */
      SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_ABORT);    /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
    count--;                                            /* 执行语句 */
  } while ((hspi->Instance->SR & SPI_FLAG_TXE) == RESET);  /* 代码块结束 */

  /* Check if an Abort process is still ongoing */
  if (hspi->hdmarx != NULL)                             /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (hspi->hdmarx->XferAbortCallback != NULL)        /* 条件判断 */
    {                                                   /* 代码块开始 */
      return;                                           /* 返回 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* No Abort process still ongoing : All DMA Stream/Channel are aborted, call user Abort Complete callback */
  hspi->RxXferCount = 0U;                               /* 赋值或初始化 */
  hspi->TxXferCount = 0U;                               /* 赋值或初始化 */

  /* Check no error during Abort procedure */
  if (hspi->ErrorCode != HAL_SPI_ERROR_ABORT)           /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Reset errorCode */
    hspi->ErrorCode = HAL_SPI_ERROR_NONE;               /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Clear the Error flags in the SR register */
  __HAL_SPI_CLEAR_OVRFLAG(hspi);                        /* HAL辅助宏/函数 */
  __HAL_SPI_CLEAR_FREFLAG(hspi);                        /* HAL辅助宏/函数 */

  /* Restore hspi->State to Ready */
  hspi->State  = HAL_SPI_STATE_READY;                   /* 赋值或初始化 */

  /* Call user Abort complete callback */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
  hspi->AbortCpltCallback(hspi);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_SPI_AbortCpltCallback(hspi);                      /* HAL SPI操作 */
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA SPI Rx communication abort callback, when initiated by user
  *         (To be called at end of DMA Rx Abort procedure following user abort request).
  * @note   When this callback is executed, User Abort complete call back is called only if no
  *         Abort still ongoing for Tx DMA Handle.
  * @param  hdma DMA handle.
  * @retval None
  */
static void SPI_DMARxAbortCallback(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef *)(((DMA_HandleTypeDef *)hdma)->Parent); /* Derogation MISRAC2012-Rule-11.5 */

  /* Disable SPI Peripheral */
  __HAL_SPI_DISABLE(hspi);                              /* HAL辅助宏/函数 */

  hspi->hdmarx->XferAbortCallback = NULL;               /* 赋值或初始化 */

  /* Disable Rx DMA Request */
  CLEAR_BIT(hspi->Instance->CR2, SPI_CR2_RXDMAEN);      /* 执行语句 */

  /* Check Busy flag */
  if (SPI_EndRxTxTransaction(hspi, SPI_DEFAULT_TIMEOUT, HAL_GetTick()) != HAL_OK)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_ABORT);      /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Check if an Abort process is still ongoing */
  if (hspi->hdmatx != NULL)                             /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (hspi->hdmatx->XferAbortCallback != NULL)        /* 条件判断 */
    {                                                   /* 代码块开始 */
      return;                                           /* 返回 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* No Abort process still ongoing : All DMA Stream/Channel are aborted, call user Abort Complete callback */
  hspi->RxXferCount = 0U;                               /* 赋值或初始化 */
  hspi->TxXferCount = 0U;                               /* 赋值或初始化 */

  /* Check no error during Abort procedure */
  if (hspi->ErrorCode != HAL_SPI_ERROR_ABORT)           /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Reset errorCode */
    hspi->ErrorCode = HAL_SPI_ERROR_NONE;               /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Clear the Error flags in the SR register */
  __HAL_SPI_CLEAR_OVRFLAG(hspi);                        /* HAL辅助宏/函数 */
  __HAL_SPI_CLEAR_FREFLAG(hspi);                        /* HAL辅助宏/函数 */

  /* Restore hspi->State to Ready */
  hspi->State  = HAL_SPI_STATE_READY;                   /* 赋值或初始化 */

  /* Call user Abort complete callback */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
  hspi->AbortCpltCallback(hspi);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_SPI_AbortCpltCallback(hspi);                      /* HAL SPI操作 */
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
}                                                       /* 代码块结束 */

/**
  * @brief  Rx 8-bit handler for Transmit and Receive in Interrupt mode.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
static void SPI_2linesRxISR_8BIT(struct __SPI_HandleTypeDef *hspi)  /* 静态函数 */
{                                                       /* 代码块开始 */
  /* Receive data in 8bit mode */
  *hspi->pRxBuffPtr = *((__IO uint8_t *)&hspi->Instance->DR);  /* 赋值或初始化 */
  hspi->pRxBuffPtr++;                                   /* 执行语句 */
  hspi->RxXferCount--;                                  /* 执行语句 */

  /* Check end of the reception */
  if (hspi->RxXferCount == 0U)                          /* 条件判断 */
  {                                                     /* 代码块开始 */
#if (USE_SPI_CRC != 0U)                                 /* 条件编译 */
    if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hspi->RxISR =  SPI_2linesRxISR_8BITCRC;           /* 赋值或初始化 */
      return;                                           /* 返回 */
    }                                                   /* 代码块结束 */
#endif /* USE_SPI_CRC */

    /* Disable RXNE  and ERR interrupt */
    __HAL_SPI_DISABLE_IT(hspi, (SPI_IT_RXNE | SPI_IT_ERR));  /* HAL辅助宏/函数 */

    if (hspi->TxXferCount == 0U)                        /* 条件判断 */
    {                                                   /* 代码块开始 */
      SPI_CloseRxTx_ISR(hspi);                          /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

#if (USE_SPI_CRC != 0U)                                 /* 条件编译 */
/**
  * @brief  Rx 8-bit handler for Transmit and Receive in Interrupt mode.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
static void SPI_2linesRxISR_8BITCRC(struct __SPI_HandleTypeDef *hspi)  /* 静态函数 */
{                                                       /* 代码块开始 */
  __IO uint32_t tmpreg = 0U;                            /* 赋值或初始化 */

  /* Read 8bit CRC to flush Data Register */
  tmpreg = READ_REG(*(__IO uint8_t *)&hspi->Instance->DR);  /* 赋值或初始化 */
  /* To avoid GCC warning */
  UNUSED(tmpreg);                                       /* 执行语句 */

  /* Disable RXNE and ERR interrupt */
  __HAL_SPI_DISABLE_IT(hspi, (SPI_IT_RXNE | SPI_IT_ERR));  /* HAL辅助宏/函数 */

  if (hspi->TxXferCount == 0U)                          /* 条件判断 */
  {                                                     /* 代码块开始 */
    SPI_CloseRxTx_ISR(hspi);                            /* 执行语句 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */
#endif /* USE_SPI_CRC */

/**
  * @brief  Tx 8-bit handler for Transmit and Receive in Interrupt mode.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
static void SPI_2linesTxISR_8BIT(struct __SPI_HandleTypeDef *hspi)  /* 静态函数 */
{                                                       /* 代码块开始 */
  *(__IO uint8_t *)&hspi->Instance->DR = (*hspi->pTxBuffPtr);  /* 赋值或初始化 */
  hspi->pTxBuffPtr++;                                   /* 执行语句 */
  hspi->TxXferCount--;                                  /* 执行语句 */

  /* Check the end of the transmission */
  if (hspi->TxXferCount == 0U)                          /* 条件判断 */
  {                                                     /* 代码块开始 */
#if (USE_SPI_CRC != 0U)                                 /* 条件编译 */
    if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set CRC Next Bit to send CRC */
      SET_BIT(hspi->Instance->CR1, SPI_CR1_CRCNEXT);    /* 执行语句 */
      /* Disable TXE interrupt */
      __HAL_SPI_DISABLE_IT(hspi, SPI_IT_TXE);           /* HAL辅助宏/函数 */
      return;                                           /* 返回 */
    }                                                   /* 代码块结束 */
#endif /* USE_SPI_CRC */

    /* Disable TXE interrupt */
    __HAL_SPI_DISABLE_IT(hspi, SPI_IT_TXE);             /* HAL辅助宏/函数 */

    if (hspi->RxXferCount == 0U)                        /* 条件判断 */
    {                                                   /* 代码块开始 */
      SPI_CloseRxTx_ISR(hspi);                          /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Rx 16-bit handler for Transmit and Receive in Interrupt mode.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
static void SPI_2linesRxISR_16BIT(struct __SPI_HandleTypeDef *hspi)  /* 静态函数 */
{                                                       /* 代码块开始 */
  /* Receive data in 16 Bit mode */
  *((uint16_t *)hspi->pRxBuffPtr) = (uint16_t)(hspi->Instance->DR);  /* 赋值或初始化 */
  hspi->pRxBuffPtr += sizeof(uint16_t);                 /* 赋值或初始化 */
  hspi->RxXferCount--;                                  /* 执行语句 */

  if (hspi->RxXferCount == 0U)                          /* 条件判断 */
  {                                                     /* 代码块开始 */
#if (USE_SPI_CRC != 0U)                                 /* 条件编译 */
    if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hspi->RxISR =  SPI_2linesRxISR_16BITCRC;          /* 赋值或初始化 */
      return;                                           /* 返回 */
    }                                                   /* 代码块结束 */
#endif /* USE_SPI_CRC */

    /* Disable RXNE interrupt */
    __HAL_SPI_DISABLE_IT(hspi, SPI_IT_RXNE);            /* HAL辅助宏/函数 */

    if (hspi->TxXferCount == 0U)                        /* 条件判断 */
    {                                                   /* 代码块开始 */
      SPI_CloseRxTx_ISR(hspi);                          /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

#if (USE_SPI_CRC != 0U)                                 /* 条件编译 */
/**
  * @brief  Manage the CRC 16-bit receive for Transmit and Receive in Interrupt mode.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
static void SPI_2linesRxISR_16BITCRC(struct __SPI_HandleTypeDef *hspi)  /* 静态函数 */
{                                                       /* 代码块开始 */
  __IO uint32_t tmpreg = 0U;                            /* 赋值或初始化 */

  /* Read 16bit CRC to flush Data Register */
  tmpreg = READ_REG(hspi->Instance->DR);                /* 赋值或初始化 */
  /* To avoid GCC warning */
  UNUSED(tmpreg);                                       /* 执行语句 */

  /* Disable RXNE interrupt */
  __HAL_SPI_DISABLE_IT(hspi, SPI_IT_RXNE);              /* HAL辅助宏/函数 */

  SPI_CloseRxTx_ISR(hspi);                              /* 执行语句 */
}                                                       /* 代码块结束 */
#endif /* USE_SPI_CRC */

/**
  * @brief  Tx 16-bit handler for Transmit and Receive in Interrupt mode.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
static void SPI_2linesTxISR_16BIT(struct __SPI_HandleTypeDef *hspi)  /* 静态函数 */
{                                                       /* 代码块开始 */
  /* Transmit data in 16 Bit mode */
  hspi->Instance->DR = *((uint16_t *)hspi->pTxBuffPtr);  /* 赋值或初始化 */
  hspi->pTxBuffPtr += sizeof(uint16_t);                 /* 赋值或初始化 */
  hspi->TxXferCount--;                                  /* 执行语句 */

  /* Enable CRC Transmission */
  if (hspi->TxXferCount == 0U)                          /* 条件判断 */
  {                                                     /* 代码块开始 */
#if (USE_SPI_CRC != 0U)                                 /* 条件编译 */
    if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set CRC Next Bit to send CRC */
      SET_BIT(hspi->Instance->CR1, SPI_CR1_CRCNEXT);    /* 执行语句 */
      /* Disable TXE interrupt */
      __HAL_SPI_DISABLE_IT(hspi, SPI_IT_TXE);           /* HAL辅助宏/函数 */
      return;                                           /* 返回 */
    }                                                   /* 代码块结束 */
#endif /* USE_SPI_CRC */

    /* Disable TXE interrupt */
    __HAL_SPI_DISABLE_IT(hspi, SPI_IT_TXE);             /* HAL辅助宏/函数 */

    if (hspi->RxXferCount == 0U)                        /* 条件判断 */
    {                                                   /* 代码块开始 */
      SPI_CloseRxTx_ISR(hspi);                          /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

#if (USE_SPI_CRC != 0U)                                 /* 条件编译 */
/**
  * @brief  Manage the CRC 8-bit receive in Interrupt context.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
static void SPI_RxISR_8BITCRC(struct __SPI_HandleTypeDef *hspi)  /* 静态函数 */
{                                                       /* 代码块开始 */
  __IO uint32_t tmpreg = 0U;                            /* 赋值或初始化 */

  /* Read 8bit CRC to flush Data Register */
  tmpreg = READ_REG(*(__IO uint8_t *)&hspi->Instance->DR);  /* 赋值或初始化 */
  /* To avoid GCC warning */
  UNUSED(tmpreg);                                       /* 执行语句 */

  SPI_CloseRx_ISR(hspi);                                /* 执行语句 */
}                                                       /* 代码块结束 */
#endif /* USE_SPI_CRC */

/**
  * @brief  Manage the receive 8-bit in Interrupt context.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
static void SPI_RxISR_8BIT(struct __SPI_HandleTypeDef *hspi)  /* 静态函数 */
{                                                       /* 代码块开始 */
  *hspi->pRxBuffPtr = (*(__IO uint8_t *)&hspi->Instance->DR);  /* 赋值或初始化 */
  hspi->pRxBuffPtr++;                                   /* 执行语句 */
  hspi->RxXferCount--;                                  /* 执行语句 */

#if (USE_SPI_CRC != 0U)                                 /* 条件编译 */
  /* Enable CRC Transmission */
  if ((hspi->RxXferCount == 1U) && (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    SET_BIT(hspi->Instance->CR1, SPI_CR1_CRCNEXT);      /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif /* USE_SPI_CRC */

  if (hspi->RxXferCount == 0U)                          /* 条件判断 */
  {                                                     /* 代码块开始 */
#if (USE_SPI_CRC != 0U)                                 /* 条件编译 */
    if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hspi->RxISR =  SPI_RxISR_8BITCRC;                 /* 赋值或初始化 */
      return;                                           /* 返回 */
    }                                                   /* 代码块结束 */
#endif /* USE_SPI_CRC */
    SPI_CloseRx_ISR(hspi);                              /* 执行语句 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

#if (USE_SPI_CRC != 0U)                                 /* 条件编译 */
/**
  * @brief  Manage the CRC 16-bit receive in Interrupt context.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
static void SPI_RxISR_16BITCRC(struct __SPI_HandleTypeDef *hspi)  /* 静态函数 */
{                                                       /* 代码块开始 */
  __IO uint32_t tmpreg = 0U;                            /* 赋值或初始化 */

  /* Read 16bit CRC to flush Data Register */
  tmpreg = READ_REG(hspi->Instance->DR);                /* 赋值或初始化 */
  /* To avoid GCC warning */
  UNUSED(tmpreg);                                       /* 执行语句 */

  /* Disable RXNE and ERR interrupt */
  __HAL_SPI_DISABLE_IT(hspi, (SPI_IT_RXNE | SPI_IT_ERR));  /* HAL辅助宏/函数 */

  SPI_CloseRx_ISR(hspi);                                /* 执行语句 */
}                                                       /* 代码块结束 */
#endif /* USE_SPI_CRC */

/**
  * @brief  Manage the 16-bit receive in Interrupt context.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
static void SPI_RxISR_16BIT(struct __SPI_HandleTypeDef *hspi)  /* 静态函数 */
{                                                       /* 代码块开始 */
  *((uint16_t *)hspi->pRxBuffPtr) = (uint16_t)(hspi->Instance->DR);  /* 赋值或初始化 */
  hspi->pRxBuffPtr += sizeof(uint16_t);                 /* 赋值或初始化 */
  hspi->RxXferCount--;                                  /* 执行语句 */

#if (USE_SPI_CRC != 0U)                                 /* 条件编译 */
  /* Enable CRC Transmission */
  if ((hspi->RxXferCount == 1U) && (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    SET_BIT(hspi->Instance->CR1, SPI_CR1_CRCNEXT);      /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif /* USE_SPI_CRC */

  if (hspi->RxXferCount == 0U)                          /* 条件判断 */
  {                                                     /* 代码块开始 */
#if (USE_SPI_CRC != 0U)                                 /* 条件编译 */
    if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hspi->RxISR = SPI_RxISR_16BITCRC;                 /* 赋值或初始化 */
      return;                                           /* 返回 */
    }                                                   /* 代码块结束 */
#endif /* USE_SPI_CRC */
    SPI_CloseRx_ISR(hspi);                              /* 执行语句 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Handle the data 8-bit transmit in Interrupt mode.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
static void SPI_TxISR_8BIT(struct __SPI_HandleTypeDef *hspi)  /* 静态函数 */
{                                                       /* 代码块开始 */
  *(__IO uint8_t *)&hspi->Instance->DR = (*hspi->pTxBuffPtr);  /* 赋值或初始化 */
  hspi->pTxBuffPtr++;                                   /* 执行语句 */
  hspi->TxXferCount--;                                  /* 执行语句 */

  if (hspi->TxXferCount == 0U)                          /* 条件判断 */
  {                                                     /* 代码块开始 */
#if (USE_SPI_CRC != 0U)                                 /* 条件编译 */
    if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable CRC Transmission */
      SET_BIT(hspi->Instance->CR1, SPI_CR1_CRCNEXT);    /* 执行语句 */
    }                                                   /* 代码块结束 */
#endif /* USE_SPI_CRC */
    SPI_CloseTx_ISR(hspi);                              /* 执行语句 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Handle the data 16-bit transmit in Interrupt mode.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
static void SPI_TxISR_16BIT(struct __SPI_HandleTypeDef *hspi)  /* 静态函数 */
{                                                       /* 代码块开始 */
  /* Transmit data in 16 Bit mode */
  hspi->Instance->DR = *((uint16_t *)hspi->pTxBuffPtr);  /* 赋值或初始化 */
  hspi->pTxBuffPtr += sizeof(uint16_t);                 /* 赋值或初始化 */
  hspi->TxXferCount--;                                  /* 执行语句 */

  if (hspi->TxXferCount == 0U)                          /* 条件判断 */
  {                                                     /* 代码块开始 */
#if (USE_SPI_CRC != 0U)                                 /* 条件编译 */
    if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable CRC Transmission */
      SET_BIT(hspi->Instance->CR1, SPI_CR1_CRCNEXT);    /* 执行语句 */
    }                                                   /* 代码块结束 */
#endif /* USE_SPI_CRC */
    SPI_CloseTx_ISR(hspi);                              /* 执行语句 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Handle SPI Communication Timeout.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *              the configuration information for SPI module.
  * @param  Flag SPI flag to check
  * @param  State flag state to check
  * @param  Timeout Timeout duration
  * @param  Tickstart tick start value
  * @retval HAL status
  */
static HAL_StatusTypeDef SPI_WaitFlagStateUntilTimeout(SPI_HandleTypeDef *hspi, uint32_t Flag, FlagStatus State,  /* 静态变量/函数 */
                                                       uint32_t Timeout, uint32_t Tickstart)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  __IO uint32_t count;                                  /* 执行语句 */
  uint32_t tmp_timeout;                                 /* 32位无符号变量/函数 */
  uint32_t tmp_tickstart;                               /* 32位无符号变量/函数 */

  /* Adjust Timeout value  in case of end of transfer */
  tmp_timeout   = Timeout - (HAL_GetTick() - Tickstart);  /* 赋值或初始化 */
  tmp_tickstart = HAL_GetTick();                        /* 赋值或初始化 */

  /* Calculate Timeout based on a software loop to avoid blocking issue if Systick is disabled */
  count = tmp_timeout * ((SystemCoreClock * 32U) >> 20U);  /* 赋值或初始化 */

  while ((__HAL_SPI_GET_FLAG(hspi, Flag) ? SET : RESET) != State)  /* while循环 */
  {                                                     /* 代码块开始 */
    if (Timeout != HAL_MAX_DELAY)                       /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (((HAL_GetTick() - tmp_tickstart) >= tmp_timeout) || (tmp_timeout == 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Disable the SPI and reset the CRC: the CRC value should be cleared
           on both master and slave sides in order to resynchronize the master
           and slave for their respective CRC calculation */

        /* Disable TXE, RXNE and ERR interrupts for the interrupt process */
        __HAL_SPI_DISABLE_IT(hspi, (SPI_IT_TXE | SPI_IT_RXNE | SPI_IT_ERR));  /* HAL辅助宏/函数 */

        if ((hspi->Init.Mode == SPI_MODE_MASTER) && ((hspi->Init.Direction == SPI_DIRECTION_1LINE)  /* 条件判断 */
                                                     || (hspi->Init.Direction == SPI_DIRECTION_2LINES_RXONLY)))  /* 源代码 */
        {                                               /* 代码块开始 */
          /* Disable SPI peripheral */
          __HAL_SPI_DISABLE(hspi);                      /* HAL辅助宏/函数 */
        }                                               /* 代码块结束 */

        /* Reset CRC Calculation */
        if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)  /* 条件判断 */
        {                                               /* 代码块开始 */
          SPI_RESET_CRC(hspi);                          /* 执行语句 */
        }                                               /* 代码块结束 */

        hspi->State = HAL_SPI_STATE_READY;              /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hspi);                             /* HAL辅助宏/函数 */

        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
      /* If Systick is disabled or not incremented, deactivate timeout to go in disable loop procedure */
      if(count == 0U)                                   /* 条件判断 */
      {                                                 /* 代码块开始 */
        tmp_timeout = 0U;                               /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      count--;                                          /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Handle the check of the RX transaction complete.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @param  Timeout Timeout duration
  * @param  Tickstart tick start value
  * @retval HAL status
  */
static HAL_StatusTypeDef SPI_EndRxTransaction(SPI_HandleTypeDef *hspi,  uint32_t Timeout, uint32_t Tickstart)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  if ((hspi->Init.Mode == SPI_MODE_MASTER) && ((hspi->Init.Direction == SPI_DIRECTION_1LINE)  /* 条件判断 */
                                               || (hspi->Init.Direction == SPI_DIRECTION_2LINES_RXONLY)))  /* 源代码 */
  {                                                     /* 代码块开始 */
    /* Disable SPI peripheral */
    __HAL_SPI_DISABLE(hspi);                            /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Erratasheet: BSY bit may stay high at the end of a data transfer in Slave mode */
  if (hspi->Init.Mode == SPI_MODE_MASTER)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (hspi->Init.Direction != SPI_DIRECTION_2LINES_RXONLY)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Control the BSY flag */
      if (SPI_WaitFlagStateUntilTimeout(hspi, SPI_FLAG_BSY, RESET, Timeout, Tickstart) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_FLAG);   /* 执行语句 */
        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Wait the RXNE reset */
      if (SPI_WaitFlagStateUntilTimeout(hspi, SPI_FLAG_RXNE, RESET, Timeout, Tickstart) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_FLAG);   /* 执行语句 */
        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Wait the RXNE reset */
    if (SPI_WaitFlagStateUntilTimeout(hspi, SPI_FLAG_RXNE, RESET, Timeout, Tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_FLAG);     /* 执行语句 */
      return HAL_TIMEOUT;                               /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Handle the check of the RXTX or TX transaction complete.
  * @param  hspi SPI handle
  * @param  Timeout Timeout duration
  * @param  Tickstart tick start value
  * @retval HAL status
  */
static HAL_StatusTypeDef SPI_EndRxTxTransaction(SPI_HandleTypeDef *hspi, uint32_t Timeout, uint32_t Tickstart)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  /* Timeout in µs */
  __IO uint32_t count = SPI_BSY_FLAG_WORKAROUND_TIMEOUT * (SystemCoreClock / 24U / 1000000U);  /* 赋值或初始化 */
  /* Erratasheet: BSY bit may stay high at the end of a data transfer in Slave mode */
  if (hspi->Init.Mode == SPI_MODE_MASTER)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Control the BSY flag */
    if (SPI_WaitFlagStateUntilTimeout(hspi, SPI_FLAG_BSY, RESET, Timeout, Tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_FLAG);     /* 执行语句 */
      return HAL_TIMEOUT;                               /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Wait BSY flag during 1 Byte time transfer in case of Full-Duplex and Tx transfer
    * If Timeout is reached, the transfer is considered as finish.
    * User have to calculate the timeout value to fit with the time of 1 byte transfer.
    * This time is directly link with the SPI clock from Master device.
    */
    do                                                  /* do-while循环 */
    {                                                   /* 代码块开始 */
      if (count == 0U)                                  /* 条件判断 */
      {                                                 /* 代码块开始 */
        break;                                          /* 跳出 */
      }                                                 /* 代码块结束 */
      count--;                                          /* 执行语句 */
    } while (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_BSY) != RESET);  /* 代码块结束 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Handle the end of the RXTX transaction.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
static void SPI_CloseRxTx_ISR(SPI_HandleTypeDef *hspi)  /* 静态函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */
  __IO uint32_t count = SPI_DEFAULT_TIMEOUT * (SystemCoreClock / 24U / 1000U);  /* 赋值或初始化 */

  /* Init tickstart for timeout management */
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  /* Disable ERR interrupt */
  __HAL_SPI_DISABLE_IT(hspi, SPI_IT_ERR);               /* HAL辅助宏/函数 */

  /* Wait until TXE flag is set */
  do                                                    /* do-while循环 */
  {                                                     /* 代码块开始 */
    if (count == 0U)                                    /* 条件判断 */
    {                                                   /* 代码块开始 */
      SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_FLAG);     /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
    count--;                                            /* 执行语句 */
  } while ((hspi->Instance->SR & SPI_FLAG_TXE) == RESET);  /* 代码块结束 */

  /* Check the end of the transaction */
  if (SPI_EndRxTxTransaction(hspi, SPI_DEFAULT_TIMEOUT, tickstart) != HAL_OK)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_FLAG);       /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Clear overrun flag in 2 Lines communication mode because received is not read */
  if (hspi->Init.Direction == SPI_DIRECTION_2LINES)     /* 条件判断 */
  {                                                     /* 代码块开始 */
    __HAL_SPI_CLEAR_OVRFLAG(hspi);                      /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

#if (USE_SPI_CRC != 0U)                                 /* 条件编译 */
  /* Check if CRC error occurred */
  if (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_CRCERR) != RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hspi->State = HAL_SPI_STATE_READY;                  /* 赋值或初始化 */
    SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_CRC);        /* 执行语句 */
    __HAL_SPI_CLEAR_CRCERRFLAG(hspi);                   /* HAL辅助宏/函数 */
    /* Call user error callback */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
    hspi->ErrorCallback(hspi);                          /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_SPI_ErrorCallback(hspi);                        /* HAL SPI操作 */
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
#endif /* USE_SPI_CRC */
    if (hspi->ErrorCode == HAL_SPI_ERROR_NONE)          /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (hspi->State == HAL_SPI_STATE_BUSY_RX)         /* 条件判断 */
      {                                                 /* 代码块开始 */
        hspi->State = HAL_SPI_STATE_READY;              /* 赋值或初始化 */
        /* Call user Rx complete callback */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
        hspi->RxCpltCallback(hspi);                     /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_SPI_RxCpltCallback(hspi);                   /* HAL SPI操作 */
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        hspi->State = HAL_SPI_STATE_READY;              /* 赋值或初始化 */
        /* Call user TxRx complete callback */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
        hspi->TxRxCpltCallback(hspi);                   /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_SPI_TxRxCpltCallback(hspi);                 /* HAL SPI操作 */
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hspi->State = HAL_SPI_STATE_READY;                /* 赋值或初始化 */
      /* Call user error callback */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
      hspi->ErrorCallback(hspi);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_SPI_ErrorCallback(hspi);                      /* HAL SPI操作 */
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
#if (USE_SPI_CRC != 0U)                                 /* 条件编译 */
  }                                                     /* 代码块结束 */
#endif /* USE_SPI_CRC */
}                                                       /* 代码块结束 */

/**
  * @brief  Handle the end of the RX transaction.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
static void SPI_CloseRx_ISR(SPI_HandleTypeDef *hspi)    /* 静态函数 */
{                                                       /* 代码块开始 */
  /* Disable RXNE and ERR interrupt */
  __HAL_SPI_DISABLE_IT(hspi, (SPI_IT_RXNE | SPI_IT_ERR));  /* HAL辅助宏/函数 */

  /* Check the end of the transaction */
  if (SPI_EndRxTransaction(hspi, SPI_DEFAULT_TIMEOUT, HAL_GetTick()) != HAL_OK)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_FLAG);       /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Clear overrun flag in 2 Lines communication mode because received is not read */
  if (hspi->Init.Direction == SPI_DIRECTION_2LINES)     /* 条件判断 */
  {                                                     /* 代码块开始 */
    __HAL_SPI_CLEAR_OVRFLAG(hspi);                      /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  hspi->State = HAL_SPI_STATE_READY;                    /* 赋值或初始化 */

#if (USE_SPI_CRC != 0U)                                 /* 条件编译 */
  /* Check if CRC error occurred */
  if (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_CRCERR) != RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_CRC);        /* 执行语句 */
    __HAL_SPI_CLEAR_CRCERRFLAG(hspi);                   /* HAL辅助宏/函数 */
    /* Call user error callback */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
    hspi->ErrorCallback(hspi);                          /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_SPI_ErrorCallback(hspi);                        /* HAL SPI操作 */
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
#endif /* USE_SPI_CRC */
    if (hspi->ErrorCode == HAL_SPI_ERROR_NONE)          /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Call user Rx complete callback */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
      hspi->RxCpltCallback(hspi);                       /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_SPI_RxCpltCallback(hspi);                     /* HAL SPI操作 */
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Call user error callback */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
      hspi->ErrorCallback(hspi);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_SPI_ErrorCallback(hspi);                      /* HAL SPI操作 */
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
#if (USE_SPI_CRC != 0U)                                 /* 条件编译 */
  }                                                     /* 代码块结束 */
#endif /* USE_SPI_CRC */
}                                                       /* 代码块结束 */

/**
  * @brief  Handle the end of the TX transaction.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
static void SPI_CloseTx_ISR(SPI_HandleTypeDef *hspi)    /* 静态函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */
  __IO uint32_t count = SPI_DEFAULT_TIMEOUT * (SystemCoreClock / 24U / 1000U);  /* 赋值或初始化 */

  /* Init tickstart for timeout management*/
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  /* Wait until TXE flag is set */
  do                                                    /* do-while循环 */
  {                                                     /* 代码块开始 */
    if (count == 0U)                                    /* 条件判断 */
    {                                                   /* 代码块开始 */
      SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_FLAG);     /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
    count--;                                            /* 执行语句 */
  } while ((hspi->Instance->SR & SPI_FLAG_TXE) == RESET);  /* 代码块结束 */

  /* Disable TXE and ERR interrupt */
  __HAL_SPI_DISABLE_IT(hspi, (SPI_IT_TXE | SPI_IT_ERR));  /* HAL辅助宏/函数 */

  /* Check the end of the transaction */
  if (SPI_EndRxTxTransaction(hspi, SPI_DEFAULT_TIMEOUT, tickstart) != HAL_OK)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_FLAG);       /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Clear overrun flag in 2 Lines communication mode because received is not read */
  if (hspi->Init.Direction == SPI_DIRECTION_2LINES)     /* 条件判断 */
  {                                                     /* 代码块开始 */
    __HAL_SPI_CLEAR_OVRFLAG(hspi);                      /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  hspi->State = HAL_SPI_STATE_READY;                    /* 赋值或初始化 */
  if (hspi->ErrorCode != HAL_SPI_ERROR_NONE)            /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Call user error callback */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
    hspi->ErrorCallback(hspi);                          /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_SPI_ErrorCallback(hspi);                        /* HAL SPI操作 */
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Call user Rx complete callback */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
    hspi->TxCpltCallback(hspi);                         /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_SPI_TxCpltCallback(hspi);                       /* HAL SPI操作 */
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Handle abort a Rx transaction.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
static void SPI_AbortRx_ISR(SPI_HandleTypeDef *hspi)    /* 静态函数 */
{                                                       /* 代码块开始 */
  __IO uint32_t tmpreg = 0U;                            /* 赋值或初始化 */
  __IO uint32_t count = SPI_DEFAULT_TIMEOUT * (SystemCoreClock / 24U / 1000U);  /* 赋值或初始化 */

  /* Wait until TXE flag is set */
  do                                                    /* do-while循环 */
  {                                                     /* 代码块开始 */
    if (count == 0U)                                    /* 条件判断 */
    {                                                   /* 代码块开始 */
      SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_ABORT);    /* 执行语句 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
    count--;                                            /* 执行语句 */
  } while ((hspi->Instance->SR & SPI_FLAG_TXE) == RESET);  /* 代码块结束 */

  /* Disable SPI Peripheral */
  __HAL_SPI_DISABLE(hspi);                              /* HAL辅助宏/函数 */

  /* Disable TXEIE, RXNEIE and ERRIE(mode fault event, overrun error, TI frame error) interrupts */
  CLEAR_BIT(hspi->Instance->CR2, (SPI_CR2_TXEIE | SPI_CR2_RXNEIE | SPI_CR2_ERRIE));  /* 执行语句 */

  /* Flush Data Register by a blank read */
  tmpreg = READ_REG(hspi->Instance->DR);                /* 赋值或初始化 */
  /* To avoid GCC warning */
  UNUSED(tmpreg);                                       /* 执行语句 */

  hspi->State = HAL_SPI_STATE_ABORT;                    /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  Handle abort a Tx or Rx/Tx transaction.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
static void SPI_AbortTx_ISR(SPI_HandleTypeDef *hspi)    /* 静态函数 */
{                                                       /* 代码块开始 */
  /* Disable TXEIE interrupt */
  CLEAR_BIT(hspi->Instance->CR2, (SPI_CR2_TXEIE));      /* 执行语句 */

  /* Disable SPI Peripheral */
  __HAL_SPI_DISABLE(hspi);                              /* HAL辅助宏/函数 */

  hspi->State = HAL_SPI_STATE_ABORT;                    /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

#endif /* HAL_SPI_MODULE_ENABLED */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
