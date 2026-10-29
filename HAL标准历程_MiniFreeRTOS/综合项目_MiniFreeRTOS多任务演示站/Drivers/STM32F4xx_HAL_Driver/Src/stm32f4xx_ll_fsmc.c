/* 文件: stm32f4xx_ll_fsmc.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_ll_fsmc.c
  * @author  MCD Application Team
  * @brief   FSMC Low Layer HAL module driver.
  *    
  *          This file provides firmware functions to manage the following 
  *          functionalities of the Flexible Static Memory Controller (FSMC) peripheral memories:
  *           + Initialization/de-initialization functions
  *           + Peripheral Control functions 
  *           + Peripheral State functions
  *         
  @verbatim
  ==============================================================================
                        ##### FSMC peripheral features #####
  ==============================================================================                  
    [..] The Flexible static memory controller (FSMC) includes two memory controllers:
         (+) The NOR/PSRAM memory controller
         (+) The NAND/PC Card memory controller

    [..] The FSMC functional block makes the interface with synchronous and asynchronous static
         memories, SDRAM memories, and 16-bit PC memory cards. Its main purposes are:
         (+) to translate AHB transactions into the appropriate external device protocol.
         (+) to meet the access time requirements of the external memory devices.

    [..] All external memories share the addresses, data and control signals with the controller.
         Each external device is accessed by means of a unique Chip Select. The FSMC performs
         only one access at a time to an external device.
         The main features of the FSMC controller are the following:
          (+) Interface with static-memory mapped devices including:
             (++) Static random access memory (SRAM).
             (++) Read-only memory (ROM).
             (++) NOR Flash memory/OneNAND Flash memory.
             (++) PSRAM (4 memory banks).
             (++) 16-bit PC Card compatible devices.
             (++) Two banks of NAND Flash memory with ECC hardware to check up to 8 Kbytes of
                  data.
          (+) Independent Chip Select control for each memory bank.
          (+) Independent configuration for each memory bank.          

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

/** @defgroup FSMC_LL  FSMC Low Layer
  * @brief FSMC driver modules
  * @{
  */

#if defined (HAL_SRAM_MODULE_ENABLED) || defined(HAL_NOR_MODULE_ENABLED) || defined(HAL_NAND_MODULE_ENABLED) || defined(HAL_PCCARD_MODULE_ENABLED)  /* 条件编译 */
#if defined(STM32F405xx) || defined(STM32F415xx) || defined(STM32F407xx) || defined(STM32F417xx) || defined(STM32F412Zx) ||\
    defined(STM32F412Vx) || defined(STM32F412Rx) || defined(STM32F413xx) || defined(STM32F423xx)  /* 源代码 */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/** @addtogroup FSMC_LL_Private_Functions
  * @{
  */

/** @addtogroup FSMC_LL_NORSRAM
  * @brief    NORSRAM Controller functions 
  *
  @verbatim 
  ==============================================================================
                   ##### How to use NORSRAM device driver #####
  ==============================================================================

  [..] 
    This driver contains a set of APIs to interface with the FSMC NORSRAM banks in order
    to run the NORSRAM external devices.

    (+) FSMC NORSRAM bank reset using the function FSMC_NORSRAM_DeInit() 
    (+) FSMC NORSRAM bank control configuration using the function FSMC_NORSRAM_Init()
    (+) FSMC NORSRAM bank timing configuration using the function FSMC_NORSRAM_Timing_Init()
    (+) FSMC NORSRAM bank extended timing configuration using the function 
        FSMC_NORSRAM_Extended_Timing_Init()
    (+) FSMC NORSRAM bank enable/disable write operation using the functions
        FSMC_NORSRAM_WriteOperation_Enable()/FSMC_NORSRAM_WriteOperation_Disable()

@endverbatim
  * @{
  */

/** @addtogroup FSMC_LL_NORSRAM_Private_Functions_Group1
  * @brief    Initialization and Configuration functions 
  *
  @verbatim    
  ==============================================================================
              ##### Initialization and de_initialization functions #####
  ==============================================================================
  [..]  
    This section provides functions allowing to:
    (+) Initialize and configure the FSMC NORSRAM interface
    (+) De-initialize the FSMC NORSRAM interface 
    (+) Configure the FSMC clock and associated GPIOs    

@endverbatim
  * @{
  */

/**
  * @brief  Initialize the FSMC_NORSRAM device according to the specified
  *         control parameters in the FSMC_NORSRAM_InitTypeDef
  * @param  Device Pointer to NORSRAM device instance
  * @param  Init Pointer to NORSRAM Initialization structure   
  * @retval HAL status
  */
HAL_StatusTypeDef  FSMC_NORSRAM_Init(FSMC_NORSRAM_TypeDef *Device, FSMC_NORSRAM_InitTypeDef* Init)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpr = 0U;                                   /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_FSMC_NORSRAM_DEVICE(Device));         /* 执行语句 */
  assert_param(IS_FSMC_NORSRAM_BANK(Init->NSBank));     /* 执行语句 */
  assert_param(IS_FSMC_MUX(Init->DataAddressMux));      /* 执行语句 */
  assert_param(IS_FSMC_MEMORY(Init->MemoryType));       /* 执行语句 */
  assert_param(IS_FSMC_NORSRAM_MEMORY_WIDTH(Init->MemoryDataWidth));  /* 执行语句 */
  assert_param(IS_FSMC_BURSTMODE(Init->BurstAccessMode));  /* 执行语句 */
  assert_param(IS_FSMC_WAIT_POLARITY(Init->WaitSignalPolarity));  /* 执行语句 */
#if defined(STM32F405xx) || defined(STM32F415xx) || defined(STM32F407xx) || defined(STM32F417xx)  /* 条件编译 */
  assert_param(IS_FSMC_WRAP_MODE(Init->WrapMode));      /* 执行语句 */
#endif /* STM32F405xx || STM32F415xx || STM32F407xx || STM32F417xx */
  assert_param(IS_FSMC_WAIT_SIGNAL_ACTIVE(Init->WaitSignalActive));  /* 执行语句 */
  assert_param(IS_FSMC_WRITE_OPERATION(Init->WriteOperation));  /* 执行语句 */
  assert_param(IS_FSMC_WAITE_SIGNAL(Init->WaitSignal));  /* 执行语句 */
  assert_param(IS_FSMC_EXTENDED_MODE(Init->ExtendedMode));  /* 执行语句 */
  assert_param(IS_FSMC_ASYNWAIT(Init->AsynchronousWait));  /* 执行语句 */
  assert_param(IS_FSMC_WRITE_BURST(Init->WriteBurst));  /* 执行语句 */
  assert_param(IS_FSMC_PAGESIZE(Init->PageSize));       /* 执行语句 */
#if defined(STM32F412Zx) || defined(STM32F412Vx) || defined(STM32F412Rx) || defined(STM32F413xx) || defined(STM32F423xx)  /* 条件编译 */
  assert_param(IS_FSMC_WRITE_FIFO(Init->WriteFifo));    /* 执行语句 */
  assert_param(IS_FSMC_CONTINOUS_CLOCK(Init->ContinuousClock));  /* 执行语句 */
#endif /* STM32F412Zx || STM32F412Vx || STM32F413xx || STM32F423xx */

  /* Get the BTCR register value */
  tmpr = Device->BTCR[Init->NSBank];                    /* 赋值或初始化 */

#if defined(STM32F405xx) || defined(STM32F415xx) || defined(STM32F407xx) || defined(STM32F417xx)  /* 条件编译 */
  /* Clear MBKEN, MUXEN, MTYP, MWID, FACCEN, BURSTEN, WAITPOL, WRAPMOD, WAITCFG, WREN,
           WAITEN, EXTMOD, ASYNCWAIT, CPSIZE and CBURSTRW bits */
  tmpr &= ((uint32_t)~(FSMC_BCR1_MBKEN     | FSMC_BCR1_MUXEN    | FSMC_BCR1_MTYP     | \
                       FSMC_BCR1_MWID      | FSMC_BCR1_FACCEN   | FSMC_BCR1_BURSTEN  | \
                       FSMC_BCR1_WAITPOL   | FSMC_BCR1_WRAPMOD  | FSMC_BCR1_WAITCFG  | \
                       FSMC_BCR1_WREN      | FSMC_BCR1_WAITEN   | FSMC_BCR1_EXTMOD   | \
                       FSMC_BCR1_ASYNCWAIT | FSMC_BCR1_CPSIZE   | FSMC_BCR1_CBURSTRW));  /* 执行语句 */
  /* Set NORSRAM device control parameters */
  tmpr |= (uint32_t)(Init->DataAddressMux       |\
                     Init->MemoryType           |\
                     Init->MemoryDataWidth      |\
                     Init->BurstAccessMode      |\
                     Init->WaitSignalPolarity   |\
                     Init->WrapMode             |\
                     Init->WaitSignalActive     |\
                     Init->WriteOperation       |\
                     Init->WaitSignal           |\
                     Init->ExtendedMode         |\
                     Init->AsynchronousWait     |\
                     Init->PageSize             |\
                     Init->WriteBurst                   /* 源代码 */
                     );                                 /* 执行语句 */
#else /* STM32F412Zx || STM32F412Vx || STM32F412Rx || STM32F413xx || STM32F423xx */
  /* Clear MBKEN, MUXEN, MTYP, MWID, FACCEN, BURSTEN, WAITPOL, WAITCFG, WREN,
           WAITEN, EXTMOD, ASYNCWAIT,CPSIZE,  CBURSTRW, CCLKEN and WFDIS bits */
  tmpr &= ((uint32_t)~(FSMC_BCR1_MBKEN     | FSMC_BCR1_MUXEN    | FSMC_BCR1_MTYP      | \
                       FSMC_BCR1_MWID      | FSMC_BCR1_FACCEN   | FSMC_BCR1_BURSTEN   | \
                       FSMC_BCR1_WAITPOL   | FSMC_BCR1_WAITCFG  | FSMC_BCR1_WREN      | \
                       FSMC_BCR1_WAITEN    | FSMC_BCR1_EXTMOD   | FSMC_BCR1_ASYNCWAIT | \
                       FSMC_BCR1_CPSIZE    | FSMC_BCR1_CBURSTRW | FSMC_BCR1_CCLKEN    | \
                       FSMC_BCR1_WFDIS));               /* 执行语句 */
  /* Set NORSRAM device control parameters */
  tmpr |= (uint32_t)(Init->DataAddressMux       |\
                     Init->MemoryType           |\
                     Init->MemoryDataWidth      |\
                     Init->BurstAccessMode      |\
                     Init->WaitSignalPolarity   |\
                     Init->WaitSignalActive     |\
                     Init->WriteOperation       |\
                     Init->WaitSignal           |\
                     Init->ExtendedMode         |\
                     Init->AsynchronousWait     |\
                     Init->WriteBurst           |\
                     Init->ContinuousClock      |\
                     Init->PageSize             |\
                     Init->WriteFifo);                  /* 执行语句 */
#endif /* STM32F405xx || STM32F415xx || STM32F407xx || STM32F417xx */ 

  if(Init->MemoryType == FSMC_MEMORY_TYPE_NOR)          /* 条件判断 */
  {                                                     /* 代码块开始 */
    tmpr |= (uint32_t)FSMC_NORSRAM_FLASH_ACCESS_ENABLE;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  Device->BTCR[Init->NSBank] = tmpr;                    /* 赋值或初始化 */

#if defined(STM32F412Zx) || defined(STM32F412Vx) || defined(STM32F412Rx) || defined(STM32F413xx) || defined(STM32F423xx)  /* 条件编译 */
  /* Configure synchronous mode when Continuous clock is enabled for bank2..4 */
  if((Init->ContinuousClock == FSMC_CONTINUOUS_CLOCK_SYNC_ASYNC) && (Init->NSBank != FSMC_NORSRAM_BANK1))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    Device->BTCR[FSMC_NORSRAM_BANK1] |= (uint32_t)(Init->ContinuousClock);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if(Init->NSBank != FSMC_NORSRAM_BANK1)                /* 条件判断 */
  {                                                     /* 代码块开始 */
    Device->BTCR[FSMC_NORSRAM_BANK1] |= (uint32_t)(Init->WriteFifo);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
#endif /* STM32F412Zx || STM32F412Vx || STM32F412Rx || STM32F413xx || STM32F423xx */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  DeInitialize the FSMC_NORSRAM peripheral 
  * @param  Device Pointer to NORSRAM device instance
  * @param  ExDevice Pointer to NORSRAM extended mode device instance  
  * @param  Bank NORSRAM bank number  
  * @retval HAL status
  */
HAL_StatusTypeDef FSMC_NORSRAM_DeInit(FSMC_NORSRAM_TypeDef *Device, FSMC_NORSRAM_EXTENDED_TypeDef *ExDevice, uint32_t Bank)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_FSMC_NORSRAM_DEVICE(Device));         /* 执行语句 */
  assert_param(IS_FSMC_NORSRAM_EXTENDED_DEVICE(ExDevice));  /* 执行语句 */
  assert_param(IS_FSMC_NORSRAM_BANK(Bank));             /* 执行语句 */

  /* Disable the FSMC_NORSRAM device */
  __FSMC_NORSRAM_DISABLE(Device, Bank);                 /* 执行语句 */

  /* De-initialize the FSMC_NORSRAM device */
  /* FSMC_NORSRAM_BANK1 */
  if(Bank == FSMC_NORSRAM_BANK1)                        /* 条件判断 */
  {                                                     /* 代码块开始 */
    Device->BTCR[Bank] = 0x000030DBU;                   /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  /* FSMC_NORSRAM_BANK2, FSMC_NORSRAM_BANK3 or FSMC_NORSRAM_BANK4 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    Device->BTCR[Bank] = 0x000030D2U;                   /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  Device->BTCR[Bank + 1U] = 0x0FFFFFFFU;                /* 赋值或初始化 */
  ExDevice->BWTR[Bank]    = 0x0FFFFFFFU;                /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */


/**
  * @brief  Initialize the FSMC_NORSRAM Timing according to the specified
  *         parameters in the FSMC_NORSRAM_TimingTypeDef
  * @param  Device Pointer to NORSRAM device instance
  * @param  Timing Pointer to NORSRAM Timing structure
  * @param  Bank NORSRAM bank number  
  * @retval HAL status
  */
HAL_StatusTypeDef FSMC_NORSRAM_Timing_Init(FSMC_NORSRAM_TypeDef *Device, FSMC_NORSRAM_TimingTypeDef *Timing, uint32_t Bank)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpr = 0U;                                   /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_FSMC_NORSRAM_DEVICE(Device));         /* 执行语句 */
  assert_param(IS_FSMC_ADDRESS_SETUP_TIME(Timing->AddressSetupTime));  /* 执行语句 */
  assert_param(IS_FSMC_ADDRESS_HOLD_TIME(Timing->AddressHoldTime));  /* 执行语句 */
  assert_param(IS_FSMC_DATASETUP_TIME(Timing->DataSetupTime));  /* 执行语句 */
  assert_param(IS_FSMC_TURNAROUND_TIME(Timing->BusTurnAroundDuration));  /* 执行语句 */
  assert_param(IS_FSMC_CLK_DIV(Timing->CLKDivision));   /* 执行语句 */
  assert_param(IS_FSMC_DATA_LATENCY(Timing->DataLatency));  /* 执行语句 */
  assert_param(IS_FSMC_ACCESS_MODE(Timing->AccessMode));  /* 执行语句 */
  assert_param(IS_FSMC_NORSRAM_BANK(Bank));             /* 执行语句 */

  /* Get the BTCR register value */
  tmpr = Device->BTCR[Bank + 1U];                       /* 赋值或初始化 */

  /* Clear ADDSET, ADDHLD, DATAST, BUSTURN, CLKDIV, DATLAT and ACCMOD bits */
  tmpr &= ((uint32_t)~(FSMC_BTR1_ADDSET  | FSMC_BTR1_ADDHLD | FSMC_BTR1_DATAST | \
                       FSMC_BTR1_BUSTURN | FSMC_BTR1_CLKDIV | FSMC_BTR1_DATLAT | \
                       FSMC_BTR1_ACCMOD));              /* 执行语句 */

  /* Set FSMC_NORSRAM device timing parameters */  
  tmpr |= (uint32_t)(Timing->AddressSetupTime                  |\
                    ((Timing->AddressHoldTime) << 4U)          |\
                    ((Timing->DataSetupTime) << 8U)            |\
                    ((Timing->BusTurnAroundDuration) << 16U)   |\
                    (((Timing->CLKDivision)-1U) << 20U)        |\
                    (((Timing->DataLatency)-2U) << 24U)        |\
                    (Timing->AccessMode));              /* 执行语句 */

  Device->BTCR[Bank + 1] = tmpr;                        /* 赋值或初始化 */

#if defined(STM32F412Zx) || defined(STM32F412Vx) || defined(STM32F412Rx) || defined(STM32F413xx) || defined(STM32F423xx)  /* 条件编译 */
  /* Configure Clock division value (in NORSRAM bank 1) when continuous clock is enabled */
  if(HAL_IS_BIT_SET(Device->BTCR[FSMC_NORSRAM_BANK1], FSMC_BCR1_CCLKEN))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    tmpr = (uint32_t)(Device->BTCR[FSMC_NORSRAM_BANK1 + 1U] & ~(0x0FU << 20U));   /* 赋值或初始化 */
    tmpr |= (uint32_t)(((Timing->CLKDivision)-1U) << 20U);  /* 赋值或初始化 */
    Device->BTCR[FSMC_NORSRAM_BANK1 + 1U] = tmpr;       /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
#endif /* STM32F412Zx || STM32F412Vx || STM32F412Rx || STM32F413xx || STM32F423xx */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initialize the FSMC_NORSRAM Extended mode Timing according to the specified
  *         parameters in the FSMC_NORSRAM_TimingTypeDef
  * @param  Device Pointer to NORSRAM device instance
  * @param  Timing Pointer to NORSRAM Timing structure
  * @param  Bank NORSRAM bank number  
  * @retval HAL status
  */
HAL_StatusTypeDef  FSMC_NORSRAM_Extended_Timing_Init(FSMC_NORSRAM_EXTENDED_TypeDef *Device, FSMC_NORSRAM_TimingTypeDef *Timing, uint32_t Bank, uint32_t ExtendedMode)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpr = 0U;                                   /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_FSMC_EXTENDED_MODE(ExtendedMode));    /* 执行语句 */

  /* Set NORSRAM device timing register for write configuration, if extended mode is used */
  if(ExtendedMode == FSMC_EXTENDED_MODE_ENABLE)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check the parameters */
    assert_param(IS_FSMC_NORSRAM_EXTENDED_DEVICE(Device));  /* 执行语句 */
    assert_param(IS_FSMC_ADDRESS_SETUP_TIME(Timing->AddressSetupTime));  /* 执行语句 */
    assert_param(IS_FSMC_ADDRESS_HOLD_TIME(Timing->AddressHoldTime));  /* 执行语句 */
    assert_param(IS_FSMC_DATASETUP_TIME(Timing->DataSetupTime));  /* 执行语句 */
    assert_param(IS_FSMC_TURNAROUND_TIME(Timing->BusTurnAroundDuration));  /* 执行语句 */
    assert_param(IS_FSMC_ACCESS_MODE(Timing->AccessMode));  /* 执行语句 */
    assert_param(IS_FSMC_NORSRAM_BANK(Bank));           /* 执行语句 */

    /* Get the BWTR register value */
    tmpr = Device->BWTR[Bank];                          /* 赋值或初始化 */

    /* Clear ADDSET, ADDHLD, DATAST, BUSTURN and ACCMOD bits */
    tmpr &= ((uint32_t)~(FSMC_BWTR1_ADDSET  | FSMC_BWTR1_ADDHLD | FSMC_BWTR1_DATAST | \
                         FSMC_BWTR1_BUSTURN | FSMC_BWTR1_ACCMOD));  /* 执行语句 */

    tmpr |= (uint32_t)(Timing->AddressSetupTime                  |\
                      ((Timing->AddressHoldTime) << 4U)          |\
                      ((Timing->DataSetupTime) << 8U)            |\
                      ((Timing->BusTurnAroundDuration) << 16U)   |\
                      (Timing->AccessMode));            /* 执行语句 */

    Device->BWTR[Bank] = tmpr;                          /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    Device->BWTR[Bank] = 0x0FFFFFFFU;                   /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */
/**
  * @}
  */

/** @addtogroup FSMC_LL_NORSRAM_Private_Functions_Group2
  *  @brief   management functions 
  *
@verbatim   
  ==============================================================================
                      ##### FSMC_NORSRAM Control functions #####
  ==============================================================================
  [..]
    This subsection provides a set of functions allowing to control dynamically
    the FSMC NORSRAM interface.

@endverbatim
  * @{
  */

/**
  * @brief  Enables dynamically FSMC_NORSRAM write operation.
  * @param  Device Pointer to NORSRAM device instance
  * @param  Bank NORSRAM bank number   
  * @retval HAL status
  */
HAL_StatusTypeDef FSMC_NORSRAM_WriteOperation_Enable(FSMC_NORSRAM_TypeDef *Device, uint32_t Bank)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_FSMC_NORSRAM_DEVICE(Device));         /* 执行语句 */
  assert_param(IS_FSMC_NORSRAM_BANK(Bank));             /* 执行语句 */

  /* Enable write operation */
  Device->BTCR[Bank] |= FSMC_WRITE_OPERATION_ENABLE;    /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Disables dynamically FSMC_NORSRAM write operation.
  * @param  Device Pointer to NORSRAM device instance
  * @param  Bank NORSRAM bank number   
  * @retval HAL status
  */
HAL_StatusTypeDef FSMC_NORSRAM_WriteOperation_Disable(FSMC_NORSRAM_TypeDef *Device, uint32_t Bank)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_FSMC_NORSRAM_DEVICE(Device));         /* 执行语句 */
  assert_param(IS_FSMC_NORSRAM_BANK(Bank));             /* 执行语句 */

  /* Disable write operation */
  Device->BTCR[Bank] &= ~FSMC_WRITE_OPERATION_ENABLE;   /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */
/**
  * @}
  */

/**
  * @}
  */

#if defined(STM32F405xx) || defined(STM32F415xx) || defined(STM32F407xx) || defined(STM32F417xx)  /* 条件编译 */
/** @addtogroup FSMC_LL_NAND
  * @brief    NAND Controller functions 
  *
  @verbatim 
  ==============================================================================   
                    ##### How to use NAND device driver #####
  ==============================================================================
  [..]
    This driver contains a set of APIs to interface with the FSMC NAND banks in order
    to run the NAND external devices.

    (+) FSMC NAND bank reset using the function FSMC_NAND_DeInit() 
    (+) FSMC NAND bank control configuration using the function FSMC_NAND_Init()
    (+) FSMC NAND bank common space timing configuration using the function 
        FSMC_NAND_CommonSpace_Timing_Init()
    (+) FSMC NAND bank attribute space timing configuration using the function 
        FSMC_NAND_AttributeSpace_Timing_Init()
    (+) FSMC NAND bank enable/disable ECC correction feature using the functions
        FSMC_NAND_ECC_Enable()/FSMC_NAND_ECC_Disable()
    (+) FSMC NAND bank get ECC correction code using the function FSMC_NAND_GetECC()  

@endverbatim
  * @{
  */

/** @addtogroup FSMC_LL_NAND_Private_Functions_Group1
  *  @brief    Initialization and Configuration functions 
  *
@verbatim    
  ==============================================================================
              ##### Initialization and de_initialization functions #####
  ==============================================================================
  [..]  
    This section provides functions allowing to:
    (+) Initialize and configure the FSMC NAND interface
    (+) De-initialize the FSMC NAND interface 
    (+) Configure the FSMC clock and associated GPIOs

@endverbatim
  * @{
  */

/**
  * @brief  Initializes the FSMC_NAND device according to the specified
  *         control parameters in the FSMC_NAND_HandleTypeDef
  * @param  Device Pointer to NAND device instance
  * @param  Init Pointer to NAND Initialization structure
  * @retval HAL status
  */
HAL_StatusTypeDef FSMC_NAND_Init(FSMC_NAND_TypeDef *Device, FSMC_NAND_InitTypeDef *Init)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpr  = 0U;                                  /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_FSMC_NAND_BANK(Init->NandBank));      /* 执行语句 */
  assert_param(IS_FSMC_WAIT_FEATURE(Init->Waitfeature));  /* 执行语句 */
  assert_param(IS_FSMC_NAND_MEMORY_WIDTH(Init->MemoryDataWidth));  /* 执行语句 */
  assert_param(IS_FSMC_ECC_STATE(Init->EccComputation));  /* 执行语句 */
  assert_param(IS_FSMC_ECCPAGE_SIZE(Init->ECCPageSize));  /* 执行语句 */
  assert_param(IS_FSMC_TCLR_TIME(Init->TCLRSetupTime));  /* 执行语句 */
  assert_param(IS_FSMC_TAR_TIME(Init->TARSetupTime));     /* 执行语句 */

    if(Init->NandBank == FSMC_NAND_BANK2)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Get the NAND bank 2 register value */
    tmpr = Device->PCR2;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Get the NAND bank 3 register value */
    tmpr = Device->PCR3;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Clear PWAITEN, PBKEN, PTYP, PWID, ECCEN, TCLR, TAR and ECCPS bits */
  tmpr &= ((uint32_t)~(FSMC_PCR2_PWAITEN  | FSMC_PCR2_PBKEN | FSMC_PCR2_PTYP | \
                       FSMC_PCR2_PWID | FSMC_PCR2_ECCEN | FSMC_PCR2_TCLR | \
                       FSMC_PCR2_TAR | FSMC_PCR2_ECCPS));    /* 执行语句 */

  /* Set NAND device control parameters */
  tmpr |= (uint32_t)(Init->Waitfeature                |\
                     FSMC_PCR_MEMORY_TYPE_NAND        |\
                     Init->MemoryDataWidth            |\
                     Init->EccComputation             |\
                     Init->ECCPageSize                |\
                     ((Init->TCLRSetupTime) << 9U)    |\
                     ((Init->TARSetupTime) << 13U));     /* 执行语句 */

  if(Init->NandBank == FSMC_NAND_BANK2)                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* NAND bank 2 registers configuration */
    Device->PCR2  = tmpr;                               /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* NAND bank 3 registers configuration */
    Device->PCR3  = tmpr;                               /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initializes the FSMC_NAND Common space Timing according to the specified
  *         parameters in the FSMC_NAND_PCC_TimingTypeDef
  * @param  Device Pointer to NAND device instance
  * @param  Timing Pointer to NAND timing structure
  * @param  Bank NAND bank number   
  * @retval HAL status
  */
HAL_StatusTypeDef FSMC_NAND_CommonSpace_Timing_Init(FSMC_NAND_TypeDef *Device, FSMC_NAND_PCC_TimingTypeDef *Timing, uint32_t Bank)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpr = 0U;                                   /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_FSMC_SETUP_TIME(Timing->SetupTime));  /* 执行语句 */
  assert_param(IS_FSMC_WAIT_TIME(Timing->WaitSetupTime));  /* 执行语句 */
  assert_param(IS_FSMC_HOLD_TIME(Timing->HoldSetupTime));  /* 执行语句 */
  assert_param(IS_FSMC_HIZ_TIME(Timing->HiZSetupTime));  /* 执行语句 */

  if(Bank == FSMC_NAND_BANK2)                           /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Get the NAND bank 2 register value */
    tmpr = Device->PMEM2;                               /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Get the NAND bank 3 register value */
    tmpr = Device->PMEM3;                               /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Clear MEMSETx, MEMWAITx, MEMHOLDx and MEMHIZx bits */
  tmpr &= ((uint32_t)~(FSMC_PMEM2_MEMSET2  | FSMC_PMEM2_MEMWAIT2 | FSMC_PMEM2_MEMHOLD2 | \
                       FSMC_PMEM2_MEMHIZ2));            /* 执行语句 */

  /* Set FSMC_NAND device timing parameters */
  tmpr |= (uint32_t)(Timing->SetupTime                     |\
                       ((Timing->WaitSetupTime) << 8U)     |\
                       ((Timing->HoldSetupTime) << 16U)    |\
                       ((Timing->HiZSetupTime) << 24U)  /* 源代码 */
                       );                               /* 执行语句 */

  if(Bank == FSMC_NAND_BANK2)                           /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* NAND bank 2 registers configuration */
    Device->PMEM2 = tmpr;                               /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* NAND bank 3 registers configuration */
    Device->PMEM3 = tmpr;                               /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initializes the FSMC_NAND Attribute space Timing according to the specified
  *         parameters in the FSMC_NAND_PCC_TimingTypeDef
  * @param  Device Pointer to NAND device instance
  * @param  Timing Pointer to NAND timing structure
  * @param  Bank NAND bank number 
  * @retval HAL status
  */
HAL_StatusTypeDef FSMC_NAND_AttributeSpace_Timing_Init(FSMC_NAND_TypeDef *Device, FSMC_NAND_PCC_TimingTypeDef *Timing, uint32_t Bank)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpr = 0U;                                   /* 32位无符号变量/函数 */

  /* Check the parameters */  
  assert_param(IS_FSMC_SETUP_TIME(Timing->SetupTime));  /* 执行语句 */
  assert_param(IS_FSMC_WAIT_TIME(Timing->WaitSetupTime));  /* 执行语句 */
  assert_param(IS_FSMC_HOLD_TIME(Timing->HoldSetupTime));  /* 执行语句 */
  assert_param(IS_FSMC_HIZ_TIME(Timing->HiZSetupTime));  /* 执行语句 */

  if(Bank == FSMC_NAND_BANK2)                           /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Get the NAND bank 2 register value */
    tmpr = Device->PATT2;                               /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Get the NAND bank 3 register value */
    tmpr = Device->PATT3;                               /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Clear ATTSETx, ATTWAITx, ATTHOLDx and ATTHIZx bits */
  tmpr &= ((uint32_t)~(FSMC_PATT2_ATTSET2  | FSMC_PATT2_ATTWAIT2 | FSMC_PATT2_ATTHOLD2 | \
                       FSMC_PATT2_ATTHIZ2));            /* 执行语句 */

  /* Set FSMC_NAND device timing parameters */
  tmpr |= (uint32_t)(Timing->SetupTime                     |\
                       ((Timing->WaitSetupTime) << 8U)     |\
                       ((Timing->HoldSetupTime) << 16U)    |\
                       ((Timing->HiZSetupTime) << 24U)  /* 源代码 */
                       );                               /* 执行语句 */

  if(Bank == FSMC_NAND_BANK2)                           /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* NAND bank 2 registers configuration */
    Device->PATT2 = tmpr;                               /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* NAND bank 3 registers configuration */
    Device->PATT3 = tmpr;                               /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  DeInitializes the FSMC_NAND device 
  * @param  Device Pointer to NAND device instance
  * @param  Bank NAND bank number
  * @retval HAL status
  */
HAL_StatusTypeDef FSMC_NAND_DeInit(FSMC_NAND_TypeDef *Device, uint32_t Bank)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Disable the NAND Bank */
  __FSMC_NAND_DISABLE(Device, Bank);                    /* 执行语句 */

  /* De-initialize the NAND Bank */
  if(Bank == FSMC_NAND_BANK2)                           /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Set the FSMC_NAND_BANK2 registers to their reset values */
    Device->PCR2  = 0x00000018U;                        /* 赋值或初始化 */
    Device->SR2   = 0x00000040U;                        /* 赋值或初始化 */
    Device->PMEM2 = 0xFCFCFCFCU;                        /* 赋值或初始化 */
    Device->PATT2 = 0xFCFCFCFCU;                        /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  /* FSMC_Bank3_NAND */  
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Set the FSMC_NAND_BANK3 registers to their reset values */
    Device->PCR3  = 0x00000018U;                        /* 赋值或初始化 */
    Device->SR3   = 0x00000040U;                        /* 赋值或初始化 */
    Device->PMEM3 = 0xFCFCFCFCU;                        /* 赋值或初始化 */
    Device->PATT3 = 0xFCFCFCFCU;                        /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */
/**
  * @}
  */

/** @addtogroup FSMC_LL_NAND_Private_Functions_Group2
  *  @brief   management functions 
  *
@verbatim   
  ==============================================================================
                       ##### FSMC_NAND Control functions #####
  ==============================================================================
  [..]
    This subsection provides a set of functions allowing to control dynamically
    the FSMC NAND interface.

@endverbatim
  * @{
  */ 

/**
  * @brief  Enables dynamically FSMC_NAND ECC feature.
  * @param  Device Pointer to NAND device instance
  * @param  Bank NAND bank number
  * @retval HAL status
  */    
HAL_StatusTypeDef  FSMC_NAND_ECC_Enable(FSMC_NAND_TypeDef *Device, uint32_t Bank)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Enable ECC feature */
  if(Bank == FSMC_NAND_BANK2)                           /* 条件判断 */
  {                                                     /* 代码块开始 */
    Device->PCR2 |= FSMC_PCR2_ECCEN;                    /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    Device->PCR3 |= FSMC_PCR3_ECCEN;                    /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Disables dynamically FSMC_NAND ECC feature.
  * @param  Device Pointer to NAND device instance
  * @param  Bank NAND bank number
  * @retval HAL status
  */  
HAL_StatusTypeDef FSMC_NAND_ECC_Disable(FSMC_NAND_TypeDef *Device, uint32_t Bank)    /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Disable ECC feature */
  if(Bank == FSMC_NAND_BANK2)                           /* 条件判断 */
  {                                                     /* 代码块开始 */
    Device->PCR2 &= ~FSMC_PCR2_ECCEN;                   /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    Device->PCR3 &= ~FSMC_PCR3_ECCEN;                   /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Disables dynamically FSMC_NAND ECC feature.
  * @param  Device Pointer to NAND device instance
  * @param  ECCval Pointer to ECC value
  * @param  Bank NAND bank number
  * @param  Timeout Timeout wait value  
  * @retval HAL status
  */
HAL_StatusTypeDef FSMC_NAND_GetECC(FSMC_NAND_TypeDef *Device, uint32_t *ECCval, uint32_t Bank, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart = 0U;                              /* 32位无符号变量/函数 */

  /* Check the parameters */ 
  assert_param(IS_FSMC_NAND_DEVICE(Device));            /* 执行语句 */
  assert_param(IS_FSMC_NAND_BANK(Bank));                /* 执行语句 */

  /* Get tick */ 
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  /* Wait until FIFO is empty */
  while(__FSMC_NAND_GET_FLAG(Device, Bank, FSMC_FLAG_FEMPT) == RESET)  /* while循环 */
  {                                                     /* 代码块开始 */
    /* Check for the Timeout */
    if(Timeout != HAL_MAX_DELAY)                        /* 条件判断 */
    {                                                   /* 代码块开始 */
      if((Timeout == 0U)||((HAL_GetTick() - tickstart ) > Timeout))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  if(Bank == FSMC_NAND_BANK2)                           /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Get the ECCR2 register value */
    *ECCval = (uint32_t)Device->ECCR2;                  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Get the ECCR3 register value */
    *ECCval = (uint32_t)Device->ECCR3;                  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup FSMC_LL_PCCARD
  * @brief    PCCARD Controller functions 
  *
  @verbatim 
  ==============================================================================
                    ##### How to use PCCARD device driver #####
  ==============================================================================
  [..]
    This driver contains a set of APIs to interface with the FSMC PCCARD bank in order
    to run the PCCARD/compact flash external devices.

    (+) FSMC PCCARD bank reset using the function FSMC_PCCARD_DeInit() 
    (+) FSMC PCCARD bank control configuration using the function FSMC_PCCARD_Init()
    (+) FSMC PCCARD bank common space timing configuration using the function 
        FSMC_PCCARD_CommonSpace_Timing_Init()
    (+) FSMC PCCARD bank attribute space timing configuration using the function 
        FSMC_PCCARD_AttributeSpace_Timing_Init()
    (+) FSMC PCCARD bank IO space timing configuration using the function 
        FSMC_PCCARD_IOSpace_Timing_Init()

@endverbatim
  * @{
  */

/** @addtogroup FSMC_LL_PCCARD_Private_Functions_Group1
  *  @brief   Initialization and Configuration functions 
  *
@verbatim    
  ==============================================================================
              ##### Initialization and de_initialization functions #####
  ==============================================================================
  [..]  
    This section provides functions allowing to:
    (+) Initialize and configure the FSMC PCCARD interface
    (+) De-initialize the FSMC PCCARD interface 
    (+) Configure the FSMC clock and associated GPIOs

@endverbatim
  * @{
  */

/**
  * @brief  Initializes the FSMC_PCCARD device according to the specified
  *         control parameters in the FSMC_PCCARD_HandleTypeDef
  * @param  Device Pointer to PCCARD device instance
  * @param  Init Pointer to PCCARD Initialization structure   
  * @retval HAL status
  */
HAL_StatusTypeDef FSMC_PCCARD_Init(FSMC_PCCARD_TypeDef *Device, FSMC_PCCARD_InitTypeDef *Init)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpr = 0U;                                   /* 32位无符号变量/函数 */

  /* Check the parameters */ 
  assert_param(IS_FSMC_WAIT_FEATURE(Init->Waitfeature));  /* 执行语句 */
  assert_param(IS_FSMC_TCLR_TIME(Init->TCLRSetupTime));  /* 执行语句 */
  assert_param(IS_FSMC_TAR_TIME(Init->TARSetupTime));       /* 执行语句 */

  /* Get PCCARD control register value */
  tmpr = Device->PCR4;                                  /* 赋值或初始化 */

  /* Clear TAR, TCLR, PWAITEN and PWID bits */
  tmpr &= ((uint32_t)~(FSMC_PCR4_TAR  | FSMC_PCR4_TCLR | FSMC_PCR4_PWAITEN | \
                       FSMC_PCR4_PWID | FSMC_PCR4_PTYP));  /* 执行语句 */

  /* Set FSMC_PCCARD device control parameters */
  tmpr |= (uint32_t)(Init->Waitfeature               |\
                     FSMC_NAND_PCC_MEM_BUS_WIDTH_16  |\
                     (Init->TCLRSetupTime << 9U)     |\
                     (Init->TARSetupTime << 13U));      /* 执行语句 */

  Device->PCR4 = tmpr;                                  /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initializes the FSMC_PCCARD Common space Timing according to the specified
  *         parameters in the FSMC_NAND_PCC_TimingTypeDef
  * @param  Device Pointer to PCCARD device instance
  * @param  Timing Pointer to PCCARD timing structure 
  * @retval HAL status
  */
HAL_StatusTypeDef FSMC_PCCARD_CommonSpace_Timing_Init(FSMC_PCCARD_TypeDef *Device, FSMC_NAND_PCC_TimingTypeDef *Timing)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpr = 0U;                                   /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_FSMC_SETUP_TIME(Timing->SetupTime));  /* 执行语句 */
  assert_param(IS_FSMC_WAIT_TIME(Timing->WaitSetupTime));  /* 执行语句 */
  assert_param(IS_FSMC_HOLD_TIME(Timing->HoldSetupTime));  /* 执行语句 */
  assert_param(IS_FSMC_HIZ_TIME(Timing->HiZSetupTime));  /* 执行语句 */

  /* Get PCCARD common space timing register value */
  tmpr = Device->PMEM4;                                 /* 赋值或初始化 */

  /* Clear MEMSETx, MEMWAITx, MEMHOLDx and MEMHIZx bits */
  tmpr &= ((uint32_t)~(FSMC_PMEM4_MEMSET4  | FSMC_PMEM4_MEMWAIT4 | FSMC_PMEM4_MEMHOLD4 | \
                       FSMC_PMEM4_MEMHIZ4));            /* 执行语句 */
  /* Set PCCARD timing parameters */
  tmpr |= (uint32_t)((Timing->SetupTime                 |\
                    ((Timing->WaitSetupTime) << 8U)     |\
                    (Timing->HoldSetupTime) << 16U)     |\
                    ((Timing->HiZSetupTime) << 24U));   /* 执行语句 */

  Device->PMEM4 = tmpr;                                 /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initializes the FSMC_PCCARD Attribute space Timing according to the specified
  *         parameters in the FSMC_NAND_PCC_TimingTypeDef
  * @param  Device Pointer to PCCARD device instance
  * @param  Timing Pointer to PCCARD timing structure  
  * @retval HAL status
  */
HAL_StatusTypeDef FSMC_PCCARD_AttributeSpace_Timing_Init(FSMC_PCCARD_TypeDef *Device, FSMC_NAND_PCC_TimingTypeDef *Timing)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpr = 0U;                                   /* 32位无符号变量/函数 */

  /* Check the parameters */  
  assert_param(IS_FSMC_SETUP_TIME(Timing->SetupTime));  /* 执行语句 */
  assert_param(IS_FSMC_WAIT_TIME(Timing->WaitSetupTime));  /* 执行语句 */
  assert_param(IS_FSMC_HOLD_TIME(Timing->HoldSetupTime));  /* 执行语句 */
  assert_param(IS_FSMC_HIZ_TIME(Timing->HiZSetupTime));  /* 执行语句 */

  /* Get PCCARD timing parameters */
  tmpr = Device->PATT4;                                 /* 赋值或初始化 */

  /* Clear ATTSETx, ATTWAITx, ATTHOLDx and ATTHIZx bits */
  tmpr &= ((uint32_t)~(FSMC_PATT4_ATTSET4  | FSMC_PATT4_ATTWAIT4 | FSMC_PATT4_ATTHOLD4 | \
                       FSMC_PATT4_ATTHIZ4));            /* 执行语句 */

  /* Set PCCARD timing parameters */
  tmpr |= (uint32_t)(Timing->SetupTime                 |\
                   ((Timing->WaitSetupTime) << 8U)     |\
                   ((Timing->HoldSetupTime) << 16U)    |\
                   ((Timing->HiZSetupTime) << 24U));    /* 执行语句 */
  Device->PATT4 = tmpr;                                 /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initializes the FSMC_PCCARD IO space Timing according to the specified
  *         parameters in the FSMC_NAND_PCC_TimingTypeDef
  * @param  Device Pointer to PCCARD device instance
  * @param  Timing Pointer to PCCARD timing structure  
  * @retval HAL status
  */
HAL_StatusTypeDef FSMC_PCCARD_IOSpace_Timing_Init(FSMC_PCCARD_TypeDef *Device, FSMC_NAND_PCC_TimingTypeDef *Timing)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpr = 0U;                                   /* 32位无符号变量/函数 */

  /* Check the parameters */  
  assert_param(IS_FSMC_SETUP_TIME(Timing->SetupTime));  /* 执行语句 */
  assert_param(IS_FSMC_WAIT_TIME(Timing->WaitSetupTime));  /* 执行语句 */
  assert_param(IS_FSMC_HOLD_TIME(Timing->HoldSetupTime));  /* 执行语句 */
  assert_param(IS_FSMC_HIZ_TIME(Timing->HiZSetupTime));  /* 执行语句 */

  /* Get FSMC_PCCARD device timing parameters */
  tmpr = Device->PIO4;                                  /* 赋值或初始化 */

  /* Clear IOSET4, IOWAIT4, IOHOLD4 and IOHIZ4 bits */
  tmpr &= ((uint32_t)~(FSMC_PIO4_IOSET4  | FSMC_PIO4_IOWAIT4 | FSMC_PIO4_IOHOLD4 | \
                       FSMC_PIO4_IOHIZ4));              /* 执行语句 */

  /* Set FSMC_PCCARD device timing parameters */
  tmpr |= (uint32_t)(Timing->SetupTime                   |\
                     ((Timing->WaitSetupTime) << 8U)     |\
                     ((Timing->HoldSetupTime) << 16U)    |\
                     ((Timing->HiZSetupTime) << 24U));     /* 执行语句 */

  Device->PIO4 = tmpr;                                  /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  DeInitializes the FSMC_PCCARD device 
  * @param  Device Pointer to PCCARD device instance
  * @retval HAL status
  */
HAL_StatusTypeDef FSMC_PCCARD_DeInit(FSMC_PCCARD_TypeDef *Device)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Disable the FSMC_PCCARD device */
  __FSMC_PCCARD_DISABLE(Device);                        /* 执行语句 */

  /* De-initialize the FSMC_PCCARD device */
  Device->PCR4    = 0x00000018U;                        /* 赋值或初始化 */
  Device->SR4     = 0x00000000U;	                    /* 赋值或初始化 */
  Device->PMEM4   = 0xFCFCFCFCU;                        /* 赋值或初始化 */
  Device->PATT4   = 0xFCFCFCFCU;                        /* 赋值或初始化 */
  Device->PIO4    = 0xFCFCFCFCU;                        /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */
/**
  * @}
  */

/**
  * @}
  */
#endif /* STM32F405xx || STM32F415xx || STM32F407xx || STM32F417xx */

/**
  * @}
  */
#endif /* STM32F405xx || STM32F415xx || STM32F407xx || STM32F417xx || STM32F412Zx || STM32F412Vx || STM32F413xx || STM32F423xx */
#endif /* HAL_SRAM_MODULE_ENABLED || HAL_NOR_MODULE_ENABLED || HAL_NAND_MODULE_ENABLED || HAL_PCCARD_MODULE_ENABLED */

/**
  * @}
  */

/**
  * @}
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
