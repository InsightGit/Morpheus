//
// Created by bobby on 25/05/2021.
//

#include "streaming_background_base.hpp"

morpheus::core::gfx::StreamingBackgroundBase::StreamingBackgroundBase(TiledBackgroundBase *background_to_use,
                                                                      Vector2 map_tile_update_threshold,
                                                                      Vector2 player_position) {
    m_background.reset(background_to_use),

    m_map_tile_update_threshold = map_tile_update_threshold;

    set_player_position(player_position);
}

bool
morpheus::core::gfx::StreamingBackgroundBase::load_from_arrays(const unsigned int *tiles, const unsigned int tiles_len,
                                                               const unsigned short *palette,
                                                               const unsigned int pal_len,
                                                               const unsigned short **tilemaps,
                                                               const unsigned short tilemaps_len,
                                                               StreamingBackgroundSize size) {
    m_background_size = size;
    m_using_files = false;

    /*for(int i = 0; tilemaps_len > i; ++i) {
        m_tilemaps.push_back(tilemaps[i]);
    }*/

    m_tilemaps = tilemaps;
    m_tilemaps_len = tilemaps_len;

    m_background->load_from_array(tiles, tiles_len, palette, pal_len, m_tilemaps[0],
                                  64 * 64 * 2, TiledBackgroundSize::BG_64x64);

    return true;
}

bool morpheus::core::gfx::StreamingBackgroundBase::load_from_files(const unsigned int *tiles,
                                                                   const unsigned int tiles_len,
                                                                   const unsigned short *palette,
                                                                   const unsigned int pal_len,
                                                                   const std::vector<std::string> &tilemap_file_paths,
                                                                   StreamingBackgroundSize size) {
    m_background_size = size;
    m_tilemap_file_paths = tilemap_file_paths;
    m_using_files = true;

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

    m_background->load_from_array(tiles, tiles_len, palette, pal_len, &m_current_tile_map[0],
                                  m_current_tile_map.size(), TiledBackgroundSize::BG_64x64);

    return true;
}

void morpheus::core::gfx::StreamingBackgroundBase::load_x_tile_strip(FILE *tilemap_file,
                                                                     const unsigned short *tilemap_array,
                                                                     const bool right,
                                                                     const Vector2 &global_scroll_offset_vector,
                                                                     int &previous_tile_index) {
    for(int y = 0; 64 > y; ++y) {
        int current_tile_index;
        int vram_map_tile_index;

        if(right) {
            current_tile_index = m_background->get_tile_index_at_position(
                    Vector2(global_scroll_offset_vector.get_x(), y), false);
            //vram_map_tile_index = get_tile_index_at_position(Vector2(63, y), false);
        } else {
            current_tile_index = m_background->get_tile_index_at_position(
                    Vector2(64 - global_scroll_offset_vector.get_x(), y), false);
            //vram_map_tile_index = get_tile_index_at_position(Vector2(0, y), false);
        }

        //override_map_tile(get_tile_id_at_index(vram_map_tile_index));

        vram_map_tile_index = current_tile_index;

        if(m_using_files) {
            load_tile_from_file(tilemap_file, current_tile_index, vram_map_tile_index, previous_tile_index);
        } else {
            //load_tile_from_array(tilemap_array, current_tile_index, vram_map_tile_index);
        }
    }
}

void morpheus::core::gfx::StreamingBackgroundBase::load_y_tile_strip(FILE *tilemap_file,
                                                                     const unsigned short *tilemap_array,
                                                                     const bool down,
                                                                     const Vector2 &global_scroll_offset_vector,
                                                                     int &previous_tile_index) {
    for(int x = 0; 64 > x; ++x) {
        int current_tile_index;
        int vram_map_tile_index;

        if(down) {
            current_tile_index = m_background->get_tile_index_at_position(
                    Vector2(x, global_scroll_offset_vector.get_y()), false);
            //vram_map_tile_index = get_tile_index_at_position(Vector2(x, 63), false);
        } else {
            current_tile_index = m_background->get_tile_index_at_position(
                    Vector2(x, 64 - global_scroll_offset_vector.get_y()), false);
            //vram_map_tile_index = get_tile_index_at_position(Vector2(x, 0), false);
        }

        vram_map_tile_index = current_tile_index;

        if(m_using_files) {
            load_tile_from_file(tilemap_file, current_tile_index, vram_map_tile_index, previous_tile_index);
        } else {
            //load_tile_from_array(tilemap_array, current_tile_index, vram_map_tile_index);
        }
    }
}

void morpheus::core::gfx::StreamingBackgroundBase::load_tile_from_file(FILE *tilemap_file, int current_tile_index,
                                                                       int vram_tile_index, int &previous_tile_index) {
    if(current_tile_index - previous_tile_index != 1) {
        fseek(tilemap_file, current_tile_index - previous_tile_index, SEEK_CUR);
    }

    fread(&m_current_tile_map[vram_tile_index], 2, 1, tilemap_file);

    previous_tile_index = current_tile_index;

    m_background->set_tile_id_at_index(current_tile_index, m_current_tile_map[vram_tile_index]);
}

void morpheus::core::gfx::StreamingBackgroundBase::load_tile_from_array(const unsigned short *tilemap_array,
                                                                        int current_tile_index, int vram_tile_index) {
    //m_current_tile_map[vram_tile_index] = tilemap_array[current_tile_index] & 0x03FF;

    /*nocash_puts(("tile id is " + std::to_string(tilemap_array[current_tile_index] & 0x03FF) + " at index " +
                    std::to_string(vram_tile_index)).c_str());*/

    m_background->set_tile_id_at_index(vram_tile_index, tilemap_array[current_tile_index]);
}

void morpheus::core::gfx::StreamingBackgroundBase::reload_tiles() {
    Vector2 global_scroll_difference = m_global_scroll - m_last_file_update_at;
    bool global_scroll_x_differs = abs(global_scroll_difference.get_x()) > 7;
    bool global_scroll_y_differs = abs(global_scroll_difference.get_y()) > 7;

    if(global_scroll_x_differs) {
        reload_x_tiles(global_scroll_difference.get_x() > 0);
    }

    if(global_scroll_y_differs) {
        //reload_y_tiles();
    }

    m_last_file_update_at = m_global_scroll;
}

void morpheus::core::gfx::StreamingBackgroundBase::refresh_current_background_file_pointer(
                                                                              FILE **background_file_pointer,
                                                                              unsigned int &current_background_number,
                                                                              const Vector2 &current_scroll_vector) {
    Vector2 global_scroll_background_vector = current_scroll_vector / Vector2(64, 64);
    unsigned int new_global_scroll_background_number = global_scroll_background_vector.get_x() +
            (2 * global_scroll_background_vector.get_y());

    if(new_global_scroll_background_number != current_background_number) {
        current_background_number = new_global_scroll_background_number;

        if(background_file_pointer != nullptr) {
            fclose(*background_file_pointer);
        }

        *background_file_pointer = fopen(m_tilemap_file_paths[current_background_number].c_str(), "rb");

        nocash_puts(("Refreshing current bg file pointer to " +
                         std::to_string(new_global_scroll_background_number)).c_str());
    }
}


const unsigned short *morpheus::core::gfx::StreamingBackgroundBase::refresh_current_background_array_pointer(
                                              const unsigned short *current_background_array_pointer,
                                              unsigned int &current_background_number,
                                              const Vector2 &current_scroll_vector) {
    Vector2 global_scroll_background_vector = current_scroll_vector / Vector2(64, 64);
    unsigned int new_global_scroll_background_number = global_scroll_background_vector.get_x() +
            (2 * global_scroll_background_vector.get_y());

    if(new_global_scroll_background_number != current_background_number
       || current_background_array_pointer == nullptr) {
        current_background_number = new_global_scroll_background_number;

        nocash_puts(("Refreshing current bg array to " + std::to_string(new_global_scroll_background_number)).c_str());

        return m_tilemaps[current_background_number];
    } else {
        return current_background_array_pointer;
    }
}

void morpheus::core::gfx::StreamingBackgroundBase::reload_x_tiles(const bool scrolling_right) {
    unsigned int current_background_number = 0;
    int previous_tile_index = 0;
    const unsigned short *tilemap_array = nullptr;
    FILE *tilemap_file = nullptr;
    int x_strip_value;
    int y_end_value = m_global_scroll.get_y() + ((m_map_tile_update_threshold.get_y() * 8) - m_player_position.get_y());
    int y_start_value = m_global_scroll.get_y() - m_player_position.get_y();

    if(scrolling_right) {
        x_strip_value = m_global_scroll.get_x() + ((m_map_tile_update_threshold.get_x() + 1) * 8);
    } else {
        x_strip_value = m_global_scroll.get_x() - 8;
    }

    for(int y = y_start_value; y_end_value > y; y += 8) {
        int current_tile_index = 0;
        Vector2 current_tile_vector = Vector2(x_strip_value / 8, y / 8);
        int vram_map_tile_index = 0;

        current_tile_index = m_background->get_tile_index_at_position(current_tile_vector % Vector2(64, 64), false,
                                                                      true);
        vram_map_tile_index = m_background->get_tile_index_at_position(current_tile_vector % Vector2(64, 64), false,
                                                                       true);

        nocash_puts(std::string("Current Tile-" + current_tile_vector.to_string()).c_str());

        if(m_using_files) {
            refresh_current_background_file_pointer(&tilemap_file, current_background_number, current_tile_vector);
            load_tile_from_file(tilemap_file, current_tile_index, vram_map_tile_index, previous_tile_index);
        } else {
            tilemap_array = refresh_current_background_array_pointer(tilemap_array,
                                                                     current_background_number, current_tile_vector);

            load_tile_from_array(tilemap_array, current_tile_index, vram_map_tile_index);
        }
    }

    if(m_using_files && tilemap_file != nullptr) {
        fclose(tilemap_file);
    }
}

void morpheus::core::gfx::StreamingBackgroundBase::reload_y_tiles(const bool scrolling_down) {
    unsigned int current_background_number = 0;
    int previous_tile_index = 0;
    const unsigned short *tilemap_array = nullptr;
    FILE *tilemap_file = nullptr;
    int x_end_value = m_global_scroll.get_x() + ((m_map_tile_update_threshold.get_x() * 8) - m_player_position.get_x());
    int x_start_value = m_global_scroll.get_x() - m_player_position.get_x();
    int y_strip_value;

    if(scrolling_down) {
        y_strip_value = m_global_scroll.get_y() + ((m_map_tile_update_threshold.get_y() + 1) * 8);
    } else {
        y_strip_value = m_global_scroll.get_y() - 8;
    }

    for(int x = x_start_value; x_end_value > x; ++x) {
        //
    }
}
