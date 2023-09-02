#pragma once

#include <string>
#include <linux/input.h>

namespace Input {
    class Event {
        public:

        Event(const std::string& path, bool nonblock = true);
        ~Event();

        struct input_event get_event();

        private:
        
        int fd = -1;
    };
}