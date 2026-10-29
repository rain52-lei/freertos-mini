/* 文件: stm32f4xx_hal_i2c.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_i2c.c
  * @author  MCD Application Team
  * @brief   I2C HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Inter Integrated Circuit (I2C) peripheral:
  *           + Initialization and de-initialization functions
  *           + IO operation functions
  *           + Peripheral State, Mode and Error functions
  *
  @verbatim
  ==============================================================================
                        ##### How to use this driver #####
  ==============================================================================
  [..]
    The I2C HAL driver can be used as follows:

    (#) Declare a I2C_HandleTypeDef handle structure, for example:
        I2C_HandleTypeDef  hi2c;

    (#)Initialize the I2C low level resources by implementing the @ref HAL_I2C_MspInit() API:
        (##) Enable the I2Cx interface clock
        (##) I2C pins configuration
            (+++) Enable the clock for the I2C GPIOs
            (+++) Configure I2C pins as alternate function open-drain
        (##) NVIC configuration if you need to use interrupt process
            (+++) Configure the I2Cx interrupt priority
            (+++) Enable the NVIC I2C IRQ Channel
        (##) DMA Configuration if you need to use DMA process
            (+++) Declare a DMA_HandleTypeDef handle structure for the transmit or receive stream
            (+++) Enable the DMAx interface clock using
            (+++) Configure the DMA handle parameters
            (+++) Configure the DMA Tx or Rx stream
            (+++) Associate the initialized DMA handle to the hi2c DMA Tx or Rx handle
            (+++) Configure the priority and enable the NVIC for the transfer complete interrupt on
                  the DMA Tx or Rx stream

    (#) Configure the Communication Speed, Duty cycle, Addressing mode, Own Address1,
        Dual Addressing mode, Own Address2, General call and Nostretch mode in the hi2c Init structure.

    (#) Initialize the I2C registers by calling the @ref HAL_I2C_Init(), configures also the low level Hardware
        (GPIO, CLOCK, NVIC...etc) by calling the customized @ref HAL_I2C_MspInit() API.

    (#) To check if target device is ready for communication, use the function @ref HAL_I2C_IsDeviceReady()

    (#) For I2C IO and IO MEM operations, three operation modes are available within this driver :

    *** Polling mode IO operation ***
    =================================
    [..]
      (+) Transmit in master mode an amount of data in blocking mode using @ref HAL_I2C_Master_Transmit()
      (+) Receive in master mode an amount of data in blocking mode using @ref HAL_I2C_Master_Receive()
      (+) Transmit in slave mode an amount of data in blocking mode using @ref HAL_I2C_Slave_Transmit()
      (+) Receive in slave mode an amount of data in blocking mode using @ref HAL_I2C_Slave_Receive()

    *** Polling mode IO MEM operation ***
    =====================================
    [..]
      (+) Write an amount of data in blocking mode to a specific memory address using @ref HAL_I2C_Mem_Write()
      (+) Read an amount of data in blocking mode from a specific memory address using @ref HAL_I2C_Mem_Read()


    *** Interrupt mode IO operation ***
    ===================================
    [..]
      (+) Transmit in master mode an amount of data in non-blocking mode using @ref HAL_I2C_Master_Transmit_IT()
      (+) At transmission end of transfer, @ref HAL_I2C_MasterTxCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_I2C_MasterTxCpltCallback()
      (+) Receive in master mode an amount of data in non-blocking mode using @ref HAL_I2C_Master_Receive_IT()
      (+) At reception end of transfer, @ref HAL_I2C_MasterRxCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_I2C_MasterRxCpltCallback()
      (+) Transmit in slave mode an amount of data in non-blocking mode using @ref HAL_I2C_Slave_Transmit_IT()
      (+) At transmission end of transfer, @ref HAL_I2C_SlaveTxCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_I2C_SlaveTxCpltCallback()
      (+) Receive in slave mode an amount of data in non-blocking mode using @ref HAL_I2C_Slave_Receive_IT()
      (+) At reception end of transfer, @ref HAL_I2C_SlaveRxCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_I2C_SlaveRxCpltCallback()
      (+) In case of transfer Error, @ref HAL_I2C_ErrorCallback() function is executed and user can
           add his own code by customization of function pointer @ref HAL_I2C_ErrorCallback()
      (+) Abort a master I2C process communication with Interrupt using @ref HAL_I2C_Master_Abort_IT()
      (+) End of abort process, @ref HAL_I2C_AbortCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_I2C_AbortCpltCallback()

    *** Interrupt mode or DMA mode IO sequential operation ***
    ==========================================================
    [..]
      (@) These interfaces allow to manage a sequential transfer with a repeated start condition
          when a direction change during transfer
    [..]
      (+) A specific option field manage the different steps of a sequential transfer
      (+) Option field values are defined through @ref I2C_XferOptions_definition and are listed below:
      (++) I2C_FIRST_AND_LAST_FRAME: No sequential usage, functional is same as associated interfaces in no sequential mode
      (++) I2C_FIRST_FRAME: Sequential usage, this option allow to manage a sequence with start condition, address
                            and data to transfer without a final stop condition
      (++) I2C_FIRST_AND_NEXT_FRAME: Sequential usage (Master only), this option allow to manage a sequence with start condition, address
                            and data to transfer without a final stop condition, an then permit a call the same master sequential interface
                            several times (like @ref HAL_I2C_Master_Seq_Transmit_IT() then @ref HAL_I2C_Master_Seq_Transmit_IT()
                            or @ref HAL_I2C_Master_Seq_Transmit_DMA() then @ref HAL_I2C_Master_Seq_Transmit_DMA())
      (++) I2C_NEXT_FRAME: Sequential usage, this option allow to manage a sequence with a restart condition, address
                            and with new data to transfer if the direction change or manage only the new data to transfer
                            if no direction change and without a final stop condition in both cases
      (++) I2C_LAST_FRAME: Sequential usage, this option allow to manage a sequance with a restart condition, address
                            and with new data to transfer if the direction change or manage only the new data to transfer
                            if no direction change and with a final stop condition in both cases
      (++) I2C_LAST_FRAME_NO_STOP: Sequential usage (Master only), this option allow to manage a restart condition after several call of the same master sequential
                            interface several times (link with option I2C_FIRST_AND_NEXT_FRAME).
                            Usage can, transfer several bytes one by one using HAL_I2C_Master_Seq_Transmit_IT(option I2C_FIRST_AND_NEXT_FRAME then I2C_NEXT_FRAME)
                              or HAL_I2C_Master_Seq_Receive_IT(option I2C_FIRST_AND_NEXT_FRAME then I2C_NEXT_FRAME)
                              or HAL_I2C_Master_Seq_Transmit_DMA(option I2C_FIRST_AND_NEXT_FRAME then I2C_NEXT_FRAME)
                              or HAL_I2C_Master_Seq_Receive_DMA(option I2C_FIRST_AND_NEXT_FRAME then I2C_NEXT_FRAME).
                            Then usage of this option I2C_LAST_FRAME_NO_STOP at the last Transmit or Receive sequence permit to call the opposite interface Receive or Transmit
                              without stopping the communication and so generate a restart condition.
      (++) I2C_OTHER_FRAME: Sequential usage (Master only), this option allow to manage a restart condition after each call of the same master sequential
                            interface.
                            Usage can, transfer several bytes one by one with a restart with slave address between each bytes using HAL_I2C_Master_Seq_Transmit_IT(option I2C_FIRST_FRAME then I2C_OTHER_FRAME)
                              or HAL_I2C_Master_Seq_Receive_IT(option I2C_FIRST_FRAME then I2C_OTHER_FRAME)
                              or HAL_I2C_Master_Seq_Transmit_DMA(option I2C_FIRST_FRAME then I2C_OTHER_FRAME)
                              or HAL_I2C_Master_Seq_Receive_DMA(option I2C_FIRST_FRAME then I2C_OTHER_FRAME).
                            Then usage of this option I2C_OTHER_AND_LAST_FRAME at the last frame to help automatic generation of STOP condition.

      (+) Different sequential I2C interfaces are listed below:
      (++) Sequential transmit in master I2C mode an amount of data in non-blocking mode using @ref HAL_I2C_Master_Seq_Transmit_IT()
            or using @ref HAL_I2C_Master_Seq_Transmit_DMA()
      (+++) At transmission end of current frame transfer, @ref HAL_I2C_MasterTxCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_I2C_MasterTxCpltCallback()
      (++) Sequential receive in master I2C mode an amount of data in non-blocking mode using @ref HAL_I2C_Master_Seq_Receive_IT()
            or using @ref HAL_I2C_Master_Seq_Receive_DMA()
      (+++) At reception end of current frame transfer, @ref HAL_I2C_MasterRxCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_I2C_MasterRxCpltCallback()
      (++) Abort a master IT or DMA I2C process communication with Interrupt using @ref HAL_I2C_Master_Abort_IT()
      (+++) End of abort process, @ref HAL_I2C_AbortCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_I2C_AbortCpltCallback()
      (++) Enable/disable the Address listen mode in slave I2C mode using @ref HAL_I2C_EnableListen_IT() @ref HAL_I2C_DisableListen_IT()
      (+++) When address slave I2C match, @ref HAL_I2C_AddrCallback() is executed and user can
           add his own code to check the Address Match Code and the transmission direction request by master (Write/Read).
      (+++) At Listen mode end @ref HAL_I2C_ListenCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_I2C_ListenCpltCallback()
      (++) Sequential transmit in slave I2C mode an amount of data in non-blocking mode using @ref HAL_I2C_Slave_Seq_Transmit_IT()
            or using @ref HAL_I2C_Slave_Seq_Transmit_DMA()
      (+++) At transmission end of current frame transfer, @ref HAL_I2C_SlaveTxCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_I2C_SlaveTxCpltCallback()
      (++) Sequential receive in slave I2C mode an amount of data in non-blocking mode using @ref HAL_I2C_Slave_Seq_Receive_IT()
            or using @ref HAL_I2C_Slave_Seq_Receive_DMA()
      (+++) At reception end of current frame transfer, @ref HAL_I2C_SlaveRxCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_I2C_SlaveRxCpltCallback()
      (++) In case of transfer Error, @ref HAL_I2C_ErrorCallback() function is executed and user can
           add his own code by customization of function pointer @ref HAL_I2C_ErrorCallback()

    *** Interrupt mode IO MEM operation ***
    =======================================
    [..]
      (+) Write an amount of data in non-blocking mode with Interrupt to a specific memory address using
          @ref HAL_I2C_Mem_Write_IT()
      (+) At Memory end of write transfer, @ref HAL_I2C_MemTxCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_I2C_MemTxCpltCallback()
      (+) Read an amount of data in non-blocking mode with Interrupt from a specific memory address using
          @ref HAL_I2C_Mem_Read_IT()
      (+) At Memory end of read transfer, @ref HAL_I2C_MemRxCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_I2C_MemRxCpltCallback()
      (+) In case of transfer Error, @ref HAL_I2C_ErrorCallback() function is executed and user can
           add his own code by customization of function pointer @ref HAL_I2C_ErrorCallback()

    *** DMA mode IO operation ***
    ==============================
    [..]
      (+) Transmit in master mode an amount of data in non-blocking mode (DMA) using
          @ref HAL_I2C_Master_Transmit_DMA()
      (+) At transmission end of transfer, @ref HAL_I2C_MasterTxCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_I2C_MasterTxCpltCallback()
      (+) Receive in master mode an amount of data in non-blocking mode (DMA) using
          @ref HAL_I2C_Master_Receive_DMA()
      (+) At reception end of transfer, @ref HAL_I2C_MasterRxCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_I2C_MasterRxCpltCallback()
      (+) Transmit in slave mode an amount of data in non-blocking mode (DMA) using
          @ref HAL_I2C_Slave_Transmit_DMA()
      (+) At transmission end of transfer, @ref HAL_I2C_SlaveTxCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_I2C_SlaveTxCpltCallback()
      (+) Receive in slave mode an amount of data in non-blocking mode (DMA) using
          @ref HAL_I2C_Slave_Receive_DMA()
      (+) At reception end of transfer, @ref HAL_I2C_SlaveRxCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_I2C_SlaveRxCpltCallback()
      (+) In case of transfer Error, @ref HAL_I2C_ErrorCallback() function is executed and user can
           add his own code by customization of function pointer @ref HAL_I2C_ErrorCallback()
      (+) Abort a master I2C process communication with Interrupt using @ref HAL_I2C_Master_Abort_IT()
      (+) End of abort process, @ref HAL_I2C_AbortCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_I2C_AbortCpltCallback()

    *** DMA mode IO MEM operation ***
    =================================
    [..]
      (+) Write an amount of data in non-blocking mode with DMA to a specific memory address using
          @ref HAL_I2C_Mem_Write_DMA()
      (+) At Memory end of write transfer, @ref HAL_I2C_MemTxCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_I2C_MemTxCpltCallback()
      (+) Read an amount of data in non-blocking mode with DMA from a specific memory address using
          @ref HAL_I2C_Mem_Read_DMA()
      (+) At Memory end of read transfer, @ref HAL_I2C_MemRxCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_I2C_MemRxCpltCallback()
      (+) In case of transfer Error, @ref HAL_I2C_ErrorCallback() function is executed and user can
           add his own code by customization of function pointer @ref HAL_I2C_ErrorCallback()


     *** I2C HAL driver macros list ***
     ==================================
     [..]
       Below the list of most used macros in I2C HAL driver.

      (+) @ref __HAL_I2C_ENABLE:     Enable the I2C peripheral
      (+) @ref __HAL_I2C_DISABLE:    Disable the I2C peripheral
      (+) @ref __HAL_I2C_GET_FLAG:   Checks whether the specified I2C flag is set or not
      (+) @ref __HAL_I2C_CLEAR_FLAG: Clear the specified I2C pending flag
      (+) @ref __HAL_I2C_ENABLE_IT:  Enable the specified I2C interrupt
      (+) @ref __HAL_I2C_DISABLE_IT: Disable the specified I2C interrupt

     *** Callback registration ***
     =============================================
    [..]
     The compilation flag USE_HAL_I2C_REGISTER_CALLBACKS when set to 1
     allows the user to configure dynamically the driver callbacks.
     Use Functions @ref HAL_I2C_RegisterCallback() or @ref HAL_I2C_RegisterAddrCallback()
     to register an interrupt callback.
    [..]
     Function @ref HAL_I2C_RegisterCallback() allows to register following callbacks:
       (+) MasterTxCpltCallback : callback for Master transmission end of transfer.
       (+) MasterRxCpltCallback : callback for Master reception end of transfer.
       (+) SlaveTxCpltCallback  : callback for Slave transmission end of transfer.
       (+) SlaveRxCpltCallback  : callback for Slave reception end of transfer.
       (+) ListenCpltCallback   : callback for end of listen mode.
       (+) MemTxCpltCallback    : callback for Memory transmission end of transfer.
       (+) MemRxCpltCallback    : callback for Memory reception end of transfer.
       (+) ErrorCallback        : callback for error detection.
       (+) AbortCpltCallback    : callback for abort completion process.
       (+) MspInitCallback      : callback for Msp Init.
       (+) MspDeInitCallback    : callback for Msp DeInit.
     This function takes as parameters the HAL peripheral handle, the Callback ID
     and a pointer to the user callback function.
    [..]
     For specific callback AddrCallback use dedicated register callbacks : @ref HAL_I2C_RegisterAddrCallback().
    [..]
     Use function @ref HAL_I2C_UnRegisterCallback to reset a callback to the default
     weak function.
     @ref HAL_I2C_UnRegisterCallback takes as parameters the HAL peripheral handle,
     and the Callback ID.
     This function allows to reset following callbacks:
       (+) MasterTxCpltCallback : callback for Master transmission end of transfer.
       (+) MasterRxCpltCallback : callback for Master reception end of transfer.
       (+) SlaveTxCpltCallback  : callback for Slave transmission end of transfer.
       (+) SlaveRxCpltCallback  : callback for Slave reception end of transfer.
       (+) ListenCpltCallback   : callback for end of listen mode.
       (+) MemTxCpltCallback    : callback for Memory transmission end of transfer.
       (+) MemRxCpltCallback    : callback for Memory reception end of transfer.
       (+) ErrorCallback        : callback for error detection.
       (+) AbortCpltCallback    : callback for abort completion process.
       (+) MspInitCallback      : callback for Msp Init.
       (+) MspDeInitCallback    : callback for Msp DeInit.
    [..]
     For callback AddrCallback use dedicated register callbacks : @ref HAL_I2C_UnRegisterAddrCallback().
    [..]
     By default, after the @ref HAL_I2C_Init() and when the state is @ref HAL_I2C_STATE_RESET
     all callbacks are set to the corresponding weak functions:
     examples @ref HAL_I2C_MasterTxCpltCallback(), @ref HAL_I2C_MasterRxCpltCallback().
     Exception done for MspInit and MspDeInit functions that are
     reset to the legacy weak functions in the @ref HAL_I2C_Init()/ @ref HAL_I2C_DeInit() only when
     these callbacks are null (not registered beforehand).
     If MspInit or MspDeInit are not null, the @ref HAL_I2C_Init()/ @ref HAL_I2C_DeInit()
     keep and use the user MspInit/MspDeInit callbacks (registered beforehand) whatever the state.
    [..]
     Callbacks can be registered/unregistered in @ref HAL_I2C_STATE_READY state only.
     Exception done MspInit/MspDeInit functions that can be registered/unregistered
     in @ref HAL_I2C_STATE_READY or @ref HAL_I2C_STATE_RESET state,
     thus registered (user) MspInit/DeInit callbacks can be used during the Init/DeInit.
     Then, the user first registers the MspInit/MspDeInit user callbacks
     using @ref HAL_I2C_RegisterCallback() before calling @ref HAL_I2C_DeInit()
     or @ref HAL_I2C_Init() function.
    [..]
     When the compilation flag USE_HAL_I2C_REGISTER_CALLBACKS is set to 0 or
     not defined, the callback registration feature is not available and all callbacks
     are set to the corresponding weak functions.



     [..]
       (@) You can refer to the I2C HAL driver header file for more useful macros

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

/** @defgroup I2C I2C
  * @brief I2C HAL module driver
  * @{
  */

#ifdef HAL_I2C_MODULE_ENABLED                           /* 条件编译 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @addtogroup I2C_Private_Define
  * @{
  */
#define I2C_TIMEOUT_FLAG          35U         /*!< Timeout 35 ms             */
#define I2C_TIMEOUT_BUSY_FLAG     25U         /*!< Timeout 25 ms             */
#define I2C_TIMEOUT_STOP_FLAG     5U          /*!< Timeout 5 ms              */
#define I2C_NO_OPTION_FRAME       0xFFFF0000U /*!< XferOptions default value */

/* Private define for @ref PreviousState usage */
#define I2C_STATE_MSK             ((uint32_t)((uint32_t)((uint32_t)HAL_I2C_STATE_BUSY_TX | (uint32_t)HAL_I2C_STATE_BUSY_RX) & (uint32_t)(~((uint32_t)HAL_I2C_STATE_READY)))) /*!< Mask State define, keep only RX and TX bits            */
#define I2C_STATE_NONE            ((uint32_t)(HAL_I2C_MODE_NONE))                                                        /*!< Default Value                                          */
#define I2C_STATE_MASTER_BUSY_TX  ((uint32_t)(((uint32_t)HAL_I2C_STATE_BUSY_TX & I2C_STATE_MSK) | (uint32_t)HAL_I2C_MODE_MASTER))            /*!< Master Busy TX, combinaison of State LSB and Mode enum */
#define I2C_STATE_MASTER_BUSY_RX  ((uint32_t)(((uint32_t)HAL_I2C_STATE_BUSY_RX & I2C_STATE_MSK) | (uint32_t)HAL_I2C_MODE_MASTER))            /*!< Master Busy RX, combinaison of State LSB and Mode enum */
#define I2C_STATE_SLAVE_BUSY_TX   ((uint32_t)(((uint32_t)HAL_I2C_STATE_BUSY_TX & I2C_STATE_MSK) | (uint32_t)HAL_I2C_MODE_SLAVE))             /*!< Slave Busy TX, combinaison of State LSB and Mode enum  */
#define I2C_STATE_SLAVE_BUSY_RX   ((uint32_t)(((uint32_t)HAL_I2C_STATE_BUSY_RX & I2C_STATE_MSK) | (uint32_t)HAL_I2C_MODE_SLAVE))             /*!< Slave Busy RX, combinaison of State LSB and Mode enum  */

/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/** @defgroup I2C_Private_Functions I2C Private Functions
  * @{
  */
/* Private functions to handle DMA transfer */
static void I2C_DMAXferCplt(DMA_HandleTypeDef *hdma);   /* 静态函数 */
static void I2C_DMAError(DMA_HandleTypeDef *hdma);      /* 静态函数 */
static void I2C_DMAAbort(DMA_HandleTypeDef *hdma);      /* 静态函数 */

static void I2C_ITError(I2C_HandleTypeDef *hi2c);       /* 静态函数 */

static HAL_StatusTypeDef I2C_MasterRequestWrite(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint32_t Timeout, uint32_t Tickstart);  /* 静态变量/函数 */
static HAL_StatusTypeDef I2C_MasterRequestRead(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint32_t Timeout, uint32_t Tickstart);  /* 静态变量/函数 */
static HAL_StatusTypeDef I2C_RequestMemoryWrite(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint32_t Timeout, uint32_t Tickstart);  /* 静态变量/函数 */
static HAL_StatusTypeDef I2C_RequestMemoryRead(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint32_t Timeout, uint32_t Tickstart);  /* 静态变量/函数 */

/* Private functions to handle flags during polling transfer */
static HAL_StatusTypeDef I2C_WaitOnFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Flag, FlagStatus Status, uint32_t Timeout, uint32_t Tickstart);  /* 静态变量/函数 */
static HAL_StatusTypeDef I2C_WaitOnMasterAddressFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Flag, uint32_t Timeout, uint32_t Tickstart);  /* 静态变量/函数 */
static HAL_StatusTypeDef I2C_WaitOnTXEFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Timeout, uint32_t Tickstart);  /* 静态变量/函数 */
static HAL_StatusTypeDef I2C_WaitOnBTFFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Timeout, uint32_t Tickstart);  /* 静态变量/函数 */
static HAL_StatusTypeDef I2C_WaitOnRXNEFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Timeout, uint32_t Tickstart);  /* 静态变量/函数 */
static HAL_StatusTypeDef I2C_WaitOnSTOPFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Timeout, uint32_t Tickstart);  /* 静态变量/函数 */
static HAL_StatusTypeDef I2C_WaitOnSTOPRequestThroughIT(I2C_HandleTypeDef *hi2c);  /* 静态变量/函数 */
static HAL_StatusTypeDef I2C_IsAcknowledgeFailed(I2C_HandleTypeDef *hi2c);  /* 静态变量/函数 */

/* Private functions for I2C transfer IRQ handler */
static void I2C_MasterTransmit_TXE(I2C_HandleTypeDef *hi2c);  /* 静态函数 */
static void I2C_MasterTransmit_BTF(I2C_HandleTypeDef *hi2c);  /* 静态函数 */
static void I2C_MasterReceive_RXNE(I2C_HandleTypeDef *hi2c);  /* 静态函数 */
static void I2C_MasterReceive_BTF(I2C_HandleTypeDef *hi2c);  /* 静态函数 */
static void I2C_Master_SB(I2C_HandleTypeDef *hi2c);     /* 静态函数 */
static void I2C_Master_ADD10(I2C_HandleTypeDef *hi2c);  /* 静态函数 */
static void I2C_Master_ADDR(I2C_HandleTypeDef *hi2c);   /* 静态函数 */

static void I2C_SlaveTransmit_TXE(I2C_HandleTypeDef *hi2c);  /* 静态函数 */
static void I2C_SlaveTransmit_BTF(I2C_HandleTypeDef *hi2c);  /* 静态函数 */
static void I2C_SlaveReceive_RXNE(I2C_HandleTypeDef *hi2c);  /* 静态函数 */
static void I2C_SlaveReceive_BTF(I2C_HandleTypeDef *hi2c);  /* 静态函数 */
static void I2C_Slave_ADDR(I2C_HandleTypeDef *hi2c, uint32_t IT2Flags);  /* 静态函数 */
static void I2C_Slave_STOPF(I2C_HandleTypeDef *hi2c);   /* 静态函数 */
static void I2C_Slave_AF(I2C_HandleTypeDef *hi2c);      /* 静态函数 */

static void I2C_MemoryTransmit_TXE_BTF(I2C_HandleTypeDef *hi2c);  /* 静态函数 */

/* Private function to Convert Specific options */
static void I2C_ConvertOtherXferOptions(I2C_HandleTypeDef *hi2c);  /* 静态函数 */
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/

/** @defgroup I2C_Exported_Functions I2C Exported Functions
  * @{
  */

/** @defgroup I2C_Exported_Functions_Group1 Initialization and de-initialization functions
 *  @brief    Initialization and Configuration functions
 *
@verbatim
 ===============================================================================
              ##### Initialization and de-initialization functions #####
 ===============================================================================
    [..]  This subsection provides a set of functions allowing to initialize and
          deinitialize the I2Cx peripheral:

      (+) User must Implement HAL_I2C_MspInit() function in which he configures
          all related peripherals resources (CLOCK, GPIO, DMA, IT and NVIC).

      (+) Call the function HAL_I2C_Init() to configure the selected device with
          the selected configuration:
        (++) Communication Speed
        (++) Duty cycle
        (++) Addressing mode
        (++) Own Address 1
        (++) Dual Addressing mode
        (++) Own Address 2
        (++) General call mode
        (++) Nostretch mode

      (+) Call the function HAL_I2C_DeInit() to restore the default configuration
          of the selected I2Cx peripheral.

@endverbatim
  * @{
  */

/**
  * @brief  Initializes the I2C according to the specified parameters
  *         in the I2C_InitTypeDef and initialize the associated handle.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Init(I2C_HandleTypeDef *hi2c)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t freqrange;                                   /* 32位无符号变量/函数 */
  uint32_t pclk1;                                       /* 32位无符号变量/函数 */

  /* Check the I2C handle allocation */
  if (hi2c == NULL)                                     /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the parameters */
  assert_param(IS_I2C_ALL_INSTANCE(hi2c->Instance));    /* 执行语句 */
  assert_param(IS_I2C_CLOCK_SPEED(hi2c->Init.ClockSpeed));  /* 执行语句 */
  assert_param(IS_I2C_DUTY_CYCLE(hi2c->Init.DutyCycle));  /* 执行语句 */
  assert_param(IS_I2C_OWN_ADDRESS1(hi2c->Init.OwnAddress1));  /* 执行语句 */
  assert_param(IS_I2C_ADDRESSING_MODE(hi2c->Init.AddressingMode));  /* 执行语句 */
  assert_param(IS_I2C_DUAL_ADDRESS(hi2c->Init.DualAddressMode));  /* 执行语句 */
  assert_param(IS_I2C_OWN_ADDRESS2(hi2c->Init.OwnAddress2));  /* 执行语句 */
  assert_param(IS_I2C_GENERAL_CALL(hi2c->Init.GeneralCallMode));  /* 执行语句 */
  assert_param(IS_I2C_NO_STRETCH(hi2c->Init.NoStretchMode));  /* 执行语句 */

  if (hi2c->State == HAL_I2C_STATE_RESET)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Allocate lock resource and initialize it */
    hi2c->Lock = HAL_UNLOCKED;                          /* 赋值或初始化 */

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)               /* 条件编译 */
    /* Init the I2C Callback settings */
    hi2c->MasterTxCpltCallback = HAL_I2C_MasterTxCpltCallback; /* Legacy weak MasterTxCpltCallback */
    hi2c->MasterRxCpltCallback = HAL_I2C_MasterRxCpltCallback; /* Legacy weak MasterRxCpltCallback */
    hi2c->SlaveTxCpltCallback  = HAL_I2C_SlaveTxCpltCallback;  /* Legacy weak SlaveTxCpltCallback  */
    hi2c->SlaveRxCpltCallback  = HAL_I2C_SlaveRxCpltCallback;  /* Legacy weak SlaveRxCpltCallback  */
    hi2c->ListenCpltCallback   = HAL_I2C_ListenCpltCallback;   /* Legacy weak ListenCpltCallback   */
    hi2c->MemTxCpltCallback    = HAL_I2C_MemTxCpltCallback;    /* Legacy weak MemTxCpltCallback    */
    hi2c->MemRxCpltCallback    = HAL_I2C_MemRxCpltCallback;    /* Legacy weak MemRxCpltCallback    */
    hi2c->ErrorCallback        = HAL_I2C_ErrorCallback;        /* Legacy weak ErrorCallback        */
    hi2c->AbortCpltCallback    = HAL_I2C_AbortCpltCallback;    /* Legacy weak AbortCpltCallback    */
    hi2c->AddrCallback         = HAL_I2C_AddrCallback;         /* Legacy weak AddrCallback         */

    if (hi2c->MspInitCallback == NULL)                  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hi2c->MspInitCallback = HAL_I2C_MspInit; /* Legacy weak MspInit  */
    }                                                   /* 代码块结束 */

    /* Init the low level hardware : GPIO, CLOCK, NVIC */
    hi2c->MspInitCallback(hi2c);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /* Init the low level hardware : GPIO, CLOCK, NVIC */
    HAL_I2C_MspInit(hi2c);                              /* HAL I2C操作 */
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */

  hi2c->State = HAL_I2C_STATE_BUSY;                     /* 赋值或初始化 */

  /* Disable the selected I2C peripheral */
  __HAL_I2C_DISABLE(hi2c);                              /* HAL辅助宏/函数 */

  /*Reset I2C*/
  hi2c->Instance->CR1 |= I2C_CR1_SWRST;                 /* 赋值或初始化 */
  hi2c->Instance->CR1 &= ~I2C_CR1_SWRST;                /* 赋值或初始化 */

  /* Get PCLK1 frequency */
  pclk1 = HAL_RCC_GetPCLK1Freq();                       /* 赋值或初始化 */

  /* Check the minimum allowed PCLK1 frequency */
  if (I2C_MIN_PCLK_FREQ(pclk1, hi2c->Init.ClockSpeed) == 1U)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Calculate frequency range */
  freqrange = I2C_FREQRANGE(pclk1);                     /* 赋值或初始化 */

  /*---------------------------- I2Cx CR2 Configuration ----------------------*/
  /* Configure I2Cx: Frequency range */
  MODIFY_REG(hi2c->Instance->CR2, I2C_CR2_FREQ, freqrange);  /* 执行语句 */

  /*---------------------------- I2Cx TRISE Configuration --------------------*/
  /* Configure I2Cx: Rise Time */
  MODIFY_REG(hi2c->Instance->TRISE, I2C_TRISE_TRISE, I2C_RISE_TIME(freqrange, hi2c->Init.ClockSpeed));  /* 执行语句 */

  /*---------------------------- I2Cx CCR Configuration ----------------------*/
  /* Configure I2Cx: Speed */
  MODIFY_REG(hi2c->Instance->CCR, (I2C_CCR_FS | I2C_CCR_DUTY | I2C_CCR_CCR), I2C_SPEED(pclk1, hi2c->Init.ClockSpeed, hi2c->Init.DutyCycle));  /* 执行语句 */

  /*---------------------------- I2Cx CR1 Configuration ----------------------*/
  /* Configure I2Cx: Generalcall and NoStretch mode */
  MODIFY_REG(hi2c->Instance->CR1, (I2C_CR1_ENGC | I2C_CR1_NOSTRETCH), (hi2c->Init.GeneralCallMode | hi2c->Init.NoStretchMode));  /* 执行语句 */

  /*---------------------------- I2Cx OAR1 Configuration ---------------------*/
  /* Configure I2Cx: Own Address1 and addressing mode */
  MODIFY_REG(hi2c->Instance->OAR1, (I2C_OAR1_ADDMODE | I2C_OAR1_ADD8_9 | I2C_OAR1_ADD1_7 | I2C_OAR1_ADD0), (hi2c->Init.AddressingMode | hi2c->Init.OwnAddress1));  /* 执行语句 */

  /*---------------------------- I2Cx OAR2 Configuration ---------------------*/
  /* Configure I2Cx: Dual mode and Own Address2 */
  MODIFY_REG(hi2c->Instance->OAR2, (I2C_OAR2_ENDUAL | I2C_OAR2_ADD2), (hi2c->Init.DualAddressMode | hi2c->Init.OwnAddress2));  /* 执行语句 */

  /* Enable the selected I2C peripheral */
  __HAL_I2C_ENABLE(hi2c);                               /* HAL辅助宏/函数 */

  hi2c->ErrorCode = HAL_I2C_ERROR_NONE;                 /* 赋值或初始化 */
  hi2c->State = HAL_I2C_STATE_READY;                    /* 赋值或初始化 */
  hi2c->PreviousState = I2C_STATE_NONE;                 /* 赋值或初始化 */
  hi2c->Mode = HAL_I2C_MODE_NONE;                       /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  DeInitialize the I2C peripheral.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for the specified I2C.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_DeInit(I2C_HandleTypeDef *hi2c)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the I2C handle allocation */
  if (hi2c == NULL)                                     /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the parameters */
  assert_param(IS_I2C_ALL_INSTANCE(hi2c->Instance));    /* 执行语句 */

  hi2c->State = HAL_I2C_STATE_BUSY;                     /* 赋值或初始化 */

  /* Disable the I2C Peripheral Clock */
  __HAL_I2C_DISABLE(hi2c);                              /* HAL辅助宏/函数 */

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)               /* 条件编译 */
  if (hi2c->MspDeInitCallback == NULL)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hi2c->MspDeInitCallback = HAL_I2C_MspDeInit; /* Legacy weak MspDeInit  */
  }                                                     /* 代码块结束 */

  /* DeInit the low level hardware: GPIO, CLOCK, NVIC */
  hi2c->MspDeInitCallback(hi2c);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* DeInit the low level hardware: GPIO, CLOCK, NVIC */
  HAL_I2C_MspDeInit(hi2c);                              /* HAL I2C操作 */
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */

  hi2c->ErrorCode     = HAL_I2C_ERROR_NONE;             /* 赋值或初始化 */
  hi2c->State         = HAL_I2C_STATE_RESET;            /* 赋值或初始化 */
  hi2c->PreviousState = I2C_STATE_NONE;                 /* 赋值或初始化 */
  hi2c->Mode          = HAL_I2C_MODE_NONE;              /* 赋值或初始化 */

  /* Release Lock */
  __HAL_UNLOCK(hi2c);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initialize the I2C MSP.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for the specified I2C.
  * @retval None
  */
__weak void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)    /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hi2c);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_I2C_MspInit could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  DeInitialize the I2C MSP.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for the specified I2C.
  * @retval None
  */
__weak void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hi2c);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_I2C_MspDeInit could be implemented in the user file
   */
}                                                       /* 代码块结束 */

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)               /* 条件编译 */
/**
  * @brief  Register a User I2C Callback
  *         To be used instead of the weak predefined callback
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  CallbackID ID of the callback to be registered
  *         This parameter can be one of the following values:
  *          @arg @ref HAL_I2C_MASTER_TX_COMPLETE_CB_ID Master Tx Transfer completed callback ID
  *          @arg @ref HAL_I2C_MASTER_RX_COMPLETE_CB_ID Master Rx Transfer completed callback ID
  *          @arg @ref HAL_I2C_SLAVE_TX_COMPLETE_CB_ID Slave Tx Transfer completed callback ID
  *          @arg @ref HAL_I2C_SLAVE_RX_COMPLETE_CB_ID Slave Rx Transfer completed callback ID
  *          @arg @ref HAL_I2C_LISTEN_COMPLETE_CB_ID Listen Complete callback ID
  *          @arg @ref HAL_I2C_MEM_TX_COMPLETE_CB_ID Memory Tx Transfer callback ID
  *          @arg @ref HAL_I2C_MEM_RX_COMPLETE_CB_ID Memory Rx Transfer completed callback ID
  *          @arg @ref HAL_I2C_ERROR_CB_ID Error callback ID
  *          @arg @ref HAL_I2C_ABORT_CB_ID Abort callback ID
  *          @arg @ref HAL_I2C_MSPINIT_CB_ID MspInit callback ID
  *          @arg @ref HAL_I2C_MSPDEINIT_CB_ID MspDeInit callback ID
  * @param  pCallback pointer to the Callback function
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_RegisterCallback(I2C_HandleTypeDef *hi2c, HAL_I2C_CallbackIDTypeDef CallbackID, pI2C_CallbackTypeDef pCallback)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  if (pCallback == NULL)                                /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hi2c->ErrorCode |= HAL_I2C_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  /* Process locked */
  __HAL_LOCK(hi2c);                                     /* HAL辅助宏/函数 */

  if (HAL_I2C_STATE_READY == hi2c->State)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_I2C_MASTER_TX_COMPLETE_CB_ID :           /* case标签 */
        hi2c->MasterTxCpltCallback = pCallback;         /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_I2C_MASTER_RX_COMPLETE_CB_ID :           /* case标签 */
        hi2c->MasterRxCpltCallback = pCallback;         /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_I2C_SLAVE_TX_COMPLETE_CB_ID :            /* case标签 */
        hi2c->SlaveTxCpltCallback = pCallback;          /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_I2C_SLAVE_RX_COMPLETE_CB_ID :            /* case标签 */
        hi2c->SlaveRxCpltCallback = pCallback;          /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_I2C_LISTEN_COMPLETE_CB_ID :              /* case标签 */
        hi2c->ListenCpltCallback = pCallback;           /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_I2C_MEM_TX_COMPLETE_CB_ID :              /* case标签 */
        hi2c->MemTxCpltCallback = pCallback;            /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_I2C_MEM_RX_COMPLETE_CB_ID :              /* case标签 */
        hi2c->MemRxCpltCallback = pCallback;            /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_I2C_ERROR_CB_ID :                        /* case标签 */
        hi2c->ErrorCallback = pCallback;                /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_I2C_ABORT_CB_ID :                        /* case标签 */
        hi2c->AbortCpltCallback = pCallback;            /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_I2C_MSPINIT_CB_ID :                      /* case标签 */
        hi2c->MspInitCallback = pCallback;              /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_I2C_MSPDEINIT_CB_ID :                    /* case标签 */
        hi2c->MspDeInitCallback = pCallback;            /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hi2c->ErrorCode |= HAL_I2C_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (HAL_I2C_STATE_RESET == hi2c->State)          /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_I2C_MSPINIT_CB_ID :                      /* case标签 */
        hi2c->MspInitCallback = pCallback;              /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_I2C_MSPDEINIT_CB_ID :                    /* case标签 */
        hi2c->MspDeInitCallback = pCallback;            /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hi2c->ErrorCode |= HAL_I2C_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hi2c->ErrorCode |= HAL_I2C_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hi2c);                                   /* HAL辅助宏/函数 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Unregister an I2C Callback
  *         I2C callback is redirected to the weak predefined callback
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  CallbackID ID of the callback to be unregistered
  *         This parameter can be one of the following values:
  *         This parameter can be one of the following values:
  *          @arg @ref HAL_I2C_MASTER_TX_COMPLETE_CB_ID Master Tx Transfer completed callback ID
  *          @arg @ref HAL_I2C_MASTER_RX_COMPLETE_CB_ID Master Rx Transfer completed callback ID
  *          @arg @ref HAL_I2C_SLAVE_TX_COMPLETE_CB_ID Slave Tx Transfer completed callback ID
  *          @arg @ref HAL_I2C_SLAVE_RX_COMPLETE_CB_ID Slave Rx Transfer completed callback ID
  *          @arg @ref HAL_I2C_LISTEN_COMPLETE_CB_ID Listen Complete callback ID
  *          @arg @ref HAL_I2C_MEM_TX_COMPLETE_CB_ID Memory Tx Transfer callback ID
  *          @arg @ref HAL_I2C_MEM_RX_COMPLETE_CB_ID Memory Rx Transfer completed callback ID
  *          @arg @ref HAL_I2C_ERROR_CB_ID Error callback ID
  *          @arg @ref HAL_I2C_ABORT_CB_ID Abort callback ID
  *          @arg @ref HAL_I2C_MSPINIT_CB_ID MspInit callback ID
  *          @arg @ref HAL_I2C_MSPDEINIT_CB_ID MspDeInit callback ID
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_UnRegisterCallback(I2C_HandleTypeDef *hi2c, HAL_I2C_CallbackIDTypeDef CallbackID)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Process locked */
  __HAL_LOCK(hi2c);                                     /* HAL辅助宏/函数 */

  if (HAL_I2C_STATE_READY == hi2c->State)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_I2C_MASTER_TX_COMPLETE_CB_ID :           /* case标签 */
        hi2c->MasterTxCpltCallback = HAL_I2C_MasterTxCpltCallback; /* Legacy weak MasterTxCpltCallback */
        break;                                          /* 跳出 */

      case HAL_I2C_MASTER_RX_COMPLETE_CB_ID :           /* case标签 */
        hi2c->MasterRxCpltCallback = HAL_I2C_MasterRxCpltCallback; /* Legacy weak MasterRxCpltCallback */
        break;                                          /* 跳出 */

      case HAL_I2C_SLAVE_TX_COMPLETE_CB_ID :            /* case标签 */
        hi2c->SlaveTxCpltCallback = HAL_I2C_SlaveTxCpltCallback;   /* Legacy weak SlaveTxCpltCallback  */
        break;                                          /* 跳出 */

      case HAL_I2C_SLAVE_RX_COMPLETE_CB_ID :            /* case标签 */
        hi2c->SlaveRxCpltCallback = HAL_I2C_SlaveRxCpltCallback;   /* Legacy weak SlaveRxCpltCallback  */
        break;                                          /* 跳出 */

      case HAL_I2C_LISTEN_COMPLETE_CB_ID :              /* case标签 */
        hi2c->ListenCpltCallback = HAL_I2C_ListenCpltCallback;     /* Legacy weak ListenCpltCallback   */
        break;                                          /* 跳出 */

      case HAL_I2C_MEM_TX_COMPLETE_CB_ID :              /* case标签 */
        hi2c->MemTxCpltCallback = HAL_I2C_MemTxCpltCallback;       /* Legacy weak MemTxCpltCallback    */
        break;                                          /* 跳出 */

      case HAL_I2C_MEM_RX_COMPLETE_CB_ID :              /* case标签 */
        hi2c->MemRxCpltCallback = HAL_I2C_MemRxCpltCallback;       /* Legacy weak MemRxCpltCallback    */
        break;                                          /* 跳出 */

      case HAL_I2C_ERROR_CB_ID :                        /* case标签 */
        hi2c->ErrorCallback = HAL_I2C_ErrorCallback;               /* Legacy weak ErrorCallback        */
        break;                                          /* 跳出 */

      case HAL_I2C_ABORT_CB_ID :                        /* case标签 */
        hi2c->AbortCpltCallback = HAL_I2C_AbortCpltCallback;       /* Legacy weak AbortCpltCallback    */
        break;                                          /* 跳出 */

      case HAL_I2C_MSPINIT_CB_ID :                      /* case标签 */
        hi2c->MspInitCallback = HAL_I2C_MspInit;                   /* Legacy weak MspInit              */
        break;                                          /* 跳出 */

      case HAL_I2C_MSPDEINIT_CB_ID :                    /* case标签 */
        hi2c->MspDeInitCallback = HAL_I2C_MspDeInit;               /* Legacy weak MspDeInit            */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hi2c->ErrorCode |= HAL_I2C_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (HAL_I2C_STATE_RESET == hi2c->State)          /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_I2C_MSPINIT_CB_ID :                      /* case标签 */
        hi2c->MspInitCallback = HAL_I2C_MspInit;                   /* Legacy weak MspInit              */
        break;                                          /* 跳出 */

      case HAL_I2C_MSPDEINIT_CB_ID :                    /* case标签 */
        hi2c->MspDeInitCallback = HAL_I2C_MspDeInit;               /* Legacy weak MspDeInit            */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hi2c->ErrorCode |= HAL_I2C_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hi2c->ErrorCode |= HAL_I2C_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hi2c);                                   /* HAL辅助宏/函数 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Register the Slave Address Match I2C Callback
  *         To be used instead of the weak HAL_I2C_AddrCallback() predefined callback
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  pCallback pointer to the Address Match Callback function
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_RegisterAddrCallback(I2C_HandleTypeDef *hi2c, pI2C_AddrCallbackTypeDef pCallback)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  if (pCallback == NULL)                                /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hi2c->ErrorCode |= HAL_I2C_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  /* Process locked */
  __HAL_LOCK(hi2c);                                     /* HAL辅助宏/函数 */

  if (HAL_I2C_STATE_READY == hi2c->State)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    hi2c->AddrCallback = pCallback;                     /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hi2c->ErrorCode |= HAL_I2C_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hi2c);                                   /* HAL辅助宏/函数 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  UnRegister the Slave Address Match I2C Callback
  *         Info Ready I2C Callback is redirected to the weak HAL_I2C_AddrCallback() predefined callback
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_UnRegisterAddrCallback(I2C_HandleTypeDef *hi2c)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Process locked */
  __HAL_LOCK(hi2c);                                     /* HAL辅助宏/函数 */

  if (HAL_I2C_STATE_READY == hi2c->State)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    hi2c->AddrCallback = HAL_I2C_AddrCallback; /* Legacy weak AddrCallback  */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hi2c->ErrorCode |= HAL_I2C_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hi2c);                                   /* HAL辅助宏/函数 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */

/**
  * @}
  */

/** @defgroup I2C_Exported_Functions_Group2 Input and Output operation functions
 *  @brief   Data transfers functions
 *
@verbatim
 ===============================================================================
                      ##### IO operation functions #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to manage the I2C data
    transfers.

    (#) There are two modes of transfer:
       (++) Blocking mode : The communication is performed in the polling mode.
            The status of all data processing is returned by the same function
            after finishing transfer.
       (++) No-Blocking mode : The communication is performed using Interrupts
            or DMA. These functions return the status of the transfer startup.
            The end of the data processing will be indicated through the
            dedicated I2C IRQ when using Interrupt mode or the DMA IRQ when
            using DMA mode.

    (#) Blocking mode functions are :
        (++) HAL_I2C_Master_Transmit()
        (++) HAL_I2C_Master_Receive()
        (++) HAL_I2C_Slave_Transmit()
        (++) HAL_I2C_Slave_Receive()
        (++) HAL_I2C_Mem_Write()
        (++) HAL_I2C_Mem_Read()
        (++) HAL_I2C_IsDeviceReady()

    (#) No-Blocking mode functions with Interrupt are :
        (++) HAL_I2C_Master_Transmit_IT()
        (++) HAL_I2C_Master_Receive_IT()
        (++) HAL_I2C_Slave_Transmit_IT()
        (++) HAL_I2C_Slave_Receive_IT()
        (++) HAL_I2C_Mem_Write_IT()
        (++) HAL_I2C_Mem_Read_IT()
        (++) HAL_I2C_Master_Seq_Transmit_IT()
        (++) HAL_I2C_Master_Seq_Receive_IT()
        (++) HAL_I2C_Slave_Seq_Transmit_IT()
        (++) HAL_I2C_Slave_Seq_Receive_IT()
        (++) HAL_I2C_EnableListen_IT()
        (++) HAL_I2C_DisableListen_IT()
        (++) HAL_I2C_Master_Abort_IT()

    (#) No-Blocking mode functions with DMA are :
        (++) HAL_I2C_Master_Transmit_DMA()
        (++) HAL_I2C_Master_Receive_DMA()
        (++) HAL_I2C_Slave_Transmit_DMA()
        (++) HAL_I2C_Slave_Receive_DMA()
        (++) HAL_I2C_Mem_Write_DMA()
        (++) HAL_I2C_Mem_Read_DMA()
        (++) HAL_I2C_Master_Seq_Transmit_DMA()
        (++) HAL_I2C_Master_Seq_Receive_DMA()
        (++) HAL_I2C_Slave_Seq_Transmit_DMA()
        (++) HAL_I2C_Slave_Seq_Receive_DMA()

    (#) A set of Transfer Complete Callbacks are provided in non Blocking mode:
        (++) HAL_I2C_MasterTxCpltCallback()
        (++) HAL_I2C_MasterRxCpltCallback()
        (++) HAL_I2C_SlaveTxCpltCallback()
        (++) HAL_I2C_SlaveRxCpltCallback()
        (++) HAL_I2C_MemTxCpltCallback()
        (++) HAL_I2C_MemRxCpltCallback()
        (++) HAL_I2C_AddrCallback()
        (++) HAL_I2C_ListenCpltCallback()
        (++) HAL_I2C_ErrorCallback()
        (++) HAL_I2C_AbortCpltCallback()

@endverbatim
  * @{
  */

/**
  * @brief  Transmits in master mode an amount of data in blocking mode.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Init tickstart for timeout management*/
  uint32_t tickstart = HAL_GetTick();                   /* 32位无符号变量/函数 */

  if (hi2c->State == HAL_I2C_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Wait until BUSY flag is reset */
    if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_BUSY, SET, I2C_TIMEOUT_BUSY_FLAG, tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_BUSY;                                  /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hi2c);                                   /* HAL辅助宏/函数 */

    /* Check if the I2C is already enabled */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable I2C peripheral */
      __HAL_I2C_ENABLE(hi2c);                           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Disable Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);        /* 执行语句 */

    hi2c->State       = HAL_I2C_STATE_BUSY_TX;          /* 赋值或初始化 */
    hi2c->Mode        = HAL_I2C_MODE_MASTER;            /* 赋值或初始化 */
    hi2c->ErrorCode   = HAL_I2C_ERROR_NONE;             /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hi2c->pBuffPtr    = pData;                          /* 赋值或初始化 */
    hi2c->XferCount   = Size;                           /* 赋值或初始化 */
    hi2c->XferSize    = hi2c->XferCount;                /* 赋值或初始化 */
    hi2c->XferOptions = I2C_NO_OPTION_FRAME;            /* 赋值或初始化 */

    /* Send Slave Address */
    if (I2C_MasterRequestWrite(hi2c, DevAddress, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Clear ADDR flag */
    __HAL_I2C_CLEAR_ADDRFLAG(hi2c);                     /* HAL辅助宏/函数 */

    while (hi2c->XferSize > 0U)                         /* while循环 */
    {                                                   /* 代码块开始 */
      /* Wait until TXE flag is set */
      if (I2C_WaitOnTXEFlagUntilTimeout(hi2c, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        if (hi2c->ErrorCode == HAL_I2C_ERROR_AF)        /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Generate Stop */
          SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);   /* 执行语句 */
        }                                               /* 代码块结束 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */

      /* Write data to DR */
      hi2c->Instance->DR = *hi2c->pBuffPtr;             /* 赋值或初始化 */

      /* Increment Buffer pointer */
      hi2c->pBuffPtr++;                                 /* 执行语句 */

      /* Update counter */
      hi2c->XferCount--;                                /* 执行语句 */
      hi2c->XferSize--;                                 /* 执行语句 */

      if ((__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BTF) == SET) && (hi2c->XferSize != 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Write data to DR */
        hi2c->Instance->DR = *hi2c->pBuffPtr;           /* 赋值或初始化 */

        /* Increment Buffer pointer */
        hi2c->pBuffPtr++;                               /* 执行语句 */

        /* Update counter */
        hi2c->XferCount--;                              /* 执行语句 */
        hi2c->XferSize--;                               /* 执行语句 */
      }                                                 /* 代码块结束 */

      /* Wait until BTF flag is set */
      if (I2C_WaitOnBTFFlagUntilTimeout(hi2c, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        if (hi2c->ErrorCode == HAL_I2C_ERROR_AF)        /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Generate Stop */
          SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);   /* 执行语句 */
        }                                               /* 代码块结束 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Generate Stop */
    SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);         /* 执行语句 */

    hi2c->State = HAL_I2C_STATE_READY;                  /* 赋值或初始化 */
    hi2c->Mode = HAL_I2C_MODE_NONE;                     /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hi2c);                                 /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Receives in master mode an amount of data in blocking mode.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Master_Receive(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Init tickstart for timeout management*/
  uint32_t tickstart = HAL_GetTick();                   /* 32位无符号变量/函数 */

  if (hi2c->State == HAL_I2C_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Wait until BUSY flag is reset */
    if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_BUSY, SET, I2C_TIMEOUT_BUSY_FLAG, tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_BUSY;                                  /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hi2c);                                   /* HAL辅助宏/函数 */

    /* Check if the I2C is already enabled */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable I2C peripheral */
      __HAL_I2C_ENABLE(hi2c);                           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Disable Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);        /* 执行语句 */

    hi2c->State       = HAL_I2C_STATE_BUSY_RX;          /* 赋值或初始化 */
    hi2c->Mode        = HAL_I2C_MODE_MASTER;            /* 赋值或初始化 */
    hi2c->ErrorCode   = HAL_I2C_ERROR_NONE;             /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hi2c->pBuffPtr    = pData;                          /* 赋值或初始化 */
    hi2c->XferCount   = Size;                           /* 赋值或初始化 */
    hi2c->XferSize    = hi2c->XferCount;                /* 赋值或初始化 */
    hi2c->XferOptions = I2C_NO_OPTION_FRAME;            /* 赋值或初始化 */

    /* Send Slave Address */
    if (I2C_MasterRequestRead(hi2c, DevAddress, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    if (hi2c->XferSize == 0U)                           /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear ADDR flag */
      __HAL_I2C_CLEAR_ADDRFLAG(hi2c);                   /* HAL辅助宏/函数 */

      /* Generate Stop */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);       /* 执行语句 */
    }                                                   /* 代码块结束 */
    else if (hi2c->XferSize == 1U)                      /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* Disable Acknowledge */
      CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);      /* 执行语句 */

      /* Clear ADDR flag */
      __HAL_I2C_CLEAR_ADDRFLAG(hi2c);                   /* HAL辅助宏/函数 */

      /* Generate Stop */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);       /* 执行语句 */
    }                                                   /* 代码块结束 */
    else if (hi2c->XferSize == 2U)                      /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* Disable Acknowledge */
      CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);      /* 执行语句 */

      /* Enable Pos */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_POS);        /* 执行语句 */

      /* Clear ADDR flag */
      __HAL_I2C_CLEAR_ADDRFLAG(hi2c);                   /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Enable Acknowledge */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);        /* 执行语句 */

      /* Clear ADDR flag */
      __HAL_I2C_CLEAR_ADDRFLAG(hi2c);                   /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    while (hi2c->XferSize > 0U)                         /* while循环 */
    {                                                   /* 代码块开始 */
      if (hi2c->XferSize <= 3U)                         /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* One byte */
        if (hi2c->XferSize == 1U)                       /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Wait until RXNE flag is set */
          if (I2C_WaitOnRXNEFlagUntilTimeout(hi2c, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
          {                                             /* 代码块开始 */
            return HAL_ERROR;                           /* 返回值 */
          }                                             /* 代码块结束 */

          /* Read data from DR */
          *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;  /* 赋值或初始化 */

          /* Increment Buffer pointer */
          hi2c->pBuffPtr++;                             /* 执行语句 */

          /* Update counter */
          hi2c->XferSize--;                             /* 执行语句 */
          hi2c->XferCount--;                            /* 执行语句 */
        }                                               /* 代码块结束 */
        /* Two bytes */
        else if (hi2c->XferSize == 2U)                  /* 否则如果 */
        {                                               /* 代码块开始 */
          /* Wait until BTF flag is set */
          if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_BTF, RESET, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
          {                                             /* 代码块开始 */
            return HAL_ERROR;                           /* 返回值 */
          }                                             /* 代码块结束 */

          /* Generate Stop */
          SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);   /* 执行语句 */

          /* Read data from DR */
          *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;  /* 赋值或初始化 */

          /* Increment Buffer pointer */
          hi2c->pBuffPtr++;                             /* 执行语句 */

          /* Update counter */
          hi2c->XferSize--;                             /* 执行语句 */
          hi2c->XferCount--;                            /* 执行语句 */

          /* Read data from DR */
          *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;  /* 赋值或初始化 */

          /* Increment Buffer pointer */
          hi2c->pBuffPtr++;                             /* 执行语句 */

          /* Update counter */
          hi2c->XferSize--;                             /* 执行语句 */
          hi2c->XferCount--;                            /* 执行语句 */
        }                                               /* 代码块结束 */
        /* 3 Last bytes */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          /* Wait until BTF flag is set */
          if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_BTF, RESET, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
          {                                             /* 代码块开始 */
            return HAL_ERROR;                           /* 返回值 */
          }                                             /* 代码块结束 */

          /* Disable Acknowledge */
          CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);  /* 执行语句 */

          /* Read data from DR */
          *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;  /* 赋值或初始化 */

          /* Increment Buffer pointer */
          hi2c->pBuffPtr++;                             /* 执行语句 */

          /* Update counter */
          hi2c->XferSize--;                             /* 执行语句 */
          hi2c->XferCount--;                            /* 执行语句 */

          /* Wait until BTF flag is set */
          if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_BTF, RESET, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
          {                                             /* 代码块开始 */
            return HAL_ERROR;                           /* 返回值 */
          }                                             /* 代码块结束 */

          /* Generate Stop */
          SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);   /* 执行语句 */

          /* Read data from DR */
          *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;  /* 赋值或初始化 */

          /* Increment Buffer pointer */
          hi2c->pBuffPtr++;                             /* 执行语句 */

          /* Update counter */
          hi2c->XferSize--;                             /* 执行语句 */
          hi2c->XferCount--;                            /* 执行语句 */

          /* Read data from DR */
          *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;  /* 赋值或初始化 */

          /* Increment Buffer pointer */
          hi2c->pBuffPtr++;                             /* 执行语句 */

          /* Update counter */
          hi2c->XferSize--;                             /* 执行语句 */
          hi2c->XferCount--;                            /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Wait until RXNE flag is set */
        if (I2C_WaitOnRXNEFlagUntilTimeout(hi2c, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
        {                                               /* 代码块开始 */
          return HAL_ERROR;                             /* 返回值 */
        }                                               /* 代码块结束 */

        /* Read data from DR */
        *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;  /* 赋值或初始化 */

        /* Increment Buffer pointer */
        hi2c->pBuffPtr++;                               /* 执行语句 */

        /* Update counter */
        hi2c->XferSize--;                               /* 执行语句 */
        hi2c->XferCount--;                              /* 执行语句 */

        if (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BTF) == SET)  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Read data from DR */
          *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;  /* 赋值或初始化 */

          /* Increment Buffer pointer */
          hi2c->pBuffPtr++;                             /* 执行语句 */

          /* Update counter */
          hi2c->XferSize--;                             /* 执行语句 */
          hi2c->XferCount--;                            /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    hi2c->State = HAL_I2C_STATE_READY;                  /* 赋值或初始化 */
    hi2c->Mode = HAL_I2C_MODE_NONE;                     /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hi2c);                                 /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Transmits in slave mode an amount of data in blocking mode.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Slave_Transmit(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Init tickstart for timeout management*/
  uint32_t tickstart = HAL_GetTick();                   /* 32位无符号变量/函数 */

  if (hi2c->State == HAL_I2C_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) || (Size == 0U))                /* 条件判断 */
    {                                                   /* 代码块开始 */
      return  HAL_ERROR;                                /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hi2c);                                   /* HAL辅助宏/函数 */

    /* Check if the I2C is already enabled */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable I2C peripheral */
      __HAL_I2C_ENABLE(hi2c);                           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Disable Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);        /* 执行语句 */

    hi2c->State       = HAL_I2C_STATE_BUSY_TX;          /* 赋值或初始化 */
    hi2c->Mode        = HAL_I2C_MODE_SLAVE;             /* 赋值或初始化 */
    hi2c->ErrorCode   = HAL_I2C_ERROR_NONE;             /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hi2c->pBuffPtr    = pData;                          /* 赋值或初始化 */
    hi2c->XferCount   = Size;                           /* 赋值或初始化 */
    hi2c->XferSize    = hi2c->XferCount;                /* 赋值或初始化 */
    hi2c->XferOptions = I2C_NO_OPTION_FRAME;            /* 赋值或初始化 */

    /* Enable Address Acknowledge */
    SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);          /* 执行语句 */

    /* Wait until ADDR flag is set */
    if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_ADDR, RESET, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Clear ADDR flag */
    __HAL_I2C_CLEAR_ADDRFLAG(hi2c);                     /* HAL辅助宏/函数 */

    /* If 10bit addressing mode is selected */
    if (hi2c->Init.AddressingMode == I2C_ADDRESSINGMODE_10BIT)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Wait until ADDR flag is set */
      if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_ADDR, RESET, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */

      /* Clear ADDR flag */
      __HAL_I2C_CLEAR_ADDRFLAG(hi2c);                   /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    while (hi2c->XferSize > 0U)                         /* while循环 */
    {                                                   /* 代码块开始 */
      /* Wait until TXE flag is set */
      if (I2C_WaitOnTXEFlagUntilTimeout(hi2c, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Disable Address Acknowledge */
        CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);    /* 执行语句 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */

      /* Write data to DR */
      hi2c->Instance->DR = *hi2c->pBuffPtr;             /* 赋值或初始化 */

      /* Increment Buffer pointer */
      hi2c->pBuffPtr++;                                 /* 执行语句 */

      /* Update counter */
      hi2c->XferCount--;                                /* 执行语句 */
      hi2c->XferSize--;                                 /* 执行语句 */

      if ((__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BTF) == SET) && (hi2c->XferSize != 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Write data to DR */
        hi2c->Instance->DR = *hi2c->pBuffPtr;           /* 赋值或初始化 */

        /* Increment Buffer pointer */
        hi2c->pBuffPtr++;                               /* 执行语句 */

        /* Update counter */
        hi2c->XferCount--;                              /* 执行语句 */
        hi2c->XferSize--;                               /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Wait until AF flag is set */
    if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_AF, RESET, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Clear AF flag */
    __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_AF);            /* HAL辅助宏/函数 */

    /* Disable Address Acknowledge */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);        /* 执行语句 */

    hi2c->State = HAL_I2C_STATE_READY;                  /* 赋值或初始化 */
    hi2c->Mode = HAL_I2C_MODE_NONE;                     /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hi2c);                                 /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Receive in slave mode an amount of data in blocking mode
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for the specified I2C.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Slave_Receive(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Init tickstart for timeout management*/
  uint32_t tickstart = HAL_GetTick();                   /* 32位无符号变量/函数 */

  if (hi2c->State == HAL_I2C_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) || (Size == (uint16_t)0))       /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hi2c);                                   /* HAL辅助宏/函数 */

    /* Check if the I2C is already enabled */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable I2C peripheral */
      __HAL_I2C_ENABLE(hi2c);                           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Disable Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);        /* 执行语句 */

    hi2c->State       = HAL_I2C_STATE_BUSY_RX;          /* 赋值或初始化 */
    hi2c->Mode        = HAL_I2C_MODE_SLAVE;             /* 赋值或初始化 */
    hi2c->ErrorCode   = HAL_I2C_ERROR_NONE;             /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hi2c->pBuffPtr    = pData;                          /* 赋值或初始化 */
    hi2c->XferCount   = Size;                           /* 赋值或初始化 */
    hi2c->XferSize    = hi2c->XferCount;                /* 赋值或初始化 */
    hi2c->XferOptions = I2C_NO_OPTION_FRAME;            /* 赋值或初始化 */

    /* Enable Address Acknowledge */
    SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);          /* 执行语句 */

    /* Wait until ADDR flag is set */
    if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_ADDR, RESET, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Clear ADDR flag */
    __HAL_I2C_CLEAR_ADDRFLAG(hi2c);                     /* HAL辅助宏/函数 */

    while (hi2c->XferSize > 0U)                         /* while循环 */
    {                                                   /* 代码块开始 */
      /* Wait until RXNE flag is set */
      if (I2C_WaitOnRXNEFlagUntilTimeout(hi2c, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Disable Address Acknowledge */
        CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);    /* 执行语句 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */

      /* Read data from DR */
      *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;    /* 赋值或初始化 */

      /* Increment Buffer pointer */
      hi2c->pBuffPtr++;                                 /* 执行语句 */

      /* Update counter */
      hi2c->XferSize--;                                 /* 执行语句 */
      hi2c->XferCount--;                                /* 执行语句 */

      if ((__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BTF) == SET) && (hi2c->XferSize != 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Read data from DR */
        *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;  /* 赋值或初始化 */

        /* Increment Buffer pointer */
        hi2c->pBuffPtr++;                               /* 执行语句 */

        /* Update counter */
        hi2c->XferSize--;                               /* 执行语句 */
        hi2c->XferCount--;                              /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Wait until STOP flag is set */
    if (I2C_WaitOnSTOPFlagUntilTimeout(hi2c, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable Address Acknowledge */
      CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);      /* 执行语句 */

      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Clear STOP flag */
    __HAL_I2C_CLEAR_STOPFLAG(hi2c);                     /* HAL辅助宏/函数 */

    /* Disable Address Acknowledge */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);        /* 执行语句 */

    hi2c->State = HAL_I2C_STATE_READY;                  /* 赋值或初始化 */
    hi2c->Mode = HAL_I2C_MODE_NONE;                     /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hi2c);                                 /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Transmit in master mode an amount of data in non-blocking mode with Interrupt
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Master_Transmit_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  __IO uint32_t count = 0U;                             /* 赋值或初始化 */

  if (hi2c->State == HAL_I2C_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Wait until BUSY flag is reset */
    count = I2C_TIMEOUT_BUSY_FLAG * (SystemCoreClock / 25U / 1000U);  /* 赋值或初始化 */
    do                                                  /* do-while循环 */
    {                                                   /* 代码块开始 */
      count--;                                          /* 执行语句 */
      if (count == 0U)                                  /* 条件判断 */
      {                                                 /* 代码块开始 */
        hi2c->PreviousState       = I2C_STATE_NONE;     /* 赋值或初始化 */
        hi2c->State               = HAL_I2C_STATE_READY;  /* 赋值或初始化 */
        hi2c->Mode                = HAL_I2C_MODE_NONE;  /* 赋值或初始化 */
        hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;  /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);                             /* HAL辅助宏/函数 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    while (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BUSY) != RESET);  /* while循环 */

    /* Process Locked */
    __HAL_LOCK(hi2c);                                   /* HAL辅助宏/函数 */

    /* Check if the I2C is already enabled */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable I2C peripheral */
      __HAL_I2C_ENABLE(hi2c);                           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Disable Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);        /* 执行语句 */

    hi2c->State     = HAL_I2C_STATE_BUSY_TX;            /* 赋值或初始化 */
    hi2c->Mode      = HAL_I2C_MODE_MASTER;              /* 赋值或初始化 */
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;               /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hi2c->pBuffPtr    = pData;                          /* 赋值或初始化 */
    hi2c->XferCount   = Size;                           /* 赋值或初始化 */
    hi2c->XferSize    = hi2c->XferCount;                /* 赋值或初始化 */
    hi2c->XferOptions = I2C_NO_OPTION_FRAME;            /* 赋值或初始化 */
    hi2c->Devaddress  = DevAddress;                     /* 赋值或初始化 */

    /* Generate Start */
    SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);        /* 执行语句 */

    /* Process Unlocked */
    __HAL_UNLOCK(hi2c);                                 /* HAL辅助宏/函数 */

    /* Note : The I2C interrupts must be enabled after unlocking current process
              to avoid the risk of I2C interrupt handle execution before current
              process unlock */
    /* Enable EVT, BUF and ERR interrupt */
    __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);  /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Receive in master mode an amount of data in non-blocking mode with Interrupt
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Master_Receive_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  __IO uint32_t count = 0U;                             /* 赋值或初始化 */

  if (hi2c->State == HAL_I2C_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Wait until BUSY flag is reset */
    count = I2C_TIMEOUT_BUSY_FLAG * (SystemCoreClock / 25U / 1000U);  /* 赋值或初始化 */
    do                                                  /* do-while循环 */
    {                                                   /* 代码块开始 */
      count--;                                          /* 执行语句 */
      if (count == 0U)                                  /* 条件判断 */
      {                                                 /* 代码块开始 */
        hi2c->PreviousState       = I2C_STATE_NONE;     /* 赋值或初始化 */
        hi2c->State               = HAL_I2C_STATE_READY;  /* 赋值或初始化 */
        hi2c->Mode                = HAL_I2C_MODE_NONE;  /* 赋值或初始化 */
        hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;  /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);                             /* HAL辅助宏/函数 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    while (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BUSY) != RESET);  /* while循环 */

    /* Process Locked */
    __HAL_LOCK(hi2c);                                   /* HAL辅助宏/函数 */

    /* Check if the I2C is already enabled */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable I2C peripheral */
      __HAL_I2C_ENABLE(hi2c);                           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Disable Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);        /* 执行语句 */

    hi2c->State     = HAL_I2C_STATE_BUSY_RX;            /* 赋值或初始化 */
    hi2c->Mode      = HAL_I2C_MODE_MASTER;              /* 赋值或初始化 */
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;               /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hi2c->pBuffPtr    = pData;                          /* 赋值或初始化 */
    hi2c->XferCount   = Size;                           /* 赋值或初始化 */
    hi2c->XferSize    = hi2c->XferCount;                /* 赋值或初始化 */
    hi2c->XferOptions = I2C_NO_OPTION_FRAME;            /* 赋值或初始化 */
    hi2c->Devaddress  = DevAddress;                     /* 赋值或初始化 */

    /* Enable Acknowledge */
    SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);          /* 执行语句 */

    /* Generate Start */
    SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);        /* 执行语句 */

    /* Process Unlocked */
    __HAL_UNLOCK(hi2c);                                 /* HAL辅助宏/函数 */

    /* Note : The I2C interrupts must be enabled after unlocking current process
    to avoid the risk of I2C interrupt handle execution before current
    process unlock */

    /* Enable EVT, BUF and ERR interrupt */
    __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);  /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Transmit in slave mode an amount of data in non-blocking mode with Interrupt
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for the specified I2C.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Slave_Transmit_IT(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */

  if (hi2c->State == HAL_I2C_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) || (Size == 0U))                /* 条件判断 */
    {                                                   /* 代码块开始 */
      return  HAL_ERROR;                                /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hi2c);                                   /* HAL辅助宏/函数 */

    /* Check if the I2C is already enabled */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable I2C peripheral */
      __HAL_I2C_ENABLE(hi2c);                           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Disable Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);        /* 执行语句 */

    hi2c->State     = HAL_I2C_STATE_BUSY_TX;            /* 赋值或初始化 */
    hi2c->Mode      = HAL_I2C_MODE_SLAVE;               /* 赋值或初始化 */
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;               /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hi2c->pBuffPtr    = pData;                          /* 赋值或初始化 */
    hi2c->XferCount   = Size;                           /* 赋值或初始化 */
    hi2c->XferSize    = hi2c->XferCount;                /* 赋值或初始化 */
    hi2c->XferOptions = I2C_NO_OPTION_FRAME;            /* 赋值或初始化 */

    /* Enable Address Acknowledge */
    SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);          /* 执行语句 */

    /* Process Unlocked */
    __HAL_UNLOCK(hi2c);                                 /* HAL辅助宏/函数 */

    /* Note : The I2C interrupts must be enabled after unlocking current process
              to avoid the risk of I2C interrupt handle execution before current
              process unlock */

    /* Enable EVT, BUF and ERR interrupt */
    __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);  /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Receive in slave mode an amount of data in non-blocking mode with Interrupt
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Slave_Receive_IT(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */

  if (hi2c->State == HAL_I2C_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) || (Size == 0U))                /* 条件判断 */
    {                                                   /* 代码块开始 */
      return  HAL_ERROR;                                /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hi2c);                                   /* HAL辅助宏/函数 */

    /* Check if the I2C is already enabled */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable I2C peripheral */
      __HAL_I2C_ENABLE(hi2c);                           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Disable Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);        /* 执行语句 */

    hi2c->State     = HAL_I2C_STATE_BUSY_RX;            /* 赋值或初始化 */
    hi2c->Mode      = HAL_I2C_MODE_SLAVE;               /* 赋值或初始化 */
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;               /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hi2c->pBuffPtr    = pData;                          /* 赋值或初始化 */
    hi2c->XferCount   = Size;                           /* 赋值或初始化 */
    hi2c->XferSize    = hi2c->XferCount;                /* 赋值或初始化 */
    hi2c->XferOptions = I2C_NO_OPTION_FRAME;            /* 赋值或初始化 */

    /* Enable Address Acknowledge */
    SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);          /* 执行语句 */

    /* Process Unlocked */
    __HAL_UNLOCK(hi2c);                                 /* HAL辅助宏/函数 */

    /* Note : The I2C interrupts must be enabled after unlocking current process
              to avoid the risk of I2C interrupt handle execution before current
              process unlock */

    /* Enable EVT, BUF and ERR interrupt */
    __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);  /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Transmit in master mode an amount of data in non-blocking mode with DMA
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Master_Transmit_DMA(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  __IO uint32_t count = 0U;                             /* 赋值或初始化 */
  HAL_StatusTypeDef dmaxferstatus;                      /* HAL状态类型函数 */

  if (hi2c->State == HAL_I2C_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Wait until BUSY flag is reset */
    count = I2C_TIMEOUT_BUSY_FLAG * (SystemCoreClock / 25U / 1000U);  /* 赋值或初始化 */
    do                                                  /* do-while循环 */
    {                                                   /* 代码块开始 */
      count--;                                          /* 执行语句 */
      if (count == 0U)                                  /* 条件判断 */
      {                                                 /* 代码块开始 */
        hi2c->PreviousState       = I2C_STATE_NONE;     /* 赋值或初始化 */
        hi2c->State               = HAL_I2C_STATE_READY;  /* 赋值或初始化 */
        hi2c->Mode                = HAL_I2C_MODE_NONE;  /* 赋值或初始化 */
        hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;  /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);                             /* HAL辅助宏/函数 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    while (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BUSY) != RESET);  /* while循环 */

    /* Process Locked */
    __HAL_LOCK(hi2c);                                   /* HAL辅助宏/函数 */

    /* Check if the I2C is already enabled */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable I2C peripheral */
      __HAL_I2C_ENABLE(hi2c);                           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Disable Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);        /* 执行语句 */

    hi2c->State     = HAL_I2C_STATE_BUSY_TX;            /* 赋值或初始化 */
    hi2c->Mode      = HAL_I2C_MODE_MASTER;              /* 赋值或初始化 */
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;               /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hi2c->pBuffPtr    = pData;                          /* 赋值或初始化 */
    hi2c->XferCount   = Size;                           /* 赋值或初始化 */
    hi2c->XferSize    = hi2c->XferCount;                /* 赋值或初始化 */
    hi2c->XferOptions = I2C_NO_OPTION_FRAME;            /* 赋值或初始化 */
    hi2c->Devaddress  = DevAddress;                     /* 赋值或初始化 */

    if (hi2c->XferSize > 0U)                            /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (hi2c->hdmatx != NULL)                         /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Set the I2C DMA transfer complete callback */
        hi2c->hdmatx->XferCpltCallback = I2C_DMAXferCplt;  /* 赋值或初始化 */

        /* Set the DMA error callback */
        hi2c->hdmatx->XferErrorCallback = I2C_DMAError;  /* 赋值或初始化 */

        /* Set the unused DMA callbacks to NULL */
        hi2c->hdmatx->XferHalfCpltCallback = NULL;      /* 赋值或初始化 */
        hi2c->hdmatx->XferM1CpltCallback = NULL;        /* 赋值或初始化 */
        hi2c->hdmatx->XferM1HalfCpltCallback = NULL;    /* 赋值或初始化 */
        hi2c->hdmatx->XferAbortCallback = NULL;         /* 赋值或初始化 */

        /* Enable the DMA stream */
        dmaxferstatus = HAL_DMA_Start_IT(hi2c->hdmatx, (uint32_t)hi2c->pBuffPtr, (uint32_t)&hi2c->Instance->DR, hi2c->XferSize);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Update I2C state */
        hi2c->State     = HAL_I2C_STATE_READY;          /* 赋值或初始化 */
        hi2c->Mode      = HAL_I2C_MODE_NONE;            /* 赋值或初始化 */

        /* Update I2C error code */
        hi2c->ErrorCode |= HAL_I2C_ERROR_DMA_PARAM;     /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);                             /* HAL辅助宏/函数 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */

      if (dmaxferstatus == HAL_OK)                      /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Enable Acknowledge */
        SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);      /* 执行语句 */

        /* Generate Start */
        SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);    /* 执行语句 */

        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);                             /* HAL辅助宏/函数 */

        /* Note : The I2C interrupts must be enabled after unlocking current process
        to avoid the risk of I2C interrupt handle execution before current
        process unlock */

        /* Enable EVT and ERR interrupt */
        __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_ERR);  /* HAL辅助宏/函数 */

        /* Enable DMA Request */
        SET_BIT(hi2c->Instance->CR2, I2C_CR2_DMAEN);    /* 执行语句 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Update I2C state */
        hi2c->State     = HAL_I2C_STATE_READY;          /* 赋值或初始化 */
        hi2c->Mode      = HAL_I2C_MODE_NONE;            /* 赋值或初始化 */

        /* Update I2C error code */
        hi2c->ErrorCode |= HAL_I2C_ERROR_DMA;           /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);                             /* HAL辅助宏/函数 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Enable Acknowledge */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);        /* 执行语句 */

      /* Generate Start */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);      /* 执行语句 */

      /* Process Unlocked */
      __HAL_UNLOCK(hi2c);                               /* HAL辅助宏/函数 */

      /* Note : The I2C interrupts must be enabled after unlocking current process
      to avoid the risk of I2C interrupt handle execution before current
      process unlock */

      /* Enable EVT, BUF and ERR interrupt */
      __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Receive in master mode an amount of data in non-blocking mode with DMA
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Master_Receive_DMA(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  __IO uint32_t count = 0U;                             /* 赋值或初始化 */
  HAL_StatusTypeDef dmaxferstatus;                      /* HAL状态类型函数 */

  if (hi2c->State == HAL_I2C_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Wait until BUSY flag is reset */
    count = I2C_TIMEOUT_BUSY_FLAG * (SystemCoreClock / 25U / 1000U);  /* 赋值或初始化 */
    do                                                  /* do-while循环 */
    {                                                   /* 代码块开始 */
      count--;                                          /* 执行语句 */
      if (count == 0U)                                  /* 条件判断 */
      {                                                 /* 代码块开始 */
        hi2c->PreviousState       = I2C_STATE_NONE;     /* 赋值或初始化 */
        hi2c->State               = HAL_I2C_STATE_READY;  /* 赋值或初始化 */
        hi2c->Mode                = HAL_I2C_MODE_NONE;  /* 赋值或初始化 */
        hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;  /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);                             /* HAL辅助宏/函数 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    while (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BUSY) != RESET);  /* while循环 */

    /* Process Locked */
    __HAL_LOCK(hi2c);                                   /* HAL辅助宏/函数 */

    /* Check if the I2C is already enabled */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable I2C peripheral */
      __HAL_I2C_ENABLE(hi2c);                           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Disable Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);        /* 执行语句 */

    hi2c->State     = HAL_I2C_STATE_BUSY_RX;            /* 赋值或初始化 */
    hi2c->Mode      = HAL_I2C_MODE_MASTER;              /* 赋值或初始化 */
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;               /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hi2c->pBuffPtr    = pData;                          /* 赋值或初始化 */
    hi2c->XferCount   = Size;                           /* 赋值或初始化 */
    hi2c->XferSize    = hi2c->XferCount;                /* 赋值或初始化 */
    hi2c->XferOptions = I2C_NO_OPTION_FRAME;            /* 赋值或初始化 */
    hi2c->Devaddress  = DevAddress;                     /* 赋值或初始化 */

    if (hi2c->XferSize > 0U)                            /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (hi2c->hdmarx != NULL)                         /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Set the I2C DMA transfer complete callback */
        hi2c->hdmarx->XferCpltCallback = I2C_DMAXferCplt;  /* 赋值或初始化 */

        /* Set the DMA error callback */
        hi2c->hdmarx->XferErrorCallback = I2C_DMAError;  /* 赋值或初始化 */

        /* Set the unused DMA callbacks to NULL */
        hi2c->hdmarx->XferHalfCpltCallback = NULL;      /* 赋值或初始化 */
        hi2c->hdmarx->XferM1CpltCallback = NULL;        /* 赋值或初始化 */
        hi2c->hdmarx->XferM1HalfCpltCallback = NULL;    /* 赋值或初始化 */
        hi2c->hdmarx->XferAbortCallback = NULL;         /* 赋值或初始化 */

        /* Enable the DMA stream */
        dmaxferstatus = HAL_DMA_Start_IT(hi2c->hdmarx, (uint32_t)&hi2c->Instance->DR, (uint32_t)hi2c->pBuffPtr, hi2c->XferSize);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Update I2C state */
        hi2c->State     = HAL_I2C_STATE_READY;          /* 赋值或初始化 */
        hi2c->Mode      = HAL_I2C_MODE_NONE;            /* 赋值或初始化 */

        /* Update I2C error code */
        hi2c->ErrorCode |= HAL_I2C_ERROR_DMA_PARAM;     /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);                             /* HAL辅助宏/函数 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */

      if (dmaxferstatus == HAL_OK)                      /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Enable Acknowledge */
        SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);      /* 执行语句 */

        /* Generate Start */
        SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);    /* 执行语句 */

        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);                             /* HAL辅助宏/函数 */

        /* Note : The I2C interrupts must be enabled after unlocking current process
        to avoid the risk of I2C interrupt handle execution before current
        process unlock */

        /* Enable EVT and ERR interrupt */
        __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_ERR);  /* HAL辅助宏/函数 */

        /* Enable DMA Request */
        SET_BIT(hi2c->Instance->CR2, I2C_CR2_DMAEN);    /* 执行语句 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Update I2C state */
        hi2c->State     = HAL_I2C_STATE_READY;          /* 赋值或初始化 */
        hi2c->Mode      = HAL_I2C_MODE_NONE;            /* 赋值或初始化 */

        /* Update I2C error code */
        hi2c->ErrorCode |= HAL_I2C_ERROR_DMA;           /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);                             /* HAL辅助宏/函数 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Enable Acknowledge */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);        /* 执行语句 */

      /* Generate Start */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);      /* 执行语句 */

      /* Process Unlocked */
      __HAL_UNLOCK(hi2c);                               /* HAL辅助宏/函数 */

      /* Note : The I2C interrupts must be enabled after unlocking current process
      to avoid the risk of I2C interrupt handle execution before current
      process unlock */

      /* Enable EVT, BUF and ERR interrupt */
      __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Transmit in slave mode an amount of data in non-blocking mode with DMA
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Slave_Transmit_DMA(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef dmaxferstatus;                      /* HAL状态类型函数 */

  if (hi2c->State == HAL_I2C_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) || (Size == 0U))                /* 条件判断 */
    {                                                   /* 代码块开始 */
      return  HAL_ERROR;                                /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hi2c);                                   /* HAL辅助宏/函数 */

    /* Check if the I2C is already enabled */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable I2C peripheral */
      __HAL_I2C_ENABLE(hi2c);                           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Disable Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);        /* 执行语句 */

    hi2c->State     = HAL_I2C_STATE_BUSY_TX;            /* 赋值或初始化 */
    hi2c->Mode      = HAL_I2C_MODE_SLAVE;               /* 赋值或初始化 */
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;               /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hi2c->pBuffPtr    = pData;                          /* 赋值或初始化 */
    hi2c->XferCount   = Size;                           /* 赋值或初始化 */
    hi2c->XferSize    = hi2c->XferCount;                /* 赋值或初始化 */
    hi2c->XferOptions = I2C_NO_OPTION_FRAME;            /* 赋值或初始化 */

    if (hi2c->hdmatx != NULL)                           /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the I2C DMA transfer complete callback */
      hi2c->hdmatx->XferCpltCallback = I2C_DMAXferCplt;  /* 赋值或初始化 */

      /* Set the DMA error callback */
      hi2c->hdmatx->XferErrorCallback = I2C_DMAError;   /* 赋值或初始化 */

      /* Set the unused DMA callbacks to NULL */
      hi2c->hdmatx->XferHalfCpltCallback = NULL;        /* 赋值或初始化 */
      hi2c->hdmatx->XferM1CpltCallback = NULL;          /* 赋值或初始化 */
      hi2c->hdmatx->XferM1HalfCpltCallback = NULL;      /* 赋值或初始化 */
      hi2c->hdmatx->XferAbortCallback = NULL;           /* 赋值或初始化 */

      /* Enable the DMA stream */
      dmaxferstatus = HAL_DMA_Start_IT(hi2c->hdmatx, (uint32_t)hi2c->pBuffPtr, (uint32_t)&hi2c->Instance->DR, hi2c->XferSize);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Update I2C state */
      hi2c->State     = HAL_I2C_STATE_LISTEN;           /* 赋值或初始化 */
      hi2c->Mode      = HAL_I2C_MODE_NONE;              /* 赋值或初始化 */

      /* Update I2C error code */
      hi2c->ErrorCode |= HAL_I2C_ERROR_DMA_PARAM;       /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hi2c);                               /* HAL辅助宏/函数 */

      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    if (dmaxferstatus == HAL_OK)                        /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable Address Acknowledge */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);        /* 执行语句 */

      /* Process Unlocked */
      __HAL_UNLOCK(hi2c);                               /* HAL辅助宏/函数 */

      /* Note : The I2C interrupts must be enabled after unlocking current process
      to avoid the risk of I2C interrupt handle execution before current
      process unlock */
      /* Enable EVT and ERR interrupt */
      __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_ERR);  /* HAL辅助宏/函数 */

      /* Enable DMA Request */
      hi2c->Instance->CR2 |= I2C_CR2_DMAEN;             /* 赋值或初始化 */

      return HAL_OK;                                    /* 返回值 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Update I2C state */
      hi2c->State     = HAL_I2C_STATE_READY;            /* 赋值或初始化 */
      hi2c->Mode      = HAL_I2C_MODE_NONE;              /* 赋值或初始化 */

      /* Update I2C error code */
      hi2c->ErrorCode |= HAL_I2C_ERROR_DMA;             /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hi2c);                               /* HAL辅助宏/函数 */

      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Receive in slave mode an amount of data in non-blocking mode with DMA
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Slave_Receive_DMA(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef dmaxferstatus;                      /* HAL状态类型函数 */

  if (hi2c->State == HAL_I2C_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) || (Size == 0U))                /* 条件判断 */
    {                                                   /* 代码块开始 */
      return  HAL_ERROR;                                /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hi2c);                                   /* HAL辅助宏/函数 */

    /* Check if the I2C is already enabled */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable I2C peripheral */
      __HAL_I2C_ENABLE(hi2c);                           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Disable Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);        /* 执行语句 */

    hi2c->State     = HAL_I2C_STATE_BUSY_RX;            /* 赋值或初始化 */
    hi2c->Mode      = HAL_I2C_MODE_SLAVE;               /* 赋值或初始化 */
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;               /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hi2c->pBuffPtr    = pData;                          /* 赋值或初始化 */
    hi2c->XferCount   = Size;                           /* 赋值或初始化 */
    hi2c->XferSize    = hi2c->XferCount;                /* 赋值或初始化 */
    hi2c->XferOptions = I2C_NO_OPTION_FRAME;            /* 赋值或初始化 */

    if (hi2c->hdmarx != NULL)                           /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the I2C DMA transfer complete callback */
      hi2c->hdmarx->XferCpltCallback = I2C_DMAXferCplt;  /* 赋值或初始化 */

      /* Set the DMA error callback */
      hi2c->hdmarx->XferErrorCallback = I2C_DMAError;   /* 赋值或初始化 */

      /* Set the unused DMA callbacks to NULL */
      hi2c->hdmarx->XferHalfCpltCallback = NULL;        /* 赋值或初始化 */
      hi2c->hdmarx->XferM1CpltCallback = NULL;          /* 赋值或初始化 */
      hi2c->hdmarx->XferM1HalfCpltCallback = NULL;      /* 赋值或初始化 */
      hi2c->hdmarx->XferAbortCallback = NULL;           /* 赋值或初始化 */

      /* Enable the DMA stream */
      dmaxferstatus = HAL_DMA_Start_IT(hi2c->hdmarx, (uint32_t)&hi2c->Instance->DR, (uint32_t)hi2c->pBuffPtr, hi2c->XferSize);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Update I2C state */
      hi2c->State     = HAL_I2C_STATE_LISTEN;           /* 赋值或初始化 */
      hi2c->Mode      = HAL_I2C_MODE_NONE;              /* 赋值或初始化 */

      /* Update I2C error code */
      hi2c->ErrorCode |= HAL_I2C_ERROR_DMA_PARAM;       /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hi2c);                               /* HAL辅助宏/函数 */

      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    if (dmaxferstatus == HAL_OK)                        /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable Address Acknowledge */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);        /* 执行语句 */

      /* Process Unlocked */
      __HAL_UNLOCK(hi2c);                               /* HAL辅助宏/函数 */

      /* Note : The I2C interrupts must be enabled after unlocking current process
      to avoid the risk of I2C interrupt handle execution before current
      process unlock */
      /* Enable EVT and ERR interrupt */
      __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_ERR);  /* HAL辅助宏/函数 */

      /* Enable DMA Request */
      SET_BIT(hi2c->Instance->CR2, I2C_CR2_DMAEN);      /* 执行语句 */

      return HAL_OK;                                    /* 返回值 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Update I2C state */
      hi2c->State     = HAL_I2C_STATE_READY;            /* 赋值或初始化 */
      hi2c->Mode      = HAL_I2C_MODE_NONE;              /* 赋值或初始化 */

      /* Update I2C error code */
      hi2c->ErrorCode |= HAL_I2C_ERROR_DMA;             /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hi2c);                               /* HAL辅助宏/函数 */

      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Write an amount of data in blocking mode to a specific memory address
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  MemAddress Internal memory address
  * @param  MemAddSize Size of internal memory address
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Init tickstart for timeout management*/
  uint32_t tickstart = HAL_GetTick();                   /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_I2C_MEMADD_SIZE(MemAddSize));         /* 执行语句 */

  if (hi2c->State == HAL_I2C_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Wait until BUSY flag is reset */
    if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_BUSY, SET, I2C_TIMEOUT_BUSY_FLAG, tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_BUSY;                                  /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hi2c);                                   /* HAL辅助宏/函数 */

    /* Check if the I2C is already enabled */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable I2C peripheral */
      __HAL_I2C_ENABLE(hi2c);                           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Disable Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);        /* 执行语句 */

    hi2c->State     = HAL_I2C_STATE_BUSY_TX;            /* 赋值或初始化 */
    hi2c->Mode      = HAL_I2C_MODE_MEM;                 /* 赋值或初始化 */
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;               /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hi2c->pBuffPtr    = pData;                          /* 赋值或初始化 */
    hi2c->XferCount   = Size;                           /* 赋值或初始化 */
    hi2c->XferSize    = hi2c->XferCount;                /* 赋值或初始化 */
    hi2c->XferOptions = I2C_NO_OPTION_FRAME;            /* 赋值或初始化 */

    /* Send Slave Address and Memory Address */
    if (I2C_RequestMemoryWrite(hi2c, DevAddress, MemAddress, MemAddSize, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    while (hi2c->XferSize > 0U)                         /* while循环 */
    {                                                   /* 代码块开始 */
      /* Wait until TXE flag is set */
      if (I2C_WaitOnTXEFlagUntilTimeout(hi2c, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        if (hi2c->ErrorCode == HAL_I2C_ERROR_AF)        /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Generate Stop */
          SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);   /* 执行语句 */
        }                                               /* 代码块结束 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */

      /* Write data to DR */
      hi2c->Instance->DR = *hi2c->pBuffPtr;             /* 赋值或初始化 */

      /* Increment Buffer pointer */
      hi2c->pBuffPtr++;                                 /* 执行语句 */

      /* Update counter */
      hi2c->XferSize--;                                 /* 执行语句 */
      hi2c->XferCount--;                                /* 执行语句 */

      if ((__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BTF) == SET) && (hi2c->XferSize != 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Write data to DR */
        hi2c->Instance->DR = *hi2c->pBuffPtr;           /* 赋值或初始化 */

        /* Increment Buffer pointer */
        hi2c->pBuffPtr++;                               /* 执行语句 */

        /* Update counter */
        hi2c->XferSize--;                               /* 执行语句 */
        hi2c->XferCount--;                              /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Wait until BTF flag is set */
    if (I2C_WaitOnBTFFlagUntilTimeout(hi2c, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (hi2c->ErrorCode == HAL_I2C_ERROR_AF)          /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Generate Stop */
        SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);     /* 执行语句 */
      }                                                 /* 代码块结束 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Generate Stop */
    SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);         /* 执行语句 */

    hi2c->State = HAL_I2C_STATE_READY;                  /* 赋值或初始化 */
    hi2c->Mode = HAL_I2C_MODE_NONE;                     /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hi2c);                                 /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Read an amount of data in blocking mode from a specific memory address
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  MemAddress Internal memory address
  * @param  MemAddSize Size of internal memory address
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Init tickstart for timeout management*/
  uint32_t tickstart = HAL_GetTick();                   /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_I2C_MEMADD_SIZE(MemAddSize));         /* 执行语句 */

  if (hi2c->State == HAL_I2C_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Wait until BUSY flag is reset */
    if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_BUSY, SET, I2C_TIMEOUT_BUSY_FLAG, tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_BUSY;                                  /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hi2c);                                   /* HAL辅助宏/函数 */

    /* Check if the I2C is already enabled */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable I2C peripheral */
      __HAL_I2C_ENABLE(hi2c);                           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Disable Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);        /* 执行语句 */

    hi2c->State     = HAL_I2C_STATE_BUSY_RX;            /* 赋值或初始化 */
    hi2c->Mode      = HAL_I2C_MODE_MEM;                 /* 赋值或初始化 */
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;               /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hi2c->pBuffPtr    = pData;                          /* 赋值或初始化 */
    hi2c->XferCount   = Size;                           /* 赋值或初始化 */
    hi2c->XferSize    = hi2c->XferCount;                /* 赋值或初始化 */
    hi2c->XferOptions = I2C_NO_OPTION_FRAME;            /* 赋值或初始化 */

    /* Send Slave Address and Memory Address */
    if (I2C_RequestMemoryRead(hi2c, DevAddress, MemAddress, MemAddSize, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    if (hi2c->XferSize == 0U)                           /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear ADDR flag */
      __HAL_I2C_CLEAR_ADDRFLAG(hi2c);                   /* HAL辅助宏/函数 */

      /* Generate Stop */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);       /* 执行语句 */
    }                                                   /* 代码块结束 */
    else if (hi2c->XferSize == 1U)                      /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* Disable Acknowledge */
      CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);      /* 执行语句 */

      /* Clear ADDR flag */
      __HAL_I2C_CLEAR_ADDRFLAG(hi2c);                   /* HAL辅助宏/函数 */

      /* Generate Stop */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);       /* 执行语句 */
    }                                                   /* 代码块结束 */
    else if (hi2c->XferSize == 2U)                      /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* Disable Acknowledge */
      CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);      /* 执行语句 */

      /* Enable Pos */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_POS);        /* 执行语句 */

      /* Clear ADDR flag */
      __HAL_I2C_CLEAR_ADDRFLAG(hi2c);                   /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Clear ADDR flag */
      __HAL_I2C_CLEAR_ADDRFLAG(hi2c);                   /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    while (hi2c->XferSize > 0U)                         /* while循环 */
    {                                                   /* 代码块开始 */
      if (hi2c->XferSize <= 3U)                         /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* One byte */
        if (hi2c->XferSize == 1U)                       /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Wait until RXNE flag is set */
          if (I2C_WaitOnRXNEFlagUntilTimeout(hi2c, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
          {                                             /* 代码块开始 */
            return HAL_ERROR;                           /* 返回值 */
          }                                             /* 代码块结束 */

          /* Read data from DR */
          *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;  /* 赋值或初始化 */

          /* Increment Buffer pointer */
          hi2c->pBuffPtr++;                             /* 执行语句 */

          /* Update counter */
          hi2c->XferSize--;                             /* 执行语句 */
          hi2c->XferCount--;                            /* 执行语句 */
        }                                               /* 代码块结束 */
        /* Two bytes */
        else if (hi2c->XferSize == 2U)                  /* 否则如果 */
        {                                               /* 代码块开始 */
          /* Wait until BTF flag is set */
          if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_BTF, RESET, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
          {                                             /* 代码块开始 */
            return HAL_ERROR;                           /* 返回值 */
          }                                             /* 代码块结束 */

          /* Generate Stop */
          SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);   /* 执行语句 */

          /* Read data from DR */
          *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;  /* 赋值或初始化 */

          /* Increment Buffer pointer */
          hi2c->pBuffPtr++;                             /* 执行语句 */

          /* Update counter */
          hi2c->XferSize--;                             /* 执行语句 */
          hi2c->XferCount--;                            /* 执行语句 */

          /* Read data from DR */
          *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;  /* 赋值或初始化 */

          /* Increment Buffer pointer */
          hi2c->pBuffPtr++;                             /* 执行语句 */

          /* Update counter */
          hi2c->XferSize--;                             /* 执行语句 */
          hi2c->XferCount--;                            /* 执行语句 */
        }                                               /* 代码块结束 */
        /* 3 Last bytes */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          /* Wait until BTF flag is set */
          if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_BTF, RESET, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
          {                                             /* 代码块开始 */
            return HAL_ERROR;                           /* 返回值 */
          }                                             /* 代码块结束 */

          /* Disable Acknowledge */
          CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);  /* 执行语句 */

          /* Read data from DR */
          *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;  /* 赋值或初始化 */

          /* Increment Buffer pointer */
          hi2c->pBuffPtr++;                             /* 执行语句 */

          /* Update counter */
          hi2c->XferSize--;                             /* 执行语句 */
          hi2c->XferCount--;                            /* 执行语句 */

          /* Wait until BTF flag is set */
          if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_BTF, RESET, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
          {                                             /* 代码块开始 */
            return HAL_ERROR;                           /* 返回值 */
          }                                             /* 代码块结束 */

          /* Generate Stop */
          SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);   /* 执行语句 */

          /* Read data from DR */
          *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;  /* 赋值或初始化 */

          /* Increment Buffer pointer */
          hi2c->pBuffPtr++;                             /* 执行语句 */

          /* Update counter */
          hi2c->XferSize--;                             /* 执行语句 */
          hi2c->XferCount--;                            /* 执行语句 */

          /* Read data from DR */
          *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;  /* 赋值或初始化 */

          /* Increment Buffer pointer */
          hi2c->pBuffPtr++;                             /* 执行语句 */

          /* Update counter */
          hi2c->XferSize--;                             /* 执行语句 */
          hi2c->XferCount--;                            /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Wait until RXNE flag is set */
        if (I2C_WaitOnRXNEFlagUntilTimeout(hi2c, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
        {                                               /* 代码块开始 */
          return HAL_ERROR;                             /* 返回值 */
        }                                               /* 代码块结束 */

        /* Read data from DR */
        *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;  /* 赋值或初始化 */

        /* Increment Buffer pointer */
        hi2c->pBuffPtr++;                               /* 执行语句 */

        /* Update counter */
        hi2c->XferSize--;                               /* 执行语句 */
        hi2c->XferCount--;                              /* 执行语句 */

        if (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BTF) == SET)  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Read data from DR */
          *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;  /* 赋值或初始化 */

          /* Increment Buffer pointer */
          hi2c->pBuffPtr++;                             /* 执行语句 */

          /* Update counter */
          hi2c->XferSize--;                             /* 执行语句 */
          hi2c->XferCount--;                            /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    hi2c->State = HAL_I2C_STATE_READY;                  /* 赋值或初始化 */
    hi2c->Mode = HAL_I2C_MODE_NONE;                     /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hi2c);                                 /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Write an amount of data in non-blocking mode with Interrupt to a specific memory address
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  MemAddress Internal memory address
  * @param  MemAddSize Size of internal memory address
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Mem_Write_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  __IO uint32_t count = 0U;                             /* 赋值或初始化 */

  /* Check the parameters */
  assert_param(IS_I2C_MEMADD_SIZE(MemAddSize));         /* 执行语句 */

  if (hi2c->State == HAL_I2C_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Wait until BUSY flag is reset */
    count = I2C_TIMEOUT_BUSY_FLAG * (SystemCoreClock / 25U / 1000U);  /* 赋值或初始化 */
    do                                                  /* do-while循环 */
    {                                                   /* 代码块开始 */
      count--;                                          /* 执行语句 */
      if (count == 0U)                                  /* 条件判断 */
      {                                                 /* 代码块开始 */
        hi2c->PreviousState       = I2C_STATE_NONE;     /* 赋值或初始化 */
        hi2c->State               = HAL_I2C_STATE_READY;  /* 赋值或初始化 */
        hi2c->Mode                = HAL_I2C_MODE_NONE;  /* 赋值或初始化 */
        hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;  /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);                             /* HAL辅助宏/函数 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    while (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BUSY) != RESET);  /* while循环 */

    /* Process Locked */
    __HAL_LOCK(hi2c);                                   /* HAL辅助宏/函数 */

    /* Check if the I2C is already enabled */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable I2C peripheral */
      __HAL_I2C_ENABLE(hi2c);                           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Disable Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);        /* 执行语句 */

    hi2c->State     = HAL_I2C_STATE_BUSY_TX;            /* 赋值或初始化 */
    hi2c->Mode      = HAL_I2C_MODE_MEM;                 /* 赋值或初始化 */
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;               /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hi2c->pBuffPtr    = pData;                          /* 赋值或初始化 */
    hi2c->XferCount   = Size;                           /* 赋值或初始化 */
    hi2c->XferSize    = hi2c->XferCount;                /* 赋值或初始化 */
    hi2c->XferOptions = I2C_NO_OPTION_FRAME;            /* 赋值或初始化 */
    hi2c->Devaddress  = DevAddress;                     /* 赋值或初始化 */
    hi2c->Memaddress  = MemAddress;                     /* 赋值或初始化 */
    hi2c->MemaddSize  = MemAddSize;                     /* 赋值或初始化 */
    hi2c->EventCount  = 0U;                             /* 赋值或初始化 */

    /* Generate Start */
    SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);        /* 执行语句 */

    /* Process Unlocked */
    __HAL_UNLOCK(hi2c);                                 /* HAL辅助宏/函数 */

    /* Note : The I2C interrupts must be enabled after unlocking current process
    to avoid the risk of I2C interrupt handle execution before current
    process unlock */

    /* Enable EVT, BUF and ERR interrupt */
    __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);  /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Read an amount of data in non-blocking mode with Interrupt from a specific memory address
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  DevAddress Target device address
  * @param  MemAddress Internal memory address
  * @param  MemAddSize Size of internal memory address
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Mem_Read_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  __IO uint32_t count = 0U;                             /* 赋值或初始化 */

  /* Check the parameters */
  assert_param(IS_I2C_MEMADD_SIZE(MemAddSize));         /* 执行语句 */

  if (hi2c->State == HAL_I2C_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Wait until BUSY flag is reset */
    count = I2C_TIMEOUT_BUSY_FLAG * (SystemCoreClock / 25U / 1000U);  /* 赋值或初始化 */
    do                                                  /* do-while循环 */
    {                                                   /* 代码块开始 */
      count--;                                          /* 执行语句 */
      if (count == 0U)                                  /* 条件判断 */
      {                                                 /* 代码块开始 */
        hi2c->PreviousState       = I2C_STATE_NONE;     /* 赋值或初始化 */
        hi2c->State               = HAL_I2C_STATE_READY;  /* 赋值或初始化 */
        hi2c->Mode                = HAL_I2C_MODE_NONE;  /* 赋值或初始化 */
        hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;  /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);                             /* HAL辅助宏/函数 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    while (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BUSY) != RESET);  /* while循环 */

    /* Process Locked */
    __HAL_LOCK(hi2c);                                   /* HAL辅助宏/函数 */

    /* Check if the I2C is already enabled */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable I2C peripheral */
      __HAL_I2C_ENABLE(hi2c);                           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Disable Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);        /* 执行语句 */

    hi2c->State     = HAL_I2C_STATE_BUSY_RX;            /* 赋值或初始化 */
    hi2c->Mode      = HAL_I2C_MODE_MEM;                 /* 赋值或初始化 */
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;               /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hi2c->pBuffPtr    = pData;                          /* 赋值或初始化 */
    hi2c->XferCount   = Size;                           /* 赋值或初始化 */
    hi2c->XferSize    = hi2c->XferCount;                /* 赋值或初始化 */
    hi2c->XferOptions = I2C_NO_OPTION_FRAME;            /* 赋值或初始化 */
    hi2c->Devaddress  = DevAddress;                     /* 赋值或初始化 */
    hi2c->Memaddress  = MemAddress;                     /* 赋值或初始化 */
    hi2c->MemaddSize  = MemAddSize;                     /* 赋值或初始化 */
    hi2c->EventCount  = 0U;                             /* 赋值或初始化 */

    /* Enable Acknowledge */
    SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);          /* 执行语句 */

    /* Generate Start */
    SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);        /* 执行语句 */

    /* Process Unlocked */
    __HAL_UNLOCK(hi2c);                                 /* HAL辅助宏/函数 */

    if (hi2c->XferSize > 0U)                            /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Note : The I2C interrupts must be enabled after unlocking current process
      to avoid the risk of I2C interrupt handle execution before current
      process unlock */

      /* Enable EVT, BUF and ERR interrupt */
      __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Write an amount of data in non-blocking mode with DMA to a specific memory address
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  MemAddress Internal memory address
  * @param  MemAddSize Size of internal memory address
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Mem_Write_DMA(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  __IO uint32_t count = 0U;                             /* 赋值或初始化 */
  HAL_StatusTypeDef dmaxferstatus;                      /* HAL状态类型函数 */

  /* Init tickstart for timeout management*/
  uint32_t tickstart = HAL_GetTick();                   /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_I2C_MEMADD_SIZE(MemAddSize));         /* 执行语句 */

  if (hi2c->State == HAL_I2C_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Wait until BUSY flag is reset */
    count = I2C_TIMEOUT_BUSY_FLAG * (SystemCoreClock / 25U / 1000U);  /* 赋值或初始化 */
    do                                                  /* do-while循环 */
    {                                                   /* 代码块开始 */
      count--;                                          /* 执行语句 */
      if (count == 0U)                                  /* 条件判断 */
      {                                                 /* 代码块开始 */
        hi2c->PreviousState       = I2C_STATE_NONE;     /* 赋值或初始化 */
        hi2c->State               = HAL_I2C_STATE_READY;  /* 赋值或初始化 */
        hi2c->Mode                = HAL_I2C_MODE_NONE;  /* 赋值或初始化 */
        hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;  /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);                             /* HAL辅助宏/函数 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    while (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BUSY) != RESET);  /* while循环 */

    /* Process Locked */
    __HAL_LOCK(hi2c);                                   /* HAL辅助宏/函数 */

    /* Check if the I2C is already enabled */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable I2C peripheral */
      __HAL_I2C_ENABLE(hi2c);                           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Disable Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);        /* 执行语句 */

    hi2c->State     = HAL_I2C_STATE_BUSY_TX;            /* 赋值或初始化 */
    hi2c->Mode      = HAL_I2C_MODE_MEM;                 /* 赋值或初始化 */
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;               /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hi2c->pBuffPtr    = pData;                          /* 赋值或初始化 */
    hi2c->XferCount   = Size;                           /* 赋值或初始化 */
    hi2c->XferSize    = hi2c->XferCount;                /* 赋值或初始化 */
    hi2c->XferOptions = I2C_NO_OPTION_FRAME;            /* 赋值或初始化 */

    if (hi2c->XferSize > 0U)                            /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (hi2c->hdmatx != NULL)                         /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Set the I2C DMA transfer complete callback */
        hi2c->hdmatx->XferCpltCallback = I2C_DMAXferCplt;  /* 赋值或初始化 */

        /* Set the DMA error callback */
        hi2c->hdmatx->XferErrorCallback = I2C_DMAError;  /* 赋值或初始化 */

        /* Set the unused DMA callbacks to NULL */
        hi2c->hdmatx->XferHalfCpltCallback = NULL;      /* 赋值或初始化 */
        hi2c->hdmatx->XferM1CpltCallback = NULL;        /* 赋值或初始化 */
        hi2c->hdmatx->XferM1HalfCpltCallback = NULL;    /* 赋值或初始化 */
        hi2c->hdmatx->XferAbortCallback = NULL;         /* 赋值或初始化 */

        /* Enable the DMA stream */
        dmaxferstatus = HAL_DMA_Start_IT(hi2c->hdmatx, (uint32_t)hi2c->pBuffPtr, (uint32_t)&hi2c->Instance->DR, hi2c->XferSize);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Update I2C state */
        hi2c->State     = HAL_I2C_STATE_READY;          /* 赋值或初始化 */
        hi2c->Mode      = HAL_I2C_MODE_NONE;            /* 赋值或初始化 */

        /* Update I2C error code */
        hi2c->ErrorCode |= HAL_I2C_ERROR_DMA_PARAM;     /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);                             /* HAL辅助宏/函数 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */

      if (dmaxferstatus == HAL_OK)                      /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Send Slave Address and Memory Address */
        if (I2C_RequestMemoryWrite(hi2c, DevAddress, MemAddress, MemAddSize, I2C_TIMEOUT_FLAG, tickstart) != HAL_OK)  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Abort the ongoing DMA */
          dmaxferstatus = HAL_DMA_Abort_IT(hi2c->hdmatx);  /* 赋值或初始化 */

          /* Prevent unused argument(s) compilation and MISRA warning */
          UNUSED(dmaxferstatus);                        /* 执行语句 */

          /* Set the unused I2C DMA transfer complete callback to NULL */
          hi2c->hdmatx->XferCpltCallback = NULL;        /* 赋值或初始化 */

          /* Disable Acknowledge */
          CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);  /* 执行语句 */

          hi2c->XferSize = 0U;                          /* 赋值或初始化 */
          hi2c->XferCount = 0U;                         /* 赋值或初始化 */

          /* Disable I2C peripheral to prevent dummy data in buffer */
          __HAL_I2C_DISABLE(hi2c);                      /* HAL辅助宏/函数 */

          return HAL_ERROR;                             /* 返回值 */
        }                                               /* 代码块结束 */

        /* Clear ADDR flag */
        __HAL_I2C_CLEAR_ADDRFLAG(hi2c);                 /* HAL辅助宏/函数 */

        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);                             /* HAL辅助宏/函数 */

        /* Note : The I2C interrupts must be enabled after unlocking current process
        to avoid the risk of I2C interrupt handle execution before current
        process unlock */
        /* Enable ERR interrupt */
        __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_ERR);          /* HAL辅助宏/函数 */

        /* Enable DMA Request */
        SET_BIT(hi2c->Instance->CR2, I2C_CR2_DMAEN);    /* 执行语句 */

        return HAL_OK;                                  /* 返回值 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Update I2C state */
        hi2c->State     = HAL_I2C_STATE_READY;          /* 赋值或初始化 */
        hi2c->Mode      = HAL_I2C_MODE_NONE;            /* 赋值或初始化 */

        /* Update I2C error code */
        hi2c->ErrorCode |= HAL_I2C_ERROR_DMA;           /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);                             /* HAL辅助宏/函数 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Update I2C state */
      hi2c->State     = HAL_I2C_STATE_READY;            /* 赋值或初始化 */
      hi2c->Mode      = HAL_I2C_MODE_NONE;              /* 赋值或初始化 */

      /* Update I2C error code */
      hi2c->ErrorCode |= HAL_I2C_ERROR_SIZE;            /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hi2c);                               /* HAL辅助宏/函数 */

      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Reads an amount of data in non-blocking mode with DMA from a specific memory address.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  MemAddress Internal memory address
  * @param  MemAddSize Size of internal memory address
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be read
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Mem_Read_DMA(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Init tickstart for timeout management*/
  uint32_t tickstart = HAL_GetTick();                   /* 32位无符号变量/函数 */
  __IO uint32_t count = 0U;                             /* 赋值或初始化 */
  HAL_StatusTypeDef dmaxferstatus;                      /* HAL状态类型函数 */

  /* Check the parameters */
  assert_param(IS_I2C_MEMADD_SIZE(MemAddSize));         /* 执行语句 */

  if (hi2c->State == HAL_I2C_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Wait until BUSY flag is reset */
    count = I2C_TIMEOUT_BUSY_FLAG * (SystemCoreClock / 25U / 1000U);  /* 赋值或初始化 */
    do                                                  /* do-while循环 */
    {                                                   /* 代码块开始 */
      count--;                                          /* 执行语句 */
      if (count == 0U)                                  /* 条件判断 */
      {                                                 /* 代码块开始 */
        hi2c->PreviousState       = I2C_STATE_NONE;     /* 赋值或初始化 */
        hi2c->State               = HAL_I2C_STATE_READY;  /* 赋值或初始化 */
        hi2c->Mode                = HAL_I2C_MODE_NONE;  /* 赋值或初始化 */
        hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;  /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);                             /* HAL辅助宏/函数 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    while (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BUSY) != RESET);  /* while循环 */

    /* Process Locked */
    __HAL_LOCK(hi2c);                                   /* HAL辅助宏/函数 */

    /* Check if the I2C is already enabled */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable I2C peripheral */
      __HAL_I2C_ENABLE(hi2c);                           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Disable Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);        /* 执行语句 */

    hi2c->State     = HAL_I2C_STATE_BUSY_RX;            /* 赋值或初始化 */
    hi2c->Mode      = HAL_I2C_MODE_MEM;                 /* 赋值或初始化 */
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;               /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hi2c->pBuffPtr    = pData;                          /* 赋值或初始化 */
    hi2c->XferCount   = Size;                           /* 赋值或初始化 */
    hi2c->XferSize    = hi2c->XferCount;                /* 赋值或初始化 */
    hi2c->XferOptions = I2C_NO_OPTION_FRAME;            /* 赋值或初始化 */

    if (hi2c->XferSize > 0U)                            /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (hi2c->hdmarx != NULL)                         /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Set the I2C DMA transfer complete callback */
        hi2c->hdmarx->XferCpltCallback = I2C_DMAXferCplt;  /* 赋值或初始化 */

        /* Set the DMA error callback */
        hi2c->hdmarx->XferErrorCallback = I2C_DMAError;  /* 赋值或初始化 */

        /* Set the unused DMA callbacks to NULL */
        hi2c->hdmarx->XferHalfCpltCallback = NULL;      /* 赋值或初始化 */
        hi2c->hdmarx->XferM1CpltCallback = NULL;        /* 赋值或初始化 */
        hi2c->hdmarx->XferM1HalfCpltCallback = NULL;    /* 赋值或初始化 */
        hi2c->hdmarx->XferAbortCallback = NULL;         /* 赋值或初始化 */

        /* Enable the DMA stream */
        dmaxferstatus = HAL_DMA_Start_IT(hi2c->hdmarx, (uint32_t)&hi2c->Instance->DR, (uint32_t)hi2c->pBuffPtr, hi2c->XferSize);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Update I2C state */
        hi2c->State     = HAL_I2C_STATE_READY;          /* 赋值或初始化 */
        hi2c->Mode      = HAL_I2C_MODE_NONE;            /* 赋值或初始化 */

        /* Update I2C error code */
        hi2c->ErrorCode |= HAL_I2C_ERROR_DMA_PARAM;     /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);                             /* HAL辅助宏/函数 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */

      if (dmaxferstatus == HAL_OK)                      /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Send Slave Address and Memory Address */
        if (I2C_RequestMemoryRead(hi2c, DevAddress, MemAddress, MemAddSize, I2C_TIMEOUT_FLAG, tickstart) != HAL_OK)  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Abort the ongoing DMA */
          dmaxferstatus = HAL_DMA_Abort_IT(hi2c->hdmarx);  /* 赋值或初始化 */

          /* Prevent unused argument(s) compilation and MISRA warning */
          UNUSED(dmaxferstatus);                        /* 执行语句 */

          /* Set the unused I2C DMA transfer complete callback to NULL */
          hi2c->hdmarx->XferCpltCallback = NULL;        /* 赋值或初始化 */

          /* Disable Acknowledge */
          CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);  /* 执行语句 */

          hi2c->XferSize = 0U;                          /* 赋值或初始化 */
          hi2c->XferCount = 0U;                         /* 赋值或初始化 */

          /* Disable I2C peripheral to prevent dummy data in buffer */
          __HAL_I2C_DISABLE(hi2c);                      /* HAL辅助宏/函数 */

          return HAL_ERROR;                             /* 返回值 */
        }                                               /* 代码块结束 */

        if (hi2c->XferSize == 1U)                       /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Disable Acknowledge */
          CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);  /* 执行语句 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          /* Enable Last DMA bit */
          SET_BIT(hi2c->Instance->CR2, I2C_CR2_LAST);   /* 执行语句 */
        }                                               /* 代码块结束 */

        /* Clear ADDR flag */
        __HAL_I2C_CLEAR_ADDRFLAG(hi2c);                 /* HAL辅助宏/函数 */

        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);                             /* HAL辅助宏/函数 */

        /* Note : The I2C interrupts must be enabled after unlocking current process
        to avoid the risk of I2C interrupt handle execution before current
        process unlock */
        /* Enable ERR interrupt */
        __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_ERR);          /* HAL辅助宏/函数 */

        /* Enable DMA Request */
        hi2c->Instance->CR2 |= I2C_CR2_DMAEN;           /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Update I2C state */
        hi2c->State     = HAL_I2C_STATE_READY;          /* 赋值或初始化 */
        hi2c->Mode      = HAL_I2C_MODE_NONE;            /* 赋值或初始化 */

        /* Update I2C error code */
        hi2c->ErrorCode |= HAL_I2C_ERROR_DMA;           /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);                             /* HAL辅助宏/函数 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Send Slave Address and Memory Address */
      if (I2C_RequestMemoryRead(hi2c, DevAddress, MemAddress, MemAddSize, I2C_TIMEOUT_FLAG, tickstart) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */

      /* Clear ADDR flag */
      __HAL_I2C_CLEAR_ADDRFLAG(hi2c);                   /* HAL辅助宏/函数 */

      /* Generate Stop */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);       /* 执行语句 */

      hi2c->State = HAL_I2C_STATE_READY;                /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hi2c);                               /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Checks if target device is ready for communication.
  * @note   This function is used with Memory devices
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  Trials Number of trials
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_IsDeviceReady(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint32_t Trials, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Get tick */
  uint32_t tickstart = HAL_GetTick();                   /* 32位无符号变量/函数 */
  uint32_t I2C_Trials = 1U;                             /* 32位无符号变量/函数 */
  FlagStatus tmp1;                                      /* 执行语句 */
  FlagStatus tmp2;                                      /* 执行语句 */

  if (hi2c->State == HAL_I2C_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Wait until BUSY flag is reset */
    if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_BUSY, SET, I2C_TIMEOUT_BUSY_FLAG, tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_BUSY;                                  /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hi2c);                                   /* HAL辅助宏/函数 */

    /* Check if the I2C is already enabled */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable I2C peripheral */
      __HAL_I2C_ENABLE(hi2c);                           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Disable Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);        /* 执行语句 */

    hi2c->State = HAL_I2C_STATE_BUSY;                   /* 赋值或初始化 */
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;               /* 赋值或初始化 */
    hi2c->XferOptions = I2C_NO_OPTION_FRAME;            /* 赋值或初始化 */

    do                                                  /* do-while循环 */
    {                                                   /* 代码块开始 */
      /* Generate Start */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);      /* 执行语句 */

      /* Wait until SB flag is set */
      if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_SB, RESET, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        if (READ_BIT(hi2c->Instance->CR1, I2C_CR1_START) == I2C_CR1_START)  /* 条件判断 */
        {                                               /* 代码块开始 */
          hi2c->ErrorCode = HAL_I2C_WRONG_START;        /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */

      /* Send slave address */
      hi2c->Instance->DR = I2C_7BIT_ADD_WRITE(DevAddress);  /* 赋值或初始化 */

      /* Wait until ADDR or AF flag are set */
      /* Get tick */
      tickstart = HAL_GetTick();                        /* 赋值或初始化 */

      tmp1 = __HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_ADDR);   /* 赋值或初始化 */
      tmp2 = __HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_AF);     /* 赋值或初始化 */
      while ((hi2c->State != HAL_I2C_STATE_TIMEOUT) && (tmp1 == RESET) && (tmp2 == RESET))  /* while循环 */
      {                                                 /* 代码块开始 */
        if (((HAL_GetTick() - tickstart) > Timeout) || (Timeout == 0U))  /* 条件判断 */
        {                                               /* 代码块开始 */
          hi2c->State = HAL_I2C_STATE_TIMEOUT;          /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        tmp1 = __HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_ADDR);  /* 赋值或初始化 */
        tmp2 = __HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_AF);   /* 赋值或初始化 */
      }                                                 /* 代码块结束 */

      hi2c->State = HAL_I2C_STATE_READY;                /* 赋值或初始化 */

      /* Check if the ADDR flag has been set */
      if (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_ADDR) == SET)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Generate Stop */
        SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);     /* 执行语句 */

        /* Clear ADDR Flag */
        __HAL_I2C_CLEAR_ADDRFLAG(hi2c);                 /* HAL辅助宏/函数 */

        /* Wait until BUSY flag is reset */
        if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_BUSY, SET, I2C_TIMEOUT_BUSY_FLAG, tickstart) != HAL_OK)  /* 条件判断 */
        {                                               /* 代码块开始 */
          return HAL_ERROR;                             /* 返回值 */
        }                                               /* 代码块结束 */

        hi2c->State = HAL_I2C_STATE_READY;              /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);                             /* HAL辅助宏/函数 */

        return HAL_OK;                                  /* 返回值 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Generate Stop */
        SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);     /* 执行语句 */

        /* Clear AF Flag */
        __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_AF);        /* HAL辅助宏/函数 */

        /* Wait until BUSY flag is reset */
        if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_BUSY, SET, I2C_TIMEOUT_BUSY_FLAG, tickstart) != HAL_OK)  /* 条件判断 */
        {                                               /* 代码块开始 */
          return HAL_ERROR;                             /* 返回值 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */

      /* Increment Trials */
      I2C_Trials++;                                     /* 执行语句 */
    }                                                   /* 代码块结束 */
    while (I2C_Trials < Trials);                        /* while循环 */

    hi2c->State = HAL_I2C_STATE_READY;                  /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hi2c);                                 /* HAL辅助宏/函数 */

    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Sequential transmit in master I2C mode an amount of data in non-blocking mode with Interrupt.
  * @note   This interface allow to manage repeated start condition when a direction change during transfer
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for the specified I2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  XferOptions Options of Transfer, value of @ref I2C_XferOptions_definition
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Master_Seq_Transmit_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t XferOptions)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  __IO uint32_t Prev_State = 0x00U;                     /* 赋值或初始化 */
  __IO uint32_t count      = 0x00U;                     /* 赋值或初始化 */

  /* Check the parameters */
  assert_param(IS_I2C_TRANSFER_OPTIONS_REQUEST(XferOptions));  /* 执行语句 */

  if (hi2c->State == HAL_I2C_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check Busy Flag only if FIRST call of Master interface */
    if ((READ_BIT(hi2c->Instance->CR1, I2C_CR1_STOP) == I2C_CR1_STOP) || (XferOptions == I2C_FIRST_AND_LAST_FRAME) || (XferOptions == I2C_FIRST_FRAME))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Wait until BUSY flag is reset */
      count = I2C_TIMEOUT_BUSY_FLAG * (SystemCoreClock / 25U / 1000U);  /* 赋值或初始化 */
      do                                                /* do-while循环 */
      {                                                 /* 代码块开始 */
        count--;                                        /* 执行语句 */
        if (count == 0U)                                /* 条件判断 */
        {                                               /* 代码块开始 */
          hi2c->PreviousState       = I2C_STATE_NONE;   /* 赋值或初始化 */
          hi2c->State               = HAL_I2C_STATE_READY;  /* 赋值或初始化 */
          hi2c->Mode                = HAL_I2C_MODE_NONE;  /* 赋值或初始化 */
          hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;  /* 赋值或初始化 */

          /* Process Unlocked */
          __HAL_UNLOCK(hi2c);                           /* HAL辅助宏/函数 */

          return HAL_ERROR;                             /* 返回值 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      while (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BUSY) != RESET);  /* while循环 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hi2c);                                   /* HAL辅助宏/函数 */

    /* Check if the I2C is already enabled */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable I2C peripheral */
      __HAL_I2C_ENABLE(hi2c);                           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Disable Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);        /* 执行语句 */

    hi2c->State     = HAL_I2C_STATE_BUSY_TX;            /* 赋值或初始化 */
    hi2c->Mode      = HAL_I2C_MODE_MASTER;              /* 赋值或初始化 */
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;               /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hi2c->pBuffPtr    = pData;                          /* 赋值或初始化 */
    hi2c->XferCount   = Size;                           /* 赋值或初始化 */
    hi2c->XferSize    = hi2c->XferCount;                /* 赋值或初始化 */
    hi2c->XferOptions = XferOptions;                    /* 赋值或初始化 */
    hi2c->Devaddress  = DevAddress;                     /* 赋值或初始化 */

    Prev_State = hi2c->PreviousState;                   /* 赋值或初始化 */

    /* If transfer direction not change and there is no request to start another frame, do not generate Restart Condition */
    /* Mean Previous state is same as current state */
    if ((Prev_State != I2C_STATE_MASTER_BUSY_TX) || (IS_I2C_TRANSFER_OTHER_OPTIONS_REQUEST(XferOptions) == 1))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Generate Start */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);      /* 执行语句 */
    }                                                   /* 代码块结束 */

    /* Process Unlocked */
    __HAL_UNLOCK(hi2c);                                 /* HAL辅助宏/函数 */

    /* Note : The I2C interrupts must be enabled after unlocking current process
    to avoid the risk of I2C interrupt handle execution before current
    process unlock */

    /* Enable EVT, BUF and ERR interrupt */
    __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);  /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Sequential transmit in master I2C mode an amount of data in non-blocking mode with DMA.
  * @note   This interface allow to manage repeated start condition when a direction change during transfer
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for the specified I2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  XferOptions Options of Transfer, value of @ref I2C_XferOptions_definition
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Master_Seq_Transmit_DMA(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t XferOptions)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  __IO uint32_t Prev_State = 0x00U;                     /* 赋值或初始化 */
  __IO uint32_t count      = 0x00U;                     /* 赋值或初始化 */
  HAL_StatusTypeDef dmaxferstatus;                      /* HAL状态类型函数 */

  /* Check the parameters */
  assert_param(IS_I2C_TRANSFER_OPTIONS_REQUEST(XferOptions));  /* 执行语句 */

  if (hi2c->State == HAL_I2C_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check Busy Flag only if FIRST call of Master interface */
    if ((READ_BIT(hi2c->Instance->CR1, I2C_CR1_STOP) == I2C_CR1_STOP) || (XferOptions == I2C_FIRST_AND_LAST_FRAME) || (XferOptions == I2C_FIRST_FRAME))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Wait until BUSY flag is reset */
      count = I2C_TIMEOUT_BUSY_FLAG * (SystemCoreClock / 25U / 1000U);  /* 赋值或初始化 */
      do                                                /* do-while循环 */
      {                                                 /* 代码块开始 */
        count--;                                        /* 执行语句 */
        if (count == 0U)                                /* 条件判断 */
        {                                               /* 代码块开始 */
          hi2c->PreviousState       = I2C_STATE_NONE;   /* 赋值或初始化 */
          hi2c->State               = HAL_I2C_STATE_READY;  /* 赋值或初始化 */
          hi2c->Mode                = HAL_I2C_MODE_NONE;  /* 赋值或初始化 */
          hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;  /* 赋值或初始化 */

          /* Process Unlocked */
          __HAL_UNLOCK(hi2c);                           /* HAL辅助宏/函数 */

          return HAL_ERROR;                             /* 返回值 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      while (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BUSY) != RESET);  /* while循环 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hi2c);                                   /* HAL辅助宏/函数 */

    /* Check if the I2C is already enabled */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable I2C peripheral */
      __HAL_I2C_ENABLE(hi2c);                           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Disable Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);        /* 执行语句 */

    hi2c->State     = HAL_I2C_STATE_BUSY_TX;            /* 赋值或初始化 */
    hi2c->Mode      = HAL_I2C_MODE_MASTER;              /* 赋值或初始化 */
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;               /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hi2c->pBuffPtr    = pData;                          /* 赋值或初始化 */
    hi2c->XferCount   = Size;                           /* 赋值或初始化 */
    hi2c->XferSize    = hi2c->XferCount;                /* 赋值或初始化 */
    hi2c->XferOptions = XferOptions;                    /* 赋值或初始化 */
    hi2c->Devaddress  = DevAddress;                     /* 赋值或初始化 */

    Prev_State = hi2c->PreviousState;                   /* 赋值或初始化 */

    if (hi2c->XferSize > 0U)                            /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (hi2c->hdmatx != NULL)                         /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Set the I2C DMA transfer complete callback */
        hi2c->hdmatx->XferCpltCallback = I2C_DMAXferCplt;  /* 赋值或初始化 */

        /* Set the DMA error callback */
        hi2c->hdmatx->XferErrorCallback = I2C_DMAError;  /* 赋值或初始化 */

        /* Set the unused DMA callbacks to NULL */
        hi2c->hdmatx->XferHalfCpltCallback = NULL;      /* 赋值或初始化 */
        hi2c->hdmatx->XferAbortCallback = NULL;         /* 赋值或初始化 */

        /* Enable the DMA stream */
        dmaxferstatus = HAL_DMA_Start_IT(hi2c->hdmatx, (uint32_t)hi2c->pBuffPtr, (uint32_t)&hi2c->Instance->DR, hi2c->XferSize);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Update I2C state */
        hi2c->State     = HAL_I2C_STATE_READY;          /* 赋值或初始化 */
        hi2c->Mode      = HAL_I2C_MODE_NONE;            /* 赋值或初始化 */

        /* Update I2C error code */
        hi2c->ErrorCode |= HAL_I2C_ERROR_DMA_PARAM;     /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);                             /* HAL辅助宏/函数 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */

      if (dmaxferstatus == HAL_OK)                      /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Enable Acknowledge */
        SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);      /* 执行语句 */

        /* If transfer direction not change and there is no request to start another frame, do not generate Restart Condition */
        /* Mean Previous state is same as current state */
        if ((Prev_State != I2C_STATE_MASTER_BUSY_TX) || (IS_I2C_TRANSFER_OTHER_OPTIONS_REQUEST(XferOptions) == 1))  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Generate Start */
          SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);  /* 执行语句 */
        }                                               /* 代码块结束 */

        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);                             /* HAL辅助宏/函数 */

        /* Note : The I2C interrupts must be enabled after unlocking current process
        to avoid the risk of I2C interrupt handle execution before current
        process unlock */

        /* If XferOptions is not associated to a new frame, mean no start bit is request, enable directly the DMA request */
        /* In other cases, DMA request is enabled after Slave address treatment in IRQHandler */
        if ((XferOptions == I2C_NEXT_FRAME) || (XferOptions == I2C_LAST_FRAME) || (XferOptions == I2C_LAST_FRAME_NO_STOP))  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Enable DMA Request */
          SET_BIT(hi2c->Instance->CR2, I2C_CR2_DMAEN);  /* 执行语句 */
        }                                               /* 代码块结束 */

        /* Enable EVT and ERR interrupt */
        __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_ERR);  /* HAL辅助宏/函数 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Update I2C state */
        hi2c->State     = HAL_I2C_STATE_READY;          /* 赋值或初始化 */
        hi2c->Mode      = HAL_I2C_MODE_NONE;            /* 赋值或初始化 */

        /* Update I2C error code */
        hi2c->ErrorCode |= HAL_I2C_ERROR_DMA;           /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);                             /* HAL辅助宏/函数 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Enable Acknowledge */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);        /* 执行语句 */

      /* If transfer direction not change and there is no request to start another frame, do not generate Restart Condition */
      /* Mean Previous state is same as current state */
      if ((Prev_State != I2C_STATE_MASTER_BUSY_TX) || (IS_I2C_TRANSFER_OTHER_OPTIONS_REQUEST(XferOptions) == 1))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Generate Start */
        SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);    /* 执行语句 */
      }                                                 /* 代码块结束 */

      /* Process Unlocked */
      __HAL_UNLOCK(hi2c);                               /* HAL辅助宏/函数 */

      /* Note : The I2C interrupts must be enabled after unlocking current process
      to avoid the risk of I2C interrupt handle execution before current
      process unlock */

      /* Enable EVT, BUF and ERR interrupt */
      __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Sequential receive in master I2C mode an amount of data in non-blocking mode with Interrupt
  * @note   This interface allow to manage repeated start condition when a direction change during transfer
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for the specified I2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  XferOptions Options of Transfer, value of @ref I2C_XferOptions_definition
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Master_Seq_Receive_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t XferOptions)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  __IO uint32_t Prev_State = 0x00U;                     /* 赋值或初始化 */
  __IO uint32_t count = 0U;                             /* 赋值或初始化 */
  uint32_t enableIT = (I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);  /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_I2C_TRANSFER_OPTIONS_REQUEST(XferOptions));  /* 执行语句 */

  if (hi2c->State == HAL_I2C_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check Busy Flag only if FIRST call of Master interface */
    if ((READ_BIT(hi2c->Instance->CR1, I2C_CR1_STOP) == I2C_CR1_STOP) || (XferOptions == I2C_FIRST_AND_LAST_FRAME) || (XferOptions == I2C_FIRST_FRAME))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Wait until BUSY flag is reset */
      count = I2C_TIMEOUT_BUSY_FLAG * (SystemCoreClock / 25U / 1000U);  /* 赋值或初始化 */
      do                                                /* do-while循环 */
      {                                                 /* 代码块开始 */
        count--;                                        /* 执行语句 */
        if (count == 0U)                                /* 条件判断 */
        {                                               /* 代码块开始 */
          hi2c->PreviousState       = I2C_STATE_NONE;   /* 赋值或初始化 */
          hi2c->State               = HAL_I2C_STATE_READY;  /* 赋值或初始化 */
          hi2c->Mode                = HAL_I2C_MODE_NONE;  /* 赋值或初始化 */
          hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;  /* 赋值或初始化 */

          /* Process Unlocked */
          __HAL_UNLOCK(hi2c);                           /* HAL辅助宏/函数 */

          return HAL_ERROR;                             /* 返回值 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      while (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BUSY) != RESET);  /* while循环 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hi2c);                                   /* HAL辅助宏/函数 */

    /* Check if the I2C is already enabled */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable I2C peripheral */
      __HAL_I2C_ENABLE(hi2c);                           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Disable Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);        /* 执行语句 */

    hi2c->State     = HAL_I2C_STATE_BUSY_RX;            /* 赋值或初始化 */
    hi2c->Mode      = HAL_I2C_MODE_MASTER;              /* 赋值或初始化 */
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;               /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hi2c->pBuffPtr    = pData;                          /* 赋值或初始化 */
    hi2c->XferCount   = Size;                           /* 赋值或初始化 */
    hi2c->XferSize    = hi2c->XferCount;                /* 赋值或初始化 */
    hi2c->XferOptions = XferOptions;                    /* 赋值或初始化 */
    hi2c->Devaddress  = DevAddress;                     /* 赋值或初始化 */

    Prev_State = hi2c->PreviousState;                   /* 赋值或初始化 */

    if ((hi2c->XferCount == 2U) && ((XferOptions == I2C_LAST_FRAME) || (XferOptions == I2C_LAST_FRAME_NO_STOP)))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (Prev_State == I2C_STATE_MASTER_BUSY_RX)       /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Disable Acknowledge */
        CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);    /* 执行语句 */

        /* Enable Pos */
        SET_BIT(hi2c->Instance->CR1, I2C_CR1_POS);      /* 执行语句 */

        /* Remove Enabling of IT_BUF, mean RXNE treatment, treat the 2 bytes through BTF */
        enableIT &= ~I2C_IT_BUF;                        /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Enable Acknowledge */
        SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);      /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Enable Acknowledge */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);        /* 执行语句 */
    }                                                   /* 代码块结束 */

    /* If transfer direction not change and there is no request to start another frame, do not generate Restart Condition */
    /* Mean Previous state is same as current state */
    if ((Prev_State != I2C_STATE_MASTER_BUSY_RX) || (IS_I2C_TRANSFER_OTHER_OPTIONS_REQUEST(XferOptions) == 1))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Generate Start */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);      /* 执行语句 */
    }                                                   /* 代码块结束 */

    /* Process Unlocked */
    __HAL_UNLOCK(hi2c);                                 /* HAL辅助宏/函数 */

    /* Note : The I2C interrupts must be enabled after unlocking current process
    to avoid the risk of I2C interrupt handle execution before current
    process unlock */

    /* Enable interrupts */
    __HAL_I2C_ENABLE_IT(hi2c, enableIT);                /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Sequential receive in master mode an amount of data in non-blocking mode with DMA
  * @note   This interface allow to manage repeated start condition when a direction change during transfer
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for the specified I2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  XferOptions Options of Transfer, value of @ref I2C_XferOptions_definition
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Master_Seq_Receive_DMA(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t XferOptions)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  __IO uint32_t Prev_State = 0x00U;                     /* 赋值或初始化 */
  __IO uint32_t count = 0U;                             /* 赋值或初始化 */
  uint32_t enableIT = (I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);  /* 32位无符号变量/函数 */
  HAL_StatusTypeDef dmaxferstatus;                      /* HAL状态类型函数 */

  /* Check the parameters */
  assert_param(IS_I2C_TRANSFER_OPTIONS_REQUEST(XferOptions));  /* 执行语句 */

  if (hi2c->State == HAL_I2C_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check Busy Flag only if FIRST call of Master interface */
    if ((READ_BIT(hi2c->Instance->CR1, I2C_CR1_STOP) == I2C_CR1_STOP) || (XferOptions == I2C_FIRST_AND_LAST_FRAME) || (XferOptions == I2C_FIRST_FRAME))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Wait until BUSY flag is reset */
      count = I2C_TIMEOUT_BUSY_FLAG * (SystemCoreClock / 25U / 1000U);  /* 赋值或初始化 */
      do                                                /* do-while循环 */
      {                                                 /* 代码块开始 */
        count--;                                        /* 执行语句 */
        if (count == 0U)                                /* 条件判断 */
        {                                               /* 代码块开始 */
          hi2c->PreviousState       = I2C_STATE_NONE;   /* 赋值或初始化 */
          hi2c->State               = HAL_I2C_STATE_READY;  /* 赋值或初始化 */
          hi2c->Mode                = HAL_I2C_MODE_NONE;  /* 赋值或初始化 */
          hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;  /* 赋值或初始化 */

          /* Process Unlocked */
          __HAL_UNLOCK(hi2c);                           /* HAL辅助宏/函数 */

          return HAL_ERROR;                             /* 返回值 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      while (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BUSY) != RESET);  /* while循环 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hi2c);                                   /* HAL辅助宏/函数 */

    /* Check if the I2C is already enabled */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable I2C peripheral */
      __HAL_I2C_ENABLE(hi2c);                           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Disable Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);        /* 执行语句 */

    /* Clear Last DMA bit */
    CLEAR_BIT(hi2c->Instance->CR2, I2C_CR2_LAST);       /* 执行语句 */

    hi2c->State     = HAL_I2C_STATE_BUSY_RX;            /* 赋值或初始化 */
    hi2c->Mode      = HAL_I2C_MODE_MASTER;              /* 赋值或初始化 */
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;               /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hi2c->pBuffPtr    = pData;                          /* 赋值或初始化 */
    hi2c->XferCount   = Size;                           /* 赋值或初始化 */
    hi2c->XferSize    = hi2c->XferCount;                /* 赋值或初始化 */
    hi2c->XferOptions = XferOptions;                    /* 赋值或初始化 */
    hi2c->Devaddress  = DevAddress;                     /* 赋值或初始化 */

    Prev_State = hi2c->PreviousState;                   /* 赋值或初始化 */

    if (hi2c->XferSize > 0U)                            /* 条件判断 */
    {                                                   /* 代码块开始 */
      if ((hi2c->XferCount == 2U) && ((XferOptions == I2C_LAST_FRAME) || (XferOptions == I2C_LAST_FRAME_NO_STOP)))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        if (Prev_State == I2C_STATE_MASTER_BUSY_RX)     /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Disable Acknowledge */
          CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);  /* 执行语句 */

          /* Enable Pos */
          SET_BIT(hi2c->Instance->CR1, I2C_CR1_POS);    /* 执行语句 */

          /* Enable Last DMA bit */
          SET_BIT(hi2c->Instance->CR2, I2C_CR2_LAST);   /* 执行语句 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          /* Enable Acknowledge */
          SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);    /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Enable Acknowledge */
        SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);      /* 执行语句 */

        if ((XferOptions == I2C_LAST_FRAME) || (XferOptions == I2C_OTHER_AND_LAST_FRAME) || (XferOptions == I2C_LAST_FRAME_NO_STOP))  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Enable Last DMA bit */
          SET_BIT(hi2c->Instance->CR2, I2C_CR2_LAST);   /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      if (hi2c->hdmarx != NULL)                         /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Set the I2C DMA transfer complete callback */
        hi2c->hdmarx->XferCpltCallback = I2C_DMAXferCplt;  /* 赋值或初始化 */

        /* Set the DMA error callback */
        hi2c->hdmarx->XferErrorCallback = I2C_DMAError;  /* 赋值或初始化 */

        /* Set the unused DMA callbacks to NULL */
        hi2c->hdmarx->XferHalfCpltCallback = NULL;      /* 赋值或初始化 */
        hi2c->hdmarx->XferAbortCallback = NULL;         /* 赋值或初始化 */

        /* Enable the DMA stream */
        dmaxferstatus = HAL_DMA_Start_IT(hi2c->hdmarx, (uint32_t)&hi2c->Instance->DR, (uint32_t)hi2c->pBuffPtr, hi2c->XferSize);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Update I2C state */
        hi2c->State     = HAL_I2C_STATE_READY;          /* 赋值或初始化 */
        hi2c->Mode      = HAL_I2C_MODE_NONE;            /* 赋值或初始化 */

        /* Update I2C error code */
        hi2c->ErrorCode |= HAL_I2C_ERROR_DMA_PARAM;     /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);                             /* HAL辅助宏/函数 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
      if (dmaxferstatus == HAL_OK)                      /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* If transfer direction not change and there is no request to start another frame, do not generate Restart Condition */
        /* Mean Previous state is same as current state */
        if ((Prev_State != I2C_STATE_MASTER_BUSY_RX) || (IS_I2C_TRANSFER_OTHER_OPTIONS_REQUEST(XferOptions) == 1))  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Generate Start */
          SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);  /* 执行语句 */

          /* Update interrupt for only EVT and ERR */
          enableIT = (I2C_IT_EVT | I2C_IT_ERR);         /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          /* Update interrupt for only ERR */
          enableIT = I2C_IT_ERR;                        /* 赋值或初始化 */
        }                                               /* 代码块结束 */

        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);                             /* HAL辅助宏/函数 */

        /* Note : The I2C interrupts must be enabled after unlocking current process
        to avoid the risk of I2C interrupt handle execution before current
        process unlock */

        /* If XferOptions is not associated to a new frame, mean no start bit is request, enable directly the DMA request */
        /* In other cases, DMA request is enabled after Slave address treatment in IRQHandler */
        if ((XferOptions == I2C_NEXT_FRAME) || (XferOptions == I2C_LAST_FRAME) || (XferOptions == I2C_LAST_FRAME_NO_STOP))  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Enable DMA Request */
          SET_BIT(hi2c->Instance->CR2, I2C_CR2_DMAEN);  /* 执行语句 */
        }                                               /* 代码块结束 */

        /* Enable EVT and ERR interrupt */
        __HAL_I2C_ENABLE_IT(hi2c, enableIT);            /* HAL辅助宏/函数 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Update I2C state */
        hi2c->State     = HAL_I2C_STATE_READY;          /* 赋值或初始化 */
        hi2c->Mode      = HAL_I2C_MODE_NONE;            /* 赋值或初始化 */

        /* Update I2C error code */
        hi2c->ErrorCode |= HAL_I2C_ERROR_DMA;           /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);                             /* HAL辅助宏/函数 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Enable Acknowledge */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);        /* 执行语句 */

      /* If transfer direction not change and there is no request to start another frame, do not generate Restart Condition */
      /* Mean Previous state is same as current state */
      if ((Prev_State != I2C_STATE_MASTER_BUSY_RX) || (IS_I2C_TRANSFER_OTHER_OPTIONS_REQUEST(XferOptions) == 1))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Generate Start */
        SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);    /* 执行语句 */
      }                                                 /* 代码块结束 */

      /* Process Unlocked */
      __HAL_UNLOCK(hi2c);                               /* HAL辅助宏/函数 */

      /* Note : The I2C interrupts must be enabled after unlocking current process
      to avoid the risk of I2C interrupt handle execution before current
      process unlock */

      /* Enable interrupts */
      __HAL_I2C_ENABLE_IT(hi2c, enableIT);              /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Sequential transmit in slave mode an amount of data in non-blocking mode with Interrupt
  * @note   This interface allow to manage repeated start condition when a direction change during transfer
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for the specified I2C.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  XferOptions Options of Transfer, value of @ref I2C_XferOptions_definition
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Slave_Seq_Transmit_IT(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size, uint32_t XferOptions)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_I2C_TRANSFER_OPTIONS_REQUEST(XferOptions));  /* 执行语句 */

  if (((uint32_t)hi2c->State & (uint32_t)HAL_I2C_STATE_LISTEN) == (uint32_t)HAL_I2C_STATE_LISTEN)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) || (Size == 0U))                /* 条件判断 */
    {                                                   /* 代码块开始 */
      return  HAL_ERROR;                                /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hi2c);                                   /* HAL辅助宏/函数 */

    /* Check if the I2C is already enabled */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable I2C peripheral */
      __HAL_I2C_ENABLE(hi2c);                           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Disable Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);        /* 执行语句 */

    hi2c->State     = HAL_I2C_STATE_BUSY_TX_LISTEN;     /* 赋值或初始化 */
    hi2c->Mode      = HAL_I2C_MODE_SLAVE;               /* 赋值或初始化 */
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;               /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hi2c->pBuffPtr    = pData;                          /* 赋值或初始化 */
    hi2c->XferCount   = Size;                           /* 赋值或初始化 */
    hi2c->XferSize    = hi2c->XferCount;                /* 赋值或初始化 */
    hi2c->XferOptions = XferOptions;                    /* 赋值或初始化 */

    /* Clear ADDR flag */
    __HAL_I2C_CLEAR_ADDRFLAG(hi2c);                     /* HAL辅助宏/函数 */

    /* Process Unlocked */
    __HAL_UNLOCK(hi2c);                                 /* HAL辅助宏/函数 */

    /* Note : The I2C interrupts must be enabled after unlocking current process
              to avoid the risk of I2C interrupt handle execution before current
              process unlock */

    /* Enable EVT, BUF and ERR interrupt */
    __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);  /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Sequential transmit in slave mode an amount of data in non-blocking mode with DMA
  * @note   This interface allow to manage repeated start condition when a direction change during transfer
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for the specified I2C.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  XferOptions Options of Transfer, value of @ref I2C_XferOptions_definition
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Slave_Seq_Transmit_DMA(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size, uint32_t XferOptions)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef dmaxferstatus;                      /* HAL状态类型函数 */

  /* Check the parameters */
  assert_param(IS_I2C_TRANSFER_OPTIONS_REQUEST(XferOptions));  /* 执行语句 */

  if (((uint32_t)hi2c->State & (uint32_t)HAL_I2C_STATE_LISTEN) == (uint32_t)HAL_I2C_STATE_LISTEN)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) || (Size == 0U))                /* 条件判断 */
    {                                                   /* 代码块开始 */
      return  HAL_ERROR;                                /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hi2c);                                   /* HAL辅助宏/函数 */

    /* Disable Interrupts, to prevent preemption during treatment in case of multicall */
    __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_ERR);  /* HAL辅助宏/函数 */

    /* I2C cannot manage full duplex exchange so disable previous IT enabled if any */
    /* and then toggle the HAL slave RX state to TX state */
    if (hi2c->State == HAL_I2C_STATE_BUSY_RX_LISTEN)    /* 条件判断 */
    {                                                   /* 代码块开始 */
      if ((hi2c->Instance->CR2 & I2C_CR2_DMAEN) == I2C_CR2_DMAEN)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Abort DMA Xfer if any */
        if (hi2c->hdmarx != NULL)                       /* 条件判断 */
        {                                               /* 代码块开始 */
          CLEAR_BIT(hi2c->Instance->CR2, I2C_CR2_DMAEN);  /* 执行语句 */

          /* Set the I2C DMA Abort callback :
           will lead to call HAL_I2C_ErrorCallback() at end of DMA abort procedure */
          hi2c->hdmarx->XferAbortCallback = I2C_DMAAbort;  /* 赋值或初始化 */

          /* Abort DMA RX */
          if (HAL_DMA_Abort_IT(hi2c->hdmarx) != HAL_OK)  /* 条件判断 */
          {                                             /* 代码块开始 */
            /* Call Directly XferAbortCallback function in case of error */
            hi2c->hdmarx->XferAbortCallback(hi2c->hdmarx);  /* 执行语句 */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else if (hi2c->State == HAL_I2C_STATE_BUSY_TX_LISTEN)  /* 否则如果 */
    {                                                   /* 代码块开始 */
      if ((hi2c->Instance->CR2 & I2C_CR2_DMAEN) == I2C_CR2_DMAEN)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        CLEAR_BIT(hi2c->Instance->CR2, I2C_CR2_DMAEN);  /* 执行语句 */

        /* Abort DMA Xfer if any */
        if (hi2c->hdmatx != NULL)                       /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Set the I2C DMA Abort callback :
           will lead to call HAL_I2C_ErrorCallback() at end of DMA abort procedure */
          hi2c->hdmatx->XferAbortCallback = I2C_DMAAbort;  /* 赋值或初始化 */

          /* Abort DMA TX */
          if (HAL_DMA_Abort_IT(hi2c->hdmatx) != HAL_OK)  /* 条件判断 */
          {                                             /* 代码块开始 */
            /* Call Directly XferAbortCallback function in case of error */
            hi2c->hdmatx->XferAbortCallback(hi2c->hdmatx);  /* 执行语句 */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Nothing to do */
    }                                                   /* 代码块结束 */

    /* Check if the I2C is already enabled */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable I2C peripheral */
      __HAL_I2C_ENABLE(hi2c);                           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Disable Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);        /* 执行语句 */

    hi2c->State     = HAL_I2C_STATE_BUSY_TX_LISTEN;     /* 赋值或初始化 */
    hi2c->Mode      = HAL_I2C_MODE_SLAVE;               /* 赋值或初始化 */
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;               /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hi2c->pBuffPtr    = pData;                          /* 赋值或初始化 */
    hi2c->XferCount   = Size;                           /* 赋值或初始化 */
    hi2c->XferSize    = hi2c->XferCount;                /* 赋值或初始化 */
    hi2c->XferOptions = XferOptions;                    /* 赋值或初始化 */

    if (hi2c->hdmatx != NULL)                           /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the I2C DMA transfer complete callback */
      hi2c->hdmatx->XferCpltCallback = I2C_DMAXferCplt;  /* 赋值或初始化 */

      /* Set the DMA error callback */
      hi2c->hdmatx->XferErrorCallback = I2C_DMAError;   /* 赋值或初始化 */

      /* Set the unused DMA callbacks to NULL */
      hi2c->hdmatx->XferHalfCpltCallback = NULL;        /* 赋值或初始化 */
      hi2c->hdmatx->XferAbortCallback = NULL;           /* 赋值或初始化 */

      /* Enable the DMA stream */
      dmaxferstatus = HAL_DMA_Start_IT(hi2c->hdmatx, (uint32_t)hi2c->pBuffPtr, (uint32_t)&hi2c->Instance->DR, hi2c->XferSize);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Update I2C state */
      hi2c->State     = HAL_I2C_STATE_LISTEN;           /* 赋值或初始化 */
      hi2c->Mode      = HAL_I2C_MODE_NONE;              /* 赋值或初始化 */

      /* Update I2C error code */
      hi2c->ErrorCode |= HAL_I2C_ERROR_DMA_PARAM;       /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hi2c);                               /* HAL辅助宏/函数 */

      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    if (dmaxferstatus == HAL_OK)                        /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable Address Acknowledge */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);        /* 执行语句 */

      /* Clear ADDR flag */
      __HAL_I2C_CLEAR_ADDRFLAG(hi2c);                   /* HAL辅助宏/函数 */

      /* Process Unlocked */
      __HAL_UNLOCK(hi2c);                               /* HAL辅助宏/函数 */

      /* Note : The I2C interrupts must be enabled after unlocking current process
      to avoid the risk of I2C interrupt handle execution before current
      process unlock */
      /* Enable EVT and ERR interrupt */
      __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_ERR);  /* HAL辅助宏/函数 */

      /* Enable DMA Request */
      hi2c->Instance->CR2 |= I2C_CR2_DMAEN;             /* 赋值或初始化 */

      return HAL_OK;                                    /* 返回值 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Update I2C state */
      hi2c->State     = HAL_I2C_STATE_READY;            /* 赋值或初始化 */
      hi2c->Mode      = HAL_I2C_MODE_NONE;              /* 赋值或初始化 */

      /* Update I2C error code */
      hi2c->ErrorCode |= HAL_I2C_ERROR_DMA;             /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hi2c);                               /* HAL辅助宏/函数 */

      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Sequential receive in slave mode an amount of data in non-blocking mode with Interrupt
  * @note   This interface allow to manage repeated start condition when a direction change during transfer
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for the specified I2C.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  XferOptions Options of Transfer, value of @ref I2C_XferOptions_definition
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Slave_Seq_Receive_IT(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size, uint32_t XferOptions)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_I2C_TRANSFER_OPTIONS_REQUEST(XferOptions));  /* 执行语句 */

  if (((uint32_t)hi2c->State & (uint32_t)HAL_I2C_STATE_LISTEN) == (uint32_t)HAL_I2C_STATE_LISTEN)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) || (Size == 0U))                /* 条件判断 */
    {                                                   /* 代码块开始 */
      return  HAL_ERROR;                                /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hi2c);                                   /* HAL辅助宏/函数 */

    /* Check if the I2C is already enabled */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable I2C peripheral */
      __HAL_I2C_ENABLE(hi2c);                           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Disable Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);        /* 执行语句 */

    hi2c->State     = HAL_I2C_STATE_BUSY_RX_LISTEN;     /* 赋值或初始化 */
    hi2c->Mode      = HAL_I2C_MODE_SLAVE;               /* 赋值或初始化 */
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;               /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hi2c->pBuffPtr    = pData;                          /* 赋值或初始化 */
    hi2c->XferCount   = Size;                           /* 赋值或初始化 */
    hi2c->XferSize    = hi2c->XferCount;                /* 赋值或初始化 */
    hi2c->XferOptions = XferOptions;                    /* 赋值或初始化 */

    /* Clear ADDR flag */
    __HAL_I2C_CLEAR_ADDRFLAG(hi2c);                     /* HAL辅助宏/函数 */

    /* Process Unlocked */
    __HAL_UNLOCK(hi2c);                                 /* HAL辅助宏/函数 */

    /* Note : The I2C interrupts must be enabled after unlocking current process
              to avoid the risk of I2C interrupt handle execution before current
              process unlock */

    /* Enable EVT, BUF and ERR interrupt */
    __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);  /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Sequential receive in slave mode an amount of data in non-blocking mode with DMA
  * @note   This interface allow to manage repeated start condition when a direction change during transfer
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for the specified I2C.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  XferOptions Options of Transfer, value of @ref I2C_XferOptions_definition
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Slave_Seq_Receive_DMA(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size, uint32_t XferOptions)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef dmaxferstatus;                      /* HAL状态类型函数 */

  /* Check the parameters */
  assert_param(IS_I2C_TRANSFER_OPTIONS_REQUEST(XferOptions));  /* 执行语句 */

  if (((uint32_t)hi2c->State & (uint32_t)HAL_I2C_STATE_LISTEN) == (uint32_t)HAL_I2C_STATE_LISTEN)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) || (Size == 0U))                /* 条件判断 */
    {                                                   /* 代码块开始 */
      return  HAL_ERROR;                                /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hi2c);                                   /* HAL辅助宏/函数 */

    /* Disable Interrupts, to prevent preemption during treatment in case of multicall */
    __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_ERR);  /* HAL辅助宏/函数 */

    /* I2C cannot manage full duplex exchange so disable previous IT enabled if any */
    /* and then toggle the HAL slave RX state to TX state */
    if (hi2c->State == HAL_I2C_STATE_BUSY_RX_LISTEN)    /* 条件判断 */
    {                                                   /* 代码块开始 */
      if ((hi2c->Instance->CR2 & I2C_CR2_DMAEN) == I2C_CR2_DMAEN)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Abort DMA Xfer if any */
        if (hi2c->hdmarx != NULL)                       /* 条件判断 */
        {                                               /* 代码块开始 */
          CLEAR_BIT(hi2c->Instance->CR2, I2C_CR2_DMAEN);  /* 执行语句 */

          /* Set the I2C DMA Abort callback :
           will lead to call HAL_I2C_ErrorCallback() at end of DMA abort procedure */
          hi2c->hdmarx->XferAbortCallback = I2C_DMAAbort;  /* 赋值或初始化 */

          /* Abort DMA RX */
          if (HAL_DMA_Abort_IT(hi2c->hdmarx) != HAL_OK)  /* 条件判断 */
          {                                             /* 代码块开始 */
            /* Call Directly XferAbortCallback function in case of error */
            hi2c->hdmarx->XferAbortCallback(hi2c->hdmarx);  /* 执行语句 */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else if (hi2c->State == HAL_I2C_STATE_BUSY_TX_LISTEN)  /* 否则如果 */
    {                                                   /* 代码块开始 */
      if ((hi2c->Instance->CR2 & I2C_CR2_DMAEN) == I2C_CR2_DMAEN)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        CLEAR_BIT(hi2c->Instance->CR2, I2C_CR2_DMAEN);  /* 执行语句 */

        /* Abort DMA Xfer if any */
        if (hi2c->hdmatx != NULL)                       /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Set the I2C DMA Abort callback :
           will lead to call HAL_I2C_ErrorCallback() at end of DMA abort procedure */
          hi2c->hdmatx->XferAbortCallback = I2C_DMAAbort;  /* 赋值或初始化 */

          /* Abort DMA TX */
          if (HAL_DMA_Abort_IT(hi2c->hdmatx) != HAL_OK)  /* 条件判断 */
          {                                             /* 代码块开始 */
            /* Call Directly XferAbortCallback function in case of error */
            hi2c->hdmatx->XferAbortCallback(hi2c->hdmatx);  /* 执行语句 */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Nothing to do */
    }                                                   /* 代码块结束 */

    /* Check if the I2C is already enabled */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable I2C peripheral */
      __HAL_I2C_ENABLE(hi2c);                           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Disable Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);        /* 执行语句 */

    hi2c->State     = HAL_I2C_STATE_BUSY_RX_LISTEN;     /* 赋值或初始化 */
    hi2c->Mode      = HAL_I2C_MODE_SLAVE;               /* 赋值或初始化 */
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;               /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hi2c->pBuffPtr    = pData;                          /* 赋值或初始化 */
    hi2c->XferCount   = Size;                           /* 赋值或初始化 */
    hi2c->XferSize    = hi2c->XferCount;                /* 赋值或初始化 */
    hi2c->XferOptions = XferOptions;                    /* 赋值或初始化 */

    if (hi2c->hdmarx != NULL)                           /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the I2C DMA transfer complete callback */
      hi2c->hdmarx->XferCpltCallback = I2C_DMAXferCplt;  /* 赋值或初始化 */

      /* Set the DMA error callback */
      hi2c->hdmarx->XferErrorCallback = I2C_DMAError;   /* 赋值或初始化 */

      /* Set the unused DMA callbacks to NULL */
      hi2c->hdmarx->XferHalfCpltCallback = NULL;        /* 赋值或初始化 */
      hi2c->hdmarx->XferAbortCallback = NULL;           /* 赋值或初始化 */

      /* Enable the DMA stream */
      dmaxferstatus = HAL_DMA_Start_IT(hi2c->hdmarx, (uint32_t)&hi2c->Instance->DR, (uint32_t)hi2c->pBuffPtr, hi2c->XferSize);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Update I2C state */
      hi2c->State     = HAL_I2C_STATE_LISTEN;           /* 赋值或初始化 */
      hi2c->Mode      = HAL_I2C_MODE_NONE;              /* 赋值或初始化 */

      /* Update I2C error code */
      hi2c->ErrorCode |= HAL_I2C_ERROR_DMA_PARAM;       /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hi2c);                               /* HAL辅助宏/函数 */

      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    if (dmaxferstatus == HAL_OK)                        /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable Address Acknowledge */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);        /* 执行语句 */

      /* Clear ADDR flag */
      __HAL_I2C_CLEAR_ADDRFLAG(hi2c);                   /* HAL辅助宏/函数 */

      /* Process Unlocked */
      __HAL_UNLOCK(hi2c);                               /* HAL辅助宏/函数 */

      /* Enable DMA Request */
      SET_BIT(hi2c->Instance->CR2, I2C_CR2_DMAEN);      /* 执行语句 */

      /* Note : The I2C interrupts must be enabled after unlocking current process
      to avoid the risk of I2C interrupt handle execution before current
      process unlock */
      /* Enable EVT and ERR interrupt */
      __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_ERR);  /* HAL辅助宏/函数 */

      return HAL_OK;                                    /* 返回值 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Update I2C state */
      hi2c->State     = HAL_I2C_STATE_READY;            /* 赋值或初始化 */
      hi2c->Mode      = HAL_I2C_MODE_NONE;              /* 赋值或初始化 */

      /* Update I2C error code */
      hi2c->ErrorCode |= HAL_I2C_ERROR_DMA;             /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hi2c);                               /* HAL辅助宏/函数 */

      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Enable the Address listen mode with Interrupt.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_EnableListen_IT(I2C_HandleTypeDef *hi2c)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  if (hi2c->State == HAL_I2C_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    hi2c->State = HAL_I2C_STATE_LISTEN;                 /* 赋值或初始化 */

    /* Check if the I2C is already enabled */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable I2C peripheral */
      __HAL_I2C_ENABLE(hi2c);                           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Enable Address Acknowledge */
    SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);          /* 执行语句 */

    /* Enable EVT and ERR interrupt */
    __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_ERR);  /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Disable the Address listen mode with Interrupt.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_DisableListen_IT(I2C_HandleTypeDef *hi2c)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Declaration of tmp to prevent undefined behavior of volatile usage */
  uint32_t tmp;                                         /* 32位无符号变量/函数 */

  /* Disable Address listen mode only if a transfer is not ongoing */
  if (hi2c->State == HAL_I2C_STATE_LISTEN)              /* 条件判断 */
  {                                                     /* 代码块开始 */
    tmp = (uint32_t)(hi2c->State) & I2C_STATE_MSK;      /* 赋值或初始化 */
    hi2c->PreviousState = tmp | (uint32_t)(hi2c->Mode);  /* 赋值或初始化 */
    hi2c->State = HAL_I2C_STATE_READY;                  /* 赋值或初始化 */
    hi2c->Mode = HAL_I2C_MODE_NONE;                     /* 赋值或初始化 */

    /* Disable Address Acknowledge */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);        /* 执行语句 */

    /* Disable EVT and ERR interrupt */
    __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_ERR);  /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Abort a master I2C IT or DMA process communication with Interrupt.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for the specified I2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Master_Abort_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Declaration of temporary variables to prevent undefined behavior of volatile usage */
  HAL_I2C_ModeTypeDef CurrentMode   = hi2c->Mode;       /* HAL I2C操作 */

  /* Prevent unused argument(s) compilation warning */
  UNUSED(DevAddress);                                   /* 执行语句 */

  /* Abort Master transfer during Receive or Transmit process    */
  if ((__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BUSY) != RESET) && (CurrentMode == HAL_I2C_MODE_MASTER))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Process Locked */
    __HAL_LOCK(hi2c);                                   /* HAL辅助宏/函数 */

    hi2c->PreviousState = I2C_STATE_NONE;               /* 赋值或初始化 */
    hi2c->State = HAL_I2C_STATE_ABORT;                  /* 赋值或初始化 */

    /* Disable Acknowledge */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);        /* 执行语句 */

    /* Generate Stop */
    SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);         /* 执行语句 */

    hi2c->XferCount = 0U;                               /* 赋值或初始化 */

    /* Disable EVT, BUF and ERR interrupt */
    __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);  /* HAL辅助宏/函数 */

    /* Process Unlocked */
    __HAL_UNLOCK(hi2c);                                 /* HAL辅助宏/函数 */

    /* Call the corresponding callback to inform upper layer of End of Transfer */
    I2C_ITError(hi2c);                                  /* 执行语句 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Wrong usage of abort function */
    /* This function should be used only in case of abort monitored by master device */
    /* Or periphal is not in busy state, mean there is no active sequence to be abort */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup I2C_IRQ_Handler_and_Callbacks IRQ Handler and Callbacks
 * @{
 */

/**
  * @brief  This function handles I2C event interrupt request.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval None
  */
void HAL_I2C_EV_IRQHandler(I2C_HandleTypeDef *hi2c)     /* 中断服务函数 */
{                                                       /* 代码块开始 */
  uint32_t sr1itflags;                                  /* 32位无符号变量/函数 */
  uint32_t sr2itflags               = 0U;               /* 32位无符号变量/函数 */
  uint32_t itsources                = READ_REG(hi2c->Instance->CR2);  /* 32位无符号变量/函数 */
  uint32_t CurrentXferOptions       = hi2c->XferOptions;  /* 32位无符号变量/函数 */
  HAL_I2C_ModeTypeDef CurrentMode   = hi2c->Mode;       /* HAL I2C操作 */
  HAL_I2C_StateTypeDef CurrentState = hi2c->State;      /* HAL I2C操作 */

  /* Master or Memory mode selected */
  if ((CurrentMode == HAL_I2C_MODE_MASTER) || (CurrentMode == HAL_I2C_MODE_MEM))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    sr2itflags   = READ_REG(hi2c->Instance->SR2);       /* 赋值或初始化 */
    sr1itflags   = READ_REG(hi2c->Instance->SR1);       /* 赋值或初始化 */

    /* Exit IRQ event until Start Bit detected in case of Other frame requested */
    if ((I2C_CHECK_FLAG(sr1itflags, I2C_FLAG_SB) == RESET) && (IS_I2C_TRANSFER_OTHER_OPTIONS_REQUEST(CurrentXferOptions) == 1U))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return;                                           /* 返回 */
    }                                                   /* 代码块结束 */

    /* SB Set ----------------------------------------------------------------*/
    if ((I2C_CHECK_FLAG(sr1itflags, I2C_FLAG_SB) != RESET) && (I2C_CHECK_IT_SOURCE(itsources, I2C_IT_EVT) != RESET))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Convert OTHER_xxx XferOptions if any */
      I2C_ConvertOtherXferOptions(hi2c);                /* 执行语句 */

      I2C_Master_SB(hi2c);                              /* 执行语句 */
    }                                                   /* 代码块结束 */
    /* ADD10 Set -------------------------------------------------------------*/
    else if ((I2C_CHECK_FLAG(sr1itflags, I2C_FLAG_ADD10) != RESET) && (I2C_CHECK_IT_SOURCE(itsources, I2C_IT_EVT) != RESET))  /* 否则如果 */
    {                                                   /* 代码块开始 */
      I2C_Master_ADD10(hi2c);                           /* 执行语句 */
    }                                                   /* 代码块结束 */
    /* ADDR Set --------------------------------------------------------------*/
    else if ((I2C_CHECK_FLAG(sr1itflags, I2C_FLAG_ADDR) != RESET) && (I2C_CHECK_IT_SOURCE(itsources, I2C_IT_EVT) != RESET))  /* 否则如果 */
    {                                                   /* 代码块开始 */
      I2C_Master_ADDR(hi2c);                            /* 执行语句 */
    }                                                   /* 代码块结束 */
    /* I2C in mode Transmitter -----------------------------------------------*/
    else if (I2C_CHECK_FLAG(sr2itflags, I2C_FLAG_TRA) != RESET)  /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* Do not check buffer and BTF flag if a Xfer DMA is on going */
      if (READ_BIT(hi2c->Instance->CR2, I2C_CR2_DMAEN) != I2C_CR2_DMAEN)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* TXE set and BTF reset -----------------------------------------------*/
        if ((I2C_CHECK_FLAG(sr1itflags, I2C_FLAG_TXE) != RESET) && (I2C_CHECK_IT_SOURCE(itsources, I2C_IT_BUF) != RESET) && (I2C_CHECK_FLAG(sr1itflags, I2C_FLAG_BTF) == RESET))  /* 条件判断 */
        {                                               /* 代码块开始 */
          I2C_MasterTransmit_TXE(hi2c);                 /* 执行语句 */
        }                                               /* 代码块结束 */
        /* BTF set -------------------------------------------------------------*/
        else if ((I2C_CHECK_FLAG(sr1itflags, I2C_FLAG_BTF) != RESET) && (I2C_CHECK_IT_SOURCE(itsources, I2C_IT_EVT) != RESET))  /* 否则如果 */
        {                                               /* 代码块开始 */
          if (CurrentMode == HAL_I2C_MODE_MASTER)       /* 条件判断 */
          {                                             /* 代码块开始 */
            I2C_MasterTransmit_BTF(hi2c);               /* 执行语句 */
          }                                             /* 代码块结束 */
          else /* HAL_I2C_MODE_MEM */
          {                                             /* 代码块开始 */
            I2C_MemoryTransmit_TXE_BTF(hi2c);           /* 执行语句 */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          /* Do nothing */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    /* I2C in mode Receiver --------------------------------------------------*/
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Do not check buffer and BTF flag if a Xfer DMA is on going */
      if (READ_BIT(hi2c->Instance->CR2, I2C_CR2_DMAEN) != I2C_CR2_DMAEN)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* RXNE set and BTF reset -----------------------------------------------*/
        if ((I2C_CHECK_FLAG(sr1itflags, I2C_FLAG_RXNE) != RESET) && (I2C_CHECK_IT_SOURCE(itsources, I2C_IT_BUF) != RESET) && (I2C_CHECK_FLAG(sr1itflags, I2C_FLAG_BTF) == RESET))  /* 条件判断 */
        {                                               /* 代码块开始 */
          I2C_MasterReceive_RXNE(hi2c);                 /* 执行语句 */
        }                                               /* 代码块结束 */
        /* BTF set -------------------------------------------------------------*/
        else if ((I2C_CHECK_FLAG(sr1itflags, I2C_FLAG_BTF) != RESET) && (I2C_CHECK_IT_SOURCE(itsources, I2C_IT_EVT) != RESET))  /* 否则如果 */
        {                                               /* 代码块开始 */
          I2C_MasterReceive_BTF(hi2c);                  /* 执行语句 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          /* Do nothing */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /* Slave mode selected */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* If an error is detected, read only SR1 register to prevent */
    /* a clear of ADDR flags by reading SR2 after reading SR1 in Error treatment */
    if (hi2c->ErrorCode != HAL_I2C_ERROR_NONE)          /* 条件判断 */
    {                                                   /* 代码块开始 */
      sr1itflags   = READ_REG(hi2c->Instance->SR1);     /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      sr2itflags   = READ_REG(hi2c->Instance->SR2);     /* 赋值或初始化 */
      sr1itflags   = READ_REG(hi2c->Instance->SR1);     /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* ADDR set --------------------------------------------------------------*/
    if ((I2C_CHECK_FLAG(sr1itflags, I2C_FLAG_ADDR) != RESET) && (I2C_CHECK_IT_SOURCE(itsources, I2C_IT_EVT) != RESET))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Now time to read SR2, this will clear ADDR flag automatically */
      if (hi2c->ErrorCode != HAL_I2C_ERROR_NONE)        /* 条件判断 */
      {                                                 /* 代码块开始 */
        sr2itflags   = READ_REG(hi2c->Instance->SR2);   /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      I2C_Slave_ADDR(hi2c, sr2itflags);                 /* 执行语句 */
    }                                                   /* 代码块结束 */
    /* STOPF set --------------------------------------------------------------*/
    else if ((I2C_CHECK_FLAG(sr1itflags, I2C_FLAG_STOPF) != RESET) && (I2C_CHECK_IT_SOURCE(itsources, I2C_IT_EVT) != RESET))  /* 否则如果 */
    {                                                   /* 代码块开始 */
      I2C_Slave_STOPF(hi2c);                            /* 执行语句 */
    }                                                   /* 代码块结束 */
    /* I2C in mode Transmitter -----------------------------------------------*/
    else if ((CurrentState == HAL_I2C_STATE_BUSY_TX) || (CurrentState == HAL_I2C_STATE_BUSY_TX_LISTEN))  /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* TXE set and BTF reset -----------------------------------------------*/
      if ((I2C_CHECK_FLAG(sr1itflags, I2C_FLAG_TXE) != RESET) && (I2C_CHECK_IT_SOURCE(itsources, I2C_IT_BUF) != RESET) && (I2C_CHECK_FLAG(sr1itflags, I2C_FLAG_BTF) == RESET))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        I2C_SlaveTransmit_TXE(hi2c);                    /* 执行语句 */
      }                                                 /* 代码块结束 */
      /* BTF set -------------------------------------------------------------*/
      else if ((I2C_CHECK_FLAG(sr1itflags, I2C_FLAG_BTF) != RESET) && (I2C_CHECK_IT_SOURCE(itsources, I2C_IT_EVT) != RESET))  /* 否则如果 */
      {                                                 /* 代码块开始 */
        I2C_SlaveTransmit_BTF(hi2c);                    /* 执行语句 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Do nothing */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    /* I2C in mode Receiver --------------------------------------------------*/
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* RXNE set and BTF reset ----------------------------------------------*/
      if ((I2C_CHECK_FLAG(sr1itflags, I2C_FLAG_RXNE) != RESET) && (I2C_CHECK_IT_SOURCE(itsources, I2C_IT_BUF) != RESET) && (I2C_CHECK_FLAG(sr1itflags, I2C_FLAG_BTF) == RESET))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        I2C_SlaveReceive_RXNE(hi2c);                    /* 执行语句 */
      }                                                 /* 代码块结束 */
      /* BTF set -------------------------------------------------------------*/
      else if ((I2C_CHECK_FLAG(sr1itflags, I2C_FLAG_BTF) != RESET) && (I2C_CHECK_IT_SOURCE(itsources, I2C_IT_EVT) != RESET))  /* 否则如果 */
      {                                                 /* 代码块开始 */
        I2C_SlaveReceive_BTF(hi2c);                     /* 执行语句 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Do nothing */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  This function handles I2C error interrupt request.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval None
  */
void HAL_I2C_ER_IRQHandler(I2C_HandleTypeDef *hi2c)     /* 中断服务函数 */
{                                                       /* 代码块开始 */
  HAL_I2C_ModeTypeDef tmp1;                             /* HAL I2C操作 */
  uint32_t tmp2;                                        /* 32位无符号变量/函数 */
  HAL_I2C_StateTypeDef tmp3;                            /* HAL I2C操作 */
  uint32_t tmp4;                                        /* 32位无符号变量/函数 */
  uint32_t sr1itflags = READ_REG(hi2c->Instance->SR1);  /* 32位无符号变量/函数 */
  uint32_t itsources  = READ_REG(hi2c->Instance->CR2);  /* 32位无符号变量/函数 */
  uint32_t error      = HAL_I2C_ERROR_NONE;             /* 32位无符号变量/函数 */
  HAL_I2C_ModeTypeDef CurrentMode   = hi2c->Mode;       /* HAL I2C操作 */

  /* I2C Bus error interrupt occurred ----------------------------------------*/
  if ((I2C_CHECK_FLAG(sr1itflags, I2C_FLAG_BERR) != RESET) && (I2C_CHECK_IT_SOURCE(itsources, I2C_IT_ERR) != RESET))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    error |= HAL_I2C_ERROR_BERR;                        /* 赋值或初始化 */

    /* Clear BERR flag */
    __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_BERR);          /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* I2C Arbitration Lost error interrupt occurred ---------------------------*/
  if ((I2C_CHECK_FLAG(sr1itflags, I2C_FLAG_ARLO) != RESET) && (I2C_CHECK_IT_SOURCE(itsources, I2C_IT_ERR) != RESET))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    error |= HAL_I2C_ERROR_ARLO;                        /* 赋值或初始化 */

    /* Clear ARLO flag */
    __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_ARLO);          /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* I2C Acknowledge failure error interrupt occurred ------------------------*/
  if ((I2C_CHECK_FLAG(sr1itflags, I2C_FLAG_AF) != RESET) && (I2C_CHECK_IT_SOURCE(itsources, I2C_IT_ERR) != RESET))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    tmp1 = CurrentMode;                                 /* 赋值或初始化 */
    tmp2 = hi2c->XferCount;                             /* 赋值或初始化 */
    tmp3 = hi2c->State;                                 /* 赋值或初始化 */
    tmp4 = hi2c->PreviousState;                         /* 赋值或初始化 */
    if ((tmp1 == HAL_I2C_MODE_SLAVE) && (tmp2 == 0U) && \
        ((tmp3 == HAL_I2C_STATE_BUSY_TX) || (tmp3 == HAL_I2C_STATE_BUSY_TX_LISTEN) || \
         ((tmp3 == HAL_I2C_STATE_LISTEN) && (tmp4 == I2C_STATE_SLAVE_BUSY_TX))))  /* 源代码 */
    {                                                   /* 代码块开始 */
      I2C_Slave_AF(hi2c);                               /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Clear AF flag */
      __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_AF);          /* HAL辅助宏/函数 */

      error |= HAL_I2C_ERROR_AF;                        /* 赋值或初始化 */

      /* Do not generate a STOP in case of Slave receive non acknowledge during transfer (mean not at the end of transfer) */
      if ((CurrentMode == HAL_I2C_MODE_MASTER) || (CurrentMode == HAL_I2C_MODE_MEM))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Generate Stop */
        SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);     /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* I2C Over-Run/Under-Run interrupt occurred -------------------------------*/
  if ((I2C_CHECK_FLAG(sr1itflags, I2C_FLAG_OVR) != RESET) && (I2C_CHECK_IT_SOURCE(itsources, I2C_IT_ERR) != RESET))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    error |= HAL_I2C_ERROR_OVR;                         /* 赋值或初始化 */
    /* Clear OVR flag */
    __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_OVR);           /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Call the Error Callback in case of Error detected -----------------------*/
  if (error != HAL_I2C_ERROR_NONE)                      /* 条件判断 */
  {                                                     /* 代码块开始 */
    hi2c->ErrorCode |= error;                           /* 赋值或初始化 */
    I2C_ITError(hi2c);                                  /* 执行语句 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Master Tx Transfer completed callback.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval None
  */
__weak void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hi2c);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_I2C_MasterTxCpltCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Master Rx Transfer completed callback.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval None
  */
__weak void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hi2c);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_I2C_MasterRxCpltCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/** @brief  Slave Tx Transfer completed callback.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval None
  */
__weak void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hi2c);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_I2C_SlaveTxCpltCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Slave Rx Transfer completed callback.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval None
  */
__weak void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hi2c);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_I2C_SlaveRxCpltCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Slave Address Match callback.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  TransferDirection Master request Transfer Direction (Write/Read), value of @ref I2C_XferDirection_definition
  * @param  AddrMatchCode Address Match Code
  * @retval None
  */
__weak void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hi2c);                                         /* 执行语句 */
  UNUSED(TransferDirection);                            /* 执行语句 */
  UNUSED(AddrMatchCode);                                /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_I2C_AddrCallback() could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Listen Complete callback.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval None
  */
__weak void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hi2c);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_I2C_ListenCpltCallback() could be implemented in the user file
  */
}                                                       /* 代码块结束 */

/**
  * @brief  Memory Tx Transfer completed callback.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval None
  */
__weak void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hi2c);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_I2C_MemTxCpltCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Memory Rx Transfer completed callback.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval None
  */
__weak void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hi2c);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_I2C_MemRxCpltCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  I2C error callback.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval None
  */
__weak void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hi2c);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_I2C_ErrorCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  I2C abort callback.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval None
  */
__weak void HAL_I2C_AbortCpltCallback(I2C_HandleTypeDef *hi2c)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hi2c);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_I2C_AbortCpltCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup I2C_Exported_Functions_Group3 Peripheral State, Mode and Error functions
 *  @brief   Peripheral State, Mode and Error functions
  *
@verbatim
 ===============================================================================
            ##### Peripheral State, Mode and Error functions #####
 ===============================================================================
    [..]
    This subsection permit to get in run-time the status of the peripheral
    and the data flow.

@endverbatim
  * @{
  */

/**
  * @brief  Return the I2C handle state.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval HAL state
  */
HAL_I2C_StateTypeDef HAL_I2C_GetState(I2C_HandleTypeDef *hi2c)  /* HAL I2C操作 */
{                                                       /* 代码块开始 */
  /* Return I2C handle state */
  return hi2c->State;                                   /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Returns the I2C Master, Slave, Memory or no mode.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for I2C module
  * @retval HAL mode
  */
HAL_I2C_ModeTypeDef HAL_I2C_GetMode(I2C_HandleTypeDef *hi2c)  /* HAL I2C操作 */
{                                                       /* 代码块开始 */
  return hi2c->Mode;                                    /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return the I2C error code.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *              the configuration information for the specified I2C.
  * @retval I2C Error Code
  */
uint32_t HAL_I2C_GetError(I2C_HandleTypeDef *hi2c)      /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  return hi2c->ErrorCode;                               /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup I2C_Private_Functions
  * @{
  */

/**
  * @brief  Handle TXE flag for Master
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for I2C module
  * @retval None
  */
static void I2C_MasterTransmit_TXE(I2C_HandleTypeDef *hi2c)  /* 静态函数 */
{                                                       /* 代码块开始 */
  /* Declaration of temporary variables to prevent undefined behavior of volatile usage */
  HAL_I2C_StateTypeDef CurrentState = hi2c->State;      /* HAL I2C操作 */
  HAL_I2C_ModeTypeDef CurrentMode   = hi2c->Mode;       /* HAL I2C操作 */
  uint32_t CurrentXferOptions       = hi2c->XferOptions;  /* 32位无符号变量/函数 */

  if ((hi2c->XferSize == 0U) && (CurrentState == HAL_I2C_STATE_BUSY_TX))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Call TxCpltCallback() directly if no stop mode is set */
    if ((CurrentXferOptions != I2C_FIRST_AND_LAST_FRAME) && (CurrentXferOptions != I2C_LAST_FRAME) && (CurrentXferOptions != I2C_NO_OPTION_FRAME))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);  /* HAL辅助宏/函数 */

      hi2c->PreviousState = I2C_STATE_MASTER_BUSY_TX;   /* 赋值或初始化 */
      hi2c->Mode = HAL_I2C_MODE_NONE;                   /* 赋值或初始化 */
      hi2c->State = HAL_I2C_STATE_READY;                /* 赋值或初始化 */

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)               /* 条件编译 */
      hi2c->MasterTxCpltCallback(hi2c);                 /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_I2C_MasterTxCpltCallback(hi2c);               /* HAL I2C操作 */
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
    else /* Generate Stop condition then Call TxCpltCallback() */
    {                                                   /* 代码块开始 */
      /* Disable EVT, BUF and ERR interrupt */
      __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);  /* HAL辅助宏/函数 */

      /* Generate Stop */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);       /* 执行语句 */

      hi2c->PreviousState = I2C_STATE_NONE;             /* 赋值或初始化 */
      hi2c->State = HAL_I2C_STATE_READY;                /* 赋值或初始化 */

      if (hi2c->Mode == HAL_I2C_MODE_MEM)               /* 条件判断 */
      {                                                 /* 代码块开始 */
        hi2c->Mode = HAL_I2C_MODE_NONE;                 /* 赋值或初始化 */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)               /* 条件编译 */
        hi2c->MemTxCpltCallback(hi2c);                  /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_I2C_MemTxCpltCallback(hi2c);                /* HAL I2C操作 */
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        hi2c->Mode = HAL_I2C_MODE_NONE;                 /* 赋值或初始化 */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)               /* 条件编译 */
        hi2c->MasterTxCpltCallback(hi2c);               /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_I2C_MasterTxCpltCallback(hi2c);             /* HAL I2C操作 */
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if ((CurrentState == HAL_I2C_STATE_BUSY_TX) || \
           ((CurrentMode == HAL_I2C_MODE_MEM) && (CurrentState == HAL_I2C_STATE_BUSY_RX)))  /* 源代码 */
  {                                                     /* 代码块开始 */
    if (hi2c->XferCount == 0U)                          /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable BUF interrupt */
      __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_BUF);           /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      if (hi2c->Mode == HAL_I2C_MODE_MEM)               /* 条件判断 */
      {                                                 /* 代码块开始 */
        I2C_MemoryTransmit_TXE_BTF(hi2c);               /* 执行语句 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Write data to DR */
        hi2c->Instance->DR = *hi2c->pBuffPtr;           /* 赋值或初始化 */

        /* Increment Buffer pointer */
        hi2c->pBuffPtr++;                               /* 执行语句 */

        /* Update counter */
        hi2c->XferCount--;                              /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Do nothing */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Handle BTF flag for Master transmitter
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for I2C module
  * @retval None
  */
static void I2C_MasterTransmit_BTF(I2C_HandleTypeDef *hi2c)  /* 静态函数 */
{                                                       /* 代码块开始 */
  /* Declaration of temporary variables to prevent undefined behavior of volatile usage */
  uint32_t CurrentXferOptions = hi2c->XferOptions;      /* 32位无符号变量/函数 */

  if (hi2c->State == HAL_I2C_STATE_BUSY_TX)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (hi2c->XferCount != 0U)                          /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Write data to DR */
      hi2c->Instance->DR = *hi2c->pBuffPtr;             /* 赋值或初始化 */

      /* Increment Buffer pointer */
      hi2c->pBuffPtr++;                                 /* 执行语句 */

      /* Update counter */
      hi2c->XferCount--;                                /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Call TxCpltCallback() directly if no stop mode is set */
      if ((CurrentXferOptions != I2C_FIRST_AND_LAST_FRAME) && (CurrentXferOptions != I2C_LAST_FRAME) && (CurrentXferOptions != I2C_NO_OPTION_FRAME))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);  /* HAL辅助宏/函数 */

        hi2c->PreviousState = I2C_STATE_MASTER_BUSY_TX;  /* 赋值或初始化 */
        hi2c->Mode = HAL_I2C_MODE_NONE;                 /* 赋值或初始化 */
        hi2c->State = HAL_I2C_STATE_READY;              /* 赋值或初始化 */

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)               /* 条件编译 */
        hi2c->MasterTxCpltCallback(hi2c);               /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_I2C_MasterTxCpltCallback(hi2c);             /* HAL I2C操作 */
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
      }                                                 /* 代码块结束 */
      else /* Generate Stop condition then Call TxCpltCallback() */
      {                                                 /* 代码块开始 */
        /* Disable EVT, BUF and ERR interrupt */
        __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);  /* HAL辅助宏/函数 */

        /* Generate Stop */
        SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);     /* 执行语句 */

        hi2c->PreviousState = I2C_STATE_NONE;           /* 赋值或初始化 */
        hi2c->State = HAL_I2C_STATE_READY;              /* 赋值或初始化 */
        hi2c->Mode = HAL_I2C_MODE_NONE;                 /* 赋值或初始化 */

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)               /* 条件编译 */
        hi2c->MasterTxCpltCallback(hi2c);               /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_I2C_MasterTxCpltCallback(hi2c);             /* HAL I2C操作 */
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Do nothing */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Handle TXE and BTF flag for Memory transmitter
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for I2C module
  * @retval None
  */
static void I2C_MemoryTransmit_TXE_BTF(I2C_HandleTypeDef *hi2c)  /* 静态函数 */
{                                                       /* 代码块开始 */
  /* Declaration of temporary variables to prevent undefined behavior of volatile usage */
  HAL_I2C_StateTypeDef CurrentState = hi2c->State;      /* HAL I2C操作 */

  if (hi2c->EventCount == 0U)                           /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* If Memory address size is 8Bit */
    if (hi2c->MemaddSize == I2C_MEMADD_SIZE_8BIT)       /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Send Memory Address */
      hi2c->Instance->DR = I2C_MEM_ADD_LSB(hi2c->Memaddress);  /* 赋值或初始化 */

      hi2c->EventCount += 2U;                           /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    /* If Memory address size is 16Bit */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Send MSB of Memory Address */
      hi2c->Instance->DR = I2C_MEM_ADD_MSB(hi2c->Memaddress);  /* 赋值或初始化 */

      hi2c->EventCount++;                               /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (hi2c->EventCount == 1U)                      /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* Send LSB of Memory Address */
    hi2c->Instance->DR = I2C_MEM_ADD_LSB(hi2c->Memaddress);  /* 赋值或初始化 */

    hi2c->EventCount++;                                 /* 执行语句 */
  }                                                     /* 代码块结束 */
  else if (hi2c->EventCount == 2U)                      /* 否则如果 */
  {                                                     /* 代码块开始 */
    if (CurrentState == HAL_I2C_STATE_BUSY_RX)          /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Generate Restart */
      hi2c->Instance->CR1 |= I2C_CR1_START;             /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if ((hi2c->XferCount > 0U) && (CurrentState == HAL_I2C_STATE_BUSY_TX))  /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* Write data to DR */
      hi2c->Instance->DR = *hi2c->pBuffPtr;             /* 赋值或初始化 */

      /* Increment Buffer pointer */
      hi2c->pBuffPtr++;                                 /* 执行语句 */

      /* Update counter */
      hi2c->XferCount--;                                /* 执行语句 */
    }                                                   /* 代码块结束 */
    else if ((hi2c->XferCount == 0U) && (CurrentState == HAL_I2C_STATE_BUSY_TX))  /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* Generate Stop condition then Call TxCpltCallback() */
      /* Disable EVT, BUF and ERR interrupt */
      __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);  /* HAL辅助宏/函数 */

      /* Generate Stop */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);       /* 执行语句 */

      hi2c->PreviousState = I2C_STATE_NONE;             /* 赋值或初始化 */
      hi2c->State = HAL_I2C_STATE_READY;                /* 赋值或初始化 */
      hi2c->Mode = HAL_I2C_MODE_NONE;                   /* 赋值或初始化 */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)               /* 条件编译 */
      hi2c->MemTxCpltCallback(hi2c);                    /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_I2C_MemTxCpltCallback(hi2c);                  /* HAL I2C操作 */
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Do nothing */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Do nothing */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Handle RXNE flag for Master
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for I2C module
  * @retval None
  */
static void I2C_MasterReceive_RXNE(I2C_HandleTypeDef *hi2c)  /* 静态函数 */
{                                                       /* 代码块开始 */
  if (hi2c->State == HAL_I2C_STATE_BUSY_RX)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    uint32_t tmp;                                       /* 32位无符号变量/函数 */

    tmp = hi2c->XferCount;                              /* 赋值或初始化 */
    if (tmp > 3U)                                       /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Read data from DR */
      *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;    /* 赋值或初始化 */

      /* Increment Buffer pointer */
      hi2c->pBuffPtr++;                                 /* 执行语句 */

      /* Update counter */
      hi2c->XferCount--;                                /* 执行语句 */

      if (hi2c->XferCount == (uint16_t)3)               /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Disable BUF interrupt, this help to treat correctly the last 4 bytes
        on BTF subroutine */
        /* Disable BUF interrupt */
        __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_BUF);         /* HAL辅助宏/函数 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else if ((hi2c->XferOptions != I2C_FIRST_AND_NEXT_FRAME) && ((tmp == 1U) || (tmp == 0U)))  /* 否则如果 */
    {                                                   /* 代码块开始 */
      if (I2C_WaitOnSTOPRequestThroughIT(hi2c) == HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Disable Acknowledge */
        CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);    /* 执行语句 */

        /* Disable EVT, BUF and ERR interrupt */
        __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);  /* HAL辅助宏/函数 */

        /* Read data from DR */
        *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;  /* 赋值或初始化 */

        /* Increment Buffer pointer */
        hi2c->pBuffPtr++;                               /* 执行语句 */

        /* Update counter */
        hi2c->XferCount--;                              /* 执行语句 */

        hi2c->State = HAL_I2C_STATE_READY;              /* 赋值或初始化 */

        if (hi2c->Mode == HAL_I2C_MODE_MEM)             /* 条件判断 */
        {                                               /* 代码块开始 */
          hi2c->Mode = HAL_I2C_MODE_NONE;               /* 赋值或初始化 */
          hi2c->PreviousState = I2C_STATE_NONE;         /* 赋值或初始化 */

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)               /* 条件编译 */
          hi2c->MemRxCpltCallback(hi2c);                /* 执行语句 */
#else                                                   /* 条件编译否则 */
          HAL_I2C_MemRxCpltCallback(hi2c);              /* HAL I2C操作 */
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          hi2c->Mode = HAL_I2C_MODE_NONE;               /* 赋值或初始化 */
          hi2c->PreviousState = I2C_STATE_MASTER_BUSY_RX;  /* 赋值或初始化 */

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)               /* 条件编译 */
          hi2c->MasterRxCpltCallback(hi2c);             /* 执行语句 */
#else                                                   /* 条件编译否则 */
          HAL_I2C_MasterRxCpltCallback(hi2c);           /* HAL I2C操作 */
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Disable EVT, BUF and ERR interrupt */
        __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);  /* HAL辅助宏/函数 */

        /* Read data from DR */
        *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;  /* 赋值或初始化 */

        /* Increment Buffer pointer */
        hi2c->pBuffPtr++;                               /* 执行语句 */

        /* Update counter */
        hi2c->XferCount--;                              /* 执行语句 */

        hi2c->State = HAL_I2C_STATE_READY;              /* 赋值或初始化 */
        hi2c->Mode = HAL_I2C_MODE_NONE;                 /* 赋值或初始化 */

        /* Call user error callback */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)               /* 条件编译 */
        hi2c->ErrorCallback(hi2c);                      /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_I2C_ErrorCallback(hi2c);                    /* HAL I2C操作 */
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Do nothing */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Handle BTF flag for Master receiver
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for I2C module
  * @retval None
  */
static void I2C_MasterReceive_BTF(I2C_HandleTypeDef *hi2c)  /* 静态函数 */
{                                                       /* 代码块开始 */
  /* Declaration of temporary variables to prevent undefined behavior of volatile usage */
  uint32_t CurrentXferOptions = hi2c->XferOptions;      /* 32位无符号变量/函数 */

  if (hi2c->XferCount == 4U)                            /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable BUF interrupt, this help to treat correctly the last 2 bytes
       on BTF subroutine if there is a reception delay between N-1 and N byte */
    __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_BUF);             /* HAL辅助宏/函数 */

    /* Read data from DR */
    *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;      /* 赋值或初始化 */

    /* Increment Buffer pointer */
    hi2c->pBuffPtr++;                                   /* 执行语句 */

    /* Update counter */
    hi2c->XferCount--;                                  /* 执行语句 */
  }                                                     /* 代码块结束 */
  else if (hi2c->XferCount == 3U)                       /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* Disable BUF interrupt, this help to treat correctly the last 2 bytes
       on BTF subroutine if there is a reception delay between N-1 and N byte */
    __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_BUF);             /* HAL辅助宏/函数 */

    if ((CurrentXferOptions != I2C_NEXT_FRAME) && (CurrentXferOptions != I2C_FIRST_AND_NEXT_FRAME))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable Acknowledge */
      CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);      /* 执行语句 */
    }                                                   /* 代码块结束 */

    /* Read data from DR */
    *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;      /* 赋值或初始化 */

    /* Increment Buffer pointer */
    hi2c->pBuffPtr++;                                   /* 执行语句 */

    /* Update counter */
    hi2c->XferCount--;                                  /* 执行语句 */
  }                                                     /* 代码块结束 */
  else if (hi2c->XferCount == 2U)                       /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* Prepare next transfer or stop current transfer */
    if ((CurrentXferOptions == I2C_FIRST_FRAME) || (CurrentXferOptions == I2C_LAST_FRAME_NO_STOP))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable Acknowledge */
      CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);      /* 执行语句 */
    }                                                   /* 代码块结束 */
    else if ((CurrentXferOptions == I2C_NEXT_FRAME) || (CurrentXferOptions == I2C_FIRST_AND_NEXT_FRAME))  /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* Enable Acknowledge */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);        /* 执行语句 */
    }                                                   /* 代码块结束 */
    else if (CurrentXferOptions != I2C_LAST_FRAME_NO_STOP)  /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* Generate Stop */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);       /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Do nothing */
    }                                                   /* 代码块结束 */

    /* Read data from DR */
    *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;      /* 赋值或初始化 */

    /* Increment Buffer pointer */
    hi2c->pBuffPtr++;                                   /* 执行语句 */

    /* Update counter */
    hi2c->XferCount--;                                  /* 执行语句 */

    /* Read data from DR */
    *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;      /* 赋值或初始化 */

    /* Increment Buffer pointer */
    hi2c->pBuffPtr++;                                   /* 执行语句 */

    /* Update counter */
    hi2c->XferCount--;                                  /* 执行语句 */

    /* Disable EVT and ERR interrupt */
    __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_ERR);  /* HAL辅助宏/函数 */

    hi2c->State = HAL_I2C_STATE_READY;                  /* 赋值或初始化 */
    if (hi2c->Mode == HAL_I2C_MODE_MEM)                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      hi2c->Mode = HAL_I2C_MODE_NONE;                   /* 赋值或初始化 */
      hi2c->PreviousState = I2C_STATE_NONE;             /* 赋值或初始化 */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)               /* 条件编译 */
      hi2c->MemRxCpltCallback(hi2c);                    /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_I2C_MemRxCpltCallback(hi2c);                  /* HAL I2C操作 */
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hi2c->Mode = HAL_I2C_MODE_NONE;                   /* 赋值或初始化 */
      hi2c->PreviousState = I2C_STATE_MASTER_BUSY_RX;   /* 赋值或初始化 */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)               /* 条件编译 */
      hi2c->MasterRxCpltCallback(hi2c);                 /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_I2C_MasterRxCpltCallback(hi2c);               /* HAL I2C操作 */
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Read data from DR */
    *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;      /* 赋值或初始化 */

    /* Increment Buffer pointer */
    hi2c->pBuffPtr++;                                   /* 执行语句 */

    /* Update counter */
    hi2c->XferCount--;                                  /* 执行语句 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Handle SB flag for Master
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for I2C module
  * @retval None
  */
static void I2C_Master_SB(I2C_HandleTypeDef *hi2c)      /* 静态函数 */
{                                                       /* 代码块开始 */
  if (hi2c->Mode == HAL_I2C_MODE_MEM)                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (hi2c->EventCount == 0U)                         /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Send slave address */
      hi2c->Instance->DR = I2C_7BIT_ADD_WRITE(hi2c->Devaddress);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hi2c->Instance->DR = I2C_7BIT_ADD_READ(hi2c->Devaddress);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    if (hi2c->Init.AddressingMode == I2C_ADDRESSINGMODE_7BIT)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Send slave 7 Bits address */
      if (hi2c->State == HAL_I2C_STATE_BUSY_TX)         /* 条件判断 */
      {                                                 /* 代码块开始 */
        hi2c->Instance->DR = I2C_7BIT_ADD_WRITE(hi2c->Devaddress);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        hi2c->Instance->DR = I2C_7BIT_ADD_READ(hi2c->Devaddress);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */

      if (((hi2c->hdmatx != NULL) && (hi2c->hdmatx->XferCpltCallback != NULL))  /* 条件判断 */
          || ((hi2c->hdmarx != NULL) && (hi2c->hdmarx->XferCpltCallback != NULL)))  /* 源代码 */
      {                                                 /* 代码块开始 */
        /* Enable DMA Request */
        SET_BIT(hi2c->Instance->CR2, I2C_CR2_DMAEN);    /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      if (hi2c->EventCount == 0U)                       /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Send header of slave address */
        hi2c->Instance->DR = I2C_10BIT_HEADER_WRITE(hi2c->Devaddress);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else if (hi2c->EventCount == 1U)                  /* 否则如果 */
      {                                                 /* 代码块开始 */
        /* Send header of slave address */
        hi2c->Instance->DR = I2C_10BIT_HEADER_READ(hi2c->Devaddress);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Do nothing */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Handle ADD10 flag for Master
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for I2C module
  * @retval None
  */
static void I2C_Master_ADD10(I2C_HandleTypeDef *hi2c)   /* 静态函数 */
{                                                       /* 代码块开始 */
  /* Send slave address */
  hi2c->Instance->DR = I2C_10BIT_ADDRESS(hi2c->Devaddress);  /* 赋值或初始化 */

  if (((hi2c->hdmatx != NULL) && (hi2c->hdmatx->XferCpltCallback != NULL))  /* 条件判断 */
      || ((hi2c->hdmarx != NULL) && (hi2c->hdmarx->XferCpltCallback != NULL)))  /* 源代码 */
  {                                                     /* 代码块开始 */
    /* Enable DMA Request */
    SET_BIT(hi2c->Instance->CR2, I2C_CR2_DMAEN);        /* 执行语句 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Handle ADDR flag for Master
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for I2C module
  * @retval None
  */
static void I2C_Master_ADDR(I2C_HandleTypeDef *hi2c)    /* 静态函数 */
{                                                       /* 代码块开始 */
  /* Declaration of temporary variable to prevent undefined behavior of volatile usage */
  HAL_I2C_ModeTypeDef CurrentMode       = hi2c->Mode;   /* HAL I2C操作 */
  uint32_t CurrentXferOptions           = hi2c->XferOptions;  /* 32位无符号变量/函数 */
  uint32_t Prev_State                   = hi2c->PreviousState;  /* 32位无符号变量/函数 */

  if (hi2c->State == HAL_I2C_STATE_BUSY_RX)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((hi2c->EventCount == 0U) && (CurrentMode == HAL_I2C_MODE_MEM))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear ADDR flag */
      __HAL_I2C_CLEAR_ADDRFLAG(hi2c);                   /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
    else if ((hi2c->EventCount == 0U) && (hi2c->Init.AddressingMode == I2C_ADDRESSINGMODE_10BIT))  /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* Clear ADDR flag */
      __HAL_I2C_CLEAR_ADDRFLAG(hi2c);                   /* HAL辅助宏/函数 */

      /* Generate Restart */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);      /* 执行语句 */

      hi2c->EventCount++;                               /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      if (hi2c->XferCount == 0U)                        /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Clear ADDR flag */
        __HAL_I2C_CLEAR_ADDRFLAG(hi2c);                 /* HAL辅助宏/函数 */

        /* Generate Stop */
        SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);     /* 执行语句 */
      }                                                 /* 代码块结束 */
      else if (hi2c->XferCount == 1U)                   /* 否则如果 */
      {                                                 /* 代码块开始 */
        if (CurrentXferOptions == I2C_NO_OPTION_FRAME)  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Disable Acknowledge */
          CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);  /* 执行语句 */

          if ((hi2c->Instance->CR2 & I2C_CR2_DMAEN) == I2C_CR2_DMAEN)  /* 条件判断 */
          {                                             /* 代码块开始 */
            /* Disable Acknowledge */
            CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);  /* 执行语句 */

            /* Clear ADDR flag */
            __HAL_I2C_CLEAR_ADDRFLAG(hi2c);             /* HAL辅助宏/函数 */
          }                                             /* 代码块结束 */
          else                                          /* 否则 */
          {                                             /* 代码块开始 */
            /* Clear ADDR flag */
            __HAL_I2C_CLEAR_ADDRFLAG(hi2c);             /* HAL辅助宏/函数 */

            /* Generate Stop */
            SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);  /* 执行语句 */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */
        /* Prepare next transfer or stop current transfer */
        else if ((CurrentXferOptions != I2C_FIRST_AND_LAST_FRAME) && (CurrentXferOptions != I2C_LAST_FRAME) \
                 && ((Prev_State != I2C_STATE_MASTER_BUSY_RX) || (CurrentXferOptions == I2C_FIRST_FRAME)))  /* 源代码 */
        {                                               /* 代码块开始 */
          if ((CurrentXferOptions != I2C_NEXT_FRAME) && (CurrentXferOptions != I2C_FIRST_AND_NEXT_FRAME) && (CurrentXferOptions != I2C_LAST_FRAME_NO_STOP))  /* 条件判断 */
          {                                             /* 代码块开始 */
            /* Disable Acknowledge */
            CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);  /* 执行语句 */
          }                                             /* 代码块结束 */
          else                                          /* 否则 */
          {                                             /* 代码块开始 */
            /* Enable Acknowledge */
            SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);  /* 执行语句 */
          }                                             /* 代码块结束 */

          /* Clear ADDR flag */
          __HAL_I2C_CLEAR_ADDRFLAG(hi2c);               /* HAL辅助宏/函数 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          /* Disable Acknowledge */
          CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);  /* 执行语句 */

          /* Clear ADDR flag */
          __HAL_I2C_CLEAR_ADDRFLAG(hi2c);               /* HAL辅助宏/函数 */

          /* Generate Stop */
          SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);   /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else if (hi2c->XferCount == 2U)                   /* 否则如果 */
      {                                                 /* 代码块开始 */
        if ((CurrentXferOptions != I2C_NEXT_FRAME) && (CurrentXferOptions != I2C_FIRST_AND_NEXT_FRAME) && (CurrentXferOptions != I2C_LAST_FRAME_NO_STOP))  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Disable Acknowledge */
          CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);  /* 执行语句 */

          /* Enable Pos */
          SET_BIT(hi2c->Instance->CR1, I2C_CR1_POS);    /* 执行语句 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          /* Enable Acknowledge */
          SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);    /* 执行语句 */
        }                                               /* 代码块结束 */

        if (((hi2c->Instance->CR2 & I2C_CR2_DMAEN) == I2C_CR2_DMAEN) && ((CurrentXferOptions == I2C_NO_OPTION_FRAME) || (CurrentXferOptions == I2C_FIRST_FRAME) || (CurrentXferOptions == I2C_FIRST_AND_LAST_FRAME) || (CurrentXferOptions == I2C_LAST_FRAME_NO_STOP) || (CurrentXferOptions == I2C_LAST_FRAME)))  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Enable Last DMA bit */
          SET_BIT(hi2c->Instance->CR2, I2C_CR2_LAST);   /* 执行语句 */
        }                                               /* 代码块结束 */

        /* Clear ADDR flag */
        __HAL_I2C_CLEAR_ADDRFLAG(hi2c);                 /* HAL辅助宏/函数 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Enable Acknowledge */
        SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);      /* 执行语句 */

        if (((hi2c->Instance->CR2 & I2C_CR2_DMAEN) == I2C_CR2_DMAEN) && ((CurrentXferOptions == I2C_NO_OPTION_FRAME) || (CurrentXferOptions == I2C_FIRST_FRAME) || (CurrentXferOptions == I2C_FIRST_AND_LAST_FRAME) || (CurrentXferOptions == I2C_LAST_FRAME_NO_STOP) || (CurrentXferOptions == I2C_LAST_FRAME)))  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Enable Last DMA bit */
          SET_BIT(hi2c->Instance->CR2, I2C_CR2_LAST);   /* 执行语句 */
        }                                               /* 代码块结束 */

        /* Clear ADDR flag */
        __HAL_I2C_CLEAR_ADDRFLAG(hi2c);                 /* HAL辅助宏/函数 */
      }                                                 /* 代码块结束 */

      /* Reset Event counter  */
      hi2c->EventCount = 0U;                            /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Clear ADDR flag */
    __HAL_I2C_CLEAR_ADDRFLAG(hi2c);                     /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Handle TXE flag for Slave
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for I2C module
  * @retval None
  */
static void I2C_SlaveTransmit_TXE(I2C_HandleTypeDef *hi2c)  /* 静态函数 */
{                                                       /* 代码块开始 */
  /* Declaration of temporary variables to prevent undefined behavior of volatile usage */
  HAL_I2C_StateTypeDef CurrentState = hi2c->State;      /* HAL I2C操作 */

  if (hi2c->XferCount != 0U)                            /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Write data to DR */
    hi2c->Instance->DR = *hi2c->pBuffPtr;               /* 赋值或初始化 */

    /* Increment Buffer pointer */
    hi2c->pBuffPtr++;                                   /* 执行语句 */

    /* Update counter */
    hi2c->XferCount--;                                  /* 执行语句 */

    if ((hi2c->XferCount == 0U) && (CurrentState == HAL_I2C_STATE_BUSY_TX_LISTEN))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Last Byte is received, disable Interrupt */
      __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_BUF);           /* HAL辅助宏/函数 */

      /* Set state at HAL_I2C_STATE_LISTEN */
      hi2c->PreviousState = I2C_STATE_SLAVE_BUSY_TX;    /* 赋值或初始化 */
      hi2c->State = HAL_I2C_STATE_LISTEN;               /* 赋值或初始化 */

      /* Call the corresponding callback to inform upper layer of End of Transfer */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)               /* 条件编译 */
      hi2c->SlaveTxCpltCallback(hi2c);                  /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_I2C_SlaveTxCpltCallback(hi2c);                /* HAL I2C操作 */
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Handle BTF flag for Slave transmitter
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for I2C module
  * @retval None
  */
static void I2C_SlaveTransmit_BTF(I2C_HandleTypeDef *hi2c)  /* 静态函数 */
{                                                       /* 代码块开始 */
  if (hi2c->XferCount != 0U)                            /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Write data to DR */
    hi2c->Instance->DR = *hi2c->pBuffPtr;               /* 赋值或初始化 */

    /* Increment Buffer pointer */
    hi2c->pBuffPtr++;                                   /* 执行语句 */

    /* Update counter */
    hi2c->XferCount--;                                  /* 执行语句 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Handle RXNE flag for Slave
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for I2C module
  * @retval None
  */
static void I2C_SlaveReceive_RXNE(I2C_HandleTypeDef *hi2c)  /* 静态函数 */
{                                                       /* 代码块开始 */
  /* Declaration of temporary variables to prevent undefined behavior of volatile usage */
  HAL_I2C_StateTypeDef CurrentState = hi2c->State;      /* HAL I2C操作 */

  if (hi2c->XferCount != 0U)                            /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Read data from DR */
    *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;      /* 赋值或初始化 */

    /* Increment Buffer pointer */
    hi2c->pBuffPtr++;                                   /* 执行语句 */

    /* Update counter */
    hi2c->XferCount--;                                  /* 执行语句 */

    if ((hi2c->XferCount == 0U) && (CurrentState == HAL_I2C_STATE_BUSY_RX_LISTEN))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Last Byte is received, disable Interrupt */
      __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_BUF);           /* HAL辅助宏/函数 */

      /* Set state at HAL_I2C_STATE_LISTEN */
      hi2c->PreviousState = I2C_STATE_SLAVE_BUSY_RX;    /* 赋值或初始化 */
      hi2c->State = HAL_I2C_STATE_LISTEN;               /* 赋值或初始化 */

      /* Call the corresponding callback to inform upper layer of End of Transfer */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)               /* 条件编译 */
      hi2c->SlaveRxCpltCallback(hi2c);                  /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_I2C_SlaveRxCpltCallback(hi2c);                /* HAL I2C操作 */
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Handle BTF flag for Slave receiver
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for I2C module
  * @retval None
  */
static void I2C_SlaveReceive_BTF(I2C_HandleTypeDef *hi2c)  /* 静态函数 */
{                                                       /* 代码块开始 */
  if (hi2c->XferCount != 0U)                            /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Read data from DR */
    *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;      /* 赋值或初始化 */

    /* Increment Buffer pointer */
    hi2c->pBuffPtr++;                                   /* 执行语句 */

    /* Update counter */
    hi2c->XferCount--;                                  /* 执行语句 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Handle ADD flag for Slave
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for I2C module
  * @param  IT2Flags Interrupt2 flags to handle.
  * @retval None
  */
static void I2C_Slave_ADDR(I2C_HandleTypeDef *hi2c, uint32_t IT2Flags)  /* 静态函数 */
{                                                       /* 代码块开始 */
  uint8_t TransferDirection = I2C_DIRECTION_RECEIVE;    /* 8位无符号变量/函数 */
  uint16_t SlaveAddrCode;                               /* 16位无符号变量/函数 */

  if (((uint32_t)hi2c->State & (uint32_t)HAL_I2C_STATE_LISTEN) == (uint32_t)HAL_I2C_STATE_LISTEN)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable BUF interrupt, BUF enabling is manage through slave specific interface */
    __HAL_I2C_DISABLE_IT(hi2c, (I2C_IT_BUF));           /* HAL辅助宏/函数 */

    /* Transfer Direction requested by Master */
    if (I2C_CHECK_FLAG(IT2Flags, I2C_FLAG_TRA) == RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      TransferDirection = I2C_DIRECTION_TRANSMIT;       /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    if (I2C_CHECK_FLAG(IT2Flags, I2C_FLAG_DUALF) == RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      SlaveAddrCode = (uint16_t)hi2c->Init.OwnAddress1;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      SlaveAddrCode = (uint16_t)hi2c->Init.OwnAddress2;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Process Unlocked */
    __HAL_UNLOCK(hi2c);                                 /* HAL辅助宏/函数 */

    /* Call Slave Addr callback */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)               /* 条件编译 */
    hi2c->AddrCallback(hi2c, TransferDirection, SlaveAddrCode);  /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_I2C_AddrCallback(hi2c, TransferDirection, SlaveAddrCode);  /* HAL I2C操作 */
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Clear ADDR flag */
    __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_ADDR);          /* HAL辅助宏/函数 */

    /* Process Unlocked */
    __HAL_UNLOCK(hi2c);                                 /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Handle STOPF flag for Slave
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for I2C module
  * @retval None
  */
static void I2C_Slave_STOPF(I2C_HandleTypeDef *hi2c)    /* 静态函数 */
{                                                       /* 代码块开始 */
  /* Declaration of temporary variable to prevent undefined behavior of volatile usage */
  HAL_I2C_StateTypeDef CurrentState = hi2c->State;      /* HAL I2C操作 */

  /* Disable EVT, BUF and ERR interrupt */
  __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);  /* HAL辅助宏/函数 */

  /* Clear STOPF flag */
  __HAL_I2C_CLEAR_STOPFLAG(hi2c);                       /* HAL辅助宏/函数 */

  /* Disable Acknowledge */
  CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);          /* 执行语句 */

  /* If a DMA is ongoing, Update handle size context */
  if ((hi2c->Instance->CR2 & I2C_CR2_DMAEN) == I2C_CR2_DMAEN)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((CurrentState == HAL_I2C_STATE_BUSY_RX) || (CurrentState == HAL_I2C_STATE_BUSY_RX_LISTEN))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hi2c->XferCount = (uint16_t)(__HAL_DMA_GET_COUNTER(hi2c->hdmarx));  /* 赋值或初始化 */

      if (hi2c->XferCount != 0U)                        /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Set ErrorCode corresponding to a Non-Acknowledge */
        hi2c->ErrorCode |= HAL_I2C_ERROR_AF;            /* 赋值或初始化 */
      }                                                 /* 代码块结束 */

      /* Disable, stop the current DMA */
      CLEAR_BIT(hi2c->Instance->CR2, I2C_CR2_DMAEN);    /* 执行语句 */

      /* Abort DMA Xfer if any */
      if (HAL_DMA_GetState(hi2c->hdmarx) != HAL_DMA_STATE_READY)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Set the I2C DMA Abort callback :
        will lead to call HAL_I2C_ErrorCallback() at end of DMA abort procedure */
        hi2c->hdmarx->XferAbortCallback = I2C_DMAAbort;  /* 赋值或初始化 */

        /* Abort DMA RX */
        if (HAL_DMA_Abort_IT(hi2c->hdmarx) != HAL_OK)   /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Call Directly XferAbortCallback function in case of error */
          hi2c->hdmarx->XferAbortCallback(hi2c->hdmarx);  /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hi2c->XferCount = (uint16_t)(__HAL_DMA_GET_COUNTER(hi2c->hdmatx));  /* 赋值或初始化 */

      if (hi2c->XferCount != 0U)                        /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Set ErrorCode corresponding to a Non-Acknowledge */
        hi2c->ErrorCode |= HAL_I2C_ERROR_AF;            /* 赋值或初始化 */
      }                                                 /* 代码块结束 */

      /* Disable, stop the current DMA */
      CLEAR_BIT(hi2c->Instance->CR2, I2C_CR2_DMAEN);    /* 执行语句 */

      /* Abort DMA Xfer if any */
      if (HAL_DMA_GetState(hi2c->hdmatx) != HAL_DMA_STATE_READY)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Set the I2C DMA Abort callback :
        will lead to call HAL_I2C_ErrorCallback() at end of DMA abort procedure */
        hi2c->hdmatx->XferAbortCallback = I2C_DMAAbort;  /* 赋值或初始化 */

        /* Abort DMA TX */
        if (HAL_DMA_Abort_IT(hi2c->hdmatx) != HAL_OK)   /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Call Directly XferAbortCallback function in case of error */
          hi2c->hdmatx->XferAbortCallback(hi2c->hdmatx);  /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* All data are not transferred, so set error code accordingly */
  if (hi2c->XferCount != 0U)                            /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Store Last receive data if any */
    if (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BTF) == SET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Read data from DR */
      *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;    /* 赋值或初始化 */

      /* Increment Buffer pointer */
      hi2c->pBuffPtr++;                                 /* 执行语句 */

      /* Update counter */
      hi2c->XferCount--;                                /* 执行语句 */
    }                                                   /* 代码块结束 */

    /* Store Last receive data if any */
    if (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_RXNE) == SET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Read data from DR */
      *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;    /* 赋值或初始化 */

      /* Increment Buffer pointer */
      hi2c->pBuffPtr++;                                 /* 执行语句 */

      /* Update counter */
      hi2c->XferCount--;                                /* 执行语句 */
    }                                                   /* 代码块结束 */

    if (hi2c->XferCount != 0U)                          /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set ErrorCode corresponding to a Non-Acknowledge */
      hi2c->ErrorCode |= HAL_I2C_ERROR_AF;              /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  if (hi2c->ErrorCode != HAL_I2C_ERROR_NONE)            /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Call the corresponding callback to inform upper layer of End of Transfer */
    I2C_ITError(hi2c);                                  /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    if (CurrentState == HAL_I2C_STATE_BUSY_RX_LISTEN)   /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set state at HAL_I2C_STATE_LISTEN */
      hi2c->PreviousState = I2C_STATE_NONE;             /* 赋值或初始化 */
      hi2c->State = HAL_I2C_STATE_LISTEN;               /* 赋值或初始化 */

      /* Call the corresponding callback to inform upper layer of End of Transfer */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)               /* 条件编译 */
      hi2c->SlaveRxCpltCallback(hi2c);                  /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_I2C_SlaveRxCpltCallback(hi2c);                /* HAL I2C操作 */
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */

    if (hi2c->State == HAL_I2C_STATE_LISTEN)            /* 条件判断 */
    {                                                   /* 代码块开始 */
      hi2c->XferOptions = I2C_NO_OPTION_FRAME;          /* 赋值或初始化 */
      hi2c->PreviousState = I2C_STATE_NONE;             /* 赋值或初始化 */
      hi2c->State = HAL_I2C_STATE_READY;                /* 赋值或初始化 */
      hi2c->Mode = HAL_I2C_MODE_NONE;                   /* 赋值或初始化 */

      /* Call the Listen Complete callback, to inform upper layer of the end of Listen usecase */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)               /* 条件编译 */
      hi2c->ListenCpltCallback(hi2c);                   /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_I2C_ListenCpltCallback(hi2c);                 /* HAL I2C操作 */
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      if ((hi2c->PreviousState  == I2C_STATE_SLAVE_BUSY_RX) || (CurrentState == HAL_I2C_STATE_BUSY_RX))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        hi2c->PreviousState = I2C_STATE_NONE;           /* 赋值或初始化 */
        hi2c->State = HAL_I2C_STATE_READY;              /* 赋值或初始化 */
        hi2c->Mode = HAL_I2C_MODE_NONE;                 /* 赋值或初始化 */

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)               /* 条件编译 */
        hi2c->SlaveRxCpltCallback(hi2c);                /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_I2C_SlaveRxCpltCallback(hi2c);              /* HAL I2C操作 */
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for I2C module
  * @retval None
  */
static void I2C_Slave_AF(I2C_HandleTypeDef *hi2c)       /* 静态函数 */
{                                                       /* 代码块开始 */
  /* Declaration of temporary variables to prevent undefined behavior of volatile usage */
  HAL_I2C_StateTypeDef CurrentState = hi2c->State;      /* HAL I2C操作 */
  uint32_t CurrentXferOptions       = hi2c->XferOptions;  /* 32位无符号变量/函数 */

  if (((CurrentXferOptions ==  I2C_FIRST_AND_LAST_FRAME) || (CurrentXferOptions == I2C_LAST_FRAME)) && \
      (CurrentState == HAL_I2C_STATE_LISTEN))           /* 源代码 */
  {                                                     /* 代码块开始 */
    hi2c->XferOptions = I2C_NO_OPTION_FRAME;            /* 赋值或初始化 */

    /* Disable EVT, BUF and ERR interrupt */
    __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);  /* HAL辅助宏/函数 */

    /* Clear AF flag */
    __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_AF);            /* HAL辅助宏/函数 */

    /* Disable Acknowledge */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);        /* 执行语句 */

    hi2c->PreviousState = I2C_STATE_NONE;               /* 赋值或初始化 */
    hi2c->State         = HAL_I2C_STATE_READY;          /* 赋值或初始化 */
    hi2c->Mode          = HAL_I2C_MODE_NONE;            /* 赋值或初始化 */

    /* Call the Listen Complete callback, to inform upper layer of the end of Listen usecase */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)               /* 条件编译 */
    hi2c->ListenCpltCallback(hi2c);                     /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_I2C_ListenCpltCallback(hi2c);                   /* HAL I2C操作 */
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
  else if (CurrentState == HAL_I2C_STATE_BUSY_TX)       /* 否则如果 */
  {                                                     /* 代码块开始 */
    hi2c->XferOptions   = I2C_NO_OPTION_FRAME;          /* 赋值或初始化 */
    hi2c->PreviousState = I2C_STATE_SLAVE_BUSY_TX;      /* 赋值或初始化 */
    hi2c->State         = HAL_I2C_STATE_READY;          /* 赋值或初始化 */
    hi2c->Mode          = HAL_I2C_MODE_NONE;            /* 赋值或初始化 */

    /* Disable EVT, BUF and ERR interrupt */
    __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);  /* HAL辅助宏/函数 */

    /* Clear AF flag */
    __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_AF);            /* HAL辅助宏/函数 */

    /* Disable Acknowledge */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);        /* 执行语句 */

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)               /* 条件编译 */
    hi2c->SlaveTxCpltCallback(hi2c);                    /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_I2C_SlaveTxCpltCallback(hi2c);                  /* HAL I2C操作 */
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Clear AF flag only */
    /* State Listen, but XferOptions == FIRST or NEXT */
    __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_AF);            /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  I2C interrupts error process
  * @param  hi2c I2C handle.
  * @retval None
  */
static void I2C_ITError(I2C_HandleTypeDef *hi2c)        /* 静态函数 */
{                                                       /* 代码块开始 */
  /* Declaration of temporary variable to prevent undefined behavior of volatile usage */
  HAL_I2C_StateTypeDef CurrentState = hi2c->State;      /* HAL I2C操作 */
  HAL_I2C_ModeTypeDef CurrentMode = hi2c->Mode;         /* HAL I2C操作 */
  uint32_t CurrentError;                                /* 32位无符号变量/函数 */

  if (((CurrentMode == HAL_I2C_MODE_MASTER) || (CurrentMode == HAL_I2C_MODE_MEM)) && (CurrentState == HAL_I2C_STATE_BUSY_RX))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable Pos bit in I2C CR1 when error occurred in Master/Mem Receive IT Process */
    hi2c->Instance->CR1 &= ~I2C_CR1_POS;                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if (((uint32_t)CurrentState & (uint32_t)HAL_I2C_STATE_LISTEN) == (uint32_t)HAL_I2C_STATE_LISTEN)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* keep HAL_I2C_STATE_LISTEN */
    hi2c->PreviousState = I2C_STATE_NONE;               /* 赋值或初始化 */
    hi2c->State = HAL_I2C_STATE_LISTEN;                 /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* If state is an abort treatment on going, don't change state */
    /* This change will be do later */
    if ((READ_BIT(hi2c->Instance->CR2, I2C_CR2_DMAEN) != I2C_CR2_DMAEN) && (CurrentState != HAL_I2C_STATE_ABORT))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hi2c->State = HAL_I2C_STATE_READY;                /* 赋值或初始化 */
      hi2c->Mode = HAL_I2C_MODE_NONE;                   /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    hi2c->PreviousState = I2C_STATE_NONE;               /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Abort DMA transfer */
  if (READ_BIT(hi2c->Instance->CR2, I2C_CR2_DMAEN) == I2C_CR2_DMAEN)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hi2c->Instance->CR2 &= ~I2C_CR2_DMAEN;              /* 赋值或初始化 */

    if (hi2c->hdmatx->State != HAL_DMA_STATE_READY)     /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the DMA Abort callback :
      will lead to call HAL_I2C_ErrorCallback() at end of DMA abort procedure */
      hi2c->hdmatx->XferAbortCallback = I2C_DMAAbort;   /* 赋值或初始化 */

      if (HAL_DMA_Abort_IT(hi2c->hdmatx) != HAL_OK)     /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Disable I2C peripheral to prevent dummy data in buffer */
        __HAL_I2C_DISABLE(hi2c);                        /* HAL辅助宏/函数 */

        hi2c->State = HAL_I2C_STATE_READY;              /* 赋值或初始化 */

        /* Call Directly XferAbortCallback function in case of error */
        hi2c->hdmatx->XferAbortCallback(hi2c->hdmatx);  /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Set the DMA Abort callback :
      will lead to call HAL_I2C_ErrorCallback() at end of DMA abort procedure */
      hi2c->hdmarx->XferAbortCallback = I2C_DMAAbort;   /* 赋值或初始化 */

      if (HAL_DMA_Abort_IT(hi2c->hdmarx) != HAL_OK)     /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Store Last receive data if any */
        if (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_RXNE) == SET)  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Read data from DR */
          *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;  /* 赋值或初始化 */

          /* Increment Buffer pointer */
          hi2c->pBuffPtr++;                             /* 执行语句 */
        }                                               /* 代码块结束 */

        /* Disable I2C peripheral to prevent dummy data in buffer */
        __HAL_I2C_DISABLE(hi2c);                        /* HAL辅助宏/函数 */

        hi2c->State = HAL_I2C_STATE_READY;              /* 赋值或初始化 */

        /* Call Directly hi2c->hdmarx->XferAbortCallback function in case of error */
        hi2c->hdmarx->XferAbortCallback(hi2c->hdmarx);  /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (hi2c->State == HAL_I2C_STATE_ABORT)          /* 否则如果 */
  {                                                     /* 代码块开始 */
    hi2c->State = HAL_I2C_STATE_READY;                  /* 赋值或初始化 */
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;               /* 赋值或初始化 */

    /* Store Last receive data if any */
    if (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_RXNE) == SET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Read data from DR */
      *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;    /* 赋值或初始化 */

      /* Increment Buffer pointer */
      hi2c->pBuffPtr++;                                 /* 执行语句 */
    }                                                   /* 代码块结束 */

    /* Disable I2C peripheral to prevent dummy data in buffer */
    __HAL_I2C_DISABLE(hi2c);                            /* HAL辅助宏/函数 */

    /* Call the corresponding callback to inform upper layer of End of Transfer */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)               /* 条件编译 */
    hi2c->AbortCpltCallback(hi2c);                      /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_I2C_AbortCpltCallback(hi2c);                    /* HAL I2C操作 */
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Store Last receive data if any */
    if (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_RXNE) == SET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Read data from DR */
      *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;    /* 赋值或初始化 */

      /* Increment Buffer pointer */
      hi2c->pBuffPtr++;                                 /* 执行语句 */
    }                                                   /* 代码块结束 */

    /* Call user error callback */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)               /* 条件编译 */
    hi2c->ErrorCallback(hi2c);                          /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_I2C_ErrorCallback(hi2c);                        /* HAL I2C操作 */
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */

  /* STOP Flag is not set after a NACK reception, BusError, ArbitrationLost, OverRun */
  CurrentError = hi2c->ErrorCode;                       /* 赋值或初始化 */

  if (((CurrentError & HAL_I2C_ERROR_BERR) == HAL_I2C_ERROR_BERR) || \
      ((CurrentError & HAL_I2C_ERROR_ARLO) == HAL_I2C_ERROR_ARLO) || \
      ((CurrentError & HAL_I2C_ERROR_AF) == HAL_I2C_ERROR_AF)     || \
      ((CurrentError & HAL_I2C_ERROR_OVR) == HAL_I2C_ERROR_OVR))  /* 源代码 */
  {                                                     /* 代码块开始 */
    /* Disable EVT, BUF and ERR interrupt */
    __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* So may inform upper layer that listen phase is stopped */
  /* during NACK error treatment */
  CurrentState = hi2c->State;                           /* 赋值或初始化 */
  if (((hi2c->ErrorCode & HAL_I2C_ERROR_AF) == HAL_I2C_ERROR_AF) && (CurrentState == HAL_I2C_STATE_LISTEN))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hi2c->XferOptions   = I2C_NO_OPTION_FRAME;          /* 赋值或初始化 */
    hi2c->PreviousState = I2C_STATE_NONE;               /* 赋值或初始化 */
    hi2c->State         = HAL_I2C_STATE_READY;          /* 赋值或初始化 */
    hi2c->Mode          = HAL_I2C_MODE_NONE;            /* 赋值或初始化 */

    /* Call the Listen Complete callback, to inform upper layer of the end of Listen usecase */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)               /* 条件编译 */
    hi2c->ListenCpltCallback(hi2c);                     /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_I2C_ListenCpltCallback(hi2c);                   /* HAL I2C操作 */
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for I2C module
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  Timeout Timeout duration
  * @param  Tickstart Tick start value
  * @retval HAL status
  */
static HAL_StatusTypeDef I2C_MasterRequestWrite(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint32_t Timeout, uint32_t Tickstart)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  /* Declaration of temporary variable to prevent undefined behavior of volatile usage */
  uint32_t CurrentXferOptions = hi2c->XferOptions;      /* 32位无符号变量/函数 */

  /* Generate Start condition if first transfer */
  if ((CurrentXferOptions == I2C_FIRST_AND_LAST_FRAME) || (CurrentXferOptions == I2C_FIRST_FRAME) || (CurrentXferOptions == I2C_NO_OPTION_FRAME))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Generate Start */
    SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);        /* 执行语句 */
  }                                                     /* 代码块结束 */
  else if (hi2c->PreviousState == I2C_STATE_MASTER_BUSY_RX)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* Generate ReStart */
    SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);        /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Do nothing */
  }                                                     /* 代码块结束 */

  /* Wait until SB flag is set */
  if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_SB, RESET, Timeout, Tickstart) != HAL_OK)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (READ_BIT(hi2c->Instance->CR1, I2C_CR1_START) == I2C_CR1_START)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hi2c->ErrorCode = HAL_I2C_WRONG_START;            /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    return HAL_TIMEOUT;                                 /* 返回值 */
  }                                                     /* 代码块结束 */

  if (hi2c->Init.AddressingMode == I2C_ADDRESSINGMODE_7BIT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Send slave address */
    hi2c->Instance->DR = I2C_7BIT_ADD_WRITE(DevAddress);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Send header of slave address */
    hi2c->Instance->DR = I2C_10BIT_HEADER_WRITE(DevAddress);  /* 赋值或初始化 */

    /* Wait until ADD10 flag is set */
    if (I2C_WaitOnMasterAddressFlagUntilTimeout(hi2c, I2C_FLAG_ADD10, Timeout, Tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Send slave address */
    hi2c->Instance->DR = I2C_10BIT_ADDRESS(DevAddress);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Wait until ADDR flag is set */
  if (I2C_WaitOnMasterAddressFlagUntilTimeout(hi2c, I2C_FLAG_ADDR, Timeout, Tickstart) != HAL_OK)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Master sends target device address for read request.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for I2C module
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  Timeout Timeout duration
  * @param  Tickstart Tick start value
  * @retval HAL status
  */
static HAL_StatusTypeDef I2C_MasterRequestRead(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint32_t Timeout, uint32_t Tickstart)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  /* Declaration of temporary variable to prevent undefined behavior of volatile usage */
  uint32_t CurrentXferOptions = hi2c->XferOptions;      /* 32位无符号变量/函数 */

  /* Enable Acknowledge */
  SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);            /* 执行语句 */

  /* Generate Start condition if first transfer */
  if ((CurrentXferOptions == I2C_FIRST_AND_LAST_FRAME) || (CurrentXferOptions == I2C_FIRST_FRAME)  || (CurrentXferOptions == I2C_NO_OPTION_FRAME))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Generate Start */
    SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);        /* 执行语句 */
  }                                                     /* 代码块结束 */
  else if (hi2c->PreviousState == I2C_STATE_MASTER_BUSY_TX)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* Generate ReStart */
    SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);        /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Do nothing */
  }                                                     /* 代码块结束 */

  /* Wait until SB flag is set */
  if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_SB, RESET, Timeout, Tickstart) != HAL_OK)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (READ_BIT(hi2c->Instance->CR1, I2C_CR1_START) == I2C_CR1_START)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hi2c->ErrorCode = HAL_I2C_WRONG_START;            /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    return HAL_TIMEOUT;                                 /* 返回值 */
  }                                                     /* 代码块结束 */

  if (hi2c->Init.AddressingMode == I2C_ADDRESSINGMODE_7BIT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Send slave address */
    hi2c->Instance->DR = I2C_7BIT_ADD_READ(DevAddress);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Send header of slave address */
    hi2c->Instance->DR = I2C_10BIT_HEADER_WRITE(DevAddress);  /* 赋值或初始化 */

    /* Wait until ADD10 flag is set */
    if (I2C_WaitOnMasterAddressFlagUntilTimeout(hi2c, I2C_FLAG_ADD10, Timeout, Tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Send slave address */
    hi2c->Instance->DR = I2C_10BIT_ADDRESS(DevAddress);  /* 赋值或初始化 */

    /* Wait until ADDR flag is set */
    if (I2C_WaitOnMasterAddressFlagUntilTimeout(hi2c, I2C_FLAG_ADDR, Timeout, Tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Clear ADDR flag */
    __HAL_I2C_CLEAR_ADDRFLAG(hi2c);                     /* HAL辅助宏/函数 */

    /* Generate Restart */
    SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);        /* 执行语句 */

    /* Wait until SB flag is set */
    if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_SB, RESET, Timeout, Tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (READ_BIT(hi2c->Instance->CR1, I2C_CR1_START) == I2C_CR1_START)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        hi2c->ErrorCode = HAL_I2C_WRONG_START;          /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      return HAL_TIMEOUT;                               /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Send header of slave address */
    hi2c->Instance->DR = I2C_10BIT_HEADER_READ(DevAddress);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Wait until ADDR flag is set */
  if (I2C_WaitOnMasterAddressFlagUntilTimeout(hi2c, I2C_FLAG_ADDR, Timeout, Tickstart) != HAL_OK)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Master sends target device address followed by internal memory address for write request.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for I2C module
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  MemAddress Internal memory address
  * @param  MemAddSize Size of internal memory address
  * @param  Timeout Timeout duration
  * @param  Tickstart Tick start value
  * @retval HAL status
  */
static HAL_StatusTypeDef I2C_RequestMemoryWrite(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint32_t Timeout, uint32_t Tickstart)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  /* Generate Start */
  SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);          /* 执行语句 */

  /* Wait until SB flag is set */
  if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_SB, RESET, Timeout, Tickstart) != HAL_OK)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (READ_BIT(hi2c->Instance->CR1, I2C_CR1_START) == I2C_CR1_START)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hi2c->ErrorCode = HAL_I2C_WRONG_START;            /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    return HAL_TIMEOUT;                                 /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Send slave address */
  hi2c->Instance->DR = I2C_7BIT_ADD_WRITE(DevAddress);  /* 赋值或初始化 */

  /* Wait until ADDR flag is set */
  if (I2C_WaitOnMasterAddressFlagUntilTimeout(hi2c, I2C_FLAG_ADDR, Timeout, Tickstart) != HAL_OK)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Clear ADDR flag */
  __HAL_I2C_CLEAR_ADDRFLAG(hi2c);                       /* HAL辅助宏/函数 */

  /* Wait until TXE flag is set */
  if (I2C_WaitOnTXEFlagUntilTimeout(hi2c, Timeout, Tickstart) != HAL_OK)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (hi2c->ErrorCode == HAL_I2C_ERROR_AF)            /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Generate Stop */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);       /* 执行语句 */
    }                                                   /* 代码块结束 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* If Memory address size is 8Bit */
  if (MemAddSize == I2C_MEMADD_SIZE_8BIT)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Send Memory Address */
    hi2c->Instance->DR = I2C_MEM_ADD_LSB(MemAddress);   /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  /* If Memory address size is 16Bit */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Send MSB of Memory Address */
    hi2c->Instance->DR = I2C_MEM_ADD_MSB(MemAddress);   /* 赋值或初始化 */

    /* Wait until TXE flag is set */
    if (I2C_WaitOnTXEFlagUntilTimeout(hi2c, Timeout, Tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (hi2c->ErrorCode == HAL_I2C_ERROR_AF)          /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Generate Stop */
        SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);     /* 执行语句 */
      }                                                 /* 代码块结束 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Send LSB of Memory Address */
    hi2c->Instance->DR = I2C_MEM_ADD_LSB(MemAddress);   /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Master sends target device address followed by internal memory address for read request.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for I2C module
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  MemAddress Internal memory address
  * @param  MemAddSize Size of internal memory address
  * @param  Timeout Timeout duration
  * @param  Tickstart Tick start value
  * @retval HAL status
  */
static HAL_StatusTypeDef I2C_RequestMemoryRead(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint32_t Timeout, uint32_t Tickstart)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  /* Enable Acknowledge */
  SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);            /* 执行语句 */

  /* Generate Start */
  SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);          /* 执行语句 */

  /* Wait until SB flag is set */
  if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_SB, RESET, Timeout, Tickstart) != HAL_OK)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (READ_BIT(hi2c->Instance->CR1, I2C_CR1_START) == I2C_CR1_START)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hi2c->ErrorCode = HAL_I2C_WRONG_START;            /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    return HAL_TIMEOUT;                                 /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Send slave address */
  hi2c->Instance->DR = I2C_7BIT_ADD_WRITE(DevAddress);  /* 赋值或初始化 */

  /* Wait until ADDR flag is set */
  if (I2C_WaitOnMasterAddressFlagUntilTimeout(hi2c, I2C_FLAG_ADDR, Timeout, Tickstart) != HAL_OK)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Clear ADDR flag */
  __HAL_I2C_CLEAR_ADDRFLAG(hi2c);                       /* HAL辅助宏/函数 */

  /* Wait until TXE flag is set */
  if (I2C_WaitOnTXEFlagUntilTimeout(hi2c, Timeout, Tickstart) != HAL_OK)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (hi2c->ErrorCode == HAL_I2C_ERROR_AF)            /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Generate Stop */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);       /* 执行语句 */
    }                                                   /* 代码块结束 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* If Memory address size is 8Bit */
  if (MemAddSize == I2C_MEMADD_SIZE_8BIT)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Send Memory Address */
    hi2c->Instance->DR = I2C_MEM_ADD_LSB(MemAddress);   /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  /* If Memory address size is 16Bit */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Send MSB of Memory Address */
    hi2c->Instance->DR = I2C_MEM_ADD_MSB(MemAddress);   /* 赋值或初始化 */

    /* Wait until TXE flag is set */
    if (I2C_WaitOnTXEFlagUntilTimeout(hi2c, Timeout, Tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (hi2c->ErrorCode == HAL_I2C_ERROR_AF)          /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Generate Stop */
        SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);     /* 执行语句 */
      }                                                 /* 代码块结束 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Send LSB of Memory Address */
    hi2c->Instance->DR = I2C_MEM_ADD_LSB(MemAddress);   /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Wait until TXE flag is set */
  if (I2C_WaitOnTXEFlagUntilTimeout(hi2c, Timeout, Tickstart) != HAL_OK)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (hi2c->ErrorCode == HAL_I2C_ERROR_AF)            /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Generate Stop */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);       /* 执行语句 */
    }                                                   /* 代码块结束 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Generate Restart */
  SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);          /* 执行语句 */

  /* Wait until SB flag is set */
  if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_SB, RESET, Timeout, Tickstart) != HAL_OK)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (READ_BIT(hi2c->Instance->CR1, I2C_CR1_START) == I2C_CR1_START)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hi2c->ErrorCode = HAL_I2C_WRONG_START;            /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    return HAL_TIMEOUT;                                 /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Send slave address */
  hi2c->Instance->DR = I2C_7BIT_ADD_READ(DevAddress);   /* 赋值或初始化 */

  /* Wait until ADDR flag is set */
  if (I2C_WaitOnMasterAddressFlagUntilTimeout(hi2c, I2C_FLAG_ADDR, Timeout, Tickstart) != HAL_OK)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA I2C process complete callback.
  * @param  hdma DMA handle
  * @retval None
  */
static void I2C_DMAXferCplt(DMA_HandleTypeDef *hdma)    /* 静态函数 */
{                                                       /* 代码块开始 */
  I2C_HandleTypeDef *hi2c = (I2C_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent; /* Derogation MISRAC2012-Rule-11.5 */

  /* Declaration of temporary variable to prevent undefined behavior of volatile usage */
  HAL_I2C_StateTypeDef CurrentState = hi2c->State;      /* HAL I2C操作 */
  HAL_I2C_ModeTypeDef CurrentMode   = hi2c->Mode;       /* HAL I2C操作 */
  uint32_t CurrentXferOptions       = hi2c->XferOptions;  /* 32位无符号变量/函数 */

  /* Disable EVT and ERR interrupt */
  __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_ERR);  /* HAL辅助宏/函数 */

  /* Clear Complete callback */
  if (hi2c->hdmatx != NULL)                             /* 条件判断 */
  {                                                     /* 代码块开始 */
    hi2c->hdmatx->XferCpltCallback = NULL;              /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  if (hi2c->hdmarx != NULL)                             /* 条件判断 */
  {                                                     /* 代码块开始 */
    hi2c->hdmarx->XferCpltCallback = NULL;              /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if ((((uint32_t)CurrentState & (uint32_t)HAL_I2C_STATE_BUSY_TX) == (uint32_t)HAL_I2C_STATE_BUSY_TX) || ((((uint32_t)CurrentState & (uint32_t)HAL_I2C_STATE_BUSY_RX) == (uint32_t)HAL_I2C_STATE_BUSY_RX) && (CurrentMode == HAL_I2C_MODE_SLAVE)))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable DMA Request */
    CLEAR_BIT(hi2c->Instance->CR2, I2C_CR2_DMAEN);      /* 执行语句 */

    hi2c->XferCount = 0U;                               /* 赋值或初始化 */

    if (CurrentState == HAL_I2C_STATE_BUSY_TX_LISTEN)   /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set state at HAL_I2C_STATE_LISTEN */
      hi2c->PreviousState = I2C_STATE_SLAVE_BUSY_TX;    /* 赋值或初始化 */
      hi2c->State = HAL_I2C_STATE_LISTEN;               /* 赋值或初始化 */

      /* Call the corresponding callback to inform upper layer of End of Transfer */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)               /* 条件编译 */
      hi2c->SlaveTxCpltCallback(hi2c);                  /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_I2C_SlaveTxCpltCallback(hi2c);                /* HAL I2C操作 */
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
    else if (CurrentState == HAL_I2C_STATE_BUSY_RX_LISTEN)  /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* Set state at HAL_I2C_STATE_LISTEN */
      hi2c->PreviousState = I2C_STATE_SLAVE_BUSY_RX;    /* 赋值或初始化 */
      hi2c->State = HAL_I2C_STATE_LISTEN;               /* 赋值或初始化 */

      /* Call the corresponding callback to inform upper layer of End of Transfer */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)               /* 条件编译 */
      hi2c->SlaveRxCpltCallback(hi2c);                  /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_I2C_SlaveRxCpltCallback(hi2c);                /* HAL I2C操作 */
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Do nothing */
    }                                                   /* 代码块结束 */

    /* Enable EVT and ERR interrupt to treat end of transfer in IRQ handler */
    __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_ERR);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  /* Check current Mode, in case of treatment DMA handler have been preempted by a prior interrupt */
  else if (hi2c->Mode != HAL_I2C_MODE_NONE)             /* 否则如果 */
  {                                                     /* 代码块开始 */
    if (hi2c->XferCount == (uint16_t)1)                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable Acknowledge */
      CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);      /* 执行语句 */
    }                                                   /* 代码块结束 */

    /* Disable EVT and ERR interrupt */
    __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_ERR);  /* HAL辅助宏/函数 */

    /* Prepare next transfer or stop current transfer */
    if ((CurrentXferOptions == I2C_NO_OPTION_FRAME) || (CurrentXferOptions == I2C_FIRST_AND_LAST_FRAME) || (CurrentXferOptions == I2C_OTHER_AND_LAST_FRAME) || (CurrentXferOptions == I2C_LAST_FRAME))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Generate Stop */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);       /* 执行语句 */
    }                                                   /* 代码块结束 */

    /* Disable Last DMA */
    CLEAR_BIT(hi2c->Instance->CR2, I2C_CR2_LAST);       /* 执行语句 */

    /* Disable DMA Request */
    CLEAR_BIT(hi2c->Instance->CR2, I2C_CR2_DMAEN);      /* 执行语句 */

    hi2c->XferCount = 0U;                               /* 赋值或初始化 */

    /* Check if Errors has been detected during transfer */
    if (hi2c->ErrorCode != HAL_I2C_ERROR_NONE)          /* 条件判断 */
    {                                                   /* 代码块开始 */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)               /* 条件编译 */
      hi2c->ErrorCallback(hi2c);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_I2C_ErrorCallback(hi2c);                      /* HAL I2C操作 */
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hi2c->State = HAL_I2C_STATE_READY;                /* 赋值或初始化 */

      if (hi2c->Mode == HAL_I2C_MODE_MEM)               /* 条件判断 */
      {                                                 /* 代码块开始 */
        hi2c->Mode = HAL_I2C_MODE_NONE;                 /* 赋值或初始化 */
        hi2c->PreviousState = I2C_STATE_NONE;           /* 赋值或初始化 */

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)               /* 条件编译 */
        hi2c->MemRxCpltCallback(hi2c);                  /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_I2C_MemRxCpltCallback(hi2c);                /* HAL I2C操作 */
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        hi2c->Mode = HAL_I2C_MODE_NONE;                 /* 赋值或初始化 */
        hi2c->PreviousState = I2C_STATE_MASTER_BUSY_RX;  /* 赋值或初始化 */

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)               /* 条件编译 */
        hi2c->MasterRxCpltCallback(hi2c);               /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_I2C_MasterRxCpltCallback(hi2c);             /* HAL I2C操作 */
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Do nothing */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA I2C communication error callback.
  * @param  hdma DMA handle
  * @retval None
  */
static void I2C_DMAError(DMA_HandleTypeDef *hdma)       /* 静态函数 */
{                                                       /* 代码块开始 */
  I2C_HandleTypeDef *hi2c = (I2C_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent; /* Derogation MISRAC2012-Rule-11.5 */

  /* Clear Complete callback */
  if (hi2c->hdmatx != NULL)                             /* 条件判断 */
  {                                                     /* 代码块开始 */
    hi2c->hdmatx->XferCpltCallback = NULL;              /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  if (hi2c->hdmarx != NULL)                             /* 条件判断 */
  {                                                     /* 代码块开始 */
    hi2c->hdmarx->XferCpltCallback = NULL;              /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Ignore DMA FIFO error */
  if (HAL_DMA_GetError(hdma) != HAL_DMA_ERROR_FE)       /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable Acknowledge */
    hi2c->Instance->CR1 &= ~I2C_CR1_ACK;                /* 赋值或初始化 */

    hi2c->XferCount = 0U;                               /* 赋值或初始化 */

    hi2c->State = HAL_I2C_STATE_READY;                  /* 赋值或初始化 */
    hi2c->Mode = HAL_I2C_MODE_NONE;                     /* 赋值或初始化 */

    hi2c->ErrorCode |= HAL_I2C_ERROR_DMA;               /* 赋值或初始化 */

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)               /* 条件编译 */
    hi2c->ErrorCallback(hi2c);                          /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_I2C_ErrorCallback(hi2c);                        /* HAL I2C操作 */
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief DMA I2C communication abort callback
  *        (To be called at end of DMA Abort procedure).
  * @param hdma DMA handle.
  * @retval None
  */
static void I2C_DMAAbort(DMA_HandleTypeDef *hdma)       /* 静态函数 */
{                                                       /* 代码块开始 */
  __IO uint32_t count = 0U;                             /* 赋值或初始化 */
  I2C_HandleTypeDef *hi2c = (I2C_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent; /* Derogation MISRAC2012-Rule-11.5 */

  /* Declaration of temporary variable to prevent undefined behavior of volatile usage */
  HAL_I2C_StateTypeDef CurrentState = hi2c->State;      /* HAL I2C操作 */

  /* During abort treatment, check that there is no pending STOP request */
  /* Wait until STOP flag is reset */
  count = I2C_TIMEOUT_FLAG * (SystemCoreClock / 25U / 1000U);  /* 赋值或初始化 */
  do                                                    /* do-while循环 */
  {                                                     /* 代码块开始 */
    if (count == 0U)                                    /* 条件判断 */
    {                                                   /* 代码块开始 */
      hi2c->ErrorCode |= HAL_I2C_ERROR_TIMEOUT;         /* 赋值或初始化 */
      break;                                            /* 跳出 */
    }                                                   /* 代码块结束 */
    count--;                                            /* 执行语句 */
  }                                                     /* 代码块结束 */
  while (READ_BIT(hi2c->Instance->CR1, I2C_CR1_STOP) == I2C_CR1_STOP);  /* while循环 */

  /* Clear Complete callback */
  if (hi2c->hdmatx != NULL)                             /* 条件判断 */
  {                                                     /* 代码块开始 */
    hi2c->hdmatx->XferCpltCallback = NULL;              /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  if (hi2c->hdmarx != NULL)                             /* 条件判断 */
  {                                                     /* 代码块开始 */
    hi2c->hdmarx->XferCpltCallback = NULL;              /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Disable Acknowledge */
  CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);          /* 执行语句 */

  hi2c->XferCount = 0U;                                 /* 赋值或初始化 */

  /* Reset XferAbortCallback */
  if (hi2c->hdmatx != NULL)                             /* 条件判断 */
  {                                                     /* 代码块开始 */
    hi2c->hdmatx->XferAbortCallback = NULL;             /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  if (hi2c->hdmarx != NULL)                             /* 条件判断 */
  {                                                     /* 代码块开始 */
    hi2c->hdmarx->XferAbortCallback = NULL;             /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Disable I2C peripheral to prevent dummy data in buffer */
  __HAL_I2C_DISABLE(hi2c);                              /* HAL辅助宏/函数 */

  /* Check if come from abort from user */
  if (hi2c->State == HAL_I2C_STATE_ABORT)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    hi2c->State         = HAL_I2C_STATE_READY;          /* 赋值或初始化 */
    hi2c->Mode          = HAL_I2C_MODE_NONE;            /* 赋值或初始化 */
    hi2c->ErrorCode     = HAL_I2C_ERROR_NONE;           /* 赋值或初始化 */

    /* Call the corresponding callback to inform upper layer of End of Transfer */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)               /* 条件编译 */
    hi2c->AbortCpltCallback(hi2c);                      /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_I2C_AbortCpltCallback(hi2c);                    /* HAL I2C操作 */
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    if (((uint32_t)CurrentState & (uint32_t)HAL_I2C_STATE_LISTEN) == (uint32_t)HAL_I2C_STATE_LISTEN)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Renable I2C peripheral */
      __HAL_I2C_ENABLE(hi2c);                           /* HAL辅助宏/函数 */

      /* Enable Acknowledge */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);        /* 执行语句 */

      /* keep HAL_I2C_STATE_LISTEN */
      hi2c->PreviousState = I2C_STATE_NONE;             /* 赋值或初始化 */
      hi2c->State = HAL_I2C_STATE_LISTEN;               /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hi2c->State = HAL_I2C_STATE_READY;                /* 赋值或初始化 */
      hi2c->Mode = HAL_I2C_MODE_NONE;                   /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Call the corresponding callback to inform upper layer of End of Transfer */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)               /* 条件编译 */
    hi2c->ErrorCallback(hi2c);                          /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_I2C_ErrorCallback(hi2c);                        /* HAL I2C操作 */
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  This function handles I2C Communication Timeout.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for I2C module
  * @param  Flag specifies the I2C flag to check.
  * @param  Status The new Flag status (SET or RESET).
  * @param  Timeout Timeout duration
  * @param  Tickstart Tick start value
  * @retval HAL status
  */
static HAL_StatusTypeDef I2C_WaitOnFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Flag, FlagStatus Status, uint32_t Timeout, uint32_t Tickstart)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  /* Wait until flag is set */
  while (__HAL_I2C_GET_FLAG(hi2c, Flag) == Status)      /* while循环 */
  {                                                     /* 代码块开始 */
    /* Check for the Timeout */
    if (Timeout != HAL_MAX_DELAY)                       /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (((HAL_GetTick() - Tickstart) > Timeout) || (Timeout == 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        hi2c->PreviousState     = I2C_STATE_NONE;       /* 赋值或初始化 */
        hi2c->State             = HAL_I2C_STATE_READY;  /* 赋值或初始化 */
        hi2c->Mode              = HAL_I2C_MODE_NONE;    /* 赋值或初始化 */
        hi2c->ErrorCode         |= HAL_I2C_ERROR_TIMEOUT;  /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);                             /* HAL辅助宏/函数 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  This function handles I2C Communication Timeout for Master addressing phase.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for I2C module
  * @param  Flag specifies the I2C flag to check.
  * @param  Timeout Timeout duration
  * @param  Tickstart Tick start value
  * @retval HAL status
  */
static HAL_StatusTypeDef I2C_WaitOnMasterAddressFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Flag, uint32_t Timeout, uint32_t Tickstart)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  while (__HAL_I2C_GET_FLAG(hi2c, Flag) == RESET)       /* while循环 */
  {                                                     /* 代码块开始 */
    if (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_AF) == SET)   /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Generate Stop */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);       /* 执行语句 */

      /* Clear AF Flag */
      __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_AF);          /* HAL辅助宏/函数 */

      hi2c->PreviousState       = I2C_STATE_NONE;       /* 赋值或初始化 */
      hi2c->State               = HAL_I2C_STATE_READY;  /* 赋值或初始化 */
      hi2c->Mode                = HAL_I2C_MODE_NONE;    /* 赋值或初始化 */
      hi2c->ErrorCode           |= HAL_I2C_ERROR_AF;    /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hi2c);                               /* HAL辅助宏/函数 */

      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Check for the Timeout */
    if (Timeout != HAL_MAX_DELAY)                       /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (((HAL_GetTick() - Tickstart) > Timeout) || (Timeout == 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        hi2c->PreviousState       = I2C_STATE_NONE;     /* 赋值或初始化 */
        hi2c->State               = HAL_I2C_STATE_READY;  /* 赋值或初始化 */
        hi2c->Mode                = HAL_I2C_MODE_NONE;  /* 赋值或初始化 */
        hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;  /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);                             /* HAL辅助宏/函数 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  This function handles I2C Communication Timeout for specific usage of TXE flag.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  Timeout Timeout duration
  * @param  Tickstart Tick start value
  * @retval HAL status
  */
static HAL_StatusTypeDef I2C_WaitOnTXEFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Timeout, uint32_t Tickstart)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  while (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_TXE) == RESET)  /* while循环 */
  {                                                     /* 代码块开始 */
    /* Check if a NACK is detected */
    if (I2C_IsAcknowledgeFailed(hi2c) != HAL_OK)        /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Check for the Timeout */
    if (Timeout != HAL_MAX_DELAY)                       /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (((HAL_GetTick() - Tickstart) > Timeout) || (Timeout == 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        hi2c->PreviousState       = I2C_STATE_NONE;     /* 赋值或初始化 */
        hi2c->State               = HAL_I2C_STATE_READY;  /* 赋值或初始化 */
        hi2c->Mode                = HAL_I2C_MODE_NONE;  /* 赋值或初始化 */
        hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;  /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);                             /* HAL辅助宏/函数 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  This function handles I2C Communication Timeout for specific usage of BTF flag.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  Timeout Timeout duration
  * @param  Tickstart Tick start value
  * @retval HAL status
  */
static HAL_StatusTypeDef I2C_WaitOnBTFFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Timeout, uint32_t Tickstart)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  while (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BTF) == RESET)  /* while循环 */
  {                                                     /* 代码块开始 */
    /* Check if a NACK is detected */
    if (I2C_IsAcknowledgeFailed(hi2c) != HAL_OK)        /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Check for the Timeout */
    if (Timeout != HAL_MAX_DELAY)                       /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (((HAL_GetTick() - Tickstart) > Timeout) || (Timeout == 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        hi2c->PreviousState       = I2C_STATE_NONE;     /* 赋值或初始化 */
        hi2c->State               = HAL_I2C_STATE_READY;  /* 赋值或初始化 */
        hi2c->Mode                = HAL_I2C_MODE_NONE;  /* 赋值或初始化 */
        hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;  /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);                             /* HAL辅助宏/函数 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  This function handles I2C Communication Timeout for specific usage of STOP flag.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  Timeout Timeout duration
  * @param  Tickstart Tick start value
  * @retval HAL status
  */
static HAL_StatusTypeDef I2C_WaitOnSTOPFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Timeout, uint32_t Tickstart)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  while (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_STOPF) == RESET)  /* while循环 */
  {                                                     /* 代码块开始 */
    /* Check if a NACK is detected */
    if (I2C_IsAcknowledgeFailed(hi2c) != HAL_OK)        /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Check for the Timeout */
    if (((HAL_GetTick() - Tickstart) > Timeout) || (Timeout == 0U))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hi2c->PreviousState       = I2C_STATE_NONE;       /* 赋值或初始化 */
      hi2c->State               = HAL_I2C_STATE_READY;  /* 赋值或初始化 */
      hi2c->Mode                = HAL_I2C_MODE_NONE;    /* 赋值或初始化 */
      hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;  /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hi2c);                               /* HAL辅助宏/函数 */

      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  This function handles I2C Communication Timeout for specific usage of STOP request through Interrupt.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval HAL status
  */
static HAL_StatusTypeDef I2C_WaitOnSTOPRequestThroughIT(I2C_HandleTypeDef *hi2c)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  __IO uint32_t count = 0U;                             /* 赋值或初始化 */

  /* Wait until STOP flag is reset */
  count = I2C_TIMEOUT_STOP_FLAG * (SystemCoreClock / 25U / 1000U);  /* 赋值或初始化 */
  do                                                    /* do-while循环 */
  {                                                     /* 代码块开始 */
    count--;                                            /* 执行语句 */
    if (count == 0U)                                    /* 条件判断 */
    {                                                   /* 代码块开始 */
      hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;  /* 赋值或初始化 */

      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  while (READ_BIT(hi2c->Instance->CR1, I2C_CR1_STOP) == I2C_CR1_STOP);  /* while循环 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  This function handles I2C Communication Timeout for specific usage of RXNE flag.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  Timeout Timeout duration
  * @param  Tickstart Tick start value
  * @retval HAL status
  */
static HAL_StatusTypeDef I2C_WaitOnRXNEFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Timeout, uint32_t Tickstart)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */

  while (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_RXNE) == RESET)  /* while循环 */
  {                                                     /* 代码块开始 */
    /* Check if a STOPF is detected */
    if (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_STOPF) == SET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear STOP Flag */
      __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_STOPF);       /* HAL辅助宏/函数 */

      hi2c->PreviousState       = I2C_STATE_NONE;       /* 赋值或初始化 */
      hi2c->State               = HAL_I2C_STATE_READY;  /* 赋值或初始化 */
      hi2c->Mode                = HAL_I2C_MODE_NONE;    /* 赋值或初始化 */
      hi2c->ErrorCode           |= HAL_I2C_ERROR_NONE;  /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hi2c);                               /* HAL辅助宏/函数 */

      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Check for the Timeout */
    if (((HAL_GetTick() - Tickstart) > Timeout) || (Timeout == 0U))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hi2c->PreviousState       = I2C_STATE_NONE;       /* 赋值或初始化 */
      hi2c->State               = HAL_I2C_STATE_READY;  /* 赋值或初始化 */
      hi2c->Mode                = HAL_I2C_MODE_NONE;    /* 赋值或初始化 */
      hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;  /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hi2c);                               /* HAL辅助宏/函数 */

      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  This function handles Acknowledge failed detection during an I2C Communication.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval HAL status
  */
static HAL_StatusTypeDef I2C_IsAcknowledgeFailed(I2C_HandleTypeDef *hi2c)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  if (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_AF) == SET)     /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Clear NACKF Flag */
    __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_AF);            /* HAL辅助宏/函数 */

    hi2c->PreviousState       = I2C_STATE_NONE;         /* 赋值或初始化 */
    hi2c->State               = HAL_I2C_STATE_READY;    /* 赋值或初始化 */
    hi2c->Mode                = HAL_I2C_MODE_NONE;      /* 赋值或初始化 */
    hi2c->ErrorCode           |= HAL_I2C_ERROR_AF;      /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hi2c);                                 /* HAL辅助宏/函数 */

    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Convert I2Cx OTHER_xxx XferOptions to functional XferOptions.
  * @param  hi2c I2C handle.
  * @retval None
  */
static void I2C_ConvertOtherXferOptions(I2C_HandleTypeDef *hi2c)  /* 静态函数 */
{                                                       /* 代码块开始 */
  /* if user set XferOptions to I2C_OTHER_FRAME            */
  /* it request implicitly to generate a restart condition */
  /* set XferOptions to I2C_FIRST_FRAME                    */
  if (hi2c->XferOptions == I2C_OTHER_FRAME)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    hi2c->XferOptions = I2C_FIRST_FRAME;                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  /* else if user set XferOptions to I2C_OTHER_AND_LAST_FRAME */
  /* it request implicitly to generate a restart condition    */
  /* then generate a stop condition at the end of transfer    */
  /* set XferOptions to I2C_FIRST_AND_LAST_FRAME              */
  else if (hi2c->XferOptions == I2C_OTHER_AND_LAST_FRAME)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    hi2c->XferOptions = I2C_FIRST_AND_LAST_FRAME;       /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Nothing to do */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

#endif /* HAL_I2C_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
