/* 文件: stm32f4xx_hal_pcd_ex.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_pcd_ex.c
  * @author  MCD Application Team
  * @brief   PCD Extended HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the USB Peripheral Controller:
  *           + Extended features functions
  *
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

/** @defgroup PCDEx PCDEx
  * @brief PCD Extended HAL module driver
  * @{
  */

#ifdef HAL_PCD_MODULE_ENABLED                           /* 条件编译 */

#if defined (USB_OTG_FS) || defined (USB_OTG_HS)        /* 条件编译 */
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/** @defgroup PCDEx_Exported_Functions PCDEx Exported Functions
  * @{
  */

/** @defgroup PCDEx_Exported_Functions_Group1 Peripheral Control functions
  * @brief    PCDEx control functions
  *
@verbatim
 ===============================================================================
                 ##### Extended features functions #####
 ===============================================================================
    [..]  This section provides functions allowing to:
      (+) Update FIFO configuration

@endverbatim
  * @{
  */
#if defined (USB_OTG_FS) || defined (USB_OTG_HS)        /* 条件编译 */
/**
  * @brief  Set Tx FIFO
  * @param  hpcd PCD handle
  * @param  fifo The number of Tx fifo
  * @param  size Fifo size
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCDEx_SetTxFiFo(PCD_HandleTypeDef *hpcd, uint8_t fifo, uint16_t size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint8_t i;                                            /* 8位无符号变量/函数 */
  uint32_t Tx_Offset;                                   /* 32位无符号变量/函数 */

  /*  TXn min size = 16 words. (n  : Transmit FIFO index)
      When a TxFIFO is not used, the Configuration should be as follows:
          case 1 :  n > m    and Txn is not used    (n,m  : Transmit FIFO indexes)
         --> Txm can use the space allocated for Txn.
         case2  :  n < m    and Txn is not used    (n,m  : Transmit FIFO indexes)
         --> Txn should be configured with the minimum space of 16 words
     The FIFO is used optimally when used TxFIFOs are allocated in the top
         of the FIFO.Ex: use EP1 and EP2 as IN instead of EP1 and EP3 as IN ones.
     When DMA is used 3n * FIFO locations should be reserved for internal DMA registers */

  Tx_Offset = hpcd->Instance->GRXFSIZ;                  /* 赋值或初始化 */

  if (fifo == 0U)                                       /* 条件判断 */
  {                                                     /* 代码块开始 */
    hpcd->Instance->DIEPTXF0_HNPTXFSIZ = ((uint32_t)size << 16) | Tx_Offset;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    Tx_Offset += (hpcd->Instance->DIEPTXF0_HNPTXFSIZ) >> 16;  /* 赋值或初始化 */
    for (i = 0U; i < (fifo - 1U); i++)                  /* for循环 */
    {                                                   /* 代码块开始 */
      Tx_Offset += (hpcd->Instance->DIEPTXF[i] >> 16);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Multiply Tx_Size by 2 to get higher performance */
    hpcd->Instance->DIEPTXF[fifo - 1U] = ((uint32_t)size << 16) | Tx_Offset;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Set Rx FIFO
  * @param  hpcd PCD handle
  * @param  size Size of Rx fifo
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCDEx_SetRxFiFo(PCD_HandleTypeDef *hpcd, uint16_t size)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  hpcd->Instance->GRXFSIZ = size;                       /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */
#if defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx) || defined(STM32F412Zx) || defined(STM32F412Vx) || defined(STM32F412Rx) || defined(STM32F412Cx) || defined(STM32F413xx) || defined(STM32F423xx)  /* 条件编译 */
/**
  * @brief  Activate LPM feature.
  * @param  hpcd PCD handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCDEx_ActivateLPM(PCD_HandleTypeDef *hpcd)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  USB_OTG_GlobalTypeDef *USBx = hpcd->Instance;         /* 赋值或初始化 */

  hpcd->lpm_active = 1U;                                /* 赋值或初始化 */
  hpcd->LPM_State = LPM_L0;                             /* 赋值或初始化 */
  USBx->GINTMSK |= USB_OTG_GINTMSK_LPMINTM;             /* 赋值或初始化 */
  USBx->GLPMCFG |= (USB_OTG_GLPMCFG_LPMEN | USB_OTG_GLPMCFG_LPMACK | USB_OTG_GLPMCFG_ENBESL);  /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Deactivate LPM feature.
  * @param  hpcd PCD handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCDEx_DeActivateLPM(PCD_HandleTypeDef *hpcd)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  USB_OTG_GlobalTypeDef *USBx = hpcd->Instance;         /* 赋值或初始化 */

  hpcd->lpm_active = 0U;                                /* 赋值或初始化 */
  USBx->GINTMSK &= ~USB_OTG_GINTMSK_LPMINTM;            /* 赋值或初始化 */
  USBx->GLPMCFG &= ~(USB_OTG_GLPMCFG_LPMEN | USB_OTG_GLPMCFG_LPMACK | USB_OTG_GLPMCFG_ENBESL);  /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx) || defined(STM32F412Zx) || defined(STM32F412Vx) || defined(STM32F412Rx) || defined(STM32F412Cx) || defined(STM32F413xx) || defined(STM32F423xx) */
#if defined(STM32F412Zx) || defined(STM32F412Vx) || defined(STM32F412Rx) || defined(STM32F412Cx) || defined(STM32F413xx) || defined(STM32F423xx)  /* 条件编译 */
/**
  * @brief  Handle BatteryCharging Process.
  * @param  hpcd PCD handle
  * @retval HAL status
  */
void HAL_PCDEx_BCD_VBUSDetect(PCD_HandleTypeDef *hpcd)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
  USB_OTG_GlobalTypeDef *USBx = hpcd->Instance;         /* 赋值或初始化 */
  uint32_t tickstart = HAL_GetTick();                   /* 32位无符号变量/函数 */

  /* Enable DCD : Data Contact Detect */
  USBx->GCCFG |= USB_OTG_GCCFG_DCDEN;                   /* 赋值或初始化 */

  /* Wait Detect flag or a timeout is happen*/
  while ((USBx->GCCFG & USB_OTG_GCCFG_DCDET) == 0U)     /* while循环 */
  {                                                     /* 代码块开始 */
    /* Check for the Timeout */
    if ((HAL_GetTick() - tickstart) > 1000U)            /* 条件判断 */
    {                                                   /* 代码块开始 */
#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
      hpcd->BCDCallback(hpcd, PCD_BCD_ERROR);           /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_PCDEx_BCD_Callback(hpcd, PCD_BCD_ERROR);      /* HAL库函数调用 */
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */

      return;                                           /* 返回 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Right response got */
  HAL_Delay(200U);                                      /* HAL库函数调用 */

  /* Check Detect flag*/
  if ((USBx->GCCFG & USB_OTG_GCCFG_DCDET) == USB_OTG_GCCFG_DCDET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
    hpcd->BCDCallback(hpcd, PCD_BCD_CONTACT_DETECTION);  /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_PCDEx_BCD_Callback(hpcd, PCD_BCD_CONTACT_DETECTION);  /* HAL库函数调用 */
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */

  /*Primary detection: checks if connected to Standard Downstream Port
  (without charging capability) */
  USBx->GCCFG &= ~ USB_OTG_GCCFG_DCDEN;                 /* 赋值或初始化 */
  HAL_Delay(50U);                                       /* HAL库函数调用 */
  USBx->GCCFG |=  USB_OTG_GCCFG_PDEN;                   /* 赋值或初始化 */
  HAL_Delay(50U);                                       /* HAL库函数调用 */

  if ((USBx->GCCFG & USB_OTG_GCCFG_PDET) == 0U)         /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Case of Standard Downstream Port */
#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
    hpcd->BCDCallback(hpcd, PCD_BCD_STD_DOWNSTREAM_PORT);  /* 执行语句 */
#else                                                   /* 条件编译否则 */
    HAL_PCDEx_BCD_Callback(hpcd, PCD_BCD_STD_DOWNSTREAM_PORT);  /* HAL库函数调用 */
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* start secondary detection to check connection to Charging Downstream
    Port or Dedicated Charging Port */
    USBx->GCCFG &= ~ USB_OTG_GCCFG_PDEN;                /* 赋值或初始化 */
    HAL_Delay(50U);                                     /* HAL库函数调用 */
    USBx->GCCFG |=  USB_OTG_GCCFG_SDEN;                 /* 赋值或初始化 */
    HAL_Delay(50U);                                     /* HAL库函数调用 */

    if ((USBx->GCCFG & USB_OTG_GCCFG_SDET) == USB_OTG_GCCFG_SDET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* case Dedicated Charging Port  */
#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
      hpcd->BCDCallback(hpcd, PCD_BCD_DEDICATED_CHARGING_PORT);  /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_PCDEx_BCD_Callback(hpcd, PCD_BCD_DEDICATED_CHARGING_PORT);  /* HAL库函数调用 */
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* case Charging Downstream Port  */
#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
      hpcd->BCDCallback(hpcd, PCD_BCD_CHARGING_DOWNSTREAM_PORT);  /* 执行语句 */
#else                                                   /* 条件编译否则 */
      HAL_PCDEx_BCD_Callback(hpcd, PCD_BCD_CHARGING_DOWNSTREAM_PORT);  /* HAL库函数调用 */
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  /* Battery Charging capability discovery finished */
  (void)HAL_PCDEx_DeActivateBCD(hpcd);                  /* 执行语句 */

#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)              /* 条件编译 */
  hpcd->BCDCallback(hpcd, PCD_BCD_DISCOVERY_COMPLETED);  /* 执行语句 */
#else                                                   /* 条件编译否则 */
  HAL_PCDEx_BCD_Callback(hpcd, PCD_BCD_DISCOVERY_COMPLETED);  /* HAL库函数调用 */
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */
}                                                       /* 代码块结束 */

/**
  * @brief  Activate BatteryCharging feature.
  * @param  hpcd PCD handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCDEx_ActivateBCD(PCD_HandleTypeDef *hpcd)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  USB_OTG_GlobalTypeDef *USBx = hpcd->Instance;         /* 赋值或初始化 */

  USBx->GCCFG &= ~(USB_OTG_GCCFG_PDEN);                 /* 赋值或初始化 */
  USBx->GCCFG &= ~(USB_OTG_GCCFG_SDEN);                 /* 赋值或初始化 */

  /* Power Down USB transceiver  */
  USBx->GCCFG &= ~(USB_OTG_GCCFG_PWRDWN);               /* 赋值或初始化 */

  /* Enable Battery charging */
  USBx->GCCFG |= USB_OTG_GCCFG_BCDEN;                   /* 赋值或初始化 */

  hpcd->battery_charging_active = 1U;                   /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Deactivate BatteryCharging feature.
  * @param  hpcd PCD handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PCDEx_DeActivateBCD(PCD_HandleTypeDef *hpcd)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  USB_OTG_GlobalTypeDef *USBx = hpcd->Instance;         /* 赋值或初始化 */

  USBx->GCCFG &= ~(USB_OTG_GCCFG_SDEN);                 /* 赋值或初始化 */
  USBx->GCCFG &= ~(USB_OTG_GCCFG_PDEN);                 /* 赋值或初始化 */

  /* Disable Battery charging */
  USBx->GCCFG &= ~(USB_OTG_GCCFG_BCDEN);                /* 赋值或初始化 */

  hpcd->battery_charging_active = 0U;                   /* 赋值或初始化 */

  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* defined(STM32F412Zx) || defined(STM32F412Vx) || defined(STM32F412Rx) || defined(STM32F412Cx) || defined(STM32F413xx) || defined(STM32F423xx) */
#endif /* defined (USB_OTG_FS) || defined (USB_OTG_HS) */

/**
  * @brief  Send LPM message to user layer callback.
  * @param  hpcd PCD handle
  * @param  msg LPM message
  * @retval HAL status
  */
__weak void HAL_PCDEx_LPM_Callback(PCD_HandleTypeDef *hpcd, PCD_LPM_MsgTypeDef msg)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hpcd);                                         /* 执行语句 */
  UNUSED(msg);                                          /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_PCDEx_LPM_Callback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Send BatteryCharging message to user layer callback.
  * @param  hpcd PCD handle
  * @param  msg LPM message
  * @retval HAL status
  */
__weak void HAL_PCDEx_BCD_Callback(PCD_HandleTypeDef *hpcd, PCD_BCD_MsgTypeDef msg)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hpcd);                                         /* 执行语句 */
  UNUSED(msg);                                          /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_PCDEx_BCD_Callback could be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @}
  */

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
