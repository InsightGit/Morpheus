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
                Sprite8Bpp() : morpheus::gba::gfx::Sprite(false) {}

                void load_into_palette(const unsigned short *palette, const unsigned int palette_id,
                                       const unsigned int pal_len = 512)override;
            protected:
                void array_load(const unsigned short *tile_array, const unsigned short width,
                                const unsigned short height, const unsigned short tile_id)override;
                void array_load(const unsigned short *tile_array, const unsigned short *palette,
                                const unsigned short width, const unsigned short height,
                                const unsigned short tile_id)override;
            private:
                int m_tile_id;
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_SPRITE_8_BPP_HPP
