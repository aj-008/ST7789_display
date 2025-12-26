#ifndef BITMAP_H
#define BITMAP_H


#include "pico/stdlib.h"

typedef struct {
    uint16_t width;
    uint16_t height;
    uint16_t bpp;  // bits per pixel
    uint32_t data_offset;
} BMP_Header;

bool parse_bmp_header(uint8_t *bmp_data, BMP_Header *header);
uint16_t rgb888_to_rgb565(uint8_t r, uint8_t g, uint8_t b);
void draw_bmp(uint16_t x0, uint16_t y0, uint8_t *bmp_data);
void draw_bmp_rotated(uint16_t x0, uint16_t y0, uint8_t *bmp_data, uint16_t rotation);

#endif
