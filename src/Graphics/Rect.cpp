#include "Graphics/Pixel.hpp"
#include "Graphics/Rect.hpp"

void Graphics::DrawFillRect(Video::BackFramebuffer& framebuffer, size_t x, size_t y, size_t width, size_t height, size_t color) {
    size_t end_x = x + width;
    size_t end_y = y + height;
    
    for(size_t i = y; i < end_y; i++)
        for(size_t j = x; j < end_x; j++)
            DrawPixel(framebuffer, j, i, color);
}

void Graphics::DrawRect(Video::BackFramebuffer& framebuffer, size_t x, size_t y, size_t width, size_t height, size_t color) {
    size_t end_x = x + width;
    size_t end_y = y + height;
    size_t i = y;

    for(; i < end_y; i++) {
        DrawPixel(framebuffer, x, i, color);
        DrawPixel(framebuffer, end_x, i, color);
    }

    for(i = x; i < end_x; i++) {
        DrawPixel(framebuffer, i, y, color);
        DrawPixel(framebuffer, i, end_y, color);
    }
}