/* 文件: cmsis_armcc.h | 编码: UTF-8 | 每行代码附中文注释 */
/**************************************************************************//**
 * @file     cmsis_armcc.h
 * @brief    CMSIS compiler ARMCC (Arm Compiler 5) header file
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

#ifndef __CMSIS_ARMCC_H                                 /* 防止头文件重复包含 */
#define __CMSIS_ARMCC_H                                 /* 定义包含保护宏 */


#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION < 400677)  /* 条件编译 */
  #error "Please use Arm Compiler Toolchain V4.0.677 or later!"  /* 编译错误指示 */
#endif                                                  /* 条件编译结束 */

/* CMSIS compiler control architecture macros */
#if ((defined (__TARGET_ARCH_6_M  ) && (__TARGET_ARCH_6_M   == 1)) || \
     (defined (__TARGET_ARCH_6S_M ) && (__TARGET_ARCH_6S_M  == 1))   )  /* 源代码 */
  #define __ARM_ARCH_6M__           1                   /* 定义包含保护宏 */
#endif                                                  /* 条件编译结束 */

#if (defined (__TARGET_ARCH_7_M ) && (__TARGET_ARCH_7_M  == 1))  /* 条件编译 */
  #define __ARM_ARCH_7M__           1                   /* 定义包含保护宏 */
#endif                                                  /* 条件编译结束 */

#if (defined (__TARGET_ARCH_7E_M) && (__TARGET_ARCH_7E_M == 1))  /* 条件编译 */
  #define __ARM_ARCH_7EM__          1                   /* 定义包含保护宏 */
#endif                                                  /* 条件编译结束 */

  /* __ARM_ARCH_8M_BASE__  not applicable */
  /* __ARM_ARCH_8M_MAIN__  not applicable */


/* CMSIS compiler specific defines */
#ifndef   __ASM                                         /* 防止头文件重复包含 */
  #define __ASM                                  __asm  /* 定义包含保护宏 */
#endif                                                  /* 条件编译结束 */
#ifndef   __INLINE                                      /* 防止头文件重复包含 */
  #define __INLINE                               __inline  /* 定义包含保护宏 */
#endif                                                  /* 条件编译结束 */
#ifndef   __STATIC_INLINE                               /* 防止头文件重复包含 */
  #define __STATIC_INLINE                        static __inline  /* 定义包含保护宏 */
#endif                                                  /* 条件编译结束 */
#ifndef   __STATIC_FORCEINLINE                          /* 防止头文件重复包含 */
  #define __STATIC_FORCEINLINE                   static __forceinline  /* 定义包含保护宏 */
#endif                                                  /* 条件编译结束 */
#ifndef   __NO_RETURN                                   /* 防止头文件重复包含 */
  #define __NO_RETURN                            __declspec(noreturn)  /* 定义包含保护宏 */
#endif                                                  /* 条件编译结束 */
#ifndef   __USED                                        /* 防止头文件重复包含 */
  #define __USED                                 __attribute__((used))  /* 定义包含保护宏 */
#endif                                                  /* 条件编译结束 */
#ifndef   __WEAK                                        /* 防止头文件重复包含 */
  #define __WEAK                                 __attribute__((weak))  /* 定义包含保护宏 */
#endif                                                  /* 条件编译结束 */
#ifndef   __PACKED                                      /* 防止头文件重复包含 */
  #define __PACKED                               __attribute__((packed))  /* 定义包含保护宏 */
#endif                                                  /* 条件编译结束 */
#ifndef   __PACKED_STRUCT                               /* 防止头文件重复包含 */
  #define __PACKED_STRUCT                        __packed struct  /* 定义包含保护宏 */
#endif                                                  /* 条件编译结束 */
#ifndef   __PACKED_UNION                                /* 防止头文件重复包含 */
  #define __PACKED_UNION                         __packed union  /* 定义包含保护宏 */
#endif                                                  /* 条件编译结束 */
#ifndef   __UNALIGNED_UINT32        /* deprecated */
  #define __UNALIGNED_UINT32(x)                  (*((__packed uint32_t *)(x)))  /* 定义包含保护宏 */
#endif                                                  /* 条件编译结束 */
#ifndef   __UNALIGNED_UINT16_WRITE                      /* 防止头文件重复包含 */
  #define __UNALIGNED_UINT16_WRITE(addr, val)    ((*((__packed uint16_t *)(addr))) = (val))  /* 定义包含保护宏 */
#endif                                                  /* 条件编译结束 */
#ifndef   __UNALIGNED_UINT16_READ                       /* 防止头文件重复包含 */
  #define __UNALIGNED_UINT16_READ(addr)          (*((const __packed uint16_t *)(addr)))  /* 定义包含保护宏 */
#endif                                                  /* 条件编译结束 */
#ifndef   __UNALIGNED_UINT32_WRITE                      /* 防止头文件重复包含 */
  #define __UNALIGNED_UINT32_WRITE(addr, val)    ((*((__packed uint32_t *)(addr))) = (val))  /* 定义包含保护宏 */
#endif                                                  /* 条件编译结束 */
#ifndef   __UNALIGNED_UINT32_READ                       /* 防止头文件重复包含 */
  #define __UNALIGNED_UINT32_READ(addr)          (*((const __packed uint32_t *)(addr)))  /* 定义包含保护宏 */
#endif                                                  /* 条件编译结束 */
#ifndef   __ALIGNED                                     /* 防止头文件重复包含 */
  #define __ALIGNED(x)                           __attribute__((aligned(x)))  /* 定义包含保护宏 */
#endif                                                  /* 条件编译结束 */
#ifndef   __RESTRICT                                    /* 防止头文件重复包含 */
  #define __RESTRICT                             __restrict  /* 定义包含保护宏 */
#endif                                                  /* 条件编译结束 */

/* ###########################  Core Function Access  ########################### */
/** \ingroup  CMSIS_Core_FunctionInterface
    \defgroup CMSIS_Core_RegAccFunctions CMSIS Core Register Access Functions
  @{
 */

/**
  \brief   Enable IRQ Interrupts
  \details Enables IRQ interrupts by clearing the I-bit in the CPSR.
           Can only be executed in Privileged modes.
 */
/* intrinsic void __enable_irq();     */


/**
  \brief   Disable IRQ Interrupts
  \details Disables IRQ interrupts by setting the I-bit in the CPSR.
           Can only be executed in Privileged modes.
 */
/* intrinsic void __disable_irq();    */

/**
  \brief   Get Control Register
  \details Returns the content of the Control Register.
  \return               Control Register value
 */
__STATIC_INLINE uint32_t __get_CONTROL(void)            /* 源代码 */
{                                                       /* 代码块开始 */
  register uint32_t __regControl         __ASM("control");  /* 执行语句 */
  return(__regControl);                                 /* 执行语句 */
}                                                       /* 代码块结束 */


/**
  \brief   Set Control Register
  \details Writes the given value to the Control Register.
  \param [in]    control  Control Register value to set
 */
__STATIC_INLINE void __set_CONTROL(uint32_t control)    /* 源代码 */
{                                                       /* 代码块开始 */
  register uint32_t __regControl         __ASM("control");  /* 执行语句 */
  __regControl = control;                               /* 赋值或初始化 */
}                                                       /* 代码块结束 */


/**
  \brief   Get IPSR Register
  \details Returns the content of the IPSR Register.
  \return               IPSR Register value
 */
__STATIC_INLINE uint32_t __get_IPSR(void)               /* 源代码 */
{                                                       /* 代码块开始 */
  register uint32_t __regIPSR          __ASM("ipsr");   /* 执行语句 */
  return(__regIPSR);                                    /* 执行语句 */
}                                                       /* 代码块结束 */


/**
  \brief   Get APSR Register
  \details Returns the content of the APSR Register.
  \return               APSR Register value
 */
__STATIC_INLINE uint32_t __get_APSR(void)               /* 源代码 */
{                                                       /* 代码块开始 */
  register uint32_t __regAPSR          __ASM("apsr");   /* 执行语句 */
  return(__regAPSR);                                    /* 执行语句 */
}                                                       /* 代码块结束 */


/**
  \brief   Get xPSR Register
  \details Returns the content of the xPSR Register.
  \return               xPSR Register value
 */
__STATIC_INLINE uint32_t __get_xPSR(void)               /* 源代码 */
{                                                       /* 代码块开始 */
  register uint32_t __regXPSR          __ASM("xpsr");   /* 执行语句 */
  return(__regXPSR);                                    /* 执行语句 */
}                                                       /* 代码块结束 */


/**
  \brief   Get Process Stack Pointer
  \details Returns the current value of the Process Stack Pointer (PSP).
  \return               PSP Register value
 */
__STATIC_INLINE uint32_t __get_PSP(void)                /* 源代码 */
{                                                       /* 代码块开始 */
  register uint32_t __regProcessStackPointer  __ASM("psp");  /* 执行语句 */
  return(__regProcessStackPointer);                     /* 执行语句 */
}                                                       /* 代码块结束 */


/**
  \brief   Set Process Stack Pointer
  \details Assigns the given value to the Process Stack Pointer (PSP).
  \param [in]    topOfProcStack  Process Stack Pointer value to set
 */
__STATIC_INLINE void __set_PSP(uint32_t topOfProcStack)  /* 源代码 */
{                                                       /* 代码块开始 */
  register uint32_t __regProcessStackPointer  __ASM("psp");  /* 执行语句 */
  __regProcessStackPointer = topOfProcStack;            /* 赋值或初始化 */
}                                                       /* 代码块结束 */


/**
  \brief   Get Main Stack Pointer
  \details Returns the current value of the Main Stack Pointer (MSP).
  \return               MSP Register value
 */
__STATIC_INLINE uint32_t __get_MSP(void)                /* 源代码 */
{                                                       /* 代码块开始 */
  register uint32_t __regMainStackPointer     __ASM("msp");  /* 执行语句 */
  return(__regMainStackPointer);                        /* 执行语句 */
}                                                       /* 代码块结束 */


/**
  \brief   Set Main Stack Pointer
  \details Assigns the given value to the Main Stack Pointer (MSP).
  \param [in]    topOfMainStack  Main Stack Pointer value to set
 */
__STATIC_INLINE void __set_MSP(uint32_t topOfMainStack)  /* 源代码 */
{                                                       /* 代码块开始 */
  register uint32_t __regMainStackPointer     __ASM("msp");  /* 执行语句 */
  __regMainStackPointer = topOfMainStack;               /* 赋值或初始化 */
}                                                       /* 代码块结束 */


/**
  \brief   Get Priority Mask
  \details Returns the current state of the priority mask bit from the Priority Mask Register.
  \return               Priority Mask value
 */
__STATIC_INLINE uint32_t __get_PRIMASK(void)            /* 源代码 */
{                                                       /* 代码块开始 */
  register uint32_t __regPriMask         __ASM("primask");  /* 执行语句 */
  return(__regPriMask);                                 /* 执行语句 */
}                                                       /* 代码块结束 */


/**
  \brief   Set Priority Mask
  \details Assigns the given value to the Priority Mask Register.
  \param [in]    priMask  Priority Mask
 */
__STATIC_INLINE void __set_PRIMASK(uint32_t priMask)    /* 源代码 */
{                                                       /* 代码块开始 */
  register uint32_t __regPriMask         __ASM("primask");  /* 执行语句 */
  __regPriMask = (priMask);                             /* 赋值或初始化 */
}                                                       /* 代码块结束 */


#if ((defined (__ARM_ARCH_7M__ ) && (__ARM_ARCH_7M__  == 1)) || \
     (defined (__ARM_ARCH_7EM__) && (__ARM_ARCH_7EM__ == 1))     )  /* 源代码 */

/**
  \brief   Enable FIQ
  \details Enables FIQ interrupts by clearing the F-bit in the CPSR.
           Can only be executed in Privileged modes.
 */
#define __enable_fault_irq                __enable_fiq  /* 定义包含保护宏 */


/**
  \brief   Disable FIQ
  \details Disables FIQ interrupts by setting the F-bit in the CPSR.
           Can only be executed in Privileged modes.
 */
#define __disable_fault_irq               __disable_fiq  /* 定义包含保护宏 */


/**
  \brief   Get Base Priority
  \details Returns the current value of the Base Priority register.
  \return               Base Priority register value
 */
__STATIC_INLINE uint32_t  __get_BASEPRI(void)           /* 源代码 */
{                                                       /* 代码块开始 */
  register uint32_t __regBasePri         __ASM("basepri");  /* 执行语句 */
  return(__regBasePri);                                 /* 执行语句 */
}                                                       /* 代码块结束 */


/**
  \brief   Set Base Priority
  \details Assigns the given value to the Base Priority register.
  \param [in]    basePri  Base Priority value to set
 */
__STATIC_INLINE void __set_BASEPRI(uint32_t basePri)    /* 源代码 */
{                                                       /* 代码块开始 */
  register uint32_t __regBasePri         __ASM("basepri");  /* 执行语句 */
  __regBasePri = (basePri & 0xFFU);                     /* 赋值或初始化 */
}                                                       /* 代码块结束 */


/**
  \brief   Set Base Priority with condition
  \details Assigns the given value to the Base Priority register only if BASEPRI masking is disabled,
           or the new value increases the BASEPRI priority level.
  \param [in]    basePri  Base Priority value to set
 */
__STATIC_INLINE void __set_BASEPRI_MAX(uint32_t basePri)  /* 源代码 */
{                                                       /* 代码块开始 */
  register uint32_t __regBasePriMax      __ASM("basepri_max");  /* 执行语句 */
  __regBasePriMax = (basePri & 0xFFU);                  /* 赋值或初始化 */
}                                                       /* 代码块结束 */


/**
  \brief   Get Fault Mask
  \details Returns the current value of the Fault Mask register.
  \return               Fault Mask register value
 */
__STATIC_INLINE uint32_t __get_FAULTMASK(void)          /* 源代码 */
{                                                       /* 代码块开始 */
  register uint32_t __regFaultMask       __ASM("faultmask");  /* 执行语句 */
  return(__regFaultMask);                               /* 执行语句 */
}                                                       /* 代码块结束 */


/**
  \brief   Set Fault Mask
  \details Assigns the given value to the Fault Mask register.
  \param [in]    faultMask  Fault Mask value to set
 */
__STATIC_INLINE void __set_FAULTMASK(uint32_t faultMask)  /* 源代码 */
{                                                       /* 代码块开始 */
  register uint32_t __regFaultMask       __ASM("faultmask");  /* 执行语句 */
  __regFaultMask = (faultMask & (uint32_t)1U);          /* 赋值或初始化 */
}                                                       /* 代码块结束 */

#endif /* ((defined (__ARM_ARCH_7M__ ) && (__ARM_ARCH_7M__  == 1)) || \
           (defined (__ARM_ARCH_7EM__) && (__ARM_ARCH_7EM__ == 1))     ) */


/**
  \brief   Get FPSCR
  \details Returns the current value of the Floating Point Status/Control register.
  \return               Floating Point Status/Control register value
 */
__STATIC_INLINE uint32_t __get_FPSCR(void)              /* 源代码 */
{                                                       /* 代码块开始 */
#if ((defined (__FPU_PRESENT) && (__FPU_PRESENT == 1U)) && \
     (defined (__FPU_USED   ) && (__FPU_USED    == 1U))     )  /* 源代码 */
  register uint32_t __regfpscr         __ASM("fpscr");  /* 执行语句 */
  return(__regfpscr);                                   /* 执行语句 */
#else                                                   /* 条件编译否则 */
   return(0U);                                          /* 执行语句 */
#endif                                                  /* 条件编译结束 */
}                                                       /* 代码块结束 */


/**
  \brief   Set FPSCR
  \details Assigns the given value to the Floating Point Status/Control register.
  \param [in]    fpscr  Floating Point Status/Control value to set
 */
__STATIC_INLINE void __set_FPSCR(uint32_t fpscr)        /* 源代码 */
{                                                       /* 代码块开始 */
#if ((defined (__FPU_PRESENT) && (__FPU_PRESENT == 1U)) && \
     (defined (__FPU_USED   ) && (__FPU_USED    == 1U))     )  /* 源代码 */
  register uint32_t __regfpscr         __ASM("fpscr");  /* 执行语句 */
  __regfpscr = (fpscr);                                 /* 赋值或初始化 */
#else                                                   /* 条件编译否则 */
  (void)fpscr;                                          /* 执行语句 */
#endif                                                  /* 条件编译结束 */
}                                                       /* 代码块结束 */


/*@} end of CMSIS_Core_RegAccFunctions */


/* ##########################  Core Instruction Access  ######################### */
/** \defgroup CMSIS_Core_InstructionInterface CMSIS Core Instruction Interface
  Access to dedicated instructions
  @{
*/

/**
  \brief   No Operation
  \details No Operation does nothing. This instruction can be used for code alignment purposes.
 */
#define __NOP                             __nop         /* 定义包含保护宏 */


/**
  \brief   Wait For Interrupt
  \details Wait For Interrupt is a hint instruction that suspends execution until one of a number of events occurs.
 */
#define __WFI                             __wfi         /* 定义包含保护宏 */


/**
  \brief   Wait For Event
  \details Wait For Event is a hint instruction that permits the processor to enter
           a low-power state until one of a number of events occurs.
 */
#define __WFE                             __wfe         /* 定义包含保护宏 */


/**
  \brief   Send Event
  \details Send Event is a hint instruction. It causes an event to be signaled to the CPU.
 */
#define __SEV                             __sev         /* 定义包含保护宏 */


/**
  \brief   Instruction Synchronization Barrier
  \details Instruction Synchronization Barrier flushes the pipeline in the processor,
           so that all instructions following the ISB are fetched from cache or memory,
           after the instruction has been completed.
 */
#define __ISB() do {\
                   __schedule_barrier();\
                   __isb(0xF);\
                   __schedule_barrier();\
                } while (0U)                            /* 代码块结束 */

/**
  \brief   Data Synchronization Barrier
  \details Acts as a special kind of Data Memory Barrier.
           It completes when all explicit memory accesses before this instruction complete.
 */
#define __DSB() do {\
                   __schedule_barrier();\
                   __dsb(0xF);\
                   __schedule_barrier();\
                } while (0U)                            /* 代码块结束 */

/**
  \brief   Data Memory Barrier
  \details Ensures the apparent order of the explicit memory operations before
           and after the instruction, without ensuring their completion.
 */
#define __DMB() do {\
                   __schedule_barrier();\
                   __dmb(0xF);\
                   __schedule_barrier();\
                } while (0U)                            /* 代码块结束 */


/**
  \brief   Reverse byte order (32 bit)
  \details Reverses the byte order in unsigned integer value. For example, 0x12345678 becomes 0x78563412.
  \param [in]    value  Value to reverse
  \return               Reversed value
 */
#define __REV                             __rev         /* 定义包含保护宏 */


/**
  \brief   Reverse byte order (16 bit)
  \details Reverses the byte order within each halfword of a word. For example, 0x12345678 becomes 0x34127856.
  \param [in]    value  Value to reverse
  \return               Reversed value
 */
#ifndef __NO_EMBEDDED_ASM                               /* 防止头文件重复包含 */
__attribute__((section(".rev16_text"))) __STATIC_INLINE __ASM uint32_t __REV16(uint32_t value)  /* 源代码 */
{                                                       /* 代码块开始 */
  rev16 r0, r0                                          /* 源代码 */
  bx lr                                                 /* 源代码 */
}                                                       /* 代码块结束 */
#endif                                                  /* 条件编译结束 */


/**
  \brief   Reverse byte order (16 bit)
  \details Reverses the byte order in a 16-bit value and returns the signed 16-bit result. For example, 0x0080 becomes 0x8000.
  \param [in]    value  Value to reverse
  \return               Reversed value
 */
#ifndef __NO_EMBEDDED_ASM                               /* 防止头文件重复包含 */
__attribute__((section(".revsh_text"))) __STATIC_INLINE __ASM int16_t __REVSH(int16_t value)  /* 源代码 */
{                                                       /* 代码块开始 */
  revsh r0, r0                                          /* 源代码 */
  bx lr                                                 /* 源代码 */
}                                                       /* 代码块结束 */
#endif                                                  /* 条件编译结束 */


/**
  \brief   Rotate Right in unsigned value (32 bit)
  \details Rotate Right (immediate) provides the value of the contents of a register rotated by a variable number of bits.
  \param [in]    op1  Value to rotate
  \param [in]    op2  Number of Bits to rotate
  \return               Rotated value
 */
#define __ROR                             __ror         /* 定义包含保护宏 */


/**
  \brief   Breakpoint
  \details Causes the processor to enter Debug state.
           Debug tools can use this to investigate system state when the instruction at a particular address is reached.
  \param [in]    value  is ignored by the processor.
                 If required, a debugger can use it to store additional information about the breakpoint.
 */
#define __BKPT(value)                       __breakpoint(value)  /* 定义包含保护宏 */


/**
  \brief   Reverse bit order of value
  \details Reverses the bit order of the given value.
  \param [in]    value  Value to reverse
  \return               Reversed value
 */
#if ((defined (__ARM_ARCH_7M__ ) && (__ARM_ARCH_7M__  == 1)) || \
     (defined (__ARM_ARCH_7EM__) && (__ARM_ARCH_7EM__ == 1))     )  /* 源代码 */
  #define __RBIT                          __rbit        /* 定义包含保护宏 */
#else                                                   /* 条件编译否则 */
__attribute__((always_inline)) __STATIC_INLINE uint32_t __RBIT(uint32_t value)  /* 源代码 */
{                                                       /* 代码块开始 */
  uint32_t result;                                      /* 32位无符号变量/函数 */
  uint32_t s = (4U /*sizeof(v)*/ * 8U) - 1U; /* extra shift needed at end */

  result = value;                      /* r will be reversed bits of v; first get LSB of v */
  for (value >>= 1U; value != 0U; value >>= 1U)         /* for循环 */
  {                                                     /* 代码块开始 */
    result <<= 1U;                                      /* 赋值或初始化 */
    result |= value & 1U;                               /* 赋值或初始化 */
    s--;                                                /* 执行语句 */
  }                                                     /* 代码块结束 */
  result <<= s;                        /* shift when v's highest bits are zero */
  return result;                                        /* 返回值 */
}                                                       /* 代码块结束 */
#endif                                                  /* 条件编译结束 */


/**
  \brief   Count leading zeros
  \details Counts the number of leading zeros of a data value.
  \param [in]  value  Value to count the leading zeros
  \return             number of leading zeros in value
 */
#define __CLZ                             __clz         /* 定义包含保护宏 */


#if ((defined (__ARM_ARCH_7M__ ) && (__ARM_ARCH_7M__  == 1)) || \
     (defined (__ARM_ARCH_7EM__) && (__ARM_ARCH_7EM__ == 1))     )  /* 源代码 */

/**
  \brief   LDR Exclusive (8 bit)
  \details Executes a exclusive LDR instruction for 8 bit value.
  \param [in]    ptr  Pointer to data
  \return             value of type uint8_t at (*ptr)
 */
#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION < 5060020)  /* 条件编译 */
  #define __LDREXB(ptr)                                                        ((uint8_t ) __ldrex(ptr))  /* 定义包含保护宏 */
#else                                                   /* 条件编译否则 */
  #define __LDREXB(ptr)          _Pragma("push") _Pragma("diag_suppress 3731") ((uint8_t ) __ldrex(ptr))  _Pragma("pop")  /* 定义包含保护宏 */
#endif                                                  /* 条件编译结束 */


/**
  \brief   LDR Exclusive (16 bit)
  \details Executes a exclusive LDR instruction for 16 bit values.
  \param [in]    ptr  Pointer to data
  \return        value of type uint16_t at (*ptr)
 */
#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION < 5060020)  /* 条件编译 */
  #define __LDREXH(ptr)                                                        ((uint16_t) __ldrex(ptr))  /* 定义包含保护宏 */
#else                                                   /* 条件编译否则 */
  #define __LDREXH(ptr)          _Pragma("push") _Pragma("diag_suppress 3731") ((uint16_t) __ldrex(ptr))  _Pragma("pop")  /* 定义包含保护宏 */
#endif                                                  /* 条件编译结束 */


/**
  \brief   LDR Exclusive (32 bit)
  \details Executes a exclusive LDR instruction for 32 bit values.
  \param [in]    ptr  Pointer to data
  \return        value of type uint32_t at (*ptr)
 */
#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION < 5060020)  /* 条件编译 */
  #define __LDREXW(ptr)                                                        ((uint32_t ) __ldrex(ptr))  /* 定义包含保护宏 */
#else                                                   /* 条件编译否则 */
  #define __LDREXW(ptr)          _Pragma("push") _Pragma("diag_suppress 3731") ((uint32_t ) __ldrex(ptr))  _Pragma("pop")  /* 定义包含保护宏 */
#endif                                                  /* 条件编译结束 */


/**
  \brief   STR Exclusive (8 bit)
  \details Executes a exclusive STR instruction for 8 bit values.
  \param [in]  value  Value to store
  \param [in]    ptr  Pointer to location
  \return          0  Function succeeded
  \return          1  Function failed
 */
#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION < 5060020)  /* 条件编译 */
  #define __STREXB(value, ptr)                                                 __strex(value, ptr)  /* 定义包含保护宏 */
#else                                                   /* 条件编译否则 */
  #define __STREXB(value, ptr)   _Pragma("push") _Pragma("diag_suppress 3731") __strex(value, ptr)        _Pragma("pop")  /* 定义包含保护宏 */
#endif                                                  /* 条件编译结束 */


/**
  \brief   STR Exclusive (16 bit)
  \details Executes a exclusive STR instruction for 16 bit values.
  \param [in]  value  Value to store
  \param [in]    ptr  Pointer to location
  \return          0  Function succeeded
  \return          1  Function failed
 */
#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION < 5060020)  /* 条件编译 */
  #define __STREXH(value, ptr)                                                 __strex(value, ptr)  /* 定义包含保护宏 */
#else                                                   /* 条件编译否则 */
  #define __STREXH(value, ptr)   _Pragma("push") _Pragma("diag_suppress 3731") __strex(value, ptr)        _Pragma("pop")  /* 定义包含保护宏 */
#endif                                                  /* 条件编译结束 */


/**
  \brief   STR Exclusive (32 bit)
  \details Executes a exclusive STR instruction for 32 bit values.
  \param [in]  value  Value to store
  \param [in]    ptr  Pointer to location
  \return          0  Function succeeded
  \return          1  Function failed
 */
#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION < 5060020)  /* 条件编译 */
  #define __STREXW(value, ptr)                                                 __strex(value, ptr)  /* 定义包含保护宏 */
#else                                                   /* 条件编译否则 */
  #define __STREXW(value, ptr)   _Pragma("push") _Pragma("diag_suppress 3731") __strex(value, ptr)        _Pragma("pop")  /* 定义包含保护宏 */
#endif                                                  /* 条件编译结束 */


/**
  \brief   Remove the exclusive lock
  \details Removes the exclusive lock which is created by LDREX.
 */
#define __CLREX                           __clrex       /* 定义包含保护宏 */


/**
  \brief   Signed Saturate
  \details Saturates a signed value.
  \param [in]  value  Value to be saturated
  \param [in]    sat  Bit position to saturate to (1..32)
  \return             Saturated value
 */
#define __SSAT                            __ssat        /* 定义包含保护宏 */


/**
  \brief   Unsigned Saturate
  \details Saturates an unsigned value.
  \param [in]  value  Value to be saturated
  \param [in]    sat  Bit position to saturate to (0..31)
  \return             Saturated value
 */
#define __USAT                            __usat        /* 定义包含保护宏 */


/**
  \brief   Rotate Right with Extend (32 bit)
  \details Moves each bit of a bitstring right by one bit.
           The carry input is shifted in at the left end of the bitstring.
  \param [in]    value  Value to rotate
  \return               Rotated value
 */
#ifndef __NO_EMBEDDED_ASM                               /* 防止头文件重复包含 */
__attribute__((section(".rrx_text"))) __STATIC_INLINE __ASM uint32_t __RRX(uint32_t value)  /* 源代码 */
{                                                       /* 代码块开始 */
  rrx r0, r0                                            /* 源代码 */
  bx lr                                                 /* 源代码 */
}                                                       /* 代码块结束 */
#endif                                                  /* 条件编译结束 */


/**
  \brief   LDRT Unprivileged (8 bit)
  \details Executes a Unprivileged LDRT instruction for 8 bit value.
  \param [in]    ptr  Pointer to data
  \return             value of type uint8_t at (*ptr)
 */
#define __LDRBT(ptr)                      ((uint8_t )  __ldrt(ptr))  /* 定义包含保护宏 */


/**
  \brief   LDRT Unprivileged (16 bit)
  \details Executes a Unprivileged LDRT instruction for 16 bit values.
  \param [in]    ptr  Pointer to data
  \return        value of type uint16_t at (*ptr)
 */
#define __LDRHT(ptr)                      ((uint16_t)  __ldrt(ptr))  /* 定义包含保护宏 */


/**
  \brief   LDRT Unprivileged (32 bit)
  \details Executes a Unprivileged LDRT instruction for 32 bit values.
  \param [in]    ptr  Pointer to data
  \return        value of type uint32_t at (*ptr)
 */
#define __LDRT(ptr)                       ((uint32_t ) __ldrt(ptr))  /* 定义包含保护宏 */


/**
  \brief   STRT Unprivileged (8 bit)
  \details Executes a Unprivileged STRT instruction for 8 bit values.
  \param [in]  value  Value to store
  \param [in]    ptr  Pointer to location
 */
#define __STRBT(value, ptr)               __strt(value, ptr)  /* 定义包含保护宏 */


/**
  \brief   STRT Unprivileged (16 bit)
  \details Executes a Unprivileged STRT instruction for 16 bit values.
  \param [in]  value  Value to store
  \param [in]    ptr  Pointer to location
 */
#define __STRHT(value, ptr)               __strt(value, ptr)  /* 定义包含保护宏 */


/**
  \brief   STRT Unprivileged (32 bit)
  \details Executes a Unprivileged STRT instruction for 32 bit values.
  \param [in]  value  Value to store
  \param [in]    ptr  Pointer to location
 */
#define __STRT(value, ptr)                __strt(value, ptr)  /* 定义包含保护宏 */

#else  /* ((defined (__ARM_ARCH_7M__ ) && (__ARM_ARCH_7M__  == 1)) || \
           (defined (__ARM_ARCH_7EM__) && (__ARM_ARCH_7EM__ == 1))     ) */

/**
  \brief   Signed Saturate
  \details Saturates a signed value.
  \param [in]  value  Value to be saturated
  \param [in]    sat  Bit position to saturate to (1..32)
  \return             Saturated value
 */
__attribute__((always_inline)) __STATIC_INLINE int32_t __SSAT(int32_t val, uint32_t sat)  /* 源代码 */
{                                                       /* 代码块开始 */
  if ((sat >= 1U) && (sat <= 32U))                      /* 条件判断 */
  {                                                     /* 代码块开始 */
    const int32_t max = (int32_t)((1U << (sat - 1U)) - 1U);  /* 常量定义 */
    const int32_t min = -1 - max ;                      /* 常量定义 */
    if (val > max)                                      /* 条件判断 */
    {                                                   /* 代码块开始 */
      return max;                                       /* 返回值 */
    }                                                   /* 代码块结束 */
    else if (val < min)                                 /* 否则如果 */
    {                                                   /* 代码块开始 */
      return min;                                       /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  return val;                                           /* 返回值 */
}                                                       /* 代码块结束 */

/**
  \brief   Unsigned Saturate
  \details Saturates an unsigned value.
  \param [in]  value  Value to be saturated
  \param [in]    sat  Bit position to saturate to (0..31)
  \return             Saturated value
 */
__attribute__((always_inline)) __STATIC_INLINE uint32_t __USAT(int32_t val, uint32_t sat)  /* 源代码 */
{                                                       /* 代码块开始 */
  if (sat <= 31U)                                       /* 条件判断 */
  {                                                     /* 代码块开始 */
    const uint32_t max = ((1U << sat) - 1U);            /* 常量定义 */
    if (val > (int32_t)max)                             /* 条件判断 */
    {                                                   /* 代码块开始 */
      return max;                                       /* 返回值 */
    }                                                   /* 代码块结束 */
    else if (val < 0)                                   /* 否则如果 */
    {                                                   /* 代码块开始 */
      return 0U;                                        /* 返回值 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  return (uint32_t)val;                                 /* 返回值 */
}                                                       /* 代码块结束 */

#endif /* ((defined (__ARM_ARCH_7M__ ) && (__ARM_ARCH_7M__  == 1)) || \
           (defined (__ARM_ARCH_7EM__) && (__ARM_ARCH_7EM__ == 1))     ) */

/*@}*/ /* end of group CMSIS_Core_InstructionInterface */


/* ###################  Compiler specific Intrinsics  ########################### */
/** \defgroup CMSIS_SIMD_intrinsics CMSIS SIMD Intrinsics
  Access to dedicated SIMD instructions
  @{
*/

#if ((defined (__ARM_ARCH_7EM__) && (__ARM_ARCH_7EM__ == 1))     )  /* 条件编译 */

#define __SADD8                           __sadd8       /* 定义包含保护宏 */
#define __QADD8                           __qadd8       /* 定义包含保护宏 */
#define __SHADD8                          __shadd8      /* 定义包含保护宏 */
#define __UADD8                           __uadd8       /* 定义包含保护宏 */
#define __UQADD8                          __uqadd8      /* 定义包含保护宏 */
#define __UHADD8                          __uhadd8      /* 定义包含保护宏 */
#define __SSUB8                           __ssub8       /* 定义包含保护宏 */
#define __QSUB8                           __qsub8       /* 定义包含保护宏 */
#define __SHSUB8                          __shsub8      /* 定义包含保护宏 */
#define __USUB8                           __usub8       /* 定义包含保护宏 */
#define __UQSUB8                          __uqsub8      /* 定义包含保护宏 */
#define __UHSUB8                          __uhsub8      /* 定义包含保护宏 */
#define __SADD16                          __sadd16      /* 定义包含保护宏 */
#define __QADD16                          __qadd16      /* 定义包含保护宏 */
#define __SHADD16                         __shadd16     /* 定义包含保护宏 */
#define __UADD16                          __uadd16      /* 定义包含保护宏 */
#define __UQADD16                         __uqadd16     /* 定义包含保护宏 */
#define __UHADD16                         __uhadd16     /* 定义包含保护宏 */
#define __SSUB16                          __ssub16      /* 定义包含保护宏 */
#define __QSUB16                          __qsub16      /* 定义包含保护宏 */
#define __SHSUB16                         __shsub16     /* 定义包含保护宏 */
#define __USUB16                          __usub16      /* 定义包含保护宏 */
#define __UQSUB16                         __uqsub16     /* 定义包含保护宏 */
#define __UHSUB16                         __uhsub16     /* 定义包含保护宏 */
#define __SASX                            __sasx        /* 定义包含保护宏 */
#define __QASX                            __qasx        /* 定义包含保护宏 */
#define __SHASX                           __shasx       /* 定义包含保护宏 */
#define __UASX                            __uasx        /* 定义包含保护宏 */
#define __UQASX                           __uqasx       /* 定义包含保护宏 */
#define __UHASX                           __uhasx       /* 定义包含保护宏 */
#define __SSAX                            __ssax        /* 定义包含保护宏 */
#define __QSAX                            __qsax        /* 定义包含保护宏 */
#define __SHSAX                           __shsax       /* 定义包含保护宏 */
#define __USAX                            __usax        /* 定义包含保护宏 */
#define __UQSAX                           __uqsax       /* 定义包含保护宏 */
#define __UHSAX                           __uhsax       /* 定义包含保护宏 */
#define __USAD8                           __usad8       /* 定义包含保护宏 */
#define __USADA8                          __usada8      /* 定义包含保护宏 */
#define __SSAT16                          __ssat16      /* 定义包含保护宏 */
#define __USAT16                          __usat16      /* 定义包含保护宏 */
#define __UXTB16                          __uxtb16      /* 定义包含保护宏 */
#define __UXTAB16                         __uxtab16     /* 定义包含保护宏 */
#define __SXTB16                          __sxtb16      /* 定义包含保护宏 */
#define __SXTAB16                         __sxtab16     /* 定义包含保护宏 */
#define __SMUAD                           __smuad       /* 定义包含保护宏 */
#define __SMUADX                          __smuadx      /* 定义包含保护宏 */
#define __SMLAD                           __smlad       /* 定义包含保护宏 */
#define __SMLADX                          __smladx      /* 定义包含保护宏 */
#define __SMLALD                          __smlald      /* 定义包含保护宏 */
#define __SMLALDX                         __smlaldx     /* 定义包含保护宏 */
#define __SMUSD                           __smusd       /* 定义包含保护宏 */
#define __SMUSDX                          __smusdx      /* 定义包含保护宏 */
#define __SMLSD                           __smlsd       /* 定义包含保护宏 */
#define __SMLSDX                          __smlsdx      /* 定义包含保护宏 */
#define __SMLSLD                          __smlsld      /* 定义包含保护宏 */
#define __SMLSLDX                         __smlsldx     /* 定义包含保护宏 */
#define __SEL                             __sel         /* 定义包含保护宏 */
#define __QADD                            __qadd        /* 定义包含保护宏 */
#define __QSUB                            __qsub        /* 定义包含保护宏 */

#define __PKHBT(ARG1,ARG2,ARG3)          ( ((((uint32_t)(ARG1))          ) & 0x0000FFFFUL) |  \
                                           ((((uint32_t)(ARG2)) << (ARG3)) & 0xFFFF0000UL)  )  /* 源代码 */

#define __PKHTB(ARG1,ARG2,ARG3)          ( ((((uint32_t)(ARG1))          ) & 0xFFFF0000UL) |  \
                                           ((((uint32_t)(ARG2)) >> (ARG3)) & 0x0000FFFFUL)  )  /* 源代码 */

#define __SMMLA(ARG1,ARG2,ARG3)          ( (int32_t)((((int64_t)(ARG1) * (ARG2)) + \
                                                      ((int64_t)(ARG3) << 32U)     ) >> 32U))  /* 源代码 */

#endif /* ((defined (__ARM_ARCH_7EM__) && (__ARM_ARCH_7EM__ == 1))     ) */
/*@} end of group CMSIS_SIMD_intrinsics */


#endif /* __CMSIS_ARMCC_H */
