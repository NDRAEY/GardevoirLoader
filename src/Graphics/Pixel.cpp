#include <stdio.h>
#include <iostream>
#include <string>
#include "Helpers.hpp"

#include "Graphics/Pixel.hpp"

void Graphics::DrawPixel(Video::BackFramebuffer& fb, size_t x, size_t y, uint32_t color) {
    if(x >= fb.screen.info.width || y >= fb.screen.info.height)
        return;

    uint8_t color_a = (color >> 24) & 0xFF;
    uint8_t color_r = (color >> 16) & 0xFF;
    uint8_t color_g = (color >> 8 ) & 0xFF;
    uint8_t color_b = (color >> 0 ) & 0xFF;

    if(fb.screen.info.bits_per_pixel == 32) {
        uint32_t result_color = (color_a << (/*16 - */fb.screen.info.transp.offset))
                              | (color_r << (16 - fb.screen.info.red.offset))
                              | (color_g << (16 - fb.screen.info.green.offset))
                              | (color_b << (16 - fb.screen.info.blue.offset));
    
        ((uint32_t*)fb.buffer)[y * fb.screen.info.width + x] = result_color;
    } else {
        throw std::runtime_error("BPP " + std::to_string(fb.screen.info.bits_per_pixel) + "is not supported!");
    }
}