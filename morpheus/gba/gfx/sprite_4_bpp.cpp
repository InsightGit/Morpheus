//
// Created by bobby on 22/12/2020.
//

#include "sprite_4_bpp.hpp"

morpheus::gba::gfx::Sprite4Bpp::Sprite4Bpp(GbaBlendingController *blending_controller, unsigned int palette_id) :
                                            morpheus::gba::gfx::Sprite(true, blending_controller) {
    m_palette_id = palette_id;
}

morpheus::gba::gfx::Sprite4Bpp::Sprite4Bpp(GbaBlendingController *blending_controller, const unsigned int tile_id,
                                           const unsigned int palette_id, const core::gfx::SpriteSize size) :
                                           Sprite4Bpp(blending_controller, palette_id) {
    m_tile_id = tile_id;

    set_sprite_size(size);
    set_palette_id(palette_id);

    build_attr2(m_palette_id, tile_id);
}

void morpheus::gba::gfx::Sprite4Bpp::load_from_array(const unsigned short *tile_array,
                                                     const unsigned int tile_array_len,
                                                     const unsigned short *palette, const unsigned int palette_len,
                                                     const unsigned int palette_id, const core::gfx::SpriteSize size,
                                                     const unsigned int tile_id) {
    m_tile_id = tile_id;

    set_palette_id(palette_id);

    Sprite::load_from_array(tile_array, tile_array_len, palette, palette_len, size, tile_id);

    build_attr2(m_palette_id, m_tile_id);
}

void morpheus::gba::gfx::Sprite4Bpp::load_from_array(const unsigned short *tile_array,
                                                     const unsigned int tile_array_len, const unsigned int palette_id,
                                                     const core::gfx::SpriteSize size, const unsigned int tile_id) {
    m_tile_id = tile_id;

    set_palette_id(palette_id);

    Sprite::load_from_array(tile_array, tile_array_len, size, m_tile_id);

    build_attr2(m_palette_id, m_tile_id);
}

void morpheus::gba::gfx::Sprite4Bpp::array_load(const unsigned short *tile_array, const unsigned int tile_array_len,
                                                const core::gfx::SpriteSize size, const unsigned int tile_id) {
    m_tile_id = tile_id;

    memcpy16(&tile_mem[4][m_tile_id], tile_array, tile_array_len / 2);

    build_attr2(m_palette_id, m_tile_id);
}

void morpheus::gba::gfx::Sprite4Bpp::array_load(const unsigned short *tile_array, const unsigned int tile_array_len,
                                                const unsigned short *palette, const unsigned int palette_len,
                                                const core::gfx::SpriteSize size, const unsigned int tile_id) {
    array_load(tile_array, tile_array_len, size, tile_id);
    load_into_palette(palette, palette_len);
}

bool morpheus::gba::gfx::Sprite4Bpp::load_into_palette(const unsigned short *palette,
                                                       const unsigned int palette_len) {
    memcpy16(pal_obj_mem + (m_palette_id * 16), palette, palette_len / 2);

    return true;
}
