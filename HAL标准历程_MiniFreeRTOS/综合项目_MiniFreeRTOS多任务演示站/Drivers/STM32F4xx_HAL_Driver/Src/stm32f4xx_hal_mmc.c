/* 文件: stm32f4xx_hal_mmc.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_mmc.c
  * @author  MCD Application Team
  * @brief   MMC card HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Secure Digital (MMC) peripheral:
  *           + Initialization and de-initialization functions
  *           + IO operation functions
  *           + Peripheral Control functions
  *           + MMC card Control functions
  *
  @verbatim
  ==============================================================================
                        ##### How to use this driver #####
  ==============================================================================
  [..]
    This driver implements a high level communication layer for read and write from/to
    this memory. The needed STM32 hardware resources (SDMMC and GPIO) are performed by
    the user in HAL_MMC_MspInit() function (MSP layer).
    Basically, the MSP layer configuration should be the same as we provide in the
    examples.
    You can easily tailor this configuration according to hardware resources.

  [..]
    This driver is a generic layered driver for SDMMC memories which uses the HAL
    SDMMC driver functions to interface with MMC and eMMC cards devices.
    It is used as follows:

    (#)Initialize the SDMMC low level resources by implement the HAL_MMC_MspInit() API:
        (##) Enable the SDMMC interface clock using __HAL_RCC_SDMMC_CLK_ENABLE();
        (##) SDMMC pins configuration for MMC card
            (+++) Enable the clock for the SDMMC GPIOs using the functions __HAL_RCC_GPIOx_CLK_ENABLE();
            (+++) Configure these SDMMC pins as alternate function pull-up using HAL_GPIO_Init()
                  and according to your pin assignment;
        (##) DMA Configuration if you need to use DMA process (HAL_MMC_ReadBlocks_DMA()
             and HAL_MMC_WriteBlocks_DMA() APIs).
            (+++) Enable the DMAx interface clock using __HAL_RCC_DMAx_CLK_ENABLE(); 
            (+++) Configure the DMA using the function HAL_DMA_Init() with predeclared and filled. 
        (##) NVIC configuration if you need to use interrupt process when using DMA transfer.
            (+++) Configure the SDMMC and DMA interrupt priorities using function HAL_NVIC_SetPriority();
                  DMA priority is superior to SDMMC's priority
            (+++) Enable the NVIC DMA and SDMMC IRQs using function HAL_NVIC_EnableIRQ()
            (+++) SDMMC interrupts are managed using the macros __HAL_MMC_ENABLE_IT() 
                  and __HAL_MMC_DISABLE_IT() inside the communication process.
            (+++) SDMMC interrupts pending bits are managed using the macros __HAL_MMC_GET_IT()
                  and __HAL_MMC_CLEAR_IT()
        (##) NVIC configuration if you need to use interrupt process (HAL_MMC_ReadBlocks_IT()
             and HAL_MMC_WriteBlocks_IT() APIs).
            (+++) Configure the SDMMC interrupt priorities using function HAL_NVIC_SetPriority();
            (+++) Enable the NVIC SDMMC IRQs using function HAL_NVIC_EnableIRQ()
            (+++) SDMMC interrupts are managed using the macros __HAL_MMC_ENABLE_IT()
                  and __HAL_MMC_DISABLE_IT() inside the communication process.
            (+++) SDMMC interrupts pending bits are managed using the macros __HAL_MMC_GET_IT()
                  and __HAL_MMC_CLEAR_IT()
    (#) At this stage, you can perform MMC read/write/erase operations after MMC card initialization


  *** MMC Card Initialization and configuration ***
  ================================================
  [..]
    To initialize the MMC Card, use the HAL_MMC_Init() function. It Initializes
    SDMMC Peripheral (STM32 side) and the MMC Card, and put it into StandBy State (Ready for data transfer).
    This function provide the following operations:

    (#) Initialize the SDMMC peripheral interface with defaullt configuration.
        The initialization process is done at 400KHz. You can change or adapt
        this frequency by adjusting the "ClockDiv" field.
        The MMC Card frequency (SDMMC_CK) is computed as follows:

           SDMMC_CK = SDMMCCLK / (ClockDiv + 2)

        In initialization mode and according to the MMC Card standard,
        make sure that the SDMMC_CK frequency doesn't exceed 400KHz.

        This phase of initialization is done through SDMMC_Init() and
        SDMMC_PowerState_ON() SDMMC low level APIs.

    (#) Initialize the MMC card. The API used is HAL_MMC_InitCard().
        This phase allows the card initialization and identification
        and check the MMC Card type (Standard Capacity or High Capacity)
        The initialization flow is compatible with MMC standard.

        This API (HAL_MMC_InitCard()) could be used also to reinitialize the card in case
        of plug-off plug-in.

    (#) Configure the MMC Card Data transfer frequency. By Default, the card transfer
        frequency is set to 24MHz. You can change or adapt this frequency by adjusting 
        the "ClockDiv" field.
        In transfer mode and according to the MMC Card standard, make sure that the
        SDMMC_CK frequency doesn't exceed 25MHz and 50MHz in High-speed mode switch.
        To be able to use a frequency higher than 24MHz, you should use the SDMMC
        peripheral in bypass mode. Refer to the corresponding reference manual
        for more details.

    (#) Select the corresponding MMC Card according to the address read with the step 2.

    (#) Configure the MMC Card in wide bus mode: 4-bits data.

  *** MMC Card Read operation ***
  ==============================
  [..]
    (+) You can read from MMC card in polling mode by using function HAL_MMC_ReadBlocks().
        This function support only 512-bytes block length (the block size should be
        chosen as 512 bytes).
        You can choose either one block read operation or multiple block read operation
        by adjusting the "NumberOfBlocks" parameter.
        After this, you have to ensure that the transfer is done correctly. The check is done
        through HAL_MMC_GetCardState() function for MMC card state.

    (+) You can read from MMC card in DMA mode by using function HAL_MMC_ReadBlocks_DMA().
        This function support only 512-bytes block length (the block size should be
        chosen as 512 bytes).
        You can choose either one block read operation or multiple block read operation
        by adjusting the "NumberOfBlocks" parameter.
        After this, you have to ensure that the transfer is done correctly. The check is done
        through HAL_MMC_GetCardState() function for MMC card state.
        You could also check the DMA transfer process through the MMC Rx interrupt event.

    (+) You can read from MMC card in Interrupt mode by using function HAL_MMC_ReadBlocks_IT().
        This function allows the read of 512 bytes blocks.
        You can choose either one block read operation or multiple block read operation 
        by adjusting the "NumberOfBlocks" parameter.
        After this, you have to ensure that the transfer is done correctly. The check is done
        through HAL_MMC_GetCardState() function for MMC card state.
        You could also check the IT transfer process through the MMC Rx interrupt event.

  *** MMC Card Write operation ***
  ===============================
  [..]
    (+) You can write to MMC card in polling mode by using function HAL_MMC_WriteBlocks().
        This function support only 512-bytes block length (the block size should be
        chosen as 512 bytes).
        You can choose either one block read operation or multiple block read operation
        by adjusting the "NumberOfBlocks" parameter.
        After this, you have to ensure that the transfer is done correctly. The check is done
        through HAL_MMC_GetCardState() function for MMC card state.

    (+) You can write to MMC card in DMA mode by using function HAL_MMC_WriteBlocks_DMA().
        This function support only 512-bytes block length (the block size should be
        chosen as 512 byte).
        You can choose either one block read operation or multiple block read operation
        by adjusting the "NumberOfBlocks" parameter.
        After this, you have to ensure that the transfer is done correctly. The check is done
        through HAL_MMC_GetCardState() function for MMC card state.
        You could also check the DMA transfer process through the MMC Tx interrupt event.  

    (+) You can write to MMC card in Interrupt mode by using function HAL_MMC_WriteBlocks_IT().
        This function allows the read of 512 bytes blocks.
        You can choose either one block read operation or multiple block read operation 
        by adjusting the "NumberOfBlocks" parameter.
        After this, you have to ensure that the transfer is done correctly. The check is done
        through HAL_MMC_GetCardState() function for MMC card state.
        You could also check the IT transfer process through the MMC Tx interrupt event.

  *** MMC card information ***
  =========================== 
  [..]
    (+) To get MMC card information, you can use the function HAL_MMC_GetCardInfo().
        It returns useful information about the MMC card such as block size, card type,
        block number ...

  *** MMC card CSD register ***
  ============================
  [..]
    (+) The HAL_MMC_GetCardCSD() API allows to get the parameters of the CSD register.
        Some of the CSD parameters are useful for card initialization and identification.

  *** MMC card CID register ***
  ============================
  [..]
    (+) The HAL_MMC_GetCardCID() API allows to get the parameters of the CID register.
        Some of the CID parameters are useful for card initialization and identification.

  *** MMC HAL driver macros list ***
  ==================================
  [..]
    Below the list of most used macros in MMC HAL driver.

    (+) __HAL_MMC_ENABLE : Enable the MMC device
    (+) __HAL_MMC_DISABLE : Disable the MMC device
    (+) __HAL_MMC_DMA_ENABLE: Enable the SDMMC DMA transfer
    (+) __HAL_MMC_DMA_DISABLE: Disable the SDMMC DMA transfer
    (+) __HAL_MMC_ENABLE_IT: Enable the MMC device interrupt
    (+) __HAL_MMC_DISABLE_IT: Disable the MMC device interrupt
    (+) __HAL_MMC_GET_FLAG:Check whether the specified MMC flag is set or not
    (+) __HAL_MMC_CLEAR_FLAG: Clear the MMC's pending flags

  [..]
    (@) You can refer to the MMC HAL driver header file for more useful macros

  *** Callback registration ***
  =============================================
  [..]
    The compilation define USE_HAL_MMC_REGISTER_CALLBACKS when set to 1
    allows the user to configure dynamically the driver callbacks.

    Use Functions @ref HAL_MMC_RegisterCallback() to register a user callback,
    it allows to register following callbacks:
      (+) TxCpltCallback : callback when a transmission transfer is completed.
      (+) RxCpltCallback : callback when a reception transfer is completed.
      (+) ErrorCallback : callback when error occurs.
      (+) AbortCpltCallback : callback when abort is completed.
      (+) MspInitCallback    : MMC MspInit.
      (+) MspDeInitCallback  : MMC MspDeInit.
    This function takes as parameters the HAL peripheral handle, the Callback ID
    and a pointer to the user callback function.

    Use function @ref HAL_MMC_UnRegisterCallback() to reset a callback to the default
    weak (surcharged) function. It allows to reset following callbacks:
      (+) TxCpltCallback : callback when a transmission transfer is completed.
      (+) RxCpltCallback : callback when a reception transfer is completed.
      (+) ErrorCallback : callback when error occurs.
      (+) AbortCpltCallback : callback when abort is completed.
      (+) MspInitCallback    : MMC MspInit.
      (+) MspDeInitCallback  : MMC MspDeInit.
    This function) takes as parameters the HAL peripheral handle and the Callback ID.

    By default, after the @ref HAL_MMC_Init and if the state is HAL_MMC_STATE_RESET
    all callbacks are reset to the corresponding legacy weak (surcharged) functions.
    Exception done for MspInit and MspDeInit callbacks that are respectively
    reset to the legacy weak (surcharged) functions in the @ref HAL_MMC_Init
    and @ref  HAL_MMC_DeInit only when these callbacks are null (not registered beforehand).
    If not, MspInit or MspDeInit are not null, the @ref HAL_MMC_Init and @ref HAL_MMC_DeInit
    keep and use the user MspInit/MspDeInit callbacks (registered beforehand)

    Callbacks can be registered/unregistered in READY state only.
    Exception done for MspInit/MspDeInit callbacks that can be registered/unregistered
    in READY or RESET state, thus registered (user) MspInit/DeInit callbacks can be used
    during the Init/DeInit.
    In that case first register the MspInit/MspDeInit user callbacks
    using @ref HAL_MMC_RegisterCallback before calling @ref HAL_MMC_DeInit
    or @ref HAL_MMC_Init function.

    When The compilation define USE_HAL_MMC_REGISTER_CALLBACKS is set to 0 or
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

/** @addtogroup STM32F4xx_HAL_Driver
  * @{
  */

/** @defgroup MMC MMC
  * @brief MMC HAL module driver
  * @{
  */

#ifdef HAL_MMC_MODULE_ENABLED                           /* 条件编译 */

#if defined(SDIO)                                       /* 条件编译 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @addtogroup MMC_Private_Defines
  * @{
  */

/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/** @defgroup MMC_Private_Functions MMC Private Functions
  * @{
  */
static uint32_t MMC_InitCard(MMC_HandleTypeDef *hmmc);  /* 静态变量/函数 */
static uint32_t MMC_PowerON(MMC_HandleTypeDef *hmmc);   /* 静态变量/函数 */
static uint32_t MMC_SendStatus(MMC_HandleTypeDef *hmmc, uint32_t *pCardStatus);  /* 静态变量/函数 */
static uint32_t MMC_ReadExtCSD(MMC_HandleTypeDef *hmmc, uint32_t *pFieldData, uint16_t FieldIndex, uint32_t Timeout);  /* 静态变量/函数 */
static void     MMC_PowerOFF(MMC_HandleTypeDef *hmmc);  /* 静态函数 */
static void     MMC_Write_IT(MMC_HandleTypeDef *hmmc);  /* 静态函数 */
static void     MMC_Read_IT(MMC_HandleTypeDef *hmmc);   /* 静态函数 */
static void     MMC_DMATransmitCplt(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void     MMC_DMAReceiveCplt(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void     MMC_DMAError(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void     MMC_DMATxAbort(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void     MMC_DMARxAbort(DMA_HandleTypeDef *hdma);  /* 静态函数 */
/**
  * @}
  */
/* Exported functions --------------------------------------------------------*/
/** @addtogroup MMC_Exported_Functions
  * @{
  */

/** @addtogroup MMC_Exported_Functions_Group1
 *  @brief   Initialization and de-initialization functions
 *
@verbatim
  ==============================================================================
          ##### Initialization and de-initialization functions #####
  ==============================================================================
  [..]
    This section provides functions allowing to initialize/de-initialize the MMC
    card device to be ready for use.

@endverbatim
  * @{
  */

/**
  * @brief  Initializes the MMC according to the specified parameters in the
            MMC_HandleTypeDef and create the associated handle.
  * @param  hmmc: Pointer to the MMC handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_Init(MMC_HandleTypeDef *hmmc)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the MMC handle allocation */
  if(hmmc == NULL)                                      /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the parameters */
  assert_param(IS_SDIO_ALL_INSTANCE(hmmc->Instance));   /* 执行语句 */
  assert_param(IS_SDIO_CLOCK_EDGE(hmmc->Init.ClockEdge));  /* 执行语句 */
  assert_param(IS_SDIO_CLOCK_BYPASS(hmmc->Init.ClockBypass));  /* 执行语句 */
  assert_param(IS_SDIO_CLOCK_POWER_SAVE(hmmc->Init.ClockPowerSave));  /* 执行语句 */
  assert_param(IS_SDIO_BUS_WIDE(hmmc->Init.BusWide));   /* 执行语句 */
  assert_param(IS_SDIO_HARDWARE_FLOW_CONTROL(hmmc->Init.HardwareFlowControl));  /* 执行语句 */
  assert_param(IS_SDIO_CLKDIV(hmmc->Init.ClockDiv));    /* 执行语句 */

  if(hmmc->State == HAL_MMC_STATE_RESET)                /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Allocate lock resource and initialize it */
    hmmc->Lock = HAL_UNLOCKED;                          /* 赋值或初始化 */
#if defined (USE_HAL_MMC_REGISTER_CALLBACKS) && (USE_HAL_MMC_REGISTER_CALLBACKS == 1U)  /* 条件编译 */
    /* Reset Callback pointers in HAL_MMC_STATE_RESET only */
    hmmc->TxCpltCallback    = HAL_MMC_TxCpltCallback;   /* 赋值或初始化 */
    hmmc->RxCpltCallback    = HAL_MMC_RxCpltCallback;   /* 赋值或初始化 */
    hmmc->ErrorCallback     = HAL_MMC_ErrorCallback;    /* 赋值或初始化 */
    hmmc->AbortCpltCallback = HAL_MMC_AbortCallback;    /* 赋值或初始化 */

    if(hmmc->MspInitCallback == NULL)                   /* 条件判断 */
    {                                                   /* 代码块开始 */
      hmmc->MspInitCallback = HAL_MMC_MspInit;          /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Init the low level hardware */
    hmmc->MspInitCallback(hmmc);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /* Init the low level hardware : GPIO, CLOCK, CORTEX...etc */
    HAL_MMC_MspInit(hmmc);                              /* HAL库函数调用 */
#endif                                                  /* 条件编译结束 */
  }                                                     /* 代码块结束 */

  hmmc->State = HAL_MMC_STATE_BUSY;                     /* 赋值或初始化 */

  /* Initialize the Card parameters */
  if(HAL_MMC_InitCard(hmmc) == HAL_ERROR)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Initialize the error code */
  hmmc->ErrorCode = HAL_DMA_ERROR_NONE;                 /* 赋值或初始化 */

  /* Initialize the MMC operation */
  hmmc->Context = MMC_CONTEXT_NONE;                     /* 赋值或初始化 */

  /* Initialize the MMC state */
  hmmc->State = HAL_MMC_STATE_READY;                    /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initializes the MMC Card.
  * @param  hmmc: Pointer to MMC handle
  * @note   This function initializes the MMC card. It could be used when a card
            re-initialization is needed.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_InitCard(MMC_HandleTypeDef *hmmc)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t errorstate;                                  /* 32位无符号变量/函数 */
  MMC_InitTypeDef Init;                                 /* 执行语句 */
  HAL_StatusTypeDef status;                             /* HAL状态类型函数 */

  /* Default SDIO peripheral configuration for MMC card initialization */
  Init.ClockEdge           = SDIO_CLOCK_EDGE_RISING;    /* 赋值或初始化 */
  Init.ClockBypass         = SDIO_CLOCK_BYPASS_DISABLE;  /* 赋值或初始化 */
  Init.ClockPowerSave      = SDIO_CLOCK_POWER_SAVE_DISABLE;  /* 赋值或初始化 */
  Init.BusWide             = SDIO_BUS_WIDE_1B;          /* 赋值或初始化 */
  Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;  /* 赋值或初始化 */
  Init.ClockDiv            = SDIO_INIT_CLK_DIV;         /* 赋值或初始化 */

  /* Initialize SDIO peripheral interface with default configuration */
  status = SDIO_Init(hmmc->Instance, Init);             /* 赋值或初始化 */
  if(status == HAL_ERROR)                               /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Disable SDIO Clock */
  __HAL_MMC_DISABLE(hmmc);                              /* HAL辅助宏/函数 */

  /* Set Power State to ON */
  status = SDIO_PowerState_ON(hmmc->Instance);          /* 赋值或初始化 */
  if(status == HAL_ERROR)                               /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Enable MMC Clock */
  __HAL_MMC_ENABLE(hmmc);                               /* HAL辅助宏/函数 */

  /* Identify card operating voltage */
  errorstate = MMC_PowerON(hmmc);                       /* 赋值或初始化 */
  if(errorstate != HAL_MMC_ERROR_NONE)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hmmc->State = HAL_MMC_STATE_READY;                  /* 赋值或初始化 */
    hmmc->ErrorCode |= errorstate;                      /* 赋值或初始化 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Card initialization */
  errorstate = MMC_InitCard(hmmc);                      /* 赋值或初始化 */
  if(errorstate != HAL_MMC_ERROR_NONE)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hmmc->State = HAL_MMC_STATE_READY;                  /* 赋值或初始化 */
    hmmc->ErrorCode |= errorstate;                      /* 赋值或初始化 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Set Block Size for Card */
  errorstate = SDMMC_CmdBlockLength(hmmc->Instance, MMC_BLOCKSIZE);  /* 赋值或初始化 */
  if(errorstate != HAL_MMC_ERROR_NONE)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Clear all the static flags */
    __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);      /* HAL辅助宏/函数 */
    hmmc->ErrorCode |= errorstate;                      /* 赋值或初始化 */
    hmmc->State = HAL_MMC_STATE_READY;                  /* 赋值或初始化 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  De-Initializes the MMC card.
  * @param  hmmc: Pointer to MMC handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_DeInit(MMC_HandleTypeDef *hmmc)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the MMC handle allocation */
  if(hmmc == NULL)                                      /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the parameters */
  assert_param(IS_SDIO_ALL_INSTANCE(hmmc->Instance));   /* 执行语句 */

  hmmc->State = HAL_MMC_STATE_BUSY;                     /* 赋值或初始化 */

  /* Set MMC power state to off */
  MMC_PowerOFF(hmmc);                                   /* 执行语句 */

#if defined (USE_HAL_MMC_REGISTER_CALLBACKS) && (USE_HAL_MMC_REGISTER_CALLBACKS == 1U)  /* 条件编译 */
  if(hmmc->MspDeInitCallback == NULL)                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    hmmc->MspDeInitCallback = HAL_MMC_MspDeInit;        /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* DeInit the low level hardware */
  hmmc->MspDeInitCallback(hmmc);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* De-Initialize the MSP layer */
  HAL_MMC_MspDeInit(hmmc);                              /* HAL库函数调用 */
#endif                                                  /* 条件编译结束 */

  hmmc->ErrorCode = HAL_MMC_ERROR_NONE;                 /* 赋值或初始化 */
  hmmc->State = HAL_MMC_STATE_RESET;                    /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */


/**
  * @brief  Initializes the MMC MSP.
  * @param  hmmc: Pointer to MMC handle
  * @retval None
  */
__weak void HAL_MMC_MspInit(MMC_HandleTypeDef *hmmc)    /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hmmc);                                         /* 执行语句 */

  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_MMC_MspInit could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  De-Initialize MMC MSP.
  * @param  hmmc: Pointer to MMC handle
  * @retval None
  */
__weak void HAL_MMC_MspDeInit(MMC_HandleTypeDef *hmmc)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hmmc);                                         /* 执行语句 */

  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_MMC_MspDeInit could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @addtogroup MMC_Exported_Functions_Group2
 *  @brief   Data transfer functions
 *
@verbatim
  ==============================================================================
                        ##### IO operation functions #####
  ==============================================================================
  [..]
    This subsection provides a set of functions allowing to manage the data
    transfer from/to MMC card.

@endverbatim
  * @{
  */

/**
  * @brief  Reads block(s) from a specified address in a card. The Data transfer
  *         is managed by polling mode.
  * @note   This API should be followed by a check on the card state through
  *         HAL_MMC_GetCardState().
  * @param  hmmc: Pointer to MMC handle
  * @param  pData: pointer to the buffer that will contain the received data
  * @param  BlockAdd: Block Address from where data is to be read
  * @param  NumberOfBlocks: Number of MMC blocks to read
  * @param  Timeout: Specify timeout value
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_ReadBlocks(MMC_HandleTypeDef *hmmc, uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  SDIO_DataInitTypeDef config;                          /* 执行语句 */
  uint32_t errorstate;                                  /* 32位无符号变量/函数 */
  uint32_t tickstart = HAL_GetTick();                   /* 32位无符号变量/函数 */
  uint32_t count, data, dataremaining;                  /* 32位无符号变量/函数 */
  uint32_t add = BlockAdd;                              /* 32位无符号变量/函数 */
  uint8_t *tempbuff = pData;                            /* 8位无符号变量/函数 */

  if(NULL == pData)                                     /* 条件判断 */
  {                                                     /* 代码块开始 */
    hmmc->ErrorCode |= HAL_MMC_ERROR_PARAM;             /* 赋值或初始化 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  if(hmmc->State == HAL_MMC_STATE_READY)                /* 条件判断 */
  {                                                     /* 代码块开始 */
    hmmc->ErrorCode = HAL_MMC_ERROR_NONE;               /* 赋值或初始化 */

    if((BlockAdd + NumberOfBlocks) > (hmmc->MmcCard.LogBlockNbr))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hmmc->ErrorCode |= HAL_MMC_ERROR_ADDR_OUT_OF_RANGE;  /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    hmmc->State = HAL_MMC_STATE_BUSY;                   /* 赋值或初始化 */

    /* Initialize data control register */
    hmmc->Instance->DCTRL = 0U;                         /* 赋值或初始化 */

    if ((hmmc->MmcCard.CardType) != MMC_HIGH_CAPACITY_CARD)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      add *= 512U;                                      /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Configure the MMC DPSM (Data Path State Machine) */
    config.DataTimeOut   = SDMMC_DATATIMEOUT;           /* 赋值或初始化 */
    config.DataLength    = NumberOfBlocks * MMC_BLOCKSIZE;  /* 赋值或初始化 */
    config.DataBlockSize = SDIO_DATABLOCK_SIZE_512B;    /* 赋值或初始化 */
    config.TransferDir   = SDIO_TRANSFER_DIR_TO_SDIO;   /* 赋值或初始化 */
    config.TransferMode  = SDIO_TRANSFER_MODE_BLOCK;    /* 赋值或初始化 */
    config.DPSM          = SDIO_DPSM_ENABLE;            /* 赋值或初始化 */
    (void)SDIO_ConfigData(hmmc->Instance, &config);     /* 执行语句 */

    /* Read block(s) in polling mode */
    if(NumberOfBlocks > 1U)                             /* 条件判断 */
    {                                                   /* 代码块开始 */
      hmmc->Context = MMC_CONTEXT_READ_MULTIPLE_BLOCK;  /* 赋值或初始化 */

      /* Read Multi Block command */
      errorstate = SDMMC_CmdReadMultiBlock(hmmc->Instance, add);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hmmc->Context = MMC_CONTEXT_READ_SINGLE_BLOCK;    /* 赋值或初始化 */

      /* Read Single Block command */
      errorstate = SDMMC_CmdReadSingleBlock(hmmc->Instance, add);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    if(errorstate != HAL_MMC_ERROR_NONE)                /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear all the static flags */
      __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);    /* HAL辅助宏/函数 */
      hmmc->ErrorCode |= errorstate;                    /* 赋值或初始化 */
      hmmc->State = HAL_MMC_STATE_READY;                /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Poll on SDIO flags */
    dataremaining = config.DataLength;                  /* 赋值或初始化 */
#if defined(SDIO_STA_STBITERR)                          /* 条件编译 */
    while(!__HAL_MMC_GET_FLAG(hmmc, SDIO_FLAG_RXOVERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DATAEND | SDIO_FLAG_STBITERR))  /* while循环 */
#else /* SDIO_STA_STBITERR not defined */
    while(!__HAL_MMC_GET_FLAG(hmmc, SDIO_FLAG_RXOVERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DATAEND))  /* while循环 */
#endif /* SDIO_STA_STBITERR */
    {                                                   /* 代码块开始 */
      if(__HAL_MMC_GET_FLAG(hmmc, SDIO_FLAG_RXFIFOHF) && (dataremaining > 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Read data from SDIO Rx FIFO */
        for(count = 0U; count < 8U; count++)            /* for循环 */
        {                                               /* 代码块开始 */
          data = SDIO_ReadFIFO(hmmc->Instance);         /* 赋值或初始化 */
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
        __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);  /* HAL辅助宏/函数 */
        hmmc->ErrorCode |= HAL_MMC_ERROR_TIMEOUT;       /* 赋值或初始化 */
        hmmc->State= HAL_MMC_STATE_READY;               /* 赋值或初始化 */
        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Send stop transmission command in case of multiblock read */
    if(__HAL_MMC_GET_FLAG(hmmc, SDIO_FLAG_DATAEND) && (NumberOfBlocks > 1U))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Send stop transmission command */
      errorstate = SDMMC_CmdStopTransfer(hmmc->Instance);  /* 赋值或初始化 */
      if(errorstate != HAL_MMC_ERROR_NONE)              /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Clear all the static flags */
        __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);  /* HAL辅助宏/函数 */
        hmmc->ErrorCode |= errorstate;                  /* 赋值或初始化 */
        hmmc->State = HAL_MMC_STATE_READY;              /* 赋值或初始化 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Get error state */
    if(__HAL_MMC_GET_FLAG(hmmc, SDIO_FLAG_DTIMEOUT))    /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear all the static flags */
      __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);    /* HAL辅助宏/函数 */
      hmmc->ErrorCode |= HAL_MMC_ERROR_DATA_TIMEOUT;    /* 赋值或初始化 */
      hmmc->State = HAL_MMC_STATE_READY;                /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
    else if(__HAL_MMC_GET_FLAG(hmmc, SDIO_FLAG_DCRCFAIL))  /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* Clear all the static flags */
      __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);    /* HAL辅助宏/函数 */
      hmmc->ErrorCode |= HAL_MMC_ERROR_DATA_CRC_FAIL;   /* 赋值或初始化 */
      hmmc->State = HAL_MMC_STATE_READY;                /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
    else if(__HAL_MMC_GET_FLAG(hmmc, SDIO_FLAG_RXOVERR))  /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* Clear all the static flags */
      __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);    /* HAL辅助宏/函数 */
      hmmc->ErrorCode |= HAL_MMC_ERROR_RX_OVERRUN;      /* 赋值或初始化 */
      hmmc->State = HAL_MMC_STATE_READY;                /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Nothing to do */
    }                                                   /* 代码块结束 */

    /* Empty FIFO if there is still any data */
    while ((__HAL_MMC_GET_FLAG(hmmc, SDIO_FLAG_RXDAVL)) && (dataremaining > 0U))  /* while循环 */
    {                                                   /* 代码块开始 */
      data = SDIO_ReadFIFO(hmmc->Instance);             /* 赋值或初始化 */
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
        __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);          /* HAL辅助宏/函数 */
        hmmc->ErrorCode |= HAL_MMC_ERROR_TIMEOUT;       /* 赋值或初始化 */
        hmmc->State= HAL_MMC_STATE_READY;               /* 赋值或初始化 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Clear all the static flags */
    __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_DATA_FLAGS);  /* HAL辅助宏/函数 */

    hmmc->State = HAL_MMC_STATE_READY;                  /* 赋值或初始化 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    hmmc->ErrorCode |= HAL_MMC_ERROR_BUSY;              /* 赋值或初始化 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Allows to write block(s) to a specified address in a card. The Data
  *         transfer is managed by polling mode.
  * @note   This API should be followed by a check on the card state through
  *         HAL_MMC_GetCardState().
  * @param  hmmc: Pointer to MMC handle
  * @param  pData: pointer to the buffer that will contain the data to transmit
  * @param  BlockAdd: Block Address where data will be written
  * @param  NumberOfBlocks: Number of MMC blocks to write
  * @param  Timeout: Specify timeout value
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_WriteBlocks(MMC_HandleTypeDef *hmmc, uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  SDIO_DataInitTypeDef config;                          /* 执行语句 */
  uint32_t errorstate;                                  /* 32位无符号变量/函数 */
  uint32_t tickstart = HAL_GetTick();                   /* 32位无符号变量/函数 */
  uint32_t count, data, dataremaining;                  /* 32位无符号变量/函数 */
  uint32_t add = BlockAdd;                              /* 32位无符号变量/函数 */
  uint8_t *tempbuff = pData;                            /* 8位无符号变量/函数 */

  if(NULL == pData)                                     /* 条件判断 */
  {                                                     /* 代码块开始 */
    hmmc->ErrorCode |= HAL_MMC_ERROR_PARAM;             /* 赋值或初始化 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  if(hmmc->State == HAL_MMC_STATE_READY)                /* 条件判断 */
  {                                                     /* 代码块开始 */
    hmmc->ErrorCode = HAL_MMC_ERROR_NONE;               /* 赋值或初始化 */

    if((BlockAdd + NumberOfBlocks) > (hmmc->MmcCard.LogBlockNbr))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hmmc->ErrorCode |= HAL_MMC_ERROR_ADDR_OUT_OF_RANGE;  /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    hmmc->State = HAL_MMC_STATE_BUSY;                   /* 赋值或初始化 */

    /* Initialize data control register */
    hmmc->Instance->DCTRL = 0U;                         /* 赋值或初始化 */

    if ((hmmc->MmcCard.CardType) != MMC_HIGH_CAPACITY_CARD)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      add *= 512U;                                      /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Write Blocks in Polling mode */
    if(NumberOfBlocks > 1U)                             /* 条件判断 */
    {                                                   /* 代码块开始 */
      hmmc->Context = MMC_CONTEXT_WRITE_MULTIPLE_BLOCK;  /* 赋值或初始化 */

      /* Write Multi Block command */
      errorstate = SDMMC_CmdWriteMultiBlock(hmmc->Instance, add);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hmmc->Context = MMC_CONTEXT_WRITE_SINGLE_BLOCK;   /* 赋值或初始化 */

      /* Write Single Block command */
      errorstate = SDMMC_CmdWriteSingleBlock(hmmc->Instance, add);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    if(errorstate != HAL_MMC_ERROR_NONE)                /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear all the static flags */
      __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);    /* HAL辅助宏/函数 */
      hmmc->ErrorCode |= errorstate;                    /* 赋值或初始化 */
      hmmc->State = HAL_MMC_STATE_READY;                /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Configure the MMC DPSM (Data Path State Machine) */
    config.DataTimeOut   = SDMMC_DATATIMEOUT;           /* 赋值或初始化 */
    config.DataLength    = NumberOfBlocks * MMC_BLOCKSIZE;  /* 赋值或初始化 */
    config.DataBlockSize = SDIO_DATABLOCK_SIZE_512B;    /* 赋值或初始化 */
    config.TransferDir   = SDIO_TRANSFER_DIR_TO_CARD;   /* 赋值或初始化 */
    config.TransferMode  = SDIO_TRANSFER_MODE_BLOCK;    /* 赋值或初始化 */
    config.DPSM          = SDIO_DPSM_ENABLE;            /* 赋值或初始化 */
    (void)SDIO_ConfigData(hmmc->Instance, &config);     /* 执行语句 */

    /* Write block(s) in polling mode */
    dataremaining = config.DataLength;                  /* 赋值或初始化 */
#if defined(SDIO_STA_STBITERR)                          /* 条件编译 */
    while(!__HAL_MMC_GET_FLAG(hmmc, SDIO_FLAG_TXUNDERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DATAEND | SDIO_FLAG_STBITERR))  /* while循环 */
#else /* SDIO_STA_STBITERR not defined */
    while(!__HAL_MMC_GET_FLAG(hmmc, SDIO_FLAG_TXUNDERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DATAEND))  /* while循环 */
#endif /* SDIO_STA_STBITERR */
    {                                                   /* 代码块开始 */
      if(__HAL_MMC_GET_FLAG(hmmc, SDIO_FLAG_TXFIFOHE) && (dataremaining > 0U))  /* 条件判断 */
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
          (void)SDIO_WriteFIFO(hmmc->Instance, &data);  /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */

      if(((HAL_GetTick()-tickstart) >=  Timeout) || (Timeout == 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Clear all the static flags */
        __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);  /* HAL辅助宏/函数 */
        hmmc->ErrorCode |= errorstate;                  /* 赋值或初始化 */
        hmmc->State = HAL_MMC_STATE_READY;              /* 赋值或初始化 */
        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Send stop transmission command in case of multiblock write */
    if(__HAL_MMC_GET_FLAG(hmmc, SDIO_FLAG_DATAEND) && (NumberOfBlocks > 1U))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Send stop transmission command */
      errorstate = SDMMC_CmdStopTransfer(hmmc->Instance);  /* 赋值或初始化 */
      if(errorstate != HAL_MMC_ERROR_NONE)              /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Clear all the static flags */
        __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);  /* HAL辅助宏/函数 */
        hmmc->ErrorCode |= errorstate;                  /* 赋值或初始化 */
        hmmc->State = HAL_MMC_STATE_READY;              /* 赋值或初始化 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Get error state */
    if(__HAL_MMC_GET_FLAG(hmmc, SDIO_FLAG_DTIMEOUT))    /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear all the static flags */
      __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);    /* HAL辅助宏/函数 */
      hmmc->ErrorCode |= HAL_MMC_ERROR_DATA_TIMEOUT;    /* 赋值或初始化 */
      hmmc->State = HAL_MMC_STATE_READY;                /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
    else if(__HAL_MMC_GET_FLAG(hmmc, SDIO_FLAG_DCRCFAIL))  /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* Clear all the static flags */
      __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);    /* HAL辅助宏/函数 */
      hmmc->ErrorCode |= HAL_MMC_ERROR_DATA_CRC_FAIL;   /* 赋值或初始化 */
      hmmc->State = HAL_MMC_STATE_READY;                /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
    else if(__HAL_MMC_GET_FLAG(hmmc, SDIO_FLAG_TXUNDERR))  /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* Clear all the static flags */
      __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);    /* HAL辅助宏/函数 */
      hmmc->ErrorCode |= HAL_MMC_ERROR_TX_UNDERRUN;     /* 赋值或初始化 */
      hmmc->State = HAL_MMC_STATE_READY;                /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Nothing to do */
    }                                                   /* 代码块结束 */

    /* Clear all the static flags */
    __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_DATA_FLAGS);  /* HAL辅助宏/函数 */

    hmmc->State = HAL_MMC_STATE_READY;                  /* 赋值或初始化 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    hmmc->ErrorCode |= HAL_MMC_ERROR_BUSY;              /* 赋值或初始化 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Reads block(s) from a specified address in a card. The Data transfer
  *         is managed in interrupt mode.
  * @note   This API should be followed by a check on the card state through
  *         HAL_MMC_GetCardState().
  * @note   You could also check the IT transfer process through the MMC Rx
  *         interrupt event.
  * @param  hmmc: Pointer to MMC handle
  * @param  pData: Pointer to the buffer that will contain the received data
  * @param  BlockAdd: Block Address from where data is to be read
  * @param  NumberOfBlocks: Number of blocks to read.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_ReadBlocks_IT(MMC_HandleTypeDef *hmmc, uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  SDIO_DataInitTypeDef config;                          /* 执行语句 */
  uint32_t errorstate;                                  /* 32位无符号变量/函数 */
  uint32_t add = BlockAdd;                              /* 32位无符号变量/函数 */

  if(NULL == pData)                                     /* 条件判断 */
  {                                                     /* 代码块开始 */
    hmmc->ErrorCode |= HAL_MMC_ERROR_PARAM;             /* 赋值或初始化 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  if(hmmc->State == HAL_MMC_STATE_READY)                /* 条件判断 */
  {                                                     /* 代码块开始 */
    hmmc->ErrorCode = HAL_MMC_ERROR_NONE;               /* 赋值或初始化 */

    if((BlockAdd + NumberOfBlocks) > (hmmc->MmcCard.LogBlockNbr))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hmmc->ErrorCode |= HAL_MMC_ERROR_ADDR_OUT_OF_RANGE;  /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    hmmc->State = HAL_MMC_STATE_BUSY;                   /* 赋值或初始化 */

    /* Initialize data control register */
    hmmc->Instance->DCTRL = 0U;                         /* 赋值或初始化 */

    hmmc->pRxBuffPtr = pData;                           /* 赋值或初始化 */
    hmmc->RxXferSize = MMC_BLOCKSIZE * NumberOfBlocks;  /* 赋值或初始化 */

#if defined(SDIO_STA_STBITERR)                          /* 条件编译 */
    __HAL_MMC_ENABLE_IT(hmmc, (SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_RXOVERR | SDIO_IT_DATAEND | SDIO_FLAG_RXFIFOHF | SDIO_IT_STBITERR));  /* HAL辅助宏/函数 */
#else /* SDIO_STA_STBITERR not defined */
    __HAL_MMC_ENABLE_IT(hmmc, (SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_RXOVERR | SDIO_IT_DATAEND | SDIO_FLAG_RXFIFOHF));  /* HAL辅助宏/函数 */
#endif /* SDIO_STA_STBITERR */

    if ((hmmc->MmcCard.CardType) != MMC_HIGH_CAPACITY_CARD)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      add *= 512U;                                      /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Configure the MMC DPSM (Data Path State Machine) */
    config.DataTimeOut   = SDMMC_DATATIMEOUT;           /* 赋值或初始化 */
    config.DataLength    = MMC_BLOCKSIZE * NumberOfBlocks;  /* 赋值或初始化 */
    config.DataBlockSize = SDIO_DATABLOCK_SIZE_512B;    /* 赋值或初始化 */
    config.TransferDir   = SDIO_TRANSFER_DIR_TO_SDIO;   /* 赋值或初始化 */
    config.TransferMode  = SDIO_TRANSFER_MODE_BLOCK;    /* 赋值或初始化 */
    config.DPSM          = SDIO_DPSM_ENABLE;            /* 赋值或初始化 */
    (void)SDIO_ConfigData(hmmc->Instance, &config);     /* 执行语句 */

    /* Read Blocks in IT mode */
    if(NumberOfBlocks > 1U)                             /* 条件判断 */
    {                                                   /* 代码块开始 */
      hmmc->Context = (MMC_CONTEXT_READ_MULTIPLE_BLOCK | MMC_CONTEXT_IT);  /* 赋值或初始化 */

      /* Read Multi Block command */
      errorstate = SDMMC_CmdReadMultiBlock(hmmc->Instance, add);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hmmc->Context = (MMC_CONTEXT_READ_SINGLE_BLOCK | MMC_CONTEXT_IT);  /* 赋值或初始化 */

      /* Read Single Block command */
      errorstate = SDMMC_CmdReadSingleBlock(hmmc->Instance, add);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    if(errorstate != HAL_MMC_ERROR_NONE)                /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear all the static flags */
      __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);    /* HAL辅助宏/函数 */
      hmmc->ErrorCode |= errorstate;                    /* 赋值或初始化 */
      hmmc->State = HAL_MMC_STATE_READY;                /* 赋值或初始化 */
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
  *         HAL_MMC_GetCardState().
  * @note   You could also check the IT transfer process through the MMC Tx
  *         interrupt event.
  * @param  hmmc: Pointer to MMC handle
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  BlockAdd: Block Address where data will be written
  * @param  NumberOfBlocks: Number of blocks to write
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_WriteBlocks_IT(MMC_HandleTypeDef *hmmc, uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  SDIO_DataInitTypeDef config;                          /* 执行语句 */
  uint32_t errorstate;                                  /* 32位无符号变量/函数 */
  uint32_t add = BlockAdd;                              /* 32位无符号变量/函数 */

  if(NULL == pData)                                     /* 条件判断 */
  {                                                     /* 代码块开始 */
    hmmc->ErrorCode |= HAL_MMC_ERROR_PARAM;             /* 赋值或初始化 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  if(hmmc->State == HAL_MMC_STATE_READY)                /* 条件判断 */
  {                                                     /* 代码块开始 */
    hmmc->ErrorCode = HAL_MMC_ERROR_NONE;               /* 赋值或初始化 */

    if((BlockAdd + NumberOfBlocks) > (hmmc->MmcCard.LogBlockNbr))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hmmc->ErrorCode |= HAL_MMC_ERROR_ADDR_OUT_OF_RANGE;  /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    hmmc->State = HAL_MMC_STATE_BUSY;                   /* 赋值或初始化 */

    /* Initialize data control register */
    hmmc->Instance->DCTRL = 0U;                         /* 赋值或初始化 */

    hmmc->pTxBuffPtr = pData;                           /* 赋值或初始化 */
    hmmc->TxXferSize = MMC_BLOCKSIZE * NumberOfBlocks;  /* 赋值或初始化 */

    /* Enable transfer interrupts */
#if defined(SDIO_STA_STBITERR)                          /* 条件编译 */
    __HAL_MMC_ENABLE_IT(hmmc, (SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_TXUNDERR | SDIO_IT_DATAEND | SDIO_FLAG_TXFIFOHE | SDIO_IT_STBITERR));  /* HAL辅助宏/函数 */
#else /* SDIO_STA_STBITERR not defined */
    __HAL_MMC_ENABLE_IT(hmmc, (SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_TXUNDERR | SDIO_IT_DATAEND | SDIO_FLAG_TXFIFOHE));  /* HAL辅助宏/函数 */
#endif /* SDIO_STA_STBITERR */

    if ((hmmc->MmcCard.CardType) != MMC_HIGH_CAPACITY_CARD)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      add *= 512U;                                      /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Write Blocks in Polling mode */
    if(NumberOfBlocks > 1U)                             /* 条件判断 */
    {                                                   /* 代码块开始 */
      hmmc->Context = (MMC_CONTEXT_WRITE_MULTIPLE_BLOCK| MMC_CONTEXT_IT);  /* 赋值或初始化 */

      /* Write Multi Block command */
      errorstate = SDMMC_CmdWriteMultiBlock(hmmc->Instance, add);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hmmc->Context = (MMC_CONTEXT_WRITE_SINGLE_BLOCK | MMC_CONTEXT_IT);  /* 赋值或初始化 */

      /* Write Single Block command */
      errorstate = SDMMC_CmdWriteSingleBlock(hmmc->Instance, add);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    if(errorstate != HAL_MMC_ERROR_NONE)                /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear all the static flags */
      __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);    /* HAL辅助宏/函数 */
      hmmc->ErrorCode |= errorstate;                    /* 赋值或初始化 */
      hmmc->State = HAL_MMC_STATE_READY;                /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Configure the MMC DPSM (Data Path State Machine) */ 
    config.DataTimeOut   = SDMMC_DATATIMEOUT;           /* 赋值或初始化 */
    config.DataLength    = MMC_BLOCKSIZE * NumberOfBlocks;  /* 赋值或初始化 */
    config.DataBlockSize = SDIO_DATABLOCK_SIZE_512B;    /* 赋值或初始化 */
    config.TransferDir   = SDIO_TRANSFER_DIR_TO_CARD;   /* 赋值或初始化 */
    config.TransferMode  = SDIO_TRANSFER_MODE_BLOCK;    /* 赋值或初始化 */
    config.DPSM          = SDIO_DPSM_ENABLE;            /* 赋值或初始化 */
    (void)SDIO_ConfigData(hmmc->Instance, &config);     /* 执行语句 */

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
  *         HAL_MMC_GetCardState().
  * @note   You could also check the DMA transfer process through the MMC Rx
  *         interrupt event.
  * @param  hmmc: Pointer MMC handle
  * @param  pData: Pointer to the buffer that will contain the received data
  * @param  BlockAdd: Block Address from where data is to be read
  * @param  NumberOfBlocks: Number of blocks to read.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_ReadBlocks_DMA(MMC_HandleTypeDef *hmmc, uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  SDIO_DataInitTypeDef config;                          /* 执行语句 */
  uint32_t errorstate;                                  /* 32位无符号变量/函数 */
  uint32_t add = BlockAdd;                              /* 32位无符号变量/函数 */

  if(NULL == pData)                                     /* 条件判断 */
  {                                                     /* 代码块开始 */
    hmmc->ErrorCode |= HAL_MMC_ERROR_PARAM;             /* 赋值或初始化 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  if(hmmc->State == HAL_MMC_STATE_READY)                /* 条件判断 */
  {                                                     /* 代码块开始 */
    hmmc->ErrorCode = HAL_DMA_ERROR_NONE;               /* 赋值或初始化 */

    if((BlockAdd + NumberOfBlocks) > (hmmc->MmcCard.LogBlockNbr))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hmmc->ErrorCode |= HAL_MMC_ERROR_ADDR_OUT_OF_RANGE;  /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    hmmc->State = HAL_MMC_STATE_BUSY;                   /* 赋值或初始化 */

    /* Initialize data control register */
    hmmc->Instance->DCTRL = 0U;                         /* 赋值或初始化 */

#if defined(SDIO_STA_STBITERR)                          /* 条件编译 */
    __HAL_MMC_ENABLE_IT(hmmc, (SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_RXOVERR | SDIO_IT_DATAEND | SDIO_IT_STBITERR));  /* HAL辅助宏/函数 */
#else /* SDIO_STA_STBITERR not defined */
    __HAL_MMC_ENABLE_IT(hmmc, (SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_RXOVERR | SDIO_IT_DATAEND));  /* HAL辅助宏/函数 */
#endif /* SDIO_STA_STBITERR */

    /* Set the DMA transfer complete callback */
    hmmc->hdmarx->XferCpltCallback = MMC_DMAReceiveCplt;  /* 赋值或初始化 */

    /* Set the DMA error callback */
    hmmc->hdmarx->XferErrorCallback = MMC_DMAError;     /* 赋值或初始化 */

    /* Set the DMA Abort callback */
    hmmc->hdmarx->XferAbortCallback = NULL;             /* 赋值或初始化 */

    if ((hmmc->MmcCard.CardType) != MMC_HIGH_CAPACITY_CARD)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      add *= 512U;                                      /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Force DMA Direction */
    hmmc->hdmarx->Init.Direction = DMA_PERIPH_TO_MEMORY;  /* 赋值或初始化 */
    MODIFY_REG(hmmc->hdmarx->Instance->CR, DMA_SxCR_DIR, hmmc->hdmarx->Init.Direction);  /* 执行语句 */

    /* Enable the DMA Channel */
    if(HAL_DMA_Start_IT(hmmc->hdmarx, (uint32_t)&hmmc->Instance->FIFO, (uint32_t)pData, (uint32_t)(MMC_BLOCKSIZE * NumberOfBlocks)/4) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      __HAL_MMC_DISABLE_IT(hmmc, (SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_RXOVERR | SDIO_IT_DATAEND));  /* HAL辅助宏/函数 */
      __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);    /* HAL辅助宏/函数 */
      hmmc->ErrorCode = HAL_MMC_ERROR_DMA;              /* 赋值或初始化 */
      hmmc->State = HAL_MMC_STATE_READY;                /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Enable MMC DMA transfer */
      __HAL_MMC_DMA_ENABLE(hmmc);                       /* HAL辅助宏/函数 */

      /* Configure the MMC DPSM (Data Path State Machine) */
      config.DataTimeOut   = SDMMC_DATATIMEOUT;         /* 赋值或初始化 */
      config.DataLength    = MMC_BLOCKSIZE * NumberOfBlocks;  /* 赋值或初始化 */
      config.DataBlockSize = SDIO_DATABLOCK_SIZE_512B;  /* 赋值或初始化 */
      config.TransferDir   = SDIO_TRANSFER_DIR_TO_SDIO;  /* 赋值或初始化 */
      config.TransferMode  = SDIO_TRANSFER_MODE_BLOCK;  /* 赋值或初始化 */
      config.DPSM          = SDIO_DPSM_ENABLE;          /* 赋值或初始化 */
      (void)SDIO_ConfigData(hmmc->Instance, &config);   /* 执行语句 */

      /* Read Blocks in DMA mode */
      if(NumberOfBlocks > 1U)                           /* 条件判断 */
      {                                                 /* 代码块开始 */
        hmmc->Context = (MMC_CONTEXT_READ_MULTIPLE_BLOCK | MMC_CONTEXT_DMA);  /* 赋值或初始化 */

        /* Read Multi Block command */
        errorstate = SDMMC_CmdReadMultiBlock(hmmc->Instance, add);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        hmmc->Context = (MMC_CONTEXT_READ_SINGLE_BLOCK | MMC_CONTEXT_DMA);  /* 赋值或初始化 */

        /* Read Single Block command */
        errorstate = SDMMC_CmdReadSingleBlock(hmmc->Instance, add);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      if(errorstate != HAL_MMC_ERROR_NONE)              /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Clear all the static flags */
        __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);   /* HAL辅助宏/函数 */
        __HAL_MMC_DISABLE_IT(hmmc, (SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_RXOVERR | SDIO_IT_DATAEND));  /* HAL辅助宏/函数 */
        hmmc->ErrorCode = errorstate;                   /* 赋值或初始化 */
        hmmc->State = HAL_MMC_STATE_READY;              /* 赋值或初始化 */
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
  *         HAL_MMC_GetCardState().
  * @note   You could also check the DMA transfer process through the MMC Tx
  *         interrupt event.
  * @param  hmmc: Pointer to MMC handle
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  BlockAdd: Block Address where data will be written
  * @param  NumberOfBlocks: Number of blocks to write
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_WriteBlocks_DMA(MMC_HandleTypeDef *hmmc, uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  SDIO_DataInitTypeDef config;                          /* 执行语句 */
  uint32_t errorstate;                                  /* 32位无符号变量/函数 */
  uint32_t add = BlockAdd;                              /* 32位无符号变量/函数 */

  if(NULL == pData)                                     /* 条件判断 */
  {                                                     /* 代码块开始 */
    hmmc->ErrorCode |= HAL_MMC_ERROR_PARAM;             /* 赋值或初始化 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  if(hmmc->State == HAL_MMC_STATE_READY)                /* 条件判断 */
  {                                                     /* 代码块开始 */
    hmmc->ErrorCode = HAL_MMC_ERROR_NONE;               /* 赋值或初始化 */

    if((BlockAdd + NumberOfBlocks) > (hmmc->MmcCard.LogBlockNbr))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hmmc->ErrorCode |= HAL_MMC_ERROR_ADDR_OUT_OF_RANGE;  /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    hmmc->State = HAL_MMC_STATE_BUSY;                   /* 赋值或初始化 */

    /* Initialize data control register */
    hmmc->Instance->DCTRL = 0U;                         /* 赋值或初始化 */

    /* Enable MMC Error interrupts */
#if defined(SDIO_STA_STBITERR)                          /* 条件编译 */
	__HAL_MMC_ENABLE_IT(hmmc, (SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_TXUNDERR | SDIO_IT_STBITERR));  /* HAL辅助宏/函数 */
#else /* SDIO_STA_STBITERR not defined */
	__HAL_MMC_ENABLE_IT(hmmc, (SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_TXUNDERR));  /* HAL辅助宏/函数 */
#endif /* SDIO_STA_STBITERR */	

    /* Set the DMA transfer complete callback */
    hmmc->hdmatx->XferCpltCallback = MMC_DMATransmitCplt;  /* 赋值或初始化 */

    /* Set the DMA error callback */
    hmmc->hdmatx->XferErrorCallback = MMC_DMAError;     /* 赋值或初始化 */

    /* Set the DMA Abort callback */
    hmmc->hdmatx->XferAbortCallback = NULL;             /* 赋值或初始化 */

    if ((hmmc->MmcCard.CardType) != MMC_HIGH_CAPACITY_CARD)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      add *= 512U;                                      /* 赋值或初始化 */
    }                                                   /* 代码块结束 */


    /* Write Blocks in Polling mode */
    if(NumberOfBlocks > 1U)                             /* 条件判断 */
    {                                                   /* 代码块开始 */
      hmmc->Context = (MMC_CONTEXT_WRITE_MULTIPLE_BLOCK | MMC_CONTEXT_DMA);  /* 赋值或初始化 */

      /* Write Multi Block command */
      errorstate = SDMMC_CmdWriteMultiBlock(hmmc->Instance, add);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hmmc->Context = (MMC_CONTEXT_WRITE_SINGLE_BLOCK | MMC_CONTEXT_DMA);  /* 赋值或初始化 */

      /* Write Single Block command */
      errorstate = SDMMC_CmdWriteSingleBlock(hmmc->Instance, add);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    if(errorstate != HAL_MMC_ERROR_NONE)                /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear all the static flags */
      __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);    /* HAL辅助宏/函数 */
      __HAL_MMC_DISABLE_IT(hmmc, (SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_TXUNDERR | SDIO_IT_DATAEND));  /* HAL辅助宏/函数 */
      hmmc->ErrorCode |= errorstate;                    /* 赋值或初始化 */
      hmmc->State = HAL_MMC_STATE_READY;                /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Enable SDIO DMA transfer */
    __HAL_MMC_DMA_ENABLE(hmmc);                         /* HAL辅助宏/函数 */

    /* Force DMA Direction */
    hmmc->hdmatx->Init.Direction = DMA_MEMORY_TO_PERIPH;  /* 赋值或初始化 */
    MODIFY_REG(hmmc->hdmatx->Instance->CR, DMA_SxCR_DIR, hmmc->hdmatx->Init.Direction);  /* 执行语句 */

    /* Enable the DMA Channel */
    if(HAL_DMA_Start_IT(hmmc->hdmatx, (uint32_t)pData, (uint32_t)&hmmc->Instance->FIFO, (uint32_t)(MMC_BLOCKSIZE * NumberOfBlocks)/4) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      __HAL_MMC_DISABLE_IT(hmmc, (SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_TXUNDERR | SDIO_IT_DATAEND));  /* HAL辅助宏/函数 */
      __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);    /* HAL辅助宏/函数 */
      hmmc->ErrorCode |= HAL_MMC_ERROR_DMA;             /* 赋值或初始化 */
      hmmc->State = HAL_MMC_STATE_READY;                /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Configure the MMC DPSM (Data Path State Machine) */ 
      config.DataTimeOut   = SDMMC_DATATIMEOUT;         /* 赋值或初始化 */
      config.DataLength    = MMC_BLOCKSIZE * NumberOfBlocks;  /* 赋值或初始化 */
      config.DataBlockSize = SDIO_DATABLOCK_SIZE_512B;  /* 赋值或初始化 */
      config.TransferDir   = SDIO_TRANSFER_DIR_TO_CARD;  /* 赋值或初始化 */
      config.TransferMode  = SDIO_TRANSFER_MODE_BLOCK;  /* 赋值或初始化 */
      config.DPSM          = SDIO_DPSM_ENABLE;          /* 赋值或初始化 */
      (void)SDIO_ConfigData(hmmc->Instance, &config);   /* 执行语句 */

      return HAL_OK;                                    /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Erases the specified memory area of the given MMC card.
  * @note   This API should be followed by a check on the card state through
  *         HAL_MMC_GetCardState().
  * @param  hmmc: Pointer to MMC handle
  * @param  BlockStartAdd: Start Block address
  * @param  BlockEndAdd: End Block address
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_Erase(MMC_HandleTypeDef *hmmc, uint32_t BlockStartAdd, uint32_t BlockEndAdd)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t errorstate;                                  /* 32位无符号变量/函数 */
  uint32_t start_add = BlockStartAdd;                   /* 32位无符号变量/函数 */
  uint32_t end_add = BlockEndAdd;                       /* 32位无符号变量/函数 */

  if(hmmc->State == HAL_MMC_STATE_READY)                /* 条件判断 */
  {                                                     /* 代码块开始 */
    hmmc->ErrorCode = HAL_MMC_ERROR_NONE;               /* 赋值或初始化 */

    if(end_add < start_add)                             /* 条件判断 */
    {                                                   /* 代码块开始 */
      hmmc->ErrorCode |= HAL_MMC_ERROR_PARAM;           /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    if(end_add > (hmmc->MmcCard.LogBlockNbr))           /* 条件判断 */
    {                                                   /* 代码块开始 */
      hmmc->ErrorCode |= HAL_MMC_ERROR_ADDR_OUT_OF_RANGE;  /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    hmmc->State = HAL_MMC_STATE_BUSY;                   /* 赋值或初始化 */

    /* Check if the card command class supports erase command */
    if(((hmmc->MmcCard.Class) & SDIO_CCCC_ERASE) == 0U)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear all the static flags */
      __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);    /* HAL辅助宏/函数 */
      hmmc->ErrorCode |= HAL_MMC_ERROR_REQUEST_NOT_APPLICABLE;  /* 赋值或初始化 */
      hmmc->State = HAL_MMC_STATE_READY;                /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    if((SDIO_GetResponse(hmmc->Instance, SDIO_RESP1) & SDMMC_CARD_LOCKED) == SDMMC_CARD_LOCKED)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear all the static flags */
      __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);    /* HAL辅助宏/函数 */
      hmmc->ErrorCode |= HAL_MMC_ERROR_LOCK_UNLOCK_FAILED;  /* 赋值或初始化 */
      hmmc->State = HAL_MMC_STATE_READY;                /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    if ((hmmc->MmcCard.CardType) != MMC_HIGH_CAPACITY_CARD)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      start_add *= 512U;                                /* 赋值或初始化 */
      end_add   *= 512U;                                /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Send CMD35 MMC_ERASE_GRP_START with argument as addr  */
    errorstate = SDMMC_CmdEraseStartAdd(hmmc->Instance, start_add);  /* 赋值或初始化 */
    if(errorstate != HAL_MMC_ERROR_NONE)                /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear all the static flags */
      __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);    /* HAL辅助宏/函数 */
      hmmc->ErrorCode |= errorstate;                    /* 赋值或初始化 */
      hmmc->State = HAL_MMC_STATE_READY;                /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Send CMD36 MMC_ERASE_GRP_END with argument as addr  */
    errorstate = SDMMC_CmdEraseEndAdd(hmmc->Instance, end_add);  /* 赋值或初始化 */
    if(errorstate != HAL_MMC_ERROR_NONE)                /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear all the static flags */
      __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);    /* HAL辅助宏/函数 */
      hmmc->ErrorCode |= errorstate;                    /* 赋值或初始化 */
      hmmc->State = HAL_MMC_STATE_READY;                /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Send CMD38 ERASE */
    errorstate = SDMMC_CmdErase(hmmc->Instance);        /* 赋值或初始化 */
    if(errorstate != HAL_MMC_ERROR_NONE)                /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear all the static flags */
      __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);    /* HAL辅助宏/函数 */
      hmmc->ErrorCode |= errorstate;                    /* 赋值或初始化 */
      hmmc->State = HAL_MMC_STATE_READY;                /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    hmmc->State = HAL_MMC_STATE_READY;                  /* 赋值或初始化 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  This function handles MMC card interrupt request.
  * @param  hmmc: Pointer to MMC handle
  * @retval None
  */
void HAL_MMC_IRQHandler(MMC_HandleTypeDef *hmmc)        /* 中断服务函数 */
{                                                       /* 代码块开始 */
  uint32_t errorstate;                                  /* 32位无符号变量/函数 */
  uint32_t context = hmmc->Context;                     /* 32位无符号变量/函数 */

  /* Check for SDIO interrupt flags */
  if((__HAL_MMC_GET_FLAG(hmmc, SDIO_FLAG_RXFIFOHF) != RESET) && ((context & MMC_CONTEXT_IT) != 0U))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    MMC_Read_IT(hmmc);                                  /* 执行语句 */
  }                                                     /* 代码块结束 */

  else if(__HAL_MMC_GET_FLAG(hmmc, SDIO_FLAG_DATAEND) != RESET)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_FLAG_DATAEND);      /* HAL辅助宏/函数 */

#if defined(SDIO_STA_STBITERR)                          /* 条件编译 */
    __HAL_MMC_DISABLE_IT(hmmc, SDIO_IT_DATAEND | SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT|\
                             SDIO_IT_TXUNDERR| SDIO_IT_RXOVERR | SDIO_IT_STBITERR);  /* 执行语句 */
#else /* SDIO_STA_STBITERR not defined */
    __HAL_MMC_DISABLE_IT(hmmc, SDIO_IT_DATAEND  | SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT |\
                               SDIO_IT_TXUNDERR | SDIO_IT_RXOVERR  | SDIO_IT_TXFIFOHE |\
                               SDIO_IT_RXFIFOHF);       /* 执行语句 */
#endif /* SDIO_STA_STBITERR */

    hmmc->Instance->DCTRL &= ~(SDIO_DCTRL_DTEN);        /* 赋值或初始化 */

    if((context & MMC_CONTEXT_DMA) != 0U)               /* 条件判断 */
    {                                                   /* 代码块开始 */
      if((context & MMC_CONTEXT_WRITE_MULTIPLE_BLOCK) != 0U)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        errorstate = SDMMC_CmdStopTransfer(hmmc->Instance);  /* 赋值或初始化 */
        if(errorstate != HAL_MMC_ERROR_NONE)            /* 条件判断 */
        {                                               /* 代码块开始 */
          hmmc->ErrorCode |= errorstate;                /* 赋值或初始化 */
#if defined (USE_HAL_MMC_REGISTER_CALLBACKS) && (USE_HAL_MMC_REGISTER_CALLBACKS == 1U)  /* 条件编译 */
          hmmc->ErrorCallback(hmmc);                    /* 执行语句 */
#else                                                   /* 条件编译否则 */
          HAL_MMC_ErrorCallback(hmmc);                  /* HAL库函数调用 */
#endif                                                  /* 条件编译结束 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      if(((context & MMC_CONTEXT_READ_SINGLE_BLOCK) == 0U) && ((context & MMC_CONTEXT_READ_MULTIPLE_BLOCK) == 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Disable the DMA transfer for transmit request by setting the DMAEN bit
        in the MMC DCTRL register */
        hmmc->Instance->DCTRL &= (uint32_t)~((uint32_t)SDIO_DCTRL_DMAEN);  /* 赋值或初始化 */

        hmmc->State = HAL_MMC_STATE_READY;              /* 赋值或初始化 */

#if defined (USE_HAL_MMC_REGISTER_CALLBACKS) && (USE_HAL_MMC_REGISTER_CALLBACKS == 1U)  /* 条件编译 */
        hmmc->TxCpltCallback(hmmc);                     /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_MMC_TxCpltCallback(hmmc);                   /* HAL库函数调用 */
#endif                                                  /* 条件编译结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else if((context & MMC_CONTEXT_IT) != 0U)           /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* Stop Transfer for Write Multi blocks or Read Multi blocks */
      if(((context & MMC_CONTEXT_READ_MULTIPLE_BLOCK) != 0U) || ((context & MMC_CONTEXT_WRITE_MULTIPLE_BLOCK) != 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        errorstate = SDMMC_CmdStopTransfer(hmmc->Instance);  /* 赋值或初始化 */
        if(errorstate != HAL_MMC_ERROR_NONE)            /* 条件判断 */
        {                                               /* 代码块开始 */
          hmmc->ErrorCode |= errorstate;                /* 赋值或初始化 */
#if defined (USE_HAL_MMC_REGISTER_CALLBACKS) && (USE_HAL_MMC_REGISTER_CALLBACKS == 1U)  /* 条件编译 */
          hmmc->ErrorCallback(hmmc);                    /* 执行语句 */
#else                                                   /* 条件编译否则 */
          HAL_MMC_ErrorCallback(hmmc);                  /* HAL库函数调用 */
#endif                                                  /* 条件编译结束 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */

      /* Clear all the static flags */
      __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_DATA_FLAGS);  /* HAL辅助宏/函数 */

      hmmc->State = HAL_MMC_STATE_READY;                /* 赋值或初始化 */
      if(((context & MMC_CONTEXT_READ_SINGLE_BLOCK) != 0U) || ((context & MMC_CONTEXT_READ_MULTIPLE_BLOCK) != 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
#if defined (USE_HAL_MMC_REGISTER_CALLBACKS) && (USE_HAL_MMC_REGISTER_CALLBACKS == 1U)  /* 条件编译 */
        hmmc->RxCpltCallback(hmmc);                     /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_MMC_RxCpltCallback(hmmc);                   /* HAL库函数调用 */
#endif                                                  /* 条件编译结束 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
#if defined (USE_HAL_MMC_REGISTER_CALLBACKS) && (USE_HAL_MMC_REGISTER_CALLBACKS == 1U)  /* 条件编译 */
        hmmc->TxCpltCallback(hmmc);                     /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_MMC_TxCpltCallback(hmmc);                   /* HAL库函数调用 */
#endif                                                  /* 条件编译结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Nothing to do */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  else if((__HAL_MMC_GET_FLAG(hmmc, SDIO_FLAG_TXFIFOHE) != RESET) && ((context & MMC_CONTEXT_IT) != 0U))  /* 否则如果 */
  {                                                     /* 代码块开始 */
    MMC_Write_IT(hmmc);                                 /* 执行语句 */
  }                                                     /* 代码块结束 */

#if defined(SDIO_STA_STBITERR)                          /* 条件编译 */
  else if(__HAL_MMC_GET_FLAG(hmmc, SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_RXOVERR | SDIO_FLAG_TXUNDERR | SDIO_FLAG_STBITERR) != RESET)  /* 否则如果 */
#else /* SDIO_STA_STBITERR not defined */
  else if(__HAL_MMC_GET_FLAG(hmmc, SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_RXOVERR | SDIO_FLAG_TXUNDERR) != RESET)  /* 否则如果 */
#endif /* SDIO_STA_STBITERR */
  {                                                     /* 代码块开始 */
    /* Set Error code */
    if(__HAL_MMC_GET_FLAG(hmmc, SDIO_FLAG_DCRCFAIL) != RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hmmc->ErrorCode |= HAL_MMC_ERROR_DATA_CRC_FAIL;   /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    if(__HAL_MMC_GET_FLAG(hmmc, SDIO_FLAG_DTIMEOUT) != RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hmmc->ErrorCode |= HAL_MMC_ERROR_DATA_TIMEOUT;    /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    if(__HAL_MMC_GET_FLAG(hmmc, SDIO_FLAG_RXOVERR) != RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hmmc->ErrorCode |= HAL_MMC_ERROR_RX_OVERRUN;      /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    if(__HAL_MMC_GET_FLAG(hmmc, SDIO_FLAG_TXUNDERR) != RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hmmc->ErrorCode |= HAL_MMC_ERROR_TX_UNDERRUN;     /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
#if defined(SDIO_STA_STBITERR)                          /* 条件编译 */
    if(__HAL_MMC_GET_FLAG(hmmc, SDIO_FLAG_STBITERR) != RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hmmc->ErrorCode |= HAL_MMC_ERROR_DATA_TIMEOUT;    /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
#endif /* SDIO_STA_STBITERR */

#if defined(SDIO_STA_STBITERR)                          /* 条件编译 */
    /* Clear All flags */
    __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_DATA_FLAGS | SDIO_FLAG_STBITERR);  /* HAL辅助宏/函数 */

    /* Disable all interrupts */
    __HAL_MMC_DISABLE_IT(hmmc, SDIO_IT_DATAEND | SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT|\
                               SDIO_IT_TXUNDERR| SDIO_IT_RXOVERR | SDIO_IT_STBITERR);  /* 执行语句 */
#else /* SDIO_STA_STBITERR */
    /* Clear All flags */
    __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_DATA_FLAGS);  /* HAL辅助宏/函数 */

    /* Disable all interrupts */
    __HAL_MMC_DISABLE_IT(hmmc, SDIO_IT_DATAEND | SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT|\
                             SDIO_IT_TXUNDERR| SDIO_IT_RXOVERR);  /* 执行语句 */
#endif /* SDIO_STA_STBITERR */

    hmmc->ErrorCode |= SDMMC_CmdStopTransfer(hmmc->Instance);  /* 赋值或初始化 */

    if((context & MMC_CONTEXT_IT) != 0U)                /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the MMC state to ready to be able to start again the process */
      hmmc->State = HAL_MMC_STATE_READY;                /* 赋值或初始化 */
#if defined (USE_HAL_MMC_REGISTER_CALLBACKS) && (USE_HAL_MMC_REGISTER_CALLBACKS == 1U)  /* 条件编译 */
      hmmc->ErrorCallback(hmmc);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_MMC_ErrorCallback(hmmc);                      /* HAL库函数调用 */
#endif /* USE_HAL_MMC_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
    else if((context & MMC_CONTEXT_DMA) != 0U)          /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* Abort the MMC DMA Streams */
      if(hmmc->hdmatx != NULL)                          /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Set the DMA Tx abort callback */
        hmmc->hdmatx->XferAbortCallback = MMC_DMATxAbort;  /* 赋值或初始化 */
        /* Abort DMA in IT mode */
        if(HAL_DMA_Abort_IT(hmmc->hdmatx) != HAL_OK)    /* 条件判断 */
        {                                               /* 代码块开始 */
          MMC_DMATxAbort(hmmc->hdmatx);                 /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else if(hmmc->hdmarx != NULL)                     /* 否则如果 */
      {                                                 /* 代码块开始 */
        /* Set the DMA Rx abort callback */
        hmmc->hdmarx->XferAbortCallback = MMC_DMARxAbort;  /* 赋值或初始化 */
        /* Abort DMA in IT mode */
        if(HAL_DMA_Abort_IT(hmmc->hdmarx) != HAL_OK)    /* 条件判断 */
        {                                               /* 代码块开始 */
          MMC_DMARxAbort(hmmc->hdmarx);                 /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        hmmc->ErrorCode = HAL_MMC_ERROR_NONE;           /* 赋值或初始化 */
        hmmc->State = HAL_MMC_STATE_READY;              /* 赋值或初始化 */
#if defined (USE_HAL_MMC_REGISTER_CALLBACKS) && (USE_HAL_MMC_REGISTER_CALLBACKS == 1U)  /* 条件编译 */
        hmmc->AbortCpltCallback(hmmc);                  /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_MMC_AbortCallback(hmmc);                    /* HAL库函数调用 */
#endif                                                  /* 条件编译结束 */
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
  * @brief return the MMC state
  * @param hmmc: Pointer to mmc handle
  * @retval HAL state
  */
HAL_MMC_StateTypeDef HAL_MMC_GetState(MMC_HandleTypeDef *hmmc)  /* HAL库函数调用 */
{                                                       /* 代码块开始 */
  return hmmc->State;                                   /* 返回值 */
}                                                       /* 代码块结束 */

/**
* @brief  Return the MMC error code
* @param  hmmc : Pointer to a MMC_HandleTypeDef structure that contains
  *              the configuration information.
* @retval MMC Error Code
*/
uint32_t HAL_MMC_GetError(MMC_HandleTypeDef *hmmc)      /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  return hmmc->ErrorCode;                               /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief Tx Transfer completed callbacks
  * @param hmmc: Pointer to MMC handle
  * @retval None
  */
__weak void HAL_MMC_TxCpltCallback(MMC_HandleTypeDef *hmmc)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hmmc);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_MMC_TxCpltCallback can be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief Rx Transfer completed callbacks
  * @param hmmc: Pointer MMC handle
  * @retval None
  */
__weak void HAL_MMC_RxCpltCallback(MMC_HandleTypeDef *hmmc)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hmmc);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_MMC_RxCpltCallback can be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief MMC error callbacks
  * @param hmmc: Pointer MMC handle
  * @retval None
  */
__weak void HAL_MMC_ErrorCallback(MMC_HandleTypeDef *hmmc)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hmmc);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_MMC_ErrorCallback can be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief MMC Abort callbacks
  * @param hmmc: Pointer MMC handle
  * @retval None
  */
__weak void HAL_MMC_AbortCallback(MMC_HandleTypeDef *hmmc)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hmmc);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_MMC_AbortCallback can be implemented in the user file
   */
}                                                       /* 代码块结束 */

#if defined (USE_HAL_MMC_REGISTER_CALLBACKS) && (USE_HAL_MMC_REGISTER_CALLBACKS == 1U)  /* 条件编译 */
/**
  * @brief  Register a User MMC Callback
  *         To be used instead of the weak (surcharged) predefined callback
  * @param hmmc : MMC handle
  * @param CallbackId : ID of the callback to be registered
  *        This parameter can be one of the following values:
  *          @arg @ref HAL_MMC_TX_CPLT_CB_ID    MMC Tx Complete Callback ID
  *          @arg @ref HAL_MMC_RX_CPLT_CB_ID    MMC Rx Complete Callback ID
  *          @arg @ref HAL_MMC_ERROR_CB_ID      MMC Error Callback ID
  *          @arg @ref HAL_MMC_ABORT_CB_ID      MMC Abort Callback ID
  *          @arg @ref HAL_MMC_MSP_INIT_CB_ID   MMC MspInit Callback ID
  *          @arg @ref HAL_MMC_MSP_DEINIT_CB_ID MMC MspDeInit Callback ID
  * @param pCallback : pointer to the Callback function
  * @retval status
  */
HAL_StatusTypeDef HAL_MMC_RegisterCallback(MMC_HandleTypeDef *hmmc, HAL_MMC_CallbackIDTypeDef CallbackId, pMMC_CallbackTypeDef pCallback)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  if(pCallback == NULL)                                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hmmc->ErrorCode |= HAL_MMC_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Process locked */
  __HAL_LOCK(hmmc);                                     /* HAL辅助宏/函数 */

  if(hmmc->State == HAL_MMC_STATE_READY)                /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackId)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
    case HAL_MMC_TX_CPLT_CB_ID :                        /* case标签 */
      hmmc->TxCpltCallback = pCallback;                 /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_MMC_RX_CPLT_CB_ID :                        /* case标签 */
      hmmc->RxCpltCallback = pCallback;                 /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_MMC_ERROR_CB_ID :                          /* case标签 */
      hmmc->ErrorCallback = pCallback;                  /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_MMC_ABORT_CB_ID :                          /* case标签 */
      hmmc->AbortCpltCallback = pCallback;              /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_MMC_MSP_INIT_CB_ID :                       /* case标签 */
      hmmc->MspInitCallback = pCallback;                /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_MMC_MSP_DEINIT_CB_ID :                     /* case标签 */
      hmmc->MspDeInitCallback = pCallback;              /* 赋值或初始化 */
      break;                                            /* 跳出 */
    default :                                           /* default分支 */
      /* Update the error code */
      hmmc->ErrorCode |= HAL_MMC_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
      /* update return status */
      status =  HAL_ERROR;                              /* 赋值或初始化 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (hmmc->State == HAL_MMC_STATE_RESET)          /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackId)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
    case HAL_MMC_MSP_INIT_CB_ID :                       /* case标签 */
      hmmc->MspInitCallback = pCallback;                /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_MMC_MSP_DEINIT_CB_ID :                     /* case标签 */
      hmmc->MspDeInitCallback = pCallback;              /* 赋值或初始化 */
      break;                                            /* 跳出 */
    default :                                           /* default分支 */
      /* Update the error code */
      hmmc->ErrorCode |= HAL_MMC_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
      /* update return status */
      status =  HAL_ERROR;                              /* 赋值或初始化 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hmmc->ErrorCode |= HAL_MMC_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
    /* update return status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hmmc);                                   /* HAL辅助宏/函数 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Unregister a User MMC Callback
  *         MMC Callback is redirected to the weak (surcharged) predefined callback
  * @param hmmc : MMC handle
  * @param CallbackId : ID of the callback to be unregistered
  *        This parameter can be one of the following values:
  *          @arg @ref HAL_MMC_TX_CPLT_CB_ID    MMC Tx Complete Callback ID
  *          @arg @ref HAL_MMC_RX_CPLT_CB_ID    MMC Rx Complete Callback ID
  *          @arg @ref HAL_MMC_ERROR_CB_ID      MMC Error Callback ID
  *          @arg @ref HAL_MMC_ABORT_CB_ID      MMC Abort Callback ID
  *          @arg @ref HAL_MMC_MSP_INIT_CB_ID   MMC MspInit Callback ID
  *          @arg @ref HAL_MMC_MSP_DEINIT_CB_ID MMC MspDeInit Callback ID
  * @retval status
  */
HAL_StatusTypeDef HAL_MMC_UnRegisterCallback(MMC_HandleTypeDef *hmmc, HAL_MMC_CallbackIDTypeDef CallbackId)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Process locked */
  __HAL_LOCK(hmmc);                                     /* HAL辅助宏/函数 */

  if(hmmc->State == HAL_MMC_STATE_READY)                /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackId)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
    case HAL_MMC_TX_CPLT_CB_ID :                        /* case标签 */
      hmmc->TxCpltCallback = HAL_MMC_TxCpltCallback;    /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_MMC_RX_CPLT_CB_ID :                        /* case标签 */
      hmmc->RxCpltCallback = HAL_MMC_RxCpltCallback;    /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_MMC_ERROR_CB_ID :                          /* case标签 */
      hmmc->ErrorCallback = HAL_MMC_ErrorCallback;      /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_MMC_ABORT_CB_ID :                          /* case标签 */
      hmmc->AbortCpltCallback = HAL_MMC_AbortCallback;  /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_MMC_MSP_INIT_CB_ID :                       /* case标签 */
      hmmc->MspInitCallback = HAL_MMC_MspInit;          /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_MMC_MSP_DEINIT_CB_ID :                     /* case标签 */
      hmmc->MspDeInitCallback = HAL_MMC_MspDeInit;      /* 赋值或初始化 */
      break;                                            /* 跳出 */
    default :                                           /* default分支 */
      /* Update the error code */
      hmmc->ErrorCode |= HAL_MMC_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
      /* update return status */
      status =  HAL_ERROR;                              /* 赋值或初始化 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (hmmc->State == HAL_MMC_STATE_RESET)          /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackId)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
    case HAL_MMC_MSP_INIT_CB_ID :                       /* case标签 */
      hmmc->MspInitCallback = HAL_MMC_MspInit;          /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_MMC_MSP_DEINIT_CB_ID :                     /* case标签 */
      hmmc->MspDeInitCallback = HAL_MMC_MspDeInit;      /* 赋值或初始化 */
      break;                                            /* 跳出 */
    default :                                           /* default分支 */
      /* Update the error code */
      hmmc->ErrorCode |= HAL_MMC_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
      /* update return status */
      status =  HAL_ERROR;                              /* 赋值或初始化 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hmmc->ErrorCode |= HAL_MMC_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
    /* update return status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hmmc);                                   /* HAL辅助宏/函数 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */
#endif                                                  /* 条件编译结束 */

/**
  * @}
  */

/** @addtogroup MMC_Exported_Functions_Group3
 *  @brief   management functions
 *
@verbatim
  ==============================================================================
                      ##### Peripheral Control functions #####
  ==============================================================================
  [..]
    This subsection provides a set of functions allowing to control the MMC card
    operations and get the related information

@endverbatim
  * @{
  */

/**
  * @brief  Returns information the information of the card which are stored on
  *         the CID register.
  * @param  hmmc: Pointer to MMC handle
  * @param  pCID: Pointer to a HAL_MMC_CIDTypedef structure that
  *         contains all CID register parameters
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_GetCardCID(MMC_HandleTypeDef *hmmc, HAL_MMC_CardCIDTypeDef *pCID)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  pCID->ManufacturerID = (uint8_t)((hmmc->CID[0] & 0xFF000000U) >> 24U);  /* 赋值或初始化 */

  pCID->OEM_AppliID = (uint16_t)((hmmc->CID[0] & 0x00FFFF00U) >> 8U);  /* 赋值或初始化 */

  pCID->ProdName1 = (((hmmc->CID[0] & 0x000000FFU) << 24U) | ((hmmc->CID[1] & 0xFFFFFF00U) >> 8U));  /* 赋值或初始化 */

  pCID->ProdName2 = (uint8_t)(hmmc->CID[1] & 0x000000FFU);  /* 赋值或初始化 */

  pCID->ProdRev = (uint8_t)((hmmc->CID[2] & 0xFF000000U) >> 24U);  /* 赋值或初始化 */

  pCID->ProdSN = (((hmmc->CID[2] & 0x00FFFFFFU) << 8U) | ((hmmc->CID[3] & 0xFF000000U) >> 24U));  /* 赋值或初始化 */

  pCID->Reserved1 = (uint8_t)((hmmc->CID[3] & 0x00F00000U) >> 20U);  /* 赋值或初始化 */

  pCID->ManufactDate = (uint16_t)((hmmc->CID[3] & 0x000FFF00U) >> 8U);  /* 赋值或初始化 */

  pCID->CID_CRC = (uint8_t)((hmmc->CID[3] & 0x000000FEU) >> 1U);  /* 赋值或初始化 */

  pCID->Reserved2 = 1U;                                 /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Returns information the information of the card which are stored on
  *         the CSD register.
  * @param  hmmc: Pointer to MMC handle
  * @param  pCSD: Pointer to a HAL_MMC_CardCSDTypeDef structure that
  *         contains all CSD register parameters
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_GetCardCSD(MMC_HandleTypeDef *hmmc, HAL_MMC_CardCSDTypeDef *pCSD)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t block_nbr = 0;                               /* 32位无符号变量/函数 */

  pCSD->CSDStruct = (uint8_t)((hmmc->CSD[0] & 0xC0000000U) >> 30U);  /* 赋值或初始化 */

  pCSD->SysSpecVersion = (uint8_t)((hmmc->CSD[0] & 0x3C000000U) >> 26U);  /* 赋值或初始化 */

  pCSD->Reserved1 = (uint8_t)((hmmc->CSD[0] & 0x03000000U) >> 24U);  /* 赋值或初始化 */

  pCSD->TAAC = (uint8_t)((hmmc->CSD[0] & 0x00FF0000U) >> 16U);  /* 赋值或初始化 */

  pCSD->NSAC = (uint8_t)((hmmc->CSD[0] & 0x0000FF00U) >> 8U);  /* 赋值或初始化 */

  pCSD->MaxBusClkFrec = (uint8_t)(hmmc->CSD[0] & 0x000000FFU);  /* 赋值或初始化 */

  pCSD->CardComdClasses = (uint16_t)((hmmc->CSD[1] & 0xFFF00000U) >> 20U);  /* 赋值或初始化 */

  pCSD->RdBlockLen = (uint8_t)((hmmc->CSD[1] & 0x000F0000U) >> 16U);  /* 赋值或初始化 */

  pCSD->PartBlockRead   = (uint8_t)((hmmc->CSD[1] & 0x00008000U) >> 15U);  /* 赋值或初始化 */

  pCSD->WrBlockMisalign = (uint8_t)((hmmc->CSD[1] & 0x00004000U) >> 14U);  /* 赋值或初始化 */

  pCSD->RdBlockMisalign = (uint8_t)((hmmc->CSD[1] & 0x00002000U) >> 13U);  /* 赋值或初始化 */

  pCSD->DSRImpl = (uint8_t)((hmmc->CSD[1] & 0x00001000U) >> 12U);  /* 赋值或初始化 */

  pCSD->Reserved2 = 0U; /*!< Reserved */

  pCSD->DeviceSize = (((hmmc->CSD[1] & 0x000003FFU) << 2U) | ((hmmc->CSD[2] & 0xC0000000U) >> 30U));  /* 赋值或初始化 */

  pCSD->MaxRdCurrentVDDMin = (uint8_t)((hmmc->CSD[2] & 0x38000000U) >> 27U);  /* 赋值或初始化 */

  pCSD->MaxRdCurrentVDDMax = (uint8_t)((hmmc->CSD[2] & 0x07000000U) >> 24U);  /* 赋值或初始化 */

  pCSD->MaxWrCurrentVDDMin = (uint8_t)((hmmc->CSD[2] & 0x00E00000U) >> 21U);  /* 赋值或初始化 */

  pCSD->MaxWrCurrentVDDMax = (uint8_t)((hmmc->CSD[2] & 0x001C0000U) >> 18U);  /* 赋值或初始化 */

  pCSD->DeviceSizeMul = (uint8_t)((hmmc->CSD[2] & 0x00038000U) >> 15U);  /* 赋值或初始化 */

  if(MMC_ReadExtCSD(hmmc, &block_nbr, 212, 0x0FFFFFFFU) != HAL_OK) /* Field SEC_COUNT [215:212] */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  if(hmmc->MmcCard.CardType == MMC_LOW_CAPACITY_CARD)   /* 条件判断 */
  {                                                     /* 代码块开始 */
    hmmc->MmcCard.BlockNbr  = (pCSD->DeviceSize + 1U) ;  /* 赋值或初始化 */
    hmmc->MmcCard.BlockNbr *= (1UL << ((pCSD->DeviceSizeMul & 0x07U) + 2U));  /* 赋值或初始化 */
    hmmc->MmcCard.BlockSize = (1UL << (pCSD->RdBlockLen & 0x0FU));  /* 赋值或初始化 */
    hmmc->MmcCard.LogBlockNbr =  (hmmc->MmcCard.BlockNbr) * ((hmmc->MmcCard.BlockSize) / 512U);  /* 赋值或初始化 */
    hmmc->MmcCard.LogBlockSize = 512U;                  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if(hmmc->MmcCard.CardType == MMC_HIGH_CAPACITY_CARD)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    hmmc->MmcCard.BlockNbr = block_nbr;                 /* 赋值或初始化 */
    hmmc->MmcCard.LogBlockNbr = hmmc->MmcCard.BlockNbr;  /* 赋值或初始化 */
    hmmc->MmcCard.BlockSize = 512U;                     /* 赋值或初始化 */
    hmmc->MmcCard.LogBlockSize = hmmc->MmcCard.BlockSize;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Clear all the static flags */
    __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);      /* HAL辅助宏/函数 */
    hmmc->ErrorCode |= HAL_MMC_ERROR_UNSUPPORTED_FEATURE;  /* 赋值或初始化 */
    hmmc->State = HAL_MMC_STATE_READY;                  /* 赋值或初始化 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  pCSD->EraseGrSize = (uint8_t)((hmmc->CSD[2] & 0x00004000U) >> 14U);  /* 赋值或初始化 */

  pCSD->EraseGrMul = (uint8_t)((hmmc->CSD[2] & 0x00003F80U) >> 7U);  /* 赋值或初始化 */

  pCSD->WrProtectGrSize = (uint8_t)(hmmc->CSD[2] & 0x0000007FU);  /* 赋值或初始化 */

  pCSD->WrProtectGrEnable = (uint8_t)((hmmc->CSD[3] & 0x80000000U) >> 31U);  /* 赋值或初始化 */

  pCSD->ManDeflECC = (uint8_t)((hmmc->CSD[3] & 0x60000000U) >> 29U);  /* 赋值或初始化 */

  pCSD->WrSpeedFact = (uint8_t)((hmmc->CSD[3] & 0x1C000000U) >> 26U);  /* 赋值或初始化 */

  pCSD->MaxWrBlockLen= (uint8_t)((hmmc->CSD[3] & 0x03C00000U) >> 22U);  /* 赋值或初始化 */

  pCSD->WriteBlockPaPartial = (uint8_t)((hmmc->CSD[3] & 0x00200000U) >> 21U);  /* 赋值或初始化 */

  pCSD->Reserved3 = 0;                                  /* 赋值或初始化 */

  pCSD->ContentProtectAppli = (uint8_t)((hmmc->CSD[3] & 0x00010000U) >> 16U);  /* 赋值或初始化 */

  pCSD->FileFormatGroup = (uint8_t)((hmmc->CSD[3] & 0x00008000U) >> 15U);  /* 赋值或初始化 */

  pCSD->CopyFlag = (uint8_t)((hmmc->CSD[3] & 0x00004000U) >> 14U);  /* 赋值或初始化 */

  pCSD->PermWrProtect = (uint8_t)((hmmc->CSD[3] & 0x00002000U) >> 13U);  /* 赋值或初始化 */

  pCSD->TempWrProtect = (uint8_t)((hmmc->CSD[3] & 0x00001000U) >> 12U);  /* 赋值或初始化 */

  pCSD->FileFormat = (uint8_t)((hmmc->CSD[3] & 0x00000C00U) >> 10U);  /* 赋值或初始化 */

  pCSD->ECC= (uint8_t)((hmmc->CSD[3] & 0x00000300U) >> 8U);  /* 赋值或初始化 */

  pCSD->CSD_CRC = (uint8_t)((hmmc->CSD[3] & 0x000000FEU) >> 1U);  /* 赋值或初始化 */

  pCSD->Reserved4 = 1;                                  /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Gets the MMC card info.
  * @param  hmmc: Pointer to MMC handle
  * @param  pCardInfo: Pointer to the HAL_MMC_CardInfoTypeDef structure that
  *         will contain the MMC card status information
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_GetCardInfo(MMC_HandleTypeDef *hmmc, HAL_MMC_CardInfoTypeDef *pCardInfo)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  pCardInfo->CardType     = (uint32_t)(hmmc->MmcCard.CardType);  /* 赋值或初始化 */
  pCardInfo->Class        = (uint32_t)(hmmc->MmcCard.Class);  /* 赋值或初始化 */
  pCardInfo->RelCardAdd   = (uint32_t)(hmmc->MmcCard.RelCardAdd);  /* 赋值或初始化 */
  pCardInfo->BlockNbr     = (uint32_t)(hmmc->MmcCard.BlockNbr);  /* 赋值或初始化 */
  pCardInfo->BlockSize    = (uint32_t)(hmmc->MmcCard.BlockSize);  /* 赋值或初始化 */
  pCardInfo->LogBlockNbr  = (uint32_t)(hmmc->MmcCard.LogBlockNbr);  /* 赋值或初始化 */
  pCardInfo->LogBlockSize = (uint32_t)(hmmc->MmcCard.LogBlockSize);  /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Enables wide bus operation for the requested card if supported by
  *         card.
  * @param  hmmc: Pointer to MMC handle
  * @param  WideMode: Specifies the MMC card wide bus mode
  *          This parameter can be one of the following values:
  *            @arg SDIO_BUS_WIDE_8B: 8-bit data transfer
  *            @arg SDIO_BUS_WIDE_4B: 4-bit data transfer
  *            @arg SDIO_BUS_WIDE_1B: 1-bit data transfer
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_ConfigWideBusOperation(MMC_HandleTypeDef *hmmc, uint32_t WideMode)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  __IO uint32_t count = 0U;                             /* 赋值或初始化 */
  SDIO_InitTypeDef Init;                                /* 执行语句 */
  uint32_t errorstate;                                  /* 32位无符号变量/函数 */
  uint32_t response = 0U, busy = 0U;                    /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_SDIO_BUS_WIDE(WideMode));             /* 执行语句 */

  /* Change State */
  hmmc->State = HAL_MMC_STATE_BUSY;                     /* 赋值或初始化 */

  /* Update Clock for Bus mode update */
  Init.ClockEdge           = SDIO_CLOCK_EDGE_RISING;    /* 赋值或初始化 */
  Init.ClockBypass         = SDIO_CLOCK_BYPASS_DISABLE;  /* 赋值或初始化 */
  Init.ClockPowerSave      = SDIO_CLOCK_POWER_SAVE_DISABLE;  /* 赋值或初始化 */
  Init.BusWide             = WideMode;                  /* 赋值或初始化 */
  Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;  /* 赋值或初始化 */
  Init.ClockDiv            = SDIO_INIT_CLK_DIV;         /* 赋值或初始化 */
  /* Initialize SDIO*/
  (void)SDIO_Init(hmmc->Instance, Init);                /* 执行语句 */

  if(WideMode == SDIO_BUS_WIDE_8B)                      /* 条件判断 */
  {                                                     /* 代码块开始 */
    errorstate = SDMMC_CmdSwitch(hmmc->Instance, 0x03B70200U);  /* 赋值或初始化 */
    if(errorstate != HAL_MMC_ERROR_NONE)                /* 条件判断 */
    {                                                   /* 代码块开始 */
      hmmc->ErrorCode |= errorstate;                    /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if(WideMode == SDIO_BUS_WIDE_4B)                 /* 否则如果 */
  {                                                     /* 代码块开始 */
    errorstate = SDMMC_CmdSwitch(hmmc->Instance, 0x03B70100U);  /* 赋值或初始化 */
    if(errorstate != HAL_MMC_ERROR_NONE)                /* 条件判断 */
    {                                                   /* 代码块开始 */
      hmmc->ErrorCode |= errorstate;                    /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if(WideMode == SDIO_BUS_WIDE_1B)                 /* 否则如果 */
  {                                                     /* 代码块开始 */
    errorstate = SDMMC_CmdSwitch(hmmc->Instance, 0x03B70000U);  /* 赋值或初始化 */
    if(errorstate != HAL_MMC_ERROR_NONE)                /* 条件判断 */
    {                                                   /* 代码块开始 */
      hmmc->ErrorCode |= errorstate;                    /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* WideMode is not a valid argument*/
    hmmc->ErrorCode |= HAL_MMC_ERROR_PARAM;             /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Check for switch error and violation of the trial number of sending CMD 13 */
  while(busy == 0U)                                     /* while循环 */
  {                                                     /* 代码块开始 */
    if(count == SDMMC_MAX_TRIAL)                        /* 条件判断 */
    {                                                   /* 代码块开始 */
      hmmc->State = HAL_MMC_STATE_READY;                /* 赋值或初始化 */
      hmmc->ErrorCode |= HAL_MMC_ERROR_REQUEST_NOT_APPLICABLE;  /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
    count++;                                            /* 执行语句 */

    /* While card is not ready for data and trial number for sending CMD13 is not exceeded */
    errorstate = SDMMC_CmdSendStatus(hmmc->Instance, (uint32_t)(((uint32_t)hmmc->MmcCard.RelCardAdd) << 16U));  /* 赋值或初始化 */
    if(errorstate != HAL_MMC_ERROR_NONE)                /* 条件判断 */
    {                                                   /* 代码块开始 */
      hmmc->ErrorCode |= errorstate;                    /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Get command response */
    response = SDIO_GetResponse(hmmc->Instance, SDIO_RESP1);  /* 赋值或初始化 */

    /* Get operating voltage*/
    busy = (((response >> 7U) == 1U) ? 0U : 1U);        /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* While card is not ready for data and trial number for sending CMD13 is not exceeded */
  count = SDMMC_DATATIMEOUT;                            /* 赋值或初始化 */
  while((response & 0x00000100U) == 0U)                 /* while循环 */
  {                                                     /* 代码块开始 */
    if(count == 0U)                                     /* 条件判断 */
    {                                                   /* 代码块开始 */
      hmmc->State = HAL_MMC_STATE_READY;                /* 赋值或初始化 */
      hmmc->ErrorCode |= HAL_MMC_ERROR_REQUEST_NOT_APPLICABLE;  /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
    count--;                                            /* 执行语句 */

    /* While card is not ready for data and trial number for sending CMD13 is not exceeded */
    errorstate = SDMMC_CmdSendStatus(hmmc->Instance, (uint32_t)(((uint32_t)hmmc->MmcCard.RelCardAdd) << 16U));  /* 赋值或初始化 */
    if(errorstate != HAL_MMC_ERROR_NONE)                /* 条件判断 */
    {                                                   /* 代码块开始 */
      hmmc->ErrorCode |= errorstate;                    /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Get command response */
    response = SDIO_GetResponse(hmmc->Instance, SDIO_RESP1);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if(hmmc->ErrorCode != HAL_MMC_ERROR_NONE)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Clear all the static flags */
    __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);      /* HAL辅助宏/函数 */
    hmmc->State = HAL_MMC_STATE_READY;                  /* 赋值或初始化 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Configure the SDIO peripheral */
    Init.ClockEdge           = hmmc->Init.ClockEdge;    /* 赋值或初始化 */
    Init.ClockBypass         = hmmc->Init.ClockBypass;  /* 赋值或初始化 */
    Init.ClockPowerSave      = hmmc->Init.ClockPowerSave;  /* 赋值或初始化 */
    Init.BusWide             = WideMode;                /* 赋值或初始化 */
    Init.HardwareFlowControl = hmmc->Init.HardwareFlowControl;  /* 赋值或初始化 */
    Init.ClockDiv            = hmmc->Init.ClockDiv;     /* 赋值或初始化 */
    (void)SDIO_Init(hmmc->Instance, Init);              /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Change State */
  hmmc->State = HAL_MMC_STATE_READY;                    /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Gets the current mmc card data state.
  * @param  hmmc: pointer to MMC handle
  * @retval Card state
  */
HAL_MMC_CardStateTypeDef HAL_MMC_GetCardState(MMC_HandleTypeDef *hmmc)  /* HAL库函数调用 */
{                                                       /* 代码块开始 */
  uint32_t cardstate;                                   /* 32位无符号变量/函数 */
  uint32_t errorstate;                                  /* 32位无符号变量/函数 */
  uint32_t resp1 = 0U;                                  /* 32位无符号变量/函数 */

  errorstate = MMC_SendStatus(hmmc, &resp1);            /* 赋值或初始化 */
  if(errorstate != HAL_MMC_ERROR_NONE)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hmmc->ErrorCode |= errorstate;                      /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  cardstate = ((resp1 >> 9U) & 0x0FU);                  /* 赋值或初始化 */

  return (HAL_MMC_CardStateTypeDef)cardstate;           /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Abort the current transfer and disable the MMC.
  * @param  hmmc: pointer to a MMC_HandleTypeDef structure that contains
  *                the configuration information for MMC module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_Abort(MMC_HandleTypeDef *hmmc)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_MMC_CardStateTypeDef CardState;                   /* HAL库函数调用 */

  /* DIsable All interrupts */
  __HAL_MMC_DISABLE_IT(hmmc, SDIO_IT_DATAEND | SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT|\
                             SDIO_IT_TXUNDERR| SDIO_IT_RXOVERR);  /* 执行语句 */

  /* Clear All flags */
  __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_DATA_FLAGS);   /* HAL辅助宏/函数 */

  if((hmmc->hdmatx != NULL) || (hmmc->hdmarx != NULL))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable the MMC DMA request */
    hmmc->Instance->DCTRL &= (uint32_t)~((uint32_t)SDIO_DCTRL_DMAEN);  /* 赋值或初始化 */

    /* Abort the MMC DMA Tx Stream */
    if(hmmc->hdmatx != NULL)                            /* 条件判断 */
    {                                                   /* 代码块开始 */
      if(HAL_DMA_Abort(hmmc->hdmatx) != HAL_OK)         /* 条件判断 */
      {                                                 /* 代码块开始 */
        hmmc->ErrorCode |= HAL_MMC_ERROR_DMA;           /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    /* Abort the MMC DMA Rx Stream */
    if(hmmc->hdmarx != NULL)                            /* 条件判断 */
    {                                                   /* 代码块开始 */
      if(HAL_DMA_Abort(hmmc->hdmarx) != HAL_OK)         /* 条件判断 */
      {                                                 /* 代码块开始 */
        hmmc->ErrorCode |= HAL_MMC_ERROR_DMA;           /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  hmmc->State = HAL_MMC_STATE_READY;                    /* 赋值或初始化 */

  /* Initialize the MMC operation */
  hmmc->Context = MMC_CONTEXT_NONE;                     /* 赋值或初始化 */

  CardState = HAL_MMC_GetCardState(hmmc);               /* 赋值或初始化 */
  if((CardState == HAL_MMC_CARD_RECEIVING) || (CardState == HAL_MMC_CARD_SENDING))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hmmc->ErrorCode = SDMMC_CmdStopTransfer(hmmc->Instance);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  if(hmmc->ErrorCode != HAL_MMC_ERROR_NONE)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Abort the current transfer and disable the MMC (IT mode).
  * @param  hmmc: pointer to a MMC_HandleTypeDef structure that contains
  *                the configuration information for MMC module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_Abort_IT(MMC_HandleTypeDef *hmmc)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_MMC_CardStateTypeDef CardState;                   /* HAL库函数调用 */

  /* DIsable All interrupts */
  __HAL_MMC_DISABLE_IT(hmmc, SDIO_IT_DATAEND | SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT|\
                           SDIO_IT_TXUNDERR| SDIO_IT_RXOVERR);  /* 执行语句 */

  /* Clear All flags */
  __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_DATA_FLAGS);   /* HAL辅助宏/函数 */

  if((hmmc->hdmatx != NULL) || (hmmc->hdmarx != NULL))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable the MMC DMA request */
    hmmc->Instance->DCTRL &= (uint32_t)~((uint32_t)SDIO_DCTRL_DMAEN);  /* 赋值或初始化 */

    /* Abort the MMC DMA Tx Stream */
    if(hmmc->hdmatx != NULL)                            /* 条件判断 */
    {                                                   /* 代码块开始 */
      hmmc->hdmatx->XferAbortCallback =  MMC_DMATxAbort;  /* 赋值或初始化 */
      if(HAL_DMA_Abort_IT(hmmc->hdmatx) != HAL_OK)      /* 条件判断 */
      {                                                 /* 代码块开始 */
        hmmc->hdmatx = NULL;                            /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    /* Abort the MMC DMA Rx Stream */
    if(hmmc->hdmarx != NULL)                            /* 条件判断 */
    {                                                   /* 代码块开始 */
      hmmc->hdmarx->XferAbortCallback =  MMC_DMARxAbort;  /* 赋值或初始化 */
      if(HAL_DMA_Abort_IT(hmmc->hdmarx) != HAL_OK)      /* 条件判断 */
      {                                                 /* 代码块开始 */
        hmmc->hdmarx = NULL;                            /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* No transfer ongoing on both DMA channels*/
  if((hmmc->hdmatx == NULL) && (hmmc->hdmarx == NULL))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    CardState = HAL_MMC_GetCardState(hmmc);             /* 赋值或初始化 */
    hmmc->State = HAL_MMC_STATE_READY;                  /* 赋值或初始化 */

    if((CardState == HAL_MMC_CARD_RECEIVING) || (CardState == HAL_MMC_CARD_SENDING))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hmmc->ErrorCode = SDMMC_CmdStopTransfer(hmmc->Instance);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    if(hmmc->ErrorCode != HAL_MMC_ERROR_NONE)           /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
#if defined (USE_HAL_MMC_REGISTER_CALLBACKS) && (USE_HAL_MMC_REGISTER_CALLBACKS == 1U)  /* 条件编译 */
      hmmc->AbortCpltCallback(hmmc);                    /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_MMC_AbortCallback(hmmc);                      /* HAL库函数调用 */
#endif                                                  /* 条件编译结束 */
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
/** @addtogroup MMC_Private_Functions
  * @{
  */

/**
  * @brief  DMA MMC transmit process complete callback 
  * @param  hdma: DMA handle
  * @retval None
  */
static void MMC_DMATransmitCplt(DMA_HandleTypeDef *hdma)       /* 静态函数 */
{                                                       /* 代码块开始 */
  MMC_HandleTypeDef* hmmc = (MMC_HandleTypeDef* )(hdma->Parent);  /* 赋值或初始化 */

  /* Enable DATAEND Interrupt */
  __HAL_MMC_ENABLE_IT(hmmc, (SDIO_IT_DATAEND));         /* HAL辅助宏/函数 */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA MMC receive process complete callback 
  * @param  hdma: DMA handle
  * @retval None
  */
static void MMC_DMAReceiveCplt(DMA_HandleTypeDef *hdma)    /* 静态函数 */
{                                                       /* 代码块开始 */
  MMC_HandleTypeDef* hmmc = (MMC_HandleTypeDef* )(hdma->Parent);  /* 赋值或初始化 */
  uint32_t errorstate;                                  /* 32位无符号变量/函数 */

  /* Send stop command in multiblock write */
  if(hmmc->Context == (MMC_CONTEXT_READ_MULTIPLE_BLOCK | MMC_CONTEXT_DMA))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    errorstate = SDMMC_CmdStopTransfer(hmmc->Instance);  /* 赋值或初始化 */
    if(errorstate != HAL_MMC_ERROR_NONE)                /* 条件判断 */
    {                                                   /* 代码块开始 */
      hmmc->ErrorCode |= errorstate;                    /* 赋值或初始化 */
#if defined (USE_HAL_MMC_REGISTER_CALLBACKS) && (USE_HAL_MMC_REGISTER_CALLBACKS == 1U)  /* 条件编译 */
      hmmc->ErrorCallback(hmmc);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_MMC_ErrorCallback(hmmc);                      /* HAL库函数调用 */
#endif                                                  /* 条件编译结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Disable the DMA transfer for transmit request by setting the DMAEN bit
  in the MMC DCTRL register */
  hmmc->Instance->DCTRL &= (uint32_t)~((uint32_t)SDIO_DCTRL_DMAEN);  /* 赋值或初始化 */

  /* Clear all the static flags */
  __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_DATA_FLAGS);   /* HAL辅助宏/函数 */

  hmmc->State = HAL_MMC_STATE_READY;                    /* 赋值或初始化 */

#if defined (USE_HAL_MMC_REGISTER_CALLBACKS) && (USE_HAL_MMC_REGISTER_CALLBACKS == 1U)  /* 条件编译 */
  hmmc->RxCpltCallback(hmmc);                           /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_MMC_RxCpltCallback(hmmc);                         /* HAL库函数调用 */
#endif                                                  /* 条件编译结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA MMC communication error callback 
  * @param  hdma: DMA handle
  * @retval None
  */
static void MMC_DMAError(DMA_HandleTypeDef *hdma)       /* 静态函数 */
{                                                       /* 代码块开始 */
  MMC_HandleTypeDef* hmmc = (MMC_HandleTypeDef* )(hdma->Parent);  /* 赋值或初始化 */
  HAL_MMC_CardStateTypeDef CardState;                   /* HAL库函数调用 */
  uint32_t RxErrorCode, TxErrorCode;                    /* 32位无符号变量/函数 */

  /* if DMA error is FIFO error ignore it */
  if(HAL_DMA_GetError(hdma) != HAL_DMA_ERROR_FE)        /* 条件判断 */
  {                                                     /* 代码块开始 */
    RxErrorCode = hmmc->hdmarx->ErrorCode;              /* 赋值或初始化 */
    TxErrorCode = hmmc->hdmatx->ErrorCode;              /* 赋值或初始化 */
    if((RxErrorCode == HAL_DMA_ERROR_TE) || (TxErrorCode == HAL_DMA_ERROR_TE))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear All flags */
      __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);    /* HAL辅助宏/函数 */

      /* Disable All interrupts */
      __HAL_MMC_DISABLE_IT(hmmc, SDIO_IT_DATAEND | SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT|\
        SDIO_IT_TXUNDERR| SDIO_IT_RXOVERR);             /* 执行语句 */

      hmmc->ErrorCode |= HAL_MMC_ERROR_DMA;             /* 赋值或初始化 */
      CardState = HAL_MMC_GetCardState(hmmc);           /* 赋值或初始化 */
      if((CardState == HAL_MMC_CARD_RECEIVING) || (CardState == HAL_MMC_CARD_SENDING))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        hmmc->ErrorCode |= SDMMC_CmdStopTransfer(hmmc->Instance);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */

      hmmc->State= HAL_MMC_STATE_READY;                 /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

#if defined (USE_HAL_MMC_REGISTER_CALLBACKS) && (USE_HAL_MMC_REGISTER_CALLBACKS == 1U)  /* 条件编译 */
    hmmc->ErrorCallback(hmmc);                          /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_MMC_ErrorCallback(hmmc);                        /* HAL库函数调用 */
#endif                                                  /* 条件编译结束 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA MMC Tx Abort callback 
  * @param  hdma: DMA handle
  * @retval None
  */
static void MMC_DMATxAbort(DMA_HandleTypeDef *hdma)     /* 静态函数 */
{                                                       /* 代码块开始 */
  MMC_HandleTypeDef* hmmc = (MMC_HandleTypeDef* )(hdma->Parent);  /* 赋值或初始化 */
  HAL_MMC_CardStateTypeDef CardState;                   /* HAL库函数调用 */

  if(hmmc->hdmatx != NULL)                              /* 条件判断 */
  {                                                     /* 代码块开始 */
    hmmc->hdmatx = NULL;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* All DMA channels are aborted */
  if(hmmc->hdmarx == NULL)                              /* 条件判断 */
  {                                                     /* 代码块开始 */
    CardState = HAL_MMC_GetCardState(hmmc);             /* 赋值或初始化 */
    hmmc->ErrorCode = HAL_MMC_ERROR_NONE;               /* 赋值或初始化 */
    hmmc->State = HAL_MMC_STATE_READY;                  /* 赋值或初始化 */
    if((CardState == HAL_MMC_CARD_RECEIVING) || (CardState == HAL_MMC_CARD_SENDING))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hmmc->ErrorCode |= SDMMC_CmdStopTransfer(hmmc->Instance);  /* 赋值或初始化 */

      if(hmmc->ErrorCode != HAL_MMC_ERROR_NONE)         /* 条件判断 */
      {                                                 /* 代码块开始 */
#if defined (USE_HAL_MMC_REGISTER_CALLBACKS) && (USE_HAL_MMC_REGISTER_CALLBACKS == 1U)  /* 条件编译 */
        hmmc->AbortCpltCallback(hmmc);                  /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_MMC_AbortCallback(hmmc);                    /* HAL库函数调用 */
#endif                                                  /* 条件编译结束 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
#if defined (USE_HAL_MMC_REGISTER_CALLBACKS) && (USE_HAL_MMC_REGISTER_CALLBACKS == 1U)  /* 条件编译 */
        hmmc->ErrorCallback(hmmc);                      /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_MMC_ErrorCallback(hmmc);                    /* HAL库函数调用 */
#endif                                                  /* 条件编译结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA MMC Rx Abort callback 
  * @param  hdma: DMA handle
  * @retval None
  */
static void MMC_DMARxAbort(DMA_HandleTypeDef *hdma)     /* 静态函数 */
{                                                       /* 代码块开始 */
  MMC_HandleTypeDef* hmmc = (MMC_HandleTypeDef* )(hdma->Parent);  /* 赋值或初始化 */
  HAL_MMC_CardStateTypeDef CardState;                   /* HAL库函数调用 */

  if(hmmc->hdmarx != NULL)                              /* 条件判断 */
  {                                                     /* 代码块开始 */
    hmmc->hdmarx = NULL;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* All DMA channels are aborted */
  if(hmmc->hdmatx == NULL)                              /* 条件判断 */
  {                                                     /* 代码块开始 */
    CardState = HAL_MMC_GetCardState(hmmc);             /* 赋值或初始化 */
    hmmc->ErrorCode = HAL_MMC_ERROR_NONE;               /* 赋值或初始化 */
    hmmc->State = HAL_MMC_STATE_READY;                  /* 赋值或初始化 */
    if((CardState == HAL_MMC_CARD_RECEIVING) || (CardState == HAL_MMC_CARD_SENDING))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hmmc->ErrorCode |= SDMMC_CmdStopTransfer(hmmc->Instance);  /* 赋值或初始化 */

      if(hmmc->ErrorCode != HAL_MMC_ERROR_NONE)         /* 条件判断 */
      {                                                 /* 代码块开始 */
#if defined (USE_HAL_MMC_REGISTER_CALLBACKS) && (USE_HAL_MMC_REGISTER_CALLBACKS == 1U)  /* 条件编译 */
        hmmc->AbortCpltCallback(hmmc);                  /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_MMC_AbortCallback(hmmc);                    /* HAL库函数调用 */
#endif                                                  /* 条件编译结束 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
#if defined (USE_HAL_MMC_REGISTER_CALLBACKS) && (USE_HAL_MMC_REGISTER_CALLBACKS == 1U)  /* 条件编译 */
        hmmc->ErrorCallback(hmmc);                      /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_MMC_ErrorCallback(hmmc);                    /* HAL库函数调用 */
#endif                                                  /* 条件编译结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initializes the mmc card.
  * @param  hmmc: Pointer to MMC handle
  * @retval MMC Card error state
  */
static uint32_t MMC_InitCard(MMC_HandleTypeDef *hmmc)   /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  HAL_MMC_CardCSDTypeDef CSD;                           /* HAL库函数调用 */
  uint32_t errorstate;                                  /* 32位无符号变量/函数 */
  uint16_t mmc_rca = 1U;                                /* 16位无符号变量/函数 */

  /* Check the power State */
  if(SDIO_GetPowerState(hmmc->Instance) == 0U)          /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Power off */
    return HAL_MMC_ERROR_REQUEST_NOT_APPLICABLE;        /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Send CMD2 ALL_SEND_CID */
  errorstate = SDMMC_CmdSendCID(hmmc->Instance);        /* 赋值或初始化 */
  if(errorstate != HAL_MMC_ERROR_NONE)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return errorstate;                                  /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Get Card identification number data */
    hmmc->CID[0U] = SDIO_GetResponse(hmmc->Instance, SDIO_RESP1);  /* 赋值或初始化 */
    hmmc->CID[1U] = SDIO_GetResponse(hmmc->Instance, SDIO_RESP2);  /* 赋值或初始化 */
    hmmc->CID[2U] = SDIO_GetResponse(hmmc->Instance, SDIO_RESP3);  /* 赋值或初始化 */
    hmmc->CID[3U] = SDIO_GetResponse(hmmc->Instance, SDIO_RESP4);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Send CMD3 SET_REL_ADDR with argument 0 */
  /* MMC Card publishes its RCA. */
  errorstate = SDMMC_CmdSetRelAdd(hmmc->Instance, &mmc_rca);  /* 赋值或初始化 */
  if(errorstate != HAL_MMC_ERROR_NONE)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return errorstate;                                  /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Get the MMC card RCA */
  hmmc->MmcCard.RelCardAdd = mmc_rca;                   /* 赋值或初始化 */

  /* Send CMD9 SEND_CSD with argument as card's RCA */
  errorstate = SDMMC_CmdSendCSD(hmmc->Instance, (uint32_t)(hmmc->MmcCard.RelCardAdd << 16U));  /* 赋值或初始化 */
  if(errorstate != HAL_MMC_ERROR_NONE)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return errorstate;                                  /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Get Card Specific Data */
    hmmc->CSD[0U] = SDIO_GetResponse(hmmc->Instance, SDIO_RESP1);  /* 赋值或初始化 */
    hmmc->CSD[1U] = SDIO_GetResponse(hmmc->Instance, SDIO_RESP2);  /* 赋值或初始化 */
    hmmc->CSD[2U] = SDIO_GetResponse(hmmc->Instance, SDIO_RESP3);  /* 赋值或初始化 */
    hmmc->CSD[3U] = SDIO_GetResponse(hmmc->Instance, SDIO_RESP4);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Get the Card Class */
  hmmc->MmcCard.Class = (SDIO_GetResponse(hmmc->Instance, SDIO_RESP2) >> 20U);  /* 赋值或初始化 */

  /* Get CSD parameters */
  if (HAL_MMC_GetCardCSD(hmmc, &CSD) != HAL_OK)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    return hmmc->ErrorCode;                             /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Select the Card */
  errorstate = SDMMC_CmdSelDesel(hmmc->Instance, (uint32_t)(((uint32_t)hmmc->MmcCard.RelCardAdd) << 16U));  /* 赋值或初始化 */
  if(errorstate != HAL_MMC_ERROR_NONE)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return errorstate;                                  /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Configure SDIO peripheral interface */
  (void)SDIO_Init(hmmc->Instance, hmmc->Init);          /* 执行语句 */

  /* All cards are initialized */
  return HAL_MMC_ERROR_NONE;                            /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Enquires cards about their operating voltage and configures clock
  *         controls and stores MMC information that will be needed in future
  *         in the MMC handle.
  * @param  hmmc: Pointer to MMC handle
  * @retval error state
  */
static uint32_t MMC_PowerON(MMC_HandleTypeDef *hmmc)    /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  __IO uint32_t count = 0U;                             /* 赋值或初始化 */
  uint32_t response = 0U, validvoltage = 0U;            /* 32位无符号变量/函数 */
  uint32_t errorstate;                                  /* 32位无符号变量/函数 */

  /* CMD0: GO_IDLE_STATE */
  errorstate = SDMMC_CmdGoIdleState(hmmc->Instance);    /* 赋值或初始化 */
  if(errorstate != HAL_MMC_ERROR_NONE)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return errorstate;                                  /* 返回值 */
  }                                                     /* 代码块结束 */

  while(validvoltage == 0U)                             /* while循环 */
  {                                                     /* 代码块开始 */
    if(count++ == SDMMC_MAX_VOLT_TRIAL)                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_MMC_ERROR_INVALID_VOLTRANGE;           /* 返回值 */
    }                                                   /* 代码块结束 */

    /* SEND CMD1 APP_CMD with MMC_HIGH_VOLTAGE_RANGE(0xC0FF8000) as argument */
    errorstate = SDMMC_CmdOpCondition(hmmc->Instance, eMMC_HIGH_VOLTAGE_RANGE);  /* 赋值或初始化 */
    if(errorstate != HAL_MMC_ERROR_NONE)                /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_MMC_ERROR_UNSUPPORTED_FEATURE;         /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Get command response */
    response = SDIO_GetResponse(hmmc->Instance, SDIO_RESP1);  /* 赋值或初始化 */

    /* Get operating voltage*/
    validvoltage = (((response >> 31U) == 1U) ? 1U : 0U);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* When power routine is finished and command returns valid voltage */
  if (((response & (0xFF000000U)) >> 24U) == 0xC0U)     /* 条件判断 */
  {                                                     /* 代码块开始 */
    hmmc->MmcCard.CardType = MMC_HIGH_CAPACITY_CARD;    /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    hmmc->MmcCard.CardType = MMC_LOW_CAPACITY_CARD;     /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return HAL_MMC_ERROR_NONE;                            /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Turns the SDIO output signals off.
  * @param  hmmc: Pointer to MMC handle
  * @retval None
  */
static void MMC_PowerOFF(MMC_HandleTypeDef *hmmc)       /* 静态函数 */
{                                                       /* 代码块开始 */
  /* Set Power State to OFF */
  (void)SDIO_PowerState_OFF(hmmc->Instance);            /* 执行语句 */
}                                                       /* 代码块结束 */

/**
  * @brief  Returns the current card's status.
  * @param  hmmc: Pointer to MMC handle
  * @param  pCardStatus: pointer to the buffer that will contain the MMC card
  *         status (Card Status register)
  * @retval error state
  */
static uint32_t MMC_SendStatus(MMC_HandleTypeDef *hmmc, uint32_t *pCardStatus)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t errorstate;                                  /* 32位无符号变量/函数 */

  if(pCardStatus == NULL)                               /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_MMC_ERROR_PARAM;                         /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Send Status command */
  errorstate = SDMMC_CmdSendStatus(hmmc->Instance, (uint32_t)(hmmc->MmcCard.RelCardAdd << 16U));  /* 赋值或初始化 */
  if(errorstate != HAL_MMC_ERROR_NONE)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return errorstate;                                  /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Get MMC card status */
  *pCardStatus = SDIO_GetResponse(hmmc->Instance, SDIO_RESP1);  /* 赋值或初始化 */

  return HAL_MMC_ERROR_NONE;                            /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Reads extended CSD register to get the sectors number of the device
  * @param  hmmc: Pointer to MMC handle
  * @param  pFieldData: Pointer to the read buffer
  * @param  FieldIndex: Index of the field to be read
  * @param  Timeout: Specify timeout value
  * @retval HAL status
  */
static uint32_t MMC_ReadExtCSD(MMC_HandleTypeDef *hmmc, uint32_t *pFieldData, uint16_t FieldIndex, uint32_t Timeout)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  SDIO_DataInitTypeDef config;                          /* 执行语句 */
  uint32_t errorstate;                                  /* 32位无符号变量/函数 */
  uint32_t tickstart = HAL_GetTick();                   /* 32位无符号变量/函数 */
  uint32_t count;                                       /* 32位无符号变量/函数 */
  uint32_t i = 0;                                       /* 32位无符号变量/函数 */
  uint32_t tmp_data;                                    /* 32位无符号变量/函数 */

  hmmc->ErrorCode = HAL_MMC_ERROR_NONE;                 /* 赋值或初始化 */

  /* Initialize data control register */
  hmmc->Instance->DCTRL = 0;                            /* 赋值或初始化 */

  /* Configure the MMC DPSM (Data Path State Machine) */
  config.DataTimeOut   = SDMMC_DATATIMEOUT;             /* 赋值或初始化 */
  config.DataLength    = 512;                           /* 赋值或初始化 */
  config.DataBlockSize = SDIO_DATABLOCK_SIZE_512B;      /* 赋值或初始化 */
  config.TransferDir   = SDIO_TRANSFER_DIR_TO_SDIO;     /* 赋值或初始化 */
  config.TransferMode  = SDIO_TRANSFER_MODE_BLOCK;      /* 赋值或初始化 */
  config.DPSM          = SDIO_DPSM_ENABLE;              /* 赋值或初始化 */
  (void)SDIO_ConfigData(hmmc->Instance, &config);       /* 执行语句 */

  /* Set Block Size for Card */
  errorstate = SDMMC_CmdSendEXTCSD(hmmc->Instance, 0);  /* 赋值或初始化 */
  if(errorstate != HAL_MMC_ERROR_NONE)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Clear all the static flags */
    __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);      /* HAL辅助宏/函数 */
    hmmc->ErrorCode |= errorstate;                      /* 赋值或初始化 */
    hmmc->State = HAL_MMC_STATE_READY;                  /* 赋值或初始化 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Poll on SDMMC flags */
  while(!__HAL_MMC_GET_FLAG(hmmc, SDIO_FLAG_RXOVERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DATAEND))  /* while循环 */
  {                                                     /* 代码块开始 */
    if(__HAL_MMC_GET_FLAG(hmmc, SDIO_FLAG_RXFIFOHF))    /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Read data from SDMMC Rx FIFO */
      for(count = 0U; count < 8U; count++)              /* for循环 */
      {                                                 /* 代码块开始 */
        tmp_data = SDIO_ReadFIFO(hmmc->Instance);       /* 赋值或初始化 */
        /* eg : SEC_COUNT   : FieldIndex = 212 => i+count = 53 */
        /*      DEVICE_TYPE : FieldIndex = 196 => i+count = 49 */
        if ((i + count) == ((uint32_t)FieldIndex/4U))   /* 条件判断 */
        {                                               /* 代码块开始 */
          *pFieldData = tmp_data;                       /* 赋值或初始化 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      i += 8U;                                          /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    if(((HAL_GetTick()-tickstart) >=  Timeout) || (Timeout == 0U))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear all the static flags */
      __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);    /* HAL辅助宏/函数 */
      hmmc->ErrorCode |= HAL_MMC_ERROR_TIMEOUT;         /* 赋值或初始化 */
      hmmc->State= HAL_MMC_STATE_READY;                 /* 赋值或初始化 */
      return HAL_TIMEOUT;                               /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* While card is not ready for data and trial number for sending CMD13 is not exceeded */
  errorstate = SDMMC_CmdSendStatus(hmmc->Instance, (uint32_t)(((uint32_t)hmmc->MmcCard.RelCardAdd) << 16));  /* 赋值或初始化 */
  if(errorstate != HAL_MMC_ERROR_NONE)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hmmc->ErrorCode |= errorstate;                      /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Clear all the static flags */
  __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_DATA_FLAGS);   /* HAL辅助宏/函数 */

  hmmc->State = HAL_MMC_STATE_READY;                    /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */


/**
  * @brief  Wrap up reading in non-blocking mode.
  * @param  hmmc: pointer to a MMC_HandleTypeDef structure that contains
  *              the configuration information.
  * @retval None
  */
static void MMC_Read_IT(MMC_HandleTypeDef *hmmc)        /* 静态函数 */
{                                                       /* 代码块开始 */
  uint32_t count, data, dataremaining;                  /* 32位无符号变量/函数 */
  uint8_t* tmp;                                         /* 执行语句 */

  tmp = hmmc->pRxBuffPtr;                               /* 赋值或初始化 */
  dataremaining = hmmc->RxXferSize;                     /* 赋值或初始化 */

  if (dataremaining > 0U)                               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Read data from SDIO Rx FIFO */
    for(count = 0U; count < 8U; count++)                /* for循环 */
    {                                                   /* 代码块开始 */
      data = SDIO_ReadFIFO(hmmc->Instance);             /* 赋值或初始化 */
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

    hmmc->pRxBuffPtr = tmp;                             /* 赋值或初始化 */
    hmmc->RxXferSize = dataremaining;                   /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Wrap up writing in non-blocking mode.
  * @param  hmmc: pointer to a MMC_HandleTypeDef structure that contains
  *              the configuration information.
  * @retval None
  */
static void MMC_Write_IT(MMC_HandleTypeDef *hmmc)       /* 静态函数 */
{                                                       /* 代码块开始 */
  uint32_t count, data, dataremaining;                  /* 32位无符号变量/函数 */
  uint8_t* tmp;                                         /* 执行语句 */

  tmp = hmmc->pTxBuffPtr;                               /* 赋值或初始化 */
  dataremaining = hmmc->TxXferSize;                     /* 赋值或初始化 */

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
      (void)SDIO_WriteFIFO(hmmc->Instance, &data);      /* 执行语句 */
    }                                                   /* 代码块结束 */

    hmmc->pTxBuffPtr = tmp;                             /* 赋值或初始化 */
    hmmc->TxXferSize = dataremaining;                   /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

#endif /* SDIO */

#endif /* HAL_MMC_MODULE_ENABLED */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
