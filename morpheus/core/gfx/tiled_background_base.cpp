//
// Created by bobby on 23/01/2021.
//

#include "tiled_background_base.hpp"

const morpheus::core::gfx::Vector2 morpheus::core::gfx::TiledBackgroundBase::TILE_SCREEN_SIZE =
        morpheus::core::gfx::Vector2(30, 20);

morpheus::core::gfx::TiledBackgroundBase::TiledBackgroundBase(bool affine, unsigned int background_num,
                                                              BlendingController *blending_controller,
                                                              MosaicController *mosaic_controller,
                                                              unsigned int cbb_num, unsigned int sbb_num,
                                                              bool use_tile_overrides) {
    m_affine = affine;
    m_background_num = std::min(3u, background_num);
    m_blending_controller = blending_controller;
    m_cbb_num = std::min(3u, cbb_num);
    m_mosaic_controller = mosaic_controller;
    m_sbb_num = std::min(31u, sbb_num);
    m_use_tile_overrides = use_tile_overrides;
}

int morpheus::core::gfx::TiledBackgroundBase::get_tile_id_at_position(const Vector2 position,
                                                                      const bool with_scrolling,
                                                                      const bool tiled_position) const {
    int tile_index = get_tile_index_at_position(position, with_scrolling, tiled_position);

    if(tile_index < 0) {
        return -1;
    } else {
        //nocashMessage(std::string("tile overrides: " + std::to_string(m_tile_overrides.size())).c_str());

        return get_tile_id_at_index(tile_index);
    }
}

int morpheus::core::gfx::TiledBackgroundBase::get_tile_index_at_position(const Vector2 position,
                                                                         const bool with_scrolling,
                                                                         const bool tiled_position) const {
    int tile_pitch = get_tile_map_size_vector().get_x();
    Vector2 tile_position = get_tile_position_at_screen_position(position, with_scrolling, tiled_position);

    if(tile_pitch > 0) {
        int sbb = ((tile_position.get_x() >> 5) + (tile_position.get_y() >> 5) * (tile_pitch >> 5));

        return (sbb * 1024) + ((tile_position.get_x() & 31) + (tile_position.get_y() & 31) * 32);
    } else {
        return -1;
    }
}

morpheus::core::gfx::Vector2 morpheus::core::gfx::TiledBackgroundBase::get_tile_position_at_screen_position(
        const morpheus::core::gfx::Vector2 position, const bool with_scrolling, const bool tiled_position) const {
    morpheus::core::gfx::Vector2 tile_position = position;
    morpheus::core::gfx::Vector2 tile_map_size = get_tile_map_size_vector();

    if(m_tile_map == nullptr || tile_map_size == Vector2(0, 0)) {
        return Vector2(-1, -1);
    }

    if(with_scrolling) {
        if(tiled_position) {
            tile_position = tile_position + (m_scroll_position / Vector2(8, 8));
        } else {
            tile_position = tile_position + m_scroll_position;
        }
    }

    if(!tiled_position) {
        tile_position = tile_position / Vector2(8, 8);
    }

    if(tile_position.get_x() >= tile_map_size.get_x() || tile_position.get_y() >= tile_map_size.get_y()) {
        return Vector2(-1, -1);
    }

    return tile_position;
}

bool morpheus::core::gfx::TiledBackgroundBase::set_tile_id_at_position(const morpheus::core::gfx::Vector2 position,
                                                                       const unsigned int tile_id,
                                                                       const bool with_scrolling,
                                                                       const bool tiled_position) {
    int tile_index = get_tile_index_at_position(position, with_scrolling, tiled_position);

    if(tile_index < 0) {
        return false;
    } else {
        return set_tile_id_at_index(tile_index, tile_id);
    }
}

int morpheus::core::gfx::TiledBackgroundBase::get_tile_id_at_index(const unsigned int tile_index) const {
    Vector2 tile_size_vector = get_size_vector();

    if(tile_index < static_cast<unsigned int>(tile_size_vector.get_x() * tile_size_vector.get_y())) {
        for(const TileOverride &tile_override : m_tile_overrides) {
            if(tile_override.tile_index == static_cast<unsigned int>(tile_index)) {
                return static_cast<int>(tile_override.tile_id & 0x03FF);
            }
        }

        return m_tile_map[tile_index] & 0x03FF;
    } else {
        return -1;
    }

}

bool morpheus::core::gfx::TiledBackgroundBase::set_tile_id_at_index(const unsigned int tile_index,
                                                                    const unsigned int tile_id) {
    bool found = false;
    Vector2 tile_size_vector = get_size_vector();

    /*if(tile_index >= static_cast<unsigned int>(tile_size_vector.get_x() * tile_size_vector.get_y())) {
        return false;
    }*/

    m_past_tile_overrides = m_tile_overrides;

    for(TileOverride &tile_override : m_tile_overrides) {
        if(tile_override.tile_index == static_cast<unsigned int>(tile_index)) {
            if(tile_override.tile_id == tile_id) {
                return true;
            } else {
                found = true;
                tile_override.tile_id = tile_id;

                override_map_tile(tile_override.tile_index, tile_override.tile_id);

                break;
            }
        }
    }

    if(!found) {
        m_tile_overrides.push_back({ .tile_index = static_cast<unsigned int>(tile_index),
                                     .tile_id = (tile_id & 0x3FF) });

        override_map_tile(m_tile_overrides.back().tile_index, m_tile_overrides.back().tile_id);
    }

    return true;
}

void morpheus::core::gfx::TiledBackgroundBase::update_tilemap_vars(const unsigned short *tile_map,
                                                                   const unsigned int tile_map_len,
                                                                   morpheus::core::gfx::TiledBackgroundSize size) {
    m_tile_map = tile_map;
    m_tile_map_size = size;

    if(!m_use_tile_overrides) {
        m_tile_map_rw_copy.reserve(tile_map_len);

        for(unsigned int i = 0; tile_map_len > i; ++i) {
            m_tile_map_rw_copy.push_back(tile_map[i]);
        }
    }
}
