//
// Created by bobby on 26/01/2021.
//

#ifndef MORPHEUS_GBA_TEST_TILED_BACKGROUND_4_BPP_HPP
#define MORPHEUS_GBA_TEST_TILED_BACKGROUND_4_BPP_HPP

#include <morpheus/nds/gfx/tiled_background.hpp>

namespace morpheus {
    namespace nds {
        namespace gfx {
            class TiledBackground4Bpp : public TiledBackground {
            public:
                /// Constructs a TiledBackground4Bpp.
                /// \param affine Whether this TiledBackground4Bpp is affine or
                /// not (Affine backgrounds not fully supported)
                /// \param use_sub_display Whether to display this
                /// TiledBackground4Bpp on the bottom sub display (true) or the
                /// top main display (false).
                /// \param background_num The [0-3] background number of this
                /// TiledBackground4Bpp.
                /// \param blending_controller The optional
                /// NdsBlendingController for this TiledBackground4Bpp.
                /// \param mosaic_controller The optional
                /// NdsMosaicController for this TiledBackground4Bpp.
                /// \param main_loop A pointer to the current NdsMainLoop
                /// \param cbb_num The [0-3] tile offset to load this
                /// TiledBackground4Bpp object's tile graphics data onto.
                /// \param sbb_num The [0-31] tilemap offset to load this
                /// TiledBackground4Bpp object's tilemap onto.
                explicit TiledBackground4Bpp(bool affine, bool use_sub_display, unsigned int background_num,
                                             NdsBlendingController *blending_controller,
                                             NdsMosaicController *mosaic_controller,
                                             NdsMainLoop *main_loop, unsigned int cbb_num, unsigned int sbb_num) :
                                             TiledBackground(affine, use_sub_display, background_num,
                                                             blending_controller, mosaic_controller, main_loop, cbb_num,
                                                             sbb_num) {
                    if(affine) {
                        init_background_reference_num(BgType_Rotation);
                    } else {
                        init_background_reference_num(BgType_Text4bpp);
                    }
                }

                /// Destructs the TiledBackground4Bpp object.
                virtual ~TiledBackground4Bpp() = default;
            protected:
                void array_load(const unsigned int *tiles, const unsigned int tiles_len,
                                const unsigned short *palette, const unsigned int pal_len,
                                const unsigned short *tile_map, const unsigned int tile_map_len,
                                const core::gfx::TiledBackgroundSize size,
                                const core::gfx::BitUnpacking unpacking_needed = core::gfx::BitUnpacking::NONE,
                                const unsigned int palette_offset = 0)override;
                void array_load(const unsigned int *tiles, const unsigned int tiles_len,
                                const unsigned short *tile_map, const unsigned int tile_map_len,
                                core::gfx::TiledBackgroundSize size,
                                const core::gfx::BitUnpacking unpacking_needed = core::gfx::BitUnpacking::NONE)override;
                void array_load(const unsigned short *tile_map, const unsigned int tile_map_len,
                                core::gfx::TiledBackgroundSize size)override;
            };

            /// \class morpheus::nds::gfx::TiledBackground4Bpp
            /// Nintendo DS implementation of
            /// morpheus::nds::gfx::TiledBackground and
            /// morpheus::core::gfx::TiledBackgroundBase. Represents tiled
            /// (or "text" mode) backgrounds of 4bpp (or with 16 colors) that
            /// can be displayed on the bottom sub display or the top main
            /// display of the DS.
            /// Doesn't require the extended palette to be enabled or disabled
            /// to properly display the background (but still only uses the
            /// first 256 colors, still dividing it into 16 palettes of 16
            /// colors). For more details about this class, consult the
            /// documentation of the aforementioned parent and grandparent
            /// class.
        }
    }
}


#endif //MORPHEUS_GBA_TEST_TILED_BACKGROUND_4_BPP_HPP
