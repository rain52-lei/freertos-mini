/* 文件: lcd_ex.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
 ****************************************************************************************************
 * @file        lcd_ex.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.2
 * @date        2024-12-14
 * @brief       lcd_ex.c存放各个LCD驱动IC的寄存器初始化部分代码,以简化lcd.c,该.c文件
 *              不直接加入到工程里面,只有lcd.c会用到,所以通过include的形式添加.(不要在
 *              其他文件再包含该.c文件!!否则会报错!) 
 * 
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 STM32F407开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20200323
 * 第一次发布
 * V1.1 20230529
 * 1，新增对ST7796和ILI9806 IC支持
 * V1.2 20241214
 * 1，对NT35510 IPS屏幕进行支持
 ****************************************************************************************************
 */

#include "./SYSTEM/delay/delay.h"                       /* 包含系统层头文件 */
#include "./BSP/LCD/lcd.h"                              /* 包含板级驱动头文件 */


/**
 * @brief       ST7789 寄存器初始化代码
 * @param       无
 * @retval      无
 */
void lcd_ex_st7789_reginit(void)                        /* 函数定义或声明 */
{                                                       /* 代码块开始 */
    lcd_wr_regno(0x11);                                 /* LCD驱动函数 */

    delay_ms(120);                                      /* 毫秒延时 */

    lcd_wr_regno(0x36);                                 /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */

    lcd_wr_regno(0x3A);                                 /* LCD驱动函数 */
    lcd_wr_data(0X05);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xB2);                                 /* LCD驱动函数 */
    lcd_wr_data(0x0C);                                  /* LCD驱动函数 */
    lcd_wr_data(0x0C);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x33);                                  /* LCD驱动函数 */
    lcd_wr_data(0x33);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xB7);                                 /* LCD驱动函数 */
    lcd_wr_data(0x35);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xBB); /* vcom */
    lcd_wr_data(0x32);  /* 30 */

    lcd_wr_regno(0xC0);                                 /* LCD驱动函数 */
    lcd_wr_data(0x0C);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xC2);                                 /* LCD驱动函数 */
    lcd_wr_data(0x01);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xC3); /* vrh */
    lcd_wr_data(0x10);  /* 17 0D */

    lcd_wr_regno(0xC4); /* vdv */
    lcd_wr_data(0x20);  /* 20 */

    lcd_wr_regno(0xC6);                                 /* LCD驱动函数 */
    lcd_wr_data(0x0f);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xD0);                                 /* LCD驱动函数 */
    lcd_wr_data(0xA4);                                  /* LCD驱动函数 */
    lcd_wr_data(0xA1);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xE0); /* Set Gamma  */
    lcd_wr_data(0xd0);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x02);                                  /* LCD驱动函数 */
    lcd_wr_data(0x07);                                  /* LCD驱动函数 */
    lcd_wr_data(0x0a);                                  /* LCD驱动函数 */
    lcd_wr_data(0x28);                                  /* LCD驱动函数 */
    lcd_wr_data(0x32);                                  /* LCD驱动函数 */
    lcd_wr_data(0X44);                                  /* LCD驱动函数 */
    lcd_wr_data(0x42);                                  /* LCD驱动函数 */
    lcd_wr_data(0x06);                                  /* LCD驱动函数 */
    lcd_wr_data(0x0e);                                  /* LCD驱动函数 */
    lcd_wr_data(0x12);                                  /* LCD驱动函数 */
    lcd_wr_data(0x14);                                  /* LCD驱动函数 */
    lcd_wr_data(0x17);                                  /* LCD驱动函数 */

    lcd_wr_regno(0XE1); /* Set Gamma */
    lcd_wr_data(0xd0);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x02);                                  /* LCD驱动函数 */
    lcd_wr_data(0x07);                                  /* LCD驱动函数 */
    lcd_wr_data(0x0a);                                  /* LCD驱动函数 */
    lcd_wr_data(0x28);                                  /* LCD驱动函数 */
    lcd_wr_data(0x31);                                  /* LCD驱动函数 */
    lcd_wr_data(0x54);                                  /* LCD驱动函数 */
    lcd_wr_data(0x47);                                  /* LCD驱动函数 */
    lcd_wr_data(0x0e);                                  /* LCD驱动函数 */
    lcd_wr_data(0x1c);                                  /* LCD驱动函数 */
    lcd_wr_data(0x17);                                  /* LCD驱动函数 */
    lcd_wr_data(0x1b);                                  /* LCD驱动函数 */
    lcd_wr_data(0x1e);                                  /* LCD驱动函数 */

    lcd_wr_regno(0x2A);                                 /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xef);                                  /* LCD驱动函数 */

    lcd_wr_regno(0x2B);                                 /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x01);                                  /* LCD驱动函数 */
    lcd_wr_data(0x3f);                                  /* LCD驱动函数 */

    lcd_wr_regno(0x29); /* display on */
}                                                       /* 代码块结束 */

/**
 * @brief       ILI9341寄存器初始化代码
 * @param       无
 * @retval      无
 */
void lcd_ex_ili9341_reginit(void)                       /* 函数定义或声明 */
{                                                       /* 代码块开始 */
    lcd_wr_regno(0xCF);                                 /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xC1);                                  /* LCD驱动函数 */
    lcd_wr_data(0X30);                                  /* LCD驱动函数 */
    lcd_wr_regno(0xED);                                 /* LCD驱动函数 */
    lcd_wr_data(0x64);                                  /* LCD驱动函数 */
    lcd_wr_data(0x03);                                  /* LCD驱动函数 */
    lcd_wr_data(0X12);                                  /* LCD驱动函数 */
    lcd_wr_data(0X81);                                  /* LCD驱动函数 */
    lcd_wr_regno(0xE8);                                 /* LCD驱动函数 */
    lcd_wr_data(0x85);                                  /* LCD驱动函数 */
    lcd_wr_data(0x10);                                  /* LCD驱动函数 */
    lcd_wr_data(0x7A);                                  /* LCD驱动函数 */
    lcd_wr_regno(0xCB);                                 /* LCD驱动函数 */
    lcd_wr_data(0x39);                                  /* LCD驱动函数 */
    lcd_wr_data(0x2C);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x34);                                  /* LCD驱动函数 */
    lcd_wr_data(0x02);                                  /* LCD驱动函数 */
    lcd_wr_regno(0xF7);                                 /* LCD驱动函数 */
    lcd_wr_data(0x20);                                  /* LCD驱动函数 */
    lcd_wr_regno(0xEA);                                 /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_regno(0xC0); /* Power control */
    lcd_wr_data(0x1B);  /* VRH[5:0] */
    lcd_wr_regno(0xC1); /* Power control */
    lcd_wr_data(0x01);  /* SAP[2:0];BT[3:0] */
    lcd_wr_regno(0xC5); /* VCM control */
    lcd_wr_data(0x30);  /* 3F */
    lcd_wr_data(0x30);  /* 3C */
    lcd_wr_regno(0xC7); /* VCM control2 */
    lcd_wr_data(0XB7);                                  /* LCD驱动函数 */
    lcd_wr_regno(0x36); /* Memory Access Control */
    lcd_wr_data(0x48);                                  /* LCD驱动函数 */
    lcd_wr_regno(0x3A);                                 /* LCD驱动函数 */
    lcd_wr_data(0x55);                                  /* LCD驱动函数 */
    lcd_wr_regno(0xB1);                                 /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x1A);                                  /* LCD驱动函数 */
    lcd_wr_regno(0xB6); /* Display Function Control */
    lcd_wr_data(0x0A);                                  /* LCD驱动函数 */
    lcd_wr_data(0xA2);                                  /* LCD驱动函数 */
    lcd_wr_regno(0xF2); /* 3Gamma Function Disable */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_regno(0x26); /* Gamma curve selected */
    lcd_wr_data(0x01);                                  /* LCD驱动函数 */
    lcd_wr_regno(0xE0); /* Set Gamma */
    lcd_wr_data(0x0F);                                  /* LCD驱动函数 */
    lcd_wr_data(0x2A);                                  /* LCD驱动函数 */
    lcd_wr_data(0x28);                                  /* LCD驱动函数 */
    lcd_wr_data(0x08);                                  /* LCD驱动函数 */
    lcd_wr_data(0x0E);                                  /* LCD驱动函数 */
    lcd_wr_data(0x08);                                  /* LCD驱动函数 */
    lcd_wr_data(0x54);                                  /* LCD驱动函数 */
    lcd_wr_data(0XA9);                                  /* LCD驱动函数 */
    lcd_wr_data(0x43);                                  /* LCD驱动函数 */
    lcd_wr_data(0x0A);                                  /* LCD驱动函数 */
    lcd_wr_data(0x0F);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_regno(0XE1); /* Set Gamma */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x15);                                  /* LCD驱动函数 */
    lcd_wr_data(0x17);                                  /* LCD驱动函数 */
    lcd_wr_data(0x07);                                  /* LCD驱动函数 */
    lcd_wr_data(0x11);                                  /* LCD驱动函数 */
    lcd_wr_data(0x06);                                  /* LCD驱动函数 */
    lcd_wr_data(0x2B);                                  /* LCD驱动函数 */
    lcd_wr_data(0x56);                                  /* LCD驱动函数 */
    lcd_wr_data(0x3C);                                  /* LCD驱动函数 */
    lcd_wr_data(0x05);                                  /* LCD驱动函数 */
    lcd_wr_data(0x10);                                  /* LCD驱动函数 */
    lcd_wr_data(0x0F);                                  /* LCD驱动函数 */
    lcd_wr_data(0x3F);                                  /* LCD驱动函数 */
    lcd_wr_data(0x3F);                                  /* LCD驱动函数 */
    lcd_wr_data(0x0F);                                  /* LCD驱动函数 */
    lcd_wr_regno(0x2B);                                 /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x01);                                  /* LCD驱动函数 */
    lcd_wr_data(0x3f);                                  /* LCD驱动函数 */
    lcd_wr_regno(0x2A);                                 /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xef);                                  /* LCD驱动函数 */
    lcd_wr_regno(0x11); /* Exit Sleep */
    delay_ms(120);                                      /* 毫秒延时 */
    lcd_wr_regno(0x29); /* display on */
 }                                                      /* 代码块结束 */


/**
 * @brief       NT35310寄存器初始化代码 
 * @param       无
 * @retval      无
 */
void lcd_ex_nt35310_reginit(void)                       /* 函数定义或声明 */
{                                                       /* 代码块开始 */
    lcd_wr_regno(0xED);                                 /* LCD驱动函数 */
    lcd_wr_data(0x01);                                  /* LCD驱动函数 */
    lcd_wr_data(0xFE);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xEE);                                 /* LCD驱动函数 */
    lcd_wr_data(0xDE);                                  /* LCD驱动函数 */
    lcd_wr_data(0x21);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xF1);                                 /* LCD驱动函数 */
    lcd_wr_data(0x01);                                  /* LCD驱动函数 */
    lcd_wr_regno(0xDF);                                 /* LCD驱动函数 */
    lcd_wr_data(0x10);                                  /* LCD驱动函数 */

    /* VCOMvoltage */
    lcd_wr_regno(0xC4);                                 /* LCD驱动函数 */
    lcd_wr_data(0x8F);  /* 5f */

    lcd_wr_regno(0xC6);                                 /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xE2);                                  /* LCD驱动函数 */
    lcd_wr_data(0xE2);                                  /* LCD驱动函数 */
    lcd_wr_data(0xE2);                                  /* LCD驱动函数 */
    lcd_wr_regno(0xBF);                                 /* LCD驱动函数 */
    lcd_wr_data(0xAA);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xB0);                                 /* LCD驱动函数 */
    lcd_wr_data(0x0D);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x0D);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x11);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x19);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x21);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x2D);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x3D);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x5D);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x5D);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xB1);                                 /* LCD驱动函数 */
    lcd_wr_data(0x80);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x8B);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x96);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xB2);                                 /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x02);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x03);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xB3);                                 /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xB4);                                 /* LCD驱动函数 */
    lcd_wr_data(0x8B);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x96);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xA1);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xB5);                                 /* LCD驱动函数 */
    lcd_wr_data(0x02);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x03);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x04);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xB6);                                 /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xB7);                                 /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x3F);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x5E);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x64);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x8C);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xAC);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xDC);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x70);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x90);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xEB);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xDC);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xB8);                                 /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xBA);                                 /* LCD驱动函数 */
    lcd_wr_data(0x24);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xC1);                                 /* LCD驱动函数 */
    lcd_wr_data(0x20);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x54);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xFF);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xC2);                                 /* LCD驱动函数 */
    lcd_wr_data(0x0A);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x04);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xC3);                                 /* LCD驱动函数 */
    lcd_wr_data(0x3C);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x3A);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x39);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x37);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x3C);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x36);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x32);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x2F);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x2C);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x29);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x26);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x24);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x24);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x23);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x3C);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x36);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x32);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x2F);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x2C);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x29);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x26);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x24);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x24);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x23);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xC4);                                 /* LCD驱动函数 */
    lcd_wr_data(0x62);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x05);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x84);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xF0);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x18);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xA4);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x18);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x50);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x0C);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x17);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x95);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xF3);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xE6);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xC5);                                 /* LCD驱动函数 */
    lcd_wr_data(0x32);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x44);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x65);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x76);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x88);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xC6);                                 /* LCD驱动函数 */
    lcd_wr_data(0x20);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x17);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x01);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xC7);                                 /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xC8);                                 /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xC9);                                 /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xE0);                                 /* LCD驱动函数 */
    lcd_wr_data(0x16);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x1C);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x21);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x36);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x46);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x52);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x64);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x7A);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x8B);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x99);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xA8);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xB9);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xC4);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xCA);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xD2);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xD9);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xE0);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xF3);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xE1);                                 /* LCD驱动函数 */
    lcd_wr_data(0x16);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x1C);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x22);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x36);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x45);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x52);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x64);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x7A);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x8B);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x99);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xA8);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xB9);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xC4);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xCA);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xD2);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xD8);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xE0);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xF3);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xE2);                                 /* LCD驱动函数 */
    lcd_wr_data(0x05);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x0B);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x1B);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x34);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x44);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x4F);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x61);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x79);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x88);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x97);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xA6);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xB7);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xC2);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xC7);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xD1);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xD6);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xDD);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xF3);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_regno(0xE3);                                 /* LCD驱动函数 */
    lcd_wr_data(0x05);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xA);                                   /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x1C);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x33);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x44);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x50);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x62);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x78);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x88);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x97);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xA6);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xB7);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xC2);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xC7);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xD1);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xD5);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xDD);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xF3);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xE4);                                 /* LCD驱动函数 */
    lcd_wr_data(0x01);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x01);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x02);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x2A);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x3C);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x4B);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x5D);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x74);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x84);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x93);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xA2);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xB3);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xBE);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xC4);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xCD);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xD3);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xDD);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xF3);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_regno(0xE5);                                 /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x02);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x29);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x3C);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x4B);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x5D);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x74);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x84);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x93);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xA2);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xB3);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xBE);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xC4);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xCD);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xD3);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xDC);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xF3);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xE6);                                 /* LCD驱动函数 */
    lcd_wr_data(0x11);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x34);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x56);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x76);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x77);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x66);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x88);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x99);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xBB);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x99);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x66);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x55);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x55);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x45);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x43);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x44);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xE7);                                 /* LCD驱动函数 */
    lcd_wr_data(0x32);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x55);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x76);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x66);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x67);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x67);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x87);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x99);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xBB);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x99);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x77);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x44);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x56);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x23);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x33);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x45);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xE8);                                 /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x99);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x87);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x88);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x77);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x66);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x88);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xAA);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xBB);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x99);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x66);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x55);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x55);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x44);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x44);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x55);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xE9);                                 /* LCD驱动函数 */
    lcd_wr_data(0xAA);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */

    lcd_wr_regno(0x00);                                 /* LCD驱动函数 */
    lcd_wr_data(0xAA);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xCF);                                 /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xF0);                                 /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x50);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xF3);                                 /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xF9);                                 /* LCD驱动函数 */
    lcd_wr_data(0x06);                                  /* LCD驱动函数 */
    lcd_wr_data(0x10);                                  /* LCD驱动函数 */
    lcd_wr_data(0x29);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */

    lcd_wr_regno(0x3A);                                 /* LCD驱动函数 */
    lcd_wr_data(0x55);  /* 66 */

    lcd_wr_regno(0x11);                                 /* LCD驱动函数 */
    delay_ms(100);                                      /* 毫秒延时 */
    lcd_wr_regno(0x29);                                 /* LCD驱动函数 */
    lcd_wr_regno(0x35);                                 /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */

    lcd_wr_regno(0x51);                                 /* LCD驱动函数 */
    lcd_wr_data(0xFF);                                  /* LCD驱动函数 */
    lcd_wr_regno(0x53);                                 /* LCD驱动函数 */
    lcd_wr_data(0x2C);                                  /* LCD驱动函数 */
    lcd_wr_regno(0x55);                                 /* LCD驱动函数 */
    lcd_wr_data(0x82);                                  /* LCD驱动函数 */
    lcd_wr_regno(0x2c);                                 /* LCD驱动函数 */
}                                                       /* 代码块结束 */

/**
 * @brief       ST7796寄存器初始化代码 
 * @param       无
 * @retval      无
 */
void lcd_ex_st7796_reginit(void)                        /* 函数定义或声明 */
{                                                       /* 代码块开始 */
    lcd_wr_regno(0x11);                                 /* LCD驱动函数 */

    delay_ms(120);                                      /* 毫秒延时 */

    lcd_wr_regno(0x36); /* Memory Data Access Control MY,MX~~ */
    lcd_wr_data(0x48);                                  /* LCD驱动函数 */

    lcd_wr_regno(0x3A);                                 /* LCD驱动函数 */
    lcd_wr_data(0x55);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xF0);                                 /* LCD驱动函数 */
    lcd_wr_data(0xC3);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xF0);                                 /* LCD驱动函数 */
    lcd_wr_data(0x96);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xB4);                                 /* LCD驱动函数 */
    lcd_wr_data(0x01);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xB6); /* Display Function Control */
    lcd_wr_data(0x0A);                                  /* LCD驱动函数 */
    lcd_wr_data(0xA2);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xB7);                                 /* LCD驱动函数 */
    lcd_wr_data(0xC6);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xB9);                                 /* LCD驱动函数 */
    lcd_wr_data(0x02);                                  /* LCD驱动函数 */
    lcd_wr_data(0xE0);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xC0);                                 /* LCD驱动函数 */
    lcd_wr_data(0x80);                                  /* LCD驱动函数 */
    lcd_wr_data(0x16);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xC1);                                 /* LCD驱动函数 */
    lcd_wr_data(0x19);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xC2);                                 /* LCD驱动函数 */
    lcd_wr_data(0xA7);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xC5);                                 /* LCD驱动函数 */
    lcd_wr_data(0x16);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xE8);                                 /* LCD驱动函数 */
    lcd_wr_data(0x40);                                  /* LCD驱动函数 */
    lcd_wr_data(0x8A);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x29);                                  /* LCD驱动函数 */
    lcd_wr_data(0x19);                                  /* LCD驱动函数 */
    lcd_wr_data(0xA5);                                  /* LCD驱动函数 */
    lcd_wr_data(0x33);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xE0);                                 /* LCD驱动函数 */
    lcd_wr_data(0xF0);                                  /* LCD驱动函数 */
    lcd_wr_data(0x07);                                  /* LCD驱动函数 */
    lcd_wr_data(0x0D);                                  /* LCD驱动函数 */
    lcd_wr_data(0x04);                                  /* LCD驱动函数 */
    lcd_wr_data(0x05);                                  /* LCD驱动函数 */
    lcd_wr_data(0x14);                                  /* LCD驱动函数 */
    lcd_wr_data(0x36);                                  /* LCD驱动函数 */
    lcd_wr_data(0x54);                                  /* LCD驱动函数 */
    lcd_wr_data(0x4C);                                  /* LCD驱动函数 */
    lcd_wr_data(0x38);                                  /* LCD驱动函数 */
    lcd_wr_data(0x13);                                  /* LCD驱动函数 */
    lcd_wr_data(0x14);                                  /* LCD驱动函数 */
    lcd_wr_data(0x2E);                                  /* LCD驱动函数 */
    lcd_wr_data(0x34);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xE1);                                 /* LCD驱动函数 */
    lcd_wr_data(0xF0);                                  /* LCD驱动函数 */
    lcd_wr_data(0x10);                                  /* LCD驱动函数 */
    lcd_wr_data(0x14);                                  /* LCD驱动函数 */
    lcd_wr_data(0x0E);                                  /* LCD驱动函数 */
    lcd_wr_data(0x0C);                                  /* LCD驱动函数 */
    lcd_wr_data(0x08);                                  /* LCD驱动函数 */
    lcd_wr_data(0x35);                                  /* LCD驱动函数 */
    lcd_wr_data(0x44);                                  /* LCD驱动函数 */
    lcd_wr_data(0x4C);                                  /* LCD驱动函数 */
    lcd_wr_data(0x26);                                  /* LCD驱动函数 */
    lcd_wr_data(0x10);                                  /* LCD驱动函数 */
    lcd_wr_data(0x12);                                  /* LCD驱动函数 */
    lcd_wr_data(0x2C);                                  /* LCD驱动函数 */
    lcd_wr_data(0x32);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xF0);                                 /* LCD驱动函数 */
    lcd_wr_data(0x3C);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xF0);                                 /* LCD驱动函数 */
    lcd_wr_data(0x69);                                  /* LCD驱动函数 */

    delay_ms(120);                                      /* 毫秒延时 */

    lcd_wr_regno(0x21);                                 /* LCD驱动函数 */
    lcd_wr_regno(0x29);                                 /* LCD驱动函数 */
}                                                       /* 代码块结束 */

/**
 * @brief       NT35510寄存器初始化代码 
 * @param       无
 * @retval      无
 */
void lcd_ex_nt35510_reginit(void)                       /* 函数定义或声明 */
{                                                       /* 代码块开始 */
    lcd_write_reg(0xF000, 0x55);                        /* LCD驱动函数 */
    lcd_write_reg(0xF001, 0xAA);                        /* LCD驱动函数 */
    lcd_write_reg(0xF002, 0x52);                        /* LCD驱动函数 */
    lcd_write_reg(0xF003, 0x08);                        /* LCD驱动函数 */
    lcd_write_reg(0xF004, 0x01);                        /* LCD驱动函数 */
    /* AVDD ratio */
    lcd_write_reg(0xB600, 0x34); /* 34 */
    lcd_write_reg(0xB601, 0x34); /* 34 */
    lcd_write_reg(0xB602, 0x34); /* 34 */
    /* AVDD 5.2V */
    lcd_write_reg(0xB000, 0x0D); /* 0C,减小，AVDD电压增大 */
    lcd_write_reg(0xB001, 0x0D); /* 0C */
    lcd_write_reg(0xB002, 0x0D); /* 0C */

    /* AVEE ratio */
    lcd_write_reg(0xB700, 0x24); /* 24 */
    lcd_write_reg(0xB701, 0x24); /* 24 */
    lcd_write_reg(0xB702, 0x24); /* 24 */
    /* AVEE -5.2V */
    lcd_write_reg(0xB100, 0x0D); /* 0C减小，AVEE电压增大 */
    lcd_write_reg(0xB101, 0x0D); /* 0C */
    lcd_write_reg(0xB102, 0x0D); /* 0C */

    /* VCL */
    lcd_write_reg(0xB800, 0x24);                        /* LCD驱动函数 */
    lcd_write_reg(0xB801, 0x24);                        /* LCD驱动函数 */
    lcd_write_reg(0xB802, 0x24);                        /* LCD驱动函数 */

    lcd_write_reg(0xB200, 0x00);                        /* LCD驱动函数 */

    /***************VGH ratio***************/
    /* VGH ratio */
    lcd_write_reg(0xB900, 0x24); /* 14 */
    lcd_write_reg(0xB901, 0x24); /* 14 */
    lcd_write_reg(0xB902, 0x24); /* 14 */
    /* VGH 10.2V */
    lcd_write_reg(0xB300, 0x05); /* 08 */
    lcd_write_reg(0xB301, 0x05); /* 08 */
    lcd_write_reg(0xB302, 0x05); /* 08 */

    /***************VGLX ratio***************/
    /* VGLX ratio */
    lcd_write_reg(0xBA00, 0x34);                        /* LCD驱动函数 */
    lcd_write_reg(0xBA01, 0x34);                        /* LCD驱动函数 */
    lcd_write_reg(0xBA02, 0x34);                        /* LCD驱动函数 */
    /* VGL_REG -12.6V */
    lcd_write_reg(0xB500, 0x0B);                        /* LCD驱动函数 */
    lcd_write_reg(0xB501, 0x0B);                        /* LCD驱动函数 */
    lcd_write_reg(0xB502, 0x0B);                        /* LCD驱动函数 */

    /***************VGMP/VGSP 4.5V***********/
    lcd_write_reg(0xBC00, 0x00); /* 00 */
    lcd_write_reg(0xBC01, 0xA3); /* 80 */
    lcd_write_reg(0xBC02, 0x00); /* 00 */

    /* GMN/VGSN -4.5V */
    lcd_write_reg(0xBD00, 0x00); /* 00 */
    lcd_write_reg(0xBD01, 0xA3); /* 80 */
    lcd_write_reg(0xBD02, 0x00); /* 00 */

    /* VCOM  -1.25V */
    lcd_write_reg(0xBE00, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xBE01, 0x37); /* 64~2F */


    /***************Gamma Setting***************/
    lcd_write_reg(0xD100, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD101, 0x37);                        /* LCD驱动函数 */
    lcd_write_reg(0xD102, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD103, 0x53);                        /* LCD驱动函数 */
    lcd_write_reg(0xD104, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD105, 0x79);                        /* LCD驱动函数 */
    lcd_write_reg(0xD106, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD107, 0x97);                        /* LCD驱动函数 */
    lcd_write_reg(0xD108, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD109, 0xB1);                        /* LCD驱动函数 */
    lcd_write_reg(0xD10A, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD10B, 0xD5);                        /* LCD驱动函数 */
    lcd_write_reg(0xD10C, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD10D, 0xF4);                        /* LCD驱动函数 */
    lcd_write_reg(0xD10E, 0x01);                        /* LCD驱动函数 */
    lcd_write_reg(0xD10F, 0x23);                        /* LCD驱动函数 */
    lcd_write_reg(0xD110, 0x01);                        /* LCD驱动函数 */
    lcd_write_reg(0xD111, 0x49);                        /* LCD驱动函数 */
    lcd_write_reg(0xD112, 0x01);                        /* LCD驱动函数 */
    lcd_write_reg(0xD113, 0x87);                        /* LCD驱动函数 */
    lcd_write_reg(0xD114, 0x01);                        /* LCD驱动函数 */
    lcd_write_reg(0xD115, 0xB6);                        /* LCD驱动函数 */
    lcd_write_reg(0xD116, 0x02);                        /* LCD驱动函数 */
    lcd_write_reg(0xD117, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD118, 0x02);                        /* LCD驱动函数 */
    lcd_write_reg(0xD119, 0x3B);                        /* LCD驱动函数 */
    lcd_write_reg(0xD11A, 0x02);                        /* LCD驱动函数 */
    lcd_write_reg(0xD11B, 0x3D);                        /* LCD驱动函数 */
    lcd_write_reg(0xD11C, 0x02);                        /* LCD驱动函数 */
    lcd_write_reg(0xD11D, 0x75);                        /* LCD驱动函数 */
    lcd_write_reg(0xD11E, 0x02);                        /* LCD驱动函数 */
    lcd_write_reg(0xD11F, 0xB1);                        /* LCD驱动函数 */
    lcd_write_reg(0xD120, 0x02);                        /* LCD驱动函数 */
    lcd_write_reg(0xD121, 0xD5);                        /* LCD驱动函数 */
    lcd_write_reg(0xD122, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD123, 0x09);                        /* LCD驱动函数 */
    lcd_write_reg(0xD124, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD125, 0x28);                        /* LCD驱动函数 */
    lcd_write_reg(0xD126, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD127, 0x52);                        /* LCD驱动函数 */
    lcd_write_reg(0xD128, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD129, 0x6B);                        /* LCD驱动函数 */
    lcd_write_reg(0xD12A, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD12B, 0x8D);                        /* LCD驱动函数 */
    lcd_write_reg(0xD12C, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD12D, 0xA2);                        /* LCD驱动函数 */
    lcd_write_reg(0xD12E, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD12F, 0xBB);                        /* LCD驱动函数 */
    lcd_write_reg(0xD130, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD131, 0xC1);                        /* LCD驱动函数 */
    lcd_write_reg(0xD132, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD133, 0xC1);                        /* LCD驱动函数 */

    lcd_write_reg(0xD200, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD201, 0x37);                        /* LCD驱动函数 */
    lcd_write_reg(0xD202, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD203, 0x53);                        /* LCD驱动函数 */
    lcd_write_reg(0xD204, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD205, 0x79);                        /* LCD驱动函数 */
    lcd_write_reg(0xD206, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD207, 0x97);                        /* LCD驱动函数 */
    lcd_write_reg(0xD208, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD209, 0xB1);                        /* LCD驱动函数 */
    lcd_write_reg(0xD20A, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD20B, 0xD5);                        /* LCD驱动函数 */
    lcd_write_reg(0xD20C, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD20D, 0xF4);                        /* LCD驱动函数 */
    lcd_write_reg(0xD20E, 0x01);                        /* LCD驱动函数 */
    lcd_write_reg(0xD20F, 0x23);                        /* LCD驱动函数 */
    lcd_write_reg(0xD210, 0x01);                        /* LCD驱动函数 */
    lcd_write_reg(0xD211, 0x49);                        /* LCD驱动函数 */
    lcd_write_reg(0xD212, 0x01);                        /* LCD驱动函数 */
    lcd_write_reg(0xD213, 0x87);                        /* LCD驱动函数 */
    lcd_write_reg(0xD214, 0x01);                        /* LCD驱动函数 */
    lcd_write_reg(0xD215, 0xB6);                        /* LCD驱动函数 */
    lcd_write_reg(0xD216, 0x02);                        /* LCD驱动函数 */
    lcd_write_reg(0xD217, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD218, 0x02);                        /* LCD驱动函数 */
    lcd_write_reg(0xD219, 0x3B);                        /* LCD驱动函数 */
    lcd_write_reg(0xD21A, 0x02);                        /* LCD驱动函数 */
    lcd_write_reg(0xD21B, 0x3D);                        /* LCD驱动函数 */
    lcd_write_reg(0xD21C, 0x02);                        /* LCD驱动函数 */
    lcd_write_reg(0xD21D, 0x75);                        /* LCD驱动函数 */
    lcd_write_reg(0xD21E, 0x02);                        /* LCD驱动函数 */
    lcd_write_reg(0xD21F, 0xB1);                        /* LCD驱动函数 */
    lcd_write_reg(0xD220, 0x02);                        /* LCD驱动函数 */
    lcd_write_reg(0xD221, 0xD5);                        /* LCD驱动函数 */
    lcd_write_reg(0xD222, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD223, 0x09);                        /* LCD驱动函数 */
    lcd_write_reg(0xD224, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD225, 0x28);                        /* LCD驱动函数 */
    lcd_write_reg(0xD226, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD227, 0x52);                        /* LCD驱动函数 */
    lcd_write_reg(0xD228, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD229, 0x6B);                        /* LCD驱动函数 */
    lcd_write_reg(0xD22A, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD22B, 0x8D);                        /* LCD驱动函数 */
    lcd_write_reg(0xD22C, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD22D, 0xA2);                        /* LCD驱动函数 */
    lcd_write_reg(0xD22E, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD22F, 0xBB);                        /* LCD驱动函数 */
    lcd_write_reg(0xD230, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD231, 0xC1);                        /* LCD驱动函数 */
    lcd_write_reg(0xD232, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD233, 0xC1);                        /* LCD驱动函数 */

    lcd_write_reg(0xD300, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD301, 0x37);                        /* LCD驱动函数 */
    lcd_write_reg(0xD302, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD303, 0x53);                        /* LCD驱动函数 */
    lcd_write_reg(0xD304, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD305, 0x79);                        /* LCD驱动函数 */
    lcd_write_reg(0xD306, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD307, 0x97);                        /* LCD驱动函数 */
    lcd_write_reg(0xD308, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD309, 0xB1);                        /* LCD驱动函数 */
    lcd_write_reg(0xD30A, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD30B, 0xD5);                        /* LCD驱动函数 */
    lcd_write_reg(0xD30C, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD30D, 0xF4);                        /* LCD驱动函数 */
    lcd_write_reg(0xD30E, 0x01);                        /* LCD驱动函数 */
    lcd_write_reg(0xD30F, 0x23);                        /* LCD驱动函数 */
    lcd_write_reg(0xD310, 0x01);                        /* LCD驱动函数 */
    lcd_write_reg(0xD311, 0x49);                        /* LCD驱动函数 */
    lcd_write_reg(0xD312, 0x01);                        /* LCD驱动函数 */
    lcd_write_reg(0xD313, 0x87);                        /* LCD驱动函数 */
    lcd_write_reg(0xD314, 0x01);                        /* LCD驱动函数 */
    lcd_write_reg(0xD315, 0xB6);                        /* LCD驱动函数 */
    lcd_write_reg(0xD316, 0x02);                        /* LCD驱动函数 */
    lcd_write_reg(0xD317, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD318, 0x02);                        /* LCD驱动函数 */
    lcd_write_reg(0xD319, 0x3b);                        /* LCD驱动函数 */
    lcd_write_reg(0xD31A, 0x02);                        /* LCD驱动函数 */
    lcd_write_reg(0xD31B, 0x3d);                        /* LCD驱动函数 */
    lcd_write_reg(0xD31C, 0x02);                        /* LCD驱动函数 */
    lcd_write_reg(0xD31D, 0x75);                        /* LCD驱动函数 */
    lcd_write_reg(0xD31E, 0x02);                        /* LCD驱动函数 */
    lcd_write_reg(0xD31F, 0xB1);                        /* LCD驱动函数 */
    lcd_write_reg(0xD320, 0x02);                        /* LCD驱动函数 */
    lcd_write_reg(0xD321, 0xD5);                        /* LCD驱动函数 */
    lcd_write_reg(0xD322, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD323, 0x09);                        /* LCD驱动函数 */
    lcd_write_reg(0xD324, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD325, 0x28);                        /* LCD驱动函数 */
    lcd_write_reg(0xD326, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD327, 0x52);                        /* LCD驱动函数 */
    lcd_write_reg(0xD328, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD329, 0x6B);                        /* LCD驱动函数 */
    lcd_write_reg(0xD32A, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD32B, 0x8D);                        /* LCD驱动函数 */
    lcd_write_reg(0xD32C, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD32D, 0xA2);                        /* LCD驱动函数 */
    lcd_write_reg(0xD32E, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD32F, 0xBB);                        /* LCD驱动函数 */
    lcd_write_reg(0xD330, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD331, 0xC1);                        /* LCD驱动函数 */
    lcd_write_reg(0xD332, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD333, 0xC1);                        /* LCD驱动函数 */

    lcd_write_reg(0xD400, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD401, 0x37);                        /* LCD驱动函数 */
    lcd_write_reg(0xD402, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD403, 0x53);                        /* LCD驱动函数 */
    lcd_write_reg(0xD404, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD405, 0x79);                        /* LCD驱动函数 */
    lcd_write_reg(0xD406, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD407, 0x97);                        /* LCD驱动函数 */
    lcd_write_reg(0xD408, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD409, 0xB1);                        /* LCD驱动函数 */
    lcd_write_reg(0xD40A, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD40B, 0xD5);                        /* LCD驱动函数 */
    lcd_write_reg(0xD40C, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD40D, 0xF4);                        /* LCD驱动函数 */
    lcd_write_reg(0xD40E, 0x01);                        /* LCD驱动函数 */
    lcd_write_reg(0xD40F, 0x23);                        /* LCD驱动函数 */
    lcd_write_reg(0xD410, 0x01);                        /* LCD驱动函数 */
    lcd_write_reg(0xD411, 0x49);                        /* LCD驱动函数 */
    lcd_write_reg(0xD412, 0x01);                        /* LCD驱动函数 */
    lcd_write_reg(0xD413, 0x87);                        /* LCD驱动函数 */
    lcd_write_reg(0xD414, 0x01);                        /* LCD驱动函数 */
    lcd_write_reg(0xD415, 0xB6);                        /* LCD驱动函数 */
    lcd_write_reg(0xD416, 0x02);                        /* LCD驱动函数 */
    lcd_write_reg(0xD417, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD418, 0x02);                        /* LCD驱动函数 */
    lcd_write_reg(0xD419, 0x3B);                        /* LCD驱动函数 */
    lcd_write_reg(0xD41A, 0x02);                        /* LCD驱动函数 */
    lcd_write_reg(0xD41B, 0x3D);                        /* LCD驱动函数 */
    lcd_write_reg(0xD41C, 0x02);                        /* LCD驱动函数 */
    lcd_write_reg(0xD41D, 0x75);                        /* LCD驱动函数 */
    lcd_write_reg(0xD41E, 0x02);                        /* LCD驱动函数 */
    lcd_write_reg(0xD41F, 0xB1);                        /* LCD驱动函数 */
    lcd_write_reg(0xD420, 0x02);                        /* LCD驱动函数 */
    lcd_write_reg(0xD421, 0xD5);                        /* LCD驱动函数 */
    lcd_write_reg(0xD422, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD423, 0x09);                        /* LCD驱动函数 */
    lcd_write_reg(0xD424, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD425, 0x28);                        /* LCD驱动函数 */
    lcd_write_reg(0xD426, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD427, 0x52);                        /* LCD驱动函数 */
    lcd_write_reg(0xD428, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD429, 0x6B);                        /* LCD驱动函数 */
    lcd_write_reg(0xD42A, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD42B, 0x8D);                        /* LCD驱动函数 */
    lcd_write_reg(0xD42C, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD42D, 0xA2);                        /* LCD驱动函数 */
    lcd_write_reg(0xD42E, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD42F, 0xBB);                        /* LCD驱动函数 */
    lcd_write_reg(0xD430, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD431, 0xC1);                        /* LCD驱动函数 */
    lcd_write_reg(0xD432, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD433, 0xC1);                        /* LCD驱动函数 */

    lcd_write_reg(0xD500, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD501, 0x37);                        /* LCD驱动函数 */
    lcd_write_reg(0xD502, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD503, 0x53);                        /* LCD驱动函数 */
    lcd_write_reg(0xD504, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD505, 0x79);                        /* LCD驱动函数 */
    lcd_write_reg(0xD506, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD507, 0x97);                        /* LCD驱动函数 */
    lcd_write_reg(0xD508, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD509, 0xB1);                        /* LCD驱动函数 */
    lcd_write_reg(0xD50A, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD50B, 0xD5);                        /* LCD驱动函数 */
    lcd_write_reg(0xD50C, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD50D, 0xF4);                        /* LCD驱动函数 */
    lcd_write_reg(0xD50E, 0x01);                        /* LCD驱动函数 */
    lcd_write_reg(0xD50F, 0x23);                        /* LCD驱动函数 */
    lcd_write_reg(0xD510, 0x01);                        /* LCD驱动函数 */
    lcd_write_reg(0xD511, 0x49);                        /* LCD驱动函数 */
    lcd_write_reg(0xD512, 0x01);                        /* LCD驱动函数 */
    lcd_write_reg(0xD513, 0x87);                        /* LCD驱动函数 */
    lcd_write_reg(0xD514, 0x01);                        /* LCD驱动函数 */
    lcd_write_reg(0xD515, 0xB6);                        /* LCD驱动函数 */
    lcd_write_reg(0xD516, 0x02);                        /* LCD驱动函数 */
    lcd_write_reg(0xD517, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD518, 0x02);                        /* LCD驱动函数 */
    lcd_write_reg(0xD519, 0x3B);                        /* LCD驱动函数 */
    lcd_write_reg(0xD51A, 0x02);                        /* LCD驱动函数 */
    lcd_write_reg(0xD51B, 0x3D);                        /* LCD驱动函数 */
    lcd_write_reg(0xD51C, 0x02);                        /* LCD驱动函数 */
    lcd_write_reg(0xD51D, 0x75);                        /* LCD驱动函数 */
    lcd_write_reg(0xD51E, 0x02);                        /* LCD驱动函数 */
    lcd_write_reg(0xD51F, 0xB1);                        /* LCD驱动函数 */
    lcd_write_reg(0xD520, 0x02);                        /* LCD驱动函数 */
    lcd_write_reg(0xD521, 0xD5);                        /* LCD驱动函数 */
    lcd_write_reg(0xD522, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD523, 0x09);                        /* LCD驱动函数 */
    lcd_write_reg(0xD524, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD525, 0x28);                        /* LCD驱动函数 */
    lcd_write_reg(0xD526, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD527, 0x52);                        /* LCD驱动函数 */
    lcd_write_reg(0xD528, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD529, 0x6B);                        /* LCD驱动函数 */
    lcd_write_reg(0xD52A, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD52B, 0x8D);                        /* LCD驱动函数 */
    lcd_write_reg(0xD52C, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD52D, 0xA2);                        /* LCD驱动函数 */
    lcd_write_reg(0xD52E, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD52F, 0xBB);                        /* LCD驱动函数 */
    lcd_write_reg(0xD530, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD531, 0xC1);                        /* LCD驱动函数 */
    lcd_write_reg(0xD532, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD533, 0xC1);                        /* LCD驱动函数 */

    lcd_write_reg(0xD600, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD601, 0x37);                        /* LCD驱动函数 */
    lcd_write_reg(0xD602, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD603, 0x53);                        /* LCD驱动函数 */
    lcd_write_reg(0xD604, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD605, 0x79);                        /* LCD驱动函数 */
    lcd_write_reg(0xD606, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD607, 0x97);                        /* LCD驱动函数 */
    lcd_write_reg(0xD608, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD609, 0xB1);                        /* LCD驱动函数 */
    lcd_write_reg(0xD60A, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD60B, 0xD5);                        /* LCD驱动函数 */
    lcd_write_reg(0xD60C, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD60D, 0xF4);                        /* LCD驱动函数 */
    lcd_write_reg(0xD60E, 0x01);                        /* LCD驱动函数 */
    lcd_write_reg(0xD60F, 0x23);                        /* LCD驱动函数 */
    lcd_write_reg(0xD610, 0x01);                        /* LCD驱动函数 */
    lcd_write_reg(0xD611, 0x49);                        /* LCD驱动函数 */
    lcd_write_reg(0xD612, 0x01);                        /* LCD驱动函数 */
    lcd_write_reg(0xD613, 0x87);                        /* LCD驱动函数 */
    lcd_write_reg(0xD614, 0x01);                        /* LCD驱动函数 */
    lcd_write_reg(0xD615, 0xB6);                        /* LCD驱动函数 */
    lcd_write_reg(0xD616, 0x02);                        /* LCD驱动函数 */
    lcd_write_reg(0xD617, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xD618, 0x02);                        /* LCD驱动函数 */
    lcd_write_reg(0xD619, 0x3B);                        /* LCD驱动函数 */
    lcd_write_reg(0xD61A, 0x02);                        /* LCD驱动函数 */
    lcd_write_reg(0xD61B, 0x3D);                        /* LCD驱动函数 */
    lcd_write_reg(0xD61C, 0x02);                        /* LCD驱动函数 */
    lcd_write_reg(0xD61D, 0x75);                        /* LCD驱动函数 */
    lcd_write_reg(0xD61E, 0x02);                        /* LCD驱动函数 */
    lcd_write_reg(0xD61F, 0xB1);                        /* LCD驱动函数 */
    lcd_write_reg(0xD620, 0x02);                        /* LCD驱动函数 */
    lcd_write_reg(0xD621, 0xD5);                        /* LCD驱动函数 */
    lcd_write_reg(0xD622, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD623, 0x09);                        /* LCD驱动函数 */
    lcd_write_reg(0xD624, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD625, 0x28);                        /* LCD驱动函数 */
    lcd_write_reg(0xD626, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD627, 0x52);                        /* LCD驱动函数 */
    lcd_write_reg(0xD628, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD629, 0x6B);                        /* LCD驱动函数 */
    lcd_write_reg(0xD62A, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD62B, 0x8D);                        /* LCD驱动函数 */
    lcd_write_reg(0xD62C, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD62D, 0xA2);                        /* LCD驱动函数 */
    lcd_write_reg(0xD62E, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD62F, 0xBB);                        /* LCD驱动函数 */
    lcd_write_reg(0xD630, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD631, 0xC1);                        /* LCD驱动函数 */
    lcd_write_reg(0xD632, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xD633, 0xC1);                        /* LCD驱动函数 */

    /***************LV2 Page 0 enable***************/
    lcd_write_reg(0xF000, 0x55);                        /* LCD驱动函数 */
    lcd_write_reg(0xF001, 0xAA);                        /* LCD驱动函数 */
    lcd_write_reg(0xF002, 0x52);                        /* LCD驱动函数 */
    lcd_write_reg(0xF003, 0x08);                        /* LCD驱动函数 */
    lcd_write_reg(0xF004, 0x00);                        /* LCD驱动函数 */

    /***************480x800***************/
    //lcd_write_reg(0xB500, 0x50);
    //RGB Internal mode
    //REGW 0xB30001
    //RGB I/F Setting

    lcd_write_reg(0xB000, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xB001, 0x05);                        /* LCD驱动函数 */
    lcd_write_reg(0xB002, 0x02);                        /* LCD驱动函数 */
    lcd_write_reg(0xB003, 0x05);                        /* LCD驱动函数 */
    lcd_write_reg(0xB004, 0x02);                        /* LCD驱动函数 */

    /*## SDT: */
    lcd_write_reg(0xB600, 0x08);                        /* LCD驱动函数 */
    lcd_write_reg(0xB500, 0x50); /* 0x6b 480x854       0x50 480x800 */

    /***************Display control***************/
    //lcd_write_reg(0xB100, 0xCC);
    //lcd_write_reg(0xB101, 0x00);

    /***************Source hold time***************/
    lcd_write_reg(0xB600, 0x05);                        /* LCD驱动函数 */

    /***************Gate EQ control***************/
    lcd_write_reg(0xB700, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xB701, 0x00);                        /* LCD驱动函数 */


    /***************Source EQ control (Mode 2)***************/
    lcd_write_reg(0xB800, 0x01);                        /* LCD驱动函数 */
    lcd_write_reg(0xB801, 0x05);                        /* LCD驱动函数 */
    lcd_write_reg(0xB802, 0x05);                        /* LCD驱动函数 */
    lcd_write_reg(0xB803, 0x05);                        /* LCD驱动函数 */

    /***************Inversion mode  (2-dot)***************/
    lcd_write_reg(0xBC00, 0x02);//02,2-dot inversion    /* LCD驱动函数 */
    lcd_write_reg(0xBC01, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xBC02, 0x00);                        /* LCD驱动函数 */


    /*************** BOE SETTING(DEFAULT) ***************/
    lcd_write_reg(0xCC00, 0x03);                        /* LCD驱动函数 */
    lcd_write_reg(0xCC01, 0x00);                        /* LCD驱动函数 */
    lcd_write_reg(0xCC02, 0x00);                        /* LCD驱动函数 */

    /***************Frame rate***************/
    lcd_write_reg(0xBD00, 0x01);                        /* LCD驱动函数 */
    lcd_write_reg(0xBD01, 0x84);                        /* LCD驱动函数 */
    lcd_write_reg(0xBD02, 0x07);  /* 0X1C   0x07 */
    lcd_write_reg(0xBD03, 0x31);                        /* LCD驱动函数 */
    lcd_write_reg(0xBD04, 0x00);                        /* LCD驱动函数 */


    lcd_write_reg(0xBA00, 0x01);                        /* LCD驱动函数 */

    lcd_write_reg(0xFF00, 0xAA);                        /* LCD驱动函数 */
    lcd_write_reg(0xFF01, 0x55);                        /* LCD驱动函数 */
    lcd_write_reg(0xFF02, 0x25);                        /* LCD驱动函数 */
    lcd_write_reg(0xFF03, 0x01);                        /* LCD驱动函数 */

    /*************** Timing control 4H w/ 4-Delayms ***************/
    lcd_write_reg(0x3600, 0x40);                        /* LCD驱动函数 */
    lcd_write_reg(0x3500, 0x00);                        /* LCD驱动函数 */

    lcd_write_reg(0x3A00, 0x05);                        /* LCD驱动函数 */

    lcd_wr_regno(0x1100);                               /* LCD驱动函数 */
    delay_ms(120);                                      /* 毫秒延时 */
    lcd_wr_regno(0x2900);                               /* LCD驱动函数 */
    delay_ms(10);                                       /* 毫秒延时 */
    lcd_wr_regno(0x2C00);                               /* LCD驱动函数 */
}                                                       /* 代码块结束 */

/**
 * @brief       ILI9806寄存器初始化代码 
 * @param       无
 * @retval      无
 */
void lcd_ex_ili9806_reginit(void)                       /* 函数定义或声明 */
{                                                       /* 代码块开始 */
    lcd_wr_regno(0xFF); /* EXTC Command Set enable register */
    lcd_wr_data(0xFF);                                  /* LCD驱动函数 */
    lcd_wr_data(0x98);                                  /* LCD驱动函数 */
    lcd_wr_data(0x06);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xBC); /* GIP 1 */
    lcd_wr_data(0x01);                                  /* LCD驱动函数 */
    lcd_wr_data(0x0F);                                  /* LCD驱动函数 */
    lcd_wr_data(0x61);                                  /* LCD驱动函数 */
    lcd_wr_data(0xFF);                                  /* LCD驱动函数 */
    lcd_wr_data(0x01);                                  /* LCD驱动函数 */
    lcd_wr_data(0x01);                                  /* LCD驱动函数 */
    lcd_wr_data(0x0B);                                  /* LCD驱动函数 */
    lcd_wr_data(0x10);                                  /* LCD驱动函数 */
    lcd_wr_data(0x37);                                  /* LCD驱动函数 */
    lcd_wr_data(0x63);                                  /* LCD驱动函数 */
    lcd_wr_data(0xFF);                                  /* LCD驱动函数 */
    lcd_wr_data(0xFF);                                  /* LCD驱动函数 */
    lcd_wr_data(0x01);                                  /* LCD驱动函数 */
    lcd_wr_data(0x01);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0xFF);                                  /* LCD驱动函数 */
    lcd_wr_data(0x52);                                  /* LCD驱动函数 */
    lcd_wr_data(0x01);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x40);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xBD); /* GIP 2 */
    lcd_wr_data(0x01);                                  /* LCD驱动函数 */
    lcd_wr_data(0x23);                                  /* LCD驱动函数 */
    lcd_wr_data(0x45);                                  /* LCD驱动函数 */
    lcd_wr_data(0x67);                                  /* LCD驱动函数 */
    lcd_wr_data(0x01);                                  /* LCD驱动函数 */
    lcd_wr_data(0x23);                                  /* LCD驱动函数 */
    lcd_wr_data(0x45);                                  /* LCD驱动函数 */
    lcd_wr_data(0x67);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xBE); /* GIP 3 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x01);                                  /* LCD驱动函数 */
    lcd_wr_data(0xAB);                                  /* LCD驱动函数 */
    lcd_wr_data(0x60);                                  /* LCD驱动函数 */
    lcd_wr_data(0x22);                                  /* LCD驱动函数 */
    lcd_wr_data(0x22);                                  /* LCD驱动函数 */
    lcd_wr_data(0x22);                                  /* LCD驱动函数 */
    lcd_wr_data(0x22);                                  /* LCD驱动函数 */
    lcd_wr_data(0x22);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xC7); /* VCOM Control */
    lcd_wr_data(0x36);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xED); /* EN_volt_reg VGMP / VGMN /VGSP / VGSN voltage to output */
    lcd_wr_data(0x7F);                                  /* LCD驱动函数 */
    lcd_wr_data(0x0F);                                  /* LCD驱动函数 */

    lcd_wr_regno(0XC0); /* Power Control 1 Setting AVDD / AVEE / VGH / VGL */
    lcd_wr_data(0x0F);                                  /* LCD驱动函数 */
    lcd_wr_data(0x0B);                                  /* LCD驱动函数 */
    lcd_wr_data(0x0A);  /* VGH 15V,VGLO-10V */

    lcd_wr_regno(0XFC); /* AVDD / AVEE generated by internal pumping. */
    lcd_wr_data(0x08);                                  /* LCD驱动函数 */

    lcd_wr_regno(0XDF);                                 /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x20);                                  /* LCD驱动函数 */

    lcd_wr_regno(0XF3); /* DVDD Voltage Setting */
    lcd_wr_data(0x74);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xB4); /* Inversion Type */
    lcd_wr_data(0x00);  /* 02 */
    lcd_wr_data(0x00);  /* 02 */
    lcd_wr_data(0x00);  /* 02 */

    lcd_wr_regno(0xF7); /* Resolution Control */
    lcd_wr_data(0x82);  /* 480*800 */

    lcd_wr_regno(0xB1); /* FRAME RATE Setting */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x13);                                  /* LCD驱动函数 */
    lcd_wr_data(0x13);                                  /* LCD驱动函数 */

    lcd_wr_regno(0XF2); /* CR_EQ_PC_SDT  #C0,06,40,28 */
    lcd_wr_data(0x80);                                  /* LCD驱动函数 */
    lcd_wr_data(0x04);                                  /* LCD驱动函数 */
    lcd_wr_data(0x40);                                  /* LCD驱动函数 */
    lcd_wr_data(0x28);                                  /* LCD驱动函数 */

    lcd_wr_regno(0XC1); /* Power Control 2  SD OP Bias_VRH1_VRH2_EXT_CPCK_SEL */
    lcd_wr_data(0x17);                                  /* LCD驱动函数 */
    lcd_wr_data(0x88);  /* VGMP */
    lcd_wr_data(0x88);  /* VGMN */
    lcd_wr_data(0x20);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xE0); /* Positive Gamma Control */
    lcd_wr_data(0x00);  /* P1 */
    lcd_wr_data(0x0A);  /* P2 */
    lcd_wr_data(0x12);  /* P3 */
    lcd_wr_data(0x10);  /* P4 */
    lcd_wr_data(0x0E);  /* P5 */
    lcd_wr_data(0x20);  /* P6 */
    lcd_wr_data(0xCC);  /* P7 */
    lcd_wr_data(0x07);  /* P8 */
    lcd_wr_data(0x06);  /* P9 */
    lcd_wr_data(0x0B);  /* P10 */
    lcd_wr_data(0x0E);  /* P11 */
    lcd_wr_data(0x0F);  /* P12 */
    lcd_wr_data(0x0D);  /* P13 */
    lcd_wr_data(0x15);  /* P14 */
    lcd_wr_data(0x10);  /* P15 */
    lcd_wr_data(0x00);  /* P16 */

    lcd_wr_regno(0xE1); /* Negative Gamma Correction */
    lcd_wr_data(0x00);  /* P1 */
    lcd_wr_data(0x0B);  /* P2 */
    lcd_wr_data(0x13);  /* P3 */
    lcd_wr_data(0x0D);  /* P4 */
    lcd_wr_data(0x0E);  /* P5 */
    lcd_wr_data(0x1B);  /* P6 */
    lcd_wr_data(0x71);  /* P7 */
    lcd_wr_data(0x06);  /* P8 */
    lcd_wr_data(0x06);  /* P9 */
    lcd_wr_data(0x0A);  /* P10 */
    lcd_wr_data(0x0F);  /* P11 */
    lcd_wr_data(0x0E);  /* P12 */
    lcd_wr_data(0x0F);  /* P13 */
    lcd_wr_data(0x15);  /* P14 */
    lcd_wr_data(0x0C);  /* P15 */
    lcd_wr_data(0x00);  /* P16 */

    lcd_wr_regno(0x2a);                                 /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x01);                                  /* LCD驱动函数 */
    lcd_wr_data(0xdf);                                  /* LCD驱动函数 */

    lcd_wr_regno(0x2b);                                 /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x03);                                  /* LCD驱动函数 */
    lcd_wr_data(0x1f);                                  /* LCD驱动函数 */

    lcd_wr_regno(0x3A); /* Pixel Format */
    lcd_wr_data(0x55);                                  /* LCD驱动函数 */

    lcd_wr_regno(0x36); /* Memory Access Control */
    lcd_wr_data(0x00);  /* 02-180 */

    lcd_wr_regno(0x11);                                 /* LCD驱动函数 */
    delay_ms(120);                                      /* 毫秒延时 */
    lcd_wr_regno(0x29);                                 /* LCD驱动函数 */
    delay_ms(20);                                       /* 毫秒延时 */
    lcd_wr_regno(0x2C);                                 /* LCD驱动函数 */
}                                                       /* 代码块结束 */

/**
 * @brief       SSD1963寄存器初始化代码 
 * @param       无
 * @retval      无
 */
void lcd_ex_ssd1963_reginit(void)                       /* 函数定义或声明 */
{                                                       /* 代码块开始 */
    lcd_wr_regno(0xE2); /* Set PLL with OSC = 10MHz (hardware),	Multiplier N = 35, 250MHz < VCO < 800MHz = OSC*(N+1), VCO = 300MHz */
    lcd_wr_data(0x1D);  /* 参数1 */
    lcd_wr_data(0x02);  /* 参数2 Divider M = 2, PLL = 300/(M+1) = 100MHz */
    lcd_wr_data(0x04);  /* 参数3 Validate M and N values */
    delay_us(100);                                      /* 微秒延时 */
    lcd_wr_regno(0xE0); /*  Start PLL command */
    lcd_wr_data(0x01);  /*  enable PLL */
    delay_ms(10);                                       /* 毫秒延时 */
    lcd_wr_regno(0xE0); /*  Start PLL command again */
    lcd_wr_data(0x03);  /*  now, use PLL output as system clock */
    delay_ms(12);                                       /* 毫秒延时 */
    lcd_wr_regno(0x01); /* 软复位 */
    delay_ms(10);                                       /* 毫秒延时 */

    lcd_wr_regno(0xE6); /* 设置像素频率,33Mhz */
    lcd_wr_data(0x2F);                                  /* LCD驱动函数 */
    lcd_wr_data(0xFF);                                  /* LCD驱动函数 */
    lcd_wr_data(0xFF);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xB0); /* 设置LCD模式 */
    lcd_wr_data(0x20);  /* 24位模式 */
    lcd_wr_data(0x00);  /* TFT 模式 */

    lcd_wr_data((SSD_HOR_RESOLUTION - 1) >> 8); /* 设置LCD水平像素 */
    lcd_wr_data(SSD_HOR_RESOLUTION - 1);                /* LCD驱动函数 */
    lcd_wr_data((SSD_VER_RESOLUTION - 1) >> 8); /* 设置LCD垂直像素 */
    lcd_wr_data(SSD_VER_RESOLUTION - 1);                /* LCD驱动函数 */
    lcd_wr_data(0x00);  /* RGB序列 */

    lcd_wr_regno(0xB4); /* Set horizontal period */
    lcd_wr_data((SSD_HT - 1) >> 8);                     /* LCD驱动函数 */
    lcd_wr_data(SSD_HT - 1);                            /* LCD驱动函数 */
    lcd_wr_data(SSD_HPS >> 8);                          /* LCD驱动函数 */
    lcd_wr_data(SSD_HPS);                               /* LCD驱动函数 */
    lcd_wr_data(SSD_HOR_PULSE_WIDTH - 1);               /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_regno(0xB6); /* Set vertical perio */
    lcd_wr_data((SSD_VT - 1) >> 8);                     /* LCD驱动函数 */
    lcd_wr_data(SSD_VT - 1);                            /* LCD驱动函数 */
    lcd_wr_data(SSD_VPS >> 8);                          /* LCD驱动函数 */
    lcd_wr_data(SSD_VPS);                               /* LCD驱动函数 */
    lcd_wr_data(SSD_VER_FRONT_PORCH - 1);               /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */
    lcd_wr_data(0x00);                                  /* LCD驱动函数 */

    lcd_wr_regno(0xF0); /* 设置SSD1963与CPU接口为16bit */
    lcd_wr_data(0x03);  /* 16-bit(565 format) data for 16bpp */

    lcd_wr_regno(0x29); /* 开启显示 */
    /* 设置PWM输出  背光通过占空比可调 */
    lcd_wr_regno(0xD0); /* 设置自动白平衡DBC */
    lcd_wr_data(0x00);  /* disable */

    lcd_wr_regno(0xBE); /* 配置PWM输出 */
    lcd_wr_data(0x05);  /* 1设置PWM频率 */
    lcd_wr_data(0xFE);  /* 2设置PWM占空比 */
    lcd_wr_data(0x01);  /* 3设置C */
    lcd_wr_data(0x00);  /* 4设置D */
    lcd_wr_data(0x00);  /* 5设置E */
    lcd_wr_data(0x00);  /* 6设置F */

    lcd_wr_regno(0xB8); /* 设置GPIO配置 */
    lcd_wr_data(0x03);  /* 2个IO口设置成输出 */
    lcd_wr_data(0x01);  /* GPIO使用正常的IO功能 */
    lcd_wr_regno(0xBA);                                 /* LCD驱动函数 */
    lcd_wr_data(0X01);  /* GPIO[1:0]=01,控制LCD方向 */
}                                                       /* 代码块结束 */


















