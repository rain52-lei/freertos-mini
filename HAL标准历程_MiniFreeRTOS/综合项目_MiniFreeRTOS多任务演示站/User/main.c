/* 文件: main.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
 * MiniFreeRTOS 综合项目 —— 手机式多应用演示站 (Phone OS Demo)
 *
 * 整合历程能力:
 *  实验01 跑马灯      → LED 应用 / 状态栏指示灯
 *  实验02 蜂鸣器      → 通知音 / BEEP 应用
 *  实验03 按键输入    → 实体键 Home/Back/通知
 *  实验05 串口通信    → 聊天/调试命令 + 状态同步
 *  实验13 TFTLCD      → 手机主屏与各 App 界面
 *  实验15 RTC         → 时钟/状态栏时间
 *  实验24 IIC+24CXX   → 触摸校准参数存储(驱动层)
 *  实验28 触摸屏      → 触控点击 App / 画板 / 计算器
 *  MiniFreeRTOS       → 多任务 + 队列 + 信号量 + 互斥量
 *
 * 平台: 正点原子 探索者 F407 | HAL + MiniFreeRTOS
 * 串口: USART1 115200 (板载 USB 串口 P10)
 * 打开: Projects\MDK-ARM\atk_f407.uvprojx
 */
#include <stdio.h>                                      /* 标准输入输出(printf) */
#include <string.h>                                     /* 字符串处理 */
#include <stdlib.h>                                     /* 标准库 */

#include "./SYSTEM/sys/sys.h"                           /* 系统底层 */
#include "./SYSTEM/usart/usart.h"                       /* 串口与 printf 重定向 */
#include "./SYSTEM/delay/delay.h"                       /* 延时 */
#include "./BSP/LED/led.h"                              /* LED 驱动 */
#include "./BSP/BEEP/beep.h"                            /* 蜂鸣器驱动 */
#include "./BSP/KEY/key.h"                              /* 按键驱动 */
#include "./BSP/LCD/lcd.h"                              /* TFTLCD 驱动 */
#include "./BSP/TOUCH/touch.h"                          /* 触摸屏驱动 */
#include "./BSP/RTC/rtc.h"                              /* RTC 实时时钟 */
#include "rtos.h"                                       /* MiniFreeRTOS */
#include "ui_text.h"                                    /* UTF-8 中文 UI 文字 */

/* ======================== 颜色主题(现代手机 UI, RGB565) ======================== */
#define UI_BG           0x10A2                           /* 深空蓝黑 #102040 */
#define UI_BG2          0x18C3                           /* 渐变上层 */
#define UI_CARD         0x2126                           /* 卡片深灰蓝 */
#define UI_CARD2        0x31A8                           /* 卡片高亮 */
#define UI_ACCENT       0x3C9F                           /* 活力蓝 */
#define UI_ACCENT2      0x07FF                           /* 青色点缀 */
#define UI_TEXT         0xFFFF                           /* 主文字 */
#define UI_SUB          0x9CF3                           /* 次文字灰蓝 */
#define UI_MUTED        0x6B4D                           /* 更淡 */
#define UI_BAR          0x0C41                           /* 状态栏/底栏 */
#define UI_DANGER       0xF9A6                           /* 柔和红 */
#define UI_OK           0x2E8A                           /* 柔和绿 */
#define UI_WARN         0xFE60                           /* 琥珀黄 */
#define UI_ORANGE       0xFD20                           /* 橙 */
#define UI_PURPLE       0xA19F                           /* 紫 */
#define UI_PINK         0xF81F                           /* 品红 */
#define UI_TEAL         0x07F5                           /* 青绿 */
#define UI_LINE         0x39C7                           /* 分割线 */
#define UI_SHADOW       0x0841                           /* 阴影 */
#define UI_SB_H         28                               /* 状态栏高度 */
#define UI_NAV_H        40                               /* 导航栏高度 */
#define UI_CONTENT_Y    (UI_SB_H + UI_NAV_H)             /* 内容区起点 */
#define UI_DOCK_H       26                               /* 桌面底栏 */
#define UI_PAD          12                               /* 通用边距 */

/* ======================== 页面 / 事件 ======================== */
typedef enum                                            /* 页面枚举: 类似手机 App */
{                                                       /* 枚举开始 */
    PAGE_HOME = 0,                                      /* 桌面 */
    PAGE_LED,                                           /* LED 控制 App */
    PAGE_BEEP,                                          /* 蜂鸣器 App */
    PAGE_CLOCK,                                         /* 时钟 App */
    PAGE_PAINT,                                         /* 画板 App */
    PAGE_SETTINGS,                                      /* 设置 App */
    PAGE_CALC,                                          /* 计算器 App */
    PAGE_INFO,                                          /* 系统信息 App */
    PAGE_CHAT,                                          /* 串口聊天 App */
    PAGE_ALARM,                                         /* 闹钟/模式 App */
    PAGE_MAX                                            /* 页面数量上限 */
} page_id_t;                                            /* 枚举结束 */

typedef enum                                            /* 系统事件类型 */
{                                                       /* 枚举开始 */
    EVT_NONE = 0,                                       /* 空事件 */
    EVT_TOUCH_DOWN,                                     /* 触摸按下(带坐标) */
    EVT_KEY,                                            /* 实体按键 */
    EVT_UART_LINE,                                      /* 串口收到一行 */
    EVT_TICK_1S,                                        /* 1 秒节拍 */
    EVT_GOTO_PAGE,                                      /* 跳转页面 */
    EVT_NOTIFY                                          /* 通知(短鸣+提示) */
} evt_type_t;                                           /* 枚举结束 */

typedef struct                                          /* 事件消息结构 */
{                                                       /* 结构开始 */
    evt_type_t type;                                    /* 事件类型 */
    uint16_t x;                                         /* 触摸 X 或附加参数 */
    uint16_t y;                                         /* 触摸 Y 或附加参数 */
    uint16_t v;                                         /* 通用值(键值/页码等) */
} evt_msg_t;                                            /* 结构结束 */

typedef struct                                          /* 全局手机状态 */
{                                                       /* 结构开始 */
    volatile page_id_t page;                            /* 当前页面 */
    volatile page_id_t page_prev;                       /* 上一页面(返回用) */
    volatile uint8_t dirty;                             /* 界面需要整页重绘 */
    volatile uint8_t led0_on;                           /* LED0 逻辑点亮 */
    volatile uint8_t led1_on;                           /* LED1 逻辑点亮 */
    volatile uint8_t led0_auto;                         /* LED0 自动闪 */
    volatile uint8_t led1_auto;                         /* LED1 自动闪 */
    volatile uint8_t beep_hold;                         /* 蜂鸣保持 */
    volatile uint8_t mode;                              /* 0正常 1省电 2勿扰 3告警 */
    volatile uint8_t battery;                           /* 模拟电量 0~100 */
    volatile uint8_t signal;                            /* 模拟信号格 0~4 */
    volatile uint32_t uptime;                           /* 开机秒数 */
    volatile uint32_t touch_cnt;                        /* 触摸次数 */
    volatile uint32_t key_cnt;                          /* 按键次数 */
    volatile uint32_t cmd_cnt;                          /* 命令次数 */
    volatile uint8_t rtc_ok;                            /* RTC 是否可用 */
    volatile uint8_t alarm_en;                          /* 软件闹钟使能 */
    volatile uint8_t alarm_min;                         /* 闹钟分钟匹配 */
    volatile uint8_t notify_flag;                       /* 通知闪一下状态栏 */
    char chat_line[48];                                 /* 最近一条聊天/串口 */
    char status_tip[32];                                /* 状态栏提示 */
    /* 软时钟备份(RTC 失败时用) */
    volatile uint8_t sh, sm, ss;                        /* 时分秒 */
    /* 计算器 */
    volatile int32_t calc_a;                            /* 操作数 A */
    volatile int32_t calc_b;                            /* 操作数 B */
    volatile int8_t  calc_op;                           /* 0无 1+ 2- 3* 4/ */
    volatile uint8_t calc_new;                          /* 下一次输入覆盖 */
    char calc_disp[16];                                 /* 显示缓冲 */
} phone_t;                                              /* 结构结束 */

static phone_t g_phone;                                 /* 全局手机状态实例 */

/* RTOS 对象 */
static Queue_t g_q;                                     /* 事件队列 */
static evt_msg_t g_qbuf[24];                            /* 队列存储区 */
static Semaphore_t g_sem_beep;                          /* 短鸣信号量 */
static Mutex_t g_mtx_print;                             /* 串口打印互斥 */
static Mutex_t g_mtx_lcd;                               /* LCD 访问互斥(多任务保护) */

/* 任务栈与 TCB (栈单位: 字) */
static uint32_t g_stk_idle[256];                        /* 空闲任务栈 */
static uint32_t g_stk_ui[2048];                         /* UI 任务栈(最大) */
static uint32_t g_stk_in[768];                          /* 输入任务栈 */
static uint32_t g_stk_uart[1024];                       /* 串口任务栈 */
static uint32_t g_stk_led[512];                         /* LED 任务栈 */
static uint32_t g_stk_beep[384];                        /* 蜂鸣任务栈 */
static uint32_t g_stk_mon[768];                         /* 监控任务栈 */
static tcb_t g_t_idle;                                  /* 空闲 TCB */
static tcb_t g_t_ui;                                    /* UI TCB */
static tcb_t g_t_in;                                    /* 输入 TCB */
static tcb_t g_t_uart;                                  /* 串口 TCB */
static tcb_t g_t_led;                                   /* LED TCB */
static tcb_t g_t_beep;                                  /* 蜂鸣 TCB */
static tcb_t g_t_mon;                                   /* 监控 TCB */

/* 桌面 App 图标表 */
typedef struct                                          /* 桌面图标描述 */
{                                                       /* 结构开始 */
    const char *name;                                   /* 英文名(ASCII 字库) */
    page_id_t page;                                     /* 对应页面 */
    uint16_t color;                                     /* 图标颜色 */
} app_icon_t;                                           /* 结构结束 */

static const app_icon_t g_apps[] =                      /* 9 宫格应用(中文名) */
{                                                       /* 数组开始 */
    {"灯控", PAGE_LED,      UI_DANGER},
    {"蜂鸣", PAGE_BEEP,     UI_WARN},
    {"时钟", PAGE_CLOCK,    UI_ACCENT},
    {"画板", PAGE_PAINT,    UI_PINK},
    {"设置", PAGE_SETTINGS, UI_TEAL},
    {"计算", PAGE_CALC,     UI_OK},
    {"信息", PAGE_INFO,     UI_PURPLE},
    {"聊天", PAGE_CHAT,     UI_ACCENT2},
    {"闹钟", PAGE_ALARM,    UI_ORANGE},
};                                                      /* 数组结束 */

#define APP_COUNT  (sizeof(g_apps)/sizeof(g_apps[0]))   /* 应用个数 */

/* ======================== 工具函数 ======================== */
static void post_evt(evt_type_t t, uint16_t x, uint16_t y, uint16_t v)  /* 投递事件到队列 */
{                                                       /* 函数体开始 */
    evt_msg_t m;                                        /* 临时消息 */
    m.type = t;                                         /* 填类型 */
    m.x = x;                                            /* 填 X */
    m.y = y;                                            /* 填 Y */
    m.v = v;                                            /* 填附加值 */
    (void)xQueueSend(&g_q, &m, 10);                     /* 非无限等待发送 */
}                                                       /* 函数体结束 */

static void safe_printf(const char *s)                  /* 带互斥的字符串打印 */
{                                                       /* 函数体开始 */
    if (xMutexTake(&g_mtx_print, 100) == 1)             /* 获取打印锁 */
    {                                                   /* if 开始 */
        printf("%s", s);                                /* 输出字符串 */
        xMutexGive(&g_mtx_print);                       /* 释放打印锁 */
    }                                                   /* if 结束 */
}                                                       /* 函数体结束 */

static void lcd_lock(void)                              /* 获取 LCD 锁 */
{                                                       /* 函数体开始 */
    (void)xMutexTake(&g_mtx_lcd, portMAX_DELAY);        /* 无限等待拿锁 */
}                                                       /* 函数体结束 */

static void lcd_unlock(void)                            /* 释放 LCD 锁 */
{                                                       /* 函数体开始 */
    (void)xMutexGive(&g_mtx_lcd);                       /* 释放 */
}                                                       /* 函数体结束 */

static uint8_t hit_rect(uint16_t x, uint16_t y, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)  /* 点是否在矩形内 */
{                                                       /* 函数体开始 */
    if (x >= x1 && x <= x2 && y >= y1 && y <= y2)       /* 范围判断 */
    {                                                   /* if 开始 */
        return 1;                                       /* 命中 */
    }                                                   /* if 结束 */
    return 0;                                           /* 未命中 */
}                                                       /* 函数体结束 */

/* ---------- 基础绘制组件 ---------- */
static void draw_round_rect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t fill)
{
    uint16_t r = 4;
    if (x2 <= x1 + 2 * r || y2 <= y1 + 2 * r)
    {
        lcd_fill(x1, y1, x2, y2, fill);
        return;
    }
    /* 主体 + 四边条模拟圆角矩形 */
    lcd_fill((uint16_t)(x1 + r), y1, (uint16_t)(x2 - r), y2, fill);
    lcd_fill(x1, (uint16_t)(y1 + r), x2, (uint16_t)(y2 - r), fill);
    lcd_fill_circle((uint16_t)(x1 + r), (uint16_t)(y1 + r), r, fill);
    lcd_fill_circle((uint16_t)(x2 - r), (uint16_t)(y1 + r), r, fill);
    lcd_fill_circle((uint16_t)(x1 + r), (uint16_t)(y2 - r), r, fill);
    lcd_fill_circle((uint16_t)(x2 - r), (uint16_t)(y2 - r), r, fill);
}

static void draw_soft_btn(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                          uint16_t fill, const char *txt, uint16_t tc)
{
    uint16_t mid_y;
    /* 轻微阴影 */
    if (y2 + 2 < lcddev.height && x2 + 1 < lcddev.width)
    {
        lcd_fill((uint16_t)(x1 + 2), (uint16_t)(y2 + 1), (uint16_t)(x2 + 1), (uint16_t)(y2 + 2), UI_SHADOW);
    }
    draw_round_rect(x1, y1, x2, y2, fill);
    lcd_draw_hline((uint16_t)(x1 + 4), (uint16_t)(y1 + 1), (uint16_t)(x2 - x1 - 8), UI_CARD2);
    mid_y = (uint16_t)(y1 + ((y2 - y1) > 18 ? ((y2 - y1 - 16) / 2) : 2));
    ui_show_str_mid(x1, mid_y, (uint16_t)(x2 - x1), txt, 16, tc);
}

/* 兼容旧名 */
static void draw_btn(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t fill, const char *txt, uint16_t tc)
{
    draw_soft_btn(x1, y1, x2, y2, fill, txt, tc);
}

static void draw_signal_bars(uint16_t x, uint16_t y, uint8_t level)
{
    uint8_t i;
    uint16_t h0[4] = {4, 7, 10, 13};
    for (i = 0; i < 4; i++)
    {
        uint16_t bh = h0[i];
        uint16_t by = (uint16_t)(y + 13 - bh);
        uint16_t col = (i < level) ? UI_OK : UI_MUTED;
        lcd_fill((uint16_t)(x + i * 5), by, (uint16_t)(x + i * 5 + 3), (uint16_t)(y + 13), col);
    }
}

static void draw_battery_icon(uint16_t x, uint16_t y, uint8_t pct)
{
    uint16_t bw = 22;
    uint16_t bh = 12;
    uint16_t fill_w;
    uint16_t col = (pct > 20) ? UI_OK : UI_DANGER;
    if (pct > 100) pct = 100;
    lcd_draw_rectangle(x, y, (uint16_t)(x + bw), (uint16_t)(y + bh), UI_TEXT);
    lcd_fill((uint16_t)(x + bw + 1), (uint16_t)(y + 3), (uint16_t)(x + bw + 3), (uint16_t)(y + bh - 3), UI_TEXT);
    fill_w = (uint16_t)((bw - 3) * pct / 100);
    if (fill_w > 0)
    {
        lcd_fill((uint16_t)(x + 2), (uint16_t)(y + 2), (uint16_t)(x + 1 + fill_w), (uint16_t)(y + bh - 2), col);
    }
}

static void draw_app_glyph(uint16_t cx, uint16_t cy, page_id_t page, uint16_t color)
{
    /* 在图标中心画简洁符号 */
    switch (page)
    {
    case PAGE_LED:
        lcd_fill_circle(cx, cy, 6, color);
        lcd_draw_line(cx, (uint16_t)(cy - 10), cx, (uint16_t)(cy - 6), color);
        lcd_draw_line((uint16_t)(cx - 7), (uint16_t)(cy - 7), (uint16_t)(cx - 4), (uint16_t)(cy - 4), color);
        lcd_draw_line((uint16_t)(cx + 7), (uint16_t)(cy - 7), (uint16_t)(cx + 4), (uint16_t)(cy - 4), color);
        break;
    case PAGE_BEEP:
        lcd_fill((uint16_t)(cx - 6), (uint16_t)(cy - 3), (uint16_t)(cx - 1), (uint16_t)(cy + 3), color);
        lcd_draw_line((uint16_t)(cx - 1), (uint16_t)(cy - 3), (uint16_t)(cx + 5), (uint16_t)(cy - 8), color);
        lcd_draw_line((uint16_t)(cx - 1), (uint16_t)(cy + 3), (uint16_t)(cx + 5), (uint16_t)(cy + 8), color);
        lcd_draw_line((uint16_t)(cx + 5), (uint16_t)(cy - 8), (uint16_t)(cx + 5), (uint16_t)(cy + 8), color);
        break;
    case PAGE_CLOCK:
        lcd_draw_circle(cx, cy, 9, color);
        lcd_draw_line(cx, cy, cx, (uint16_t)(cy - 6), color);
        lcd_draw_line(cx, cy, (uint16_t)(cx + 5), cy, color);
        break;
    case PAGE_PAINT:
        lcd_draw_line((uint16_t)(cx - 6), (uint16_t)(cy + 6), (uint16_t)(cx + 4), (uint16_t)(cy - 6), color);
        lcd_fill_circle((uint16_t)(cx + 5), (uint16_t)(cy - 7), 2, color);
        break;
    case PAGE_SETTINGS:
        lcd_draw_circle(cx, cy, 5, color);
        lcd_draw_circle(cx, cy, 9, color);
        break;
    case PAGE_CALC:
        lcd_draw_rectangle((uint16_t)(cx - 8), (uint16_t)(cy - 8), (uint16_t)(cx + 8), (uint16_t)(cy + 8), color);
        lcd_draw_hline((uint16_t)(cx - 5), cy, 10, color);
        lcd_draw_line(cx, (uint16_t)(cy - 5), cx, (uint16_t)(cy + 5), color);
        break;
    case PAGE_INFO:
        lcd_draw_circle(cx, cy, 8, color);
        lcd_fill((uint16_t)(cx - 1), (uint16_t)(cy - 3), (uint16_t)(cx + 1), (uint16_t)(cy + 5), color);
        lcd_fill_circle(cx, (uint16_t)(cy - 6), 1, color);
        break;
    case PAGE_CHAT:
        draw_round_rect((uint16_t)(cx - 9), (uint16_t)(cy - 7), (uint16_t)(cx + 9), (uint16_t)(cy + 4), color);
        lcd_draw_line((uint16_t)(cx - 3), (uint16_t)(cy + 4), (uint16_t)(cx - 6), (uint16_t)(cy + 9), color);
        break;
    case PAGE_ALARM:
        lcd_draw_circle(cx, (uint16_t)(cy + 1), 7, color);
        lcd_draw_line((uint16_t)(cx - 8), (uint16_t)(cy - 6), (uint16_t)(cx - 4), (uint16_t)(cy - 2), color);
        lcd_draw_line((uint16_t)(cx + 8), (uint16_t)(cy - 6), (uint16_t)(cx + 4), (uint16_t)(cy - 2), color);
        break;
    default:
        break;
    }
}

static void ui_page_bg(void)
{
    uint16_t w = lcddev.width;
    uint16_t h = lcddev.height;
    lcd_fill(0, UI_SB_H, (uint16_t)(w - 1), (uint16_t)(h - 1), UI_BG);
    /* 顶部轻微渐变带 */
    lcd_fill(0, UI_SB_H, (uint16_t)(w - 1), (uint16_t)(UI_SB_H + 18), UI_BG2);
}

static void phone_get_hms(uint8_t *h, uint8_t *m, uint8_t *s)  /* 取时分秒(优先 RTC) */
{                                                       /* 函数体开始 */
    uint8_t ampm = 0;                                   /* 上午/下午占位 */
    if (g_phone.rtc_ok)                                 /* RTC 可用 */
    {                                                   /* if 开始 */
        rtc_get_time(h, m, s, &ampm);                   /* 读硬件 RTC */
    }                                                   /* if 结束 */
    else                                                /* 否则用软时钟 */
    {                                                   /* else 开始 */
        *h = g_phone.sh;                                /* 软时 */
        *m = g_phone.sm;                                /* 软分 */
        *s = g_phone.ss;                                /* 软秒 */
    }                                                   /* else 结束 */
}                                                       /* 函数体结束 */

static void phone_soft_tick(void)                       /* 软时钟走秒 + 电量模拟 */
{                                                       /* 函数体开始 */
    g_phone.ss++;                                       /* 秒++ */
    if (g_phone.ss >= 60)                               /* 进位分 */
    {                                                   /* if 开始 */
        g_phone.ss = 0;                                 /* 清秒 */
        g_phone.sm++;                                   /* 分++ */
        if (g_phone.sm >= 60)                           /* 进位时 */
        {                                               /* if 开始 */
            g_phone.sm = 0;                             /* 清分 */
            g_phone.sh++;                               /* 时++ */
            if (g_phone.sh >= 24)                       /* 跨天 */
            {                                           /* if 开始 */
                g_phone.sh = 0;                         /* 清时 */
            }                                           /* if 结束 */
        }                                               /* if 结束 */
    }                                                   /* if 结束 */
    if ((g_phone.uptime % 30u) == 0u && g_phone.battery > 5)  /* 每 30s 掉一点假电量 */
    {                                                   /* if 开始 */
        g_phone.battery--;                              /* 电量减 1 */
    }                                                   /* if 结束 */
    if (g_phone.alarm_en)                               /* 闹钟开启时检查 */
    {                                                   /* if 开始 */
        uint8_t h, m, s;                                /* 临时 */
        phone_get_hms(&h, &m, &s);                      /* 取时间 */
        if (m == g_phone.alarm_min && s == 0)           /* 整分触发一次 */
        {                                               /* if 开始 */
            post_evt(EVT_NOTIFY, 0, 0, 1);              /* 发通知 */
            strcpy((char *)g_phone.status_tip, "ALARM!");  /* 状态栏提示 */
            g_phone.notify_flag = 1;                    /* 标记通知 */
        }                                               /* if 结束 */
    }                                                   /* if 结束 */
}                                                       /* 函数体结束 */

/* ======================== 各页面绘制 ======================== */
static void draw_status_bar(void)
{
    char buf[16];
    uint8_t h, m, s;
    uint16_t w = lcddev.width;
    lcd_fill(0, 0, (uint16_t)(w - 1), (uint16_t)(UI_SB_H - 1), UI_BAR);
    lcd_draw_hline(0, (uint16_t)(UI_SB_H - 1), w, UI_LINE);
    phone_get_hms(&h, &m, &s);
    sprintf(buf, "%02d:%02d", h, m);
    lcd_show_string(8, 7, 48, 16, 16, buf, UI_TEXT);
    draw_signal_bars((uint16_t)(w / 2 - 18), 6, g_phone.signal);
    if (g_phone.notify_flag)
    {
        lcd_fill_circle((uint16_t)(w - 48), 13, 3, UI_DANGER);
    }
    draw_battery_icon((uint16_t)(w - 34), 8, g_phone.battery);
}

static void draw_nav_bar(const char *title)
{
    uint16_t w = lcddev.width;
    uint16_t y1 = UI_SB_H;
    uint16_t y2 = (uint16_t)(UI_CONTENT_Y - 1);
    lcd_fill(0, y1, (uint16_t)(w - 1), y2, UI_CARD);
    /* 返回胶囊按钮 */
    draw_round_rect(8, (uint16_t)(y1 + 8), 72, (uint16_t)(y2 - 6), UI_BG2);
    ui_show_str(18, (uint16_t)(y1 + 12), 56, 16, "返回", 16, 1, UI_ACCENT2);
    ui_show_str(84, (uint16_t)(y1 + 12), (uint16_t)(w - 96), 16, title, 16, 1, UI_TEXT);
    lcd_draw_hline(0, y2, w, UI_LINE);
}

/* 桌面网格几何(绘制与命中共用) */
static void home_grid_geom(uint16_t *top, uint16_t *cell_w, uint16_t *cell_h, uint16_t *icon)
{
    uint16_t w = lcddev.width;
    uint16_t h = lcddev.height;
    uint16_t bottom = (uint16_t)(h - UI_DOCK_H - 4);
    *top = (uint16_t)(UI_SB_H + 44);
    *cell_w = (uint16_t)((w - 16) / 3);
    *cell_h = (uint16_t)((bottom - *top) / 3);
    *icon = (uint16_t)((*cell_w < *cell_h ? *cell_w : *cell_h) - 22);
    if (*icon < 40) *icon = 40;
    if (*icon > 64) *icon = 64;
}

static void home_app_rect(uint16_t i, uint16_t *ix1, uint16_t *iy1, uint16_t *ix2, uint16_t *iy2,
                          uint16_t *tx, uint16_t *ty)
{
    uint16_t top, cell_w, cell_h, icon, r, c, cx, cy;
    home_grid_geom(&top, &cell_w, &cell_h, &icon);
    r = (uint16_t)(i / 3);
    c = (uint16_t)(i % 3);
    cx = (uint16_t)(8 + c * cell_w + cell_w / 2);
    cy = (uint16_t)(top + r * cell_h + icon / 2 + 2);
    *ix1 = (uint16_t)(cx - icon / 2);
    *iy1 = (uint16_t)(cy - icon / 2);
    *ix2 = (uint16_t)(cx + icon / 2);
    *iy2 = (uint16_t)(cy + icon / 2);
    *tx = (uint16_t)(cx - 20);
    *ty = (uint16_t)(*iy2 + 4);
}

static void draw_home(void)
{
    char gre[32];
    uint8_t hh, mm, ss;
    uint16_t w = lcddev.width;
    uint16_t h = lcddev.height;
    uint16_t i, ix1, iy1, ix2, iy2, tx, ty;
    uint16_t dock_y = (uint16_t)(h - UI_DOCK_H);

    lcd_fill(0, UI_SB_H, (uint16_t)(w - 1), (uint16_t)(h - 1), UI_BG);
    /* 桌面顶部装饰带 */
    lcd_fill(0, UI_SB_H, (uint16_t)(w - 1), (uint16_t)(UI_SB_H + 40), UI_BG2);
    draw_status_bar();
    phone_get_hms(&hh, &mm, &ss);
    if (hh < 12) strcpy(gre, "早上好");
    else if (hh < 18) strcpy(gre, "下午好");
    else strcpy(gre, "晚上好");
    ui_show_str(12, (uint16_t)(UI_SB_H + 8), 160, 16, gre, 16, 1, UI_SUB);
    ui_show_str(12, (uint16_t)(UI_SB_H + 24), 180, 16, "迷你手机系统", 16, 1, UI_TEXT);

    for (i = 0; i < APP_COUNT; i++)
    {
        home_app_rect(i, &ix1, &iy1, &ix2, &iy2, &tx, &ty);
        draw_round_rect((uint16_t)(ix1 + 2), (uint16_t)(iy1 + 3), (uint16_t)(ix2 + 2), (uint16_t)(iy2 + 3), UI_SHADOW);
        draw_round_rect(ix1, iy1, ix2, iy2, g_apps[i].color);
        draw_round_rect((uint16_t)(ix1 + 6), (uint16_t)(iy1 + 6),
                        (uint16_t)(ix2 - 6), (uint16_t)(iy2 - 6), UI_BG);
        draw_app_glyph((uint16_t)((ix1 + ix2) / 2), (uint16_t)((iy1 + iy2) / 2),
                       g_apps[i].page, g_apps[i].color);
        /* 名称居中在图标下方 */
        ui_show_str_mid((uint16_t)(ix1 - 4), ty, (uint16_t)(ix2 - ix1 + 8), g_apps[i].name, 16, UI_TEXT);
    }

    lcd_fill(0, dock_y, (uint16_t)(w - 1), (uint16_t)(h - 1), UI_BAR);
    lcd_draw_hline(0, dock_y, w, UI_LINE);
    draw_round_rect((uint16_t)(w / 2 - 28), (uint16_t)(dock_y + 8),
                    (uint16_t)(w / 2 + 28), (uint16_t)(dock_y + 14), UI_MUTED);
    ui_show_str(6, (uint16_t)(dock_y + 5), 120, 16, "KEY0主页", 16, 1, UI_MUTED);
}

static int home_hit_app(uint16_t x, uint16_t y)
{
    uint16_t i, ix1, iy1, ix2, iy2, tx, ty;
    for (i = 0; i < APP_COUNT; i++)
    {
        home_app_rect(i, &ix1, &iy1, &ix2, &iy2, &tx, &ty);
        /* 点图标或名称都算命中 */
        if (hit_rect(x, y, ix1, iy1, ix2, (uint16_t)(ty + 14)))
        {
            return (int)i;
        }
    }
    return -1;
}

static void draw_page_led(void)
{
    uint16_t w = lcddev.width;
    uint16_t y = UI_CONTENT_Y;
    ui_page_bg();
    draw_status_bar();
    draw_nav_bar("灯控工作室");
    draw_soft_btn(UI_PAD, (uint16_t)(y + 12), (uint16_t)(w / 2 - 6), (uint16_t)(y + 56),
                  g_phone.led0_on ? UI_DANGER : UI_CARD, "LED0", UI_TEXT);
    draw_soft_btn((uint16_t)(w / 2 + 6), (uint16_t)(y + 12), (uint16_t)(w - UI_PAD), (uint16_t)(y + 56),
                  g_phone.led1_on ? UI_OK : UI_CARD, "LED1", UI_TEXT);
    draw_soft_btn(UI_PAD, (uint16_t)(y + 72), (uint16_t)(w / 2 - 6), (uint16_t)(y + 116),
                  g_phone.led0_auto ? UI_ACCENT : UI_CARD, "自动0", UI_TEXT);
    draw_soft_btn((uint16_t)(w / 2 + 6), (uint16_t)(y + 72), (uint16_t)(w - UI_PAD), (uint16_t)(y + 116),
                  g_phone.led1_auto ? UI_ACCENT : UI_CARD, "自动1", UI_TEXT);
    draw_round_rect(UI_PAD, (uint16_t)(y + 136), (uint16_t)(w - UI_PAD), (uint16_t)(y + 190), UI_CARD);
    ui_show_str((uint16_t)(UI_PAD + 10), (uint16_t)(y + 148), 200, 16,
                g_phone.led0_auto ? "LED0 自动: 开" : "LED0 自动: 关", 16, 1, UI_SUB);
    ui_show_str((uint16_t)(UI_PAD + 10), (uint16_t)(y + 168), 200, 16,
                g_phone.led1_auto ? "LED1 自动: 开" : "LED1 自动: 关", 16, 1, UI_SUB);
}

static void draw_page_beep(void)
{
    uint16_t w = lcddev.width;
    uint16_t y = UI_CONTENT_Y;
    ui_page_bg();
    draw_status_bar();
    draw_nav_bar("声音");
    draw_soft_btn(UI_PAD, (uint16_t)(y + 20), (uint16_t)(w - UI_PAD), (uint16_t)(y + 68),
                  UI_ACCENT, "播放短鸣", UI_TEXT);
    draw_soft_btn(UI_PAD, (uint16_t)(y + 84), (uint16_t)(w - UI_PAD), (uint16_t)(y + 132),
                  g_phone.beep_hold ? UI_WARN : UI_CARD,
                  g_phone.beep_hold ? "保持: 开" : "保持: 关", UI_TEXT);
    draw_round_rect(UI_PAD, (uint16_t)(y + 152), (uint16_t)(w - UI_PAD), (uint16_t)(y + 200), UI_CARD);
    ui_show_str((uint16_t)(UI_PAD + 10), (uint16_t)(y + 168), 200, 16, "点击短鸣发声", 16, 1, UI_SUB);
}

static void draw_page_clock(void)
{
    char buf[48];
    uint8_t h, m, s, y, mo, d, wk;
    uint16_t ww = lcddev.width;
    uint16_t y0 = UI_CONTENT_Y;
    ui_page_bg();
    draw_status_bar();
    draw_nav_bar("时钟");
    phone_get_hms(&h, &m, &s);
    draw_round_rect(UI_PAD, (uint16_t)(y0 + 16), (uint16_t)(ww - UI_PAD), (uint16_t)(y0 + 120), UI_CARD);
    sprintf(buf, "%02d:%02d", h, m);
    lcd_show_string((uint16_t)(ww / 2 - 48), (uint16_t)(y0 + 36), 120, 24, 24, buf, UI_ACCENT2);
    sprintf(buf, ":%02d", s);
    lcd_show_string((uint16_t)(ww / 2 + 40), (uint16_t)(y0 + 44), 48, 16, 16, buf, UI_SUB);
    if (g_phone.rtc_ok)
    {
        rtc_get_date(&y, &mo, &d, &wk);
        sprintf(buf, "20%02d-%02d-%02d", y, mo, d);
        lcd_show_string((uint16_t)(UI_PAD + 16), (uint16_t)(y0 + 90), 200, 16, 16, buf, UI_TEXT);
        ui_show_str((uint16_t)(UI_PAD + 16), (uint16_t)(y0 + 136), 160, 16, "硬件时钟", 16, 1, UI_OK);
    }
    else
    {
        ui_show_str((uint16_t)(UI_PAD + 16), (uint16_t)(y0 + 90), 200, 16, "软时钟模式", 16, 1, UI_WARN);
        ui_show_str((uint16_t)(UI_PAD + 16), (uint16_t)(y0 + 136), 160, 16, "软件时钟", 16, 1, UI_SUB);
    }
    sprintf(buf, "运行 %lu 秒", (unsigned long)g_phone.uptime);
    ui_show_str((uint16_t)(UI_PAD + 16), (uint16_t)(y0 + 160), 200, 16, buf, 16, 1, UI_MUTED);
}

static void draw_page_paint(void)
{
    uint16_t w = lcddev.width;
    /* 只刷顶栏，保留画布 */
    lcd_fill(0, 0, (uint16_t)(w - 1), (uint16_t)(UI_CONTENT_Y + 22), UI_BG);
    draw_status_bar();
    draw_nav_bar("画板");
    ui_show_str(UI_PAD, (uint16_t)(UI_CONTENT_Y + 4), 100, 16, "自由绘制", 16, 1, UI_SUB);
    draw_soft_btn((uint16_t)(w - 78), (uint16_t)(UI_CONTENT_Y + 2), (uint16_t)(w - UI_PAD),
                  (uint16_t)(UI_CONTENT_Y + 22), UI_DANGER, "清除", UI_TEXT);
}

static void draw_page_settings(void)
{
    char buf[40];
    uint16_t w = lcddev.width;
    uint16_t y = UI_CONTENT_Y;
    const char *modes[] = {"正常", "省电", "勿扰", "告警"};
    ui_page_bg();
    draw_status_bar();
    draw_nav_bar("设置");
    draw_soft_btn(UI_PAD, (uint16_t)(y + 12), (uint16_t)(w - UI_PAD), (uint16_t)(y + 54),
                  UI_CARD, "下一模式", UI_TEXT);
    draw_soft_btn(UI_PAD, (uint16_t)(y + 66), (uint16_t)(w - UI_PAD), (uint16_t)(y + 108),
                  UI_CARD, "电量+10%", UI_TEXT);
    draw_soft_btn(UI_PAD, (uint16_t)(y + 120), (uint16_t)(w - UI_PAD), (uint16_t)(y + 162),
                  UI_CARD, "信号+1", UI_TEXT);
    draw_round_rect(UI_PAD, (uint16_t)(y + 178), (uint16_t)(w - UI_PAD), (uint16_t)(y + 220), UI_CARD);
    sprintf(buf, "模式: %s", modes[g_phone.mode % 4]);
    ui_show_str((uint16_t)(UI_PAD + 12), (uint16_t)(y + 194), 200, 16, buf, 16, 1, UI_ACCENT2);
}

static void draw_page_calc(void)
{
    uint16_t w = lcddev.width;
    uint16_t h = lcddev.height;
    const char *keys = "789+456-123*C0=/";
    uint16_t i, bw, bh, key_top;
    uint16_t y = UI_CONTENT_Y;
    ui_page_bg();
    draw_status_bar();
    draw_nav_bar("计算器");
    draw_round_rect(UI_PAD, (uint16_t)(y + 8), (uint16_t)(w - UI_PAD), (uint16_t)(y + 44), 0x0000);
    lcd_show_string((uint16_t)(UI_PAD + 10), (uint16_t)(y + 18), (uint16_t)(w - 40), 16, 16,
                    g_phone.calc_disp, UI_OK);
    key_top = (uint16_t)(y + 56);
    bw = (uint16_t)((w - 20) / 4);
    bh = (uint16_t)((h - key_top - 8) / 4);
    for (i = 0; i < 16; i++)
    {
        char t[2];
        uint16_t r = (uint16_t)(i / 4);
        uint16_t c = (uint16_t)(i % 4);
        uint16_t x1 = (uint16_t)(10 + c * bw);
        uint16_t y1 = (uint16_t)(key_top + r * bh);
        uint16_t x2 = (uint16_t)(x1 + bw - 6);
        uint16_t y2 = (uint16_t)(y1 + bh - 6);
        uint16_t fill = UI_CARD;
        t[0] = keys[i];
        t[1] = 0;
        if (keys[i] == '+' || keys[i] == '-' || keys[i] == '*' || keys[i] == '/' || keys[i] == '=')
            fill = UI_ACCENT;
        else if (keys[i] == 'C')
            fill = UI_DANGER;
        draw_soft_btn(x1, y1, x2, y2, fill, t, UI_TEXT);
    }
}

static void draw_page_info(void)
{
    char buf[48];
    uint16_t w = lcddev.width;
    uint16_t y = UI_CONTENT_Y;
    ui_page_bg();
    draw_status_bar();
    draw_nav_bar("信息");
    draw_round_rect(UI_PAD, (uint16_t)(y + 10), (uint16_t)(w - UI_PAD), (uint16_t)(y + 210), UI_CARD);
    ui_show_str((uint16_t)(UI_PAD + 10), (uint16_t)(y + 20), 200, 16, "迷你手机系统", 16, 1, UI_ACCENT2);
    ui_show_str((uint16_t)(UI_PAD + 10), (uint16_t)(y + 40), 200, 16, "HAL+迷你系统", 16, 1, UI_SUB);
    sprintf(buf, "屏幕 %dx%d", lcddev.width, lcddev.height);
    ui_show_str((uint16_t)(UI_PAD + 10), (uint16_t)(y + 70), 200, 16, buf, 16, 1, UI_TEXT);
    sprintf(buf, "编号 0x%04X", lcddev.id);
    ui_show_str((uint16_t)(UI_PAD + 10), (uint16_t)(y + 90), 200, 16, buf, 16, 1, UI_TEXT);
    sprintf(buf, "触摸 0x%02X", tp_dev.touchtype);
    ui_show_str((uint16_t)(UI_PAD + 10), (uint16_t)(y + 110), 200, 16, buf, 16, 1, UI_TEXT);
    sprintf(buf, "运行 %lu 秒", (unsigned long)g_phone.uptime);
    ui_show_str((uint16_t)(UI_PAD + 10), (uint16_t)(y + 130), 200, 16, buf, 16, 1, UI_SUB);
    sprintf(buf, "按键%lu 触摸%lu", (unsigned long)g_phone.key_cnt, (unsigned long)g_phone.touch_cnt);
    ui_show_str((uint16_t)(UI_PAD + 10), (uint16_t)(y + 150), 210, 16, buf, 16, 1, UI_SUB);
    ui_show_str((uint16_t)(UI_PAD + 10), (uint16_t)(y + 180), 200, 16, "串口115200", 16, 1, UI_WARN);
}

static void draw_page_chat(void)
{
    uint16_t w = lcddev.width;
    uint16_t y = UI_CONTENT_Y;
    ui_page_bg();
    draw_status_bar();
    draw_nav_bar("聊天");
    ui_show_str(UI_PAD, (uint16_t)(y + 8), 120, 16, "最近消息", 16, 1, UI_MUTED);
    draw_round_rect(UI_PAD, (uint16_t)(y + 28), (uint16_t)(w - UI_PAD), (uint16_t)(y + 100), UI_CARD2);
    /* 聊天内容可能是英文命令, ASCII 显示 */
    lcd_show_string((uint16_t)(UI_PAD + 10), (uint16_t)(y + 44), (uint16_t)(w - 40), 40, 16,
                    g_phone.chat_line, UI_TEXT);
    ui_show_str(UI_PAD, (uint16_t)(y + 116), 220, 16, "电脑串口发送", 16, 1, UI_SUB);
    ui_show_str(UI_PAD, (uint16_t)(y + 136), 220, 16, "命令 help", 16, 1, UI_MUTED);
    draw_soft_btn(UI_PAD, (uint16_t)(y + 164), (uint16_t)(w - UI_PAD), (uint16_t)(y + 208),
                  UI_ACCENT, "测试通知", UI_TEXT);
}

static void draw_page_alarm(void)
{
    char buf[40];
    uint16_t w = lcddev.width;
    uint16_t y = UI_CONTENT_Y;
    ui_page_bg();
    draw_status_bar();
    draw_nav_bar("闹钟");
    draw_soft_btn(UI_PAD, (uint16_t)(y + 16), (uint16_t)(w - UI_PAD), (uint16_t)(y + 64),
                  g_phone.alarm_en ? UI_DANGER : UI_CARD,
                  g_phone.alarm_en ? "闹钟开启" : "闹钟关闭", UI_TEXT);
    draw_soft_btn(UI_PAD, (uint16_t)(y + 80), (uint16_t)(w - UI_PAD), (uint16_t)(y + 128),
                  UI_CARD, "分钟+1", UI_TEXT);
    draw_round_rect(UI_PAD, (uint16_t)(y + 148), (uint16_t)(w - UI_PAD), (uint16_t)(y + 210), UI_CARD);
    sprintf(buf, "触发于 :%02d:00", g_phone.alarm_min);
    ui_show_str((uint16_t)(UI_PAD + 12), (uint16_t)(y + 168), 200, 16, buf, 16, 1, UI_ACCENT2);
    ui_show_str((uint16_t)(UI_PAD + 12), (uint16_t)(y + 188), 200, 16, "每分钟匹配时响铃", 16, 1, UI_MUTED);
}

static void phone_full_redraw(void)                     /* 按当前页完整重绘 */
{                                                       /* 函数体开始 */
    lcd_lock();                                         /* 锁 LCD */
    switch (g_phone.page)                               /* 分支页面 */
    {                                                   /* switch 开始 */
    case PAGE_HOME:     draw_home(); break;             /* 桌面 */
    case PAGE_LED:      draw_page_led(); break;         /* LED */
    case PAGE_BEEP:     draw_page_beep(); break;        /* 蜂鸣 */
    case PAGE_CLOCK:    draw_page_clock(); break;       /* 时钟 */
    case PAGE_PAINT:    draw_page_paint(); break;       /* 画板头 */
    case PAGE_SETTINGS: draw_page_settings(); break;    /* 设置 */
    case PAGE_CALC:     draw_page_calc(); break;        /* 计算 */
    case PAGE_INFO:     draw_page_info(); break;        /* 信息 */
    case PAGE_CHAT:     draw_page_chat(); break;        /* 聊天 */
    case PAGE_ALARM:    draw_page_alarm(); break;       /* 闹钟 */
    default:            draw_home(); break;             /* 默认桌面 */
    }                                                   /* switch 结束 */
    lcd_unlock();                                       /* 解锁 LCD */
    g_phone.dirty = 0;                                  /* 清除脏标记 */
}                                                       /* 函数体结束 */

static void phone_goto(page_id_t p)                     /* 切换页面 */
{                                                       /* 函数体开始 */
    g_phone.page_prev = g_phone.page;                   /* 记录上一页 */
    g_phone.page = p;                                   /* 设新页 */
    g_phone.dirty = 1;                                  /* 需要重绘 */
    if (p == PAGE_PAINT)                                /* 进入画板时清画布 */
    {                                                   /* if 开始 */
        lcd_lock();                                     /* 锁 */
        lcd_fill(0, (uint16_t)(UI_CONTENT_Y + 26), (uint16_t)(lcddev.width - 1),
                 (uint16_t)(lcddev.height - 1), 0xFFFF); /* 白画布 */
        lcd_unlock();                                   /* 解锁 */
    }                                                   /* if 结束 */
}                                                       /* 函数体结束 */

static void phone_back(void)                            /* 返回上一页或桌面 */
{                                                       /* 函数体开始 */
    if (g_phone.page == PAGE_HOME)                      /* 已在桌面 */
    {                                                   /* if 开始 */
        return;                                         /* 不处理 */
    }                                                   /* if 结束 */
    phone_goto(PAGE_HOME);                              /* 统一回桌面更像手机 Home */
}                                                       /* 函数体结束 */

/* ======================== 交互处理 ======================== */
static void calc_input_char(char ch)                    /* 计算器按键逻辑 */
{                                                       /* 函数体开始 */
    int32_t digit;                                      /* 数字 */
    if (ch >= '0' && ch <= '9')                         /* 数字键 */
    {                                                   /* if 开始 */
        digit = ch - '0';                               /* 转数值 */
        if (g_phone.calc_new)                           /* 新输入覆盖 */
        {                                               /* if 开始 */
            g_phone.calc_b = 0;                         /* 清 B */
            g_phone.calc_new = 0;                       /* 清除标记 */
        }                                               /* if 结束 */
        if (g_phone.calc_b < 100000000)                 /* 防溢出 */
        {                                               /* if 开始 */
            g_phone.calc_b = g_phone.calc_b * 10 + digit;  /* 累加数字 */
        }                                               /* if 结束 */
        sprintf(g_phone.calc_disp, "%ld", (long)g_phone.calc_b);  /* 刷新显示 */
    }                                                   /* if 结束 */
    else if (ch == 'C')                                 /* 清除 */
    {                                                   /* else if */
        g_phone.calc_a = 0;                             /* 清 A */
        g_phone.calc_b = 0;                             /* 清 B */
        g_phone.calc_op = 0;                            /* 清运算符 */
        g_phone.calc_new = 1;                           /* 下次覆盖 */
        strcpy(g_phone.calc_disp, "0");                 /* 显示 0 */
    }                                                   /* else if 结束 */
    else if (ch == '+' || ch == '-' || ch == '*' || ch == '/')  /* 四则 */
    {                                                   /* else if */
        g_phone.calc_a = g_phone.calc_b;                 /* A <- B */
        g_phone.calc_op = (ch == '+') ? 1 : (ch == '-') ? 2 : (ch == '*') ? 3 : 4;  /* 记运算符 */
        g_phone.calc_new = 1;                           /* 下个数新开 */
    }                                                   /* else if 结束 */
    else if (ch == '=')                                 /* 等于 */
    {                                                   /* else if */
        int32_t r = g_phone.calc_b;                     /* 默认结果 */
        if (g_phone.calc_op == 1) r = g_phone.calc_a + g_phone.calc_b;  /* 加 */
        if (g_phone.calc_op == 2) r = g_phone.calc_a - g_phone.calc_b;  /* 减 */
        if (g_phone.calc_op == 3) r = g_phone.calc_a * g_phone.calc_b;  /* 乘 */
        if (g_phone.calc_op == 4)                       /* 除 */
        {                                               /* if 开始 */
            if (g_phone.calc_b != 0) r = g_phone.calc_a / g_phone.calc_b;  /* 正常除 */
            else r = 0;                                 /* 除零保护 */
        }                                               /* if 结束 */
        g_phone.calc_b = r;                             /* 结果放 B */
        g_phone.calc_op = 0;                            /* 清运算符 */
        g_phone.calc_new = 1;                           /* 下次新输入 */
        sprintf(g_phone.calc_disp, "%ld", (long)r);     /* 显示结果 */
    }                                                   /* else if 结束 */
    g_phone.dirty = 1;                                  /* 重绘计算器 */
}                                                       /* 函数体结束 */

static void handle_touch(uint16_t x, uint16_t y)        /* 处理触摸点击 */
{                                                       /* 函数体开始 */
    uint16_t w = lcddev.width;                          /* 宽 */
    g_phone.touch_cnt++;                                /* 统计 */
    /* 任意页点左上返回胶囊 */
    if (g_phone.page != PAGE_HOME &&
        hit_rect(x, y, 0, UI_SB_H, 72, (uint16_t)(UI_CONTENT_Y - 1)))
    {
        phone_back();
        return;
    }

    if (g_phone.page == PAGE_HOME)
    {
        int idx = home_hit_app(x, y);
        if (idx >= 0)
        {
            phone_goto(g_apps[idx].page);
            xSemaphoreGive(&g_sem_beep);
        }
        return;
    }

    if (g_phone.page == PAGE_LED)
    {
        uint16_t y0 = UI_CONTENT_Y;
        if (hit_rect(x, y, UI_PAD, (uint16_t)(y0 + 12), (uint16_t)(w / 2 - 6), (uint16_t)(y0 + 56)))
        {
            g_phone.led0_on ^= 1;
            g_phone.led0_auto = 0;
            LED0(g_phone.led0_on ? 0 : 1);
            g_phone.dirty = 1;
        }
        else if (hit_rect(x, y, (uint16_t)(w / 2 + 6), (uint16_t)(y0 + 12), (uint16_t)(w - UI_PAD), (uint16_t)(y0 + 56)))
        {
            g_phone.led1_on ^= 1;
            g_phone.led1_auto = 0;
            LED1(g_phone.led1_on ? 0 : 1);
            g_phone.dirty = 1;
        }
        else if (hit_rect(x, y, UI_PAD, (uint16_t)(y0 + 72), (uint16_t)(w / 2 - 6), (uint16_t)(y0 + 116)))
        {
            g_phone.led0_auto ^= 1;
            g_phone.dirty = 1;
        }
        else if (hit_rect(x, y, (uint16_t)(w / 2 + 6), (uint16_t)(y0 + 72), (uint16_t)(w - UI_PAD), (uint16_t)(y0 + 116)))
        {
            g_phone.led1_auto ^= 1;
            g_phone.dirty = 1;
        }
        return;
    }

    if (g_phone.page == PAGE_BEEP)
    {
        uint16_t y0 = UI_CONTENT_Y;
        if (hit_rect(x, y, UI_PAD, (uint16_t)(y0 + 20), (uint16_t)(w - UI_PAD), (uint16_t)(y0 + 68)))
        {
            xSemaphoreGive(&g_sem_beep);
        }
        else if (hit_rect(x, y, UI_PAD, (uint16_t)(y0 + 84), (uint16_t)(w - UI_PAD), (uint16_t)(y0 + 132)))
        {
            g_phone.beep_hold ^= 1;
            BEEP(g_phone.beep_hold ? 1 : 0);
            g_phone.dirty = 1;
        }
        return;
    }

    if (g_phone.page == PAGE_PAINT)
    {
        uint16_t canvas_y = (uint16_t)(UI_CONTENT_Y + 26);
        if (hit_rect(x, y, (uint16_t)(w - 78), (uint16_t)(UI_CONTENT_Y + 2),
                     (uint16_t)(w - UI_PAD), (uint16_t)(UI_CONTENT_Y + 22)))
        {
            lcd_lock();
            lcd_fill(0, canvas_y, (uint16_t)(w - 1), (uint16_t)(lcddev.height - 1), 0xFFFF);
            lcd_unlock();
            g_phone.dirty = 1;
        }
        else if (y > canvas_y)
        {
            lcd_lock();
            tp_draw_big_point(x, y, UI_DANGER);
            lcd_unlock();
        }
        return;
    }

    if (g_phone.page == PAGE_SETTINGS)
    {
        uint16_t y0 = UI_CONTENT_Y;
        if (hit_rect(x, y, UI_PAD, (uint16_t)(y0 + 12), (uint16_t)(w - UI_PAD), (uint16_t)(y0 + 54)))
        {
            g_phone.mode = (uint8_t)((g_phone.mode + 1) % 4);
            if (g_phone.mode == 1) g_phone.led1_auto = 0;
            if (g_phone.mode == 2) { g_phone.beep_hold = 0; BEEP(0); }
            g_phone.dirty = 1;
        }
        else if (hit_rect(x, y, UI_PAD, (uint16_t)(y0 + 66), (uint16_t)(w - UI_PAD), (uint16_t)(y0 + 108)))
        {
            g_phone.battery = (uint8_t)((g_phone.battery + 10 > 100) ? 100 : g_phone.battery + 10);
            g_phone.dirty = 1;
        }
        else if (hit_rect(x, y, UI_PAD, (uint16_t)(y0 + 120), (uint16_t)(w - UI_PAD), (uint16_t)(y0 + 162)))
        {
            g_phone.signal = (uint8_t)((g_phone.signal + 1) % 5);
            g_phone.dirty = 1;
        }
        return;
    }

    if (g_phone.page == PAGE_CALC)
    {
        const char *keys = "789+456-123*C0=/";
        uint16_t key_top = (uint16_t)(UI_CONTENT_Y + 56);
        uint16_t bw = (uint16_t)((w - 20) / 4);
        uint16_t bh = (uint16_t)((lcddev.height - key_top - 8) / 4);
        uint16_t i;
        for (i = 0; i < 16; i++)
        {
            uint16_t r = (uint16_t)(i / 4);
            uint16_t c = (uint16_t)(i % 4);
            uint16_t x1 = (uint16_t)(10 + c * bw);
            uint16_t y1 = (uint16_t)(key_top + r * bh);
            uint16_t x2 = (uint16_t)(x1 + bw - 6);
            uint16_t y2 = (uint16_t)(y1 + bh - 6);
            if (hit_rect(x, y, x1, y1, x2, y2))
            {
                calc_input_char(keys[i]);
                break;
            }
        }
        return;
    }

    if (g_phone.page == PAGE_CHAT)
    {
        uint16_t y0 = UI_CONTENT_Y;
        if (hit_rect(x, y, UI_PAD, (uint16_t)(y0 + 164), (uint16_t)(w - UI_PAD), (uint16_t)(y0 + 208)))
        {
            post_evt(EVT_NOTIFY, 0, 0, 0);
        }
        return;
    }

    if (g_phone.page == PAGE_ALARM)
    {
        uint16_t y0 = UI_CONTENT_Y;
        if (hit_rect(x, y, UI_PAD, (uint16_t)(y0 + 16), (uint16_t)(w - UI_PAD), (uint16_t)(y0 + 64)))
        {
            g_phone.alarm_en ^= 1;
            g_phone.dirty = 1;
        }
        else if (hit_rect(x, y, UI_PAD, (uint16_t)(y0 + 80), (uint16_t)(w - UI_PAD), (uint16_t)(y0 + 128)))
        {
            g_phone.alarm_min = (uint8_t)((g_phone.alarm_min + 1) % 60);
            g_phone.dirty = 1;
        }
        return;
    }
}

static void handle_key(uint8_t key)                     /* 实体键: 像手机侧键 */
{                                                       /* 函数体开始 */
    g_phone.key_cnt++;                                  /* 统计 */
    if (key == KEY0_PRES)                               /* KEY0 = Home */
    {                                                   /* if 开始 */
        phone_goto(PAGE_HOME);                          /* 回桌面 */
    }                                                   /* if 结束 */
    else if (key == KEY1_PRES)                          /* KEY1 = Back */
    {                                                   /* else if */
        phone_back();                                   /* 返回 */
    }                                                   /* else if 结束 */
    else if (key == KEY2_PRES)                          /* KEY2 = 通知 */
    {                                                   /* else if */
        post_evt(EVT_NOTIFY, 0, 0, 0);                  /* 发通知 */
    }                                                   /* else if 结束 */
    else if (key == WKUP_PRES)                          /* WK_UP = 状态到串口 */
    {                                                   /* else if */
        if (xMutexTake(&g_mtx_print, 100) == 1)         /* 锁打印 */
        {                                               /* if 开始 */
            printf("[PHONE] page=%d up=%lu bat=%d mode=%d\r\n",  /* 打印摘要 */
                   (int)g_phone.page,                   /* 页 */
                   (unsigned long)g_phone.uptime,       /* 时长 */
                   g_phone.battery,                     /* 电量 */
                   g_phone.mode);                       /* 模式 */
            xMutexGive(&g_mtx_print);                   /* 解锁 */
        }                                               /* if 结束 */
        xSemaphoreGive(&g_sem_beep);                    /* 附带短鸣 */
    }                                                   /* else if 结束 */
}                                                       /* 函数体结束 */

static void handle_uart_line(char *line)                /* 串口命令(像手机调试桥) */
{                                                       /* 函数体开始 */
    g_phone.cmd_cnt++;                                  /* 统计 */
    strncpy(g_phone.chat_line, line, sizeof(g_phone.chat_line) - 1);  /* 记入聊天 */
    g_phone.chat_line[sizeof(g_phone.chat_line) - 1] = 0;  /* 保证结尾 0 */
    if (strcmp(line, "help") == 0)                      /* 帮助 */
    {                                                   /* if 开始 */
        safe_printf("cmds: help status home led beep clock paint set calc info chat alarm mode notify\r\n");  /* 列表 */
    }                                                   /* if 结束 */
    else if (strcmp(line, "status") == 0)               /* 状态 */
    {                                                   /* else if */
        if (xMutexTake(&g_mtx_print, 100) == 1)         /* 锁 */
        {                                               /* if 开始 */
            printf("page=%d up=%lu bat=%d sig=%d mode=%d rtc=%d\r\n",  /* 打印 */
                   (int)g_phone.page, (unsigned long)g_phone.uptime,  /* 字段 */
                   g_phone.battery, g_phone.signal, g_phone.mode, g_phone.rtc_ok);  /* 字段 */
            xMutexGive(&g_mtx_print);                   /* 解锁 */
        }                                               /* if 结束 */
    }                                                   /* else if 结束 */
    else if (strcmp(line, "home") == 0) phone_goto(PAGE_HOME);  /* 桌面 */
    else if (strcmp(line, "led") == 0) phone_goto(PAGE_LED);    /* LED */
    else if (strcmp(line, "beep") == 0) phone_goto(PAGE_BEEP);  /* 蜂鸣 */
    else if (strcmp(line, "clock") == 0) phone_goto(PAGE_CLOCK);/* 时钟 */
    else if (strcmp(line, "paint") == 0) phone_goto(PAGE_PAINT);/* 画板 */
    else if (strcmp(line, "set") == 0) phone_goto(PAGE_SETTINGS);/* 设置 */
    else if (strcmp(line, "calc") == 0) phone_goto(PAGE_CALC);  /* 计算 */
    else if (strcmp(line, "info") == 0) phone_goto(PAGE_INFO);  /* 信息 */
    else if (strcmp(line, "chat") == 0) phone_goto(PAGE_CHAT);  /* 聊天 */
    else if (strcmp(line, "alarm") == 0) phone_goto(PAGE_ALARM);/* 闹钟 */
    else if (strcmp(line, "mode") == 0)                 /* 切模式 */
    {                                                   /* else if */
        g_phone.mode = (uint8_t)((g_phone.mode + 1) % 4);  /* +1 */
        g_phone.dirty = 1;                              /* 重绘 */
    }                                                   /* else if 结束 */
    else if (strcmp(line, "notify") == 0)               /* 通知 */
    {                                                   /* else if */
        post_evt(EVT_NOTIFY, 0, 0, 0);                  /* 投递 */
    }                                                   /* else if 结束 */
    else                                                /* 未知当聊天内容 */
    {                                                   /* else 开始 */
        if (g_phone.page == PAGE_CHAT) g_phone.dirty = 1;  /* 聊天页刷新 */
        safe_printf("[chat ok]\r\n");                   /* 回执 */
    }                                                   /* else 结束 */
}                                                       /* 函数体结束 */

/* ======================== 任务实现 ======================== */
static void task_idle(void)                             /* 空闲任务 */
{                                                       /* 函数体开始 */
    while (1)                                           /* 永远空转 */
    {                                                   /* while 开始 */
    }                                                   /* while 结束 */
}                                                       /* 函数体结束 */

static void task_input(void)                            /* 输入任务: 触摸+按键 */
{                                                       /* 函数体开始 */
    uint8_t key;                                        /* 键值 */
    uint8_t down = 0;                                   /* 触摸按下边沿锁 */
    while (1)                                           /* 循环 */
    {                                                   /* while 开始 */
        key = key_scan(0);                              /* 扫描实体键 */
        if (key)                                        /* 有键 */
        {                                               /* if 开始 */
            post_evt(EVT_KEY, 0, 0, key);               /* 投递按键事件 */
        }                                               /* if 结束 */
        tp_dev.scan(0);                                 /* 扫描触摸 */
        if (tp_dev.sta & TP_PRES_DOWN)                  /* 按下 */
        {                                               /* if 开始 */
            if (!down && tp_dev.x[0] < lcddev.width && tp_dev.y[0] < lcddev.height)  /* 边沿且坐标合法 */
            {                                           /* if 开始 */
                down = 1;                               /* 标记已处理按下 */
                post_evt(EVT_TOUCH_DOWN, tp_dev.x[0], tp_dev.y[0], 0);  /* 投递触摸 */
            }                                           /* if 结束 */
            if (g_phone.page == PAGE_PAINT && tp_dev.x[0] < lcddev.width &&
                tp_dev.y[0] > (uint16_t)(UI_CONTENT_Y + 26))  /* 画板连续画 */
            {
                lcd_lock();
                tp_draw_big_point(tp_dev.x[0], tp_dev.y[0], UI_ACCENT);
                lcd_unlock();
            }
        }                                               /* if 结束 */
        else                                            /* 松开 */
        {                                               /* else 开始 */
            down = 0;                                   /* 允许下次边沿 */
        }                                               /* else 结束 */
        task_delay(20);                                 /* 20ms 轮询 */
    }                                                   /* while 结束 */
}                                                       /* 函数体结束 */

static void task_uart(void)                             /* 串口任务 */
{                                                       /* 函数体开始 */
    uint8_t len;                                        /* 长度 */
    char line[USART_REC_LEN + 1];                       /* 行缓冲 */
    while (1)                                           /* 循环 */
    {                                                   /* while 开始 */
        if (g_usart_rx_sta & 0x8000)                    /* 收完一帧 */
        {                                               /* if 开始 */
            len = (uint8_t)(g_usart_rx_sta & 0x3fff);   /* 取长度 */
            if (len >= USART_REC_LEN) len = USART_REC_LEN - 1;  /* 限幅 */
            memcpy(line, g_usart_rx_buf, len);          /* 拷贝 */
            line[len] = 0;                              /* 结尾 */
            while (len && (line[len - 1] == '\r' || line[len - 1] == '\n'))  /* 去换行 */
            {                                           /* while 开始 */
                line[--len] = 0;                        /* 截断 */
            }                                           /* while 结束 */
            g_usart_rx_sta = 0;                         /* 清接收状态 */
            post_evt(EVT_UART_LINE, 0, 0, 0);           /* 先通知(内容在全局 chat 前写入) */
            /* 直接处理,避免再开缓冲队列传字符串 */
            handle_uart_line(line);                     /* 解析命令 */
        }                                               /* if 结束 */
        task_delay(15);                                 /* 延时 */
    }                                                   /* while 结束 */
}                                                       /* 函数体结束 */

static void task_led(void)                              /* LED 任务(跑马灯整合) */
{                                                       /* 函数体开始 */
    while (1)                                           /* 循环 */
    {                                                   /* while 开始 */
        uint32_t d = (g_phone.mode == 3) ? 80 : (g_phone.mode == 1) ? 400 : 200;  /* 周期随模式 */
        if (g_phone.led0_auto)                          /* 自动 LED0 */
        {                                               /* if 开始 */
            LED0_TOGGLE();                              /* 翻转 */
            g_phone.led0_on ^= 1;                       /* 同步逻辑 */
        }                                               /* if 结束 */
        task_delay(d);                                  /* 延时 */
        if (g_phone.led1_auto && g_phone.mode != 1)     /* ECO 不闪 LED1 */
        {                                               /* if 开始 */
            LED1_TOGGLE();                              /* 翻转 */
            g_phone.led1_on ^= 1;                       /* 同步 */
        }                                               /* if 结束 */
        task_delay(d);                                  /* 延时 */
    }                                                   /* while 结束 */
}                                                       /* 函数体结束 */

static void task_beep(void)                             /* 蜂鸣任务 */
{                                                       /* 函数体开始 */
    while (1)                                           /* 循环 */
    {                                                   /* while 开始 */
        if (xSemaphoreTake(&g_sem_beep, 80) == 1)       /* 等到短鸣 */
        {                                               /* if 开始 */
            if (g_phone.mode != 2)                      /* 非勿扰 */
            {                                           /* if 开始 */
                BEEP(1);                                /* 响 */
                task_delay(70);                         /* 持续 */
                if (!g_phone.beep_hold) BEEP(0);        /* 非保持则关 */
            }                                           /* if 结束 */
        }                                               /* if 结束 */
        else if (g_phone.mode == 3)                     /* 告警模式间歇 */
        {                                               /* else if */
            BEEP(1);                                    /* 响 */
            task_delay(50);                             /* 短 */
            BEEP(0);                                    /* 停 */
            task_delay(150);                            /* 间隔 */
        }                                               /* else if 结束 */
        else                                            /* 普通刷新保持 */
        {                                               /* else 开始 */
            BEEP(g_phone.beep_hold ? 1 : 0);            /* 按保持状态 */
        }                                               /* else 结束 */
    }                                                   /* while 结束 */
}                                                       /* 函数体结束 */

static void task_monitor(void)                          /* 1s 监控: 时钟/电量/节拍事件 */
{                                                       /* 函数体开始 */
    while (1)                                           /* 循环 */
    {                                                   /* while 开始 */
        task_delay(1000);                               /* 1 秒 */
        g_phone.uptime++;                               /* 开机秒 */
        phone_soft_tick();                              /* 软时钟与闹钟 */
        post_evt(EVT_TICK_1S, 0, 0, 0);                 /* 通知 UI 可局部刷新 */
        if ((g_phone.uptime % 10u) == 0u)               /* 每 10s 串口心跳 */
        {                                               /* if 开始 */
            if (xMutexTake(&g_mtx_print, 30) == 1)      /* 尝试锁 */
            {                                           /* if 开始 */
                printf("[HB] t=%lu page=%d bat=%d\r\n", (unsigned long)g_phone.uptime, (int)g_phone.page, g_phone.battery);  /* 心跳 */
                xMutexGive(&g_mtx_print);               /* 解锁 */
            }                                           /* if 结束 */
        }                                               /* if 结束 */
    }                                                   /* while 结束 */
}                                                       /* 函数体结束 */

static void task_ui(void)                               /* UI 中枢: 消费事件+重绘 */
{                                                       /* 函数体开始 */
    evt_msg_t ev;                                       /* 事件 */
    task_delay(30);                                     /* 等其它任务就绪 */
    g_phone.dirty = 1;                                  /* 首屏强制绘制 */
    while (1)                                           /* 循环 */
    {                                                   /* while 开始 */
        if (g_phone.dirty)                              /* 需要整页刷新 */
        {                                               /* if 开始 */
            phone_full_redraw();                        /* 重绘 */
        }                                               /* if 结束 */
        if (xQueueReceive(&g_q, &ev, 50) == 1)          /* 等事件, 50tick 超时也可刷 dirty */
        {                                               /* if 开始 */
            switch (ev.type)                            /* 分发 */
            {                                           /* switch 开始 */
            case EVT_TOUCH_DOWN:                        /* 触摸 */
                handle_touch(ev.x, ev.y);               /* 处理 */
                break;                                  /* 跳出 */
            case EVT_KEY:                               /* 按键 */
                handle_key((uint8_t)ev.v);              /* 处理 */
                break;                                  /* 跳出 */
            case EVT_NOTIFY:                            /* 通知 */
                g_phone.notify_flag = 1;                /* 亮通知 */
                strcpy(g_phone.status_tip, "Notify");   /* 提示字 */
                if (g_phone.mode != 2) xSemaphoreGive(&g_sem_beep);  /* 非勿扰则响 */
                g_phone.dirty = 1;                      /* 重绘状态栏 */
                break;                                  /* 跳出 */
            case EVT_TICK_1S:                           /* 秒刷新 */
                if (g_phone.page == PAGE_HOME || g_phone.page == PAGE_CLOCK || g_phone.page == PAGE_INFO)  /* 这些页显示时间 */
                {                                       /* if 开始 */
                    g_phone.dirty = 1;                  /* 触发重绘 */
                }                                       /* if 结束 */
                else                                    /* 其它页只刷状态栏时间 */
                {                                       /* else 开始 */
                    lcd_lock();                         /* 锁 */
                    draw_status_bar();                  /* 状态栏 */
                    lcd_unlock();                       /* 解锁 */
                }                                       /* else 结束 */
                if (g_phone.notify_flag && (g_phone.uptime % 3u) == 0u)  /* 通知闪 3s 消 */
                {                                       /* if 开始 */
                    g_phone.notify_flag = 0;            /* 清除 */
                }                                       /* if 结束 */
                break;                                  /* 跳出 */
            case EVT_GOTO_PAGE:                         /* 跳转 */
                phone_goto((page_id_t)ev.v);            /* 去指定页 */
                break;                                  /* 跳出 */
            default:                                    /* 其它 */
                break;                                  /* 忽略 */
            }                                           /* switch 结束 */
        }                                               /* if 结束 */
    }                                                   /* while 结束 */
}                                                       /* 函数体结束 */

/* ======================== 板级初始化 ======================== */
static void phone_state_init(void)                      /* 初始化手机状态默认值 */
{                                                       /* 函数体开始 */
    memset(&g_phone, 0, sizeof(g_phone));               /* 清零 */
    g_phone.page = PAGE_HOME;                           /* 桌面 */
    g_phone.led0_auto = 1;                              /* 默认跑马 */
    g_phone.led1_auto = 1;                              /* 默认跑马 */
    g_phone.battery = 86;                               /* 假电量 */
    g_phone.signal = 4;                                 /* 满格 */
    g_phone.sh = 12;                                    /* 默认中午 */
    g_phone.sm = 0;                                     /* 0 分 */
    g_phone.ss = 0;                                     /* 0 秒 */
    g_phone.alarm_min = 0;                              /* 闹钟 0 分 */
    strcpy(g_phone.calc_disp, "0");                     /* 计算器显示 */
    g_phone.calc_new = 1;                               /* 新输入 */
    strcpy(g_phone.chat_line, "Hello Phone OS");        /* 默认聊天(串口英文) */
    strcpy(g_phone.status_tip, "就绪");                 /* 状态提示 */
}                                                       /* 函数体结束 */

/* 启动阶段闪灯：即使后面卡在 LCD/触摸，也能判断程序是否在跑 */
static void boot_led_blink(uint8_t times)
{
    uint8_t i;
    for (i = 0; i < times; i++)
    {
        LED0(0);                                        /* 红灯亮(低电平有效) */
        LED1(0);                                        /* 绿灯亮 */
        delay_ms(80);
        LED0(1);                                        /* 灭 */
        LED1(1);
        delay_ms(80);
    }
}

static void board_init(void)                            /* 板级外设初始化 */
{                                                       /* 函数体开始 */
    uint8_t rtc_ret;                                    /* RTC 返回 */
    phone_state_init();                                 /* 状态默认 */
    HAL_Init();                                         /* HAL 库初始化 */
    sys_stm32_clock_init(336, 8, 2, 7);                 /* 系统 168MHz (HSE=8M) */
    SystemCoreClockUpdate();                            /* 同步 CMSIS 的 SystemCoreClock */
    if (SystemCoreClock < 1000000u || SystemCoreClock > 200000000u)
    {
        SystemCoreClock = 168000000u;                   /* 兜底，保证 SysTick 1ms 正确 */
    }
    delay_init(168);                                    /* 延时基准 */
    led_init();                                         /* 先 LED：用于判断“程序是否跑起来” */
    boot_led_blink(3);                                  /* 上电连闪 3 次 */
    usart_init(115200);                                 /* 串口(板载 USB 转串口 P10) */
    beep_init();                                        /* 蜂鸣器 */
    key_init();                                         /* 按键 */
    printf("\r\n[BOOT] MiniFreeRTOS Phone OS\r\n");
    printf("[BOOT] CLK=%lu Hz\r\n", (unsigned long)SystemCoreClock);
    printf("[BOOT] init LCD...\r\n");
    lcd_init();                                         /* TFT MCU 屏(FSMC)，勿插 RGB 屏 */
    printf("[BOOT] LCD ID=0x%04X %ux%u\r\n",
           (unsigned)lcddev.id, (unsigned)lcddev.width, (unsigned)lcddev.height);
    if (lcddev.id == 0u)
    {
        printf("[BOOT][WARN] LCD ID=0: 检查 MCU 屏排线/座子/背光跳线\r\n");
    }
    printf("[BOOT] init touch...\r\n");
    tp_dev.init();                                      /* 电阻屏首次可能进入校准(约10s超时) */
    printf("[BOOT] touchtype=0x%02X\r\n", (unsigned)tp_dev.touchtype);
    rtc_ret = rtc_init();                               /* 尝试 RTC: 0 成功, 非0 失败 */
    g_phone.rtc_ok = (rtc_ret == 0) ? 1 : 0;            /* 失败则状态栏/时钟走软时钟 */
    printf("[BOOT] RTC %s\r\n", g_phone.rtc_ok ? "OK" : "soft-clock");
    LED0(1);                                            /* 先灭，交给 LED 任务闪 */
    LED1(1);
    BEEP(0);
    /* 启动短鸣一声：确认蜂鸣器与代码已执行到此 */
    BEEP(1);
    delay_ms(50);
    BEEP(0);
    xQueueCreate(&g_q, g_qbuf, sizeof(evt_msg_t), 24);  /* 事件队列 */
    xSemaphoreCreateCounting(&g_sem_beep, 5, 0);        /* 短鸣信号量 */
    xMutexCreate(&g_mtx_print);                         /* 打印锁 */
    xMutexCreate(&g_mtx_lcd);                           /* LCD 锁 */
    printf("[BOOT] UART 115200, type help\r\n");
    printf("[BOOT] Touch apps / KEY0=Home KEY1=Back\r\n");
    printf("[BOOT] start RTOS...\r\n");
}                                                       /* 函数体结束 */

int main(void)                                          /* 程序入口 */
{                                                       /* 函数体开始 */
    board_init();                                       /* 初始化板级与状态 */
    /* 创建任务: 数字越大优先级越高 */
    task_create(&g_t_ui,   task_ui,      g_stk_ui,   2048, 5);  /* UI 最高业务 */
    task_create(&g_t_in,   task_input,   g_stk_in,   768,  4);  /* 输入 */
    task_create(&g_t_uart, task_uart,    g_stk_uart, 1024, 4);  /* 串口 */
    task_create(&g_t_beep, task_beep,    g_stk_beep, 384,  3);  /* 蜂鸣 */
    task_create(&g_t_led,  task_led,     g_stk_led,  512,  2);  /* LED */
    task_create(&g_t_mon,  task_monitor, g_stk_mon,  768,  2);  /* 监控 */
    task_create(&g_t_idle, task_idle,    g_stk_idle, 256,  0);  /* 空闲 */
    current_tcb = &g_t_ui;                              /* 当前任务指 UI */
    NVIC_SetPriority(PendSV_IRQn, 0xFF);                /* PendSV 最低优先级 */
    systick_init();                                     /* 1ms 节拍(必须在任务创建后) */
    __enable_irq();                                     /* 确保开中断，否则 task_delay 永不返回 */
    start_first_task();                                 /* 启动第一个任务(不返回) */
    while (1)                                           /* 不应执行到 */
    {
    }
}                                                       /* 函数体结束 */
