//
// Created by bobby on 26/01/2021.
//

#ifndef MORPHEUS_GBA_TEST_TILED_BACKGROUND_8_BPP_HPP
#define MORPHEUS_GBA_TEST_TILED_BACKGROUND_8_BPP_HPP

#include <nds/gfx/tiled_background.hpp>

namespace morpheus {
    namespace nds {
        namespace gfx {
            class TiledBackground8Bpp : public TiledBackground {
            public:
                explicit TiledBackground8Bpp(bool affine, bool use_sub_display, unsigned int background_num,
                                             NdsBlendingController *blending_controller,
                                             NdsMosaicController *mosaic_controller,
                                             NdsMainLoop *main_loop,
                                             unsigned int cbb_num, unsigned int sbb_num) :
                        TiledBackground(affine, use_sub_display, background_num, blending_controller, mosaic_controller,
                                        main_loop, cbb_num, sbb_num) {
                    if(affine) {
                        init_background_reference_num(BgType_Rotation);
                    } else {
                        init_background_reference_num(BgType_Text8bpp);
                    }
                }

                virtual ~TiledBackground8Bpp() = default;

                // Extended Palette Mode loading functions
                void load_from_array(const unsigned int *tiles, const unsigned int tiles_len,
                                     const unsigned short *palette, const unsigned int pal_len,
                                     const unsigned int palette_id, const unsigned short *tile_map,
                                     const unsigned int tile_map_len, core::gfx::TiledBackgroundSize size);

            protected:
                // Single Palette Mode loading functions
                void array_load(const unsigned int *tiles, const unsigned int tiles_len,
                                     const unsigned short *palette, const unsigned int pal_len,
                                     const unsigned short *tile_map, const unsigned int tile_map_len,
                                     core::gfx::TiledBackgroundSize size)override;
                void array_load(const unsigned int *tiles, const unsigned int tiles_len,
                                     const unsigned short *tile_map, const unsigned int tile_map_len,
                                     core::gfx::TiledBackgroundSize size)override;
            };
        }
    }
}


#endif //MORPHEUS_GBA_TEST_TILED_BACKGROUND_8_BPP_HPP
