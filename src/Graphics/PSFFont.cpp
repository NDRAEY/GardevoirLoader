// Ported from SayoriOS by NDRAEY
// Original authors below

/**
 * @file drv/psf.c
 * @author Пиминов Никита (nikita.piminoff@yandex.ru), Арен Елчинян (SynapseOS)
 * @brief Драйвер виртуальной файловой системы
 * @version 0.3.3
 * @date 2023-01-13
 * @copyright Copyright SayoriOS Team (c) 2023
 */

#include "Graphics/Pixel.hpp"
#include "Graphics/PSFFont.hpp"

Graphics::PSFFont::PSFFont(const std::string& path) {
    stream.open(path, std::ios::binary | std::ios::in);

    if(stream.fail())
        throw std::runtime_error("Failed to open: " + path);

    stream.seekg(0, std::ios::end);

    size_t filesize = stream.tellg();

    stream.seekg(0);

    pixels = new uint8_t[filesize];

    stream.read((char*)pixels, filesize);

    psf_header_t* header = (psf_header_t*)pixels;

    if(!(header->magic[0] == PSF1_MAGIC0
         && header->magic[1] == PSF1_MAGIC1)) {
        throw std::runtime_error("Not a PSF file!");
    }

    glyph_height = header->charHeight;
}

void Graphics::PSFFont::draw_glyph(Video::BackFramebuffer& framebuffer, uint16_t glyph,
                                   size_t x, size_t y, size_t color) {
    char mask[8] = {128, 64, 32, 16, 8, 4, 2, 1};

    uint8_t* pix = pixels + sizeof(psf_header_t) + (glyph * glyph_height);

    for(size_t sy = 0; sy < glyph_height; sy++){
        for(size_t sx = 0; sx < glyph_width; sx++){
            if (pix[sy] & mask[sx]) {
                Graphics::DrawPixel(framebuffer, sx + x, sy + y, color);
            }
        }
    }
}

void Graphics::PSFFont::draw_string(Video::BackFramebuffer& framebuffer,
                                    const std::string& string,
                                    size_t x, size_t y, size_t color) {
    size_t sx = x;
    size_t sy = y;
    
    for(char i : string) {
        draw_glyph(framebuffer, i, sx, sy, color);

        sx += glyph_width;
    }
}

Graphics::PSFFont::~PSFFont() {
    stream.close();
    delete[] pixels;
}
