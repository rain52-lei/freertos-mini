#ifndef __TPAD_H
#define __TPAD_H

#include "stm32f4xx.h"

/* 触摸判定门限：当前采样值超过“空载基准 + 门限”时认为触摸有效。 */
#define TPAD_GATE_VAL 100U
/* TIM2 是 32 位定时器，官方例程也使用最大 ARR 来扩大测量范围。 */
#define TPAD_ARR_MAX_VAL 0xFFFFFFFFUL
/* 调试用超时值，避免没有捕获边沿时长时间卡在等待循环里。 */
#define TPAD_TIMEOUT_VAL 0x0000FFFFUL

/* 上电初始化时测得的空载基准值。 */
extern volatile uint16_t g_tpad_default_val;

/* 初始化 TPAD，成功返回 0，失败返回 1。 */
uint8_t tpad_init(void);
/* 扫描触摸状态：mode=0 单次触发，mode!=0 支持连续触发。 */
uint8_t tpad_scan(uint8_t mode);
/* 简化接口，等价于 tpad_scan(0)。 */
uint8_t tpad_is_touched(void);
/* 串口调试时可调用，读取一次原始捕获值。 */
uint16_t tpad_get_debug_val(void);

#endif
