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
                explicit TiledBackground(bool affine, bool use_sub_display, unsigned int background_num,
                                         NdsBlendingController *blending_controller,
                                         NdsMosaicController *mosaic_controller,
                                         NdsMainLoop *main_loop,
                                         unsigned int cbb_num, unsigned int sbb_num);

                virtual ~TiledBackground() = default;

                unsigned int get_priority() const override {
                    if(m_background_reference_num >= 0) {
                        return bgGetPriority(m_background_reference_num);
                    } else {
                        return -1;
                    }
                }

                void set_priority(unsigned int priority) override {
                    if(m_background_reference_num >= 0) {
                        bgSetPriority(m_background_reference_num, priority);
                    }
                }

            protected:
                volatile unsigned short *get_background_register() const {
                    return m_background_register;
                }

                int get_background_reference_num() const {
                    return m_background_reference_num;
                }

                BgSize get_background_size() const {
                    return m_background_size;
                }

                bool is_using_sub_display() const {
                    return m_use_sub_display;
                }

                void override_map_tile(const unsigned int tile_index, const unsigned short tile_id) override {
                    if(m_background_reference_num >= 0) {
                        *(bgGetMapPtr(m_background_reference_num) + tile_index) = tile_id;
                    }
                }

                void affine_state_updated()override;
                void mosaic_state_updated()override;

                void init_background_reference_num(BgType background_type);
                void set_background_size(core::gfx::TiledBackgroundSize size);
                void update_scroll()override;
            private:
                volatile unsigned short *m_background_register;
                bool m_use_sub_display;
                NdsMainLoop *m_main_loop;

                int m_background_reference_num = -1;

                BgSize m_background_size;
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_TILED_BACKGROUND_BASE_HPP
