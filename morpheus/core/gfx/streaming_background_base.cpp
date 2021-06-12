//
// Created by bobby on 25/05/2021.
//

#include "streaming_background_base.hpp"

morpheus::core::gfx::StreamingBackgroundBase::StreamingBackgroundBase(bool affine, unsigned int background_num,
                                                                      BlendingController *blending_controller,
                                                                      MosaicController *mosaic_controller,
                                                                      unsigned int cbb_num, unsigned int sbb_num,
                                                                      Vector2 map_tile_update_threshold)
        : TiledBackgroundBase(affine, background_num, blending_controller, mosaic_controller, cbb_num, sbb_num, false) {
    m_map_tile_update_threshold = map_tile_update_threshold;
}

bool morpheus::core::gfx::StreamingBackgroundBase::load_from_files(const unsigned int *tiles,
                                                                   const unsigned int tiles_len,
                                                                   const unsigned short *palette,
                                                                   const unsigned int pal_len,
                                                                   const std::vector<std::string> &tilemap_file_paths,
                                                                   StreamingBackgroundSize size) {
    m_background_size = size;
    m_tilemap_file_paths = tilemap_file_paths;

    load_tiles_and_palette(tiles, tiles_len, palette, pal_len);

    FILE *initial_map_file = fopen(m_tilemap_file_paths[0].c_str(), "rb");

    fseek(initial_map_file, 0, SEEK_END);

    if(ftell(initial_map_file) != 64 * 64 * 2) {
        // StreamingBackgroundBase only uses 64x64 tilemaps
        return false;
    }

    for(int i = 0; 64 * 64 * 2 > i; ++i) {
        m_current_tile_map.push_back(0);

        fread(&m_current_tile_map.back(), 2, 1, initial_map_file);
    }

    fclose(initial_map_file);

    return true;
}

void morpheus::core::gfx::StreamingBackgroundBase::load_x_tile_strip(FILE *tilemap_file, const bool right,
                                                                const Vector2 &global_scroll_offset_vector,
                                                                int &previous_tile_index) {
    for(int y = 0; 64 > y; ++y) {
        int current_tile_index;
        int vram_map_tile_index;

        if(right) {
            current_tile_index = get_tile_index_at_position(Vector2(64 - global_scroll_offset_vector.get_x(), y),
                                                            false);
            vram_map_tile_index = get_tile_index_at_position(Vector2(63, y), false);
        } else {
            current_tile_index = get_tile_index_at_position(Vector2(global_scroll_offset_vector.get_x(), y),
                                                            false);
            vram_map_tile_index = get_tile_index_at_position(Vector2(0, y), false);
        }

        load_tile(tilemap_file, current_tile_index, vram_map_tile_index, previous_tile_index);
    }
}

void morpheus::core::gfx::StreamingBackgroundBase::load_y_tile_strip(FILE *tilemap_file, const bool down,
                                                                     const Vector2 &global_scroll_offset_vector,
                                                                     int &previous_tile_index) {
    for(int x = 0; 64 > x; ++x) {
        int current_tile_index;
        int vram_map_tile_index;

        if(down) {
            current_tile_index = get_tile_index_at_position(Vector2(x,64 - global_scroll_offset_vector.get_y()),
                                                            false);
            vram_map_tile_index = get_tile_index_at_position(Vector2(x, 63), false);
        } else {
            current_tile_index = get_tile_index_at_position(Vector2(x, global_scroll_offset_vector.get_y()), false);
            vram_map_tile_index = get_tile_index_at_position(Vector2(x, 0), false);
        }

        load_tile(tilemap_file, current_tile_index, vram_map_tile_index, previous_tile_index);
    }
}

void morpheus::core::gfx::StreamingBackgroundBase::load_tile(FILE *tilemap_file, int current_tile_index,
                                                             int vram_tile_index,
                                                             int &previous_tile_index) {
    if(current_tile_index - previous_tile_index != 1) {
        fseek(tilemap_file, current_tile_index - previous_tile_index, SEEK_CUR);
    }

    fread(&m_current_tile_map[vram_tile_index], 2, 1, tilemap_file);

    previous_tile_index = current_tile_index;

    override_map_tile(current_tile_index, m_current_tile_map[vram_tile_index]);
}

void morpheus::core::gfx::StreamingBackgroundBase::reload_tiles() {
    Vector2 global_scroll_difference = m_global_scroll - m_last_file_update_at;
    Vector2 global_scroll_difference_tile_shift = global_scroll_difference / Vector2(8, 8);
    bool global_scroll_x_differs = global_scroll_difference.get_x() > (m_map_tile_update_threshold.get_x() * 8);
    bool global_scroll_y_differs = global_scroll_difference.get_y() > (m_map_tile_update_threshold.get_y() * 8);
    int previous_tile_index = 0;

    if(global_scroll_x_differs || global_scroll_y_differs) {
        Vector2 global_scroll_background_vector = m_global_scroll / Vector2(64 * 8, 64 * 8);

        unsigned int global_scroll_background_number = global_scroll_background_vector.get_x() +
                (2 * global_scroll_background_vector.get_y());
        Vector2 global_scroll_offset_vector = m_global_scroll % Vector2(64 * 8, 64 * 8);
        Vector2 initial_scroll_vector = m_last_file_update_at + Vector2(8 * 8, 8 * 8);
        FILE *tilemap_file = fopen(m_tilemap_file_paths[global_scroll_background_number].c_str(), "rb");

        if(initial_scroll_vector.get_x() > (m_map_tile_update_threshold.get_x() * 8) ) {
            //
        } else if(initial_scroll_vector.get_y() > m_map_tile_update_threshold.get_y()) {
            //
        }

        if(global_scroll_background_number < m_tilemap_file_paths.size()) {
            return;
        }

        while(global_scroll_difference_tile_shift.get_x() > 0) {
            //unsigned int new_global_scroll_background_number;

            load_x_tile_strip(tilemap_file, global_scroll_difference.get_x() > 0, global_scroll_offset_vector,
                              previous_tile_index);

            //global_scroll_offset_vector = Vector2(global_scroll_offset_vector.get_x() + )
            global_scroll_difference_tile_shift = Vector2(global_scroll_difference_tile_shift.get_x() - 1,
                                                          global_scroll_difference_tile_shift.get_y());
        }

        while(global_scroll_difference_tile_shift.get_y() > 0) {
            load_y_tile_strip(tilemap_file, global_scroll_difference.get_y() > 0, global_scroll_offset_vector,
                              previous_tile_index);

            global_scroll_difference_tile_shift = Vector2(global_scroll_difference_tile_shift.get_x(),
                                                          global_scroll_difference_tile_shift.get_y() - 1);
        }

        fclose(tilemap_file);
    }
}
