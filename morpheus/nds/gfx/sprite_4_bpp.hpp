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
                explicit Sprite4Bpp(bool use_sub_display, NdsBlendingController *blending_controller) :
                    nds::gfx::Sprite(use_sub_display, blending_controller, SpriteMapping_1D_32,
                                     ExtendedPaletteStatus::NOTNEEDED) {}
                explicit Sprite4Bpp(bool use_sub_display, NdsBlendingController *blending_controller,
                                    unsigned short *nds_oam_address, unsigned char width, unsigned char height) :
                    nds::gfx::Sprite(use_sub_display, blending_controller, SpriteMapping_1D_32,
                                     ExtendedPaletteStatus::NOTNEEDED,
                                     nds_oam_address, width, height) {}

                ~Sprite4Bpp() override = default;

                bool load_from_array(const unsigned short *tile_array, const unsigned int palette_id,
                                     const unsigned int width, const unsigned int height)override;
                bool load_from_array(const unsigned short *tile_array, const unsigned short *palette,
                                     const unsigned int palette_len, const unsigned int palette_id,
                                     const unsigned int width, const unsigned int height)override;

                bool load_into_palette(const unsigned short *palette, const unsigned int pal_len)override;

                void draw_node(std::vector<void *> &obj_attr_buffer, int obj_attr_num, int priority)override;
            protected:
                virtual void input(core::InputEvent input_event)override {}
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_SPRITE_4_BPP_HPP
