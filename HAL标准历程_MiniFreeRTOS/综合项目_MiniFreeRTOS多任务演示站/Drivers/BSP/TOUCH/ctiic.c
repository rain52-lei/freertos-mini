/* 文件: ctiic.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
 ****************************************************************************************************
 * @file        ctiic.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-25
 * @brief       电容触摸屏 驱动代码
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
 *
 ****************************************************************************************************
 */

#include "./BSP/TOUCH/ctiic.h"                          /* 包含板级驱动头文件 */
#include "./SYSTEM/delay/delay.h"                       /* 包含系统层头文件 */


/**
 * @brief       控制I2C速度的延时
 * @param       无
 * @retval      无
 */
static void ct_iic_delay(void)                          /* 静态函数 */
{                                                       /* 代码块开始 */
    delay_us(2);                                        /* 微秒延时 */
}                                                       /* 代码块结束 */

/**
 * @brief       电容触摸芯片IIC接口初始化
 * @param       无
 * @retval      无
 */
void ct_iic_init(void)                                  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
    GPIO_InitTypeDef gpio_init_struct;                  /* GPIO相关操作 */

    CT_IIC_SCL_GPIO_CLK_ENABLE();   /* SCL引脚时钟使能 */
    CT_IIC_SDA_GPIO_CLK_ENABLE();   /* SDA引脚时钟使能 */

    gpio_init_struct.Pin = CT_IIC_SCL_GPIO_PIN;         /* 赋值或初始化 */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_OD;             /* 开漏输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                     /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;      /* 高速 */
    HAL_GPIO_Init(CT_IIC_SCL_GPIO_PORT, &gpio_init_struct);  /* 初始化SCL引脚 */

    gpio_init_struct.Pin = CT_IIC_SDA_GPIO_PIN;         /* 赋值或初始化 */
    HAL_GPIO_Init(CT_IIC_SDA_GPIO_PORT, &gpio_init_struct);  /* 初始化SDA引脚 */
    /* SDA引脚模式设置,开漏输出,上拉, 这样就不用再设置IO方向了, 开漏输出的时候(=1), 也可以读取外部信号的高低电平 */

    ct_iic_stop();  /* 停止总线上所有设备 */
}                                                       /* 代码块结束 */

/**
 * @brief       产生IIC起始信号
 * @param       无
 * @retval      无
 */
void ct_iic_start(void)                                 /* 函数定义或声明 */
{                                                       /* 代码块开始 */
    CT_IIC_SDA(1);                                      /* 执行语句 */
    CT_IIC_SCL(1);                                      /* 执行语句 */
    ct_iic_delay();                                     /* 执行语句 */
    CT_IIC_SDA(0);      /* START信号: 当SCL为高时, SDA从高变成低, 表示起始信号 */
    ct_iic_delay();                                     /* 执行语句 */
    CT_IIC_SCL(0);      /* 钳住I2C总线，准备发送或接收数据 */
    ct_iic_delay();                                     /* 执行语句 */
}                                                       /* 代码块结束 */

/**
 * @brief       产生IIC停止信号
 * @param       无
 * @retval      无
 */
void ct_iic_stop(void)                                  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
    CT_IIC_SDA(0);      /* STOP信号: 当SCL为高时, SDA从低变成高, 表示停止信号 */
    ct_iic_delay();                                     /* 执行语句 */
    CT_IIC_SCL(1);                                      /* 执行语句 */
    ct_iic_delay();                                     /* 执行语句 */
    CT_IIC_SDA(1);      /* 发送I2C总线结束信号 */
    ct_iic_delay();                                     /* 执行语句 */
}                                                       /* 代码块结束 */

/**
 * @brief       等待应答信号到来
 * @param       无
 * @retval      1，接收应答失败
 *              0，接收应答成功
 */
uint8_t ct_iic_wait_ack(void)                           /* 8位无符号变量/函数 */
{                                                       /* 代码块开始 */
    uint8_t waittime = 0;                               /* 8位无符号变量/函数 */
    uint8_t rack = 0;                                   /* 8位无符号变量/函数 */

    CT_IIC_SDA(1);      /* 主机释放SDA线(此时外部器件可以拉低SDA线) */
    ct_iic_delay();                                     /* 执行语句 */
    CT_IIC_SCL(1);      /* SCL=1, 此时从机可以返回ACK */
    ct_iic_delay();                                     /* 执行语句 */

    while (CT_READ_SDA) /* 等待应答 */
    {                                                   /* 代码块开始 */
        waittime++;                                     /* 执行语句 */

        if (waittime > 250)                             /* 条件判断 */
        {                                               /* 代码块开始 */
            ct_iic_stop();                              /* 执行语句 */
            rack = 1;                                   /* 赋值或初始化 */
            break;                                      /* 跳出 */
        }                                               /* 代码块结束 */

        ct_iic_delay();                                 /* 执行语句 */
    }                                                   /* 代码块结束 */

    CT_IIC_SCL(0);      /* SCL=0, 结束ACK检查 */
    ct_iic_delay();                                     /* 执行语句 */
    return rack;                                        /* 返回值 */
}                                                       /* 代码块结束 */


/**
 * @brief       产生ACK应答
 * @param       无
 * @retval      无
 */
void ct_iic_ack(void)                                   /* 函数定义或声明 */
{                                                       /* 代码块开始 */
    CT_IIC_SDA(0);  /* SCL 0 -> 1  时SDA = 0,表示应答 */
    ct_iic_delay();                                     /* 执行语句 */
    CT_IIC_SCL(1);                                      /* 执行语句 */
    ct_iic_delay();                                     /* 执行语句 */
    CT_IIC_SCL(0);                                      /* 执行语句 */
    ct_iic_delay();                                     /* 执行语句 */
    CT_IIC_SDA(1);  /* 主机释放SDA线 */
    ct_iic_delay();                                     /* 执行语句 */
}                                                       /* 代码块结束 */

/**
 * @brief       不产生ACK应答
 * @param       无
 * @retval      无
 */
void ct_iic_nack(void)                                  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
    CT_IIC_SDA(1);  /* SCL 0 -> 1  时 SDA = 1,表示不应答 */
    ct_iic_delay();                                     /* 执行语句 */
    CT_IIC_SCL(1);                                      /* 执行语句 */
    ct_iic_delay();                                     /* 执行语句 */
    CT_IIC_SCL(0);                                      /* 执行语句 */
    ct_iic_delay();                                     /* 执行语句 */
}                                                       /* 代码块结束 */

/**
 * @brief       IIC发送一个字节
 * @param       data: 要发送的数据
 * @retval      无
 */
void ct_iic_send_byte(uint8_t data)                     /* 函数定义或声明 */
{                                                       /* 代码块开始 */
    uint8_t t;                                          /* 8位无符号变量/函数 */

    for (t = 0; t < 8; t++)                             /* for循环 */
    {                                                   /* 代码块开始 */
        CT_IIC_SDA((data & 0x80) >> 7); /* 高位先发送 */
        ct_iic_delay();                                 /* 执行语句 */
        CT_IIC_SCL(1);                                  /* 执行语句 */
        ct_iic_delay();                                 /* 执行语句 */
        CT_IIC_SCL(0);                                  /* 执行语句 */
        data <<= 1;     /* 左移1位,用于下一次发送 */
    }                                                   /* 代码块结束 */

    CT_IIC_SDA(1);      /* 发送完成, 主机释放SDA线 */
}                                                       /* 代码块结束 */

/**
 * @brief       IIC发送一个字节
 * @param       ack:  ack=1时，发送ack; ack=0时，发送nack
 * @retval      接收到的数据
 */
uint8_t ct_iic_read_byte(unsigned char ack)             /* 8位无符号变量/函数 */
{                                                       /* 代码块开始 */
    uint8_t i, receive = 0;                             /* 8位无符号变量/函数 */

    for (i = 0; i < 8; i++ )    /* 接收1个字节数据 */
    {                                                   /* 代码块开始 */
        receive <<= 1;          /* 高位先输出,所以先收到的数据位要左移 */
        CT_IIC_SCL(1);                                  /* 执行语句 */
        ct_iic_delay();                                 /* 执行语句 */

        if (CT_READ_SDA)                                /* 条件判断 */
        {                                               /* 代码块开始 */
            receive++;                                  /* 执行语句 */
        }                                               /* 代码块结束 */

        CT_IIC_SCL(0);                                  /* 执行语句 */
        ct_iic_delay();                                 /* 执行语句 */

    }                                                   /* 代码块结束 */

    if (!ack)                                           /* 条件判断 */
    {                                                   /* 代码块开始 */
        ct_iic_nack();  /* 发送nACK */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
        ct_iic_ack();   /* 发送ACK */
    }                                                   /* 代码块结束 */

    return receive;                                     /* 返回值 */
}                                                       /* 代码块结束 */







