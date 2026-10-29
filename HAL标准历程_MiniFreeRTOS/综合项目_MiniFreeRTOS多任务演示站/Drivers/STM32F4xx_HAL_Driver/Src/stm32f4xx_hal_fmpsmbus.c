/* 文件: stm32f4xx_hal_fmpsmbus.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_fmpsmbus.c
  * @author  MCD Application Team
  * @brief   FMPSMBUS HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the System Management Bus (SMBus) peripheral,
  *          based on I2C principles of operation :
  *           + Initialization and de-initialization functions
  *           + IO operation functions
  *           + Peripheral State and Errors functions
  *
  @verbatim
  ==============================================================================
                        ##### How to use this driver #####
  ==============================================================================
    [..]
    The FMPSMBUS HAL driver can be used as follows:

    (#) Declare a FMPSMBUS_HandleTypeDef handle structure, for example:
        FMPSMBUS_HandleTypeDef  hfmpsmbus;

    (#)Initialize the FMPSMBUS low level resources by implementing the @ref HAL_FMPSMBUS_MspInit() API:
        (##) Enable the FMPSMBUSx interface clock
        (##) FMPSMBUS pins configuration
            (+++) Enable the clock for the FMPSMBUS GPIOs
            (+++) Configure FMPSMBUS pins as alternate function open-drain
        (##) NVIC configuration if you need to use interrupt process
            (+++) Configure the FMPSMBUSx interrupt priority
            (+++) Enable the NVIC FMPSMBUS IRQ Channel

    (#) Configure the Communication Clock Timing, Bus Timeout, Own Address1, Master Addressing mode,
        Dual Addressing mode, Own Address2, Own Address2 Mask, General call, Nostretch mode,
        Peripheral mode and Packet Error Check mode in the hfmpsmbus Init structure.

    (#) Initialize the FMPSMBUS registers by calling the @ref HAL_FMPSMBUS_Init() API:
        (++) These API's configures also the low level Hardware GPIO, CLOCK, CORTEX...etc)
             by calling the customized @ref HAL_FMPSMBUS_MspInit(&hfmpsmbus) API.

    (#) To check if target device is ready for communication, use the function @ref HAL_FMPSMBUS_IsDeviceReady()

    (#) For FMPSMBUS IO operations, only one mode of operations is available within this driver

    *** Interrupt mode IO operation ***
    ===================================
    [..]
      (+) Transmit in master/host FMPSMBUS mode an amount of data in non-blocking mode using @ref HAL_FMPSMBUS_Master_Transmit_IT()
      (++) At transmission end of transfer @ref HAL_FMPSMBUS_MasterTxCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_FMPSMBUS_MasterTxCpltCallback()
      (+) Receive in master/host FMPSMBUS mode an amount of data in non-blocking mode using @ref HAL_FMPSMBUS_Master_Receive_IT()
      (++) At reception end of transfer @ref HAL_FMPSMBUS_MasterRxCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_FMPSMBUS_MasterRxCpltCallback()
      (+) Abort a master/host FMPSMBUS process communication with Interrupt using @ref HAL_FMPSMBUS_Master_Abort_IT()
      (++) The associated previous transfer callback is called at the end of abort process
      (++) mean @ref HAL_FMPSMBUS_MasterTxCpltCallback() in case of previous state was master transmit
      (++) mean @ref HAL_FMPSMBUS_MasterRxCpltCallback() in case of previous state was master receive
      (+) Enable/disable the Address listen mode in slave/device or host/slave FMPSMBUS mode
           using @ref HAL_FMPSMBUS_EnableListen_IT() @ref HAL_FMPSMBUS_DisableListen_IT()
      (++) When address slave/device FMPSMBUS match, @ref HAL_FMPSMBUS_AddrCallback() is executed and user can
           add his own code to check the Address Match Code and the transmission direction request by master/host (Write/Read).
      (++) At Listen mode end @ref HAL_FMPSMBUS_ListenCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_FMPSMBUS_ListenCpltCallback()
      (+) Transmit in slave/device FMPSMBUS mode an amount of data in non-blocking mode using @ref HAL_FMPSMBUS_Slave_Transmit_IT()
      (++) At transmission end of transfer @ref HAL_FMPSMBUS_SlaveTxCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_FMPSMBUS_SlaveTxCpltCallback()
      (+) Receive in slave/device FMPSMBUS mode an amount of data in non-blocking mode using @ref HAL_FMPSMBUS_Slave_Receive_IT()
      (++) At reception end of transfer @ref HAL_FMPSMBUS_SlaveRxCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_FMPSMBUS_SlaveRxCpltCallback()
      (+) Enable/Disable the FMPSMBUS alert mode using @ref HAL_FMPSMBUS_EnableAlert_IT() @ref HAL_FMPSMBUS_DisableAlert_IT()
      (++) When FMPSMBUS Alert is generated @ref HAL_FMPSMBUS_ErrorCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_FMPSMBUS_ErrorCallback()
           to check the Alert Error Code using function @ref HAL_FMPSMBUS_GetError()
      (+) Get HAL state machine or error values using @ref HAL_FMPSMBUS_GetState() or @ref HAL_FMPSMBUS_GetError()
      (+) In case of transfer Error, @ref HAL_FMPSMBUS_ErrorCallback() function is executed and user can
           add his own code by customization of function pointer @ref HAL_FMPSMBUS_ErrorCallback()
           to check the Error Code using function @ref HAL_FMPSMBUS_GetError()

     *** FMPSMBUS HAL driver macros list ***
     ==================================
     [..]
       Below the list of most used macros in FMPSMBUS HAL driver.

      (+) @ref __HAL_FMPSMBUS_ENABLE:      Enable the FMPSMBUS peripheral
      (+) @ref __HAL_FMPSMBUS_DISABLE:     Disable the FMPSMBUS peripheral
      (+) @ref __HAL_FMPSMBUS_GET_FLAG:    Check whether the specified FMPSMBUS flag is set or not
      (+) @ref __HAL_FMPSMBUS_CLEAR_FLAG:  Clear the specified FMPSMBUS pending flag
      (+) @ref __HAL_FMPSMBUS_ENABLE_IT:   Enable the specified FMPSMBUS interrupt
      (+) @ref __HAL_FMPSMBUS_DISABLE_IT:  Disable the specified FMPSMBUS interrupt

     *** Callback registration ***
     =============================================
    [..]
     The compilation flag USE_HAL_FMPSMBUS_REGISTER_CALLBACKS when set to 1
     allows the user to configure dynamically the driver callbacks.
     Use Functions @ref HAL_FMPSMBUS_RegisterCallback() or @ref HAL_FMPSMBUS_RegisterAddrCallback()
     to register an interrupt callback.
    [..]
     Function @ref HAL_FMPSMBUS_RegisterCallback() allows to register following callbacks:
       (+) MasterTxCpltCallback : callback for Master transmission end of transfer.
       (+) MasterRxCpltCallback : callback for Master reception end of transfer.
       (+) SlaveTxCpltCallback  : callback for Slave transmission end of transfer.
       (+) SlaveRxCpltCallback  : callback for Slave reception end of transfer.
       (+) ListenCpltCallback   : callback for end of listen mode.
       (+) ErrorCallback        : callback for error detection.
       (+) MspInitCallback      : callback for Msp Init.
       (+) MspDeInitCallback    : callback for Msp DeInit.
     This function takes as parameters the HAL peripheral handle, the Callback ID
     and a pointer to the user callback function.
    [..]
     For specific callback AddrCallback use dedicated register callbacks : @ref HAL_FMPSMBUS_RegisterAddrCallback.
    [..]
     Use function @ref HAL_FMPSMBUS_UnRegisterCallback to reset a callback to the default
     weak function.
     @ref HAL_FMPSMBUS_UnRegisterCallback takes as parameters the HAL peripheral handle,
     and the Callback ID.
     This function allows to reset following callbacks:
       (+) MasterTxCpltCallback : callback for Master transmission end of transfer.
       (+) MasterRxCpltCallback : callback for Master reception end of transfer.
       (+) SlaveTxCpltCallback  : callback for Slave transmission end of transfer.
       (+) SlaveRxCpltCallback  : callback for Slave reception end of transfer.
       (+) ListenCpltCallback   : callback for end of listen mode.
       (+) ErrorCallback        : callback for error detection.
       (+) MspInitCallback      : callback for Msp Init.
       (+) MspDeInitCallback    : callback for Msp DeInit.
    [..]
     For callback AddrCallback use dedicated register callbacks : @ref HAL_FMPSMBUS_UnRegisterAddrCallback.
    [..]
     By default, after the @ref HAL_FMPSMBUS_Init() and when the state is @ref HAL_FMPI2C_STATE_RESET
     all callbacks are set to the corresponding weak functions:
     examples @ref HAL_FMPSMBUS_MasterTxCpltCallback(), @ref HAL_FMPSMBUS_MasterRxCpltCallback().
     Exception done for MspInit and MspDeInit functions that are
     reset to the legacy weak functions in the @ref HAL_FMPSMBUS_Init()/ @ref HAL_FMPSMBUS_DeInit() only when
     these callbacks are null (not registered beforehand).
     If MspInit or MspDeInit are not null, the @ref HAL_FMPSMBUS_Init()/ @ref HAL_FMPSMBUS_DeInit()
     keep and use the user MspInit/MspDeInit callbacks (registered beforehand) whatever the state.
    [..]
     Callbacks can be registered/unregistered in @ref HAL_FMPI2C_STATE_READY state only.
     Exception done MspInit/MspDeInit functions that can be registered/unregistered
     in @ref HAL_FMPI2C_STATE_READY or @ref HAL_FMPI2C_STATE_RESET state,
     thus registered (user) MspInit/DeInit callbacks can be used during the Init/DeInit.
     Then, the user first registers the MspInit/MspDeInit user callbacks
     using @ref HAL_FMPSMBUS_RegisterCallback() before calling @ref HAL_FMPSMBUS_DeInit()
     or @ref HAL_FMPSMBUS_Init() function.
    [..]
     When the compilation flag USE_HAL_FMPSMBUS_REGISTER_CALLBACKS is set to 0 or
     not defined, the callback registration feature is not available and all callbacks
     are set to the corresponding weak functions.

     [..]
       (@) You can refer to the FMPSMBUS HAL driver header file for more useful macros

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

/** @defgroup FMPSMBUS FMPSMBUS
  * @brief FMPSMBUS HAL module driver
  * @{
  */

#ifdef HAL_FMPSMBUS_MODULE_ENABLED                      /* 条件编译 */

#if defined(FMPI2C_CR1_PE)                              /* 条件编译 */
/* Private typedef -----------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/** @defgroup FMPSMBUS_Private_Define FMPSMBUS Private Constants
  * @{
  */
#define TIMING_CLEAR_MASK   (0xF0FFFFFFUL)     /*!< FMPSMBUS TIMING clear register Mask */
#define HAL_TIMEOUT_ADDR    (10000U)           /*!< 10 s  */
#define HAL_TIMEOUT_BUSY    (25U)              /*!< 25 ms */
#define HAL_TIMEOUT_DIR     (25U)              /*!< 25 ms */
#define HAL_TIMEOUT_RXNE    (25U)              /*!< 25 ms */
#define HAL_TIMEOUT_STOPF   (25U)              /*!< 25 ms */
#define HAL_TIMEOUT_TC      (25U)              /*!< 25 ms */
#define HAL_TIMEOUT_TCR     (25U)              /*!< 25 ms */
#define HAL_TIMEOUT_TXIS    (25U)              /*!< 25 ms */
#define MAX_NBYTE_SIZE      255U                        /* 定义宏 */
/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/** @addtogroup FMPSMBUS_Private_Functions FMPSMBUS Private Functions
  * @{
  */
static HAL_StatusTypeDef FMPSMBUS_WaitOnFlagUntilTimeout(FMPSMBUS_HandleTypeDef *hfmpsmbus, uint32_t Flag, FlagStatus Status,  /* 静态变量/函数 */
                                                      uint32_t Timeout);  /* 32位无符号变量/函数 */

static void FMPSMBUS_Enable_IRQ(FMPSMBUS_HandleTypeDef *hfmpsmbus, uint32_t InterruptRequest);  /* 静态函数 */
static void FMPSMBUS_Disable_IRQ(FMPSMBUS_HandleTypeDef *hfmpsmbus, uint32_t InterruptRequest);  /* 静态函数 */
static HAL_StatusTypeDef FMPSMBUS_Master_ISR(FMPSMBUS_HandleTypeDef *hfmpsmbus, uint32_t StatusFlags);  /* 静态变量/函数 */
static HAL_StatusTypeDef FMPSMBUS_Slave_ISR(FMPSMBUS_HandleTypeDef *hfmpsmbus, uint32_t StatusFlags);  /* 静态变量/函数 */

static void FMPSMBUS_ConvertOtherXferOptions(FMPSMBUS_HandleTypeDef *hfmpsmbus);  /* 静态函数 */

static void FMPSMBUS_ITErrorHandler(FMPSMBUS_HandleTypeDef *hfmpsmbus);  /* 静态函数 */

static void FMPSMBUS_TransferConfig(FMPSMBUS_HandleTypeDef *hfmpsmbus,  uint16_t DevAddress, uint8_t Size, uint32_t Mode,  /* 静态函数 */
                                 uint32_t Request);     /* 32位无符号变量/函数 */
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/

/** @defgroup FMPSMBUS_Exported_Functions FMPSMBUS Exported Functions
  * @{
  */

/** @defgroup FMPSMBUS_Exported_Functions_Group1 Initialization and de-initialization functions
  *  @brief    Initialization and Configuration functions
  *
@verbatim
 ===============================================================================
              ##### Initialization and de-initialization functions #####
 ===============================================================================
    [..]  This subsection provides a set of functions allowing to initialize and
          deinitialize the FMPSMBUSx peripheral:

      (+) User must Implement HAL_FMPSMBUS_MspInit() function in which he configures
          all related peripherals resources (CLOCK, GPIO, IT and NVIC ).

      (+) Call the function HAL_FMPSMBUS_Init() to configure the selected device with
          the selected configuration:
        (++) Clock Timing
        (++) Bus Timeout
        (++) Analog Filer mode
        (++) Own Address 1
        (++) Addressing mode (Master, Slave)
        (++) Dual Addressing mode
        (++) Own Address 2
        (++) Own Address 2 Mask
        (++) General call mode
        (++) Nostretch mode
        (++) Packet Error Check mode
        (++) Peripheral mode


      (+) Call the function HAL_FMPSMBUS_DeInit() to restore the default configuration
          of the selected FMPSMBUSx peripheral.

      (+) Enable/Disable Analog/Digital filters with HAL_FMPSMBUS_ConfigAnalogFilter() and
          HAL_FMPSMBUS_ConfigDigitalFilter().

@endverbatim
  * @{
  */

/**
  * @brief  Initialize the FMPSMBUS according to the specified parameters
  *         in the FMPSMBUS_InitTypeDef and initialize the associated handle.
  * @param  hfmpsmbus Pointer to a FMPSMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPSMBUS.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPSMBUS_Init(FMPSMBUS_HandleTypeDef *hfmpsmbus)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the FMPSMBUS handle allocation */
  if (hfmpsmbus == NULL)                                /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the parameters */
  assert_param(IS_FMPSMBUS_ALL_INSTANCE(hfmpsmbus->Instance));  /* 执行语句 */
  assert_param(IS_FMPSMBUS_ANALOG_FILTER(hfmpsmbus->Init.AnalogFilter));  /* 执行语句 */
  assert_param(IS_FMPSMBUS_OWN_ADDRESS1(hfmpsmbus->Init.OwnAddress1));  /* 执行语句 */
  assert_param(IS_FMPSMBUS_ADDRESSING_MODE(hfmpsmbus->Init.AddressingMode));  /* 执行语句 */
  assert_param(IS_FMPSMBUS_DUAL_ADDRESS(hfmpsmbus->Init.DualAddressMode));  /* 执行语句 */
  assert_param(IS_FMPSMBUS_OWN_ADDRESS2(hfmpsmbus->Init.OwnAddress2));  /* 执行语句 */
  assert_param(IS_FMPSMBUS_OWN_ADDRESS2_MASK(hfmpsmbus->Init.OwnAddress2Masks));  /* 执行语句 */
  assert_param(IS_FMPSMBUS_GENERAL_CALL(hfmpsmbus->Init.GeneralCallMode));  /* 执行语句 */
  assert_param(IS_FMPSMBUS_NO_STRETCH(hfmpsmbus->Init.NoStretchMode));  /* 执行语句 */
  assert_param(IS_FMPSMBUS_PEC(hfmpsmbus->Init.PacketErrorCheckMode));  /* 执行语句 */
  assert_param(IS_FMPSMBUS_PERIPHERAL_MODE(hfmpsmbus->Init.PeripheralMode));  /* 执行语句 */

  if (hfmpsmbus->State == HAL_FMPSMBUS_STATE_RESET)     /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Allocate lock resource and initialize it */
    hfmpsmbus->Lock = HAL_UNLOCKED;                     /* 赋值或初始化 */

#if (USE_HAL_FMPSMBUS_REGISTER_CALLBACKS == 1)          /* 条件编译 */
    hfmpsmbus->MasterTxCpltCallback = HAL_FMPSMBUS_MasterTxCpltCallback; /* Legacy weak MasterTxCpltCallback */
    hfmpsmbus->MasterRxCpltCallback = HAL_FMPSMBUS_MasterRxCpltCallback; /* Legacy weak MasterRxCpltCallback */
    hfmpsmbus->SlaveTxCpltCallback  = HAL_FMPSMBUS_SlaveTxCpltCallback;  /* Legacy weak SlaveTxCpltCallback  */
    hfmpsmbus->SlaveRxCpltCallback  = HAL_FMPSMBUS_SlaveRxCpltCallback;  /* Legacy weak SlaveRxCpltCallback  */
    hfmpsmbus->ListenCpltCallback   = HAL_FMPSMBUS_ListenCpltCallback;   /* Legacy weak ListenCpltCallback   */
    hfmpsmbus->ErrorCallback        = HAL_FMPSMBUS_ErrorCallback;        /* Legacy weak ErrorCallback        */
    hfmpsmbus->AddrCallback         = HAL_FMPSMBUS_AddrCallback;         /* Legacy weak AddrCallback         */

    if (hfmpsmbus->MspInitCallback == NULL)             /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpsmbus->MspInitCallback = HAL_FMPSMBUS_MspInit; /* Legacy weak MspInit  */
    }                                                   /* 代码块结束 */

    /* Init the low level hardware : GPIO, CLOCK, CORTEX...etc */
    hfmpsmbus->MspInitCallback(hfmpsmbus);              /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /* Init the low level hardware : GPIO, CLOCK, NVIC */
    HAL_FMPSMBUS_MspInit(hfmpsmbus);                    /* HAL库函数调用 */
#endif /* USE_HAL_FMPSMBUS_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */

  hfmpsmbus->State = HAL_FMPSMBUS_STATE_BUSY;           /* 赋值或初始化 */

  /* Disable the selected FMPSMBUS peripheral */
  __HAL_FMPSMBUS_DISABLE(hfmpsmbus);                    /* HAL辅助宏/函数 */

  /*---------------------------- FMPSMBUSx TIMINGR Configuration ------------------------*/
  /* Configure FMPSMBUSx: Frequency range */
  hfmpsmbus->Instance->TIMINGR = hfmpsmbus->Init.Timing & TIMING_CLEAR_MASK;  /* 赋值或初始化 */

  /*---------------------------- FMPSMBUSx TIMEOUTR Configuration ------------------------*/
  /* Configure FMPSMBUSx: Bus Timeout  */
  hfmpsmbus->Instance->TIMEOUTR &= ~FMPI2C_TIMEOUTR_TIMOUTEN;  /* 赋值或初始化 */
  hfmpsmbus->Instance->TIMEOUTR &= ~FMPI2C_TIMEOUTR_TEXTEN;  /* 赋值或初始化 */
  hfmpsmbus->Instance->TIMEOUTR = hfmpsmbus->Init.SMBusTimeout;  /* 赋值或初始化 */

  /*---------------------------- FMPSMBUSx OAR1 Configuration -----------------------*/
  /* Configure FMPSMBUSx: Own Address1 and ack own address1 mode */
  hfmpsmbus->Instance->OAR1 &= ~FMPI2C_OAR1_OA1EN;      /* 赋值或初始化 */

  if (hfmpsmbus->Init.OwnAddress1 != 0UL)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (hfmpsmbus->Init.AddressingMode == FMPSMBUS_ADDRESSINGMODE_7BIT)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpsmbus->Instance->OAR1 = (FMPI2C_OAR1_OA1EN | hfmpsmbus->Init.OwnAddress1);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else /* FMPSMBUS_ADDRESSINGMODE_10BIT */
    {                                                   /* 代码块开始 */
      hfmpsmbus->Instance->OAR1 = (FMPI2C_OAR1_OA1EN | FMPI2C_OAR1_OA1MODE | hfmpsmbus->Init.OwnAddress1);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /*---------------------------- FMPSMBUSx CR2 Configuration ------------------------*/
  /* Configure FMPSMBUSx: Addressing Master mode */
  if (hfmpsmbus->Init.AddressingMode == FMPSMBUS_ADDRESSINGMODE_10BIT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hfmpsmbus->Instance->CR2 = (FMPI2C_CR2_ADD10);      /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  /* Enable the AUTOEND by default, and enable NACK (should be disable only during Slave process) */
  /* AUTOEND and NACK bit will be manage during Transfer process */
  hfmpsmbus->Instance->CR2 |= (FMPI2C_CR2_AUTOEND | FMPI2C_CR2_NACK);  /* 赋值或初始化 */

  /*---------------------------- FMPSMBUSx OAR2 Configuration -----------------------*/
  /* Configure FMPSMBUSx: Dual mode and Own Address2 */
  hfmpsmbus->Instance->OAR2 = (hfmpsmbus->Init.DualAddressMode | hfmpsmbus->Init.OwnAddress2 | (hfmpsmbus->Init.OwnAddress2Masks << 8U));  /* 赋值或初始化 */

  /*---------------------------- FMPSMBUSx CR1 Configuration ------------------------*/
  /* Configure FMPSMBUSx: Generalcall and NoStretch mode */
  hfmpsmbus->Instance->CR1 = (hfmpsmbus->Init.GeneralCallMode | hfmpsmbus->Init.NoStretchMode | hfmpsmbus->Init.PacketErrorCheckMode | hfmpsmbus->Init.PeripheralMode | hfmpsmbus->Init.AnalogFilter);  /* 赋值或初始化 */

  /* Enable Slave Byte Control only in case of Packet Error Check is enabled and FMPSMBUS Peripheral is set in Slave mode */
  if ((hfmpsmbus->Init.PacketErrorCheckMode == FMPSMBUS_PEC_ENABLE)  /* 条件判断 */
      && ((hfmpsmbus->Init.PeripheralMode == FMPSMBUS_PERIPHERAL_MODE_FMPSMBUS_SLAVE) || (hfmpsmbus->Init.PeripheralMode == FMPSMBUS_PERIPHERAL_MODE_FMPSMBUS_SLAVE_ARP)))  /* 源代码 */
  {                                                     /* 代码块开始 */
    hfmpsmbus->Instance->CR1 |= FMPI2C_CR1_SBC;         /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Enable the selected FMPSMBUS peripheral */
  __HAL_FMPSMBUS_ENABLE(hfmpsmbus);                     /* HAL辅助宏/函数 */

  hfmpsmbus->ErrorCode = HAL_FMPSMBUS_ERROR_NONE;       /* 赋值或初始化 */
  hfmpsmbus->PreviousState = HAL_FMPSMBUS_STATE_READY;  /* 赋值或初始化 */
  hfmpsmbus->State = HAL_FMPSMBUS_STATE_READY;          /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  DeInitialize the FMPSMBUS peripheral.
  * @param  hfmpsmbus Pointer to a FMPSMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPSMBUS.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPSMBUS_DeInit(FMPSMBUS_HandleTypeDef *hfmpsmbus)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the FMPSMBUS handle allocation */
  if (hfmpsmbus == NULL)                                /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the parameters */
  assert_param(IS_FMPSMBUS_ALL_INSTANCE(hfmpsmbus->Instance));  /* 执行语句 */

  hfmpsmbus->State = HAL_FMPSMBUS_STATE_BUSY;           /* 赋值或初始化 */

  /* Disable the FMPSMBUS Peripheral Clock */
  __HAL_FMPSMBUS_DISABLE(hfmpsmbus);                    /* HAL辅助宏/函数 */

#if (USE_HAL_FMPSMBUS_REGISTER_CALLBACKS == 1)          /* 条件编译 */
  if (hfmpsmbus->MspDeInitCallback == NULL)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    hfmpsmbus->MspDeInitCallback = HAL_FMPSMBUS_MspDeInit; /* Legacy weak MspDeInit  */
  }                                                     /* 代码块结束 */

  /* DeInit the low level hardware: GPIO, CLOCK, NVIC */
  hfmpsmbus->MspDeInitCallback(hfmpsmbus);              /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* DeInit the low level hardware: GPIO, CLOCK, NVIC */
  HAL_FMPSMBUS_MspDeInit(hfmpsmbus);                    /* HAL库函数调用 */
#endif /* USE_HAL_FMPSMBUS_REGISTER_CALLBACKS */

  hfmpsmbus->ErrorCode = HAL_FMPSMBUS_ERROR_NONE;       /* 赋值或初始化 */
  hfmpsmbus->PreviousState =  HAL_FMPSMBUS_STATE_RESET;  /* 赋值或初始化 */
  hfmpsmbus->State = HAL_FMPSMBUS_STATE_RESET;          /* 赋值或初始化 */

  /* Release Lock */
  __HAL_UNLOCK(hfmpsmbus);                              /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief Initialize the FMPSMBUS MSP.
  * @param  hfmpsmbus Pointer to a FMPSMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPSMBUS.
  * @retval None
  */
__weak void HAL_FMPSMBUS_MspInit(FMPSMBUS_HandleTypeDef *hfmpsmbus)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hfmpsmbus);                                    /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_FMPSMBUS_MspInit could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief DeInitialize the FMPSMBUS MSP.
  * @param  hfmpsmbus Pointer to a FMPSMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPSMBUS.
  * @retval None
  */
__weak void HAL_FMPSMBUS_MspDeInit(FMPSMBUS_HandleTypeDef *hfmpsmbus)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hfmpsmbus);                                    /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_FMPSMBUS_MspDeInit could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Configure Analog noise filter.
  * @param  hfmpsmbus Pointer to a FMPSMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPSMBUS.
  * @param  AnalogFilter This parameter can be one of the following values:
  *         @arg @ref FMPSMBUS_ANALOGFILTER_ENABLE
  *         @arg @ref FMPSMBUS_ANALOGFILTER_DISABLE
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPSMBUS_ConfigAnalogFilter(FMPSMBUS_HandleTypeDef *hfmpsmbus, uint32_t AnalogFilter)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_FMPSMBUS_ALL_INSTANCE(hfmpsmbus->Instance));  /* 执行语句 */
  assert_param(IS_FMPSMBUS_ANALOG_FILTER(AnalogFilter));  /* 执行语句 */

  if (hfmpsmbus->State == HAL_FMPSMBUS_STATE_READY)     /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Process Locked */
    __HAL_LOCK(hfmpsmbus);                              /* HAL辅助宏/函数 */

    hfmpsmbus->State = HAL_FMPSMBUS_STATE_BUSY;         /* 赋值或初始化 */

    /* Disable the selected FMPSMBUS peripheral */
    __HAL_FMPSMBUS_DISABLE(hfmpsmbus);                  /* HAL辅助宏/函数 */

    /* Reset ANOFF bit */
    hfmpsmbus->Instance->CR1 &= ~(FMPI2C_CR1_ANFOFF);   /* 赋值或初始化 */

    /* Set analog filter bit*/
    hfmpsmbus->Instance->CR1 |= AnalogFilter;           /* 赋值或初始化 */

    __HAL_FMPSMBUS_ENABLE(hfmpsmbus);                   /* HAL辅助宏/函数 */

    hfmpsmbus->State = HAL_FMPSMBUS_STATE_READY;        /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpsmbus);                            /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configure Digital noise filter.
  * @param  hfmpsmbus Pointer to a FMPSMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPSMBUS.
  * @param  DigitalFilter Coefficient of digital noise filter between Min_Data=0x00 and Max_Data=0x0F.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPSMBUS_ConfigDigitalFilter(FMPSMBUS_HandleTypeDef *hfmpsmbus, uint32_t DigitalFilter)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpreg;                                      /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_FMPSMBUS_ALL_INSTANCE(hfmpsmbus->Instance));  /* 执行语句 */
  assert_param(IS_FMPSMBUS_DIGITAL_FILTER(DigitalFilter));  /* 执行语句 */

  if (hfmpsmbus->State == HAL_FMPSMBUS_STATE_READY)     /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Process Locked */
    __HAL_LOCK(hfmpsmbus);                              /* HAL辅助宏/函数 */

    hfmpsmbus->State = HAL_FMPSMBUS_STATE_BUSY;         /* 赋值或初始化 */

    /* Disable the selected FMPSMBUS peripheral */
    __HAL_FMPSMBUS_DISABLE(hfmpsmbus);                  /* HAL辅助宏/函数 */

    /* Get the old register value */
    tmpreg = hfmpsmbus->Instance->CR1;                  /* 赋值或初始化 */

    /* Reset FMPI2C DNF bits [11:8] */
    tmpreg &= ~(FMPI2C_CR1_DNF);                        /* 赋值或初始化 */

    /* Set FMPI2Cx DNF coefficient */
    tmpreg |= DigitalFilter << FMPI2C_CR1_DNF_Pos;      /* 赋值或初始化 */

    /* Store the new register value */
    hfmpsmbus->Instance->CR1 = tmpreg;                  /* 赋值或初始化 */

    __HAL_FMPSMBUS_ENABLE(hfmpsmbus);                   /* HAL辅助宏/函数 */

    hfmpsmbus->State = HAL_FMPSMBUS_STATE_READY;        /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpsmbus);                            /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

#if (USE_HAL_FMPSMBUS_REGISTER_CALLBACKS == 1)          /* 条件编译 */
/**
  * @brief  Register a User FMPSMBUS Callback
  *         To be used instead of the weak predefined callback
  * @param  hfmpsmbus Pointer to a FMPSMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPSMBUS.
  * @param  CallbackID ID of the callback to be registered
  *         This parameter can be one of the following values:
  *          @arg @ref HAL_FMPSMBUS_MASTER_TX_COMPLETE_CB_ID Master Tx Transfer completed callback ID
  *          @arg @ref HAL_FMPSMBUS_MASTER_RX_COMPLETE_CB_ID Master Rx Transfer completed callback ID
  *          @arg @ref HAL_FMPSMBUS_SLAVE_TX_COMPLETE_CB_ID Slave Tx Transfer completed callback ID
  *          @arg @ref HAL_FMPSMBUS_SLAVE_RX_COMPLETE_CB_ID Slave Rx Transfer completed callback ID
  *          @arg @ref HAL_FMPSMBUS_LISTEN_COMPLETE_CB_ID Listen Complete callback ID
  *          @arg @ref HAL_FMPSMBUS_ERROR_CB_ID Error callback ID
  *          @arg @ref HAL_FMPSMBUS_MSPINIT_CB_ID MspInit callback ID
  *          @arg @ref HAL_FMPSMBUS_MSPDEINIT_CB_ID MspDeInit callback ID
  * @param  pCallback pointer to the Callback function
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPSMBUS_RegisterCallback(FMPSMBUS_HandleTypeDef *hfmpsmbus, HAL_FMPSMBUS_CallbackIDTypeDef CallbackID,  /* HAL状态类型函数 */
                                             pFMPSMBUS_CallbackTypeDef pCallback)  /* 源代码 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  if (pCallback == NULL)                                /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hfmpsmbus->ErrorCode |= HAL_FMPSMBUS_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Process locked */
  __HAL_LOCK(hfmpsmbus);                                /* HAL辅助宏/函数 */

  if (HAL_FMPSMBUS_STATE_READY == hfmpsmbus->State)     /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_FMPSMBUS_MASTER_TX_COMPLETE_CB_ID :      /* case标签 */
        hfmpsmbus->MasterTxCpltCallback = pCallback;    /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_FMPSMBUS_MASTER_RX_COMPLETE_CB_ID :      /* case标签 */
        hfmpsmbus->MasterRxCpltCallback = pCallback;    /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_FMPSMBUS_SLAVE_TX_COMPLETE_CB_ID :       /* case标签 */
        hfmpsmbus->SlaveTxCpltCallback = pCallback;     /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_FMPSMBUS_SLAVE_RX_COMPLETE_CB_ID :       /* case标签 */
        hfmpsmbus->SlaveRxCpltCallback = pCallback;     /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_FMPSMBUS_LISTEN_COMPLETE_CB_ID :         /* case标签 */
        hfmpsmbus->ListenCpltCallback = pCallback;      /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_FMPSMBUS_ERROR_CB_ID :                   /* case标签 */
        hfmpsmbus->ErrorCallback = pCallback;           /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_FMPSMBUS_MSPINIT_CB_ID :                 /* case标签 */
        hfmpsmbus->MspInitCallback = pCallback;         /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_FMPSMBUS_MSPDEINIT_CB_ID :               /* case标签 */
        hfmpsmbus->MspDeInitCallback = pCallback;       /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hfmpsmbus->ErrorCode |= HAL_FMPSMBUS_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (HAL_FMPSMBUS_STATE_RESET == hfmpsmbus->State)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_FMPSMBUS_MSPINIT_CB_ID :                 /* case标签 */
        hfmpsmbus->MspInitCallback = pCallback;         /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_FMPSMBUS_MSPDEINIT_CB_ID :               /* case标签 */
        hfmpsmbus->MspDeInitCallback = pCallback;       /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hfmpsmbus->ErrorCode |= HAL_FMPSMBUS_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hfmpsmbus->ErrorCode |= HAL_FMPSMBUS_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hfmpsmbus);                              /* HAL辅助宏/函数 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Unregister an FMPSMBUS Callback
  *         FMPSMBUS callback is redirected to the weak predefined callback
  * @param  hfmpsmbus Pointer to a FMPSMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPSMBUS.
  * @param  CallbackID ID of the callback to be unregistered
  *         This parameter can be one of the following values:
  *         This parameter can be one of the following values:
  *          @arg @ref HAL_FMPSMBUS_MASTER_TX_COMPLETE_CB_ID Master Tx Transfer completed callback ID
  *          @arg @ref HAL_FMPSMBUS_MASTER_RX_COMPLETE_CB_ID Master Rx Transfer completed callback ID
  *          @arg @ref HAL_FMPSMBUS_SLAVE_TX_COMPLETE_CB_ID Slave Tx Transfer completed callback ID
  *          @arg @ref HAL_FMPSMBUS_SLAVE_RX_COMPLETE_CB_ID Slave Rx Transfer completed callback ID
  *          @arg @ref HAL_FMPSMBUS_LISTEN_COMPLETE_CB_ID Listen Complete callback ID
  *          @arg @ref HAL_FMPSMBUS_ERROR_CB_ID Error callback ID
  *          @arg @ref HAL_FMPSMBUS_MSPINIT_CB_ID MspInit callback ID
  *          @arg @ref HAL_FMPSMBUS_MSPDEINIT_CB_ID MspDeInit callback ID
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPSMBUS_UnRegisterCallback(FMPSMBUS_HandleTypeDef *hfmpsmbus, HAL_FMPSMBUS_CallbackIDTypeDef CallbackID)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Process locked */
  __HAL_LOCK(hfmpsmbus);                                /* HAL辅助宏/函数 */

  if (HAL_FMPSMBUS_STATE_READY == hfmpsmbus->State)     /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_FMPSMBUS_MASTER_TX_COMPLETE_CB_ID :      /* case标签 */
        hfmpsmbus->MasterTxCpltCallback = HAL_FMPSMBUS_MasterTxCpltCallback; /* Legacy weak MasterTxCpltCallback */
        break;                                          /* 跳出 */

      case HAL_FMPSMBUS_MASTER_RX_COMPLETE_CB_ID :      /* case标签 */
        hfmpsmbus->MasterRxCpltCallback = HAL_FMPSMBUS_MasterRxCpltCallback; /* Legacy weak MasterRxCpltCallback */
        break;                                          /* 跳出 */

      case HAL_FMPSMBUS_SLAVE_TX_COMPLETE_CB_ID :       /* case标签 */
        hfmpsmbus->SlaveTxCpltCallback = HAL_FMPSMBUS_SlaveTxCpltCallback;   /* Legacy weak SlaveTxCpltCallback  */
        break;                                          /* 跳出 */

      case HAL_FMPSMBUS_SLAVE_RX_COMPLETE_CB_ID :       /* case标签 */
        hfmpsmbus->SlaveRxCpltCallback = HAL_FMPSMBUS_SlaveRxCpltCallback;   /* Legacy weak SlaveRxCpltCallback  */
        break;                                          /* 跳出 */

      case HAL_FMPSMBUS_LISTEN_COMPLETE_CB_ID :         /* case标签 */
        hfmpsmbus->ListenCpltCallback = HAL_FMPSMBUS_ListenCpltCallback;     /* Legacy weak ListenCpltCallback   */
        break;                                          /* 跳出 */

      case HAL_FMPSMBUS_ERROR_CB_ID :                   /* case标签 */
        hfmpsmbus->ErrorCallback = HAL_FMPSMBUS_ErrorCallback;               /* Legacy weak ErrorCallback        */
        break;                                          /* 跳出 */

      case HAL_FMPSMBUS_MSPINIT_CB_ID :                 /* case标签 */
        hfmpsmbus->MspInitCallback = HAL_FMPSMBUS_MspInit;                   /* Legacy weak MspInit              */
        break;                                          /* 跳出 */

      case HAL_FMPSMBUS_MSPDEINIT_CB_ID :               /* case标签 */
        hfmpsmbus->MspDeInitCallback = HAL_FMPSMBUS_MspDeInit;               /* Legacy weak MspDeInit            */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hfmpsmbus->ErrorCode |= HAL_FMPSMBUS_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (HAL_FMPSMBUS_STATE_RESET == hfmpsmbus->State)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_FMPSMBUS_MSPINIT_CB_ID :                 /* case标签 */
        hfmpsmbus->MspInitCallback = HAL_FMPSMBUS_MspInit;                   /* Legacy weak MspInit              */
        break;                                          /* 跳出 */

      case HAL_FMPSMBUS_MSPDEINIT_CB_ID :               /* case标签 */
        hfmpsmbus->MspDeInitCallback = HAL_FMPSMBUS_MspDeInit;               /* Legacy weak MspDeInit            */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hfmpsmbus->ErrorCode |= HAL_FMPSMBUS_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hfmpsmbus->ErrorCode |= HAL_FMPSMBUS_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hfmpsmbus);                              /* HAL辅助宏/函数 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Register the Slave Address Match FMPSMBUS Callback
  *         To be used instead of the weak HAL_FMPSMBUS_AddrCallback() predefined callback
  * @param  hfmpsmbus Pointer to a FMPSMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPSMBUS.
  * @param  pCallback pointer to the Address Match Callback function
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPSMBUS_RegisterAddrCallback(FMPSMBUS_HandleTypeDef *hfmpsmbus, pFMPSMBUS_AddrCallbackTypeDef pCallback)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  if (pCallback == NULL)                                /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hfmpsmbus->ErrorCode |= HAL_FMPSMBUS_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  /* Process locked */
  __HAL_LOCK(hfmpsmbus);                                /* HAL辅助宏/函数 */

  if (HAL_FMPSMBUS_STATE_READY == hfmpsmbus->State)     /* 条件判断 */
  {                                                     /* 代码块开始 */
    hfmpsmbus->AddrCallback = pCallback;                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hfmpsmbus->ErrorCode |= HAL_FMPSMBUS_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hfmpsmbus);                              /* HAL辅助宏/函数 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  UnRegister the Slave Address Match FMPSMBUS Callback
  *         Info Ready FMPSMBUS Callback is redirected to the weak HAL_FMPSMBUS_AddrCallback() predefined callback
  * @param  hfmpsmbus Pointer to a FMPSMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPSMBUS.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPSMBUS_UnRegisterAddrCallback(FMPSMBUS_HandleTypeDef *hfmpsmbus)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Process locked */
  __HAL_LOCK(hfmpsmbus);                                /* HAL辅助宏/函数 */

  if (HAL_FMPSMBUS_STATE_READY == hfmpsmbus->State)     /* 条件判断 */
  {                                                     /* 代码块开始 */
    hfmpsmbus->AddrCallback = HAL_FMPSMBUS_AddrCallback; /* Legacy weak AddrCallback  */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hfmpsmbus->ErrorCode |= HAL_FMPSMBUS_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hfmpsmbus);                              /* HAL辅助宏/函数 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

#endif /* USE_HAL_FMPSMBUS_REGISTER_CALLBACKS */

/**
  * @}
  */

/** @defgroup FMPSMBUS_Exported_Functions_Group2 Input and Output operation functions
  *  @brief   Data transfers functions
  *
@verbatim
 ===============================================================================
                      ##### IO operation functions #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to manage the FMPSMBUS data
    transfers.

    (#) Blocking mode function to check if device is ready for usage is :
        (++) HAL_FMPSMBUS_IsDeviceReady()

    (#) There is only one mode of transfer:
       (++) Non-Blocking mode : The communication is performed using Interrupts.
            These functions return the status of the transfer startup.
            The end of the data processing will be indicated through the
            dedicated FMPSMBUS IRQ when using Interrupt mode.

    (#) Non-Blocking mode functions with Interrupt are :
        (++) HAL_FMPSMBUS_Master_Transmit_IT()
        (++) HAL_FMPSMBUS_Master_Receive_IT()
        (++) HAL_FMPSMBUS_Slave_Transmit_IT()
        (++) HAL_FMPSMBUS_Slave_Receive_IT()
        (++) HAL_FMPSMBUS_EnableListen_IT() or alias HAL_FMPSMBUS_EnableListen_IT()
        (++) HAL_FMPSMBUS_DisableListen_IT()
        (++) HAL_FMPSMBUS_EnableAlert_IT()
        (++) HAL_FMPSMBUS_DisableAlert_IT()

    (#) A set of Transfer Complete Callbacks are provided in non-Blocking mode:
        (++) HAL_FMPSMBUS_MasterTxCpltCallback()
        (++) HAL_FMPSMBUS_MasterRxCpltCallback()
        (++) HAL_FMPSMBUS_SlaveTxCpltCallback()
        (++) HAL_FMPSMBUS_SlaveRxCpltCallback()
        (++) HAL_FMPSMBUS_AddrCallback()
        (++) HAL_FMPSMBUS_ListenCpltCallback()
        (++) HAL_FMPSMBUS_ErrorCallback()

@endverbatim
  * @{
  */

/**
  * @brief  Transmit in master/host FMPSMBUS mode an amount of data in non-blocking mode with Interrupt.
  * @param  hfmpsmbus Pointer to a FMPSMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPSMBUS.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  XferOptions Options of Transfer, value of @ref FMPSMBUS_XferOptions_definition
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPSMBUS_Master_Transmit_IT(FMPSMBUS_HandleTypeDef *hfmpsmbus, uint16_t DevAddress, uint8_t *pData,  /* HAL状态类型函数 */
                                               uint16_t Size, uint32_t XferOptions)  /* 16位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t tmp;                                         /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_FMPSMBUS_TRANSFER_OPTIONS_REQUEST(XferOptions));  /* 执行语句 */

  if (hfmpsmbus->State == HAL_FMPSMBUS_STATE_READY)     /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Process Locked */
    __HAL_LOCK(hfmpsmbus);                              /* HAL辅助宏/函数 */

    hfmpsmbus->State = HAL_FMPSMBUS_STATE_MASTER_BUSY_TX;  /* 赋值或初始化 */
    hfmpsmbus->ErrorCode = HAL_FMPSMBUS_ERROR_NONE;     /* 赋值或初始化 */
    /* Prepare transfer parameters */
    hfmpsmbus->pBuffPtr = pData;                        /* 赋值或初始化 */
    hfmpsmbus->XferCount = Size;                        /* 赋值或初始化 */
    hfmpsmbus->XferOptions = XferOptions;               /* 赋值或初始化 */

    /* In case of Quick command, remove autoend mode */
    /* Manage the stop generation by software */
    if (hfmpsmbus->pBuffPtr == NULL)                    /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpsmbus->XferOptions &= ~FMPSMBUS_AUTOEND_MODE;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    if (Size > MAX_NBYTE_SIZE)                          /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpsmbus->XferSize = MAX_NBYTE_SIZE;             /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hfmpsmbus->XferSize = Size;                       /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Send Slave Address */
    /* Set NBYTES to write and reload if size > MAX_NBYTE_SIZE and generate RESTART */
    if ((hfmpsmbus->XferSize < hfmpsmbus->XferCount) && (hfmpsmbus->XferSize == MAX_NBYTE_SIZE))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      FMPSMBUS_TransferConfig(hfmpsmbus, DevAddress, (uint8_t)hfmpsmbus->XferSize,  /* 列表续行 */
                           FMPSMBUS_RELOAD_MODE | (hfmpsmbus->XferOptions & FMPSMBUS_SENDPEC_MODE), FMPSMBUS_GENERATE_START_WRITE);  /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* If transfer direction not change, do not generate Restart Condition */
      /* Mean Previous state is same as current state */

      /* Store current volatile XferOptions, misra rule */
      tmp = hfmpsmbus->XferOptions;                     /* 赋值或初始化 */

      if ((hfmpsmbus->PreviousState == HAL_FMPSMBUS_STATE_MASTER_BUSY_TX) && (IS_FMPSMBUS_TRANSFER_OTHER_OPTIONS_REQUEST(tmp) == 0))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        FMPSMBUS_TransferConfig(hfmpsmbus, DevAddress, (uint8_t)hfmpsmbus->XferSize, hfmpsmbus->XferOptions, FMPSMBUS_NO_STARTSTOP);  /* 执行语句 */
      }                                                 /* 代码块结束 */
      /* Else transfer direction change, so generate Restart with new transfer direction */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Convert OTHER_xxx XferOptions if any */
        FMPSMBUS_ConvertOtherXferOptions(hfmpsmbus);    /* 执行语句 */

        /* Handle Transfer */
        FMPSMBUS_TransferConfig(hfmpsmbus, DevAddress, (uint8_t)hfmpsmbus->XferSize, hfmpsmbus->XferOptions, FMPSMBUS_GENERATE_START_WRITE);  /* 执行语句 */
      }                                                 /* 代码块结束 */

      /* If PEC mode is enable, size to transmit manage by SW part should be Size-1 byte, corresponding to PEC byte */
      /* PEC byte is automatically sent by HW block, no need to manage it in Transmit process */
      if (FMPSMBUS_GET_PEC_MODE(hfmpsmbus) != 0UL)      /* 条件判断 */
      {                                                 /* 代码块开始 */
        hfmpsmbus->XferSize--;                          /* 执行语句 */
        hfmpsmbus->XferCount--;                         /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpsmbus);                            /* HAL辅助宏/函数 */

    /* Note : The FMPSMBUS interrupts must be enabled after unlocking current process
              to avoid the risk of FMPSMBUS interrupt handle execution before current
              process unlock */
    FMPSMBUS_Enable_IRQ(hfmpsmbus, FMPSMBUS_IT_TX);     /* 执行语句 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Receive in master/host FMPSMBUS mode an amount of data in non-blocking mode with Interrupt.
  * @param  hfmpsmbus Pointer to a FMPSMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPSMBUS.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  XferOptions Options of Transfer, value of @ref FMPSMBUS_XferOptions_definition
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPSMBUS_Master_Receive_IT(FMPSMBUS_HandleTypeDef *hfmpsmbus, uint16_t DevAddress, uint8_t *pData,  /* HAL状态类型函数 */
                                              uint16_t Size, uint32_t XferOptions)  /* 16位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t tmp;                                         /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_FMPSMBUS_TRANSFER_OPTIONS_REQUEST(XferOptions));  /* 执行语句 */

  if (hfmpsmbus->State == HAL_FMPSMBUS_STATE_READY)     /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Process Locked */
    __HAL_LOCK(hfmpsmbus);                              /* HAL辅助宏/函数 */

    hfmpsmbus->State = HAL_FMPSMBUS_STATE_MASTER_BUSY_RX;  /* 赋值或初始化 */
    hfmpsmbus->ErrorCode = HAL_FMPSMBUS_ERROR_NONE;     /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hfmpsmbus->pBuffPtr = pData;                        /* 赋值或初始化 */
    hfmpsmbus->XferCount = Size;                        /* 赋值或初始化 */
    hfmpsmbus->XferOptions = XferOptions;               /* 赋值或初始化 */

    /* In case of Quick command, remove autoend mode */
    /* Manage the stop generation by software */
    if (hfmpsmbus->pBuffPtr == NULL)                    /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpsmbus->XferOptions &= ~FMPSMBUS_AUTOEND_MODE;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    if (Size > MAX_NBYTE_SIZE)                          /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpsmbus->XferSize = MAX_NBYTE_SIZE;             /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hfmpsmbus->XferSize = Size;                       /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Send Slave Address */
    /* Set NBYTES to write and reload if size > MAX_NBYTE_SIZE and generate RESTART */
    if ((hfmpsmbus->XferSize < hfmpsmbus->XferCount) && (hfmpsmbus->XferSize == MAX_NBYTE_SIZE))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      FMPSMBUS_TransferConfig(hfmpsmbus, DevAddress, (uint8_t)hfmpsmbus->XferSize,  /* 列表续行 */
                           FMPSMBUS_RELOAD_MODE  | (hfmpsmbus->XferOptions & FMPSMBUS_SENDPEC_MODE), FMPSMBUS_GENERATE_START_READ);  /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* If transfer direction not change, do not generate Restart Condition */
      /* Mean Previous state is same as current state */

      /* Store current volatile XferOptions, Misra rule */
      tmp = hfmpsmbus->XferOptions;                     /* 赋值或初始化 */

      if ((hfmpsmbus->PreviousState == HAL_FMPSMBUS_STATE_MASTER_BUSY_RX) && (IS_FMPSMBUS_TRANSFER_OTHER_OPTIONS_REQUEST(tmp) == 0))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        FMPSMBUS_TransferConfig(hfmpsmbus, DevAddress, (uint8_t)hfmpsmbus->XferSize, hfmpsmbus->XferOptions, FMPSMBUS_NO_STARTSTOP);  /* 执行语句 */
      }                                                 /* 代码块结束 */
      /* Else transfer direction change, so generate Restart with new transfer direction */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Convert OTHER_xxx XferOptions if any */
        FMPSMBUS_ConvertOtherXferOptions(hfmpsmbus);    /* 执行语句 */

        /* Handle Transfer */
        FMPSMBUS_TransferConfig(hfmpsmbus, DevAddress, (uint8_t)hfmpsmbus->XferSize, hfmpsmbus->XferOptions, FMPSMBUS_GENERATE_START_READ);  /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpsmbus);                            /* HAL辅助宏/函数 */

    /* Note : The FMPSMBUS interrupts must be enabled after unlocking current process
              to avoid the risk of FMPSMBUS interrupt handle execution before current
              process unlock */
    FMPSMBUS_Enable_IRQ(hfmpsmbus, FMPSMBUS_IT_RX);     /* 执行语句 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Abort a master/host FMPSMBUS process communication with Interrupt.
  * @note   This abort can be called only if state is ready
  * @param  hfmpsmbus Pointer to a FMPSMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPSMBUS.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPSMBUS_Master_Abort_IT(FMPSMBUS_HandleTypeDef *hfmpsmbus, uint16_t DevAddress)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  if (hfmpsmbus->State == HAL_FMPSMBUS_STATE_READY)     /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Process Locked */
    __HAL_LOCK(hfmpsmbus);                              /* HAL辅助宏/函数 */

    /* Keep the same state as previous */
    /* to perform as well the call of the corresponding end of transfer callback */
    if (hfmpsmbus->PreviousState == HAL_FMPSMBUS_STATE_MASTER_BUSY_TX)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpsmbus->State = HAL_FMPSMBUS_STATE_MASTER_BUSY_TX;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if (hfmpsmbus->PreviousState == HAL_FMPSMBUS_STATE_MASTER_BUSY_RX)  /* 否则如果 */
    {                                                   /* 代码块开始 */
      hfmpsmbus->State = HAL_FMPSMBUS_STATE_MASTER_BUSY_RX;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Wrong usage of abort function */
      /* This function should be used only in case of abort monitored by master device */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
    hfmpsmbus->ErrorCode = HAL_FMPSMBUS_ERROR_NONE;     /* 赋值或初始化 */

    /* Set NBYTES to 1 to generate a dummy read on FMPSMBUS peripheral */
    /* Set AUTOEND mode, this will generate a NACK then STOP condition to abort the current transfer */
    FMPSMBUS_TransferConfig(hfmpsmbus, DevAddress, 1, FMPSMBUS_AUTOEND_MODE, FMPSMBUS_NO_STARTSTOP);  /* 执行语句 */

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpsmbus);                            /* HAL辅助宏/函数 */

    /* Note : The FMPSMBUS interrupts must be enabled after unlocking current process
              to avoid the risk of FMPSMBUS interrupt handle execution before current
              process unlock */
    if (hfmpsmbus->State == HAL_FMPSMBUS_STATE_MASTER_BUSY_TX)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      FMPSMBUS_Enable_IRQ(hfmpsmbus, FMPSMBUS_IT_TX);   /* 执行语句 */
    }                                                   /* 代码块结束 */
    else if (hfmpsmbus->State == HAL_FMPSMBUS_STATE_MASTER_BUSY_RX)  /* 否则如果 */
    {                                                   /* 代码块开始 */
      FMPSMBUS_Enable_IRQ(hfmpsmbus, FMPSMBUS_IT_RX);   /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Nothing to do */
    }                                                   /* 代码块结束 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Transmit in slave/device FMPSMBUS mode an amount of data in non-blocking mode with Interrupt.
  * @param  hfmpsmbus Pointer to a FMPSMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPSMBUS.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  XferOptions Options of Transfer, value of @ref FMPSMBUS_XferOptions_definition
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPSMBUS_Slave_Transmit_IT(FMPSMBUS_HandleTypeDef *hfmpsmbus, uint8_t *pData, uint16_t Size,  /* HAL状态类型函数 */
                                              uint32_t XferOptions)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_FMPSMBUS_TRANSFER_OPTIONS_REQUEST(XferOptions));  /* 执行语句 */

  if ((hfmpsmbus->State & HAL_FMPSMBUS_STATE_LISTEN) == HAL_FMPSMBUS_STATE_LISTEN)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) || (Size == 0UL))               /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpsmbus->ErrorCode = HAL_FMPSMBUS_ERROR_INVALID_PARAM;  /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Disable Interrupts, to prevent preemption during treatment in case of multicall */
    FMPSMBUS_Disable_IRQ(hfmpsmbus, FMPSMBUS_IT_ADDR | FMPSMBUS_IT_TX);  /* 执行语句 */

    /* Process Locked */
    __HAL_LOCK(hfmpsmbus);                              /* HAL辅助宏/函数 */

    hfmpsmbus->State = (HAL_FMPSMBUS_STATE_SLAVE_BUSY_TX | HAL_FMPSMBUS_STATE_LISTEN);  /* 赋值或初始化 */
    hfmpsmbus->ErrorCode = HAL_FMPSMBUS_ERROR_NONE;     /* 赋值或初始化 */

    /* Set SBC bit to manage Acknowledge at each bit */
    hfmpsmbus->Instance->CR1 |= FMPI2C_CR1_SBC;         /* 赋值或初始化 */

    /* Enable Address Acknowledge */
    hfmpsmbus->Instance->CR2 &= ~FMPI2C_CR2_NACK;       /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hfmpsmbus->pBuffPtr = pData;                        /* 赋值或初始化 */
    hfmpsmbus->XferCount = Size;                        /* 赋值或初始化 */
    hfmpsmbus->XferOptions = XferOptions;               /* 赋值或初始化 */

    /* Convert OTHER_xxx XferOptions if any */
    FMPSMBUS_ConvertOtherXferOptions(hfmpsmbus);        /* 执行语句 */

    if (Size > MAX_NBYTE_SIZE)                          /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpsmbus->XferSize = MAX_NBYTE_SIZE;             /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hfmpsmbus->XferSize = Size;                       /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Set NBYTES to write and reload if size > MAX_NBYTE_SIZE and generate RESTART */
    if ((hfmpsmbus->XferSize < hfmpsmbus->XferCount) && (hfmpsmbus->XferSize == MAX_NBYTE_SIZE))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      FMPSMBUS_TransferConfig(hfmpsmbus, 0, (uint8_t)hfmpsmbus->XferSize,  /* 列表续行 */
                           FMPSMBUS_RELOAD_MODE | (hfmpsmbus->XferOptions & FMPSMBUS_SENDPEC_MODE), FMPSMBUS_NO_STARTSTOP);  /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Set NBYTE to transmit */
      FMPSMBUS_TransferConfig(hfmpsmbus, 0, (uint8_t)hfmpsmbus->XferSize, hfmpsmbus->XferOptions, FMPSMBUS_NO_STARTSTOP);  /* 执行语句 */

      /* If PEC mode is enable, size to transmit should be Size-1 byte, corresponding to PEC byte */
      /* PEC byte is automatically sent by HW block, no need to manage it in Transmit process */
      if (FMPSMBUS_GET_PEC_MODE(hfmpsmbus) != 0UL)      /* 条件判断 */
      {                                                 /* 代码块开始 */
        hfmpsmbus->XferSize--;                          /* 执行语句 */
        hfmpsmbus->XferCount--;                         /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Clear ADDR flag after prepare the transfer parameters */
    /* This action will generate an acknowledge to the HOST */
    __HAL_FMPSMBUS_CLEAR_FLAG(hfmpsmbus, FMPSMBUS_FLAG_ADDR);  /* HAL辅助宏/函数 */

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpsmbus);                            /* HAL辅助宏/函数 */

    /* Note : The FMPSMBUS interrupts must be enabled after unlocking current process
              to avoid the risk of FMPSMBUS interrupt handle execution before current
              process unlock */
    /* REnable ADDR interrupt */
    FMPSMBUS_Enable_IRQ(hfmpsmbus, FMPSMBUS_IT_TX | FMPSMBUS_IT_ADDR);  /* 执行语句 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Receive in slave/device FMPSMBUS mode an amount of data in non-blocking mode with Interrupt.
  * @param  hfmpsmbus Pointer to a FMPSMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPSMBUS.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  XferOptions Options of Transfer, value of @ref FMPSMBUS_XferOptions_definition
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPSMBUS_Slave_Receive_IT(FMPSMBUS_HandleTypeDef *hfmpsmbus, uint8_t *pData, uint16_t Size,  /* HAL状态类型函数 */
                                             uint32_t XferOptions)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_FMPSMBUS_TRANSFER_OPTIONS_REQUEST(XferOptions));  /* 执行语句 */

  if ((hfmpsmbus->State & HAL_FMPSMBUS_STATE_LISTEN) == HAL_FMPSMBUS_STATE_LISTEN)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) || (Size == 0UL))               /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpsmbus->ErrorCode = HAL_FMPSMBUS_ERROR_INVALID_PARAM;  /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Disable Interrupts, to prevent preemption during treatment in case of multicall */
    FMPSMBUS_Disable_IRQ(hfmpsmbus, FMPSMBUS_IT_ADDR | FMPSMBUS_IT_RX);  /* 执行语句 */

    /* Process Locked */
    __HAL_LOCK(hfmpsmbus);                              /* HAL辅助宏/函数 */

    hfmpsmbus->State = (HAL_FMPSMBUS_STATE_SLAVE_BUSY_RX | HAL_FMPSMBUS_STATE_LISTEN);  /* 赋值或初始化 */
    hfmpsmbus->ErrorCode = HAL_FMPSMBUS_ERROR_NONE;     /* 赋值或初始化 */

    /* Set SBC bit to manage Acknowledge at each bit */
    hfmpsmbus->Instance->CR1 |= FMPI2C_CR1_SBC;         /* 赋值或初始化 */

    /* Enable Address Acknowledge */
    hfmpsmbus->Instance->CR2 &= ~FMPI2C_CR2_NACK;       /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hfmpsmbus->pBuffPtr = pData;                        /* 赋值或初始化 */
    hfmpsmbus->XferSize = Size;                         /* 赋值或初始化 */
    hfmpsmbus->XferCount = Size;                        /* 赋值或初始化 */
    hfmpsmbus->XferOptions = XferOptions;               /* 赋值或初始化 */

    /* Convert OTHER_xxx XferOptions if any */
    FMPSMBUS_ConvertOtherXferOptions(hfmpsmbus);        /* 执行语句 */

    /* Set NBYTE to receive */
    /* If XferSize equal "1", or XferSize equal "2" with PEC requested (mean 1 data byte + 1 PEC byte */
    /* no need to set RELOAD bit mode, a ACK will be automatically generated in that case */
    /* else need to set RELOAD bit mode to generate an automatic ACK at each byte Received */
    /* This RELOAD bit will be reset for last BYTE to be receive in FMPSMBUS_Slave_ISR */
    if (((FMPSMBUS_GET_PEC_MODE(hfmpsmbus) != 0UL) && (hfmpsmbus->XferSize == 2U)) || (hfmpsmbus->XferSize == 1U))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      FMPSMBUS_TransferConfig(hfmpsmbus, 0, (uint8_t)hfmpsmbus->XferSize, hfmpsmbus->XferOptions, FMPSMBUS_NO_STARTSTOP);  /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      FMPSMBUS_TransferConfig(hfmpsmbus, 0, 1, hfmpsmbus->XferOptions | FMPSMBUS_RELOAD_MODE, FMPSMBUS_NO_STARTSTOP);  /* 执行语句 */
    }                                                   /* 代码块结束 */

    /* Clear ADDR flag after prepare the transfer parameters */
    /* This action will generate an acknowledge to the HOST */
    __HAL_FMPSMBUS_CLEAR_FLAG(hfmpsmbus, FMPSMBUS_FLAG_ADDR);  /* HAL辅助宏/函数 */

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpsmbus);                            /* HAL辅助宏/函数 */

    /* Note : The FMPSMBUS interrupts must be enabled after unlocking current process
              to avoid the risk of FMPSMBUS interrupt handle execution before current
              process unlock */
    /* REnable ADDR interrupt */
    FMPSMBUS_Enable_IRQ(hfmpsmbus, FMPSMBUS_IT_RX | FMPSMBUS_IT_ADDR);  /* 执行语句 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Enable the Address listen mode with Interrupt.
  * @param  hfmpsmbus Pointer to a FMPSMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPSMBUS.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPSMBUS_EnableListen_IT(FMPSMBUS_HandleTypeDef *hfmpsmbus)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  hfmpsmbus->State = HAL_FMPSMBUS_STATE_LISTEN;         /* 赋值或初始化 */

  /* Enable the Address Match interrupt */
  FMPSMBUS_Enable_IRQ(hfmpsmbus, FMPSMBUS_IT_ADDR);     /* 执行语句 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Disable the Address listen mode with Interrupt.
  * @param  hfmpsmbus Pointer to a FMPSMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPSMBUS.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPSMBUS_DisableListen_IT(FMPSMBUS_HandleTypeDef *hfmpsmbus)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Disable Address listen mode only if a transfer is not ongoing */
  if (hfmpsmbus->State == HAL_FMPSMBUS_STATE_LISTEN)    /* 条件判断 */
  {                                                     /* 代码块开始 */
    hfmpsmbus->State = HAL_FMPSMBUS_STATE_READY;        /* 赋值或初始化 */

    /* Disable the Address Match interrupt */
    FMPSMBUS_Disable_IRQ(hfmpsmbus, FMPSMBUS_IT_ADDR);  /* 执行语句 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Enable the FMPSMBUS alert mode with Interrupt.
  * @param  hfmpsmbus Pointer to a FMPSMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPSMBUSx peripheral.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPSMBUS_EnableAlert_IT(FMPSMBUS_HandleTypeDef *hfmpsmbus)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Enable SMBus alert */
  hfmpsmbus->Instance->CR1 |= FMPI2C_CR1_ALERTEN;       /* 赋值或初始化 */

  /* Clear ALERT flag */
  __HAL_FMPSMBUS_CLEAR_FLAG(hfmpsmbus, FMPSMBUS_FLAG_ALERT);  /* HAL辅助宏/函数 */

  /* Enable Alert Interrupt */
  FMPSMBUS_Enable_IRQ(hfmpsmbus, FMPSMBUS_IT_ALERT);    /* 执行语句 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */
/**
  * @brief  Disable the FMPSMBUS alert mode with Interrupt.
  * @param  hfmpsmbus Pointer to a FMPSMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPSMBUSx peripheral.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPSMBUS_DisableAlert_IT(FMPSMBUS_HandleTypeDef *hfmpsmbus)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Enable SMBus alert */
  hfmpsmbus->Instance->CR1 &= ~FMPI2C_CR1_ALERTEN;      /* 赋值或初始化 */

  /* Disable Alert Interrupt */
  FMPSMBUS_Disable_IRQ(hfmpsmbus, FMPSMBUS_IT_ALERT);   /* 执行语句 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Check if target device is ready for communication.
  * @param  hfmpsmbus Pointer to a FMPSMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPSMBUS.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  Trials Number of trials
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPSMBUS_IsDeviceReady(FMPSMBUS_HandleTypeDef *hfmpsmbus, uint16_t DevAddress, uint32_t Trials,  /* HAL状态类型函数 */
                                          uint32_t Timeout)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */

  __IO uint32_t FMPSMBUS_Trials = 0UL;                  /* 赋值或初始化 */

  FlagStatus tmp1;                                      /* 执行语句 */
  FlagStatus tmp2;                                      /* 执行语句 */

  if (hfmpsmbus->State == HAL_FMPSMBUS_STATE_READY)     /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (__HAL_FMPSMBUS_GET_FLAG(hfmpsmbus, FMPSMBUS_FLAG_BUSY) != RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_BUSY;                                  /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hfmpsmbus);                              /* HAL辅助宏/函数 */

    hfmpsmbus->State = HAL_FMPSMBUS_STATE_BUSY;         /* 赋值或初始化 */
    hfmpsmbus->ErrorCode = HAL_FMPSMBUS_ERROR_NONE;     /* 赋值或初始化 */

    do                                                  /* do-while循环 */
    {                                                   /* 代码块开始 */
      /* Generate Start */
      hfmpsmbus->Instance->CR2 = FMPSMBUS_GENERATE_START(hfmpsmbus->Init.AddressingMode, DevAddress);  /* 赋值或初始化 */

      /* No need to Check TC flag, with AUTOEND mode the stop is automatically generated */
      /* Wait until STOPF flag is set or a NACK flag is set*/
      tickstart = HAL_GetTick();                        /* 赋值或初始化 */

      tmp1 = __HAL_FMPSMBUS_GET_FLAG(hfmpsmbus, FMPSMBUS_FLAG_STOPF);  /* 赋值或初始化 */
      tmp2 = __HAL_FMPSMBUS_GET_FLAG(hfmpsmbus, FMPSMBUS_FLAG_AF);  /* 赋值或初始化 */

      while ((tmp1 == RESET) && (tmp2 == RESET))        /* while循环 */
      {                                                 /* 代码块开始 */
        if (Timeout != HAL_MAX_DELAY)                   /* 条件判断 */
        {                                               /* 代码块开始 */
          if (((HAL_GetTick() - tickstart) > Timeout) || (Timeout == 0UL))  /* 条件判断 */
          {                                             /* 代码块开始 */
            /* Device is ready */
            hfmpsmbus->State = HAL_FMPSMBUS_STATE_READY;  /* 赋值或初始化 */

            /* Update FMPSMBUS error code */
            hfmpsmbus->ErrorCode |= HAL_FMPSMBUS_ERROR_HALTIMEOUT;  /* 赋值或初始化 */

            /* Process Unlocked */
            __HAL_UNLOCK(hfmpsmbus);                    /* HAL辅助宏/函数 */
            return HAL_ERROR;                           /* 返回值 */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */

        tmp1 = __HAL_FMPSMBUS_GET_FLAG(hfmpsmbus, FMPSMBUS_FLAG_STOPF);  /* 赋值或初始化 */
        tmp2 = __HAL_FMPSMBUS_GET_FLAG(hfmpsmbus, FMPSMBUS_FLAG_AF);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */

      /* Check if the NACKF flag has not been set */
      if (__HAL_FMPSMBUS_GET_FLAG(hfmpsmbus, FMPSMBUS_FLAG_AF) == RESET)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Wait until STOPF flag is reset */
        if (FMPSMBUS_WaitOnFlagUntilTimeout(hfmpsmbus, FMPSMBUS_FLAG_STOPF, RESET, Timeout) != HAL_OK)  /* 条件判断 */
        {                                               /* 代码块开始 */
          return HAL_ERROR;                             /* 返回值 */
        }                                               /* 代码块结束 */

        /* Clear STOP Flag */
        __HAL_FMPSMBUS_CLEAR_FLAG(hfmpsmbus, FMPSMBUS_FLAG_STOPF);  /* HAL辅助宏/函数 */

        /* Device is ready */
        hfmpsmbus->State = HAL_FMPSMBUS_STATE_READY;    /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hfmpsmbus);                        /* HAL辅助宏/函数 */

        return HAL_OK;                                  /* 返回值 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Wait until STOPF flag is reset */
        if (FMPSMBUS_WaitOnFlagUntilTimeout(hfmpsmbus, FMPSMBUS_FLAG_STOPF, RESET, Timeout) != HAL_OK)  /* 条件判断 */
        {                                               /* 代码块开始 */
          return HAL_ERROR;                             /* 返回值 */
        }                                               /* 代码块结束 */

        /* Clear NACK Flag */
        __HAL_FMPSMBUS_CLEAR_FLAG(hfmpsmbus, FMPSMBUS_FLAG_AF);  /* HAL辅助宏/函数 */

        /* Clear STOP Flag, auto generated with autoend*/
        __HAL_FMPSMBUS_CLEAR_FLAG(hfmpsmbus, FMPSMBUS_FLAG_STOPF);  /* HAL辅助宏/函数 */
      }                                                 /* 代码块结束 */

      /* Check if the maximum allowed number of trials has been reached */
      if (FMPSMBUS_Trials == Trials)                    /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Generate Stop */
        hfmpsmbus->Instance->CR2 |= FMPI2C_CR2_STOP;    /* 赋值或初始化 */

        /* Wait until STOPF flag is reset */
        if (FMPSMBUS_WaitOnFlagUntilTimeout(hfmpsmbus, FMPSMBUS_FLAG_STOPF, RESET, Timeout) != HAL_OK)  /* 条件判断 */
        {                                               /* 代码块开始 */
          return HAL_ERROR;                             /* 返回值 */
        }                                               /* 代码块结束 */

        /* Clear STOP Flag */
        __HAL_FMPSMBUS_CLEAR_FLAG(hfmpsmbus, FMPSMBUS_FLAG_STOPF);  /* HAL辅助宏/函数 */
      }                                                 /* 代码块结束 */

      /* Increment Trials */
      FMPSMBUS_Trials++;                                /* 执行语句 */
    } while (FMPSMBUS_Trials < Trials);                 /* 代码块结束 */

    hfmpsmbus->State = HAL_FMPSMBUS_STATE_READY;        /* 赋值或初始化 */

    /* Update FMPSMBUS error code */
    hfmpsmbus->ErrorCode |= HAL_FMPSMBUS_ERROR_HALTIMEOUT;  /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpsmbus);                            /* HAL辅助宏/函数 */

    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */
/**
  * @}
  */

/** @defgroup FMPSMBUS_IRQ_Handler_and_Callbacks IRQ Handler and Callbacks
  * @{
  */

/**
  * @brief  Handle FMPSMBUS event interrupt request.
  * @param  hfmpsmbus Pointer to a FMPSMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPSMBUS.
  * @retval None
  */
void HAL_FMPSMBUS_EV_IRQHandler(FMPSMBUS_HandleTypeDef *hfmpsmbus)  /* 中断服务函数 */
{                                                       /* 代码块开始 */
  /* Use a local variable to store the current ISR flags */
  /* This action will avoid a wrong treatment due to ISR flags change during interrupt handler */
  uint32_t tmpisrvalue = READ_REG(hfmpsmbus->Instance->ISR);  /* 32位无符号变量/函数 */
  uint32_t tmpcr1value = READ_REG(hfmpsmbus->Instance->CR1);  /* 32位无符号变量/函数 */

  /* FMPSMBUS in mode Transmitter ---------------------------------------------------*/
  if ((FMPSMBUS_CHECK_IT_SOURCE(tmpcr1value, (FMPSMBUS_IT_TCI | FMPSMBUS_IT_STOPI | FMPSMBUS_IT_NACKI | FMPSMBUS_IT_TXI)) != RESET) &&  /* 条件判断 */
      ((FMPSMBUS_CHECK_FLAG(tmpisrvalue, FMPSMBUS_FLAG_TXIS) != RESET) ||  /* 源代码 */
       (FMPSMBUS_CHECK_FLAG(tmpisrvalue, FMPSMBUS_FLAG_TCR) != RESET) ||  /* 源代码 */
       (FMPSMBUS_CHECK_FLAG(tmpisrvalue, FMPSMBUS_FLAG_TC) != RESET) ||  /* 源代码 */
       (FMPSMBUS_CHECK_FLAG(tmpisrvalue, FMPSMBUS_FLAG_STOPF) != RESET) ||  /* 源代码 */
       (FMPSMBUS_CHECK_FLAG(tmpisrvalue, FMPSMBUS_FLAG_AF) != RESET)))  /* 源代码 */
  {                                                     /* 代码块开始 */
    /* Slave mode selected */
    if ((hfmpsmbus->State & HAL_FMPSMBUS_STATE_SLAVE_BUSY_TX) == HAL_FMPSMBUS_STATE_SLAVE_BUSY_TX)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      (void)FMPSMBUS_Slave_ISR(hfmpsmbus, tmpisrvalue);  /* 执行语句 */
    }                                                   /* 代码块结束 */
    /* Master mode selected */
    else if ((hfmpsmbus->State & HAL_FMPSMBUS_STATE_MASTER_BUSY_TX) == HAL_FMPSMBUS_STATE_MASTER_BUSY_TX)  /* 否则如果 */
    {                                                   /* 代码块开始 */
      (void)FMPSMBUS_Master_ISR(hfmpsmbus, tmpisrvalue);  /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Nothing to do */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* FMPSMBUS in mode Receiver ----------------------------------------------------*/
  if ((FMPSMBUS_CHECK_IT_SOURCE(tmpcr1value, (FMPSMBUS_IT_TCI | FMPSMBUS_IT_STOPI | FMPSMBUS_IT_NACKI | FMPSMBUS_IT_RXI)) != RESET) &&  /* 条件判断 */
      ((FMPSMBUS_CHECK_FLAG(tmpisrvalue, FMPSMBUS_FLAG_RXNE) != RESET) ||  /* 源代码 */
       (FMPSMBUS_CHECK_FLAG(tmpisrvalue, FMPSMBUS_FLAG_TCR) != RESET) ||  /* 源代码 */
       (FMPSMBUS_CHECK_FLAG(tmpisrvalue, FMPSMBUS_FLAG_TC) != RESET) ||  /* 源代码 */
       (FMPSMBUS_CHECK_FLAG(tmpisrvalue, FMPSMBUS_FLAG_STOPF) != RESET) ||  /* 源代码 */
       (FMPSMBUS_CHECK_FLAG(tmpisrvalue, FMPSMBUS_FLAG_AF) != RESET)))  /* 源代码 */
  {                                                     /* 代码块开始 */
    /* Slave mode selected */
    if ((hfmpsmbus->State & HAL_FMPSMBUS_STATE_SLAVE_BUSY_RX) == HAL_FMPSMBUS_STATE_SLAVE_BUSY_RX)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      (void)FMPSMBUS_Slave_ISR(hfmpsmbus, tmpisrvalue);  /* 执行语句 */
    }                                                   /* 代码块结束 */
    /* Master mode selected */
    else if ((hfmpsmbus->State & HAL_FMPSMBUS_STATE_MASTER_BUSY_RX) == HAL_FMPSMBUS_STATE_MASTER_BUSY_RX)  /* 否则如果 */
    {                                                   /* 代码块开始 */
      (void)FMPSMBUS_Master_ISR(hfmpsmbus, tmpisrvalue);  /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Nothing to do */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* FMPSMBUS in mode Listener Only --------------------------------------------------*/
  if (((FMPSMBUS_CHECK_IT_SOURCE(tmpcr1value, FMPSMBUS_IT_ADDRI) != RESET) ||  /* 条件判断 */
       (FMPSMBUS_CHECK_IT_SOURCE(tmpcr1value, FMPSMBUS_IT_STOPI) != RESET) ||  /* 源代码 */
       (FMPSMBUS_CHECK_IT_SOURCE(tmpcr1value, FMPSMBUS_IT_NACKI) != RESET)) &&  /* 源代码 */
      ((FMPSMBUS_CHECK_FLAG(tmpisrvalue, FMPSMBUS_FLAG_ADDR) != RESET) ||  /* 源代码 */
       (FMPSMBUS_CHECK_FLAG(tmpisrvalue, FMPSMBUS_FLAG_STOPF) != RESET) ||  /* 源代码 */
       (FMPSMBUS_CHECK_FLAG(tmpisrvalue, FMPSMBUS_FLAG_AF) != RESET)))  /* 源代码 */
  {                                                     /* 代码块开始 */
    if ((hfmpsmbus->State & HAL_FMPSMBUS_STATE_LISTEN) == HAL_FMPSMBUS_STATE_LISTEN)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      (void)FMPSMBUS_Slave_ISR(hfmpsmbus, tmpisrvalue);  /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Handle FMPSMBUS error interrupt request.
  * @param  hfmpsmbus Pointer to a FMPSMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPSMBUS.
  * @retval None
  */
void HAL_FMPSMBUS_ER_IRQHandler(FMPSMBUS_HandleTypeDef *hfmpsmbus)  /* 中断服务函数 */
{                                                       /* 代码块开始 */
  FMPSMBUS_ITErrorHandler(hfmpsmbus);                   /* 执行语句 */
}                                                       /* 代码块结束 */

/**
  * @brief  Master Tx Transfer completed callback.
  * @param  hfmpsmbus Pointer to a FMPSMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPSMBUS.
  * @retval None
  */
__weak void HAL_FMPSMBUS_MasterTxCpltCallback(FMPSMBUS_HandleTypeDef *hfmpsmbus)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hfmpsmbus);                                    /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_FMPSMBUS_MasterTxCpltCallback() could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Master Rx Transfer completed callback.
  * @param  hfmpsmbus Pointer to a FMPSMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPSMBUS.
  * @retval None
  */
__weak void HAL_FMPSMBUS_MasterRxCpltCallback(FMPSMBUS_HandleTypeDef *hfmpsmbus)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hfmpsmbus);                                    /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_FMPSMBUS_MasterRxCpltCallback() could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/** @brief  Slave Tx Transfer completed callback.
  * @param  hfmpsmbus Pointer to a FMPSMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPSMBUS.
  * @retval None
  */
__weak void HAL_FMPSMBUS_SlaveTxCpltCallback(FMPSMBUS_HandleTypeDef *hfmpsmbus)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hfmpsmbus);                                    /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_FMPSMBUS_SlaveTxCpltCallback() could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Slave Rx Transfer completed callback.
  * @param  hfmpsmbus Pointer to a FMPSMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPSMBUS.
  * @retval None
  */
__weak void HAL_FMPSMBUS_SlaveRxCpltCallback(FMPSMBUS_HandleTypeDef *hfmpsmbus)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hfmpsmbus);                                    /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_FMPSMBUS_SlaveRxCpltCallback() could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Slave Address Match callback.
  * @param  hfmpsmbus Pointer to a FMPSMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPSMBUS.
  * @param  TransferDirection Master request Transfer Direction (Write/Read)
  * @param  AddrMatchCode Address Match Code
  * @retval None
  */
__weak void HAL_FMPSMBUS_AddrCallback(FMPSMBUS_HandleTypeDef *hfmpsmbus, uint8_t TransferDirection, uint16_t AddrMatchCode)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hfmpsmbus);                                    /* 执行语句 */
  UNUSED(TransferDirection);                            /* 执行语句 */
  UNUSED(AddrMatchCode);                                /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_FMPSMBUS_AddrCallback() could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Listen Complete callback.
  * @param  hfmpsmbus Pointer to a FMPSMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPSMBUS.
  * @retval None
  */
__weak void HAL_FMPSMBUS_ListenCpltCallback(FMPSMBUS_HandleTypeDef *hfmpsmbus)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hfmpsmbus);                                    /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_FMPSMBUS_ListenCpltCallback() could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  FMPSMBUS error callback.
  * @param  hfmpsmbus Pointer to a FMPSMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPSMBUS.
  * @retval None
  */
__weak void HAL_FMPSMBUS_ErrorCallback(FMPSMBUS_HandleTypeDef *hfmpsmbus)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hfmpsmbus);                                    /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_FMPSMBUS_ErrorCallback() could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup FMPSMBUS_Exported_Functions_Group3 Peripheral State and Errors functions
  *  @brief   Peripheral State and Errors functions
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
  * @brief  Return the FMPSMBUS handle state.
  * @param  hfmpsmbus Pointer to a FMPSMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPSMBUS.
  * @retval HAL state
  */
uint32_t HAL_FMPSMBUS_GetState(FMPSMBUS_HandleTypeDef *hfmpsmbus)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  /* Return FMPSMBUS handle state */
  return hfmpsmbus->State;                              /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return the FMPSMBUS error code.
  * @param  hfmpsmbus Pointer to a FMPSMBUS_HandleTypeDef structure that contains
  *              the configuration information for the specified FMPSMBUS.
  * @retval FMPSMBUS Error Code
  */
uint32_t HAL_FMPSMBUS_GetError(FMPSMBUS_HandleTypeDef *hfmpsmbus)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  return hfmpsmbus->ErrorCode;                          /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup FMPSMBUS_Private_Functions FMPSMBUS Private Functions
  *  @brief   Data transfers Private functions
  * @{
  */

/**
  * @brief  Interrupt Sub-Routine which handle the Interrupt Flags Master Mode.
  * @param  hfmpsmbus Pointer to a FMPSMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPSMBUS.
  * @param  StatusFlags Value of Interrupt Flags.
  * @retval HAL status
  */
static HAL_StatusTypeDef FMPSMBUS_Master_ISR(FMPSMBUS_HandleTypeDef *hfmpsmbus, uint32_t StatusFlags)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  uint16_t DevAddress;                                  /* 16位无符号变量/函数 */

  /* Process Locked */
  __HAL_LOCK(hfmpsmbus);                                /* HAL辅助宏/函数 */

  if (FMPSMBUS_CHECK_FLAG(StatusFlags, FMPSMBUS_FLAG_AF) != RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Clear NACK Flag */
    __HAL_FMPSMBUS_CLEAR_FLAG(hfmpsmbus, FMPSMBUS_FLAG_AF);  /* HAL辅助宏/函数 */

    /* Set corresponding Error Code */
    /* No need to generate STOP, it is automatically done */
    hfmpsmbus->ErrorCode |= HAL_FMPSMBUS_ERROR_ACKF;    /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpsmbus);                            /* HAL辅助宏/函数 */

    /* Call the Error callback to inform upper layer */
#if (USE_HAL_FMPSMBUS_REGISTER_CALLBACKS == 1)          /* 条件编译 */
    hfmpsmbus->ErrorCallback(hfmpsmbus);                /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_FMPSMBUS_ErrorCallback(hfmpsmbus);              /* HAL库函数调用 */
#endif /* USE_HAL_FMPSMBUS_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
  else if (FMPSMBUS_CHECK_FLAG(StatusFlags, FMPSMBUS_FLAG_STOPF) != RESET)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* Check and treat errors if errors occurs during STOP process */
    FMPSMBUS_ITErrorHandler(hfmpsmbus);                 /* 执行语句 */

    /* Call the corresponding callback to inform upper layer of End of Transfer */
    if (hfmpsmbus->State == HAL_FMPSMBUS_STATE_MASTER_BUSY_TX)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable Interrupt */
      FMPSMBUS_Disable_IRQ(hfmpsmbus, FMPSMBUS_IT_TX);  /* 执行语句 */

      /* Clear STOP Flag */
      __HAL_FMPSMBUS_CLEAR_FLAG(hfmpsmbus, FMPSMBUS_FLAG_STOPF);  /* HAL辅助宏/函数 */

      /* Clear Configuration Register 2 */
      FMPSMBUS_RESET_CR2(hfmpsmbus);                    /* 执行语句 */

      /* Flush remaining data in Fifo register in case of error occurs before TXEmpty */
      /* Disable the selected FMPSMBUS peripheral */
      __HAL_FMPSMBUS_DISABLE(hfmpsmbus);                /* HAL辅助宏/函数 */

      hfmpsmbus->PreviousState = HAL_FMPSMBUS_STATE_READY;  /* 赋值或初始化 */
      hfmpsmbus->State = HAL_FMPSMBUS_STATE_READY;      /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hfmpsmbus);                          /* HAL辅助宏/函数 */

      /* Re-enable the selected FMPSMBUS peripheral */
      __HAL_FMPSMBUS_ENABLE(hfmpsmbus);                 /* HAL辅助宏/函数 */

      /* Call the corresponding callback to inform upper layer of End of Transfer */
#if (USE_HAL_FMPSMBUS_REGISTER_CALLBACKS == 1)          /* 条件编译 */
      hfmpsmbus->MasterTxCpltCallback(hfmpsmbus);       /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_FMPSMBUS_MasterTxCpltCallback(hfmpsmbus);     /* HAL库函数调用 */
#endif /* USE_HAL_FMPSMBUS_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
    else if (hfmpsmbus->State == HAL_FMPSMBUS_STATE_MASTER_BUSY_RX)  /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* Store Last receive data if any */
      if (FMPSMBUS_CHECK_FLAG(StatusFlags, FMPSMBUS_FLAG_RXNE) != RESET)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Read data from RXDR */
        *hfmpsmbus->pBuffPtr = (uint8_t)(hfmpsmbus->Instance->RXDR);  /* 赋值或初始化 */

        /* Increment Buffer pointer */
        hfmpsmbus->pBuffPtr++;                          /* 执行语句 */

        if ((hfmpsmbus->XferSize > 0U))                 /* 条件判断 */
        {                                               /* 代码块开始 */
          hfmpsmbus->XferSize--;                        /* 执行语句 */
          hfmpsmbus->XferCount--;                       /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */

      /* Disable Interrupt */
      FMPSMBUS_Disable_IRQ(hfmpsmbus, FMPSMBUS_IT_RX);  /* 执行语句 */

      /* Clear STOP Flag */
      __HAL_FMPSMBUS_CLEAR_FLAG(hfmpsmbus, FMPSMBUS_FLAG_STOPF);  /* HAL辅助宏/函数 */

      /* Clear Configuration Register 2 */
      FMPSMBUS_RESET_CR2(hfmpsmbus);                    /* 执行语句 */

      hfmpsmbus->PreviousState = HAL_FMPSMBUS_STATE_READY;  /* 赋值或初始化 */
      hfmpsmbus->State = HAL_FMPSMBUS_STATE_READY;      /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hfmpsmbus);                          /* HAL辅助宏/函数 */

      /* Call the corresponding callback to inform upper layer of End of Transfer */
#if (USE_HAL_FMPSMBUS_REGISTER_CALLBACKS == 1)          /* 条件编译 */
      hfmpsmbus->MasterRxCpltCallback(hfmpsmbus);       /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_FMPSMBUS_MasterRxCpltCallback(hfmpsmbus);     /* HAL库函数调用 */
#endif /* USE_HAL_FMPSMBUS_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Nothing to do */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (FMPSMBUS_CHECK_FLAG(StatusFlags, FMPSMBUS_FLAG_RXNE) != RESET)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* Read data from RXDR */
    *hfmpsmbus->pBuffPtr = (uint8_t)(hfmpsmbus->Instance->RXDR);  /* 赋值或初始化 */

    /* Increment Buffer pointer */
    hfmpsmbus->pBuffPtr++;                              /* 执行语句 */

    /* Increment Size counter */
    hfmpsmbus->XferSize--;                              /* 执行语句 */
    hfmpsmbus->XferCount--;                             /* 执行语句 */
  }                                                     /* 代码块结束 */
  else if (FMPSMBUS_CHECK_FLAG(StatusFlags, FMPSMBUS_FLAG_TXIS) != RESET)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* Write data to TXDR */
    hfmpsmbus->Instance->TXDR = *hfmpsmbus->pBuffPtr;   /* 赋值或初始化 */

    /* Increment Buffer pointer */
    hfmpsmbus->pBuffPtr++;                              /* 执行语句 */

    /* Increment Size counter */
    hfmpsmbus->XferSize--;                              /* 执行语句 */
    hfmpsmbus->XferCount--;                             /* 执行语句 */
  }                                                     /* 代码块结束 */
  else if (FMPSMBUS_CHECK_FLAG(StatusFlags, FMPSMBUS_FLAG_TCR) != RESET)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    if ((hfmpsmbus->XferCount != 0U) && (hfmpsmbus->XferSize == 0U))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      DevAddress = (uint16_t)(hfmpsmbus->Instance->CR2 & FMPI2C_CR2_SADD);  /* 赋值或初始化 */

      if (hfmpsmbus->XferCount > MAX_NBYTE_SIZE)        /* 条件判断 */
      {                                                 /* 代码块开始 */
        FMPSMBUS_TransferConfig(hfmpsmbus, DevAddress, MAX_NBYTE_SIZE,  /* 列表续行 */
                             (FMPSMBUS_RELOAD_MODE | (hfmpsmbus->XferOptions & FMPSMBUS_SENDPEC_MODE)), FMPSMBUS_NO_STARTSTOP);  /* 执行语句 */
        hfmpsmbus->XferSize = MAX_NBYTE_SIZE;           /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        hfmpsmbus->XferSize = hfmpsmbus->XferCount;     /* 赋值或初始化 */
        FMPSMBUS_TransferConfig(hfmpsmbus, DevAddress, (uint8_t)hfmpsmbus->XferSize, hfmpsmbus->XferOptions, FMPSMBUS_NO_STARTSTOP);  /* 执行语句 */
        /* If PEC mode is enable, size to transmit should be Size-1 byte, corresponding to PEC byte */
        /* PEC byte is automatically sent by HW block, no need to manage it in Transmit process */
        if (FMPSMBUS_GET_PEC_MODE(hfmpsmbus) != 0UL)    /* 条件判断 */
        {                                               /* 代码块开始 */
          hfmpsmbus->XferSize--;                        /* 执行语句 */
          hfmpsmbus->XferCount--;                       /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else if ((hfmpsmbus->XferCount == 0U) && (hfmpsmbus->XferSize == 0U))  /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* Call TxCpltCallback() if no stop mode is set */
      if (FMPSMBUS_GET_STOP_MODE(hfmpsmbus) != FMPSMBUS_AUTOEND_MODE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Call the corresponding callback to inform upper layer of End of Transfer */
        if (hfmpsmbus->State == HAL_FMPSMBUS_STATE_MASTER_BUSY_TX)  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Disable Interrupt */
          FMPSMBUS_Disable_IRQ(hfmpsmbus, FMPSMBUS_IT_TX);  /* 执行语句 */
          hfmpsmbus->PreviousState = hfmpsmbus->State;  /* 赋值或初始化 */
          hfmpsmbus->State = HAL_FMPSMBUS_STATE_READY;  /* 赋值或初始化 */

          /* Process Unlocked */
          __HAL_UNLOCK(hfmpsmbus);                      /* HAL辅助宏/函数 */

          /* Call the corresponding callback to inform upper layer of End of Transfer */
#if (USE_HAL_FMPSMBUS_REGISTER_CALLBACKS == 1)          /* 条件编译 */
          hfmpsmbus->MasterTxCpltCallback(hfmpsmbus);   /* 执行语句 */
#else                                                   /* 条件编译否则 */
          HAL_FMPSMBUS_MasterTxCpltCallback(hfmpsmbus);  /* HAL库函数调用 */
#endif /* USE_HAL_FMPSMBUS_REGISTER_CALLBACKS */
        }                                               /* 代码块结束 */
        else if (hfmpsmbus->State == HAL_FMPSMBUS_STATE_MASTER_BUSY_RX)  /* 否则如果 */
        {                                               /* 代码块开始 */
          FMPSMBUS_Disable_IRQ(hfmpsmbus, FMPSMBUS_IT_RX);  /* 执行语句 */
          hfmpsmbus->PreviousState = hfmpsmbus->State;  /* 赋值或初始化 */
          hfmpsmbus->State = HAL_FMPSMBUS_STATE_READY;  /* 赋值或初始化 */

          /* Process Unlocked */
          __HAL_UNLOCK(hfmpsmbus);                      /* HAL辅助宏/函数 */

          /* Call the corresponding callback to inform upper layer of End of Transfer */
#if (USE_HAL_FMPSMBUS_REGISTER_CALLBACKS == 1)          /* 条件编译 */
          hfmpsmbus->MasterRxCpltCallback(hfmpsmbus);   /* 执行语句 */
#else                                                   /* 条件编译否则 */
          HAL_FMPSMBUS_MasterRxCpltCallback(hfmpsmbus);  /* HAL库函数调用 */
#endif /* USE_HAL_FMPSMBUS_REGISTER_CALLBACKS */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          /* Nothing to do */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Nothing to do */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (FMPSMBUS_CHECK_FLAG(StatusFlags, FMPSMBUS_FLAG_TC) != RESET)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    if (hfmpsmbus->XferCount == 0U)                     /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Specific use case for Quick command */
      if (hfmpsmbus->pBuffPtr == NULL)                  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Generate a Stop command */
        hfmpsmbus->Instance->CR2 |= FMPI2C_CR2_STOP;    /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      /* Call TxCpltCallback() if no stop mode is set */
      else if (FMPSMBUS_GET_STOP_MODE(hfmpsmbus) != FMPSMBUS_AUTOEND_MODE)  /* 否则如果 */
      {                                                 /* 代码块开始 */
        /* No Generate Stop, to permit restart mode */
        /* The stop will be done at the end of transfer, when FMPSMBUS_AUTOEND_MODE enable */

        /* Call the corresponding callback to inform upper layer of End of Transfer */
        if (hfmpsmbus->State == HAL_FMPSMBUS_STATE_MASTER_BUSY_TX)  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Disable Interrupt */
          FMPSMBUS_Disable_IRQ(hfmpsmbus, FMPSMBUS_IT_TX);  /* 执行语句 */
          hfmpsmbus->PreviousState = hfmpsmbus->State;  /* 赋值或初始化 */
          hfmpsmbus->State = HAL_FMPSMBUS_STATE_READY;  /* 赋值或初始化 */

          /* Process Unlocked */
          __HAL_UNLOCK(hfmpsmbus);                      /* HAL辅助宏/函数 */

          /* Call the corresponding callback to inform upper layer of End of Transfer */
#if (USE_HAL_FMPSMBUS_REGISTER_CALLBACKS == 1)          /* 条件编译 */
          hfmpsmbus->MasterTxCpltCallback(hfmpsmbus);   /* 执行语句 */
#else                                                   /* 条件编译否则 */
          HAL_FMPSMBUS_MasterTxCpltCallback(hfmpsmbus);  /* HAL库函数调用 */
#endif /* USE_HAL_FMPSMBUS_REGISTER_CALLBACKS */
        }                                               /* 代码块结束 */
        else if (hfmpsmbus->State == HAL_FMPSMBUS_STATE_MASTER_BUSY_RX)  /* 否则如果 */
        {                                               /* 代码块开始 */
          FMPSMBUS_Disable_IRQ(hfmpsmbus, FMPSMBUS_IT_RX);  /* 执行语句 */
          hfmpsmbus->PreviousState = hfmpsmbus->State;  /* 赋值或初始化 */
          hfmpsmbus->State = HAL_FMPSMBUS_STATE_READY;  /* 赋值或初始化 */

          /* Process Unlocked */
          __HAL_UNLOCK(hfmpsmbus);                      /* HAL辅助宏/函数 */

          /* Call the corresponding callback to inform upper layer of End of Transfer */
#if (USE_HAL_FMPSMBUS_REGISTER_CALLBACKS == 1)          /* 条件编译 */
          hfmpsmbus->MasterRxCpltCallback(hfmpsmbus);   /* 执行语句 */
#else                                                   /* 条件编译否则 */
          HAL_FMPSMBUS_MasterRxCpltCallback(hfmpsmbus);  /* HAL库函数调用 */
#endif /* USE_HAL_FMPSMBUS_REGISTER_CALLBACKS */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          /* Nothing to do */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Nothing to do */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Nothing to do */
  }                                                     /* 代码块结束 */

  /* Process Unlocked */
  __HAL_UNLOCK(hfmpsmbus);                              /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */
/**
  * @brief  Interrupt Sub-Routine which handle the Interrupt Flags Slave Mode.
  * @param  hfmpsmbus Pointer to a FMPSMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPSMBUS.
  * @param  StatusFlags Value of Interrupt Flags.
  * @retval HAL status
  */
static HAL_StatusTypeDef FMPSMBUS_Slave_ISR(FMPSMBUS_HandleTypeDef *hfmpsmbus, uint32_t StatusFlags)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  uint8_t TransferDirection;                            /* 8位无符号变量/函数 */
  uint16_t SlaveAddrCode;                               /* 16位无符号变量/函数 */

  /* Process Locked */
  __HAL_LOCK(hfmpsmbus);                                /* HAL辅助宏/函数 */

  if (FMPSMBUS_CHECK_FLAG(StatusFlags, FMPSMBUS_FLAG_AF) != RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check that FMPSMBUS transfer finished */
    /* if yes, normal usecase, a NACK is sent by the HOST when Transfer is finished */
    /* Mean XferCount == 0*/
    /* So clear Flag NACKF only */
    if (hfmpsmbus->XferCount == 0U)                     /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear NACK Flag */
      __HAL_FMPSMBUS_CLEAR_FLAG(hfmpsmbus, FMPSMBUS_FLAG_AF);  /* HAL辅助宏/函数 */

      /* Process Unlocked */
      __HAL_UNLOCK(hfmpsmbus);                          /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* if no, error usecase, a Non-Acknowledge of last Data is generated by the HOST*/
      /* Clear NACK Flag */
      __HAL_FMPSMBUS_CLEAR_FLAG(hfmpsmbus, FMPSMBUS_FLAG_AF);  /* HAL辅助宏/函数 */

      /* Set HAL State to "Idle" State, mean to LISTEN state */
      /* So reset Slave Busy state */
      hfmpsmbus->PreviousState = hfmpsmbus->State;      /* 赋值或初始化 */
      hfmpsmbus->State &= ~((uint32_t)HAL_FMPSMBUS_STATE_SLAVE_BUSY_TX);  /* 赋值或初始化 */
      hfmpsmbus->State &= ~((uint32_t)HAL_FMPSMBUS_STATE_SLAVE_BUSY_RX);  /* 赋值或初始化 */

      /* Disable RX/TX Interrupts, keep only ADDR Interrupt */
      FMPSMBUS_Disable_IRQ(hfmpsmbus, FMPSMBUS_IT_RX | FMPSMBUS_IT_TX);  /* 执行语句 */

      /* Set ErrorCode corresponding to a Non-Acknowledge */
      hfmpsmbus->ErrorCode |= HAL_FMPSMBUS_ERROR_ACKF;  /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hfmpsmbus);                          /* HAL辅助宏/函数 */

      /* Call the Error callback to inform upper layer */
#if (USE_HAL_FMPSMBUS_REGISTER_CALLBACKS == 1)          /* 条件编译 */
      hfmpsmbus->ErrorCallback(hfmpsmbus);              /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_FMPSMBUS_ErrorCallback(hfmpsmbus);            /* HAL库函数调用 */
#endif /* USE_HAL_FMPSMBUS_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (FMPSMBUS_CHECK_FLAG(StatusFlags, FMPSMBUS_FLAG_ADDR) != RESET)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    TransferDirection = (uint8_t)(FMPSMBUS_GET_DIR(hfmpsmbus));  /* 赋值或初始化 */
    SlaveAddrCode = (uint16_t)(FMPSMBUS_GET_ADDR_MATCH(hfmpsmbus));  /* 赋值或初始化 */

    /* Disable ADDR interrupt to prevent multiple ADDRInterrupt*/
    /* Other ADDRInterrupt will be treat in next Listen usecase */
    __HAL_FMPSMBUS_DISABLE_IT(hfmpsmbus, FMPSMBUS_IT_ADDRI);  /* HAL辅助宏/函数 */

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpsmbus);                            /* HAL辅助宏/函数 */

    /* Call Slave Addr callback */
#if (USE_HAL_FMPSMBUS_REGISTER_CALLBACKS == 1)          /* 条件编译 */
    hfmpsmbus->AddrCallback(hfmpsmbus, TransferDirection, SlaveAddrCode);  /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_FMPSMBUS_AddrCallback(hfmpsmbus, TransferDirection, SlaveAddrCode);  /* HAL库函数调用 */
#endif /* USE_HAL_FMPSMBUS_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
  else if ((FMPSMBUS_CHECK_FLAG(StatusFlags, FMPSMBUS_FLAG_RXNE) != RESET) ||  /* 否则如果 */
           (FMPSMBUS_CHECK_FLAG(StatusFlags, FMPSMBUS_FLAG_TCR) != RESET))  /* 源代码 */
  {                                                     /* 代码块开始 */
    if ((hfmpsmbus->State & HAL_FMPSMBUS_STATE_SLAVE_BUSY_RX) == HAL_FMPSMBUS_STATE_SLAVE_BUSY_RX)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Read data from RXDR */
      *hfmpsmbus->pBuffPtr = (uint8_t)(hfmpsmbus->Instance->RXDR);  /* 赋值或初始化 */

      /* Increment Buffer pointer */
      hfmpsmbus->pBuffPtr++;                            /* 执行语句 */

      hfmpsmbus->XferSize--;                            /* 执行语句 */
      hfmpsmbus->XferCount--;                           /* 执行语句 */

      if (hfmpsmbus->XferCount == 1U)                   /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Receive last Byte, can be PEC byte in case of PEC BYTE enabled */
        /* or only the last Byte of Transfer */
        /* So reset the RELOAD bit mode */
        hfmpsmbus->XferOptions &= ~FMPSMBUS_RELOAD_MODE;  /* 赋值或初始化 */
        FMPSMBUS_TransferConfig(hfmpsmbus, 0, 1, hfmpsmbus->XferOptions, FMPSMBUS_NO_STARTSTOP);  /* 执行语句 */
      }                                                 /* 代码块结束 */
      else if (hfmpsmbus->XferCount == 0U)              /* 否则如果 */
      {                                                 /* 代码块开始 */
        /* Last Byte is received, disable Interrupt */
        FMPSMBUS_Disable_IRQ(hfmpsmbus, FMPSMBUS_IT_RX);  /* 执行语句 */

        /* Remove HAL_FMPSMBUS_STATE_SLAVE_BUSY_RX, keep only HAL_FMPSMBUS_STATE_LISTEN */
        hfmpsmbus->PreviousState = hfmpsmbus->State;    /* 赋值或初始化 */
        hfmpsmbus->State &= ~((uint32_t)HAL_FMPSMBUS_STATE_SLAVE_BUSY_RX);  /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hfmpsmbus);                        /* HAL辅助宏/函数 */

        /* Call the corresponding callback to inform upper layer of End of Transfer */
#if (USE_HAL_FMPSMBUS_REGISTER_CALLBACKS == 1)          /* 条件编译 */
        hfmpsmbus->SlaveRxCpltCallback(hfmpsmbus);      /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_FMPSMBUS_SlaveRxCpltCallback(hfmpsmbus);    /* HAL库函数调用 */
#endif /* USE_HAL_FMPSMBUS_REGISTER_CALLBACKS */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Set Reload for next Bytes */
        FMPSMBUS_TransferConfig(hfmpsmbus, 0, 1, FMPSMBUS_RELOAD_MODE  | (hfmpsmbus->XferOptions & FMPSMBUS_SENDPEC_MODE), FMPSMBUS_NO_STARTSTOP);  /* 执行语句 */

        /* Ack last Byte Read */
        hfmpsmbus->Instance->CR2 &= ~FMPI2C_CR2_NACK;   /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else if ((hfmpsmbus->State & HAL_FMPSMBUS_STATE_SLAVE_BUSY_TX) == HAL_FMPSMBUS_STATE_SLAVE_BUSY_TX)  /* 否则如果 */
    {                                                   /* 代码块开始 */
      if ((hfmpsmbus->XferCount != 0U) && (hfmpsmbus->XferSize == 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        if (hfmpsmbus->XferCount > MAX_NBYTE_SIZE)      /* 条件判断 */
        {                                               /* 代码块开始 */
          FMPSMBUS_TransferConfig(hfmpsmbus, 0, MAX_NBYTE_SIZE, (FMPSMBUS_RELOAD_MODE | (hfmpsmbus->XferOptions & FMPSMBUS_SENDPEC_MODE)),  /* 列表续行 */
                               FMPSMBUS_NO_STARTSTOP);  /* 执行语句 */
          hfmpsmbus->XferSize = MAX_NBYTE_SIZE;         /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          hfmpsmbus->XferSize = hfmpsmbus->XferCount;   /* 赋值或初始化 */
          FMPSMBUS_TransferConfig(hfmpsmbus, 0, (uint8_t)hfmpsmbus->XferSize, hfmpsmbus->XferOptions, FMPSMBUS_NO_STARTSTOP);  /* 执行语句 */
          /* If PEC mode is enable, size to transmit should be Size-1 byte, corresponding to PEC byte */
          /* PEC byte is automatically sent by HW block, no need to manage it in Transmit process */
          if (FMPSMBUS_GET_PEC_MODE(hfmpsmbus) != 0UL)  /* 条件判断 */
          {                                             /* 代码块开始 */
            hfmpsmbus->XferSize--;                      /* 执行语句 */
            hfmpsmbus->XferCount--;                     /* 执行语句 */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Nothing to do */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (FMPSMBUS_CHECK_FLAG(StatusFlags, FMPSMBUS_FLAG_TXIS) != RESET)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* Write data to TXDR only if XferCount not reach "0" */
    /* A TXIS flag can be set, during STOP treatment      */
    /* Check if all Data have already been sent */
    /* If it is the case, this last write in TXDR is not sent, correspond to a dummy TXIS event */
    if (hfmpsmbus->XferCount > 0U)                      /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Write data to TXDR */
      hfmpsmbus->Instance->TXDR = *hfmpsmbus->pBuffPtr;  /* 赋值或初始化 */

      /* Increment Buffer pointer */
      hfmpsmbus->pBuffPtr++;                            /* 执行语句 */

      hfmpsmbus->XferCount--;                           /* 执行语句 */
      hfmpsmbus->XferSize--;                            /* 执行语句 */
    }                                                   /* 代码块结束 */

    if (hfmpsmbus->XferCount == 0U)                     /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Last Byte is Transmitted */
      /* Remove HAL_FMPSMBUS_STATE_SLAVE_BUSY_TX, keep only HAL_FMPSMBUS_STATE_LISTEN */
      FMPSMBUS_Disable_IRQ(hfmpsmbus, FMPSMBUS_IT_TX);  /* 执行语句 */
      hfmpsmbus->PreviousState = hfmpsmbus->State;      /* 赋值或初始化 */
      hfmpsmbus->State &= ~((uint32_t)HAL_FMPSMBUS_STATE_SLAVE_BUSY_TX);  /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hfmpsmbus);                          /* HAL辅助宏/函数 */

      /* Call the corresponding callback to inform upper layer of End of Transfer */
#if (USE_HAL_FMPSMBUS_REGISTER_CALLBACKS == 1)          /* 条件编译 */
      hfmpsmbus->SlaveTxCpltCallback(hfmpsmbus);        /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_FMPSMBUS_SlaveTxCpltCallback(hfmpsmbus);      /* HAL库函数调用 */
#endif /* USE_HAL_FMPSMBUS_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Nothing to do */
  }                                                     /* 代码块结束 */

  /* Check if STOPF is set */
  if (FMPSMBUS_CHECK_FLAG(StatusFlags, FMPSMBUS_FLAG_STOPF) != RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((hfmpsmbus->State & HAL_FMPSMBUS_STATE_LISTEN) == HAL_FMPSMBUS_STATE_LISTEN)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Store Last receive data if any */
      if (__HAL_FMPSMBUS_GET_FLAG(hfmpsmbus, FMPSMBUS_FLAG_RXNE) != RESET)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Read data from RXDR */
        *hfmpsmbus->pBuffPtr = (uint8_t)(hfmpsmbus->Instance->RXDR);  /* 赋值或初始化 */

        /* Increment Buffer pointer */
        hfmpsmbus->pBuffPtr++;                          /* 执行语句 */

        if ((hfmpsmbus->XferSize > 0U))                 /* 条件判断 */
        {                                               /* 代码块开始 */
          hfmpsmbus->XferSize--;                        /* 执行语句 */
          hfmpsmbus->XferCount--;                       /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */

      /* Disable RX and TX Interrupts */
      FMPSMBUS_Disable_IRQ(hfmpsmbus, FMPSMBUS_IT_RX | FMPSMBUS_IT_TX);  /* 执行语句 */

      /* Disable ADDR Interrupt */
      FMPSMBUS_Disable_IRQ(hfmpsmbus, FMPSMBUS_IT_ADDR);  /* 执行语句 */

      /* Disable Address Acknowledge */
      hfmpsmbus->Instance->CR2 |= FMPI2C_CR2_NACK;      /* 赋值或初始化 */

      /* Clear Configuration Register 2 */
      FMPSMBUS_RESET_CR2(hfmpsmbus);                    /* 执行语句 */

      /* Clear STOP Flag */
      __HAL_FMPSMBUS_CLEAR_FLAG(hfmpsmbus, FMPSMBUS_FLAG_STOPF);  /* HAL辅助宏/函数 */

      /* Clear ADDR flag */
      __HAL_FMPSMBUS_CLEAR_FLAG(hfmpsmbus, FMPSMBUS_FLAG_ADDR);  /* HAL辅助宏/函数 */

      hfmpsmbus->XferOptions = 0;                       /* 赋值或初始化 */
      hfmpsmbus->PreviousState = hfmpsmbus->State;      /* 赋值或初始化 */
      hfmpsmbus->State = HAL_FMPSMBUS_STATE_READY;      /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hfmpsmbus);                          /* HAL辅助宏/函数 */

      /* Call the Listen Complete callback, to inform upper layer of the end of Listen usecase */
#if (USE_HAL_FMPSMBUS_REGISTER_CALLBACKS == 1)          /* 条件编译 */
      hfmpsmbus->ListenCpltCallback(hfmpsmbus);         /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_FMPSMBUS_ListenCpltCallback(hfmpsmbus);       /* HAL库函数调用 */
#endif /* USE_HAL_FMPSMBUS_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Process Unlocked */
  __HAL_UNLOCK(hfmpsmbus);                              /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */
/**
  * @brief  Manage the enabling of Interrupts.
  * @param  hfmpsmbus Pointer to a FMPSMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPSMBUS.
  * @param  InterruptRequest Value of @ref FMPSMBUS_Interrupt_configuration_definition.
  * @retval HAL status
  */
static void FMPSMBUS_Enable_IRQ(FMPSMBUS_HandleTypeDef *hfmpsmbus, uint32_t InterruptRequest)  /* 静态函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpisr = 0UL;                                /* 32位无符号变量/函数 */

  if ((InterruptRequest & FMPSMBUS_IT_ALERT) == FMPSMBUS_IT_ALERT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable ERR interrupt */
    tmpisr |= FMPSMBUS_IT_ERRI;                         /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if ((InterruptRequest & FMPSMBUS_IT_ADDR) == FMPSMBUS_IT_ADDR)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable ADDR, STOP interrupt */
    tmpisr |= FMPSMBUS_IT_ADDRI | FMPSMBUS_IT_STOPI | FMPSMBUS_IT_NACKI | FMPSMBUS_IT_ERRI;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if ((InterruptRequest & FMPSMBUS_IT_TX) == FMPSMBUS_IT_TX)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable ERR, TC, STOP, NACK, RXI interrupt */
    tmpisr |= FMPSMBUS_IT_ERRI | FMPSMBUS_IT_TCI | FMPSMBUS_IT_STOPI | FMPSMBUS_IT_NACKI | FMPSMBUS_IT_TXI;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if ((InterruptRequest & FMPSMBUS_IT_RX) == FMPSMBUS_IT_RX)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable ERR, TC, STOP, NACK, TXI interrupt */
    tmpisr |= FMPSMBUS_IT_ERRI | FMPSMBUS_IT_TCI | FMPSMBUS_IT_STOPI | FMPSMBUS_IT_NACKI | FMPSMBUS_IT_RXI;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Enable interrupts only at the end */
  /* to avoid the risk of FMPSMBUS interrupt handle execution before */
  /* all interrupts requested done */
  __HAL_FMPSMBUS_ENABLE_IT(hfmpsmbus, tmpisr);          /* HAL辅助宏/函数 */
}                                                       /* 代码块结束 */
/**
  * @brief  Manage the disabling of Interrupts.
  * @param  hfmpsmbus Pointer to a FMPSMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPSMBUS.
  * @param  InterruptRequest Value of @ref FMPSMBUS_Interrupt_configuration_definition.
  * @retval HAL status
  */
static void FMPSMBUS_Disable_IRQ(FMPSMBUS_HandleTypeDef *hfmpsmbus, uint32_t InterruptRequest)  /* 静态函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpisr = 0UL;                                /* 32位无符号变量/函数 */
  uint32_t tmpstate = hfmpsmbus->State;                 /* 32位无符号变量/函数 */

  if ((tmpstate == HAL_FMPSMBUS_STATE_READY) && ((InterruptRequest & FMPSMBUS_IT_ALERT) == FMPSMBUS_IT_ALERT))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable ERR interrupt */
    tmpisr |= FMPSMBUS_IT_ERRI;                         /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if ((InterruptRequest & FMPSMBUS_IT_TX) == FMPSMBUS_IT_TX)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable TC, STOP, NACK and TXI interrupt */
    tmpisr |= FMPSMBUS_IT_TCI | FMPSMBUS_IT_TXI;        /* 赋值或初始化 */

    if ((FMPSMBUS_GET_ALERT_ENABLED(hfmpsmbus) == 0UL)  /* 条件判断 */
        && ((tmpstate & HAL_FMPSMBUS_STATE_LISTEN) != HAL_FMPSMBUS_STATE_LISTEN))  /* 源代码 */
    {                                                   /* 代码块开始 */
      /* Disable ERR interrupt */
      tmpisr |= FMPSMBUS_IT_ERRI;                       /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    if ((tmpstate & HAL_FMPSMBUS_STATE_LISTEN) != HAL_FMPSMBUS_STATE_LISTEN)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable STOP and NACK interrupt */
      tmpisr |= FMPSMBUS_IT_STOPI | FMPSMBUS_IT_NACKI;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  if ((InterruptRequest & FMPSMBUS_IT_RX) == FMPSMBUS_IT_RX)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable TC, STOP, NACK and RXI interrupt */
    tmpisr |= FMPSMBUS_IT_TCI | FMPSMBUS_IT_RXI;        /* 赋值或初始化 */

    if ((FMPSMBUS_GET_ALERT_ENABLED(hfmpsmbus) == 0UL)  /* 条件判断 */
        && ((tmpstate & HAL_FMPSMBUS_STATE_LISTEN) != HAL_FMPSMBUS_STATE_LISTEN))  /* 源代码 */
    {                                                   /* 代码块开始 */
      /* Disable ERR interrupt */
      tmpisr |= FMPSMBUS_IT_ERRI;                       /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    if ((tmpstate & HAL_FMPSMBUS_STATE_LISTEN) != HAL_FMPSMBUS_STATE_LISTEN)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable STOP and NACK interrupt */
      tmpisr |= FMPSMBUS_IT_STOPI | FMPSMBUS_IT_NACKI;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  if ((InterruptRequest & FMPSMBUS_IT_ADDR) == FMPSMBUS_IT_ADDR)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable ADDR, STOP and NACK interrupt */
    tmpisr |= FMPSMBUS_IT_ADDRI | FMPSMBUS_IT_STOPI | FMPSMBUS_IT_NACKI;  /* 赋值或初始化 */

    if (FMPSMBUS_GET_ALERT_ENABLED(hfmpsmbus) == 0UL)   /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable ERR interrupt */
      tmpisr |= FMPSMBUS_IT_ERRI;                       /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Disable interrupts only at the end */
  /* to avoid a breaking situation like at "t" time */
  /* all disable interrupts request are not done */
  __HAL_FMPSMBUS_DISABLE_IT(hfmpsmbus, tmpisr);         /* HAL辅助宏/函数 */
}                                                       /* 代码块结束 */

/**
  * @brief  FMPSMBUS interrupts error handler.
  * @param  hfmpsmbus FMPSMBUS handle.
  * @retval None
  */
static void FMPSMBUS_ITErrorHandler(FMPSMBUS_HandleTypeDef *hfmpsmbus)  /* 静态函数 */
{                                                       /* 代码块开始 */
  uint32_t itflags   = READ_REG(hfmpsmbus->Instance->ISR);  /* 32位无符号变量/函数 */
  uint32_t itsources = READ_REG(hfmpsmbus->Instance->CR1);  /* 32位无符号变量/函数 */
  uint32_t tmpstate;                                    /* 32位无符号变量/函数 */
  uint32_t tmperror;                                    /* 32位无符号变量/函数 */

  /* FMPSMBUS Bus error interrupt occurred ------------------------------------*/
  if (((itflags & FMPSMBUS_FLAG_BERR) == FMPSMBUS_FLAG_BERR) && ((itsources & FMPSMBUS_IT_ERRI) == FMPSMBUS_IT_ERRI))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hfmpsmbus->ErrorCode |= HAL_FMPSMBUS_ERROR_BERR;    /* 赋值或初始化 */

    /* Clear BERR flag */
    __HAL_FMPSMBUS_CLEAR_FLAG(hfmpsmbus, FMPSMBUS_FLAG_BERR);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* FMPSMBUS Over-Run/Under-Run interrupt occurred ----------------------------------------*/
  if (((itflags & FMPSMBUS_FLAG_OVR) == FMPSMBUS_FLAG_OVR) && ((itsources & FMPSMBUS_IT_ERRI) == FMPSMBUS_IT_ERRI))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hfmpsmbus->ErrorCode |= HAL_FMPSMBUS_ERROR_OVR;     /* 赋值或初始化 */

    /* Clear OVR flag */
    __HAL_FMPSMBUS_CLEAR_FLAG(hfmpsmbus, FMPSMBUS_FLAG_OVR);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* FMPSMBUS Arbitration Loss error interrupt occurred ------------------------------------*/
  if (((itflags & FMPSMBUS_FLAG_ARLO) == FMPSMBUS_FLAG_ARLO) && ((itsources & FMPSMBUS_IT_ERRI) == FMPSMBUS_IT_ERRI))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hfmpsmbus->ErrorCode |= HAL_FMPSMBUS_ERROR_ARLO;    /* 赋值或初始化 */

    /* Clear ARLO flag */
    __HAL_FMPSMBUS_CLEAR_FLAG(hfmpsmbus, FMPSMBUS_FLAG_ARLO);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* FMPSMBUS Timeout error interrupt occurred ---------------------------------------------*/
  if (((itflags & FMPSMBUS_FLAG_TIMEOUT) == FMPSMBUS_FLAG_TIMEOUT) && ((itsources & FMPSMBUS_IT_ERRI) == FMPSMBUS_IT_ERRI))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hfmpsmbus->ErrorCode |= HAL_FMPSMBUS_ERROR_BUSTIMEOUT;  /* 赋值或初始化 */

    /* Clear TIMEOUT flag */
    __HAL_FMPSMBUS_CLEAR_FLAG(hfmpsmbus, FMPSMBUS_FLAG_TIMEOUT);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* FMPSMBUS Alert error interrupt occurred -----------------------------------------------*/
  if (((itflags & FMPSMBUS_FLAG_ALERT) == FMPSMBUS_FLAG_ALERT) && ((itsources & FMPSMBUS_IT_ERRI) == FMPSMBUS_IT_ERRI))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hfmpsmbus->ErrorCode |= HAL_FMPSMBUS_ERROR_ALERT;   /* 赋值或初始化 */

    /* Clear ALERT flag */
    __HAL_FMPSMBUS_CLEAR_FLAG(hfmpsmbus, FMPSMBUS_FLAG_ALERT);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* FMPSMBUS Packet Error Check error interrupt occurred ----------------------------------*/
  if (((itflags & FMPSMBUS_FLAG_PECERR) == FMPSMBUS_FLAG_PECERR) && ((itsources & FMPSMBUS_IT_ERRI) == FMPSMBUS_IT_ERRI))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hfmpsmbus->ErrorCode |= HAL_FMPSMBUS_ERROR_PECERR;  /* 赋值或初始化 */

    /* Clear PEC error flag */
    __HAL_FMPSMBUS_CLEAR_FLAG(hfmpsmbus, FMPSMBUS_FLAG_PECERR);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Store current volatile hfmpsmbus->State, misra rule */
  tmperror = hfmpsmbus->ErrorCode;                      /* 赋值或初始化 */

  /* Call the Error Callback in case of Error detected */
  if ((tmperror != HAL_FMPSMBUS_ERROR_NONE) && (tmperror != HAL_FMPSMBUS_ERROR_ACKF))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Do not Reset the HAL state in case of ALERT error */
    if ((tmperror & HAL_FMPSMBUS_ERROR_ALERT) != HAL_FMPSMBUS_ERROR_ALERT)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Store current volatile hfmpsmbus->State, misra rule */
      tmpstate = hfmpsmbus->State;                      /* 赋值或初始化 */

      if (((tmpstate & HAL_FMPSMBUS_STATE_SLAVE_BUSY_TX) == HAL_FMPSMBUS_STATE_SLAVE_BUSY_TX)  /* 条件判断 */
          || ((tmpstate & HAL_FMPSMBUS_STATE_SLAVE_BUSY_RX) == HAL_FMPSMBUS_STATE_SLAVE_BUSY_RX))  /* 源代码 */
      {                                                 /* 代码块开始 */
        /* Reset only HAL_FMPSMBUS_STATE_SLAVE_BUSY_XX */
        /* keep HAL_FMPSMBUS_STATE_LISTEN if set */
        hfmpsmbus->PreviousState = HAL_FMPSMBUS_STATE_READY;  /* 赋值或初始化 */
        hfmpsmbus->State = HAL_FMPSMBUS_STATE_LISTEN;   /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Call the Error callback to inform upper layer */
#if (USE_HAL_FMPSMBUS_REGISTER_CALLBACKS == 1)          /* 条件编译 */
    hfmpsmbus->ErrorCallback(hfmpsmbus);                /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_FMPSMBUS_ErrorCallback(hfmpsmbus);              /* HAL库函数调用 */
#endif /* USE_HAL_FMPSMBUS_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Handle FMPSMBUS Communication Timeout.
  * @param  hfmpsmbus Pointer to a FMPSMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPSMBUS.
  * @param  Flag Specifies the FMPSMBUS flag to check.
  * @param  Status The new Flag status (SET or RESET).
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
static HAL_StatusTypeDef FMPSMBUS_WaitOnFlagUntilTimeout(FMPSMBUS_HandleTypeDef *hfmpsmbus, uint32_t Flag, FlagStatus Status,  /* 静态变量/函数 */
                                                      uint32_t Timeout)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart = HAL_GetTick();                   /* 32位无符号变量/函数 */

  /* Wait until flag is set */
  while ((FlagStatus)(__HAL_FMPSMBUS_GET_FLAG(hfmpsmbus, Flag)) == Status)  /* while循环 */
  {                                                     /* 代码块开始 */
    /* Check for the Timeout */
    if (Timeout != HAL_MAX_DELAY)                       /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (((HAL_GetTick() - tickstart) > Timeout) || (Timeout == 0UL))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        hfmpsmbus->PreviousState = hfmpsmbus->State;    /* 赋值或初始化 */
        hfmpsmbus->State = HAL_FMPSMBUS_STATE_READY;    /* 赋值或初始化 */

        /* Update FMPSMBUS error code */
        hfmpsmbus->ErrorCode |= HAL_FMPSMBUS_ERROR_HALTIMEOUT;  /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hfmpsmbus);                        /* HAL辅助宏/函数 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Handle FMPSMBUSx communication when starting transfer or during transfer (TC or TCR flag are set).
  * @param  hfmpsmbus FMPSMBUS handle.
  * @param  DevAddress specifies the slave address to be programmed.
  * @param  Size specifies the number of bytes to be programmed.
  *   This parameter must be a value between 0 and 255.
  * @param  Mode New state of the FMPSMBUS START condition generation.
  *   This parameter can be one or a combination  of the following values:
  *     @arg @ref FMPSMBUS_RELOAD_MODE Enable Reload mode.
  *     @arg @ref FMPSMBUS_AUTOEND_MODE Enable Automatic end mode.
  *     @arg @ref FMPSMBUS_SOFTEND_MODE Enable Software end mode and Reload mode.
  *     @arg @ref FMPSMBUS_SENDPEC_MODE Enable Packet Error Calculation mode.
  * @param  Request New state of the FMPSMBUS START condition generation.
  *   This parameter can be one of the following values:
  *     @arg @ref FMPSMBUS_NO_STARTSTOP Don't Generate stop and start condition.
  *     @arg @ref FMPSMBUS_GENERATE_STOP Generate stop condition (Size should be set to 0).
  *     @arg @ref FMPSMBUS_GENERATE_START_READ Generate Restart for read request.
  *     @arg @ref FMPSMBUS_GENERATE_START_WRITE Generate Restart for write request.
  * @retval None
  */
static void FMPSMBUS_TransferConfig(FMPSMBUS_HandleTypeDef *hfmpsmbus,  uint16_t DevAddress, uint8_t Size, uint32_t Mode,  /* 静态函数 */
                                 uint32_t Request)      /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_FMPSMBUS_ALL_INSTANCE(hfmpsmbus->Instance));  /* 执行语句 */
  assert_param(IS_FMPSMBUS_TRANSFER_MODE(Mode));        /* 执行语句 */
  assert_param(IS_FMPSMBUS_TRANSFER_REQUEST(Request));  /* 执行语句 */

  /* update CR2 register */
  MODIFY_REG(hfmpsmbus->Instance->CR2,                  /* 列表续行 */
             ((FMPI2C_CR2_SADD | FMPI2C_CR2_NBYTES | FMPI2C_CR2_RELOAD | FMPI2C_CR2_AUTOEND | \
               (FMPI2C_CR2_RD_WRN & (uint32_t)(Request >> (31UL - FMPI2C_CR2_RD_WRN_Pos))) | \
               FMPI2C_CR2_START | FMPI2C_CR2_STOP  | FMPI2C_CR2_PECBYTE)), \
             (uint32_t)(((uint32_t)DevAddress & FMPI2C_CR2_SADD) | \
                        (((uint32_t)Size << FMPI2C_CR2_NBYTES_Pos) & FMPI2C_CR2_NBYTES) | (uint32_t)Mode | (uint32_t)Request));  /* 执行语句 */
}                                                       /* 代码块结束 */

/**
  * @brief  Convert FMPSMBUSx OTHER_xxx XferOptions to functional XferOptions.
  * @param  hfmpsmbus FMPSMBUS handle.
  * @retval None
  */
static void FMPSMBUS_ConvertOtherXferOptions(FMPSMBUS_HandleTypeDef *hfmpsmbus)  /* 静态函数 */
{                                                       /* 代码块开始 */
  /* if user set XferOptions to FMPSMBUS_OTHER_FRAME_NO_PEC   */
  /* it request implicitly to generate a restart condition */
  /* set XferOptions to FMPSMBUS_FIRST_FRAME                  */
  if (hfmpsmbus->XferOptions == FMPSMBUS_OTHER_FRAME_NO_PEC)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hfmpsmbus->XferOptions = FMPSMBUS_FIRST_FRAME;      /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  /* else if user set XferOptions to FMPSMBUS_OTHER_FRAME_WITH_PEC */
  /* it request implicitly to generate a restart condition      */
  /* set XferOptions to FMPSMBUS_FIRST_FRAME | FMPSMBUS_SENDPEC_MODE  */
  else if (hfmpsmbus->XferOptions == FMPSMBUS_OTHER_FRAME_WITH_PEC)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    hfmpsmbus->XferOptions = FMPSMBUS_FIRST_FRAME | FMPSMBUS_SENDPEC_MODE;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  /* else if user set XferOptions to FMPSMBUS_OTHER_AND_LAST_FRAME_NO_PEC */
  /* it request implicitly to generate a restart condition             */
  /* then generate a stop condition at the end of transfer             */
  /* set XferOptions to FMPSMBUS_FIRST_AND_LAST_FRAME_NO_PEC              */
  else if (hfmpsmbus->XferOptions == FMPSMBUS_OTHER_AND_LAST_FRAME_NO_PEC)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    hfmpsmbus->XferOptions = FMPSMBUS_FIRST_AND_LAST_FRAME_NO_PEC;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  /* else if user set XferOptions to FMPSMBUS_OTHER_AND_LAST_FRAME_WITH_PEC */
  /* it request implicitly to generate a restart condition               */
  /* then generate a stop condition at the end of transfer               */
  /* set XferOptions to FMPSMBUS_FIRST_AND_LAST_FRAME_WITH_PEC              */
  else if (hfmpsmbus->XferOptions == FMPSMBUS_OTHER_AND_LAST_FRAME_WITH_PEC)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    hfmpsmbus->XferOptions = FMPSMBUS_FIRST_AND_LAST_FRAME_WITH_PEC;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Nothing to do */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */
/**
  * @}
  */

#endif /* FMPI2C_CR1_PE */
#endif /* HAL_FMPSMBUS_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
