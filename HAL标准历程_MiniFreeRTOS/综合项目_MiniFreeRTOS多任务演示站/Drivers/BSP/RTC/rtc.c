/* 文件: rtc.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
 ****************************************************************************************************
 * @file        rtc.c
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

#include "./BSP/RTC/rtc.h"                              /* 包含板级驱动头文件 */
#include "./BSP/LED/led.h"                              /* 包含板级驱动头文件 */
#include "./SYSTEM/usart/usart.h"                       /* 包含系统层头文件 */
#include "./SYSTEM/delay/delay.h"                       /* 包含系统层头文件 */


RTC_HandleTypeDef g_rtc_handle;  /* RTC句柄 */

/**
 * @brief       RTC写入后备区域SRAM
 * @param       bkrx : 后备区寄存器编号,范围:0~31
 *                     对应 RTC_BKP_DR0~RTC_BKP_DR31
 * @param       data : 要写入的数据,32位长度
 * @retval      无
 */
void rtc_write_bkr(uint32_t bkrx, uint32_t data)        /* 函数定义或声明 */
{                                                       /* 代码块开始 */
    HAL_PWR_EnableBkUpAccess();     /* 取消备份区写保护 */
    HAL_RTCEx_BKUPWrite(&g_rtc_handle, bkrx, data);     /* HAL库函数调用 */
}                                                       /* 代码块结束 */

/**
 * @brief       RTC读取后备区域SRAM
 * @param       bkrx : 后备区寄存器编号,范围:0~31
 * @retval      读取到的值
 */
uint32_t rtc_read_bkr(uint32_t bkrx)                    /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
    uint32_t temp = 0;                                  /* 32位无符号变量/函数 */
    temp = HAL_RTCEx_BKUPRead(&g_rtc_handle, bkrx);     /* 赋值或初始化 */
    return temp; /* 返回读取到的值 */
}                                                       /* 代码块结束 */

/**
 * @brief       RTC时间设置
 * @param       hour,min,sec: 小时,分钟,秒钟
 * @param       ampm        : AM/PM, 0=AM/24H; 1=PM/12H
 * @retval      0,成功
 *              其他,异常状态
 */
HAL_StatusTypeDef rtc_set_time(uint8_t hour, uint8_t min, uint8_t sec, uint8_t ampm)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
    RTC_TimeTypeDef rtc_time_handle;                    /* 执行语句 */

    rtc_time_handle.Hours = hour;                       /* 赋值或初始化 */
    rtc_time_handle.Minutes = min;                      /* 赋值或初始化 */
    rtc_time_handle.Seconds = sec;                      /* 赋值或初始化 */
    rtc_time_handle.TimeFormat = ampm;                  /* 赋值或初始化 */
    rtc_time_handle.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;  /* 赋值或初始化 */
    rtc_time_handle.StoreOperation = RTC_STOREOPERATION_RESET;  /* 赋值或初始化 */
    return HAL_RTC_SetTime(&g_rtc_handle, &rtc_time_handle, RTC_FORMAT_BIN);  /* 返回值 */
}                                                       /* 代码块结束 */

/**
 * @brief       RTC日期设置
 * @param       year,month,date : 年(0~99),月(1~12),日(0~31)
 * @param       week            : 星期(1~7,0,非法!)
 * @retval      0,成功
 *              其他,异常状态
 */
HAL_StatusTypeDef rtc_set_date(uint8_t year, uint8_t month, uint8_t date, uint8_t week)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
    RTC_DateTypeDef rtc_date_handle;                    /* 执行语句 */

    rtc_date_handle.Date = date;                        /* 赋值或初始化 */
    rtc_date_handle.Month = month;                      /* 赋值或初始化 */
    rtc_date_handle.WeekDay = week;                     /* 赋值或初始化 */
    rtc_date_handle.Year = year;                        /* 赋值或初始化 */
    return HAL_RTC_SetDate(&g_rtc_handle, &rtc_date_handle, RTC_FORMAT_BIN);  /* 返回值 */
}                                                       /* 代码块结束 */

/**
 * @brief       获取RTC时间
 * @param       *hour,*min,*sec : 小时,分钟,秒钟
 * @param       *ampm           : AM/PM,0=AM/24H,1=PM/12H.
 * @retval      无
 */
void rtc_get_time(uint8_t *hour, uint8_t *min, uint8_t *sec, uint8_t *ampm)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */

    RTC_TimeTypeDef rtc_time_handle;                    /* 执行语句 */

    HAL_RTC_GetTime(&g_rtc_handle, &rtc_time_handle, RTC_FORMAT_BIN);  /* HAL库函数调用 */

    *hour = rtc_time_handle.Hours;                      /* 赋值或初始化 */
    *min = rtc_time_handle.Minutes;                     /* 赋值或初始化 */
    *sec = rtc_time_handle.Seconds;                     /* 赋值或初始化 */
    *ampm = rtc_time_handle.TimeFormat;                 /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/**
 * @brief       获取RTC日期
 * @param       *year,*mon,*date: 年,月,日
 * @param       *week           : 星期
 * @retval      无
 */
void rtc_get_date(uint8_t *year, uint8_t *month, uint8_t *date, uint8_t *week)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
    RTC_DateTypeDef rtc_date_handle;                    /* 执行语句 */

    HAL_RTC_GetDate(&g_rtc_handle, &rtc_date_handle, RTC_FORMAT_BIN);  /* HAL库函数调用 */

    *year = rtc_date_handle.Year;                       /* 赋值或初始化 */
    *month = rtc_date_handle.Month;                     /* 赋值或初始化 */
    *date = rtc_date_handle.Date;                       /* 赋值或初始化 */
    *week = rtc_date_handle.WeekDay;                    /* 赋值或初始化 */
}                                                       /* 代码块结束 */

/* 月修正数据表 */
uint8_t const table_week[12] = {0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5};  /* 8位无符号变量/函数 */

/**
 * @breif       获得现在是星期几, 输入公历日期得到星期(只允许1901-2099年)
 * @param       year,month,day : 公历年月日
 * @retval      星期号(1~7,代表周1~周日)
 */
uint8_t rtc_get_week(uint16_t year, uint8_t month, uint8_t day)  /* 8位无符号变量/函数 */
{                                                       /* 代码块开始 */
    uint16_t temp;                                      /* 16位无符号变量/函数 */
    uint8_t yearH, yearL;                               /* 8位无符号变量/函数 */

    yearH = year / 100;                                 /* 赋值或初始化 */
    yearL = year % 100;                                 /* 赋值或初始化 */

    /*  如果为21世纪,年份数加100 */
    if (yearH > 19)                                     /* 条件判断 */
    {                                                   /* 代码块开始 */
        yearL += 100;                                   /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /*  所过闰年数只算1900年之后的 */
    temp = yearL + yearL / 4;                           /* 赋值或初始化 */
    temp = temp % 7;                                    /* 赋值或初始化 */
    temp = temp + day + table_week[month - 1];          /* 赋值或初始化 */

    if (yearL % 4 == 0 && month < 3)                    /* 条件判断 */
    {                                                   /* 代码块开始 */
        temp--;                                         /* 执行语句 */
    }                                                   /* 代码块结束 */

    temp %= 7;                                          /* 赋值或初始化 */

    if (temp == 0)                                      /* 条件判断 */
    {                                                   /* 代码块开始 */
        temp = 7;                                       /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    return temp;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
 * @brief       RTC初始化
 *   @note
 *              默认尝试使用LSE,当LSE启动失败后,切换为LSI.
 *              通过BKP寄存器0的值,可以判断RTC使用的是LSE/LSI:
 *              当BKP0==0x5050时,使用的是LSE
 *              当BKP0==0x5051时,
 *              注意:切换LSI/LSE将导致时间/日期丢失,切换后需重新设置.
 *
 * @param       无
 * @retval      0,成功
 *              1,进入初始化模式失败
 */
uint8_t rtc_init(void)                                  /* 8位无符号变量/函数 */
{                                                       /* 代码块开始 */
    uint16_t bkpflag = 0;                               /* 16位无符号变量/函数 */

    __HAL_RCC_RTC_ENABLE();     /* RTC使能 */
    HAL_PWR_EnableBkUpAccess(); /* 取消备份区域写保护 */
    __HAL_RCC_PWR_CLK_ENABLE(); /* 使能电源时钟PWR */

    g_rtc_handle.Instance = RTC;                        /* 赋值或初始化 */
    g_rtc_handle.Init.HourFormat = RTC_HOURFORMAT_24;   /* RTC设置为24小时格式 */
    g_rtc_handle.Init.AsynchPrediv = 0x7F;              /* RTC异步分频系数(1~0x7F) */
    g_rtc_handle.Init.SynchPrediv = 0xFF;               /* RTC同步分频系数(0~0x7FFF) */
    g_rtc_handle.Init.OutPut = RTC_OUTPUT_DISABLE;      /* 赋值或初始化 */
    g_rtc_handle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;  /* 赋值或初始化 */
    g_rtc_handle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;  /* 赋值或初始化 */

    /* 检查是不是第一次配置时钟 */
    bkpflag = rtc_read_bkr(0);                /* 读取BKP0的值 */

    if (HAL_RTC_Init(&g_rtc_handle) != HAL_OK)          /* 条件判断 */
    {                                                   /* 代码块开始 */
        return 1;                                       /* 返回值 */
    }                                                   /* 代码块结束 */

    if ((bkpflag != 0x5050) && (bkpflag != 0x5051))     /* 之前未初始化过, 重新配置 */
    {                                                   /* 代码块开始 */
        rtc_set_time(6, 59, 56, RTC_HOURFORMAT12_AM);   /* 设置时间, 根据实际时间修改 */
        rtc_set_date(20, 4, 22, 3);                     /* 设置日期 */
    }                                                   /* 代码块结束 */
    return 0;                                           /* 返回值 */
}                                                       /* 代码块结束 */

/**
 * @brief       RTC底层驱动, 时钟配置
 * @param       hrtc : RTC句柄
 * @note        此函数会被HAL_RTC_Init()调用
 * @retval      无
 */
void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc)           /* 函数定义或声明 */
{                                                       /* 代码块开始 */
    uint16_t retry = 200;                               /* 16位无符号变量/函数 */

    RCC_OscInitTypeDef rcc_osc_init_handle;             /* 时钟相关操作 */
    RCC_PeriphCLKInitTypeDef rcc_periphclk_init_handle;  /* 时钟相关操作 */

    __HAL_RCC_RTC_ENABLE();     /* RTC使能 */
    HAL_PWR_EnableBkUpAccess(); /* 取消备份区域写保护 */
    __HAL_RCC_PWR_CLK_ENABLE(); /* 使能电源时钟PWR */
    __HAL_RCC_RTC_ENABLE();     /* RTC使能 */

    /* 使用寄存器的方式去检测LSE是否可以正常工作 */
    RCC->BDCR |= 1 << 0;        /* 尝试开启LSE */

    while (retry && ((RCC->BDCR & 0x02) == 0))  /* 等待LSE准备好 */
    {                                                   /* 代码块开始 */
        retry--;                                        /* 执行语句 */
        delay_ms(5);                                    /* 毫秒延时 */
    }                                                   /* 代码块结束 */

    if (retry == 0)     /* LSE起振失败 使用LSI */
    {                                                   /* 代码块开始 */
        rcc_osc_init_handle.OscillatorType = RCC_OSCILLATORTYPE_LSI;  /* 选择要配置的振荡器 */
        rcc_osc_init_handle.LSIState = RCC_LSI_ON;                    /* LSI状态：开启 */
        rcc_osc_init_handle.PLL.PLLState = RCC_PLL_NONE;              /* PLL无配置 */
        HAL_RCC_OscConfig(&rcc_osc_init_handle);                      /* 配置设置的rcc_oscinitstruct */

        rcc_periphclk_init_handle.PeriphClockSelection = RCC_PERIPHCLK_RTC; /* 选择要配置外设 RTC */
        rcc_periphclk_init_handle.RTCClockSelection = RCC_RTCCLKSOURCE_LSI; /* RTC时钟源选择LSI */
        HAL_RCCEx_PeriphCLKConfig(&rcc_periphclk_init_handle);              /* 配置设置的rcc_periphclkinitstruct */
        rtc_write_bkr(0, 0x5051);                       /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
        rcc_osc_init_handle.OscillatorType = RCC_OSCILLATORTYPE_LSE;        /* 选择要配置的振荡器 */
        rcc_osc_init_handle.PLL.PLLState = RCC_PLL_NONE;                    /* PLL不配置 */
        rcc_osc_init_handle.LSEState = RCC_LSE_ON;                          /* LSE状态:开启 */
        HAL_RCC_OscConfig(&rcc_osc_init_handle);                            /* 配置设置的rcc_oscinitstruct */

        rcc_periphclk_init_handle.PeriphClockSelection = RCC_PERIPHCLK_RTC; /* 选择要配置外设RTC */
        rcc_periphclk_init_handle.RTCClockSelection = RCC_RTCCLKSOURCE_LSE; /* RTC时钟源选择LSE */
        HAL_RCCEx_PeriphCLKConfig(&rcc_periphclk_init_handle);              /* 配置设置的rcc_periphclkinitstruct */
        rtc_write_bkr(0, 0x5050);                       /* 执行语句 */
    }                                                   /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
 * @breif       设置闹钟时间(按星期闹铃,24小时制)
 * @param       week        : 星期几(1~7) 
 * @param       hour,min,sec: 小时,分钟,秒钟
 * @retval      无
 */
void rtc_set_alarma(uint8_t week, uint8_t hour, uint8_t min, uint8_t sec)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
    RTC_AlarmTypeDef rtc_alarm_handle;                  /* 执行语句 */

    rtc_alarm_handle.AlarmTime.Hours = hour;                                /* 小时 */
    rtc_alarm_handle.AlarmTime.Minutes = min;                               /* 分钟 */
    rtc_alarm_handle.AlarmTime.Seconds = sec;                               /* 秒 */
    rtc_alarm_handle.AlarmTime.SubSeconds = 0;          /* 赋值或初始化 */
    rtc_alarm_handle.AlarmTime.TimeFormat = RTC_HOURFORMAT12_AM;  /* 赋值或初始化 */

    rtc_alarm_handle.AlarmMask = RTC_ALARMMASK_NONE;                        /* 精确匹配星期, 时分秒 */
    rtc_alarm_handle.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_NONE;  /* 赋值或初始化 */
    rtc_alarm_handle.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_WEEKDAY; /* 按星期 */
    rtc_alarm_handle.AlarmDateWeekDay = week;                               /* 星期 */
    rtc_alarm_handle.Alarm = RTC_ALARM_A;                                   /* 闹钟A */
    HAL_RTC_SetAlarm_IT(&g_rtc_handle, &rtc_alarm_handle, RTC_FORMAT_BIN);  /* HAL库函数调用 */

    HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 1, 2);   /* 抢占优先级1,子优先级2 */
    HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);                 /* HAL 中断配置 */
}                                                       /* 代码块结束 */

/**
 * @breif       周期性唤醒定时器设置
 * @param       wksel
 *   @arg       RTC_WAKEUPCLOCK_RTCCLK_DIV16        ((uint32_t)0x00000000)
 *   @arg       RTC_WAKEUPCLOCK_RTCCLK_DIV8         ((uint32_t)0x00000001)
 *   @arg       RTC_WAKEUPCLOCK_RTCCLK_DIV4         ((uint32_t)0x00000002)
 *   @arg       RTC_WAKEUPCLOCK_RTCCLK_DIV2         ((uint32_t)0x00000003)
 *   @arg       RTC_WAKEUPCLOCK_CK_SPRE_16BITS      ((uint32_t)0x00000004)
 *   @arg       RTC_WAKEUPCLOCK_CK_SPRE_17BITS      ((uint32_t)0x00000006)
 * @note        000,RTC/16;001,RTC/8;010,RTC/4;011,RTC/2;
 * @note        注意:RTC就是RTC的时钟频率,即RTCCLK!
 * @param       cnt: 自动重装载值.减到0,产生中断.
 * @retval      无
 */
void rtc_set_wakeup(uint8_t wksel, uint16_t cnt)        /* 函数定义或声明 */
{                                                       /* 代码块开始 */
    __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&g_rtc_handle, RTC_FLAG_WUTF);  /* 清除RTC WAKE UP的标志 */

    HAL_RTCEx_SetWakeUpTimer_IT(&g_rtc_handle, cnt, wksel);          /* 设置重装载值和时钟 */

    HAL_NVIC_SetPriority(RTC_WKUP_IRQn, 2, 2);                       /* 抢占优先级2, 子优先级2 */
    HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);                  /* HAL 中断配置 */
}                                                       /* 代码块结束 */

/**
 * @breif       RTC闹钟中断服务函数
 * @param       无
 * @retval      无
 */
void RTC_Alarm_IRQHandler(void)                         /* 中断服务函数 */
{                                                       /* 代码块开始 */
    HAL_RTC_AlarmIRQHandler(&g_rtc_handle);             /* HAL库函数调用 */
}                                                       /* 代码块结束 */

/**
 * @breif       RTC闹钟A中断处理回调函数
 * @param       hrtc : RTC句柄
 * @retval      无
 */
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
    printf("ALARM A!\r\n");                             /* 串口打印 */
}                                                       /* 代码块结束 */

/**
 * @breif       RTC WAKE UP中断服务函数
 * @param       无
 * @retval      无
 */
void RTC_WKUP_IRQHandler(void)                          /* 中断服务函数 */
{                                                       /* 代码块开始 */
    HAL_RTCEx_WakeUpTimerIRQHandler(&g_rtc_handle);     /* HAL库函数调用 */
}                                                       /* 代码块结束 */

/**
 * @breif       RTC WAKE UP中断处理处理回调函数
 * @param       hrtc : RTC句柄
 * @retval      无
 */
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)  /* 函数定义或声明 */
{                                                       /* 代码块开始 */
    LED1_TOGGLE();                                      /* 翻转绿灯 */
}                                                       /* 代码块结束 */







