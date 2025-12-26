#include "ST7789/hardware.h"
#include "graphics/util.h"
#include "graphics/text.h"
#include "graphics/shapes.h"
#include "graphics/image.h"


int main() { 

    display_spi_init();
    display_dma_init();

    gpio_pin_init();

    st7789_init();

    uint16_t black = color565(0, 0, 0);
    uint16_t red = color565(255, 0, 0);

    fill_screen(black);

    //draw_line(0, 0, 170, 120, red);

    /*fill_rectangle(50, 95, 150, 50, red);

    fill_circle(50, 95, 25, red);
    fill_circle(50, 140, 25, red);

    fill_circle(200, 119, 25, red);

    sleep_ms(500);

    draw_hline(235, 120, 10, red);

    sleep_ms(500);

    draw_hline(250, 120, 10, red);

    sleep_ms(500);

    draw_hline(265, 120, 10, red);

    sleep_ms(500);

    draw_text(0, 30, 16, red, "penis.");

    sleep_ms(500);*/

    draw_text(0, 30, 16, red, "IM A GOOFY GOOBER, YEAH!!");

    //draw_bmp(0, 0, midnight_bmp, 90);
}
