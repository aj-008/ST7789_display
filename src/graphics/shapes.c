#include "shapes.h"
#include "util.h"
#include <stdlib.h>


void draw_circle(uint16_t xc, uint16_t yc, uint16_t r, uint16_t color) {
    int16_t d, dx, dy, x, y;

	d = 1 - r, dx = 1, dy = -2 * r, x = 0, y = r;
	draw_pixel(xc, yc + r, color);
	draw_pixel(xc, yc - r, color);
	draw_pixel(xc + r, yc, color);
	draw_pixel(xc - r, yc, color);
	while (x < y) {
		if (d >= 0) {
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


/*
 * 1 ---------- 2
 * |            |
 * |            |
 * 4 ---------- 3
 */
void draw_corner_round(uint16_t x0, uint16_t y0, uint16_t r, uint16_t corner, uint16_t color) {
	int16_t f, dx, dy, x, y;
	f = 1 - r, dx = 1, dy = -2 * r, x = 0, y = r;
	while (x < y)
	{
		if (f >= 0)
		{
			y--;
			dy += 2;
			f += dy;
		}
		x++;
		dx += 2;
		f += dx;
        if (corner == 1) {
            draw_pixel(x0 + x, y0 + y, color);
            draw_pixel(x0 + y, y0 + x, color);
        }
        if (corner == 2) {
            draw_pixel(x0 + x, y0 - y, color);
            draw_pixel(x0 + y, y0 - x, color);
        }
        if (corner == 3) {
            draw_pixel(x0 - y, y0 + x, color);
            draw_pixel(x0 - x, y0 + y, color);
        }
        if (corner == 4) {
            draw_pixel(x0 - y, y0 - x, color);
            draw_pixel(x0 - x, y0 - y, color);
        }
    }
}

// x and y are center coords
void fill_circle(uint16_t xc, uint16_t yc, uint16_t r, uint16_t color) {
    int16_t x = 0;
    int16_t y = r;
    int16_t d = 1 - r;
    int16_t dx = 1;
    int16_t dy = -2 * r;
    
    draw_hline(xc - r, yc, 2 * r + 1, color);
    
    while (x < y) {
        if (d >= 0) {
            y--;
            dy += 2;
            d += dy;
        }
        x++;
        dx += 2;
        d += dx;
        
        draw_hline(xc - x, yc + y, 2 * x + 1, color);
        draw_hline(xc - x, yc - y, 2 * x + 1, color);
        draw_hline(xc - y, yc + x, 2 * y + 1, color);
        draw_hline(xc - y, yc - x, 2 * y + 1, color);
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


void draw_vline(uint16_t x, uint16_t y, uint16_t length, uint16_t color) {
    for (uint16_t i = y; i < y + length; i++) {
        draw_pixel(x, i, color);
    }
}

void draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
    int16_t dx = abs((int16_t)x1 - (int16_t)x0);
    int16_t dy = abs((int16_t)y1 - (int16_t)y0);
    int16_t sx = x0 < x1 ? 1 : -1;  
    int16_t sy = y0 < y1 ? 1 : -1;
    int16_t err = dx - dy;
    
    int16_t x = x0;
    int16_t y = y0;
    
    while (true) {
        draw_pixel(x, y, color);
        
        if (x == x1 && y == y1) break;
        
        int16_t e2 = 2 * err;
        
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        
        if (e2 < dx) {
            err += dx;
            y += sy;
        }
    }
}



void draw_rounded_rec(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint16_t color) {
	draw_hline(x + r, y, w - 2 * r, color);
	draw_hline(x + r, y + h - 1, w - 2 * r, color);
	draw_vline(x, y + r, h - 2 * r, color);
	draw_vline(x + w - 1, y + r, h - 2 * r, color);
	draw_corner_round(x + r, y + r, r, 4, color);
	draw_corner_round(x + w - r - 1, y + r, r, 2, color);
	draw_corner_round(x + w - r - 1, y + h - r - 1, r, 1, color);
	draw_corner_round(x + r, y + h - r - 1, r, 3, color);
}

