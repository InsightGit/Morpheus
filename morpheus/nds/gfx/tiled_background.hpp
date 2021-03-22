//
// Created by bobby on 21/01/2021.
//

#ifndef MORPHEUS_GBA_TEST_TILED_BACKGROUND_HPP
#define MORPHEUS_GBA_TEST_TILED_BACKGROUND_HPP

#include <nds.h>

#include <core/core.hpp>
#include <nds/nds_main_loop.hpp>

namespace morpheus {
    namespace nds {
        namespace gfx {
            class TiledBackground : public core::gfx::TiledBackgroundBase {
            public:
                explicit TiledBackground(bool use_sub_display, unsigned int background_num,
                                         NdsBlendingController *blending_controller,
                                         NdsMainLoop *main_loop,
                                         unsigned int cbb_num, unsigned int sbb_num);

                virtual ~TiledBackground() = default;

                morpheus::core::gfx::Vector2 get_mosaic_levels() const override {
                    if(m_use_sub_display) {
                        return morpheus::core::gfx::Vector2(REG_MOSAIC & 0x000F, REG_MOSAIC & 0x00F0);
                    } else {
                        return morpheus::core::gfx::Vector2(REG_MOSAIC_SUB & 0x000F, REG_MOSAIC_SUB & 0x00F0);
                    }
                }
                unsigned int get_priority() const override {
                    if(m_background_reference_num >= 0) {
                        return bgGetPriority(m_background_reference_num);
                    } else {
                        return -1;
                    }
                }

                void set_mosaic_levels(morpheus::core::gfx::Vector2 mosaic_levels) override {
                    if(m_use_sub_display) {
                        bgSetMosaicSub(mosaic_levels.get_x(), mosaic_levels.get_y());
                    } else {
                        bgSetMosaic(mosaic_levels.get_x(), mosaic_levels.get_y());
                    }
                }
                void set_priority(unsigned int priority) override {
                    if(m_background_reference_num >= 0) {
                        bgSetPriority(m_background_reference_num, priority);
                    }
                }

            protected:
                int get_background_reference_num() const {
                    return m_background_reference_num;
                }

                BgSize get_background_size() const {
                    return m_background_size;
                }

                bool is_using_sub_display() const {
                    return m_use_sub_display;
                }

                void init_background_reference_num(BgType background_type);
                void set_background_size(core::gfx::TiledBackgroundSize size);
                void update_scroll()override;
            private:
                bool m_use_sub_display;
                NdsMainLoop *m_main_loop;

                int m_background_reference_num = -1;

                BgSize m_background_size;
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_TILED_BACKGROUND_BASE_HPP
