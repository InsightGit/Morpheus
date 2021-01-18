//
// Created by bobby on 24/12/2020.
//

#include "background.hpp"


morpheus::gba::gfx::Background::Background(unsigned int background_num, std::shared_ptr<GbaMainLoop> main_loop) {
    m_background_num = std::min(3u, background_num);

    switch (m_background_num) {
        case 0:
            m_bg_control_register = REG_BG0CNT;
            break;
        case 1:
            m_bg_control_register = REG_BG1CNT;
            break;
        case 2:
            m_bg_control_register = REG_BG2CNT;
            break;
        case 3:
            m_bg_control_register = REG_BG3CNT;
            break;
    }

    m_bg_control_register = BG_CBB(0) | BG_SBB(30) | BG_8BPP | BG_REG_32x32;
    main_loop->enable_background(background_num);
}


void morpheus::gba::gfx::Background::load_from_array(const unsigned int *tiles, const int tiles_len,
                                                     const unsigned short *palette, const int pal_len,
                                                     const unsigned short *tile_map,
                                                     const int map_len) {
    memcpy(pal_bg_mem, palette, pal_len);
    memcpy(&tile_mem[0][0], tiles, tiles_len);
    memcpy(&se_mem[30][0], tile_map, map_len);

    //load_from_array(tiles, tiles_len, tile_map);
    //memcpy32(pal_bg_mem, palette, 256 / 8);
}

void morpheus::gba::gfx::Background::load_from_array(const unsigned int *tiles, const unsigned int tiles_len,
                                                     const unsigned short *tile_map) {
    memcpy32(&tile_mem[0][0], tiles, tiles_len / 2);
    memcpy32(&se_mem[30][0], tile_map, 1024 / 2);
}
