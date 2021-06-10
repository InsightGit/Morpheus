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

void morpheus::core::gfx::StreamingBackgroundBase::load_from_files(const unsigned int *tiles,
                                                                   const unsigned int tiles_len,
                                                                   const unsigned short *palette,
                                                                   const unsigned int pal_len,
                                                                   const std::vector<std::string> &tilemap_file_paths,
                                                                   StreamingBackgroundSize size) {
    m_background_size = size;
    m_tilemap_file_paths = tilemap_file_paths;

    load_tiles_and_palette(tiles, tiles_len, palette, pal_len);
    //load_from_file(tilemap_file_paths[0]);
}

void morpheus::core::gfx::StreamingBackgroundBase::load_x_tiles(bool right) {
    Vector2 global_scroll_background_vector = m_global_scroll / Vector2(64, 64);
    unsigned int global_scroll_background_number = global_scroll_background_vector.get_x() +
            (2 * global_scroll_background_vector.get_y());
    Vector2 global_scroll_offset_vector = m_global_scroll % Vector2(64, 64);

    if(right) {
        if(global_scroll_background_number < m_tilemap_file_paths.size()) {
            //
        } else {
            FILE *file = fopen(m_tilemap_file_paths[global_scroll_background_number].c_str(), "rb");


            fseek(file, get_tile_index_at_position(global_scroll_offset_vector), 0);
            fread();

            fclose(file);
        }
    }
}

void morpheus::core::gfx::StreamingBackgroundBase::load_y_tiles(bool up) {
    //
}
