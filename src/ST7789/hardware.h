#ifndef HARDWARE_H
#define HARDWARE_H

#include "pico/stdlib.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

void spi_write_command(uint8_t cmd);
void spi_write_data(uint8_t *data, size_t length);
void set_address_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void display_spi_init();
void st7789_init();
void gpio_pin_init();
void display_dma_init();
void start_display_transfer(uint16_t* data_buffer, size_t num_pixels);

#endif
