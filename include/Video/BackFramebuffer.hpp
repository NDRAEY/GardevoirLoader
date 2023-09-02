#pragma once

#include <stdint.h>
#include "Video/Screen.hpp"

namespace Video {
    class BackFramebuffer {
        public:
        
        BackFramebuffer(Screen& screen);
        ~BackFramebuffer();

        void flush();
        
        Screen& screen;
        std::size_t size = 0;
        uint8_t* buffer = nullptr;
    };
}