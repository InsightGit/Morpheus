//
// Created by bobby on 22/12/2020.
//

#ifndef MORPHEUS_GBA_TEST_SPRITE_4_BPP_HPP
#define MORPHEUS_GBA_TEST_SPRITE_4_BPP_HPP

#include <morpheus/gba/gfx/sprite.hpp>

namespace morpheus {
    namespace gba {
        namespace gfx {
            class Sprite4Bpp : public morpheus::gba::gfx::Sprite {
            public:
                /// Constructs a Sprite4Bpp, or a 16-color sprite.
                /// \param affine Whether this Sprite4Bpp is affine or not
                /// \param blending_controller The GbaBlendingController for
                /// this Sprite4Bpp to use.
                /// \param mosaic_controller The GbaMosaicController for
                /// this Sprite4Bpp to use.
                /// \param palette_id The optional palette id that this
                /// Sprite4Bpp will use.
                explicit Sprite4Bpp(bool affine, GbaBlendingController *blending_controller,
                                    GbaMosaicController *mosaic_controller, const unsigned int palette_id = 0);

                /// Constructs a Sprite4Bpp, or a 16-color sprite, with a
                /// predetermined tile id and palette id.
                /// \param affine Whether this Sprite4Bpp is affine or not
                /// \param blending_controller The GbaBlendingController for
                /// this Sprite4Bpp to use.
                /// \param mosaic_controller The GbaMosaicController for
                /// this Sprite4Bpp to use.
                /// \param tile_id The optional GBA-specific [0-1024] tile id
                /// that this Sprite4Bpp will use.
                /// \param palette_id The optional [0-16] palette id that this
                /// Sprite4Bpp will use.
                /// \param size The morpheus::core::gfx::SpriteSize for this
                /// Sprite4Bpp to use.
                explicit Sprite4Bpp(bool affine, GbaBlendingController *blending_controller,
                                    GbaMosaicController *mosaic_controller, const unsigned int tile_id,
                                    const unsigned int palette_id, const core::gfx::SpriteSize size);

                /// Destructs the Sprite4Bpp object.
                virtual ~Sprite4Bpp() = default;

                /// \return The current palette id being used by this
                /// Sprite4Bpp.
                unsigned int get_palette_id() const {
                    return m_palette_id;
                }

                /// Sets the palette id to be used by this Sprite4Bpp.
                /// \param palette_id The palette id to be used
                void set_palette_id(const unsigned int palette_id) {
                    m_palette_id = std::min(palette_id, 15u);

                    build_attr2(m_palette_id, m_tile_id);
                }

                /// Loads grit-generated tile graphics data into this
                /// Sprite4Bpp with a [0-16] palette id and a palette.
                /// \param tile_array The grit-generated tile graphics
                /// buffer to load
                /// \param tile_array_len The length of the grit-generated
                /// tile graphics buffer
                /// \param palette The grit-generated color palette buffer
                /// to load
                /// \param palette_len The length of the grit-generated
                /// color palette buffer to load
                /// \param palette_id The [0-16] palette id to set on this
                /// Sprite4Bpp
                /// \param size The SpriteSize to be set on this Sprite4Bpp
                /// \param tile_id The GBA-specific tile_id to load the
                /// tile graphics buffer into
                void load_from_array(const unsigned short *tile_array, const unsigned int tile_array_len,
                                     const unsigned short *palette, const unsigned int palette_len,
                                     const unsigned int palette_id, const core::gfx::SpriteSize size,
                                     const unsigned int tile_id);

                /// Loads grit-generated tile graphics data into this
                /// Sprite4Bpp with a [0-16] palette id. Internally calls the
                /// first array_load function.
                /// \param tile_array The grit-generated tile graphics
                /// buffer to load
                /// \param tile_array_len The length of the grit-generated
                /// tile graphics buffer
                /// \param palette_id The [0-16] palette id to set on this
                /// Sprite4Bpp
                /// \param size The SpriteSize to be set on this Sprite4Bpp
                /// \param tile_id The GBA-specific tile_id to load the
                /// tile graphics buffer into
                void load_from_array(const unsigned short *tile_array, const unsigned int tile_array_len,
                                     const unsigned int palette_id, const core::gfx::SpriteSize size,
                                     const unsigned int tile_id);

                /// Loads color palette data directly into the GBA's Sprite
                /// (or Object/OBJ) color palette. Internally calls the second
                /// array_load function.
                /// \param palette The color palette buffer to load in
                /// \param palette_len The length of the color palette buffer
                /// to load in
                /// \param pal_offset The [0-255] GBA Sprite color palette
                /// offset to load the palette buffer data into
                /// \return Whether the color palette data was sucessfully
                /// loaded.
                bool load_into_palette(const unsigned short *palette, const unsigned int palette_len,
                                       const unsigned int pal_offset = 256)override;
            protected:
                void array_load(const unsigned short *tile_array, const unsigned int tile_array_len,
                                const core::gfx::SpriteSize size, const unsigned int tile_id)override;

                void array_load(const unsigned short *tile_array, const unsigned int tile_array_len,
                                const unsigned short *palette, const unsigned int palette_len,
                                const core::gfx::SpriteSize size, const unsigned int tile_id)override;
            private:
                unsigned int m_palette_id;
                unsigned int m_tile_id;
            };

            /// \class morpheus::gba::gfx::Sprite4Bpp
            /// A 4BPP (4 bits per pixel or 16 color) implementation of the
            /// morpheus::gba::gfx::Sprite and
            /// morpheus::core::gfx::SpriteBase classes. Divides the single
            /// GBA Sprite palette of 256 colors into 16 palettes of 16 colors.
            /// For more information about this class, consult the
            /// aforementioned parent class and grandparent class.
        }
    }
}

#endif //MORPHEUS_GBA_TEST_SPRITE_4_BPP_HPP
