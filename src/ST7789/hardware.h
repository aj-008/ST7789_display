/**************************************************************
 *
 *                          hardware.h
 *
 *     Author:  AJ Romeo
 *
 *     Public interface for the ST7789 display driver hardware layer.
 *
 **************************************************************/

#ifndef HARDWARE_H
#define HARDWARE_H

#include <stddef.h>
#include <stdint.h>
#include "../display_config.h"

/*
 * Pin defaults (Pico SDK GPIO numbers).
 * Override at build time, for example: -DST7789_PIN_CS=5
 */
#ifndef ST7789_PIN_SCK
#define ST7789_PIN_SCK  18
#endif

#ifndef ST7789_PIN_MOSI
#define ST7789_PIN_MOSI 19
#endif

#ifndef ST7789_PIN_CS
#define ST7789_PIN_CS   17
#endif

#ifndef ST7789_PIN_DC
#define ST7789_PIN_DC   16
#endif

#ifndef ST7789_PIN_RST
#define ST7789_PIN_RST  20
#endif

#ifndef ST7789_PIN_BL
#define ST7789_PIN_BL   21
#endif

#ifndef ST7789_SPI_PORT
#define ST7789_SPI_PORT spi0
#endif

void display_spi_init(void);

void gpio_pin_init(void);

void display_dma_init(void);

void st7789_init(void);

void set_address_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

void start_display_transfer(const uint16_t *data, size_t count);

#endif

