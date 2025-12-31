/**************************************************************
 *
 *                          util.c
 *
 *     Author:  AJ Romeo
 *
 *     Framebuffer storage and primitive pixel/present operations.
 *
 **************************************************************/

#include "util.h"
#include <string.h>
#include "../ST7789/hardware.h"

uint16_t framebuffer[SCREEN_WIDTH * SCREEN_HEIGHT];

/********** color565 ********
 *
 * Convert 24-bit RGB to 16-bit RGB565 format.
 *
 * Parameters:
 *      r, g, b: 8-bit color components (0-255)
 *
 * Return:
 *      RGB565 color value in native-endian format
 *
 ************************/
uint16_t color565(uint8_t r, uint8_t g, uint8_t b)
{
        return (uint16_t)(((r & 0xF8u) << 8) | ((g & 0xFCu) << 3) | 
                          (b >> 3));
}

/********** fb_put565 ********
 *
 * Write a pixel to the framebuffer with byte-swapping for DMA.
 *
 * Parameters:
 *      x, y:              pixel coordinates
 *      color565_native:   RGB565 color in native-endian format
 *
 * Notes:
 *      The framebuffer stores pixels byte-swapped (big-endian) so they can be
 *      DMA'd directly to the ST7789 without per-pixel processing.
 *
 ************************/
void fb_put565(uint16_t x, uint16_t y, uint16_t color565_native)
{
        if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) {
                return;
        }
        framebuffer[y * SCREEN_WIDTH + x] =
                (uint16_t)((color565_native << 8) | (color565_native >> 8));
}

/********** draw_pixel ********
 *
 * Draw a single pixel to the framebuffer.
 *
 * Parameters:
 *      x, y:  pixel coordinates
 *      color: RGB565 color in native-endian format
 *
 ************************/
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

/********** fill_screen ********
 *
 * Fill the entire screen with a solid color and present to the display.
 *
 * Parameters:
 *      color: RGB565 color in native-endian format
 *
 ************************/
void fill_screen(uint16_t color)
{
        const uint16_t swapped = (uint16_t)((color << 8) | (color >> 8));
        for (size_t i = 0; 
             i < (size_t)SCREEN_WIDTH * (size_t)SCREEN_HEIGHT;
             i++) {
                framebuffer[i] = swapped;
        }
        present_full();
}

/********** push_scanline_swapped ********
 *
 * Push a full-width scanline to the display from pre-swapped data.
 *
 * Parameters:
 *      y:            scanline y-coordinate
 *      line_swapped: pointer to byte-swapped RGB565 pixel data
 *      len:          number of pixels to transfer
 *
 * Notes:
 *      The input data must already be byte-swapped for the ST7789.
 *
 ************************/
void push_scanline_swapped(uint16_t y, const uint16_t *line_swapped,
                           uint16_t len)
{
        if (y >= SCREEN_HEIGHT) {
                return;
        }
        if (len == 0) {
                return;
        }
        if (len > SCREEN_WIDTH) {
                len = SCREEN_WIDTH;
        }

        set_address_window(0, y, (uint16_t)(len - 1), y);
        start_display_transfer(line_swapped, len);
}

/********** push_scanline_swapped_xy ********
 *
 * Push a partial scanline to the display from pre-swapped data.
 *
 * Parameters:
 *      x0:           starting x-coordinate
 *      y:            scanline y-coordinate
 *      line_swapped: pointer to byte-swapped RGB565 pixel data
 *      len:          number of pixels to transfer
 *
 * Notes:
 *      The input data must already be byte-swapped for the ST7789.
 *
 ************************/
void push_scanline_swapped_xy(uint16_t x0, uint16_t y,
                              const uint16_t *line_swapped, uint16_t len)
{
        if (y >= SCREEN_HEIGHT) {
                return;
        }
        if (x0 >= SCREEN_WIDTH) {
                return;
        }
        if (len == 0) {
                return;
        }
        if ((uint32_t)x0 + (uint32_t)len > SCREEN_WIDTH) {
                len = (uint16_t)(SCREEN_WIDTH - x0);
        }

        set_address_window(x0, y, (uint16_t)(x0 + len - 1), y);
        start_display_transfer(line_swapped, len);
}

/********** push_rect_fb ********
 *
 * Push a rectangular region from the framebuffer to the display.
 *
 * Parameters:
 *      x, y: top-left corner in pixels
 *      w, h: width and height in pixels
 *
 * Notes:
 *      The region is clipped to display bounds.
 *      Data is sent row by row from the framebuffer.
 *
 ************************/
void push_rect_fb(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
        if (w == 0 || h == 0) {
                return;
        }
        if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) {
                return;
        }

        if ((uint32_t)x + (uint32_t)w > SCREEN_WIDTH) {
                w = (uint16_t)(SCREEN_WIDTH - x);
        }
        if ((uint32_t)y + (uint32_t)h > SCREEN_HEIGHT) {
                h = (uint16_t)(SCREEN_HEIGHT - y);
        }

        set_address_window(x, y, (uint16_t)(x + w - 1), (uint16_t)(y + h - 1));

        for (uint16_t row = 0; row < h; row++) {
                const uint16_t *row_ptr;

                row_ptr = &framebuffer[(y + row) * SCREEN_WIDTH + x];
                start_display_transfer(row_ptr, w);
        }
}

