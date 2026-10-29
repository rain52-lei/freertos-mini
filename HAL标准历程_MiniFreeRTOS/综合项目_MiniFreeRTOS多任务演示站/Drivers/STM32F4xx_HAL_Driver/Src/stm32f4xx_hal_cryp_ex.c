/* 文件: stm32f4xx_hal_cryp_ex.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_cryp_ex.c
  * @author  MCD Application Team
  * @brief   Extended CRYP HAL module driver
  *          This file provides firmware functions to manage the following
  *          functionalities of CRYP extension peripheral:
  *           + Extended AES processing functions
  *
  @verbatim
  ==============================================================================
                     ##### How to use this driver #####
  ==============================================================================
    [..]
    The CRYP extension HAL driver can be used as follows:
    (#)After AES-GCM or AES-CCM  Encryption/Decryption user can start following API
       to get the  authentication messages :
      (##) HAL_CRYPEx_AESGCM_GenerateAuthTAG
      (##) HAL_CRYPEx_AESCCM_GenerateAuthTAG

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
#if defined (AES)  || defined (CRYP)                    /* 条件编译 */
#if defined (CRYP_CR_ALGOMODE_AES_GCM)|| defined (AES)  /* 条件编译 */
/** @defgroup CRYPEx CRYPEx
  * @brief CRYP Extension HAL module driver.
  * @{
  */


#ifdef HAL_CRYP_MODULE_ENABLED                          /* 条件编译 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @addtogroup CRYPEx_Private_Defines
  * @{
  */
#if defined(AES)                                        /* 条件编译 */
#define CRYP_PHASE_INIT                              0x00000000U             /*!< GCM/GMAC (or CCM) init phase */
#define CRYP_PHASE_HEADER                            AES_CR_GCMPH_0          /*!< GCM/GMAC or CCM header phase */
#define CRYP_PHASE_PAYLOAD                           AES_CR_GCMPH_1          /*!< GCM(/CCM) payload phase   */
#define CRYP_PHASE_FINAL                             AES_CR_GCMPH            /*!< GCM/GMAC or CCM  final phase  */

#define CRYP_OPERATINGMODE_ENCRYPT                   0x00000000U             /*!< Encryption mode   */
#define CRYP_OPERATINGMODE_KEYDERIVATION             AES_CR_MODE_0           /*!< Key derivation mode  only used when performing ECB and CBC decryptions  */
#define CRYP_OPERATINGMODE_DECRYPT                   AES_CR_MODE_1           /*!< Decryption       */
#define CRYP_OPERATINGMODE_KEYDERIVATION_DECRYPT     AES_CR_MODE             /*!< Key derivation and decryption only used when performing ECB and CBC decryptions  */

#else /* CRYP */

#define CRYP_PHASE_INIT                 0x00000000U     /* 定义宏 */
#define CRYP_PHASE_HEADER               CRYP_CR_GCM_CCMPH_0  /* 定义宏 */
#define CRYP_PHASE_PAYLOAD              CRYP_CR_GCM_CCMPH_1  /* 定义宏 */
#define CRYP_PHASE_FINAL                CRYP_CR_GCM_CCMPH  /* 定义宏 */

#define CRYP_OPERATINGMODE_ENCRYPT      0x00000000U     /* 定义宏 */
#define CRYP_OPERATINGMODE_DECRYPT      CRYP_CR_ALGODIR  /* 定义宏 */
#endif /* End AES or CRYP */

#define  CRYPEx_PHASE_PROCESS       0x02U     /*!< CRYP peripheral is in processing phase */
#define  CRYPEx_PHASE_FINAL         0x03U     /*!< CRYP peripheral is in final phase this is relevant only with CCM and GCM modes */

/*  CTR0 information to use in CCM algorithm */
#define CRYP_CCM_CTR0_0            0x07FFFFFFU          /* 定义宏 */
#define CRYP_CCM_CTR0_3            0xFFFFFF00U          /* 定义宏 */


/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/



/* Exported functions---------------------------------------------------------*/
/** @addtogroup CRYPEx_Exported_Functions
  * @{
  */

/** @defgroup CRYPEx_Exported_Functions_Group1 Extended AES processing functions
  *  @brief   Extended processing functions.
  *
@verbatim
  ==============================================================================
              ##### Extended AES processing functions #####
  ==============================================================================
    [..]  This section provides functions allowing to generate the authentication
          TAG in Polling mode
      (#)HAL_CRYPEx_AESGCM_GenerateAuthTAG
      (#)HAL_CRYPEx_AESCCM_GenerateAuthTAG
         they should be used after Encrypt/Decrypt operation.

@endverbatim
  * @{
  */


/**
  * @brief  generate the GCM authentication TAG.
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module
  * @param  AuthTag: Pointer to the authentication buffer
  * @param  Timeout: Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_CRYPEx_AESGCM_GenerateAuthTAG(CRYP_HandleTypeDef *hcryp, uint32_t *AuthTag, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */
  /* Assume first Init.HeaderSize is in words */
  uint64_t headerlength = (uint64_t)(hcryp->Init.HeaderSize) * 32U; /* Header length in bits */
  uint64_t inputlength = (uint64_t)hcryp->SizesSum * 8U; /* Input length in bits */
  uint32_t tagaddr = (uint32_t)AuthTag;                 /* 32位无符号变量/函数 */

  /* Correct headerlength if Init.HeaderSize is actually in bytes */
  if (hcryp->Init.HeaderWidthUnit == CRYP_HEADERWIDTHUNIT_BYTE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    headerlength /= 4U;                                 /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if (hcryp->State == HAL_CRYP_STATE_READY)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Process locked */
    __HAL_LOCK(hcryp);                                  /* HAL辅助宏/函数 */

    /* Change the CRYP peripheral state */
    hcryp->State = HAL_CRYP_STATE_BUSY;                 /* 赋值或初始化 */

    /* Check if initialization phase has already been performed */
    if (hcryp->Phase == CRYPEx_PHASE_PROCESS)           /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Change the CRYP phase */
      hcryp->Phase = CRYPEx_PHASE_FINAL;                /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else /* Initialization phase has not been performed*/
    {                                                   /* 代码块开始 */
      /* Disable the Peripheral */
      __HAL_CRYP_DISABLE(hcryp);                        /* HAL辅助宏/函数 */

      /* Sequence error code field */
      hcryp->ErrorCode |= HAL_CRYP_ERROR_AUTH_TAG_SEQUENCE;  /* 赋值或初始化 */

      /* Change the CRYP peripheral state */
      hcryp->State = HAL_CRYP_STATE_READY;              /* 赋值或初始化 */

      /* Process unlocked */
      __HAL_UNLOCK(hcryp);                              /* HAL辅助宏/函数 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

#if defined(CRYP)                                       /* 条件编译 */

    /* Disable CRYP to start the final phase */
    __HAL_CRYP_DISABLE(hcryp);                          /* HAL辅助宏/函数 */

    /* Select final phase */
    MODIFY_REG(hcryp->Instance->CR, CRYP_CR_GCM_CCMPH, CRYP_PHASE_FINAL);  /* 执行语句 */

    /*ALGODIR bit must be set to 0.*/
    hcryp->Instance->CR &=  ~CRYP_CR_ALGODIR;           /* 赋值或初始化 */

    /* Enable the CRYP peripheral */
    __HAL_CRYP_ENABLE(hcryp);                           /* HAL辅助宏/函数 */

    /* Write the number of bits in header (64 bits) followed by the number of bits
    in the payload */
    if (hcryp->Init.DataType == CRYP_DATATYPE_1B)       /* 条件判断 */
    {                                                   /* 代码块开始 */
      hcryp->Instance->DIN = 0U;                        /* 赋值或初始化 */
      hcryp->Instance->DIN = __RBIT((uint32_t)(headerlength));  /* 赋值或初始化 */
      hcryp->Instance->DIN = 0U;                        /* 赋值或初始化 */
      hcryp->Instance->DIN = __RBIT((uint32_t)(inputlength));  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if (hcryp->Init.DataType == CRYP_DATATYPE_8B)  /* 否则如果 */
    {                                                   /* 代码块开始 */
      hcryp->Instance->DIN = 0U;                        /* 赋值或初始化 */
      hcryp->Instance->DIN = __REV((uint32_t)(headerlength));  /* 赋值或初始化 */
      hcryp->Instance->DIN = 0U;                        /* 赋值或初始化 */
      hcryp->Instance->DIN = __REV((uint32_t)(inputlength));  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if (hcryp->Init.DataType == CRYP_DATATYPE_16B)  /* 否则如果 */
    {                                                   /* 代码块开始 */
      hcryp->Instance->DIN = 0U;                        /* 赋值或初始化 */
      hcryp->Instance->DIN = __ROR((uint32_t)headerlength, 16U);  /* 赋值或初始化 */
      hcryp->Instance->DIN = 0U;                        /* 赋值或初始化 */
      hcryp->Instance->DIN = __ROR((uint32_t)inputlength, 16U);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if (hcryp->Init.DataType == CRYP_DATATYPE_32B)  /* 否则如果 */
    {                                                   /* 代码块开始 */
      hcryp->Instance->DIN = 0U;                        /* 赋值或初始化 */
      hcryp->Instance->DIN = (uint32_t)(headerlength);  /* 赋值或初始化 */
      hcryp->Instance->DIN = 0U;                        /* 赋值或初始化 */
      hcryp->Instance->DIN = (uint32_t)(inputlength);   /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Nothing to do */
    }                                                   /* 代码块结束 */

    /* Wait for OFNE flag to be raised */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */
    while (HAL_IS_BIT_CLR(hcryp->Instance->SR, CRYP_FLAG_OFNE))  /* while循环 */
    {                                                   /* 代码块开始 */
      /* Check for the Timeout */
      if (Timeout != HAL_MAX_DELAY)                     /* 条件判断 */
      {                                                 /* 代码块开始 */
        if (((HAL_GetTick() - tickstart) > Timeout) || (Timeout == 0U))  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Disable the CRYP Peripheral Clock */
          __HAL_CRYP_DISABLE(hcryp);                    /* HAL辅助宏/函数 */

          /* Change state */
          hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;   /* 赋值或初始化 */
          hcryp->State = HAL_CRYP_STATE_READY;          /* 赋值或初始化 */

          /* Process unlocked */
          __HAL_UNLOCK(hcryp);                          /* HAL辅助宏/函数 */
          return HAL_ERROR;                             /* 返回值 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Read the authentication TAG in the output FIFO */
    *(uint32_t *)(tagaddr) = hcryp->Instance->DOUT;     /* 赋值或初始化 */
    tagaddr += 4U;                                      /* 赋值或初始化 */
    *(uint32_t *)(tagaddr) = hcryp->Instance->DOUT;     /* 赋值或初始化 */
    tagaddr += 4U;                                      /* 赋值或初始化 */
    *(uint32_t *)(tagaddr) = hcryp->Instance->DOUT;     /* 赋值或初始化 */
    tagaddr += 4U;                                      /* 赋值或初始化 */
    *(uint32_t *)(tagaddr) = hcryp->Instance->DOUT;     /* 赋值或初始化 */

#else /* AES*/

    /* Select final phase */
    MODIFY_REG(hcryp->Instance->CR, AES_CR_GCMPH, CRYP_PHASE_FINAL);  /* 执行语句 */

    /* Write the number of bits in header (64 bits) followed by the number of bits
    in the payload */
    if (hcryp->Init.DataType == CRYP_DATATYPE_1B)       /* 条件判断 */
    {                                                   /* 代码块开始 */
      hcryp->Instance->DINR = 0U;                       /* 赋值或初始化 */
      hcryp->Instance->DINR = __RBIT((uint32_t)(headerlength));  /* 赋值或初始化 */
      hcryp->Instance->DINR = 0U;                       /* 赋值或初始化 */
      hcryp->Instance->DINR = __RBIT((uint32_t)(inputlength));  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if (hcryp->Init.DataType == CRYP_DATATYPE_8B)  /* 否则如果 */
    {                                                   /* 代码块开始 */
      hcryp->Instance->DINR = 0U;                       /* 赋值或初始化 */
      hcryp->Instance->DINR = __REV((uint32_t)(headerlength));  /* 赋值或初始化 */
      hcryp->Instance->DINR = 0U;                       /* 赋值或初始化 */
      hcryp->Instance->DINR = __REV((uint32_t)(inputlength));  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if (hcryp->Init.DataType == CRYP_DATATYPE_16B)  /* 否则如果 */
    {                                                   /* 代码块开始 */
      hcryp->Instance->DINR = 0U;                       /* 赋值或初始化 */
      hcryp->Instance->DINR = __ROR((uint32_t)headerlength, 16U);  /* 赋值或初始化 */
      hcryp->Instance->DINR = 0U;                       /* 赋值或初始化 */
      hcryp->Instance->DINR = __ROR((uint32_t)inputlength, 16U);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if (hcryp->Init.DataType == CRYP_DATATYPE_32B)  /* 否则如果 */
    {                                                   /* 代码块开始 */
      hcryp->Instance->DINR = 0U;                       /* 赋值或初始化 */
      hcryp->Instance->DINR = (uint32_t)(headerlength);  /* 赋值或初始化 */
      hcryp->Instance->DINR = 0U;                       /* 赋值或初始化 */
      hcryp->Instance->DINR = (uint32_t)(inputlength);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Nothing to do */
    }                                                   /* 代码块结束 */
    /* Wait for CCF flag to be raised */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */
    while (HAL_IS_BIT_CLR(hcryp->Instance->SR, AES_SR_CCF))  /* while循环 */
    {                                                   /* 代码块开始 */
      /* Check for the Timeout */
      if (Timeout != HAL_MAX_DELAY)                     /* 条件判断 */
      {                                                 /* 代码块开始 */
        if (((HAL_GetTick() - tickstart) > Timeout) || (Timeout == 0U))  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Disable the CRYP peripheral clock */
          __HAL_CRYP_DISABLE(hcryp);                    /* HAL辅助宏/函数 */

          /* Change state */
          hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;   /* 赋值或初始化 */
          hcryp->State = HAL_CRYP_STATE_READY;          /* 赋值或初始化 */

          /* Process unlocked */
          __HAL_UNLOCK(hcryp);                          /* HAL辅助宏/函数 */
          return HAL_ERROR;                             /* 返回值 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Read the authentication TAG in the output FIFO */
    *(uint32_t *)(tagaddr) = hcryp->Instance->DOUTR;    /* 赋值或初始化 */
    tagaddr += 4U;                                      /* 赋值或初始化 */
    *(uint32_t *)(tagaddr) = hcryp->Instance->DOUTR;    /* 赋值或初始化 */
    tagaddr += 4U;                                      /* 赋值或初始化 */
    *(uint32_t *)(tagaddr) = hcryp->Instance->DOUTR;    /* 赋值或初始化 */
    tagaddr += 4U;                                      /* 赋值或初始化 */
    *(uint32_t *)(tagaddr) = hcryp->Instance->DOUTR;    /* 赋值或初始化 */

    /* Clear CCF flag */
    __HAL_CRYP_CLEAR_FLAG(hcryp, CRYP_CCF_CLEAR);       /* HAL辅助宏/函数 */

#endif /* End AES or CRYP */

    /* Disable the peripheral */
    __HAL_CRYP_DISABLE(hcryp);                          /* HAL辅助宏/函数 */

    /* Change the CRYP peripheral state */
    hcryp->State = HAL_CRYP_STATE_READY;                /* 赋值或初始化 */

    /* Process unlocked */
    __HAL_UNLOCK(hcryp);                                /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Busy error code field */
    hcryp->ErrorCode |= HAL_CRYP_ERROR_BUSY;            /* 赋值或初始化 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  AES CCM Authentication TAG generation.
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module
  * @param  AuthTag: Pointer to the authentication buffer
  * @param  Timeout: Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_CRYPEx_AESCCM_GenerateAuthTAG(CRYP_HandleTypeDef *hcryp, uint32_t *AuthTag, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tagaddr = (uint32_t)AuthTag;                 /* 32位无符号变量/函数 */
  uint32_t ctr0 [4] = {0};                              /* 32位无符号变量/函数 */
  uint32_t ctr0addr = (uint32_t)ctr0;                   /* 32位无符号变量/函数 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */

  if (hcryp->State == HAL_CRYP_STATE_READY)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Process locked */
    __HAL_LOCK(hcryp);                                  /* HAL辅助宏/函数 */

    /* Change the CRYP peripheral state */
    hcryp->State = HAL_CRYP_STATE_BUSY;                 /* 赋值或初始化 */

    /* Check if initialization phase has already been performed */
    if (hcryp->Phase == CRYPEx_PHASE_PROCESS)           /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Change the CRYP phase */
      hcryp->Phase = CRYPEx_PHASE_FINAL;                /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else /* Initialization phase has not been performed*/
    {                                                   /* 代码块开始 */
      /* Disable the peripheral */
      __HAL_CRYP_DISABLE(hcryp);                        /* HAL辅助宏/函数 */

      /* Sequence error code field */
      hcryp->ErrorCode |= HAL_CRYP_ERROR_AUTH_TAG_SEQUENCE;  /* 赋值或初始化 */

      /* Change the CRYP peripheral state */
      hcryp->State = HAL_CRYP_STATE_READY;              /* 赋值或初始化 */

      /* Process unlocked */
      __HAL_UNLOCK(hcryp);                              /* HAL辅助宏/函数 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

#if defined(CRYP)                                       /* 条件编译 */

    /* Disable CRYP to start the final phase */
    __HAL_CRYP_DISABLE(hcryp);                          /* HAL辅助宏/函数 */

    /* Select final phase & ALGODIR bit must be set to 0. */
    MODIFY_REG(hcryp->Instance->CR, CRYP_CR_GCM_CCMPH | CRYP_CR_ALGODIR, CRYP_PHASE_FINAL | CRYP_OPERATINGMODE_ENCRYPT);  /* 执行语句 */

    /* Enable the CRYP peripheral */
    __HAL_CRYP_ENABLE(hcryp);                           /* HAL辅助宏/函数 */

    /* Write the counter block in the IN FIFO, CTR0 information from B0
    data has to be swapped according to the DATATYPE*/
    ctr0[0] = (hcryp->Init.B0[0]) & CRYP_CCM_CTR0_0;    /* 赋值或初始化 */
    ctr0[1] = hcryp->Init.B0[1];                        /* 赋值或初始化 */
    ctr0[2] = hcryp->Init.B0[2];                        /* 赋值或初始化 */
    ctr0[3] = hcryp->Init.B0[3] &  CRYP_CCM_CTR0_3;     /* 赋值或初始化 */

    if (hcryp->Init.DataType == CRYP_DATATYPE_8B)       /* 条件判断 */
    {                                                   /* 代码块开始 */
      hcryp->Instance->DIN = __REV(*(uint32_t *)(ctr0addr));  /* 赋值或初始化 */
      ctr0addr += 4U;                                   /* 赋值或初始化 */
      hcryp->Instance->DIN = __REV(*(uint32_t *)(ctr0addr));  /* 赋值或初始化 */
      ctr0addr += 4U;                                   /* 赋值或初始化 */
      hcryp->Instance->DIN = __REV(*(uint32_t *)(ctr0addr));  /* 赋值或初始化 */
      ctr0addr += 4U;                                   /* 赋值或初始化 */
      hcryp->Instance->DIN = __REV(*(uint32_t *)(ctr0addr));  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if (hcryp->Init.DataType == CRYP_DATATYPE_16B)  /* 否则如果 */
    {                                                   /* 代码块开始 */
      hcryp->Instance->DIN = __ROR(*(uint32_t *)(ctr0addr), 16U);  /* 赋值或初始化 */
      ctr0addr += 4U;                                   /* 赋值或初始化 */
      hcryp->Instance->DIN = __ROR(*(uint32_t *)(ctr0addr), 16U);  /* 赋值或初始化 */
      ctr0addr += 4U;                                   /* 赋值或初始化 */
      hcryp->Instance->DIN = __ROR(*(uint32_t *)(ctr0addr), 16U);  /* 赋值或初始化 */
      ctr0addr += 4U;                                   /* 赋值或初始化 */
      hcryp->Instance->DIN = __ROR(*(uint32_t *)(ctr0addr), 16U);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if (hcryp->Init.DataType == CRYP_DATATYPE_1B)  /* 否则如果 */
    {                                                   /* 代码块开始 */
      hcryp->Instance->DIN = __RBIT(*(uint32_t *)(ctr0addr));  /* 赋值或初始化 */
      ctr0addr += 4U;                                   /* 赋值或初始化 */
      hcryp->Instance->DIN = __RBIT(*(uint32_t *)(ctr0addr));  /* 赋值或初始化 */
      ctr0addr += 4U;                                   /* 赋值或初始化 */
      hcryp->Instance->DIN = __RBIT(*(uint32_t *)(ctr0addr));  /* 赋值或初始化 */
      ctr0addr += 4U;                                   /* 赋值或初始化 */
      hcryp->Instance->DIN = __RBIT(*(uint32_t *)(ctr0addr));  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hcryp->Instance->DIN = *(uint32_t *)(ctr0addr);   /* 赋值或初始化 */
      ctr0addr += 4U;                                   /* 赋值或初始化 */
      hcryp->Instance->DIN = *(uint32_t *)(ctr0addr);   /* 赋值或初始化 */
      ctr0addr += 4U;                                   /* 赋值或初始化 */
      hcryp->Instance->DIN = *(uint32_t *)(ctr0addr);   /* 赋值或初始化 */
      ctr0addr += 4U;                                   /* 赋值或初始化 */
      hcryp->Instance->DIN = *(uint32_t *)(ctr0addr);   /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    /* Wait for OFNE flag to be raised */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */
    while (HAL_IS_BIT_CLR(hcryp->Instance->SR, CRYP_FLAG_OFNE))  /* while循环 */
    {                                                   /* 代码块开始 */
      /* Check for the Timeout */
      if (Timeout != HAL_MAX_DELAY)                     /* 条件判断 */
      {                                                 /* 代码块开始 */
        if (((HAL_GetTick() - tickstart) > Timeout) || (Timeout == 0U))  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Disable the CRYP peripheral Clock */
          __HAL_CRYP_DISABLE(hcryp);                    /* HAL辅助宏/函数 */

          /* Change state */
          hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;   /* 赋值或初始化 */
          hcryp->State = HAL_CRYP_STATE_READY;          /* 赋值或初始化 */

          /* Process unlocked */
          __HAL_UNLOCK(hcryp);                          /* HAL辅助宏/函数 */
          return HAL_ERROR;                             /* 返回值 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Read the Auth TAG in the IN FIFO */
    *(uint32_t *)(tagaddr) = hcryp->Instance->DOUT;     /* 赋值或初始化 */
    tagaddr += 4U;                                      /* 赋值或初始化 */
    *(uint32_t *)(tagaddr) = hcryp->Instance->DOUT;     /* 赋值或初始化 */
    tagaddr += 4U;                                      /* 赋值或初始化 */
    *(uint32_t *)(tagaddr) = hcryp->Instance->DOUT;     /* 赋值或初始化 */
    tagaddr += 4U;                                      /* 赋值或初始化 */
    *(uint32_t *)(tagaddr) = hcryp->Instance->DOUT;     /* 赋值或初始化 */

#else /* AES */

    /* Select final phase */
    MODIFY_REG(hcryp->Instance->CR, AES_CR_GCMPH, CRYP_PHASE_FINAL);  /* 执行语句 */

    /* Write the counter block in the IN FIFO, CTR0 information from B0
    data has to be swapped according to the DATATYPE*/
    if (hcryp->Init.DataType == CRYP_DATATYPE_8B)       /* 条件判断 */
    {                                                   /* 代码块开始 */
      ctr0[0] = (__REV(hcryp->Init.B0[0]) & CRYP_CCM_CTR0_0);  /* 赋值或初始化 */
      ctr0[1] = __REV(hcryp->Init.B0[1]);               /* 赋值或初始化 */
      ctr0[2] = __REV(hcryp->Init.B0[2]);               /* 赋值或初始化 */
      ctr0[3] = (__REV(hcryp->Init.B0[3])& CRYP_CCM_CTR0_3);  /* 赋值或初始化 */

      hcryp->Instance->DINR = __REV(*(uint32_t *)(ctr0addr));  /* 赋值或初始化 */
      ctr0addr += 4U;                                   /* 赋值或初始化 */
      hcryp->Instance->DINR = __REV(*(uint32_t *)(ctr0addr));  /* 赋值或初始化 */
      ctr0addr += 4U;                                   /* 赋值或初始化 */
      hcryp->Instance->DINR = __REV(*(uint32_t *)(ctr0addr));  /* 赋值或初始化 */
      ctr0addr += 4U;                                   /* 赋值或初始化 */
      hcryp->Instance->DINR = __REV(*(uint32_t *)(ctr0addr));  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if (hcryp->Init.DataType == CRYP_DATATYPE_16B)  /* 否则如果 */
    {                                                   /* 代码块开始 */
      ctr0[0] = (__ROR((hcryp->Init.B0[0]), 16U)& CRYP_CCM_CTR0_0);  /* 赋值或初始化 */
      ctr0[1] =   __ROR((hcryp->Init.B0[1]), 16U);      /* 赋值或初始化 */
      ctr0[2] =   __ROR((hcryp->Init.B0[2]), 16U);      /* 赋值或初始化 */
      ctr0[3] = (__ROR((hcryp->Init.B0[3]), 16U)& CRYP_CCM_CTR0_3);  /* 赋值或初始化 */

      hcryp->Instance->DINR = __ROR(*(uint32_t *)(ctr0addr), 16U);  /* 赋值或初始化 */
      ctr0addr += 4U;                                   /* 赋值或初始化 */
      hcryp->Instance->DINR = __ROR(*(uint32_t *)(ctr0addr), 16U);  /* 赋值或初始化 */
      ctr0addr += 4U;                                   /* 赋值或初始化 */
      hcryp->Instance->DINR = __ROR(*(uint32_t *)(ctr0addr), 16U);  /* 赋值或初始化 */
      ctr0addr += 4U;                                   /* 赋值或初始化 */
      hcryp->Instance->DINR = __ROR(*(uint32_t *)(ctr0addr), 16U);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if (hcryp->Init.DataType == CRYP_DATATYPE_1B)  /* 否则如果 */
    {                                                   /* 代码块开始 */
      ctr0[0] = (__RBIT(hcryp->Init.B0[0])& CRYP_CCM_CTR0_0);  /* 赋值或初始化 */
      ctr0[1] = __RBIT(hcryp->Init.B0[1]);              /* 赋值或初始化 */
      ctr0[2] = __RBIT(hcryp->Init.B0[2]);              /* 赋值或初始化 */
      ctr0[3] = (__RBIT(hcryp->Init.B0[3])& CRYP_CCM_CTR0_3);  /* 赋值或初始化 */

      hcryp->Instance->DINR = __RBIT(*(uint32_t *)(ctr0addr));  /* 赋值或初始化 */
      ctr0addr += 4U;                                   /* 赋值或初始化 */
      hcryp->Instance->DINR = __RBIT(*(uint32_t *)(ctr0addr));  /* 赋值或初始化 */
      ctr0addr += 4U;                                   /* 赋值或初始化 */
      hcryp->Instance->DINR = __RBIT(*(uint32_t *)(ctr0addr));  /* 赋值或初始化 */
      ctr0addr += 4U;                                   /* 赋值或初始化 */
      hcryp->Instance->DINR = __RBIT(*(uint32_t *)(ctr0addr));  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      ctr0[0] = (hcryp->Init.B0[0]) & CRYP_CCM_CTR0_0;  /* 赋值或初始化 */
      ctr0[1] = hcryp->Init.B0[1];                      /* 赋值或初始化 */
      ctr0[2] = hcryp->Init.B0[2];                      /* 赋值或初始化 */
      ctr0[3] = hcryp->Init.B0[3] &  CRYP_CCM_CTR0_3;   /* 赋值或初始化 */

      hcryp->Instance->DINR = *(uint32_t *)(ctr0addr);  /* 赋值或初始化 */
      ctr0addr += 4U;                                   /* 赋值或初始化 */
      hcryp->Instance->DINR = *(uint32_t *)(ctr0addr);  /* 赋值或初始化 */
      ctr0addr += 4U;                                   /* 赋值或初始化 */
      hcryp->Instance->DINR = *(uint32_t *)(ctr0addr);  /* 赋值或初始化 */
      ctr0addr += 4U;                                   /* 赋值或初始化 */
      hcryp->Instance->DINR = *(uint32_t *)(ctr0addr);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Wait for CCF flag to be raised */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */
    while (HAL_IS_BIT_CLR(hcryp->Instance->SR, AES_SR_CCF))  /* while循环 */
    {                                                   /* 代码块开始 */
      /* Check for the Timeout */
      if (Timeout != HAL_MAX_DELAY)                     /* 条件判断 */
      {                                                 /* 代码块开始 */
        if (((HAL_GetTick() - tickstart) > Timeout) || (Timeout == 0U))  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Disable the CRYP peripheral Clock */
          __HAL_CRYP_DISABLE(hcryp);                    /* HAL辅助宏/函数 */

          /* Change state */
          hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;   /* 赋值或初始化 */
          hcryp->State = HAL_CRYP_STATE_READY;          /* 赋值或初始化 */

          /* Process unlocked */
          __HAL_UNLOCK(hcryp);                          /* HAL辅助宏/函数 */
          return HAL_ERROR;                             /* 返回值 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Read the authentication TAG in the output FIFO */
    *(uint32_t *)(tagaddr) = hcryp->Instance->DOUTR;    /* 赋值或初始化 */
    tagaddr += 4U;                                      /* 赋值或初始化 */
    *(uint32_t *)(tagaddr) = hcryp->Instance->DOUTR;    /* 赋值或初始化 */
    tagaddr += 4U;                                      /* 赋值或初始化 */
    *(uint32_t *)(tagaddr) = hcryp->Instance->DOUTR;    /* 赋值或初始化 */
    tagaddr += 4U;                                      /* 赋值或初始化 */
    *(uint32_t *)(tagaddr) = hcryp->Instance->DOUTR;    /* 赋值或初始化 */

    /* Clear CCF Flag */
    __HAL_CRYP_CLEAR_FLAG(hcryp, CRYP_CCF_CLEAR);       /* HAL辅助宏/函数 */

#endif /* End of AES || CRYP */

    /* Change the CRYP peripheral state */
    hcryp->State = HAL_CRYP_STATE_READY;                /* 赋值或初始化 */

    /* Process unlocked */
    __HAL_UNLOCK(hcryp);                                /* HAL辅助宏/函数 */

    /* Disable CRYP  */
    __HAL_CRYP_DISABLE(hcryp);                          /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Busy error code field */
    hcryp->ErrorCode = HAL_CRYP_ERROR_BUSY;             /* 赋值或初始化 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

#if defined (AES)                                       /* 条件编译 */
/** @defgroup CRYPEx_Exported_Functions_Group2 Key Derivation functions
  *  @brief   AutoKeyDerivation functions
  *
@verbatim
  ==============================================================================
              ##### Key Derivation functions #####
  ==============================================================================
    [..]  This section provides functions allowing to Enable or Disable the
          the AutoKeyDerivation parameter in CRYP_HandleTypeDef structure
          These function are allowed only in TinyAES IP.

@endverbatim
  * @{
  */

/**
  * @brief  AES enable key derivation functions
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure.
  * @retval None
  */
void  HAL_CRYPEx_EnableAutoKeyDerivation(CRYP_HandleTypeDef *hcryp)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  if (hcryp->State == HAL_CRYP_STATE_READY)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    hcryp->AutoKeyDerivation = ENABLE;                  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Busy error code field */
    hcryp->ErrorCode = HAL_CRYP_ERROR_BUSY;             /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */
/**
  * @brief  AES disable key derivation functions
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure.
  * @retval None
  */
void  HAL_CRYPEx_DisableAutoKeyDerivation(CRYP_HandleTypeDef *hcryp)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  if (hcryp->State == HAL_CRYP_STATE_READY)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    hcryp->AutoKeyDerivation = DISABLE;                 /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Busy error code field */
    hcryp->ErrorCode = HAL_CRYP_ERROR_BUSY;             /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @}
  */
#endif /* AES or GCM CCM defined*/
#endif /* AES */
#endif /* HAL_CRYP_MODULE_ENABLED */

/**
  * @}
  */
#endif /* TinyAES or CRYP*/
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
