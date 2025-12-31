/**************************************************************
 *
 *                          shapes.c
 *
 *     Author:  AJ Romeo
 *
 *     Implementations of basic 2D shapes (lines, circles, rectangles).
 *
 **************************************************************/

#include "shapes.h"
#include "util.h"
#include <stdlib.h>

/********** draw_circle ********
 *
 * Draw a circle outline on the framebuffer.
 *
 * Parameters:
 *      xc, yc: center coordinates
 *      r:      radius in pixels
 *      color:  line color in native-endian RGB565
 *
 ************************/
void draw_circle(uint16_t xc, uint16_t yc, uint16_t r, uint16_t color)
{
        int16_t d = 1 - r;
        int16_t dx = 1;
        int16_t dy = -2 * r;
        int16_t x = 0;
        int16_t y = r;

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

/********** draw_corner_round ********
 *
 * Draw one quadrant of a circle for rounded rectangle corners.
 *
 * Parameters:
 *      x0, y0: center of the corner arc
 *      r:      radius in pixels
 *      corner: which corner (1-4, clockwise from top-left)
 *      color:  line color in native-endian RGB565
 *
 ************************/
static void draw_corner_round(uint16_t x0, uint16_t y0, uint16_t r,
                              uint16_t corner, uint16_t color)
{
        int16_t f = 1 - r;
        int16_t dx = 1;
        int16_t dy = -2 * r;
        int16_t x = 0;
        int16_t y = r;

        while (x < y) {
                if (f >= 0) {
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

/********** fill_circle ********
 *
 * Draw a filled circle on the framebuffer.
 *
 * Parameters:
 *      xc, yc: center coordinates
 *      r:      radius in pixels
 *      color:  fill color in native-endian RGB565
 *
 ************************/
void fill_circle(uint16_t xc, uint16_t yc, uint16_t r, uint16_t color)
{
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

/********** draw_rectangle ********
 *
 * Draw a rectangle outline on the framebuffer.
 *
 * Parameters:
 *      x, y:  top-left corner
 *      w, h:  width and height in pixels
 *      color: line color in native-endian RGB565
 *
 ************************/
void draw_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                    uint16_t color)
{
        for (uint16_t i = 0; i < w; i++) {
                draw_pixel(x + i, y, color);
                draw_pixel(x + i, y + h, color);
        }
        for (uint16_t i = 0; i < h; i++) {
                draw_pixel(x, y + i, color);
                draw_pixel(x + w, y + i, color);
        }
}

/********** fill_rectangle ********
 *
 * Draw a filled rectangle on the framebuffer.
 *
 * Parameters:
 *      x, y:  top-left corner
 *      w, h:  width and height in pixels
 *      color: fill color in native-endian RGB565
 *
 ************************/
void fill_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                    uint16_t color)
{
        for (uint16_t i = x; i < w + x; i++) {
                for (uint16_t j = y; j < h + y; j++) {
                        draw_pixel(i, j, color);
                }
        }
}

/********** draw_hline ********
 *
 * Draw a horizontal line on the framebuffer.
 *
 * Parameters:
 *      x, y:   starting point
 *      length: line length in pixels
 *      color:  line color in native-endian RGB565
 *
 ************************/
void draw_hline(uint16_t x, uint16_t y, uint16_t length, uint16_t color)
{
        for (uint16_t i = x; i < x + length; i++) {
                draw_pixel(i, y, color);
        }
}

/********** draw_vline ********
 *
 * Draw a vertical line on the framebuffer.
 *
 * Parameters:
 *      x, y:   starting point
 *      length: line length in pixels
 *      color:  line color in native-endian RGB565
 *
 ************************/
void draw_vline(uint16_t x, uint16_t y, uint16_t length, uint16_t color)
{
        for (uint16_t i = y; i < y + length; i++) {
                draw_pixel(x, i, color);
        }
}

/********** draw_line ********
 *
 * Draw a line segment on the framebuffer using integer arithmetic.
 *
 * Parameters:
 *      x0, y0: start point
 *      x1, y1: end point
 *      color:  line color in native-endian RGB565
 *
 * Notes:
 *      Uses a Bresenham-style algorithm.
 *
 ************************/
void draw_line(int x0, int y0, int x1, int y1, uint16_t color)
{
        int16_t dx = abs((int16_t)x1 - (int16_t)x0);
        int16_t dy = abs((int16_t)y1 - (int16_t)y0);
        int16_t sx = x0 < x1 ? 1 : -1;
        int16_t sy = y0 < y1 ? 1 : -1;
        int16_t err = dx - dy;

        int16_t x = x0;
        int16_t y = y0;

        while (true) {
                draw_pixel(x, y, color);

                if (x == x1 && y == y1) {
                        break;
                }

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

/********** draw_rounded_rec ********
 *
 * Draw an outline rectangle with rounded corners.
 *
 * Parameters:
 *      x, y:   top-left corner
 *      w, h:   width and height in pixels
 *      radius: corner radius in pixels
 *      color:  outline color in native-endian RGB565
 *
 ************************/
void draw_rounded_rec(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                      uint16_t radius, uint16_t color)
{
        draw_hline(x + radius, y, w - 2 * radius, color);
        draw_hline(x + radius, y + h - 1, w - 2 * radius, color);
        draw_vline(x, y + radius, h - 2 * radius, color);
        draw_vline(x + w - 1, y + radius, h - 2 * radius, color);
        draw_corner_round(x + radius, y + radius, radius, 4, color);
        draw_corner_round(x + w - radius - 1, y + radius, radius, 2, color);
        draw_corner_round(x + w - radius - 1, y + h - radius - 1, 
                          radius, 1, color);
        draw_corner_round(x + radius, y + h - radius - 1, radius, 3, color);
}

