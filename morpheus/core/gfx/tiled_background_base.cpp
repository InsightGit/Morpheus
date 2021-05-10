//
// Created by bobby on 23/01/2021.
//

#include "tiled_background_base.hpp"

morpheus::core::gfx::TiledBackgroundBase::TiledBackgroundBase(bool affine, unsigned int background_num,
                                                              BlendingController *blending_controller,
                                                              MosaicController *mosaic_controller,
                                                              unsigned int cbb_num, unsigned int sbb_num) {
    m_affine = affine;
    m_background_num = std::min(3u, background_num);
    m_blending_controller = blending_controller;
    m_cbb_num = std::min(3u, cbb_num);
    m_mosaic_controller = mosaic_controller;
    m_sbb_num = std::min(31u, sbb_num);
}

int morpheus::core::gfx::TiledBackgroundBase::get_tile_id_at_position(morpheus::core::gfx::Vector2 position,
                                                                      bool with_scrolling) {
    int tile_id = 0;
    int tile_pitch = -1;
    morpheus::core::gfx::Vector2 tile_position = position;

    if(m_tile_map == nullptr) {
        return -1;
    }

    if(with_scrolling) {
        tile_position = position + m_scroll_position;
    }
    
    tile_position = morpheus::core::gfx::Vector2(tile_position.get_x() / 8, tile_position.get_y() / 8);

    switch (m_tile_map_size) {
        case TiledBackgroundSize::BG_32x32:
            if(tile_position.get_x() >= 32 || tile_position.get_y() >= 32) {
                return -1;
            }

            tile_id = tile_position.get_x() + (tile_position.get_y() * 32);
            break;
        case TiledBackgroundSize::BG_64x32:
            if(tile_position.get_x() >= 64 || tile_position.get_y() >= 32) {
                return -1;
            }

            tile_pitch = 64;
            break;
        case TiledBackgroundSize::BG_32x64:
            if(tile_position.get_x() >= 32 || tile_position.get_y() >= 64) {
                return -1;
            }

            tile_pitch = 32;
            break;
        case TiledBackgroundSize::BG_64x64:
            if(tile_position.get_x() >= 64 || tile_position.get_y() >= 64) {
                return -1;
            }

            tile_pitch = 64;
            break;
        default:
            return -1;
    }

    if(tile_pitch > 0) {
        int sbb = ((tile_position.get_x() >> 5) + (tile_position.get_y() >> 5) * (tile_pitch >> 5));

        tile_id = (sbb * 1024) + ((tile_position.get_x() & 31) + (tile_position.get_y() & 31) * 32);
    }

    return (m_tile_map[tile_id] & 0x03FF);
}

