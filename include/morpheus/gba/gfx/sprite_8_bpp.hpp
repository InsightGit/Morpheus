//
// Created by bobby on 22/12/2020.
//

#ifndef MORPHEUS_GBA_TEST_SPRITE_8_BPP_HPP
#define MORPHEUS_GBA_TEST_SPRITE_8_BPP_HPP

#include <morpheus/gba/gfx/sprite.hpp>

namespace morpheus {
    namespace gba {
        namespace gfx {
            class Sprite8Bpp : public morpheus::gba::gfx::Sprite {
            public:
                /// Constructs a Sprite8Bpp, or a 256 color sprite.
                /// \param affine Whether this Sprite8Bpp is affine or not
                /// \param blending_controller The GbaBlendingController for
                /// this Sprite8Bpp to use
                /// \param mosaic_controller The GbaMosaicController for
                /// this Sprite8Bpp to use
                Sprite8Bpp(bool affine, GbaBlendingController *blending_controller,
                           GbaMosaicController *mosaic_controller) :
                    morpheus::gba::gfx::Sprite(affine, false, blending_controller, mosaic_controller) {}

                /// Destructs the Sprite8Bpp object.
                virtual ~Sprite8Bpp() = default;

                bool load_into_palette(const unsigned short *palette, const unsigned int palette_len,
                                       const unsigned int pal_offset = 256)override;
            protected:
                void array_load(const unsigned short *tile_array, const unsigned int tile_array_len,
                                const core::gfx::SpriteSize size, const unsigned int tile_id)override;
                void array_load(const unsigned short *tile_array, const unsigned int tile_array_len,
                                const unsigned short *palette, const unsigned int palette_len,
                                const core::gfx::SpriteSize size, const unsigned int tile_id)override;
            private:
                unsigned int m_tile_id;
            };

            /// \class morpheus::gba::gfx::Sprite8Bpp
            /// An 8BPP (8 bits per pixel or 256 color) implementation of the
            /// morpheus::gba::gfx::Sprite and
            /// morpheus::core::gfx::SpriteBase classes. Uses the entire GBA
            /// Sprite (or Object/OBJ) palette as one single palette of
            /// 256 colors. For more information about this class, consult the
            /// aforementioned parent class and grandparent class.
        }
    }
}

#endif //MORPHEUS_GBA_TEST_SPRITE_8_BPP_HPP
