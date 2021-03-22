//
// Created by bobby on 22/12/2020.
//

#include "sprite_4_bpp.hpp"

morpheus::gba::gfx::Sprite4Bpp::Sprite4Bpp(GbaBlendingController *blending_controller, unsigned int palette_id) :
                                            morpheus::gba::gfx::Sprite(true, blending_controller) {
    m_palette_id = palette_id;
}

morpheus::gba::gfx::Sprite4Bpp::Sprite4Bpp(GbaBlendingController *blending_controller, const unsigned int tile_id,
                                           const unsigned int palette_id, const unsigned int width,
                                           const unsigned int height) : Sprite4Bpp(blending_controller, palette_id) {
    m_tile_id = tile_id;

    setup_size_attr(width, height);

    build_attr2(m_palette_id, tile_id);
}

void morpheus::gba::gfx::Sprite4Bpp::load_from_array(const unsigned short *tile_array, const unsigned short *palette,
                                                     const unsigned int palette_len, const unsigned int palette_id,
                                                     const unsigned int width, const unsigned int height,
                                                     const unsigned int tile_id) {
    m_tile_id = tile_id;

    set_palette_id(palette_id);

    Sprite::load_from_array(tile_array, palette, palette_len, width, height, tile_id);

    build_attr2(m_palette_id, m_tile_id);
}

void morpheus::gba::gfx::Sprite4Bpp::array_load(const unsigned short *tile_array, const unsigned int width,
                                                const unsigned int height, const unsigned int tile_id) {
    m_tile_id = tile_id;

    memcpy32(&tile_mem[4][m_tile_id], tile_array, (width * height) / 8);

    build_attr2(m_palette_id, m_tile_id);
}

void morpheus::gba::gfx::Sprite4Bpp::array_load(const unsigned short *tile_array, const unsigned short *palette,
                                                const unsigned int palette_len, const unsigned int width,
                                                const unsigned int height, const unsigned int tile_id) {
    array_load(tile_array, width, height, tile_id);
    load_into_palette(palette, palette_len);
}

void morpheus::gba::gfx::Sprite4Bpp::load_from_array(const unsigned short *tile_array,
                                                     const unsigned int palette_id,
                                                     const unsigned int width, const unsigned int height,
                                                     const unsigned int tile_id) {
    m_tile_id = tile_id;

    set_palette_id(palette_id);

    Sprite::load_from_array(tile_array, width, height, m_tile_id);

    build_attr2(m_palette_id, m_tile_id);
}

void morpheus::gba::gfx::Sprite4Bpp::load_into_palette(const unsigned short *palette,
                                                       const unsigned int palette_len) {
    memcpy16(pal_obj_mem, palette, palette_len / 2);
}
