/**
 * 10_SPI FLASH - 官方 HAL 实验25 + MiniFreeRTOS
 * 读 ID 并打印, 红灯闪
 */
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/NORFLASH/norflash.h"
#include "rtos.h"
#include <stdio.h>

static uint32_t sf[512], si[128];
static tcb_t tf, ti;

static void task_idle(void) { while (1) {} }

static void task_flash(void)
{
    uint16_t id;
    norflash_init();
    id = norflash_read_id();
    printf("FLASH ID: 0x%04X\r\n", id);
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
    usart_init(115200);
    led_init();
    printf("\r\n[10] SPI FLASH + MiniFreeRTOS\r\n");

    task_create(&tf, task_flash, sf, 512, 2);
    task_create(&ti, task_idle, si, 128, 0);
    current_tcb = &tf;
    NVIC_SetPriority(PendSV_IRQn, 0xFF);
    systick_init();
    start_first_task();
    while (1) {}
}
