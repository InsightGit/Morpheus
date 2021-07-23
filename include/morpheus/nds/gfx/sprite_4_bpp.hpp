//
// Created by bobby on 16/12/2020.
//

#ifndef MORPHEUS_GBA_TEST_SPRITE_4_BPP_HPP
#define MORPHEUS_GBA_TEST_SPRITE_4_BPP_HPP

#include <iostream>

#include <morpheus/nds/gfx/sprite.hpp>

namespace morpheus {
    namespace nds {
        namespace gfx {
            class Sprite4Bpp : public nds::gfx::Sprite {
            public:
                /// Constructs a Sprite4Bpp, or a 16 color sprite.
                /// \param affine Whether this Sprite4Bpp is affine or not
                /// \param use_sub_display Whether to display this Sprite4Bpp on
                /// the bottom sub display (true) or the top main display
                /// (false)
                /// \param blending_controller The NdsBlendingController for
                /// this Sprite4Bpp
                /// \param mosaic_controller The NdsMosaicController for
                /// this Sprite4Bpp
                explicit Sprite4Bpp(bool affine, bool use_sub_display, NdsBlendingController *blending_controller,
                                    NdsMosaicController *mosaic_controller) :
                    nds::gfx::Sprite(affine, use_sub_display, blending_controller, mosaic_controller,
                                     SpriteMapping_1D_32,ExtendedPaletteStatus::NOTNEEDED) {}

                /// Constructs a Sprite4Bpp, or a 16 color sprite with a
                /// predetermined libnds graphics pointer and
                /// morpheus::core::gfx::SpriteSize.
                /// \param affine Whether this Sprite4Bpp is affine or not
                /// \param use_sub_display Whether to display this Sprite4Bpp on
                /// the bottom sub display (true) or the top main display
                /// (false)
                /// \param blending_controller The NdsBlendingController for
                /// this Sprite4Bpp
                /// \param mosaic_controller The NdsMosaicController for
                /// this Sprite4Bpp
                /// \param nds_oam_address The pre-determined libnds DS OAM
                /// graphics pointer for this Sprite4Bpp
                /// \param size The morpheus::core::gfx::SpriteSize for this
                /// Sprite4Bpp.
                explicit Sprite4Bpp(bool affine, bool use_sub_display, NdsBlendingController *blending_controller,
                                    NdsMosaicController *mosaic_controller, unsigned short *nds_oam_address,
                                    core::gfx::SpriteSize size) :
                    nds::gfx::Sprite(affine, use_sub_display, blending_controller, mosaic_controller,
                                     SpriteMapping_1D_32, ExtendedPaletteStatus::NOTNEEDED, nds_oam_address, size) {}

                /// Destructs the Sprite4Bpp object.
                ~Sprite4Bpp() override = default;

                bool load_from_array(const unsigned short *tile_array, const unsigned int tile_array_len,
                                     const unsigned int palette_id,
                                     const morpheus::core::gfx::SpriteSize size)override;
                bool load_from_array(const unsigned short *tile_array, const unsigned int tile_array_len,
                                     const unsigned short *palette, const unsigned int palette_len,
                                     const unsigned int palette_id, core::gfx::SpriteSize size)override;

                bool load_into_palette(const unsigned short *palette, const unsigned int pal_len,
                                       const unsigned int pal_offset = 256)override;

                void draw_node(std::vector<void *> &obj_attr_buffer, unsigned int obj_attr_num)override;
            protected:
                virtual void input(core::InputEvent input_event) override {}
            };

            /// \class morpheus::nds::gfx::Sprite4Bpp
            /// A 4BPP (4 bits per pixel or 16 color) implementation of the
            /// morpheus::nds::gfx::Sprite and
            /// morpheus::core::gfx::SpriteBase classes. Divides the first
            /// DS Sprite palette of 256 colors into 16 palettes of 16 colors.
            /// These sprites work regardless of the extended palette/single
            /// palette mode, but will only use the first 256 palette even
            /// if extended palette mode is enabled.
            /// For more information about this class, consult the
            /// aforementioned parent class and grandparent class.
        }
    }
}

#endif //MORPHEUS_GBA_TEST_SPRITE_4_BPP_HPP
