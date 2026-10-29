/* 文件: stm32f4xx_ll_fmc.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_ll_fmc.c
  * @author  MCD Application Team
  * @brief   FMC Low Layer HAL module driver.
  *    
  *          This file provides firmware functions to manage the following 
  *          functionalities of the Flexible Memory Controller (FMC) peripheral memories:
  *           + Initialization/de-initialization functions
  *           + Peripheral Control functions 
  *           + Peripheral State functions
  *         
  @verbatim
  ==============================================================================
                        ##### FMC peripheral features #####
  ==============================================================================
  [..] The Flexible memory controller (FMC) includes three memory controllers:
       (+) The NOR/PSRAM memory controller
       (+) The NAND/PC Card memory controller
       (+) The Synchronous DRAM (SDRAM) controller 

  [..] The FMC functional block makes the interface with synchronous and asynchronous static
       memories, SDRAM memories, and 16-bit PC memory cards. Its main purposes are:
       (+) to translate AHB transactions into the appropriate external device protocol
       (+) to meet the access time requirements of the external memory devices

  [..] All external memories share the addresses, data and control signals with the controller.
       Each external device is accessed by means of a unique Chip Select. The FMC performs
       only one access at a time to an external device.
       The main features of the FMC controller are the following:
        (+) Interface with static-memory mapped devices including:
           (++) Static random access memory (SRAM)
           (++) Read-only memory (ROM)
           (++) NOR Flash memory/OneNAND Flash memory
           (++) PSRAM (4 memory banks)
           (++) 16-bit PC Card compatible devices
           (++) Two banks of NAND Flash memory with ECC hardware to check up to 8 Kbytes of
                data
        (+) Interface with synchronous DRAM (SDRAM) memories
        (+) Independent Chip Select control for each memory bank
        (+) Independent configuration for each memory bank

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

/** @defgroup FMC_LL  FMC Low Layer
  * @brief FMC driver modules
  * @{
  */

#if defined (HAL_SRAM_MODULE_ENABLED) || defined(HAL_NOR_MODULE_ENABLED) || defined(HAL_NAND_MODULE_ENABLED) || defined(HAL_PCCARD_MODULE_ENABLED) || defined(HAL_SDRAM_MODULE_ENABLED)  /* 条件编译 */

#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx) || defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx)  /* 条件编译 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/** @addtogroup FMC_LL_Private_Functions
  * @{
  */

/** @addtogroup FMC_LL_NORSRAM
  * @brief  NORSRAM Controller functions 
  *
  @verbatim 
  ==============================================================================   
                   ##### How to use NORSRAM device driver #####
  ==============================================================================

  [..] 
    This driver contains a set of APIs to interface with the FMC NORSRAM banks in order
    to run the NORSRAM external devices.

    (+) FMC NORSRAM bank reset using the function FMC_NORSRAM_DeInit() 
    (+) FMC NORSRAM bank control configuration using the function FMC_NORSRAM_Init()
    (+) FMC NORSRAM bank timing configuration using the function FMC_NORSRAM_Timing_Init()
    (+) FMC NORSRAM bank extended timing configuration using the function 
        FMC_NORSRAM_Extended_Timing_Init()
    (+) FMC NORSRAM bank enable/disable write operation using the functions
        FMC_NORSRAM_WriteOperation_Enable()/FMC_NORSRAM_WriteOperation_Disable()


@endverbatim
  * @{
  */

/** @addtogroup FMC_LL_NORSRAM_Private_Functions_Group1
  * @brief    Initialization and Configuration functions 
  *
  @verbatim    
  ==============================================================================
              ##### Initialization and de_initialization functions #####
  ==============================================================================
  [..]  
    This section provides functions allowing to:
    (+) Initialize and configure the FMC NORSRAM interface
    (+) De-initialize the FMC NORSRAM interface 
    (+) Configure the FMC clock and associated GPIOs    

@endverbatim
  * @{
  */

/**
  * @brief  Initialize the FMC_NORSRAM device according to the specified
  *         control parameters in the FMC_NORSRAM_InitTypeDef
  * @param  Device Pointer to NORSRAM device instance
  * @param  Init Pointer to NORSRAM Initialization structure   
  * @retval HAL status
  */
HAL_StatusTypeDef  FMC_NORSRAM_Init(FMC_NORSRAM_TypeDef *Device, FMC_NORSRAM_InitTypeDef* Init)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpr = 0U;                                   /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_FMC_NORSRAM_DEVICE(Device));          /* 执行语句 */
  assert_param(IS_FMC_NORSRAM_BANK(Init->NSBank));      /* 执行语句 */
  assert_param(IS_FMC_MUX(Init->DataAddressMux));       /* 执行语句 */
  assert_param(IS_FMC_MEMORY(Init->MemoryType));        /* 执行语句 */
  assert_param(IS_FMC_NORSRAM_MEMORY_WIDTH(Init->MemoryDataWidth));  /* 执行语句 */
  assert_param(IS_FMC_BURSTMODE(Init->BurstAccessMode));  /* 执行语句 */
  assert_param(IS_FMC_WAIT_POLARITY(Init->WaitSignalPolarity));  /* 执行语句 */
#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx)  /* 条件编译 */
  assert_param(IS_FMC_WRAP_MODE(Init->WrapMode));       /* 执行语句 */
#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx */
  assert_param(IS_FMC_WAIT_SIGNAL_ACTIVE(Init->WaitSignalActive));  /* 执行语句 */
  assert_param(IS_FMC_WRITE_OPERATION(Init->WriteOperation));  /* 执行语句 */
  assert_param(IS_FMC_WAITE_SIGNAL(Init->WaitSignal));  /* 执行语句 */
  assert_param(IS_FMC_EXTENDED_MODE(Init->ExtendedMode));  /* 执行语句 */
  assert_param(IS_FMC_ASYNWAIT(Init->AsynchronousWait));  /* 执行语句 */
  assert_param(IS_FMC_WRITE_BURST(Init->WriteBurst));   /* 执行语句 */
  assert_param(IS_FMC_CONTINOUS_CLOCK(Init->ContinuousClock));  /* 执行语句 */
  assert_param(IS_FMC_PAGESIZE(Init->PageSize));        /* 执行语句 */
#if defined (STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx)  /* 条件编译 */
  assert_param(IS_FMC_WRITE_FIFO(Init->WriteFifo));     /* 执行语句 */
#endif /* STM32F446xx || STM32F469xx || STM32F479xx */

  /* Get the BTCR register value */
  tmpr = Device->BTCR[Init->NSBank];                    /* 赋值或初始化 */

#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx)  /* 条件编译 */
  /* Clear MBKEN, MUXEN, MTYP, MWID, FACCEN, BURSTEN, WAITPOL, WRAPMOD, WAITCFG, WREN,
           WAITEN, EXTMOD, ASYNCWAIT, CPSIZE, CBURSTRW and CCLKEN bits */
  tmpr &= ((uint32_t)~(FMC_BCR1_MBKEN     | FMC_BCR1_MUXEN    | FMC_BCR1_MTYP     | \
                       FMC_BCR1_MWID      | FMC_BCR1_FACCEN   | FMC_BCR1_BURSTEN  | \
                       FMC_BCR1_WAITPOL   | FMC_BCR1_WRAPMOD  | FMC_BCR1_WAITCFG  | \
                       FMC_BCR1_WREN      | FMC_BCR1_WAITEN   | FMC_BCR1_EXTMOD   | \
                       FMC_BCR1_ASYNCWAIT | FMC_BCR1_CPSIZE   | FMC_BCR1_CBURSTRW | \
                       FMC_BCR1_CCLKEN));               /* 执行语句 */

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
                    Init->WriteBurst           |\
                    Init->ContinuousClock);             /* 执行语句 */
#else /* defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx) */
  /* Clear MBKEN, MUXEN, MTYP, MWID, FACCEN, BURSTEN, WAITPOL, CPSIZE, WAITCFG, WREN,
           WAITEN, EXTMOD, ASYNCWAIT, CBURSTRW, CCLKEN and WFDIS bits */
  tmpr &= ((uint32_t)~(FMC_BCR1_MBKEN     | FMC_BCR1_MUXEN    | FMC_BCR1_MTYP     | \
                       FMC_BCR1_MWID      | FMC_BCR1_FACCEN   | FMC_BCR1_BURSTEN  | \
                       FMC_BCR1_WAITPOL   | FMC_BCR1_WAITCFG  | FMC_BCR1_CPSIZE   | \
                       FMC_BCR1_WREN      | FMC_BCR1_WAITEN   | FMC_BCR1_EXTMOD   | \
                       FMC_BCR1_ASYNCWAIT | FMC_BCR1_CBURSTRW | FMC_BCR1_CCLKEN   | \
                       FMC_BCR1_WFDIS));                /* 执行语句 */

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
                    Init->WriteFifo);                   /* 执行语句 */
#endif /*  defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx) */

  if(Init->MemoryType == FMC_MEMORY_TYPE_NOR)           /* 条件判断 */
  {                                                     /* 代码块开始 */
    tmpr |= (uint32_t)FMC_NORSRAM_FLASH_ACCESS_ENABLE;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  Device->BTCR[Init->NSBank] = tmpr;                    /* 赋值或初始化 */

  /* Configure synchronous mode when Continuous clock is enabled for bank2..4 */
  if((Init->ContinuousClock == FMC_CONTINUOUS_CLOCK_SYNC_ASYNC) && (Init->NSBank != FMC_NORSRAM_BANK1))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    Device->BTCR[FMC_NORSRAM_BANK1] |= (uint32_t)(Init->ContinuousClock);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

#if defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx)  /* 条件编译 */
  if(Init->NSBank != FMC_NORSRAM_BANK1)                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    Device->BTCR[FMC_NORSRAM_BANK1] |= (uint32_t)(Init->WriteFifo);                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
#endif /* STM32F446xx || STM32F469xx || STM32F479xx */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  DeInitialize the FMC_NORSRAM peripheral 
  * @param  Device Pointer to NORSRAM device instance
  * @param  ExDevice Pointer to NORSRAM extended mode device instance  
  * @param  Bank NORSRAM bank number  
  * @retval HAL status
  */
HAL_StatusTypeDef FMC_NORSRAM_DeInit(FMC_NORSRAM_TypeDef *Device, FMC_NORSRAM_EXTENDED_TypeDef *ExDevice, uint32_t Bank)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_FMC_NORSRAM_DEVICE(Device));          /* 执行语句 */
  assert_param(IS_FMC_NORSRAM_EXTENDED_DEVICE(ExDevice));  /* 执行语句 */
  assert_param(IS_FMC_NORSRAM_BANK(Bank));              /* 执行语句 */

  /* Disable the FMC_NORSRAM device */
  __FMC_NORSRAM_DISABLE(Device, Bank);                  /* 执行语句 */

  /* De-initialize the FMC_NORSRAM device */
  /* FMC_NORSRAM_BANK1 */
  if(Bank == FMC_NORSRAM_BANK1)                         /* 条件判断 */
  {                                                     /* 代码块开始 */
    Device->BTCR[Bank] = 0x000030DBU;                   /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  /* FMC_NORSRAM_BANK2, FMC_NORSRAM_BANK3 or FMC_NORSRAM_BANK4 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    Device->BTCR[Bank] = 0x000030D2U;                   /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  Device->BTCR[Bank + 1U] = 0x0FFFFFFFU;                /* 赋值或初始化 */
  ExDevice->BWTR[Bank]    = 0x0FFFFFFFU;                /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initialize the FMC_NORSRAM Timing according to the specified
  *         parameters in the FMC_NORSRAM_TimingTypeDef
  * @param  Device Pointer to NORSRAM device instance
  * @param  Timing Pointer to NORSRAM Timing structure
  * @param  Bank NORSRAM bank number  
  * @retval HAL status
  */
HAL_StatusTypeDef FMC_NORSRAM_Timing_Init(FMC_NORSRAM_TypeDef *Device, FMC_NORSRAM_TimingTypeDef *Timing, uint32_t Bank)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpr = 0U;                                   /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_FMC_NORSRAM_DEVICE(Device));          /* 执行语句 */
  assert_param(IS_FMC_ADDRESS_SETUP_TIME(Timing->AddressSetupTime));  /* 执行语句 */
  assert_param(IS_FMC_ADDRESS_HOLD_TIME(Timing->AddressHoldTime));  /* 执行语句 */
  assert_param(IS_FMC_DATASETUP_TIME(Timing->DataSetupTime));  /* 执行语句 */
  assert_param(IS_FMC_TURNAROUND_TIME(Timing->BusTurnAroundDuration));  /* 执行语句 */
  assert_param(IS_FMC_CLK_DIV(Timing->CLKDivision));    /* 执行语句 */
  assert_param(IS_FMC_DATA_LATENCY(Timing->DataLatency));  /* 执行语句 */
  assert_param(IS_FMC_ACCESS_MODE(Timing->AccessMode));  /* 执行语句 */
  assert_param(IS_FMC_NORSRAM_BANK(Bank));              /* 执行语句 */

  /* Get the BTCR register value */
  tmpr = Device->BTCR[Bank + 1U];                       /* 赋值或初始化 */

  /* Clear ADDSET, ADDHLD, DATAST, BUSTURN, CLKDIV, DATLAT and ACCMOD bits */
  tmpr &= ((uint32_t)~(FMC_BTR1_ADDSET  | FMC_BTR1_ADDHLD | FMC_BTR1_DATAST | \
                       FMC_BTR1_BUSTURN | FMC_BTR1_CLKDIV | FMC_BTR1_DATLAT | \
                       FMC_BTR1_ACCMOD));               /* 执行语句 */

  /* Set FMC_NORSRAM device timing parameters */  
  tmpr |= (uint32_t)(Timing->AddressSetupTime                   |\
                    ((Timing->AddressHoldTime) << 4U)           |\
                    ((Timing->DataSetupTime) << 8U)             |\
                    ((Timing->BusTurnAroundDuration) << 16U)    |\
                    (((Timing->CLKDivision) - 1U) << 20U)         |\
                    (((Timing->DataLatency) - 2U) << 24U)         |\
                    (Timing->AccessMode));              /* 执行语句 */

  Device->BTCR[Bank + 1U] = tmpr;                       /* 赋值或初始化 */

  /* Configure Clock division value (in NORSRAM bank 1) when continuous clock is enabled */
  if(HAL_IS_BIT_SET(Device->BTCR[FMC_NORSRAM_BANK1], FMC_BCR1_CCLKEN))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    tmpr = (uint32_t)(Device->BTCR[FMC_NORSRAM_BANK1 + 1U] & ~(0x0FU << 20U));   /* 赋值或初始化 */
    tmpr |= (uint32_t)(((Timing->CLKDivision) - 1U) << 20U);  /* 赋值或初始化 */
    Device->BTCR[FMC_NORSRAM_BANK1 + 1U] = tmpr;        /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initialize the FMC_NORSRAM Extended mode Timing according to the specified
  *         parameters in the FMC_NORSRAM_TimingTypeDef
  * @param  Device Pointer to NORSRAM device instance
  * @param  Timing Pointer to NORSRAM Timing structure
  * @param  Bank NORSRAM bank number  
  * @retval HAL status
  */
HAL_StatusTypeDef  FMC_NORSRAM_Extended_Timing_Init(FMC_NORSRAM_EXTENDED_TypeDef *Device, FMC_NORSRAM_TimingTypeDef *Timing, uint32_t Bank, uint32_t ExtendedMode)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpr = 0U;                                   /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_FMC_EXTENDED_MODE(ExtendedMode));     /* 执行语句 */

  /* Set NORSRAM device timing register for write configuration, if extended mode is used */
  if(ExtendedMode == FMC_EXTENDED_MODE_ENABLE)          /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check the parameters */
    assert_param(IS_FMC_NORSRAM_EXTENDED_DEVICE(Device));    /* 执行语句 */
    assert_param(IS_FMC_ADDRESS_SETUP_TIME(Timing->AddressSetupTime));  /* 执行语句 */
    assert_param(IS_FMC_ADDRESS_HOLD_TIME(Timing->AddressHoldTime));  /* 执行语句 */
    assert_param(IS_FMC_DATASETUP_TIME(Timing->DataSetupTime));  /* 执行语句 */
    assert_param(IS_FMC_TURNAROUND_TIME(Timing->BusTurnAroundDuration));  /* 执行语句 */
    assert_param(IS_FMC_ACCESS_MODE(Timing->AccessMode));  /* 执行语句 */
    assert_param(IS_FMC_NORSRAM_BANK(Bank));            /* 执行语句 */

    /* Get the BWTR register value */
    tmpr = Device->BWTR[Bank];                          /* 赋值或初始化 */

    /* Clear ADDSET, ADDHLD, DATAST, BUSTURN and ACCMOD bits */
    tmpr &= ((uint32_t)~(FMC_BWTR1_ADDSET  | FMC_BWTR1_ADDHLD | FMC_BWTR1_DATAST | \
                         FMC_BWTR1_BUSTURN | FMC_BWTR1_ACCMOD));  /* 执行语句 */

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

/** @addtogroup FMC_LL_NORSRAM_Private_Functions_Group2
 *  @brief   management functions 
 *
@verbatim   
  ==============================================================================
                      ##### FMC_NORSRAM Control functions #####
  ==============================================================================  
  [..]
    This subsection provides a set of functions allowing to control dynamically
    the FMC NORSRAM interface.

@endverbatim
  * @{
  */
/**
  * @brief  Enables dynamically FMC_NORSRAM write operation.
  * @param  Device Pointer to NORSRAM device instance
  * @param  Bank NORSRAM bank number   
  * @retval HAL status
  */
HAL_StatusTypeDef FMC_NORSRAM_WriteOperation_Enable(FMC_NORSRAM_TypeDef *Device, uint32_t Bank)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_FMC_NORSRAM_DEVICE(Device));          /* 执行语句 */
  assert_param(IS_FMC_NORSRAM_BANK(Bank));              /* 执行语句 */

  /* Enable write operation */
  Device->BTCR[Bank] |= FMC_WRITE_OPERATION_ENABLE;     /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Disables dynamically FMC_NORSRAM write operation.
  * @param  Device Pointer to NORSRAM device instance
  * @param  Bank NORSRAM bank number   
  * @retval HAL status
  */
HAL_StatusTypeDef FMC_NORSRAM_WriteOperation_Disable(FMC_NORSRAM_TypeDef *Device, uint32_t Bank)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_FMC_NORSRAM_DEVICE(Device));          /* 执行语句 */
  assert_param(IS_FMC_NORSRAM_BANK(Bank));              /* 执行语句 */

  /* Disable write operation */
  Device->BTCR[Bank] &= ~FMC_WRITE_OPERATION_ENABLE;    /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup FMC_LL_NAND
  * @brief    NAND Controller functions 
  *
  @verbatim 
  ==============================================================================
                    ##### How to use NAND device driver #####
  ==============================================================================
  [..]
    This driver contains a set of APIs to interface with the FMC NAND banks in order
    to run the NAND external devices.

    (+) FMC NAND bank reset using the function FMC_NAND_DeInit() 
    (+) FMC NAND bank control configuration using the function FMC_NAND_Init()
    (+) FMC NAND bank common space timing configuration using the function 
        FMC_NAND_CommonSpace_Timing_Init()
    (+) FMC NAND bank attribute space timing configuration using the function 
        FMC_NAND_AttributeSpace_Timing_Init()
    (+) FMC NAND bank enable/disable ECC correction feature using the functions
        FMC_NAND_ECC_Enable()/FMC_NAND_ECC_Disable()
    (+) FMC NAND bank get ECC correction code using the function FMC_NAND_GetECC()    

@endverbatim
  * @{
  */

#if defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx)  /* 条件编译 */
/** @defgroup HAL_FMC_NAND_Group1 Initialization/de-initialization functions 
 *  @brief    Initialization and Configuration functions 
 *
@verbatim    
  ==============================================================================
              ##### Initialization and de_initialization functions #####
  ==============================================================================
  [..]  
    This section provides functions allowing to:
    (+) Initialize and configure the FMC NAND interface
    (+) De-initialize the FMC NAND interface 
    (+) Configure the FMC clock and associated GPIOs

@endverbatim
  * @{
  */

/**
  * @brief  Initializes the FMC_NAND device according to the specified
  *         control parameters in the FMC_NAND_HandleTypeDef
  * @param  Device Pointer to NAND device instance
  * @param  Init Pointer to NAND Initialization structure
  * @retval HAL status
  */
HAL_StatusTypeDef FMC_NAND_Init(FMC_NAND_TypeDef *Device, FMC_NAND_InitTypeDef *Init)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpr  = 0U;                                  /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_FMC_NAND_DEVICE(Device));             /* 执行语句 */
  assert_param(IS_FMC_NAND_BANK(Init->NandBank));       /* 执行语句 */
  assert_param(IS_FMC_WAIT_FEATURE(Init->Waitfeature));  /* 执行语句 */
  assert_param(IS_FMC_NAND_MEMORY_WIDTH(Init->MemoryDataWidth));  /* 执行语句 */
  assert_param(IS_FMC_ECC_STATE(Init->EccComputation));  /* 执行语句 */
  assert_param(IS_FMC_ECCPAGE_SIZE(Init->ECCPageSize));  /* 执行语句 */
  assert_param(IS_FMC_TCLR_TIME(Init->TCLRSetupTime));  /* 执行语句 */
  assert_param(IS_FMC_TAR_TIME(Init->TARSetupTime));     /* 执行语句 */

  /* Get the NAND bank register value */
  tmpr = Device->PCR;                                   /* 赋值或初始化 */

  /* Clear PWAITEN, PBKEN, PTYP, PWID, ECCEN, TCLR, TAR and ECCPS bits */
  tmpr &= ((uint32_t)~(FMC_PCR_PWAITEN  | FMC_PCR_PBKEN | FMC_PCR_PTYP | \
                       FMC_PCR_PWID | FMC_PCR_ECCEN | FMC_PCR_TCLR | \
                       FMC_PCR_TAR | FMC_PCR_ECCPS));    /* 执行语句 */

  /* Set NAND device control parameters */
  tmpr |= (uint32_t)(Init->Waitfeature                |\
                     FMC_PCR_MEMORY_TYPE_NAND         |\
                     Init->MemoryDataWidth            |\
                     Init->EccComputation             |\
                     Init->ECCPageSize                |\
                     ((Init->TCLRSetupTime) << 9U)    |\
                     ((Init->TARSetupTime) << 13U));    /* 执行语句 */

  /* NAND bank registers configuration */
  Device->PCR  = tmpr;                                  /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initializes the FMC_NAND Common space Timing according to the specified
  *         parameters in the FMC_NAND_PCC_TimingTypeDef
  * @param  Device Pointer to NAND device instance
  * @param  Timing Pointer to NAND timing structure
  * @param  Bank NAND bank number   
  * @retval HAL status
  */
HAL_StatusTypeDef FMC_NAND_CommonSpace_Timing_Init(FMC_NAND_TypeDef *Device, FMC_NAND_PCC_TimingTypeDef *Timing, uint32_t Bank)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpr = 0U;                                   /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_FMC_NAND_DEVICE(Device));             /* 执行语句 */
  assert_param(IS_FMC_SETUP_TIME(Timing->SetupTime));   /* 执行语句 */
  assert_param(IS_FMC_WAIT_TIME(Timing->WaitSetupTime));  /* 执行语句 */
  assert_param(IS_FMC_HOLD_TIME(Timing->HoldSetupTime));  /* 执行语句 */
  assert_param(IS_FMC_HIZ_TIME(Timing->HiZSetupTime));  /* 执行语句 */
  assert_param(IS_FMC_NAND_BANK(Bank));                 /* 执行语句 */

  /* Get the NAND bank 2 register value */
  tmpr = Device->PMEM;                                  /* 赋值或初始化 */


  /* Clear MEMSETx, MEMWAITx, MEMHOLDx and MEMHIZx bits */
  tmpr &= ((uint32_t)~(FMC_PMEM_MEMSET2  | FMC_PMEM_MEMWAIT2 | FMC_PMEM_MEMHOLD2 | \
                       FMC_PMEM_MEMHIZ2));              /* 执行语句 */

  /* Set FMC_NAND device timing parameters */
  tmpr |= (uint32_t)(Timing->SetupTime                     |\
                       ((Timing->WaitSetupTime) << 8U)     |\
                       ((Timing->HoldSetupTime) << 16U)    |\
                       ((Timing->HiZSetupTime) << 24U)  /* 源代码 */
                       );                               /* 执行语句 */

  /* NAND bank  registers configuration */
  Device->PMEM = tmpr;                                  /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initializes the FMC_NAND Attribute space Timing according to the specified
  *         parameters in the FMC_NAND_PCC_TimingTypeDef
  * @param  Device Pointer to NAND device instance
  * @param  Timing Pointer to NAND timing structure
  * @param  Bank NAND bank number 
  * @retval HAL status
  */
HAL_StatusTypeDef FMC_NAND_AttributeSpace_Timing_Init(FMC_NAND_TypeDef *Device, FMC_NAND_PCC_TimingTypeDef *Timing, uint32_t Bank)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpr = 0U;                                   /* 32位无符号变量/函数 */

  /* Check the parameters */ 
  assert_param(IS_FMC_NAND_DEVICE(Device));             /* 执行语句 */
  assert_param(IS_FMC_SETUP_TIME(Timing->SetupTime));   /* 执行语句 */
  assert_param(IS_FMC_WAIT_TIME(Timing->WaitSetupTime));  /* 执行语句 */
  assert_param(IS_FMC_HOLD_TIME(Timing->HoldSetupTime));  /* 执行语句 */
  assert_param(IS_FMC_HIZ_TIME(Timing->HiZSetupTime));  /* 执行语句 */
  assert_param(IS_FMC_NAND_BANK(Bank));                 /* 执行语句 */

  /* Get the NAND bank  register value */
  tmpr = Device->PATT;                                  /* 赋值或初始化 */

  /* Clear ATTSETx, ATTWAITx, ATTHOLDx and ATTHIZx bits */
  tmpr &= ((uint32_t)~(FMC_PATT_ATTSET2  | FMC_PATT_ATTWAIT2 | FMC_PATT_ATTHOLD2 | \
                       FMC_PATT_ATTHIZ2));              /* 执行语句 */

  /* Set FMC_NAND device timing parameters */
  tmpr |= (uint32_t)(Timing->SetupTime                 |\
                   ((Timing->WaitSetupTime) << 8U)     |\
                   ((Timing->HoldSetupTime) << 16U)    |\
                   ((Timing->HiZSetupTime) << 24U));    /* 执行语句 */

  /* NAND bank registers configuration */
  Device->PATT = tmpr;                                  /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */


/**
  * @brief  DeInitializes the FMC_NAND device 
  * @param  Device Pointer to NAND device instance
  * @param  Bank NAND bank number
  * @retval HAL status
  */
HAL_StatusTypeDef FMC_NAND_DeInit(FMC_NAND_TypeDef *Device, uint32_t Bank)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */ 
  assert_param(IS_FMC_NAND_DEVICE(Device));             /* 执行语句 */
  assert_param(IS_FMC_NAND_BANK(Bank));                 /* 执行语句 */

  /* Disable the NAND Bank */
  __FMC_NAND_DISABLE(Device, Bank);                     /* 执行语句 */

  /* De-initialize the NAND Bank */
  /* Set the FMC_NAND_BANK registers to their reset values */
  Device->PCR  = 0x00000018U;                           /* 赋值或初始化 */
  Device->SR   = 0x00000040U;                           /* 赋值或初始化 */
  Device->PMEM = 0xFCFCFCFCU;                           /* 赋值或初始化 */
  Device->PATT = 0xFCFCFCFCU;                           /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */


/** @defgroup HAL_FMC_NAND_Group2 Control functions 
 *  @brief   management functions 
 *
@verbatim   
  ==============================================================================
                       ##### FMC_NAND Control functions #####
  ==============================================================================  
  [..]
    This subsection provides a set of functions allowing to control dynamically
    the FMC NAND interface.

@endverbatim
  * @{
  */ 


/**
  * @brief  Enables dynamically FMC_NAND ECC feature.
  * @param  Device Pointer to NAND device instance
  * @param  Bank NAND bank number
  * @retval HAL status
  */    
HAL_StatusTypeDef FMC_NAND_ECC_Enable(FMC_NAND_TypeDef *Device, uint32_t Bank)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */ 
  assert_param(IS_FMC_NAND_DEVICE(Device));             /* 执行语句 */
  assert_param(IS_FMC_NAND_BANK(Bank));                 /* 执行语句 */

  /* Enable ECC feature */
  Device->PCR |= FMC_PCR_ECCEN;                         /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */


/**
  * @brief  Disables dynamically FMC_NAND ECC feature.
  * @param  Device Pointer to NAND device instance
  * @param  Bank NAND bank number
  * @retval HAL status
  */  
HAL_StatusTypeDef FMC_NAND_ECC_Disable(FMC_NAND_TypeDef *Device, uint32_t Bank)    /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */ 
  assert_param(IS_FMC_NAND_DEVICE(Device));             /* 执行语句 */
  assert_param(IS_FMC_NAND_BANK(Bank));                 /* 执行语句 */

  /* Disable ECC feature */
  Device->PCR &= ~FMC_PCR_ECCEN;                        /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Disables dynamically FMC_NAND ECC feature.
  * @param  Device Pointer to NAND device instance
  * @param  ECCval Pointer to ECC value
  * @param  Bank NAND bank number
  * @param  Timeout Timeout wait value  
  * @retval HAL status
  */
HAL_StatusTypeDef FMC_NAND_GetECC(FMC_NAND_TypeDef *Device, uint32_t *ECCval, uint32_t Bank, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart = 0U;                              /* 32位无符号变量/函数 */

  /* Check the parameters */ 
  assert_param(IS_FMC_NAND_DEVICE(Device));             /* 执行语句 */
  assert_param(IS_FMC_NAND_BANK(Bank));                 /* 执行语句 */

  /* Get tick */ 
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  /* Wait until FIFO is empty */
  while(__FMC_NAND_GET_FLAG(Device, Bank, FMC_FLAG_FEMPT) == RESET)  /* while循环 */
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

  /* Get the ECCR register value */
  *ECCval = (uint32_t)Device->ECCR;                     /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

#else /* defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx) */
/** @defgroup HAL_FMC_NAND_Group1 Initialization/de-initialization functions 
 *  @brief    Initialization and Configuration functions 
 *
@verbatim    
  ==============================================================================
              ##### Initialization and de_initialization functions #####
  ==============================================================================
  [..]  
    This section provides functions allowing to:
    (+) Initialize and configure the FMC NAND interface
    (+) De-initialize the FMC NAND interface 
    (+) Configure the FMC clock and associated GPIOs

@endverbatim
  * @{
  */
/**
  * @brief  Initializes the FMC_NAND device according to the specified
  *         control parameters in the FMC_NAND_HandleTypeDef
  * @param  Device Pointer to NAND device instance
  * @param  Init Pointer to NAND Initialization structure
  * @retval HAL status
  */
HAL_StatusTypeDef FMC_NAND_Init(FMC_NAND_TypeDef *Device, FMC_NAND_InitTypeDef *Init)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpr  = 0U;                                  /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_FMC_NAND_DEVICE(Device));             /* 执行语句 */
  assert_param(IS_FMC_NAND_BANK(Init->NandBank));       /* 执行语句 */
  assert_param(IS_FMC_WAIT_FEATURE(Init->Waitfeature));  /* 执行语句 */
  assert_param(IS_FMC_NAND_MEMORY_WIDTH(Init->MemoryDataWidth));  /* 执行语句 */
  assert_param(IS_FMC_ECC_STATE(Init->EccComputation));  /* 执行语句 */
  assert_param(IS_FMC_ECCPAGE_SIZE(Init->ECCPageSize));  /* 执行语句 */
  assert_param(IS_FMC_TCLR_TIME(Init->TCLRSetupTime));  /* 执行语句 */
  assert_param(IS_FMC_TAR_TIME(Init->TARSetupTime));     /* 执行语句 */

  if(Init->NandBank == FMC_NAND_BANK2)                  /* 条件判断 */
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
  tmpr &= ((uint32_t)~(FMC_PCR2_PWAITEN  | FMC_PCR2_PBKEN | FMC_PCR2_PTYP | \
                       FMC_PCR2_PWID | FMC_PCR2_ECCEN | FMC_PCR2_TCLR | \
                       FMC_PCR2_TAR | FMC_PCR2_ECCPS));    /* 执行语句 */

  /* Set NAND device control parameters */
  tmpr |= (uint32_t)(Init->Waitfeature                 |\
                     FMC_PCR_MEMORY_TYPE_NAND          |\
                     Init->MemoryDataWidth             |\
                     Init->EccComputation              |\
                     Init->ECCPageSize                 |\
                     ((Init->TCLRSetupTime) << 9U)     |\
                     ((Init->TARSetupTime) << 13U));     /* 执行语句 */

  if(Init->NandBank == FMC_NAND_BANK2)                  /* 条件判断 */
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
  * @brief  Initializes the FMC_NAND Common space Timing according to the specified
  *         parameters in the FMC_NAND_PCC_TimingTypeDef
  * @param  Device Pointer to NAND device instance
  * @param  Timing Pointer to NAND timing structure
  * @param  Bank NAND bank number   
  * @retval HAL status
  */
HAL_StatusTypeDef FMC_NAND_CommonSpace_Timing_Init(FMC_NAND_TypeDef *Device, FMC_NAND_PCC_TimingTypeDef *Timing, uint32_t Bank)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpr = 0U;                                   /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_FMC_NAND_DEVICE(Device));             /* 执行语句 */
  assert_param(IS_FMC_SETUP_TIME(Timing->SetupTime));   /* 执行语句 */
  assert_param(IS_FMC_WAIT_TIME(Timing->WaitSetupTime));  /* 执行语句 */
  assert_param(IS_FMC_HOLD_TIME(Timing->HoldSetupTime));  /* 执行语句 */
  assert_param(IS_FMC_HIZ_TIME(Timing->HiZSetupTime));  /* 执行语句 */
  assert_param(IS_FMC_NAND_BANK(Bank));                 /* 执行语句 */

  if(Bank == FMC_NAND_BANK2)                            /* 条件判断 */
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
  tmpr &= ((uint32_t)~(FMC_PMEM2_MEMSET2  | FMC_PMEM2_MEMWAIT2 | FMC_PMEM2_MEMHOLD2 | \
                       FMC_PMEM2_MEMHIZ2));             /* 执行语句 */

  /* Set FMC_NAND device timing parameters */
  tmpr |= (uint32_t)(Timing->SetupTime                     |\
                       ((Timing->WaitSetupTime) << 8U)     |\
                       ((Timing->HoldSetupTime) << 16U)    |\
                       ((Timing->HiZSetupTime) << 24U)  /* 源代码 */
                       );                               /* 执行语句 */

  if(Bank == FMC_NAND_BANK2)                            /* 条件判断 */
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
  * @brief  Initializes the FMC_NAND Attribute space Timing according to the specified
  *         parameters in the FMC_NAND_PCC_TimingTypeDef
  * @param  Device Pointer to NAND device instance
  * @param  Timing Pointer to NAND timing structure
  * @param  Bank NAND bank number 
  * @retval HAL status
  */
HAL_StatusTypeDef FMC_NAND_AttributeSpace_Timing_Init(FMC_NAND_TypeDef *Device, FMC_NAND_PCC_TimingTypeDef *Timing, uint32_t Bank)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpr = 0U;                                   /* 32位无符号变量/函数 */

  /* Check the parameters */ 
  assert_param(IS_FMC_NAND_DEVICE(Device));             /* 执行语句 */
  assert_param(IS_FMC_SETUP_TIME(Timing->SetupTime));   /* 执行语句 */
  assert_param(IS_FMC_WAIT_TIME(Timing->WaitSetupTime));  /* 执行语句 */
  assert_param(IS_FMC_HOLD_TIME(Timing->HoldSetupTime));  /* 执行语句 */
  assert_param(IS_FMC_HIZ_TIME(Timing->HiZSetupTime));  /* 执行语句 */
  assert_param(IS_FMC_NAND_BANK(Bank));                 /* 执行语句 */

  if(Bank == FMC_NAND_BANK2)                            /* 条件判断 */
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
  tmpr &= ((uint32_t)~(FMC_PATT2_ATTSET2  | FMC_PATT2_ATTWAIT2 | FMC_PATT2_ATTHOLD2 | \
                       FMC_PATT2_ATTHIZ2));             /* 执行语句 */

  /* Set FMC_NAND device timing parameters */
  tmpr |= (uint32_t)(Timing->SetupTime                  |\
                   ((Timing->WaitSetupTime) << 8U)      |\
                   ((Timing->HoldSetupTime) << 16U)     |\
                   ((Timing->HiZSetupTime) << 24U));    /* 执行语句 */

  if(Bank == FMC_NAND_BANK2)                            /* 条件判断 */
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
  * @brief  DeInitializes the FMC_NAND device 
  * @param  Device Pointer to NAND device instance
  * @param  Bank NAND bank number
  * @retval HAL status
  */
HAL_StatusTypeDef FMC_NAND_DeInit(FMC_NAND_TypeDef *Device, uint32_t Bank)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */ 
  assert_param(IS_FMC_NAND_DEVICE(Device));             /* 执行语句 */
  assert_param(IS_FMC_NAND_BANK(Bank));                 /* 执行语句 */

  /* Disable the NAND Bank */
  __FMC_NAND_DISABLE(Device, Bank);                     /* 执行语句 */

  /* De-initialize the NAND Bank */
  if(Bank == FMC_NAND_BANK2)                            /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Set the FMC_NAND_BANK2 registers to their reset values */
    Device->PCR2  = 0x00000018U;                        /* 赋值或初始化 */
    Device->SR2   = 0x00000040U;                        /* 赋值或初始化 */
    Device->PMEM2 = 0xFCFCFCFCU;                        /* 赋值或初始化 */
    Device->PATT2 = 0xFCFCFCFCU;                        /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  /* FMC_Bank3_NAND */  
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Set the FMC_NAND_BANK3 registers to their reset values */
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

/** @addtogroup FMC_LL_NAND_Private_Functions_Group2
  *  @brief   management functions 
  *
@verbatim   
  ==============================================================================
                       ##### FMC_NAND Control functions #####
  ==============================================================================  
  [..]
    This subsection provides a set of functions allowing to control dynamically
    the FMC NAND interface.

@endverbatim
  * @{
  */ 
/**
  * @brief  Enables dynamically FMC_NAND ECC feature.
  * @param  Device Pointer to NAND device instance
  * @param  Bank NAND bank number
  * @retval HAL status
  */    
HAL_StatusTypeDef FMC_NAND_ECC_Enable(FMC_NAND_TypeDef *Device, uint32_t Bank)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */ 
  assert_param(IS_FMC_NAND_DEVICE(Device));             /* 执行语句 */
  assert_param(IS_FMC_NAND_BANK(Bank));                 /* 执行语句 */

  /* Enable ECC feature */
  if(Bank == FMC_NAND_BANK2)                            /* 条件判断 */
  {                                                     /* 代码块开始 */
    Device->PCR2 |= FMC_PCR2_ECCEN;                     /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    Device->PCR3 |= FMC_PCR3_ECCEN;                     /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Disables dynamically FMC_NAND ECC feature.
  * @param  Device Pointer to NAND device instance
  * @param  Bank NAND bank number
  * @retval HAL status
  */  
HAL_StatusTypeDef FMC_NAND_ECC_Disable(FMC_NAND_TypeDef *Device, uint32_t Bank)    /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */ 
  assert_param(IS_FMC_NAND_DEVICE(Device));             /* 执行语句 */
  assert_param(IS_FMC_NAND_BANK(Bank));                 /* 执行语句 */

  /* Disable ECC feature */
  if(Bank == FMC_NAND_BANK2)                            /* 条件判断 */
  {                                                     /* 代码块开始 */
    Device->PCR2 &= ~FMC_PCR2_ECCEN;                    /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    Device->PCR3 &= ~FMC_PCR3_ECCEN;                    /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Disables dynamically FMC_NAND ECC feature.
  * @param  Device Pointer to NAND device instance
  * @param  ECCval Pointer to ECC value
  * @param  Bank NAND bank number
  * @param  Timeout Timeout wait value  
  * @retval HAL status
  */
HAL_StatusTypeDef FMC_NAND_GetECC(FMC_NAND_TypeDef *Device, uint32_t *ECCval, uint32_t Bank, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart = 0U;                              /* 32位无符号变量/函数 */

  /* Check the parameters */ 
  assert_param(IS_FMC_NAND_DEVICE(Device));             /* 执行语句 */
  assert_param(IS_FMC_NAND_BANK(Bank));                 /* 执行语句 */

  /* Get tick */ 
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  /* Wait until FIFO is empty */
  while(__FMC_NAND_GET_FLAG(Device, Bank, FMC_FLAG_FEMPT) == RESET)  /* while循环 */
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

  if(Bank == FMC_NAND_BANK2)                            /* 条件判断 */
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

#endif /* defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx) */
/**
  * @}
  */

#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx)  /* 条件编译 */
/** @addtogroup FMC_LL_PCCARD
  * @brief    PCCARD Controller functions 
  *
  @verbatim 
  ==============================================================================  
                    ##### How to use PCCARD device driver #####
  ==============================================================================
  [..]
    This driver contains a set of APIs to interface with the FMC PCCARD bank in order
    to run the PCCARD/compact flash external devices.

    (+) FMC PCCARD bank reset using the function FMC_PCCARD_DeInit() 
    (+) FMC PCCARD bank control configuration using the function FMC_PCCARD_Init()
    (+) FMC PCCARD bank common space timing configuration using the function 
        FMC_PCCARD_CommonSpace_Timing_Init()
    (+) FMC PCCARD bank attribute space timing configuration using the function 
        FMC_PCCARD_AttributeSpace_Timing_Init()
    (+) FMC PCCARD bank IO space timing configuration using the function 
        FMC_PCCARD_IOSpace_Timing_Init()
@endverbatim
  * @{
  */

/** @addtogroup FMC_LL_PCCARD_Private_Functions_Group1
  *  @brief    Initialization and Configuration functions 
  *
@verbatim    
  ==============================================================================
              ##### Initialization and de_initialization functions #####
  ==============================================================================
  [..]  
    This section provides functions allowing to:
    (+) Initialize and configure the FMC PCCARD interface
    (+) De-initialize the FMC PCCARD interface 
    (+) Configure the FMC clock and associated GPIOs

@endverbatim
  * @{
  */

/**
  * @brief  Initializes the FMC_PCCARD device according to the specified
  *         control parameters in the FMC_PCCARD_HandleTypeDef
  * @param  Device Pointer to PCCARD device instance
  * @param  Init Pointer to PCCARD Initialization structure   
  * @retval HAL status
  */
HAL_StatusTypeDef FMC_PCCARD_Init(FMC_PCCARD_TypeDef *Device, FMC_PCCARD_InitTypeDef *Init)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpr = 0U;                                   /* 32位无符号变量/函数 */

  /* Check the parameters */ 
  assert_param(IS_FMC_PCCARD_DEVICE(Device));           /* 执行语句 */
  assert_param(IS_FMC_WAIT_FEATURE(Init->Waitfeature));  /* 执行语句 */
  assert_param(IS_FMC_TCLR_TIME(Init->TCLRSetupTime));  /* 执行语句 */
  assert_param(IS_FMC_TAR_TIME(Init->TARSetupTime));       /* 执行语句 */

  /* Get PCCARD control register value */
  tmpr = Device->PCR4;                                  /* 赋值或初始化 */

  /* Clear TAR, TCLR, PWAITEN and PWID bits */
  tmpr &= ((uint32_t)~(FMC_PCR4_TAR  | FMC_PCR4_TCLR | FMC_PCR4_PWAITEN | \
                       FMC_PCR4_PWID | FMC_PCR4_PTYP));  /* 执行语句 */

  /* Set FMC_PCCARD device control parameters */
  tmpr |= (uint32_t)(Init->Waitfeature              |\
                     FMC_NAND_PCC_MEM_BUS_WIDTH_16  |\
                    (Init->TCLRSetupTime << 9U)     |\
                    (Init->TARSetupTime << 13U));       /* 执行语句 */

  Device->PCR4 = tmpr;                                  /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initializes the FMC_PCCARD Common space Timing according to the specified
  *         parameters in the FMC_NAND_PCC_TimingTypeDef
  * @param  Device Pointer to PCCARD device instance
  * @param  Timing Pointer to PCCARD timing structure 
  * @retval HAL status
  */
HAL_StatusTypeDef FMC_PCCARD_CommonSpace_Timing_Init(FMC_PCCARD_TypeDef *Device, FMC_NAND_PCC_TimingTypeDef *Timing)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpr = 0U;                                   /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_FMC_PCCARD_DEVICE(Device));           /* 执行语句 */
  assert_param(IS_FMC_SETUP_TIME(Timing->SetupTime));   /* 执行语句 */
  assert_param(IS_FMC_WAIT_TIME(Timing->WaitSetupTime));  /* 执行语句 */
  assert_param(IS_FMC_HOLD_TIME(Timing->HoldSetupTime));  /* 执行语句 */
  assert_param(IS_FMC_HIZ_TIME(Timing->HiZSetupTime));  /* 执行语句 */

  /* Get PCCARD common space timing register value */
  tmpr = Device->PMEM4;                                 /* 赋值或初始化 */

  /* Clear MEMSETx, MEMWAITx, MEMHOLDx and MEMHIZx bits */
  tmpr &= ((uint32_t)~(FMC_PMEM4_MEMSET4  | FMC_PMEM4_MEMWAIT4 | FMC_PMEM4_MEMHOLD4 | \
                       FMC_PMEM4_MEMHIZ4));             /* 执行语句 */
  /* Set PCCARD timing parameters */
  tmpr |= (uint32_t)(Timing->SetupTime                  |\
                    ((Timing->WaitSetupTime) << 8U)     |\
                    ((Timing->HoldSetupTime) << 16U)    |\
                    ((Timing->HiZSetupTime) << 24U));   /* 执行语句 */

  Device->PMEM4 = tmpr;                                 /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initializes the FMC_PCCARD Attribute space Timing according to the specified
  *         parameters in the FMC_NAND_PCC_TimingTypeDef
  * @param  Device Pointer to PCCARD device instance
  * @param  Timing Pointer to PCCARD timing structure  
  * @retval HAL status
  */
HAL_StatusTypeDef FMC_PCCARD_AttributeSpace_Timing_Init(FMC_PCCARD_TypeDef *Device, FMC_NAND_PCC_TimingTypeDef *Timing)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpr = 0U;                                   /* 32位无符号变量/函数 */

  /* Check the parameters */ 
  assert_param(IS_FMC_PCCARD_DEVICE(Device));           /* 执行语句 */
  assert_param(IS_FMC_SETUP_TIME(Timing->SetupTime));   /* 执行语句 */
  assert_param(IS_FMC_WAIT_TIME(Timing->WaitSetupTime));  /* 执行语句 */
  assert_param(IS_FMC_HOLD_TIME(Timing->HoldSetupTime));  /* 执行语句 */
  assert_param(IS_FMC_HIZ_TIME(Timing->HiZSetupTime));  /* 执行语句 */

  /* Get PCCARD timing parameters */
  tmpr = Device->PATT4;                                 /* 赋值或初始化 */

  /* Clear ATTSETx, ATTWAITx, ATTHOLDx and ATTHIZx bits */
  tmpr &= ((uint32_t)~(FMC_PATT4_ATTSET4  | FMC_PATT4_ATTWAIT4 | FMC_PATT4_ATTHOLD4 | \
                       FMC_PATT4_ATTHIZ4));             /* 执行语句 */

  /* Set PCCARD timing parameters */
  tmpr |= (uint32_t)(Timing->SetupTime                 |\
                   ((Timing->WaitSetupTime) << 8U)     |\
                   ((Timing->HoldSetupTime) << 16U)    |\
                   ((Timing->HiZSetupTime) << 24U));    /* 执行语句 */
  Device->PATT4 = tmpr;                                 /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initializes the FMC_PCCARD IO space Timing according to the specified
  *         parameters in the FMC_NAND_PCC_TimingTypeDef
  * @param  Device Pointer to PCCARD device instance
  * @param  Timing Pointer to PCCARD timing structure  
  * @retval HAL status
  */
HAL_StatusTypeDef FMC_PCCARD_IOSpace_Timing_Init(FMC_PCCARD_TypeDef *Device, FMC_NAND_PCC_TimingTypeDef *Timing)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpr = 0;                                    /* 32位无符号变量/函数 */

  /* Check the parameters */  
  assert_param(IS_FMC_PCCARD_DEVICE(Device));           /* 执行语句 */
  assert_param(IS_FMC_SETUP_TIME(Timing->SetupTime));   /* 执行语句 */
  assert_param(IS_FMC_WAIT_TIME(Timing->WaitSetupTime));  /* 执行语句 */
  assert_param(IS_FMC_HOLD_TIME(Timing->HoldSetupTime));  /* 执行语句 */
  assert_param(IS_FMC_HIZ_TIME(Timing->HiZSetupTime));  /* 执行语句 */

  /* Get FMC_PCCARD device timing parameters */
  tmpr = Device->PIO4;                                  /* 赋值或初始化 */

  /* Clear IOSET4, IOWAIT4, IOHOLD4 and IOHIZ4 bits */
  tmpr &= ((uint32_t)~(FMC_PIO4_IOSET4  | FMC_PIO4_IOWAIT4 | FMC_PIO4_IOHOLD4 | \
                       FMC_PIO4_IOHIZ4));               /* 执行语句 */

  /* Set FMC_PCCARD device timing parameters */
  tmpr |= (uint32_t)(Timing->SetupTime                   |\
                     ((Timing->WaitSetupTime) << 8U)     |\
                     ((Timing->HoldSetupTime) << 16U)    |\
                     ((Timing->HiZSetupTime) << 24U));     /* 执行语句 */

  Device->PIO4 = tmpr;                                  /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  DeInitializes the FMC_PCCARD device 
  * @param  Device Pointer to PCCARD device instance
  * @retval HAL status
  */
HAL_StatusTypeDef FMC_PCCARD_DeInit(FMC_PCCARD_TypeDef *Device)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */  
  assert_param(IS_FMC_PCCARD_DEVICE(Device));           /* 执行语句 */

  /* Disable the FMC_PCCARD device */
  __FMC_PCCARD_DISABLE(Device);                         /* 执行语句 */

  /* De-initialize the FMC_PCCARD device */
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
#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx */


/** @addtogroup FMC_LL_SDRAM
  * @brief    SDRAM Controller functions 
  *
  @verbatim 
  ==============================================================================
                     ##### How to use SDRAM device driver #####
  ==============================================================================
  [..] 
    This driver contains a set of APIs to interface with the FMC SDRAM banks in order
    to run the SDRAM external devices.

    (+) FMC SDRAM bank reset using the function FMC_SDRAM_DeInit() 
    (+) FMC SDRAM bank control configuration using the function FMC_SDRAM_Init()
    (+) FMC SDRAM bank timing configuration using the function FMC_SDRAM_Timing_Init()
    (+) FMC SDRAM bank enable/disable write operation using the functions
        FMC_SDRAM_WriteOperation_Enable()/FMC_SDRAM_WriteOperation_Disable()   
    (+) FMC SDRAM bank send command using the function FMC_SDRAM_SendCommand()      

@endverbatim
  * @{
  */

/** @addtogroup FMC_LL_SDRAM_Private_Functions_Group1
  *  @brief    Initialization and Configuration functions 
  *
@verbatim    
  ==============================================================================
              ##### Initialization and de_initialization functions #####
  ==============================================================================
  [..]  
    This section provides functions allowing to:
    (+) Initialize and configure the FMC SDRAM interface
    (+) De-initialize the FMC SDRAM interface 
    (+) Configure the FMC clock and associated GPIOs

@endverbatim
  * @{
  */

/**
  * @brief  Initializes the FMC_SDRAM device according to the specified
  *         control parameters in the FMC_SDRAM_InitTypeDef
  * @param  Device Pointer to SDRAM device instance
  * @param  Init Pointer to SDRAM Initialization structure   
  * @retval HAL status
  */
HAL_StatusTypeDef FMC_SDRAM_Init(FMC_SDRAM_TypeDef *Device, FMC_SDRAM_InitTypeDef *Init)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpr1 = 0U;                                  /* 32位无符号变量/函数 */
  uint32_t tmpr2 = 0U;                                  /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_FMC_SDRAM_DEVICE(Device));            /* 执行语句 */
  assert_param(IS_FMC_SDRAM_BANK(Init->SDBank));        /* 执行语句 */
  assert_param(IS_FMC_COLUMNBITS_NUMBER(Init->ColumnBitsNumber));  /* 执行语句 */
  assert_param(IS_FMC_ROWBITS_NUMBER(Init->RowBitsNumber));  /* 执行语句 */
  assert_param(IS_FMC_SDMEMORY_WIDTH(Init->MemoryDataWidth));  /* 执行语句 */
  assert_param(IS_FMC_INTERNALBANK_NUMBER(Init->InternalBankNumber));  /* 执行语句 */
  assert_param(IS_FMC_CAS_LATENCY(Init->CASLatency));   /* 执行语句 */
  assert_param(IS_FMC_WRITE_PROTECTION(Init->WriteProtection));  /* 执行语句 */
  assert_param(IS_FMC_SDCLOCK_PERIOD(Init->SDClockPeriod));  /* 执行语句 */
  assert_param(IS_FMC_READ_BURST(Init->ReadBurst));     /* 执行语句 */
  assert_param(IS_FMC_READPIPE_DELAY(Init->ReadPipeDelay));     /* 执行语句 */

  /* Set SDRAM bank configuration parameters */
  if (Init->SDBank != FMC_SDRAM_BANK2)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    tmpr1 = Device->SDCR[FMC_SDRAM_BANK1];              /* 赋值或初始化 */

    /* Clear NC, NR, MWID, NB, CAS, WP, SDCLK, RBURST, and RPIPE bits */
    tmpr1 &= ((uint32_t)~(FMC_SDCR1_NC  | FMC_SDCR1_NR | FMC_SDCR1_MWID | \
                         FMC_SDCR1_NB  | FMC_SDCR1_CAS | FMC_SDCR1_WP | \
                         FMC_SDCR1_SDCLK | FMC_SDCR1_RBURST | FMC_SDCR1_RPIPE));  /* 执行语句 */


    tmpr1 |= (uint32_t)(Init->ColumnBitsNumber   |\
                                               Init->RowBitsNumber      |\
                                               Init->MemoryDataWidth    |\
                                               Init->InternalBankNumber |\
                                               Init->CASLatency         |\
                                               Init->WriteProtection    |\
                                               Init->SDClockPeriod      |\
                                               Init->ReadBurst          |\
                                               Init->ReadPipeDelay  /* 源代码 */
                                               );       /* 执行语句 */
    Device->SDCR[FMC_SDRAM_BANK1] = tmpr1;              /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else /* FMC_Bank2_SDRAM */                      
  {                                                     /* 代码块开始 */
    tmpr1 = Device->SDCR[FMC_SDRAM_BANK1];              /* 赋值或初始化 */

    /* Clear NC, NR, MWID, NB, CAS, WP, SDCLK, RBURST, and RPIPE bits */
    tmpr1 &= ((uint32_t)~(FMC_SDCR1_SDCLK | FMC_SDCR1_RBURST | FMC_SDCR1_RPIPE));  /* 赋值或初始化 */

    tmpr1 |= (uint32_t)(Init->SDClockPeriod      |\
                        Init->ReadBurst          |\
                        Init->ReadPipeDelay);           /* 执行语句 */

    tmpr2 = Device->SDCR[FMC_SDRAM_BANK2];              /* 赋值或初始化 */

    /* Clear NC, NR, MWID, NB, CAS, WP, SDCLK, RBURST, and RPIPE bits */
    tmpr2 &= ((uint32_t)~(FMC_SDCR1_NC  | FMC_SDCR1_NR | FMC_SDCR1_MWID | \
                          FMC_SDCR1_NB  | FMC_SDCR1_CAS | FMC_SDCR1_WP | \
                          FMC_SDCR1_SDCLK | FMC_SDCR1_RBURST | FMC_SDCR1_RPIPE));  /* 执行语句 */

    tmpr2 |= (uint32_t)(Init->ColumnBitsNumber   |\
                       Init->RowBitsNumber      |\
                       Init->MemoryDataWidth    |\
                       Init->InternalBankNumber |\
                       Init->CASLatency         |\
                       Init->WriteProtection);          /* 执行语句 */

    Device->SDCR[FMC_SDRAM_BANK1] = tmpr1;              /* 赋值或初始化 */
    Device->SDCR[FMC_SDRAM_BANK2] = tmpr2;              /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initializes the FMC_SDRAM device timing according to the specified
  *         parameters in the FMC_SDRAM_TimingTypeDef
  * @param  Device Pointer to SDRAM device instance
  * @param  Timing Pointer to SDRAM Timing structure
  * @param  Bank SDRAM bank number   
  * @retval HAL status
  */
HAL_StatusTypeDef FMC_SDRAM_Timing_Init(FMC_SDRAM_TypeDef *Device, FMC_SDRAM_TimingTypeDef *Timing, uint32_t Bank)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpr1 = 0U;                                  /* 32位无符号变量/函数 */
  uint32_t tmpr2 = 0U;                                  /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_FMC_SDRAM_DEVICE(Device));            /* 执行语句 */
  assert_param(IS_FMC_LOADTOACTIVE_DELAY(Timing->LoadToActiveDelay));  /* 执行语句 */
  assert_param(IS_FMC_EXITSELFREFRESH_DELAY(Timing->ExitSelfRefreshDelay));  /* 执行语句 */
  assert_param(IS_FMC_SELFREFRESH_TIME(Timing->SelfRefreshTime));  /* 执行语句 */
  assert_param(IS_FMC_ROWCYCLE_DELAY(Timing->RowCycleDelay));  /* 执行语句 */
  assert_param(IS_FMC_WRITE_RECOVERY_TIME(Timing->WriteRecoveryTime));  /* 执行语句 */
  assert_param(IS_FMC_RP_DELAY(Timing->RPDelay));       /* 执行语句 */
  assert_param(IS_FMC_RCD_DELAY(Timing->RCDDelay));     /* 执行语句 */
  assert_param(IS_FMC_SDRAM_BANK(Bank));                /* 执行语句 */

  /* Set SDRAM device timing parameters */ 
  if (Bank != FMC_SDRAM_BANK2)                          /* 条件判断 */
  {                                                     /* 代码块开始 */
    tmpr1 = Device->SDTR[FMC_SDRAM_BANK1];              /* 赋值或初始化 */

    /* Clear TMRD, TXSR, TRAS, TRC, TWR, TRP and TRCD bits */
    tmpr1 &= ((uint32_t)~(FMC_SDTR1_TMRD  | FMC_SDTR1_TXSR | FMC_SDTR1_TRAS | \
                          FMC_SDTR1_TRC  | FMC_SDTR1_TWR | FMC_SDTR1_TRP | \
                          FMC_SDTR1_TRCD));             /* 执行语句 */

    tmpr1 |= (uint32_t)(((Timing->LoadToActiveDelay)-1U)           |\
                       (((Timing->ExitSelfRefreshDelay)-1U) << 4U) |\
                       (((Timing->SelfRefreshTime)-1U) << 8U)      |\
                       (((Timing->RowCycleDelay)-1U) << 12U)       |\
                       (((Timing->WriteRecoveryTime)-1U) <<16U)    |\
                       (((Timing->RPDelay)-1U) << 20U)             |\
                       (((Timing->RCDDelay)-1U) << 24U));  /* 执行语句 */
    Device->SDTR[FMC_SDRAM_BANK1] = tmpr1;              /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
   else /* FMC_Bank2_SDRAM */
  {                                                     /* 代码块开始 */
    tmpr1 = Device->SDTR[FMC_SDRAM_BANK1];              /* 赋值或初始化 */

    /* Clear TRC and TRP bits */
    tmpr1 &= ((uint32_t)~(FMC_SDTR1_TRC | FMC_SDTR1_TRP));  /* 赋值或初始化 */

    tmpr1 |= (uint32_t)((((Timing->RowCycleDelay)-1U) << 12U)       |\
                        (((Timing->RPDelay)-1U) << 20U));   /* 执行语句 */

    tmpr2 = Device->SDTR[FMC_SDRAM_BANK2];              /* 赋值或初始化 */

    /* Clear TMRD, TXSR, TRAS, TRC, TWR, TRP and TRCD bits */
    tmpr2 &= ((uint32_t)~(FMC_SDTR1_TMRD  | FMC_SDTR1_TXSR | FMC_SDTR1_TRAS | \
                          FMC_SDTR1_TRC  | FMC_SDTR1_TWR | FMC_SDTR1_TRP | \
                          FMC_SDTR1_TRCD));             /* 执行语句 */

    tmpr2 |= (uint32_t)((((Timing->LoadToActiveDelay)-1U)           |\
                       (((Timing->ExitSelfRefreshDelay)-1U) << 4U)  |\
                       (((Timing->SelfRefreshTime)-1U) << 8U)       |\
                       (((Timing->WriteRecoveryTime)-1U) <<16U)     |\
                       (((Timing->RCDDelay)-1U) << 24U)));     /* 执行语句 */

    Device->SDTR[FMC_SDRAM_BANK1] = tmpr1;              /* 赋值或初始化 */
    Device->SDTR[FMC_SDRAM_BANK2] = tmpr2;              /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  DeInitializes the FMC_SDRAM peripheral 
  * @param  Device Pointer to SDRAM device instance
  * @retval HAL status
  */
HAL_StatusTypeDef FMC_SDRAM_DeInit(FMC_SDRAM_TypeDef *Device, uint32_t Bank)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_FMC_SDRAM_DEVICE(Device));            /* 执行语句 */
  assert_param(IS_FMC_SDRAM_BANK(Bank));                /* 执行语句 */

  /* De-initialize the SDRAM device */
  Device->SDCR[Bank] = 0x000002D0U;                     /* 赋值或初始化 */
  Device->SDTR[Bank] = 0x0FFFFFFFU;                     /* 赋值或初始化 */
  Device->SDCMR      = 0x00000000U;                     /* 赋值或初始化 */
  Device->SDRTR      = 0x00000000U;                     /* 赋值或初始化 */
  Device->SDSR       = 0x00000000U;                     /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @addtogroup FMC_LL_SDRAMPrivate_Functions_Group2
  *  @brief   management functions 
  *
@verbatim   
  ==============================================================================
                      ##### FMC_SDRAM Control functions #####
  ==============================================================================  
  [..]
    This subsection provides a set of functions allowing to control dynamically
    the FMC SDRAM interface.

@endverbatim
  * @{
  */
/**
  * @brief  Enables dynamically FMC_SDRAM write protection.
  * @param  Device Pointer to SDRAM device instance
  * @param  Bank SDRAM bank number 
  * @retval HAL status
  */
HAL_StatusTypeDef FMC_SDRAM_WriteProtection_Enable(FMC_SDRAM_TypeDef *Device, uint32_t Bank)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_FMC_SDRAM_DEVICE(Device));            /* 执行语句 */
  assert_param(IS_FMC_SDRAM_BANK(Bank));                /* 执行语句 */

  /* Enable write protection */
  Device->SDCR[Bank] |= FMC_SDRAM_WRITE_PROTECTION_ENABLE;  /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Disables dynamically FMC_SDRAM write protection.
  * @param  hsdram FMC_SDRAM handle
  * @retval HAL status
  */
HAL_StatusTypeDef FMC_SDRAM_WriteProtection_Disable(FMC_SDRAM_TypeDef *Device, uint32_t Bank)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_FMC_SDRAM_DEVICE(Device));            /* 执行语句 */
  assert_param(IS_FMC_SDRAM_BANK(Bank));                /* 执行语句 */

  /* Disable write protection */
  Device->SDCR[Bank] &= ~FMC_SDRAM_WRITE_PROTECTION_ENABLE;  /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Send Command to the FMC SDRAM bank
  * @param  Device Pointer to SDRAM device instance
  * @param  Command Pointer to SDRAM command structure   
  * @param  Timing Pointer to SDRAM Timing structure
  * @param  Timeout Timeout wait value
  * @retval HAL state
  */  
HAL_StatusTypeDef FMC_SDRAM_SendCommand(FMC_SDRAM_TypeDef *Device, FMC_SDRAM_CommandTypeDef *Command, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  __IO uint32_t tmpr = 0U;                              /* 赋值或初始化 */
  uint32_t tickstart = 0U;                              /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_FMC_SDRAM_DEVICE(Device));            /* 执行语句 */
  assert_param(IS_FMC_COMMAND_MODE(Command->CommandMode));  /* 执行语句 */
  assert_param(IS_FMC_COMMAND_TARGET(Command->CommandTarget));  /* 执行语句 */
  assert_param(IS_FMC_AUTOREFRESH_NUMBER(Command->AutoRefreshNumber));  /* 执行语句 */
  assert_param(IS_FMC_MODE_REGISTER(Command->ModeRegisterDefinition));    /* 执行语句 */

  /* Set command register */
  tmpr = (uint32_t)((Command->CommandMode)                  |\
                    (Command->CommandTarget)                |\
                    (((Command->AutoRefreshNumber)-1U) << 5U) |\
                    ((Command->ModeRegisterDefinition) << 9U)  /* 源代码 */
                    );                                  /* 执行语句 */

  Device->SDCMR = tmpr;                                 /* 赋值或初始化 */

  /* Get tick */ 
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  /* Wait until command is send */
  while(HAL_IS_BIT_SET(Device->SDSR, FMC_SDSR_BUSY))    /* while循环 */
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

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Program the SDRAM Memory Refresh rate.
  * @param  Device Pointer to SDRAM device instance  
  * @param  RefreshRate The SDRAM refresh rate value.
  * @retval HAL state
  */
HAL_StatusTypeDef FMC_SDRAM_ProgramRefreshRate(FMC_SDRAM_TypeDef *Device, uint32_t RefreshRate)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_FMC_SDRAM_DEVICE(Device));            /* 执行语句 */
  assert_param(IS_FMC_REFRESH_RATE(RefreshRate));       /* 执行语句 */

  /* Set the refresh rate in command register */
  Device->SDRTR |= (RefreshRate<<1U);                   /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Set the Number of consecutive SDRAM Memory auto Refresh commands.
  * @param  Device Pointer to SDRAM device instance  
  * @param  AutoRefreshNumber Specifies the auto Refresh number.       
  * @retval None
  */
HAL_StatusTypeDef FMC_SDRAM_SetAutoRefreshNumber(FMC_SDRAM_TypeDef *Device, uint32_t AutoRefreshNumber)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_FMC_SDRAM_DEVICE(Device));            /* 执行语句 */
  assert_param(IS_FMC_AUTOREFRESH_NUMBER(AutoRefreshNumber));  /* 执行语句 */

  /* Set the Auto-refresh number in command register */
  Device->SDCMR |= (AutoRefreshNumber << 5U);           /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Returns the indicated FMC SDRAM bank mode status.
  * @param  Device Pointer to SDRAM device instance  
  * @param  Bank Defines the FMC SDRAM bank. This parameter can be 
  *                     FMC_Bank1_SDRAM or FMC_Bank2_SDRAM. 
  * @retval The FMC SDRAM bank mode status, could be on of the following values:
  *         FMC_SDRAM_NORMAL_MODE, FMC_SDRAM_SELF_REFRESH_MODE or 
  *         FMC_SDRAM_POWER_DOWN_MODE.           
  */
uint32_t FMC_SDRAM_GetModeStatus(FMC_SDRAM_TypeDef *Device, uint32_t Bank)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpreg = 0U;                                 /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_FMC_SDRAM_DEVICE(Device));            /* 执行语句 */
  assert_param(IS_FMC_SDRAM_BANK(Bank));                /* 执行语句 */

  /* Get the corresponding bank mode */
  if(Bank == FMC_SDRAM_BANK1)                           /* 条件判断 */
  {                                                     /* 代码块开始 */
    tmpreg = (uint32_t)(Device->SDSR & FMC_SDSR_MODES1);   /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    tmpreg = ((uint32_t)(Device->SDSR & FMC_SDSR_MODES2) >> 2U);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Return the mode status */
  return tmpreg;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx || STM32F446xx || STM32F469xx || STM32F479xx */
#endif /* HAL_SRAM_MODULE_ENABLED || HAL_NOR_MODULE_ENABLED || HAL_NAND_MODULE_ENABLED || HAL_PCCARD_MODULE_ENABLED || HAL_SDRAM_MODULE_ENABLED */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
