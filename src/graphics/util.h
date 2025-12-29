#ifndef UTIL_H
#define UTIL_H

#include "pico/stdlib.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240


extern uint16_t framebuffer[SCREEN_WIDTH * SCREEN_HEIGHT];

uint16_t color565(uint8_t r, uint8_t g, uint8_t b);
void draw_pixel(uint16_t x, uint16_t y, uint16_t color);
void fill_screen(uint16_t color);
void draw_circle(uint16_t xc, uint16_t yc, uint16_t r, uint16_t color);
void fill_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color);
void draw_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void fill_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h,  uint16_t color);
void draw_hline(uint16_t x, uint16_t y, uint16_t length, uint16_t color);
void fb_put565(uint16_t x, uint16_t y, uint16_t color565_native);
void push_scanline_swapped(uint16_t y, const uint16_t *line_swapped, uint16_t len);

#endif
