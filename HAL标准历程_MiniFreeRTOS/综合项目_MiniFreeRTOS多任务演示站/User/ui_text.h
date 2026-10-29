/* 轻量中文 UI 文字 (UTF-8, 内嵌 16x16 字库, 不依赖 SD/字库升级) */
#ifndef UI_TEXT_H
#define UI_TEXT_H

#include <stdint.h>

/* mode: 0 填充背景(g_back_color), 1 叠加 */
void ui_show_str(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                 const char *utf8, uint8_t size, uint8_t mode, uint16_t color);

/* 在 [x, x+width) 内水平居中显示一行 */
void ui_show_str_mid(uint16_t x, uint16_t y, uint16_t width,
                     const char *utf8, uint8_t size, uint16_t color);

/* 估算字符串显示像素宽度(中文=size, 英文=size/2) */
uint16_t ui_str_width(const char *utf8, uint8_t size);

#endif
