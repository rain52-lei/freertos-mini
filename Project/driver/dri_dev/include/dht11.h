#ifndef __DHT11_H
#define __DHT11_H

#include "stm32f4xx.h"
#include "delay.h"

void dht11_init();
uint8_t dht11_read_data(uint8_t *temp, uint8_t *humi);


#endif
