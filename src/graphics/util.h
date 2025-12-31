/**************************************************************
 *
 *                          util.h
 *
 *     Author:  AJ Romeo
 *
 *     Framebuffer utilities and low-level drawing/present helpers.
 *
 **************************************************************/

#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>
#include <stdint.h>
#include "../display_config.h"

/*
 * The framebuffer is stored in byte-swapped RGB565 (big-endian) so it can be
 * DMA'd directly to the ST7789 without per-pixel swapping on transmit.
 */
extern uint16_t framebuffer[SCREEN_WIDTH * SCREEN_HEIGHT];

uint16_t color565(uint8_t r, uint8_t g, uint8_t b);

void draw_pixel(uint16_t x, uint16_t y, uint16_t color);

void present_full(void);

void present_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

void fill_screen(uint16_t color);

void push_scanline_swapped(uint16_t y, const uint16_t *line_swapped,
                           uint16_t len);

void push_scanline_swapped_xy(uint16_t x0, uint16_t y,
                              const uint16_t *line_swapped, uint16_t len);

void push_rect_fb(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

#endif

