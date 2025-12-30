cmake_minimum_required(VERSION 3.13)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(display C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(display
    src/ST7789/hardware_init.c
    src/graphics/util.c
    src/graphics/text.c
    src/graphics/image.c
    src/graphics/shapes.c
)

target_link_libraries(display 
    pico_stdlib
    hardware_spi
    hardware_dma
)

pico_enable_stdio_usb(display 1)
pico_enable_stdio_uart(display 0)

pico_add_extra_outputs(display)
