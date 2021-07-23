//
// Created by bobby on 24/12/2020.
//

#ifndef MORPHEUS_GBA_TEST_TILED_BACKGROUND_HPP
#define MORPHEUS_GBA_TEST_TILED_BACKGROUND_HPP

#include <cassert>
#include <cstring>
#include <memory>

#include <tonc.h>

#include <morpheus/gba/gba_main_loop.hpp>

#include <morpheus/core/gfx/tiled_background_base.hpp>

namespace morpheus {
    namespace gba {
        namespace gfx {
            class TiledBackground : public core::gfx::TiledBackgroundBase {
            public:
                /// Constructs a TiledBackground object.
                /// \param affine Whether this background is affine
                /// (affine backgrounds currently unsupported).
                /// \param background_num The background number of the
                /// TiledBackground object
                /// \param blending_controller The optional
                /// GbaBlendingController to be set on this
                /// TiledBackground object
                /// \param mosaic_controller The optional
                /// GbaMosaicController to be set on this
                /// TiledBackground object
                /// \param main_loop A pointer to the GbaMainLoop
                /// \param is_8bpp Whether this TiledBackground is
                /// 8bpp (256 color) (true) or 4bpp (16 color) (false)
                /// \param cbb The [0-3] tile offset to load this
                /// TiledBackground object's tile graphics data onto.
                /// \param sbb The [0-31] tilemap offset to load this
                /// TiledBackground object's tilemap onto.
                explicit TiledBackground(bool affine, unsigned int background_num,
                                         GbaBlendingController *blending_controller,
                                         GbaMosaicController *mosaic_controller, GbaMainLoop *main_loop,
                                         bool is_8bpp, unsigned int cbb_num, unsigned int sbb_num);

                /// Destroys a TiledBackground object.
                virtual ~TiledBackground() = default;

                unsigned int get_priority() const override {
                    return m_background_priority;
                }

                void set_priority(const unsigned int priority) override {
                    m_background_priority = std::min(3u, priority);

                    update_background_register();
                }
            protected:
                void array_load(const unsigned int *tiles, const unsigned int tiles_len,
                                const unsigned short *palette, const unsigned int pal_len,
                                const unsigned short *tile_map, const unsigned int tile_map_len,
                                const core::gfx::TiledBackgroundSize size,
                                const core::gfx::BitUnpacking unpacking_needed = core::gfx::BitUnpacking::NONE,
                                const unsigned int palette_offset = 0)override;
                void array_load(const unsigned int *tiles, const unsigned int tiles_len,
                                const unsigned short *tile_map, const unsigned int tile_map_len,
                                const core::gfx::TiledBackgroundSize size,
                                const core::gfx::BitUnpacking unpacking_needed = core::gfx::BitUnpacking::NONE)override;
                void array_load(const unsigned short *tile_map, const unsigned int tile_map_len,
                                core::gfx::TiledBackgroundSize size)override;

                void override_map_tile(const unsigned int tile_index, const unsigned short tile_id)override {
                    se_mem[get_sbb_num()][tile_index] = tile_id;
                }

                void affine_state_updated()override {}
                void mosaic_state_updated()override;
                void update_scroll()override;
            private:
                bool unpack_if_needed(const core::gfx::BitUnpacking unpacking_needed,
                                      const unsigned int *tiles, const unsigned int tiles_len);
                void update_background_register();

                unsigned int m_background_priority;
                unsigned int m_background_register;
                BG_AFFINE m_bg_affine = bg_aff_default;
                AFF_SRC_EX m_bg_affine_src_ex;
                bool m_is_8bpp;
                GbaMainLoop *m_main_loop;
                bool m_main_loop_notified = false;
            };

            /// \class morpheus::gba::gfx::TiledBackground
            /// The GBA implementation of the
            /// morpheus::core::gfx::TiledBackgroundBase class.
            /// Represents tiled backgrounds from 32x32 tiles
            /// (or 256 px x 256 px) in size to 64x64 tiles
            /// (or 512 px x 512 px), as well as backgrounds with
            /// 8bpp (256 color) tiles or 4bpp (16 color) tiles.
            /// For larger backgrounds, pass a new TiledBackground
            /// object into morpheus::core::gfx::StreamingBackgroundBase.
            /// For more details about this class, consult the documentation
            /// of the aforementioned parent class.
        }
    }
}

#endif //MORPHEUS_GBA_TEST_TILED_BACKGROUND_BASE_HPP
