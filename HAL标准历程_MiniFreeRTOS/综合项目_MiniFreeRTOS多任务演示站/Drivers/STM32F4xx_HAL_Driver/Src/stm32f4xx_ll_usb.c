/* 文件: stm32f4xx_ll_usb.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_ll_usb.c
  * @author  MCD Application Team
  * @brief   USB Low Layer HAL module driver.
  *
  *          This file provides firmware functions to manage the following
  *          functionalities of the USB Peripheral Controller:
  *           + Initialization/de-initialization functions
  *           + I/O operation functions
  *           + Peripheral Control functions
  *           + Peripheral State functions
  *
  @verbatim
  ==============================================================================
                    ##### How to use this driver #####
  ==============================================================================
    [..]
      (#) Fill parameters of Init structure in USB_OTG_CfgTypeDef structure.

      (#) Call USB_CoreInit() API to initialize the USB Core peripheral.

      (#) The upper HAL HCD/PCD driver will call the right routines for its internal processes.

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

/** @addtogroup STM32F4xx_LL_USB_DRIVER
  * @{
  */

#if defined (HAL_PCD_MODULE_ENABLED) || defined (HAL_HCD_MODULE_ENABLED)  /* 条件编译 */
#if defined (USB_OTG_FS) || defined (USB_OTG_HS)        /* 条件编译 */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
#if defined (USB_OTG_FS) || defined (USB_OTG_HS)        /* 条件编译 */
static HAL_StatusTypeDef USB_CoreReset(USB_OTG_GlobalTypeDef *USBx);  /* 静态变量/函数 */

/* Exported functions --------------------------------------------------------*/
/** @defgroup USB_LL_Exported_Functions USB Low Layer Exported Functions
  * @{
  */

/** @defgroup USB_LL_Exported_Functions_Group1 Initialization/de-initialization functions
  *  @brief    Initialization and Configuration functions
  *
@verbatim
 ===============================================================================
                      ##### Initialization/de-initialization functions #####
 ===============================================================================

@endverbatim
  * @{
  */

/**
  * @brief  Initializes the USB Core
  * @param  USBx USB Instance
  * @param  cfg pointer to a USB_OTG_CfgTypeDef structure that contains
  *         the configuration information for the specified USBx peripheral.
  * @retval HAL status
  */
HAL_StatusTypeDef USB_CoreInit(USB_OTG_GlobalTypeDef *USBx, USB_OTG_CfgTypeDef cfg)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef ret;                                /* HAL状态类型函数 */

  if (cfg.phy_itface == USB_OTG_ULPI_PHY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    USBx->GCCFG &= ~(USB_OTG_GCCFG_PWRDWN);             /* 赋值或初始化 */

    /* Init The ULPI Interface */
    USBx->GUSBCFG &= ~(USB_OTG_GUSBCFG_TSDPS | USB_OTG_GUSBCFG_ULPIFSLS | USB_OTG_GUSBCFG_PHYSEL);  /* 赋值或初始化 */

    /* Select vbus source */
    USBx->GUSBCFG &= ~(USB_OTG_GUSBCFG_ULPIEVBUSD | USB_OTG_GUSBCFG_ULPIEVBUSI);  /* 赋值或初始化 */
    if (cfg.use_external_vbus == 1U)                    /* 条件判断 */
    {                                                   /* 代码块开始 */
      USBx->GUSBCFG |= USB_OTG_GUSBCFG_ULPIEVBUSD;      /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    /* Reset after a PHY select  */
    ret = USB_CoreReset(USBx);                          /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else /* FS interface (embedded Phy) */
  {                                                     /* 代码块开始 */
    /* Select FS Embedded PHY */
    USBx->GUSBCFG |= USB_OTG_GUSBCFG_PHYSEL;            /* 赋值或初始化 */

    /* Reset after a PHY select */
    ret = USB_CoreReset(USBx);                          /* 赋值或初始化 */

    if (cfg.battery_charging_enable == 0U)              /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Activate the USB Transceiver */
      USBx->GCCFG |= USB_OTG_GCCFG_PWRDWN;              /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Deactivate the USB Transceiver */
      USBx->GCCFG &= ~(USB_OTG_GCCFG_PWRDWN);           /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  if (cfg.dma_enable == 1U)                             /* 条件判断 */
  {                                                     /* 代码块开始 */
    USBx->GAHBCFG |= USB_OTG_GAHBCFG_HBSTLEN_2;         /* 赋值或初始化 */
    USBx->GAHBCFG |= USB_OTG_GAHBCFG_DMAEN;             /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return ret;                                           /* 返回值 */
}                                                       /* 代码块结束 */


/**
  * @brief  Set the USB turnaround time
  * @param  USBx USB Instance
  * @param  hclk: AHB clock frequency
  * @retval USB turnaround time In PHY Clocks number
  */
HAL_StatusTypeDef USB_SetTurnaroundTime(USB_OTG_GlobalTypeDef *USBx,  /* HAL状态类型函数 */
                                        uint32_t hclk, uint8_t speed)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t UsbTrd;                                      /* 32位无符号变量/函数 */

  /* The USBTRD is configured according to the tables below, depending on AHB frequency
  used by application. In the low AHB frequency range it is used to stretch enough the USB response
  time to IN tokens, the USB turnaround time, so to compensate for the longer AHB read access
  latency to the Data FIFO */
  if (speed == USBD_FS_SPEED)                           /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((hclk >= 14200000U) && (hclk < 15000000U))      /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* hclk Clock Range between 14.2-15 MHz */
      UsbTrd = 0xFU;                                    /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if ((hclk >= 15000000U) && (hclk < 16000000U))  /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* hclk Clock Range between 15-16 MHz */
      UsbTrd = 0xEU;                                    /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if ((hclk >= 16000000U) && (hclk < 17200000U))  /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* hclk Clock Range between 16-17.2 MHz */
      UsbTrd = 0xDU;                                    /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if ((hclk >= 17200000U) && (hclk < 18500000U))  /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* hclk Clock Range between 17.2-18.5 MHz */
      UsbTrd = 0xCU;                                    /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if ((hclk >= 18500000U) && (hclk < 20000000U))  /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* hclk Clock Range between 18.5-20 MHz */
      UsbTrd = 0xBU;                                    /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if ((hclk >= 20000000U) && (hclk < 21800000U))  /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* hclk Clock Range between 20-21.8 MHz */
      UsbTrd = 0xAU;                                    /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if ((hclk >= 21800000U) && (hclk < 24000000U))  /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* hclk Clock Range between 21.8-24 MHz */
      UsbTrd = 0x9U;                                    /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if ((hclk >= 24000000U) && (hclk < 27700000U))  /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* hclk Clock Range between 24-27.7 MHz */
      UsbTrd = 0x8U;                                    /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if ((hclk >= 27700000U) && (hclk < 32000000U))  /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* hclk Clock Range between 27.7-32 MHz */
      UsbTrd = 0x7U;                                    /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else /* if(hclk >= 32000000) */
    {                                                   /* 代码块开始 */
      /* hclk Clock Range between 32-200 MHz */
      UsbTrd = 0x6U;                                    /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (speed == USBD_HS_SPEED)                      /* 否则如果 */
  {                                                     /* 代码块开始 */
    UsbTrd = USBD_HS_TRDT_VALUE;                        /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    UsbTrd = USBD_DEFAULT_TRDT_VALUE;                   /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  USBx->GUSBCFG &= ~USB_OTG_GUSBCFG_TRDT;               /* 赋值或初始化 */
  USBx->GUSBCFG |= (uint32_t)((UsbTrd << 10) & USB_OTG_GUSBCFG_TRDT);  /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  USB_EnableGlobalInt
  *         Enables the controller's Global Int in the AHB Config reg
  * @param  USBx  Selected device
  * @retval HAL status
  */
HAL_StatusTypeDef USB_EnableGlobalInt(USB_OTG_GlobalTypeDef *USBx)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  USBx->GAHBCFG |= USB_OTG_GAHBCFG_GINT;                /* 赋值或初始化 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  USB_DisableGlobalInt
  *         Disable the controller's Global Int in the AHB Config reg
  * @param  USBx  Selected device
  * @retval HAL status
  */
HAL_StatusTypeDef USB_DisableGlobalInt(USB_OTG_GlobalTypeDef *USBx)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  USBx->GAHBCFG &= ~USB_OTG_GAHBCFG_GINT;               /* 赋值或初始化 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  USB_SetCurrentMode Set functional mode
  * @param  USBx  Selected device
  * @param  mode  current core mode
  *          This parameter can be one of these values:
  *            @arg USB_DEVICE_MODE Peripheral mode
  *            @arg USB_HOST_MODE Host mode
  * @retval HAL status
  */
HAL_StatusTypeDef USB_SetCurrentMode(USB_OTG_GlobalTypeDef *USBx, USB_OTG_ModeTypeDef mode)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  USBx->GUSBCFG &= ~(USB_OTG_GUSBCFG_FHMOD | USB_OTG_GUSBCFG_FDMOD);  /* 赋值或初始化 */

  if (mode == USB_HOST_MODE)                            /* 条件判断 */
  {                                                     /* 代码块开始 */
    USBx->GUSBCFG |= USB_OTG_GUSBCFG_FHMOD;             /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if (mode == USB_DEVICE_MODE)                     /* 否则如果 */
  {                                                     /* 代码块开始 */
    USBx->GUSBCFG |= USB_OTG_GUSBCFG_FDMOD;             /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  HAL_Delay(50U);                                       /* HAL库函数调用 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  USB_DevInit Initializes the USB_OTG controller registers
  *         for device mode
  * @param  USBx  Selected device
  * @param  cfg   pointer to a USB_OTG_CfgTypeDef structure that contains
  *         the configuration information for the specified USBx peripheral.
  * @retval HAL status
  */
HAL_StatusTypeDef USB_DevInit(USB_OTG_GlobalTypeDef *USBx, USB_OTG_CfgTypeDef cfg)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef ret = HAL_OK;                       /* HAL状态类型函数 */
  uint32_t USBx_BASE = (uint32_t)USBx;                  /* 32位无符号变量/函数 */
  uint32_t i;                                           /* 32位无符号变量/函数 */

  for (i = 0U; i < 15U; i++)                            /* for循环 */
  {                                                     /* 代码块开始 */
    USBx->DIEPTXF[i] = 0U;                              /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

#if defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx) || defined(STM32F412Zx) || defined(STM32F412Vx) || defined(STM32F412Rx) || defined(STM32F412Cx) || defined(STM32F413xx) || defined(STM32F423xx)  /* 条件编译 */
  /* VBUS Sensing setup */
  if (cfg.vbus_sensing_enable == 0U)                    /* 条件判断 */
  {                                                     /* 代码块开始 */
    USBx_DEVICE->DCTL |= USB_OTG_DCTL_SDIS;             /* 赋值或初始化 */

    /* Deactivate VBUS Sensing B */
    USBx->GCCFG &= ~USB_OTG_GCCFG_VBDEN;                /* 赋值或初始化 */

    /* B-peripheral session valid override enable */
    USBx->GOTGCTL |= USB_OTG_GOTGCTL_BVALOEN;           /* 赋值或初始化 */
    USBx->GOTGCTL |= USB_OTG_GOTGCTL_BVALOVAL;          /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Enable HW VBUS sensing */
    USBx->GCCFG |= USB_OTG_GCCFG_VBDEN;                 /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
#else                                                   /* 条件编译否则 */
  /* VBUS Sensing setup */
  if (cfg.vbus_sensing_enable == 0U)                    /* 条件判断 */
  {                                                     /* 代码块开始 */
    /*
     * Disable HW VBUS sensing. VBUS is internally considered to be always
     * at VBUS-Valid level (5V).
     */
    USBx_DEVICE->DCTL |= USB_OTG_DCTL_SDIS;             /* 赋值或初始化 */
    USBx->GCCFG |= USB_OTG_GCCFG_NOVBUSSENS;            /* 赋值或初始化 */
    USBx->GCCFG &= ~USB_OTG_GCCFG_VBUSBSEN;             /* 赋值或初始化 */
    USBx->GCCFG &= ~USB_OTG_GCCFG_VBUSASEN;             /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Enable HW VBUS sensing */
    USBx->GCCFG &= ~USB_OTG_GCCFG_NOVBUSSENS;           /* 赋值或初始化 */
    USBx->GCCFG |= USB_OTG_GCCFG_VBUSBSEN;              /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
#endif /* defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx) || defined(STM32F412Zx) || defined(STM32F412Vx) || defined(STM32F412Rx) || defined(STM32F412Cx) || defined(STM32F413xx) || defined(STM32F423xx) */

  /* Restart the Phy Clock */
  USBx_PCGCCTL = 0U;                                    /* 赋值或初始化 */

  /* Device mode configuration */
  USBx_DEVICE->DCFG |= DCFG_FRAME_INTERVAL_80;          /* 赋值或初始化 */

  if (cfg.phy_itface == USB_OTG_ULPI_PHY)               /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (cfg.speed == USBD_HS_SPEED)                     /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set Core speed to High speed mode */
      (void)USB_SetDevSpeed(USBx, USB_OTG_SPEED_HIGH);  /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Set Core speed to Full speed mode */
      (void)USB_SetDevSpeed(USBx, USB_OTG_SPEED_HIGH_IN_FULL);  /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Set Core speed to Full speed mode */
    (void)USB_SetDevSpeed(USBx, USB_OTG_SPEED_FULL);    /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Flush the FIFOs */
  if (USB_FlushTxFifo(USBx, 0x10U) != HAL_OK) /* all Tx FIFOs */
  {                                                     /* 代码块开始 */
    ret = HAL_ERROR;                                    /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if (USB_FlushRxFifo(USBx) != HAL_OK)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    ret = HAL_ERROR;                                    /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Clear all pending Device Interrupts */
  USBx_DEVICE->DIEPMSK = 0U;                            /* 赋值或初始化 */
  USBx_DEVICE->DOEPMSK = 0U;                            /* 赋值或初始化 */
  USBx_DEVICE->DAINTMSK = 0U;                           /* 赋值或初始化 */

  for (i = 0U; i < cfg.dev_endpoints; i++)              /* for循环 */
  {                                                     /* 代码块开始 */
    if ((USBx_INEP(i)->DIEPCTL & USB_OTG_DIEPCTL_EPENA) == USB_OTG_DIEPCTL_EPENA)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (i == 0U)                                      /* 条件判断 */
      {                                                 /* 代码块开始 */
        USBx_INEP(i)->DIEPCTL = USB_OTG_DIEPCTL_SNAK;   /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        USBx_INEP(i)->DIEPCTL = USB_OTG_DIEPCTL_EPDIS | USB_OTG_DIEPCTL_SNAK;  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      USBx_INEP(i)->DIEPCTL = 0U;                       /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    USBx_INEP(i)->DIEPTSIZ = 0U;                        /* 赋值或初始化 */
    USBx_INEP(i)->DIEPINT  = 0xFB7FU;                   /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  for (i = 0U; i < cfg.dev_endpoints; i++)              /* for循环 */
  {                                                     /* 代码块开始 */
    if ((USBx_OUTEP(i)->DOEPCTL & USB_OTG_DOEPCTL_EPENA) == USB_OTG_DOEPCTL_EPENA)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (i == 0U)                                      /* 条件判断 */
      {                                                 /* 代码块开始 */
        USBx_OUTEP(i)->DOEPCTL = USB_OTG_DOEPCTL_SNAK;  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        USBx_OUTEP(i)->DOEPCTL = USB_OTG_DOEPCTL_EPDIS | USB_OTG_DOEPCTL_SNAK;  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      USBx_OUTEP(i)->DOEPCTL = 0U;                      /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    USBx_OUTEP(i)->DOEPTSIZ = 0U;                       /* 赋值或初始化 */
    USBx_OUTEP(i)->DOEPINT  = 0xFB7FU;                  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  USBx_DEVICE->DIEPMSK &= ~(USB_OTG_DIEPMSK_TXFURM);    /* 赋值或初始化 */

  /* Disable all interrupts. */
  USBx->GINTMSK = 0U;                                   /* 赋值或初始化 */

  /* Clear any pending interrupts */
  USBx->GINTSTS = 0xBFFFFFFFU;                          /* 赋值或初始化 */

  /* Enable the common interrupts */
  if (cfg.dma_enable == 0U)                             /* 条件判断 */
  {                                                     /* 代码块开始 */
    USBx->GINTMSK |= USB_OTG_GINTMSK_RXFLVLM;           /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Enable interrupts matching to the Device mode ONLY */
  USBx->GINTMSK |= USB_OTG_GINTMSK_USBSUSPM | USB_OTG_GINTMSK_USBRST |  /* 源代码 */
                   USB_OTG_GINTMSK_ENUMDNEM | USB_OTG_GINTMSK_IEPINT |  /* 源代码 */
                   USB_OTG_GINTMSK_OEPINT   | USB_OTG_GINTMSK_IISOIXFRM |  /* 源代码 */
                   USB_OTG_GINTMSK_PXFRM_IISOOXFRM | USB_OTG_GINTMSK_WUIM;  /* 执行语句 */

  if (cfg.Sof_enable != 0U)                             /* 条件判断 */
  {                                                     /* 代码块开始 */
    USBx->GINTMSK |= USB_OTG_GINTMSK_SOFM;              /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if (cfg.vbus_sensing_enable == 1U)                    /* 条件判断 */
  {                                                     /* 代码块开始 */
    USBx->GINTMSK |= (USB_OTG_GINTMSK_SRQIM | USB_OTG_GINTMSK_OTGINT);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return ret;                                           /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  USB_OTG_FlushTxFifo : Flush a Tx FIFO
  * @param  USBx  Selected device
  * @param  num  FIFO number
  *         This parameter can be a value from 1 to 15
            15 means Flush all Tx FIFOs
  * @retval HAL status
  */
HAL_StatusTypeDef USB_FlushTxFifo(USB_OTG_GlobalTypeDef *USBx, uint32_t num)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t count = 0U;                                  /* 32位无符号变量/函数 */

  USBx->GRSTCTL = (USB_OTG_GRSTCTL_TXFFLSH | (num << 6));  /* 赋值或初始化 */

  do                                                    /* do-while循环 */
  {                                                     /* 代码块开始 */
    if (++count > 200000U)                              /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_TIMEOUT;                               /* 返回值 */
    }                                                   /* 代码块结束 */
  } while ((USBx->GRSTCTL & USB_OTG_GRSTCTL_TXFFLSH) == USB_OTG_GRSTCTL_TXFFLSH);  /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  USB_FlushRxFifo : Flush Rx FIFO
  * @param  USBx  Selected device
  * @retval HAL status
  */
HAL_StatusTypeDef USB_FlushRxFifo(USB_OTG_GlobalTypeDef *USBx)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t count = 0;                                   /* 32位无符号变量/函数 */

  USBx->GRSTCTL = USB_OTG_GRSTCTL_RXFFLSH;              /* 赋值或初始化 */

  do                                                    /* do-while循环 */
  {                                                     /* 代码块开始 */
    if (++count > 200000U)                              /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_TIMEOUT;                               /* 返回值 */
    }                                                   /* 代码块结束 */
  } while ((USBx->GRSTCTL & USB_OTG_GRSTCTL_RXFFLSH) == USB_OTG_GRSTCTL_RXFFLSH);  /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  USB_SetDevSpeed  Initializes the DevSpd field of DCFG register
  *         depending the PHY type and the enumeration speed of the device.
  * @param  USBx  Selected device
  * @param  speed  device speed
  *          This parameter can be one of these values:
  *            @arg USB_OTG_SPEED_HIGH: High speed mode
  *            @arg USB_OTG_SPEED_HIGH_IN_FULL: High speed core in Full Speed mode
  *            @arg USB_OTG_SPEED_FULL: Full speed mode
  * @retval  Hal status
  */
HAL_StatusTypeDef USB_SetDevSpeed(USB_OTG_GlobalTypeDef *USBx, uint8_t speed)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t USBx_BASE = (uint32_t)USBx;                  /* 32位无符号变量/函数 */

  USBx_DEVICE->DCFG |= speed;                           /* 赋值或初始化 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  USB_GetDevSpeed  Return the Dev Speed
  * @param  USBx  Selected device
  * @retval speed  device speed
  *          This parameter can be one of these values:
  *            @arg USBD_HS_SPEED: High speed mode
  *            @arg USBD_FS_SPEED: Full speed mode
  */
uint8_t USB_GetDevSpeed(USB_OTG_GlobalTypeDef *USBx)    /* 8位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t USBx_BASE = (uint32_t)USBx;                  /* 32位无符号变量/函数 */
  uint8_t speed;                                        /* 8位无符号变量/函数 */
  uint32_t DevEnumSpeed = USBx_DEVICE->DSTS & USB_OTG_DSTS_ENUMSPD;  /* 32位无符号变量/函数 */

  if (DevEnumSpeed == DSTS_ENUMSPD_HS_PHY_30MHZ_OR_60MHZ)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    speed = USBD_HS_SPEED;                              /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if ((DevEnumSpeed == DSTS_ENUMSPD_FS_PHY_30MHZ_OR_60MHZ) ||  /* 否则如果 */
           (DevEnumSpeed == DSTS_ENUMSPD_FS_PHY_48MHZ))  /* 源代码 */
  {                                                     /* 代码块开始 */
    speed = USBD_FS_SPEED;                              /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    speed = 0xFU;                                       /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return speed;                                         /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Activate and configure an endpoint
  * @param  USBx  Selected device
  * @param  ep pointer to endpoint structure
  * @retval HAL status
  */
HAL_StatusTypeDef USB_ActivateEndpoint(USB_OTG_GlobalTypeDef *USBx, USB_OTG_EPTypeDef *ep)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t USBx_BASE = (uint32_t)USBx;                  /* 32位无符号变量/函数 */
  uint32_t epnum = (uint32_t)ep->num;                   /* 32位无符号变量/函数 */

  if (ep->is_in == 1U)                                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    USBx_DEVICE->DAINTMSK |= USB_OTG_DAINTMSK_IEPM & (uint32_t)(1UL << (ep->num & EP_ADDR_MSK));  /* 赋值或初始化 */

    if ((USBx_INEP(epnum)->DIEPCTL & USB_OTG_DIEPCTL_USBAEP) == 0U)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      USBx_INEP(epnum)->DIEPCTL |= (ep->maxpacket & USB_OTG_DIEPCTL_MPSIZ) |  /* 源代码 */
                                   ((uint32_t)ep->type << 18) | (epnum << 22) |  /* 源代码 */
                                   USB_OTG_DIEPCTL_SD0PID_SEVNFRM |  /* 源代码 */
                                   USB_OTG_DIEPCTL_USBAEP;  /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    USBx_DEVICE->DAINTMSK |= USB_OTG_DAINTMSK_OEPM & ((uint32_t)(1UL << (ep->num & EP_ADDR_MSK)) << 16);  /* 赋值或初始化 */

    if (((USBx_OUTEP(epnum)->DOEPCTL) & USB_OTG_DOEPCTL_USBAEP) == 0U)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      USBx_OUTEP(epnum)->DOEPCTL |= (ep->maxpacket & USB_OTG_DOEPCTL_MPSIZ) |  /* 源代码 */
                                    ((uint32_t)ep->type << 18) |  /* 源代码 */
                                    USB_OTG_DIEPCTL_SD0PID_SEVNFRM |  /* 源代码 */
                                    USB_OTG_DOEPCTL_USBAEP;  /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Activate and configure a dedicated endpoint
  * @param  USBx  Selected device
  * @param  ep pointer to endpoint structure
  * @retval HAL status
  */
HAL_StatusTypeDef USB_ActivateDedicatedEndpoint(USB_OTG_GlobalTypeDef *USBx, USB_OTG_EPTypeDef *ep)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t USBx_BASE = (uint32_t)USBx;                  /* 32位无符号变量/函数 */
  uint32_t epnum = (uint32_t)ep->num;                   /* 32位无符号变量/函数 */

  /* Read DEPCTLn register */
  if (ep->is_in == 1U)                                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (((USBx_INEP(epnum)->DIEPCTL) & USB_OTG_DIEPCTL_USBAEP) == 0U)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      USBx_INEP(epnum)->DIEPCTL |= (ep->maxpacket & USB_OTG_DIEPCTL_MPSIZ) |  /* 源代码 */
                                   ((uint32_t)ep->type << 18) | (epnum << 22) |  /* 源代码 */
                                   USB_OTG_DIEPCTL_SD0PID_SEVNFRM |  /* 源代码 */
                                   USB_OTG_DIEPCTL_USBAEP;  /* 执行语句 */
    }                                                   /* 代码块结束 */

    USBx_DEVICE->DEACHMSK |= USB_OTG_DAINTMSK_IEPM & (uint32_t)(1UL << (ep->num & EP_ADDR_MSK));  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    if (((USBx_OUTEP(epnum)->DOEPCTL) & USB_OTG_DOEPCTL_USBAEP) == 0U)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      USBx_OUTEP(epnum)->DOEPCTL |= (ep->maxpacket & USB_OTG_DOEPCTL_MPSIZ) |  /* 源代码 */
                                    ((uint32_t)ep->type << 18) | (epnum << 22) |  /* 源代码 */
                                    USB_OTG_DOEPCTL_USBAEP;  /* 执行语句 */
    }                                                   /* 代码块结束 */

    USBx_DEVICE->DEACHMSK |= USB_OTG_DAINTMSK_OEPM & ((uint32_t)(1UL << (ep->num & EP_ADDR_MSK)) << 16);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  De-activate and de-initialize an endpoint
  * @param  USBx  Selected device
  * @param  ep pointer to endpoint structure
  * @retval HAL status
  */
HAL_StatusTypeDef USB_DeactivateEndpoint(USB_OTG_GlobalTypeDef *USBx, USB_OTG_EPTypeDef *ep)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t USBx_BASE = (uint32_t)USBx;                  /* 32位无符号变量/函数 */
  uint32_t epnum = (uint32_t)ep->num;                   /* 32位无符号变量/函数 */

  /* Read DEPCTLn register */
  if (ep->is_in == 1U)                                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((USBx_INEP(epnum)->DIEPCTL & USB_OTG_DIEPCTL_EPENA) == USB_OTG_DIEPCTL_EPENA)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      USBx_INEP(epnum)->DIEPCTL |= USB_OTG_DIEPCTL_SNAK;  /* 赋值或初始化 */
      USBx_INEP(epnum)->DIEPCTL |= USB_OTG_DIEPCTL_EPDIS;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    USBx_DEVICE->DEACHMSK &= ~(USB_OTG_DAINTMSK_IEPM & (uint32_t)(1UL << (ep->num & EP_ADDR_MSK)));  /* 赋值或初始化 */
    USBx_DEVICE->DAINTMSK &= ~(USB_OTG_DAINTMSK_IEPM & (uint32_t)(1UL << (ep->num & EP_ADDR_MSK)));  /* 赋值或初始化 */
    USBx_INEP(epnum)->DIEPCTL &= ~(USB_OTG_DIEPCTL_USBAEP |  /* 源代码 */
                                   USB_OTG_DIEPCTL_MPSIZ |  /* 源代码 */
                                   USB_OTG_DIEPCTL_TXFNUM |  /* 源代码 */
                                   USB_OTG_DIEPCTL_SD0PID_SEVNFRM |  /* 源代码 */
                                   USB_OTG_DIEPCTL_EPTYP);  /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    if ((USBx_OUTEP(epnum)->DOEPCTL & USB_OTG_DOEPCTL_EPENA) == USB_OTG_DOEPCTL_EPENA)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      USBx_OUTEP(epnum)->DOEPCTL |= USB_OTG_DOEPCTL_SNAK;  /* 赋值或初始化 */
      USBx_OUTEP(epnum)->DOEPCTL |= USB_OTG_DOEPCTL_EPDIS;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    USBx_DEVICE->DEACHMSK &= ~(USB_OTG_DAINTMSK_OEPM & ((uint32_t)(1UL << (ep->num & EP_ADDR_MSK)) << 16));  /* 赋值或初始化 */
    USBx_DEVICE->DAINTMSK &= ~(USB_OTG_DAINTMSK_OEPM & ((uint32_t)(1UL << (ep->num & EP_ADDR_MSK)) << 16));  /* 赋值或初始化 */
    USBx_OUTEP(epnum)->DOEPCTL &= ~(USB_OTG_DOEPCTL_USBAEP |  /* 源代码 */
                                    USB_OTG_DOEPCTL_MPSIZ |  /* 源代码 */
                                    USB_OTG_DOEPCTL_SD0PID_SEVNFRM |  /* 源代码 */
                                    USB_OTG_DOEPCTL_EPTYP);  /* 执行语句 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  De-activate and de-initialize a dedicated endpoint
  * @param  USBx  Selected device
  * @param  ep pointer to endpoint structure
  * @retval HAL status
  */
HAL_StatusTypeDef USB_DeactivateDedicatedEndpoint(USB_OTG_GlobalTypeDef *USBx, USB_OTG_EPTypeDef *ep)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t USBx_BASE = (uint32_t)USBx;                  /* 32位无符号变量/函数 */
  uint32_t epnum = (uint32_t)ep->num;                   /* 32位无符号变量/函数 */

  /* Read DEPCTLn register */
  if (ep->is_in == 1U)                                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((USBx_INEP(epnum)->DIEPCTL & USB_OTG_DIEPCTL_EPENA) == USB_OTG_DIEPCTL_EPENA)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      USBx_INEP(epnum)->DIEPCTL  |= USB_OTG_DIEPCTL_SNAK;  /* 赋值或初始化 */
      USBx_INEP(epnum)->DIEPCTL  |= USB_OTG_DIEPCTL_EPDIS;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    USBx_INEP(epnum)->DIEPCTL &= ~ USB_OTG_DIEPCTL_USBAEP;  /* 赋值或初始化 */
    USBx_DEVICE->DAINTMSK &= ~(USB_OTG_DAINTMSK_IEPM & (uint32_t)(1UL << (ep->num & EP_ADDR_MSK)));  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    if ((USBx_OUTEP(epnum)->DOEPCTL & USB_OTG_DOEPCTL_EPENA) == USB_OTG_DOEPCTL_EPENA)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      USBx_OUTEP(epnum)->DOEPCTL  |= USB_OTG_DOEPCTL_SNAK;  /* 赋值或初始化 */
      USBx_OUTEP(epnum)->DOEPCTL  |= USB_OTG_DOEPCTL_EPDIS;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    USBx_OUTEP(epnum)->DOEPCTL &= ~USB_OTG_DOEPCTL_USBAEP;  /* 赋值或初始化 */
    USBx_DEVICE->DAINTMSK &= ~(USB_OTG_DAINTMSK_OEPM & ((uint32_t)(1UL << (ep->num & EP_ADDR_MSK)) << 16));  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  USB_EPStartXfer : setup and starts a transfer over an EP
  * @param  USBx  Selected device
  * @param  ep pointer to endpoint structure
  * @param  dma USB dma enabled or disabled
  *          This parameter can be one of these values:
  *           0 : DMA feature not used
  *           1 : DMA feature used
  * @retval HAL status
  */
HAL_StatusTypeDef USB_EPStartXfer(USB_OTG_GlobalTypeDef *USBx, USB_OTG_EPTypeDef *ep, uint8_t dma)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t USBx_BASE = (uint32_t)USBx;                  /* 32位无符号变量/函数 */
  uint32_t epnum = (uint32_t)ep->num;                   /* 32位无符号变量/函数 */
  uint16_t pktcnt;                                      /* 16位无符号变量/函数 */

  /* IN endpoint */
  if (ep->is_in == 1U)                                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Zero Length Packet? */
    if (ep->xfer_len == 0U)                             /* 条件判断 */
    {                                                   /* 代码块开始 */
      USBx_INEP(epnum)->DIEPTSIZ &= ~(USB_OTG_DIEPTSIZ_PKTCNT);  /* 赋值或初始化 */
      USBx_INEP(epnum)->DIEPTSIZ |= (USB_OTG_DIEPTSIZ_PKTCNT & (1U << 19));  /* 赋值或初始化 */
      USBx_INEP(epnum)->DIEPTSIZ &= ~(USB_OTG_DIEPTSIZ_XFRSIZ);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Program the transfer size and packet count
      * as follows: xfersize = N * maxpacket +
      * short_packet pktcnt = N + (short_packet
      * exist ? 1 : 0)
      */
      USBx_INEP(epnum)->DIEPTSIZ &= ~(USB_OTG_DIEPTSIZ_XFRSIZ);  /* 赋值或初始化 */
      USBx_INEP(epnum)->DIEPTSIZ &= ~(USB_OTG_DIEPTSIZ_PKTCNT);  /* 赋值或初始化 */
      USBx_INEP(epnum)->DIEPTSIZ |= (USB_OTG_DIEPTSIZ_PKTCNT & (((ep->xfer_len + ep->maxpacket - 1U) / ep->maxpacket) << 19));  /* 赋值或初始化 */
      USBx_INEP(epnum)->DIEPTSIZ |= (USB_OTG_DIEPTSIZ_XFRSIZ & ep->xfer_len);  /* 赋值或初始化 */

      if (ep->type == EP_TYPE_ISOC)                     /* 条件判断 */
      {                                                 /* 代码块开始 */
        USBx_INEP(epnum)->DIEPTSIZ &= ~(USB_OTG_DIEPTSIZ_MULCNT);  /* 赋值或初始化 */
        USBx_INEP(epnum)->DIEPTSIZ |= (USB_OTG_DIEPTSIZ_MULCNT & (1U << 29));  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    if (dma == 1U)                                      /* 条件判断 */
    {                                                   /* 代码块开始 */
      if ((uint32_t)ep->dma_addr != 0U)                 /* 条件判断 */
      {                                                 /* 代码块开始 */
        USBx_INEP(epnum)->DIEPDMA = (uint32_t)(ep->dma_addr);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */

      if (ep->type == EP_TYPE_ISOC)                     /* 条件判断 */
      {                                                 /* 代码块开始 */
        if ((USBx_DEVICE->DSTS & (1U << 8)) == 0U)      /* 条件判断 */
        {                                               /* 代码块开始 */
          USBx_INEP(epnum)->DIEPCTL |= USB_OTG_DIEPCTL_SODDFRM;  /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          USBx_INEP(epnum)->DIEPCTL |= USB_OTG_DIEPCTL_SD0PID_SEVNFRM;  /* 赋值或初始化 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */

      /* EP enable, IN data in FIFO */
      USBx_INEP(epnum)->DIEPCTL |= (USB_OTG_DIEPCTL_CNAK | USB_OTG_DIEPCTL_EPENA);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* EP enable, IN data in FIFO */
      USBx_INEP(epnum)->DIEPCTL |= (USB_OTG_DIEPCTL_CNAK | USB_OTG_DIEPCTL_EPENA);  /* 赋值或初始化 */

      if (ep->type != EP_TYPE_ISOC)                     /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Enable the Tx FIFO Empty Interrupt for this EP */
        if (ep->xfer_len > 0U)                          /* 条件判断 */
        {                                               /* 代码块开始 */
          USBx_DEVICE->DIEPEMPMSK |= 1UL << (ep->num & EP_ADDR_MSK);  /* 赋值或初始化 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        if ((USBx_DEVICE->DSTS & (1U << 8)) == 0U)      /* 条件判断 */
        {                                               /* 代码块开始 */
          USBx_INEP(epnum)->DIEPCTL |= USB_OTG_DIEPCTL_SODDFRM;  /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          USBx_INEP(epnum)->DIEPCTL |= USB_OTG_DIEPCTL_SD0PID_SEVNFRM;  /* 赋值或初始化 */
        }                                               /* 代码块结束 */

        (void)USB_WritePacket(USBx, ep->xfer_buff, ep->num, (uint16_t)ep->xfer_len, dma);  /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else /* OUT endpoint */
  {                                                     /* 代码块开始 */
    /* Program the transfer size and packet count as follows:
    * pktcnt = N
    * xfersize = N * maxpacket
    */
    USBx_OUTEP(epnum)->DOEPTSIZ &= ~(USB_OTG_DOEPTSIZ_XFRSIZ);  /* 赋值或初始化 */
    USBx_OUTEP(epnum)->DOEPTSIZ &= ~(USB_OTG_DOEPTSIZ_PKTCNT);  /* 赋值或初始化 */

    if (ep->xfer_len == 0U)                             /* 条件判断 */
    {                                                   /* 代码块开始 */
      USBx_OUTEP(epnum)->DOEPTSIZ |= (USB_OTG_DOEPTSIZ_XFRSIZ & ep->maxpacket);  /* 赋值或初始化 */
      USBx_OUTEP(epnum)->DOEPTSIZ |= (USB_OTG_DOEPTSIZ_PKTCNT & (1U << 19));  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      pktcnt = (uint16_t)((ep->xfer_len + ep->maxpacket - 1U) / ep->maxpacket);  /* 赋值或初始化 */
      USBx_OUTEP(epnum)->DOEPTSIZ |= USB_OTG_DOEPTSIZ_PKTCNT & ((uint32_t)pktcnt << 19);  /* 赋值或初始化 */
      USBx_OUTEP(epnum)->DOEPTSIZ |= USB_OTG_DOEPTSIZ_XFRSIZ & (ep->maxpacket * pktcnt);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    if (dma == 1U)                                      /* 条件判断 */
    {                                                   /* 代码块开始 */
      if ((uint32_t)ep->xfer_buff != 0U)                /* 条件判断 */
      {                                                 /* 代码块开始 */
        USBx_OUTEP(epnum)->DOEPDMA = (uint32_t)(ep->xfer_buff);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    if (ep->type == EP_TYPE_ISOC)                       /* 条件判断 */
    {                                                   /* 代码块开始 */
      if ((USBx_DEVICE->DSTS & (1U << 8)) == 0U)        /* 条件判断 */
      {                                                 /* 代码块开始 */
        USBx_OUTEP(epnum)->DOEPCTL |= USB_OTG_DOEPCTL_SODDFRM;  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        USBx_OUTEP(epnum)->DOEPCTL |= USB_OTG_DOEPCTL_SD0PID_SEVNFRM;  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    /* EP enable */
    USBx_OUTEP(epnum)->DOEPCTL |= (USB_OTG_DOEPCTL_CNAK | USB_OTG_DOEPCTL_EPENA);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  USB_EP0StartXfer : setup and starts a transfer over the EP  0
  * @param  USBx  Selected device
  * @param  ep pointer to endpoint structure
  * @param  dma USB dma enabled or disabled
  *          This parameter can be one of these values:
  *           0 : DMA feature not used
  *           1 : DMA feature used
  * @retval HAL status
  */
HAL_StatusTypeDef USB_EP0StartXfer(USB_OTG_GlobalTypeDef *USBx, USB_OTG_EPTypeDef *ep, uint8_t dma)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t USBx_BASE = (uint32_t)USBx;                  /* 32位无符号变量/函数 */
  uint32_t epnum = (uint32_t)ep->num;                   /* 32位无符号变量/函数 */

  /* IN endpoint */
  if (ep->is_in == 1U)                                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Zero Length Packet? */
    if (ep->xfer_len == 0U)                             /* 条件判断 */
    {                                                   /* 代码块开始 */
      USBx_INEP(epnum)->DIEPTSIZ &= ~(USB_OTG_DIEPTSIZ_PKTCNT);  /* 赋值或初始化 */
      USBx_INEP(epnum)->DIEPTSIZ |= (USB_OTG_DIEPTSIZ_PKTCNT & (1U << 19));  /* 赋值或初始化 */
      USBx_INEP(epnum)->DIEPTSIZ &= ~(USB_OTG_DIEPTSIZ_XFRSIZ);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Program the transfer size and packet count
      * as follows: xfersize = N * maxpacket +
      * short_packet pktcnt = N + (short_packet
      * exist ? 1 : 0)
      */
      USBx_INEP(epnum)->DIEPTSIZ &= ~(USB_OTG_DIEPTSIZ_XFRSIZ);  /* 赋值或初始化 */
      USBx_INEP(epnum)->DIEPTSIZ &= ~(USB_OTG_DIEPTSIZ_PKTCNT);  /* 赋值或初始化 */

      if (ep->xfer_len > ep->maxpacket)                 /* 条件判断 */
      {                                                 /* 代码块开始 */
        ep->xfer_len = ep->maxpacket;                   /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      USBx_INEP(epnum)->DIEPTSIZ |= (USB_OTG_DIEPTSIZ_PKTCNT & (1U << 19));  /* 赋值或初始化 */
      USBx_INEP(epnum)->DIEPTSIZ |= (USB_OTG_DIEPTSIZ_XFRSIZ & ep->xfer_len);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    if (dma == 1U)                                      /* 条件判断 */
    {                                                   /* 代码块开始 */
      if ((uint32_t)ep->dma_addr != 0U)                 /* 条件判断 */
      {                                                 /* 代码块开始 */
        USBx_INEP(epnum)->DIEPDMA = (uint32_t)(ep->dma_addr);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */

      /* EP enable, IN data in FIFO */
      USBx_INEP(epnum)->DIEPCTL |= (USB_OTG_DIEPCTL_CNAK | USB_OTG_DIEPCTL_EPENA);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* EP enable, IN data in FIFO */
      USBx_INEP(epnum)->DIEPCTL |= (USB_OTG_DIEPCTL_CNAK | USB_OTG_DIEPCTL_EPENA);  /* 赋值或初始化 */

      /* Enable the Tx FIFO Empty Interrupt for this EP */
      if (ep->xfer_len > 0U)                            /* 条件判断 */
      {                                                 /* 代码块开始 */
        USBx_DEVICE->DIEPEMPMSK |= 1UL << (ep->num & EP_ADDR_MSK);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else /* OUT endpoint */
  {                                                     /* 代码块开始 */
    /* Program the transfer size and packet count as follows:
    * pktcnt = N
    * xfersize = N * maxpacket
    */
    USBx_OUTEP(epnum)->DOEPTSIZ &= ~(USB_OTG_DOEPTSIZ_XFRSIZ);  /* 赋值或初始化 */
    USBx_OUTEP(epnum)->DOEPTSIZ &= ~(USB_OTG_DOEPTSIZ_PKTCNT);  /* 赋值或初始化 */

    if (ep->xfer_len > 0U)                              /* 条件判断 */
    {                                                   /* 代码块开始 */
      ep->xfer_len = ep->maxpacket;                     /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    USBx_OUTEP(epnum)->DOEPTSIZ |= (USB_OTG_DOEPTSIZ_PKTCNT & (1U << 19));  /* 赋值或初始化 */
    USBx_OUTEP(epnum)->DOEPTSIZ |= (USB_OTG_DOEPTSIZ_XFRSIZ & (ep->maxpacket));  /* 赋值或初始化 */

    if (dma == 1U)                                      /* 条件判断 */
    {                                                   /* 代码块开始 */
      if ((uint32_t)ep->xfer_buff != 0U)                /* 条件判断 */
      {                                                 /* 代码块开始 */
        USBx_OUTEP(epnum)->DOEPDMA = (uint32_t)(ep->xfer_buff);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /* EP enable */
    USBx_OUTEP(epnum)->DOEPCTL |= (USB_OTG_DOEPCTL_CNAK | USB_OTG_DOEPCTL_EPENA);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  USB_WritePacket : Writes a packet into the Tx FIFO associated
  *         with the EP/channel
  * @param  USBx  Selected device
  * @param  src   pointer to source buffer
  * @param  ch_ep_num  endpoint or host channel number
  * @param  len  Number of bytes to write
  * @param  dma USB dma enabled or disabled
  *          This parameter can be one of these values:
  *           0 : DMA feature not used
  *           1 : DMA feature used
  * @retval HAL status
  */
HAL_StatusTypeDef USB_WritePacket(USB_OTG_GlobalTypeDef *USBx, uint8_t *src,  /* HAL状态类型函数 */
                                  uint8_t ch_ep_num, uint16_t len, uint8_t dma)  /* 8位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t USBx_BASE = (uint32_t)USBx;                  /* 32位无符号变量/函数 */
  uint32_t *pSrc = (uint32_t *)src;                     /* 32位无符号变量/函数 */
  uint32_t count32b, i;                                 /* 32位无符号变量/函数 */

  if (dma == 0U)                                        /* 条件判断 */
  {                                                     /* 代码块开始 */
    count32b = ((uint32_t)len + 3U) / 4U;               /* 赋值或初始化 */
    for (i = 0U; i < count32b; i++)                     /* for循环 */
    {                                                   /* 代码块开始 */
      USBx_DFIFO((uint32_t)ch_ep_num) = __UNALIGNED_UINT32_READ(pSrc);  /* 赋值或初始化 */
      pSrc++;                                           /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  USB_ReadPacket : read a packet from the RX FIFO
  * @param  USBx  Selected device
  * @param  dest  source pointer
  * @param  len  Number of bytes to read
  * @retval pointer to destination buffer
  */
void *USB_ReadPacket(USB_OTG_GlobalTypeDef *USBx, uint8_t *dest, uint16_t len)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  uint32_t USBx_BASE = (uint32_t)USBx;                  /* 32位无符号变量/函数 */
  uint32_t *pDest = (uint32_t *)dest;                   /* 32位无符号变量/函数 */
  uint32_t i;                                           /* 32位无符号变量/函数 */
  uint32_t count32b = ((uint32_t)len + 3U) / 4U;        /* 32位无符号变量/函数 */

  for (i = 0U; i < count32b; i++)                       /* for循环 */
  {                                                     /* 代码块开始 */
    __UNALIGNED_UINT32_WRITE(pDest, USBx_DFIFO(0U));    /* 执行语句 */
    pDest++;                                            /* 执行语句 */
  }                                                     /* 代码块结束 */

  return ((void *)pDest);                               /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  USB_EPSetStall : set a stall condition over an EP
  * @param  USBx  Selected device
  * @param  ep pointer to endpoint structure
  * @retval HAL status
  */
HAL_StatusTypeDef USB_EPSetStall(USB_OTG_GlobalTypeDef *USBx, USB_OTG_EPTypeDef *ep)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t USBx_BASE = (uint32_t)USBx;                  /* 32位无符号变量/函数 */
  uint32_t epnum = (uint32_t)ep->num;                   /* 32位无符号变量/函数 */

  if (ep->is_in == 1U)                                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (((USBx_INEP(epnum)->DIEPCTL & USB_OTG_DIEPCTL_EPENA) == 0U) && (epnum != 0U))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      USBx_INEP(epnum)->DIEPCTL &= ~(USB_OTG_DIEPCTL_EPDIS);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    USBx_INEP(epnum)->DIEPCTL |= USB_OTG_DIEPCTL_STALL;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    if (((USBx_OUTEP(epnum)->DOEPCTL & USB_OTG_DOEPCTL_EPENA) == 0U) && (epnum != 0U))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      USBx_OUTEP(epnum)->DOEPCTL &= ~(USB_OTG_DOEPCTL_EPDIS);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    USBx_OUTEP(epnum)->DOEPCTL |= USB_OTG_DOEPCTL_STALL;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  USB_EPClearStall : Clear a stall condition over an EP
  * @param  USBx  Selected device
  * @param  ep pointer to endpoint structure
  * @retval HAL status
  */
HAL_StatusTypeDef USB_EPClearStall(USB_OTG_GlobalTypeDef *USBx, USB_OTG_EPTypeDef *ep)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t USBx_BASE = (uint32_t)USBx;                  /* 32位无符号变量/函数 */
  uint32_t epnum = (uint32_t)ep->num;                   /* 32位无符号变量/函数 */

  if (ep->is_in == 1U)                                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    USBx_INEP(epnum)->DIEPCTL &= ~USB_OTG_DIEPCTL_STALL;  /* 赋值或初始化 */
    if ((ep->type == EP_TYPE_INTR) || (ep->type == EP_TYPE_BULK))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      USBx_INEP(epnum)->DIEPCTL |= USB_OTG_DIEPCTL_SD0PID_SEVNFRM; /* DATA0 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    USBx_OUTEP(epnum)->DOEPCTL &= ~USB_OTG_DOEPCTL_STALL;  /* 赋值或初始化 */
    if ((ep->type == EP_TYPE_INTR) || (ep->type == EP_TYPE_BULK))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      USBx_OUTEP(epnum)->DOEPCTL |= USB_OTG_DOEPCTL_SD0PID_SEVNFRM; /* DATA0 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  USB_StopDevice : Stop the usb device mode
  * @param  USBx  Selected device
  * @retval HAL status
  */
HAL_StatusTypeDef USB_StopDevice(USB_OTG_GlobalTypeDef *USBx)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef ret;                                /* HAL状态类型函数 */
  uint32_t USBx_BASE = (uint32_t)USBx;                  /* 32位无符号变量/函数 */
  uint32_t i;                                           /* 32位无符号变量/函数 */

  /* Clear Pending interrupt */
  for (i = 0U; i < 15U; i++)                            /* for循环 */
  {                                                     /* 代码块开始 */
    USBx_INEP(i)->DIEPINT = 0xFB7FU;                    /* 赋值或初始化 */
    USBx_OUTEP(i)->DOEPINT = 0xFB7FU;                   /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Clear interrupt masks */
  USBx_DEVICE->DIEPMSK  = 0U;                           /* 赋值或初始化 */
  USBx_DEVICE->DOEPMSK  = 0U;                           /* 赋值或初始化 */
  USBx_DEVICE->DAINTMSK = 0U;                           /* 赋值或初始化 */

  /* Flush the FIFO */
  ret = USB_FlushRxFifo(USBx);                          /* 赋值或初始化 */
  if (ret != HAL_OK)                                    /* 条件判断 */
  {                                                     /* 代码块开始 */
    return ret;                                         /* 返回值 */
  }                                                     /* 代码块结束 */

  ret = USB_FlushTxFifo(USBx,  0x10U);                  /* 赋值或初始化 */
  if (ret != HAL_OK)                                    /* 条件判断 */
  {                                                     /* 代码块开始 */
    return ret;                                         /* 返回值 */
  }                                                     /* 代码块结束 */

  return ret;                                           /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  USB_SetDevAddress : Stop the usb device mode
  * @param  USBx  Selected device
  * @param  address  new device address to be assigned
  *          This parameter can be a value from 0 to 255
  * @retval HAL status
  */
HAL_StatusTypeDef  USB_SetDevAddress(USB_OTG_GlobalTypeDef *USBx, uint8_t address)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t USBx_BASE = (uint32_t)USBx;                  /* 32位无符号变量/函数 */

  USBx_DEVICE->DCFG &= ~(USB_OTG_DCFG_DAD);             /* 赋值或初始化 */
  USBx_DEVICE->DCFG |= ((uint32_t)address << 4) & USB_OTG_DCFG_DAD;  /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  USB_DevConnect : Connect the USB device by enabling Rpu
  * @param  USBx  Selected device
  * @retval HAL status
  */
HAL_StatusTypeDef  USB_DevConnect(USB_OTG_GlobalTypeDef *USBx)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t USBx_BASE = (uint32_t)USBx;                  /* 32位无符号变量/函数 */

  /* In case phy is stopped, ensure to ungate and restore the phy CLK */
  USBx_PCGCCTL &= ~(USB_OTG_PCGCCTL_STOPCLK | USB_OTG_PCGCCTL_GATECLK);  /* 赋值或初始化 */

  USBx_DEVICE->DCTL &= ~USB_OTG_DCTL_SDIS;              /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  USB_DevDisconnect : Disconnect the USB device by disabling Rpu
  * @param  USBx  Selected device
  * @retval HAL status
  */
HAL_StatusTypeDef  USB_DevDisconnect(USB_OTG_GlobalTypeDef *USBx)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t USBx_BASE = (uint32_t)USBx;                  /* 32位无符号变量/函数 */

  /* In case phy is stopped, ensure to ungate and restore the phy CLK */
  USBx_PCGCCTL &= ~(USB_OTG_PCGCCTL_STOPCLK | USB_OTG_PCGCCTL_GATECLK);  /* 赋值或初始化 */

  USBx_DEVICE->DCTL |= USB_OTG_DCTL_SDIS;               /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  USB_ReadInterrupts: return the global USB interrupt status
  * @param  USBx  Selected device
  * @retval HAL status
  */
uint32_t  USB_ReadInterrupts(USB_OTG_GlobalTypeDef *USBx)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t tmpreg;                                      /* 32位无符号变量/函数 */

  tmpreg = USBx->GINTSTS;                               /* 赋值或初始化 */
  tmpreg &= USBx->GINTMSK;                              /* 赋值或初始化 */

  return tmpreg;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  USB_ReadDevAllOutEpInterrupt: return the USB device OUT endpoints interrupt status
  * @param  USBx  Selected device
  * @retval HAL status
  */
uint32_t USB_ReadDevAllOutEpInterrupt(USB_OTG_GlobalTypeDef *USBx)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t USBx_BASE = (uint32_t)USBx;                  /* 32位无符号变量/函数 */
  uint32_t tmpreg;                                      /* 32位无符号变量/函数 */

  tmpreg  = USBx_DEVICE->DAINT;                         /* 赋值或初始化 */
  tmpreg &= USBx_DEVICE->DAINTMSK;                      /* 赋值或初始化 */

  return ((tmpreg & 0xffff0000U) >> 16);                /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  USB_ReadDevAllInEpInterrupt: return the USB device IN endpoints interrupt status
  * @param  USBx  Selected device
  * @retval HAL status
  */
uint32_t USB_ReadDevAllInEpInterrupt(USB_OTG_GlobalTypeDef *USBx)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t USBx_BASE = (uint32_t)USBx;                  /* 32位无符号变量/函数 */
  uint32_t tmpreg;                                      /* 32位无符号变量/函数 */

  tmpreg  = USBx_DEVICE->DAINT;                         /* 赋值或初始化 */
  tmpreg &= USBx_DEVICE->DAINTMSK;                      /* 赋值或初始化 */

  return ((tmpreg & 0xFFFFU));                          /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Returns Device OUT EP Interrupt register
  * @param  USBx  Selected device
  * @param  epnum  endpoint number
  *          This parameter can be a value from 0 to 15
  * @retval Device OUT EP Interrupt register
  */
uint32_t USB_ReadDevOutEPInterrupt(USB_OTG_GlobalTypeDef *USBx, uint8_t epnum)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t USBx_BASE = (uint32_t)USBx;                  /* 32位无符号变量/函数 */
  uint32_t tmpreg;                                      /* 32位无符号变量/函数 */

  tmpreg  = USBx_OUTEP((uint32_t)epnum)->DOEPINT;       /* 赋值或初始化 */
  tmpreg &= USBx_DEVICE->DOEPMSK;                       /* 赋值或初始化 */

  return tmpreg;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Returns Device IN EP Interrupt register
  * @param  USBx  Selected device
  * @param  epnum  endpoint number
  *          This parameter can be a value from 0 to 15
  * @retval Device IN EP Interrupt register
  */
uint32_t USB_ReadDevInEPInterrupt(USB_OTG_GlobalTypeDef *USBx, uint8_t epnum)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t USBx_BASE = (uint32_t)USBx;                  /* 32位无符号变量/函数 */
  uint32_t tmpreg, msk, emp;                            /* 32位无符号变量/函数 */

  msk = USBx_DEVICE->DIEPMSK;                           /* 赋值或初始化 */
  emp = USBx_DEVICE->DIEPEMPMSK;                        /* 赋值或初始化 */
  msk |= ((emp >> (epnum & EP_ADDR_MSK)) & 0x1U) << 7;  /* 赋值或初始化 */
  tmpreg = USBx_INEP((uint32_t)epnum)->DIEPINT & msk;   /* 赋值或初始化 */

  return tmpreg;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  USB_ClearInterrupts: clear a USB interrupt
  * @param  USBx  Selected device
  * @param  interrupt  flag
  * @retval None
  */
void  USB_ClearInterrupts(USB_OTG_GlobalTypeDef *USBx, uint32_t interrupt)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  USBx->GINTSTS |= interrupt;                           /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
  * @brief  Returns USB core mode
  * @param  USBx  Selected device
  * @retval return core mode : Host or Device
  *          This parameter can be one of these values:
  *           0 : Host
  *           1 : Device
  */
uint32_t USB_GetMode(USB_OTG_GlobalTypeDef *USBx)       /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  return ((USBx->GINTSTS) & 0x1U);                      /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Activate EP0 for Setup transactions
  * @param  USBx  Selected device
  * @retval HAL status
  */
HAL_StatusTypeDef  USB_ActivateSetup(USB_OTG_GlobalTypeDef *USBx)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t USBx_BASE = (uint32_t)USBx;                  /* 32位无符号变量/函数 */

  /* Set the MPS of the IN EP0 to 64 bytes */
  USBx_INEP(0U)->DIEPCTL &= ~USB_OTG_DIEPCTL_MPSIZ;     /* 赋值或初始化 */

  USBx_DEVICE->DCTL |= USB_OTG_DCTL_CGINAK;             /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Prepare the EP0 to start the first control setup
  * @param  USBx  Selected device
  * @param  dma USB dma enabled or disabled
  *          This parameter can be one of these values:
  *           0 : DMA feature not used
  *           1 : DMA feature used
  * @param  psetup  pointer to setup packet
  * @retval HAL status
  */
HAL_StatusTypeDef USB_EP0_OutStart(USB_OTG_GlobalTypeDef *USBx, uint8_t dma, uint8_t *psetup)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t USBx_BASE = (uint32_t)USBx;                  /* 32位无符号变量/函数 */
  uint32_t gSNPSiD = *(__IO uint32_t *)(&USBx->CID + 0x1U);  /* 32位无符号变量/函数 */

  if (gSNPSiD > USB_OTG_CORE_ID_300A)                   /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((USBx_OUTEP(0U)->DOEPCTL & USB_OTG_DOEPCTL_EPENA) == USB_OTG_DOEPCTL_EPENA)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_OK;                                    /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  USBx_OUTEP(0U)->DOEPTSIZ = 0U;                        /* 赋值或初始化 */
  USBx_OUTEP(0U)->DOEPTSIZ |= (USB_OTG_DOEPTSIZ_PKTCNT & (1U << 19));  /* 赋值或初始化 */
  USBx_OUTEP(0U)->DOEPTSIZ |= (3U * 8U);                /* 赋值或初始化 */
  USBx_OUTEP(0U)->DOEPTSIZ |=  USB_OTG_DOEPTSIZ_STUPCNT;  /* 赋值或初始化 */

  if (dma == 1U)                                        /* 条件判断 */
  {                                                     /* 代码块开始 */
    USBx_OUTEP(0U)->DOEPDMA = (uint32_t)psetup;         /* 赋值或初始化 */
    /* EP enable */
    USBx_OUTEP(0U)->DOEPCTL |= USB_OTG_DOEPCTL_EPENA | USB_OTG_DOEPCTL_USBAEP;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Reset the USB Core (needed after USB clock settings change)
  * @param  USBx  Selected device
  * @retval HAL status
  */
static HAL_StatusTypeDef USB_CoreReset(USB_OTG_GlobalTypeDef *USBx)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t count = 0U;                                  /* 32位无符号变量/函数 */

  /* Wait for AHB master IDLE state. */
  do                                                    /* do-while循环 */
  {                                                     /* 代码块开始 */
    if (++count > 200000U)                              /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_TIMEOUT;                               /* 返回值 */
    }                                                   /* 代码块结束 */
  } while ((USBx->GRSTCTL & USB_OTG_GRSTCTL_AHBIDL) == 0U);  /* 代码块结束 */

  /* Core Soft Reset */
  count = 0U;                                           /* 赋值或初始化 */
  USBx->GRSTCTL |= USB_OTG_GRSTCTL_CSRST;               /* 赋值或初始化 */

  do                                                    /* do-while循环 */
  {                                                     /* 代码块开始 */
    if (++count > 200000U)                              /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_TIMEOUT;                               /* 返回值 */
    }                                                   /* 代码块结束 */
  } while ((USBx->GRSTCTL & USB_OTG_GRSTCTL_CSRST) == USB_OTG_GRSTCTL_CSRST);  /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  USB_HostInit : Initializes the USB OTG controller registers
  *         for Host mode
  * @param  USBx  Selected device
  * @param  cfg   pointer to a USB_OTG_CfgTypeDef structure that contains
  *         the configuration information for the specified USBx peripheral.
  * @retval HAL status
  */
HAL_StatusTypeDef USB_HostInit(USB_OTG_GlobalTypeDef *USBx, USB_OTG_CfgTypeDef cfg)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t USBx_BASE = (uint32_t)USBx;                  /* 32位无符号变量/函数 */
  uint32_t i;                                           /* 32位无符号变量/函数 */

  /* Restart the Phy Clock */
  USBx_PCGCCTL = 0U;                                    /* 赋值或初始化 */

#if defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx) || defined(STM32F412Zx) || defined(STM32F412Vx) || defined(STM32F412Rx) || defined(STM32F412Cx) || defined(STM32F413xx) || defined(STM32F423xx)  /* 条件编译 */
  /* Disable HW VBUS sensing */
  USBx->GCCFG &= ~(USB_OTG_GCCFG_VBDEN);                /* 赋值或初始化 */
#else                                                   /* 条件编译否则 */
  /*
  * Disable HW VBUS sensing. VBUS is internally considered to be always
  * at VBUS-Valid level (5V).
  */
  USBx->GCCFG |= USB_OTG_GCCFG_NOVBUSSENS;              /* 赋值或初始化 */
  USBx->GCCFG &= ~USB_OTG_GCCFG_VBUSBSEN;               /* 赋值或初始化 */
  USBx->GCCFG &= ~USB_OTG_GCCFG_VBUSASEN;               /* 赋值或初始化 */
#endif /* defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx) || defined(STM32F412Zx) || defined(STM32F412Vx) || defined(STM32F412Rx) || defined(STM32F412Cx) || defined(STM32F413xx) || defined(STM32F423xx) */
#if defined(STM32F412Zx) || defined(STM32F412Vx) || defined(STM32F412Rx) || defined(STM32F412Cx) || defined(STM32F413xx) || defined(STM32F423xx)  /* 条件编译 */
  /* Disable Battery chargin detector */
  USBx->GCCFG &= ~(USB_OTG_GCCFG_BCDEN);                /* 赋值或初始化 */
#endif /* defined(STM32F412Zx) || defined(STM32F412Vx) || defined(STM32F412Rx) || defined(STM32F412Cx) || defined(STM32F413xx) || defined(STM32F423xx) */

  if ((USBx->CID & (0x1U << 8)) != 0U)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (cfg.speed == USBH_FSLS_SPEED)                   /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Force Device Enumeration to FS/LS mode only */
      USBx_HOST->HCFG |= USB_OTG_HCFG_FSLSS;            /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Set default Max speed support */
      USBx_HOST->HCFG &= ~(USB_OTG_HCFG_FSLSS);         /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Set default Max speed support */
    USBx_HOST->HCFG &= ~(USB_OTG_HCFG_FSLSS);           /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Make sure the FIFOs are flushed. */
  (void)USB_FlushTxFifo(USBx, 0x10U); /* all Tx FIFOs */
  (void)USB_FlushRxFifo(USBx);                          /* 执行语句 */

  /* Clear all pending HC Interrupts */
  for (i = 0U; i < cfg.Host_channels; i++)              /* for循环 */
  {                                                     /* 代码块开始 */
    USBx_HC(i)->HCINT = 0xFFFFFFFFU;                    /* 赋值或初始化 */
    USBx_HC(i)->HCINTMSK = 0U;                          /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Enable VBUS driving */
  (void)USB_DriveVbus(USBx, 1U);                        /* 执行语句 */

  HAL_Delay(200U);                                      /* HAL库函数调用 */

  /* Disable all interrupts. */
  USBx->GINTMSK = 0U;                                   /* 赋值或初始化 */

  /* Clear any pending interrupts */
  USBx->GINTSTS = 0xFFFFFFFFU;                          /* 赋值或初始化 */

  if ((USBx->CID & (0x1U << 8)) != 0U)                  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* set Rx FIFO size */
    USBx->GRXFSIZ  = 0x200U;                            /* 赋值或初始化 */
    USBx->DIEPTXF0_HNPTXFSIZ = (uint32_t)(((0x100U << 16) & USB_OTG_NPTXFD) | 0x200U);  /* 赋值或初始化 */
    USBx->HPTXFSIZ = (uint32_t)(((0xE0U << 16) & USB_OTG_HPTXFSIZ_PTXFD) | 0x300U);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* set Rx FIFO size */
    USBx->GRXFSIZ  = 0x80U;                             /* 赋值或初始化 */
    USBx->DIEPTXF0_HNPTXFSIZ = (uint32_t)(((0x60U << 16) & USB_OTG_NPTXFD) | 0x80U);  /* 赋值或初始化 */
    USBx->HPTXFSIZ = (uint32_t)(((0x40U << 16)& USB_OTG_HPTXFSIZ_PTXFD) | 0xE0U);  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Enable the common interrupts */
  if (cfg.dma_enable == 0U)                             /* 条件判断 */
  {                                                     /* 代码块开始 */
    USBx->GINTMSK |= USB_OTG_GINTMSK_RXFLVLM;           /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Enable interrupts matching to the Host mode ONLY */
  USBx->GINTMSK |= (USB_OTG_GINTMSK_PRTIM            | USB_OTG_GINTMSK_HCIM | \
                    USB_OTG_GINTMSK_SOFM             | USB_OTG_GINTSTS_DISCINT | \
                    USB_OTG_GINTMSK_PXFRM_IISOOXFRM  | USB_OTG_GINTMSK_WUIM);  /* 执行语句 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  USB_InitFSLSPClkSel : Initializes the FSLSPClkSel field of the
  *         HCFG register on the PHY type and set the right frame interval
  * @param  USBx  Selected device
  * @param  freq  clock frequency
  *          This parameter can be one of these values:
  *           HCFG_48_MHZ : Full Speed 48 MHz Clock
  *           HCFG_6_MHZ : Low Speed 6 MHz Clock
  * @retval HAL status
  */
HAL_StatusTypeDef USB_InitFSLSPClkSel(USB_OTG_GlobalTypeDef *USBx, uint8_t freq)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t USBx_BASE = (uint32_t)USBx;                  /* 32位无符号变量/函数 */

  USBx_HOST->HCFG &= ~(USB_OTG_HCFG_FSLSPCS);           /* 赋值或初始化 */
  USBx_HOST->HCFG |= (uint32_t)freq & USB_OTG_HCFG_FSLSPCS;  /* 赋值或初始化 */

  if (freq == HCFG_48_MHZ)                              /* 条件判断 */
  {                                                     /* 代码块开始 */
    USBx_HOST->HFIR = 48000U;                           /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else if (freq == HCFG_6_MHZ)                          /* 否则如果 */
  {                                                     /* 代码块开始 */
    USBx_HOST->HFIR = 6000U;                            /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* ... */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  USB_OTG_ResetPort : Reset Host Port
  * @param  USBx  Selected device
  * @retval HAL status
  * @note (1)The application must wait at least 10 ms
  *   before clearing the reset bit.
  */
HAL_StatusTypeDef USB_ResetPort(USB_OTG_GlobalTypeDef *USBx)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t USBx_BASE = (uint32_t)USBx;                  /* 32位无符号变量/函数 */

  __IO uint32_t hprt0 = 0U;                             /* 赋值或初始化 */

  hprt0 = USBx_HPRT0;                                   /* 赋值或初始化 */

  hprt0 &= ~(USB_OTG_HPRT_PENA | USB_OTG_HPRT_PCDET |   /* 源代码 */
             USB_OTG_HPRT_PENCHNG | USB_OTG_HPRT_POCCHNG);  /* 执行语句 */

  USBx_HPRT0 = (USB_OTG_HPRT_PRST | hprt0);             /* 赋值或初始化 */
  HAL_Delay(100U);                                 /* See Note #1 */
  USBx_HPRT0 = ((~USB_OTG_HPRT_PRST) & hprt0);          /* 赋值或初始化 */
  HAL_Delay(10U);                                       /* HAL库函数调用 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  USB_DriveVbus : activate or de-activate vbus
  * @param  state  VBUS state
  *          This parameter can be one of these values:
  *           0 : Deactivate VBUS
  *           1 : Activate VBUS
  * @retval HAL status
  */
HAL_StatusTypeDef USB_DriveVbus(USB_OTG_GlobalTypeDef *USBx, uint8_t state)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t USBx_BASE = (uint32_t)USBx;                  /* 32位无符号变量/函数 */
  __IO uint32_t hprt0 = 0U;                             /* 赋值或初始化 */

  hprt0 = USBx_HPRT0;                                   /* 赋值或初始化 */

  hprt0 &= ~(USB_OTG_HPRT_PENA | USB_OTG_HPRT_PCDET |   /* 源代码 */
             USB_OTG_HPRT_PENCHNG | USB_OTG_HPRT_POCCHNG);  /* 执行语句 */

  if (((hprt0 & USB_OTG_HPRT_PPWR) == 0U) && (state == 1U))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    USBx_HPRT0 = (USB_OTG_HPRT_PPWR | hprt0);           /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  if (((hprt0 & USB_OTG_HPRT_PPWR) == USB_OTG_HPRT_PPWR) && (state == 0U))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    USBx_HPRT0 = ((~USB_OTG_HPRT_PPWR) & hprt0);        /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return Host Core speed
  * @param  USBx  Selected device
  * @retval speed : Host speed
  *          This parameter can be one of these values:
  *            @arg HCD_SPEED_HIGH: High speed mode
  *            @arg HCD_SPEED_FULL: Full speed mode
  *            @arg HCD_SPEED_LOW: Low speed mode
  */
uint32_t USB_GetHostSpeed(USB_OTG_GlobalTypeDef *USBx)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t USBx_BASE = (uint32_t)USBx;                  /* 32位无符号变量/函数 */
  __IO uint32_t hprt0 = 0U;                             /* 赋值或初始化 */

  hprt0 = USBx_HPRT0;                                   /* 赋值或初始化 */
  return ((hprt0 & USB_OTG_HPRT_PSPD) >> 17);           /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Return Host Current Frame number
  * @param  USBx  Selected device
  * @retval current frame number
  */
uint32_t USB_GetCurrentFrame(USB_OTG_GlobalTypeDef *USBx)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t USBx_BASE = (uint32_t)USBx;                  /* 32位无符号变量/函数 */

  return (USBx_HOST->HFNUM & USB_OTG_HFNUM_FRNUM);      /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initialize a host channel
  * @param  USBx  Selected device
  * @param  ch_num  Channel number
  *         This parameter can be a value from 1 to 15
  * @param  epnum  Endpoint number
  *          This parameter can be a value from 1 to 15
  * @param  dev_address  Current device address
  *          This parameter can be a value from 0 to 255
  * @param  speed  Current device speed
  *          This parameter can be one of these values:
  *            @arg USB_OTG_SPEED_HIGH: High speed mode
  *            @arg USB_OTG_SPEED_FULL: Full speed mode
  *            @arg USB_OTG_SPEED_LOW: Low speed mode
  * @param  ep_type  Endpoint Type
  *          This parameter can be one of these values:
  *            @arg EP_TYPE_CTRL: Control type
  *            @arg EP_TYPE_ISOC: Isochronous type
  *            @arg EP_TYPE_BULK: Bulk type
  *            @arg EP_TYPE_INTR: Interrupt type
  * @param  mps  Max Packet Size
  *          This parameter can be a value from 0 to 32K
  * @retval HAL state
  */
HAL_StatusTypeDef USB_HC_Init(USB_OTG_GlobalTypeDef *USBx, uint8_t ch_num,  /* HAL状态类型函数 */
                              uint8_t epnum, uint8_t dev_address, uint8_t speed,  /* 8位无符号变量/函数 */
                              uint8_t ep_type, uint16_t mps)  /* 8位无符号变量/函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef ret = HAL_OK;                       /* HAL状态类型函数 */
  uint32_t USBx_BASE = (uint32_t)USBx;                  /* 32位无符号变量/函数 */
  uint32_t HCcharEpDir, HCcharLowSpeed;                 /* 32位无符号变量/函数 */
  uint32_t HostCoreSpeed;                               /* 32位无符号变量/函数 */

  /* Clear old interrupt conditions for this host channel. */
  USBx_HC((uint32_t)ch_num)->HCINT = 0xFFFFFFFFU;       /* 赋值或初始化 */

  /* Enable channel interrupts required for this transfer. */
  switch (ep_type)                                      /* switch分支 */
  {                                                     /* 代码块开始 */
    case EP_TYPE_CTRL:                                  /* case标签 */
    case EP_TYPE_BULK:                                  /* case标签 */
      USBx_HC((uint32_t)ch_num)->HCINTMSK = USB_OTG_HCINTMSK_XFRCM  |  /* 源代码 */
                                            USB_OTG_HCINTMSK_STALLM |  /* 源代码 */
                                            USB_OTG_HCINTMSK_TXERRM |  /* 源代码 */
                                            USB_OTG_HCINTMSK_DTERRM |  /* 源代码 */
                                            USB_OTG_HCINTMSK_AHBERR |  /* 源代码 */
                                            USB_OTG_HCINTMSK_NAKM;  /* 执行语句 */

      if ((epnum & 0x80U) == 0x80U)                     /* 条件判断 */
      {                                                 /* 代码块开始 */
        USBx_HC((uint32_t)ch_num)->HCINTMSK |= USB_OTG_HCINTMSK_BBERRM;  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        if ((USBx->CID & (0x1U << 8)) != 0U)            /* 条件判断 */
        {                                               /* 代码块开始 */
          USBx_HC((uint32_t)ch_num)->HCINTMSK |= USB_OTG_HCINTMSK_NYET |  /* 源代码 */
                                                 USB_OTG_HCINTMSK_ACKM;  /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */

    case EP_TYPE_INTR:                                  /* case标签 */
      USBx_HC((uint32_t)ch_num)->HCINTMSK = USB_OTG_HCINTMSK_XFRCM  |  /* 源代码 */
                                            USB_OTG_HCINTMSK_STALLM |  /* 源代码 */
                                            USB_OTG_HCINTMSK_TXERRM |  /* 源代码 */
                                            USB_OTG_HCINTMSK_DTERRM |  /* 源代码 */
                                            USB_OTG_HCINTMSK_NAKM   |  /* 源代码 */
                                            USB_OTG_HCINTMSK_AHBERR |  /* 源代码 */
                                            USB_OTG_HCINTMSK_FRMORM;  /* 执行语句 */

      if ((epnum & 0x80U) == 0x80U)                     /* 条件判断 */
      {                                                 /* 代码块开始 */
        USBx_HC((uint32_t)ch_num)->HCINTMSK |= USB_OTG_HCINTMSK_BBERRM;  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */

      break;                                            /* 跳出 */

    case EP_TYPE_ISOC:                                  /* case标签 */
      USBx_HC((uint32_t)ch_num)->HCINTMSK = USB_OTG_HCINTMSK_XFRCM  |  /* 源代码 */
                                            USB_OTG_HCINTMSK_ACKM   |  /* 源代码 */
                                            USB_OTG_HCINTMSK_AHBERR |  /* 源代码 */
                                            USB_OTG_HCINTMSK_FRMORM;  /* 执行语句 */

      if ((epnum & 0x80U) == 0x80U)                     /* 条件判断 */
      {                                                 /* 代码块开始 */
        USBx_HC((uint32_t)ch_num)->HCINTMSK |= (USB_OTG_HCINTMSK_TXERRM | USB_OTG_HCINTMSK_BBERRM);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      break;                                            /* 跳出 */

    default:                                            /* default分支 */
      ret = HAL_ERROR;                                  /* 赋值或初始化 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */

  /* Enable the top level host channel interrupt. */
  USBx_HOST->HAINTMSK |= 1UL << (ch_num & 0xFU);        /* 赋值或初始化 */

  /* Make sure host channel interrupts are enabled. */
  USBx->GINTMSK |= USB_OTG_GINTMSK_HCIM;                /* 赋值或初始化 */

  /* Program the HCCHAR register */
  if ((epnum & 0x80U) == 0x80U)                         /* 条件判断 */
  {                                                     /* 代码块开始 */
    HCcharEpDir = (0x1U << 15) & USB_OTG_HCCHAR_EPDIR;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    HCcharEpDir = 0U;                                   /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  HostCoreSpeed = USB_GetHostSpeed(USBx);               /* 赋值或初始化 */

  /* LS device plugged to HUB */
  if ((speed == HPRT0_PRTSPD_LOW_SPEED) && (HostCoreSpeed != HPRT0_PRTSPD_LOW_SPEED))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    HCcharLowSpeed = (0x1U << 17) & USB_OTG_HCCHAR_LSDEV;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    HCcharLowSpeed = 0U;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  USBx_HC((uint32_t)ch_num)->HCCHAR = (((uint32_t)dev_address << 22) & USB_OTG_HCCHAR_DAD) |  /* 源代码 */
                                      ((((uint32_t)epnum & 0x7FU) << 11) & USB_OTG_HCCHAR_EPNUM) |  /* 源代码 */
                                      (((uint32_t)ep_type << 18) & USB_OTG_HCCHAR_EPTYP) |  /* 源代码 */
                                      ((uint32_t)mps & USB_OTG_HCCHAR_MPSIZ) | HCcharEpDir | HCcharLowSpeed;  /* 执行语句 */

  if (ep_type == EP_TYPE_INTR)                          /* 条件判断 */
  {                                                     /* 代码块开始 */
    USBx_HC((uint32_t)ch_num)->HCCHAR |= USB_OTG_HCCHAR_ODDFRM ;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return ret;                                           /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Start a transfer over a host channel
  * @param  USBx  Selected device
  * @param  hc  pointer to host channel structure
  * @param  dma USB dma enabled or disabled
  *          This parameter can be one of these values:
  *           0 : DMA feature not used
  *           1 : DMA feature used
  * @retval HAL state
  */
HAL_StatusTypeDef USB_HC_StartXfer(USB_OTG_GlobalTypeDef *USBx, USB_OTG_HCTypeDef *hc, uint8_t dma)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t USBx_BASE = (uint32_t)USBx;                  /* 32位无符号变量/函数 */
  uint32_t ch_num = (uint32_t)hc->ch_num;               /* 32位无符号变量/函数 */
  __IO uint32_t tmpreg;                                 /* 执行语句 */
  uint8_t  is_oddframe;                                 /* 8位无符号变量/函数 */
  uint16_t len_words;                                   /* 16位无符号变量/函数 */
  uint16_t num_packets;                                 /* 16位无符号变量/函数 */
  uint16_t max_hc_pkt_count = 256U;                     /* 16位无符号变量/函数 */

  if (((USBx->CID & (0x1U << 8)) != 0U) && (hc->speed == USBH_HS_SPEED))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* in DMA mode host Core automatically issues ping  in case of NYET/NAK */
    if ((dma == 1U) && ((hc->ep_type == EP_TYPE_CTRL) || (hc->ep_type == EP_TYPE_BULK)))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      USBx_HC((uint32_t)ch_num)->HCINTMSK &= ~(USB_OTG_HCINTMSK_NYET |  /* 源代码 */
                                               USB_OTG_HCINTMSK_ACKM |  /* 源代码 */
                                               USB_OTG_HCINTMSK_NAKM);  /* 执行语句 */
    }                                                   /* 代码块结束 */

    if ((dma == 0U) && (hc->do_ping == 1U))             /* 条件判断 */
    {                                                   /* 代码块开始 */
      (void)USB_DoPing(USBx, hc->ch_num);               /* 执行语句 */
      return HAL_OK;                                    /* 返回值 */
    }                                                   /* 代码块结束 */

  }                                                     /* 代码块结束 */

  /* Compute the expected number of packets associated to the transfer */
  if (hc->xfer_len > 0U)                                /* 条件判断 */
  {                                                     /* 代码块开始 */
    num_packets = (uint16_t)((hc->xfer_len + hc->max_packet - 1U) / hc->max_packet);  /* 赋值或初始化 */

    if (num_packets > max_hc_pkt_count)                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      num_packets = max_hc_pkt_count;                   /* 赋值或初始化 */
      hc->XferSize = (uint32_t)num_packets * hc->max_packet;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    num_packets = 1U;                                   /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /*
   * For IN channel HCTSIZ.XferSize is expected to be an integer multiple of
   * max_packet size.
   */
  if (hc->ep_is_in != 0U)                               /* 条件判断 */
  {                                                     /* 代码块开始 */
    hc->XferSize = (uint32_t)num_packets * hc->max_packet;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    hc->XferSize = hc->xfer_len;                        /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Initialize the HCTSIZn register */
  USBx_HC(ch_num)->HCTSIZ = (hc->XferSize & USB_OTG_HCTSIZ_XFRSIZ) |  /* 源代码 */
                            (((uint32_t)num_packets << 19) & USB_OTG_HCTSIZ_PKTCNT) |  /* 源代码 */
                            (((uint32_t)hc->data_pid << 29) & USB_OTG_HCTSIZ_DPID);  /* 执行语句 */

  if (dma != 0U)                                        /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* xfer_buff MUST be 32-bits aligned */
    USBx_HC(ch_num)->HCDMA = (uint32_t)hc->xfer_buff;   /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  is_oddframe = (((uint32_t)USBx_HOST->HFNUM & 0x01U) != 0U) ? 0U : 1U;  /* 赋值或初始化 */
  USBx_HC(ch_num)->HCCHAR &= ~USB_OTG_HCCHAR_ODDFRM;    /* 赋值或初始化 */
  USBx_HC(ch_num)->HCCHAR |= (uint32_t)is_oddframe << 29;  /* 赋值或初始化 */

  /* Set host channel enable */
  tmpreg = USBx_HC(ch_num)->HCCHAR;                     /* 赋值或初始化 */
  tmpreg &= ~USB_OTG_HCCHAR_CHDIS;                      /* 赋值或初始化 */

  /* make sure to set the correct ep direction */
  if (hc->ep_is_in != 0U)                               /* 条件判断 */
  {                                                     /* 代码块开始 */
    tmpreg |= USB_OTG_HCCHAR_EPDIR;                     /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    tmpreg &= ~USB_OTG_HCCHAR_EPDIR;                    /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  tmpreg |= USB_OTG_HCCHAR_CHENA;                       /* 赋值或初始化 */
  USBx_HC(ch_num)->HCCHAR = tmpreg;                     /* 赋值或初始化 */

  if (dma != 0U) /* dma mode */
  {                                                     /* 代码块开始 */
    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */

  if ((hc->ep_is_in == 0U) && (hc->xfer_len > 0U))      /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (hc->ep_type)                                /* switch分支 */
    {                                                   /* 代码块开始 */
      /* Non periodic transfer */
      case EP_TYPE_CTRL:                                /* case标签 */
      case EP_TYPE_BULK:                                /* case标签 */

        len_words = (uint16_t)((hc->xfer_len + 3U) / 4U);  /* 赋值或初始化 */

        /* check if there is enough space in FIFO space */
        if (len_words > (USBx->HNPTXSTS & 0xFFFFU))     /* 条件判断 */
        {                                               /* 代码块开始 */
          /* need to process data in nptxfempty interrupt */
          USBx->GINTMSK |= USB_OTG_GINTMSK_NPTXFEM;     /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        break;                                          /* 跳出 */

      /* Periodic transfer */
      case EP_TYPE_INTR:                                /* case标签 */
      case EP_TYPE_ISOC:                                /* case标签 */
        len_words = (uint16_t)((hc->xfer_len + 3U) / 4U);  /* 赋值或初始化 */
        /* check if there is enough space in FIFO space */
        if (len_words > (USBx_HOST->HPTXSTS & 0xFFFFU)) /* split the transfer */
        {                                               /* 代码块开始 */
          /* need to process data in ptxfempty interrupt */
          USBx->GINTMSK |= USB_OTG_GINTMSK_PTXFEM;      /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        break;                                          /* 跳出 */

      default:                                          /* default分支 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */

    /* Write packet into the Tx FIFO. */
    (void)USB_WritePacket(USBx, hc->xfer_buff, hc->ch_num, (uint16_t)hc->xfer_len, 0);  /* 执行语句 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief Read all host channel interrupts status
  * @param  USBx  Selected device
  * @retval HAL state
  */
uint32_t USB_HC_ReadInterrupt(USB_OTG_GlobalTypeDef *USBx)  /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t USBx_BASE = (uint32_t)USBx;                  /* 32位无符号变量/函数 */

  return ((USBx_HOST->HAINT) & 0xFFFFU);                /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Halt a host channel
  * @param  USBx  Selected device
  * @param  hc_num  Host Channel number
  *         This parameter can be a value from 1 to 15
  * @retval HAL state
  */
HAL_StatusTypeDef USB_HC_Halt(USB_OTG_GlobalTypeDef *USBx, uint8_t hc_num)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t USBx_BASE = (uint32_t)USBx;                  /* 32位无符号变量/函数 */
  uint32_t hcnum = (uint32_t)hc_num;                    /* 32位无符号变量/函数 */
  uint32_t count = 0U;                                  /* 32位无符号变量/函数 */
  uint32_t HcEpType = (USBx_HC(hcnum)->HCCHAR & USB_OTG_HCCHAR_EPTYP) >> 18;  /* 32位无符号变量/函数 */
  uint32_t ChannelEna = (USBx_HC(hcnum)->HCCHAR & USB_OTG_HCCHAR_CHENA) >> 31;  /* 32位无符号变量/函数 */

  if (((USBx->GAHBCFG & USB_OTG_GAHBCFG_DMAEN) == USB_OTG_GAHBCFG_DMAEN) &&  /* 条件判断 */
      (ChannelEna == 0U))                               /* 源代码 */
  {                                                     /* 代码块开始 */
    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check for space in the request queue to issue the halt. */
  if ((HcEpType == HCCHAR_CTRL) || (HcEpType == HCCHAR_BULK))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    USBx_HC(hcnum)->HCCHAR |= USB_OTG_HCCHAR_CHDIS;     /* 赋值或初始化 */

    if ((USBx->GAHBCFG & USB_OTG_GAHBCFG_DMAEN) == 0U)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      if ((USBx->HNPTXSTS & (0xFFU << 16)) == 0U)       /* 条件判断 */
      {                                                 /* 代码块开始 */
        USBx_HC(hcnum)->HCCHAR &= ~USB_OTG_HCCHAR_CHENA;  /* 赋值或初始化 */
        USBx_HC(hcnum)->HCCHAR |= USB_OTG_HCCHAR_CHENA;  /* 赋值或初始化 */
        USBx_HC(hcnum)->HCCHAR &= ~USB_OTG_HCCHAR_EPDIR;  /* 赋值或初始化 */
        do                                              /* do-while循环 */
        {                                               /* 代码块开始 */
          if (++count > 1000U)                          /* 条件判断 */
          {                                             /* 代码块开始 */
            break;                                      /* 跳出 */
          }                                             /* 代码块结束 */
        } while ((USBx_HC(hcnum)->HCCHAR & USB_OTG_HCCHAR_CHENA) == USB_OTG_HCCHAR_CHENA);  /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        USBx_HC(hcnum)->HCCHAR |= USB_OTG_HCCHAR_CHENA;  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    USBx_HC(hcnum)->HCCHAR |= USB_OTG_HCCHAR_CHDIS;     /* 赋值或初始化 */

    if ((USBx_HOST->HPTXSTS & (0xFFU << 16)) == 0U)     /* 条件判断 */
    {                                                   /* 代码块开始 */
      USBx_HC(hcnum)->HCCHAR &= ~USB_OTG_HCCHAR_CHENA;  /* 赋值或初始化 */
      USBx_HC(hcnum)->HCCHAR |= USB_OTG_HCCHAR_CHENA;   /* 赋值或初始化 */
      USBx_HC(hcnum)->HCCHAR &= ~USB_OTG_HCCHAR_EPDIR;  /* 赋值或初始化 */
      do                                                /* do-while循环 */
      {                                                 /* 代码块开始 */
        if (++count > 1000U)                            /* 条件判断 */
        {                                               /* 代码块开始 */
          break;                                        /* 跳出 */
        }                                               /* 代码块结束 */
      } while ((USBx_HC(hcnum)->HCCHAR & USB_OTG_HCCHAR_CHENA) == USB_OTG_HCCHAR_CHENA);  /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      USBx_HC(hcnum)->HCCHAR |= USB_OTG_HCCHAR_CHENA;   /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Initiate Do Ping protocol
  * @param  USBx  Selected device
  * @param  hc_num  Host Channel number
  *         This parameter can be a value from 1 to 15
  * @retval HAL state
  */
HAL_StatusTypeDef USB_DoPing(USB_OTG_GlobalTypeDef *USBx, uint8_t ch_num)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t USBx_BASE = (uint32_t)USBx;                  /* 32位无符号变量/函数 */
  uint32_t chnum = (uint32_t)ch_num;                    /* 32位无符号变量/函数 */
  uint32_t num_packets = 1U;                            /* 32位无符号变量/函数 */
  uint32_t tmpreg;                                      /* 32位无符号变量/函数 */

  USBx_HC(chnum)->HCTSIZ = ((num_packets << 19) & USB_OTG_HCTSIZ_PKTCNT) |  /* 源代码 */
                           USB_OTG_HCTSIZ_DOPING;       /* 执行语句 */

  /* Set host channel enable */
  tmpreg = USBx_HC(chnum)->HCCHAR;                      /* 赋值或初始化 */
  tmpreg &= ~USB_OTG_HCCHAR_CHDIS;                      /* 赋值或初始化 */
  tmpreg |= USB_OTG_HCCHAR_CHENA;                       /* 赋值或初始化 */
  USBx_HC(chnum)->HCCHAR = tmpreg;                      /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Stop Host Core
  * @param  USBx  Selected device
  * @retval HAL state
  */
HAL_StatusTypeDef USB_StopHost(USB_OTG_GlobalTypeDef *USBx)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t USBx_BASE = (uint32_t)USBx;                  /* 32位无符号变量/函数 */
  uint32_t count = 0U;                                  /* 32位无符号变量/函数 */
  uint32_t value;                                       /* 32位无符号变量/函数 */
  uint32_t i;                                           /* 32位无符号变量/函数 */

  (void)USB_DisableGlobalInt(USBx);                     /* 执行语句 */

  /* Flush FIFO */
  (void)USB_FlushTxFifo(USBx, 0x10U);                   /* 执行语句 */
  (void)USB_FlushRxFifo(USBx);                          /* 执行语句 */

  /* Flush out any leftover queued requests. */
  for (i = 0U; i <= 15U; i++)                           /* for循环 */
  {                                                     /* 代码块开始 */
    value = USBx_HC(i)->HCCHAR;                         /* 赋值或初始化 */
    value |=  USB_OTG_HCCHAR_CHDIS;                     /* 赋值或初始化 */
    value &= ~USB_OTG_HCCHAR_CHENA;                     /* 赋值或初始化 */
    value &= ~USB_OTG_HCCHAR_EPDIR;                     /* 赋值或初始化 */
    USBx_HC(i)->HCCHAR = value;                         /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Halt all channels to put them into a known state. */
  for (i = 0U; i <= 15U; i++)                           /* for循环 */
  {                                                     /* 代码块开始 */
    value = USBx_HC(i)->HCCHAR;                         /* 赋值或初始化 */
    value |= USB_OTG_HCCHAR_CHDIS;                      /* 赋值或初始化 */
    value |= USB_OTG_HCCHAR_CHENA;                      /* 赋值或初始化 */
    value &= ~USB_OTG_HCCHAR_EPDIR;                     /* 赋值或初始化 */
    USBx_HC(i)->HCCHAR = value;                         /* 赋值或初始化 */

    do                                                  /* do-while循环 */
    {                                                   /* 代码块开始 */
      if (++count > 1000U)                              /* 条件判断 */
      {                                                 /* 代码块开始 */
        break;                                          /* 跳出 */
      }                                                 /* 代码块结束 */
    } while ((USBx_HC(i)->HCCHAR & USB_OTG_HCCHAR_CHENA) == USB_OTG_HCCHAR_CHENA);  /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Clear any pending Host interrupts */
  USBx_HOST->HAINT = 0xFFFFFFFFU;                       /* 赋值或初始化 */
  USBx->GINTSTS = 0xFFFFFFFFU;                          /* 赋值或初始化 */

  (void)USB_EnableGlobalInt(USBx);                      /* 执行语句 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  USB_ActivateRemoteWakeup active remote wakeup signalling
  * @param  USBx Selected device
  * @retval HAL status
  */
HAL_StatusTypeDef USB_ActivateRemoteWakeup(USB_OTG_GlobalTypeDef *USBx)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t USBx_BASE = (uint32_t)USBx;                  /* 32位无符号变量/函数 */

  if ((USBx_DEVICE->DSTS & USB_OTG_DSTS_SUSPSTS) == USB_OTG_DSTS_SUSPSTS)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* active Remote wakeup signalling */
    USBx_DEVICE->DCTL |= USB_OTG_DCTL_RWUSIG;           /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  USB_DeActivateRemoteWakeup de-active remote wakeup signalling
  * @param  USBx Selected device
  * @retval HAL status
  */
HAL_StatusTypeDef USB_DeActivateRemoteWakeup(USB_OTG_GlobalTypeDef *USBx)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t USBx_BASE = (uint32_t)USBx;                  /* 32位无符号变量/函数 */

  /* active Remote wakeup signalling */
  USBx_DEVICE->DCTL &= ~(USB_OTG_DCTL_RWUSIG);          /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* defined (USB_OTG_FS) || defined (USB_OTG_HS) */


/**
  * @}
  */

/**
  * @}
  */
#endif /* defined (USB_OTG_FS) || defined (USB_OTG_HS) */
#endif /* defined (HAL_PCD_MODULE_ENABLED) || defined (HAL_HCD_MODULE_ENABLED) */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
