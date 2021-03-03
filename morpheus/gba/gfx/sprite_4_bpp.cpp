//
// Created by bobby on 22/12/2020.
//

#include "sprite_4_bpp.hpp"

morpheus::gba::gfx::Sprite4Bpp::Sprite4Bpp(unsigned int palette_id) : morpheus::gba::gfx::Sprite(true) {
    m_palette_id = palette_id;
}

morpheus::gba::gfx::Sprite4Bpp::Sprite4Bpp(const unsigned short tile_id, const unsigned int palette_id,
                                           const unsigned short width, const unsigned short height) :
                                           Sprite4Bpp(palette_id) {
    setup_size_attr(width, height);

    build_attr2(m_palette_id, tile_id);
}

void morpheus::gba::gfx::Sprite4Bpp::load_from_array(const unsigned short *tile_array, const unsigned short *palette,
                                                     const unsigned short palette_id, const unsigned short width,
                                                     const unsigned short height, const unsigned short tile_id) {
    set_palette_id(palette_id);

    Sprite::load_from_array(tile_array, palette, width, height, tile_id);

    build_attr2(m_palette_id, tile_id);
}

void morpheus::gba::gfx::Sprite4Bpp::array_load(const unsigned short *tile_array, const unsigned short width,
                                                const unsigned short height, const unsigned short tile_id) {
    memcpy32(&tile_mem[4][tile_id], tile_array, (width * height) / 8);

    nocash_puts(std::string("Building attr2 with tile id " + std::to_string(tile_id) +
                                " and palette id " + std::to_string(m_palette_id)).c_str());

    build_attr2(tile_id, m_palette_id);
}

void morpheus::gba::gfx::Sprite4Bpp::array_load(const unsigned short *tile_array, const unsigned short *palette,
                                                const unsigned short width, const unsigned short height,
                                                const unsigned short tile_id) {
    std::cout << "Loading palette to offset " << 16 * m_palette_id << " with palette num " << m_palette_id << "\n";

    array_load(tile_array, width, height, tile_id);
    load_into_palette(palette, m_palette_id);
}

void morpheus::gba::gfx::Sprite4Bpp::load_from_array(const unsigned short *tile_array,
                                                     const unsigned short palette_id,
                                                     const unsigned short width, const unsigned short height,
                                                     const unsigned short tile_id) {
    set_palette_id(palette_id);

    Sprite::load_from_array(tile_array, width, height, tile_id);

    build_attr2(m_palette_id, tile_id);
}

void morpheus::gba::gfx::Sprite4Bpp::load_into_palette(const unsigned short *palette, const unsigned int palette_id,
                                                       const unsigned int pal_len) {
    memcpy16(pal_obj_mem + (16 * palette_id), palette, pal_len / 2);
}
