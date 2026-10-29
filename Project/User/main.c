#include "system.h"
#include "delay.h"
#include "rtos.h"
#include "led.h"
#include "task.h"
#include "beep.h"
#include "key.h"
#include "usart.h"
#include "tpad.h"
#include "dht11.h"
#include "oled.h"

/* 保留这些全局对象，是为了以后切回 MiniFreeRTOS 任务实验时还能直接使用。 */
uint32_t stack1[256];
uint32_t stack2[256];
uint32_t stack_idle[128];

tcb_t tcb1;
tcb_t tcb2;
tcb_t tcb_idle;

Semaphore_t ledSem;
Mutex_t ledMutex;

Queue_t myQueue;
uint8_t queueBuffer[20 * sizeof(int32_t)];

int main(void)
{
    //uint8_t t = 1;

    /* 先配置系统时钟和延时，再初始化依赖时钟的外设。 */
    system_clock_init();
    delay_init();
    usart_init();
    led_init();
    beep_init();
    key_init();
    oled_init();
    oled_clear();         // 清屏

    // 显示测试
    oled_show_string(0, 0, "Hello OLED");
    oled_show_string(2, 0, "STM32F407");
    oled_show_string(4, 0, "1234567890");

//    /* TPAD 上电后先采样空载基准值；失败时用串口和 LED0 快闪提示。 */
//    if (tpad_init() != 0)
//    {
//        usart_send_str("TPAD init failed\r\n");

//        while (1)
//        {
//            led0_toggle();
//            delay_ms(100);
//        }
//    }

//    usart_send_str("TPAD init ok\r\n");

    /* LED0 做心跳，确认程序在运行；触摸 TPAD 时翻转 LED1。 */
    while (1)
    {
//        if (tpad_scan(0))
//        {
//            led1_toggle();
//        }

//        t++;
//        if (t == 15)
//        {
//            t = 0;
//            led0_toggle();
//        }

//        delay_ms(10);
    }
}
