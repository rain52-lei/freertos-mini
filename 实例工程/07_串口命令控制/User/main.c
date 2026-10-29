/**
 * 07_串口命令控制 - 官方 HAL 串口 + MiniFreeRTOS
 * 命令: 1亮 0灭 t翻转 b蜂鸣(若有BEEP则响,无则只闪灯)
 */
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "rtos.h"
#include <stdio.h>

static uint32_t sc[256], sl[256], si[128];
static tcb_t tc, tl, ti;

static void task_idle(void) { while (1) {} }
static void task_led(void) { while (1) { LED1_TOGGLE(); task_delay(500); } }

static void task_cmd(void)
{
    while (1)
    {
        /* 轮询官方串口接收状态 */
        if (__HAL_UART_GET_FLAG(&g_uart1_handle, UART_FLAG_RXNE) != RESET)
        {
            uint8_t c = (uint8_t)(g_uart1_handle.Instance->DR & 0xFF);
            if (c == '1') LED0(0);
            else if (c == '0') LED0(1);
            else if (c == 't') LED0_TOGGLE();
            else if (c == 'b') { LED0(0); task_delay(80); LED0(1); }
            printf("got '%c'\r\n", c);
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
    printf("\r\n[07] cmd: 1/0/t/b\r\n");

    task_create(&tc, task_cmd, sc, 256, 3);
    task_create(&tl, task_led, sl, 256, 2);
    task_create(&ti, task_idle, si, 128, 0);
    current_tcb = &tc;
    NVIC_SetPriority(PendSV_IRQn, 0xFF);
    systick_init();
    start_first_task();
    while (1) {}
}
