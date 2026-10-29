/**
 * 11_红外遥控 - 官方 HAL 实验29 + MiniFreeRTOS
 */
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/REMOTE/remote.h"
#include "rtos.h"
#include <stdio.h>

static uint32_t sr[256], si[128];
static tcb_t tr, ti;

static void task_idle(void) { while (1) {} }

static void task_ir(void)
{
    uint8_t key;
    remote_init();
    printf("IR ready\r\n");
    while (1)
    {
        key = remote_scan();
        if (key)
        {
            printf("IR key=%u\r\n", (unsigned)key);
            LED0_TOGGLE();
        }
        task_delay(10);
    }
}

int main(void)
{
    HAL_Init();
    sys_stm32_clock_init(336, 8, 2, 7);
    delay_init(168);
    usart_init(115200);
    led_init();
    printf("\r\n[11] IR + MiniFreeRTOS\r\n");

    task_create(&tr, task_ir, sr, 256, 3);
    task_create(&ti, task_idle, si, 128, 0);
    current_tcb = &tr;
    NVIC_SetPriority(PendSV_IRQn, 0xFF);
    systick_init();
    start_first_task();
    while (1) {}
}
