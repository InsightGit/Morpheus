//
// Created by bobby on 24/12/2020.
//

#include "tiled_background.hpp"


morpheus::gba::gfx::TiledBackground::TiledBackground(bool affine, unsigned int background_num,
                                                     GbaBlendingController *blending_controller,
                                                     GbaMosaicController *mosaic_controller, GbaMainLoop *main_loop,
                                                     bool is_8bpp, unsigned int cbb_num, unsigned int sbb_num) :
                                         morpheus::core::gfx::TiledBackgroundBase(affine, background_num,
                                                                                  blending_controller,
                                                                                  mosaic_controller, cbb_num, sbb_num) {
    m_background_register = BG_CBB(get_cbb_num()) | BG_SBB(get_sbb_num());
    m_main_loop = main_loop;
    m_is_8bpp = is_8bpp;

    if(m_is_8bpp) {
        m_background_register |= BG_8BPP;
    } else {
        m_background_register |= BG_4BPP;
    }
}


void morpheus::gba::gfx::TiledBackground::array_load(const unsigned int *tiles, const unsigned int tiles_len,
                                                     const unsigned short *palette, const unsigned int pal_len,
                                                     const unsigned short *tile_map, const unsigned int tile_map_len,
                                                     core::gfx::TiledBackgroundSize size) {
    load_from_array(tiles, tiles_len, tile_map, tile_map_len, size);

    memcpy16(pal_bg_mem, palette, pal_len / 2);
}

void morpheus::gba::gfx::TiledBackground::array_load(const unsigned int *tiles, const unsigned int tiles_len,
                                                     const unsigned short *tile_map, const unsigned int tile_map_len,
                                                     core::gfx::TiledBackgroundSize size) {
    switch(size) {
        default:
        case core::gfx::TiledBackgroundSize::BG_32x32:
            m_background_register |= BG_REG_32x32;
            break;
        case core::gfx::TiledBackgroundSize::BG_32x64:
            m_background_register |= BG_REG_32x64;
            break;
        case core::gfx::TiledBackgroundSize::BG_64x32:
            m_background_register |= BG_REG_64x32;
            break;
        case core::gfx::TiledBackgroundSize::BG_64x64:
            m_background_register |= BG_REG_64x64;
            break;
    }

    if(m_is_8bpp) {
        memcpy16(&tile8_mem[get_cbb_num()][0], tiles, tiles_len);
    } else {
        memcpy16(&tile_mem[get_cbb_num()][0], tiles, tiles_len);
    }

    memcpy16(&se_mem[get_sbb_num()][0], tile_map, tile_map_len);

    update_background_register();
}

void morpheus::gba::gfx::TiledBackground::update_background_register() {
    nocash_puts(std::string("updating background register #" + std::to_string(get_background_num())).c_str());

    switch (get_background_num()) {
        case 0u:
            REG_BG0CNT = m_background_register | BG_PRIO(m_background_priority);

            nocash_puts(std::string("new bg0cnt value:" + std::to_string(REG_BG0CNT)).c_str());
            break;
        case 1u:
            REG_BG1CNT = m_background_register | BG_PRIO(m_background_priority);

            nocash_puts(std::string("new bg1cnt value:" + std::to_string(REG_BG1CNT)).c_str());
            break;
        case 2u:
            REG_BG2CNT = m_background_register | BG_PRIO(m_background_priority);

            nocash_puts(std::string("new bg2cnt value:" + std::to_string(REG_BG2CNT)).c_str());
            break;
        case 3u:
            REG_BG3CNT = m_background_register | BG_PRIO(m_background_priority);

            nocash_puts(std::string("new bg3cnt value:" + std::to_string(REG_BG3CNT)).c_str());
            break;
    }

    if(!m_main_loop_notified) {
        m_main_loop->enable_background(get_background_num());

        m_main_loop_notified = true;
    }
}

void morpheus::gba::gfx::TiledBackground::update_scroll() {
    morpheus::core::gfx::Vector2 scroll_position = get_scroll();

    switch(get_background_num()) {
        case 0:
            REG_BG0HOFS = scroll_position.get_x();
            REG_BG0VOFS = scroll_position.get_y();
            break;
        case 1:
            REG_BG1HOFS = scroll_position.get_x();
            REG_BG1VOFS = scroll_position.get_y();
            break;
        case 2:
            REG_BG2HOFS = scroll_position.get_x();
            REG_BG2VOFS = scroll_position.get_y();
            break;
        case 3:
            REG_BG3HOFS = scroll_position.get_x();
            REG_BG3VOFS = scroll_position.get_y();
            break;
    }
}

void morpheus::gba::gfx::TiledBackground::mosaic_state_updated() {
    if(is_mosaic()) {
        m_background_register |= BG_MOSAIC;
    } else {
        m_background_register &= ~(BG_MOSAIC);
    }

    update_background_register();
}
