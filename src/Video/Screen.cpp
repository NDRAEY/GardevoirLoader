#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>

#include "Video/Screen.hpp"

using namespace Video;

Screen::Screen(const std::string& path) {
    this->fd = open(path.c_str(), O_RDWR);

    if(this->fd < 0)
        throw std::runtime_error("Failed to open framebuffer at: " + path);

    ioctl(this->fd, FBIOGET_VSCREENINFO, &(this->info));

    this->screen_buffer_size = this->info.width * this->info.height * (this->info.bits_per_pixel >> 3);

    this->buffer = (uint8_t*)mmap(0, this->screen_buffer_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if(!this->buffer)
        throw std::runtime_error("mmap() failed!");

    if(this->info.red.offset == 16)
        this->pixel_format = SCREEN_BGR;
    else
        this->pixel_format = SCREEN_RGB;

    // Double sync fixes buffer drawing problem idk why.

    this->info.sync = !this->info.sync;
    ioctl(fd, FBIOPUT_VSCREENINFO, &(this->info));

    this->info.sync = !this->info.sync;
    ioctl(fd, FBIOPUT_VSCREENINFO, &(this->info));
}

Screen::~Screen() {
    munmap((void*)this->buffer, this->screen_buffer_size);
    close(this->fd);
}
