#include "delay.h"

#define CPU_FREQ_HZ 168000000UL
#define CPU_CYCLES_PER_US (CPU_FREQ_HZ / 1000000UL)

void delay_init(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

void delay_us(uint32_t us)
{
    uint32_t start = DWT->CYCCNT;
    uint32_t ticks = us * CPU_CYCLES_PER_US;

    while ((DWT->CYCCNT - start) < ticks)
    {
    }
}

void delay_ms(uint32_t ms)
{
    while (ms--)
    {
        delay_us(1000);
    }
}
