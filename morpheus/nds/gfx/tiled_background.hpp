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
                                         std::shared_ptr<NdsMainLoop> main_loop, bool is_8bpp,
                                         unsigned int cbb_num, unsigned int sbb_num);

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
                bool m_use_sub_display;
                std::shared_ptr<NdsMainLoop> m_main_loop;

                int m_background_reference_num = -1;

                BgSize m_background_size;
                BgType m_background_type;
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_TILED_BACKGROUND_BASE_HPP
