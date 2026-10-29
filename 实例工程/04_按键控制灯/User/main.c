/* 文件: Project/User/main.c  本文件每一行代码均附中文注释 */
#include "rtos.h"                                 /* 包含 MiniFreeRTOS 内核头文件 */
#include "led.h"                                  /* 包含 LED 驱动头文件 */
#include "key.h"                                  /* 包含按键驱动头文件 */
#include "task.h"                                 /* 包含本工程任务函数声明 */
uint32_t stack_key[256], stack_led[256], stack_idle[128]; /* 任务栈数组(单位:字=4字节) */
tcb_t tcb_key, tcb_led, tcb_idle;                 /* 任务控制块 TCB */
volatile uint8_t g_led0_run = 1;                  /* 易失变量, 多任务/中断共享 */
volatile uint8_t g_led1_run = 0;                  /* 易失变量, 多任务/中断共享 */
int main(void)                                    /* 程序入口函数 main */
{                                                 /* 代码块开始 */
    led_init();                                   /* 初始化 LED 引脚 */
    key_init();                                   /* 初始化按键引脚 */
    task_create(&tcb_key, task_key, stack_key, 256, 3); /* 创建任务: TCB+栈+函数+优先级 */
    task_create(&tcb_led, task_led, stack_led, 256, 2); /* 创建任务: TCB+栈+函数+优先级 */
    task_create(&tcb_idle, task_idle, stack_idle, 128, 0); /* 创建任务: TCB+栈+函数+优先级 */
    current_tcb = &tcb_key;                       /* 设置当前/首个运行任务指针 */
    NVIC_SetPriority(PendSV_IRQn, 0xFF);          /* 配置中断优先级 */
    systick_init();                               /* 配置系统节拍定时器 */
    start_first_task();                           /* 启动第一个任务, 进入多任务 */
    while (1) {}                                  /* 任务/空闲死循环 */
}                                                 /* 代码块结束 */
