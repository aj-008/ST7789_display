/**************************************************************
 *
 *                     util.c
 *
 *     Assignment: ST7789_display
 *     Author:    AJ Romeo
 *     Date:      December 30, 2025
 *
 *     Framebuffer storage and primitive pixel/present operations.
 *
 **************************************************************/

#include "util.h"
#include <string.h>
#include "../ST7789/hardware.h"

uint16_t framebuffer[SCREEN_WIDTH * SCREEN_HEIGHT];

uint16_t color565(uint8_t r, uint8_t g, uint8_t b)
{
        return (uint16_t)(((r & 0xF8u) << 8) | ((g & 0xFCu) << 3) | (b >> 3));
}

void fb_put565(uint16_t x, uint16_t y, uint16_t color565_native)
{
        if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) return;
        framebuffer[y * SCREEN_WIDTH + x] =
        (uint16_t)((color565_native << 8) | (color565_native >> 8));
}

void draw_pixel(uint16_t x, uint16_t y, uint16_t color)
{
        fb_put565(x, y, color);
}

/********** present_full ********
 *
 * Transfer the entire framebuffer to the display.
 *
 * Notes:
 *      The framebuffer is stored byte-swapped for DMA; this function sends it
 *      as-is after setting the full-screen address window.
 *
 ************************/
void present_full(void)
{
        set_address_window(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
        start_display_transfer(framebuffer,
        (size_t)SCREEN_WIDTH *
        (size_t)SCREEN_HEIGHT);
}

/********** present_rect ********
 *
 * Transfer a rectangular region of the framebuffer to the display.
 *
 * Parameters:
 *      x, y: top-left corner in pixels
 *      w, h: width and height in pixels
 *
 * Notes:
 *      The region is clipped to the display bounds.
 *
 ************************/
void present_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
        push_rect_fb(x, y, w, h);
}

void fill_screen(uint16_t color)
{
        const uint16_t swapped = (uint16_t)((color << 8) | (color >> 8));
        for (size_t i = 0;
        i < (size_t)SCREEN_WIDTH *
        (size_t)SCREEN_HEIGHT;
        i++) {
                framebuffer[i] = swapped;
        }
        present_full();
}

void push_scanline_swapped(uint16_t y, const uint16_t *line_swapped,
                           uint16_t len)
{
        if (y >= SCREEN_HEIGHT) return;
        if (len == 0) return;
        if (len > SCREEN_WIDTH) len = SCREEN_WIDTH;

        set_address_window(0, y, (uint16_t)(len - 1), y);
        start_display_transfer(line_swapped, len);
}

void push_scanline_swapped_xy(uint16_t x0, uint16_t y, 
                              const uint16_t *line_swapped, uint16_t len)
{
        if (y >= SCREEN_HEIGHT) return;
        if (x0 >= SCREEN_WIDTH) return;
        if (len == 0) return;
        if ((uint32_t)x0 + (uint32_t)len > SCREEN_WIDTH) {
                len = (uint16_t)(SCREEN_WIDTH - x0);
        }

        set_address_window(x0, y,
        (uint16_t)(x0 + len - 1), y);
        start_display_transfer(line_swapped, len);
}

void push_rect_fb(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
        if (w == 0 || h == 0) return;
        if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) return;

        if ((uint32_t)x + (uint32_t)w > SCREEN_WIDTH) {
                w = (uint16_t)(SCREEN_WIDTH - x);
        }
        if ((uint32_t)y + (uint32_t)h > SCREEN_HEIGHT) {
                h = (uint16_t)(SCREEN_HEIGHT - y);
        }

        set_address_window(x, y, (uint16_t)(x + w - 1), (uint16_t)(y + h - 1));

        for (uint16_t row = 0; row < h; row++) {
                const uint16_t *row_ptr;

                row_ptr = &framebuffer[(y + row) *
                SCREEN_WIDTH + x];
                start_display_transfer(row_ptr, w);
        }
}
