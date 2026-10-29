/* 文件: stm32f4xx_hal_sd.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_sd.c
  * @author  MCD Application Team
  * @brief   SD card HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Secure Digital (SD) peripheral:
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
    This driver implements a high level communication layer for read and write from/to
    this memory. The needed STM32 hardware resources (SDIO and GPIO) are performed by
    the user in HAL_SD_MspInit() function (MSP layer).
    Basically, the MSP layer configuration should be the same as we provide in the
    examples.
    You can easily tailor this configuration according to hardware resources.

  [..]
    This driver is a generic layered driver for SDIO memories which uses the HAL
    SDIO driver functions to interface with SD and uSD cards devices.
    It is used as follows:

    (#)Initialize the SDIO low level resources by implementing the HAL_SD_MspInit() API:
        (##) Enable the SDIO interface clock using __HAL_RCC_SDIO_CLK_ENABLE();
        (##) SDIO pins configuration for SD card
            (+++) Enable the clock for the SDIO GPIOs using the functions __HAL_RCC_GPIOx_CLK_ENABLE();
            (+++) Configure these SDIO pins as alternate function pull-up using HAL_GPIO_Init()
                  and according to your pin assignment;
        (##) DMA configuration if you need to use DMA process (HAL_SD_ReadBlocks_DMA()
             and HAL_SD_WriteBlocks_DMA() APIs).
            (+++) Enable the DMAx interface clock using __HAL_RCC_DMAx_CLK_ENABLE();
            (+++) Configure the DMA using the function HAL_DMA_Init() with predeclared and filled.
        (##) NVIC configuration if you need to use interrupt process when using DMA transfer.
            (+++) Configure the SDIO and DMA interrupt priorities using functions
                  HAL_NVIC_SetPriority(); DMA priority is superior to SDIO's priority
            (+++) Enable the NVIC DMA and SDIO IRQs using function HAL_NVIC_EnableIRQ()
            (+++) SDIO interrupts are managed using the macros __HAL_SD_ENABLE_IT()
                  and __HAL_SD_DISABLE_IT() inside the communication process.
            (+++) SDIO interrupts pending bits are managed using the macros __HAL_SD_GET_IT()
                  and __HAL_SD_CLEAR_IT()
        (##) NVIC configuration if you need to use interrupt process (HAL_SD_ReadBlocks_IT()
             and HAL_SD_WriteBlocks_IT() APIs).
            (+++) Configure the SDIO interrupt priorities using function HAL_NVIC_SetPriority();
            (+++) Enable the NVIC SDIO IRQs using function HAL_NVIC_EnableIRQ()
            (+++) SDIO interrupts are managed using the macros __HAL_SD_ENABLE_IT()
                  and __HAL_SD_DISABLE_IT() inside the communication process.
            (+++) SDIO interrupts pending bits are managed using the macros __HAL_SD_GET_IT()
                  and __HAL_SD_CLEAR_IT()
    (#) At this stage, you can perform SD read/write/erase operations after SD card initialization


  *** SD Card Initialization and configuration ***
  ================================================
  [..]
    To initialize the SD Card, use the HAL_SD_Init() function. It Initializes
    SDIO Peripheral(STM32 side) and the SD Card, and put it into StandBy State (Ready for data transfer).
    This function provide the following operations:

    (#) Apply the SD Card initialization process at 400KHz and check the SD Card
        type (Standard Capacity or High Capacity). You can change or adapt this
        frequency by adjusting the "ClockDiv" field.
        The SD Card frequency (SDIO_CK) is computed as follows:

           SDIO_CK = SDIOCLK / (ClockDiv + 2)

        In initialization mode and according to the SD Card standard,
        make sure that the SDIO_CK frequency doesn't exceed 400KHz.

        This phase of initialization is done through SDIO_Init() and
        SDIO_PowerState_ON() SDIO low level APIs.

    (#) Initialize the SD card. The API used is HAL_SD_InitCard().
        This phase allows the card initialization and identification
        and check the SD Card type (Standard Capacity or High Capacity)
        The initialization flow is compatible with SD standard.

        This API (HAL_SD_InitCard()) could be used also to reinitialize the card in case
        of plug-off plug-in.

    (#) Configure the SD Card Data transfer frequency. You can change or adapt this
        frequency by adjusting the "ClockDiv" field.
        In transfer mode and according to the SD Card standard, make sure that the
        SDIO_CK frequency doesn't exceed 25MHz and 50MHz in High-speed mode switch.
        To be able to use a frequency higher than 24MHz, you should use the SDIO
        peripheral in bypass mode. Refer to the corresponding reference manual
        for more details.

    (#) Select the corresponding SD Card according to the address read with the step 2.

    (#) Configure the SD Card in wide bus mode: 4-bits data.

  *** SD Card Read operation ***
  ==============================
  [..]
    (+) You can read from SD card in polling mode by using function HAL_SD_ReadBlocks().
        This function support only 512-bytes block length (the block size should be
        chosen as 512 bytes).
        You can choose either one block read operation or multiple block read operation
        by adjusting the "NumberOfBlocks" parameter.
        After this, you have to ensure that the transfer is done correctly. The check is done
        through HAL_SD_GetCardState() function for SD card state.

    (+) You can read from SD card in DMA mode by using function HAL_SD_ReadBlocks_DMA().
        This function support only 512-bytes block length (the block size should be
        chosen as 512 bytes).
        You can choose either one block read operation or multiple block read operation
        by adjusting the "NumberOfBlocks" parameter.
        After this, you have to ensure that the transfer is done correctly. The check is done
        through HAL_SD_GetCardState() function for SD card state.
        You could also check the DMA transfer process through the SD Rx interrupt event.

    (+) You can read from SD card in Interrupt mode by using function HAL_SD_ReadBlocks_IT().
        This function support only 512-bytes block length (the block size should be
        chosen as 512 bytes).
        You can choose either one block read operation or multiple block read operation
        by adjusting the "NumberOfBlocks" parameter.
        After this, you have to ensure that the transfer is done correctly. The check is done
        through HAL_SD_GetCardState() function for SD card state.
        You could also check the IT transfer process through the SD Rx interrupt event.

  *** SD Card Write operation ***
  ===============================
  [..]
    (+) You can write to SD card in polling mode by using function HAL_SD_WriteBlocks().
        This function support only 512-bytes block length (the block size should be
        chosen as 512 bytes).
        You can choose either one block read operation or multiple block read operation
        by adjusting the "NumberOfBlocks" parameter.
        After this, you have to ensure that the transfer is done correctly. The check is done
        through HAL_SD_GetCardState() function for SD card state.

    (+) You can write to SD card in DMA mode by using function HAL_SD_WriteBlocks_DMA().
        This function support only 512-bytes block length (the block size should be
        chosen as 512 bytes).
        You can choose either one block read operation or multiple block read operation
        by adjusting the "NumberOfBlocks" parameter.
        After this, you have to ensure that the transfer is done correctly. The check is done
        through HAL_SD_GetCardState() function for SD card state.
        You could also check the DMA transfer process through the SD Tx interrupt event.

    (+) You can write to SD card in Interrupt mode by using function HAL_SD_WriteBlocks_IT().
        This function support only 512-bytes block length (the block size should be
        chosen as 512 bytes).
        You can choose either one block read operation or multiple block read operation
        by adjusting the "NumberOfBlocks" parameter.
        After this, you have to ensure that the transfer is done correctly. The check is done
        through HAL_SD_GetCardState() function for SD card state.
        You could also check the IT transfer process through the SD Tx interrupt event.

  *** SD card status ***
  ======================
  [..]
    (+) The SD Status contains status bits that are related to the SD Memory
        Card proprietary features. To get SD card status use the HAL_SD_GetCardStatus().

  *** SD card information ***
  ===========================
  [..]
    (+) To get SD card information, you can use the function HAL_SD_GetCardInfo().
        It returns useful information about the SD card such as block size, card type,
        block number ...

  *** SD card CSD register ***
  ============================
    (+) The HAL_SD_GetCardCSD() API allows to get the parameters of the CSD register.
        Some of the CSD parameters are useful for card initialization and identification.

  *** SD card CID register ***
  ============================
    (+) The HAL_SD_GetCardCID() API allows to get the parameters of the CID register.
        Some of the CSD parameters are useful for card initialization and identification.

  *** SD HAL driver macros list ***
  ==================================
  [..]
    Below the list of most used macros in SD HAL driver.

    (+) __HAL_SD_ENABLE : Enable the SD device
    (+) __HAL_SD_DISABLE : Disable the SD device
    (+) __HAL_SD_DMA_ENABLE: Enable the SDIO DMA transfer
    (+) __HAL_SD_DMA_DISABLE: Disable the SDIO DMA transfer
    (+) __HAL_SD_ENABLE_IT: Enable the SD device interrupt
    (+) __HAL_SD_DISABLE_IT: Disable the SD device interrupt
    (+) __HAL_SD_GET_FLAG:Check whether the specified SD flag is set or not
    (+) __HAL_SD_CLEAR_FLAG: Clear the SD's pending flags

    (@) You can refer to the SD HAL driver header file for more useful macros

  *** Callback registration ***
  =============================================
  [..]
    The compilation define USE_HAL_SD_REGISTER_CALLBACKS when set to 1
    allows the user to configure dynamically the driver callbacks.

    Use Functions @ref HAL_SD_RegisterCallback() to register a user callback,
    it allows to register following callbacks:
      (+) TxCpltCallback : callback when a transmission transfer is completed.
      (+) RxCpltCallback : callback when a reception transfer is completed.
      (+) ErrorCallback : callback when error occurs.
      (+) AbortCpltCallback : callback when abort is completed.
      (+) MspInitCallback    : SD MspInit.
      (+) MspDeInitCallback  : SD MspDeInit.
    This function takes as parameters the HAL peripheral handle, the Callback ID
    and a pointer to the user callback function.

    Use function @ref HAL_SD_UnRegisterCallback() to reset a callback to the default
    weak (surcharged) function. It allows to reset following callbacks:
      (+) TxCpltCallback : callback when a transmission transfer is completed.
      (+) RxCpltCallback : callback when a reception transfer is completed.
      (+) ErrorCallback : callback when error occurs.
      (+) AbortCpltCallback : callback when abort is completed.
      (+) MspInitCallback    : SD MspInit.
      (+) MspDeInitCallback  : SD MspDeInit.
    This function) takes as parameters the HAL peripheral handle and the Callback ID.

    By default, after the @ref HAL_SD_Init and if the state is HAL_SD_STATE_RESET
    all callbacks are reset to the corresponding legacy weak (surcharged) functions.
    Exception done for MspInit and MspDeInit callbacks that are respectively
    reset to the legacy weak (surcharged) functions in the @ref HAL_SD_Init
    and @ref  HAL_SD_DeInit only when these callbacks are null (not registered beforehand).
    If not, MspInit or MspDeInit are not null, the @ref HAL_SD_Init and @ref HAL_SD_DeInit
    keep and use the user MspInit/MspDeInit callbacks (registered beforehand)

    Callbacks can be registered/unregistered in READY state only.
    Exception done for MspInit/MspDeInit callbacks that can be registered/unregistered
    in READY or RESET state, thus registered (user) MspInit/DeInit callbacks can be used
    during the Init/DeInit.
    In that case first register the MspInit/MspDeInit user callbacks
    using @ref HAL_SD_RegisterCallback before calling @ref HAL_SD_DeInit
    or @ref HAL_SD_Init function.

    When The compilation define USE_HAL_SD_REGISTER_CALLBACKS is set to 0 or
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
  *                       opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"                              /* 包含 STM32/HAL 头文件 */

#if defined(SDIO)                                       /* 条件编译 */

/** @addtogroup STM32F4xx_HAL_Driver
  * @{
  */

/** @addtogroup SD
  * @{
  */

#ifdef HAL_SD_MODULE_ENABLED                            /* 条件编译 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @addtogroup SD_Private_Defines
  * @{
  */

/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/** @defgroup SD_Private_Functions SD Private Functions
  * @{
  */
static uint32_t SD_InitCard(SD_HandleTypeDef *hsd);     /* 静态变量/函数 */
static uint32_t SD_PowerON(SD_HandleTypeDef *hsd);      /* 静态变量/函数 */
static uint32_t SD_SendSDStatus(SD_HandleTypeDef *hsd, uint32_t *pSDstatus);  /* 静态变量/函数 */
static uint32_t SD_SendStatus(SD_HandleTypeDef *hsd, uint32_t *pCardStatus);  /* 静态变量/函数 */
static uint32_t SD_WideBus_Enable(SD_HandleTypeDef *hsd);  /* 静态变量/函数 */
static uint32_t SD_WideBus_Disable(SD_HandleTypeDef *hsd);  /* 静态变量/函数 */
static uint32_t SD_FindSCR(SD_HandleTypeDef *hsd, uint32_t *pSCR);  /* 静态变量/函数 */
static void SD_PowerOFF(SD_HandleTypeDef *hsd);         /* 静态函数 */
static void SD_Write_IT(SD_HandleTypeDef *hsd);         /* 静态函数 */
static void SD_Read_IT(SD_HandleTypeDef *hsd);          /* 静态函数 */
static void SD_DMATransmitCplt(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void SD_DMAReceiveCplt(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void SD_DMAError(DMA_HandleTypeDef *hdma);       /* 静态函数 */
static void SD_DMATxAbort(DMA_HandleTypeDef *hdma);     /* 静态函数 */
static void SD_DMARxAbort(DMA_HandleTypeDef *hdma);     /* 静态函数 */
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup SD_Exported_Functions
  * @{
  */

/** @addtogroup SD_Exported_Functions_Group1
 *  @brief   Initialization and de-initialization functions
 *
@verbatim
  ==============================================================================
          ##### Initialization and de-initialization functions #####
  ==============================================================================
  [..]
    This section provides functions allowing to initialize/de-initialize the SD
    card device to be ready for use.

@endverbatim
  * @{
  */

/**
  * @brief  Initializes the SD according to the specified parameters in the
            SD_HandleTypeDef and create the associated handle.
  * @param  hsd: Pointer to the SD handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SD_Init(SD_HandleTypeDef *hsd)    /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the SD handle allocation */
  if(hsd == NULL)                                       /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the parameters */
  assert_param(IS_SDIO_ALL_INSTANCE(hsd->Instance));    /* 执行语句 */
  assert_param(IS_SDIO_CLOCK_EDGE(hsd->Init.ClockEdge));  /* 执行语句 */
  assert_param(IS_SDIO_CLOCK_BYPASS(hsd->Init.ClockBypass));  /* 执行语句 */
  assert_param(IS_SDIO_CLOCK_POWER_SAVE(hsd->Init.ClockPowerSave));  /* 执行语句 */
  assert_param(IS_SDIO_BUS_WIDE(hsd->Init.BusWide));    /* 执行语句 */
  assert_param(IS_SDIO_HARDWARE_FLOW_CONTROL(hsd->Init.HardwareFlowControl));  /* 执行语句 */
  assert_param(IS_SDIO_CLKDIV(hsd->Init.ClockDiv));     /* 执行语句 */

  if(hsd->State == HAL_SD_STATE_RESET)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Allocate lock resource and initialize it */
    hsd->Lock = HAL_UNLOCKED;                           /* 赋值或初始化 */
#if defined (USE_HAL_SD_REGISTER_CALLBACKS) && (USE_HAL_SD_REGISTER_CALLBACKS == 1U)  /* 条件编译 */
    /* Reset Callback pointers in HAL_SD_STATE_RESET only */
    hsd->TxCpltCallback    = HAL_SD_TxCpltCallback;     /* 赋值或初始化 */
    hsd->RxCpltCallback    = HAL_SD_RxCpltCallback;     /* 赋值或初始化 */
    hsd->ErrorCallback     = HAL_SD_ErrorCallback;      /* 赋值或初始化 */
    hsd->AbortCpltCallback = HAL_SD_AbortCallback;      /* 赋值或初始化 */

    if(hsd->MspInitCallback == NULL)                    /* 条件判断 */
    {                                                   /* 代码块开始 */
      hsd->MspInitCallback = HAL_SD_MspInit;            /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Init the low level hardware */
    hsd->MspInitCallback(hsd);                          /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /* Init the low level hardware : GPIO, CLOCK, CORTEX...etc */
    HAL_SD_MspInit(hsd);                                /* HAL库函数调用 */
#endif /* USE_HAL_SD_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */

  hsd->State = HAL_SD_STATE_BUSY;                       /* 赋值或初始化 */

  /* Initialize the Card parameters */
  if (HAL_SD_InitCard(hsd) != HAL_OK)                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Initialize the error code */
  hsd->ErrorCode = HAL_SD_ERROR_NONE;                   /* 赋值或初始化 */

  /* Initialize the SD operation */
  hsd->Context = SD_CONTEXT_NONE;                       /* 赋值或初始化 */

  /* Initialize the SD state */
  hsd->State = HAL_SD_STATE_READY;                      /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initializes the SD Card.
  * @param  hsd: Pointer to SD handle
  * @note   This function initializes the SD card. It could be used when a card
            re-initialization is needed.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SD_InitCard(SD_HandleTypeDef *hsd)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t errorstate;                                  /* 32位无符号变量/函数 */
  HAL_StatusTypeDef status;                             /* HAL状态类型函数 */
  SD_InitTypeDef Init;                                  /* 执行语句 */

  /* Default SDIO peripheral configuration for SD card initialization */
  Init.ClockEdge           = SDIO_CLOCK_EDGE_RISING;    /* 赋值或初始化 */
  Init.ClockBypass         = SDIO_CLOCK_BYPASS_DISABLE;  /* 赋值或初始化 */
  Init.ClockPowerSave      = SDIO_CLOCK_POWER_SAVE_DISABLE;  /* 赋值或初始化 */
  Init.BusWide             = SDIO_BUS_WIDE_1B;          /* 赋值或初始化 */
  Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;  /* 赋值或初始化 */
  Init.ClockDiv            = SDIO_INIT_CLK_DIV;         /* 赋值或初始化 */

  /* Initialize SDIO peripheral interface with default configuration */
  status = SDIO_Init(hsd->Instance, Init);              /* 赋值或初始化 */
  if(status != HAL_OK)                                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Disable SDIO Clock */
  __HAL_SD_DISABLE(hsd);                                /* HAL辅助宏/函数 */

  /* Set Power State to ON */
  (void)SDIO_PowerState_ON(hsd->Instance);              /* 执行语句 */

  /* Enable SDIO Clock */
  __HAL_SD_ENABLE(hsd);                                 /* HAL辅助宏/函数 */

  /* Identify card operating voltage */
  errorstate = SD_PowerON(hsd);                         /* 赋值或初始化 */
  if(errorstate != HAL_SD_ERROR_NONE)                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    hsd->State = HAL_SD_STATE_READY;                    /* 赋值或初始化 */
    hsd->ErrorCode |= errorstate;                       /* 赋值或初始化 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Card initialization */
  errorstate = SD_InitCard(hsd);                        /* 赋值或初始化 */
  if(errorstate != HAL_SD_ERROR_NONE)                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    hsd->State = HAL_SD_STATE_READY;                    /* 赋值或初始化 */
    hsd->ErrorCode |= errorstate;                       /* 赋值或初始化 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Set Block Size for Card */
  errorstate = SDMMC_CmdBlockLength(hsd->Instance, BLOCKSIZE);  /* 赋值或初始化 */
  if(errorstate != HAL_SD_ERROR_NONE)                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Clear all the static flags */
    __HAL_SD_CLEAR_FLAG(hsd, SDIO_STATIC_FLAGS);        /* HAL辅助宏/函数 */
    hsd->ErrorCode |= errorstate;                       /* 赋值或初始化 */
    hsd->State = HAL_SD_STATE_READY;                    /* 赋值或初始化 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  De-Initializes the SD card.
  * @param  hsd: Pointer to SD handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SD_DeInit(SD_HandleTypeDef *hsd)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the SD handle allocation */
  if(hsd == NULL)                                       /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the parameters */
  assert_param(IS_SDIO_ALL_INSTANCE(hsd->Instance));    /* 执行语句 */

  hsd->State = HAL_SD_STATE_BUSY;                       /* 赋值或初始化 */

  /* Set SD power state to off */
  SD_PowerOFF(hsd);                                     /* 执行语句 */

#if defined (USE_HAL_SD_REGISTER_CALLBACKS) && (USE_HAL_SD_REGISTER_CALLBACKS == 1U)  /* 条件编译 */
  if(hsd->MspDeInitCallback == NULL)                    /* 条件判断 */
  {                                                     /* 代码块开始 */
    hsd->MspDeInitCallback = HAL_SD_MspDeInit;          /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* DeInit the low level hardware */
  hsd->MspDeInitCallback(hsd);                          /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* De-Initialize the MSP layer */
  HAL_SD_MspDeInit(hsd);                                /* HAL库函数调用 */
#endif /* USE_HAL_SD_REGISTER_CALLBACKS */

  hsd->ErrorCode = HAL_SD_ERROR_NONE;                   /* 赋值或初始化 */
  hsd->State = HAL_SD_STATE_RESET;                      /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */


/**
  * @brief  Initializes the SD MSP.
  * @param  hsd: Pointer to SD handle
  * @retval None
  */
__weak void HAL_SD_MspInit(SD_HandleTypeDef *hsd)       /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsd);                                          /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SD_MspInit could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  De-Initialize SD MSP.
  * @param  hsd: Pointer to SD handle
  * @retval None
  */
__weak void HAL_SD_MspDeInit(SD_HandleTypeDef *hsd)     /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsd);                                          /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SD_MspDeInit could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @addtogroup SD_Exported_Functions_Group2
 *  @brief   Data transfer functions
 *
@verbatim
  ==============================================================================
                        ##### IO operation functions #####
  ==============================================================================
  [..]
    This subsection provides a set of functions allowing to manage the data
    transfer from/to SD card.

@endverbatim
  * @{
  */

/**
  * @brief  Reads block(s) from a specified address in a card. The Data transfer
  *         is managed by polling mode.
  * @note   This API should be followed by a check on the card state through
  *         HAL_SD_GetCardState().
  * @param  hsd: Pointer to SD handle
  * @param  pData: pointer to the buffer that will contain the received data
  * @param  BlockAdd: Block Address from where data is to be read
  * @param  NumberOfBlocks: Number of SD blocks to read
  * @param  Timeout: Specify timeout value
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SD_ReadBlocks(SD_HandleTypeDef *hsd, uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  SDIO_DataInitTypeDef config;                          /* 执行语句 */
  uint32_t errorstate;                                  /* 32位无符号变量/函数 */
  uint32_t tickstart = HAL_GetTick();                   /* 32位无符号变量/函数 */
  uint32_t count, data, dataremaining;                  /* 32位无符号变量/函数 */
  uint32_t add = BlockAdd;                              /* 32位无符号变量/函数 */
  uint8_t *tempbuff = pData;                            /* 8位无符号变量/函数 */

  if(NULL == pData)                                     /* 条件判断 */
  {                                                     /* 代码块开始 */
    hsd->ErrorCode |= HAL_SD_ERROR_PARAM;               /* 赋值或初始化 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  if(hsd->State == HAL_SD_STATE_READY)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hsd->ErrorCode = HAL_SD_ERROR_NONE;                 /* 赋值或初始化 */

    if((add + NumberOfBlocks) > (hsd->SdCard.LogBlockNbr))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hsd->ErrorCode |= HAL_SD_ERROR_ADDR_OUT_OF_RANGE;  /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    hsd->State = HAL_SD_STATE_BUSY;                     /* 赋值或初始化 */

    /* Initialize data control register */
    hsd->Instance->DCTRL = 0U;                          /* 赋值或初始化 */

    if(hsd->SdCard.CardType != CARD_SDHC_SDXC)          /* 条件判断 */
    {                                                   /* 代码块开始 */
      add *= 512U;                                      /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Configure the SD DPSM (Data Path State Machine) */
    config.DataTimeOut   = SDMMC_DATATIMEOUT;           /* 赋值或初始化 */
    config.DataLength    = NumberOfBlocks * BLOCKSIZE;  /* 赋值或初始化 */
    config.DataBlockSize = SDIO_DATABLOCK_SIZE_512B;    /* 赋值或初始化 */
    config.TransferDir   = SDIO_TRANSFER_DIR_TO_SDIO;   /* 赋值或初始化 */
    config.TransferMode  = SDIO_TRANSFER_MODE_BLOCK;    /* 赋值或初始化 */
    config.DPSM          = SDIO_DPSM_ENABLE;            /* 赋值或初始化 */
    (void)SDIO_ConfigData(hsd->Instance, &config);      /* 执行语句 */

    /* Read block(s) in polling mode */
    if(NumberOfBlocks > 1U)                             /* 条件判断 */
    {                                                   /* 代码块开始 */
      hsd->Context = SD_CONTEXT_READ_MULTIPLE_BLOCK;    /* 赋值或初始化 */

      /* Read Multi Block command */
      errorstate = SDMMC_CmdReadMultiBlock(hsd->Instance, add);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hsd->Context = SD_CONTEXT_READ_SINGLE_BLOCK;      /* 赋值或初始化 */

      /* Read Single Block command */
      errorstate = SDMMC_CmdReadSingleBlock(hsd->Instance, add);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    if(errorstate != HAL_SD_ERROR_NONE)                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear all the static flags */
      __HAL_SD_CLEAR_FLAG(hsd, SDIO_STATIC_FLAGS);      /* HAL辅助宏/函数 */
      hsd->ErrorCode |= errorstate;                     /* 赋值或初始化 */
      hsd->State = HAL_SD_STATE_READY;                  /* 赋值或初始化 */
      hsd->Context = SD_CONTEXT_NONE;                   /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Poll on SDIO flags */
    dataremaining = config.DataLength;                  /* 赋值或初始化 */
#if defined(SDIO_STA_STBITERR)                          /* 条件编译 */
    while(!__HAL_SD_GET_FLAG(hsd, SDIO_FLAG_RXOVERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DATAEND | SDIO_FLAG_STBITERR))  /* while循环 */
#else /* SDIO_STA_STBITERR not defined */
    while(!__HAL_SD_GET_FLAG(hsd, SDIO_FLAG_RXOVERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DATAEND))  /* while循环 */
#endif /* SDIO_STA_STBITERR */
    {                                                   /* 代码块开始 */
      if(__HAL_SD_GET_FLAG(hsd, SDIO_FLAG_RXFIFOHF) && (dataremaining > 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Read data from SDIO Rx FIFO */
        for(count = 0U; count < 8U; count++)            /* for循环 */
        {                                               /* 代码块开始 */
          data = SDIO_ReadFIFO(hsd->Instance);          /* 赋值或初始化 */
          *tempbuff = (uint8_t)(data & 0xFFU);          /* 赋值或初始化 */
          tempbuff++;                                   /* 执行语句 */
          dataremaining--;                              /* 执行语句 */
          *tempbuff = (uint8_t)((data >> 8U) & 0xFFU);  /* 赋值或初始化 */
          tempbuff++;                                   /* 执行语句 */
          dataremaining--;                              /* 执行语句 */
          *tempbuff = (uint8_t)((data >> 16U) & 0xFFU);  /* 赋值或初始化 */
          tempbuff++;                                   /* 执行语句 */
          dataremaining--;                              /* 执行语句 */
          *tempbuff = (uint8_t)((data >> 24U) & 0xFFU);  /* 赋值或初始化 */
          tempbuff++;                                   /* 执行语句 */
          dataremaining--;                              /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */

      if(((HAL_GetTick()-tickstart) >=  Timeout) || (Timeout == 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Clear all the static flags */
        __HAL_SD_CLEAR_FLAG(hsd, SDIO_STATIC_FLAGS);    /* HAL辅助宏/函数 */
        hsd->ErrorCode |= HAL_SD_ERROR_TIMEOUT;         /* 赋值或初始化 */
        hsd->State= HAL_SD_STATE_READY;                 /* 赋值或初始化 */
        hsd->Context = SD_CONTEXT_NONE;                 /* 赋值或初始化 */
        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Send stop transmission command in case of multiblock read */
    if(__HAL_SD_GET_FLAG(hsd, SDIO_FLAG_DATAEND) && (NumberOfBlocks > 1U))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      if(hsd->SdCard.CardType != CARD_SECURED)          /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Send stop transmission command */
        errorstate = SDMMC_CmdStopTransfer(hsd->Instance);  /* 赋值或初始化 */
        if(errorstate != HAL_SD_ERROR_NONE)             /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Clear all the static flags */
          __HAL_SD_CLEAR_FLAG(hsd, SDIO_STATIC_FLAGS);  /* HAL辅助宏/函数 */
          hsd->ErrorCode |= errorstate;                 /* 赋值或初始化 */
          hsd->State = HAL_SD_STATE_READY;              /* 赋值或初始化 */
          hsd->Context = SD_CONTEXT_NONE;               /* 赋值或初始化 */
          return HAL_ERROR;                             /* 返回值 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Get error state */
    if(__HAL_SD_GET_FLAG(hsd, SDIO_FLAG_DTIMEOUT))      /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear all the static flags */
      __HAL_SD_CLEAR_FLAG(hsd, SDIO_STATIC_FLAGS);      /* HAL辅助宏/函数 */
      hsd->ErrorCode |= HAL_SD_ERROR_DATA_TIMEOUT;      /* 赋值或初始化 */
      hsd->State = HAL_SD_STATE_READY;                  /* 赋值或初始化 */
      hsd->Context = SD_CONTEXT_NONE;                   /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
    else if(__HAL_SD_GET_FLAG(hsd, SDIO_FLAG_DCRCFAIL))  /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* Clear all the static flags */
      __HAL_SD_CLEAR_FLAG(hsd, SDIO_STATIC_FLAGS);      /* HAL辅助宏/函数 */
      hsd->ErrorCode |= HAL_SD_ERROR_DATA_CRC_FAIL;     /* 赋值或初始化 */
      hsd->State = HAL_SD_STATE_READY;                  /* 赋值或初始化 */
      hsd->Context = SD_CONTEXT_NONE;                   /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
    else if(__HAL_SD_GET_FLAG(hsd, SDIO_FLAG_RXOVERR))  /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* Clear all the static flags */
      __HAL_SD_CLEAR_FLAG(hsd, SDIO_STATIC_FLAGS);      /* HAL辅助宏/函数 */
      hsd->ErrorCode |= HAL_SD_ERROR_RX_OVERRUN;        /* 赋值或初始化 */
      hsd->State = HAL_SD_STATE_READY;                  /* 赋值或初始化 */
      hsd->Context = SD_CONTEXT_NONE;                   /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Nothing to do */
    }                                                   /* 代码块结束 */

    /* Empty FIFO if there is still any data */
    while ((__HAL_SD_GET_FLAG(hsd, SDIO_FLAG_RXDAVL)) && (dataremaining > 0U))  /* while循环 */
    {                                                   /* 代码块开始 */
      data = SDIO_ReadFIFO(hsd->Instance);              /* 赋值或初始化 */
      *tempbuff = (uint8_t)(data & 0xFFU);              /* 赋值或初始化 */
      tempbuff++;                                       /* 执行语句 */
      dataremaining--;                                  /* 执行语句 */
      *tempbuff = (uint8_t)((data >> 8U) & 0xFFU);      /* 赋值或初始化 */
      tempbuff++;                                       /* 执行语句 */
      dataremaining--;                                  /* 执行语句 */
      *tempbuff = (uint8_t)((data >> 16U) & 0xFFU);     /* 赋值或初始化 */
      tempbuff++;                                       /* 执行语句 */
      dataremaining--;                                  /* 执行语句 */
      *tempbuff = (uint8_t)((data >> 24U) & 0xFFU);     /* 赋值或初始化 */
      tempbuff++;                                       /* 执行语句 */
      dataremaining--;                                  /* 执行语句 */

      if(((HAL_GetTick()-tickstart) >=  Timeout) || (Timeout == 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Clear all the static flags */
        __HAL_SD_CLEAR_FLAG(hsd, SDIO_STATIC_FLAGS);    /* HAL辅助宏/函数 */
        hsd->ErrorCode |= HAL_SD_ERROR_TIMEOUT;         /* 赋值或初始化 */
        hsd->State= HAL_SD_STATE_READY;                 /* 赋值或初始化 */
        hsd->Context = SD_CONTEXT_NONE;                 /* 赋值或初始化 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Clear all the static flags */
    __HAL_SD_CLEAR_FLAG(hsd, SDIO_STATIC_DATA_FLAGS);   /* HAL辅助宏/函数 */

    hsd->State = HAL_SD_STATE_READY;                    /* 赋值或初始化 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    hsd->ErrorCode |= HAL_SD_ERROR_BUSY;                /* 赋值或初始化 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Allows to write block(s) to a specified address in a card. The Data
  *         transfer is managed by polling mode.
  * @note   This API should be followed by a check on the card state through
  *         HAL_SD_GetCardState().
  * @param  hsd: Pointer to SD handle
  * @param  pData: pointer to the buffer that will contain the data to transmit
  * @param  BlockAdd: Block Address where data will be written
  * @param  NumberOfBlocks: Number of SD blocks to write
  * @param  Timeout: Specify timeout value
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SD_WriteBlocks(SD_HandleTypeDef *hsd, uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  SDIO_DataInitTypeDef config;                          /* 执行语句 */
  uint32_t errorstate;                                  /* 32位无符号变量/函数 */
  uint32_t tickstart = HAL_GetTick();                   /* 32位无符号变量/函数 */
  uint32_t count, data, dataremaining;                  /* 32位无符号变量/函数 */
  uint32_t add = BlockAdd;                              /* 32位无符号变量/函数 */
  uint8_t *tempbuff = pData;                            /* 8位无符号变量/函数 */

  if(NULL == pData)                                     /* 条件判断 */
  {                                                     /* 代码块开始 */
    hsd->ErrorCode |= HAL_SD_ERROR_PARAM;               /* 赋值或初始化 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  if(hsd->State == HAL_SD_STATE_READY)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hsd->ErrorCode = HAL_SD_ERROR_NONE;                 /* 赋值或初始化 */

    if((add + NumberOfBlocks) > (hsd->SdCard.LogBlockNbr))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hsd->ErrorCode |= HAL_SD_ERROR_ADDR_OUT_OF_RANGE;  /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    hsd->State = HAL_SD_STATE_BUSY;                     /* 赋值或初始化 */

    /* Initialize data control register */
    hsd->Instance->DCTRL = 0U;                          /* 赋值或初始化 */

    if(hsd->SdCard.CardType != CARD_SDHC_SDXC)          /* 条件判断 */
    {                                                   /* 代码块开始 */
      add *= 512U;                                      /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Configure the SD DPSM (Data Path State Machine) */
    config.DataTimeOut   = SDMMC_DATATIMEOUT;           /* 赋值或初始化 */
    config.DataLength    = NumberOfBlocks * BLOCKSIZE;  /* 赋值或初始化 */
    config.DataBlockSize = SDIO_DATABLOCK_SIZE_512B;    /* 赋值或初始化 */
    config.TransferDir   = SDIO_TRANSFER_DIR_TO_CARD;   /* 赋值或初始化 */
    config.TransferMode  = SDIO_TRANSFER_MODE_BLOCK;    /* 赋值或初始化 */
    config.DPSM          = SDIO_DPSM_ENABLE;            /* 赋值或初始化 */
    (void)SDIO_ConfigData(hsd->Instance, &config);      /* 执行语句 */

    /* Write Blocks in Polling mode */
    if(NumberOfBlocks > 1U)                             /* 条件判断 */
    {                                                   /* 代码块开始 */
      hsd->Context = SD_CONTEXT_WRITE_MULTIPLE_BLOCK;   /* 赋值或初始化 */

      /* Write Multi Block command */
      errorstate = SDMMC_CmdWriteMultiBlock(hsd->Instance, add);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hsd->Context = SD_CONTEXT_WRITE_SINGLE_BLOCK;     /* 赋值或初始化 */

      /* Write Single Block command */
      errorstate = SDMMC_CmdWriteSingleBlock(hsd->Instance, add);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    if(errorstate != HAL_SD_ERROR_NONE)                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear all the static flags */
      __HAL_SD_CLEAR_FLAG(hsd, SDIO_STATIC_FLAGS);      /* HAL辅助宏/函数 */
      hsd->ErrorCode |= errorstate;                     /* 赋值或初始化 */
      hsd->State = HAL_SD_STATE_READY;                  /* 赋值或初始化 */
      hsd->Context = SD_CONTEXT_NONE;                   /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Write block(s) in polling mode */
    dataremaining = config.DataLength;                  /* 赋值或初始化 */
#if defined(SDIO_STA_STBITERR)                          /* 条件编译 */
    while(!__HAL_SD_GET_FLAG(hsd, SDIO_FLAG_TXUNDERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DATAEND | SDIO_FLAG_STBITERR))  /* while循环 */
#else /* SDIO_STA_STBITERR not defined */
    while(!__HAL_SD_GET_FLAG(hsd, SDIO_FLAG_TXUNDERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DATAEND))  /* while循环 */
#endif /* SDIO_STA_STBITERR */
    {                                                   /* 代码块开始 */
      if(__HAL_SD_GET_FLAG(hsd, SDIO_FLAG_TXFIFOHE) && (dataremaining > 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Write data to SDIO Tx FIFO */
        for(count = 0U; count < 8U; count++)            /* for循环 */
        {                                               /* 代码块开始 */
          data = (uint32_t)(*tempbuff);                 /* 赋值或初始化 */
          tempbuff++;                                   /* 执行语句 */
          dataremaining--;                              /* 执行语句 */
          data |= ((uint32_t)(*tempbuff) << 8U);        /* 赋值或初始化 */
          tempbuff++;                                   /* 执行语句 */
          dataremaining--;                              /* 执行语句 */
          data |= ((uint32_t)(*tempbuff) << 16U);       /* 赋值或初始化 */
          tempbuff++;                                   /* 执行语句 */
          dataremaining--;                              /* 执行语句 */
          data |= ((uint32_t)(*tempbuff) << 24U);       /* 赋值或初始化 */
          tempbuff++;                                   /* 执行语句 */
          dataremaining--;                              /* 执行语句 */
          (void)SDIO_WriteFIFO(hsd->Instance, &data);   /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */

      if(((HAL_GetTick()-tickstart) >=  Timeout) || (Timeout == 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Clear all the static flags */
        __HAL_SD_CLEAR_FLAG(hsd, SDIO_STATIC_FLAGS);    /* HAL辅助宏/函数 */
        hsd->ErrorCode |= errorstate;                   /* 赋值或初始化 */
        hsd->State = HAL_SD_STATE_READY;                /* 赋值或初始化 */
        hsd->Context = SD_CONTEXT_NONE;                 /* 赋值或初始化 */
        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Send stop transmission command in case of multiblock write */
    if(__HAL_SD_GET_FLAG(hsd, SDIO_FLAG_DATAEND) && (NumberOfBlocks > 1U))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      if(hsd->SdCard.CardType != CARD_SECURED)          /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Send stop transmission command */
        errorstate = SDMMC_CmdStopTransfer(hsd->Instance);  /* 赋值或初始化 */
        if(errorstate != HAL_SD_ERROR_NONE)             /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Clear all the static flags */
          __HAL_SD_CLEAR_FLAG(hsd, SDIO_STATIC_FLAGS);  /* HAL辅助宏/函数 */
          hsd->ErrorCode |= errorstate;                 /* 赋值或初始化 */
          hsd->State = HAL_SD_STATE_READY;              /* 赋值或初始化 */
          hsd->Context = SD_CONTEXT_NONE;               /* 赋值或初始化 */
          return HAL_ERROR;                             /* 返回值 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Get error state */
    if(__HAL_SD_GET_FLAG(hsd, SDIO_FLAG_DTIMEOUT))      /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear all the static flags */
      __HAL_SD_CLEAR_FLAG(hsd, SDIO_STATIC_FLAGS);      /* HAL辅助宏/函数 */
      hsd->ErrorCode |= HAL_SD_ERROR_DATA_TIMEOUT;      /* 赋值或初始化 */
      hsd->State = HAL_SD_STATE_READY;                  /* 赋值或初始化 */
      hsd->Context = SD_CONTEXT_NONE;                   /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
    else if(__HAL_SD_GET_FLAG(hsd, SDIO_FLAG_DCRCFAIL))  /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* Clear all the static flags */
      __HAL_SD_CLEAR_FLAG(hsd, SDIO_STATIC_FLAGS);      /* HAL辅助宏/函数 */
      hsd->ErrorCode |= HAL_SD_ERROR_DATA_CRC_FAIL;     /* 赋值或初始化 */
      hsd->State = HAL_SD_STATE_READY;                  /* 赋值或初始化 */
      hsd->Context = SD_CONTEXT_NONE;                   /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
    else if(__HAL_SD_GET_FLAG(hsd, SDIO_FLAG_TXUNDERR))  /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* Clear all the static flags */
      __HAL_SD_CLEAR_FLAG(hsd, SDIO_STATIC_FLAGS);      /* HAL辅助宏/函数 */
      hsd->ErrorCode |= HAL_SD_ERROR_TX_UNDERRUN;       /* 赋值或初始化 */
      hsd->State = HAL_SD_STATE_READY;                  /* 赋值或初始化 */
      hsd->Context = SD_CONTEXT_NONE;                   /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Nothing to do */
    }                                                   /* 代码块结束 */

    /* Clear all the static flags */
    __HAL_SD_CLEAR_FLAG(hsd, SDIO_STATIC_DATA_FLAGS);   /* HAL辅助宏/函数 */

    hsd->State = HAL_SD_STATE_READY;                    /* 赋值或初始化 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    hsd->ErrorCode |= HAL_SD_ERROR_BUSY;                /* 赋值或初始化 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Reads block(s) from a specified address in a card. The Data transfer
  *         is managed in interrupt mode.
  * @note   This API should be followed by a check on the card state through
  *         HAL_SD_GetCardState().
  * @note   You could also check the IT transfer process through the SD Rx
  *         interrupt event.
  * @param  hsd: Pointer to SD handle
  * @param  pData: Pointer to the buffer that will contain the received data
  * @param  BlockAdd: Block Address from where data is to be read
  * @param  NumberOfBlocks: Number of blocks to read.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SD_ReadBlocks_IT(SD_HandleTypeDef *hsd, uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  SDIO_DataInitTypeDef config;                          /* 执行语句 */
  uint32_t errorstate;                                  /* 32位无符号变量/函数 */
  uint32_t add = BlockAdd;                              /* 32位无符号变量/函数 */

  if(NULL == pData)                                     /* 条件判断 */
  {                                                     /* 代码块开始 */
    hsd->ErrorCode |= HAL_SD_ERROR_PARAM;               /* 赋值或初始化 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  if(hsd->State == HAL_SD_STATE_READY)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hsd->ErrorCode = HAL_SD_ERROR_NONE;                 /* 赋值或初始化 */

    if((add + NumberOfBlocks) > (hsd->SdCard.LogBlockNbr))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hsd->ErrorCode |= HAL_SD_ERROR_ADDR_OUT_OF_RANGE;  /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    hsd->State = HAL_SD_STATE_BUSY;                     /* 赋值或初始化 */

    /* Initialize data control register */
    hsd->Instance->DCTRL = 0U;                          /* 赋值或初始化 */

    hsd->pRxBuffPtr = pData;                            /* 赋值或初始化 */
    hsd->RxXferSize = BLOCKSIZE * NumberOfBlocks;       /* 赋值或初始化 */

#if defined(SDIO_STA_STBITERR)                          /* 条件编译 */
    __HAL_SD_ENABLE_IT(hsd, (SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_RXOVERR | SDIO_IT_DATAEND | SDIO_FLAG_RXFIFOHF | SDIO_IT_STBITERR));  /* HAL辅助宏/函数 */
#else /* SDIO_STA_STBITERR not defined */
    __HAL_SD_ENABLE_IT(hsd, (SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_RXOVERR | SDIO_IT_DATAEND | SDIO_FLAG_RXFIFOHF));  /* HAL辅助宏/函数 */
#endif /* SDIO_STA_STBITERR */

    if(hsd->SdCard.CardType != CARD_SDHC_SDXC)          /* 条件判断 */
    {                                                   /* 代码块开始 */
      add *= 512U;                                      /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Configure the SD DPSM (Data Path State Machine) */
    config.DataTimeOut   = SDMMC_DATATIMEOUT;           /* 赋值或初始化 */
    config.DataLength    = BLOCKSIZE * NumberOfBlocks;  /* 赋值或初始化 */
    config.DataBlockSize = SDIO_DATABLOCK_SIZE_512B;    /* 赋值或初始化 */
    config.TransferDir   = SDIO_TRANSFER_DIR_TO_SDIO;   /* 赋值或初始化 */
    config.TransferMode  = SDIO_TRANSFER_MODE_BLOCK;    /* 赋值或初始化 */
    config.DPSM          = SDIO_DPSM_ENABLE;            /* 赋值或初始化 */
    (void)SDIO_ConfigData(hsd->Instance, &config);      /* 执行语句 */

    /* Read Blocks in IT mode */
    if(NumberOfBlocks > 1U)                             /* 条件判断 */
    {                                                   /* 代码块开始 */
      hsd->Context = (SD_CONTEXT_READ_MULTIPLE_BLOCK | SD_CONTEXT_IT);  /* 赋值或初始化 */

      /* Read Multi Block command */
      errorstate = SDMMC_CmdReadMultiBlock(hsd->Instance, add);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hsd->Context = (SD_CONTEXT_READ_SINGLE_BLOCK | SD_CONTEXT_IT);  /* 赋值或初始化 */

      /* Read Single Block command */
      errorstate = SDMMC_CmdReadSingleBlock(hsd->Instance, add);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    if(errorstate != HAL_SD_ERROR_NONE)                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear all the static flags */
      __HAL_SD_CLEAR_FLAG(hsd, SDIO_STATIC_FLAGS);      /* HAL辅助宏/函数 */
      hsd->ErrorCode |= errorstate;                     /* 赋值或初始化 */
      hsd->State = HAL_SD_STATE_READY;                  /* 赋值或初始化 */
      hsd->Context = SD_CONTEXT_NONE;                   /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Writes block(s) to a specified address in a card. The Data transfer
  *         is managed in interrupt mode.
  * @note   This API should be followed by a check on the card state through
  *         HAL_SD_GetCardState().
  * @note   You could also check the IT transfer process through the SD Tx
  *         interrupt event.
  * @param  hsd: Pointer to SD handle
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  BlockAdd: Block Address where data will be written
  * @param  NumberOfBlocks: Number of blocks to write
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SD_WriteBlocks_IT(SD_HandleTypeDef *hsd, uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  SDIO_DataInitTypeDef config;                          /* 执行语句 */
  uint32_t errorstate;                                  /* 32位无符号变量/函数 */
  uint32_t add = BlockAdd;                              /* 32位无符号变量/函数 */

  if(NULL == pData)                                     /* 条件判断 */
  {                                                     /* 代码块开始 */
    hsd->ErrorCode |= HAL_SD_ERROR_PARAM;               /* 赋值或初始化 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  if(hsd->State == HAL_SD_STATE_READY)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hsd->ErrorCode = HAL_SD_ERROR_NONE;                 /* 赋值或初始化 */

    if((add + NumberOfBlocks) > (hsd->SdCard.LogBlockNbr))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hsd->ErrorCode |= HAL_SD_ERROR_ADDR_OUT_OF_RANGE;  /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    hsd->State = HAL_SD_STATE_BUSY;                     /* 赋值或初始化 */

    /* Initialize data control register */
    hsd->Instance->DCTRL = 0U;                          /* 赋值或初始化 */

    hsd->pTxBuffPtr = pData;                            /* 赋值或初始化 */
    hsd->TxXferSize = BLOCKSIZE * NumberOfBlocks;       /* 赋值或初始化 */

    /* Enable transfer interrupts */
#if defined(SDIO_STA_STBITERR)                          /* 条件编译 */
    __HAL_SD_ENABLE_IT(hsd, (SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_TXUNDERR | SDIO_IT_DATAEND | SDIO_FLAG_TXFIFOHE | SDIO_IT_STBITERR));  /* HAL辅助宏/函数 */
#else /* SDIO_STA_STBITERR not defined */
    __HAL_SD_ENABLE_IT(hsd, (SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_TXUNDERR | SDIO_IT_DATAEND | SDIO_FLAG_TXFIFOHE));  /* HAL辅助宏/函数 */
#endif /* SDIO_STA_STBITERR */

    if(hsd->SdCard.CardType != CARD_SDHC_SDXC)          /* 条件判断 */
    {                                                   /* 代码块开始 */
      add *= 512U;                                      /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Write Blocks in Polling mode */
    if(NumberOfBlocks > 1U)                             /* 条件判断 */
    {                                                   /* 代码块开始 */
      hsd->Context = (SD_CONTEXT_WRITE_MULTIPLE_BLOCK| SD_CONTEXT_IT);  /* 赋值或初始化 */

      /* Write Multi Block command */
      errorstate = SDMMC_CmdWriteMultiBlock(hsd->Instance, add);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hsd->Context = (SD_CONTEXT_WRITE_SINGLE_BLOCK | SD_CONTEXT_IT);  /* 赋值或初始化 */

      /* Write Single Block command */
      errorstate = SDMMC_CmdWriteSingleBlock(hsd->Instance, add);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    if(errorstate != HAL_SD_ERROR_NONE)                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear all the static flags */
      __HAL_SD_CLEAR_FLAG(hsd, SDIO_STATIC_FLAGS);      /* HAL辅助宏/函数 */
      hsd->ErrorCode |= errorstate;                     /* 赋值或初始化 */
      hsd->State = HAL_SD_STATE_READY;                  /* 赋值或初始化 */
      hsd->Context = SD_CONTEXT_NONE;                   /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Configure the SD DPSM (Data Path State Machine) */
    config.DataTimeOut   = SDMMC_DATATIMEOUT;           /* 赋值或初始化 */
    config.DataLength    = BLOCKSIZE * NumberOfBlocks;  /* 赋值或初始化 */
    config.DataBlockSize = SDIO_DATABLOCK_SIZE_512B;    /* 赋值或初始化 */
    config.TransferDir   = SDIO_TRANSFER_DIR_TO_CARD;   /* 赋值或初始化 */
    config.TransferMode  = SDIO_TRANSFER_MODE_BLOCK;    /* 赋值或初始化 */
    config.DPSM          = SDIO_DPSM_ENABLE;            /* 赋值或初始化 */
    (void)SDIO_ConfigData(hsd->Instance, &config);      /* 执行语句 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Reads block(s) from a specified address in a card. The Data transfer
  *         is managed by DMA mode.
  * @note   This API should be followed by a check on the card state through
  *         HAL_SD_GetCardState().
  * @note   You could also check the DMA transfer process through the SD Rx
  *         interrupt event.
  * @param  hsd: Pointer SD handle
  * @param  pData: Pointer to the buffer that will contain the received data
  * @param  BlockAdd: Block Address from where data is to be read
  * @param  NumberOfBlocks: Number of blocks to read.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SD_ReadBlocks_DMA(SD_HandleTypeDef *hsd, uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  SDIO_DataInitTypeDef config;                          /* 执行语句 */
  uint32_t errorstate;                                  /* 32位无符号变量/函数 */
  uint32_t add = BlockAdd;                              /* 32位无符号变量/函数 */

  if(NULL == pData)                                     /* 条件判断 */
  {                                                     /* 代码块开始 */
    hsd->ErrorCode |= HAL_SD_ERROR_PARAM;               /* 赋值或初始化 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  if(hsd->State == HAL_SD_STATE_READY)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hsd->ErrorCode = HAL_SD_ERROR_NONE;                 /* 赋值或初始化 */

    if((add + NumberOfBlocks) > (hsd->SdCard.LogBlockNbr))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hsd->ErrorCode |= HAL_SD_ERROR_ADDR_OUT_OF_RANGE;  /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    hsd->State = HAL_SD_STATE_BUSY;                     /* 赋值或初始化 */

    /* Initialize data control register */
    hsd->Instance->DCTRL = 0U;                          /* 赋值或初始化 */

#if defined(SDIO_STA_STBITERR)                          /* 条件编译 */
    __HAL_SD_ENABLE_IT(hsd, (SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_RXOVERR | SDIO_IT_DATAEND | SDIO_IT_STBITERR));  /* HAL辅助宏/函数 */
#else /* SDIO_STA_STBITERR not defined */
    __HAL_SD_ENABLE_IT(hsd, (SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_RXOVERR | SDIO_IT_DATAEND));  /* HAL辅助宏/函数 */
#endif /* SDIO_STA_STBITERR */

    /* Set the DMA transfer complete callback */
    hsd->hdmarx->XferCpltCallback = SD_DMAReceiveCplt;  /* 赋值或初始化 */

    /* Set the DMA error callback */
    hsd->hdmarx->XferErrorCallback = SD_DMAError;       /* 赋值或初始化 */

    /* Set the DMA Abort callback */
    hsd->hdmarx->XferAbortCallback = NULL;              /* 赋值或初始化 */

    /* Force DMA Direction */
    hsd->hdmarx->Init.Direction = DMA_PERIPH_TO_MEMORY;  /* 赋值或初始化 */
    MODIFY_REG(hsd->hdmarx->Instance->CR, DMA_SxCR_DIR, hsd->hdmarx->Init.Direction);  /* 执行语句 */

    /* Enable the DMA Channel */
    if(HAL_DMA_Start_IT(hsd->hdmarx, (uint32_t)&hsd->Instance->FIFO, (uint32_t)pData, (uint32_t)(BLOCKSIZE * NumberOfBlocks)/4U) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      __HAL_SD_DISABLE_IT(hsd, (SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_RXOVERR | SDIO_IT_DATAEND));  /* HAL辅助宏/函数 */
      __HAL_SD_CLEAR_FLAG(hsd, SDIO_STATIC_FLAGS);      /* HAL辅助宏/函数 */
      hsd->ErrorCode |= HAL_SD_ERROR_DMA;               /* 赋值或初始化 */
      hsd->State = HAL_SD_STATE_READY;                  /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Enable SD DMA transfer */
      __HAL_SD_DMA_ENABLE(hsd);                         /* HAL辅助宏/函数 */

      if(hsd->SdCard.CardType != CARD_SDHC_SDXC)        /* 条件判断 */
      {                                                 /* 代码块开始 */
        add *= 512U;                                    /* 赋值或初始化 */
      }                                                 /* 代码块结束 */

      /* Configure the SD DPSM (Data Path State Machine) */
      config.DataTimeOut   = SDMMC_DATATIMEOUT;         /* 赋值或初始化 */
      config.DataLength    = BLOCKSIZE * NumberOfBlocks;  /* 赋值或初始化 */
      config.DataBlockSize = SDIO_DATABLOCK_SIZE_512B;  /* 赋值或初始化 */
      config.TransferDir   = SDIO_TRANSFER_DIR_TO_SDIO;  /* 赋值或初始化 */
      config.TransferMode  = SDIO_TRANSFER_MODE_BLOCK;  /* 赋值或初始化 */
      config.DPSM          = SDIO_DPSM_ENABLE;          /* 赋值或初始化 */
      (void)SDIO_ConfigData(hsd->Instance, &config);    /* 执行语句 */

      /* Read Blocks in DMA mode */
      if(NumberOfBlocks > 1U)                           /* 条件判断 */
      {                                                 /* 代码块开始 */
        hsd->Context = (SD_CONTEXT_READ_MULTIPLE_BLOCK | SD_CONTEXT_DMA);  /* 赋值或初始化 */

        /* Read Multi Block command */
        errorstate = SDMMC_CmdReadMultiBlock(hsd->Instance, add);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        hsd->Context = (SD_CONTEXT_READ_SINGLE_BLOCK | SD_CONTEXT_DMA);  /* 赋值或初始化 */

        /* Read Single Block command */
        errorstate = SDMMC_CmdReadSingleBlock(hsd->Instance, add);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      if(errorstate != HAL_SD_ERROR_NONE)               /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Clear all the static flags */
        __HAL_SD_CLEAR_FLAG(hsd, SDIO_STATIC_FLAGS);    /* HAL辅助宏/函数 */
        hsd->ErrorCode |= errorstate;                   /* 赋值或初始化 */
        hsd->State = HAL_SD_STATE_READY;                /* 赋值或初始化 */
        hsd->Context = SD_CONTEXT_NONE;                 /* 赋值或初始化 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */

      return HAL_OK;                                    /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Writes block(s) to a specified address in a card. The Data transfer
  *         is managed by DMA mode.
  * @note   This API should be followed by a check on the card state through
  *         HAL_SD_GetCardState().
  * @note   You could also check the DMA transfer process through the SD Tx
  *         interrupt event.
  * @param  hsd: Pointer to SD handle
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  BlockAdd: Block Address where data will be written
  * @param  NumberOfBlocks: Number of blocks to write
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SD_WriteBlocks_DMA(SD_HandleTypeDef *hsd, uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  SDIO_DataInitTypeDef config;                          /* 执行语句 */
  uint32_t errorstate;                                  /* 32位无符号变量/函数 */
  uint32_t add = BlockAdd;                              /* 32位无符号变量/函数 */

  if(NULL == pData)                                     /* 条件判断 */
  {                                                     /* 代码块开始 */
    hsd->ErrorCode |= HAL_SD_ERROR_PARAM;               /* 赋值或初始化 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  if(hsd->State == HAL_SD_STATE_READY)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hsd->ErrorCode = HAL_SD_ERROR_NONE;                 /* 赋值或初始化 */

    if((add + NumberOfBlocks) > (hsd->SdCard.LogBlockNbr))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hsd->ErrorCode |= HAL_SD_ERROR_ADDR_OUT_OF_RANGE;  /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    hsd->State = HAL_SD_STATE_BUSY;                     /* 赋值或初始化 */

    /* Initialize data control register */
    hsd->Instance->DCTRL = 0U;                          /* 赋值或初始化 */

    /* Enable SD Error interrupts */
#if defined(SDIO_STA_STBITERR)                          /* 条件编译 */
    __HAL_SD_ENABLE_IT(hsd, (SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_TXUNDERR | SDIO_IT_STBITERR));  /* HAL辅助宏/函数 */
#else /* SDIO_STA_STBITERR not defined */
    __HAL_SD_ENABLE_IT(hsd, (SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_TXUNDERR));     /* HAL辅助宏/函数 */
#endif /* SDIO_STA_STBITERR */

    /* Set the DMA transfer complete callback */
    hsd->hdmatx->XferCpltCallback = SD_DMATransmitCplt;  /* 赋值或初始化 */

    /* Set the DMA error callback */
    hsd->hdmatx->XferErrorCallback = SD_DMAError;       /* 赋值或初始化 */

    /* Set the DMA Abort callback */
    hsd->hdmatx->XferAbortCallback = NULL;              /* 赋值或初始化 */

    if(hsd->SdCard.CardType != CARD_SDHC_SDXC)          /* 条件判断 */
    {                                                   /* 代码块开始 */
      add *= 512U;                                      /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Write Blocks in Polling mode */
    if(NumberOfBlocks > 1U)                             /* 条件判断 */
    {                                                   /* 代码块开始 */
      hsd->Context = (SD_CONTEXT_WRITE_MULTIPLE_BLOCK | SD_CONTEXT_DMA);  /* 赋值或初始化 */

      /* Write Multi Block command */
      errorstate = SDMMC_CmdWriteMultiBlock(hsd->Instance, add);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hsd->Context = (SD_CONTEXT_WRITE_SINGLE_BLOCK | SD_CONTEXT_DMA);  /* 赋值或初始化 */

      /* Write Single Block command */
      errorstate = SDMMC_CmdWriteSingleBlock(hsd->Instance, add);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    if(errorstate != HAL_SD_ERROR_NONE)                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear all the static flags */
      __HAL_SD_CLEAR_FLAG(hsd, SDIO_STATIC_FLAGS);      /* HAL辅助宏/函数 */
      hsd->ErrorCode |= errorstate;                     /* 赋值或初始化 */
      hsd->State = HAL_SD_STATE_READY;                  /* 赋值或初始化 */
      hsd->Context = SD_CONTEXT_NONE;                   /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Enable SDIO DMA transfer */
    __HAL_SD_DMA_ENABLE(hsd);                           /* HAL辅助宏/函数 */

    /* Force DMA Direction */
    hsd->hdmatx->Init.Direction = DMA_MEMORY_TO_PERIPH;  /* 赋值或初始化 */
    MODIFY_REG(hsd->hdmatx->Instance->CR, DMA_SxCR_DIR, hsd->hdmatx->Init.Direction);  /* 执行语句 */

    /* Enable the DMA Channel */
    if(HAL_DMA_Start_IT(hsd->hdmatx, (uint32_t)pData, (uint32_t)&hsd->Instance->FIFO, (uint32_t)(BLOCKSIZE * NumberOfBlocks)/4U) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
#if defined(SDIO_STA_STBITERR)                          /* 条件编译 */
      __HAL_SD_DISABLE_IT(hsd, (SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_TXUNDERR | SDIO_IT_STBITERR));  /* HAL辅助宏/函数 */
#else /* SDIO_STA_STBITERR not defined */
      __HAL_SD_DISABLE_IT(hsd, (SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_TXUNDERR));     /* HAL辅助宏/函数 */
#endif /* SDIO_STA_STBITERR */
      __HAL_SD_CLEAR_FLAG(hsd, SDIO_STATIC_FLAGS);      /* HAL辅助宏/函数 */
      hsd->ErrorCode |= HAL_SD_ERROR_DMA;               /* 赋值或初始化 */
      hsd->State = HAL_SD_STATE_READY;                  /* 赋值或初始化 */
      hsd->Context = SD_CONTEXT_NONE;                   /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Configure the SD DPSM (Data Path State Machine) */
      config.DataTimeOut   = SDMMC_DATATIMEOUT;         /* 赋值或初始化 */
      config.DataLength    = BLOCKSIZE * NumberOfBlocks;  /* 赋值或初始化 */
      config.DataBlockSize = SDIO_DATABLOCK_SIZE_512B;  /* 赋值或初始化 */
      config.TransferDir   = SDIO_TRANSFER_DIR_TO_CARD;  /* 赋值或初始化 */
      config.TransferMode  = SDIO_TRANSFER_MODE_BLOCK;  /* 赋值或初始化 */
      config.DPSM          = SDIO_DPSM_ENABLE;          /* 赋值或初始化 */
      (void)SDIO_ConfigData(hsd->Instance, &config);    /* 执行语句 */

      return HAL_OK;                                    /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Erases the specified memory area of the given SD card.
  * @note   This API should be followed by a check on the card state through
  *         HAL_SD_GetCardState().
  * @param  hsd: Pointer to SD handle
  * @param  BlockStartAdd: Start Block address
  * @param  BlockEndAdd: End Block address
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SD_Erase(SD_HandleTypeDef *hsd, uint32_t BlockStartAdd, uint32_t BlockEndAdd)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t errorstate;                                  /* 32位无符号变量/函数 */
  uint32_t start_add = BlockStartAdd;                   /* 32位无符号变量/函数 */
  uint32_t end_add = BlockEndAdd;                       /* 32位无符号变量/函数 */

  if(hsd->State == HAL_SD_STATE_READY)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hsd->ErrorCode = HAL_SD_ERROR_NONE;                 /* 赋值或初始化 */

    if(end_add < start_add)                             /* 条件判断 */
    {                                                   /* 代码块开始 */
      hsd->ErrorCode |= HAL_SD_ERROR_PARAM;             /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    if(end_add > (hsd->SdCard.LogBlockNbr))             /* 条件判断 */
    {                                                   /* 代码块开始 */
      hsd->ErrorCode |= HAL_SD_ERROR_ADDR_OUT_OF_RANGE;  /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    hsd->State = HAL_SD_STATE_BUSY;                     /* 赋值或初始化 */

    /* Check if the card command class supports erase command */
    if(((hsd->SdCard.Class) & SDIO_CCCC_ERASE) == 0U)   /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear all the static flags */
      __HAL_SD_CLEAR_FLAG(hsd, SDIO_STATIC_FLAGS);      /* HAL辅助宏/函数 */
      hsd->ErrorCode |= HAL_SD_ERROR_REQUEST_NOT_APPLICABLE;  /* 赋值或初始化 */
      hsd->State = HAL_SD_STATE_READY;                  /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    if((SDIO_GetResponse(hsd->Instance, SDIO_RESP1) & SDMMC_CARD_LOCKED) == SDMMC_CARD_LOCKED)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear all the static flags */
      __HAL_SD_CLEAR_FLAG(hsd, SDIO_STATIC_FLAGS);      /* HAL辅助宏/函数 */
      hsd->ErrorCode |= HAL_SD_ERROR_LOCK_UNLOCK_FAILED;  /* 赋值或初始化 */
      hsd->State = HAL_SD_STATE_READY;                  /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Get start and end block for high capacity cards */
    if(hsd->SdCard.CardType != CARD_SDHC_SDXC)          /* 条件判断 */
    {                                                   /* 代码块开始 */
      start_add *= 512U;                                /* 赋值或初始化 */
      end_add   *= 512U;                                /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* According to sd-card spec 1.0 ERASE_GROUP_START (CMD32) and erase_group_end(CMD33) */
    if(hsd->SdCard.CardType != CARD_SECURED)            /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Send CMD32 SD_ERASE_GRP_START with argument as addr  */
      errorstate = SDMMC_CmdSDEraseStartAdd(hsd->Instance, start_add);  /* 赋值或初始化 */
      if(errorstate != HAL_SD_ERROR_NONE)               /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Clear all the static flags */
        __HAL_SD_CLEAR_FLAG(hsd, SDIO_STATIC_FLAGS);    /* HAL辅助宏/函数 */
        hsd->ErrorCode |= errorstate;                   /* 赋值或初始化 */
        hsd->State = HAL_SD_STATE_READY;                /* 赋值或初始化 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */

      /* Send CMD33 SD_ERASE_GRP_END with argument as addr  */
      errorstate = SDMMC_CmdSDEraseEndAdd(hsd->Instance, end_add);  /* 赋值或初始化 */
      if(errorstate != HAL_SD_ERROR_NONE)               /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Clear all the static flags */
        __HAL_SD_CLEAR_FLAG(hsd, SDIO_STATIC_FLAGS);    /* HAL辅助宏/函数 */
        hsd->ErrorCode |= errorstate;                   /* 赋值或初始化 */
        hsd->State = HAL_SD_STATE_READY;                /* 赋值或初始化 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Send CMD38 ERASE */
    errorstate = SDMMC_CmdErase(hsd->Instance);         /* 赋值或初始化 */
    if(errorstate != HAL_SD_ERROR_NONE)                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear all the static flags */
      __HAL_SD_CLEAR_FLAG(hsd, SDIO_STATIC_FLAGS);      /* HAL辅助宏/函数 */
      hsd->ErrorCode |= errorstate;                     /* 赋值或初始化 */
      hsd->State = HAL_SD_STATE_READY;                  /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    hsd->State = HAL_SD_STATE_READY;                    /* 赋值或初始化 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  This function handles SD card interrupt request.
  * @param  hsd: Pointer to SD handle
  * @retval None
  */
void HAL_SD_IRQHandler(SD_HandleTypeDef *hsd)           /* 中断服务函数 */
{                                                       /* 代码块开始 */
  uint32_t errorstate;                                  /* 32位无符号变量/函数 */
  uint32_t context = hsd->Context;                      /* 32位无符号变量/函数 */

  /* Check for SDIO interrupt flags */
  if((__HAL_SD_GET_FLAG(hsd, SDIO_FLAG_RXFIFOHF) != RESET) && ((context & SD_CONTEXT_IT) != 0U))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    SD_Read_IT(hsd);                                    /* 执行语句 */
  }                                                     /* 代码块结束 */

  else if(__HAL_SD_GET_FLAG(hsd, SDIO_FLAG_DATAEND) != RESET)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    __HAL_SD_CLEAR_FLAG(hsd, SDIO_FLAG_DATAEND);        /* HAL辅助宏/函数 */

#if defined(SDIO_STA_STBITERR)                          /* 条件编译 */
    __HAL_SD_DISABLE_IT(hsd, SDIO_IT_DATAEND  | SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT|\
                             SDIO_IT_TXUNDERR | SDIO_IT_RXOVERR  | SDIO_IT_TXFIFOHE |\
                             SDIO_IT_RXFIFOHF | SDIO_IT_STBITERR);  /* 执行语句 */
#else /* SDIO_STA_STBITERR not defined */
    __HAL_SD_DISABLE_IT(hsd, SDIO_IT_DATAEND  | SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT|\
                             SDIO_IT_TXUNDERR | SDIO_IT_RXOVERR  | SDIO_IT_TXFIFOHE |\
                             SDIO_IT_RXFIFOHF);         /* 执行语句 */
#endif /* SDIO_STA_STBITERR */

    hsd->Instance->DCTRL &= ~(SDIO_DCTRL_DTEN);         /* 赋值或初始化 */

    if((context & SD_CONTEXT_IT) != 0U)                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      if(((context & SD_CONTEXT_READ_MULTIPLE_BLOCK) != 0U) || ((context & SD_CONTEXT_WRITE_MULTIPLE_BLOCK) != 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        errorstate = SDMMC_CmdStopTransfer(hsd->Instance);  /* 赋值或初始化 */
        if(errorstate != HAL_SD_ERROR_NONE)             /* 条件判断 */
        {                                               /* 代码块开始 */
          hsd->ErrorCode |= errorstate;                 /* 赋值或初始化 */
#if defined (USE_HAL_SD_REGISTER_CALLBACKS) && (USE_HAL_SD_REGISTER_CALLBACKS == 1U)  /* 条件编译 */
          hsd->ErrorCallback(hsd);                      /* 执行语句 */
#else                                                   /* 条件编译否则 */
          HAL_SD_ErrorCallback(hsd);                    /* HAL库函数调用 */
#endif /* USE_HAL_SD_REGISTER_CALLBACKS */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */

      /* Clear all the static flags */
      __HAL_SD_CLEAR_FLAG(hsd, SDIO_STATIC_DATA_FLAGS);  /* HAL辅助宏/函数 */

      hsd->State = HAL_SD_STATE_READY;                  /* 赋值或初始化 */
      hsd->Context = SD_CONTEXT_NONE;                   /* 赋值或初始化 */
      if(((context & SD_CONTEXT_READ_SINGLE_BLOCK) != 0U) || ((context & SD_CONTEXT_READ_MULTIPLE_BLOCK) != 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
#if defined (USE_HAL_SD_REGISTER_CALLBACKS) && (USE_HAL_SD_REGISTER_CALLBACKS == 1U)  /* 条件编译 */
        hsd->RxCpltCallback(hsd);                       /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_SD_RxCpltCallback(hsd);                     /* HAL库函数调用 */
#endif /* USE_HAL_SD_REGISTER_CALLBACKS */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
#if defined (USE_HAL_SD_REGISTER_CALLBACKS) && (USE_HAL_SD_REGISTER_CALLBACKS == 1U)  /* 条件编译 */
        hsd->TxCpltCallback(hsd);                       /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_SD_TxCpltCallback(hsd);                     /* HAL库函数调用 */
#endif /* USE_HAL_SD_REGISTER_CALLBACKS */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else if((context & SD_CONTEXT_DMA) != 0U)           /* 否则如果 */
    {                                                   /* 代码块开始 */
      if((context & SD_CONTEXT_WRITE_MULTIPLE_BLOCK) != 0U)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        errorstate = SDMMC_CmdStopTransfer(hsd->Instance);  /* 赋值或初始化 */
        if(errorstate != HAL_SD_ERROR_NONE)             /* 条件判断 */
        {                                               /* 代码块开始 */
          hsd->ErrorCode |= errorstate;                 /* 赋值或初始化 */
#if defined (USE_HAL_SD_REGISTER_CALLBACKS) && (USE_HAL_SD_REGISTER_CALLBACKS == 1U)  /* 条件编译 */
          hsd->ErrorCallback(hsd);                      /* 执行语句 */
#else                                                   /* 条件编译否则 */
          HAL_SD_ErrorCallback(hsd);                    /* HAL库函数调用 */
#endif /* USE_HAL_SD_REGISTER_CALLBACKS */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      if(((context & SD_CONTEXT_READ_SINGLE_BLOCK) == 0U) && ((context & SD_CONTEXT_READ_MULTIPLE_BLOCK) == 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Disable the DMA transfer for transmit request by setting the DMAEN bit
        in the SD DCTRL register */
        hsd->Instance->DCTRL &= (uint32_t)~((uint32_t)SDIO_DCTRL_DMAEN);  /* 赋值或初始化 */

        hsd->State = HAL_SD_STATE_READY;                /* 赋值或初始化 */

#if defined (USE_HAL_SD_REGISTER_CALLBACKS) && (USE_HAL_SD_REGISTER_CALLBACKS == 1U)  /* 条件编译 */
        hsd->TxCpltCallback(hsd);                       /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_SD_TxCpltCallback(hsd);                     /* HAL库函数调用 */
#endif /* USE_HAL_SD_REGISTER_CALLBACKS */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Nothing to do */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  else if((__HAL_SD_GET_FLAG(hsd, SDIO_FLAG_TXFIFOHE) != RESET) && ((context & SD_CONTEXT_IT) != 0U))  /* 否则如果 */
  {                                                     /* 代码块开始 */
    SD_Write_IT(hsd);                                   /* 执行语句 */
  }                                                     /* 代码块结束 */

#if defined(SDIO_STA_STBITERR)                          /* 条件编译 */
  else if(__HAL_SD_GET_FLAG(hsd, SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_RXOVERR | SDIO_FLAG_TXUNDERR | SDIO_FLAG_STBITERR) != RESET)  /* 否则如果 */
#else /* SDIO_STA_STBITERR not defined */
  else if(__HAL_SD_GET_FLAG(hsd, SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_RXOVERR | SDIO_FLAG_TXUNDERR) != RESET)  /* 否则如果 */
#endif /* SDIO_STA_STBITERR */
  {                                                     /* 代码块开始 */
    /* Set Error code */
    if(__HAL_SD_GET_FLAG(hsd, SDIO_FLAG_DCRCFAIL) != RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hsd->ErrorCode |= HAL_SD_ERROR_DATA_CRC_FAIL;     /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    if(__HAL_SD_GET_FLAG(hsd, SDIO_FLAG_DTIMEOUT) != RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hsd->ErrorCode |= HAL_SD_ERROR_DATA_TIMEOUT;      /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    if(__HAL_SD_GET_FLAG(hsd, SDIO_FLAG_RXOVERR) != RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hsd->ErrorCode |= HAL_SD_ERROR_RX_OVERRUN;        /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    if(__HAL_SD_GET_FLAG(hsd, SDIO_FLAG_TXUNDERR) != RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hsd->ErrorCode |= HAL_SD_ERROR_TX_UNDERRUN;       /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
#if defined(SDIO_STA_STBITERR)                          /* 条件编译 */
    if(__HAL_SD_GET_FLAG(hsd, SDIO_FLAG_STBITERR) != RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hsd->ErrorCode |= HAL_SD_ERROR_DATA_TIMEOUT;      /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
#endif /* SDIO_STA_STBITERR */

#if defined(SDIO_STA_STBITERR)                          /* 条件编译 */
    /* Clear All flags */
    __HAL_SD_CLEAR_FLAG(hsd, SDIO_STATIC_DATA_FLAGS | SDIO_FLAG_STBITERR);  /* HAL辅助宏/函数 */

    /* Disable all interrupts */
    __HAL_SD_DISABLE_IT(hsd, SDIO_IT_DATAEND | SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT|\
                             SDIO_IT_TXUNDERR| SDIO_IT_RXOVERR | SDIO_IT_STBITERR);  /* 执行语句 */
#else /* SDIO_STA_STBITERR not defined */
    /* Clear All flags */
    __HAL_SD_CLEAR_FLAG(hsd, SDIO_STATIC_DATA_FLAGS);   /* HAL辅助宏/函数 */

    /* Disable all interrupts */
    __HAL_SD_DISABLE_IT(hsd, SDIO_IT_DATAEND | SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT|\
                             SDIO_IT_TXUNDERR| SDIO_IT_RXOVERR);  /* 执行语句 */
#endif /* SDIO_STA_STBITERR */

    hsd->ErrorCode |= SDMMC_CmdStopTransfer(hsd->Instance);  /* 赋值或初始化 */

    if((context & SD_CONTEXT_IT) != 0U)                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the SD state to ready to be able to start again the process */
      hsd->State = HAL_SD_STATE_READY;                  /* 赋值或初始化 */
      hsd->Context = SD_CONTEXT_NONE;                   /* 赋值或初始化 */
#if defined (USE_HAL_SD_REGISTER_CALLBACKS) && (USE_HAL_SD_REGISTER_CALLBACKS == 1U)  /* 条件编译 */
      hsd->ErrorCallback(hsd);                          /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_SD_ErrorCallback(hsd);                        /* HAL库函数调用 */
#endif /* USE_HAL_SD_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
    else if((context & SD_CONTEXT_DMA) != 0U)           /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* Abort the SD DMA channel */
      if(((context & SD_CONTEXT_WRITE_SINGLE_BLOCK) != 0U) || ((context & SD_CONTEXT_WRITE_MULTIPLE_BLOCK) != 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Set the DMA Tx abort callback */
        hsd->hdmatx->XferAbortCallback = SD_DMATxAbort;  /* 赋值或初始化 */
        /* Abort DMA in IT mode */
        if(HAL_DMA_Abort_IT(hsd->hdmatx) != HAL_OK)     /* 条件判断 */
        {                                               /* 代码块开始 */
          SD_DMATxAbort(hsd->hdmatx);                   /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else if(((context & SD_CONTEXT_READ_SINGLE_BLOCK) != 0U) || ((context & SD_CONTEXT_READ_MULTIPLE_BLOCK) != 0U))  /* 否则如果 */
      {                                                 /* 代码块开始 */
        /* Set the DMA Rx abort callback */
        hsd->hdmarx->XferAbortCallback = SD_DMARxAbort;  /* 赋值或初始化 */
        /* Abort DMA in IT mode */
        if(HAL_DMA_Abort_IT(hsd->hdmarx) != HAL_OK)     /* 条件判断 */
        {                                               /* 代码块开始 */
          SD_DMARxAbort(hsd->hdmarx);                   /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        hsd->ErrorCode = HAL_SD_ERROR_NONE;             /* 赋值或初始化 */
        hsd->State = HAL_SD_STATE_READY;                /* 赋值或初始化 */
        hsd->Context = SD_CONTEXT_NONE;                 /* 赋值或初始化 */
#if defined (USE_HAL_SD_REGISTER_CALLBACKS) && (USE_HAL_SD_REGISTER_CALLBACKS == 1U)  /* 条件编译 */
        hsd->AbortCpltCallback(hsd);                    /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_SD_AbortCallback(hsd);                      /* HAL库函数调用 */
#endif /* USE_HAL_SD_REGISTER_CALLBACKS */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Nothing to do */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Nothing to do */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief return the SD state
  * @param hsd: Pointer to sd handle
  * @retval HAL state
  */
HAL_SD_StateTypeDef HAL_SD_GetState(SD_HandleTypeDef *hsd)  /* HAL库函数调用 */
{                                                       /* 代码块开始 */
  return hsd->State;                                    /* 返回值 */
}                                                       /* 代码块结束 */

/**
* @brief  Return the SD error code
* @param  hsd : Pointer to a SD_HandleTypeDef structure that contains
  *              the configuration information.
* @retval SD Error Code
*/
uint32_t HAL_SD_GetError(SD_HandleTypeDef *hsd)         /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  return hsd->ErrorCode;                                /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief Tx Transfer completed callbacks
  * @param hsd: Pointer to SD handle
  * @retval None
  */
__weak void HAL_SD_TxCpltCallback(SD_HandleTypeDef *hsd)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsd);                                          /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SD_TxCpltCallback can be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief Rx Transfer completed callbacks
  * @param hsd: Pointer SD handle
  * @retval None
  */
__weak void HAL_SD_RxCpltCallback(SD_HandleTypeDef *hsd)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsd);                                          /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SD_RxCpltCallback can be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief SD error callbacks
  * @param hsd: Pointer SD handle
  * @retval None
  */
__weak void HAL_SD_ErrorCallback(SD_HandleTypeDef *hsd)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsd);                                          /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SD_ErrorCallback can be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief SD Abort callbacks
  * @param hsd: Pointer SD handle
  * @retval None
  */
__weak void HAL_SD_AbortCallback(SD_HandleTypeDef *hsd)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsd);                                          /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SD_AbortCallback can be implemented in the user file
   */
}                                                       /* 代码块结束 */

#if defined (USE_HAL_SD_REGISTER_CALLBACKS) && (USE_HAL_SD_REGISTER_CALLBACKS == 1U)  /* 条件编译 */
/**
  * @brief  Register a User SD Callback
  *         To be used instead of the weak (surcharged) predefined callback
  * @param hsd : SD handle
  * @param CallbackID : ID of the callback to be registered
  *        This parameter can be one of the following values:
  *          @arg @ref HAL_SD_TX_CPLT_CB_ID    SD Tx Complete Callback ID
  *          @arg @ref HAL_SD_RX_CPLT_CB_ID    SD Rx Complete Callback ID
  *          @arg @ref HAL_SD_ERROR_CB_ID      SD Error Callback ID
  *          @arg @ref HAL_SD_ABORT_CB_ID      SD Abort Callback ID
  *          @arg @ref HAL_SD_MSP_INIT_CB_ID   SD MspInit Callback ID
  *          @arg @ref HAL_SD_MSP_DEINIT_CB_ID SD MspDeInit Callback ID
  * @param pCallback : pointer to the Callback function
  * @retval status
  */
HAL_StatusTypeDef HAL_SD_RegisterCallback(SD_HandleTypeDef *hsd, HAL_SD_CallbackIDTypeDef CallbackID, pSD_CallbackTypeDef pCallback)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  if(pCallback == NULL)                                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hsd->ErrorCode |= HAL_SD_ERROR_INVALID_CALLBACK;    /* 赋值或初始化 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Process locked */
  __HAL_LOCK(hsd);                                      /* HAL辅助宏/函数 */

  if(hsd->State == HAL_SD_STATE_READY)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
    case HAL_SD_TX_CPLT_CB_ID :                         /* case标签 */
      hsd->TxCpltCallback = pCallback;                  /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_SD_RX_CPLT_CB_ID :                         /* case标签 */
      hsd->RxCpltCallback = pCallback;                  /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_SD_ERROR_CB_ID :                           /* case标签 */
      hsd->ErrorCallback = pCallback;                   /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_SD_ABORT_CB_ID :                           /* case标签 */
      hsd->AbortCpltCallback = pCallback;               /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_SD_MSP_INIT_CB_ID :                        /* case标签 */
      hsd->MspInitCallback = pCallback;                 /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_SD_MSP_DEINIT_CB_ID :                      /* case标签 */
      hsd->MspDeInitCallback = pCallback;               /* 赋值或初始化 */
      break;                                            /* 跳出 */
    default :                                           /* default分支 */
      /* Update the error code */
      hsd->ErrorCode |= HAL_SD_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
      /* update return status */
      status =  HAL_ERROR;                              /* 赋值或初始化 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (hsd->State == HAL_SD_STATE_RESET)            /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
    case HAL_SD_MSP_INIT_CB_ID :                        /* case标签 */
      hsd->MspInitCallback = pCallback;                 /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_SD_MSP_DEINIT_CB_ID :                      /* case标签 */
      hsd->MspDeInitCallback = pCallback;               /* 赋值或初始化 */
      break;                                            /* 跳出 */
    default :                                           /* default分支 */
      /* Update the error code */
      hsd->ErrorCode |= HAL_SD_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
      /* update return status */
      status =  HAL_ERROR;                              /* 赋值或初始化 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hsd->ErrorCode |= HAL_SD_ERROR_INVALID_CALLBACK;    /* 赋值或初始化 */
    /* update return status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hsd);                                    /* HAL辅助宏/函数 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Unregister a User SD Callback
  *         SD Callback is redirected to the weak (surcharged) predefined callback
  * @param hsd : SD handle
  * @param CallbackID : ID of the callback to be unregistered
  *        This parameter can be one of the following values:
  *          @arg @ref HAL_SD_TX_CPLT_CB_ID    SD Tx Complete Callback ID
  *          @arg @ref HAL_SD_RX_CPLT_CB_ID    SD Rx Complete Callback ID
  *          @arg @ref HAL_SD_ERROR_CB_ID      SD Error Callback ID
  *          @arg @ref HAL_SD_ABORT_CB_ID      SD Abort Callback ID
  *          @arg @ref HAL_SD_MSP_INIT_CB_ID   SD MspInit Callback ID
  *          @arg @ref HAL_SD_MSP_DEINIT_CB_ID SD MspDeInit Callback ID
  * @retval status
  */
HAL_StatusTypeDef HAL_SD_UnRegisterCallback(SD_HandleTypeDef *hsd, HAL_SD_CallbackIDTypeDef CallbackID)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Process locked */
  __HAL_LOCK(hsd);                                      /* HAL辅助宏/函数 */

  if(hsd->State == HAL_SD_STATE_READY)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
    case HAL_SD_TX_CPLT_CB_ID :                         /* case标签 */
      hsd->TxCpltCallback = HAL_SD_TxCpltCallback;      /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_SD_RX_CPLT_CB_ID :                         /* case标签 */
      hsd->RxCpltCallback = HAL_SD_RxCpltCallback;      /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_SD_ERROR_CB_ID :                           /* case标签 */
      hsd->ErrorCallback = HAL_SD_ErrorCallback;        /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_SD_ABORT_CB_ID :                           /* case标签 */
      hsd->AbortCpltCallback = HAL_SD_AbortCallback;    /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_SD_MSP_INIT_CB_ID :                        /* case标签 */
      hsd->MspInitCallback = HAL_SD_MspInit;            /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_SD_MSP_DEINIT_CB_ID :                      /* case标签 */
      hsd->MspDeInitCallback = HAL_SD_MspDeInit;        /* 赋值或初始化 */
      break;                                            /* 跳出 */
    default :                                           /* default分支 */
      /* Update the error code */
      hsd->ErrorCode |= HAL_SD_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
      /* update return status */
      status =  HAL_ERROR;                              /* 赋值或初始化 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (hsd->State == HAL_SD_STATE_RESET)            /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
    case HAL_SD_MSP_INIT_CB_ID :                        /* case标签 */
      hsd->MspInitCallback = HAL_SD_MspInit;            /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_SD_MSP_DEINIT_CB_ID :                      /* case标签 */
      hsd->MspDeInitCallback = HAL_SD_MspDeInit;        /* 赋值或初始化 */
      break;                                            /* 跳出 */
    default :                                           /* default分支 */
      /* Update the error code */
      hsd->ErrorCode |= HAL_SD_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
      /* update return status */
      status =  HAL_ERROR;                              /* 赋值或初始化 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hsd->ErrorCode |= HAL_SD_ERROR_INVALID_CALLBACK;    /* 赋值或初始化 */
    /* update return status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hsd);                                    /* HAL辅助宏/函数 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* USE_HAL_SD_REGISTER_CALLBACKS */

/**
  * @}
  */

/** @addtogroup SD_Exported_Functions_Group3
 *  @brief   management functions
 *
@verbatim
  ==============================================================================
                      ##### Peripheral Control functions #####
  ==============================================================================
  [..]
    This subsection provides a set of functions allowing to control the SD card
    operations and get the related information

@endverbatim
  * @{
  */

/**
  * @brief  Returns information the information of the card which are stored on
  *         the CID register.
  * @param  hsd: Pointer to SD handle
  * @param  pCID: Pointer to a HAL_SD_CardCIDTypeDef structure that  
  *         contains all CID register parameters
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SD_GetCardCID(SD_HandleTypeDef *hsd, HAL_SD_CardCIDTypeDef *pCID)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  pCID->ManufacturerID = (uint8_t)((hsd->CID[0] & 0xFF000000U) >> 24U);  /* 赋值或初始化 */

  pCID->OEM_AppliID = (uint16_t)((hsd->CID[0] & 0x00FFFF00U) >> 8U);  /* 赋值或初始化 */

  pCID->ProdName1 = (((hsd->CID[0] & 0x000000FFU) << 24U) | ((hsd->CID[1] & 0xFFFFFF00U) >> 8U));  /* 赋值或初始化 */

  pCID->ProdName2 = (uint8_t)(hsd->CID[1] & 0x000000FFU);  /* 赋值或初始化 */

  pCID->ProdRev = (uint8_t)((hsd->CID[2] & 0xFF000000U) >> 24U);  /* 赋值或初始化 */

  pCID->ProdSN = (((hsd->CID[2] & 0x00FFFFFFU) << 8U) | ((hsd->CID[3] & 0xFF000000U) >> 24U));  /* 赋值或初始化 */

  pCID->Reserved1 = (uint8_t)((hsd->CID[3] & 0x00F00000U) >> 20U);  /* 赋值或初始化 */

  pCID->ManufactDate = (uint16_t)((hsd->CID[3] & 0x000FFF00U) >> 8U);  /* 赋值或初始化 */

  pCID->CID_CRC = (uint8_t)((hsd->CID[3] & 0x000000FEU) >> 1U);  /* 赋值或初始化 */

  pCID->Reserved2 = 1U;                                 /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Returns information the information of the card which are stored on
  *         the CSD register.
  * @param  hsd: Pointer to SD handle
  * @param  pCSD: Pointer to a HAL_SD_CardCSDTypeDef structure that  
  *         contains all CSD register parameters
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SD_GetCardCSD(SD_HandleTypeDef *hsd, HAL_SD_CardCSDTypeDef *pCSD)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  pCSD->CSDStruct = (uint8_t)((hsd->CSD[0] & 0xC0000000U) >> 30U);  /* 赋值或初始化 */

  pCSD->SysSpecVersion = (uint8_t)((hsd->CSD[0] & 0x3C000000U) >> 26U);  /* 赋值或初始化 */

  pCSD->Reserved1 = (uint8_t)((hsd->CSD[0] & 0x03000000U) >> 24U);  /* 赋值或初始化 */

  pCSD->TAAC = (uint8_t)((hsd->CSD[0] & 0x00FF0000U) >> 16U);  /* 赋值或初始化 */

  pCSD->NSAC = (uint8_t)((hsd->CSD[0] & 0x0000FF00U) >> 8U);  /* 赋值或初始化 */

  pCSD->MaxBusClkFrec = (uint8_t)(hsd->CSD[0] & 0x000000FFU);  /* 赋值或初始化 */

  pCSD->CardComdClasses = (uint16_t)((hsd->CSD[1] & 0xFFF00000U) >> 20U);  /* 赋值或初始化 */

  pCSD->RdBlockLen = (uint8_t)((hsd->CSD[1] & 0x000F0000U) >> 16U);  /* 赋值或初始化 */

  pCSD->PartBlockRead   = (uint8_t)((hsd->CSD[1] & 0x00008000U) >> 15U);  /* 赋值或初始化 */

  pCSD->WrBlockMisalign = (uint8_t)((hsd->CSD[1] & 0x00004000U) >> 14U);  /* 赋值或初始化 */

  pCSD->RdBlockMisalign = (uint8_t)((hsd->CSD[1] & 0x00002000U) >> 13U);  /* 赋值或初始化 */

  pCSD->DSRImpl = (uint8_t)((hsd->CSD[1] & 0x00001000U) >> 12U);  /* 赋值或初始化 */

  pCSD->Reserved2 = 0U; /*!< Reserved */

  if(hsd->SdCard.CardType == CARD_SDSC)                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    pCSD->DeviceSize = (((hsd->CSD[1] & 0x000003FFU) << 2U) | ((hsd->CSD[2] & 0xC0000000U) >> 30U));  /* 赋值或初始化 */

    pCSD->MaxRdCurrentVDDMin = (uint8_t)((hsd->CSD[2] & 0x38000000U) >> 27U);  /* 赋值或初始化 */

    pCSD->MaxRdCurrentVDDMax = (uint8_t)((hsd->CSD[2] & 0x07000000U) >> 24U);  /* 赋值或初始化 */

    pCSD->MaxWrCurrentVDDMin = (uint8_t)((hsd->CSD[2] & 0x00E00000U) >> 21U);  /* 赋值或初始化 */

    pCSD->MaxWrCurrentVDDMax = (uint8_t)((hsd->CSD[2] & 0x001C0000U) >> 18U);  /* 赋值或初始化 */

    pCSD->DeviceSizeMul = (uint8_t)((hsd->CSD[2] & 0x00038000U) >> 15U);  /* 赋值或初始化 */

    hsd->SdCard.BlockNbr  = (pCSD->DeviceSize + 1U) ;   /* 赋值或初始化 */
    hsd->SdCard.BlockNbr *= (1UL << ((pCSD->DeviceSizeMul & 0x07U) + 2U));  /* 赋值或初始化 */
    hsd->SdCard.BlockSize = (1UL << (pCSD->RdBlockLen & 0x0FU));  /* 赋值或初始化 */

    hsd->SdCard.LogBlockNbr =  (hsd->SdCard.BlockNbr) * ((hsd->SdCard.BlockSize) / 512U);  /* 赋值或初始化 */
    hsd->SdCard.LogBlockSize = 512U;                    /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if(hsd->SdCard.CardType == CARD_SDHC_SDXC)       /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* Byte 7 */
    pCSD->DeviceSize = (((hsd->CSD[1] & 0x0000003FU) << 16U) | ((hsd->CSD[2] & 0xFFFF0000U) >> 16U));  /* 赋值或初始化 */

    hsd->SdCard.BlockNbr = ((pCSD->DeviceSize + 1U) * 1024U);  /* 赋值或初始化 */
    hsd->SdCard.LogBlockNbr = hsd->SdCard.BlockNbr;     /* 赋值或初始化 */
    hsd->SdCard.BlockSize = 512U;                       /* 赋值或初始化 */
    hsd->SdCard.LogBlockSize = hsd->SdCard.BlockSize;   /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Clear all the static flags */
    __HAL_SD_CLEAR_FLAG(hsd, SDIO_STATIC_FLAGS);        /* HAL辅助宏/函数 */
    hsd->ErrorCode |= HAL_SD_ERROR_UNSUPPORTED_FEATURE;  /* 赋值或初始化 */
    hsd->State = HAL_SD_STATE_READY;                    /* 赋值或初始化 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  pCSD->EraseGrSize = (uint8_t)((hsd->CSD[2] & 0x00004000U) >> 14U);  /* 赋值或初始化 */

  pCSD->EraseGrMul = (uint8_t)((hsd->CSD[2] & 0x00003F80U) >> 7U);  /* 赋值或初始化 */

  pCSD->WrProtectGrSize = (uint8_t)(hsd->CSD[2] & 0x0000007FU);  /* 赋值或初始化 */

  pCSD->WrProtectGrEnable = (uint8_t)((hsd->CSD[3] & 0x80000000U) >> 31U);  /* 赋值或初始化 */

  pCSD->ManDeflECC = (uint8_t)((hsd->CSD[3] & 0x60000000U) >> 29U);  /* 赋值或初始化 */

  pCSD->WrSpeedFact = (uint8_t)((hsd->CSD[3] & 0x1C000000U) >> 26U);  /* 赋值或初始化 */

  pCSD->MaxWrBlockLen= (uint8_t)((hsd->CSD[3] & 0x03C00000U) >> 22U);  /* 赋值或初始化 */

  pCSD->WriteBlockPaPartial = (uint8_t)((hsd->CSD[3] & 0x00200000U) >> 21U);  /* 赋值或初始化 */

  pCSD->Reserved3 = 0;                                  /* 赋值或初始化 */

  pCSD->ContentProtectAppli = (uint8_t)((hsd->CSD[3] & 0x00010000U) >> 16U);  /* 赋值或初始化 */

  pCSD->FileFormatGroup = (uint8_t)((hsd->CSD[3] & 0x00008000U) >> 15U);  /* 赋值或初始化 */

  pCSD->CopyFlag = (uint8_t)((hsd->CSD[3] & 0x00004000U) >> 14U);  /* 赋值或初始化 */

  pCSD->PermWrProtect = (uint8_t)((hsd->CSD[3] & 0x00002000U) >> 13U);  /* 赋值或初始化 */

  pCSD->TempWrProtect = (uint8_t)((hsd->CSD[3] & 0x00001000U) >> 12U);  /* 赋值或初始化 */

  pCSD->FileFormat = (uint8_t)((hsd->CSD[3] & 0x00000C00U) >> 10U);  /* 赋值或初始化 */

  pCSD->ECC= (uint8_t)((hsd->CSD[3] & 0x00000300U) >> 8U);  /* 赋值或初始化 */

  pCSD->CSD_CRC = (uint8_t)((hsd->CSD[3] & 0x000000FEU) >> 1U);  /* 赋值或初始化 */

  pCSD->Reserved4 = 1;                                  /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Gets the SD status info.
  * @param  hsd: Pointer to SD handle
  * @param  pStatus: Pointer to the HAL_SD_CardStatusTypeDef structure that 
  *         will contain the SD card status information
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SD_GetCardStatus(SD_HandleTypeDef *hsd, HAL_SD_CardStatusTypeDef *pStatus)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t sd_status[16];                               /* 32位无符号变量/函数 */
  uint32_t errorstate;                                  /* 32位无符号变量/函数 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  errorstate = SD_SendSDStatus(hsd, sd_status);         /* 赋值或初始化 */
  if(errorstate != HAL_SD_ERROR_NONE)                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Clear all the static flags */
    __HAL_SD_CLEAR_FLAG(hsd, SDIO_STATIC_FLAGS);        /* HAL辅助宏/函数 */
    hsd->ErrorCode |= errorstate;                       /* 赋值或初始化 */
    hsd->State = HAL_SD_STATE_READY;                    /* 赋值或初始化 */
    status = HAL_ERROR;                                 /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    pStatus->DataBusWidth = (uint8_t)((sd_status[0] & 0xC0U) >> 6U);  /* 赋值或初始化 */

    pStatus->SecuredMode = (uint8_t)((sd_status[0] & 0x20U) >> 5U);  /* 赋值或初始化 */

    pStatus->CardType = (uint16_t)(((sd_status[0] & 0x00FF0000U) >> 8U) | ((sd_status[0] & 0xFF000000U) >> 24U));  /* 赋值或初始化 */

    pStatus->ProtectedAreaSize = (((sd_status[1] & 0xFFU) << 24U)    | ((sd_status[1] & 0xFF00U) << 8U) |  /* 源代码 */
                                  ((sd_status[1] & 0xFF0000U) >> 8U) | ((sd_status[1] & 0xFF000000U) >> 24U));  /* 执行语句 */

    pStatus->SpeedClass = (uint8_t)(sd_status[2] & 0xFFU);  /* 赋值或初始化 */

    pStatus->PerformanceMove = (uint8_t)((sd_status[2] & 0xFF00U) >> 8U);  /* 赋值或初始化 */

    pStatus->AllocationUnitSize = (uint8_t)((sd_status[2] & 0xF00000U) >> 20U);  /* 赋值或初始化 */

    pStatus->EraseSize = (uint16_t)(((sd_status[2] & 0xFF000000U) >> 16U) | (sd_status[3] & 0xFFU));  /* 赋值或初始化 */

    pStatus->EraseTimeout = (uint8_t)((sd_status[3] & 0xFC00U) >> 10U);  /* 赋值或初始化 */

    pStatus->EraseOffset = (uint8_t)((sd_status[3] & 0x0300U) >> 8U);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Set Block Size for Card */
  errorstate = SDMMC_CmdBlockLength(hsd->Instance, BLOCKSIZE);  /* 赋值或初始化 */
  if(errorstate != HAL_SD_ERROR_NONE)                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Clear all the static flags */
    __HAL_SD_CLEAR_FLAG(hsd, SDIO_STATIC_FLAGS);        /* HAL辅助宏/函数 */
    hsd->ErrorCode = errorstate;                        /* 赋值或初始化 */
    hsd->State = HAL_SD_STATE_READY;                    /* 赋值或初始化 */
    status = HAL_ERROR;                                 /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Gets the SD card info.
  * @param  hsd: Pointer to SD handle
  * @param  pCardInfo: Pointer to the HAL_SD_CardInfoTypeDef structure that
  *         will contain the SD card status information
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SD_GetCardInfo(SD_HandleTypeDef *hsd, HAL_SD_CardInfoTypeDef *pCardInfo)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  pCardInfo->CardType     = (uint32_t)(hsd->SdCard.CardType);  /* 赋值或初始化 */
  pCardInfo->CardVersion  = (uint32_t)(hsd->SdCard.CardVersion);  /* 赋值或初始化 */
  pCardInfo->Class        = (uint32_t)(hsd->SdCard.Class);  /* 赋值或初始化 */
  pCardInfo->RelCardAdd   = (uint32_t)(hsd->SdCard.RelCardAdd);  /* 赋值或初始化 */
  pCardInfo->BlockNbr     = (uint32_t)(hsd->SdCard.BlockNbr);  /* 赋值或初始化 */
  pCardInfo->BlockSize    = (uint32_t)(hsd->SdCard.BlockSize);  /* 赋值或初始化 */
  pCardInfo->LogBlockNbr  = (uint32_t)(hsd->SdCard.LogBlockNbr);  /* 赋值或初始化 */
  pCardInfo->LogBlockSize = (uint32_t)(hsd->SdCard.LogBlockSize);  /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Enables wide bus operation for the requested card if supported by
  *         card.
  * @param  hsd: Pointer to SD handle
  * @param  WideMode: Specifies the SD card wide bus mode
  *          This parameter can be one of the following values:
  *            @arg SDIO_BUS_WIDE_8B: 8-bit data transfer
  *            @arg SDIO_BUS_WIDE_4B: 4-bit data transfer
  *            @arg SDIO_BUS_WIDE_1B: 1-bit data transfer
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SD_ConfigWideBusOperation(SD_HandleTypeDef *hsd, uint32_t WideMode)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  SDIO_InitTypeDef Init;                                /* 执行语句 */
  uint32_t errorstate;                                  /* 32位无符号变量/函数 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Check the parameters */
  assert_param(IS_SDIO_BUS_WIDE(WideMode));             /* 执行语句 */

  /* Change State */
  hsd->State = HAL_SD_STATE_BUSY;                       /* 赋值或初始化 */

  if(hsd->SdCard.CardType != CARD_SECURED)              /* 条件判断 */
  {                                                     /* 代码块开始 */
    if(WideMode == SDIO_BUS_WIDE_8B)                    /* 条件判断 */
    {                                                   /* 代码块开始 */
      hsd->ErrorCode |= HAL_SD_ERROR_UNSUPPORTED_FEATURE;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if(WideMode == SDIO_BUS_WIDE_4B)               /* 否则如果 */
    {                                                   /* 代码块开始 */
      errorstate = SD_WideBus_Enable(hsd);              /* 赋值或初始化 */

      hsd->ErrorCode |= errorstate;                     /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if(WideMode == SDIO_BUS_WIDE_1B)               /* 否则如果 */
    {                                                   /* 代码块开始 */
      errorstate = SD_WideBus_Disable(hsd);             /* 赋值或初始化 */

      hsd->ErrorCode |= errorstate;                     /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* WideMode is not a valid argument*/
      hsd->ErrorCode |= HAL_SD_ERROR_PARAM;             /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* MMC Card does not support this feature */
    hsd->ErrorCode |= HAL_SD_ERROR_UNSUPPORTED_FEATURE;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if(hsd->ErrorCode != HAL_SD_ERROR_NONE)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Clear all the static flags */
    __HAL_SD_CLEAR_FLAG(hsd, SDIO_STATIC_FLAGS);        /* HAL辅助宏/函数 */
    hsd->State = HAL_SD_STATE_READY;                    /* 赋值或初始化 */
    status = HAL_ERROR;                                 /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Configure the SDIO peripheral */
    Init.ClockEdge           = hsd->Init.ClockEdge;     /* 赋值或初始化 */
    Init.ClockBypass         = hsd->Init.ClockBypass;   /* 赋值或初始化 */
    Init.ClockPowerSave      = hsd->Init.ClockPowerSave;  /* 赋值或初始化 */
    Init.BusWide             = WideMode;                /* 赋值或初始化 */
    Init.HardwareFlowControl = hsd->Init.HardwareFlowControl;  /* 赋值或初始化 */
    Init.ClockDiv            = hsd->Init.ClockDiv;      /* 赋值或初始化 */
    (void)SDIO_Init(hsd->Instance, Init);               /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Set Block Size for Card */
  errorstate = SDMMC_CmdBlockLength(hsd->Instance, BLOCKSIZE);  /* 赋值或初始化 */
  if(errorstate != HAL_SD_ERROR_NONE)                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Clear all the static flags */
    __HAL_SD_CLEAR_FLAG(hsd, SDIO_STATIC_FLAGS);        /* HAL辅助宏/函数 */
    hsd->ErrorCode |= errorstate;                       /* 赋值或初始化 */
    status = HAL_ERROR;                                 /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Change State */
  hsd->State = HAL_SD_STATE_READY;                      /* 赋值或初始化 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Gets the current sd card data state.
  * @param  hsd: pointer to SD handle
  * @retval Card state
  */
HAL_SD_CardStateTypeDef HAL_SD_GetCardState(SD_HandleTypeDef *hsd)  /* HAL库函数调用 */
{                                                       /* 代码块开始 */
  uint32_t cardstate;                                   /* 32位无符号变量/函数 */
  uint32_t errorstate;                                  /* 32位无符号变量/函数 */
  uint32_t resp1 = 0;                                   /* 32位无符号变量/函数 */

  errorstate = SD_SendStatus(hsd, &resp1);              /* 赋值或初始化 */
  if(errorstate != HAL_SD_ERROR_NONE)                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    hsd->ErrorCode |= errorstate;                       /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  cardstate = ((resp1 >> 9U) & 0x0FU);                  /* 赋值或初始化 */

  return (HAL_SD_CardStateTypeDef)cardstate;            /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Abort the current transfer and disable the SD.
  * @param  hsd: pointer to a SD_HandleTypeDef structure that contains
  *                the configuration information for SD module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SD_Abort(SD_HandleTypeDef *hsd)   /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_SD_CardStateTypeDef CardState;                    /* HAL库函数调用 */
  uint32_t context = hsd->Context;                      /* 32位无符号变量/函数 */

  /* DIsable All interrupts */
  __HAL_SD_DISABLE_IT(hsd, SDIO_IT_DATAEND | SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT|\
                           SDIO_IT_TXUNDERR| SDIO_IT_RXOVERR);  /* 执行语句 */

  /* Clear All flags */
  __HAL_SD_CLEAR_FLAG(hsd, SDIO_STATIC_DATA_FLAGS);     /* HAL辅助宏/函数 */

  CLEAR_BIT(hsd->Instance->DCTRL, SDIO_DCTRL_DTEN);     /* 执行语句 */

  if ((context & SD_CONTEXT_DMA) != 0U)                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable the SD DMA request */
    hsd->Instance->DCTRL &= (uint32_t)~((uint32_t)SDIO_DCTRL_DMAEN);  /* 赋值或初始化 */

    /* Abort the SD DMA Tx channel */
    if (((context & SD_CONTEXT_WRITE_SINGLE_BLOCK) != 0U) || ((context & SD_CONTEXT_WRITE_MULTIPLE_BLOCK) != 0U))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      if(HAL_DMA_Abort(hsd->hdmatx) != HAL_OK)          /* 条件判断 */
      {                                                 /* 代码块开始 */
        hsd->ErrorCode |= HAL_SD_ERROR_DMA;             /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    /* Abort the SD DMA Rx channel */
    else if (((context & SD_CONTEXT_READ_SINGLE_BLOCK) != 0U) || ((context & SD_CONTEXT_READ_MULTIPLE_BLOCK) != 0U))  /* 否则如果 */
    {                                                   /* 代码块开始 */
      if(HAL_DMA_Abort(hsd->hdmarx) != HAL_OK)          /* 条件判断 */
      {                                                 /* 代码块开始 */
        hsd->ErrorCode |= HAL_SD_ERROR_DMA;             /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Nothing to do */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  hsd->State = HAL_SD_STATE_READY;                      /* 赋值或初始化 */

  /* Initialize the SD operation */
  hsd->Context = SD_CONTEXT_NONE;                       /* 赋值或初始化 */

  CardState = HAL_SD_GetCardState(hsd);                 /* 赋值或初始化 */
  if((CardState == HAL_SD_CARD_RECEIVING) || (CardState == HAL_SD_CARD_SENDING))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hsd->ErrorCode = SDMMC_CmdStopTransfer(hsd->Instance);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  if(hsd->ErrorCode != HAL_SD_ERROR_NONE)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Abort the current transfer and disable the SD (IT mode).
  * @param  hsd: pointer to a SD_HandleTypeDef structure that contains
  *                the configuration information for SD module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SD_Abort_IT(SD_HandleTypeDef *hsd)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_SD_CardStateTypeDef CardState;                    /* HAL库函数调用 */
  uint32_t context = hsd->Context;                      /* 32位无符号变量/函数 */

  /* Disable All interrupts */
  __HAL_SD_DISABLE_IT(hsd, SDIO_IT_DATAEND | SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT|\
                           SDIO_IT_TXUNDERR| SDIO_IT_RXOVERR);  /* 执行语句 */

  CLEAR_BIT(hsd->Instance->DCTRL, SDIO_DCTRL_DTEN);     /* 执行语句 */

  if ((context & SD_CONTEXT_DMA) != 0U)                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable the SD DMA request */
    hsd->Instance->DCTRL &= (uint32_t)~((uint32_t)SDIO_DCTRL_DMAEN);  /* 赋值或初始化 */

    /* Abort the SD DMA Tx channel */
    if (((context & SD_CONTEXT_WRITE_SINGLE_BLOCK) != 0U) || ((context & SD_CONTEXT_WRITE_MULTIPLE_BLOCK) != 0U))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hsd->hdmatx->XferAbortCallback = SD_DMATxAbort;   /* 赋值或初始化 */
      if(HAL_DMA_Abort_IT(hsd->hdmatx) != HAL_OK)       /* 条件判断 */
      {                                                 /* 代码块开始 */
        hsd->hdmatx = NULL;                             /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    /* Abort the SD DMA Rx channel */
    else if (((context & SD_CONTEXT_READ_SINGLE_BLOCK) != 0U) || ((context & SD_CONTEXT_READ_MULTIPLE_BLOCK) != 0U))  /* 否则如果 */
    {                                                   /* 代码块开始 */
      hsd->hdmarx->XferAbortCallback = SD_DMARxAbort;   /* 赋值或初始化 */
      if(HAL_DMA_Abort_IT(hsd->hdmarx) != HAL_OK)       /* 条件判断 */
      {                                                 /* 代码块开始 */
        hsd->hdmarx = NULL;                             /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Nothing to do */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /* No transfer ongoing on both DMA channels*/
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Clear All flags */
    __HAL_SD_CLEAR_FLAG(hsd, SDIO_STATIC_DATA_FLAGS);   /* HAL辅助宏/函数 */

    CardState = HAL_SD_GetCardState(hsd);               /* 赋值或初始化 */
    hsd->State = HAL_SD_STATE_READY;                    /* 赋值或初始化 */
    hsd->Context = SD_CONTEXT_NONE;                     /* 赋值或初始化 */
    if((CardState == HAL_SD_CARD_RECEIVING) || (CardState == HAL_SD_CARD_SENDING))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hsd->ErrorCode = SDMMC_CmdStopTransfer(hsd->Instance);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    if(hsd->ErrorCode != HAL_SD_ERROR_NONE)             /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
#if defined (USE_HAL_SD_REGISTER_CALLBACKS) && (USE_HAL_SD_REGISTER_CALLBACKS == 1U)  /* 条件编译 */
      hsd->AbortCpltCallback(hsd);                      /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_SD_AbortCallback(hsd);                        /* HAL库函数调用 */
#endif /* USE_HAL_SD_REGISTER_CALLBACKS */
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

/* Private function ----------------------------------------------------------*/
/** @addtogroup SD_Private_Functions
  * @{
  */

/**
  * @brief  DMA SD transmit process complete callback
  * @param  hdma: DMA handle
  * @retval None
  */
static void SD_DMATransmitCplt(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  SD_HandleTypeDef* hsd = (SD_HandleTypeDef* )(hdma->Parent);  /* 赋值或初始化 */

  /* Enable DATAEND Interrupt */
  __HAL_SD_ENABLE_IT(hsd, (SDIO_IT_DATAEND));           /* HAL辅助宏/函数 */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA SD receive process complete callback
  * @param  hdma: DMA handle
  * @retval None
  */
static void SD_DMAReceiveCplt(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  SD_HandleTypeDef* hsd = (SD_HandleTypeDef* )(hdma->Parent);  /* 赋值或初始化 */
  uint32_t errorstate;                                  /* 32位无符号变量/函数 */

  /* Send stop command in multiblock write */
  if(hsd->Context == (SD_CONTEXT_READ_MULTIPLE_BLOCK | SD_CONTEXT_DMA))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    errorstate = SDMMC_CmdStopTransfer(hsd->Instance);  /* 赋值或初始化 */
    if(errorstate != HAL_SD_ERROR_NONE)                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      hsd->ErrorCode |= errorstate;                     /* 赋值或初始化 */
#if (USE_HAL_SD_REGISTER_CALLBACKS == 1)                /* 条件编译 */
      hsd->ErrorCallback(hsd);                          /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_SD_ErrorCallback(hsd);                        /* HAL库函数调用 */
#endif                                                  /* 条件编译结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Disable the DMA transfer for transmit request by setting the DMAEN bit
  in the SD DCTRL register */
  hsd->Instance->DCTRL &= (uint32_t)~((uint32_t)SDIO_DCTRL_DMAEN);  /* 赋值或初始化 */

  /* Clear all the static flags */
  __HAL_SD_CLEAR_FLAG(hsd, SDIO_STATIC_DATA_FLAGS);     /* HAL辅助宏/函数 */

  hsd->State = HAL_SD_STATE_READY;                      /* 赋值或初始化 */
  hsd->Context = SD_CONTEXT_NONE;                       /* 赋值或初始化 */

#if (USE_HAL_SD_REGISTER_CALLBACKS == 1)                /* 条件编译 */
  hsd->RxCpltCallback(hsd);                             /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_SD_RxCpltCallback(hsd);                           /* HAL库函数调用 */
#endif                                                  /* 条件编译结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA SD communication error callback
  * @param  hdma: DMA handle
  * @retval None
  */
static void SD_DMAError(DMA_HandleTypeDef *hdma)        /* 静态函数 */
{                                                       /* 代码块开始 */
  SD_HandleTypeDef* hsd = (SD_HandleTypeDef* )(hdma->Parent);  /* 赋值或初始化 */
  HAL_SD_CardStateTypeDef CardState;                    /* HAL库函数调用 */
  uint32_t RxErrorCode, TxErrorCode;                    /* 32位无符号变量/函数 */

  /* if DMA error is FIFO error ignore it */
  if(HAL_DMA_GetError(hdma) != HAL_DMA_ERROR_FE)        /* 条件判断 */
  {                                                     /* 代码块开始 */
    RxErrorCode = hsd->hdmarx->ErrorCode;               /* 赋值或初始化 */
    TxErrorCode = hsd->hdmatx->ErrorCode;               /* 赋值或初始化 */
    if((RxErrorCode == HAL_DMA_ERROR_TE) || (TxErrorCode == HAL_DMA_ERROR_TE))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear All flags */
      __HAL_SD_CLEAR_FLAG(hsd, SDIO_STATIC_FLAGS);      /* HAL辅助宏/函数 */

      /* Disable All interrupts */
      __HAL_SD_DISABLE_IT(hsd, SDIO_IT_DATAEND | SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT|\
        SDIO_IT_TXUNDERR| SDIO_IT_RXOVERR);             /* 执行语句 */

      hsd->ErrorCode |= HAL_SD_ERROR_DMA;               /* 赋值或初始化 */
      CardState = HAL_SD_GetCardState(hsd);             /* 赋值或初始化 */
      if((CardState == HAL_SD_CARD_RECEIVING) || (CardState == HAL_SD_CARD_SENDING))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        hsd->ErrorCode |= SDMMC_CmdStopTransfer(hsd->Instance);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */

      hsd->State= HAL_SD_STATE_READY;                   /* 赋值或初始化 */
      hsd->Context = SD_CONTEXT_NONE;                   /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

#if (USE_HAL_SD_REGISTER_CALLBACKS == 1)                /* 条件编译 */
    hsd->ErrorCallback(hsd);                            /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_SD_ErrorCallback(hsd);                          /* HAL库函数调用 */
#endif                                                  /* 条件编译结束 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA SD Tx Abort callback
  * @param  hdma: DMA handle
  * @retval None
  */
static void SD_DMATxAbort(DMA_HandleTypeDef *hdma)      /* 静态函数 */
{                                                       /* 代码块开始 */
  SD_HandleTypeDef* hsd = (SD_HandleTypeDef* )(hdma->Parent);  /* 赋值或初始化 */
  HAL_SD_CardStateTypeDef CardState;                    /* HAL库函数调用 */

  /* Clear All flags */
  __HAL_SD_CLEAR_FLAG(hsd, SDIO_STATIC_DATA_FLAGS);     /* HAL辅助宏/函数 */

  CardState = HAL_SD_GetCardState(hsd);                 /* 赋值或初始化 */
  hsd->State = HAL_SD_STATE_READY;                      /* 赋值或初始化 */
  hsd->Context = SD_CONTEXT_NONE;                       /* 赋值或初始化 */
  if((CardState == HAL_SD_CARD_RECEIVING) || (CardState == HAL_SD_CARD_SENDING))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hsd->ErrorCode |= SDMMC_CmdStopTransfer(hsd->Instance);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if(hsd->ErrorCode == HAL_SD_ERROR_NONE)               /* 条件判断 */
  {                                                     /* 代码块开始 */
#if (USE_HAL_SD_REGISTER_CALLBACKS == 1)                /* 条件编译 */
    hsd->AbortCpltCallback(hsd);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_SD_AbortCallback(hsd);                          /* HAL库函数调用 */
#endif                                                  /* 条件编译结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
#if (USE_HAL_SD_REGISTER_CALLBACKS == 1)                /* 条件编译 */
    hsd->ErrorCallback(hsd);                            /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_SD_ErrorCallback(hsd);                          /* HAL库函数调用 */
#endif                                                  /* 条件编译结束 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA SD Rx Abort callback
  * @param  hdma: DMA handle
  * @retval None
  */
static void SD_DMARxAbort(DMA_HandleTypeDef *hdma)      /* 静态函数 */
{                                                       /* 代码块开始 */
  SD_HandleTypeDef* hsd = (SD_HandleTypeDef* )(hdma->Parent);  /* 赋值或初始化 */
  HAL_SD_CardStateTypeDef CardState;                    /* HAL库函数调用 */

  /* Clear All flags */
  __HAL_SD_CLEAR_FLAG(hsd, SDIO_STATIC_DATA_FLAGS);     /* HAL辅助宏/函数 */

  CardState = HAL_SD_GetCardState(hsd);                 /* 赋值或初始化 */
  hsd->State = HAL_SD_STATE_READY;                      /* 赋值或初始化 */
  hsd->Context = SD_CONTEXT_NONE;                       /* 赋值或初始化 */
  if((CardState == HAL_SD_CARD_RECEIVING) || (CardState == HAL_SD_CARD_SENDING))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hsd->ErrorCode |= SDMMC_CmdStopTransfer(hsd->Instance);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if(hsd->ErrorCode == HAL_SD_ERROR_NONE)               /* 条件判断 */
  {                                                     /* 代码块开始 */
#if (USE_HAL_SD_REGISTER_CALLBACKS == 1)                /* 条件编译 */
    hsd->AbortCpltCallback(hsd);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_SD_AbortCallback(hsd);                          /* HAL库函数调用 */
#endif                                                  /* 条件编译结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
#if (USE_HAL_SD_REGISTER_CALLBACKS == 1)                /* 条件编译 */
    hsd->ErrorCallback(hsd);                            /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_SD_ErrorCallback(hsd);                          /* HAL库函数调用 */
#endif                                                  /* 条件编译结束 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initializes the sd card.
  * @param  hsd: Pointer to SD handle
  * @retval SD Card error state
  */
static uint32_t SD_InitCard(SD_HandleTypeDef *hsd)      /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  HAL_SD_CardCSDTypeDef CSD;                            /* HAL库函数调用 */
  uint32_t errorstate;                                  /* 32位无符号变量/函数 */
  uint16_t sd_rca = 1U;                                 /* 16位无符号变量/函数 */

  /* Check the power State */
  if(SDIO_GetPowerState(hsd->Instance) == 0U)           /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Power off */
    return HAL_SD_ERROR_REQUEST_NOT_APPLICABLE;         /* 返回值 */
  }                                                     /* 代码块结束 */

  if(hsd->SdCard.CardType != CARD_SECURED)              /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Send CMD2 ALL_SEND_CID */
    errorstate = SDMMC_CmdSendCID(hsd->Instance);       /* 赋值或初始化 */
    if(errorstate != HAL_SD_ERROR_NONE)                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      return errorstate;                                /* 返回值 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Get Card identification number data */
      hsd->CID[0U] = SDIO_GetResponse(hsd->Instance, SDIO_RESP1);  /* 赋值或初始化 */
      hsd->CID[1U] = SDIO_GetResponse(hsd->Instance, SDIO_RESP2);  /* 赋值或初始化 */
      hsd->CID[2U] = SDIO_GetResponse(hsd->Instance, SDIO_RESP3);  /* 赋值或初始化 */
      hsd->CID[3U] = SDIO_GetResponse(hsd->Instance, SDIO_RESP4);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  if(hsd->SdCard.CardType != CARD_SECURED)              /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Send CMD3 SET_REL_ADDR with argument 0 */
    /* SD Card publishes its RCA. */
    errorstate = SDMMC_CmdSetRelAdd(hsd->Instance, &sd_rca);  /* 赋值或初始化 */
    if(errorstate != HAL_SD_ERROR_NONE)                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      return errorstate;                                /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  if(hsd->SdCard.CardType != CARD_SECURED)              /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Get the SD card RCA */
    hsd->SdCard.RelCardAdd = sd_rca;                    /* 赋值或初始化 */

    /* Send CMD9 SEND_CSD with argument as card's RCA */
    errorstate = SDMMC_CmdSendCSD(hsd->Instance, (uint32_t)(hsd->SdCard.RelCardAdd << 16U));  /* 赋值或初始化 */
    if(errorstate != HAL_SD_ERROR_NONE)                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      return errorstate;                                /* 返回值 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Get Card Specific Data */
      hsd->CSD[0U] = SDIO_GetResponse(hsd->Instance, SDIO_RESP1);  /* 赋值或初始化 */
      hsd->CSD[1U] = SDIO_GetResponse(hsd->Instance, SDIO_RESP2);  /* 赋值或初始化 */
      hsd->CSD[2U] = SDIO_GetResponse(hsd->Instance, SDIO_RESP3);  /* 赋值或初始化 */
      hsd->CSD[3U] = SDIO_GetResponse(hsd->Instance, SDIO_RESP4);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Get the Card Class */
  hsd->SdCard.Class = (SDIO_GetResponse(hsd->Instance, SDIO_RESP2) >> 20U);  /* 赋值或初始化 */

  /* Get CSD parameters */
  if (HAL_SD_GetCardCSD(hsd, &CSD) != HAL_OK)           /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_SD_ERROR_UNSUPPORTED_FEATURE;            /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Select the Card */
  errorstate = SDMMC_CmdSelDesel(hsd->Instance, (uint32_t)(((uint32_t)hsd->SdCard.RelCardAdd) << 16U));  /* 赋值或初始化 */
  if(errorstate != HAL_SD_ERROR_NONE)                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    return errorstate;                                  /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Configure SDIO peripheral interface */
  (void)SDIO_Init(hsd->Instance, hsd->Init);            /* 执行语句 */

  /* All cards are initialized */
  return HAL_SD_ERROR_NONE;                             /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Enquires cards about their operating voltage and configures clock
  *         controls and stores SD information that will be needed in future
  *         in the SD handle.
  * @param  hsd: Pointer to SD handle
  * @retval error state
  */
static uint32_t SD_PowerON(SD_HandleTypeDef *hsd)       /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  __IO uint32_t count = 0U;                             /* 赋值或初始化 */
  uint32_t response = 0U, validvoltage = 0U;            /* 32位无符号变量/函数 */
  uint32_t errorstate;                                  /* 32位无符号变量/函数 */

  /* CMD0: GO_IDLE_STATE */
  errorstate = SDMMC_CmdGoIdleState(hsd->Instance);     /* 赋值或初始化 */
  if(errorstate != HAL_SD_ERROR_NONE)                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    return errorstate;                                  /* 返回值 */
  }                                                     /* 代码块结束 */

  /* CMD8: SEND_IF_COND: Command available only on V2.0 cards */
  errorstate = SDMMC_CmdOperCond(hsd->Instance);        /* 赋值或初始化 */
  if(errorstate != HAL_SD_ERROR_NONE)                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    hsd->SdCard.CardVersion = CARD_V1_X;                /* 赋值或初始化 */
    /* CMD0: GO_IDLE_STATE */
    errorstate = SDMMC_CmdGoIdleState(hsd->Instance);   /* 赋值或初始化 */
    if(errorstate != HAL_SD_ERROR_NONE)                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      return errorstate;                                /* 返回值 */
    }                                                   /* 代码块结束 */

  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    hsd->SdCard.CardVersion = CARD_V2_X;                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if( hsd->SdCard.CardVersion == CARD_V2_X)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* SEND CMD55 APP_CMD with RCA as 0 */
    errorstate = SDMMC_CmdAppCommand(hsd->Instance, 0);  /* 赋值或初始化 */
    if(errorstate != HAL_SD_ERROR_NONE)                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_SD_ERROR_UNSUPPORTED_FEATURE;          /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /* SD CARD */
  /* Send ACMD41 SD_APP_OP_COND with Argument 0x80100000 */
  while((count < SDMMC_MAX_VOLT_TRIAL) && (validvoltage == 0U))  /* while循环 */
  {                                                     /* 代码块开始 */
    /* SEND CMD55 APP_CMD with RCA as 0 */
    errorstate = SDMMC_CmdAppCommand(hsd->Instance, 0);  /* 赋值或初始化 */
    if(errorstate != HAL_SD_ERROR_NONE)                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      return errorstate;                                /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Send CMD41 */
    errorstate = SDMMC_CmdAppOperCommand(hsd->Instance, SDMMC_VOLTAGE_WINDOW_SD | SDMMC_HIGH_CAPACITY | SD_SWITCH_1_8V_CAPACITY);  /* 赋值或初始化 */
    if(errorstate != HAL_SD_ERROR_NONE)                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_SD_ERROR_UNSUPPORTED_FEATURE;          /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Get command response */
    response = SDIO_GetResponse(hsd->Instance, SDIO_RESP1);  /* 赋值或初始化 */

    /* Get operating voltage*/
    validvoltage = (((response >> 31U) == 1U) ? 1U : 0U);  /* 赋值或初始化 */

    count++;                                            /* 执行语句 */
  }                                                     /* 代码块结束 */

  if(count >= SDMMC_MAX_VOLT_TRIAL)                     /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_SD_ERROR_INVALID_VOLTRANGE;              /* 返回值 */
  }                                                     /* 代码块结束 */

  if((response & SDMMC_HIGH_CAPACITY) == SDMMC_HIGH_CAPACITY) /* (response &= SD_HIGH_CAPACITY) */
  {                                                     /* 代码块开始 */
    hsd->SdCard.CardType = CARD_SDHC_SDXC;              /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    hsd->SdCard.CardType = CARD_SDSC;                   /* 赋值或初始化 */
  }                                                     /* 代码块结束 */


  return HAL_SD_ERROR_NONE;                             /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Turns the SDIO output signals off.
  * @param  hsd: Pointer to SD handle
  * @retval None
  */
static void SD_PowerOFF(SD_HandleTypeDef *hsd)          /* 静态函数 */
{                                                       /* 代码块开始 */
  /* Set Power State to OFF */
  (void)SDIO_PowerState_OFF(hsd->Instance);             /* 执行语句 */
}                                                       /* 代码块结束 */

/**
  * @brief  Send Status info command.
  * @param  hsd: pointer to SD handle
  * @param  pSDstatus: Pointer to the buffer that will contain the SD card status
  *         SD Status register)
  * @retval error state
  */
static uint32_t SD_SendSDStatus(SD_HandleTypeDef *hsd, uint32_t *pSDstatus)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  SDIO_DataInitTypeDef config;                          /* 执行语句 */
  uint32_t errorstate;                                  /* 32位无符号变量/函数 */
  uint32_t tickstart = HAL_GetTick();                   /* 32位无符号变量/函数 */
  uint32_t count;                                       /* 32位无符号变量/函数 */
  uint32_t *pData = pSDstatus;                          /* 32位无符号变量/函数 */

  /* Check SD response */
  if((SDIO_GetResponse(hsd->Instance, SDIO_RESP1) & SDMMC_CARD_LOCKED) == SDMMC_CARD_LOCKED)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_SD_ERROR_LOCK_UNLOCK_FAILED;             /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Set block size for card if it is not equal to current block size for card */
  errorstate = SDMMC_CmdBlockLength(hsd->Instance, 64U);  /* 赋值或初始化 */
  if(errorstate != HAL_SD_ERROR_NONE)                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    hsd->ErrorCode |= HAL_SD_ERROR_NONE;                /* 赋值或初始化 */
    return errorstate;                                  /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Send CMD55 */
  errorstate = SDMMC_CmdAppCommand(hsd->Instance, (uint32_t)(hsd->SdCard.RelCardAdd << 16U));  /* 赋值或初始化 */
  if(errorstate != HAL_SD_ERROR_NONE)                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    hsd->ErrorCode |= HAL_SD_ERROR_NONE;                /* 赋值或初始化 */
    return errorstate;                                  /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Configure the SD DPSM (Data Path State Machine) */
  config.DataTimeOut   = SDMMC_DATATIMEOUT;             /* 赋值或初始化 */
  config.DataLength    = 64U;                           /* 赋值或初始化 */
  config.DataBlockSize = SDIO_DATABLOCK_SIZE_64B;       /* 赋值或初始化 */
  config.TransferDir   = SDIO_TRANSFER_DIR_TO_SDIO;     /* 赋值或初始化 */
  config.TransferMode  = SDIO_TRANSFER_MODE_BLOCK;      /* 赋值或初始化 */
  config.DPSM          = SDIO_DPSM_ENABLE;              /* 赋值或初始化 */
  (void)SDIO_ConfigData(hsd->Instance, &config);        /* 执行语句 */

  /* Send ACMD13 (SD_APP_STAUS)  with argument as card's RCA */
  errorstate = SDMMC_CmdStatusRegister(hsd->Instance);  /* 赋值或初始化 */
  if(errorstate != HAL_SD_ERROR_NONE)                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    hsd->ErrorCode |= HAL_SD_ERROR_NONE;                /* 赋值或初始化 */
    return errorstate;                                  /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Get status data */
  while(!__HAL_SD_GET_FLAG(hsd, SDIO_FLAG_RXOVERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DBCKEND))  /* while循环 */
  {                                                     /* 代码块开始 */
    if(__HAL_SD_GET_FLAG(hsd, SDIO_FLAG_RXFIFOHF))      /* 条件判断 */
    {                                                   /* 代码块开始 */
      for(count = 0U; count < 8U; count++)              /* for循环 */
      {                                                 /* 代码块开始 */
        *pData = SDIO_ReadFIFO(hsd->Instance);          /* 赋值或初始化 */
        pData++;                                        /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    if((HAL_GetTick() - tickstart) >=  SDMMC_DATATIMEOUT)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_SD_ERROR_TIMEOUT;                      /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  if(__HAL_SD_GET_FLAG(hsd, SDIO_FLAG_DTIMEOUT))        /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_SD_ERROR_DATA_TIMEOUT;                   /* 返回值 */
  }                                                     /* 代码块结束 */
  else if(__HAL_SD_GET_FLAG(hsd, SDIO_FLAG_DCRCFAIL))   /* 否则如果 */
  {                                                     /* 代码块开始 */
    return HAL_SD_ERROR_DATA_CRC_FAIL;                  /* 返回值 */
  }                                                     /* 代码块结束 */
  else if(__HAL_SD_GET_FLAG(hsd, SDIO_FLAG_RXOVERR))    /* 否则如果 */
  {                                                     /* 代码块开始 */
    return HAL_SD_ERROR_RX_OVERRUN;                     /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Nothing to do */
  }                                                     /* 代码块结束 */

  while ((__HAL_SD_GET_FLAG(hsd, SDIO_FLAG_RXDAVL)))    /* while循环 */
  {                                                     /* 代码块开始 */
    *pData = SDIO_ReadFIFO(hsd->Instance);              /* 赋值或初始化 */
    pData++;                                            /* 执行语句 */

    if((HAL_GetTick() - tickstart) >=  SDMMC_DATATIMEOUT)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_SD_ERROR_TIMEOUT;                      /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Clear all the static status flags*/
  __HAL_SD_CLEAR_FLAG(hsd, SDIO_STATIC_DATA_FLAGS);     /* HAL辅助宏/函数 */

  return HAL_SD_ERROR_NONE;                             /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Returns the current card's status.
  * @param  hsd: Pointer to SD handle
  * @param  pCardStatus: pointer to the buffer that will contain the SD card
  *         status (Card Status register)
  * @retval error state
  */
static uint32_t SD_SendStatus(SD_HandleTypeDef *hsd, uint32_t *pCardStatus)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t errorstate;                                  /* 32位无符号变量/函数 */

  if(pCardStatus == NULL)                               /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_SD_ERROR_PARAM;                          /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Send Status command */
  errorstate = SDMMC_CmdSendStatus(hsd->Instance, (uint32_t)(hsd->SdCard.RelCardAdd << 16U));  /* 赋值或初始化 */
  if(errorstate != HAL_SD_ERROR_NONE)                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    return errorstate;                                  /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Get SD card status */
  *pCardStatus = SDIO_GetResponse(hsd->Instance, SDIO_RESP1);  /* 赋值或初始化 */

  return HAL_SD_ERROR_NONE;                             /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Enables the SDIO wide bus mode.
  * @param  hsd: pointer to SD handle
  * @retval error state
  */
static uint32_t SD_WideBus_Enable(SD_HandleTypeDef *hsd)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t scr[2U] = {0U, 0U};                          /* 32位无符号变量/函数 */
  uint32_t errorstate;                                  /* 32位无符号变量/函数 */

  if((SDIO_GetResponse(hsd->Instance, SDIO_RESP1) & SDMMC_CARD_LOCKED) == SDMMC_CARD_LOCKED)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_SD_ERROR_LOCK_UNLOCK_FAILED;             /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Get SCR Register */
  errorstate = SD_FindSCR(hsd, scr);                    /* 赋值或初始化 */
  if(errorstate != HAL_SD_ERROR_NONE)                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    return errorstate;                                  /* 返回值 */
  }                                                     /* 代码块结束 */

  /* If requested card supports wide bus operation */
  if((scr[1U] & SDMMC_WIDE_BUS_SUPPORT) != SDMMC_ALLZERO)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Send CMD55 APP_CMD with argument as card's RCA.*/
    errorstate = SDMMC_CmdAppCommand(hsd->Instance, (uint32_t)(hsd->SdCard.RelCardAdd << 16U));  /* 赋值或初始化 */
    if(errorstate != HAL_SD_ERROR_NONE)                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      return errorstate;                                /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Send ACMD6 APP_CMD with argument as 2 for wide bus mode */
    errorstate = SDMMC_CmdBusWidth(hsd->Instance, 2U);  /* 赋值或初始化 */
    if(errorstate != HAL_SD_ERROR_NONE)                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      return errorstate;                                /* 返回值 */
    }                                                   /* 代码块结束 */

    return HAL_SD_ERROR_NONE;                           /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_SD_ERROR_REQUEST_NOT_APPLICABLE;         /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Disables the SDIO wide bus mode.
  * @param  hsd: Pointer to SD handle
  * @retval error state
  */
static uint32_t SD_WideBus_Disable(SD_HandleTypeDef *hsd)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t scr[2U] = {0U, 0U};                          /* 32位无符号变量/函数 */
  uint32_t errorstate;                                  /* 32位无符号变量/函数 */

  if((SDIO_GetResponse(hsd->Instance, SDIO_RESP1) & SDMMC_CARD_LOCKED) == SDMMC_CARD_LOCKED)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_SD_ERROR_LOCK_UNLOCK_FAILED;             /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Get SCR Register */
  errorstate = SD_FindSCR(hsd, scr);                    /* 赋值或初始化 */
  if(errorstate != HAL_SD_ERROR_NONE)                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    return errorstate;                                  /* 返回值 */
  }                                                     /* 代码块结束 */

  /* If requested card supports 1 bit mode operation */
  if((scr[1U] & SDMMC_SINGLE_BUS_SUPPORT) != SDMMC_ALLZERO)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Send CMD55 APP_CMD with argument as card's RCA */
    errorstate = SDMMC_CmdAppCommand(hsd->Instance, (uint32_t)(hsd->SdCard.RelCardAdd << 16U));  /* 赋值或初始化 */
    if(errorstate != HAL_SD_ERROR_NONE)                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      return errorstate;                                /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Send ACMD6 APP_CMD with argument as 0 for single bus mode */
    errorstate = SDMMC_CmdBusWidth(hsd->Instance, 0U);  /* 赋值或初始化 */
    if(errorstate != HAL_SD_ERROR_NONE)                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      return errorstate;                                /* 返回值 */
    }                                                   /* 代码块结束 */

    return HAL_SD_ERROR_NONE;                           /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_SD_ERROR_REQUEST_NOT_APPLICABLE;         /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */


/**
  * @brief  Finds the SD card SCR register value.
  * @param  hsd: Pointer to SD handle
  * @param  pSCR: pointer to the buffer that will contain the SCR value
  * @retval error state
  */
static uint32_t SD_FindSCR(SD_HandleTypeDef *hsd, uint32_t *pSCR)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  SDIO_DataInitTypeDef config;                          /* 执行语句 */
  uint32_t errorstate;                                  /* 32位无符号变量/函数 */
  uint32_t tickstart = HAL_GetTick();                   /* 32位无符号变量/函数 */
  uint32_t index = 0U;                                  /* 32位无符号变量/函数 */
  uint32_t tempscr[2U] = {0U, 0U};                      /* 32位无符号变量/函数 */
  uint32_t *scr = pSCR;                                 /* 32位无符号变量/函数 */

  /* Set Block Size To 8 Bytes */
  errorstate = SDMMC_CmdBlockLength(hsd->Instance, 8U);  /* 赋值或初始化 */
  if(errorstate != HAL_SD_ERROR_NONE)                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    return errorstate;                                  /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Send CMD55 APP_CMD with argument as card's RCA */
  errorstate = SDMMC_CmdAppCommand(hsd->Instance, (uint32_t)((hsd->SdCard.RelCardAdd) << 16U));  /* 赋值或初始化 */
  if(errorstate != HAL_SD_ERROR_NONE)                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    return errorstate;                                  /* 返回值 */
  }                                                     /* 代码块结束 */

  config.DataTimeOut   = SDMMC_DATATIMEOUT;             /* 赋值或初始化 */
  config.DataLength    = 8U;                            /* 赋值或初始化 */
  config.DataBlockSize = SDIO_DATABLOCK_SIZE_8B;        /* 赋值或初始化 */
  config.TransferDir   = SDIO_TRANSFER_DIR_TO_SDIO;     /* 赋值或初始化 */
  config.TransferMode  = SDIO_TRANSFER_MODE_BLOCK;      /* 赋值或初始化 */
  config.DPSM          = SDIO_DPSM_ENABLE;              /* 赋值或初始化 */
  (void)SDIO_ConfigData(hsd->Instance, &config);        /* 执行语句 */

  /* Send ACMD51 SD_APP_SEND_SCR with argument as 0 */
  errorstate = SDMMC_CmdSendSCR(hsd->Instance);         /* 赋值或初始化 */
  if(errorstate != HAL_SD_ERROR_NONE)                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    return errorstate;                                  /* 返回值 */
  }                                                     /* 代码块结束 */

  while(!__HAL_SD_GET_FLAG(hsd, SDIO_FLAG_RXOVERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT))  /* while循环 */
  {                                                     /* 代码块开始 */
    if(__HAL_SD_GET_FLAG(hsd, SDIO_FLAG_RXDAVL))        /* 条件判断 */
    {                                                   /* 代码块开始 */
      *(tempscr + index) = SDIO_ReadFIFO(hsd->Instance);  /* 赋值或初始化 */
      index++;                                          /* 执行语句 */
    }                                                   /* 代码块结束 */
    else if(!__HAL_SD_GET_FLAG(hsd, SDIO_FLAG_RXACT))   /* 否则如果 */
    {                                                   /* 代码块开始 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */

    if((HAL_GetTick() - tickstart) >=  SDMMC_DATATIMEOUT)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_SD_ERROR_TIMEOUT;                      /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  if(__HAL_SD_GET_FLAG(hsd, SDIO_FLAG_DTIMEOUT))        /* 条件判断 */
  {                                                     /* 代码块开始 */
    __HAL_SD_CLEAR_FLAG(hsd, SDIO_FLAG_DTIMEOUT);       /* HAL辅助宏/函数 */

    return HAL_SD_ERROR_DATA_TIMEOUT;                   /* 返回值 */
  }                                                     /* 代码块结束 */
  else if(__HAL_SD_GET_FLAG(hsd, SDIO_FLAG_DCRCFAIL))   /* 否则如果 */
  {                                                     /* 代码块开始 */
    __HAL_SD_CLEAR_FLAG(hsd, SDIO_FLAG_DCRCFAIL);       /* HAL辅助宏/函数 */

    return HAL_SD_ERROR_DATA_CRC_FAIL;                  /* 返回值 */
  }                                                     /* 代码块结束 */
  else if(__HAL_SD_GET_FLAG(hsd, SDIO_FLAG_RXOVERR))    /* 否则如果 */
  {                                                     /* 代码块开始 */
    __HAL_SD_CLEAR_FLAG(hsd, SDIO_FLAG_RXOVERR);        /* HAL辅助宏/函数 */

    return HAL_SD_ERROR_RX_OVERRUN;                     /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* No error flag set */
    /* Clear all the static flags */
    __HAL_SD_CLEAR_FLAG(hsd, SDIO_STATIC_DATA_FLAGS);   /* HAL辅助宏/函数 */

    *scr = (((tempscr[1] & SDMMC_0TO7BITS) << 24)  | ((tempscr[1] & SDMMC_8TO15BITS) << 8) |\
            ((tempscr[1] & SDMMC_16TO23BITS) >> 8) | ((tempscr[1] & SDMMC_24TO31BITS) >> 24));  /* 执行语句 */
    scr++;                                              /* 执行语句 */
    *scr = (((tempscr[0] & SDMMC_0TO7BITS) << 24)  | ((tempscr[0] & SDMMC_8TO15BITS) << 8) |\
            ((tempscr[0] & SDMMC_16TO23BITS) >> 8) | ((tempscr[0] & SDMMC_24TO31BITS) >> 24));  /* 执行语句 */

  }                                                     /* 代码块结束 */

  return HAL_SD_ERROR_NONE;                             /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Wrap up reading in non-blocking mode.
  * @param  hsd: pointer to a SD_HandleTypeDef structure that contains
  *              the configuration information.
  * @retval None
  */
static void SD_Read_IT(SD_HandleTypeDef *hsd)           /* 静态函数 */
{                                                       /* 代码块开始 */
  uint32_t count, data, dataremaining;                  /* 32位无符号变量/函数 */
  uint8_t* tmp;                                         /* 执行语句 */

  tmp = hsd->pRxBuffPtr;                                /* 赋值或初始化 */
  dataremaining = hsd->RxXferSize;                      /* 赋值或初始化 */

  if (dataremaining > 0U)                               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Read data from SDIO Rx FIFO */
    for(count = 0U; count < 8U; count++)                /* for循环 */
    {                                                   /* 代码块开始 */
      data = SDIO_ReadFIFO(hsd->Instance);              /* 赋值或初始化 */
      *tmp = (uint8_t)(data & 0xFFU);                   /* 赋值或初始化 */
      tmp++;                                            /* 执行语句 */
      dataremaining--;                                  /* 执行语句 */
      *tmp = (uint8_t)((data >> 8U) & 0xFFU);           /* 赋值或初始化 */
      tmp++;                                            /* 执行语句 */
      dataremaining--;                                  /* 执行语句 */
      *tmp = (uint8_t)((data >> 16U) & 0xFFU);          /* 赋值或初始化 */
      tmp++;                                            /* 执行语句 */
      dataremaining--;                                  /* 执行语句 */
      *tmp = (uint8_t)((data >> 24U) & 0xFFU);          /* 赋值或初始化 */
      tmp++;                                            /* 执行语句 */
      dataremaining--;                                  /* 执行语句 */
    }                                                   /* 代码块结束 */

    hsd->pRxBuffPtr = tmp;                              /* 赋值或初始化 */
    hsd->RxXferSize = dataremaining;                    /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Wrap up writing in non-blocking mode.
  * @param  hsd: pointer to a SD_HandleTypeDef structure that contains
  *              the configuration information.
  * @retval None
  */
static void SD_Write_IT(SD_HandleTypeDef *hsd)          /* 静态函数 */
{                                                       /* 代码块开始 */
  uint32_t count, data, dataremaining;                  /* 32位无符号变量/函数 */
  uint8_t* tmp;                                         /* 执行语句 */

  tmp = hsd->pTxBuffPtr;                                /* 赋值或初始化 */
  dataremaining = hsd->TxXferSize;                      /* 赋值或初始化 */

  if (dataremaining > 0U)                               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Write data to SDIO Tx FIFO */
    for(count = 0U; count < 8U; count++)                /* for循环 */
    {                                                   /* 代码块开始 */
      data = (uint32_t)(*tmp);                          /* 赋值或初始化 */
      tmp++;                                            /* 执行语句 */
      dataremaining--;                                  /* 执行语句 */
      data |= ((uint32_t)(*tmp) << 8U);                 /* 赋值或初始化 */
      tmp++;                                            /* 执行语句 */
      dataremaining--;                                  /* 执行语句 */
      data |= ((uint32_t)(*tmp) << 16U);                /* 赋值或初始化 */
      tmp++;                                            /* 执行语句 */
      dataremaining--;                                  /* 执行语句 */
      data |= ((uint32_t)(*tmp) << 24U);                /* 赋值或初始化 */
      tmp++;                                            /* 执行语句 */
      dataremaining--;                                  /* 执行语句 */
      (void)SDIO_WriteFIFO(hsd->Instance, &data);       /* 执行语句 */
    }                                                   /* 代码块结束 */

    hsd->pTxBuffPtr = tmp;                              /* 赋值或初始化 */
    hsd->TxXferSize = dataremaining;                    /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

#endif /* HAL_SD_MODULE_ENABLED */

/**
  * @}
  */

/**
  * @}
  */

#endif /* SDIO */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
