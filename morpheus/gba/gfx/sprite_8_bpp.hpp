//
// Created by bobby on 22/12/2020.
//

#ifndef MORPHEUS_GBA_TEST_SPRITE_8_BPP_HPP
#define MORPHEUS_GBA_TEST_SPRITE_8_BPP_HPP

#include <gba/gfx/sprite.hpp>

namespace morpheus {
    namespace gba {
        namespace gfx {
            class Sprite8Bpp : public morpheus::gba::gfx::Sprite {
            public:
                Sprite8Bpp(GbaBlendingController *blending_controller, GbaMosaicController *mosaic_controller) :
                    morpheus::gba::gfx::Sprite(false, blending_controller, mosaic_controller) {}

                virtual ~Sprite8Bpp() = default;

                bool load_into_palette(const unsigned short *palette, const unsigned int palette_len)override;
            protected:
                void array_load(const unsigned short *tile_array, const unsigned int tile_array_len,
                                const core::gfx::SpriteSize size, const unsigned int tile_id)override;
                void array_load(const unsigned short *tile_array, const unsigned int tile_array_len,
                                const unsigned short *palette, const unsigned int palette_len,
                                const core::gfx::SpriteSize size, const unsigned int tile_id)override;
            private:
                unsigned int m_tile_id;
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_SPRITE_8_BPP_HPP
