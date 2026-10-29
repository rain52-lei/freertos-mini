#include "usart.h"

void usart_init()
{
    /* 使能USART1时钟 */
    RCC->APB2ENR |= (1<<4);

    /* 使能GPIOA时钟 配置GPIOA  PA9-TX为复用推挽输出 PA10-RX为复用*/
    RCC->AHB1ENR |= (1<<0);
    GPIOA->MODER &= ~((3<<18) | (3<<20)); 
    GPIOA->MODER |= ((2<<18) | (2<<20));
    GPIOA->OTYPER &= ~(1<<9);

    /* 复用配置 */
    GPIOA->AFR[1] &= ~(0xff<<4);
    GPIOA->AFR[1] |= (7<<4);
    GPIOA->AFR[1] |= (7<<8);

    /* 配置波特率 */
    USART1->BRR = 729;

    /* 配置停止位，数据位，使能串口/接收/发送 */
    USART1->CR1 &= ~(3<<12);
    USART1->CR1 |= ((1<<13) |(1<<2) |(1<<3));
}

void usart_send_byte(uint32_t data)
{
    while(!(USART1->SR & (1<<7)));
    USART1->DR = data; 
}

void usart_send_str(const char *str)
{
    while(*str)
    {
        usart_send_byte(*str++);
    }
}

uint32_t usart_receive_byte()
{
    while(!(USART1->SR & (1<<5)));
    return (uint32_t)(USART1->DR & 0xff);
}




