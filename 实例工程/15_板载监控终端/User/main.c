/**
 * 15_板载监控终端 - 官方 HAL TFT + KEY + MiniFreeRTOS 综合
 */
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
#include "./BSP/BEEP/beep.h"
#include "./BSP/LCD/lcd.h"
#include "rtos.h"
#include <stdio.h>

static uint32_t sk[256], sb[256], sg[512], si[128];
static tcb_t tk, tb, tg, ti;
static Queue_t q;
static uint8_t qbuf[8];
static Mutex_t mu;
static volatile uint32_t g_period = 500;

static void task_idle(void) { while (1) {} }

static void task_key(void)
{
    while (1)
    {
        uint8_t k = key_scan(0);
        uint8_t m;
        if (k == KEY0_PRES)
        {
            g_period = (g_period == 200) ? 800 : 200;
        }
        if (k == KEY1_PRES) { m = 1; xQueueSend(&q, &m, 50); }
        if (k == KEY2_PRES) { m = 2; xQueueSend(&q, &m, 50); }
        task_delay(20);
    }
}

static void task_beep(void)
{
    uint8_t m;
    while (1)
    {
        if (xQueueReceive(&q, &m, portMAX_DELAY) == 1)
        {
            if (m == 1) { BEEP(1); task_delay(80); BEEP(0); }
            else { BEEP(1); task_delay(60); BEEP(0); task_delay(60); BEEP(1); task_delay(60); BEEP(0); }
        }
    }
}

static void task_gui(void)
{
    char buf[32];
    while (1)
    {
        LED0_TOGGLE();
        sprintf(buf, "PERIOD:%lu ms  ", (unsigned long)g_period);
        lcd_show_string(10, 100, 240, 24, 24, buf, BLUE);
        task_delay(g_period);
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
    beep_init();
    lcd_init();
    xQueueCreate(&q, qbuf, 1, 8);
    xMutexCreate(&mu);

    lcd_clear(WHITE);
    lcd_show_string(10, 10, 240, 32, 32, "MONITOR", RED);
    lcd_show_string(10, 50, 240, 16, 16, "K0 period K1 beep K2 dbl", BLACK);
    printf("\r\n[15] Monitor terminal\r\n");

    task_create(&tk, task_key, sk, 256, 4);
    task_create(&tb, task_beep, sb, 256, 5);
    task_create(&tg, task_gui, sg, 512, 2);
    task_create(&ti, task_idle, si, 128, 0);
    current_tcb = &tk;
    NVIC_SetPriority(PendSV_IRQn, 0xFF);
    systick_init();
    start_first_task();
    while (1) {}
}
