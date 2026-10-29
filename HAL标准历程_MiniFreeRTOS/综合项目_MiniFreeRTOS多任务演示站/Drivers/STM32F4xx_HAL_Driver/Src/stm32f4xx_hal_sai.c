/* 文件: stm32f4xx_hal_sai.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_sai.c
  * @author  MCD Application Team
  * @brief   SAI HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Serial Audio Interface (SAI) peripheral:
  *           + Initialization/de-initialization functions
  *           + I/O operation functions
  *           + Peripheral Control functions
  *           + Peripheral State functions
  *
  @verbatim
  ==============================================================================
                  ##### How to use this driver #####
  ==============================================================================

  [..]
    The SAI HAL driver can be used as follows:

    (#) Declare a SAI_HandleTypeDef handle structure (eg. SAI_HandleTypeDef hsai).
    (#) Initialize the SAI low level resources by implementing the HAL_SAI_MspInit() API:
        (##) Enable the SAI interface clock.
        (##) SAI pins configuration:
            (+++) Enable the clock for the SAI GPIOs.
            (+++) Configure these SAI pins as alternate function pull-up.
        (##) NVIC configuration if you need to use interrupt process (HAL_SAI_Transmit_IT()
             and HAL_SAI_Receive_IT() APIs):
            (+++) Configure the SAI interrupt priority.
            (+++) Enable the NVIC SAI IRQ handle.

        (##) DMA Configuration if you need to use DMA process (HAL_SAI_Transmit_DMA()
             and HAL_SAI_Receive_DMA() APIs):
            (+++) Declare a DMA handle structure for the Tx/Rx stream.
            (+++) Enable the DMAx interface clock.
            (+++) Configure the declared DMA handle structure with the required Tx/Rx parameters.
            (+++) Configure the DMA Tx/Rx Stream.
            (+++) Associate the initialized DMA handle to the SAI DMA Tx/Rx handle.
            (+++) Configure the priority and enable the NVIC for the transfer complete interrupt on the
                DMA Tx/Rx Stream.

    (#) The initialization can be done by two ways
        (##) Expert mode : Initialize the structures Init, FrameInit and SlotInit and call HAL_SAI_Init().
        (##) Simplified mode : Initialize the high part of Init Structure and call HAL_SAI_InitProtocol().

  [..]
    (@) The specific SAI interrupts (FIFO request and Overrun underrun interrupt)
        will be managed using the macros __HAL_SAI_ENABLE_IT() and __HAL_SAI_DISABLE_IT()
        inside the transmit and receive process.

  [..]
   (@) SAI Clock Source configuration is managed differently depending on the selected
       STM32F4 devices :
       (+@) For STM32F446xx devices, the configuration is managed through RCCEx_PeriphCLKConfig()
            function in the HAL RCC drivers
       (+@) For STM32F439xx/STM32F437xx/STM32F429xx/STM32F427xx devices, the configuration
            is managed within HAL SAI drivers through HAL_SAI_Init() function using
            ClockSource field of SAI_InitTypeDef structure.
  [..]
   (@) Make sure that either:
       (+@) I2S PLL is configured or
       (+@) SAI PLL is configured or
       (+@) External clock source is configured after setting correctly
            the define constant EXTERNAL_CLOCK_VALUE in the stm32f4xx_hal_conf.h file.
  [..]
    (@) In master Tx mode: enabling the audio block immediately generates the bit clock
        for the external slaves even if there is no data in the FIFO, However FS signal
        generation is conditioned by the presence of data in the FIFO.

  [..]
    (@) In master Rx mode: enabling the audio block immediately generates the bit clock
        and FS signal for the external slaves.

  [..]
    (@) It is mandatory to respect the following conditions in order to avoid bad SAI behavior:
        (+@) First bit Offset <= (SLOT size - Data size)
        (+@) Data size <= SLOT size
        (+@) Number of SLOT x SLOT size = Frame length
        (+@) The number of slots should be even when SAI_FS_CHANNEL_IDENTIFICATION is selected.

  [..]
    Three operation modes are available within this driver :

    *** Polling mode IO operation ***
    =================================
    [..]
      (+) Send an amount of data in blocking mode using HAL_SAI_Transmit()
      (+) Receive an amount of data in blocking mode using HAL_SAI_Receive()

    *** Interrupt mode IO operation ***
    ===================================
    [..]
      (+) Send an amount of data in non-blocking mode using HAL_SAI_Transmit_IT()
      (+) At transmission end of transfer HAL_SAI_TxCpltCallback() is executed and user can
          add his own code by customization of function pointer HAL_SAI_TxCpltCallback()
      (+) Receive an amount of data in non-blocking mode using HAL_SAI_Receive_IT()
      (+) At reception end of transfer HAL_SAI_RxCpltCallback() is executed and user can
          add his own code by customization of function pointer HAL_SAI_RxCpltCallback()
      (+) In case of flag error, HAL_SAI_ErrorCallback() function is executed and user can
          add his own code by customization of function pointer HAL_SAI_ErrorCallback()

    *** DMA mode IO operation ***
    =============================
    [..]
      (+) Send an amount of data in non-blocking mode (DMA) using HAL_SAI_Transmit_DMA()
      (+) At transmission end of transfer HAL_SAI_TxCpltCallback() is executed and user can
          add his own code by customization of function pointer HAL_SAI_TxCpltCallback()
      (+) Receive an amount of data in non-blocking mode (DMA) using HAL_SAI_Receive_DMA()
      (+) At reception end of transfer HAL_SAI_RxCpltCallback() is executed and user can
          add his own code by customization of function pointer HAL_SAI_RxCpltCallback()
      (+) In case of flag error, HAL_SAI_ErrorCallback() function is executed and user can
          add his own code by customization of function pointer HAL_SAI_ErrorCallback()
      (+) Pause the DMA Transfer using HAL_SAI_DMAPause()
      (+) Resume the DMA Transfer using HAL_SAI_DMAResume()
      (+) Stop the DMA Transfer using HAL_SAI_DMAStop()

    *** SAI HAL driver additional function list ***
    ===============================================
    [..]
      Below the list the others API available SAI HAL driver :

      (+) HAL_SAI_EnableTxMuteMode(): Enable the mute in tx mode
      (+) HAL_SAI_DisableTxMuteMode(): Disable the mute in tx mode
      (+) HAL_SAI_EnableRxMuteMode(): Enable the mute in Rx mode
      (+) HAL_SAI_DisableRxMuteMode(): Disable the mute in Rx mode
      (+) HAL_SAI_FlushRxFifo(): Flush the rx fifo.
      (+) HAL_SAI_Abort(): Abort the current transfer

    *** SAI HAL driver macros list ***
    ==================================
    [..]
      Below the list of most used macros in SAI HAL driver :

      (+) __HAL_SAI_ENABLE(): Enable the SAI peripheral
      (+) __HAL_SAI_DISABLE(): Disable the SAI peripheral
      (+) __HAL_SAI_ENABLE_IT(): Enable the specified SAI interrupts
      (+) __HAL_SAI_DISABLE_IT(): Disable the specified SAI interrupts
      (+) __HAL_SAI_GET_IT_SOURCE(): Check if the specified SAI interrupt source is
          enabled or disabled
      (+) __HAL_SAI_GET_FLAG(): Check whether the specified SAI flag is set or not

    *** Callback registration ***
    =============================
    [..]
    The compilation define USE_HAL_SAI_REGISTER_CALLBACKS when set to 1
    allows the user to configure dynamically the driver callbacks.
    Use functions HAL_SAI_RegisterCallback() to register a user callback.

    [..]
    Function HAL_SAI_RegisterCallback() allows to register following callbacks:
      (+) RxCpltCallback     : SAI receive complete.
      (+) RxHalfCpltCallback : SAI receive half complete.
      (+) TxCpltCallback     : SAI transmit complete.
      (+) TxHalfCpltCallback : SAI transmit half complete.
      (+) ErrorCallback      : SAI error.
      (+) MspInitCallback    : SAI MspInit.
      (+) MspDeInitCallback  : SAI MspDeInit.
    [..]
    This function takes as parameters the HAL peripheral handle, the callback ID
    and a pointer to the user callback function.

    [..]
    Use function HAL_SAI_UnRegisterCallback() to reset a callback to the default
    weak (surcharged) function.
    HAL_SAI_UnRegisterCallback() takes as parameters the HAL peripheral handle,
    and the callback ID.
    [..]
    This function allows to reset following callbacks:
      (+) RxCpltCallback     : SAI receive complete.
      (+) RxHalfCpltCallback : SAI receive half complete.
      (+) TxCpltCallback     : SAI transmit complete.
      (+) TxHalfCpltCallback : SAI transmit half complete.
      (+) ErrorCallback      : SAI error.
      (+) MspInitCallback    : SAI MspInit.
      (+) MspDeInitCallback  : SAI MspDeInit.

    [..]
    By default, after the HAL_SAI_Init and if the state is HAL_SAI_STATE_RESET
    all callbacks are reset to the corresponding legacy weak (surcharged) functions:
    examples HAL_SAI_RxCpltCallback(), HAL_SAI_ErrorCallback().
    Exception done for MspInit and MspDeInit callbacks that are respectively
    reset to the legacy weak (surcharged) functions in the HAL_SAI_Init
    and HAL_SAI_DeInit only when these callbacks are null (not registered beforehand).
    If not, MspInit or MspDeInit are not null, the HAL_SAI_Init and HAL_SAI_DeInit
    keep and use the user MspInit/MspDeInit callbacks (registered beforehand).

    [..]
    Callbacks can be registered/unregistered in READY state only.
    Exception done for MspInit/MspDeInit callbacks that can be registered/unregistered
    in READY or RESET state, thus registered (user) MspInit/DeInit callbacks can be used
    during the Init/DeInit.
    In that case first register the MspInit/MspDeInit user callbacks
    using HAL_SAI_RegisterCallback before calling HAL_SAI_DeInit
    or HAL_SAI_Init function.

    [..]
    When the compilation define USE_HAL_SAI_REGISTER_CALLBACKS is set to 0 or
    not defined, the callback registering feature is not available
    and weak (surcharged) callbacks are used.

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

/** @defgroup SAI SAI
  * @brief SAI HAL module driver
  * @{
  */

#ifdef HAL_SAI_MODULE_ENABLED                           /* 条件编译 */

#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx) ||\
    defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx) || defined(STM32F413xx) ||\
    defined(STM32F423xx)                                /* 源代码 */

/** @defgroup SAI_Private_Typedefs  SAI Private Typedefs
  * @{
  */
typedef enum                                            /* 类型定义 */
{                                                       /* 代码块开始 */
  SAI_MODE_DMA,                                         /* 列表续行 */
  SAI_MODE_IT                                           /* 源代码 */
} SAI_ModeTypedef;                                      /* 代码块结束 */
/**
  * @}
  */

/* Private define ------------------------------------------------------------*/

/** @defgroup SAI_Private_Constants  SAI Private Constants
  * @{
  */
#define SAI_DEFAULT_TIMEOUT   4U /* 4ms */
#define SAI_LONG_TIMEOUT      1000U /* 1s */
/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/** @defgroup SAI_Private_Functions  SAI Private Functions
  * @{
  */
static void SAI_FillFifo(SAI_HandleTypeDef *hsai);      /* 静态函数 */
static uint32_t SAI_InterruptFlag(SAI_HandleTypeDef *hsai, uint32_t mode);  /* 静态变量/函数 */
static HAL_StatusTypeDef SAI_InitI2S(SAI_HandleTypeDef *hsai, uint32_t protocol, uint32_t datasize, uint32_t nbslot);  /* 静态变量/函数 */
static HAL_StatusTypeDef SAI_InitPCM(SAI_HandleTypeDef *hsai, uint32_t protocol, uint32_t datasize, uint32_t nbslot);  /* 静态变量/函数 */

static HAL_StatusTypeDef SAI_Disable(SAI_HandleTypeDef *hsai);  /* 静态变量/函数 */
static void SAI_Transmit_IT8Bit(SAI_HandleTypeDef *hsai);  /* 静态函数 */
static void SAI_Transmit_IT16Bit(SAI_HandleTypeDef *hsai);  /* 静态函数 */
static void SAI_Transmit_IT32Bit(SAI_HandleTypeDef *hsai);  /* 静态函数 */
static void SAI_Receive_IT8Bit(SAI_HandleTypeDef *hsai);  /* 静态函数 */
static void SAI_Receive_IT16Bit(SAI_HandleTypeDef *hsai);  /* 静态函数 */
static void SAI_Receive_IT32Bit(SAI_HandleTypeDef *hsai);  /* 静态函数 */

static void SAI_DMATxCplt(DMA_HandleTypeDef *hdma);     /* 静态函数 */
static void SAI_DMATxHalfCplt(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void SAI_DMARxCplt(DMA_HandleTypeDef *hdma);     /* 静态函数 */
static void SAI_DMARxHalfCplt(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void SAI_DMAError(DMA_HandleTypeDef *hdma);      /* 静态函数 */
static void SAI_DMAAbort(DMA_HandleTypeDef *hdma);      /* 静态函数 */
/**
  * @}
  */

/* Exported functions ---------------------------------------------------------*/
/** @defgroup SAI_Exported_Functions SAI Exported Functions
  * @{
  */

/** @defgroup SAI_Exported_Functions_Group1 Initialization and de-initialization functions
  * @brief    Initialization and Configuration functions
  *
@verbatim
 ===============================================================================
            ##### Initialization and de-initialization functions #####
 ===============================================================================
  [..]  This subsection provides a set of functions allowing to initialize and
        de-initialize the SAIx peripheral:

      (+) User must implement HAL_SAI_MspInit() function in which he configures
          all related peripherals resources (CLOCK, GPIO, DMA, IT and NVIC ).

      (+) Call the function HAL_SAI_Init() to configure the selected device with
          the selected configuration:
        (++) Mode (Master/slave TX/RX)
        (++) Protocol
        (++) Data Size
        (++) MCLK Output
        (++) Audio frequency
        (++) FIFO Threshold
        (++) Frame Config
        (++) Slot Config

      (+) Call the function HAL_SAI_DeInit() to restore the default configuration
          of the selected SAI peripheral.

@endverbatim
  * @{
  */

/**
  * @brief  Initialize the structure FrameInit, SlotInit and the low part of
  *         Init according to the specified parameters and call the function
  *         HAL_SAI_Init to initialize the SAI block.
  * @param  hsai pointer to a SAI_HandleTypeDef structure that contains
  *               the configuration information for SAI module.
  * @param  protocol one of the supported protocol @ref SAI_Protocol
  * @param  datasize one of the supported datasize @ref SAI_Protocol_DataSize
  *                   the configuration information for SAI module.
  * @param  nbslot Number of slot.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SAI_InitProtocol(SAI_HandleTypeDef *hsai, uint32_t protocol, uint32_t datasize, uint32_t nbslot)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Check the parameters */
  assert_param(IS_SAI_SUPPORTED_PROTOCOL(protocol));    /* 执行语句 */
  assert_param(IS_SAI_PROTOCOL_DATASIZE(datasize));     /* 执行语句 */

  switch (protocol)                                     /* switch分支 */
  {                                                     /* 代码块开始 */
    case SAI_I2S_STANDARD :                             /* case标签 */
    case SAI_I2S_MSBJUSTIFIED :                         /* case标签 */
    case SAI_I2S_LSBJUSTIFIED :                         /* case标签 */
      status = SAI_InitI2S(hsai, protocol, datasize, nbslot);  /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case SAI_PCM_LONG :                                 /* case标签 */
    case SAI_PCM_SHORT :                                /* case标签 */
      status = SAI_InitPCM(hsai, protocol, datasize, nbslot);  /* 赋值或初始化 */
      break;                                            /* 跳出 */
    default :                                           /* default分支 */
      status = HAL_ERROR;                               /* 赋值或初始化 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  if (status == HAL_OK)                                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    status = HAL_SAI_Init(hsai);                        /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initialize the SAI according to the specified parameters.
  *         in the SAI_InitTypeDef structure and initialize the associated handle.
  * @param  hsai pointer to a SAI_HandleTypeDef structure that contains
  *               the configuration information for SAI module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SAI_Init(SAI_HandleTypeDef *hsai)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpregisterGCR = 0U;                         /* 32位无符号变量/函数 */

  /* This variable used to store the SAI_CK_x (value in Hz) */
  uint32_t freq = 0U;                                   /* 32位无符号变量/函数 */

  /* This variable is used to compute CKSTR bits of SAI CR1 according to
     ClockStrobing and AudioMode fields */
  uint32_t ckstr_bits = 0U;                             /* 32位无符号变量/函数 */
  uint32_t syncen_bits = 0U;                            /* 32位无符号变量/函数 */

  /* Check the SAI handle allocation */
  if (hsai == NULL)                                     /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* check the instance */
  assert_param(IS_SAI_ALL_INSTANCE(hsai->Instance));    /* 执行语句 */

  /* Check the SAI Block parameters */
  assert_param(IS_SAI_AUDIO_FREQUENCY(hsai->Init.AudioFrequency));  /* 执行语句 */
  assert_param(IS_SAI_BLOCK_PROTOCOL(hsai->Init.Protocol));  /* 执行语句 */
  assert_param(IS_SAI_BLOCK_MODE(hsai->Init.AudioMode));  /* 执行语句 */
  assert_param(IS_SAI_BLOCK_SYNCEXT(hsai->Init.SynchroExt));  /* 执行语句 */
  assert_param(IS_SAI_BLOCK_DATASIZE(hsai->Init.DataSize));  /* 执行语句 */
  assert_param(IS_SAI_BLOCK_FIRST_BIT(hsai->Init.FirstBit));  /* 执行语句 */
  assert_param(IS_SAI_BLOCK_CLOCK_STROBING(hsai->Init.ClockStrobing));  /* 执行语句 */
  assert_param(IS_SAI_BLOCK_SYNCHRO(hsai->Init.Synchro));  /* 执行语句 */
  assert_param(IS_SAI_BLOCK_OUTPUT_DRIVE(hsai->Init.OutputDrive));  /* 执行语句 */
  assert_param(IS_SAI_BLOCK_NODIVIDER(hsai->Init.NoDivider));  /* 执行语句 */
  assert_param(IS_SAI_BLOCK_FIFO_THRESHOLD(hsai->Init.FIFOThreshold));  /* 执行语句 */
  assert_param(IS_SAI_MONO_STEREO_MODE(hsai->Init.MonoStereoMode));  /* 执行语句 */
  assert_param(IS_SAI_BLOCK_COMPANDING_MODE(hsai->Init.CompandingMode));  /* 执行语句 */
  assert_param(IS_SAI_BLOCK_TRISTATE_MANAGEMENT(hsai->Init.TriState));  /* 执行语句 */

  /* Check the SAI Block Frame parameters */
  assert_param(IS_SAI_BLOCK_FRAME_LENGTH(hsai->FrameInit.FrameLength));  /* 执行语句 */
  assert_param(IS_SAI_BLOCK_ACTIVE_FRAME(hsai->FrameInit.ActiveFrameLength));  /* 执行语句 */
  assert_param(IS_SAI_BLOCK_FS_DEFINITION(hsai->FrameInit.FSDefinition));  /* 执行语句 */
  assert_param(IS_SAI_BLOCK_FS_POLARITY(hsai->FrameInit.FSPolarity));  /* 执行语句 */
  assert_param(IS_SAI_BLOCK_FS_OFFSET(hsai->FrameInit.FSOffset));  /* 执行语句 */

  /* Check the SAI Block Slot parameters */
  assert_param(IS_SAI_BLOCK_FIRSTBIT_OFFSET(hsai->SlotInit.FirstBitOffset));  /* 执行语句 */
  assert_param(IS_SAI_BLOCK_SLOT_SIZE(hsai->SlotInit.SlotSize));  /* 执行语句 */
  assert_param(IS_SAI_BLOCK_SLOT_NUMBER(hsai->SlotInit.SlotNumber));  /* 执行语句 */
  assert_param(IS_SAI_SLOT_ACTIVE(hsai->SlotInit.SlotActive));  /* 执行语句 */

  if (hsai->State == HAL_SAI_STATE_RESET)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Allocate lock resource and initialize it */
    hsai->Lock = HAL_UNLOCKED;                          /* 赋值或初始化 */

#if (USE_HAL_SAI_REGISTER_CALLBACKS == 1)               /* 条件编译 */
    /* Reset callback pointers to the weak predefined callbacks */
    hsai->RxCpltCallback     = HAL_SAI_RxCpltCallback;  /* 赋值或初始化 */
    hsai->RxHalfCpltCallback = HAL_SAI_RxHalfCpltCallback;  /* 赋值或初始化 */
    hsai->TxCpltCallback     = HAL_SAI_TxCpltCallback;  /* 赋值或初始化 */
    hsai->TxHalfCpltCallback = HAL_SAI_TxHalfCpltCallback;  /* 赋值或初始化 */
    hsai->ErrorCallback      = HAL_SAI_ErrorCallback;   /* 赋值或初始化 */

    /* Init the low level hardware : GPIO, CLOCK, NVIC and DMA */
    if (hsai->MspInitCallback == NULL)                  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hsai->MspInitCallback = HAL_SAI_MspInit;          /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    hsai->MspInitCallback(hsai);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /* Init the low level hardware : GPIO, CLOCK, NVIC and DMA */
    HAL_SAI_MspInit(hsai);                              /* HAL库函数调用 */
#endif /* USE_HAL_SAI_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */

  hsai->State = HAL_SAI_STATE_BUSY;                     /* 赋值或初始化 */

  /* Disable the selected SAI peripheral */
  SAI_Disable(hsai);                                    /* 执行语句 */

  /* SAI Block Synchro Configuration -----------------------------------------*/
  SAI_BlockSynchroConfig(hsai);                         /* 执行语句 */

  /* Configure Master Clock using the following formula :
     MCLK_x = SAI_CK_x / (MCKDIV[3:0] * 2) with MCLK_x = 256 * FS
     FS = SAI_CK_x / (MCKDIV[3:0] * 2) * 256
     MCKDIV[3:0] = SAI_CK_x / FS * 512 */
  if (hsai->Init.AudioFrequency != SAI_AUDIO_FREQUENCY_MCKDIV)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Get SAI clock source based on Source clock selection from RCC */
    freq = SAI_GetInputClock(hsai);                     /* 赋值或初始化 */

    /* (saiclocksource x 10) to keep Significant digits */
    tmpregisterGCR = (((freq * 10U) / ((hsai->Init.AudioFrequency) * 512U)));  /* 赋值或初始化 */

    hsai->Init.Mckdiv = tmpregisterGCR / 10U;           /* 赋值或初始化 */

    /* Round result to the nearest integer */
    if ((tmpregisterGCR % 10U) > 8U)                    /* 条件判断 */
    {                                                   /* 代码块开始 */
      hsai->Init.Mckdiv += 1U;                          /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* For SPDIF protocol, SAI shall provide a bit clock twice faster the symbol-rate */
    if (hsai->Init.Protocol == SAI_SPDIF_PROTOCOL)      /* 条件判断 */
    {                                                   /* 代码块开始 */
      hsai->Init.Mckdiv = hsai->Init.Mckdiv >> 1;       /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Check the SAI Block master clock divider parameter */
  assert_param(IS_SAI_BLOCK_MASTER_DIVIDER(hsai->Init.Mckdiv));  /* 执行语句 */

  /* Compute CKSTR bits of SAI CR1 according to ClockStrobing and AudioMode */
  if ((hsai->Init.AudioMode == SAI_MODEMASTER_TX) || (hsai->Init.AudioMode == SAI_MODESLAVE_TX))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    ckstr_bits = (hsai->Init.ClockStrobing == SAI_CLOCKSTROBING_RISINGEDGE) ? 0U : SAI_xCR1_CKSTR;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    ckstr_bits = (hsai->Init.ClockStrobing == SAI_CLOCKSTROBING_RISINGEDGE) ? SAI_xCR1_CKSTR : 0U;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* SAI Block Configuration -------------------------------------------------*/
  switch (hsai->Init.Synchro)                           /* switch分支 */
  {                                                     /* 代码块开始 */
    case SAI_ASYNCHRONOUS :                             /* case标签 */
    {                                                   /* 代码块开始 */
      syncen_bits = 0U;                                 /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    break;                                              /* 跳出 */
    case SAI_SYNCHRONOUS :                              /* case标签 */
    {                                                   /* 代码块开始 */
      syncen_bits = SAI_xCR1_SYNCEN_0;                  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    break;                                              /* 跳出 */
    case SAI_SYNCHRONOUS_EXT_SAI1 :                     /* case标签 */
    case SAI_SYNCHRONOUS_EXT_SAI2 :                     /* case标签 */
    {                                                   /* 代码块开始 */
      syncen_bits = SAI_xCR1_SYNCEN_1;                  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    break;                                              /* 跳出 */
    default:                                            /* default分支 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  /* SAI CR1 Configuration */
  hsai->Instance->CR1 &= ~(SAI_xCR1_MODE | SAI_xCR1_PRTCFG |  SAI_xCR1_DS |      \
                           SAI_xCR1_LSBFIRST | SAI_xCR1_CKSTR | SAI_xCR1_SYNCEN | \
                           SAI_xCR1_MONO | SAI_xCR1_OUTDRIV  | SAI_xCR1_DMAEN |  \
                           SAI_xCR1_NODIV | SAI_xCR1_MCKDIV);  /* 执行语句 */

  hsai->Instance->CR1 |= (hsai->Init.AudioMode | hsai->Init.Protocol |           \
                          hsai->Init.DataSize | hsai->Init.FirstBit  |           \
                          ckstr_bits | syncen_bits |                             \
                          hsai->Init.MonoStereoMode | hsai->Init.OutputDrive |   \
                          hsai->Init.NoDivider | (hsai->Init.Mckdiv << 20U));  /* 执行语句 */

  /* SAI CR2 Configuration */
  hsai->Instance->CR2 &= ~(SAI_xCR2_FTH | SAI_xCR2_FFLUSH | SAI_xCR2_COMP | SAI_xCR2_CPL);  /* 赋值或初始化 */
  hsai->Instance->CR2 |= (hsai->Init.FIFOThreshold | hsai->Init.CompandingMode | hsai->Init.TriState);  /* 赋值或初始化 */

  /* SAI Frame Configuration -----------------------------------------*/
  hsai->Instance->FRCR &= (~(SAI_xFRCR_FRL | SAI_xFRCR_FSALL | SAI_xFRCR_FSDEF | \
                             SAI_xFRCR_FSPOL | SAI_xFRCR_FSOFF));  /* 执行语句 */
  hsai->Instance->FRCR |= ((hsai->FrameInit.FrameLength - 1U) |  /* 源代码 */
                           hsai->FrameInit.FSOffset     |  /* 源代码 */
                           hsai->FrameInit.FSDefinition |  /* 源代码 */
                           hsai->FrameInit.FSPolarity   |  /* 源代码 */
                           ((hsai->FrameInit.ActiveFrameLength - 1U) << 8U));  /* 执行语句 */

  /* SAI Block_x SLOT Configuration ------------------------------------------*/
  /* This register has no meaning in AC 97 and SPDIF audio protocol */
  hsai->Instance->SLOTR &= ~(SAI_xSLOTR_FBOFF | SAI_xSLOTR_SLOTSZ | \
                             SAI_xSLOTR_NBSLOT | SAI_xSLOTR_SLOTEN);  /* 执行语句 */

  hsai->Instance->SLOTR |=  hsai->SlotInit.FirstBitOffset |  hsai->SlotInit.SlotSize  | \
                            (hsai->SlotInit.SlotActive << 16U) | ((hsai->SlotInit.SlotNumber - 1U) <<  8U);  /* 执行语句 */

  /* Initialize the error code */
  hsai->ErrorCode = HAL_SAI_ERROR_NONE;                 /* 赋值或初始化 */

  /* Initialize the SAI state */
  hsai->State = HAL_SAI_STATE_READY;                    /* 赋值或初始化 */

  /* Release Lock */
  __HAL_UNLOCK(hsai);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  DeInitialize the SAI peripheral.
  * @param  hsai pointer to a SAI_HandleTypeDef structure that contains
  *               the configuration information for SAI module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SAI_DeInit(SAI_HandleTypeDef *hsai)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the SAI handle allocation */
  if (hsai == NULL)                                     /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  hsai->State = HAL_SAI_STATE_BUSY;                     /* 赋值或初始化 */

  /* Disabled All interrupt and clear all the flag */
  hsai->Instance->IMR = 0U;                             /* 赋值或初始化 */
  hsai->Instance->CLRFR = 0xFFFFFFFFU;                  /* 赋值或初始化 */

  /* Disable the SAI */
  SAI_Disable(hsai);                                    /* 执行语句 */

  /* Flush the fifo */
  SET_BIT(hsai->Instance->CR2, SAI_xCR2_FFLUSH);        /* 执行语句 */

  /* DeInit the low level hardware: GPIO, CLOCK, NVIC and DMA */
#if (USE_HAL_SAI_REGISTER_CALLBACKS == 1)               /* 条件编译 */
  if (hsai->MspDeInitCallback == NULL)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hsai->MspDeInitCallback = HAL_SAI_MspDeInit;        /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  hsai->MspDeInitCallback(hsai);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_SAI_MspDeInit(hsai);                              /* HAL库函数调用 */
#endif /* USE_HAL_SAI_REGISTER_CALLBACKS */

  /* Initialize the error code */
  hsai->ErrorCode = HAL_SAI_ERROR_NONE;                 /* 赋值或初始化 */

  /* Initialize the SAI state */
  hsai->State = HAL_SAI_STATE_RESET;                    /* 赋值或初始化 */

  /* Release Lock */
  __HAL_UNLOCK(hsai);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief Initialize the SAI MSP.
  * @param  hsai pointer to a SAI_HandleTypeDef structure that contains
  *               the configuration information for SAI module.
  * @retval None
  */
__weak void HAL_SAI_MspInit(SAI_HandleTypeDef *hsai)    /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsai);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SAI_MspInit could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief DeInitialize the SAI MSP.
  * @param  hsai pointer to a SAI_HandleTypeDef structure that contains
  *               the configuration information for SAI module.
  * @retval None
  */
__weak void HAL_SAI_MspDeInit(SAI_HandleTypeDef *hsai)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsai);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SAI_MspDeInit could be implemented in the user file
   */
}                                                       /* 代码块结束 */

#if (USE_HAL_SAI_REGISTER_CALLBACKS == 1)               /* 条件编译 */
/**
  * @brief  Register a user SAI callback
  *         to be used instead of the weak predefined callback.
  * @param  hsai SAI handle.
  * @param  CallbackID ID of the callback to be registered.
  *         This parameter can be one of the following values:
  *           @arg @ref HAL_SAI_RX_COMPLETE_CB_ID receive complete callback ID.
  *           @arg @ref HAL_SAI_RX_HALFCOMPLETE_CB_ID receive half complete callback ID.
  *           @arg @ref HAL_SAI_TX_COMPLETE_CB_ID transmit complete callback ID.
  *           @arg @ref HAL_SAI_TX_HALFCOMPLETE_CB_ID transmit half complete callback ID.
  *           @arg @ref HAL_SAI_ERROR_CB_ID error callback ID.
  *           @arg @ref HAL_SAI_MSPINIT_CB_ID MSP init callback ID.
  *           @arg @ref HAL_SAI_MSPDEINIT_CB_ID MSP de-init callback ID.
  * @param  pCallback pointer to the callback function.
  * @retval HAL status.
  */
HAL_StatusTypeDef HAL_SAI_RegisterCallback(SAI_HandleTypeDef        *hsai,  /* HAL状态类型函数 */
                                           HAL_SAI_CallbackIDTypeDef CallbackID,  /* HAL库函数调用 */
                                           pSAI_CallbackTypeDef      pCallback)  /* 源代码 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  if (pCallback == NULL)                                /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* update the error code */
    hsai->ErrorCode |= HAL_SAI_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
    /* update return status */
    status = HAL_ERROR;                                 /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    if (HAL_SAI_STATE_READY == hsai->State)             /* 条件判断 */
    {                                                   /* 代码块开始 */
      switch (CallbackID)                               /* switch分支 */
      {                                                 /* 代码块开始 */
        case HAL_SAI_RX_COMPLETE_CB_ID :                /* case标签 */
          hsai->RxCpltCallback = pCallback;             /* 赋值或初始化 */
          break;                                        /* 跳出 */
        case HAL_SAI_RX_HALFCOMPLETE_CB_ID :            /* case标签 */
          hsai->RxHalfCpltCallback = pCallback;         /* 赋值或初始化 */
          break;                                        /* 跳出 */
        case HAL_SAI_TX_COMPLETE_CB_ID :                /* case标签 */
          hsai->TxCpltCallback = pCallback;             /* 赋值或初始化 */
          break;                                        /* 跳出 */
        case HAL_SAI_TX_HALFCOMPLETE_CB_ID :            /* case标签 */
          hsai->TxHalfCpltCallback = pCallback;         /* 赋值或初始化 */
          break;                                        /* 跳出 */
        case HAL_SAI_ERROR_CB_ID :                      /* case标签 */
          hsai->ErrorCallback = pCallback;              /* 赋值或初始化 */
          break;                                        /* 跳出 */
        case HAL_SAI_MSPINIT_CB_ID :                    /* case标签 */
          hsai->MspInitCallback = pCallback;            /* 赋值或初始化 */
          break;                                        /* 跳出 */
        case HAL_SAI_MSPDEINIT_CB_ID :                  /* case标签 */
          hsai->MspDeInitCallback = pCallback;          /* 赋值或初始化 */
          break;                                        /* 跳出 */
        default :                                       /* default分支 */
          /* update the error code */
          hsai->ErrorCode |= HAL_SAI_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
          /* update return status */
          status = HAL_ERROR;                           /* 赋值或初始化 */
          break;                                        /* 跳出 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else if (HAL_SAI_STATE_RESET == hsai->State)        /* 否则如果 */
    {                                                   /* 代码块开始 */
      switch (CallbackID)                               /* switch分支 */
      {                                                 /* 代码块开始 */
        case HAL_SAI_MSPINIT_CB_ID :                    /* case标签 */
          hsai->MspInitCallback = pCallback;            /* 赋值或初始化 */
          break;                                        /* 跳出 */
        case HAL_SAI_MSPDEINIT_CB_ID :                  /* case标签 */
          hsai->MspDeInitCallback = pCallback;          /* 赋值或初始化 */
          break;                                        /* 跳出 */
        default :                                       /* default分支 */
          /* update the error code */
          hsai->ErrorCode |= HAL_SAI_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
          /* update return status */
          status = HAL_ERROR;                           /* 赋值或初始化 */
          break;                                        /* 跳出 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* update the error code */
      hsai->ErrorCode |= HAL_SAI_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
      /* update return status */
      status = HAL_ERROR;                               /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Unregister a user SAI callback.
  *         SAI callback is redirected to the weak predefined callback.
  * @param  hsai SAI handle.
  * @param  CallbackID ID of the callback to be unregistered.
  *         This parameter can be one of the following values:
  *           @arg @ref HAL_SAI_RX_COMPLETE_CB_ID receive complete callback ID.
  *           @arg @ref HAL_SAI_RX_HALFCOMPLETE_CB_ID receive half complete callback ID.
  *           @arg @ref HAL_SAI_TX_COMPLETE_CB_ID transmit complete callback ID.
  *           @arg @ref HAL_SAI_TX_HALFCOMPLETE_CB_ID transmit half complete callback ID.
  *           @arg @ref HAL_SAI_ERROR_CB_ID error callback ID.
  *           @arg @ref HAL_SAI_MSPINIT_CB_ID MSP init callback ID.
  *           @arg @ref HAL_SAI_MSPDEINIT_CB_ID MSP de-init callback ID.
  * @retval HAL status.
  */
HAL_StatusTypeDef HAL_SAI_UnRegisterCallback(SAI_HandleTypeDef        *hsai,  /* HAL状态类型函数 */
                                             HAL_SAI_CallbackIDTypeDef CallbackID)  /* HAL库函数调用 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  if (HAL_SAI_STATE_READY == hsai->State)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_SAI_RX_COMPLETE_CB_ID :                  /* case标签 */
        hsai->RxCpltCallback = HAL_SAI_RxCpltCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */
      case HAL_SAI_RX_HALFCOMPLETE_CB_ID :              /* case标签 */
        hsai->RxHalfCpltCallback = HAL_SAI_RxHalfCpltCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */
      case HAL_SAI_TX_COMPLETE_CB_ID :                  /* case标签 */
        hsai->TxCpltCallback = HAL_SAI_TxCpltCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */
      case HAL_SAI_TX_HALFCOMPLETE_CB_ID :              /* case标签 */
        hsai->TxHalfCpltCallback = HAL_SAI_TxHalfCpltCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */
      case HAL_SAI_ERROR_CB_ID :                        /* case标签 */
        hsai->ErrorCallback = HAL_SAI_ErrorCallback;    /* 赋值或初始化 */
        break;                                          /* 跳出 */
      case HAL_SAI_MSPINIT_CB_ID :                      /* case标签 */
        hsai->MspInitCallback = HAL_SAI_MspInit;        /* 赋值或初始化 */
        break;                                          /* 跳出 */
      case HAL_SAI_MSPDEINIT_CB_ID :                    /* case标签 */
        hsai->MspDeInitCallback = HAL_SAI_MspDeInit;    /* 赋值或初始化 */
        break;                                          /* 跳出 */
      default :                                         /* default分支 */
        /* update the error code */
        hsai->ErrorCode |= HAL_SAI_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
        /* update return status */
        status = HAL_ERROR;                             /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (HAL_SAI_STATE_RESET == hsai->State)          /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_SAI_MSPINIT_CB_ID :                      /* case标签 */
        hsai->MspInitCallback = HAL_SAI_MspInit;        /* 赋值或初始化 */
        break;                                          /* 跳出 */
      case HAL_SAI_MSPDEINIT_CB_ID :                    /* case标签 */
        hsai->MspDeInitCallback = HAL_SAI_MspDeInit;    /* 赋值或初始化 */
        break;                                          /* 跳出 */
      default :                                         /* default分支 */
        /* update the error code */
        hsai->ErrorCode |= HAL_SAI_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
        /* update return status */
        status = HAL_ERROR;                             /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* update the error code */
    hsai->ErrorCode |= HAL_SAI_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
    /* update return status */
    status = HAL_ERROR;                                 /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* USE_HAL_SAI_REGISTER_CALLBACKS */

/**
  * @}
  */

/** @defgroup SAI_Exported_Functions_Group2 IO operation functions
  * @brief    Data transfers functions
  *
@verbatim
  ==============================================================================
                      ##### IO operation functions #####
  ==============================================================================
  [..]
    This subsection provides a set of functions allowing to manage the SAI data
    transfers.

    (+) There are two modes of transfer:
      (++) Blocking mode : The communication is performed in the polling mode.
           The status of all data processing is returned by the same function
           after finishing transfer.
      (++) No-Blocking mode : The communication is performed using Interrupts
           or DMA. These functions return the status of the transfer startup.
           The end of the data processing will be indicated through the
           dedicated SAI IRQ when using Interrupt mode or the DMA IRQ when
           using DMA mode.

    (+) Blocking mode functions are :
      (++) HAL_SAI_Transmit()
      (++) HAL_SAI_Receive()

    (+) Non Blocking mode functions with Interrupt are :
      (++) HAL_SAI_Transmit_IT()
      (++) HAL_SAI_Receive_IT()

    (+) Non Blocking mode functions with DMA are :
      (++) HAL_SAI_Transmit_DMA()
      (++) HAL_SAI_Receive_DMA()

    (+) A set of Transfer Complete Callbacks are provided in non Blocking mode:
      (++) HAL_SAI_TxCpltCallback()
      (++) HAL_SAI_RxCpltCallback()
      (++) HAL_SAI_ErrorCallback()

@endverbatim
  * @{
  */

/**
  * @brief  Transmit an amount of data in blocking mode.
  * @param  hsai pointer to a SAI_HandleTypeDef structure that contains
  *               the configuration information for SAI module.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SAI_Transmit(SAI_HandleTypeDef *hsai, uint8_t *pData, uint16_t Size, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart = HAL_GetTick();                   /* 32位无符号变量/函数 */

  if ((pData == NULL) || (Size == 0))                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    return  HAL_ERROR;                                  /* 返回值 */
  }                                                     /* 代码块结束 */

  if (hsai->State == HAL_SAI_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Process Locked */
    __HAL_LOCK(hsai);                                   /* HAL辅助宏/函数 */

    hsai->XferSize = Size;                              /* 赋值或初始化 */
    hsai->XferCount = Size;                             /* 赋值或初始化 */
    hsai->pBuffPtr = pData;                             /* 赋值或初始化 */
    hsai->State = HAL_SAI_STATE_BUSY_TX;                /* 赋值或初始化 */
    hsai->ErrorCode = HAL_SAI_ERROR_NONE;               /* 赋值或初始化 */

    /* Check if the SAI is already enabled */
    if ((hsai->Instance->CR1 & SAI_xCR1_SAIEN) == RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* fill the fifo with data before to enabled the SAI */
      SAI_FillFifo(hsai);                               /* 执行语句 */
      /* Enable SAI peripheral */
      __HAL_SAI_ENABLE(hsai);                           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    while (hsai->XferCount > 0U)                        /* while循环 */
    {                                                   /* 代码块开始 */
      /* Write data if the FIFO is not full */
      if ((hsai->Instance->SR & SAI_xSR_FLVL) != SAI_FIFOSTATUS_FULL)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        if ((hsai->Init.DataSize == SAI_DATASIZE_8) && (hsai->Init.CompandingMode == SAI_NOCOMPANDING))  /* 条件判断 */
        {                                               /* 代码块开始 */
          hsai->Instance->DR = (*hsai->pBuffPtr++);     /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        else if (hsai->Init.DataSize <= SAI_DATASIZE_16)  /* 否则如果 */
        {                                               /* 代码块开始 */
          hsai->Instance->DR = *((uint16_t *)hsai->pBuffPtr);  /* 赋值或初始化 */
          hsai->pBuffPtr += 2U;                         /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          hsai->Instance->DR = *((uint32_t *)hsai->pBuffPtr);  /* 赋值或初始化 */
          hsai->pBuffPtr += 4U;                         /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        hsai->XferCount--;                              /* 执行语句 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Check for the Timeout */
        if ((Timeout != HAL_MAX_DELAY) && ((Timeout == 0U) || ((HAL_GetTick() - tickstart) > Timeout)))  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Update error code */
          hsai->ErrorCode |= HAL_SAI_ERROR_TIMEOUT;     /* 赋值或初始化 */

          /* Clear all the flags */
          hsai->Instance->CLRFR = 0xFFFFFFFFU;          /* 赋值或初始化 */

          /* Disable SAI peripheral */
          SAI_Disable(hsai);                            /* 执行语句 */

          /* Flush the fifo */
          SET_BIT(hsai->Instance->CR2, SAI_xCR2_FFLUSH);  /* 执行语句 */

          /* Change the SAI state */
          hsai->State = HAL_SAI_STATE_READY;            /* 赋值或初始化 */

          /* Process Unlocked */
          __HAL_UNLOCK(hsai);                           /* HAL辅助宏/函数 */

          return HAL_ERROR;                             /* 返回值 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    hsai->State = HAL_SAI_STATE_READY;                  /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hsai);                                 /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Receive an amount of data in blocking mode.
  * @param  hsai pointer to a SAI_HandleTypeDef structure that contains
  *               the configuration information for SAI module.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be received
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SAI_Receive(SAI_HandleTypeDef *hsai, uint8_t *pData, uint16_t Size, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart = HAL_GetTick();                   /* 32位无符号变量/函数 */

  if ((pData == NULL) || (Size == 0))                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    return  HAL_ERROR;                                  /* 返回值 */
  }                                                     /* 代码块结束 */

  if (hsai->State == HAL_SAI_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Process Locked */
    __HAL_LOCK(hsai);                                   /* HAL辅助宏/函数 */

    hsai->pBuffPtr = pData;                             /* 赋值或初始化 */
    hsai->XferSize = Size;                              /* 赋值或初始化 */
    hsai->XferCount = Size;                             /* 赋值或初始化 */
    hsai->State = HAL_SAI_STATE_BUSY_RX;                /* 赋值或初始化 */
    hsai->ErrorCode = HAL_SAI_ERROR_NONE;               /* 赋值或初始化 */

    /* Check if the SAI is already enabled */
    if ((hsai->Instance->CR1 & SAI_xCR1_SAIEN) == RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable SAI peripheral */
      __HAL_SAI_ENABLE(hsai);                           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Receive data */
    while (hsai->XferCount > 0U)                        /* while循环 */
    {                                                   /* 代码块开始 */
      if ((hsai->Instance->SR & SAI_xSR_FLVL) != SAI_FIFOSTATUS_EMPTY)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        if ((hsai->Init.DataSize == SAI_DATASIZE_8) && (hsai->Init.CompandingMode == SAI_NOCOMPANDING))  /* 条件判断 */
        {                                               /* 代码块开始 */
          (*hsai->pBuffPtr++) = hsai->Instance->DR;     /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        else if (hsai->Init.DataSize <= SAI_DATASIZE_16)  /* 否则如果 */
        {                                               /* 代码块开始 */
          *((uint16_t *)hsai->pBuffPtr) = hsai->Instance->DR;  /* 赋值或初始化 */
          hsai->pBuffPtr += 2U;                         /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          *((uint32_t *)hsai->pBuffPtr) = hsai->Instance->DR;  /* 赋值或初始化 */
          hsai->pBuffPtr += 4U;                         /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        hsai->XferCount--;                              /* 执行语句 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Check for the Timeout */
        if ((Timeout != HAL_MAX_DELAY) && ((Timeout == 0U) || ((HAL_GetTick() - tickstart) > Timeout)))  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Update error code */
          hsai->ErrorCode |= HAL_SAI_ERROR_TIMEOUT;     /* 赋值或初始化 */

          /* Clear all the flags */
          hsai->Instance->CLRFR = 0xFFFFFFFFU;          /* 赋值或初始化 */

          /* Disable SAI peripheral */
          SAI_Disable(hsai);                            /* 执行语句 */

          /* Flush the fifo */
          SET_BIT(hsai->Instance->CR2, SAI_xCR2_FFLUSH);  /* 执行语句 */

          /* Change the SAI state */
          hsai->State = HAL_SAI_STATE_READY;            /* 赋值或初始化 */

          /* Process Unlocked */
          __HAL_UNLOCK(hsai);                           /* HAL辅助宏/函数 */

          return HAL_ERROR;                             /* 返回值 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    hsai->State = HAL_SAI_STATE_READY;                  /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hsai);                                 /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Transmit an amount of data in non-blocking mode with Interrupt.
  * @param  hsai pointer to a SAI_HandleTypeDef structure that contains
  *               the configuration information for SAI module.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SAI_Transmit_IT(SAI_HandleTypeDef *hsai, uint8_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  if ((pData == NULL) || (Size == 0))                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    return  HAL_ERROR;                                  /* 返回值 */
  }                                                     /* 代码块结束 */

  if (hsai->State == HAL_SAI_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Process Locked */
    __HAL_LOCK(hsai);                                   /* HAL辅助宏/函数 */

    hsai->pBuffPtr = pData;                             /* 赋值或初始化 */
    hsai->XferSize = Size;                              /* 赋值或初始化 */
    hsai->XferCount = Size;                             /* 赋值或初始化 */
    hsai->ErrorCode = HAL_SAI_ERROR_NONE;               /* 赋值或初始化 */
    hsai->State = HAL_SAI_STATE_BUSY_TX;                /* 赋值或初始化 */

    if ((hsai->Init.DataSize == SAI_DATASIZE_8) && (hsai->Init.CompandingMode == SAI_NOCOMPANDING))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hsai->InterruptServiceRoutine = SAI_Transmit_IT8Bit;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if (hsai->Init.DataSize <= SAI_DATASIZE_16)    /* 否则如果 */
    {                                                   /* 代码块开始 */
      hsai->InterruptServiceRoutine = SAI_Transmit_IT16Bit;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hsai->InterruptServiceRoutine = SAI_Transmit_IT32Bit;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Fill the fifo before starting the communication */
    SAI_FillFifo(hsai);                                 /* 执行语句 */

    /* Enable FRQ and OVRUDR interrupts */
    __HAL_SAI_ENABLE_IT(hsai, SAI_InterruptFlag(hsai, SAI_MODE_IT));  /* HAL辅助宏/函数 */

    /* Check if the SAI is already enabled */
    if ((hsai->Instance->CR1 & SAI_xCR1_SAIEN) == RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable SAI peripheral */
      __HAL_SAI_ENABLE(hsai);                           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
    /* Process Unlocked */
    __HAL_UNLOCK(hsai);                                 /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Receive an amount of data in non-blocking mode with Interrupt.
  * @param  hsai pointer to a SAI_HandleTypeDef structure that contains
  *               the configuration information for SAI module.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be received
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SAI_Receive_IT(SAI_HandleTypeDef *hsai, uint8_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  if ((pData == NULL) || (Size == 0))                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    return  HAL_ERROR;                                  /* 返回值 */
  }                                                     /* 代码块结束 */

  if (hsai->State == HAL_SAI_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Process Locked */
    __HAL_LOCK(hsai);                                   /* HAL辅助宏/函数 */

    hsai->pBuffPtr = pData;                             /* 赋值或初始化 */
    hsai->XferSize = Size;                              /* 赋值或初始化 */
    hsai->XferCount = Size;                             /* 赋值或初始化 */
    hsai->ErrorCode = HAL_SAI_ERROR_NONE;               /* 赋值或初始化 */
    hsai->State = HAL_SAI_STATE_BUSY_RX;                /* 赋值或初始化 */

    if ((hsai->Init.DataSize == SAI_DATASIZE_8) && (hsai->Init.CompandingMode == SAI_NOCOMPANDING))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hsai->InterruptServiceRoutine = SAI_Receive_IT8Bit;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if (hsai->Init.DataSize <= SAI_DATASIZE_16)    /* 否则如果 */
    {                                                   /* 代码块开始 */
      hsai->InterruptServiceRoutine = SAI_Receive_IT16Bit;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hsai->InterruptServiceRoutine = SAI_Receive_IT32Bit;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Enable TXE and OVRUDR interrupts */
    __HAL_SAI_ENABLE_IT(hsai, SAI_InterruptFlag(hsai, SAI_MODE_IT));  /* HAL辅助宏/函数 */

    /* Check if the SAI is already enabled */
    if ((hsai->Instance->CR1 & SAI_xCR1_SAIEN) == RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable SAI peripheral */
      __HAL_SAI_ENABLE(hsai);                           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Process Unlocked */
    __HAL_UNLOCK(hsai);                                 /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief Pause the audio stream playing from the Media.
  * @param  hsai pointer to a SAI_HandleTypeDef structure that contains
  *               the configuration information for SAI module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SAI_DMAPause(SAI_HandleTypeDef *hsai)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Process Locked */
  __HAL_LOCK(hsai);                                     /* HAL辅助宏/函数 */

  /* Pause the audio file playing by disabling the SAI DMA requests */
  hsai->Instance->CR1 &= ~SAI_xCR1_DMAEN;               /* 赋值或初始化 */

  /* Process Unlocked */
  __HAL_UNLOCK(hsai);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief Resume the audio stream playing from the Media.
  * @param  hsai pointer to a SAI_HandleTypeDef structure that contains
  *               the configuration information for SAI module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SAI_DMAResume(SAI_HandleTypeDef *hsai)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Process Locked */
  __HAL_LOCK(hsai);                                     /* HAL辅助宏/函数 */

  /* Enable the SAI DMA requests */
  hsai->Instance->CR1 |= SAI_xCR1_DMAEN;                /* 赋值或初始化 */

  /* If the SAI peripheral is still not enabled, enable it */
  if ((hsai->Instance->CR1 & SAI_xCR1_SAIEN) == RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable SAI peripheral */
    __HAL_SAI_ENABLE(hsai);                             /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Process Unlocked */
  __HAL_UNLOCK(hsai);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief Stop the audio stream playing from the Media.
  * @param  hsai pointer to a SAI_HandleTypeDef structure that contains
  *               the configuration information for SAI module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SAI_DMAStop(SAI_HandleTypeDef *hsai)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Process Locked */
  __HAL_LOCK(hsai);                                     /* HAL辅助宏/函数 */

  /* Disable the SAI DMA request */
  hsai->Instance->CR1 &= ~SAI_xCR1_DMAEN;               /* 赋值或初始化 */

  /* Abort the SAI Tx DMA Stream */
  if ((hsai->hdmatx != NULL) && (hsai->State == HAL_SAI_STATE_BUSY_TX))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (HAL_DMA_Abort(hsai->hdmatx) != HAL_OK)          /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* If the DMA Tx errorCode is different from DMA No Transfer then return Error */
      if (hsai->hdmatx->ErrorCode != HAL_DMA_ERROR_NO_XFER)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        status = HAL_ERROR;                             /* 赋值或初始化 */
        hsai->ErrorCode |= HAL_SAI_ERROR_DMA;           /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Abort the SAI Rx DMA Stream */
  if ((hsai->hdmarx != NULL) && (hsai->State == HAL_SAI_STATE_BUSY_RX))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (HAL_DMA_Abort(hsai->hdmarx) != HAL_OK)          /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* If the DMA Rx errorCode is different from DMA No Transfer then return Error */
      if (hsai->hdmarx->ErrorCode != HAL_DMA_ERROR_NO_XFER)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        status = HAL_ERROR;                             /* 赋值或初始化 */
        hsai->ErrorCode |= HAL_SAI_ERROR_DMA;           /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Disable SAI peripheral */
  SAI_Disable(hsai);                                    /* 执行语句 */

  /* Flush the fifo */
  SET_BIT(hsai->Instance->CR2, SAI_xCR2_FFLUSH);        /* 执行语句 */

  /* Set hsai state to ready */
  hsai->State = HAL_SAI_STATE_READY;                    /* 赋值或初始化 */

  /* Process Unlocked */
  __HAL_UNLOCK(hsai);                                   /* HAL辅助宏/函数 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief Abort the current transfer and disable the SAI.
  * @param  hsai pointer to a SAI_HandleTypeDef structure that contains
  *               the configuration information for SAI module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SAI_Abort(SAI_HandleTypeDef *hsai)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Process Locked */
  __HAL_LOCK(hsai);                                     /* HAL辅助宏/函数 */

  /* Check SAI DMA is enabled or not */
  if ((hsai->Instance->CR1 & SAI_xCR1_DMAEN) == SAI_xCR1_DMAEN)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable the SAI DMA request */
    hsai->Instance->CR1 &= ~SAI_xCR1_DMAEN;             /* 赋值或初始化 */

    /* Abort the SAI Tx DMA Stream */
    if ((hsai->hdmatx != NULL) && (hsai->State == HAL_SAI_STATE_BUSY_TX))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (HAL_DMA_Abort(hsai->hdmatx) != HAL_OK)        /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* If the DMA Tx errorCode is different from DMA No Transfer then return Error */
        if (hsai->hdmatx->ErrorCode != HAL_DMA_ERROR_NO_XFER)  /* 条件判断 */
        {                                               /* 代码块开始 */
          status = HAL_ERROR;                           /* 赋值或初始化 */
          hsai->ErrorCode |= HAL_SAI_ERROR_DMA;         /* 赋值或初始化 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Abort the SAI Rx DMA Stream */
    if ((hsai->hdmarx != NULL) && (hsai->State == HAL_SAI_STATE_BUSY_RX))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (HAL_DMA_Abort(hsai->hdmarx) != HAL_OK)        /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* If the DMA Rx errorCode is different from DMA No Transfer then return Error */
        if (hsai->hdmarx->ErrorCode != HAL_DMA_ERROR_NO_XFER)  /* 条件判断 */
        {                                               /* 代码块开始 */
          status = HAL_ERROR;                           /* 赋值或初始化 */
          hsai->ErrorCode |= HAL_SAI_ERROR_DMA;         /* 赋值或初始化 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Disabled All interrupt and clear all the flag */
  hsai->Instance->IMR = 0U;                             /* 赋值或初始化 */
  hsai->Instance->CLRFR = 0xFFFFFFFFU;                  /* 赋值或初始化 */

  /* Disable SAI peripheral */
  SAI_Disable(hsai);                                    /* 执行语句 */

  /* Flush the fifo */
  SET_BIT(hsai->Instance->CR2, SAI_xCR2_FFLUSH);        /* 执行语句 */

  /* Set hsai state to ready */
  hsai->State = HAL_SAI_STATE_READY;                    /* 赋值或初始化 */

  /* Process Unlocked */
  __HAL_UNLOCK(hsai);                                   /* HAL辅助宏/函数 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Transmit an amount of data in non-blocking mode with DMA.
  * @param  hsai pointer to a SAI_HandleTypeDef structure that contains
  *               the configuration information for SAI module.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SAI_Transmit_DMA(SAI_HandleTypeDef *hsai, uint8_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart = HAL_GetTick();                   /* 32位无符号变量/函数 */

  if ((pData == NULL) || (Size == 0))                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    return  HAL_ERROR;                                  /* 返回值 */
  }                                                     /* 代码块结束 */

  if (hsai->State == HAL_SAI_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Process Locked */
    __HAL_LOCK(hsai);                                   /* HAL辅助宏/函数 */

    hsai->pBuffPtr = pData;                             /* 赋值或初始化 */
    hsai->XferSize = Size;                              /* 赋值或初始化 */
    hsai->XferCount = Size;                             /* 赋值或初始化 */
    hsai->ErrorCode = HAL_SAI_ERROR_NONE;               /* 赋值或初始化 */
    hsai->State = HAL_SAI_STATE_BUSY_TX;                /* 赋值或初始化 */

    /* Set the SAI Tx DMA Half transfer complete callback */
    hsai->hdmatx->XferHalfCpltCallback = SAI_DMATxHalfCplt;  /* 赋值或初始化 */

    /* Set the SAI TxDMA transfer complete callback */
    hsai->hdmatx->XferCpltCallback = SAI_DMATxCplt;     /* 赋值或初始化 */

    /* Set the DMA error callback */
    hsai->hdmatx->XferErrorCallback = SAI_DMAError;     /* 赋值或初始化 */

    /* Set the DMA Tx abort callback */
    hsai->hdmatx->XferAbortCallback = NULL;             /* 赋值或初始化 */

    /* Enable the Tx DMA Stream */
    if (HAL_DMA_Start_IT(hsai->hdmatx, (uint32_t)hsai->pBuffPtr, (uint32_t)&hsai->Instance->DR, hsai->XferSize) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      __HAL_UNLOCK(hsai);                               /* HAL辅助宏/函数 */
      return  HAL_ERROR;                                /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Enable the interrupts for error handling */
    __HAL_SAI_ENABLE_IT(hsai, SAI_InterruptFlag(hsai, SAI_MODE_DMA));  /* HAL辅助宏/函数 */

    /* Enable SAI Tx DMA Request */
    hsai->Instance->CR1 |= SAI_xCR1_DMAEN;              /* 赋值或初始化 */

    /* Wait until FIFO is not empty */
    while ((hsai->Instance->SR & SAI_xSR_FLVL) == SAI_FIFOSTATUS_EMPTY)  /* while循环 */
    {                                                   /* 代码块开始 */
      /* Check for the Timeout */
      if ((HAL_GetTick() - tickstart) > SAI_LONG_TIMEOUT)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Update error code */
        hsai->ErrorCode |= HAL_SAI_ERROR_TIMEOUT;       /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hsai);                             /* HAL辅助宏/函数 */

        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Check if the SAI is already enabled */
    if ((hsai->Instance->CR1 & SAI_xCR1_SAIEN) == 0U)   /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable SAI peripheral */
      __HAL_SAI_ENABLE(hsai);                           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Process Unlocked */
    __HAL_UNLOCK(hsai);                                 /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Receive an amount of data in non-blocking mode with DMA.
  * @param  hsai pointer to a SAI_HandleTypeDef structure that contains
  *               the configuration information for SAI module.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be received
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SAI_Receive_DMA(SAI_HandleTypeDef *hsai, uint8_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  if ((pData == NULL) || (Size == 0))                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    return  HAL_ERROR;                                  /* 返回值 */
  }                                                     /* 代码块结束 */

  if (hsai->State == HAL_SAI_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Process Locked */
    __HAL_LOCK(hsai);                                   /* HAL辅助宏/函数 */

    hsai->pBuffPtr = pData;                             /* 赋值或初始化 */
    hsai->XferSize = Size;                              /* 赋值或初始化 */
    hsai->XferCount = Size;                             /* 赋值或初始化 */
    hsai->ErrorCode = HAL_SAI_ERROR_NONE;               /* 赋值或初始化 */
    hsai->State = HAL_SAI_STATE_BUSY_RX;                /* 赋值或初始化 */

    /* Set the SAI Rx DMA Half transfer complete callback */
    hsai->hdmarx->XferHalfCpltCallback = SAI_DMARxHalfCplt;  /* 赋值或初始化 */

    /* Set the SAI Rx DMA transfer complete callback */
    hsai->hdmarx->XferCpltCallback = SAI_DMARxCplt;     /* 赋值或初始化 */

    /* Set the DMA error callback */
    hsai->hdmarx->XferErrorCallback = SAI_DMAError;     /* 赋值或初始化 */

    /* Set the DMA Rx abort callback */
    hsai->hdmarx->XferAbortCallback = NULL;             /* 赋值或初始化 */

    /* Enable the Rx DMA Stream */
    if (HAL_DMA_Start_IT(hsai->hdmarx, (uint32_t)&hsai->Instance->DR, (uint32_t)hsai->pBuffPtr, hsai->XferSize) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      __HAL_UNLOCK(hsai);                               /* HAL辅助宏/函数 */
      return  HAL_ERROR;                                /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Enable the interrupts for error handling */
    __HAL_SAI_ENABLE_IT(hsai, SAI_InterruptFlag(hsai, SAI_MODE_DMA));  /* HAL辅助宏/函数 */

    /* Enable SAI Rx DMA Request */
    hsai->Instance->CR1 |= SAI_xCR1_DMAEN;              /* 赋值或初始化 */

    /* Check if the SAI is already enabled */
    if ((hsai->Instance->CR1 & SAI_xCR1_SAIEN) == RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable SAI peripheral */
      __HAL_SAI_ENABLE(hsai);                           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Process Unlocked */
    __HAL_UNLOCK(hsai);                                 /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Enable the Tx mute mode.
  * @param  hsai pointer to a SAI_HandleTypeDef structure that contains
  *               the configuration information for SAI module.
  * @param  val  value sent during the mute @ref SAI_Block_Mute_Value
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SAI_EnableTxMuteMode(SAI_HandleTypeDef *hsai, uint16_t val)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  assert_param(IS_SAI_BLOCK_MUTE_VALUE(val));           /* 执行语句 */

  if (hsai->State != HAL_SAI_STATE_RESET)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    CLEAR_BIT(hsai->Instance->CR2, SAI_xCR2_MUTEVAL | SAI_xCR2_MUTE);  /* 执行语句 */
    SET_BIT(hsai->Instance->CR2, SAI_xCR2_MUTE | val);  /* 执行语句 */
    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  return HAL_ERROR;                                     /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Disable the Tx mute mode.
  * @param  hsai pointer to a SAI_HandleTypeDef structure that contains
  *               the configuration information for SAI module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SAI_DisableTxMuteMode(SAI_HandleTypeDef *hsai)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  if (hsai->State != HAL_SAI_STATE_RESET)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    CLEAR_BIT(hsai->Instance->CR2, SAI_xCR2_MUTEVAL | SAI_xCR2_MUTE);  /* 执行语句 */
    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  return HAL_ERROR;                                     /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Enable the Rx mute detection.
  * @param  hsai pointer to a SAI_HandleTypeDef structure that contains
  *               the configuration information for SAI module.
  * @param  callback function called when the mute is detected.
  * @param  counter number a data before mute detection max 63.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SAI_EnableRxMuteMode(SAI_HandleTypeDef *hsai, SAIcallback callback, uint16_t counter)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  assert_param(IS_SAI_BLOCK_MUTE_COUNTER(counter));     /* 执行语句 */

  if (hsai->State != HAL_SAI_STATE_RESET)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* set the mute counter */
    CLEAR_BIT(hsai->Instance->CR2, SAI_xCR2_MUTECNT);   /* 执行语句 */
    SET_BIT(hsai->Instance->CR2, (uint32_t)((uint32_t)counter << SAI_xCR2_MUTECNT_Pos));  /* 执行语句 */
    hsai->mutecallback = callback;                      /* 赋值或初始化 */
    /* enable the IT interrupt */
    __HAL_SAI_ENABLE_IT(hsai, SAI_IT_MUTEDET);          /* HAL辅助宏/函数 */
    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  return HAL_ERROR;                                     /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Disable the Rx mute detection.
  * @param  hsai pointer to a SAI_HandleTypeDef structure that contains
  *               the configuration information for SAI module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SAI_DisableRxMuteMode(SAI_HandleTypeDef *hsai)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  if (hsai->State != HAL_SAI_STATE_RESET)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* set the mutecallback to NULL */
    hsai->mutecallback = (SAIcallback)NULL;             /* 赋值或初始化 */
    /* enable the IT interrupt */
    __HAL_SAI_DISABLE_IT(hsai, SAI_IT_MUTEDET);         /* HAL辅助宏/函数 */
    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  return HAL_ERROR;                                     /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Handle SAI interrupt request.
  * @param  hsai pointer to a SAI_HandleTypeDef structure that contains
  *               the configuration information for SAI module.
  * @retval None
  */
void HAL_SAI_IRQHandler(SAI_HandleTypeDef *hsai)        /* 中断服务函数 */
{                                                       /* 代码块开始 */
  if (hsai->State != HAL_SAI_STATE_RESET)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    uint32_t itflags = hsai->Instance->SR;              /* 32位无符号变量/函数 */
    uint32_t itsources = hsai->Instance->IMR;           /* 32位无符号变量/函数 */
    uint32_t cr1config = hsai->Instance->CR1;           /* 32位无符号变量/函数 */
    uint32_t tmperror;                                  /* 32位无符号变量/函数 */

    /* SAI Fifo request interrupt occurred ------------------------------------*/
    if (((itflags & SAI_xSR_FREQ) == SAI_xSR_FREQ) && ((itsources & SAI_IT_FREQ) == SAI_IT_FREQ))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hsai->InterruptServiceRoutine(hsai);              /* 执行语句 */
    }                                                   /* 代码块结束 */
    /* SAI Overrun error interrupt occurred ----------------------------------*/
    else if (((itflags & SAI_FLAG_OVRUDR) == SAI_FLAG_OVRUDR) && ((itsources & SAI_IT_OVRUDR) == SAI_IT_OVRUDR))  /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* Clear the SAI Overrun flag */
      __HAL_SAI_CLEAR_FLAG(hsai, SAI_FLAG_OVRUDR);      /* HAL辅助宏/函数 */

      /* Get the SAI error code */
      tmperror = ((hsai->State == HAL_SAI_STATE_BUSY_RX) ? HAL_SAI_ERROR_OVR : HAL_SAI_ERROR_UDR);  /* 赋值或初始化 */

      /* Change the SAI error code */
      hsai->ErrorCode |= tmperror;                      /* 赋值或初始化 */

      /* the transfer is not stopped, we will forward the information to the user and we let the user decide what needs to be done */
#if (USE_HAL_SAI_REGISTER_CALLBACKS == 1)               /* 条件编译 */
      hsai->ErrorCallback(hsai);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_SAI_ErrorCallback(hsai);                      /* HAL库函数调用 */
#endif /* USE_HAL_SAI_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
    /* SAI mutedet interrupt occurred ----------------------------------*/
    else if (((itflags & SAI_FLAG_MUTEDET) == SAI_FLAG_MUTEDET) && ((itsources & SAI_IT_MUTEDET) == SAI_IT_MUTEDET))  /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* Clear the SAI mutedet flag */
      __HAL_SAI_CLEAR_FLAG(hsai, SAI_FLAG_MUTEDET);     /* HAL辅助宏/函数 */

      /* call the call back function */
      if (hsai->mutecallback != (SAIcallback)NULL)      /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* inform the user that an RX mute event has been detected */
        hsai->mutecallback();                           /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    /* SAI AFSDET interrupt occurred ----------------------------------*/
    else if (((itflags & SAI_FLAG_AFSDET) == SAI_FLAG_AFSDET) && ((itsources & SAI_IT_AFSDET) == SAI_IT_AFSDET))  /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* Clear the SAI AFSDET flag */
      __HAL_SAI_CLEAR_FLAG(hsai, SAI_FLAG_AFSDET);      /* HAL辅助宏/函数 */

      /* Change the SAI error code */
      hsai->ErrorCode |= HAL_SAI_ERROR_AFSDET;          /* 赋值或初始化 */

      /* Check SAI DMA is enabled or not */
      if ((cr1config & SAI_xCR1_DMAEN) == SAI_xCR1_DMAEN)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Abort the SAI DMA Streams */
        if (hsai->hdmatx != NULL)                       /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Set the DMA Tx abort callback */
          hsai->hdmatx->XferAbortCallback = SAI_DMAAbort;  /* 赋值或初始化 */

          /* Abort DMA in IT mode */
          HAL_DMA_Abort_IT(hsai->hdmatx);               /* HAL DMA操作 */
        }                                               /* 代码块结束 */
        else if (hsai->hdmarx != NULL)                  /* 否则如果 */
        {                                               /* 代码块开始 */
          /* Set the DMA Rx abort callback */
          hsai->hdmarx->XferAbortCallback = SAI_DMAAbort;  /* 赋值或初始化 */

          /* Abort DMA in IT mode */
          HAL_DMA_Abort_IT(hsai->hdmarx);               /* HAL DMA操作 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Abort SAI */
        HAL_SAI_Abort(hsai);                            /* HAL库函数调用 */

        /* Set error callback */
#if (USE_HAL_SAI_REGISTER_CALLBACKS == 1)               /* 条件编译 */
        hsai->ErrorCallback(hsai);                      /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_SAI_ErrorCallback(hsai);                    /* HAL库函数调用 */
#endif /* USE_HAL_SAI_REGISTER_CALLBACKS */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    /* SAI LFSDET interrupt occurred ----------------------------------*/
    else if (((itflags & SAI_FLAG_LFSDET) == SAI_FLAG_LFSDET) && ((itsources & SAI_IT_LFSDET) == SAI_IT_LFSDET))  /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* Clear the SAI LFSDET flag */
      __HAL_SAI_CLEAR_FLAG(hsai, SAI_FLAG_LFSDET);      /* HAL辅助宏/函数 */

      /* Change the SAI error code */
      hsai->ErrorCode |= HAL_SAI_ERROR_LFSDET;          /* 赋值或初始化 */

      /* Check SAI DMA is enabled or not */
      if ((cr1config & SAI_xCR1_DMAEN) == SAI_xCR1_DMAEN)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Abort the SAI DMA Streams */
        if (hsai->hdmatx != NULL)                       /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Set the DMA Tx abort callback */
          hsai->hdmatx->XferAbortCallback = SAI_DMAAbort;  /* 赋值或初始化 */

          /* Abort DMA in IT mode */
          HAL_DMA_Abort_IT(hsai->hdmatx);               /* HAL DMA操作 */
        }                                               /* 代码块结束 */
        else if (hsai->hdmarx != NULL)                  /* 否则如果 */
        {                                               /* 代码块开始 */
          /* Set the DMA Rx abort callback */
          hsai->hdmarx->XferAbortCallback = SAI_DMAAbort;  /* 赋值或初始化 */

          /* Abort DMA in IT mode */
          HAL_DMA_Abort_IT(hsai->hdmarx);               /* HAL DMA操作 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Abort SAI */
        HAL_SAI_Abort(hsai);                            /* HAL库函数调用 */

        /* Set error callback */
#if (USE_HAL_SAI_REGISTER_CALLBACKS == 1)               /* 条件编译 */
        hsai->ErrorCallback(hsai);                      /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_SAI_ErrorCallback(hsai);                    /* HAL库函数调用 */
#endif /* USE_HAL_SAI_REGISTER_CALLBACKS */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    /* SAI WCKCFG interrupt occurred ----------------------------------*/
    else if (((itflags & SAI_FLAG_WCKCFG) == SAI_FLAG_WCKCFG) && ((itsources & SAI_IT_WCKCFG) == SAI_IT_WCKCFG))  /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* Clear the SAI WCKCFG flag */
      __HAL_SAI_CLEAR_FLAG(hsai, SAI_FLAG_WCKCFG);      /* HAL辅助宏/函数 */

      /* Change the SAI error code */
      hsai->ErrorCode |= HAL_SAI_ERROR_WCKCFG;          /* 赋值或初始化 */

      /* Check SAI DMA is enabled or not */
      if ((cr1config & SAI_xCR1_DMAEN) == SAI_xCR1_DMAEN)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Abort the SAI DMA Streams */
        if (hsai->hdmatx != NULL)                       /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Set the DMA Tx abort callback */
          hsai->hdmatx->XferAbortCallback = SAI_DMAAbort;  /* 赋值或初始化 */

          /* Abort DMA in IT mode */
          HAL_DMA_Abort_IT(hsai->hdmatx);               /* HAL DMA操作 */
        }                                               /* 代码块结束 */
        else if (hsai->hdmarx != NULL)                  /* 否则如果 */
        {                                               /* 代码块开始 */
          /* Set the DMA Rx abort callback */
          hsai->hdmarx->XferAbortCallback = SAI_DMAAbort;  /* 赋值或初始化 */

          /* Abort DMA in IT mode */
          HAL_DMA_Abort_IT(hsai->hdmarx);               /* HAL DMA操作 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* If WCKCFG occurs, SAI audio block is automatically disabled */
        /* Disable all interrupts and clear all flags */
        hsai->Instance->IMR = 0U;                       /* 赋值或初始化 */
        hsai->Instance->CLRFR = 0xFFFFFFFFU;            /* 赋值或初始化 */

        /* Set the SAI state to ready to be able to start again the process */
        hsai->State = HAL_SAI_STATE_READY;              /* 赋值或初始化 */

        /* Initialize XferCount */
        hsai->XferCount = 0U;                           /* 赋值或初始化 */

        /* SAI error Callback */
#if (USE_HAL_SAI_REGISTER_CALLBACKS == 1)               /* 条件编译 */
        hsai->ErrorCallback(hsai);                      /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_SAI_ErrorCallback(hsai);                    /* HAL库函数调用 */
#endif /* USE_HAL_SAI_REGISTER_CALLBACKS */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    /* SAI CNRDY interrupt occurred ----------------------------------*/
    else if (((itflags & SAI_FLAG_CNRDY) == SAI_FLAG_CNRDY) && ((itsources & SAI_IT_CNRDY) == SAI_IT_CNRDY))  /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* Clear the SAI CNRDY flag */
      __HAL_SAI_CLEAR_FLAG(hsai, SAI_FLAG_CNRDY);       /* HAL辅助宏/函数 */

      /* Change the SAI error code */
      hsai->ErrorCode |= HAL_SAI_ERROR_CNREADY;         /* 赋值或初始化 */

      /* the transfer is not stopped, we will forward the information to the user and we let the user decide what needs to be done */
#if (USE_HAL_SAI_REGISTER_CALLBACKS == 1)               /* 条件编译 */
      hsai->ErrorCallback(hsai);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_SAI_ErrorCallback(hsai);                      /* HAL库函数调用 */
#endif /* USE_HAL_SAI_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Nothing to do */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief Tx Transfer completed callback.
  * @param  hsai pointer to a SAI_HandleTypeDef structure that contains
  *                the configuration information for SAI module.
  * @retval None
  */
__weak void HAL_SAI_TxCpltCallback(SAI_HandleTypeDef *hsai)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsai);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SAI_TxCpltCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief Tx Transfer Half completed callback.
  * @param  hsai pointer to a SAI_HandleTypeDef structure that contains
  *               the configuration information for SAI module.
  * @retval None
  */
__weak void HAL_SAI_TxHalfCpltCallback(SAI_HandleTypeDef *hsai)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsai);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SAI_TxHalfCpltCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief Rx Transfer completed callback.
  * @param  hsai pointer to a SAI_HandleTypeDef structure that contains
  *               the configuration information for SAI module.
  * @retval None
  */
__weak void HAL_SAI_RxCpltCallback(SAI_HandleTypeDef *hsai)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsai);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SAI_RxCpltCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief Rx Transfer half completed callback.
  * @param  hsai pointer to a SAI_HandleTypeDef structure that contains
  *               the configuration information for SAI module.
  * @retval None
  */
__weak void HAL_SAI_RxHalfCpltCallback(SAI_HandleTypeDef *hsai)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsai);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SAI_RxHalfCpltCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief SAI error callback.
  * @param  hsai pointer to a SAI_HandleTypeDef structure that contains
  *               the configuration information for SAI module.
  * @retval None
  */
__weak void HAL_SAI_ErrorCallback(SAI_HandleTypeDef *hsai)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsai);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SAI_ErrorCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup SAI_Exported_Functions_Group3 Peripheral State functions
  * @brief    Peripheral State functions
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
  * @brief  Return the SAI handle state.
  * @param  hsai pointer to a SAI_HandleTypeDef structure that contains
  *               the configuration information for SAI module.
  * @retval HAL state
  */
HAL_SAI_StateTypeDef HAL_SAI_GetState(SAI_HandleTypeDef *hsai)  /* HAL库函数调用 */
{                                                       /* 代码块开始 */
  return hsai->State;                                   /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return the SAI error code.
  * @param  hsai pointer to a SAI_HandleTypeDef structure that contains
  *              the configuration information for the specified SAI Block.
  * @retval SAI Error Code
  */
uint32_t HAL_SAI_GetError(SAI_HandleTypeDef *hsai)      /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  return hsai->ErrorCode;                               /* 返回值 */
}                                                       /* 代码块结束 */
/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup SAI_Private_Functions
  * @brief      Private functions
  * @{
  */

/**
  * @brief  Initialize the SAI I2S protocol according to the specified parameters
  *         in the SAI_InitTypeDef and create the associated handle.
  * @param  hsai pointer to a SAI_HandleTypeDef structure that contains
  *               the configuration information for SAI module.
  * @param  protocol one of the supported protocol.
  * @param  datasize one of the supported datasize @ref SAI_Protocol_DataSize
  *                the configuration information for SAI module.
  * @param  nbslot number of slot minimum value is 2 and max is 16.
  *                    the value must be a multiple of 2.
  * @retval HAL status
  */
static HAL_StatusTypeDef SAI_InitI2S(SAI_HandleTypeDef *hsai, uint32_t protocol, uint32_t datasize, uint32_t nbslot)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  hsai->Init.Protocol            = SAI_FREE_PROTOCOL;   /* 赋值或初始化 */
  hsai->Init.FirstBit            = SAI_FIRSTBIT_MSB;    /* 赋值或初始化 */
  /* Compute ClockStrobing according AudioMode */
  if ((hsai->Init.AudioMode == SAI_MODEMASTER_TX) || (hsai->Init.AudioMode == SAI_MODESLAVE_TX))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Transmit */
    hsai->Init.ClockStrobing     = SAI_CLOCKSTROBING_FALLINGEDGE;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Receive */
    hsai->Init.ClockStrobing     = SAI_CLOCKSTROBING_RISINGEDGE;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  hsai->FrameInit.FSDefinition   = SAI_FS_CHANNEL_IDENTIFICATION;  /* 赋值或初始化 */
  hsai->SlotInit.SlotActive      = SAI_SLOTACTIVE_ALL;  /* 赋值或初始化 */
  hsai->SlotInit.FirstBitOffset  = 0U;                  /* 赋值或初始化 */
  hsai->SlotInit.SlotNumber      = nbslot;              /* 赋值或初始化 */

  /* in IS2 the number of slot must be even */
  if ((nbslot & 0x1U) != 0U)                            /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  if (protocol == SAI_I2S_STANDARD)                     /* 条件判断 */
  {                                                     /* 代码块开始 */
    hsai->FrameInit.FSPolarity = SAI_FS_ACTIVE_LOW;     /* 赋值或初始化 */
    hsai->FrameInit.FSOffset   = SAI_FS_BEFOREFIRSTBIT;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* SAI_I2S_MSBJUSTIFIED or SAI_I2S_LSBJUSTIFIED */
    hsai->FrameInit.FSPolarity = SAI_FS_ACTIVE_HIGH;    /* 赋值或初始化 */
    hsai->FrameInit.FSOffset   = SAI_FS_FIRSTBIT;       /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Frame definition */
  switch (datasize)                                     /* switch分支 */
  {                                                     /* 代码块开始 */
    case SAI_PROTOCOL_DATASIZE_16BIT:                   /* case标签 */
      hsai->Init.DataSize = SAI_DATASIZE_16;            /* 赋值或初始化 */
      hsai->FrameInit.FrameLength = 32U * (nbslot / 2U);  /* 赋值或初始化 */
      hsai->FrameInit.ActiveFrameLength = 16U * (nbslot / 2U);  /* 赋值或初始化 */
      hsai->SlotInit.SlotSize = SAI_SLOTSIZE_16B;       /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case SAI_PROTOCOL_DATASIZE_16BITEXTENDED :          /* case标签 */
      hsai->Init.DataSize = SAI_DATASIZE_16;            /* 赋值或初始化 */
      hsai->FrameInit.FrameLength = 64U * (nbslot / 2U);  /* 赋值或初始化 */
      hsai->FrameInit.ActiveFrameLength = 32U * (nbslot / 2U);  /* 赋值或初始化 */
      hsai->SlotInit.SlotSize = SAI_SLOTSIZE_32B;       /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case SAI_PROTOCOL_DATASIZE_24BIT:                   /* case标签 */
      hsai->Init.DataSize = SAI_DATASIZE_24;            /* 赋值或初始化 */
      hsai->FrameInit.FrameLength = 64U * (nbslot / 2U);  /* 赋值或初始化 */
      hsai->FrameInit.ActiveFrameLength = 32U * (nbslot / 2U);  /* 赋值或初始化 */
      hsai->SlotInit.SlotSize = SAI_SLOTSIZE_32B;       /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case SAI_PROTOCOL_DATASIZE_32BIT:                   /* case标签 */
      hsai->Init.DataSize = SAI_DATASIZE_32;            /* 赋值或初始化 */
      hsai->FrameInit.FrameLength = 64U * (nbslot / 2U);  /* 赋值或初始化 */
      hsai->FrameInit.ActiveFrameLength = 32U * (nbslot / 2U);  /* 赋值或初始化 */
      hsai->SlotInit.SlotSize = SAI_SLOTSIZE_32B;       /* 赋值或初始化 */
      break;                                            /* 跳出 */
    default :                                           /* default分支 */
      return HAL_ERROR;                                 /* 返回值 */
  }                                                     /* 代码块结束 */
  if (protocol == SAI_I2S_LSBJUSTIFIED)                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (datasize == SAI_PROTOCOL_DATASIZE_16BITEXTENDED)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hsai->SlotInit.FirstBitOffset = 16U;              /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    if (datasize == SAI_PROTOCOL_DATASIZE_24BIT)        /* 条件判断 */
    {                                                   /* 代码块开始 */
      hsai->SlotInit.FirstBitOffset = 8U;               /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initialize the SAI PCM protocol according to the specified parameters
  *         in the SAI_InitTypeDef and create the associated handle.
  * @param  hsai pointer to a SAI_HandleTypeDef structure that contains
  *               the configuration information for SAI module.
  * @param  protocol one of the supported protocol
  * @param  datasize one of the supported datasize @ref SAI_Protocol_DataSize
  * @param  nbslot number of slot minimum value is 1 and the max is 16.
  * @retval HAL status
  */
static HAL_StatusTypeDef SAI_InitPCM(SAI_HandleTypeDef *hsai, uint32_t protocol, uint32_t datasize, uint32_t nbslot)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  hsai->Init.Protocol            = SAI_FREE_PROTOCOL;   /* 赋值或初始化 */
  hsai->Init.FirstBit            = SAI_FIRSTBIT_MSB;    /* 赋值或初始化 */
  /* Compute ClockStrobing according AudioMode */
  if ((hsai->Init.AudioMode == SAI_MODEMASTER_TX) || (hsai->Init.AudioMode == SAI_MODESLAVE_TX))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Transmit */
    hsai->Init.ClockStrobing     = SAI_CLOCKSTROBING_RISINGEDGE;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Receive */
    hsai->Init.ClockStrobing     = SAI_CLOCKSTROBING_FALLINGEDGE;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  hsai->FrameInit.FSDefinition   = SAI_FS_STARTFRAME;   /* 赋值或初始化 */
  hsai->FrameInit.FSPolarity     = SAI_FS_ACTIVE_HIGH;  /* 赋值或初始化 */
  hsai->FrameInit.FSOffset       = SAI_FS_BEFOREFIRSTBIT;  /* 赋值或初始化 */
  hsai->SlotInit.FirstBitOffset  = 0U;                  /* 赋值或初始化 */
  hsai->SlotInit.SlotNumber      = nbslot;              /* 赋值或初始化 */
  hsai->SlotInit.SlotActive      = SAI_SLOTACTIVE_ALL;  /* 赋值或初始化 */

  if (protocol == SAI_PCM_SHORT)                        /* 条件判断 */
  {                                                     /* 代码块开始 */
    hsai->FrameInit.ActiveFrameLength = 1;              /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* SAI_PCM_LONG */
    hsai->FrameInit.ActiveFrameLength = 13;             /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  switch (datasize)                                     /* switch分支 */
  {                                                     /* 代码块开始 */
    case SAI_PROTOCOL_DATASIZE_16BIT:                   /* case标签 */
      hsai->Init.DataSize = SAI_DATASIZE_16;            /* 赋值或初始化 */
      hsai->FrameInit.FrameLength = 16U * nbslot;       /* 赋值或初始化 */
      hsai->SlotInit.SlotSize = SAI_SLOTSIZE_16B;       /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case SAI_PROTOCOL_DATASIZE_16BITEXTENDED :          /* case标签 */
      hsai->Init.DataSize = SAI_DATASIZE_16;            /* 赋值或初始化 */
      hsai->FrameInit.FrameLength = 32U * nbslot;       /* 赋值或初始化 */
      hsai->SlotInit.SlotSize = SAI_SLOTSIZE_32B;       /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case SAI_PROTOCOL_DATASIZE_24BIT :                  /* case标签 */
      hsai->Init.DataSize = SAI_DATASIZE_24;            /* 赋值或初始化 */
      hsai->FrameInit.FrameLength = 32U * nbslot;       /* 赋值或初始化 */
      hsai->SlotInit.SlotSize = SAI_SLOTSIZE_32B;       /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case SAI_PROTOCOL_DATASIZE_32BIT:                   /* case标签 */
      hsai->Init.DataSize = SAI_DATASIZE_32;            /* 赋值或初始化 */
      hsai->FrameInit.FrameLength = 32U * nbslot;       /* 赋值或初始化 */
      hsai->SlotInit.SlotSize = SAI_SLOTSIZE_32B;       /* 赋值或初始化 */
      break;                                            /* 跳出 */
    default :                                           /* default分支 */
      return HAL_ERROR;                                 /* 返回值 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Fill the fifo.
  * @param  hsai pointer to a SAI_HandleTypeDef structure that contains
  *               the configuration information for SAI module.
  * @retval None
  */
static void SAI_FillFifo(SAI_HandleTypeDef *hsai)       /* 静态函数 */
{                                                       /* 代码块开始 */
  /* fill the fifo with data before to enabled the SAI */
  while (((hsai->Instance->SR & SAI_xSR_FLVL) != SAI_FIFOSTATUS_FULL) && (hsai->XferCount > 0U))  /* while循环 */
  {                                                     /* 代码块开始 */
    if ((hsai->Init.DataSize == SAI_DATASIZE_8) && (hsai->Init.CompandingMode == SAI_NOCOMPANDING))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hsai->Instance->DR = (*hsai->pBuffPtr++);         /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if (hsai->Init.DataSize <= SAI_DATASIZE_16)    /* 否则如果 */
    {                                                   /* 代码块开始 */
      hsai->Instance->DR = *((uint32_t *)hsai->pBuffPtr);  /* 赋值或初始化 */
      hsai->pBuffPtr += 2U;                             /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hsai->Instance->DR = *((uint32_t *)hsai->pBuffPtr);  /* 赋值或初始化 */
      hsai->pBuffPtr += 4U;                             /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    hsai->XferCount--;                                  /* 执行语句 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return the interrupt flag to set according the SAI setup.
  * @param  hsai pointer to a SAI_HandleTypeDef structure that contains
  *               the configuration information for SAI module.
  * @param  mode SAI_MODE_DMA or SAI_MODE_IT
  * @retval the list of the IT flag to enable
 */
static uint32_t SAI_InterruptFlag(SAI_HandleTypeDef *hsai, uint32_t mode)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpIT = SAI_IT_OVRUDR;                       /* 32位无符号变量/函数 */

  if (mode == SAI_MODE_IT)                              /* 条件判断 */
  {                                                     /* 代码块开始 */
    tmpIT |= SAI_IT_FREQ;                               /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if ((hsai->Init.Protocol == SAI_AC97_PROTOCOL) &&     /* 条件判断 */
      ((hsai->Init.AudioMode == SAI_MODESLAVE_RX) || (hsai->Init.AudioMode == SAI_MODEMASTER_RX)))  /* 源代码 */
  {                                                     /* 代码块开始 */
    tmpIT |= SAI_IT_CNRDY;                              /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if ((hsai->Init.AudioMode == SAI_MODESLAVE_RX) || (hsai->Init.AudioMode == SAI_MODESLAVE_TX))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    tmpIT |= SAI_IT_AFSDET | SAI_IT_LFSDET;             /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* hsai has been configured in master mode */
    tmpIT |= SAI_IT_WCKCFG;                             /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  return tmpIT;                                         /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Disable the SAI and wait for the disabling.
  * @param  hsai  pointer to a SAI_HandleTypeDef structure that contains
  *               the configuration information for SAI module.
  * @retval None
  */
static HAL_StatusTypeDef SAI_Disable(SAI_HandleTypeDef *hsai)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t count = SAI_DEFAULT_TIMEOUT * (SystemCoreClock / 7U / 1000U);  /* 32位无符号变量/函数 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Disable the SAI instance */
  __HAL_SAI_DISABLE(hsai);                              /* HAL辅助宏/函数 */

  do                                                    /* do-while循环 */
  {                                                     /* 代码块开始 */
    /* Check for the Timeout */
    if (count-- == 0U)                                  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Update error code */
      hsai->ErrorCode |= HAL_SAI_ERROR_TIMEOUT;         /* 赋值或初始化 */
      status = HAL_TIMEOUT;                             /* 赋值或初始化 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  while ((hsai->Instance->CR1 & SAI_xCR1_SAIEN) != RESET);  /* while循环 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Tx Handler for Transmit in Interrupt mode 8-Bit transfer.
  * @param  hsai pointer to a SAI_HandleTypeDef structure that contains
  *               the configuration information for SAI module.
  * @retval None
  */
static void SAI_Transmit_IT8Bit(SAI_HandleTypeDef *hsai)  /* 静态函数 */
{                                                       /* 代码块开始 */
  if (hsai->XferCount == 0U)                            /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Handle the end of the transmission */
    /* Disable FREQ and OVRUDR interrupts */
    __HAL_SAI_DISABLE_IT(hsai, SAI_InterruptFlag(hsai, SAI_MODE_IT));  /* HAL辅助宏/函数 */
    hsai->State = HAL_SAI_STATE_READY;                  /* 赋值或初始化 */
#if (USE_HAL_SAI_REGISTER_CALLBACKS == 1)               /* 条件编译 */
    hsai->TxCpltCallback(hsai);                         /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_SAI_TxCpltCallback(hsai);                       /* HAL库函数调用 */
#endif /* USE_HAL_SAI_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Write data on DR register */
    hsai->Instance->DR = (*hsai->pBuffPtr++);           /* 赋值或初始化 */
    hsai->XferCount--;                                  /* 执行语句 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Tx Handler for Transmit in Interrupt mode for 16-Bit transfer.
  * @param  hsai pointer to a SAI_HandleTypeDef structure that contains
  *               the configuration information for SAI module.
  * @retval None
  */
static void SAI_Transmit_IT16Bit(SAI_HandleTypeDef *hsai)  /* 静态函数 */
{                                                       /* 代码块开始 */
  if (hsai->XferCount == 0U)                            /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Handle the end of the transmission */
    /* Disable FREQ and OVRUDR interrupts */
    __HAL_SAI_DISABLE_IT(hsai, SAI_InterruptFlag(hsai, SAI_MODE_IT));  /* HAL辅助宏/函数 */
    hsai->State = HAL_SAI_STATE_READY;                  /* 赋值或初始化 */
#if (USE_HAL_SAI_REGISTER_CALLBACKS == 1)               /* 条件编译 */
    hsai->TxCpltCallback(hsai);                         /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_SAI_TxCpltCallback(hsai);                       /* HAL库函数调用 */
#endif /* USE_HAL_SAI_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Write data on DR register */
    hsai->Instance->DR = *(uint16_t *)hsai->pBuffPtr;   /* 赋值或初始化 */
    hsai->pBuffPtr += 2U;                               /* 赋值或初始化 */
    hsai->XferCount--;                                  /* 执行语句 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Tx Handler for Transmit in Interrupt mode for 32-Bit transfer.
  * @param  hsai pointer to a SAI_HandleTypeDef structure that contains
  *               the configuration information for SAI module.
  * @retval None
  */
static void SAI_Transmit_IT32Bit(SAI_HandleTypeDef *hsai)  /* 静态函数 */
{                                                       /* 代码块开始 */
  if (hsai->XferCount == 0U)                            /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Handle the end of the transmission */
    /* Disable FREQ and OVRUDR interrupts */
    __HAL_SAI_DISABLE_IT(hsai, SAI_InterruptFlag(hsai, SAI_MODE_IT));  /* HAL辅助宏/函数 */
    hsai->State = HAL_SAI_STATE_READY;                  /* 赋值或初始化 */
#if (USE_HAL_SAI_REGISTER_CALLBACKS == 1)               /* 条件编译 */
    hsai->TxCpltCallback(hsai);                         /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_SAI_TxCpltCallback(hsai);                       /* HAL库函数调用 */
#endif /* USE_HAL_SAI_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Write data on DR register */
    hsai->Instance->DR = *(uint32_t *)hsai->pBuffPtr;   /* 赋值或初始化 */
    hsai->pBuffPtr += 4U;                               /* 赋值或初始化 */
    hsai->XferCount--;                                  /* 执行语句 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Rx Handler for Receive in Interrupt mode 8-Bit transfer.
  * @param  hsai pointer to a SAI_HandleTypeDef structure that contains
  *               the configuration information for SAI module.
  * @retval None
  */
static void SAI_Receive_IT8Bit(SAI_HandleTypeDef *hsai)  /* 静态函数 */
{                                                       /* 代码块开始 */
  /* Receive data */
  (*hsai->pBuffPtr++) = hsai->Instance->DR;             /* 赋值或初始化 */
  hsai->XferCount--;                                    /* 执行语句 */

  /* Check end of the transfer */
  if (hsai->XferCount == 0U)                            /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable TXE and OVRUDR interrupts */
    __HAL_SAI_DISABLE_IT(hsai, SAI_InterruptFlag(hsai, SAI_MODE_IT));  /* HAL辅助宏/函数 */

    /* Clear the SAI Overrun flag */
    __HAL_SAI_CLEAR_FLAG(hsai, SAI_FLAG_OVRUDR);        /* HAL辅助宏/函数 */

    hsai->State = HAL_SAI_STATE_READY;                  /* 赋值或初始化 */
#if (USE_HAL_SAI_REGISTER_CALLBACKS == 1)               /* 条件编译 */
    hsai->RxCpltCallback(hsai);                         /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_SAI_RxCpltCallback(hsai);                       /* HAL库函数调用 */
#endif /* USE_HAL_SAI_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Rx Handler for Receive in Interrupt mode for 16-Bit transfer.
  * @param  hsai pointer to a SAI_HandleTypeDef structure that contains
  *               the configuration information for SAI module.
  * @retval None
  */
static void SAI_Receive_IT16Bit(SAI_HandleTypeDef *hsai)  /* 静态函数 */
{                                                       /* 代码块开始 */
  /* Receive data */
  *(uint16_t *)hsai->pBuffPtr = hsai->Instance->DR;     /* 赋值或初始化 */
  hsai->pBuffPtr += 2U;                                 /* 赋值或初始化 */
  hsai->XferCount--;                                    /* 执行语句 */

  /* Check end of the transfer */
  if (hsai->XferCount == 0U)                            /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable TXE and OVRUDR interrupts */
    __HAL_SAI_DISABLE_IT(hsai, SAI_InterruptFlag(hsai, SAI_MODE_IT));  /* HAL辅助宏/函数 */

    /* Clear the SAI Overrun flag */
    __HAL_SAI_CLEAR_FLAG(hsai, SAI_FLAG_OVRUDR);        /* HAL辅助宏/函数 */

    hsai->State = HAL_SAI_STATE_READY;                  /* 赋值或初始化 */
#if (USE_HAL_SAI_REGISTER_CALLBACKS == 1)               /* 条件编译 */
    hsai->RxCpltCallback(hsai);                         /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_SAI_RxCpltCallback(hsai);                       /* HAL库函数调用 */
#endif /* USE_HAL_SAI_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Rx Handler for Receive in Interrupt mode for 32-Bit transfer.
  * @param  hsai pointer to a SAI_HandleTypeDef structure that contains
  *               the configuration information for SAI module.
  * @retval None
  */
static void SAI_Receive_IT32Bit(SAI_HandleTypeDef *hsai)  /* 静态函数 */
{                                                       /* 代码块开始 */
  /* Receive data */
  *(uint32_t *)hsai->pBuffPtr = hsai->Instance->DR;     /* 赋值或初始化 */
  hsai->pBuffPtr += 4U;                                 /* 赋值或初始化 */
  hsai->XferCount--;                                    /* 执行语句 */

  /* Check end of the transfer */
  if (hsai->XferCount == 0U)                            /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable TXE and OVRUDR interrupts */
    __HAL_SAI_DISABLE_IT(hsai, SAI_InterruptFlag(hsai, SAI_MODE_IT));  /* HAL辅助宏/函数 */

    /* Clear the SAI Overrun flag */
    __HAL_SAI_CLEAR_FLAG(hsai, SAI_FLAG_OVRUDR);        /* HAL辅助宏/函数 */

    hsai->State = HAL_SAI_STATE_READY;                  /* 赋值或初始化 */
#if (USE_HAL_SAI_REGISTER_CALLBACKS == 1)               /* 条件编译 */
    hsai->RxCpltCallback(hsai);                         /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_SAI_RxCpltCallback(hsai);                       /* HAL库函数调用 */
#endif /* USE_HAL_SAI_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief DMA SAI transmit process complete callback.
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA module.
  * @retval None
  */
static void SAI_DMATxCplt(DMA_HandleTypeDef *hdma)      /* 静态函数 */
{                                                       /* 代码块开始 */
  SAI_HandleTypeDef *hsai = (SAI_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;  /* 赋值或初始化 */

  if (hdma->Init.Mode != DMA_CIRCULAR)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hsai->XferCount = 0U;                               /* 赋值或初始化 */

    /* Disable SAI Tx DMA Request */
    hsai->Instance->CR1 &= (uint32_t)(~SAI_xCR1_DMAEN);  /* 赋值或初始化 */

    /* Stop the interrupts error handling */
    __HAL_SAI_DISABLE_IT(hsai, SAI_InterruptFlag(hsai, SAI_MODE_DMA));  /* HAL辅助宏/函数 */

    hsai->State = HAL_SAI_STATE_READY;                  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
#if (USE_HAL_SAI_REGISTER_CALLBACKS == 1)               /* 条件编译 */
  hsai->TxCpltCallback(hsai);                           /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_SAI_TxCpltCallback(hsai);                         /* HAL库函数调用 */
#endif /* USE_HAL_SAI_REGISTER_CALLBACKS */
}                                                       /* 代码块结束 */

/**
  * @brief DMA SAI transmit process half complete callback.
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA module.
  * @retval None
  */
static void SAI_DMATxHalfCplt(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  SAI_HandleTypeDef *hsai = (SAI_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;  /* 赋值或初始化 */

#if (USE_HAL_SAI_REGISTER_CALLBACKS == 1)               /* 条件编译 */
  hsai->TxHalfCpltCallback(hsai);                       /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_SAI_TxHalfCpltCallback(hsai);                     /* HAL库函数调用 */
#endif /* USE_HAL_SAI_REGISTER_CALLBACKS */
}                                                       /* 代码块结束 */

/**
  * @brief DMA SAI receive process complete callback.
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA module.
  * @retval None
  */
static void SAI_DMARxCplt(DMA_HandleTypeDef *hdma)      /* 静态函数 */
{                                                       /* 代码块开始 */
  SAI_HandleTypeDef *hsai = (SAI_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;  /* 赋值或初始化 */

  if (hdma->Init.Mode != DMA_CIRCULAR)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable Rx DMA Request */
    hsai->Instance->CR1 &= (uint32_t)(~SAI_xCR1_DMAEN);  /* 赋值或初始化 */
    hsai->XferCount = 0U;                               /* 赋值或初始化 */

    /* Stop the interrupts error handling */
    __HAL_SAI_DISABLE_IT(hsai, SAI_InterruptFlag(hsai, SAI_MODE_DMA));  /* HAL辅助宏/函数 */

    hsai->State = HAL_SAI_STATE_READY;                  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
#if (USE_HAL_SAI_REGISTER_CALLBACKS == 1)               /* 条件编译 */
  hsai->RxCpltCallback(hsai);                           /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_SAI_RxCpltCallback(hsai);                         /* HAL库函数调用 */
#endif /* USE_HAL_SAI_REGISTER_CALLBACKS */
}                                                       /* 代码块结束 */

/**
  * @brief DMA SAI receive process half complete callback
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA module.
  * @retval None
  */
static void SAI_DMARxHalfCplt(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  SAI_HandleTypeDef *hsai = (SAI_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;  /* 赋值或初始化 */

#if (USE_HAL_SAI_REGISTER_CALLBACKS == 1)               /* 条件编译 */
  hsai->RxHalfCpltCallback(hsai);                       /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_SAI_RxHalfCpltCallback(hsai);                     /* HAL库函数调用 */
#endif /* USE_HAL_SAI_REGISTER_CALLBACKS */
}                                                       /* 代码块结束 */

/**
  * @brief DMA SAI communication error callback.
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA module.
  * @retval None
  */
static void SAI_DMAError(DMA_HandleTypeDef *hdma)       /* 静态函数 */
{                                                       /* 代码块开始 */
  SAI_HandleTypeDef *hsai = (SAI_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;  /* 赋值或初始化 */

  /* Set SAI error code */
  hsai->ErrorCode |= HAL_SAI_ERROR_DMA;                 /* 赋值或初始化 */

  if ((hsai->hdmatx->ErrorCode == HAL_DMA_ERROR_TE) || (hsai->hdmarx->ErrorCode == HAL_DMA_ERROR_TE))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable the SAI DMA request */
    hsai->Instance->CR1 &= ~SAI_xCR1_DMAEN;             /* 赋值或初始化 */

    /* Disable SAI peripheral */
    SAI_Disable(hsai);                                  /* 执行语句 */

    /* Set the SAI state ready to be able to start again the process */
    hsai->State = HAL_SAI_STATE_READY;                  /* 赋值或初始化 */

    /* Initialize XferCount */
    hsai->XferCount = 0U;                               /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  /* SAI error Callback */
#if (USE_HAL_SAI_REGISTER_CALLBACKS == 1)               /* 条件编译 */
  hsai->ErrorCallback(hsai);                            /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_SAI_ErrorCallback(hsai);                          /* HAL库函数调用 */
#endif /* USE_HAL_SAI_REGISTER_CALLBACKS */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA SAI Abort callback.
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA module.
  * @retval None
  */
static void SAI_DMAAbort(DMA_HandleTypeDef *hdma)       /* 静态函数 */
{                                                       /* 代码块开始 */
  SAI_HandleTypeDef *hsai = (SAI_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;  /* 赋值或初始化 */

  /* Disable DMA request */
  hsai->Instance->CR1 &= ~SAI_xCR1_DMAEN;               /* 赋值或初始化 */

  /* Disable all interrupts and clear all flags */
  hsai->Instance->IMR = 0U;                             /* 赋值或初始化 */
  hsai->Instance->CLRFR = 0xFFFFFFFFU;                  /* 赋值或初始化 */

  if (hsai->ErrorCode != HAL_SAI_ERROR_WCKCFG)          /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable SAI peripheral */
    SAI_Disable(hsai);                                  /* 执行语句 */

    /* Flush the fifo */
    SET_BIT(hsai->Instance->CR2, SAI_xCR2_FFLUSH);      /* 执行语句 */
  }                                                     /* 代码块结束 */
  /* Set the SAI state to ready to be able to start again the process */
  hsai->State = HAL_SAI_STATE_READY;                    /* 赋值或初始化 */

  /* Initialize XferCount */
  hsai->XferCount = 0U;                                 /* 赋值或初始化 */

  /* SAI error Callback */
#if (USE_HAL_SAI_REGISTER_CALLBACKS == 1)               /* 条件编译 */
  hsai->ErrorCallback(hsai);                            /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_SAI_ErrorCallback(hsai);                          /* HAL库函数调用 */
#endif /* USE_HAL_SAI_REGISTER_CALLBACKS */
}                                                       /* 代码块结束 */

/**
  * @}
  */

#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx || STM32F446xx || STM32F469xx || STM32F479xx || STM32F413xx || STM32F423xx */
#endif /* HAL_SAI_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
