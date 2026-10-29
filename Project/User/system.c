#include "system.h"


void system_clock_init(void)
{
    RCC->CR |= (1 << 16);                 // HSEON
    while (!(RCC->CR & (1 << 17)));       // HSERDY

    FLASH->ACR |= (5 << 0);               // 5 wait states
    FLASH->ACR |= (1 << 8) | (1 << 9) | (1 << 10); // PRFTEN ICEN DCEN

    RCC->CFGR &= ~(0xF << 4);
    RCC->CFGR |=  (0 << 4);               // AHB DIV1

    RCC->CFGR &= ~(0x7 << 10);
    RCC->CFGR |=  (5 << 10);              // APB1 DIV4

    RCC->CFGR &= ~(0x7 << 13);
    RCC->CFGR |=  (4 << 13);              // APB2 DIV2

    RCC->PLLCFGR = (8 << 0) |             // PLLM = 8
                   (336 << 6) |           // PLLN = 336
                   (0 << 16) |            // PLLP = 2
                   (1 << 22) |            // PLL source HSE
                   (7 << 24);             // PLLQ = 7

    RCC->CR |= (1 << 24);                 // PLLON
    while (!(RCC->CR & (1 << 25)));       // PLLRDY

    RCC->CFGR &= ~(3 << 0);
    RCC->CFGR |=  (2 << 0);               // SYSCLK = PLL
    while (((RCC->CFGR >> 2) & 3) != 2);
}



