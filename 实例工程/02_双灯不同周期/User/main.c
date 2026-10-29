/**
 * 02_双灯不同周期 - 官方 HAL LED + MiniFreeRTOS
 */
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "rtos.h"

static uint32_t s0[256], s1[256], si[128];
static tcb_t t0, t1, ti;

static void task_idle(void) { while (1) {} }
static void task0(void) { while (1) { LED0_TOGGLE(); task_delay(500); } }
static void task1(void) { while (1) { LED1_TOGGLE(); task_delay(200); } }

int main(void)
{
    HAL_Init();
    sys_stm32_clock_init(336, 8, 2, 7);
    delay_init(168);
    led_init();

    task_create(&t0, task0, s0, 256, 2);
    task_create(&t1, task1, s1, 256, 2);
    task_create(&ti, task_idle, si, 128, 0);
    current_tcb = &t0;
    NVIC_SetPriority(PendSV_IRQn, 0xFF);
    systick_init();
    start_first_task();
    while (1) {}
}
