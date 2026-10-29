#include "task.h"
#include "led.h"
#include "rtos.h"

void task_idle(void)
{
    while (1)
    {
        /* 空闲任务：可在此进入低功耗 */
    }
}

void task1(void)
{
    while (1)
    {
        led0_toggle();          // 任务1闪灯0
        task_delay(300);        // 300ms
    }
}

void task2(void)
{
    while (1)
    {
        led1_toggle();          // 任务2闪灯1
        task_delay(700);        // 700ms
    }
}