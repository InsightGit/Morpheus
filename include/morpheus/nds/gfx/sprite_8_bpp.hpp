//
// Created by bobby on 27/11/2020.
//

#ifndef MORPHEUS_NDS_SPRITE8BPP_HPP
#define MORPHEUS_NDS_SPRITE8BPP_HPP

#include <iostream>

#include <nds.h>


#include <morpheus/nds/gfx/sprite.hpp>

namespace morpheus {
    namespace nds {
        namespace gfx {
            class Sprite8Bpp : public nds::gfx::Sprite {
                public:
                    /// Constructs a Sprite8Bpp, or a 256 color sprite.
                    /// \param affine Whether this Sprite8Bpp is affine or not
                    /// \param use_sub_display Whether this Sprite8Bpp is
                    /// displayed on the bottom sub display (true) or the top
                    /// main display (false)
                    /// \param blending_controller The NdsBlendingController for
                    /// this Sprite8Bpp to use
                    /// \param mosaic_controller The NdsMosaicController for
                    /// this Sprite8Bpp to use
                    /// \param use_extended_palette Whether this Sprite8Bpp
                    /// uses extended palette mode (16 palettes of 256 colors,
                    /// only available on the DS), or single palette mode
                    /// (1 palette of 256 colors, available on the DS and the
                    /// GBA)
                    explicit Sprite8Bpp(bool affine, bool use_sub_display, NdsBlendingController *blending_controller,
                                        NdsMosaicController *mosaic_controller, bool use_extended_palette = false) :
                        nds::gfx::Sprite(affine, use_sub_display, blending_controller, mosaic_controller,
                                         SpriteMapping_1D_32,
                                         use_extended_palette ? ExtendedPaletteStatus::NEEDED :
                                                                ExtendedPaletteStatus::NEEDOFF) {}

                    /// Destructs the Sprite8Bpp object.
                    ~Sprite8Bpp() override = default;

                    // Single palette load functions

                    /// Loads grit-generated graphics tile data onto this
                    /// Sprite8Bpp if single palette mode is enabled. If
                    /// extended palette mode is enabled, this function
                    /// will not be able to successfully load the data in
                    /// and will return false.
                    /// \param tile_array The grit-generated graphics tile
                    /// buffer to load in.
                    /// \param tile_array_len The length of the grit-generated
                    /// graphics tile buffer to load in.
                    /// \param size The morpheus::core::gfx::SpriteSize to be
                    /// set on this Sprite8Bpp.
                    /// \return Whether the grit-generated graphics tile data
                    /// was succesfully loaded into this Sprite8Bpp.
                    bool load_from_array(const unsigned short *tile_array, const unsigned int tile_array_len,
                                         const morpheus::core::gfx::SpriteSize size);
                    /// Loads grit-generated graphics tile data onto this
                    /// Sprite8Bpp if single palette mode is enabled with a
                    /// color palette. If extended palette mode is enabled, this
                    /// function will not be able to successfully load the data
                    /// in and will return false.
                    /// \param tile_array The grit-generated graphics tile
                    /// buffer to load in.
                    /// \param tile_array_len The length of the grit-generated
                    /// graphics tile buffer to load in.
                    /// \param palette The color palette buffer to load in.
                    /// \param palette_len The length of the color palette
                    /// buffer to load in.
                    /// \param size The morpheus::core::gfx::SpriteSize to be
                    /// set on this Sprite8Bpp.
                    /// \return Whether the grit-generated graphics tile data
                    /// and palette was successfully loaded into this
                    /// Sprite8Bpp.
                    bool load_from_array(const unsigned short *tile_array, const unsigned int tile_array_len,
                                         const unsigned short *palette, const unsigned int palette_len,
                                         const morpheus::core::gfx::SpriteSize size);


                    bool load_into_palette(const unsigned short *palette, const unsigned int pal_len,
                                           const unsigned int pal_offset = 256) override {
                        return load_into_palette_with_id(palette, pal_len, 0, pal_offset);
                    }

                    // Extended palette load functions

                    /// Loads grit-generated graphics tile data onto this
                    /// Sprite8Bpp if extended palette mode is enabled with a
                    /// [0-16] palette id. If single palette mode is enabled,
                    /// this function will not be able to successfully load the
                    /// data in and will return false.
                    /// \param tile_array The grit-generated graphics tile
                    /// buffer to load in.
                    /// \param tile_array_len The length of the grit-generated
                    /// graphics tile buffer to load in.
                    /// \param palette_id The [0-16] color palette id for this
                    /// Sprite8Bpp to use.
                    /// \param size The morpheus::core::gfx::SpriteSize to be
                    /// set on this Sprite8Bpp.
                    /// \return Whether the grit-generated graphics tile data
                    /// was successfully loaded into this Sprite8Bpp.
                    bool load_from_array(const unsigned short *tile_array, const unsigned int tile_array_len,
                                         const unsigned int palette_id,
                                         const morpheus::core::gfx::SpriteSize size)override;

                    /// Loads grit-generated graphics tile data onto this
                    /// Sprite8Bpp if extended palette mode is enabled with a
                    /// palette and a [0-16] palette id. If single palette mode
                    /// is enabled, this function will not be able to
                    /// successfully load the data in and will return false.
                    /// \param tile_array The grit-generated graphics tile
                    /// buffer to load in.
                    /// \param tile_array_len The length of the grit-generated
                    /// graphics tile buffer to load in.
                    /// \param palette The color palette buffer to load in.
                    /// \param palette_len The length of the color palette
                    /// buffer to load in.
                    /// \param palette_id The [0-16] color palette id for this
                    /// Sprite8Bpp to use.
                    /// \param size The morpheus::core::gfx::SpriteSize to be
                    /// set on this Sprite8Bpp.
                    /// \return Whether the grit-generated graphics tile data
                    /// and palette was successfully loaded into this
                    /// Sprite8Bpp.
                    bool load_from_array(const unsigned short *tile_array, const unsigned int tile_array_len,
                                         const unsigned short *palette,
                                         const unsigned int palette_len, const unsigned int palette_id,
                                         const morpheus::core::gfx::SpriteSize size)override;

                    /// Loads palette data into a palette with a given
                    /// palette_id. If this DS is in single palette mode,
                    /// the palette_id will be ignored.
                    /// \param palette The color palette buffer to load in.
                    /// \param palette_len The length of the color palette
                    /// buffer to load in.
                    /// \param palette_id The [0-16] color palette id for this
                    /// Sprite8Bpp to use.
                    /// \param pal_offset The offset within the 256 color palette
                    /// to start loading the buffer into.
                    /// \return Whether the palette data was successfully loaded
                    /// into the corresponding DS sprite (or Object/OBJ) color
                    /// palette
                    bool load_into_palette_with_id(const unsigned short *palette, const unsigned int palette_len,
                                                   const unsigned int palette_id, const unsigned int pal_offset = 256);

                    /// Loads data from a 256 color pcx file into this
                    /// Sprite8Bpp. Note: It is recommended to use grit to
                    /// convert your images and then use load_from_array
                    /// to load the data as opposed to using pcx files
                    /// and loading with this function as this function
                    /// is only supported on the DS and with 256-color
                    /// sprites.
                    /// \param pcx_data The buffer of the pcx data to load in.
                    /// \param palette_id The optional palette id to load in
                    /// the pcx file's color palette data into.
                    /// \param copy_palette Whether to load in this pcx file's
                    /// palette or not.
                    /// \return Whether the tile and palette data was
                    /// successfully loaded into this Sprite8Bpp
                    bool load_from_pcx(const unsigned char *pcx_data, unsigned int palette_id = 0,
                                       bool copy_palette = true);
                protected:
                    void draw_node(std::vector<void *> &obj_attr_buffer, unsigned int obj_attr_num)override;
                private:
                    std::unique_ptr<sImage> m_sprite_image;
            };

            /// \class morpheus::nds::gfx::Sprite8Bpp
            /// An 8BPP (8 bits per pixel or 256 color) implementation of the
            /// morpheus::nds::gfx::Sprite and
            /// morpheus::core::gfx::SpriteBase classes. Uses the entire NDS
            /// Sprite (or Object/OBJ) palette as one single palette of
            /// 256 colors in single palette mode like on the GBA. It can also
            /// use a DS-specific extended palette mode however, to use 16
            /// palettes of 256 colors.
            /// For more information about this class, consult the
            /// aforementioned parent class and grandparent class.
        }
    }
}

#endif //MORPHEUS_NDS_SPRITE8BPP_HPP*/
