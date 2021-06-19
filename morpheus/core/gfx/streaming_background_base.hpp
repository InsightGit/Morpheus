//
// Created by bobby on 25/05/2021.
//

#ifndef MORPHEUS_GBA_TEST_STREAMING_BACKGROUND_BASE_HPP
#define MORPHEUS_GBA_TEST_STREAMING_BACKGROUND_BASE_HPP

#include <fat.h>

#include <memory>

#include "tiled_background_base.hpp"

namespace morpheus {
    namespace core {
        namespace gfx {
            enum class StreamingBackgroundSize {
                BG_64x128,
                BG_128x64,
                BG_128x128,
                BG_128x256,
                BG_256x128,
                BG_256x256
            };

            class StreamingBackgroundBase {
                public:
                    StreamingBackgroundBase(TiledBackgroundBase *background_to_use,
                                            Vector2 map_tile_update_threshold);

                    bool load_from_arrays(const unsigned int *tiles, const unsigned int tiles_len,
                                          const unsigned short *palette, const unsigned int pal_len,
                                          const unsigned short **tilemaps, const unsigned short tilemaps_len,
                                          StreamingBackgroundSize size);

                    bool load_from_files(const unsigned int *tiles, const unsigned int tiles_len,
                                         const unsigned short *palette, const unsigned int pal_len,
                                         const std::vector<std::string> &tilemap_file_paths,
                                         StreamingBackgroundSize size);


                    Vector2 get_global_scroll() const {
                        return m_global_scroll;
                    }

                    void set_global_scroll(Vector2 global_scroll) {
                        m_global_scroll = global_scroll;

                        reload_tiles();
                    }
                private:
                    Vector2 get_streaming_background_size_vector() const {
                        switch (m_background_size) {
                            case StreamingBackgroundSize::BG_64x128:
                                return Vector2(64, 128);
                            case StreamingBackgroundSize::BG_128x64:
                                return Vector2(128, 64);
                            case StreamingBackgroundSize::BG_128x128:
                                return Vector2(128, 128);
                            case StreamingBackgroundSize::BG_128x256:
                                return Vector2(128, 256);
                            case StreamingBackgroundSize::BG_256x128:
                                return Vector2(256, 128);
                            case StreamingBackgroundSize::BG_256x256:
                                return Vector2(256, 256);
                        }
                    }

                    //FILE *get_tilemap_file();

                    //void load_from_file(const std::string &tilemap_file_name);

                    void load_tile_from_file(FILE *tilemap_file, int current_tile_index, int vram_tile_index,
                                             int &previous_tile_index);
                    void load_tile_from_array(const unsigned short *tilemap_array, int current_tile_index,
                                              int vram_tile_index);

                    void refresh_current_background_file_pointer(FILE **background_file_pointer,
                                                                 unsigned int &current_background_number,
                                                                 const Vector2 &current_scroll_vector);
                    void refresh_current_background_array_pointer(const unsigned short **background_array_pointer,
                                                                  unsigned int &current_background_number,
                                                                  const Vector2 &current_scroll_vector);

                    void reload_tiles();
                    void reload_x_tiles(FILE **tilemap_file, const unsigned short **tilemap_array,
                                        Vector2 initial_scroll_vector,
                                        Vector2 initial_scroll_difference_vector,
                                        int &previous_tile_index);
                    void reload_y_tiles(Vector2 initial_scroll_vector,
                                        Vector2 initial_scroll_difference_vector);

                    void load_x_tile_strip(FILE *tilemap_file, const unsigned short *tilemap_array, const bool right,
                                           const Vector2 &global_scroll_offset_vector, int &previous_tile_index);
                    void load_y_tile_strip(FILE *tilemap_file, const unsigned short *tilemap_array, const bool down,
                                           const Vector2 &global_scroll_offset_vector, int &previous_tile_index);

                    StreamingBackgroundSize m_background_size;
                    std::unique_ptr<morpheus::core::gfx::TiledBackgroundBase> m_background;
                    std::vector<unsigned short> m_current_tile_map;
                    Vector2 m_global_scroll = Vector2(0, 0);
                    Vector2 m_last_file_update_at = Vector2(0, 0);
                    Vector2 m_map_tile_update_threshold;
                    std::vector<std::string> m_tilemap_file_paths;
                    std::vector<const unsigned short *> m_tilemaps;
                    bool m_using_files = false;
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_STREAMING_BACKGROUND_HPP
