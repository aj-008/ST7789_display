#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "pico/time.h"

// RGB LED pins
#define LED_R 6
#define LED_G 7
#define LED_B 8

// Button pins
#define BUTTON_A 12
#define BUTTON_B 13
#define BUTTON_X 14
#define BUTTON_Y 15

// Display dimensions
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 340

// Define GPIO pins 
#define PIN_CS   17
#define PIN_DC   16
#define PIN_RST  22
#define PIN_SCK  18
#define PIN_MOSI 19
#define PIN_BL   20
#define SPI_PORT        spi0

// ST7789 commands
#define ST7789_SWRST    0x01
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

    // Set the drawing window to the single pixel location
    set_address_window(x, y, x, y);

    // The ST7789 expects big-endian RGB565 data.
    // The Pico is little-endian, so we need to swap bytes.
    uint8_t high_byte = (color >> 8) & 0xFF;
    uint8_t low_byte = color & 0xFF;
    uint8_t pixel_bytes[2] = {high_byte, low_byte}; // Correct byte order for the display

    // Send the 16-bit color data
    spi_write_data(pixel_bytes, 2);
}



void display_spi_init() {
    spi_inst_t *spi = spi0; 
    uint baudrate = 1000 * 1000; 

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
    
    gpio_init(PIN_CS);
    gpio_set_dir(PIN_CS, GPIO_OUT);
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
    data[0] = 0x00;
    spi_write_data(data, 1); 

    spi_write_command(ST7789_INVON); 


    spi_write_command(ST7789_DISPON); 
    sleep_ms(100);

    gpio_init(PIN_BL);
    gpio_set_dir(PIN_BL, GPIO_OUT);
    gpio_put(PIN_BL, 1);
}

int main() { 

    display_spi_init();

    gpio_init(PIN_DC);
    gpio_set_dir(PIN_DC, GPIO_OUT);
    gpio_put(PIN_DC, 1);

    gpio_init(PIN_RST);
    gpio_set_dir(PIN_RST, GPIO_OUT);
    gpio_put(PIN_RST, 1);


    gpio_init(LED_R);
    gpio_set_dir(LED_R, GPIO_OUT);
    gpio_put(LED_R, 1);  
    
    gpio_init(LED_G);
    gpio_set_dir(LED_G, GPIO_OUT);
    gpio_put(LED_G, 1);  
    
    gpio_init(LED_B);
    gpio_set_dir(LED_B, GPIO_OUT);
    gpio_put(LED_B, 1);  
    
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);
    
    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);
    
    gpio_init(BUTTON_X);
    gpio_set_dir(BUTTON_X, GPIO_IN);
    gpio_pull_up(BUTTON_X);
    
    gpio_init(BUTTON_Y);
    gpio_set_dir(BUTTON_Y, GPIO_IN);
    gpio_pull_up(BUTTON_Y);


    st7789_init();


    uint16_t red = color565(255, 0, 0);
    for (uint32_t i = 0; i < SCREEN_HEIGHT; i++) {
        for (uint32_t j = 0; j < SCREEN_WIDTH; j++) {
            draw_pixel(j, i, red);
        }
    }
}
