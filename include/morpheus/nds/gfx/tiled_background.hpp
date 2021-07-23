//
// Created by bobby on 21/01/2021.
//

#ifndef MORPHEUS_GBA_TEST_TILED_BACKGROUND_HPP
#define MORPHEUS_GBA_TEST_TILED_BACKGROUND_HPP

#include <nds.h>

#include <morpheus/core/core.hpp>
#include <morpheus/nds/nds_main_loop.hpp>

namespace morpheus {
    namespace nds {
        namespace gfx {
            class TiledBackground : public core::gfx::TiledBackgroundBase {
            public:
                /// Constructs a TiledBackground object. Note: This is
                /// an abstract class and thus shouldn't be manually
                /// constructed. If you want to construct a DS TiledBackground,
                /// construct either a TiledBackground4Bpp or
                /// TiledBackground8Bpp, both of which inherit and implement
                /// this abstract class.
                /// \param affine Whether this background is affine
                /// (affine backgrounds currently unsupported).
                /// \param use_sub_display Whether to display this
                /// TiledBackground on the bottom sub display or the top main
                /// display
                /// \param background_num The background number of the
                /// TiledBackground object
                /// \param blending_controller The optional
                /// GbaBlendingController to be set on this
                /// TiledBackground object
                /// \param mosaic_controller The optional
                /// GbaMosaicController to be set on this
                /// TiledBackground object
                /// \param main_loop A pointer to the GbaMainLoop
                /// \param cbb_num The [0-3] tile offset to load this
                /// TiledBackground object's tile graphics data onto.
                /// \param sbb_num The [0-31] tilemap offset to load this
                /// TiledBackground object's tilemap onto.
                explicit TiledBackground(bool affine, bool use_sub_display, unsigned int background_num,
                                         NdsBlendingController *blending_controller,
                                         NdsMosaicController *mosaic_controller,
                                         NdsMainLoop *main_loop,
                                         unsigned int cbb_num, unsigned int sbb_num);

                /// Destroys a TiledBackground object.
                virtual ~TiledBackground() = default;

                unsigned int get_priority() const override {
                    if(m_background_reference_num >= 0) {
                        return bgGetPriority(m_background_reference_num);
                    } else {
                        return -1;
                    }
                }

                void set_priority(unsigned int priority) override {
                    if(m_background_reference_num >= 0) {
                        bgSetPriority(m_background_reference_num, priority);
                    }
                }

            protected:
                volatile unsigned short *get_background_register() const {
                    return m_background_register;
                }

                /// \return The internally-used libnds background reference
                /// number of this TiledBackground.
                int get_background_reference_num() const {
                    return m_background_reference_num;
                }

                /// \return The internally-used libnds background size
                /// of this TiledBackground.
                BgSize get_background_size() const {
                    return m_background_size;
                }

                /// \return Whether this
                bool is_using_sub_display() const {
                    return m_use_sub_display;
                }

                void override_map_tile(const unsigned int tile_index, const unsigned short tile_id) override {
                    if(m_background_reference_num >= 0) {
                        *(bgGetMapPtr(m_background_reference_num) + tile_index) = tile_id;
                    }
                }

                void affine_state_updated()override;
                void mosaic_state_updated()override;

                ///
                /// \param background_type
                void init_background_reference_num(BgType background_type);

                ///
                /// \param size
                void set_background_size(core::gfx::TiledBackgroundSize size);
                void update_scroll()override;
            private:
                volatile unsigned short *m_background_register;
                bool m_use_sub_display;
                NdsMainLoop *m_main_loop;

                int m_background_reference_num = -1;

                BgSize m_background_size;
            };

            /// \class morpheus::nds::gfx::TiledBackground
            /// The DS implementation of the
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
