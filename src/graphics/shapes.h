#ifndef SHAPES_H
#define SHAPES_H

#include "pico/stdlib.h"

void draw_circle(uint16_t xc, uint16_t yc, uint16_t r, uint16_t color);
void fill_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color);
void draw_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void fill_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h,  uint16_t color);
void draw_hline(uint16_t x, uint16_t y, uint16_t length, uint16_t color);
void draw_corner_round(uint16_t x0, uint16_t y0, uint16_t r, uint16_t corner, uint16_t color);
void draw_rounded_rec(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint16_t color);
void draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);

#endif
