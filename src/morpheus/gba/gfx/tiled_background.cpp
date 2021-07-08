//
// Created by bobby on 24/12/2020.
//

#include "morpheus/gba/gfx/tiled_background.hpp"


morpheus::gba::gfx::TiledBackground::TiledBackground(bool affine, unsigned int background_num,
                                                     GbaBlendingController *blending_controller,
                                                     GbaMosaicController *mosaic_controller, GbaMainLoop *main_loop,
                                                     bool is_8bpp, unsigned int cbb_num, unsigned int sbb_num) :
                                         morpheus::core::gfx::TiledBackgroundBase(affine, background_num,
                                                                                  blending_controller,
                                                                                  mosaic_controller, cbb_num, sbb_num,
                                                                                  false) {
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
                                                     const core::gfx::TiledBackgroundSize size,
                                                     const core::gfx::BitUnpacking unpacking_needed,
                                                     const unsigned int palette_offset) {
    load_from_array(tiles, tiles_len, tile_map, tile_map_len, size, unpacking_needed);

    memcpy16(pal_bg_mem + (2 * palette_offset), palette, pal_len / 2);
}

void morpheus::gba::gfx::TiledBackground::array_load(const unsigned int *tiles, const unsigned int tiles_len,
                                                     const unsigned short *tile_map, const unsigned int tile_map_len,
                                                     const core::gfx::TiledBackgroundSize size,
                                                     const core::gfx::BitUnpacking unpacking_needed) {
    if(unpacking_needed == core::gfx::BitUnpacking::NONE || !unpack_if_needed(unpacking_needed, tiles, tiles_len)) {
        if(m_is_8bpp) {
            memcpy16(&tile8_mem[get_cbb_num()][0], tiles, tiles_len / 2);
        } else {
            memcpy16(&tile_mem[get_cbb_num()][0], tiles, tiles_len / 2);
        }
    }

    array_load(tile_map, tile_map_len, size);
}

void morpheus::gba::gfx::TiledBackground::array_load(const unsigned short *tile_map, const unsigned int tile_map_len,
                                                     morpheus::core::gfx::TiledBackgroundSize size) {
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
        case core::gfx::TiledBackgroundSize::BG_AFFINE_16x16:
            m_background_register |= BG_AFF_16x16;
            break;
        case core::gfx::TiledBackgroundSize::BG_AFFINE_32x32:
            m_background_register |= BG_AFF_32x32;
            break;
        case core::gfx::TiledBackgroundSize::BG_AFFINE_64x64:
            m_background_register |= BG_AFF_64x64;
            break;
        case core::gfx::TiledBackgroundSize::BG_AFFINE_128x128:
            m_background_register |= BG_AFF_128x128;
            break;
    }

    if(is_affine()) {
        memcpy32(reinterpret_cast<unsigned int*>(&se_mem[get_sbb_num()][0]),
                 reinterpret_cast<const unsigned int*>(tile_map), tile_map_len / 2);
    } else {
        memcpy16(&se_mem[get_sbb_num()][0], tile_map, tile_map_len);
    }

    update_background_register();
}

void morpheus::gba::gfx::TiledBackground::update_background_register() {
    switch (get_background_num()) {
        case 0u:
            REG_BG0CNT = m_background_register | BG_PRIO(m_background_priority);
            break;
        case 1u:
            REG_BG1CNT = m_background_register | BG_PRIO(m_background_priority);
            break;
        case 2u:
            REG_BG2CNT = m_background_register | BG_PRIO(m_background_priority);
            break;
        case 3u:
            REG_BG3CNT = m_background_register | BG_PRIO(m_background_priority);
            break;
    }

    if(is_affine()) {
        REG_BG_AFFINE[get_background_num()] = m_bg_affine;
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
            if(!is_affine()) {
                REG_BG2HOFS = scroll_position.get_x();
                REG_BG2VOFS = scroll_position.get_y();
            }
            break;
        case 3:
            if(!is_affine()) {
                REG_BG3HOFS = scroll_position.get_x();
                REG_BG3VOFS = scroll_position.get_y();
            }
            break;
    }

    if(is_affine()) {
        m_bg_affine_src_ex.tex_x = scroll_position.get_x();
        m_bg_affine_src_ex.tex_y = scroll_position.get_y();

        m_bg_affine_src_ex.sx = (1 << 16) / 0x0100;
        m_bg_affine_src_ex.sy = m_bg_affine_src_ex.sx;

        bg_rotscale_ex(&m_bg_affine, &m_bg_affine_src_ex);

        update_background_register();
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

bool morpheus::gba::gfx::TiledBackground::unpack_if_needed(const morpheus::core::gfx::BitUnpacking unpacking_needed,
                                                           const unsigned int *tiles, const unsigned int tiles_len) {
    asm_BitUnPackOptions bit_unpacking_options;
    //BUP bit_unpacking_options;

    bit_unpacking_options.source_len = tiles_len;

    switch(unpacking_needed) {
        case core::gfx::BitUnpacking::NONE:
            return true;
        case core::gfx::BitUnpacking::BPP_1_TO_4:
            if(m_is_8bpp) {
                m_main_loop->get_no_cash_debug_controller()->send_to_debug_window(
                        "Incorrect bit conversion specified! BPP_1_TO_4 was tried on a 8bpp background!");
                return false;
            } else {
                /*bit_unpacking_options.src_bpp = 1;
                bit_unpacking_options.dst_bpp = 4;
                bit_unpacking_options.dst_ofs = 0;*/
                bit_unpacking_options.source_bit_width = 1;
                bit_unpacking_options.dest_bit_width = 4;
                bit_unpacking_options.offset_plus_zero_data_flag = 0;

                m_main_loop->get_no_cash_debug_controller()->send_to_debug_window("Set to BPP_1_TO_4!");
            }
            break;
        case core::gfx::BitUnpacking::BPP_1_TO_8:
            if(!m_is_8bpp) {
                m_main_loop->get_no_cash_debug_controller()->send_to_debug_window(
                        "Incorrect bit conversion specified! BPP_1_TO_8 was tried on a 4bpp background!");
                return false;
            } else {
                bit_unpacking_options.source_bit_width = 1;
                bit_unpacking_options.dest_bit_width = 8;
                bit_unpacking_options.offset_plus_zero_data_flag = 0;
            }
            break;
        case core::gfx::BitUnpacking::BPP_4_TO_8:
            if(!m_is_8bpp) {
                m_main_loop->get_no_cash_debug_controller()->send_to_debug_window(
                        "Incorrect bit conversion specified! BPP_4_TO_8 was tried on a 4bpp background!");
                return false;
            } else {
                bit_unpacking_options.source_bit_width = 4;
                bit_unpacking_options.dest_bit_width = 8;
                bit_unpacking_options.offset_plus_zero_data_flag = 0;
            }
            break;
    }

    if(m_is_8bpp) {
        asm_BitUnPack(tiles, &tile8_mem[get_cbb_num()][0], &bit_unpacking_options);
    } else {
        asm_BitUnPack(tiles, &tile_mem[get_cbb_num()][0], &bit_unpacking_options);
    }

    return true;
}
