#ifndef TEXT_H
#define TEXT_H

#include "pico/stdlib.h"

void draw_text(uint16_t x, uint16_t y, uint16_t size, uint16_t color, uint8_t *text);
extern const uint8_t CGA16[];

#endif
