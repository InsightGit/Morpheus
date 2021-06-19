//
// Created by bobby on 25/05/2021.
//

#include "streaming_background_base.hpp"

morpheus::core::gfx::StreamingBackgroundBase::StreamingBackgroundBase(TiledBackgroundBase *background_to_use,
                                                                      Vector2 map_tile_update_threshold) {
    m_background.reset(background_to_use),

    m_map_tile_update_threshold = map_tile_update_threshold;
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

    for(int i = 0; tilemaps_len > i; ++i) {
        m_tilemaps.push_back(tilemaps[i]);
    }

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
            load_tile_from_array(tilemap_array, current_tile_index, vram_map_tile_index);
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
            load_tile_from_array(tilemap_array, current_tile_index, vram_map_tile_index);
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
    m_current_tile_map[vram_tile_index] = tilemap_array[current_tile_index];

    m_background->set_tile_id_at_index(current_tile_index, m_current_tile_map[vram_tile_index]);
}

void morpheus::core::gfx::StreamingBackgroundBase::reload_tiles() {
    Vector2 global_scroll_difference = m_global_scroll - m_last_file_update_at;
    bool global_scroll_x_differs = global_scroll_difference.get_x() > (m_map_tile_update_threshold.get_x() * 8);
    bool global_scroll_y_differs = global_scroll_difference.get_y() > (m_map_tile_update_threshold.get_y() * 8);
    int previous_tile_index = 0;

    if(global_scroll_x_differs || global_scroll_y_differs) {
        Vector2 global_scroll_offset_vector = m_global_scroll % Vector2(64 * 8, 64 * 8);
        Vector2 initial_scroll_vector = m_last_file_update_at + Vector2(8 * 8, 8 * 8);

        Vector2 initial_scroll_difference_vector = m_global_scroll - initial_scroll_vector;

        /*if(global_scroll_background_number < m_tilemap_file_paths.size()) {
            return;
        }*/

        Vector2 global_scroll_background_vector = initial_scroll_vector / Vector2(64 * 8, 64 * 8);

        unsigned int global_scroll_background_number = global_scroll_background_vector.get_x() +
                (2 * global_scroll_background_vector.get_y());
        const unsigned short *tilemap_array = nullptr;
        FILE *tilemap_file = nullptr;

        if(m_using_files) {
            tilemap_file = fopen(m_tilemap_file_paths[global_scroll_background_number].c_str(), "rb");
        } else {
            tilemap_array = m_tilemaps[global_scroll_background_number];
        }

        reload_x_tiles();
        reload_y_tiles();

        if(m_using_files) {
            fclose(tilemap_file);
        }
    }
}

void morpheus::core::gfx::StreamingBackgroundBase::refresh_current_background_file_pointer(
                                                                              FILE **background_file_pointer,
                                                                              unsigned int &current_background_number,
                                                                              const Vector2 &current_scroll_vector) {
    Vector2 global_scroll_background_vector = current_scroll_vector / Vector2(64 * 8, 64 * 8);
    unsigned int new_global_scroll_background_number = global_scroll_background_vector.get_x() +
            (2 * global_scroll_background_vector.get_y());

    if(new_global_scroll_background_number != current_background_number) {
        current_background_number = new_global_scroll_background_number;

        if(background_file_pointer != nullptr) {
            fclose(*background_file_pointer);
        }

        *background_file_pointer = fopen(m_tilemap_file_paths[current_background_number].c_str(), "rb");
    }
}


void morpheus::core::gfx::StreamingBackgroundBase::refresh_current_background_array_pointer(
                                              const unsigned short **background_array_pointer,
                                              unsigned int &current_background_number,
                                              const Vector2 &current_scroll_vector) {
    Vector2 global_scroll_background_vector = current_scroll_vector / Vector2(64 * 8, 64 * 8);
    unsigned int new_global_scroll_background_number = global_scroll_background_vector.get_x() +
            (2 * global_scroll_background_vector.get_y());

    if(new_global_scroll_background_number != current_background_number) {
        *background_array_pointer = m_tilemaps[current_background_number];
    }
}

void morpheus::core::gfx::StreamingBackgroundBase::reload_x_tiles(FILE **tilemap_file,
                                                                  const unsigned short **tilemap_array,
                                                                  int &previous_tile_index) {
    int x_strip_value = m_global_scroll.get_x() + ((m_map_tile_update_threshold.get_x() + 1) * 8);
    int y_end_value = m_global_scroll.get_y() + ((m_map_tile_update_threshold.get_y() * 8) - m_player_position.get_y());
    int y_start_value = m_global_scroll.get_y() - m_player_position.get_y();

    if(scrolling_right) {
        x_strip_value = m_global_scroll.get_x() + ((m_map_tile_update_threshold.get_x() + 1) * 8);
        y_end_value = m_global_scroll.get_y() + ((m_map_tile_update_threshold.get_y() * 8) - m_player_position.get_y());
        y_start_value = m_global_scroll.get_y() - m_player_position.get_y();
    } else {
        // TODO(Bobby): Write left scrolling code
    }

    for(int y = y_start_value; y_end_value > y; ++y) {
        int current_tile_index;
        Vector2 current_tile_vector = Vector2(x_strip_value, y);
        int vram_map_tile_index;

        if(right) {
            current_tile_index = m_background->get_tile_index_at_position(current_tile_vector, false);
            vram_map_tile_index = m_background->get_tile_index_at_position(current_tile_vector % Vector2(64, 64),
                                                                           false);
        } else {
            current_tile_index = m_background->get_tile_index_at_position(current_tile_vector, false);
            vram_map_tile_index = get_tile_index_at_position(current_tile_vector % Vector2(64, 64), false);
        }

        if(m_using_files) {
            load_tile_from_file(tilemap_file, current_tile_index, vram_map_tile_index, previous_tile_index);
        } else {
            load_tile_from_array(tilemap_array, current_tile_index, vram_map_tile_index);
        }
    }

    /*Vector2 global_scroll_background_vector = initial_scroll_vector / Vector2(64 * 8, 64 * 8);
    Vector2 initial_scroll_difference_tile_vector = initial_scroll_difference_vector / Vector2(8, 8);
    bool scrolling_right;

    unsigned int global_scroll_background_number = global_scroll_background_vector.get_x() +
            (2 * global_scroll_background_vector.get_y());

    initial_scroll_difference_tile_vector = Vector2(abs(initial_scroll_difference_tile_vector.get_x()),
                                                    abs(initial_scroll_difference_tile_vector.get_y()));

    if(initial_scroll_difference_vector.get_x() > 0) {
        initial_scroll_vector = Vector2(m_global_scroll.get_x() - (m_map_tile_update_threshold.get_x() * 8),
                                        initial_scroll_vector.get_y());
        scrolling_right = true;
    } else if(initial_scroll_difference_vector.get_x() < 0) {
        initial_scroll_vector = Vector2(m_global_scroll.get_x() + (m_map_tile_update_threshold.get_x() * 8),
                                        initial_scroll_vector.get_y());
        scrolling_right = false;
    }*/

    while(initial_scroll_difference_tile_vector.get_x() > 0) {
        load_x_tile_strip(*tilemap_file, *tilemap_array, scrolling_right,
                          initial_scroll_vector % Vector2(64, 64), previous_tile_index);

        //global_scroll_offset_vector = Vector2(global_scroll_offset_vector.get_x() + )
        if(scrolling_right) {
            initial_scroll_vector = Vector2(initial_scroll_difference_tile_vector.get_x() + (8 * 8),
                                            initial_scroll_difference_tile_vector.get_y());
        } else {
            initial_scroll_vector = Vector2(initial_scroll_difference_tile_vector.get_x() - (8 * 8),
                                            initial_scroll_difference_tile_vector.get_y());
        }

        if(m_using_files) {
            refresh_current_background_file_pointer(tilemap_file, global_scroll_background_number,
                                                    initial_scroll_vector);
        } else {
            refresh_current_background_array_pointer(tilemap_array, global_scroll_background_number,
                                                     initial_scroll_vector);
        }

        initial_scroll_difference_tile_vector = Vector2(initial_scroll_difference_tile_vector.get_x() - 1,
                                                        initial_scroll_difference_vector.get_y());
    }
}

void morpheus::core::gfx::StreamingBackgroundBase::reload_y_tiles(Vector2 initial_scroll_vector,
                                                                  Vector2 initial_scroll_difference_vector) {
    bool scrolling_down;

    if(initial_scroll_difference_vector.get_y() > 0) {
        initial_scroll_vector = Vector2(initial_scroll_vector.get_x(), m_global_scroll.get_y() -
        (m_map_tile_update_threshold.get_y() * 8));
        scrolling_down = true;
    } else if(initial_scroll_difference_vector.get_y() < 0) {
        initial_scroll_vector = Vector2(initial_scroll_vector.get_x(), m_global_scroll.get_y() +
        (m_map_tile_update_threshold.get_y() * 8));
        scrolling_down = false;
    }

    while(initial_scroll_difference_tile_vector.get_y() > 0) {
        load_y_tile_strip(tilemap_file, tilemap_array, scrolling_down,
                          initial_scroll_vector % Vector2(64, 64),
                          previous_tile_index);

        if(scrolling_down) {
            initial_scroll_vector = Vector2(initial_scroll_difference_tile_vector.get_x(),
                                            initial_scroll_difference_tile_vector.get_y() + (8 * 8));
        } else {
            initial_scroll_vector = Vector2(initial_scroll_difference_tile_vector.get_x(),
                                            initial_scroll_difference_tile_vector.get_y() - (8 * 8));
        }

        if(m_using_files) {
            refresh_current_background_file_pointer(&tilemap_file, global_scroll_background_number,
                                                    initial_scroll_vector);
        } else {
            refresh_current_background_array_pointer(&tilemap_array, global_scroll_background_number,
                                                     initial_scroll_vector);
        }

        initial_scroll_difference_tile_vector = Vector2(initial_scroll_difference_tile_vector.get_x(),
                                                        initial_scroll_difference_tile_vector.get_y() - 1);
    }
}
