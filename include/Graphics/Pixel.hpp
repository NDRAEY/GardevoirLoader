#include "Video/BackFramebuffer.hpp"

namespace Graphics {
    // Draws a pixel onto framebuffer. Color should be in ARGB format.
    void DrawPixel(Video::BackFramebuffer& fb, size_t x, size_t y, uint32_t color);
}