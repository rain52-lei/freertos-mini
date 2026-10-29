/**
 * 12_温湿度 - 官方 HAL 实验31 DHT11 + MiniFreeRTOS
 */
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/DHT11/dht11.h"
#include "rtos.h"
#include <stdio.h>

static uint32_t sd[512], si[128];
static tcb_t td, ti;

static void task_idle(void) { while (1) {} }

static void task_dht(void)
{
    uint8_t t, h;
    while (dht11_init()) { printf("DHT11 check...\r\n"); task_delay(500); }
    while (1)
    {
        if (dht11_read_data(&t, &h) == 0)
        {
            printf("T=%u H=%u\r\n", t, h);
            if (t >= 30) LED0(0); else { LED0(1); LED1_TOGGLE(); }
        }
        task_delay(2000);
    }
}

int main(void)
{
    HAL_Init();
    sys_stm32_clock_init(336, 8, 2, 7);
    delay_init(168);
    usart_init(115200);
    led_init();
    printf("\r\n[12] DHT11 + MiniFreeRTOS\r\n");

    task_create(&td, task_dht, sd, 512, 2);
    task_create(&ti, task_idle, si, 128, 0);
    current_tcb = &td;
    NVIC_SetPriority(PendSV_IRQn, 0xFF);
    systick_init();
    start_first_task();
    while (1) {}
}
