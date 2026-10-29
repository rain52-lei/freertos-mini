/**
 * 01_单灯心跳 - 基于官方 HAL 实验1 + MiniFreeRTOS
 * 红灯 PF9 约 500ms 翻转
 */
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "rtos.h"

static uint32_t stack_led[256];
static uint32_t stack_idle[128];
static tcb_t tcb_led, tcb_idle;

static void task_idle(void) { while (1) {} }

static void task_led(void)
{
    while (1)
    {
        LED0_TOGGLE();
        task_delay(500);
    }
}

int main(void)
{
    HAL_Init();
    sys_stm32_clock_init(336, 8, 2, 7);
    delay_init(168);
    led_init();

    task_create(&tcb_led, task_led, stack_led, 256, 2);
    task_create(&tcb_idle, task_idle, stack_idle, 128, 0);
    current_tcb = &tcb_led;
    NVIC_SetPriority(PendSV_IRQn, 0xFF);
    systick_init();
    start_first_task();
    while (1) {}
}
