/**
 * 03_蜂鸣器提示音 - 官方 HAL 实验2 + MiniFreeRTOS
 */
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/BEEP/beep.h"
#include "rtos.h"

static uint32_t sb[256], si[128];
static tcb_t tb, ti;

static void task_idle(void) { while (1) {} }
static void task_beep(void)
{
    while (1)
    {
        BEEP(1); LED0(0);
        task_delay(100);
        BEEP(0); LED0(1);
        task_delay(900);
    }
}

int main(void)
{
    HAL_Init();
    sys_stm32_clock_init(336, 8, 2, 7);
    delay_init(168);
    led_init();
    beep_init();

    task_create(&tb, task_beep, sb, 256, 2);
    task_create(&ti, task_idle, si, 128, 0);
    current_tcb = &tb;
    NVIC_SetPriority(PendSV_IRQn, 0xFF);
    systick_init();
    start_first_task();
    while (1) {}
}
