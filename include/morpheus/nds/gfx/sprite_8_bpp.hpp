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
                    explicit Sprite8Bpp(bool affine, bool use_sub_display, NdsBlendingController *blending_controller,
                                        NdsMosaicController *mosaic_controller, bool use_extended_palette = false) :
                        nds::gfx::Sprite(affine, use_sub_display, blending_controller, mosaic_controller,
                                         SpriteMapping_1D_32,
                                         use_extended_palette ? ExtendedPaletteStatus::NEEDED :
                                                                ExtendedPaletteStatus::NEEDOFF) {}

                    ~Sprite8Bpp() override = default;

                    // Single palette load functions
                    bool load_from_array(const unsigned short *tile_array, const unsigned int tile_array_len,
                                         const morpheus::core::gfx::SpriteSize size);
                    bool load_from_array(const unsigned short *tile_array, const unsigned int tile_array_len,
                                         const unsigned short *palette, const unsigned int palette_len,
                                         const morpheus::core::gfx::SpriteSize size);
                    bool load_into_palette(const unsigned short *palette, const unsigned int pal_len,
                                           const unsigned int pal_offset = 256) override {
                        return load_into_palette_with_id(palette, pal_len, 0, pal_offset);
                    }

                    // Extended palette load functions
                    bool load_from_array(const unsigned short *tile_array, const unsigned int tile_array_len,
                                         const unsigned int palette_id,
                                         const morpheus::core::gfx::SpriteSize size)override;
                    bool load_from_array(const unsigned short *tile_array, const unsigned int tile_array_len,
                                         const unsigned short *palette,
                                         const unsigned int palette_len, const unsigned int palette_id,
                                         const morpheus::core::gfx::SpriteSize size)override;
                    bool load_into_palette_with_id(const unsigned short *palette, const unsigned int palette_len,
                                                   const unsigned int palette_id, const unsigned int pal_offset = 256);

                    bool load_from_pcx(const unsigned char *pcx_data, unsigned int palette_id = 0,
                                       bool copy_palette = true);
                protected:
                    void draw_node(std::vector<void *> &obj_attr_buffer, unsigned int obj_attr_num)override;
                private:
                    std::unique_ptr<sImage> m_sprite_image;
            };
        }
    }
}

#endif //MORPHEUS_NDS_SPRITE8BPP_HPP*/
