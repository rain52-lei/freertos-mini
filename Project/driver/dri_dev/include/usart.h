#ifndef __USART_H
#define __USART_H

#include "stm32f4xx.h"


void usart_init();
void usart_send_byte(uint32_t data);
void usart_send_str(const char *str);
uint32_t usart_receive_byte();



#endif
