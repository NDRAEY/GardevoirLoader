#pragma once

#include <stdint.h>
#include "Video/BackFramebuffer.hpp"

namespace Graphics {
    void DrawFillRect(Video::BackFramebuffer& framebuffer, size_t x, size_t y, size_t width, size_t height, size_t color);
    void DrawRect(Video::BackFramebuffer& framebuffer, size_t x, size_t y, size_t width, size_t height, size_t color);
}