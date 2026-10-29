/**
 * 14_LCD状态仪表盘 - 官方 HAL TFT + MiniFreeRTOS
 * KEY0 切换 mode, 屏幕刷新状态
 */
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
#include "./BSP/LCD/lcd.h"
#include "rtos.h"
#include <stdio.h>

static uint32_t sa[256], sg[512], si[128];
static tcb_t ta, tg, ti;
static Mutex_t mu;
static volatile uint8_t g_mode = 1;
static volatile uint32_t g_tick = 0;

static void task_idle(void) { while (1) {} }

static void task_app(void)
{
    while (1)
    {
        uint8_t k = key_scan(0);
        xMutexTake(&mu, portMAX_DELAY);
        g_tick++;
        if (k == KEY0_PRES) g_mode ^= 1;
        xMutexGive(&mu);
        if (g_mode) LED0_TOGGLE(); else LED0(1);
        task_delay(100);
    }
}

static void task_gui(void)
{
    char buf[32];
    while (1)
    {
        uint8_t mode;
        uint32_t tick;
        xMutexTake(&mu, portMAX_DELAY);
        mode = g_mode;
        tick = g_tick;
        xMutexGive(&mu);

        sprintf(buf, "MODE:%u  ", mode);
        lcd_show_string(10, 100, 200, 24, 24, buf, BLUE);
        sprintf(buf, "TICK:%lu   ", (unsigned long)tick);
        lcd_show_string(10, 130, 200, 16, 16, buf, BLACK);
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
    key_init();
    lcd_init();
    xMutexCreate(&mu);

    lcd_clear(WHITE);
    lcd_show_string(10, 10, 240, 32, 32, "DASHBOARD", RED);
    lcd_show_string(10, 50, 240, 16, 16, "KEY0: toggle mode", BLACK);
    printf("\r\n[14] LCD dashboard ID=0x%04X\r\n", lcddev.id);

    task_create(&ta, task_app, sa, 256, 3);
    task_create(&tg, task_gui, sg, 512, 2);
    task_create(&ti, task_idle, si, 128, 0);
    current_tcb = &ta;
    NVIC_SetPriority(PendSV_IRQn, 0xFF);
    systick_init();
    start_first_task();
    while (1) {}
}
