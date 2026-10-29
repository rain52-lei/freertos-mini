/* 文件: stm32f4xx_hal_pcd.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_pcd.c
  * @author  MCD Application Team
  * @brief   PCD HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the USB Peripheral Controller:
  *           + Initialization and de-initialization functions
  *           + IO operation functions
  *           + Peripheral Control functions
  *           + Peripheral State functions
  *
  @verbatim
  ==============================================================================
                    ##### How to use this driver #####
  ==============================================================================
    [..]
      The PCD HAL driver can be used as follows:

     (#) Declare a PCD_HandleTypeDef handle structure, for example:
         PCD_HandleTypeDef  hpcd;

     (#) Fill parameters of Init structure in HCD handle

     (#) Call HAL_PCD_Init() API to initialize the PCD peripheral (Core, Device core, ...)

     (#) Initialize the PCD low level resources through the HAL_PCD_MspInit() API:
         (##) Enable the PCD/USB Low Level interface clock using
              (+++) __HAL_RCC_USB_OTG_FS_CLK_ENABLE();
              (+++) __HAL_RCC_USB_OTG_HS_CLK_ENABLE(); (For High Speed Mode)

         (##) Initialize the related GPIO clocks
         (##) Configure PCD pin-out
         (##) Configure PCD NVIC interrupt

     (#)Associate the Upper USB device stack to the HAL PCD Driver:
         (##) hpcd.pData = pdev;

     (#)Enable PCD transmission and reception:
         (##) HAL_PCD_Start();

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

/** @defgroup PCD PCD
  * @brief PCD HAL module driver
  * @{
  */

#ifdef HAL_PCD_MODULE_ENABLED                           /* 条件编译 */

#if defined (USB_OTG_FS) || defined (USB_OTG_HS)        /* 条件编译 */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/** @defgroup PCD_Private_Macros PCD Private Macros
  * @{
  */
#define PCD_MIN(a, b)  (((a) < (b)) ? (a) : (b))        /* 定义宏 */
#define PCD_MAX(a, b)  (((a) > (b)) ? (a) : (b))        /* 定义宏 */
/**
  * @}
  */

/* Private functions prototypes ----------------------------------------------*/
/** @defgroup PCD_Private_Functions PCD Private Functions
  * @{
  */
#if defined (USB_OTG_FS) || defined (USB_OTG_HS)        /* 条件编译 */
static HAL_StatusTypeDef PCD_WriteEmptyTxFifo(PCD_HandleTypeDef *hpcd, uint32_t epnum);  /* 静态变量/函数 */
static HAL_StatusTypeDef PCD_EP_OutXfrComplete_int(PCD_HandleTypeDef *hpcd, uint32_t epnum);  /* 静态变量/函数 */
static HAL_StatusTypeDef PCD_EP_OutSetupPacket_int(PCD_HandleTypeDef *hpcd, uint32_t epnum);  /* 静态变量/函数 */
#endif /* defined (USB_OTG_FS) || defined (USB_OTG_HS) */
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup PCD_Exported_Functions PCD Exported Functions
  * @{
  */

/** @defgroup PCD_Exported_Functions_Group1 Initialization and de-initialization functions
  *  @brief    Initialization and Configuration functions
  *
@verbatim
 ===============================================================================
            ##### Initialization and de-initialization functions #####
 ===============================================================================
    [..]  This section provides functions allowing to:

@endverbatim
  * @{
  */

/**
  * @brief  Initializes the PCD according to the specified
  *         parameters in the PCD_InitTypeDef and initialize the associated handle.
  * @param  hpcd PCD handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCD_Init(PCD_HandleTypeDef *hpcd)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  USB_OTG_GlobalTypeDef *USBx;                          /* 执行语句 */
  uint8_t i;                                            /* 8位无符号变量/函数 */

  /* Check the PCD handle allocation */
  if (hpcd == NULL)                                     /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the parameters */
  assert_param(IS_PCD_ALL_INSTANCE(hpcd->Instance));    /* 执行语句 */

  USBx = hpcd->Instance;                                /* 赋值或初始化 */

  if (hpcd->State == HAL_PCD_STATE_RESET)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Allocate lock resource and initialize it */
    hpcd->Lock = HAL_UNLOCKED;                          /* 赋值或初始化 */

#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
    hpcd->SOFCallback = HAL_PCD_SOFCallback;            /* 赋值或初始化 */
    hpcd->SetupStageCallback = HAL_PCD_SetupStageCallback;  /* 赋值或初始化 */
    hpcd->ResetCallback = HAL_PCD_ResetCallback;        /* 赋值或初始化 */
    hpcd->SuspendCallback = HAL_PCD_SuspendCallback;    /* 赋值或初始化 */
    hpcd->ResumeCallback = HAL_PCD_ResumeCallback;      /* 赋值或初始化 */
    hpcd->ConnectCallback = HAL_PCD_ConnectCallback;    /* 赋值或初始化 */
    hpcd->DisconnectCallback = HAL_PCD_DisconnectCallback;  /* 赋值或初始化 */
    hpcd->DataOutStageCallback = HAL_PCD_DataOutStageCallback;  /* 赋值或初始化 */
    hpcd->DataInStageCallback = HAL_PCD_DataInStageCallback;  /* 赋值或初始化 */
    hpcd->ISOOUTIncompleteCallback = HAL_PCD_ISOOUTIncompleteCallback;  /* 赋值或初始化 */
    hpcd->ISOINIncompleteCallback = HAL_PCD_ISOINIncompleteCallback;  /* 赋值或初始化 */
    hpcd->LPMCallback = HAL_PCDEx_LPM_Callback;         /* 赋值或初始化 */
    hpcd->BCDCallback = HAL_PCDEx_BCD_Callback;         /* 赋值或初始化 */

    if (hpcd->MspInitCallback == NULL)                  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hpcd->MspInitCallback = HAL_PCD_MspInit;          /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Init the low level hardware */
    hpcd->MspInitCallback(hpcd);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /* Init the low level hardware : GPIO, CLOCK, NVIC... */
    HAL_PCD_MspInit(hpcd);                              /* HAL库函数调用 */
#endif /* (USE_HAL_PCD_REGISTER_CALLBACKS) */
  }                                                     /* 代码块结束 */

  hpcd->State = HAL_PCD_STATE_BUSY;                     /* 赋值或初始化 */

  /* Disable DMA mode for FS instance */
  if ((USBx->CID & (0x1U << 8)) == 0U)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hpcd->Init.dma_enable = 0U;                         /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Disable the Interrupts */
  __HAL_PCD_DISABLE(hpcd);                              /* HAL辅助宏/函数 */

  /*Init the Core (common init.) */
  if (USB_CoreInit(hpcd->Instance, hpcd->Init) != HAL_OK)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hpcd->State = HAL_PCD_STATE_ERROR;                  /* 赋值或初始化 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Force Device Mode*/
  (void)USB_SetCurrentMode(hpcd->Instance, USB_DEVICE_MODE);  /* 执行语句 */

  /* Init endpoints structures */
  for (i = 0U; i < hpcd->Init.dev_endpoints; i++)       /* for循环 */
  {                                                     /* 代码块开始 */
    /* Init ep structure */
    hpcd->IN_ep[i].is_in = 1U;                          /* 赋值或初始化 */
    hpcd->IN_ep[i].num = i;                             /* 赋值或初始化 */
    hpcd->IN_ep[i].tx_fifo_num = i;                     /* 赋值或初始化 */
    /* Control until ep is activated */
    hpcd->IN_ep[i].type = EP_TYPE_CTRL;                 /* 赋值或初始化 */
    hpcd->IN_ep[i].maxpacket = 0U;                      /* 赋值或初始化 */
    hpcd->IN_ep[i].xfer_buff = 0U;                      /* 赋值或初始化 */
    hpcd->IN_ep[i].xfer_len = 0U;                       /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  for (i = 0U; i < hpcd->Init.dev_endpoints; i++)       /* for循环 */
  {                                                     /* 代码块开始 */
    hpcd->OUT_ep[i].is_in = 0U;                         /* 赋值或初始化 */
    hpcd->OUT_ep[i].num = i;                            /* 赋值或初始化 */
    /* Control until ep is activated */
    hpcd->OUT_ep[i].type = EP_TYPE_CTRL;                /* 赋值或初始化 */
    hpcd->OUT_ep[i].maxpacket = 0U;                     /* 赋值或初始化 */
    hpcd->OUT_ep[i].xfer_buff = 0U;                     /* 赋值或初始化 */
    hpcd->OUT_ep[i].xfer_len = 0U;                      /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Init Device */
  if (USB_DevInit(hpcd->Instance, hpcd->Init) != HAL_OK)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hpcd->State = HAL_PCD_STATE_ERROR;                  /* 赋值或初始化 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  hpcd->USB_Address = 0U;                               /* 赋值或初始化 */
  hpcd->State = HAL_PCD_STATE_READY;                    /* 赋值或初始化 */
  #if defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx) || defined(STM32F412Zx) || defined(STM32F412Vx) || defined(STM32F412Rx) || defined(STM32F412Cx) || defined(STM32F413xx) || defined(STM32F423xx)  /* 条件编译 */
  /* Activate LPM */
  if (hpcd->Init.lpm_enable == 1U)                      /* 条件判断 */
  {                                                     /* 代码块开始 */
    (void)HAL_PCDEx_ActivateLPM(hpcd);                  /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif /* defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx) || defined(STM32F412Zx) || defined(STM32F412Vx) || defined(STM32F412Rx) || defined(STM32F412Cx) || defined(STM32F413xx) || defined(STM32F423xx) */
  (void)USB_DevDisconnect(hpcd->Instance);              /* 执行语句 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  DeInitializes the PCD peripheral.
  * @param  hpcd PCD handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCD_DeInit(PCD_HandleTypeDef *hpcd)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the PCD handle allocation */
  if (hpcd == NULL)                                     /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  hpcd->State = HAL_PCD_STATE_BUSY;                     /* 赋值或初始化 */

  /* Stop Device */
  if (USB_StopDevice(hpcd->Instance) != HAL_OK)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
  if (hpcd->MspDeInitCallback == NULL)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hpcd->MspDeInitCallback = HAL_PCD_MspDeInit; /* Legacy weak MspDeInit  */
  }                                                     /* 代码块结束 */

  /* DeInit the low level hardware */
  hpcd->MspDeInitCallback(hpcd);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* DeInit the low level hardware: CLOCK, NVIC.*/
  HAL_PCD_MspDeInit(hpcd);                              /* HAL库函数调用 */
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */

  hpcd->State = HAL_PCD_STATE_RESET;                    /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initializes the PCD MSP.
  * @param  hpcd PCD handle
  * @retval None
  */
__weak void HAL_PCD_MspInit(PCD_HandleTypeDef *hpcd)    /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hpcd);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_PCD_MspInit could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  DeInitializes PCD MSP.
  * @param  hpcd PCD handle
  * @retval None
  */
__weak void HAL_PCD_MspDeInit(PCD_HandleTypeDef *hpcd)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hpcd);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_PCD_MspDeInit could be implemented in the user file
   */
}                                                       /* 代码块结束 */

#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
/**
  * @brief  Register a User USB PCD Callback
  *         To be used instead of the weak predefined callback
  * @param  hpcd USB PCD handle
  * @param  CallbackID ID of the callback to be registered
  *         This parameter can be one of the following values:
  *          @arg @ref HAL_PCD_SOF_CB_ID USB PCD SOF callback ID
  *          @arg @ref HAL_PCD_SETUPSTAGE_CB_ID USB PCD Setup callback ID
  *          @arg @ref HAL_PCD_RESET_CB_ID USB PCD Reset callback ID
  *          @arg @ref HAL_PCD_SUSPEND_CB_ID USB PCD Suspend callback ID
  *          @arg @ref HAL_PCD_RESUME_CB_ID USB PCD Resume callback ID
  *          @arg @ref HAL_PCD_CONNECT_CB_ID USB PCD Connect callback ID
  *          @arg @ref HAL_PCD_DISCONNECT_CB_ID OTG PCD Disconnect callback ID
  *          @arg @ref HAL_PCD_MSPINIT_CB_ID MspDeInit callback ID
  *          @arg @ref HAL_PCD_MSPDEINIT_CB_ID MspDeInit callback ID
  * @param  pCallback pointer to the Callback function
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCD_RegisterCallback(PCD_HandleTypeDef *hpcd,  /* HAL状态类型函数 */
                                           HAL_PCD_CallbackIDTypeDef CallbackID,  /* HAL库函数调用 */
                                           pPCD_CallbackTypeDef pCallback)  /* 源代码 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  if (pCallback == NULL)                                /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hpcd->ErrorCode |= HAL_PCD_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  /* Process locked */
  __HAL_LOCK(hpcd);                                     /* HAL辅助宏/函数 */

  if (hpcd->State == HAL_PCD_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_PCD_SOF_CB_ID :                          /* case标签 */
        hpcd->SOFCallback = pCallback;                  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_PCD_SETUPSTAGE_CB_ID :                   /* case标签 */
        hpcd->SetupStageCallback = pCallback;           /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_PCD_RESET_CB_ID :                        /* case标签 */
        hpcd->ResetCallback = pCallback;                /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_PCD_SUSPEND_CB_ID :                      /* case标签 */
        hpcd->SuspendCallback = pCallback;              /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_PCD_RESUME_CB_ID :                       /* case标签 */
        hpcd->ResumeCallback = pCallback;               /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_PCD_CONNECT_CB_ID :                      /* case标签 */
        hpcd->ConnectCallback = pCallback;              /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_PCD_DISCONNECT_CB_ID :                   /* case标签 */
        hpcd->DisconnectCallback = pCallback;           /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_PCD_MSPINIT_CB_ID :                      /* case标签 */
        hpcd->MspInitCallback = pCallback;              /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_PCD_MSPDEINIT_CB_ID :                    /* case标签 */
        hpcd->MspDeInitCallback = pCallback;            /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hpcd->ErrorCode |= HAL_PCD_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (hpcd->State == HAL_PCD_STATE_RESET)          /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_PCD_MSPINIT_CB_ID :                      /* case标签 */
        hpcd->MspInitCallback = pCallback;              /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_PCD_MSPDEINIT_CB_ID :                    /* case标签 */
        hpcd->MspDeInitCallback = pCallback;            /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hpcd->ErrorCode |= HAL_PCD_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hpcd->ErrorCode |= HAL_PCD_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hpcd);                                   /* HAL辅助宏/函数 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Unregister an USB PCD Callback
  *         USB PCD callabck is redirected to the weak predefined callback
  * @param  hpcd USB PCD handle
  * @param  CallbackID ID of the callback to be unregistered
  *         This parameter can be one of the following values:
  *          @arg @ref HAL_PCD_SOF_CB_ID USB PCD SOF callback ID
  *          @arg @ref HAL_PCD_SETUPSTAGE_CB_ID USB PCD Setup callback ID
  *          @arg @ref HAL_PCD_RESET_CB_ID USB PCD Reset callback ID
  *          @arg @ref HAL_PCD_SUSPEND_CB_ID USB PCD Suspend callback ID
  *          @arg @ref HAL_PCD_RESUME_CB_ID USB PCD Resume callback ID
  *          @arg @ref HAL_PCD_CONNECT_CB_ID USB PCD Connect callback ID
  *          @arg @ref HAL_PCD_DISCONNECT_CB_ID OTG PCD Disconnect callback ID
  *          @arg @ref HAL_PCD_MSPINIT_CB_ID MspDeInit callback ID
  *          @arg @ref HAL_PCD_MSPDEINIT_CB_ID MspDeInit callback ID
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCD_UnRegisterCallback(PCD_HandleTypeDef *hpcd, HAL_PCD_CallbackIDTypeDef CallbackID)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Process locked */
  __HAL_LOCK(hpcd);                                     /* HAL辅助宏/函数 */

  /* Setup Legacy weak Callbacks  */
  if (hpcd->State == HAL_PCD_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_PCD_SOF_CB_ID :                          /* case标签 */
        hpcd->SOFCallback = HAL_PCD_SOFCallback;        /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_PCD_SETUPSTAGE_CB_ID :                   /* case标签 */
        hpcd->SetupStageCallback = HAL_PCD_SetupStageCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_PCD_RESET_CB_ID :                        /* case标签 */
        hpcd->ResetCallback = HAL_PCD_ResetCallback;    /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_PCD_SUSPEND_CB_ID :                      /* case标签 */
        hpcd->SuspendCallback = HAL_PCD_SuspendCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_PCD_RESUME_CB_ID :                       /* case标签 */
        hpcd->ResumeCallback = HAL_PCD_ResumeCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_PCD_CONNECT_CB_ID :                      /* case标签 */
        hpcd->ConnectCallback = HAL_PCD_ConnectCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_PCD_DISCONNECT_CB_ID :                   /* case标签 */
        hpcd->DisconnectCallback = HAL_PCD_DisconnectCallback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_PCD_MSPINIT_CB_ID :                      /* case标签 */
        hpcd->MspInitCallback = HAL_PCD_MspInit;        /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_PCD_MSPDEINIT_CB_ID :                    /* case标签 */
        hpcd->MspDeInitCallback = HAL_PCD_MspDeInit;    /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hpcd->ErrorCode |= HAL_PCD_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (hpcd->State == HAL_PCD_STATE_RESET)          /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_PCD_MSPINIT_CB_ID :                      /* case标签 */
        hpcd->MspInitCallback = HAL_PCD_MspInit;        /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_PCD_MSPDEINIT_CB_ID :                    /* case标签 */
        hpcd->MspDeInitCallback = HAL_PCD_MspDeInit;    /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hpcd->ErrorCode |= HAL_PCD_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hpcd->ErrorCode |= HAL_PCD_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hpcd);                                   /* HAL辅助宏/函数 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Register USB PCD Data OUT Stage Callback
  *         To be used instead of the weak HAL_PCD_DataOutStageCallback() predefined callback
  * @param  hpcd PCD handle
  * @param  pCallback pointer to the USB PCD Data OUT Stage Callback function
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCD_RegisterDataOutStageCallback(PCD_HandleTypeDef *hpcd,  /* HAL状态类型函数 */
                                                       pPCD_DataOutStageCallbackTypeDef pCallback)  /* 源代码 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  if (pCallback == NULL)                                /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hpcd->ErrorCode |= HAL_PCD_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Process locked */
  __HAL_LOCK(hpcd);                                     /* HAL辅助宏/函数 */

  if (hpcd->State == HAL_PCD_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    hpcd->DataOutStageCallback = pCallback;             /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hpcd->ErrorCode |= HAL_PCD_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hpcd);                                   /* HAL辅助宏/函数 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Unregister the USB PCD Data OUT Stage Callback
  *         USB PCD Data OUT Stage Callback is redirected to the weak HAL_PCD_DataOutStageCallback() predefined callback
  * @param  hpcd PCD handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCD_UnRegisterDataOutStageCallback(PCD_HandleTypeDef *hpcd)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Process locked */
  __HAL_LOCK(hpcd);                                     /* HAL辅助宏/函数 */

  if (hpcd->State == HAL_PCD_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    hpcd->DataOutStageCallback = HAL_PCD_DataOutStageCallback; /* Legacy weak DataOutStageCallback  */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hpcd->ErrorCode |= HAL_PCD_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hpcd);                                   /* HAL辅助宏/函数 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Register USB PCD Data IN Stage Callback
  *         To be used instead of the weak HAL_PCD_DataInStageCallback() predefined callback
  * @param  hpcd PCD handle
  * @param  pCallback pointer to the USB PCD Data IN Stage Callback function
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCD_RegisterDataInStageCallback(PCD_HandleTypeDef *hpcd,  /* HAL状态类型函数 */
                                                      pPCD_DataInStageCallbackTypeDef pCallback)  /* 源代码 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  if (pCallback == NULL)                                /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hpcd->ErrorCode |= HAL_PCD_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Process locked */
  __HAL_LOCK(hpcd);                                     /* HAL辅助宏/函数 */

  if (hpcd->State == HAL_PCD_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    hpcd->DataInStageCallback = pCallback;              /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hpcd->ErrorCode |= HAL_PCD_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hpcd);                                   /* HAL辅助宏/函数 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Unregister the USB PCD Data IN Stage Callback
  *         USB PCD Data OUT Stage Callback is redirected to the weak HAL_PCD_DataInStageCallback() predefined callback
  * @param  hpcd PCD handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCD_UnRegisterDataInStageCallback(PCD_HandleTypeDef *hpcd)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Process locked */
  __HAL_LOCK(hpcd);                                     /* HAL辅助宏/函数 */

  if (hpcd->State == HAL_PCD_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    hpcd->DataInStageCallback = HAL_PCD_DataInStageCallback; /* Legacy weak DataInStageCallback  */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hpcd->ErrorCode |= HAL_PCD_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hpcd);                                   /* HAL辅助宏/函数 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Register USB PCD Iso OUT incomplete Callback
  *         To be used instead of the weak HAL_PCD_ISOOUTIncompleteCallback() predefined callback
  * @param  hpcd PCD handle
  * @param  pCallback pointer to the USB PCD Iso OUT incomplete Callback function
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCD_RegisterIsoOutIncpltCallback(PCD_HandleTypeDef *hpcd,  /* HAL状态类型函数 */
                                                       pPCD_IsoOutIncpltCallbackTypeDef pCallback)  /* 源代码 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  if (pCallback == NULL)                                /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hpcd->ErrorCode |= HAL_PCD_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Process locked */
  __HAL_LOCK(hpcd);                                     /* HAL辅助宏/函数 */

  if (hpcd->State == HAL_PCD_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    hpcd->ISOOUTIncompleteCallback = pCallback;         /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hpcd->ErrorCode |= HAL_PCD_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hpcd);                                   /* HAL辅助宏/函数 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Unregister the USB PCD Iso OUT incomplete Callback
  *         USB PCD Iso OUT incomplete Callback is redirected to the weak HAL_PCD_ISOOUTIncompleteCallback() predefined callback
  * @param  hpcd PCD handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCD_UnRegisterIsoOutIncpltCallback(PCD_HandleTypeDef *hpcd)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Process locked */
  __HAL_LOCK(hpcd);                                     /* HAL辅助宏/函数 */

  if (hpcd->State == HAL_PCD_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    hpcd->ISOOUTIncompleteCallback = HAL_PCD_ISOOUTIncompleteCallback; /* Legacy weak ISOOUTIncompleteCallback  */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hpcd->ErrorCode |= HAL_PCD_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hpcd);                                   /* HAL辅助宏/函数 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Register USB PCD Iso IN incomplete Callback
  *         To be used instead of the weak HAL_PCD_ISOINIncompleteCallback() predefined callback
  * @param  hpcd PCD handle
  * @param  pCallback pointer to the USB PCD Iso IN incomplete Callback function
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCD_RegisterIsoInIncpltCallback(PCD_HandleTypeDef *hpcd,  /* HAL状态类型函数 */
                                                      pPCD_IsoInIncpltCallbackTypeDef pCallback)  /* 源代码 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  if (pCallback == NULL)                                /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hpcd->ErrorCode |= HAL_PCD_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Process locked */
  __HAL_LOCK(hpcd);                                     /* HAL辅助宏/函数 */

  if (hpcd->State == HAL_PCD_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    hpcd->ISOINIncompleteCallback = pCallback;          /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hpcd->ErrorCode |= HAL_PCD_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hpcd);                                   /* HAL辅助宏/函数 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Unregister the USB PCD Iso IN incomplete Callback
  *         USB PCD Iso IN incomplete Callback is redirected to the weak HAL_PCD_ISOINIncompleteCallback() predefined callback
  * @param  hpcd PCD handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCD_UnRegisterIsoInIncpltCallback(PCD_HandleTypeDef *hpcd)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Process locked */
  __HAL_LOCK(hpcd);                                     /* HAL辅助宏/函数 */

  if (hpcd->State == HAL_PCD_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    hpcd->ISOINIncompleteCallback = HAL_PCD_ISOINIncompleteCallback; /* Legacy weak ISOINIncompleteCallback  */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hpcd->ErrorCode |= HAL_PCD_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hpcd);                                   /* HAL辅助宏/函数 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Register USB PCD BCD Callback
  *         To be used instead of the weak HAL_PCDEx_BCD_Callback() predefined callback
  * @param  hpcd PCD handle
  * @param  pCallback pointer to the USB PCD BCD Callback function
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCD_RegisterBcdCallback(PCD_HandleTypeDef *hpcd, pPCD_BcdCallbackTypeDef pCallback)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  if (pCallback == NULL)                                /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hpcd->ErrorCode |= HAL_PCD_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Process locked */
  __HAL_LOCK(hpcd);                                     /* HAL辅助宏/函数 */

  if (hpcd->State == HAL_PCD_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    hpcd->BCDCallback = pCallback;                      /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hpcd->ErrorCode |= HAL_PCD_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hpcd);                                   /* HAL辅助宏/函数 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Unregister the USB PCD BCD Callback
  *         USB BCD Callback is redirected to the weak HAL_PCDEx_BCD_Callback() predefined callback
  * @param  hpcd PCD handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCD_UnRegisterBcdCallback(PCD_HandleTypeDef *hpcd)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Process locked */
  __HAL_LOCK(hpcd);                                     /* HAL辅助宏/函数 */

  if (hpcd->State == HAL_PCD_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    hpcd->BCDCallback = HAL_PCDEx_BCD_Callback; /* Legacy weak HAL_PCDEx_BCD_Callback  */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hpcd->ErrorCode |= HAL_PCD_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hpcd);                                   /* HAL辅助宏/函数 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Register USB PCD LPM Callback
  *         To be used instead of the weak HAL_PCDEx_LPM_Callback() predefined callback
  * @param  hpcd PCD handle
  * @param  pCallback pointer to the USB PCD LPM Callback function
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCD_RegisterLpmCallback(PCD_HandleTypeDef *hpcd, pPCD_LpmCallbackTypeDef pCallback)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  if (pCallback == NULL)                                /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hpcd->ErrorCode |= HAL_PCD_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Process locked */
  __HAL_LOCK(hpcd);                                     /* HAL辅助宏/函数 */

  if (hpcd->State == HAL_PCD_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    hpcd->LPMCallback = pCallback;                      /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hpcd->ErrorCode |= HAL_PCD_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hpcd);                                   /* HAL辅助宏/函数 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Unregister the USB PCD LPM Callback
  *         USB LPM Callback is redirected to the weak HAL_PCDEx_LPM_Callback() predefined callback
  * @param  hpcd PCD handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCD_UnRegisterLpmCallback(PCD_HandleTypeDef *hpcd)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Process locked */
  __HAL_LOCK(hpcd);                                     /* HAL辅助宏/函数 */

  if (hpcd->State == HAL_PCD_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    hpcd->LPMCallback = HAL_PCDEx_LPM_Callback; /* Legacy weak HAL_PCDEx_LPM_Callback  */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hpcd->ErrorCode |= HAL_PCD_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hpcd);                                   /* HAL辅助宏/函数 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */

/**
  * @}
  */

/** @defgroup PCD_Exported_Functions_Group2 Input and Output operation functions
  *  @brief   Data transfers functions
  *
@verbatim
 ===============================================================================
                      ##### IO operation functions #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to manage the PCD data
    transfers.

@endverbatim
  * @{
  */

/**
  * @brief  Start the USB device
  * @param  hpcd PCD handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCD_Start(PCD_HandleTypeDef *hpcd)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  USB_OTG_GlobalTypeDef *USBx = hpcd->Instance;         /* 赋值或初始化 */

  __HAL_LOCK(hpcd);                                     /* HAL辅助宏/函数 */

  if ((hpcd->Init.battery_charging_enable == 1U) &&     /* 条件判断 */
      (hpcd->Init.phy_itface != USB_OTG_ULPI_PHY))      /* 源代码 */
  {                                                     /* 代码块开始 */
    /* Enable USB Transceiver */
    USBx->GCCFG |= USB_OTG_GCCFG_PWRDWN;                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  __HAL_PCD_ENABLE(hpcd);                               /* HAL辅助宏/函数 */
  (void)USB_DevConnect(hpcd->Instance);                 /* 执行语句 */
  __HAL_UNLOCK(hpcd);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Stop the USB device.
  * @param  hpcd PCD handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCD_Stop(PCD_HandleTypeDef *hpcd)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  USB_OTG_GlobalTypeDef *USBx = hpcd->Instance;         /* 赋值或初始化 */

  __HAL_LOCK(hpcd);                                     /* HAL辅助宏/函数 */
  __HAL_PCD_DISABLE(hpcd);                              /* HAL辅助宏/函数 */
  (void)USB_DevDisconnect(hpcd->Instance);              /* 执行语句 */

  (void)USB_FlushTxFifo(hpcd->Instance, 0x10U);         /* 执行语句 */

  if ((hpcd->Init.battery_charging_enable == 1U) &&     /* 条件判断 */
      (hpcd->Init.phy_itface != USB_OTG_ULPI_PHY))      /* 源代码 */
  {                                                     /* 代码块开始 */
    /* Disable USB Transceiver */
    USBx->GCCFG &= ~(USB_OTG_GCCFG_PWRDWN);             /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  __HAL_UNLOCK(hpcd);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

#if defined (USB_OTG_FS) || defined (USB_OTG_HS)        /* 条件编译 */
/**
  * @brief  Handles PCD interrupt request.
  * @param  hpcd PCD handle
  * @retval HAL status
  */
void HAL_PCD_IRQHandler(PCD_HandleTypeDef *hpcd)        /* 中断服务函数 */
{                                                       /* 代码块开始 */
  USB_OTG_GlobalTypeDef *USBx = hpcd->Instance;         /* 赋值或初始化 */
  uint32_t USBx_BASE = (uint32_t)USBx;                  /* 32位无符号变量/函数 */
  uint32_t i, ep_intr, epint, epnum;                    /* 32位无符号变量/函数 */
  uint32_t fifoemptymsk, temp;                          /* 32位无符号变量/函数 */
  USB_OTG_EPTypeDef *ep;                                /* 执行语句 */

  /* ensure that we are in device mode */
  if (USB_GetMode(hpcd->Instance) == USB_OTG_MODE_DEVICE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* avoid spurious interrupt */
    if (__HAL_PCD_IS_INVALID_INTERRUPT(hpcd))           /* 条件判断 */
    {                                                   /* 代码块开始 */
      return;                                           /* 返回 */
    }                                                   /* 代码块结束 */

    if (__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_MMIS))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* incorrect mode, acknowledge the interrupt */
      __HAL_PCD_CLEAR_FLAG(hpcd, USB_OTG_GINTSTS_MMIS);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Handle RxQLevel Interrupt */
    if (__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_RXFLVL))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      USB_MASK_INTERRUPT(hpcd->Instance, USB_OTG_GINTSTS_RXFLVL);  /* 执行语句 */

      temp = USBx->GRXSTSP;                             /* 赋值或初始化 */

      ep = &hpcd->OUT_ep[temp & USB_OTG_GRXSTSP_EPNUM];  /* 赋值或初始化 */

      if (((temp & USB_OTG_GRXSTSP_PKTSTS) >> 17) ==  STS_DATA_UPDT)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        if ((temp & USB_OTG_GRXSTSP_BCNT) != 0U)        /* 条件判断 */
        {                                               /* 代码块开始 */
          (void)USB_ReadPacket(USBx, ep->xfer_buff,     /* 列表续行 */
                               (uint16_t)((temp & USB_OTG_GRXSTSP_BCNT) >> 4));  /* 执行语句 */

          ep->xfer_buff += (temp & USB_OTG_GRXSTSP_BCNT) >> 4;  /* 赋值或初始化 */
          ep->xfer_count += (temp & USB_OTG_GRXSTSP_BCNT) >> 4;  /* 赋值或初始化 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else if (((temp & USB_OTG_GRXSTSP_PKTSTS) >> 17) ==  STS_SETUP_UPDT)  /* 否则如果 */
      {                                                 /* 代码块开始 */
        (void)USB_ReadPacket(USBx, (uint8_t *)hpcd->Setup, 8U);  /* 执行语句 */
        ep->xfer_count += (temp & USB_OTG_GRXSTSP_BCNT) >> 4;  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* ... */
      }                                                 /* 代码块结束 */
      USB_UNMASK_INTERRUPT(hpcd->Instance, USB_OTG_GINTSTS_RXFLVL);  /* 执行语句 */
    }                                                   /* 代码块结束 */

    if (__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_OEPINT))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      epnum = 0U;                                       /* 赋值或初始化 */

      /* Read in the device interrupt bits */
      ep_intr = USB_ReadDevAllOutEpInterrupt(hpcd->Instance);  /* 赋值或初始化 */

      while (ep_intr != 0U)                             /* while循环 */
      {                                                 /* 代码块开始 */
        if ((ep_intr & 0x1U) != 0U)                     /* 条件判断 */
        {                                               /* 代码块开始 */
          epint = USB_ReadDevOutEPInterrupt(hpcd->Instance, (uint8_t)epnum);  /* 赋值或初始化 */

          if ((epint & USB_OTG_DOEPINT_XFRC) == USB_OTG_DOEPINT_XFRC)  /* 条件判断 */
          {                                             /* 代码块开始 */
            CLEAR_OUT_EP_INTR(epnum, USB_OTG_DOEPINT_XFRC);  /* 执行语句 */
            (void)PCD_EP_OutXfrComplete_int(hpcd, epnum);  /* 执行语句 */
          }                                             /* 代码块结束 */

          if ((epint & USB_OTG_DOEPINT_STUP) == USB_OTG_DOEPINT_STUP)  /* 条件判断 */
          {                                             /* 代码块开始 */
            CLEAR_OUT_EP_INTR(epnum, USB_OTG_DOEPINT_STUP);  /* 执行语句 */
            /* Class B setup phase done for previous decoded setup */
            (void)PCD_EP_OutSetupPacket_int(hpcd, epnum);  /* 执行语句 */
          }                                             /* 代码块结束 */

          if ((epint & USB_OTG_DOEPINT_OTEPDIS) == USB_OTG_DOEPINT_OTEPDIS)  /* 条件判断 */
          {                                             /* 代码块开始 */
            CLEAR_OUT_EP_INTR(epnum, USB_OTG_DOEPINT_OTEPDIS);  /* 执行语句 */
          }                                             /* 代码块结束 */

          /* Clear Status Phase Received interrupt */
          if ((epint & USB_OTG_DOEPINT_OTEPSPR) == USB_OTG_DOEPINT_OTEPSPR)  /* 条件判断 */
          {                                             /* 代码块开始 */
            CLEAR_OUT_EP_INTR(epnum, USB_OTG_DOEPINT_OTEPSPR);  /* 执行语句 */
          }                                             /* 代码块结束 */

          /* Clear OUT NAK interrupt */
          if ((epint & USB_OTG_DOEPINT_NAK) == USB_OTG_DOEPINT_NAK)  /* 条件判断 */
          {                                             /* 代码块开始 */
            CLEAR_OUT_EP_INTR(epnum, USB_OTG_DOEPINT_NAK);  /* 执行语句 */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */
        epnum++;                                        /* 执行语句 */
        ep_intr >>= 1U;                                 /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    if (__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_IEPINT))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Read in the device interrupt bits */
      ep_intr = USB_ReadDevAllInEpInterrupt(hpcd->Instance);  /* 赋值或初始化 */

      epnum = 0U;                                       /* 赋值或初始化 */

      while (ep_intr != 0U)                             /* while循环 */
      {                                                 /* 代码块开始 */
        if ((ep_intr & 0x1U) != 0U) /* In ITR */
        {                                               /* 代码块开始 */
          epint = USB_ReadDevInEPInterrupt(hpcd->Instance, (uint8_t)epnum);  /* 赋值或初始化 */

          if ((epint & USB_OTG_DIEPINT_XFRC) == USB_OTG_DIEPINT_XFRC)  /* 条件判断 */
          {                                             /* 代码块开始 */
            fifoemptymsk = (uint32_t)(0x1UL << (epnum & EP_ADDR_MSK));  /* 赋值或初始化 */
            USBx_DEVICE->DIEPEMPMSK &= ~fifoemptymsk;   /* 赋值或初始化 */

            CLEAR_IN_EP_INTR(epnum, USB_OTG_DIEPINT_XFRC);  /* 执行语句 */

            if (hpcd->Init.dma_enable == 1U)            /* 条件判断 */
            {                                           /* 代码块开始 */
              hpcd->IN_ep[epnum].xfer_buff += hpcd->IN_ep[epnum].maxpacket;  /* 赋值或初始化 */

              /* this is ZLP, so prepare EP0 for next setup */
              if ((epnum == 0U) && (hpcd->IN_ep[epnum].xfer_len == 0U))  /* 条件判断 */
              {                                         /* 代码块开始 */
                /* prepare to rx more setup packets */
                (void)USB_EP0_OutStart(hpcd->Instance, 1U, (uint8_t *)hpcd->Setup);  /* 执行语句 */
              }                                         /* 代码块结束 */
            }                                           /* 代码块结束 */

#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
            hpcd->DataInStageCallback(hpcd, (uint8_t)epnum);  /* 执行语句 */
#else                                                   /* 条件编译否则 */
            HAL_PCD_DataInStageCallback(hpcd, (uint8_t)epnum);  /* HAL库函数调用 */
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */
          }                                             /* 代码块结束 */
          if ((epint & USB_OTG_DIEPINT_TOC) == USB_OTG_DIEPINT_TOC)  /* 条件判断 */
          {                                             /* 代码块开始 */
            CLEAR_IN_EP_INTR(epnum, USB_OTG_DIEPINT_TOC);  /* 执行语句 */
          }                                             /* 代码块结束 */
          if ((epint & USB_OTG_DIEPINT_ITTXFE) == USB_OTG_DIEPINT_ITTXFE)  /* 条件判断 */
          {                                             /* 代码块开始 */
            CLEAR_IN_EP_INTR(epnum, USB_OTG_DIEPINT_ITTXFE);  /* 执行语句 */
          }                                             /* 代码块结束 */
          if ((epint & USB_OTG_DIEPINT_INEPNE) == USB_OTG_DIEPINT_INEPNE)  /* 条件判断 */
          {                                             /* 代码块开始 */
            CLEAR_IN_EP_INTR(epnum, USB_OTG_DIEPINT_INEPNE);  /* 执行语句 */
          }                                             /* 代码块结束 */
          if ((epint & USB_OTG_DIEPINT_EPDISD) == USB_OTG_DIEPINT_EPDISD)  /* 条件判断 */
          {                                             /* 代码块开始 */
            CLEAR_IN_EP_INTR(epnum, USB_OTG_DIEPINT_EPDISD);  /* 执行语句 */
          }                                             /* 代码块结束 */
          if ((epint & USB_OTG_DIEPINT_TXFE) == USB_OTG_DIEPINT_TXFE)  /* 条件判断 */
          {                                             /* 代码块开始 */
            (void)PCD_WriteEmptyTxFifo(hpcd, epnum);    /* 执行语句 */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */
        epnum++;                                        /* 执行语句 */
        ep_intr >>= 1U;                                 /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Handle Resume Interrupt */
    if (__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_WKUINT))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Clear the Remote Wake-up Signaling */
      USBx_DEVICE->DCTL &= ~USB_OTG_DCTL_RWUSIG;        /* 赋值或初始化 */

      if (hpcd->LPM_State == LPM_L1)                    /* 条件判断 */
      {                                                 /* 代码块开始 */
        hpcd->LPM_State = LPM_L0;                       /* 赋值或初始化 */

#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
        hpcd->LPMCallback(hpcd, PCD_LPM_L0_ACTIVE);     /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_PCDEx_LPM_Callback(hpcd, PCD_LPM_L0_ACTIVE);  /* HAL库函数调用 */
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
        hpcd->ResumeCallback(hpcd);                     /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_PCD_ResumeCallback(hpcd);                   /* HAL库函数调用 */
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */
      }                                                 /* 代码块结束 */

      __HAL_PCD_CLEAR_FLAG(hpcd, USB_OTG_GINTSTS_WKUINT);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Handle Suspend Interrupt */
    if (__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_USBSUSP))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      if ((USBx_DEVICE->DSTS & USB_OTG_DSTS_SUSPSTS) == USB_OTG_DSTS_SUSPSTS)  /* 条件判断 */
      {                                                 /* 代码块开始 */
#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
        hpcd->SuspendCallback(hpcd);                    /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_PCD_SuspendCallback(hpcd);                  /* HAL库函数调用 */
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */
      }                                                 /* 代码块结束 */
      __HAL_PCD_CLEAR_FLAG(hpcd, USB_OTG_GINTSTS_USBSUSP);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
#if defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx) || defined(STM32F412Zx) || defined(STM32F412Vx) || defined(STM32F412Rx) || defined(STM32F412Cx) || defined(STM32F413xx) || defined(STM32F423xx)  /* 条件编译 */
    /* Handle LPM Interrupt */
    if (__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_LPMINT))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      __HAL_PCD_CLEAR_FLAG(hpcd, USB_OTG_GINTSTS_LPMINT);  /* HAL辅助宏/函数 */

      if (hpcd->LPM_State == LPM_L0)                    /* 条件判断 */
      {                                                 /* 代码块开始 */
        hpcd->LPM_State = LPM_L1;                       /* 赋值或初始化 */
        hpcd->BESL = (hpcd->Instance->GLPMCFG & USB_OTG_GLPMCFG_BESL) >> 2U;  /* 赋值或初始化 */

#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
        hpcd->LPMCallback(hpcd, PCD_LPM_L1_ACTIVE);     /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_PCDEx_LPM_Callback(hpcd, PCD_LPM_L1_ACTIVE);  /* HAL库函数调用 */
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
        hpcd->SuspendCallback(hpcd);                    /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_PCD_SuspendCallback(hpcd);                  /* HAL库函数调用 */
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
#endif /* defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx) || defined(STM32F412Zx) || defined(STM32F412Vx) || defined(STM32F412Rx) || defined(STM32F412Cx) || defined(STM32F413xx) || defined(STM32F423xx) */
    /* Handle Reset Interrupt */
    if (__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_USBRST))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      USBx_DEVICE->DCTL &= ~USB_OTG_DCTL_RWUSIG;        /* 赋值或初始化 */
      (void)USB_FlushTxFifo(hpcd->Instance, 0x10U);     /* 执行语句 */

      for (i = 0U; i < hpcd->Init.dev_endpoints; i++)   /* for循环 */
      {                                                 /* 代码块开始 */
        USBx_INEP(i)->DIEPINT = 0xFB7FU;                /* 赋值或初始化 */
        USBx_INEP(i)->DIEPCTL &= ~USB_OTG_DIEPCTL_STALL;  /* 赋值或初始化 */
        USBx_INEP(i)->DIEPCTL |= USB_OTG_DIEPCTL_SNAK;  /* 赋值或初始化 */
        USBx_OUTEP(i)->DOEPINT = 0xFB7FU;               /* 赋值或初始化 */
        USBx_OUTEP(i)->DOEPCTL &= ~USB_OTG_DOEPCTL_STALL;  /* 赋值或初始化 */
        USBx_OUTEP(i)->DOEPCTL |= USB_OTG_DOEPCTL_SNAK;  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      USBx_DEVICE->DAINTMSK |= 0x10001U;                /* 赋值或初始化 */

      if (hpcd->Init.use_dedicated_ep1 != 0U)           /* 条件判断 */
      {                                                 /* 代码块开始 */
        USBx_DEVICE->DOUTEP1MSK |= USB_OTG_DOEPMSK_STUPM |  /* 源代码 */
                                   USB_OTG_DOEPMSK_XFRCM |  /* 源代码 */
                                   USB_OTG_DOEPMSK_EPDM;  /* 执行语句 */

        USBx_DEVICE->DINEP1MSK |= USB_OTG_DIEPMSK_TOM |  /* 源代码 */
                                  USB_OTG_DIEPMSK_XFRCM |  /* 源代码 */
                                  USB_OTG_DIEPMSK_EPDM;  /* 执行语句 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        USBx_DEVICE->DOEPMSK |= USB_OTG_DOEPMSK_STUPM |  /* 源代码 */
                                USB_OTG_DOEPMSK_XFRCM |  /* 源代码 */
                                USB_OTG_DOEPMSK_EPDM |  /* 源代码 */
                                USB_OTG_DOEPMSK_OTEPSPRM |  /* 源代码 */
                                USB_OTG_DOEPMSK_NAKM;   /* 执行语句 */

        USBx_DEVICE->DIEPMSK |= USB_OTG_DIEPMSK_TOM |   /* 源代码 */
                                USB_OTG_DIEPMSK_XFRCM |  /* 源代码 */
                                USB_OTG_DIEPMSK_EPDM;   /* 执行语句 */
      }                                                 /* 代码块结束 */

      /* Set Default Address to 0 */
      USBx_DEVICE->DCFG &= ~USB_OTG_DCFG_DAD;           /* 赋值或初始化 */

      /* setup EP0 to receive SETUP packets */
      (void)USB_EP0_OutStart(hpcd->Instance, (uint8_t)hpcd->Init.dma_enable,  /* 列表续行 */
                             (uint8_t *)hpcd->Setup);   /* 执行语句 */

      __HAL_PCD_CLEAR_FLAG(hpcd, USB_OTG_GINTSTS_USBRST);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Handle Enumeration done Interrupt */
    if (__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_ENUMDNE))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      (void)USB_ActivateSetup(hpcd->Instance);          /* 执行语句 */
      hpcd->Init.speed = USB_GetDevSpeed(hpcd->Instance);  /* 赋值或初始化 */

      /* Set USB Turnaround time */
      (void)USB_SetTurnaroundTime(hpcd->Instance,       /* 列表续行 */
                                  HAL_RCC_GetHCLKFreq(),  /* HAL 时钟配置 */
                                  (uint8_t)hpcd->Init.speed);  /* 执行语句 */

#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
      hpcd->ResetCallback(hpcd);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_PCD_ResetCallback(hpcd);                      /* HAL库函数调用 */
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */

      __HAL_PCD_CLEAR_FLAG(hpcd, USB_OTG_GINTSTS_ENUMDNE);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Handle SOF Interrupt */
    if (__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_SOF))  /* 条件判断 */
    {                                                   /* 代码块开始 */
#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
      hpcd->SOFCallback(hpcd);                          /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_PCD_SOFCallback(hpcd);                        /* HAL库函数调用 */
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */

      __HAL_PCD_CLEAR_FLAG(hpcd, USB_OTG_GINTSTS_SOF);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Handle Incomplete ISO IN Interrupt */
    if (__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_IISOIXFR))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Keep application checking the corresponding Iso IN endpoint
      causing the incomplete Interrupt */
      epnum = 0U;                                       /* 赋值或初始化 */

#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
      hpcd->ISOINIncompleteCallback(hpcd, (uint8_t)epnum);  /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_PCD_ISOINIncompleteCallback(hpcd, (uint8_t)epnum);  /* HAL库函数调用 */
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */

      __HAL_PCD_CLEAR_FLAG(hpcd, USB_OTG_GINTSTS_IISOIXFR);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Handle Incomplete ISO OUT Interrupt */
    if (__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_PXFR_INCOMPISOOUT))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Keep application checking the corresponding Iso OUT endpoint
      causing the incomplete Interrupt */
      epnum = 0U;                                       /* 赋值或初始化 */

#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
      hpcd->ISOOUTIncompleteCallback(hpcd, (uint8_t)epnum);  /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_PCD_ISOOUTIncompleteCallback(hpcd, (uint8_t)epnum);  /* HAL库函数调用 */
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */

      __HAL_PCD_CLEAR_FLAG(hpcd, USB_OTG_GINTSTS_PXFR_INCOMPISOOUT);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Handle Connection event Interrupt */
    if (__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_SRQINT))  /* 条件判断 */
    {                                                   /* 代码块开始 */
#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
      hpcd->ConnectCallback(hpcd);                      /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_PCD_ConnectCallback(hpcd);                    /* HAL库函数调用 */
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */

      __HAL_PCD_CLEAR_FLAG(hpcd, USB_OTG_GINTSTS_SRQINT);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Handle Disconnection event Interrupt */
    if (__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_OTGINT))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      temp = hpcd->Instance->GOTGINT;                   /* 赋值或初始化 */

      if ((temp & USB_OTG_GOTGINT_SEDET) == USB_OTG_GOTGINT_SEDET)  /* 条件判断 */
      {                                                 /* 代码块开始 */
#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
        hpcd->DisconnectCallback(hpcd);                 /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_PCD_DisconnectCallback(hpcd);               /* HAL库函数调用 */
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */
      }                                                 /* 代码块结束 */
      hpcd->Instance->GOTGINT |= temp;                  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */


/**
  * @brief  Handles PCD Wakeup interrupt request.
  * @param  hpcd PCD handle
  * @retval HAL status
  */
void HAL_PCD_WKUP_IRQHandler(PCD_HandleTypeDef *hpcd)   /* 中断服务函数 */
{                                                       /* 代码块开始 */
  USB_OTG_GlobalTypeDef *USBx;                          /* 执行语句 */

  USBx = hpcd->Instance;                                /* 赋值或初始化 */

  if ((USBx->CID & (0x1U << 8)) == 0U)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Clear EXTI pending Bit */
    __HAL_USB_OTG_FS_WAKEUP_EXTI_CLEAR_FLAG();          /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Clear EXTI pending Bit */
    __HAL_USB_OTG_HS_WAKEUP_EXTI_CLEAR_FLAG();          /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */
#endif /* defined (USB_OTG_FS) || defined (USB_OTG_HS) */


/**
  * @brief  Data OUT stage callback.
  * @param  hpcd PCD handle
  * @param  epnum endpoint number
  * @retval None
  */
__weak void HAL_PCD_DataOutStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hpcd);                                         /* 执行语句 */
  UNUSED(epnum);                                        /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_PCD_DataOutStageCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Data IN stage callback
  * @param  hpcd PCD handle
  * @param  epnum endpoint number
  * @retval None
  */
__weak void HAL_PCD_DataInStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hpcd);                                         /* 执行语句 */
  UNUSED(epnum);                                        /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_PCD_DataInStageCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */
/**
  * @brief  Setup stage callback
  * @param  hpcd PCD handle
  * @retval None
  */
__weak void HAL_PCD_SetupStageCallback(PCD_HandleTypeDef *hpcd)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hpcd);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_PCD_SetupStageCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  USB Start Of Frame callback.
  * @param  hpcd PCD handle
  * @retval None
  */
__weak void HAL_PCD_SOFCallback(PCD_HandleTypeDef *hpcd)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hpcd);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_PCD_SOFCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  USB Reset callback.
  * @param  hpcd PCD handle
  * @retval None
  */
__weak void HAL_PCD_ResetCallback(PCD_HandleTypeDef *hpcd)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hpcd);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_PCD_ResetCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Suspend event callback.
  * @param  hpcd PCD handle
  * @retval None
  */
__weak void HAL_PCD_SuspendCallback(PCD_HandleTypeDef *hpcd)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hpcd);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_PCD_SuspendCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Resume event callback.
  * @param  hpcd PCD handle
  * @retval None
  */
__weak void HAL_PCD_ResumeCallback(PCD_HandleTypeDef *hpcd)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hpcd);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_PCD_ResumeCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Incomplete ISO OUT callback.
  * @param  hpcd PCD handle
  * @param  epnum endpoint number
  * @retval None
  */
__weak void HAL_PCD_ISOOUTIncompleteCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hpcd);                                         /* 执行语句 */
  UNUSED(epnum);                                        /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_PCD_ISOOUTIncompleteCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Incomplete ISO IN callback.
  * @param  hpcd PCD handle
  * @param  epnum endpoint number
  * @retval None
  */
__weak void HAL_PCD_ISOINIncompleteCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hpcd);                                         /* 执行语句 */
  UNUSED(epnum);                                        /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_PCD_ISOINIncompleteCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Connection event callback.
  * @param  hpcd PCD handle
  * @retval None
  */
__weak void HAL_PCD_ConnectCallback(PCD_HandleTypeDef *hpcd)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hpcd);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_PCD_ConnectCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Disconnection event callback.
  * @param  hpcd PCD handle
  * @retval None
  */
__weak void HAL_PCD_DisconnectCallback(PCD_HandleTypeDef *hpcd)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hpcd);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_PCD_DisconnectCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup PCD_Exported_Functions_Group3 Peripheral Control functions
  *  @brief   management functions
  *
@verbatim
 ===============================================================================
                      ##### Peripheral Control functions #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to control the PCD data
    transfers.

@endverbatim
  * @{
  */

/**
  * @brief  Connect the USB device
  * @param  hpcd PCD handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCD_DevConnect(PCD_HandleTypeDef *hpcd)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
#if defined (USB_OTG_FS) || defined (USB_OTG_HS)        /* 条件编译 */
  USB_OTG_GlobalTypeDef *USBx = hpcd->Instance;         /* 赋值或初始化 */
#endif /* defined (USB_OTG_FS) || defined (USB_OTG_HS) */

  __HAL_LOCK(hpcd);                                     /* HAL辅助宏/函数 */
#if defined (USB_OTG_FS) || defined (USB_OTG_HS)        /* 条件编译 */
  if ((hpcd->Init.battery_charging_enable == 1U) &&     /* 条件判断 */
      (hpcd->Init.phy_itface != USB_OTG_ULPI_PHY))      /* 源代码 */
  {                                                     /* 代码块开始 */
    /* Enable USB Transceiver */
    USBx->GCCFG |= USB_OTG_GCCFG_PWRDWN;                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
#endif /* defined (USB_OTG_FS) || defined (USB_OTG_HS) */
  (void)USB_DevConnect(hpcd->Instance);                 /* 执行语句 */
  __HAL_UNLOCK(hpcd);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Disconnect the USB device.
  * @param  hpcd PCD handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCD_DevDisconnect(PCD_HandleTypeDef *hpcd)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
#if defined (USB_OTG_FS) || defined (USB_OTG_HS)        /* 条件编译 */
  USB_OTG_GlobalTypeDef *USBx = hpcd->Instance;         /* 赋值或初始化 */
#endif /* defined (USB_OTG_FS) || defined (USB_OTG_HS) */

  __HAL_LOCK(hpcd);                                     /* HAL辅助宏/函数 */
  (void)USB_DevDisconnect(hpcd->Instance);              /* 执行语句 */

#if defined (USB_OTG_FS) || defined (USB_OTG_HS)        /* 条件编译 */
  if ((hpcd->Init.battery_charging_enable == 1U) &&     /* 条件判断 */
      (hpcd->Init.phy_itface != USB_OTG_ULPI_PHY))      /* 源代码 */
  {                                                     /* 代码块开始 */
    /* Disable USB Transceiver */
    USBx->GCCFG &= ~(USB_OTG_GCCFG_PWRDWN);             /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
#endif /* defined (USB_OTG_FS) || defined (USB_OTG_HS) */

  __HAL_UNLOCK(hpcd);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Set the USB Device address.
  * @param  hpcd PCD handle
  * @param  address new device address
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCD_SetAddress(PCD_HandleTypeDef *hpcd, uint8_t address)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  __HAL_LOCK(hpcd);                                     /* HAL辅助宏/函数 */
  hpcd->USB_Address = address;                          /* 赋值或初始化 */
  (void)USB_SetDevAddress(hpcd->Instance, address);     /* 执行语句 */
  __HAL_UNLOCK(hpcd);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */
/**
  * @brief  Open and configure an endpoint.
  * @param  hpcd PCD handle
  * @param  ep_addr endpoint address
  * @param  ep_mps endpoint max packet size
  * @param  ep_type endpoint type
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCD_EP_Open(PCD_HandleTypeDef *hpcd, uint8_t ep_addr,  /* HAL状态类型函数 */
                                  uint16_t ep_mps, uint8_t ep_type)  /* 16位无符号变量/函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef  ret = HAL_OK;                      /* HAL状态类型函数 */
  PCD_EPTypeDef *ep;                                    /* 执行语句 */

  if ((ep_addr & 0x80U) == 0x80U)                       /* 条件判断 */
  {                                                     /* 代码块开始 */
    ep = &hpcd->IN_ep[ep_addr & EP_ADDR_MSK];           /* 赋值或初始化 */
    ep->is_in = 1U;                                     /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    ep = &hpcd->OUT_ep[ep_addr & EP_ADDR_MSK];          /* 赋值或初始化 */
    ep->is_in = 0U;                                     /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  ep->num = ep_addr & EP_ADDR_MSK;                      /* 赋值或初始化 */
  ep->maxpacket = ep_mps;                               /* 赋值或初始化 */
  ep->type = ep_type;                                   /* 赋值或初始化 */

  if (ep->is_in != 0U)                                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Assign a Tx FIFO */
    ep->tx_fifo_num = ep->num;                          /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  /* Set initial data PID. */
  if (ep_type == EP_TYPE_BULK)                          /* 条件判断 */
  {                                                     /* 代码块开始 */
    ep->data_pid_start = 0U;                            /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  __HAL_LOCK(hpcd);                                     /* HAL辅助宏/函数 */
  (void)USB_ActivateEndpoint(hpcd->Instance, ep);       /* 执行语句 */
  __HAL_UNLOCK(hpcd);                                   /* HAL辅助宏/函数 */

  return ret;                                           /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Deactivate an endpoint.
  * @param  hpcd PCD handle
  * @param  ep_addr endpoint address
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCD_EP_Close(PCD_HandleTypeDef *hpcd, uint8_t ep_addr)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  PCD_EPTypeDef *ep;                                    /* 执行语句 */

  if ((ep_addr & 0x80U) == 0x80U)                       /* 条件判断 */
  {                                                     /* 代码块开始 */
    ep = &hpcd->IN_ep[ep_addr & EP_ADDR_MSK];           /* 赋值或初始化 */
    ep->is_in = 1U;                                     /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    ep = &hpcd->OUT_ep[ep_addr & EP_ADDR_MSK];          /* 赋值或初始化 */
    ep->is_in = 0U;                                     /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  ep->num   = ep_addr & EP_ADDR_MSK;                    /* 赋值或初始化 */

  __HAL_LOCK(hpcd);                                     /* HAL辅助宏/函数 */
  (void)USB_DeactivateEndpoint(hpcd->Instance, ep);     /* 执行语句 */
  __HAL_UNLOCK(hpcd);                                   /* HAL辅助宏/函数 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */


/**
  * @brief  Receive an amount of data.
  * @param  hpcd PCD handle
  * @param  ep_addr endpoint address
  * @param  pBuf pointer to the reception buffer
  * @param  len amount of data to be received
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCD_EP_Receive(PCD_HandleTypeDef *hpcd, uint8_t ep_addr, uint8_t *pBuf, uint32_t len)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  PCD_EPTypeDef *ep;                                    /* 执行语句 */

  ep = &hpcd->OUT_ep[ep_addr & EP_ADDR_MSK];            /* 赋值或初始化 */

  /*setup and start the Xfer */
  ep->xfer_buff = pBuf;                                 /* 赋值或初始化 */
  ep->xfer_len = len;                                   /* 赋值或初始化 */
  ep->xfer_count = 0U;                                  /* 赋值或初始化 */
  ep->is_in = 0U;                                       /* 赋值或初始化 */
  ep->num = ep_addr & EP_ADDR_MSK;                      /* 赋值或初始化 */

  if (hpcd->Init.dma_enable == 1U)                      /* 条件判断 */
  {                                                     /* 代码块开始 */
    ep->dma_addr = (uint32_t)pBuf;                      /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if ((ep_addr & EP_ADDR_MSK) == 0U)                    /* 条件判断 */
  {                                                     /* 代码块开始 */
    (void)USB_EP0StartXfer(hpcd->Instance, ep, (uint8_t)hpcd->Init.dma_enable);  /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    (void)USB_EPStartXfer(hpcd->Instance, ep, (uint8_t)hpcd->Init.dma_enable);  /* 执行语句 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Get Received Data Size
  * @param  hpcd PCD handle
  * @param  ep_addr endpoint address
  * @retval Data Size
  */
uint32_t HAL_PCD_EP_GetRxCount(PCD_HandleTypeDef *hpcd, uint8_t ep_addr)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  return hpcd->OUT_ep[ep_addr & EP_ADDR_MSK].xfer_count;  /* 返回值 */
}                                                       /* 代码块结束 */
/**
  * @brief  Send an amount of data
  * @param  hpcd PCD handle
  * @param  ep_addr endpoint address
  * @param  pBuf pointer to the transmission buffer
  * @param  len amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCD_EP_Transmit(PCD_HandleTypeDef *hpcd, uint8_t ep_addr, uint8_t *pBuf, uint32_t len)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  PCD_EPTypeDef *ep;                                    /* 执行语句 */

  ep = &hpcd->IN_ep[ep_addr & EP_ADDR_MSK];             /* 赋值或初始化 */

  /*setup and start the Xfer */
  ep->xfer_buff = pBuf;                                 /* 赋值或初始化 */
  ep->xfer_len = len;                                   /* 赋值或初始化 */
  ep->xfer_count = 0U;                                  /* 赋值或初始化 */
  ep->is_in = 1U;                                       /* 赋值或初始化 */
  ep->num = ep_addr & EP_ADDR_MSK;                      /* 赋值或初始化 */

  if (hpcd->Init.dma_enable == 1U)                      /* 条件判断 */
  {                                                     /* 代码块开始 */
    ep->dma_addr = (uint32_t)pBuf;                      /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if ((ep_addr & EP_ADDR_MSK) == 0U)                    /* 条件判断 */
  {                                                     /* 代码块开始 */
    (void)USB_EP0StartXfer(hpcd->Instance, ep, (uint8_t)hpcd->Init.dma_enable);  /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    (void)USB_EPStartXfer(hpcd->Instance, ep, (uint8_t)hpcd->Init.dma_enable);  /* 执行语句 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Set a STALL condition over an endpoint
  * @param  hpcd PCD handle
  * @param  ep_addr endpoint address
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCD_EP_SetStall(PCD_HandleTypeDef *hpcd, uint8_t ep_addr)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  PCD_EPTypeDef *ep;                                    /* 执行语句 */

  if (((uint32_t)ep_addr & EP_ADDR_MSK) > hpcd->Init.dev_endpoints)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  if ((0x80U & ep_addr) == 0x80U)                       /* 条件判断 */
  {                                                     /* 代码块开始 */
    ep = &hpcd->IN_ep[ep_addr & EP_ADDR_MSK];           /* 赋值或初始化 */
    ep->is_in = 1U;                                     /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    ep = &hpcd->OUT_ep[ep_addr];                        /* 赋值或初始化 */
    ep->is_in = 0U;                                     /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  ep->is_stall = 1U;                                    /* 赋值或初始化 */
  ep->num = ep_addr & EP_ADDR_MSK;                      /* 赋值或初始化 */

  __HAL_LOCK(hpcd);                                     /* HAL辅助宏/函数 */

  (void)USB_EPSetStall(hpcd->Instance, ep);             /* 执行语句 */

  if ((ep_addr & EP_ADDR_MSK) == 0U)                    /* 条件判断 */
  {                                                     /* 代码块开始 */
    (void)USB_EP0_OutStart(hpcd->Instance, (uint8_t)hpcd->Init.dma_enable, (uint8_t *)hpcd->Setup);  /* 执行语句 */
  }                                                     /* 代码块结束 */

  __HAL_UNLOCK(hpcd);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Clear a STALL condition over in an endpoint
  * @param  hpcd PCD handle
  * @param  ep_addr endpoint address
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCD_EP_ClrStall(PCD_HandleTypeDef *hpcd, uint8_t ep_addr)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  PCD_EPTypeDef *ep;                                    /* 执行语句 */

  if (((uint32_t)ep_addr & 0x0FU) > hpcd->Init.dev_endpoints)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  if ((0x80U & ep_addr) == 0x80U)                       /* 条件判断 */
  {                                                     /* 代码块开始 */
    ep = &hpcd->IN_ep[ep_addr & EP_ADDR_MSK];           /* 赋值或初始化 */
    ep->is_in = 1U;                                     /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    ep = &hpcd->OUT_ep[ep_addr & EP_ADDR_MSK];          /* 赋值或初始化 */
    ep->is_in = 0U;                                     /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  ep->is_stall = 0U;                                    /* 赋值或初始化 */
  ep->num = ep_addr & EP_ADDR_MSK;                      /* 赋值或初始化 */

  __HAL_LOCK(hpcd);                                     /* HAL辅助宏/函数 */
  (void)USB_EPClearStall(hpcd->Instance, ep);           /* 执行语句 */
  __HAL_UNLOCK(hpcd);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Flush an endpoint
  * @param  hpcd PCD handle
  * @param  ep_addr endpoint address
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCD_EP_Flush(PCD_HandleTypeDef *hpcd, uint8_t ep_addr)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  __HAL_LOCK(hpcd);                                     /* HAL辅助宏/函数 */

  if ((ep_addr & 0x80U) == 0x80U)                       /* 条件判断 */
  {                                                     /* 代码块开始 */
    (void)USB_FlushTxFifo(hpcd->Instance, (uint32_t)ep_addr & EP_ADDR_MSK);  /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    (void)USB_FlushRxFifo(hpcd->Instance);              /* 执行语句 */
  }                                                     /* 代码块结束 */

  __HAL_UNLOCK(hpcd);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Activate remote wakeup signalling
  * @param  hpcd PCD handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCD_ActivateRemoteWakeup(PCD_HandleTypeDef *hpcd)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  return (USB_ActivateRemoteWakeup(hpcd->Instance));    /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  De-activate remote wakeup signalling.
  * @param  hpcd PCD handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCD_DeActivateRemoteWakeup(PCD_HandleTypeDef *hpcd)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  return (USB_DeActivateRemoteWakeup(hpcd->Instance));  /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup PCD_Exported_Functions_Group4 Peripheral State functions
  *  @brief   Peripheral State functions
  *
@verbatim
 ===============================================================================
                      ##### Peripheral State functions #####
 ===============================================================================
    [..]
    This subsection permits to get in run-time the status of the peripheral
    and the data flow.

@endverbatim
  * @{
  */

/**
  * @brief  Return the PCD handle state.
  * @param  hpcd PCD handle
  * @retval HAL state
  */
PCD_StateTypeDef HAL_PCD_GetState(PCD_HandleTypeDef *hpcd)  /* 源代码 */
{                                                       /* 代码块开始 */
  return hpcd->State;                                   /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/
/** @addtogroup PCD_Private_Functions
  * @{
  */
#if defined (USB_OTG_FS) || defined (USB_OTG_HS)        /* 条件编译 */
/**
  * @brief  Check FIFO for the next packet to be loaded.
  * @param  hpcd PCD handle
  * @param  epnum endpoint number
  * @retval HAL status
  */
static HAL_StatusTypeDef PCD_WriteEmptyTxFifo(PCD_HandleTypeDef *hpcd, uint32_t epnum)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  USB_OTG_GlobalTypeDef *USBx = hpcd->Instance;         /* 赋值或初始化 */
  uint32_t USBx_BASE = (uint32_t)USBx;                  /* 32位无符号变量/函数 */
  USB_OTG_EPTypeDef *ep;                                /* 执行语句 */
  uint32_t len;                                         /* 32位无符号变量/函数 */
  uint32_t len32b;                                      /* 32位无符号变量/函数 */
  uint32_t fifoemptymsk;                                /* 32位无符号变量/函数 */

  ep = &hpcd->IN_ep[epnum];                             /* 赋值或初始化 */

  if (ep->xfer_count > ep->xfer_len)                    /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  len = ep->xfer_len - ep->xfer_count;                  /* 赋值或初始化 */

  if (len > ep->maxpacket)                              /* 条件判断 */
  {                                                     /* 代码块开始 */
    len = ep->maxpacket;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  len32b = (len + 3U) / 4U;                             /* 赋值或初始化 */

  while (((USBx_INEP(epnum)->DTXFSTS & USB_OTG_DTXFSTS_INEPTFSAV) >= len32b) &&  /* while循环 */
         (ep->xfer_count < ep->xfer_len) && (ep->xfer_len != 0U))  /* 源代码 */
  {                                                     /* 代码块开始 */
    /* Write the FIFO */
    len = ep->xfer_len - ep->xfer_count;                /* 赋值或初始化 */

    if (len > ep->maxpacket)                            /* 条件判断 */
    {                                                   /* 代码块开始 */
      len = ep->maxpacket;                              /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    len32b = (len + 3U) / 4U;                           /* 赋值或初始化 */

    (void)USB_WritePacket(USBx, ep->xfer_buff, (uint8_t)epnum, (uint16_t)len,  /* 列表续行 */
                          (uint8_t)hpcd->Init.dma_enable);  /* 执行语句 */

    ep->xfer_buff  += len;                              /* 赋值或初始化 */
    ep->xfer_count += len;                              /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if (ep->xfer_len <= ep->xfer_count)                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    fifoemptymsk = (uint32_t)(0x1UL << (epnum & EP_ADDR_MSK));  /* 赋值或初始化 */
    USBx_DEVICE->DIEPEMPMSK &= ~fifoemptymsk;           /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */


/**
  * @brief  process EP OUT transfer complete interrupt.
  * @param  hpcd PCD handle
  * @param  epnum endpoint number
  * @retval HAL status
  */
static HAL_StatusTypeDef PCD_EP_OutXfrComplete_int(PCD_HandleTypeDef *hpcd, uint32_t epnum)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  USB_OTG_GlobalTypeDef *USBx = hpcd->Instance;         /* 赋值或初始化 */
  uint32_t USBx_BASE = (uint32_t)USBx;                  /* 32位无符号变量/函数 */
  uint32_t gSNPSiD = *(__IO uint32_t *)(&USBx->CID + 0x1U);  /* 32位无符号变量/函数 */
  uint32_t DoepintReg = USBx_OUTEP(epnum)->DOEPINT;     /* 32位无符号变量/函数 */

  if (hpcd->Init.dma_enable == 1U)                      /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((DoepintReg & USB_OTG_DOEPINT_STUP) == USB_OTG_DOEPINT_STUP) /* Class C */
    {                                                   /* 代码块开始 */
      /* StupPktRcvd = 1 this is a setup packet */
      if ((gSNPSiD > USB_OTG_CORE_ID_300A) &&           /* 条件判断 */
          ((DoepintReg & USB_OTG_DOEPINT_STPKTRX) == USB_OTG_DOEPINT_STPKTRX))  /* 源代码 */
      {                                                 /* 代码块开始 */
        CLEAR_OUT_EP_INTR(epnum, USB_OTG_DOEPINT_STPKTRX);  /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else if ((DoepintReg & USB_OTG_DOEPINT_OTEPSPR) == USB_OTG_DOEPINT_OTEPSPR) /* Class E */
    {                                                   /* 代码块开始 */
      CLEAR_OUT_EP_INTR(epnum, USB_OTG_DOEPINT_OTEPSPR);  /* 执行语句 */
    }                                                   /* 代码块结束 */
    else if ((DoepintReg & (USB_OTG_DOEPINT_STUP | USB_OTG_DOEPINT_OTEPSPR)) == 0U)  /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* StupPktRcvd = 1 this is a setup packet */
      if ((gSNPSiD > USB_OTG_CORE_ID_300A) &&           /* 条件判断 */
          ((DoepintReg & USB_OTG_DOEPINT_STPKTRX) == USB_OTG_DOEPINT_STPKTRX))  /* 源代码 */
      {                                                 /* 代码块开始 */
        CLEAR_OUT_EP_INTR(epnum, USB_OTG_DOEPINT_STPKTRX);  /* 执行语句 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* out data packet received over EP0 */
        hpcd->OUT_ep[epnum].xfer_count =                /* 源代码 */
          hpcd->OUT_ep[epnum].maxpacket -               /* 源代码 */
          (USBx_OUTEP(epnum)->DOEPTSIZ & USB_OTG_DOEPTSIZ_XFRSIZ);  /* 执行语句 */

        hpcd->OUT_ep[epnum].xfer_buff += hpcd->OUT_ep[epnum].maxpacket;  /* 赋值或初始化 */

        if ((epnum == 0U) && (hpcd->OUT_ep[epnum].xfer_len == 0U))  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* this is ZLP, so prepare EP0 for next setup */
          (void)USB_EP0_OutStart(hpcd->Instance, 1U, (uint8_t *)hpcd->Setup);  /* 执行语句 */
        }                                               /* 代码块结束 */
#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
        hpcd->DataOutStageCallback(hpcd, (uint8_t)epnum);  /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_PCD_DataOutStageCallback(hpcd, (uint8_t)epnum);  /* HAL库函数调用 */
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* ... */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    if (gSNPSiD == USB_OTG_CORE_ID_310A)                /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* StupPktRcvd = 1 this is a setup packet */
      if ((DoepintReg & USB_OTG_DOEPINT_STPKTRX) == USB_OTG_DOEPINT_STPKTRX)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        CLEAR_OUT_EP_INTR(epnum, USB_OTG_DOEPINT_STPKTRX);  /* 执行语句 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        if ((DoepintReg & USB_OTG_DOEPINT_OTEPSPR) == USB_OTG_DOEPINT_OTEPSPR)  /* 条件判断 */
        {                                               /* 代码块开始 */
          CLEAR_OUT_EP_INTR(epnum, USB_OTG_DOEPINT_OTEPSPR);  /* 执行语句 */
        }                                               /* 代码块结束 */

#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
        hpcd->DataOutStageCallback(hpcd, (uint8_t)epnum);  /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_PCD_DataOutStageCallback(hpcd, (uint8_t)epnum);  /* HAL库函数调用 */
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      if ((epnum == 0U) && (hpcd->OUT_ep[epnum].xfer_len == 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* this is ZLP, so prepare EP0 for next setup */
        (void)USB_EP0_OutStart(hpcd->Instance, 0U, (uint8_t *)hpcd->Setup);  /* 执行语句 */
      }                                                 /* 代码块结束 */

#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
      hpcd->DataOutStageCallback(hpcd, (uint8_t)epnum);  /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_PCD_DataOutStageCallback(hpcd, (uint8_t)epnum);  /* HAL库函数调用 */
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */


/**
  * @brief  process EP OUT setup packet received interrupt.
  * @param  hpcd PCD handle
  * @param  epnum endpoint number
  * @retval HAL status
  */
static HAL_StatusTypeDef PCD_EP_OutSetupPacket_int(PCD_HandleTypeDef *hpcd, uint32_t epnum)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  USB_OTG_GlobalTypeDef *USBx = hpcd->Instance;         /* 赋值或初始化 */
  uint32_t USBx_BASE = (uint32_t)USBx;                  /* 32位无符号变量/函数 */
  uint32_t gSNPSiD = *(__IO uint32_t *)(&USBx->CID + 0x1U);  /* 32位无符号变量/函数 */
  uint32_t DoepintReg = USBx_OUTEP(epnum)->DOEPINT;     /* 32位无符号变量/函数 */

  if ((gSNPSiD > USB_OTG_CORE_ID_300A) &&               /* 条件判断 */
      ((DoepintReg & USB_OTG_DOEPINT_STPKTRX) == USB_OTG_DOEPINT_STPKTRX))  /* 源代码 */
  {                                                     /* 代码块开始 */
    CLEAR_OUT_EP_INTR(epnum, USB_OTG_DOEPINT_STPKTRX);  /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Inform the upper layer that a setup packet is available */
#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
  hpcd->SetupStageCallback(hpcd);                       /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_PCD_SetupStageCallback(hpcd);                     /* HAL库函数调用 */
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */

  if ((gSNPSiD > USB_OTG_CORE_ID_300A) && (hpcd->Init.dma_enable == 1U))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    (void)USB_EP0_OutStart(hpcd->Instance, 1U, (uint8_t *)hpcd->Setup);  /* 执行语句 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* defined (USB_OTG_FS) || defined (USB_OTG_HS) */


/**
  * @}
  */
#endif /* defined (USB_OTG_FS) || defined (USB_OTG_HS) */
#endif /* HAL_PCD_MODULE_ENABLED */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
