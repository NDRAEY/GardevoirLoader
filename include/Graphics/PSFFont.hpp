#pragma once

#include <stdint.h>
#include <string>
#include <fstream>

#include "Video/BackFramebuffer.hpp"

#define PSF_V1 0x01
#define PSF_V2 0x02

#define PSF1_MODE512    0X01
#define PSF1_MODEHASTAB 0X02
#define PSF1_MODEHASSEQ 0X04
#define PSF1_MAXMODE    0X05

#define PSF1_SEPARATOR  0XFFFF
#define PSF1_STARTSEQ   0XFFFE

#define PSF1_MAGIC0     0X36
#define PSF1_MAGIC1     0X04

namespace Graphics {
    class PSFFont {
        public:
    
        typedef struct {
            uint8_t magic[2];
            uint8_t mode;
            uint8_t charHeight;
        } __attribute__((packed)) psf_header_t;
    
        PSFFont(const std::string& path);
        ~PSFFont();

        void draw_glyph(Video::BackFramebuffer& framebuffer, uint16_t glyph, size_t x, size_t y, size_t color);
        void draw_string(Video::BackFramebuffer& framebuffer, const std::string& string, size_t x, size_t y, size_t color);
		void draw_string(Video::BackFramebuffer& framebuffer, const char* string, size_t x, size_t y, size_t color);

        size_t get_glyph_width() const { return glyph_width; }
        size_t get_glyph_height() const { return glyph_height; }

        private:
        
        uint8_t* pixels = nullptr;
        std::fstream stream;

        const size_t glyph_width = 8;
        size_t glyph_height = 0;
    };
}


// void draw_vga_ch(uint16_t c, uint16_t c2, int pos_x, int pos_y, int color);
// void draw_vga_str(const char* text, size_t len, int x, int y, int color);
// size_t psf1_get_w();
// size_t psf1_get_h();