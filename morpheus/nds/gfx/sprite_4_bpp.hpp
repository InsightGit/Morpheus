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
                explicit Sprite4Bpp(bool use_sub_display) :
                    nds::gfx::Sprite(use_sub_display, SpriteMapping_1D_32,
                                     ExtendedPaletteStatus::NOTNEEDED) {}

                bool load_from_array(const unsigned short *tile_array,  const unsigned int palette_id,
                                     const unsigned int width, const unsigned int height)override;
                bool load_from_array(const unsigned short *tile_array, const unsigned int palette_id,
                                     const unsigned short *palette, const unsigned int width,
                                     const unsigned int height)override;

                void draw_node(std::vector<void *>obj_attr_buffer, int obj_attr_num, int priority)override;
            protected:
                bool copy_into_palette(const unsigned short *palette, const unsigned int palette_id)override;

                virtual void input(core::InputEvent input_event)override {}
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_SPRITE_4_BPP_HPP
