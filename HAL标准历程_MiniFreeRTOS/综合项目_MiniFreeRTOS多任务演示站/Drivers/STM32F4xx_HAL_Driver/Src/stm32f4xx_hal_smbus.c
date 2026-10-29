/* 文件: stm32f4xx_hal_smbus.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_smbus.c
  * @author  MCD Application Team
  * @brief   SMBUS HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the System Management Bus (SMBus) peripheral,
  *          based on SMBUS principals of operation :
  *           + Initialization and de-initialization functions
  *           + IO operation functions
  *           + Peripheral State, Mode and Error functions
  *
  @verbatim
  ==============================================================================
                        ##### How to use this driver #####
  ==============================================================================
  [..]
    The SMBUS HAL driver can be used as follows:

    (#) Declare a SMBUS_HandleTypeDef handle structure, for example:
        SMBUS_HandleTypeDef  hsmbus;

    (#)Initialize the SMBUS low level resources by implementing the @ref HAL_SMBUS_MspInit() API:
        (##) Enable the SMBUSx interface clock
        (##) SMBUS pins configuration
            (+++) Enable the clock for the SMBUS GPIOs
            (+++) Configure SMBUS pins as alternate function open-drain
        (##) NVIC configuration if you need to use interrupt process
            (+++) Configure the SMBUSx interrupt priority
            (+++) Enable the NVIC SMBUS IRQ Channel

    (#) Configure the Communication Speed, Duty cycle, Addressing mode, Own Address1,
        Dual Addressing mode, Own Address2, General call and Nostretch mode in the hsmbus Init structure.

    (#) Initialize the SMBUS registers by calling the @ref HAL_SMBUS_Init(), configures also the low level Hardware
        (GPIO, CLOCK, NVIC...etc) by calling the customized @ref HAL_SMBUS_MspInit(&hsmbus) API.

    (#) To check if target device is ready for communication, use the function @ref HAL_SMBUS_IsDeviceReady()

    (#) For SMBUS IO operations, only one mode of operations is available within this driver :


    *** Interrupt mode IO operation ***
    ===================================

  [..]
      (+) Transmit in master/host SMBUS mode an amount of data in non blocking mode using @ref HAL_SMBUS_Master_Transmit_IT()
      (++) At transmission end of transfer @ref HAL_SMBUS_MasterTxCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_SMBUS_MasterTxCpltCallback()
      (+) Receive in master/host SMBUS mode an amount of data in non blocking mode using @ref HAL_SMBUS_Master_Receive_IT()
      (++) At reception end of transfer @ref HAL_SMBUS_MasterRxCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_SMBUS_MasterRxCpltCallback()
      (+) Abort a master/Host SMBUS process communication with Interrupt using @ref HAL_SMBUS_Master_Abort_IT()
      (++) End of abort process, @ref HAL_SMBUS_AbortCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_SMBUS_AbortCpltCallback()
      (+) Enable/disable the Address listen mode in slave/device or host/slave SMBUS mode
           using @ref HAL_SMBUS_EnableListen_IT() @ref HAL_SMBUS_DisableListen_IT()
      (++) When address slave/device SMBUS match, @ref HAL_SMBUS_AddrCallback() is executed and user can
           add his own code to check the Address Match Code and the transmission direction request by master/host (Write/Read).
      (++) At Listen mode end @ref HAL_SMBUS_ListenCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_SMBUS_ListenCpltCallback()
      (+) Transmit in slave/device SMBUS mode an amount of data in non blocking mode using @ref HAL_SMBUS_Slave_Transmit_IT()
      (++) At transmission end of transfer @ref HAL_SMBUS_SlaveTxCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_SMBUS_SlaveTxCpltCallback()
      (+) Receive in slave/device SMBUS mode an amount of data in non blocking mode using @ref HAL_SMBUS_Slave_Receive_IT()
      (++) At reception end of transfer @ref HAL_SMBUS_SlaveRxCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_SMBUS_SlaveRxCpltCallback()
      (+) Enable/Disable the SMBUS alert mode using @ref HAL_SMBUS_EnableAlert_IT() and @ref HAL_SMBUS_DisableAlert_IT()
      (++) When SMBUS Alert is generated @ref HAL_SMBUS_ErrorCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_SMBUS_ErrorCallback()
           to check the Alert Error Code using function @ref HAL_SMBUS_GetError()
      (+) Get HAL state machine or error values using @ref HAL_SMBUS_GetState() or HAL_SMBUS_GetError()
      (+) In case of transfer Error, @ref HAL_SMBUS_ErrorCallback() function is executed and user can
           add his own code by customization of function pointer @ref HAL_SMBUS_ErrorCallback()
           to check the Error Code using function @ref HAL_SMBUS_GetError()


     *** SMBUS HAL driver macros list ***
     ==================================
     [..]
       Below the list of most used macros in SMBUS HAL driver.

      (+) @ref __HAL_SMBUS_ENABLE    : Enable the SMBUS peripheral
      (+) @ref __HAL_SMBUS_DISABLE   : Disable the SMBUS peripheral
      (+) @ref __HAL_SMBUS_GET_FLAG  : Checks whether the specified SMBUS flag is set or not
      (+) @ref __HAL_SMBUS_CLEAR_FLAG: Clear the specified SMBUS pending flag
      (+) @ref __HAL_SMBUS_ENABLE_IT : Enable the specified SMBUS interrupt
      (+) @ref __HAL_SMBUS_DISABLE_IT: Disable the specified SMBUS interrupt

     [..]
       (@) You can refer to the SMBUS HAL driver header file for more useful macros

     *** Callback registration ***
     =============================================
    [..]
     The compilation flag USE_HAL_SMBUS_REGISTER_CALLBACKS when set to 1
     allows the user to configure dynamically the driver callbacks.
     Use Functions @ref HAL_SMBUS_RegisterCallback() or @ref HAL_SMBUS_RegisterXXXCallback()
     to register an interrupt callback.

     Function @ref HAL_SMBUS_RegisterCallback() allows to register following callbacks:
       (+) MasterTxCpltCallback : callback for Master transmission end of transfer.
       (+) MasterRxCpltCallback : callback for Master reception end of transfer.
       (+) SlaveTxCpltCallback  : callback for Slave transmission end of transfer.
       (+) SlaveRxCpltCallback  : callback for Slave reception end of transfer.
       (+) ListenCpltCallback   : callback for end of listen mode.
       (+) ErrorCallback        : callback for error detection.
       (+) AbortCpltCallback    : callback for abort completion process.
       (+) MspInitCallback      : callback for Msp Init.
       (+) MspDeInitCallback    : callback for Msp DeInit.
     This function takes as parameters the HAL peripheral handle, the Callback ID
     and a pointer to the user callback function.
    [..]
     For specific callback AddrCallback use dedicated register callbacks : @ref HAL_SMBUS_RegisterAddrCallback().
    [..]
     Use function @ref HAL_SMBUS_UnRegisterCallback to reset a callback to the default
     weak function.
     @ref HAL_SMBUS_UnRegisterCallback takes as parameters the HAL peripheral handle,
     and the Callback ID.
     This function allows to reset following callbacks:
       (+) MasterTxCpltCallback : callback for Master transmission end of transfer.
       (+) MasterRxCpltCallback : callback for Master reception end of transfer.
       (+) SlaveTxCpltCallback  : callback for Slave transmission end of transfer.
       (+) SlaveRxCpltCallback  : callback for Slave reception end of transfer.
       (+) ListenCpltCallback   : callback for end of listen mode.
       (+) ErrorCallback        : callback for error detection.
       (+) AbortCpltCallback    : callback for abort completion process.
       (+) MspInitCallback      : callback for Msp Init.
       (+) MspDeInitCallback    : callback for Msp DeInit.
    [..]
     For callback AddrCallback use dedicated register callbacks : @ref HAL_SMBUS_UnRegisterAddrCallback().
    [..]
     By default, after the @ref HAL_SMBUS_Init() and when the state is @ref HAL_SMBUS_STATE_RESET
     all callbacks are set to the corresponding weak functions:
     examples @ref HAL_SMBUS_MasterTxCpltCallback(), @ref HAL_SMBUS_MasterRxCpltCallback().
     Exception done for MspInit and MspDeInit functions that are
     reset to the legacy weak functions in the @ref HAL_SMBUS_Init()/ @ref HAL_SMBUS_DeInit() only when
     these callbacks are null (not registered beforehand).
     If MspInit or MspDeInit are not null, the @ref HAL_SMBUS_Init()/ @ref HAL_SMBUS_DeInit()
     keep and use the user MspInit/MspDeInit callbacks (registered beforehand) whatever the state.
    [..]
     Callbacks can be registered/unregistered in @ref HAL_SMBUS_STATE_READY state only.
     Exception done MspInit/MspDeInit functions that can be registered/unregistered
     in @ref HAL_SMBUS_STATE_READY or @ref HAL_SMBUS_STATE_RESET state,
     thus registered (user) MspInit/DeInit callbacks can be used during the Init/DeInit.
     Then, the user first registers the MspInit/MspDeInit user callbacks
     using @ref HAL_SMBUS_RegisterCallback() before calling @ref HAL_SMBUS_DeInit()
     or @ref HAL_SMBUS_Init() function.
    [..]
     When the compilation flag USE_HAL_SMBUS_REGISTER_CALLBACKS is set to 0 or
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

/** @defgroup SMBUS SMBUS
  * @brief SMBUS HAL module driver
  * @{
  */

#ifdef HAL_SMBUS_MODULE_ENABLED                         /* 条件编译 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @addtogroup SMBUS_Private_Define
  * @{
  */
#define SMBUS_TIMEOUT_FLAG          35U         /*!< Timeout 35 ms             */
#define SMBUS_TIMEOUT_BUSY_FLAG     25U         /*!< Timeout 25 ms             */
#define SMBUS_NO_OPTION_FRAME       0xFFFF0000U /*!< XferOptions default value */

#define SMBUS_SENDPEC_MODE          I2C_CR1_PEC         /* 定义宏 */
#define SMBUS_GET_PEC(__HANDLE__)             (((__HANDLE__)->Instance->SR2 & I2C_SR2_PEC) >> 8)  /* 定义宏 */

/* Private define for @ref PreviousState usage */
#define SMBUS_STATE_MSK             ((uint32_t)((HAL_SMBUS_STATE_BUSY_TX | HAL_SMBUS_STATE_BUSY_RX) & (~(uint32_t)HAL_SMBUS_STATE_READY))) /*!< Mask State define, keep only RX and TX bits            */
#define SMBUS_STATE_NONE            ((uint32_t)(HAL_SMBUS_MODE_NONE))                                                                      /*!< Default Value                                          */
#define SMBUS_STATE_MASTER_BUSY_TX  ((uint32_t)((HAL_SMBUS_STATE_BUSY_TX & SMBUS_STATE_MSK) | HAL_SMBUS_MODE_MASTER))                      /*!< Master Busy TX, combinaison of State LSB and Mode enum */
#define SMBUS_STATE_MASTER_BUSY_RX  ((uint32_t)((HAL_SMBUS_STATE_BUSY_RX & SMBUS_STATE_MSK) | HAL_SMBUS_MODE_MASTER))                      /*!< Master Busy RX, combinaison of State LSB and Mode enum */
#define SMBUS_STATE_SLAVE_BUSY_TX   ((uint32_t)((HAL_SMBUS_STATE_BUSY_TX & SMBUS_STATE_MSK) | HAL_SMBUS_MODE_SLAVE))                       /*!< Slave Busy TX, combinaison of State LSB and Mode enum  */
#define SMBUS_STATE_SLAVE_BUSY_RX   ((uint32_t)((HAL_SMBUS_STATE_BUSY_RX & SMBUS_STATE_MSK) | HAL_SMBUS_MODE_SLAVE))                       /*!< Slave Busy RX, combinaison of State LSB and Mode enum  */

/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/** @addtogroup SMBUS_Private_Functions
  * @{
  */

static HAL_StatusTypeDef SMBUS_WaitOnFlagUntilTimeout(SMBUS_HandleTypeDef *hsmbus, uint32_t Flag, FlagStatus Status, uint32_t Timeout, uint32_t Tickstart);  /* 静态变量/函数 */
static void SMBUS_ITError(SMBUS_HandleTypeDef *hsmbus);  /* 静态函数 */

/* Private functions for SMBUS transfer IRQ handler */
static HAL_StatusTypeDef SMBUS_MasterTransmit_TXE(SMBUS_HandleTypeDef *hsmbus);  /* 静态变量/函数 */
static HAL_StatusTypeDef SMBUS_MasterTransmit_BTF(SMBUS_HandleTypeDef *hsmbus);  /* 静态变量/函数 */
static HAL_StatusTypeDef SMBUS_MasterReceive_RXNE(SMBUS_HandleTypeDef *hsmbus);  /* 静态变量/函数 */
static HAL_StatusTypeDef SMBUS_MasterReceive_BTF(SMBUS_HandleTypeDef *hsmbus);  /* 静态变量/函数 */
static HAL_StatusTypeDef SMBUS_Master_SB(SMBUS_HandleTypeDef *hsmbus);  /* 静态变量/函数 */
static HAL_StatusTypeDef SMBUS_Master_ADD10(SMBUS_HandleTypeDef *hsmbus);  /* 静态变量/函数 */
static HAL_StatusTypeDef SMBUS_Master_ADDR(SMBUS_HandleTypeDef *hsmbus);  /* 静态变量/函数 */

static HAL_StatusTypeDef SMBUS_SlaveTransmit_TXE(SMBUS_HandleTypeDef *hsmbus);  /* 静态变量/函数 */
static HAL_StatusTypeDef SMBUS_SlaveTransmit_BTF(SMBUS_HandleTypeDef *hsmbus);  /* 静态变量/函数 */
static HAL_StatusTypeDef SMBUS_SlaveReceive_RXNE(SMBUS_HandleTypeDef *hsmbus);  /* 静态变量/函数 */
static HAL_StatusTypeDef SMBUS_SlaveReceive_BTF(SMBUS_HandleTypeDef *hsmbus);  /* 静态变量/函数 */
static HAL_StatusTypeDef SMBUS_Slave_ADDR(SMBUS_HandleTypeDef *hsmbus);  /* 静态变量/函数 */
static HAL_StatusTypeDef SMBUS_Slave_STOPF(SMBUS_HandleTypeDef *hsmbus);  /* 静态变量/函数 */
static HAL_StatusTypeDef SMBUS_Slave_AF(SMBUS_HandleTypeDef *hsmbus);  /* 静态变量/函数 */
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup SMBUS_Exported_Functions SMBUS Exported Functions
  * @{
  */

/** @defgroup SMBUS_Exported_Functions_Group1 Initialization and de-initialization functions
 *  @brief    Initialization and Configuration functions
 *
@verbatim
 ===============================================================================
              ##### Initialization and de-initialization functions #####
 ===============================================================================
    [..]  This subsection provides a set of functions allowing to initialize and
          deinitialize the SMBUSx peripheral:

      (+) User must Implement HAL_SMBUS_MspInit() function in which he configures
          all related peripherals resources (CLOCK, GPIO, IT and NVIC).

      (+) Call the function HAL_SMBUS_Init() to configure the selected device with
          the selected configuration:
        (++) Communication Speed
        (++) Addressing mode
        (++) Own Address 1
        (++) Dual Addressing mode
        (++) Own Address 2
        (++) General call mode
        (++) Nostretch mode
        (++) Packet Error Check mode
        (++) Peripheral mode

      (+) Call the function HAL_SMBUS_DeInit() to restore the default configuration
          of the selected SMBUSx peripheral.

@endverbatim
  * @{
  */

/**
  * @brief  Initializes the SMBUS according to the specified parameters
  *         in the SMBUS_InitTypeDef and initialize the associated handle.
  * @param  hsmbus pointer to a SMBUS_HandleTypeDef structure that contains
  *         the configuration information for the specified SMBUS
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SMBUS_Init(SMBUS_HandleTypeDef *hsmbus)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t freqrange = 0U;                              /* 32位无符号变量/函数 */
  uint32_t pclk1 = 0U;                                  /* 32位无符号变量/函数 */

  /* Check the SMBUS handle allocation */
  if (hsmbus == NULL)                                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the parameters */
  assert_param(IS_SMBUS_ALL_INSTANCE(hsmbus->Instance));  /* 执行语句 */
#if  defined(I2C_FLTR_ANOFF)                            /* 条件编译 */
  assert_param(IS_SMBUS_ANALOG_FILTER(hsmbus->Init.AnalogFilter));  /* 执行语句 */
#endif                                                  /* 条件编译结束 */
  assert_param(IS_SMBUS_CLOCK_SPEED(hsmbus->Init.ClockSpeed));  /* 执行语句 */
  assert_param(IS_SMBUS_OWN_ADDRESS1(hsmbus->Init.OwnAddress1));  /* 执行语句 */
  assert_param(IS_SMBUS_ADDRESSING_MODE(hsmbus->Init.AddressingMode));  /* 执行语句 */
  assert_param(IS_SMBUS_DUAL_ADDRESS(hsmbus->Init.DualAddressMode));  /* 执行语句 */
  assert_param(IS_SMBUS_OWN_ADDRESS2(hsmbus->Init.OwnAddress2));  /* 执行语句 */
  assert_param(IS_SMBUS_GENERAL_CALL(hsmbus->Init.GeneralCallMode));  /* 执行语句 */
  assert_param(IS_SMBUS_NO_STRETCH(hsmbus->Init.NoStretchMode));  /* 执行语句 */
  assert_param(IS_SMBUS_PEC(hsmbus->Init.PacketErrorCheckMode));  /* 执行语句 */
  assert_param(IS_SMBUS_PERIPHERAL_MODE(hsmbus->Init.PeripheralMode));  /* 执行语句 */

  if (hsmbus->State == HAL_SMBUS_STATE_RESET)           /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Allocate lock resource and initialize it */
    hsmbus->Lock = HAL_UNLOCKED;                        /* 赋值或初始化 */
    /* Init the low level hardware : GPIO, CLOCK, NVIC */
#if (USE_HAL_SMBUS_REGISTER_CALLBACKS == 1)             /* 条件编译 */
    /* Init the SMBUS Callback settings */
    hsmbus->MasterTxCpltCallback = HAL_SMBUS_MasterTxCpltCallback; /* Legacy weak MasterTxCpltCallback */
    hsmbus->MasterRxCpltCallback = HAL_SMBUS_MasterRxCpltCallback; /* Legacy weak MasterRxCpltCallback */
    hsmbus->SlaveTxCpltCallback  = HAL_SMBUS_SlaveTxCpltCallback;  /* Legacy weak SlaveTxCpltCallback  */
    hsmbus->SlaveRxCpltCallback  = HAL_SMBUS_SlaveRxCpltCallback;  /* Legacy weak SlaveRxCpltCallback  */
    hsmbus->ListenCpltCallback   = HAL_SMBUS_ListenCpltCallback;   /* Legacy weak ListenCpltCallback   */
    hsmbus->ErrorCallback        = HAL_SMBUS_ErrorCallback;        /* Legacy weak ErrorCallback        */
    hsmbus->AbortCpltCallback    = HAL_SMBUS_AbortCpltCallback;    /* Legacy weak AbortCpltCallback    */
    hsmbus->AddrCallback         = HAL_SMBUS_AddrCallback;         /* Legacy weak AddrCallback         */

    if (hsmbus->MspInitCallback == NULL)                /* 条件判断 */
    {                                                   /* 代码块开始 */
      hsmbus->MspInitCallback = HAL_SMBUS_MspInit; /* Legacy weak MspInit  */
    }                                                   /* 代码块结束 */

    /* Init the low level hardware : GPIO, CLOCK, NVIC */
    hsmbus->MspInitCallback(hsmbus);                    /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /* Init the low level hardware : GPIO, CLOCK, NVIC */
    HAL_SMBUS_MspInit(hsmbus);                          /* HAL库函数调用 */
#endif /* USE_HAL_SMBUS_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */

  hsmbus->State = HAL_SMBUS_STATE_BUSY;                 /* 赋值或初始化 */

  /* Disable the selected SMBUS peripheral */
  __HAL_SMBUS_DISABLE(hsmbus);                          /* HAL辅助宏/函数 */

  /* Get PCLK1 frequency */
  pclk1 = HAL_RCC_GetPCLK1Freq();                       /* 赋值或初始化 */

  /* Calculate frequency range */
  freqrange = SMBUS_FREQRANGE(pclk1);                   /* 赋值或初始化 */

  /*---------------------------- SMBUSx CR2 Configuration ----------------------*/
  /* Configure SMBUSx: Frequency range */
  MODIFY_REG(hsmbus->Instance->CR2, I2C_CR2_FREQ, freqrange);  /* 执行语句 */

  /*---------------------------- SMBUSx TRISE Configuration --------------------*/
  /* Configure SMBUSx: Rise Time */
  MODIFY_REG(hsmbus->Instance->TRISE, I2C_TRISE_TRISE, SMBUS_RISE_TIME(freqrange));  /* 执行语句 */

  /*---------------------------- SMBUSx CCR Configuration ----------------------*/
  /* Configure SMBUSx: Speed */
  MODIFY_REG(hsmbus->Instance->CCR, (I2C_CCR_FS | I2C_CCR_DUTY | I2C_CCR_CCR), SMBUS_SPEED_STANDARD(pclk1, hsmbus->Init.ClockSpeed));  /* 执行语句 */

  /*---------------------------- SMBUSx CR1 Configuration ----------------------*/
  /* Configure SMBUSx: Generalcall , PEC , Peripheral mode and  NoStretch mode */
  MODIFY_REG(hsmbus->Instance->CR1, (I2C_CR1_NOSTRETCH | I2C_CR1_ENGC | I2C_CR1_PEC | I2C_CR1_ENARP | I2C_CR1_SMBTYPE | I2C_CR1_SMBUS), (hsmbus->Init.NoStretchMode | hsmbus->Init.GeneralCallMode |  hsmbus->Init.PacketErrorCheckMode | hsmbus->Init.PeripheralMode));  /* 执行语句 */

  /*---------------------------- SMBUSx OAR1 Configuration ---------------------*/
  /* Configure SMBUSx: Own Address1 and addressing mode */
  MODIFY_REG(hsmbus->Instance->OAR1, (I2C_OAR1_ADDMODE | I2C_OAR1_ADD8_9 | I2C_OAR1_ADD1_7 | I2C_OAR1_ADD0), (hsmbus->Init.AddressingMode | hsmbus->Init.OwnAddress1));  /* 执行语句 */

  /*---------------------------- SMBUSx OAR2 Configuration ---------------------*/
  /* Configure SMBUSx: Dual mode and Own Address2 */
  MODIFY_REG(hsmbus->Instance->OAR2, (I2C_OAR2_ENDUAL | I2C_OAR2_ADD2), (hsmbus->Init.DualAddressMode | hsmbus->Init.OwnAddress2));  /* 执行语句 */
#if  defined(I2C_FLTR_ANOFF)                            /* 条件编译 */
  /*---------------------------- SMBUSx FLTR Configuration ------------------------*/
  /* Configure SMBUSx: Analog noise filter */
  SET_BIT(hsmbus->Instance->FLTR, hsmbus->Init.AnalogFilter);  /* 执行语句 */
#endif                                                  /* 条件编译结束 */

  /* Enable the selected SMBUS peripheral */
  __HAL_SMBUS_ENABLE(hsmbus);                           /* HAL辅助宏/函数 */

  hsmbus->ErrorCode = HAL_SMBUS_ERROR_NONE;             /* 赋值或初始化 */
  hsmbus->State = HAL_SMBUS_STATE_READY;                /* 赋值或初始化 */
  hsmbus->PreviousState = SMBUS_STATE_NONE;             /* 赋值或初始化 */
  hsmbus->Mode = HAL_SMBUS_MODE_NONE;                   /* 赋值或初始化 */
  hsmbus->XferPEC = 0x00;                               /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  DeInitializes the SMBUS peripheral.
  * @param  hsmbus pointer to a SMBUS_HandleTypeDef structure that contains
  *         the configuration information for the specified SMBUS.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SMBUS_DeInit(SMBUS_HandleTypeDef *hsmbus)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the SMBUS handle allocation */
  if (hsmbus == NULL)                                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the parameters */
  assert_param(IS_SMBUS_ALL_INSTANCE(hsmbus->Instance));  /* 执行语句 */

  hsmbus->State = HAL_SMBUS_STATE_BUSY;                 /* 赋值或初始化 */

  /* Disable the SMBUS Peripheral Clock */
  __HAL_SMBUS_DISABLE(hsmbus);                          /* HAL辅助宏/函数 */

#if (USE_HAL_SMBUS_REGISTER_CALLBACKS == 1)             /* 条件编译 */
  if (hsmbus->MspDeInitCallback == NULL)                /* 条件判断 */
  {                                                     /* 代码块开始 */
    hsmbus->MspDeInitCallback = HAL_SMBUS_MspDeInit; /* Legacy weak MspDeInit  */
  }                                                     /* 代码块结束 */

  /* DeInit the low level hardware: GPIO, CLOCK, NVIC */
  hsmbus->MspDeInitCallback(hsmbus);                    /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* DeInit the low level hardware: GPIO, CLOCK, NVIC */
  HAL_SMBUS_MspDeInit(hsmbus);                          /* HAL库函数调用 */
#endif /* USE_HAL_SMBUS_REGISTER_CALLBACKS */

  hsmbus->ErrorCode     = HAL_SMBUS_ERROR_NONE;         /* 赋值或初始化 */
  hsmbus->State         = HAL_SMBUS_STATE_RESET;        /* 赋值或初始化 */
  hsmbus->PreviousState = SMBUS_STATE_NONE;             /* 赋值或初始化 */
  hsmbus->Mode          = HAL_SMBUS_MODE_NONE;          /* 赋值或初始化 */

  /* Release Lock */
  __HAL_UNLOCK(hsmbus);                                 /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initialize the SMBUS MSP.
  * @param  hsmbus pointer to a SMBUS_HandleTypeDef structure that contains
  *         the configuration information for the specified SMBUS
  * @retval None
  */
__weak void HAL_SMBUS_MspInit(SMBUS_HandleTypeDef *hsmbus)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsmbus);                                       /* 执行语句 */
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_SMBUS_MspInit could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  DeInitialize the SMBUS MSP.
  * @param  hsmbus pointer to a SMBUS_HandleTypeDef structure that contains
  *         the configuration information for the specified SMBUS
  * @retval None
  */
__weak void HAL_SMBUS_MspDeInit(SMBUS_HandleTypeDef *hsmbus)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsmbus);                                       /* 执行语句 */
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_SMBUS_MspDeInit could be implemented in the user file
   */
}                                                       /* 代码块结束 */

#if  defined(I2C_FLTR_ANOFF)&&defined(I2C_FLTR_DNF)     /* 条件编译 */
/**
  * @brief  Configures SMBUS Analog noise filter.
  * @param  hsmbus pointer to a SMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified SMBUSx peripheral.
  * @param  AnalogFilter new state of the Analog filter.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SMBUS_ConfigAnalogFilter(SMBUS_HandleTypeDef *hsmbus, uint32_t AnalogFilter)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_SMBUS_ALL_INSTANCE(hsmbus->Instance));  /* 执行语句 */
  assert_param(IS_SMBUS_ANALOG_FILTER(AnalogFilter));   /* 执行语句 */

  if (hsmbus->State == HAL_SMBUS_STATE_READY)           /* 条件判断 */
  {                                                     /* 代码块开始 */
    hsmbus->State = HAL_SMBUS_STATE_BUSY;               /* 赋值或初始化 */

    /* Disable the selected SMBUS peripheral */
    __HAL_SMBUS_DISABLE(hsmbus);                        /* HAL辅助宏/函数 */

    /* Reset SMBUSx ANOFF bit */
    hsmbus->Instance->FLTR &= ~(I2C_FLTR_ANOFF);        /* 赋值或初始化 */

    /* Disable the analog filter */
    hsmbus->Instance->FLTR |= AnalogFilter;             /* 赋值或初始化 */

    __HAL_SMBUS_ENABLE(hsmbus);                         /* HAL辅助宏/函数 */

    hsmbus->State = HAL_SMBUS_STATE_READY;              /* 赋值或初始化 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configures SMBUS Digital noise filter.
  * @param  hsmbus pointer to a SMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified SMBUSx peripheral.
  * @param  DigitalFilter Coefficient of digital noise filter between 0x00 and 0x0F.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SMBUS_ConfigDigitalFilter(SMBUS_HandleTypeDef *hsmbus, uint32_t DigitalFilter)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint16_t tmpreg = 0;                                  /* 16位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_SMBUS_ALL_INSTANCE(hsmbus->Instance));  /* 执行语句 */
  assert_param(IS_SMBUS_DIGITAL_FILTER(DigitalFilter));  /* 执行语句 */

  if (hsmbus->State == HAL_SMBUS_STATE_READY)           /* 条件判断 */
  {                                                     /* 代码块开始 */
    hsmbus->State = HAL_SMBUS_STATE_BUSY;               /* 赋值或初始化 */

    /* Disable the selected SMBUS peripheral */
    __HAL_SMBUS_DISABLE(hsmbus);                        /* HAL辅助宏/函数 */

    /* Get the old register value */
    tmpreg = hsmbus->Instance->FLTR;                    /* 赋值或初始化 */

    /* Reset SMBUSx DNF bit [3:0] */
    tmpreg &= ~(I2C_FLTR_DNF);                          /* 赋值或初始化 */

    /* Set SMBUSx DNF coefficient */
    tmpreg |= DigitalFilter;                            /* 赋值或初始化 */

    /* Store the new register value */
    hsmbus->Instance->FLTR = tmpreg;                    /* 赋值或初始化 */

    __HAL_SMBUS_ENABLE(hsmbus);                         /* HAL辅助宏/函数 */

    hsmbus->State = HAL_SMBUS_STATE_READY;              /* 赋值或初始化 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */
#endif                                                  /* 条件编译结束 */
#if (USE_HAL_SMBUS_REGISTER_CALLBACKS == 1)             /* 条件编译 */
/**
  * @brief  Register a User SMBUS Callback
  *         To be used instead of the weak predefined callback
  * @param  hsmbus Pointer to a SMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified SMBUS.
  * @param  CallbackID ID of the callback to be registered
  *         This parameter can be one of the following values:
  *          @arg @ref HAL_SMBUS_MASTER_TX_COMPLETE_CB_ID Master Tx Transfer completed callback ID
  *          @arg @ref HAL_SMBUS_MASTER_RX_COMPLETE_CB_ID Master Rx Transfer completed callback ID
  *          @arg @ref HAL_SMBUS_SLAVE_TX_COMPLETE_CB_ID Slave Tx Transfer completed callback ID
  *          @arg @ref HAL_SMBUS_SLAVE_RX_COMPLETE_CB_ID Slave Rx Transfer completed callback ID
  *          @arg @ref HAL_SMBUS_LISTEN_COMPLETE_CB_ID Listen Complete callback ID
  *          @arg @ref HAL_SMBUS_ERROR_CB_ID Error callback ID
  *          @arg @ref HAL_SMBUS_ABORT_CB_ID Abort callback ID
  *          @arg @ref HAL_SMBUS_MSPINIT_CB_ID MspInit callback ID
  *          @arg @ref HAL_SMBUS_MSPDEINIT_CB_ID MspDeInit callback ID
  * @param  pCallback pointer to the Callback function
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SMBUS_RegisterCallback(SMBUS_HandleTypeDef *hsmbus, HAL_SMBUS_CallbackIDTypeDef CallbackID, pSMBUS_CallbackTypeDef pCallback)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  if (pCallback == NULL)                                /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hsmbus->ErrorCode |= HAL_SMBUS_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  /* Process locked */
  __HAL_LOCK(hsmbus);                                   /* HAL辅助宏/函数 */

  if (HAL_SMBUS_STATE_READY == hsmbus->State)           /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_SMBUS_MASTER_TX_COMPLETE_CB_ID :         /* case标签 */
        hsmbus->MasterTxCpltCallback = pCallback;       /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_SMBUS_MASTER_RX_COMPLETE_CB_ID :         /* case标签 */
        hsmbus->MasterRxCpltCallback = pCallback;       /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_SMBUS_SLAVE_TX_COMPLETE_CB_ID :          /* case标签 */
        hsmbus->SlaveTxCpltCallback = pCallback;        /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_SMBUS_SLAVE_RX_COMPLETE_CB_ID :          /* case标签 */
        hsmbus->SlaveRxCpltCallback = pCallback;        /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_SMBUS_LISTEN_COMPLETE_CB_ID :            /* case标签 */
        hsmbus->ListenCpltCallback = pCallback;         /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_SMBUS_ERROR_CB_ID :                      /* case标签 */
        hsmbus->ErrorCallback = pCallback;              /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_SMBUS_ABORT_CB_ID :                      /* case标签 */
        hsmbus->AbortCpltCallback = pCallback;          /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_SMBUS_MSPINIT_CB_ID :                    /* case标签 */
        hsmbus->MspInitCallback = pCallback;            /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_SMBUS_MSPDEINIT_CB_ID :                  /* case标签 */
        hsmbus->MspDeInitCallback = pCallback;          /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hsmbus->ErrorCode |= HAL_SMBUS_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (HAL_SMBUS_STATE_RESET == hsmbus->State)      /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_SMBUS_MSPINIT_CB_ID :                    /* case标签 */
        hsmbus->MspInitCallback = pCallback;            /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_SMBUS_MSPDEINIT_CB_ID :                  /* case标签 */
        hsmbus->MspDeInitCallback = pCallback;          /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hsmbus->ErrorCode |= HAL_SMBUS_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hsmbus->ErrorCode |= HAL_SMBUS_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hsmbus);                                 /* HAL辅助宏/函数 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Unregister an SMBUS Callback
  *         SMBUS callback is redirected to the weak predefined callback
  * @param  hsmbus Pointer to a SMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified SMBUS.
  * @param  CallbackID ID of the callback to be unregistered
  *         This parameter can be one of the following values:
  *         This parameter can be one of the following values:
  *          @arg @ref HAL_SMBUS_MASTER_TX_COMPLETE_CB_ID Master Tx Transfer completed callback ID
  *          @arg @ref HAL_SMBUS_MASTER_RX_COMPLETE_CB_ID Master Rx Transfer completed callback ID
  *          @arg @ref HAL_SMBUS_SLAVE_TX_COMPLETE_CB_ID Slave Tx Transfer completed callback ID
  *          @arg @ref HAL_SMBUS_SLAVE_RX_COMPLETE_CB_ID Slave Rx Transfer completed callback ID
  *          @arg @ref HAL_SMBUS_LISTEN_COMPLETE_CB_ID Listen Complete callback ID
  *          @arg @ref HAL_SMBUS_ERROR_CB_ID Error callback ID
  *          @arg @ref HAL_SMBUS_ABORT_CB_ID Abort callback ID
  *          @arg @ref HAL_SMBUS_MSPINIT_CB_ID MspInit callback ID
  *          @arg @ref HAL_SMBUS_MSPDEINIT_CB_ID MspDeInit callback ID
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SMBUS_UnRegisterCallback(SMBUS_HandleTypeDef *hsmbus, HAL_SMBUS_CallbackIDTypeDef CallbackID)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Process locked */
  __HAL_LOCK(hsmbus);                                   /* HAL辅助宏/函数 */

  if (HAL_SMBUS_STATE_READY == hsmbus->State)           /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_SMBUS_MASTER_TX_COMPLETE_CB_ID :         /* case标签 */
        hsmbus->MasterTxCpltCallback = HAL_SMBUS_MasterTxCpltCallback; /* Legacy weak MasterTxCpltCallback */
        break;                                          /* 跳出 */

      case HAL_SMBUS_MASTER_RX_COMPLETE_CB_ID :         /* case标签 */
        hsmbus->MasterRxCpltCallback = HAL_SMBUS_MasterRxCpltCallback; /* Legacy weak MasterRxCpltCallback */
        break;                                          /* 跳出 */

      case HAL_SMBUS_SLAVE_TX_COMPLETE_CB_ID :          /* case标签 */
        hsmbus->SlaveTxCpltCallback = HAL_SMBUS_SlaveTxCpltCallback;   /* Legacy weak SlaveTxCpltCallback  */
        break;                                          /* 跳出 */

      case HAL_SMBUS_SLAVE_RX_COMPLETE_CB_ID :          /* case标签 */
        hsmbus->SlaveRxCpltCallback = HAL_SMBUS_SlaveRxCpltCallback;   /* Legacy weak SlaveRxCpltCallback  */
        break;                                          /* 跳出 */

      case HAL_SMBUS_LISTEN_COMPLETE_CB_ID :            /* case标签 */
        hsmbus->ListenCpltCallback = HAL_SMBUS_ListenCpltCallback;     /* Legacy weak ListenCpltCallback   */
        break;                                          /* 跳出 */

      case HAL_SMBUS_ERROR_CB_ID :                      /* case标签 */
        hsmbus->ErrorCallback = HAL_SMBUS_ErrorCallback;               /* Legacy weak ErrorCallback        */
        break;                                          /* 跳出 */

      case HAL_SMBUS_ABORT_CB_ID :                      /* case标签 */
        hsmbus->AbortCpltCallback = HAL_SMBUS_AbortCpltCallback;       /* Legacy weak AbortCpltCallback    */
        break;                                          /* 跳出 */

      case HAL_SMBUS_MSPINIT_CB_ID :                    /* case标签 */
        hsmbus->MspInitCallback = HAL_SMBUS_MspInit;                   /* Legacy weak MspInit              */
        break;                                          /* 跳出 */

      case HAL_SMBUS_MSPDEINIT_CB_ID :                  /* case标签 */
        hsmbus->MspDeInitCallback = HAL_SMBUS_MspDeInit;               /* Legacy weak MspDeInit            */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hsmbus->ErrorCode |= HAL_SMBUS_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (HAL_SMBUS_STATE_RESET == hsmbus->State)      /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_SMBUS_MSPINIT_CB_ID :                    /* case标签 */
        hsmbus->MspInitCallback = HAL_SMBUS_MspInit;                   /* Legacy weak MspInit              */
        break;                                          /* 跳出 */

      case HAL_SMBUS_MSPDEINIT_CB_ID :                  /* case标签 */
        hsmbus->MspDeInitCallback = HAL_SMBUS_MspDeInit;               /* Legacy weak MspDeInit            */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hsmbus->ErrorCode |= HAL_SMBUS_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hsmbus->ErrorCode |= HAL_SMBUS_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hsmbus);                                 /* HAL辅助宏/函数 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Register the Slave Address Match SMBUS Callback
  *         To be used instead of the weak HAL_SMBUS_AddrCallback() predefined callback
  * @param  hsmbus Pointer to a SMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified SMBUS.
  * @param  pCallback pointer to the Address Match Callback function
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SMBUS_RegisterAddrCallback(SMBUS_HandleTypeDef *hsmbus, pSMBUS_AddrCallbackTypeDef pCallback)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  if (pCallback == NULL)                                /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  /* Process locked */
  __HAL_LOCK(hsmbus);                                   /* HAL辅助宏/函数 */

  if (HAL_SMBUS_STATE_READY == hsmbus->State)           /* 条件判断 */
  {                                                     /* 代码块开始 */
    hsmbus->AddrCallback = pCallback;                   /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hsmbus->ErrorCode |= HAL_SMBUS_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hsmbus);                                 /* HAL辅助宏/函数 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  UnRegister the Slave Address Match SMBUS Callback
  *         Info Ready SMBUS Callback is redirected to the weak HAL_SMBUS_AddrCallback() predefined callback
  * @param  hsmbus Pointer to a SMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified SMBUS.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SMBUS_UnRegisterAddrCallback(SMBUS_HandleTypeDef *hsmbus)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Process locked */
  __HAL_LOCK(hsmbus);                                   /* HAL辅助宏/函数 */

  if (HAL_SMBUS_STATE_READY == hsmbus->State)           /* 条件判断 */
  {                                                     /* 代码块开始 */
    hsmbus->AddrCallback = HAL_SMBUS_AddrCallback; /* Legacy weak AddrCallback  */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hsmbus->ErrorCode |= HAL_SMBUS_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hsmbus);                                 /* HAL辅助宏/函数 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

#endif /* USE_HAL_SMBUS_REGISTER_CALLBACKS */

/**
  * @}
  */

/** @defgroup SMBUS_Exported_Functions_Group2 Input and Output operation functions
 *  @brief    Data transfers functions
 *
@verbatim
 ===============================================================================
                      ##### IO operation functions #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to manage the SMBUS data
    transfers.

    (#) Blocking mode function to check if device is ready for usage is :
        (++) HAL_SMBUS_IsDeviceReady()

    (#) There is only one mode of transfer:
       (++) Non Blocking mode : The communication is performed using Interrupts.
            These functions return the status of the transfer startup.
            The end of the data processing will be indicated through the
            dedicated SMBUS IRQ when using Interrupt mode.

    (#) Non Blocking mode functions with Interrupt are :
        (++) HAL_SMBUS_Master_Transmit_IT()
        (++) HAL_SMBUS_Master_Receive_IT()
        (++) HAL_SMBUS_Master_Abort_IT()
        (++) HAL_SMBUS_Slave_Transmit_IT()
        (++) HAL_SMBUS_Slave_Receive_IT()
        (++) HAL_SMBUS_EnableAlert_IT()
        (++) HAL_SMBUS_DisableAlert_IT()

    (#) A set of Transfer Complete Callbacks are provided in No_Blocking mode:
        (++) HAL_SMBUS_MasterTxCpltCallback()
        (++) HAL_SMBUS_MasterRxCpltCallback()
        (++) HAL_SMBUS_SlaveTxCpltCallback()
        (++) HAL_SMBUS_SlaveRxCpltCallback()
        (++) HAL_SMBUS_AddrCallback()
        (++) HAL_SMBUS_ListenCpltCallback()
        (++) HAL_SMBUS_ErrorCallback()
        (++) HAL_SMBUS_AbortCpltCallback()

@endverbatim
  * @{
  */

/**
  * @brief  Transmits in master mode an amount of data in blocking mode.
  * @param  hsmbus Pointer to a SMBUS_HandleTypeDef structure that contains
  *         the configuration information for the specified SMBUS.
  * @param  DevAddress Target device address The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  XferOptions Options of Transfer
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SMBUS_Master_Transmit_IT(SMBUS_HandleTypeDef *hsmbus, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t XferOptions)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t count      = 0x00U;                          /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_SMBUS_TRANSFER_OPTIONS_REQUEST(XferOptions));  /* 执行语句 */

  if (hsmbus->State == HAL_SMBUS_STATE_READY)           /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check Busy Flag only if FIRST call of Master interface */
    if ((XferOptions == SMBUS_FIRST_AND_LAST_FRAME_NO_PEC) || (XferOptions == SMBUS_FIRST_AND_LAST_FRAME_WITH_PEC) || (XferOptions == SMBUS_FIRST_FRAME))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Wait until BUSY flag is reset */
      count = SMBUS_TIMEOUT_BUSY_FLAG * (SystemCoreClock / 25U / 1000U);  /* 赋值或初始化 */
      do                                                /* do-while循环 */
      {                                                 /* 代码块开始 */
        if (count-- == 0U)                              /* 条件判断 */
        {                                               /* 代码块开始 */
          hsmbus->PreviousState = SMBUS_STATE_NONE;     /* 赋值或初始化 */
          hsmbus->State = HAL_SMBUS_STATE_READY;        /* 赋值或初始化 */

          /* Process Unlocked */
          __HAL_UNLOCK(hsmbus);                         /* HAL辅助宏/函数 */

          return HAL_TIMEOUT;                           /* 返回值 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      while (__HAL_SMBUS_GET_FLAG(hsmbus, SMBUS_FLAG_BUSY) != RESET);  /* while循环 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hsmbus);                                 /* HAL辅助宏/函数 */

    /* Check if the SMBUS is already enabled */
    if ((hsmbus->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable SMBUS peripheral */
      __HAL_SMBUS_ENABLE(hsmbus);                       /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Disable Pos */
    CLEAR_BIT(hsmbus->Instance->CR1, I2C_CR1_POS);      /* 执行语句 */

    hsmbus->State     = HAL_SMBUS_STATE_BUSY_TX;        /* 赋值或初始化 */
    hsmbus->ErrorCode = HAL_SMBUS_ERROR_NONE;           /* 赋值或初始化 */
    hsmbus->Mode      = HAL_SMBUS_MODE_MASTER;          /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hsmbus->pBuffPtr    = pData;                        /* 赋值或初始化 */
    hsmbus->XferCount   = Size;                         /* 赋值或初始化 */
    hsmbus->XferOptions = XferOptions;                  /* 赋值或初始化 */
    hsmbus->XferSize    = hsmbus->XferCount;            /* 赋值或初始化 */
    hsmbus->Devaddress  = DevAddress;                   /* 赋值或初始化 */

    /* Generate Start */
    SET_BIT(hsmbus->Instance->CR1, I2C_CR1_START);      /* 执行语句 */

    /* Process Unlocked */
    __HAL_UNLOCK(hsmbus);                               /* HAL辅助宏/函数 */

    /* Note : The SMBUS interrupts must be enabled after unlocking current process
    to avoid the risk of hsmbus interrupt handle execution before current
    process unlock */

    /* Enable EVT, BUF and ERR interrupt */
    __HAL_SMBUS_ENABLE_IT(hsmbus, SMBUS_IT_EVT | SMBUS_IT_BUF | SMBUS_IT_ERR);  /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */
/**
  * @brief  Receive in master/host SMBUS mode an amount of data in non blocking mode with Interrupt.
  * @param  hsmbus Pointer to a SMBUS_HandleTypeDef structure that contains
  *         the configuration information for the specified SMBUS.
  * @param  DevAddress Target device address The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  XferOptions Options of Transfer, value of @ref SMBUS_XferOptions_definition
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SMBUS_Master_Receive_IT(SMBUS_HandleTypeDef *hsmbus, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t XferOptions)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  __IO uint32_t count = 0U;                             /* 赋值或初始化 */

  /* Check the parameters */
  assert_param(IS_SMBUS_TRANSFER_OPTIONS_REQUEST(XferOptions));  /* 执行语句 */

  if (hsmbus->State == HAL_SMBUS_STATE_READY)           /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check Busy Flag only if FIRST call of Master interface */
    if ((XferOptions == SMBUS_FIRST_AND_LAST_FRAME_NO_PEC) || (XferOptions == SMBUS_FIRST_AND_LAST_FRAME_WITH_PEC) || (XferOptions == SMBUS_FIRST_FRAME))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Wait until BUSY flag is reset */
      count = SMBUS_TIMEOUT_BUSY_FLAG * (SystemCoreClock / 25U / 1000U);  /* 赋值或初始化 */
      do                                                /* do-while循环 */
      {                                                 /* 代码块开始 */
        if (count-- == 0U)                              /* 条件判断 */
        {                                               /* 代码块开始 */
          hsmbus->PreviousState = SMBUS_STATE_NONE;     /* 赋值或初始化 */
          hsmbus->State = HAL_SMBUS_STATE_READY;        /* 赋值或初始化 */

          /* Process Unlocked */
          __HAL_UNLOCK(hsmbus);                         /* HAL辅助宏/函数 */

          return HAL_TIMEOUT;                           /* 返回值 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      while (__HAL_SMBUS_GET_FLAG(hsmbus, SMBUS_FLAG_BUSY) != RESET);  /* while循环 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hsmbus);                                 /* HAL辅助宏/函数 */

    /* Check if the SMBUS is already enabled */
    if ((hsmbus->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable SMBUS peripheral */
      __HAL_SMBUS_ENABLE(hsmbus);                       /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Disable Pos */
    CLEAR_BIT(hsmbus->Instance->CR1, I2C_CR1_POS);      /* 执行语句 */

    hsmbus->State     = HAL_SMBUS_STATE_BUSY_RX;        /* 赋值或初始化 */
    hsmbus->ErrorCode = HAL_SMBUS_ERROR_NONE;           /* 赋值或初始化 */
    hsmbus->Mode      = HAL_SMBUS_MODE_MASTER;          /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hsmbus->pBuffPtr    = pData;                        /* 赋值或初始化 */
    hsmbus->XferCount   = Size;                         /* 赋值或初始化 */
    hsmbus->XferOptions = XferOptions;                  /* 赋值或初始化 */
    hsmbus->XferSize    = hsmbus->XferCount;            /* 赋值或初始化 */
    hsmbus->Devaddress  = DevAddress;                   /* 赋值或初始化 */

    if ((hsmbus->PreviousState == SMBUS_STATE_MASTER_BUSY_TX) || (hsmbus->PreviousState == SMBUS_STATE_NONE))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Generate Start condition if first transfer */
      if ((XferOptions == SMBUS_NEXT_FRAME)  || (XferOptions == SMBUS_FIRST_AND_LAST_FRAME_NO_PEC) || (XferOptions == SMBUS_FIRST_AND_LAST_FRAME_WITH_PEC) || (XferOptions == SMBUS_FIRST_FRAME)  || (XferOptions == SMBUS_NO_OPTION_FRAME))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Enable Acknowledge */
        SET_BIT(hsmbus->Instance->CR1, I2C_CR1_ACK);    /* 执行语句 */

        /* Generate Start */
        SET_BIT(hsmbus->Instance->CR1, I2C_CR1_START);  /* 执行语句 */
      }                                                 /* 代码块结束 */

      if ((XferOptions == SMBUS_LAST_FRAME_NO_PEC) || (XferOptions == SMBUS_LAST_FRAME_WITH_PEC))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        if (hsmbus->PreviousState == SMBUS_STATE_NONE)  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Enable Acknowledge */
          SET_BIT(hsmbus->Instance->CR1, I2C_CR1_ACK);  /* 执行语句 */
        }                                               /* 代码块结束 */

        if (hsmbus->PreviousState == SMBUS_STATE_MASTER_BUSY_TX)  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Enable Acknowledge */
          SET_BIT(hsmbus->Instance->CR1, I2C_CR1_ACK);  /* 执行语句 */

          /* Generate Start */
          SET_BIT(hsmbus->Instance->CR1, I2C_CR1_START);  /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */



    /* Process Unlocked */
    __HAL_UNLOCK(hsmbus);                               /* HAL辅助宏/函数 */

    /* Note : The SMBUS interrupts must be enabled after unlocking current process
    to avoid the risk of SMBUS interrupt handle execution before current
    process unlock */

    /* Enable EVT, BUF and ERR interrupt */
    __HAL_SMBUS_ENABLE_IT(hsmbus, SMBUS_IT_EVT | SMBUS_IT_BUF | SMBUS_IT_ERR);  /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Abort a master/host SMBUS process communication with Interrupt.
  * @note   This abort can be called only if state is ready
  * @param  hsmbus Pointer to a SMBUS_HandleTypeDef structure that contains
  *         the configuration information for the specified SMBUS.
  * @param  DevAddress Target device address The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SMBUS_Master_Abort_IT(SMBUS_HandleTypeDef *hsmbus, uint16_t DevAddress)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(DevAddress);                                   /* 执行语句 */
  if (hsmbus->Init.PeripheralMode == SMBUS_PERIPHERAL_MODE_SMBUS_HOST)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Process Locked */
    __HAL_LOCK(hsmbus);                                 /* HAL辅助宏/函数 */

    hsmbus->ErrorCode = HAL_SMBUS_ERROR_NONE;           /* 赋值或初始化 */

    hsmbus->PreviousState = SMBUS_STATE_NONE;           /* 赋值或初始化 */
    hsmbus->State = HAL_SMBUS_STATE_ABORT;              /* 赋值或初始化 */


    /* Disable Acknowledge */
    CLEAR_BIT(hsmbus->Instance->CR1, I2C_CR1_ACK);      /* 执行语句 */

    /* Generate Stop */
    SET_BIT(hsmbus->Instance->CR1, I2C_CR1_STOP);       /* 执行语句 */

    hsmbus->XferCount = 0U;                             /* 赋值或初始化 */

    /* Disable EVT, BUF and ERR interrupt */
    __HAL_SMBUS_DISABLE_IT(hsmbus, SMBUS_IT_EVT | SMBUS_IT_BUF | SMBUS_IT_ERR);  /* HAL辅助宏/函数 */

    /* Process Unlocked */
    __HAL_UNLOCK(hsmbus);                               /* HAL辅助宏/函数 */

    /* Call the corresponding callback to inform upper layer of End of Transfer */
    SMBUS_ITError(hsmbus);                              /* 执行语句 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */


/**
  * @brief  Transmit in slave/device SMBUS mode an amount of data in non blocking mode with Interrupt.
  * @param  hsmbus Pointer to a SMBUS_HandleTypeDef structure that contains
  *         the configuration information for the specified SMBUS.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  XferOptions Options of Transfer, value of @ref SMBUS_XferOptions_definition
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SMBUS_Slave_Transmit_IT(SMBUS_HandleTypeDef *hsmbus, uint8_t *pData, uint16_t Size, uint32_t XferOptions)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_SMBUS_TRANSFER_OPTIONS_REQUEST(XferOptions));  /* 执行语句 */

  if (hsmbus->State == HAL_SMBUS_STATE_LISTEN)          /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) || (Size == 0U))                /* 条件判断 */
    {                                                   /* 代码块开始 */
      return  HAL_ERROR;                                /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hsmbus);                                 /* HAL辅助宏/函数 */

    /* Check if the SMBUS is already enabled */
    if ((hsmbus->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable SMBUS peripheral */
      __HAL_SMBUS_ENABLE(hsmbus);                       /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Disable Pos */
    CLEAR_BIT(hsmbus->Instance->CR1, I2C_CR1_POS);      /* 执行语句 */

    hsmbus->State     = HAL_SMBUS_STATE_BUSY_TX_LISTEN;  /* 赋值或初始化 */
    hsmbus->ErrorCode = HAL_SMBUS_ERROR_NONE;           /* 赋值或初始化 */
    hsmbus->Mode      = HAL_SMBUS_MODE_SLAVE;           /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hsmbus->pBuffPtr    = pData;                        /* 赋值或初始化 */
    hsmbus->XferCount   = Size;                         /* 赋值或初始化 */
    hsmbus->XferOptions = XferOptions;                  /* 赋值或初始化 */
    hsmbus->XferSize    = hsmbus->XferCount;            /* 赋值或初始化 */

    /* Clear ADDR flag after prepare the transfer parameters */
    /* This action will generate an acknowledge to the HOST */
    __HAL_SMBUS_CLEAR_ADDRFLAG(hsmbus);                 /* HAL辅助宏/函数 */

    /* Process Unlocked */
    __HAL_UNLOCK(hsmbus);                               /* HAL辅助宏/函数 */

    /* Note : The SMBUS interrupts must be enabled after unlocking current process
              to avoid the risk of SMBUS interrupt handle execution before current
              process unlock */

    /* Enable EVT, BUF and ERR interrupt */
    __HAL_SMBUS_ENABLE_IT(hsmbus, SMBUS_IT_EVT | SMBUS_IT_BUF | SMBUS_IT_ERR);  /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Enable the Address listen mode with Interrupt.
  * @param  hsmbus Pointer to a SMBUS_HandleTypeDef structure that contains
  *         the configuration information for the specified SMBUS.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  XferOptions Options of Transfer, value of @ref SMBUS_XferOptions_definition
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SMBUS_Slave_Receive_IT(SMBUS_HandleTypeDef *hsmbus, uint8_t *pData, uint16_t Size, uint32_t XferOptions)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_SMBUS_TRANSFER_OPTIONS_REQUEST(XferOptions));  /* 执行语句 */

  if (hsmbus->State == HAL_SMBUS_STATE_LISTEN)          /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) || (Size == 0U))                /* 条件判断 */
    {                                                   /* 代码块开始 */
      return  HAL_ERROR;                                /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hsmbus);                                 /* HAL辅助宏/函数 */

    /* Check if the SMBUS is already enabled */
    if ((hsmbus->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable SMBUS peripheral */
      __HAL_SMBUS_ENABLE(hsmbus);                       /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Disable Pos */
    CLEAR_BIT(hsmbus->Instance->CR1, I2C_CR1_POS);      /* 执行语句 */

    hsmbus->State     = HAL_SMBUS_STATE_BUSY_RX_LISTEN;  /* 赋值或初始化 */
    hsmbus->ErrorCode = HAL_SMBUS_ERROR_NONE;           /* 赋值或初始化 */
    hsmbus->Mode      = HAL_SMBUS_MODE_SLAVE;           /* 赋值或初始化 */



    /* Prepare transfer parameters */
    hsmbus->pBuffPtr = pData;                           /* 赋值或初始化 */
    hsmbus->XferCount = Size;                           /* 赋值或初始化 */
    hsmbus->XferOptions = XferOptions;                  /* 赋值或初始化 */
    hsmbus->XferSize    = hsmbus->XferCount;            /* 赋值或初始化 */

    __HAL_SMBUS_CLEAR_ADDRFLAG(hsmbus);                 /* HAL辅助宏/函数 */

    /* Process Unlocked */
    __HAL_UNLOCK(hsmbus);                               /* HAL辅助宏/函数 */

    /* Note : The SMBUS interrupts must be enabled after unlocking current process
              to avoid the risk of SMBUS interrupt handle execution before current
              process unlock */

    /* Enable EVT, BUF and ERR interrupt */
    __HAL_SMBUS_ENABLE_IT(hsmbus, SMBUS_IT_EVT | SMBUS_IT_BUF | SMBUS_IT_ERR);  /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */


/**
  * @brief  Enable the Address listen mode with Interrupt.
  * @param  hsmbus Pointer to a SMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified SMBUS.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SMBUS_EnableListen_IT(SMBUS_HandleTypeDef *hsmbus)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  if (hsmbus->State == HAL_SMBUS_STATE_READY)           /* 条件判断 */
  {                                                     /* 代码块开始 */
    hsmbus->State = HAL_SMBUS_STATE_LISTEN;             /* 赋值或初始化 */

    /* Check if the SMBUS is already enabled */
    if ((hsmbus->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable SMBUS peripheral */
      __HAL_SMBUS_ENABLE(hsmbus);                       /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Enable Address Acknowledge */
    SET_BIT(hsmbus->Instance->CR1, I2C_CR1_ACK);        /* 执行语句 */

    /* Enable EVT and ERR interrupt */
    __HAL_SMBUS_ENABLE_IT(hsmbus, SMBUS_IT_EVT | SMBUS_IT_ERR);  /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Disable the Address listen mode with Interrupt.
  * @param  hsmbus Pointer to a SMBUS_HandleTypeDef structure that contains
  *         the configuration information for the specified SMBUS.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SMBUS_DisableListen_IT(SMBUS_HandleTypeDef *hsmbus)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Declaration of tmp to prevent undefined behavior of volatile usage */
  uint32_t tmp;                                         /* 32位无符号变量/函数 */

  /* Disable Address listen mode only if a transfer is not ongoing */
  if (hsmbus->State == HAL_SMBUS_STATE_LISTEN)          /* 条件判断 */
  {                                                     /* 代码块开始 */
    tmp = (uint32_t)(hsmbus->State) & SMBUS_STATE_MSK;  /* 赋值或初始化 */
    hsmbus->PreviousState = tmp | (uint32_t)(hsmbus->Mode);  /* 赋值或初始化 */
    hsmbus->State = HAL_SMBUS_STATE_READY;              /* 赋值或初始化 */
    hsmbus->Mode = HAL_SMBUS_MODE_NONE;                 /* 赋值或初始化 */

    /* Disable Address Acknowledge */
    CLEAR_BIT(hsmbus->Instance->CR1, I2C_CR1_ACK);      /* 执行语句 */

    /* Disable EVT and ERR interrupt */
    __HAL_SMBUS_DISABLE_IT(hsmbus, SMBUS_IT_EVT | SMBUS_IT_ERR);  /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Enable the SMBUS alert mode with Interrupt.
  * @param  hsmbus Pointer to a SMBUS_HandleTypeDef structure that contains
  *         the configuration information for the specified SMBUSx peripheral.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SMBUS_EnableAlert_IT(SMBUS_HandleTypeDef *hsmbus)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Enable SMBus alert */
  SET_BIT(hsmbus->Instance->CR1, I2C_CR1_ALERT);        /* 执行语句 */

  /* Clear ALERT flag */
  __HAL_SMBUS_CLEAR_FLAG(hsmbus, SMBUS_FLAG_SMBALERT);  /* HAL辅助宏/函数 */

  /* Enable Alert Interrupt */
  __HAL_SMBUS_ENABLE_IT(hsmbus, SMBUS_IT_ERR);          /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */
/**
  * @brief  Disable the SMBUS alert mode with Interrupt.
  * @param  hsmbus Pointer to a SMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified SMBUSx peripheral.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SMBUS_DisableAlert_IT(SMBUS_HandleTypeDef *hsmbus)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Disable SMBus alert */
  CLEAR_BIT(hsmbus->Instance->CR1, I2C_CR1_ALERT);      /* 执行语句 */

  /* Disable Alert Interrupt */
  __HAL_SMBUS_DISABLE_IT(hsmbus, SMBUS_IT_ERR);         /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */


/**
  * @brief  Check if target device is ready for communication.
  * @param  hsmbus Pointer to a SMBUS_HandleTypeDef structure that contains
  *         the configuration information for the specified SMBUS.
  * @param  DevAddress Target device address The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  Trials Number of trials
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SMBUS_IsDeviceReady(SMBUS_HandleTypeDef *hsmbus, uint16_t DevAddress, uint32_t Trials, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart = 0U, tmp1 = 0U, tmp2 = 0U, tmp3 = 0U, SMBUS_Trials = 1U;  /* 32位无符号变量/函数 */

  /* Get tick */
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  if (hsmbus->State == HAL_SMBUS_STATE_READY)           /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Wait until BUSY flag is reset */
    if (SMBUS_WaitOnFlagUntilTimeout(hsmbus, SMBUS_FLAG_BUSY, SET, SMBUS_TIMEOUT_BUSY_FLAG, tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_BUSY;                                  /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hsmbus);                                 /* HAL辅助宏/函数 */

    /* Check if the SMBUS is already enabled */
    if ((hsmbus->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable SMBUS peripheral */
      __HAL_SMBUS_ENABLE(hsmbus);                       /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Disable Pos */
    CLEAR_BIT(hsmbus->Instance->CR1, I2C_CR1_POS);      /* 执行语句 */

    hsmbus->State = HAL_SMBUS_STATE_BUSY;               /* 赋值或初始化 */
    hsmbus->ErrorCode = HAL_SMBUS_ERROR_NONE;           /* 赋值或初始化 */
    hsmbus->XferOptions = SMBUS_NO_OPTION_FRAME;        /* 赋值或初始化 */

    do                                                  /* do-while循环 */
    {                                                   /* 代码块开始 */
      /* Generate Start */
      SET_BIT(hsmbus->Instance->CR1, I2C_CR1_START);    /* 执行语句 */

      /* Wait until SB flag is set */
      if (SMBUS_WaitOnFlagUntilTimeout(hsmbus, SMBUS_FLAG_SB, RESET, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */

      /* Send slave address */
      hsmbus->Instance->DR = SMBUS_7BIT_ADD_WRITE(DevAddress);  /* 赋值或初始化 */

      /* Wait until ADDR or AF flag are set */
      /* Get tick */
      tickstart = HAL_GetTick();                        /* 赋值或初始化 */

      tmp1 = __HAL_SMBUS_GET_FLAG(hsmbus, SMBUS_FLAG_ADDR);  /* 赋值或初始化 */
      tmp2 = __HAL_SMBUS_GET_FLAG(hsmbus, SMBUS_FLAG_AF);  /* 赋值或初始化 */
      tmp3 = hsmbus->State;                             /* 赋值或初始化 */
      while ((tmp1 == RESET) && (tmp2 == RESET) && (tmp3 != HAL_SMBUS_STATE_TIMEOUT))  /* while循环 */
      {                                                 /* 代码块开始 */
        if ((Timeout == 0U) || ((HAL_GetTick() - tickstart) > Timeout))  /* 条件判断 */
        {                                               /* 代码块开始 */
          hsmbus->State = HAL_SMBUS_STATE_TIMEOUT;      /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        tmp1 = __HAL_SMBUS_GET_FLAG(hsmbus, SMBUS_FLAG_ADDR);  /* 赋值或初始化 */
        tmp2 = __HAL_SMBUS_GET_FLAG(hsmbus, SMBUS_FLAG_AF);  /* 赋值或初始化 */
        tmp3 = hsmbus->State;                           /* 赋值或初始化 */
      }                                                 /* 代码块结束 */

      hsmbus->State = HAL_SMBUS_STATE_READY;            /* 赋值或初始化 */

      /* Check if the ADDR flag has been set */
      if (__HAL_SMBUS_GET_FLAG(hsmbus, SMBUS_FLAG_ADDR) == SET)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Generate Stop */
        SET_BIT(hsmbus->Instance->CR1, I2C_CR1_STOP);   /* 执行语句 */

        /* Clear ADDR Flag */
        __HAL_SMBUS_CLEAR_ADDRFLAG(hsmbus);             /* HAL辅助宏/函数 */

        /* Wait until BUSY flag is reset */
        if (SMBUS_WaitOnFlagUntilTimeout(hsmbus, SMBUS_FLAG_BUSY, SET, SMBUS_TIMEOUT_BUSY_FLAG, tickstart) != HAL_OK)  /* 条件判断 */
        {                                               /* 代码块开始 */
          return HAL_TIMEOUT;                           /* 返回值 */
        }                                               /* 代码块结束 */

        hsmbus->State = HAL_SMBUS_STATE_READY;          /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hsmbus);                           /* HAL辅助宏/函数 */

        return HAL_OK;                                  /* 返回值 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Generate Stop */
        SET_BIT(hsmbus->Instance->CR1, I2C_CR1_STOP);   /* 执行语句 */

        /* Clear AF Flag */
        __HAL_SMBUS_CLEAR_FLAG(hsmbus, SMBUS_FLAG_AF);  /* HAL辅助宏/函数 */

        /* Wait until BUSY flag is reset */
        if (SMBUS_WaitOnFlagUntilTimeout(hsmbus, SMBUS_FLAG_BUSY, SET, SMBUS_TIMEOUT_BUSY_FLAG, tickstart) != HAL_OK)  /* 条件判断 */
        {                                               /* 代码块开始 */
          return HAL_TIMEOUT;                           /* 返回值 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    while (SMBUS_Trials++ < Trials);                    /* while循环 */

    hsmbus->State = HAL_SMBUS_STATE_READY;              /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hsmbus);                               /* HAL辅助宏/函数 */

    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  This function handles SMBUS event interrupt request.
  * @param  hsmbus Pointer to a SMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified SMBUS.
  * @retval None
  */
void HAL_SMBUS_EV_IRQHandler(SMBUS_HandleTypeDef *hsmbus)  /* 中断服务函数 */
{                                                       /* 代码块开始 */
  uint32_t sr2itflags   = READ_REG(hsmbus->Instance->SR2);  /* 32位无符号变量/函数 */
  uint32_t sr1itflags   = READ_REG(hsmbus->Instance->SR1);  /* 32位无符号变量/函数 */
  uint32_t itsources    = READ_REG(hsmbus->Instance->CR2);  /* 32位无符号变量/函数 */

  uint32_t CurrentMode  = hsmbus->Mode;                 /* 32位无符号变量/函数 */

  /* Master mode selected */
  if (CurrentMode == HAL_SMBUS_MODE_MASTER)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* SB Set ----------------------------------------------------------------*/
    if (((sr1itflags & SMBUS_FLAG_SB) != RESET) && ((itsources & SMBUS_IT_EVT) != RESET))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      SMBUS_Master_SB(hsmbus);                          /* 执行语句 */
    }                                                   /* 代码块结束 */
    /* ADD10 Set -------------------------------------------------------------*/
    else if (((sr1itflags & SMBUS_FLAG_ADD10) != RESET) && ((itsources & SMBUS_IT_EVT) != RESET))  /* 否则如果 */
    {                                                   /* 代码块开始 */
      SMBUS_Master_ADD10(hsmbus);                       /* 执行语句 */
    }                                                   /* 代码块结束 */
    /* ADDR Set --------------------------------------------------------------*/
    else if (((sr1itflags & SMBUS_FLAG_ADDR) != RESET) && ((itsources & SMBUS_IT_EVT) != RESET))  /* 否则如果 */
    {                                                   /* 代码块开始 */
      SMBUS_Master_ADDR(hsmbus);                        /* 执行语句 */
    }                                                   /* 代码块结束 */
    /* SMBUS in mode Transmitter -----------------------------------------------*/
    if ((sr2itflags & SMBUS_FLAG_TRA) != RESET)         /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* TXE set and BTF reset -----------------------------------------------*/
      if (((sr1itflags & SMBUS_FLAG_TXE) != RESET) && ((itsources & SMBUS_IT_BUF) != RESET) && ((sr1itflags & SMBUS_FLAG_BTF) == RESET))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        SMBUS_MasterTransmit_TXE(hsmbus);               /* 执行语句 */
      }                                                 /* 代码块结束 */
      /* BTF set -------------------------------------------------------------*/
      else if (((sr1itflags & SMBUS_FLAG_BTF) != RESET) && ((itsources & SMBUS_IT_EVT) != RESET))  /* 否则如果 */
      {                                                 /* 代码块开始 */
        SMBUS_MasterTransmit_BTF(hsmbus);               /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    /* SMBUS in mode Receiver --------------------------------------------------*/
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* RXNE set and BTF reset -----------------------------------------------*/
      if (((sr1itflags & SMBUS_FLAG_RXNE) != RESET) && ((itsources & SMBUS_IT_BUF) != RESET) && ((sr1itflags & SMBUS_FLAG_BTF) == RESET))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        SMBUS_MasterReceive_RXNE(hsmbus);               /* 执行语句 */
      }                                                 /* 代码块结束 */
      /* BTF set -------------------------------------------------------------*/
      else if (((sr1itflags & SMBUS_FLAG_BTF) != RESET) && ((itsources & SMBUS_IT_EVT) != RESET))  /* 否则如果 */
      {                                                 /* 代码块开始 */
        SMBUS_MasterReceive_BTF(hsmbus);                /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /* Slave mode selected */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* ADDR set --------------------------------------------------------------*/
    if (((sr1itflags & SMBUS_FLAG_ADDR) != RESET) && ((itsources & SMBUS_IT_EVT) != RESET))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      SMBUS_Slave_ADDR(hsmbus);                         /* 执行语句 */
    }                                                   /* 代码块结束 */
    /* STOPF set --------------------------------------------------------------*/
    else if (((sr1itflags & SMBUS_FLAG_STOPF) != RESET) && ((itsources & SMBUS_IT_EVT) != RESET))  /* 否则如果 */
    {                                                   /* 代码块开始 */
      SMBUS_Slave_STOPF(hsmbus);                        /* 执行语句 */
    }                                                   /* 代码块结束 */
    /* SMBUS in mode Transmitter -----------------------------------------------*/
    else if ((sr2itflags & SMBUS_FLAG_TRA) != RESET)    /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* TXE set and BTF reset -----------------------------------------------*/
      if (((sr1itflags & SMBUS_FLAG_TXE) != RESET) && ((itsources & SMBUS_IT_BUF) != RESET) && ((sr1itflags & SMBUS_FLAG_BTF) == RESET))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        SMBUS_SlaveTransmit_TXE(hsmbus);                /* 执行语句 */
      }                                                 /* 代码块结束 */
      /* BTF set -------------------------------------------------------------*/
      else if (((sr1itflags & SMBUS_FLAG_BTF) != RESET) && ((itsources & SMBUS_IT_EVT) != RESET))  /* 否则如果 */
      {                                                 /* 代码块开始 */
        SMBUS_SlaveTransmit_BTF(hsmbus);                /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    /* SMBUS in mode Receiver --------------------------------------------------*/
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* RXNE set and BTF reset ----------------------------------------------*/
      if (((sr1itflags & SMBUS_FLAG_RXNE) != RESET) && ((itsources & SMBUS_IT_BUF) != RESET) && ((sr1itflags & SMBUS_FLAG_BTF) == RESET))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        SMBUS_SlaveReceive_RXNE(hsmbus);                /* 执行语句 */
      }                                                 /* 代码块结束 */
      /* BTF set -------------------------------------------------------------*/
      else if (((sr1itflags & SMBUS_FLAG_BTF) != RESET) && ((itsources & SMBUS_IT_EVT) != RESET))  /* 否则如果 */
      {                                                 /* 代码块开始 */
        SMBUS_SlaveReceive_BTF(hsmbus);                 /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  This function handles SMBUS error interrupt request.
  * @param  hsmbus Pointer to a SMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified SMBUS.
  * @retval None
  */
void HAL_SMBUS_ER_IRQHandler(SMBUS_HandleTypeDef *hsmbus)  /* 中断服务函数 */
{                                                       /* 代码块开始 */
  uint32_t tmp1 = 0U, tmp2 = 0U, tmp3 = 0U, tmp4 = 0U;  /* 32位无符号变量/函数 */
  uint32_t sr1itflags = READ_REG(hsmbus->Instance->SR1);  /* 32位无符号变量/函数 */
  uint32_t itsources  = READ_REG(hsmbus->Instance->CR2);  /* 32位无符号变量/函数 */

  /* SMBUS Bus error interrupt occurred ------------------------------------*/
  if (((sr1itflags & SMBUS_FLAG_BERR) != RESET) && ((itsources & SMBUS_IT_ERR) != RESET))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hsmbus->ErrorCode |= HAL_SMBUS_ERROR_BERR;          /* 赋值或初始化 */

    /* Clear BERR flag */
    __HAL_SMBUS_CLEAR_FLAG(hsmbus, SMBUS_FLAG_BERR);    /* HAL辅助宏/函数 */

  }                                                     /* 代码块结束 */

  /* SMBUS Over-Run/Under-Run interrupt occurred ----------------------------------------*/
  if (((sr1itflags & SMBUS_FLAG_OVR) != RESET) && ((itsources & SMBUS_IT_ERR) != RESET))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hsmbus->ErrorCode |= HAL_SMBUS_ERROR_OVR;           /* 赋值或初始化 */

    /* Clear OVR flag */
    __HAL_SMBUS_CLEAR_FLAG(hsmbus, SMBUS_FLAG_OVR);     /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* SMBUS Arbitration Loss error interrupt occurred ------------------------------------*/
  if (((sr1itflags & SMBUS_FLAG_ARLO) != RESET) && ((itsources & SMBUS_IT_ERR) != RESET))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hsmbus->ErrorCode |= HAL_SMBUS_ERROR_ARLO;          /* 赋值或初始化 */

    /* Clear ARLO flag */
    __HAL_SMBUS_CLEAR_FLAG(hsmbus, SMBUS_FLAG_ARLO);    /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* SMBUS Acknowledge failure error interrupt occurred ------------------------------------*/
  if (((sr1itflags & SMBUS_FLAG_AF) != RESET) && ((itsources & SMBUS_IT_ERR) != RESET))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    tmp1 = hsmbus->Mode;                                /* 赋值或初始化 */
    tmp2 = hsmbus->XferCount;                           /* 赋值或初始化 */
    tmp3 = hsmbus->State;                               /* 赋值或初始化 */
    tmp4 = hsmbus->PreviousState;                       /* 赋值或初始化 */

    if ((tmp1 == HAL_SMBUS_MODE_SLAVE) && (tmp2 == 0U) && \
        ((tmp3 == HAL_SMBUS_STATE_BUSY_TX) || (tmp3 == HAL_SMBUS_STATE_BUSY_TX_LISTEN) || \
         ((tmp3 == HAL_SMBUS_STATE_LISTEN) && (tmp4 == SMBUS_STATE_SLAVE_BUSY_TX))))  /* 源代码 */
    {                                                   /* 代码块开始 */
      SMBUS_Slave_AF(hsmbus);                           /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hsmbus->ErrorCode |= HAL_SMBUS_ERROR_AF;          /* 赋值或初始化 */

      /* Do not generate a STOP in case of Slave receive non acknowledge during transfer (mean not at the end of transfer) */
      if (hsmbus->Mode == HAL_SMBUS_MODE_MASTER)        /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Generate Stop */
        SET_BIT(hsmbus->Instance->CR1, I2C_CR1_STOP);   /* 执行语句 */

      }                                                 /* 代码块结束 */

      /* Clear AF flag */
      __HAL_SMBUS_CLEAR_FLAG(hsmbus, SMBUS_FLAG_AF);    /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* SMBUS Timeout error interrupt occurred ---------------------------------------------*/
  if (((sr1itflags & SMBUS_FLAG_TIMEOUT) != RESET) && ((itsources & SMBUS_IT_ERR) != RESET))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hsmbus->ErrorCode |= HAL_SMBUS_ERROR_TIMEOUT;       /* 赋值或初始化 */

    /* Clear TIMEOUT flag */
    __HAL_SMBUS_CLEAR_FLAG(hsmbus, SMBUS_FLAG_TIMEOUT);  /* HAL辅助宏/函数 */

  }                                                     /* 代码块结束 */

  /* SMBUS Alert error interrupt occurred -----------------------------------------------*/
  if (((sr1itflags & SMBUS_FLAG_SMBALERT) != RESET) && ((itsources & SMBUS_IT_ERR) != RESET))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hsmbus->ErrorCode |= HAL_SMBUS_ERROR_ALERT;         /* 赋值或初始化 */

    /* Clear ALERT flag */
    __HAL_SMBUS_CLEAR_FLAG(hsmbus, SMBUS_FLAG_SMBALERT);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* SMBUS Packet Error Check error interrupt occurred ----------------------------------*/
  if (((sr1itflags & SMBUS_FLAG_PECERR) != RESET) && ((itsources & SMBUS_IT_ERR) != RESET))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hsmbus->ErrorCode |= SMBUS_FLAG_PECERR;             /* 赋值或初始化 */

    /* Clear PEC error flag */
    __HAL_SMBUS_CLEAR_FLAG(hsmbus, SMBUS_FLAG_PECERR);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Call the Error Callback in case of Error detected -----------------------*/
  if (hsmbus->ErrorCode != HAL_SMBUS_ERROR_NONE)        /* 条件判断 */
  {                                                     /* 代码块开始 */
    SMBUS_ITError(hsmbus);                              /* 执行语句 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Master Tx Transfer completed callback.
  * @param  hsmbus Pointer to a SMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified SMBUS.
  * @retval None
  */
__weak void HAL_SMBUS_MasterTxCpltCallback(SMBUS_HandleTypeDef *hsmbus)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsmbus);                                       /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SMBUS_MasterTxCpltCallback can be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Master Rx Transfer completed callback.
  * @param  hsmbus Pointer to a SMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified SMBUS.
  * @retval None
  */
__weak void HAL_SMBUS_MasterRxCpltCallback(SMBUS_HandleTypeDef *hsmbus)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsmbus);                                       /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SMBUS_MasterRxCpltCallback can be implemented in the user file
   */
}                                                       /* 代码块结束 */

/** @brief  Slave Tx Transfer completed callback.
  * @param  hsmbus Pointer to a SMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified SMBUS.
  * @retval None
  */
__weak void HAL_SMBUS_SlaveTxCpltCallback(SMBUS_HandleTypeDef *hsmbus)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsmbus);                                       /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SMBUS_SlaveTxCpltCallback can be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Slave Rx Transfer completed callback.
  * @param  hsmbus Pointer to a SMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified SMBUS.
  * @retval None
  */
__weak void HAL_SMBUS_SlaveRxCpltCallback(SMBUS_HandleTypeDef *hsmbus)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsmbus);                                       /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SMBUS_SlaveRxCpltCallback can be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Slave Address Match callback.
  * @param  hsmbus Pointer to a SMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified SMBUS.
  * @param  TransferDirection Master request Transfer Direction (Write/Read), value of @ref SMBUS_XferOptions_definition
  * @param  AddrMatchCode Address Match Code
  * @retval None
  */
__weak void HAL_SMBUS_AddrCallback(SMBUS_HandleTypeDef *hsmbus, uint8_t TransferDirection, uint16_t AddrMatchCode)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsmbus);                                       /* 执行语句 */
  UNUSED(TransferDirection);                            /* 执行语句 */
  UNUSED(AddrMatchCode);                                /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SMBUS_AddrCallback can be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Listen Complete callback.
  * @param  hsmbus Pointer to a SMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified SMBUS.
  * @retval None
  */
__weak void HAL_SMBUS_ListenCpltCallback(SMBUS_HandleTypeDef *hsmbus)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsmbus);                                       /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
          the HAL_SMBUS_ListenCpltCallback can be implemented in the user file
  */
}                                                       /* 代码块结束 */

/**
  * @brief  SMBUS error callback.
  * @param  hsmbus Pointer to a SMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified SMBUS.
  * @retval None
  */
__weak void HAL_SMBUS_ErrorCallback(SMBUS_HandleTypeDef *hsmbus)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsmbus);                                       /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SMBUS_ErrorCallback can be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  SMBUS abort callback.
  * @param  hsmbus Pointer to a SMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified SMBUS.
  * @retval None
  */
__weak void HAL_SMBUS_AbortCpltCallback(SMBUS_HandleTypeDef *hsmbus)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsmbus);                                       /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SMBUS_AbortCpltCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup   SMBUS_Exported_Functions_Group3 Peripheral State, Mode and Error functions
  *  @brief   Peripheral State and Errors functions
  *
@verbatim
 ===============================================================================
            ##### Peripheral State, Mode and Error functions #####
 ===============================================================================
    [..]
    This subsection permits to get in run-time the status of the peripheral
    and the data flow.

@endverbatim
  * @{
  */

/**
  * @brief  Return the SMBUS handle state.
  * @param  hsmbus Pointer to a SMBUS_HandleTypeDef structure that contains
  *                the configuration information for the specified SMBUS.
  * @retval HAL state
  */
HAL_SMBUS_StateTypeDef HAL_SMBUS_GetState(SMBUS_HandleTypeDef *hsmbus)  /* HAL库函数调用 */
{                                                       /* 代码块开始 */
  /* Return SMBUS handle state */
  return hsmbus->State;                                 /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return the SMBUS Master, Slave or no mode.
  * @param  hsmbus Pointer to a SMBUS_HandleTypeDef structure that contains
  *                the configuration information for SMBUS module
  * @retval HAL mode
  */
HAL_SMBUS_ModeTypeDef HAL_SMBUS_GetMode(SMBUS_HandleTypeDef *hsmbus)  /* HAL库函数调用 */
{                                                       /* 代码块开始 */
  return hsmbus->Mode;                                  /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return the SMBUS error code
  * @param  hsmbus Pointer to a SMBUS_HandleTypeDef structure that contains
  *              the configuration information for the specified SMBUS.
  * @retval SMBUS Error Code
  */
uint32_t HAL_SMBUS_GetError(SMBUS_HandleTypeDef *hsmbus)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  return hsmbus->ErrorCode;                             /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup SMBUS_Private_Functions
  * @{
  */

/**
  * @brief  Handle TXE flag for Master
  * @param  hsmbus Pointer to a SMBUS_HandleTypeDef structure that contains
  *         the configuration information for SMBUS module
  * @retval HAL status
  */
static HAL_StatusTypeDef SMBUS_MasterTransmit_TXE(SMBUS_HandleTypeDef *hsmbus)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  /* Declaration of temporary variables to prevent undefined behavior of volatile usage */
  uint32_t CurrentState       = hsmbus->State;          /* 32位无符号变量/函数 */
  uint32_t CurrentXferOptions = hsmbus->XferOptions;    /* 32位无符号变量/函数 */

  if ((hsmbus->XferSize == 0U) && (CurrentState == HAL_SMBUS_STATE_BUSY_TX))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Call TxCpltCallback() directly if no stop mode is set */
    if (((CurrentXferOptions == SMBUS_FIRST_FRAME) || (CurrentXferOptions == SMBUS_NEXT_FRAME)) && (CurrentXferOptions != SMBUS_NO_OPTION_FRAME))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      __HAL_SMBUS_DISABLE_IT(hsmbus, SMBUS_IT_EVT | SMBUS_IT_BUF | SMBUS_IT_ERR);  /* HAL辅助宏/函数 */

      hsmbus->PreviousState = SMBUS_STATE_MASTER_BUSY_TX;  /* 赋值或初始化 */
      hsmbus->Mode = HAL_SMBUS_MODE_NONE;               /* 赋值或初始化 */
      hsmbus->State = HAL_SMBUS_STATE_READY;            /* 赋值或初始化 */

#if (USE_HAL_SMBUS_REGISTER_CALLBACKS == 1)             /* 条件编译 */
      hsmbus->MasterTxCpltCallback(hsmbus);             /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_SMBUS_MasterTxCpltCallback(hsmbus);           /* HAL库函数调用 */
#endif /* USE_HAL_SMBUS_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
    else /* Generate Stop condition then Call TxCpltCallback() */
    {                                                   /* 代码块开始 */
      /* Disable EVT, BUF and ERR interrupt */
      __HAL_SMBUS_DISABLE_IT(hsmbus, SMBUS_IT_EVT | SMBUS_IT_BUF | SMBUS_IT_ERR);  /* HAL辅助宏/函数 */

      /* Generate Stop */
      SET_BIT(hsmbus->Instance->CR1, I2C_CR1_STOP);     /* 执行语句 */

      hsmbus->PreviousState = HAL_SMBUS_STATE_READY;    /* 赋值或初始化 */
      hsmbus->State = HAL_SMBUS_STATE_READY;            /* 赋值或初始化 */

      hsmbus->Mode = HAL_SMBUS_MODE_NONE;               /* 赋值或初始化 */
#if (USE_HAL_SMBUS_REGISTER_CALLBACKS == 1)             /* 条件编译 */
      hsmbus->MasterTxCpltCallback(hsmbus);             /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_SMBUS_MasterTxCpltCallback(hsmbus);           /* HAL库函数调用 */
#endif /* USE_HAL_SMBUS_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if ((CurrentState == HAL_SMBUS_STATE_BUSY_TX))   /* 否则如果 */
  {                                                     /* 代码块开始 */

    if ((hsmbus->XferCount == 2U) && (SMBUS_GET_PEC_MODE(hsmbus) == SMBUS_PEC_ENABLE) && ((hsmbus->XferOptions == SMBUS_FIRST_AND_LAST_FRAME_WITH_PEC) || (hsmbus->XferOptions == SMBUS_LAST_FRAME_WITH_PEC)))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hsmbus->XferCount--;                              /* 执行语句 */
    }                                                   /* 代码块结束 */

    if (hsmbus->XferCount == 0U)                        /* 条件判断 */
    {                                                   /* 代码块开始 */

      /* Disable BUF interrupt */
      __HAL_SMBUS_DISABLE_IT(hsmbus, SMBUS_IT_BUF);     /* HAL辅助宏/函数 */

      if ((SMBUS_GET_PEC_MODE(hsmbus) == SMBUS_PEC_ENABLE) && ((hsmbus->XferOptions == SMBUS_FIRST_AND_LAST_FRAME_WITH_PEC) || (hsmbus->XferOptions == SMBUS_LAST_FRAME_WITH_PEC)))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        SET_BIT(hsmbus->Instance->CR1, I2C_CR1_PEC);    /* 执行语句 */
      }                                                 /* 代码块结束 */

    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Write data to DR */
      hsmbus->Instance->DR = (*hsmbus->pBuffPtr++);     /* 赋值或初始化 */
      hsmbus->XferCount--;                              /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Handle BTF flag for Master transmitter
  * @param  hsmbus Pointer to a SMBUS_HandleTypeDef structure that contains
  *         the configuration information for SMBUS module
  * @retval HAL status
  */
static HAL_StatusTypeDef SMBUS_MasterTransmit_BTF(SMBUS_HandleTypeDef *hsmbus)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  /* Declaration of temporary variables to prevent undefined behavior of volatile usage */
  uint32_t CurrentXferOptions = hsmbus->XferOptions;    /* 32位无符号变量/函数 */

  if (hsmbus->State == HAL_SMBUS_STATE_BUSY_TX)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (hsmbus->XferCount != 0U)                        /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Write data to DR */
      hsmbus->Instance->DR = (*hsmbus->pBuffPtr++);     /* 赋值或初始化 */
      hsmbus->XferCount--;                              /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Call TxCpltCallback() directly if no stop mode is set */
      if (((CurrentXferOptions == SMBUS_FIRST_FRAME) || (CurrentXferOptions == SMBUS_NEXT_FRAME)) && (CurrentXferOptions != SMBUS_NO_OPTION_FRAME))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        __HAL_SMBUS_DISABLE_IT(hsmbus, SMBUS_IT_EVT | SMBUS_IT_BUF | SMBUS_IT_ERR);  /* HAL辅助宏/函数 */

        hsmbus->PreviousState = SMBUS_STATE_MASTER_BUSY_TX;  /* 赋值或初始化 */
        hsmbus->Mode = HAL_SMBUS_MODE_NONE;             /* 赋值或初始化 */
        hsmbus->State = HAL_SMBUS_STATE_READY;          /* 赋值或初始化 */

#if (USE_HAL_SMBUS_REGISTER_CALLBACKS == 1)             /* 条件编译 */
        hsmbus->MasterTxCpltCallback(hsmbus);           /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_SMBUS_MasterTxCpltCallback(hsmbus);         /* HAL库函数调用 */
#endif /* USE_HAL_SMBUS_REGISTER_CALLBACKS */
      }                                                 /* 代码块结束 */
      else /* Generate Stop condition then Call TxCpltCallback() */
      {                                                 /* 代码块开始 */
        /* Disable EVT, BUF and ERR interrupt */
        __HAL_SMBUS_DISABLE_IT(hsmbus, SMBUS_IT_EVT | SMBUS_IT_BUF | SMBUS_IT_ERR);  /* HAL辅助宏/函数 */

        /* Generate Stop */
        SET_BIT(hsmbus->Instance->CR1, I2C_CR1_STOP);   /* 执行语句 */

        hsmbus->PreviousState = HAL_SMBUS_STATE_READY;  /* 赋值或初始化 */
        hsmbus->State = HAL_SMBUS_STATE_READY;          /* 赋值或初始化 */
        hsmbus->Mode = HAL_SMBUS_MODE_NONE;             /* 赋值或初始化 */
#if (USE_HAL_SMBUS_REGISTER_CALLBACKS == 1)             /* 条件编译 */
        hsmbus->MasterTxCpltCallback(hsmbus);           /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_SMBUS_MasterTxCpltCallback(hsmbus);         /* HAL库函数调用 */
#endif /* USE_HAL_SMBUS_REGISTER_CALLBACKS */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Handle RXNE flag for Master
  * @param  hsmbus Pointer to a SMBUS_HandleTypeDef structure that contains
  *         the configuration information for SMBUS module
  * @retval HAL status
  */
static HAL_StatusTypeDef SMBUS_MasterReceive_RXNE(SMBUS_HandleTypeDef *hsmbus)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  /* Declaration of temporary variables to prevent undefined behavior of volatile usage */
  uint32_t CurrentXferOptions = hsmbus->XferOptions;    /* 32位无符号变量/函数 */

  if (hsmbus->State == HAL_SMBUS_STATE_BUSY_RX)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    uint32_t tmp = hsmbus->XferCount;                   /* 32位无符号变量/函数 */

    if (tmp > 3U)                                       /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Read data from DR */
      (*hsmbus->pBuffPtr++) = hsmbus->Instance->DR;     /* 赋值或初始化 */
      hsmbus->XferCount--;                              /* 执行语句 */

      if (hsmbus->XferCount == 3)                       /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Disable BUF interrupt, this help to treat correctly the last 4 bytes
        on BTF subroutine */
        /* Disable BUF interrupt */
        __HAL_SMBUS_DISABLE_IT(hsmbus, SMBUS_IT_BUF);   /* HAL辅助宏/函数 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    else if (tmp == 2U)                                 /* 否则如果 */
    {                                                   /* 代码块开始 */

      /* Read data from DR */
      (*hsmbus->pBuffPtr++) = hsmbus->Instance->DR;     /* 赋值或初始化 */
      hsmbus->XferCount--;                              /* 执行语句 */

      if ((CurrentXferOptions == SMBUS_FIRST_AND_LAST_FRAME_WITH_PEC) || (CurrentXferOptions == SMBUS_LAST_FRAME_WITH_PEC))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* PEC of slave */
        hsmbus->XferPEC = SMBUS_GET_PEC(hsmbus);        /* 赋值或初始化 */

      }                                                 /* 代码块结束 */
      /* Generate Stop */
      SET_BIT(hsmbus->Instance->CR1, I2C_CR1_STOP);     /* 执行语句 */
    }                                                   /* 代码块结束 */

    else if ((tmp == 1U) || (tmp == 0U))                /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* Disable Acknowledge */
      CLEAR_BIT(hsmbus->Instance->CR1, I2C_CR1_ACK);    /* 执行语句 */

      /* Disable EVT, BUF and ERR interrupt */
      __HAL_SMBUS_DISABLE_IT(hsmbus, SMBUS_IT_EVT | SMBUS_IT_BUF | SMBUS_IT_ERR);  /* HAL辅助宏/函数 */

      /* Read data from DR */
      (*hsmbus->pBuffPtr++) = hsmbus->Instance->DR;     /* 赋值或初始化 */
      hsmbus->XferCount--;                              /* 执行语句 */

      hsmbus->State = HAL_SMBUS_STATE_READY;            /* 赋值或初始化 */
      hsmbus->PreviousState = SMBUS_STATE_NONE;         /* 赋值或初始化 */
      hsmbus->Mode = HAL_SMBUS_MODE_NONE;               /* 赋值或初始化 */

#if (USE_HAL_SMBUS_REGISTER_CALLBACKS == 1)             /* 条件编译 */
      hsmbus->MasterRxCpltCallback(hsmbus);             /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_SMBUS_MasterRxCpltCallback(hsmbus);           /* HAL库函数调用 */
#endif /* USE_HAL_SMBUS_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Handle BTF flag for Master receiver
  * @param  hsmbus Pointer to a SMBUS_HandleTypeDef structure that contains
  *         the configuration information for SMBUS module
  * @retval HAL status
  */
static HAL_StatusTypeDef SMBUS_MasterReceive_BTF(SMBUS_HandleTypeDef *hsmbus)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  /* Declaration of temporary variables to prevent undefined behavior of volatile usage */
  uint32_t CurrentXferOptions = hsmbus->XferOptions;    /* 32位无符号变量/函数 */

  if (hsmbus->XferCount == 4U)                          /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable BUF interrupt, this help to treat correctly the last 2 bytes
       on BTF subroutine if there is a reception delay between N-1 and N byte */
    __HAL_SMBUS_DISABLE_IT(hsmbus, SMBUS_IT_BUF);       /* HAL辅助宏/函数 */

    /* Read data from DR */
    (*hsmbus->pBuffPtr++) = hsmbus->Instance->DR;       /* 赋值或初始化 */
    hsmbus->XferCount--;                                /* 执行语句 */
    hsmbus->XferPEC = SMBUS_GET_PEC(hsmbus);            /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if (hsmbus->XferCount == 3U)                     /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* Disable BUF interrupt, this help to treat correctly the last 2 bytes
       on BTF subroutine if there is a reception delay between N-1 and N byte */
    __HAL_SMBUS_DISABLE_IT(hsmbus, SMBUS_IT_BUF);       /* HAL辅助宏/函数 */

    /* Disable Acknowledge */
    CLEAR_BIT(hsmbus->Instance->CR1, I2C_CR1_ACK);      /* 执行语句 */

    /* Read data from DR */
    (*hsmbus->pBuffPtr++) = hsmbus->Instance->DR;       /* 赋值或初始化 */
    hsmbus->XferCount--;                                /* 执行语句 */
    hsmbus->XferPEC = SMBUS_GET_PEC(hsmbus);            /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if (hsmbus->XferCount == 2U)                     /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* Prepare next transfer or stop current transfer */
    if ((CurrentXferOptions == SMBUS_NEXT_FRAME) || (CurrentXferOptions == SMBUS_FIRST_FRAME))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable Acknowledge */
      CLEAR_BIT(hsmbus->Instance->CR1, I2C_CR1_ACK);    /* 执行语句 */

      /* Generate ReStart */
      SET_BIT(hsmbus->Instance->CR1, I2C_CR1_START);    /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Generate Stop */
      SET_BIT(hsmbus->Instance->CR1, I2C_CR1_STOP);     /* 执行语句 */
    }                                                   /* 代码块结束 */

    /* Read data from DR */
    (*hsmbus->pBuffPtr++) = hsmbus->Instance->DR;       /* 赋值或初始化 */
    hsmbus->XferCount--;                                /* 执行语句 */

    /* Read data from DR */
    (*hsmbus->pBuffPtr++) = hsmbus->Instance->DR;       /* 赋值或初始化 */
    hsmbus->XferCount--;                                /* 执行语句 */

    /* Disable EVT and ERR interrupt */
    __HAL_SMBUS_DISABLE_IT(hsmbus, SMBUS_IT_EVT | SMBUS_IT_ERR);  /* HAL辅助宏/函数 */

    hsmbus->State = HAL_SMBUS_STATE_READY;              /* 赋值或初始化 */
    hsmbus->PreviousState = SMBUS_STATE_NONE;           /* 赋值或初始化 */
    hsmbus->Mode = HAL_SMBUS_MODE_NONE;                 /* 赋值或初始化 */
#if (USE_HAL_SMBUS_REGISTER_CALLBACKS == 1)             /* 条件编译 */
    hsmbus->MasterRxCpltCallback(hsmbus);               /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_SMBUS_MasterRxCpltCallback(hsmbus);             /* HAL库函数调用 */
#endif /* USE_HAL_SMBUS_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Read data from DR */
    (*hsmbus->pBuffPtr++) = hsmbus->Instance->DR;       /* 赋值或初始化 */
    hsmbus->XferCount--;                                /* 执行语句 */
  }                                                     /* 代码块结束 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Handle SB flag for Master
  * @param  hsmbus Pointer to a SMBUS_HandleTypeDef structure that contains
  *         the configuration information for SMBUS module
  * @retval HAL status
  */
static HAL_StatusTypeDef SMBUS_Master_SB(SMBUS_HandleTypeDef *hsmbus)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  if (hsmbus->Init.AddressingMode == SMBUS_ADDRESSINGMODE_7BIT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Send slave 7 Bits address */
    if (hsmbus->State == HAL_SMBUS_STATE_BUSY_TX)       /* 条件判断 */
    {                                                   /* 代码块开始 */
      hsmbus->Instance->DR = SMBUS_7BIT_ADD_WRITE(hsmbus->Devaddress);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hsmbus->Instance->DR = SMBUS_7BIT_ADD_READ(hsmbus->Devaddress);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    if (hsmbus->EventCount == 0U)                       /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Send header of slave address */
      hsmbus->Instance->DR = SMBUS_10BIT_HEADER_WRITE(hsmbus->Devaddress);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if (hsmbus->EventCount == 1U)                  /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* Send header of slave address */
      hsmbus->Instance->DR = SMBUS_10BIT_HEADER_READ(hsmbus->Devaddress);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Handle ADD10 flag for Master
  * @param  hsmbus Pointer to a SMBUS_HandleTypeDef structure that contains
  *         the configuration information for SMBUS module
  * @retval HAL status
  */
static HAL_StatusTypeDef SMBUS_Master_ADD10(SMBUS_HandleTypeDef *hsmbus)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  /* Send slave address */
  hsmbus->Instance->DR = SMBUS_10BIT_ADDRESS(hsmbus->Devaddress);  /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Handle ADDR flag for Master
  * @param  hsmbus Pointer to a SMBUS_HandleTypeDef structure that contains
  *         the configuration information for SMBUS module
  * @retval HAL status
  */
static HAL_StatusTypeDef SMBUS_Master_ADDR(SMBUS_HandleTypeDef *hsmbus)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  /* Declaration of temporary variable to prevent undefined behavior of volatile usage */
  uint32_t Prev_State         = hsmbus->PreviousState;  /* 32位无符号变量/函数 */

  if (hsmbus->State == HAL_SMBUS_STATE_BUSY_RX)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((hsmbus->EventCount == 0U) && (hsmbus->Init.AddressingMode == SMBUS_ADDRESSINGMODE_10BIT))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear ADDR flag */
      __HAL_SMBUS_CLEAR_ADDRFLAG(hsmbus);               /* HAL辅助宏/函数 */

      /* Generate Restart */
      SET_BIT(hsmbus->Instance->CR1, I2C_CR1_START);    /* 执行语句 */

      hsmbus->EventCount++;                             /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /*  In the case of the Quick Command, the ADDR flag is cleared and a stop is generated */
      if (hsmbus->XferCount == 0U)                      /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Clear ADDR flag */
        __HAL_SMBUS_CLEAR_ADDRFLAG(hsmbus);             /* HAL辅助宏/函数 */

        /* Generate Stop */
        SET_BIT(hsmbus->Instance->CR1, I2C_CR1_STOP);   /* 执行语句 */
      }                                                 /* 代码块结束 */
      else if (hsmbus->XferCount == 1U)                 /* 否则如果 */
      {                                                 /* 代码块开始 */
        /* Prepare next transfer or stop current transfer */
        if ((hsmbus->XferOptions == SMBUS_FIRST_FRAME) && (Prev_State != SMBUS_STATE_MASTER_BUSY_RX))  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Disable Acknowledge */
          CLEAR_BIT(hsmbus->Instance->CR1, I2C_CR1_ACK);  /* 执行语句 */

          /* Clear ADDR flag */
          __HAL_SMBUS_CLEAR_ADDRFLAG(hsmbus);           /* HAL辅助宏/函数 */
        }                                               /* 代码块结束 */
        else if ((hsmbus->XferOptions == SMBUS_NEXT_FRAME) && (Prev_State != SMBUS_STATE_MASTER_BUSY_RX))  /* 否则如果 */
        {                                               /* 代码块开始 */
          /* Enable Acknowledge */
          SET_BIT(hsmbus->Instance->CR1, I2C_CR1_ACK);  /* 执行语句 */

          /* Clear ADDR flag */
          __HAL_SMBUS_CLEAR_ADDRFLAG(hsmbus);           /* HAL辅助宏/函数 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          /* Disable Acknowledge */
          CLEAR_BIT(hsmbus->Instance->CR1, I2C_CR1_ACK);  /* 执行语句 */

          /* Clear ADDR flag */
          __HAL_SMBUS_CLEAR_ADDRFLAG(hsmbus);           /* HAL辅助宏/函数 */

          /* Generate Stop */
          SET_BIT(hsmbus->Instance->CR1, I2C_CR1_STOP);  /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else if (hsmbus->XferCount == 2U)                 /* 否则如果 */
      {                                                 /* 代码块开始 */
        if (hsmbus->XferOptions != SMBUS_NEXT_FRAME)    /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Disable Acknowledge */
          CLEAR_BIT(hsmbus->Instance->CR1, I2C_CR1_ACK);  /* 执行语句 */

          /* Enable Pos */
          SET_BIT(hsmbus->Instance->CR1, I2C_CR1_POS);  /* 执行语句 */


        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          /* Enable Acknowledge */
          SET_BIT(hsmbus->Instance->CR1, I2C_CR1_ACK);  /* 执行语句 */
        }                                               /* 代码块结束 */

        /* Clear ADDR flag */
        __HAL_SMBUS_CLEAR_ADDRFLAG(hsmbus);             /* HAL辅助宏/函数 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Enable Acknowledge */
        SET_BIT(hsmbus->Instance->CR1, I2C_CR1_ACK);    /* 执行语句 */

        /* Clear ADDR flag */
        __HAL_SMBUS_CLEAR_ADDRFLAG(hsmbus);             /* HAL辅助宏/函数 */
      }                                                 /* 代码块结束 */

      /* Reset Event counter  */
      hsmbus->EventCount = 0U;                          /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Clear ADDR flag */
    __HAL_SMBUS_CLEAR_ADDRFLAG(hsmbus);                 /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Handle TXE flag for Slave
  * @param  hsmbus Pointer to a SMBUS_HandleTypeDef structure that contains
  *         the configuration information for SMBUS module
  * @retval HAL status
  */
static HAL_StatusTypeDef SMBUS_SlaveTransmit_TXE(SMBUS_HandleTypeDef *hsmbus)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  /* Declaration of temporary variables to prevent undefined behavior of volatile usage */
  uint32_t CurrentState = hsmbus->State;                /* 32位无符号变量/函数 */

  if (hsmbus->XferCount != 0U)                          /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Write data to DR */
    hsmbus->Instance->DR = (*hsmbus->pBuffPtr++);       /* 赋值或初始化 */
    hsmbus->XferCount--;                                /* 执行语句 */

    if ((hsmbus->XferCount == 2U) && (SMBUS_GET_PEC_MODE(hsmbus) == SMBUS_PEC_ENABLE) && ((hsmbus->XferOptions == SMBUS_FIRST_AND_LAST_FRAME_WITH_PEC) || (hsmbus->XferOptions == SMBUS_LAST_FRAME_WITH_PEC)))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hsmbus->XferCount--;                              /* 执行语句 */
    }                                                   /* 代码块结束 */

    if ((hsmbus->XferCount == 0U) && (CurrentState == (HAL_SMBUS_STATE_BUSY_TX_LISTEN)))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Last Byte is received, disable Interrupt */
      __HAL_SMBUS_DISABLE_IT(hsmbus, SMBUS_IT_BUF);     /* HAL辅助宏/函数 */

      /* Set state at HAL_SMBUS_STATE_LISTEN */
      hsmbus->PreviousState = SMBUS_STATE_SLAVE_BUSY_TX;  /* 赋值或初始化 */
      hsmbus->State = HAL_SMBUS_STATE_LISTEN;           /* 赋值或初始化 */

      /* Call the corresponding callback to inform upper layer of End of Transfer */
#if (USE_HAL_SMBUS_REGISTER_CALLBACKS == 1)             /* 条件编译 */
      hsmbus->SlaveTxCpltCallback(hsmbus);              /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_SMBUS_SlaveTxCpltCallback(hsmbus);            /* HAL库函数调用 */
#endif /* USE_HAL_SMBUS_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Handle BTF flag for Slave transmitter
  * @param  hsmbus Pointer to a SMBUS_HandleTypeDef structure that contains
  *         the configuration information for SMBUS module
  * @retval HAL status
  */
static HAL_StatusTypeDef SMBUS_SlaveTransmit_BTF(SMBUS_HandleTypeDef *hsmbus)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  if (hsmbus->XferCount != 0U)                          /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Write data to DR */
    hsmbus->Instance->DR = (*hsmbus->pBuffPtr++);       /* 赋值或初始化 */
    hsmbus->XferCount--;                                /* 执行语句 */
  }                                                     /* 代码块结束 */



  else if ((hsmbus->XferCount == 0U) && (SMBUS_GET_PEC_MODE(hsmbus) == SMBUS_PEC_ENABLE) && ((hsmbus->XferOptions == SMBUS_FIRST_AND_LAST_FRAME_WITH_PEC) || (hsmbus->XferOptions == SMBUS_LAST_FRAME_WITH_PEC)))  /* 否则如果 */
  {                                                     /* 代码块开始 */
    SET_BIT(hsmbus->Instance->CR1, I2C_CR1_PEC);        /* 执行语句 */
  }                                                     /* 代码块结束 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Handle RXNE flag for Slave
  * @param  hsmbus Pointer to a SMBUS_HandleTypeDef structure that contains
  *         the configuration information for SMBUS module
  * @retval HAL status
  */
static HAL_StatusTypeDef SMBUS_SlaveReceive_RXNE(SMBUS_HandleTypeDef *hsmbus)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  /* Declaration of temporary variables to prevent undefined behavior of volatile usage */
  uint32_t CurrentState = hsmbus->State;                /* 32位无符号变量/函数 */

  if (hsmbus->XferCount != 0U)                          /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Read data from DR */
    (*hsmbus->pBuffPtr++) = hsmbus->Instance->DR;       /* 赋值或初始化 */
    hsmbus->XferCount--;                                /* 执行语句 */

    if ((hsmbus->XferCount == 1U) && (SMBUS_GET_PEC_MODE(hsmbus) == SMBUS_PEC_ENABLE) && ((hsmbus->XferOptions == SMBUS_FIRST_AND_LAST_FRAME_WITH_PEC) || (hsmbus->XferOptions == SMBUS_LAST_FRAME_WITH_PEC)))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      SET_BIT(hsmbus->Instance->CR1, I2C_CR1_PEC);      /* 执行语句 */
      hsmbus->XferPEC = SMBUS_GET_PEC(hsmbus);          /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    if ((hsmbus->XferCount == 0U) && (CurrentState == HAL_SMBUS_STATE_BUSY_RX_LISTEN))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Last Byte is received, disable Interrupt */
      __HAL_SMBUS_DISABLE_IT(hsmbus, SMBUS_IT_BUF);     /* HAL辅助宏/函数 */

      /* Set state at HAL_SMBUS_STATE_LISTEN */
      hsmbus->PreviousState = SMBUS_STATE_SLAVE_BUSY_RX;  /* 赋值或初始化 */
      hsmbus->State = HAL_SMBUS_STATE_LISTEN;           /* 赋值或初始化 */

      /* Call the corresponding callback to inform upper layer of End of Transfer */
#if (USE_HAL_SMBUS_REGISTER_CALLBACKS == 1)             /* 条件编译 */
      hsmbus->SlaveRxCpltCallback(hsmbus);              /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_SMBUS_SlaveRxCpltCallback(hsmbus);            /* HAL库函数调用 */
#endif /* USE_HAL_SMBUS_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Handle BTF flag for Slave receiver
  * @param  hsmbus Pointer to a SMBUS_HandleTypeDef structure that contains
  *         the configuration information for SMBUS module
  * @retval HAL status
  */
static HAL_StatusTypeDef SMBUS_SlaveReceive_BTF(SMBUS_HandleTypeDef *hsmbus)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  if (hsmbus->XferCount != 0U)                          /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Read data from DR */
    (*hsmbus->pBuffPtr++) = hsmbus->Instance->DR;       /* 赋值或初始化 */
    hsmbus->XferCount--;                                /* 执行语句 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Handle ADD flag for Slave
  * @param  hsmbus Pointer to a SMBUS_HandleTypeDef structure that contains
  *         the configuration information for SMBUS module
  * @retval HAL status
  */
static HAL_StatusTypeDef SMBUS_Slave_ADDR(SMBUS_HandleTypeDef *hsmbus)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  uint8_t TransferDirection = SMBUS_DIRECTION_RECEIVE ;  /* 8位无符号变量/函数 */
  uint16_t SlaveAddrCode = 0U;                          /* 16位无符号变量/函数 */

  /* Transfer Direction requested by Master */
  if (__HAL_SMBUS_GET_FLAG(hsmbus, SMBUS_FLAG_TRA) == RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    TransferDirection = SMBUS_DIRECTION_TRANSMIT;       /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if (__HAL_SMBUS_GET_FLAG(hsmbus, SMBUS_FLAG_DUALF) == RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    SlaveAddrCode = hsmbus->Init.OwnAddress1;           /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    SlaveAddrCode = hsmbus->Init.OwnAddress2;           /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Call Slave Addr callback */
#if (USE_HAL_SMBUS_REGISTER_CALLBACKS == 1)             /* 条件编译 */
  hsmbus->AddrCallback(hsmbus, TransferDirection, SlaveAddrCode);  /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_SMBUS_AddrCallback(hsmbus, TransferDirection, SlaveAddrCode);  /* HAL库函数调用 */
#endif /* USE_HAL_SMBUS_REGISTER_CALLBACKS */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Handle STOPF flag for Slave
  * @param  hsmbus Pointer to a SMBUS_HandleTypeDef structure that contains
  *         the configuration information for SMBUS module
  * @retval HAL status
  */
static HAL_StatusTypeDef SMBUS_Slave_STOPF(SMBUS_HandleTypeDef *hsmbus)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  /* Declaration of temporary variable to prevent undefined behavior of volatile usage */
  uint32_t CurrentState = hsmbus->State;                /* 32位无符号变量/函数 */

  /* Disable EVT, BUF and ERR interrupt */
  __HAL_SMBUS_DISABLE_IT(hsmbus, SMBUS_IT_EVT | SMBUS_IT_BUF | SMBUS_IT_ERR);  /* HAL辅助宏/函数 */

  /* Clear STOPF flag */
  __HAL_SMBUS_CLEAR_STOPFLAG(hsmbus);                   /* HAL辅助宏/函数 */

  /* Disable Acknowledge */
  CLEAR_BIT(hsmbus->Instance->CR1, I2C_CR1_ACK);        /* 执行语句 */

  /* All data are not transferred, so set error code accordingly */
  if (hsmbus->XferCount != 0U)                          /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Store Last receive data if any */
    if (__HAL_SMBUS_GET_FLAG(hsmbus, SMBUS_FLAG_BTF) == SET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Read data from DR */
      (*hsmbus->pBuffPtr++) = hsmbus->Instance->DR;     /* 赋值或初始化 */

      if (hsmbus->XferCount > 0)                        /* 条件判断 */
      {                                                 /* 代码块开始 */
        hsmbus->XferSize--;                             /* 执行语句 */
        hsmbus->XferCount--;                            /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Store Last receive data if any */
    if (__HAL_SMBUS_GET_FLAG(hsmbus, SMBUS_FLAG_RXNE) == SET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Read data from DR */
      (*hsmbus->pBuffPtr++) = hsmbus->Instance->DR;     /* 赋值或初始化 */

      if (hsmbus->XferCount > 0)                        /* 条件判断 */
      {                                                 /* 代码块开始 */
        hsmbus->XferSize--;                             /* 执行语句 */
        hsmbus->XferCount--;                            /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  if (hsmbus->ErrorCode != HAL_SMBUS_ERROR_NONE)        /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Call the corresponding callback to inform upper layer of End of Transfer */
    SMBUS_ITError(hsmbus);                              /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    if ((CurrentState == HAL_SMBUS_STATE_LISTEN) || (CurrentState == HAL_SMBUS_STATE_BUSY_RX_LISTEN)  || \
        (CurrentState == HAL_SMBUS_STATE_BUSY_TX_LISTEN))  /* 源代码 */
    {                                                   /* 代码块开始 */
      hsmbus->XferOptions = SMBUS_NO_OPTION_FRAME;      /* 赋值或初始化 */
      hsmbus->PreviousState = HAL_SMBUS_STATE_READY;    /* 赋值或初始化 */
      hsmbus->State = HAL_SMBUS_STATE_READY;            /* 赋值或初始化 */
      hsmbus->Mode = HAL_SMBUS_MODE_NONE;               /* 赋值或初始化 */

#if (USE_HAL_SMBUS_REGISTER_CALLBACKS == 1)             /* 条件编译 */
      hsmbus->ListenCpltCallback(hsmbus);               /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_SMBUS_ListenCpltCallback(hsmbus);             /* HAL库函数调用 */
#endif /* USE_HAL_SMBUS_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @param  hsmbus Pointer to a SMBUS_HandleTypeDef structure that contains
  *         the configuration information for SMBUS module
  * @retval HAL status
  */
static HAL_StatusTypeDef SMBUS_Slave_AF(SMBUS_HandleTypeDef *hsmbus)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  /* Declaration of temporary variables to prevent undefined behavior of volatile usage */
  uint32_t CurrentState       = hsmbus->State;          /* 32位无符号变量/函数 */
  uint32_t CurrentXferOptions = hsmbus->XferOptions;    /* 32位无符号变量/函数 */

  if (((CurrentXferOptions ==  SMBUS_FIRST_AND_LAST_FRAME_NO_PEC) || (CurrentXferOptions ==  SMBUS_FIRST_AND_LAST_FRAME_WITH_PEC) || \
       (CurrentXferOptions == SMBUS_LAST_FRAME_NO_PEC) || (CurrentXferOptions ==  SMBUS_LAST_FRAME_WITH_PEC)) && \
      (CurrentState == HAL_SMBUS_STATE_LISTEN))         /* 源代码 */
  {                                                     /* 代码块开始 */
    hsmbus->XferOptions = SMBUS_NO_OPTION_FRAME;        /* 赋值或初始化 */

    /* Disable EVT, BUF and ERR interrupt */
    __HAL_SMBUS_DISABLE_IT(hsmbus, SMBUS_IT_EVT | SMBUS_IT_BUF | SMBUS_IT_ERR);  /* HAL辅助宏/函数 */

    /* Clear AF flag */
    __HAL_SMBUS_CLEAR_FLAG(hsmbus, SMBUS_FLAG_AF);      /* HAL辅助宏/函数 */

    /* Disable Acknowledge */
    CLEAR_BIT(hsmbus->Instance->CR1, I2C_CR1_ACK);      /* 执行语句 */

    hsmbus->PreviousState = HAL_SMBUS_STATE_READY;      /* 赋值或初始化 */
    hsmbus->State = HAL_SMBUS_STATE_READY;              /* 赋值或初始化 */
    hsmbus->Mode = HAL_SMBUS_MODE_NONE;                 /* 赋值或初始化 */

    /* Call the Listen Complete callback, to inform upper layer of the end of Listen usecase */
#if (USE_HAL_SMBUS_REGISTER_CALLBACKS == 1)             /* 条件编译 */
    hsmbus->ListenCpltCallback(hsmbus);                 /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_SMBUS_ListenCpltCallback(hsmbus);               /* HAL库函数调用 */
#endif /* USE_HAL_SMBUS_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */



/**
  * @brief SMBUS interrupts error process
  * @param  hsmbus SMBUS handle.
  * @retval None
  */
static void SMBUS_ITError(SMBUS_HandleTypeDef *hsmbus)  /* 静态函数 */
{                                                       /* 代码块开始 */
  /* Declaration of temporary variable to prevent undefined behavior of volatile usage */
  uint32_t CurrentState = hsmbus->State;                /* 32位无符号变量/函数 */

  if ((CurrentState == HAL_SMBUS_STATE_BUSY_TX_LISTEN) || (CurrentState == HAL_SMBUS_STATE_BUSY_RX_LISTEN))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* keep HAL_SMBUS_STATE_LISTEN */
    hsmbus->PreviousState = SMBUS_STATE_NONE;           /* 赋值或初始化 */
    hsmbus->State = HAL_SMBUS_STATE_LISTEN;             /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* If state is an abort treatment on going, don't change state */
    /* This change will be done later */
    if (hsmbus->State != HAL_SMBUS_STATE_ABORT)         /* 条件判断 */
    {                                                   /* 代码块开始 */
      hsmbus->State = HAL_SMBUS_STATE_READY;            /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    hsmbus->PreviousState = SMBUS_STATE_NONE;           /* 赋值或初始化 */
    hsmbus->Mode = HAL_SMBUS_MODE_NONE;                 /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Disable Pos bit in SMBUS CR1 when error occurred in Master/Mem Receive IT Process */
  CLEAR_BIT(hsmbus->Instance->CR1, I2C_CR1_POS);        /* 执行语句 */

  if (hsmbus->State == HAL_SMBUS_STATE_ABORT)           /* 条件判断 */
  {                                                     /* 代码块开始 */
    hsmbus->State = HAL_SMBUS_STATE_READY;              /* 赋值或初始化 */
    hsmbus->ErrorCode = HAL_SMBUS_ERROR_NONE;           /* 赋值或初始化 */

    /* Store Last receive data if any */
    if (__HAL_SMBUS_GET_FLAG(hsmbus, SMBUS_FLAG_RXNE) == SET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Read data from DR */
      (*hsmbus->pBuffPtr++) = hsmbus->Instance->DR;     /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Disable SMBUS peripheral to prevent dummy data in buffer */
    __HAL_SMBUS_DISABLE(hsmbus);                        /* HAL辅助宏/函数 */

    /* Call the corresponding callback to inform upper layer of End of Transfer */
#if (USE_HAL_SMBUS_REGISTER_CALLBACKS == 1)             /* 条件编译 */
    hsmbus->AbortCpltCallback(hsmbus);                  /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_SMBUS_AbortCpltCallback(hsmbus);                /* HAL库函数调用 */
#endif /* USE_HAL_SMBUS_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Store Last receive data if any */
    if (__HAL_SMBUS_GET_FLAG(hsmbus, SMBUS_FLAG_RXNE) == SET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Read data from DR */
      (*hsmbus->pBuffPtr++) = hsmbus->Instance->DR;     /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Call user error callback */
#if (USE_HAL_SMBUS_REGISTER_CALLBACKS == 1)             /* 条件编译 */
    hsmbus->ErrorCallback(hsmbus);                      /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_SMBUS_ErrorCallback(hsmbus);                    /* HAL库函数调用 */
#endif /* USE_HAL_SMBUS_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
  /* STOP Flag is not set after a NACK reception */
  /* So may inform upper layer that listen phase is stopped */
  /* during NACK error treatment */
  if ((hsmbus->State == HAL_SMBUS_STATE_LISTEN) && ((hsmbus->ErrorCode & HAL_SMBUS_ERROR_AF) == HAL_SMBUS_ERROR_AF))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hsmbus->XferOptions = SMBUS_NO_OPTION_FRAME;        /* 赋值或初始化 */
    hsmbus->PreviousState = SMBUS_STATE_NONE;           /* 赋值或初始化 */
    hsmbus->State = HAL_SMBUS_STATE_READY;              /* 赋值或初始化 */
    hsmbus->Mode = HAL_SMBUS_MODE_NONE;                 /* 赋值或初始化 */

    /* Call the Listen Complete callback, to inform upper layer of the end of Listen usecase */
#if (USE_HAL_SMBUS_REGISTER_CALLBACKS == 1)             /* 条件编译 */
    hsmbus->ListenCpltCallback(hsmbus);                 /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_SMBUS_ListenCpltCallback(hsmbus);               /* HAL库函数调用 */
#endif /* USE_HAL_SMBUS_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  This function handles SMBUS Communication Timeout.
  * @param  hsmbus Pointer to a SMBUS_HandleTypeDef structure that contains
  *         the configuration information for SMBUS module
  * @param  Flag specifies the SMBUS flag to check.
  * @param  Status The new Flag status (SET or RESET).
  * @param  Timeout Timeout duration
  * @param  Tickstart Tick start value
  * @retval HAL status
  */
static HAL_StatusTypeDef SMBUS_WaitOnFlagUntilTimeout(SMBUS_HandleTypeDef *hsmbus, uint32_t Flag, FlagStatus Status, uint32_t Timeout, uint32_t Tickstart)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  /* Wait until flag is set */
  if (Status == RESET)                                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    while (__HAL_SMBUS_GET_FLAG(hsmbus, Flag) == RESET)  /* while循环 */
    {                                                   /* 代码块开始 */
      /* Check for the Timeout */
      if (Timeout != HAL_MAX_DELAY)                     /* 条件判断 */
      {                                                 /* 代码块开始 */
        if ((Timeout == 0U) || ((HAL_GetTick() - Tickstart) > Timeout))  /* 条件判断 */
        {                                               /* 代码块开始 */
          hsmbus->PreviousState = SMBUS_STATE_NONE;     /* 赋值或初始化 */
          hsmbus->State = HAL_SMBUS_STATE_READY;        /* 赋值或初始化 */
          hsmbus->Mode = HAL_SMBUS_MODE_NONE;           /* 赋值或初始化 */

          /* Process Unlocked */
          __HAL_UNLOCK(hsmbus);                         /* HAL辅助宏/函数 */
          return HAL_TIMEOUT;                           /* 返回值 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */


#endif /* HAL_SMBUS_MODULE_ENABLED */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
