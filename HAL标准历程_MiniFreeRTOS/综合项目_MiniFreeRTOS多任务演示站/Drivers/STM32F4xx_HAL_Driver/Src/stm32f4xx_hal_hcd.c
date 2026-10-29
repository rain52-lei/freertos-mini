/* 文件: stm32f4xx_hal_hcd.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_hcd.c
  * @author  MCD Application Team
  * @brief   HCD HAL module driver.
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
    (#)Declare a HCD_HandleTypeDef handle structure, for example:
       HCD_HandleTypeDef  hhcd;

    (#)Fill parameters of Init structure in HCD handle

    (#)Call HAL_HCD_Init() API to initialize the HCD peripheral (Core, Host core, ...)

    (#)Initialize the HCD low level resources through the HAL_HCD_MspInit() API:
        (##) Enable the HCD/USB Low Level interface clock using the following macros
             (+++) __HAL_RCC_USB_OTG_FS_CLK_ENABLE();
             (+++) __HAL_RCC_USB_OTG_HS_CLK_ENABLE(); (For High Speed Mode)
             (+++) __HAL_RCC_USB_OTG_HS_ULPI_CLK_ENABLE(); (For High Speed Mode)

        (##) Initialize the related GPIO clocks
        (##) Configure HCD pin-out
        (##) Configure HCD NVIC interrupt

    (#)Associate the Upper USB Host stack to the HAL HCD Driver:
        (##) hhcd.pData = phost;

    (#)Enable HCD transmission and reception:
        (##) HAL_HCD_Start();

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

#ifdef HAL_HCD_MODULE_ENABLED                           /* 条件编译 */

#if defined (USB_OTG_FS) || defined (USB_OTG_HS)        /* 条件编译 */

/** @defgroup HCD HCD
  * @brief HCD HAL module driver
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/** @defgroup HCD_Private_Functions HCD Private Functions
  * @{
  */
static void HCD_HC_IN_IRQHandler(HCD_HandleTypeDef *hhcd, uint8_t chnum);  /* 静态函数 */
static void HCD_HC_OUT_IRQHandler(HCD_HandleTypeDef *hhcd, uint8_t chnum);  /* 静态函数 */
static void HCD_RXQLVL_IRQHandler(HCD_HandleTypeDef *hhcd);  /* 静态函数 */
static void HCD_Port_IRQHandler(HCD_HandleTypeDef *hhcd);  /* 静态函数 */
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup HCD_Exported_Functions HCD Exported Functions
  * @{
  */

/** @defgroup HCD_Exported_Functions_Group1 Initialization and de-initialization functions
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
  * @brief  Initialize the host driver.
  * @param  hhcd HCD handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_HCD_Init(HCD_HandleTypeDef *hhcd)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  USB_OTG_GlobalTypeDef *USBx;                          /* 执行语句 */

  /* Check the HCD handle allocation */
  if (hhcd == NULL)                                     /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check the parameters */
  assert_param(IS_HCD_ALL_INSTANCE(hhcd->Instance));    /* 执行语句 */

  USBx = hhcd->Instance;                                /* 赋值或初始化 */

  if (hhcd->State == HAL_HCD_STATE_RESET)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Allocate lock resource and initialize it */
    hhcd->Lock = HAL_UNLOCKED;                          /* 赋值或初始化 */

#if (USE_HAL_HCD_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
    hhcd->SOFCallback = HAL_HCD_SOF_Callback;           /* 赋值或初始化 */
    hhcd->ConnectCallback = HAL_HCD_Connect_Callback;   /* 赋值或初始化 */
    hhcd->DisconnectCallback = HAL_HCD_Disconnect_Callback;  /* 赋值或初始化 */
    hhcd->PortEnabledCallback = HAL_HCD_PortEnabled_Callback;  /* 赋值或初始化 */
    hhcd->PortDisabledCallback = HAL_HCD_PortDisabled_Callback;  /* 赋值或初始化 */
    hhcd->HC_NotifyURBChangeCallback = HAL_HCD_HC_NotifyURBChange_Callback;  /* 赋值或初始化 */

    if (hhcd->MspInitCallback == NULL)                  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hhcd->MspInitCallback = HAL_HCD_MspInit;          /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Init the low level hardware */
    hhcd->MspInitCallback(hhcd);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /* Init the low level hardware : GPIO, CLOCK, NVIC... */
    HAL_HCD_MspInit(hhcd);                              /* HAL库函数调用 */
#endif /* (USE_HAL_HCD_REGISTER_CALLBACKS) */
  }                                                     /* 代码块结束 */

  hhcd->State = HAL_HCD_STATE_BUSY;                     /* 赋值或初始化 */

  /* Disable DMA mode for FS instance */
  if ((USBx->CID & (0x1U << 8)) == 0U)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hhcd->Init.dma_enable = 0U;                         /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Disable the Interrupts */
  __HAL_HCD_DISABLE(hhcd);                              /* HAL辅助宏/函数 */

  /* Init the Core (common init.) */
  (void)USB_CoreInit(hhcd->Instance, hhcd->Init);       /* 执行语句 */

  /* Force Host Mode*/
  (void)USB_SetCurrentMode(hhcd->Instance, USB_HOST_MODE);  /* 执行语句 */

  /* Init Host */
  (void)USB_HostInit(hhcd->Instance, hhcd->Init);       /* 执行语句 */

  hhcd->State = HAL_HCD_STATE_READY;                    /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initialize a host channel.
  * @param  hhcd HCD handle
  * @param  ch_num Channel number.
  *         This parameter can be a value from 1 to 15
  * @param  epnum Endpoint number.
  *          This parameter can be a value from 1 to 15
  * @param  dev_address Current device address
  *          This parameter can be a value from 0 to 255
  * @param  speed Current device speed.
  *          This parameter can be one of these values:
  *            HCD_DEVICE_SPEED_HIGH: High speed mode,
  *            HCD_DEVICE_SPEED_FULL: Full speed mode,
  *            HCD_DEVICE_SPEED_LOW: Low speed mode
  * @param  ep_type Endpoint Type.
  *          This parameter can be one of these values:
  *            EP_TYPE_CTRL: Control type,
  *            EP_TYPE_ISOC: Isochronous type,
  *            EP_TYPE_BULK: Bulk type,
  *            EP_TYPE_INTR: Interrupt type
  * @param  mps Max Packet Size.
  *          This parameter can be a value from 0 to32K
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_HCD_HC_Init(HCD_HandleTypeDef *hhcd,  /* HAL状态类型函数 */
                                  uint8_t ch_num,       /* 8位无符号变量/函数 */
                                  uint8_t epnum,        /* 8位无符号变量/函数 */
                                  uint8_t dev_address,  /* 8位无符号变量/函数 */
                                  uint8_t speed,        /* 8位无符号变量/函数 */
                                  uint8_t ep_type,      /* 8位无符号变量/函数 */
                                  uint16_t mps)         /* 16位无符号变量/函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status;                             /* HAL状态类型函数 */

  __HAL_LOCK(hhcd);                                     /* HAL辅助宏/函数 */
  hhcd->hc[ch_num].do_ping = 0U;                        /* 赋值或初始化 */
  hhcd->hc[ch_num].dev_addr = dev_address;              /* 赋值或初始化 */
  hhcd->hc[ch_num].max_packet = mps;                    /* 赋值或初始化 */
  hhcd->hc[ch_num].ch_num = ch_num;                     /* 赋值或初始化 */
  hhcd->hc[ch_num].ep_type = ep_type;                   /* 赋值或初始化 */
  hhcd->hc[ch_num].ep_num = epnum & 0x7FU;              /* 赋值或初始化 */

  if ((epnum & 0x80U) == 0x80U)                         /* 条件判断 */
  {                                                     /* 代码块开始 */
    hhcd->hc[ch_num].ep_is_in = 1U;                     /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    hhcd->hc[ch_num].ep_is_in = 0U;                     /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  hhcd->hc[ch_num].speed = speed;                       /* 赋值或初始化 */

  status =  USB_HC_Init(hhcd->Instance,                 /* 列表续行 */
                        ch_num,                         /* 列表续行 */
                        epnum,                          /* 列表续行 */
                        dev_address,                    /* 列表续行 */
                        speed,                          /* 列表续行 */
                        ep_type,                        /* 列表续行 */
                        mps);                           /* 执行语句 */
  __HAL_UNLOCK(hhcd);                                   /* HAL辅助宏/函数 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Halt a host channel.
  * @param  hhcd HCD handle
  * @param  ch_num Channel number.
  *         This parameter can be a value from 1 to 15
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_HCD_HC_Halt(HCD_HandleTypeDef *hhcd, uint8_t ch_num)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  __HAL_LOCK(hhcd);                                     /* HAL辅助宏/函数 */
  (void)USB_HC_Halt(hhcd->Instance, (uint8_t)ch_num);   /* 执行语句 */
  __HAL_UNLOCK(hhcd);                                   /* HAL辅助宏/函数 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  DeInitialize the host driver.
  * @param  hhcd HCD handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_HCD_DeInit(HCD_HandleTypeDef *hhcd)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the HCD handle allocation */
  if (hhcd == NULL)                                     /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  hhcd->State = HAL_HCD_STATE_BUSY;                     /* 赋值或初始化 */

#if (USE_HAL_HCD_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
  if (hhcd->MspDeInitCallback == NULL)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hhcd->MspDeInitCallback = HAL_HCD_MspDeInit; /* Legacy weak MspDeInit  */
  }                                                     /* 代码块结束 */

  /* DeInit the low level hardware */
  hhcd->MspDeInitCallback(hhcd);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /* DeInit the low level hardware: CLOCK, NVIC.*/
  HAL_HCD_MspDeInit(hhcd);                              /* HAL库函数调用 */
#endif /* USE_HAL_HCD_REGISTER_CALLBACKS */

  __HAL_HCD_DISABLE(hhcd);                              /* HAL辅助宏/函数 */

  hhcd->State = HAL_HCD_STATE_RESET;                    /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initialize the HCD MSP.
  * @param  hhcd HCD handle
  * @retval None
  */
__weak void  HAL_HCD_MspInit(HCD_HandleTypeDef *hhcd)   /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hhcd);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_HCD_MspInit could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  DeInitialize the HCD MSP.
  * @param  hhcd HCD handle
  * @retval None
  */
__weak void  HAL_HCD_MspDeInit(HCD_HandleTypeDef *hhcd)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hhcd);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_HCD_MspDeInit could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup HCD_Exported_Functions_Group2 Input and Output operation functions
  *  @brief   HCD IO operation functions
  *
@verbatim
 ===============================================================================
                      ##### IO operation functions #####
 ===============================================================================
 [..] This subsection provides a set of functions allowing to manage the USB Host Data
    Transfer

@endverbatim
  * @{
  */

/**
  * @brief  Submit a new URB for processing.
  * @param  hhcd HCD handle
  * @param  ch_num Channel number.
  *         This parameter can be a value from 1 to 15
  * @param  direction Channel number.
  *          This parameter can be one of these values:
  *           0 : Output / 1 : Input
  * @param  ep_type Endpoint Type.
  *          This parameter can be one of these values:
  *            EP_TYPE_CTRL: Control type/
  *            EP_TYPE_ISOC: Isochronous type/
  *            EP_TYPE_BULK: Bulk type/
  *            EP_TYPE_INTR: Interrupt type/
  * @param  token Endpoint Type.
  *          This parameter can be one of these values:
  *            0: HC_PID_SETUP / 1: HC_PID_DATA1
  * @param  pbuff pointer to URB data
  * @param  length Length of URB data
  * @param  do_ping activate do ping protocol (for high speed only).
  *          This parameter can be one of these values:
  *           0 : do ping inactive / 1 : do ping active
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_HCD_HC_SubmitRequest(HCD_HandleTypeDef *hhcd,  /* HAL状态类型函数 */
                                           uint8_t ch_num,  /* 8位无符号变量/函数 */
                                           uint8_t direction,  /* 8位无符号变量/函数 */
                                           uint8_t ep_type,  /* 8位无符号变量/函数 */
                                           uint8_t token,  /* 8位无符号变量/函数 */
                                           uint8_t *pbuff,  /* 8位无符号变量/函数 */
                                           uint16_t length,  /* 16位无符号变量/函数 */
                                           uint8_t do_ping)  /* 8位无符号变量/函数 */
{                                                       /* 代码块开始 */
  hhcd->hc[ch_num].ep_is_in = direction;                /* 赋值或初始化 */
  hhcd->hc[ch_num].ep_type  = ep_type;                  /* 赋值或初始化 */

  if (token == 0U)                                      /* 条件判断 */
  {                                                     /* 代码块开始 */
    hhcd->hc[ch_num].data_pid = HC_PID_SETUP;           /* 赋值或初始化 */
    hhcd->hc[ch_num].do_ping = do_ping;                 /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    hhcd->hc[ch_num].data_pid = HC_PID_DATA1;           /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Manage Data Toggle */
  switch (ep_type)                                      /* switch分支 */
  {                                                     /* 代码块开始 */
    case EP_TYPE_CTRL:                                  /* case标签 */
      if ((token == 1U) && (direction == 0U)) /*send data */
      {                                                 /* 代码块开始 */
        if (length == 0U)                               /* 条件判断 */
        {                                               /* 代码块开始 */
          /* For Status OUT stage, Length==0, Status Out PID = 1 */
          hhcd->hc[ch_num].toggle_out = 1U;             /* 赋值或初始化 */
        }                                               /* 代码块结束 */

        /* Set the Data Toggle bit as per the Flag */
        if (hhcd->hc[ch_num].toggle_out == 0U)          /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Put the PID 0 */
          hhcd->hc[ch_num].data_pid = HC_PID_DATA0;     /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          /* Put the PID 1 */
          hhcd->hc[ch_num].data_pid = HC_PID_DATA1;     /* 赋值或初始化 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */

    case EP_TYPE_BULK:                                  /* case标签 */
      if (direction == 0U)                              /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Set the Data Toggle bit as per the Flag */
        if (hhcd->hc[ch_num].toggle_out == 0U)          /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Put the PID 0 */
          hhcd->hc[ch_num].data_pid = HC_PID_DATA0;     /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          /* Put the PID 1 */
          hhcd->hc[ch_num].data_pid = HC_PID_DATA1;     /* 赋值或初始化 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        if (hhcd->hc[ch_num].toggle_in == 0U)           /* 条件判断 */
        {                                               /* 代码块开始 */
          hhcd->hc[ch_num].data_pid = HC_PID_DATA0;     /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          hhcd->hc[ch_num].data_pid = HC_PID_DATA1;     /* 赋值或初始化 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */

      break;                                            /* 跳出 */
    case EP_TYPE_INTR:                                  /* case标签 */
      if (direction == 0U)                              /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Set the Data Toggle bit as per the Flag */
        if (hhcd->hc[ch_num].toggle_out == 0U)          /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Put the PID 0 */
          hhcd->hc[ch_num].data_pid = HC_PID_DATA0;     /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          /* Put the PID 1 */
          hhcd->hc[ch_num].data_pid = HC_PID_DATA1;     /* 赋值或初始化 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        if (hhcd->hc[ch_num].toggle_in == 0U)           /* 条件判断 */
        {                                               /* 代码块开始 */
          hhcd->hc[ch_num].data_pid = HC_PID_DATA0;     /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          hhcd->hc[ch_num].data_pid = HC_PID_DATA1;     /* 赋值或初始化 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */

    case EP_TYPE_ISOC:                                  /* case标签 */
      hhcd->hc[ch_num].data_pid = HC_PID_DATA0;         /* 赋值或初始化 */
      break;                                            /* 跳出 */

    default:                                            /* default分支 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  hhcd->hc[ch_num].xfer_buff = pbuff;                   /* 赋值或初始化 */
  hhcd->hc[ch_num].xfer_len  = length;                  /* 赋值或初始化 */
  hhcd->hc[ch_num].urb_state = URB_IDLE;                /* 赋值或初始化 */
  hhcd->hc[ch_num].xfer_count = 0U;                     /* 赋值或初始化 */
  hhcd->hc[ch_num].ch_num = ch_num;                     /* 赋值或初始化 */
  hhcd->hc[ch_num].state = HC_IDLE;                     /* 赋值或初始化 */

  return USB_HC_StartXfer(hhcd->Instance, &hhcd->hc[ch_num], (uint8_t)hhcd->Init.dma_enable);  /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Handle HCD interrupt request.
  * @param  hhcd HCD handle
  * @retval None
  */
void HAL_HCD_IRQHandler(HCD_HandleTypeDef *hhcd)        /* 中断服务函数 */
{                                                       /* 代码块开始 */
  USB_OTG_GlobalTypeDef *USBx = hhcd->Instance;         /* 赋值或初始化 */
  uint32_t USBx_BASE = (uint32_t)USBx;                  /* 32位无符号变量/函数 */
  uint32_t i, interrupt;                                /* 32位无符号变量/函数 */

  /* Ensure that we are in device mode */
  if (USB_GetMode(hhcd->Instance) == USB_OTG_MODE_HOST)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Avoid spurious interrupt */
    if (__HAL_HCD_IS_INVALID_INTERRUPT(hhcd))           /* 条件判断 */
    {                                                   /* 代码块开始 */
      return;                                           /* 返回 */
    }                                                   /* 代码块结束 */

    if (__HAL_HCD_GET_FLAG(hhcd, USB_OTG_GINTSTS_PXFR_INCOMPISOOUT))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Incorrect mode, acknowledge the interrupt */
      __HAL_HCD_CLEAR_FLAG(hhcd, USB_OTG_GINTSTS_PXFR_INCOMPISOOUT);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    if (__HAL_HCD_GET_FLAG(hhcd, USB_OTG_GINTSTS_IISOIXFR))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Incorrect mode, acknowledge the interrupt */
      __HAL_HCD_CLEAR_FLAG(hhcd, USB_OTG_GINTSTS_IISOIXFR);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    if (__HAL_HCD_GET_FLAG(hhcd, USB_OTG_GINTSTS_PTXFE))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Incorrect mode, acknowledge the interrupt */
      __HAL_HCD_CLEAR_FLAG(hhcd, USB_OTG_GINTSTS_PTXFE);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    if (__HAL_HCD_GET_FLAG(hhcd, USB_OTG_GINTSTS_MMIS))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Incorrect mode, acknowledge the interrupt */
      __HAL_HCD_CLEAR_FLAG(hhcd, USB_OTG_GINTSTS_MMIS);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Handle Host Disconnect Interrupts */
    if (__HAL_HCD_GET_FLAG(hhcd, USB_OTG_GINTSTS_DISCINT))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      __HAL_HCD_CLEAR_FLAG(hhcd, USB_OTG_GINTSTS_DISCINT);  /* HAL辅助宏/函数 */

      if ((USBx_HPRT0 & USB_OTG_HPRT_PCSTS) == 0U)      /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Handle Host Port Disconnect Interrupt */
#if (USE_HAL_HCD_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
        hhcd->DisconnectCallback(hhcd);                 /* 执行语句 */
#else                                                   /* 条件编译否则 */
        HAL_HCD_Disconnect_Callback(hhcd);              /* HAL库函数调用 */
#endif /* USE_HAL_HCD_REGISTER_CALLBACKS */

        (void)USB_InitFSLSPClkSel(hhcd->Instance, HCFG_48_MHZ);  /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* Handle Host Port Interrupts */
    if (__HAL_HCD_GET_FLAG(hhcd, USB_OTG_GINTSTS_HPRTINT))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      HCD_Port_IRQHandler(hhcd);                        /* 执行语句 */
    }                                                   /* 代码块结束 */

    /* Handle Host SOF Interrupt */
    if (__HAL_HCD_GET_FLAG(hhcd, USB_OTG_GINTSTS_SOF))  /* 条件判断 */
    {                                                   /* 代码块开始 */
#if (USE_HAL_HCD_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
      hhcd->SOFCallback(hhcd);                          /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_HCD_SOF_Callback(hhcd);                       /* HAL库函数调用 */
#endif /* USE_HAL_HCD_REGISTER_CALLBACKS */

      __HAL_HCD_CLEAR_FLAG(hhcd, USB_OTG_GINTSTS_SOF);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */

    /* Handle Rx Queue Level Interrupts */
    if ((__HAL_HCD_GET_FLAG(hhcd, USB_OTG_GINTSTS_RXFLVL)) != 0U)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      USB_MASK_INTERRUPT(hhcd->Instance, USB_OTG_GINTSTS_RXFLVL);  /* 执行语句 */

      HCD_RXQLVL_IRQHandler(hhcd);                      /* 执行语句 */

      USB_UNMASK_INTERRUPT(hhcd->Instance, USB_OTG_GINTSTS_RXFLVL);  /* 执行语句 */
    }                                                   /* 代码块结束 */

    /* Handle Host channel Interrupt */
    if (__HAL_HCD_GET_FLAG(hhcd, USB_OTG_GINTSTS_HCINT))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      interrupt = USB_HC_ReadInterrupt(hhcd->Instance);  /* 赋值或初始化 */
      for (i = 0U; i < hhcd->Init.Host_channels; i++)   /* for循环 */
      {                                                 /* 代码块开始 */
        if ((interrupt & (1UL << (i & 0xFU))) != 0U)    /* 条件判断 */
        {                                               /* 代码块开始 */
          if ((USBx_HC(i)->HCCHAR & USB_OTG_HCCHAR_EPDIR) == USB_OTG_HCCHAR_EPDIR)  /* 条件判断 */
          {                                             /* 代码块开始 */
            HCD_HC_IN_IRQHandler(hhcd, (uint8_t)i);     /* 执行语句 */
          }                                             /* 代码块结束 */
          else                                          /* 否则 */
          {                                             /* 代码块开始 */
            HCD_HC_OUT_IRQHandler(hhcd, (uint8_t)i);    /* 执行语句 */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      __HAL_HCD_CLEAR_FLAG(hhcd, USB_OTG_GINTSTS_HCINT);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */


/**
  * @brief  Handles HCD Wakeup interrupt request.
  * @param  hhcd HCD handle
  * @retval HAL status
  */
void HAL_HCD_WKUP_IRQHandler(HCD_HandleTypeDef *hhcd)   /* 中断服务函数 */
{                                                       /* 代码块开始 */
  UNUSED(hhcd);                                         /* 执行语句 */
}                                                       /* 代码块结束 */


/**
  * @brief  SOF callback.
  * @param  hhcd HCD handle
  * @retval None
  */
__weak void HAL_HCD_SOF_Callback(HCD_HandleTypeDef *hhcd)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hhcd);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_HCD_SOF_Callback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief Connection Event callback.
  * @param  hhcd HCD handle
  * @retval None
  */
__weak void HAL_HCD_Connect_Callback(HCD_HandleTypeDef *hhcd)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hhcd);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_HCD_Connect_Callback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Disconnection Event callback.
  * @param  hhcd HCD handle
  * @retval None
  */
__weak void HAL_HCD_Disconnect_Callback(HCD_HandleTypeDef *hhcd)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hhcd);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_HCD_Disconnect_Callback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Port Enabled  Event callback.
  * @param  hhcd HCD handle
  * @retval None
  */
__weak void HAL_HCD_PortEnabled_Callback(HCD_HandleTypeDef *hhcd)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hhcd);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_HCD_Disconnect_Callback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Port Disabled  Event callback.
  * @param  hhcd HCD handle
  * @retval None
  */
__weak void HAL_HCD_PortDisabled_Callback(HCD_HandleTypeDef *hhcd)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hhcd);                                         /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_HCD_Disconnect_Callback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Notify URB state change callback.
  * @param  hhcd HCD handle
  * @param  chnum Channel number.
  *         This parameter can be a value from 1 to 15
  * @param  urb_state:
  *          This parameter can be one of these values:
  *            URB_IDLE/
  *            URB_DONE/
  *            URB_NOTREADY/
  *            URB_NYET/
  *            URB_ERROR/
  *            URB_STALL/
  * @retval None
  */
__weak void HAL_HCD_HC_NotifyURBChange_Callback(HCD_HandleTypeDef *hhcd, uint8_t chnum, HCD_URBStateTypeDef urb_state)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hhcd);                                         /* 执行语句 */
  UNUSED(chnum);                                        /* 执行语句 */
  UNUSED(urb_state);                                    /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_HCD_HC_NotifyURBChange_Callback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

#if (USE_HAL_HCD_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
/**
  * @brief  Register a User USB HCD Callback
  *         To be used instead of the weak predefined callback
  * @param  hhcd USB HCD handle
  * @param  CallbackID ID of the callback to be registered
  *         This parameter can be one of the following values:
  *          @arg @ref HAL_HCD_SOF_CB_ID USB HCD SOF callback ID
  *          @arg @ref HAL_HCD_CONNECT_CB_ID USB HCD Connect callback ID
  *          @arg @ref HAL_HCD_DISCONNECT_CB_ID OTG HCD Disconnect callback ID
  *          @arg @ref HAL_HCD_PORT_ENABLED_CB_ID USB HCD Port Enable callback ID
  *          @arg @ref HAL_HCD_PORT_DISABLED_CB_ID USB HCD Port Disable callback ID
  *          @arg @ref HAL_HCD_MSPINIT_CB_ID MspDeInit callback ID
  *          @arg @ref HAL_HCD_MSPDEINIT_CB_ID MspDeInit callback ID
  * @param  pCallback pointer to the Callback function
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_HCD_RegisterCallback(HCD_HandleTypeDef *hhcd,  /* HAL状态类型函数 */
                                           HAL_HCD_CallbackIDTypeDef CallbackID,  /* HAL库函数调用 */
                                           pHCD_CallbackTypeDef pCallback)  /* 源代码 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  if (pCallback == NULL)                                /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hhcd->ErrorCode |= HAL_HCD_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  /* Process locked */
  __HAL_LOCK(hhcd);                                     /* HAL辅助宏/函数 */

  if (hhcd->State == HAL_HCD_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_HCD_SOF_CB_ID :                          /* case标签 */
        hhcd->SOFCallback = pCallback;                  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_HCD_CONNECT_CB_ID :                      /* case标签 */
        hhcd->ConnectCallback = pCallback;              /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_HCD_DISCONNECT_CB_ID :                   /* case标签 */
        hhcd->DisconnectCallback = pCallback;           /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_HCD_PORT_ENABLED_CB_ID :                 /* case标签 */
        hhcd->PortEnabledCallback = pCallback;          /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_HCD_PORT_DISABLED_CB_ID :                /* case标签 */
        hhcd->PortDisabledCallback = pCallback;         /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_HCD_MSPINIT_CB_ID :                      /* case标签 */
        hhcd->MspInitCallback = pCallback;              /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_HCD_MSPDEINIT_CB_ID :                    /* case标签 */
        hhcd->MspDeInitCallback = pCallback;            /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hhcd->ErrorCode |= HAL_HCD_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (hhcd->State == HAL_HCD_STATE_RESET)          /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_HCD_MSPINIT_CB_ID :                      /* case标签 */
        hhcd->MspInitCallback = pCallback;              /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_HCD_MSPDEINIT_CB_ID :                    /* case标签 */
        hhcd->MspDeInitCallback = pCallback;            /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hhcd->ErrorCode |= HAL_HCD_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hhcd->ErrorCode |= HAL_HCD_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hhcd);                                   /* HAL辅助宏/函数 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Unregister an USB HCD Callback
  *         USB HCD callback is redirected to the weak predefined callback
  * @param  hhcd USB HCD handle
  * @param  CallbackID ID of the callback to be unregistered
  *         This parameter can be one of the following values:
  *          @arg @ref HAL_HCD_SOF_CB_ID USB HCD SOF callback ID
  *          @arg @ref HAL_HCD_CONNECT_CB_ID USB HCD Connect callback ID
  *          @arg @ref HAL_HCD_DISCONNECT_CB_ID OTG HCD Disconnect callback ID
  *          @arg @ref HAL_HCD_PORT_ENABLED_CB_ID USB HCD Port Enabled callback ID
  *          @arg @ref HAL_HCD_PORT_DISABLED_CB_ID USB HCD Port Disabled callback ID
  *          @arg @ref HAL_HCD_MSPINIT_CB_ID MspDeInit callback ID
  *          @arg @ref HAL_HCD_MSPDEINIT_CB_ID MspDeInit callback ID
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_HCD_UnRegisterCallback(HCD_HandleTypeDef *hhcd, HAL_HCD_CallbackIDTypeDef CallbackID)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Process locked */
  __HAL_LOCK(hhcd);                                     /* HAL辅助宏/函数 */

  /* Setup Legacy weak Callbacks  */
  if (hhcd->State == HAL_HCD_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_HCD_SOF_CB_ID :                          /* case标签 */
        hhcd->SOFCallback = HAL_HCD_SOF_Callback;       /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_HCD_CONNECT_CB_ID :                      /* case标签 */
        hhcd->ConnectCallback = HAL_HCD_Connect_Callback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_HCD_DISCONNECT_CB_ID :                   /* case标签 */
        hhcd->DisconnectCallback = HAL_HCD_Disconnect_Callback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_HCD_PORT_ENABLED_CB_ID :                 /* case标签 */
        hhcd->PortEnabledCallback = HAL_HCD_PortEnabled_Callback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_HCD_PORT_DISABLED_CB_ID :                /* case标签 */
        hhcd->PortDisabledCallback = HAL_HCD_PortDisabled_Callback;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_HCD_MSPINIT_CB_ID :                      /* case标签 */
        hhcd->MspInitCallback = HAL_HCD_MspInit;        /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_HCD_MSPDEINIT_CB_ID :                    /* case标签 */
        hhcd->MspDeInitCallback = HAL_HCD_MspDeInit;    /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hhcd->ErrorCode |= HAL_HCD_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (hhcd->State == HAL_HCD_STATE_RESET)          /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_HCD_MSPINIT_CB_ID :                      /* case标签 */
        hhcd->MspInitCallback = HAL_HCD_MspInit;        /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_HCD_MSPDEINIT_CB_ID :                    /* case标签 */
        hhcd->MspDeInitCallback = HAL_HCD_MspDeInit;    /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hhcd->ErrorCode |= HAL_HCD_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hhcd->ErrorCode |= HAL_HCD_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hhcd);                                   /* HAL辅助宏/函数 */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Register USB HCD Host Channel Notify URB Change Callback
  *         To be used instead of the weak HAL_HCD_HC_NotifyURBChange_Callback() predefined callback
  * @param  hhcd HCD handle
  * @param  pCallback pointer to the USB HCD Host Channel Notify URB Change Callback function
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_HCD_RegisterHC_NotifyURBChangeCallback(HCD_HandleTypeDef *hhcd,  /* HAL状态类型函数 */
                                                             pHCD_HC_NotifyURBChangeCallbackTypeDef pCallback)  /* 源代码 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  if (pCallback == NULL)                                /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hhcd->ErrorCode |= HAL_HCD_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Process locked */
  __HAL_LOCK(hhcd);                                     /* HAL辅助宏/函数 */

  if (hhcd->State == HAL_HCD_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    hhcd->HC_NotifyURBChangeCallback = pCallback;       /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hhcd->ErrorCode |= HAL_HCD_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hhcd);                                   /* HAL辅助宏/函数 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Unregister the USB HCD Host Channel Notify URB Change Callback
  *         USB HCD Host Channel Notify URB Change Callback is redirected to the weak HAL_HCD_HC_NotifyURBChange_Callback() predefined callback
  * @param  hhcd HCD handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_HCD_UnRegisterHC_NotifyURBChangeCallback(HCD_HandleTypeDef *hhcd)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Process locked */
  __HAL_LOCK(hhcd);                                     /* HAL辅助宏/函数 */

  if (hhcd->State == HAL_HCD_STATE_READY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    hhcd->HC_NotifyURBChangeCallback = HAL_HCD_HC_NotifyURBChange_Callback; /* Legacy weak DataOutStageCallback  */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hhcd->ErrorCode |= HAL_HCD_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hhcd);                                   /* HAL辅助宏/函数 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* USE_HAL_HCD_REGISTER_CALLBACKS */

/**
  * @}
  */

/** @defgroup HCD_Exported_Functions_Group3 Peripheral Control functions
  *  @brief   Management functions
  *
@verbatim
 ===============================================================================
                      ##### Peripheral Control functions #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to control the HCD data
    transfers.

@endverbatim
  * @{
  */

/**
  * @brief  Start the host driver.
  * @param  hhcd HCD handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_HCD_Start(HCD_HandleTypeDef *hhcd)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  __HAL_LOCK(hhcd);                                     /* HAL辅助宏/函数 */
  __HAL_HCD_ENABLE(hhcd);                               /* HAL辅助宏/函数 */
  (void)USB_DriveVbus(hhcd->Instance, 1U);              /* 执行语句 */
  __HAL_UNLOCK(hhcd);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Stop the host driver.
  * @param  hhcd HCD handle
  * @retval HAL status
  */

HAL_StatusTypeDef HAL_HCD_Stop(HCD_HandleTypeDef *hhcd)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  __HAL_LOCK(hhcd);                                     /* HAL辅助宏/函数 */
  (void)USB_StopHost(hhcd->Instance);                   /* 执行语句 */
  __HAL_UNLOCK(hhcd);                                   /* HAL辅助宏/函数 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Reset the host port.
  * @param  hhcd HCD handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_HCD_ResetPort(HCD_HandleTypeDef *hhcd)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  return (USB_ResetPort(hhcd->Instance));               /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup HCD_Exported_Functions_Group4 Peripheral State functions
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
  * @brief  Return the HCD handle state.
  * @param  hhcd HCD handle
  * @retval HAL state
  */
HCD_StateTypeDef HAL_HCD_GetState(HCD_HandleTypeDef *hhcd)  /* 源代码 */
{                                                       /* 代码块开始 */
  return hhcd->State;                                   /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return  URB state for a channel.
  * @param  hhcd HCD handle
  * @param  chnum Channel number.
  *         This parameter can be a value from 1 to 15
  * @retval URB state.
  *          This parameter can be one of these values:
  *            URB_IDLE/
  *            URB_DONE/
  *            URB_NOTREADY/
  *            URB_NYET/
  *            URB_ERROR/
  *            URB_STALL
  */
HCD_URBStateTypeDef HAL_HCD_HC_GetURBState(HCD_HandleTypeDef *hhcd, uint8_t chnum)  /* 源代码 */
{                                                       /* 代码块开始 */
  return hhcd->hc[chnum].urb_state;                     /* 返回值 */
}                                                       /* 代码块结束 */


/**
  * @brief  Return the last host transfer size.
  * @param  hhcd HCD handle
  * @param  chnum Channel number.
  *         This parameter can be a value from 1 to 15
  * @retval last transfer size in byte
  */
uint32_t HAL_HCD_HC_GetXferCount(HCD_HandleTypeDef *hhcd, uint8_t chnum)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  return hhcd->hc[chnum].xfer_count;                    /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return the Host Channel state.
  * @param  hhcd HCD handle
  * @param  chnum Channel number.
  *         This parameter can be a value from 1 to 15
  * @retval Host channel state
  *          This parameter can be one of these values:
  *            HC_IDLE/
  *            HC_XFRC/
  *            HC_HALTED/
  *            HC_NYET/
  *            HC_NAK/
  *            HC_STALL/
  *            HC_XACTERR/
  *            HC_BBLERR/
  *            HC_DATATGLERR
  */
HCD_HCStateTypeDef  HAL_HCD_HC_GetState(HCD_HandleTypeDef *hhcd, uint8_t chnum)  /* 源代码 */
{                                                       /* 代码块开始 */
  return hhcd->hc[chnum].state;                         /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return the current Host frame number.
  * @param  hhcd HCD handle
  * @retval Current Host frame number
  */
uint32_t HAL_HCD_GetCurrentFrame(HCD_HandleTypeDef *hhcd)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  return (USB_GetCurrentFrame(hhcd->Instance));         /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return the Host enumeration speed.
  * @param  hhcd HCD handle
  * @retval Enumeration speed
  */
uint32_t HAL_HCD_GetCurrentSpeed(HCD_HandleTypeDef *hhcd)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  return (USB_GetHostSpeed(hhcd->Instance));            /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup HCD_Private_Functions
  * @{
  */
/**
  * @brief  Handle Host Channel IN interrupt requests.
  * @param  hhcd HCD handle
  * @param  chnum Channel number.
  *         This parameter can be a value from 1 to 15
  * @retval none
  */
static void HCD_HC_IN_IRQHandler(HCD_HandleTypeDef *hhcd, uint8_t chnum)  /* 静态函数 */
{                                                       /* 代码块开始 */
  USB_OTG_GlobalTypeDef *USBx = hhcd->Instance;         /* 赋值或初始化 */
  uint32_t USBx_BASE = (uint32_t)USBx;                  /* 32位无符号变量/函数 */
  uint32_t ch_num = (uint32_t)chnum;                    /* 32位无符号变量/函数 */

  uint32_t tmpreg;                                      /* 32位无符号变量/函数 */

  if ((USBx_HC(ch_num)->HCINT & USB_OTG_HCINT_AHBERR) == USB_OTG_HCINT_AHBERR)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    __HAL_HCD_CLEAR_HC_INT(ch_num, USB_OTG_HCINT_AHBERR);  /* HAL辅助宏/函数 */
    __HAL_HCD_UNMASK_HALT_HC_INT(ch_num);               /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  else if ((USBx_HC(ch_num)->HCINT & USB_OTG_HCINT_BBERR) == USB_OTG_HCINT_BBERR)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    __HAL_HCD_CLEAR_HC_INT(ch_num, USB_OTG_HCINT_BBERR);  /* HAL辅助宏/函数 */
    hhcd->hc[ch_num].state = HC_BBLERR;                 /* 赋值或初始化 */
    __HAL_HCD_UNMASK_HALT_HC_INT(ch_num);               /* HAL辅助宏/函数 */
    (void)USB_HC_Halt(hhcd->Instance, (uint8_t)ch_num);  /* 执行语句 */
  }                                                     /* 代码块结束 */
  else if ((USBx_HC(ch_num)->HCINT & USB_OTG_HCINT_ACK) == USB_OTG_HCINT_ACK)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    __HAL_HCD_CLEAR_HC_INT(ch_num, USB_OTG_HCINT_ACK);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  else if ((USBx_HC(ch_num)->HCINT & USB_OTG_HCINT_STALL) == USB_OTG_HCINT_STALL)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    __HAL_HCD_UNMASK_HALT_HC_INT(ch_num);               /* HAL辅助宏/函数 */
    hhcd->hc[ch_num].state = HC_STALL;                  /* 赋值或初始化 */
    __HAL_HCD_CLEAR_HC_INT(ch_num, USB_OTG_HCINT_NAK);  /* HAL辅助宏/函数 */
    __HAL_HCD_CLEAR_HC_INT(ch_num, USB_OTG_HCINT_STALL);  /* HAL辅助宏/函数 */
    (void)USB_HC_Halt(hhcd->Instance, (uint8_t)ch_num);  /* 执行语句 */
  }                                                     /* 代码块结束 */
  else if ((USBx_HC(ch_num)->HCINT & USB_OTG_HCINT_DTERR) == USB_OTG_HCINT_DTERR)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    __HAL_HCD_UNMASK_HALT_HC_INT(ch_num);               /* HAL辅助宏/函数 */
    hhcd->hc[ch_num].state = HC_DATATGLERR;             /* 赋值或初始化 */
    __HAL_HCD_CLEAR_HC_INT(ch_num, USB_OTG_HCINT_NAK);  /* HAL辅助宏/函数 */
    __HAL_HCD_CLEAR_HC_INT(ch_num, USB_OTG_HCINT_DTERR);  /* HAL辅助宏/函数 */
    (void)USB_HC_Halt(hhcd->Instance, (uint8_t)ch_num);  /* 执行语句 */
  }                                                     /* 代码块结束 */
  else if ((USBx_HC(ch_num)->HCINT & USB_OTG_HCINT_TXERR) == USB_OTG_HCINT_TXERR)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    __HAL_HCD_UNMASK_HALT_HC_INT(ch_num);               /* HAL辅助宏/函数 */
    hhcd->hc[ch_num].state = HC_XACTERR;                /* 赋值或初始化 */
    (void)USB_HC_Halt(hhcd->Instance, (uint8_t)ch_num);  /* 执行语句 */
    __HAL_HCD_CLEAR_HC_INT(ch_num, USB_OTG_HCINT_TXERR);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* ... */
  }                                                     /* 代码块结束 */

  if ((USBx_HC(ch_num)->HCINT & USB_OTG_HCINT_FRMOR) == USB_OTG_HCINT_FRMOR)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    __HAL_HCD_UNMASK_HALT_HC_INT(ch_num);               /* HAL辅助宏/函数 */
    (void)USB_HC_Halt(hhcd->Instance, (uint8_t)ch_num);  /* 执行语句 */
    __HAL_HCD_CLEAR_HC_INT(ch_num, USB_OTG_HCINT_FRMOR);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  else if ((USBx_HC(ch_num)->HCINT & USB_OTG_HCINT_XFRC) == USB_OTG_HCINT_XFRC)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    if (hhcd->Init.dma_enable != 0U)                    /* 条件判断 */
    {                                                   /* 代码块开始 */
      hhcd->hc[ch_num].xfer_count = hhcd->hc[ch_num].XferSize - \
                                    (USBx_HC(ch_num)->HCTSIZ & USB_OTG_HCTSIZ_XFRSIZ);  /* 执行语句 */
    }                                                   /* 代码块结束 */

    hhcd->hc[ch_num].state = HC_XFRC;                   /* 赋值或初始化 */
    hhcd->hc[ch_num].ErrCnt = 0U;                       /* 赋值或初始化 */
    __HAL_HCD_CLEAR_HC_INT(ch_num, USB_OTG_HCINT_XFRC);  /* HAL辅助宏/函数 */

    if ((hhcd->hc[ch_num].ep_type == EP_TYPE_CTRL) ||   /* 条件判断 */
        (hhcd->hc[ch_num].ep_type == EP_TYPE_BULK))     /* 源代码 */
    {                                                   /* 代码块开始 */
      __HAL_HCD_UNMASK_HALT_HC_INT(ch_num);             /* HAL辅助宏/函数 */
      (void)USB_HC_Halt(hhcd->Instance, (uint8_t)ch_num);  /* 执行语句 */
      __HAL_HCD_CLEAR_HC_INT(ch_num, USB_OTG_HCINT_NAK);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
    else if (hhcd->hc[ch_num].ep_type == EP_TYPE_INTR)  /* 否则如果 */
    {                                                   /* 代码块开始 */
      USBx_HC(ch_num)->HCCHAR |= USB_OTG_HCCHAR_ODDFRM;  /* 赋值或初始化 */
      hhcd->hc[ch_num].urb_state = URB_DONE;            /* 赋值或初始化 */

#if (USE_HAL_HCD_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
      hhcd->HC_NotifyURBChangeCallback(hhcd, (uint8_t)ch_num, hhcd->hc[ch_num].urb_state);  /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_HCD_HC_NotifyURBChange_Callback(hhcd, (uint8_t)ch_num, hhcd->hc[ch_num].urb_state);  /* HAL库函数调用 */
#endif /* USE_HAL_HCD_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
    else if (hhcd->hc[ch_num].ep_type == EP_TYPE_ISOC)  /* 否则如果 */
    {                                                   /* 代码块开始 */
      hhcd->hc[ch_num].urb_state = URB_DONE;            /* 赋值或初始化 */
      hhcd->hc[ch_num].toggle_in ^= 1U;                 /* 赋值或初始化 */

#if (USE_HAL_HCD_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
      hhcd->HC_NotifyURBChangeCallback(hhcd, (uint8_t)ch_num, hhcd->hc[ch_num].urb_state);  /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_HCD_HC_NotifyURBChange_Callback(hhcd, (uint8_t)ch_num, hhcd->hc[ch_num].urb_state);  /* HAL库函数调用 */
#endif /* USE_HAL_HCD_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* ... */
    }                                                   /* 代码块结束 */

    if (hhcd->Init.dma_enable == 1U)                    /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (((hhcd->hc[ch_num].XferSize / hhcd->hc[ch_num].max_packet) & 1U) != 0U)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        hhcd->hc[ch_num].toggle_in ^= 1U;               /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hhcd->hc[ch_num].toggle_in ^= 1U;                 /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if ((USBx_HC(ch_num)->HCINT & USB_OTG_HCINT_CHH) == USB_OTG_HCINT_CHH)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    __HAL_HCD_MASK_HALT_HC_INT(ch_num);                 /* HAL辅助宏/函数 */

    if (hhcd->hc[ch_num].state == HC_XFRC)              /* 条件判断 */
    {                                                   /* 代码块开始 */
      hhcd->hc[ch_num].urb_state = URB_DONE;            /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if (hhcd->hc[ch_num].state == HC_STALL)        /* 否则如果 */
    {                                                   /* 代码块开始 */
      hhcd->hc[ch_num].urb_state = URB_STALL;           /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if ((hhcd->hc[ch_num].state == HC_XACTERR) ||  /* 否则如果 */
             (hhcd->hc[ch_num].state == HC_DATATGLERR))  /* 源代码 */
    {                                                   /* 代码块开始 */
      hhcd->hc[ch_num].ErrCnt++;                        /* 执行语句 */
      if (hhcd->hc[ch_num].ErrCnt > 2U)                 /* 条件判断 */
      {                                                 /* 代码块开始 */
        hhcd->hc[ch_num].ErrCnt = 0U;                   /* 赋值或初始化 */
        hhcd->hc[ch_num].urb_state = URB_ERROR;         /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        hhcd->hc[ch_num].urb_state = URB_NOTREADY;      /* 赋值或初始化 */

        /* re-activate the channel */
        tmpreg = USBx_HC(ch_num)->HCCHAR;               /* 赋值或初始化 */
        tmpreg &= ~USB_OTG_HCCHAR_CHDIS;                /* 赋值或初始化 */
        tmpreg |= USB_OTG_HCCHAR_CHENA;                 /* 赋值或初始化 */
        USBx_HC(ch_num)->HCCHAR = tmpreg;               /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else if (hhcd->hc[ch_num].state == HC_NAK)          /* 否则如果 */
    {                                                   /* 代码块开始 */
      hhcd->hc[ch_num].urb_state  = URB_NOTREADY;       /* 赋值或初始化 */

      /* re-activate the channel */
      tmpreg = USBx_HC(ch_num)->HCCHAR;                 /* 赋值或初始化 */
      tmpreg &= ~USB_OTG_HCCHAR_CHDIS;                  /* 赋值或初始化 */
      tmpreg |= USB_OTG_HCCHAR_CHENA;                   /* 赋值或初始化 */
      USBx_HC(ch_num)->HCCHAR = tmpreg;                 /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if (hhcd->hc[ch_num].state == HC_BBLERR)       /* 否则如果 */
    {                                                   /* 代码块开始 */
      hhcd->hc[ch_num].ErrCnt++;                        /* 执行语句 */
      hhcd->hc[ch_num].urb_state = URB_ERROR;           /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* ... */
    }                                                   /* 代码块结束 */
    __HAL_HCD_CLEAR_HC_INT(ch_num, USB_OTG_HCINT_CHH);  /* HAL辅助宏/函数 */
    HAL_HCD_HC_NotifyURBChange_Callback(hhcd, (uint8_t)ch_num, hhcd->hc[ch_num].urb_state);  /* HAL库函数调用 */
  }                                                     /* 代码块结束 */
  else if ((USBx_HC(ch_num)->HCINT & USB_OTG_HCINT_NAK) == USB_OTG_HCINT_NAK)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    if (hhcd->hc[ch_num].ep_type == EP_TYPE_INTR)       /* 条件判断 */
    {                                                   /* 代码块开始 */
      hhcd->hc[ch_num].ErrCnt = 0U;                     /* 赋值或初始化 */
      __HAL_HCD_UNMASK_HALT_HC_INT(ch_num);             /* HAL辅助宏/函数 */
      (void)USB_HC_Halt(hhcd->Instance, (uint8_t)ch_num);  /* 执行语句 */
    }                                                   /* 代码块结束 */
    else if ((hhcd->hc[ch_num].ep_type == EP_TYPE_CTRL) ||  /* 否则如果 */
             (hhcd->hc[ch_num].ep_type == EP_TYPE_BULK))  /* 源代码 */
    {                                                   /* 代码块开始 */
      hhcd->hc[ch_num].ErrCnt = 0U;                     /* 赋值或初始化 */

      if (hhcd->Init.dma_enable == 0U)                  /* 条件判断 */
      {                                                 /* 代码块开始 */
        hhcd->hc[ch_num].state = HC_NAK;                /* 赋值或初始化 */
        __HAL_HCD_UNMASK_HALT_HC_INT(ch_num);           /* HAL辅助宏/函数 */
        (void)USB_HC_Halt(hhcd->Instance, (uint8_t)ch_num);  /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* ... */
    }                                                   /* 代码块结束 */
    __HAL_HCD_CLEAR_HC_INT(ch_num, USB_OTG_HCINT_NAK);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* ... */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Handle Host Channel OUT interrupt requests.
  * @param  hhcd HCD handle
  * @param  chnum Channel number.
  *         This parameter can be a value from 1 to 15
  * @retval none
  */
static void HCD_HC_OUT_IRQHandler(HCD_HandleTypeDef *hhcd, uint8_t chnum)  /* 静态函数 */
{                                                       /* 代码块开始 */
  USB_OTG_GlobalTypeDef *USBx = hhcd->Instance;         /* 赋值或初始化 */
  uint32_t USBx_BASE = (uint32_t)USBx;                  /* 32位无符号变量/函数 */
  uint32_t ch_num = (uint32_t)chnum;                    /* 32位无符号变量/函数 */
  uint32_t tmpreg;                                      /* 32位无符号变量/函数 */
  uint32_t num_packets;                                 /* 32位无符号变量/函数 */

  if ((USBx_HC(ch_num)->HCINT & USB_OTG_HCINT_AHBERR) == USB_OTG_HCINT_AHBERR)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    __HAL_HCD_CLEAR_HC_INT(ch_num, USB_OTG_HCINT_AHBERR);  /* HAL辅助宏/函数 */
    __HAL_HCD_UNMASK_HALT_HC_INT(ch_num);               /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  else if ((USBx_HC(ch_num)->HCINT & USB_OTG_HCINT_ACK) == USB_OTG_HCINT_ACK)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    __HAL_HCD_CLEAR_HC_INT(ch_num, USB_OTG_HCINT_ACK);  /* HAL辅助宏/函数 */

    if (hhcd->hc[ch_num].do_ping == 1U)                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      hhcd->hc[ch_num].do_ping = 0U;                    /* 赋值或初始化 */
      hhcd->hc[ch_num].urb_state  = URB_NOTREADY;       /* 赋值或初始化 */
      __HAL_HCD_UNMASK_HALT_HC_INT(ch_num);             /* HAL辅助宏/函数 */
      (void)USB_HC_Halt(hhcd->Instance, (uint8_t)ch_num);  /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if ((USBx_HC(ch_num)->HCINT & USB_OTG_HCINT_FRMOR) == USB_OTG_HCINT_FRMOR)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    __HAL_HCD_UNMASK_HALT_HC_INT(ch_num);               /* HAL辅助宏/函数 */
    (void)USB_HC_Halt(hhcd->Instance, (uint8_t)ch_num);  /* 执行语句 */
    __HAL_HCD_CLEAR_HC_INT(ch_num, USB_OTG_HCINT_FRMOR);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  else if ((USBx_HC(ch_num)->HCINT & USB_OTG_HCINT_XFRC) == USB_OTG_HCINT_XFRC)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    hhcd->hc[ch_num].ErrCnt = 0U;                       /* 赋值或初始化 */

    /* transaction completed with NYET state, update do ping state */
    if ((USBx_HC(ch_num)->HCINT & USB_OTG_HCINT_NYET) == USB_OTG_HCINT_NYET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hhcd->hc[ch_num].do_ping = 1U;                    /* 赋值或初始化 */
      __HAL_HCD_CLEAR_HC_INT(ch_num, USB_OTG_HCINT_NYET);  /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
    __HAL_HCD_UNMASK_HALT_HC_INT(ch_num);               /* HAL辅助宏/函数 */
    (void)USB_HC_Halt(hhcd->Instance, (uint8_t)ch_num);  /* 执行语句 */
    __HAL_HCD_CLEAR_HC_INT(ch_num, USB_OTG_HCINT_XFRC);  /* HAL辅助宏/函数 */
    hhcd->hc[ch_num].state = HC_XFRC;                   /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if ((USBx_HC(ch_num)->HCINT & USB_OTG_HCINT_NYET) == USB_OTG_HCINT_NYET)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    hhcd->hc[ch_num].state = HC_NYET;                   /* 赋值或初始化 */
    hhcd->hc[ch_num].do_ping = 1U;                      /* 赋值或初始化 */
    hhcd->hc[ch_num].ErrCnt = 0U;                       /* 赋值或初始化 */
    __HAL_HCD_UNMASK_HALT_HC_INT(ch_num);               /* HAL辅助宏/函数 */
    (void)USB_HC_Halt(hhcd->Instance, (uint8_t)ch_num);  /* 执行语句 */
    __HAL_HCD_CLEAR_HC_INT(ch_num, USB_OTG_HCINT_NYET);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  else if ((USBx_HC(ch_num)->HCINT & USB_OTG_HCINT_STALL) == USB_OTG_HCINT_STALL)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    __HAL_HCD_CLEAR_HC_INT(ch_num, USB_OTG_HCINT_STALL);  /* HAL辅助宏/函数 */
    __HAL_HCD_UNMASK_HALT_HC_INT(ch_num);               /* HAL辅助宏/函数 */
    (void)USB_HC_Halt(hhcd->Instance, (uint8_t)ch_num);  /* 执行语句 */
    hhcd->hc[ch_num].state = HC_STALL;                  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if ((USBx_HC(ch_num)->HCINT & USB_OTG_HCINT_NAK) == USB_OTG_HCINT_NAK)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    hhcd->hc[ch_num].ErrCnt = 0U;                       /* 赋值或初始化 */
    hhcd->hc[ch_num].state = HC_NAK;                    /* 赋值或初始化 */

    if (hhcd->hc[ch_num].do_ping == 0U)                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (hhcd->hc[ch_num].speed == HCD_DEVICE_SPEED_HIGH)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        hhcd->hc[ch_num].do_ping = 1U;                  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    __HAL_HCD_UNMASK_HALT_HC_INT(ch_num);               /* HAL辅助宏/函数 */
    (void)USB_HC_Halt(hhcd->Instance, (uint8_t)ch_num);  /* 执行语句 */
    __HAL_HCD_CLEAR_HC_INT(ch_num, USB_OTG_HCINT_NAK);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  else if ((USBx_HC(ch_num)->HCINT & USB_OTG_HCINT_TXERR) == USB_OTG_HCINT_TXERR)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    if (hhcd->Init.dma_enable == 0U)                    /* 条件判断 */
    {                                                   /* 代码块开始 */
      hhcd->hc[ch_num].state = HC_XACTERR;              /* 赋值或初始化 */
      __HAL_HCD_UNMASK_HALT_HC_INT(ch_num);             /* HAL辅助宏/函数 */
      (void)USB_HC_Halt(hhcd->Instance, (uint8_t)ch_num);  /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hhcd->hc[ch_num].ErrCnt++;                        /* 执行语句 */
      if (hhcd->hc[ch_num].ErrCnt > 2U)                 /* 条件判断 */
      {                                                 /* 代码块开始 */
        hhcd->hc[ch_num].ErrCnt = 0U;                   /* 赋值或初始化 */
        hhcd->hc[ch_num].urb_state = URB_ERROR;         /* 赋值或初始化 */
        HAL_HCD_HC_NotifyURBChange_Callback(hhcd, (uint8_t)ch_num, hhcd->hc[ch_num].urb_state);  /* HAL库函数调用 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        hhcd->hc[ch_num].urb_state = URB_NOTREADY;      /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    __HAL_HCD_CLEAR_HC_INT(ch_num, USB_OTG_HCINT_TXERR);  /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  else if ((USBx_HC(ch_num)->HCINT & USB_OTG_HCINT_DTERR) == USB_OTG_HCINT_DTERR)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    __HAL_HCD_UNMASK_HALT_HC_INT(ch_num);               /* HAL辅助宏/函数 */
    (void)USB_HC_Halt(hhcd->Instance, (uint8_t)ch_num);  /* 执行语句 */
    __HAL_HCD_CLEAR_HC_INT(ch_num, USB_OTG_HCINT_NAK);  /* HAL辅助宏/函数 */
    __HAL_HCD_CLEAR_HC_INT(ch_num, USB_OTG_HCINT_DTERR);  /* HAL辅助宏/函数 */
    hhcd->hc[ch_num].state = HC_DATATGLERR;             /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if ((USBx_HC(ch_num)->HCINT & USB_OTG_HCINT_CHH) == USB_OTG_HCINT_CHH)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    __HAL_HCD_MASK_HALT_HC_INT(ch_num);                 /* HAL辅助宏/函数 */

    if (hhcd->hc[ch_num].state == HC_XFRC)              /* 条件判断 */
    {                                                   /* 代码块开始 */
      hhcd->hc[ch_num].urb_state  = URB_DONE;           /* 赋值或初始化 */
      if ((hhcd->hc[ch_num].ep_type == EP_TYPE_BULK) ||  /* 条件判断 */
          (hhcd->hc[ch_num].ep_type == EP_TYPE_INTR))   /* 源代码 */
      {                                                 /* 代码块开始 */
        if (hhcd->Init.dma_enable == 1U)                /* 条件判断 */
        {                                               /* 代码块开始 */
          if (hhcd->hc[ch_num].xfer_len > 0U)           /* 条件判断 */
          {                                             /* 代码块开始 */
            num_packets = (hhcd->hc[ch_num].xfer_len + hhcd->hc[ch_num].max_packet - 1U) / hhcd->hc[ch_num].max_packet;  /* 赋值或初始化 */

            if ((num_packets & 1U) != 0U)               /* 条件判断 */
            {                                           /* 代码块开始 */
              hhcd->hc[ch_num].toggle_out ^= 1U;        /* 赋值或初始化 */
            }                                           /* 代码块结束 */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          hhcd->hc[ch_num].toggle_out ^= 1U;            /* 赋值或初始化 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else if (hhcd->hc[ch_num].state == HC_NAK)          /* 否则如果 */
    {                                                   /* 代码块开始 */
      hhcd->hc[ch_num].urb_state = URB_NOTREADY;        /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if (hhcd->hc[ch_num].state == HC_NYET)         /* 否则如果 */
    {                                                   /* 代码块开始 */
      hhcd->hc[ch_num].urb_state  = URB_NOTREADY;       /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if (hhcd->hc[ch_num].state == HC_STALL)        /* 否则如果 */
    {                                                   /* 代码块开始 */
      hhcd->hc[ch_num].urb_state  = URB_STALL;          /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if ((hhcd->hc[ch_num].state == HC_XACTERR) ||  /* 否则如果 */
             (hhcd->hc[ch_num].state == HC_DATATGLERR))  /* 源代码 */
    {                                                   /* 代码块开始 */
      hhcd->hc[ch_num].ErrCnt++;                        /* 执行语句 */
      if (hhcd->hc[ch_num].ErrCnt > 2U)                 /* 条件判断 */
      {                                                 /* 代码块开始 */
        hhcd->hc[ch_num].ErrCnt = 0U;                   /* 赋值或初始化 */
        hhcd->hc[ch_num].urb_state = URB_ERROR;         /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        hhcd->hc[ch_num].urb_state = URB_NOTREADY;      /* 赋值或初始化 */

        /* re-activate the channel  */
        tmpreg = USBx_HC(ch_num)->HCCHAR;               /* 赋值或初始化 */
        tmpreg &= ~USB_OTG_HCCHAR_CHDIS;                /* 赋值或初始化 */
        tmpreg |= USB_OTG_HCCHAR_CHENA;                 /* 赋值或初始化 */
        USBx_HC(ch_num)->HCCHAR = tmpreg;               /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* ... */
    }                                                   /* 代码块结束 */

    __HAL_HCD_CLEAR_HC_INT(ch_num, USB_OTG_HCINT_CHH);  /* HAL辅助宏/函数 */
    HAL_HCD_HC_NotifyURBChange_Callback(hhcd, (uint8_t)ch_num, hhcd->hc[ch_num].urb_state);  /* HAL库函数调用 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* ... */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Handle Rx Queue Level interrupt requests.
  * @param  hhcd HCD handle
  * @retval none
  */
static void HCD_RXQLVL_IRQHandler(HCD_HandleTypeDef *hhcd)  /* 静态函数 */
{                                                       /* 代码块开始 */
  USB_OTG_GlobalTypeDef *USBx = hhcd->Instance;         /* 赋值或初始化 */
  uint32_t USBx_BASE = (uint32_t)USBx;                  /* 32位无符号变量/函数 */
  uint32_t pktsts;                                      /* 32位无符号变量/函数 */
  uint32_t pktcnt;                                      /* 32位无符号变量/函数 */
  uint32_t GrxstspReg;                                  /* 32位无符号变量/函数 */
  uint32_t xferSizePktCnt;                              /* 32位无符号变量/函数 */
  uint32_t tmpreg;                                      /* 32位无符号变量/函数 */
  uint32_t ch_num;                                      /* 32位无符号变量/函数 */

  GrxstspReg = hhcd->Instance->GRXSTSP;                 /* 赋值或初始化 */
  ch_num = GrxstspReg & USB_OTG_GRXSTSP_EPNUM;          /* 赋值或初始化 */
  pktsts = (GrxstspReg & USB_OTG_GRXSTSP_PKTSTS) >> 17;  /* 赋值或初始化 */
  pktcnt = (GrxstspReg & USB_OTG_GRXSTSP_BCNT) >> 4;    /* 赋值或初始化 */

  switch (pktsts)                                       /* switch分支 */
  {                                                     /* 代码块开始 */
    case GRXSTS_PKTSTS_IN:                              /* case标签 */
      /* Read the data into the host buffer. */
      if ((pktcnt > 0U) && (hhcd->hc[ch_num].xfer_buff != (void *)0))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        if ((hhcd->hc[ch_num].xfer_count + pktcnt) <= hhcd->hc[ch_num].xfer_len)  /* 条件判断 */
        {                                               /* 代码块开始 */
          (void)USB_ReadPacket(hhcd->Instance,          /* 列表续行 */
                               hhcd->hc[ch_num].xfer_buff, (uint16_t)pktcnt);  /* 执行语句 */

          /* manage multiple Xfer */
          hhcd->hc[ch_num].xfer_buff += pktcnt;         /* 赋值或初始化 */
          hhcd->hc[ch_num].xfer_count += pktcnt;        /* 赋值或初始化 */

          /* get transfer size packet count */
          xferSizePktCnt = (USBx_HC(ch_num)->HCTSIZ & USB_OTG_HCTSIZ_PKTCNT) >> 19;  /* 赋值或初始化 */

          if ((hhcd->hc[ch_num].max_packet == pktcnt) && (xferSizePktCnt > 0U))  /* 条件判断 */
          {                                             /* 代码块开始 */
            /* re-activate the channel when more packets are expected */
            tmpreg = USBx_HC(ch_num)->HCCHAR;           /* 赋值或初始化 */
            tmpreg &= ~USB_OTG_HCCHAR_CHDIS;            /* 赋值或初始化 */
            tmpreg |= USB_OTG_HCCHAR_CHENA;             /* 赋值或初始化 */
            USBx_HC(ch_num)->HCCHAR = tmpreg;           /* 赋值或初始化 */
            hhcd->hc[ch_num].toggle_in ^= 1U;           /* 赋值或初始化 */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          hhcd->hc[ch_num].urb_state = URB_ERROR;       /* 赋值或初始化 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */

    case GRXSTS_PKTSTS_DATA_TOGGLE_ERR:                 /* case标签 */
      break;                                            /* 跳出 */

    case GRXSTS_PKTSTS_IN_XFER_COMP:                    /* case标签 */
    case GRXSTS_PKTSTS_CH_HALTED:                       /* case标签 */
    default:                                            /* default分支 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Handle Host Port interrupt requests.
  * @param  hhcd HCD handle
  * @retval None
  */
static void HCD_Port_IRQHandler(HCD_HandleTypeDef *hhcd)  /* 静态函数 */
{                                                       /* 代码块开始 */
  USB_OTG_GlobalTypeDef *USBx = hhcd->Instance;         /* 赋值或初始化 */
  uint32_t USBx_BASE = (uint32_t)USBx;                  /* 32位无符号变量/函数 */
  __IO uint32_t hprt0, hprt0_dup;                       /* 执行语句 */

  /* Handle Host Port Interrupts */
  hprt0 = USBx_HPRT0;                                   /* 赋值或初始化 */
  hprt0_dup = USBx_HPRT0;                               /* 赋值或初始化 */

  hprt0_dup &= ~(USB_OTG_HPRT_PENA | USB_OTG_HPRT_PCDET | \
                 USB_OTG_HPRT_PENCHNG | USB_OTG_HPRT_POCCHNG);  /* 执行语句 */

  /* Check whether Port Connect detected */
  if ((hprt0 & USB_OTG_HPRT_PCDET) == USB_OTG_HPRT_PCDET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((hprt0 & USB_OTG_HPRT_PCSTS) == USB_OTG_HPRT_PCSTS)  /* 条件判断 */
    {                                                   /* 代码块开始 */
#if (USE_HAL_HCD_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
      hhcd->ConnectCallback(hhcd);                      /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_HCD_Connect_Callback(hhcd);                   /* HAL库函数调用 */
#endif /* USE_HAL_HCD_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
    hprt0_dup  |= USB_OTG_HPRT_PCDET;                   /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Check whether Port Enable Changed */
  if ((hprt0 & USB_OTG_HPRT_PENCHNG) == USB_OTG_HPRT_PENCHNG)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hprt0_dup |= USB_OTG_HPRT_PENCHNG;                  /* 赋值或初始化 */

    if ((hprt0 & USB_OTG_HPRT_PENA) == USB_OTG_HPRT_PENA)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (hhcd->Init.phy_itface  == USB_OTG_EMBEDDED_PHY)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        if ((hprt0 & USB_OTG_HPRT_PSPD) == (HPRT0_PRTSPD_LOW_SPEED << 17))  /* 条件判断 */
        {                                               /* 代码块开始 */
          (void)USB_InitFSLSPClkSel(hhcd->Instance, HCFG_6_MHZ);  /* 执行语句 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          (void)USB_InitFSLSPClkSel(hhcd->Instance, HCFG_48_MHZ);  /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        if (hhcd->Init.speed == HCD_SPEED_FULL)         /* 条件判断 */
        {                                               /* 代码块开始 */
          USBx_HOST->HFIR = 60000U;                     /* 赋值或初始化 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
#if (USE_HAL_HCD_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
      hhcd->PortEnabledCallback(hhcd);                  /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_HCD_PortEnabled_Callback(hhcd);               /* HAL库函数调用 */
#endif /* USE_HAL_HCD_REGISTER_CALLBACKS */

    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
#if (USE_HAL_HCD_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
      hhcd->PortDisabledCallback(hhcd);                 /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_HCD_PortDisabled_Callback(hhcd);              /* HAL库函数调用 */
#endif /* USE_HAL_HCD_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Check for an overcurrent */
  if ((hprt0 & USB_OTG_HPRT_POCCHNG) == USB_OTG_HPRT_POCCHNG)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    hprt0_dup |= USB_OTG_HPRT_POCCHNG;                  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Clear Port Interrupts */
  USBx_HPRT0 = hprt0_dup;                               /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/**
  * @}
  */

#endif /* defined (USB_OTG_FS) || defined (USB_OTG_HS) */
#endif /* HAL_HCD_MODULE_ENABLED */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
