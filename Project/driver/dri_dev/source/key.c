#include "key.h"

void key_init()
{
    /* 开启GPIOE 和 GPIOA 的时钟 */
    RCC->AHB1ENR |= (1<<4) | (1 << 0);

    /* 输入模式 */
    GPIOE->MODER &= ~(63 << 4);
    GPIOA->MODER &= ~(3 << 0);

    /* 配置上下拉 */
    GPIOE->PUPDR &= ~(63 << 4);
    GPIOA->PUPDR &= ~(3 << 0);
    GPIOE->PUPDR |= (21 << 4);
    GPIOA->PUPDR |= (2 << 0);
}

int32_t key_scan()
{
    uint32_t key_0, key_1, key_2, key_up;
    key_up = GPIOA->IDR & 1;
    key_2 = GPIOE->IDR & (1 << 2);
    key_1 = GPIOE->IDR & (1 << 3);
    key_0 = GPIOE->IDR & (1 << 4);

    if(key_0 == 0)
    {
        return 0;
    }
    if(key_1 == 0)
    {
        return 1;
    }
    if(key_2 == 0)
    {
        return 2;
    }
    if(key_up == 1)
    {
        return 3;
    }
    return -1;
}