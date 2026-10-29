/**
 * 08_ADC采样 - 官方 HAL 光敏/ADC 实验 + MiniFreeRTOS
 * 依赖官方 lsens/adc 驱动接口
 */
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "rtos.h"
#include <stdio.h>

/* 光敏实验一般提供 lsens_init / lsens_get_val */
#include "./BSP/LSENS/lsens.h"

static uint32_t sa[256], si[128];
static tcb_t ta, ti;

static void task_idle(void) { while (1) {} }

static void task_adc(void)
{
    while (1)
    {
        uint8_t v = lsens_get_val();
        printf("lsens=%u\r\n", (unsigned)v);
        if (v > 80) { LED0(0); LED1(1); }
        else { LED0(1); LED1_TOGGLE(); }
        task_delay(200);
    }
}

int main(void)
{
    HAL_Init();
    sys_stm32_clock_init(336, 8, 2, 7);
    delay_init(168);
    usart_init(115200);
    led_init();
    lsens_init();
    printf("\r\n[08] ADC/LSENS + MiniFreeRTOS\r\n");

    task_create(&ta, task_adc, sa, 256, 2);
    task_create(&ti, task_idle, si, 128, 0);
    current_tcb = &ta;
    NVIC_SetPriority(PendSV_IRQn, 0xFF);
    systick_init();
    start_first_task();
    while (1) {}
}
