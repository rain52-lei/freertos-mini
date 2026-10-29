/* Auto-generated 16x16 Chinese UI font (UTF-16 codepoint) */
#ifndef UI_FONT16_H
#define UI_FONT16_H
#include <stdint.h>
#define UI_FONT16_SIZE 16
#define UI_FONT16_BYTES 32
#define UI_FONT16_COUNT 107
typedef struct { uint16_t cp; uint8_t data[32]; } ui_font16_item_t;
extern const ui_font16_item_t g_ui_font16[UI_FONT16_COUNT];
const uint8_t *ui_font16_get(uint16_t cp);
#endif
