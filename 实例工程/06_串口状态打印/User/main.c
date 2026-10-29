/**
 * 06_串口状态打印 - 官方 HAL 实验5 + MiniFreeRTOS
 */
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "rtos.h"
#include <stdio.h>

static uint32_t ss[256], sl[256], si[128];
static tcb_t ts, tl, ti;
static Mutex_t mu;

static void task_idle(void) { while (1) {} }
static void task_led(void) { while (1) { LED0_TOGGLE(); task_delay(500); } }
static void task_stat(void)
{
    uint32_t n = 0;
    while (1)
    {
        xMutexTake(&mu, portMAX_DELAY);
        printf("[stat] heartbeat %lu\r\n", (unsigned long)n++);
        xMutexGive(&mu);
        task_delay(1000);
    }
}

int main(void)
{
    HAL_Init();
    sys_stm32_clock_init(336, 8, 2, 7);
    delay_init(168);
    usart_init(115200);
    led_init();
    xMutexCreate(&mu);
    printf("\r\n[06] UART + MiniFreeRTOS\r\n");

    task_create(&ts, task_stat, ss, 256, 2);
    task_create(&tl, task_led, sl, 256, 2);
    task_create(&ti, task_idle, si, 128, 0);
    current_tcb = &ts;
    NVIC_SetPriority(PendSV_IRQn, 0xFF);
    systick_init();
    start_first_task();
    while (1) {}
}
