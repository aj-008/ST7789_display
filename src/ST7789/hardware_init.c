/**************************************************************
 *
 *                     hardware_init.c
 *
 *     Assignment: ST7789_display
 *     Author:    AJ Romeo
 *     Date:      December 30, 2025
 *
 *     ST7789 SPI transport and initialization for RP2040 + Pico SDK.
 *
 **************************************************************/

#include "hardware.h"

#include "hardware/dma.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"
#include "pico/time.h"

/*ST7789 commands*/
#define ST7789_SWRST  0x01
#define ST7789_SLPOUT 0x11
#define ST7789_INVON  0x21
#define ST7789_DISPON 0x29
#define ST7789_CASET  0x2A
#define ST7789_RASET  0x2B
#define ST7789_RAMWR  0x2C
#define ST7789_MADCTL 0x36
#define ST7789_COLMOD 0x3A

static int dma_channel = -1;

void spi_write_command(uint8_t cmd)
{
        gpio_put(ST7789_PIN_DC, 0);
        gpio_put(ST7789_PIN_CS, 0);
        spi_write_blocking(ST7789_SPI_PORT, &cmd, 1);
        gpio_put(ST7789_PIN_CS, 1);
}

void spi_write_data(const uint8_t *data, size_t length)
{
        if (length == 0) return;

        gpio_put(ST7789_PIN_DC, 1);
        gpio_put(ST7789_PIN_CS, 0);
        spi_write_blocking(ST7789_SPI_PORT, data, length);
        gpio_put(ST7789_PIN_CS, 1);
}

/********** set_address_window ********
 *
 * Set the ST7789 column and row address window.
 *
 * Parameters:
 *      x0, y0: top-left corner (inclusive)
 *      x1, y1: bottom-right corner (inclusive)
 *
 ************************/
void set_address_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
        uint8_t data[4];

        spi_write_command(ST7789_CASET);
        data[0] = (uint8_t)((x0 >> 8) & 0xFF);
        data[1] = (uint8_t)(x0 & 0xFF);
        data[2] = (uint8_t)((x1 >> 8) & 0xFF);
        data[3] = (uint8_t)(x1 & 0xFF);
        spi_write_data(data, 4);

        spi_write_command(ST7789_RASET);
        data[0] = (uint8_t)((y0 >> 8) & 0xFF);
        data[1] = (uint8_t)(y0 & 0xFF);
        data[2] = (uint8_t)((y1 >> 8) & 0xFF);
        data[3] = (uint8_t)(y1 & 0xFF);
        spi_write_data(data, 4);

        spi_write_command(ST7789_RAMWR);
}

void display_spi_init(void)
{
        const uint baudrate = 1000u * 1000u * 64u;

        spi_init(ST7789_SPI_PORT, baudrate);
        spi_set_format(ST7789_SPI_PORT, 8, 0, 0, SPI_MSB_FIRST);

        gpio_set_function(ST7789_PIN_SCK, GPIO_FUNC_SPI);
        gpio_set_function(ST7789_PIN_MOSI, GPIO_FUNC_SPI);
}

void display_dma_init(void)
{
        dma_channel = dma_claim_unused_channel(true);

        dma_channel_config c = dma_channel_get_default_config(dma_channel);
        channel_config_set_transfer_data_size(&c, DMA_SIZE_8);
        channel_config_set_dreq(&c, DREQ_SPI0_TX);
        channel_config_set_read_increment(&c, true);
        channel_config_set_write_increment(&c, false);

        dma_channel_configure(
        dma_channel,
        &c,
        &spi_get_hw(ST7789_SPI_PORT)->dr,
        NULL,
        0,
        false
        );
}

/********** start_display_transfer ********
 *
 * Begin a DMA-backed SPI transfer of pixel data to the display.
 *
 * Parameters:
 *      data:   pointer to RGB565 pixel data (byte order must match the driver)
 *      count:  number of pixels to transfer
 *
 * Notes:
 *      The caller must set an appropriate address window before calling.
 *
 ************************/

void start_display_transfer(const uint16_t *data_buffer, size_t num_pixels)
{
        if (num_pixels == 0) return;
        if (dma_channel < 0) return;

        gpio_put(ST7789_PIN_CS, 0);
        gpio_put(ST7789_PIN_DC, 1);

        dma_channel_wait_for_finish_blocking(dma_channel);

        dma_channel_set_read_addr(dma_channel, data_buffer, false);
        dma_channel_set_trans_count(dma_channel, num_pixels * 2u, true);

        dma_channel_wait_for_finish_blocking(dma_channel);

        while (spi_is_busy(ST7789_SPI_PORT)) {
                tight_loop_contents();
        }

        gpio_put(ST7789_PIN_CS, 1);
}


/********** st7789_init ********
 *
 * Initialize SPI, GPIO, DMA, and the ST7789 controller.
 *
 * Notes:
 *      Must be called before any drawing is presented to the display.
 *      Uses the pin configuration in hardware.h (overridable at build time).
 *
 ************************/

void st7789_init(void)
{
        uint8_t data[1];

        gpio_put(ST7789_PIN_RST, 0);
        sleep_ms(20);
        gpio_put(ST7789_PIN_RST, 1);
        sleep_ms(150);

        spi_write_command(ST7789_SWRST);
        sleep_ms(150);

        spi_write_command(ST7789_SLPOUT);
        sleep_ms(500);

        spi_write_command(ST7789_COLMOD);
        data[0] = 0x55; /* 16bpp (RGB565) */
        spi_write_data(data, 1);
        sleep_ms(10);

        spi_write_command(ST7789_MADCTL);
        data[0] = 0x60; /* row/col order + RGB/BGR */
        spi_write_data(data, 1);

        spi_write_command(ST7789_INVON);

        spi_write_command(ST7789_DISPON);
        sleep_ms(100);
}

void gpio_pin_init(void)
{
        gpio_init(ST7789_PIN_DC);
        gpio_set_dir(ST7789_PIN_DC, GPIO_OUT);
        gpio_put(ST7789_PIN_DC, 1);

        gpio_init(ST7789_PIN_RST);
        gpio_set_dir(ST7789_PIN_RST, GPIO_OUT);
        gpio_put(ST7789_PIN_RST, 1);

        gpio_init(ST7789_PIN_BL);
        gpio_set_dir(ST7789_PIN_BL, GPIO_OUT);
        gpio_put(ST7789_PIN_BL, 1);

        gpio_init(ST7789_PIN_CS);
        gpio_set_dir(ST7789_PIN_CS, GPIO_OUT);
        gpio_put(ST7789_PIN_CS, 1);
}
