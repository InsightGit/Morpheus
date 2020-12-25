//
// Created by bobby on 22/12/2020.
//

#include "sprite_8_bpp.hpp"

void morpheus::gba::gfx::Sprite8Bpp::array_load(const unsigned short *tile_array, const unsigned short width,
                                                const unsigned short height, const unsigned short tile_id) {
    memcpy32(&tile_mem[4][tile_id], tile_array, (width * height) / 4);
}


void morpheus::gba::gfx::Sprite8Bpp::array_load(const unsigned short *tile_array, const unsigned short *palette,
                                                const unsigned short width, const unsigned short height,
                                                const unsigned short tile_id) {
    std::cout << "loading array\n";

    array_load(tile_array, width, height, tile_id);
    memcpy32(pal_obj_mem, palette, 256 / 4);

    std::cout << "loaded array\n";
}
