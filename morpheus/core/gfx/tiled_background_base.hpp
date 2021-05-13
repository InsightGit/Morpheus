//
// Created by bobby on 23/01/2021.
//

#ifndef MORPHEUS_GBA_TEST_TILED_BACKGROUND_BASE_HPP
#define MORPHEUS_GBA_TEST_TILED_BACKGROUND_BASE_HPP

#include <algorithm>

#include <tonc.h>

#include <core/main_loop.hpp>
#include <core/gfx/vector_2.hpp>

namespace morpheus {
    namespace core {
        namespace gfx {
            enum class TiledBackgroundSize {
                BG_32x32,
                BG_64x32,
                BG_32x64,
                BG_64x64,

                BG_AFFINE_16x16,
                BG_AFFINE_32x32,
                BG_AFFINE_64x64,
                BG_AFFINE_128x128
            };

            class TiledBackgroundBase {
            public:
                TiledBackgroundBase(bool affine, unsigned int background_num, BlendingController *blending_controller,
                                    MosaicController *mosaic_controller, unsigned int cbb_num, unsigned int sbb_num);

                virtual ~TiledBackgroundBase() = default;

                void load_from_array(const unsigned int *tiles, const unsigned int tiles_len,
                                     const unsigned short *palette, const unsigned int pal_len,
                                     const unsigned short *tile_map, const unsigned int tile_map_len,
                                     TiledBackgroundSize size) {
                    m_tile_map = tile_map;
                    m_tile_map_size = size;

                    array_load(tiles, tiles_len, palette, pal_len, tile_map, tile_map_len, size);
                }
                void load_from_array(const unsigned int *tiles, const unsigned int tiles_len,
                                     const unsigned short *tile_map, const unsigned int tile_map_len,
                                     TiledBackgroundSize size) {
                    m_tile_map = tile_map;
                    m_tile_map_size = size;

                    array_load(tiles, tiles_len, tile_map, tile_map_len, size);
                }

                void disable_blending() {
                    m_blending_controller->disable_background_blending(get_background_num());
                }

                void enable_blending(bool bottom) {
                    // insures both top and bottom blending bits aren't set at the same time
                    disable_blending();

                    m_blending_controller->enable_background_blending(bottom, get_background_num());
                }

                unsigned int get_affine_index() const {
                    if(m_affine) {
                        return m_affine_index;
                    } else {
                        return 32u;
                    }
                }

                unsigned int get_background_num() const {
                    return m_background_num;
                }

                unsigned int get_cbb_num() const {
                    return m_cbb_num;
                }

                Vector2 get_mosaic_levels() const {
                    return m_mosaic_controller->get_background_mosaic_levels();
                }

                int get_rotation() const {
                    return m_rotation;
                }

                Vector2 get_scale() const {
                    return m_scale;
                }

                unsigned int get_sbb_num() const {
                    return m_sbb_num;
                }

                Vector2 get_scroll() const {
                    return m_scroll_position;
                }

                bool is_affine() const {
                    return m_affine;
                }

                bool is_mosaic() const {
                    return m_mosaic;
                }

                void set_affine_index(const unsigned int affine_index) {
                    if(m_affine) {
                        m_affine_index = std::min(31u, affine_index);
                    }
                }

                void set_mosaic_levels(morpheus::core::gfx::Vector2 mosaic_levels) {
                    m_mosaic_controller->set_background_mosaic_levels(mosaic_levels);
                }

                void set_rotation(const int rotation) {
                    if(m_affine) {
                        m_rotation = rotation;

                        affine_state_updated();
                    }
                }

                void set_scale(const core::gfx::Vector2 scale) {
                    if(m_affine) {
                        m_scale = scale;

                        affine_state_updated();
                    }
                }

                void set_scroll(Vector2 scroll_position) {
                    m_scroll_position = scroll_position % ((get_size_vector() * Vector2(8, 8)) - TILE_SCREEN_SIZE);

                    update_scroll();
                }

                void toggle_mosaic() {
                    m_mosaic = !m_mosaic;

                    mosaic_state_updated();
                }

                int get_tile_id_at_position(core::gfx::Vector2 position, bool with_scrolling = true);

                virtual unsigned int get_priority() const = 0;
                virtual void set_priority(unsigned int priority) = 0;
            protected:
                virtual void affine_state_updated() = 0;
                virtual void array_load(const unsigned int *tiles, const unsigned int tiles_len,
                                        const unsigned short *palette, const unsigned int pal_len,
                                        const unsigned short *tile_map, const unsigned int tile_map_len,
                                        TiledBackgroundSize size) = 0;
                virtual void array_load(const unsigned int *tiles, const unsigned int tiles_len,
                                        const unsigned short *tile_map, const unsigned int tile_map_len,
                                        TiledBackgroundSize size) = 0;
                virtual void mosaic_state_updated() = 0;
                virtual void update_scroll() = 0;
            private:
                const static Vector2 TILE_SCREEN_SIZE;

                Vector2 get_size_vector() const {
                    switch(m_tile_map_size) {
                        case TiledBackgroundSize::BG_32x32:
                            return Vector2(32, 32);
                        case TiledBackgroundSize::BG_64x32:
                            return Vector2(64, 32);
                        case TiledBackgroundSize::BG_32x64:
                            return Vector2(32, 64);
                        case TiledBackgroundSize::BG_64x64:
                            return Vector2(64, 64);
                        case TiledBackgroundSize::BG_AFFINE_16x16:
                            return Vector2(16, 16);
                        case TiledBackgroundSize::BG_AFFINE_32x32:
                            return Vector2(32, 32);
                        case TiledBackgroundSize::BG_AFFINE_64x64:
                            return Vector2(64, 64);
                        case TiledBackgroundSize::BG_AFFINE_128x128:
                            return Vector2(128, 128);
                    }

                    return Vector2(0, 0);
                }

                bool m_affine;
                unsigned int m_affine_index;
                gfx::BlendingController *m_blending_controller;
                unsigned int m_background_num;
                unsigned int m_cbb_num;
                bool m_mosaic = false;
                gfx::MosaicController *m_mosaic_controller;
                int m_rotation = 0;
                Vector2 m_scale = Vector2(1 << 8, 1 << 8);
                Vector2 m_scroll_position;
                unsigned int m_sbb_num;
                const unsigned short *m_tile_map;
                TiledBackgroundSize m_tile_map_size;
            };
        }
    }
}


#endif //MORPHEUS_GBA_TEST_TILED_BACKGROUND_BASE_HPP
