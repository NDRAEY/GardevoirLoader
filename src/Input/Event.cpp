#include <fcntl.h>
#include <unistd.h>
#include "Input/Event.hpp"

Input::Event::Event(const std::string& path, bool nonblock) {
    auto flags = O_RDONLY;

    if(nonblock)
        flags |= O_NONBLOCK;

    fd = open(path.c_str(), flags);
}

struct input_event Input::Event::get_event() {
    struct input_event ev = {0};

    read(fd, &ev, sizeof(ev));

    return ev;
}

Input::Event::~Event() {
    close(fd);
}