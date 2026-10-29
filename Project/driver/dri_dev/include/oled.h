#ifndef __OLED_H
#define __OLED_H

#include "stm32f4xx.h"
#include "delay.h"


void oled_gpio_init();
void oled_i2c_start();
void oled_i2c_stop();
uint8_t oled_i2c_wait_ack();
void oled_i2c_send_byte(uint8_t byte);
void oled_write_cmd(uint8_t cmd);
void oled_write_data(uint8_t data);
void oled_init();
void oled_clear();
void oled_set_pos(uint8_t page, uint8_t col);
void oled_show_char(uint8_t page, uint8_t col, char c);
void oled_show_string(uint8_t page, uint8_t col, char *str);




#endif
