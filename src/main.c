#include "ST7789/hardware.h"


#include <string.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

uint16_t framebuffer[SCREEN_WIDTH * SCREEN_HEIGHT];


void clear_screen() {
    uint16_t black = color565(0, 0, 0);
    for (uint32_t i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        framebuffer[i] = black;
    }
    
    set_address_window(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
    
    spi_write_data((uint8_t*)framebuffer, SCREEN_WIDTH * SCREEN_HEIGHT * 2);
}


int main() { 

    display_spi_init();

    gpio_pin_init();

    st7789_init();

    uint16_t red = color565(255, 0, 0);
    for (uint32_t i = 0; i < SCREEN_HEIGHT; i++) {
        for (uint32_t j = 0; j < SCREEN_WIDTH; j++) {
            draw_pixel(j, i, red);
        }
    }

    clear_screen();
}
