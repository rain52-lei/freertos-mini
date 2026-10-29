/* 文件: stm32f4xx_hal_qspi.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_qspi.c
  * @author  MCD Application Team
  * @brief   QSPI HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the QuadSPI interface (QSPI).
  *           + Initialization and de-initialization functions
  *           + Indirect functional mode management
  *           + Memory-mapped functional mode management
  *           + Auto-polling functional mode management
  *           + Interrupts and flags management
  *           + DMA channel configuration for indirect functional mode
  *           + Errors management and abort functionality
  *
  *
  @verbatim
 ===============================================================================
                        ##### How to use this driver #####
 ===============================================================================
  [..]
    *** Initialization ***
    ======================
    [..]
      (#) As prerequisite, fill in the HAL_QSPI_MspInit() :
        (++) Enable QuadSPI clock interface with __HAL_RCC_QSPI_CLK_ENABLE().
        (++) Reset QuadSPI Peripheral with __HAL_RCC_QSPI_FORCE_RESET() and __HAL_RCC_QSPI_RELEASE_RESET().
        (++) Enable the clocks for the QuadSPI GPIOS with __HAL_RCC_GPIOx_CLK_ENABLE().
        (++) Configure these QuadSPI pins in alternate mode using HAL_GPIO_Init().
        (++) If interrupt mode is used, enable and configure QuadSPI global
            interrupt with HAL_NVIC_SetPriority() and HAL_NVIC_EnableIRQ().
        (++) If DMA mode is used, enable the clocks for the QuadSPI DMA channel
            with __HAL_RCC_DMAx_CLK_ENABLE(), configure DMA with HAL_DMA_Init(),
            link it with QuadSPI handle using __HAL_LINKDMA(), enable and configure
            DMA channel global interrupt with HAL_NVIC_SetPriority() and HAL_NVIC_EnableIRQ().
      (#) Configure the flash size, the clock prescaler, the fifo threshold, the
          clock mode, the sample shifting and the CS high time using the HAL_QSPI_Init() function.

    *** Indirect functional mode ***
    ================================
    [..]
      (#) Configure the command sequence using the HAL_QSPI_Command() or HAL_QSPI_Command_IT()
          functions :
         (++) Instruction phase : the mode used and if present the instruction opcode.
         (++) Address phase : the mode used and if present the size and the address value.
         (++) Alternate-bytes phase : the mode used and if present the size and the alternate
             bytes values.
         (++) Dummy-cycles phase : the number of dummy cycles (mode used is same as data phase).
         (++) Data phase : the mode used and if present the number of bytes.
         (++) Double Data Rate (DDR) mode : the activation (or not) of this mode and the delay
             if activated.
         (++) Sending Instruction Only Once (SIOO) mode : the activation (or not) of this mode.
      (#) If no data is required for the command, it is sent directly to the memory :
         (++) In polling mode, the output of the function is done when the transfer is complete.
         (++) In interrupt mode, HAL_QSPI_CmdCpltCallback() will be called when the transfer is complete.
      (#) For the indirect write mode, use HAL_QSPI_Transmit(), HAL_QSPI_Transmit_DMA() or
          HAL_QSPI_Transmit_IT() after the command configuration :
         (++) In polling mode, the output of the function is done when the transfer is complete.
         (++) In interrupt mode, HAL_QSPI_FifoThresholdCallback() will be called when the fifo threshold
             is reached and HAL_QSPI_TxCpltCallback() will be called when the transfer is complete.
         (++) In DMA mode, HAL_QSPI_TxHalfCpltCallback() will be called at the half transfer and
             HAL_QSPI_TxCpltCallback() will be called when the transfer is complete.
      (#) For the indirect read mode, use HAL_QSPI_Receive(), HAL_QSPI_Receive_DMA() or
          HAL_QSPI_Receive_IT() after the command configuration :
         (++) In polling mode, the output of the function is done when the transfer is complete.
         (++) In interrupt mode, HAL_QSPI_FifoThresholdCallback() will be called when the fifo threshold
             is reached and HAL_QSPI_RxCpltCallback() will be called when the transfer is complete.
         (++) In DMA mode, HAL_QSPI_RxHalfCpltCallback() will be called at the half transfer and
             HAL_QSPI_RxCpltCallback() will be called when the transfer is complete.

    *** Auto-polling functional mode ***
    ====================================
    [..]
      (#) Configure the command sequence and the auto-polling functional mode using the
          HAL_QSPI_AutoPolling() or HAL_QSPI_AutoPolling_IT() functions :
         (++) Instruction phase : the mode used and if present the instruction opcode.
         (++) Address phase : the mode used and if present the size and the address value.
         (++) Alternate-bytes phase : the mode used and if present the size and the alternate
             bytes values.
         (++) Dummy-cycles phase : the number of dummy cycles (mode used is same as data phase).
         (++) Data phase : the mode used.
         (++) Double Data Rate (DDR) mode : the activation (or not) of this mode and the delay
             if activated.
         (++) Sending Instruction Only Once (SIOO) mode : the activation (or not) of this mode.
         (++) The size of the status bytes, the match value, the mask used, the match mode (OR/AND),
             the polling interval and the automatic stop activation.
      (#) After the configuration :
         (++) In polling mode, the output of the function is done when the status match is reached. The
             automatic stop is activated to avoid an infinite loop.
         (++) In interrupt mode, HAL_QSPI_StatusMatchCallback() will be called each time the status match is reached.

    *** Memory-mapped functional mode ***
    =====================================
    [..]
      (#) Configure the command sequence and the memory-mapped functional mode using the
          HAL_QSPI_MemoryMapped() functions :
         (++) Instruction phase : the mode used and if present the instruction opcode.
         (++) Address phase : the mode used and the size.
         (++) Alternate-bytes phase : the mode used and if present the size and the alternate
             bytes values.
         (++) Dummy-cycles phase : the number of dummy cycles (mode used is same as data phase).
         (++) Data phase : the mode used.
         (++) Double Data Rate (DDR) mode : the activation (or not) of this mode and the delay
             if activated.
         (++) Sending Instruction Only Once (SIOO) mode : the activation (or not) of this mode.
         (++) The timeout activation and the timeout period.
      (#) After the configuration, the QuadSPI will be used as soon as an access on the AHB is done on
          the address range. HAL_QSPI_TimeOutCallback() will be called when the timeout expires.

    *** Errors management and abort functionality ***
    =================================================
    [..]
      (#) HAL_QSPI_GetError() function gives the error raised during the last operation.
      (#) HAL_QSPI_Abort() and HAL_QSPI_AbortIT() functions aborts any on-going operation and
          flushes the fifo :
         (++) In polling mode, the output of the function is done when the transfer
              complete bit is set and the busy bit cleared.
         (++) In interrupt mode, HAL_QSPI_AbortCpltCallback() will be called when
              the transfer complete bit is set.

    *** Control functions ***
    =========================
    [..]
      (#) HAL_QSPI_GetState() function gives the current state of the HAL QuadSPI driver.
      (#) HAL_QSPI_SetTimeout() function configures the timeout value used in the driver.
      (#) HAL_QSPI_SetFifoThreshold() function configures the threshold on the Fifo of the QSPI IP.
      (#) HAL_QSPI_GetFifoThreshold() function gives the current of the Fifo's threshold
      (#) HAL_QSPI_SetFlashID() function configures the index of the flash memory to be accessed.

    *** Callback registration ***
    =============================================
    [..]
      The compilation define  USE_HAL_QSPI_REGISTER_CALLBACKS when set to 1
      allows the user to configure dynamically the driver callbacks.

      Use Functions @ref HAL_QSPI_RegisterCallback() to register a user callback,
      it allows to register following callbacks:
        (+) ErrorCallback : callback when error occurs.
        (+) AbortCpltCallback : callback when abort is completed.
        (+) FifoThresholdCallback : callback when the fifo threshold is reached.
        (+) CmdCpltCallback : callback when a command without data is completed.
        (+) RxCpltCallback : callback when a reception transfer is completed.
        (+) TxCpltCallback : callback when a transmission transfer is completed.
        (+) RxHalfCpltCallback : callback when half of the reception transfer is completed.
        (+) TxHalfCpltCallback : callback when half of the transmission transfer is completed.
        (+) StatusMatchCallback : callback when a status match occurs.
        (+) TimeOutCallback : callback when the timeout perioed expires.
        (+) MspInitCallback    : QSPI MspInit.
        (+) MspDeInitCallback  : QSPI MspDeInit.
      This function takes as parameters the HAL peripheral handle, the Callback ID
      and a pointer to the user callback function.

      Use function @ref HAL_QSPI_UnRegisterCallback() to reset a callback to the default
      weak (surcharged) function. It allows to reset following callbacks:
        (+) ErrorCallback : callback when error occurs.
        (+) AbortCpltCallback : callback when abort is completed.
        (+) FifoThresholdCallback : callback when the fifo threshold is reached.
        (+) CmdCpltCallback : callback when a command without data is completed.
        (+) RxCpltCallback : callback when a reception transfer is completed.
        (+) TxCpltCallback : callback when a transmission transfer is completed.
        (+) RxHalfCpltCallback : callback when half of the reception transfer is completed.
        (+) TxHalfCpltCallback : callback when half of the transmission transfer is completed.
        (+) StatusMatchCallback : callback when a status match occurs.
        (+) TimeOutCallback : callback when the timeout perioed expires.
        (+) MspInitCallback    : QSPI MspInit.
        (+) MspDeInitCallback  : QSPI MspDeInit.
      This function) takes as parameters the HAL peripheral handle and the Callback ID.

      By default, after the @ref HAL_QSPI_Init and if the state is HAL_QSPI_STATE_RESET
      all callbacks are reset to the corresponding legacy weak (surcharged) functions.
      Exception done for MspInit and MspDeInit callbacks that are respectively
      reset to the legacy weak (surcharged) functions in the @ref HAL_QSPI_Init
      and @ref  HAL_QSPI_DeInit only when these callbacks are null (not registered beforehand).
      If not, MspInit or MspDeInit are not null, the @ref HAL_QSPI_Init and @ref HAL_QSPI_DeInit
      keep and use the user MspInit/MspDeInit callbacks (registered beforehand)

      Callbacks can be registered/unregistered in READY state only.
      Exception done for MspInit/MspDeInit callbacks that can be registered/unregistered
      in READY or RESET state, thus registered (user) MspInit/DeInit callbacks can be used
      during the Init/DeInit.
      In that case first register the MspInit/MspDeInit user callbacks
      using @ref HAL_QSPI_RegisterCallback before calling @ref HAL_QSPI_DeInit
      or @ref HAL_QSPI_Init function.

      When The compilation define USE_HAL_QSPI_REGISTER_CALLBACKS is set to 0 or
      not defined, the callback registering feature is not available
      and weak (surcharged) callbacks are used.

    *** Workarounds linked to Silicon Limitation ***
    ====================================================
    [..]
      (#) Workarounds Implemented inside HAL Driver
         (++) Extra data written in the FIFO at the end of a read transfer

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
  *                       opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"                              /* 包含 STM32/HAL 头文件 */

#if defined(QUADSPI)                                    /* 条件编译 */

/** @addtogroup STM32F4xx_HAL_Driver
  * @{
  */

/** @defgroup QSPI QSPI
  * @brief QSPI HAL module driver
  * @{
  */
#ifdef HAL_QSPI_MODULE_ENABLED                          /* 条件编译 */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/** @defgroup QSPI_Private_Constants QSPI Private Constants
  * @{
  */
#define QSPI_FUNCTIONAL_MODE_INDIRECT_WRITE 0x00000000U                     /*!<Indirect write mode*/
#define QSPI_FUNCTIONAL_MODE_INDIRECT_READ  ((uint32_t)QUADSPI_CCR_FMODE_0) /*!<Indirect read mode*/
#define QSPI_FUNCTIONAL_MODE_AUTO_POLLING   ((uint32_t)QUADSPI_CCR_FMODE_1) /*!<Automatic polling mode*/
#define QSPI_FUNCTIONAL_MODE_MEMORY_MAPPED  ((uint32_t)QUADSPI_CCR_FMODE)   /*!<Memory-mapped mode*/
/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/
/** @defgroup QSPI_Private_Macros QSPI Private Macros
  * @{
  */
#define IS_QSPI_FUNCTIONAL_MODE(MODE) (((MODE) == QSPI_FUNCTIONAL_MODE_INDIRECT_WRITE) || \
                                       ((MODE) == QSPI_FUNCTIONAL_MODE_INDIRECT_READ)  || \
                                       ((MODE) == QSPI_FUNCTIONAL_MODE_AUTO_POLLING)   || \
                                       ((MODE) == QSPI_FUNCTIONAL_MODE_MEMORY_MAPPED))  /* 源代码 */
/**
  * @}
  */

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void QSPI_DMARxCplt(DMA_HandleTypeDef *hdma);    /* 静态函数 */
static void QSPI_DMATxCplt(DMA_HandleTypeDef *hdma);    /* 静态函数 */
static void QSPI_DMARxHalfCplt(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void QSPI_DMATxHalfCplt(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void QSPI_DMAError(DMA_HandleTypeDef *hdma);     /* 静态函数 */
static void QSPI_DMAAbortCplt(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static HAL_StatusTypeDef QSPI_WaitFlagStateUntilTimeout(QSPI_HandleTypeDef *hqspi, uint32_t Flag, FlagStatus State, uint32_t Tickstart, uint32_t Timeout);  /* 静态变量/函数 */
static void QSPI_Config(QSPI_HandleTypeDef *hqspi, QSPI_CommandTypeDef *cmd, uint32_t FunctionalMode);  /* 静态函数 */

/* Exported functions --------------------------------------------------------*/

/** @defgroup QSPI_Exported_Functions QSPI Exported Functions
  * @{
  */

/** @defgroup QSPI_Exported_Functions_Group1 Initialization/de-initialization functions
  *  @brief    Initialization and Configuration functions
  *
@verbatim
===============================================================================
            ##### Initialization and Configuration functions #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to :
      (+) Initialize the QuadSPI.
      (+) De-initialize the QuadSPI.

@endverbatim
  * @{
  */

/**
  * @brief Initialize the QSPI mode according to the specified parameters
  *        in the QSPI_InitTypeDef and initialize the associated handle.
  * @param hqspi : QSPI handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_QSPI_Init(QSPI_HandleTypeDef *hqspi)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status;                             /* HAL状态类型函数 */
  uint32_t tickstart = HAL_GetTick();                   /* 32位无符号变量/函数 */

  /* Check the QSPI handle allocation */
  if(hqspi == NULL)                                     /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the parameters */
  assert_param(IS_QSPI_ALL_INSTANCE(hqspi->Instance));  /* 执行语句 */
  assert_param(IS_QSPI_CLOCK_PRESCALER(hqspi->Init.ClockPrescaler));  /* 执行语句 */
  assert_param(IS_QSPI_FIFO_THRESHOLD(hqspi->Init.FifoThreshold));  /* 执行语句 */
  assert_param(IS_QSPI_SSHIFT(hqspi->Init.SampleShifting));  /* 执行语句 */
  assert_param(IS_QSPI_FLASH_SIZE(hqspi->Init.FlashSize));  /* 执行语句 */
  assert_param(IS_QSPI_CS_HIGH_TIME(hqspi->Init.ChipSelectHighTime));  /* 执行语句 */
  assert_param(IS_QSPI_CLOCK_MODE(hqspi->Init.ClockMode));  /* 执行语句 */
  assert_param(IS_QSPI_DUAL_FLASH_MODE(hqspi->Init.DualFlash));  /* 执行语句 */

  if (hqspi->Init.DualFlash != QSPI_DUALFLASH_ENABLE )  /* 条件判断 */
  {                                                     /* 代码块开始 */
    assert_param(IS_QSPI_FLASH_ID(hqspi->Init.FlashID));  /* 执行语句 */
  }                                                     /* 代码块结束 */

  if(hqspi->State == HAL_QSPI_STATE_RESET)              /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Allocate lock resource and initialize it */
    hqspi->Lock = HAL_UNLOCKED;                         /* 赋值或初始化 */

#if (USE_HAL_QSPI_REGISTER_CALLBACKS == 1)              /* 条件编译 */
    /* Reset Callback pointers in HAL_QSPI_STATE_RESET only */
    hqspi->ErrorCallback         = HAL_QSPI_ErrorCallback;  /* 赋值或初始化 */
    hqspi->AbortCpltCallback     = HAL_QSPI_AbortCpltCallback;  /* 赋值或初始化 */
    hqspi->FifoThresholdCallback = HAL_QSPI_FifoThresholdCallback;  /* 赋值或初始化 */
    hqspi->CmdCpltCallback       = HAL_QSPI_CmdCpltCallback;  /* 赋值或初始化 */
    hqspi->RxCpltCallback        = HAL_QSPI_RxCpltCallback;  /* 赋值或初始化 */
    hqspi->TxCpltCallback        = HAL_QSPI_TxCpltCallback;  /* 赋值或初始化 */
    hqspi->RxHalfCpltCallback    = HAL_QSPI_RxHalfCpltCallback;  /* 赋值或初始化 */
    hqspi->TxHalfCpltCallback    = HAL_QSPI_TxHalfCpltCallback;  /* 赋值或初始化 */
    hqspi->StatusMatchCallback   = HAL_QSPI_StatusMatchCallback;  /* 赋值或初始化 */
    hqspi->TimeOutCallback       = HAL_QSPI_TimeOutCallback;  /* 赋值或初始化 */

    if(hqspi->MspInitCallback == NULL)                  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hqspi->MspInitCallback = HAL_QSPI_MspInit;        /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Init the low level hardware */
    hqspi->MspInitCallback(hqspi);                      /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /* Init the low level hardware : GPIO, CLOCK */
    HAL_QSPI_MspInit(hqspi);                            /* HAL库函数调用 */
#endif                                                  /* 条件编译结束 */

    /* Configure the default timeout for the QSPI memory access */
    HAL_QSPI_SetTimeout(hqspi, HAL_QSPI_TIMEOUT_DEFAULT_VALUE);  /* HAL库函数调用 */
  }                                                     /* 代码块结束 */

  /* Configure QSPI FIFO Threshold */
  MODIFY_REG(hqspi->Instance->CR, QUADSPI_CR_FTHRES,    /* 列表续行 */
             ((hqspi->Init.FifoThreshold - 1U) << QUADSPI_CR_FTHRES_Pos));  /* 执行语句 */

  /* Wait till BUSY flag reset */
  status = QSPI_WaitFlagStateUntilTimeout(hqspi, QSPI_FLAG_BUSY, RESET, tickstart, hqspi->Timeout);  /* 赋值或初始化 */

  if(status == HAL_OK)                                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Configure QSPI Clock Prescaler and Sample Shift */
    MODIFY_REG(hqspi->Instance->CR, (QUADSPI_CR_PRESCALER | QUADSPI_CR_SSHIFT | QUADSPI_CR_FSEL | QUADSPI_CR_DFM),  /* 列表续行 */
               ((hqspi->Init.ClockPrescaler << QUADSPI_CR_PRESCALER_Pos) |  /* 源代码 */
                hqspi->Init.SampleShifting  | hqspi->Init.FlashID | hqspi->Init.DualFlash));  /* 执行语句 */

    /* Configure QSPI Flash Size, CS High Time and Clock Mode */
    MODIFY_REG(hqspi->Instance->DCR, (QUADSPI_DCR_FSIZE | QUADSPI_DCR_CSHT | QUADSPI_DCR_CKMODE),  /* 列表续行 */
               ((hqspi->Init.FlashSize << QUADSPI_DCR_FSIZE_Pos) |  /* 源代码 */
                hqspi->Init.ChipSelectHighTime | hqspi->Init.ClockMode));  /* 执行语句 */

    /* Enable the QSPI peripheral */
    __HAL_QSPI_ENABLE(hqspi);                           /* HAL辅助宏/函数 */

    /* Set QSPI error code to none */
    hqspi->ErrorCode = HAL_QSPI_ERROR_NONE;             /* 赋值或初始化 */

    /* Initialize the QSPI state */
    hqspi->State = HAL_QSPI_STATE_READY;                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hqspi);                                  /* HAL辅助宏/函数 */

  /* Return function status */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief De-Initialize the QSPI peripheral.
  * @param hqspi : QSPI handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_QSPI_DeInit(QSPI_HandleTypeDef *hqspi)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the QSPI handle allocation */
  if(hqspi == NULL)                                     /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Disable the QSPI Peripheral Clock */
  __HAL_QSPI_DISABLE(hqspi);                            /* HAL辅助宏/函数 */

#if (USE_HAL_QSPI_REGISTER_CALLBACKS == 1)              /* 条件编译 */
  if(hqspi->MspDeInitCallback == NULL)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hqspi->MspDeInitCallback = HAL_QSPI_MspDeInit;      /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* DeInit the low level hardware */
  hqspi->MspDeInitCallback(hqspi);                      /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* DeInit the low level hardware: GPIO, CLOCK, NVIC... */
  HAL_QSPI_MspDeInit(hqspi);                            /* HAL库函数调用 */
#endif                                                  /* 条件编译结束 */

  /* Set QSPI error code to none */
  hqspi->ErrorCode = HAL_QSPI_ERROR_NONE;               /* 赋值或初始化 */

  /* Initialize the QSPI state */
  hqspi->State = HAL_QSPI_STATE_RESET;                  /* 赋值或初始化 */

  /* Release Lock */
  __HAL_UNLOCK(hqspi);                                  /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief Initialize the QSPI MSP.
  * @param hqspi : QSPI handle
  * @retval None
  */
__weak void HAL_QSPI_MspInit(QSPI_HandleTypeDef *hqspi)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hqspi);                                        /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_QSPI_MspInit can be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief DeInitialize the QSPI MSP.
  * @param hqspi : QSPI handle
  * @retval None
  */
__weak void HAL_QSPI_MspDeInit(QSPI_HandleTypeDef *hqspi)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hqspi);                                        /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_QSPI_MspDeInit can be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup QSPI_Exported_Functions_Group2 Input and Output operation functions
  *  @brief QSPI Transmit/Receive functions
  *
@verbatim
 ===============================================================================
                      ##### IO operation functions #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to :
      (+) Handle the interrupts.
      (+) Handle the command sequence.
      (+) Transmit data in blocking, interrupt or DMA mode.
      (+) Receive data in blocking, interrupt or DMA mode.
      (+) Manage the auto-polling functional mode.
      (+) Manage the memory-mapped functional mode.

@endverbatim
  * @{
  */

/**
  * @brief Handle QSPI interrupt request.
  * @param hqspi : QSPI handle
  * @retval None
  */
void HAL_QSPI_IRQHandler(QSPI_HandleTypeDef *hqspi)     /* 中断服务函数 */
{                                                       /* 代码块开始 */
  __IO uint32_t *data_reg;                              /* 执行语句 */
  uint32_t flag = READ_REG(hqspi->Instance->SR);        /* 32位无符号变量/函数 */
  uint32_t itsource = READ_REG(hqspi->Instance->CR);    /* 32位无符号变量/函数 */

  /* QSPI Fifo Threshold interrupt occurred ----------------------------------*/
  if(((flag & QSPI_FLAG_FT) != 0U) && ((itsource & QSPI_IT_FT) != 0U))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    data_reg = &hqspi->Instance->DR;                    /* 赋值或初始化 */

    if(hqspi->State == HAL_QSPI_STATE_BUSY_INDIRECT_TX)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Transmission process */
      while(__HAL_QSPI_GET_FLAG(hqspi, QSPI_FLAG_FT) != RESET)  /* while循环 */
      {                                                 /* 代码块开始 */
        if (hqspi->TxXferCount > 0U)                    /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Fill the FIFO until the threshold is reached */
          *((__IO uint8_t *)data_reg) = *hqspi->pTxBuffPtr;  /* 赋值或初始化 */
          hqspi->pTxBuffPtr++;                          /* 执行语句 */
          hqspi->TxXferCount--;                         /* 执行语句 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          /* No more data available for the transfer */
          /* Disable the QSPI FIFO Threshold Interrupt */
          __HAL_QSPI_DISABLE_IT(hqspi, QSPI_IT_FT);     /* HAL辅助宏/函数 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else if(hqspi->State == HAL_QSPI_STATE_BUSY_INDIRECT_RX)  /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* Receiving Process */
      while(__HAL_QSPI_GET_FLAG(hqspi, QSPI_FLAG_FT) != RESET)  /* while循环 */
      {                                                 /* 代码块开始 */
        if (hqspi->RxXferCount > 0U)                    /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Read the FIFO until the threshold is reached */
          *hqspi->pRxBuffPtr = *((__IO uint8_t *)data_reg);  /* 赋值或初始化 */
          hqspi->pRxBuffPtr++;                          /* 执行语句 */
          hqspi->RxXferCount--;                         /* 执行语句 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          /* All data have been received for the transfer */
          /* Disable the QSPI FIFO Threshold Interrupt */
          __HAL_QSPI_DISABLE_IT(hqspi, QSPI_IT_FT);     /* HAL辅助宏/函数 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Nothing to do */
    }                                                   /* 代码块结束 */

    /* FIFO Threshold callback */
#if (USE_HAL_QSPI_REGISTER_CALLBACKS == 1)              /* 条件编译 */
    hqspi->FifoThresholdCallback(hqspi);                /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_QSPI_FifoThresholdCallback(hqspi);              /* HAL库函数调用 */
#endif                                                  /* 条件编译结束 */
  }                                                     /* 代码块结束 */

  /* QSPI Transfer Complete interrupt occurred -------------------------------*/
  else if(((flag & QSPI_FLAG_TC) != 0U) && ((itsource & QSPI_IT_TC) != 0U))  /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* Clear interrupt */
    WRITE_REG(hqspi->Instance->FCR, QSPI_FLAG_TC);      /* 执行语句 */

    /* Disable the QSPI FIFO Threshold, Transfer Error and Transfer complete Interrupts */
    __HAL_QSPI_DISABLE_IT(hqspi, QSPI_IT_TC | QSPI_IT_TE | QSPI_IT_FT);  /* HAL辅助宏/函数 */

    /* Transfer complete callback */
    if(hqspi->State == HAL_QSPI_STATE_BUSY_INDIRECT_TX)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      if ((hqspi->Instance->CR & QUADSPI_CR_DMAEN) != 0U)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Disable the DMA transfer by clearing the DMAEN bit in the QSPI CR register */
        CLEAR_BIT(hqspi->Instance->CR, QUADSPI_CR_DMAEN);  /* 执行语句 */

        /* Disable the DMA channel */
        __HAL_DMA_DISABLE(hqspi->hdma);                 /* HAL辅助宏/函数 */
      }                                                 /* 代码块结束 */

      /* Clear Busy bit */
      HAL_QSPI_Abort_IT(hqspi);                         /* HAL库函数调用 */

      /* Change state of QSPI */
      hqspi->State = HAL_QSPI_STATE_READY;              /* 赋值或初始化 */

      /* TX Complete callback */
#if (USE_HAL_QSPI_REGISTER_CALLBACKS == 1)              /* 条件编译 */
      hqspi->TxCpltCallback(hqspi);                     /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_QSPI_TxCpltCallback(hqspi);                   /* HAL库函数调用 */
#endif                                                  /* 条件编译结束 */
    }                                                   /* 代码块结束 */
    else if(hqspi->State == HAL_QSPI_STATE_BUSY_INDIRECT_RX)  /* 否则如果 */
    {                                                   /* 代码块开始 */
      if ((hqspi->Instance->CR & QUADSPI_CR_DMAEN) != 0U)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Disable the DMA transfer by clearing the DMAEN bit in the QSPI CR register */
        CLEAR_BIT(hqspi->Instance->CR, QUADSPI_CR_DMAEN);  /* 执行语句 */

        /* Disable the DMA channel */
        __HAL_DMA_DISABLE(hqspi->hdma);                 /* HAL辅助宏/函数 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        data_reg = &hqspi->Instance->DR;                /* 赋值或初始化 */
        while(READ_BIT(hqspi->Instance->SR, QUADSPI_SR_FLEVEL) != 0U)  /* while循环 */
        {                                               /* 代码块开始 */
          if (hqspi->RxXferCount > 0U)                  /* 条件判断 */
          {                                             /* 代码块开始 */
            /* Read the last data received in the FIFO until it is empty */
            *hqspi->pRxBuffPtr = *((__IO uint8_t *)data_reg);  /* 赋值或初始化 */
            hqspi->pRxBuffPtr++;                        /* 执行语句 */
            hqspi->RxXferCount--;                       /* 执行语句 */
          }                                             /* 代码块结束 */
          else                                          /* 否则 */
          {                                             /* 代码块开始 */
            /* All data have been received for the transfer */
            break;                                      /* 跳出 */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */

      /* Workaround - Extra data written in the FIFO at the end of a read transfer */
      HAL_QSPI_Abort_IT(hqspi);                         /* HAL库函数调用 */

      /* Change state of QSPI */
      hqspi->State = HAL_QSPI_STATE_READY;              /* 赋值或初始化 */

      /* RX Complete callback */
#if (USE_HAL_QSPI_REGISTER_CALLBACKS == 1)              /* 条件编译 */
      hqspi->RxCpltCallback(hqspi);                     /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_QSPI_RxCpltCallback(hqspi);                   /* HAL库函数调用 */
#endif                                                  /* 条件编译结束 */
    }                                                   /* 代码块结束 */
    else if(hqspi->State == HAL_QSPI_STATE_BUSY)        /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* Change state of QSPI */
      hqspi->State = HAL_QSPI_STATE_READY;              /* 赋值或初始化 */

      /* Command Complete callback */
#if (USE_HAL_QSPI_REGISTER_CALLBACKS == 1)              /* 条件编译 */
      hqspi->CmdCpltCallback(hqspi);                    /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_QSPI_CmdCpltCallback(hqspi);                  /* HAL库函数调用 */
#endif                                                  /* 条件编译结束 */
    }                                                   /* 代码块结束 */
    else if(hqspi->State == HAL_QSPI_STATE_ABORT)       /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* Reset functional mode configuration to indirect write mode by default */
      CLEAR_BIT(hqspi->Instance->CCR, QUADSPI_CCR_FMODE);  /* 执行语句 */

      /* Change state of QSPI */
      hqspi->State = HAL_QSPI_STATE_READY;              /* 赋值或初始化 */

      if (hqspi->ErrorCode == HAL_QSPI_ERROR_NONE)      /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Abort called by the user */

        /* Abort Complete callback */
#if (USE_HAL_QSPI_REGISTER_CALLBACKS == 1)              /* 条件编译 */
        hqspi->AbortCpltCallback(hqspi);                /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_QSPI_AbortCpltCallback(hqspi);              /* HAL库函数调用 */
#endif                                                  /* 条件编译结束 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Abort due to an error (eg :  DMA error) */

        /* Error callback */
#if (USE_HAL_QSPI_REGISTER_CALLBACKS == 1)              /* 条件编译 */
        hqspi->ErrorCallback(hqspi);                    /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_QSPI_ErrorCallback(hqspi);                  /* HAL库函数调用 */
#endif                                                  /* 条件编译结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
     /* Nothing to do */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* QSPI Status Match interrupt occurred ------------------------------------*/
  else if(((flag & QSPI_FLAG_SM) != 0U) && ((itsource & QSPI_IT_SM) != 0U))  /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* Clear interrupt */
    WRITE_REG(hqspi->Instance->FCR, QSPI_FLAG_SM);      /* 执行语句 */

    /* Check if the automatic poll mode stop is activated */
    if(READ_BIT(hqspi->Instance->CR, QUADSPI_CR_APMS) != 0U)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable the QSPI Transfer Error and Status Match Interrupts */
      __HAL_QSPI_DISABLE_IT(hqspi, (QSPI_IT_SM | QSPI_IT_TE));  /* HAL辅助宏/函数 */

      /* Change state of QSPI */
      hqspi->State = HAL_QSPI_STATE_READY;              /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Status match callback */
#if (USE_HAL_QSPI_REGISTER_CALLBACKS == 1)              /* 条件编译 */
    hqspi->StatusMatchCallback(hqspi);                  /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_QSPI_StatusMatchCallback(hqspi);                /* HAL库函数调用 */
#endif                                                  /* 条件编译结束 */
  }                                                     /* 代码块结束 */

  /* QSPI Transfer Error interrupt occurred ----------------------------------*/
  else if(((flag & QSPI_FLAG_TE) != 0U) && ((itsource & QSPI_IT_TE) != 0U))  /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* Clear interrupt */
    WRITE_REG(hqspi->Instance->FCR, QSPI_FLAG_TE);      /* 执行语句 */

    /* Disable all the QSPI Interrupts */
    __HAL_QSPI_DISABLE_IT(hqspi, QSPI_IT_SM | QSPI_IT_TC | QSPI_IT_TE | QSPI_IT_FT);  /* HAL辅助宏/函数 */

    /* Set error code */
    hqspi->ErrorCode |= HAL_QSPI_ERROR_TRANSFER;        /* 赋值或初始化 */

    if ((hqspi->Instance->CR & QUADSPI_CR_DMAEN) != 0U)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable the DMA transfer by clearing the DMAEN bit in the QSPI CR register */
      CLEAR_BIT(hqspi->Instance->CR, QUADSPI_CR_DMAEN);  /* 执行语句 */

      /* Disable the DMA channel */
      hqspi->hdma->XferAbortCallback = QSPI_DMAAbortCplt;  /* 赋值或初始化 */
      if (HAL_DMA_Abort_IT(hqspi->hdma) != HAL_OK)      /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Set error code to DMA */
        hqspi->ErrorCode |= HAL_QSPI_ERROR_DMA;         /* 赋值或初始化 */

        /* Change state of QSPI */
        hqspi->State = HAL_QSPI_STATE_READY;            /* 赋值或初始化 */

        /* Error callback */
#if (USE_HAL_QSPI_REGISTER_CALLBACKS == 1)              /* 条件编译 */
        hqspi->ErrorCallback(hqspi);                    /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_QSPI_ErrorCallback(hqspi);                  /* HAL库函数调用 */
#endif                                                  /* 条件编译结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Change state of QSPI */
      hqspi->State = HAL_QSPI_STATE_READY;              /* 赋值或初始化 */

      /* Error callback */
#if (USE_HAL_QSPI_REGISTER_CALLBACKS == 1)              /* 条件编译 */
      hqspi->ErrorCallback(hqspi);                      /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_QSPI_ErrorCallback(hqspi);                    /* HAL库函数调用 */
#endif                                                  /* 条件编译结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* QSPI Timeout interrupt occurred -----------------------------------------*/
  else if(((flag & QSPI_FLAG_TO) != 0U) && ((itsource & QSPI_IT_TO) != 0U))  /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* Clear interrupt */
    WRITE_REG(hqspi->Instance->FCR, QSPI_FLAG_TO);      /* 执行语句 */

    /* Timeout callback */
#if (USE_HAL_QSPI_REGISTER_CALLBACKS == 1)              /* 条件编译 */
    hqspi->TimeOutCallback(hqspi);                      /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_QSPI_TimeOutCallback(hqspi);                    /* HAL库函数调用 */
#endif                                                  /* 条件编译结束 */
  }                                                     /* 代码块结束 */

   else                                                 /* 否则 */
  {                                                     /* 代码块开始 */
   /* Nothing to do */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief Set the command configuration.
  * @param hqspi : QSPI handle
  * @param cmd : structure that contains the command configuration information
  * @param Timeout : Timeout duration
  * @note   This function is used only in Indirect Read or Write Modes
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_QSPI_Command(QSPI_HandleTypeDef *hqspi, QSPI_CommandTypeDef *cmd, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status;                             /* HAL状态类型函数 */
  uint32_t tickstart = HAL_GetTick();                   /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_QSPI_INSTRUCTION_MODE(cmd->InstructionMode));  /* 执行语句 */
  if (cmd->InstructionMode != QSPI_INSTRUCTION_NONE)    /* 条件判断 */
  {                                                     /* 代码块开始 */
    assert_param(IS_QSPI_INSTRUCTION(cmd->Instruction));  /* 执行语句 */
  }                                                     /* 代码块结束 */

  assert_param(IS_QSPI_ADDRESS_MODE(cmd->AddressMode));  /* 执行语句 */
  if (cmd->AddressMode != QSPI_ADDRESS_NONE)            /* 条件判断 */
  {                                                     /* 代码块开始 */
    assert_param(IS_QSPI_ADDRESS_SIZE(cmd->AddressSize));  /* 执行语句 */
  }                                                     /* 代码块结束 */

  assert_param(IS_QSPI_ALTERNATE_BYTES_MODE(cmd->AlternateByteMode));  /* 执行语句 */
  if (cmd->AlternateByteMode != QSPI_ALTERNATE_BYTES_NONE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    assert_param(IS_QSPI_ALTERNATE_BYTES_SIZE(cmd->AlternateBytesSize));  /* 执行语句 */
  }                                                     /* 代码块结束 */

  assert_param(IS_QSPI_DUMMY_CYCLES(cmd->DummyCycles));  /* 执行语句 */
  assert_param(IS_QSPI_DATA_MODE(cmd->DataMode));       /* 执行语句 */

  assert_param(IS_QSPI_DDR_MODE(cmd->DdrMode));         /* 执行语句 */
  assert_param(IS_QSPI_DDR_HHC(cmd->DdrHoldHalfCycle));  /* 执行语句 */
  assert_param(IS_QSPI_SIOO_MODE(cmd->SIOOMode));       /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hqspi);                                    /* HAL辅助宏/函数 */

  if(hqspi->State == HAL_QSPI_STATE_READY)              /* 条件判断 */
  {                                                     /* 代码块开始 */
    hqspi->ErrorCode = HAL_QSPI_ERROR_NONE;             /* 赋值或初始化 */

    /* Update QSPI state */
    hqspi->State = HAL_QSPI_STATE_BUSY;                 /* 赋值或初始化 */

    /* Wait till BUSY flag reset */
    status = QSPI_WaitFlagStateUntilTimeout(hqspi, QSPI_FLAG_BUSY, RESET, tickstart, Timeout);  /* 赋值或初始化 */

    if (status == HAL_OK)                               /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Call the configuration function */
      QSPI_Config(hqspi, cmd, QSPI_FUNCTIONAL_MODE_INDIRECT_WRITE);  /* 执行语句 */

      if (cmd->DataMode == QSPI_DATA_NONE)              /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* When there is no data phase, the transfer start as soon as the configuration is done
        so wait until TC flag is set to go back in idle state */
        status = QSPI_WaitFlagStateUntilTimeout(hqspi, QSPI_FLAG_TC, SET, tickstart, Timeout);  /* 赋值或初始化 */

        if (status == HAL_OK)                           /* 条件判断 */
        {                                               /* 代码块开始 */
          __HAL_QSPI_CLEAR_FLAG(hqspi, QSPI_FLAG_TC);   /* HAL辅助宏/函数 */

          /* Update QSPI state */
          hqspi->State = HAL_QSPI_STATE_READY;          /* 赋值或初始化 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Update QSPI state */
        hqspi->State = HAL_QSPI_STATE_READY;            /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    status = HAL_BUSY;                                  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Process unlocked */
  __HAL_UNLOCK(hqspi);                                  /* HAL辅助宏/函数 */

  /* Return function status */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief Set the command configuration in interrupt mode.
  * @param hqspi : QSPI handle
  * @param cmd : structure that contains the command configuration information
  * @note   This function is used only in Indirect Read or Write Modes
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_QSPI_Command_IT(QSPI_HandleTypeDef *hqspi, QSPI_CommandTypeDef *cmd)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status;                             /* HAL状态类型函数 */
  uint32_t tickstart = HAL_GetTick();                   /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_QSPI_INSTRUCTION_MODE(cmd->InstructionMode));  /* 执行语句 */
  if (cmd->InstructionMode != QSPI_INSTRUCTION_NONE)    /* 条件判断 */
  {                                                     /* 代码块开始 */
    assert_param(IS_QSPI_INSTRUCTION(cmd->Instruction));  /* 执行语句 */
  }                                                     /* 代码块结束 */

  assert_param(IS_QSPI_ADDRESS_MODE(cmd->AddressMode));  /* 执行语句 */
  if (cmd->AddressMode != QSPI_ADDRESS_NONE)            /* 条件判断 */
  {                                                     /* 代码块开始 */
    assert_param(IS_QSPI_ADDRESS_SIZE(cmd->AddressSize));  /* 执行语句 */
  }                                                     /* 代码块结束 */

  assert_param(IS_QSPI_ALTERNATE_BYTES_MODE(cmd->AlternateByteMode));  /* 执行语句 */
  if (cmd->AlternateByteMode != QSPI_ALTERNATE_BYTES_NONE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    assert_param(IS_QSPI_ALTERNATE_BYTES_SIZE(cmd->AlternateBytesSize));  /* 执行语句 */
  }                                                     /* 代码块结束 */

  assert_param(IS_QSPI_DUMMY_CYCLES(cmd->DummyCycles));  /* 执行语句 */
  assert_param(IS_QSPI_DATA_MODE(cmd->DataMode));       /* 执行语句 */

  assert_param(IS_QSPI_DDR_MODE(cmd->DdrMode));         /* 执行语句 */
  assert_param(IS_QSPI_DDR_HHC(cmd->DdrHoldHalfCycle));  /* 执行语句 */
  assert_param(IS_QSPI_SIOO_MODE(cmd->SIOOMode));       /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hqspi);                                    /* HAL辅助宏/函数 */

  if(hqspi->State == HAL_QSPI_STATE_READY)              /* 条件判断 */
  {                                                     /* 代码块开始 */
    hqspi->ErrorCode = HAL_QSPI_ERROR_NONE;             /* 赋值或初始化 */

    /* Update QSPI state */
    hqspi->State = HAL_QSPI_STATE_BUSY;                 /* 赋值或初始化 */

    /* Wait till BUSY flag reset */
    status = QSPI_WaitFlagStateUntilTimeout(hqspi, QSPI_FLAG_BUSY, RESET, tickstart, hqspi->Timeout);  /* 赋值或初始化 */

    if (status == HAL_OK)                               /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (cmd->DataMode == QSPI_DATA_NONE)              /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Clear interrupt */
        __HAL_QSPI_CLEAR_FLAG(hqspi, QSPI_FLAG_TE | QSPI_FLAG_TC);  /* HAL辅助宏/函数 */
      }                                                 /* 代码块结束 */

      /* Call the configuration function */
      QSPI_Config(hqspi, cmd, QSPI_FUNCTIONAL_MODE_INDIRECT_WRITE);  /* 执行语句 */

      if (cmd->DataMode == QSPI_DATA_NONE)              /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* When there is no data phase, the transfer start as soon as the configuration is done
        so activate TC and TE interrupts */
        /* Process unlocked */
        __HAL_UNLOCK(hqspi);                            /* HAL辅助宏/函数 */

        /* Enable the QSPI Transfer Error Interrupt */
        __HAL_QSPI_ENABLE_IT(hqspi, QSPI_IT_TE | QSPI_IT_TC);  /* HAL辅助宏/函数 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Update QSPI state */
        hqspi->State = HAL_QSPI_STATE_READY;            /* 赋值或初始化 */

        /* Process unlocked */
        __HAL_UNLOCK(hqspi);                            /* HAL辅助宏/函数 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Process unlocked */
      __HAL_UNLOCK(hqspi);                              /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    status = HAL_BUSY;                                  /* 赋值或初始化 */

    /* Process unlocked */
    __HAL_UNLOCK(hqspi);                                /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Return function status */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief Transmit an amount of data in blocking mode.
  * @param hqspi : QSPI handle
  * @param pData : pointer to data buffer
  * @param Timeout : Timeout duration
  * @note   This function is used only in Indirect Write Mode
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_QSPI_Transmit(QSPI_HandleTypeDef *hqspi, uint8_t *pData, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */
  uint32_t tickstart = HAL_GetTick();                   /* 32位无符号变量/函数 */
  __IO uint32_t *data_reg = &hqspi->Instance->DR;       /* 赋值或初始化 */

  /* Process locked */
  __HAL_LOCK(hqspi);                                    /* HAL辅助宏/函数 */

  if(hqspi->State == HAL_QSPI_STATE_READY)              /* 条件判断 */
  {                                                     /* 代码块开始 */
    hqspi->ErrorCode = HAL_QSPI_ERROR_NONE;             /* 赋值或初始化 */

    if(pData != NULL )                                  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Update state */
      hqspi->State = HAL_QSPI_STATE_BUSY_INDIRECT_TX;   /* 赋值或初始化 */

      /* Configure counters and size of the handle */
      hqspi->TxXferCount = READ_REG(hqspi->Instance->DLR) + 1U;  /* 赋值或初始化 */
      hqspi->TxXferSize = READ_REG(hqspi->Instance->DLR) + 1U;  /* 赋值或初始化 */
      hqspi->pTxBuffPtr = pData;                        /* 赋值或初始化 */

      /* Configure QSPI: CCR register with functional as indirect write */
      MODIFY_REG(hqspi->Instance->CCR, QUADSPI_CCR_FMODE, QSPI_FUNCTIONAL_MODE_INDIRECT_WRITE);  /* 执行语句 */

      while(hqspi->TxXferCount > 0U)                    /* while循环 */
      {                                                 /* 代码块开始 */
        /* Wait until FT flag is set to send data */
        status = QSPI_WaitFlagStateUntilTimeout(hqspi, QSPI_FLAG_FT, SET, tickstart, Timeout);  /* 赋值或初始化 */

        if (status != HAL_OK)                           /* 条件判断 */
        {                                               /* 代码块开始 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */

        *((__IO uint8_t *)data_reg) = *hqspi->pTxBuffPtr;  /* 赋值或初始化 */
        hqspi->pTxBuffPtr++;                            /* 执行语句 */
        hqspi->TxXferCount--;                           /* 执行语句 */
      }                                                 /* 代码块结束 */

      if (status == HAL_OK)                             /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Wait until TC flag is set to go back in idle state */
        status = QSPI_WaitFlagStateUntilTimeout(hqspi, QSPI_FLAG_TC, SET, tickstart, Timeout);  /* 赋值或初始化 */

        if (status == HAL_OK)                           /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Clear Transfer Complete bit */
          __HAL_QSPI_CLEAR_FLAG(hqspi, QSPI_FLAG_TC);   /* HAL辅助宏/函数 */

          /* Clear Busy bit */
          status = HAL_QSPI_Abort(hqspi);               /* 赋值或初始化 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */

      /* Update QSPI state */
      hqspi->State = HAL_QSPI_STATE_READY;              /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hqspi->ErrorCode |= HAL_QSPI_ERROR_INVALID_PARAM;  /* 赋值或初始化 */
      status = HAL_ERROR;                               /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    status = HAL_BUSY;                                  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Process unlocked */
  __HAL_UNLOCK(hqspi);                                  /* HAL辅助宏/函数 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */


/**
  * @brief Receive an amount of data in blocking mode.
  * @param hqspi : QSPI handle
  * @param pData : pointer to data buffer
  * @param Timeout : Timeout duration
  * @note   This function is used only in Indirect Read Mode
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_QSPI_Receive(QSPI_HandleTypeDef *hqspi, uint8_t *pData, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */
  uint32_t tickstart = HAL_GetTick();                   /* 32位无符号变量/函数 */
  uint32_t addr_reg = READ_REG(hqspi->Instance->AR);    /* 32位无符号变量/函数 */
  __IO uint32_t *data_reg = &hqspi->Instance->DR;       /* 赋值或初始化 */

  /* Process locked */
  __HAL_LOCK(hqspi);                                    /* HAL辅助宏/函数 */

  if(hqspi->State == HAL_QSPI_STATE_READY)              /* 条件判断 */
  {                                                     /* 代码块开始 */
    hqspi->ErrorCode = HAL_QSPI_ERROR_NONE;             /* 赋值或初始化 */

    if(pData != NULL )                                  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Update state */
      hqspi->State = HAL_QSPI_STATE_BUSY_INDIRECT_RX;   /* 赋值或初始化 */

      /* Configure counters and size of the handle */
      hqspi->RxXferCount = READ_REG(hqspi->Instance->DLR) + 1U;  /* 赋值或初始化 */
      hqspi->RxXferSize = READ_REG(hqspi->Instance->DLR) + 1U;  /* 赋值或初始化 */
      hqspi->pRxBuffPtr = pData;                        /* 赋值或初始化 */

      /* Configure QSPI: CCR register with functional as indirect read */
      MODIFY_REG(hqspi->Instance->CCR, QUADSPI_CCR_FMODE, QSPI_FUNCTIONAL_MODE_INDIRECT_READ);  /* 执行语句 */

      /* Start the transfer by re-writing the address in AR register */
      WRITE_REG(hqspi->Instance->AR, addr_reg);         /* 执行语句 */

      while(hqspi->RxXferCount > 0U)                    /* while循环 */
      {                                                 /* 代码块开始 */
        /* Wait until FT or TC flag is set to read received data */
        status = QSPI_WaitFlagStateUntilTimeout(hqspi, (QSPI_FLAG_FT | QSPI_FLAG_TC), SET, tickstart, Timeout);  /* 赋值或初始化 */

        if  (status != HAL_OK)                          /* 条件判断 */
        {                                               /* 代码块开始 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */

        *hqspi->pRxBuffPtr = *((__IO uint8_t *)data_reg);  /* 赋值或初始化 */
        hqspi->pRxBuffPtr++;                            /* 执行语句 */
        hqspi->RxXferCount--;                           /* 执行语句 */
      }                                                 /* 代码块结束 */

      if (status == HAL_OK)                             /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Wait until TC flag is set to go back in idle state */
        status = QSPI_WaitFlagStateUntilTimeout(hqspi, QSPI_FLAG_TC, SET, tickstart, Timeout);  /* 赋值或初始化 */

        if  (status == HAL_OK)                          /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Clear Transfer Complete bit */
          __HAL_QSPI_CLEAR_FLAG(hqspi, QSPI_FLAG_TC);   /* HAL辅助宏/函数 */

          /* Workaround - Extra data written in the FIFO at the end of a read transfer */
          status = HAL_QSPI_Abort(hqspi);               /* 赋值或初始化 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */

      /* Update QSPI state */
      hqspi->State = HAL_QSPI_STATE_READY;              /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hqspi->ErrorCode |= HAL_QSPI_ERROR_INVALID_PARAM;  /* 赋值或初始化 */
      status = HAL_ERROR;                               /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    status = HAL_BUSY;                                  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Process unlocked */
  __HAL_UNLOCK(hqspi);                                  /* HAL辅助宏/函数 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Send an amount of data in non-blocking mode with interrupt.
  * @param  hqspi : QSPI handle
  * @param  pData : pointer to data buffer
  * @note   This function is used only in Indirect Write Mode
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_QSPI_Transmit_IT(QSPI_HandleTypeDef *hqspi, uint8_t *pData)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Process locked */
  __HAL_LOCK(hqspi);                                    /* HAL辅助宏/函数 */

  if(hqspi->State == HAL_QSPI_STATE_READY)              /* 条件判断 */
  {                                                     /* 代码块开始 */
    hqspi->ErrorCode = HAL_QSPI_ERROR_NONE;             /* 赋值或初始化 */

    if(pData != NULL )                                  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Update state */
      hqspi->State = HAL_QSPI_STATE_BUSY_INDIRECT_TX;   /* 赋值或初始化 */

      /* Configure counters and size of the handle */
      hqspi->TxXferCount = READ_REG(hqspi->Instance->DLR) + 1U;  /* 赋值或初始化 */
      hqspi->TxXferSize = READ_REG(hqspi->Instance->DLR) + 1U;  /* 赋值或初始化 */
      hqspi->pTxBuffPtr = pData;                        /* 赋值或初始化 */

      /* Clear interrupt */
      __HAL_QSPI_CLEAR_FLAG(hqspi, QSPI_FLAG_TE | QSPI_FLAG_TC);  /* HAL辅助宏/函数 */

      /* Configure QSPI: CCR register with functional as indirect write */
      MODIFY_REG(hqspi->Instance->CCR, QUADSPI_CCR_FMODE, QSPI_FUNCTIONAL_MODE_INDIRECT_WRITE);  /* 执行语句 */

      /* Process unlocked */
      __HAL_UNLOCK(hqspi);                              /* HAL辅助宏/函数 */

      /* Enable the QSPI transfer error, FIFO threshold and transfer complete Interrupts */
      __HAL_QSPI_ENABLE_IT(hqspi, QSPI_IT_TE | QSPI_IT_FT | QSPI_IT_TC);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hqspi->ErrorCode |= HAL_QSPI_ERROR_INVALID_PARAM;  /* 赋值或初始化 */
      status = HAL_ERROR;                               /* 赋值或初始化 */

      /* Process unlocked */
      __HAL_UNLOCK(hqspi);                              /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    status = HAL_BUSY;                                  /* 赋值或初始化 */

    /* Process unlocked */
    __HAL_UNLOCK(hqspi);                                /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Receive an amount of data in non-blocking mode with interrupt.
  * @param  hqspi : QSPI handle
  * @param  pData : pointer to data buffer
  * @note   This function is used only in Indirect Read Mode
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_QSPI_Receive_IT(QSPI_HandleTypeDef *hqspi, uint8_t *pData)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */
  uint32_t addr_reg = READ_REG(hqspi->Instance->AR);    /* 32位无符号变量/函数 */

  /* Process locked */
  __HAL_LOCK(hqspi);                                    /* HAL辅助宏/函数 */

  if(hqspi->State == HAL_QSPI_STATE_READY)              /* 条件判断 */
  {                                                     /* 代码块开始 */
    hqspi->ErrorCode = HAL_QSPI_ERROR_NONE;             /* 赋值或初始化 */

    if(pData != NULL )                                  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Update state */
      hqspi->State = HAL_QSPI_STATE_BUSY_INDIRECT_RX;   /* 赋值或初始化 */

      /* Configure counters and size of the handle */
      hqspi->RxXferCount = READ_REG(hqspi->Instance->DLR) + 1U;  /* 赋值或初始化 */
      hqspi->RxXferSize = READ_REG(hqspi->Instance->DLR) + 1U;  /* 赋值或初始化 */
      hqspi->pRxBuffPtr = pData;                        /* 赋值或初始化 */

      /* Clear interrupt */
      __HAL_QSPI_CLEAR_FLAG(hqspi, QSPI_FLAG_TE | QSPI_FLAG_TC);  /* HAL辅助宏/函数 */

      /* Configure QSPI: CCR register with functional as indirect read */
      MODIFY_REG(hqspi->Instance->CCR, QUADSPI_CCR_FMODE, QSPI_FUNCTIONAL_MODE_INDIRECT_READ);  /* 执行语句 */

      /* Start the transfer by re-writing the address in AR register */
      WRITE_REG(hqspi->Instance->AR, addr_reg);         /* 执行语句 */

      /* Process unlocked */
      __HAL_UNLOCK(hqspi);                              /* HAL辅助宏/函数 */

      /* Enable the QSPI transfer error, FIFO threshold and transfer complete Interrupts */
      __HAL_QSPI_ENABLE_IT(hqspi, QSPI_IT_TE | QSPI_IT_FT | QSPI_IT_TC);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hqspi->ErrorCode |= HAL_QSPI_ERROR_INVALID_PARAM;  /* 赋值或初始化 */
      status = HAL_ERROR;                               /* 赋值或初始化 */

      /* Process unlocked */
      __HAL_UNLOCK(hqspi);                              /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    status = HAL_BUSY;                                  /* 赋值或初始化 */

    /* Process unlocked */
    __HAL_UNLOCK(hqspi);                                /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Send an amount of data in non-blocking mode with DMA.
  * @param  hqspi : QSPI handle
  * @param  pData : pointer to data buffer
  * @note   This function is used only in Indirect Write Mode
  * @note   If DMA peripheral access is configured as halfword, the number
  *         of data and the fifo threshold should be aligned on halfword
  * @note   If DMA peripheral access is configured as word, the number
  *         of data and the fifo threshold should be aligned on word
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_QSPI_Transmit_DMA(QSPI_HandleTypeDef *hqspi, uint8_t *pData)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */
  uint32_t data_size = (READ_REG(hqspi->Instance->DLR) + 1U);  /* 32位无符号变量/函数 */

  /* Process locked */
  __HAL_LOCK(hqspi);                                    /* HAL辅助宏/函数 */

  if(hqspi->State == HAL_QSPI_STATE_READY)              /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Clear the error code */
    hqspi->ErrorCode = HAL_QSPI_ERROR_NONE;             /* 赋值或初始化 */

    if(pData != NULL )                                  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Configure counters of the handle */
      if (hqspi->hdma->Init.PeriphDataAlignment == DMA_PDATAALIGN_BYTE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        hqspi->TxXferCount = data_size;                 /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else if (hqspi->hdma->Init.PeriphDataAlignment == DMA_PDATAALIGN_HALFWORD)  /* 否则如果 */
      {                                                 /* 代码块开始 */
        if (((data_size % 2U) != 0U) || ((hqspi->Init.FifoThreshold % 2U) != 0U))  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* The number of data or the fifo threshold is not aligned on halfword
          => no transfer possible with DMA peripheral access configured as halfword */
          hqspi->ErrorCode |= HAL_QSPI_ERROR_INVALID_PARAM;  /* 赋值或初始化 */
          status = HAL_ERROR;                           /* 赋值或初始化 */

          /* Process unlocked */
          __HAL_UNLOCK(hqspi);                          /* HAL辅助宏/函数 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          hqspi->TxXferCount = (data_size >> 1U);       /* 赋值或初始化 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else if (hqspi->hdma->Init.PeriphDataAlignment == DMA_PDATAALIGN_WORD)  /* 否则如果 */
      {                                                 /* 代码块开始 */
        if (((data_size % 4U) != 0U) || ((hqspi->Init.FifoThreshold % 4U) != 0U))  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* The number of data or the fifo threshold is not aligned on word
          => no transfer possible with DMA peripheral access configured as word */
          hqspi->ErrorCode |= HAL_QSPI_ERROR_INVALID_PARAM;  /* 赋值或初始化 */
          status = HAL_ERROR;                           /* 赋值或初始化 */

          /* Process unlocked */
          __HAL_UNLOCK(hqspi);                          /* HAL辅助宏/函数 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          hqspi->TxXferCount = (data_size >> 2U);       /* 赋值或初始化 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Nothing to do */
      }                                                 /* 代码块结束 */

      if (status == HAL_OK)                             /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Update state */
        hqspi->State = HAL_QSPI_STATE_BUSY_INDIRECT_TX;  /* 赋值或初始化 */

        /* Clear interrupt */
        __HAL_QSPI_CLEAR_FLAG(hqspi, (QSPI_FLAG_TE | QSPI_FLAG_TC));  /* HAL辅助宏/函数 */

        /* Configure size and pointer of the handle */
        hqspi->TxXferSize = hqspi->TxXferCount;         /* 赋值或初始化 */
        hqspi->pTxBuffPtr = pData;                      /* 赋值或初始化 */

        /* Configure QSPI: CCR register with functional mode as indirect write */
        MODIFY_REG(hqspi->Instance->CCR, QUADSPI_CCR_FMODE, QSPI_FUNCTIONAL_MODE_INDIRECT_WRITE);  /* 执行语句 */

        /* Set the QSPI DMA transfer complete callback */
        hqspi->hdma->XferCpltCallback = QSPI_DMATxCplt;  /* 赋值或初始化 */

        /* Set the QSPI DMA Half transfer complete callback */
        hqspi->hdma->XferHalfCpltCallback = QSPI_DMATxHalfCplt;  /* 赋值或初始化 */

        /* Set the DMA error callback */
        hqspi->hdma->XferErrorCallback = QSPI_DMAError;  /* 赋值或初始化 */

        /* Clear the DMA abort callback */
        hqspi->hdma->XferAbortCallback = NULL;          /* 赋值或初始化 */

#if defined (QSPI1_V2_1L)                               /* 条件编译 */
        /* Bug "ES0305 section 2.1.8 In some specific cases, DMA2 data corruption occurs when managing
           AHB and APB2 peripherals in a concurrent way" Workaround Implementation:
           Change the following configuration of DMA peripheral
             - Enable peripheral increment
             - Disable memory increment
             - Set DMA direction as peripheral to memory mode */

        /* Enable peripheral increment mode of the DMA */
        hqspi->hdma->Init.PeriphInc = DMA_PINC_ENABLE;  /* 赋值或初始化 */

        /* Disable memory increment mode of the DMA */
        hqspi->hdma->Init.MemInc = DMA_MINC_DISABLE;    /* 赋值或初始化 */

        /* Update peripheral/memory increment mode bits */
        MODIFY_REG(hqspi->hdma->Instance->CR, (DMA_SxCR_MINC | DMA_SxCR_PINC), (hqspi->hdma->Init.MemInc | hqspi->hdma->Init.PeriphInc));  /* 执行语句 */

        /* Configure the direction of the DMA */
        hqspi->hdma->Init.Direction = DMA_PERIPH_TO_MEMORY;  /* 赋值或初始化 */
#else                                                   /* 条件编译否则 */
        /* Configure the direction of the DMA */
        hqspi->hdma->Init.Direction = DMA_MEMORY_TO_PERIPH;  /* 赋值或初始化 */
#endif /* QSPI1_V2_1L */

        /* Update direction mode bit */
        MODIFY_REG(hqspi->hdma->Instance->CR, DMA_SxCR_DIR, hqspi->hdma->Init.Direction);  /* 执行语句 */

        /* Enable the QSPI transmit DMA Channel */
        if (HAL_DMA_Start_IT(hqspi->hdma, (uint32_t)pData, (uint32_t)&hqspi->Instance->DR, hqspi->TxXferSize) == HAL_OK)  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Process unlocked */
          __HAL_UNLOCK(hqspi);                          /* HAL辅助宏/函数 */

          /* Enable the QSPI transfer error Interrupt */
          __HAL_QSPI_ENABLE_IT(hqspi, QSPI_IT_TE);      /* HAL辅助宏/函数 */

          /* Enable the DMA transfer by setting the DMAEN bit in the QSPI CR register */
          SET_BIT(hqspi->Instance->CR, QUADSPI_CR_DMAEN);  /* 执行语句 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          status = HAL_ERROR;                           /* 赋值或初始化 */
          hqspi->ErrorCode |= HAL_QSPI_ERROR_DMA;       /* 赋值或初始化 */
          hqspi->State = HAL_QSPI_STATE_READY;          /* 赋值或初始化 */

          /* Process unlocked */
          __HAL_UNLOCK(hqspi);                          /* HAL辅助宏/函数 */
        }                                               /* 代码块结束 */
     }                                                  /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hqspi->ErrorCode |= HAL_QSPI_ERROR_INVALID_PARAM;  /* 赋值或初始化 */
      status = HAL_ERROR;                               /* 赋值或初始化 */

      /* Process unlocked */
      __HAL_UNLOCK(hqspi);                              /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    status = HAL_BUSY;                                  /* 赋值或初始化 */

    /* Process unlocked */
    __HAL_UNLOCK(hqspi);                                /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Receive an amount of data in non-blocking mode with DMA.
  * @param  hqspi : QSPI handle
  * @param  pData : pointer to data buffer.
  * @note   This function is used only in Indirect Read Mode
  * @note   If DMA peripheral access is configured as halfword, the number
  *         of data and the fifo threshold should be aligned on halfword
  * @note   If DMA peripheral access is configured as word, the number
  *         of data and the fifo threshold should be aligned on word
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_QSPI_Receive_DMA(QSPI_HandleTypeDef *hqspi, uint8_t *pData)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */
  uint32_t addr_reg = READ_REG(hqspi->Instance->AR);    /* 32位无符号变量/函数 */
  uint32_t data_size = (READ_REG(hqspi->Instance->DLR) + 1U);  /* 32位无符号变量/函数 */

  /* Process locked */
  __HAL_LOCK(hqspi);                                    /* HAL辅助宏/函数 */

  if(hqspi->State == HAL_QSPI_STATE_READY)              /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Clear the error code */
    hqspi->ErrorCode = HAL_QSPI_ERROR_NONE;             /* 赋值或初始化 */

    if(pData != NULL )                                  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Configure counters of the handle */
      if (hqspi->hdma->Init.PeriphDataAlignment == DMA_PDATAALIGN_BYTE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        hqspi->RxXferCount = data_size;                 /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else if (hqspi->hdma->Init.PeriphDataAlignment == DMA_PDATAALIGN_HALFWORD)  /* 否则如果 */
      {                                                 /* 代码块开始 */
        if (((data_size % 2U) != 0U) || ((hqspi->Init.FifoThreshold % 2U) != 0U))  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* The number of data or the fifo threshold is not aligned on halfword
             => no transfer possible with DMA peripheral access configured as halfword */
          hqspi->ErrorCode |= HAL_QSPI_ERROR_INVALID_PARAM;  /* 赋值或初始化 */
          status = HAL_ERROR;                           /* 赋值或初始化 */

          /* Process unlocked */
          __HAL_UNLOCK(hqspi);                          /* HAL辅助宏/函数 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          hqspi->RxXferCount = (data_size >> 1U);       /* 赋值或初始化 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else if (hqspi->hdma->Init.PeriphDataAlignment == DMA_PDATAALIGN_WORD)  /* 否则如果 */
      {                                                 /* 代码块开始 */
        if (((data_size % 4U) != 0U) || ((hqspi->Init.FifoThreshold % 4U) != 0U))  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* The number of data or the fifo threshold is not aligned on word
             => no transfer possible with DMA peripheral access configured as word */
          hqspi->ErrorCode |= HAL_QSPI_ERROR_INVALID_PARAM;  /* 赋值或初始化 */
          status = HAL_ERROR;                           /* 赋值或初始化 */

          /* Process unlocked */
          __HAL_UNLOCK(hqspi);                          /* HAL辅助宏/函数 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          hqspi->RxXferCount = (data_size >> 2U);       /* 赋值或初始化 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Nothing to do */
      }                                                 /* 代码块结束 */

      if (status == HAL_OK)                             /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Update state */
        hqspi->State = HAL_QSPI_STATE_BUSY_INDIRECT_RX;  /* 赋值或初始化 */

        /* Clear interrupt */
        __HAL_QSPI_CLEAR_FLAG(hqspi, (QSPI_FLAG_TE | QSPI_FLAG_TC));  /* HAL辅助宏/函数 */

        /* Configure size and pointer of the handle */
        hqspi->RxXferSize = hqspi->RxXferCount;         /* 赋值或初始化 */
        hqspi->pRxBuffPtr = pData;                      /* 赋值或初始化 */

        /* Set the QSPI DMA transfer complete callback */
        hqspi->hdma->XferCpltCallback = QSPI_DMARxCplt;  /* 赋值或初始化 */

        /* Set the QSPI DMA Half transfer complete callback */
        hqspi->hdma->XferHalfCpltCallback = QSPI_DMARxHalfCplt;  /* 赋值或初始化 */

        /* Set the DMA error callback */
        hqspi->hdma->XferErrorCallback = QSPI_DMAError;  /* 赋值或初始化 */

        /* Clear the DMA abort callback */
        hqspi->hdma->XferAbortCallback = NULL;          /* 赋值或初始化 */

#if defined (QSPI1_V2_1L)                               /* 条件编译 */
      /* Bug "ES0305 section 2.1.8 In some specific cases, DMA2 data corruption occurs when managing
         AHB and APB2 peripherals in a concurrent way" Workaround Implementation:
         Change the following configuration of DMA peripheral
           - Enable peripheral increment
           - Disable memory increment
           - Set DMA direction as memory to peripheral mode
           - 4 Extra words (32-bits) are added for read operation to guarantee
              the last data is transferred from DMA FIFO to RAM memory */

        /* Enable peripheral increment of the DMA */
        hqspi->hdma->Init.PeriphInc = DMA_PINC_ENABLE;  /* 赋值或初始化 */

        /* Disable memory increment of the DMA */
        hqspi->hdma->Init.MemInc = DMA_MINC_DISABLE;    /* 赋值或初始化 */

        /* Update peripheral/memory increment mode bits */
        MODIFY_REG(hqspi->hdma->Instance->CR, (DMA_SxCR_MINC | DMA_SxCR_PINC), (hqspi->hdma->Init.MemInc | hqspi->hdma->Init.PeriphInc));  /* 执行语句 */

        /* Configure the direction of the DMA */
        hqspi->hdma->Init.Direction = DMA_MEMORY_TO_PERIPH;  /* 赋值或初始化 */

        /* 4 Extra words (32-bits) are needed for read operation to guarantee
        the last data is transferred from DMA FIFO to RAM memory */
        WRITE_REG(hqspi->Instance->DLR, (data_size - 1U + 16U));  /* 执行语句 */
#else                                                   /* 条件编译否则 */
        /* Configure the direction of the DMA */
        hqspi->hdma->Init.Direction = DMA_PERIPH_TO_MEMORY;  /* 赋值或初始化 */
#endif                                                  /* 条件编译结束 */
        /* Update direction mode bit */
        MODIFY_REG(hqspi->hdma->Instance->CR, DMA_SxCR_DIR, hqspi->hdma->Init.Direction);  /* 执行语句 */

        /* Enable the DMA Channel */
        if (HAL_DMA_Start_IT(hqspi->hdma, (uint32_t)&hqspi->Instance->DR, (uint32_t)pData, hqspi->RxXferSize) == HAL_OK)  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Configure QSPI: CCR register with functional as indirect read */
          MODIFY_REG(hqspi->Instance->CCR, QUADSPI_CCR_FMODE, QSPI_FUNCTIONAL_MODE_INDIRECT_READ);  /* 执行语句 */

          /* Start the transfer by re-writing the address in AR register */
          WRITE_REG(hqspi->Instance->AR, addr_reg);     /* 执行语句 */

          /* Process unlocked */
          __HAL_UNLOCK(hqspi);                          /* HAL辅助宏/函数 */

          /* Enable the QSPI transfer error Interrupt */
          __HAL_QSPI_ENABLE_IT(hqspi, QSPI_IT_TE);      /* HAL辅助宏/函数 */

          /* Enable the DMA transfer by setting the DMAEN bit in the QSPI CR register */
          SET_BIT(hqspi->Instance->CR, QUADSPI_CR_DMAEN);  /* 执行语句 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          status = HAL_ERROR;                           /* 赋值或初始化 */
          hqspi->ErrorCode |= HAL_QSPI_ERROR_DMA;       /* 赋值或初始化 */
          hqspi->State = HAL_QSPI_STATE_READY;          /* 赋值或初始化 */

          /* Process unlocked */
          __HAL_UNLOCK(hqspi);                          /* HAL辅助宏/函数 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hqspi->ErrorCode |= HAL_QSPI_ERROR_INVALID_PARAM;  /* 赋值或初始化 */
      status = HAL_ERROR;                               /* 赋值或初始化 */

      /* Process unlocked */
      __HAL_UNLOCK(hqspi);                              /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    status = HAL_BUSY;                                  /* 赋值或初始化 */

    /* Process unlocked */
    __HAL_UNLOCK(hqspi);                                /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configure the QSPI Automatic Polling Mode in blocking mode.
  * @param  hqspi : QSPI handle
  * @param  cmd : structure that contains the command configuration information.
  * @param  cfg : structure that contains the polling configuration information.
  * @param  Timeout : Timeout duration
  * @note   This function is used only in Automatic Polling Mode
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_QSPI_AutoPolling(QSPI_HandleTypeDef *hqspi, QSPI_CommandTypeDef *cmd, QSPI_AutoPollingTypeDef *cfg, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status;                             /* HAL状态类型函数 */
  uint32_t tickstart = HAL_GetTick();                   /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_QSPI_INSTRUCTION_MODE(cmd->InstructionMode));  /* 执行语句 */
  if (cmd->InstructionMode != QSPI_INSTRUCTION_NONE)    /* 条件判断 */
  {                                                     /* 代码块开始 */
    assert_param(IS_QSPI_INSTRUCTION(cmd->Instruction));  /* 执行语句 */
  }                                                     /* 代码块结束 */

  assert_param(IS_QSPI_ADDRESS_MODE(cmd->AddressMode));  /* 执行语句 */
  if (cmd->AddressMode != QSPI_ADDRESS_NONE)            /* 条件判断 */
  {                                                     /* 代码块开始 */
    assert_param(IS_QSPI_ADDRESS_SIZE(cmd->AddressSize));  /* 执行语句 */
  }                                                     /* 代码块结束 */

  assert_param(IS_QSPI_ALTERNATE_BYTES_MODE(cmd->AlternateByteMode));  /* 执行语句 */
  if (cmd->AlternateByteMode != QSPI_ALTERNATE_BYTES_NONE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    assert_param(IS_QSPI_ALTERNATE_BYTES_SIZE(cmd->AlternateBytesSize));  /* 执行语句 */
  }                                                     /* 代码块结束 */

  assert_param(IS_QSPI_DUMMY_CYCLES(cmd->DummyCycles));  /* 执行语句 */
  assert_param(IS_QSPI_DATA_MODE(cmd->DataMode));       /* 执行语句 */

  assert_param(IS_QSPI_DDR_MODE(cmd->DdrMode));         /* 执行语句 */
  assert_param(IS_QSPI_DDR_HHC(cmd->DdrHoldHalfCycle));  /* 执行语句 */
  assert_param(IS_QSPI_SIOO_MODE(cmd->SIOOMode));       /* 执行语句 */

  assert_param(IS_QSPI_INTERVAL(cfg->Interval));        /* 执行语句 */
  assert_param(IS_QSPI_STATUS_BYTES_SIZE(cfg->StatusBytesSize));  /* 执行语句 */
  assert_param(IS_QSPI_MATCH_MODE(cfg->MatchMode));     /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hqspi);                                    /* HAL辅助宏/函数 */

  if(hqspi->State == HAL_QSPI_STATE_READY)              /* 条件判断 */
  {                                                     /* 代码块开始 */
    hqspi->ErrorCode = HAL_QSPI_ERROR_NONE;             /* 赋值或初始化 */

    /* Update state */
    hqspi->State = HAL_QSPI_STATE_BUSY_AUTO_POLLING;    /* 赋值或初始化 */

    /* Wait till BUSY flag reset */
    status = QSPI_WaitFlagStateUntilTimeout(hqspi, QSPI_FLAG_BUSY, RESET, tickstart, Timeout);  /* 赋值或初始化 */

    if (status == HAL_OK)                               /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Configure QSPI: PSMAR register with the status match value */
      WRITE_REG(hqspi->Instance->PSMAR, cfg->Match);    /* 执行语句 */

      /* Configure QSPI: PSMKR register with the status mask value */
      WRITE_REG(hqspi->Instance->PSMKR, cfg->Mask);     /* 执行语句 */

      /* Configure QSPI: PIR register with the interval value */
      WRITE_REG(hqspi->Instance->PIR, cfg->Interval);   /* 执行语句 */

      /* Configure QSPI: CR register with Match mode and Automatic stop enabled
      (otherwise there will be an infinite loop in blocking mode) */
      MODIFY_REG(hqspi->Instance->CR, (QUADSPI_CR_PMM | QUADSPI_CR_APMS),  /* 列表续行 */
               (cfg->MatchMode | QSPI_AUTOMATIC_STOP_ENABLE));  /* 执行语句 */

      /* Call the configuration function */
      cmd->NbData = cfg->StatusBytesSize;               /* 赋值或初始化 */
      QSPI_Config(hqspi, cmd, QSPI_FUNCTIONAL_MODE_AUTO_POLLING);  /* 执行语句 */

      /* Wait until SM flag is set to go back in idle state */
      status = QSPI_WaitFlagStateUntilTimeout(hqspi, QSPI_FLAG_SM, SET, tickstart, Timeout);  /* 赋值或初始化 */

      if (status == HAL_OK)                             /* 条件判断 */
      {                                                 /* 代码块开始 */
        __HAL_QSPI_CLEAR_FLAG(hqspi, QSPI_FLAG_SM);     /* HAL辅助宏/函数 */

        /* Update state */
        hqspi->State = HAL_QSPI_STATE_READY;            /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    status = HAL_BUSY;                                  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Process unlocked */
  __HAL_UNLOCK(hqspi);                                  /* HAL辅助宏/函数 */

  /* Return function status */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configure the QSPI Automatic Polling Mode in non-blocking mode.
  * @param  hqspi : QSPI handle
  * @param  cmd : structure that contains the command configuration information.
  * @param  cfg : structure that contains the polling configuration information.
  * @note   This function is used only in Automatic Polling Mode
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_QSPI_AutoPolling_IT(QSPI_HandleTypeDef *hqspi, QSPI_CommandTypeDef *cmd, QSPI_AutoPollingTypeDef *cfg)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status;                             /* HAL状态类型函数 */
  uint32_t tickstart = HAL_GetTick();                   /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_QSPI_INSTRUCTION_MODE(cmd->InstructionMode));  /* 执行语句 */
  if (cmd->InstructionMode != QSPI_INSTRUCTION_NONE)    /* 条件判断 */
  {                                                     /* 代码块开始 */
    assert_param(IS_QSPI_INSTRUCTION(cmd->Instruction));  /* 执行语句 */
  }                                                     /* 代码块结束 */

  assert_param(IS_QSPI_ADDRESS_MODE(cmd->AddressMode));  /* 执行语句 */
  if (cmd->AddressMode != QSPI_ADDRESS_NONE)            /* 条件判断 */
  {                                                     /* 代码块开始 */
    assert_param(IS_QSPI_ADDRESS_SIZE(cmd->AddressSize));  /* 执行语句 */
  }                                                     /* 代码块结束 */

  assert_param(IS_QSPI_ALTERNATE_BYTES_MODE(cmd->AlternateByteMode));  /* 执行语句 */
  if (cmd->AlternateByteMode != QSPI_ALTERNATE_BYTES_NONE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    assert_param(IS_QSPI_ALTERNATE_BYTES_SIZE(cmd->AlternateBytesSize));  /* 执行语句 */
  }                                                     /* 代码块结束 */

  assert_param(IS_QSPI_DUMMY_CYCLES(cmd->DummyCycles));  /* 执行语句 */
  assert_param(IS_QSPI_DATA_MODE(cmd->DataMode));       /* 执行语句 */

  assert_param(IS_QSPI_DDR_MODE(cmd->DdrMode));         /* 执行语句 */
  assert_param(IS_QSPI_DDR_HHC(cmd->DdrHoldHalfCycle));  /* 执行语句 */
  assert_param(IS_QSPI_SIOO_MODE(cmd->SIOOMode));       /* 执行语句 */

  assert_param(IS_QSPI_INTERVAL(cfg->Interval));        /* 执行语句 */
  assert_param(IS_QSPI_STATUS_BYTES_SIZE(cfg->StatusBytesSize));  /* 执行语句 */
  assert_param(IS_QSPI_MATCH_MODE(cfg->MatchMode));     /* 执行语句 */
  assert_param(IS_QSPI_AUTOMATIC_STOP(cfg->AutomaticStop));  /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hqspi);                                    /* HAL辅助宏/函数 */

  if(hqspi->State == HAL_QSPI_STATE_READY)              /* 条件判断 */
  {                                                     /* 代码块开始 */
    hqspi->ErrorCode = HAL_QSPI_ERROR_NONE;             /* 赋值或初始化 */

    /* Update state */
    hqspi->State = HAL_QSPI_STATE_BUSY_AUTO_POLLING;    /* 赋值或初始化 */

    /* Wait till BUSY flag reset */
    status = QSPI_WaitFlagStateUntilTimeout(hqspi, QSPI_FLAG_BUSY, RESET, tickstart, hqspi->Timeout);  /* 赋值或初始化 */

    if (status == HAL_OK)                               /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Configure QSPI: PSMAR register with the status match value */
      WRITE_REG(hqspi->Instance->PSMAR, cfg->Match);    /* 执行语句 */

      /* Configure QSPI: PSMKR register with the status mask value */
      WRITE_REG(hqspi->Instance->PSMKR, cfg->Mask);     /* 执行语句 */

      /* Configure QSPI: PIR register with the interval value */
      WRITE_REG(hqspi->Instance->PIR, cfg->Interval);   /* 执行语句 */

      /* Configure QSPI: CR register with Match mode and Automatic stop mode */
      MODIFY_REG(hqspi->Instance->CR, (QUADSPI_CR_PMM | QUADSPI_CR_APMS),  /* 列表续行 */
               (cfg->MatchMode | cfg->AutomaticStop));  /* 执行语句 */

      /* Clear interrupt */
      __HAL_QSPI_CLEAR_FLAG(hqspi, QSPI_FLAG_TE | QSPI_FLAG_SM);  /* HAL辅助宏/函数 */

      /* Call the configuration function */
      cmd->NbData = cfg->StatusBytesSize;               /* 赋值或初始化 */
      QSPI_Config(hqspi, cmd, QSPI_FUNCTIONAL_MODE_AUTO_POLLING);  /* 执行语句 */

      /* Process unlocked */
      __HAL_UNLOCK(hqspi);                              /* HAL辅助宏/函数 */

      /* Enable the QSPI Transfer Error and status match Interrupt */
      __HAL_QSPI_ENABLE_IT(hqspi, (QSPI_IT_SM | QSPI_IT_TE));  /* HAL辅助宏/函数 */

    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Process unlocked */
      __HAL_UNLOCK(hqspi);                              /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    status = HAL_BUSY;                                  /* 赋值或初始化 */

    /* Process unlocked */
    __HAL_UNLOCK(hqspi);                                /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Return function status */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configure the Memory Mapped mode.
  * @param  hqspi : QSPI handle
  * @param  cmd : structure that contains the command configuration information.
  * @param  cfg : structure that contains the memory mapped configuration information.
  * @note   This function is used only in Memory mapped Mode
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_QSPI_MemoryMapped(QSPI_HandleTypeDef *hqspi, QSPI_CommandTypeDef *cmd, QSPI_MemoryMappedTypeDef *cfg)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status;                             /* HAL状态类型函数 */
  uint32_t tickstart = HAL_GetTick();                   /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_QSPI_INSTRUCTION_MODE(cmd->InstructionMode));  /* 执行语句 */
  if (cmd->InstructionMode != QSPI_INSTRUCTION_NONE)    /* 条件判断 */
  {                                                     /* 代码块开始 */
  assert_param(IS_QSPI_INSTRUCTION(cmd->Instruction));  /* 执行语句 */
  }                                                     /* 代码块结束 */

  assert_param(IS_QSPI_ADDRESS_MODE(cmd->AddressMode));  /* 执行语句 */
  if (cmd->AddressMode != QSPI_ADDRESS_NONE)            /* 条件判断 */
  {                                                     /* 代码块开始 */
    assert_param(IS_QSPI_ADDRESS_SIZE(cmd->AddressSize));  /* 执行语句 */
  }                                                     /* 代码块结束 */

  assert_param(IS_QSPI_ALTERNATE_BYTES_MODE(cmd->AlternateByteMode));  /* 执行语句 */
  if (cmd->AlternateByteMode != QSPI_ALTERNATE_BYTES_NONE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    assert_param(IS_QSPI_ALTERNATE_BYTES_SIZE(cmd->AlternateBytesSize));  /* 执行语句 */
  }                                                     /* 代码块结束 */

  assert_param(IS_QSPI_DUMMY_CYCLES(cmd->DummyCycles));  /* 执行语句 */
  assert_param(IS_QSPI_DATA_MODE(cmd->DataMode));       /* 执行语句 */

  assert_param(IS_QSPI_DDR_MODE(cmd->DdrMode));         /* 执行语句 */
  assert_param(IS_QSPI_DDR_HHC(cmd->DdrHoldHalfCycle));  /* 执行语句 */
  assert_param(IS_QSPI_SIOO_MODE(cmd->SIOOMode));       /* 执行语句 */

  assert_param(IS_QSPI_TIMEOUT_ACTIVATION(cfg->TimeOutActivation));  /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hqspi);                                    /* HAL辅助宏/函数 */

  if(hqspi->State == HAL_QSPI_STATE_READY)              /* 条件判断 */
  {                                                     /* 代码块开始 */
    hqspi->ErrorCode = HAL_QSPI_ERROR_NONE;             /* 赋值或初始化 */

    /* Update state */
    hqspi->State = HAL_QSPI_STATE_BUSY_MEM_MAPPED;      /* 赋值或初始化 */

    /* Wait till BUSY flag reset */
    status = QSPI_WaitFlagStateUntilTimeout(hqspi, QSPI_FLAG_BUSY, RESET, tickstart, hqspi->Timeout);  /* 赋值或初始化 */

    if (status == HAL_OK)                               /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Configure QSPI: CR register with timeout counter enable */
    MODIFY_REG(hqspi->Instance->CR, QUADSPI_CR_TCEN, cfg->TimeOutActivation);  /* 执行语句 */

    if (cfg->TimeOutActivation == QSPI_TIMEOUT_COUNTER_ENABLE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        assert_param(IS_QSPI_TIMEOUT_PERIOD(cfg->TimeOutPeriod));  /* 执行语句 */

        /* Configure QSPI: LPTR register with the low-power timeout value */
        WRITE_REG(hqspi->Instance->LPTR, cfg->TimeOutPeriod);  /* 执行语句 */

        /* Clear interrupt */
        __HAL_QSPI_CLEAR_FLAG(hqspi, QSPI_FLAG_TO);     /* HAL辅助宏/函数 */

        /* Enable the QSPI TimeOut Interrupt */
        __HAL_QSPI_ENABLE_IT(hqspi, QSPI_IT_TO);        /* HAL辅助宏/函数 */
      }                                                 /* 代码块结束 */

      /* Call the configuration function */
      QSPI_Config(hqspi, cmd, QSPI_FUNCTIONAL_MODE_MEMORY_MAPPED);  /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    status = HAL_BUSY;                                  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Process unlocked */
  __HAL_UNLOCK(hqspi);                                  /* HAL辅助宏/函数 */

  /* Return function status */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Transfer Error callback.
  * @param  hqspi : QSPI handle
  * @retval None
  */
__weak void HAL_QSPI_ErrorCallback(QSPI_HandleTypeDef *hqspi)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hqspi);                                        /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_QSPI_ErrorCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Abort completed callback.
  * @param  hqspi : QSPI handle
  * @retval None
  */
__weak void HAL_QSPI_AbortCpltCallback(QSPI_HandleTypeDef *hqspi)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hqspi);                                        /* 执行语句 */

  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_QSPI_AbortCpltCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Command completed callback.
  * @param  hqspi : QSPI handle
  * @retval None
  */
__weak void HAL_QSPI_CmdCpltCallback(QSPI_HandleTypeDef *hqspi)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hqspi);                                        /* 执行语句 */

  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_QSPI_CmdCpltCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Rx Transfer completed callback.
  * @param  hqspi : QSPI handle
  * @retval None
  */
__weak void HAL_QSPI_RxCpltCallback(QSPI_HandleTypeDef *hqspi)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hqspi);                                        /* 执行语句 */

  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_QSPI_RxCpltCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Tx Transfer completed callback.
  * @param  hqspi : QSPI handle
  * @retval None
  */
__weak void HAL_QSPI_TxCpltCallback(QSPI_HandleTypeDef *hqspi)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hqspi);                                        /* 执行语句 */

  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_QSPI_TxCpltCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Rx Half Transfer completed callback.
  * @param  hqspi : QSPI handle
  * @retval None
  */
__weak void HAL_QSPI_RxHalfCpltCallback(QSPI_HandleTypeDef *hqspi)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hqspi);                                        /* 执行语句 */

  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_QSPI_RxHalfCpltCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Tx Half Transfer completed callback.
  * @param  hqspi : QSPI handle
  * @retval None
  */
__weak void HAL_QSPI_TxHalfCpltCallback(QSPI_HandleTypeDef *hqspi)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hqspi);                                        /* 执行语句 */

  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_QSPI_TxHalfCpltCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  FIFO Threshold callback.
  * @param  hqspi : QSPI handle
  * @retval None
  */
__weak void HAL_QSPI_FifoThresholdCallback(QSPI_HandleTypeDef *hqspi)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hqspi);                                        /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_QSPI_FIFOThresholdCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Status Match callback.
  * @param  hqspi : QSPI handle
  * @retval None
  */
__weak void HAL_QSPI_StatusMatchCallback(QSPI_HandleTypeDef *hqspi)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hqspi);                                        /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_QSPI_StatusMatchCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Timeout callback.
  * @param  hqspi : QSPI handle
  * @retval None
  */
__weak void HAL_QSPI_TimeOutCallback(QSPI_HandleTypeDef *hqspi)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hqspi);                                        /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_QSPI_TimeOutCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */
#if (USE_HAL_QSPI_REGISTER_CALLBACKS == 1)              /* 条件编译 */
/**
  * @brief  Register a User QSPI Callback
  *         To be used instead of the weak (surcharged) predefined callback
  * @param hqspi : QSPI handle
  * @param CallbackId : ID of the callback to be registered
  *        This parameter can be one of the following values:
  *          @arg @ref HAL_QSPI_ERROR_CB_ID          QSPI Error Callback ID
  *          @arg @ref HAL_QSPI_ABORT_CB_ID          QSPI Abort Callback ID
  *          @arg @ref HAL_QSPI_FIFO_THRESHOLD_CB_ID QSPI FIFO Threshold Callback ID
  *          @arg @ref HAL_QSPI_CMD_CPLT_CB_ID       QSPI Command Complete Callback ID
  *          @arg @ref HAL_QSPI_RX_CPLT_CB_ID        QSPI Rx Complete Callback ID
  *          @arg @ref HAL_QSPI_TX_CPLT_CB_ID        QSPI Tx Complete Callback ID
  *          @arg @ref HAL_QSPI_RX_HALF_CPLT_CB_ID   QSPI Rx Half Complete Callback ID
  *          @arg @ref HAL_QSPI_TX_HALF_CPLT_CB_ID   QSPI Tx Half Complete Callback ID
  *          @arg @ref HAL_QSPI_STATUS_MATCH_CB_ID   QSPI Status Match Callback ID
  *          @arg @ref HAL_QSPI_TIMEOUT_CB_ID        QSPI Timeout Callback ID
  *          @arg @ref HAL_QSPI_MSP_INIT_CB_ID       QSPI MspInit callback ID
  *          @arg @ref HAL_QSPI_MSP_DEINIT_CB_ID     QSPI MspDeInit callback ID
  * @param pCallback : pointer to the Callback function
  * @retval status
  */
HAL_StatusTypeDef HAL_QSPI_RegisterCallback (QSPI_HandleTypeDef *hqspi, HAL_QSPI_CallbackIDTypeDef CallbackId, pQSPI_CallbackTypeDef pCallback)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  if(pCallback == NULL)                                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hqspi->ErrorCode |= HAL_QSPI_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Process locked */
  __HAL_LOCK(hqspi);                                    /* HAL辅助宏/函数 */

  if(hqspi->State == HAL_QSPI_STATE_READY)              /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackId)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
    case  HAL_QSPI_ERROR_CB_ID :                        /* case标签 */
      hqspi->ErrorCallback = pCallback;                 /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_QSPI_ABORT_CB_ID :                         /* case标签 */
      hqspi->AbortCpltCallback = pCallback;             /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_QSPI_FIFO_THRESHOLD_CB_ID :                /* case标签 */
      hqspi->FifoThresholdCallback = pCallback;         /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_QSPI_CMD_CPLT_CB_ID :                      /* case标签 */
      hqspi->CmdCpltCallback = pCallback;               /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_QSPI_RX_CPLT_CB_ID :                       /* case标签 */
      hqspi->RxCpltCallback = pCallback;                /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_QSPI_TX_CPLT_CB_ID :                       /* case标签 */
      hqspi->TxCpltCallback = pCallback;                /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_QSPI_RX_HALF_CPLT_CB_ID :                  /* case标签 */
      hqspi->RxHalfCpltCallback = pCallback;            /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_QSPI_TX_HALF_CPLT_CB_ID :                  /* case标签 */
      hqspi->TxHalfCpltCallback = pCallback;            /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_QSPI_STATUS_MATCH_CB_ID :                  /* case标签 */
      hqspi->StatusMatchCallback = pCallback;           /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_QSPI_TIMEOUT_CB_ID :                       /* case标签 */
      hqspi->TimeOutCallback = pCallback;               /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_QSPI_MSP_INIT_CB_ID :                      /* case标签 */
      hqspi->MspInitCallback = pCallback;               /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_QSPI_MSP_DEINIT_CB_ID :                    /* case标签 */
      hqspi->MspDeInitCallback = pCallback;             /* 赋值或初始化 */
      break;                                            /* 跳出 */
    default :                                           /* default分支 */
      /* Update the error code */
      hqspi->ErrorCode |= HAL_QSPI_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
      /* update return status */
      status =  HAL_ERROR;                              /* 赋值或初始化 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (hqspi->State == HAL_QSPI_STATE_RESET)        /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackId)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
    case HAL_QSPI_MSP_INIT_CB_ID :                      /* case标签 */
      hqspi->MspInitCallback = pCallback;               /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_QSPI_MSP_DEINIT_CB_ID :                    /* case标签 */
      hqspi->MspDeInitCallback = pCallback;             /* 赋值或初始化 */
      break;                                            /* 跳出 */
    default :                                           /* default分支 */
      /* Update the error code */
      hqspi->ErrorCode |= HAL_QSPI_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
      /* update return status */
      status =  HAL_ERROR;                              /* 赋值或初始化 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hqspi->ErrorCode |= HAL_QSPI_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
    /* update return status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hqspi);                                  /* HAL辅助宏/函数 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Unregister a User QSPI Callback
  *         QSPI Callback is redirected to the weak (surcharged) predefined callback
  * @param hqspi : QSPI handle
  * @param CallbackId : ID of the callback to be unregistered
  *        This parameter can be one of the following values:
  *          @arg @ref HAL_QSPI_ERROR_CB_ID          QSPI Error Callback ID
  *          @arg @ref HAL_QSPI_ABORT_CB_ID          QSPI Abort Callback ID
  *          @arg @ref HAL_QSPI_FIFO_THRESHOLD_CB_ID QSPI FIFO Threshold Callback ID
  *          @arg @ref HAL_QSPI_CMD_CPLT_CB_ID       QSPI Command Complete Callback ID
  *          @arg @ref HAL_QSPI_RX_CPLT_CB_ID        QSPI Rx Complete Callback ID
  *          @arg @ref HAL_QSPI_TX_CPLT_CB_ID        QSPI Tx Complete Callback ID
  *          @arg @ref HAL_QSPI_RX_HALF_CPLT_CB_ID   QSPI Rx Half Complete Callback ID
  *          @arg @ref HAL_QSPI_TX_HALF_CPLT_CB_ID   QSPI Tx Half Complete Callback ID
  *          @arg @ref HAL_QSPI_STATUS_MATCH_CB_ID   QSPI Status Match Callback ID
  *          @arg @ref HAL_QSPI_TIMEOUT_CB_ID        QSPI Timeout Callback ID
  *          @arg @ref HAL_QSPI_MSP_INIT_CB_ID       QSPI MspInit callback ID
  *          @arg @ref HAL_QSPI_MSP_DEINIT_CB_ID     QSPI MspDeInit callback ID
  * @retval status
  */
HAL_StatusTypeDef HAL_QSPI_UnRegisterCallback (QSPI_HandleTypeDef *hqspi, HAL_QSPI_CallbackIDTypeDef CallbackId)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Process locked */
  __HAL_LOCK(hqspi);                                    /* HAL辅助宏/函数 */

  if(hqspi->State == HAL_QSPI_STATE_READY)              /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackId)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
    case  HAL_QSPI_ERROR_CB_ID :                        /* case标签 */
      hqspi->ErrorCallback = HAL_QSPI_ErrorCallback;    /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_QSPI_ABORT_CB_ID :                         /* case标签 */
      hqspi->AbortCpltCallback = HAL_QSPI_AbortCpltCallback;  /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_QSPI_FIFO_THRESHOLD_CB_ID :                /* case标签 */
      hqspi->FifoThresholdCallback = HAL_QSPI_FifoThresholdCallback;  /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_QSPI_CMD_CPLT_CB_ID :                      /* case标签 */
      hqspi->CmdCpltCallback = HAL_QSPI_CmdCpltCallback;  /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_QSPI_RX_CPLT_CB_ID :                       /* case标签 */
      hqspi->RxCpltCallback = HAL_QSPI_RxCpltCallback;  /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_QSPI_TX_CPLT_CB_ID :                       /* case标签 */
      hqspi->TxCpltCallback = HAL_QSPI_TxCpltCallback;  /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_QSPI_RX_HALF_CPLT_CB_ID :                  /* case标签 */
      hqspi->RxHalfCpltCallback = HAL_QSPI_RxHalfCpltCallback;  /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_QSPI_TX_HALF_CPLT_CB_ID :                  /* case标签 */
      hqspi->TxHalfCpltCallback = HAL_QSPI_TxHalfCpltCallback;  /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_QSPI_STATUS_MATCH_CB_ID :                  /* case标签 */
      hqspi->StatusMatchCallback = HAL_QSPI_StatusMatchCallback;  /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_QSPI_TIMEOUT_CB_ID :                       /* case标签 */
      hqspi->TimeOutCallback = HAL_QSPI_TimeOutCallback;  /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_QSPI_MSP_INIT_CB_ID :                      /* case标签 */
      hqspi->MspInitCallback = HAL_QSPI_MspInit;        /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_QSPI_MSP_DEINIT_CB_ID :                    /* case标签 */
      hqspi->MspDeInitCallback = HAL_QSPI_MspDeInit;    /* 赋值或初始化 */
      break;                                            /* 跳出 */
    default :                                           /* default分支 */
      /* Update the error code */
      hqspi->ErrorCode |= HAL_QSPI_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
      /* update return status */
      status =  HAL_ERROR;                              /* 赋值或初始化 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (hqspi->State == HAL_QSPI_STATE_RESET)        /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackId)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
    case HAL_QSPI_MSP_INIT_CB_ID :                      /* case标签 */
      hqspi->MspInitCallback = HAL_QSPI_MspInit;        /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case HAL_QSPI_MSP_DEINIT_CB_ID :                    /* case标签 */
      hqspi->MspDeInitCallback = HAL_QSPI_MspDeInit;    /* 赋值或初始化 */
      break;                                            /* 跳出 */
    default :                                           /* default分支 */
      /* Update the error code */
      hqspi->ErrorCode |= HAL_QSPI_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
      /* update return status */
      status =  HAL_ERROR;                              /* 赋值或初始化 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hqspi->ErrorCode |= HAL_QSPI_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
    /* update return status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hqspi);                                  /* HAL辅助宏/函数 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */
#endif                                                  /* 条件编译结束 */

/**
  * @}
  */

/** @defgroup QSPI_Exported_Functions_Group3 Peripheral Control and State functions
  *  @brief   QSPI control and State functions
  *
@verbatim
 ===============================================================================
                  ##### Peripheral Control and State functions #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to :
      (+) Check in run-time the state of the driver.
      (+) Check the error code set during last operation.
      (+) Abort any operation.


@endverbatim
  * @{
  */

/**
  * @brief  Return the QSPI handle state.
  * @param  hqspi : QSPI handle
  * @retval HAL state
  */
HAL_QSPI_StateTypeDef HAL_QSPI_GetState(QSPI_HandleTypeDef *hqspi)  /* HAL库函数调用 */
{                                                       /* 代码块开始 */
  /* Return QSPI handle state */
  return hqspi->State;                                  /* 返回值 */
}                                                       /* 代码块结束 */

/**
* @brief  Return the QSPI error code.
* @param  hqspi : QSPI handle
* @retval QSPI Error Code
*/
uint32_t HAL_QSPI_GetError(QSPI_HandleTypeDef *hqspi)   /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  return hqspi->ErrorCode;                              /* 返回值 */
}                                                       /* 代码块结束 */

/**
* @brief  Abort the current transmission.
* @param  hqspi : QSPI handle
* @retval HAL status
*/
HAL_StatusTypeDef HAL_QSPI_Abort(QSPI_HandleTypeDef *hqspi)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */
  uint32_t tickstart = HAL_GetTick();                   /* 32位无符号变量/函数 */

  /* Check if the state is in one of the busy states */
  if (((uint32_t)hqspi->State & 0x2U) != 0U)            /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Process unlocked */
    __HAL_UNLOCK(hqspi);                                /* HAL辅助宏/函数 */

    if ((hqspi->Instance->CR & QUADSPI_CR_DMAEN) != 0U)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable the DMA transfer by clearing the DMAEN bit in the QSPI CR register */
      CLEAR_BIT(hqspi->Instance->CR, QUADSPI_CR_DMAEN);  /* 执行语句 */

      /* Abort DMA channel */
      status = HAL_DMA_Abort(hqspi->hdma);              /* 赋值或初始化 */
      if(status != HAL_OK)                              /* 条件判断 */
      {                                                 /* 代码块开始 */
        hqspi->ErrorCode |= HAL_QSPI_ERROR_DMA;         /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Configure QSPI: CR register with Abort request */
    SET_BIT(hqspi->Instance->CR, QUADSPI_CR_ABORT);     /* 执行语句 */

    /* Wait until TC flag is set to go back in idle state */
    status = QSPI_WaitFlagStateUntilTimeout(hqspi, QSPI_FLAG_TC, SET, tickstart, hqspi->Timeout);  /* 赋值或初始化 */

    if (status == HAL_OK)                               /* 条件判断 */
    {                                                   /* 代码块开始 */
      __HAL_QSPI_CLEAR_FLAG(hqspi, QSPI_FLAG_TC);       /* HAL辅助宏/函数 */

      /* Wait until BUSY flag is reset */
      status = QSPI_WaitFlagStateUntilTimeout(hqspi, QSPI_FLAG_BUSY, RESET, tickstart, hqspi->Timeout);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    if (status == HAL_OK)                               /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Reset functional mode configuration to indirect write mode by default */
      CLEAR_BIT(hqspi->Instance->CCR, QUADSPI_CCR_FMODE);  /* 执行语句 */

      /* Update state */
      hqspi->State = HAL_QSPI_STATE_READY;              /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
* @brief  Abort the current transmission (non-blocking function)
* @param  hqspi : QSPI handle
* @retval HAL status
*/
HAL_StatusTypeDef HAL_QSPI_Abort_IT(QSPI_HandleTypeDef *hqspi)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Check if the state is in one of the busy states */
  if (((uint32_t)hqspi->State & 0x2U) != 0U)            /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Process unlocked */
    __HAL_UNLOCK(hqspi);                                /* HAL辅助宏/函数 */

    /* Update QSPI state */
    hqspi->State = HAL_QSPI_STATE_ABORT;                /* 赋值或初始化 */

    /* Disable all interrupts */
    __HAL_QSPI_DISABLE_IT(hqspi, (QSPI_IT_TO | QSPI_IT_SM | QSPI_IT_FT | QSPI_IT_TC | QSPI_IT_TE));  /* HAL辅助宏/函数 */

    if ((hqspi->Instance->CR & QUADSPI_CR_DMAEN) != 0U)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable the DMA transfer by clearing the DMAEN bit in the QSPI CR register */
      CLEAR_BIT(hqspi->Instance->CR, QUADSPI_CR_DMAEN);  /* 执行语句 */

      /* Abort DMA channel */
      hqspi->hdma->XferAbortCallback = QSPI_DMAAbortCplt;  /* 赋值或初始化 */
      if (HAL_DMA_Abort_IT(hqspi->hdma) != HAL_OK)      /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Change state of QSPI */
        hqspi->State = HAL_QSPI_STATE_READY;            /* 赋值或初始化 */

        /* Abort Complete callback */
#if (USE_HAL_QSPI_REGISTER_CALLBACKS == 1)              /* 条件编译 */
        hqspi->AbortCpltCallback(hqspi);                /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_QSPI_AbortCpltCallback(hqspi);              /* HAL库函数调用 */
#endif                                                  /* 条件编译结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Clear interrupt */
      __HAL_QSPI_CLEAR_FLAG(hqspi, QSPI_FLAG_TC);       /* HAL辅助宏/函数 */

      /* Enable the QSPI Transfer Complete Interrupt */
      __HAL_QSPI_ENABLE_IT(hqspi, QSPI_IT_TC);          /* HAL辅助宏/函数 */

      /* Configure QSPI: CR register with Abort request */
      SET_BIT(hqspi->Instance->CR, QUADSPI_CR_ABORT);   /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/** @brief Set QSPI timeout.
  * @param  hqspi : QSPI handle.
  * @param  Timeout : Timeout for the QSPI memory access.
  * @retval None
  */
void HAL_QSPI_SetTimeout(QSPI_HandleTypeDef *hqspi, uint32_t Timeout)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  hqspi->Timeout = Timeout;                             /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/** @brief Set QSPI Fifo threshold.
  * @param  hqspi : QSPI handle.
  * @param  Threshold : Threshold of the Fifo (value between 1 and 16).
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_QSPI_SetFifoThreshold(QSPI_HandleTypeDef *hqspi, uint32_t Threshold)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Process locked */
  __HAL_LOCK(hqspi);                                    /* HAL辅助宏/函数 */

  if(hqspi->State == HAL_QSPI_STATE_READY)              /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Synchronize init structure with new FIFO threshold value */
    hqspi->Init.FifoThreshold = Threshold;              /* 赋值或初始化 */

    /* Configure QSPI FIFO Threshold */
    MODIFY_REG(hqspi->Instance->CR, QUADSPI_CR_FTHRES,  /* 列表续行 */
               ((hqspi->Init.FifoThreshold - 1U) << QUADSPI_CR_FTHRES_Pos));  /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    status = HAL_BUSY;                                  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Process unlocked */
  __HAL_UNLOCK(hqspi);                                  /* HAL辅助宏/函数 */

  /* Return function status */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/** @brief Get QSPI Fifo threshold.
  * @param  hqspi : QSPI handle.
  * @retval Fifo threshold (value between 1 and 16)
  */
uint32_t HAL_QSPI_GetFifoThreshold(QSPI_HandleTypeDef *hqspi)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  return ((READ_BIT(hqspi->Instance->CR, QUADSPI_CR_FTHRES) >> QUADSPI_CR_FTHRES_Pos) + 1U);  /* 返回值 */
}                                                       /* 代码块结束 */

/** @brief  Set FlashID.
  * @param  hqspi : QSPI handle.
  * @param  FlashID : Index of the flash memory to be accessed.
  *                   This parameter can be a value of @ref QSPI_Flash_Select.
  * @note   The FlashID is ignored when dual flash mode is enabled.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_QSPI_SetFlashID(QSPI_HandleTypeDef *hqspi, uint32_t FlashID)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Check the parameter */
  assert_param(IS_QSPI_FLASH_ID(FlashID));              /* 执行语句 */

  /* Process locked */
  __HAL_LOCK(hqspi);                                    /* HAL辅助宏/函数 */

  if(hqspi->State == HAL_QSPI_STATE_READY)              /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Synchronize init structure with new FlashID value */
    hqspi->Init.FlashID = FlashID;                      /* 赋值或初始化 */

    /* Configure QSPI FlashID */
    MODIFY_REG(hqspi->Instance->CR, QUADSPI_CR_FSEL, FlashID);  /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    status = HAL_BUSY;                                  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Process unlocked */
  __HAL_UNLOCK(hqspi);                                  /* HAL辅助宏/函数 */

  /* Return function status */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup QSPI_Private_Functions QSPI Private Functions
  * @{
  */

/**
  * @brief  DMA QSPI receive process complete callback.
  * @param  hdma : DMA handle
  * @retval None
  */
static void QSPI_DMARxCplt(DMA_HandleTypeDef *hdma)     /* 静态函数 */
{                                                       /* 代码块开始 */
  QSPI_HandleTypeDef* hqspi = (QSPI_HandleTypeDef*)(hdma->Parent);  /* 赋值或初始化 */
  hqspi->RxXferCount = 0U;                              /* 赋值或初始化 */

  /* Enable the QSPI transfer complete Interrupt */
  __HAL_QSPI_ENABLE_IT(hqspi, QSPI_IT_TC);              /* HAL辅助宏/函数 */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA QSPI transmit process complete callback.
  * @param  hdma : DMA handle
  * @retval None
  */
static void QSPI_DMATxCplt(DMA_HandleTypeDef *hdma)     /* 静态函数 */
{                                                       /* 代码块开始 */
  QSPI_HandleTypeDef* hqspi = (QSPI_HandleTypeDef*)(hdma->Parent);  /* 赋值或初始化 */
  hqspi->TxXferCount = 0U;                              /* 赋值或初始化 */

  /* Enable the QSPI transfer complete Interrupt */
  __HAL_QSPI_ENABLE_IT(hqspi, QSPI_IT_TC);              /* HAL辅助宏/函数 */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA QSPI receive process half complete callback.
  * @param  hdma : DMA handle
  * @retval None
  */
static void QSPI_DMARxHalfCplt(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  QSPI_HandleTypeDef* hqspi = (QSPI_HandleTypeDef*)(hdma->Parent);  /* 赋值或初始化 */

#if (USE_HAL_QSPI_REGISTER_CALLBACKS == 1)              /* 条件编译 */
  hqspi->RxHalfCpltCallback(hqspi);                     /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_QSPI_RxHalfCpltCallback(hqspi);                   /* HAL库函数调用 */
#endif                                                  /* 条件编译结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA QSPI transmit process half complete callback.
  * @param  hdma : DMA handle
  * @retval None
  */
static void QSPI_DMATxHalfCplt(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  QSPI_HandleTypeDef* hqspi = (QSPI_HandleTypeDef*)(hdma->Parent);  /* 赋值或初始化 */

#if (USE_HAL_QSPI_REGISTER_CALLBACKS == 1)              /* 条件编译 */
  hqspi->TxHalfCpltCallback(hqspi);                     /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_QSPI_TxHalfCpltCallback(hqspi);                   /* HAL库函数调用 */
#endif                                                  /* 条件编译结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA QSPI communication error callback.
  * @param  hdma : DMA handle
  * @retval None
  */
static void QSPI_DMAError(DMA_HandleTypeDef *hdma)      /* 静态函数 */
{                                                       /* 代码块开始 */
  QSPI_HandleTypeDef* hqspi = ( QSPI_HandleTypeDef* )(hdma->Parent);  /* 赋值或初始化 */

  /* if DMA error is FIFO error ignore it */
  if(HAL_DMA_GetError(hdma) != HAL_DMA_ERROR_FE)        /* 条件判断 */
  {                                                     /* 代码块开始 */
  hqspi->RxXferCount = 0U;                              /* 赋值或初始化 */
  hqspi->TxXferCount = 0U;                              /* 赋值或初始化 */
  hqspi->ErrorCode   |= HAL_QSPI_ERROR_DMA;             /* 赋值或初始化 */

  /* Disable the DMA transfer by clearing the DMAEN bit in the QSPI CR register */
  CLEAR_BIT(hqspi->Instance->CR, QUADSPI_CR_DMAEN);     /* 执行语句 */

  /* Abort the QSPI */
  (void)HAL_QSPI_Abort_IT(hqspi);                       /* 执行语句 */

  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA QSPI abort complete callback.
  * @param  hdma : DMA handle
  * @retval None
  */
static void QSPI_DMAAbortCplt(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  QSPI_HandleTypeDef* hqspi = ( QSPI_HandleTypeDef* )(hdma->Parent);  /* 赋值或初始化 */

  hqspi->RxXferCount = 0U;                              /* 赋值或初始化 */
  hqspi->TxXferCount = 0U;                              /* 赋值或初始化 */

  if(hqspi->State == HAL_QSPI_STATE_ABORT)              /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* DMA Abort called by QSPI abort */
    /* Clear interrupt */
    __HAL_QSPI_CLEAR_FLAG(hqspi, QSPI_FLAG_TC);         /* HAL辅助宏/函数 */

    /* Enable the QSPI Transfer Complete Interrupt */
    __HAL_QSPI_ENABLE_IT(hqspi, QSPI_IT_TC);            /* HAL辅助宏/函数 */

    /* Configure QSPI: CR register with Abort request */
    SET_BIT(hqspi->Instance->CR, QUADSPI_CR_ABORT);     /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* DMA Abort called due to a transfer error interrupt */
    /* Change state of QSPI */
    hqspi->State = HAL_QSPI_STATE_READY;                /* 赋值或初始化 */

    /* Error callback */
#if (USE_HAL_QSPI_REGISTER_CALLBACKS == 1)              /* 条件编译 */
    hqspi->ErrorCallback(hqspi);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_QSPI_ErrorCallback(hqspi);                      /* HAL库函数调用 */
#endif                                                  /* 条件编译结束 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Wait for a flag state until timeout.
  * @param  hqspi : QSPI handle
  * @param  Flag : Flag checked
  * @param  State : Value of the flag expected
  * @param  Tickstart : Tick start value
  * @param  Timeout : Duration of the timeout
  * @retval HAL status
  */
static HAL_StatusTypeDef QSPI_WaitFlagStateUntilTimeout(QSPI_HandleTypeDef *hqspi, uint32_t Flag,  /* 静态变量/函数 */
                                                        FlagStatus State, uint32_t Tickstart, uint32_t Timeout)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Wait until flag is in expected state */
  while((__HAL_QSPI_GET_FLAG(hqspi, Flag)) != State)    /* while循环 */
  {                                                     /* 代码块开始 */
    /* Check for the Timeout */
    if (Timeout != HAL_MAX_DELAY)                       /* 条件判断 */
    {                                                   /* 代码块开始 */
      if(((HAL_GetTick() - Tickstart) > Timeout) || (Timeout == 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        hqspi->State     = HAL_QSPI_STATE_ERROR;        /* 赋值或初始化 */
        hqspi->ErrorCode |= HAL_QSPI_ERROR_TIMEOUT;     /* 赋值或初始化 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configure the communication registers.
  * @param  hqspi : QSPI handle
  * @param  cmd : structure that contains the command configuration information
  * @param  FunctionalMode : functional mode to configured
  *           This parameter can be one of the following values:
  *            @arg QSPI_FUNCTIONAL_MODE_INDIRECT_WRITE: Indirect write mode
  *            @arg QSPI_FUNCTIONAL_MODE_INDIRECT_READ: Indirect read mode
  *            @arg QSPI_FUNCTIONAL_MODE_AUTO_POLLING: Automatic polling mode
  *            @arg QSPI_FUNCTIONAL_MODE_MEMORY_MAPPED: Memory-mapped mode
  * @retval None
  */
static void QSPI_Config(QSPI_HandleTypeDef *hqspi, QSPI_CommandTypeDef *cmd, uint32_t FunctionalMode)  /* 静态函数 */
{                                                       /* 代码块开始 */
  assert_param(IS_QSPI_FUNCTIONAL_MODE(FunctionalMode));  /* 执行语句 */

  if ((cmd->DataMode != QSPI_DATA_NONE) && (FunctionalMode != QSPI_FUNCTIONAL_MODE_MEMORY_MAPPED))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Configure QSPI: DLR register with the number of data to read or write */
    WRITE_REG(hqspi->Instance->DLR, (cmd->NbData - 1U));  /* 执行语句 */
  }                                                     /* 代码块结束 */

  if (cmd->InstructionMode != QSPI_INSTRUCTION_NONE)    /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (cmd->AlternateByteMode != QSPI_ALTERNATE_BYTES_NONE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Configure QSPI: ABR register with alternate bytes value */
      WRITE_REG(hqspi->Instance->ABR, cmd->AlternateBytes);  /* 执行语句 */

      if (cmd->AddressMode != QSPI_ADDRESS_NONE)        /* 条件判断 */
      {                                                 /* 代码块开始 */
        /*---- Command with instruction, address and alternate bytes ----*/
        /* Configure QSPI: CCR register with all communications parameters */
        WRITE_REG(hqspi->Instance->CCR, (cmd->DdrMode | cmd->DdrHoldHalfCycle | cmd->SIOOMode |  /* 源代码 */
                                         cmd->DataMode | (cmd->DummyCycles << QUADSPI_CCR_DCYC_Pos) |  /* 源代码 */
                                         cmd->AlternateBytesSize | cmd->AlternateByteMode |  /* 源代码 */
                                         cmd->AddressSize | cmd->AddressMode | cmd->InstructionMode |  /* 源代码 */
                                         cmd->Instruction | FunctionalMode));  /* 执行语句 */

        if (FunctionalMode != QSPI_FUNCTIONAL_MODE_MEMORY_MAPPED)  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Configure QSPI: AR register with address value */
          WRITE_REG(hqspi->Instance->AR, cmd->Address);  /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /*---- Command with instruction and alternate bytes ----*/
        /* Configure QSPI: CCR register with all communications parameters */
        WRITE_REG(hqspi->Instance->CCR, (cmd->DdrMode | cmd->DdrHoldHalfCycle | cmd->SIOOMode |  /* 源代码 */
                                         cmd->DataMode | (cmd->DummyCycles << QUADSPI_CCR_DCYC_Pos) |  /* 源代码 */
                                         cmd->AlternateBytesSize | cmd->AlternateByteMode |  /* 源代码 */
                                         cmd->AddressMode | cmd->InstructionMode |  /* 源代码 */
                                         cmd->Instruction | FunctionalMode));  /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      if (cmd->AddressMode != QSPI_ADDRESS_NONE)        /* 条件判断 */
      {                                                 /* 代码块开始 */
        /*---- Command with instruction and address ----*/
        /* Configure QSPI: CCR register with all communications parameters */
        WRITE_REG(hqspi->Instance->CCR, (cmd->DdrMode | cmd->DdrHoldHalfCycle | cmd->SIOOMode |  /* 源代码 */
                                         cmd->DataMode | (cmd->DummyCycles << QUADSPI_CCR_DCYC_Pos) |  /* 源代码 */
                                         cmd->AlternateByteMode | cmd->AddressSize | cmd->AddressMode |  /* 源代码 */
                                         cmd->InstructionMode | cmd->Instruction | FunctionalMode));  /* 执行语句 */

        if (FunctionalMode != QSPI_FUNCTIONAL_MODE_MEMORY_MAPPED)  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Configure QSPI: AR register with address value */
          WRITE_REG(hqspi->Instance->AR, cmd->Address);  /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /*---- Command with only instruction ----*/
        /* Configure QSPI: CCR register with all communications parameters */
        WRITE_REG(hqspi->Instance->CCR, (cmd->DdrMode | cmd->DdrHoldHalfCycle | cmd->SIOOMode |  /* 源代码 */
                                         cmd->DataMode | (cmd->DummyCycles << QUADSPI_CCR_DCYC_Pos) |  /* 源代码 */
                                         cmd->AlternateByteMode | cmd->AddressMode |  /* 源代码 */
                                         cmd->InstructionMode | cmd->Instruction | FunctionalMode));  /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    if (cmd->AlternateByteMode != QSPI_ALTERNATE_BYTES_NONE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Configure QSPI: ABR register with alternate bytes value */
      WRITE_REG(hqspi->Instance->ABR, cmd->AlternateBytes);  /* 执行语句 */

      if (cmd->AddressMode != QSPI_ADDRESS_NONE)        /* 条件判断 */
      {                                                 /* 代码块开始 */
        /*---- Command with address and alternate bytes ----*/
        /* Configure QSPI: CCR register with all communications parameters */
        WRITE_REG(hqspi->Instance->CCR, (cmd->DdrMode | cmd->DdrHoldHalfCycle | cmd->SIOOMode |  /* 源代码 */
                                         cmd->DataMode | (cmd->DummyCycles << QUADSPI_CCR_DCYC_Pos) |  /* 源代码 */
                                         cmd->AlternateBytesSize | cmd->AlternateByteMode |  /* 源代码 */
                                         cmd->AddressSize | cmd->AddressMode |  /* 源代码 */
                                         cmd->InstructionMode | FunctionalMode));  /* 执行语句 */

        if (FunctionalMode != QSPI_FUNCTIONAL_MODE_MEMORY_MAPPED)  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Configure QSPI: AR register with address value */
          WRITE_REG(hqspi->Instance->AR, cmd->Address);  /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /*---- Command with only alternate bytes ----*/
        /* Configure QSPI: CCR register with all communications parameters */
        WRITE_REG(hqspi->Instance->CCR, (cmd->DdrMode | cmd->DdrHoldHalfCycle | cmd->SIOOMode |  /* 源代码 */
                                         cmd->DataMode | (cmd->DummyCycles << QUADSPI_CCR_DCYC_Pos) |  /* 源代码 */
                                         cmd->AlternateBytesSize | cmd->AlternateByteMode |  /* 源代码 */
                                         cmd->AddressMode | cmd->InstructionMode | FunctionalMode));  /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      if (cmd->AddressMode != QSPI_ADDRESS_NONE)        /* 条件判断 */
      {                                                 /* 代码块开始 */
        /*---- Command with only address ----*/
        /* Configure QSPI: CCR register with all communications parameters */
        WRITE_REG(hqspi->Instance->CCR, (cmd->DdrMode | cmd->DdrHoldHalfCycle | cmd->SIOOMode |  /* 源代码 */
                                         cmd->DataMode | (cmd->DummyCycles << QUADSPI_CCR_DCYC_Pos) |  /* 源代码 */
                                         cmd->AlternateByteMode | cmd->AddressSize |  /* 源代码 */
                                         cmd->AddressMode | cmd->InstructionMode | FunctionalMode));  /* 执行语句 */

        if (FunctionalMode != QSPI_FUNCTIONAL_MODE_MEMORY_MAPPED)  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Configure QSPI: AR register with address value */
          WRITE_REG(hqspi->Instance->AR, cmd->Address);  /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /*---- Command with only data phase ----*/
        if (cmd->DataMode != QSPI_DATA_NONE)            /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Configure QSPI: CCR register with all communications parameters */
          WRITE_REG(hqspi->Instance->CCR, (cmd->DdrMode | cmd->DdrHoldHalfCycle | cmd->SIOOMode |  /* 源代码 */
                                           cmd->DataMode | (cmd->DummyCycles << QUADSPI_CCR_DCYC_Pos) |  /* 源代码 */
                                           cmd->AlternateByteMode | cmd->AddressMode |  /* 源代码 */
                                           cmd->InstructionMode | FunctionalMode));  /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/**
  * @}
  */

#endif /* HAL_QSPI_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

#endif /* defined(QUADSPI) || defined(QUADSPI1) || defined(QUADSPI2) */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
