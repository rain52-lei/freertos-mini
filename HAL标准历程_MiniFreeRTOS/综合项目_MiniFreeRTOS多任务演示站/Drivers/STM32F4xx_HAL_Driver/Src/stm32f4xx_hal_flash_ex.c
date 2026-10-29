/* 文件: stm32f4xx_hal_flash_ex.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_flash_ex.c
  * @author  MCD Application Team
  * @brief   Extended FLASH HAL module driver.
  *          This file provides firmware functions to manage the following 
  *          functionalities of the FLASH extension peripheral:
  *           + Extended programming operations functions
  *  
  @verbatim
  ==============================================================================
                   ##### Flash Extension features #####
  ==============================================================================

  [..] Comparing to other previous devices, the FLASH interface for STM32F427xx/437xx and 
       STM32F429xx/439xx devices contains the following additional features 

       (+) Capacity up to 2 Mbyte with dual bank architecture supporting read-while-write
           capability (RWW)
       (+) Dual bank memory organization       
       (+) PCROP protection for all banks

                      ##### How to use this driver #####
  ==============================================================================
  [..] This driver provides functions to configure and program the FLASH memory 
       of all STM32F427xx/437xx, STM32F429xx/439xx, STM32F469xx/479xx and STM32F446xx 
       devices. It includes
      (#) FLASH Memory Erase functions: 
           (++) Lock and Unlock the FLASH interface using HAL_FLASH_Unlock() and 
                HAL_FLASH_Lock() functions
           (++) Erase function: Erase sector, erase all sectors
           (++) There are two modes of erase :
             (+++) Polling Mode using HAL_FLASHEx_Erase()
             (+++) Interrupt Mode using HAL_FLASHEx_Erase_IT()

      (#) Option Bytes Programming functions: Use HAL_FLASHEx_OBProgram() to :
           (++) Set/Reset the write protection
           (++) Set the Read protection Level
           (++) Set the BOR level
           (++) Program the user Option Bytes
      (#) Advanced Option Bytes Programming functions: Use HAL_FLASHEx_AdvOBProgram() to :  
       (++) Extended space (bank 2) erase function
       (++) Full FLASH space (2 Mo) erase (bank 1 and bank 2)
       (++) Dual Boot activation
       (++) Write protection configuration for bank 2
       (++) PCROP protection configuration and control for both banks

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

/** @defgroup FLASHEx FLASHEx
  * @brief FLASH HAL Extension module driver
  * @{
  */

#ifdef HAL_FLASH_MODULE_ENABLED                         /* 条件编译 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @addtogroup FLASHEx_Private_Constants
  * @{
  */    
#define FLASH_TIMEOUT_VALUE       50000U /* 50 s */
/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/** @addtogroup FLASHEx_Private_Variables
  * @{
  */    
extern FLASH_ProcessTypeDef pFlash;                     /* 外部符号声明 */
/**
  * @}
  */

/* Private function prototypes -----------------------------------------------*/
/** @addtogroup FLASHEx_Private_Functions
  * @{
  */
/* Option bytes control */
static void               FLASH_MassErase(uint8_t VoltageRange, uint32_t Banks);  /* 静态函数 */
static HAL_StatusTypeDef  FLASH_OB_EnableWRP(uint32_t WRPSector, uint32_t Banks);  /* 静态变量/函数 */
static HAL_StatusTypeDef  FLASH_OB_DisableWRP(uint32_t WRPSector, uint32_t Banks);  /* 静态变量/函数 */
static HAL_StatusTypeDef  FLASH_OB_RDP_LevelConfig(uint8_t Level);  /* 静态变量/函数 */
static HAL_StatusTypeDef  FLASH_OB_UserConfig(uint8_t Iwdg, uint8_t Stop, uint8_t Stdby);  /* 静态变量/函数 */
static HAL_StatusTypeDef  FLASH_OB_BOR_LevelConfig(uint8_t Level);  /* 静态变量/函数 */
static uint8_t            FLASH_OB_GetUser(void);       /* 静态变量/函数 */
static uint16_t           FLASH_OB_GetWRP(void);        /* 静态变量/函数 */
static uint8_t            FLASH_OB_GetRDP(void);        /* 静态变量/函数 */
static uint8_t            FLASH_OB_GetBOR(void);        /* 静态变量/函数 */

#if defined(STM32F401xC) || defined(STM32F401xE) || defined(STM32F410Tx) || defined(STM32F410Cx) || defined(STM32F410Rx) || defined(STM32F411xE) ||\
    defined(STM32F446xx) || defined(STM32F412Zx) || defined(STM32F412Vx) || defined(STM32F412Rx) || defined(STM32F412Cx) || defined(STM32F413xx) ||\
    defined(STM32F423xx)                                /* 源代码 */
static HAL_StatusTypeDef  FLASH_OB_EnablePCROP(uint32_t Sector);  /* 静态变量/函数 */
static HAL_StatusTypeDef  FLASH_OB_DisablePCROP(uint32_t Sector);  /* 静态变量/函数 */
#endif /* STM32F401xC || STM32F401xE || STM32F410xx || STM32F411xE || STM32F446xx || STM32F412Zx || STM32F412Vx || STM32F412Rx || STM32F412Cx
          STM32F413xx || STM32F423xx */

#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx)|| defined(STM32F439xx) || defined(STM32F469xx) || defined(STM32F479xx)   /* 条件编译 */
static HAL_StatusTypeDef FLASH_OB_EnablePCROP(uint32_t SectorBank1, uint32_t SectorBank2, uint32_t Banks);  /* 静态变量/函数 */
static HAL_StatusTypeDef FLASH_OB_DisablePCROP(uint32_t SectorBank1, uint32_t SectorBank2, uint32_t Banks);  /* 静态变量/函数 */
static HAL_StatusTypeDef FLASH_OB_BootConfig(uint8_t BootConfig);  /* 静态变量/函数 */
#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx || STM32F469xx || STM32F479xx */

extern HAL_StatusTypeDef         FLASH_WaitForLastOperation(uint32_t Timeout);  /* 外部符号声明 */
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup FLASHEx_Exported_Functions FLASHEx Exported Functions
  * @{
  */

/** @defgroup FLASHEx_Exported_Functions_Group1 Extended IO operation functions
 *  @brief   Extended IO operation functions 
 *
@verbatim   
 ===============================================================================
                ##### Extended programming operation functions #####
 ===============================================================================  
    [..]
    This subsection provides a set of functions allowing to manage the Extension FLASH 
    programming operations.

@endverbatim
  * @{
  */
/**
  * @brief  Perform a mass erase or erase the specified FLASH memory sectors 
  * @param[in]  pEraseInit pointer to an FLASH_EraseInitTypeDef structure that
  *         contains the configuration information for the erasing.
  * 
  * @param[out]  SectorError pointer to variable  that
  *         contains the configuration information on faulty sector in case of error 
  *         (0xFFFFFFFFU means that all the sectors have been correctly erased)
  * 
  * @retval HAL Status
  */
HAL_StatusTypeDef HAL_FLASHEx_Erase(FLASH_EraseInitTypeDef *pEraseInit, uint32_t *SectorError)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_ERROR;                 /* HAL状态类型函数 */
  uint32_t index = 0U;                                  /* 32位无符号变量/函数 */

  /* Process Locked */
  __HAL_LOCK(&pFlash);                                  /* HAL辅助宏/函数 */

  /* Check the parameters */
  assert_param(IS_FLASH_TYPEERASE(pEraseInit->TypeErase));  /* 执行语句 */

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);  /* 赋值或初始化 */

  if(status == HAL_OK)                                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /*Initialization of SectorError variable*/
    *SectorError = 0xFFFFFFFFU;                         /* 赋值或初始化 */

    if(pEraseInit->TypeErase == FLASH_TYPEERASE_MASSERASE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /*Mass erase to be done*/
      FLASH_MassErase((uint8_t) pEraseInit->VoltageRange, pEraseInit->Banks);  /* 执行语句 */

      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);  /* 赋值或初始化 */

      /* if the erase operation is completed, disable the MER Bit */
      FLASH->CR &= (~FLASH_MER_BIT);                    /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Check the parameters */
      assert_param(IS_FLASH_NBSECTORS(pEraseInit->NbSectors + pEraseInit->Sector));  /* 执行语句 */

      /* Erase by sector by sector to be done*/
      for(index = pEraseInit->Sector; index < (pEraseInit->NbSectors + pEraseInit->Sector); index++)  /* for循环 */
      {                                                 /* 代码块开始 */
        FLASH_Erase_Sector(index, (uint8_t) pEraseInit->VoltageRange);  /* 执行语句 */

        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);  /* 赋值或初始化 */

        /* If the erase operation is completed, disable the SER and SNB Bits */
        CLEAR_BIT(FLASH->CR, (FLASH_CR_SER | FLASH_CR_SNB));  /* 执行语句 */

        if(status != HAL_OK)                            /* 条件判断 */
        {                                               /* 代码块开始 */
          /* In case of error, stop erase procedure and return the faulty sector*/
          *SectorError = index;                         /* 赋值或初始化 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    /* Flush the caches to be sure of the data consistency */
    FLASH_FlushCaches();                                /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Process Unlocked */
  __HAL_UNLOCK(&pFlash);                                /* HAL辅助宏/函数 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Perform a mass erase or erase the specified FLASH memory sectors  with interrupt enabled
  * @param  pEraseInit pointer to an FLASH_EraseInitTypeDef structure that
  *         contains the configuration information for the erasing.
  * 
  * @retval HAL Status
  */
HAL_StatusTypeDef HAL_FLASHEx_Erase_IT(FLASH_EraseInitTypeDef *pEraseInit)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Process Locked */
  __HAL_LOCK(&pFlash);                                  /* HAL辅助宏/函数 */

  /* Check the parameters */
  assert_param(IS_FLASH_TYPEERASE(pEraseInit->TypeErase));  /* 执行语句 */

  /* Enable End of FLASH Operation interrupt */
  __HAL_FLASH_ENABLE_IT(FLASH_IT_EOP);                  /* HAL辅助宏/函数 */

  /* Enable Error source interrupt */
  __HAL_FLASH_ENABLE_IT(FLASH_IT_ERR);                  /* HAL辅助宏/函数 */

  /* Clear pending flags (if any) */  
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP    | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |\
                         FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR| FLASH_FLAG_PGSERR);    /* 执行语句 */

  if(pEraseInit->TypeErase == FLASH_TYPEERASE_MASSERASE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /*Mass erase to be done*/
    pFlash.ProcedureOnGoing = FLASH_PROC_MASSERASE;     /* 赋值或初始化 */
    pFlash.Bank = pEraseInit->Banks;                    /* 赋值或初始化 */
    FLASH_MassErase((uint8_t) pEraseInit->VoltageRange, pEraseInit->Banks);  /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Erase by sector to be done*/

    /* Check the parameters */
    assert_param(IS_FLASH_NBSECTORS(pEraseInit->NbSectors + pEraseInit->Sector));  /* 执行语句 */

    pFlash.ProcedureOnGoing = FLASH_PROC_SECTERASE;     /* 赋值或初始化 */
    pFlash.NbSectorsToErase = pEraseInit->NbSectors;    /* 赋值或初始化 */
    pFlash.Sector = pEraseInit->Sector;                 /* 赋值或初始化 */
    pFlash.VoltageForErase = (uint8_t)pEraseInit->VoltageRange;  /* 赋值或初始化 */

    /*Erase 1st sector and wait for IT*/
    FLASH_Erase_Sector(pEraseInit->Sector, pEraseInit->VoltageRange);  /* 执行语句 */
  }                                                     /* 代码块结束 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief   Program option bytes
  * @param  pOBInit pointer to an FLASH_OBInitStruct structure that
  *         contains the configuration information for the programming.
  * 
  * @retval HAL Status
  */
HAL_StatusTypeDef HAL_FLASHEx_OBProgram(FLASH_OBProgramInitTypeDef *pOBInit)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_ERROR;                 /* HAL状态类型函数 */

  /* Process Locked */
  __HAL_LOCK(&pFlash);                                  /* HAL辅助宏/函数 */

  /* Check the parameters */
  assert_param(IS_OPTIONBYTE(pOBInit->OptionType));     /* 执行语句 */

  /*Write protection configuration*/
  if((pOBInit->OptionType & OPTIONBYTE_WRP) == OPTIONBYTE_WRP)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    assert_param(IS_WRPSTATE(pOBInit->WRPState));       /* 执行语句 */
    if(pOBInit->WRPState == OB_WRPSTATE_ENABLE)         /* 条件判断 */
    {                                                   /* 代码块开始 */
      /*Enable of Write protection on the selected Sector*/
      status = FLASH_OB_EnableWRP(pOBInit->WRPSector, pOBInit->Banks);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /*Disable of Write protection on the selected Sector*/
      status = FLASH_OB_DisableWRP(pOBInit->WRPSector, pOBInit->Banks);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /*Read protection configuration*/
  if((pOBInit->OptionType & OPTIONBYTE_RDP) == OPTIONBYTE_RDP)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    status = FLASH_OB_RDP_LevelConfig(pOBInit->RDPLevel);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /*USER  configuration*/
  if((pOBInit->OptionType & OPTIONBYTE_USER) == OPTIONBYTE_USER)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    status = FLASH_OB_UserConfig(pOBInit->USERConfig&OB_IWDG_SW,   /* 列表续行 */
                                     pOBInit->USERConfig&OB_STOP_NO_RST,  /* 列表续行 */
                                     pOBInit->USERConfig&OB_STDBY_NO_RST);  /* 执行语句 */
  }                                                     /* 代码块结束 */

  /*BOR Level  configuration*/
  if((pOBInit->OptionType & OPTIONBYTE_BOR) == OPTIONBYTE_BOR)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    status = FLASH_OB_BOR_LevelConfig(pOBInit->BORLevel);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Process Unlocked */
  __HAL_UNLOCK(&pFlash);                                /* HAL辅助宏/函数 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief   Get the Option byte configuration
  * @param  pOBInit pointer to an FLASH_OBInitStruct structure that
  *         contains the configuration information for the programming.
  * 
  * @retval None
  */
void HAL_FLASHEx_OBGetConfig(FLASH_OBProgramInitTypeDef *pOBInit)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  pOBInit->OptionType = OPTIONBYTE_WRP | OPTIONBYTE_RDP | OPTIONBYTE_USER | OPTIONBYTE_BOR;  /* 赋值或初始化 */

  /*Get WRP*/
  pOBInit->WRPSector = (uint32_t)FLASH_OB_GetWRP();     /* 赋值或初始化 */

  /*Get RDP Level*/
  pOBInit->RDPLevel = (uint32_t)FLASH_OB_GetRDP();      /* 赋值或初始化 */

  /*Get USER*/
  pOBInit->USERConfig = (uint8_t)FLASH_OB_GetUser();    /* 赋值或初始化 */

  /*Get BOR Level*/
  pOBInit->BORLevel = (uint32_t)FLASH_OB_GetBOR();      /* 赋值或初始化 */
}                                                       /* 代码块结束 */

#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx) ||\
    defined(STM32F401xC) || defined(STM32F401xE) || defined(STM32F410Tx) || defined(STM32F410Cx) ||\
    defined(STM32F410Rx) || defined(STM32F411xE) || defined(STM32F446xx) || defined(STM32F469xx) ||\
    defined(STM32F479xx) || defined(STM32F412Zx) || defined(STM32F412Vx) || defined(STM32F412Rx) ||\
    defined(STM32F412Cx) || defined(STM32F413xx) || defined(STM32F423xx)  /* 源代码 */
/**
  * @brief   Program option bytes
  * @param  pAdvOBInit pointer to an FLASH_AdvOBProgramInitTypeDef structure that
  *         contains the configuration information for the programming.
  * 
  * @retval HAL Status
  */
HAL_StatusTypeDef HAL_FLASHEx_AdvOBProgram (FLASH_AdvOBProgramInitTypeDef *pAdvOBInit)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_ERROR;                 /* HAL状态类型函数 */

  /* Check the parameters */
  assert_param(IS_OBEX(pAdvOBInit->OptionType));        /* 执行语句 */

  /*Program PCROP option byte*/
  if(((pAdvOBInit->OptionType) & OPTIONBYTE_PCROP) == OPTIONBYTE_PCROP)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Check the parameters */
    assert_param(IS_PCROPSTATE(pAdvOBInit->PCROPState));  /* 执行语句 */
    if((pAdvOBInit->PCROPState) == OB_PCROP_STATE_ENABLE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /*Enable of Write protection on the selected Sector*/
#if defined(STM32F401xC) || defined(STM32F401xE) || defined(STM32F410Tx) || defined(STM32F410Cx) || defined(STM32F410Rx) ||\
    defined(STM32F411xE) || defined(STM32F446xx) || defined(STM32F412Zx) || defined(STM32F412Vx) || defined(STM32F412Rx) ||\
    defined(STM32F412Cx) || defined(STM32F413xx) || defined(STM32F423xx)  /* 源代码 */
      status = FLASH_OB_EnablePCROP(pAdvOBInit->Sectors);  /* 赋值或初始化 */
#else  /* STM32F427xx || STM32F437xx || STM32F429xx|| STM32F439xx || STM32F469xx || STM32F479xx */
      status = FLASH_OB_EnablePCROP(pAdvOBInit->SectorsBank1, pAdvOBInit->SectorsBank2, pAdvOBInit->Banks);  /* 赋值或初始化 */
#endif /* STM32F401xC || STM32F401xE || STM32F410xx || STM32F411xE || STM32F446xx || STM32F412Zx || STM32F412Vx || STM32F412Rx || STM32F412Cx ||
          STM32F413xx || STM32F423xx */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /*Disable of Write protection on the selected Sector*/
#if defined(STM32F401xC) || defined(STM32F401xE) || defined(STM32F410Tx) || defined(STM32F410Cx) || defined(STM32F410Rx) ||\
    defined(STM32F411xE) || defined(STM32F446xx) || defined(STM32F412Zx) || defined(STM32F412Vx) || defined(STM32F412Rx) ||\
    defined(STM32F412Cx) || defined(STM32F413xx) || defined(STM32F423xx)  /* 源代码 */
      status = FLASH_OB_DisablePCROP(pAdvOBInit->Sectors);  /* 赋值或初始化 */
#else /* STM32F427xx || STM32F437xx || STM32F429xx|| STM32F439xx || STM32F469xx || STM32F479xx */
      status = FLASH_OB_DisablePCROP(pAdvOBInit->SectorsBank1, pAdvOBInit->SectorsBank2, pAdvOBInit->Banks);  /* 赋值或初始化 */
#endif /* STM32F401xC || STM32F401xE || STM32F410xx || STM32F411xE || STM32F446xx || STM32F412Zx || STM32F412Vx || STM32F412Rx || STM32F412Cx ||
          STM32F413xx || STM32F423xx */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx) || defined(STM32F469xx) || defined(STM32F479xx)  /* 条件编译 */
  /*Program BOOT config option byte*/
  if(((pAdvOBInit->OptionType) & OPTIONBYTE_BOOTCONFIG) == OPTIONBYTE_BOOTCONFIG)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    status = FLASH_OB_BootConfig(pAdvOBInit->BootConfig);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx || STM32F469xx || STM32F479xx */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief   Get the OBEX byte configuration
  * @param  pAdvOBInit pointer to an FLASH_AdvOBProgramInitTypeDef structure that
  *         contains the configuration information for the programming.
  * 
  * @retval None
  */
void HAL_FLASHEx_AdvOBGetConfig(FLASH_AdvOBProgramInitTypeDef *pAdvOBInit)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
#if defined(STM32F401xC) || defined(STM32F401xE) || defined(STM32F410Tx) || defined(STM32F410Cx) || defined(STM32F410Rx) ||\
    defined(STM32F411xE) || defined(STM32F446xx) || defined(STM32F412Zx) || defined(STM32F412Vx) || defined(STM32F412Rx) ||\
    defined(STM32F412Cx) || defined(STM32F413xx) || defined(STM32F423xx)  /* 源代码 */
  /*Get Sector*/
  pAdvOBInit->Sectors = (*(__IO uint16_t *)(OPTCR_BYTE2_ADDRESS));  /* 赋值或初始化 */
#else  /* STM32F427xx || STM32F437xx || STM32F429xx|| STM32F439xx || STM32F469xx || STM32F479xx */
  /*Get Sector for Bank1*/
  pAdvOBInit->SectorsBank1 = (*(__IO uint16_t *)(OPTCR_BYTE2_ADDRESS));  /* 赋值或初始化 */

  /*Get Sector for Bank2*/
  pAdvOBInit->SectorsBank2 = (*(__IO uint16_t *)(OPTCR1_BYTE2_ADDRESS));  /* 赋值或初始化 */

  /*Get Boot config OB*/
  pAdvOBInit->BootConfig = *(__IO uint8_t *)OPTCR_BYTE0_ADDRESS;  /* 赋值或初始化 */
#endif /* STM32F401xC || STM32F401xE || STM32F410xx || STM32F411xE || STM32F446xx || STM32F412Zx || STM32F412Vx || STM32F412Rx || STM32F412Cx ||
          STM32F413xx || STM32F423xx */
}                                                       /* 代码块结束 */

/**
  * @brief  Select the Protection Mode 
  * 
  * @note   After PCROP activated Option Byte modification NOT POSSIBLE! excepted 
  *         Global Read Out Protection modification (from level1 to level0) 
  * @note   Once SPRMOD bit is active unprotection of a protected sector is not possible 
  * @note   Read a protected sector will set RDERR Flag and write a protected sector will set WRPERR Flag
  * @note   This function can be used only for STM32F42xxx/STM32F43xxx/STM32F401xx/STM32F411xx/STM32F446xx/
  *         STM32F469xx/STM32F479xx/STM32F412xx/STM32F413xx devices.
  * 
  * @retval HAL Status
  */
HAL_StatusTypeDef HAL_FLASHEx_OB_SelectPCROP(void)      /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint8_t optiontmp = 0xFF;                             /* 8位无符号变量/函数 */

  /* Mask SPRMOD bit */
  optiontmp =  (uint8_t)((*(__IO uint8_t *)OPTCR_BYTE3_ADDRESS) & (uint8_t)0x7F);   /* 赋值或初始化 */

  /* Update Option Byte */
  *(__IO uint8_t *)OPTCR_BYTE3_ADDRESS = (uint8_t)(OB_PCROP_SELECTED | optiontmp);   /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Deselect the Protection Mode 
  * 
  * @note   After PCROP activated Option Byte modification NOT POSSIBLE! excepted 
  *         Global Read Out Protection modification (from level1 to level0) 
  * @note   Once SPRMOD bit is active unprotection of a protected sector is not possible 
  * @note   Read a protected sector will set RDERR Flag and write a protected sector will set WRPERR Flag
  * @note   This function can be used only for STM32F42xxx/STM32F43xxx/STM32F401xx/STM32F411xx/STM32F446xx/
  *         STM32F469xx/STM32F479xx/STM32F412xx/STM32F413xx devices.
  * 
  * @retval HAL Status
  */
HAL_StatusTypeDef HAL_FLASHEx_OB_DeSelectPCROP(void)    /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint8_t optiontmp = 0xFF;                             /* 8位无符号变量/函数 */

  /* Mask SPRMOD bit */
  optiontmp =  (uint8_t)((*(__IO uint8_t *)OPTCR_BYTE3_ADDRESS) & (uint8_t)0x7F);   /* 赋值或初始化 */

  /* Update Option Byte */
  *(__IO uint8_t *)OPTCR_BYTE3_ADDRESS = (uint8_t)(OB_PCROP_DESELECTED | optiontmp);    /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx || STM32F401xC || STM32F401xE || STM32F410xx ||\
          STM32F411xE || STM32F469xx || STM32F479xx || STM32F412Zx || STM32F412Vx || STM32F412Rx || STM32F412Cx ||
          STM32F413xx || STM32F423xx */

#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx)|| defined(STM32F439xx) || defined(STM32F469xx) || defined(STM32F479xx)  /* 条件编译 */
/**
  * @brief  Returns the FLASH Write Protection Option Bytes value for Bank 2
  * @note   This function can be used only for STM32F42xxx/STM32F43xxx/STM32F469xx/STM32F479xx devices.  
  * @retval The FLASH Write Protection  Option Bytes value
  */
uint16_t HAL_FLASHEx_OB_GetBank2WRP(void)               /* 16位无符号变量/函数 */
{                                                       /* 代码块开始 */
  /* Return the FLASH write protection Register value */
  return (*(__IO uint16_t *)(OPTCR1_BYTE2_ADDRESS));    /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx || STM32F469xx || STM32F479xx */

/**
  * @}
  */

#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx) || defined(STM32F469xx) || defined(STM32F479xx)  /* 条件编译 */
/**
  * @brief  Full erase of FLASH memory sectors 
  * @param  VoltageRange The device voltage range which defines the erase parallelism.  
  *          This parameter can be one of the following values:
  *            @arg FLASH_VOLTAGE_RANGE_1: when the device voltage range is 1.8V to 2.1V, 
  *                                  the operation will be done by byte (8-bit) 
  *            @arg FLASH_VOLTAGE_RANGE_2: when the device voltage range is 2.1V to 2.7V,
  *                                  the operation will be done by half word (16-bit)
  *            @arg FLASH_VOLTAGE_RANGE_3: when the device voltage range is 2.7V to 3.6V,
  *                                  the operation will be done by word (32-bit)
  *            @arg FLASH_VOLTAGE_RANGE_4: when the device voltage range is 2.7V to 3.6V + External Vpp, 
  *                                  the operation will be done by double word (64-bit)
  * 
  * @param  Banks Banks to be erased
  *          This parameter can be one of the following values:
  *            @arg FLASH_BANK_1: Bank1 to be erased
  *            @arg FLASH_BANK_2: Bank2 to be erased
  *            @arg FLASH_BANK_BOTH: Bank1 and Bank2 to be erased
  *
  * @retval HAL Status
  */
static void FLASH_MassErase(uint8_t VoltageRange, uint32_t Banks)  /* 静态函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_VOLTAGERANGE(VoltageRange));          /* 执行语句 */
  assert_param(IS_FLASH_BANK(Banks));                   /* 执行语句 */

  /* if the previous operation is completed, proceed to erase all sectors */
  CLEAR_BIT(FLASH->CR, FLASH_CR_PSIZE);                 /* 执行语句 */

  if(Banks == FLASH_BANK_BOTH)                          /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* bank1 & bank2 will be erased*/
    FLASH->CR |= FLASH_MER_BIT;                         /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if(Banks == FLASH_BANK_1)                        /* 否则如果 */
  {                                                     /* 代码块开始 */
    /*Only bank1 will be erased*/
    FLASH->CR |= FLASH_CR_MER1;                         /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /*Only bank2 will be erased*/
    FLASH->CR |= FLASH_CR_MER2;                         /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  FLASH->CR |= FLASH_CR_STRT | ((uint32_t)VoltageRange <<8U);  /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  Erase the specified FLASH memory sector
  * @param  Sector FLASH sector to erase
  *         The value of this parameter depend on device used within the same series      
  * @param  VoltageRange The device voltage range which defines the erase parallelism.  
  *          This parameter can be one of the following values:
  *            @arg FLASH_VOLTAGE_RANGE_1: when the device voltage range is 1.8V to 2.1V, 
  *                                  the operation will be done by byte (8-bit) 
  *            @arg FLASH_VOLTAGE_RANGE_2: when the device voltage range is 2.1V to 2.7V,
  *                                  the operation will be done by half word (16-bit)
  *            @arg FLASH_VOLTAGE_RANGE_3: when the device voltage range is 2.7V to 3.6V,
  *                                  the operation will be done by word (32-bit)
  *            @arg FLASH_VOLTAGE_RANGE_4: when the device voltage range is 2.7V to 3.6V + External Vpp, 
  *                                  the operation will be done by double word (64-bit)
  * 
  * @retval None
  */
void FLASH_Erase_Sector(uint32_t Sector, uint8_t VoltageRange)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  uint32_t tmp_psize = 0U;                              /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_FLASH_SECTOR(Sector));                /* 执行语句 */
  assert_param(IS_VOLTAGERANGE(VoltageRange));          /* 执行语句 */

  if(VoltageRange == FLASH_VOLTAGE_RANGE_1)             /* 条件判断 */
  {                                                     /* 代码块开始 */
     tmp_psize = FLASH_PSIZE_BYTE;                      /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if(VoltageRange == FLASH_VOLTAGE_RANGE_2)        /* 否则如果 */
  {                                                     /* 代码块开始 */
    tmp_psize = FLASH_PSIZE_HALF_WORD;                  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if(VoltageRange == FLASH_VOLTAGE_RANGE_3)        /* 否则如果 */
  {                                                     /* 代码块开始 */
    tmp_psize = FLASH_PSIZE_WORD;                       /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    tmp_psize = FLASH_PSIZE_DOUBLE_WORD;                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Need to add offset of 4 when sector higher than FLASH_SECTOR_11 */
  if(Sector > FLASH_SECTOR_11)                          /* 条件判断 */
  {                                                     /* 代码块开始 */
    Sector += 4U;                                       /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  /* If the previous operation is completed, proceed to erase the sector */
  CLEAR_BIT(FLASH->CR, FLASH_CR_PSIZE);                 /* 执行语句 */
  FLASH->CR |= tmp_psize;                               /* 赋值或初始化 */
  CLEAR_BIT(FLASH->CR, FLASH_CR_SNB);                   /* 执行语句 */
  FLASH->CR |= FLASH_CR_SER | (Sector << FLASH_CR_SNB_Pos);  /* 赋值或初始化 */
  FLASH->CR |= FLASH_CR_STRT;                           /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  Enable the write protection of the desired bank1 or bank 2 sectors
  *
  * @note   When the memory read protection level is selected (RDP level = 1), 
  *         it is not possible to program or erase the flash sector i if CortexM4  
  *         debug features are connected or boot code is executed in RAM, even if nWRPi = 1 
  * @note   Active value of nWRPi bits is inverted when PCROP mode is active (SPRMOD =1).   
  * 
  * @param  WRPSector specifies the sector(s) to be write protected.
  *          This parameter can be one of the following values:
  *            @arg WRPSector: A value between OB_WRP_SECTOR_0 and OB_WRP_SECTOR_23
  *            @arg OB_WRP_SECTOR_All
  * @note   BANK2 starts from OB_WRP_SECTOR_12
  *
  * @param  Banks Enable write protection on all the sectors for the specific bank
  *          This parameter can be one of the following values:
  *            @arg FLASH_BANK_1: WRP on all sectors of bank1
  *            @arg FLASH_BANK_2: WRP on all sectors of bank2
  *            @arg FLASH_BANK_BOTH: WRP on all sectors of bank1 & bank2
  *
  * @retval HAL FLASH State   
  */
static HAL_StatusTypeDef FLASH_OB_EnableWRP(uint32_t WRPSector, uint32_t Banks)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Check the parameters */
  assert_param(IS_OB_WRP_SECTOR(WRPSector));            /* 执行语句 */
  assert_param(IS_FLASH_BANK(Banks));                   /* 执行语句 */

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);  /* 赋值或初始化 */

  if(status == HAL_OK)                                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if(((WRPSector == OB_WRP_SECTOR_All) && ((Banks == FLASH_BANK_1) || (Banks == FLASH_BANK_BOTH))) ||  /* 条件判断 */
         (WRPSector < OB_WRP_SECTOR_12))                /* 源代码 */
    {                                                   /* 代码块开始 */
       if(WRPSector == OB_WRP_SECTOR_All)               /* 条件判断 */
       {                                                /* 代码块开始 */
          /*Write protection on all sector of BANK1*/
          *(__IO uint16_t*)OPTCR_BYTE2_ADDRESS &= (~(WRPSector>>12));    /* 赋值或初始化 */
       }                                                /* 代码块结束 */
       else                                             /* 否则 */
       {                                                /* 代码块开始 */
          /*Write protection done on sectors of BANK1*/
          *(__IO uint16_t*)OPTCR_BYTE2_ADDRESS &= (~WRPSector);    /* 赋值或初始化 */
       }                                                /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /*Write protection done on sectors of BANK2*/
      *(__IO uint16_t*)OPTCR1_BYTE2_ADDRESS &= (~(WRPSector>>12));    /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /*Write protection on all sector of BANK2*/
    if((WRPSector == OB_WRP_SECTOR_All) && (Banks == FLASH_BANK_BOTH))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);  /* 赋值或初始化 */

      if(status == HAL_OK)                              /* 条件判断 */
      {                                                 /* 代码块开始 */
        *(__IO uint16_t*)OPTCR1_BYTE2_ADDRESS &= (~(WRPSector>>12));    /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

  }                                                     /* 代码块结束 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Disable the write protection of the desired bank1 or bank 2 sectors
  *
  * @note   When the memory read protection level is selected (RDP level = 1), 
  *         it is not possible to program or erase the flash sector i if CortexM4  
  *         debug features are connected or boot code is executed in RAM, even if nWRPi = 1 
  * @note   Active value of nWRPi bits is inverted when PCROP mode is active (SPRMOD =1).   
  * 
  * @param  WRPSector specifies the sector(s) to be write protected.
  *          This parameter can be one of the following values:
  *            @arg WRPSector: A value between OB_WRP_SECTOR_0 and OB_WRP_SECTOR_23
  *            @arg OB_WRP_Sector_All
  * @note   BANK2 starts from OB_WRP_SECTOR_12
  *
  * @param  Banks Disable write protection on all the sectors for the specific bank
  *          This parameter can be one of the following values:
  *            @arg FLASH_BANK_1: Bank1 to be erased
  *            @arg FLASH_BANK_2: Bank2 to be erased
  *            @arg FLASH_BANK_BOTH: Bank1 and Bank2 to be erased
  *
  * @retval HAL Status   
  */
static HAL_StatusTypeDef FLASH_OB_DisableWRP(uint32_t WRPSector, uint32_t Banks)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Check the parameters */
  assert_param(IS_OB_WRP_SECTOR(WRPSector));            /* 执行语句 */
  assert_param(IS_FLASH_BANK(Banks));                   /* 执行语句 */

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);  /* 赋值或初始化 */

  if(status == HAL_OK)                                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if(((WRPSector == OB_WRP_SECTOR_All) && ((Banks == FLASH_BANK_1) || (Banks == FLASH_BANK_BOTH))) ||  /* 条件判断 */
         (WRPSector < OB_WRP_SECTOR_12))                /* 源代码 */
    {                                                   /* 代码块开始 */
       if(WRPSector == OB_WRP_SECTOR_All)               /* 条件判断 */
       {                                                /* 代码块开始 */
          /*Write protection on all sector of BANK1*/
          *(__IO uint16_t*)OPTCR_BYTE2_ADDRESS |= (uint16_t)(WRPSector>>12);   /* 赋值或初始化 */
       }                                                /* 代码块结束 */
       else                                             /* 否则 */
       {                                                /* 代码块开始 */
          /*Write protection done on sectors of BANK1*/
          *(__IO uint16_t*)OPTCR_BYTE2_ADDRESS |= (uint16_t)WRPSector;   /* 赋值或初始化 */
       }                                                /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /*Write protection done on sectors of BANK2*/
      *(__IO uint16_t*)OPTCR1_BYTE2_ADDRESS |= (uint16_t)(WRPSector>>12);   /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /*Write protection on all sector  of BANK2*/
    if((WRPSector == OB_WRP_SECTOR_All) && (Banks == FLASH_BANK_BOTH))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);  /* 赋值或初始化 */

      if(status == HAL_OK)                              /* 条件判断 */
      {                                                 /* 代码块开始 */
        *(__IO uint16_t*)OPTCR1_BYTE2_ADDRESS |= (uint16_t)(WRPSector>>12);   /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

  }                                                     /* 代码块结束 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configure the Dual Bank Boot.
  *   
  * @note   This function can be used only for STM32F42xxx/43xxx devices.
  *      
  * @param  BootConfig specifies the Dual Bank Boot Option byte.
  *          This parameter can be one of the following values:
  *            @arg OB_Dual_BootEnabled: Dual Bank Boot Enable
  *            @arg OB_Dual_BootDisabled: Dual Bank Boot Disabled
  * @retval None
  */
static HAL_StatusTypeDef FLASH_OB_BootConfig(uint8_t BootConfig)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Check the parameters */
  assert_param(IS_OB_BOOT(BootConfig));                 /* 执行语句 */

  /* Wait for last operation to be completed */  
  status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);  /* 赋值或初始化 */

  if(status == HAL_OK)                                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Set Dual Bank Boot */
    *(__IO uint8_t *)OPTCR_BYTE0_ADDRESS &= (~FLASH_OPTCR_BFB2);  /* 赋值或初始化 */
    *(__IO uint8_t *)OPTCR_BYTE0_ADDRESS |= BootConfig;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Enable the read/write protection (PCROP) of the desired 
  *         sectors of Bank 1 and/or Bank 2.
  * @note   This function can be used only for STM32F42xxx/43xxx devices.
  * @param  SectorBank1 Specifies the sector(s) to be read/write protected or unprotected for bank1.
  *          This parameter can be one of the following values:
  *            @arg OB_PCROP: A value between OB_PCROP_SECTOR_0 and OB_PCROP_SECTOR_11
  *            @arg OB_PCROP_SECTOR__All                         
  * @param  SectorBank2 Specifies the sector(s) to be read/write protected or unprotected for bank2.
  *          This parameter can be one of the following values:
  *            @arg OB_PCROP: A value between OB_PCROP_SECTOR_12 and OB_PCROP_SECTOR_23
  *            @arg OB_PCROP_SECTOR__All                         
  * @param  Banks Enable PCROP protection on all the sectors for the specific bank
  *          This parameter can be one of the following values:
  *            @arg FLASH_BANK_1: WRP on all sectors of bank1
  *            @arg FLASH_BANK_2: WRP on all sectors of bank2
  *            @arg FLASH_BANK_BOTH: WRP on all sectors of bank1 & bank2
  *
  * @retval HAL Status  
  */
static HAL_StatusTypeDef FLASH_OB_EnablePCROP(uint32_t SectorBank1, uint32_t SectorBank2, uint32_t Banks)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  assert_param(IS_FLASH_BANK(Banks));                   /* 执行语句 */

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);  /* 赋值或初始化 */

  if(status == HAL_OK)                                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if((Banks == FLASH_BANK_1) || (Banks == FLASH_BANK_BOTH))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      assert_param(IS_OB_PCROP(SectorBank1));           /* 执行语句 */
      /*Write protection done on sectors of BANK1*/
      *(__IO uint16_t*)OPTCR_BYTE2_ADDRESS |= (uint16_t)SectorBank1;   /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      assert_param(IS_OB_PCROP(SectorBank2));           /* 执行语句 */
      /*Write protection done on sectors of BANK2*/
      *(__IO uint16_t*)OPTCR1_BYTE2_ADDRESS |= (uint16_t)SectorBank2;   /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /*Write protection on all sector  of BANK2*/
    if(Banks == FLASH_BANK_BOTH)                        /* 条件判断 */
    {                                                   /* 代码块开始 */
      assert_param(IS_OB_PCROP(SectorBank2));           /* 执行语句 */
      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);  /* 赋值或初始化 */

      if(status == HAL_OK)                              /* 条件判断 */
      {                                                 /* 代码块开始 */
        /*Write protection done on sectors of BANK2*/
        *(__IO uint16_t*)OPTCR1_BYTE2_ADDRESS |= (uint16_t)SectorBank2;   /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

  }                                                     /* 代码块结束 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */


/**
  * @brief  Disable the read/write protection (PCROP) of the desired 
  *         sectors  of Bank 1 and/or Bank 2.
  * @note   This function can be used only for STM32F42xxx/43xxx devices.
  * @param  SectorBank1 specifies the sector(s) to be read/write protected or unprotected for bank1.
  *          This parameter can be one of the following values:
  *            @arg OB_PCROP: A value between OB_PCROP_SECTOR_0 and OB_PCROP_SECTOR_11
  *            @arg OB_PCROP_SECTOR__All                         
  * @param  SectorBank2 Specifies the sector(s) to be read/write protected or unprotected for bank2.
  *          This parameter can be one of the following values:
  *            @arg OB_PCROP: A value between OB_PCROP_SECTOR_12 and OB_PCROP_SECTOR_23
  *            @arg OB_PCROP_SECTOR__All                         
  * @param  Banks Disable PCROP protection on all the sectors for the specific bank
  *          This parameter can be one of the following values:
  *            @arg FLASH_BANK_1: WRP on all sectors of bank1
  *            @arg FLASH_BANK_2: WRP on all sectors of bank2
  *            @arg FLASH_BANK_BOTH: WRP on all sectors of bank1 & bank2
  *
  * @retval HAL Status  
  */
static HAL_StatusTypeDef FLASH_OB_DisablePCROP(uint32_t SectorBank1, uint32_t SectorBank2, uint32_t Banks)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Check the parameters */
  assert_param(IS_FLASH_BANK(Banks));                   /* 执行语句 */

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);  /* 赋值或初始化 */

  if(status == HAL_OK)                                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if((Banks == FLASH_BANK_1) || (Banks == FLASH_BANK_BOTH))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      assert_param(IS_OB_PCROP(SectorBank1));           /* 执行语句 */
      /*Write protection done on sectors of BANK1*/
      *(__IO uint16_t*)OPTCR_BYTE2_ADDRESS &= (~SectorBank1);   /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /*Write protection done on sectors of BANK2*/
      assert_param(IS_OB_PCROP(SectorBank2));           /* 执行语句 */
      *(__IO uint16_t*)OPTCR1_BYTE2_ADDRESS &= (~SectorBank2);   /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /*Write protection on all sector  of BANK2*/
    if(Banks == FLASH_BANK_BOTH)                        /* 条件判断 */
    {                                                   /* 代码块开始 */
      assert_param(IS_OB_PCROP(SectorBank2));           /* 执行语句 */
     /* Wait for last operation to be completed */
      status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);  /* 赋值或初始化 */

      if(status == HAL_OK)                              /* 条件判断 */
      {                                                 /* 代码块开始 */
        /*Write protection done on sectors of BANK2*/
        *(__IO uint16_t*)OPTCR1_BYTE2_ADDRESS &= (~SectorBank2);   /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

  }                                                     /* 代码块结束 */

  return status;                                        /* 返回值 */

}                                                       /* 代码块结束 */

#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx || STM32F469xx || STM32F479xx */

#if defined(STM32F405xx) || defined(STM32F415xx) || defined(STM32F407xx) || defined(STM32F417xx) ||\
    defined(STM32F401xC) || defined(STM32F401xE) || defined(STM32F410Tx) || defined(STM32F410Cx) ||\
    defined(STM32F410Rx) || defined(STM32F411xE) || defined(STM32F446xx) || defined(STM32F412Zx) ||\
    defined(STM32F412Vx) || defined(STM32F412Rx) || defined(STM32F412Cx) || defined(STM32F413xx) ||\
    defined(STM32F423xx)                                /* 源代码 */
/**
  * @brief  Mass erase of FLASH memory
  * @param  VoltageRange The device voltage range which defines the erase parallelism.  
  *          This parameter can be one of the following values:
  *            @arg FLASH_VOLTAGE_RANGE_1: when the device voltage range is 1.8V to 2.1V, 
  *                                  the operation will be done by byte (8-bit) 
  *            @arg FLASH_VOLTAGE_RANGE_2: when the device voltage range is 2.1V to 2.7V,
  *                                  the operation will be done by half word (16-bit)
  *            @arg FLASH_VOLTAGE_RANGE_3: when the device voltage range is 2.7V to 3.6V,
  *                                  the operation will be done by word (32-bit)
  *            @arg FLASH_VOLTAGE_RANGE_4: when the device voltage range is 2.7V to 3.6V + External Vpp, 
  *                                  the operation will be done by double word (64-bit)
  * 
  * @param  Banks Banks to be erased
  *          This parameter can be one of the following values:
  *            @arg FLASH_BANK_1: Bank1 to be erased
  *
  * @retval None
  */
static void FLASH_MassErase(uint8_t VoltageRange, uint32_t Banks)  /* 静态函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_VOLTAGERANGE(VoltageRange));          /* 执行语句 */
  assert_param(IS_FLASH_BANK(Banks));                   /* 执行语句 */

  /* If the previous operation is completed, proceed to erase all sectors */
  CLEAR_BIT(FLASH->CR, FLASH_CR_PSIZE);                 /* 执行语句 */
  FLASH->CR |= FLASH_CR_MER;                            /* 赋值或初始化 */
  FLASH->CR |= FLASH_CR_STRT | ((uint32_t)VoltageRange <<8U);  /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  Erase the specified FLASH memory sector
  * @param  Sector FLASH sector to erase
  *         The value of this parameter depend on device used within the same series      
  * @param  VoltageRange The device voltage range which defines the erase parallelism.  
  *          This parameter can be one of the following values:
  *            @arg FLASH_VOLTAGE_RANGE_1: when the device voltage range is 1.8V to 2.1V, 
  *                                  the operation will be done by byte (8-bit) 
  *            @arg FLASH_VOLTAGE_RANGE_2: when the device voltage range is 2.1V to 2.7V,
  *                                  the operation will be done by half word (16-bit)
  *            @arg FLASH_VOLTAGE_RANGE_3: when the device voltage range is 2.7V to 3.6V,
  *                                  the operation will be done by word (32-bit)
  *            @arg FLASH_VOLTAGE_RANGE_4: when the device voltage range is 2.7V to 3.6V + External Vpp, 
  *                                  the operation will be done by double word (64-bit)
  * 
  * @retval None
  */
void FLASH_Erase_Sector(uint32_t Sector, uint8_t VoltageRange)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  uint32_t tmp_psize = 0U;                              /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_FLASH_SECTOR(Sector));                /* 执行语句 */
  assert_param(IS_VOLTAGERANGE(VoltageRange));          /* 执行语句 */

  if(VoltageRange == FLASH_VOLTAGE_RANGE_1)             /* 条件判断 */
  {                                                     /* 代码块开始 */
     tmp_psize = FLASH_PSIZE_BYTE;                      /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if(VoltageRange == FLASH_VOLTAGE_RANGE_2)        /* 否则如果 */
  {                                                     /* 代码块开始 */
    tmp_psize = FLASH_PSIZE_HALF_WORD;                  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if(VoltageRange == FLASH_VOLTAGE_RANGE_3)        /* 否则如果 */
  {                                                     /* 代码块开始 */
    tmp_psize = FLASH_PSIZE_WORD;                       /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    tmp_psize = FLASH_PSIZE_DOUBLE_WORD;                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* If the previous operation is completed, proceed to erase the sector */
  CLEAR_BIT(FLASH->CR, FLASH_CR_PSIZE);                 /* 执行语句 */
  FLASH->CR |= tmp_psize;                               /* 赋值或初始化 */
  CLEAR_BIT(FLASH->CR, FLASH_CR_SNB);                   /* 执行语句 */
  FLASH->CR |= FLASH_CR_SER | (Sector << FLASH_CR_SNB_Pos);  /* 赋值或初始化 */
  FLASH->CR |= FLASH_CR_STRT;                           /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  Enable the write protection of the desired bank 1 sectors
  *
  * @note   When the memory read protection level is selected (RDP level = 1), 
  *         it is not possible to program or erase the flash sector i if CortexM4  
  *         debug features are connected or boot code is executed in RAM, even if nWRPi = 1 
  * @note   Active value of nWRPi bits is inverted when PCROP mode is active (SPRMOD =1).   
  * 
  * @param  WRPSector specifies the sector(s) to be write protected.
  *         The value of this parameter depend on device used within the same series 
  * 
  * @param  Banks Enable write protection on all the sectors for the specific bank
  *          This parameter can be one of the following values:
  *            @arg FLASH_BANK_1: WRP on all sectors of bank1
  *
  * @retval HAL Status 
  */
static HAL_StatusTypeDef FLASH_OB_EnableWRP(uint32_t WRPSector, uint32_t Banks)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Check the parameters */
  assert_param(IS_OB_WRP_SECTOR(WRPSector));            /* 执行语句 */
  assert_param(IS_FLASH_BANK(Banks));                   /* 执行语句 */

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);  /* 赋值或初始化 */

  if(status == HAL_OK)                                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    *(__IO uint16_t*)OPTCR_BYTE2_ADDRESS &= (~WRPSector);    /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Disable the write protection of the desired bank 1 sectors
  *
  * @note   When the memory read protection level is selected (RDP level = 1), 
  *         it is not possible to program or erase the flash sector i if CortexM4  
  *         debug features are connected or boot code is executed in RAM, even if nWRPi = 1 
  * @note   Active value of nWRPi bits is inverted when PCROP mode is active (SPRMOD =1).   
  * 
  * @param  WRPSector specifies the sector(s) to be write protected.
  *         The value of this parameter depend on device used within the same series 
  * 
  * @param  Banks Enable write protection on all the sectors for the specific bank
  *          This parameter can be one of the following values:
  *            @arg FLASH_BANK_1: WRP on all sectors of bank1
  *
  * @retval HAL Status 
  */
static HAL_StatusTypeDef FLASH_OB_DisableWRP(uint32_t WRPSector, uint32_t Banks)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Check the parameters */
  assert_param(IS_OB_WRP_SECTOR(WRPSector));            /* 执行语句 */
  assert_param(IS_FLASH_BANK(Banks));                   /* 执行语句 */

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);  /* 赋值或初始化 */

  if(status == HAL_OK)                                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    *(__IO uint16_t*)OPTCR_BYTE2_ADDRESS |= (uint16_t)WRPSector;   /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* STM32F40xxx || STM32F41xxx || STM32F401xx || STM32F410xx || STM32F411xE || STM32F446xx || STM32F412Zx || STM32F412Vx || STM32F412Rx || STM32F412Cx
          STM32F413xx || STM32F423xx */

#if defined(STM32F401xC) || defined(STM32F401xE) || defined(STM32F410Tx) || defined(STM32F410Cx) || defined(STM32F410Rx) ||\
    defined(STM32F411xE) || defined(STM32F446xx) || defined(STM32F412Zx) || defined(STM32F412Vx) || defined(STM32F412Rx) ||\
    defined(STM32F412Cx) || defined(STM32F413xx) || defined(STM32F423xx)  /* 源代码 */
/**
  * @brief  Enable the read/write protection (PCROP) of the desired sectors.
  * @note   This function can be used only for STM32F401xx devices.
  * @param  Sector specifies the sector(s) to be read/write protected or unprotected.
  *          This parameter can be one of the following values:
  *            @arg OB_PCROP: A value between OB_PCROP_Sector0 and OB_PCROP_Sector5
  *            @arg OB_PCROP_Sector_All                         
  * @retval HAL Status  
  */
static HAL_StatusTypeDef FLASH_OB_EnablePCROP(uint32_t Sector)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Check the parameters */
  assert_param(IS_OB_PCROP(Sector));                    /* 执行语句 */

  /* Wait for last operation to be completed */  
  status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);  /* 赋值或初始化 */

  if(status == HAL_OK)                                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    *(__IO uint16_t*)OPTCR_BYTE2_ADDRESS |= (uint16_t)Sector;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */


/**
  * @brief  Disable the read/write protection (PCROP) of the desired sectors.
  * @note   This function can be used only for STM32F401xx devices.
  * @param  Sector specifies the sector(s) to be read/write protected or unprotected.
  *          This parameter can be one of the following values:
  *            @arg OB_PCROP: A value between OB_PCROP_Sector0 and OB_PCROP_Sector5
  *            @arg OB_PCROP_Sector_All                         
  * @retval HAL Status  
  */
static HAL_StatusTypeDef FLASH_OB_DisablePCROP(uint32_t Sector)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Check the parameters */
  assert_param(IS_OB_PCROP(Sector));                    /* 执行语句 */

  /* Wait for last operation to be completed */  
  status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);  /* 赋值或初始化 */

  if(status == HAL_OK)                                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    *(__IO uint16_t*)OPTCR_BYTE2_ADDRESS &= (~Sector);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return status;                                        /* 返回值 */

}                                                       /* 代码块结束 */
#endif /* STM32F401xC || STM32F401xE || STM32F411xE || STM32F446xx || STM32F412Zx || STM32F412Vx || STM32F412Rx || STM32F412Cx
          STM32F413xx || STM32F423xx */

/**
  * @brief  Set the read protection level.
  * @param  Level specifies the read protection level.
  *          This parameter can be one of the following values:
  *            @arg OB_RDP_LEVEL_0: No protection
  *            @arg OB_RDP_LEVEL_1: Read protection of the memory
  *            @arg OB_RDP_LEVEL_2: Full chip protection
  *   
  * @note WARNING: When enabling OB_RDP level 2 it's no more possible to go back to level 1 or 0
  *    
  * @retval HAL Status
  */
static HAL_StatusTypeDef FLASH_OB_RDP_LevelConfig(uint8_t Level)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Check the parameters */
  assert_param(IS_OB_RDP_LEVEL(Level));                 /* 执行语句 */

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);  /* 赋值或初始化 */

  if(status == HAL_OK)                                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    *(__IO uint8_t*)OPTCR_BYTE1_ADDRESS = Level;        /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Program the FLASH User Option Byte: IWDG_SW / RST_STOP / RST_STDBY.    
  * @param  Iwdg Selects the IWDG mode
  *          This parameter can be one of the following values:
  *            @arg OB_IWDG_SW: Software IWDG selected
  *            @arg OB_IWDG_HW: Hardware IWDG selected
  * @param  Stop Reset event when entering STOP mode.
  *          This parameter  can be one of the following values:
  *            @arg OB_STOP_NO_RST: No reset generated when entering in STOP
  *            @arg OB_STOP_RST: Reset generated when entering in STOP
  * @param  Stdby Reset event when entering Standby mode.
  *          This parameter  can be one of the following values:
  *            @arg OB_STDBY_NO_RST: No reset generated when entering in STANDBY
  *            @arg OB_STDBY_RST: Reset generated when entering in STANDBY
  * @retval HAL Status
  */
static HAL_StatusTypeDef FLASH_OB_UserConfig(uint8_t Iwdg, uint8_t Stop, uint8_t Stdby)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  uint8_t optiontmp = 0xFF;                             /* 8位无符号变量/函数 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Check the parameters */
  assert_param(IS_OB_IWDG_SOURCE(Iwdg));                /* 执行语句 */
  assert_param(IS_OB_STOP_SOURCE(Stop));                /* 执行语句 */
  assert_param(IS_OB_STDBY_SOURCE(Stdby));              /* 执行语句 */

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);  /* 赋值或初始化 */

  if(status == HAL_OK)                                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Mask OPTLOCK, OPTSTRT, BOR_LEV and BFB2 bits */
    optiontmp =  (uint8_t)((*(__IO uint8_t *)OPTCR_BYTE0_ADDRESS) & (uint8_t)0x1F);  /* 赋值或初始化 */

    /* Update User Option Byte */
    *(__IO uint8_t *)OPTCR_BYTE0_ADDRESS = Iwdg | (uint8_t)(Stdby | (uint8_t)(Stop | ((uint8_t)optiontmp)));   /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Set the BOR Level. 
  * @param  Level specifies the Option Bytes BOR Reset Level.
  *          This parameter can be one of the following values:
  *            @arg OB_BOR_LEVEL3: Supply voltage ranges from 2.7 to 3.6 V
  *            @arg OB_BOR_LEVEL2: Supply voltage ranges from 2.4 to 2.7 V
  *            @arg OB_BOR_LEVEL1: Supply voltage ranges from 2.1 to 2.4 V
  *            @arg OB_BOR_OFF: Supply voltage ranges from 1.62 to 2.1 V
  * @retval HAL Status
  */
static HAL_StatusTypeDef FLASH_OB_BOR_LevelConfig(uint8_t Level)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_OB_BOR_LEVEL(Level));                 /* 执行语句 */

  /* Set the BOR Level */
  *(__IO uint8_t *)OPTCR_BYTE0_ADDRESS &= (~FLASH_OPTCR_BOR_LEV);  /* 赋值或初始化 */
  *(__IO uint8_t *)OPTCR_BYTE0_ADDRESS |= Level;        /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */

}                                                       /* 代码块结束 */

/**
  * @brief  Return the FLASH User Option Byte value.
  * @retval uint8_t FLASH User Option Bytes values: IWDG_SW(Bit0), RST_STOP(Bit1)
  *         and RST_STDBY(Bit2).
  */
static uint8_t FLASH_OB_GetUser(void)                   /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  /* Return the User Option Byte */
  return ((uint8_t)(FLASH->OPTCR & 0xE0));              /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return the FLASH Write Protection Option Bytes value.
  * @retval uint16_t FLASH Write Protection Option Bytes value
  */
static uint16_t FLASH_OB_GetWRP(void)                   /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  /* Return the FLASH write protection Register value */
  return (*(__IO uint16_t *)(OPTCR_BYTE2_ADDRESS));     /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Returns the FLASH Read Protection level.
  * @retval FLASH ReadOut Protection Status:
  *         This parameter can be one of the following values:
  *            @arg OB_RDP_LEVEL_0: No protection
  *            @arg OB_RDP_LEVEL_1: Read protection of the memory
  *            @arg OB_RDP_LEVEL_2: Full chip protection
  */
static uint8_t FLASH_OB_GetRDP(void)                    /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  uint8_t readstatus = OB_RDP_LEVEL_0;                  /* 8位无符号变量/函数 */

  if((*(__IO uint8_t*)(OPTCR_BYTE1_ADDRESS) == (uint8_t)OB_RDP_LEVEL_2))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    readstatus = OB_RDP_LEVEL_2;                        /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if((*(__IO uint8_t*)(OPTCR_BYTE1_ADDRESS) == (uint8_t)OB_RDP_LEVEL_0))  /* 否则如果 */
  {                                                     /* 代码块开始 */
    readstatus = OB_RDP_LEVEL_0;                        /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    readstatus = OB_RDP_LEVEL_1;                        /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return readstatus;                                    /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Returns the FLASH BOR level.
  * @retval uint8_t The FLASH BOR level:
  *           - OB_BOR_LEVEL3: Supply voltage ranges from 2.7 to 3.6 V
  *           - OB_BOR_LEVEL2: Supply voltage ranges from 2.4 to 2.7 V
  *           - OB_BOR_LEVEL1: Supply voltage ranges from 2.1 to 2.4 V
  *           - OB_BOR_OFF   : Supply voltage ranges from 1.62 to 2.1 V  
  */
static uint8_t FLASH_OB_GetBOR(void)                    /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  /* Return the FLASH BOR level */
  return (uint8_t)(*(__IO uint8_t *)(OPTCR_BYTE0_ADDRESS) & (uint8_t)0x0C);  /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Flush the instruction and data caches
  * @retval None
  */
void FLASH_FlushCaches(void)                            /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  /* Flush instruction cache  */
  if(READ_BIT(FLASH->ACR, FLASH_ACR_ICEN)!= RESET)      /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable instruction cache  */
    __HAL_FLASH_INSTRUCTION_CACHE_DISABLE();            /* HAL辅助宏/函数 */
    /* Reset instruction cache */
    __HAL_FLASH_INSTRUCTION_CACHE_RESET();              /* HAL辅助宏/函数 */
    /* Enable instruction cache */
    __HAL_FLASH_INSTRUCTION_CACHE_ENABLE();             /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Flush data cache */
  if(READ_BIT(FLASH->ACR, FLASH_ACR_DCEN) != RESET)     /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable data cache  */
    __HAL_FLASH_DATA_CACHE_DISABLE();                   /* HAL辅助宏/函数 */
    /* Reset data cache */
    __HAL_FLASH_DATA_CACHE_RESET();                     /* HAL辅助宏/函数 */
    /* Enable data cache */
    __HAL_FLASH_DATA_CACHE_ENABLE();                    /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

#endif /* HAL_FLASH_MODULE_ENABLED */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
