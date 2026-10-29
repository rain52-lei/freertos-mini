/* 文件: stm32f4xx_hal_can.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_can.c
  * @author  MCD Application Team
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the Controller Area Network (CAN) peripheral:
  *           + Initialization and de-initialization functions 
  *           + IO operation functions
  *           + Peripheral Control functions
  *           + Peripheral State and Error functions
  *
  @verbatim
  ==============================================================================
                                 ##### User NOTE #####
  ==============================================================================
    [..]
      (#) This HAL CAN driver is deprecated, it contains some CAN Tx/Rx FIFO management limitations.
          Another HAL CAN driver version has been designed with new API's, to fix these limitations.

  ==============================================================================
                        ##### How to use this driver #####
  ==============================================================================
    [..]            
      (#) Enable the CAN controller interface clock using 
          __HAL_RCC_CAN1_CLK_ENABLE() for CAN1, __HAL_RCC_CAN2_CLK_ENABLE() for CAN2
         and __HAL_RCC_CAN3_CLK_ENABLE() for CAN3
      -@- In case you are using CAN2 only, you have to enable the CAN1 clock.

      (#) CAN pins configuration
        (++) Enable the clock for the CAN GPIOs using the following function:
             __GPIOx_CLK_ENABLE()   
        (++) Connect and configure the involved CAN pins to AF9 using the 
              following function HAL_GPIO_Init() 

      (#) Initialize and configure the CAN using CAN_Init() function.   

      (#) Transmit the desired CAN frame using HAL_CAN_Transmit() function.

      (#) Or transmit the desired CAN frame using HAL_CAN_Transmit_IT() function.

      (#) Receive a CAN frame using HAL_CAN_Receive() function.

      (#) Or receive a CAN frame using HAL_CAN_Receive_IT() function.

     *** Polling mode IO operation ***
     =================================
     [..]    
       (+) Start the CAN peripheral transmission and wait the end of this operation 
           using HAL_CAN_Transmit(), at this stage user can specify the value of timeout
           according to his end application
       (+) Start the CAN peripheral reception and wait the end of this operation 
           using HAL_CAN_Receive(), at this stage user can specify the value of timeout
           according to his end application 

     *** Interrupt mode IO operation ***    
     ===================================
     [..]    
       (+) Start the CAN peripheral transmission using HAL_CAN_Transmit_IT()
       (+) Start the CAN peripheral reception using HAL_CAN_Receive_IT()         
       (+) Use HAL_CAN_IRQHandler() called under the used CAN Interrupt subroutine
       (+) At CAN end of transmission HAL_CAN_TxCpltCallback() function is executed and user can 
            add his own code by customization of function pointer HAL_CAN_TxCpltCallback 
       (+) In case of CAN Error, HAL_CAN_ErrorCallback() function is executed and user can 
            add his own code by customization of function pointer HAL_CAN_ErrorCallback

     *** CAN HAL driver macros list ***
     ============================================= 
     [..]
       Below the list of most used macros in CAN HAL driver.

      (+) __HAL_CAN_ENABLE_IT: Enable the specified CAN interrupts
      (+) __HAL_CAN_DISABLE_IT: Disable the specified CAN interrupts
      (+) __HAL_CAN_GET_IT_SOURCE: Check if the specified CAN interrupt source is enabled or disabled
      (+) __HAL_CAN_CLEAR_FLAG: Clear the CAN's pending flags
      (+) __HAL_CAN_GET_FLAG: Get the selected CAN's flag status

     [..]
      (@) You can refer to the CAN Legacy HAL driver header file for more useful macros

  @endverbatim

  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"                              /* 包含 STM32/HAL 头文件 */

/** @addtogroup STM32F4xx_HAL_Driver
  * @{
  */

/** @defgroup CAN CAN
  * @brief CAN driver modules
  * @{
  */ 

#ifdef HAL_CAN_LEGACY_MODULE_ENABLED                    /* 条件编译 */

#if defined(STM32F405xx) || defined(STM32F415xx) || defined(STM32F407xx) || defined(STM32F417xx) ||\
    defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx) ||\
    defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx) || defined(STM32F412Zx) ||\
    defined(STM32F412Vx) || defined(STM32F412Rx) || defined(STM32F412Cx) || defined(STM32F413xx) ||\
    defined(STM32F423xx)                                /* 源代码 */

#ifdef HAL_CAN_MODULE_ENABLED                           /* 条件编译 */
/* Select HAL CAN module in stm32f4xx_hal_conf.h file:
   (#) HAL_CAN_MODULE_ENABLED for new HAL CAN driver fixing FIFO limitations
   (#) HAL_CAN_LEGACY_MODULE_ENABLED for legacy HAL CAN driver */
#error 'The HAL CAN driver cannot be used with its legacy, Please ensure to enable only one HAL CAN module at once in stm32f4xx_hal_conf.h file'  /* 编译错误指示 */
#endif /* HAL_CAN_MODULE_ENABLED */

#warning 'Legacy HAL CAN driver is enabled! It can be used with known limitations, refer to the release notes. However it is recommended to use rather the new HAL CAN driver'  /* 编译警告指示 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @addtogroup CAN_Private_Constants
  * @{
  */
#define CAN_TIMEOUT_VALUE  10U                          /* 定义宏 */
/**
  * @}
  */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/** @addtogroup CAN_Private_Functions
  * @{
  */
static HAL_StatusTypeDef CAN_Receive_IT(CAN_HandleTypeDef* hcan, uint8_t FIFONumber);  /* 静态变量/函数 */
static HAL_StatusTypeDef CAN_Transmit_IT(CAN_HandleTypeDef* hcan);  /* 静态变量/函数 */
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup CAN_Exported_Functions CAN Exported Functions
  * @{
  */

/** @defgroup CAN_Exported_Functions_Group1 Initialization and de-initialization functions 
 *  @brief    Initialization and Configuration functions 
 *
@verbatim    
  ==============================================================================
              ##### Initialization and de-initialization functions #####
  ==============================================================================
    [..]  This section provides functions allowing to:
      (+) Initialize and configure the CAN. 
      (+) De-initialize the CAN. 

@endverbatim
  * @{
  */

/**
  * @brief  Initializes the CAN peripheral according to the specified
  *         parameters in the CAN_InitStruct.
  * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.  
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_CAN_Init(CAN_HandleTypeDef* hcan)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t InitStatus = CAN_INITSTATUS_FAILED;          /* 32位无符号变量/函数 */
  uint32_t tickstart = 0U;                              /* 32位无符号变量/函数 */

  /* Check CAN handle */
  if(hcan == NULL)                                      /* 条件判断 */
  {                                                     /* 代码块开始 */
     return HAL_ERROR;                                  /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the parameters */
  assert_param(IS_CAN_ALL_INSTANCE(hcan->Instance));    /* 执行语句 */
  assert_param(IS_FUNCTIONAL_STATE(hcan->Init.TTCM));   /* 执行语句 */
  assert_param(IS_FUNCTIONAL_STATE(hcan->Init.ABOM));   /* 执行语句 */
  assert_param(IS_FUNCTIONAL_STATE(hcan->Init.AWUM));   /* 执行语句 */
  assert_param(IS_FUNCTIONAL_STATE(hcan->Init.NART));   /* 执行语句 */
  assert_param(IS_FUNCTIONAL_STATE(hcan->Init.RFLM));   /* 执行语句 */
  assert_param(IS_FUNCTIONAL_STATE(hcan->Init.TXFP));   /* 执行语句 */
  assert_param(IS_CAN_MODE(hcan->Init.Mode));           /* 执行语句 */
  assert_param(IS_CAN_SJW(hcan->Init.SJW));             /* 执行语句 */
  assert_param(IS_CAN_BS1(hcan->Init.BS1));             /* 执行语句 */
  assert_param(IS_CAN_BS2(hcan->Init.BS2));             /* 执行语句 */
  assert_param(IS_CAN_PRESCALER(hcan->Init.Prescaler));  /* 执行语句 */


  if(hcan->State == HAL_CAN_STATE_RESET)                /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Allocate lock resource and initialize it */
    hcan->Lock = HAL_UNLOCKED;                          /* 赋值或初始化 */
    /* Init the low level hardware */
    HAL_CAN_MspInit(hcan);                              /* HAL库函数调用 */
  }                                                     /* 代码块结束 */

  /* Initialize the CAN state*/
  hcan->State = HAL_CAN_STATE_BUSY;                     /* 赋值或初始化 */

  /* Exit from sleep mode */
  hcan->Instance->MCR &= (~(uint32_t)CAN_MCR_SLEEP);    /* 赋值或初始化 */

  /* Request initialisation */
  hcan->Instance->MCR |= CAN_MCR_INRQ ;                 /* 赋值或初始化 */

  /* Get tick */
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  /* Wait the acknowledge */
  while((hcan->Instance->MSR & CAN_MSR_INAK) != CAN_MSR_INAK)  /* while循环 */
  {                                                     /* 代码块开始 */
    if((HAL_GetTick() - tickstart ) > CAN_TIMEOUT_VALUE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hcan->State= HAL_CAN_STATE_TIMEOUT;               /* 赋值或初始化 */
      /* Process unlocked */
      __HAL_UNLOCK(hcan);                               /* HAL辅助宏/函数 */
      return HAL_TIMEOUT;                               /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Check acknowledge */
  if ((hcan->Instance->MSR & CAN_MSR_INAK) == CAN_MSR_INAK)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Set the time triggered communication mode */
    if (hcan->Init.TTCM == ENABLE)                      /* 条件判断 */
    {                                                   /* 代码块开始 */
      hcan->Instance->MCR |= CAN_MCR_TTCM;              /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hcan->Instance->MCR &= ~(uint32_t)CAN_MCR_TTCM;   /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Set the automatic bus-off management */
    if (hcan->Init.ABOM == ENABLE)                      /* 条件判断 */
    {                                                   /* 代码块开始 */
      hcan->Instance->MCR |= CAN_MCR_ABOM;              /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hcan->Instance->MCR &= ~(uint32_t)CAN_MCR_ABOM;   /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Set the automatic wake-up mode */
    if (hcan->Init.AWUM == ENABLE)                      /* 条件判断 */
    {                                                   /* 代码块开始 */
      hcan->Instance->MCR |= CAN_MCR_AWUM;              /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hcan->Instance->MCR &= ~(uint32_t)CAN_MCR_AWUM;   /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Set the no automatic retransmission */
    if (hcan->Init.NART == ENABLE)                      /* 条件判断 */
    {                                                   /* 代码块开始 */
      hcan->Instance->MCR |= CAN_MCR_NART;              /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hcan->Instance->MCR &= ~(uint32_t)CAN_MCR_NART;   /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Set the receive FIFO locked mode */
    if (hcan->Init.RFLM == ENABLE)                      /* 条件判断 */
    {                                                   /* 代码块开始 */
      hcan->Instance->MCR |= CAN_MCR_RFLM;              /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hcan->Instance->MCR &= ~(uint32_t)CAN_MCR_RFLM;   /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Set the transmit FIFO priority */
    if (hcan->Init.TXFP == ENABLE)                      /* 条件判断 */
    {                                                   /* 代码块开始 */
      hcan->Instance->MCR |= CAN_MCR_TXFP;              /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hcan->Instance->MCR &= ~(uint32_t)CAN_MCR_TXFP;   /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Set the bit timing register */
    hcan->Instance->BTR = (uint32_t)((uint32_t)hcan->Init.Mode) | \
                ((uint32_t)hcan->Init.SJW) | \
                ((uint32_t)hcan->Init.BS1) | \
                ((uint32_t)hcan->Init.BS2) | \
               ((uint32_t)hcan->Init.Prescaler - 1U);   /* 执行语句 */

    /* Request leave initialisation */
    hcan->Instance->MCR &= ~(uint32_t)CAN_MCR_INRQ;     /* 赋值或初始化 */

  /* Get tick */
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

   /* Wait the acknowledge */
   while((hcan->Instance->MSR & CAN_MSR_INAK) == CAN_MSR_INAK)  /* while循环 */
   {                                                    /* 代码块开始 */
    if((HAL_GetTick() - tickstart ) > CAN_TIMEOUT_VALUE)  /* 条件判断 */
     {                                                  /* 代码块开始 */
       hcan->State= HAL_CAN_STATE_TIMEOUT;              /* 赋值或初始化 */
       /* Process unlocked */
       __HAL_UNLOCK(hcan);                              /* HAL辅助宏/函数 */
       return HAL_TIMEOUT;                              /* 返回值 */
     }                                                  /* 代码块结束 */
   }                                                    /* 代码块结束 */

    /* Check acknowledged */
    if ((hcan->Instance->MSR & CAN_MSR_INAK) != CAN_MSR_INAK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      InitStatus = CAN_INITSTATUS_SUCCESS;              /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  if(InitStatus == CAN_INITSTATUS_SUCCESS)              /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Set CAN error code to none */
    hcan->ErrorCode = HAL_CAN_ERROR_NONE;               /* 赋值或初始化 */

    /* Initialize the CAN state */
    hcan->State = HAL_CAN_STATE_READY;                  /* 赋值或初始化 */

    /* Return function status */
    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Initialize the CAN state */
    hcan->State = HAL_CAN_STATE_ERROR;                  /* 赋值或初始化 */

    /* Return function status */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configures the CAN reception filter according to the specified
  *         parameters in the CAN_FilterInitStruct.
  * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @param  sFilterConfig pointer to a CAN_FilterConfTypeDef structure that
  *         contains the filter configuration information.
  * @retval None
  */
HAL_StatusTypeDef HAL_CAN_ConfigFilter(CAN_HandleTypeDef* hcan, CAN_FilterConfTypeDef* sFilterConfig)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t filternbrbitpos = 0U;                        /* 32位无符号变量/函数 */
  CAN_TypeDef *can_ip;                                  /* 执行语句 */

  /* Prevent unused argument(s) compilation warning */
  UNUSED(hcan);                                         /* 执行语句 */

  /* Check the parameters */
  assert_param(IS_CAN_FILTER_NUMBER(sFilterConfig->FilterNumber));  /* 执行语句 */
  assert_param(IS_CAN_FILTER_MODE(sFilterConfig->FilterMode));  /* 执行语句 */
  assert_param(IS_CAN_FILTER_SCALE(sFilterConfig->FilterScale));  /* 执行语句 */
  assert_param(IS_CAN_FILTER_FIFO(sFilterConfig->FilterFIFOAssignment));  /* 执行语句 */
  assert_param(IS_FUNCTIONAL_STATE(sFilterConfig->FilterActivation));  /* 执行语句 */
  assert_param(IS_CAN_BANKNUMBER(sFilterConfig->BankNumber));  /* 执行语句 */

  filternbrbitpos = 1U << sFilterConfig->FilterNumber;  /* 赋值或初始化 */
#if defined (CAN3)                                      /* 条件编译 */
  /* Check the CAN instance */
  if(hcan->Instance == CAN3)                            /* 条件判断 */
  {	                                                    /* 代码块开始 */
    can_ip = CAN3;                                      /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    can_ip = CAN1;                                      /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
#else                                                   /* 条件编译否则 */
  can_ip = CAN1;                                        /* 赋值或初始化 */
#endif                                                  /* 条件编译结束 */

  /* Initialisation mode for the filter */
  can_ip->FMR |= (uint32_t)CAN_FMR_FINIT;               /* 赋值或初始化 */

#if defined (CAN2)                                      /* 条件编译 */
  /* Select the start slave bank */
  can_ip->FMR &= ~((uint32_t)CAN_FMR_CAN2SB);           /* 赋值或初始化 */
  can_ip->FMR |= (uint32_t)(sFilterConfig->BankNumber << 8U);  /* 赋值或初始化 */
#endif                                                  /* 条件编译结束 */

  /* Filter Deactivation */
  can_ip->FA1R &= ~(uint32_t)filternbrbitpos;           /* 赋值或初始化 */

  /* Filter Scale */
  if (sFilterConfig->FilterScale == CAN_FILTERSCALE_16BIT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* 16-bit scale for the filter */
    can_ip->FS1R &= ~(uint32_t)filternbrbitpos;         /* 赋值或初始化 */

    /* First 16-bit identifier and First 16-bit mask */
    /* Or First 16-bit identifier and Second 16-bit identifier */
    can_ip->sFilterRegister[sFilterConfig->FilterNumber].FR1 =   /* 源代码 */
       ((0x0000FFFFU & (uint32_t)sFilterConfig->FilterMaskIdLow) << 16U) |  /* 源代码 */
        (0x0000FFFFU & (uint32_t)sFilterConfig->FilterIdLow);  /* 执行语句 */

    /* Second 16-bit identifier and Second 16-bit mask */
    /* Or Third 16-bit identifier and Fourth 16-bit identifier */
    can_ip->sFilterRegister[sFilterConfig->FilterNumber].FR2 =   /* 源代码 */
       ((0x0000FFFFU & (uint32_t)sFilterConfig->FilterMaskIdHigh) << 16U) |  /* 源代码 */
        (0x0000FFFFU & (uint32_t)sFilterConfig->FilterIdHigh);  /* 执行语句 */
  }                                                     /* 代码块结束 */

  if (sFilterConfig->FilterScale == CAN_FILTERSCALE_32BIT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* 32-bit scale for the filter */
    can_ip->FS1R |= filternbrbitpos;                    /* 赋值或初始化 */

    /* 32-bit identifier or First 32-bit identifier */
    can_ip->sFilterRegister[sFilterConfig->FilterNumber].FR1 =   /* 源代码 */
       ((0x0000FFFFU & (uint32_t)sFilterConfig->FilterIdHigh) << 16U) |  /* 源代码 */
        (0x0000FFFFU & (uint32_t)sFilterConfig->FilterIdLow);  /* 执行语句 */
    /* 32-bit mask or Second 32-bit identifier */
    can_ip->sFilterRegister[sFilterConfig->FilterNumber].FR2 =   /* 源代码 */
       ((0x0000FFFFU & (uint32_t)sFilterConfig->FilterMaskIdHigh) << 16U) |  /* 源代码 */
        (0x0000FFFFU & (uint32_t)sFilterConfig->FilterMaskIdLow);  /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Filter Mode */
  if (sFilterConfig->FilterMode == CAN_FILTERMODE_IDMASK)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /*Id/Mask mode for the filter*/
    can_ip->FM1R &= ~(uint32_t)filternbrbitpos;         /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else /* CAN_FilterInitStruct->CAN_FilterMode == CAN_FilterMode_IdList */
  {                                                     /* 代码块开始 */
    /*Identifier list mode for the filter*/
    can_ip->FM1R |= (uint32_t)filternbrbitpos;          /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Filter FIFO assignment */
  if (sFilterConfig->FilterFIFOAssignment == CAN_FILTER_FIFO0)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* FIFO 0 assignation for the filter */
    can_ip->FFA1R &= ~(uint32_t)filternbrbitpos;        /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if (sFilterConfig->FilterFIFOAssignment == CAN_FILTER_FIFO1)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* FIFO 1 assignation for the filter */
    can_ip->FFA1R |= (uint32_t)filternbrbitpos;         /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Filter activation */
  if (sFilterConfig->FilterActivation == ENABLE)        /* 条件判断 */
  {                                                     /* 代码块开始 */
    can_ip->FA1R |= filternbrbitpos;                    /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Leave the initialisation mode for the filter */
  can_ip->FMR &= ~((uint32_t)CAN_FMR_FINIT);            /* 赋值或初始化 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Deinitializes the CANx peripheral registers to their default reset values. 
  * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.  
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_CAN_DeInit(CAN_HandleTypeDef* hcan)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check CAN handle */
  if(hcan == NULL)                                      /* 条件判断 */
  {                                                     /* 代码块开始 */
     return HAL_ERROR;                                  /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the parameters */
  assert_param(IS_CAN_ALL_INSTANCE(hcan->Instance));    /* 执行语句 */

  /* Change CAN state */
  hcan->State = HAL_CAN_STATE_BUSY;                     /* 赋值或初始化 */

  /* DeInit the low level hardware */
  HAL_CAN_MspDeInit(hcan);                              /* HAL库函数调用 */

  /* Change CAN state */
  hcan->State = HAL_CAN_STATE_RESET;                    /* 赋值或初始化 */

  /* Release Lock */
  __HAL_UNLOCK(hcan);                                   /* HAL辅助宏/函数 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initializes the CAN MSP.
  * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.  
  * @retval None
  */
__weak void HAL_CAN_MspInit(CAN_HandleTypeDef* hcan)    /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hcan);                                         /* 执行语句 */
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_CAN_MspInit could be implemented in the user file
   */ 
}                                                       /* 代码块结束 */

/**
  * @brief  DeInitializes the CAN MSP.
  * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.  
  * @retval None
  */
__weak void HAL_CAN_MspDeInit(CAN_HandleTypeDef* hcan)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hcan);                                         /* 执行语句 */
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_CAN_MspDeInit could be implemented in the user file
   */ 
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup CAN_Exported_Functions_Group2 IO operation functions
 *  @brief    IO operation functions 
 *
@verbatim   
  ==============================================================================
                      ##### IO operation functions #####
  ==============================================================================
    [..]  This section provides functions allowing to:
      (+) Transmit a CAN frame message.
      (+) Receive a CAN frame message.
      (+) Enter CAN peripheral in sleep mode. 
      (+) Wake up the CAN peripheral from sleep mode.

@endverbatim
  * @{
  */

/**
  * @brief  Initiates and transmits a CAN frame message.
  * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.  
  * @param  Timeout Specify Timeout value   
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_CAN_Transmit(CAN_HandleTypeDef* hcan, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t transmitmailbox = CAN_TXSTATUS_NOMAILBOX;    /* 32位无符号变量/函数 */
  uint32_t tickstart = 0U;                              /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_CAN_IDTYPE(hcan->pTxMsg->IDE));       /* 执行语句 */
  assert_param(IS_CAN_RTR(hcan->pTxMsg->RTR));          /* 执行语句 */
  assert_param(IS_CAN_DLC(hcan->pTxMsg->DLC));          /* 执行语句 */

  if(((hcan->Instance->TSR&CAN_TSR_TME0) == CAN_TSR_TME0) || \
     ((hcan->Instance->TSR&CAN_TSR_TME1) == CAN_TSR_TME1) || \
     ((hcan->Instance->TSR&CAN_TSR_TME2) == CAN_TSR_TME2))  /* 源代码 */
  {                                                     /* 代码块开始 */
    /* Process locked */
    __HAL_LOCK(hcan);                                   /* HAL辅助宏/函数 */

    /* Change CAN state */
    switch(hcan->State)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case(HAL_CAN_STATE_BUSY_RX0):                     /* 标签 */
          hcan->State = HAL_CAN_STATE_BUSY_TX_RX0;      /* 赋值或初始化 */
          break;                                        /* 跳出 */
      case(HAL_CAN_STATE_BUSY_RX1):                     /* 标签 */
          hcan->State = HAL_CAN_STATE_BUSY_TX_RX1;      /* 赋值或初始化 */
          break;                                        /* 跳出 */
      case(HAL_CAN_STATE_BUSY_RX0_RX1):                 /* 标签 */
          hcan->State = HAL_CAN_STATE_BUSY_TX_RX0_RX1;  /* 赋值或初始化 */
          break;                                        /* 跳出 */
      default: /* HAL_CAN_STATE_READY */
          hcan->State = HAL_CAN_STATE_BUSY_TX;          /* 赋值或初始化 */
          break;                                        /* 跳出 */
    }                                                   /* 代码块结束 */

    /* Select one empty transmit mailbox */
    if ((hcan->Instance->TSR&CAN_TSR_TME0) == CAN_TSR_TME0)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      transmitmailbox = CAN_TXMAILBOX_0;                /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if ((hcan->Instance->TSR&CAN_TSR_TME1) == CAN_TSR_TME1)  /* 否则如果 */
    {                                                   /* 代码块开始 */
      transmitmailbox = CAN_TXMAILBOX_1;                /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      transmitmailbox = CAN_TXMAILBOX_2;                /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Set up the Id */
    hcan->Instance->sTxMailBox[transmitmailbox].TIR &= CAN_TI0R_TXRQ;  /* 赋值或初始化 */
    if (hcan->pTxMsg->IDE == CAN_ID_STD)                /* 条件判断 */
    {                                                   /* 代码块开始 */
      assert_param(IS_CAN_STDID(hcan->pTxMsg->StdId));    /* 执行语句 */
      hcan->Instance->sTxMailBox[transmitmailbox].TIR |= ((hcan->pTxMsg->StdId << 21U) | \
                                                  hcan->pTxMsg->RTR);  /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      assert_param(IS_CAN_EXTID(hcan->pTxMsg->ExtId));  /* 执行语句 */
      hcan->Instance->sTxMailBox[transmitmailbox].TIR |= ((hcan->pTxMsg->ExtId << 3U) | \
                                                  hcan->pTxMsg->IDE | \
                                                  hcan->pTxMsg->RTR);  /* 执行语句 */
    }                                                   /* 代码块结束 */

    /* Set up the DLC */
    hcan->pTxMsg->DLC &= (uint8_t)0x0000000F;           /* 赋值或初始化 */
    hcan->Instance->sTxMailBox[transmitmailbox].TDTR &= (uint32_t)0xFFFFFFF0U;  /* 赋值或初始化 */
    hcan->Instance->sTxMailBox[transmitmailbox].TDTR |= hcan->pTxMsg->DLC;  /* 赋值或初始化 */

    /* Set up the data field */
    hcan->Instance->sTxMailBox[transmitmailbox].TDLR = (((uint32_t)hcan->pTxMsg->Data[3U] << 24U) |   /* 源代码 */
                                             ((uint32_t)hcan->pTxMsg->Data[2U] << 16U) |  /* 源代码 */
                                             ((uint32_t)hcan->pTxMsg->Data[1U] << 8U) |   /* 源代码 */
                                             ((uint32_t)hcan->pTxMsg->Data[0U]));  /* 执行语句 */
    hcan->Instance->sTxMailBox[transmitmailbox].TDHR = (((uint32_t)hcan->pTxMsg->Data[7U] << 24U) |   /* 源代码 */
                                             ((uint32_t)hcan->pTxMsg->Data[6U] << 16U) |  /* 源代码 */
                                             ((uint32_t)hcan->pTxMsg->Data[5U] << 8U) |  /* 源代码 */
                                             ((uint32_t)hcan->pTxMsg->Data[4U]));  /* 执行语句 */
    /* Request transmission */
    hcan->Instance->sTxMailBox[transmitmailbox].TIR |= CAN_TI0R_TXRQ;  /* 赋值或初始化 */

    /* Get tick */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */

    /* Check End of transmission flag */
    while(!(__HAL_CAN_TRANSMIT_STATUS(hcan, transmitmailbox)))  /* while循环 */
    {                                                   /* 代码块开始 */
      /* Check for the Timeout */
      if(Timeout != HAL_MAX_DELAY)                      /* 条件判断 */
      {                                                 /* 代码块开始 */
       if((Timeout == 0U)||((HAL_GetTick() - tickstart ) > Timeout))  /* 条件判断 */
       {                                                /* 代码块开始 */
         hcan->State = HAL_CAN_STATE_TIMEOUT;           /* 赋值或初始化 */

         __HAL_CAN_CANCEL_TRANSMIT(hcan, transmitmailbox);  /* HAL辅助宏/函数 */

         /* Process unlocked */
         __HAL_UNLOCK(hcan);                            /* HAL辅助宏/函数 */
         return HAL_TIMEOUT;                            /* 返回值 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Change CAN state */
    switch(hcan->State)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case(HAL_CAN_STATE_BUSY_TX_RX0):                  /* 标签 */
          hcan->State = HAL_CAN_STATE_BUSY_RX0;         /* 赋值或初始化 */
          break;                                        /* 跳出 */
      case(HAL_CAN_STATE_BUSY_TX_RX1):                  /* 标签 */
          hcan->State = HAL_CAN_STATE_BUSY_RX1;         /* 赋值或初始化 */
          break;                                        /* 跳出 */
      case(HAL_CAN_STATE_BUSY_TX_RX0_RX1):              /* 标签 */
          hcan->State = HAL_CAN_STATE_BUSY_RX0_RX1;     /* 赋值或初始化 */
          break;                                        /* 跳出 */
      default: /* HAL_CAN_STATE_BUSY_TX */
          hcan->State = HAL_CAN_STATE_READY;            /* 赋值或初始化 */
          break;                                        /* 跳出 */
    }                                                   /* 代码块结束 */

    /* Process unlocked */
    __HAL_UNLOCK(hcan);                                 /* HAL辅助宏/函数 */

    /* Return function status */
    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Change CAN state */
    hcan->State = HAL_CAN_STATE_ERROR;                  /* 赋值或初始化 */

    /* Return function status */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initiates and transmits a CAN frame message.
  * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.  
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_CAN_Transmit_IT(CAN_HandleTypeDef* hcan)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t  transmitmailbox = CAN_TXSTATUS_NOMAILBOX;   /* 32位无符号变量/函数 */

  /* Check the parameters */
  assert_param(IS_CAN_IDTYPE(hcan->pTxMsg->IDE));       /* 执行语句 */
  assert_param(IS_CAN_RTR(hcan->pTxMsg->RTR));          /* 执行语句 */
  assert_param(IS_CAN_DLC(hcan->pTxMsg->DLC));          /* 执行语句 */

  if(((hcan->Instance->TSR&CAN_TSR_TME0) == CAN_TSR_TME0) || \
     ((hcan->Instance->TSR&CAN_TSR_TME1) == CAN_TSR_TME1) || \
     ((hcan->Instance->TSR&CAN_TSR_TME2) == CAN_TSR_TME2))  /* 源代码 */
  {                                                     /* 代码块开始 */
    /* Process Locked */
    __HAL_LOCK(hcan);                                   /* HAL辅助宏/函数 */

    /* Select one empty transmit mailbox */
    if((hcan->Instance->TSR&CAN_TSR_TME0) == CAN_TSR_TME0)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      transmitmailbox = CAN_TXMAILBOX_0;                /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if((hcan->Instance->TSR&CAN_TSR_TME1) == CAN_TSR_TME1)  /* 否则如果 */
    {                                                   /* 代码块开始 */
      transmitmailbox = CAN_TXMAILBOX_1;                /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      transmitmailbox = CAN_TXMAILBOX_2;                /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Set up the Id */
    hcan->Instance->sTxMailBox[transmitmailbox].TIR &= CAN_TI0R_TXRQ;  /* 赋值或初始化 */
    if(hcan->pTxMsg->IDE == CAN_ID_STD)                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      assert_param(IS_CAN_STDID(hcan->pTxMsg->StdId));    /* 执行语句 */
      hcan->Instance->sTxMailBox[transmitmailbox].TIR |= ((hcan->pTxMsg->StdId << 21U) | \
                                                hcan->pTxMsg->RTR);  /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      assert_param(IS_CAN_EXTID(hcan->pTxMsg->ExtId));  /* 执行语句 */
      hcan->Instance->sTxMailBox[transmitmailbox].TIR |= ((hcan->pTxMsg->ExtId << 3U) | \
                                                hcan->pTxMsg->IDE | \
                                                hcan->pTxMsg->RTR);  /* 执行语句 */
    }                                                   /* 代码块结束 */

    /* Set up the DLC */
    hcan->pTxMsg->DLC &= (uint8_t)0x0000000F;           /* 赋值或初始化 */
    hcan->Instance->sTxMailBox[transmitmailbox].TDTR &= (uint32_t)0xFFFFFFF0U;  /* 赋值或初始化 */
    hcan->Instance->sTxMailBox[transmitmailbox].TDTR |= hcan->pTxMsg->DLC;  /* 赋值或初始化 */

    /* Set up the data field */
    hcan->Instance->sTxMailBox[transmitmailbox].TDLR = (((uint32_t)hcan->pTxMsg->Data[3U] << 24U) |  /* 源代码 */
                                           ((uint32_t)hcan->pTxMsg->Data[2U] << 16U) |  /* 源代码 */
                                           ((uint32_t)hcan->pTxMsg->Data[1U] << 8U) |   /* 源代码 */
                                           ((uint32_t)hcan->pTxMsg->Data[0U]));  /* 执行语句 */
    hcan->Instance->sTxMailBox[transmitmailbox].TDHR = (((uint32_t)hcan->pTxMsg->Data[7U] << 24U) |  /* 源代码 */
                                           ((uint32_t)hcan->pTxMsg->Data[6U] << 16U) |  /* 源代码 */
                                           ((uint32_t)hcan->pTxMsg->Data[5U] << 8U) |  /* 源代码 */
                                           ((uint32_t)hcan->pTxMsg->Data[4U]));  /* 执行语句 */

    /* Change CAN state */
    switch(hcan->State)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case(HAL_CAN_STATE_BUSY_RX0):                     /* 标签 */
          hcan->State = HAL_CAN_STATE_BUSY_TX_RX0;      /* 赋值或初始化 */
          break;                                        /* 跳出 */
      case(HAL_CAN_STATE_BUSY_RX1):                     /* 标签 */
          hcan->State = HAL_CAN_STATE_BUSY_TX_RX1;      /* 赋值或初始化 */
          break;                                        /* 跳出 */
      case(HAL_CAN_STATE_BUSY_RX0_RX1):                 /* 标签 */
          hcan->State = HAL_CAN_STATE_BUSY_TX_RX0_RX1;  /* 赋值或初始化 */
          break;                                        /* 跳出 */
      default: /* HAL_CAN_STATE_READY */
          hcan->State = HAL_CAN_STATE_BUSY_TX;          /* 赋值或初始化 */
          break;                                        /* 跳出 */
    }                                                   /* 代码块结束 */

    /* Set CAN error code to none */
    hcan->ErrorCode = HAL_CAN_ERROR_NONE;               /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hcan);                                 /* HAL辅助宏/函数 */

    /* Request transmission */
    hcan->Instance->sTxMailBox[transmitmailbox].TIR |= CAN_TI0R_TXRQ;  /* 赋值或初始化 */

    /* Enable Error warning, Error passive, Bus-off,
       Last error and Error Interrupts */
    __HAL_CAN_ENABLE_IT(hcan, CAN_IT_EWG |              /* HAL辅助宏/函数 */
                              CAN_IT_EPV |              /* 源代码 */
                              CAN_IT_BOF |              /* 源代码 */
                              CAN_IT_LEC |              /* 源代码 */
                              CAN_IT_ERR |              /* 源代码 */
                              CAN_IT_TME);              /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Change CAN state */
    hcan->State = HAL_CAN_STATE_ERROR;                  /* 赋值或初始化 */

    /* Return function status */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Receives a correct CAN frame.
  * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.  
  * @param  FIFONumber FIFO Number value
  * @param  Timeout Specify Timeout value 
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_CAN_Receive(CAN_HandleTypeDef* hcan, uint8_t FIFONumber, uint32_t Timeout)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart = 0U;                              /* 32位无符号变量/函数 */
  CanRxMsgTypeDef* pRxMsg = NULL;                       /* 赋值或初始化 */

  /* Check the parameters */
  assert_param(IS_CAN_FIFO(FIFONumber));                /* 执行语句 */

  /* Check if CAN state is not busy for RX FIFO0 */
  if ((FIFONumber == CAN_FIFO0) && ((hcan->State == HAL_CAN_STATE_BUSY_RX0) ||         \
                                    (hcan->State == HAL_CAN_STATE_BUSY_TX_RX0) ||      \
                                    (hcan->State == HAL_CAN_STATE_BUSY_RX0_RX1) ||     \
                                    (hcan->State == HAL_CAN_STATE_BUSY_TX_RX0_RX1)))  /* 源代码 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check if CAN state is not busy for RX FIFO1 */
  if ((FIFONumber == CAN_FIFO1) && ((hcan->State == HAL_CAN_STATE_BUSY_RX1) ||         \
                                    (hcan->State == HAL_CAN_STATE_BUSY_TX_RX1) ||      \
                                    (hcan->State == HAL_CAN_STATE_BUSY_RX0_RX1) ||     \
                                    (hcan->State == HAL_CAN_STATE_BUSY_TX_RX0_RX1)))  /* 源代码 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Process locked */
  __HAL_LOCK(hcan);                                     /* HAL辅助宏/函数 */

  /* Change CAN state */
  if (FIFONumber == CAN_FIFO0)                          /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch(hcan->State)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case(HAL_CAN_STATE_BUSY_TX):                      /* 标签 */
        hcan->State = HAL_CAN_STATE_BUSY_TX_RX0;        /* 赋值或初始化 */
        break;                                          /* 跳出 */
      case(HAL_CAN_STATE_BUSY_RX1):                     /* 标签 */
        hcan->State = HAL_CAN_STATE_BUSY_RX0_RX1;       /* 赋值或初始化 */
        break;                                          /* 跳出 */
      case(HAL_CAN_STATE_BUSY_TX_RX1):                  /* 标签 */
        hcan->State = HAL_CAN_STATE_BUSY_TX_RX0_RX1;    /* 赋值或初始化 */
        break;                                          /* 跳出 */
      default: /* HAL_CAN_STATE_READY */
        hcan->State = HAL_CAN_STATE_BUSY_RX0;           /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else /* FIFONumber == CAN_FIFO1 */
  {                                                     /* 代码块开始 */
    switch(hcan->State)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case(HAL_CAN_STATE_BUSY_TX):                      /* 标签 */
        hcan->State = HAL_CAN_STATE_BUSY_TX_RX1;        /* 赋值或初始化 */
        break;                                          /* 跳出 */
      case(HAL_CAN_STATE_BUSY_RX0):                     /* 标签 */
        hcan->State = HAL_CAN_STATE_BUSY_RX0_RX1;       /* 赋值或初始化 */
        break;                                          /* 跳出 */
      case(HAL_CAN_STATE_BUSY_TX_RX0):                  /* 标签 */
        hcan->State = HAL_CAN_STATE_BUSY_TX_RX0_RX1;    /* 赋值或初始化 */
        break;                                          /* 跳出 */
      default: /* HAL_CAN_STATE_READY */
        hcan->State = HAL_CAN_STATE_BUSY_RX1;           /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Get tick */ 
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  /* Check pending message */
  while(__HAL_CAN_MSG_PENDING(hcan, FIFONumber) == 0U)  /* while循环 */
  {                                                     /* 代码块开始 */
    /* Check for the Timeout */
    if(Timeout != HAL_MAX_DELAY)                        /* 条件判断 */
    {                                                   /* 代码块开始 */
      if((Timeout == 0U)||((HAL_GetTick() - tickstart ) > Timeout))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        hcan->State = HAL_CAN_STATE_TIMEOUT;            /* 赋值或初始化 */
        /* Process unlocked */
        __HAL_UNLOCK(hcan);                             /* HAL辅助宏/函数 */
        return HAL_TIMEOUT;                             /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Set RxMsg pointer */
  if(FIFONumber == CAN_FIFO0)                           /* 条件判断 */
  {                                                     /* 代码块开始 */
    pRxMsg = hcan->pRxMsg;                              /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else /* FIFONumber == CAN_FIFO1 */
  {                                                     /* 代码块开始 */
    pRxMsg = hcan->pRx1Msg;                             /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Get the Id */
  pRxMsg->IDE = (uint8_t)0x04 & hcan->Instance->sFIFOMailBox[FIFONumber].RIR;  /* 赋值或初始化 */
  if (pRxMsg->IDE == CAN_ID_STD)                        /* 条件判断 */
  {                                                     /* 代码块开始 */
    pRxMsg->StdId = 0x000007FFU & (hcan->Instance->sFIFOMailBox[FIFONumber].RIR >> 21U);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    pRxMsg->ExtId = 0x1FFFFFFFU & (hcan->Instance->sFIFOMailBox[FIFONumber].RIR >> 3U);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  pRxMsg->RTR = (uint8_t)0x02 & hcan->Instance->sFIFOMailBox[FIFONumber].RIR;  /* 赋值或初始化 */
  /* Get the DLC */
  pRxMsg->DLC = (uint8_t)0x0F & hcan->Instance->sFIFOMailBox[FIFONumber].RDTR;  /* 赋值或初始化 */
  /* Get the FMI */
  pRxMsg->FMI = (uint8_t)0xFF & (hcan->Instance->sFIFOMailBox[FIFONumber].RDTR >> 8U);  /* 赋值或初始化 */
  /* Get the FIFONumber */
  pRxMsg->FIFONumber = FIFONumber;                      /* 赋值或初始化 */
  /* Get the data field */
  pRxMsg->Data[0] = (uint8_t)0xFF & hcan->Instance->sFIFOMailBox[FIFONumber].RDLR;  /* 赋值或初始化 */
  pRxMsg->Data[1] = (uint8_t)0xFF & (hcan->Instance->sFIFOMailBox[FIFONumber].RDLR >> 8U);  /* 赋值或初始化 */
  pRxMsg->Data[2] = (uint8_t)0xFF & (hcan->Instance->sFIFOMailBox[FIFONumber].RDLR >> 16U);  /* 赋值或初始化 */
  pRxMsg->Data[3] = (uint8_t)0xFF & (hcan->Instance->sFIFOMailBox[FIFONumber].RDLR >> 24U);  /* 赋值或初始化 */
  pRxMsg->Data[4] = (uint8_t)0xFF & hcan->Instance->sFIFOMailBox[FIFONumber].RDHR;  /* 赋值或初始化 */
  pRxMsg->Data[5] = (uint8_t)0xFF & (hcan->Instance->sFIFOMailBox[FIFONumber].RDHR >> 8U);  /* 赋值或初始化 */
  pRxMsg->Data[6] = (uint8_t)0xFF & (hcan->Instance->sFIFOMailBox[FIFONumber].RDHR >> 16U);  /* 赋值或初始化 */
  pRxMsg->Data[7] = (uint8_t)0xFF & (hcan->Instance->sFIFOMailBox[FIFONumber].RDHR >> 24U);  /* 赋值或初始化 */

  /* Release the FIFO */
  if(FIFONumber == CAN_FIFO0)                           /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Release FIFO0 */
    __HAL_CAN_FIFO_RELEASE(hcan, CAN_FIFO0);            /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  else /* FIFONumber == CAN_FIFO1 */
  {                                                     /* 代码块开始 */
    /* Release FIFO1 */
    __HAL_CAN_FIFO_RELEASE(hcan, CAN_FIFO1);            /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Change CAN state */
  if (FIFONumber == CAN_FIFO0)                          /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch(hcan->State)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case(HAL_CAN_STATE_BUSY_TX_RX0):                  /* 标签 */
        hcan->State = HAL_CAN_STATE_BUSY_TX;            /* 赋值或初始化 */
        break;                                          /* 跳出 */
      case(HAL_CAN_STATE_BUSY_RX0_RX1):                 /* 标签 */
        hcan->State = HAL_CAN_STATE_BUSY_RX1;           /* 赋值或初始化 */
        break;                                          /* 跳出 */
      case(HAL_CAN_STATE_BUSY_TX_RX0_RX1):              /* 标签 */
        hcan->State = HAL_CAN_STATE_BUSY_TX_RX1;        /* 赋值或初始化 */
        break;                                          /* 跳出 */
      default: /* HAL_CAN_STATE_BUSY_RX0 */
        hcan->State = HAL_CAN_STATE_READY;              /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else /* FIFONumber == CAN_FIFO1 */
  {                                                     /* 代码块开始 */
    switch(hcan->State)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case(HAL_CAN_STATE_BUSY_TX_RX1):                  /* 标签 */
        hcan->State = HAL_CAN_STATE_BUSY_TX;            /* 赋值或初始化 */
        break;                                          /* 跳出 */
      case(HAL_CAN_STATE_BUSY_RX0_RX1):                 /* 标签 */
        hcan->State = HAL_CAN_STATE_BUSY_RX0;           /* 赋值或初始化 */
        break;                                          /* 跳出 */
      case(HAL_CAN_STATE_BUSY_TX_RX0_RX1):              /* 标签 */
        hcan->State = HAL_CAN_STATE_BUSY_TX_RX0;        /* 赋值或初始化 */
        break;                                          /* 跳出 */
      default: /* HAL_CAN_STATE_BUSY_RX1 */
        hcan->State = HAL_CAN_STATE_READY;              /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Process unlocked */
  __HAL_UNLOCK(hcan);                                   /* HAL辅助宏/函数 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Receives a correct CAN frame.
  * @param  hcan       Pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.  
  * @param  FIFONumber Specify the FIFO number    
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_CAN_Receive_IT(CAN_HandleTypeDef* hcan, uint8_t FIFONumber)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the parameters */
  assert_param(IS_CAN_FIFO(FIFONumber));                /* 执行语句 */

  /* Check if CAN state is not busy for RX FIFO0 */
  if((FIFONumber == CAN_FIFO0) && ((hcan->State == HAL_CAN_STATE_BUSY_RX0) ||         \
                                   (hcan->State == HAL_CAN_STATE_BUSY_TX_RX0) ||      \
                                   (hcan->State == HAL_CAN_STATE_BUSY_RX0_RX1) ||     \
                                   (hcan->State == HAL_CAN_STATE_BUSY_TX_RX0_RX1)))  /* 源代码 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check if CAN state is not busy for RX FIFO1 */
  if((FIFONumber == CAN_FIFO1) && ((hcan->State == HAL_CAN_STATE_BUSY_RX1) ||         \
                                   (hcan->State == HAL_CAN_STATE_BUSY_TX_RX1) ||      \
                                   (hcan->State == HAL_CAN_STATE_BUSY_RX0_RX1) ||     \
                                   (hcan->State == HAL_CAN_STATE_BUSY_TX_RX0_RX1)))  /* 源代码 */
  {                                                     /* 代码块开始 */
    return HAL_BUSY;                                    /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Process locked */
  __HAL_LOCK(hcan);                                     /* HAL辅助宏/函数 */

  /* Change CAN state */
  if(FIFONumber == CAN_FIFO0)                           /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch(hcan->State)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case(HAL_CAN_STATE_BUSY_TX):                      /* 标签 */
        hcan->State = HAL_CAN_STATE_BUSY_TX_RX0;        /* 赋值或初始化 */
        break;                                          /* 跳出 */
      case(HAL_CAN_STATE_BUSY_RX1):                     /* 标签 */
        hcan->State = HAL_CAN_STATE_BUSY_RX0_RX1;       /* 赋值或初始化 */
        break;                                          /* 跳出 */
      case(HAL_CAN_STATE_BUSY_TX_RX1):                  /* 标签 */
        hcan->State = HAL_CAN_STATE_BUSY_TX_RX0_RX1;    /* 赋值或初始化 */
        break;                                          /* 跳出 */
      default: /* HAL_CAN_STATE_READY */
        hcan->State = HAL_CAN_STATE_BUSY_RX0;           /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else /* FIFONumber == CAN_FIFO1 */
  {                                                     /* 代码块开始 */
    switch(hcan->State)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case(HAL_CAN_STATE_BUSY_TX):                      /* 标签 */
        hcan->State = HAL_CAN_STATE_BUSY_TX_RX1;        /* 赋值或初始化 */
        break;                                          /* 跳出 */
      case(HAL_CAN_STATE_BUSY_RX0):                     /* 标签 */
        hcan->State = HAL_CAN_STATE_BUSY_RX0_RX1;       /* 赋值或初始化 */
        break;                                          /* 跳出 */
      case(HAL_CAN_STATE_BUSY_TX_RX0):                  /* 标签 */
        hcan->State = HAL_CAN_STATE_BUSY_TX_RX0_RX1;    /* 赋值或初始化 */
        break;                                          /* 跳出 */
      default: /* HAL_CAN_STATE_READY */
        hcan->State = HAL_CAN_STATE_BUSY_RX1;           /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /* Set CAN error code to none */
  hcan->ErrorCode = HAL_CAN_ERROR_NONE;                 /* 赋值或初始化 */

  /* Enable interrupts: */
  /*  - Enable Error warning Interrupt */
  /*  - Enable Error passive Interrupt */
  /*  - Enable Bus-off Interrupt */
  /*  - Enable Last error code Interrupt */
  /*  - Enable Error Interrupt */
  /*  - Enable Transmit mailbox empty Interrupt */
  __HAL_CAN_ENABLE_IT(hcan, CAN_IT_EWG |                /* HAL辅助宏/函数 */
                            CAN_IT_EPV |                /* 源代码 */
                            CAN_IT_BOF |                /* 源代码 */
                            CAN_IT_LEC |                /* 源代码 */
                            CAN_IT_ERR |                /* 源代码 */
                            CAN_IT_TME);                /* 执行语句 */

  /* Process unlocked */
   __HAL_UNLOCK(hcan);                                  /* HAL辅助宏/函数 */

  if(FIFONumber == CAN_FIFO0)                           /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable FIFO 0 overrun and message pending Interrupt */
    __HAL_CAN_ENABLE_IT(hcan, CAN_IT_FOV0 | CAN_IT_FMP0);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Enable FIFO 1 overrun and message pending Interrupt */
    __HAL_CAN_ENABLE_IT(hcan, CAN_IT_FOV1 | CAN_IT_FMP1);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Enters the Sleep (low power) mode.
  * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval HAL status.
  */
HAL_StatusTypeDef HAL_CAN_Sleep(CAN_HandleTypeDef* hcan)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart = 0U;                              /* 32位无符号变量/函数 */

  /* Process locked */
  __HAL_LOCK(hcan);                                     /* HAL辅助宏/函数 */

  /* Change CAN state */
  hcan->State = HAL_CAN_STATE_BUSY;                     /* 赋值或初始化 */

  /* Request Sleep mode */
   hcan->Instance->MCR = (((hcan->Instance->MCR) & (uint32_t)(~(uint32_t)CAN_MCR_INRQ)) | CAN_MCR_SLEEP);  /* 赋值或初始化 */

  /* Sleep mode status */
  if ((hcan->Instance->MSR & (CAN_MSR_SLAK|CAN_MSR_INAK)) != CAN_MSR_SLAK)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Process unlocked */
    __HAL_UNLOCK(hcan);                                 /* HAL辅助宏/函数 */

    /* Return function status */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Get tick */
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  /* Wait the acknowledge */
  while((hcan->Instance->MSR & (CAN_MSR_SLAK|CAN_MSR_INAK)) != CAN_MSR_SLAK)  /* while循环 */
  {                                                     /* 代码块开始 */
    if((HAL_GetTick()  - tickstart) > CAN_TIMEOUT_VALUE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hcan->State = HAL_CAN_STATE_TIMEOUT;              /* 赋值或初始化 */
      /* Process unlocked */
      __HAL_UNLOCK(hcan);                               /* HAL辅助宏/函数 */
      return HAL_TIMEOUT;                               /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Change CAN state */
  hcan->State = HAL_CAN_STATE_READY;                    /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hcan);                                   /* HAL辅助宏/函数 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Wakes up the CAN peripheral from sleep mode, after that the CAN peripheral
  *         is in the normal mode.
  * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval HAL status.
  */
HAL_StatusTypeDef HAL_CAN_WakeUp(CAN_HandleTypeDef* hcan)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart = 0U;                              /* 32位无符号变量/函数 */

  /* Process locked */
  __HAL_LOCK(hcan);                                     /* HAL辅助宏/函数 */

  /* Change CAN state */
  hcan->State = HAL_CAN_STATE_BUSY;                     /* 赋值或初始化 */

  /* Wake up request */
  hcan->Instance->MCR &= ~(uint32_t)CAN_MCR_SLEEP;      /* 赋值或初始化 */

  /* Get tick */ 
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  /* Sleep mode status */
  while((hcan->Instance->MSR & CAN_MSR_SLAK) == CAN_MSR_SLAK)  /* while循环 */
  {                                                     /* 代码块开始 */
    if((HAL_GetTick()  - tickstart) > CAN_TIMEOUT_VALUE)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hcan->State= HAL_CAN_STATE_TIMEOUT;               /* 赋值或初始化 */
      /* Process unlocked */
      __HAL_UNLOCK(hcan);                               /* HAL辅助宏/函数 */
      return HAL_TIMEOUT;                               /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  if((hcan->Instance->MSR & CAN_MSR_SLAK) == CAN_MSR_SLAK)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Process unlocked */
    __HAL_UNLOCK(hcan);                                 /* HAL辅助宏/函数 */

    /* Return function status */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Change CAN state */
  hcan->State = HAL_CAN_STATE_READY;                    /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hcan);                                   /* HAL辅助宏/函数 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Handles CAN interrupt request  
  * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval None
  */
void HAL_CAN_IRQHandler(CAN_HandleTypeDef* hcan)        /* 中断服务函数 */
{                                                       /* 代码块开始 */
  uint32_t tmp1 = 0U, tmp2 = 0U, tmp3 = 0U;             /* 32位无符号变量/函数 */
  uint32_t errorcode = HAL_CAN_ERROR_NONE;              /* 32位无符号变量/函数 */

  /* Check Overrun flag for FIFO0 */
  tmp1 = __HAL_CAN_GET_FLAG(hcan, CAN_FLAG_FOV0);       /* 赋值或初始化 */
  tmp2 = __HAL_CAN_GET_IT_SOURCE(hcan, CAN_IT_FOV0);    /* 赋值或初始化 */
  if(tmp1 && tmp2)                                      /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Set CAN error code to FOV0 error */
    errorcode |= HAL_CAN_ERROR_FOV0;                    /* 赋值或初始化 */

    /* Clear FIFO0 Overrun Flag */
    __HAL_CAN_CLEAR_FLAG(hcan, CAN_FLAG_FOV0);          /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  /* Check Overrun flag for FIFO1 */
  tmp1 = __HAL_CAN_GET_FLAG(hcan, CAN_FLAG_FOV1);       /* 赋值或初始化 */
  tmp2 = __HAL_CAN_GET_IT_SOURCE(hcan, CAN_IT_FOV1);    /* 赋值或初始化 */

  if(tmp1 && tmp2)                                      /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Set CAN error code to FOV1 error */
    errorcode |= HAL_CAN_ERROR_FOV1;                    /* 赋值或初始化 */

    /* Clear FIFO1 Overrun Flag */
    __HAL_CAN_CLEAR_FLAG(hcan, CAN_FLAG_FOV1);          /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Check End of transmission flag */
  if(__HAL_CAN_GET_IT_SOURCE(hcan, CAN_IT_TME))         /* 条件判断 */
  {                                                     /* 代码块开始 */
    tmp1 = __HAL_CAN_TRANSMIT_STATUS(hcan, CAN_TXMAILBOX_0);  /* 赋值或初始化 */
    tmp2 = __HAL_CAN_TRANSMIT_STATUS(hcan, CAN_TXMAILBOX_1);  /* 赋值或初始化 */
    tmp3 = __HAL_CAN_TRANSMIT_STATUS(hcan, CAN_TXMAILBOX_2);  /* 赋值或初始化 */
    if(tmp1 || tmp2 || tmp3)                            /* 条件判断 */
    {                                                   /* 代码块开始 */
      tmp1 = __HAL_CAN_GET_FLAG(hcan, CAN_FLAG_TXOK0);  /* 赋值或初始化 */
      tmp2 = __HAL_CAN_GET_FLAG(hcan, CAN_FLAG_TXOK1);  /* 赋值或初始化 */
      tmp3 = __HAL_CAN_GET_FLAG(hcan, CAN_FLAG_TXOK2);  /* 赋值或初始化 */
      /* Check Transmit success */
      if(tmp1 || tmp2 || tmp3)                          /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Call transmit function */
        CAN_Transmit_IT(hcan);                          /* 执行语句 */
      }                                                 /* 代码块结束 */
      else /* Transmit failure */
      {                                                 /* 代码块开始 */
        /* Set CAN error code to TXFAIL error */
        errorcode |= HAL_CAN_ERROR_TXFAIL;              /* 赋值或初始化 */
      }                                                 /* 代码块结束 */

      /* Clear transmission status flags (RQCPx and TXOKx) */
      SET_BIT(hcan->Instance->TSR, CAN_TSR_RQCP0  | CAN_TSR_RQCP1  | CAN_TSR_RQCP2 | \
                                   CAN_FLAG_TXOK0 | CAN_FLAG_TXOK1 | CAN_FLAG_TXOK2);  /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  tmp1 = __HAL_CAN_MSG_PENDING(hcan, CAN_FIFO0);        /* 赋值或初始化 */
  tmp2 = __HAL_CAN_GET_IT_SOURCE(hcan, CAN_IT_FMP0);    /* 赋值或初始化 */
  /* Check End of reception flag for FIFO0 */
  if((tmp1 != 0U) && tmp2)                              /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Call receive function */
    CAN_Receive_IT(hcan, CAN_FIFO0);                    /* 执行语句 */
  }                                                     /* 代码块结束 */

  tmp1 = __HAL_CAN_MSG_PENDING(hcan, CAN_FIFO1);        /* 赋值或初始化 */
  tmp2 = __HAL_CAN_GET_IT_SOURCE(hcan, CAN_IT_FMP1);    /* 赋值或初始化 */
  /* Check End of reception flag for FIFO1 */
  if((tmp1 != 0U) && tmp2)                              /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Call receive function */
    CAN_Receive_IT(hcan, CAN_FIFO1);                    /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Set error code in handle */
  hcan->ErrorCode |= errorcode;                         /* 赋值或初始化 */

  tmp1 = __HAL_CAN_GET_FLAG(hcan, CAN_FLAG_EWG);        /* 赋值或初始化 */
  tmp2 = __HAL_CAN_GET_IT_SOURCE(hcan, CAN_IT_EWG);     /* 赋值或初始化 */
  tmp3 = __HAL_CAN_GET_IT_SOURCE(hcan, CAN_IT_ERR);     /* 赋值或初始化 */
  /* Check Error Warning Flag */
  if(tmp1 && tmp2 && tmp3)                              /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Set CAN error code to EWG error */
    hcan->ErrorCode |= HAL_CAN_ERROR_EWG;               /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  tmp1 = __HAL_CAN_GET_FLAG(hcan, CAN_FLAG_EPV);        /* 赋值或初始化 */
  tmp2 = __HAL_CAN_GET_IT_SOURCE(hcan, CAN_IT_EPV);     /* 赋值或初始化 */
  tmp3 = __HAL_CAN_GET_IT_SOURCE(hcan, CAN_IT_ERR);     /* 赋值或初始化 */
  /* Check Error Passive Flag */
  if(tmp1 && tmp2 && tmp3)                              /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Set CAN error code to EPV error */
    hcan->ErrorCode |= HAL_CAN_ERROR_EPV;               /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  tmp1 = __HAL_CAN_GET_FLAG(hcan, CAN_FLAG_BOF);        /* 赋值或初始化 */
  tmp2 = __HAL_CAN_GET_IT_SOURCE(hcan, CAN_IT_BOF);     /* 赋值或初始化 */
  tmp3 = __HAL_CAN_GET_IT_SOURCE(hcan, CAN_IT_ERR);     /* 赋值或初始化 */
  /* Check Bus-Off Flag */
  if(tmp1 && tmp2 && tmp3)                              /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Set CAN error code to BOF error */
    hcan->ErrorCode |= HAL_CAN_ERROR_BOF;               /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  tmp1 = HAL_IS_BIT_CLR(hcan->Instance->ESR, CAN_ESR_LEC);  /* 赋值或初始化 */
  tmp2 = __HAL_CAN_GET_IT_SOURCE(hcan, CAN_IT_LEC);     /* 赋值或初始化 */
  tmp3 = __HAL_CAN_GET_IT_SOURCE(hcan, CAN_IT_ERR);     /* 赋值或初始化 */
  /* Check Last error code Flag */
  if((!tmp1) && tmp2 && tmp3)                           /* 条件判断 */
  {                                                     /* 代码块开始 */
    tmp1 = (hcan->Instance->ESR) & CAN_ESR_LEC;         /* 赋值或初始化 */
    switch(tmp1)                                        /* switch分支 */
    {                                                   /* 代码块开始 */
      case(CAN_ESR_LEC_0):                              /* 标签 */
          /* Set CAN error code to STF error */
          hcan->ErrorCode |= HAL_CAN_ERROR_STF;         /* 赋值或初始化 */
          break;                                        /* 跳出 */
      case(CAN_ESR_LEC_1):                              /* 标签 */
          /* Set CAN error code to FOR error */
          hcan->ErrorCode |= HAL_CAN_ERROR_FOR;         /* 赋值或初始化 */
          break;                                        /* 跳出 */
      case(CAN_ESR_LEC_1 | CAN_ESR_LEC_0):              /* 标签 */
          /* Set CAN error code to ACK error */
          hcan->ErrorCode |= HAL_CAN_ERROR_ACK;         /* 赋值或初始化 */
          break;                                        /* 跳出 */
      case(CAN_ESR_LEC_2):                              /* 标签 */
          /* Set CAN error code to BR error */
          hcan->ErrorCode |= HAL_CAN_ERROR_BR;          /* 赋值或初始化 */
          break;                                        /* 跳出 */
      case(CAN_ESR_LEC_2 | CAN_ESR_LEC_0):              /* 标签 */
          /* Set CAN error code to BD error */
          hcan->ErrorCode |= HAL_CAN_ERROR_BD;          /* 赋值或初始化 */
          break;                                        /* 跳出 */
      case(CAN_ESR_LEC_2 | CAN_ESR_LEC_1):              /* 标签 */
          /* Set CAN error code to CRC error */
          hcan->ErrorCode |= HAL_CAN_ERROR_CRC;         /* 赋值或初始化 */
          break;                                        /* 跳出 */
      default:                                          /* default分支 */
          break;                                        /* 跳出 */
    }                                                   /* 代码块结束 */

    /* Clear Last error code Flag */ 
    hcan->Instance->ESR &= ~(CAN_ESR_LEC);              /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Call the Error call Back in case of Errors */
  if(hcan->ErrorCode != HAL_CAN_ERROR_NONE)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Clear ERRI Flag */ 
    hcan->Instance->MSR = CAN_MSR_ERRI;                 /* 赋值或初始化 */
    /* Set the CAN state ready to be able to start again the process */
    hcan->State = HAL_CAN_STATE_READY;                  /* 赋值或初始化 */

    /* Disable interrupts: */
    /*  - Disable Error warning Interrupt */
    /*  - Disable Error passive Interrupt */
    /*  - Disable Bus-off Interrupt */
    /*  - Disable Last error code Interrupt */
    /*  - Disable Error Interrupt */
    /*  - Disable FIFO 0 message pending Interrupt */
    /*  - Disable FIFO 0 Overrun Interrupt */
    /*  - Disable FIFO 1 message pending Interrupt */
    /*  - Disable FIFO 1 Overrun Interrupt */
    /*  - Disable Transmit mailbox empty Interrupt */
    __HAL_CAN_DISABLE_IT(hcan, CAN_IT_EWG |             /* HAL辅助宏/函数 */
                               CAN_IT_EPV |             /* 源代码 */
                               CAN_IT_BOF |             /* 源代码 */
                               CAN_IT_LEC |             /* 源代码 */
                               CAN_IT_ERR |             /* 源代码 */
                               CAN_IT_FMP0|             /* 源代码 */
                               CAN_IT_FOV0|             /* 源代码 */
                               CAN_IT_FMP1|             /* 源代码 */
                               CAN_IT_FOV1|             /* 源代码 */
                               CAN_IT_TME);             /* 执行语句 */

    /* Call Error callback function */
    HAL_CAN_ErrorCallback(hcan);                        /* HAL库函数调用 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Transmission  complete callback in non blocking mode 
  * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval None
  */
__weak void HAL_CAN_TxCpltCallback(CAN_HandleTypeDef* hcan)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hcan);                                         /* 执行语句 */
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_CAN_TxCpltCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Transmission  complete callback in non blocking mode 
  * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval None
  */
__weak void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* hcan)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hcan);                                         /* 执行语句 */
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_CAN_RxCpltCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Error CAN callback.
  * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval None
  */
__weak void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hcan);                                         /* 执行语句 */
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_CAN_ErrorCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup CAN_Exported_Functions_Group3 Peripheral State and Error functions
  *  @brief   CAN Peripheral State functions 
  *
@verbatim   
  ==============================================================================
            ##### Peripheral State and Error functions #####
  ==============================================================================
    [..]
    This subsection provides functions allowing to :
      (+) Check the CAN state.
      (+) Check CAN Errors detected during interrupt process

@endverbatim
  * @{
  */

/**
  * @brief  return the CAN state
  * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval HAL state
  */
HAL_CAN_StateTypeDef HAL_CAN_GetState(CAN_HandleTypeDef* hcan)  /* HAL库函数调用 */
{                                                       /* 代码块开始 */
  /* Return CAN state */
  return hcan->State;                                   /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return the CAN error code
  * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval CAN Error Code
  */
uint32_t HAL_CAN_GetError(CAN_HandleTypeDef *hcan)      /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  return hcan->ErrorCode;                               /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */
/**
  * @brief  Initiates and transmits a CAN frame message.
  * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.  
  * @retval HAL status
  */
static HAL_StatusTypeDef CAN_Transmit_IT(CAN_HandleTypeDef* hcan)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  /* Disable Transmit mailbox empty Interrupt */
  __HAL_CAN_DISABLE_IT(hcan, CAN_IT_TME);               /* HAL辅助宏/函数 */

  if(hcan->State == HAL_CAN_STATE_BUSY_TX)              /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable Error warning, Error passive, Bus-off, Last error code
       and Error Interrupts */
    __HAL_CAN_DISABLE_IT(hcan, CAN_IT_EWG |             /* HAL辅助宏/函数 */
                               CAN_IT_EPV |             /* 源代码 */
                               CAN_IT_BOF |             /* 源代码 */
                               CAN_IT_LEC |             /* 源代码 */
                               CAN_IT_ERR );            /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Change CAN state */
  switch(hcan->State)                                   /* switch分支 */
  {                                                     /* 代码块开始 */
    case(HAL_CAN_STATE_BUSY_TX_RX0):                    /* 标签 */
      hcan->State = HAL_CAN_STATE_BUSY_RX0;             /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case(HAL_CAN_STATE_BUSY_TX_RX1):                    /* 标签 */
      hcan->State = HAL_CAN_STATE_BUSY_RX1;             /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case(HAL_CAN_STATE_BUSY_TX_RX0_RX1):                /* 标签 */
      hcan->State = HAL_CAN_STATE_BUSY_RX0_RX1;         /* 赋值或初始化 */
      break;                                            /* 跳出 */
    default: /* HAL_CAN_STATE_BUSY_TX */
      hcan->State = HAL_CAN_STATE_READY;                /* 赋值或初始化 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  /* Transmission complete callback */ 
  HAL_CAN_TxCpltCallback(hcan);                         /* HAL库函数调用 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Receives a correct CAN frame.
  * @param  hcan       Pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.  
  * @param  FIFONumber Specify the FIFO number    
  * @retval HAL status
  * @retval None
  */
static HAL_StatusTypeDef CAN_Receive_IT(CAN_HandleTypeDef* hcan, uint8_t FIFONumber)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t tmp1 = 0U;                                   /* 32位无符号变量/函数 */
  CanRxMsgTypeDef* pRxMsg = NULL;                       /* 赋值或初始化 */

  /* Set RxMsg pointer */
  if(FIFONumber == CAN_FIFO0)                           /* 条件判断 */
  {                                                     /* 代码块开始 */
    pRxMsg = hcan->pRxMsg;                              /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else /* FIFONumber == CAN_FIFO1 */
  {                                                     /* 代码块开始 */
    pRxMsg = hcan->pRx1Msg;                             /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Get the Id */
  pRxMsg->IDE = (uint8_t)0x04 & hcan->Instance->sFIFOMailBox[FIFONumber].RIR;  /* 赋值或初始化 */
  if (pRxMsg->IDE == CAN_ID_STD)                        /* 条件判断 */
  {                                                     /* 代码块开始 */
    pRxMsg->StdId = 0x000007FFU & (hcan->Instance->sFIFOMailBox[FIFONumber].RIR >> 21U);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    pRxMsg->ExtId = 0x1FFFFFFFU & (hcan->Instance->sFIFOMailBox[FIFONumber].RIR >> 3U);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  pRxMsg->RTR = (uint8_t)0x02 & hcan->Instance->sFIFOMailBox[FIFONumber].RIR;  /* 赋值或初始化 */
  /* Get the DLC */
  pRxMsg->DLC = (uint8_t)0x0F & hcan->Instance->sFIFOMailBox[FIFONumber].RDTR;  /* 赋值或初始化 */
  /* Get the FIFONumber */
  pRxMsg->FIFONumber = FIFONumber;                      /* 赋值或初始化 */
  /* Get the FMI */
  pRxMsg->FMI = (uint8_t)0xFF & (hcan->Instance->sFIFOMailBox[FIFONumber].RDTR >> 8U);  /* 赋值或初始化 */
  /* Get the data field */
  pRxMsg->Data[0] = (uint8_t)0xFF & hcan->Instance->sFIFOMailBox[FIFONumber].RDLR;  /* 赋值或初始化 */
  pRxMsg->Data[1] = (uint8_t)0xFF & (hcan->Instance->sFIFOMailBox[FIFONumber].RDLR >> 8U);  /* 赋值或初始化 */
  pRxMsg->Data[2] = (uint8_t)0xFF & (hcan->Instance->sFIFOMailBox[FIFONumber].RDLR >> 16U);  /* 赋值或初始化 */
  pRxMsg->Data[3] = (uint8_t)0xFF & (hcan->Instance->sFIFOMailBox[FIFONumber].RDLR >> 24U);  /* 赋值或初始化 */
  pRxMsg->Data[4] = (uint8_t)0xFF & hcan->Instance->sFIFOMailBox[FIFONumber].RDHR;  /* 赋值或初始化 */
  pRxMsg->Data[5] = (uint8_t)0xFF & (hcan->Instance->sFIFOMailBox[FIFONumber].RDHR >> 8U);  /* 赋值或初始化 */
  pRxMsg->Data[6] = (uint8_t)0xFF & (hcan->Instance->sFIFOMailBox[FIFONumber].RDHR >> 16U);  /* 赋值或初始化 */
  pRxMsg->Data[7] = (uint8_t)0xFF & (hcan->Instance->sFIFOMailBox[FIFONumber].RDHR >> 24U);  /* 赋值或初始化 */
  /* Release the FIFO */
  /* Release FIFO0 */
  if (FIFONumber == CAN_FIFO0)                          /* 条件判断 */
  {                                                     /* 代码块开始 */
    __HAL_CAN_FIFO_RELEASE(hcan, CAN_FIFO0);            /* HAL辅助宏/函数 */

    /* Disable FIFO 0 overrun and message pending Interrupt */
    __HAL_CAN_DISABLE_IT(hcan, CAN_IT_FOV0 | CAN_IT_FMP0);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  /* Release FIFO1 */
  else /* FIFONumber == CAN_FIFO1 */
  {                                                     /* 代码块开始 */
    __HAL_CAN_FIFO_RELEASE(hcan, CAN_FIFO1);            /* HAL辅助宏/函数 */

    /* Disable FIFO 1 overrun and message pending Interrupt */
    __HAL_CAN_DISABLE_IT(hcan, CAN_IT_FOV1 | CAN_IT_FMP1);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  tmp1 = hcan->State;                                   /* 赋值或初始化 */
  if((tmp1 == HAL_CAN_STATE_BUSY_RX0) || (tmp1 == HAL_CAN_STATE_BUSY_RX1))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable Error warning, Error passive, Bus-off, Last error code
       and Error Interrupts */
    __HAL_CAN_DISABLE_IT(hcan, CAN_IT_EWG |             /* HAL辅助宏/函数 */
                               CAN_IT_EPV |             /* 源代码 */
                               CAN_IT_BOF |             /* 源代码 */
                               CAN_IT_LEC |             /* 源代码 */
                               CAN_IT_ERR);             /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Change CAN state */
  if (FIFONumber == CAN_FIFO0)                          /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch(hcan->State)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case(HAL_CAN_STATE_BUSY_TX_RX0):                  /* 标签 */
        hcan->State = HAL_CAN_STATE_BUSY_TX;            /* 赋值或初始化 */
        break;                                          /* 跳出 */
      case(HAL_CAN_STATE_BUSY_RX0_RX1):                 /* 标签 */
        hcan->State = HAL_CAN_STATE_BUSY_RX1;           /* 赋值或初始化 */
        break;                                          /* 跳出 */
      case(HAL_CAN_STATE_BUSY_TX_RX0_RX1):              /* 标签 */
        hcan->State = HAL_CAN_STATE_BUSY_TX_RX1;        /* 赋值或初始化 */
        break;                                          /* 跳出 */
      default: /* HAL_CAN_STATE_BUSY_RX0 */
        hcan->State = HAL_CAN_STATE_READY;              /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else /* FIFONumber == CAN_FIFO1 */
  {                                                     /* 代码块开始 */
    switch(hcan->State)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case(HAL_CAN_STATE_BUSY_TX_RX1):                  /* 标签 */
        hcan->State = HAL_CAN_STATE_BUSY_TX;            /* 赋值或初始化 */
        break;                                          /* 跳出 */
      case(HAL_CAN_STATE_BUSY_RX0_RX1):                 /* 标签 */
        hcan->State = HAL_CAN_STATE_BUSY_RX0;           /* 赋值或初始化 */
        break;                                          /* 跳出 */
      case(HAL_CAN_STATE_BUSY_TX_RX0_RX1):              /* 标签 */
        hcan->State = HAL_CAN_STATE_BUSY_TX_RX0;        /* 赋值或初始化 */
        break;                                          /* 跳出 */
      default: /* HAL_CAN_STATE_BUSY_RX1 */
        hcan->State = HAL_CAN_STATE_READY;              /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Receive complete callback */ 
  HAL_CAN_RxCpltCallback(hcan);                         /* HAL库函数调用 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */
#endif /* STM32F405xx || STM32F415xx || STM32F407xx || STM32F417xx || STM32F427xx || STM32F437xx ||\
          STM32F429xx || STM32F439xx || STM32F446xx || STM32F469xx || STM32F479xx || STM32F412Zx ||\
          STM32F412Vx || STM32F412Rx || STM32F412Cx || STM32F413xx || STM32F423xx */

#endif /* HAL_CAN_LEGACY_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
