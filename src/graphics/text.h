#ifndef TEXT_H
#define TEXT_H

#include "pico/stdlib.h"

void draw_text(uint16_t x, uint16_t y, uint16_t size, uint16_t color, uint8_t *text);
void draw_text_bg(uint16_t x, uint16_t y, uint16_t size, uint16_t fg, uint16_t bg,
                  const char *text);
void draw_text_center(uint16_t y, uint16_t size, uint16_t color, uint8_t *text);
void draw_text_center_bg(uint16_t y, uint16_t size, uint16_t text_color, uint16_t bg, uint8_t *text);
void draw_text_center(uint16_t y, uint16_t size, uint16_t color, uint8_t *text);
extern const uint8_t CGA16[];

#endif
