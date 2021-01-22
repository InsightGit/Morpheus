//
// Created by bobby on 24/12/2020.
//

#include "background.hpp"


morpheus::gba::gfx::Background::Background(unsigned int background_num, std::shared_ptr<GbaMainLoop> main_loop,
                                           bool is_8bpp, unsigned int cbb_num, unsigned int sbb_num) {
    m_background_num = std::min(3u, background_num);
    m_cbb_num = std::min(3u, cbb_num);
    m_is_8bpp = is_8bpp;
    m_main_loop = main_loop;
    m_sbb_num = std::min(31u, sbb_num);

    m_background_register = BG_CBB(m_cbb_num) | BG_SBB(m_sbb_num);

    if(m_is_8bpp) {
        m_background_register |= BG_8BPP;
    } else {
        m_background_register |= BG_4BPP;
    }
}


void morpheus::gba::gfx::Background::load_from_array(const unsigned int *tiles, const unsigned int tiles_len,
                                                     const unsigned short *palette, const unsigned int pal_len,
                                                     const unsigned short *tile_map, const unsigned int tile_map_len,
                                                     const core::gfx::Vector2 size) {
    load_from_array(tiles, tiles_len, tile_map, tile_map_len, size);

    memcpy16(pal_bg_mem, palette, pal_len);
}

void morpheus::gba::gfx::Background::load_from_array(const unsigned int *tiles, const unsigned int tiles_len,
                                                     const unsigned short *tile_map, const unsigned int tile_map_len,
                                                     const core::gfx::Vector2 size) {
    assert(size.get_y() == 32 || size.get_y() == 64 || size.get_x() == 32 || size.get_y() == 64);

    switch(size.get_x()) {
        case 32:
            if(size.get_y() == 64) {
                m_background_register |= BG_REG_32x64;
            } else {
                m_background_register |= BG_REG_32x32;
            }
            break;
        case 64:
            if(size.get_y() == 64) {
                m_background_register |= BG_REG_64x64;
            } else {
                m_background_register |= BG_REG_64x32;
            }
            break;
    }

    if(m_is_8bpp) {
        memcpy16(&tile8_mem[m_cbb_num][0], tiles, tiles_len);
    } else {
        memcpy16(&tile_mem[m_cbb_num][0], tiles, tiles_len);
    }

    memcpy16(&se_mem[m_sbb_num][0], tile_map, tile_map_len);

    update_background_register();
}

void morpheus::gba::gfx::Background::update_background_register() {
    switch (m_background_num) {
        case 0u:
            REG_BG0CNT = m_background_register;
            break;
        case 1u:
            REG_BG1CNT = m_background_register;
            break;
        case 2u:
            REG_BG2CNT = m_background_register;
            break;
        case 3u:
            REG_BG3CNT = m_background_register;
            break;
    }

    if(!m_main_loop_notified) {
        m_main_loop->enable_background(m_background_num);
        m_main_loop_notified = true;
    }
}
