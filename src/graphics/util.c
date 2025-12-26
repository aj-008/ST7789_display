#include "util.h"
#include "fonts.h"
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
    // Ensure coordinates are within bounds
    if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) {
        return;
    }

    set_address_window(x, y, x, y);

    // The ST7789 expects big-endian RGB565 data.
    // The Pico is little-endian, so we need to swap bytes.
    uint8_t high_byte = (color >> 8) & 0xFF;
    uint8_t low_byte = color & 0xFF;
    uint8_t pixel_bytes[2] = {high_byte, low_byte}; // Correct byte order for the display

    // Send the 16-bit color data
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

void draw_circle(uint16_t xc, uint16_t yc, uint16_t r, uint16_t color) {
    int16_t d, dx, dy, x, y;

	d = 1 - r, dx = 1, dy = -2 * r, x = 0, y = r;
	draw_pixel(xc, yc + r, color);
	draw_pixel(xc, yc - r, color);
	draw_pixel(xc + r, yc, color);
	draw_pixel(xc - r, yc, color);
	while (x < y)
	{
		if (d >= 0)
		{
			y--;
			dy += 2;
			d += dy;
		}
		x++;
		dx += 2;
		d += dx;
		draw_pixel(xc + x, yc + y, color);
		draw_pixel(xc - x, yc + y, color);
		draw_pixel(xc + x, yc - y, color);
		draw_pixel(xc - x, yc - y, color);
		draw_pixel(xc + y, yc + x, color);
		draw_pixel(xc - y, yc + x, color);
		draw_pixel(xc + y, yc - x, color);
		draw_pixel(xc - y, yc - x, color);
	}
    
}

// x and y are center coords
void fill_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color) {
    for (r; r > 0; r--) {
        draw_circle(x, y, r, color);
    }
}


// x and y desigate the top left
void draw_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    for (uint16_t i = 0; i < w; i++) {
        draw_pixel(x + i, y, color);
        draw_pixel(x + i, y + h, color);
    }
    for (uint16_t i = 0; i < h; i++) {
        draw_pixel(x, y + i, color);
        draw_pixel(x + w, y + i, color);
    }
}


void fill_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h,  uint16_t color) {
    for (uint16_t i = x; i < w + x; i++) {
        for (uint16_t j = y; j < h + y; j++) {
            draw_pixel(i, j, color);
        }
    }
}

void draw_hline(uint16_t x, uint16_t y, uint16_t length, uint16_t color) {
    for (uint16_t i = x; i < x + length; i++) {
        draw_pixel(i, y, color);
    }
}


void draw_bitmap(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t color, uint8_t *bitmap) {
    uint16_t bytes_per_row = (w + 7) / 8;
    
    for (uint16_t row = 0; row < h; row++) {
        for (uint16_t col = 0; col < w; col++) {
            uint16_t byte_index = row * bytes_per_row + (col / 8);  
            uint8_t bit_position = col % 8;  
            
            if (bitmap[byte_index] & (1 << bit_position)) {
                draw_pixel(x0 + col, y0 + row, color);
            }
        }
    }
}


void draw_text(uint16_t x, uint16_t y, uint16_t size, uint16_t color, uint8_t *text) {
    uint8_t *draw_char;
    const uint8_t *font;

    uint16_t x_curr = x;
    uint16_t y_curr = y;

    uint16_t target;

    switch (size) {
        case 16:
        default:
              font = CGA16;
    }

    for (uint16_t i = 0; i < strlen(text); i++) {

        if (x_curr >= SCREEN_WIDTH - size) {
            x_curr = x;
            y_curr += size;
        }

        if (y_curr >= SCREEN_HEIGHT - size) {
            return;
        }


        target = (size*size >> 3) * (text[i] - ' ');
        draw_char = font + target;

        draw_bitmap(x_curr, y_curr, size, size, color, draw_char);

        x_curr += size;
    }
}

