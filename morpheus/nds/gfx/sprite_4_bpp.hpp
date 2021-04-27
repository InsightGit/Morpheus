//
// Created by bobby on 16/12/2020.
//

#ifndef MORPHEUS_GBA_TEST_SPRITE_4_BPP_HPP
#define MORPHEUS_GBA_TEST_SPRITE_4_BPP_HPP

#include <iostream>

#include <nds/gfx/sprite.hpp>

namespace morpheus {
    namespace nds {
        namespace gfx {
            class Sprite4Bpp : public nds::gfx::Sprite {
            public:
                explicit Sprite4Bpp(bool affine, bool use_sub_display, NdsBlendingController *blending_controller,
                                    NdsMosaicController *mosaic_controller) :
                    nds::gfx::Sprite(affine, use_sub_display, blending_controller, mosaic_controller,
                                     SpriteMapping_1D_32,ExtendedPaletteStatus::NOTNEEDED) {}
                explicit Sprite4Bpp(bool affine, bool use_sub_display, NdsBlendingController *blending_controller,
                                    NdsMosaicController *mosaic_controller, unsigned short *nds_oam_address,
                                    core::gfx::SpriteSize size) :
                    nds::gfx::Sprite(affine, use_sub_display, blending_controller, mosaic_controller,
                                     SpriteMapping_1D_32, ExtendedPaletteStatus::NOTNEEDED, nds_oam_address, size) {}

                ~Sprite4Bpp() override = default;

                bool load_from_array(const unsigned short *tile_array, const unsigned int tile_array_len,
                                     const unsigned int palette_id,
                                     const morpheus::core::gfx::SpriteSize size)override;
                bool load_from_array(const unsigned short *tile_array, const unsigned int tile_array_len,
                                     const unsigned short *palette, const unsigned int palette_len,
                                     const unsigned int palette_id, core::gfx::SpriteSize size)override;

                bool load_into_palette(const unsigned short *palette, const unsigned int pal_len)override;

                void draw_node(std::vector<void *> &obj_attr_buffer, unsigned int obj_attr_num)override;
            protected:
                virtual void input(core::InputEvent input_event) override {}
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_SPRITE_4_BPP_HPP
