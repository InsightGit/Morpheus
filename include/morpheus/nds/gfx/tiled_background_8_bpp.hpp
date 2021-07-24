//
// Created by bobby on 26/01/2021.
//

#ifndef MORPHEUS_GBA_TEST_TILED_BACKGROUND_8_BPP_HPP
#define MORPHEUS_GBA_TEST_TILED_BACKGROUND_8_BPP_HPP

#include <morpheus/nds/gfx/tiled_background.hpp>

namespace morpheus {
    namespace nds {
        namespace gfx {
            class TiledBackground8Bpp : public TiledBackground {
            public:
                /// Constructs a TiledBackground8Bpp.
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

                /// Destructs the TiledBackground8Bpp object.
                virtual ~TiledBackground8Bpp() = default;

                // Extended Palette Mode loading functions
                /// Loads grit-generated tile graphics data into this
                /// TiledBackground with a color palette when extended palette
                /// mode is enabled. If single palette mode is enabled,
                /// extended palette mode will be enabled.
                /// \param tiles The buffer of the grit-generated graphical tile
                /// data to load
                /// \param tiles_len The length of the graphical tile data
                /// buffer to load in bytes
                /// \param palette The buffer of the color palette data
                /// to load
                /// \param pal_len The length of the palette data buffer to load
                /// in bytes
                /// \param palette_id The 256-color extended palette [0-16] id
                /// to load this palette into.
                /// \param tile_map The buffer of the tilemap to load
                /// \param tile_map_len The length of the tilemap buffer to load
                /// \param size The background size to set on this TiledBackground8Bpp.
                void load_from_array(const unsigned int *tiles, const unsigned int tiles_len,
                                     const unsigned short *palette, const unsigned int pal_len,
                                     const unsigned int palette_id, const unsigned short *tile_map,
                                     const unsigned int tile_map_len, core::gfx::TiledBackgroundSize size);

            protected:
                // Single Palette Mode loading functions
                void array_load(const unsigned int *tiles, const unsigned int tiles_len,
                                const unsigned short *palette, const unsigned int pal_len,
                                const unsigned short *tile_map, const unsigned int tile_map_len,
                                core::gfx::TiledBackgroundSize size,
                                const core::gfx::BitUnpacking unpacking_needed = core::gfx::BitUnpacking::NONE,
                                const unsigned int palette_offset = 0)override;
                void array_load(const unsigned int *tiles, const unsigned int tiles_len,
                                const unsigned short *tile_map, const unsigned int tile_map_len,
                                core::gfx::TiledBackgroundSize size,
                                const core::gfx::BitUnpacking unpacking_needed = core::gfx::BitUnpacking::NONE)override;
                void array_load(const unsigned short *tile_map, const unsigned int tile_map_len,
                                core::gfx::TiledBackgroundSize size)override;
            };

            /// \class morpheus::nds::gfx::TiledBackground8Bpp
            /// Nintendo DS implementation of
            /// morpheus::nds::gfx::TiledBackground and
            /// morpheus::core::gfx::TiledBackgroundBase. Represents tiled
            /// (or "text" mode) backgrounds of 8bpp (or with 256 colors) that
            /// can be displayed on the bottom sub display or the top main
            /// display of the DS.
            /// Can either use single palette mode, having one palette of 256
            /// colors, or extended palette mode having 16 palettes of 256
            /// colors.
            /// For more details about this class, consult the
            /// documentation of the aforementioned parent and grandparent
            /// class.
        }
    }
}


#endif //MORPHEUS_GBA_TEST_TILED_BACKGROUND_8_BPP_HPP
