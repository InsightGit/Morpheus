//
// Created by bobby on 24/12/2020.
//

#ifndef MORPHEUS_GBA_TEST_TILED_BACKGROUND_HPP
#define MORPHEUS_GBA_TEST_TILED_BACKGROUND_HPP

#include <cassert>
#include <cstring>
#include <memory>

#include <tonc.h>

#include <gba/gba_main_loop.hpp>

#include <core/gfx/tiled_background_base.hpp>

namespace morpheus {
    namespace gba {
        namespace gfx {
            class TiledBackground : public core::gfx::TiledBackgroundBase {
            public:
                explicit TiledBackground(unsigned int background_num, GbaBlendingController *blending_controller,
                                         GbaMainLoop *main_loop, bool is_8bpp, unsigned int cbb_num,
                                         unsigned int sbb_num);

                virtual ~TiledBackground() = default;

                morpheus::core::gfx::Vector2 get_mosaic_levels() const override {
                    return morpheus::core::gfx::Vector2(REG_MOSAIC & MOS_BH_MASK, REG_MOSAIC & MOS_BV_MASK);
                }

                unsigned int get_priority() const override {
                    return m_background_priority;
                }

                void set_mosaic_levels(morpheus::core::gfx::Vector2 mosaic_levels) override {
                    REG_MOSAIC = MOS_BH(mosaic_levels.get_x()) | MOS_BV(mosaic_levels.get_y());
                }

                void set_priority(const unsigned int priority) override {
                    m_background_priority = std::min(3u, priority);

                    update_background_register();
                }

                void load_from_array(const unsigned int *tiles, const unsigned int tiles_len,
                                     const unsigned short *palette, const unsigned int pal_len,
                                     const unsigned short *tile_map, const unsigned int tile_map_len,
                                     core::gfx::TiledBackgroundSize size)override;
                void load_from_array(const unsigned int *tiles, const unsigned int tiles_len,
                                     const unsigned short *tile_map, const unsigned int tile_map_len,
                                     core::gfx::TiledBackgroundSize size)override;
            protected:
                void update_scroll()override;
            private:
                void update_background_register();

                unsigned int m_background_priority;
                unsigned int m_background_register;
                bool m_is_8bpp;
                GbaMainLoop *m_main_loop;
                bool m_main_loop_notified = false;
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_TILED_BACKGROUND_BASE_HPP
