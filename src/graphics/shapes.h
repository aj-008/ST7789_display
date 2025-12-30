/**************************************************************
 *
 *                     shapes.h
 *
 *     Assignment: ST7789_display
 *     Author:    AJ Romeo
 *     Date:      December 30, 2025
 *
 *     Basic 2D shape drawing routines built on the framebuffer.
 *
 **************************************************************/

#ifndef SHAPES_H
#define SHAPES_H

#include <stdint.h>

void draw_circle(int x0, int y0, int r, uint16_t color);

void fill_circle(int x0, int y0, int r, uint16_t color);

void draw_rectangle(uint16_t x, uint16_t y,
                    uint16_t w, uint16_t h,
                    uint16_t color);

void fill_rectangle(uint16_t x, uint16_t y,
                    uint16_t w, uint16_t h,
                    uint16_t color);

void draw_hline(uint16_t x, uint16_t y,
                uint16_t len, uint16_t color);

void draw_vline(uint16_t x, uint16_t y,
                uint16_t len, uint16_t color);

void draw_line(int x0, int y0, int x1, int y1, uint16_t color);

void draw_rounded_rec(uint16_t x, uint16_t y,
                      uint16_t w, uint16_t h,
                      uint16_t radius, uint16_t color);

#endif
