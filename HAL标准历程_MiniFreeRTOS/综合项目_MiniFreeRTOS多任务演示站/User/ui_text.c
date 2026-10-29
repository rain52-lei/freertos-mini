#include "ui_text.h"
#include "ui_font16.h"
#include "./BSP/LCD/lcd.h"

/* 解码一个 UTF-8 码点, 返回消耗字节数; 失败返回 0 */
static uint8_t utf8_next(const uint8_t *s, uint32_t *cp)
{
    uint8_t c = s[0];
    if (c < 0x80)
    {
        *cp = c;
        return 1;
    }
    if ((c & 0xE0) == 0xC0)
    {
        if ((s[1] & 0xC0) != 0x80) return 0;
        *cp = ((uint32_t)(c & 0x1F) << 6) | (s[1] & 0x3F);
        return 2;
    }
    if ((c & 0xF0) == 0xE0)
    {
        if ((s[1] & 0xC0) != 0x80 || (s[2] & 0xC0) != 0x80) return 0;
        *cp = ((uint32_t)(c & 0x0F) << 12) | ((uint32_t)(s[1] & 0x3F) << 6) | (s[2] & 0x3F);
        return 3;
    }
    if ((c & 0xF8) == 0xF0)
    {
        if ((s[1] & 0xC0) != 0x80 || (s[2] & 0xC0) != 0x80 || (s[3] & 0xC0) != 0x80) return 0;
        *cp = ((uint32_t)(c & 0x07) << 18) | ((uint32_t)(s[1] & 0x3F) << 12) |
              ((uint32_t)(s[2] & 0x3F) << 6) | (s[3] & 0x3F);
        return 4;
    }
    return 0;
}

static void draw_hz16(uint16_t x, uint16_t y, const uint8_t *mat, uint8_t mode, uint16_t color)
{
    uint8_t t, t1, temp;
    uint16_t y0 = y;
    uint16_t xx = x;
    uint16_t yy = y;
    for (t = 0; t < 32; t++)
    {
        temp = mat[t];
        for (t1 = 0; t1 < 8; t1++)
        {
            if (temp & 0x80)
            {
                lcd_draw_point(xx, yy, color);
            }
            else if (mode == 0)
            {
                lcd_draw_point(xx, yy, g_back_color);
            }
            temp <<= 1;
            yy++;
            if ((yy - y0) == 16)
            {
                yy = y0;
                xx++;
                break;
            }
        }
    }
}

uint16_t ui_str_width(const char *utf8, uint8_t size)
{
    const uint8_t *p = (const uint8_t *)utf8;
    uint16_t w = 0;
    uint32_t cp;
    uint8_t n;
    uint8_t half = (uint8_t)(size / 2);
    if (size != 16) size = 16;
    half = 8;
    while (*p)
    {
        n = utf8_next(p, &cp);
        if (n == 0) { p++; continue; }
        if (cp < 0x80) w = (uint16_t)(w + half);
        else w = (uint16_t)(w + size);
        p += n;
    }
    return w;
}

void ui_show_str(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                 const char *utf8, uint8_t size, uint8_t mode, uint16_t color)
{
    const uint8_t *p = (const uint8_t *)utf8;
    uint16_t x0 = x, y0 = y;
    uint32_t cp;
    uint8_t n;
    const uint8_t *mat;
    uint8_t sz = 16;
    uint8_t half = 8;

    if (size == 12 || size == 16 || size == 24) sz = size;
    if (sz != 16)
    {
        /* 仅 16 号中文字库; 其它尺寸中文仍按 16 画, 英文用 size */
        half = (uint8_t)(size / 2);
    }

    while (*p)
    {
        n = utf8_next(p, &cp);
        if (n == 0) { p++; continue; }

        if (cp < 0x80)
        {
            if (x > (uint16_t)(x0 + width - half))
            {
                y = (uint16_t)(y + ((sz == 16) ? 16 : size));
                x = x0;
            }
            if (y > (uint16_t)(y0 + height - ((sz == 16) ? 16 : size))) break;
            if (cp == 13)
            {
                y = (uint16_t)(y + ((sz == 16) ? 16 : size));
                x = x0;
            }
            else
            {
                lcd_show_char(x, y, (char)cp, (sz == 16) ? 16 : size, mode, color);
            }
            x = (uint16_t)(x + half);
        }
        else
        {
            if (x > (uint16_t)(x0 + width - 16))
            {
                y = (uint16_t)(y + 16);
                x = x0;
            }
            if (y > (uint16_t)(y0 + height - 16)) break;
            mat = ui_font16_get((uint16_t)cp);
            if (mat)
            {
                draw_hz16(x, y, mat, mode, color);
            }
            else
            {
                /* 缺字: 画一个方框占位 */
                lcd_draw_rectangle(x, y, (uint16_t)(x + 14), (uint16_t)(y + 14), color);
            }
            x = (uint16_t)(x + 16);
        }
        p += n;
    }
}

void ui_show_str_mid(uint16_t x, uint16_t y, uint16_t width,
                     const char *utf8, uint8_t size, uint16_t color)
{
    uint16_t tw = ui_str_width(utf8, size);
    uint16_t ox;
    if (tw >= width) ox = x;
    else ox = (uint16_t)(x + (width - tw) / 2);
    ui_show_str(ox, y, width, 24, utf8, size, 1, color);
}
