/**
 * 13_TF卡 - 官方 HAL 实验38 + MiniFreeRTOS
 * 初始化 SD 并打印信息, 心跳灯
 */
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/SDIO/sdio_sdcard.h"
#include "rtos.h"
#include <stdio.h>

static uint32_t ss[512], si[128];
static tcb_t ts, ti;

static void task_idle(void) { while (1) {} }

static void task_sd(void)
{
    while (sd_init())
    {
        printf("SD init retry...\r\n");
        LED0_TOGGLE();
        task_delay(500);
    }
    printf("SD OK, capacity logic blocks ready\r\n");
    while (1)
    {
        LED1_TOGGLE();
        task_delay(500);
    }
}

int main(void)
{
    HAL_Init();
    sys_stm32_clock_init(336, 8, 2, 7);
    delay_init(168);
    usart_init(115200);
    led_init();
    printf("\r\n[13] SD + MiniFreeRTOS\r\n");

    task_create(&ts, task_sd, ss, 512, 2);
    task_create(&ti, task_idle, si, 128, 0);
    current_tcb = &ts;
    NVIC_SetPriority(PendSV_IRQn, 0xFF);
    systick_init();
    start_first_task();
    while (1) {}
}
