/* 文件: stm32f4xx.h | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx.h
  * @author  MCD Application Team
  * @brief   CMSIS STM32F4xx Device Peripheral Access Layer Header File.
  *            
  *          The file is the unique include file that the application programmer
  *          is using in the C source code, usually in main.c. This file contains:
  *           - Configuration section that allows to select:
  *              - The STM32F4xx device used in the target application
  *              - To use or not the peripheral抯 drivers in application code(i.e. 
  *                code will be based on direct access to peripheral抯 registers 
  *                rather than drivers API), this option is controlled by 
  *                "#define USE_HAL_DRIVER"
  *  
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

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup stm32f4xx
  * @{
  */

#ifndef __STM32F4xx_H                                   /* 防止头文件重复包含 */
#define __STM32F4xx_H                                   /* 定义包含保护宏 */

#ifdef __cplusplus                                      /* 条件编译 */
 extern "C" {                                           /* 外部符号声明 */
#endif /* __cplusplus */

/** @addtogroup Library_configuration_section
  * @{
  */

/**
  * @brief STM32 Family
  */
#if !defined  (STM32F4)                                 /* 条件编译 */
#define STM32F4                                         /* 定义宏 */
#endif /* STM32F4 */

/* Uncomment the line below according to the target STM32 device used in your
   application 
  */
#if !defined (STM32F405xx) && !defined (STM32F415xx) && !defined (STM32F407xx) && !defined (STM32F417xx) && \
    !defined (STM32F427xx) && !defined (STM32F437xx) && !defined (STM32F429xx) && !defined (STM32F439xx) && \
    !defined (STM32F401xC) && !defined (STM32F401xE) && !defined (STM32F410Tx) && !defined (STM32F410Cx) && \
    !defined (STM32F410Rx) && !defined (STM32F411xE) && !defined (STM32F446xx) && !defined (STM32F469xx) && \
    !defined (STM32F479xx) && !defined (STM32F412Cx) && !defined (STM32F412Rx) && !defined (STM32F412Vx) && \
    !defined (STM32F412Zx) && !defined (STM32F413xx) && !defined (STM32F423xx)  /* 源代码 */
  /* #define STM32F405xx */   /*!< STM32F405RG, STM32F405VG and STM32F405ZG Devices */
  /* #define STM32F415xx */   /*!< STM32F415RG, STM32F415VG and STM32F415ZG Devices */
  /* #define STM32F407xx */   /*!< STM32F407VG, STM32F407VE, STM32F407ZG, STM32F407ZE, STM32F407IG  and STM32F407IE Devices */
  /* #define STM32F417xx */   /*!< STM32F417VG, STM32F417VE, STM32F417ZG, STM32F417ZE, STM32F417IG and STM32F417IE Devices */
  /* #define STM32F427xx */   /*!< STM32F427VG, STM32F427VI, STM32F427ZG, STM32F427ZI, STM32F427IG and STM32F427II Devices */
  /* #define STM32F437xx */   /*!< STM32F437VG, STM32F437VI, STM32F437ZG, STM32F437ZI, STM32F437IG and STM32F437II Devices */
  /* #define STM32F429xx */   /*!< STM32F429VG, STM32F429VI, STM32F429ZG, STM32F429ZI, STM32F429BG, STM32F429BI, STM32F429NG, 
                                   STM32F439NI, STM32F429IG  and STM32F429II Devices */
  /* #define STM32F439xx */   /*!< STM32F439VG, STM32F439VI, STM32F439ZG, STM32F439ZI, STM32F439BG, STM32F439BI, STM32F439NG, 
                                   STM32F439NI, STM32F439IG and STM32F439II Devices */
  /* #define STM32F401xC */   /*!< STM32F401CB, STM32F401CC, STM32F401RB, STM32F401RC, STM32F401VB and STM32F401VC Devices */
  /* #define STM32F401xE */   /*!< STM32F401CD, STM32F401RD, STM32F401VD, STM32F401CE, STM32F401RE and STM32F401VE Devices */
  /* #define STM32F410Tx */   /*!< STM32F410T8 and STM32F410TB Devices */
  /* #define STM32F410Cx */   /*!< STM32F410C8 and STM32F410CB Devices */
  /* #define STM32F410Rx */   /*!< STM32F410R8 and STM32F410RB Devices */
  /* #define STM32F411xE */   /*!< STM32F411CC, STM32F411RC, STM32F411VC, STM32F411CE, STM32F411RE and STM32F411VE Devices */
  /* #define STM32F446xx */   /*!< STM32F446MC, STM32F446ME, STM32F446RC, STM32F446RE, STM32F446VC, STM32F446VE, STM32F446ZC, 
                                   and STM32F446ZE Devices */
  /* #define STM32F469xx */   /*!< STM32F469AI, STM32F469II, STM32F469BI, STM32F469NI, STM32F469AG, STM32F469IG, STM32F469BG, 
                                   STM32F469NG, STM32F469AE, STM32F469IE, STM32F469BE and STM32F469NE Devices */
  /* #define STM32F479xx */   /*!< STM32F479AI, STM32F479II, STM32F479BI, STM32F479NI, STM32F479AG, STM32F479IG, STM32F479BG 
                                   and STM32F479NG Devices */
  /* #define STM32F412Cx */   /*!< STM32F412CEU and STM32F412CGU Devices */
  /* #define STM32F412Zx */   /*!< STM32F412ZET, STM32F412ZGT, STM32F412ZEJ and STM32F412ZGJ Devices */
  /* #define STM32F412Vx */   /*!< STM32F412VET, STM32F412VGT, STM32F412VEH and STM32F412VGH Devices */
  /* #define STM32F412Rx */   /*!< STM32F412RET, STM32F412RGT, STM32F412REY and STM32F412RGY Devices */
  /* #define STM32F413xx */   /*!< STM32F413CH, STM32F413MH, STM32F413RH, STM32F413VH, STM32F413ZH, STM32F413CG, STM32F413MG,
                                   STM32F413RG, STM32F413VG and STM32F413ZG Devices */
  /* #define STM32F423xx */   /*!< STM32F423CH, STM32F423RH, STM32F423VH and STM32F423ZH Devices */
#endif                                                  /* 条件编译结束 */

/*  Tip: To avoid modifying this file each time you need to switch between these
        devices, you can define the device in your toolchain compiler preprocessor.
  */
#if !defined  (USE_HAL_DRIVER)                          /* 条件编译 */
/**
 * @brief Comment the line below if you will not use the peripherals drivers.
   In this case, these drivers will not be included and the application code will 
   be based on direct access to peripherals registers 
   */
  /*#define USE_HAL_DRIVER */
#endif /* USE_HAL_DRIVER */

/**
  * @brief CMSIS version number V2.6.6
  */
#define __STM32F4xx_CMSIS_VERSION_MAIN   (0x02U) /*!< [31:24] main version */
#define __STM32F4xx_CMSIS_VERSION_SUB1   (0x06U) /*!< [23:16] sub1 version */
#define __STM32F4xx_CMSIS_VERSION_SUB2   (0x06U) /*!< [15:8]  sub2 version */
#define __STM32F4xx_CMSIS_VERSION_RC     (0x00U) /*!< [7:0]  release candidate */
#define __STM32F4xx_CMSIS_VERSION        ((__STM32F4xx_CMSIS_VERSION_MAIN << 24)\
                                         |(__STM32F4xx_CMSIS_VERSION_SUB1 << 16)\
                                         |(__STM32F4xx_CMSIS_VERSION_SUB2 << 8 )\
                                         |(__STM32F4xx_CMSIS_VERSION))  /* 源代码 */

/**
  * @}
  */

/** @addtogroup Device_Included
  * @{
  */

#if defined(STM32F405xx)                                /* 条件编译 */
  #include "stm32f405xx.h"                              /* 包含 STM32/HAL 头文件 */
#elif defined(STM32F415xx)                              /* 条件编译否则如果 */
  #include "stm32f415xx.h"                              /* 包含 STM32/HAL 头文件 */
#elif defined(STM32F407xx)                              /* 条件编译否则如果 */
  #include "stm32f407xx.h"                              /* 包含 STM32/HAL 头文件 */
#elif defined(STM32F417xx)                              /* 条件编译否则如果 */
  #include "stm32f417xx.h"                              /* 包含 STM32/HAL 头文件 */
#elif defined(STM32F427xx)                              /* 条件编译否则如果 */
  #include "stm32f427xx.h"                              /* 包含 STM32/HAL 头文件 */
#elif defined(STM32F437xx)                              /* 条件编译否则如果 */
  #include "stm32f437xx.h"                              /* 包含 STM32/HAL 头文件 */
#elif defined(STM32F429xx)                              /* 条件编译否则如果 */
  #include "stm32f429xx.h"                              /* 包含 STM32/HAL 头文件 */
#elif defined(STM32F439xx)                              /* 条件编译否则如果 */
  #include "stm32f439xx.h"                              /* 包含 STM32/HAL 头文件 */
#elif defined(STM32F401xC)                              /* 条件编译否则如果 */
  #include "stm32f401xc.h"                              /* 包含 STM32/HAL 头文件 */
#elif defined(STM32F401xE)                              /* 条件编译否则如果 */
  #include "stm32f401xe.h"                              /* 包含 STM32/HAL 头文件 */
#elif defined(STM32F410Tx)                              /* 条件编译否则如果 */
  #include "stm32f410tx.h"                              /* 包含 STM32/HAL 头文件 */
#elif defined(STM32F410Cx)                              /* 条件编译否则如果 */
  #include "stm32f410cx.h"                              /* 包含 STM32/HAL 头文件 */
#elif defined(STM32F410Rx)                              /* 条件编译否则如果 */
  #include "stm32f410rx.h"                              /* 包含 STM32/HAL 头文件 */
#elif defined(STM32F411xE)                              /* 条件编译否则如果 */
  #include "stm32f411xe.h"                              /* 包含 STM32/HAL 头文件 */
#elif defined(STM32F446xx)                              /* 条件编译否则如果 */
  #include "stm32f446xx.h"                              /* 包含 STM32/HAL 头文件 */
#elif defined(STM32F469xx)                              /* 条件编译否则如果 */
  #include "stm32f469xx.h"                              /* 包含 STM32/HAL 头文件 */
#elif defined(STM32F479xx)                              /* 条件编译否则如果 */
  #include "stm32f479xx.h"                              /* 包含 STM32/HAL 头文件 */
#elif defined(STM32F412Cx)                              /* 条件编译否则如果 */
  #include "stm32f412cx.h"                              /* 包含 STM32/HAL 头文件 */
#elif defined(STM32F412Zx)                              /* 条件编译否则如果 */
  #include "stm32f412zx.h"                              /* 包含 STM32/HAL 头文件 */
#elif defined(STM32F412Rx)                              /* 条件编译否则如果 */
  #include "stm32f412rx.h"                              /* 包含 STM32/HAL 头文件 */
#elif defined(STM32F412Vx)                              /* 条件编译否则如果 */
  #include "stm32f412vx.h"                              /* 包含 STM32/HAL 头文件 */
#elif defined(STM32F413xx)                              /* 条件编译否则如果 */
  #include "stm32f413xx.h"                              /* 包含 STM32/HAL 头文件 */
#elif defined(STM32F423xx)                              /* 条件编译否则如果 */
  #include "stm32f423xx.h"                              /* 包含 STM32/HAL 头文件 */
#else                                                   /* 条件编译否则 */
 #error "Please select first the target STM32F4xx device used in your application (in stm32f4xx.h file)"  /* 编译错误指示 */
#endif                                                  /* 条件编译结束 */

/**
  * @}
  */

/** @addtogroup Exported_types
  * @{
  */ 
typedef enum                                            /* 类型定义 */
{                                                       /* 代码块开始 */
  RESET = 0U,                                           /* 列表续行 */
  SET = !RESET                                          /* 源代码 */
} FlagStatus, ITStatus;                                 /* 代码块结束 */

typedef enum                                            /* 类型定义 */
{                                                       /* 代码块开始 */
  DISABLE = 0U,                                         /* 列表续行 */
  ENABLE = !DISABLE                                     /* 源代码 */
} FunctionalState;                                      /* 代码块结束 */
#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))  /* 定义宏 */

typedef enum                                            /* 类型定义 */
{                                                       /* 代码块开始 */
  SUCCESS = 0U,                                         /* 列表续行 */
  ERROR = !SUCCESS                                      /* 源代码 */
} ErrorStatus;                                          /* 代码块结束 */

/**
  * @}
  */


/** @addtogroup Exported_macro
  * @{
  */
#define SET_BIT(REG, BIT)     ((REG) |= (BIT))          /* 定义宏 */

#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))         /* 定义宏 */

#define READ_BIT(REG, BIT)    ((REG) & (BIT))           /* 定义宏 */

#define CLEAR_REG(REG)        ((REG) = (0x0))           /* 定义宏 */

#define WRITE_REG(REG, VAL)   ((REG) = (VAL))           /* 定义宏 */

#define READ_REG(REG)         ((REG))                   /* 定义宏 */

#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))  /* 定义宏 */

#define POSITION_VAL(VAL)     (__CLZ(__RBIT(VAL)))      /* 定义宏 */


/**
  * @}
  */

#if defined (USE_HAL_DRIVER)                            /* 条件编译 */
 #include "stm32f4xx_hal.h"                             /* 包含 STM32/HAL 头文件 */
#endif /* USE_HAL_DRIVER */

#ifdef __cplusplus                                      /* 条件编译 */
}                                                       /* 代码块结束 */
#endif /* __cplusplus */

#endif /* __STM32F4xx_H */
/**
  * @}
  */

/**
  * @}
  */




/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
