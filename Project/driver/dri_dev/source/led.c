#include "led.h"

void led_init(void)
{
    // 使能 GPIOF 时钟
    RCC->AHB1ENR |= (1 << 5);

    // PF9 和 PF10 配置为输出
    GPIOF->MODER &= ~((3 << (9 * 2)) | (3 << (10 * 2)));
    GPIOF->MODER |=  ((1 << (9 * 2)) | (1 << (10 * 2)));

    // 推挽输出
    GPIOF->OTYPER &= ~((1 << 9) | (1 << 10));

    // 高速
    GPIOF->OSPEEDR |= ((3 << (9 * 2)) | (3 << (10 * 2)));
    GPIOF->ODR |= ((1 << 9) | (1 << 10));
}

void led0_toggle(void)
{
    GPIOF->ODR ^= (1 << 9);
}

void led1_toggle(void)
{
    GPIOF->ODR ^= (1 << 10);
}
