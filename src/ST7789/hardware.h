#ifndef HARDWARE_H
#define HARDWARE_H

#include "pico/stdlib.h"

void spi_write_command(uint8_t cmd);
void spi_write_data(uint8_t *data, size_t length);
void set_address_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
uint16_t color565(uint8_t r, uint8_t g, uint8_t b);
void draw_pixel(uint16_t x, uint16_t y, uint16_t color);
void display_spi_init();
void st7789_init();
void gpio_pin_init();

#endif
