#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "hardware/dma.h"
#include "pico/time.h"

// Display dimensions
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

// Define GPIO pins 
#define PIN_CS   17
#define PIN_DC   16
#define PIN_RST  22
#define PIN_SCK  18
#define PIN_MOSI 19
#define PIN_BL   20
#define SPI_PORT spi0

// ST7789 commands
#define ST7789_SWRST  0x01
#define ST7789_SLPOUT 0x11
#define ST7789_NORON  0x13
#define ST7789_INVOFF 0x20
#define ST7789_INVON  0x21
#define ST7789_DISPOFF 0x28
#define ST7789_DISPON 0x29
#define ST7789_CASET  0x2A
#define ST7789_RASET  0x2B
#define ST7789_RAMWR  0x2C
#define ST7789_COLMOD 0x3A
#define ST7789_MADCTL 0x36



int dma_channel;


void spi_write_command(uint8_t cmd) {
    gpio_put(PIN_DC, 0); 
    gpio_put(PIN_CS, 0); 
    spi_write_blocking(spi0, &cmd, 1);
    gpio_put(PIN_CS, 1); 
}

void spi_write_data(uint8_t *data, size_t length) {
    gpio_put(PIN_DC, 1); 
    gpio_put(PIN_CS, 0); 
    spi_write_blocking(SPI_PORT, data, length);
    gpio_put(PIN_CS, 1); 
}


void set_address_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    uint8_t data[4];

    spi_write_command(ST7789_CASET); // Column address
    data[0] = (x0 >> 8) & 0xFF;
    data[1] = x0 & 0xFF;
    data[2] = (x1 >> 8) & 0xFF;
    data[3] = x1 & 0xFF;
    spi_write_data(data, 4);

    spi_write_command(ST7789_RASET); // Row address
    data[0] = (y0 >> 8) & 0xFF;
    data[1] = y0 & 0xFF;
    data[2] = (y1 >> 8) & 0xFF;
    data[3] = y1 & 0xFF;
    spi_write_data(data, 4);

    spi_write_command(ST7789_RAMWR); // Write to RAM
}


void display_spi_init() {
    spi_inst_t *spi = spi0; 
    uint baudrate = 1000 * 1000 * 64; 

    spi_init(spi, baudrate);

    spi_set_format(
        spi,
        8,          
        0,          
        0,          
        SPI_MSB_FIRST 
    );

    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    

}

void display_dma_init() {
    dma_channel = dma_claim_unused_channel(true);

    dma_channel_config c = dma_channel_get_default_config(dma_channel);

    channel_config_set_transfer_data_size(&c, DMA_SIZE_8);
    channel_config_set_dreq(&c, DREQ_SPI0_TX);  
    channel_config_set_read_increment(&c, true);
    channel_config_set_write_increment(&c, false);
    

    dma_channel_configure(
        dma_channel,
        &c,
        &spi_get_hw(SPI_PORT)->dr,             
        NULL,                      
        0,                         
        false                      
    );
}

void start_display_transfer(uint16_t* data_buffer, size_t num_pixels) {
    gpio_put(PIN_CS, 0);  
    gpio_put(PIN_DC, 1); 

    dma_channel_wait_for_finish_blocking(dma_channel);

    dma_channel_set_read_addr(dma_channel, data_buffer, true);
    dma_channel_set_trans_count(dma_channel, num_pixels * 2, true);

    dma_channel_wait_for_finish_blocking(dma_channel);

    while (spi_is_busy(SPI_PORT)) {
        tight_loop_contents();
    }
    

    gpio_put(PIN_CS, 1);
}



void st7789_init() {
    uint8_t data[1];
        
    gpio_put(PIN_RST, 0);
    sleep_ms(20);
    gpio_put(PIN_RST, 1);
    sleep_ms(150);

    spi_write_command(ST7789_SWRST); 
    sleep_ms(150);

    spi_write_command(ST7789_SLPOUT); 
    sleep_ms(500);

    spi_write_command(ST7789_COLMOD);
    data[0] = 0x55;
    spi_write_data(data, 1); 
    sleep_ms(10);

    spi_write_command(ST7789_MADCTL); 
    data[0] = 0x60;
    spi_write_data(data, 1); 

    spi_write_command(ST7789_INVON); 

    spi_write_command(ST7789_DISPON); 
    sleep_ms(100);


}

void gpio_pin_init() {
    gpio_init(PIN_DC);
    gpio_set_dir(PIN_DC, GPIO_OUT);
    gpio_put(PIN_DC, 1);

    gpio_init(PIN_RST);
    gpio_set_dir(PIN_RST, GPIO_OUT);
    gpio_put(PIN_RST, 1);

    gpio_init(PIN_BL);
    gpio_set_dir(PIN_BL, GPIO_OUT);
    gpio_put(PIN_BL, 1);

    gpio_init(PIN_CS);
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1);
}
