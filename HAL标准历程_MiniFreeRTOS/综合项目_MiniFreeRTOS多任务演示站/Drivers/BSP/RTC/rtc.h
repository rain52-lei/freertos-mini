/* 文件: rtc.h | 编码: UTF-8 | 每行代码附中文注释 */
/**
 ****************************************************************************************************
 * @file        rtc.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-16
 * @brief       RTC 驱动代码
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
 * V1.0 20211016
 * 第一次发布
 *
 ****************************************************************************************************
 */

#ifndef __RTC_H                                         /* 防止头文件重复包含 */
#define __RTC_H                                         /* 定义包含保护宏 */

#include "./SYSTEM/sys/sys.h"                           /* 包含系统层头文件 */


extern RTC_HandleTypeDef g_rtc_handle;                  /* 外部符号声明 */

uint8_t rtc_init(void);                             /* 初始化RTC */
uint32_t rtc_read_bkr(uint32_t bkrx);               /* 读后备寄存器 */
void rtc_write_bkr(uint32_t bkrx, uint32_t data);   /* 写后备寄存器 */
void rtc_get_time(uint8_t *hour, uint8_t *min, uint8_t *sec, uint8_t *ampm);            /* 获取时间 */
HAL_StatusTypeDef rtc_set_time(uint8_t hour, uint8_t min, uint8_t sec, uint8_t ampm);   /* 设置时间 */
void rtc_get_date(uint8_t *year, uint8_t *month, uint8_t *date, uint8_t *week);         /* 获取日期 */
HAL_StatusTypeDef rtc_set_date(uint8_t year, uint8_t month, uint8_t date, uint8_t week);/* 设置日期 */

void rtc_set_wakeup(uint8_t wksel, uint16_t cnt);                                       /* 设置周期性唤醒 */
uint8_t rtc_get_week(uint16_t year, uint8_t month, uint8_t day);                        /* 获取星期 */
void rtc_set_alarma(uint8_t week, uint8_t hour, uint8_t min, uint8_t sec);              /* 设置闹钟 */

#endif                                                  /* 条件编译结束 */
