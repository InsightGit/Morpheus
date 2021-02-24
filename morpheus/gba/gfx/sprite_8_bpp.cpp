//
// Created by bobby on 22/12/2020.
//

#include "sprite_8_bpp.hpp"

void morpheus::gba::gfx::Sprite8Bpp::array_load(const unsigned short *tile_array, const unsigned short width,
                                                const unsigned short height, const unsigned short tile_id) {
    m_tile_id = tile_id;

    memcpy32(&tile_mem[4][m_tile_id], tile_array, (width * height) / 4);
}


void morpheus::gba::gfx::Sprite8Bpp::array_load(const unsigned short *tile_array, const unsigned short *palette,
                                                const unsigned short width, const unsigned short height,
                                                const unsigned short tile_id) {
    std::cout << "loading array\n";

    array_load(tile_array, width, height, tile_id);
    load_into_palette(palette, 0, 256);
    //memcpy32(pal_obj_mem, palette, 256 / 4);

    std::cout << "loaded array\n";
}

void morpheus::gba::gfx::Sprite8Bpp::load_into_palette(const unsigned short *palette, const unsigned int palette_id,
                                                       const unsigned int pal_len) {
    memcpy16(pal_obj_mem + (palette_id * 16), palette, pal_len);
}
