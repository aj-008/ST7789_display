/**************************************************************
 *
 *                          image.h
 *
 *     Author:  AJ Romeo
 *
 *     Bitmap and BMP image drawing interface.
 *
 **************************************************************/

#ifndef IMAGE_H
#define IMAGE_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
        uint32_t data_offset;
        uint32_t width;
        uint32_t height;
        uint16_t bpp;
} BMP_Header;

void draw_bitmap(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h,
                 uint16_t color, const uint8_t *bitmap);

void draw_bitmap_bg(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h,
                    uint16_t fg, uint16_t bg, const uint8_t *bitmap);

void draw_bmp(uint16_t x0, uint16_t y0, const uint8_t *bmp_data,
              uint16_t rotation);

#endif

