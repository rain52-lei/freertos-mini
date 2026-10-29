#include "oled.h"
#include "oledfont.h"

#define OLED_SCL_PORT     GPIOD
#define OLED_SCL_PIN      6
#define OLED_SDA_PORT     GPIOD
#define OLED_SDA_PIN      7

#define OLED_SCL_H()      do{OLED_SCL_PORT->ODR |= (1<<OLED_SCL_PIN);}while(0)
#define OLED_SCL_L()      do{OLED_SCL_PORT->ODR &= ~(1<<OLED_SCL_PIN);}while(0)
#define OLED_SDA_H()      do{OLED_SDA_PORT->ODR |= (1<<OLED_SDA_PIN);}while(0)
#define OLED_SDA_L()      do{OLED_SDA_PORT->ODR &= ~(1<<OLED_SDA_PIN);}while(0)
#define OLED_SDA_READ()   ((OLED_SDA_PORT->IDR >> OLED_SDA_PIN) & 0x01)

#define OLED_ADDRESS      0x78

void oled_gpio_init()
{
    RCC->AHB1ENR |= (1<<3);

    GPIOD->MODER &= ~((3 << 12) | (3 << 14));
    GPIOD->MODER |= ((1 << 12) | (1 << 14));

    GPIOD->OTYPER &= ~((1 << 6) | (1 << 7));
    GPIOD->OSPEEDR |= ((2 << 12) | (2 << 14));
    GPIOD->PUPDR &= ~((3 << 12) | (3 << 14));

    OLED_SCL_H();
    OLED_SDA_H();
}

static void oled_delay()
{
    uint8_t i = 20;
    while(i--);
}

//起始信号
void oled_i2c_start()
{
    OLED_SDA_H();
    OLED_SCL_H();
    oled_delay();
    OLED_SDA_L();
    oled_delay();
    OLED_SCL_L();
}

//停止信号
void oled_i2c_stop()
{
    OLED_SDA_L();
    OLED_SCL_H();
    oled_delay();
    OLED_SDA_H();
    oled_delay();
}

//等待应答(返回0表示有应答)
uint8_t oled_i2c_wait_ack()
{
    uint8_t timeout = 0;

    OLED_SDA_H();
    oled_delay();
    OLED_SCL_H();
    oled_delay();

    while(OLED_SDA_READ())
    {
        timeout++;
        if(timeout > 20)
        {
            oled_i2c_stop();
            return 1;
        }
    }

    OLED_SCL_L();
    return 0;
}

//发送一个字节
void oled_i2c_send_byte(uint8_t byte)
{
    uint8_t i;

    for(i = 0;i < 8;i++)
    {
        if(byte & 0x80)
            OLED_SDA_H();
        else
            OLED_SDA_L();
        oled_delay();
        OLED_SCL_H();
        oled_delay();
        OLED_SCL_L();
        byte <<= 1;
    }
}

//写一个命令
void oled_write_cmd(uint8_t cmd)
{
    oled_i2c_start();
    oled_i2c_send_byte(OLED_ADDRESS);
    oled_i2c_wait_ack();
    oled_i2c_send_byte(0x00);
    oled_i2c_wait_ack();
    oled_i2c_send_byte(cmd);
    oled_i2c_wait_ack();
    oled_i2c_stop();
}

//写一个数据
void oled_write_data(uint8_t data)
{
    oled_i2c_start();
    oled_i2c_send_byte(OLED_ADDRESS);
    oled_i2c_wait_ack();
    oled_i2c_send_byte(0x40);
    oled_i2c_wait_ack();
    oled_i2c_send_byte(data);
    oled_i2c_wait_ack();
    oled_i2c_stop();
}

void oled_init()
{
    oled_gpio_init();
    delay_ms(100);

    oled_write_cmd(0xAE);

    oled_write_cmd(0xD5);
    oled_write_cmd(0x80);

    oled_write_cmd(0xA8);
    oled_write_cmd(0x3F);

    oled_write_cmd(0xD3);
    oled_write_cmd(0x00);

    oled_write_cmd(0x40);

    oled_write_cmd(0x8D);
    oled_write_cmd(0x14);

    oled_write_cmd(0x20);
    oled_write_cmd(0x02);

    oled_write_cmd(0xA1);
    oled_write_cmd(0xC8);

    oled_write_cmd(0xDA);
    oled_write_cmd(0x12);

    oled_write_cmd(0x81);
    oled_write_cmd(0xCF);

    oled_write_cmd(0xD9);
    oled_write_cmd(0xF1);

    oled_write_cmd(0xDB);
    oled_write_cmd(0x40);

    oled_write_cmd(0xA4);
    oled_write_cmd(0xA6);

    oled_write_cmd(0xAF);
}

//清屏
void oled_clear()
{
    uint8_t page, col;

    for(page = 0; page < 8; page++)
    {
        oled_write_cmd(0xB0 + page);
        oled_write_cmd(0x00);
        oled_write_cmd(0x10);

        for(col = 0;col < 128; col++)
        {
            oled_write_data(0x00);
        }
    }
}

//设置显示位置
void oled_set_pos(uint8_t page, uint8_t col)
{
    oled_write_cmd(0xB0 + page);
    oled_write_cmd(((col & 0xF0) >> 4) | 0x10);
    oled_write_cmd(col & 0x0F);
}

/**
 * @brief  在指定位置显示一个 ASCII 字符（16x16）
 * @param  page: 页地址（0~6，因为字符高16像素，占2页）
 * @param  col:  列地址（0~127）
 * @param  c:    要显示的字符
 */
void oled_show_char(uint8_t page, uint8_t col, char c)
{
    uint8_t i;
    uint8_t index = c - ' ';

    if(index >= 95) return;

    oled_set_pos(page,col);
    for(i = 0;i < 8; i++)
    {
        oled_write_data(oled_asc2_1608[index][i]);
    }

    oled_set_pos(page + 1,col);
    for(i = 0;i < 8; i++)
    {
        oled_write_data(oled_asc2_1608[index][i + 8]);
    }
}

/**
 * @brief  显示字符串
 * @param  page: 起始页
 * @param  col:  起始列
 * @param  str:  字符串指针
 */
void oled_show_string(uint8_t page, uint8_t col, char *str)
{
    while(*str != '\0')
    {
        oled_show_char(page, col, *str);
        col += 8;

        if(col > 120)
        {
            col = 0;
            page += 2;
        }
        str++;
    }
}