#include "bitmap.h"
#include "pico/stdio.h"
#include <stdio.h> 
#include "util.h"


bool parse_bmp_header(uint8_t *bmp_data, BMP_Header *header) {

    if (bmp_data[0] != 'B' || bmp_data[1] != 'M') {
        return false;
    }
    
    header->data_offset = bmp_data[10] | (bmp_data[11] << 8) | 
                         (bmp_data[12] << 16) | (bmp_data[13] << 24);
    
    header->width = bmp_data[18] | (bmp_data[19] << 8) | 
                   (bmp_data[20] << 16) | (bmp_data[21] << 24);
    
    header->height = bmp_data[22] | (bmp_data[23] << 8) | 
                    (bmp_data[24] << 16) | (bmp_data[25] << 24);
    
    header->bpp = bmp_data[28] | (bmp_data[29] << 8);


    return true;
}

uint16_t rgb888_to_rgb565(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

void draw_bmp(uint16_t x0, uint16_t y0, uint8_t *bmp_data) {

    BMP_Header header;

    if (!parse_bmp_header(bmp_data, &header)) {
        return;  
    }

    if (header.bpp != 24) {
        return;
    }
    
    uint8_t *pixel_data = bmp_data + header.data_offset;
    uint16_t row_size = ((header.width * 3 + 3) / 4) * 4;  
    
    for (int16_t y = header.height - 1; y >= 0; y--) {
        for (uint16_t x = 0; x < header.width; x++) {
            uint32_t pixel_offset = y * row_size + x * 3;
            
            uint8_t b = pixel_data[pixel_offset];
            uint8_t g = pixel_data[pixel_offset + 1];
            uint8_t r = pixel_data[pixel_offset + 2];
            
            uint16_t color = rgb888_to_rgb565(r, g, b);
            draw_pixel(x0 + x, y0 + (header.height - 1 - y), color);
        }
    }
}


void draw_bmp_rotated(uint16_t x0, uint16_t y0, uint8_t *bmp_data, uint16_t rotation) {
    BMP_Header header;
    
    if (!parse_bmp_header(bmp_data, &header)) {
        return;
    }
    
    if (header.bpp != 24) {
        return;
    }
    
    uint8_t *pixel_data = bmp_data + header.data_offset;
    uint16_t row_size = ((header.width * 3 + 3) / 4) * 4;
    
    uint16_t display_width = (rotation % 2 == 0) ? header.width : header.height;
    uint16_t display_height = (rotation % 2 == 0) ? header.height : header.width;
    
    for (int16_t y = header.height - 1; y >= 0; y--) {
        for (uint16_t x = 0; x < header.width; x++) {
            uint32_t pixel_offset = y * row_size + x * 3;
            
            uint8_t b = pixel_data[pixel_offset];
            uint8_t g = pixel_data[pixel_offset + 1];
            uint8_t r = pixel_data[pixel_offset + 2];
            
            uint16_t color = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
            
            uint16_t src_x = x;
            uint16_t src_y = header.height - 1 - y;
            
            uint16_t dst_x, dst_y;
            switch(rotation) {
                case 0:  
                    dst_x = src_x;
                    dst_y = src_y;
                    break;
                case 90:  
                    dst_x = src_y;
                    dst_y = header.width - 1 - src_x;
                    break;
                case 180:  
                    dst_x = header.width - 1 - src_x;
                    dst_y = header.height - 1 - src_y;
                    break;
                case 270:  
                    dst_x = header.height - 1 - src_y;
                    dst_y = src_x;
                    break;
                default:
                    dst_x = src_x;
                    dst_y = src_y;
            }
            
            draw_pixel(x0 + dst_x, y0 + dst_y, color);
        }
    }
}
