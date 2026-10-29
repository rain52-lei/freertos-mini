/**
 * 05_按键队列驱动蜂鸣器 - 官方 HAL 驱动 + MiniFreeRTOS 队列
 * KEY0 短鸣 KEY1 双鸣
 */
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
#include "./BSP/BEEP/beep.h"
#include "rtos.h"

static uint32_t sk[256], sb[256], sl[256], si[128];
static tcb_t tk, tb, tl, ti;
static Queue_t gq;
static uint8_t gqbuf[8];

static void task_idle(void) { while (1) {} }
static void task_led(void) { while (1) { LED0_TOGGLE(); task_delay(500); } }

static void task_key(void)
{
    while (1)
    {
        uint8_t k = key_scan(0);
        uint8_t m;
        if (k == KEY0_PRES) { m = 1; xQueueSend(&gq, &m, 100); }
        if (k == KEY1_PRES) { m = 2; xQueueSend(&gq, &m, 100); }
        task_delay(20);
    }
}

static void task_beep(void)
{
    uint8_t m;
    while (1)
    {
        if (xQueueReceive(&gq, &m, portMAX_DELAY) == 1)
        {
            if (m == 1) { BEEP(1); task_delay(80); BEEP(0); }
            else if (m == 2) {
                BEEP(1); task_delay(60); BEEP(0); task_delay(60);
                BEEP(1); task_delay(60); BEEP(0);
            }
        }
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
    xQueueCreate(&gq, gqbuf, sizeof(uint8_t), 8);

    task_create(&tk, task_key, sk, 256, 3);
    task_create(&tb, task_beep, sb, 256, 4);
    task_create(&tl, task_led, sl, 256, 2);
    task_create(&ti, task_idle, si, 128, 0);
    current_tcb = &tk;
    NVIC_SetPriority(PendSV_IRQn, 0xFF);
    systick_init();
    start_first_task();
    while (1) {}
}
