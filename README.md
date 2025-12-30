# ST7789_display

C driver + small graphics helpers for an ST7789 display, targeting the Raspberry Pi Pico SDK.

This project assumes `PICO_SDK_PATH` is set.



## Configuration

Defaults live in `src/display_config.h` and `src/ST7789/hardware.h`.


## API overview

Display API (ST7789 + Pico SPI/DMA) is in `src/ST7789/`:

- `display_spi_init()`
- `gpio_pin_init()`
- `display_dma_init()`
- `st7789_init()`
- `set_address_window(...)`
- `start_display_transfer(...)`

Graphics helpers are in `src/graphics/`:

- `color565(r,g,b)`
- `draw_pixel(x,y,color)` (framebuffer only)
- `fill_screen(color)` (fill + present)
- `present_full()` / `present_rect(...)`

**Framebuffer note:** the framebuffer is stored as byte-swapped RGB565 so it can be DMA'd directly to the ST7789.

## License

MIT (see `LICENSE`).
