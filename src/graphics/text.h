/**************************************************************
 *
 *                     text.h
 *
 *     Assignment: ST7789_display
 *     Author:    AJ Romeo
 *     Date:      December 30, 2025
 *
 *     Text drawing interface (bitmap fonts) for the framebuffer.
 *
 **************************************************************/

#ifndef TEXT_H
#define TEXT_H

#include <stdint.h>

void draw_text(uint16_t x, uint16_t y,
               uint16_t size, uint16_t color,
               const char *text);

void draw_text_bg(uint16_t x, uint16_t y,
                  uint16_t size, uint16_t fg, uint16_t bg,
                  const char *text);

void draw_text_bg_unwrapped(uint16_t x, uint16_t y,
                            uint16_t size, uint16_t fg, uint16_t bg,
                            const char *text);

void draw_text_center(uint16_t y, uint16_t size,
                      uint16_t color, const char *text);

void draw_text_center_bg(uint16_t y, uint16_t size,
                         uint16_t text_color, uint16_t bg,
                         const char *text);

/* 16x16 word-wrapped centering helper (used for quotes). */
void draw_quote_centered(const char *quote, uint16_t color);

/* Font bitmaps. */
extern const uint8_t CGA16[];
extern const uint8_t jmk32[];
extern const uint8_t big48[];

#endif
