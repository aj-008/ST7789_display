#include "util.h"
#include <string.h>
#include "../ST7789/hardware.h"


uint16_t framebuffer[SCREEN_WIDTH * SCREEN_HEIGHT];

/**
 * @brief Converts R, G, B (0-255) to 16-bit RGB565 format.
 */
uint16_t color565(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

/**
 * @brief Draws a single pixel at specified coordinates (x, y) with a given color.
 */
void draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
    if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) {
        return;
    }

    set_address_window(x, y, x, y);

    uint8_t high_byte = (color >> 8) & 0xFF;
    uint8_t low_byte = color & 0xFF;
    uint8_t pixel_bytes[2] = {high_byte, low_byte}; 

    spi_write_data(pixel_bytes, 2);
}

void draw_pixel_fb(uint16_t x, uint16_t y, uint16_t color) {
    if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) return;
    
    uint16_t swapped = (color << 8) | (color >> 8);  
    framebuffer[y * SCREEN_WIDTH + x] = swapped;
}


void fill_screen(uint16_t color) {
    uint16_t pixel = (color << 8) | (color >> 8);
    for (uint32_t i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        framebuffer[i] = pixel;
    }
    
    set_address_window(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
    start_display_transfer(framebuffer, SCREEN_WIDTH * SCREEN_HEIGHT);
}


void fb_put565(uint16_t x, uint16_t y, uint16_t color565_native) {
    if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) return;
    framebuffer[y * SCREEN_WIDTH + x] =
        (uint16_t)((color565_native << 8) | (color565_native >> 8));
}

void push_scanline_swapped(uint16_t y, const uint16_t *line_swapped, uint16_t len) {
    if (y >= SCREEN_HEIGHT) return;
    if (len == 0) return;
    if (len > SCREEN_WIDTH) len = SCREEN_WIDTH;

    set_address_window(0, y, (uint16_t)(len - 1), y);
    start_display_transfer((uint16_t *)line_swapped, len);
}


void push_scanline_swapped_xy(uint16_t x0, uint16_t y, const uint16_t *line_swapped, uint16_t len) {
    if (y >= SCREEN_HEIGHT) return;
    if (x0 >= SCREEN_WIDTH) return;
    if (len == 0) return;
    if (x0 + len > SCREEN_WIDTH) len = (uint16_t)(SCREEN_WIDTH - x0);

    set_address_window(x0, y, (uint16_t)(x0 + len - 1), y);
    start_display_transfer((uint16_t *)line_swapped, len);
}



