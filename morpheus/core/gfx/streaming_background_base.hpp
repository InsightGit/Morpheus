//
// Created by bobby on 25/05/2021.
//

#ifndef MORPHEUS_GBA_TEST_STREAMING_BACKGROUND_BASE_HPP
#define MORPHEUS_GBA_TEST_STREAMING_BACKGROUND_BASE_HPP

#include <memory>

#include "tiled_background_base.hpp"

namespace morpheus {
    namespace core {
        namespace gfx {
            enum class StreamingBackgroundSize {
                BG_128x128,
                // TODO(Bobby): Test 256x256 and 512x512
                BG_256x256,
                BG_512x512
            };

            class StreamingBackgroundBase {
                public:
                    StreamingBackgroundBase(TiledBackgroundBase *background_to_use,
                                            const Vector2 &map_tile_update_threshold,
                                            const Vector2 &player_position = Vector2(0, 0),
                                            const bool enable_wrapping = false);

                    bool load_from_arrays(const unsigned int *tiles, const unsigned int tiles_len,
                                          const unsigned short *palette, const unsigned int pal_len,
                                          const unsigned short **tilemaps, const unsigned short tilemaps_len,
                                          StreamingBackgroundSize size);

                    /*bool load_from_files(const unsigned int *tiles, const unsigned int tiles_len,
                                         const unsigned short *palette, const unsigned int pal_len,
                                         const std::vector<std::string> &tilemap_file_paths,
                                         StreamingBackgroundSize size);*/


                    Vector2 get_global_scroll() const {
                        return m_global_scroll;
                    }

                    void set_global_scroll(Vector2 global_scroll);

                    void set_player_position(Vector2 player_position) {
                        m_player_position = player_position;
                    }

                    Vector2 get_player_position() const {
                        return m_player_position;
                    }
                private:
                    Vector2 get_streaming_background_size_vector() const {
                        switch (m_background_size) {
                            case StreamingBackgroundSize::BG_128x128:
                                return Vector2(128, 128);
                            case StreamingBackgroundSize::BG_256x256:
                                return Vector2(256, 256);
                            case StreamingBackgroundSize::BG_512x512:
                                return Vector2(512, 512);
                        }

                        return Vector2(0, 0);
                    }

                    //FILE *get_tilemap_file();

                    //void load_from_file(const std::string &tilemap_file_name);

                    void load_tile_from_file(FILE *tilemap_file, int current_tile_index, int &previous_tile_index);
                    void load_tile_from_array(const unsigned short *tilemap_array, int current_tile_index);

                    void refresh_current_background_file_pointer(FILE **background_file_pointer,
                                                                 unsigned int &current_background_number,
                                                                 const Vector2 &current_scroll_vector);
                    const unsigned short *refresh_current_background_array_pointer(
                                                                  const unsigned short *background_array_pointer,
                                                                  unsigned int &current_background_number,
                                                                  const Vector2 &current_scroll_vector);

                    void reload_tiles();

                    void reload_x_tiles(const bool scrolling_right, const bool double_refresh);
                    void reload_y_tiles(const bool scrolling_down, const bool double_refresh);

                    void reload_x_tile_strip(const int x_start_value, const int x_end_value,
                                             const int y_value, FILE **tilemap_file,
                                             const unsigned short *tilemap_array,
                                             unsigned int &current_background_number,
                                             int &previous_tile_index);
                    void reload_y_tile_strip(const int y_start_value, const int y_end_value,
                                             const int x_value, FILE **tilemap_file,
                                             const unsigned short *tilemap_array,
                                             unsigned int &current_background_number,
                                             int &previous_tile_index);

                    StreamingBackgroundSize m_background_size;
                    std::unique_ptr<morpheus::core::gfx::TiledBackgroundBase> m_background;
                    std::vector<unsigned short> m_current_tile_map;
                    bool m_enable_wrapping;
                    Vector2 m_global_scroll = Vector2(0, 0);
                    Vector2 m_last_file_update_at = Vector2(0, 0);
                    bool m_last_scrolled_x = false;
                    Vector2 m_map_tile_update_threshold;
                    Vector2 m_player_position;
                    std::vector<std::string> m_tilemap_file_paths;
                    const unsigned short **m_tilemaps;
                    unsigned int m_tilemaps_len;
                    bool m_using_files = false;
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_STREAMING_BACKGROUND_HPP
