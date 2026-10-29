/* 文件: stm32f4xx_hal_fmpi2c.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_fmpi2c.c
  * @author  MCD Application Team
  * @brief   FMPI2C HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Inter Integrated Circuit (FMPI2C) peripheral:
  *           + Initialization and de-initialization functions
  *           + IO operation functions
  *           + Peripheral State and Errors functions
  *
  @verbatim
  ==============================================================================
                        ##### How to use this driver #####
  ==============================================================================
    [..]
    The FMPI2C HAL driver can be used as follows:

    (#) Declare a FMPI2C_HandleTypeDef handle structure, for example:
        FMPI2C_HandleTypeDef  hfmpi2c;

    (#)Initialize the FMPI2C low level resources by implementing the @ref HAL_FMPI2C_MspInit() API:
        (##) Enable the FMPI2Cx interface clock
        (##) FMPI2C pins configuration
            (+++) Enable the clock for the FMPI2C GPIOs
            (+++) Configure FMPI2C pins as alternate function open-drain
        (##) NVIC configuration if you need to use interrupt process
            (+++) Configure the FMPI2Cx interrupt priority
            (+++) Enable the NVIC FMPI2C IRQ Channel
        (##) DMA Configuration if you need to use DMA process
            (+++) Declare a DMA_HandleTypeDef handle structure for the transmit or receive stream
            (+++) Enable the DMAx interface clock using
            (+++) Configure the DMA handle parameters
            (+++) Configure the DMA Tx or Rx stream
            (+++) Associate the initialized DMA handle to the hfmpi2c DMA Tx or Rx handle
            (+++) Configure the priority and enable the NVIC for the transfer complete interrupt on
                  the DMA Tx or Rx stream

    (#) Configure the Communication Clock Timing, Own Address1, Master Addressing mode, Dual Addressing mode,
        Own Address2, Own Address2 Mask, General call and Nostretch mode in the hfmpi2c Init structure.

    (#) Initialize the FMPI2C registers by calling the @ref HAL_FMPI2C_Init(), configures also the low level Hardware
        (GPIO, CLOCK, NVIC...etc) by calling the customized @ref HAL_FMPI2C_MspInit(&hfmpi2c) API.

    (#) To check if target device is ready for communication, use the function @ref HAL_FMPI2C_IsDeviceReady()

    (#) For FMPI2C IO and IO MEM operations, three operation modes are available within this driver :

    *** Polling mode IO operation ***
    =================================
    [..]
      (+) Transmit in master mode an amount of data in blocking mode using @ref HAL_FMPI2C_Master_Transmit()
      (+) Receive in master mode an amount of data in blocking mode using @ref HAL_FMPI2C_Master_Receive()
      (+) Transmit in slave mode an amount of data in blocking mode using @ref HAL_FMPI2C_Slave_Transmit()
      (+) Receive in slave mode an amount of data in blocking mode using @ref HAL_FMPI2C_Slave_Receive()

    *** Polling mode IO MEM operation ***
    =====================================
    [..]
      (+) Write an amount of data in blocking mode to a specific memory address using @ref HAL_FMPI2C_Mem_Write()
      (+) Read an amount of data in blocking mode from a specific memory address using @ref HAL_FMPI2C_Mem_Read()


    *** Interrupt mode IO operation ***
    ===================================
    [..]
      (+) Transmit in master mode an amount of data in non-blocking mode using @ref HAL_FMPI2C_Master_Transmit_IT()
      (+) At transmission end of transfer, @ref HAL_FMPI2C_MasterTxCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_FMPI2C_MasterTxCpltCallback()
      (+) Receive in master mode an amount of data in non-blocking mode using @ref HAL_FMPI2C_Master_Receive_IT()
      (+) At reception end of transfer, @ref HAL_FMPI2C_MasterRxCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_FMPI2C_MasterRxCpltCallback()
      (+) Transmit in slave mode an amount of data in non-blocking mode using @ref HAL_FMPI2C_Slave_Transmit_IT()
      (+) At transmission end of transfer, @ref HAL_FMPI2C_SlaveTxCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_FMPI2C_SlaveTxCpltCallback()
      (+) Receive in slave mode an amount of data in non-blocking mode using @ref HAL_FMPI2C_Slave_Receive_IT()
      (+) At reception end of transfer, @ref HAL_FMPI2C_SlaveRxCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_FMPI2C_SlaveRxCpltCallback()
      (+) In case of transfer Error, @ref HAL_FMPI2C_ErrorCallback() function is executed and user can
           add his own code by customization of function pointer @ref HAL_FMPI2C_ErrorCallback()
      (+) Abort a master FMPI2C process communication with Interrupt using @ref HAL_FMPI2C_Master_Abort_IT()
      (+) End of abort process, @ref HAL_FMPI2C_AbortCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_FMPI2C_AbortCpltCallback()
      (+) Discard a slave FMPI2C process communication using @ref __HAL_FMPI2C_GENERATE_NACK() macro.
           This action will inform Master to generate a Stop condition to discard the communication.


    *** Interrupt mode or DMA mode IO sequential operation ***
    ==========================================================
    [..]
      (@) These interfaces allow to manage a sequential transfer with a repeated start condition
          when a direction change during transfer
    [..]
      (+) A specific option field manage the different steps of a sequential transfer
      (+) Option field values are defined through @ref FMPI2C_XFEROPTIONS and are listed below:
      (++) FMPI2C_FIRST_AND_LAST_FRAME: No sequential usage, functional is same as associated interfaces in no sequential mode
      (++) FMPI2C_FIRST_FRAME: Sequential usage, this option allow to manage a sequence with start condition, address
                            and data to transfer without a final stop condition
      (++) FMPI2C_FIRST_AND_NEXT_FRAME: Sequential usage (Master only), this option allow to manage a sequence with start condition, address
                            and data to transfer without a final stop condition, an then permit a call the same master sequential interface
                            several times (like @ref HAL_FMPI2C_Master_Seq_Transmit_IT() then @ref HAL_FMPI2C_Master_Seq_Transmit_IT()
                            or @ref HAL_FMPI2C_Master_Seq_Transmit_DMA() then @ref HAL_FMPI2C_Master_Seq_Transmit_DMA())
      (++) FMPI2C_NEXT_FRAME: Sequential usage, this option allow to manage a sequence with a restart condition, address
                            and with new data to transfer if the direction change or manage only the new data to transfer
                            if no direction change and without a final stop condition in both cases
      (++) FMPI2C_LAST_FRAME: Sequential usage, this option allow to manage a sequance with a restart condition, address
                            and with new data to transfer if the direction change or manage only the new data to transfer
                            if no direction change and with a final stop condition in both cases
      (++) FMPI2C_LAST_FRAME_NO_STOP: Sequential usage (Master only), this option allow to manage a restart condition after several call of the same master sequential
                            interface several times (link with option FMPI2C_FIRST_AND_NEXT_FRAME).
                            Usage can, transfer several bytes one by one using HAL_FMPI2C_Master_Seq_Transmit_IT(option FMPI2C_FIRST_AND_NEXT_FRAME then FMPI2C_NEXT_FRAME)
                              or HAL_FMPI2C_Master_Seq_Receive_IT(option FMPI2C_FIRST_AND_NEXT_FRAME then FMPI2C_NEXT_FRAME)
                              or HAL_FMPI2C_Master_Seq_Transmit_DMA(option FMPI2C_FIRST_AND_NEXT_FRAME then FMPI2C_NEXT_FRAME)
                              or HAL_FMPI2C_Master_Seq_Receive_DMA(option FMPI2C_FIRST_AND_NEXT_FRAME then FMPI2C_NEXT_FRAME).
                            Then usage of this option FMPI2C_LAST_FRAME_NO_STOP at the last Transmit or Receive sequence permit to call the opposite interface Receive or Transmit
                              without stopping the communication and so generate a restart condition.
      (++) FMPI2C_OTHER_FRAME: Sequential usage (Master only), this option allow to manage a restart condition after each call of the same master sequential
                            interface.
                            Usage can, transfer several bytes one by one with a restart with slave address between each bytes using HAL_FMPI2C_Master_Seq_Transmit_IT(option FMPI2C_FIRST_FRAME then FMPI2C_OTHER_FRAME)
                              or HAL_FMPI2C_Master_Seq_Receive_IT(option FMPI2C_FIRST_FRAME then FMPI2C_OTHER_FRAME)
                              or HAL_FMPI2C_Master_Seq_Transmit_DMA(option FMPI2C_FIRST_FRAME then FMPI2C_OTHER_FRAME)
                              or HAL_FMPI2C_Master_Seq_Receive_DMA(option FMPI2C_FIRST_FRAME then FMPI2C_OTHER_FRAME).
                            Then usage of this option FMPI2C_OTHER_AND_LAST_FRAME at the last frame to help automatic generation of STOP condition.

      (+) Different sequential FMPI2C interfaces are listed below:
      (++) Sequential transmit in master FMPI2C mode an amount of data in non-blocking mode using @ref HAL_FMPI2C_Master_Seq_Transmit_IT()
            or using @ref HAL_FMPI2C_Master_Seq_Transmit_DMA()
      (+++) At transmission end of current frame transfer, @ref HAL_FMPI2C_MasterTxCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_FMPI2C_MasterTxCpltCallback()
      (++) Sequential receive in master FMPI2C mode an amount of data in non-blocking mode using @ref HAL_FMPI2C_Master_Seq_Receive_IT()
            or using @ref HAL_FMPI2C_Master_Seq_Receive_DMA()
      (+++) At reception end of current frame transfer, @ref HAL_FMPI2C_MasterRxCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_FMPI2C_MasterRxCpltCallback()
      (++) Abort a master IT or DMA FMPI2C process communication with Interrupt using @ref HAL_FMPI2C_Master_Abort_IT()
      (+++) End of abort process, @ref HAL_FMPI2C_AbortCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_FMPI2C_AbortCpltCallback()
      (++) Enable/disable the Address listen mode in slave FMPI2C mode using @ref HAL_FMPI2C_EnableListen_IT() @ref HAL_FMPI2C_DisableListen_IT()
      (+++) When address slave FMPI2C match, @ref HAL_FMPI2C_AddrCallback() is executed and user can
           add his own code to check the Address Match Code and the transmission direction request by master (Write/Read).
      (+++) At Listen mode end @ref HAL_FMPI2C_ListenCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_FMPI2C_ListenCpltCallback()
      (++) Sequential transmit in slave FMPI2C mode an amount of data in non-blocking mode using @ref HAL_FMPI2C_Slave_Seq_Transmit_IT()
            or using @ref HAL_FMPI2C_Slave_Seq_Transmit_DMA()
      (+++) At transmission end of current frame transfer, @ref HAL_FMPI2C_SlaveTxCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_FMPI2C_SlaveTxCpltCallback()
      (++) Sequential receive in slave FMPI2C mode an amount of data in non-blocking mode using @ref HAL_FMPI2C_Slave_Seq_Receive_IT()
            or using @ref HAL_FMPI2C_Slave_Seq_Receive_DMA()
      (+++) At reception end of current frame transfer, @ref HAL_FMPI2C_SlaveRxCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_FMPI2C_SlaveRxCpltCallback()
      (++) In case of transfer Error, @ref HAL_FMPI2C_ErrorCallback() function is executed and user can
           add his own code by customization of function pointer @ref HAL_FMPI2C_ErrorCallback()
      (++) Discard a slave FMPI2C process communication using @ref __HAL_FMPI2C_GENERATE_NACK() macro.
           This action will inform Master to generate a Stop condition to discard the communication.

    *** Interrupt mode IO MEM operation ***
    =======================================
    [..]
      (+) Write an amount of data in non-blocking mode with Interrupt to a specific memory address using
          @ref HAL_FMPI2C_Mem_Write_IT()
      (+) At Memory end of write transfer, @ref HAL_FMPI2C_MemTxCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_FMPI2C_MemTxCpltCallback()
      (+) Read an amount of data in non-blocking mode with Interrupt from a specific memory address using
          @ref HAL_FMPI2C_Mem_Read_IT()
      (+) At Memory end of read transfer, @ref HAL_FMPI2C_MemRxCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_FMPI2C_MemRxCpltCallback()
      (+) In case of transfer Error, @ref HAL_FMPI2C_ErrorCallback() function is executed and user can
           add his own code by customization of function pointer @ref HAL_FMPI2C_ErrorCallback()

    *** DMA mode IO operation ***
    ==============================
    [..]
      (+) Transmit in master mode an amount of data in non-blocking mode (DMA) using
          @ref HAL_FMPI2C_Master_Transmit_DMA()
      (+) At transmission end of transfer, @ref HAL_FMPI2C_MasterTxCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_FMPI2C_MasterTxCpltCallback()
      (+) Receive in master mode an amount of data in non-blocking mode (DMA) using
          @ref HAL_FMPI2C_Master_Receive_DMA()
      (+) At reception end of transfer, @ref HAL_FMPI2C_MasterRxCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_FMPI2C_MasterRxCpltCallback()
      (+) Transmit in slave mode an amount of data in non-blocking mode (DMA) using
          @ref HAL_FMPI2C_Slave_Transmit_DMA()
      (+) At transmission end of transfer, @ref HAL_FMPI2C_SlaveTxCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_FMPI2C_SlaveTxCpltCallback()
      (+) Receive in slave mode an amount of data in non-blocking mode (DMA) using
          @ref HAL_FMPI2C_Slave_Receive_DMA()
      (+) At reception end of transfer, @ref HAL_FMPI2C_SlaveRxCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_FMPI2C_SlaveRxCpltCallback()
      (+) In case of transfer Error, @ref HAL_FMPI2C_ErrorCallback() function is executed and user can
           add his own code by customization of function pointer @ref HAL_FMPI2C_ErrorCallback()
      (+) Abort a master FMPI2C process communication with Interrupt using @ref HAL_FMPI2C_Master_Abort_IT()
      (+) End of abort process, @ref HAL_FMPI2C_AbortCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_FMPI2C_AbortCpltCallback()
      (+) Discard a slave FMPI2C process communication using @ref __HAL_FMPI2C_GENERATE_NACK() macro.
           This action will inform Master to generate a Stop condition to discard the communication.

    *** DMA mode IO MEM operation ***
    =================================
    [..]
      (+) Write an amount of data in non-blocking mode with DMA to a specific memory address using
          @ref HAL_FMPI2C_Mem_Write_DMA()
      (+) At Memory end of write transfer, @ref HAL_FMPI2C_MemTxCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_FMPI2C_MemTxCpltCallback()
      (+) Read an amount of data in non-blocking mode with DMA from a specific memory address using
          @ref HAL_FMPI2C_Mem_Read_DMA()
      (+) At Memory end of read transfer, @ref HAL_FMPI2C_MemRxCpltCallback() is executed and user can
           add his own code by customization of function pointer @ref HAL_FMPI2C_MemRxCpltCallback()
      (+) In case of transfer Error, @ref HAL_FMPI2C_ErrorCallback() function is executed and user can
           add his own code by customization of function pointer @ref HAL_FMPI2C_ErrorCallback()


     *** FMPI2C HAL driver macros list ***
     ==================================
     [..]
       Below the list of most used macros in FMPI2C HAL driver.

      (+) @ref __HAL_FMPI2C_ENABLE: Enable the FMPI2C peripheral
      (+) @ref __HAL_FMPI2C_DISABLE: Disable the FMPI2C peripheral
      (+) @ref __HAL_FMPI2C_GENERATE_NACK: Generate a Non-Acknowledge FMPI2C peripheral in Slave mode
      (+) @ref __HAL_FMPI2C_GET_FLAG: Check whether the specified FMPI2C flag is set or not
      (+) @ref __HAL_FMPI2C_CLEAR_FLAG: Clear the specified FMPI2C pending flag
      (+) @ref __HAL_FMPI2C_ENABLE_IT: Enable the specified FMPI2C interrupt
      (+) @ref __HAL_FMPI2C_DISABLE_IT: Disable the specified FMPI2C interrupt

     *** Callback registration ***
     =============================================
    [..]
     The compilation flag USE_HAL_FMPI2C_REGISTER_CALLBACKS when set to 1
     allows the user to configure dynamically the driver callbacks.
     Use Functions @ref HAL_FMPI2C_RegisterCallback() or @ref HAL_FMPI2C_RegisterAddrCallback()
     to register an interrupt callback.
    [..]
     Function @ref HAL_FMPI2C_RegisterCallback() allows to register following callbacks:
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
     For specific callback AddrCallback use dedicated register callbacks : @ref HAL_FMPI2C_RegisterAddrCallback().
    [..]
     Use function @ref HAL_FMPI2C_UnRegisterCallback to reset a callback to the default
     weak function.
     @ref HAL_FMPI2C_UnRegisterCallback takes as parameters the HAL peripheral handle,
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
     For callback AddrCallback use dedicated register callbacks : @ref HAL_FMPI2C_UnRegisterAddrCallback().
    [..]
     By default, after the @ref HAL_FMPI2C_Init() and when the state is @ref HAL_FMPI2C_STATE_RESET
     all callbacks are set to the corresponding weak functions:
     examples @ref HAL_FMPI2C_MasterTxCpltCallback(), @ref HAL_FMPI2C_MasterRxCpltCallback().
     Exception done for MspInit and MspDeInit functions that are
     reset to the legacy weak functions in the @ref HAL_FMPI2C_Init()/ @ref HAL_FMPI2C_DeInit() only when
     these callbacks are null (not registered beforehand).
     If MspInit or MspDeInit are not null, the @ref HAL_FMPI2C_Init()/ @ref HAL_FMPI2C_DeInit()
     keep and use the user MspInit/MspDeInit callbacks (registered beforehand) whatever the state.
    [..]
     Callbacks can be registered/unregistered in @ref HAL_FMPI2C_STATE_READY state only.
     Exception done MspInit/MspDeInit functions that can be registered/unregistered
     in @ref HAL_FMPI2C_STATE_READY or @ref HAL_FMPI2C_STATE_RESET state,
     thus registered (user) MspInit/DeInit callbacks can be used during the Init/DeInit.
     Then, the user first registers the MspInit/MspDeInit user callbacks
     using @ref HAL_FMPI2C_RegisterCallback() before calling @ref HAL_FMPI2C_DeInit()
     or @ref HAL_FMPI2C_Init() function.
    [..]
     When the compilation flag USE_HAL_FMPI2C_REGISTER_CALLBACKS is set to 0 or
     not defined, the callback registration feature is not available and all callbacks
     are set to the corresponding weak functions.

     [..]
       (@) You can refer to the FMPI2C HAL driver header file for more useful macros

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

/** @defgroup FMPI2C FMPI2C
  * @brief FMPI2C HAL module driver
  * @{
  */

#ifdef HAL_FMPI2C_MODULE_ENABLED                        /* 条件编译 */
#if defined(FMPI2C_CR1_PE)                              /* 条件编译 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/** @defgroup FMPI2C_Private_Define FMPI2C Private Define
  * @{
  */
#define TIMING_CLEAR_MASK   (0xF0FFFFFFU)  /*!< FMPI2C TIMING clear register Mask */
#define FMPI2C_TIMEOUT_ADDR    (10000U)       /*!< 10 s  */
#define FMPI2C_TIMEOUT_BUSY    (25U)          /*!< 25 ms */
#define FMPI2C_TIMEOUT_DIR     (25U)          /*!< 25 ms */
#define FMPI2C_TIMEOUT_RXNE    (25U)          /*!< 25 ms */
#define FMPI2C_TIMEOUT_STOPF   (25U)          /*!< 25 ms */
#define FMPI2C_TIMEOUT_TC      (25U)          /*!< 25 ms */
#define FMPI2C_TIMEOUT_TCR     (25U)          /*!< 25 ms */
#define FMPI2C_TIMEOUT_TXIS    (25U)          /*!< 25 ms */
#define FMPI2C_TIMEOUT_FLAG    (25U)          /*!< 25 ms */

#define MAX_NBYTE_SIZE      255U                        /* 定义宏 */
#define SlaveAddr_SHIFT     7U                          /* 定义宏 */
#define SlaveAddr_MSK       0x06U                       /* 定义宏 */

/* Private define for @ref PreviousState usage */
#define FMPI2C_STATE_MSK             ((uint32_t)((uint32_t)((uint32_t)HAL_FMPI2C_STATE_BUSY_TX | (uint32_t)HAL_FMPI2C_STATE_BUSY_RX) & (uint32_t)(~((uint32_t)HAL_FMPI2C_STATE_READY)))) /*!< Mask State define, keep only RX and TX bits            */
#define FMPI2C_STATE_NONE            ((uint32_t)(HAL_FMPI2C_MODE_NONE))                                                        /*!< Default Value                                          */
#define FMPI2C_STATE_MASTER_BUSY_TX  ((uint32_t)(((uint32_t)HAL_FMPI2C_STATE_BUSY_TX & FMPI2C_STATE_MSK) | (uint32_t)HAL_FMPI2C_MODE_MASTER))            /*!< Master Busy TX, combinaison of State LSB and Mode enum */
#define FMPI2C_STATE_MASTER_BUSY_RX  ((uint32_t)(((uint32_t)HAL_FMPI2C_STATE_BUSY_RX & FMPI2C_STATE_MSK) | (uint32_t)HAL_FMPI2C_MODE_MASTER))            /*!< Master Busy RX, combinaison of State LSB and Mode enum */
#define FMPI2C_STATE_SLAVE_BUSY_TX   ((uint32_t)(((uint32_t)HAL_FMPI2C_STATE_BUSY_TX & FMPI2C_STATE_MSK) | (uint32_t)HAL_FMPI2C_MODE_SLAVE))             /*!< Slave Busy TX, combinaison of State LSB and Mode enum  */
#define FMPI2C_STATE_SLAVE_BUSY_RX   ((uint32_t)(((uint32_t)HAL_FMPI2C_STATE_BUSY_RX & FMPI2C_STATE_MSK) | (uint32_t)HAL_FMPI2C_MODE_SLAVE))             /*!< Slave Busy RX, combinaison of State LSB and Mode enum  */
#define FMPI2C_STATE_MEM_BUSY_TX     ((uint32_t)(((uint32_t)HAL_FMPI2C_STATE_BUSY_TX & FMPI2C_STATE_MSK) | (uint32_t)HAL_FMPI2C_MODE_MEM))               /*!< Memory Busy TX, combinaison of State LSB and Mode enum */
#define FMPI2C_STATE_MEM_BUSY_RX     ((uint32_t)(((uint32_t)HAL_FMPI2C_STATE_BUSY_RX & FMPI2C_STATE_MSK) | (uint32_t)HAL_FMPI2C_MODE_MEM))               /*!< Memory Busy RX, combinaison of State LSB and Mode enum */


/* Private define to centralize the enable/disable of Interrupts */
#define FMPI2C_XFER_TX_IT          (uint16_t)(0x0001U)   /* Bit field can be combinated with @ref FMPI2C_XFER_LISTEN_IT */
#define FMPI2C_XFER_RX_IT          (uint16_t)(0x0002U)   /* Bit field can be combinated with @ref FMPI2C_XFER_LISTEN_IT */
#define FMPI2C_XFER_LISTEN_IT      (uint16_t)(0x8000U)   /* Bit field can be combinated with @ref FMPI2C_XFER_TX_IT and @ref FMPI2C_XFER_RX_IT */

#define FMPI2C_XFER_ERROR_IT       (uint16_t)(0x0010U)   /* Bit definition to manage addition of global Error and NACK treatment */
#define FMPI2C_XFER_CPLT_IT        (uint16_t)(0x0020U)   /* Bit definition to manage only STOP evenement */
#define FMPI2C_XFER_RELOAD_IT      (uint16_t)(0x0040U)   /* Bit definition to manage only Reload of NBYTE */

/* Private define Sequential Transfer Options default/reset value */
#define FMPI2C_NO_OPTION_FRAME     (0xFFFF0000U)        /* 定义宏 */
/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/** @defgroup FMPI2C_Private_Functions FMPI2C Private Functions
  * @{
  */
/* Private functions to handle DMA transfer */
static void FMPI2C_DMAMasterTransmitCplt(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void FMPI2C_DMAMasterReceiveCplt(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void FMPI2C_DMASlaveTransmitCplt(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void FMPI2C_DMASlaveReceiveCplt(DMA_HandleTypeDef *hdma);  /* 静态函数 */
static void FMPI2C_DMAError(DMA_HandleTypeDef *hdma);   /* 静态函数 */
static void FMPI2C_DMAAbort(DMA_HandleTypeDef *hdma);   /* 静态函数 */

/* Private functions to handle IT transfer */
static void FMPI2C_ITAddrCplt(FMPI2C_HandleTypeDef *hfmpi2c, uint32_t ITFlags);  /* 静态函数 */
static void FMPI2C_ITMasterSeqCplt(FMPI2C_HandleTypeDef *hfmpi2c);  /* 静态函数 */
static void FMPI2C_ITSlaveSeqCplt(FMPI2C_HandleTypeDef *hfmpi2c);  /* 静态函数 */
static void FMPI2C_ITMasterCplt(FMPI2C_HandleTypeDef *hfmpi2c, uint32_t ITFlags);  /* 静态函数 */
static void FMPI2C_ITSlaveCplt(FMPI2C_HandleTypeDef *hfmpi2c, uint32_t ITFlags);  /* 静态函数 */
static void FMPI2C_ITListenCplt(FMPI2C_HandleTypeDef *hfmpi2c, uint32_t ITFlags);  /* 静态函数 */
static void FMPI2C_ITError(FMPI2C_HandleTypeDef *hfmpi2c, uint32_t ErrorCode);  /* 静态函数 */

/* Private functions to handle IT transfer */
static HAL_StatusTypeDef FMPI2C_RequestMemoryWrite(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t DevAddress, uint16_t MemAddress,  /* 静态变量/函数 */
                                                uint16_t MemAddSize, uint32_t Timeout, uint32_t Tickstart);  /* 16位无符号变量/函数 */
static HAL_StatusTypeDef FMPI2C_RequestMemoryRead(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t DevAddress, uint16_t MemAddress,  /* 静态变量/函数 */
                                               uint16_t MemAddSize, uint32_t Timeout, uint32_t Tickstart);  /* 16位无符号变量/函数 */

/* Private functions for FMPI2C transfer IRQ handler */
static HAL_StatusTypeDef FMPI2C_Master_ISR_IT(struct __FMPI2C_HandleTypeDef *hfmpi2c, uint32_t ITFlags, uint32_t ITSources);  /* 静态变量/函数 */
static HAL_StatusTypeDef FMPI2C_Slave_ISR_IT(struct __FMPI2C_HandleTypeDef *hfmpi2c, uint32_t ITFlags, uint32_t ITSources);  /* 静态变量/函数 */
static HAL_StatusTypeDef FMPI2C_Master_ISR_DMA(struct __FMPI2C_HandleTypeDef *hfmpi2c, uint32_t ITFlags, uint32_t ITSources);  /* 静态变量/函数 */
static HAL_StatusTypeDef FMPI2C_Slave_ISR_DMA(struct __FMPI2C_HandleTypeDef *hfmpi2c, uint32_t ITFlags, uint32_t ITSources);  /* 静态变量/函数 */

/* Private functions to handle flags during polling transfer */
static HAL_StatusTypeDef FMPI2C_WaitOnFlagUntilTimeout(FMPI2C_HandleTypeDef *hfmpi2c, uint32_t Flag, FlagStatus Status,  /* 静态变量/函数 */
                                                    uint32_t Timeout, uint32_t Tickstart);  /* 32位无符号变量/函数 */
static HAL_StatusTypeDef FMPI2C_WaitOnTXISFlagUntilTimeout(FMPI2C_HandleTypeDef *hfmpi2c, uint32_t Timeout, uint32_t Tickstart);  /* 静态变量/函数 */
static HAL_StatusTypeDef FMPI2C_WaitOnRXNEFlagUntilTimeout(FMPI2C_HandleTypeDef *hfmpi2c, uint32_t Timeout, uint32_t Tickstart);  /* 静态变量/函数 */
static HAL_StatusTypeDef FMPI2C_WaitOnSTOPFlagUntilTimeout(FMPI2C_HandleTypeDef *hfmpi2c, uint32_t Timeout, uint32_t Tickstart);  /* 静态变量/函数 */
static HAL_StatusTypeDef FMPI2C_IsAcknowledgeFailed(FMPI2C_HandleTypeDef *hfmpi2c, uint32_t Timeout, uint32_t Tickstart);  /* 静态变量/函数 */

/* Private functions to centralize the enable/disable of Interrupts */
static void FMPI2C_Enable_IRQ(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t InterruptRequest);  /* 静态函数 */
static void FMPI2C_Disable_IRQ(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t InterruptRequest);  /* 静态函数 */

/* Private function to treat different error callback */
static void FMPI2C_TreatErrorCallback(FMPI2C_HandleTypeDef *hfmpi2c);  /* 静态函数 */

/* Private function to flush TXDR register */
static void FMPI2C_Flush_TXDR(FMPI2C_HandleTypeDef *hfmpi2c);  /* 静态函数 */

/* Private function to handle  start, restart or stop a transfer */
static void FMPI2C_TransferConfig(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t DevAddress, uint8_t Size, uint32_t Mode,  /* 静态函数 */
                               uint32_t Request);       /* 32位无符号变量/函数 */

/* Private function to Convert Specific options */
static void FMPI2C_ConvertOtherXferOptions(FMPI2C_HandleTypeDef *hfmpi2c);  /* 静态函数 */
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/

/** @defgroup FMPI2C_Exported_Functions FMPI2C Exported Functions
  * @{
  */

/** @defgroup FMPI2C_Exported_Functions_Group1 Initialization and de-initialization functions
  *  @brief    Initialization and Configuration functions
  *
@verbatim
 ===============================================================================
              ##### Initialization and de-initialization functions #####
 ===============================================================================
    [..]  This subsection provides a set of functions allowing to initialize and
          deinitialize the FMPI2Cx peripheral:

      (+) User must Implement HAL_FMPI2C_MspInit() function in which he configures
          all related peripherals resources (CLOCK, GPIO, DMA, IT and NVIC ).

      (+) Call the function HAL_FMPI2C_Init() to configure the selected device with
          the selected configuration:
        (++) Clock Timing
        (++) Own Address 1
        (++) Addressing mode (Master, Slave)
        (++) Dual Addressing mode
        (++) Own Address 2
        (++) Own Address 2 Mask
        (++) General call mode
        (++) Nostretch mode

      (+) Call the function HAL_FMPI2C_DeInit() to restore the default configuration
          of the selected FMPI2Cx peripheral.

@endverbatim
  * @{
  */

/**
  * @brief  Initializes the FMPI2C according to the specified parameters
  *         in the FMPI2C_InitTypeDef and initialize the associated handle.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Init(FMPI2C_HandleTypeDef *hfmpi2c)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the FMPI2C handle allocation */
  if (hfmpi2c == NULL)                                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the parameters */
  assert_param(IS_FMPI2C_ALL_INSTANCE(hfmpi2c->Instance));  /* 执行语句 */
  assert_param(IS_FMPI2C_OWN_ADDRESS1(hfmpi2c->Init.OwnAddress1));  /* 执行语句 */
  assert_param(IS_FMPI2C_ADDRESSING_MODE(hfmpi2c->Init.AddressingMode));  /* 执行语句 */
  assert_param(IS_FMPI2C_DUAL_ADDRESS(hfmpi2c->Init.DualAddressMode));  /* 执行语句 */
  assert_param(IS_FMPI2C_OWN_ADDRESS2(hfmpi2c->Init.OwnAddress2));  /* 执行语句 */
  assert_param(IS_FMPI2C_OWN_ADDRESS2_MASK(hfmpi2c->Init.OwnAddress2Masks));  /* 执行语句 */
  assert_param(IS_FMPI2C_GENERAL_CALL(hfmpi2c->Init.GeneralCallMode));  /* 执行语句 */
  assert_param(IS_FMPI2C_NO_STRETCH(hfmpi2c->Init.NoStretchMode));  /* 执行语句 */

  if (hfmpi2c->State == HAL_FMPI2C_STATE_RESET)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Allocate lock resource and initialize it */
    hfmpi2c->Lock = HAL_UNLOCKED;                       /* 赋值或初始化 */

#if (USE_HAL_FMPI2C_REGISTER_CALLBACKS == 1)            /* 条件编译 */
    /* Init the FMPI2C Callback settings */
    hfmpi2c->MasterTxCpltCallback = HAL_FMPI2C_MasterTxCpltCallback; /* Legacy weak MasterTxCpltCallback */
    hfmpi2c->MasterRxCpltCallback = HAL_FMPI2C_MasterRxCpltCallback; /* Legacy weak MasterRxCpltCallback */
    hfmpi2c->SlaveTxCpltCallback  = HAL_FMPI2C_SlaveTxCpltCallback;  /* Legacy weak SlaveTxCpltCallback  */
    hfmpi2c->SlaveRxCpltCallback  = HAL_FMPI2C_SlaveRxCpltCallback;  /* Legacy weak SlaveRxCpltCallback  */
    hfmpi2c->ListenCpltCallback   = HAL_FMPI2C_ListenCpltCallback;   /* Legacy weak ListenCpltCallback   */
    hfmpi2c->MemTxCpltCallback    = HAL_FMPI2C_MemTxCpltCallback;    /* Legacy weak MemTxCpltCallback    */
    hfmpi2c->MemRxCpltCallback    = HAL_FMPI2C_MemRxCpltCallback;    /* Legacy weak MemRxCpltCallback    */
    hfmpi2c->ErrorCallback        = HAL_FMPI2C_ErrorCallback;        /* Legacy weak ErrorCallback        */
    hfmpi2c->AbortCpltCallback    = HAL_FMPI2C_AbortCpltCallback;    /* Legacy weak AbortCpltCallback    */
    hfmpi2c->AddrCallback         = HAL_FMPI2C_AddrCallback;         /* Legacy weak AddrCallback         */

    if (hfmpi2c->MspInitCallback == NULL)               /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpi2c->MspInitCallback = HAL_FMPI2C_MspInit; /* Legacy weak MspInit  */
    }                                                   /* 代码块结束 */

    /* Init the low level hardware : GPIO, CLOCK, CORTEX...etc */
    hfmpi2c->MspInitCallback(hfmpi2c);                  /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /* Init the low level hardware : GPIO, CLOCK, CORTEX...etc */
    HAL_FMPI2C_MspInit(hfmpi2c);                        /* HAL库函数调用 */
#endif /* USE_HAL_FMPI2C_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */

  hfmpi2c->State = HAL_FMPI2C_STATE_BUSY;               /* 赋值或初始化 */

  /* Disable the selected FMPI2C peripheral */
  __HAL_FMPI2C_DISABLE(hfmpi2c);                        /* HAL辅助宏/函数 */

  /*---------------------------- FMPI2Cx TIMINGR Configuration ------------------*/
  /* Configure FMPI2Cx: Frequency range */
  hfmpi2c->Instance->TIMINGR = hfmpi2c->Init.Timing & TIMING_CLEAR_MASK;  /* 赋值或初始化 */

  /*---------------------------- FMPI2Cx OAR1 Configuration ---------------------*/
  /* Disable Own Address1 before set the Own Address1 configuration */
  hfmpi2c->Instance->OAR1 &= ~FMPI2C_OAR1_OA1EN;        /* 赋值或初始化 */

  /* Configure FMPI2Cx: Own Address1 and ack own address1 mode */
  if (hfmpi2c->Init.AddressingMode == FMPI2C_ADDRESSINGMODE_7BIT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hfmpi2c->Instance->OAR1 = (FMPI2C_OAR1_OA1EN | hfmpi2c->Init.OwnAddress1);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else /* FMPI2C_ADDRESSINGMODE_10BIT */
  {                                                     /* 代码块开始 */
    hfmpi2c->Instance->OAR1 = (FMPI2C_OAR1_OA1EN | FMPI2C_OAR1_OA1MODE | hfmpi2c->Init.OwnAddress1);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /*---------------------------- FMPI2Cx CR2 Configuration ----------------------*/
  /* Configure FMPI2Cx: Addressing Master mode */
  if (hfmpi2c->Init.AddressingMode == FMPI2C_ADDRESSINGMODE_10BIT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hfmpi2c->Instance->CR2 = (FMPI2C_CR2_ADD10);        /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  /* Enable the AUTOEND by default, and enable NACK (should be disable only during Slave process */
  hfmpi2c->Instance->CR2 |= (FMPI2C_CR2_AUTOEND | FMPI2C_CR2_NACK);  /* 赋值或初始化 */

  /*---------------------------- FMPI2Cx OAR2 Configuration ---------------------*/
  /* Disable Own Address2 before set the Own Address2 configuration */
  hfmpi2c->Instance->OAR2 &= ~FMPI2C_DUALADDRESS_ENABLE;  /* 赋值或初始化 */

  /* Configure FMPI2Cx: Dual mode and Own Address2 */
  hfmpi2c->Instance->OAR2 = (hfmpi2c->Init.DualAddressMode | hfmpi2c->Init.OwnAddress2 | (hfmpi2c->Init.OwnAddress2Masks << 8));  /* 赋值或初始化 */

  /*---------------------------- FMPI2Cx CR1 Configuration ----------------------*/
  /* Configure FMPI2Cx: Generalcall and NoStretch mode */
  hfmpi2c->Instance->CR1 = (hfmpi2c->Init.GeneralCallMode | hfmpi2c->Init.NoStretchMode);  /* 赋值或初始化 */

  /* Enable the selected FMPI2C peripheral */
  __HAL_FMPI2C_ENABLE(hfmpi2c);                         /* HAL辅助宏/函数 */

  hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_NONE;           /* 赋值或初始化 */
  hfmpi2c->State = HAL_FMPI2C_STATE_READY;              /* 赋值或初始化 */
  hfmpi2c->PreviousState = FMPI2C_STATE_NONE;           /* 赋值或初始化 */
  hfmpi2c->Mode = HAL_FMPI2C_MODE_NONE;                 /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  DeInitialize the FMPI2C peripheral.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_DeInit(FMPI2C_HandleTypeDef *hfmpi2c)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the FMPI2C handle allocation */
  if (hfmpi2c == NULL)                                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the parameters */
  assert_param(IS_FMPI2C_ALL_INSTANCE(hfmpi2c->Instance));  /* 执行语句 */

  hfmpi2c->State = HAL_FMPI2C_STATE_BUSY;               /* 赋值或初始化 */

  /* Disable the FMPI2C Peripheral Clock */
  __HAL_FMPI2C_DISABLE(hfmpi2c);                        /* HAL辅助宏/函数 */

#if (USE_HAL_FMPI2C_REGISTER_CALLBACKS == 1)            /* 条件编译 */
  if (hfmpi2c->MspDeInitCallback == NULL)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    hfmpi2c->MspDeInitCallback = HAL_FMPI2C_MspDeInit; /* Legacy weak MspDeInit  */
  }                                                     /* 代码块结束 */

  /* DeInit the low level hardware: GPIO, CLOCK, NVIC */
  hfmpi2c->MspDeInitCallback(hfmpi2c);                  /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* DeInit the low level hardware: GPIO, CLOCK, NVIC */
  HAL_FMPI2C_MspDeInit(hfmpi2c);                        /* HAL库函数调用 */
#endif /* USE_HAL_FMPI2C_REGISTER_CALLBACKS */

  hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_NONE;           /* 赋值或初始化 */
  hfmpi2c->State = HAL_FMPI2C_STATE_RESET;              /* 赋值或初始化 */
  hfmpi2c->PreviousState = FMPI2C_STATE_NONE;           /* 赋值或初始化 */
  hfmpi2c->Mode = HAL_FMPI2C_MODE_NONE;                 /* 赋值或初始化 */

  /* Release Lock */
  __HAL_UNLOCK(hfmpi2c);                                /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief Initialize the FMPI2C MSP.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @retval None
  */
__weak void HAL_FMPI2C_MspInit(FMPI2C_HandleTypeDef *hfmpi2c)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hfmpi2c);                                      /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_FMPI2C_MspInit could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief DeInitialize the FMPI2C MSP.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @retval None
  */
__weak void HAL_FMPI2C_MspDeInit(FMPI2C_HandleTypeDef *hfmpi2c)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hfmpi2c);                                      /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_FMPI2C_MspDeInit could be implemented in the user file
   */
}                                                       /* 代码块结束 */

#if (USE_HAL_FMPI2C_REGISTER_CALLBACKS == 1)            /* 条件编译 */
/**
  * @brief  Register a User FMPI2C Callback
  *         To be used instead of the weak predefined callback
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  CallbackID ID of the callback to be registered
  *         This parameter can be one of the following values:
  *          @arg @ref HAL_FMPI2C_MASTER_TX_COMPLETE_CB_ID Master Tx Transfer completed callback ID
  *          @arg @ref HAL_FMPI2C_MASTER_RX_COMPLETE_CB_ID Master Rx Transfer completed callback ID
  *          @arg @ref HAL_FMPI2C_SLAVE_TX_COMPLETE_CB_ID Slave Tx Transfer completed callback ID
  *          @arg @ref HAL_FMPI2C_SLAVE_RX_COMPLETE_CB_ID Slave Rx Transfer completed callback ID
  *          @arg @ref HAL_FMPI2C_LISTEN_COMPLETE_CB_ID Listen Complete callback ID
  *          @arg @ref HAL_FMPI2C_MEM_TX_COMPLETE_CB_ID Memory Tx Transfer callback ID
  *          @arg @ref HAL_FMPI2C_MEM_RX_COMPLETE_CB_ID Memory Rx Transfer completed callback ID
  *          @arg @ref HAL_FMPI2C_ERROR_CB_ID Error callback ID
  *          @arg @ref HAL_FMPI2C_ABORT_CB_ID Abort callback ID
  *          @arg @ref HAL_FMPI2C_MSPINIT_CB_ID MspInit callback ID
  *          @arg @ref HAL_FMPI2C_MSPDEINIT_CB_ID MspDeInit callback ID
  * @param  pCallback pointer to the Callback function
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_RegisterCallback(FMPI2C_HandleTypeDef *hfmpi2c, HAL_FMPI2C_CallbackIDTypeDef CallbackID,  /* HAL状态类型函数 */
                                           pFMPI2C_CallbackTypeDef pCallback)  /* 源代码 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  if (pCallback == NULL)                                /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  /* Process locked */
  __HAL_LOCK(hfmpi2c);                                  /* HAL辅助宏/函数 */

  if (HAL_FMPI2C_STATE_READY == hfmpi2c->State)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_FMPI2C_MASTER_TX_COMPLETE_CB_ID :        /* case标签 */
        hfmpi2c->MasterTxCpltCallback = pCallback;      /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_FMPI2C_MASTER_RX_COMPLETE_CB_ID :        /* case标签 */
        hfmpi2c->MasterRxCpltCallback = pCallback;      /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_FMPI2C_SLAVE_TX_COMPLETE_CB_ID :         /* case标签 */
        hfmpi2c->SlaveTxCpltCallback = pCallback;       /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_FMPI2C_SLAVE_RX_COMPLETE_CB_ID :         /* case标签 */
        hfmpi2c->SlaveRxCpltCallback = pCallback;       /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_FMPI2C_LISTEN_COMPLETE_CB_ID :           /* case标签 */
        hfmpi2c->ListenCpltCallback = pCallback;        /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_FMPI2C_MEM_TX_COMPLETE_CB_ID :           /* case标签 */
        hfmpi2c->MemTxCpltCallback = pCallback;         /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_FMPI2C_MEM_RX_COMPLETE_CB_ID :           /* case标签 */
        hfmpi2c->MemRxCpltCallback = pCallback;         /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_FMPI2C_ERROR_CB_ID :                     /* case标签 */
        hfmpi2c->ErrorCallback = pCallback;             /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_FMPI2C_ABORT_CB_ID :                     /* case标签 */
        hfmpi2c->AbortCpltCallback = pCallback;         /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_FMPI2C_MSPINIT_CB_ID :                   /* case标签 */
        hfmpi2c->MspInitCallback = pCallback;           /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_FMPI2C_MSPDEINIT_CB_ID :                 /* case标签 */
        hfmpi2c->MspDeInitCallback = pCallback;         /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (HAL_FMPI2C_STATE_RESET == hfmpi2c->State)    /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_FMPI2C_MSPINIT_CB_ID :                   /* case标签 */
        hfmpi2c->MspInitCallback = pCallback;           /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_FMPI2C_MSPDEINIT_CB_ID :                 /* case标签 */
        hfmpi2c->MspDeInitCallback = pCallback;         /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hfmpi2c);                                /* HAL辅助宏/函数 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Unregister an FMPI2C Callback
  *         FMPI2C callback is redirected to the weak predefined callback
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  CallbackID ID of the callback to be unregistered
  *         This parameter can be one of the following values:
  *         This parameter can be one of the following values:
  *          @arg @ref HAL_FMPI2C_MASTER_TX_COMPLETE_CB_ID Master Tx Transfer completed callback ID
  *          @arg @ref HAL_FMPI2C_MASTER_RX_COMPLETE_CB_ID Master Rx Transfer completed callback ID
  *          @arg @ref HAL_FMPI2C_SLAVE_TX_COMPLETE_CB_ID Slave Tx Transfer completed callback ID
  *          @arg @ref HAL_FMPI2C_SLAVE_RX_COMPLETE_CB_ID Slave Rx Transfer completed callback ID
  *          @arg @ref HAL_FMPI2C_LISTEN_COMPLETE_CB_ID Listen Complete callback ID
  *          @arg @ref HAL_FMPI2C_MEM_TX_COMPLETE_CB_ID Memory Tx Transfer callback ID
  *          @arg @ref HAL_FMPI2C_MEM_RX_COMPLETE_CB_ID Memory Rx Transfer completed callback ID
  *          @arg @ref HAL_FMPI2C_ERROR_CB_ID Error callback ID
  *          @arg @ref HAL_FMPI2C_ABORT_CB_ID Abort callback ID
  *          @arg @ref HAL_FMPI2C_MSPINIT_CB_ID MspInit callback ID
  *          @arg @ref HAL_FMPI2C_MSPDEINIT_CB_ID MspDeInit callback ID
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_UnRegisterCallback(FMPI2C_HandleTypeDef *hfmpi2c, HAL_FMPI2C_CallbackIDTypeDef CallbackID)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Process locked */
  __HAL_LOCK(hfmpi2c);                                  /* HAL辅助宏/函数 */

  if (HAL_FMPI2C_STATE_READY == hfmpi2c->State)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_FMPI2C_MASTER_TX_COMPLETE_CB_ID :        /* case标签 */
        hfmpi2c->MasterTxCpltCallback = HAL_FMPI2C_MasterTxCpltCallback; /* Legacy weak MasterTxCpltCallback */
        break;                                          /* 跳出 */

      case HAL_FMPI2C_MASTER_RX_COMPLETE_CB_ID :        /* case标签 */
        hfmpi2c->MasterRxCpltCallback = HAL_FMPI2C_MasterRxCpltCallback; /* Legacy weak MasterRxCpltCallback */
        break;                                          /* 跳出 */

      case HAL_FMPI2C_SLAVE_TX_COMPLETE_CB_ID :         /* case标签 */
        hfmpi2c->SlaveTxCpltCallback = HAL_FMPI2C_SlaveTxCpltCallback;   /* Legacy weak SlaveTxCpltCallback  */
        break;                                          /* 跳出 */

      case HAL_FMPI2C_SLAVE_RX_COMPLETE_CB_ID :         /* case标签 */
        hfmpi2c->SlaveRxCpltCallback = HAL_FMPI2C_SlaveRxCpltCallback;   /* Legacy weak SlaveRxCpltCallback  */
        break;                                          /* 跳出 */

      case HAL_FMPI2C_LISTEN_COMPLETE_CB_ID :           /* case标签 */
        hfmpi2c->ListenCpltCallback = HAL_FMPI2C_ListenCpltCallback;     /* Legacy weak ListenCpltCallback   */
        break;                                          /* 跳出 */

      case HAL_FMPI2C_MEM_TX_COMPLETE_CB_ID :           /* case标签 */
        hfmpi2c->MemTxCpltCallback = HAL_FMPI2C_MemTxCpltCallback;       /* Legacy weak MemTxCpltCallback    */
        break;                                          /* 跳出 */

      case HAL_FMPI2C_MEM_RX_COMPLETE_CB_ID :           /* case标签 */
        hfmpi2c->MemRxCpltCallback = HAL_FMPI2C_MemRxCpltCallback;       /* Legacy weak MemRxCpltCallback    */
        break;                                          /* 跳出 */

      case HAL_FMPI2C_ERROR_CB_ID :                     /* case标签 */
        hfmpi2c->ErrorCallback = HAL_FMPI2C_ErrorCallback;               /* Legacy weak ErrorCallback        */
        break;                                          /* 跳出 */

      case HAL_FMPI2C_ABORT_CB_ID :                     /* case标签 */
        hfmpi2c->AbortCpltCallback = HAL_FMPI2C_AbortCpltCallback;       /* Legacy weak AbortCpltCallback    */
        break;                                          /* 跳出 */

      case HAL_FMPI2C_MSPINIT_CB_ID :                   /* case标签 */
        hfmpi2c->MspInitCallback = HAL_FMPI2C_MspInit;                   /* Legacy weak MspInit              */
        break;                                          /* 跳出 */

      case HAL_FMPI2C_MSPDEINIT_CB_ID :                 /* case标签 */
        hfmpi2c->MspDeInitCallback = HAL_FMPI2C_MspDeInit;               /* Legacy weak MspDeInit            */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (HAL_FMPI2C_STATE_RESET == hfmpi2c->State)    /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_FMPI2C_MSPINIT_CB_ID :                   /* case标签 */
        hfmpi2c->MspInitCallback = HAL_FMPI2C_MspInit;                   /* Legacy weak MspInit              */
        break;                                          /* 跳出 */

      case HAL_FMPI2C_MSPDEINIT_CB_ID :                 /* case标签 */
        hfmpi2c->MspDeInitCallback = HAL_FMPI2C_MspDeInit;               /* Legacy weak MspDeInit            */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hfmpi2c);                                /* HAL辅助宏/函数 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Register the Slave Address Match FMPI2C Callback
  *         To be used instead of the weak HAL_FMPI2C_AddrCallback() predefined callback
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  pCallback pointer to the Address Match Callback function
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_RegisterAddrCallback(FMPI2C_HandleTypeDef *hfmpi2c, pFMPI2C_AddrCallbackTypeDef pCallback)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  if (pCallback == NULL)                                /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  /* Process locked */
  __HAL_LOCK(hfmpi2c);                                  /* HAL辅助宏/函数 */

  if (HAL_FMPI2C_STATE_READY == hfmpi2c->State)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    hfmpi2c->AddrCallback = pCallback;                  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hfmpi2c);                                /* HAL辅助宏/函数 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  UnRegister the Slave Address Match FMPI2C Callback
  *         Info Ready FMPI2C Callback is redirected to the weak HAL_FMPI2C_AddrCallback() predefined callback
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_UnRegisterAddrCallback(FMPI2C_HandleTypeDef *hfmpi2c)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Process locked */
  __HAL_LOCK(hfmpi2c);                                  /* HAL辅助宏/函数 */

  if (HAL_FMPI2C_STATE_READY == hfmpi2c->State)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    hfmpi2c->AddrCallback = HAL_FMPI2C_AddrCallback; /* Legacy weak AddrCallback  */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hfmpi2c);                                /* HAL辅助宏/函数 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

#endif /* USE_HAL_FMPI2C_REGISTER_CALLBACKS */

/**
  * @}
  */

/** @defgroup FMPI2C_Exported_Functions_Group2 Input and Output operation functions
  *  @brief   Data transfers functions
  *
@verbatim
 ===============================================================================
                      ##### IO operation functions #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to manage the FMPI2C data
    transfers.

    (#) There are two modes of transfer:
       (++) Blocking mode : The communication is performed in the polling mode.
            The status of all data processing is returned by the same function
            after finishing transfer.
       (++) No-Blocking mode : The communication is performed using Interrupts
            or DMA. These functions return the status of the transfer startup.
            The end of the data processing will be indicated through the
            dedicated FMPI2C IRQ when using Interrupt mode or the DMA IRQ when
            using DMA mode.

    (#) Blocking mode functions are :
        (++) HAL_FMPI2C_Master_Transmit()
        (++) HAL_FMPI2C_Master_Receive()
        (++) HAL_FMPI2C_Slave_Transmit()
        (++) HAL_FMPI2C_Slave_Receive()
        (++) HAL_FMPI2C_Mem_Write()
        (++) HAL_FMPI2C_Mem_Read()
        (++) HAL_FMPI2C_IsDeviceReady()

    (#) No-Blocking mode functions with Interrupt are :
        (++) HAL_FMPI2C_Master_Transmit_IT()
        (++) HAL_FMPI2C_Master_Receive_IT()
        (++) HAL_FMPI2C_Slave_Transmit_IT()
        (++) HAL_FMPI2C_Slave_Receive_IT()
        (++) HAL_FMPI2C_Mem_Write_IT()
        (++) HAL_FMPI2C_Mem_Read_IT()
        (++) HAL_FMPI2C_Master_Seq_Transmit_IT()
        (++) HAL_FMPI2C_Master_Seq_Receive_IT()
        (++) HAL_FMPI2C_Slave_Seq_Transmit_IT()
        (++) HAL_FMPI2C_Slave_Seq_Receive_IT()
        (++) HAL_FMPI2C_EnableListen_IT()
        (++) HAL_FMPI2C_DisableListen_IT()
        (++) HAL_FMPI2C_Master_Abort_IT()

    (#) No-Blocking mode functions with DMA are :
        (++) HAL_FMPI2C_Master_Transmit_DMA()
        (++) HAL_FMPI2C_Master_Receive_DMA()
        (++) HAL_FMPI2C_Slave_Transmit_DMA()
        (++) HAL_FMPI2C_Slave_Receive_DMA()
        (++) HAL_FMPI2C_Mem_Write_DMA()
        (++) HAL_FMPI2C_Mem_Read_DMA()
        (++) HAL_FMPI2C_Master_Seq_Transmit_DMA()
        (++) HAL_FMPI2C_Master_Seq_Receive_DMA()
        (++) HAL_FMPI2C_Slave_Seq_Transmit_DMA()
        (++) HAL_FMPI2C_Slave_Seq_Receive_DMA()

    (#) A set of Transfer Complete Callbacks are provided in non Blocking mode:
        (++) HAL_FMPI2C_MasterTxCpltCallback()
        (++) HAL_FMPI2C_MasterRxCpltCallback()
        (++) HAL_FMPI2C_SlaveTxCpltCallback()
        (++) HAL_FMPI2C_SlaveRxCpltCallback()
        (++) HAL_FMPI2C_MemTxCpltCallback()
        (++) HAL_FMPI2C_MemRxCpltCallback()
        (++) HAL_FMPI2C_AddrCallback()
        (++) HAL_FMPI2C_ListenCpltCallback()
        (++) HAL_FMPI2C_ErrorCallback()
        (++) HAL_FMPI2C_AbortCpltCallback()

@endverbatim
  * @{
  */

/**
  * @brief  Transmits in master mode an amount of data in blocking mode.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Master_Transmit(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size,  /* HAL状态类型函数 */
                                          uint32_t Timeout)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */

  if (hfmpi2c->State == HAL_FMPI2C_STATE_READY)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Process Locked */
    __HAL_LOCK(hfmpi2c);                                /* HAL辅助宏/函数 */

    /* Init tickstart for timeout management*/
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */

    if (FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_BUSY, SET, FMPI2C_TIMEOUT_BUSY, tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    hfmpi2c->State     = HAL_FMPI2C_STATE_BUSY_TX;      /* 赋值或初始化 */
    hfmpi2c->Mode      = HAL_FMPI2C_MODE_MASTER;        /* 赋值或初始化 */
    hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_NONE;         /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hfmpi2c->pBuffPtr  = pData;                         /* 赋值或初始化 */
    hfmpi2c->XferCount = Size;                          /* 赋值或初始化 */
    hfmpi2c->XferISR   = NULL;                          /* 赋值或初始化 */

    /* Send Slave Address */
    /* Set NBYTES to write and reload if hfmpi2c->XferCount > MAX_NBYTE_SIZE and generate RESTART */
    if (hfmpi2c->XferCount > MAX_NBYTE_SIZE)            /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpi2c->XferSize = MAX_NBYTE_SIZE;               /* 赋值或初始化 */
      FMPI2C_TransferConfig(hfmpi2c, DevAddress, (uint8_t)hfmpi2c->XferSize, FMPI2C_RELOAD_MODE, FMPI2C_GENERATE_START_WRITE);  /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hfmpi2c->XferSize = hfmpi2c->XferCount;           /* 赋值或初始化 */
      FMPI2C_TransferConfig(hfmpi2c, DevAddress, (uint8_t)hfmpi2c->XferSize, FMPI2C_AUTOEND_MODE, FMPI2C_GENERATE_START_WRITE);  /* 执行语句 */
    }                                                   /* 代码块结束 */

    while (hfmpi2c->XferCount > 0U)                     /* while循环 */
    {                                                   /* 代码块开始 */
      /* Wait until TXIS flag is set */
      if (FMPI2C_WaitOnTXISFlagUntilTimeout(hfmpi2c, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
      /* Write data to TXDR */
      hfmpi2c->Instance->TXDR = *hfmpi2c->pBuffPtr;     /* 赋值或初始化 */

      /* Increment Buffer pointer */
      hfmpi2c->pBuffPtr++;                              /* 执行语句 */

      hfmpi2c->XferCount--;                             /* 执行语句 */
      hfmpi2c->XferSize--;                              /* 执行语句 */

      if ((hfmpi2c->XferCount != 0U) && (hfmpi2c->XferSize == 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Wait until TCR flag is set */
        if (FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_TCR, RESET, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
        {                                               /* 代码块开始 */
          return HAL_ERROR;                             /* 返回值 */
        }                                               /* 代码块结束 */

        if (hfmpi2c->XferCount > MAX_NBYTE_SIZE)        /* 条件判断 */
        {                                               /* 代码块开始 */
          hfmpi2c->XferSize = MAX_NBYTE_SIZE;           /* 赋值或初始化 */
          FMPI2C_TransferConfig(hfmpi2c, DevAddress, (uint8_t)hfmpi2c->XferSize, FMPI2C_RELOAD_MODE, FMPI2C_NO_STARTSTOP);  /* 执行语句 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          hfmpi2c->XferSize = hfmpi2c->XferCount;       /* 赋值或初始化 */
          FMPI2C_TransferConfig(hfmpi2c, DevAddress, (uint8_t)hfmpi2c->XferSize, FMPI2C_AUTOEND_MODE, FMPI2C_NO_STARTSTOP);  /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* No need to Check TC flag, with AUTOEND mode the stop is automatically generated */
    /* Wait until STOPF flag is set */
    if (FMPI2C_WaitOnSTOPFlagUntilTimeout(hfmpi2c, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Clear STOP Flag */
    __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF);  /* HAL辅助宏/函数 */

    /* Clear Configuration Register 2 */
    FMPI2C_RESET_CR2(hfmpi2c);                          /* 执行语句 */

    hfmpi2c->State = HAL_FMPI2C_STATE_READY;            /* 赋值或初始化 */
    hfmpi2c->Mode  = HAL_FMPI2C_MODE_NONE;              /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c);                              /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Receives in master mode an amount of data in blocking mode.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Master_Receive(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size,  /* HAL状态类型函数 */
                                         uint32_t Timeout)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */

  if (hfmpi2c->State == HAL_FMPI2C_STATE_READY)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Process Locked */
    __HAL_LOCK(hfmpi2c);                                /* HAL辅助宏/函数 */

    /* Init tickstart for timeout management*/
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */

    if (FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_BUSY, SET, FMPI2C_TIMEOUT_BUSY, tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    hfmpi2c->State     = HAL_FMPI2C_STATE_BUSY_RX;      /* 赋值或初始化 */
    hfmpi2c->Mode      = HAL_FMPI2C_MODE_MASTER;        /* 赋值或初始化 */
    hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_NONE;         /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hfmpi2c->pBuffPtr  = pData;                         /* 赋值或初始化 */
    hfmpi2c->XferCount = Size;                          /* 赋值或初始化 */
    hfmpi2c->XferISR   = NULL;                          /* 赋值或初始化 */

    /* Send Slave Address */
    /* Set NBYTES to write and reload if hfmpi2c->XferCount > MAX_NBYTE_SIZE and generate RESTART */
    if (hfmpi2c->XferCount > MAX_NBYTE_SIZE)            /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpi2c->XferSize = MAX_NBYTE_SIZE;               /* 赋值或初始化 */
      FMPI2C_TransferConfig(hfmpi2c, DevAddress, (uint8_t)hfmpi2c->XferSize, FMPI2C_RELOAD_MODE, FMPI2C_GENERATE_START_READ);  /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hfmpi2c->XferSize = hfmpi2c->XferCount;           /* 赋值或初始化 */
      FMPI2C_TransferConfig(hfmpi2c, DevAddress, (uint8_t)hfmpi2c->XferSize, FMPI2C_AUTOEND_MODE, FMPI2C_GENERATE_START_READ);  /* 执行语句 */
    }                                                   /* 代码块结束 */

    while (hfmpi2c->XferCount > 0U)                     /* while循环 */
    {                                                   /* 代码块开始 */
      /* Wait until RXNE flag is set */
      if (FMPI2C_WaitOnRXNEFlagUntilTimeout(hfmpi2c, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */

      /* Read data from RXDR */
      *hfmpi2c->pBuffPtr = (uint8_t)hfmpi2c->Instance->RXDR;  /* 赋值或初始化 */

      /* Increment Buffer pointer */
      hfmpi2c->pBuffPtr++;                              /* 执行语句 */

      hfmpi2c->XferSize--;                              /* 执行语句 */
      hfmpi2c->XferCount--;                             /* 执行语句 */

      if ((hfmpi2c->XferCount != 0U) && (hfmpi2c->XferSize == 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Wait until TCR flag is set */
        if (FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_TCR, RESET, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
        {                                               /* 代码块开始 */
          return HAL_ERROR;                             /* 返回值 */
        }                                               /* 代码块结束 */

        if (hfmpi2c->XferCount > MAX_NBYTE_SIZE)        /* 条件判断 */
        {                                               /* 代码块开始 */
          hfmpi2c->XferSize = MAX_NBYTE_SIZE;           /* 赋值或初始化 */
          FMPI2C_TransferConfig(hfmpi2c, DevAddress, (uint8_t)hfmpi2c->XferSize, FMPI2C_RELOAD_MODE, FMPI2C_NO_STARTSTOP);  /* 执行语句 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          hfmpi2c->XferSize = hfmpi2c->XferCount;       /* 赋值或初始化 */
          FMPI2C_TransferConfig(hfmpi2c, DevAddress, (uint8_t)hfmpi2c->XferSize, FMPI2C_AUTOEND_MODE, FMPI2C_NO_STARTSTOP);  /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* No need to Check TC flag, with AUTOEND mode the stop is automatically generated */
    /* Wait until STOPF flag is set */
    if (FMPI2C_WaitOnSTOPFlagUntilTimeout(hfmpi2c, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Clear STOP Flag */
    __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF);  /* HAL辅助宏/函数 */

    /* Clear Configuration Register 2 */
    FMPI2C_RESET_CR2(hfmpi2c);                          /* 执行语句 */

    hfmpi2c->State = HAL_FMPI2C_STATE_READY;            /* 赋值或初始化 */
    hfmpi2c->Mode  = HAL_FMPI2C_MODE_NONE;              /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c);                              /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Transmits in slave mode an amount of data in blocking mode.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Slave_Transmit(FMPI2C_HandleTypeDef *hfmpi2c, uint8_t *pData, uint16_t Size, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */

  if (hfmpi2c->State == HAL_FMPI2C_STATE_READY)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) || (Size == 0U))                /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_INVALID_PARAM;  /* 赋值或初始化 */
      return  HAL_ERROR;                                /* 返回值 */
    }                                                   /* 代码块结束 */
    /* Process Locked */
    __HAL_LOCK(hfmpi2c);                                /* HAL辅助宏/函数 */

    /* Init tickstart for timeout management*/
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */

    hfmpi2c->State     = HAL_FMPI2C_STATE_BUSY_TX;      /* 赋值或初始化 */
    hfmpi2c->Mode      = HAL_FMPI2C_MODE_SLAVE;         /* 赋值或初始化 */
    hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_NONE;         /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hfmpi2c->pBuffPtr  = pData;                         /* 赋值或初始化 */
    hfmpi2c->XferCount = Size;                          /* 赋值或初始化 */
    hfmpi2c->XferISR   = NULL;                          /* 赋值或初始化 */

    /* Enable Address Acknowledge */
    hfmpi2c->Instance->CR2 &= ~FMPI2C_CR2_NACK;         /* 赋值或初始化 */

    /* Wait until ADDR flag is set */
    if (FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_ADDR, RESET, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable Address Acknowledge */
      hfmpi2c->Instance->CR2 |= FMPI2C_CR2_NACK;        /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Clear ADDR flag */
    __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_ADDR);  /* HAL辅助宏/函数 */

    /* If 10bit addressing mode is selected */
    if (hfmpi2c->Init.AddressingMode == FMPI2C_ADDRESSINGMODE_10BIT)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Wait until ADDR flag is set */
      if (FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_ADDR, RESET, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Disable Address Acknowledge */
        hfmpi2c->Instance->CR2 |= FMPI2C_CR2_NACK;      /* 赋值或初始化 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */

      /* Clear ADDR flag */
      __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_ADDR);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Wait until DIR flag is set Transmitter mode */
    if (FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_DIR, RESET, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable Address Acknowledge */
      hfmpi2c->Instance->CR2 |= FMPI2C_CR2_NACK;        /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    while (hfmpi2c->XferCount > 0U)                     /* while循环 */
    {                                                   /* 代码块开始 */
      /* Wait until TXIS flag is set */
      if (FMPI2C_WaitOnTXISFlagUntilTimeout(hfmpi2c, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Disable Address Acknowledge */
        hfmpi2c->Instance->CR2 |= FMPI2C_CR2_NACK;      /* 赋值或初始化 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */

      /* Write data to TXDR */
      hfmpi2c->Instance->TXDR = *hfmpi2c->pBuffPtr;     /* 赋值或初始化 */

      /* Increment Buffer pointer */
      hfmpi2c->pBuffPtr++;                              /* 执行语句 */

      hfmpi2c->XferCount--;                             /* 执行语句 */
    }                                                   /* 代码块结束 */

    /* Wait until STOP flag is set */
    if (FMPI2C_WaitOnSTOPFlagUntilTimeout(hfmpi2c, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable Address Acknowledge */
      hfmpi2c->Instance->CR2 |= FMPI2C_CR2_NACK;        /* 赋值或初始化 */

      if (hfmpi2c->ErrorCode == HAL_FMPI2C_ERROR_AF)    /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Normal use case for Transmitter mode */
        /* A NACK is generated to confirm the end of transfer */
        hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_NONE;     /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Clear STOP flag */
    __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF);  /* HAL辅助宏/函数 */

    /* Wait until BUSY flag is reset */
    if (FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_BUSY, SET, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable Address Acknowledge */
      hfmpi2c->Instance->CR2 |= FMPI2C_CR2_NACK;        /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Disable Address Acknowledge */
    hfmpi2c->Instance->CR2 |= FMPI2C_CR2_NACK;          /* 赋值或初始化 */

    hfmpi2c->State = HAL_FMPI2C_STATE_READY;            /* 赋值或初始化 */
    hfmpi2c->Mode  = HAL_FMPI2C_MODE_NONE;              /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c);                              /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Receive in slave mode an amount of data in blocking mode
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Slave_Receive(FMPI2C_HandleTypeDef *hfmpi2c, uint8_t *pData, uint16_t Size, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */

  if (hfmpi2c->State == HAL_FMPI2C_STATE_READY)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) || (Size == 0U))                /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_INVALID_PARAM;  /* 赋值或初始化 */
      return  HAL_ERROR;                                /* 返回值 */
    }                                                   /* 代码块结束 */
    /* Process Locked */
    __HAL_LOCK(hfmpi2c);                                /* HAL辅助宏/函数 */

    /* Init tickstart for timeout management*/
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */

    hfmpi2c->State     = HAL_FMPI2C_STATE_BUSY_RX;      /* 赋值或初始化 */
    hfmpi2c->Mode      = HAL_FMPI2C_MODE_SLAVE;         /* 赋值或初始化 */
    hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_NONE;         /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hfmpi2c->pBuffPtr  = pData;                         /* 赋值或初始化 */
    hfmpi2c->XferCount = Size;                          /* 赋值或初始化 */
    hfmpi2c->XferISR   = NULL;                          /* 赋值或初始化 */

    /* Enable Address Acknowledge */
    hfmpi2c->Instance->CR2 &= ~FMPI2C_CR2_NACK;         /* 赋值或初始化 */

    /* Wait until ADDR flag is set */
    if (FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_ADDR, RESET, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable Address Acknowledge */
      hfmpi2c->Instance->CR2 |= FMPI2C_CR2_NACK;        /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Clear ADDR flag */
    __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_ADDR);  /* HAL辅助宏/函数 */

    /* Wait until DIR flag is reset Receiver mode */
    if (FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_DIR, SET, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable Address Acknowledge */
      hfmpi2c->Instance->CR2 |= FMPI2C_CR2_NACK;        /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    while (hfmpi2c->XferCount > 0U)                     /* while循环 */
    {                                                   /* 代码块开始 */
      /* Wait until RXNE flag is set */
      if (FMPI2C_WaitOnRXNEFlagUntilTimeout(hfmpi2c, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Disable Address Acknowledge */
        hfmpi2c->Instance->CR2 |= FMPI2C_CR2_NACK;      /* 赋值或初始化 */

        /* Store Last receive data if any */
        if (__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_RXNE) == SET)  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Read data from RXDR */
          *hfmpi2c->pBuffPtr = (uint8_t)hfmpi2c->Instance->RXDR;  /* 赋值或初始化 */

          /* Increment Buffer pointer */
          hfmpi2c->pBuffPtr++;                          /* 执行语句 */

          hfmpi2c->XferCount--;                         /* 执行语句 */
        }                                               /* 代码块结束 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */

      /* Read data from RXDR */
      *hfmpi2c->pBuffPtr = (uint8_t)hfmpi2c->Instance->RXDR;  /* 赋值或初始化 */

      /* Increment Buffer pointer */
      hfmpi2c->pBuffPtr++;                              /* 执行语句 */

      hfmpi2c->XferCount--;                             /* 执行语句 */
    }                                                   /* 代码块结束 */

    /* Wait until STOP flag is set */
    if (FMPI2C_WaitOnSTOPFlagUntilTimeout(hfmpi2c, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable Address Acknowledge */
      hfmpi2c->Instance->CR2 |= FMPI2C_CR2_NACK;        /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Clear STOP flag */
    __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF);  /* HAL辅助宏/函数 */

    /* Wait until BUSY flag is reset */
    if (FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_BUSY, SET, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable Address Acknowledge */
      hfmpi2c->Instance->CR2 |= FMPI2C_CR2_NACK;        /* 赋值或初始化 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Disable Address Acknowledge */
    hfmpi2c->Instance->CR2 |= FMPI2C_CR2_NACK;          /* 赋值或初始化 */

    hfmpi2c->State = HAL_FMPI2C_STATE_READY;            /* 赋值或初始化 */
    hfmpi2c->Mode  = HAL_FMPI2C_MODE_NONE;              /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c);                              /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Transmit in master mode an amount of data in non-blocking mode with Interrupt
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Master_Transmit_IT(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t DevAddress, uint8_t *pData,  /* HAL状态类型函数 */
                                             uint16_t Size)  /* 16位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t xfermode;                                    /* 32位无符号变量/函数 */

  if (hfmpi2c->State == HAL_FMPI2C_STATE_READY)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_BUSY) == SET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_BUSY;                                  /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hfmpi2c);                                /* HAL辅助宏/函数 */

    hfmpi2c->State       = HAL_FMPI2C_STATE_BUSY_TX;    /* 赋值或初始化 */
    hfmpi2c->Mode        = HAL_FMPI2C_MODE_MASTER;      /* 赋值或初始化 */
    hfmpi2c->ErrorCode   = HAL_FMPI2C_ERROR_NONE;       /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hfmpi2c->pBuffPtr    = pData;                       /* 赋值或初始化 */
    hfmpi2c->XferCount   = Size;                        /* 赋值或初始化 */
    hfmpi2c->XferOptions = FMPI2C_NO_OPTION_FRAME;      /* 赋值或初始化 */
    hfmpi2c->XferISR     = FMPI2C_Master_ISR_IT;        /* 赋值或初始化 */

    if (hfmpi2c->XferCount > MAX_NBYTE_SIZE)            /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpi2c->XferSize = MAX_NBYTE_SIZE;               /* 赋值或初始化 */
      xfermode = FMPI2C_RELOAD_MODE;                    /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hfmpi2c->XferSize = hfmpi2c->XferCount;           /* 赋值或初始化 */
      xfermode = FMPI2C_AUTOEND_MODE;                   /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Send Slave Address */
    /* Set NBYTES to write and reload if hfmpi2c->XferCount > MAX_NBYTE_SIZE */
    FMPI2C_TransferConfig(hfmpi2c, DevAddress, (uint8_t)hfmpi2c->XferSize, xfermode, FMPI2C_GENERATE_START_WRITE);  /* 执行语句 */

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c);                              /* HAL辅助宏/函数 */

    /* Note : The FMPI2C interrupts must be enabled after unlocking current process
              to avoid the risk of FMPI2C interrupt handle execution before current
              process unlock */

    /* Enable ERR, TC, STOP, NACK, TXI interrupt */
    /* possible to enable all of these */
    /* FMPI2C_IT_ERRI | FMPI2C_IT_TCI| FMPI2C_IT_STOPI| FMPI2C_IT_NACKI | FMPI2C_IT_ADDRI | FMPI2C_IT_RXI | FMPI2C_IT_TXI */
    FMPI2C_Enable_IRQ(hfmpi2c, FMPI2C_XFER_TX_IT);      /* 执行语句 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Receive in master mode an amount of data in non-blocking mode with Interrupt
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Master_Receive_IT(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t xfermode;                                    /* 32位无符号变量/函数 */

  if (hfmpi2c->State == HAL_FMPI2C_STATE_READY)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_BUSY) == SET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_BUSY;                                  /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hfmpi2c);                                /* HAL辅助宏/函数 */

    hfmpi2c->State       = HAL_FMPI2C_STATE_BUSY_RX;    /* 赋值或初始化 */
    hfmpi2c->Mode        = HAL_FMPI2C_MODE_MASTER;      /* 赋值或初始化 */
    hfmpi2c->ErrorCode   = HAL_FMPI2C_ERROR_NONE;       /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hfmpi2c->pBuffPtr    = pData;                       /* 赋值或初始化 */
    hfmpi2c->XferCount   = Size;                        /* 赋值或初始化 */
    hfmpi2c->XferOptions = FMPI2C_NO_OPTION_FRAME;      /* 赋值或初始化 */
    hfmpi2c->XferISR     = FMPI2C_Master_ISR_IT;        /* 赋值或初始化 */

    if (hfmpi2c->XferCount > MAX_NBYTE_SIZE)            /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpi2c->XferSize = MAX_NBYTE_SIZE;               /* 赋值或初始化 */
      xfermode = FMPI2C_RELOAD_MODE;                    /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hfmpi2c->XferSize = hfmpi2c->XferCount;           /* 赋值或初始化 */
      xfermode = FMPI2C_AUTOEND_MODE;                   /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Send Slave Address */
    /* Set NBYTES to write and reload if hfmpi2c->XferCount > MAX_NBYTE_SIZE */
    FMPI2C_TransferConfig(hfmpi2c, DevAddress, (uint8_t)hfmpi2c->XferSize, xfermode, FMPI2C_GENERATE_START_READ);  /* 执行语句 */

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c);                              /* HAL辅助宏/函数 */

    /* Note : The FMPI2C interrupts must be enabled after unlocking current process
              to avoid the risk of FMPI2C interrupt handle execution before current
              process unlock */

    /* Enable ERR, TC, STOP, NACK, RXI interrupt */
    /* possible to enable all of these */
    /* FMPI2C_IT_ERRI | FMPI2C_IT_TCI| FMPI2C_IT_STOPI| FMPI2C_IT_NACKI | FMPI2C_IT_ADDRI | FMPI2C_IT_RXI | FMPI2C_IT_TXI */
    FMPI2C_Enable_IRQ(hfmpi2c, FMPI2C_XFER_RX_IT);      /* 执行语句 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Transmit in slave mode an amount of data in non-blocking mode with Interrupt
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Slave_Transmit_IT(FMPI2C_HandleTypeDef *hfmpi2c, uint8_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  if (hfmpi2c->State == HAL_FMPI2C_STATE_READY)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Process Locked */
    __HAL_LOCK(hfmpi2c);                                /* HAL辅助宏/函数 */

    hfmpi2c->State       = HAL_FMPI2C_STATE_BUSY_TX;    /* 赋值或初始化 */
    hfmpi2c->Mode        = HAL_FMPI2C_MODE_SLAVE;       /* 赋值或初始化 */
    hfmpi2c->ErrorCode   = HAL_FMPI2C_ERROR_NONE;       /* 赋值或初始化 */

    /* Enable Address Acknowledge */
    hfmpi2c->Instance->CR2 &= ~FMPI2C_CR2_NACK;         /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hfmpi2c->pBuffPtr    = pData;                       /* 赋值或初始化 */
    hfmpi2c->XferCount   = Size;                        /* 赋值或初始化 */
    hfmpi2c->XferSize    = hfmpi2c->XferCount;          /* 赋值或初始化 */
    hfmpi2c->XferOptions = FMPI2C_NO_OPTION_FRAME;      /* 赋值或初始化 */
    hfmpi2c->XferISR     = FMPI2C_Slave_ISR_IT;         /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c);                              /* HAL辅助宏/函数 */

    /* Note : The FMPI2C interrupts must be enabled after unlocking current process
              to avoid the risk of FMPI2C interrupt handle execution before current
              process unlock */

    /* Enable ERR, TC, STOP, NACK, TXI interrupt */
    /* possible to enable all of these */
    /* FMPI2C_IT_ERRI | FMPI2C_IT_TCI| FMPI2C_IT_STOPI| FMPI2C_IT_NACKI | FMPI2C_IT_ADDRI | FMPI2C_IT_RXI | FMPI2C_IT_TXI */
    FMPI2C_Enable_IRQ(hfmpi2c, FMPI2C_XFER_TX_IT | FMPI2C_XFER_LISTEN_IT);  /* 执行语句 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Receive in slave mode an amount of data in non-blocking mode with Interrupt
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Slave_Receive_IT(FMPI2C_HandleTypeDef *hfmpi2c, uint8_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  if (hfmpi2c->State == HAL_FMPI2C_STATE_READY)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Process Locked */
    __HAL_LOCK(hfmpi2c);                                /* HAL辅助宏/函数 */

    hfmpi2c->State       = HAL_FMPI2C_STATE_BUSY_RX;    /* 赋值或初始化 */
    hfmpi2c->Mode        = HAL_FMPI2C_MODE_SLAVE;       /* 赋值或初始化 */
    hfmpi2c->ErrorCode   = HAL_FMPI2C_ERROR_NONE;       /* 赋值或初始化 */

    /* Enable Address Acknowledge */
    hfmpi2c->Instance->CR2 &= ~FMPI2C_CR2_NACK;         /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hfmpi2c->pBuffPtr    = pData;                       /* 赋值或初始化 */
    hfmpi2c->XferCount   = Size;                        /* 赋值或初始化 */
    hfmpi2c->XferSize    = hfmpi2c->XferCount;          /* 赋值或初始化 */
    hfmpi2c->XferOptions = FMPI2C_NO_OPTION_FRAME;      /* 赋值或初始化 */
    hfmpi2c->XferISR     = FMPI2C_Slave_ISR_IT;         /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c);                              /* HAL辅助宏/函数 */

    /* Note : The FMPI2C interrupts must be enabled after unlocking current process
              to avoid the risk of FMPI2C interrupt handle execution before current
              process unlock */

    /* Enable ERR, TC, STOP, NACK, RXI interrupt */
    /* possible to enable all of these */
    /* FMPI2C_IT_ERRI | FMPI2C_IT_TCI| FMPI2C_IT_STOPI| FMPI2C_IT_NACKI | FMPI2C_IT_ADDRI | FMPI2C_IT_RXI | FMPI2C_IT_TXI */
    FMPI2C_Enable_IRQ(hfmpi2c, FMPI2C_XFER_RX_IT | FMPI2C_XFER_LISTEN_IT);  /* 执行语句 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Transmit in master mode an amount of data in non-blocking mode with DMA
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Master_Transmit_DMA(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t DevAddress, uint8_t *pData,  /* HAL状态类型函数 */
                                              uint16_t Size)  /* 16位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t xfermode;                                    /* 32位无符号变量/函数 */
  HAL_StatusTypeDef dmaxferstatus;                      /* HAL状态类型函数 */

  if (hfmpi2c->State == HAL_FMPI2C_STATE_READY)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_BUSY) == SET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_BUSY;                                  /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hfmpi2c);                                /* HAL辅助宏/函数 */

    hfmpi2c->State       = HAL_FMPI2C_STATE_BUSY_TX;    /* 赋值或初始化 */
    hfmpi2c->Mode        = HAL_FMPI2C_MODE_MASTER;      /* 赋值或初始化 */
    hfmpi2c->ErrorCode   = HAL_FMPI2C_ERROR_NONE;       /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hfmpi2c->pBuffPtr    = pData;                       /* 赋值或初始化 */
    hfmpi2c->XferCount   = Size;                        /* 赋值或初始化 */
    hfmpi2c->XferOptions = FMPI2C_NO_OPTION_FRAME;      /* 赋值或初始化 */
    hfmpi2c->XferISR     = FMPI2C_Master_ISR_DMA;       /* 赋值或初始化 */

    if (hfmpi2c->XferCount > MAX_NBYTE_SIZE)            /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpi2c->XferSize = MAX_NBYTE_SIZE;               /* 赋值或初始化 */
      xfermode = FMPI2C_RELOAD_MODE;                    /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hfmpi2c->XferSize = hfmpi2c->XferCount;           /* 赋值或初始化 */
      xfermode = FMPI2C_AUTOEND_MODE;                   /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    if (hfmpi2c->XferSize > 0U)                         /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (hfmpi2c->hdmatx != NULL)                      /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Set the FMPI2C DMA transfer complete callback */
        hfmpi2c->hdmatx->XferCpltCallback = FMPI2C_DMAMasterTransmitCplt;  /* 赋值或初始化 */

        /* Set the DMA error callback */
        hfmpi2c->hdmatx->XferErrorCallback = FMPI2C_DMAError;  /* 赋值或初始化 */

        /* Set the unused DMA callbacks to NULL */
        hfmpi2c->hdmatx->XferHalfCpltCallback = NULL;   /* 赋值或初始化 */
        hfmpi2c->hdmatx->XferAbortCallback = NULL;      /* 赋值或初始化 */

        /* Enable the DMA stream */
        dmaxferstatus = HAL_DMA_Start_IT(hfmpi2c->hdmatx, (uint32_t)pData, (uint32_t)&hfmpi2c->Instance->TXDR, hfmpi2c->XferSize);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Update FMPI2C state */
        hfmpi2c->State     = HAL_FMPI2C_STATE_READY;    /* 赋值或初始化 */
        hfmpi2c->Mode      = HAL_FMPI2C_MODE_NONE;      /* 赋值或初始化 */

        /* Update FMPI2C error code */
        hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_DMA_PARAM;  /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hfmpi2c);                          /* HAL辅助宏/函数 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */

      if (dmaxferstatus == HAL_OK)                      /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Send Slave Address */
        /* Set NBYTES to write and reload if hfmpi2c->XferCount > MAX_NBYTE_SIZE and generate RESTART */
        FMPI2C_TransferConfig(hfmpi2c, DevAddress, (uint8_t)hfmpi2c->XferSize, xfermode, FMPI2C_GENERATE_START_WRITE);  /* 执行语句 */

        /* Update XferCount value */
        hfmpi2c->XferCount -= hfmpi2c->XferSize;        /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hfmpi2c);                          /* HAL辅助宏/函数 */

        /* Note : The FMPI2C interrupts must be enabled after unlocking current process
                  to avoid the risk of FMPI2C interrupt handle execution before current
                  process unlock */
        /* Enable ERR and NACK interrupts */
        FMPI2C_Enable_IRQ(hfmpi2c, FMPI2C_XFER_ERROR_IT);  /* 执行语句 */

        /* Enable DMA Request */
        hfmpi2c->Instance->CR1 |= FMPI2C_CR1_TXDMAEN;   /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Update FMPI2C state */
        hfmpi2c->State     = HAL_FMPI2C_STATE_READY;    /* 赋值或初始化 */
        hfmpi2c->Mode      = HAL_FMPI2C_MODE_NONE;      /* 赋值或初始化 */

        /* Update FMPI2C error code */
        hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_DMA;     /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hfmpi2c);                          /* HAL辅助宏/函数 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Update Transfer ISR function pointer */
      hfmpi2c->XferISR = FMPI2C_Master_ISR_IT;          /* 赋值或初始化 */

      /* Send Slave Address */
      /* Set NBYTES to write and generate START condition */
      FMPI2C_TransferConfig(hfmpi2c, DevAddress, (uint8_t)hfmpi2c->XferSize, FMPI2C_AUTOEND_MODE, FMPI2C_GENERATE_START_WRITE);  /* 执行语句 */

      /* Process Unlocked */
      __HAL_UNLOCK(hfmpi2c);                            /* HAL辅助宏/函数 */

      /* Note : The FMPI2C interrupts must be enabled after unlocking current process
                to avoid the risk of FMPI2C interrupt handle execution before current
                process unlock */
      /* Enable ERR, TC, STOP, NACK, TXI interrupt */
      /* possible to enable all of these */
      /* FMPI2C_IT_ERRI | FMPI2C_IT_TCI| FMPI2C_IT_STOPI| FMPI2C_IT_NACKI | FMPI2C_IT_ADDRI | FMPI2C_IT_RXI | FMPI2C_IT_TXI */
      FMPI2C_Enable_IRQ(hfmpi2c, FMPI2C_XFER_TX_IT);    /* 执行语句 */
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
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Master_Receive_DMA(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t DevAddress, uint8_t *pData,  /* HAL状态类型函数 */
                                             uint16_t Size)  /* 16位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t xfermode;                                    /* 32位无符号变量/函数 */
  HAL_StatusTypeDef dmaxferstatus;                      /* HAL状态类型函数 */

  if (hfmpi2c->State == HAL_FMPI2C_STATE_READY)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_BUSY) == SET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_BUSY;                                  /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hfmpi2c);                                /* HAL辅助宏/函数 */

    hfmpi2c->State       = HAL_FMPI2C_STATE_BUSY_RX;    /* 赋值或初始化 */
    hfmpi2c->Mode        = HAL_FMPI2C_MODE_MASTER;      /* 赋值或初始化 */
    hfmpi2c->ErrorCode   = HAL_FMPI2C_ERROR_NONE;       /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hfmpi2c->pBuffPtr    = pData;                       /* 赋值或初始化 */
    hfmpi2c->XferCount   = Size;                        /* 赋值或初始化 */
    hfmpi2c->XferOptions = FMPI2C_NO_OPTION_FRAME;      /* 赋值或初始化 */
    hfmpi2c->XferISR     = FMPI2C_Master_ISR_DMA;       /* 赋值或初始化 */

    if (hfmpi2c->XferCount > MAX_NBYTE_SIZE)            /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpi2c->XferSize = MAX_NBYTE_SIZE;               /* 赋值或初始化 */
      xfermode = FMPI2C_RELOAD_MODE;                    /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hfmpi2c->XferSize = hfmpi2c->XferCount;           /* 赋值或初始化 */
      xfermode = FMPI2C_AUTOEND_MODE;                   /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    if (hfmpi2c->XferSize > 0U)                         /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (hfmpi2c->hdmarx != NULL)                      /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Set the FMPI2C DMA transfer complete callback */
        hfmpi2c->hdmarx->XferCpltCallback = FMPI2C_DMAMasterReceiveCplt;  /* 赋值或初始化 */

        /* Set the DMA error callback */
        hfmpi2c->hdmarx->XferErrorCallback = FMPI2C_DMAError;  /* 赋值或初始化 */

        /* Set the unused DMA callbacks to NULL */
        hfmpi2c->hdmarx->XferHalfCpltCallback = NULL;   /* 赋值或初始化 */
        hfmpi2c->hdmarx->XferAbortCallback = NULL;      /* 赋值或初始化 */

        /* Enable the DMA stream */
        dmaxferstatus = HAL_DMA_Start_IT(hfmpi2c->hdmarx, (uint32_t)&hfmpi2c->Instance->RXDR, (uint32_t)pData, hfmpi2c->XferSize);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Update FMPI2C state */
        hfmpi2c->State     = HAL_FMPI2C_STATE_READY;    /* 赋值或初始化 */
        hfmpi2c->Mode      = HAL_FMPI2C_MODE_NONE;      /* 赋值或初始化 */

        /* Update FMPI2C error code */
        hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_DMA_PARAM;  /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hfmpi2c);                          /* HAL辅助宏/函数 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */

      if (dmaxferstatus == HAL_OK)                      /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Send Slave Address */
        /* Set NBYTES to read and reload if hfmpi2c->XferCount > MAX_NBYTE_SIZE and generate RESTART */
        FMPI2C_TransferConfig(hfmpi2c, DevAddress, (uint8_t)hfmpi2c->XferSize, xfermode, FMPI2C_GENERATE_START_READ);  /* 执行语句 */

        /* Update XferCount value */
        hfmpi2c->XferCount -= hfmpi2c->XferSize;        /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hfmpi2c);                          /* HAL辅助宏/函数 */

        /* Note : The FMPI2C interrupts must be enabled after unlocking current process
                  to avoid the risk of FMPI2C interrupt handle execution before current
                  process unlock */
        /* Enable ERR and NACK interrupts */
        FMPI2C_Enable_IRQ(hfmpi2c, FMPI2C_XFER_ERROR_IT);  /* 执行语句 */

        /* Enable DMA Request */
        hfmpi2c->Instance->CR1 |= FMPI2C_CR1_RXDMAEN;   /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Update FMPI2C state */
        hfmpi2c->State     = HAL_FMPI2C_STATE_READY;    /* 赋值或初始化 */
        hfmpi2c->Mode      = HAL_FMPI2C_MODE_NONE;      /* 赋值或初始化 */

        /* Update FMPI2C error code */
        hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_DMA;     /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hfmpi2c);                          /* HAL辅助宏/函数 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Update Transfer ISR function pointer */
      hfmpi2c->XferISR = FMPI2C_Master_ISR_IT;          /* 赋值或初始化 */

      /* Send Slave Address */
      /* Set NBYTES to read and generate START condition */
      FMPI2C_TransferConfig(hfmpi2c, DevAddress, (uint8_t)hfmpi2c->XferSize, FMPI2C_AUTOEND_MODE, FMPI2C_GENERATE_START_READ);  /* 执行语句 */

      /* Process Unlocked */
      __HAL_UNLOCK(hfmpi2c);                            /* HAL辅助宏/函数 */

      /* Note : The FMPI2C interrupts must be enabled after unlocking current process
                to avoid the risk of FMPI2C interrupt handle execution before current
                process unlock */
      /* Enable ERR, TC, STOP, NACK, TXI interrupt */
      /* possible to enable all of these */
      /* FMPI2C_IT_ERRI | FMPI2C_IT_TCI| FMPI2C_IT_STOPI| FMPI2C_IT_NACKI | FMPI2C_IT_ADDRI | FMPI2C_IT_RXI | FMPI2C_IT_TXI */
      FMPI2C_Enable_IRQ(hfmpi2c, FMPI2C_XFER_TX_IT);    /* 执行语句 */
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
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Slave_Transmit_DMA(FMPI2C_HandleTypeDef *hfmpi2c, uint8_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef dmaxferstatus;                      /* HAL状态类型函数 */

  if (hfmpi2c->State == HAL_FMPI2C_STATE_READY)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) || (Size == 0U))                /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_INVALID_PARAM;  /* 赋值或初始化 */
      return  HAL_ERROR;                                /* 返回值 */
    }                                                   /* 代码块结束 */
    /* Process Locked */
    __HAL_LOCK(hfmpi2c);                                /* HAL辅助宏/函数 */

    hfmpi2c->State       = HAL_FMPI2C_STATE_BUSY_TX;    /* 赋值或初始化 */
    hfmpi2c->Mode        = HAL_FMPI2C_MODE_SLAVE;       /* 赋值或初始化 */
    hfmpi2c->ErrorCode   = HAL_FMPI2C_ERROR_NONE;       /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hfmpi2c->pBuffPtr    = pData;                       /* 赋值或初始化 */
    hfmpi2c->XferCount   = Size;                        /* 赋值或初始化 */
    hfmpi2c->XferSize    = hfmpi2c->XferCount;          /* 赋值或初始化 */
    hfmpi2c->XferOptions = FMPI2C_NO_OPTION_FRAME;      /* 赋值或初始化 */
    hfmpi2c->XferISR     = FMPI2C_Slave_ISR_DMA;        /* 赋值或初始化 */

    if (hfmpi2c->hdmatx != NULL)                        /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the FMPI2C DMA transfer complete callback */
      hfmpi2c->hdmatx->XferCpltCallback = FMPI2C_DMASlaveTransmitCplt;  /* 赋值或初始化 */

      /* Set the DMA error callback */
      hfmpi2c->hdmatx->XferErrorCallback = FMPI2C_DMAError;  /* 赋值或初始化 */

      /* Set the unused DMA callbacks to NULL */
      hfmpi2c->hdmatx->XferHalfCpltCallback = NULL;     /* 赋值或初始化 */
      hfmpi2c->hdmatx->XferAbortCallback = NULL;        /* 赋值或初始化 */

      /* Enable the DMA stream */
      dmaxferstatus = HAL_DMA_Start_IT(hfmpi2c->hdmatx, (uint32_t)pData, (uint32_t)&hfmpi2c->Instance->TXDR, hfmpi2c->XferSize);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Update FMPI2C state */
      hfmpi2c->State     = HAL_FMPI2C_STATE_LISTEN;     /* 赋值或初始化 */
      hfmpi2c->Mode      = HAL_FMPI2C_MODE_NONE;        /* 赋值或初始化 */

      /* Update FMPI2C error code */
      hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_DMA_PARAM;  /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hfmpi2c);                            /* HAL辅助宏/函数 */

      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    if (dmaxferstatus == HAL_OK)                        /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable Address Acknowledge */
      hfmpi2c->Instance->CR2 &= ~FMPI2C_CR2_NACK;       /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hfmpi2c);                            /* HAL辅助宏/函数 */

      /* Note : The FMPI2C interrupts must be enabled after unlocking current process
                to avoid the risk of FMPI2C interrupt handle execution before current
                process unlock */
      /* Enable ERR, STOP, NACK, ADDR interrupts */
      FMPI2C_Enable_IRQ(hfmpi2c, FMPI2C_XFER_LISTEN_IT);  /* 执行语句 */

      /* Enable DMA Request */
      hfmpi2c->Instance->CR1 |= FMPI2C_CR1_TXDMAEN;     /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Update FMPI2C state */
      hfmpi2c->State     = HAL_FMPI2C_STATE_LISTEN;     /* 赋值或初始化 */
      hfmpi2c->Mode      = HAL_FMPI2C_MODE_NONE;        /* 赋值或初始化 */

      /* Update FMPI2C error code */
      hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_DMA;       /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hfmpi2c);                            /* HAL辅助宏/函数 */

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
  * @brief  Receive in slave mode an amount of data in non-blocking mode with DMA
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Slave_Receive_DMA(FMPI2C_HandleTypeDef *hfmpi2c, uint8_t *pData, uint16_t Size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef dmaxferstatus;                      /* HAL状态类型函数 */

  if (hfmpi2c->State == HAL_FMPI2C_STATE_READY)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) || (Size == 0U))                /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_INVALID_PARAM;  /* 赋值或初始化 */
      return  HAL_ERROR;                                /* 返回值 */
    }                                                   /* 代码块结束 */
    /* Process Locked */
    __HAL_LOCK(hfmpi2c);                                /* HAL辅助宏/函数 */

    hfmpi2c->State       = HAL_FMPI2C_STATE_BUSY_RX;    /* 赋值或初始化 */
    hfmpi2c->Mode        = HAL_FMPI2C_MODE_SLAVE;       /* 赋值或初始化 */
    hfmpi2c->ErrorCode   = HAL_FMPI2C_ERROR_NONE;       /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hfmpi2c->pBuffPtr    = pData;                       /* 赋值或初始化 */
    hfmpi2c->XferCount   = Size;                        /* 赋值或初始化 */
    hfmpi2c->XferSize    = hfmpi2c->XferCount;          /* 赋值或初始化 */
    hfmpi2c->XferOptions = FMPI2C_NO_OPTION_FRAME;      /* 赋值或初始化 */
    hfmpi2c->XferISR     = FMPI2C_Slave_ISR_DMA;        /* 赋值或初始化 */

    if (hfmpi2c->hdmarx != NULL)                        /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the FMPI2C DMA transfer complete callback */
      hfmpi2c->hdmarx->XferCpltCallback = FMPI2C_DMASlaveReceiveCplt;  /* 赋值或初始化 */

      /* Set the DMA error callback */
      hfmpi2c->hdmarx->XferErrorCallback = FMPI2C_DMAError;  /* 赋值或初始化 */

      /* Set the unused DMA callbacks to NULL */
      hfmpi2c->hdmarx->XferHalfCpltCallback = NULL;     /* 赋值或初始化 */
      hfmpi2c->hdmarx->XferAbortCallback = NULL;        /* 赋值或初始化 */

      /* Enable the DMA stream */
      dmaxferstatus = HAL_DMA_Start_IT(hfmpi2c->hdmarx, (uint32_t)&hfmpi2c->Instance->RXDR, (uint32_t)pData, hfmpi2c->XferSize);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Update FMPI2C state */
      hfmpi2c->State     = HAL_FMPI2C_STATE_LISTEN;     /* 赋值或初始化 */
      hfmpi2c->Mode      = HAL_FMPI2C_MODE_NONE;        /* 赋值或初始化 */

      /* Update FMPI2C error code */
      hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_DMA_PARAM;  /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hfmpi2c);                            /* HAL辅助宏/函数 */

      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    if (dmaxferstatus == HAL_OK)                        /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable Address Acknowledge */
      hfmpi2c->Instance->CR2 &= ~FMPI2C_CR2_NACK;       /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hfmpi2c);                            /* HAL辅助宏/函数 */

      /* Note : The FMPI2C interrupts must be enabled after unlocking current process
                to avoid the risk of FMPI2C interrupt handle execution before current
                process unlock */
      /* Enable ERR, STOP, NACK, ADDR interrupts */
      FMPI2C_Enable_IRQ(hfmpi2c, FMPI2C_XFER_LISTEN_IT);  /* 执行语句 */

      /* Enable DMA Request */
      hfmpi2c->Instance->CR1 |= FMPI2C_CR1_RXDMAEN;     /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Update FMPI2C state */
      hfmpi2c->State     = HAL_FMPI2C_STATE_LISTEN;     /* 赋值或初始化 */
      hfmpi2c->Mode      = HAL_FMPI2C_MODE_NONE;        /* 赋值或初始化 */

      /* Update FMPI2C error code */
      hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_DMA;       /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hfmpi2c);                            /* HAL辅助宏/函数 */

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
  * @brief  Write an amount of data in blocking mode to a specific memory address
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  MemAddress Internal memory address
  * @param  MemAddSize Size of internal memory address
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Mem_Write(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t DevAddress, uint16_t MemAddress,  /* HAL状态类型函数 */
                                    uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)  /* 16位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_FMPI2C_MEMADD_SIZE(MemAddSize));      /* 执行语句 */

  if (hfmpi2c->State == HAL_FMPI2C_STATE_READY)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) || (Size == 0U))                /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_INVALID_PARAM;  /* 赋值或初始化 */
      return  HAL_ERROR;                                /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hfmpi2c);                                /* HAL辅助宏/函数 */

    /* Init tickstart for timeout management*/
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */

    if (FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_BUSY, SET, FMPI2C_TIMEOUT_BUSY, tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    hfmpi2c->State     = HAL_FMPI2C_STATE_BUSY_TX;      /* 赋值或初始化 */
    hfmpi2c->Mode      = HAL_FMPI2C_MODE_MEM;           /* 赋值或初始化 */
    hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_NONE;         /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hfmpi2c->pBuffPtr  = pData;                         /* 赋值或初始化 */
    hfmpi2c->XferCount = Size;                          /* 赋值或初始化 */
    hfmpi2c->XferISR   = NULL;                          /* 赋值或初始化 */

    /* Send Slave Address and Memory Address */
    if (FMPI2C_RequestMemoryWrite(hfmpi2c, DevAddress, MemAddress, MemAddSize, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Process Unlocked */
      __HAL_UNLOCK(hfmpi2c);                            /* HAL辅助宏/函数 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Set NBYTES to write and reload if hfmpi2c->XferCount > MAX_NBYTE_SIZE */
    if (hfmpi2c->XferCount > MAX_NBYTE_SIZE)            /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpi2c->XferSize = MAX_NBYTE_SIZE;               /* 赋值或初始化 */
      FMPI2C_TransferConfig(hfmpi2c, DevAddress, (uint8_t)hfmpi2c->XferSize, FMPI2C_RELOAD_MODE, FMPI2C_NO_STARTSTOP);  /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hfmpi2c->XferSize = hfmpi2c->XferCount;           /* 赋值或初始化 */
      FMPI2C_TransferConfig(hfmpi2c, DevAddress, (uint8_t)hfmpi2c->XferSize, FMPI2C_AUTOEND_MODE, FMPI2C_NO_STARTSTOP);  /* 执行语句 */
    }                                                   /* 代码块结束 */

    do                                                  /* do-while循环 */
    {                                                   /* 代码块开始 */
      /* Wait until TXIS flag is set */
      if (FMPI2C_WaitOnTXISFlagUntilTimeout(hfmpi2c, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */

      /* Write data to TXDR */
      hfmpi2c->Instance->TXDR = *hfmpi2c->pBuffPtr;     /* 赋值或初始化 */

      /* Increment Buffer pointer */
      hfmpi2c->pBuffPtr++;                              /* 执行语句 */

      hfmpi2c->XferCount--;                             /* 执行语句 */
      hfmpi2c->XferSize--;                              /* 执行语句 */

      if ((hfmpi2c->XferCount != 0U) && (hfmpi2c->XferSize == 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Wait until TCR flag is set */
        if (FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_TCR, RESET, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
        {                                               /* 代码块开始 */
          return HAL_ERROR;                             /* 返回值 */
        }                                               /* 代码块结束 */

        if (hfmpi2c->XferCount > MAX_NBYTE_SIZE)        /* 条件判断 */
        {                                               /* 代码块开始 */
          hfmpi2c->XferSize = MAX_NBYTE_SIZE;           /* 赋值或初始化 */
          FMPI2C_TransferConfig(hfmpi2c, DevAddress, (uint8_t)hfmpi2c->XferSize, FMPI2C_RELOAD_MODE, FMPI2C_NO_STARTSTOP);  /* 执行语句 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          hfmpi2c->XferSize = hfmpi2c->XferCount;       /* 赋值或初始化 */
          FMPI2C_TransferConfig(hfmpi2c, DevAddress, (uint8_t)hfmpi2c->XferSize, FMPI2C_AUTOEND_MODE, FMPI2C_NO_STARTSTOP);  /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */

    } while (hfmpi2c->XferCount > 0U);                  /* 代码块结束 */

    /* No need to Check TC flag, with AUTOEND mode the stop is automatically generated */
    /* Wait until STOPF flag is reset */
    if (FMPI2C_WaitOnSTOPFlagUntilTimeout(hfmpi2c, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Clear STOP Flag */
    __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF);  /* HAL辅助宏/函数 */

    /* Clear Configuration Register 2 */
    FMPI2C_RESET_CR2(hfmpi2c);                          /* 执行语句 */

    hfmpi2c->State = HAL_FMPI2C_STATE_READY;            /* 赋值或初始化 */
    hfmpi2c->Mode  = HAL_FMPI2C_MODE_NONE;              /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c);                              /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Read an amount of data in blocking mode from a specific memory address
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  MemAddress Internal memory address
  * @param  MemAddSize Size of internal memory address
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Mem_Read(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t DevAddress, uint16_t MemAddress,  /* HAL状态类型函数 */
                                   uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)  /* 16位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_FMPI2C_MEMADD_SIZE(MemAddSize));      /* 执行语句 */

  if (hfmpi2c->State == HAL_FMPI2C_STATE_READY)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) || (Size == 0U))                /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_INVALID_PARAM;  /* 赋值或初始化 */
      return  HAL_ERROR;                                /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hfmpi2c);                                /* HAL辅助宏/函数 */

    /* Init tickstart for timeout management*/
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */

    if (FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_BUSY, SET, FMPI2C_TIMEOUT_BUSY, tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    hfmpi2c->State     = HAL_FMPI2C_STATE_BUSY_RX;      /* 赋值或初始化 */
    hfmpi2c->Mode      = HAL_FMPI2C_MODE_MEM;           /* 赋值或初始化 */
    hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_NONE;         /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hfmpi2c->pBuffPtr  = pData;                         /* 赋值或初始化 */
    hfmpi2c->XferCount = Size;                          /* 赋值或初始化 */
    hfmpi2c->XferISR   = NULL;                          /* 赋值或初始化 */

    /* Send Slave Address and Memory Address */
    if (FMPI2C_RequestMemoryRead(hfmpi2c, DevAddress, MemAddress, MemAddSize, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Process Unlocked */
      __HAL_UNLOCK(hfmpi2c);                            /* HAL辅助宏/函数 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Send Slave Address */
    /* Set NBYTES to write and reload if hfmpi2c->XferCount > MAX_NBYTE_SIZE and generate RESTART */
    if (hfmpi2c->XferCount > MAX_NBYTE_SIZE)            /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpi2c->XferSize = MAX_NBYTE_SIZE;               /* 赋值或初始化 */
      FMPI2C_TransferConfig(hfmpi2c, DevAddress, (uint8_t)hfmpi2c->XferSize, FMPI2C_RELOAD_MODE, FMPI2C_GENERATE_START_READ);  /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hfmpi2c->XferSize = hfmpi2c->XferCount;           /* 赋值或初始化 */
      FMPI2C_TransferConfig(hfmpi2c, DevAddress, (uint8_t)hfmpi2c->XferSize, FMPI2C_AUTOEND_MODE, FMPI2C_GENERATE_START_READ);  /* 执行语句 */
    }                                                   /* 代码块结束 */

    do                                                  /* do-while循环 */
    {                                                   /* 代码块开始 */
      /* Wait until RXNE flag is set */
      if (FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_RXNE, RESET, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */

      /* Read data from RXDR */
      *hfmpi2c->pBuffPtr = (uint8_t)hfmpi2c->Instance->RXDR;  /* 赋值或初始化 */

      /* Increment Buffer pointer */
      hfmpi2c->pBuffPtr++;                              /* 执行语句 */

      hfmpi2c->XferSize--;                              /* 执行语句 */
      hfmpi2c->XferCount--;                             /* 执行语句 */

      if ((hfmpi2c->XferCount != 0U) && (hfmpi2c->XferSize == 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Wait until TCR flag is set */
        if (FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_TCR, RESET, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
        {                                               /* 代码块开始 */
          return HAL_ERROR;                             /* 返回值 */
        }                                               /* 代码块结束 */

        if (hfmpi2c->XferCount > MAX_NBYTE_SIZE)        /* 条件判断 */
        {                                               /* 代码块开始 */
          hfmpi2c->XferSize = MAX_NBYTE_SIZE;           /* 赋值或初始化 */
          FMPI2C_TransferConfig(hfmpi2c, DevAddress, (uint8_t) hfmpi2c->XferSize, FMPI2C_RELOAD_MODE, FMPI2C_NO_STARTSTOP);  /* 执行语句 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          hfmpi2c->XferSize = hfmpi2c->XferCount;       /* 赋值或初始化 */
          FMPI2C_TransferConfig(hfmpi2c, DevAddress, (uint8_t)hfmpi2c->XferSize, FMPI2C_AUTOEND_MODE, FMPI2C_NO_STARTSTOP);  /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    } while (hfmpi2c->XferCount > 0U);                  /* 代码块结束 */

    /* No need to Check TC flag, with AUTOEND mode the stop is automatically generated */
    /* Wait until STOPF flag is reset */
    if (FMPI2C_WaitOnSTOPFlagUntilTimeout(hfmpi2c, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Clear STOP Flag */
    __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF);  /* HAL辅助宏/函数 */

    /* Clear Configuration Register 2 */
    FMPI2C_RESET_CR2(hfmpi2c);                          /* 执行语句 */

    hfmpi2c->State = HAL_FMPI2C_STATE_READY;            /* 赋值或初始化 */
    hfmpi2c->Mode  = HAL_FMPI2C_MODE_NONE;              /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c);                              /* HAL辅助宏/函数 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */
/**
  * @brief  Write an amount of data in non-blocking mode with Interrupt to a specific memory address
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  MemAddress Internal memory address
  * @param  MemAddSize Size of internal memory address
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Mem_Write_IT(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t DevAddress, uint16_t MemAddress,  /* HAL状态类型函数 */
                                       uint16_t MemAddSize, uint8_t *pData, uint16_t Size)  /* 16位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */
  uint32_t xfermode;                                    /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_FMPI2C_MEMADD_SIZE(MemAddSize));      /* 执行语句 */

  if (hfmpi2c->State == HAL_FMPI2C_STATE_READY)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) || (Size == 0U))                /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_INVALID_PARAM;  /* 赋值或初始化 */
      return  HAL_ERROR;                                /* 返回值 */
    }                                                   /* 代码块结束 */

    if (__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_BUSY) == SET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_BUSY;                                  /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hfmpi2c);                                /* HAL辅助宏/函数 */

    /* Init tickstart for timeout management*/
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */

    hfmpi2c->State       = HAL_FMPI2C_STATE_BUSY_TX;    /* 赋值或初始化 */
    hfmpi2c->Mode        = HAL_FMPI2C_MODE_MEM;         /* 赋值或初始化 */
    hfmpi2c->ErrorCode   = HAL_FMPI2C_ERROR_NONE;       /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hfmpi2c->pBuffPtr    = pData;                       /* 赋值或初始化 */
    hfmpi2c->XferCount   = Size;                        /* 赋值或初始化 */
    hfmpi2c->XferOptions = FMPI2C_NO_OPTION_FRAME;      /* 赋值或初始化 */
    hfmpi2c->XferISR     = FMPI2C_Master_ISR_IT;        /* 赋值或初始化 */

    if (hfmpi2c->XferCount > MAX_NBYTE_SIZE)            /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpi2c->XferSize = MAX_NBYTE_SIZE;               /* 赋值或初始化 */
      xfermode = FMPI2C_RELOAD_MODE;                    /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hfmpi2c->XferSize = hfmpi2c->XferCount;           /* 赋值或初始化 */
      xfermode = FMPI2C_AUTOEND_MODE;                   /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Send Slave Address and Memory Address */
    if (FMPI2C_RequestMemoryWrite(hfmpi2c, DevAddress, MemAddress, MemAddSize, FMPI2C_TIMEOUT_FLAG, tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Process Unlocked */
      __HAL_UNLOCK(hfmpi2c);                            /* HAL辅助宏/函数 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Set NBYTES to write and reload if hfmpi2c->XferCount > MAX_NBYTE_SIZE and generate RESTART */
    FMPI2C_TransferConfig(hfmpi2c, DevAddress, (uint8_t)hfmpi2c->XferSize, xfermode, FMPI2C_NO_STARTSTOP);  /* 执行语句 */

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c);                              /* HAL辅助宏/函数 */

    /* Note : The FMPI2C interrupts must be enabled after unlocking current process
              to avoid the risk of FMPI2C interrupt handle execution before current
              process unlock */

    /* Enable ERR, TC, STOP, NACK, TXI interrupt */
    /* possible to enable all of these */
    /* FMPI2C_IT_ERRI | FMPI2C_IT_TCI| FMPI2C_IT_STOPI| FMPI2C_IT_NACKI | FMPI2C_IT_ADDRI | FMPI2C_IT_RXI | FMPI2C_IT_TXI */
    FMPI2C_Enable_IRQ(hfmpi2c, FMPI2C_XFER_TX_IT);      /* 执行语句 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Read an amount of data in non-blocking mode with Interrupt from a specific memory address
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  MemAddress Internal memory address
  * @param  MemAddSize Size of internal memory address
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Mem_Read_IT(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t DevAddress, uint16_t MemAddress,  /* HAL状态类型函数 */
                                      uint16_t MemAddSize, uint8_t *pData, uint16_t Size)  /* 16位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */
  uint32_t xfermode;                                    /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_FMPI2C_MEMADD_SIZE(MemAddSize));      /* 执行语句 */

  if (hfmpi2c->State == HAL_FMPI2C_STATE_READY)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) || (Size == 0U))                /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_INVALID_PARAM;  /* 赋值或初始化 */
      return  HAL_ERROR;                                /* 返回值 */
    }                                                   /* 代码块结束 */

    if (__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_BUSY) == SET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_BUSY;                                  /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hfmpi2c);                                /* HAL辅助宏/函数 */

    /* Init tickstart for timeout management*/
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */

    hfmpi2c->State       = HAL_FMPI2C_STATE_BUSY_RX;    /* 赋值或初始化 */
    hfmpi2c->Mode        = HAL_FMPI2C_MODE_MEM;         /* 赋值或初始化 */
    hfmpi2c->ErrorCode   = HAL_FMPI2C_ERROR_NONE;       /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hfmpi2c->pBuffPtr    = pData;                       /* 赋值或初始化 */
    hfmpi2c->XferCount   = Size;                        /* 赋值或初始化 */
    hfmpi2c->XferOptions = FMPI2C_NO_OPTION_FRAME;      /* 赋值或初始化 */
    hfmpi2c->XferISR     = FMPI2C_Master_ISR_IT;        /* 赋值或初始化 */

    if (hfmpi2c->XferCount > MAX_NBYTE_SIZE)            /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpi2c->XferSize = MAX_NBYTE_SIZE;               /* 赋值或初始化 */
      xfermode = FMPI2C_RELOAD_MODE;                    /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hfmpi2c->XferSize = hfmpi2c->XferCount;           /* 赋值或初始化 */
      xfermode = FMPI2C_AUTOEND_MODE;                   /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Send Slave Address and Memory Address */
    if (FMPI2C_RequestMemoryRead(hfmpi2c, DevAddress, MemAddress, MemAddSize, FMPI2C_TIMEOUT_FLAG, tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Process Unlocked */
      __HAL_UNLOCK(hfmpi2c);                            /* HAL辅助宏/函数 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Set NBYTES to write and reload if hfmpi2c->XferCount > MAX_NBYTE_SIZE and generate RESTART */
    FMPI2C_TransferConfig(hfmpi2c, DevAddress, (uint8_t)hfmpi2c->XferSize, xfermode, FMPI2C_GENERATE_START_READ);  /* 执行语句 */

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c);                              /* HAL辅助宏/函数 */

    /* Note : The FMPI2C interrupts must be enabled after unlocking current process
              to avoid the risk of FMPI2C interrupt handle execution before current
              process unlock */

    /* Enable ERR, TC, STOP, NACK, RXI interrupt */
    /* possible to enable all of these */
    /* FMPI2C_IT_ERRI | FMPI2C_IT_TCI| FMPI2C_IT_STOPI| FMPI2C_IT_NACKI | FMPI2C_IT_ADDRI | FMPI2C_IT_RXI | FMPI2C_IT_TXI */
    FMPI2C_Enable_IRQ(hfmpi2c, FMPI2C_XFER_RX_IT);      /* 执行语句 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */
/**
  * @brief  Write an amount of data in non-blocking mode with DMA to a specific memory address
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  MemAddress Internal memory address
  * @param  MemAddSize Size of internal memory address
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Mem_Write_DMA(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t DevAddress, uint16_t MemAddress,  /* HAL状态类型函数 */
                                        uint16_t MemAddSize, uint8_t *pData, uint16_t Size)  /* 16位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */
  uint32_t xfermode;                                    /* 32位无符号变量/函数 */
  HAL_StatusTypeDef dmaxferstatus;                      /* HAL状态类型函数 */

  /* Check the parameters */
  assert_param(IS_FMPI2C_MEMADD_SIZE(MemAddSize));      /* 执行语句 */

  if (hfmpi2c->State == HAL_FMPI2C_STATE_READY)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) || (Size == 0U))                /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_INVALID_PARAM;  /* 赋值或初始化 */
      return  HAL_ERROR;                                /* 返回值 */
    }                                                   /* 代码块结束 */

    if (__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_BUSY) == SET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_BUSY;                                  /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hfmpi2c);                                /* HAL辅助宏/函数 */

    /* Init tickstart for timeout management*/
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */

    hfmpi2c->State       = HAL_FMPI2C_STATE_BUSY_TX;    /* 赋值或初始化 */
    hfmpi2c->Mode        = HAL_FMPI2C_MODE_MEM;         /* 赋值或初始化 */
    hfmpi2c->ErrorCode   = HAL_FMPI2C_ERROR_NONE;       /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hfmpi2c->pBuffPtr    = pData;                       /* 赋值或初始化 */
    hfmpi2c->XferCount   = Size;                        /* 赋值或初始化 */
    hfmpi2c->XferOptions = FMPI2C_NO_OPTION_FRAME;      /* 赋值或初始化 */
    hfmpi2c->XferISR     = FMPI2C_Master_ISR_DMA;       /* 赋值或初始化 */

    if (hfmpi2c->XferCount > MAX_NBYTE_SIZE)            /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpi2c->XferSize = MAX_NBYTE_SIZE;               /* 赋值或初始化 */
      xfermode = FMPI2C_RELOAD_MODE;                    /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hfmpi2c->XferSize = hfmpi2c->XferCount;           /* 赋值或初始化 */
      xfermode = FMPI2C_AUTOEND_MODE;                   /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Send Slave Address and Memory Address */
    if (FMPI2C_RequestMemoryWrite(hfmpi2c, DevAddress, MemAddress, MemAddSize, FMPI2C_TIMEOUT_FLAG, tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Process Unlocked */
      __HAL_UNLOCK(hfmpi2c);                            /* HAL辅助宏/函数 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */


    if (hfmpi2c->hdmatx != NULL)                        /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the FMPI2C DMA transfer complete callback */
      hfmpi2c->hdmatx->XferCpltCallback = FMPI2C_DMAMasterTransmitCplt;  /* 赋值或初始化 */

      /* Set the DMA error callback */
      hfmpi2c->hdmatx->XferErrorCallback = FMPI2C_DMAError;  /* 赋值或初始化 */

      /* Set the unused DMA callbacks to NULL */
      hfmpi2c->hdmatx->XferHalfCpltCallback = NULL;     /* 赋值或初始化 */
      hfmpi2c->hdmatx->XferAbortCallback = NULL;        /* 赋值或初始化 */

      /* Enable the DMA stream */
      dmaxferstatus = HAL_DMA_Start_IT(hfmpi2c->hdmatx, (uint32_t)pData, (uint32_t)&hfmpi2c->Instance->TXDR, hfmpi2c->XferSize);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Update FMPI2C state */
      hfmpi2c->State     = HAL_FMPI2C_STATE_READY;      /* 赋值或初始化 */
      hfmpi2c->Mode      = HAL_FMPI2C_MODE_NONE;        /* 赋值或初始化 */

      /* Update FMPI2C error code */
      hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_DMA_PARAM;  /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hfmpi2c);                            /* HAL辅助宏/函数 */

      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    if (dmaxferstatus == HAL_OK)                        /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Send Slave Address */
      /* Set NBYTES to write and reload if hfmpi2c->XferCount > MAX_NBYTE_SIZE and generate RESTART */
      FMPI2C_TransferConfig(hfmpi2c, DevAddress, (uint8_t)hfmpi2c->XferSize, xfermode, FMPI2C_NO_STARTSTOP);  /* 执行语句 */

      /* Update XferCount value */
      hfmpi2c->XferCount -= hfmpi2c->XferSize;          /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hfmpi2c);                            /* HAL辅助宏/函数 */

      /* Note : The FMPI2C interrupts must be enabled after unlocking current process
                to avoid the risk of FMPI2C interrupt handle execution before current
                process unlock */
      /* Enable ERR and NACK interrupts */
      FMPI2C_Enable_IRQ(hfmpi2c, FMPI2C_XFER_ERROR_IT);  /* 执行语句 */

      /* Enable DMA Request */
      hfmpi2c->Instance->CR1 |= FMPI2C_CR1_TXDMAEN;     /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Update FMPI2C state */
      hfmpi2c->State     = HAL_FMPI2C_STATE_READY;      /* 赋值或初始化 */
      hfmpi2c->Mode      = HAL_FMPI2C_MODE_NONE;        /* 赋值或初始化 */

      /* Update FMPI2C error code */
      hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_DMA;       /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hfmpi2c);                            /* HAL辅助宏/函数 */

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
  * @brief  Reads an amount of data in non-blocking mode with DMA from a specific memory address.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  MemAddress Internal memory address
  * @param  MemAddSize Size of internal memory address
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be read
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Mem_Read_DMA(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t DevAddress, uint16_t MemAddress,  /* HAL状态类型函数 */
                                       uint16_t MemAddSize, uint8_t *pData, uint16_t Size)  /* 16位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */
  uint32_t xfermode;                                    /* 32位无符号变量/函数 */
  HAL_StatusTypeDef dmaxferstatus;                      /* HAL状态类型函数 */

  /* Check the parameters */
  assert_param(IS_FMPI2C_MEMADD_SIZE(MemAddSize));      /* 执行语句 */

  if (hfmpi2c->State == HAL_FMPI2C_STATE_READY)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) || (Size == 0U))                /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_INVALID_PARAM;  /* 赋值或初始化 */
      return  HAL_ERROR;                                /* 返回值 */
    }                                                   /* 代码块结束 */

    if (__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_BUSY) == SET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_BUSY;                                  /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hfmpi2c);                                /* HAL辅助宏/函数 */

    /* Init tickstart for timeout management*/
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */

    hfmpi2c->State       = HAL_FMPI2C_STATE_BUSY_RX;    /* 赋值或初始化 */
    hfmpi2c->Mode        = HAL_FMPI2C_MODE_MEM;         /* 赋值或初始化 */
    hfmpi2c->ErrorCode   = HAL_FMPI2C_ERROR_NONE;       /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hfmpi2c->pBuffPtr    = pData;                       /* 赋值或初始化 */
    hfmpi2c->XferCount   = Size;                        /* 赋值或初始化 */
    hfmpi2c->XferOptions = FMPI2C_NO_OPTION_FRAME;      /* 赋值或初始化 */
    hfmpi2c->XferISR     = FMPI2C_Master_ISR_DMA;       /* 赋值或初始化 */

    if (hfmpi2c->XferCount > MAX_NBYTE_SIZE)            /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpi2c->XferSize = MAX_NBYTE_SIZE;               /* 赋值或初始化 */
      xfermode = FMPI2C_RELOAD_MODE;                    /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hfmpi2c->XferSize = hfmpi2c->XferCount;           /* 赋值或初始化 */
      xfermode = FMPI2C_AUTOEND_MODE;                   /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Send Slave Address and Memory Address */
    if (FMPI2C_RequestMemoryRead(hfmpi2c, DevAddress, MemAddress, MemAddSize, FMPI2C_TIMEOUT_FLAG, tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Process Unlocked */
      __HAL_UNLOCK(hfmpi2c);                            /* HAL辅助宏/函数 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    if (hfmpi2c->hdmarx != NULL)                        /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the FMPI2C DMA transfer complete callback */
      hfmpi2c->hdmarx->XferCpltCallback = FMPI2C_DMAMasterReceiveCplt;  /* 赋值或初始化 */

      /* Set the DMA error callback */
      hfmpi2c->hdmarx->XferErrorCallback = FMPI2C_DMAError;  /* 赋值或初始化 */

      /* Set the unused DMA callbacks to NULL */
      hfmpi2c->hdmarx->XferHalfCpltCallback = NULL;     /* 赋值或初始化 */
      hfmpi2c->hdmarx->XferAbortCallback = NULL;        /* 赋值或初始化 */

      /* Enable the DMA stream */
      dmaxferstatus = HAL_DMA_Start_IT(hfmpi2c->hdmarx, (uint32_t)&hfmpi2c->Instance->RXDR, (uint32_t)pData, hfmpi2c->XferSize);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Update FMPI2C state */
      hfmpi2c->State     = HAL_FMPI2C_STATE_READY;      /* 赋值或初始化 */
      hfmpi2c->Mode      = HAL_FMPI2C_MODE_NONE;        /* 赋值或初始化 */

      /* Update FMPI2C error code */
      hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_DMA_PARAM;  /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hfmpi2c);                            /* HAL辅助宏/函数 */

      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    if (dmaxferstatus == HAL_OK)                        /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set NBYTES to write and reload if hfmpi2c->XferCount > MAX_NBYTE_SIZE and generate RESTART */
      FMPI2C_TransferConfig(hfmpi2c, DevAddress, (uint8_t)hfmpi2c->XferSize, xfermode, FMPI2C_GENERATE_START_READ);  /* 执行语句 */

      /* Update XferCount value */
      hfmpi2c->XferCount -= hfmpi2c->XferSize;          /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hfmpi2c);                            /* HAL辅助宏/函数 */

      /* Note : The FMPI2C interrupts must be enabled after unlocking current process
                to avoid the risk of FMPI2C interrupt handle execution before current
                process unlock */
      /* Enable ERR and NACK interrupts */
      FMPI2C_Enable_IRQ(hfmpi2c, FMPI2C_XFER_ERROR_IT);  /* 执行语句 */

      /* Enable DMA Request */
      hfmpi2c->Instance->CR1 |= FMPI2C_CR1_RXDMAEN;     /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Update FMPI2C state */
      hfmpi2c->State     = HAL_FMPI2C_STATE_READY;      /* 赋值或初始化 */
      hfmpi2c->Mode      = HAL_FMPI2C_MODE_NONE;        /* 赋值或初始化 */

      /* Update FMPI2C error code */
      hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_DMA;       /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hfmpi2c);                            /* HAL辅助宏/函数 */

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
  * @brief  Checks if target device is ready for communication.
  * @note   This function is used with Memory devices
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  Trials Number of trials
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_IsDeviceReady(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t DevAddress, uint32_t Trials, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */

  __IO uint32_t FMPI2C_Trials = 0UL;                    /* 赋值或初始化 */

  FlagStatus tmp1;                                      /* 执行语句 */
  FlagStatus tmp2;                                      /* 执行语句 */

  if (hfmpi2c->State == HAL_FMPI2C_STATE_READY)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_BUSY) == SET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_BUSY;                                  /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hfmpi2c);                                /* HAL辅助宏/函数 */

    hfmpi2c->State = HAL_FMPI2C_STATE_BUSY;             /* 赋值或初始化 */
    hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_NONE;         /* 赋值或初始化 */

    do                                                  /* do-while循环 */
    {                                                   /* 代码块开始 */
      /* Generate Start */
      hfmpi2c->Instance->CR2 = FMPI2C_GENERATE_START(hfmpi2c->Init.AddressingMode, DevAddress);  /* 赋值或初始化 */

      /* No need to Check TC flag, with AUTOEND mode the stop is automatically generated */
      /* Wait until STOPF flag is set or a NACK flag is set*/
      tickstart = HAL_GetTick();                        /* 赋值或初始化 */

      tmp1 = __HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF);  /* 赋值或初始化 */
      tmp2 = __HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_AF);  /* 赋值或初始化 */

      while ((tmp1 == RESET) && (tmp2 == RESET))        /* while循环 */
      {                                                 /* 代码块开始 */
        if (Timeout != HAL_MAX_DELAY)                   /* 条件判断 */
        {                                               /* 代码块开始 */
          if (((HAL_GetTick() - tickstart) > Timeout) || (Timeout == 0U))  /* 条件判断 */
          {                                             /* 代码块开始 */
            /* Update FMPI2C state */
            hfmpi2c->State = HAL_FMPI2C_STATE_READY;    /* 赋值或初始化 */

            /* Update FMPI2C error code */
            hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_TIMEOUT;  /* 赋值或初始化 */

            /* Process Unlocked */
            __HAL_UNLOCK(hfmpi2c);                      /* HAL辅助宏/函数 */

            return HAL_ERROR;                           /* 返回值 */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */

        tmp1 = __HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF);  /* 赋值或初始化 */
        tmp2 = __HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_AF);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */

      /* Check if the NACKF flag has not been set */
      if (__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_AF) == RESET)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Wait until STOPF flag is reset */
        if (FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_STOPF, RESET, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
        {                                               /* 代码块开始 */
          return HAL_ERROR;                             /* 返回值 */
        }                                               /* 代码块结束 */

        /* Clear STOP Flag */
        __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF);  /* HAL辅助宏/函数 */

        /* Device is ready */
        hfmpi2c->State = HAL_FMPI2C_STATE_READY;        /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hfmpi2c);                          /* HAL辅助宏/函数 */

        return HAL_OK;                                  /* 返回值 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Wait until STOPF flag is reset */
        if (FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_STOPF, RESET, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
        {                                               /* 代码块开始 */
          return HAL_ERROR;                             /* 返回值 */
        }                                               /* 代码块结束 */

        /* Clear NACK Flag */
        __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_AF);  /* HAL辅助宏/函数 */

        /* Clear STOP Flag, auto generated with autoend*/
        __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF);  /* HAL辅助宏/函数 */
      }                                                 /* 代码块结束 */

      /* Check if the maximum allowed number of trials has been reached */
      if (FMPI2C_Trials == Trials)                      /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Generate Stop */
        hfmpi2c->Instance->CR2 |= FMPI2C_CR2_STOP;      /* 赋值或初始化 */

        /* Wait until STOPF flag is reset */
        if (FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_STOPF, RESET, Timeout, tickstart) != HAL_OK)  /* 条件判断 */
        {                                               /* 代码块开始 */
          return HAL_ERROR;                             /* 返回值 */
        }                                               /* 代码块结束 */

        /* Clear STOP Flag */
        __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF);  /* HAL辅助宏/函数 */
      }                                                 /* 代码块结束 */

      /* Increment Trials */
      FMPI2C_Trials++;                                  /* 执行语句 */
    } while (FMPI2C_Trials < Trials);                   /* 代码块结束 */

    /* Update FMPI2C state */
    hfmpi2c->State = HAL_FMPI2C_STATE_READY;            /* 赋值或初始化 */

    /* Update FMPI2C error code */
    hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_TIMEOUT;     /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c);                              /* HAL辅助宏/函数 */

    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Sequential transmit in master FMPI2C mode an amount of data in non-blocking mode with Interrupt.
  * @note   This interface allow to manage repeated start condition when a direction change during transfer
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  XferOptions Options of Transfer, value of @ref FMPI2C_XFEROPTIONS
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Master_Seq_Transmit_IT(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t DevAddress, uint8_t *pData,  /* HAL状态类型函数 */
                                                 uint16_t Size, uint32_t XferOptions)  /* 16位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t xfermode;                                    /* 32位无符号变量/函数 */
  uint32_t xferrequest = FMPI2C_GENERATE_START_WRITE;   /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_FMPI2C_TRANSFER_OPTIONS_REQUEST(XferOptions));  /* 执行语句 */

  if (hfmpi2c->State == HAL_FMPI2C_STATE_READY)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Process Locked */
    __HAL_LOCK(hfmpi2c);                                /* HAL辅助宏/函数 */

    hfmpi2c->State     = HAL_FMPI2C_STATE_BUSY_TX;      /* 赋值或初始化 */
    hfmpi2c->Mode      = HAL_FMPI2C_MODE_MASTER;        /* 赋值或初始化 */
    hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_NONE;         /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hfmpi2c->pBuffPtr    = pData;                       /* 赋值或初始化 */
    hfmpi2c->XferCount   = Size;                        /* 赋值或初始化 */
    hfmpi2c->XferOptions = XferOptions;                 /* 赋值或初始化 */
    hfmpi2c->XferISR     = FMPI2C_Master_ISR_IT;        /* 赋值或初始化 */

    /* If hfmpi2c->XferCount > MAX_NBYTE_SIZE, use reload mode */
    if (hfmpi2c->XferCount > MAX_NBYTE_SIZE)            /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpi2c->XferSize = MAX_NBYTE_SIZE;               /* 赋值或初始化 */
      xfermode = FMPI2C_RELOAD_MODE;                    /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hfmpi2c->XferSize = hfmpi2c->XferCount;           /* 赋值或初始化 */
      xfermode = hfmpi2c->XferOptions;                  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* If transfer direction not change and there is no request to start another frame, do not generate Restart Condition */
    /* Mean Previous state is same as current state */
    if ((hfmpi2c->PreviousState == FMPI2C_STATE_MASTER_BUSY_TX) && (IS_FMPI2C_TRANSFER_OTHER_OPTIONS_REQUEST(XferOptions) == 0))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      xferrequest = FMPI2C_NO_STARTSTOP;                /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Convert OTHER_xxx XferOptions if any */
      FMPI2C_ConvertOtherXferOptions(hfmpi2c);          /* 执行语句 */

      /* Update xfermode accordingly if no reload is necessary */
      if (hfmpi2c->XferCount <= MAX_NBYTE_SIZE)         /* 条件判断 */
      {                                                 /* 代码块开始 */
        xfermode = hfmpi2c->XferOptions;                /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Send Slave Address and set NBYTES to write */
    FMPI2C_TransferConfig(hfmpi2c, DevAddress, (uint8_t)hfmpi2c->XferSize, xfermode, xferrequest);  /* 执行语句 */

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c);                              /* HAL辅助宏/函数 */

    /* Note : The FMPI2C interrupts must be enabled after unlocking current process
              to avoid the risk of FMPI2C interrupt handle execution before current
              process unlock */
    FMPI2C_Enable_IRQ(hfmpi2c, FMPI2C_XFER_TX_IT);      /* 执行语句 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Sequential transmit in master FMPI2C mode an amount of data in non-blocking mode with DMA.
  * @note   This interface allow to manage repeated start condition when a direction change during transfer
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  XferOptions Options of Transfer, value of @ref FMPI2C_XFEROPTIONS
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Master_Seq_Transmit_DMA(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t DevAddress, uint8_t *pData,  /* HAL状态类型函数 */
                                                  uint16_t Size, uint32_t XferOptions)  /* 16位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t xfermode;                                    /* 32位无符号变量/函数 */
  uint32_t xferrequest = FMPI2C_GENERATE_START_WRITE;   /* 32位无符号变量/函数 */
  HAL_StatusTypeDef dmaxferstatus;                      /* HAL状态类型函数 */

  /* Check the parameters */
  assert_param(IS_FMPI2C_TRANSFER_OPTIONS_REQUEST(XferOptions));  /* 执行语句 */

  if (hfmpi2c->State == HAL_FMPI2C_STATE_READY)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Process Locked */
    __HAL_LOCK(hfmpi2c);                                /* HAL辅助宏/函数 */

    hfmpi2c->State     = HAL_FMPI2C_STATE_BUSY_TX;      /* 赋值或初始化 */
    hfmpi2c->Mode      = HAL_FMPI2C_MODE_MASTER;        /* 赋值或初始化 */
    hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_NONE;         /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hfmpi2c->pBuffPtr    = pData;                       /* 赋值或初始化 */
    hfmpi2c->XferCount   = Size;                        /* 赋值或初始化 */
    hfmpi2c->XferOptions = XferOptions;                 /* 赋值或初始化 */
    hfmpi2c->XferISR     = FMPI2C_Master_ISR_DMA;       /* 赋值或初始化 */

    /* If hfmpi2c->XferCount > MAX_NBYTE_SIZE, use reload mode */
    if (hfmpi2c->XferCount > MAX_NBYTE_SIZE)            /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpi2c->XferSize = MAX_NBYTE_SIZE;               /* 赋值或初始化 */
      xfermode = FMPI2C_RELOAD_MODE;                    /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hfmpi2c->XferSize = hfmpi2c->XferCount;           /* 赋值或初始化 */
      xfermode = hfmpi2c->XferOptions;                  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* If transfer direction not change and there is no request to start another frame, do not generate Restart Condition */
    /* Mean Previous state is same as current state */
    if ((hfmpi2c->PreviousState == FMPI2C_STATE_MASTER_BUSY_TX) && (IS_FMPI2C_TRANSFER_OTHER_OPTIONS_REQUEST(XferOptions) == 0))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      xferrequest = FMPI2C_NO_STARTSTOP;                /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Convert OTHER_xxx XferOptions if any */
      FMPI2C_ConvertOtherXferOptions(hfmpi2c);          /* 执行语句 */

      /* Update xfermode accordingly if no reload is necessary */
      if (hfmpi2c->XferCount <= MAX_NBYTE_SIZE)         /* 条件判断 */
      {                                                 /* 代码块开始 */
        xfermode = hfmpi2c->XferOptions;                /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    if (hfmpi2c->XferSize > 0U)                         /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (hfmpi2c->hdmatx != NULL)                      /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Set the FMPI2C DMA transfer complete callback */
        hfmpi2c->hdmatx->XferCpltCallback = FMPI2C_DMAMasterTransmitCplt;  /* 赋值或初始化 */

        /* Set the DMA error callback */
        hfmpi2c->hdmatx->XferErrorCallback = FMPI2C_DMAError;  /* 赋值或初始化 */

        /* Set the unused DMA callbacks to NULL */
        hfmpi2c->hdmatx->XferHalfCpltCallback = NULL;   /* 赋值或初始化 */
        hfmpi2c->hdmatx->XferAbortCallback = NULL;      /* 赋值或初始化 */

        /* Enable the DMA stream */
        dmaxferstatus = HAL_DMA_Start_IT(hfmpi2c->hdmatx, (uint32_t)pData, (uint32_t)&hfmpi2c->Instance->TXDR, hfmpi2c->XferSize);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Update FMPI2C state */
        hfmpi2c->State     = HAL_FMPI2C_STATE_READY;    /* 赋值或初始化 */
        hfmpi2c->Mode      = HAL_FMPI2C_MODE_NONE;      /* 赋值或初始化 */

        /* Update FMPI2C error code */
        hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_DMA_PARAM;  /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hfmpi2c);                          /* HAL辅助宏/函数 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */

      if (dmaxferstatus == HAL_OK)                      /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Send Slave Address and set NBYTES to write */
        FMPI2C_TransferConfig(hfmpi2c, DevAddress, (uint8_t)hfmpi2c->XferSize, xfermode, xferrequest);  /* 执行语句 */

        /* Update XferCount value */
        hfmpi2c->XferCount -= hfmpi2c->XferSize;        /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hfmpi2c);                          /* HAL辅助宏/函数 */

        /* Note : The FMPI2C interrupts must be enabled after unlocking current process
                  to avoid the risk of FMPI2C interrupt handle execution before current
                  process unlock */
        /* Enable ERR and NACK interrupts */
        FMPI2C_Enable_IRQ(hfmpi2c, FMPI2C_XFER_ERROR_IT);  /* 执行语句 */

        /* Enable DMA Request */
        hfmpi2c->Instance->CR1 |= FMPI2C_CR1_TXDMAEN;   /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Update FMPI2C state */
        hfmpi2c->State     = HAL_FMPI2C_STATE_READY;    /* 赋值或初始化 */
        hfmpi2c->Mode      = HAL_FMPI2C_MODE_NONE;      /* 赋值或初始化 */

        /* Update FMPI2C error code */
        hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_DMA;     /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hfmpi2c);                          /* HAL辅助宏/函数 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Update Transfer ISR function pointer */
      hfmpi2c->XferISR = FMPI2C_Master_ISR_IT;          /* 赋值或初始化 */

      /* Send Slave Address */
      /* Set NBYTES to write and generate START condition */
      FMPI2C_TransferConfig(hfmpi2c, DevAddress, (uint8_t)hfmpi2c->XferSize, FMPI2C_AUTOEND_MODE, FMPI2C_GENERATE_START_WRITE);  /* 执行语句 */

      /* Process Unlocked */
      __HAL_UNLOCK(hfmpi2c);                            /* HAL辅助宏/函数 */

      /* Note : The FMPI2C interrupts must be enabled after unlocking current process
                to avoid the risk of FMPI2C interrupt handle execution before current
                process unlock */
      /* Enable ERR, TC, STOP, NACK, TXI interrupt */
      /* possible to enable all of these */
      /* FMPI2C_IT_ERRI | FMPI2C_IT_TCI| FMPI2C_IT_STOPI| FMPI2C_IT_NACKI | FMPI2C_IT_ADDRI | FMPI2C_IT_RXI | FMPI2C_IT_TXI */
      FMPI2C_Enable_IRQ(hfmpi2c, FMPI2C_XFER_TX_IT);    /* 执行语句 */
    }                                                   /* 代码块结束 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Sequential receive in master FMPI2C mode an amount of data in non-blocking mode with Interrupt
  * @note   This interface allow to manage repeated start condition when a direction change during transfer
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  XferOptions Options of Transfer, value of @ref FMPI2C_XFEROPTIONS
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Master_Seq_Receive_IT(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t DevAddress, uint8_t *pData,  /* HAL状态类型函数 */
                                                uint16_t Size, uint32_t XferOptions)  /* 16位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t xfermode;                                    /* 32位无符号变量/函数 */
  uint32_t xferrequest = FMPI2C_GENERATE_START_READ;    /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_FMPI2C_TRANSFER_OPTIONS_REQUEST(XferOptions));  /* 执行语句 */

  if (hfmpi2c->State == HAL_FMPI2C_STATE_READY)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Process Locked */
    __HAL_LOCK(hfmpi2c);                                /* HAL辅助宏/函数 */

    hfmpi2c->State     = HAL_FMPI2C_STATE_BUSY_RX;      /* 赋值或初始化 */
    hfmpi2c->Mode      = HAL_FMPI2C_MODE_MASTER;        /* 赋值或初始化 */
    hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_NONE;         /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hfmpi2c->pBuffPtr    = pData;                       /* 赋值或初始化 */
    hfmpi2c->XferCount   = Size;                        /* 赋值或初始化 */
    hfmpi2c->XferOptions = XferOptions;                 /* 赋值或初始化 */
    hfmpi2c->XferISR     = FMPI2C_Master_ISR_IT;        /* 赋值或初始化 */

    /* If hfmpi2c->XferCount > MAX_NBYTE_SIZE, use reload mode */
    if (hfmpi2c->XferCount > MAX_NBYTE_SIZE)            /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpi2c->XferSize = MAX_NBYTE_SIZE;               /* 赋值或初始化 */
      xfermode = FMPI2C_RELOAD_MODE;                    /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hfmpi2c->XferSize = hfmpi2c->XferCount;           /* 赋值或初始化 */
      xfermode = hfmpi2c->XferOptions;                  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* If transfer direction not change and there is no request to start another frame, do not generate Restart Condition */
    /* Mean Previous state is same as current state */
    if ((hfmpi2c->PreviousState == FMPI2C_STATE_MASTER_BUSY_RX) && (IS_FMPI2C_TRANSFER_OTHER_OPTIONS_REQUEST(XferOptions) == 0))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      xferrequest = FMPI2C_NO_STARTSTOP;                /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Convert OTHER_xxx XferOptions if any */
      FMPI2C_ConvertOtherXferOptions(hfmpi2c);          /* 执行语句 */

      /* Update xfermode accordingly if no reload is necessary */
      if (hfmpi2c->XferCount <= MAX_NBYTE_SIZE)         /* 条件判断 */
      {                                                 /* 代码块开始 */
        xfermode = hfmpi2c->XferOptions;                /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Send Slave Address and set NBYTES to read */
    FMPI2C_TransferConfig(hfmpi2c, DevAddress, (uint8_t)hfmpi2c->XferSize, xfermode, xferrequest);  /* 执行语句 */

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c);                              /* HAL辅助宏/函数 */

    /* Note : The FMPI2C interrupts must be enabled after unlocking current process
              to avoid the risk of FMPI2C interrupt handle execution before current
              process unlock */
    FMPI2C_Enable_IRQ(hfmpi2c, FMPI2C_XFER_RX_IT);      /* 执行语句 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Sequential receive in master FMPI2C mode an amount of data in non-blocking mode with DMA
  * @note   This interface allow to manage repeated start condition when a direction change during transfer
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  XferOptions Options of Transfer, value of @ref FMPI2C_XFEROPTIONS
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Master_Seq_Receive_DMA(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t DevAddress, uint8_t *pData,  /* HAL状态类型函数 */
                                                 uint16_t Size, uint32_t XferOptions)  /* 16位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t xfermode;                                    /* 32位无符号变量/函数 */
  uint32_t xferrequest = FMPI2C_GENERATE_START_READ;    /* 32位无符号变量/函数 */
  HAL_StatusTypeDef dmaxferstatus;                      /* HAL状态类型函数 */

  /* Check the parameters */
  assert_param(IS_FMPI2C_TRANSFER_OPTIONS_REQUEST(XferOptions));  /* 执行语句 */

  if (hfmpi2c->State == HAL_FMPI2C_STATE_READY)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Process Locked */
    __HAL_LOCK(hfmpi2c);                                /* HAL辅助宏/函数 */

    hfmpi2c->State     = HAL_FMPI2C_STATE_BUSY_RX;      /* 赋值或初始化 */
    hfmpi2c->Mode      = HAL_FMPI2C_MODE_MASTER;        /* 赋值或初始化 */
    hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_NONE;         /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hfmpi2c->pBuffPtr    = pData;                       /* 赋值或初始化 */
    hfmpi2c->XferCount   = Size;                        /* 赋值或初始化 */
    hfmpi2c->XferOptions = XferOptions;                 /* 赋值或初始化 */
    hfmpi2c->XferISR     = FMPI2C_Master_ISR_DMA;       /* 赋值或初始化 */

    /* If hfmpi2c->XferCount > MAX_NBYTE_SIZE, use reload mode */
    if (hfmpi2c->XferCount > MAX_NBYTE_SIZE)            /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpi2c->XferSize = MAX_NBYTE_SIZE;               /* 赋值或初始化 */
      xfermode = FMPI2C_RELOAD_MODE;                    /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hfmpi2c->XferSize = hfmpi2c->XferCount;           /* 赋值或初始化 */
      xfermode = hfmpi2c->XferOptions;                  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* If transfer direction not change and there is no request to start another frame, do not generate Restart Condition */
    /* Mean Previous state is same as current state */
    if ((hfmpi2c->PreviousState == FMPI2C_STATE_MASTER_BUSY_RX) && (IS_FMPI2C_TRANSFER_OTHER_OPTIONS_REQUEST(XferOptions) == 0))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      xferrequest = FMPI2C_NO_STARTSTOP;                /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Convert OTHER_xxx XferOptions if any */
      FMPI2C_ConvertOtherXferOptions(hfmpi2c);          /* 执行语句 */

      /* Update xfermode accordingly if no reload is necessary */
      if (hfmpi2c->XferCount <= MAX_NBYTE_SIZE)         /* 条件判断 */
      {                                                 /* 代码块开始 */
        xfermode = hfmpi2c->XferOptions;                /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    if (hfmpi2c->XferSize > 0U)                         /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (hfmpi2c->hdmarx != NULL)                      /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Set the FMPI2C DMA transfer complete callback */
        hfmpi2c->hdmarx->XferCpltCallback = FMPI2C_DMAMasterReceiveCplt;  /* 赋值或初始化 */

        /* Set the DMA error callback */
        hfmpi2c->hdmarx->XferErrorCallback = FMPI2C_DMAError;  /* 赋值或初始化 */

        /* Set the unused DMA callbacks to NULL */
        hfmpi2c->hdmarx->XferHalfCpltCallback = NULL;   /* 赋值或初始化 */
        hfmpi2c->hdmarx->XferAbortCallback = NULL;      /* 赋值或初始化 */

        /* Enable the DMA stream */
        dmaxferstatus = HAL_DMA_Start_IT(hfmpi2c->hdmarx, (uint32_t)&hfmpi2c->Instance->RXDR, (uint32_t)pData, hfmpi2c->XferSize);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Update FMPI2C state */
        hfmpi2c->State     = HAL_FMPI2C_STATE_READY;    /* 赋值或初始化 */
        hfmpi2c->Mode      = HAL_FMPI2C_MODE_NONE;      /* 赋值或初始化 */

        /* Update FMPI2C error code */
        hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_DMA_PARAM;  /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hfmpi2c);                          /* HAL辅助宏/函数 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */

      if (dmaxferstatus == HAL_OK)                      /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Send Slave Address and set NBYTES to read */
        FMPI2C_TransferConfig(hfmpi2c, DevAddress, (uint8_t)hfmpi2c->XferSize, xfermode, xferrequest);  /* 执行语句 */

        /* Update XferCount value */
        hfmpi2c->XferCount -= hfmpi2c->XferSize;        /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hfmpi2c);                          /* HAL辅助宏/函数 */

        /* Note : The FMPI2C interrupts must be enabled after unlocking current process
                  to avoid the risk of FMPI2C interrupt handle execution before current
                  process unlock */
        /* Enable ERR and NACK interrupts */
        FMPI2C_Enable_IRQ(hfmpi2c, FMPI2C_XFER_ERROR_IT);  /* 执行语句 */

        /* Enable DMA Request */
        hfmpi2c->Instance->CR1 |= FMPI2C_CR1_RXDMAEN;   /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Update FMPI2C state */
        hfmpi2c->State     = HAL_FMPI2C_STATE_READY;    /* 赋值或初始化 */
        hfmpi2c->Mode      = HAL_FMPI2C_MODE_NONE;      /* 赋值或初始化 */

        /* Update FMPI2C error code */
        hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_DMA;     /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hfmpi2c);                          /* HAL辅助宏/函数 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Update Transfer ISR function pointer */
      hfmpi2c->XferISR = FMPI2C_Master_ISR_IT;          /* 赋值或初始化 */

      /* Send Slave Address */
      /* Set NBYTES to read and generate START condition */
      FMPI2C_TransferConfig(hfmpi2c, DevAddress, (uint8_t)hfmpi2c->XferSize, FMPI2C_AUTOEND_MODE, FMPI2C_GENERATE_START_READ);  /* 执行语句 */

      /* Process Unlocked */
      __HAL_UNLOCK(hfmpi2c);                            /* HAL辅助宏/函数 */

      /* Note : The FMPI2C interrupts must be enabled after unlocking current process
                to avoid the risk of FMPI2C interrupt handle execution before current
                process unlock */
      /* Enable ERR, TC, STOP, NACK, TXI interrupt */
      /* possible to enable all of these */
      /* FMPI2C_IT_ERRI | FMPI2C_IT_TCI| FMPI2C_IT_STOPI| FMPI2C_IT_NACKI | FMPI2C_IT_ADDRI | FMPI2C_IT_RXI | FMPI2C_IT_TXI */
      FMPI2C_Enable_IRQ(hfmpi2c, FMPI2C_XFER_TX_IT);    /* 执行语句 */
    }                                                   /* 代码块结束 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Sequential transmit in slave/device FMPI2C mode an amount of data in non-blocking mode with Interrupt
  * @note   This interface allow to manage repeated start condition when a direction change during transfer
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  XferOptions Options of Transfer, value of @ref FMPI2C_XFEROPTIONS
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Slave_Seq_Transmit_IT(FMPI2C_HandleTypeDef *hfmpi2c, uint8_t *pData, uint16_t Size,  /* HAL状态类型函数 */
                                                uint32_t XferOptions)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_FMPI2C_TRANSFER_OPTIONS_REQUEST(XferOptions));  /* 执行语句 */

  if (((uint32_t)hfmpi2c->State & (uint32_t)HAL_FMPI2C_STATE_LISTEN) == (uint32_t)HAL_FMPI2C_STATE_LISTEN)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) || (Size == 0U))                /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_INVALID_PARAM;  /* 赋值或初始化 */
      return  HAL_ERROR;                                /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Disable Interrupts, to prevent preemption during treatment in case of multicall */
    FMPI2C_Disable_IRQ(hfmpi2c, FMPI2C_XFER_LISTEN_IT | FMPI2C_XFER_TX_IT);  /* 执行语句 */

    /* Process Locked */
    __HAL_LOCK(hfmpi2c);                                /* HAL辅助宏/函数 */

    /* FMPI2C cannot manage full duplex exchange so disable previous IT enabled if any */
    /* and then toggle the HAL slave RX state to TX state */
    if (hfmpi2c->State == HAL_FMPI2C_STATE_BUSY_RX_LISTEN)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable associated Interrupts */
      FMPI2C_Disable_IRQ(hfmpi2c, FMPI2C_XFER_RX_IT);   /* 执行语句 */

      /* Abort DMA Xfer if any */
      if ((hfmpi2c->Instance->CR1 & FMPI2C_CR1_RXDMAEN) == FMPI2C_CR1_RXDMAEN)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        hfmpi2c->Instance->CR1 &= ~FMPI2C_CR1_RXDMAEN;  /* 赋值或初始化 */

        if (hfmpi2c->hdmarx != NULL)                    /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Set the FMPI2C DMA Abort callback :
           will lead to call HAL_FMPI2C_ErrorCallback() at end of DMA abort procedure */
          hfmpi2c->hdmarx->XferAbortCallback = FMPI2C_DMAAbort;  /* 赋值或初始化 */

          /* Abort DMA RX */
          if (HAL_DMA_Abort_IT(hfmpi2c->hdmarx) != HAL_OK)  /* 条件判断 */
          {                                             /* 代码块开始 */
            /* Call Directly XferAbortCallback function in case of error */
            hfmpi2c->hdmarx->XferAbortCallback(hfmpi2c->hdmarx);  /* 执行语句 */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    hfmpi2c->State     = HAL_FMPI2C_STATE_BUSY_TX_LISTEN;  /* 赋值或初始化 */
    hfmpi2c->Mode      = HAL_FMPI2C_MODE_SLAVE;         /* 赋值或初始化 */
    hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_NONE;         /* 赋值或初始化 */

    /* Enable Address Acknowledge */
    hfmpi2c->Instance->CR2 &= ~FMPI2C_CR2_NACK;         /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hfmpi2c->pBuffPtr    = pData;                       /* 赋值或初始化 */
    hfmpi2c->XferCount   = Size;                        /* 赋值或初始化 */
    hfmpi2c->XferSize    = hfmpi2c->XferCount;          /* 赋值或初始化 */
    hfmpi2c->XferOptions = XferOptions;                 /* 赋值或初始化 */
    hfmpi2c->XferISR     = FMPI2C_Slave_ISR_IT;         /* 赋值或初始化 */

    if (FMPI2C_GET_DIR(hfmpi2c) == FMPI2C_DIRECTION_RECEIVE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear ADDR flag after prepare the transfer parameters */
      /* This action will generate an acknowledge to the Master */
      __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_ADDR);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c);                              /* HAL辅助宏/函数 */

    /* Note : The FMPI2C interrupts must be enabled after unlocking current process
    to avoid the risk of FMPI2C interrupt handle execution before current
    process unlock */
    /* REnable ADDR interrupt */
    FMPI2C_Enable_IRQ(hfmpi2c, FMPI2C_XFER_TX_IT | FMPI2C_XFER_LISTEN_IT);  /* 执行语句 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Sequential transmit in slave/device FMPI2C mode an amount of data in non-blocking mode with DMA
  * @note   This interface allow to manage repeated start condition when a direction change during transfer
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  XferOptions Options of Transfer, value of @ref FMPI2C_XFEROPTIONS
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Slave_Seq_Transmit_DMA(FMPI2C_HandleTypeDef *hfmpi2c, uint8_t *pData, uint16_t Size,  /* HAL状态类型函数 */
                                                 uint32_t XferOptions)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef dmaxferstatus;                      /* HAL状态类型函数 */

  /* Check the parameters */
  assert_param(IS_FMPI2C_TRANSFER_OPTIONS_REQUEST(XferOptions));  /* 执行语句 */

  if (((uint32_t)hfmpi2c->State & (uint32_t)HAL_FMPI2C_STATE_LISTEN) == (uint32_t)HAL_FMPI2C_STATE_LISTEN)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) || (Size == 0U))                /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_INVALID_PARAM;  /* 赋值或初始化 */
      return  HAL_ERROR;                                /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Process Locked */
    __HAL_LOCK(hfmpi2c);                                /* HAL辅助宏/函数 */

    /* Disable Interrupts, to prevent preemption during treatment in case of multicall */
    FMPI2C_Disable_IRQ(hfmpi2c, FMPI2C_XFER_LISTEN_IT | FMPI2C_XFER_TX_IT);  /* 执行语句 */

    /* FMPI2C cannot manage full duplex exchange so disable previous IT enabled if any */
    /* and then toggle the HAL slave RX state to TX state */
    if (hfmpi2c->State == HAL_FMPI2C_STATE_BUSY_RX_LISTEN)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable associated Interrupts */
      FMPI2C_Disable_IRQ(hfmpi2c, FMPI2C_XFER_RX_IT);   /* 执行语句 */

      if ((hfmpi2c->Instance->CR1 & FMPI2C_CR1_RXDMAEN) == FMPI2C_CR1_RXDMAEN)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Abort DMA Xfer if any */
        if (hfmpi2c->hdmarx != NULL)                    /* 条件判断 */
        {                                               /* 代码块开始 */
          hfmpi2c->Instance->CR1 &= ~FMPI2C_CR1_RXDMAEN;  /* 赋值或初始化 */

          /* Set the FMPI2C DMA Abort callback :
           will lead to call HAL_FMPI2C_ErrorCallback() at end of DMA abort procedure */
          hfmpi2c->hdmarx->XferAbortCallback = FMPI2C_DMAAbort;  /* 赋值或初始化 */

          /* Abort DMA RX */
          if (HAL_DMA_Abort_IT(hfmpi2c->hdmarx) != HAL_OK)  /* 条件判断 */
          {                                             /* 代码块开始 */
            /* Call Directly XferAbortCallback function in case of error */
            hfmpi2c->hdmarx->XferAbortCallback(hfmpi2c->hdmarx);  /* 执行语句 */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else if (hfmpi2c->State == HAL_FMPI2C_STATE_BUSY_TX_LISTEN)  /* 否则如果 */
    {                                                   /* 代码块开始 */
      if ((hfmpi2c->Instance->CR1 & FMPI2C_CR1_TXDMAEN) == FMPI2C_CR1_TXDMAEN)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        hfmpi2c->Instance->CR1 &= ~FMPI2C_CR1_TXDMAEN;  /* 赋值或初始化 */

        /* Abort DMA Xfer if any */
        if (hfmpi2c->hdmatx != NULL)                    /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Set the FMPI2C DMA Abort callback :
           will lead to call HAL_FMPI2C_ErrorCallback() at end of DMA abort procedure */
          hfmpi2c->hdmatx->XferAbortCallback = FMPI2C_DMAAbort;  /* 赋值或初始化 */

          /* Abort DMA TX */
          if (HAL_DMA_Abort_IT(hfmpi2c->hdmatx) != HAL_OK)  /* 条件判断 */
          {                                             /* 代码块开始 */
            /* Call Directly XferAbortCallback function in case of error */
            hfmpi2c->hdmatx->XferAbortCallback(hfmpi2c->hdmatx);  /* 执行语句 */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Nothing to do */
    }                                                   /* 代码块结束 */

    hfmpi2c->State     = HAL_FMPI2C_STATE_BUSY_TX_LISTEN;  /* 赋值或初始化 */
    hfmpi2c->Mode      = HAL_FMPI2C_MODE_SLAVE;         /* 赋值或初始化 */
    hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_NONE;         /* 赋值或初始化 */

    /* Enable Address Acknowledge */
    hfmpi2c->Instance->CR2 &= ~FMPI2C_CR2_NACK;         /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hfmpi2c->pBuffPtr    = pData;                       /* 赋值或初始化 */
    hfmpi2c->XferCount   = Size;                        /* 赋值或初始化 */
    hfmpi2c->XferSize    = hfmpi2c->XferCount;          /* 赋值或初始化 */
    hfmpi2c->XferOptions = XferOptions;                 /* 赋值或初始化 */
    hfmpi2c->XferISR     = FMPI2C_Slave_ISR_DMA;        /* 赋值或初始化 */

    if (hfmpi2c->hdmatx != NULL)                        /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the FMPI2C DMA transfer complete callback */
      hfmpi2c->hdmatx->XferCpltCallback = FMPI2C_DMASlaveTransmitCplt;  /* 赋值或初始化 */

      /* Set the DMA error callback */
      hfmpi2c->hdmatx->XferErrorCallback = FMPI2C_DMAError;  /* 赋值或初始化 */

      /* Set the unused DMA callbacks to NULL */
      hfmpi2c->hdmatx->XferHalfCpltCallback = NULL;     /* 赋值或初始化 */
      hfmpi2c->hdmatx->XferAbortCallback = NULL;        /* 赋值或初始化 */

      /* Enable the DMA stream */
      dmaxferstatus = HAL_DMA_Start_IT(hfmpi2c->hdmatx, (uint32_t)pData, (uint32_t)&hfmpi2c->Instance->TXDR, hfmpi2c->XferSize);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Update FMPI2C state */
      hfmpi2c->State     = HAL_FMPI2C_STATE_LISTEN;     /* 赋值或初始化 */
      hfmpi2c->Mode      = HAL_FMPI2C_MODE_NONE;        /* 赋值或初始化 */

      /* Update FMPI2C error code */
      hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_DMA_PARAM;  /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hfmpi2c);                            /* HAL辅助宏/函数 */

      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    if (dmaxferstatus == HAL_OK)                        /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Update XferCount value */
      hfmpi2c->XferCount -= hfmpi2c->XferSize;          /* 赋值或初始化 */

      /* Reset XferSize */
      hfmpi2c->XferSize = 0;                            /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Update FMPI2C state */
      hfmpi2c->State     = HAL_FMPI2C_STATE_LISTEN;     /* 赋值或初始化 */
      hfmpi2c->Mode      = HAL_FMPI2C_MODE_NONE;        /* 赋值或初始化 */

      /* Update FMPI2C error code */
      hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_DMA;       /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hfmpi2c);                            /* HAL辅助宏/函数 */

      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    if (FMPI2C_GET_DIR(hfmpi2c) == FMPI2C_DIRECTION_RECEIVE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear ADDR flag after prepare the transfer parameters */
      /* This action will generate an acknowledge to the Master */
      __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_ADDR);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c);                              /* HAL辅助宏/函数 */

    /* Note : The FMPI2C interrupts must be enabled after unlocking current process
    to avoid the risk of FMPI2C interrupt handle execution before current
    process unlock */
    /* Enable ERR, STOP, NACK, ADDR interrupts */
    FMPI2C_Enable_IRQ(hfmpi2c, FMPI2C_XFER_LISTEN_IT);  /* 执行语句 */

    /* Enable DMA Request */
    hfmpi2c->Instance->CR1 |= FMPI2C_CR1_TXDMAEN;       /* 赋值或初始化 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Sequential receive in slave/device FMPI2C mode an amount of data in non-blocking mode with Interrupt
  * @note   This interface allow to manage repeated start condition when a direction change during transfer
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  XferOptions Options of Transfer, value of @ref FMPI2C_XFEROPTIONS
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Slave_Seq_Receive_IT(FMPI2C_HandleTypeDef *hfmpi2c, uint8_t *pData, uint16_t Size,  /* HAL状态类型函数 */
                                               uint32_t XferOptions)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_FMPI2C_TRANSFER_OPTIONS_REQUEST(XferOptions));  /* 执行语句 */

  if (((uint32_t)hfmpi2c->State & (uint32_t)HAL_FMPI2C_STATE_LISTEN) == (uint32_t)HAL_FMPI2C_STATE_LISTEN)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) || (Size == 0U))                /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_INVALID_PARAM;  /* 赋值或初始化 */
      return  HAL_ERROR;                                /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Disable Interrupts, to prevent preemption during treatment in case of multicall */
    FMPI2C_Disable_IRQ(hfmpi2c, FMPI2C_XFER_LISTEN_IT | FMPI2C_XFER_RX_IT);  /* 执行语句 */

    /* Process Locked */
    __HAL_LOCK(hfmpi2c);                                /* HAL辅助宏/函数 */

    /* FMPI2C cannot manage full duplex exchange so disable previous IT enabled if any */
    /* and then toggle the HAL slave TX state to RX state */
    if (hfmpi2c->State == HAL_FMPI2C_STATE_BUSY_TX_LISTEN)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable associated Interrupts */
      FMPI2C_Disable_IRQ(hfmpi2c, FMPI2C_XFER_TX_IT);   /* 执行语句 */

      if ((hfmpi2c->Instance->CR1 & FMPI2C_CR1_TXDMAEN) == FMPI2C_CR1_TXDMAEN)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        hfmpi2c->Instance->CR1 &= ~FMPI2C_CR1_TXDMAEN;  /* 赋值或初始化 */

        /* Abort DMA Xfer if any */
        if (hfmpi2c->hdmatx != NULL)                    /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Set the FMPI2C DMA Abort callback :
           will lead to call HAL_FMPI2C_ErrorCallback() at end of DMA abort procedure */
          hfmpi2c->hdmatx->XferAbortCallback = FMPI2C_DMAAbort;  /* 赋值或初始化 */

          /* Abort DMA TX */
          if (HAL_DMA_Abort_IT(hfmpi2c->hdmatx) != HAL_OK)  /* 条件判断 */
          {                                             /* 代码块开始 */
            /* Call Directly XferAbortCallback function in case of error */
            hfmpi2c->hdmatx->XferAbortCallback(hfmpi2c->hdmatx);  /* 执行语句 */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    hfmpi2c->State     = HAL_FMPI2C_STATE_BUSY_RX_LISTEN;  /* 赋值或初始化 */
    hfmpi2c->Mode      = HAL_FMPI2C_MODE_SLAVE;         /* 赋值或初始化 */
    hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_NONE;         /* 赋值或初始化 */

    /* Enable Address Acknowledge */
    hfmpi2c->Instance->CR2 &= ~FMPI2C_CR2_NACK;         /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hfmpi2c->pBuffPtr    = pData;                       /* 赋值或初始化 */
    hfmpi2c->XferCount   = Size;                        /* 赋值或初始化 */
    hfmpi2c->XferSize    = hfmpi2c->XferCount;          /* 赋值或初始化 */
    hfmpi2c->XferOptions = XferOptions;                 /* 赋值或初始化 */
    hfmpi2c->XferISR     = FMPI2C_Slave_ISR_IT;         /* 赋值或初始化 */

    if (FMPI2C_GET_DIR(hfmpi2c) == FMPI2C_DIRECTION_TRANSMIT)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear ADDR flag after prepare the transfer parameters */
      /* This action will generate an acknowledge to the Master */
      __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_ADDR);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c);                              /* HAL辅助宏/函数 */

    /* Note : The FMPI2C interrupts must be enabled after unlocking current process
    to avoid the risk of FMPI2C interrupt handle execution before current
    process unlock */
    /* REnable ADDR interrupt */
    FMPI2C_Enable_IRQ(hfmpi2c, FMPI2C_XFER_RX_IT | FMPI2C_XFER_LISTEN_IT);  /* 执行语句 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Sequential receive in slave/device FMPI2C mode an amount of data in non-blocking mode with DMA
  * @note   This interface allow to manage repeated start condition when a direction change during transfer
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  XferOptions Options of Transfer, value of @ref FMPI2C_XFEROPTIONS
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Slave_Seq_Receive_DMA(FMPI2C_HandleTypeDef *hfmpi2c, uint8_t *pData, uint16_t Size,  /* HAL状态类型函数 */
                                                uint32_t XferOptions)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef dmaxferstatus;                      /* HAL状态类型函数 */

  /* Check the parameters */
  assert_param(IS_FMPI2C_TRANSFER_OPTIONS_REQUEST(XferOptions));  /* 执行语句 */

  if (((uint32_t)hfmpi2c->State & (uint32_t)HAL_FMPI2C_STATE_LISTEN) == (uint32_t)HAL_FMPI2C_STATE_LISTEN)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((pData == NULL) || (Size == 0U))                /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_INVALID_PARAM;  /* 赋值或初始化 */
      return  HAL_ERROR;                                /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Disable Interrupts, to prevent preemption during treatment in case of multicall */
    FMPI2C_Disable_IRQ(hfmpi2c, FMPI2C_XFER_LISTEN_IT | FMPI2C_XFER_RX_IT);  /* 执行语句 */

    /* Process Locked */
    __HAL_LOCK(hfmpi2c);                                /* HAL辅助宏/函数 */

    /* FMPI2C cannot manage full duplex exchange so disable previous IT enabled if any */
    /* and then toggle the HAL slave TX state to RX state */
    if (hfmpi2c->State == HAL_FMPI2C_STATE_BUSY_TX_LISTEN)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable associated Interrupts */
      FMPI2C_Disable_IRQ(hfmpi2c, FMPI2C_XFER_TX_IT);   /* 执行语句 */

      if ((hfmpi2c->Instance->CR1 & FMPI2C_CR1_TXDMAEN) == FMPI2C_CR1_TXDMAEN)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Abort DMA Xfer if any */
        if (hfmpi2c->hdmatx != NULL)                    /* 条件判断 */
        {                                               /* 代码块开始 */
          hfmpi2c->Instance->CR1 &= ~FMPI2C_CR1_TXDMAEN;  /* 赋值或初始化 */

          /* Set the FMPI2C DMA Abort callback :
           will lead to call HAL_FMPI2C_ErrorCallback() at end of DMA abort procedure */
          hfmpi2c->hdmatx->XferAbortCallback = FMPI2C_DMAAbort;  /* 赋值或初始化 */

          /* Abort DMA TX */
          if (HAL_DMA_Abort_IT(hfmpi2c->hdmatx) != HAL_OK)  /* 条件判断 */
          {                                             /* 代码块开始 */
            /* Call Directly XferAbortCallback function in case of error */
            hfmpi2c->hdmatx->XferAbortCallback(hfmpi2c->hdmatx);  /* 执行语句 */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else if (hfmpi2c->State == HAL_FMPI2C_STATE_BUSY_RX_LISTEN)  /* 否则如果 */
    {                                                   /* 代码块开始 */
      if ((hfmpi2c->Instance->CR1 & FMPI2C_CR1_RXDMAEN) == FMPI2C_CR1_RXDMAEN)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        hfmpi2c->Instance->CR1 &= ~FMPI2C_CR1_RXDMAEN;  /* 赋值或初始化 */

        /* Abort DMA Xfer if any */
        if (hfmpi2c->hdmarx != NULL)                    /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Set the FMPI2C DMA Abort callback :
           will lead to call HAL_FMPI2C_ErrorCallback() at end of DMA abort procedure */
          hfmpi2c->hdmarx->XferAbortCallback = FMPI2C_DMAAbort;  /* 赋值或初始化 */

          /* Abort DMA RX */
          if (HAL_DMA_Abort_IT(hfmpi2c->hdmarx) != HAL_OK)  /* 条件判断 */
          {                                             /* 代码块开始 */
            /* Call Directly XferAbortCallback function in case of error */
            hfmpi2c->hdmarx->XferAbortCallback(hfmpi2c->hdmarx);  /* 执行语句 */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Nothing to do */
    }                                                   /* 代码块结束 */

    hfmpi2c->State     = HAL_FMPI2C_STATE_BUSY_RX_LISTEN;  /* 赋值或初始化 */
    hfmpi2c->Mode      = HAL_FMPI2C_MODE_SLAVE;         /* 赋值或初始化 */
    hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_NONE;         /* 赋值或初始化 */

    /* Enable Address Acknowledge */
    hfmpi2c->Instance->CR2 &= ~FMPI2C_CR2_NACK;         /* 赋值或初始化 */

    /* Prepare transfer parameters */
    hfmpi2c->pBuffPtr    = pData;                       /* 赋值或初始化 */
    hfmpi2c->XferCount   = Size;                        /* 赋值或初始化 */
    hfmpi2c->XferSize    = hfmpi2c->XferCount;          /* 赋值或初始化 */
    hfmpi2c->XferOptions = XferOptions;                 /* 赋值或初始化 */
    hfmpi2c->XferISR     = FMPI2C_Slave_ISR_DMA;        /* 赋值或初始化 */

    if (hfmpi2c->hdmarx != NULL)                        /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the FMPI2C DMA transfer complete callback */
      hfmpi2c->hdmarx->XferCpltCallback = FMPI2C_DMASlaveReceiveCplt;  /* 赋值或初始化 */

      /* Set the DMA error callback */
      hfmpi2c->hdmarx->XferErrorCallback = FMPI2C_DMAError;  /* 赋值或初始化 */

      /* Set the unused DMA callbacks to NULL */
      hfmpi2c->hdmarx->XferHalfCpltCallback = NULL;     /* 赋值或初始化 */
      hfmpi2c->hdmarx->XferAbortCallback = NULL;        /* 赋值或初始化 */

      /* Enable the DMA stream */
      dmaxferstatus = HAL_DMA_Start_IT(hfmpi2c->hdmarx, (uint32_t)&hfmpi2c->Instance->RXDR, (uint32_t)pData, hfmpi2c->XferSize);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Update FMPI2C state */
      hfmpi2c->State     = HAL_FMPI2C_STATE_LISTEN;     /* 赋值或初始化 */
      hfmpi2c->Mode      = HAL_FMPI2C_MODE_NONE;        /* 赋值或初始化 */

      /* Update FMPI2C error code */
      hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_DMA_PARAM;  /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hfmpi2c);                            /* HAL辅助宏/函数 */

      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    if (dmaxferstatus == HAL_OK)                        /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Update XferCount value */
      hfmpi2c->XferCount -= hfmpi2c->XferSize;          /* 赋值或初始化 */

      /* Reset XferSize */
      hfmpi2c->XferSize = 0;                            /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Update FMPI2C state */
      hfmpi2c->State     = HAL_FMPI2C_STATE_LISTEN;     /* 赋值或初始化 */
      hfmpi2c->Mode      = HAL_FMPI2C_MODE_NONE;        /* 赋值或初始化 */

      /* Update FMPI2C error code */
      hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_DMA;       /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hfmpi2c);                            /* HAL辅助宏/函数 */

      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    if (FMPI2C_GET_DIR(hfmpi2c) == FMPI2C_DIRECTION_TRANSMIT)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear ADDR flag after prepare the transfer parameters */
      /* This action will generate an acknowledge to the Master */
      __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_ADDR);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c);                              /* HAL辅助宏/函数 */

    /* Note : The FMPI2C interrupts must be enabled after unlocking current process
    to avoid the risk of FMPI2C interrupt handle execution before current
    process unlock */
    /* REnable ADDR interrupt */
    FMPI2C_Enable_IRQ(hfmpi2c, FMPI2C_XFER_RX_IT | FMPI2C_XFER_LISTEN_IT);  /* 执行语句 */

    /* Enable DMA Request */
    hfmpi2c->Instance->CR1 |= FMPI2C_CR1_RXDMAEN;       /* 赋值或初始化 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Enable the Address listen mode with Interrupt.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_EnableListen_IT(FMPI2C_HandleTypeDef *hfmpi2c)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  if (hfmpi2c->State == HAL_FMPI2C_STATE_READY)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    hfmpi2c->State = HAL_FMPI2C_STATE_LISTEN;           /* 赋值或初始化 */
    hfmpi2c->XferISR = FMPI2C_Slave_ISR_IT;             /* 赋值或初始化 */

    /* Enable the Address Match interrupt */
    FMPI2C_Enable_IRQ(hfmpi2c, FMPI2C_XFER_LISTEN_IT);  /* 执行语句 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Disable the Address listen mode with Interrupt.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_DisableListen_IT(FMPI2C_HandleTypeDef *hfmpi2c)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Declaration of tmp to prevent undefined behavior of volatile usage */
  uint32_t tmp;                                         /* 32位无符号变量/函数 */

  /* Disable Address listen mode only if a transfer is not ongoing */
  if (hfmpi2c->State == HAL_FMPI2C_STATE_LISTEN)        /* 条件判断 */
  {                                                     /* 代码块开始 */
    tmp = (uint32_t)(hfmpi2c->State) & FMPI2C_STATE_MSK;  /* 赋值或初始化 */
    hfmpi2c->PreviousState = tmp | (uint32_t)(hfmpi2c->Mode);  /* 赋值或初始化 */
    hfmpi2c->State = HAL_FMPI2C_STATE_READY;            /* 赋值或初始化 */
    hfmpi2c->Mode = HAL_FMPI2C_MODE_NONE;               /* 赋值或初始化 */
    hfmpi2c->XferISR = NULL;                            /* 赋值或初始化 */

    /* Disable the Address Match interrupt */
    FMPI2C_Disable_IRQ(hfmpi2c, FMPI2C_XFER_LISTEN_IT);  /* 执行语句 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Abort a master FMPI2C IT or DMA process communication with Interrupt.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Master_Abort_IT(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t DevAddress)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  if (hfmpi2c->Mode == HAL_FMPI2C_MODE_MASTER)          /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Process Locked */
    __HAL_LOCK(hfmpi2c);                                /* HAL辅助宏/函数 */

    /* Disable Interrupts and Store Previous state */
    if (hfmpi2c->State == HAL_FMPI2C_STATE_BUSY_TX)     /* 条件判断 */
    {                                                   /* 代码块开始 */
      FMPI2C_Disable_IRQ(hfmpi2c, FMPI2C_XFER_TX_IT);   /* 执行语句 */
      hfmpi2c->PreviousState = FMPI2C_STATE_MASTER_BUSY_TX;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if (hfmpi2c->State == HAL_FMPI2C_STATE_BUSY_RX)  /* 否则如果 */
    {                                                   /* 代码块开始 */
      FMPI2C_Disable_IRQ(hfmpi2c, FMPI2C_XFER_RX_IT);   /* 执行语句 */
      hfmpi2c->PreviousState = FMPI2C_STATE_MASTER_BUSY_RX;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Do nothing */
    }                                                   /* 代码块结束 */

    /* Set State at HAL_FMPI2C_STATE_ABORT */
    hfmpi2c->State = HAL_FMPI2C_STATE_ABORT;            /* 赋值或初始化 */

    /* Set NBYTES to 1 to generate a dummy read on FMPI2C peripheral */
    /* Set AUTOEND mode, this will generate a NACK then STOP condition to abort the current transfer */
    FMPI2C_TransferConfig(hfmpi2c, DevAddress, 1, FMPI2C_AUTOEND_MODE, FMPI2C_GENERATE_STOP);  /* 执行语句 */

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c);                              /* HAL辅助宏/函数 */

    /* Note : The FMPI2C interrupts must be enabled after unlocking current process
              to avoid the risk of FMPI2C interrupt handle execution before current
              process unlock */
    FMPI2C_Enable_IRQ(hfmpi2c, FMPI2C_XFER_CPLT_IT);    /* 执行语句 */

    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Wrong usage of abort function */
    /* This function should be used only in case of abort monitored by master device */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup FMPI2C_IRQ_Handler_and_Callbacks IRQ Handler and Callbacks
  * @{
  */

/**
  * @brief  This function handles FMPI2C event interrupt request.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @retval None
  */
void HAL_FMPI2C_EV_IRQHandler(FMPI2C_HandleTypeDef *hfmpi2c)  /* 中断服务函数 */
{                                                       /* 代码块开始 */
  /* Get current IT Flags and IT sources value */
  uint32_t itflags   = READ_REG(hfmpi2c->Instance->ISR);  /* 32位无符号变量/函数 */
  uint32_t itsources = READ_REG(hfmpi2c->Instance->CR1);  /* 32位无符号变量/函数 */

  /* FMPI2C events treatment -------------------------------------*/
  if (hfmpi2c->XferISR != NULL)                         /* 条件判断 */
  {                                                     /* 代码块开始 */
    hfmpi2c->XferISR(hfmpi2c, itflags, itsources);      /* 执行语句 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  This function handles FMPI2C error interrupt request.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @retval None
  */
void HAL_FMPI2C_ER_IRQHandler(FMPI2C_HandleTypeDef *hfmpi2c)  /* 中断服务函数 */
{                                                       /* 代码块开始 */
  uint32_t itflags   = READ_REG(hfmpi2c->Instance->ISR);  /* 32位无符号变量/函数 */
  uint32_t itsources = READ_REG(hfmpi2c->Instance->CR1);  /* 32位无符号变量/函数 */
  uint32_t tmperror;                                    /* 32位无符号变量/函数 */

  /* FMPI2C Bus error interrupt occurred ------------------------------------*/
  if ((FMPI2C_CHECK_FLAG(itflags, FMPI2C_FLAG_BERR) != RESET) && (FMPI2C_CHECK_IT_SOURCE(itsources, FMPI2C_IT_ERRI) != RESET))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_BERR;        /* 赋值或初始化 */

    /* Clear BERR flag */
    __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_BERR);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* FMPI2C Over-Run/Under-Run interrupt occurred ----------------------------------------*/
  if ((FMPI2C_CHECK_FLAG(itflags, FMPI2C_FLAG_OVR) != RESET) && (FMPI2C_CHECK_IT_SOURCE(itsources, FMPI2C_IT_ERRI) != RESET))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_OVR;         /* 赋值或初始化 */

    /* Clear OVR flag */
    __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_OVR);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* FMPI2C Arbitration Loss error interrupt occurred -------------------------------------*/
  if ((FMPI2C_CHECK_FLAG(itflags, FMPI2C_FLAG_ARLO) != RESET) && (FMPI2C_CHECK_IT_SOURCE(itsources, FMPI2C_IT_ERRI) != RESET))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_ARLO;        /* 赋值或初始化 */

    /* Clear ARLO flag */
    __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_ARLO);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Store current volatile hfmpi2c->ErrorCode, misra rule */
  tmperror = hfmpi2c->ErrorCode;                        /* 赋值或初始化 */

  /* Call the Error Callback in case of Error detected */
  if ((tmperror & (HAL_FMPI2C_ERROR_BERR | HAL_FMPI2C_ERROR_OVR | HAL_FMPI2C_ERROR_ARLO)) !=  HAL_FMPI2C_ERROR_NONE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    FMPI2C_ITError(hfmpi2c, tmperror);                  /* 执行语句 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Master Tx Transfer completed callback.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @retval None
  */
__weak void HAL_FMPI2C_MasterTxCpltCallback(FMPI2C_HandleTypeDef *hfmpi2c)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hfmpi2c);                                      /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_FMPI2C_MasterTxCpltCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Master Rx Transfer completed callback.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @retval None
  */
__weak void HAL_FMPI2C_MasterRxCpltCallback(FMPI2C_HandleTypeDef *hfmpi2c)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hfmpi2c);                                      /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_FMPI2C_MasterRxCpltCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/** @brief  Slave Tx Transfer completed callback.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @retval None
  */
__weak void HAL_FMPI2C_SlaveTxCpltCallback(FMPI2C_HandleTypeDef *hfmpi2c)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hfmpi2c);                                      /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_FMPI2C_SlaveTxCpltCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Slave Rx Transfer completed callback.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @retval None
  */
__weak void HAL_FMPI2C_SlaveRxCpltCallback(FMPI2C_HandleTypeDef *hfmpi2c)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hfmpi2c);                                      /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_FMPI2C_SlaveRxCpltCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Slave Address Match callback.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  TransferDirection Master request Transfer Direction (Write/Read), value of @ref FMPI2C_XFERDIRECTION
  * @param  AddrMatchCode Address Match Code
  * @retval None
  */
__weak void HAL_FMPI2C_AddrCallback(FMPI2C_HandleTypeDef *hfmpi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hfmpi2c);                                      /* 执行语句 */
  UNUSED(TransferDirection);                            /* 执行语句 */
  UNUSED(AddrMatchCode);                                /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_FMPI2C_AddrCallback() could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Listen Complete callback.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @retval None
  */
__weak void HAL_FMPI2C_ListenCpltCallback(FMPI2C_HandleTypeDef *hfmpi2c)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hfmpi2c);                                      /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_FMPI2C_ListenCpltCallback() could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Memory Tx Transfer completed callback.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @retval None
  */
__weak void HAL_FMPI2C_MemTxCpltCallback(FMPI2C_HandleTypeDef *hfmpi2c)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hfmpi2c);                                      /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_FMPI2C_MemTxCpltCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Memory Rx Transfer completed callback.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @retval None
  */
__weak void HAL_FMPI2C_MemRxCpltCallback(FMPI2C_HandleTypeDef *hfmpi2c)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hfmpi2c);                                      /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_FMPI2C_MemRxCpltCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  FMPI2C error callback.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @retval None
  */
__weak void HAL_FMPI2C_ErrorCallback(FMPI2C_HandleTypeDef *hfmpi2c)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hfmpi2c);                                      /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_FMPI2C_ErrorCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  FMPI2C abort callback.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @retval None
  */
__weak void HAL_FMPI2C_AbortCpltCallback(FMPI2C_HandleTypeDef *hfmpi2c)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hfmpi2c);                                      /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_FMPI2C_AbortCpltCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup FMPI2C_Exported_Functions_Group3 Peripheral State, Mode and Error functions
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
  * @brief  Return the FMPI2C handle state.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @retval HAL state
  */
HAL_FMPI2C_StateTypeDef HAL_FMPI2C_GetState(FMPI2C_HandleTypeDef *hfmpi2c)  /* HAL库函数调用 */
{                                                       /* 代码块开始 */
  /* Return FMPI2C handle state */
  return hfmpi2c->State;                                /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Returns the FMPI2C Master, Slave, Memory or no mode.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *         the configuration information for FMPI2C module
  * @retval HAL mode
  */
HAL_FMPI2C_ModeTypeDef HAL_FMPI2C_GetMode(FMPI2C_HandleTypeDef *hfmpi2c)  /* HAL库函数调用 */
{                                                       /* 代码块开始 */
  return hfmpi2c->Mode;                                 /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return the FMPI2C error code.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *              the configuration information for the specified FMPI2C.
  * @retval FMPI2C Error Code
  */
uint32_t HAL_FMPI2C_GetError(FMPI2C_HandleTypeDef *hfmpi2c)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  return hfmpi2c->ErrorCode;                            /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup FMPI2C_Private_Functions
  * @{
  */

/**
  * @brief  Interrupt Sub-Routine which handle the Interrupt Flags Master Mode with Interrupt.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  ITFlags Interrupt flags to handle.
  * @param  ITSources Interrupt sources enabled.
  * @retval HAL status
  */
static HAL_StatusTypeDef FMPI2C_Master_ISR_IT(struct __FMPI2C_HandleTypeDef *hfmpi2c, uint32_t ITFlags, uint32_t ITSources)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  uint16_t devaddress;                                  /* 16位无符号变量/函数 */
  uint32_t tmpITFlags = ITFlags;                        /* 32位无符号变量/函数 */

  /* Process Locked */
  __HAL_LOCK(hfmpi2c);                                  /* HAL辅助宏/函数 */

  if ((FMPI2C_CHECK_FLAG(tmpITFlags, FMPI2C_FLAG_AF) != RESET) && (FMPI2C_CHECK_IT_SOURCE(ITSources, FMPI2C_IT_NACKI) != RESET))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Clear NACK Flag */
    __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_AF);   /* HAL辅助宏/函数 */

    /* Set corresponding Error Code */
    /* No need to generate STOP, it is automatically done */
    /* Error callback will be send during stop flag treatment */
    hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_AF;          /* 赋值或初始化 */

    /* Flush TX register */
    FMPI2C_Flush_TXDR(hfmpi2c);                         /* 执行语句 */
  }                                                     /* 代码块结束 */
  else if ((FMPI2C_CHECK_FLAG(tmpITFlags, FMPI2C_FLAG_RXNE) != RESET) && (FMPI2C_CHECK_IT_SOURCE(ITSources, FMPI2C_IT_RXI) != RESET))  /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* Remove RXNE flag on temporary variable as read done */
    tmpITFlags &= ~FMPI2C_FLAG_RXNE;                    /* 赋值或初始化 */

    /* Read data from RXDR */
    *hfmpi2c->pBuffPtr = (uint8_t)hfmpi2c->Instance->RXDR;  /* 赋值或初始化 */

    /* Increment Buffer pointer */
    hfmpi2c->pBuffPtr++;                                /* 执行语句 */

    hfmpi2c->XferSize--;                                /* 执行语句 */
    hfmpi2c->XferCount--;                               /* 执行语句 */
  }                                                     /* 代码块结束 */
  else if ((FMPI2C_CHECK_FLAG(tmpITFlags, FMPI2C_FLAG_TXIS) != RESET) && (FMPI2C_CHECK_IT_SOURCE(ITSources, FMPI2C_IT_TXI) != RESET))  /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* Write data to TXDR */
    hfmpi2c->Instance->TXDR = *hfmpi2c->pBuffPtr;       /* 赋值或初始化 */

    /* Increment Buffer pointer */
    hfmpi2c->pBuffPtr++;                                /* 执行语句 */

    hfmpi2c->XferSize--;                                /* 执行语句 */
    hfmpi2c->XferCount--;                               /* 执行语句 */
  }                                                     /* 代码块结束 */
  else if ((FMPI2C_CHECK_FLAG(tmpITFlags, FMPI2C_FLAG_TCR) != RESET) && (FMPI2C_CHECK_IT_SOURCE(ITSources, FMPI2C_IT_TCI) != RESET))  /* 否则如果 */
  {                                                     /* 代码块开始 */
    if ((hfmpi2c->XferCount != 0U) && (hfmpi2c->XferSize == 0U))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      devaddress = (uint16_t)(hfmpi2c->Instance->CR2 & FMPI2C_CR2_SADD);  /* 赋值或初始化 */

      if (hfmpi2c->XferCount > MAX_NBYTE_SIZE)          /* 条件判断 */
      {                                                 /* 代码块开始 */
        hfmpi2c->XferSize = MAX_NBYTE_SIZE;             /* 赋值或初始化 */
        FMPI2C_TransferConfig(hfmpi2c, devaddress, (uint8_t)hfmpi2c->XferSize, FMPI2C_RELOAD_MODE, FMPI2C_NO_STARTSTOP);  /* 执行语句 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        hfmpi2c->XferSize = hfmpi2c->XferCount;         /* 赋值或初始化 */
        if (hfmpi2c->XferOptions != FMPI2C_NO_OPTION_FRAME)  /* 条件判断 */
        {                                               /* 代码块开始 */
          FMPI2C_TransferConfig(hfmpi2c, devaddress, (uint8_t)hfmpi2c->XferSize, hfmpi2c->XferOptions, FMPI2C_NO_STARTSTOP);  /* 执行语句 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          FMPI2C_TransferConfig(hfmpi2c, devaddress, (uint8_t)hfmpi2c->XferSize, FMPI2C_AUTOEND_MODE, FMPI2C_NO_STARTSTOP);  /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Call TxCpltCallback() if no stop mode is set */
      if (FMPI2C_GET_STOP_MODE(hfmpi2c) != FMPI2C_AUTOEND_MODE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Call FMPI2C Master Sequential complete process */
        FMPI2C_ITMasterSeqCplt(hfmpi2c);                /* 执行语句 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Wrong size Status regarding TCR flag event */
        /* Call the corresponding callback to inform upper layer of End of Transfer */
        FMPI2C_ITError(hfmpi2c, HAL_FMPI2C_ERROR_SIZE);  /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if ((FMPI2C_CHECK_FLAG(tmpITFlags, FMPI2C_FLAG_TC) != RESET) && (FMPI2C_CHECK_IT_SOURCE(ITSources, FMPI2C_IT_TCI) != RESET))  /* 否则如果 */
  {                                                     /* 代码块开始 */
    if (hfmpi2c->XferCount == 0U)                       /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (FMPI2C_GET_STOP_MODE(hfmpi2c) != FMPI2C_AUTOEND_MODE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Generate a stop condition in case of no transfer option */
        if (hfmpi2c->XferOptions == FMPI2C_NO_OPTION_FRAME)  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Generate Stop */
          hfmpi2c->Instance->CR2 |= FMPI2C_CR2_STOP;    /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          /* Call FMPI2C Master Sequential complete process */
          FMPI2C_ITMasterSeqCplt(hfmpi2c);              /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Wrong size Status regarding TC flag event */
      /* Call the corresponding callback to inform upper layer of End of Transfer */
      FMPI2C_ITError(hfmpi2c, HAL_FMPI2C_ERROR_SIZE);   /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Nothing to do */
  }                                                     /* 代码块结束 */

  if ((FMPI2C_CHECK_FLAG(tmpITFlags, FMPI2C_FLAG_STOPF) != RESET) && (FMPI2C_CHECK_IT_SOURCE(ITSources, FMPI2C_IT_STOPI) != RESET))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Call FMPI2C Master complete process */
    FMPI2C_ITMasterCplt(hfmpi2c, tmpITFlags);           /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Process Unlocked */
  __HAL_UNLOCK(hfmpi2c);                                /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Interrupt Sub-Routine which handle the Interrupt Flags Slave Mode with Interrupt.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  ITFlags Interrupt flags to handle.
  * @param  ITSources Interrupt sources enabled.
  * @retval HAL status
  */
static HAL_StatusTypeDef FMPI2C_Slave_ISR_IT(struct __FMPI2C_HandleTypeDef *hfmpi2c, uint32_t ITFlags, uint32_t ITSources)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpoptions = hfmpi2c->XferOptions;           /* 32位无符号变量/函数 */
  uint32_t tmpITFlags = ITFlags;                        /* 32位无符号变量/函数 */

  /* Process locked */
  __HAL_LOCK(hfmpi2c);                                  /* HAL辅助宏/函数 */

  /* Check if STOPF is set */
  if ((FMPI2C_CHECK_FLAG(tmpITFlags, FMPI2C_FLAG_STOPF) != RESET) && (FMPI2C_CHECK_IT_SOURCE(ITSources, FMPI2C_IT_STOPI) != RESET))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Call FMPI2C Slave complete process */
    FMPI2C_ITSlaveCplt(hfmpi2c, tmpITFlags);            /* 执行语句 */
  }                                                     /* 代码块结束 */

  if ((FMPI2C_CHECK_FLAG(tmpITFlags, FMPI2C_FLAG_AF) != RESET) && (FMPI2C_CHECK_IT_SOURCE(ITSources, FMPI2C_IT_NACKI) != RESET))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check that FMPI2C transfer finished */
    /* if yes, normal use case, a NACK is sent by the MASTER when Transfer is finished */
    /* Mean XferCount == 0*/
    /* So clear Flag NACKF only */
    if (hfmpi2c->XferCount == 0U)                       /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Same action must be done for (tmpoptions == FMPI2C_LAST_FRAME) which removed for Warning[Pa134]: left and right operands are identical */
      if ((hfmpi2c->State == HAL_FMPI2C_STATE_LISTEN) && (tmpoptions == FMPI2C_FIRST_AND_LAST_FRAME))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Call FMPI2C Listen complete process */
        FMPI2C_ITListenCplt(hfmpi2c, tmpITFlags);       /* 执行语句 */
      }                                                 /* 代码块结束 */
      else if ((hfmpi2c->State == HAL_FMPI2C_STATE_BUSY_TX_LISTEN) && (tmpoptions != FMPI2C_NO_OPTION_FRAME))  /* 否则如果 */
      {                                                 /* 代码块开始 */
        /* Clear NACK Flag */
        __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_AF);  /* HAL辅助宏/函数 */

        /* Flush TX register */
        FMPI2C_Flush_TXDR(hfmpi2c);                     /* 执行语句 */

        /* Last Byte is Transmitted */
        /* Call FMPI2C Slave Sequential complete process */
        FMPI2C_ITSlaveSeqCplt(hfmpi2c);                 /* 执行语句 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Clear NACK Flag */
        __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_AF);  /* HAL辅助宏/函数 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* if no, error use case, a Non-Acknowledge of last Data is generated by the MASTER*/
      /* Clear NACK Flag */
      __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_AF);  /* HAL辅助宏/函数 */

      /* Set ErrorCode corresponding to a Non-Acknowledge */
      hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_AF;        /* 赋值或初始化 */

      if ((tmpoptions == FMPI2C_FIRST_FRAME) || (tmpoptions == FMPI2C_NEXT_FRAME))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Call the corresponding callback to inform upper layer of End of Transfer */
        FMPI2C_ITError(hfmpi2c, hfmpi2c->ErrorCode);    /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if ((FMPI2C_CHECK_FLAG(tmpITFlags, FMPI2C_FLAG_RXNE) != RESET) && (FMPI2C_CHECK_IT_SOURCE(ITSources, FMPI2C_IT_RXI) != RESET))  /* 否则如果 */
  {                                                     /* 代码块开始 */
    if (hfmpi2c->XferCount > 0U)                        /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Read data from RXDR */
      *hfmpi2c->pBuffPtr = (uint8_t)hfmpi2c->Instance->RXDR;  /* 赋值或初始化 */

      /* Increment Buffer pointer */
      hfmpi2c->pBuffPtr++;                              /* 执行语句 */

      hfmpi2c->XferSize--;                              /* 执行语句 */
      hfmpi2c->XferCount--;                             /* 执行语句 */
    }                                                   /* 代码块结束 */

    if ((hfmpi2c->XferCount == 0U) && \
        (tmpoptions != FMPI2C_NO_OPTION_FRAME))         /* 源代码 */
    {                                                   /* 代码块开始 */
      /* Call FMPI2C Slave Sequential complete process */
      FMPI2C_ITSlaveSeqCplt(hfmpi2c);                   /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if ((FMPI2C_CHECK_FLAG(tmpITFlags, FMPI2C_FLAG_ADDR) != RESET) && \
           (FMPI2C_CHECK_IT_SOURCE(ITSources, FMPI2C_IT_ADDRI) != RESET))  /* 源代码 */
  {                                                     /* 代码块开始 */
    FMPI2C_ITAddrCplt(hfmpi2c, tmpITFlags);             /* 执行语句 */
  }                                                     /* 代码块结束 */
  else if ((FMPI2C_CHECK_FLAG(tmpITFlags, FMPI2C_FLAG_TXIS) != RESET) && (FMPI2C_CHECK_IT_SOURCE(ITSources, FMPI2C_IT_TXI) != RESET))  /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* Write data to TXDR only if XferCount not reach "0" */
    /* A TXIS flag can be set, during STOP treatment      */
    /* Check if all data have already been sent */
    /* If it is the case, this last write in TXDR is not sent, correspond to a dummy TXIS event */
    if (hfmpi2c->XferCount > 0U)                        /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Write data to TXDR */
      hfmpi2c->Instance->TXDR = *hfmpi2c->pBuffPtr;     /* 赋值或初始化 */

      /* Increment Buffer pointer */
      hfmpi2c->pBuffPtr++;                              /* 执行语句 */

      hfmpi2c->XferCount--;                             /* 执行语句 */
      hfmpi2c->XferSize--;                              /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      if ((tmpoptions == FMPI2C_NEXT_FRAME) || (tmpoptions == FMPI2C_FIRST_FRAME))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Last Byte is Transmitted */
        /* Call FMPI2C Slave Sequential complete process */
        FMPI2C_ITSlaveSeqCplt(hfmpi2c);                 /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Nothing to do */
  }                                                     /* 代码块结束 */

  /* Process Unlocked */
  __HAL_UNLOCK(hfmpi2c);                                /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Interrupt Sub-Routine which handle the Interrupt Flags Master Mode with DMA.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  ITFlags Interrupt flags to handle.
  * @param  ITSources Interrupt sources enabled.
  * @retval HAL status
  */
static HAL_StatusTypeDef FMPI2C_Master_ISR_DMA(struct __FMPI2C_HandleTypeDef *hfmpi2c, uint32_t ITFlags, uint32_t ITSources)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  uint16_t devaddress;                                  /* 16位无符号变量/函数 */
  uint32_t xfermode;                                    /* 32位无符号变量/函数 */

  /* Process Locked */
  __HAL_LOCK(hfmpi2c);                                  /* HAL辅助宏/函数 */

  if ((FMPI2C_CHECK_FLAG(ITFlags, FMPI2C_FLAG_AF) != RESET) && (FMPI2C_CHECK_IT_SOURCE(ITSources, FMPI2C_IT_NACKI) != RESET))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Clear NACK Flag */
    __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_AF);   /* HAL辅助宏/函数 */

    /* Set corresponding Error Code */
    hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_AF;          /* 赋值或初始化 */

    /* No need to generate STOP, it is automatically done */
    /* But enable STOP interrupt, to treat it */
    /* Error callback will be send during stop flag treatment */
    FMPI2C_Enable_IRQ(hfmpi2c, FMPI2C_XFER_CPLT_IT);    /* 执行语句 */

    /* Flush TX register */
    FMPI2C_Flush_TXDR(hfmpi2c);                         /* 执行语句 */
  }                                                     /* 代码块结束 */
  else if ((FMPI2C_CHECK_FLAG(ITFlags, FMPI2C_FLAG_TCR) != RESET) && (FMPI2C_CHECK_IT_SOURCE(ITSources, FMPI2C_IT_TCI) != RESET))  /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* Disable TC interrupt */
    __HAL_FMPI2C_DISABLE_IT(hfmpi2c, FMPI2C_IT_TCI);    /* HAL辅助宏/函数 */

    if (hfmpi2c->XferCount != 0U)                       /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Recover Slave address */
      devaddress = (uint16_t)(hfmpi2c->Instance->CR2 & FMPI2C_CR2_SADD);  /* 赋值或初始化 */

      /* Prepare the new XferSize to transfer */
      if (hfmpi2c->XferCount > MAX_NBYTE_SIZE)          /* 条件判断 */
      {                                                 /* 代码块开始 */
        hfmpi2c->XferSize = MAX_NBYTE_SIZE;             /* 赋值或初始化 */
        xfermode = FMPI2C_RELOAD_MODE;                  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        hfmpi2c->XferSize = hfmpi2c->XferCount;         /* 赋值或初始化 */
        if (hfmpi2c->XferOptions != FMPI2C_NO_OPTION_FRAME)  /* 条件判断 */
        {                                               /* 代码块开始 */
          xfermode = hfmpi2c->XferOptions;              /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          xfermode = FMPI2C_AUTOEND_MODE;               /* 赋值或初始化 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */

      /* Set the new XferSize in Nbytes register */
      FMPI2C_TransferConfig(hfmpi2c, devaddress, (uint8_t)hfmpi2c->XferSize, xfermode, FMPI2C_NO_STARTSTOP);  /* 执行语句 */

      /* Update XferCount value */
      hfmpi2c->XferCount -= hfmpi2c->XferSize;          /* 赋值或初始化 */

      /* Enable DMA Request */
      if (hfmpi2c->State == HAL_FMPI2C_STATE_BUSY_RX)   /* 条件判断 */
      {                                                 /* 代码块开始 */
        hfmpi2c->Instance->CR1 |= FMPI2C_CR1_RXDMAEN;   /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        hfmpi2c->Instance->CR1 |= FMPI2C_CR1_TXDMAEN;   /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Call TxCpltCallback() if no stop mode is set */
      if (FMPI2C_GET_STOP_MODE(hfmpi2c) != FMPI2C_AUTOEND_MODE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Call FMPI2C Master Sequential complete process */
        FMPI2C_ITMasterSeqCplt(hfmpi2c);                /* 执行语句 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Wrong size Status regarding TCR flag event */
        /* Call the corresponding callback to inform upper layer of End of Transfer */
        FMPI2C_ITError(hfmpi2c, HAL_FMPI2C_ERROR_SIZE);  /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if ((FMPI2C_CHECK_FLAG(ITFlags, FMPI2C_FLAG_TC) != RESET) && (FMPI2C_CHECK_IT_SOURCE(ITSources, FMPI2C_IT_TCI) != RESET))  /* 否则如果 */
  {                                                     /* 代码块开始 */
    if (hfmpi2c->XferCount == 0U)                       /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (FMPI2C_GET_STOP_MODE(hfmpi2c) != FMPI2C_AUTOEND_MODE)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Generate a stop condition in case of no transfer option */
        if (hfmpi2c->XferOptions == FMPI2C_NO_OPTION_FRAME)  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Generate Stop */
          hfmpi2c->Instance->CR2 |= FMPI2C_CR2_STOP;    /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          /* Call FMPI2C Master Sequential complete process */
          FMPI2C_ITMasterSeqCplt(hfmpi2c);              /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Wrong size Status regarding TC flag event */
      /* Call the corresponding callback to inform upper layer of End of Transfer */
      FMPI2C_ITError(hfmpi2c, HAL_FMPI2C_ERROR_SIZE);   /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if ((FMPI2C_CHECK_FLAG(ITFlags, FMPI2C_FLAG_STOPF) != RESET) && (FMPI2C_CHECK_IT_SOURCE(ITSources, FMPI2C_IT_STOPI) != RESET))  /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* Call FMPI2C Master complete process */
    FMPI2C_ITMasterCplt(hfmpi2c, ITFlags);              /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Nothing to do */
  }                                                     /* 代码块结束 */

  /* Process Unlocked */
  __HAL_UNLOCK(hfmpi2c);                                /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Interrupt Sub-Routine which handle the Interrupt Flags Slave Mode with DMA.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  ITFlags Interrupt flags to handle.
  * @param  ITSources Interrupt sources enabled.
  * @retval HAL status
  */
static HAL_StatusTypeDef FMPI2C_Slave_ISR_DMA(struct __FMPI2C_HandleTypeDef *hfmpi2c, uint32_t ITFlags, uint32_t ITSources)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpoptions = hfmpi2c->XferOptions;           /* 32位无符号变量/函数 */
  uint32_t treatdmanack = 0U;                           /* 32位无符号变量/函数 */
  HAL_FMPI2C_StateTypeDef tmpstate;                     /* HAL库函数调用 */

  /* Process locked */
  __HAL_LOCK(hfmpi2c);                                  /* HAL辅助宏/函数 */

  /* Check if STOPF is set */
  if ((FMPI2C_CHECK_FLAG(ITFlags, FMPI2C_FLAG_STOPF) != RESET) && (FMPI2C_CHECK_IT_SOURCE(ITSources, FMPI2C_IT_STOPI) != RESET))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Call FMPI2C Slave complete process */
    FMPI2C_ITSlaveCplt(hfmpi2c, ITFlags);               /* 执行语句 */
  }                                                     /* 代码块结束 */

  if ((FMPI2C_CHECK_FLAG(ITFlags, FMPI2C_FLAG_AF) != RESET) && (FMPI2C_CHECK_IT_SOURCE(ITSources, FMPI2C_IT_NACKI) != RESET))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check that FMPI2C transfer finished */
    /* if yes, normal use case, a NACK is sent by the MASTER when Transfer is finished */
    /* Mean XferCount == 0 */
    /* So clear Flag NACKF only */
    if ((FMPI2C_CHECK_IT_SOURCE(ITSources, FMPI2C_CR1_TXDMAEN) != RESET) ||  /* 条件判断 */
        (FMPI2C_CHECK_IT_SOURCE(ITSources, FMPI2C_CR1_RXDMAEN) != RESET))  /* 源代码 */
    {                                                   /* 代码块开始 */
      /* Split check of hdmarx, for MISRA compliance */
      if (hfmpi2c->hdmarx != NULL)                      /* 条件判断 */
      {                                                 /* 代码块开始 */
        if (FMPI2C_CHECK_IT_SOURCE(ITSources, FMPI2C_CR1_RXDMAEN) != RESET)  /* 条件判断 */
        {                                               /* 代码块开始 */
          if (__HAL_DMA_GET_COUNTER(hfmpi2c->hdmarx) == 0U)  /* 条件判断 */
          {                                             /* 代码块开始 */
            treatdmanack = 1U;                          /* 赋值或初始化 */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */

      /* Split check of hdmatx, for MISRA compliance  */
      if (hfmpi2c->hdmatx != NULL)                      /* 条件判断 */
      {                                                 /* 代码块开始 */
        if (FMPI2C_CHECK_IT_SOURCE(ITSources, FMPI2C_CR1_TXDMAEN) != RESET)  /* 条件判断 */
        {                                               /* 代码块开始 */
          if (__HAL_DMA_GET_COUNTER(hfmpi2c->hdmatx) == 0U)  /* 条件判断 */
          {                                             /* 代码块开始 */
            treatdmanack = 1U;                          /* 赋值或初始化 */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */

      if (treatdmanack == 1U)                           /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Same action must be done for (tmpoptions == FMPI2C_LAST_FRAME) which removed for Warning[Pa134]: left and right operands are identical */
        if ((hfmpi2c->State == HAL_FMPI2C_STATE_LISTEN) && (tmpoptions == FMPI2C_FIRST_AND_LAST_FRAME))  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Call FMPI2C Listen complete process */
          FMPI2C_ITListenCplt(hfmpi2c, ITFlags);        /* 执行语句 */
        }                                               /* 代码块结束 */
        else if ((hfmpi2c->State == HAL_FMPI2C_STATE_BUSY_TX_LISTEN) && (tmpoptions != FMPI2C_NO_OPTION_FRAME))  /* 否则如果 */
        {                                               /* 代码块开始 */
          /* Clear NACK Flag */
          __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_AF);  /* HAL辅助宏/函数 */

          /* Flush TX register */
          FMPI2C_Flush_TXDR(hfmpi2c);                   /* 执行语句 */

          /* Last Byte is Transmitted */
          /* Call FMPI2C Slave Sequential complete process */
          FMPI2C_ITSlaveSeqCplt(hfmpi2c);               /* 执行语句 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          /* Clear NACK Flag */
          __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_AF);  /* HAL辅助宏/函数 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* if no, error use case, a Non-Acknowledge of last Data is generated by the MASTER*/
        /* Clear NACK Flag */
        __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_AF);  /* HAL辅助宏/函数 */

        /* Set ErrorCode corresponding to a Non-Acknowledge */
        hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_AF;      /* 赋值或初始化 */

        /* Store current hfmpi2c->State, solve MISRA2012-Rule-13.5 */
        tmpstate = hfmpi2c->State;                      /* 赋值或初始化 */

        if ((tmpoptions == FMPI2C_FIRST_FRAME) || (tmpoptions == FMPI2C_NEXT_FRAME))  /* 条件判断 */
        {                                               /* 代码块开始 */
          if ((tmpstate == HAL_FMPI2C_STATE_BUSY_TX) || (tmpstate == HAL_FMPI2C_STATE_BUSY_TX_LISTEN))  /* 条件判断 */
          {                                             /* 代码块开始 */
            hfmpi2c->PreviousState = FMPI2C_STATE_SLAVE_BUSY_TX;  /* 赋值或初始化 */
          }                                             /* 代码块结束 */
          else if ((tmpstate == HAL_FMPI2C_STATE_BUSY_RX) || (tmpstate == HAL_FMPI2C_STATE_BUSY_RX_LISTEN))  /* 否则如果 */
          {                                             /* 代码块开始 */
            hfmpi2c->PreviousState = FMPI2C_STATE_SLAVE_BUSY_RX;  /* 赋值或初始化 */
          }                                             /* 代码块结束 */
          else                                          /* 否则 */
          {                                             /* 代码块开始 */
            /* Do nothing */
          }                                             /* 代码块结束 */

          /* Call the corresponding callback to inform upper layer of End of Transfer */
          FMPI2C_ITError(hfmpi2c, hfmpi2c->ErrorCode);  /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Only Clear NACK Flag, no DMA treatment is pending */
      __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_AF);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if ((FMPI2C_CHECK_FLAG(ITFlags, FMPI2C_FLAG_ADDR) != RESET) && (FMPI2C_CHECK_IT_SOURCE(ITSources, FMPI2C_IT_ADDRI) != RESET))  /* 否则如果 */
  {                                                     /* 代码块开始 */
    FMPI2C_ITAddrCplt(hfmpi2c, ITFlags);                /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Nothing to do */
  }                                                     /* 代码块结束 */

  /* Process Unlocked */
  __HAL_UNLOCK(hfmpi2c);                                /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Master sends target device address followed by internal memory address for write request.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  MemAddress Internal memory address
  * @param  MemAddSize Size of internal memory address
  * @param  Timeout Timeout duration
  * @param  Tickstart Tick start value
  * @retval HAL status
  */
static HAL_StatusTypeDef FMPI2C_RequestMemoryWrite(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t DevAddress, uint16_t MemAddress,  /* 静态变量/函数 */
                                                uint16_t MemAddSize, uint32_t Timeout, uint32_t Tickstart)  /* 16位无符号变量/函数 */
{                                                       /* 代码块开始 */
  FMPI2C_TransferConfig(hfmpi2c, DevAddress, (uint8_t)MemAddSize, FMPI2C_RELOAD_MODE, FMPI2C_GENERATE_START_WRITE);  /* 执行语句 */

  /* Wait until TXIS flag is set */
  if (FMPI2C_WaitOnTXISFlagUntilTimeout(hfmpi2c, Timeout, Tickstart) != HAL_OK)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* If Memory address size is 8Bit */
  if (MemAddSize == FMPI2C_MEMADD_SIZE_8BIT)            /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Send Memory Address */
    hfmpi2c->Instance->TXDR = FMPI2C_MEM_ADD_LSB(MemAddress);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  /* If Memory address size is 16Bit */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Send MSB of Memory Address */
    hfmpi2c->Instance->TXDR = FMPI2C_MEM_ADD_MSB(MemAddress);  /* 赋值或初始化 */

    /* Wait until TXIS flag is set */
    if (FMPI2C_WaitOnTXISFlagUntilTimeout(hfmpi2c, Timeout, Tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Send LSB of Memory Address */
    hfmpi2c->Instance->TXDR = FMPI2C_MEM_ADD_LSB(MemAddress);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Wait until TCR flag is set */
  if (FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_TCR, RESET, Timeout, Tickstart) != HAL_OK)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Master sends target device address followed by internal memory address for read request.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  MemAddress Internal memory address
  * @param  MemAddSize Size of internal memory address
  * @param  Timeout Timeout duration
  * @param  Tickstart Tick start value
  * @retval HAL status
  */
static HAL_StatusTypeDef FMPI2C_RequestMemoryRead(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t DevAddress, uint16_t MemAddress,  /* 静态变量/函数 */
                                               uint16_t MemAddSize, uint32_t Timeout, uint32_t Tickstart)  /* 16位无符号变量/函数 */
{                                                       /* 代码块开始 */
  FMPI2C_TransferConfig(hfmpi2c, DevAddress, (uint8_t)MemAddSize, FMPI2C_SOFTEND_MODE, FMPI2C_GENERATE_START_WRITE);  /* 执行语句 */

  /* Wait until TXIS flag is set */
  if (FMPI2C_WaitOnTXISFlagUntilTimeout(hfmpi2c, Timeout, Tickstart) != HAL_OK)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* If Memory address size is 8Bit */
  if (MemAddSize == FMPI2C_MEMADD_SIZE_8BIT)            /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Send Memory Address */
    hfmpi2c->Instance->TXDR = FMPI2C_MEM_ADD_LSB(MemAddress);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  /* If Memory address size is 16Bit */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Send MSB of Memory Address */
    hfmpi2c->Instance->TXDR = FMPI2C_MEM_ADD_MSB(MemAddress);  /* 赋值或初始化 */

    /* Wait until TXIS flag is set */
    if (FMPI2C_WaitOnTXISFlagUntilTimeout(hfmpi2c, Timeout, Tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Send LSB of Memory Address */
    hfmpi2c->Instance->TXDR = FMPI2C_MEM_ADD_LSB(MemAddress);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Wait until TC flag is set */
  if (FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_TC, RESET, Timeout, Tickstart) != HAL_OK)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  FMPI2C Address complete process callback.
  * @param  hfmpi2c FMPI2C handle.
  * @param  ITFlags Interrupt flags to handle.
  * @retval None
  */
static void FMPI2C_ITAddrCplt(FMPI2C_HandleTypeDef *hfmpi2c, uint32_t ITFlags)  /* 静态函数 */
{                                                       /* 代码块开始 */
  uint8_t transferdirection;                            /* 8位无符号变量/函数 */
  uint16_t slaveaddrcode;                               /* 16位无符号变量/函数 */
  uint16_t ownadd1code;                                 /* 16位无符号变量/函数 */
  uint16_t ownadd2code;                                 /* 16位无符号变量/函数 */

  /* Prevent unused argument(s) compilation warning */
  UNUSED(ITFlags);                                      /* 执行语句 */

  /* In case of Listen state, need to inform upper layer of address match code event */
  if (((uint32_t)hfmpi2c->State & (uint32_t)HAL_FMPI2C_STATE_LISTEN) == (uint32_t)HAL_FMPI2C_STATE_LISTEN)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    transferdirection = FMPI2C_GET_DIR(hfmpi2c);        /* 赋值或初始化 */
    slaveaddrcode     = FMPI2C_GET_ADDR_MATCH(hfmpi2c);  /* 赋值或初始化 */
    ownadd1code       = FMPI2C_GET_OWN_ADDRESS1(hfmpi2c);  /* 赋值或初始化 */
    ownadd2code       = FMPI2C_GET_OWN_ADDRESS2(hfmpi2c);  /* 赋值或初始化 */

    /* If 10bits addressing mode is selected */
    if (hfmpi2c->Init.AddressingMode == FMPI2C_ADDRESSINGMODE_10BIT)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      if ((slaveaddrcode & SlaveAddr_MSK) == ((ownadd1code >> SlaveAddr_SHIFT) & SlaveAddr_MSK))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        slaveaddrcode = ownadd1code;                    /* 赋值或初始化 */
        hfmpi2c->AddrEventCount++;                      /* 执行语句 */
        if (hfmpi2c->AddrEventCount == 2U)              /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Reset Address Event counter */
          hfmpi2c->AddrEventCount = 0U;                 /* 赋值或初始化 */

          /* Clear ADDR flag */
          __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_ADDR);  /* HAL辅助宏/函数 */

          /* Process Unlocked */
          __HAL_UNLOCK(hfmpi2c);                        /* HAL辅助宏/函数 */

          /* Call Slave Addr callback */
#if (USE_HAL_FMPI2C_REGISTER_CALLBACKS == 1)            /* 条件编译 */
          hfmpi2c->AddrCallback(hfmpi2c, transferdirection, slaveaddrcode);  /* 执行语句 */
#else                                                   /* 条件编译否则 */
          HAL_FMPI2C_AddrCallback(hfmpi2c, transferdirection, slaveaddrcode);  /* HAL库函数调用 */
#endif /* USE_HAL_FMPI2C_REGISTER_CALLBACKS */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        slaveaddrcode = ownadd2code;                    /* 赋值或初始化 */

        /* Disable ADDR Interrupts */
        FMPI2C_Disable_IRQ(hfmpi2c, FMPI2C_XFER_LISTEN_IT);  /* 执行语句 */

        /* Process Unlocked */
        __HAL_UNLOCK(hfmpi2c);                          /* HAL辅助宏/函数 */

        /* Call Slave Addr callback */
#if (USE_HAL_FMPI2C_REGISTER_CALLBACKS == 1)            /* 条件编译 */
        hfmpi2c->AddrCallback(hfmpi2c, transferdirection, slaveaddrcode);  /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_FMPI2C_AddrCallback(hfmpi2c, transferdirection, slaveaddrcode);  /* HAL库函数调用 */
#endif /* USE_HAL_FMPI2C_REGISTER_CALLBACKS */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    /* else 7 bits addressing mode is selected */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Disable ADDR Interrupts */
      FMPI2C_Disable_IRQ(hfmpi2c, FMPI2C_XFER_LISTEN_IT);  /* 执行语句 */

      /* Process Unlocked */
      __HAL_UNLOCK(hfmpi2c);                            /* HAL辅助宏/函数 */

      /* Call Slave Addr callback */
#if (USE_HAL_FMPI2C_REGISTER_CALLBACKS == 1)            /* 条件编译 */
      hfmpi2c->AddrCallback(hfmpi2c, transferdirection, slaveaddrcode);  /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_FMPI2C_AddrCallback(hfmpi2c, transferdirection, slaveaddrcode);  /* HAL库函数调用 */
#endif /* USE_HAL_FMPI2C_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /* Else clear address flag only */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Clear ADDR flag */
    __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_ADDR);  /* HAL辅助宏/函数 */

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c);                              /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  FMPI2C Master sequential complete process.
  * @param  hfmpi2c FMPI2C handle.
  * @retval None
  */
static void FMPI2C_ITMasterSeqCplt(FMPI2C_HandleTypeDef *hfmpi2c)  /* 静态函数 */
{                                                       /* 代码块开始 */
  /* Reset FMPI2C handle mode */
  hfmpi2c->Mode = HAL_FMPI2C_MODE_NONE;                 /* 赋值或初始化 */

  /* No Generate Stop, to permit restart mode */
  /* The stop will be done at the end of transfer, when FMPI2C_AUTOEND_MODE enable */
  if (hfmpi2c->State == HAL_FMPI2C_STATE_BUSY_TX)       /* 条件判断 */
  {                                                     /* 代码块开始 */
    hfmpi2c->State         = HAL_FMPI2C_STATE_READY;    /* 赋值或初始化 */
    hfmpi2c->PreviousState = FMPI2C_STATE_MASTER_BUSY_TX;  /* 赋值或初始化 */
    hfmpi2c->XferISR       = NULL;                      /* 赋值或初始化 */

    /* Disable Interrupts */
    FMPI2C_Disable_IRQ(hfmpi2c, FMPI2C_XFER_TX_IT);     /* 执行语句 */

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c);                              /* HAL辅助宏/函数 */

    /* Call the corresponding callback to inform upper layer of End of Transfer */
#if (USE_HAL_FMPI2C_REGISTER_CALLBACKS == 1)            /* 条件编译 */
    hfmpi2c->MasterTxCpltCallback(hfmpi2c);             /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_FMPI2C_MasterTxCpltCallback(hfmpi2c);           /* HAL库函数调用 */
#endif /* USE_HAL_FMPI2C_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
  /* hfmpi2c->State == HAL_FMPI2C_STATE_BUSY_RX */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    hfmpi2c->State         = HAL_FMPI2C_STATE_READY;    /* 赋值或初始化 */
    hfmpi2c->PreviousState = FMPI2C_STATE_MASTER_BUSY_RX;  /* 赋值或初始化 */
    hfmpi2c->XferISR       = NULL;                      /* 赋值或初始化 */

    /* Disable Interrupts */
    FMPI2C_Disable_IRQ(hfmpi2c, FMPI2C_XFER_RX_IT);     /* 执行语句 */

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c);                              /* HAL辅助宏/函数 */

    /* Call the corresponding callback to inform upper layer of End of Transfer */
#if (USE_HAL_FMPI2C_REGISTER_CALLBACKS == 1)            /* 条件编译 */
    hfmpi2c->MasterRxCpltCallback(hfmpi2c);             /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_FMPI2C_MasterRxCpltCallback(hfmpi2c);           /* HAL库函数调用 */
#endif /* USE_HAL_FMPI2C_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  FMPI2C Slave sequential complete process.
  * @param  hfmpi2c FMPI2C handle.
  * @retval None
  */
static void FMPI2C_ITSlaveSeqCplt(FMPI2C_HandleTypeDef *hfmpi2c)  /* 静态函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpcr1value = READ_REG(hfmpi2c->Instance->CR1);  /* 32位无符号变量/函数 */

  /* Reset FMPI2C handle mode */
  hfmpi2c->Mode = HAL_FMPI2C_MODE_NONE;                 /* 赋值或初始化 */

  /* If a DMA is ongoing, Update handle size context */
  if (FMPI2C_CHECK_IT_SOURCE(tmpcr1value, FMPI2C_CR1_TXDMAEN) != RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable DMA Request */
    hfmpi2c->Instance->CR1 &= ~FMPI2C_CR1_TXDMAEN;      /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if (FMPI2C_CHECK_IT_SOURCE(tmpcr1value, FMPI2C_CR1_RXDMAEN) != RESET)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* Disable DMA Request */
    hfmpi2c->Instance->CR1 &= ~FMPI2C_CR1_RXDMAEN;      /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Do nothing */
  }                                                     /* 代码块结束 */

  if (hfmpi2c->State == HAL_FMPI2C_STATE_BUSY_TX_LISTEN)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Remove HAL_FMPI2C_STATE_SLAVE_BUSY_TX, keep only HAL_FMPI2C_STATE_LISTEN */
    hfmpi2c->State         = HAL_FMPI2C_STATE_LISTEN;   /* 赋值或初始化 */
    hfmpi2c->PreviousState = FMPI2C_STATE_SLAVE_BUSY_TX;  /* 赋值或初始化 */

    /* Disable Interrupts */
    FMPI2C_Disable_IRQ(hfmpi2c, FMPI2C_XFER_TX_IT);     /* 执行语句 */

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c);                              /* HAL辅助宏/函数 */

    /* Call the corresponding callback to inform upper layer of End of Transfer */
#if (USE_HAL_FMPI2C_REGISTER_CALLBACKS == 1)            /* 条件编译 */
    hfmpi2c->SlaveTxCpltCallback(hfmpi2c);              /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_FMPI2C_SlaveTxCpltCallback(hfmpi2c);            /* HAL库函数调用 */
#endif /* USE_HAL_FMPI2C_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */

  else if (hfmpi2c->State == HAL_FMPI2C_STATE_BUSY_RX_LISTEN)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* Remove HAL_FMPI2C_STATE_SLAVE_BUSY_RX, keep only HAL_FMPI2C_STATE_LISTEN */
    hfmpi2c->State         = HAL_FMPI2C_STATE_LISTEN;   /* 赋值或初始化 */
    hfmpi2c->PreviousState = FMPI2C_STATE_SLAVE_BUSY_RX;  /* 赋值或初始化 */

    /* Disable Interrupts */
    FMPI2C_Disable_IRQ(hfmpi2c, FMPI2C_XFER_RX_IT);     /* 执行语句 */

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c);                              /* HAL辅助宏/函数 */

    /* Call the corresponding callback to inform upper layer of End of Transfer */
#if (USE_HAL_FMPI2C_REGISTER_CALLBACKS == 1)            /* 条件编译 */
    hfmpi2c->SlaveRxCpltCallback(hfmpi2c);              /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_FMPI2C_SlaveRxCpltCallback(hfmpi2c);            /* HAL库函数调用 */
#endif /* USE_HAL_FMPI2C_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Nothing to do */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  FMPI2C Master complete process.
  * @param  hfmpi2c FMPI2C handle.
  * @param  ITFlags Interrupt flags to handle.
  * @retval None
  */
static void FMPI2C_ITMasterCplt(FMPI2C_HandleTypeDef *hfmpi2c, uint32_t ITFlags)  /* 静态函数 */
{                                                       /* 代码块开始 */
  uint32_t tmperror;                                    /* 32位无符号变量/函数 */
  uint32_t tmpITFlags = ITFlags;                        /* 32位无符号变量/函数 */
  __IO uint32_t tmpreg;                                 /* 执行语句 */

  /* Clear STOP Flag */
  __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF);  /* HAL辅助宏/函数 */

  /* Disable Interrupts and Store Previous state */
  if (hfmpi2c->State == HAL_FMPI2C_STATE_BUSY_TX)       /* 条件判断 */
  {                                                     /* 代码块开始 */
    FMPI2C_Disable_IRQ(hfmpi2c, FMPI2C_XFER_TX_IT);     /* 执行语句 */
    hfmpi2c->PreviousState = FMPI2C_STATE_MASTER_BUSY_TX;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if (hfmpi2c->State == HAL_FMPI2C_STATE_BUSY_RX)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    FMPI2C_Disable_IRQ(hfmpi2c, FMPI2C_XFER_RX_IT);     /* 执行语句 */
    hfmpi2c->PreviousState = FMPI2C_STATE_MASTER_BUSY_RX;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Do nothing */
  }                                                     /* 代码块结束 */

  /* Clear Configuration Register 2 */
  FMPI2C_RESET_CR2(hfmpi2c);                            /* 执行语句 */

  /* Reset handle parameters */
  hfmpi2c->XferISR       = NULL;                        /* 赋值或初始化 */
  hfmpi2c->XferOptions   = FMPI2C_NO_OPTION_FRAME;      /* 赋值或初始化 */

  if (FMPI2C_CHECK_FLAG(tmpITFlags, FMPI2C_FLAG_AF) != RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Clear NACK Flag */
    __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_AF);   /* HAL辅助宏/函数 */

    /* Set acknowledge error code */
    hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_AF;          /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Fetch Last receive data if any */
  if ((hfmpi2c->State == HAL_FMPI2C_STATE_ABORT) && (FMPI2C_CHECK_FLAG(tmpITFlags, FMPI2C_FLAG_RXNE) != RESET))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Read data from RXDR */
    tmpreg = (uint8_t)hfmpi2c->Instance->RXDR;          /* 赋值或初始化 */
    UNUSED(tmpreg);                                     /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Flush TX register */
  FMPI2C_Flush_TXDR(hfmpi2c);                           /* 执行语句 */

  /* Store current volatile hfmpi2c->ErrorCode, misra rule */
  tmperror = hfmpi2c->ErrorCode;                        /* 赋值或初始化 */

  /* Call the corresponding callback to inform upper layer of End of Transfer */
  if ((hfmpi2c->State == HAL_FMPI2C_STATE_ABORT) || (tmperror != HAL_FMPI2C_ERROR_NONE))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Call the corresponding callback to inform upper layer of End of Transfer */
    FMPI2C_ITError(hfmpi2c, hfmpi2c->ErrorCode);        /* 执行语句 */
  }                                                     /* 代码块结束 */
  /* hfmpi2c->State == HAL_FMPI2C_STATE_BUSY_TX */
  else if (hfmpi2c->State == HAL_FMPI2C_STATE_BUSY_TX)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    hfmpi2c->State = HAL_FMPI2C_STATE_READY;            /* 赋值或初始化 */
    hfmpi2c->PreviousState = FMPI2C_STATE_NONE;         /* 赋值或初始化 */

    if (hfmpi2c->Mode == HAL_FMPI2C_MODE_MEM)           /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpi2c->Mode = HAL_FMPI2C_MODE_NONE;             /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hfmpi2c);                            /* HAL辅助宏/函数 */

      /* Call the corresponding callback to inform upper layer of End of Transfer */
#if (USE_HAL_FMPI2C_REGISTER_CALLBACKS == 1)            /* 条件编译 */
      hfmpi2c->MemTxCpltCallback(hfmpi2c);              /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_FMPI2C_MemTxCpltCallback(hfmpi2c);            /* HAL库函数调用 */
#endif /* USE_HAL_FMPI2C_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hfmpi2c->Mode = HAL_FMPI2C_MODE_NONE;             /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hfmpi2c);                            /* HAL辅助宏/函数 */

      /* Call the corresponding callback to inform upper layer of End of Transfer */
#if (USE_HAL_FMPI2C_REGISTER_CALLBACKS == 1)            /* 条件编译 */
      hfmpi2c->MasterTxCpltCallback(hfmpi2c);           /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_FMPI2C_MasterTxCpltCallback(hfmpi2c);         /* HAL库函数调用 */
#endif /* USE_HAL_FMPI2C_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /* hfmpi2c->State == HAL_FMPI2C_STATE_BUSY_RX */
  else if (hfmpi2c->State == HAL_FMPI2C_STATE_BUSY_RX)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    hfmpi2c->State = HAL_FMPI2C_STATE_READY;            /* 赋值或初始化 */
    hfmpi2c->PreviousState = FMPI2C_STATE_NONE;         /* 赋值或初始化 */

    if (hfmpi2c->Mode == HAL_FMPI2C_MODE_MEM)           /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpi2c->Mode = HAL_FMPI2C_MODE_NONE;             /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hfmpi2c);                            /* HAL辅助宏/函数 */

      /* Call the corresponding callback to inform upper layer of End of Transfer */
#if (USE_HAL_FMPI2C_REGISTER_CALLBACKS == 1)            /* 条件编译 */
      hfmpi2c->MemRxCpltCallback(hfmpi2c);              /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_FMPI2C_MemRxCpltCallback(hfmpi2c);            /* HAL库函数调用 */
#endif /* USE_HAL_FMPI2C_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hfmpi2c->Mode = HAL_FMPI2C_MODE_NONE;             /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hfmpi2c);                            /* HAL辅助宏/函数 */

      /* Call the corresponding callback to inform upper layer of End of Transfer */
#if (USE_HAL_FMPI2C_REGISTER_CALLBACKS == 1)            /* 条件编译 */
      hfmpi2c->MasterRxCpltCallback(hfmpi2c);           /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_FMPI2C_MasterRxCpltCallback(hfmpi2c);         /* HAL库函数调用 */
#endif /* USE_HAL_FMPI2C_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Nothing to do */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  FMPI2C Slave complete process.
  * @param  hfmpi2c FMPI2C handle.
  * @param  ITFlags Interrupt flags to handle.
  * @retval None
  */
static void FMPI2C_ITSlaveCplt(FMPI2C_HandleTypeDef *hfmpi2c, uint32_t ITFlags)  /* 静态函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpcr1value = READ_REG(hfmpi2c->Instance->CR1);  /* 32位无符号变量/函数 */
  uint32_t tmpITFlags = ITFlags;                        /* 32位无符号变量/函数 */
  HAL_FMPI2C_StateTypeDef tmpstate = hfmpi2c->State;    /* HAL库函数调用 */

  /* Clear STOP Flag */
  __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF);  /* HAL辅助宏/函数 */

  /* Disable Interrupts and Store Previous state */
  if ((tmpstate == HAL_FMPI2C_STATE_BUSY_TX) || (tmpstate == HAL_FMPI2C_STATE_BUSY_TX_LISTEN))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    FMPI2C_Disable_IRQ(hfmpi2c, FMPI2C_XFER_LISTEN_IT | FMPI2C_XFER_TX_IT);  /* 执行语句 */
    hfmpi2c->PreviousState = FMPI2C_STATE_SLAVE_BUSY_TX;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if ((tmpstate == HAL_FMPI2C_STATE_BUSY_RX) || (tmpstate == HAL_FMPI2C_STATE_BUSY_RX_LISTEN))  /* 否则如果 */
  {                                                     /* 代码块开始 */
    FMPI2C_Disable_IRQ(hfmpi2c, FMPI2C_XFER_LISTEN_IT | FMPI2C_XFER_RX_IT);  /* 执行语句 */
    hfmpi2c->PreviousState = FMPI2C_STATE_SLAVE_BUSY_RX;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Do nothing */
  }                                                     /* 代码块结束 */

  /* Disable Address Acknowledge */
  hfmpi2c->Instance->CR2 |= FMPI2C_CR2_NACK;            /* 赋值或初始化 */

  /* Clear Configuration Register 2 */
  FMPI2C_RESET_CR2(hfmpi2c);                            /* 执行语句 */

  /* Flush TX register */
  FMPI2C_Flush_TXDR(hfmpi2c);                           /* 执行语句 */

  /* If a DMA is ongoing, Update handle size context */
  if (FMPI2C_CHECK_IT_SOURCE(tmpcr1value, FMPI2C_CR1_TXDMAEN) != RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable DMA Request */
    hfmpi2c->Instance->CR1 &= ~FMPI2C_CR1_TXDMAEN;      /* 赋值或初始化 */

    if (hfmpi2c->hdmatx != NULL)                        /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpi2c->XferCount = (uint16_t)__HAL_DMA_GET_COUNTER(hfmpi2c->hdmatx);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (FMPI2C_CHECK_IT_SOURCE(tmpcr1value, FMPI2C_CR1_RXDMAEN) != RESET)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* Disable DMA Request */
    hfmpi2c->Instance->CR1 &= ~FMPI2C_CR1_RXDMAEN;      /* 赋值或初始化 */

    if (hfmpi2c->hdmarx != NULL)                        /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpi2c->XferCount = (uint16_t)__HAL_DMA_GET_COUNTER(hfmpi2c->hdmarx);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Do nothing */
  }                                                     /* 代码块结束 */

  /* Store Last receive data if any */
  if (FMPI2C_CHECK_FLAG(tmpITFlags, FMPI2C_FLAG_RXNE) != RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Remove RXNE flag on temporary variable as read done */
    tmpITFlags &= ~FMPI2C_FLAG_RXNE;                    /* 赋值或初始化 */

    /* Read data from RXDR */
    *hfmpi2c->pBuffPtr = (uint8_t)hfmpi2c->Instance->RXDR;  /* 赋值或初始化 */

    /* Increment Buffer pointer */
    hfmpi2c->pBuffPtr++;                                /* 执行语句 */

    if ((hfmpi2c->XferSize > 0U))                       /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpi2c->XferSize--;                              /* 执行语句 */
      hfmpi2c->XferCount--;                             /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* All data are not transferred, so set error code accordingly */
  if (hfmpi2c->XferCount != 0U)                         /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Set ErrorCode corresponding to a Non-Acknowledge */
    hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_AF;          /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  hfmpi2c->Mode = HAL_FMPI2C_MODE_NONE;                 /* 赋值或初始化 */
  hfmpi2c->XferISR = NULL;                              /* 赋值或初始化 */

  if (hfmpi2c->ErrorCode != HAL_FMPI2C_ERROR_NONE)      /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Call the corresponding callback to inform upper layer of End of Transfer */
    FMPI2C_ITError(hfmpi2c, hfmpi2c->ErrorCode);        /* 执行语句 */

    /* Call the Listen Complete callback, to inform upper layer of the end of Listen usecase */
    if (hfmpi2c->State == HAL_FMPI2C_STATE_LISTEN)      /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Call FMPI2C Listen complete process */
      FMPI2C_ITListenCplt(hfmpi2c, tmpITFlags);         /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (hfmpi2c->XferOptions != FMPI2C_NO_OPTION_FRAME)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* Call the Sequential Complete callback, to inform upper layer of the end of Transfer */
    FMPI2C_ITSlaveSeqCplt(hfmpi2c);                     /* 执行语句 */

    hfmpi2c->XferOptions = FMPI2C_NO_OPTION_FRAME;      /* 赋值或初始化 */
    hfmpi2c->State = HAL_FMPI2C_STATE_READY;            /* 赋值或初始化 */
    hfmpi2c->PreviousState = FMPI2C_STATE_NONE;         /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c);                              /* HAL辅助宏/函数 */

    /* Call the Listen Complete callback, to inform upper layer of the end of Listen usecase */
#if (USE_HAL_FMPI2C_REGISTER_CALLBACKS == 1)            /* 条件编译 */
    hfmpi2c->ListenCpltCallback(hfmpi2c);               /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_FMPI2C_ListenCpltCallback(hfmpi2c);             /* HAL库函数调用 */
#endif /* USE_HAL_FMPI2C_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
  /* Call the corresponding callback to inform upper layer of End of Transfer */
  else if (hfmpi2c->State == HAL_FMPI2C_STATE_BUSY_RX)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    hfmpi2c->State = HAL_FMPI2C_STATE_READY;            /* 赋值或初始化 */
    hfmpi2c->PreviousState = FMPI2C_STATE_NONE;         /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c);                              /* HAL辅助宏/函数 */

    /* Call the corresponding callback to inform upper layer of End of Transfer */
#if (USE_HAL_FMPI2C_REGISTER_CALLBACKS == 1)            /* 条件编译 */
    hfmpi2c->SlaveRxCpltCallback(hfmpi2c);              /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_FMPI2C_SlaveRxCpltCallback(hfmpi2c);            /* HAL库函数调用 */
#endif /* USE_HAL_FMPI2C_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    hfmpi2c->State = HAL_FMPI2C_STATE_READY;            /* 赋值或初始化 */
    hfmpi2c->PreviousState = FMPI2C_STATE_NONE;         /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c);                              /* HAL辅助宏/函数 */

    /* Call the corresponding callback to inform upper layer of End of Transfer */
#if (USE_HAL_FMPI2C_REGISTER_CALLBACKS == 1)            /* 条件编译 */
    hfmpi2c->SlaveTxCpltCallback(hfmpi2c);              /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_FMPI2C_SlaveTxCpltCallback(hfmpi2c);            /* HAL库函数调用 */
#endif /* USE_HAL_FMPI2C_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  FMPI2C Listen complete process.
  * @param  hfmpi2c FMPI2C handle.
  * @param  ITFlags Interrupt flags to handle.
  * @retval None
  */
static void FMPI2C_ITListenCplt(FMPI2C_HandleTypeDef *hfmpi2c, uint32_t ITFlags)  /* 静态函数 */
{                                                       /* 代码块开始 */
  /* Reset handle parameters */
  hfmpi2c->XferOptions = FMPI2C_NO_OPTION_FRAME;        /* 赋值或初始化 */
  hfmpi2c->PreviousState = FMPI2C_STATE_NONE;           /* 赋值或初始化 */
  hfmpi2c->State = HAL_FMPI2C_STATE_READY;              /* 赋值或初始化 */
  hfmpi2c->Mode = HAL_FMPI2C_MODE_NONE;                 /* 赋值或初始化 */
  hfmpi2c->XferISR = NULL;                              /* 赋值或初始化 */

  /* Store Last receive data if any */
  if (FMPI2C_CHECK_FLAG(ITFlags, FMPI2C_FLAG_RXNE) != RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Read data from RXDR */
    *hfmpi2c->pBuffPtr = (uint8_t)hfmpi2c->Instance->RXDR;  /* 赋值或初始化 */

    /* Increment Buffer pointer */
    hfmpi2c->pBuffPtr++;                                /* 执行语句 */

    if ((hfmpi2c->XferSize > 0U))                       /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpi2c->XferSize--;                              /* 执行语句 */
      hfmpi2c->XferCount--;                             /* 执行语句 */

      /* Set ErrorCode corresponding to a Non-Acknowledge */
      hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_AF;        /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Disable all Interrupts*/
  FMPI2C_Disable_IRQ(hfmpi2c, FMPI2C_XFER_LISTEN_IT | FMPI2C_XFER_RX_IT | FMPI2C_XFER_TX_IT);  /* 执行语句 */

  /* Clear NACK Flag */
  __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_AF);     /* HAL辅助宏/函数 */

  /* Process Unlocked */
  __HAL_UNLOCK(hfmpi2c);                                /* HAL辅助宏/函数 */

  /* Call the Listen Complete callback, to inform upper layer of the end of Listen usecase */
#if (USE_HAL_FMPI2C_REGISTER_CALLBACKS == 1)            /* 条件编译 */
  hfmpi2c->ListenCpltCallback(hfmpi2c);                 /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_FMPI2C_ListenCpltCallback(hfmpi2c);               /* HAL库函数调用 */
#endif /* USE_HAL_FMPI2C_REGISTER_CALLBACKS */
}                                                       /* 代码块结束 */

/**
  * @brief  FMPI2C interrupts error process.
  * @param  hfmpi2c FMPI2C handle.
  * @param  ErrorCode Error code to handle.
  * @retval None
  */
static void FMPI2C_ITError(FMPI2C_HandleTypeDef *hfmpi2c, uint32_t ErrorCode)  /* 静态函数 */
{                                                       /* 代码块开始 */
  HAL_FMPI2C_StateTypeDef tmpstate = hfmpi2c->State;    /* HAL库函数调用 */
  uint32_t tmppreviousstate;                            /* 32位无符号变量/函数 */

  /* Reset handle parameters */
  hfmpi2c->Mode          = HAL_FMPI2C_MODE_NONE;        /* 赋值或初始化 */
  hfmpi2c->XferOptions   = FMPI2C_NO_OPTION_FRAME;      /* 赋值或初始化 */
  hfmpi2c->XferCount     = 0U;                          /* 赋值或初始化 */

  /* Set new error code */
  hfmpi2c->ErrorCode |= ErrorCode;                      /* 赋值或初始化 */

  /* Disable Interrupts */
  if ((tmpstate == HAL_FMPI2C_STATE_LISTEN)         ||  /* 条件判断 */
      (tmpstate == HAL_FMPI2C_STATE_BUSY_TX_LISTEN) ||  /* 源代码 */
      (tmpstate == HAL_FMPI2C_STATE_BUSY_RX_LISTEN))    /* 源代码 */
  {                                                     /* 代码块开始 */
    /* Disable all interrupts, except interrupts related to LISTEN state */
    FMPI2C_Disable_IRQ(hfmpi2c, FMPI2C_XFER_RX_IT | FMPI2C_XFER_TX_IT);  /* 执行语句 */

    /* keep HAL_FMPI2C_STATE_LISTEN if set */
    hfmpi2c->State         = HAL_FMPI2C_STATE_LISTEN;   /* 赋值或初始化 */
    hfmpi2c->XferISR       = FMPI2C_Slave_ISR_IT;       /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Disable all interrupts */
    FMPI2C_Disable_IRQ(hfmpi2c, FMPI2C_XFER_LISTEN_IT | FMPI2C_XFER_RX_IT | FMPI2C_XFER_TX_IT);  /* 执行语句 */

    /* If state is an abort treatment on going, don't change state */
    /* This change will be do later */
    if (hfmpi2c->State != HAL_FMPI2C_STATE_ABORT)       /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set HAL_FMPI2C_STATE_READY */
      hfmpi2c->State         = HAL_FMPI2C_STATE_READY;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    hfmpi2c->XferISR       = NULL;                      /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Abort DMA TX transfer if any */
  tmppreviousstate = hfmpi2c->PreviousState;            /* 赋值或初始化 */
  if ((hfmpi2c->hdmatx != NULL) && ((tmppreviousstate == FMPI2C_STATE_MASTER_BUSY_TX) || \
                                 (tmppreviousstate == FMPI2C_STATE_SLAVE_BUSY_TX)))  /* 源代码 */
  {                                                     /* 代码块开始 */
    if ((hfmpi2c->Instance->CR1 & FMPI2C_CR1_TXDMAEN) == FMPI2C_CR1_TXDMAEN)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpi2c->Instance->CR1 &= ~FMPI2C_CR1_TXDMAEN;    /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    if (HAL_DMA_GetState(hfmpi2c->hdmatx) != HAL_DMA_STATE_READY)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the FMPI2C DMA Abort callback :
       will lead to call HAL_FMPI2C_ErrorCallback() at end of DMA abort procedure */
      hfmpi2c->hdmatx->XferAbortCallback = FMPI2C_DMAAbort;  /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hfmpi2c);                            /* HAL辅助宏/函数 */

      /* Abort DMA TX */
      if (HAL_DMA_Abort_IT(hfmpi2c->hdmatx) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Call Directly XferAbortCallback function in case of error */
        hfmpi2c->hdmatx->XferAbortCallback(hfmpi2c->hdmatx);  /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      FMPI2C_TreatErrorCallback(hfmpi2c);               /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /* Abort DMA RX transfer if any */
  else if ((hfmpi2c->hdmarx != NULL) && ((tmppreviousstate == FMPI2C_STATE_MASTER_BUSY_RX) || \
                                      (tmppreviousstate == FMPI2C_STATE_SLAVE_BUSY_RX)))  /* 源代码 */
  {                                                     /* 代码块开始 */
    if ((hfmpi2c->Instance->CR1 & FMPI2C_CR1_RXDMAEN) == FMPI2C_CR1_RXDMAEN)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpi2c->Instance->CR1 &= ~FMPI2C_CR1_RXDMAEN;    /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    if (HAL_DMA_GetState(hfmpi2c->hdmarx) != HAL_DMA_STATE_READY)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the FMPI2C DMA Abort callback :
        will lead to call HAL_FMPI2C_ErrorCallback() at end of DMA abort procedure */
      hfmpi2c->hdmarx->XferAbortCallback = FMPI2C_DMAAbort;  /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hfmpi2c);                            /* HAL辅助宏/函数 */

      /* Abort DMA RX */
      if (HAL_DMA_Abort_IT(hfmpi2c->hdmarx) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Call Directly hfmpi2c->hdmarx->XferAbortCallback function in case of error */
        hfmpi2c->hdmarx->XferAbortCallback(hfmpi2c->hdmarx);  /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      FMPI2C_TreatErrorCallback(hfmpi2c);               /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    FMPI2C_TreatErrorCallback(hfmpi2c);                 /* 执行语句 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  FMPI2C Error callback treatment.
  * @param  hfmpi2c FMPI2C handle.
  * @retval None
  */
static void FMPI2C_TreatErrorCallback(FMPI2C_HandleTypeDef *hfmpi2c)  /* 静态函数 */
{                                                       /* 代码块开始 */
  if (hfmpi2c->State == HAL_FMPI2C_STATE_ABORT)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    hfmpi2c->State = HAL_FMPI2C_STATE_READY;            /* 赋值或初始化 */
    hfmpi2c->PreviousState = FMPI2C_STATE_NONE;         /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c);                              /* HAL辅助宏/函数 */

    /* Call the corresponding callback to inform upper layer of End of Transfer */
#if (USE_HAL_FMPI2C_REGISTER_CALLBACKS == 1)            /* 条件编译 */
    hfmpi2c->AbortCpltCallback(hfmpi2c);                /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_FMPI2C_AbortCpltCallback(hfmpi2c);              /* HAL库函数调用 */
#endif /* USE_HAL_FMPI2C_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    hfmpi2c->PreviousState = FMPI2C_STATE_NONE;         /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c);                              /* HAL辅助宏/函数 */

    /* Call the corresponding callback to inform upper layer of End of Transfer */
#if (USE_HAL_FMPI2C_REGISTER_CALLBACKS == 1)            /* 条件编译 */
    hfmpi2c->ErrorCallback(hfmpi2c);                    /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_FMPI2C_ErrorCallback(hfmpi2c);                  /* HAL库函数调用 */
#endif /* USE_HAL_FMPI2C_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  FMPI2C Tx data register flush process.
  * @param  hfmpi2c FMPI2C handle.
  * @retval None
  */
static void FMPI2C_Flush_TXDR(FMPI2C_HandleTypeDef *hfmpi2c)  /* 静态函数 */
{                                                       /* 代码块开始 */
  /* If a pending TXIS flag is set */
  /* Write a dummy data in TXDR to clear it */
  if (__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_TXIS) != RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hfmpi2c->Instance->TXDR = 0x00U;                    /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Flush TX register if not empty */
  if (__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_TXE) == RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_TXE);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA FMPI2C master transmit process complete callback.
  * @param  hdma DMA handle
  * @retval None
  */
static void FMPI2C_DMAMasterTransmitCplt(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  FMPI2C_HandleTypeDef *hfmpi2c = (FMPI2C_HandleTypeDef *)(((DMA_HandleTypeDef *)hdma)->Parent); /* Derogation MISRAC2012-Rule-11.5 */

  /* Disable DMA Request */
  hfmpi2c->Instance->CR1 &= ~FMPI2C_CR1_TXDMAEN;        /* 赋值或初始化 */

  /* If last transfer, enable STOP interrupt */
  if (hfmpi2c->XferCount == 0U)                         /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable STOP interrupt */
    FMPI2C_Enable_IRQ(hfmpi2c, FMPI2C_XFER_CPLT_IT);    /* 执行语句 */
  }                                                     /* 代码块结束 */
  /* else prepare a new DMA transfer and enable TCReload interrupt */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update Buffer pointer */
    hfmpi2c->pBuffPtr += hfmpi2c->XferSize;             /* 赋值或初始化 */

    /* Set the XferSize to transfer */
    if (hfmpi2c->XferCount > MAX_NBYTE_SIZE)            /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpi2c->XferSize = MAX_NBYTE_SIZE;               /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hfmpi2c->XferSize = hfmpi2c->XferCount;           /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Enable the DMA stream */
    if (HAL_DMA_Start_IT(hfmpi2c->hdmatx, (uint32_t)hfmpi2c->pBuffPtr, (uint32_t)&hfmpi2c->Instance->TXDR, hfmpi2c->XferSize) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Call the corresponding callback to inform upper layer of End of Transfer */
      FMPI2C_ITError(hfmpi2c, HAL_FMPI2C_ERROR_DMA);    /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Enable TC interrupts */
      FMPI2C_Enable_IRQ(hfmpi2c, FMPI2C_XFER_RELOAD_IT);  /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA FMPI2C slave transmit process complete callback.
  * @param  hdma DMA handle
  * @retval None
  */
static void FMPI2C_DMASlaveTransmitCplt(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  FMPI2C_HandleTypeDef *hfmpi2c = (FMPI2C_HandleTypeDef *)(((DMA_HandleTypeDef *)hdma)->Parent); /* Derogation MISRAC2012-Rule-11.5 */
  uint32_t tmpoptions = hfmpi2c->XferOptions;           /* 32位无符号变量/函数 */

  if ((tmpoptions == FMPI2C_NEXT_FRAME) || (tmpoptions == FMPI2C_FIRST_FRAME))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable DMA Request */
    hfmpi2c->Instance->CR1 &= ~FMPI2C_CR1_TXDMAEN;      /* 赋值或初始化 */

    /* Last Byte is Transmitted */
    /* Call FMPI2C Slave Sequential complete process */
    FMPI2C_ITSlaveSeqCplt(hfmpi2c);                     /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* No specific action, Master fully manage the generation of STOP condition */
    /* Mean that this generation can arrive at any time, at the end or during DMA process */
    /* So STOP condition should be manage through Interrupt treatment */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief DMA FMPI2C master receive process complete callback.
  * @param  hdma DMA handle
  * @retval None
  */
static void FMPI2C_DMAMasterReceiveCplt(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  FMPI2C_HandleTypeDef *hfmpi2c = (FMPI2C_HandleTypeDef *)(((DMA_HandleTypeDef *)hdma)->Parent); /* Derogation MISRAC2012-Rule-11.5 */

  /* Disable DMA Request */
  hfmpi2c->Instance->CR1 &= ~FMPI2C_CR1_RXDMAEN;        /* 赋值或初始化 */

  /* If last transfer, enable STOP interrupt */
  if (hfmpi2c->XferCount == 0U)                         /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable STOP interrupt */
    FMPI2C_Enable_IRQ(hfmpi2c, FMPI2C_XFER_CPLT_IT);    /* 执行语句 */
  }                                                     /* 代码块结束 */
  /* else prepare a new DMA transfer and enable TCReload interrupt */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update Buffer pointer */
    hfmpi2c->pBuffPtr += hfmpi2c->XferSize;             /* 赋值或初始化 */

    /* Set the XferSize to transfer */
    if (hfmpi2c->XferCount > MAX_NBYTE_SIZE)            /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpi2c->XferSize = MAX_NBYTE_SIZE;               /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hfmpi2c->XferSize = hfmpi2c->XferCount;           /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Enable the DMA stream */
    if (HAL_DMA_Start_IT(hfmpi2c->hdmarx, (uint32_t)&hfmpi2c->Instance->RXDR, (uint32_t)hfmpi2c->pBuffPtr, hfmpi2c->XferSize) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Call the corresponding callback to inform upper layer of End of Transfer */
      FMPI2C_ITError(hfmpi2c, HAL_FMPI2C_ERROR_DMA);    /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Enable TC interrupts */
      FMPI2C_Enable_IRQ(hfmpi2c, FMPI2C_XFER_RELOAD_IT);  /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA FMPI2C slave receive process complete callback.
  * @param  hdma DMA handle
  * @retval None
  */
static void FMPI2C_DMASlaveReceiveCplt(DMA_HandleTypeDef *hdma)  /* 静态函数 */
{                                                       /* 代码块开始 */
  FMPI2C_HandleTypeDef *hfmpi2c = (FMPI2C_HandleTypeDef *)(((DMA_HandleTypeDef *)hdma)->Parent); /* Derogation MISRAC2012-Rule-11.5 */
  uint32_t tmpoptions = hfmpi2c->XferOptions;           /* 32位无符号变量/函数 */

  if ((__HAL_DMA_GET_COUNTER(hfmpi2c->hdmarx) == 0U) && \
      (tmpoptions != FMPI2C_NO_OPTION_FRAME))           /* 源代码 */
  {                                                     /* 代码块开始 */
    /* Disable DMA Request */
    hfmpi2c->Instance->CR1 &= ~FMPI2C_CR1_RXDMAEN;      /* 赋值或初始化 */

    /* Call FMPI2C Slave Sequential complete process */
    FMPI2C_ITSlaveSeqCplt(hfmpi2c);                     /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* No specific action, Master fully manage the generation of STOP condition */
    /* Mean that this generation can arrive at any time, at the end or during DMA process */
    /* So STOP condition should be manage through Interrupt treatment */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA FMPI2C communication error callback.
  * @param hdma DMA handle
  * @retval None
  */
static void FMPI2C_DMAError(DMA_HandleTypeDef *hdma)    /* 静态函数 */
{                                                       /* 代码块开始 */
  uint32_t treatdmaerror = 0U;                          /* 32位无符号变量/函数 */
  FMPI2C_HandleTypeDef *hfmpi2c = (FMPI2C_HandleTypeDef *)(((DMA_HandleTypeDef *)hdma)->Parent); /* Derogation MISRAC2012-Rule-11.5 */

  if (hfmpi2c->hdmatx != NULL)                          /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (__HAL_DMA_GET_COUNTER(hfmpi2c->hdmatx) == 0U)   /* 条件判断 */
    {                                                   /* 代码块开始 */
      treatdmaerror = 1U;                               /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  if (hfmpi2c->hdmarx != NULL)                          /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (__HAL_DMA_GET_COUNTER(hfmpi2c->hdmarx) == 0U)   /* 条件判断 */
    {                                                   /* 代码块开始 */
      treatdmaerror = 1U;                               /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Check if a FIFO error is detected, if true normal use case, so no specific action to perform */
  if (!((HAL_DMA_GetError(hdma) == HAL_DMA_ERROR_FE)) && (treatdmaerror != 0U))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable Acknowledge */
    hfmpi2c->Instance->CR2 |= FMPI2C_CR2_NACK;          /* 赋值或初始化 */

    /* Call the corresponding callback to inform upper layer of End of Transfer */
    FMPI2C_ITError(hfmpi2c, HAL_FMPI2C_ERROR_DMA);      /* 执行语句 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief DMA FMPI2C communication abort callback
  *        (To be called at end of DMA Abort procedure).
  * @param hdma DMA handle.
  * @retval None
  */
static void FMPI2C_DMAAbort(DMA_HandleTypeDef *hdma)    /* 静态函数 */
{                                                       /* 代码块开始 */
  FMPI2C_HandleTypeDef *hfmpi2c = (FMPI2C_HandleTypeDef *)(((DMA_HandleTypeDef *)hdma)->Parent); /* Derogation MISRAC2012-Rule-11.5 */

  /* Reset AbortCpltCallback */
  if (hfmpi2c->hdmatx != NULL)                          /* 条件判断 */
  {                                                     /* 代码块开始 */
    hfmpi2c->hdmatx->XferAbortCallback = NULL;          /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  if (hfmpi2c->hdmarx != NULL)                          /* 条件判断 */
  {                                                     /* 代码块开始 */
    hfmpi2c->hdmarx->XferAbortCallback = NULL;          /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  FMPI2C_TreatErrorCallback(hfmpi2c);                   /* 执行语句 */
}                                                       /* 代码块结束 */

/**
  * @brief  This function handles FMPI2C Communication Timeout.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  Flag Specifies the FMPI2C flag to check.
  * @param  Status The new Flag status (SET or RESET).
  * @param  Timeout Timeout duration
  * @param  Tickstart Tick start value
  * @retval HAL status
  */
static HAL_StatusTypeDef FMPI2C_WaitOnFlagUntilTimeout(FMPI2C_HandleTypeDef *hfmpi2c, uint32_t Flag, FlagStatus Status,  /* 静态变量/函数 */
                                                    uint32_t Timeout, uint32_t Tickstart)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  while (__HAL_FMPI2C_GET_FLAG(hfmpi2c, Flag) == Status)  /* while循环 */
  {                                                     /* 代码块开始 */
    /* Check for the Timeout */
    if (Timeout != HAL_MAX_DELAY)                       /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (((HAL_GetTick() - Tickstart) > Timeout) || (Timeout == 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_TIMEOUT;  /* 赋值或初始化 */
        hfmpi2c->State = HAL_FMPI2C_STATE_READY;        /* 赋值或初始化 */
        hfmpi2c->Mode = HAL_FMPI2C_MODE_NONE;           /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hfmpi2c);                          /* HAL辅助宏/函数 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  This function handles FMPI2C Communication Timeout for specific usage of TXIS flag.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  Timeout Timeout duration
  * @param  Tickstart Tick start value
  * @retval HAL status
  */
static HAL_StatusTypeDef FMPI2C_WaitOnTXISFlagUntilTimeout(FMPI2C_HandleTypeDef *hfmpi2c, uint32_t Timeout, uint32_t Tickstart)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  while (__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_TXIS) == RESET)  /* while循环 */
  {                                                     /* 代码块开始 */
    /* Check if a NACK is detected */
    if (FMPI2C_IsAcknowledgeFailed(hfmpi2c, Timeout, Tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Check for the Timeout */
    if (Timeout != HAL_MAX_DELAY)                       /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (((HAL_GetTick() - Tickstart) > Timeout) || (Timeout == 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_TIMEOUT;  /* 赋值或初始化 */
        hfmpi2c->State = HAL_FMPI2C_STATE_READY;        /* 赋值或初始化 */
        hfmpi2c->Mode = HAL_FMPI2C_MODE_NONE;           /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hfmpi2c);                          /* HAL辅助宏/函数 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  This function handles FMPI2C Communication Timeout for specific usage of STOP flag.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  Timeout Timeout duration
  * @param  Tickstart Tick start value
  * @retval HAL status
  */
static HAL_StatusTypeDef FMPI2C_WaitOnSTOPFlagUntilTimeout(FMPI2C_HandleTypeDef *hfmpi2c, uint32_t Timeout, uint32_t Tickstart)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  while (__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF) == RESET)  /* while循环 */
  {                                                     /* 代码块开始 */
    /* Check if a NACK is detected */
    if (FMPI2C_IsAcknowledgeFailed(hfmpi2c, Timeout, Tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Check for the Timeout */
    if (((HAL_GetTick() - Tickstart) > Timeout) || (Timeout == 0U))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_TIMEOUT;   /* 赋值或初始化 */
      hfmpi2c->State = HAL_FMPI2C_STATE_READY;          /* 赋值或初始化 */
      hfmpi2c->Mode = HAL_FMPI2C_MODE_NONE;             /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hfmpi2c);                            /* HAL辅助宏/函数 */

      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  This function handles FMPI2C Communication Timeout for specific usage of RXNE flag.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  Timeout Timeout duration
  * @param  Tickstart Tick start value
  * @retval HAL status
  */
static HAL_StatusTypeDef FMPI2C_WaitOnRXNEFlagUntilTimeout(FMPI2C_HandleTypeDef *hfmpi2c, uint32_t Timeout, uint32_t Tickstart)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  while (__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_RXNE) == RESET)  /* while循环 */
  {                                                     /* 代码块开始 */
    /* Check if a NACK is detected */
    if (FMPI2C_IsAcknowledgeFailed(hfmpi2c, Timeout, Tickstart) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /* Check if a STOPF is detected */
    if (__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF) == SET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Check if an RXNE is pending */
      /* Store Last receive data if any */
      if ((__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_RXNE) == SET) && (hfmpi2c->XferSize > 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Return HAL_OK */
        /* The Reading of data from RXDR will be done in caller function */
        return HAL_OK;                                  /* 返回值 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Clear STOP Flag */
        __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF);  /* HAL辅助宏/函数 */

        /* Clear Configuration Register 2 */
        FMPI2C_RESET_CR2(hfmpi2c);                      /* 执行语句 */

        hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_NONE;     /* 赋值或初始化 */
        hfmpi2c->State = HAL_FMPI2C_STATE_READY;        /* 赋值或初始化 */
        hfmpi2c->Mode = HAL_FMPI2C_MODE_NONE;           /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hfmpi2c);                          /* HAL辅助宏/函数 */

        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Check for the Timeout */
    if (((HAL_GetTick() - Tickstart) > Timeout) || (Timeout == 0U))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_TIMEOUT;   /* 赋值或初始化 */
      hfmpi2c->State = HAL_FMPI2C_STATE_READY;          /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hfmpi2c);                            /* HAL辅助宏/函数 */

      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  This function handles Acknowledge failed detection during an FMPI2C Communication.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  Timeout Timeout duration
  * @param  Tickstart Tick start value
  * @retval HAL status
  */
static HAL_StatusTypeDef FMPI2C_IsAcknowledgeFailed(FMPI2C_HandleTypeDef *hfmpi2c, uint32_t Timeout, uint32_t Tickstart)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  if (__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_AF) == SET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Wait until STOP Flag is reset */
    /* AutoEnd should be initiate after AF */
    while (__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF) == RESET)  /* while循环 */
    {                                                   /* 代码块开始 */
      /* Check for the Timeout */
      if (Timeout != HAL_MAX_DELAY)                     /* 条件判断 */
      {                                                 /* 代码块开始 */
        if (((HAL_GetTick() - Tickstart) > Timeout) || (Timeout == 0U))  /* 条件判断 */
        {                                               /* 代码块开始 */
          hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_TIMEOUT;  /* 赋值或初始化 */
          hfmpi2c->State = HAL_FMPI2C_STATE_READY;      /* 赋值或初始化 */
          hfmpi2c->Mode = HAL_FMPI2C_MODE_NONE;         /* 赋值或初始化 */

          /* Process Unlocked */
          __HAL_UNLOCK(hfmpi2c);                        /* HAL辅助宏/函数 */

          return HAL_ERROR;                             /* 返回值 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Clear NACKF Flag */
    __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_AF);   /* HAL辅助宏/函数 */

    /* Clear STOP Flag */
    __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF);  /* HAL辅助宏/函数 */

    /* Flush TX register */
    FMPI2C_Flush_TXDR(hfmpi2c);                         /* 执行语句 */

    /* Clear Configuration Register 2 */
    FMPI2C_RESET_CR2(hfmpi2c);                          /* 执行语句 */

    hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_AF;          /* 赋值或初始化 */
    hfmpi2c->State = HAL_FMPI2C_STATE_READY;            /* 赋值或初始化 */
    hfmpi2c->Mode = HAL_FMPI2C_MODE_NONE;               /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c);                              /* HAL辅助宏/函数 */

    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Handles FMPI2Cx communication when starting transfer or during transfer (TC or TCR flag are set).
  * @param  hfmpi2c FMPI2C handle.
  * @param  DevAddress Specifies the slave address to be programmed.
  * @param  Size Specifies the number of bytes to be programmed.
  *   This parameter must be a value between 0 and 255.
  * @param  Mode New state of the FMPI2C START condition generation.
  *   This parameter can be one of the following values:
  *     @arg @ref FMPI2C_RELOAD_MODE Enable Reload mode .
  *     @arg @ref FMPI2C_AUTOEND_MODE Enable Automatic end mode.
  *     @arg @ref FMPI2C_SOFTEND_MODE Enable Software end mode.
  * @param  Request New state of the FMPI2C START condition generation.
  *   This parameter can be one of the following values:
  *     @arg @ref FMPI2C_NO_STARTSTOP Don't Generate stop and start condition.
  *     @arg @ref FMPI2C_GENERATE_STOP Generate stop condition (Size should be set to 0).
  *     @arg @ref FMPI2C_GENERATE_START_READ Generate Restart for read request.
  *     @arg @ref FMPI2C_GENERATE_START_WRITE Generate Restart for write request.
  * @retval None
  */
static void FMPI2C_TransferConfig(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t DevAddress, uint8_t Size, uint32_t Mode,  /* 静态函数 */
                               uint32_t Request)        /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_FMPI2C_ALL_INSTANCE(hfmpi2c->Instance));  /* 执行语句 */
  assert_param(IS_TRANSFER_MODE(Mode));                 /* 执行语句 */
  assert_param(IS_TRANSFER_REQUEST(Request));           /* 执行语句 */

  /* update CR2 register */
  MODIFY_REG(hfmpi2c->Instance->CR2,                    /* 列表续行 */
             ((FMPI2C_CR2_SADD | FMPI2C_CR2_NBYTES | FMPI2C_CR2_RELOAD | FMPI2C_CR2_AUTOEND | \
               (FMPI2C_CR2_RD_WRN & (uint32_t)(Request >> (31U - FMPI2C_CR2_RD_WRN_Pos))) | FMPI2C_CR2_START | FMPI2C_CR2_STOP)), \
             (uint32_t)(((uint32_t)DevAddress & FMPI2C_CR2_SADD) |  /* 源代码 */
                        (((uint32_t)Size << FMPI2C_CR2_NBYTES_Pos) & FMPI2C_CR2_NBYTES) | (uint32_t)Mode | (uint32_t)Request));  /* 执行语句 */
}                                                       /* 代码块结束 */

/**
  * @brief  Manage the enabling of Interrupts.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  InterruptRequest Value of @ref FMPI2C_Interrupt_configuration_definition.
  * @retval None
  */
static void FMPI2C_Enable_IRQ(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t InterruptRequest)  /* 静态函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpisr = 0U;                                 /* 32位无符号变量/函数 */

  if ((hfmpi2c->XferISR == FMPI2C_Master_ISR_DMA) || \
      (hfmpi2c->XferISR == FMPI2C_Slave_ISR_DMA))       /* 源代码 */
  {                                                     /* 代码块开始 */
    if ((InterruptRequest & FMPI2C_XFER_LISTEN_IT) == FMPI2C_XFER_LISTEN_IT)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable ERR, STOP, NACK and ADDR interrupts */
      tmpisr |= FMPI2C_IT_ADDRI | FMPI2C_IT_STOPI | FMPI2C_IT_NACKI | FMPI2C_IT_ERRI;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    if (InterruptRequest == FMPI2C_XFER_ERROR_IT)       /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable ERR and NACK interrupts */
      tmpisr |= FMPI2C_IT_ERRI | FMPI2C_IT_NACKI;       /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    if (InterruptRequest == FMPI2C_XFER_CPLT_IT)        /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable STOP interrupts */
      tmpisr |= (FMPI2C_IT_STOPI | FMPI2C_IT_TCI);      /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    if (InterruptRequest == FMPI2C_XFER_RELOAD_IT)      /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable TC interrupts */
      tmpisr |= FMPI2C_IT_TCI;                          /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    if ((InterruptRequest & FMPI2C_XFER_LISTEN_IT) == FMPI2C_XFER_LISTEN_IT)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable ERR, STOP, NACK, and ADDR interrupts */
      tmpisr |= FMPI2C_IT_ADDRI | FMPI2C_IT_STOPI | FMPI2C_IT_NACKI | FMPI2C_IT_ERRI;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    if ((InterruptRequest & FMPI2C_XFER_TX_IT) == FMPI2C_XFER_TX_IT)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable ERR, TC, STOP, NACK and RXI interrupts */
      tmpisr |= FMPI2C_IT_ERRI | FMPI2C_IT_TCI | FMPI2C_IT_STOPI | FMPI2C_IT_NACKI | FMPI2C_IT_TXI;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    if ((InterruptRequest & FMPI2C_XFER_RX_IT) == FMPI2C_XFER_RX_IT)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable ERR, TC, STOP, NACK and TXI interrupts */
      tmpisr |= FMPI2C_IT_ERRI | FMPI2C_IT_TCI | FMPI2C_IT_STOPI | FMPI2C_IT_NACKI | FMPI2C_IT_RXI;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    if (InterruptRequest == FMPI2C_XFER_CPLT_IT)        /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Enable STOP interrupts */
      tmpisr |= FMPI2C_IT_STOPI;                        /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Enable interrupts only at the end */
  /* to avoid the risk of FMPI2C interrupt handle execution before */
  /* all interrupts requested done */
  __HAL_FMPI2C_ENABLE_IT(hfmpi2c, tmpisr);              /* HAL辅助宏/函数 */
}                                                       /* 代码块结束 */

/**
  * @brief  Manage the disabling of Interrupts.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  InterruptRequest Value of @ref FMPI2C_Interrupt_configuration_definition.
  * @retval None
  */
static void FMPI2C_Disable_IRQ(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t InterruptRequest)  /* 静态函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpisr = 0U;                                 /* 32位无符号变量/函数 */

  if ((InterruptRequest & FMPI2C_XFER_TX_IT) == FMPI2C_XFER_TX_IT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable TC and TXI interrupts */
    tmpisr |= FMPI2C_IT_TCI | FMPI2C_IT_TXI;            /* 赋值或初始化 */

    if (((uint32_t)hfmpi2c->State & (uint32_t)HAL_FMPI2C_STATE_LISTEN) != (uint32_t)HAL_FMPI2C_STATE_LISTEN)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable NACK and STOP interrupts */
      tmpisr |= FMPI2C_IT_STOPI | FMPI2C_IT_NACKI | FMPI2C_IT_ERRI;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  if ((InterruptRequest & FMPI2C_XFER_RX_IT) == FMPI2C_XFER_RX_IT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable TC and RXI interrupts */
    tmpisr |= FMPI2C_IT_TCI | FMPI2C_IT_RXI;            /* 赋值或初始化 */

    if (((uint32_t)hfmpi2c->State & (uint32_t)HAL_FMPI2C_STATE_LISTEN) != (uint32_t)HAL_FMPI2C_STATE_LISTEN)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable NACK and STOP interrupts */
      tmpisr |= FMPI2C_IT_STOPI | FMPI2C_IT_NACKI | FMPI2C_IT_ERRI;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  if ((InterruptRequest & FMPI2C_XFER_LISTEN_IT) == FMPI2C_XFER_LISTEN_IT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable ADDR, NACK and STOP interrupts */
    tmpisr |= FMPI2C_IT_ADDRI | FMPI2C_IT_STOPI | FMPI2C_IT_NACKI | FMPI2C_IT_ERRI;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if (InterruptRequest == FMPI2C_XFER_ERROR_IT)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable ERR and NACK interrupts */
    tmpisr |= FMPI2C_IT_ERRI | FMPI2C_IT_NACKI;         /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if (InterruptRequest == FMPI2C_XFER_CPLT_IT)          /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable STOP interrupts */
    tmpisr |= FMPI2C_IT_STOPI;                          /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if (InterruptRequest == FMPI2C_XFER_RELOAD_IT)        /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable TC interrupts */
    tmpisr |= FMPI2C_IT_TCI;                            /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Disable interrupts only at the end */
  /* to avoid a breaking situation like at "t" time */
  /* all disable interrupts request are not done */
  __HAL_FMPI2C_DISABLE_IT(hfmpi2c, tmpisr);             /* HAL辅助宏/函数 */
}                                                       /* 代码块结束 */

/**
  * @brief  Convert FMPI2Cx OTHER_xxx XferOptions to functional XferOptions.
  * @param  hfmpi2c FMPI2C handle.
  * @retval None
  */
static void FMPI2C_ConvertOtherXferOptions(FMPI2C_HandleTypeDef *hfmpi2c)  /* 静态函数 */
{                                                       /* 代码块开始 */
  /* if user set XferOptions to FMPI2C_OTHER_FRAME            */
  /* it request implicitly to generate a restart condition */
  /* set XferOptions to FMPI2C_FIRST_FRAME                    */
  if (hfmpi2c->XferOptions == FMPI2C_OTHER_FRAME)       /* 条件判断 */
  {                                                     /* 代码块开始 */
    hfmpi2c->XferOptions = FMPI2C_FIRST_FRAME;          /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  /* else if user set XferOptions to FMPI2C_OTHER_AND_LAST_FRAME */
  /* it request implicitly to generate a restart condition    */
  /* then generate a stop condition at the end of transfer    */
  /* set XferOptions to FMPI2C_FIRST_AND_LAST_FRAME              */
  else if (hfmpi2c->XferOptions == FMPI2C_OTHER_AND_LAST_FRAME)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    hfmpi2c->XferOptions = FMPI2C_FIRST_AND_LAST_FRAME;  /* 赋值或初始化 */
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
#endif /* HAL_FMPI2C_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
