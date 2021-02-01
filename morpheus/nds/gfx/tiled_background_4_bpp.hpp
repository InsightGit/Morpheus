//
// Created by bobby on 26/01/2021.
//

#ifndef MORPHEUS_GBA_TEST_TILED_BACKGROUND_4_BPP_HPP
#define MORPHEUS_GBA_TEST_TILED_BACKGROUND_4_BPP_HPP

#include <nds/gfx/tiled_background.hpp>

namespace morpheus {
    namespace nds {
        namespace gfx {
            class TiledBackground4Bpp : public TiledBackground {
            public:
                explicit TiledBackground4Bpp(bool use_sub_display, unsigned int background_num,
                                             std::shared_ptr<NdsMainLoop> main_loop,
                                             unsigned int cbb_num, unsigned int sbb_num) :
                                             TiledBackground(use_sub_display, background_num, main_loop,
                                                             cbb_num, sbb_num) {}

                void load_from_array(const unsigned int *tiles, const unsigned int tiles_len,
                                     const unsigned short *palette, const unsigned int pal_len,
                                     const unsigned short *tile_map, const unsigned int tile_map_len,
                                     core::gfx::TiledBackgroundSize size)override;
                void load_from_array(const unsigned int *tiles, const unsigned int tiles_len,
                                     const unsigned short *tile_map, const unsigned int tile_map_len,
                                     core::gfx::TiledBackgroundSize size)override;
            };
        }
    }
}


#endif //MORPHEUS_GBA_TEST_TILED_BACKGROUND_4_BPP_HPP
