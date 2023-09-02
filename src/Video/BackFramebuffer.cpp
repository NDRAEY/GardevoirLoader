#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>

#include "Video/BackFramebuffer.hpp"

using namespace Video;

BackFramebuffer::BackFramebuffer(Screen& screen)
: screen(screen), size(screen.screen_buffer_size) {
    this->buffer = new uint8_t[this->size];

    memset(this->buffer, 0, this->size);
}

void BackFramebuffer::flush() {
    memcpy(this->screen.buffer, this->buffer, this->size);
}

BackFramebuffer::~BackFramebuffer() {
    delete[] this->buffer;
}