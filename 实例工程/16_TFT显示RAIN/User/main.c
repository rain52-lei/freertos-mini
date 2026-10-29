/**
 * 16_TFT显示RAIN - 官方 HAL 实验13 TFTLCD + MiniFreeRTOS
 * 4.3 寸 MCU 屏显示 RAIN
 */
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "rtos.h"
#include <stdio.h>

static uint32_t su[256], si[128];
static tcb_t tu, ti;

static void task_idle(void) { while (1) {} }
static void task_ui(void) { while (1) { LED0_TOGGLE(); task_delay(500); } }

int main(void)
{
    char idbuf[16];

    HAL_Init();
    sys_stm32_clock_init(336, 8, 2, 7);
    delay_init(168);
    usart_init(115200);
    led_init();
    lcd_init();

    lcd_clear(WHITE);
    lcd_show_string(40, 80, 320, 48, 32, "RAIN", RED);
    lcd_show_string(40, 140, 320, 32, 24, "Explorer F407", BLUE);
    sprintf(idbuf, "ID:%04X", lcddev.id);
    lcd_show_string(40, 180, 320, 24, 16, idbuf, BLACK);
    printf("\r\n[16] TFT RAIN, LCD ID=0x%04X\r\n", lcddev.id);

    task_create(&tu, task_ui, su, 256, 2);
    task_create(&ti, task_idle, si, 128, 0);
    current_tcb = &tu;
    NVIC_SetPriority(PendSV_IRQn, 0xFF);
    systick_init();
    start_first_task();
    while (1) {}
}
