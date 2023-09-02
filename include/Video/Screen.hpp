#pragma once

#include <string>

#include <stdint.h>
#include <linux/fb.h>

#define BGR2RGB(color) ({ \
__typeof__ (color) _color = color;\
((_color & 0x000000FF) << 16) | (color & 0x0000FF00) | ((color & 0x00FF0000) >> 16);\
})

#include <stdexcept>

namespace Video {
    class Screen {
        public:

        typedef enum {
            SCREEN_RGB = 0,
            SCREEN_BGR = 1,
        } PixelFormat;

        Screen(const std::string& path);
        ~Screen();

        uint8_t* buffer = nullptr;
        size_t screen_buffer_size = 0;
        struct fb_var_screeninfo info = {};
        PixelFormat pixel_format;
        int fd = -1;
    };
}