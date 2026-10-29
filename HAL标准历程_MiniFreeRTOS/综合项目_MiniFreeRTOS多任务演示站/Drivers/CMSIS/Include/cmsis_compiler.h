/* 文件: cmsis_compiler.h | 编码: UTF-8 | 每行代码附中文注释 */
/**************************************************************************//**
 * @file     cmsis_compiler.h
 * @brief    CMSIS compiler generic header file
 * @version  V5.0.4
 * @date     10. January 2018
 ******************************************************************************/
/*
 * Copyright (c) 2009-2018 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __CMSIS_COMPILER_H                              /* 防止头文件重复包含 */
#define __CMSIS_COMPILER_H                              /* 定义包含保护宏 */

#include <stdint.h>                                     /* 包含 C 标准库头文件 */

/*
 * Arm Compiler 4/5
 */
#if   defined ( __CC_ARM )                              /* 条件编译 */
  #include "cmsis_armcc.h"                              /* 包含头文件 */


/*
 * Arm Compiler 6 (armclang)
 */
#elif defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)  /* 条件编译否则如果 */
  #include "cmsis_armclang.h"                           /* 包含头文件 */


/*
 * GNU Compiler
 */
#elif defined ( __GNUC__ )                              /* 条件编译否则如果 */
  #include "cmsis_gcc.h"                                /* 包含头文件 */


/*
 * IAR Compiler
 */
#elif defined ( __ICCARM__ )                            /* 条件编译否则如果 */
  #include <cmsis_iccarm.h>                             /* 包含 C 标准库头文件 */


/*
 * TI Arm Compiler
 */
#elif defined ( __TI_ARM__ )                            /* 条件编译否则如果 */
  #include <cmsis_ccs.h>                                /* 包含 C 标准库头文件 */

  #ifndef   __ASM                                       /* 防止头文件重复包含 */
    #define __ASM                                  __asm  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __INLINE                                    /* 防止头文件重复包含 */
    #define __INLINE                               inline  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __STATIC_INLINE                             /* 防止头文件重复包含 */
    #define __STATIC_INLINE                        static inline  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __STATIC_FORCEINLINE                        /* 防止头文件重复包含 */
    #define __STATIC_FORCEINLINE                   __STATIC_INLINE  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __NO_RETURN                                 /* 防止头文件重复包含 */
    #define __NO_RETURN                            __attribute__((noreturn))  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __USED                                      /* 防止头文件重复包含 */
    #define __USED                                 __attribute__((used))  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __WEAK                                      /* 防止头文件重复包含 */
    #define __WEAK                                 __attribute__((weak))  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __PACKED                                    /* 防止头文件重复包含 */
    #define __PACKED                               __attribute__((packed))  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __PACKED_STRUCT                             /* 防止头文件重复包含 */
    #define __PACKED_STRUCT                        struct __attribute__((packed))  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __PACKED_UNION                              /* 防止头文件重复包含 */
    #define __PACKED_UNION                         union __attribute__((packed))  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __UNALIGNED_UINT32        /* deprecated */
    struct __attribute__((packed)) T_UINT32 { uint32_t v; };  /* 执行语句 */
    #define __UNALIGNED_UINT32(x)                  (((struct T_UINT32 *)(x))->v)  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __UNALIGNED_UINT16_WRITE                    /* 防止头文件重复包含 */
    __PACKED_STRUCT T_UINT16_WRITE { uint16_t v; };     /* 执行语句 */
    #define __UNALIGNED_UINT16_WRITE(addr, val)    (void)((((struct T_UINT16_WRITE *)(void*)(addr))->v) = (val))  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __UNALIGNED_UINT16_READ                     /* 防止头文件重复包含 */
    __PACKED_STRUCT T_UINT16_READ { uint16_t v; };      /* 执行语句 */
    #define __UNALIGNED_UINT16_READ(addr)          (((const struct T_UINT16_READ *)(const void *)(addr))->v)  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __UNALIGNED_UINT32_WRITE                    /* 防止头文件重复包含 */
    __PACKED_STRUCT T_UINT32_WRITE { uint32_t v; };     /* 执行语句 */
    #define __UNALIGNED_UINT32_WRITE(addr, val)    (void)((((struct T_UINT32_WRITE *)(void *)(addr))->v) = (val))  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __UNALIGNED_UINT32_READ                     /* 防止头文件重复包含 */
    __PACKED_STRUCT T_UINT32_READ { uint32_t v; };      /* 执行语句 */
    #define __UNALIGNED_UINT32_READ(addr)          (((const struct T_UINT32_READ *)(const void *)(addr))->v)  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __ALIGNED                                   /* 防止头文件重复包含 */
    #define __ALIGNED(x)                           __attribute__((aligned(x)))  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __RESTRICT                                  /* 防止头文件重复包含 */
    #warning No compiler specific solution for __RESTRICT. __RESTRICT is ignored.  /* 编译警告指示 */
    #define __RESTRICT                                  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */


/*
 * TASKING Compiler
 */
#elif defined ( __TASKING__ )                           /* 条件编译否则如果 */
  /*
   * The CMSIS functions have been implemented as intrinsics in the compiler.
   * Please use "carm -?i" to get an up to date list of all intrinsics,
   * Including the CMSIS ones.
   */

  #ifndef   __ASM                                       /* 防止头文件重复包含 */
    #define __ASM                                  __asm  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __INLINE                                    /* 防止头文件重复包含 */
    #define __INLINE                               inline  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __STATIC_INLINE                             /* 防止头文件重复包含 */
    #define __STATIC_INLINE                        static inline  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __STATIC_FORCEINLINE                        /* 防止头文件重复包含 */
    #define __STATIC_FORCEINLINE                   __STATIC_INLINE  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __NO_RETURN                                 /* 防止头文件重复包含 */
    #define __NO_RETURN                            __attribute__((noreturn))  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __USED                                      /* 防止头文件重复包含 */
    #define __USED                                 __attribute__((used))  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __WEAK                                      /* 防止头文件重复包含 */
    #define __WEAK                                 __attribute__((weak))  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __PACKED                                    /* 防止头文件重复包含 */
    #define __PACKED                               __packed__  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __PACKED_STRUCT                             /* 防止头文件重复包含 */
    #define __PACKED_STRUCT                        struct __packed__  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __PACKED_UNION                              /* 防止头文件重复包含 */
    #define __PACKED_UNION                         union __packed__  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __UNALIGNED_UINT32        /* deprecated */
    struct __packed__ T_UINT32 { uint32_t v; };         /* 执行语句 */
    #define __UNALIGNED_UINT32(x)                  (((struct T_UINT32 *)(x))->v)  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __UNALIGNED_UINT16_WRITE                    /* 防止头文件重复包含 */
    __PACKED_STRUCT T_UINT16_WRITE { uint16_t v; };     /* 执行语句 */
    #define __UNALIGNED_UINT16_WRITE(addr, val)    (void)((((struct T_UINT16_WRITE *)(void *)(addr))->v) = (val))  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __UNALIGNED_UINT16_READ                     /* 防止头文件重复包含 */
    __PACKED_STRUCT T_UINT16_READ { uint16_t v; };      /* 执行语句 */
    #define __UNALIGNED_UINT16_READ(addr)          (((const struct T_UINT16_READ *)(const void *)(addr))->v)  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __UNALIGNED_UINT32_WRITE                    /* 防止头文件重复包含 */
    __PACKED_STRUCT T_UINT32_WRITE { uint32_t v; };     /* 执行语句 */
    #define __UNALIGNED_UINT32_WRITE(addr, val)    (void)((((struct T_UINT32_WRITE *)(void *)(addr))->v) = (val))  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __UNALIGNED_UINT32_READ                     /* 防止头文件重复包含 */
    __PACKED_STRUCT T_UINT32_READ { uint32_t v; };      /* 执行语句 */
    #define __UNALIGNED_UINT32_READ(addr)          (((const struct T_UINT32_READ *)(const void *)(addr))->v)  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __ALIGNED                                   /* 防止头文件重复包含 */
    #define __ALIGNED(x)              __align(x)        /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __RESTRICT                                  /* 防止头文件重复包含 */
    #warning No compiler specific solution for __RESTRICT. __RESTRICT is ignored.  /* 编译警告指示 */
    #define __RESTRICT                                  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */


/*
 * COSMIC Compiler
 */
#elif defined ( __CSMC__ )                              /* 条件编译否则如果 */
   #include <cmsis_csm.h>                               /* 包含 C 标准库头文件 */

 #ifndef   __ASM                                        /* 防止头文件重复包含 */
    #define __ASM                                  _asm  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __INLINE                                    /* 防止头文件重复包含 */
    #define __INLINE                               inline  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __STATIC_INLINE                             /* 防止头文件重复包含 */
    #define __STATIC_INLINE                        static inline  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __STATIC_FORCEINLINE                        /* 防止头文件重复包含 */
    #define __STATIC_FORCEINLINE                   __STATIC_INLINE  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __NO_RETURN                                 /* 防止头文件重复包含 */
    // NO RETURN is automatically detected hence no warning here
    #define __NO_RETURN                                 /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __USED                                      /* 防止头文件重复包含 */
    #warning No compiler specific solution for __USED. __USED is ignored.  /* 编译警告指示 */
    #define __USED                                      /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __WEAK                                      /* 防止头文件重复包含 */
    #define __WEAK                                 __weak  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __PACKED                                    /* 防止头文件重复包含 */
    #define __PACKED                               @packed  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __PACKED_STRUCT                             /* 防止头文件重复包含 */
    #define __PACKED_STRUCT                        @packed struct  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __PACKED_UNION                              /* 防止头文件重复包含 */
    #define __PACKED_UNION                         @packed union  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __UNALIGNED_UINT32        /* deprecated */
    @packed struct T_UINT32 { uint32_t v; };            /* 执行语句 */
    #define __UNALIGNED_UINT32(x)                  (((struct T_UINT32 *)(x))->v)  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __UNALIGNED_UINT16_WRITE                    /* 防止头文件重复包含 */
    __PACKED_STRUCT T_UINT16_WRITE { uint16_t v; };     /* 执行语句 */
    #define __UNALIGNED_UINT16_WRITE(addr, val)    (void)((((struct T_UINT16_WRITE *)(void *)(addr))->v) = (val))  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __UNALIGNED_UINT16_READ                     /* 防止头文件重复包含 */
    __PACKED_STRUCT T_UINT16_READ { uint16_t v; };      /* 执行语句 */
    #define __UNALIGNED_UINT16_READ(addr)          (((const struct T_UINT16_READ *)(const void *)(addr))->v)  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __UNALIGNED_UINT32_WRITE                    /* 防止头文件重复包含 */
    __PACKED_STRUCT T_UINT32_WRITE { uint32_t v; };     /* 执行语句 */
    #define __UNALIGNED_UINT32_WRITE(addr, val)    (void)((((struct T_UINT32_WRITE *)(void *)(addr))->v) = (val))  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __UNALIGNED_UINT32_READ                     /* 防止头文件重复包含 */
    __PACKED_STRUCT T_UINT32_READ { uint32_t v; };      /* 执行语句 */
    #define __UNALIGNED_UINT32_READ(addr)          (((const struct T_UINT32_READ *)(const void *)(addr))->v)  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __ALIGNED                                   /* 防止头文件重复包含 */
    #warning No compiler specific solution for __ALIGNED. __ALIGNED is ignored.  /* 编译警告指示 */
    #define __ALIGNED(x)                                /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */
  #ifndef   __RESTRICT                                  /* 防止头文件重复包含 */
    #warning No compiler specific solution for __RESTRICT. __RESTRICT is ignored.  /* 编译警告指示 */
    #define __RESTRICT                                  /* 定义包含保护宏 */
  #endif                                                /* 条件编译结束 */


#else                                                   /* 条件编译否则 */
  #error Unknown compiler.                              /* 编译错误指示 */
#endif                                                  /* 条件编译结束 */


#endif /* __CMSIS_COMPILER_H */

