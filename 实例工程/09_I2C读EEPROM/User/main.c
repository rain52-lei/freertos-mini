/**
 * 09_IIC读EEPROM - 官方 HAL 实验24 + MiniFreeRTOS
 */
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/24CXX/24cxx.h"
#include "rtos.h"
#include <stdio.h>

static uint32_t sa[256], sb[256], si[128];
static tcb_t ta, tb, ti;
static Mutex_t mi;

static void task_idle(void) { while (1) {} }

static void task_writer(void)
{
    uint8_t v = 0x5A;
    while (1)
    {
        xMutexTake(&mi, portMAX_DELAY);
        at24cxx_write_one_byte(0x10, v);
        printf("write 0x%02X\r\n", v);
        xMutexGive(&mi);
        v++;
        LED0_TOGGLE();
        task_delay(1000);
    }
}

static void task_reader(void)
{
    while (1)
    {
        task_delay(1200);
        xMutexTake(&mi, portMAX_DELAY);
        {
            uint8_t r = at24cxx_read_one_byte(0x10);
            printf("read  0x%02X\r\n", r);
        }
        xMutexGive(&mi);
        LED1_TOGGLE();
    }
}

int main(void)
{
    HAL_Init();
    sys_stm32_clock_init(336, 8, 2, 7);
    delay_init(168);
    usart_init(115200);
    led_init();
    at24cxx_init();
    xMutexCreate(&mi);
    printf("\r\n[09] IIC 24C02 + MiniFreeRTOS\r\n");

    task_create(&ta, task_writer, sa, 256, 2);
    task_create(&tb, task_reader, sb, 256, 2);
    task_create(&ti, task_idle, si, 128, 0);
    current_tcb = &ta;
    NVIC_SetPriority(PendSV_IRQn, 0xFF);
    systick_init();
    start_first_task();
    while (1) {}
}
