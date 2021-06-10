//
// Created by bobby on 25/05/2021.
//

#ifndef MORPHEUS_GBA_TEST_STREAMING_BACKGROUND_HPP
#define MORPHEUS_GBA_TEST_STREAMING_BACKGROUND_HPP

#include <fat.h>

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

            class StreamingBackgroundBase : public core::gfx::TiledBackgroundBase {
                public:
                    StreamingBackgroundBase(bool affine, unsigned int backgroundNum,
                                            BlendingController *blendingController, MosaicController *mosaicController,
                                            unsigned int cbbNum, unsigned int sbbNum,
                                            Vector2 map_tile_update_threshold = Vector2(30, 20));

                    void load_from_files(const unsigned int *tiles, const unsigned int tiles_len,
                                         const unsigned short *palette, const unsigned int pal_len,
                                         const std::vector<std::string> &tilemap_file_paths,
                                         StreamingBackgroundSize size);


                    Vector2 get_global_scroll() const {
                        return m_global_scroll;
                    }

                    void set_global_scroll(Vector2 global_scroll) {
                        morpheus::core::gfx::Vector2 global_scroll_difference = global_scroll - m_last_file_update_at;

                        m_global_scroll = global_scroll;

                        if(global_scroll_difference.get_x() > (m_map_tile_update_threshold.get_x() * 8)) {
                            load_x_tiles(global_scroll_difference.get_x() < 0);
                        } else if(global_scroll_difference.get_y() > (m_map_tile_update_threshold.get_y() * 8)) {
                            load_y_tiles(global_scroll_difference.get_y() < 0);
                        }
                    }
                protected:
                    virtual void load_tiles_and_palette(const unsigned int *tiles, const unsigned int tiles_len,
                                                        const unsigned short *palette, const unsigned int pal_len) = 0;

                    virtual void load_tilemap() = 0;
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

                    //void load_from_file(const std::string &tilemap_file_name);
                    void load_x_tiles(bool right);
                    void load_y_tiles(bool up);

                    StreamingBackgroundSize m_background_size;
                    std::vector<unsigned short> m_current_tile_map;
                    Vector2 m_global_scroll = Vector2(0, 0);
                    Vector2 m_last_file_update_at = Vector2(0, 0);
                    Vector2 m_map_tile_update_threshold;
                    std::vector<std::string> m_tilemap_file_paths;
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_STREAMING_BACKGROUND_HPP
