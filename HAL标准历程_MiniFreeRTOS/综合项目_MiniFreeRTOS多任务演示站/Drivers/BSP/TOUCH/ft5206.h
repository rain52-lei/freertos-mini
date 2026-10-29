/* 文件: ft5206.h | 编码: UTF-8 | 每行代码附中文注释 */
/**
 ****************************************************************************************************
 * @file        ft5206.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.1
 * @date        2023-05-29
 * @brief       7寸电容触摸屏-FT5206/FT5426 驱动代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 探索者 F407开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20211025
 * 第一次发布
 * V1.1 20230529
 * 兼容7寸 CST340触摸屏
 ****************************************************************************************************
 */

#ifndef __FT5206_H                                      /* 防止头文件重复包含 */
#define __FT5206_H                                      /* 定义包含保护宏 */

#include "./SYSTEM/sys/sys.h"                           /* 包含系统层头文件 */


/******************************************************************************************/
/* FT5206 INT 和 RST 引脚 定义 */

#define FT5206_RST_GPIO_PORT            GPIOC           /* 定义宏 */
#define FT5206_RST_GPIO_PIN             GPIO_PIN_13     /* 定义宏 */
#define FT5206_RST_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)   /* PC口时钟使能 */

#define FT5206_INT_GPIO_PORT            GPIOB           /* 定义宏 */
#define FT5206_INT_GPIO_PIN             GPIO_PIN_1      /* 定义宏 */
#define FT5206_INT_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PB口时钟使能 */

/******************************************************************************************/

/* 与电容触摸屏连接的芯片引脚(未包含IIC引脚) 
 * IO操作函数 
 */
#define FT5206_RST(x)     do{ x ? \
                              HAL_GPIO_WritePin(FT5206_RST_GPIO_PORT, FT5206_RST_GPIO_PIN, GPIO_PIN_SET) : \
                              HAL_GPIO_WritePin(FT5206_RST_GPIO_PORT, FT5206_RST_GPIO_PIN, GPIO_PIN_RESET); \
                          }while(0)       /* 复位引脚 */

#define FT5206_INT        HAL_GPIO_ReadPin(FT5206_INT_GPIO_PORT, FT5206_INT_GPIO_PIN)     /* 读取做的引脚 */

/* IIC读写命令 */
#define FT5206_CMD_WR               0X70        /* 写命令(最低位为0) */
#define FT5206_CMD_RD               0X71        /* 读命令(最低位为1) */

/* FT5206 部分寄存器定义  */
#define FT5206_DEVIDE_MODE          0x00        /* FT5206模式控制寄存器 */
#define FT5206_REG_NUM_FINGER       0x02        /* 触摸状态寄存器 */

#define FT5206_TP1_REG              0X03        /* 第一个触摸点数据地址 */
#define FT5206_TP2_REG              0X09        /* 第二个触摸点数据地址 */
#define FT5206_TP3_REG              0X0F        /* 第三个触摸点数据地址 */
#define FT5206_TP4_REG              0X15        /* 第四个触摸点数据地址 */
#define FT5206_TP5_REG              0X1B        /* 第五个触摸点数据地址 */ 


#define	FT5206_ID_G_LIB_VERSION     0xA1        /* 版本 */
#define FT5206_ID_G_MODE            0xA4        /* FT5206中断模式控制寄存器 */
#define FT5206_ID_G_THGROUP         0x80        /* 触摸有效值设置寄存器 */
#define FT5206_ID_G_PERIODACTIVE    0x88        /* 激活状态周期设置寄存器 */


uint8_t ft5206_wr_reg(uint16_t reg,uint8_t *buf,uint8_t len);  /* 8位无符号变量/函数 */
void ft5206_rd_reg(uint16_t reg,uint8_t *buf,uint8_t len);  /* 函数定义或声明 */
uint8_t ft5206_init(void);                              /* 8位无符号变量/函数 */
uint8_t ft5206_scan(uint8_t mode);                      /* 8位无符号变量/函数 */

#endif                                                  /* 条件编译结束 */

















