#include "ST7789/hardware.h"


#include <string.h>


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



int main() { 

    display_spi_init();
    display_dma_init();

    gpio_pin_init();

    st7789_init();

    uint16_t black = color565(0, 0, 0);
    uint16_t red = color565(255, 0, 0);

    fill_screen(black);

    fill_rectangle(50, 95, 150, 50, red);

    fill_circle(50, 95, 25, red);
    fill_circle(50, 140, 25, red);

    fill_circle(200, 120, 25, red);

    sleep_ms(500);

    draw_hline(235, 120, 10, red);

    sleep_ms(500);

    draw_hline(250, 120, 10, red);

    sleep_ms(500);

    draw_hline(265, 120, 10, red);
}
