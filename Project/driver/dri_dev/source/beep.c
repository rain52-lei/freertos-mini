#include "beep.h"

void beep_init()
{
    // 使能 GPIOF 时钟
    RCC->AHB1ENR |= (1 << 5);

    // PF8配置为输出
    GPIOF->MODER &= ~(3 << (8 * 2));
    GPIOF->MODER |= (1 << (8 * 2));

    // 推挽输出
    GPIOF->OTYPER &= ~((1 << 8));

    // 高速
    GPIOF->OSPEEDR &= ~(3 << (8 * 2));
    GPIOF->OSPEEDR |= (3 << (8 * 2));

    GPIOF->ODR &= ~(1<<8);
}
void beep_on()
{
    GPIOF->ODR |= 1<<8;
}

void beep_off()
{
    GPIOF->ODR &= ~(1<<8);
}